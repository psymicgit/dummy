#include "bossbattle.h"
#include "bossbbattlelog.h"
#include "bossbattlemgr.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <common/client/errdef.h>
#include <logic/player/player.h>
#include <framework/gsconfig.h>

IMPLEMENT_CLASS_POOL_MGR(CBossBattle);


CBossBattle::CBossBattle()
{
    m_qwLastMoveTime = 0;
    memset(&m_stDT_BOSS_BATTLE_PLAYER_DATA, 0, sizeof(DT_BOSS_BATTLE_PLAYER_DATA));

    m_mapBeattentionID.clear();
    m_mapEnterID.clear();
    m_mapLeaveID.clear();
    m_mapMoveID.clear();
    m_mapHurtID.clear();
    m_qwLastDealTime = 0;
}

CBossBattle::~CBossBattle()
{

}


BOOL CBossBattle::Init(DT_BOSS_BATTLE_PLAYER_DATA_INFO& stDT_BOSS_BATTLE_PLAYER_DATA_INFO, CPlayer* poOwner)
{
    if((NULL == poOwner))
    {
        return FALSE;
    }

    memcpy(&m_stDT_BOSS_BATTLE_PLAYER_DATA, &stDT_BOSS_BATTLE_PLAYER_DATA_INFO.stBossBattleData, sizeof(DT_BOSS_BATTLE_PLAYER_DATA));

    m_poOwner = poOwner;
    if(!InitBattleLog(stDT_BOSS_BATTLE_PLAYER_DATA_INFO.stBossBBLData))
    {
        return FALSE;
    }

    return TRUE;
}

VOID CBossBattle::OnAddBeattaction(UINT32 dwPlayerID, BOOL bFirst)
{
    if(dwPlayerID == m_poOwner->GetID())
    {
        return;
    }
    m_mapBeattentionID[dwPlayerID] = dwPlayerID;

    if(!bFirst)
    {
        m_mapEnterID[dwPlayerID] = dwPlayerID;
        m_mapLeaveID.erase(dwPlayerID);
    }
}

VOID CBossBattle::OnDelBeattaction(UINT32 dwPlayerID)
{
    m_mapBeattentionID.erase(dwPlayerID);

    LeaveID(dwPlayerID);
}

VOID CBossBattle::GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA)
{
    memset(&stDT_ACTIVITY_BASE_DATA, 0, sizeof(stDT_ACTIVITY_BASE_DATA));
    stDT_ACTIVITY_BASE_DATA.byKindID = EAK_BOSSBATTLE;

    UINT32 dwCurSecond = GetDayPassSecond();
    const SBossBProp* pstProp = CBossBattlePropMgr::Instance()->GetActivityTimeProp();
    const CBossBActivityTimePropMap& mapActivityEndTimeProp = CBossBattlePropMgr::Instance()->GetBossBEndTimeProp();
    for(CBossBActivityTimePropMapCItr itr = mapActivityEndTimeProp.begin(); ((itr != mapActivityEndTimeProp.end()) && (stDT_ACTIVITY_BASE_DATA.byActivityTimeNum < MAX_ACTIVITY_TIME_NUM)); itr++)
    {
        const SBossBProp& stProp = itr->second;
        DT_ACTIVITY_TIME_DATA& stDT_ACTIVITY_TIME_DATA = stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[stDT_ACTIVITY_BASE_DATA.byActivityTimeNum++];
        memset(&stDT_ACTIVITY_TIME_DATA, 0, sizeof(stDT_ACTIVITY_TIME_DATA));
        stDT_ACTIVITY_TIME_DATA.byStartHour = stProp.byStartHour;
        stDT_ACTIVITY_TIME_DATA.byStartMin = stProp.byStartMin;
        stDT_ACTIVITY_TIME_DATA.wKeepTime = stProp.wKeepTime;
        stDT_ACTIVITY_TIME_DATA.wAheadTime = stProp.wReadyCD;
        if((pstProp) && (pstProp->dwStartTime == stProp.dwStartTime) && (pstProp->dwStartTime == stProp.dwStartTime))
        {
            stDT_ACTIVITY_TIME_DATA.wEndCountDown = (UINT16)(stProp.dwEndTime - dwCurSecond);
            if(CBossBattleMgr::Instance()->IsOver())
            {
                stDT_ACTIVITY_TIME_DATA.byHaveFinishFlag = 1;
            }
        }
    }
    stDT_ACTIVITY_BASE_DATA.byNotity = m_stDT_BOSS_BATTLE_PLAYER_DATA.byNotity;
}

