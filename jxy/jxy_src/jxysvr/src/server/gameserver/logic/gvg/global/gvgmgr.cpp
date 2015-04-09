///<------------------------------------------------------------------------------
//< @file:   logic\gvg\gvgmgr.cpp
//< @author: hongkunan
//< @date:   2014年1月13日 22:43:24
//< @brief:
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/gvg/global/gvgmgr.h"

#include <algorithm>
#include <protogspk.h>
#include <sdloggerimpl.h>

#include "logic/gvg/global/pullgroup.h"
#include "pullplayermgr.h"

#include "framework/gsapi.h"
#include "logic/event/timereventmgr.h"
#include "logic/faction/faction.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/gvg/gvgvideomgr.h"
#include "logic/event/timereventmgr.h"
#include "logic/battle/battlemgr.h"
#include "logic/gvg/local/gvgbuild.h"
#include "db/dbcommoperate.h"

#include <net/pk/pkpktbuilder.h>

// 保存数据库的间隔
#define SAVE_GVG_MGR_INTERVAL 20

// 同步活动当前轮次的间隔
#define SYNC_ACTIVITY_ROUND_INTERVAL 10

// 执行活动任务的间隔
#define RUN_GVG_ACTIVITY_INTERVAL 10

IMPLEMENT_SINGLETON(CGvGMgr)

namespace gvgutil
{
    void SaveFactionInfo(DT_GVG_VIDEO_FACTION &stVideoFaction, CPKFaction *poPKFaction)
    {
        if(NULL == poPKFaction)
        {
            return;
        }

        const DT_PK_FACTION &stPKFaction = poPKFaction->GetData();
        const DT_FACTION_BASE_DATA_CLI2 &stCli = stPKFaction.stBaseData.stBaseData;

        stVideoFaction.dwFactionID     = stCli.dwFactionID;
        stVideoFaction.wZoneID         = stPKFaction.stBaseData.wOriZoneID;
        stVideoFaction.byFactionIconID = stCli.byIconID;
        stVideoFaction.stPlayerList    = stPKFaction.stPlayerList;

        SDStrncpy(stVideoFaction.aszFactionName, stCli.aszFactionName, MAX_FACTION_NUM - 1);
    }

    void SaveFactionInfo(DT_GVG_VIDEO_FACTION &stVideoFaction, CFaction *poFaction)
    {
        if(NULL == poFaction)
        {
            return;
        }

        DT_PK_FACTION stPKFaction;
        poFaction->SaveGvGPKInfoTo(stPKFaction);
        const DT_FACTION_BASE_DATA_CLI2 &stCli = stPKFaction.stBaseData.stBaseData;

        stVideoFaction.dwFactionID     = poFaction->GetFactionID();
        stVideoFaction.wZoneID         = stPKFaction.stBaseData.wOriZoneID;
        stVideoFaction.byFactionIconID = stCli.byIconID;
        stVideoFaction.stPlayerList    = stPKFaction.stPlayerList;

        SDStrncpy(stVideoFaction.aszFactionName, stCli.aszFactionName, MAX_FACTION_NUM - 1);
    }

    void GetEachObjMaxHP(DT_BATTLE_OBJ_MAX_HP_INFO &stMaxHPGroup, CPlayer *poPlayer)
    {
        memset(&stMaxHPGroup, 0, sizeof(stMaxHPGroup));

        DT_BATTLE_OBJ_GROUP_DATA3 stObjGroup;
        memset(&stObjGroup, 0, sizeof(stObjGroup));

        poPlayer->GetDT_BATTLE_OBJ_GROUP_DATA(stObjGroup);

        for(UINT16 i = 0; i < stObjGroup.byObjNum; i++)
        {
            DT_BATTLE_OBJ_DATA2 &stObjInfo    = stObjGroup.astBattleObjInfo[i];
            DT_BATTLE_OBJ_MAX_HP &stMaxHPInfo = stMaxHPGroup.astBattleObjMaxHPInfo[stMaxHPGroup.byObjNum];

            if(0 == stObjInfo.byFomationIdx)
            {
                continue;
            }

            stMaxHPInfo.byFomationIdx = stObjInfo.byFomationIdx;
            stMaxHPInfo.dwMaxHP       = stObjInfo.stBattleAttr.dwHP;

            stMaxHPGroup.byObjNum++;
        }
    }

    // 清除掉已死亡伙伴的最大血量信息
    void CleanMaxHPOfDeadHero(DT_BATTLE_OBJ_MAX_HP_INFO &stMaxHPGroup, const DT_BATTLE_OBJ_GROUP_HP *pstCurHPGroup)
    {
        if(NULL == pstCurHPGroup)
        {
            return;
        }

        DT_BATTLE_OBJ_MAX_HP_INFO stTmp = stMaxHPGroup;
        memset(&stMaxHPGroup, 0, sizeof(stMaxHPGroup));

        for (UINT16 i = 0; i < stTmp.byObjNum; i++)
        {
            DT_BATTLE_OBJ_MAX_HP &stCopyMaxHP = stTmp.astBattleObjMaxHPInfo[i];

            BOOL bDead = FALSE;
            for (UINT16 wObjIdx = 0; wObjIdx < pstCurHPGroup->byObjNum; wObjIdx++)
            {
                const DT_BATTLE_OBJ_HP &stCurHP = pstCurHPGroup->astBattleObjInfo[wObjIdx];
                if(stCurHP.byFormationIdx == stCopyMaxHP.byFomationIdx && 0 == stCurHP.dwHP)
                {
                    bDead = TRUE;
                    break;
                }
            }

            if(!bDead)
            {
                stMaxHPGroup.astBattleObjMaxHPInfo[stMaxHPGroup.byObjNum++] = stCopyMaxHP;
            }
        }        
    }

