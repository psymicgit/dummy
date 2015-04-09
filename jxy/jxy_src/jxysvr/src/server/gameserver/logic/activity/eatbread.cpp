#include "eatbread.h"
#include "eatbreadpropmgr.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/record/recordmgr.h>
#include <logic/base/basedefine.h>
#include <logic/other/locale.h>
#include <dll/sdframework/sdloggerimpl.h>
#include <utility.h>

IMPLEMENT_CLASS_POOL_MGR(CEatBread)
CEatBread::CEatBread()
{

}

CEatBread::~CEatBread()
{

}

BOOL CEatBread::Init(const DT_EAT_BREAD_DATA& stDT_EAT_BREAD_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    memcpy(&m_stDT_EAT_BREAD_DATA, &stDT_EAT_BREAD_DATA, sizeof(DT_EAT_BREAD_DATA));

    m_poOwner = poOwner;

    return TRUE;
}


VOID CEatBread::GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA)
{
    memset(&stDT_ACTIVITY_BASE_DATA, 0, sizeof(stDT_ACTIVITY_BASE_DATA));
    stDT_ACTIVITY_BASE_DATA.byKindID = EAK_EATBREAD;
    const CEatBreakActivityTimePropMap& mapActivityEndTimeProp = CEatBreadPropMgr::Instance()->GetEatBreadEndTimeProp();

    UINT32 dwCurSecond = GetDayPassSecond(m_stDT_EAT_BREAD_DATA.qwEnterTime);
    BOOL bDay = IsToday(m_stDT_EAT_BREAD_DATA.qwEnterTime);
    for(CEatBreakActivityTimePropMapCItr itr = mapActivityEndTimeProp.begin(); ((itr != mapActivityEndTimeProp.end()) && (stDT_ACTIVITY_BASE_DATA.byActivityTimeNum < MAX_ACTIVITY_TIME_NUM)); itr++)
    {
        const SEatBreadActivityTimeProp& stProp = itr->second;
        DT_ACTIVITY_TIME_DATA& stDT_ACTIVITY_TIME_DATA = stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[stDT_ACTIVITY_BASE_DATA.byActivityTimeNum++];
        memset(&stDT_ACTIVITY_TIME_DATA, 0, sizeof(stDT_ACTIVITY_TIME_DATA));
        stDT_ACTIVITY_TIME_DATA.byStartHour = stProp.byStartHour;
        stDT_ACTIVITY_TIME_DATA.byStartMin = stProp.byStartMin;
        stDT_ACTIVITY_TIME_DATA.wKeepTime = stProp.wKeepTime;
        if((dwCurSecond >= stProp.dwStartTime) && (dwCurSecond < stProp.dwEndTime) && (bDay))
        {
            stDT_ACTIVITY_TIME_DATA.wEndCountDown = (UINT16)(stProp.dwEndTime - dwCurSecond);
            if(m_stDT_EAT_BREAD_DATA.byHaveGivePhyStrengthen >= CEatBreadPropMgr::Instance()->GetMaxObtainPhyStrengthen())
            {
                stDT_ACTIVITY_TIME_DATA.byHaveFinishFlag = 1;
            }
        }
    }
    stDT_ACTIVITY_BASE_DATA.byNotity = m_stDT_EAT_BREAD_DATA.byNotity;
}