BOOL CBossBattle::GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
    memset(&stPrompt, 0, sizeof(stPrompt));
    stPrompt.byBuildKindID = EBK_ACTIVITY;
    stPrompt.byExt = EAK_BOSSBATTLE;

    const SBossBProp* pstProp = CBossBattlePropMgr::Instance()->GetActivityTimeProp();
    const SBossBProp* pstNextProp = NULL;
    UINT32 dwPassSes = GetDayPassSecond();
    if(pstProp)
    {
        //前2天不能领取竞技场奖励
        if((SDNow().DiffDateDay(CGSConfig::Instance()->GetServerOpentTime()) < pstProp->wOpenActivityPassDay))
        {
            return FALSE;
        }

        stPrompt.byFuncFlag = EAK_BOSSBATTLE;
        //活动已经结束，不可再进行活动，获取下一次活动倒计时
        if((CBossBattleMgr::Instance()->IsOver()) && (CBossBattleMgr::Instance()->GetLastActivityID() == pstProp->byActivityId))
        {
            pstNextProp = CBossBattlePropMgr::Instance()->GetNextActivityTimeProp(pstProp->dwEndTime);
        }
        else
        {
            //能进入，但未开始，则返回真正开始倒计时
            if(pstProp->dwStartTime > dwPassSes)
            {
                stPrompt.dwFuncCountDown = pstProp->dwStartTime - dwPassSes;
                stPrompt.wAheadTime = pstProp->wReadyCD;
            }
            //已经开始，则返回活动结束倒计时
            else
            {
                stPrompt.dwFuncCountDown = (pstProp->dwEndTime > dwPassSes) ? pstProp->dwEndTime - dwPassSes : 0;
            }
            return TRUE;
        }
    }
    else
    {
        pstNextProp = CBossBattlePropMgr::Instance()->GetNextActivityTimeProp();
    }

    stPrompt.byFuncFlag = 0;
    if(pstNextProp)
    {
        //今天内的活动
        if(pstNextProp->dwStartTime > dwPassSes)
        {
            stPrompt.dwFuncCountDown = pstNextProp->dwStartTime - dwPassSes;
        }
        //今天已没有活动，下一个活动为明天的第一个活动
        else
        {
            stPrompt.dwFuncCountDown = SECONDES_PER_DAY - dwPassSes + pstNextProp->dwStartTime;
        }

        stPrompt.wAheadTime = pstNextProp->wReadyCD;

        return FALSE;
    }

    return FALSE;
}

