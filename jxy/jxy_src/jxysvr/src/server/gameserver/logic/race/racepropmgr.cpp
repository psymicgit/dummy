#include "racepropmgr.h"
#include "race.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sqlite3db.h>
#include <logic/base/dbconfig.h>
#include <logic/other/singleprammgr.h>
#include <logic/lua/luamgr.h>
#include <logic/vip/vippropmgr.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <framework/gsapi.h>

#include <db/dbmgr.h>

IMPLEMENT_SINGLETON_PROPMGR(CRacePropMgr)


CRacePropMgr::CRacePropMgr()
{

}

CRacePropMgr::~CRacePropMgr()
{
}

BOOL CRacePropMgr::Init()
{
    if(!LoadFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


VOID CRacePropMgr::UnInit()
{
    m_vecChallengeSec.clear();
}



BOOL CRacePropMgr::MyCompare(const SChallengeSec &t1, const SChallengeSec &t2)
{
    if(t1.dwMax <= t2.dwMin)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    return FALSE;
}

UINT32 CRacePropMgr::GetInterval(UINT32 dwRank)
{
    SChallengeSec stProp;
    stProp.dwMin = dwRank;
    CChallengeSecVecItr itr = lower_bound(m_vecChallengeSec.begin(), m_vecChallengeSec.end(), stProp, MyCompare);
    if(itr != m_vecChallengeSec.end())
    {
        return itr->wInterval;
    }

    return 1; //未配置区间，则默认为1
}

VOID CRacePropMgr::GetRankEncourage(UINT32 dwRank, DT_RACE_RANK_ENCOURAGE_DATA& stRankEncourage)
{
    stRankEncourage.dwRank = dwRank;
    stRankEncourage.dwCoin = CLuamgr::Instance()->GetRankEncourageCoin(dwRank);
    stRankEncourage.dwScience = CLuamgr::Instance()->GetRankEncourageScience(dwRank);
    stRankEncourage.dwJingJie = CLuamgr::Instance()->GetRankEncourageJingJie(dwRank);
}

VOID CRacePropMgr::GetChallengeEncourage(UINT32 dwPlayerLevel, BOOL bWinFlag, DT_RACE_CHALLENGE_ENCOURAGE_DATA& stChallEncourage)
{
    stChallEncourage.dwCoin = CLuamgr::Instance()->GetChallengeEncourageCoin(dwPlayerLevel, bWinFlag);
    stChallEncourage.dwScience = CLuamgr::Instance()->GetChallengeEncourageScience(dwPlayerLevel, bWinFlag);
    stChallEncourage.dwJingJie = CLuamgr::Instance()->GetChallengeEncourageJingJie(dwPlayerLevel, bWinFlag);
}

VOID CRacePropMgr::GetCanChallengeTimes(CPlayer* poPlayer, UINT64 qwLastChallTime, UINT8 byChallTimes, UINT16 wContiWin, UINT8 byFailedFlag, UINT16& wChallengeCountDown,
                                        UINT16& wTotalChallengeNum, UINT16& wCanChallengeNum, UINT16& wVipExtNum, UINT16& wFreeChallengeNum, UINT32& dwChallengeNeedGold)
{
    wChallengeCountDown = 0;
    dwChallengeNeedGold = 0;
    wVipExtNum = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);

    wTotalChallengeNum = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);
    //今天
    if(IsToday(qwLastChallTime))
    {
        if(byChallTimes >= wTotalChallengeNum)
        {
            wCanChallengeNum = 0;
            wFreeChallengeNum = 0;
            return ;
        }
        else
        {
            wCanChallengeNum = wTotalChallengeNum - byChallTimes;
            if(m_stRaceBaseProp.wFreeChallengeNumPerDay > byChallTimes)
            {
                wFreeChallengeNum = (UINT8)(m_stRaceBaseProp.wFreeChallengeNumPerDay - byChallTimes);
            }
            else
            {
                wFreeChallengeNum = 0;
                dwChallengeNeedGold = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_RACEBUYNUM, byChallTimes);
            }
            if(1 == byFailedFlag)
            {
                UINT64 qwPassTime = SDTimeSecs() - qwLastChallTime;
                if(qwPassTime < m_stRaceBaseProp.dwChallengeFailedCD)
                {
                    wChallengeCountDown = m_stRaceBaseProp.dwChallengeFailedCD - (UINT32)qwPassTime;
                }
                else
                {
                    wChallengeCountDown = 0;
                }
            }
        }
    }
    //今天没挑战
    else
    {
        wCanChallengeNum = wTotalChallengeNum;
        wFreeChallengeNum = (UINT8)(m_stRaceBaseProp.wFreeChallengeNumPerDay);
        return ;
    }

}


