
#include "savebossbdatacmd.h"
#include "dbcommoperate.h"
#include <db/autosqlbuf.h>
#include <framework/gsapi.h>
#include <sdutil.h>
#include <logic/player/playersavemgr.h>
#include <sdloggerimpl.h>

#include <net/db/dbprocessor.h>

CSaveBossBDataCMD::CSaveBossBDataCMD()
{
	memset(&m_stBossBData, 0, sizeof(m_stBossBData));
}


CSaveBossBDataCMD::~CSaveBossBDataCMD()
{

}

VOID CSaveBossBDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
	if(!SaveData(poDBConn))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CSaveBossBDataCMD SaveData failed: activityid = %u, bossid = %u, totalhp = %llu, curhp = %llu, startime = %llu, savetime = %llu"), MSG_MARK, 
            m_stBossBData.byActivityID, m_stBossBData.wBossID, m_stBossBData.qwTotalHP, m_stBossBData.qwCurHP, m_stBossBData.qwStartTime, m_stBossBData.qwSaveTime);
	}
}

BOOL CSaveBossBDataCMD::SaveData(SGDP::ISDDBConnection *poDBConn)
{
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

	//memset(pszBuff, 0, MAX_SQL_LEN);
	INT32 nRet = m_stBossBData.EncodeSaveData(pszBuff, MAX_SQL_LEN);
	if(nRet <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);
		return FALSE;
	}
	CHAR szBossBData[MAXENLEN_DT_BOSS_BATTLE_DATA * 2 + 1] = {0};
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
											nRet,
											szBossBData,
											sizeof(szBossBData));
	if(dwRetCode <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
		return FALSE;
	}	
	sprintf(pszSql, "insert into module (KeyID, KeyID2, Value, UpdateTime ) values ( '%u', '%u', '%s', now() ) on duplicate key update module.Value = values(module.Value), module.UpdateTime = values(module.UpdateTime);",
		EMDT_BossBattle, m_stBossBData.byActivityID ,szBossBData);
	UINT32 dwErr;
	string strError;
	nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
	if(nRet < 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql); 
		m_bSuccess = FALSE;
		return FALSE;
	}

	if(0 != m_stBossBData.qwOverTime)
	{
		CSDDateTime oDateTime(m_stBossBData.qwOverTime);
		tstring strTime = SDTimeToString(oDateTime);
		//插入LOG记录
		sprintf(pszSql, "insert into modulelog (KeyID, KeyID2, OverTime, Value, UpdateTime ) values ( '%u', '%u', '%s', '%s', now() ) on duplicate key update modulelog.Value = values(modulelog.Value), modulelog.UpdateTime = values(modulelog.UpdateTime);",
			EMDT_BossBattle, m_stBossBData.byActivityID , strTime.c_str(), szBossBData);
		nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
		if(nRet < 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql); 
			//m_bSuccess = FALSE;
			//return FALSE;//该数据不重要，不算失败
		}
	}

	m_bSuccess = TRUE;
	return TRUE;
}

VOID CSaveBossBDataCMD::OnExecuted()
{
	if(!m_bSuccess)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: save bossb data failed!"), MSG_MARK); 
	} 

	gsapi::GetDBProcessor()->OnSaveBossBDataAck(m_bSuccess, m_dwTransID);
}

VOID CSaveBossBDataCMD::SetUserData(VOID* pUserData)
{
	memcpy(&m_stBossBData, pUserData, sizeof(m_stBossBData));
}