BOOL CBossBattle::AddBattleLog(DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA* pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA)
{
    UINT8 bySize = m_mapBossBBattleLog.size();
    CBossBBattleLog* poBattleLog = CBossBattleMgr::Instance()->CreateBattleLog();
    if(NULL == poBattleLog)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateBattleLog failed!"), MSG_MARK);
        return FALSE;
    }
    if(!poBattleLog->Init(pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA, m_poOwner, this))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CBossBBattleLog init failed!"), MSG_MARK);
        CBossBattleMgr::Instance()->RemoveBattleLog(poBattleLog);
        return FALSE;
    }
    //限制记录个数最多为MAX_BOSSB_BATTLE_LOG_NUM
    //已经达到最大记录个数，则更新之前的记录
    if(bySize >= MAX_BOSSB_BATTLE_LOG_NUM)
    {
        CBossBBattleLog* poOldBattleLog = m_mapBossBBattleLog.begin()->second;
        CBossBattleMgr::Instance()->RemoveBattleLog(poOldBattleLog);
        m_mapBossBBattleLog.erase(m_mapBossBBattleLog.begin());
    }
    //未达到记录个数上限，新增
    else
    {
        m_mapBossBBattleLog[pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA->qwOverTime] = poBattleLog;
    }
    m_stDT_BOSS_BATTLE_PLAYER_DATA.byUnreadBattleLogNum++;
    m_stDT_BOSS_BATTLE_PLAYER_DATA.byUnreadBattleLogNum = m_stDT_BOSS_BATTLE_PLAYER_DATA.byUnreadBattleLogNum > MAX_BOSSB_BATTLE_LOG_NUM ? MAX_BOSSB_BATTLE_LOG_NUM : m_stDT_BOSS_BATTLE_PLAYER_DATA.byUnreadBattleLogNum;


    return TRUE;
}

UINT16 CBossBattle::GetShowBossBBattleLog(UINT8& byNum, DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI astBossBattleBL[MAX_BOSSB_BATTLE_LOG_NUM])
{
    byNum = 0;
    UINT8 bySize = m_mapBossBBattleLog.size();
    if(0 == bySize)
    {
        return ERR_OPEN_BOSSB_BATTLE_LOG::ID_SUCCESS;
    }
    for(CBossBBattleLogMapRitr itr = m_mapBossBBattleLog.rbegin(); ((itr != m_mapBossBBattleLog.rend()) && (byNum < MAX_BOSSB_BATTLE_LOG_NUM)) ; itr++)
    {
        CBossBBattleLog* poBattleLog = itr->second;
        if(poBattleLog)
        {
            poBattleLog->GetDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI(astBossBattleBL[byNum++]);
        }

    }

    m_stDT_BOSS_BATTLE_PLAYER_DATA.byUnreadBattleLogNum = 0;

    return ERR_OPEN_BOSSB_BATTLE_LOG::ID_SUCCESS;
}

BOOL CBossBattle::InitBattleLog(const DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST& stBossBBattleLog)
{
    UINT8 bySize = stBossBBattleLog.byNum;
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        const DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA& stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA = stBossBBattleLog.astBossBattleBL[byIdx];
        CBossBBattleLog* poBattleLog = CBossBattleMgr::Instance()->CreateBattleLog();
        if(NULL == poBattleLog)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateBattleLog failed!"), MSG_MARK);
            return FALSE;
        }
        if(!poBattleLog->Init(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA, m_poOwner, this))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CBossBBattleLog init failed!"), MSG_MARK);
            CBossBattleMgr::Instance()->RemoveBattleLog(poBattleLog);
            return FALSE;
        }
        m_mapBossBBattleLog[stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.qwOverTime] = poBattleLog;
    }

    return TRUE;
}

VOID CBossBattle::EndSync()
{
    m_mapEnterID.clear();
    m_mapLeaveID.clear();
    m_mapMoveID.clear();
    m_mapHurtID.clear();
}



VOID CBossBattle::MoveID(UINT32 dwPlayerID)
{
    m_mapMoveID[dwPlayerID] = dwPlayerID;
}

VOID CBossBattle::HurtID(UINT32 dwPlayerID, UINT32 dwHurtValue)
{
    //多少伤害只同步一个即可
    m_mapHurtID[dwPlayerID] = dwHurtValue;
    m_mapMoveID.erase(dwPlayerID);//同时下发伤害和移动都是先伤害后移动，先移动后伤害的不处理
}

VOID CBossBattle::LeaveID(UINT32 dwPlayerID)
{
    m_mapLeaveID[dwPlayerID] = dwPlayerID;
    m_mapEnterID.erase(dwPlayerID);
    m_mapMoveID.erase(dwPlayerID);
}

VOID CBossBattle::InitEnterData()
{
    m_mapBeattentionID.clear();
    EndSync();
}
