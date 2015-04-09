// DealFrame44.cpp: implementation of the CDealFrame44 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe44.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/chat/chatmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame44::CDealFrame44()
{

}

CDealFrame44::~CDealFrame44()
{

}

void CDealFrame44::Release()
{
	
}

UINT16 CDealFrame44::GetFrameType()
{
	return 44;
}

void CDealFrame44::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam5))
	{
		return;
	}
	CChatMgr::Instance()->AddSysChat(SDGBK2UTF8(pszParam4).c_str(), pszParam5, dwParam1);
	poClinet->Rspone("{\"status\":\"success!\"}");
}

void CDealFrame44::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[SYS_CHATDATA_INFO_PARAM].empty() || mapField[SYS_CHATDATA_EXPIRESTIME_PARAM].empty() || mapField[OPTTYPE_PARAM].empty() )
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	UINT8 unType = SDAtou(mapField[OPTTYPE_PARAM].c_str());
	string strInfo = mapField[SYS_CHATDATA_INFO_PARAM];
	string strTime = mapField[SYS_CHATDATA_EXPIRESTIME_PARAM];
	CChatMgr::Instance()->AddSysChat(strInfo, strTime, unType);

	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame44::GetStrFrameType()
{
	return SYS_CHATDATA;
}

void CDealFrame44::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[SYS_CHATDATA_INFO_PARAM] = SDGBK2UTF8(SYS_CHATDATA_GBK) + "|" + GM_PARAM_STR;
	mapField[SYS_CHATDATA_EXPIRESTIME_PARAM] = SDGBK2UTF8(SYS_CHATDATA_EXPIRESTIME_GBK) + "|" + GM_PARAM_DATATIME;
	mapField[OPTTYPE_PARAM] = SDGBK2UTF8(SYS_CHATDATA_TYPE_GBK) + "|" + GM_PARAM_INT;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame44::GetStrFrameDesc()
{
	return SDGBK2UTF8(SYS_CHATDATA_DESC);
}

