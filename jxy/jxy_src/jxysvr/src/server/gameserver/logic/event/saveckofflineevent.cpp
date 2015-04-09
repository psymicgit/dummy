#include "saveckofflineevent.h"
#include <logic/player/playermgr.h>

CSaveCkOfflineEvent::CSaveCkOfflineEvent()
{
}

CSaveCkOfflineEvent::~CSaveCkOfflineEvent()
{
}

UINT32 CSaveCkOfflineEvent::OnEvent()
{
	CPlayerMgr::Instance()->OnSaveCkOfflineEvent(m_dwPlayerID);

	return 0;
}

BOOL CSaveCkOfflineEvent::OnDestroy()
{
	return TRUE;
}

VOID CSaveCkOfflineEvent::SetParam(UINT32 dwPlayerID)
{
	m_dwPlayerID = dwPlayerID;
}