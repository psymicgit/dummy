///<------------------------------------------------------------------------------
//< @file:   pullplayermgr.h
//< @author: hongkunan
//< @date:   2014年1月15日 11:51:47
//< @brief:  调取玩家中心，位于跨服战服务器下
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pullplayermgr_h_
#define _pullplayermgr_h_

#include <protogspk.h>
#include <sdsingleton.h>
#include <sdobjectpool.h>

#include "logic/gvg/global/pullgroup.h"

// 调取玩家中心，，位于跨服战服务器下，专门负责向游戏服务器调取玩家数据，管理各个调取玩家的组
class CPullPlayerMgr
{
    DECLARE_SINGLETON(CPullPlayerMgr)

public:
    CPullPlayerMgr()
        : m_dwAllocGroupID(0)
        , m_oGvGGroupPool(5)
    {
    }

    BOOL Init(){ return TRUE; }

    // 成功接收到玩家数据
    VOID HandleRetPlayer(UINT16 zoneID, PKT_GSPK_RET_PLAYER_ACK&);

    CPullGroup* FindPullGroup(GroupID);

    // 新建拉人组
    CPullGroup* NewGroup(EPullGroupType ePKType);

    VOID EraseGroup(GroupID);

    void Pull();

    void ClearDeadGroup();

private:
    GroupID AllocGroupID(){ return ++m_dwAllocGroupID; }
    
    CPullGroupMap m_mapPullGroup; // 所有跨服战组 = PvP + PvE + GvG + GvE

    CSDObjectPool<CGvGPullGroup> m_oGvGGroupPool; // 门派vs门派组池

private:
    GroupID m_dwAllocGroupID;
};

#endif //_pullplayermgr_h_