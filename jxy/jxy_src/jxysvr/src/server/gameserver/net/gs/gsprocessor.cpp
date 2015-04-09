#include "gsprocessor.h"
#include "gspktbuilder.h"
#include <commondef.h>
#include <sdutil.h>
#include <errdef.h>
#include <db/autosqlbuf.h>
#include <net/gt/gtpipechannel.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <sdstring.h>
#include <net/db/dbprocessor.h>
#include <sdframework/keepalive/controlkeepalive.h>
#include <logic/race/racemgr.h>
#include <logic/other/commoninfomgr.h>
#include <logic/activity/bossbattlemgr.h>
#include <logic/monster/monster.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <framework/gsapi.h>
#include <framework/gsapplication.h>
#include "net/cli/usermgr.h"
#include <logic/player/player.h>


CGSProcessor::CGSProcessor()
{
    m_itrDeal = m_mapSyncPlayer.end();
    Init() ;
}

CGSProcessor::~CGSProcessor()
{

}

CSDProtocol* CGSProcessor::GetProtocol()
{
    return CProtoGSGS::Instance();
}


BOOL CGSProcessor::Init()
{
    RegisterCommand(GSGS_START_SYNC_PLAYER_RPT, CGSProcessor::OnStartSyncPlayer);
    RegisterCommand(GSGS_SYNC_PLAYER_REQ, CGSProcessor::OnSyncPlayer);
    RegisterCommand(GSGS_SYNC_PLAYER_RPT, CGSProcessor::OnSyncPlayerRpt);
    RegisterCommand(GSGS_SYNC_DSPNAME_REQ, CGSProcessor::OnSyncDspNameReq);
    RegisterCommand(GSGS_SYNC_DSPNAME_RPT, CGSProcessor::OnSyncDspNameRpt);
    RegisterCommand(GSGS_SYNC_BOSSB_REQ, CGSProcessor::OnSyncBossBReq);
    RegisterCommand(GSGS_SYNC_BOSSB_RPT, CGSProcessor::OnSyncBossBRpt);
    return TRUE ;
}

BOOL CGSProcessor::StartSyncPlayer(UINT64 qwServerStartTime)
{
    USR_INFO(_SDT("[%s: %d]: StartSyncPlayer!"), MSG_MARK);

    UINT16 wMsgID = CGSPktBuilder::Instance()->StartSyncPlayer(qwServerStartTime);
    if(INVALID_MSGID != wMsgID)
    {
        CSDPipeChannel* poPipeChannel = gsapi::GetGSPipeChannel();
        if (poPipeChannel)
        {
            poPipeChannel->SendMsg(1, wMsgID, CGSPktBuilder::Instance()->GetPacketBuffer());
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Get GS pipe failed!"), MSG_MARK);
        }
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: StartSyncPlayer failed!"), MSG_MARK);
    }
    return TRUE;
}

BOOL CGSProcessor::OnStartSyncPlayer(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GSGS_START_SYNC_PLAYER_RPT* pRpt = (PKT_GSGS_START_SYNC_PLAYER_RPT*)pBody;
    //保证只有后面的GS发起请求
    if(gsapi::GetServerStartTime() < pRpt->qwServerStartTime)
    {
        CZoneID2PipeChannelMap&	mapAllGtPiple = ((CGSApplication *)SDGetApp())->GetAllGTPipeChannel();
        for(CZoneID2PipeChannelMapItr itr = mapAllGtPiple.begin(); itr != mapAllGtPiple.end(); itr++)
        {
            CSDPipeChannel* poPipeChannel = itr->second;
            if(poPipeChannel)
            {
                CGTPipeChannel* poGTPipeChannel = dynamic_cast<CGTPipeChannel*>(poPipeChannel);
                if(poGTPipeChannel)
                {
                    poGTPipeChannel->SetRefusePkg(TRUE);
                }
            }

        }
        ISDListener* poListener = ((CGSApplication *)SDGetApp())->GetRechargeListener();
        if(poListener) poListener->Stop();
        //CGSProcessor::Instance()->SyncPlayer(TRUE);
        PKT_GSGS_SYNC_DSPNAME_RPT stRpt;
        stRpt.dwLastPlayerID = 0;//表示刚开始
        return OnSyncDspNameRpt(pPipeChannel, pHeader, (const CHAR*)(&stRpt), sizeof(stRpt));
    }

    return TRUE;
}