UINT16 CEatBread::OpenEatBread(PKT_CLIGS_OPNE_EAT_BREAD_ACK& pstAck)
{
    UINT16 wTimePerPhyStrengthen = CEatBreadPropMgr::Instance()->GetTimePerPhyStrengthen();
    UINT8 byMaxObtainPhyStrengthen = CEatBreadPropMgr::Instance()->GetMaxObtainPhyStrengthen();

    pstAck.wEndCountDown = 0;
    pstAck.byMaxObtainPhyStrengthen = byMaxObtainPhyStrengthen;
    pstAck.wTimePerPhyStrengthen = wTimePerPhyStrengthen ;
    pstAck.wOarNumPerPhyStrengthen = CEatBreadPropMgr::Instance()->GetOarNumPerPhyStrengthen();
    pstAck.dwOnceSpeedCoin = CEatBreadPropMgr::Instance()->GetOnceSpeedCoin();
    if(0 != m_stDT_EAT_BREAD_DATA.qwEnterTime) //上次进入
    {
        const SEatBreadActivityTimeProp* pstLastActivityTimeProp = CEatBreadPropMgr::Instance()->GetActivityTimeProp(m_stDT_EAT_BREAD_DATA.qwEnterTime);
        if(NULL == pstLastActivityTimeProp) //异常,重新进入当前活动
        {
            //进入活动
            Enter(pstAck);
            return ERR_OPNE_EAT_BREAD::ID_SUCCESS;
        }
        const SEatBreadActivityTimeProp* pstCurActivityTimeProp = CEatBreadPropMgr::Instance()->GetActivityTimeProp();

        pstAck.byHaveObtainPhyStrengthen = m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen;
        pstAck.dwHaveObtainCoin = m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin;

        //上次进入的是今天的当前活动（当前活动没退出），返回已获得的信息
        if((pstLastActivityTimeProp == pstCurActivityTimeProp) && (IsToday(m_stDT_EAT_BREAD_DATA.qwEnterTime)))
        {
            //上次没主动离开，则时间+上已过时间
            if(0 == m_stDT_EAT_BREAD_DATA.byHaveLeaveFlag)
            {
                m_stDT_EAT_BREAD_DATA.wHaveUseTime = m_stDT_EAT_BREAD_DATA.wHaveUseTime + (UINT16)(GetDayPassSecond() - GetDayPassSecond(m_stDT_EAT_BREAD_DATA.qwEnterTime));
            }
            else
            {
                if(m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen <= byMaxObtainPhyStrengthen) //未达成目标，重新进入
                {
                    m_stDT_EAT_BREAD_DATA.byHaveLeaveFlag = 0;//重新进入，离开标志为0
                }
            }
            pstAck.wHaveUseTime = m_stDT_EAT_BREAD_DATA.wHaveUseTime;
            pstAck.wEndCountDown =  (UINT16)(pstLastActivityTimeProp->dwEndTime - GetDayPassSecond());
            m_poOwner->GetLocale().OnEatBread();
        }
        //上次进入的不是当前活动,给之前所得，重新进入当前活动
        else if(pstLastActivityTimeProp != pstCurActivityTimeProp)
        {
            pstAck.wHaveUseTime = m_stDT_EAT_BREAD_DATA.wHaveUseTime + (UINT16)(pstLastActivityTimeProp->dwEndTime - GetDayPassSecond(m_stDT_EAT_BREAD_DATA.qwEnterTime));

            //给之前收益
            m_poOwner->AddPhyStrength(m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen - m_stDT_EAT_BREAD_DATA.byHaveGivePhyStrengthen, CRecordMgr::EAPST_EATBREAD);
            m_poOwner->AddCoin(m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin - m_stDT_EAT_BREAD_DATA.dwHaveGiveCoin, CRecordMgr::EACT_EATBREAD);

            //进入活动
            Enter(pstAck);
        }
        //其他，重新进入活动
        else
        {
            //进入活动
            Enter(pstAck);
        }
    }
    else //没在活动内
    {
        //进入活动
        Enter(pstAck);
    }


    return ERR_OPNE_EAT_BREAD::ID_SUCCESS;
}

