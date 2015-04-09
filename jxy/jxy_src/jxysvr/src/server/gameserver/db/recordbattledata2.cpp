#include "recordbattledata2.h"
#include "dbcommoperate.h"
#include "dbsqlstring.h"
#include <db/autosqlbuf.h>
#include <sdloggerimpl.h>

CRecordBattleData2::CRecordBattleData2()
{
	
}

CRecordBattleData2::~CRecordBattleData2()
{

}

VOID SDAPI CRecordBattleData2::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	CAutoSqlBuf oAutoBuf;
	CHAR*  pszBuff=oAutoBuf.GetSqlBuf();
	

	//memset(pszBuff, 0, MAX_SQL_LEN);
	INT32 nRet = m_stBattleData.oBattleData.EncodeSaveData(pszBuff, MAX_SQL_LEN);
	if(nRet <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: m_stBattleData.oBattleData is failed"), MSG_MARK);
		return;
	}

	CHAR szBattleData[ MAXENLEN_DT_BATTLE_DATA5 * 2 + 1] = {0};
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
		nRet,
		szBattleData,
		sizeof(szBattleData));
	if(dwRetCode <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
		return;
	}

	//string m_strSql; //执行的SQL语句的记录
	//m_strSql.resize(10240);

	CAutoSqlBuf ostrBuf;
	CHAR*  pstrBuff = ostrBuf.GetSqlBuf();
	sprintf( pstrBuff, "insert into battlelog2( PlayerID1, PlayerID2, DataIndex, BattleData, CreateTime, Version ) \
													 values( '%u', '%u', '%llu', '%s', now(), '%d' )", m_stBattleData.dwPlayerMyself, 
													 m_stBattleData.dwPlayerEnemy, m_stBattleData.qwIndex, szBattleData, PROTOCOL_VERSION );

	ISDDBRecordSet*pRecordSet = NULL;
	string strError;
	INT32 nResult = poDBConn->ExecuteSqlRs( pstrBuff, &pRecordSet, NULL, &strError );
	if( nResult<0 )
	{
		SYS_CRITICAL( _SDT( "[%s:%d] CRecordBattleData2 OnExecuteSql failed, exec SQL[%s] failed, error str:%s" ), MSG_MARK, pstrBuff, strError.c_str() );
	}
}

VOID SDAPI CRecordBattleData2::OnExecuted()
{

}

VOID CRecordBattleData2::SetUserData(VOID* pUserData)
{
	m_stBattleData=*( (SPlayerBattleData2*)pUserData );
}