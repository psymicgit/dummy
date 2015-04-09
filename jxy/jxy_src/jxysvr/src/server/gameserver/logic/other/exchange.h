///<------------------------------------------------------------------------------
//< @file:   logic\other\exchange.h
//< @author: Kang_zhiquan
//< @date:   2014年8月27日 21:14:18
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _exchange_h_
#define _exchange_h_
#include "logic/base/basepropmgr.h"
#include <map>
#include "protocol/server/protocligs.h"
class CPlayer;
struct SExchangeItemProp
{
	UINT16 wGoodsID;	//物品ID

	UINT8 byResType;	//资源类型
	UINT32 dwResValue;	//资源值
};

typedef map<UINT16, SExchangeItemProp> CMapExchangeItem;
typedef CMapExchangeItem::iterator CMapExchangeItemItr;

class CExchangePropMgr : public CBasePopMgr
{
	DECLARE_SINGLETON_PROPMGR(CExchangePropMgr);
public:
	
	virtual EConfigModule	GetConfigModuleType() { return ECM_EXCHANGE ; }

	BOOL Init();

	VOID UnInit();

	SExchangeItemProp* GetExchangeItem(UINT16 wGoodsID);

public:
	VOID GetResInfo(CPlayer* poPlayer, DT_PLAYER_RES_LST_DATA& stRes);
	UINT16 OpenExchagneStroe(CPlayer* poPlayer, DT_PLAYER_RES_LST_DATA& stResInfo, DT_GOODS_EXCHANGE_LST_INFO& stItemInfo);
	UINT16 ExchangeItem(CPlayer* poPlayer, PKT_CLIGS_EXCHANGE_GOODS_REQ& stReq, PKT_CLIGS_EXCHANGE_GOODS_ACK& stAck);
protected:
	BOOL LoadCfgFromDB();
private:
	CMapExchangeItem m_mapExchangeProp;
};
#endif //_exchange_h_