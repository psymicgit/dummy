///<------------------------------------------------------------------------------
//< @file:   gvgpropmgr.cpp
//< @author: hongkunan
//< @date:   2014年1月16日 20:29:0
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "gvgpropmgr.h"
#include "logic/other/errmsgmgr.h"
#include "framework/gsapi.h"
#include "errdef.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/gvg/global/gvgevent.h"
#include <sdloggerimpl.h>
#include <common/server/utility.h>
#include "db/dbmgr.h"

namespace gvgtimeutil
{
    // 从字符串12:20解析出秒数，如: 08:00 -> 480
    UINT32 ParseTime(const CHAR *szTime)
    {
        UINT8 byColonIdx = 0;
        while(szTime[byColonIdx] && szTime[byColonIdx] != ':')
        { 
            ++byColonIdx; 
        }

        CHAR szHour[3] = {0};
        memcpy(szHour, szTime, byColonIdx);
        szHour[byColonIdx] = '\0';

        BYTE byHour   = SDAtou(szHour);
        BYTE byMinute = SDAtou(szTime + byColonIdx + 1);

        UINT32 dwSeconds = byHour * 3600 + byMinute * 60;
        return dwSeconds;
    }

    bool CmpKillCombo(SGvGKillComboProp &a, SGvGKillComboProp &b){  
        return a.wKillCombo < b.wKillCombo;
    }

    UINT32 GetTimeInWeek(UINT8 byWeek, UINT32 dwTime)
    {
        byWeek = MAX(1, byWeek);
        return (byWeek - 1) * SECONDES_PER_DAY + dwTime;
    }
}

IMPLEMENT_SINGLETON_PROPMGR(CGvGPropMgr);

