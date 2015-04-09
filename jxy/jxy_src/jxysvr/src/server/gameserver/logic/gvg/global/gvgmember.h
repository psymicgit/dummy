///<------------------------------------------------------------------------------
//< @file:   logic\gvg\gvgmember.h
//< @author: hongkunan
//< @date:   2014年2月18日 17:29:11
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgmember_h_
#define _gvgmember_h_

#include <protocommondata.h>
#include <sdobjectpool.h>

#include "logic/gvg/global/pullgroup.h"
#include "logic/gvg/gvgdef.h"

// 参战帮派
class CPKFaction
{
public:
    CPKFaction()
    {
        Clear();
    }

public:
    BOOL Init(DT_PK_FACTION&);
    void SaveTo(DT_PK_FACTION&);

    void Clear();

    DT_PK_FACTION& GetData(){ return m_stFaction; }

    UINT16 GetZoneID(){ return m_stFaction.stBaseData.wZoneID; }
    UINT32 GetFactionID(){ return m_stFaction.stBaseData.stBaseData.dwFactionID; }

    // 调取的玩家数据成功到达
    void ResetMembers(CPullPlayerMap&);

    CPlayer* FindPlayer(UINT32 dwPlayerID);

    BOOL IsMatrixEmpty(){ return 0 == m_stFaction.stFormation.byPlayerNum; }

    CPlayerVec GetFormationPlayers();

public:

    // 门派的基础信息
    DT_PK_FACTION m_stFaction;
    CPullPlayerMap m_mapPlayers; // 本派参战玩家的数据列表
};

// 参战区
class CPKZone
{
    typedef std::map<UINT32, CPKFaction*> CPKFactionMap; // 门派实例map：键 = 门派ID， 值 = 门派实例

public:
    CPKZone()
        : m_zoneID(0)
    {
    }

    // 初始化门派的阵型和参战玩家数据
    BOOL Init(DT_PK_FACTION_LIST&);

    void Clear();

    CPKFaction* FindPKFaction(UINT32 dwFactionID);

    BOOL IsEmpty()
    {
        return m_mapPKFaction.empty();
    }

    void AddPKFaction(CPKFaction *poPKFaction);

public:
    ZoneID m_zoneID;
    CPKFactionMap m_mapPKFaction; // 本区内的参战帮派列表
};

#endif //_gvgmember_h_