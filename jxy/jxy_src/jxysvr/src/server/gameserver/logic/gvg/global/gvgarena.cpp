///<------------------------------------------------------------------------------
//< @file:   gvg\global\gvgarena.cpp
//< @author: hongkunan
//< @date:   2014年5月5日 15:23:29
//< @brief:
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "gvgarena.h"
#include "logic/gvg/global/gvgmgr.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/gvg/local/gvgbuild.h"
#include "framework/gsapi.h"
#include "logic/gvg/gvgvideomgr.h"
#include "net/pk/pkpktbuilder.h"
#include "pullplayermgr.h"
#include "db/basedbccmd.h"
#include <sdloggerimpl.h>
#include "common/server/ngstring.h"

struct SFactionScore
{
    SFactionScore(){ memset(this, 0, sizeof(*this)); }

    UINT8  byFactionPos;  //该门派在门派列表中的位置
    UINT16 wScore;        //当前积分
    UINT64 qwSumPower;    //门派总战力
    UINT8 byWinCnt;       //赢的次数
};

typedef std::vector<SFactionScore> SFactionScoreVec;

namespace gvgutil
{
    typedef std::vector<DT_GVG_FACTION> SGvGFactionVec;

    void TakeFactionByZone(SGvGFactionVec &vecFaction, const DT_16_TO_8_TO_4_WAR_INFO &stWar16To8, UINT16 wZoneID)
    {
        for(UINT16 i = 0; i < stWar16To8.byFactionCnt; i++)
        {
            const DT_GVG_FACTION &stFaction = stWar16To8.astFactionList[i];
            if(stFaction.wZoneID == wZoneID)
            {
                vecFaction.push_back(stFaction);
            }
        }
    }

    // 把对决名单中区号小的门派排在左边
    void ArrangeFaction(DT_16_TO_8_TO_4_WAR_INFO &stWar16To8)
    {
        for (UINT16 bySwapPos = 0; bySwapPos < stWar16To8.byFactionCnt; bySwapPos += 2)
        {
            DT_GVG_FACTION &stFaction1 = stWar16To8.astFactionList[bySwapPos];
            DT_GVG_FACTION &stFaction2 = stWar16To8.astFactionList[bySwapPos + 1];

            if (stFaction1.stBaseData.dwFactionID <= stFaction2.stBaseData.dwFactionID)
            {
                continue;
            }

            const DT_GVG_FACTION stSwapFaction = stFaction1;
            stFaction1 = stFaction2;
            stFaction2 = stSwapFaction;
        }
    }

    void AppendFactionByZone(EachZoneFactionIDListMap &mapZones, const DT_GVG_FACTION &stFaction)
    {
        if(0 == stFaction.stBaseData.dwFactionID)
        {
            return;
        }

        mapZones[stFaction.wZoneID].insert(stFaction.stBaseData.dwFactionID);
    }

    DT_FACTION_SIMPLE_GVG_LOG* FindLogByRound(DT_FACTION_SIMPLE_GVG_LOG_LIST &stLogList, const DT_GVG_ROUND &stRound)
    {
        for (UINT16 i = 0; i < stLogList.wLogCnt; i++)
        {
            DT_FACTION_SIMPLE_GVG_LOG &stLog = stLogList.astLogs[i];
            if (stLog.stRound.byBigRound == stRound.byBigRound && stLog.stRound.bySmallRound == stRound.bySmallRound)
            {
                return &stLog;
            }
        }

        return NULL;
    }

    UINT16 CalcGvGAddScore(UINT32 dwFactionID, DT_GVG_VIDEO &stVideo)
    {
        if(0 == dwFactionID)
        {
            return 0;
        }

        DT_GVG_VIDEO_FACTION *pstFaction = (dwFactionID == stVideo.stG1.dwFactionID ? &stVideo.stG1 : NULL);
        if(NULL == pstFaction)
        {
            pstFaction = (dwFactionID == stVideo.stG2.dwFactionID ? &stVideo.stG2 : NULL);
        }
        
        if(NULL == pstFaction)
        {
            return 0;
        }

        BOOL bWin = FALSE;
        if((dwFactionID == stVideo.stG1.dwFactionID && stVideo.byBattleResult == GR_LEFT_WIN) ||
            (dwFactionID == stVideo.stG2.dwFactionID && stVideo.byBattleResult == GR_RIGHT_WIN))
        {
            bWin = TRUE;
        }

        UINT16 wAddScore = 0;
        if(bWin)
        {
            wAddScore = CGvGPropMgr::Instance()->GetGvGProp().wWinAddScore;
        }
        else
        {
            wAddScore = CGvGPropMgr::Instance()->GetGvGProp().wLoseAddScore;
        }

        for(UINT16 i = 0; i < pstFaction->stPlayerList.byPlayerCnt; i++)
        {
            UINT16 wKillCombo = pstFaction->stPlayerList.astPlayerList[i].byKillCombo;
            wAddScore += CGvGPropMgr::Instance()->CalcScoreByKillCombo((GvGRound)stVideo.stRound.byBigRound, wKillCombo);
        }

        return wAddScore;
    }

    // 根据门派积分、胜利次数、战力由高到低进行排序
    bool SortFactionByScoreDesc(const SFactionScore &stFaction1, const SFactionScore &stFaction2)
    {
        if(stFaction1.wScore > stFaction2.wScore)
        {
            return true;
        }
        else if(stFaction1.wScore < stFaction2.wScore)
        {
            return false;
        }
        else
        {
            if(stFaction1.byWinCnt > stFaction2.byWinCnt)
            {
                return true;
            }
            else if(stFaction1.byWinCnt < stFaction2.byWinCnt)
            {
                return false;
            }
            else
            {
                return stFaction1.qwSumPower > stFaction2.qwSumPower;
            }

        }

        return false;
    }

    // 对进入总决赛的门派排出名次，名次由门派积分、胜利次数、战力由高到低来决定
    void SortFaction(DT_FINAL_WAR_ROUND_INFO &stCurRoundInfo, const DT_GVG_FACTION stFactionList[])
    {
        SFactionScoreVec vecScore;
        vecScore.resize(stCurRoundInfo.byFactionCnt);

        for(UINT16 i = 0; i < stCurRoundInfo.byFactionCnt; i++)
        {
            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFactionInfo = stCurRoundInfo.astFactionList[i];
            const DT_GVG_FACTION &stGvGFaction = stFactionList[i];

            SFactionScore &stScore = vecScore[i];
            stScore.byFactionPos = i + 1;
            stScore.wScore       = stFactionInfo.wScore;
            stScore.qwSumPower   = stGvGFaction.qwSumPower;
            stScore.byWinCnt     = stGvGFaction.byWinCnt;
        }

        std::sort(vecScore.begin(), vecScore.end(), gvgutil::SortFactionByScoreDesc);

        for(UINT16 byRank = 1; byRank <= stCurRoundInfo.byFactionCnt; byRank++)
        {            
            stCurRoundInfo.abyRankList[byRank - 1] = vecScore[byRank - 1].byFactionPos;
        }
    }

    // 打印出区列表，仅用于日志跟踪
    CNGString GetZoneListStr(ZoneIDSet setZoneID)
    {
        CNGString strZoneList;
        for (ZoneIDSet::iterator itr = setZoneID.begin(); itr != setZoneID.end(); ++itr)
        {
            ZoneID zoneID = *itr;
            strZoneList += zoneID;
            strZoneList += ",";
        }

        if(strZoneList.m_strData.size() > 1)
        {
            strZoneList.m_strData.erase(strZoneList.m_strData.end() - 1);
        }
        return strZoneList;
    }

    // 根据区号获取到其所属的双服对战组ID
    BYTE GetGroupIDByZone(ZoneID zoneID, const DT_GROUP_ZONE_ID_LIST &stGroupZoneIDList)
    {
        for(UINT16 i = 0; i < stGroupZoneIDList.byGroupCnt; i++)
        {
            const DT_GROUP_ZONE &stGroupZone = stGroupZoneIDList.astGroupList[i];
            for(UINT16 i = 0; i < 2; i++)
            {
                if(zoneID == stGroupZone.awZoneIDList[i])
                {
                    return stGroupZone.byGroupID;
                }
            }
        }

        return 0;
    }

    // 四服积分总决赛中各个轮次中各门派的对决名单
    UINT8 abyFinalWarMapping[][GVG_FINAL_WAR_FACTION_CNT] =
    {
        {2, 1, 4, 3, 6, 5, 8, 7}, // 第一轮
        {3, 4, 1, 2, 7, 8, 5, 6}, // 第二轮
        {4, 7, 6, 1, 8, 3, 2, 5},
        {5, 6, 7, 8, 1, 2, 3, 4},
        {6, 5, 8, 7, 2, 1, 4, 3},
        {7, 8, 5, 6, 3, 4, 1, 2},
        {8, 3, 2, 5, 4, 7, 6, 1},
    };
}

BOOL CGvGArena::Init(const DT_GVG_ARENA_DATA &stArena)
{
    Clear();

    m_stArena = stArena;

    DT_FACTION_LIST_SIMPLE_GVG_LOG &stLogs = m_stArena.stFactionsLog;
    for(UINT16 i = 0; i < stLogs.wFactionCnt; i++)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stLog = stLogs.astLogList[i];
        if(0 == stLog.dwFactionID)
        {
            continue;
        }

        m_mapFactionLog[stLog.dwFactionID] = stLog;
    }

    SetStage((GVG_STAGE)stArena.byCurStage);
    BuildZone();    
    return TRUE;
}

BOOL CGvGArena::SaveToDB()
{
    if(0 == m_stArena.wArenaID)
    {
        return FALSE;
    }

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_GVG_ARENA);
    if(NULL == poCMD)
    {
        return FALSE;
    }

    gsapi::GetDBMgr()->AddCommand(poCMD);

    TakeDataToStruct();

    m_stArena.byIsDone = IsDone(m_eStage);
    poCMD->SetUserData(&m_stArena);

    return TRUE;
}

void CGvGArena::InitZoneList(GroupZoneIDMap& mapGroupZoneID)
{
    DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;
    memset(&stZoneIDList, 0, sizeof(stZoneIDList));

    DT_GROUP_ZONE_ID_LIST &stGroupList = m_stArena.stGroupZoneIDList;
    memset(&stGroupList, 0, sizeof(stGroupList));

    for (GroupZoneIDMap::iterator itr = mapGroupZoneID.begin(); itr != mapGroupZoneID.end(); ++itr)
    {
        ZoneIDVec &vecZoneID  = itr->second;

        DT_GROUP_ZONE &stGroupZone = stGroupList.astGroupList[stGroupList.byGroupCnt++];
        for(UINT16 i = 0; i < vecZoneID.size() && i < 2; i++)
        {
            stGroupZone.awZoneIDList[i] = vecZoneID[i];
            stZoneIDList.awZoneIDList[stZoneIDList.byZoneCnt++] = vecZoneID[i];
        }

        stGroupZone.byGroupID = itr->first;        
    }    
}

void CGvGArena::UpdateFactions(DT_GVG_FACTION astFactionList[], UINT16 dwFactionCnt)
{
    for (UINT16 i = 0; i < dwFactionCnt; i++)
    {
        DT_GVG_FACTION &stFaction = astFactionList[i];
        CPKFaction *poPKFaction = FindPKFaction(stFaction.wZoneID, stFaction.stBaseData.dwFactionID);
        if(NULL == poPKFaction)
        {
            continue;
        }

        DT_GVG_FACTION &stGvGFaction = poPKFaction->GetData().stBaseData;

        stFaction.byJoinCnt = stGvGFaction.byJoinCnt;
        stFaction.qwSumPower = stGvGFaction.qwSumPower;
        stFaction.stBaseData = stGvGFaction.stBaseData;
    }
}

