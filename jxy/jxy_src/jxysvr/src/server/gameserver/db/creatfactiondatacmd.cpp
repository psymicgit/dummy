
#include "creatfactiondatacmd.h"
#include "dbcommoperate.h"
#include <db/autosqlbuf.h>
#include <sdutil.h>
#include <logic/player/playersavemgr.h>
#include <logic/player/playermgr.h>
#include <net/gt/gtpktbuilder.h>
#include <net/db/dbprocessor.h>
#include <sdloggerimpl.h>
#include <common/server/utility.h>
#include "framework/gsapi.h"
#include "logic/lua/luamgr.h"
CCreateFactionDataCMD::CCreateFactionDataCMD()
{
	m_wErrCode = 0;
    m_dwFactionID = 0;
    memset(&m_stReq, 0, sizeof(PKT_CLIGS_CREATE_FACTION_REQ));
}


CCreateFactionDataCMD::~CCreateFactionDataCMD()
{

}

VOID CCreateFactionDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    if(!CreateData(poDBConn))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCreateFactionDataCMD create faction failed: type = %u, name = %s, desc = %s"), MSG_MARK, m_stReq.wCreateFactionType, m_stReq.aszFactionName, m_stReq.aszFactionDescName);
    }
}

BOOL CCreateFactionDataCMD::CreateData(SGDP::ISDDBConnection *poDBConn)
{
	TCHAR aszFactionName[255] = {0};
	TCHAR aszFactionDescName[255] = {0};
    m_bSuccess = FALSE;
	m_wErrCode = ERR_COMMON::ID_OTHER_ERR;

    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql		= oAutoSqlBuf.GetSqlBuf();
    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

	poDBConn->EscapeString(m_stReq.aszFactionName, SDStrlen(m_stReq.aszFactionName), aszFactionName, 255);
	poDBConn->EscapeString(m_stReq.aszFactionDescName, SDStrlen(m_stReq.aszFactionDescName), aszFactionDescName, 255);
    memset(pszSql, 0, MAX_SQL_LEN);
    sprintf(pszSql, "call sp_CreateFaction( %u, %u, '%s', '%s');", gsapi::GetZoneID(), m_dwTransID, aszFactionName, aszFactionDescName);

    UINT32 dwErr;
    string strError;
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        m_bSuccess = FALSE;
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRecordSet && pRecordSet->GetRecord())// 当有行结果时
    {
        m_bSuccess = FALSE;
		m_wErrCode = SGDP::SDAtou(pRecordSet->GetFieldValueByName("errcode"));
		if (m_wErrCode != ERR_CREATE_FACTION::ID_SUCCESS)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: create faction failed ! sql:%s, errcode = %u, factionname = %s"), MSG_MARK, pszSql, m_wErrCode, m_stReq.aszFactionName);
			return FALSE;
		}
        m_dwFactionID = SGDP::SDAtou(pRecordSet->GetFieldValueByName("factionid"));
    }

    return TRUE;
}

VOID CCreateFactionDataCMD::OnExecuted()
{
    if(CDBProcessor::OnCreateFactionReq(m_dwTransID, m_stReq.wCreateFactionType, m_stReq.aszFactionName, m_stReq.aszFactionDescName, m_dwFactionID, m_wErrCode))
	{
		CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(m_dwTransID);
		if ( NULL != poPlayer)
		{
			if (0 == m_stReq.wCreateFactionType)
			{
				UINT32 dwCreateFactionCoin = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_FACTION_CREATE_BY_COIN, 0);
				poPlayer->DecCoin(dwCreateFactionCoin, CRecordMgr::EDCT_FACTION);
			}
			else
			{
				UINT32 dwCreateFactionGold = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_FACTION_CREATE_BY_GOLD, 0);
				poPlayer->DecGold(dwCreateFactionGold, CRecordMgr::EDGT_FACTION);
			}
		}
		
	}
}


VOID CCreateFactionDataCMD::SetUserData(VOID* pUserData)
{
    memcpy(&m_stReq, pUserData, sizeof(PKT_CLIGS_CREATE_FACTION_REQ));
}