    BOOL RecoverHP(const DT_BATTLE_OBJ_MAX_HP_INFO &stMaxHPGroup, DT_BATTLE_OBJ_GROUP_HP &stCurHPGroup, BYTE byKillCombo)
    {
        UINT16 wRecoverHPPercent = CGvGPropMgr::Instance()->CalcRecoverHPByKillCombo(byKillCombo);
        if(0 == wRecoverHPPercent)
        {
            return FALSE;
        }

        for (UINT16 i = 0; i < stCurHPGroup.byObjNum; i++)
        {
            DT_BATTLE_OBJ_HP &stCurHP = stCurHPGroup.astBattleObjInfo[i];
            for (UINT16 wObjIdx = 0; wObjIdx < stMaxHPGroup.byObjNum; wObjIdx++)
            {
                const DT_BATTLE_OBJ_MAX_HP &stMaxHP = stMaxHPGroup.astBattleObjMaxHPInfo[wObjIdx];
                if(stMaxHP.byFomationIdx == stCurHP.byFormationIdx)
                {
                    UINT32 dwRecoverHP = (UINT32)(stMaxHP.dwMaxHP * ((double)wRecoverHPPercent / 100));
                    stCurHP.dwHP += dwRecoverHP;
                    stCurHP.dwHP = MIN(stCurHP.dwHP, stMaxHP.dwMaxHP);
                    break;
                }
            }
        }
        
        return TRUE;
    }

    void CopyHPGridNum(DT_BATTLE_OBJ_GROUP_DATA3 &stDst, const DT_BATTLE_OBJ_GROUP_DATA3 &stSrc)
    {
        for(UINT16 wDstIdx = 0; wDstIdx < stDst.byObjNum; wDstIdx++){
            DT_BATTLE_OBJ_DATA2 &stDstObj = stDst.astBattleObjInfo[wDstIdx];

            for(UINT16 wSrcIdx = 0; wSrcIdx < stSrc.byObjNum; wSrcIdx++){
                const DT_BATTLE_OBJ_DATA2 &stSrcObj = stSrc.astBattleObjInfo[wSrcIdx];
                if(stSrcObj.byFomationIdx == stDstObj.byFomationIdx){
                    stDstObj.byHPGridNum = stSrcObj.byHPGridNum;
                    break;
                }
            }
        }        
    }

    void CopyHp(DT_BATTLE_OBJ_MAX_HP_INFO &stDst, const DT_BATTLE_OBJ_GROUP_HP &stSrc)
    {
        memset(&stDst, 0, sizeof(stDst));

        for(UINT16 i = 0; i < stSrc.byObjNum; i++)
        {
            const DT_BATTLE_OBJ_HP &stSrcHP     = stSrc.astBattleObjInfo[i];
            DT_BATTLE_OBJ_MAX_HP &stDstHP = stDst.astBattleObjMaxHPInfo[stDst.byObjNum++];

            stDstHP.byFomationIdx = stSrcHP.byFormationIdx;
            stDstHP.dwMaxHP = stSrcHP.dwHP;
        }
    }
}

BOOL CGvGMgr::Init()
{
    // 初始化CFactionMgr管理器
    if (! CPullPlayerMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPullPlayerMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(! CPullPlayerMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPullPlayerMgr Init failed!"), MSG_MARK);
        return FALSE;
    }
    else
    {
        USR_INFO(_SDT("[%s: %d]: CPullPlayerMgr Init OK!"), MSG_MARK);
    }

    memset(&m_stActData, 0, sizeof(m_stActData));

    if(!LoadGvGActivityDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::Init LoadGvGDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadGvGArenaDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::Init LoadGvGArenaDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!RecoverProgress())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: RecoverProgress failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitTimerEvent())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitTimerEvent failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

void CGvGMgr::Run()
{
    // 上一次保存数据的时间
    static UINT64 qwLastSaveTickTime = 0;

    // 上一次同步活动轮次的时间
    static UINT64 qwLastSyncTime = 0;

    // 上一次执行活动任务的时间
    static UINT64 qwLastRunActivityTime = 0;

    UINT64 qwNow = SGDP::SDTimeSecs();
    if(qwLastSaveTickTime < (qwNow - SAVE_GVG_MGR_INTERVAL))
    {
        SaveToDB();
        qwLastSaveTickTime = qwNow;
    }
    
    if(qwLastRunActivityTime < (qwNow - RUN_GVG_ACTIVITY_INTERVAL))
    {
        // 重新进入当前轮次，确保跨服战进程不因意外事件而中断，如：游戏服务器崩溃导致某次通知未被送达、网络阻塞导致丢包等
        ReEnterCurStage();
        qwLastRunActivityTime = qwNow;
    }
}

BOOL CGvGMgr::InitTimerEvent()
{
    if(!CTimerEventMgr::Instance()->AddGvGEvent())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddGvGEvent failed!"), MSG_MARK);
        return FALSE;
    }

    if(NULL == CTimerEventMgr::Instance()->AddGvGReloadPropEvent())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddGvGReloadPropEvent failed!"), MSG_MARK);
        return FALSE;
    }

    if(NULL == CTimerEventMgr::Instance()->AddGvGRePullEvent())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddGvGRePullEvent failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

