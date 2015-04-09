///<------------------------------------------------------------------------------
//< @file:   logic\medal\playermedal.h
//< @author: Kang_zhiquan
//< @date:   2014年6月18日 19:54:8
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playermedal_h_
#define _playermedal_h_
#include "logic/base/baseobj.h"
#include <map>
#include "protocol/server/protocligs.h"
#include "logic/base/basedefine.h"
#include "common/server/utility.h"
#include "common/client/commondef.h"
using namespace std;
class CMedal;
class CPlayerMedal : public CBaseObj
{
	typedef map<UINT16, CMedal*> CMapMedal;//Key为smallType, Value为勋章对像
	typedef CMapMedal::iterator CMapMedalItr;//

	typedef multimap<UINT16, CMedal*> CMultiMapMedal;
	typedef CMultiMapMedal::iterator CMultiMapMedalItr;

	typedef map<UINT32, CMedal*> CMapSmallLevelMedal;
	typedef CMapSmallLevelMedal::iterator CMapSmallLevelMedalItr;
public:
	CPlayerMedal();
	~CPlayerMedal();
	VOID Clear();
	BOOL Init(const DT_PLAYER_MEDAL_DATA&, CPlayer*);

	//校验失效
	VOID CheckLostMedal();
	VOID AddMedalToPlayer(CMedal*);
	DT_PLAYER_MEDAL_DATA& GetDT_PLAYER_MEDAL_DATA();

	//添加后期新增勋章
	VOID AddNewMedal();
	VOID AddNewMedalByBuild(UINT8 byBuildID);
	//每天检测一下
	VOID CheckEveryDay();
	//计算属性加成
	VOID InitBattleAttr();
	//获取属性固定值
	const C3232Map& GetBattleAttrFix();
	//获取属性百分比
	const C3232Map& GetBattleAttrPercent();
	//检测勋章
	BOOL CheckMedalBySmallType(UINT16 wSmallType);
	//获取其它信息
	DT_MEDAL_BASE_DATA& GetDT_MEDAL_BASE_DATA(){ return m_stMedalBaseData; }
	//检测登陆勋章
	BOOL CheckMedalByLogin();
public:
	////////////////////////////////日常计算//////////////////////////////////////////
	//计算连续登陆时间
	VOID ContinueLogin();
	//计算购买体力d
	VOID AddBuyPhystrength();
	//计算拦截次数
	VOID AddInterceptNum();
	//护送次数
	VOID AddEscortNum();
	//招财次数
	VOID AddRankMoneyNum();
	//普通聚魂次数
	VOID AddNormalGatherNum();
	//元宝聚魂次数
	VOID AddGoldGatherNum();
	//膜拜1级别次数
	VOID AddWorshipPlayer1Num();
	//膜拜2级别次数
	VOID AddWorshipPlayer2Num();
	//膜拜3级别次数
	VOID AddWorshipPlayer3Num();
	//拜神1级别次数
	VOID AddWorshipGod1Num();
	//拜神2级别次数
	VOID AddWorshipGod2Num();
	//拜神3级别次数
	VOID AddWorshipGod3Num();
	//张天师算卦次数
	VOID AddZTSNum();
	//笑脸个数
	VOID AddSmileNum(UINT32 dwSmileNum);
	//小黑屋抓捕次数
	VOID AddArrestNum();
	//小黑层驱赶次数
	VOID AddDriveNum();
	//爬塔重置次数
	VOID AddTowerResetNum();
	//精英副本重置次数
	VOID AddEliteInstanceResetNum();
	//增加试练次数
	VOID AddGVECourageNum();
	//让勋章失效
	VOID UnActivityMedal(MEDAL_SMALL_TYPE);
	//检测限时勋章
	VOID CheckLimitMedal(MEDAL_SMALL_TYPE, BOOL bIsUnActivity = TRUE);
	//设置竞技场排名
	BOOL SetRaceRank();
public:
	//添加可用勋章
	VOID AddCanUseMedal(CMedal* poMedal);
	UINT16 OnOpenMyMedal(PKT_CLIGS_OPEN_MY_MEDAL_ACK& stAck);
	VOID GetUseMedal(DT_MEDAL_DATA_LST_CLI& stCli);
protected:
	//处理失效勋章
	VOID DealLostMedal();
	//处理新生效勋章
	VOID DealEffectMedal();
private:
	CMapMedal m_mapAllMedal;//所有勋章，起作用勋章+不起作用勋章+未激活勋章
	CMultiMapMedal m_mapUseMedal;//起作用的勋章
	CMultiMapMedal m_multiMapAllMedal;//所有勋章
	CMapMedal m_mapUnlockMedal;//显示未激的勋章

	C3232Map m_mapBattleAttrFixed;//战斗属性固定值加成
	C3232Map m_mapBattleAttrPercent;//战斗属性百分比加成

	C1616MultiMap m_mapBuildID;//已添加勋章按建筑ID分类
	UINT64 m_qwLastUpdateTime ;
	DT_PLAYER_MEDAL_DATA m_stPlayerMedal;
	DT_MEDAL_BASE_DATA m_stMedalBaseData;
};

DECLARE_CLASS_POOL_MGR(CPlayerMedal)
#endif //_playermedal_h_