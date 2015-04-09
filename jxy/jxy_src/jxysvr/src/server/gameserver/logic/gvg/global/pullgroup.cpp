///<------------------------------------------------------------------------------
//< @file:   pullgroup.cpp
//< @author: hongkunan
//< @date:   2014年1月15日 13:59:14
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/gvg/global/pullgroup.h"

#include <protogspk.h>
#include <sdloggerimpl.h>

#include "framework/gsapi.h"
#include "logic/gvg/global/gvgmgr.h"
#include "pullplayermgr.h"
#include "net/db/dbprocessor.h"

using namespace SGDP;

namespace innerutil
{
    // 根据网络数据创建玩家实例
    CPlayer* BuildPlayerFromPkt(DT_PLAYER_DATA &stDT_PLAYER_DATA)
    {
        CPlayer* poPlayer = CPlayerPoolMgr::Instance()->Create();
        if (NULL == poPlayer)
        {
            SYS_CRITICAL(_SDT("[%s: %d] CPlayerPoolMgr create player Failed, player[%u : %s]!"), 
                MSG_MARK, stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID, stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.aszDispName);

            return NULL;
        }

        //设置玩家对象
        if(!poPlayer->Init(stDT_PLAYER_DATA))
        {
            SYS_CRITICAL(_SDT("[%s: %d] player[%u : %s] Init failed!"), 
                MSG_MARK, stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID, stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.aszDispName);

            CPlayerPoolMgr::Instance()->Remove(poPlayer);
            return NULL;
        }

        return poPlayer;
    }
}

CPullPlayer::CPullPlayer(ZoneID zoneID/* 玩家所在区 */, CPlayer *poPlayer)
    : m_zoneID(zoneID)
    , m_poPlayer(poPlayer)
{

}

// 成功拉取到某个玩家的数据时执行的操作
VOID CPullGroup::HandleRetPlayerPkt(UINT16 wZoneID, PKT_GSPK_RET_PLAYER_ACK &oSyncAck)
{
    // 根据组id检测消息包是否被派发到正确的组
    if(oSyncAck.dwPKGroupID != m_groupID)
    {
        // 说明消息包派发有误
        SYS_CRITICAL(_SDT("[%s: %d]: detecting sync pkt<groupid = %u> was assigned to wrong PKGroup<groupid = %u>!"), MSG_MARK, oSyncAck.dwPKGroupID, m_groupID);
        return;
    }

    if(0 == oSyncAck.byRetCnt)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: detecting sync pkt<groupid = %u> contains 0 players!"), MSG_MARK, oSyncAck.dwPKGroupID);
        return;
    }

    DT_PLAYER_DATA stPlayerData;

    // 从消息包中创建CPlayer
    for(BYTE i = 0; i < oSyncAck.byRetCnt; ++i)
    {
        DT_SYNC_PLAYER &stSyncPlayer = oSyncAck.astPlayerList[i];

        UINT32 dwPlayerID = stSyncPlayer.stPlayerNoPvpData.stBaseData.dwID;

        // 检测该玩家是否在待拉取玩家的名单内
        if(FALSE == InPullingList(dwPlayerID))
        {
            if(FALSE == IsPulled(dwPlayerID))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: detecting group[groupid = %u] pulled wrong player[player id = %u], discard it!"), MSG_MARK, oSyncAck.dwPKGroupID, m_groupID);
            }

            continue;
        }
        
        memset(&stPlayerData.stPlayerPvpData, 0, sizeof(stPlayerData.stPlayerPvpData));
        stPlayerData.stPlayerNoPvpData = stSyncPlayer.stPlayerNoPvpData;

        CPlayer *poPlayer = innerutil::BuildPlayerFromPkt(stPlayerData);
        if(NULL == poPlayer)
        {
            continue;
        }

        CPullPlayer *poPullPlayer = new CPullPlayer(wZoneID, poPlayer);
        m_mapPulledPlayer[poPlayer->GetID()] = poPullPlayer;

        // 将玩家从待拉取玩家的名单内移除
        EraseFromPullList(poPlayer->GetID());
    }
}

