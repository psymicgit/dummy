///<------------------------------------------------------------------------------
//< @file:   logic\gvg\gvgbuild.cpp
//< @author: 
//< @date:   2014年1月18日 17:19:11
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "gvgbuild.h"

#include <commondef.h>
#include <net/pk/pkpktbuilder.h>
#include "framework/gsapi.h"
#include "net/db/dbpktbuilder.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/gvg/gvgdef.h"
#include "logic/race/racemgr.h"
#include "logic/battle/battlemgr.h"
#include "logic/faction/factionmgr.h"
#include "logic/gvg/local/gvgfactionmgr.h"
#include "logic/other/singleprammgr.h"
#include "logic/gvg/gvgvideomgr.h"
#include "logic/gvg/local/gvggiftmgr.h"
#include "logic/gvg/playergvg.h"
#include "logic/gvg/global/gvgmgr.h"
#include "logic/record/recordmgr.h"
#include "logic/player/playersnapshotmgr.h"
#include "db/dbcommoperate.h"
#include "logic/record/recordmgr.h"
#include "logic/event/timereventmgr.h"
#include "logic/other/errmsgmgr.h"
#include "net/cli/usermgr.h"
#include "logic/medal/playermedal.h"
#include "framework/gsapplication.h"

typedef std::vector<DT_BATTLE_DATA3> PvPVideoVec;
static PvPVideoVec vecPvPVideo;

#define MAX_SYNC_SUPPORT_FACTION_CNT 30
#define RUN_ACTIVITY_INTERVAL 10

namespace gvgutil
{
    // 取出门派信息用于显示门派战况面板中左侧的战旗
    void SaveFactionBaseToFlag(DT_GVG_LOG_FACTION_INFO &stFlag, const DT_FACTION_BASE_DATA_CLI2 &stBaseCli)
    {
        SDStrncpy(stFlag.aszFactionName, stBaseCli.aszFactionName, MAX_FACTION_NUM);
        SDStrncpy(stFlag.aszHeadName, stBaseCli.aszHeadName, USERNAME_LEN);

        stFlag.dwFactionID    = stBaseCli.dwFactionID;
        stFlag.wFactionIconID = stBaseCli.byIconID;
        stFlag.wFactionLevel  = stBaseCli.dwFactionLevel;
    }

    DT_FACTION_GVG_SINGLE_LOG* FindGvGLog(DT_FACTION_GVG_LOG_LIST &stLogList, DT_GVG_ROUND &stRound)
    {
        for (UINT16 i = 0; i < stLogList.wLogCnt; i++)
        {
            DT_FACTION_GVG_SINGLE_LOG &stLog = stLogList.astLogs[i];
            if(stLog.stRound.byBigRound == stRound.byBigRound && stLog.stRound.bySmallRound == stRound.bySmallRound)
            {
                return &stLog;
            }
        }

        return NULL;
    }

    GvGRound Stage2Round(GVG_STAGE eStage)
    {
        static std::vector<GvGRound> vecStageToRound;
        if(vecStageToRound.empty())
        {
            vecStageToRound.resize(GVG_STAGE_MAX + 1);

            vecStageToRound[GVG_0_NOT_START]       = GVG_SLEEPING;
            vecStageToRound[GVG_1_APPLY]           = GVG_APPLY;
            vecStageToRound[GVG_2_MONEY]           = GVG_MONEY_WAR;

            vecStageToRound[GVG_3_CHAOS_WAR_BEGIN] = GVG_CHAOS_WAR;
            vecStageToRound[GVG_3_CHAOS_WAR_END]   = GVG_CHAOS_WAR;

            vecStageToRound[GVG_4_16_TO_8_BEGIN]   = GVG_16_TO_8;
            vecStageToRound[GVG_4_16_TO_8_GO]      = GVG_16_TO_8;
            vecStageToRound[GVG_4_16_TO_8_END]     = GVG_16_TO_8;

            vecStageToRound[GVG_5_8_TO_4_BEGIN]    = GVG_8_TO_4;
            vecStageToRound[GVG_5_8_TO_4_GO]       = GVG_8_TO_4;
            vecStageToRound[GVG_5_8_TO_4_END]      = GVG_8_TO_4;

            vecStageToRound[GVG_6_FINAL_WAR_BEGIN] = GVG_FINAL_WAR;
            vecStageToRound[GVG_6_FINAL_WAR_GO]    = GVG_FINAL_WAR;

            vecStageToRound[GVG_7_END]       = GVG_END;
            vecStageToRound[GVG_8_SLEEPING]        = GVG_SLEEPING;

            vecStageToRound[GVG_STAGE_MAX]         = GVG_END;
        }

        return vecStageToRound[eStage];
    }

    // 取出录像中的门派信息用于显示
    void CopyVideoFactionInfo(DT_GVG_VIDEO_FACTION_INFO &stFactionCli, const DT_GVG_VIDEO_FACTION &stVideoFaction)
    {
        SDStrncpy(stFactionCli.aszFactionName, stVideoFaction.aszFactionName, MAX_FACTION_NUM - 1);

        const std::string& strZoneName = CGvGPropMgr::Instance()->GetZoneName(stVideoFaction.wZoneID);
        SDStrncpy(stFactionCli.aszZoneName, strZoneName.c_str(), ZONENAME_LEN - 1);
        stFactionCli.byFactionIconID = stVideoFaction.byFactionIconID;
    }

    // 获取连斩达人列表用于显示
    void GetMVPList(DT_GVG_MVP_LIST &stMVPList, const DT_GVG_FORMATION_PLAYER_LIST &stPlayerList, UINT8 byWinOrLose, UINT8 byFactionIconID, GvGRound eBigRound)
    {
        for(UINT16 i = 0; i < stPlayerList.byPlayerCnt; i++)
        {
            const DT_GVG_FORMATION_PLAYER_INFO &stPlayerInfo = stPlayerList.astPlayerList[i];
            if(stMVPList.byMVPCnt >= MAX_GVG_MVP_CNT)
            {
                break;
            }

            DT_GVG_MVP &stMVP = stMVPList.astMVPList[stMVPList.byMVPCnt++];
            
            SDStrncpy(stMVP.aszPlayerName, stPlayerInfo.aszPlayerName, USERNAME_LEN - 1);
            stMVP.byKillCombo = stPlayerInfo.byKillCombo;
            stMVP.byWinOrLose = byWinOrLose;
            stMVP.byScore = (UINT8)CGvGPropMgr::Instance()->CalcScoreByKillCombo(eBigRound, stMVP.byKillCombo);
            stMVP.dwAward = CGvGPropMgr::Instance()->GetContinuKillDoorsTribute(stMVP.byKillCombo);
            stMVP.byFactionIconID = byFactionIconID;
        }
    }

    // 根据玩家表现由好到差进行排序
    bool SortMVPByScoreDesc(const DT_GVG_MVP &stMVP1, const DT_GVG_MVP &stMVP2)
    {
        if(stMVP1.byKillCombo > stMVP2.byKillCombo)
        {
            return true;
        }
        else if(stMVP1.byKillCombo < stMVP2.byKillCombo)
        {
            return false;
        }
        else
        {
            return (stMVP1.byWinOrLose > stMVP2.byWinOrLose);
        }
        
        return false;
    }

    // 取出总决赛中的门派信息用于显示
    void TakeDisplayFinalWarFaction(DT_FINAL_WAR_FACTION_INFO_CLI &stFactionCli, const DT_GVG_FACTION &stFaction)
    {
        SDStrncpy(stFactionCli.aszFactionName, stFaction.stBaseData.aszFactionName, MAX_FACTION_NUM - 1);
        
        const std::string& strZoneName = CGvGPropMgr::Instance()->GetZoneName(stFaction.wOriZoneID);
        SDStrncpy(stFactionCli.aszZoneName, strZoneName.c_str(), ZONENAME_LEN - 1);

        stFactionCli.dwFactionID = stFaction.stBaseData.dwFactionID;
        stFactionCli.dwZoneID = stFaction.wZoneID;
        stFactionCli.qwSumPower = stFaction.qwSumPower;
        stFactionCli.wFactionIconID = stFaction.stBaseData.byIconID;
        stFactionCli.wFactionLevel = stFaction.stBaseData.dwFactionLevel;
        stFactionCli.wJoinNum = stFaction.byJoinCnt;
    }

    GVG_STAGE GetNextStage(GVG_STAGE eStage)
    {
        GVG_STAGE eNextStage = (GVG_8_SLEEPING <= eStage ? GVG_1_APPLY : (GVG_STAGE)(eStage + 1));
        return eNextStage;
    }

    UINT64 GetTodayZero()
    {
        UINT64 qwNow = SDNow().GetTimeValue();
        UINT64 qwTodayZero = qwNow - GetDayPassSecond(qwNow);

        return qwTodayZero;
    }

    std::string TimeToDayHourMinuteSec(UINT32 dwTime)
    {
        UINT8 byDay = dwTime / 86400;
        UINT8 byHour = (dwTime % 86400) / 3600;
        UINT8 byMinute = (dwTime % 3600) / 60;
        UINT8 bySec = (dwTime % 60);

        TCHAR temp[60];
        _SDTsnprintf(temp, 60, _SDT("%02dday %02d:%02d:%02d"), byDay, byHour, byMinute, bySec);
        return temp;
    }    
}

IMPLEMENT_SINGLETON(CGvGBuild)
bool SortByMoney(DT_FACTION_APPLAY_DATA& stData1, DT_FACTION_APPLAY_DATA& stData2)
{
	CFaction* poFaction1 = CFactionMgr::Instance()->GetFaction(stData1.dwFactionID);
	CFaction* poFaction2 = CFactionMgr::Instance()->GetFaction(stData2.dwFactionID);
	if ( NULL == poFaction1)
	{
		return false;
	}
	if ( NULL == poFaction2)
	{
		return true;
	}
	if( poFaction1->GetFactionGVGFunds() < poFaction2->GetFactionGVGFunds())
	{
		return false;
	}
	else if(poFaction1->GetFactionGVGFunds() == poFaction2->GetFactionGVGFunds())
	{
		if ( stData1.qwApplayTime < stData2.qwApplayTime)
		{
			return true;
		}
	}
	else
	{
		return true;
	}

	return false;
}

VOID CGvGBuild::Run()
{
    if(!IsOpenUp())
    {
        return;
    }

    UINT64 qwNow = SGDP::SDTimeSecs();

    // 上一次运行门派跨服战活动的时间
    static UINT64 qwLastRunActivityTime = qwNow;
    static UINT64 qwLastSaveTime = qwNow;

	GVG_STAGE eStage = GetCurStage();//非活动期间不需要保存
    if(GVG_0_NOT_START == eStage || eStage >= GVG_7_END)
    {
        return;
    }

	if(qwLastSaveTime < (qwNow - SAVE_GVG_BUILD_INTERVAL))
	{
		SaveToDB();
        qwLastSaveTime = qwNow;
	}
    
    if(qwLastRunActivityTime < (qwNow - RUN_ACTIVITY_INTERVAL))
    {
        if(!IsReadyByStage(m_eStage))
        {
            ReEnterStage();
            SendGetGvGActivityReq();

            if(GVG_3_CHAOS_WAR_BEGIN == m_eStage)
            {
                ChaosWarGo();
            }
        }
        else
        {
            if(GVG_3_CHAOS_WAR_BEGIN == m_eStage)
            {
                GiveChaosWarAward();
                CGvGGiftMgr::Instance()->AddGiftTODB();
            }
            else if(GVG_6_FINAL_WAR_GO == m_eStage)
            {
                GiveFinalWarAward();
                CGvGGiftMgr::Instance()->AddGiftTODB();
            }
        }

        TakeThisZoneVideo();
        SyncFactionSupport();
        qwLastRunActivityTime = qwNow;
    }
}

void CGvGBuild::GiveMoneyWarAward()
{
    if (CGvGPropMgr::Instance()->IsTest())
    {
        return;
    }

    DT_SIMPLE_FACTION_LST_INFO stMoneyResult;
    GetMoneyWarResult(stMoneyResult);

    for (BYTE byRank = 1; byRank <= stMoneyResult.wFactionNum; byRank++)
    {
        DT_SIMPLE_FACTION_INFO &stFactionInfo = stMoneyResult.astFactionInfo[byRank - 1];
        CGvGGiftMgr::Instance()->AddRankReward(stFactionInfo.stFactionBaseInfo.dwFactionID, GVG_MONEY_WAR, byRank);
    }
}

BOOL CGvGBuild::IsAllVideoHere()
{
    if(m_eStage < GVG_4_16_TO_8_GO)
    {
        return TRUE;
    }

    switch(m_eStage)
    {
    case GVG_4_16_TO_8_END:
    case GVG_4_16_TO_8_GO:
    case GVG_5_8_TO_4_GO:
    case GVG_5_8_TO_4_END:
    case GVG_6_FINAL_WAR_GO:
        break;

    default:
        return TRUE;
    }

    VideoIDSet setVideoID;
    TakeRequiredVideoIDList(setVideoID);

    for(VideoIDSet::iterator itr = setVideoID.begin(); itr != setVideoID.end(); ++itr)
    {
        VideoID videoID = *itr;

        SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(videoID);
        if (NULL == pstVideo)
        {
            return FALSE;
        }

        if (!pstVideo->IsAllPKVideoHere())
        {
            return FALSE;
        }
    }

    return TRUE;
}

void CGvGBuild::TakeThisZoneVideo()
{
    if(IsAllVideoHere())
    {
        return;
    }

    VideoIDSet setRequiredVideoID;
    TakeRequiredVideoIDList(setRequiredVideoID);

    if (setRequiredVideoID.empty())
    {
        return;
    }

    UINT16 wHereVideoCnt = CGvGVideoMgr::Instance()->CalcHereVideoCnt(setRequiredVideoID);
    DBG_INFO(_SDT("[%s: %d]: CGvGBuild::TakeThisZoneVideo, not all video is here, required video count = %u, current here video count = %u, m_eStage = %u"), MSG_MARK, setRequiredVideoID.size(), wHereVideoCnt, m_eStage);    

    static UINT64 qwLastReqTime = 0;
    UINT64 qwNow = SDTimeSecs();

    if(qwNow - qwLastReqTime > 10)
    {
        PKT_GSPK_GET_VIDEO_REQ stReq;
        memset(&stReq, 0, sizeof(stReq));

        gsapi::SendMsg2PK((CHAR*)&stReq, GSPK_GET_VIDEO_REQ);
        qwLastReqTime = qwNow;

        DBG_INFO(_SDT("[%s: %d]: CGvGBuild::TakeThisZoneVideo, send get video request, stage = %u"), MSG_MARK, m_eStage);
    }    
}

void CGvGBuild::GiveChaosWarAward()
{
    if (CGvGPropMgr::Instance()->IsTest())
    {
        return;
    }

    if(!IsChaosWarEnd())
    {
        return;
    }

    if(m_bGiveChaosWarAward)
    {
        return;
    }
    
    CMapFaction &mapFactions = CFactionMgr::Instance()->GetFactions();
    for(CMapFaction::iterator itr = mapFactions.begin(); itr != mapFactions.end(); ++itr)
    {
        UINT32 dwFactionID  = itr->first;
        CFaction *poFaction = itr->second;
        if(NULL == poFaction)
        {
            continue;
        }

        DT_FACTION_GVG_DATA &stGvGData = poFaction->GetFactionGvGData();
        DT_FACTION_GVG_LOG_LIST &stLogList = stGvGData.stGvGLogList;

        if(0 == stLogList.wLogCnt)
        {
            continue;
        }

        for (INT16 i = stLogList.wLogCnt - 1; i >= 0; i--)
        {
            DT_FACTION_GVG_SINGLE_LOG &stLog = stLogList.astLogs[i];
            DT_GVG_ROUND &stLogRound = stLog.stRound;

            if(stLogRound.byBigRound == GVG_CHAOS_WAR)
            {
                TCHAR aszEmptyFactionName[MAX_FACTION_NUM] = {0};
                TCHAR *pszFactionName = NULL;

                if(EGLR_NO_ENEMY == stLog.byResult)
                {
                    pszFactionName = aszEmptyFactionName;
                }
                else
                {
                    pszFactionName = (dwFactionID == stLog.stG1.stFactionInfo.dwFactionID ?
                        stLog.stG2.stFactionInfo.aszFactionName : stLog.stG1.stFactionInfo.aszFactionName);
                }
                
                CGvGGiftMgr::Instance()->AddBattleReward(dwFactionID, stLogRound.byBigRound, stLogRound.bySmallRound, pszFactionName, (EGLR_WIN == stLog.byResult || EGLR_NO_ENEMY == stLog.byResult));
                poFaction->AddGvGFactionBattleLog(stLogRound.byBigRound, stLogRound.bySmallRound, (GvGLogResult)stLog.byResult, pszFactionName, (stLogRound.bySmallRound < m_stGvGBuildData.stChaosWar.byChaosRound));
                GiveKillComboAward(dwFactionID, stLog.qwVideoID);
            }
        }
    }

    DT_SIMPLE_FACTION_LST_INFO stChaosWinners;
    memset(&stChaosWinners, 0, sizeof(stChaosWinners));
    GetChaosWarResult(stChaosWinners);

    for (UINT16 i = 0; i < stChaosWinners.wFactionNum; i++)
    {
        DT_SIMPLE_FACTION_INFO &stFactionInfo = stChaosWinners.astFactionInfo[i];
        CGvGGiftMgr::Instance()->AddRankReward(stFactionInfo.stFactionBaseInfo.dwFactionID, GVG_CHAOS_WAR, 4);
    }

    m_bGiveChaosWarAward = TRUE;
}

void CGvGBuild::GiveAwardByGvGResult(const DT_GVG_FACTION &stFaction1, const DT_GVG_FACTION &stFaction2, VideoID videoID)
{
    CFaction *poFaction1 = CFactionMgr::Instance()->GetFaction(stFaction1.stBaseData.dwFactionID);
    CFaction *poFaction2 = CFactionMgr::Instance()->GetFaction(stFaction2.stBaseData.dwFactionID);

    if(NULL == poFaction1 && NULL == poFaction2)
    {
        return;
    }

    SGvGFightResult stResult;
    if(GetGvGFightResult(videoID, stResult))
    {
        if(poFaction1)
        {
            GvGLogResult eLogResult = ((stResult.eResult == GR_LEFT_WIN || stResult.eResult == GR_NO_ENEMY) ? EGLR_WIN : EGLR_LOSE);
            poFaction1->AddGvGFactionBattleLog(stResult.eBigRound, stResult.bySmallRound, eLogResult, stFaction2.stBaseData.aszFactionName);
            CGvGGiftMgr::Instance()->AddBattleReward(stFaction1.stBaseData.dwFactionID, stResult.eBigRound, 0, stFaction2.stBaseData.aszFactionName, (eLogResult == EGLR_WIN));
            GiveKillComboAward(stFaction1.stBaseData.dwFactionID, stResult.videoID);
        }
        if(poFaction2)
        {
            GvGLogResult eLogResult = ((stResult.eResult == GR_RIGHT_WIN || stResult.eResult == GR_NO_ENEMY) ? EGLR_WIN : EGLR_LOSE);
            poFaction2->AddGvGFactionBattleLog(stResult.eBigRound, stResult.bySmallRound, eLogResult, stFaction1.stBaseData.aszFactionName);
            CGvGGiftMgr::Instance()->AddBattleReward(stFaction2.stBaseData.dwFactionID, stResult.eBigRound, 0, stFaction1.stBaseData.aszFactionName, (eLogResult == EGLR_WIN));
            GiveKillComboAward(stFaction2.stBaseData.dwFactionID, stResult.videoID);
        }
    }
}

