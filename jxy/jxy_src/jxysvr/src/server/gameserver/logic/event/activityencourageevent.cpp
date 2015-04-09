#include "activityencourageevent.h"
#include <db/autosqlbuf.h>
#include <logic/activityencourage/activityencouragemgr.h>
#include <logic/activityencourage/oprateingactivitymgr.h>


CCActivityEncourageEvent::CCActivityEncourageEvent()
{

}

CCActivityEncourageEvent::~CCActivityEncourageEvent()
{

}

UINT32 CCActivityEncourageEvent::OnEvent()
{
	CActivityEncourageMgr::Instance()->OnEvent();
	COprateingActivityMgr::Instance()->OnEvent();
    return 0;
}

INT32  CCActivityEncourageEvent::OnDestroy()
{
    return 0;
}

UINT32	CCActivityEncourageEvent::GetInterval()
{
    return ms_nInterval;
}