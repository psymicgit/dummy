#include "leavefactiondatacmd.h"
#include "dbcommoperate.h"
#include <framework/gsapi.h>
#include <sdutil.h>
#include <logic/player/playersavemgr.h>
#include <logic/player/playermgr.h>
#include <net/gt/gtpktbuilder.h>

#include <logic/player/player.h>
#include <db/autosqlbuf.h>
#include <logic/faction/factionmgr.h>
#include <logic/rareitemroom/rareitemroompropmgr.h>
#include <logic/gymnasium/gymnasiumpropmgr.h>
#include <logic/factionshade/shadepropmgr.h>
#include <logic/hotspring/hotspringpropmgr.h>
CLeaveFactionDataCMD::CLeaveFactionDataCMD()
{
	m_wErrCode = 0;
	memset(&m_stReq, 0, sizeof(m_stReq));
}

CLeaveFactionDataCMD::~CLeaveFactionDataCMD()
{

}

VOID CLeaveFactionDataCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	if ( !SaveData(poDBConn))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CLeaveFactionDataCMD::SaveData failed"), MSG_MARK);
	}
}

VOID CLeaveFactionDataCMD::OnExecuted()
{
	PKT_CLIGS_LEAVE_FACTION_ACK stLeaveFactionAck;
	CFaction* poFaction = CFactionMgr::Instance()->GetFaction(m_dwTransID);
	if ( NULL == poFaction )
	{
		return ;
	}
	
	UINT32 dwPlayerID = poFaction->GetDT_FACTION_BASE_DATA().dwFactionPlayerID;
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL == poPlayer)
	{
		return ;
	}
	
	if ( ERR_LEAVE_FACTION::ID_SUCCESS != m_wErrCode)
	{
		stLeaveFactionAck.wErrCode = m_wErrCode;
		poPlayer->SendMsg( (char*)&stLeaveFactionAck, CLIGS_LEAVE_FACTION_ACK);
		return;
	}

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		stLeaveFactionAck.wErrCode = ERR_LEAVE_FACTION::ID_UNOPEN;
		poPlayer->SendMsg( (char*)&stLeaveFactionAck, CLIGS_LEAVE_FACTION_ACK);
		return;
	}
	poFactionPlayer->SetFindBackDoorsTribute();
	CFactionMgr::Instance()->DissolveFaction(poFaction);	
	//清理数据
	stLeaveFactionAck.wErrCode = ERR_LEAVE_FACTION::ID_SUCCESS;
	poPlayer->SendMsg( (char*)&stLeaveFactionAck, CLIGS_LEAVE_FACTION_ACK);

	PKT_CLIGS_HOME_BUILD_PROMPT_NTF stNtf;
	memset(&stNtf, 0, sizeof(PKT_CLIGS_HOME_BUILD_PROMPT_NTF));
	poPlayer->GetFactionPrompt(stNtf.stPromptInfo.astBuildPromptInfo[stNtf.stPromptInfo.byBuildNum++]);
	poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_HOME_BUILD_PROMPT_NTF);

}


VOID CLeaveFactionDataCMD::SetUserData(VOID* pUserData)
{
	memcpy(&m_stReq, pUserData, sizeof(m_stReq));
}

BOOL CLeaveFactionDataCMD::SaveData(ISDDBConnection* poDBConn)
{
	m_bSuccess = TRUE;
	m_wErrCode = ERR_COMMON::ID_OTHER_ERR;

	CAutoSqlBuf oAutoSqlBuf;
	CHAR* pszSql		= oAutoSqlBuf.GetSqlBuf();
	if((NULL == pszSql))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
		return FALSE;
	}

	memset(pszSql, 0, MAX_SQL_LEN);
	sprintf(pszSql, "delete from faction where FactionID = %u; delete from factionplayer where FactionID = %u;", m_dwTransID, m_dwTransID);
	UINT32 dwErr;
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
	if(SDDB_NO_RECORDSET != nRet)
	{
		m_bSuccess = FALSE;
		SYS_CRITICAL(_SDT("[%s: %d]: delete  faction data error!, SQL: %s"), MSG_MARK, pszSql);
		return FALSE;
	}

	m_wErrCode = ERR_LEAVE_FACTION::ID_SUCCESS;
	return TRUE;
}
