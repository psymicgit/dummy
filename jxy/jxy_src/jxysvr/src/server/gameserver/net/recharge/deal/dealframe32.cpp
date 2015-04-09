// DealFrame32.cpp: implementation of the CDealFrame32 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe32.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/chat/chatmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame32::CDealFrame32()
{

}

CDealFrame32::~CDealFrame32()
{

}

void CDealFrame32::Release()
{
	
}

UINT16 CDealFrame32::GetFrameType()
{
	return 32;
}

void CDealFrame32::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	CHAT_DATA  astChatData[100] = {0};
	UINT8 byChatNum = 0;
	CChatMgr::Instance()->GetChatData(astChatData, byChatNum);
	if(0 == byChatNum)
	{
		poClinet->Rspone("{\"status\":\"no data!\"}");
		return;
	}
	CNGString ssMsg;
	for(UINT8 byIdx = 0; byIdx < byChatNum; byIdx++)
	{
		CHAT_DATA& stCHAT_DATA = astChatData[byIdx];
		if(0 == byIdx)
		{
			ssMsg += "	PlayerID:";
		}
		else
		{
			ssMsg += "\n	PlayerID:";
		}
		ssMsg += stCHAT_DATA.dwPlayerID;
		ssMsg += "	DspName:";
		ssMsg += stCHAT_DATA.aszUserName;
		ssMsg += "	ChatType:";
		ssMsg += stCHAT_DATA.byChatType;
		ssMsg += "	ChatTime:";
		ssMsg += SDTimeToString(stCHAT_DATA.qwChatTime);
		ssMsg += "	Content:";
		ssMsg += (CHAR*)stCHAT_DATA.abyChatContent;
	}
	poClinet->Rspone(ssMsg.str());
}

void CDealFrame32::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if(NULL == poClinet)
	{
		return;
	}

	CHAT_DATA  astChatData[100] = {0};
	UINT8 byChatNum = 0;
	CChatMgr::Instance()->GetChatData(astChatData, byChatNum);

	if(0 == byChatNum)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	vector<string> vecJson;
	for(UINT8 byIdx = 0; byIdx < byChatNum; byIdx++)
	{
		CHAT_DATA& stCHAT_DATA = astChatData[byIdx];
		CNGString ssMsg;
		ssMsg += "{\"PlayerID\":\"";
		ssMsg += stCHAT_DATA.dwPlayerID;
		ssMsg += "\",\"DspName\":\"";
		ssMsg += stCHAT_DATA.aszUserName;
		ssMsg += "\",\"ChatType\":\"";
		ssMsg += stCHAT_DATA.byChatType;
		ssMsg += "\",\"ChatTime\":\"";
		ssMsg += SDTimeToString(stCHAT_DATA.qwChatTime);
		ssMsg += "\",\"Content\":";
		ssMsg += (CHAR*)stCHAT_DATA.abyChatContent;
		ssMsg += "\"}";
		vecJson.push_back(ssMsg.str());
	}

	string strRespon = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &vecJson);
	poClinet->Rspone(strRespon.c_str());
}

string CDealFrame32::GetStrFrameType()
{
	return GET_CHAT_MSG;
}

void CDealFrame32::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	RsponeMsg(poClinet,mapField);
}

string CDealFrame32::GetStrFrameDesc()
{
	return SDGBK2UTF8(GET_CHAT_MSG_DESC);
}

