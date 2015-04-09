///<------------------------------------------------------------------------------
//< @file:   gvgbuild.h
//< @author: 
//< @date:   2014年1月18日 17:18:37
//< @brief:  跨服战建筑，每个gamesvr有且只有一个
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgbuild_h_
#define _gvgbuild_h_

#include "logic/gvg/gvgdef.h"
#include "logic/activity/activity.h"

#include <list>
#include <sdsingleton.h>
#include "protocol/server/protocligs.h"

#define SAVE_GVG_BUILD_INTERVAL 20

typedef struct tagPKT_PKGS_SYNC_GVG_NTF PKT_PKGS_SYNC_GVG_NTF;

typedef list<DT_FACTION_APPLAY_DATA> CFactionApplayDataLst; // 报名门派列表
typedef CFactionApplayDataLst::iterator CFactionApplayDataLstItr;

typedef map<UINT16, DT_SIMPLE_FACTION_INFO> CMapSimpleFaction; // 财力比拼和大乱斗结果列表
typedef CMapSimpleFaction::iterator CMapSimpleFactionItr;

typedef map<UINT16, DT_GVG_FACTION_RANK_DATA> CMapFactionLastRank; // 门派排名列表
typedef CMapFactionLastRank::iterator CMapFactionLastRankItr;

typedef set<UINT32> C32Set;
typedef C32Set::iterator C32SetItr;

typedef struct tagDT_FACTION_SUPPORT_LIST DT_FACTION_SUPPORT_LIST;

enum{ FIRST_RANK = 1, LAST_RNAK = 4};

class CPlayerGVG;

// 存储2个门派间的战斗结果信息
struct SGvGFightResult
{
    SGvGFightResult(){ memset(this, 0, sizeof(*this)); }

    VideoID videoID;
    UINT32 dwFactionID1;
    UINT32 dwFactionID2;

    GvGRound eBigRound;
    UINT8 bySmallRound;

    GvGResult eResult; // 胜败结果
};

typedef std::map<VideoID, SGvGFightResult> GvGFightResultMap; // 各门派间的对战结果，key = 对战录像id，value = 对战结果

// 门派跨服战建筑，每个gamesvr有且只有一个
class CGvGBuild
{
    DECLARE_SINGLETON(CGvGBuild)

    typedef std::vector<CFaction*> CFactionVec;    

public:
    CGvGBuild()
        : m_eStage(GVG_0_NOT_START)
        , m_eRound(GVG_SLEEPING)
        , m_bGiveChaosWarAward(FALSE)
        , m_bGiveFinalWarAward(FALSE)
    {
		memset(&m_stGvGBuildData, 0, sizeof(DT_GVG_BUILD_DATA));
		m_stFactionApplayDataLst.clear();
		
    }
	~CGvGBuild()
	{
		memset(&m_stGvGBuildData, 0, sizeof(DT_GVG_BUILD_DATA));
		m_stFactionApplayDataLst.clear();
		m_eRound = GVG_SLEEPING;
		
	}
public:
    BOOL Init();
    BOOL LoadBuildDataFromDB();
	VOID InitData();
	VOID Run();
	VOID SaveToDB();

public:
    // 进入跨服战的下一轮
    void EnterStage(GVG_STAGE);

    // 接收到跨服战服务器发送的进行下一轮消息
    void OnEnterStageNotify(GVG_STAGE);

    // 向跨服战服务器
    void SendGetGvGActivityReq();

    // 恢复跨服战的进度
    void ReEnterStage();

    // 处理来自跨服战服务器的同步通知
    void HandleSyncGvGNtf(PKT_PKGS_SYNC_GVG_NTF&);

    // 处理来自跨服战服务器的同步门派支持人数通知
    void HandleSyncSupportNtf(DT_FACTION_SUPPORT_LIST&);

    // 取出本赛区的录像id列表
    void TakeRequiredVideoIDList(VideoIDSet &mapVideoIDs);

    // 指定阶段是否已准备就绪
    BOOL IsReadyByStage(GVG_STAGE eStage);

    // 判断指定阶段是否已完成
    BOOL IsLogicReady(GVG_STAGE eStage);

    // 本区门派自动报名（供测试用）
    void AutoApply();

    // 开始本届门派跨服战活动
    void GvGStart();

    // 清除本区的跨服门派战残留数据
    void Clear();

    // 进入本届跨服战的休眠期
    void GvGSleep();

    // 结束本届门派跨服战活动
    void GvGOver();

    // 给予跨服战勋章
    void GiveMedal();

