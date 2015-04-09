#include "setapppayinfo.h"
#include <db/autosqlbuf.h>
#include "logic/http/httpmgr.h"
#include "common/server/base64.h"
#include "logic/record/recordmgr.h"
#include "logic/player/playermgr.h"
#include "net/gt/gtpktbuilder.h"
#include "logic/http/httpmgr.h"
#include "logic/http/apppayhttpcmd.h"
#include <logic/player/player.h>


CSetAppPayInfo::CSetAppPayInfo()
{
    m_bInsertSuccess = FALSE;
    m_bRepeat = FALSE;
    m_dwAlreadyPayGoldValue = 0;
}

CSetAppPayInfo::~CSetAppPayInfo()
{

}

VOID SDAPI CSetAppPayInfo::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    SaveAppPayInfo( poDBConn );
}

VOID SDAPI CSetAppPayInfo::OnExecuted()
{
    //执行完毕，向充值管理器添加玩家的信息，以求到苹果的处理
    if( m_bInsertSuccess )
    {
        if( m_bRepeat )
        {
            //告诉请求的玩家，充值已经成功（请求的玩家和实际充值的玩家可能不是同一个用户）
            CPlayer *pNotifyPlayer = CPlayerMgr::Instance()->FindPlayer( m_stPayInfo.dwQueryPlayerID );

            if( pNotifyPlayer )
            {
                //发送通知
                CPlayer *pPayPlayer = CPlayerMgr::Instance()->FindPlayer( m_stPayInfo.dwPlayerID );
                if( pPayPlayer )
                {
                    //玩家在内存中，直接发送给客户端，告诉客户端验证已经成功
                    DT_PLAYER_BASE_DATA baseData = pPayPlayer->GetDT_PLAYER_BASE_DATA();
                    UINT32 dwMsgID = CGTPktBuilder::Instance()->OnAppPaySuccess( baseData.byVipLevel, baseData.dwTotalAmount, baseData.qwGold, m_dwAlreadyPayGoldValue, m_stPayInfo.strOrderID, m_stPayInfo.dwPlayerID);
                    if( dwMsgID == INVALID_MSGID )
                    {
                        return;
                    }
                    pNotifyPlayer->SendMsg( CGTPktBuilder::Instance()->GetPacketBuffer(), dwMsgID );
                }
                else
                {
                    //玩家不在内存中
                    UINT32 dwMsgID = CGTPktBuilder::Instance()->OnAppPaySuccess( 0, 0, 0, m_dwAlreadyPayGoldValue, m_stPayInfo.strOrderID, m_stPayInfo.dwPlayerID);
                    if( dwMsgID == INVALID_MSGID )
                    {
                        return;
                    }
                    pNotifyPlayer->SendMsg( CGTPktBuilder::Instance()->GetPacketBuffer(), dwMsgID );
                }

            }
            else
            {
                //被通知玩家不在内存中，输出一个警告信息
                SYS_CRITICAL( _SDT("[%s:%d]Query player pay, but can't find query player:%u in memory"), MSG_MARK, m_stPayInfo.dwQueryPlayerID );
            }

            return;

        }
        else
        {
            //char szData[5120]={0};
            //CAppPayMgr::Instance()->Add( m_stPayInfo.dwPlayerID, Base64Encode( szData, sizeof(szData), (UINT8 *)m_stPayInfo.strReceipt.c_str(),
            //	m_stPayInfo.strReceipt.size() ), m_stPayInfo.dwQueryPlayerID, m_stPayInfo.strOrderID, 0 );
            //CHttpMgr::Instance()->AddPay( m_stPayInfo.dwPlayerID, m_stPayInfo.strReceipt, m_stPayInfo.dwQueryPlayerID, m_stPayInfo.strOrderID, 0 );

            SAppPayInfo stPayInfo;
            stPayInfo.dwPlayerID		=	m_stPayInfo.dwPlayerID;
            stPayInfo.dwQueryPlayerID	=	m_stPayInfo.dwQueryPlayerID;
            stPayInfo.strReceipt		=	m_stPayInfo.strReceipt;
            stPayInfo.strOrderID		=	m_stPayInfo.strOrderID;
            stPayInfo.nQueryCnt		=	1;
            stPayInfo.bBuyEnv			=	TRUE;
            CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_APP_PAY);
            if (NULL == poHttpCmd)
            {
                SYS_CRITICAL( _SDT("[%s:%d]Query player pay, but can't CreateCmd player:%u in memory"), MSG_MARK, m_stPayInfo.dwQueryPlayerID );
                return;
            }
            poHttpCmd->SetUserData(&stPayInfo, sizeof(SAppPayInfo), 0);
            poHttpCmd->Init();
            if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
            {
                CHttpMgr::Instance()->FreeCmd(poHttpCmd);
                SYS_CRITICAL( _SDT("[%s:%d]Query player pay, but can't AddCommand player:%u in memory"), MSG_MARK, m_stPayInfo.dwQueryPlayerID );
            }
        }
    }
    else
    {
        SYS_CRITICAL( _SDT( "set player:%u order id error,can't post data to appserver" ), m_stPayInfo.dwPlayerID );
        CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( m_stPayInfo.dwQueryPlayerID );
        if( NULL == pPlayer )
        {
            //被通知玩家不在线，则不通知
            USR_INFO( _SDT("[%s:%d]Query player pay, but can't find query player:%u in memory"), MSG_MARK, m_stPayInfo.dwQueryPlayerID );
            return;
        }

        UINT16 wMsgID = CGTPktBuilder::Instance()->OnAppIDCheckError( ERR_CHECK_APPSTORE_ORDER::ID_SERVER_INNER_ERROR, m_stPayInfo.dwQueryPlayerID, m_stPayInfo.strOrderID);
        if( wMsgID == INVALID_MSGID )
        {
            return;
        }
        pPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    }
}

