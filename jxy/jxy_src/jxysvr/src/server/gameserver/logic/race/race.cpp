#include "race.h"
#include "racepropmgr.h"
#include "racebattlelogmgr.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <logic/vip/vippropmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/base/basedefine.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsconfig.h>
#include <logic/player/playersavemgr.h>
#include <utility.h>

using namespace SGDP;

IMPLEMENT_CLASS_POOL_MGR(CRace)

CRace::CRace()
{
    m_deqRaceBattleLog.clear();
    memset(&m_stDT_RACE_BASE_DATA, 0, sizeof(m_stDT_RACE_BASE_DATA));
    m_dwRaceNeedGold = 0;
}


CRace::~CRace()
{
    for(CRaceBattleLogDeqItr itr = m_deqRaceBattleLog.begin(); itr != m_deqRaceBattleLog.end(); itr++)
    {
        CRaceBattleLog* poBattleLog = *itr;
        if(poBattleLog)
        {
            CRaceBattleLogMgr::Instance()->RemoveBattleLog(poBattleLog);
        }
    }
    m_deqRaceBattleLog.clear();
}

BOOL CRace::Init(DT_RACE_DATA& stDT_RACE_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }

    m_poOwner = poOwner;
    memcpy(&m_stDT_RACE_BASE_DATA, &stDT_RACE_DATA.stRaceBaseData, sizeof(m_stDT_RACE_BASE_DATA));
    UINT8 bySize = stDT_RACE_DATA.stRaceBattleLog.byBattleLogNum;
    UINT32 dwPlayerID = m_poOwner->GetDT_PLAYER_BASE_DATA().dwID;
    //添加/保存已按日期从大到小排序
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        DT_RACE_BATTLELOG_DATA& stDT_RACE_BATTLELOG_DATA = stDT_RACE_DATA.stRaceBattleLog.astBattleLogInfo[byIdx];
        CRaceBattleLog* poBattleLog = CRaceBattleLogMgr::Instance()->CreateBattleLog();
        if(NULL == poBattleLog)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateBattleLog failed!"), MSG_MARK);
            return FALSE;
        }
        m_deqRaceBattleLog.push_back(poBattleLog);//旧数据放后面
        if(!poBattleLog->Init(&stDT_RACE_BATTLELOG_DATA, m_poOwner))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CRaceBattleLog init failed!"), MSG_MARK);
            return FALSE;
        }
    }
    CPlayerSaveMgr::Instance()->AddPlayerRaceSaveData(m_poOwner);
    return TRUE;
}

UINT16 CRace::CkCondition()
{
    UINT16 wErrCode = CkChallengeCD();
    if(ERR_CHALLENGE::ID_SUCCESS != wErrCode)
    {
        return wErrCode;
    }

    wErrCode = CkRaceNeedGold();

    return wErrCode;
}

UINT16 CRace::CkChallengeCD()
{
    SRaceBaseProp& stProp = CRacePropMgr::Instance()->GetRaceProp();
    UINT16 wMaxChallengeNumPerDay = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
    //判断挑战次数
    //今天
    if(IsToday(m_stDT_RACE_BASE_DATA.qwLastChallTime))
    {
        if(m_stDT_RACE_BASE_DATA.byChallTimes >= wMaxChallengeNumPerDay)
        {
            return ERR_CHALLENGE::ID_REACH_MAX_TIMES_ERR;
        }
    }
    //今天没挑战
    else
    {
        //重置挑战次数为0
        m_stDT_RACE_BASE_DATA.byChallTimes = 0;
    }

    //挑战赢不判断CD,输则连赢清0，需判断CD
    if(1 == m_stDT_RACE_BASE_DATA.byFailedFlag)
    {
        //判断CD时间
        if((SDTimeSecs() - m_stDT_RACE_BASE_DATA.qwLastChallTime) < stProp.dwChallengeFailedCD)
        {
            return ERR_CHALLENGE::ID_IN_CD_ERR;
        }
    }

    return ERR_CHALLENGE::ID_SUCCESS;
}


