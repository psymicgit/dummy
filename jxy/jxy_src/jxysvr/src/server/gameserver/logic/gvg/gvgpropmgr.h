///<------------------------------------------------------------------------------
//< @file:   gvgpropmgr.h
//< @author: hongkunan
//< @date:   2014年1月16日 20:28:49
//< @brief:  跨服门派战配置
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgpropmgr_h_
#define _gvgpropmgr_h_

#include "logic/base/basedefine.h"
#include "logic/base/jsonencourage.h"
#include "logic/base/basepropmgr.h"
#include "logic/gvg/gvgdef.h"

#include <sdsingleton.h>
#include <protocligs.h>

// 门派跨服战时间接口
namespace gvgtimeutil
{
    // 从指定时间所在周的周一凌晨算起，计算出一定天数后的时间
    UINT64 GetTimeFromWeek(UINT64 qwTime, UINT32 dwAddWeekTime);

    // 根据传入的周一~周日及当天时间，获取该周经过的秒数
    UINT32 GetTimeInWeek(UINT8 byWeek /* 1-7分别代表周一~周日 */, UINT32 dwDayTime);

    // 根据传入时间获取该周凌晨0点时间
    UINT64 GetWeekZeroTime(UINT64 qwTime);
}

// 跨服门派战各参数配置
struct SGvGProp
{
    SGvGProp(){ memset(this, 0, sizeof(*this)); }

    UINT32 dwForbidFormationTime;     // 距离比赛开始剩余多少时间时停止布阵（秒）
    UINT16 wRePullInterval;           // 调取玩家的时间间隔（秒）
    UINT16 wWinAddScore;              // 总决赛中获胜者获得的积分
    UINT16 wLoseAddScore;             // 总决赛中失败者获得的积分
    UINT32 dwChaosWarEachRoundTime;   // 门派大乱斗每轮的时间（秒）
    UINT32 dwFinalWarEachRoundTime;   // 四服积分总决赛每轮的时间（秒）
                                      
    UINT16 wJoinGVGPlayerLevel;       // 加入GVG要求玩家等级
    UINT32 dwJoinGVGFactionLevel;     // 加入GVG要求门派等级
    UINT32 dwJoinGVGRank;             // 加入GVG要求的门派排名
                
    UINT16 wHonorLevel;               // 荣誉奖励要求等级

    UINT16 wAutoJoinFactionCnt;       // 门派跨服战活动开启时让多少个门派自动报名
    UINT16 wAutoFormationCnt;         // 门派报名时让多少个成员自动上阵
    UINT16 bFakeActTime;              // 是否伪造跨服战时间
    BOOL bTest;                       // 是否是测试模式，测试模式下不自动发放任何奖励
    UINT32 dwFakeGap;                 // 伪造跨服门派战时间时每个轮次间的间隔（秒）
    UINT16 wOpenUpRequiredFactionCnt; // 一个区需要多少门派达到等级才能开启门派跨服战活动
    UINT16 wAutoApplyExceptZone;      // 禁止指定区自动报名（仅用于模拟测试）
    BOOL wDisplayVideoMVPCnt;         // 观看战斗时连斩达人列表的默认显示数量
    BOOL bUsePlayerSnapShot;          // 是否使用玩家快照
    BOOL bOpenActivity;               // 是否开放门派跨服战，跨服门派战的开关
};

// 连斩配置
struct SGvGKillComboProp
{
    SGvGKillComboProp(){ memset(this, 0, sizeof(*this)); }

    UINT16 wKillCombo;        // 连斩数
    UINT32 wDoorAward;        // 门贡奖励
    UINT16 wAddScore;         // 获取到的积分
    UINT8 byRecoverHPPercent; // 回血百分比，20代表百分之20

    UINT8 byEnegy;           // 对应精力值
    UINT8 byWeakenPercent;    // 全属性（攻防+4隐藏属性）削弱的比例，50代表全属性均变为原来的50%
};

// 门派战礼包
struct SGvGGiftProp
{
	SGvGGiftProp()
	{
		byGiftID = 0;
		strGiftName = "";
	}

	UINT8 byGiftID;
	string strGiftName;
	CJsonResItemVectorInfo stGiftValue;
};

// 押注奖励
struct SGvGGambleAward
{
    SGvGGambleAward(){ memset(this, 0, sizeof(*this)); }