void CGvGBuild::Give16To8Award()
{
    DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stGvGBuildData.stWar16To8To4Info;
    for(UINT16 wNo = 0/*门派序号*/; wNo < MAX_GVG_TOP_FACTION_CNT; wNo++)
    {
        const DT_GVG_FACTION &stFaction1 = stWar16To8.astFactionList[2 * wNo];
        const DT_GVG_FACTION &stFaction2 = stWar16To8.astFactionList[2 * wNo + 1];

        VideoID videoID = stWar16To8.aqwVideoIDList16To8[wNo];

        GiveAwardByGvGResult(stFaction1, stFaction2, videoID);
    }
}

void CGvGBuild::Give8To4Award()
{
    DT_16_TO_8_TO_4_WAR_INFO &stWar8To4 = m_stGvGBuildData.stWar16To8To4Info;

    for (UINT16 i = 0; i < stWar8To4.byWin4Cnt; i++)
    {
        DT_GVG_FACTION &stFaction1 = stWar8To4.astFactionList[stWar8To4.abyWin8List[2 * i] - 1];
        DT_GVG_FACTION &stFaction2 = stWar8To4.astFactionList[stWar8To4.abyWin8List[2 * i + 1] - 1];

        VideoID videoID = stWar8To4.aqwVideoIDList8To4[i];
        GiveAwardByGvGResult(stFaction1, stFaction2, videoID);
    }

    for (UINT16 i = 0; i < stWar8To4.byWin4Cnt; i++)
    {
        if(0 == stWar8To4.abyWin4List[i])
        {
            continue;
        }

        DT_GVG_FACTION &stFaction = stWar8To4.astFactionList[stWar8To4.abyWin4List[i] - 1];
        CGvGGiftMgr::Instance()->AddRankReward(stFaction.stBaseData.dwFactionID, GVG_8_TO_4, 4);
    }
}

void CGvGBuild::GiveFinalWarAward()
{
    if (CGvGPropMgr::Instance()->IsTest())
    {
        return;
    }

    if(!IsFinalWarEnd())
    {
        return;
    }

    if(m_bGiveFinalWarAward)
    {
        return;
    }

    DT_FINAL_WAR_INFO &stFinalWar = m_stGvGBuildData.stFinalWarInfo;

    for(UINT32 bySmallRound = 1; bySmallRound <= stFinalWar.byTotalRound; bySmallRound++)
    {
        DT_FINAL_WAR_ROUND_INFO &stCurRound = stFinalWar.astEachRoundInfo[bySmallRound - 1];

        for(UINT8 byRank = 1; byRank <= stCurRound.byFactionCnt; byRank++)
        {
            UINT8 byMyselfPos = stCurRound.abyRankList[byRank - 1];

            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stRoundFaction = stCurRound.astFactionList[byMyselfPos - 1];
            DT_GVG_FACTION &stFaction      = stFinalWar.astFactionList[byMyselfPos - 1];

            CFaction *poFaction = CFactionMgr::Instance()->GetFaction(stFaction.stBaseData.dwFactionID);
            if(NULL == poFaction)
            {
                continue;
            }
            
            DT_GVG_FACTION &stEnemyFaction = stFinalWar.astFactionList[stRoundFaction.byEnemyPos - 1];

            poFaction->AddGvGFactionBattleLog(GVG_FINAL_WAR, bySmallRound, (GvGLogResult)stRoundFaction.byResult, stEnemyFaction.stBaseData.aszFactionName);
            CGvGGiftMgr::Instance()->AddBattleReward(stFaction.stBaseData.dwFactionID, GVG_FINAL_WAR, bySmallRound, stEnemyFaction.stBaseData.aszFactionName, (stRoundFaction.byResult == EGLR_WIN));
            GiveKillComboAward(stFaction.stBaseData.dwFactionID, stRoundFaction.qwVideoID);
        }
    }
    
    UINT8 byFinalRound = (stFinalWar.byTotalRound >= 1 ? stFinalWar.byTotalRound - 1 : 0);    
    byFinalRound = MIN(GVG_FINAL_WAR_FACTION_CNT - 1, byFinalRound);

    DT_FINAL_WAR_ROUND_INFO &stFinalRound = stFinalWar.astEachRoundInfo[byFinalRound];
    for(UINT8 byRank = 1; byRank <= stFinalRound.byFactionCnt; byRank++)
    {
        UINT8 byMyselfPos = stFinalRound.abyRankList[byRank - 1];

        DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stRoundFaction = stFinalRound.astFactionList[byMyselfPos - 1];
        DT_GVG_FACTION &stFaction      = stFinalWar.astFactionList[byMyselfPos - 1];

        CGvGGiftMgr::Instance()->AddRankReward(stFaction.stBaseData.dwFactionID, GVG_FINAL_WAR, byRank);
    }

    UINT8 byNo1Pos = stFinalRound.abyRankList[0];
    DT_GVG_FACTION &stNo1 = stFinalWar.astFactionList[byNo1Pos - 1];
    CGvGGiftMgr::Instance()->AddHononReward(stNo1.stBaseData.dwFactionID);

    StoreLastWinners();
    GiveMedal();

    m_bGiveFinalWarAward = TRUE;
}

void CGvGBuild::GiveAward()
{
    if (!IsOpenUp())
    {
        return;
    }

    if (CGvGPropMgr::Instance()->IsTest())
    {
        return;
    }

    // 下面的奖励接口重复调用不会导致多发奖励
    switch(m_eStage)
    {
    case GVG_7_END:
        GiveFinalWarAward();
        // 这里故意忽略所有的break

    case GVG_5_8_TO_4_END:
        Give8To4Award();

    case GVG_4_16_TO_8_END:
        Give16To8Award();

    case GVG_3_CHAOS_WAR_END:
        GiveChaosWarAward();

    case GVG_3_CHAOS_WAR_BEGIN:
        GiveMoneyWarAward();
        break;
    }

    CGvGGiftMgr::Instance()->AddGiftTODB();
}

void CGvGBuild::GiveKillComboAward(UINT32 dwFactionID, VideoID videoID)
{
    SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(videoID);
    if(NULL == pstVideo)
    {
        return;
    }

    DT_GVG_VIDEO_FACTION *pstFaction = (pstVideo->stGvGInfo.stG1.dwFactionID == dwFactionID ? &pstVideo->stGvGInfo.stG1 : NULL);
    if(NULL == pstFaction)
    {
        pstFaction = (pstVideo->stGvGInfo.stG2.dwFactionID == dwFactionID ? &pstVideo->stGvGInfo.stG2 : NULL);
        if(NULL == pstFaction)
        {
            return;
        }
    }

    UINT16 wCurSessionID = GetCurSession();

    // 给该门派的每个玩家添加连斩奖励
    DT_GVG_FORMATION_PLAYER_LIST &stPlayerList = pstFaction->stPlayerList;
    for (UINT16 i = 0; i < stPlayerList.byPlayerCnt; i++)
    {
        DT_GVG_FORMATION_PLAYER_INFO &stPlayer = stPlayerList.astPlayerList[i];
        CGvGGiftMgr::Instance()->AddContinueKill(stPlayer.dwPlayerID, wCurSessionID, pstVideo->stGvGInfo.stRound.byBigRound, pstVideo->stGvGInfo.stRound.bySmallRound, stPlayer.byKillCombo);
    }
}

void CGvGBuild::SaveLastWinnersToStruct()
{
    memset(&m_stGvGBuildData.stLastResult,  0, sizeof(m_stGvGBuildData.stLastResult));

    //上届结果
    UINT16 wIdx = 0;

    CMapFactionLastRankItr mapLastItr = m_mapLastResult.begin();
    for ( ; mapLastItr != m_mapLastResult.end(); mapLastItr++, wIdx++)
    {
        m_stGvGBuildData.stLastResult.astFactionRank[wIdx] = mapLastItr->second;
    }

    m_stGvGBuildData.stLastResult.wRankNum = wIdx;



    DT_FINAL_WAR_INFO &stFinalWar = m_stGvGBuildData.stFinalWarInfo;
    DT_FINAL_WAR_ROUND_INFO &stFinalRound = stFinalWar.astEachRoundInfo[stFinalWar.byTotalRound - 1];
    UINT8 byFactionPos = stFinalRound.abyRankList[0];
    if(0 == byFactionPos)
    {
        return;
    }

    DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFinalRoundFaction = stFinalRound.astFactionList[byFactionPos - 1];
    DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[byFactionPos - 1];

    m_stGvGBuildData.stLastResult.stNo1 = stFaction;
}

VOID CGvGBuild::SaveToDB()
{	
    if(0 == m_stGvGBuildData.qwStartTime || 0 == m_stGvGBuildData.wSessionID)
    {
        return;
    }

    memset(&m_stGvGBuildData.stApplayInfo,  0, sizeof(m_stGvGBuildData.stApplayInfo));
    memset(&m_stGvGBuildData.stMoneyResult, 0, sizeof(m_stGvGBuildData.stMoneyResult));
    memset(&m_stGvGBuildData.stChaosWar.stChaosResult, 0, sizeof(m_stGvGBuildData.stChaosWar.stChaosResult));

    //上届结果
    SaveLastWinnersToStruct();

	//门派报名列表
	UINT16 wIdx = 0;
	for ( CFactionApplayDataLstItr itr = m_stFactionApplayDataLst.begin(); itr != m_stFactionApplayDataLst.end(); itr++, wIdx++)
	{
        DT_FACTION_APPLAY_DATA &stApply = m_stGvGBuildData.stApplayInfo.astFactionApplayLst[wIdx];
        stApply = *itr;
	}
	m_stGvGBuildData.stApplayInfo.wApplayNum = wIdx;
	
	CMapSimpleFactionItr mapItr = m_mapMoneyResult.end();
	//财力排行
	wIdx = 0;
	mapItr = m_mapMoneyResult.begin();
	for ( ; mapItr != m_mapMoneyResult.end(); mapItr++, wIdx++ )
	{
		m_stGvGBuildData.stMoneyResult.astFactionInfo[wIdx] = mapItr->second;
	}
	m_stGvGBuildData.stMoneyResult.wFactionNum = wIdx;

	//大乱斗结果
	wIdx = 0;
	mapItr = m_mapChaosResult.begin();
	for ( ; mapItr != m_mapChaosResult.end(); mapItr++, wIdx++ )
	{
		m_stGvGBuildData.stChaosWar.stChaosResult.astFactionInfo[wIdx] = mapItr->second;
	}
	m_stGvGBuildData.stChaosWar.stChaosResult.wFactionNum = wIdx;

	CDBPktBuilder::Instance()->SaveGvGBuildDataReq(m_stGvGBuildData);
    CGvGVideoMgr::Instance()->SaveToDB();
    CPlayerSnapShotMgr::Instance()->SaveToDB();
}

BOOL CGvGBuild::Init()
{
    m_eStage = GVG_0_NOT_START;

    CGSApplication *pGS = gsapi::GetGS();
    if (pGS->IsServerType(TYPE_PK_SERVER)){
        return TRUE;
    }

	if ( !CGvGFactionMgr::Instance()->CreateInstance())
	{
		USR_INFO("[%s : %d] CGvGFactionMgr CreateInstance() ", MSG_MARK);
		return FALSE;
	}

	if ( !CGvGFactionMgr::Instance()->Init())
	{
		USR_INFO("[%s : %d] CGvGFactionMgr Init() ", MSG_MARK);
		return FALSE;
	}

	if ( !CGvGGiftMgr::Instance()->CreateInstance() )
	{
		USR_INFO("[%s : %d] CGvGGiftMgr CreateInstance() ", MSG_MARK);
		return FALSE;
	}

	if ( !CGvGGiftMgr::Instance()->Init())
	{
		USR_INFO("[%s : %d] CGvGGiftMgr Init() ", MSG_MARK);
		return FALSE;
	}
	if ( !LoadBuildDataFromDB())
	{
		USR_INFO("[%s:%d] LoadDataFromDB failed", MSG_MARK);
		return FALSE;
	}

    if(NULL == CTimerEventMgr::Instance()->AddGvGReloadPropEvent())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddGvGReloadPropEvent failed!"), MSG_MARK);
        return FALSE;
    }

    InitData();
    Remapping();
    GiveAward();
    GivePlayerAward();

    CGvGPropMgr::Instance()->FakeGvGTime(m_stGvGBuildData.qwStartTime, m_eStage);
    return TRUE;
}

BOOL CGvGBuild::LoadBuildDataFromDB()
{
    if (!CGvGPropMgr::Instance()->GetGvGProp().bOpenActivity)
    {
        return TRUE;
    }

	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CHAR szSql[1024] = {0};
	sprintf(szSql, "select * from gvgbuild order by SessionID desc limit 1;");

	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	string strError;
	INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );

	if(SDDB_HAS_RECORDSET == nRet && pRecordSet->GetRecord())
	{
        m_stGvGBuildData.wSessionID = SDAtou(pRecordSet->GetFieldValueByName("SessionID"));
        m_stGvGBuildData.wArenaID   = SDAtou(pRecordSet->GetFieldValueByName("ArenaID"));
        m_stGvGBuildData.byCurStage = SDAtou(pRecordSet->GetFieldValueByName("CurStage"));

        if (IsLockActivity())
        {
            ArenaID arenaID = CGvGPropMgr::Instance()->GetArenaIDByZoneID(gsapi::GetZoneID());
            UINT16 wUnLockSession = CGvGPropMgr::Instance()->GetUnLockSession(arenaID);

            USR_INFO(_SDT("[%s: %d]: CGvGBuild::LoadBuildDataFromDB skip load arena %u : the arena are waiting session %u, now session = %u!"), MSG_MARK, arenaID, wUnLockSession, m_stGvGBuildData.wSessionID);
            CGvGPropMgr::Instance()->PrintNotOpenUpArenaZone(arenaID);
            return TRUE;
        }

        const CHAR *pszStartTime = pRecordSet->GetFieldValueByName("StartTime");

        CSDDateTime oStartTime;
        if(!SDTimeFromString(pszStartTime, oStartTime))
        {
            SYS_CRITICAL( _SDT("[%s: %d] CGvGBuild::LoadDataFromDB parse StartTime %s failed, aborted!"), MSG_MARK, pszStartTime);
            return FALSE;
        }

        m_stGvGBuildData.qwStartTime = oStartTime.GetTimeValue();
        
        DecodeSaveDataFromDB("ApplayFactionData", m_stGvGBuildData.stApplayInfo);
        DecodeSaveDataFromDB("LastResult", m_stGvGBuildData.stLastResult);
        DecodeSaveDataFromDB("MoneyResult", m_stGvGBuildData.stMoneyResult);
        DecodeSaveDataFromDB("ChaosWarData", m_stGvGBuildData.stChaosWar);
        DecodeSaveDataFromDB("DoubleWarInfo", m_stGvGBuildData.stWar16To8To4Info);
        DecodeSaveDataFromDB("War16To8List", m_stGvGBuildData.stWar16To8List);
        DecodeSaveDataFromDB("FinalWarData", m_stGvGBuildData.stFinalWarInfo);
        DecodeSaveDataFromDB("FactionsGvGLogData", m_stGvGBuildData.stFactionsLog);
        DecodeSaveDataFromDB("StageFlagList", m_stGvGBuildData.stStageFlagList);
	}

	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}
	
	return TRUE;
}

VOID CGvGBuild::InitData()
{
    SetStage((GVG_STAGE)m_stGvGBuildData.byCurStage);

	ResetApplayFaction();
	ResetResult();
	ResetMoneyRank();
	ResetChaosResult();

    DT_FACTION_APPLAY_LST_DATA &stApplyList = m_stGvGBuildData.stApplayInfo;

	UINT16 wIdx = 0;
	for ( ; wIdx < m_stGvGBuildData.stApplayInfo.wApplayNum; wIdx++)
	{
        DT_FACTION_APPLAY_DATA &stApply = stApplyList.astFactionApplayLst[wIdx];
		AddApplyFaction(stApply.dwFactionID, stApply.qwApplayTime);
	}

	for ( wIdx = 0; wIdx < m_stGvGBuildData.stLastResult.wRankNum; wIdx++)
	{
		AddResult(m_stGvGBuildData.stLastResult.astFactionRank[wIdx].wRank, m_stGvGBuildData.stLastResult.astFactionRank[wIdx]);
	}

	for ( wIdx = 0; wIdx < m_stGvGBuildData.stMoneyResult.wFactionNum; wIdx++)
	{
		AddMoneyRank(m_stGvGBuildData.stMoneyResult.astFactionInfo[wIdx].wRank, m_stGvGBuildData.stMoneyResult.astFactionInfo[wIdx]);
	}

	for ( wIdx = 0; wIdx < m_stGvGBuildData.stChaosWar.stChaosResult.wFactionNum; wIdx++)
	{
		AddChaosResult(m_stGvGBuildData.stChaosWar.stChaosResult.astFactionInfo[wIdx].wRank, m_stGvGBuildData.stChaosWar.stChaosResult.astFactionInfo[wIdx]);
	}

    if (m_eStage < GVG_2_MONEY){
        ResetFactionFunds();
    }

    if (m_eStage > GVG_2_MONEY && m_eStage <= GVG_3_CHAOS_WAR_END){
        if (m_mapMoneyResult.empty()){
            AfterMoneyWar();
            PrintLocalTop8();
        }

        if(IsStageDone(GVG_3_CHAOS_WAR_BEGIN))
        {
            DT_LOCAL_WAR_FACTION_SEED_LIST &stFactionSeedList = m_stGvGBuildData.stChaosWar.stFactionSeedList;
            if(stFactionSeedList.wFactionCnt == 0){
                m_stGvGBuildData.stChaosWar.byChaosRound = 1;

                // 随机配对门派大乱斗的门派选手
                RollChaosWarSeed();
            }
        }
    }

    CGvGPropMgr::Instance()->ConvertToClientDisplayTime(GetCurSession());
}

