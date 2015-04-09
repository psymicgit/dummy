
#include "dbpktbuilder.h"
#include <logic/player/playermgr.h>
#include <common/client/commondef.h>
#include <framework/gsconfig.h>
#include "db/basedbccmd.h"
#include "db/getplayerdatacmd.h"
#include "db/getplayerdatacmd2.h"
#include "db/createplayercmd.h"
#include "db/saveplayerdatacmd.h"
#include "db/clrplayerdatacmd.h"
#include "db/setapppayinfo.h"
#include "db/setapppayprocessed.h"
#include "db/settongbupayinfo.h"
#include "db/settongbupayprocessed.h"
#include "db/getprivatemsglog.h"
#include <logic/player/player.h>
#include <db/setmypayprocessed.h>
#include <db/setcenterpayprocessed.h>
#include <db/settongbupayprocessed.h>
#include <db/setretreatepayprocessed.h>

#include <framework/gsapi.h>
#include "net/cli/user.h"


CDBPktBuilder::CDBPktBuilder()
{

}

CDBPktBuilder::~CDBPktBuilder()
{

}


BOOL CDBPktBuilder::GetPlayerDataReq(SGetPlayerData* pstGetPlayerData)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GET1);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(pstGetPlayerData), pstGetPlayerData->stGetPlayerDataReq.dwTransID);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

//BOOL CDBPktBuilder::GetPlayerDataReqOld(SGetPlayerData* pstGetPlayerData)
//{
//    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GET_OLD);
//    if(NULL == poCMD)
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
//        return FALSE;
//    }
//    poCMD->SetUserData((VOID*)(pstGetPlayerData), pstGetPlayerData->stGetPlayerDataReq.dwTransID);
//    return gsapi::GetDBMgr()->AddCommand(poCMD);
//}



BOOL CDBPktBuilder::GetPlayerDataReq2(SGetPlayerData2* pstGetPlayerData)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GET2);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(pstGetPlayerData), pstGetPlayerData->dwTransID);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::SavePlayerDataReq(UINT32 dwTransID, CPlayerSaveData* poPlayerSaveData, ESaveDBType type, BOOL bRecordOnlineTime)
{
    CBaseDBCMD* poCMD = NULL;

    poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE);

    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }

    SPlayerSaveInfo stInfo;
    stInfo.poPlayerSaveData = poPlayerSaveData;
    stInfo.poPlayerSaveData->m_bAddOnlineTime = bRecordOnlineTime;
    stInfo.enType = type;

    if( gsapi::GetDBMgr()->IsExistErrorInfo(ERROR_PLAYER, poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID ) )
    {
        stInfo.bSaveAll = TRUE;
    }


    poCMD->SetUserData( &stInfo, dwTransID );

    if( type == ESDBT_GAME )
    {
        return gsapi::GetDBMgr()->AddCommand(poCMD);
    }
    else
    {
        return gsapi::GetDBMgr()->AddRecordCommand(poCMD);
    }
}

BOOL CDBPktBuilder::CreatePlayerReq(UINT32 dwTransID, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT8 byGmFlag, DT_PLAYER_DATA& stPlayerDefault)
{
    DT_GSDB_CREATE_PALYER_DATA_REQ stReq;
    memset(&stReq, 0, sizeof(stReq));
    _SDTStrcpy(stReq.aszUserName, pszUserName);
    _SDTStrcpy(stReq.aszDeviceID, pszDeviceID);
    _SDTStrcpy(stReq.aszNotifyID, pszNotifyID);
    stReq.dwUserID = dwPlayerID;
    stReq.byGmFlag = byGmFlag;
    memcpy(&stReq.stPlayerData, &stPlayerDefault, sizeof(DT_PLAYER_DATA));

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_CREATE);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(&stReq), dwTransID);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::ClrPlayerData(UINT32 dwPlayerID)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_CLR);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(&dwPlayerID), 0);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::SavePlayersPlunderDataReq(CPlayersPlunderSaveData* poPlayersPlunderSaveData)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVEPLUNDER);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(poPlayersPlunderSaveData), 0);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::SavePlayersRaceDataReq(CPlayersRaceSaveData* poPlayersRaceSaveData)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVERACE);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(poPlayersRaceSaveData), 0);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::SaveBossBDataReq(DT_BOSS_BATTLE_DATA* pstDT_BOSS_BATTLE_DATA)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVEBOSSB);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(pstDT_BOSS_BATTLE_DATA), 0);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::SaveFactionDataReq(DT_FACTION_DATA* pstDT_FACTION_DATA)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_FACTION);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(pstDT_FACTION_DATA), 0);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::SaveGvGBuildDataReq(DT_GVG_BUILD_DATA& stData)
{
	CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_GVG_BUILD);
	if(NULL == poCMD)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
		return FALSE;
	}
	poCMD->SetUserData((VOID*)(&stData), 0);
	return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CDBPktBuilder::OnGetOrderIDReq(CPlayer* poPlayer, CUser* poUser)
{
    if((NULL == poPlayer) || (NULL == poUser))
    {
        return FALSE;
    }

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SETTONGBUPAYINFO);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    STongBuPayInfo payInfo;
    payInfo.dwPlayerID = poPlayer->GetID();
    payInfo.dwPlayerLevel = poPlayer->GetLevel();
    payInfo.dwUserID = poUser->GetUserID();
    payInfo.dwZoneID = poUser->GetZoneID();
    payInfo.strSource = "TongBu";
    payInfo.strTableName = CGSConfig::Instance()->GetAppPayTableName();
    payInfo.strOrderID = GetOrderID(payInfo.strTableName, payInfo.dwPlayerID);

    poCMD->SetUserData( &payInfo, payInfo.dwPlayerID );

    gsapi::GetDBMgr()->AddPayCommand( poCMD );


    USR_INFO( "OnGetOrderIDReq PlayerID:%u, OrderID:%s", poPlayer->GetDT_PLAYER_BASE_DATA().dwID, payInfo.strOrderID.c_str() );

    return TRUE;

}