void CGvGArena::ChaosWarEnd()
{
    m_stArena.stWar16To8List.byWarCnt = m_stArena.stGroupZoneIDList.byGroupCnt;
    GetEachZoneTop8();
}

void CGvGArena::AddNotStartLog(UINT32 dwFactionID1, UINT32 dwFactionID2, VideoID videoID, GvGRound eBigRound, UINT8 bySmallRound)
{
    if(dwFactionID1 == 0 && dwFactionID2 == 0)
    {
        return;
    }

    DT_FACTION_SIMPLE_GVG_LOG stLog;
    memset(&stLog, 0, sizeof(stLog));

    stLog.stRound.byBigRound   = eBigRound;
    stLog.stRound.bySmallRound = bySmallRound;
    stLog.byLogResult          = EGLR_NOT_START;
    stLog.byGvGResult          = GR_NOT_START;
    stLog.dwFactionID1         = dwFactionID1;
    stLog.dwFactionID2         = dwFactionID2;
    stLog.qwVideoID            = videoID;

    BOOL bNoEnemy = (dwFactionID1 == 0 || dwFactionID2 == 0);
    if(bNoEnemy)
    {
        stLog.byLogResult = EGLR_NO_ENEMY;
        stLog.byGvGResult = EGLR_WIN;
        stLog.qwVideoID   = 0;
    }

    // 单个门派的战况列表
    if(dwFactionID1 > 0)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stFaction1LogList = m_mapFactionLog[dwFactionID1];
        stFaction1LogList.dwFactionID = dwFactionID1;

        if(NULL == gvgutil::FindLogByRound(stFaction1LogList, stLog.stRound))
        {
            stFaction1LogList.astLogs[stFaction1LogList.wLogCnt++] = stLog;
        }        
    }
    if (dwFactionID2 > 0)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stFaction2LogList = m_mapFactionLog[dwFactionID2];
        stFaction2LogList.dwFactionID = dwFactionID2;

        if(NULL == gvgutil::FindLogByRound(stFaction2LogList, stLog.stRound))
        {
            stFaction2LogList.astLogs[stFaction2LogList.wLogCnt++] = stLog;
        }
    }
}

void CGvGArena::TakeDataToStruct()
{
    m_stArena.byCurStage = m_eStage;

    m_mapFactionLog.erase(0);

    DT_FACTION_LIST_SIMPLE_GVG_LOG &stLogs = m_stArena.stFactionsLog;
    memset(&stLogs, 0, sizeof(stLogs));

    for(FactionSimpleLogMap::iterator itr = m_mapFactionLog.begin(); itr != m_mapFactionLog.end(); ++itr)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stItrLog = itr->second;
        if(0 == stItrLog.wLogCnt)
        {
            continue;
        }

        DT_FACTION_SIMPLE_GVG_LOG_LIST &stLog = stLogs.astLogList[stLogs.wFactionCnt++];
        stLog = stItrLog;

        if(stLogs.wFactionCnt >= MAX_GVG_ARENA_FACTION_CNT)
        {
            break;
        }
    }
}

void CGvGArena::AddLog(UINT32 dwFactionID, const DT_GVG_VIDEO &stVideo, UINT16 wFactionAddScore)
{
    if(0 == dwFactionID)
    {
        return;
    }

    DT_FACTION_SIMPLE_GVG_LOG_LIST &stLogList = m_mapFactionLog[dwFactionID];
    stLogList.dwFactionID = dwFactionID;

    DT_FACTION_SIMPLE_GVG_LOG *pstOldLog = gvgutil::FindLogByRound(stLogList, stVideo.stRound);
    DT_FACTION_SIMPLE_GVG_LOG *pstNewLog = NULL;

    if(pstOldLog)
    {
        pstNewLog = pstOldLog;
    }
    else
    {
        pstNewLog = &stLogList.astLogs[stLogList.wLogCnt++];
        pstNewLog->qwVideoID = stVideo.qwVideoID;
        pstNewLog->dwFactionID1 = stVideo.stG1.dwFactionID;
        pstNewLog->dwFactionID2 = stVideo.stG2.dwFactionID;
        pstNewLog->stRound      = stVideo.stRound;
    }

    pstNewLog->wAddScore = wFactionAddScore;
    
    BOOL bNoEnemy = ((dwFactionID == stVideo.stG1.dwFactionID && stVideo.stG2.dwFactionID == 0) ||
                     (dwFactionID == stVideo.stG2.dwFactionID && stVideo.stG1.dwFactionID == 0));
    if(bNoEnemy)
    {
        pstNewLog->byLogResult = EGLR_NO_ENEMY;
        pstNewLog->byGvGResult = EGLR_WIN;
        pstNewLog->qwVideoID   = 0;
    }
    else
    {
        BOOL bWin = ((dwFactionID == stVideo.stG1.dwFactionID && stVideo.byBattleResult == GR_LEFT_WIN) ||
                     (dwFactionID == stVideo.stG2.dwFactionID && stVideo.byBattleResult == GR_RIGHT_WIN));

        pstNewLog->byLogResult = (bWin ? EGLR_WIN : EGLR_LOSE);
        pstNewLog->byGvGResult = stVideo.byBattleResult;
    }

    m_mapFactionLog.erase(0);
}

BOOL CGvGArena::War16To8Begin()
{
    if(IsDone(GVG_4_16_TO_8_BEGIN))
    {
        return TRUE;
    }

    // 进行选手配对
    if(!RollPair16To8())
    {
        return FALSE;
    }

    // 初始化本轮门派战况
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(stWar16To8.byFactionCnt < GVG_16_TO_8_FACTION_CNT)
        {
            USR_INFO(_SDT("[%s: %d]: CGvGArena::War16To8Begin faction cnt = %u, expecting %u, ArenaID = %u!"), MSG_MARK, stWar16To8.byFactionCnt, MAX_GVG_TOP_FACTION_CNT, GetArenaID());
        }

        for(UINT8 byNo = 0/*门派序号*/; byNo < MAX_GVG_TOP_FACTION_CNT; byNo++)
        {
            const DT_GVG_FACTION &stFaction1 = stWar16To8.astFactionList[2 * byNo];
            const DT_GVG_FACTION &stFaction2 = stWar16To8.astFactionList[2 * byNo + 1];

            if(0 == stFaction1.stBaseData.dwFactionID && 0 == stFaction2.stBaseData.dwFactionID)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::War16To8Begin ArenaID = %u, can't find faction<%u> at pos<%u> & faction<%u> at pos<%u>, !"), 
                    MSG_MARK, GetArenaID(), stFaction1.stBaseData.dwFactionID, 2 * byNo + 1, stFaction2.stBaseData.dwFactionID, 2 * byNo + 2);
                continue;
            }

            VideoID videoID = CGvGVideoMgr::Instance()->MakeVideoID(stFaction1.stBaseData.dwFactionID, stFaction2.stBaseData.dwFactionID);

            stWar16To8.aqwVideoIDList16To8[byNo] = videoID;
            AddNotStartLog(stFaction1.stBaseData.dwFactionID, stFaction2.stBaseData.dwFactionID, videoID, GVG_16_TO_8);
        }
    }

    ClearWar();
    
    SetStageDone(GVG_4_16_TO_8_BEGIN);
    return TRUE;
}

void CGvGArena::ClearWar()
{
    ClearZones();
    BuildZone();
    ClearArrivedZone();

    m_dwPullGroupID = 0;
}

BOOL CGvGArena::War8To4Begin()
{
    // 每个配对的门派间进行对战，胜者进入胜者组
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar8To4 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(stWar8To4.byReady8To4)
        {
            continue;
        }

        stWar8To4.byReady8To4 = TRUE;

        UINT16 wGamebleIdx = 0;


        // 将16进8的胜出门派两两配对进行对战
        for(BYTE byNo = 0/*门派序号*/; byNo + 1 < GVG_16_TO_8_WIN_CNT; byNo += 2)
        {
            // 寻找配对对手，如果找不到配对对手，则将对手指向到空门派
            UINT16 dwFaction1Idx = (stWar8To4.abyWin8List[byNo] > 0 ? stWar8To4.abyWin8List[byNo] - 1 : 2 * byNo);
            UINT16 dwFaction2Idx = (stWar8To4.abyWin8List[byNo + 1] > 0 ? stWar8To4.abyWin8List[byNo + 1] - 1 : 2 * (byNo + 1));

            DT_GVG_FACTION &stFaction1 = stWar8To4.astFactionList[dwFaction1Idx];
            DT_GVG_FACTION &stFaction2 = stWar8To4.astFactionList[dwFaction2Idx];

            UINT32 dwFactionID1 = stFaction1.stBaseData.dwFactionID;
            UINT32 dwFactionID2 = stFaction2.stBaseData.dwFactionID;
            if(0 == dwFactionID1 && 0 == dwFactionID2)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::War8To4Begin ArenaID = %u, can't find faction<%u> at pos<%u> & faction<%u> at pos<%u>, !"), 
                    MSG_MARK, GetArenaID(), dwFactionID1, 2 * byNo + 1, dwFactionID2, 2 * byNo + 2);
                continue;
            }

            VideoID videoID = CGvGVideoMgr::Instance()->MakeVideoID(dwFactionID1, dwFactionID2);

            stWar8To4.aqwVideoIDList8To4[wGamebleIdx++] = videoID;
            AddNotStartLog(dwFactionID1, dwFactionID2, videoID, GVG_8_TO_4);
        }
    }

    ClearWar();

    SetStageDone(GVG_5_8_TO_4_BEGIN);
    return TRUE;
}

void CGvGArena::AddFinalWarNotStartLog(UINT8 byRound)
{
    if (0 == byRound || (byRound >= m_stArena.stFinalWarInfo.byFactionCnt && 1 != byRound))
    {
        return;
    }

    BOOL abFighted[GVG_FINAL_WAR_FACTION_CNT + 1] = {0};

    DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;
    DT_FINAL_WAR_ROUND_INFO &stTargetRound = stFinalWar.astEachRoundInfo[byRound - 1];
    stTargetRound.byFactionCnt = stFinalWar.byFactionCnt;

    // 门派间互殴
    memset(abFighted, 0, sizeof(abFighted));
    for(UINT8 bySelfIdx = 1; bySelfIdx <= stFinalWar.byFactionCnt; ++bySelfIdx)
    {
        UINT8 byEnemyIdx = gvgutil::abyFinalWarMapping[byRound - 1][bySelfIdx - 1];

        const DT_GVG_FACTION &stFaction1 = stFinalWar.astFactionList[bySelfIdx - 1];
        const DT_GVG_FACTION &stFaction2 = stFinalWar.astFactionList[byEnemyIdx - 1];
        if(abFighted[bySelfIdx])
        {
            continue;
        }

        abFighted[bySelfIdx]  = TRUE;
        abFighted[byEnemyIdx] = TRUE;

        VideoID videoID = CGvGVideoMgr::Instance()->MakeVideoID(stFaction1.stBaseData.dwFactionID, stFaction2.stBaseData.dwFactionID);

        DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFactionInfo1 = stTargetRound.astFactionList[bySelfIdx - 1];
        memset(&stFactionInfo1, 0, sizeof(stFactionInfo1));
        stFactionInfo1.byEnemyPos = byEnemyIdx;
        stFactionInfo1.qwVideoID  = videoID;

        DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFactionInfo2 = stTargetRound.astFactionList[byEnemyIdx - 1];
        memset(&stFactionInfo2, 0, sizeof(stFactionInfo2));
        stFactionInfo2.byEnemyPos = bySelfIdx;
        stFactionInfo2.qwVideoID  = videoID;

        AddNotStartLog(stFaction1.stBaseData.dwFactionID, stFaction2.stBaseData.dwFactionID, videoID, GVG_FINAL_WAR, byRound);
    }
}

