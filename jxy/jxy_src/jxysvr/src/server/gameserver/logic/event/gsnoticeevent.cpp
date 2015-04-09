#include "gsnoticeevent.h"
#include <logic/other/gsnoticemgr.h>

CGetGsNoticeEvent::CGetGsNoticeEvent()
{

}

CGetGsNoticeEvent::~CGetGsNoticeEvent()
{

}

UINT32 CGetGsNoticeEvent::OnEvent()
{
	CGsNoticeMgr::Instance()->OnEvent();
    return 0;
}

INT32  CGetGsNoticeEvent::OnDestroy()
{
    return 0;
}

UINT32	CGetGsNoticeEvent::GetInterval()
{
    return ms_nInterval;
}