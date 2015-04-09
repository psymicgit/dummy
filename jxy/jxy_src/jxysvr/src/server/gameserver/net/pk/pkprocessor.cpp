///<------------------------------------------------------------------------------
//< @file:   pkprocessor.cpp
//< @author: hongkunan
//< @date:   2014年1月3日 13:54:23
//< @brief:
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "pkprocessor.h"

#include <protogspk.h>
#include "framework/gsapi.h"
#include "net/gs/gspktbuilder.h"

#include "net/pk/pkpktbuilder.h"
#include "logic/gvg/global/pullgroup.h"
#include "logic/faction/factionmgr.h"
#include "logic/gvg/global/pullplayermgr.h"
#include "logic/gvg/global/gvgmgr.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/player/playersnapshotmgr.h"
#include "logic/gvg/gvgvideomgr.h"

namespace innerutil
{
}

CPKProcessor::CPKProcessor()
{
    Init();
}

CPKProcessor::~CPKProcessor()
{
}

CSDProtocol* CPKProcessor::GetProtocol()
{
    return CProtoGSPK::Instance();
}

BOOL CPKProcessor::Init()
{
    RegisterCommand(PKGS_GVG_GET_TOP_8_REQ,         CPKProcessor::OnGetTopFactions);
    RegisterCommand(GSPK_GVG_RET_TOP_8_ACK,         CPKProcessor::OnRetTopFactions);
                                                    
    RegisterCommand(PKGS_GET_FACTIONS_REQ,          CPKProcessor::OnGetFactions);
    RegisterCommand(GSPK_RET_FACTIONS_ACK,          CPKProcessor::OnRetFactions);
                                                    
    RegisterCommand(PKGS_GET_PLAYER_REQ,            CPKProcessor::OnGetPlayerReq);
    RegisterCommand(GSPK_RET_PLAYER_ACK,            CPKProcessor::OnRetPlayerAck);
                                                    
    RegisterCommand(PKGS_GVG_NEXT_ROUND_NTF,        CPKProcessor::OnGvGNextRoundNotify);
    RegisterCommand(PKGS_SYNC_GVG_NTF,              CPKProcessor::OnSyncGvG);
    RegisterCommand(GSPK_GET_VIDEO_REQ,             CPKProcessor::OnGetVideo);
    RegisterCommand(PKGS_RET_VIDEO_ACK,             CPKProcessor::OnRetVideo);
    RegisterCommand(PKGS_SYNC_VIDEO_NTF,            CPKProcessor::OnSyncVideo);
                                                    
    RegisterCommand(PKGS_GVG_ASK_READY_REQ,         CPKProcessor::OnAskGvGReady);
    RegisterCommand(GSPK_GVG_REPLY_READY_ACK,       CPKProcessor::OnRecvGvGReadyReply);
    RegisterCommand(GSPK_SUPPORT_FACTION_REQ,       CPKProcessor::OnSupportFaction);
    RegisterCommand(PKGS_SYNC_FACTION_SUPPORT_NTF,  CPKProcessor::OnRecvFactionSupportList);
    RegisterCommand(GSPK_GET_GVG_ACTIVITY_DATA_REQ, CPKProcessor::OnGetGvGActivityDataReq);

    return TRUE;
}

BOOL CPKProcessor::OnGvGNextRoundNotify(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;

    PKT_PKGS_GVG_NEXT_ROUND_NTF *pNtf = (PKT_PKGS_GVG_NEXT_ROUND_NTF*)pBody;
    if(pNtf->byRound >= GVG_STAGE_MAX)
    {
        return FALSE;
    }

    CGvGBuild::Instance()->OnEnterStageNotify((GVG_STAGE)pNtf->byRound);
    return TRUE;
}

