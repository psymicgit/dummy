///<------------------------------------------------------------------------------
//< @file:   pkpktbuilder.cpp
//< @author: hongkunan
//< @date:   2014年1月15日 16:35:59
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "pkpktbuilder.h"

#include <protogspk.h>
#include <protogsdb.h>
#include "framework/gsapi.h"
#include "logic/gvg/global/pullgroup.h"
#include "logic/faction/factionmgr.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/gvg/gvgvideomgr.h"

#include "net/gs/gspktbuilder.h"
#include "logic/player/playersnapshotmgr.h"
#include "logic/player/playersavemgr.h"

VOID CPKPktBuilder::SyncPlayer(UINT32 dwPKGroupID, CPlayer *poPlayer)
{
    if(NULL == poPlayer)
    {
        return;
    }

    ZERO_PACKET(PKT_GSPK_RET_PLAYER_ACK);

    DT_SYNC_PLAYER &stSyncPlayer = pstAck->astPlayerList[0];
    BOOL bSucc = CPKPktBuilder::Instance()->BuildRetPlayer(stSyncPlayer, poPlayer);
    if(FALSE == bSucc)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: sync player[%u] failed, groupID = %u!"), MSG_MARK, poPlayer->GetID(), dwPKGroupID);
        return;
    }

    pstAck->dwPKGroupID = dwPKGroupID;
    pstAck->byRetCnt    = 1;
    gsapi::SendMsg2PK((CHAR*)pstAck, GSPK_RET_PLAYER_ACK);
}

BOOL CPKPktBuilder::BuildRetPlayer(DT_SYNC_PLAYER &stSyncPlayer, CPlayer *poPlayer)
{
    if(!CPlayerSaveMgr::Instance()->SavePlayerNoPvPData(poPlayer, stSyncPlayer.stPlayerNoPvpData))
    {
        return FALSE;
    }

    CPlayerSnapShotMgr::Instance()->CleanNoBattleData(stSyncPlayer.stPlayerNoPvpData);
    return TRUE;
}

UINT16 CPKPktBuilder::NotifyNextRound(GVG_STAGE eNextRound)
{
    ZERO_PACKET(PKT_PKGS_GVG_NEXT_ROUND_NTF);
    pstAck->byRound = eNextRound;

    return PKGS_GVG_NEXT_ROUND_NTF;
}

UINT16 CPKPktBuilder::RetTopFactions()
{
    ZERO_PACKET(PKT_GSPK_GVG_RET_TOP_8_ACK);

    // 获取排名前几的帮派列表
    CFactionVec vecTopFaction;
    if(!CGvGBuild::Instance()->TakeLocalWarWinners(vecTopFaction))
    {
        return INVALID_MSGID;
    }
    
    memset(pstAck->astTop8List, 0, sizeof(pstAck->astTop8List));

    // 把排名前几的帮派的参战数据：阵型、上阵玩家列表，存入消息包
    for(BYTE i = 0; i < vecTopFaction.size() && pstAck->byTop8Cnt < MAX_GVG_TOP_FACTION_CNT; i++)
    {
        // 找到对应帮派，获取阵型和上阵选手数据
        CFaction *poFaction = vecTopFaction[i];
        if(FALSE == poFaction->SaveGvGInfoTo(pstAck->astTop8List[pstAck->byTop8Cnt]))
        {
            continue;
        }

        pstAck->byTop8Cnt++;
    }

    return GSPK_GVG_RET_TOP_8_ACK;
}

UINT16 CPKPktBuilder::RetFactions(UINT32 adwFactionIDList[], BYTE byFactionCnt)
{
    ZERO_PACKET(PKT_GSPK_RET_FACTIONS_ACK);

    DT_PK_FACTION_LIST &stRetFactions = pstAck->stRetFactions;

    // 把各帮派的参战数据（阵型、上阵玩家列表）存入消息包
    for(BYTE i = 0; i < byFactionCnt; i++)
    {
        UINT32 dwFactionID       = adwFactionIDList[i];
        DT_PK_FACTION &stFaction = stRetFactions.astFactionList[stRetFactions.byFactionCnt];

        // 找到对应帮派，获取阵型和上阵选手数据
        CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
        if(NULL == poFaction)
        {
            continue;
        }

        if(FALSE == poFaction->SaveGvGPKInfoTo(stFaction))
        {
            continue;
        }

        stRetFactions.byFactionCnt++;
    }

    return GSPK_RET_FACTIONS_ACK;
}

UINT16 CPKPktBuilder::GetPvPVideoReq()
{
    ZERO_PACKET(PKT_GSPK_GET_VIDEO_REQ);
    return GSPK_GET_VIDEO_REQ;
}

UINT16 CPKPktBuilder::RetPvPVideo(VideoID aVideoIDList[], UINT16 wVideoCnt)
{
    ZERO_PACKET(PKT_PKGS_RET_VIDEO_ACK);
        
    pstAck->wPvPVideoCnt = 0;

    for(UINT16 i = 0; i < wVideoCnt && i < GVG_PK_VIDEO_RET_CNT_ONCE; i++)
    {
        VideoID videoID = aVideoIDList[i];
        DT_GVG_PK_VIDEO &stVideo = pstAck->astPvPVideoList[pstAck->wPvPVideoCnt];

        SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(videoID);
        if(NULL == pstVideo)
        {
            continue;
        }

        pstAck->wPvPVideoCnt++;
    }

    return PKGS_RET_VIDEO_ACK;
}

void CPKPktBuilder::GetGvGActivityReq()
{
    ZERO_PACKET(PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ);
    pstAck->byNothing = 0;

    gsapi::SendMsg2PK((CHAR*)pstAck, GSPK_GET_GVG_ACTIVITY_DATA_REQ);
}