// 跨服战进入下一轮
BOOL CGvGMgr::EnterStage(GVG_STAGE eTargetStage)
{
    if(GVG_0_NOT_START == eTargetStage)
    {
        return TRUE;
    }

    GVG_STAGE eNextStage = gvgutil::GetNextStage(m_eStage);
    if(eTargetStage != m_eStage && eTargetStage != eNextStage)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::EnterStage eTargetStage<%u> != m_eStage<%u> && eTargetStage<%u> != eNextStage<%u>!"), MSG_MARK, eTargetStage, m_eStage, eTargetStage, eNextStage);
        return FALSE;
    }

    if(m_eStage != eTargetStage)
    {
        if(!IsDone(m_eStage))
        {
            ReEnterCurStage();
            return FALSE;
        }
    }    

    switch(eTargetStage)
    {
    case GVG_1_APPLY:
        GvGStart();
        break;

    case GVG_7_END:
        GvGOver();
        break;

    case GVG_8_SLEEPING:
        GvGSleep();
        break;
    }

    for(UINT16 i = 0; i < m_vecArena.size(); i++)
    {
        CGvGArena *poArena = m_vecArena[i];
        if (poArena)
        {
            poArena->EnterStage(eTargetStage);
        }
    }

    SetStage(eTargetStage);
    SaveToDB();
    return TRUE;
}

void CGvGMgr::SetStage(GVG_STAGE eStage)
{
    m_eStage = eStage;
    m_stActData.byCurStage = m_eStage;
}

BOOL CGvGMgr::ReEnterCurStage()
{
    return EnterStage(m_eStage);
}

BOOL CGvGMgr::EnterNextStage()
{
    GVG_STAGE eNextGvGRound = gvgutil::GetNextStage(m_eStage);
    // USR_INFO(_SDT("[%s: %d]: OnGvGEvent: <current round = %u>, <next enter round = %u>."), MSG_MARK, m_eStage, eNextGvGRound);

    return EnterStage(eNextGvGRound);
}

BOOL CGvGMgr::GvGSleep()
{
    /*
    if (GVG_8_SLEEPING != m_eStage)
    {
        ++m_stActData.wSessionID;
    }
    */

    CGvGPropMgr::Instance()->FakeGvGTime(m_stActData.qwStartTime, GVG_8_SLEEPING);
    return TRUE;
}

BOOL CGvGMgr::GvGStart()
{
    if(IsDone(GVG_1_APPLY))
    {
        return TRUE;
    }

    // AskCanOpen();

    if(m_eStage != GVG_1_APPLY)
    {
        // 获取报名时间
        ++m_stActData.wSessionID;

        UINT64 qwOpenDate = CGvGPropMgr::Instance()->GetOpenDateBySession(m_stActData.wSessionID);
        m_stActData.qwStartTime = CGvGPropMgr::Instance()->GetStageStartTime(qwOpenDate, GVG_1_APPLY);

        CGvGPropMgr::Instance()->FakeGvGTime(m_stActData.qwStartTime, GVG_1_APPLY);
    }

    ClearActivity();
    
    BOOL bSucc = BuildEmptyArena();
    return bSucc;
}

void CGvGMgr::ClearActivity()
{
    ClearArena();
    CGvGVideoMgr::Instance()->Clear();

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_CLEAR_GVG_ACTIVITY);
    if(NULL == poCMD)
    {
        return;
    }

    gsapi::GetDBMgr()->AddCommand(poCMD);
}

void CGvGMgr::AskCanOpen()
{
    PKT_PKGS_GVG_ASK_OPEN_REQ stReq;
    stReq.byNothing = 0;

    // FillNotOpenZoneReply();

    ZoneIDSet setZoneID = GetArenaZoneIDList();
    for(ZoneIDSet::iterator itr = setZoneID.begin(); itr != setZoneID.end(); ++itr)
    {
        ZoneID zoneID = *itr;
        if(m_mapOpenReply.find(zoneID) != m_mapOpenReply.end())
        {
            continue;
        }

        gsapi::SendMsg2GS(zoneID, (CHAR*)&stReq, PKGS_GVG_ASK_OPEN_REQ);
    }
}

void CGvGMgr::GvGOver()
{
    m_mapOpenReply.clear();
}

void CGvGMgr::SyncVideo(ZoneID zoneID)
{
    CGvGArena *poArena = GetArenaByZoneID(zoneID);
    if (NULL == poArena)
    {
        SYS_CRITICAL( _SDT("[%s: %d] CGvGMgr::SyncVideo GetArena failed, zoneID = %u!"), MSG_MARK, zoneID);
        return;
    }

    poArena->SyncVideo(zoneID);
}

// 将活动数据推送给指定区
void CGvGMgr::PushActivityDataToZone(ZoneID zoneID)
{
    CGvGArena *poArena = GetArenaByZoneID(zoneID);
    if (NULL == poArena)
    {
        SYS_CRITICAL( _SDT("[%s: %d] CGvGMgr::PushActivityDataToZone GetArenaByZoneID failed, zoneID = %u, now try to get arena id from prop!"), MSG_MARK, zoneID);
        
        ArenaID arenaID = CGvGPropMgr::Instance()->GetArenaIDByZoneID(zoneID);
        poArena = GetArena(arenaID);
        if (NULL == poArena)
        {
            SYS_CRITICAL( _SDT("[%s: %d] CGvGMgr::PushActivityDataToZone CGvGPropMgr::Instance()->GetArenaIDByZoneID failed, zoneID = %u!"), MSG_MARK, zoneID);
            return;
        }
    }

    poArena->SyncActivity(zoneID);
}