    // 检测本区是否开启门派跨服战
    void CheckOpenUp();

    // 本赛区跨服门派站是否仍未解锁
    BOOL IsLockActivity();

    // 当前是否处于跨服门派战进程（未开启和已结束不属于进程中）
    BOOL IsActivityTime();

    // 重新映射门派跨服战的活动数据
    void Remapping();

    // 跨服门派战活动是否已开放
    inline BOOL IsOpenUp();

    // 本区是否满足开启跨服门派战活动的条件
    BOOL CanOpenUp();
    
    // 判断指定阶段是否已被标识为已完成
    BOOL IsStageDone(GVG_STAGE);

    // 设置指定阶段的标志为：已完成
    void SetStageDone(GVG_STAGE);

    // 重置各阶段的标志位
    void ResetStageFlag();

    // 当前是否正在进行战斗
    BOOL IsInWar();

    // 是否即将开始战斗
    BOOL IsWillWar();

    // 获取本区的跨服战数据
	const DT_GVG_BUILD_DATA& GetDT_GVG_BUILD_DATA(){ return m_stGvGBuildData; }

public:
    // 清除本区所有门派的过期跨服战门派战况
    void ClearExpiredFactionGvG();

    // 添加门派对战结果
    void AddGvGFightResult(SGvGFightResult&);

    // 获取门派对战结果，返回false说明找不到对战结果
    BOOL GetGvGFightResult(VideoID, SGvGFightResult&);

    // 查找门派对战结果
    SGvGFightResult* FindGvGFightResult(VideoID);

    // 重新更新各门派信息
    void UpdateGvGFaction();

    // 添加门派列表
    void AddGvGFaction(DT_GVG_FACTION astFactionList[], UINT8 byFactionCnt);

    // 更新各门派间的对战列表
    void UpdateFightResult();

    // 分派门派战况到本服的门派中
    void DispatchGvGLog();

    // 将缩略战况扩展成完整战况以方便存入
    void SimpleGvGLogToFullLog(DT_FACTION_GVG_LOG_LIST &stLogList, DT_FACTION_SIMPLE_GVG_LOG_LIST &stSimpleLogList);

    // 刷新本轮所处的大轮次
    void SetStage(GVG_STAGE);

    // 门派大乱斗是否已进行完毕（所有轮次均完结）
    BOOL IsChaosWarEnd();
    
    // 四服总决赛是否已进展进行完毕（所有轮次均完结）
    BOOL IsFinalWarEnd();

    // 随机配对门派大乱斗的选手
    void RollChaosWarSeed();

    // 进入门派大乱斗轮次
    void ChaosWarBegin();

    // 进入门派大乱斗
    void ChaosWarGo();

    // 打印报名门派
    void PrintApplyList();

    // 打印本区的前8强：财力大比拼前4名、门派大乱斗前4名
    void PrintLocalTop8();
    
    // 打印本赛区的最终排名
    void PrintFinalWinners();

    void EraseFactionOfflinePlayers(CFactionVec &vecFaction);

    // 获取门派大乱斗的种子选手
    void GetChaosWarSeed(CFactionVec &vecSeed);

	//获取当前第几届跨服战
	UINT16 GetCurSession(){ return m_stGvGBuildData.wSessionID; }

	//获取当前阶段
	GVG_STAGE GetCurStage(){ return m_eStage; }

    //获取当前轮次
	GvGRound GetCurRound(){ return m_eRound; }

    // 比赛报名
	BOOL ApplyGVGGame(UINT32 dwFactionID);

	//是否是财力大比拼
	BOOL IsMoneyGame(UINT32 dwFactionID);

    // 获取当前轮次的面板上应显示的倒计时
    UINT32 GetDisplayCountDownTime();

    // 获取指定阶段的开启时间
    UINT64 GetStageTime(GVG_STAGE);

    // 获取16进8、8进4面板上显示的下一轮
    GvGRound GetNextRoundAt16To8To4Panel();

    // 存储最后决赛的排名
    void StoreLastWinners();

public:
	///////////////协议////////////////
// 一级面板
    // 报名面板
    UINT16 OpenGVGApplyPanel(PKT_CLIGS_GVG_APPLAY_ACK& stAck, CPlayerGVG* poGvG);//打开报名阶段面板
    
    // 财力比拼面板
    UINT16 OpenGvGMoneyWarAck(PKT_CLIGS_GVG_MONEY_CHAOS_ACK& stAck, CPlayerGVG* poGvG);//财力大比拼阶段(还未结束)

