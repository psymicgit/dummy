#include "playerbattledata2recordmgr.h"
#include "dll/sdframework/sdloggerimpl.h"
#include "dll/sdframework/sdutil.h"
#include "db/recordbattledata2.h"
#include "db/getbattledata2.h"
#include <framework/gsapi.h>

IMPLEMENT_SINGLETON(CPlayerBattleData2RecordMgr);

CPlayerBattleData2RecordMgr::CPlayerBattleData2RecordMgr()
{
	m_qwIndex=0;
}

CPlayerBattleData2RecordMgr::~CPlayerBattleData2RecordMgr()
{

}

BOOL CPlayerBattleData2RecordMgr::Init()
{
	//读取数据库
	if(!InitDataFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitDataFromDB failed!"), MSG_MARK);
		return FALSE;
	}

	return TRUE;
}

VOID CPlayerBattleData2RecordMgr::UnInit()
{

}

BOOL CPlayerBattleData2RecordMgr::InitDataFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}


	CHAR szSql[1024] = {0};
//	sprintf(szSql, "select count(*) TotalCount from battlelog");

 	SGDP::ISDDBRecordSet* pRecordSet = NULL;
 	string strError;
// 	INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
// 	if(SDDB_HAS_RECORDSET != nRet)
// 	{
// 		SYS_CRITICAL(_SDT("[%s: %d]: has no data!:%s,sql:%s"), MSG_MARK, strError.c_str(), szSql);
// 		return FALSE;
// 	}
// 	auto dwCount=pRecordSet->GetRecordCount();
// 	//如果没有数据，则初始索引为0
// 	auto szRet=pRecordSet->GetFieldValueByName( "TotalCounts" );
// 	SDAtou64( "0" );
// 	if( 0==SDAtou64( pRecordSet->GetFieldValueByName( "TotalCount" ) ) )
// 	{
// 		m_qwIndex=0;
// 
// 		return TRUE;
// 	}

	//有数据，则直接取最大的索引数
	sprintf(szSql, "select * from battlelog2 order by DataIndex desc limit 1");
	strError.clear();
	UINT16 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );

	if( SDDB_NO_RECORDSET==nRet )
	{
		m_qwIndex=0;
		USR_INFO( _SDT("[%s: %d]:sql can't find any battle log"), MSG_MARK );

		if( pRecordSet )
		{
			pRecordSet->Release();
		}
		return TRUE;
	}

	if(SDDB_HAS_RECORDSET != nRet)
	{
		if( pRecordSet )
		{
			pRecordSet->Release();
		}

		SYS_CRITICAL(_SDT("[%s: %d]:sql error, error info:%s,sql:%s"), MSG_MARK, strError.c_str(), szSql);
		return TRUE;
	}
	if( pRecordSet->GetRecord() )
	{
		m_qwIndex=SDAtou64( pRecordSet->GetFieldValueByName( "DataIndex" ) );
	}
	

	if( pRecordSet )
	{
		pRecordSet->Release();
	}
	return TRUE;
}

BOOL CPlayerBattleData2RecordMgr::RecordBattle( const DT_BATTLE_DATA5 &battleData, UINT64 &qwIndex, UINT32 dwPlayerSelf, UINT32 dwPlayerEnemy )
{
	qwIndex=++m_qwIndex;
	CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVEBATTLEDATA2);

	if(NULL == poCMD)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);        
		return FALSE;
	}

	SPlayerBattleData2 data;
	data.oBattleData=battleData;
	data.qwIndex=qwIndex;
	data.dwPlayerMyself=dwPlayerSelf;
	data.dwPlayerEnemy=dwPlayerEnemy;
	poCMD->SetUserData( &data, 0 );
 	return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CPlayerBattleData2RecordMgr::GetBattleData( UINT32 dwPlayerID, UINT64 qwBattleIndex )
{
	CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GETBATTLEDATA2);
	if(NULL == poCMD)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);        
		return FALSE;
	}

	SBattleDataParam2 param;
	param.dwPlayerID=dwPlayerID;
	param.qwDataIndex=qwBattleIndex;
	poCMD->SetUserData( &param, 0 );
	return gsapi::GetDBMgr()->AddCommand(poCMD);
}