VOID CEatBread::Enter(PKT_CLIGS_OPNE_EAT_BREAD_ACK& pstAck)
{
    memset(&m_stDT_EAT_BREAD_DATA, 0, sizeof(m_stDT_EAT_BREAD_DATA));
    m_stDT_EAT_BREAD_DATA.byHaveDataFlag = 1;

    USR_INFO(_SDT("[%s: %d]: Player[%u] Enter EatBread!"), MSG_MARK, m_poOwner->GetID());

    const SEatBreadActivityTimeProp* pstActivityTimeProp = CEatBreadPropMgr::Instance()->GetActivityTimeProp();
    if(pstActivityTimeProp) //当前时间在活动内,进入活动
    {
        pstAck.byHaveObtainPhyStrengthen = 0;
        pstAck.dwHaveObtainCoin = 0;
        pstAck.wHaveUseTime = 0;

        //进入活动
        memset(&m_stDT_EAT_BREAD_DATA, 0, sizeof(m_stDT_EAT_BREAD_DATA));
        m_stDT_EAT_BREAD_DATA.byHaveDataFlag = 1;
        m_stDT_EAT_BREAD_DATA.qwEnterTime = SDTimeSecs();
        m_poOwner->GetLocale().OnEatBread();

        UINT32 dwCurSecond = GetDayPassSecond();
        if((dwCurSecond >= pstActivityTimeProp->dwStartTime) && (dwCurSecond < pstActivityTimeProp->dwEndTime))
        {
            pstAck.wEndCountDown = (UINT16)(pstActivityTimeProp->dwEndTime - dwCurSecond);
        }
    }
    else //不在活动时间段
    {
        pstAck.byHaveObtainPhyStrengthen = 0;
        pstAck.dwHaveObtainCoin = 0;
        pstAck.wHaveUseTime = 0;
        m_poOwner->GetLocale().OnEatBreadEnd();
    }
}


UINT16 CEatBread::ObtainPhystrengthen(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin, UINT16& wHaveUseTime)
{
    UINT16 wTimePerPhyStrengthen = CEatBreadPropMgr::Instance()->GetTimePerPhyStrengthen();
    UINT8 byMaxObtainPhyStrengthen = CEatBreadPropMgr::Instance()->GetMaxObtainPhyStrengthen();
    UINT32 dwMaxObtainCoin = CEatBreadPropMgr::Instance()->GetMaxObtainCoin();


    if(0 != m_stDT_EAT_BREAD_DATA.qwEnterTime)
    {
        const SEatBreadActivityTimeProp* pstLastActivityTimeProp = CEatBreadPropMgr::Instance()->GetActivityTimeProp(m_stDT_EAT_BREAD_DATA.qwEnterTime);
        if(NULL == pstLastActivityTimeProp) //异常
        {
            //退出活动
            m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin = 0;
            m_stDT_EAT_BREAD_DATA.qwEnterTime = 0;
            RETURN_OTHER_ERR;
        }
        //已过活动时间,给之前收益，退出活动
        if(GetDayPassSecond() > pstLastActivityTimeProp->dwEndTime)
        {
            //给之前收益
            m_poOwner->AddPhyStrength(m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen - m_stDT_EAT_BREAD_DATA.byHaveGivePhyStrengthen, CRecordMgr::EAPST_EATBREAD);
            m_poOwner->AddCoin(m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin - m_stDT_EAT_BREAD_DATA.dwHaveGiveCoin, CRecordMgr::EACT_EATBREAD);
            //退出活动
            memset(&m_stDT_EAT_BREAD_DATA, 0, sizeof(m_stDT_EAT_BREAD_DATA));
            m_stDT_EAT_BREAD_DATA.byHaveDataFlag = 1;

            m_poOwner->GetLocale().OnEatBreadEnd();
            return ERR_OBTAIN_PHYSTRENGTHEN::ID_ACTIVITY_OVER;
        }
        //在活动期间
        else
        {
            byHaveObtainPhyStrengthen = byHaveObtainPhyStrengthenCli > byMaxObtainPhyStrengthen ? byMaxObtainPhyStrengthen : byHaveObtainPhyStrengthenCli;
            dwHaveObtainCoin = dwHaveObtainCoinCli > dwMaxObtainCoin ? dwMaxObtainCoin : dwHaveObtainCoinCli;
            wHaveUseTime = m_stDT_EAT_BREAD_DATA.wHaveUseTime + (UINT16)(GetDayPassSecond() - GetDayPassSecond(m_stDT_EAT_BREAD_DATA.qwEnterTime));
            m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen = byHaveObtainPhyStrengthen;
            m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin = dwHaveObtainCoin;
        }
    }
    else
    {
        return ERR_OBTAIN_PHYSTRENGTHEN::ID_NOT_IN_ACTIVITY;
    }

    return ERR_OBTAIN_PHYSTRENGTHEN::ID_SUCCESS;
}


