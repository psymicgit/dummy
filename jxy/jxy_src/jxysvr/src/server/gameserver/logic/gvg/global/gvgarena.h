///<------------------------------------------------------------------------------
//< @file:   gvg\global\gvgarena.h
//< @author: hongkunan
//< @date:   2014年5月5日 15:23:16
//< @brief:  门派跨服战赛场
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgarena_h_
#define _gvgarena_h_

#include <protocommondata.h>

#include "logic/gvg/gvgdef.h"
#include "logic/base/basedefine.h"
#include "gvgmember.h"

struct SGvGVideo;
class CPKZone;

// 门派跨服战的赛区，一个赛区由多个区组成
class CGvGArena
{
    typedef std::map<UINT32, DT_FACTION_SIMPLE_GVG_LOG_LIST> FactionSimpleLogMap;  // 各门派的简易门派战况列表，key = 门派ID， value = 该门派的战况历史
    typedef std::map<VideoID, SGvGVideo*> GvGVideoPointerMap; // 门派间对战录像

public:
    CGvGArena()
        : m_dwPullGroupID(0)
        , m_eStage(GVG_0_NOT_START)
        , m_eRound(GVG_SLEEPING)
    {
        memset(&m_stArena, 0, sizeof(m_stArena));
    }

public:
    // 根据传入的赛区数据还原出本赛区
    BOOL Init(const DT_GVG_ARENA_DATA&);

    // 获取本赛区的数据
    DT_GVG_ARENA_DATA& GetArenaData(){ return m_stArena; }

    // 获取本赛区的赛区ID
    ArenaID GetArenaID(){ return m_stArena.wArenaID; }

    // 更新指定轮次的门派信息
    void UpdateFactionsByRound(GvGRound);

    // 添加16进8的门派选手列表
    void Add16To8Members(ZoneID, DT_GVG_FACTION astTop8List[], UINT8 byTopCnt);

    // 初始化总决赛的入围选手
    BOOL InitFinalWarMembers();

    // 生成双服16进8的对决名单
    BOOL RollPair16To8();

    // 门派跨服战的指定阶段是否已完成
    BOOL IsDone(GVG_STAGE);

    // 根据当前门派跨服战的状况数据判断指定阶段是否已完成
    BOOL IsLogicDone(GVG_STAGE);

    // 检测各区是否均已返回就绪应答，如果没有，则重新询问一次
    void CheckEachZoneReady();

    // 跨服战服务器与本赛区的各成员区是否都正常连接
    BOOL IsAllZoneConnecting();

    // 开始进行战斗（此接口要求所有门派、所有玩家数据已到位）
    BOOL Go();

    // 获取指定轮次各区的参赛门派列表
    EachZoneFactionIDListMap SplitFactionsByZone(GvGRound);

    // 是否已接收到本赛区的全部成员区的门派数据
    BOOL IsAllZoneArrived(GvGRound);

    // 是否已接收到指定区的门派数据
    BOOL IsZoneArrived(ZoneID);

    // 本区是否为空
    BOOL IsEmpty();

    // 本赛区的成员区本轮是否均已准备就绪
    BOOL IsAllZoneReady();

    // 清空各个成员区的就绪状态
    void ClearReadyZoneList(){ m_setReadyZone.clear(); }

    // 接收到某个区返回的该区门派数据
    void OnRecvFaction(ZoneID, DT_PK_FACTION_LIST&);

    // 接收到某个区返回的该区前几强门派列表
    void OnRecvTopFaction(ZoneID zoneID, DT_GVG_FACTION astTopList[], UINT8 byTopCnt);

    // 将指定区标记为已就绪
    void SetZoneArrived(ZoneID);

    // 开始本赛区门派跨服战活动
    BOOL GvGStart();

    // 结束本赛区门派跨服战活动
    BOOL GvGSleep();

    // 结束本赛区门派跨服战活动
    BOOL GvGOver();

    // 询问本赛区的各个成员区：当前阶段是否已就绪
    void AskReady();

    // 成员区返回应答：当前阶段已就绪/未就绪
    void OnZoneSayReady(ZoneID, PKT_GSPK_GVG_REPLY_READY_ACK&);

    // 获取各服的前8强
    void GetEachZoneTop8();

    // 结束门派大乱斗，即将进入16进8阶段（此时各区应已决出前8）
    void ChaosWarEnd();

    // 进入16进8轮次：为门派选手进行配对，并添加未开始战况
    BOOL War16To8Begin();

    // 进入8进4轮次
    BOOL War8To4Begin();

    // 进入总决赛
    BOOL FinalWarBegin();

    // 生成总决赛未开始战报
    void AddFinalWarNotStartLog(UINT8 byRound);

    // 16进8轮次：决出双服前8强
    BOOL War16To8Go();

    // 8进4轮次：决出双服前4强
    BOOL War8To4Go();

    // 打印16进8的胜出者
    void Print16To8Winner();

    // 打印8进4的胜出者
    void Print8To4Winner();
    
    // 打印最终决赛的胜出者
    void PrintFinalWinner();

    // 总决赛：准备开始对战
    BOOL PrepareFinalWarGo();

