// DealFrame29.cpp: implementation of the CDealFrame29 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe29.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include "logic/player/playerbasedatacachemgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame29::CDealFrame29()
{

}

CDealFrame29::~CDealFrame29()
{

}

void CDealFrame29::Release()
{
	
}

UINT16 CDealFrame29::GetFrameType()
{
	return 29;
}

void CDealFrame29::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	CNGString strRet;
	strRet += "{\"Mem Num\":\"";
	strRet += CPlayerMgr::Instance()->GetID2PlayerMap().size();
	strRet += "\",\"OL Num\":\"";
	strRet += CUserMgr::Instance()->GetConnectedNum();
	strRet += "\"}";
	poClinet->Rspone(strRet.str());
}

void CDealFrame29::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if(NULL == poClinet)
	{
		return;
	}

	map<string, UINT32> mapJson;
	mapJson["Mem_Num"] = CPlayerMgr::Instance()->GetID2PlayerMap().size();
	mapJson["OL_Num"] = CUserMgr::Instance()->GetConnectedNum();
	string strRet = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapJson);
	poClinet->Rspone(strRet);
}

string CDealFrame29::GetStrFrameType()
{
	return PLAYER_OL_NUM;
}

void CDealFrame29::Interface( CRCClient* poClinet )
{

	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	RsponeMsg(poClinet,mapField);
}

string CDealFrame29::GetStrFrameDesc()
{
	return SDGBK2UTF8(PLAYER_OL_NUM_DESC);
}

