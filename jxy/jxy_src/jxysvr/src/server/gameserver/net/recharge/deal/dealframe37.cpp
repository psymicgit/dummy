// DealFrame37.cpp: implementation of the CDealFrame37 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe37.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/player/playergetdatamgr.h"
#include <protogsdb.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame37::CDealFrame37()
{

}

CDealFrame37::~CDealFrame37()
{

}

void CDealFrame37::Release()
{
	
}

UINT16 CDealFrame37::GetFrameType()
{
	return 37;
}

void CDealFrame37::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
	if(NULL == pstBaseData)
	{
		poClinet->Rspone("{\"status\":\"player not exist!\"}");
		return;
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if(NULL == poPlayer)
	{
		CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, poClinet->GetCliSessionID(),1, qwParam3, "", byAction, dwSerialNumber);
		return ;
	}
	poClinet->RspPlayerRes(poPlayer);
}

void CDealFrame37::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

	if(NULL == pstBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);

	if(NULL == poPlayer)
	{
		CGetPlayerDataMgr::Instance()->GetPlayerData(unPlayerId, GET_PLAYERDATATYPE_GM, unPlayerId, poClinet->GetCliSessionID(),2, 0, "",(UINT8)GetFrameType(), dwSerialNumber);
		return ;
	}

	poClinet->RspPlayerRes2(poPlayer);
}

string CDealFrame37::GetStrFrameType()
{
	return GET_PLAYER_RES;
}

void CDealFrame37::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;

	RsponeMsg(poClinet,mapField);;
}

string CDealFrame37::GetStrFrameDesc()
{
	return SDGBK2UTF8(GET_PLAYER_RES_DESC);
}