UINT16 CEatBread::LeaveEatBread(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin)
{
    UINT16 wTimePerPhyStrengthen = CEatBreadPropMgr::Instance()->GetTimePerPhyStrengthen();
    UINT8 byMaxObtainPhyStrengthen = CEatBreadPropMgr::Instance()->GetMaxObtainPhyStrengthen();
    UINT32 dwMaxObtainCoin = CEatBreadPropMgr::Instance()->GetMaxObtainCoin();

    USR_INFO(_SDT("[%s: %d]: Player[%u] Leave EatBread!"), MSG_MARK, m_poOwner->GetID());

    if(0 != m_stDT_EAT_BREAD_DATA.qwEnterTime)
    {
        const SEatBreadActivityTimeProp* pstLastActivityTimeProp = CEatBreadPropMgr::Instance()->GetActivityTimeProp(m_stDT_EAT_BREAD_DATA.qwEnterTime);
        if(NULL == pstLastActivityTimeProp) //异常
        {
            //退出活动
            memset(&m_stDT_EAT_BREAD_DATA, 0, sizeof(m_stDT_EAT_BREAD_DATA));
            m_stDT_EAT_BREAD_DATA.byHaveDataFlag = 1;

            m_poOwner->GetLocale().OnEatBreadEnd();
            return ERR_LEAVE_EAT_BREAD::ID_SUCCESS;
        }
        if(0 == m_stDT_EAT_BREAD_DATA.byHaveLeaveFlag)
        {
            UINT16 wPassTime = 0;
            //已过活动时间,给之前收益，退出活动
            if(GetDayPassSecond() > pstLastActivityTimeProp->dwEndTime)
            {
                wPassTime = (UINT16)(pstLastActivityTimeProp->dwEndTime - GetDayPassSecond(m_stDT_EAT_BREAD_DATA.qwEnterTime));
            }
            //在活动期间
            else
            {
                wPassTime = (UINT16)(GetDayPassSecond() - GetDayPassSecond(m_stDT_EAT_BREAD_DATA.qwEnterTime));
            }

            byHaveObtainPhyStrengthen = byHaveObtainPhyStrengthenCli > byMaxObtainPhyStrengthen ? byMaxObtainPhyStrengthen : byHaveObtainPhyStrengthenCli;
            dwHaveObtainCoin = dwHaveObtainCoinCli > dwMaxObtainCoin ? dwMaxObtainCoin : dwHaveObtainCoinCli;
            m_stDT_EAT_BREAD_DATA.wHaveUseTime += wPassTime;
            m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen = byHaveObtainPhyStrengthen;
            m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin = dwHaveObtainCoin;

            m_stDT_EAT_BREAD_DATA.byHaveLeaveFlag = 1;

            //给之前收益
            m_poOwner->AddPhyStrength(m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen - m_stDT_EAT_BREAD_DATA.byHaveGivePhyStrengthen, CRecordMgr::EAPST_EATBREAD);
            m_poOwner->AddCoin(m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin - m_stDT_EAT_BREAD_DATA.dwHaveGiveCoin, CRecordMgr::EACT_EATBREAD);
            m_stDT_EAT_BREAD_DATA.byHaveGivePhyStrengthen = m_stDT_EAT_BREAD_DATA.byHaveObtainPhyStrengthen;
            m_stDT_EAT_BREAD_DATA.dwHaveGiveCoin = m_stDT_EAT_BREAD_DATA.dwHaveObtainCoin;

            m_poOwner->GetLocale().OnEatBreadEnd();
        }
    }
    else
    {
        //退出活动
        memset(&m_stDT_EAT_BREAD_DATA, 0, sizeof(m_stDT_EAT_BREAD_DATA));
        m_stDT_EAT_BREAD_DATA.byHaveDataFlag = 1;

        m_poOwner->GetLocale().OnEatBreadEnd();
        return ERR_LEAVE_EAT_BREAD::ID_SUCCESS;
    }

    m_poOwner->GetLocale().OnEatBreadEnd();

    return ERR_LEAVE_EAT_BREAD::ID_SUCCESS;
}