BOOL CGvGMgr::SaveToDB()
{
    if(0 == m_stActData.wSessionID)
    {
        return FALSE;
    }

    m_stActData.byCurStage = m_eStage;
    m_stActData.byDone = (IsDone(m_eStage) && IsAllArenaDone(m_eStage));

    m_stActData.byArenaCnt = 0;
    for(UINT16 i = 0; i < m_vecArena.size(); i++)
    {
        if (m_vecArena[i])
        {
            m_stActData.byArenaCnt++;
        }
    }

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_GVG_ACTIVITY);
    if(NULL == poCMD)
    {
        return FALSE;
    }

    gsapi::GetDBMgr()->AddCommand(poCMD);
    poCMD->SetUserData(&m_stActData);

    for (UINT16 i = 0; i < m_vecArena.size(); i++)
    {
        CGvGArena *poArena = m_vecArena[i];
        if(poArena)
        {
            poArena->SaveToDB();
        }
    }

    CGvGVideoMgr::Instance()->SaveToDB();
    return TRUE;
}

BOOL CGvGMgr::LoadGvGActivityDB()
{
    memset(&m_stActData, 0, sizeof(m_stActData));

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    SGDP::ISDDBRecordSet* pRes = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgactivity order by SessionID desc limit 1", &pRes, 0, &strError);
    if(SDDB_NO_RECORDSET != nRet && SDDB_HAS_RECORDSET != nRet)
    {
        return FALSE;
    }

    if(SDDB_HAS_RECORDSET == nRet && pRes->GetRecord())
    {
        m_stActData.wSessionID = SDAtou(pRes->GetFieldValueByName("SessionID"));
        m_stActData.byCurStage = SDAtou(pRes->GetFieldValueByName("CurStage"));
        m_stActData.byArenaCnt = SDAtou(pRes->GetFieldValueByName("ArenaCnt"));

        const CHAR *pszStartTime = pRes->GetFieldValueByName("StartTime");
        CSDDateTime oStartTime;
        if(!SDTimeFromString(pszStartTime, oStartTime))
        {
            SYS_CRITICAL( _SDT("[%s: %d] CGvGBuild::LoadDataFromDB parse StartTime %s failed, aborted!"), MSG_MARK, pszStartTime);
            return FALSE;
        }
        
        m_stActData.qwStartTime = oStartTime.GetTimeValue();
    }

    if(pRes)
    {
        pRes->Release();
    }

    return TRUE;
}

BOOL CGvGMgr::LoadGvGArenaDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgarena", &pRecordSet, 0, &strError);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        DT_GVG_ARENA_DATA stArena;

        while(pRecordSet->GetRecord())
        {
            memset(&stArena, 0, sizeof(stArena));

            stArena.wArenaID = SDAtou(pRecordSet->GetFieldValueByName("ArenaID"));
            if(0 == stArena.wArenaID)
            {
                continue;
            }

            stArena.byCurStage = SDAtou(pRecordSet->GetFieldValueByName("CurStage"));

            DecodeSaveDataFromDB("ZoneListData", stArena.stZoneIDList);
            DecodeSaveDataFromDB("GroupZoneIDList", stArena.stGroupZoneIDList);
            DecodeSaveDataFromDB("War16To8To4Data", stArena.stWar16To8List);
            DecodeSaveDataFromDB("FinalWarData", stArena.stFinalWarInfo);
            DecodeSaveDataFromDB("FactionsGvGLogData", stArena.stFactionsLog);
            DecodeSaveDataFromDB("StageDoneFlagList", stArena.stStageDoneFlagList);

            if(FALSE == IsArenaOpenUp(stArena.wArenaID))
            {
                USR_INFO(_SDT("[%s: %d]: CGvGMgr::LoadGvGArenaDB skip load arena %u : not all zone open up gvg activity!"), MSG_MARK, stArena.wArenaID);
                CGvGPropMgr::Instance()->PrintNotOpenUpArenaZone(stArena.wArenaID);
                continue;
            }

            // 建立起一个跨服门派竞技场
            CGvGArena *poArena = NewArena(stArena.wArenaID);
            if(NULL == poArena)   
            {
                return FALSE;
            }

            poArena->Init(stArena);
        }
    }

    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    return TRUE;
}

void CGvGMgr::GlobalGvGToLocalGvG(DT_GVG_LOCAL_ACTIVITY_DATA &stLocal, const DT_GVG_ARENA_DATA &stArena)
{
    memset(&stLocal, 0, sizeof(stLocal));

    stLocal.byCurStage  = stArena.byCurStage;
    stLocal.byDone      = m_stActData.byDone;
    stLocal.qwStartTime = m_stActData.qwStartTime;
    stLocal.stArena     = stArena;
    stLocal.wSessionID  = m_stActData.wSessionID;
}

BOOL CGvGMgr::RecoverProgress()
{
    SetStage((GVG_STAGE)m_stActData.byCurStage);
    BuildZoneIDToArenaIDMap();

    ReEnterCurStage();

    CGvGPropMgr::Instance()->FakeGvGTime(m_stActData.qwStartTime, m_eStage);
    return TRUE;
}