BOOL CGvGArena::FinalWarBegin()
{
    if(!InitFinalWarMembers())
    {
        return FALSE;
    }

    DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;
    if(stFinalWar.byTotalRound >= 1)
    {
        return TRUE;
    }

    stFinalWar.byTotalRound = 1;
    AddFinalWarNotStartLog(1);
    
    ClearWar();

    SetStageDone(GVG_6_FINAL_WAR_BEGIN);
    return TRUE;
}

BOOL CGvGArena::War16To8Go()
{
    if(IsStageDone(GVG_4_16_TO_8_GO))
    {
        return TRUE;
    }
    
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar8To4 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(0 == stWar8To4.byReady16To8)
        {
            return FALSE;
        }
    }

    // 每个配对的门派间进行对战，胜者进入胜者组
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(stWar16To8.byWin8Cnt > 0)
        {
            continue;
        }

        for(UINT16 wNo = 0/*门派序号*/; wNo < MAX_GVG_TOP_FACTION_CNT; wNo++)
        {
            DT_GVG_FACTION &stFaction1 = stWar16To8.astFactionList[2 * wNo];
            DT_GVG_FACTION &stFaction2 = stWar16To8.astFactionList[2 * wNo + 1];

            UINT32 dwFactionID1 = stFaction1.stBaseData.dwFactionID;
            UINT32 dwFactionID2 = stFaction2.stBaseData.dwFactionID;

            if(0 == dwFactionID1 && 0 == dwFactionID2)
            {
                continue;
            }

            CPKFaction *poFaction1 = FindPKFaction(stFaction1.wZoneID, dwFactionID1);
            CPKFaction *poFaction2 = FindPKFaction(stFaction2.wZoneID, dwFactionID2);
            if(NULL == poFaction1 && NULL == poFaction2)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::War16To8Go ArenaID = %u, can't find faction<%u> & faction<%u>, !"), MSG_MARK, GetArenaID(), dwFactionID1, dwFactionID2);
                continue;
            }

            SGvGVideo &stVideo = CGvGMgr::GvG(poFaction1, poFaction2);
            
            if(GR_LEFT_WIN == stVideo.stGvGInfo.byBattleResult)
            {
                stWar16To8.abyWin8List[stWar16To8.byWin8Cnt] = 2 * wNo + 1;
            }
            else
            {
                stWar16To8.abyWin8List[stWar16To8.byWin8Cnt] = 2 * wNo + 2;
            }

            stWar16To8.byWin8Cnt++;
            stVideo.stGvGInfo.stRound.byBigRound   = GVG_16_TO_8;
            stVideo.stGvGInfo.stRound.bySmallRound = 0;

            CGvGVideoMgr::Instance()->AddVideo(stVideo);

            AddLog(dwFactionID1, stVideo.stGvGInfo, 0);
            AddLog(dwFactionID2, stVideo.stGvGInfo, 0);
        }
    }

    SetStageDone(GVG_4_16_TO_8_GO);
    Print16To8Winner();
    return TRUE;
}

BOOL CGvGArena::War8To4Go()
{
    if(IsStageDone(GVG_5_8_TO_4_GO))
    {
        return TRUE;
    }

    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar8To4 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(0 == stWar8To4.byReady8To4)
        {
            return FALSE;
        }
    }

    DT_GVG_FACTION stEmptyFaction;
    memset(&stEmptyFaction, 0, sizeof(stEmptyFaction));

    // 每个配对的门派间进行对战，胜者进入胜者组
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar8To4 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(stWar8To4.byWin4Cnt > 0)
        {
            continue;
        }

        FactionIDSet setFactionID;
        for (UINT16 wFactionIdx = 0; wFactionIdx < stWar8To4.byFactionCnt; wFactionIdx++)
        {
            setFactionID.insert(stWar8To4.astFactionList[wFactionIdx].stBaseData.dwFactionID);
        }

        // 将16进8的胜出门派两两配对进行对战
        for(BYTE byNo = 0/*门派序号*/; byNo + 1 < GVG_16_TO_8_WIN_CNT; byNo += 2)
        {
            // 寻找配对对手，如果找不到配对对手，则将对手指向到空门派
            UINT16 dwFaction1Idx = (stWar8To4.abyWin8List[byNo] > 0 ? stWar8To4.abyWin8List[byNo] - 1 : 2 * byNo);
            UINT16 dwFaction2Idx = (stWar8To4.abyWin8List[byNo + 1] > 0 ? stWar8To4.abyWin8List[byNo + 1] - 1 : 2 * (byNo + 1));

            DT_GVG_FACTION &stFaction1 = stWar8To4.astFactionList[dwFaction1Idx];
            DT_GVG_FACTION &stFaction2 = stWar8To4.astFactionList[dwFaction2Idx];

            UINT32 dwFactionID1 = stFaction1.stBaseData.dwFactionID;
            UINT32 dwFactionID2 = stFaction2.stBaseData.dwFactionID;
            if(0 == dwFactionID1 && 0 == dwFactionID2)
            {
                continue;
            }
            
            CPKFaction *poFaction1 = FindPKFaction(stFaction1.wZoneID, dwFactionID1);
            CPKFaction *poFaction2 = FindPKFaction(stFaction2.wZoneID, dwFactionID2);
            if(NULL == poFaction1 && NULL == poFaction2)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::War8To4Go ArenaID = %u, can't find faction<%u> & faction<%u>, !"), MSG_MARK, GetArenaID(), dwFactionID1, dwFactionID2);
                continue;
            }

            SGvGVideo &stVideo = CGvGMgr::GvG(poFaction1, poFaction2);
            stVideo.stGvGInfo.stRound.byBigRound   = GVG_8_TO_4;
            stVideo.stGvGInfo.stRound.bySmallRound = 0;

            if(GR_LEFT_WIN == stVideo.stGvGInfo.byBattleResult)
            {
                stWar8To4.abyWin4List[stWar8To4.byWin4Cnt] = stWar8To4.abyWin8List[byNo];
            }
            else
            {
                stWar8To4.abyWin4List[stWar8To4.byWin4Cnt] = stWar8To4.abyWin8List[byNo + 1];
            }

            stWar8To4.byWin4Cnt++;
            CGvGVideoMgr::Instance()->AddVideo(stVideo);

            AddLog(dwFactionID1, stVideo.stGvGInfo, 0);
            AddLog(dwFactionID2, stVideo.stGvGInfo, 0);
        }
    }

    SetStageDone(GVG_5_8_TO_4_GO);
    Print8To4Winner();
    return TRUE;
}

UINT8 CGvGArena::GetExpectFinalWarRound()
{
    if (m_eStage < GVG_6_FINAL_WAR_GO)
    {
        return 0;
    }
    else if(m_eStage > GVG_6_FINAL_WAR_GO)
    {
        return m_stArena.stFinalWarInfo.byFactionCnt;
    }

    SGvGProp &stGvGProp = CGvGPropMgr::Instance()->GetGvGProp();

    UINT64 qwActivityStartTime = CGvGMgr::Instance()->GetActivityData().qwStartTime;
    UINT64 qwFinalWarGoTime    = CGvGPropMgr::Instance()->GetStageStartTime(qwActivityStartTime, GVG_6_FINAL_WAR_GO);

    UINT64 qwNow = SDTimeSecs();

    UINT8 byExpectRound = 0;
    if(qwNow >= qwFinalWarGoTime)
    {
        byExpectRound = (UINT8)((qwNow - qwFinalWarGoTime) / stGvGProp.dwFinalWarEachRoundTime + 1);
    }

    // UINT8 byMaxRound = (m_stArena.stFinalWarInfo.byFactionCnt > 1 ? m_stArena.stFinalWarInfo.byFactionCnt - 1 : 1);

    byExpectRound = MIN(m_stArena.stFinalWarInfo.byFactionCnt, byExpectRound);
    byExpectRound = MAX(1, byExpectRound);

    return byExpectRound;
}

// 总决赛第一轮结果是否已决出
BOOL CGvGArena::IsFinalWarRoundComplete(UINT8 byRound)
{
    DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;
    if (byRound > stFinalWar.byTotalRound){
        return FALSE;
    }else if (byRound < stFinalWar.byTotalRound){
        return TRUE;
    }else{
        if (1 == byRound){
            DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[0];
            if(stFaction.byWinCnt + stFaction.byLoseCnt >= 1){
                return TRUE;
            }

            return FALSE;
        }

        return TRUE;
    }

    return FALSE;
}

BOOL CGvGArena::IsShouldFinalWarMoveOn()
{
    if (m_eStage < GVG_6_FINAL_WAR_GO){
        return FALSE;
    }

    SGvGProp &stGvGProp = CGvGPropMgr::Instance()->GetGvGProp();

    UINT8 byExpectRound = GetExpectFinalWarRound();
    if (0 == byExpectRound){
        return FALSE;
    }

    DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;

    if (IsFinalWarRoundComplete(byExpectRound)){
        return FALSE;
    }

    if (byExpectRound >= 2){
        if (!IsFinalWarRoundComplete(byExpectRound - 1)){
            return TRUE;
        }
    }

    UINT64 qwNow = SDTimeSecs();

    UINT64 qwActivityStartTime = CGvGMgr::Instance()->GetActivityData().qwStartTime;
    UINT64 qwFinalWarGoTime    = CGvGPropMgr::Instance()->GetStageStartTime(qwActivityStartTime, GVG_6_FINAL_WAR_GO);

    UINT64 qwThisRoundEndTime = qwFinalWarGoTime + stGvGProp.dwFinalWarEachRoundTime * byExpectRound;
    if(qwNow < qwThisRoundEndTime - stGvGProp.dwForbidFormationTime){
        return FALSE;
    }

    return TRUE;
}

BOOL CGvGArena::PrepareFinalWarGo()
{
    if (!IsShouldFinalWarMoveOn())
    {
        return TRUE;
    }

    PrepareWarGo(GVG_6_FINAL_WAR_GO);
    return TRUE;
}

