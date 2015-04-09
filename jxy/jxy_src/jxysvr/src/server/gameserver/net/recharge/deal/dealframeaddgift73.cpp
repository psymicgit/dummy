#include "dealframeaddgift73.h"
#include "common/server/utility.h"
#include "framework/gsapi.h"
#include "db/addplayergift.h"
#include "logic/http/basehttpcmd.h"
#define ADD_GIFT_ZONE 1
#define ADD_GIFT_PLAYER 2
#define ADD_ALL_ZONE 3
CDealFrameAddGift::CDealFrameAddGift()
{

}

CDealFrameAddGift::~CDealFrameAddGift()
{

}
UINT16 CDealFrameAddGift::GetFrameType()
{
	return EGMT_ADD_GIFT;
}

string CDealFrameAddGift::GetStrFrameType()
{
	return ADD_GIFT_GM;
}

string CDealFrameAddGift::GetStrFrameDesc()
{
	return SDGBK2UTF8(ADD_GIFT_DESC);
}
void CDealFrameAddGift::Interface(CRCClient* poClinet)
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string, string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
	mapField[GET_RES_INFO_VALUE_PARAM] = SDGBK2UTF8(GET_RES_GBK) + "|" + GM_PARAM_STR;
	mapField[ADD_GIFT_NAME_AND_DESC] = SDGBK2UTF8(ADD_GIFT_NAME_AND_DESC_GBK) + "|" + GM_PARAM_STR;
	mapField[ADD_GIFT_OUT_TIME] = SDGBK2UTF8(GET_RES_GBK) + "|" + ADD_GIFT_OUT_TIME_GBK;

	RsponeMsg(poClinet, mapField);
}

void CDealFrameAddGift::Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField)
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty()
		|| mapField[GET_RES_INFO_VALUE_PARAM].empty()
		|| mapField[ADD_GIFT_NAME_AND_DESC].empty()
		|| mapField[ADD_GIFT_OUT_TIME].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	string strAddRes = mapField[GET_RES_INFO_VALUE_PARAM];
	string strGiftName = mapField[ADD_GIFT_NAME_AND_DESC];
	Deal(poClinet, dwSerialNumber, 0, dwAreaID, dwPlayerID, 0, 0, 0, (TCHAR*)strAddRes.c_str(), (TCHAR*)strGiftName.c_str(), NULL);
}

void CDealFrameAddGift::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4) || (NULL == pszParam5) )
	{
		return;
	}
	CHAR szRet[512] = {0};

	//增加物品解析
	vector<string> vecAddRes;
	multimap< string, vector<string> > mapAddRes;
	//检测对于没有值的都会传递默认值为0，要过虑这种没用的数据
	if (0 != SDStrcmp(pszParam4, "0"))
	{
		vecAddRes = SDSplitStringsA(pszParam4, ',');

		for(UINT8 byIdx = 0; byIdx < vecAddRes.size(); byIdx++)
		{
			vector<string> vecTmp = SDSplitStringsA(vecAddRes[byIdx], '|');
			if(vecTmp.size() < 2)
			{
				poClinet->Rspone("{\"status\":\"param5 sub param invalid!\"}");
				return;
			}
			mapAddRes.insert(make_pair(vecTmp[0], vecTmp));
		}
	}
	else
	{
		char szRspone[1024] = {0};
		sprintf(szRspone, "{\"status\":\"no resource!\"}");
		poClinet->Rspone(szRspone);
		return ;
	}
	CGenerateJson oJson;
	for (multimap< string, vector<string> >::iterator itr = mapAddRes.begin(); itr != mapAddRes.end(); itr++)
	{
		oJson.SetResByGm(itr->second[0], itr->second[1], itr->second);
	}

	//礼包结构体
	SActivityGift stGift;
	stGift.strJson = oJson.GetJson();
	vector<string> vecGift = SDSplitStringsA(pszParam5, '|');
	if ( 3 != vecGift.size() )
	{
		char szRspone[1024] = {0};
		sprintf(szRspone, "{\"status\":\"Param5 Error!\"}");
		poClinet->Rspone(szRspone);
		return ;
	}
	stGift.strGiftName = vecGift[0];
	stGift.strGiftDesc = vecGift[1];
	stGift.strTime = vecGift[2];
	stGift.dwStartPlayerID = dwPlayerID;
	stGift.dwEndPlayerID = dwPlayerID;
	stGift.dwZoneID = gsapi::GetZoneID();
	stGift.bySelectType = static_cast<UINT8>(qwParam2);
	if ( ADD_GIFT_PLAYER == dwParam1)
	{
		CBaseDBCMD* poDB = gsapi::GetDBMgr()->CreateCmd(EDBCT_ADD_PLAYER_GIFT);
		if(NULL == poDB)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
			return ;
		}
		poDB->SetUserData(&stGift);
		gsapi::GetDBMgr()->AddPayCommand(poDB);
	}
	else if (ADD_GIFT_ZONE == dwParam1)
	{
		
		CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_GET_MAX_PLAYERID_ADD_GIFT);

		if (NULL == poHttpCmd)
		{
			poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR).c_str());
			return ;
		}

		poHttpCmd->SetUserData(&stGift, sizeof(stGift), 0);
		poHttpCmd->Init();

		if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
		{
			poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR).c_str());
			return ;
		}
	}
	else
	{
		stGift.dwZoneID = 0;
		CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_GET_MAX_PLAYERID_ADD_GIFT);

		if (NULL == poHttpCmd)
		{
			poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR).c_str());
			return ;
		}

		poHttpCmd->SetUserData(&stGift, sizeof(stGift), 0);
		poHttpCmd->Init();

		if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
		{
			poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR).c_str());
			return ;
		}
	}

	char szRspone[1024] = {0};
	sprintf(szRspone, "{\"status\":\"success!\"}");
	poClinet->Rspone(szRspone);

}

void CDealFrameAddGift::Release()
{

}