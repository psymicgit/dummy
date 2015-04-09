#include "couragefireevent.h"
#include "net/recharge/rechargeclient.h"
#include <db/autosqlbuf.h>
#include <logic/activity/couragemgr.h>

CCourageFireEvent::CCourageFireEvent()
{
    m_dwPlayerID = 0;
}

CCourageFireEvent::~CCourageFireEvent()
{

}

UINT32 CCourageFireEvent::OnEvent()
{
    CCourageMgr::Instance()->OverTimeDismiss(m_dwPlayerID);
    return 0;
}

INT32  CCourageFireEvent::OnDestroy()
{
    return 0;
}

UINT32	CCourageFireEvent::GetInterval()
{
    return ms_nInterval;
}