BOOL CGvGPropMgr::Init()
{
    Clear();

    if(!LoadGvGPropDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }
	
    if(!LoadGvGTimePropDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGTimePropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadGvGOpenDatePropDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGOpenDatePropDB failed!"), MSG_MARK);
        return FALSE;
    }

    if (!LoadGvGArenaPropDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGArenaPropDB failed!"), MSG_MARK);
        return FALSE;
    }

    if (!LoadGvGUnLockPropDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGUnLockPropDB failed!"), MSG_MARK);
        return FALSE;
    }

    if (!LoadZoneNameDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadZoneNameDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadGvGKillComboPropDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGKillComboPropDB failed!"), MSG_MARK);
        return FALSE;
    }

	if( !LoadGvGFormationPropFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGFormationPropFromDB failed!"), MSG_MARK);
		return FALSE;
	}

	if (!LoadGvGGiftPropFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGGiftPropFromDB failed!"), MSG_MARK);
		return FALSE;
	}

    if (!LoadGvGGambleAwardPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGvGGambleAwardPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    string strRule = CMsgDefMgr::Instance()->GetErrMsg("GVG_RULE");
    SDStrncpy(m_aszGvGRule, strRule.c_str(), MAX_GVG_RULE - 1);

    if(!TranslateIntoStageSchedule())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::Init TranslateIntoStageSchedule failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CGvGPropMgr::UnInit()
{
	Clear();
}

VOID CGvGPropMgr::Clear()
{
    memset(&m_stGvGProp, 0, sizeof(m_stGvGProp));
    memset(&m_stDisplaySchedule, 0, sizeof(m_stDisplaySchedule));

    m_vecRoundTime.clear();
	m_mapFormationProp.clear();
	m_mapPosNumToFactionLevel.clear();
    m_mapArenZoneID.clear();
    m_vecStageTime.clear();
}

void CGvGPropMgr::FakeGvGTime(UINT64 qwActivityStartTime, GVG_STAGE eNowStage)
{
    if (!IsFakeTime())
    {
        return;
    }

    if(0 == qwActivityStartTime)
    {
        qwActivityStartTime = SDTimeSecs();
    }

    CSDDateTime oStart(qwActivityStartTime);
    UINT8 byWeek = oStart.GetWeek();
    byWeek = (0 == byWeek ? 7 : byWeek);
        
    UINT32 dwGap = m_stGvGProp.dwFakeGap;

    UINT32 dwNow = GetDayPassSecond(qwActivityStartTime);
    if(GVG_8_SLEEPING == eNowStage)
    {
        dwNow += m_vecRoundTime[GVG_END].dwEndTime - m_vecRoundTime[GVG_APPLY].dwStartTime + dwGap;
    }

    UINT32 dwNextTime = dwNow;

    for (UINT16 i = GVG_APPLY; i <= GVG_END; i++)
    {
        DT_GVG_TIME_INFO &stTime = m_vecRoundTime[i];
        memset(&stTime, 0, sizeof(stTime));

        stTime.byStartWeek = 1;
        stTime.byEndWeek = 1;
    }

    m_vecRoundTime[GVG_APPLY].dwStartTime = dwNow;
    m_vecRoundTime[GVG_MONEY_WAR].dwStartTime = m_vecRoundTime[GVG_APPLY].dwStartTime + 2 * dwGap;
    m_vecRoundTime[GVG_CHAOS_WAR].dwStartTime = m_vecRoundTime[GVG_MONEY_WAR].dwStartTime + dwGap;
    m_vecRoundTime[GVG_CHAOS_WAR].dwEndTime   = m_vecRoundTime[GVG_CHAOS_WAR].dwStartTime + 3 * m_stGvGProp.dwChaosWarEachRoundTime;

    m_vecRoundTime[GVG_16_TO_8].dwStartTime = m_vecRoundTime[GVG_CHAOS_WAR].dwEndTime + dwGap;
    m_vecRoundTime[GVG_16_TO_8].dwEndTime = m_vecRoundTime[GVG_16_TO_8].dwStartTime + dwGap + m_stGvGProp.dwForbidFormationTime;

    m_vecRoundTime[GVG_8_TO_4].dwStartTime = m_vecRoundTime[GVG_16_TO_8].dwEndTime + dwGap / 2;
    m_vecRoundTime[GVG_8_TO_4].dwEndTime = m_vecRoundTime[GVG_8_TO_4].dwStartTime + dwGap + m_stGvGProp.dwForbidFormationTime;

    m_vecRoundTime[GVG_FINAL_WAR].dwStartTime = m_vecRoundTime[GVG_8_TO_4].dwEndTime + dwGap;
    m_vecRoundTime[GVG_FINAL_WAR].dwEndTime = m_vecRoundTime[GVG_FINAL_WAR].dwStartTime + dwGap;

    m_vecRoundTime[GVG_END].dwStartTime = m_vecRoundTime[GVG_FINAL_WAR].dwEndTime + m_stGvGProp.dwFinalWarEachRoundTime * GVG_FINAL_WAR_FACTION_CNT;
    m_vecRoundTime[GVG_END].dwEndTime   = m_vecRoundTime[GVG_END].dwStartTime + dwGap;

    UINT64 qwTodayZero = gvgutil::GetTodayZero();
    std::string strTodayZero = SDTimeToString(CSDDateTime(qwTodayZero));
    for (UINT16 i = 0; i < m_vecOpenDate.size(); i++)
    {
        m_vecOpenDate[i] = qwTodayZero;
    }

    ConvertToClientDisplayTimeByDate(qwActivityStartTime);
    TranslateIntoStageSchedule();
}

UINT64 CGvGPropMgr::GetOpenDateBySession(UINT16 wSessionID)
{
    if(wSessionID >= m_vecOpenDate.size())
    {
        return 0;
    }

    return m_vecOpenDate[wSessionID];
}

UINT64 CGvGPropMgr::GetOpenDateByArena(ArenaID arenaID)
{
    CGvGPropMgr::ArenaZoneIDMap::iterator itr = m_mapArenZoneID.find(arenaID);
    if (itr == m_mapArenZoneID.end())
    {
        return 0;
    }

    UINT16 wUnLockSession = GetUnLockSession(arenaID);
    if (0 == wUnLockSession)
    {
        return 0;
    }

    UINT64 qwOpenDate = GetOpenDateBySession(wUnLockSession);
    return qwOpenDate;
}

UINT64 CGvGPropMgr::GetOpenDateByZone(ZoneID zoneID)
{
    ArenaID arenaID = GetArenaIDByZoneID(zoneID);
    return GetOpenDateByArena(arenaID);
}

const std::string& CGvGPropMgr::GetZoneName(ZoneID zoneID)
{
    std::string &strZoneName = m_mapZoneName[zoneID];

    // 如果区名还未配好，则暂时显示为: xx区
    if(strZoneName.empty())
    {
        std::vector<string> vecFillMsg;
        CHAR szBuff[64] = {0};
        sprintf(szBuff, "%u", zoneID);
        vecFillMsg.push_back(szBuff);
        strZoneName = CMsgDefMgr::Instance()->GetErrMsg("ZONE_NAME", &vecFillMsg);

        if(strZoneName.empty())
        {
            strZoneName = szBuff;
        }
    }

    return strZoneName;
}

ArenaID CGvGPropMgr::GetArenaIDByZoneID(ZoneID zoneID)
{
    ZoneIDToArenaIDMap::iterator itr = m_mapZoneIDToArenaID.find(zoneID);
    if (itr == m_mapZoneIDToArenaID.end())
    {
        return 0;
    }

    return itr->second;
}

BOOL CGvGPropMgr::IsArenaOpenUp(ArenaID arenaID)
{
    if(0 == arenaID)
    {
        return FALSE;
    }

    UINT64 qwOpenDate = GetOpenDateByArena(arenaID);
    if(0 == qwOpenDate)
    {
        return FALSE;
    }

    UINT64 qwNow = SDTimeSecs();
    return qwOpenDate <= qwNow;
}

void CGvGPropMgr::PrintNotOpenUpArenaZone(ArenaID arenaID)
{
    if(0 == arenaID)
    {
        return;
    }

    CGvGPropMgr::ArenaZoneIDMap::iterator itr = m_mapArenZoneID.find(arenaID);
    if (itr == m_mapArenZoneID.end())
    {
        return;
    }

    GroupZoneIDMap& mapGroupZoneID = itr->second;
    if(mapGroupZoneID.empty())
    {
        return;
    }

    for(GroupZoneIDMap::iterator itr = mapGroupZoneID.begin(); itr != mapGroupZoneID.end(); ++itr)
    {
        UINT8 byGroupID = itr->first;
        ZoneIDVec &vecZoneID = itr->second;

        for(UINT16 i = 0; i < vecZoneID.size(); i++)
        {
            UINT16 wZoneID = vecZoneID[i];
            USR_INFO(_SDT("[%s: %d]: CGvGMgr::PrintNotOpenUpArenaZone, zone %u of group %u in arena %u has not open up gvg activity!"), 
                MSG_MARK, wZoneID, byGroupID, arenaID);
        }
    }
}

UINT16 CGvGPropMgr::GetUnLockSession(ArenaID arenaID)
{
    UnLockMap::iterator itr = m_mapUnLock.find(arenaID);
    if (itr == m_mapUnLock.end())
    {
        return 0;
    }

    return itr->second;
}

BOOL CGvGPropMgr::TranslateIntoStageSchedule()
{
    m_vecStageTime.clear();
    m_vecStageTime.resize(GVG_STAGE_MAX);

    if(0 == m_stGvGProp.dwForbidFormationTime)
    {
        return FALSE;
    }

    m_vecStageTime[GVG_1_APPLY]           = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_APPLY].byStartWeek, m_vecRoundTime[GVG_APPLY].dwStartTime);
    m_vecStageTime[GVG_2_MONEY]           = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_MONEY_WAR].byStartWeek, m_vecRoundTime[GVG_MONEY_WAR].dwStartTime);

    m_vecStageTime[GVG_3_CHAOS_WAR_BEGIN] = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_CHAOS_WAR].byStartWeek, m_vecRoundTime[GVG_CHAOS_WAR].dwStartTime);
    m_vecStageTime[GVG_3_CHAOS_WAR_END]   = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_CHAOS_WAR].byEndWeek, m_vecRoundTime[GVG_CHAOS_WAR].dwEndTime);

    m_vecStageTime[GVG_4_16_TO_8_BEGIN]   = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_16_TO_8].byStartWeek, m_vecRoundTime[GVG_16_TO_8].dwStartTime);
    m_vecStageTime[GVG_4_16_TO_8_END]     = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_16_TO_8].byEndWeek, m_vecRoundTime[GVG_16_TO_8].dwEndTime);
    m_vecStageTime[GVG_4_16_TO_8_GO]      = m_vecStageTime[GVG_4_16_TO_8_END] - m_stGvGProp.dwForbidFormationTime;

    m_vecStageTime[GVG_5_8_TO_4_BEGIN]    = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_8_TO_4].byStartWeek, m_vecRoundTime[GVG_8_TO_4].dwStartTime);
    m_vecStageTime[GVG_5_8_TO_4_END]      = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_8_TO_4].byEndWeek, m_vecRoundTime[GVG_8_TO_4].dwEndTime);
    m_vecStageTime[GVG_5_8_TO_4_GO]       = m_vecStageTime[GVG_5_8_TO_4_END] - m_stGvGProp.dwForbidFormationTime;

    m_vecStageTime[GVG_6_FINAL_WAR_BEGIN] = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_FINAL_WAR].byStartWeek, m_vecRoundTime[GVG_FINAL_WAR].dwStartTime);
    m_vecStageTime[GVG_6_FINAL_WAR_GO]    = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_FINAL_WAR].byEndWeek, m_vecRoundTime[GVG_FINAL_WAR].dwEndTime);

    m_vecStageTime[GVG_7_END]             = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_END].byStartWeek, m_vecRoundTime[GVG_END].dwStartTime);
    m_vecStageTime[GVG_8_SLEEPING]        = gvgtimeutil::GetTimeInWeek(m_vecRoundTime[GVG_END].byEndWeek, m_vecRoundTime[GVG_END].dwEndTime);

    for (UINT16 i = GVG_1_APPLY; i < GVG_7_END; i++)
    {
        if(m_vecStageTime[i + 1] <= m_vecStageTime[i])
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::TranslateIntoStageSchedule failed, detecting stage %u <= stage %u!"), MSG_MARK, i + 1, i);
            return FALSE;
        }
    }

    return TRUE;
}

