#include "getbattledata.h"
#include "dbcommoperate.h"
#include "dbsqlstring.h"
#include <db/autosqlbuf.h>
#include <net/gt/gtpktbuilder.h>
#include <net/db/dbprocessor.h>
#include <sdloggerimpl.h>

CGetBattleData::CGetBattleData()
{
	m_strSql.resize(1024);
	m_bHaveBattleData=FALSE;
}

CGetBattleData::~CGetBattleData()
{

}

VOID SDAPI CGetBattleData::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
 	CAutoSqlBuf oAutoBuf;
 	CHAR* pszBuff=oAutoBuf.GetSqlBuf();
 	

 	//memset(pszBuff, 0, MAX_SQL_LEN);
 	INT32 nRet = m_battleData.EncodeSaveData(pszBuff, MAX_SQL_LEN);
	if(nRet <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: m_stBattleData.oBattleData is failed"), MSG_MARK);
		return;
	}
	CHAR szBattleData[ MAXENLEN_DT_BATTLE_DATA2 * 2 + 1] = {0};
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
		nRet,
		szBattleData,
		sizeof(szBattleData));
	if(dwRetCode <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
		return;
	}
	sprintf( const_cast<char *>( m_strSql.c_str() ), "select * from battlelog where DataIndex='%llu'", m_oDataParam.qwDataIndex );

	

	ISDDBRecordSet*pRecordSet = NULL;
	string strError;
	INT32 nResult = poDBConn->ExecuteSqlRs( m_strSql.c_str(), &pRecordSet, NULL, &strError );

	if( NULL==pRecordSet )
	{
		return;
	}

	if( (SDDB_HAS_RECORDSET == nResult) && (pRecordSet->GetRecord()) )
	{
		UINT32 dwLen = pRecordSet->GetFieldLengthByName( "BattleData" );
		if( -1 == m_battleData.DecodeSaveData( pRecordSet->GetFieldValueByName("BattleData"), dwLen ) )
		{
			SYS_CRITICAL( _SDT("[%s: %d] [DataIndex:%llu]: m_battleData.oBattleData Failed!"), MSG_MARK, m_oDataParam.qwDataIndex );
			return;
		}
		m_bHaveBattleData=TRUE;
	}	
	
}

VOID SDAPI CGetBattleData::OnExecuted()
{
	CDBProcessor::OnBattleDataAck( m_oDataParam.dwPlayerID, m_battleData, m_bHaveBattleData );
	
}

VOID CGetBattleData::SetUserData(VOID* pUserData)
{
	m_oDataParam=*( (SBattleDataParam*)pUserData );
}