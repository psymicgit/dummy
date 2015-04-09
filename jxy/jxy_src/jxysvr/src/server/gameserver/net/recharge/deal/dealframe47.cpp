// DealFrame47.cpp: implementation of the CDealFrame47 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe47.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include "logic/item/item.h"
#include "logic/item/equip/equip.h"
#include "logic/bag/bag.h"
#include <logic/player/player.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame47::CDealFrame47()
{

}

CDealFrame47::~CDealFrame47()
{

}

void CDealFrame47::Release()
{
	
}

UINT16 CDealFrame47::GetFrameType()
{
	return 47;
}

void CDealFrame47::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if(NULL == poPlayer)
	{
		poClinet->Rspone("{\"status\":\"Player no in mem!\"}");
		return;
	}

	vector<string> vecItemID;
	vecItemID = SDSplitStringsA(pszParam4, ',');
	UINT32 dwSize = vecItemID.size();
	if(0 == dwSize)
	{
		poClinet->Rspone("{\"status\":\"param4 invalid!\"}");
		return;
	}

	CNGString ssRet;
	CBag& oBag = poPlayer->GetBag();
	for(UINT8 byIdx = 0; byIdx < dwSize; byIdx++)
	{
		UINT16 wItemID = SDAtou(vecItemID[byIdx].c_str());
		UINT16 wNum = 0;
		if(wItemID < EQUIP_END_KINDID)
		{
			CPlayerEquipShmemMap& mapEquip = poPlayer->GetAllEquip();
			CEquip* poEquip = mapEquip.GetFistData();
			while(poEquip)
			{
				if(wItemID == poEquip->GetDT_EQUIP_DATA().wKindID)
				{
					wNum++;
				}
				poEquip = mapEquip.GetNextData();
			}
		}
		else
		{
			wNum = oBag.GetGoodsPileNum(wItemID);
		}
		if(0 != byIdx)
		{
			ssRet += ",";
		}
		ssRet += "ID=";
		ssRet += wItemID;
		ssRet += "|Num=";
		ssRet += wNum;
	}
	string strRet = ssRet.str();
	poClinet->Rspone(strRet);
}

void CDealFrame47::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty() || mapField[CK_ITEM_NUM_VALUE_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	string strItemInfo = mapField[CK_ITEM_NUM_VALUE_PARAM];
	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);

	if(NULL == poPlayer)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	vector<string> vecItemID;
	vecItemID = SDSplitStringsA(strItemInfo.c_str(), ',');
	UINT32 dwSize = vecItemID.size();

	if(0 == dwSize)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	vector<string>JsonData;
	CBag& oBag = poPlayer->GetBag();
	for(UINT8 byIdx = 0; byIdx < dwSize; byIdx++)
	{
		UINT16 wItemID = SDAtou(vecItemID[byIdx].c_str());
		UINT16 wNum = 0;
		if(wItemID < EQUIP_END_KINDID)
		{
			CPlayerEquipShmemMap& mapEquip = poPlayer->GetAllEquip();
			CEquip* poEquip = mapEquip.GetFistData();
			while(poEquip)
			{
				if(wItemID == poEquip->GetDT_EQUIP_DATA().wKindID)
				{
					wNum++;
				}
				poEquip = mapEquip.GetNextData();
			}
		}
		else
		{
			wNum = oBag.GetGoodsPileNum(wItemID);
		}
		CNGString ssRet;
		ssRet = "{";
		ssRet += "\"ID\":";
		ssRet += wItemID;
		ssRet += ",\"Num\":";
		ssRet += wNum;
		ssRet += "}";
		JsonData.push_back(ssRet.c_str());
	}

	string strRetInfo = "";
	strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &JsonData);
	poClinet->Rspone(strRetInfo.c_str());
}

string CDealFrame47::GetStrFrameType()
{
	return CK_ITEM_NUM;
}

void CDealFrame47::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
	mapField[CK_ITEM_NUM_VALUE_PARAM] = SDGBK2UTF8(CK_ITEM_NUM_GBK) + "|" + GM_PARAM_STR;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame47::GetStrFrameDesc()
{
	return SDGBK2UTF8(CK_ITEM_NUM_DESC);
}

