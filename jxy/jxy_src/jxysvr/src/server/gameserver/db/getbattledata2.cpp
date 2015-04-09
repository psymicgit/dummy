#include "getbattledata2.h"
#include "dbcommoperate.h"
#include "dbsqlstring.h"
#include <db/autosqlbuf.h>
#include <net/gt/gtpktbuilder.h>
#include <net/db/dbprocessor.h>
#include <sdloggerimpl.h>

CGetBattleData2::CGetBattleData2()
{
	m_strSql.resize(1024);
	m_bHaveBattleData=FALSE;
}

CGetBattleData2::~CGetBattleData2()
{

}

VOID SDAPI CGetBattleData2::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	sprintf( const_cast<char *>( m_strSql.c_str() ), "select * from battlelog2 where DataIndex='%llu'", m_oDataParam.qwDataIndex );

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

VOID SDAPI CGetBattleData2::OnExecuted()
{
	CDBProcessor::OnBattleDataAck( m_oDataParam.dwPlayerID, m_battleData, m_bHaveBattleData );
	
}

VOID CGetBattleData2::SetUserData(VOID* pUserData)
{
	m_oDataParam=*( (SBattleDataParam2*)pUserData );
}