VOID CRace::ChallengeEncourage(BOOL bSuccess, DT_RACE_CHALLENGE_ENCOURAGE_DATA& stChallengeEncourage)
{
    if(NULL == m_poOwner)
    {
        return;
    }

    CRacePropMgr::Instance()->GetChallengeEncourage(m_poOwner->GetLevel(), bSuccess, stChallengeEncourage);

    //DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = m_poOwner->GetDT_PLAYER_BASE_DATA();
    ////奖励
    //stDT_PLAYER_BASE_DATA.qwCoin += stChallengeEncourage.dwCoin;
    //stDT_PLAYER_BASE_DATA.qwScience += stChallengeEncourage.dwScience;
    m_poOwner->AddCoin(stChallengeEncourage.dwCoin, CRecordMgr::EACT_RACECHALLENGE);
    m_poOwner->AddScience(stChallengeEncourage.dwScience, CRecordMgr::EAST_RACECHALLENGE);
    m_poOwner->AddJingJie(stChallengeEncourage.dwJingJie, CRecordMgr::EAJT_RACECHALLENGE);

    //m_poOwner->AllotExperience(stChallengeEncourage.dwExperience);
    //CPlayerBaseDataCacheMgr::Instance()->OnFameChange(stDT_PLAYER_BASE_DATA.dwID, stDT_PLAYER_BASE_DATA.dwFame);

    //SetSaveState(ESS_WANT_SAVE);
}


UINT16 CRace::RankEncourage(DT_RACE_RANK_ENCOURAGE_DATA& stDT_RACE_RANK_ENCOURAGE_DATA)
{
    if(NULL == m_poOwner)
    {
        RETURN_OTHER_ERR;
    }

    //今天已经领取过
    if(IsToday(m_stDT_RACE_BASE_DATA.qwRecvEncourageTime))
    {
        return ERR_RECV_RANK_ENCOURAGE::ID_HAVE_RECVED_ERR;
    }

    //前2天不能领取竞技场奖励
    //if(SDNow().DiffDateDay(CGSConfig::Instance()->GetServerOpentTime()) < CRacePropMgr::Instance()->GetRaceProp().wRecvRankPassDay)
    //{
    //	return ERR_RECV_RANK_ENCOURAGE::ID_TIME_ERR;
    //}

    //领取昨天的排名奖励
    CRacePropMgr::Instance()->GetRankEncourage(m_stDT_RACE_BASE_DATA.dwRecvEncourageRank, stDT_RACE_RANK_ENCOURAGE_DATA);
    //DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = m_poOwner->GetDT_PLAYER_BASE_DATA();
    //stDT_PLAYER_BASE_DATA.qwCoin += stDT_RACE_RANK_ENCOURAGE_DATA.dwCoin;
    //stDT_PLAYER_BASE_DATA.qwScience += stDT_RACE_RANK_ENCOURAGE_DATA.dwScience;
    m_poOwner->AddCoin(stDT_RACE_RANK_ENCOURAGE_DATA.dwCoin, CRecordMgr::EACT_RANK);
    m_poOwner->AddScience(stDT_RACE_RANK_ENCOURAGE_DATA.dwScience, CRecordMgr::EAST_RACERANK);
    m_poOwner->AddJingJie(stDT_RACE_RANK_ENCOURAGE_DATA.dwJingJie, CRecordMgr::EAJT_RACE_RANK);

    m_stDT_RACE_BASE_DATA.qwRecvEncourageTime = SDTimeSecs();
    //CPlayerBaseDataCacheMgr::Instance()->OnFameChange(stDT_PLAYER_BASE_DATA.dwID, stDT_PLAYER_BASE_DATA.dwFame);

    //赋值当前排名为明天的排名奖励（在排名发生变化时判断今天是否领取过排名奖励，若领取过则赋值）
    m_stDT_RACE_BASE_DATA.dwRecvEncourageRank = m_stDT_RACE_BASE_DATA.dwRank;

    CPlayerSaveMgr::Instance()->AddPlayerRaceSaveData(m_poOwner);
    //SetSaveState(ESS_WANT_SAVE);


    DBG_INFO(_SDT("[%s: %d]: PlayerID [%u] Recv Encourage!"), MSG_MARK, m_poOwner->GetDT_PLAYER_BASE_DATA().dwID);

    return ERR_RECV_RANK_ENCOURAGE::ID_SUCCESS;
}

UINT16	CRace::GetChallengeTimes(UINT32 &dwLeftTimes, UINT32 &dwTimes)
{
    SRaceBaseProp& stProp = CRacePropMgr::Instance()->GetRaceProp();
    dwTimes = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);

    //不是今天
    if(!IsToday(m_stDT_RACE_BASE_DATA.qwLastChallTime))
    {
        //重置挑战次数为0
        m_stDT_RACE_BASE_DATA.byChallTimes = 0;
    }
    if (dwTimes > m_stDT_RACE_BASE_DATA.byChallTimes)
    {
        dwLeftTimes = dwTimes - m_stDT_RACE_BASE_DATA.byChallTimes;
    }
    else
    {
        dwLeftTimes = 0;
    }
    return 0;
}

