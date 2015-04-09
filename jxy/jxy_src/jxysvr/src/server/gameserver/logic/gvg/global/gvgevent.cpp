///<------------------------------------------------------------------------------
//< @file:   gvgevent.cpp
//< @author: hongkunan
//< @date:   2014年1月16日 22:22:57
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "gvgevent.h"
#include "logic/gvg/global/pullgroup.h"
#include "logic/event/timereventmgr.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/gvg/global/pullplayermgr.h"
#include "logic/gvg/global/gvgmgr.h"
#include "logic/gvg/local/gvgbuild.h"
#include "common/server/utility.h"

#include <sdtime.h>

#define DAYS_OF_WEEK 7
#define SECONDS_OF_DAY 86400

UINT32 CGvGEvent::OnEvent()
{
    // 检测跨服战门派活动是否应进入下一轮次
    UINT32 dwInterval = GetInterval();
    if(dwInterval > 0)
    {
        // 检测到距离进入下一轮次还剩下很多秒，说明可能重新配置过活动时间，此时重新进行下一轮比赛的定时，本次事件作废
        CTimerEventMgr::Instance()->ResetGvGEventInterval(this, this->GetInterval());
        return 0;
    }

    GVG_STAGE eCurStage  = (GVG_STAGE)CGvGMgr::Instance()->GetCurStage();
    GVG_STAGE eNextStage = gvgutil::GetNextStage(eCurStage);

    // 检测是否已成功进入到下一轮
    BOOL bSucc = CGvGMgr::Instance()->EnterNextStage();
    if(FALSE == bSucc)
    {
        dwInterval = 3;

        // 进入下一轮失败：说明上一轮未进行完毕或者出现了异常，此时重新进入本轮，并且每隔一定时间尝试着进入下一轮次，直到可以进入下一轮
        CGvGMgr::Instance()->ReEnterCurStage();
        CTimerEventMgr::Instance()->ResetGvGEventInterval(this, dwInterval);

        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::EnterNextStage fail, session = %u, <current stage = %u>, <target stage = %u>, reenter current stage now, interval = %us"), MSG_MARK, CGvGMgr::Instance()->GetCurSession(), eCurStage, eNextStage, dwInterval);
    }
    else
    {
        dwInterval = GetInterval();

        // 成功进入下一轮：重新进行下一轮比赛的定时
        CTimerEventMgr::Instance()->ResetGvGEventInterval(this, dwInterval);
        USR_INFO(_SDT("[%s: %d]: CGvGMgr::EnterNextStage success, session = %u, <before stage = %u>, <current stage = %u>, interval to next stage = %s"), MSG_MARK, CGvGMgr::Instance()->GetCurSession(), eCurStage, eNextStage, gvgutil::TimeToDayHourMinuteSec(dwInterval).c_str());
    }

    return 0;
}

UINT32 CGvGEvent::GetInterval()
{
    if (!CGvGPropMgr::Instance()->GetGvGProp().bOpenActivity)
    {
        return 86400;
    }

    UINT64 qwNow      = SDTimeSecs();

    GVG_STAGE eCurStage  = CGvGMgr::Instance()->GetCurStage();
    GVG_STAGE eNextStage = gvgutil::GetNextStage(eCurStage);

    DT_GVG_ACTIVITY_DATA &stActData = CGvGMgr::Instance()->GetActivityData();

    UINT64 qwNextStageTime = 0;

    // 跨服战活动是否是第一次开启
    BOOL bFirstActivity = (0 == stActData.qwStartTime || 0 == stActData.wSessionID);
    if(bFirstActivity){
        stActData.wSessionID  = 0;
        stActData.qwStartTime = 0;
    }

    if(bFirstActivity || GVG_APPLY == eNextStage){
        // 下一届的报名时间
        UINT64 qwNextActivityOpenDate = CGvGPropMgr::Instance()->GetOpenDateBySession(stActData.wSessionID + 1);

        // 如果未配好下一届的开启时间，就不开启下一届了，直接一直延期下去，不跳到下一届
        if (0 == qwNextActivityOpenDate){
            qwNextStageTime = qwNow + 86400;
        }else{
            qwNextStageTime = CGvGPropMgr::Instance()->GetStageStartTime(qwNextActivityOpenDate, GVG_1_APPLY);
        }
    }
    else
    {
        qwNextStageTime = CGvGPropMgr::Instance()->GetStageStartTime(stActData.qwStartTime, eNextStage);
    }

    UINT32 dwInterval = (UINT32)(qwNow < qwNextStageTime ? qwNextStageTime - qwNow : 0);
    // DBG_INFO(_SDT("[%s: %d]: CGvGEvent::GetInterval get next interval = %u, current stage = %u"), MSG_MARK, dwInterval, eCurStage);
    return dwInterval;
}

UINT32 CGvGReloadPropEvent::OnEvent()
{
    UINT32 dwPreInterval = 0;
    CGvGEvent *poGvGEvent = CTimerEventMgr::Instance()->GetGvGEvent();
    if (poGvGEvent){
        dwPreInterval = poGvGEvent->GetInterval();
    }

    // 这个地方是阻塞式读取数据库
    CGvGPropMgr::Instance()->LoadGvGArenaPropDB();
    CGvGPropMgr::Instance()->LoadGvGPropDB();
    // CGvGPropMgr::Instance()->LoadZoneNameDB();

    if (!CGvGPropMgr::Instance()->IsFakeTime()){
        CGvGPropMgr::Instance()->LoadGvGTimePropDB();
        CGvGPropMgr::Instance()->LoadGvGOpenDatePropDB();
    }
    
    CGvGPropMgr::Instance()->LoadGvGUnLockPropDB();

    if (CGvGMgr::Instance()){
        CGvGMgr::Instance()->CheckOpenUp();
        CGvGPropMgr::Instance()->ConvertToClientDisplayTime(CGvGMgr::Instance()->GetCurSession());
    }

    if (CGvGBuild::Instance()){
        CGvGBuild::Instance()->CheckOpenUp();
        CGvGPropMgr::Instance()->ConvertToClientDisplayTime(CGvGBuild::Instance()->GetCurSession());
    }
    
    if (dwPreInterval > 0 && poGvGEvent){
        UINT32 dwNowInterval = poGvGEvent->GetInterval();
        if (dwNowInterval < dwPreInterval){
            CTimerEventMgr::Instance()->StopGvGEvent();
            CTimerEventMgr::Instance()->AddGvGEvent();
        }
    }

    return 0;
}

UINT32 CGvGReloadPropEvent::GetInterval()
{
    return 300;
}

UINT32 CGvGRePullEvent::OnEvent()
{
    CPullPlayerMgr::Instance()->Pull();
    return 0;
}

UINT32 CGvGRePullEvent::GetInterval()
{
    GVG_STAGE eCurGvGStage = CGvGMgr::Instance()->GetCurStage();

    UINT32 dwInterval = 0;

    BOOL bDone = CGvGMgr::Instance()->IsDone(eCurGvGStage);
    if(!bDone)
    {
        dwInterval = CGvGPropMgr::Instance()->GetGvGProp().wRePullInterval;
    }
    else
    {
        dwInterval = CTimerEventMgr::Instance()->GetGvGEventInterval();
    }

    if(0 == dwInterval)
    {
        dwInterval = CGvGPropMgr::Instance()->GetGvGProp().wRePullInterval;

        if(0 == dwInterval)
        {
            dwInterval = 10;
        }
    }
    
    return dwInterval;    
}
