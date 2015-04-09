
#include <db/autosqlbuf.h>
#include <framework/gsapi.h>
#include "saveplunderdatacmd.h"
#include "dbsqlstring.h"
#include "dbcommoperate.h"
#include <sdutil.h>
#include "server/utility.h"
#include <logic/player/playersavemgr.h>
#include <sdloggerimpl.h>

#include <net/db/dbprocessor.h>

CSavePlayersPlunderDataCMD::CSavePlayersPlunderDataCMD()
{
	m_poPlayersPlunderSaveInfo = NULL;
}


CSavePlayersPlunderDataCMD::~CSavePlayersPlunderDataCMD()
{
}

VOID CSavePlayersPlunderDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
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

VOID CSavePlayersPlunderDataCMD::OnExecuted()
{
	
	if(!m_bSuccess)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: save plays plunder data failed!"), MSG_MARK);   
		m_poPlayersPlunderSaveInfo->m_wErrCode = FAIL;
	}
	else
	{
		m_poPlayersPlunderSaveInfo->m_wErrCode = SUCCESS;
	}

	gsapi::GetDBProcessor()->OnSavePlayersPlunderDataAck(m_poPlayersPlunderSaveInfo, m_dwTransID);


}

BOOL CSavePlayersPlunderDataCMD::SaveData(ISDDBConnection* poDBConn)
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
	vector<SPlunderSaveData*>& vecPlunderSaveData = m_poPlayersPlunderSaveInfo->m_vecPlunderSaveData;
	UINT32 dwSize = vecPlunderSaveData.size();
	BOOL bFirst = TRUE;
	for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
	{
		SPlunderSaveData* pstPlayerData = vecPlunderSaveData[dwIdx];
		if(NULL == pstPlayerData)
		{
			continue;
		}
		INT32 nRet = 0;
		UINT32 dwPlayerID = pstPlayerData->dwPlayerID;
		if(bFirst)
		{
			ENCODEDATA22(pstPlayerData->stPlunderBaseData, PlunderBaseData, EMDT_PlunderBaseData, MAXENLEN_DT_PLUNDER_BASE_DATA);
		}
		else
		{
			ENCODEDATA2(pstPlayerData->stPlunderBaseData, PlunderBaseData, EMDT_PlunderBaseData, MAXENLEN_DT_PLUNDER_BASE_DATA);
		}
		ENCODEDATA2(pstPlayerData->stPlunderBattleLog, PlunderBattleLog, EMDT_PlunderBattleLog, MAXENLEN_DT_PLUNDER_BATTLELOG_DATA_LIST);
		ENCODEDATA2(pstPlayerData->stCaptureData, CaptureData, EMDT_CaptureData, MAXENLEN_DT_CAPTURE_DATA_LIST);
		ENCODEDATA2(pstPlayerData->stEnemyData, EnemyData, EMDT_EnemyData, MAXENLEN_DT_ENEMY_DATA_LIST);	
		bFirst = FALSE;
	}
	strcat(pszSql, " on duplicate key update player_pvp.Value = values(player_pvp.Value), player_pvp.UpdateTime = values(player_pvp.UpdateTime);");	
	 



	//memset(pszBuff, 0, MAX_SQL_LEN);
	for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
	{
		SPlunderSaveData* pstPlayerData = vecPlunderSaveData[dwIdx];
		if(NULL == pstPlayerData)
		{
			continue;
		}

		CSDDateTime		oDateTime(pstPlayerData->stPlunderBaseData.qwBeCaptureTime);
		tstring			strDateTime = SDTimeToString(oDateTime);

		INT32 nRet = 0;
		UINT32 dwPlayerID = pstPlayerData->dwPlayerID;
		sprintf(pszBuff, "update players set CaptureOwnerID = '%u', BeCaptureOwnerTime= '%s', OpenPlunder = '1' where PlayerID = '%u';", pstPlayerData->stPlunderBaseData.dwCaptureOwnerPlayerID, strDateTime.c_str(), dwPlayerID);
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


VOID CSavePlayersPlunderDataCMD::SetUserData(VOID* pUserData)
{
	m_poPlayersPlunderSaveInfo = (CPlayersPlunderSaveData*)(pUserData);
}