BOOL CGvGArena::FinalWarGo()
{
    if(IsStageDone(GVG_6_FINAL_WAR_GO))
    {
        return TRUE;
    }

    DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;
    BOOL abFighted[GVG_FINAL_WAR_FACTION_CNT + 1] = {0};

    UINT8 byExpectRound = GetExpectFinalWarRound();

    DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[0];
    
    if (stFinalWar.byTotalRound < byExpectRound)
    {
        if (stFaction.byWinCnt + stFaction.byLoseCnt < stFinalWar.byTotalRound)
        {
        }
        else if (stFaction.byWinCnt + stFaction.byLoseCnt < byExpectRound)
        {
            stFinalWar.byTotalRound++;
        }
    }

    // 门派间互殴
    for(UINT8 i = 1; i <= 1; i++)
    {
        memset(abFighted, 0, sizeof(abFighted));

        DT_FINAL_WAR_ROUND_INFO &stCurRoundInfo = stFinalWar.astEachRoundInfo[stFinalWar.byTotalRound - 1];
        stCurRoundInfo.byFactionCnt = stFinalWar.byFactionCnt;

        for(UINT8 bySelfIdx = 1; bySelfIdx <= stFinalWar.byFactionCnt; ++bySelfIdx)
        {
            UINT8 byEnemyIdx = gvgutil::abyFinalWarMapping[stFinalWar.byTotalRound - 1][bySelfIdx - 1];
            if(0 == byEnemyIdx)
            {
                continue;
            }

            if(abFighted[bySelfIdx])
            {
                continue;
            }

            DT_GVG_FACTION &stFaction1 = stFinalWar.astFactionList[bySelfIdx - 1];
            DT_GVG_FACTION &stFaction2 = stFinalWar.astFactionList[byEnemyIdx - 1];            

            UINT32 dwFactionID1 = stFaction1.stBaseData.dwFactionID;
            UINT32 dwFactionID2 = stFaction2.stBaseData.dwFactionID;
            if(0 == dwFactionID1 && 0 == dwFactionID2)
            {
                continue;
            }

            DT_GVG_ROUND stRound;
            stRound.byBigRound   = GVG_FINAL_WAR;
            stRound.bySmallRound = stFinalWar.byTotalRound;
            
            CPKFaction *poFaction1 = FindPKFaction(stFaction1.wZoneID, dwFactionID1);
            CPKFaction *poFaction2 = FindPKFaction(stFaction2.wZoneID, dwFactionID2);
            if(NULL == poFaction1 && NULL == poFaction2)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::War16To8Go ArenaID = %u, can't find faction<%u> & faction<%u>, !"), MSG_MARK, GetArenaID(), dwFactionID1, dwFactionID2);
                continue;
            }

            SGvGVideo &stVideo = CGvGMgr::GvG(poFaction1, poFaction2);
            if(GR_LEFT_WIN == stVideo.stGvGInfo.byBattleResult)
            {
                stFaction1.byWinCnt++;
                stFaction2.byLoseCnt++;
            }
            else
            {
                stFaction1.byLoseCnt++;
                stFaction2.byWinCnt++;
            }

            stVideo.stGvGInfo.stRound = stRound;

            UINT16 wFaction1AddScore = gvgutil::CalcGvGAddScore(dwFactionID1, stVideo.stGvGInfo);
            UINT16 wFaction2AddScore = gvgutil::CalcGvGAddScore(dwFactionID2, stVideo.stGvGInfo);

            stFaction1.wScore += wFaction1AddScore;
            stFaction2.wScore += wFaction2AddScore;
            
            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFactionInfo1 = stCurRoundInfo.astFactionList[bySelfIdx - 1];
            DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFactionInfo2 = stCurRoundInfo.astFactionList[byEnemyIdx - 1];

            memset(&stFactionInfo1, 0, sizeof(stFactionInfo1));
            stFactionInfo1.byEnemyPos = byEnemyIdx;
            stFactionInfo1.qwVideoID  = stVideo.stGvGInfo.qwVideoID;
            stFactionInfo1.wScore     = stFaction1.wScore;

            memset(&stFactionInfo2, 0, sizeof(stFactionInfo2));
            stFactionInfo2.byEnemyPos = bySelfIdx;
            stFactionInfo2.qwVideoID  = stVideo.stGvGInfo.qwVideoID;
            stFactionInfo2.wScore     = stFaction2.wScore;
                                           
            stFactionInfo1.byResult = (GR_LEFT_WIN == stVideo.stGvGInfo.byBattleResult ? EGLR_WIN : EGLR_LOSE);
            stFactionInfo2.byResult = (GR_RIGHT_WIN == stVideo.stGvGInfo.byBattleResult ? EGLR_WIN : EGLR_LOSE);

            abFighted[bySelfIdx]  = TRUE;
            abFighted[byEnemyIdx] = TRUE;

            CGvGVideoMgr::Instance()->AddVideo(stVideo);

            AddLog(dwFactionID1, stVideo.stGvGInfo, wFaction1AddScore);
            AddLog(dwFactionID2, stVideo.stGvGInfo, wFaction2AddScore);
        }

        gvgutil::SortFaction(stCurRoundInfo, stFinalWar.astFactionList);
    }

    AddFinalWarNotStartLog(stFinalWar.byTotalRound + 1);
    ClearWar();

    UINT8 byMaxRound = (stFinalWar.byFactionCnt > 1 ? stFinalWar.byFactionCnt - 1 : 1);
    stFinalWar.byTotalRound = MIN(stFinalWar.byTotalRound, byMaxRound);

    BOOL bEnd = (stFinalWar.byTotalRound + 1 >= stFinalWar.byFactionCnt);
    if (bEnd)
    {
        SetStageDone(GVG_6_FINAL_WAR_GO);
        PrintFinalWinner();
    }

    return TRUE;
}

void CGvGArena::Print16To8Winner()
{
    DBG_INFO(_SDT("----------------------print 16 to 8 winners of session %u, arena<%u>----------------------"), CGvGMgr::Instance()->GetCurSession(), GetArenaID());
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        DBG_INFO(_SDT("*********war<%u>:"), i + 1);

        for(UINT16 wIdx = 0; wIdx < stWar16To8.byWin8Cnt; wIdx++)
        {
            UINT16 wWinPos = stWar16To8.abyWin8List[wIdx];
            if(0 == wWinPos)
            {
                continue;
            }

            DT_GVG_FACTION &stFaction = stWar16To8.astFactionList[wWinPos - 1];
            DBG_INFO(_SDT("pos<%u>: faction[%-8u] %s"), wIdx + 1, stFaction.stBaseData.dwFactionID, stFaction.stBaseData.aszFactionName);
        }
    }
}

void CGvGArena::Print8To4Winner()
{
    DBG_INFO(_SDT("----------------------print 8 to 4 winners of session %u, arena<%u>----------------------"), CGvGMgr::Instance()->GetCurSession(), GetArenaID());
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        DBG_INFO(_SDT("*********"));

        for(UINT16 wIdx = 0; wIdx < stWar16To8.byWin4Cnt; wIdx++)
        {
            UINT16 wWinPos = stWar16To8.abyWin4List[wIdx];
            if(0 == wWinPos)
            {
                continue;
            }

            DT_GVG_FACTION &stFaction = stWar16To8.astFactionList[wWinPos - 1];
            DBG_INFO(_SDT("pos<%u>: faction[%-8u] %s"), wIdx + 1, stFaction.stBaseData.dwFactionID, stFaction.stBaseData.aszFactionName);
        }
    }
}

void CGvGArena::PrintFinalWinner()
{
    DBG_INFO(_SDT("----------------------print final winners of session %u, arena<%u>----------------------"), CGvGMgr::Instance()->GetCurSession(), GetArenaID());
    DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;

    DT_FINAL_WAR_ROUND_INFO &stFinalRound = stFinalWar.astEachRoundInfo[stFinalWar.byTotalRound - 1];
    for(UINT8 byRank = 1; byRank <= stFinalRound.byFactionCnt; byRank++)
    {
        UINT8 byWinPos = stFinalRound.abyRankList[byRank - 1];
        if(0 == byWinPos)
        {
            continue;
        }

        DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &stFinalRoundFaction = stFinalRound.astFactionList[byWinPos - 1];
        DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[byWinPos - 1];

        DBG_INFO(_SDT("rank<%u>: faction[%-8u] %s, score[%u]"), byRank, stFaction.stBaseData.dwFactionID, stFaction.stBaseData.aszFactionName, stFinalRoundFaction.wScore);
    }
}

BOOL CGvGArena::PrepareWarGo(GVG_STAGE eStage)
{
    if(m_eStage != eStage)
    {
        ClearArrivedZone();
        BuildZone();
    }

    if(!IsDone((GVG_STAGE)(eStage - 1)))
    {
        return FALSE;
    }

    // 检测是否已决出获胜选手，如果没有则继续比赛进程
    if(IsStageDone(eStage) && IsLogicDone(eStage))
    {
        return TRUE;
    }

    if(m_dwPullGroupID)
    {
        USR_INFO(_SDT("[%s: %d]: CGvGMgr::WarGo arena<%u> has a pull group <%u> skipped!!"), MSG_MARK, GetArenaID(), m_dwPullGroupID);
        return TRUE;
    }

    // 取出各个赛场的门派选手列表（即上一轮的获胜门派），到对应区的游戏服索取当前的门派数据
    EachZoneFactionIDListMap mapZones = SplitFactionsByZone(gvgutil::Stage2Round(eStage));

    // 如果一个选手都没有，则本轮比赛直接完结
    if(mapZones.empty())
    {
        Go();
        return TRUE;
    }

    SendPullFactionReq(mapZones);
    return TRUE;
}

BOOL CGvGArena::WarEnd(GVG_STAGE eStage)
{
    m_dwPullGroupID = 0;
    SetStageDone(eStage);

    return TRUE;
}

BOOL CGvGArena::Go()
{
    BOOL bSucc = FALSE;
    switch(m_eRound)
    {
    case GVG_16_TO_8:
        bSucc = War16To8Go();
        break;

    case GVG_8_TO_4:
        bSucc = War8To4Go();
        break;

    case GVG_FINAL_WAR:
        bSucc = FinalWarGo();
        break;
    }

    if(bSucc)
    {
        SyncVideo();
        SyncActivity();
    }

    SaveToDB();
    CGvGVideoMgr::Instance()->SaveToDB();
    return bSucc;
}

BYTE CGvGArena::GetGroupIDByZone(ZoneID zoneID)
{
    DT_GROUP_ZONE_ID_LIST &stGroupZoneIDList = m_stArena.stGroupZoneIDList;
    for(UINT16 i = 0; i < stGroupZoneIDList.byGroupCnt; i++)
    {
        DT_GROUP_ZONE &stGroupZone = stGroupZoneIDList.astGroupList[i];
        for(UINT16 i = 0; i < 2; i++)
        {
            if(zoneID == stGroupZone.awZoneIDList[i])
            {
                return stGroupZone.byGroupID;
            }
        }
    }

    return 0;
}

