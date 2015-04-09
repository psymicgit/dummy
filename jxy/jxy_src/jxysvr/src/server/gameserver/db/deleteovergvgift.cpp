#include "db/deleteovergvgift.h"
#include "db/autosqlbuf.h"

CDeleteOverGvGift::CDeleteOverGvGift()
{
	m_wSessionID = 0;
}

CDeleteOverGvGift::~CDeleteOverGvGift()
{
	m_wSessionID = 0;
}

VOID CDeleteOverGvGift::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	m_bSuccess = TRUE;

	CAutoSqlBuf oAutoSqlBuf;
	CHAR* pszSql		= oAutoSqlBuf.GetSqlBuf();
	if((NULL == pszSql))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
		return ;
	}

	memset(pszSql, 0, MAX_SQL_LEN);
	sprintf(pszSql, "delete from gvggift where SessionID <= %u", m_wSessionID);
	UINT32 dwErr;
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
	if(SDDB_NO_RECORDSET != nRet)
	{
		m_bSuccess = FALSE;
		SYS_CRITICAL(_SDT("[%s: %d]: delete  gvggift data error!, SQL: %s"), MSG_MARK, pszSql);
		
	}

}

VOID CDeleteOverGvGift::OnExecuted()
{

}

VOID CDeleteOverGvGift::SetUserData(VOID* pUserData)
{
	m_wSessionID = *((UINT16*)pUserData);
}