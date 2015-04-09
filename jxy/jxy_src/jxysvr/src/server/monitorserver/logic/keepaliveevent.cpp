
#include "keepaliveevent.h"
#include "framework/msapplication.h"
#include "logic/monitor.h"
#include <sdutil.h>



CKeepAliveEvent::CKeepAliveEvent()
{

}

CKeepAliveEvent::~CKeepAliveEvent()
{

}

UINT32 CKeepAliveEvent::OnEvent()
{
	CMonitor::Instance()->OnEvent();
    return 0;
}

INT32  CKeepAliveEvent::OnDestroy()
{
    return 0;
}