BOOL CGvGArena::IsAllZoneArrived(GvGRound eRound)
{
    EachZoneFactionIDListMap mapZones = SplitFactionsByZone(eRound);
    if(m_setArrivedZone.size() != mapZones.size())
    {
        return FALSE;
    }
    
    for(EachZoneFactionIDListMap::iterator zoneItr = mapZones.begin(); zoneItr != mapZones.end(); ++zoneItr)
    {
        UINT16 wZoneID = zoneItr->first;

        CPKZone *poPKZone = FindPKZone(wZoneID);
        if(NULL == poPKZone)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::IsAllZoneArrived can not find pkzone of zoneID = %u, arenaID = %u!"), MSG_MARK, wZoneID, GetArenaID());
            return FALSE;
        }

        FactionIDSet &setFaction = zoneItr->second;
        for(FactionIDSet::iterator factionIDItr = setFaction.begin(); factionIDItr != setFaction.end(); ++factionIDItr)
        {
            UINT32 dwFactionID = *factionIDItr;
            if(NULL == poPKZone->FindPKFaction(dwFactionID))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::IsAllZoneArrived can not find pkfaction of faction<%u>, zoneID = %u, arenaID = %u!"), MSG_MARK, dwFactionID, wZoneID, GetArenaID());
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL CGvGArena::IsAllZoneReady()
{
    if(GVG_0_NOT_START == m_eStage || GVG_8_SLEEPING == m_eStage)
    {
        return TRUE;
    }

    return m_setReadyZone.size() == m_stArena.stZoneIDList.byZoneCnt;
}

BOOL CGvGArena::IsZoneArrived(ZoneID zoneID)
{
    return m_setArrivedZone.find(zoneID) != m_setArrivedZone.end();
}

BOOL CGvGArena::IsEmpty()
{
    BOOL bEmpty = !IsStageDone(GVG_7_END);
    bEmpty &= !IsStageDone(GVG_2_MONEY);
    bEmpty &= (0 == m_stArena.stFactionsLog.wFactionCnt);

    return bEmpty;
}

void CGvGArena::SendPullFactionReq(EachZoneFactionIDListMap &mapZones)
{
    PKT_PKGS_GET_FACTIONS_REQ req;

    DT_ONE_ZONE_FACTION_ID_LIST &stFactionIDList = req.stFactionIDList;

    // 依次向各个区的gamesvr要求调取门派名单中各门派的数据
    for(EachZoneFactionIDListMap::iterator zoneItr = mapZones.begin(); zoneItr != mapZones.end(); ++zoneItr)
    {
        UINT16 wZoneID             = zoneItr->first;
        FactionIDSet &setFactionID = zoneItr->second;

        memset(&stFactionIDList, 0, sizeof(stFactionIDList));
        stFactionIDList.wZoneID = wZoneID;

        for(FactionIDSet::iterator factionIDItr = setFactionID.begin(); factionIDItr != setFactionID.end(); ++factionIDItr)
        {
            UINT32 dwFactionID = *factionIDItr;
            stFactionIDList.adwFactionList[stFactionIDList.byFactionCnt++] = dwFactionID;
        }

        gsapi::SendMsg2GS(wZoneID, (CHAR*)&req, PKGS_GET_FACTIONS_REQ);
    }
}

// 接收到某个区返回的该区前几强门派列表
void CGvGArena::OnRecvTopFaction(ZoneID zoneID, DT_GVG_FACTION astTopList[], UINT8 byTopCnt)
{
    if(0 == byTopCnt)
    {
        // 只警告，不退出
        USR_INFO(_SDT("[%s: %d]: CGvGMgr::HandleRetTopFactions ack.byTop8Cnt = 0, zoneID = %u, current stage = %u!"), MSG_MARK, zoneID, m_eStage);
    }

    if(byTopCnt > MAX_GVG_TOP_FACTION_CNT)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::HandleRetTopFactions detecting round error, zoneid = %u, current stage = %u!"), MSG_MARK, zoneID, m_eStage);
        return;
    }

    if(IsDone(GVG_3_CHAOS_WAR_END))
    {
        return;
    }

    if(GVG_3_CHAOS_WAR_END != m_eStage)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::HandleRetTopFactions detecting round error, current stage = %u, zoneid = %u, ArenaID = %u!"), MSG_MARK, m_eStage, zoneID, GetArenaID());
        return;
    }

    Add16To8Members(zoneID, astTopList, byTopCnt);
}

void CGvGArena::OnRecvFaction(ZoneID zoneID, DT_PK_FACTION_LIST &stRetFactions)
{
    if(0 == stRetFactions.byFactionCnt)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::OnRecvFaction faction cnt = 0, zoneID = %u, arenaID = %u!"), MSG_MARK, zoneID, GetArenaID());
        return;
    }

    if(IsZoneArrived(zoneID))
    {
        USR_INFO(_SDT("[%s: %d]: CGvGArena::OnRecvFaction zoneID = %u, arenaID = %u, stage = %u, recv faction count = %u!"), MSG_MARK, zoneID, GetArenaID(), m_eStage, stRetFactions.byFactionCnt);
        return;
    }

    DBG_INFO(_SDT("[%s: %d]: CGvGArena::OnRecvFaction zoneID = %u, arenaID = %u, stage = %u, recv faction count = %u!"), MSG_MARK, zoneID, GetArenaID(), m_eStage, stRetFactions.byFactionCnt);

    CPKZone *poPKZone = FindPKZone(zoneID);
    if(NULL == poPKZone)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::OnRecvFaction FindPKZone failed, zoneID = %u, arenaID = %u!"), MSG_MARK, zoneID, GetArenaID());

        BuildZone();
        return;
    }

    switch(m_eStage)
    {
    case GVG_4_16_TO_8_GO:
    case GVG_5_8_TO_4_GO:
    case GVG_6_FINAL_WAR_GO:
        if(!poPKZone->Init(stRetFactions))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CPKZone::Init failed, zoneID = %u!"), MSG_MARK, zoneID);
            return;
        }

        SetZoneArrived(zoneID);

        UpdateFactionsByRound(m_eRound);
        if(IsAllZoneArrived(m_eRound))
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::OnRecvFaction zoneID = %u, arenaID = %u, stage = %u, recv faction count = %u, all zone is arrived, start pull players!"), MSG_MARK, zoneID, GetArenaID(), m_eStage, stRetFactions.byFactionCnt);
            StartPullPlayers();
        }

        break;

    default:
        SYS_CRITICAL(_SDT("[%s: %d]: round error: current round = %u, ArenaID = %u!"), MSG_MARK, m_eStage, GetArenaID());
        break;
    }
}

void CGvGArena::AskReady()
{
    PKT_PKGS_GVG_ASK_READY_REQ stReq;

    stReq.byStage = m_eStage;

    DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;
    for(UINT16 wZoneIdx = 0; wZoneIdx < stZoneIDList.byZoneCnt; wZoneIdx++)
    {
        UINT16 wZoneID = stZoneIDList.awZoneIDList[wZoneIdx];
        if(m_setReadyZone.find(wZoneID) != m_setReadyZone.end())
        {
            continue;
        }

        gsapi::SendMsg2GS(wZoneID, (CHAR*)&stReq, PKGS_GVG_ASK_READY_REQ);
    }
}

void CGvGArena::OnZoneSayReady(ZoneID zoneID, PKT_GSPK_GVG_REPLY_READY_ACK &stAck)
{
    if(stAck.byStage != m_eStage)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::OnZoneSayReady error, current session = %u, recv stage = %u, current stage = %u, zoneID = %u, arenaID = %u!"), MSG_MARK, CGvGMgr::Instance()->GetCurSession(), stAck.byStage, m_eStage, zoneID, GetArenaID());
        NotifyStage(m_eStage);
        return;
    }

    if(!stAck.byIsReady)
    {
        SyncActivity(zoneID);
        return;
    }

    if(m_setReadyZone.find(zoneID) != m_setReadyZone.end())
    {
        return;
    }

    m_setReadyZone.insert(zoneID);
    if(IsAllZoneReady())
    {
        CNGString strReadyZoneList = gvgutil::GetZoneListStr(m_setReadyZone);
        DBG_INFO(_SDT("[%s: %d]: CGvGArena::OnZoneSayReady current session = %u, stage = %u, ArenaID = %u, all zone is ready, ready zone list = <%s>!"), MSG_MARK, CGvGMgr::Instance()->GetCurSession(), m_eStage, GetArenaID(), strReadyZoneList.c_str());
    }
}

void CGvGArena::SetZoneArrived(ZoneID zoneID)
{
    m_setArrivedZone.insert(zoneID);
}

BOOL CGvGArena::GvGStart()
{
    if(IsDone(GVG_1_APPLY))
    {
        return TRUE;
    }

    ResetStageFlag();
    SetStageDone(GVG_0_NOT_START);
    SetStageDone(GVG_1_APPLY);
    return TRUE;
}

BOOL CGvGArena::GvGSleep()
{
    SetStageDone(GVG_8_SLEEPING);
    return TRUE;
}

BOOL CGvGArena::GvGOver()
{
    SetStageDone(GVG_7_END);
    return TRUE;
}

void CGvGArena::Add16To8Members(ZoneID zoneID, DT_GVG_FACTION astTop8List[], UINT8 byTopCnt)
{
    if(IsZoneArrived(zoneID))
    {
        return;
    }

    // 这里只进行日志跟踪
    USR_INFO(_SDT("[%s: %d]: CGvGArena::Add16To8Members top cnt = %u, expecting %u, zone<%u>, ArenaID = %u!"), MSG_MARK, byTopCnt, MAX_GVG_TOP_FACTION_CNT, zoneID, GetArenaID());

    BYTE byGroupID = gvgutil::GetGroupIDByZone(zoneID, m_stArena.stGroupZoneIDList);
    if(0 == byGroupID)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::Add16To8Members can't not find group of zone<%u>, ArenaID = %u!"), MSG_MARK, zoneID, GetArenaID());
        return;
    }

    SetZoneArrived(zoneID);

    m_stArena.stWar16To8List.byWarCnt = m_stArena.stGroupZoneIDList.byGroupCnt;

    DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[byGroupID - 1];
    for(UINT16 i = 0; i < byTopCnt && stWar16To8.byFactionCnt < GVG_16_TO_8_FACTION_CNT; i++)
    {
        DT_GVG_FACTION &stGvGFaction = astTop8List[i];
        if(0 == stGvGFaction.stBaseData.dwFactionID)
        {
            continue;
        }

        BOOL bExist = FALSE;
        for(UINT16 wFindFactionIdx = 0; wFindFactionIdx < stWar16To8.byFactionCnt; wFindFactionIdx++)
        {
            if(stWar16To8.astFactionList[wFindFactionIdx].stBaseData.dwFactionID == stGvGFaction.stBaseData.dwFactionID)
            {
                bExist = TRUE;
                break;
            }
        }

        if(bExist)
        {
            continue;
        }

        stWar16To8.astFactionList[stWar16To8.byFactionCnt++] = stGvGFaction;
        USR_INFO(_SDT("[%s: %d]: CGvGArena::Add16To8Members zone<%u>, ArenaID = %u, faction<%u>[%s], leader<%s>!"), 
            MSG_MARK, zoneID, GetArenaID(), stGvGFaction.stBaseData.dwFactionID, stGvGFaction.stBaseData.aszFactionName, stGvGFaction.stBaseData.aszHeadName);
    }

    if(m_setArrivedZone.size() == m_stArena.stZoneIDList.byZoneCnt)
    {
        RollPair16To8();
        SetStageDone(GVG_3_CHAOS_WAR_END);
    }
}

void CGvGArena::UpdateFactionsByRound(GvGRound eRound)
{
    switch(eRound)
    {
    case GVG_16_TO_8:
    case GVG_8_TO_4:
        for (UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar = m_stArena.stWar16To8List.astWar16To8To4Info[i];
            UpdateFactions(stWar.astFactionList, stWar.byFactionCnt);
        }
        break;

    case GVG_FINAL_WAR:
        UpdateFactions(m_stArena.stFinalWarInfo.astFactionList, m_stArena.stFinalWarInfo.byFactionCnt);
        break;
    }
}

BOOL CGvGArena::InitFinalWarMembers()
{
    DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;
    if(stFinalWar.byFactionCnt > 0)
    {
        return TRUE;
    }

    memset(&stFinalWar, 0, sizeof(stFinalWar));

    for (UINT16 byWarIdx = 0; byWarIdx < m_stArena.stWar16To8List.byWarCnt; byWarIdx++)
    {
        const DT_16_TO_8_TO_4_WAR_INFO &st8To4War = m_stArena.stWar16To8List.astWar16To8To4Info[byWarIdx];

        for (UINT16 i = 0; i < GVG_8_TO_4_WIN_CNT; i++)
        {
            if(0 == st8To4War.abyWin4List[i])
            {
                continue;
            }

            const DT_GVG_FACTION &stWarWinner = st8To4War.astFactionList[st8To4War.abyWin4List[i] - 1];
            if(stWarWinner.stBaseData.dwFactionID > 0)
            {
                stFinalWar.astFactionList[stFinalWar.byFactionCnt++] = stWarWinner;
            }
        }
    }

    return TRUE;
}

