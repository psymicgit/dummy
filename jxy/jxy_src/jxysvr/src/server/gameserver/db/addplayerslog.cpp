#include "addplayerslog.h"
#include "db/autosqlbuf.h"

CAddPlayersLogCMD::CAddPlayersLogCMD()
{
	memset(&m_stLstData, 0, sizeof(m_stLstData));
}

CAddPlayersLogCMD::~CAddPlayersLogCMD()
{
	memset(&m_stLstData, 0, sizeof(m_stLstData));
}

VOID CAddPlayersLogCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	m_bSuccess = TRUE;
	if ( 0 == m_stLstData.byLogNum)
	{
		return ;
	}

	CAutoSqlBuf oAutoSqlBuf;
	CAutoSqlBuf oAutoSql;
	CAutoSqlBuf oAutoTmp;
	CHAR* pszTmp		= oAutoTmp.GetSqlBuf();
	CHAR* pszSql		= oAutoSql.GetSqlBuf();
	CHAR* pszBuf        = oAutoSqlBuf.GetSqlBuf();
	if((NULL == pszSql))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
		return ;
	}

	INT32 nRet = 0;

	memset(pszSql, 0, MAX_SQL_LEN);
	sprintf(pszSql, "insert into players_log (PlayerID, ModuleID, Value, LogTime) values");
	
	for (UINT8 byIdx = 0; byIdx < m_stLstData.byLogNum; byIdx++)
	{
		if ( 0 != byIdx)
		{
			SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ",");
		}
		memset(pszBuf, 0, MAX_SQL_LEN);

		
		memset(pszTmp, 0, MAX_SQL_LEN);
		memset(pszBuf, 0, MAX_SQL_LEN);
		nRet = m_stLstData.astLogData[byIdx].EncodeSaveData(pszBuf, MAX_SQL_LEN);
		if(nRet <= 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]:  m_stLstData.astLogData[byIdx].EncodeSaveData is failed"), MSG_MARK);
			return ;
		}
		nRet = poDBConn->EscapeString(pszBuf,
			nRet,
			pszTmp,
			MAX_SQL_LEN);
		if(nRet <= 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
			return ;
		}
		CSDDateTime oLogTime(m_stLstData.astLogData[byIdx].qwLogTime);
		SDSnprintf(pszBuf, MAX_SQL_LEN - 1, "(%lu, %u, '%s', '%s')", 
			m_stLstData.astLogData[byIdx].dwPlayerID,
			m_stLstData.astLogData[byIdx].byLogBigType,
			pszTmp, oLogTime.ToString().c_str() );
		SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuf);
	}
	UINT32 dwErr;
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
	if(SDDB_SUCCESS > nRet)
	{
		m_bSuccess = FALSE;
		SYS_CRITICAL(_SDT("[%s: %d]: insert into players_log error!, SQL: %s, Error: %s"), MSG_MARK, pszSql, strError.c_str());
		return ;
	}
}

VOID CAddPlayersLogCMD::OnExecuted()
{

}

VOID CAddPlayersLogCMD::SetUserData(VOID* pUserData)
{
	memcpy(&m_stLstData, pUserData, sizeof(m_stLstData));
}