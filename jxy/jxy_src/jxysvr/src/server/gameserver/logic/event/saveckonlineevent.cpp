#include "saveckonlineevent.h"
#include <logic/player/playermgr.h>

CSaveOnlineEvent::CSaveOnlineEvent()
{
}

CSaveOnlineEvent::~CSaveOnlineEvent()
{
}

UINT32 CSaveOnlineEvent::OnEvent()
{
	CPlayerMgr::Instance()->OnSaveOnlineEvent(m_dwPlayerID);

	return 0;
}

BOOL CSaveOnlineEvent::OnDestroy()
{
	return TRUE;
}

VOID CSaveOnlineEvent::SetParam(UINT32 dwPlayerID)
{
	m_dwPlayerID = dwPlayerID;
}