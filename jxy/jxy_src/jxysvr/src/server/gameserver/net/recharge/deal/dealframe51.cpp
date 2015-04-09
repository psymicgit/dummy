// DealFrame51.cpp: implementation of the CDealFrame51 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe51.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playerbasedatacachemgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame51::CDealFrame51()
{

}

CDealFrame51::~CDealFrame51()
{

}

void CDealFrame51::Release()
{
	
}

UINT16 CDealFrame51::GetFrameType()
{
	return 51;
}

void CDealFrame51::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4) || (NULL == pszParam5))
	{
		return;
	}

	CHAR szRet[512] = {0};
	const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
	if(NULL == poPlayerBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}
	sprintf(szRet, "Level='%u'", poPlayerBaseData->wLevel);
	poClinet->Rspone(szRet);
}

void CDealFrame51::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
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
	CHAR szRet[512] = {0};
	const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

	if(NULL == poPlayerBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}
	map<string, UINT32> strMap;
	strMap["level"] = poPlayerBaseData->wLevel;
	string strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &strMap);
	poClinet->Rspone(strRetInfo);
}

string CDealFrame51::GetStrFrameType()
{
	return GET_PLAYER_LEVEL;
}

void CDealFrame51::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame51::GetStrFrameDesc()
{
	return SDGBK2UTF8(GET_PLAYER_LEVEL_DESC);
}

