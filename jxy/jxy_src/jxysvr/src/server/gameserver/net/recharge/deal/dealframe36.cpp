// DealFrame36.cpp: implementation of the CDealFrame36 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe36.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/chat/chatmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame36::CDealFrame36()
{

}

CDealFrame36::~CDealFrame36()
{

}

void CDealFrame36::Release()
{
	
}

UINT16 CDealFrame36::GetFrameType()
{
	return 36;
}

void CDealFrame36::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	CChatSilenceByDeviceIDMap& mapSilenceByDeviceID = CChatMgr::Instance()->GetSilenceByDeviceID();
	CChatSilenceByUserIDMap& mapSilenceByUserID = CChatMgr::Instance()->GetSilenceByUserID();
	CChatSilenceByPlayerIDMap& mapSilenceByPlayerID = CChatMgr::Instance()->GetSilenceByPlayerID();
	CNGString ssMsg;
	for(CChatSilenceByDeviceIDMapItr itr = mapSilenceByDeviceID.begin(); itr != mapSilenceByDeviceID.end(); itr++)
	{
		if(itr == mapSilenceByDeviceID.begin())
		{
			ssMsg += "DeviceID=";
			ssMsg += itr->first;
		}
		else
		{
			ssMsg += "|";
			ssMsg += itr->first;
		}
	}
	if(mapSilenceByDeviceID.size() > 0)
	{
		ssMsg += "\n";
	}

	/////////
	for(CChatSilenceByUserIDMapItr itr = mapSilenceByUserID.begin(); itr != mapSilenceByUserID.end(); itr++)
	{
		if(itr == mapSilenceByUserID.begin())
		{
			ssMsg += "UserID=";
			ssMsg += itr->first;
		}
		else
		{
			ssMsg += "|";
			ssMsg += itr->first;
		}
	}
	if(mapSilenceByUserID.size() > 0)
	{
		ssMsg += "\n";
	}

	////////////
	for(CChatSilenceByPlayerIDMapItr itr = mapSilenceByPlayerID.begin(); itr != mapSilenceByPlayerID.end(); itr++)
	{
		if(itr == mapSilenceByPlayerID.begin())
		{
			ssMsg += "PlayerID=";
			ssMsg += itr->first;
		}
		else
		{
			ssMsg += "|";
			ssMsg += itr->first;
		}
	}
	if(mapSilenceByPlayerID.size() > 0)
	{
		ssMsg += "\n";
	}

	if(ssMsg.str().empty())
	{
		poClinet->Rspone("{\"status\":\"no data!\"}");
	}
	else
	{
		poClinet->Rspone(ssMsg.str());
	}
}

void CDealFrame36::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if(NULL == poClinet)
	{
		return;
	}

	CChatSilenceByDeviceIDMap& mapSilenceByDeviceID = CChatMgr::Instance()->GetSilenceByDeviceID();
	CChatSilenceByUserIDMap& mapSilenceByUserID = CChatMgr::Instance()->GetSilenceByUserID();
	CChatSilenceByPlayerIDMap& mapSilenceByPlayerID = CChatMgr::Instance()->GetSilenceByPlayerID();

	vector<string>jsonData;
	CNGString ssMsg;
	ssMsg += "{\"DeviceID\":[";
	for(CChatSilenceByDeviceIDMapItr itr = mapSilenceByDeviceID.begin(); itr != mapSilenceByDeviceID.end(); itr++)
	{
		if(itr != mapSilenceByDeviceID.begin())
		{
			ssMsg += ",";
		}

		ssMsg += "\"";
		ssMsg += itr->first;
		ssMsg += "\"";
	}

	ssMsg += "],";

	/////////
	ssMsg += "\"UserId\":[";

	for(CChatSilenceByUserIDMapItr itr = mapSilenceByUserID.begin(); itr != mapSilenceByUserID.end(); itr++)
	{
		if(itr != mapSilenceByUserID.begin())
		{
			ssMsg += ",";
		}

		ssMsg += itr->first;
	}

	ssMsg += "],";

	////////////
	ssMsg += "\"PlayerID\":[";
	for(CChatSilenceByPlayerIDMapItr itr = mapSilenceByPlayerID.begin(); itr != mapSilenceByPlayerID.end(); itr++)
	{
		if(itr != mapSilenceByPlayerID.begin() )
		{
			ssMsg +=  ",";
		}

		ssMsg += itr->first;
	}

	ssMsg += "]}";
	jsonData.push_back(ssMsg.str());
	string strRetInfo = "";
	strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData);
	poClinet->Rspone(strRetInfo.c_str());
}

string CDealFrame36::GetStrFrameType()
{
	return GET_SLIENCE_LST;
}

void CDealFrame36::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	RsponeMsg(poClinet,mapField);
}

string CDealFrame36::GetStrFrameDesc()
{
	return SDGBK2UTF8(GET_SLIENCE_LST_DESC);
}

