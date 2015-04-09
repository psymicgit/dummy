///<------------------------------------------------------------------------------
//< @file:   logic\gvg\gvgmember.cpp
//< @author: hongkunan
//< @date:   2014年2月18日 17:29:20
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "gvgmember.h"

#include <protogspk.h>
#include <sdloggerimpl.h>

// 根据返回的消息包，初始化门派的阵型和参战玩家数据
BOOL CPKZone::Init(DT_PK_FACTION_LIST &stFactionList)
{
    if(0 == stFactionList.byFactionCnt)
    {
        return FALSE;
    }

    for(int i = 0; i < stFactionList.byFactionCnt; i++)
    {
        DT_PK_FACTION &stFaction = stFactionList.astFactionList[i];

        CPKFaction *poPKFaction = FindPKFaction(stFaction.stBaseData.stBaseData.dwFactionID);
        if(NULL == poPKFaction)
        {
            poPKFaction = new CPKFaction; // 这里不用对象池
            if(NULL == poPKFaction)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CPKZone::Init failed, alloc faction[%u] not enough memory, zone = %u!"), MSG_MARK, stFaction.stBaseData.stBaseData.dwFactionID, m_zoneID);
                return FALSE;
            }
        }

        if(FALSE == poPKFaction->Init(stFaction))
        {
            delete poPKFaction;
            return FALSE;
        }

        if(poPKFaction->IsMatrixEmpty())
        {
            DBG_INFO(_SDT("[%s: %d]: CPKZone::Init formation of faction [%u] is empty, zone = %u!"), MSG_MARK, poPKFaction->GetFactionID(), m_zoneID);
        }

        m_mapPKFaction[poPKFaction->GetFactionID()] = poPKFaction;
    }

    return TRUE;
}

void CPKZone::Clear()
{
    for(CPKFactionMap::iterator itr = m_mapPKFaction.begin(); itr != m_mapPKFaction.end(); ++itr)
    {
        CPKFaction *poPKFaction = itr->second;
        delete poPKFaction;
    }

    m_mapPKFaction.clear();
}

CPKFaction* CPKZone::FindPKFaction(UINT32 dwFactionID)
{
    if(0 == dwFactionID)
    {
        return NULL;
    }

    CPKFactionMap::iterator itr = m_mapPKFaction.find(dwFactionID);
    if(itr == m_mapPKFaction.end())
    {
        return NULL;
    }

    CPKFaction *poPKFaction = itr->second;
    return poPKFaction;
}

void CPKZone::AddPKFaction(CPKFaction *poPKFaction)
{
    if(FindPKFaction(poPKFaction->GetFactionID()))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPKZone::AddPKFaction to zone<%u> failed, faction<id=%u> existed!"), MSG_MARK, m_zoneID, poPKFaction->GetFactionID());
        return;
    }

    m_mapPKFaction[poPKFaction->GetFactionID()] = poPKFaction;
}

BOOL CPKFaction::Init(DT_PK_FACTION &stFaction)
{
    m_stFaction = stFaction;
    return TRUE;
}

void CPKFaction::SaveTo(DT_PK_FACTION &stFaction)
{
    stFaction = m_stFaction;
}

void CPKFaction::Clear()
{
    m_mapPlayers.clear();
    memset(&m_stFaction,  0, sizeof(m_stFaction));
}

void CPKFaction::ResetMembers(CPullPlayerMap &mapPullPlayers)
{
    m_mapPlayers = mapPullPlayers;
}

CPlayer* CPKFaction::FindPlayer(UINT32 dwPlayerID)
{
    if(0 == dwPlayerID)
    {
        return NULL;
    }

    CPullPlayerMap::iterator itr = m_mapPlayers.find(dwPlayerID);
    if(itr == m_mapPlayers.end())
    {
        return NULL;
    }

    CPullPlayer *poPullPlayer = itr->second;
    return poPullPlayer->m_poPlayer;
}

CPlayerVec CPKFaction::GetFormationPlayers()
{
    CPlayerVec vecPlayer;
    for(UINT16 i = 0; i < m_stFaction.stPlayerList.byPlayerCnt; i++)
    {
        DT_GVG_FORMATION_PLAYER_INFO &stPlayer = m_stFaction.stPlayerList.astPlayerList[i];

        CPlayer *poPlayer1 = FindPlayer(stPlayer.dwPlayerID);
        if(NULL == poPlayer1)
        {
            SYS_CRITICAL( _SDT("[%s: %d] CPKFaction::GetFormationPlayerVec could not find player<%u>!"), MSG_MARK, stPlayer.dwPlayerID);
            continue;
        }

        vecPlayer.push_back(poPlayer1);
    }

    return vecPlayer;
}