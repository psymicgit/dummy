// DealFrame41.cpp: implementation of the CDealFrame41 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe41.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame41::CDealFrame41()
{

}

CDealFrame41::~CDealFrame41()
{

}

void CDealFrame41::Release()
{
	
}

UINT16 CDealFrame41::GetFrameType()
{
	return 41;
}

void CDealFrame41::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, pszParam4, poClinet->GetCliSessionID(), pszDesc);
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

void CDealFrame41::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[FORBID_REGISTERED_USER_ID_PARAM].empty() || mapField[FORBID_REGISTERED_EQU_ID_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	string strEquipId =	mapField[FORBID_REGISTERED_EQU_ID_PARAM].c_str();
	UINT32 unUserId = SDAtou(mapField[FORBID_REGISTERED_USER_ID_PARAM].c_str());
	UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber,(UINT8)GetFrameType(), 0,unUserId,0,0,(char*)strEquipId.c_str(),poClinet->GetCliSessionID(), mapField["desc"].c_str());

	string strErrorMsg = GetRsponeResult(wErrCode);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame41::GetStrFrameType()
{
	return FORBID_REGISTERED;
}

void CDealFrame41::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[FORBID_REGISTERED_USER_ID_PARAM] = SDGBK2UTF8(FORBID_REGISTERED_USER_ID_GBK) + "|" + GM_PARAM_USER_ID;
	mapField[FORBID_REGISTERED_EQU_ID_PARAM] = SDGBK2UTF8(FORBID_REGISTERED_EQU_ID_GBK) + "|" + GM_PARAM_STR;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame41::GetStrFrameDesc()
{
	return SDGBK2UTF8(FORBID_REGISTERED_DESC);
}

