#include "db/addgvgift.h"
#include "db/autosqlbuf.h"

CAddGvGiftCMD::CAddGvGiftCMD()
{
	m_vecGigtLstData.clear();
}

CAddGvGiftCMD::~CAddGvGiftCMD()
{
	m_vecGigtLstData.clear();
}

VOID CAddGvGiftCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	m_bSuccess = TRUE;
	if ( 0 == m_vecGigtLstData.size())
	{
		return ;
	}

	CAutoSqlBuf oAutoSqlBuf;
	CAutoSqlBuf oAutoSql;
	CHAR* pszSql		= oAutoSql.GetSqlBuf();
	CHAR* pszBuf        = oAutoSqlBuf.GetSqlBuf();
	if((NULL == pszSql))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
		return ;
	}

    CAutoSqlBuf oAutoTmp;
    CHAR* pszTmp		= oAutoTmp.GetSqlBuf();

	memset(pszSql, 0, MAX_SQL_LEN);
	sprintf(pszSql, "insert into gvggift(PlayerID, SessionID, GvGRound, BattleIdx, BattleResult, FactionName, GiftID, GiftType, Rank) values ");
	 UINT32 dwIdx = 0;
	for (CVecGiftRewardDataItr itr = m_vecGigtLstData.begin(); itr != m_vecGigtLstData.end(); dwIdx++, itr++)
	{
		if ( 0 != dwIdx)
		{
			SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ",");
		}
		memset(pszBuf, 0, MAX_SQL_LEN);

        poDBConn->EscapeString(itr->aszFactionName, SDStrlen(itr->aszFactionName), pszTmp, MAX_SQL_LEN);

		SDSnprintf(pszBuf, MAX_SQL_LEN - 1, "(%lu, %u, %u, %u, %u, '%s', %u, %u, %u)", 
			itr->dwPlayerID,
			itr->wSessionID,
			itr->byGvGRound,
			itr->byBattleIdx,
			itr->byBattleResult,
			pszTmp,
			itr->byGiftID,
			itr->byGiftType,
			itr->byRank);
		SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuf);
	}
	UINT32 dwErr;
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
	if(SDDB_SUCCESS > nRet)
	{
		m_bSuccess = FALSE;
		SYS_CRITICAL(_SDT("[%s: %d]: add  gvggift data error!, SQL: %s, Error: %s"), MSG_MARK, pszSql, strError.c_str());
		return ;
	}
}

	/**
	* @brief 同步执行SQL命令相关的逻辑部分
	* @return VOID
	*/
VOID CAddGvGiftCMD::OnExecuted()
{

}

VOID CAddGvGiftCMD::SetUserData(VOID* pUserData)
{
	CVecGiftRewardData* poVecData = (CVecGiftRewardData*)pUserData;
	m_vecGigtLstData.insert(m_vecGigtLstData.begin(), poVecData->begin(), poVecData->end());
}