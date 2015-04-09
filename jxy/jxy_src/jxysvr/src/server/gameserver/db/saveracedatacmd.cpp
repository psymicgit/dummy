
#include <db/autosqlbuf.h>
#include <framework/gsapi.h>
#include "saveracedatacmd.h"
#include "dbsqlstring.h"
#include "dbcommoperate.h"
#include <sdutil.h>
#include "server/utility.h"
#include <logic/player/playersavemgr.h>
#include <sdloggerimpl.h>

#include <net/db/dbprocessor.h>

CSavePlayersRaceDataCMD::CSavePlayersRaceDataCMD()
{
	m_poPlayersRaceSaveInfo = NULL;
}


CSavePlayersRaceDataCMD::~CSavePlayersRaceDataCMD()
{
}

VOID CSavePlayersRaceDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
		
	poDBConn->BeginTransaction();
	if(SaveData(poDBConn))
	{
		poDBConn->CommitTransaction();
	}
	else
	{
		poDBConn->RollbackTransaction();
	}
}

VOID CSavePlayersRaceDataCMD::OnExecuted()
{
	
	if(!m_bSuccess)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: save plays plunder data failed!"), MSG_MARK);   
		m_poPlayersRaceSaveInfo->m_wErrCode = FAIL;
	}
	else
	{
		m_poPlayersRaceSaveInfo->m_wErrCode = SUCCESS;
	}

	gsapi::GetDBProcessor()->OnSavePlayersRaceDataAck(m_poPlayersRaceSaveInfo, m_dwTransID);


}

BOOL CSavePlayersRaceDataCMD::SaveData(ISDDBConnection* poDBConn)
{
	//CAutoCycle oAutoCycle(__FUNCTION__);

	CAutoSqlBuf oAutoSqlBuf;
	CAutoSqlBuf oAutoSaveEnDeBuf;
	CAutoSqlBuf oAutoTmp;
	CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
	CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
	CHAR* pszTmp = oAutoTmp.GetSqlBuf();
	if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
		return FALSE;
	}

	sprintf(pszSql, "insert into player_pvp (PlayerID, KeyID, Value, UpdateTime ) values ");
	INT32 nRet = 0;	
	vector<SRaceSaveData*>& vecRaceSaveData = m_poPlayersRaceSaveInfo->m_vecRaceSaveData;
	UINT32 dwSize = vecRaceSaveData.size();
	BOOL bFirst = TRUE;
	for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
	{
		SRaceSaveData* pstRaceSaveData = vecRaceSaveData[dwIdx];
		if(NULL == pstRaceSaveData)
		{
			continue;
		}
		INT32 nRet = 0;
		UINT32 dwPlayerID = pstRaceSaveData->dwPlayerID;
		if(bFirst)
		{
			ENCODEDATA22(pstRaceSaveData->stRaceData, RaceData, EMDT_RaceData, MAXENLEN_DT_RACE_BASE_DATA);
		}		
		else
		{
			ENCODEDATA2(pstRaceSaveData->stRaceData, RaceData, EMDT_RaceData, MAXENLEN_DT_RACE_BASE_DATA);
		}
		ENCODEDATA2(pstRaceSaveData->stRaceBattleLog, RaceBattleLog, EMDT_RaceBattleLog, MAXENLEN_DT_RACE_BATTLELOG_DATA_LIST);
		bFirst = FALSE;
	}
	strcat(pszSql, " on duplicate key update player_pvp.Value = values(player_pvp.Value), player_pvp.UpdateTime = values(player_pvp.UpdateTime);");	
	
	//memset(pszBuff, 0, MAX_SQL_LEN);
	for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
	{
		SRaceSaveData* pstRaceSaveData = vecRaceSaveData[dwIdx];
		if(NULL == pstRaceSaveData)
		{
			continue;
		}
		INT32 nRet = 0;
		UINT32 dwPlayerID = pstRaceSaveData->dwPlayerID;
		sprintf(pszBuff, "update players set Rank = '%u', RecvRankEncTime='%s' where PlayerID = '%u';", pstRaceSaveData->stRaceData.dwRank, SDTimeToString(pstRaceSaveData->stRaceData.qwRecvEncourageTime).c_str(), dwPlayerID);
		strcat(pszSql, pszBuff);
	}

	UINT32 dwErr;
	string strError;
	nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
	if(nRet < 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql); 
		m_bSuccess = FALSE;
		return FALSE;
	}

	m_bSuccess = TRUE;
	return TRUE;
}


VOID CSavePlayersRaceDataCMD::SetUserData(VOID* pUserData)
{
	m_poPlayersRaceSaveInfo = (CPlayersRaceSaveData*)(pUserData);
}