    // 总决赛第一轮结果是否已决出
    BOOL IsFinalWarRoundComplete(UINT8 byRound);

    // 总决赛是否应继续进展下一轮
    BOOL IsShouldFinalWarMoveOn();

    // 根据当前时间获取总决赛当前应处的轮次，最大轮次 = 门派数
    UINT8 GetExpectFinalWarRound();

    // 总决赛：开始对战
    BOOL FinalWarGo();

    // 开始指定小轮次的战斗
    BOOL PrepareWarGo(GVG_STAGE);

    // 结束指定小轮次的战斗
    BOOL WarEnd(GVG_STAGE);

    // 开始指定小轮次的战斗
    BOOL War16To8End();

    // 清空各区门派数据就绪列表
    void ClearArrivedZone(){ m_setArrivedZone.clear(); }

    // 重置本赛区数据，全部清空
    void Clear();

    // 执行各轮次的比赛开始前的清场工作
    void ClearWar();

    // 接收门派支持列表
    void SupportFaction(GvGRound, DT_FACTION_SUPPORT_LIST &stFactionList);

    // 同步给各成员区当前各门派的支持数信息
    void SyncFactionSupportCnt(FactionSupportMap &mapNowSupport);

    // 初始化本赛区的成员区列表
    void InitZoneList(GroupZoneIDMap&);

    // 将本赛区的数据存入数据库
    BOOL SaveToDB();

    // 寻找指定的决战区
    CPKZone* FindPKZone(ZoneID zoneID);

    // 寻找指定区的指定决战门派
    CPKFaction* FindPKFaction(ZoneID zoneID, UINT32 dwFactionID);

    // 刷新跨服门派竞技场的参赛选手数据
    void AddPlayers(CGvGPullGroup&);

    // 为各成员区建立一个区实例
    void BuildZone();

    // 清空所缓存的各区数据
    void ClearZones();

    // 为指定的区创建新的区实例
    CPKZone* NewPKZone(ZoneID);

    // 开始到各区拉取参赛门派选手数据
    void StartPullPlayers();

    // 同步本赛区录像到指定成员区(区为0则代表同步到各个成员区)
    void SyncVideo(ZoneID targetZoneID = 0);

    // 将当前的跨服战进度同步到指定成员区(区为0则代表同步到各个成员区)
    void SyncActivity(ZoneID targetZoneID = 0);

    // 取出本赛区的录像列表
    void TakeArenaVideo(GvGVideoPointerMap&);

    // 获取本赛区的区成员列表
    ZoneIDSet GetThisArenaZoneIDSet();

    // 将指定跨服战录像拆分成多个消息包发送到指定区
    void SendPvPVideo(ZoneID zoneID, SGvGVideo &stVideo, PKT_PKGS_RET_VIDEO_ACK &stAck);

    // 添加未开始战况
    void AddNotStartLog(UINT32 dwFactionID1, UINT32 dwFactionID2, VideoID videoID, GvGRound eBigRound, UINT8 bySmallRound = 0);
    
    // 添加已结束战报
    void AddLog(UINT32 dwFactionID, const DT_GVG_VIDEO &stVideo, UINT16 wFactionAddScore);

    // 取出数据到结构体
    void TakeDataToStruct();

    // 判断指定阶段是否已被标识为已完成
    BOOL IsStageDone(GVG_STAGE);

    // 设置指定阶段的标志为：已完成
    void SetStageDone(GVG_STAGE);

    // 重置各阶段的标志位
    void ResetStageFlag();

    // 获取16进8的门派选手列表
    FactionIDSet Get16To8Seed();

    // 跨服战进入下一轮
    BOOL EnterStage(GVG_STAGE eNextStage);

    // 更新当前轮次
    void UpdateRound();

    // 更新当前轮次
    void SetStage(GVG_STAGE);

    // 向指定的几个区列表请求调取门派数据
    void SendPullFactionReq(EachZoneFactionIDListMap &mapZones);

    // 通知各游戏服跨服战进入下一阶段
    void NotifyStage(GVG_STAGE);

private:
    // 查找指定区在列表中的位置，找得到返回1~4，找不到返回0
    BYTE GetGroupIDByZone(ZoneID);
    
    // 更新各区的最新数据
    void UpdateFactions(DT_GVG_FACTION[], UINT16 dwFactionCnt);

public:
    // 本轮次中已返回门派数据的区列表（每当跨服门派战活动进入下一轮次时，这个列表会被重新清空）
    ZoneIDSet m_setArrivedZone;

    // 本轮次中本赛区中已就绪的区列表（每当跨服门派战活动进入下一轮次时，这个列表会被重新清空）
    ZoneIDSet m_setReadyZone;

    // 隶属于本赛场的拉人组ID
    UINT32 m_dwPullGroupID;

    // 存储各门派的简易门派战况
    FactionSimpleLogMap m_mapFactionLog;

protected:
    // 隶属于本赛区的各区数据
    CPKZoneMap m_mapPKZone;

    // 本赛场的数据
    DT_GVG_ARENA_DATA m_stArena;

    // 当前小阶段
    GVG_STAGE m_eStage;

    // 当前轮次
    GvGRound m_eRound;
};

#endif //_gvgarena_h_