BOOL CPKProcessor::OnGetTopFactions(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;

    PKT_PKGS_GVG_GET_TOP_8_REQ *pReq = (PKT_PKGS_GVG_GET_TOP_8_REQ*)pBody;
    if(pReq->byExt != 0)
    {
        return FALSE;
    }

    UINT16 wMsgID = CPKPktBuilder::Instance()->RetTopFactions();
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }    

    PKT_GSPK_GVG_RET_TOP_8_ACK *pAck = (PKT_GSPK_GVG_RET_TOP_8_ACK*)CPKPktBuilder::Instance()->GetPacketBuffer();
    gsapi::SendMsg2PK((CHAR*)pAck, wMsgID);

    return TRUE;
}

BOOL CPKProcessor::OnRetTopFactions(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwZoneID              = pPipeHeader->dwTransID;

    PKT_GSPK_GVG_RET_TOP_8_ACK *pAck = (PKT_GSPK_GVG_RET_TOP_8_ACK*)pBody;
    CGvGMgr::Instance()->HandleRetTopFactions(dwZoneID, *pAck);

    return TRUE;
}

BOOL CPKProcessor::OnRetFactions(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwZoneID              = pPipeHeader->dwTransID;

    PKT_GSPK_RET_FACTIONS_ACK *pAck = (PKT_GSPK_RET_FACTIONS_ACK*)pBody;
    CGvGMgr::Instance()->HandleRetFactions(dwZoneID, *pAck);

    // printf(_SDT("[%s: %d]:OnGetRetTopFactions\n"), MSG_MARK);
    // USR_INFO(_SDT("[%s: %d]: OnGetRetTopFactions\n"), MSG_MARK);

    return TRUE;
}