BOOL CGSProcessor::OnSyncPlayer(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GSGS_SYNC_PLAYER_REQ* pReq = (PKT_GSGS_SYNC_PLAYER_REQ*)pBody;

    DT_PLAYER_DATA stDT_PLAYER_DATA;
    memset(&stDT_PLAYER_DATA, 0, sizeof(stDT_PLAYER_DATA));

    if( -1 == stDT_PLAYER_DATA.DecodeSaveData((CHAR*)(pReq->abyPlayerDataInfo), pReq->dwPlayerDataLen ))
    {
        SYS_CRITICAL(_SDT("[%s: %d] DecodeSaveData Failed, ID[%d]!"), MSG_MARK, dwTransID);
        return FALSE;
    }
    DBG_INFO(_SDT("[%s: %d] OnSyncPlayer player[%d]!"), MSG_MARK, stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);

    //先创建User
    if(1 == pReq->byHaveUserFlag)
    {
        DT_USER_DATA* pstDT_USER_DATA = &pReq->astUserInfo[0];
        if(!CUserMgr::Instance()->FindUser(pstDT_USER_DATA->dwPlayerID))
        {
            CUser* poUser = CUserMgr::Instance()->CreateUser(pstDT_USER_DATA->dwUserID, pstDT_USER_DATA->dwPlayerID, pstDT_USER_DATA->wCareerID, pstDT_USER_DATA->aszUserName,
                            pstDT_USER_DATA->aszDeviceID, pstDT_USER_DATA->aszNotifyID, pstDT_USER_DATA->wZoneID, pstDT_USER_DATA->wOriZoneID, pstDT_USER_DATA->dwLoginIP, pstDT_USER_DATA->dwCliVer, pstDT_USER_DATA->byAuthType) ;
            if(poUser)
            {
                poUser->SetUserStat(CLIS_IN_GAME);
                DBG_INFO(_SDT("[%s: %d] CreateUser[%d] OK!"), MSG_MARK, pstDT_USER_DATA->dwPlayerID);
            }
            else
            {
                DBG_INFO(_SDT("[%s: %d] CreateUser[%d] failed!"), MSG_MARK, pstDT_USER_DATA->dwPlayerID);
            }
        }
    }
    else
    {
        DBG_INFO(_SDT("[%s: %d] Player[%d] byHaveUserFlag is 0!"), MSG_MARK, stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID);
    }

    UINT32 dwPlayerID = stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID;
    if(!CPlayerMgr::Instance()->FindPlayer(dwPlayerID))
    {
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
        //管理数据使用内存值，修改dwRank，dwCaptureOwner，bOpenPlunder.等级不需修改，差1无所谓
        if(pstPlayerBaseData)
        {
            UINT32 dwRank = stDT_PLAYER_DATA.stPlayerPvpData.stRaceData.stRaceBaseData.dwRank;
            if(dwRank > CRaceMgr::Instance()->m_dwMaxRank)
            {
                CRaceMgr::Instance()->m_dwMaxRank = dwRank;
            }
            if(pstPlayerBaseData->dwRank != dwRank)
            {
                if(0 == pstPlayerBaseData->dwRank)
                {
                    CPlayerBaseDataCacheMgr::Instance()->OnRankNew(dwPlayerID, dwRank);
                }
                else
                {
                    SPlayerBaseData* pstPlayerBaseData2 = CRaceMgr::Instance()->GetRankPlayerBaseData(dwRank);
                    //该排名已经被占用，交换
                    if(pstPlayerBaseData2)
                    {
                        CPlayerBaseDataCacheMgr::Instance()->OnRankExchange(dwPlayerID, pstPlayerBaseData->dwRank, pstPlayerBaseData2->dwID, pstPlayerBaseData2->dwRank);
                    }
                    //该排名没有被占用
                    else
                    {
                        CPlayerBaseDataCacheMgr::Instance()->OnRankNew(dwPlayerID, dwRank);
                    }
                }
            }
            (const_cast<SPlayerBaseData*>(pstPlayerBaseData))->bOpenPlunder = stDT_PLAYER_DATA.stPlayerPvpData.stPlunderData.stPlunderBaseData.byHaveDataFlag == 0 ? FALSE : TRUE;
            (const_cast<SPlayerBaseData*>(pstPlayerBaseData))->dwCaptureOwner = pstPlayerBaseData->bOpenPlunder ? stDT_PLAYER_DATA.stPlayerPvpData.stPlunderData.stPlunderBaseData.dwCaptureOwnerPlayerID : 0;
        }

        CDBProcessor::OnPlayerDataAck_SyncData(SUCCESS, stDT_PLAYER_DATA, dwTransID);

        DT_PLAYER_NOPVP_DATA stPlayerNoPvPData;
        memset(&stPlayerNoPvPData, 0, sizeof(stPlayerNoPvPData));

        gsapi::GetDBMgr()->SetStartData(stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData.dwID, stPlayerNoPvPData);
    }

    UINT16 wMsgID = CGSPktBuilder::Instance()->SyncPlayerRpt(dwTransID, 0);
    if(INVALID_MSGID != wMsgID)
    {
        CSDPipeChannel* poPipeChannel = ((CGSApplication*)((CGSApplication*)SDGetApp()))->GetGSPipeChannel();
        if (poPipeChannel)
        {
            poPipeChannel->SendMsg(dwTransID, wMsgID, CGSPktBuilder::Instance()->GetPacketBuffer());
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Get GS pipe failed!"), MSG_MARK);
        }
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SyncPlayerRpt failed, PlayerID[%d]!"), MSG_MARK, dwTransID);
    }
    return TRUE;
}


