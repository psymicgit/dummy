
#include "lsevent.h"
#include "framework/lsapplication.h"
#include <sdutil.h>

CLSEvent::CLSEvent()
{

}

CLSEvent::~CLSEvent()
{
}

UINT32 CLSEvent::OnEvent()
{   
	((CLSApplication*)SDGetApp())->OnEvent();
	return 0;
}

INT32  CLSEvent::OnDestroy()
{
    return 0;
}

