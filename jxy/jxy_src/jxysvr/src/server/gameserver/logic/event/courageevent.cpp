#include "courageevent.h"
#include "timereventmgr.h"
#include <logic/activity/couragemgr.h>
#include <sdutil.h>
#include "logic/activity/couragepropmgr.h"


CCourageEvent::CCourageEvent()
{
    m_bStartFlag = TRUE;
    m_bFirstFlag = TRUE;
    m_byActivityID = 0;
}

CCourageEvent::~CCourageEvent()
{

}

UINT32 CCourageEvent::OnEvent()
{
    if(m_bStartFlag)
    {
        CCourageMgr::Instance()->OnStartEvent(m_byActivityID);
    }
    else
    {
        CCourageMgr::Instance()->OnEndEvent(m_byActivityID);
    }

    CTimerEventMgr::Instance()->ResetCourageEventInterval(this);

    return 0;
}

BOOL CCourageEvent::OnDestroy()
{
    return TRUE;
}

UINT32 CCourageEvent::GetInterval()
{
    UINT32 dwInterval = 3600 * 24 - 1; //避免不准，提前1秒
    UINT32 dwPassSec = GetDayPassSecond();
    UINT32 dwTime = 0;
    const SCourageActivityTimeProp* poProp = CCouraPropMgr::Instance()->GetCouragePropByID(m_byActivityID);
    if(NULL == poProp)
    {
        SYS_CRITICAL( _SDT("[%s: %d] SCourageActivityTimeProp[%d] Failed!"), MSG_MARK, m_byActivityID);
        return 0xFFFFFFFF; //相当于清除该定时器
    }
    //开始活动
    if(m_bStartFlag)
    {
        //活动已经结束，则活动开启为明天
        if(dwPassSec >= poProp->dwEndTime)
        {
            dwInterval = (3600 * 24 - dwPassSec) + poProp->dwStartTime;
        }
        //活动未开启
        else if(dwPassSec < poProp->dwStartTime)
        {
            dwInterval = poProp->dwStartTime - dwPassSec;
        }
        //活动已经开启，未结束
        else
        {
            //已过开启时间，则马上启动
            if(m_bFirstFlag)
            {
                dwInterval = 1;
            }
            //已经开启，则下次为明天
            else
            {
                dwInterval = (3600 * 24 - dwPassSec) + poProp->dwStartTime;
            }
        }

        m_bFirstFlag  = FALSE;
    }
    //关闭活动
    else
    {
        //活动已经结束，则活动开启为明天
        if(dwPassSec >= poProp->dwEndTime)
        {
            dwInterval = (3600 * 24 - dwPassSec) + poProp->dwEndTime;
        }
        //活动未结束
        else
        {
            dwInterval = poProp->dwEndTime - dwPassSec;
        }
    }

    return  dwInterval;
}