    // 本服门派大乱斗面板
    UINT16 OpenGvGChaosWarAck(PKT_CLIGS_GVG_MONEY_CHAOS_ACK& stAck, CPlayerGVG* poGvG);//大乱斗阶段（还未开始）
    
    // 双服16进8、8进4淘汰赛面板
    UINT16 OpenGVG16To8To4Ack(CPlayer* poPlayer, PKT_CLIGS_16_TO_8_TO_4_ACK& stAck);

    // 四服积分总决赛面板
    UINT16 OpenGVGFinalWarAck(CPlayer* poPlayer, PKT_CLIGS_FINAL_WAR_ACK& stAck);

    // 本届冠亚季军面板
    UINT16 OpenGVGChampionAck(CPlayer* poPlayer, PKT_CLIGS_GVG_CHAMPION_ACK& stAck);
    
    // 门派跨服战未开启提示
    void OpenGVGNotOpenMsgAck(CPlayer* poPlayer, PKT_CLIGS_GVG_ERR_MSG_ACK&);


// 二级面板
	UINT16 GetFactionMoneyRank(PKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK& stAck);//打开财力排行
	UINT16 GetApplayFaction(PKT_CLIGS_OPEN_ALREAD_APPLAY_ACK& stAck);//打开已报名门派名单 
	UINT16 OpenGVGFormation(PKT_CLIGS_OPEN_GVG_FORMATION_ACK& stAck, CPlayer* poPlayer);//打开GVG门派阵型布置
	UINT16 GetGambleInfoAck(PKT_CLIGS_GVG_OPEN_GAMBLE_REQ& stReq, PKT_CLIGS_GVG_OPEN_GAMBLE_ACK& stAck, CPlayerGVG* poGvG);
	void GetLastResult(DT_GVG_FACTION_RANK_LST_DATA&);
	UINT16 FactionApplyAck(CPlayer* poPlayer);

    // 观看门派战斗录像
    UINT16 SeeGvGVideo(PKT_CLIGS_SEE_GVG_VIDEO_ACK_v410&, UINT64 qwVideoID, BYTE byVideoNum);
	
    // 结束观看门派战斗录像
    UINT16 EndGvGVideo(PKT_CLIGS_END_GVG_VIDEO_ACK_v410&, UINT64 qwVideoID, BYTE byEndType);

    // 查看门派战斗的MVP列表
    UINT16 ShowGvGMVP(PKT_CLIGS_SHOW_GVG_MVP_ACK&, UINT64 qwVideoID);

    // 查看门派战况
    UINT16 OpenGvGLog(PKT_CLIGS_OPEN_GVG_LOG_ACK&, CPlayer&, UINT32 dwFactionID);

public:
    //是否是布阵时间: True 为在布阵时间， FALSE 为非布阵时间
    BOOL IsFormationTime();

    //是否报名： True 为已报名， False 为未报名
    BOOL IsApply(UINT32 dwFactionID);

    // 是否是种子选手
    BOOL IsSeedFaction(UINT32 dwFactionID);

    //当前是否处于活动报名时段
    BOOL IsApplyTime();

    // 获取本届的开启时间
    UINT64 GetStartTime();

    // 门派成员是否能离开指定门派
    BOOL CanLeaveFaction(UINT32 dwFactionID);

    // 获取本届的结束时间
    UINT64 GetEndTime();

    // 获取当前显示的门派战况条数
    UINT16 GetReadableGvGLogCnt(DT_FACTION_GVG_LOG_LIST &stLogList);

    // 支持指定门派
    void AddFactionSupport(UINT32 dwFactionID);

    // 获取本区的前8强
    BOOL TakeLocalWarWinners(CFactionVec &vecTopFaction);
    
    // 本区所需的录像是否都已经从跨服战服务器调取过来
    BOOL IsAllVideoHere();

private:
    // 是否需要隐藏该战报
    BOOL IsNeedHideGvGLog(UINT8 byBigRound, UINT8 bySmallRound);

    // 该战报是否已得出结果
    BOOL IsGvGLogStart(UINT8 byBigRound, UINT8 bySmallRound);

    // 获取本区门派的战况
    void GetLocalFactionGvGLog(DT_FACTION_GVG_LOG&, CFaction&);

    // 获取别区门派的战况
    void GetGlobalFactionGvGLog(DT_FACTION_GVG_LOG&, UINT32 dwFactionID);

    // 隐藏掉部分当前不应显示的门派战况
    void HideGvGLog(DT_FACTION_GVG_LOG_LIST &stLogList);

    //获取押注回合
    DT_GVG_ROUND GetGambleRound();

