// DealFrame30.cpp: implementation of the CDealFrame30 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe30.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/chat/chatmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame30::CDealFrame30()
{

}

CDealFrame30::~CDealFrame30()
{

}

void CDealFrame30::Release()
{
	
}

UINT16 CDealFrame30::GetFrameType()
{
	return 30;
}

void CDealFrame30::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }
    vector<string> vecField;
    vecField = SDSplitStringsA(pszParam4, ',');
    UINT32 dwSize = vecField.size();
    if(dwSize > 3)
    {
        poClinet->Rspone("{\"status\":\"param invalid!\"}");
        return;
    }
    vector<string> vecDeviceID;
    vector<string> vecUserID;
    vector<string> vecPlayerID;
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        switch(dwIdx)
        {
        case 0:
            vecDeviceID = SDSplitStringsA(vecField[dwIdx], '|');
            break;
        case 1:
            vecUserID = SDSplitStringsA(vecField[dwIdx], '|');
            break;
        case 2:
            vecPlayerID = SDSplitStringsA(vecField[dwIdx], '|');
            break;
        default:
            break;
        }
    }
    if(1 == dwParam1)
    {
        CChatMgr::Instance()->AddSilenceData(vecDeviceID, vecUserID, vecPlayerID);
    }
    else if(2 == dwParam1)
    {
        CChatMgr::Instance()->DelSilenceData(vecDeviceID, vecUserID, vecPlayerID);
    }
    poClinet->Rspone("{\"status\":\"success!\"}");
}


void CDealFrame30::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[SILENCE_CHAT_TYPE_PARAM].empty() || mapField[SILENCE_CHAT_INFO_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	string strCharInfo = mapField[SILENCE_CHAT_INFO_PARAM];
	UINT8 unType = SDAtou(mapField[SILENCE_CHAT_TYPE_PARAM].c_str());

	vector<string> vecField;
	vecField = SDSplitStringsA(strCharInfo.c_str(), ',');
	UINT32 dwSize = vecField.size();

	if(dwSize > 3)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	vector<string> vecDeviceID;
	vector<string> vecUserID;
	vector<string> vecPlayerID;

	for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
	{
		switch(dwIdx)
		{
		case 0:
			vecDeviceID = SDSplitStringsA(vecField[dwIdx], '|');
			break;
		case 1:
			vecUserID = SDSplitStringsA(vecField[dwIdx], '|');
			break;
		case 2:
			vecPlayerID = SDSplitStringsA(vecField[dwIdx], '|');
			break;
		default:
			break;
		}
	}

	if(1 == unType)
	{
		CChatMgr::Instance()->AddSilenceData(vecDeviceID, vecUserID, vecPlayerID);
	}
	else if(2 == unType)
	{
		CChatMgr::Instance()->DelSilenceData(vecDeviceID, vecUserID, vecPlayerID);
	}

	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame30::GetStrFrameType()
{
	return SILENCE_CHAT;
}

void CDealFrame30::Interface( CRCClient* poClinet )
{

	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[SILENCE_CHAT_TYPE_PARAM] = SDGBK2UTF8(SILENCE_CHAT_TYPE_GBK)  + "|" + GM_PARAM_INT;
	mapField[SILENCE_CHAT_INFO_PARAM] = SDGBK2UTF8(SILENCE_CHAT_INFO_GBK) + "|" + GM_PARAM_STR;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame30::GetStrFrameDesc()
{
	return SDGBK2UTF8(SILENCE_CHAT_DESC);
}