BOOL CSetAppPayInfo::SaveAppPayInfo(SGDP::ISDDBConnection* poDBConn)
{
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    //先判断在数据库中是否已经存在，如果存在则判断是否反复请求
    sprintf( pszSql, "select * from %s where OrderID='%s'", m_stPayInfo.strTableName.c_str(), m_stPayInfo.strOrderID.c_str() );
    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);

    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, m_stPayInfo.dwPlayerID, strError.c_str());
        return FALSE;
    }

    //如果订单数据已经到数据库
    if( (SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()) )
    {
        BOOL bProcess = BOOL( SDAtou( pRecordSet->GetFieldValueByName( "IsProcess" ) ) );
        if( bProcess )
        {
            //订单已经被处理完毕，客户端为重复请求，此时只需发送给客户端成功
            m_bRepeat = TRUE;
            m_bInsertSuccess = TRUE;
            m_dwAlreadyPayGoldValue = SDAtou( pRecordSet->GetFieldValueByName( "Amount" ) ) / 10; //分转换为元宝
        }
        else
        {
            //订单未被处理完毕，走订单插入完毕成功的流程
            m_bInsertSuccess = TRUE;
        }

        pRecordSet->Release();
        pRecordSet = NULL;
        return TRUE;
    }

    if( pRecordSet )
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    {
        CAutoSqlBuf oAutoSqlBuf2;
        CHAR* pszSql2 = oAutoSqlBuf2.GetSqlBuf();

        sprintf( pszSql2, "insert into %s( OrderID, DeviceID, UserID, ZoneID, Source, Amount, RMB, PlayerID, DateCreate, IsProcess, PlayerLevel ) values "
                 "('%s', '%s', %u, %u, '%s',0, 0, %u, now(), 0, %d )",
                 m_stPayInfo.strTableName.c_str(),
                 m_stPayInfo.strOrderID.c_str(),
                 m_stPayInfo.strDeviceID.c_str(),
                 m_stPayInfo.dwUserID,
                 m_stPayInfo.dwZoneID,
                 m_stPayInfo.strSource.c_str(),
                 m_stPayInfo.dwPlayerID,
                 m_stPayInfo.dwPlayerLevel);

        ISDDBRecordSet*pRecordSet2 = NULL;
        string strError;
        INT32 nRet = poDBConn->ExecuteSqlRs(pszSql2, &pRecordSet2, NULL, &strError);
        if(NULL != pRecordSet2)
        {
            pRecordSet2->Release();
            pRecordSet2 = NULL;
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, m_stPayInfo.dwPlayerID, strError.c_str());
            return FALSE;
        }
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, m_stPayInfo.dwPlayerID, strError.c_str());
            return FALSE;
        }

        m_bInsertSuccess = TRUE;
    }
    return TRUE;
}

VOID CSetAppPayInfo::SetUserData(VOID* pUserData)
{
    m_stPayInfo = *( (SPayInfo*)pUserData );
}


