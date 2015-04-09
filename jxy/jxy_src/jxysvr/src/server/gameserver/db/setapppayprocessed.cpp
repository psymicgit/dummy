#include "setapppayprocessed.h"
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <sdloggerimpl.h>

CSetAppPayProcessed::CSetAppPayProcessed()
{
    m_bSuccess = FALSE;
}

CSetAppPayProcessed::~CSetAppPayProcessed()
{

}


VOID SDAPI CSetAppPayProcessed::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    poDBConn->BeginTransaction();
    if( SetPayProcessed( poDBConn ) )
    {
        poDBConn->CommitTransaction();
    }
    else
    {
        poDBConn->RollbackTransaction();
    }
}

/**
* @brief 同步执行SQL命令相关的逻辑部分
* @return VOID
*/
VOID SDAPI CSetAppPayProcessed::OnExecuted()
{
    if( !m_bSuccess )
    {
        return;
    }

    //数据处理成功，增加元宝，通知客户端（如果接收元宝的玩家和接收通知的玩家不是同一个，那么则需要分开取数据）
    CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( m_stAppPayProcessed.dwPlayerID );
    if( NULL == pPlayer )
    {

        //如果玩家不在线,则调出玩家,以设置数据
        CGetPlayerDataMgr::Instance()->GetPlayerData(0 , GET_PLAYERDATATYPE_EXCHANGE, m_stAppPayProcessed.dwPlayerID,
                m_stAppPayProcessed.dwQueryPlayer, m_stAppPayProcessed.dwGold, m_stAppPayProcessed.dwRMB, (char *)m_stAppPayProcessed.strOrderID.c_str() );
        return;
    }

    //玩家在线，则增加元宝
    pPlayer->AddGold( m_stAppPayProcessed.dwGold, CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, m_stAppPayProcessed.strOrderID, m_stAppPayProcessed.dwRMB);
    _SDTStrcpy( pPlayer->GetLastOrderID(), m_stAppPayProcessed.strOrderID.c_str() );

    //并且通知玩家
    CPlayer *pNotifyPlayer = CPlayerMgr::Instance()->FindPlayer( m_stAppPayProcessed.dwQueryPlayer );
    if( !pNotifyPlayer )
    {
        //被通知玩家不在线，则不通知
        USR_INFO( _SDT("[%s:%d]Query player pay, but can't find query player:%u in memory"), MSG_MARK, m_stAppPayProcessed.dwQueryPlayer );
        return;
    }

    UINT32 dwMsgID = INVALID_MSGID;
    //基础数据（如果被通知玩家和充值玩家不是同一个，那么这么做也没关系，因为如果不是同一个，那么客户端不提示具体的充值情况，所以可以随便赋值）
    DT_PLAYER_BASE_DATA stDT_PLAYER_BASE_DATA = pNotifyPlayer->GetDT_PLAYER_BASE_DATA();
    dwMsgID = CGTPktBuilder::Instance()->OnAppPaySuccess( stDT_PLAYER_BASE_DATA.byVipLevel, stDT_PLAYER_BASE_DATA.dwTotalAmount, stDT_PLAYER_BASE_DATA.qwGold, m_stAppPayProcessed.dwGold, m_stAppPayProcessed.strOrderID, stDT_PLAYER_BASE_DATA.dwID);
    if( dwMsgID == INVALID_MSGID )
    {
        return;
    }

    pNotifyPlayer->SendMsg( CGTPktBuilder::Instance()->GetPacketBuffer(), dwMsgID );
}

//设置数据
VOID CSetAppPayProcessed::SetUserData(VOID* pUserData)
{
    m_stAppPayProcessed = *(SAppPayProcessed*)pUserData;
}

BOOL CSetAppPayProcessed::SetPayProcessed(SGDP::ISDDBConnection* poDBConn)
{

    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

// 	sprintf( pszSql, "select * from %s where OrderID='%s'", m_stAppPayProcessed.strTableName.c_str(), m_stAppPayProcessed.strOrderID.c_str() );
// 	ISDDBRecordSet *pRecordSetSelect=NULL;
// 	string strErrorSelect;
// 	INT32 nRetSelect = poDBConn->ExecuteSqlRs(pszSql, &pRecordSetSelect, NULL, &strErrorSelect);
//
    CDBLock lock( poDBConn, m_stAppPayProcessed.strTableName );

    {
        CAutoSqlBuf oAutoSqlBufSelect;
        CHAR *pszSqlSelect = oAutoSqlBufSelect.GetSqlBuf();
        if( !pszSqlSelect )
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
            return FALSE;
        }

        ISDDBRecordSet*pRecordSetSelect = NULL;
        sprintf( pszSqlSelect, "select * from %s where OrderID='%s'", m_stAppPayProcessed.strTableName.c_str(), m_stAppPayProcessed.strOrderID.c_str() );
        string strErrorSelect;
        INT32 nRet = poDBConn->ExecuteSqlRs(pszSqlSelect, &pRecordSetSelect, NULL, &strErrorSelect );
        if( SDDB_HAS_RECORDSET == nRet && pRecordSetSelect->GetRecord() )
        {
            BOOL bProcessed = BOOL( SDAtou( pRecordSetSelect->GetFieldValueByName( "IsProcess" ) ) );

            pRecordSetSelect->Release();
            //如果已经处理过，则不重复处理
            if( bProcessed )
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }

    sprintf( pszSql, "update %s set DateProcess=now(), Amount='%u', RMB='%u', IsProcess=1, PayBuyEvn=%u where OrderID='%s' and IsProcess=0",
             m_stAppPayProcessed.strTableName.c_str(), m_stAppPayProcessed.dwGold, m_stAppPayProcessed.dwRMB, m_stAppPayProcessed.bBuyEnv, m_stAppPayProcessed.strOrderID.c_str() );

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK,  m_stAppPayProcessed.dwPlayerID, strError.c_str());

        return FALSE;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, m_stAppPayProcessed.dwPlayerID, strError.c_str());
        return FALSE;
    }

    m_bSuccess = TRUE;
    return TRUE;
}