///<------------------------------------------------------------------------------
//< @file:   pullgroup.h
//< @author: hongkunan
//< @date:   2014年1月15日 11:56:19
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pullgroup_h_
#define _pullgroup_h_

#include <protogspk.h>

#include "logic/gvg/gvgdef.h"

// 参与跨服战的玩家的实例类
class CPullPlayer
{
public:
    CPullPlayer(ZoneID/* 玩家所在区 */, CPlayer*);

public:
    // 玩家实例
    CPlayer *m_poPlayer;

    // 该玩家所属的区
    ZoneID m_zoneID;
};

// 拉人组，每个组有一个待调取玩家的名单，该组将按照名单拉取玩家
class CPullGroup
{
public:
    CPullGroup()
        : m_groupID(0)
        , m_dwTryCnt(0)
    {

    }
    
    ~CPullGroup()
    {
        UnInit();
    }

    void UnInit();

    // 当所有参战玩家的数据集齐时将触发本方法
    virtual VOID OnPullOut() = 0;

    virtual VOID OnTryTooManyTimes(){}

    virtual EPullGroupType GetGroupType() = 0;

public:
    // 拉取剩余未到达的玩家
    VOID Pull();

    void SendPullReq(ZoneID, CPlayerIDSet&);

    // 成功拉取到某个玩家的数据时执行的操作
    VOID HandleRetPlayerPkt(UINT16 wZoneID, PKT_GSPK_RET_PLAYER_ACK&);

    GroupID GetGroupID(){ return m_groupID; }
    VOID SetGroupID(GroupID groupID){ m_groupID = groupID; }

    CPullPlayerMap& GetPlayers(){ return m_mapPulledPlayer; }

    // 是否已拉取完本组内所有玩家
    BOOL IsPullOut(){ return m_pullingList.empty(); }

private:
    // 该玩家是否在待拉取玩家的名单内
    BOOL InPullingList(PlayerID playerID);

    // 将玩家从待拉取玩家的名单内移除
    VOID EraseFromPullList(PlayerID playerID);

    // 是否已拉取到指定的玩家
    BOOL IsPulled(PlayerID playerID);

protected:
    // 本组的ID
    GroupID m_groupID;

    // 尝试拉取的次数
    UINT32 m_dwTryCnt;

    ZoneToPlayersMap m_mapZone2Players;

    // 剩余的待拉取的玩家id列表
    CPlayerIDSet m_pullingList;

    // 已拉取到的参战玩家列表
    CPullPlayerMap m_mapPulledPlayer;
};

// 拉取区
class CGvGPullGroup : public CPullGroup
{
public:
    CGvGPullGroup(){}

    VOID AddPullPlayerIDList(CPKFaction &oPKFaction);    

private:
    // @impl
    virtual void OnPullOut();

    // @impl
    virtual EPullGroupType GetGroupType(){ return EPullType_GvG; }

    // @override
    virtual void OnTryTooManyTimes();

public:
    // 赛区ID
    ArenaID m_arenaID;
};

#endif //_pullgroup_h_