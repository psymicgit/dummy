#include "db/addplayergift.h"
#include "db/autosqlbuf.h"
CAddPlayerGiftCMD::CAddPlayerGiftCMD()
{
	
}
CAddPlayerGiftCMD::~CAddPlayerGiftCMD()
{
	
}
	/**
	* @brief 异步执行SQL命令
	* @poDBConn 异步执行SQL命令的Connection连接
	* @return VOID
	*/						
VOID CAddPlayerGiftCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{

	CAutoSqlBuf oAutoSql;

	CHAR* pszSql		= oAutoSql.GetSqlBuf();
	CHAR aszGiftName[1024] = {0};
	CHAR aszGiftDesc[1024] = {0};
	UINT32 dwRet = poDBConn->EscapeString( m_stGiftData.strGiftName.c_str(), m_stGiftData.strGiftName.size(), aszGiftName, 1024 - 1);
	if ( 0 == dwRet)
	{
		SYS_CRITICAL(_SDT("[%s:%u] EscapeString failed"), MSG_MARK);
		return ;
	}
	dwRet = poDBConn->EscapeString( m_stGiftData.strGiftDesc.c_str(), m_stGiftData.strGiftDesc.size(), aszGiftDesc, 1024 - 1);
	if ( 0 == dwRet)
	{
		SYS_CRITICAL(_SDT("[%s:%u] EscapeString failed"), MSG_MARK);
		return ;
	}
	SDSprintf(pszSql, "call sp_AddGift(%u, %u, %u, '%s', '%s', '%s', '%s')", m_stGiftData.dwStartPlayerID, m_stGiftData.dwEndPlayerID, m_stGiftData.dwZoneID, aszGiftName, aszGiftDesc, m_stGiftData.strJson.c_str(), m_stGiftData.strTime.c_str());
	UINT32 dwErr;
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
	if(SDDB_SUCCESS > nRet)
	{
		m_bSuccess = FALSE;
		SYS_CRITICAL(_SDT("[%s: %d]: add  playergift data error!, SQL: %s, Error: %s"), MSG_MARK, pszSql, strError.c_str());
		return ;
	}
}

	/**
	* @brief 同步执行SQL命令相关的逻辑部分
	* @return VOID
	*/
VOID CAddPlayerGiftCMD::OnExecuted()
{

}

VOID CAddPlayerGiftCMD::SetUserData(VOID* pUserData)
{
	m_stGiftData = *(SActivityGift*)pUserData;
}