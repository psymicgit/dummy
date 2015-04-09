
#include "gspktbuilder.h"
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/player/playersavemgr.h>
#include <protocol/server/protogsgs.h>
#include <net/cli/usermgr.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>

CGSPktBuilder::CGSPktBuilder()
{

}

CGSPktBuilder::~CGSPktBuilder()
{

}

UINT16 CGSPktBuilder::StartSyncPlayer(UINT64 qwServerStartTime)
{
	ZERO_PACKET(PKT_GSGS_START_SYNC_PLAYER_RPT);
	pstAck->qwServerStartTime = qwServerStartTime;

	return GSGS_START_SYNC_PLAYER_RPT;
}

UINT16 CGSPktBuilder::SyncPlayer(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return INVALID_MSGID;
	}

	ZERO_PACKET(PKT_GSGS_SYNC_PLAYER_REQ);
	
	DT_PLAYER_DATA stDT_PLAYER_DATA;
	memset(&stDT_PLAYER_DATA, 0x00, sizeof(DT_PLAYER_DATA));
	if (!CPlayerSaveMgr::Instance()->SavePlayerData(poPlayer, ESDBT_GAME, &stDT_PLAYER_DATA))
	{
		return INVALID_MSGID;
	}

	//DT_PLAYER_DATA stDT_PLAYER_DATA = CPlayerSaveMgr::Instance()->GetPlayerData(poPlayer);
	pstAck->dwPlayerDataLen = stDT_PLAYER_DATA.EncodeSaveData((CHAR*)(pstAck->abyPlayerDataInfo), MAX_PACKET_LEN);
	if(pstAck->dwPlayerDataLen <= 0)
	{
		return INVALID_MSGID;
	}
	CUser* poUser = CUserMgr::Instance()->FindUser(poPlayer->GetID());
	if(poUser)
	{
		pstAck->byHaveUserFlag = 1;
		DT_USER_DATA& stDT_USER_DATA = pstAck->astUserInfo[0];
		stDT_USER_DATA.dwUserID = poUser->GetUserID();
		stDT_USER_DATA.dwPlayerID = poUser->GetPlayerID();
		stDT_USER_DATA.wCareerID = poUser->GetCareerID();
        stDT_USER_DATA.wZoneID = poUser->GetZoneID();
		stDT_USER_DATA.wOriZoneID = poUser->GetOriginalZoneID();
		stDT_USER_DATA.dwLoginIP = poUser->GetLoginIP();
		stDT_USER_DATA.dwCliVer = poUser->GetCliVer();

		_SDTStrcpy(stDT_USER_DATA.aszUserName,poUser->GetUserName());
		_SDTStrcpy(stDT_USER_DATA.aszDeviceID,poUser->GetDeviceID());
		_SDTStrcpy(stDT_USER_DATA.aszNotifyID,poUser->GetNotifyID());
	}
	else
	{
		pstAck->byHaveUserFlag = 0;
	}	

	return GSGS_SYNC_PLAYER_REQ;
}

UINT16 CGSPktBuilder::SyncPlayerRpt(UINT16 dwPlayerID, UINT16 wErrCode)
{
	ZERO_PACKET(PKT_GSGS_SYNC_PLAYER_RPT);
	pstAck->wErrCode = wErrCode;
	pstAck->dwPlayerID = dwPlayerID;

	return GSGS_SYNC_PLAYER_RPT;
}


UINT16 CGSPktBuilder::SyncDspNameRpt(UINT16 dwLastPlayerID, UINT16 wErrCode)
{
	ZERO_PACKET(PKT_GSGS_SYNC_DSPNAME_RPT);
	pstAck->wErrCode = wErrCode;
	pstAck->dwLastPlayerID = dwLastPlayerID;

	return GSGS_SYNC_DSPNAME_RPT;
}
