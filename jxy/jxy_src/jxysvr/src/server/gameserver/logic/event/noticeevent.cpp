#include "noticeevent.h"
#include "net/recharge/rechargeclient.h"
#include <db/autosqlbuf.h>
#include <logic/notice/noticemgr.h>
#include <logic/event/timereventmgr.h>
#include <common/client/commondef.h>

CNoticeEvent::CNoticeEvent()
{

}

CNoticeEvent::~CNoticeEvent()
{

}

UINT32 CNoticeEvent::OnEvent()
{
	switch(m_byNoticeType)
	{
	case ENT_OUTLINE:
		break;
	case ENT_RACE:
		CNoticeMgr::Instance()->OnRankNotify();
		break;
	case ENT_EATBREAD:
		CNoticeMgr::Instance()->OnActivityNotifyEatBread(0, 0);
		break;
	case ENT_BOSSBATTLE:
		CNoticeMgr::Instance()->OnActivityNotifyBossBattle(0, 0);
		break;
    case ENT_COURAGETTLE:
        CNoticeMgr::Instance()->OnActivityNotifyCourage(0, 0);
        break;
	case ENT_OTHER:
		break;
	default:
		break;
	}

	CTimerEventMgr::Instance()->ResetNotityEventInterval(this);
    return 0;
}

INT32  CNoticeEvent::OnDestroy()
{
    return 0;
}

UINT32	CNoticeEvent::GetInterval()
{
	UINT32 dwInterval = 3600 * 24 - 1; //避免不准，提前1秒
	UINT32 dwPassSec = GetDayPassSecond();

	if (ENT_RACE == m_byNoticeType)
	{
		//活动已经结束，则活动开启为明天
		if(dwPassSec >= m_dwStartTime)
		{
			dwInterval = 3600 * 24 - dwPassSec;
		}
		//活动未开启
		else
		{
			dwInterval = m_dwStartTime - dwPassSec;
		}
	}
	else if (ENT_EATBREAD == m_byNoticeType)
	{
		//活动已经结束，则活动开启为明天
		if(dwPassSec >= m_dwStartTime)
		{
			dwInterval = 3600 * 24 - dwPassSec;
		}
		else 
		{
			dwInterval = m_dwStartTime - dwPassSec;
		}
	}
	else if (ENT_BOSSBATTLE == m_byNoticeType)
	{
		//活动已经结束，则活动开启为明天
		if(dwPassSec >= m_dwStartEnterTime)
		{
			dwInterval = 3600 * 24 - dwPassSec;
		}
		else
		{
			dwInterval = m_dwStartEnterTime - dwPassSec;
		}
	}
    else if (ENT_COURAGETTLE == m_byNoticeType)
    {
        //活动已经结束，则活动开启为明天
        if(dwPassSec >= m_dwStartEnterTime)
        {
            dwInterval = 3600 * 24 - dwPassSec;
        }
        else
        {
            dwInterval = m_dwStartEnterTime - dwPassSec;
        }
    }
	return  dwInterval;
}