    // 获取指定门派当前在面板上应显示的积分
    UINT32 GetDisplayFactionScore(UINT32 dwFactionID);
    
    // 给予四服总决赛奖励
    void GiveFinalWarAward();

    // 财力大比拼奖励
    void GiveMoneyWarAward();        

    // 向跨服战服务器请求调取本区所需的录像
    void TakeThisZoneVideo();

    // 门派大乱斗奖励
    void GiveChaosWarAward();

    // 双服16进8奖励
    void Give16To8Award();

    // 双服8进4奖励
    void Give8To4Award();

    // 给予奖励（对战奖励、连斩奖励、排名奖励、荣耀奖励等）
    void GiveAward();

    // 根据战斗结果给予胜利、安慰奖励
    void GiveAwardByGvGResult(const DT_GVG_FACTION &stFaction1, const DT_GVG_FACTION &stFaction2, VideoID videoID);

    // 给予指定战斗中连斩奖励
    void GiveKillComboAward(UINT32 dwFactionID, VideoID videoID);

private:
    //给予玩家跨服战奖励
    void GivePlayerAward();
	//添加报名数据
	UINT16 AddApplyFaction(UINT32 dwFactionID, UINT64 qwStartTime = SGDP::SDTimeSecs());
    //添加获胜次数
    void AddWinCnt(UINT32 dwFactionID);
	//重置报名数据
	VOID ResetApplayFaction();
	//添加结果数据
	VOID AddResult(UINT16 wRank, DT_GVG_FACTION_RANK_DATA& stInfo);
	//重置上届结果
	VOID ResetResult();
	//重置财力排行结果
	VOID ResetMoneyRank();
    //重置各门派累积财力
    void ResetFactionFunds();
	//添加财力排行结果
	VOID AddMoneyRank(UINT16 wRank, DT_SIMPLE_FACTION_INFO& stInfo);
	//重置大乱斗结果
	VOID ResetChaosResult();
    //重置大乱斗配对列表
    VOID ResetChaosPair();
	//添加大乱斗结果
	VOID AddChaosResult(UINT16 wRank, DT_SIMPLE_FACTION_INFO& stInfo);
	//获取资金排行
	VOID GetMoneyRank(DT_GVG_RANK_MONEY_LST_DATA& stMoneyData);
	//获取简易资金排行
	VOID GetSimpleMoneyRank(DT_GVG_SIMPLE_RANK_MONEY_LST_DATA& stMoneyData);
	//获取当前进度
	BOOL GetCurSchedule(DT_GVG_SCHEDULE_INFO& stScheduleInfo, CPlayerGVG* poPlayer);
	//获取财力比拼结果
	VOID GetMoneyWarResult(DT_SIMPLE_FACTION_LST_INFO& stLstInfo);
	//获取大乱斗结果
	VOID GetChaosWarResult(DT_SIMPLE_FACTION_LST_INFO& stLstInfo);

	//获取下一轮的倒计时
	UINT32 GetNextStageCountDown();

    // 计算客户端下次应隔多久再重新刷新面板（即重新发送打开协议）
    UINT32 GetClientRefreshTime();

    // 同步各门派新增的支持数
    void SyncFactionSupport();

    // 将数据转储到结构体中
    void SaveLastWinnersToStruct();

    //财力大比拼结束处理方法
    VOID AfterMoneyWar();

private:
    // 当前小轮次
    GVG_STAGE m_eStage;

    // 当前大轮次
    GvGRound m_eRound;

    // 本服的门派跨服战数据，其中的16进8、8进4、总决赛等数据都是由跨服战服务器同步过来的
	DT_GVG_BUILD_DATA m_stGvGBuildData;

	C32Set m_setApplyFaction;//报名的门派列表
	CFactionApplayDataLst m_stFactionApplayDataLst;//报名列表
	CMapFactionLastRank m_mapLastResult;//上届排名（结果不会改变）
	CMapSimpleFaction m_mapMoneyResult;//财力结果（结果不会改变）
	CMapSimpleFaction m_mapChaosResult;//大乱斗结果（结果不会改变）

    GvGFightResultMap m_mapGvGResult; // 存储各门派间的对战结果，以录像ID为主键

    FactionSupportMap m_mapFactionSupport;
    UINT32 m_dwRecentSupportCnt; // 最近的投票总数

    BOOL m_bGiveChaosWarAward; // 是否已给予大乱斗奖励
    BOOL m_bGiveFinalWarAward; // 是否已给予大乱斗奖励
};

#endif //_gvgbuild_h_