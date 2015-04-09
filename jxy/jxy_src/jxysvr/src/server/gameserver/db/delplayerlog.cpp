#include "delplayerlog.h"
#include "db/autosqlbuf.h"

CDelPlayerLogCMD::CDelPlayerLogCMD()
{
	m_qwKey = 0;
}

CDelPlayerLogCMD::~CDelPlayerLogCMD()
{
	m_qwKey = 0;
}

VOID CDelPlayerLogCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	m_bSuccess = TRUE;

	CAutoSqlBuf oAutoSqlBuf;
	CHAR* pszSql		= oAutoSqlBuf.GetSqlBuf();
	if((NULL == pszSql))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
		return ;
	}
	UINT32 dwPlayerID = m_qwKey >> 32;
	UINT8 byModule = m_qwKey & 0xFF;
	memset(pszSql, 0, MAX_SQL_LEN);
	sprintf(pszSql, "delete from players_log where PlayerID=%u and ModuleID=%u", dwPlayerID, byModule);
	UINT32 dwErr;
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
	if(nRet < SDDB_SUCCESS)
	{
		m_bSuccess = FALSE;
		SYS_CRITICAL(_SDT("[%s: %d]: delete  players_log data error!, SQL: %s"), MSG_MARK, pszSql);
	}
}

VOID CDelPlayerLogCMD::OnExecuted()
{

}

VOID CDelPlayerLogCMD::SetUserData(VOID* pUserData)
{
	m_qwKey = *((UINT64*)pUserData);
}