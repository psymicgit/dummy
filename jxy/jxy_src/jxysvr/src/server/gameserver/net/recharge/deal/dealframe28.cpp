// DealFrame28.cpp: implementation of the CDealFrame28 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe28.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playerbasedatacachemgr.h"
#include "common/client/commondef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame28::CDealFrame28()
{

}

CDealFrame28::~CDealFrame28()
{

}

void CDealFrame28::Release()
{
	
}

UINT16 CDealFrame28::GetFrameType()
{
	return 28;
}

void CDealFrame28::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
#ifndef BATTLE_TEST
	poClinet->Rspone("{\"status\":\"service not define!\"}");
	return;
#endif
	CNGString strRet;
	vector<string> vecParam = SDSplitStringsA(pszParam4, '|');
	if(vecParam.size() != 2 )
	{
		strRet += "{\"status\":\"param not match!\"}";
	}
	UINT16 wErrCode = CGMProcessor::InsanceBattle(dwPlayerID, UINT16(dwParam1), UINT8(qwParam2), UINT8(qwParam3), SDAtou((CHAR*)(vecParam[0].c_str())), SDAtou((CHAR*)(vecParam[1].c_str())), strRet);
	if(0 != wErrCode)
	{
		strRet += "{\"status\":\"player not in mem\"}";
	}
	poClinet->Rspone(strRet.str());
}

void CDealFrame28::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty() 
		|| mapField[INSTANCE_BATTLE_SCENE_ID_PARAM].empty() 
		|| mapField[INSTANCE_BATTLE_TOWN_ID_PARAM].empty()
		|| mapField[INSTANCE_BATTLE_INSTANCE_ID_PARAM].empty() 
		|| mapField[INSTANCE_BATTLE_NUM_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

#ifndef BATTLE_TEST
	poClinet->Rspone("{\"status\":\"service not define!\"}");
	return;
#endif

	UINT16  unSceneId = SDAtou(mapField[INSTANCE_BATTLE_SCENE_ID_PARAM].c_str());
	UINT8 unTownId = SDAtou(mapField[INSTANCE_BATTLE_TOWN_ID_PARAM].c_str());
	UINT8 unInstanceId = SDAtou(mapField[INSTANCE_BATTLE_INSTANCE_ID_PARAM].c_str());
	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	string strInstanceCount = mapField[INSTANCE_BATTLE_NUM_PARAM];

	const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

	if(NULL == poPlayerBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	CNGString strRet;
	vector<string> vecParam = SDSplitStringsA(strInstanceCount.c_str(), '|');

	if(vecParam.size() != 2 )
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	UINT16 wErrCodeBattle = CGMProcessor::InsanceBattle(unPlayerId, UINT16(unSceneId), UINT8(unTownId), UINT8(unInstanceId), SDAtou((CHAR*)(vecParam[0].c_str())), SDAtou((CHAR*)(vecParam[1].c_str())), strRet);
	UINT16 wErrCode = 0;

	if (wErrCodeBattle == EBT_SUCCESS)
	{
		wErrCode = ERR_GM_PARM_INFO::ID_SUCCESS;
	}
	else if(wErrCodeBattle == EBT_INSTANCE_PASS)
	{
		wErrCode = ERR_GM_PARM_INFO::ID_INSTANCE_PASS;
	}
	else
	{
		wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
	}

	if (wErrCode != 0)
	{
		string strErrorMsg = GetRsponeResult(wErrCode);
		poClinet->Rspone(strErrorMsg.c_str());
	}
	else
	{
		poClinet->Rspone(strRet.str());
	}
	
}

string CDealFrame28::GetStrFrameType()
{
	return INSTANCE_BATTLE;
}

void CDealFrame28::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
	mapField[INSTANCE_BATTLE_SCENE_ID_PARAM] = SDGBK2UTF8(INSTANCE_BATTLE_SCENE_ID_GBK) + "|" + GM_PARAM_INT;
	mapField[INSTANCE_BATTLE_TOWN_ID_PARAM] = SDGBK2UTF8(INSTANCE_BATTLE_TOWN_ID_GBK) + "|" + GM_PARAM_INT;
	mapField[INSTANCE_BATTLE_INSTANCE_ID_PARAM] = SDGBK2UTF8(INSTANCE_BATTLE_INSTANCE_ID_GBK) + "|" + GM_PARAM_INT;
	mapField[INSTANCE_BATTLE_NUM_PARAM] = SDGBK2UTF8(INSTANCE_BATTLE_NUM_GBK) + "|" + GM_PARAM_STR;
	RsponeMsg(poClinet,mapField);
}

string CDealFrame28::GetStrFrameDesc()
{
	return SDGBK2UTF8(INSTANCE_BATTLE_DESC);
}

