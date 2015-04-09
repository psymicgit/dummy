#include "chatevent.h"
#include "net/recharge/rechargeclient.h"
#include <db/autosqlbuf.h>
#include <logic/chat/chatmgr.h>

CChatEvent::CChatEvent()
{

}

CChatEvent::~CChatEvent()
{

}

UINT32 CChatEvent::OnEvent()
{
	CChatMgr::Instance()->OnEvent();
    return 0;
}

INT32  CChatEvent::OnDestroy()
{
    return 0;
}

UINT32	CChatEvent::GetInterval()
{
    return ms_nInterval;
}