void CGvGBuild::GetChaosWarSeed(CFactionVec &vecSeed)
{
    DT_LOCAL_WAR_FACTION_SEED_LIST &stFactionSeedList = m_stGvGBuildData.stChaosWar.stFactionSeedList;
    if(0 == stFactionSeedList.wFactionCnt)
    {
        return;
    }

    for (UINT16 i = 0; i < stFactionSeedList.wFactionCnt; i++)
    {
        UINT32 dwFactionID = stFactionSeedList.adwFactionIDList[i];
        if(0 == dwFactionID)
        {
            continue;
        }

        CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
        if(NULL == poFaction)
        {
            continue;
        }

        vecSeed.push_back(poFaction);
    }
}

BOOL CGvGBuild::IsChaosWarEnd()
{
    if (m_eStage < GVG_3_CHAOS_WAR_BEGIN)
    {
        return FALSE;
    }
    else if(m_eStage >= GVG_3_CHAOS_WAR_END)
    {
        return TRUE;
    }
    
    if(GVG_3_CHAOS_WAR_BEGIN == m_eStage)
    {
        DT_GVG_ROUND stRound = GetGambleRound();
        if(0 == stRound.bySmallRound || 0 == m_stGvGBuildData.stChaosWar.byChaosRound)
        {
            return FALSE;
        }

        return stRound.bySmallRound > m_stGvGBuildData.stChaosWar.byChaosRound;
    }
    
    return TRUE;    
}

BOOL CGvGBuild::IsFinalWarEnd()
{
    if (m_eStage < GVG_6_FINAL_WAR_GO)
    {
        return FALSE;
    }

    if(GVG_7_END <= m_eStage)
    {
        return TRUE;
    }

    DT_GVG_ROUND stRound = GetGambleRound();
    if(0 == stRound.bySmallRound || 0 == m_stGvGBuildData.stFinalWarInfo.byTotalRound)
    {
        return FALSE;
    }

    if (m_stGvGBuildData.stFinalWarInfo.byFactionCnt > 1)
    {
        if (stRound.bySmallRound >= m_stGvGBuildData.stFinalWarInfo.byFactionCnt &&
            m_stGvGBuildData.stFinalWarInfo.byTotalRound + 1 >= m_stGvGBuildData.stFinalWarInfo.byFactionCnt)
        {
            return TRUE;
        }
    }
    else if (m_stGvGBuildData.stFinalWarInfo.byFactionCnt == 1)
    {
        if (stRound.bySmallRound >= 2)
        {
            return TRUE;
        }
    }
    else if (m_stGvGBuildData.stFinalWarInfo.byFactionCnt == 0)
    {
        return TRUE;
    }

    return FALSE;
}

void CGvGBuild::RollChaosWarSeed()
{
    DT_LOCAL_WAR_FACTION_SEED_LIST &stFactionSeedList = m_stGvGBuildData.stChaosWar.stFactionSeedList;
    memset(&stFactionSeedList, 0, sizeof(stFactionSeedList));

    DT_SIMPLE_FACTION_LST_INFO stMoneyWinner;
    GetMoneyWarResult(stMoneyWinner);

    if(0 == stMoneyWinner.wFactionNum)
    {
        return;
    }

    C32Set setApplyFaction = m_setApplyFaction;
    for (UINT16 i = 0; i < stMoneyWinner.wFactionNum; i++)
    {
        setApplyFaction.erase(stMoneyWinner.astFactionInfo[i].stFactionBaseInfo.dwFactionID);
    }

    for (C32Set::iterator itr = setApplyFaction.begin(); itr != setApplyFaction.end(); ++itr)
    {
        UINT32 dwFactionID = *itr;
        CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
        if(NULL == poFaction)
        {
            continue;
        }

        stFactionSeedList.adwFactionIDList[stFactionSeedList.wFactionCnt++] =  dwFactionID;
    }

    std::random_shuffle(stFactionSeedList.adwFactionIDList, stFactionSeedList.adwFactionIDList + stFactionSeedList.wFactionCnt);
}

void CGvGBuild::ChaosWarBegin()
{
    AfterMoneyWar();
    GiveMoneyWarAward();
    CGvGGiftMgr::Instance()->AddGiftTODB();

    ResetChaosResult();

    PrintLocalTop8();

    DT_LOCAL_WAR_FACTION_SEED_LIST &stFactionSeedList = m_stGvGBuildData.stChaosWar.stFactionSeedList;
    if(stFactionSeedList.wFactionCnt > 0)
    {
        return;
    }

    // 随机配对门派大乱斗的门派选手
    RollChaosWarSeed();

    CFactionVec vecSeed;
    GetChaosWarSeed(vecSeed);

    DT_GVG_ROUND stCurRound;
    stCurRound.byBigRound   = GVG_CHAOS_WAR;
    stCurRound.bySmallRound = 1;

    m_stGvGBuildData.stChaosWar.byChaosRound = 1;

    SGvGFightResult stResult;
    if(!vecSeed.empty())
    {
        UINT16 wHalfIdx     = (vecSeed.size() + 1) / 2;
        UINT16 wHalfSeedCnt = MAX(wHalfIdx, 4);

        for(UINT16 i = 0; i < wHalfSeedCnt && i < vecSeed.size(); i++)
        {
            CFaction &oSeed1 = *vecSeed[i];
            if(i + wHalfSeedCnt >= (UINT16)vecSeed.size())
            {
                oSeed1.AddNoEnemyGvGLog(stCurRound);
                continue;
            }

            CFaction &oSeed2 = *vecSeed[i + wHalfSeedCnt];

            UINT32 dwFactionID1 = oSeed1.GetFactionID();
            UINT32 dwFactionID2 = oSeed2.GetFactionID();

            VideoID videoID = CGvGVideoMgr::Instance()->MakeVideoID(dwFactionID1, dwFactionID2);
            oSeed1.AddLocalGvGLogNotStarted(dwFactionID1, dwFactionID2, stCurRound, videoID);
            oSeed2.AddLocalGvGLogNotStarted(dwFactionID1, dwFactionID2, stCurRound, videoID);
        }
    }
}

void CGvGBuild::EraseFactionOfflinePlayers(CFactionVec &vecFaction)
{
    for(UINT16 i = 0; i < vecFaction.size(); i++)
    {
        CFaction &oFaction = *vecFaction[i];
        CPlayerVec vecFormationPlayer = oFaction.GetFormationPlayers();
        
        for(UINT16 wPlayerIdx = 0; wPlayerIdx < vecFormationPlayer.size(); wPlayerIdx++)
        {
            CPlayer *poPlayer = vecFormationPlayer[wPlayerIdx];
            if(poPlayer->GetUser())
            {
                continue;
            }

            CPlayerMgr::Instance()->RemovePlayer(poPlayer->GetID());
        }
    }
}

void CGvGBuild::ChaosWarGo()
{
    DT_SIMPLE_FACTION_LST_INFO stChaosWinners;
    memset(&stChaosWinners, 0, sizeof(stChaosWinners));
    GetChaosWarResult(stChaosWinners);

    if(stChaosWinners.wFactionNum > 0)
    {
        return;
    }

    if (m_eStage > GVG_3_CHAOS_WAR_END)
    {
        return;
    }

    // 成员未全部被调入内存的门派列表
    CFactionVec vecOfflineFaction;

    // 门派大乱斗种子选手列表
    CFactionVec vecSeed;
    GetChaosWarSeed(vecSeed);

    if(vecSeed.empty())
    {
        return;
    }

    for(UINT16 i = 0; i < vecSeed.size(); i++)
    {
        CFaction &oSeed = *vecSeed[i];
        if(!oSeed.IsAllFormationPlayerInMemory())
        {
            vecOfflineFaction.push_back(&oSeed);
        }
    }

    // 如果仍有一部分门派的上阵玩家未被调入内存，则此时拒绝开始大乱斗，直到所有上阵玩家集结完毕
    if(!vecOfflineFaction.empty())
    {
        // 分批次调取
        for(UINT16 i = 0; i < vecOfflineFaction.size() && i < 5; i++)
        {
            CFaction *poFaction = vecOfflineFaction[i];
            poFaction->TakeFormationPlayersFromDB();
        }

        return;
    }

    CFactionVec vecOriginalSeed = vecSeed;
    CFactionVec vecWinners;

    DT_GVG_ROUND stRound;
    stRound.byBigRound   = GVG_CHAOS_WAR;
    stRound.bySmallRound = 1;

    SGvGFightResult stResult;
    do
    {
        UINT16 wHalfIdx     = (vecSeed.size() + 1) / 2;
        UINT16 wHalfSeedCnt = MAX(wHalfIdx, 4);
        
        for(UINT16 i = 0; i < wHalfSeedCnt && i < vecSeed.size(); i++)
        {
            CFaction *poSeed1 = vecSeed[i];

            if(i + wHalfSeedCnt >= (UINT16)vecSeed.size())
            {
                poSeed1->AddNoEnemyGvGLog(stRound);
                vecWinners.push_back(poSeed1);
                continue;
            }

            CFaction *poSeed2 = vecSeed[i + wHalfSeedCnt];

            SGvGVideo &stVideo = CGvGMgr::GvG(poSeed1, poSeed2);
            stVideo.stGvGInfo.stRound = stRound;

            CGvGVideoMgr::Instance()->AddVideo(stVideo);
            poSeed1->AddLocalGvGLog(stVideo.stGvGInfo);
            poSeed2->AddLocalGvGLog(stVideo.stGvGInfo);

            stVideo.stGvGInfo.byBattleResult == GR_LEFT_WIN ?
                vecWinners.push_back(poSeed1) :
                vecWinners.push_back(poSeed2);


            memset(&stResult, 0, sizeof(stResult));
            stResult.dwFactionID1 = poSeed1->GetFactionID();
            stResult.dwFactionID2 = poSeed2->GetFactionID();
            stResult.eResult      = (GvGResult)stVideo.stGvGInfo.byBattleResult;
            stResult.eBigRound    = m_eRound;
            stResult.bySmallRound = stRound.bySmallRound;
            stResult.videoID      = stVideo.stGvGInfo.qwVideoID;
            AddGvGFightResult(stResult);
        }

        vecSeed = vecWinners;
        vecWinners.clear();
        ++stRound.bySmallRound;
    }while(vecSeed.size() > 4);

    m_stGvGBuildData.stChaosWar.byChaosRound = MAX(1, stRound.bySmallRound - 1);

    ResetChaosResult();

    for (UINT16 i = 0; i < vecSeed.size(); i++)
    {
        DT_SIMPLE_FACTION_INFO stFaction; // = stChaosResult.astFactionInfo[stChaosResult.wFactionNum++];
        memset(&stFaction, 0, sizeof(stFaction));

        CFaction &oWinner = *vecSeed[i];
        oWinner.GetFactionBaseDataCli2(stFaction.stFactionBaseInfo);
        stFaction.dwZoneID = gsapi::GetZoneID();
        AddChaosResult(i + 1, stFaction);
    }

    CPlayerSnapShotMgr::Instance()->ClearCache();

    // EraseFactionOfflinePlayers(vecOriginalSeed);
    USR_INFO(_SDT("[%s: %d]: CGvGBuild::ChaosWarGo: this zone has compete out %u winners, <current stage = %u>."), MSG_MARK, vecSeed.size(), m_eStage);
    PrintLocalTop8();

    SaveToDB();
}

void CGvGBuild::PrintApplyList()
{
    DT_GVG_RANK_MONEY_LST_DATA stApplyList;
    GetMoneyRank(stApplyList);

    DBG_INFO(_SDT("----------------------print apply list----------------------"));
    for (UINT16 i = 0; i < stApplyList.wRankNum; i++)
    {
        DT_GVG_RANK_MONEY_DATA &stFaction = stApplyList.astMoneyRank[i];
        if(0 == stFaction.stFactionBaseInfo.dwFactionID)
        {
            continue;
        }

        DBG_INFO(_SDT("pos<%2u>: faction[%-8u] %s"), i + 1, stFaction.stFactionBaseInfo.dwFactionID, stFaction.stFactionBaseInfo.aszFactionName);

        CRecordMgr::Instance()->RecordInfo(stFaction.stFactionBaseInfo.dwFactionID, ERM_GVG_ACTIVITY, CRecordMgr::EGA_RANK,
            0, 0, 0, 0, 
            GetCurSession(), GVG_APPLY, i + 1, 0, stFaction.stFactionBaseInfo.aszFactionName);
    }
}

void CGvGBuild::PrintLocalTop8()
{
    DT_SIMPLE_FACTION_LST_INFO stMoneyWarTop;
    DT_SIMPLE_FACTION_LST_INFO stChaosWarTop;

    GetMoneyWarResult(stMoneyWarTop);
    GetChaosWarResult(stChaosWarTop);

    DBG_INFO(_SDT("----------------------print money war winners of session %u----------------------"), GetCurSession());
    for (UINT16 i = 0; i < stMoneyWarTop.wFactionNum; i++)
    {
        DT_SIMPLE_FACTION_INFO &stFaction = stMoneyWarTop.astFactionInfo[i];

        UINT64 qwFactionFunds = 0;
        CFaction *poFaction = CFactionMgr::Instance()->GetFaction(stFaction.stFactionBaseInfo.dwFactionID);
        if (poFaction)
        {
            qwFactionFunds = poFaction->GetFactionGVGFunds();
        }

        DBG_INFO(_SDT("rank<%u>: faction[%-8u] %s, fund : %llu"), i + 1, stFaction.stFactionBaseInfo.dwFactionID, stFaction.stFactionBaseInfo.aszFactionName, qwFactionFunds);

        CRecordMgr::Instance()->RecordInfo(stFaction.stFactionBaseInfo.dwFactionID, ERM_GVG_ACTIVITY, CRecordMgr::EGA_RANK,
            0, 0, 0, 0, 
            GetCurSession(), GVG_MONEY_WAR, i + 1, 0, stFaction.stFactionBaseInfo.aszFactionName);
    }

    if(0 == stChaosWarTop.wFactionNum)
    {
        return;
    }

    DBG_INFO(_SDT("----------------------print chaos war winners of session %u----------------------"), GetCurSession());
    for (UINT16 i = 0; i < stChaosWarTop.wFactionNum; i++)
    {
        DT_SIMPLE_FACTION_INFO &stFaction = stChaosWarTop.astFactionInfo[i];
        DBG_INFO(_SDT("rank<%u>: faction[%-8u] %s"), i + 1, stFaction.stFactionBaseInfo.dwFactionID, stFaction.stFactionBaseInfo.aszFactionName);

        CRecordMgr::Instance()->RecordInfo(stFaction.stFactionBaseInfo.dwFactionID, ERM_GVG_ACTIVITY, CRecordMgr::EGA_RANK,
            0, 0, 0, 0, 
            GetCurSession(), GVG_CHAOS_WAR, i + 1, 0, stFaction.stFactionBaseInfo.aszFactionName);
    }
}

void CGvGBuild::PrintFinalWinners()
{
    DT_GVG_FACTION_RANK_LST_DATA stLastResult;
    GetLastResult(stLastResult);

    DBG_INFO(_SDT("----------------------print final war winners of session %u----------------------"), GetCurSession());
    for (UINT16 i = 0; i < stLastResult.wRankNum; i++)
    {
        DT_GVG_FACTION_RANK_DATA &stFaction = stLastResult.astFactionRank[i];
        if(0 == stFaction.stFactionBaseInfo.dwFactionID)
        {
            continue;
        }

        DBG_INFO(_SDT("rank<%u>: faction[%-8u] %s"), i + 1, stFaction.stFactionBaseInfo.dwFactionID, stFaction.stFactionBaseInfo.aszFactionName);

        CRecordMgr::Instance()->RecordInfo(stFaction.stFactionBaseInfo.dwFactionID, ERM_GVG_ACTIVITY, CRecordMgr::EGA_RANK,
            0, 0, 0, 0, 
            GetCurSession(), GVG_FINAL_WAR, i + 1, 0, stFaction.stFactionBaseInfo.aszFactionName);
    }
}

void CGvGBuild::SetStage(GVG_STAGE eStage)
{
    m_eStage = eStage;
    m_eRound = gvgutil::Stage2Round(m_eStage);
}

void CGvGBuild::AutoApply()
{
    UINT16 wAutoJoinFactionCnt = CGvGPropMgr::Instance()->GetGvGProp().wAutoJoinFactionCnt;
    if(0 == wAutoJoinFactionCnt)
    {
        return;
    }

    if(gsapi::GetZoneID() == CGvGPropMgr::Instance()->GetGvGProp().wAutoApplyExceptZone)
    {
        return;
    }

    DT_FACTION_BASE_DATA_CLI_LST stRankList;
    memset(&stRankList, 0, sizeof(stRankList));
    CFactionMgr::Instance()->GetRankFactionCliLst(stRankList, 0);

    for(UINT16 i = 0; i < stRankList.wFactionNum && i < wAutoJoinFactionCnt; i++)
    {
        CFaction *poFaction = CFactionMgr::Instance()->GetFaction(stRankList.astFactionBaseData[i].stFactionBaseData.dwFactionID);
        if(NULL == poFaction)
        {
            continue;
        }

        ApplyGVGGame(poFaction->GetFactionID());
    }
}

void CGvGBuild::EnterStage(GVG_STAGE eTargetStage)
{
    if(m_stGvGBuildData.byCurStage != eTargetStage)
    {
        SendGetGvGActivityReq();
    }

    if(IsStageDone(eTargetStage))
    {
        return;
    }

    // USR_INFO(_SDT("[%s: %d]: CGvGBuild::EnterStage: <before round = %u>, <now round = %u>."), MSG_MARK, m_eStage, eTargetStage);

    SetStage(eTargetStage);

    switch(eTargetStage)
    {
    case GVG_1_APPLY:
        GvGStart();
        AutoApply();   
        break;

    case GVG_2_MONEY:
        PrintApplyList();
        ResetMoneyRank();
        ResetFactionFunds();
        break;

    case GVG_3_CHAOS_WAR_BEGIN:
        ChaosWarBegin();
        break;

    case GVG_3_CHAOS_WAR_END:
    case GVG_4_16_TO_8_END:
    case GVG_5_8_TO_4_END:
        GiveAward();
        break;

    case GVG_7_END:
        GvGOver();
        break;

    case GVG_8_SLEEPING:
        GvGSleep();
        break;
    }

    SetStageDone(eTargetStage);
    SaveToDB();
}

// 接收到跨服战服务器发送的进行下一轮消息
void CGvGBuild::OnEnterStageNotify(GVG_STAGE eStage)
{
    if(m_eStage != eStage)
    {
        SendGetGvGActivityReq();
    }
}

void CGvGBuild::SendGetGvGActivityReq()
{
    PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ stReq;
    stReq.byNothing = 0;

    gsapi::SendMsg2PK((CHAR*)&stReq, GSPK_GET_GVG_ACTIVITY_DATA_REQ);
}

void CGvGBuild::ReEnterStage()
{
    EnterStage(m_eStage);
}