BOOL CGvGArena::RollPair16To8()
{
    DT_GVG_FACTION stEmptyFaction;
    memset(&stEmptyFaction, 0, sizeof(stEmptyFaction));

    const DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(stWar16To8.byFactionCnt > GVG_16_TO_8_FACTION_CNT)
        {
            USR_INFO(_SDT("[%s: %d]: CGvGArena::RollPair16To8 faction cnt = %u > GVG_16_TO_8_FACTION_CNT<%u>, ArenaID = %u!"), MSG_MARK, stWar16To8.byFactionCnt, MAX_GVG_TOP_FACTION_CNT, GetArenaID());
            return FALSE;
        }

        if(stWar16To8.byReady16To8)
        {
            continue;
        }

        gvgutil::SGvGFactionVec vecFactionOfZone1;
        gvgutil::SGvGFactionVec vecFactionOfZone2;

        gvgutil::TakeFactionByZone(vecFactionOfZone1, stWar16To8, stZoneIDList.awZoneIDList[2 * i]);
        gvgutil::TakeFactionByZone(vecFactionOfZone1, stWar16To8, stZoneIDList.awZoneIDList[2 * i + 1]);

        std::random_shuffle(vecFactionOfZone1.begin(), vecFactionOfZone1.end());

        stWar16To8.byFactionCnt = MIN(GVG_16_TO_8_FACTION_CNT, vecFactionOfZone1.size());
        for(UINT16 wNo = 0/*门派序号*/; wNo < stWar16To8.byFactionCnt; wNo++)
        {
            stWar16To8.astFactionList[wNo] = vecFactionOfZone1[wNo];
        }

        if (stWar16To8.byFactionCnt % 2)
        {
            stWar16To8.astFactionList[stWar16To8.byFactionCnt++] = stEmptyFaction;
        }

        gvgutil::ArrangeFaction(stWar16To8);

        stWar16To8.byReady16To8 = TRUE;
    }

    return TRUE;
}

BOOL CGvGArena::IsAllZoneConnecting()
{
    const DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;
    for(UINT16 wZoneIdx = 0; wZoneIdx < stZoneIDList.byZoneCnt; wZoneIdx++)
    {
        UINT16 wZoneID = stZoneIDList.awZoneIDList[wZoneIdx];
        if(!gsapi::IsConnectingToGS(wZoneID))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::IsAllZoneConnecting zone<%u> is not connecting, ArenaID = %u!"), MSG_MARK, wZoneID, GetArenaID());
            return FALSE;
        }
    }

    return TRUE;
}

void CGvGArena::CheckEachZoneReady()
{
    // 要求跨服战服务器此时与本赛区的服务器均保持连接
    if(!IsAllZoneConnecting())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::IsDone stage = %u, ArenaID = %u, not all zone is connecting!"), MSG_MARK, m_eStage, GetArenaID());
        ClearReadyZoneList();
    }

    if(!IsAllZoneReady())
    {
        AskReady();
    }
}

FactionIDSet CGvGArena::Get16To8Seed()
{
    FactionIDSet setFactionID;
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        for (UINT16 wFactionIdx = 0; wFactionIdx < stWar.byFactionCnt; wFactionIdx++)
        {
            DT_GVG_FACTION &stFaction = stWar.astFactionList[wFactionIdx];
            setFactionID.insert(stFaction.stBaseData.dwFactionID);
        }
    }

    setFactionID.erase(0);
    return setFactionID;
}

BOOL CGvGArena::IsLogicDone(GVG_STAGE eStage)
{
    BOOL bDone = TRUE;
    switch(eStage)
    {
    case GVG_1_APPLY:
        bDone = IsEmpty();
        break;

    case GVG_2_MONEY:
    case GVG_3_CHAOS_WAR_BEGIN:
        bDone = TRUE;
        break;

    case GVG_3_CHAOS_WAR_END:
        // bDone  = (m_stArena.astWar16To8To4Info[0].byFactionCnt >= GVG_16_TO_8_FACTION_CNT) && (m_stArena.astWar16To8To4Info[1].byFactionCnt >= GVG_16_TO_8_FACTION_CNT);
        bDone = IsStageDone(eStage);
        if(!bDone)
        {
            CNGString strArrivedZoneList = gvgutil::GetZoneListStr(m_setArrivedZone);
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone stage = %u, ArenaID = %u, not all zone is ready, expected %u zone arrive, arrived zone list = <%s>!"), 
                MSG_MARK, eStage, GetArenaID(), m_stArena.stZoneIDList.byZoneCnt, strArrivedZoneList.c_str());
        }
        break;

    case GVG_4_16_TO_8_BEGIN:
    {
        FactionIDSet set16To8Seed = Get16To8Seed();
        for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
            if(0 == stWar16To8.byReady16To8)
            {
                bDone = FALSE;
                break;
            }
        }

        bDone &= (set16To8Seed.size() == m_mapFactionLog.size());
        if(!bDone)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone stage = %u, ArenaID = %u, bDone = false, not all byReady16To8 is true, setFactionID.size()<%u>, m_mapFactionLog.size()<%u>!"),
                MSG_MARK, eStage, GetArenaID(), set16To8Seed.size(), m_mapFactionLog.size());
        }
        break;
    }
    case GVG_4_16_TO_8_GO:
        for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
            if(stWar16To8.byWin8Cnt * 2 < stWar16To8.byFactionCnt)
            {
                bDone = FALSE;
                break;
            }
        }

        if(!bDone)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone = false, stage = %u, ArenaID = %u!"), MSG_MARK, eStage, GetArenaID());
            for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
            {
                DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
                
                DBG_INFO(_SDT("[%s: %d]: m_stArena.astWar16To8To4Info[%u].byWin8Cnt = %u, m_stArena.astWar16To8To4Info[%u].byFactionCnt = %u!"), 
                    MSG_MARK, i + 1, stWar16To8.byWin8Cnt, i + 1, stWar16To8.byFactionCnt);
            }
        }
        break;

    case GVG_5_8_TO_4_GO:
        for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
            if(stWar16To8.byWin4Cnt * 2 < stWar16To8.byWin8Cnt)
            {
                bDone = FALSE;
                break;
            }
        }

        if(!bDone)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone = false, stage = %u, ArenaID = %u!"), MSG_MARK, eStage, GetArenaID());
            for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
            {
                DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
                DBG_INFO(_SDT("[%s: %d]: m_stArena.astWar16To8To4Info[%u].byWin4Cnt = %u, m_stArena.astWar16To8To4Info[%u].byWin8Cnt = %u!"), MSG_MARK, i + 1, stWar16To8.byWin4Cnt, i + 1, stWar16To8.byWin8Cnt);
            }
        }
        break;

    case GVG_6_FINAL_WAR_BEGIN:
    {
        UINT32 dwExpectedFactionCnt = 0;
        for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
            dwExpectedFactionCnt += stWar16To8.byWin4Cnt;
        }

        bDone = (dwExpectedFactionCnt <= m_stArena.stFinalWarInfo.byFactionCnt);
        if(!bDone)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone stage = %u, ArenaID = %u, bDone = false, m_stArena.stFinalWarInfo.byFactionCnt = %u!"), MSG_MARK, eStage, GetArenaID());
            for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
            {
                DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
                DBG_INFO(_SDT("[%s: %d]: m_stArena.astWar16To8To4Info[%u].byWin4Cnt = %u, m_stArena.astWar16To8To4Info[%u].byWin8Cnt = %u!"), MSG_MARK, i + 1, stWar16To8.byWin4Cnt, i + 1, stWar16To8.byWin8Cnt);
            }
        }
        break;
    }
    case GVG_6_FINAL_WAR_GO:
    case GVG_7_END:
        bDone = (m_stArena.stFinalWarInfo.byTotalRound + 1 >= m_stArena.stFinalWarInfo.byFactionCnt);
        if(!bDone)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone stage = %u, ArenaID = %u, bDone = false, m_stArena.stFinalWarInfo.byTotalRound = %u, m_stArena.stFinalWarInfo.byFactionCnt = %u!"), 
                MSG_MARK, eStage, GetArenaID(), 
                m_stArena.stFinalWarInfo.byTotalRound, m_stArena.stFinalWarInfo.byFactionCnt);
        }
        break;

    case GVG_4_16_TO_8_END:
    case GVG_5_8_TO_4_BEGIN:
        for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
            if(0 == stWar16To8.byReady8To4)
            {
                bDone = FALSE;
                break;
            }
        }

        if(!bDone)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone stage = %u, ArenaID = %u, bDone = false, !"), MSG_MARK, eStage, GetArenaID());
        }
        break;

    case GVG_5_8_TO_4_END:
        bDone = (0 == m_dwPullGroupID);
        if(!bDone)
        {
            DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone = false, stage = %u, ArenaID = %u, m_dwPullGroupID = %u!"), MSG_MARK, eStage, GetArenaID(), m_dwPullGroupID);
        }
        break;
    }

    return bDone;
}

BOOL CGvGArena::IsDone(GVG_STAGE eStage)
{
    // 这里依次根据3种方式结合起来检测：
    // 1. 根据各个轮次的完成标识位来判断，如果已完成，则需要进行第2种判断
    // 2. 根据逻辑进行复判
    // 3. 判断各区是否均已返回就绪应答
    BOOL bDone = IsStageDone(eStage);
    if(!bDone)
    {
        // DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone is stage done flag = false, arena = %u, stage = %u!"), MSG_MARK, eStage, GetArenaID());
        return FALSE;
    }

    bDone = IsLogicDone(eStage);

    if(bDone && eStage >= m_eStage)
    {
        // 要求各区均已返回就绪应答
        if(!IsAllZoneReady())
        {
            CNGString strReadyZoneList = gvgutil::GetZoneListStr(m_setReadyZone);
            // DBG_INFO(_SDT("[%s: %d]: CGvGArena::IsDone stage = %u, ArenaID = %u, not all zone is ready, ready zone list = <%s>!"), MSG_MARK, eStage, GetArenaID(), strReadyZoneList.c_str());
            bDone = FALSE;
        }
    }

    return bDone;
}

EachZoneFactionIDListMap CGvGArena::SplitFactionsByZone(GvGRound eRound)
{
    EachZoneFactionIDListMap mapZones;
    switch(eRound)
    {
    case GVG_16_TO_8:
        for(UINT16 wWarIdx = 0; wWarIdx < m_stArena.stWar16To8List.byWarCnt; wWarIdx++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[wWarIdx];
            for(UINT16 i = 0; i < stWar16To8.byFactionCnt; i++)
            {
                gvgutil::AppendFactionByZone(mapZones, stWar16To8.astFactionList[i]);
            }
        }
        break;

    case GVG_8_TO_4:
        for(UINT16 wWarIdx = 0; wWarIdx < m_stArena.stWar16To8List.byWarCnt; wWarIdx++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[wWarIdx];
            for(UINT16 i = 0; i < stWar16To8.byWin8Cnt; i++)
            {
                gvgutil::AppendFactionByZone(mapZones, stWar16To8.astFactionList[stWar16To8.abyWin8List[i] - 1]);
            }
        }
        break;

    case GVG_FINAL_WAR:
        {
            DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;
            for(UINT16 i = 0; i < stFinalWar.byFactionCnt; i++)
            {
                gvgutil::AppendFactionByZone(mapZones, stFinalWar.astFactionList[i]);
            }
        }
        break;
    }

    mapZones.erase(0);
    return mapZones;
}

void CGvGArena::Clear()
{
    memset(&m_stArena, 0, sizeof(m_stArena));

    ClearZones();
    ClearArrivedZone();
    ClearReadyZoneList();

    m_mapFactionLog.clear();
}