UINT16 CRace::ClrChallengeCD()
{
    SRaceBaseProp& stProp = CRacePropMgr::Instance()->GetRaceProp();
    UINT16 wMaxChallengeNumPerDay = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
    //判断挑战次数
    //今天
    if(IsToday(m_stDT_RACE_BASE_DATA.qwLastChallTime))
    {
        if(m_stDT_RACE_BASE_DATA.byChallTimes >= wMaxChallengeNumPerDay)
        {
            return ERR_CLR_CHALLENGE_CD::ID_CHALLENGE_TIMES_OVER_ERR;
        }
    }
    //不是今天，则说明新的开始
    else
    {
        return ERR_CLR_CHALLENGE_CD::ID_SUCCESS;
    }

    INT64 qnDayGap = SDTimeSecs() - m_stDT_RACE_BASE_DATA.qwLastChallTime;
    UINT32 dwCountDown = 0;
    if(qnDayGap < 0)//异常
    {
        dwCountDown = stProp.dwChallengeFailedCD;
    }
    else if(qnDayGap > stProp.dwChallengeFailedCD)
    {
        dwCountDown = 0;
    }
    else
    {
        dwCountDown = stProp.dwChallengeFailedCD - (UINT32)qnDayGap;
    }

    UINT32 dwCostGold = stProp.dwClrCDGoldPerMinute * ((dwCountDown / 60) + 1);//不足1分钟算1分钟，刚好整分钟不管

    if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }
    //扣除元宝
    //m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold -= dwCostGold;
    m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_CLRCHALLENGECD, dwCountDown);


    //设置时间
    //m_stDT_RACE_BASE_DATA.qwLastChallTime = oNow.GetTimeValue() - stProp.dwChallengeFailedCD;//会导致在晚上近0点时清除CD时导致用户在0点前可免费挑战
    m_stDT_RACE_BASE_DATA.qwLastChallTime = SDTimeSecs();
    m_stDT_RACE_BASE_DATA.byFailedFlag = 0;

    CPlayerSaveMgr::Instance()->AddPlayerRaceSaveData(m_poOwner);

    return ERR_CLR_CHALLENGE_CD::ID_SUCCESS;
}


BOOL CRace::AddBattleLog(DT_RACE_BATTLELOG_DATA* pstDT_RACE_BATTLELOG_DATA, BOOL bActiveFlag)
{
    UINT8 bySize = m_deqRaceBattleLog.size();
    CRaceBattleLog* poBattleLog = CRaceBattleLogMgr::Instance()->CreateBattleLog();
    if(NULL == poBattleLog)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateBattleLog failed!"), MSG_MARK);
        return FALSE;
    }
    if(!poBattleLog->Init(pstDT_RACE_BATTLELOG_DATA, m_poOwner))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CRaceBattleLog init failed!"), MSG_MARK);
        CRaceBattleLogMgr::Instance()->RemoveBattleLog(poBattleLog);
        return FALSE;
    }
    //限制记录个数最多为MAX_RACE_BATTLELOG_NUM
    //已经达到最大记录个数，则更新之前的记录
    if(bySize >= MAX_RACE_BATTLELOG_NUM)
    {
        CRaceBattleLog* poOldBattleLog = m_deqRaceBattleLog.back();
        m_deqRaceBattleLog.pop_back();
        CRaceBattleLogMgr::Instance()->RemoveBattleLog(poOldBattleLog);
        m_deqRaceBattleLog.push_front(poBattleLog);
    }
    //未达到记录个数上限，新增
    else
    {
        m_deqRaceBattleLog.push_front(poBattleLog);
    }
    //不是主动方才需要++
    if(!bActiveFlag)
    {
        m_stDT_RACE_BASE_DATA.byUnreadBattleLogNum++;
    }
    m_stDT_RACE_BASE_DATA.byUnreadBattleLogNum = m_stDT_RACE_BASE_DATA.byUnreadBattleLogNum > MAX_RACE_BATTLELOG_NUM ? MAX_RACE_BATTLELOG_NUM : m_stDT_RACE_BASE_DATA.byUnreadBattleLogNum;
    //SetSaveState(ESS_WANT_SAVE);
    //m_poOwner->SetSaveState(ESS_WANT_SAVE);//可能非登录用户

    PKT_CLIGS_RACE_BATTLELOG_NTF stNtf;
    stNtf.byUnreadNum = m_stDT_RACE_BASE_DATA.byUnreadBattleLogNum;
    stNtf.qwPlayerCoin = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    stNtf.qwPlayerScience = m_poOwner->GetDT_PLAYER_BASE_DATA().qwScience;
    poBattleLog->GetDT_RACE_BATTLELOG_DATA_CLI(stNtf.stBattleLog);
    //下发战报通知
    m_poOwner->SendMsg((CHAR*)(&stNtf), CLIGS_RACE_BATTLELOG_NTF);

    return TRUE;
}