void CGvGMgr::TeamVsTeam(CPlayerVec &oTeam1, CPlayerVec &oTeam2, SGvGVideo &stVideo)
{
    stVideo.stGvGInfo.qwTime = SDTimeSecs();
    BYTE i1 = 0;
    BYTE i2 = 0;

    DT_BATTLE_OBJ_GROUP_HP stInitHP1;
    DT_BATTLE_OBJ_GROUP_HP stInitHP2;
    DT_BATTLE_OBJ_GROUP_HP stAfterHP1;
    DT_BATTLE_OBJ_GROUP_HP stAfterHP2;

    DT_BATTLE_OBJ_GROUP_HP *pstInitHP1 = NULL;
    DT_BATTLE_OBJ_GROUP_HP *pstInitHP2 = NULL;

    DT_GVG_VIDEO &stGvGInfo = stVideo.stGvGInfo;
    stGvGInfo.byBattleResult = GR_NOT_START;

    CPlayer *poPlayer1 = NULL;
    CPlayer *poPlayer2 = NULL;

    if(oTeam2.empty())
    {
        stGvGInfo.byBattleResult = GR_LEFT_WIN;
        return;
    }
    else if(oTeam1.empty())
    {
        stGvGInfo.byBattleResult = GR_RIGHT_WIN;        
        return;
    }

    BOOL bRecoverHP1 = FALSE; // 用于标识上一场左方是否回血
    BOOL bRecoverHP2 = FALSE; // 用于标识上一场右方是否回血

    while(TRUE)
    {
        if(i1 >= oTeam1.size() || i2 >= oTeam2.size())
        {
            break;
        }

        if(stVideo.vecPvPVideo.size() >= MAX_GVG_PK_VIDEO_CNT)
        {
            break;
        }

        stVideo.vecPvPVideo.resize(i1 + i2 + 1);
        DT_GVG_PK_VIDEO &stPKVideo = stVideo.vecPvPVideo[i1 + i2];
        memset(&stPKVideo, 0, sizeof(stPKVideo));

        poPlayer1 = oTeam1[i1];
        poPlayer2 = oTeam2[i2];

        poPlayer1->InitBattleAttr(TRUE);
        poPlayer1->InitBattleAttr(TRUE);

        poPlayer1->InitCurBattleAttr();
        poPlayer2->InitCurBattleAttr();

        stPKVideo.byPlayerIdx1 = i1;
        stPKVideo.byPlayerIdx2 = i2;

        gvgutil::GetEachObjMaxHP(stPKVideo.stPKVideo.stMyselfMaxHPInfo, poPlayer1);
        gvgutil::GetEachObjMaxHP(stPKVideo.stPKVideo.stEnemyMaxHPInfo,  poPlayer2);

        gvgutil::CleanMaxHPOfDeadHero(stPKVideo.stPKVideo.stMyselfMaxHPInfo, pstInitHP1);
        gvgutil::CleanMaxHPOfDeadHero(stPKVideo.stPKVideo.stEnemyMaxHPInfo,  pstInitHP2);

        if(bRecoverHP1)
        {
            gvgutil::CopyHp(stPKVideo.stPKVideo.stMyselfBeforeRecoverHPInfo, stAfterHP1);
        }
        if (bRecoverHP2)
        {
            gvgutil::CopyHp(stPKVideo.stPKVideo.stEnemyBeforeRecoverHPInfo, stAfterHP2);
        }

        DT_GVG_FORMATION_PLAYER_INFO &stPlayer1 = stGvGInfo.stG1.stPlayerList.astPlayerList[i1];
        DT_GVG_FORMATION_PLAYER_INFO &stPlayer2 = stGvGInfo.stG2.stPlayerList.astPlayerList[i2];

        UINT8 wWeakPercent1 = CGvGPropMgr::Instance()->GetWeakPercentByKillCombo(stPlayer1.byKillCombo);
        UINT8 wWeakPercent2 = CGvGPropMgr::Instance()->GetWeakPercentByKillCombo(stPlayer2.byKillCombo);

        CBattleMgr::Instance()->Battle4(poPlayer1, poPlayer2, stPKVideo.stPKVideo.stEmptyHPVideo, TRUE, FALSE, &stAfterHP1, &stAfterHP2, pstInitHP1, pstInitHP2, wWeakPercent1, wWeakPercent2);

        USR_INFO( _SDT("[CGvGMgr::%s: %d] video[%llu], player1[%u:%s] vs player2[%u:%s], %s!"), 
            MSG_MARK, stVideo.stGvGInfo.qwVideoID, 
            poPlayer1->GetID(), poPlayer1->GetDT_PLAYER_BASE_DATA().aszDispName, poPlayer2->GetID(), poPlayer2->GetDT_PLAYER_BASE_DATA().aszDispName, 
            (0 == stPKVideo.stPKVideo.stEmptyHPVideo.byBattleResult ? "win" : "lose")
        );

        BOOL bErr = (0 == stPKVideo.stPKVideo.stMyselfMaxHPInfo.byObjNum || 0 == stPKVideo.stPKVideo.stEnemyMaxHPInfo.byObjNum);
        if(bErr)
        {
            SYS_CRITICAL( _SDT("[%s: %d] CGvGMgr::PlayersVsPlayers zone1[%u]->faction1[%u], zone2[%u]->faction2[%u], player1[%u] objnum = %u, player2[%u] objnum = %u, error!"), 
                MSG_MARK, 
                stGvGInfo.stG1.wZoneID, stGvGInfo.stG2.wZoneID,
                stGvGInfo.stG1.dwFactionID, stGvGInfo.stG2.dwFactionID,
                poPlayer1->GetDT_PLAYER_BASE_DATA().dwID, stPKVideo.stPKVideo.stMyselfMaxHPInfo.byObjNum, poPlayer2->GetDT_PLAYER_BASE_DATA().dwID, stPKVideo.stPKVideo.stEnemyMaxHPInfo.byObjNum);
        }

        stInitHP1 = stAfterHP1;
        stInitHP2 = stAfterHP2;

        if(stPKVideo.stPKVideo.stEmptyHPVideo.byBattleResult == EBT_SUCCESS)
        {
            stGvGInfo.byBattleResult = GR_LEFT_WIN;
            pstInitHP1 = &stInitHP1;
            pstInitHP2 = NULL;

            stPlayer1.byKillCombo++;

            stPKVideo.byKillCombo1 = stPlayer1.byKillCombo;
            i2++;

            bRecoverHP1 = gvgutil::RecoverHP(stPKVideo.stPKVideo.stMyselfMaxHPInfo, stInitHP1, stPlayer1.byKillCombo);
        }
        else
        {
            stGvGInfo.byBattleResult = GR_RIGHT_WIN;
            pstInitHP1 = NULL;
            pstInitHP2 = &stInitHP2;

            stPlayer2.byKillCombo++;
            
            stPKVideo.byKillCombo2 = stPlayer2.byKillCombo;
            i1++;

            bRecoverHP2 = gvgutil::RecoverHP(stPKVideo.stPKVideo.stEnemyMaxHPInfo, stInitHP2, stPlayer2.byKillCombo);
        }
    }

    if(NULL == poPlayer1 || NULL == poPlayer2)
    {
        return;
    }

    stVideo.stGvGInfo.byPKVideoCnt = stVideo.vecPvPVideo.size();

    DT_FINAL_ROUND_BATTLE_OBJ_INFO &stFinalRoundInfo = stGvGInfo.stFinalRoundInfo;
    
    poPlayer1->InitCurBattleAttr(&stAfterHP1);
    poPlayer2->InitCurBattleAttr(&stAfterHP2);

    poPlayer1->GetDT_BATTLE_OBJ_GROUP_DATA(stFinalRoundInfo.stMyselfObj);
    poPlayer2->GetDT_BATTLE_OBJ_GROUP_DATA(stFinalRoundInfo.stEnemyObj);

    DT_GVG_PK_VIDEO &stFinalPKVideo = stVideo.vecPvPVideo.back();
    gvgutil::CopyHPGridNum(stFinalRoundInfo.stMyselfObj, stFinalPKVideo.stPKVideo.stEmptyHPVideo.stMyselfInfo);
    gvgutil::CopyHPGridNum(stFinalRoundInfo.stEnemyObj, stFinalPKVideo.stPKVideo.stEmptyHPVideo.stEnemyInfo);
}