void CGvGArena::SupportFaction(GvGRound eBigRound, DT_FACTION_SUPPORT_LIST &stFactionList)
{
    if(0 == stFactionList.dwFactionCnt)
    {
        return;
    }

    FactionSupportMap mapAddSupport;
    for(UINT16 i = 0; i < stFactionList.dwFactionCnt; i++)
    {
        DT_FACTION_SUPPORT_INFO &stSupport = stFactionList.astSupportList[i];
        mapAddSupport[stSupport.dwFactionID] = stSupport.dwSupportCnt;
    }

    FactionSupportMap mapNowSupport;

    switch(eBigRound)
    {
    case GVG_16_TO_8:
    case GVG_8_TO_4:
        for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
        {
            DT_16_TO_8_TO_4_WAR_INFO &stWar16To8 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
            for(UINT16 wNo = 0/*门派序号*/; wNo < stWar16To8.byFactionCnt; wNo++)
            {
                DT_GVG_FACTION &stFaction = stWar16To8.astFactionList[wNo];
                stFaction.dwGambleCnt += mapAddSupport[stFaction.stBaseData.dwFactionID];

                mapNowSupport[stFaction.stBaseData.dwFactionID] = stFaction.dwGambleCnt;
            }
        }
        break;

    case GVG_FINAL_WAR:
        {
            DT_FINAL_WAR_INFO &stFinalWar = m_stArena.stFinalWarInfo;
            for(UINT16 wNo = 0/*门派序号*/; wNo < stFinalWar.byFactionCnt; wNo++)
            {
                DT_GVG_FACTION &stFaction = stFinalWar.astFactionList[wNo];
                stFaction.dwGambleCnt += mapAddSupport[stFaction.stBaseData.dwFactionID];

                mapNowSupport[stFaction.stBaseData.dwFactionID] = stFaction.dwGambleCnt;
            }
        }
        break;
    }

    SyncFactionSupportCnt(mapNowSupport);
}

void CGvGArena::AddPlayers(CGvGPullGroup &oPullGroup)
{
    // 检测是否全部拉取完毕
    EachZoneFactionIDListMap mapZones = SplitFactionsByZone(m_eRound);
    CPullPlayerMap& mapPullPlayers = oPullGroup.GetPlayers();

    // 根据各门派的玩家上阵列表，将玩家存入相应门派
    for(EachZoneFactionIDListMap::iterator zoneItr = mapZones.begin(); zoneItr != mapZones.end(); ++zoneItr)
    {
        UINT16 wZoneID = zoneItr->first;

        FactionIDSet &setFactionID = zoneItr->second;
        for (FactionIDSet::iterator factionIDItr = setFactionID.begin(); factionIDItr != setFactionID.end(); ++factionIDItr)
        {
            UINT32 dwFactionID = *factionIDItr;

            CPKFaction *poPKFaction = FindPKFaction(wZoneID, dwFactionID);
            if(NULL == poPKFaction)
            {
                continue;
            }

            DT_GVG_FORMATION_DATA &stMatrixData = poPKFaction->GetData().stFormation;
            CPullPlayerMap mapFactionPullPlayers;

            // 一一取出上阵玩家玩家的相应实例
            for(UINT16 bySlot = 0; bySlot < MAX_GVG_FORMATION_PLAYER_CNT; bySlot++)
            {
                UINT32 dwPlayerID = stMatrixData.adwPlayerIDList[bySlot];

                CPullPlayer *poPullPlayer = mapPullPlayers[dwPlayerID];
                if(NULL == poPullPlayer)
                {
                    continue;
                }

                mapFactionPullPlayers[dwPlayerID] = poPullPlayer;
            }

            poPKFaction->ResetMembers(mapFactionPullPlayers);
        }
    }
}

CPKZone* CGvGArena::FindPKZone(ZoneID zoneID)
{
    if(0 == zoneID)
    {
        return NULL;
    }

    CPKZoneMap::iterator itr = m_mapPKZone.find(zoneID);
    if(itr == m_mapPKZone.end())
    {
        return NULL;
    }

    CPKZone *poPKZone = itr->second;
    return poPKZone;
}

CPKFaction* CGvGArena::FindPKFaction(ZoneID zoneID, UINT32 dwFactionID)
{
    CPKZone *poPKZone = FindPKZone(zoneID);
    if(NULL == poPKZone)
    {
        return NULL;
    }

    // 在该区内寻找对应参战帮派
    return poPKZone->FindPKFaction(dwFactionID);
}

void CGvGArena::SyncFactionSupportCnt(FactionSupportMap &mapNowSupport)
{
    PKT_PKGS_SYNC_FACTION_SUPPORT_NTF stNtf;

    DT_FACTION_SUPPORT_LIST &stSupportList = stNtf.stFactionSupportInfo;
    for(FactionSupportMap::iterator itr = mapNowSupport.begin(); itr != mapNowSupport.end(); ++itr)
    {
        DT_FACTION_SUPPORT_INFO &stSupportInfo = stSupportList.astSupportList[stSupportList.dwFactionCnt++];
        stSupportInfo.dwFactionID = itr->first;
        stSupportInfo.dwSupportCnt = itr->second;

        if(stSupportList.dwFactionCnt > MAX_GVG_ARENA_FACTION_CNT)
        {
            break;
        }
    }    

    DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;
    for(UINT16 i = 0; i < stZoneIDList.byZoneCnt; i++)
    {
        UINT16 wZoneID = stZoneIDList.awZoneIDList[i];
        gsapi::SendMsg2GS(wZoneID, (CHAR*)&stNtf, PKGS_SYNC_FACTION_SUPPORT_NTF);
    }
}

void CGvGArena::BuildZone()
{
    if(!m_mapPKZone.empty())
    {
        return;
    }

    DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;

    // 为该赛区的成员区生成一个区实例，并将其分配给所属赛区
    for (UINT16 i = 0; i < stZoneIDList.byZoneCnt; i++)
    {
        UINT32 dwZoneID = stZoneIDList.awZoneIDList[i];

        // 检测是否已有指定区，如果没有则新建一个
        NewPKZone(dwZoneID);        
    }
}

void CGvGArena::ClearZones()
{
    for (CPKZoneMap::iterator itr = m_mapPKZone.begin(); itr != m_mapPKZone.end(); ++itr)
    {
        CPKZone *poPKZone = itr->second;
        poPKZone->Clear();

        delete poPKZone;
    }

    m_mapPKZone.clear();
}

CPKZone* CGvGArena::NewPKZone(ZoneID zoneID)
{
    if(0 == zoneID)
    {
        return NULL;
    }

    CPKZone *poPKZone = FindPKZone(zoneID);
    if(poPKZone)
    {
        return poPKZone;
    }

    poPKZone = new CPKZone; // 这里不用对象池

    poPKZone->m_zoneID  = zoneID;
    m_mapPKZone[zoneID] = poPKZone;

    return poPKZone;
}

void CGvGArena::StartPullPlayers()
{
    // 说明已经为本赛场建立好了拉人组，不必重新拉取
    if(m_dwPullGroupID)
    {
        if(NULL == CPullPlayerMgr::Instance()->FindPullGroup(m_dwPullGroupID))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::StartPullPlayers FindPullGroup failed, m_dwPullGroupID = %u, arenaID = %u!"), MSG_MARK, m_dwPullGroupID, GetArenaID());
            m_dwPullGroupID = 0;
        }

        return;
    }

    // 为赛场建立拉人组，这个组将一直拉、拉、拉赛场内帮派的上阵选手，直到拉完为止
    CGvGPullGroup *poNewGroup = (CGvGPullGroup*)CPullPlayerMgr::Instance()->NewGroup(EPullType_GvG);
    if(NULL == poNewGroup)
    {
        return;
    }

    poNewGroup->m_arenaID = GetArenaID();

    EachZoneFactionIDListMap mapZones = SplitFactionsByZone(m_eRound);
    for(EachZoneFactionIDListMap::iterator zoneItr = mapZones.begin(); zoneItr != mapZones.end(); ++zoneItr)
    {
        UINT16 wZoneID = zoneItr->first;
        FactionIDSet &setFaction = zoneItr->second;

        CPKZone *poPKZone = FindPKZone(wZoneID);
        if(NULL == poPKZone)
        {
            continue;
        }

        for(FactionIDSet::iterator factionIDItr = setFaction.begin(); factionIDItr != setFaction.end(); ++factionIDItr)
        {
            UINT32 dwFactionID = *factionIDItr;

            CPKFaction *poPKFaction = poPKZone->FindPKFaction(dwFactionID);
            if(NULL == poPKFaction)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::StartPullPlayers FindPKFaction failed, zoneID = %u, factionid = %u, arenaID = %u!"), MSG_MARK, wZoneID, dwFactionID, GetArenaID());
                return;
            }

            if(poPKFaction->IsMatrixEmpty())
            {
                USR_INFO(_SDT("[%s: %d]: CGvGMgr::StartPullArena matrix is empty, zoneID = %u, factionid = %u, arenaID = %u!"), MSG_MARK, wZoneID, dwFactionID, GetArenaID());
                continue;
            }

            poNewGroup->AddPullPlayerIDList(*poPKFaction);
        }
    }

    poNewGroup->Pull();
    m_dwPullGroupID = poNewGroup->GetGroupID();
}

void CGvGArena::GetEachZoneTop8()
{
    PKT_PKGS_GVG_GET_TOP_8_REQ req;
    memset(&req, 0, sizeof(req));

    if(IsDone(GVG_3_CHAOS_WAR_END))
    {
        return;
    }

    // 向本赛区的各个区索取前8强门派名单
    DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;
    for(UINT16 i = 0; i < stZoneIDList.byZoneCnt; i++)
    {
        ZoneID zoneID = stZoneIDList.awZoneIDList[i];
        gsapi::SendMsg2GS(zoneID, (CHAR*)&req, PKGS_GVG_GET_TOP_8_REQ);
    }
}

void CGvGArena::TakeArenaVideo(GvGVideoPointerMap &mapVideos)
{
    for(UINT16 i = 0; i < m_stArena.stFactionsLog.wFactionCnt; i++)
    {
        DT_FACTION_SIMPLE_GVG_LOG_LIST &stLogList = m_stArena.stFactionsLog.astLogList[i];
        for(UINT16 wLogIdx = 0; wLogIdx < stLogList.wLogCnt; wLogIdx++)
        {
            DT_FACTION_SIMPLE_GVG_LOG &stLog = stLogList.astLogs[wLogIdx];

            if(mapVideos.find(stLog.qwVideoID) != mapVideos.end())
            {
                continue;
            }

            SGvGVideo *pstVideo = CGvGVideoMgr::Instance()->GetVideo(stLog.qwVideoID);
            if(NULL == pstVideo)
            {
                continue;
            }

            mapVideos[stLog.qwVideoID] = pstVideo;
        }
    }
}

ZoneIDSet CGvGArena::GetThisArenaZoneIDSet()
{
    ZoneIDSet zoneIDSet;

    DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;
    for(UINT16 byZoneIdx = 0; byZoneIdx < stZoneIDList.byZoneCnt; byZoneIdx++)
    {
        ZoneID zoneID = stZoneIDList.awZoneIDList[byZoneIdx];
        zoneIDSet.insert(zoneID);
    }

    CGvGPropMgr::ArenaZoneIDMap &mapArena = CGvGPropMgr::Instance()->GetArenaZoneIDMap();
    CGvGPropMgr::ArenaZoneIDMap::iterator itr = mapArena.find(m_stArena.wArenaID);
    if (itr != mapArena.end())
    {
        GroupZoneIDMap &mapGroupZoneID = itr->second;

        for(GroupZoneIDMap::iterator groupItr = mapGroupZoneID.begin(); groupItr != mapGroupZoneID.end(); ++groupItr)
        {
            ZoneIDVec &vecZoneID = groupItr->second;
            zoneIDSet.insert(vecZoneID.begin(), vecZoneID.end());
        }        
    }

    return zoneIDSet;
}

