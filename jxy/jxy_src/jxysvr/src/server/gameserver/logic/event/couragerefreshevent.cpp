#include "couragerefreshevent.h"
#include "net/recharge/rechargeclient.h"
#include <db/autosqlbuf.h>
#include <logic/activity/couragemgr.h>

CCourageRefreshEvent::CCourageRefreshEvent()
{

}

CCourageRefreshEvent::~CCourageRefreshEvent()
{

}

UINT32 CCourageRefreshEvent::OnEvent()
{
    CCourageMgr::Instance()->OnEvent();
    return 0;
}

INT32  CCourageRefreshEvent::OnDestroy()
{
    return 0;
}

UINT32	CCourageRefreshEvent::GetInterval()
{
    return ms_nInterval;
}