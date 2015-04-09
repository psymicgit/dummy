#include <framework/gsapi.h>
#include "getplayerdatacmd_old.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/server/base64.h>
#include "dbcommoperate.h"
#include <sdloggerimpl.h>

#include <net/db/dbprocessor.h>

/**
* @brief 用户保存类型
*/
/*
enum ESaveUserToDbType
{
    ESRTDT_UNVALID = -1,
    ESRTDT_SHUTDOWN_EXIT    ,       ///< 玩家断线
    ESRTDT_GATESHUTDOWN_EXIT,       ///< GT断线
    ESRTDT_TIMER            ,       ///< TIMER例行保存
    ESRTDT_KICKOUT_ROLE     ,       ///< 踢人
    ESRTDT_MAX
};
*/


//CGetPlayerDataCMD_OLD::CGetPlayerDataCMD_OLD()
//{
//    m_pstGetPlayerData = NULL;
//}
//
//
//CGetPlayerDataCMD_OLD::~CGetPlayerDataCMD_OLD()
//{
//}
//
//VOID CGetPlayerDataCMD_OLD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
//{
//    SDTRY_BEGIN
//
//
//    m_bSuccess = GetData(poDBConn);
//
//
//    SDTRY_END
//
//}
//
//VOID CGetPlayerDataCMD_OLD::OnExecuted()
//{
//    SDTRY_BEGIN
//
//    gsapi::GetDBProcessor()->OnPlayerDataAck(m_pstGetPlayerData, m_dwTransID);
//
//
//    SDTRY_END
//}
//
//BOOL CGetPlayerDataCMD_OLD::GetData(SGDP::ISDDBConnection* poDBConn)
//{
//    //CAutoCycle oAutoCycle(__FUNCTION__);
//	if(!GetPlayerData(poDBConn))
//	{
//		SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerData failed! playerid:%d"), MSG_MARK, m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID);    
//		return FALSE;
//	}
//
//	if(!GetPlayerPvpData(poDBConn))
//	{
//		SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerPvpData failed! playerid:%d"), MSG_MARK, m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID);   
//		return FALSE;
//	}
//
//    return TRUE;
//}
//
//
//BOOL CGetPlayerDataCMD_OLD::GetPlayerData(SGDP::ISDDBConnection* poDBConn)
//{
//	//CAutoCycle oAutoCycle(__FUNCTION__);
//
//	m_pstGetPlayerData->stGetPlayerDataAck.wErrCode = FAIL;
//
//	CHAR szSQL[128] = {0};
//	sprintf(szSQL, "select * from player where ID='%u'", m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID);
//
//	ISDDBRecordSet* pRecordSet = NULL;
//	string strError;
//	INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet,0, &strError );
//
//	if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
//	{
//		CDBCommOperate::FillPlayerData( *pRecordSet, m_pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData, 
//			m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID );
//	}
//	else
//	{
//		m_pstGetPlayerData->stGetPlayerDataAck.wErrCode = GET_NODATA;
//		USR_INFO( _SDT("player:%u not exist, sql:%s"), m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID, szSQL );
//		return FALSE;
//	}
//	if(NULL != pRecordSet)
//	{
//		pRecordSet->Release();
//		pRecordSet = NULL;
//	}
//	m_pstGetPlayerData->stGetPlayerDataAck.wErrCode = SUCCESS;
//
//	return TRUE;
//}
//
//
//BOOL CGetPlayerDataCMD_OLD::GetPlayerPvpData(SGDP::ISDDBConnection* poDBConn)
//{
//	//CAutoCycle oAutoCycle(__FUNCTION__);
//
//	m_pstGetPlayerData->stGetPlayerDataAck.wErrCode = FAIL;
//
//	CHAR szSQL[128] = {0};
//	sprintf(szSQL, "select * from playerpvp where id='%u'", m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID);
//
//	ISDDBRecordSet* pRecordSet = NULL;
//	INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
//
//	UINT32 dwPlayerID = m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID;
//	if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
//	{
//		DT_PLAYER_PVP_DATA& stDT_PLAYER_DATA = m_pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerPvpData;
//		ISDDBRecordSet &recordSet = *pRecordSet;
//
//		GetEncodeDataFromDB( "PlunderBaseData", stDT_PLAYER_DATA.stPlunderData.stPlunderBaseData, dwPlayerID );
//
//		GetEncodeDataFromDB( "PlunderBattleLog", stDT_PLAYER_DATA.stPlunderData.stPlunderBattleLog, dwPlayerID );
//		GetEncodeDataFromDB( "CaptureData", stDT_PLAYER_DATA.stPlunderData.stCaptureData, dwPlayerID );
//		GetEncodeDataFromDB( "EnemyData", stDT_PLAYER_DATA.stEnemyData, dwPlayerID );
//		GetEncodeDataFromDB( "RaceData", stDT_PLAYER_DATA.stRaceData.stRaceBattleLog, dwPlayerID );
//		GetEncodeDataFromDB( "RaceBattleLog", stDT_PLAYER_DATA.stRaceData.stRaceBattleLog, dwPlayerID );
//	
//	}
//	else
//	{
//		m_pstGetPlayerData->stGetPlayerDataAck.wErrCode = GET_NODATA;
//		return FALSE;
//	}
//	if(NULL != pRecordSet)
//	{
//		pRecordSet->Release();
//		pRecordSet = NULL;
//	}
//	m_pstGetPlayerData->stGetPlayerDataAck.wErrCode = SUCCESS;
//
//	return TRUE;
//}
//
//
//VOID CGetPlayerDataCMD_OLD::SetUserData(VOID* pUserData)
//{
//    m_pstGetPlayerData = (SGetPlayerData*)pUserData;
//}
