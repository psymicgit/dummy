///<------------------------------------------------------------------------------
//< @file:   logic\soul\playersoul.h
//< @author: Kang_zhiquan
//< @date:   2014年6月12日 13:35:28
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playersoul_h_
#define _playersoul_h_
#include "logic/base/baseobj.h"
#include "protocol/server/protocommondata.h"
#include "protocol/server/protocligs.h"
#include <sdtype.h>
#include <map>
#include <logic/base/basedefine.h>
#include <utility.h>

using std::map;

class CSoul;

typedef map<UINT64, CSoul> CMapSoul;
typedef CMapSoul::iterator CMapSoulItr;

class CPlayerSoul : public CBaseObj
{
public:
	CPlayerSoul();
	~CPlayerSoul();
	BOOL		Init(const DT_PLAYER_SOUL_DATA& stLstData, CPlayer* poPlayer);
	DT_PLAYER_SOUL_DATA& GetDT_PLAYER_SOUL_DATA();
	
	// 添加魂到背包里
	BOOL		AddSoulToBag(CSoul* pSoul);
	// 从背包中删除战魂
	BOOL		DelSoulFromBag(CSoul* pSoul);
	// 魂背包是否已满
	BOOL		IsSoulBagFull();
	// 获取魂
	CSoul*		GetSoul(UINT16 usSoulId, UINT32 unIndex);

	VOID		GetSoulBagDataCli(DT_SOUL_DATA_LST_CLI& rSoulInfo, DT_SOUL_POTENCY_DATA_LST_CLI* stBagSoulPotencyInfo = NULL);

	UINT8		GetSoulBagLeftSize();

	UINT16		GenerateSoulIdx(UINT32 unSoulId);

	VOID		UpdateSoulData(CSoul* pSoul);

private:
	void		initSoulData();

private:
	DT_PLAYER_SOUL_DATA m_stSoulData;
	CMapSoul	m_mpSoulData;
	map<UINT32, UINT16> m_mpSoulIdx;
};

DECLARE_CLASS_POOL_MGR(CPlayerSoul)
#endif //_playersoul_h_