// 该玩家是否在待拉取玩家的名单内
BOOL CPullGroup::InPullingList(PlayerID playerID)
{
    return m_pullingList.find(playerID) != m_pullingList.end();
}

BOOL CPullGroup::IsPulled(PlayerID playerID)
{
    return m_mapPulledPlayer.find(playerID) != m_mapPulledPlayer.end();
}

// 将玩家从待拉取玩家的名单内移除
VOID CPullGroup::EraseFromPullList(PlayerID playerID)
{
    m_pullingList.erase(playerID);
}

void CPullGroup::UnInit()
{
    for(CPullPlayerMap::iterator itr = m_mapPulledPlayer.begin(); itr != m_mapPulledPlayer.end(); ++itr)
    {
        CPullPlayer *poPullPlayer = itr->second;
        if(NULL == poPullPlayer)
        {
            continue;
        }

        CPlayerPoolMgr::Instance()->Remove(poPullPlayer->m_poPlayer);
        delete poPullPlayer;
    }

    m_mapPulledPlayer.clear();
}

VOID CPullGroup::Pull()
{
    if(IsPullOut())
    {
        return;
    }

    ++m_dwTryCnt;
    OnTryTooManyTimes();

    // 一一向对应的各个区发送请求列表
    for(ZoneToPlayersMap::iterator itr = m_mapZone2Players.begin(); itr != m_mapZone2Players.end(); ++itr)
    {
        ZoneID zoneID = itr->first;
        CPlayerIDSet &pullList = itr->second;

        SendPullReq(zoneID, pullList);
    }
}

void CPullGroup::SendPullReq(ZoneID zoneID, CPlayerIDSet &pullList)
{
    PKT_PKGS_GET_PLAYER_REQ req;
    req.dwPKGroupID = m_groupID;
    req.byGetCnt    = 0;

    // 根据名单到指定区拉取玩家
    for (CPlayerIDSet::iterator itr = pullList.begin(); itr != pullList.end(); ++itr)
    {
        PlayerID playerID = *itr;
        req.adwPlayerIDList[req.byGetCnt++] = playerID;

        // 如果请求的量过多，则拆分成几个消息包依次发送
        if(MAX_REQ_SYNC_PLAYER_CNT_ONCE == req.byGetCnt)
        {
            // 向该区请求调取玩家
            gsapi::SendMsg2GS(zoneID, (CHAR*)&req, PKGS_GET_PLAYER_REQ);

            memset(&req, 0, sizeof(req));
            req.dwPKGroupID = m_groupID;
        }
    }

    /*
    // 暂定: 只拉取一个
    req.byGetCnt = 1;
    */

    // 拉取剩余的玩家
    if(req.byGetCnt > 0)
    {
        gsapi::SendMsg2GS(zoneID, (CHAR*)&req, PKGS_GET_PLAYER_REQ);
    }
}

VOID CGvGPullGroup::AddPullPlayerIDList(CPKFaction &oPKFaction)
{
    DT_GVG_FORMATION_DATA stMatrix = oPKFaction.GetData().stFormation;
    ZoneID zoneID = oPKFaction.GetZoneID();

    for(UINT16 i = 0; i < stMatrix.byPlayerNum; i++)
    {
        UINT32 dwPlayerID = stMatrix.adwPlayerIDList[i];
        if(0 == dwPlayerID)
        {
            continue;
        }

        m_pullingList.insert(dwPlayerID);

        // 将待拉取玩家按区进行分类
        m_mapZone2Players[zoneID].insert(dwPlayerID);
    }

    m_pullingList.erase(0);
}

VOID CGvGPullGroup::OnPullOut()
{
    CGvGMgr::Instance()->OnArenaReadyGo(this);
}

void CGvGPullGroup::OnTryTooManyTimes()
{
    if(0 == m_dwTryCnt % 10)
    {
        SYS_CRITICAL(_SDT("[%s: %d] CGvGPullGroup::OnTryTooManyTimes pullgroup of arena[%u] try too many times[%u]!"), MSG_MARK, m_arenaID, m_dwTryCnt);
    }
}