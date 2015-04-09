///<------------------------------------------------------------------------------
//< @file:   gvg\local\gvggiftmgr.h
//< @author: Kang_zhiquan
//< @date:   2014年5月7日 20:25:48
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvggiftmgr_h_
#define _gvggiftmgr_h_

#include <map>
#include <sdsingleton.h>
#include "protocol/server/protocligs.h"
#include "logic/base/basedefine.h"

enum EM_RESULT
{
	GIFT_SUCCESS = 0,//成功
	GIFT_ID_NOT_EXIST = 1,//ID不存在
	GIFT_BAG_FULL = 2,//背包满
};
using namespace std;

typedef vector<DT_GVG_GIFT_REWARD_DATA> CVecGiftRewardData;
typedef CVecGiftRewardData::iterator CVecGiftRewardDataItr;

class CPlayer;

class CGvGGiftMgr 
{
	DECLARE_SINGLETON(CGvGGiftMgr)
	typedef map<UINT64, DT_GVG_GIFT_REWARD_DATA> CMapGvGGiftReward;
	typedef CMapGvGGiftReward::iterator CMapGvGGiftRewardItr;

public:
	CGvGGiftMgr()
	{
		m_mapGiftReward.clear();
		m_vecAddToDBGift.clear();
	}
	~CGvGGiftMgr()
	{
		m_mapGiftReward.clear();
		m_vecAddToDBGift.clear();
	}
	BOOL Init();
	//去掉过期礼包
	VOID RemoveOverGift();
	//获取自己礼包信息(客户端)
	VOID GetGiftInfo(UINT32 dwPlayerID, C64Set& setKey, DT_GVG_GIFT_DATA_LST_CLI& stGiftCli);
	//获取自己礼包信息(服务端)
	BOOL GetGiftInfo(UINT64 qwKey, DT_GVG_GIFT_REWARD_DATA& stData);
	//获取礼包物品
	EM_RESULT AddGiftGoods(CPlayer* poPlayer, UINT64 qwKey);
	//获取自己未领取的礼包ID
	VOID GetUnGetGiftKey(UINT32 dwPlayerID, C64Set& getKey, C64Set& ungetKey);
	//获得连斩奖励
	BOOL AddContinueKill(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvGRound, UINT8 byBattleIdx, UINT8 byKillNum);
	//添加战斗礼包
	VOID AddBattleReward(UINT32 dwFactionID, UINT8 byRound, UINT8 byBattleIdx, const TCHAR* aszFactionName, UINT8 byResult);
	//添加名次奖励
	VOID AddRankReward(UINT32 dwFactionID, UINT8 byRound, UINT8 byRank);
	//添加荣誉奖励
	VOID AddHononReward( UINT32 dwFactionID);

	//添加礼包进DB
	VOID AddGiftTODB();
	//要添加的礼包ID
	UINT8 GetBattleGiftID(UINT8 byRound, UINT8 byResult);
	UINT8 GetRankGiftID(UINT8 byRound, UINT8 byRank);
public:
	//礼包的KEY
	UINT64 GetGiftKey(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvGRound, UINT8 byBattleIdx);
	//连斩的KEY
	UINT64 GetContinueKillKey(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvgRound, UINT8 byBattleIdx);
private:
	//添加战斗礼包
	VOID AddBattleRewardByPlayerID(UINT32 dwPlayerID, UINT8 byRound, UINT8 byBattleIdx, const TCHAR* aszFactionName, UINT8 byResult);
	//添加名次奖励
	VOID AddRankRewardByPlayerID(UINT32 dwPlayerID, UINT8 byRound, UINT8 byRank);
	//添加荣誉奖励
	VOID AddHononRewardByPlayerID(UINT32 dwPlayerID, TCHAR* aszFactionName);
	//添加礼包
	VOID AddGiftReward(DT_GVG_GIFT_REWARD_DATA& stData);
	BOOL LoadGiftFromDB();
private:
	CVecGiftRewardData m_vecAddToDBGift;//要加入数据库中的
	CMapGvGGiftReward m_mapGiftReward;//已经存入DB中的礼包
};
#endif //_gvggiftmgr_h_