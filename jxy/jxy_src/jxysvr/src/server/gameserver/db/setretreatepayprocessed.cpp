#include "setretreatepayprocessed.h"
#include <framework/gsapi.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <net/recharge/rcclient.h>
#include "common/client/errdef.h"
#include "common/server/utility.h"

CSetRecreatePayProcessed::CSetRecreatePayProcessed()
{
    m_bSuccess = FALSE;
}

CSetRecreatePayProcessed::~CSetRecreatePayProcessed()
{

}


VOID SDAPI CSetRecreatePayProcessed::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    poDBConn->BeginTransaction();
    if( SetPayProcessed( poDBConn ) )
    {
        poDBConn->CommitTransaction();
        CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(m_dwTransID);

        if (poClinet != NULL)
        {
            poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS).c_str());
        }
    }
    else
    {
        poDBConn->RollbackTransaction();

        CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(m_dwTransID);

        if (poClinet != NULL)
        {
            poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_DATA_SAVE_ERR).c_str());
        }
    }
}

/**
* @brief 同步执行SQL命令相关的逻辑部分
* @return VOID
*/
VOID SDAPI CSetRecreatePayProcessed::OnExecuted()
{
    if( !m_bSuccess )
    {
        return;
    }

    for (UINT32 i = 0; i < m_vecRecreatePayProcessed.size(); i++)
    {
        CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( m_vecRecreatePayProcessed[i].dwPlayerID );

        if( NULL == pPlayer )
        {
            //如果玩家不在线,则调出玩家,以设置数据
            CGetPlayerDataMgr::Instance()->GetPlayerData( m_vecRecreatePayProcessed[i].dwSessionID , GET_PLAYERDATATYPE_RETREATE, m_vecRecreatePayProcessed[i].dwPlayerID,
                    m_vecRecreatePayProcessed[i].dwPlayerID, m_vecRecreatePayProcessed[i].dwGold, m_vecRecreatePayProcessed[i].dwRMB, (char *)m_vecRecreatePayProcessed[i].strOrderID.c_str() );
        }
        else
        {
            //玩家在线，则增加元宝
            pPlayer->AddGold( m_vecRecreatePayProcessed[i].dwGold, CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, m_vecRecreatePayProcessed[i].strOrderID, m_vecRecreatePayProcessed[i].dwRMB);
        }
    }
}

//设置数据
VOID CSetRecreatePayProcessed::SetUserData(VOID* pUserData)
{
    m_vecRecreatePayProcessed = *(vector<SRecreatePayProcessed>*)pUserData;
}

BOOL CSetRecreatePayProcessed::SetPayProcessed(SGDP::ISDDBConnection* poDBConn)
{
    CHAR szSql[512] = {0};

    for(UINT32 i = 0; i < m_vecRecreatePayProcessed.size(); i++)
    {
        memset(szSql, 0x0, sizeof(szSql));
        //sprintf( szSql, "call sp_AddRecharge('%s', %d, %d, %d, %d, '%s',%d)", m_vecRecreatePayProcessed[i].strOrderID.c_str(), m_vecRecreatePayProcessed[i].dwPlayerID, m_vecRecreatePayProcessed[i].dwGold,m_vecRecreatePayProcessed[i].dwRMB,
        //	m_vecRecreatePayProcessed[i].bySandBoxFlag, m_vecRecreatePayProcessed[i].strComment.c_str(),m_vecRecreatePayProcessed[i].dwPayID);

        sprintf( szSql, "call sp_AddRecharge('%s', %d, %d, %d, %d, '%s')", m_vecRecreatePayProcessed[i].strOrderID.c_str(), m_vecRecreatePayProcessed[i].dwPlayerID, m_vecRecreatePayProcessed[i].dwGold, m_vecRecreatePayProcessed[i].dwRMB,
                 m_vecRecreatePayProcessed[i].bySandBoxFlag, m_vecRecreatePayProcessed[i].strComment.c_str());

        ISDDBRecordSet*pRecordSet = NULL;
        string strError;
        UINT32 dwErr = 0;
        INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet, &dwErr, &strError);

        if(SDDB_HAS_RECORDSET == nRet && pRecordSet->GetRecord())
        {
            m_bSuccess = TRUE;
            UINT32 wErrCode = SGDP::SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));

            if (0 == wErrCode)
            {
                m_bSuccess = TRUE;
            }
            else
            {
                m_bSuccess = FALSE;
            }

            if(NULL != pRecordSet)
            {
                pRecordSet->Release();
                pRecordSet = NULL;
            }

            if (!m_bSuccess)
            {
                return m_bSuccess;
            }
        }
        else
        {
            m_bSuccess = FALSE;

            if(NULL != pRecordSet)
            {
                pRecordSet->Release();
                pRecordSet = NULL;
            }

            SYS_CRITICAL(_SDT("[%s:%d]: exec sql failed! sql:%s, sqlerror:%s"), MSG_MARK, szSql, strError.c_str());

            return m_bSuccess;
        }
    }

    return TRUE;
}