SGvGVideo& CGvGMgr::GvG(CFaction *g1, CFaction *g2)
{   
    static SGvGVideo s_stVideo;
    s_stVideo.Clear();

    if(NULL == g1 && NULL == g2)
    {
        return s_stVideo;
    }

    s_stVideo.stGvGInfo.qwVideoID = ((g1 && g2) ? CGvGVideoMgr::Instance()->MakeVideoID(g1->GetFactionID(), g2->GetFactionID()) : 0);

    gvgutil::SaveFactionInfo(s_stVideo.stGvGInfo.stG1, g1);
    gvgutil::SaveFactionInfo(s_stVideo.stGvGInfo.stG2, g2);

    CPlayerVec vecPlayer1;
    CPlayerVec vecPlayer2;
    if(g1)
    {
        vecPlayer1 = g1->GetFormationPlayers();
    }
    if(g2)
    {
        vecPlayer2 = g2->GetFormationPlayers();
    }   

    if(NULL == g1)
    {
        s_stVideo.stGvGInfo.byBattleResult = GR_RIGHT_WIN;
    }
    else if(NULL == g2)
    {
        s_stVideo.stGvGInfo.byBattleResult = GR_LEFT_WIN;
    }
    else
    {
        TeamVsTeam(vecPlayer1, vecPlayer2, s_stVideo);
    }

    return s_stVideo;
}

SGvGVideo& CGvGMgr::GvG(CPKFaction *g1, CPKFaction *g2)
{   
    static SGvGVideo s_stVideo;
    s_stVideo.Clear();

    if(NULL == g1 && NULL == g2)
    {
        return s_stVideo;
    }

    s_stVideo.stGvGInfo.qwVideoID = ((g1 && g2) ? CGvGVideoMgr::Instance()->MakeVideoID(g1->GetFactionID(), g2->GetFactionID()) : 0);

    gvgutil::SaveFactionInfo(s_stVideo.stGvGInfo.stG1, g1);
    gvgutil::SaveFactionInfo(s_stVideo.stGvGInfo.stG2, g2);

    CPlayerVec vecPlayer1;
    CPlayerVec vecPlayer2;
    if(g1)
    {
        vecPlayer1 = g1->GetFormationPlayers();
    }
    if(g2)
    {
        vecPlayer2 = g2->GetFormationPlayers();
    }

    if(NULL == g1)
    {
        s_stVideo.stGvGInfo.byBattleResult = GR_RIGHT_WIN;
    }
    else if(NULL == g2)
    {
        s_stVideo.stGvGInfo.byBattleResult = GR_LEFT_WIN;
    }
    else
    {
        TeamVsTeam(vecPlayer1, vecPlayer2, s_stVideo);
    }

    return s_stVideo;
}

void CGvGMgr::OnArenaReadyGo(CGvGPullGroup *poPullGroup)
{
    // 找到对应赛场
    CGvGArena *poArena = GetArena(poPullGroup->m_arenaID);
    if(NULL == poArena)
    {
        return;
    }

    poArena->AddPlayers(*poPullGroup);
    poArena->Go();
}

void CGvGMgr::HandleRetTopFactions(ZoneID zoneID, PKT_GSPK_GVG_RET_TOP_8_ACK &ack)
{
    CGvGArena *poArena = GetArenaByZoneID(zoneID);
    if(NULL == poArena)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::HandleRetTopFactions GetArena failed, zoneID = %u!"), MSG_MARK, zoneID);
        return;
    }

    poArena->OnRecvTopFaction(zoneID, ack.astTop8List, ack.byTop8Cnt);
}

