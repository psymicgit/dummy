///<------------------------------------------------------------------------------
//< @file:   pullplayermgr.cpp
//< @author: hongkunan
//< @date:   2014年1月15日 11:52:6
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "pullplayermgr.h"

#include <protogspk.h>

IMPLEMENT_SINGLETON(CPullPlayerMgr)

CPullGroup* CPullPlayerMgr::NewGroup(EPullGroupType ePKType)
{
    CPullGroup *poNewPullGroup = NULL;
    switch(ePKType)
    {
    case EPullType_GvG:
        poNewPullGroup = m_oGvGGroupPool.Alloc();
        break;
    }

    if(NULL == poNewPullGroup)
    {
        return NULL;
    }

    GroupID allocGroupID = AllocGroupID();
    poNewPullGroup->SetGroupID(allocGroupID);

    m_mapPullGroup[allocGroupID] = poNewPullGroup;
    return poNewPullGroup;
}

VOID CPullPlayerMgr::EraseGroup(GroupID groupID)
{
    CPullGroupMap::iterator itr = m_mapPullGroup.find(groupID);
    if(itr == m_mapPullGroup.end())
    {
        return;
    }    

    CPullGroup *poPullGroup = itr->second;

    switch(poPullGroup->GetGroupType())
    {
    case EPullType_GvG:
        m_oGvGGroupPool.Free((CGvGPullGroup*)poPullGroup);
        break;
    }

    m_mapPullGroup.erase(itr);
}

VOID CPullPlayerMgr::HandleRetPlayer(UINT16 wZoneID, PKT_GSPK_RET_PLAYER_ACK &ack)
{
    CPullGroup *poPullGroup = FindPullGroup(ack.dwPKGroupID);
    if(NULL == poPullGroup)
    {
        return;
    }

    poPullGroup->HandleRetPlayerPkt(wZoneID, ack);
    if(poPullGroup->IsPullOut())
    {
        poPullGroup->OnPullOut();
        EraseGroup(poPullGroup->GetGroupID());
    }
}

CPullGroup* CPullPlayerMgr::FindPullGroup(GroupID groupID)
{
    CPullGroupMap::iterator itr = m_mapPullGroup.find(groupID);
    if(itr == m_mapPullGroup.end())
    {
        return NULL;
    }

    CPullGroup *poGroup = itr->second;
    return poGroup;
}

void CPullPlayerMgr::ClearDeadGroup()
{
    CPullGroupMap mapCopyGroups = m_mapPullGroup; // 这里进行硬拷贝

    for(CPullGroupMap::iterator itr = mapCopyGroups.begin(); itr != mapCopyGroups.end(); ++itr)
    {
        CPullGroup *poPullGroup = itr->second;
        if(poPullGroup->IsPullOut())
        {
            poPullGroup->OnPullOut();
            EraseGroup(poPullGroup->GetGroupID());
        }
    }
}

VOID CPullPlayerMgr::Pull()
{
    if(m_mapPullGroup.empty())
    {
        return;
    }

    ClearDeadGroup();

    UINT16 wPullCnt = 0;
    for(CPullGroupMap::iterator itr = m_mapPullGroup.begin(); itr != m_mapPullGroup.end() && wPullCnt < 2; ++itr, ++wPullCnt)
    {
        CPullGroup *poPullGroup = itr->second;
        poPullGroup->Pull();
    }
}