BOOL CRacePropMgr::LoadFromDB()
{

    if(!LoadRaceBaseProp())
    {
        return FALSE;
    }
    if(!LoadChallengeSec())
    {
        return FALSE;
    }

    return TRUE;
}


BOOL CRacePropMgr::LoadRaceBaseProp()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("racebase").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    BOOL bHaveData = FALSE;
    if(pRes->GetRecord())
    {
        //m_stRaceBaseProp.wMaxChallengeNumPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("MaxChallengeNumPerDay"));
        m_stRaceBaseProp.wFreeChallengeNumPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("FreeChallengeNumPerDay"));
        m_stRaceBaseProp.dwChallengeFailedCD = SGDP::SDAtou(pRes->GetFieldValueByName("ChallengeFailedCD"));
        m_stRaceBaseProp.dwClrCDGoldPerMinute = SGDP::SDAtou(pRes->GetFieldValueByName("ClrCDGoldPerMinute"));
        m_stRaceBaseProp.byChallengeNum = SGDP::SDAtou(pRes->GetFieldValueByName("ChallengeNum"));
        m_stRaceBaseProp.wRankShowNum = SGDP::SDAtou(pRes->GetFieldValueByName("RankShowNum"));
        m_stRaceBaseProp.dwRobotID = SGDP::SDAtou(pRes->GetFieldValueByName("RobotID"));
		m_stRaceBaseProp.wRecvRankPassDay = SGDP::SDAtou(pRes->GetFieldValueByName("RecvRankPassDay"));

        bHaveData = TRUE;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    if(!bHaveData)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: racebase have no data!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

BOOL CRacePropMgr::LoadChallengeSec()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    BOOL bHaveData = FALSE;
    SChallengeSec stLastChallengeSec;
    stLastChallengeSec.dwMin = 0;
    stLastChallengeSec.dwMax = 0;
    stLastChallengeSec.wInterval = 0;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("racechallengesec", " order by Min asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while((pRes->GetRecord()))// 当有行结果时
    {
        SChallengeSec stChallengeSec;
        stChallengeSec.dwMin = SGDP::SDAtou(pRes->GetFieldValueByName("Min"));
        stChallengeSec.dwMax = SGDP::SDAtou(pRes->GetFieldValueByName("Max"));
        stChallengeSec.wInterval = SGDP::SDAtou(pRes->GetFieldValueByName("Interval"));
        if(0 == stChallengeSec.dwMax)//0为最大
        {
            stChallengeSec.dwMax = 0xFFFFFFFF;
        }
        if(stChallengeSec.dwMin <= stLastChallengeSec.dwMax)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: [%d]<=[%d]!"), MSG_MARK, stChallengeSec.dwMin, stLastChallengeSec.dwMax);
            return FALSE;
        }
        if(stChallengeSec.dwMin > stChallengeSec.dwMax)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: [%d]<[%d]!"), MSG_MARK, stChallengeSec.dwMin, stLastChallengeSec.dwMax);
            return FALSE;
        }

        m_vecChallengeSec.push_back(stChallengeSec);
        stLastChallengeSec = stChallengeSec;

        bHaveData = TRUE;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    if(!bHaveData)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: racechallengesec have no data!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