// 成功接收到xxx区的游戏服返回的参战门派数据
void CGvGMgr::HandleRetFactions(ZoneID zoneID, PKT_GSPK_RET_FACTIONS_ACK &stAck)
{
    if(0 == stAck.stRetFactions.byFactionCnt)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::HandleRetFactions return factions count = 0, zoneID = %u!"), MSG_MARK, zoneID);
        return;
    }

    CGvGArena *poArena = GetArenaByZoneID(zoneID);
    if(NULL == poArena)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::HandleRetFactions GetArena failed, zoneID = %u!"), MSG_MARK, zoneID);
        return;
    }

    poArena->OnRecvFaction(zoneID, stAck.stRetFactions);
}

void CGvGMgr::OnZoneSayReady(ZoneID zoneID, PKT_GSPK_GVG_REPLY_READY_ACK &stAck)
{
    CGvGArena *poArena = GetArenaByZoneID(zoneID);
    if(NULL == poArena)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::OnZoneSayReady GetArena failed, zoneID = %u!"), MSG_MARK, zoneID);
        return;
    }
    
    poArena->OnZoneSayReady(zoneID, stAck);
}

CGvGArena* CGvGMgr::NewArena(ArenaID arenaID)
{
    if(0 == arenaID)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::NewArena failed, invalid arenaID = %u!"), MSG_MARK, arenaID);
        return NULL;
    }

    // 这里不用对象池
    CGvGArena *poArena = new CGvGArena;
    if(NULL == poArena)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::NewArena failed, not enough memory, arenaID = %u!"), MSG_MARK, arenaID);
        return NULL;
    }

    DT_GVG_ARENA_DATA &stArena = poArena->GetArenaData();
    stArena.wArenaID = arenaID;

    m_vecArena.resize(arenaID);
    m_vecArena[arenaID - 1] = poArena;

    return poArena;
}

void CGvGMgr::SupportFaction(GvGRound eRound, ArenaID arenaID, DT_FACTION_SUPPORT_LIST &stFactionList)
{
    CGvGArena *poArena = GetArena(arenaID);
    if(NULL == poArena)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::SupportFaction failed, invalid arenaID = %u!"), MSG_MARK, arenaID);
        return;
    }

    poArena->SupportFaction(eRound, stFactionList);
}

void CGvGMgr::CheckOpenUp()
{
    if (0 == m_stActData.qwStartTime)
    {
        return;
    }

    if (m_stActData.wSessionID > 0 && !CGvGPropMgr::Instance()->GetGvGProp().bOpenActivity)
    {
        DBG_INFO(_SDT("[CGvGMgr::CheckOpenUp]: detecting activity is set to closed, now session = %u!"), m_stActData.wSessionID);
        memset(&m_stActData, 0, sizeof(m_stActData));

        SetStage(GVG_0_NOT_START);
        ClearActivity();

        SaveToDB();
        return;
    }

    for(UINT16 i = 0; i < m_vecArena.size(); i++)
    {
        CGvGArena *poArena = m_vecArena[i];
        if (NULL == poArena)
        {
            continue;
        }

        UINT16 wUnLockSession = CGvGPropMgr::Instance()->GetUnLockSession(poArena->GetArenaID());
        BOOL bLock = (wUnLockSession > m_stActData.wSessionID);
        if (bLock)
        {
            DBG_INFO(_SDT("[CGvGMgr::CheckOpenUp]: detecting arena<%u> is set to closed, now session = %u, arena is set to waiting session %u!"), 
                poArena->GetArenaID(), m_stActData.wSessionID, wUnLockSession);

            poArena->Clear();
            delete poArena;
            m_vecArena[i] = NULL;
        }
    }
}

BOOL CGvGMgr::IsDone(GVG_STAGE eStage)
{
    BOOL bDone = TRUE;
    switch(eStage)
    {
    case GVG_1_APPLY:
    {
        bDone  = (m_stActData.wSessionID > 0);
        bDone &= CGvGVideoMgr::Instance()->IsEmpty();
        bDone &= (!m_vecArena.empty());

        // ZoneIDSet setZoneID = GetArenaZoneIDList();
        // bDone &= (m_mapOpenReply.size() == setZoneID.size());
        bDone &= IsAllArenaDone(eStage);
        break;
    }
    case GVG_7_END:
    case GVG_8_SLEEPING:
        bDone = IsAllArenaDone(eStage);
        break;
    }

    return bDone;
}

BOOL CGvGMgr::IsAllArenaDone(GVG_STAGE eStage)
{
    for(UINT16 i = 0; i < m_vecArena.size(); ++i)
    {
        CGvGArena *poArena = m_vecArena[i];
        if (NULL == poArena)
        {
            continue;
        }

        if(!poArena->IsStageDone(eStage) || !poArena->IsLogicDone(eStage))
        {
            return FALSE;
        }
    }

    return TRUE;
}

void CGvGMgr::ClearArena()
{
    for(UINT16 i = 0; i < m_vecArena.size(); i++)
    {
        CGvGArena *poArena = m_vecArena[i];
        if (poArena)
        {
            poArena->Clear();
            delete poArena;
        }
    }

    m_vecArena.clear();
}

ZoneIDSet CGvGMgr::GetArenaZoneIDList()
{
    ZoneIDSet setZoneID;

    CGvGPropMgr::ArenaZoneIDMap &mapArenaZoneID = CGvGPropMgr::Instance()->GetArenaZoneIDMap();
    for(CGvGPropMgr::ArenaZoneIDMap::iterator itr = mapArenaZoneID.begin(); itr != mapArenaZoneID.end(); ++itr)    
    {
        ArenaID arenaID                = itr->first;
        GroupZoneIDMap &mapGroupZoneID = itr->second;

        if(!IsArenaOpenUp(arenaID))
        {
            continue;
        }

        for(GroupZoneIDMap::iterator groupItr = mapGroupZoneID.begin(); groupItr != mapGroupZoneID.end(); ++groupItr)
        {
            ZoneIDVec &vecZoneID = groupItr->second;
            setZoneID.insert(vecZoneID.begin(), vecZoneID.end());
        }
    }

    setZoneID.erase(0);
    return setZoneID;
}

