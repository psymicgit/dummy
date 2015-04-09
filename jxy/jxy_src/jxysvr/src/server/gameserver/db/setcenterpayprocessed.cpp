#include "setcenterpayprocessed.h"
#include <framework/gsapi.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <net/recharge/rcclient.h>

CSetCenterPayProcessed::CSetCenterPayProcessed()
{
    m_bSuccess = FALSE;
}

CSetCenterPayProcessed::~CSetCenterPayProcessed()
{

}


VOID SDAPI CSetCenterPayProcessed::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
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
VOID SDAPI CSetCenterPayProcessed::OnExecuted()
{
    //暂时将赠送的元宝算充值进度
    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(m_stCenterPayProcessed.dwSessionID);
    if(poClinet && 0 != m_stCenterPayProcessed.wErrCode)
    {
        poClinet->SendPayRet(m_stCenterPayProcessed.strOrderID, m_stCenterPayProcessed.wErrCode, NULL, m_stCenterPayProcessed.dwGold);
        return;
    }
	else if(NULL == poClinet)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: SetMyPayProcessed find CRCClient[%d] OrderID[%s] failed!"), MSG_MARK, m_stCenterPayProcessed.dwSessionID, m_stCenterPayProcessed.strOrderID.c_str());
	}

    if( !m_bSuccess )
    {
        return;
    }


    CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( m_stCenterPayProcessed.dwPlayerID );
    if( NULL == pPlayer )
    {
        //如果玩家不在线,则调出玩家,以设置数据
        CGetPlayerDataMgr::Instance()->GetPlayerData( m_stCenterPayProcessed.dwSessionID , GET_PLAYERDATATYPE_EXCHANGE, m_stCenterPayProcessed.dwPlayerID,
                m_stCenterPayProcessed.dwPlayerID, m_stCenterPayProcessed.dwGold, m_stCenterPayProcessed.dwRMB, (char *)m_stCenterPayProcessed.strOrderID.c_str() );
        return;
    }

    //玩家在线，则增加元宝
    pPlayer->AddGold( m_stCenterPayProcessed.dwGold, CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, m_stCenterPayProcessed.strOrderID, m_stCenterPayProcessed.dwRMB);

    //设置VIP等级
    //pPlayer->SetVipLevel(m_stCenterPayProcessed.byVipLevel);

    if (poClinet)
    {
        //直接给应答
        poClinet->SendPayRet(m_stCenterPayProcessed.strOrderID, m_stCenterPayProcessed.wErrCode, pPlayer, m_stCenterPayProcessed.dwGold);
    }


    //_SDTStrcpy( pPlayer->GetLastOrderID(), m_stCenterPayProcessed.strOrderID.c_str() );

    ////通知玩家
    //UINT32 dwMsgID = INVALID_MSGID;
    ////基础数据（如果被通知玩家和充值玩家不是同一个，那么这么做也没关系，因为如果不是同一个，那么客户端不提示具体的充值情况，所以可以随便赋值）
    //DT_PLAYER_BASE_DATA stDT_PLAYER_BASE_DATA = pPlayer->GetDT_PLAYER_BASE_DATA();
    //dwMsgID = CGTPktBuilder::Instance()->OnAppPaySuccess( stDT_PLAYER_BASE_DATA.byVipLevel, stDT_PLAYER_BASE_DATA.dwTotalAmount, stDT_PLAYER_BASE_DATA.qwGold, m_stCenterPayProcessed.dwGold, m_stCenterPayProcessed.strOrderID, stDT_PLAYER_BASE_DATA.dwID);
    //if( dwMsgID == INVALID_MSGID )
    //{
    //    return;
    //}

    //pPlayer->SendMsg( CGTPktBuilder::Instance()->GetPacketBuffer(), dwMsgID );
}

//设置数据
VOID CSetCenterPayProcessed::SetUserData(VOID* pUserData)
{
    m_stCenterPayProcessed = *(SCenterPayProcessed*)pUserData;
}

BOOL CSetCenterPayProcessed::SetPayProcessed(SGDP::ISDDBConnection* poDBConn)
{

    CHAR szSql[512] = {0};

    //sprintf( szSql, "call sp_AddRecharge('%s', %d, %d, %d, %d, '%s', %d)", m_stCenterPayProcessed.strOrderID.c_str(), m_stCenterPayProcessed.dwPlayerID, m_stCenterPayProcessed.dwGold, m_stCenterPayProcessed.dwRMB,
    //         m_stCenterPayProcessed.bySandBoxFlag, m_stCenterPayProcessed.strComment.c_str(), m_stCenterPayProcessed.dwPayID);

    sprintf( szSql, "call sp_AddRecharge('%s', %d, %d, %d, %d, '%s')", m_stCenterPayProcessed.strOrderID.c_str(), m_stCenterPayProcessed.dwPlayerID, m_stCenterPayProcessed.dwGold, m_stCenterPayProcessed.dwRMB,
             m_stCenterPayProcessed.bySandBoxFlag, m_stCenterPayProcessed.strComment.c_str());
	USR_INFO("[%s:%u] PaySql: %s", MSG_MARK, szSql);
    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    UINT32 dwErr = 0;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet, &dwErr, &strError);
    if(SDDB_HAS_RECORDSET == nRet && pRecordSet->GetRecord())
    {
        m_bSuccess = TRUE;
        m_stCenterPayProcessed.wErrCode = SGDP::SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
    }
    else
    {
        m_bSuccess = FALSE;
        m_stCenterPayProcessed.wErrCode = 99;
        SYS_CRITICAL(_SDT("[%s:%d]: exec sql failed! sql:%s, sqlerror:%s"), MSG_MARK, szSql, strError.c_str());
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}