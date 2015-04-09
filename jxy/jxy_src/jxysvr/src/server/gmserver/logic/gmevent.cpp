
#include "gmevent.h"
#include "framework/gmapplication.h"
#include <sdutil.h>

CGMEvent::CGMEvent()
{
}

CGMEvent::~CGMEvent()
{
}

UINT32 CGMEvent::OnEvent()
{   
	CGMApplication* poZSApplication = (CGMApplication*)SDGetApp();
	return 0;
}

INT32  CGMEvent::OnDestroy()
{
	return 0;
}

VOID CGMEvent::CheckSession()
{

}

