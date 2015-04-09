#include "setmypayprocessed.h"
#include <framework/gsapi.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <net/recharge/rcclient.h>

CSetMyPayProcessed::CSetMyPayProcessed()
{
    m_bSuccess = FALSE;
}

CSetMyPayProcessed::~CSetMyPayProcessed()
{

}


VOID SDAPI CSetMyPayProcessed::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
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
VOID SDAPI CSetMyPayProcessed::OnExecuted()
{
    //暂时将赠送的元宝算充值进度
    UINT32 dwGold = m_stMyPayProcessed.dwRMB / 10 + m_stMyPayProcessed.dwGivenGold;

    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(m_stMyPayProcessed.dwSessionID);
    if(poClinet && 0 != m_stMyPayProcessed.wErrCode)
    {
        poClinet->SendPayRet(m_stMyPayProcessed.strOrderID, m_stMyPayProcessed.wErrCode, NULL, dwGold);
        return;
    }
    else if(NULL == poClinet)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: SetMyPayProcessed find CRCClient[%d] OrderID[%s] failed!"), MSG_MARK, m_stMyPayProcessed.dwSessionID, m_stMyPayProcessed.strOrderID.c_str());
    }


    if( !m_bSuccess )
    {
        return;
    }

    CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( m_stMyPayProcessed.dwPlayerID );
    if( NULL == pPlayer )
    {
        //如果玩家不在线,则调出玩家,以设置数据
        CGetPlayerDataMgr::Instance()->GetPlayerData( m_stMyPayProcessed.dwSessionID, GET_PLAYERDATATYPE_EXCHANGE, m_stMyPayProcessed.dwPlayerID,
                m_stMyPayProcessed.dwPlayerID, dwGold, m_stMyPayProcessed.dwRMB, (char *)m_stMyPayProcessed.strOrderID.c_str() );
        return;
    }

    //玩家在线，则增加元宝
    pPlayer->AddGold( dwGold, CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, m_stMyPayProcessed.strOrderID, m_stMyPayProcessed.dwRMB);
    _SDTStrcpy( pPlayer->GetLastOrderID(), m_stMyPayProcessed.strOrderID.c_str() );

    //并且通知玩家
    CPlayer *pNotifyPlayer = CPlayerMgr::Instance()->FindPlayer( m_stMyPayProcessed.dwPlayerID );
    if( !pNotifyPlayer )
    {
        //被通知玩家不在线，则不通知
        USR_INFO( _SDT("[%s:%d]Query player pay, but can't find query player:%u in memory"), MSG_MARK, m_stMyPayProcessed.dwPlayerID );
        return;
    }

    poClinet->SendPayRet(m_stMyPayProcessed.strOrderID, m_stMyPayProcessed.wErrCode, pNotifyPlayer, dwGold);
}

//设置数据
VOID CSetMyPayProcessed::SetUserData(VOID* pUserData)
{
    m_stMyPayProcessed = *(SMyPayProcessed*)pUserData;
}

BOOL CSetMyPayProcessed::SetPayProcessed(SGDP::ISDDBConnection* poDBConn)
{

    CHAR szSql[512] = {0};

    sprintf( szSql, "insert into  myrecharge(CreateTime, ZoneID, OrderID, PlayerID, RMB, GiveGold, Comment, SandBoxFlag) values "\
             "( now(), '%u', '%s', '%u', '%u', '%u', '%s', '%u' ) on duplicate key update myrecharge.Comment = values(myrecharge.Comment);",
             m_stMyPayProcessed.wZoneID, m_stMyPayProcessed.strOrderID.c_str(), m_stMyPayProcessed.dwPlayerID, m_stMyPayProcessed.dwRMB / 100,
             m_stMyPayProcessed.dwGivenGold, m_stMyPayProcessed.strComment.c_str(), m_stMyPayProcessed.bySandBoxFlag);

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    UINT32 dwEffectRowNum = 0;
    INT32 nRet = poDBConn->ExecuteSql(szSql, NULL, &dwEffectRowNum, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        m_stMyPayProcessed.wErrCode = 3;//db处理失败
        SYS_CRITICAL(_SDT("[%s: %d]: SetPayProcessed exec sql failed! error:%s"), MSG_MARK, strError.c_str());
        return FALSE;
    }
    else if(0 == dwEffectRowNum)
    {
        m_stMyPayProcessed.wErrCode = 5;//已经处理
    }

    m_bSuccess = TRUE;

    return TRUE;
}