BOOL CGvGMgr::IsAllZoneReplyCanOpen(ArenaID arenaID)
{
    CGvGPropMgr::ArenaZoneIDMap &mapArenaZoneID = CGvGPropMgr::Instance()->GetArenaZoneIDMap();

    CGvGPropMgr::ArenaZoneIDMap::iterator itr = mapArenaZoneID.find(arenaID);
    if(itr == mapArenaZoneID.end())
    {
        return FALSE;
    }

    GroupZoneIDMap &mapGroupZoneID = itr->second;
    for (GroupZoneIDMap::iterator itr = mapGroupZoneID.begin(); itr != mapGroupZoneID.end(); ++itr)
    {
        ZoneIDVec &vecZoneID = itr->second;
        for(UINT16 i = 0; i < vecZoneID.size(); i++)
        {
            ZoneID zoneID = vecZoneID[i];
            if(m_mapOpenReply.find(zoneID) == m_mapOpenReply.end())
            {
                return FALSE;
            }

            if(FALSE == m_mapOpenReply[zoneID])
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL CGvGMgr::IsArenaOpenUp(ArenaID arenaID)
{
    UINT16 wUnLockSession = CGvGPropMgr::Instance()->GetUnLockSession(arenaID);
    if (0 == wUnLockSession || m_stActData.wSessionID < wUnLockSession)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CGvGMgr::BuildEmptyArena()
{
    if(IsDone(GVG_1_APPLY))
    {
        return TRUE;
    }

    m_stActData.byArenaCnt = 0;

    CGvGPropMgr::ArenaZoneIDMap &mapArenaZoneID = CGvGPropMgr::Instance()->GetArenaZoneIDMap();
    for(CGvGPropMgr::ArenaZoneIDMap::iterator itr = mapArenaZoneID.begin(); itr != mapArenaZoneID.end(); ++itr)    
    {
        ArenaID arenaID                = itr->first;
        GroupZoneIDMap &mapGroupZoneID = itr->second;

        if(0 == arenaID)
        {
            continue;
        }

        if (!IsArenaOpenUp(arenaID))
        {
            UINT16 wUnLockSession = CGvGPropMgr::Instance()->GetUnLockSession(arenaID);
            USR_INFO(_SDT("[%s: %d]: CGvGMgr::BuildEmptyArena skip build arena %u : the arena are waiting session %u, now session = %u!"), MSG_MARK, arenaID, wUnLockSession, m_stActData.wSessionID);
            continue;
        }
        
        /*
        if(!CGvGPropMgr::Instance()->IsArenaOpenUp(arenaID))
        {
            UINT64 qwOpenDate = CGvGPropMgr::Instance()->GetOpenDateByArena(arenaID);
            CSDDateTime oOpenDate(qwOpenDate);            

            USR_INFO(_SDT("[%s: %d]: CGvGMgr::BuildEmptyArena skip build arena %u : the arena has not reach the date<%s> to unlock gvg activity!"), MSG_MARK, arenaID, SDTimeToString(oOpenDate).c_str());
            // CGvGPropMgr::Instance()->PrintNotOpenUpArenaZone(arenaID);
            continue;
        }
        */

        if(GetArena(arenaID))
        {
            continue;
        }

        // 建立起一个跨服门派竞技场
        CGvGArena *poArena = NewArena(arenaID);
        if(NULL == poArena)   
        {
            return FALSE;
        }

        poArena->InitZoneList(mapGroupZoneID);
        poArena->SetStageDone(GVG_0_NOT_START);
    }

    m_stActData.byArenaCnt = m_vecArena.size();
    if(m_vecArena.empty())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::BuildEmptyArena m_vecArena.empty = true!"), MSG_MARK);
        return FALSE;
    }

    BuildZoneIDToArenaIDMap();
    return TRUE;
}

void CGvGMgr::BuildZoneIDToArenaIDMap()
{
    m_mapZoneToArena.clear();

    for(UINT16 i = 0; i < m_vecArena.size(); ++i)
    {
        CGvGArena *poArena = m_vecArena[i];
        if (NULL == poArena)
        {
            continue;
        }

        DT_GVG_ARENA_DATA &stArena = poArena->GetArenaData();

        for(UINT16 wZoneIdx = 0; wZoneIdx < stArena.stZoneIDList.byZoneCnt; wZoneIdx++)
        {
            ZoneID zoneID = stArena.stZoneIDList.awZoneIDList[wZoneIdx];
            m_mapZoneToArena[zoneID] = stArena.wArenaID;
        }
    }
}

CGvGArena* CGvGMgr::GetArena(ArenaID arenaID)
{
    if(0 == arenaID || arenaID > m_vecArena.size())
    {
        return NULL;
    }

    return m_vecArena[arenaID - 1];
}

ArenaID CGvGMgr::GetArenaIDByZoneID(ZoneID zoneID)
{
    return m_mapZoneToArena[zoneID];
}

CGvGArena* CGvGMgr::GetArenaByZoneID(ZoneID zoneID)
{
    ArenaID arenaID = GetArenaIDByZoneID(zoneID);
    if(0 == arenaID)
    {
        // SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::GetArenaByZoneID failed, zoneID = %u, arenaID = 0!"), MSG_MARK, zoneID);
        return NULL;
    }

    CGvGArena *poArena = GetArena(arenaID);
    return poArena;
}
