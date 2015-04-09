// DealFrame50.cpp: implementation of the CDealFrame50 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe50.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include "logic/activityencourage/oprateingactivitymgr.h"
#include "logic/player/playerbasedatacachemgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame50::CDealFrame50()
{

}

CDealFrame50::~CDealFrame50()
{

}

void CDealFrame50::Release()
{
	
}

UINT16 CDealFrame50::GetFrameType()
{
	return 50;
}

void CDealFrame50::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4) || (NULL == pszParam5))
	{
		return;
	}

	CHAR szRet[512] = {0};
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if(NULL == poPlayer)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}
	string strRet;
	strRet.reserve(256);
	COprateingActivityMgr::Instance()->PhpBattle(poPlayer, dwParam1, (UINT8)qwParam2, (UINT8)qwParam3, strRet);
	poClinet->Rspone(strRet.c_str());
}

void CDealFrame50::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty() || mapField[ACTIVITY_ID_PARAM].empty() || mapField[TOWN_ID_PARAM].empty() || mapField[COPY_ID_PARAM].empty() )
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	UINT32 dwParam1 = SDAtou(mapField[ACTIVITY_ID_PARAM].c_str());
	UINT32 dwParam2 = SDAtou(mapField[TOWN_ID_PARAM].c_str());
	UINT32 dwParam3 = SDAtou(mapField[COPY_ID_PARAM].c_str());

	const SPlayerBaseData* pPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

	if (NULL == pPlayerBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);

	if(NULL == poPlayer)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	string strRet;
	strRet.reserve(256);
	COprateingActivityMgr::Instance()->PhpBattle(poPlayer, dwParam1, (UINT8)dwParam2, (UINT8)dwParam3, strRet);
	poClinet->Rspone(strRet.c_str());
}

string CDealFrame50::GetStrFrameType()
{
	return OPRATING_ACTIVITY_BATTLE;
}

void CDealFrame50::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
	mapField[ACTIVITY_ID_PARAM] = SDGBK2UTF8(OPRATING_ACTIVITY_BATTLE_GBK)+ "|" + GM_PARAM_INT;
	mapField[TOWN_ID_PARAM] = SDGBK2UTF8(TOWN_ID_GBK)+ "|" + GM_PARAM_INT;
	mapField[COPY_ID_PARAM] = SDGBK2UTF8(AREA_ID_GBK)+ "|" + GM_PARAM_INT;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame50::GetStrFrameDesc()
{
	return SDGBK2UTF8(OPRATING_ACTIVITY_BATTLE_DESC);

}