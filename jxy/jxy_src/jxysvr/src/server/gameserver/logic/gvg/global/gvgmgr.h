///<------------------------------------------------------------------------------
//< @file:   gvgmgr.h
//< @author: hongkunan
//< @date:   2014年1月13日 22:34:41
//< @brief:  门派跨服战管理器
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgmgr_h_
#define _gvgmgr_h_

#include <protocommondata.h>
#include <sdobjectpool.h>
#include <sdsingleton.h>

#include "gvgarena.h"

// 跨服门派对战流程图
/*
 *          跨服战服务器                                             游戏服务器                                         数据库
 *
 *
 * 向各gamesvr发送参赛的门派选手名单                  ---->
 *                                                              
 *                                                    <----     根据门派名单返回各门派阵法配置和上阵玩家名单
 * 收到各门派的布阵数据，为每个门派建立拉人组
 * 这个拉人组将根据上阵玩家名单向gamesvr索要玩家数据  ---->
 *                                                              收到玩家名单，根据玩家名单返回数据
 *                                                              如果找得到玩家快照，则直接将数据返回给跨服战服务器
 *                                                              否则到数据库中取出玩家数据                  ---->
 *                                                                                                          <----   取出玩家数据
 *                                                    <----     得到玩家数据，构造CPlayer返回给跨服战服务器
 * 直到所有门派均已获取到上阵玩家的数据，
 * 各门派间开始对战
 *
 */

class CGvGArena;

// 帮派vs帮派
class CGvGMgr
{
    typedef std::map<ZoneID, ArenaID> ZoneToArenaMap;
    typedef std::map<ZoneID, UINT8> ZoneOpenReplyMap;

public:
    DECLARE_SINGLETON(CGvGMgr)

    CGvGMgr()
        : m_eStage(GVG_0_NOT_START)
    {
        memset(&m_stActData, 0, sizeof(m_stActData));
    }

    BOOL Init();

    void Run();

    // 从数据库中取出跨服战活动数据，如：跨服战结束、活动开始时间等信息
    BOOL LoadGvGActivityDB();

    // 读取跨服门派战各个赛区的数据
    BOOL LoadGvGArenaDB();

    // 从当前全局活动数据中提取出某个赛区的活动数据
    void GlobalGvGToLocalGvG(DT_GVG_LOCAL_ACTIVITY_DATA&, const DT_GVG_ARENA_DATA &stArena);

    UINT16 GetCurSession(){ return m_stActData.wSessionID; }

    // 将当前的跨服战录像同步到各个区
    void SyncVideo(ZoneID);
            
    // 将当前进度存入数据库
    BOOL SaveToDB();

    // 将活动数据推送给指定区
    void PushActivityDataToZone(ZoneID);

    // 初始化定时事件，比如：到第x1天x2秒时开启16进8比赛，到y1天y2秒开启8进4比赛
    BOOL InitTimerEvent();

    // 恢复跨服战的进度
    BOOL RecoverProgress();

public:
    // 跨服战进入下一轮
    BOOL EnterStage(GVG_STAGE eNextGvGRound);
    
    // 跨服战重新进入当前轮次，确保跨服战活动不因意外事件而中断，如：游戏服务器崩溃导致某次通知未被送达、网络阻塞导致丢包等
    BOOL ReEnterCurStage();

    // 跨服战进入下一轮
    BOOL EnterNextStage();
    
    // 更新当前轮次
    void SetStage(GVG_STAGE);

    // 开始本届门派跨服战活动
    BOOL GvGStart();

    // 开始门派跨服战活动数据
    void ClearActivity();

    // 询问各区是否能开启跨服门派战活动
    void AskCanOpen();

    // 进入下一届跨服战的休眠期
    BOOL GvGSleep();

    // 结束本届门派跨服战活动
    void GvGOver();

    // 指定赛区已拉取到全部参赛人员，可以开始战斗
    void OnArenaReadyGo(CGvGPullGroup*);

public:
    // 门派1 vs 门派2，返回战局结果
    static SGvGVideo& GvG(CFaction *g1, CFaction *g2);

    // 门派1 vs 门派2，返回战局结果
    static SGvGVideo& GvG(CPKFaction *g1, CPKFaction *g2);

    // 玩家队伍vs玩家队伍
    static void TeamVsTeam(CPlayerVec&, CPlayerVec&, SGvGVideo&);
    
    // 成功接收到xxx区的游戏服返回的前8强门派名单
    void HandleRetTopFactions(ZoneID, PKT_GSPK_GVG_RET_TOP_8_ACK&);

    // 成功接收到xxx区的游戏服返回的参战门派名单
    void HandleRetFactions(ZoneID, PKT_GSPK_RET_FACTIONS_ACK&);

    // 接收到某个区的就绪回复
    void OnZoneSayReady(ZoneID, PKT_GSPK_GVG_REPLY_READY_ACK&);

    // 接收到某个区的是否可开放回复
    void OnZoneSayCanOpen(ZoneID, PKT_GSPK_GVG_REPLY_OPEN_ACK&);
    
    // 获取当前所处的轮次
    GVG_STAGE GetCurStage(){ return m_eStage; }

    // 本次跨服战的所有运行信息
    DT_GVG_ACTIVITY_DATA& GetActivityData(){ return m_stActData; }

    // 检测本区是否开启门派跨服战
    void CheckOpenUp();

public:
    // 跨服门派战活动是否已完成了指定阶段
    BOOL IsDone(GVG_STAGE);

    // 各个赛区是否均已完成指定阶段的任务
    BOOL IsAllArenaDone(GVG_STAGE);

    // 清除掉门派跨服竞技场
    void ClearArena();

    // 建立空的跨服竞技场
    BOOL BuildEmptyArena();

    // 指定赛区是否开放
    BOOL IsArenaOpenUp(ArenaID);

    // 是否所有的区均返回本区可开启
    BOOL IsAllZoneReplyCanOpen(ArenaID);

    // 建立空的跨服竞技场
    ZoneIDSet GetArenaZoneIDList();

    // 给区号和赛区号建立起映射map
    void BuildZoneIDToArenaIDMap();

    // 根据跨服门派竞技场ID获取相应竞技场实例
    CGvGArena* GetArena(ArenaID);

    // 创建新的门派竞技场实例
    CGvGArena* NewArena(ArenaID);

    // 处理某个区发送过来的支持门派列表
    void SupportFaction(GvGRound, ArenaID arenaID, DT_FACTION_SUPPORT_LIST&);

    // 根据区ID获取赛区ID
    ArenaID GetArenaIDByZoneID(ZoneID);

    // 根据区ID获取赛区
    CGvGArena* GetArenaByZoneID(ZoneID);

private:
    // 当前小阶段
    GVG_STAGE m_eStage;

    // 区号 -> 赛区ID的映射列表
    ZoneToArenaMap m_mapZoneToArena;

    // 存储各区对是否已达到开启跨服门派战活动条件的应答
    ZoneOpenReplyMap m_mapOpenReply;

private:
    // 赛区列表
    CGvGArenaVec m_vecArena;

    // 本次跨服战的所有运行信息（从本数据结构可以还原出跨服战的进程和历史）
	DT_GVG_ACTIVITY_DATA m_stActData;
};

#endif //_gvgmgr_h_