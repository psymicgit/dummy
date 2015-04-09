// DealFrame11.cpp: implementation of the CDealFrame11 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe11.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame11::CDealFrame11()
{

}

CDealFrame11::~CDealFrame11()
{

}

void CDealFrame11::Release()
{
	
}

UINT16 CDealFrame11::GetFrameType()
{
	return 11;
}

void CDealFrame11::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, pszParam4, poClinet->GetCliSessionID(), pszDesc);
	if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT == wErrCode)
		return;
	CHAR szRet[128] = {0};
	if(0 == wErrCode)
	{
		sprintf(szRet, "{\"status\":\"success\"}");
	}
	else
	{
		sprintf(szRet, "{\"status\":\"error_%d\"}", wErrCode);
	}
	poClinet->Rspone(szRet);
}

void CDealFrame11::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
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
	UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber,(UINT8)GetFrameType(), unPlayerId,0,0,0,"",poClinet->GetCliSessionID(), mapField["desc"].c_str());
	if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT == wErrCode)
		return;
	string strErrorMsg = GetRsponeResult(wErrCode);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame11::GetStrFrameType()
{
	return PASS_ALL_GUIDE;
}

void CDealFrame11::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame11::GetStrFrameDesc()
{
	return SDGBK2UTF8(PASS_ALL_GUIDE_DESC);
}