BOOL CPKProcessor::TakePlayer(UINT32 dwPlayerID, DT_SYNC_PLAYER &stSyncPlayer, UINT32 dwPKGroupID)
{
    DT_PLAYER_NOPVP_DATA *pstPlayerNoPvP = CPlayerSnapShotMgr::Instance()->FindPlayerData(dwPlayerID);
    if(pstPlayerNoPvP)
    {
        // 在玩家快照表中
        stSyncPlayer.stPlayerNoPvpData = *pstPlayerNoPvP;
    }
    else
    {
        // 检测玩家数据是否在内存中，是：直接取出 / 否：从数据库中调出玩家数据
        CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if(poPlayer)
        {
            // 在内存中：直接取出玩家数据存入列表
            BOOL bSucc = CPKPktBuilder::Instance()->BuildRetPlayer(stSyncPlayer, poPlayer);
            if(FALSE == bSucc)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CPKProcessor::TakePlayer player [%u] is in memory, but build player failed!"), MSG_MARK, poPlayer->GetID());
                return FALSE;
            }
        }
        else
        {
            // 不在内存中：从数据库中调出玩家数据，成功调出后再返回给跨服战服务器
            CGetPlayerDataMgr::Instance()->GetPlayerData(dwPKGroupID, GET_PLAYERDATATYPE_REMOTE_PK, dwPlayerID);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CPKProcessor::OnGetPlayerReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    PKT_PKGS_GET_PLAYER_REQ *pReq = (PKT_PKGS_GET_PLAYER_REQ*)pBody;

    PKT_GSPK_RET_PLAYER_ACK ack;
    memset(&ack, 0, sizeof(ack));

    ack.byRetCnt    = 0;
    ack.dwPKGroupID = pReq->dwPKGroupID;

    pReq->byGetCnt = MIN(pReq->byGetCnt, MAX_REQ_SYNC_PLAYER_CNT_ONCE);

    // 根据调取的玩家id列表调出玩家数据
    // 1.将已在内存中的玩家成批返回
    // 2.其余的则到数据库一个个取出来再返回
    for(BYTE i = 0; i < pReq->byGetCnt; i++)
    {
        PlayerID playerID = pReq->adwPlayerIDList[i];

        BOOL bPlayerInMemory = TRUE;

        // 在玩家快照表中
        DT_SYNC_PLAYER &stSyncPlayer = ack.astPlayerList[ack.byRetCnt];

        BOOL bTakeOK = TakePlayer(playerID, stSyncPlayer, pReq->dwPKGroupID);
        if(!bTakeOK)
        {
            continue;
        }

        ack.byRetCnt++;

        // 如果要同步的玩家较多，则拆分成多个消息包返回
        if(ack.byRetCnt >= MAX_RET_SYNC_PLAYER_CNT_ONCE)
        {
            gsapi::SendMsg2PK((CHAR*)&ack, GSPK_RET_PLAYER_ACK);
            memset(&ack, 0, sizeof(ack));
            ack.byRetCnt    = 0;
            ack.dwPKGroupID = pReq->dwPKGroupID;
        }
    }

    if(ack.byRetCnt > 0)
    {
       gsapi::SendMsg2PK((CHAR*)&ack, GSPK_RET_PLAYER_ACK);
    }

    return TRUE;
}

BOOL CPKProcessor::OnRetPlayerAck(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwZoneID              = pPipeHeader->dwTransID;

    PKT_GSPK_RET_PLAYER_ACK *pAck = (PKT_GSPK_RET_PLAYER_ACK*)pBody;
    if(NULL == pAck)
    {
        return FALSE;
    }

    CPullPlayerMgr::Instance()->HandleRetPlayer((UINT16)dwZoneID, *pAck);
    return TRUE;
}

BOOL CPKProcessor::OnGetFactions(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    if(dwLen != sizeof(PKT_PKGS_GET_FACTIONS_REQ))
    {
        return FALSE;
    }

    PKT_PKGS_GET_FACTIONS_REQ *pReq = (PKT_PKGS_GET_FACTIONS_REQ*)pBody;
    if(NULL == pReq)
    {
        return FALSE;
    }

    // 这里要注意，即便请求的门派个数为0，仍然要返回消息包
    UINT16 wMsgID = CPKPktBuilder::Instance()->RetFactions(pReq->stFactionIDList.adwFactionList, pReq->stFactionIDList.byFactionCnt);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    PKT_GSPK_RET_FACTIONS_ACK *pAck = (PKT_GSPK_RET_FACTIONS_ACK*)CPKPktBuilder::Instance()->GetPacketBuffer();
    gsapi::SendMsg2PK((CHAR*)pAck, wMsgID);

    return TRUE;
}

BOOL CPKProcessor::OnGetVideo(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{    
    if(dwLen != sizeof(PKT_GSPK_GET_VIDEO_REQ))
    {
        return FALSE;
    }

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwZoneID = pPipeHeader->dwTransID;

    PKT_GSPK_GET_VIDEO_REQ *pReq = (PKT_GSPK_GET_VIDEO_REQ*)pBody;
    if(NULL == pReq)
    {
        return FALSE;
    }

    DBG_INFO(_SDT("[CPKProcessor::%s: %d]: CPKProcessor::OnGetVideo recv get video request from zone %u!"), MSG_MARK, dwZoneID);
    CGvGMgr::Instance()->SyncVideo(dwZoneID);
    return TRUE;
}

BOOL CPKProcessor::OnRetVideo(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    if(dwLen != sizeof(PKT_PKGS_RET_VIDEO_ACK))
    {
        return FALSE;
    }

    PKT_PKGS_RET_VIDEO_ACK *pstAck = (PKT_PKGS_RET_VIDEO_ACK*)pBody;
    if(NULL == pstAck)
    {
        return FALSE;
    }

    if (CGvGBuild::Instance()->IsAllVideoHere())
    {
        return TRUE;
    }

    CGvGVideoMgr::Instance()->AddPKVideo(*pstAck);

    // 如果录像已经全部被传输完毕，则打印日志以方便跟踪
    if (CGvGBuild::Instance()->IsAllVideoHere())
    {
        VideoIDSet setRequiredVideoID;
        CGvGBuild::Instance()->TakeRequiredVideoIDList(setRequiredVideoID);

        UINT16 wHereVideoCnt = CGvGVideoMgr::Instance()->CalcHereVideoCnt(setRequiredVideoID);
        DBG_INFO(_SDT("[%s: %d]: CPKProcessor::OnRetVideo, now all video is here, required video count = %u, current here video count = %u, current stage = %u"), MSG_MARK, setRequiredVideoID.size(), wHereVideoCnt, CGvGBuild::Instance()->GetCurStage());
    }

    return TRUE;
}

BOOL CPKProcessor::OnSyncVideo(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    if(dwLen != sizeof(PKT_PKGS_SYNC_VIDEO_NTF))
    {
        return FALSE;
    }

    PKT_PKGS_SYNC_VIDEO_NTF *pNtf = (PKT_PKGS_SYNC_VIDEO_NTF*)pBody;
    if(NULL == pNtf)
    {
        return FALSE;
    }

    if(0 == pNtf->wVideoCnt)
    {
        return FALSE;
    }

    SGvGVideo stVideo;
    for(UINT16 i = 0; i < pNtf->wVideoCnt; i++)
    {
        stVideo.stGvGInfo = pNtf->astVideoList[i];
        if(CGvGVideoMgr::Instance()->GetVideo(stVideo.stGvGInfo.qwVideoID))
        {
            continue;
        }
        
        CGvGVideoMgr::Instance()->AddVideo(stVideo);
    }

    return TRUE;
}

// gvg->gs
BOOL CPKProcessor::OnSyncGvG(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    PKT_PKGS_SYNC_GVG_NTF *pNtf = (PKT_PKGS_SYNC_GVG_NTF*)pBody;
    if(NULL == pNtf)
    {
        return FALSE;
    }

    CGvGBuild::Instance()->HandleSyncGvGNtf(*pNtf);
    return TRUE;
}

BOOL CPKProcessor::OnSupportFaction(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    PKT_GSPK_SUPPORT_FACTION_REQ *pReq = (PKT_GSPK_SUPPORT_FACTION_REQ*)pBody;
    if(NULL == pReq)
    {
        return FALSE;
    }

    CGvGMgr::Instance()->SupportFaction((GvGRound)pReq->byBigRound, pReq->dwArenaID, pReq->stFactionSupportInfo);
    return TRUE;
}

BOOL CPKProcessor::OnRecvFactionSupportList(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    PKT_PKGS_SYNC_FACTION_SUPPORT_NTF *pNtf = (PKT_PKGS_SYNC_FACTION_SUPPORT_NTF*)pBody;
    if(NULL == pNtf)
    {
        return FALSE;
    }


    CGvGBuild::Instance()->HandleSyncSupportNtf(pNtf->stFactionSupportInfo);
    return TRUE;
}

BOOL CPKProcessor::OnAskGvGReady(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    PKT_PKGS_GVG_ASK_READY_REQ *pReq = (PKT_PKGS_GVG_ASK_READY_REQ*)pBody;
    if(NULL == pReq)
    {
        return FALSE;
    }

    GVG_STAGE eStage = (GVG_STAGE)pReq->byStage;

    PKT_GSPK_GVG_REPLY_READY_ACK stAck;
    memset(&stAck, 0, sizeof(stAck));

    stAck.byStage   = eStage;
    stAck.byIsReady = CGvGBuild::Instance()->IsReadyByStage(eStage);

    gsapi::SendMsg2PK((CHAR*)&stAck, GSPK_GVG_REPLY_READY_ACK);
    return TRUE;
}

BOOL CPKProcessor::OnRecvGvGReadyReply(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwZoneID              = pPipeHeader->dwTransID;

    PKT_GSPK_GVG_REPLY_READY_ACK *pstAck = (PKT_GSPK_GVG_REPLY_READY_ACK*)pBody;
    if(NULL == pstAck)
    {
        return FALSE;
    }

    CGvGMgr::Instance()->OnZoneSayReady(dwZoneID, *pstAck);
    return TRUE;
}

BOOL CPKProcessor::OnGetGvGActivityDataReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwZoneID              = pPipeHeader->dwTransID;

    CGvGMgr::Instance()->PushActivityDataToZone((UINT16)dwZoneID);

    return TRUE;
}