    UINT16 wPlayerLevel;
    UINT32 dwDoubleWarCoin; // 双服押注铜钱奖励
    UINT32 dwFinalWarCoin;  // 四服押注铜钱奖励
};

class CGvGPropMgr : public CBasePopMgr
{
public:
    typedef std::vector<UINT64> GvGOpenDateVec;  // 各届跨服门派战活动的开始日期
    typedef std::vector<UINT32> GvGStageTimeVec; // 跨服门派战活动各个阶段的时间，阶段参见: GVG_STAGE
    typedef std::vector<DT_GVG_TIME_INFO> SGvGRoundTimeVec;
    typedef std::vector<SGvGKillComboProp> SKillComboVec;
    typedef std::vector<SGvGGambleAward> SGvGGambleAwardVec;

    typedef std::map<UINT16, GroupZoneIDMap> ArenaZoneIDMap; // 擂台配置map，值 = 擂台ID, 值 = 该擂台的区成员列表
    typedef std::map<ZoneID, ArenaID> ZoneIDToArenaIDMap;

	typedef map<UINT8, SGvGGiftProp> CMapGvGGiftProp;
    typedef CMapGvGGiftProp::iterator CMapGvGGiftPropItr;

    typedef std::map<UINT16, std::string> ZoneNameMap;
	typedef std::map<ArenaID, UINT16> UnLockMap; // key = 赛区ID，val = 该赛区将在第几届开放

public:
    DECLARE_SINGLETON_PROPMGR(CGvGPropMgr);
	CGvGPropMgr()
	{
		memset(m_aszGvGRule, 0x00, MAX_GVG_RULE);
	}
	~CGvGPropMgr()
	{
		memset(m_aszGvGRule, 0x00, MAX_GVG_RULE);
	}
public:
    virtual EConfigModule GetConfigModuleType() { return ECM_GVG; }

    BOOL Init();

    VOID UnInit();

    SGvGProp& GetGvGProp(){ return m_stGvGProp; }
    
    GvGStageTimeVec& GetGvGStageTimeVec(){ return m_vecStageTime; }

    DT_GVG_SCHEDULE_INFO& GetGvGSchedule(){ return m_stDisplaySchedule; }

	UINT16 GetGvGGameRule(TCHAR* pszRule);
	VOID GetGambleReward(DT_PLAYER_RES_DATA& stWinRes, GvGRound, UINT16 wPlayerLevel);

    // 获取各赛区成员区配置
    ArenaZoneIDMap& GetArenaZoneIDMap(){ return m_mapArenZoneID; }

    // 根据门派等级获取解锁阵位数
	UINT8 GetFormationUnlockPosNum(UINT32 dwFactionLevel);
	UINT8 GetMaxFormationPosNum();
	UINT8 GetNeedFactionLevelByPosNum(UINT8 byPosNum);
	BOOL GetGiftProp(UINT8 byID, SGvGGiftProp& stProp); 

    // 根据连斩数获取门贡
	UINT32 GetContinuKillDoorsTribute(UINT16 wKillCombo);

    // 根据连斩数获取积分
    UINT16 CalcScoreByKillCombo(GvGRound eRound, UINT16 wKillCombo);

    // 根据连斩数获取回血百分比
    UINT16 CalcRecoverHPByKillCombo(UINT16 wKillCombo);

    // 根据连斩数获取精力值
    UINT8 GetEnegyByKillCombo(UINT16 wKillCombo);

    // 根据连斩数获取削弱比例
    UINT8 GetWeakPercentByKillCombo(UINT16 wKillCombo);

    // 玩家到达到当前连斩数后，是否变得更为虚弱
    BOOL IsMoreWeaker(UINT16 wKillCombo);

    // 获取荣誉奖励要求的玩家等级
	UINT16 GetHonorLevel();

    // 获取某届跨服门派战指定阶段的开启时间
    UINT64 GetStageStartTime(UINT64 qwActivityStartTime, GVG_STAGE);

    // 伪造跨服门派战的活动时间，方便用于测试
    void FakeGvGTime(UINT64 qwActivityStartTime, GVG_STAGE);

    // 获取指定届的活动开启时间
    UINT64 GetOpenDateBySession(UINT16 wSessionID);

    // 获取指定赛区的活动开放时间
    UINT64 GetOpenDateByArena(ArenaID);