//有且未完成活动则返回TRUE
BOOL CEatBread::GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
    memset(&stPrompt, 0, sizeof(stPrompt));
    stPrompt.byBuildKindID = EBK_ACTIVITY;
    stPrompt.byExt = EAK_EATBREAD;
    const SEatBreadActivityTimeProp* pstProp = CEatBreadPropMgr::Instance()->GetActivityTimeProp();
    const SEatBreadActivityTimeProp* pstNextProp = NULL;
    UINT32 dwPassSes = GetDayPassSecond();
    if(pstProp)
    {
        stPrompt.byFuncFlag = EAK_EATBREAD;
        UINT32 dwCurSecond = GetDayPassSecond(m_stDT_EAT_BREAD_DATA.qwEnterTime);
        BOOL bDay = IsToday(m_stDT_EAT_BREAD_DATA.qwEnterTime);
        //已获得最大体力，不可再进行活动，获取下一次活动倒计时
        if((m_stDT_EAT_BREAD_DATA.byHaveGivePhyStrengthen >= CEatBreadPropMgr::Instance()->GetMaxObtainPhyStrengthen()) && (bDay) && (dwCurSecond >= pstProp->dwStartTime) && (dwCurSecond < pstProp->dwEndTime))
        {
            pstNextProp = CEatBreadPropMgr::Instance()->GetNextActivityTimeProp(pstProp->dwEndTime);
        }
        else
        {
            stPrompt.dwFuncCountDown = (pstProp->dwEndTime > dwPassSes) ? pstProp->dwEndTime - dwPassSes : 0;
            return TRUE;
        }
    }
    else
    {
        pstNextProp = CEatBreadPropMgr::Instance()->GetNextActivityTimeProp();
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

        return FALSE;
    }

    return FALSE;
}



UINT16 CEatBread::GetPhystrengthActivityData(UINT8& byActivityNum, DT_ACTIVITY_BASE_DATA astActivityInfo[MAX_ACTIVITY_NUM])
{
	byActivityNum = 0;
	GetDT_ACTIVITY_BASE_DATA(astActivityInfo[byActivityNum++]);
	//其他活动
	return ERR_OPNE_ACTIVITY_CENTER::ID_SUCCESS;
}

//UINT16 CPlayer::OpenEatBread(PKT_CLIGS_OPNE_EAT_BREAD_ACK& pstAck)
//{
//	if(NULL == m_poEatBread)
//	{
//		return ERR_OPNE_EAT_BREAD::ID_UNLOCK;
//	}
//	return m_poEatBread->OpenEatBread(pstAck);
//}
//
//
//UINT16 CEatBread::ObtainPhystrengthen(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin, UINT16& wHaveUseTime)
//{
//	if(NULL == m_poEatBread)
//	{
//		return ERR_OBTAIN_PHYSTRENGTHEN::ID_UNLOCK;
//	}
//
//	return m_poEatBread->ObtainPhystrengthen(byHaveObtainPhyStrengthenCli, dwHaveObtainCoinCli, byHaveObtainPhyStrengthen, dwHaveObtainCoin, wHaveUseTime);
//}
//
//UINT16 CEatBread::LeaveEatBread(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin)
//{
//	if(NULL == m_poEatBread)
//	{
//		return ERR_LEAVE_EAT_BREAD::ID_UNLOCK;
//	}
//
//	return m_poEatBread->LeaveEatBread(byHaveObtainPhyStrengthenCli, dwHaveObtainCoinCli, byHaveObtainPhyStrengthen, dwHaveObtainCoin);
//}