void CGvGBuild::DispatchGvGLog()
{
    DT_FACTION_LIST_SIMPLE_GVG_LOG &stFactionsLog = m_stGvGBuildData.stFactionsLog;

    for(UINT16 i = 0; i < stFactionsLog.wFactionCnt; i++)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stLogList = stFactionsLog.astLogList[i];

        // 先在本服中查找该门派，如果找得到，直接将新的门派战况塞到该门派的战况历史
        CFaction *poFaction = CFactionMgr::Instance()->GetFaction(stLogList.dwFactionID);
        if(NULL == poFaction)
        {
            continue;
        }

        poFaction->AddGvGLog(stLogList);
    }
}

void CGvGBuild::Remapping()
{
    UpdateFightResult();
    UpdateGvGFaction();
    DispatchGvGLog();
}

void CGvGBuild::HandleSyncGvGNtf(PKT_PKGS_SYNC_GVG_NTF &ntf)
{
    DT_GVG_LOCAL_ACTIVITY_DATA &stLocal = ntf.stGvGInfo;
    DT_GVG_ARENA_DATA &stArena = stLocal.stArena;

    DT_ZONE_ID_LIST &stZoneIDList = stArena.stZoneIDList;

    UINT32 dwLocalZoneID = gsapi::GetZoneID();

    BYTE byGroupID = gvgutil::GetGroupIDByZone(dwLocalZoneID, stArena.stGroupZoneIDList);
    if(0 == byGroupID){
        // SYS_CRITICAL(_SDT("[%s: %d]: CGvGBuild::HandleSyncGvGNtf can't not find group of zone<%u>, ArenaID = %u!"), MSG_MARK, dwLocalZoneID, stArena.wArenaID);
        ArenaID arenaID = CGvGPropMgr::Instance()->GetArenaIDByZoneID(dwLocalZoneID);
        if (0 == arenaID){
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGBuild::HandleSyncGvGNtf can't not find arena of zone<%u>!"), MSG_MARK, dwLocalZoneID);
            return;
        }

        byGroupID = 2; // 说明本区是本赛区的新成员，因之前无参赛历史，故直接把本赛区第2组的数据下发到本区
    }

    m_stGvGBuildData.stWar16To8To4Info = stArena.stWar16To8List.astWar16To8To4Info[byGroupID - 1];
    m_stGvGBuildData.stWar16To8List    = stArena.stWar16To8List;
    m_stGvGBuildData.stFinalWarInfo    = stArena.stFinalWarInfo;
    m_stGvGBuildData.stFactionsLog     = stArena.stFactionsLog;
    m_stGvGBuildData.byCurStage        = stLocal.byCurStage;
    m_stGvGBuildData.stFactionsLog     = stArena.stFactionsLog;
    m_stGvGBuildData.wSessionID        = stLocal.wSessionID;
    m_stGvGBuildData.wArenaID          = stLocal.stArena.wArenaID;
    m_stGvGBuildData.qwStartTime       = stLocal.qwStartTime;

    SetStage((GVG_STAGE)stLocal.byCurStage);

    Remapping();

    EnterStage(m_eStage);
    SaveToDB();

    TakeThisZoneVideo();
}

void CGvGBuild::HandleSyncSupportNtf(DT_FACTION_SUPPORT_LIST &stSupportList)
{
    FactionSupportMap mapAddFactionSupport = m_mapFactionSupport;

    FactionSupportMap mapSupport;
    for(UINT16 i = 0; i < stSupportList.dwFactionCnt; i++)
    {
        DT_FACTION_SUPPORT_INFO &stSupport = stSupportList.astSupportList[i];
        mapSupport[stSupport.dwFactionID] = stSupport.dwSupportCnt + mapAddFactionSupport[stSupport.dwFactionID];
    }

    switch(m_eRound)
    {
    case GVG_16_TO_8:
    case GVG_8_TO_4:
        for(UINT16 i = 0; i < 2; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stGvGBuildData.stWar16To8To4Info;
            for(UINT16 wNo = 0/*门派序号*/; wNo < stWar16To8.byFactionCnt; wNo++)
            {
                DT_GVG_FACTION &stFaction = stWar16To8.astFactionList[wNo];
                stFaction.dwGambleCnt = mapSupport[stFaction.stBaseData.dwFactionID];
            }
        }
        break;

    case GVG_FINAL_WAR:
        {
            DT_FINAL_WAR_INFO &stFinalWar = m_stGvGBuildData.stFinalWarInfo;
            for(UINT16 wNo = 0/*门派序号*/; wNo < stFinalWar.byFactionCnt; wNo++)
            {
                DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[wNo];
                stFaction.dwGambleCnt = mapSupport[stFaction.stBaseData.dwFactionID];
            }
        }
        break;
    }

    UpdateGvGFaction();
}

void CGvGBuild::GivePlayerAward()
{
    const CID2PlayerMap &mapID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();

    for (CID2PlayerMapConstItr itr = mapID2PlayerMap.begin();itr != mapID2PlayerMap.end();itr++)
    {
        CPlayer *pPlayer = itr->second;
        if (NULL == pPlayer)
        {
            continue;
        }

        CPlayerGVG *poPlayerGvG = pPlayer->GetPlayerGVG();
        if(poPlayerGvG)
        {
            poPlayerGvG->GetGambleReward();
        }
    }
}

void CGvGBuild::StoreLastWinners()
{
    ResetResult();

    DT_GVG_FACTION_RANK_DATA stRankInfo;

    DT_FINAL_WAR_INFO &stFinalWar = m_stGvGBuildData.stFinalWarInfo;

    DT_FINAL_WAR_ROUND_INFO &stFinalRound = stFinalWar.astEachRoundInfo[stFinalWar.byTotalRound - 1];
    for(UINT8 byRank = 1; byRank <= stFinalRound.byFactionCnt; byRank++)
    {
        UINT8 byFactionPos = stFinalRound.abyRankList[byRank - 1];
        if(0 == byFactionPos)
        {
            continue;
        }

        DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFinalRoundFaction = stFinalRound.astFactionList[byFactionPos - 1];
        DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[byFactionPos - 1];

        memset(&stRankInfo, 0, sizeof(stRankInfo));

        stRankInfo.stFactionBaseInfo = stFaction.stBaseData;
        stRankInfo.wZoneID           = stFaction.wZoneID;
        stRankInfo.wRank             = byRank;
        stRankInfo.qwSumPower        = stFaction.qwSumPower;
        stRankInfo.dwSumIntegral     = stFaction.wScore;
        stRankInfo.byWinNum          = stFaction.byWinCnt;
        stRankInfo.byLostNum         = stFaction.byLoseCnt;

        AddResult(byRank, stRankInfo);
    }

    SaveLastWinnersToStruct();
}

void CGvGBuild::Clear()
{
    ResetChaosPair();
    ResetChaosResult();
    ResetApplayFaction();
    ResetMoneyRank();
    ClearExpiredFactionGvG();
    ResetStageFlag();

    CPlayerSnapShotMgr::Instance()->Clear();
    CGvGFactionMgr::Instance()->Clear();

    m_mapGvGResult.clear();
    m_mapFactionSupport.clear();

    m_bGiveChaosWarAward = FALSE;
    m_bGiveFinalWarAward = FALSE;
}

void CGvGBuild::GvGStart()
{
    Clear();
    CGvGVideoMgr::Instance()->Clear();
    CGvGGiftMgr::Instance()->RemoveOverGift();

    CGvGPropMgr::Instance()->FakeGvGTime(m_stGvGBuildData.qwStartTime, GVG_1_APPLY);

	CUser* poUser = NULL;
	CPlayer* poPlayer = NULL;
	const CSessionID2UserMap&  mapUser = CUserMgr::Instance()->GetAllUserMap();
	for (CSessionID2UserMapConstItr itr = mapUser.begin(); itr != mapUser.end(); itr++)
	{
		poUser = itr->second;
		if(NULL == poUser)
		{
			continue;
		}

		poPlayer = poUser->GetPlayer();
		if(NULL == poPlayer)
		{
			continue;
		}
		
		CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->UnActivityMedal(EMST_GVG_MEDAL);
		}
	}

    UpdateGvGFaction();
    CGvGPropMgr::Instance()->ConvertToClientDisplayTime(GetCurSession());
}

// 跨服门派战活动是否已开放
BOOL CGvGBuild::IsOpenUp()
{
    return m_stGvGBuildData.qwStartTime > 0;
}