VOID CGvGPropMgr::ConvertToClientDisplayTimeByDate(UINT64 qwOpenDate)
{
    CSDDateTime oOpenDate(qwOpenDate);
    UINT32 byDay1To7 = oOpenDate.GetWeek();
    if (0 == byDay1To7){
        byDay1To7 = 7;
    }

    memset(&m_stDisplaySchedule, 0, sizeof(m_stDisplaySchedule));

    SGvGRoundTimeVec vecCopyGvGTime = m_vecRoundTime;
    for(UINT16 i = GVG_SLEEPING + 1; i < GVG_MAX; i++){
        DT_GVG_TIME_INFO &stTimeInfo = vecCopyGvGTime[i];
        stTimeInfo.byStartWeek += byDay1To7 - 1;
        stTimeInfo.byEndWeek   += byDay1To7 - 1;

        if(stTimeInfo.byStartWeek > 7){
            stTimeInfo.byStartWeek %= 7;
        }

        if(stTimeInfo.byEndWeek > 7){
            stTimeInfo.byEndWeek %= 7;
        }
    }

    m_stDisplaySchedule.stApply       = vecCopyGvGTime[GVG_APPLY];
    m_stDisplaySchedule.stMoneyGame   = vecCopyGvGTime[GVG_MONEY_WAR];
    m_stDisplaySchedule.stChaosWar    = vecCopyGvGTime[GVG_CHAOS_WAR];
    m_stDisplaySchedule.stDouble16TO8 = vecCopyGvGTime[GVG_16_TO_8];
    m_stDisplaySchedule.stDouble8TO4  = vecCopyGvGTime[GVG_8_TO_4];
    m_stDisplaySchedule.stFinalWar    = vecCopyGvGTime[GVG_FINAL_WAR];

    m_stDisplaySchedule.stApply.byStartWeek = m_stDisplaySchedule.stApply.byStartWeek;
    m_stDisplaySchedule.stApply.dwStartTime = m_stDisplaySchedule.stApply.dwStartTime;

    m_stDisplaySchedule.stApply.byEndWeek = 0;
    m_stDisplaySchedule.stApply.dwEndTime = 0;

    m_stDisplaySchedule.stMoneyGame.byEndWeek = m_stDisplaySchedule.stChaosWar.byStartWeek;
    m_stDisplaySchedule.stMoneyGame.dwEndTime = m_stDisplaySchedule.stChaosWar.dwStartTime;

    m_stDisplaySchedule.stDouble16TO8.byStartWeek = m_stDisplaySchedule.stDouble16TO8.byEndWeek;
    m_stDisplaySchedule.stDouble16TO8.dwStartTime = m_stDisplaySchedule.stDouble16TO8.dwEndTime;
    m_stDisplaySchedule.stDouble16TO8.byEndWeek   = 0;
    m_stDisplaySchedule.stDouble16TO8.dwEndTime   = 0;

    m_stDisplaySchedule.stDouble8TO4.byStartWeek = m_stDisplaySchedule.stDouble8TO4.byEndWeek;
    m_stDisplaySchedule.stDouble8TO4.dwStartTime = m_stDisplaySchedule.stDouble8TO4.dwEndTime;
    m_stDisplaySchedule.stDouble8TO4.byEndWeek   = 0;
    m_stDisplaySchedule.stDouble8TO4.dwEndTime   = 0;

    m_stDisplaySchedule.stFinalWar.byStartWeek = m_stDisplaySchedule.stFinalWar.byEndWeek;
    m_stDisplaySchedule.stFinalWar.dwStartTime = m_stDisplaySchedule.stFinalWar.dwEndTime;
    m_stDisplaySchedule.stFinalWar.byEndWeek   = vecCopyGvGTime[GVG_END].byStartWeek;
    m_stDisplaySchedule.stFinalWar.dwEndTime   = vecCopyGvGTime[GVG_END].dwStartTime;
}

