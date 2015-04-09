#include "crossdayevent.h"
#include <logic/other/crossdaymgr.h>

CCrossDayEvent::CCrossDayEvent()
{

}

CCrossDayEvent::~CCrossDayEvent()
{

}

UINT32 CCrossDayEvent::OnEvent()
{
	CCrossDayMgr::OnCrossDayEvent();
    return 0;
}

INT32  CCrossDayEvent::OnDestroy()
{
    return 0;
}

UINT32	CCrossDayEvent::GetInterval()
{	
	CSDDateTime oT240000;
	CSDDateTime oNow = SDNow();
	oNow.IncDay();
	oT240000.SetDateTime(oNow.GetYear(), oNow.GetMonth(), oNow.GetDay(), 0,  0, 0); 

	return  (UINT32)(oT240000.GetTimeValue() - SDTimeSecs());
}

