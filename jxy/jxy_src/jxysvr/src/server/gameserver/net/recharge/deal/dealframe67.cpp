// DealFrame65.cpp: implementation of the CDealFrame67 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe67.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/faction/factionmgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame67::CDealFrame67()
{

}

CDealFrame67::~CDealFrame67()
{

}

void CDealFrame67::Release()
{

}

UINT16 CDealFrame67::GetFrameType()
{
	return EGMT_QUERY_SUM_DOORSTRIBUTE;
}

void CDealFrame67::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if(NULL == poClinet)
	{
		return;
	}
	const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
	if(NULL == pstBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}
	BOOL bIsJoin = FALSE;
	map<string, UINT64> mapData;
	CFaction* poFaction = CFactionMgr::Instance()->GetFactionByPlayerID(dwPlayerID);
	if ( NULL == poFaction)
	{
		bIsJoin = FALSE;
		mapData["IsJoinFaction"] = FALSE;
		mapData["SumDoorsTribute"] = 0;
	}
	else
	{
		bIsJoin = TRUE;
		mapData["IsJoinFaction"] = TRUE;
		mapData["SumDoorsTribute"] = poFaction->GetSumDoorsTribute(dwPlayerID);
	}
	
	string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapData);
	poClinet->Rspone(strMsg);
}

void CDealFrame67::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
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
		return;
	}

	BOOL bIsJoin = FALSE;
	map<string, UINT64> mapData;
	CFaction* poFaction = CFactionMgr::Instance()->GetFactionByPlayerID(dwPlayerID);
	if ( NULL == poFaction)
	{
		bIsJoin = FALSE;
		mapData["IsJoinFaction"] = FALSE;
	}
	else
	{
		bIsJoin = TRUE;
		mapData["IsJoinFaction"] = TRUE;
	}
	mapData["SumDoorsTribute"] = poFaction->GetSumDoorsTribute(dwPlayerID);
	string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapData);
	poClinet->Rspone(strMsg);
}

string CDealFrame67::GetStrFrameType()
{
	return QUERY_SUM_DOORSTRIBUTE;
}

void CDealFrame67::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
//	mapField[IS_JOIN_IN_FACTION_PARAM] = SDGBK2UTF8(IS_JOIN_IN_FACTION_GBK)+ "|" + GM_PARAM_INT;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame67::GetStrFrameDesc()
{
	return SDGBK2UTF8(QUERY_SUM_DOORSTRIBUTE_DESC);
}