BOOL CGSProcessor::OnSyncPlayerRpt(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GSGS_SYNC_PLAYER_RPT* pRpt = (PKT_GSGS_SYNC_PLAYER_RPT*)pBody;
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(pRpt->dwPlayerID);
    if(poPlayer)
    {
        poPlayer->OnLogin(FALSE); //下线，避免传送后GS Uninit时还保存
    }
    CGSProcessor::Instance()->SyncPlayer();

    return TRUE;
}


VOID CGSProcessor::SyncPlayer(BOOL bStart)
{
    if((bStart) && (0 == m_mapSyncPlayer.size()))
    {
        m_mapSyncPlayer = CPlayerMgr::Instance()->GetID2PlayerMap();
        m_itrDeal = m_mapSyncPlayer.begin();
    }

    //同步完成，关闭该GS
    if(m_itrDeal == m_mapSyncPlayer.end())
    {
        USR_INFO(_SDT("[%s: %d]:SyncPlayer End!"), MSG_MARK);
        CControlKeepAlive::Instance()->SyncStop();
        ((CGSApplication *)SDGetApp())->GetNeedSave2DB() = FALSE;
    }

    BOOL bHaveData = FALSE;
    while(!bHaveData && m_itrDeal != m_mapSyncPlayer.end())
    {
        CPlayer* poPlayer = m_itrDeal->second;
        if(poPlayer)
        {
            //跳过机器人
            if(0 != poPlayer->GetDT_PLAYER_BASE_DATA().byRobotFunc)
            {
                m_itrDeal++;
                continue;
            }

            if(((FALSE == poPlayer->CkLogin()) && (ESS_WANT_SAVE == poPlayer->GetSaveState())) //非在线玩家，playerbasedata不会主动更新，则在其他模块更新是才需保存
                    || (TRUE == poPlayer->CkLogin()) //在线用户
                    || (NULL != CUserMgr::Instance()->FindUser(poPlayer->GetID()))) //不是被调入内存的玩家
            {
                UINT16 wMsgID = CGSPktBuilder::Instance()->SyncPlayer(poPlayer);
                if(INVALID_MSGID != wMsgID)
                {
                    CSDPipeChannel* poPipeChannel = ((CGSApplication*)((CGSApplication*)SDGetApp()))->GetGSPipeChannel();
                    if (poPipeChannel)
                    {
                        poPipeChannel->SendMsg(poPlayer->GetID(), wMsgID, CGSPktBuilder::Instance()->GetPacketBuffer());
                        bHaveData = TRUE;
                        DBG_INFO(_SDT("[%s: %d] SyncPlayer player[%d]!"), MSG_MARK, poPlayer->GetID());
                    }
                    else
                    {
                        SYS_CRITICAL(_SDT("[%s: %d]: Get GS pipe failed!"), MSG_MARK);
                    }
                }
                else
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: SyncPlayer failed, PlayerID[%d]!"), MSG_MARK, poPlayer->GetID());
                }
            }
            else
            {
                DBG_INFO(_SDT("[%s: %d] SyncPlayer player[%d] skip!"), MSG_MARK, poPlayer->GetID());
            }
        }
        m_itrDeal++;
    }
}