    // 获取指定区的活动开放时间
    UINT64 GetOpenDateByZone(ZoneID);

    // 获取指定区的区名
    const std::string& GetZoneName(ZoneID);

    // 获取指定区所属的赛区id
    ArenaID GetArenaIDByZoneID(ZoneID);

    // 获取参与跨服门派战所需的玩家等级
    UINT16 GetJoinGVGPlayerLevel(){ return m_stGvGProp.wJoinGVGPlayerLevel; }
    
    // 获取报名跨服门派战所需的门派等级
    UINT32 GetJoinGVGFactionLevel(){ return m_stGvGProp.dwJoinGVGFactionLevel; }

    // 获取报名跨服门派战所需的门派排名
    UINT32 GetJoinGvGRank(){ return m_stGvGProp.dwJoinGVGRank; }

    // 是否伪造跨服门派战活动时间
    BOOL IsFakeTime(){ return m_stGvGProp.bFakeActTime; }

    // 是否处于测试模式
    BOOL IsTest(){ return m_stGvGProp.bTest; }

    // 赛区的各成员区是否均已开放跨服门派战
    BOOL IsArenaOpenUp(ArenaID);

    // 打印出指定赛区中未开放跨服门派战的成员区列表
    void PrintNotOpenUpArenaZone(ArenaID);

    // 获取解锁届数
    UINT16 GetUnLockSession(ArenaID);

    // 转换成客户端面板上显示的门派跨服战进度
    VOID ConvertToClientDisplayTimeByDate(UINT64 qwOpenDate);
    VOID ConvertToClientDisplayTime(UINT16 wSession);

    // 将轮次时间点转化为阶段时间点，以方便服务端进行定时
    BOOL TranslateIntoStageSchedule();

public:
    // 载入活动参数
    BOOL LoadGvGPropDB();
    
    // 载入活动各轮次时间配置
    BOOL LoadGvGTimePropDB();
    
    // 载入各届解锁时间配置
    BOOL LoadGvGOpenDatePropDB();
    
    // 载入各赛区解锁时间配置
    BOOL LoadGvGUnLockPropDB();
    
    // 载入连斩奖励配置
    BOOL LoadGvGKillComboPropDB();
    
    // 载入各赛区的区组成配置
    BOOL LoadGvGArenaPropDB();
    
    // 载入门派布阵阵位开放配置
	BOOL LoadGvGFormationPropFromDB();
    
    // 载入活动礼包配置
    BOOL LoadGvGGiftPropFromDB();
    
    // 载入押注奖励配置
	BOOL LoadGvGGambleAwardPropFromDB();
    
    // 载入区名配置
    BOOL LoadZoneNameDB();

private:
    VOID Clear();

private:
    // 跨服门派战各参数配置
    SGvGProp m_stGvGProp;

	TCHAR m_aszGvGRule[MAX_GVG_RULE]; //GVG规则
	C88Map m_mapFormationProp;	      //门派等级对应的阵型位置解锁数
	C88Map m_mapPosNumToFactionLevel; //位置对应的门派等级
	CMapGvGGiftProp m_mapGvGGiftProp; //门派奖励配置

    // 门派跨服战活动各轮次时间点
    SGvGRoundTimeVec m_vecRoundTime;

    // 门派跨服战活动各阶段时间点
    GvGStageTimeVec m_vecStageTime;

    // 玩家达到某个连斩数对应的奖励
    SKillComboVec m_vecKillCombo;

    // 客户端面板上显示的门派跨服战进度，缓存起来用于方便跟客户端同步（注意，这个时间服务器是用不到的！）
    DT_GVG_SCHEDULE_INFO m_stDisplaySchedule;

    // 擂台表，配置每个擂台的区成员
    ArenaZoneIDMap m_mapArenZoneID;

    // 记录每个区各自属于哪个赛区
    ZoneIDToArenaIDMap m_mapZoneIDToArenaID;

    // 各届活动的开启日期
    GvGOpenDateVec m_vecOpenDate;

    // 各个赛区的解锁配置
    UnLockMap m_mapUnLock;

    // 各区区名列表
    ZoneNameMap m_mapZoneName;

    // 押注奖励列表
    SGvGGambleAwardVec m_vecGambleAward;
};

#endif //_gvgpropmgr_h_