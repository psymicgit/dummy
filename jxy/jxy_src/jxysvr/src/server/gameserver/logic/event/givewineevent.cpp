#include "givewineevent.h"
#include <logic/faction/factionprop.h>
#include <logic/faction/factionmgr.h>
#include <logic/event/timereventmgr.h>
CGiveWineEvent::CGiveWineEvent()
{
	m_dwFactionID = 0;
}
CGiveWineEvent::~CGiveWineEvent()
{
	m_dwFactionID = 0;
}
	
UINT32 CGiveWineEvent::OnEvent()
{
	CFaction* poFaction = CFactionMgr::Instance()->GetFaction(m_dwFactionID);
	if (NULL == poFaction)
	{
		return 0;
	}
	//赐酒结束
	poFaction->GetDT_FACTION_COMMON_DATA().stHotSpringData.byGiveWineOpenState = ESM_NO;
	//处理找回饮酒
	poFaction->AfterGiveWineProc();
	CTimerEventMgr::Instance()->DelGiveWineEvent(m_dwFactionID);


	return 0;
}
INT32 CGiveWineEvent::OnDestroy()
{
	return 0;
}

VOID CGiveWineEvent::SetParam( UINT32 dwFactionID )
{
	m_dwFactionID = dwFactionID;
}