BOOL CGSProcessor::OnSyncDspNameReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GSGS_SYNC_DSPNAME_REQ* pReq = (PKT_GSGS_SYNC_DSPNAME_REQ*)pBody;
    UINT16 dwLastPlayerID = 0;
    CPlayerID2NameMap& mapPlayerID2CacheName = CCommonInfoMgr::Instance()->m_mapPlayerID2CacheName;
    CName2PlayerIDMap& mapCacheName2PlayerID = CCommonInfoMgr::Instance()->m_mapCacheName2PlayerID;
    for(UINT16 wIdx = 0; wIdx < pReq->wSyncNum; wIdx++)
    {
        DT_PLAYER_DSPNAME_DATA& stDT_PLAYER_DSPNAME_DATA = pReq->astPlayerDspNameInfo[wIdx];
        mapPlayerID2CacheName[stDT_PLAYER_DSPNAME_DATA.dwPlayerID] = stDT_PLAYER_DSPNAME_DATA.aszDspName;
        mapCacheName2PlayerID[stDT_PLAYER_DSPNAME_DATA.aszDspName] = stDT_PLAYER_DSPNAME_DATA.dwPlayerID;
        dwLastPlayerID = stDT_PLAYER_DSPNAME_DATA.dwPlayerID;
    }

    UINT16 wMsgID = CGSPktBuilder::Instance()->SyncDspNameRpt(dwLastPlayerID, 0);
    if(INVALID_MSGID != wMsgID)
    {
        CSDPipeChannel* poPipeChannel = ((CGSApplication*)((CGSApplication*)SDGetApp()))->GetGSPipeChannel();
        if (poPipeChannel)
        {
            poPipeChannel->SendMsg(dwTransID, wMsgID, CGSPktBuilder::Instance()->GetPacketBuffer());
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Get GS pipe failed!"), MSG_MARK);
        }
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SyncDspNameRpt failed!"), MSG_MARK);
    }

    return TRUE;
}

BOOL CGSProcessor::OnSyncDspNameRpt(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GSGS_SYNC_DSPNAME_RPT* pRpt = (PKT_GSGS_SYNC_DSPNAME_RPT*)pBody;
    CPlayerID2NameMap& mapPlayerID2CacheName = CCommonInfoMgr::Instance()->m_mapPlayerID2CacheName;
    CPlayerID2NameMapItr itr = mapPlayerID2CacheName.find(pRpt->dwLastPlayerID);
    //已经没有，同步boss战
    if((itr == mapPlayerID2CacheName.end()) || (0 != pRpt->dwLastPlayerID))
    {
        return SyncBossB();
    }
    //取下一批
    PKT_GSGS_SYNC_DSPNAME_REQ stReq;
    for(itr++; ((itr != mapPlayerID2CacheName.end()) && (stReq.wSyncNum < MAX_SYNC_DSPNAME_ONCE)); itr++)
    {
        DT_PLAYER_DSPNAME_DATA& stDT_PLAYER_DSPNAME_DATA = stReq.astPlayerDspNameInfo[stReq.wSyncNum++];
        stDT_PLAYER_DSPNAME_DATA.dwPlayerID = itr->first;
        SDStrncpy(stDT_PLAYER_DSPNAME_DATA.aszDspName, itr->second.c_str(), MAX_SYNC_DSPNAME_ONCE - 1);
    }
    //已经没有，同步boss战
    if(0 == stReq.wSyncNum)
    {
        return SyncBossB();
    }

    CSDPipeChannel* poPipeChannel = ((CGSApplication*)((CGSApplication*)SDGetApp()))->GetGSPipeChannel();
    if (poPipeChannel)
    {
        poPipeChannel->SendMsg(dwTransID, GSGS_SYNC_DSPNAME_REQ, (const CHAR*)(&stReq));
    }


    return TRUE;
}