VOID CRace::GetShowRaceBattleLog(UINT8& byShowNum, DT_RACE_BATTLELOG_DATA_CLI astRankBattleLogInfo[MAX_RACE_BATTLELOG_NUM])
{
    byShowNum = 0;
    UINT8 bySize = m_deqRaceBattleLog.size();
    if(0 == bySize)
    {
        return;
    }

    for(CRaceBattleLogDeqItr itr = m_deqRaceBattleLog.begin(); itr != m_deqRaceBattleLog.end(); itr++)
    {
        CRaceBattleLog* poBattleLog = *itr;
        if(poBattleLog)
        {
            poBattleLog->GetDT_RACE_BATTLELOG_DATA_CLI(astRankBattleLogInfo[byShowNum++]);
            poBattleLog->GetDT_RACE_BATTLELOG_DATA().byNewFlag = 0;//置为非新战报
        }
    }

    if(m_stDT_RACE_BASE_DATA.byUnreadBattleLogNum > 0)
    {
        m_stDT_RACE_BASE_DATA.byUnreadBattleLogNum = 0;
        CPlayerSaveMgr::Instance()->AddPlayerRaceSaveData(m_poOwner);
    }

}


//检查收费挑战元宝是否足够
UINT16 CRace::CkRaceNeedGold()
{
    m_dwRaceNeedGold = 0;

    SRaceBaseProp& stRaceBaseProp = CRacePropMgr::Instance()->GetRaceProp();
    UINT16 wMaxChallengeNumPerDay = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
    if(m_stDT_RACE_BASE_DATA.byChallTimes >= wMaxChallengeNumPerDay)
    {
        USR_INFO( _SDT("玩家[%u]已达大最大挑战次数"), m_poOwner->GetDT_PLAYER_BASE_DATA().dwID);
        return ERR_CHALLENGE::ID_REACH_MAX_TIMES_ERR;
    }

    if(m_stDT_RACE_BASE_DATA.byChallTimes < stRaceBaseProp.wFreeChallengeNumPerDay)
    {
        return ERR_CHALLENGE::ID_SUCCESS;
    }

    if(stRaceBaseProp.wFreeChallengeNumPerDay > m_stDT_RACE_BASE_DATA.byChallTimes)
    {
        m_dwRaceNeedGold = 0;
    }
    else
    {
        m_dwRaceNeedGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_RACEBUYNUM, m_stDT_RACE_BASE_DATA.byChallTimes);
    }

    if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < m_dwRaceNeedGold)
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    return ERR_CHALLENGE::ID_SUCCESS;
}


VOID CRace::OnRaceComplete(BOOL bSuccess)
{
    //扣除收费掠夺元宝
    if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold > m_dwRaceNeedGold)
    {
        //m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold -= m_dwRaceNeedGold;
        m_poOwner->DecGold(m_dwRaceNeedGold, CRecordMgr::EDGT_ONRACECOMPLETE);
    }
    else
    {
        //m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold = 0;

        m_poOwner->DecGold( (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold), CRecordMgr::EDGT_ONRACECOMPLETE );
    }
}

BOOL CRace::GetPrompt(DT_FUNC_COMMON_PROMPT& stRacePrompt)
{
    memset(&stRacePrompt, 0, sizeof(&stRacePrompt));
    stRacePrompt.byBuildKindID = EBK_PVP;
    if(!IsToday(m_stDT_RACE_BASE_DATA.qwRecvEncourageTime))
    {
        //前2天不能领取竞技场奖励
        //if((SDNow().DiffDateDay(CGSConfig::Instance()->GetServerOpentTime()) > CRacePropMgr::Instance()->GetRaceProp().wRecvRankPassDay))
        //{
        stRacePrompt.byFuncFlag = EPP_RECVENC;//可领取奖励
        return TRUE;
        //}
    }

    SRaceBaseProp& stProp = CRacePropMgr::Instance()->GetRaceProp();
    //判断挑战次数
    //今天
    if(IsToday(m_stDT_RACE_BASE_DATA.qwLastChallTime))
    {
        UINT16 wMaxChallengeNumPerDay = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
        if(m_stDT_RACE_BASE_DATA.byChallTimes < wMaxChallengeNumPerDay)
        {
            stRacePrompt.byFuncFlag = EPP_CHALLENGE;//可挑战
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        stRacePrompt.byFuncFlag = EPP_CHALLENGE;//可挑战
    }

    return TRUE;
}
