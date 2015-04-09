// DealFrame31.cpp: implementation of the CDealFrame31 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe31.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/chat/chatmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame31::CDealFrame31()
{

}

CDealFrame31::~CDealFrame31()
{

}

void CDealFrame31::Release()
{
	
}

UINT16 CDealFrame31::GetFrameType()
{
	return 31;
}

void CDealFrame31::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	vector<string> vecField;
	vecField = SDSplitStringsA(pszParam4, '|');
	if(1 == dwParam1)
	{
		CChatMgr::Instance()->AddWordFilterData(vecField);
	}
	else if(2 == dwParam1)
	{
		CChatMgr::Instance()->DelWordFilterData(vecField);
	}
	poClinet->Rspone("{\"status\":\"success!\"}");
}

void CDealFrame31::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[WORD_FILTER_TYPE_PARAM].empty() || mapField[WORD_FILTER_INFO_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	string strCharInfo = mapField[WORD_FILTER_INFO_PARAM];
	UINT8 unType = SDAtou(mapField[WORD_FILTER_TYPE_PARAM].c_str());

	vector<string> vecField;
	vecField = SDSplitStringsA(strCharInfo.c_str(), '|');

	if(1 == unType)
	{
		CChatMgr::Instance()->AddWordFilterData(vecField);
	}
	else if(2 == unType)
	{
		CChatMgr::Instance()->DelWordFilterData(vecField);
	}

	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame31::GetStrFrameType()
{
	return WORD_FILTER;
}

void CDealFrame31::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[WORD_FILTER_TYPE_PARAM] = SDGBK2UTF8(WORD_FILTER_TYPE_GBK)  + "|" + GM_PARAM_INT;
	mapField[WORD_FILTER_INFO_PARAM] = SDGBK2UTF8(WORD_FILTER_INFO_GBK)  + "|" + GM_PARAM_STR;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame31::GetStrFrameDesc()
{
	return SDGBK2UTF8(WORD_FILTER_DESC);
}

