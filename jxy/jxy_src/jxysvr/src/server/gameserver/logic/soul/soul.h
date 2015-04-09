///<------------------------------------------------------------------------------
//< @file:   logic\soul\soul.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ12ÈÕ 11:46:18
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _soul_h_
#define _soul_h_
#include "protocol/server/protocligs.h"
#include "logic/base/baseobj.h"
#include <sdobjectpool.h>
#include <sdloggerimpl.h>
#include "common/server/utility.h"
#include "logic/base/basedefine.h"
#include <list>
#include "logic/soul/souldef.h"
using namespace std;
class CSpecialEffect;
class CBattleAttrObj;

class CSoul : public CBaseObj
{
public:
	CSoul();
	~CSoul();
	BOOL			Init(CPlayer* poPlayer, const DT_SOUL_DATA& stData, const DT_SOUL_POTENCY_DATA& stSoulPotency, CBattleAttrObj* poMySelf = NULL);
	DT_SOUL_DATA&	GetDT_SOUL_DATA();
	DT_SOUL_POTENCY_DATA& GetDT_SOUL_POTENCY_DATA();
	BOOL			AddExeprience(UINT64 qwExperience);
	BOOL			SaveReflash(BOOL bIsSave);
	
	UINT32			GetExperience();
	BOOL			GetSoulCli(DT_SOUL_DATA_CLI& stCli);
	BOOL			GetSoulPotencyCli(DT_SOUL_POTENCY_CLI& stCli);
	VOID			Clear();
	BOOL			GetSoulPotencyDetailCli(DT_POTENCY_DATA_LST& stLstData);
	BOOL			GetSoulPotencyAddAttrInfo(DT_POTENCY_ATTR_LST_INFO& stAddAttr);
	eSoulUpdatePotency			UpdatePotency(DT_ITEM_NUM_DATA_LIST_CLI& stItemInfo);
private:
	DT_SOUL_DATA	m_stSoulData;
	DT_SOUL_POTENCY_DATA m_stSoulPotencyData;
};

DECLARE_CLASS_POOL_MGR(CSoul)
#endif //_soul_h_
