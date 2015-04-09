// DealFrame52.cpp: implementation of the CDealFrame52 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe52.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include <logic/player/player.h>
#include "logic/player/playerbasedatacachemgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame52::CDealFrame52()
{

}

CDealFrame52::~CDealFrame52()
{

}

void CDealFrame52::Release()
{
	
}

UINT16 CDealFrame52::GetFrameType()
{
	return 52;
}

void CDealFrame52::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet))
	{
		return;
	}

	CHAR szRet[512] = {0};
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if(NULL == poPlayer)
	{
		poClinet->Rspone("{\"status\":\"Player no in mem!\"}");
		return;
	}
	if(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < dwParam1)
	{
		poClinet->Rspone("{\"status\":\"Gold not enough!\"}");
	}
	poPlayer->DecGold(dwParam1, CRecordMgr::EDGT_GM);
	poClinet->Rspone("{\"status\":\"Success!\"}");
}

void CDealFrame52::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty() || mapField[HTTP_DEC_GOLD_VALUE_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	UINT32 unAmount = SDAtou(mapField[HTTP_DEC_GOLD_VALUE_PARAM].c_str());
	const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

	if(NULL == poPlayerBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	CHAR szRet[512] = {0};
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);

	if(NULL == poPlayer)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	if(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < unAmount)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_GOLD_NOT_ENOUGH);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	poPlayer->DecGold(unAmount, CRecordMgr::EDGT_GM);
	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame52::GetStrFrameType()
{
	return HTTP_DEC_GOLD;
}

void CDealFrame52::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
	mapField[HTTP_DEC_GOLD_VALUE_PARAM] = SDGBK2UTF8(HTTP_DEC_GOLD_GBK)+ "|" + GM_PARAM_INT;
	RsponeMsg(poClinet,mapField);
}

string CDealFrame52::GetStrFrameDesc()
{
	return SDGBK2UTF8(HTTP_DEC_GOLD_DESC);
}