BOOL CGvGBuild::IsActivityTime()
{
    if(GVG_1_APPLY <= m_eStage && m_eStage < GVG_7_END)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CGvGBuild::CanOpenUp()
{
    UINT16 wFactionCnt = 0;

    CMapFaction &mapFaction = CFactionMgr::Instance()->GetFactions();
    for(CMapFaction::iterator itr = mapFaction.begin(); itr != mapFaction.end(); ++itr)
    {
        CFaction *poFaction = itr->second;

        //检查门派等级
        if ( poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel >= CGvGPropMgr::Instance()->GetJoinGVGFactionLevel())
        {
            if(++wFactionCnt >= CGvGPropMgr::Instance()->GetGvGProp().wOpenUpRequiredFactionCnt)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CGvGBuild::IsStageDone(GVG_STAGE eStage)
{
    if(eStage >= GVG_STAGE_MAX)
    {
        return FALSE;
    }

    return m_stGvGBuildData.stStageFlagList.abyFlagList[eStage];
}

void CGvGBuild::SetStageDone(GVG_STAGE eStage)
{
    if(eStage >= GVG_STAGE_MAX)
    {
        return;
    }

    m_stGvGBuildData.stStageFlagList.abyFlagList[eStage] = TRUE;
}

void CGvGBuild::ResetStageFlag()
{
    memset(&m_stGvGBuildData.stStageFlagList, 0, sizeof(m_stGvGBuildData.stStageFlagList));
}

BOOL CGvGBuild::IsInWar()
{
    BOOL bInWar = FALSE;
    switch(m_eStage)
    {
    case GVG_3_CHAOS_WAR_BEGIN:
        bInWar = !IsChaosWarEnd();
        break;
    }

    return bInWar;
}

BOOL CGvGBuild::IsWillWar()
{
    BOOL bWillWar = FALSE;
    switch(m_eStage)
    {
    case GVG_4_16_TO_8_GO:
    case GVG_5_8_TO_4_GO:
        bWillWar = TRUE;
        break;

    case GVG_6_FINAL_WAR_GO:
    {
        UINT64 qwFinalWarGoTime = GetStageTime(GVG_6_FINAL_WAR_GO);
        DT_GVG_ROUND stRound = GetGambleRound();
        
        SGvGProp &stGvGProp = CGvGPropMgr::Instance()->GetGvGProp();
        UINT64 qwThisRoundEndTime = qwFinalWarGoTime + stGvGProp.dwFinalWarEachRoundTime * stRound.bySmallRound;

        UINT64 qwNow = SDTimeSecs();
        if(qwNow >= qwThisRoundEndTime - stGvGProp.dwForbidFormationTime)
        {
            bWillWar = TRUE;
        }

        break;
    }
    }

    return bWillWar;
}

void CGvGBuild::GvGSleep()
{
    CGvGPropMgr::Instance()->FakeGvGTime(m_stGvGBuildData.qwStartTime, GVG_8_SLEEPING);
}

void CGvGBuild::GiveMedal()
{
    for ( UINT16 wIdx = 0; wIdx < m_stGvGBuildData.stLastResult.wRankNum; wIdx++)
    {
        CFaction* poFaction = CFactionMgr::Instance()->GetFaction(m_stGvGBuildData.stLastResult.astFactionRank[wIdx].stFactionBaseInfo.dwFactionID);
        if ( NULL != poFaction )
        {
            poFaction->CheckGvGMedal();
        }
    }
}

void CGvGBuild::GvGOver()
{
    GiveAward();
    GivePlayerAward();

    StoreLastWinners();

    Clear();
    PrintFinalWinners();

	GiveMedal();

    UpdateGvGFaction();
}

BOOL CGvGBuild::IsLockActivity()
{
    ArenaID arenaID = CGvGPropMgr::Instance()->GetArenaIDByZoneID(gsapi::GetZoneID());
    UINT16 wUnLockSession = CGvGPropMgr::Instance()->GetUnLockSession(m_stGvGBuildData.wArenaID);

    BOOL bLock = (m_stGvGBuildData.wSessionID < wUnLockSession || 0 == wUnLockSession);
    return bLock;
}

void CGvGBuild::CheckOpenUp()
{
    if (!IsOpenUp())
    {
        return;
    }

    BOOL bLock = IsLockActivity();
    if (bLock || !CGvGPropMgr::Instance()->GetGvGProp().bOpenActivity)
    {
        DBG_INFO(_SDT("[CGvGBuild::CheckOpenUp]: detecting activity is set to closed, now session = %u!"), GetCurSession());

        SetStage(GVG_0_NOT_START);
        memset(&m_stGvGBuildData, 0, sizeof(m_stGvGBuildData));

        Clear();
        CGvGVideoMgr::Instance()->Clear();

        SaveToDB();
    }
}

void CGvGBuild::UpdateGvGFaction()
{
    CGvGFactionMgr::Instance()->AddGvGFaction(m_stGvGBuildData.stLastResult.stNo1);

    for(UINT16 i = 0; i < m_stGvGBuildData.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stGvGBuildData.stWar16To8List.astWar16To8To4Info[i];
        AddGvGFaction(stWar16To8.astFactionList, stWar16To8.byFactionCnt);
    }

    DT_FINAL_WAR_INFO &stFinalWar = m_stGvGBuildData.stFinalWarInfo;
    AddGvGFaction(stFinalWar.astFactionList, stFinalWar.byFactionCnt);
}

void CGvGBuild::AddGvGFaction(DT_GVG_FACTION astFactionList[], UINT8 byFactionCnt)
{
    for (UINT16 i = 0; i < byFactionCnt; i++)
    {
        CGvGFactionMgr::Instance()->AddGvGFaction(astFactionList[i]);
    }
}

void CGvGBuild::UpdateFightResult()
{
    DT_FACTION_LIST_SIMPLE_GVG_LOG &stFactionsLog = m_stGvGBuildData.stFactionsLog;

    SGvGFightResult stResult;
    
    for(UINT16 i = 0; i < stFactionsLog.wFactionCnt; i++)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stFactionLogList = stFactionsLog.astLogList[i];
        for(UINT16 wLogIdx = 0; wLogIdx < stFactionLogList.wLogCnt; wLogIdx++)
        {
            DT_FACTION_SIMPLE_GVG_LOG &stLog = stFactionLogList.astLogs[wLogIdx];

            stResult.dwFactionID1 = stLog.dwFactionID1;
            stResult.dwFactionID2 = stLog.dwFactionID2;
            
            stResult.eBigRound    = (GvGRound)stLog.stRound.byBigRound;
            stResult.bySmallRound = stLog.stRound.bySmallRound;
            stResult.videoID      = stLog.qwVideoID;
            stResult.eResult      = (GvGResult)stLog.byGvGResult;

            AddGvGFightResult(stResult);
        }
    }
}

void CGvGBuild::SimpleGvGLogToFullLog(DT_FACTION_GVG_LOG_LIST &stLogList, DT_FACTION_SIMPLE_GVG_LOG_LIST &stSimpleLogList)
{
    DT_GVG_FACTION stGvGFaction1;
    DT_GVG_FACTION stGvGFaction2;

    if(stSimpleLogList.wLogCnt >= MAX_GVG_SIMPLE_LOG_CNT)
    {
        return;
    }

    for (UINT16 i = 0; i < stSimpleLogList.wLogCnt; i++)
    {
        DT_FACTION_SIMPLE_GVG_LOG &stSimpleLog = stSimpleLogList.astLogs[i];
        DT_FACTION_GVG_SINGLE_LOG *pstLog = gvgutil::FindGvGLog(stLogList, stSimpleLog.stRound);
        if(NULL == pstLog)
        {
            if(stLogList.wLogCnt >= MAX_GVG_LOG_CNT)
            {
                break;
            }

            pstLog = &stLogList.astLogs[stLogList.wLogCnt++];
        }

        pstLog->stRound   = stSimpleLog.stRound;
        pstLog->byResult  = stSimpleLog.byLogResult;
        pstLog->wScore    = stSimpleLog.wAddScore;
        pstLog->qwVideoID = stSimpleLog.qwVideoID;

        CGvGFactionMgr::Instance()->GetFactionInfo(stSimpleLog.dwFactionID1, stGvGFaction1);
        CGvGFactionMgr::Instance()->GetFactionInfo(stSimpleLog.dwFactionID2, stGvGFaction2);

        pstLog->stG1.stFactionInfo = stGvGFaction1.stBaseData;
        pstLog->stG2.stFactionInfo = stGvGFaction2.stBaseData;

        pstLog->stG1.wZoneID = stGvGFaction1.wZoneID;
        pstLog->stG2.wZoneID = stGvGFaction2.wZoneID;

        pstLog->stG1.byIsWin = (GR_LEFT_WIN == stSimpleLog.byGvGResult);
        pstLog->stG2.byIsWin = (GR_RIGHT_WIN == stSimpleLog.byGvGResult);
    }
}

void CGvGBuild::ClearExpiredFactionGvG()
{
    CMapFaction &mapFactions = CFactionMgr::Instance()->GetFactions();
    for(CMapFaction::iterator itr = mapFactions.begin(); itr != mapFactions.end(); ++itr)
    {
        CFaction *poFaction = itr->second;
        poFaction->ClearOldGvG();
    }
}

void CGvGBuild::AddGvGFightResult(SGvGFightResult &stFightResult)
{
    m_mapGvGResult[stFightResult.videoID] = stFightResult;
}

BOOL CGvGBuild::GetGvGFightResult(VideoID videoID, SGvGFightResult &stResult)
{
    SGvGFightResult *pstResult = FindGvGFightResult(videoID);
    if(NULL == pstResult)
    {
        return FALSE;
    }

    stResult = *pstResult;

    if(IsNeedHideGvGLog(stResult.eBigRound, stResult.bySmallRound))
    {
        stResult.eResult = GR_NOT_START;
        stResult.videoID = 0;
    }
    else if(!IsGvGLogStart(stResult.eBigRound, stResult.bySmallRound))
    {
        stResult.eResult = GR_NOT_START;
        stResult.videoID = 0;
    }
    
    return TRUE;
}

SGvGFightResult* CGvGBuild::FindGvGFightResult(VideoID videoID)
{
    GvGFightResultMap::iterator itr = m_mapGvGResult.find(videoID);
    if (m_mapGvGResult.end() == itr)
    {
        return NULL;
    }

    SGvGFightResult *pstResult = &itr->second;
    return pstResult;
}

UINT16 CGvGBuild::GetFactionMoneyRank(PKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK& stAck)
{
	GetMoneyRank(stAck.stMoneyRank);

	return ERR_OPEN_FACTION_MONEY_RANK::ID_SUCCESS;
}

UINT16 CGvGBuild::GetApplayFaction(PKT_CLIGS_OPEN_ALREAD_APPLAY_ACK& stAck)
{
	GetMoneyRank(stAck.stApplyInfo);
	return ERR_OPEN_ALREAD_APPLAY::ID_SUCCESS;
}

UINT16 CGvGBuild::OpenGVGFormation(PKT_CLIGS_OPEN_GVG_FORMATION_ACK& stAck, CPlayer* poPlayer)
{
	CFaction* poFaction = poPlayer->GetFaction();	
	if ( NULL == poFaction)
	{
		return ERR_OPEN_GVG_FORMATION::ID_NOT_HEAD;
	}
	em_Faction_Job eJob = poFaction->GetJob(poPlayer->GetID());
	if( eJob != em_Faction_MartialHead)
	{
		return ERR_OPEN_GVG_FORMATION::ID_NOT_HEAD;
	}
	 
	//if( !IsFormationTime() )
	//{
	//	return ERR_OPEN_GVG_FORMATION::ID_TIME_NOT_MATCH;
	//}

	//if ( !IsApplay(poFaction->GetDT_FACTION_BASE_DATA().dwFactionID))
	//{
	//	return ERR_OPEN_GVG_FORMATION::ID_APPLAY_FIRST;
	//}

	poFaction->GetFormationInfo(stAck.stFormationInfo, stAck.stPlayerLstInfo, stAck.wCurFactionLevel);
	return ERR_OPEN_GVG_FORMATION::ID_SUCCESS;
}

UINT16 CGvGBuild::SeeGvGVideo(PKT_CLIGS_SEE_GVG_VIDEO_ACK_v410 &stAck, UINT64 qwVideoID, BYTE byVideoNum)
{   
    SGvGFightResult stGvGFightResult;
    if(!GetGvGFightResult(qwVideoID, stGvGFightResult))
    {
        return ERR_SEE_GVG_VIDEO::ID_NOT_FIND_VIDEO;
    }

    if(GR_NOT_START == stGvGFightResult.eResult)
    {
        return ERR_SEE_GVG_VIDEO::ID_NOT_START;
    }

    SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(qwVideoID);
    if(NULL == pstVideo || 0 == byVideoNum)
    {
        return ERR_SEE_GVG_VIDEO::ID_NOT_FIND_VIDEO;
    }

    if(byVideoNum > pstVideo->stGvGInfo.byPKVideoCnt)
    {
        return ERR_SEE_GVG_VIDEO::ID_REACH_END;
    }

    stAck.qwVideoID = qwVideoID;
    stAck.byVideoNum = byVideoNum;

    stAck.byHaveNextVideo = (byVideoNum < pstVideo->vecPvPVideo.size());

    gvgutil::CopyVideoFactionInfo(stAck.stG1, pstVideo->stGvGInfo.stG1);
    gvgutil::CopyVideoFactionInfo(stAck.stG2, pstVideo->stGvGInfo.stG2);

    if(byVideoNum <= pstVideo->vecPvPVideo.size())
    {
        const DT_GVG_PK_VIDEO &stPKVideo = pstVideo->vecPvPVideo[byVideoNum - 1];
        stAck.stVideo = stPKVideo.stPKVideo;

        SDStrncpy(stAck.stG1.aszNickName, stPKVideo.stPKVideo.stEmptyHPVideo.stMyselfInfo.aszDispName, USERNAME_LEN - 1);
        SDStrncpy(stAck.stG2.aszNickName, stPKVideo.stPKVideo.stEmptyHPVideo.stEnemyInfo.aszDispName, USERNAME_LEN - 1);

        stAck.stG1.byJob = pstVideo->stGvGInfo.stG1.stPlayerList.astPlayerList[stPKVideo.byPlayerIdx1].byPlayerJob;
        stAck.stG2.byJob = pstVideo->stGvGInfo.stG2.stPlayerList.astPlayerList[stPKVideo.byPlayerIdx2].byPlayerJob;

        stAck.stG1.byKillCombo = stPKVideo.byKillCombo1;
        stAck.stG2.byKillCombo = stPKVideo.byKillCombo2;

        stAck.stG1.byRemainPlayerCnt = pstVideo->stGvGInfo.stG1.stPlayerList.byPlayerCnt - stPKVideo.byPlayerIdx1;
        stAck.stG2.byRemainPlayerCnt = pstVideo->stGvGInfo.stG2.stPlayerList.byPlayerCnt - stPKVideo.byPlayerIdx2;
        
        UINT8 byPreKillCombo1 = 0;
        UINT8 byPreKillCombo2 = 0;

        if (byVideoNum >= 2)
        {
            const DT_GVG_PK_VIDEO &stPrePKVideo = pstVideo->vecPvPVideo[byVideoNum - 2];
            byPreKillCombo1 = stPrePKVideo.byKillCombo1;
            byPreKillCombo2 = stPrePKVideo.byKillCombo2;
        }

        stAck.byIsWeak1 = CGvGPropMgr::Instance()->IsMoreWeaker(byPreKillCombo1);
        stAck.byIsWeak2 = CGvGPropMgr::Instance()->IsMoreWeaker(byPreKillCombo2);
        stAck.byEnegy1 = CGvGPropMgr::Instance()->GetEnegyByKillCombo(byPreKillCombo1);
        stAck.byEnegy2 = CGvGPropMgr::Instance()->GetEnegyByKillCombo(byPreKillCombo2);
    }

    return ERR_SEE_GVG_VIDEO::ID_SUCCESS;
}

// 观看录像
UINT16 CGvGBuild::EndGvGVideo(PKT_CLIGS_END_GVG_VIDEO_ACK_v410 &stAck, UINT64 qwVideoID, BYTE byEndType)
{
    SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(qwVideoID);
    if(NULL == pstVideo)
    {
        return ERR_END_GVG_VIDEO::ID_NOT_FIND_VIDEO;
    }

    stAck.qwVideoID = qwVideoID;
    stAck.byEndType = byEndType;
    stAck.stVideo.stEmptyHPVideo.byBattleRoundNum = 0;
    stAck.stRound = pstVideo->stGvGInfo.stRound;

    gvgutil::CopyVideoFactionInfo(stAck.stG1, pstVideo->stGvGInfo.stG1);
    gvgutil::CopyVideoFactionInfo(stAck.stG2, pstVideo->stGvGInfo.stG2);

    if(0 == pstVideo->stGvGInfo.byPKVideoCnt)
    {
        return ERR_END_GVG_VIDEO::ID_SUCCESS;
    }

    DT_GVG_PK_VIDEO &stPKVideo = pstVideo->vecPvPVideo.back();
    stAck.stVideo = stPKVideo.stPKVideo;

    SDStrncpy(stAck.stG1.aszNickName, stPKVideo.stPKVideo.stEmptyHPVideo.stMyselfInfo.aszDispName, USERNAME_LEN - 1);
    SDStrncpy(stAck.stG2.aszNickName, stPKVideo.stPKVideo.stEmptyHPVideo.stEnemyInfo.aszDispName, USERNAME_LEN - 1);

    DT_GVG_FORMATION_PLAYER_LIST &stPlayerList1 = pstVideo->stGvGInfo.stG1.stPlayerList;
    DT_GVG_FORMATION_PLAYER_LIST &stPlayerList2 = pstVideo->stGvGInfo.stG2.stPlayerList;

    stAck.stG1.byJob = stPlayerList1.astPlayerList[stPKVideo.byPlayerIdx1].byPlayerJob;
    stAck.stG2.byJob = stPlayerList2.astPlayerList[stPKVideo.byPlayerIdx2].byPlayerJob;

    stAck.stG1.byKillCombo = stPKVideo.byKillCombo1;
    stAck.stG2.byKillCombo = stPKVideo.byKillCombo2;

    stAck.stG1.byRemainPlayerCnt = pstVideo->stGvGInfo.stG1.stPlayerList.byPlayerCnt - stPKVideo.byPlayerIdx1;
    stAck.stG2.byRemainPlayerCnt = pstVideo->stGvGInfo.stG2.stPlayerList.byPlayerCnt - stPKVideo.byPlayerIdx2;

    if(pstVideo->stGvGInfo.byBattleResult == EGLR_WIN)
    {
        stAck.stG2.byRemainPlayerCnt--;
    }
    else
    {
        stAck.stG1.byRemainPlayerCnt--;
    }

    DT_GVG_MVP_LIST &stMVPList = stAck.stMVPList;

    gvgutil::GetMVPList(stMVPList, pstVideo->stGvGInfo.stG1.stPlayerList, (pstVideo->stGvGInfo.byBattleResult == EGLR_WIN ? 1 : 0), pstVideo->stGvGInfo.stG1.byFactionIconID, (GvGRound)pstVideo->stGvGInfo.stRound.byBigRound);
    gvgutil::GetMVPList(stMVPList, pstVideo->stGvGInfo.stG2.stPlayerList, (pstVideo->stGvGInfo.byBattleResult == EGLR_WIN ? 0 : 1), pstVideo->stGvGInfo.stG2.byFactionIconID, (GvGRound)pstVideo->stGvGInfo.stRound.byBigRound);

    std::sort(stMVPList.astMVPList, stMVPList.astMVPList + stMVPList.byMVPCnt, gvgutil::SortMVPByScoreDesc);
    stAck.stVideo.stEmptyHPVideo.byBattleRoundNum = 0;

    stMVPList.byMVPCnt = MIN(stMVPList.byMVPCnt, CGvGPropMgr::Instance()->GetGvGProp().wDisplayVideoMVPCnt);

    // 自动结束
    if(1 == stAck.byEndType)
    {
        stAck.stVideo.stEmptyHPVideo.stMyselfInfo = pstVideo->stGvGInfo.stFinalRoundInfo.stMyselfObj;
        stAck.stVideo.stEmptyHPVideo.stEnemyInfo  = pstVideo->stGvGInfo.stFinalRoundInfo.stEnemyObj;
    }

    return ERR_END_GVG_VIDEO::ID_SUCCESS;
}

UINT16 CGvGBuild::ShowGvGMVP(PKT_CLIGS_SHOW_GVG_MVP_ACK &stAck, UINT64 qwVideoID)
{
    SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(qwVideoID);
    if(NULL == pstVideo)
    {
        return ERR_SHOW_GVG_MVP::ID_NOT_FIND_VIDEO;
    }

    DT_GVG_PK_VIDEO &stPKVideo = pstVideo->vecPvPVideo.back();
    stAck.qwVideoID = qwVideoID;

    DT_GVG_FORMATION_PLAYER_LIST &stPlayerList1 = pstVideo->stGvGInfo.stG1.stPlayerList;
    DT_GVG_FORMATION_PLAYER_LIST &stPlayerList2 = pstVideo->stGvGInfo.stG2.stPlayerList;

    DT_GVG_MVP_LIST &stMVPList = stAck.stMVPList;

    gvgutil::GetMVPList(stMVPList, pstVideo->stGvGInfo.stG1.stPlayerList, (pstVideo->stGvGInfo.byBattleResult == EGLR_WIN ? 1 : 0), pstVideo->stGvGInfo.stG1.byFactionIconID, (GvGRound)pstVideo->stGvGInfo.stRound.byBigRound);
    gvgutil::GetMVPList(stMVPList, pstVideo->stGvGInfo.stG2.stPlayerList, (pstVideo->stGvGInfo.byBattleResult == EGLR_WIN ? 0 : 1), pstVideo->stGvGInfo.stG2.byFactionIconID, (GvGRound)pstVideo->stGvGInfo.stRound.byBigRound);

    std::sort(stMVPList.astMVPList, stMVPList.astMVPList + stMVPList.byMVPCnt, gvgutil::SortMVPByScoreDesc);
    return ERR_SHOW_GVG_MVP::ID_SUCCESS;
}

DT_GVG_ROUND CGvGBuild::GetGambleRound()
{
    DT_GVG_ROUND stRound;
    stRound.byBigRound   = m_eRound;
    stRound.bySmallRound = 0;
    
    UINT64 qwNow = SDTimeSecs();

    SGvGProp &stGvGProp = CGvGPropMgr::Instance()->GetGvGProp();
    switch(m_eStage)
    {
    case GVG_3_CHAOS_WAR_BEGIN:
    {
        UINT64 qwChaoWarStartTime = CGvGPropMgr::Instance()->GetStageStartTime(m_stGvGBuildData.qwStartTime, GVG_3_CHAOS_WAR_BEGIN);
        if(qwNow >= qwChaoWarStartTime)
        {
            stRound.bySmallRound = (UINT8)((qwNow - qwChaoWarStartTime) / stGvGProp.dwChaosWarEachRoundTime + 1);
        }

        stRound.bySmallRound = MIN(m_stGvGBuildData.stChaosWar.byChaosRound + 1, stRound.bySmallRound);
        stRound.bySmallRound = MAX(1, stRound.bySmallRound);
    }
    break;

    case GVG_6_FINAL_WAR_BEGIN:
        stRound.bySmallRound = 1;
        break;

    case GVG_6_FINAL_WAR_GO:
    {
        UINT64 qwFinalWarStartDisplayTime = CGvGPropMgr::Instance()->GetStageStartTime(m_stGvGBuildData.qwStartTime, GVG_6_FINAL_WAR_GO);

        if(qwNow >= qwFinalWarStartDisplayTime)
        {
            stRound.bySmallRound = (UINT8)((qwNow - qwFinalWarStartDisplayTime) / stGvGProp.dwFinalWarEachRoundTime + 1);
        }

        stRound.bySmallRound = MIN(m_stGvGBuildData.stFinalWarInfo.byTotalRound + 1, stRound.bySmallRound);
        stRound.bySmallRound = MAX(1, stRound.bySmallRound);
    }
    break;

    case GVG_7_END:
        stRound.bySmallRound = m_stGvGBuildData.stFinalWarInfo.byTotalRound + 1;
        break;
    }
    
    return stRound;
}

UINT32 CGvGBuild::GetDisplayFactionScore(UINT32 dwFactionID)
{
    DT_GVG_FACTION *pstGvGFaction = CGvGFactionMgr::Instance()->FindFactionInfo(dwFactionID);
    if(NULL == pstGvGFaction)
    {
        return 0;
    }

    if(m_eStage < GVG_6_FINAL_WAR_GO)
    {
        return 0;
    }

    DT_FINAL_WAR_INFO &stFinalWarInfo = m_stGvGBuildData.stFinalWarInfo;
    DT_GVG_ROUND stRound = GetGambleRound();
    if(stRound.bySmallRound <= 1)
    {
        return 0;
    }

    // 计算出当前处于总决赛的第几轮
    UINT32 byPassedStageCnt = MIN(stFinalWarInfo.byTotalRound, stRound.bySmallRound - 1);
    if(0 == byPassedStageCnt)
    {
        return 0;
    }

    // 虽然是处于当前轮次，但显示的是上一轮次的积分
    DT_FINAL_WAR_ROUND_INFO &stWarRoundInfo = stFinalWarInfo.astEachRoundInfo[byPassedStageCnt - 1];
    for(UINT16 i = 0; i < stWarRoundInfo.byFactionCnt; i++)
    {
        DT_GVG_FACTION &stGvGFaction = stFinalWarInfo.astFactionList[i];
        if(dwFactionID == stGvGFaction.stBaseData.dwFactionID)
        {
            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stRoundFaction = stWarRoundInfo.astFactionList[i];
            return stRoundFaction.wScore;
        }
    }

    return 0;
}

void CGvGBuild::GetLocalFactionGvGLog(DT_FACTION_GVG_LOG &stLog, CFaction &oFaction)
{
    DT_FACTION_BASE_DATA_CLI2 stBaseCli;
    oFaction.GetFactionBaseDataCli2(stBaseCli);

    DT_GVG_LOG_FACTION_INFO &stFlag = stLog.stFlag;
    gvgutil::SaveFactionBaseToFlag(stFlag, stBaseCli);

    UINT16 wFactionNumberPeople = 0;
    UINT16 wMaxFactionNumberPeople = 0;
    oFaction.GetFactionPlayerNum(wFactionNumberPeople, wMaxFactionNumberPeople);

    stFlag.wJoinNum   = oFaction.GetFactionGvGData().stGvGFormation.byPlayerNum;
    stFlag.qwSumPower = oFaction.GetSumPower();
    stFlag.dwScore    = GetDisplayFactionScore(oFaction.GetFactionID());

    stLog.stLogList = oFaction.GetFactionGvGData().stGvGLogList;
}

void CGvGBuild::GetGlobalFactionGvGLog(DT_FACTION_GVG_LOG &stLog, UINT32 dwFactionID)
{
    DT_GVG_LOG_FACTION_INFO &stFlag = stLog.stFlag;

    DT_GVG_FACTION *pstGvGFaction = CGvGFactionMgr::Instance()->FindFactionInfo(dwFactionID);
    if(NULL == pstGvGFaction)
    {
        return;
    }

    gvgutil::SaveFactionBaseToFlag(stFlag, pstGvGFaction->stBaseData);
    stFlag.wJoinNum = pstGvGFaction->byJoinCnt;
    stFlag.qwSumPower = pstGvGFaction->qwSumPower;
    stFlag.dwScore = GetDisplayFactionScore(dwFactionID);

    DT_FACTION_SIMPLE_GVG_LOG_LIST *pstFindFactionSimpleLog = NULL;
    for(UINT16 i = 0; i < m_stGvGBuildData.stFactionsLog.wFactionCnt; i++)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stSimpleLog = m_stGvGBuildData.stFactionsLog.astLogList[i];
        if(dwFactionID == stSimpleLog.dwFactionID)
        {
            pstFindFactionSimpleLog = &stSimpleLog;
            break;
        }
    }

    if(NULL == pstFindFactionSimpleLog)
    {
        return;
    }

    SimpleGvGLogToFullLog(stLog.stLogList, *pstFindFactionSimpleLog);
}

void CGvGBuild::HideGvGLog(DT_FACTION_GVG_LOG_LIST &stLogList)
{
    stLogList.wLogCnt = GetReadableGvGLogCnt(stLogList);
    if(0 == stLogList.wLogCnt)
    {
        return;
    }

    DT_FACTION_GVG_SINGLE_LOG &stLastLog = stLogList.astLogs[stLogList.wLogCnt - 1];
    if(!IsGvGLogStart(stLastLog.stRound.byBigRound, stLastLog.stRound.bySmallRound))
    {
        stLastLog.byResult     = EGLR_NOT_START;
        stLastLog.wScore       = 0;
        stLastLog.stG1.byIsWin = 0;
        stLastLog.stG2.byIsWin = 0;
        stLastLog.qwVideoID    = 0;
    }
}

UINT16 CGvGBuild::GetReadableGvGLogCnt(DT_FACTION_GVG_LOG_LIST &stLogList)
{
    if(0 == stLogList.wLogCnt)
    {
        return 0;
    }

    INT16 wReadableLogCnt = stLogList.wLogCnt;
    for (INT16 i = stLogList.wLogCnt - 1; i >= 0; i--)
    {
        DT_FACTION_GVG_SINGLE_LOG &stLog = stLogList.astLogs[i];
        if (IsNeedHideGvGLog(stLog.stRound.byBigRound, stLog.stRound.bySmallRound))
        {
            --wReadableLogCnt;
        }
        else
        {
            break;
        }
    }

    if(wReadableLogCnt < 0)
    {
        wReadableLogCnt = 0;
    }

    return (UINT16)wReadableLogCnt;
}

BOOL CGvGBuild::IsNeedHideGvGLog(UINT8 byBigRound, UINT8 bySmallRound)
{
    if(GVG_7_END == m_eStage)
    {
        return FALSE;
    }

    if(m_eRound == byBigRound)
    {
        if(GVG_CHAOS_WAR == byBigRound || GVG_FINAL_WAR == byBigRound)
        {
            if(GVG_3_CHAOS_WAR_END == m_eStage)
            {
                return FALSE;
            }

            DT_GVG_ROUND stCurRound = GetGambleRound();
            if(1 == bySmallRound)
            {
                return FALSE;
            }

            if(bySmallRound > stCurRound.bySmallRound)
            {
                return TRUE;
            }
        }        
    }

    return FALSE;
}

BOOL CGvGBuild::IsGvGLogStart(UINT8 byBigRound, UINT8 bySmallRound)
{
    if(GVG_7_END == m_eStage)
    {
        return TRUE;
    }

    if(m_eRound < byBigRound)
    {
        return FALSE;
    }
    else if(m_eRound == byBigRound)
    {
        if((GVG_16_TO_8 == byBigRound && GVG_4_16_TO_8_END != m_eStage) ||
           (GVG_8_TO_4 == byBigRound  && GVG_5_8_TO_4_END != m_eStage))
        {
            return FALSE;
        }

        if(GVG_CHAOS_WAR == byBigRound || GVG_FINAL_WAR == byBigRound)
        {
            if(GVG_3_CHAOS_WAR_END == m_eStage)
            {
                return TRUE;
            }
            else if(GVG_6_FINAL_WAR_BEGIN == m_eStage)
            {
                return FALSE;
            }

            DT_GVG_ROUND stCurRound = GetGambleRound();
            if(bySmallRound >= stCurRound.bySmallRound)
            {
                return FALSE;
            }
        }        
    }

    return TRUE;
}

UINT16 CGvGBuild::OpenGvGLog(PKT_CLIGS_OPEN_GVG_LOG_ACK &stAck, CPlayer &oPlayer, UINT32 dwFactionID)
{
    BOOL bMyselfFaction = (0 == dwFactionID);
    if (bMyselfFaction)
    {
        dwFactionID = oPlayer.GetFactionID();
        if(0 == dwFactionID)
        {
            return ERR_OPEN_GVG_LOG::ID_NOT_JOIN_FACTION;
        }

        CPlayerGVG *poPlayerGvG = oPlayer.GetPlayerGVG();
        if(poPlayerGvG)
        {
            poPlayerGvG->ClearUnReadLogCnt();

            PKT_CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF stNtf;
            stNtf.byUnReadCnt = 0;
            oPlayer.SendMsg((CHAR*)&stNtf, CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF);
        }
    }

    DT_GVG_LOG_FACTION_INFO &stFlag = stAck.stLog.stFlag;

    // 先在本服中查找该门派，如果找得到，直接取该门派的历史战况，否则到本赛区的战况缓存中去取
    CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
    if(poFaction)
    {
        GetLocalFactionGvGLog(stAck.stLog, *poFaction);
    }
    else
    {
        GetGlobalFactionGvGLog(stAck.stLog, dwFactionID);
    }

    HideGvGLog(stAck.stLog.stLogList);
    stAck.dwRefreshTime = GetClientRefreshTime();
    return ERR_OPEN_GVG_LOG::ID_SUCCESS;
}

UINT16 CGvGBuild::GetGambleInfoAck(PKT_CLIGS_GVG_OPEN_GAMBLE_REQ& stReq, PKT_CLIGS_GVG_OPEN_GAMBLE_ACK& stAck, CPlayerGVG* poGvG)
{
	SGvGFightResult stResult ;
	if ( NULL == poGvG)
	{
		return ERR_GVG_OPEN_GAMBLE::ID_IS_UNOPEN;
	}

	if( !GetGvGFightResult(stReq.qwGambleID, stResult) )
	{
		return ERR_GVG_OPEN_GAMBLE::ID_NOT_HAVE_BATTLE;
	}

	if ( GR_NOT_START != stResult.eResult)
	{
		return ERR_GVG_OPEN_GAMBLE::ID_ALREAD_BATTLE;
	}

	BOOL bRet = CGvGFactionMgr::Instance()->GetFactionGambleInfo(stResult.dwFactionID1, stAck.stFactionInfo1);
	if ( !bRet)
	{
		return ERR_GVG_OPEN_GAMBLE::ID_NOT_FIND_FACTION;
	}
	bRet = CGvGFactionMgr::Instance()->GetFactionGambleInfo(stResult.dwFactionID2, stAck.stFactionInfo2);
	if ( !bRet)
	{
		return ERR_GVG_OPEN_GAMBLE::ID_NOT_FIND_FACTION;
	}
	CGvGPropMgr::Instance()->GetGambleReward(stAck.stWinRes, stResult.eBigRound, poGvG->GetOwner()->GetLevel());
	stAck.qwVideoID = stReq.qwGambleID;

    stAck.stFactionInfo1.dwSumIntegral = GetDisplayFactionScore(stResult.dwFactionID1);
    stAck.stFactionInfo2.dwSumIntegral = GetDisplayFactionScore(stResult.dwFactionID2);

	return ERR_GVG_OPEN_GAMBLE::ID_SUCCESS;
}

void CGvGBuild::GetLastResult(DT_GVG_FACTION_RANK_LST_DATA &stLastResult)
{
    memset(&stLastResult, 0, sizeof(stLastResult));

	CMapFactionLastRankItr itr = m_mapLastResult.begin();
	UINT16 wIdx = 0;
	for( ; itr != m_mapLastResult.end(); itr++, wIdx++)
	{
		stLastResult.astFactionRank[wIdx] = m_mapLastResult[wIdx + 1];
	}
	stLastResult.wRankNum = wIdx;
}

UINT16 CGvGBuild::FactionApplyAck(CPlayer* poPlayer)
{
    if(!IsApplyTime())
    {
        return ERR_GVG_FACTION_APPLAY::ID_NOT_APPLAY_TIME;
    }

	CFaction* poFaction = poPlayer->GetFaction();	
	if ( NULL == poFaction)
	{
		return ERR_GVG_FACTION_APPLAY::ID_NOT_HEAD;
	}

    DBG_INFO(_SDT("[%s: %d]: CGvGBuild::FactionApplayAck: player[%u] of faction[%-8u] %s try to apply, <current stage = %u> session = %u."), 
        MSG_MARK, poPlayer->GetID(), poFaction->GetFactionID(), poFaction->GetDT_FACTION_BASE_DATA().aszFactionName, m_eStage, GetCurSession());

	em_Faction_Job eJob = poFaction->GetJob(poPlayer->GetID());
	if( eJob != em_Faction_MartialHead)
	{
		return ERR_GVG_FACTION_APPLAY::ID_NOT_HEAD;
	}

	UINT16 wRet = AddApplyFaction(poFaction->GetDT_FACTION_BASE_DATA().dwFactionID);
	if ( wRet != ERR_GVG_FACTION_APPLAY::ID_SUCCESS)
	{
		return wRet;
	}
	poFaction->ResetFactionGvGData();

    CRecordMgr::Instance()->RecordInfo(poPlayer->GetID(),  ERM_GVG_ACTIVITY, CRecordMgr::EGA_APPLY, 0, 0, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 
        GetCurSession(), poFaction->GetFactionID(), 0, 0, poFaction->GetDT_FACTION_BASE_DATA().aszFactionName);

    DBG_INFO(_SDT("[%s: %d]: CGvGBuild::FactionApplayAck: faction[%-8u] %s apply success, <current stage = %u> session = %u."), 
        MSG_MARK, poFaction->GetFactionID(), poFaction->GetDT_FACTION_BASE_DATA().aszFactionName, m_eStage, GetCurSession());

	return ERR_GVG_FACTION_APPLAY::ID_SUCCESS;
}

UINT16 CGvGBuild::OpenGVGApplyPanel(PKT_CLIGS_GVG_APPLAY_ACK& stAck, CPlayerGVG* poGvG)
{	
    if (NULL == poGvG)
	{
		return ERR_GVG_APPLAY::ID_IS_UNOPEN;
	}

    if (FALSE == IsOpenUp())
    {
        return ERR_GVG_APPLAY::ID_IS_UNOPEN;
    }

	GetCurSchedule(stAck.stScheduleInfo, poGvG);
	CMapFactionLastRankItr itr = m_mapLastResult.find(1);
	if ( itr != m_mapLastResult.end() )
	{
		stAck.stLastChampionInfo = itr->second;
	}

    stAck.byIsApplyTime = IsApplyTime();
    stAck.dwCountDown   = (stAck.byIsApplyTime ? GetNextStageCountDown() : 0);

	return ERR_GVG_APPLAY::ID_SUCCESS;
}

UINT16 CGvGBuild::OpenGvGMoneyWarAck(PKT_CLIGS_GVG_MONEY_CHAOS_ACK& stAck, CPlayerGVG* poGvG)
{
	if ( NULL == poGvG)
	{
		return ERR_OPEN_GVG_MONEY_CHAOS::ID_IS_UNOPEN;
	}
	//获取进度信息
	GetCurSchedule( stAck.stScheduleInfo, poGvG);
	//简易财力排行
	GetSimpleMoneyRank(stAck.stMoneyRank);
	//面板ID
	stAck.byCurPanel = 1;//财力比拼面板
	//获取倒计时
	stAck.qwCountDwonTime = GetDisplayCountDownTime();
	
	return ERR_GVG_MONEY_CHAOS::ID_SUCCESS;
}

UINT16 CGvGBuild::OpenGvGChaosWarAck(PKT_CLIGS_GVG_MONEY_CHAOS_ACK& stAck, CPlayerGVG* poGvG)
{
	if ( NULL == poGvG)
	{
		return ERR_GVG_MONEY_CHAOS::ID_IS_UNOPEN;
	}

    //获取进度信息
    GetCurSchedule(stAck.stScheduleInfo, poGvG);
    //获取倒计时
    stAck.qwCountDwonTime = GetDisplayCountDownTime();

    if(m_eStage > GVG_2_MONEY)
    {
        //获取财力比拼结果
        GetMoneyWarResult(stAck.stMoneyInfo);
    }    

    BOOL bChaosWarEnd = IsChaosWarEnd();

	//大乱斗进行中
	if(!bChaosWarEnd)
	{
		//面板ID
		stAck.byCurPanel = 2;//财力比拼结束后，大乱斗还未开始
	
        //第几小轮轮大轮斗
		stAck.bySmallRound = GetGambleRound().bySmallRound;
	}
    else
    {
        //面板ID
        stAck.byCurPanel = 3;//大乱斗结束

        //获取大乱斗结果
        GetChaosWarResult(stAck.stBattleInfo);
    }

    return ERR_GVG_MONEY_CHAOS::ID_SUCCESS;
}

UINT64 CGvGBuild::GetStageTime(GVG_STAGE eStage)
{
    return CGvGPropMgr::Instance()->GetStageStartTime(m_stGvGBuildData.qwStartTime, eStage);
}

UINT32 CGvGBuild::GetDisplayCountDownTime()
{
    UINT32 dwDisplayCountDown = 0;

    GVG_STAGE eNextStage = GVG_0_NOT_START;
    switch(m_eStage)
    {
    case GVG_2_MONEY:
        eNextStage = GVG_3_CHAOS_WAR_BEGIN;
        break;

    case GVG_3_CHAOS_WAR_BEGIN:
        if(IsChaosWarEnd())
        {
            eNextStage = GVG_4_16_TO_8_BEGIN;
        }
        else
        {
            dwDisplayCountDown = GetNextStageCountDown();
        }
        break;

    case GVG_3_CHAOS_WAR_END:
        eNextStage = GVG_4_16_TO_8_BEGIN;
        break;

    case GVG_4_16_TO_8_BEGIN:
    case GVG_4_16_TO_8_GO:
        eNextStage = GVG_4_16_TO_8_END;
        break;

    case GVG_4_16_TO_8_END:
    case GVG_5_8_TO_4_BEGIN:
    case GVG_5_8_TO_4_GO:
        eNextStage = GVG_5_8_TO_4_END;
        break;

    case GVG_5_8_TO_4_END:
        eNextStage = GVG_6_FINAL_WAR_BEGIN;
        break;

    case GVG_6_FINAL_WAR_BEGIN:
        eNextStage = GVG_6_FINAL_WAR_GO;
        break;

    case GVG_6_FINAL_WAR_GO:
        if(IsFinalWarEnd())
        {
            eNextStage = GVG_7_END;
        }
        else
        {
            dwDisplayCountDown = GetNextStageCountDown();
        }
        break;
    }

    if(0 == dwDisplayCountDown)
    {
        UINT64 qwNow = SDTimeSecs();

        UINT64 qwNextStageTime = GetStageTime(eNextStage);
        dwDisplayCountDown = (UINT32)(qwNow < qwNextStageTime ? qwNextStageTime - qwNow : 0);
    }    

    return dwDisplayCountDown;
}

GvGRound CGvGBuild::GetNextRoundAt16To8To4Panel()
{
    GvGRound eNextRound = GVG_SLEEPING;

    switch(m_eStage)
    {
    case GVG_4_16_TO_8_BEGIN:
    case GVG_4_16_TO_8_GO:
        eNextRound = GVG_16_TO_8;
        break;

    case GVG_4_16_TO_8_END:
    case GVG_5_8_TO_4_BEGIN:
    case GVG_5_8_TO_4_GO:
        eNextRound = GVG_8_TO_4;
        break;

    case GVG_5_8_TO_4_END:
        eNextRound = GVG_FINAL_WAR;
        break;
    }

    return eNextRound;
}

UINT16 CGvGBuild::OpenGVG16To8To4Ack(CPlayer* poPlayer, PKT_CLIGS_16_TO_8_TO_4_ACK& stAck)
{
	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	if ( NULL == poGvG)
	{
		return ERR_16_TO_8_TO_4::ID_IS_UNOPEN;
	}
    GetCurSchedule(stAck.stScheduleInfo, poGvG);

    DT_16_TO_8_TO_4_WAR_INFO &stWar16To8To4Info = m_stGvGBuildData.stWar16To8To4Info;

    for(UINT16 i = 0; i < stWar16To8To4Info.byFactionCnt && i < GVG_16_TO_8_FACTION_CNT; i++)
    {
        DT_FACTION_BASE_DATA_CLI2 &stFactionCli = stAck.astFactionList[i];
        DT_GVG_FACTION &stFaction = stWar16To8To4Info.astFactionList[i];

        if(0 == stFaction.stBaseData.dwFactionID)
        {
            continue;
        }

        stFactionCli = stFaction.stBaseData;
    }
    
    stAck.dwCountDownTime = GetDisplayCountDownTime();
    stAck.byNextRound     = GetNextRoundAt16To8To4Panel();
    stAck.byWin4Cnt       = m_stGvGBuildData.stWar16To8To4Info.byWin4Cnt;
    stAck.byWin8Cnt       = m_stGvGBuildData.stWar16To8To4Info.byWin8Cnt;
    
    // DBG_INFO(_SDT("[%s: %d]: CGvGBuild::OpenGVG16To8To4Ack, stAck.byNextRound = %u"), MSG_MARK, stAck.byNextRound);

    for(UINT16 i = 0; i < GVG_16_TO_8_WIN_CNT; i++)
    {
        stAck.aqwVideoIDList16To8[i] = stWar16To8To4Info.aqwVideoIDList16To8[i];
    }
    for(UINT16 i = 0; i < stAck.byWin8Cnt; i++)
    {
        stAck.abyWin8List[i] = stWar16To8To4Info.abyWin8List[i];
    }

    for(UINT16 i = 0; i < GVG_8_TO_4_WIN_CNT; i++)
    {
        stAck.aqwVideoIDList8To4[i] = stWar16To8To4Info.aqwVideoIDList8To4[i];
    }    
    for(UINT16 i = 0; i < stAck.byWin4Cnt; i++)
    {
        stAck.abyWin4List[i] = stWar16To8To4Info.abyWin4List[i];
    }

    DT_GVG_ROUND stRound = GetGambleRound();
    switch(m_eStage)
    {
    case GVG_4_16_TO_8_BEGIN:
    case GVG_4_16_TO_8_GO:
        stAck.byWin8Cnt = 0;
        stAck.byWin4Cnt = 0;
        if(poGvG->IsGamble(stRound))
        {
            memset(stAck.aqwVideoIDList16To8, 0, sizeof(stAck.aqwVideoIDList16To8));
        }
        break;

    case GVG_4_16_TO_8_END:
    case GVG_5_8_TO_4_BEGIN:
    case GVG_5_8_TO_4_GO:
    {
        DT_GVG_ROUND stRound8To4;
        stRound8To4.byBigRound   = GVG_8_TO_4;
        stRound8To4.bySmallRound = 0;

        stAck.byWin4Cnt = 0;
        if(poGvG->IsGamble(stRound8To4))
        {
            memset(stAck.aqwVideoIDList8To4, 0, sizeof(stAck.aqwVideoIDList8To4));
        }
        break;
    }        
    }

    if(GVG_5_8_TO_4_END != m_eStage )
    {
        if(GVG_4_16_TO_8_END == m_eStage)
        {
            stRound.byBigRound = GVG_8_TO_4;
        }

        UINT32 dwGamebleFactionID = poGvG->GetGambleFactionID(stRound);
        if(dwGamebleFactionID > 0)
        {
            for (UINT16 i = 0; i < stWar16To8To4Info.byFactionCnt; i++)
            {
                if(stAck.astFactionList[i].dwFactionID == dwGamebleFactionID)
                {
                    stAck.byGamblePos = i + 1;
                    break;
                }
            }
        }
    }     

    return ERR_16_TO_8_TO_4::ID_SUCCESS;
}

UINT16 CGvGBuild::OpenGVGFinalWarAck(CPlayer* poPlayer, PKT_CLIGS_FINAL_WAR_ACK& stAck)
{
	CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
	if ( NULL == poGvG)
	{
		return ERR_FINAL_WAR::ID_IS_UNOPEN;
	}

    GetCurSchedule(stAck.stScheduleInfo, poGvG);
    stAck.dwCountDwonTime = GetDisplayCountDownTime();

    DT_FINAL_WAR_INFO &stFinalWarInfo = m_stGvGBuildData.stFinalWarInfo;

    stAck.byTotalStage = (stFinalWarInfo.byFactionCnt > 0 ? stFinalWarInfo.byFactionCnt - 1 : 0);
    stAck.byTotalStage = MAX(1, stAck.byTotalStage);
    stAck.byFactionCnt = stFinalWarInfo.byFactionCnt;

    BOOL bFinalWarEnd = IsFinalWarEnd();

    DT_GVG_ROUND stRound = GetGambleRound(); 

    BOOL bNotStarted = ((m_eStage == GVG_6_FINAL_WAR_BEGIN) || 
                        (stRound.bySmallRound <= 1 && GVG_6_FINAL_WAR_GO == m_eStage)); // || (m_stGvGBuildData.stFinalWarInfo.byTotalRound + 1 < m_stGvGBuildData.stFinalWarInfo.byFactionCnt));

    // 总决赛未开始
    if(bNotStarted)
    {
        stAck.byPassedStageCnt = 0;

        // 获取第一轮时每个门派的信息
        DT_FINAL_WAR_ROUND_INFO &stFirstRoundInfo = stFinalWarInfo.astEachRoundInfo[0];
        for(UINT8 byFactionIdx = 1; byFactionIdx <= stAck.byFactionCnt; byFactionIdx++)
        {
            DT_GVG_FACTION &stFaction = stFinalWarInfo.astFactionList[byFactionIdx - 1];
            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFirstRoundFaction = stFirstRoundInfo.astFactionList[byFactionIdx - 1];

            DT_FINAL_WAR_FACTION_INFO_CLI &stFactionCli = stAck.astFactionList[byFactionIdx - 1];

            gvgutil::TakeDisplayFinalWarFaction(stFactionCli, stFaction);
            stFactionCli.byRank    = byFactionIdx;
            stFactionCli.dwScore   = 0;
            stFactionCli.qwVideoID = stFirstRoundFaction.qwVideoID;
        }

        stAck.byCurStage = (GVG_6_FINAL_WAR_GO == m_eStage ? 1 : 0);

        // 因为是针对第一轮的押注，所以把押注轮次设为1
        DT_GVG_ROUND stFirstRound = stRound;
        stFirstRound.bySmallRound = 1;
        stAck.dwGambleFactionID = poGvG->GetGambleFactionID(stFirstRound);
    }
    // 总决赛正在进行中
    else if(!bFinalWarEnd)
    {
        // 根据前一轮的积分从大到小进行显示，而对手则按本轮的来显示
        // 如：本轮第三轮，则显示第二轮的积分排名结果，但显示第三轮的对手
        stAck.byPassedStageCnt = MIN(stFinalWarInfo.byTotalRound, stRound.bySmallRound - 1);

        UINT8 byPreRound = (stRound.bySmallRound >= 2 ? stRound.bySmallRound - 2 : 0);
        UINT8 byCurRound = (stRound.bySmallRound >= 1 ? stRound.bySmallRound - 1 : 0);
        DT_FINAL_WAR_ROUND_INFO &stPreRoundInfo = stFinalWarInfo.astEachRoundInfo[byPreRound];
        DT_FINAL_WAR_ROUND_INFO &stCurRoundInfo = stFinalWarInfo.astEachRoundInfo[byCurRound];

        for(UINT8 byRank = 1; byRank <= stPreRoundInfo.byFactionCnt; byRank++)
        {
            UINT8 byFactionPos = stPreRoundInfo.abyRankList[byRank - 1];
            if(0 == byFactionPos)
            {
                continue;
            }

            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stPreRoundFaction = stPreRoundInfo.astFactionList[byFactionPos - 1];
            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stCurRoundFaction = stCurRoundInfo.astFactionList[byFactionPos - 1];

            DT_GVG_FACTION &stFaction = m_stGvGBuildData.stFinalWarInfo.astFactionList[byFactionPos - 1];
            DT_FINAL_WAR_FACTION_INFO_CLI &stFactionCli = stAck.astFactionList[byRank - 1];

            gvgutil::TakeDisplayFinalWarFaction(stFactionCli, stFaction);
            stFactionCli.byRank    = byRank;
            stFactionCli.dwScore   = stPreRoundFaction.wScore;
            stFactionCli.qwVideoID = stCurRoundFaction.qwVideoID;
        }

        stAck.byCurStage = 1;
        stAck.dwGambleFactionID = poGvG->GetGambleFactionID(stRound);
    }
    else
    {
        stAck.byPassedStageCnt = stFinalWarInfo.byTotalRound;

        DT_FINAL_WAR_ROUND_INFO &stFinalRound = stFinalWarInfo.astEachRoundInfo[stFinalWarInfo.byTotalRound - 1];
        for(UINT8 byRank = 1; byRank <= stFinalRound.byFactionCnt; byRank++)
        {
            UINT8 byFactionPos = stFinalRound.abyRankList[byRank - 1];
            if(0 == byFactionPos)
            {
                continue;
            }

            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stCurRoundFaction = stFinalRound.astFactionList[byFactionPos - 1];
            DT_GVG_FACTION &stFaction = m_stGvGBuildData.stFinalWarInfo.astFactionList[byFactionPos - 1];

            DT_FINAL_WAR_FACTION_INFO_CLI &stFactionCli = stAck.astFactionList[byRank - 1];

            gvgutil::TakeDisplayFinalWarFaction(stFactionCli, stFaction);
            stFactionCli.byRank    = byRank;
            stFactionCli.dwScore   = stCurRoundFaction.wScore;
            stFactionCli.qwVideoID = stCurRoundFaction.qwVideoID;
        }

        stAck.byCurStage = 2;
    }

    for (UINT16 i = 0; i < stAck.byFactionCnt; i++)
    {
        DT_FINAL_WAR_FACTION_INFO_CLI &stCliFaction = stAck.astFactionList[i];
        if(0 == stCliFaction.dwFactionID || '\0' == stCliFaction.aszFactionName[0])
        {
            continue;
        }
    }

    // DBG_INFO(_SDT("[%s: %d]: CGvGBuild::OpenGVGFinalWarAck count down time = %u, current stage = %u, client state = %u"), MSG_MARK, stAck.dwCountDwonTime, m_eStage, stAck.byCurStage);

    // std::sort(stAck.astFactionList, stAck.astFactionList + GVG_FINAL_WAR_FACTION_CNT, gvgutil::SortFactionByScoreDesc);
    return ERR_FINAL_WAR::ID_SUCCESS;
}

UINT16 CGvGBuild::OpenGVGChampionAck(CPlayer* poPlayer, PKT_CLIGS_GVG_CHAMPION_ACK& stAck)
{
    CPlayerGVG* poGvG = poPlayer->GetPlayerGVG();
    if (NULL == poGvG || FALSE == IsOpenUp())
    {
        return ERR_FINAL_WAR::ID_IS_UNOPEN;
    }

    GetCurSchedule(stAck.stScheduleInfo, poGvG);
    GetLastResult(stAck.stNo123);
    stAck.stNo123.wRankNum = MIN(3, stAck.stNo123.wRankNum);

    return ERR_FINAL_WAR::ID_SUCCESS;
}

void CGvGBuild::OpenGVGNotOpenMsgAck(CPlayer* poPlayer, PKT_CLIGS_GVG_ERR_MSG_ACK &stAck)
{
    ZoneID localZoneID = gsapi::GetZoneID();
    UINT64 qwOpenDate = CGvGPropMgr::Instance()->GetOpenDateByZone(localZoneID);

    if (0 == qwOpenDate)
    {
        // 本赛区仍未配置好将在第几届开放门派跨服战
        std::string strMsg = CMsgDefMgr::Instance()->GetErrMsg("GVG_ACTIVITY_IS_CLOSED");
        SDStrncpy(stAck.aszErrMsg, strMsg.c_str(), MAX_GVG_ERR_MSG_LEN - 1);
    }
    else
    {
        // 本赛区已配置好将在第几届开放门派跨服战
        CSDDateTime oOpenDate(qwOpenDate);

        vector<string> vecFillMsg;
        CHAR szBuff[32] = {0};
        sprintf(szBuff, "%u", oOpenDate.GetMonth());
        vecFillMsg.push_back(szBuff);
        sprintf(szBuff, "%u", oOpenDate.GetDay());
        vecFillMsg.push_back(szBuff);

        std::string strMsg = CMsgDefMgr::Instance()->GetErrMsg("GVG_ACTIVITY_WILL_OPEN", &vecFillMsg);
        SDStrncpy(stAck.aszErrMsg, strMsg.c_str(), MAX_GVG_ERR_MSG_LEN - 1);
    }
}

BOOL CGvGBuild::ApplyGVGGame(UINT32 dwFactionID)
{
	if ( GVG_1_APPLY != GetCurStage())
	{
		return FALSE;
	}

	CFaction* poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
	if ( NULL == poFaction)
	{
		return FALSE;
	}

	AddApplyFaction(dwFactionID);
    poFaction->ReCalcSumPower();
    
    poFaction->AddPlayerTosnapshot();
	return TRUE;
}

UINT16 CGvGBuild::AddApplyFaction(UINT32 dwFactionID, UINT64 qwStartTime)
{
	//已报名
	if ( IsApply(dwFactionID) )
	{
		return ERR_GVG_FACTION_APPLAY::ID_ALREAD_APPLAY;
	}
	CFaction* poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
	if ( NULL == poFaction)
	{
		return ERR_GVG_FACTION_APPLAY::ID_NOT_HEAD;
	}
	//检查门派等级
	if ( poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel < CGvGPropMgr::Instance()->GetJoinGVGFactionLevel())
	{
		return ERR_GVG_FACTION_APPLAY::ID_LEVEL_NOT_ENOUGH;
	}
	//排序
	CFactionMgr::Instance()->FactionSort();
	//检查门派排名
	if ( poFaction->GetDT_FACTION_BASE_DATA().dwFactionRank > CGvGPropMgr::Instance()->GetJoinGvGRank() )
	{
		return ERR_GVG_FACTION_APPLAY::ID_RANK_NOT_ENOUGH;
	}

	DT_FACTION_APPLAY_DATA stData = {0};
	stData.dwFactionID = dwFactionID;
	stData.qwApplayTime = qwStartTime;
	m_stFactionApplayDataLst.push_back(stData);
	m_setApplyFaction.insert(dwFactionID);

    poFaction->AutoFormation(CGvGPropMgr::Instance()->GetGvGProp().wAutoFormationCnt);
	return ERR_OPEN_GVG_FORMATION::ID_SUCCESS;
}

VOID CGvGBuild::ResetApplayFaction()
{
	m_stFactionApplayDataLst.clear();
	m_setApplyFaction.clear();
}

VOID CGvGBuild::AddResult(UINT16 wRank, DT_GVG_FACTION_RANK_DATA& stInfo)
{
	CMapFactionLastRankItr itr =  m_mapLastResult.find(wRank);
	if ( itr != m_mapLastResult.end())
	{
		return ;
	}
	m_mapLastResult[wRank] = stInfo;
}

VOID CGvGBuild::ResetResult()
{
	m_mapLastResult.clear();
}

//重置财力排行结果
VOID CGvGBuild::ResetMoneyRank()
{
	m_mapMoneyResult.clear();
}

void CGvGBuild::ResetFactionFunds()
{
    CMapFaction &mapFaction = CFactionMgr::Instance()->GetFactions();
    for(CMapFaction::iterator itr = mapFaction.begin(); itr != mapFaction.end(); ++itr){
        CFaction *poFaction = itr->second;
        if (poFaction){
            poFaction->ResetGvGFactionFunds();
        }
    }
}

//添加财力排行结果
VOID CGvGBuild::AddMoneyRank(UINT16 wRank, DT_SIMPLE_FACTION_INFO& stInfo)
{
	CMapSimpleFactionItr itr =  m_mapMoneyResult.find(wRank);
	if ( itr != m_mapMoneyResult.end())
	{
		return ;
	}
	m_mapMoneyResult[wRank] = stInfo;
}

//重置大乱斗结果
VOID CGvGBuild::ResetChaosResult()
{
	m_mapChaosResult.clear();

    memset(&m_stGvGBuildData.stChaosWar.stChaosResult, 0, sizeof(m_stGvGBuildData.stChaosWar.stChaosResult));
}

VOID CGvGBuild::ResetChaosPair()
{
    DT_LOCAL_WAR_FACTION_SEED_LIST &stFactionSeedList = m_stGvGBuildData.stChaosWar.stFactionSeedList;
    memset(&stFactionSeedList, 0, sizeof(stFactionSeedList));
}

//添加大乱斗结果
VOID CGvGBuild::AddChaosResult(UINT16 wRank, DT_SIMPLE_FACTION_INFO& stInfo)
{
	m_mapChaosResult[wRank] = stInfo;
}

BOOL CGvGBuild::IsMoneyGame(UINT32 dwFactionID)
{
	GVG_STAGE eStage = GetCurStage();
	if( GVG_2_MONEY == eStage )
	{
        return IsApply(dwFactionID);
	}

	return FALSE;
}

VOID CGvGBuild::GetMoneyRank(DT_GVG_RANK_MONEY_LST_DATA& stMoneyData)
{
	GVG_STAGE eStage = GetCurStage();
	if( GVG_2_MONEY == eStage )
	{
		m_stFactionApplayDataLst.sort(SortByMoney);
	}
	CFactionApplayDataLstItr itr = m_stFactionApplayDataLst.begin();
	CFaction* poFaction = NULL;
	UINT16 wIdx = 0;
	for ( ; itr != m_stFactionApplayDataLst.end(); itr++ )
	{
		poFaction = CFactionMgr::Instance()->GetFaction(itr->dwFactionID);
		if ( NULL != poFaction)
		{
			stMoneyData.astMoneyRank[wIdx].wRank = wIdx + 1;
			stMoneyData.astMoneyRank[wIdx].qwFactionFunds = poFaction->GetFactionGVGFunds();
			stMoneyData.astMoneyRank[wIdx].qwSumPower = poFaction->GetSumPower();
			poFaction->GetFactionBaseDataCli2(stMoneyData.astMoneyRank[wIdx].stFactionBaseInfo);
			stMoneyData.astMoneyRank[wIdx].stFactionBaseInfo.dwFactionRank = wIdx + 1;
			wIdx++;
		}
	}
	stMoneyData.wRankNum = wIdx;
}

VOID CGvGBuild::GetSimpleMoneyRank(DT_GVG_SIMPLE_RANK_MONEY_LST_DATA& stMoneyData)
{
	GVG_STAGE eStage = GetCurStage();
	if( GVG_2_MONEY == eStage )
	{
		m_stFactionApplayDataLst.sort(SortByMoney);
	}
	CFactionApplayDataLstItr itr = m_stFactionApplayDataLst.begin();
	CFaction* poFaction = NULL;
	UINT16 wIdx = 0;
	for ( ; itr != m_stFactionApplayDataLst.end(); itr++ )
	{
		poFaction = CFactionMgr::Instance()->GetFaction(itr->dwFactionID);
		if ( NULL != poFaction)
		{
			stMoneyData.astMoneyRank[wIdx].wRank = wIdx + 1;
			stMoneyData.astMoneyRank[wIdx].qwFactionFunds = poFaction->GetFactionGVGFunds();
			SDStrncpy(stMoneyData.astMoneyRank[wIdx].aszFactionName, poFaction->GetDT_FACTION_BASE_DATA().aszFactionName, MAX_FACTION_NUM - 1);
			wIdx++;
		}
	}
	stMoneyData.wRankNum = wIdx;
}

BOOL CGvGBuild::GetCurSchedule(DT_GVG_SCHEDULE_INFO& stScheduleInfo, CPlayerGVG* poGvG)
{
    stScheduleInfo = CGvGPropMgr::Instance()->GetGvGSchedule();
    stScheduleInfo.byCurRound = m_eRound;
    stScheduleInfo.dwRefreshTime = GetClientRefreshTime();
	stScheduleInfo.wSessionID = GetCurSession();

    if (GVG_8_SLEEPING == m_eStage)
    {
        // 假装成已经是下一届
        stScheduleInfo.wSessionID++;
    }

	if ( NULL == poGvG)
	{
		return FALSE;
	}
	
    stScheduleInfo.byGetGiftNum   = poGvG->GetUnGetGiftNum();
    stScheduleInfo.byUnReadLogCnt = (UINT8)poGvG->GetUnReadLogCnt();
	CPlayer* poPlayer = poGvG->GetOwner();
	if ( NULL == poPlayer)
	{
		return FALSE;
	}
	CFaction* poFaction = poPlayer->GetFaction();
	if ( NULL == poFaction)
	{
		//没有门派
		stScheduleInfo.byFormationState = 1;
	}
	else if (GVG_7_END == GetCurStage() || IsFinalWarEnd())
	{
		//本届已结束
		stScheduleInfo.byFormationState = 6;
	}
	else if ( !IsApply(poFaction->GetDT_FACTION_BASE_DATA().dwFactionID) )
	{
		//未报名
		stScheduleInfo.byFormationState = 2;
	}
	else if ( IsInWar() )
	{
		//比赛进行中
		stScheduleInfo.byFormationState = 5;
	}
	else if ( IsWillWar() )
	{
		//不是布阵时间
		stScheduleInfo.byFormationState = 5;
	}
	else if ( em_Faction_MartialHead != poFaction->GetJob(poPlayer->GetID() ))
	{
		//没有权限
		stScheduleInfo.byFormationState = 4;
	}
	
    if(GVG_8_SLEEPING == GetCurStage())
    {
        // 2014-07-10 这里设置byFormationState = 3是因为客户端用错了byFormationState状态，今天刚提交客户端版本，客户端改不了，所以只能服务端改了。。。
        stScheduleInfo.byFormationState = 3;
    }

    poGvG->GetGambleReward();
	return TRUE;
}

VOID CGvGBuild::GetMoneyWarResult(DT_SIMPLE_FACTION_LST_INFO& stLstInfo)
{
    memset(&stLstInfo, 0, sizeof(stLstInfo));
    if(m_mapMoneyResult.empty())
    {
        return;
    }

	for ( UINT8 byIdx = FIRST_RANK; byIdx <= LAST_RNAK; byIdx++)
	{
        DT_SIMPLE_FACTION_INFO &stFactionInfo = stLstInfo.astFactionInfo[stLstInfo.wFactionNum];
        stFactionInfo = m_mapMoneyResult[byIdx];

        if(0 == stFactionInfo.stFactionBaseInfo.dwFactionID)
        {
            continue;
        }

        stLstInfo.wFactionNum++;
	}
}

VOID CGvGBuild::GetChaosWarResult(DT_SIMPLE_FACTION_LST_INFO& stLstInfo)
{
    memset(&stLstInfo, 0, sizeof(stLstInfo));
    if(m_mapChaosResult.empty())
    {
        return;
    }

	for ( UINT8 byIdx = FIRST_RANK; byIdx <= LAST_RNAK; byIdx++)
	{
        DT_SIMPLE_FACTION_INFO &stFactionInfo = stLstInfo.astFactionInfo[stLstInfo.wFactionNum];
		stFactionInfo = m_mapChaosResult[byIdx];
        if(0 == stFactionInfo.stFactionBaseInfo.dwFactionID)
        {
            continue;
        }

        stLstInfo.wFactionNum++;
	}	
}

UINT32 CGvGBuild::GetClientRefreshTime()
{
    UINT32 dwNextCountDownTime = GetNextStageCountDown();
    if (GVG_6_FINAL_WAR_GO == m_eStage)
    {
        if(!IsFinalWarEnd())
        {
            SGvGProp &stGvGProp = CGvGPropMgr::Instance()->GetGvGProp();

            UINT64 qwNow = SDTimeSecs();
            UINT64 qwStageTime = GetStageTime(GVG_6_FINAL_WAR_GO);

            DT_GVG_ROUND stRound = GetGambleRound();
            UINT64 qwNextStageTime = qwStageTime + stGvGProp.dwFinalWarEachRoundTime * stRound.bySmallRound;
            if (qwNow > qwNextStageTime - stGvGProp.dwForbidFormationTime)
            {
                dwNextCountDownTime = (UINT32)(qwNextStageTime > qwNow ? qwNextStageTime - qwNow : 0);
            }
            else
            {
                dwNextCountDownTime = (UINT32)(qwNextStageTime - stGvGProp.dwForbidFormationTime - qwNow);
            }
        }
    }

    if(dwNextCountDownTime > 3)
    {
        // 这里特地把下次刷新时间除以2
        dwNextCountDownTime >>= 1;
    }

    if(0 == dwNextCountDownTime)
    {
        dwNextCountDownTime = 3;
    }

    // dwNextCountDownTime = 5;
    // dwNextCountDownTime = 1;
    // DBG_INFO(_SDT("[%s: %d]: CGvGBuild::GetRefreshTime() refresh time = %s, current stage = %u"), MSG_MARK, gvgutil::TimeToDayHourMinuteSec(dwNextCountDownTime).c_str(), m_eStage);
    return dwNextCountDownTime;
}

UINT32 CGvGBuild::GetNextStageCountDown()
{
    SGvGProp &stGvGProp = CGvGPropMgr::Instance()->GetGvGProp();

    GVG_STAGE eNextStage = gvgutil::GetNextStage(m_eStage);
    UINT64 qwNextStageTime = GetStageTime(eNextStage);

    switch(m_eStage)
    {
    /*
    case GVG_1_APPLY:
    {
        if (!IsApplyTime())
        {
            // 获取本届的报名时间
            qwNextStageTime = GetStartApplyTime();
        }
        
        break;
    }
    */
    case GVG_8_SLEEPING:
    {
        // 获取下次跨服战活动的开启日期
        UINT64 qwNextActivityOpenDate = CGvGPropMgr::Instance()->GetOpenDateBySession(m_stGvGBuildData.wSessionID + 1);

        // 下一轮的报名时间
        qwNextStageTime = CGvGPropMgr::Instance()->GetStageStartTime(qwNextActivityOpenDate, GVG_1_APPLY);
        break;
    }
    case GVG_3_CHAOS_WAR_BEGIN:
    {
        if(!IsChaosWarEnd())
        {
            UINT64 qwStageTime = GetStageTime(m_eStage);

            DT_GVG_ROUND stRound = GetGambleRound();
            qwNextStageTime = qwStageTime + stGvGProp.dwChaosWarEachRoundTime * stRound.bySmallRound;
        }

        break;
    }
    case GVG_6_FINAL_WAR_GO:
    {
        if(!IsFinalWarEnd())
        {
            UINT64 qwStageTime = GetStageTime(m_eStage);

            DT_GVG_ROUND stRound = GetGambleRound();
            qwNextStageTime = qwStageTime + stGvGProp.dwFinalWarEachRoundTime * stRound.bySmallRound;
        }
        
        break;
    }
    }

    UINT64 qwNow = SDTimeSecs();
    UINT32 dwCountDown = (UINT32)(qwNow < qwNextStageTime ? qwNextStageTime - qwNow : 0);
    return dwCountDown;
}

VOID CGvGBuild::AfterMoneyWar()
{
    ResetMoneyRank();
	m_stFactionApplayDataLst.sort(SortByMoney);
	CFactionApplayDataLstItr itr = m_stFactionApplayDataLst.begin();
	for ( UINT8 byIdx = FIRST_RANK; byIdx <= LAST_RNAK && itr != m_stFactionApplayDataLst.end(); byIdx++, itr++)
	{
		DT_SIMPLE_FACTION_INFO stInfo = {0};
		stInfo.dwZoneID = gsapi::GetZoneID();
		CFaction* poFaction = CFactionMgr::Instance()->GetFaction(itr->dwFactionID);
		if ( NULL == poFaction)
		{
			continue;
		}
		poFaction->GetFactionBaseDataCli2(stInfo.stFactionBaseInfo);
		stInfo.wRank = byIdx;
		AddMoneyRank( byIdx, stInfo);
	}
}

BOOL CGvGBuild::TakeLocalWarWinners(CFactionVec &vecTopFaction)
{
    ChaosWarGo();

    DT_SIMPLE_FACTION_LST_INFO stMoneyWarTop;
    DT_SIMPLE_FACTION_LST_INFO stChaosWarTop;

    GetMoneyWarResult(stMoneyWarTop);
    GetChaosWarResult(stChaosWarTop);

    if(!IsStageDone(GVG_3_CHAOS_WAR_BEGIN) || !IsLogicReady(GVG_3_CHAOS_WAR_BEGIN))
    {
        return FALSE;
    }

    FactionIDSet setWinners;

    for(UINT16 i = 0; i < stMoneyWarTop.wFactionNum && vecTopFaction.size() < MAX_GVG_TOP_FACTION_CNT; i++)
    {
        setWinners.insert(stMoneyWarTop.astFactionInfo[i].stFactionBaseInfo.dwFactionID);
    }

    for(UINT16 i = 0; i < stChaosWarTop.wFactionNum && vecTopFaction.size() < MAX_GVG_TOP_FACTION_CNT; i++)
    {
        setWinners.insert(stChaosWarTop.astFactionInfo[i].stFactionBaseInfo.dwFactionID);
    }

    for(FactionIDSet::iterator itr = setWinners.begin(); itr != setWinners.end(); ++itr)
    {
        UINT32 dwFactionID = *itr;
        CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
        if(poFaction)
        {
            vecTopFaction.push_back(poFaction);
        }
    }

    if(vecTopFaction.size() != (stMoneyWarTop.wFactionNum + stChaosWarTop.wFactionNum))
    {
        SYS_CRITICAL( _SDT("[%s: %d] CGvGBuild::TakeLocalWarWinners return faction count[%u] != %u + %u, there are some faction disappeared"), MSG_MARK, vecTopFaction.size(), stMoneyWarTop.wFactionNum, stChaosWarTop.wFactionNum);
    }

    return TRUE;
}

void CGvGBuild::SyncFactionSupport()
{
    if(m_mapFactionSupport.empty())
    {
        return;
    }

    PKT_GSPK_SUPPORT_FACTION_REQ stReq;
    memset(&stReq, 0, sizeof(stReq));

    DT_FACTION_SUPPORT_LIST &stFactionSupportInfo = stReq.stFactionSupportInfo;
    for(FactionSupportMap::iterator itr = m_mapFactionSupport.begin(); itr != m_mapFactionSupport.end(); ++itr)
    {
        DT_FACTION_SUPPORT_INFO &stSupport = stFactionSupportInfo.astSupportList[stFactionSupportInfo.dwFactionCnt++];
        stSupport.dwFactionID  = itr->first;
        stSupport.dwSupportCnt = itr->second;
    }

    stReq.byBigRound = m_eRound;
    stReq.dwArenaID = m_stGvGBuildData.wArenaID;

    m_mapFactionSupport.clear();
    m_dwRecentSupportCnt = 0;

    gsapi::SendMsg2PK((CHAR*)&stReq, GSPK_SUPPORT_FACTION_REQ);
}

void CGvGBuild::AddFactionSupport(UINT32 dwFactionID)
{
    ++m_mapFactionSupport[dwFactionID];
    ++m_dwRecentSupportCnt;

    DT_GVG_FACTION *pstGvGFaction = CGvGFactionMgr::Instance()->FindFactionInfo(dwFactionID);
    if(NULL == pstGvGFaction)
    {
        return;
    }

    pstGvGFaction->dwGambleCnt++;

    if(m_dwRecentSupportCnt >= MAX_SYNC_SUPPORT_FACTION_CNT)
    {
        SyncFactionSupport();
    }
}

BOOL CGvGBuild::IsFormationTime()
{
	if (GVG_4_16_TO_8_BEGIN == m_eStage
		|| GVG_5_8_TO_4_BEGIN == m_eStage 
		|| GVG_6_FINAL_WAR_BEGIN == m_eStage
		|| GVG_1_APPLY == m_eStage
		|| GVG_2_MONEY == m_eStage)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CGvGBuild::IsApply(UINT32 dwFactionID)
{
	if ( m_setApplyFaction.find(dwFactionID) != m_setApplyFaction.end())
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CGvGBuild::IsSeedFaction(UINT32 dwFactionID)
{
    FactionIDSet setSeed;

    for(UINT16 i = 0; i < m_stGvGBuildData.stWar16To8List.byWarCnt; i++)
    {
        const DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stGvGBuildData.stWar16To8List.astWar16To8To4Info[i];
        for(UINT16 wFactionIdx = 0; wFactionIdx < stWar16To8.byFactionCnt; wFactionIdx++)
        {
            const DT_GVG_FACTION &stFaction = stWar16To8.astFactionList[wFactionIdx];
            if(dwFactionID == stFaction.stBaseData.dwFactionID)
            {
                return TRUE;
            }
        }
    }

    const DT_FINAL_WAR_INFO &stFinalWar = m_stGvGBuildData.stFinalWarInfo;
    for(UINT16 wFactionIdx = 0; wFactionIdx < stFinalWar.byFactionCnt; wFactionIdx++)
    {
        const DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[wFactionIdx];
        if(dwFactionID == stFaction.stBaseData.dwFactionID)
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CGvGBuild::IsApplyTime()
{
    return (GVG_1_APPLY == m_eStage);
}

BOOL CGvGBuild::CanLeaveFaction(UINT32 dwFactionID)
{
    if (IsActivityTime() || m_eStage == GVG_7_END)
    {
        if(IsApply(dwFactionID) || IsSeedFaction(dwFactionID))
        {
            return FALSE;
        }
    }

    return TRUE;
}

UINT64 CGvGBuild::GetStartTime()
{
    return m_stGvGBuildData.qwStartTime;
}

UINT64 CGvGBuild::GetEndTime()
{
    return GetStageTime(GVG_7_END);
}

void CGvGBuild::TakeRequiredVideoIDList(VideoIDSet &mapVideoIDs)
{
    for(UINT16 i = 0; i < m_stGvGBuildData.stFactionsLog.wFactionCnt; i++)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stLogList = m_stGvGBuildData.stFactionsLog.astLogList[i];
        for(UINT16 wLogIdx = 0; wLogIdx < stLogList.wLogCnt; wLogIdx++)
        {
            DT_FACTION_SIMPLE_GVG_LOG &stLog = stLogList.astLogs[wLogIdx];
            if (EGLR_NOT_START != stLog.byLogResult)
            {
                mapVideoIDs.insert(stLog.qwVideoID);
            }
        }
    }

    mapVideoIDs.erase(0);
}

BOOL CGvGBuild::IsLogicReady(GVG_STAGE eStage)
{
    BOOL bReady = TRUE;
    switch(eStage)
    {
    case GVG_1_APPLY:
    case GVG_2_MONEY:
        bReady = TRUE; // !m_setApplyFaction.empty();
        break;

    case GVG_3_CHAOS_WAR_BEGIN:
        {
            m_setApplyFaction.erase(0);

            DT_SIMPLE_FACTION_LST_INFO stMoneyInfo;
            GetMoneyWarResult(stMoneyInfo);

            UINT32 dwApplyCnt = m_setApplyFaction.size();
            UINT32 dwRemailFactionCnt = (dwApplyCnt > stMoneyInfo.wFactionNum ? dwApplyCnt - stMoneyInfo.wFactionNum : 0);

            if(dwRemailFactionCnt > 0)
            {
                DT_LOCAL_WAR_FACTION_SEED_LIST &stFactionSeedList = m_stGvGBuildData.stChaosWar.stFactionSeedList;
                if(0 == stFactionSeedList.wFactionCnt)
                {
                    bReady = FALSE;
                    if(!bReady)
                    {
                        DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = GVG_3_CHAOS_WAR_BEGIN<%u>, dwApplyCnt = %u, dwRemailFactionCnt = %u, but stFactionSeedList.wFactionCnt = 0"), 
                            MSG_MARK, m_eStage, dwApplyCnt, dwRemailFactionCnt);
                    }
                }
                else
                {
                    DT_SIMPLE_FACTION_LST_INFO stChaosWinners;
                    memset(&stChaosWinners, 0, sizeof(stChaosWinners));
                    GetChaosWarResult(stChaosWinners);

                    bReady = (stChaosWinners.wFactionNum > 0);
                    if(!bReady)
                    {
                        DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = GVG_3_CHAOS_WAR_BEGIN<%u>, dwApplyCnt = %u, dwRemailFactionCnt = %u, stFactionSeedList.wFactionCnt = %u, but stChaosWinners.wFactionNum = 0"), 
                            MSG_MARK, m_eStage, dwApplyCnt, dwRemailFactionCnt, stFactionSeedList.wFactionCnt);
                    }
                }
            }
            else
            {
                bReady = TRUE;
            }

            break;
        }

    case GVG_3_CHAOS_WAR_END:
        {
            DT_LOCAL_WAR_FACTION_SEED_LIST &stFactionSeedList = m_stGvGBuildData.stChaosWar.stFactionSeedList;
            if(0 == stFactionSeedList.wFactionCnt)
            {
                bReady = TRUE;
            }
            else
            {
                DT_SIMPLE_FACTION_LST_INFO stChaosWinners;
                memset(&stChaosWinners, 0, sizeof(stChaosWinners));
                GetChaosWarResult(stChaosWinners);

                // 到了这一步，如果说上一轮满足的条件到这轮却不满足，说明可能上一轮的数据没被成功保存，这时候也没办法了，直接返回TRUE
                bReady = TRUE;
                if(0 == stChaosWinners.wFactionNum)
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = GVG_3_CHAOS_WAR_END<%u>, stFactionSeedList.wFactionCnt = %u, but stChaosWinners.wFactionNum = 0"), 
                        MSG_MARK, m_eStage, stFactionSeedList.wFactionCnt);
                }
            }

            break;
        }

    case GVG_4_16_TO_8_BEGIN:
    case GVG_4_16_TO_8_GO:
        {
            CFactionVec vecTopFaction;
            TakeLocalWarWinners(vecTopFaction);

            if(vecTopFaction.empty())
            {
                bReady = TRUE;
            }
            else
            {
                UINT32 dwSeedCnt = vecTopFaction.size();
                bReady = (m_stGvGBuildData.stWar16To8To4Info.byFactionCnt >= dwSeedCnt);
                if(!bReady)
                {
                    DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = %u, m_stGvGBuildData.stWar16To8To4Info.byFactionCnt = %u, dwSeedCnt = %u"), 
                        MSG_MARK, m_eStage, m_stGvGBuildData.stWar16To8To4Info.byFactionCnt, dwSeedCnt);
                }
            }

            break;
        }

    case GVG_4_16_TO_8_END:
    case GVG_5_8_TO_4_BEGIN:
    case GVG_5_8_TO_4_GO:
        bReady = (m_stGvGBuildData.stWar16To8To4Info.byWin8Cnt * 2 >= m_stGvGBuildData.stWar16To8To4Info.byFactionCnt);
        if(!bReady)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = %u, m_stGvGBuildData.stWar16To8To4Info.byWin8Cnt = %u, m_stGvGBuildData.stWar16To8To4Info.byFactionCnt = %u"), 
                MSG_MARK, m_eStage, m_stGvGBuildData.stWar16To8To4Info.byWin8Cnt, m_stGvGBuildData.stWar16To8To4Info.byFactionCnt);
        }
        break;

    case GVG_5_8_TO_4_END:
        bReady = (m_stGvGBuildData.stWar16To8To4Info.byWin4Cnt * 2 >= m_stGvGBuildData.stWar16To8To4Info.byWin8Cnt);
        if(!bReady)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = GVG_5_8_TO_4_END<%u>, m_stGvGBuildData.stWar16To8To4Info.byWin4Cnt = %u, m_stGvGBuildData.stWar16To8To4Info.byWin8Cnt = %u"), 
                MSG_MARK, m_eStage, m_stGvGBuildData.stWar16To8To4Info.byWin4Cnt, m_stGvGBuildData.stWar16To8To4Info.byWin8Cnt);
        }
        break;

    case GVG_6_FINAL_WAR_BEGIN:
        bReady = (m_stGvGBuildData.stFinalWarInfo.byFactionCnt >= m_stGvGBuildData.stWar16To8To4Info.byWin4Cnt);
        if(!bReady)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = %u, m_stGvGBuildData.stFinalWarInfo.byFactionCnt = %u, m_stGvGBuildData.stWar16To8To4Info.byWin4Cnt = %u"), 
                MSG_MARK, m_eStage, m_stGvGBuildData.stFinalWarInfo.byFactionCnt, m_stGvGBuildData.stWar16To8To4Info.byWin4Cnt);
        }
        break;

    case GVG_6_FINAL_WAR_GO:
        bReady = (m_stGvGBuildData.stFinalWarInfo.byTotalRound + 1 >= m_stGvGBuildData.stFinalWarInfo.byFactionCnt);
        if(!bReady)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsLogicReady = false, m_eStage = GVG_6_FINAL_WAR_END<%u>, m_stGvGBuildData.stFinalWarInfo.byTotalRound = %u, m_stGvGBuildData.stFinalWarInfo.byFactionCnt = %u"), 
                MSG_MARK, m_eStage, m_stGvGBuildData.stFinalWarInfo.byTotalRound, m_stGvGBuildData.stFinalWarInfo.byFactionCnt);
        }
        break;
    }

    return bReady;
}

BOOL CGvGBuild::IsReadyByStage(GVG_STAGE eStage)
{
    if(m_eStage != eStage)
    {
        DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsReadyByStage = false, current stage = %u != target stage = %u"), MSG_MARK, m_eStage, eStage);
        return FALSE;
    }

    if(m_eStage != m_stGvGBuildData.byCurStage)
    {
        DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsReadyByStage = false, m_eStage = %u < m_stGvGBuildData.byCurStage = %u, not equal"), MSG_MARK, m_eStage, m_stGvGBuildData.byCurStage);
        return FALSE;
    }

    if(!IsStageDone(eStage))
    {
        DBG_INFO(_SDT("[%s: %d]: CGvGBuild::IsReadyByStage = false, eStage = %u is not done"), MSG_MARK, m_eStage);
        return FALSE;
    }    

    BOOL bReady = IsLogicReady(eStage);
    return bReady;
}
