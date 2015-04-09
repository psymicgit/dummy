#include "setreadprivatemsgcmd.h"
#include "db/autosqlbuf.h"
CSetReadPrivateMsgCMD::CSetReadPrivateMsgCMD()
{

}

CSetReadPrivateMsgCMD::~CSetReadPrivateMsgCMD()
{

}

VOID CSetReadPrivateMsgCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	m_bSuccess = TRUE;

	CAutoSqlBuf oAutoSqlBuf;
	CAutoSqlBuf oAutoTmpBuf;
	CHAR* pszSql		= oAutoSqlBuf.GetSqlBuf();
	CHAR* pszTmp = oAutoTmpBuf.GetSqlBuf();

	if((NULL == pszSql) || (NULL == pszTmp))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
		return ;
	}
	
	memset(pszSql, 0, MAX_SQL_LEN);
	SDStrcpy(pszSql, "update `private_chat_log` set isRead=1 where ");
	UINT32 dwSendPlayerID = 0;
	UINT32 dwRecvPlayerID = 0;
	for ( C64SetItr itr = m_setReadID.begin(); itr != m_setReadID.end(); itr++)
	{
		if ( itr != m_setReadID.begin() )
		{
			SDStrcpy(pszTmp, " or ");
			SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszTmp);
		}
		dwSendPlayerID = (*itr) >> 32;
		dwRecvPlayerID = (*itr) & 0xFFFFFFFF;
		SDSprintf(pszTmp, "(sendPid=%u and recvPid=%u)", dwSendPlayerID, dwRecvPlayerID);
		SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszTmp);

	}
	UINT32 dwErr;
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
	if(nRet < SDDB_SUCCESS)
	{
		m_bSuccess = FALSE;
		SYS_CRITICAL(_SDT("[%s: %d]: update  private_chat_log data error!, SQL: %s"), MSG_MARK, pszSql);
	}
}

	/**
	* @brief 同步执行SQL命令相关的逻辑部分
	* @return VOID
	*/
VOID CSetReadPrivateMsgCMD::OnExecuted()
{

}

VOID CSetReadPrivateMsgCMD::SetUserData(VOID* pUserData)
{
	m_setReadID = *(C64Set*)pUserData;
}