BOOL CGSProcessor::OnSyncBossBReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GSGS_SYNC_BOSSB_REQ* pReq = (PKT_GSGS_SYNC_BOSSB_REQ*)pBody;

    //boss战已经开启
    if((NULL != CBossBattleMgr::Instance()->m_pstCurBossBProp) && (NULL != CBossBattleMgr::Instance()->m_poBoss))
    {
        if((CBossBattleMgr::Instance()->m_pstCurBossBProp->byActivityId == pReq->byActivityID) && (pReq->qwCurHP < CBossBattleMgr::Instance()->m_poBoss->GetDT_BATTLE_ATTRIBUTE().qwHP) && (0 != pReq->byActivityID))
        {
            CBossBattleMgr::Instance()->m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP = pReq->qwCurHP;
            CBossBattleMgr::Instance()->m_qwStartTime = pReq->qwStartTime;
            CBossBattleMgr::Instance()->m_bSyncFlag = TRUE;
        }
    }
    //未开启，设置参数
    else
    {
        CBossBattleMgr::Instance()->m_bySyncActivityID = pReq->byActivityID;
        CBossBattleMgr::Instance()->m_qwSyncCurBossHP = pReq->qwCurHP;
        CBossBattleMgr::Instance()->m_qwSyncStartTime = pReq->qwStartTime;
    }

    PKT_GSGS_SYNC_BOSSB_RPT stRpt;
    CSDPipeChannel* poPipeChannel = ((CGSApplication*)((CGSApplication*)SDGetApp()))->GetGSPipeChannel();
    if (poPipeChannel)
    {
        poPipeChannel->SendMsg(1, GSGS_SYNC_BOSSB_RPT, (const CHAR*)(&stRpt));
    }

    return TRUE;
}

BOOL CGSProcessor::OnSyncBossBRpt(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    //同步玩家
    CGSProcessor::Instance()->SyncPlayer(TRUE);

    return TRUE;
}


BOOL CGSProcessor::SyncBossB()
{
    //boss战活动中
    if((NULL != CBossBattleMgr::Instance()->m_pstCurBossBProp) && (NULL != CBossBattleMgr::Instance()->m_poBoss))
    {
        PKT_GSGS_SYNC_BOSSB_REQ stReq;
        stReq.byActivityID = CBossBattleMgr::Instance()->m_pstCurBossBProp->byActivityId;
        stReq.qwCurHP = CBossBattleMgr::Instance()->m_poBoss->GetCurDT_BATTLE_ATTRIBUTE().qwHP;
        stReq.qwStartTime = CBossBattleMgr::Instance()->m_qwStartTime;
        CSDPipeChannel* poPipeChannel = ((CGSApplication*)((CGSApplication*)SDGetApp()))->GetGSPipeChannel();
        if (poPipeChannel)
        {
            poPipeChannel->SendMsg(1, GSGS_SYNC_BOSSB_REQ, (const CHAR*)(&stReq));
        }
    }
    //不在活动中，不需同步，直接同步玩家
    else
    {
        CGSProcessor::Instance()->SyncPlayer(TRUE);
    }

    return TRUE;
}