void CGvGArena::SyncVideo(ZoneID targetZoneID /* = 0 */)
{
    PKT_PKGS_SYNC_VIDEO_NTF stNtf;
    PKT_PKGS_RET_VIDEO_ACK *pstAck = (PKT_PKGS_RET_VIDEO_ACK*)CPKPktBuilder::Instance()->GetPacketBuffer();

    GvGVideoPointerMap mapVideos;
    TakeArenaVideo(mapVideos);

    if(mapVideos.empty())
    {
        return;
    }

    memset(&stNtf, 0, sizeof(stNtf));
    for (GvGVideoPointerMap::iterator videoItr = mapVideos.begin(); videoItr != mapVideos.end(); ++videoItr)
    {
        SGvGVideo *pstVideo = videoItr->second;
        stNtf.astVideoList[stNtf.wVideoCnt++] = pstVideo->stGvGInfo;

        if(stNtf.wVideoCnt >= GVG_VIDEO_SYNC_CNT_ONCE)
        {
            break;
        }
    }

    ZoneIDSet zoneIDSet;
    if(0 == targetZoneID)
    {
        zoneIDSet = GetThisArenaZoneIDSet();
    }
    else
    {
        zoneIDSet.insert(targetZoneID);        
    }

    for(ZoneIDSet::iterator zoneIDItr = zoneIDSet.begin(); zoneIDItr != zoneIDSet.end(); ++zoneIDItr)
    {
        ZoneID zoneID = *zoneIDItr;
        if(FALSE == gsapi::IsConnectingToGS(zoneID))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::SyncVideo failed, can not find connection to zone<%u>, arenaID = %u!"), MSG_MARK, zoneID, GetArenaID());
            continue;
        }

        gsapi::SendMsg2GS(zoneID, (CHAR*)&stNtf, PKGS_SYNC_VIDEO_NTF);

        for (GvGVideoPointerMap::iterator videoItr = mapVideos.begin(); videoItr != mapVideos.end(); ++videoItr)
        {
            SGvGVideo *pstVideo = videoItr->second;
            SendPvPVideo(zoneID, *pstVideo, *pstAck);
        }
    }
}

void CGvGArena::SyncActivity(ZoneID targetZoneID /* = 0 */)
{
    PKT_PKGS_SYNC_GVG_NTF *pstNtf = (PKT_PKGS_SYNC_GVG_NTF*)CPKPktBuilder::Instance()->GetPacketBuffer();
    DT_GVG_LOCAL_ACTIVITY_DATA &stLocal = pstNtf->stGvGInfo;

    TakeDataToStruct();

    CGvGMgr::Instance()->GlobalGvGToLocalGvG(stLocal, m_stArena);

    ZoneIDSet zoneIDSet;
    if(0 == targetZoneID)
    {
        zoneIDSet = GetThisArenaZoneIDSet();
    }
    else
    {
        zoneIDSet.insert(targetZoneID);        
    }

    for(ZoneIDSet::iterator itr = zoneIDSet.begin(); itr != zoneIDSet.end(); ++itr)
    {
        ZoneID zoneID = *itr;
        if(FALSE == gsapi::IsConnectingToGS(zoneID))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::SyncActivity failed, can not find connection to zone<%u>, arenaID = %u!"), MSG_MARK, zoneID, GetArenaID());
            continue;
        }

        gsapi::SendMsg2GS(zoneID, (CHAR*)pstNtf, PKGS_SYNC_GVG_NTF);
    }
}

void CGvGArena::SendPvPVideo(ZoneID zoneID, SGvGVideo &stVideo, PKT_PKGS_RET_VIDEO_ACK &stAck)
{
    memset(&stAck, 0, sizeof(stAck));

    stAck.qwVideoID = stVideo.stGvGInfo.qwVideoID;
    stAck.wPvPVideoCnt = 0;
    stAck.byStartSlot = 0;

    for(UINT8 i = 0; i < stVideo.vecPvPVideo.size(); i++)
    {
        if(GVG_PK_VIDEO_RET_CNT_ONCE == stAck.wPvPVideoCnt)
        {            
            gsapi::SendMsg2GS(zoneID, (CHAR*)&stAck, PKGS_RET_VIDEO_ACK);

            stAck.wPvPVideoCnt = 0;
            stAck.byStartSlot = i;
        }

        stAck.astPvPVideoList[stAck.wPvPVideoCnt] = stVideo.vecPvPVideo[stAck.byStartSlot + stAck.wPvPVideoCnt];
        stAck.wPvPVideoCnt++;
    }

    if(stAck.wPvPVideoCnt > 0)
    {
        gsapi::SendMsg2GS(zoneID, (CHAR*)&stAck, PKGS_RET_VIDEO_ACK);
    }
}

BOOL CGvGArena::IsStageDone(GVG_STAGE eStage)
{
    if(eStage >= GVG_STAGE_MAX)
    {
        return FALSE;
    }

    BOOL bDone = m_stArena.stStageDoneFlagList.abyFlagList[eStage];
    return bDone;
}

void CGvGArena::SetStageDone(GVG_STAGE eStage)
{
    if(eStage >= GVG_STAGE_MAX)
    {
        return;
    }

    m_stArena.stStageDoneFlagList.abyFlagList[eStage] = TRUE;
}

void CGvGArena::ResetStageFlag()
{
    memset(&m_stArena.stStageDoneFlagList, 0, sizeof(m_stArena.stStageDoneFlagList));
}

BOOL CGvGArena::War16To8End()
{
    // 每个配对的门派间进行对战，胜者进入胜者组
    for(UINT16 i = 0; i < m_stArena.stWar16To8List.byWarCnt; i++)
    {
        DT_16_TO_8_TO_4_WAR_INFO &stWar8To4 = m_stArena.stWar16To8List.astWar16To8To4Info[i];
        if(stWar8To4.byReady8To4)
        {
            continue;
        }

        stWar8To4.byReady8To4 = TRUE;

        UINT16 wGamebleIdx = 0;


        // 将16进8的胜出门派两两配对进行对战
        for(BYTE byNo = 0/*门派序号*/; byNo + 1 < GVG_16_TO_8_WIN_CNT; byNo += 2)
        {
            // 寻找配对对手，如果找不到配对对手，则将对手指向到空门派
            UINT16 dwFaction1Idx = (stWar8To4.abyWin8List[byNo] > 0 ? stWar8To4.abyWin8List[byNo] - 1 : 2 * byNo);
            UINT16 dwFaction2Idx = (stWar8To4.abyWin8List[byNo + 1] > 0 ? stWar8To4.abyWin8List[byNo + 1] - 1 : 2 * (byNo + 1));

            DT_GVG_FACTION &stFaction1 = stWar8To4.astFactionList[dwFaction1Idx];
            DT_GVG_FACTION &stFaction2 = stWar8To4.astFactionList[dwFaction2Idx];

            UINT32 dwFactionID1 = stFaction1.stBaseData.dwFactionID;
            UINT32 dwFactionID2 = stFaction2.stBaseData.dwFactionID;
            if(0 == dwFactionID1 && 0 == dwFactionID2)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::War8To4Begin ArenaID = %u, can't find faction<%u> at pos<%u> & faction<%u> at pos<%u>, !"), 
                    MSG_MARK, GetArenaID(), dwFactionID1, 2 * byNo + 1, dwFactionID2, 2 * byNo + 2);
                continue;
            }

            VideoID videoID = CGvGVideoMgr::Instance()->MakeVideoID(dwFactionID1, dwFactionID2);

            stWar8To4.aqwVideoIDList8To4[wGamebleIdx++] = videoID;
            AddNotStartLog(dwFactionID1, dwFactionID2, videoID, GVG_8_TO_4);
        }
    }

    ClearWar();

    SetStageDone(GVG_4_16_TO_8_END);
    return TRUE;
}

BOOL CGvGArena::EnterStage(GVG_STAGE eTargetStage)
{
    if(eTargetStage < m_eStage)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::EnterStage error: eTargetStage<%u> < m_eStage<%u>!"), MSG_MARK, eTargetStage, m_eStage);
        return FALSE;
    }

    // 如果本赛区当前轮次远落后于正常轮次，则让本赛区一轮一轮往下跳，直到追上正常轮次
    if(eTargetStage > m_eStage + 1)
    {
        eTargetStage = gvgutil::GetNextStage(m_eStage);
    }

    CheckEachZoneReady();
    
    if(eTargetStage != m_eStage)
    {
        if(!IsDone(m_eStage))
        {
            EnterStage(m_eStage);
            return FALSE;
        }
    }    

    if(eTargetStage == m_eStage && IsDone(eTargetStage))
    {
        return TRUE;
    }

    BOOL bSucc = TRUE;
    switch(eTargetStage)
    {
    case GVG_1_APPLY:
        GvGStart();
        break;

    case GVG_2_MONEY:
    case GVG_3_CHAOS_WAR_BEGIN:
        SetStageDone(eTargetStage);
        break;

    case GVG_3_CHAOS_WAR_END:
        ChaosWarEnd();
        break;

        // 混合双服淘汰赛
    case GVG_4_16_TO_8_BEGIN:
        bSucc = War16To8Begin();
        break;

    case GVG_5_8_TO_4_BEGIN:
        bSucc = War8To4Begin();
        break;

    case GVG_6_FINAL_WAR_BEGIN:
        bSucc = FinalWarBegin();
        break;

    case GVG_4_16_TO_8_GO:
    case GVG_5_8_TO_4_GO:
        bSucc = PrepareWarGo(eTargetStage);
        break;

    case GVG_6_FINAL_WAR_GO:
        PrepareFinalWarGo();
        break;

    case GVG_4_16_TO_8_END:
        bSucc = War16To8End();
        break;

    case GVG_5_8_TO_4_END:
        bSucc = WarEnd(eTargetStage);
        break;

    case GVG_7_END:
        GvGOver();
        break;

    case GVG_8_SLEEPING:
        GvGSleep();
        break;
    }

    // 如果进入下一回合失败，则说明前一回合并未进展完，所以，此时应拒绝进入下一回合
    if(!bSucc)
    {
        NotifyStage(m_eStage);
        return FALSE;
    }

    // 如果已经进入本轮，则不需要再保存了
    if(m_eStage == eTargetStage)
    {
        // 打印跟踪日志
        // SYS_CRITICAL(_SDT("[%s: %d]: CGvGArena::EnterStage success: eTargetStage<%u> == m_eStage<%u>!"), MSG_MARK, eTargetStage, m_eStage);
        NotifyStage(m_eStage);
        return TRUE;
    }

    SetStage(eTargetStage);

    NotifyStage(m_eStage);

    ClearReadyZoneList();
    SyncActivity();
    SaveToDB();

    return TRUE;
}

void CGvGArena::NotifyStage(GVG_STAGE eStage)
{
    PKT_PKGS_GVG_NEXT_ROUND_NTF ntf;
    memset(&ntf, 0, sizeof(ntf));

    ntf.byRound = eStage;

    DT_ZONE_ID_LIST &stZoneIDList = m_stArena.stZoneIDList;

    // 向各个区派发通知
    for(UINT16 byZoneIdx = 0; byZoneIdx < stZoneIDList.byZoneCnt; byZoneIdx++)
    {
        ZoneID zoneID = stZoneIDList.awZoneIDList[byZoneIdx];
        gsapi::SendMsg2GS(zoneID, (CHAR*)&ntf, PKGS_GVG_NEXT_ROUND_NTF);
    }
}

void CGvGArena::UpdateRound()
{
    m_eRound = gvgutil::Stage2Round(m_eStage);
}

void CGvGArena::SetStage(GVG_STAGE eStage)
{
    m_eStage = eStage;
    UpdateRound();
}