string CDBPktBuilder::GetOrderID(string strTableName, UINT32 dwPlayerID)
{
    CHAR szBuff[218] = {0};
    vector<string> vecField = SDSplitStringsA(strTableName, '_');
    if(vecField.size() != 2)
    {
        sprintf(szBuff, "%s_%u_%llu_%u_%u", strTableName.c_str(), dwPlayerID, SDTimeSecs(), GetCycleCount(), RandomReseed(1000));
        USR_INFO( "GetOrderID TableName[%s] is invalid!", strTableName.c_str());
    }
    else
    {
        sprintf(szBuff, "%s_%u_%llu_%u_%u", vecField[1].c_str(), dwPlayerID, SDTimeSecs(), GetCycleCount(), RandomReseed(1000));
    }

    return szBuff;
}


BOOL CDBPktBuilder::OnProcessOrderReq(STongBuPayProcessed& stInfo)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SETTONGBUPAYPPROCESSED);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }

    poCMD->SetUserData( &stInfo, 0 );

    gsapi::GetDBMgr()->AddPayCommand( poCMD );


    USR_INFO( "OnProcessOrderReq OrderID:%s", stInfo.strOrderID.c_str());

    return TRUE;
}

BOOL CDBPktBuilder::OnProcessMyOrderReq(SMyPayProcessed& stInfo)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SETMYPAYPPROCESSED);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }

    poCMD->SetUserData( &stInfo, 0 );

    gsapi::GetDBMgr()->AddPayCommand( poCMD );


    USR_INFO( "OnProcessMyOrderReq OrderID:%s", stInfo.strOrderID.c_str());

    return TRUE;
}

BOOL CDBPktBuilder::OnProcessCenterOrderReq(SCenterPayProcessed& stInfo)
{
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SETCENTERPAYPPROCESSED);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }

    poCMD->SetUserData( &stInfo, 0 );

    gsapi::GetDBMgr()->AddCommand( poCMD );


    USR_INFO( "OnProcessCenterOrderReq OrderID:%s", stInfo.strOrderID.c_str());

    return TRUE;
}

BOOL CDBPktBuilder::OnProcessRechargeOrderReq(vector<SRecreatePayProcessed>& vecInfo,UINT32 dwTransID)
{
	CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SETRECREATEPAYPROCESSED);
	if(NULL == poCMD)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
		return FALSE;
	}

	poCMD->SetUserData( &vecInfo, dwTransID);

	gsapi::GetDBMgr()->AddCommand( poCMD );

	USR_INFO( "OnProcessRetreateOrderReq");

	return TRUE;
}

BOOL CDBPktBuilder::GetPrivateMsg(UINT32 dwSenderID, UINT32 dwRecverID, em_GetPrivateMsg emType, string strMsg, UINT32 dwStartIdx, UINT8  byGetMsgNum)
{
    GET_PRIVATEMSGLOG stInfo;
    stInfo.dwSenderID = dwSenderID;
    stInfo.dwRecverID = dwRecverID;
    stInfo.byType = emType;
    stInfo.strMsgContent = strMsg;
    stInfo.dwStartIdx = dwStartIdx;
    stInfo.byGetMsgNum = byGetMsgNum;

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GET_PRIVATEMSGLOG);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }

    poCMD->SetUserData( &stInfo, 0 );

    gsapi::GetDBMgr()->AddCommand( poCMD );

    return TRUE;
}


