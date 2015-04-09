#include "bossbattleevent.h"
#include "timereventmgr.h"
#include <logic/activity/bossbattlemgr.h>
#include <sdutil.h>


CBossBattleEvent::CBossBattleEvent()
{
	m_bStartFlag = TRUE;
	m_bFirstFlag = TRUE;
	m_byActivityID = 0;
}

CBossBattleEvent::~CBossBattleEvent()
{

}

UINT32 CBossBattleEvent::OnEvent()
{
	if(m_bStartFlag)
	{
		CBossBattleMgr::Instance()->OnStartEvent(m_byActivityID);
	}
	else
	{
		CBossBattleMgr::Instance()->OnEndEvent(m_byActivityID);
	}
	
	CTimerEventMgr::Instance()->ResetBossBattleEventInterval(this);

	return 0;
}

BOOL CBossBattleEvent::OnDestroy()
{
	return TRUE;
}

UINT32 CBossBattleEvent::GetInterval()
{
	UINT32 dwInterval = 3600 * 24-1; //避免不准，提前1秒
	UINT32 dwPassSec = GetDayPassSecond();
	UINT32 dwTime = 0;
	SBossBProp* poBossBProp = CBossBattlePropMgr::Instance()->GetBossBPropByID(m_byActivityID);
	if(NULL == poBossBProp)
	{
		SYS_CRITICAL( _SDT("[%s: %d] GetBossBPropByID[%d] Failed!"), MSG_MARK, m_byActivityID);		
		return 0xFFFFFFFF; //相当于清除该定时器
	}
	//开始活动
	if(m_bStartFlag)
	{
		//活动已经结束，则活动开启为明天
		if(dwPassSec >= poBossBProp->dwEndTime)
		{
			dwInterval = (3600 * 24 - dwPassSec) + poBossBProp->dwStartEnterTime;
		}
		//活动未开启
		else if(dwPassSec < poBossBProp->dwStartEnterTime)
		{
			dwInterval = poBossBProp->dwStartEnterTime - dwPassSec;
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
				dwInterval = (3600 * 24 - dwPassSec) + poBossBProp->dwStartEnterTime;
			}			
		}

		m_bFirstFlag  = FALSE;
	}
	//关闭活动
	else
	{
		//活动已经结束，则活动开启为明天
		if(dwPassSec >= poBossBProp->dwEndTime)
		{
			dwInterval = (3600 * 24 - dwPassSec) + poBossBProp->dwEndTime;
		}
		//活动未结束
		else
		{
			dwInterval = poBossBProp->dwEndTime - dwPassSec;
		}
	}	

	return  dwInterval;
}