VOID CGvGPropMgr::ConvertToClientDisplayTime(UINT16 wSessionID)
{
    if (IsFakeTime()){
        return;
    }

    UINT64 qwOpenDate = GetOpenDateBySession(wSessionID);
    if (0 == qwOpenDate){
        return;
    }

    ConvertToClientDisplayTimeByDate(qwOpenDate);
}

BOOL CGvGPropMgr::LoadZoneNameDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;

    m_mapZoneName.clear();

    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gamezones order by zoneid asc", &pRes, &dwErrID,  &strErr);
    if(SDDB_NO_RECORDSET == nRet)
    {
        return TRUE;
    }

    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadZoneNameDB has no data!"), MSG_MARK);
        return FALSE; //
    }

    while(pRes->GetRecord())
    {
        UINT16 wZoneID          = SGDP::SDAtou(pRes->GetFieldValueByName("zoneid"));
        std::string strZoneName = pRes->GetFieldValueByName("name");

        m_mapZoneName[wZoneID] = strZoneName;
    }

    if(pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

BOOL CGvGPropMgr::LoadGvGPropDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgprop", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGPropFromDB has no data!"), MSG_MARK);
        return TRUE; //
    }

    if(pRes->GetRecord())
    {
        m_stGvGProp.dwForbidFormationTime     = SGDP::SDAtou(pRes->GetFieldValueByName("ForbidFormationTime"));
        m_stGvGProp.wRePullInterval           = SGDP::SDAtou(pRes->GetFieldValueByName("RePullInterval"));
        m_stGvGProp.wWinAddScore              = SGDP::SDAtou(pRes->GetFieldValueByName("WinAddScore"));
        m_stGvGProp.wLoseAddScore             = SGDP::SDAtou(pRes->GetFieldValueByName("LoseAddScore"));
        m_stGvGProp.dwChaosWarEachRoundTime   = SGDP::SDAtou(pRes->GetFieldValueByName("ChaosWarEachRoundTime"));
        m_stGvGProp.dwFinalWarEachRoundTime   = SGDP::SDAtou(pRes->GetFieldValueByName("FinalWarEachRoundTime"));

        m_stGvGProp.wJoinGVGPlayerLevel       = SGDP::SDAtou(pRes->GetFieldValueByName("JoinGVGPlayerLevel"));
        m_stGvGProp.dwJoinGVGFactionLevel     = SGDP::SDAtou(pRes->GetFieldValueByName("JoinGVGFactionLevel"));
        m_stGvGProp.dwJoinGVGRank             = SGDP::SDAtou(pRes->GetFieldValueByName("JoinGvGRank"));
        m_stGvGProp.wHonorLevel               = SGDP::SDAtou(pRes->GetFieldValueByName("HonorLevel"));

        m_stGvGProp.wAutoJoinFactionCnt       = SGDP::SDAtou(pRes->GetFieldValueByName("AutoJoinFactionCnt"));
        m_stGvGProp.wAutoFormationCnt         = SGDP::SDAtou(pRes->GetFieldValueByName("AutoFormationCnt"));
        m_stGvGProp.bFakeActTime              = SGDP::SDAtou(pRes->GetFieldValueByName("IsFakeActTime"));        
        m_stGvGProp.bTest                     = SGDP::SDAtou(pRes->GetFieldValueByName("IsTest"));
                                              
        m_stGvGProp.dwFakeGap                 = SGDP::SDAtou(pRes->GetFieldValueByName("FakeGap"));
        m_stGvGProp.wOpenUpRequiredFactionCnt = SGDP::SDAtou(pRes->GetFieldValueByName("OpenUpRequiredFactionCnt"));
        m_stGvGProp.wAutoApplyExceptZone      = SGDP::SDAtou(pRes->GetFieldValueByName("AutoApplyExceptZone"));
        m_stGvGProp.wDisplayVideoMVPCnt       = SGDP::SDAtou(pRes->GetFieldValueByName("DisplayVideoMVPCnt"));
        m_stGvGProp.bUsePlayerSnapShot        = SGDP::SDAtou(pRes->GetFieldValueByName("UsePlayerSnapShot"));
        m_stGvGProp.bOpenActivity             = SGDP::SDAtou(pRes->GetFieldValueByName("IsOpenActivity"));
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

BOOL CGvGPropMgr::LoadGvGTimePropDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgtimeprop order by Round asc", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGTimePropFromDB has no data!"), MSG_MARK);
        return TRUE; //
    }

    DT_GVG_TIME_INFO stGvGTimeInfo;
    memset(&stGvGTimeInfo, 0, sizeof(stGvGTimeInfo));

    m_vecRoundTime.clear();
    m_vecRoundTime.resize(GVG_MAX);
    m_vecRoundTime[0] = stGvGTimeInfo;

    while(pRes->GetRecord())
    {
        GvGRound eRound = (GvGRound)SGDP::SDAtou(pRes->GetFieldValueByName("Round"));
        if(eRound >= GVG_MAX)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGTimePropFromDB detecting invalid round = %u, aborted!"), MSG_MARK, eRound);
            return FALSE;
        }

        stGvGTimeInfo.byStartWeek = SGDP::SDAtou(pRes->GetFieldValueByName("BeginDay"));
        stGvGTimeInfo.dwStartTime = gvgtimeutil::ParseTime(pRes->GetFieldValueByName("BeginTime"));

        stGvGTimeInfo.byEndWeek = SGDP::SDAtou(pRes->GetFieldValueByName("EndDay"));
        stGvGTimeInfo.dwEndTime = gvgtimeutil::ParseTime(pRes->GetFieldValueByName("EndTime"));

        m_vecRoundTime[eRound] = stGvGTimeInfo;
    }

    if(pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    for(UINT16 i = 1; i < GVG_MAX - 1; i++)
    {
        DT_GVG_TIME_INFO &stTime = m_vecRoundTime[i];
        if(0 == stTime.byStartWeek)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGTimePropFromDB round = %u have not configured, aborted!"), MSG_MARK, i);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CGvGPropMgr::LoadGvGOpenDatePropDB()
{   
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgopendateprop order by SessionID asc", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGOpenDatePropDB has no data!"), MSG_MARK);
        return TRUE;
    }

    m_vecOpenDate.clear();

    while(pRes->GetRecord())
    {
        UINT16 wSessionID = SGDP::SDAtou(pRes->GetFieldValueByName("SessionID"));

        const CHAR *pszOpenDate = pRes->GetFieldValueByName("OpenDate");
        if(NULL == pszOpenDate)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGOpenDatePropDB detecting OpenDate field = NULL, session = %u, aborted!"), MSG_MARK, wSessionID);
            return FALSE;
        }

        CSDDateTime oOpenDate;

        if(!SDTimeFromString(pszOpenDate, oOpenDate))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGTimePropDB parse OpenDate %s error, session= %u, aborted!"), MSG_MARK, pszOpenDate, wSessionID);
            return FALSE;
        }

        m_vecOpenDate.resize(wSessionID + 1);
        m_vecOpenDate[wSessionID] = oOpenDate.GetTimeValue();
    }

    if(pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    for(UINT16 wSessionID = 1; wSessionID < m_vecOpenDate.size(); wSessionID++)
    {
        if(0 == m_vecOpenDate[wSessionID])
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGOpenDatePropDB detecting open date of session = %u have not configured, aborted!"), MSG_MARK, wSessionID);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CGvGPropMgr::LoadGvGUnLockPropDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgunlockprop order by ArenaID asc", &pRes, &dwErrID,  &strErr);
    if(SDDB_NO_RECORDSET == nRet)
    {
        return TRUE;
    }

    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGUnLockPropDB has no data!"), MSG_MARK);
        return TRUE;
    }

    m_mapUnLock.clear();

    while(pRes->GetRecord())
    {
        ArenaID arenaID       = SGDP::SDAtou(pRes->GetFieldValueByName("ArenaID"));
        UINT16 wUnLockSession = SGDP::SDAtou(pRes->GetFieldValueByName("UnLockSession"));

        if(m_mapArenZoneID.find(arenaID) == m_mapArenZoneID.end())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGUnLockPropDB not found the arena<%u>, aborted!"), MSG_MARK, arenaID);
            return FALSE;
        }

        m_mapUnLock[arenaID] = wUnLockSession;
    }

    if(pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

BOOL CGvGPropMgr::LoadGvGKillComboPropDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgkillcomboprop order by KillCombo asc", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGKillComboPropDB has no data!"), MSG_MARK);
        return TRUE; //
    }

    m_vecKillCombo.clear();

    while(pRes->GetRecord())
    {
        UINT16 wKillCombo         = SGDP::SDAtou(pRes->GetFieldValueByName("KillCombo"));
        m_vecKillCombo.resize(wKillCombo + 1);
        
        SGvGKillComboProp &stKillComboProp = m_vecKillCombo[wKillCombo];

        stKillComboProp.wKillCombo         = wKillCombo;
        stKillComboProp.wDoorAward         = SGDP::SDAtou(pRes->GetFieldValueByName("DoorAward"));
        stKillComboProp.wAddScore          = SGDP::SDAtou(pRes->GetFieldValueByName("AddScore"));
        stKillComboProp.byRecoverHPPercent = SGDP::SDAtou(pRes->GetFieldValueByName("RecoverHPPercent"));
        stKillComboProp.byEnegy            = SGDP::SDAtou(pRes->GetFieldValueByName("Enegy"));
        stKillComboProp.byWeakenPercent    = SGDP::SDAtou(pRes->GetFieldValueByName("WeakenPercent"));
    }

    if(pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

BOOL CGvGPropMgr::LoadGvGArenaPropDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgarenaprop where ArenaID <> 0 and ZoneID <> 0", &pRes, &dwErrID,  &strErr);
    if(SDDB_NO_RECORDSET == nRet)
    {
        return TRUE;
    }

    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGArenaPropDB has no data!"), MSG_MARK);
        return FALSE;
    }

    m_mapArenZoneID.clear();
    m_mapZoneIDToArenaID.clear();

    while(pRes->GetRecord())
    {
        ArenaID arenaID = SGDP::SDAtou(pRes->GetFieldValueByName("ArenaID"));
        UINT8 byGroupID = SGDP::SDAtou(pRes->GetFieldValueByName("GroupID"));
        UINT16 wZoneID  = SGDP::SDAtou(pRes->GetFieldValueByName("ZoneID"));
        
        if(0 == arenaID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGArenaPropDB detecting arenaID<%u> invalid, aborted!"), MSG_MARK, arenaID);
            return FALSE;
        }

        m_mapZoneIDToArenaID[wZoneID] = arenaID;

        GroupZoneIDMap &mapGroupZone = m_mapArenZoneID[arenaID];
        ZoneIDVec &vecZoneID = mapGroupZone[byGroupID];
        
        vecZoneID.push_back(wZoneID);
    }

    if(pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    for(ArenaZoneIDMap::iterator arenaItr = m_mapArenZoneID.begin(); arenaItr != m_mapArenZoneID.end(); ++arenaItr)    
    {
        GroupZoneIDMap &mapGroupZoneID = arenaItr->second;

        if(mapGroupZoneID.size() != 2)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGArenaPropDB detecting group count of arena %u is not %u, aborted!"), MSG_MARK, arenaItr->first, 2);
            return FALSE;
        }

        for(GroupZoneIDMap::iterator groupItr = mapGroupZoneID.begin(); groupItr != mapGroupZoneID.end(); ++groupItr)
        {
            GroupID groupID      = groupItr->first;
            ZoneIDVec &vecZoneID = groupItr->second;

            if (!(1 <= groupID && groupID <= 2))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGArenaPropDB detecting group %u of arena %u error, must 1 <= groupid <= 2, aborted!"), 
                    MSG_MARK, groupItr->first, arenaItr->first);
                return FALSE;
            }

            sort(vecZoneID.begin(), vecZoneID.end(), less<UINT16>());

            if(vecZoneID.size() != 2)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGArenaPropDB detecting group %u of arena %u have not enough zones, aborted!"), 
                    MSG_MARK, groupItr->first, arenaItr->first);
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL CGvGPropMgr::LoadGvGFormationPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvgformationprop order by FactionLevel", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGFormationPropFromDB has no data!"), MSG_MARK);
		return FALSE; //
	}
	UINT8 byCurPosNum = 1;
	while(pRes->GetRecord())
	{
		UINT8 byFactionLevel = SDAtou(pRes->GetFieldValueByName("FactionLevel"));
		UINT8 byUnlockPosNum = SDAtou(pRes->GetFieldValueByName("FormationPosNum"));

		m_mapFormationProp[byFactionLevel] = byUnlockPosNum;
		while ( byCurPosNum <= byUnlockPosNum)
		{
			m_mapPosNumToFactionLevel[byCurPosNum] = byFactionLevel;
			byCurPosNum++;
		}
	}

	if(pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CGvGPropMgr::LoadGvGGiftPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvggiftprop order by GiftID", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGGiftPropFromDB has no data!"), MSG_MARK);
		return FALSE; //
	}
	
	while(pRes->GetRecord())
	{
		SGvGGiftProp stProp;
		stProp.byGiftID = SDAtou(pRes->GetFieldValueByName("GiftID"));
		stProp.strGiftName = pRes->GetFieldValueByName("GiftName");

		if(!JsonParseResItem(stProp.stGiftValue, pRes->GetFieldValueByName("GiftValue")))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGGiftPropFromDB parse gift json data failed, gift = %u, giftname = %u!"), 
                MSG_MARK, stProp.byGiftID, stProp.strGiftName.c_str());
            return FALSE;
        }
		
		m_mapGvGGiftProp[stProp.byGiftID] = stProp;
	}

	if(pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CGvGPropMgr::LoadGvGGambleAwardPropFromDB()
{
    m_vecGambleAward.clear();

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from gvggambleawardprop order by PlayerLevel asc", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGPropMgr::LoadGvGGambleAwardPropFromDB has no data!"), MSG_MARK);
        return FALSE; //
    }

    SGvGGambleAward stGambleAward;

    UINT8 byCurPosNum = 1;
    while(pRes->GetRecord())
    {
        stGambleAward.wPlayerLevel    = SDAtou(pRes->GetFieldValueByName("PlayerLevel"));
        stGambleAward.dwDoubleWarCoin = SDAtou(pRes->GetFieldValueByName("DoubleWarCoin"));
        stGambleAward.dwFinalWarCoin  = SDAtou(pRes->GetFieldValueByName("FinalWarCoin"));

        m_vecGambleAward.push_back(stGambleAward);
    }

    if(pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

UINT16 CGvGPropMgr::GetGvGGameRule(TCHAR* pszRule)
{
	SDStrncpy(pszRule, m_aszGvGRule, MAX_GVG_RULE - 1);
	return ERR_OPEN_GVG_RULE::ID_SUCCESS;
}

VOID CGvGPropMgr::GetGambleReward(DT_PLAYER_RES_DATA& stWinRes, GvGRound eRound, UINT16 wPlayerLevel)
{
    stWinRes.byResKind  = ACTE_COIN;
	stWinRes.qwResValue = 0;

    if(m_vecGambleAward.empty() || 0 == wPlayerLevel)
    {
        return;
    }

    UINT16 i = 0;
    for(; i < m_vecGambleAward.size(); ++i)
    {
        SGvGGambleAward &stGambleAward = m_vecGambleAward[i];
        if(wPlayerLevel < stGambleAward.wPlayerLevel)
        {
            break;
        }
    }

    if (i >= m_vecGambleAward.size())
    {
        i = m_vecGambleAward.size() - 1;
    }

    SGvGGambleAward &stAward = m_vecGambleAward[i];
    switch(eRound)
    {
    case GVG_16_TO_8:
    case GVG_8_TO_4:
        stWinRes.qwResValue = stAward.dwDoubleWarCoin;
        break;

    case GVG_FINAL_WAR:
        stWinRes.qwResValue = stAward.dwFinalWarCoin;
        break;
    }
}

UINT8 CGvGPropMgr::GetFormationUnlockPosNum(UINT32 dwFactionLevel)
{
    C88Map::iterator itr = m_mapFormationProp.find(dwFactionLevel);
    if(itr == m_mapFormationProp.end())
    {
        return 0;
    }

    UINT8 byUnLockPosNum = itr->second;
    return byUnLockPosNum;
}

UINT8 CGvGPropMgr::GetMaxFormationPosNum()
{
	return m_mapFormationProp.rbegin()->second;
}

UINT8 CGvGPropMgr::GetNeedFactionLevelByPosNum(UINT8 byPosNum)
{
    C88Map::iterator itr = m_mapPosNumToFactionLevel.find(byPosNum);
    if(itr == m_mapPosNumToFactionLevel.end())
    {
        return 0;
    }

    UINT8 byNeeedFactionLevel = itr->second;
	return byNeeedFactionLevel;
}

 BOOL CGvGPropMgr::GetGiftProp(UINT8 byID, SGvGGiftProp& stProp)
{
	CMapGvGGiftPropItr itr = m_mapGvGGiftProp.find(byID);
	if ( itr == m_mapGvGGiftProp.end())
	{
		return FALSE;
	}
	stProp = itr->second;
	return TRUE;
}

UINT32 CGvGPropMgr::GetContinuKillDoorsTribute(UINT16 wKillCombo)
{
     if(m_vecKillCombo.empty())
     {
         return 0;
     }

     for(INT16 i = m_vecKillCombo.size() - 1; i >= 0; i--)
     {
         SGvGKillComboProp &stKillCombo = m_vecKillCombo[i];
         if(stKillCombo.wKillCombo > 0 && wKillCombo >= stKillCombo.wKillCombo && stKillCombo.wDoorAward > 0)
         {
             return stKillCombo.wDoorAward;
         }
     }

     return 0;
}

UINT16 CGvGPropMgr::CalcScoreByKillCombo(GvGRound eRound, UINT16 wKillCombo)
{
    if(m_vecKillCombo.empty())
    {
        return 0;
    }

    if(eRound != GVG_FINAL_WAR)
    {
        return 0;
    }

    for(INT16 i = m_vecKillCombo.size() - 1; i >= 0; i--)
    {
        SGvGKillComboProp &stKillCombo = m_vecKillCombo[i];
        if(stKillCombo.wKillCombo > 0 && wKillCombo >= stKillCombo.wKillCombo && stKillCombo.wAddScore > 0)
        {
            return stKillCombo.wAddScore;
        }
    }

    return 0;
    // std::lower_bound(m_vecKillCombo.begin(), m_vecKillCombo.end(), stCombo, gvgproputil::CmpKillCombo)
}

UINT16 CGvGPropMgr::CalcRecoverHPByKillCombo(UINT16 wKillCombo)
{
    if(m_vecKillCombo.empty())
    {
        return 0;
    }

    if(m_vecKillCombo.size() - 1 < wKillCombo)
    {
        return 0;
    }

    SGvGKillComboProp &stKillCombo = m_vecKillCombo[wKillCombo];
    return stKillCombo.byRecoverHPPercent;
}

UINT8 CGvGPropMgr::GetEnegyByKillCombo(UINT16 wKillCombo)
{
    if(m_vecKillCombo.empty())
    {
        return 0;
    }

    if (0 == wKillCombo)
    {
        wKillCombo = 1;
    }

    wKillCombo = MIN(m_vecKillCombo.size() - 1, wKillCombo);

    SGvGKillComboProp &stKillCombo = m_vecKillCombo[wKillCombo];
    return stKillCombo.byEnegy;
}

UINT8 CGvGPropMgr::GetWeakPercentByKillCombo(UINT16 wKillCombo)
{
    if (0 == wKillCombo)
    {
        return 100;
    }

    if(m_vecKillCombo.empty())
    {
        return 100;
    }

    wKillCombo = MIN(m_vecKillCombo.size() - 1, wKillCombo);

    SGvGKillComboProp &stKillCombo = m_vecKillCombo[wKillCombo];
    return stKillCombo.byWeakenPercent;
}

BOOL CGvGPropMgr::IsMoreWeaker(UINT16 wKillCombo)
{
    if (wKillCombo <= 1)
    {
        return FALSE;
    }

    UINT8 byPreEnegy = GetEnegyByKillCombo(wKillCombo - 1);
    UINT8 byCurEnegy = GetEnegyByKillCombo(wKillCombo);

    return byCurEnegy < byPreEnegy;
}

UINT16 CGvGPropMgr::GetHonorLevel()
{
	return m_stGvGProp.wHonorLevel;
}

UINT64 CGvGPropMgr::GetStageStartTime(UINT64 qwActivityStartTime, GVG_STAGE eStage)
{
    // 当天凌晨0点
    UINT64 qwDayZero = qwActivityStartTime - GetDayPassSecond(qwActivityStartTime);
    return qwDayZero + m_vecStageTime[eStage];
}
