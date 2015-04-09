#include "dealframeaddjewelry.h"
#include "gmdef.h"
#include "logic/item/itemmgr.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
using namespace SGDP;
using namespace InterfaceDealBase;
CDealFrameAddJewelry::CDealFrameAddJewelry()
{

}

CDealFrameAddJewelry::~CDealFrameAddJewelry()
{

}

UINT16 CDealFrameAddJewelry::GetFrameType()
{
	return EGMT_ADD_JEWELRY;
}

string CDealFrameAddJewelry::GetStrFrameType()
{
	return ADD_JEWELRY_GM;
}

string CDealFrameAddJewelry::GetStrFrameDesc()
{
	 return SDGBK2UTF8(ADD_JEWELRY_DESC);
}

void CDealFrameAddJewelry::Interface(CRCClient* poClinet)
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
	mapField[ADD_JEWELRY_KINDID] = SDGBK2UTF8(ADD_ATTRIBUTE_1_GBK) + "|" + GM_PARAM_INT;
	mapField[ADD_JEWELRY_ATTR_NUM] = SDGBK2UTF8(ADD_ATTRIBUTE_2_GBK) + "|" + GM_PARAM_INT;
	mapField[ADD_JEWELRY_ATTR] = SDGBK2UTF8(ADD_ATTRIBUTE_3_GBK) + "|" + GM_PARAM_INT;

	RsponeMsg(poClinet,mapField);
}

void CDealFrameAddJewelry::Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField)
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty()
		|| mapField[ADD_JEWELRY_KINDID].empty()
		|| mapField[ADD_JEWELRY_ATTR_NUM].empty()
		|| mapField[ADD_JEWELRY_ATTR].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	UINT32 dwParam1 = SDAtou(mapField[ADD_JEWELRY_KINDID].c_str());
	UINT32 dwParam2 = SDAtou(mapField[ADD_JEWELRY_ATTR_NUM].c_str());
	UINT32 dwParam3 = SDAtou(mapField[ADD_JEWELRY_ATTR].c_str());

	Deal(poClinet, dwSerialNumber, 0, dwAreaID, dwPlayerID, dwParam1, dwParam2, dwParam3, NULL, NULL, NULL);
}

void CDealFrameAddJewelry::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc)
{
	if((NULL == poClinet))
	{
		return;
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if(NULL == poPlayer)
	{
		poClinet->Rspone("{\"status\":\"Player no in mem!\"}");
		return;
	}
	C32Vec vec32;
	UINT8 byNum = static_cast<UINT8>(qwParam2);
	for ( UINT8 byIdx = 0; byIdx < byNum; byIdx++)
	{
		UINT8 byAttr = (qwParam3 >> ((byNum - byIdx - 1) * 8)) & 0xFF;
		vec32.push_back(byAttr);
	}
	ECreateItemRet eRet;
	CItem* poItem = CItemMgr::Instance()->CreateJewelryIntoBagByGM(poPlayer, dwParam1, vec32, eRet);
	if ( NULL == poItem)
	{
		if ( ECIR_FAILED_BAG_FULL == eRet)
		{
			poClinet->Rspone("{\"status\":\"BAG FULL!\"}");
		}
		else if ( ECIR_FAILED_KIND_INVALID == eRet)
		{
			poClinet->Rspone("{\"status\":\"KIND_INVALID\"}");
		}
		else
		{
			poClinet->Rspone("{\"status\":\"FAILED_OTHER\"}");
		}
		return ;
	}
	else
	{
		poClinet->Rspone("{\"status\":\"success\"}");
	}
}

void CDealFrameAddJewelry::Release()
{

}