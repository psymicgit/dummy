///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframeoperatingactivity74.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê7ÔÂ29ÈÕ 14:15:46
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "dealframeoperatingactivity74.h"
#include "db/operatingactivitycmd.h"
#include "framework/gsapi.h"

CDealFrameOperatingActivity::CDealFrameOperatingActivity()
{

}

CDealFrameOperatingActivity::~CDealFrameOperatingActivity()
{

}

UINT16 CDealFrameOperatingActivity::GetFrameType()
{
	return EGMT_OPERATING_ACTIVITY;
}

string CDealFrameOperatingActivity::GetStrFrameType()
{
	return OPERATING_ACTIVITY_GM;
}

string CDealFrameOperatingActivity::GetStrFrameDesc()
{
	return SDGBK2UTF8(OPERATING_ACTIVITY_DESC);
}

void CDealFrameOperatingActivity::Interface(CRCClient* poClinet)
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[START_ZONE_ID] = SDGBK2UTF8(START_ZONE_ID_GBK)+ "|" + GM_PARAM_INT;
	mapField[END_ZONE_ID] = SDGBK2UTF8(END_ZONE_ID_GBK)+ "|" + GM_PARAM_INT;
	mapField[START_TIME] = SDGBK2UTF8(START_TIME_GBK) + "|" + GM_PARAM_DATATIME;
	mapField[END_TIME] = SDGBK2UTF8(END_TIME_GBK) + "|" + GM_PARAM_DATATIME;

	RsponeMsg(poClinet,mapField);
}

void CDealFrameOperatingActivity::Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField)
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[START_ZONE_ID].empty()
		||mapField[END_ZONE_ID].empty()
		|| mapField[START_TIME].empty()
		|| mapField[END_TIME].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	UINT32 dwStartZoneID = SDAtou(mapField[START_ZONE_ID].c_str());
	UINT32 dwEndZoneID = SDAtou(mapField[END_ZONE_ID].c_str());
	string strStartTime = mapField[START_TIME];
	string strEndTime = mapField[END_TIME];
	Deal(poClinet, dwSerialNumber, 0, dwAreaID, dwPlayerID, dwStartZoneID, dwEndZoneID, 0, (TCHAR*)strStartTime.c_str(), (TCHAR*)strEndTime.c_str(), NULL);
}

void CDealFrameOperatingActivity::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4) || (NULL == pszParam5))
	{
		return;
	}

	SOperatingActivity stActivity;
	stActivity.dwStartZoneID = dwParam1;
	stActivity.dwEndZoneID = static_cast<UINT32>(qwParam2);
	stActivity.strStartTime = pszParam4;
	stActivity.strEndTime = pszParam5;

	
	CBaseDBCMD* poDB = gsapi::GetDBMgr()->CreateCmd(EDBCT_OPERATING_ACTIVITY);
	if(NULL == poDB)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
		return ;
	}
	poDB->SetUserData(&stActivity);
	gsapi::GetDBMgr()->AddPayCommand(poDB);

	char szRspone[1024] = {0};
	sprintf(szRspone, "{\"status\":\"success!\"}");
	poClinet->Rspone(szRspone);
}

void CDealFrameOperatingActivity::Release()
{

}