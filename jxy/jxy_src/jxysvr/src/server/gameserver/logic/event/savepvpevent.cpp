#include "savepvpevent.h"
#include <logic/player/playersavemgr.h>

CSavePvpEvent::CSavePvpEvent()
{
}

CSavePvpEvent::~CSavePvpEvent()
{
}

UINT32 CSavePvpEvent::OnEvent()
{
	CPlayerSaveMgr::Instance()->SaveAllPlayerPvpData();

	return 0;
}

BOOL CSavePvpEvent::OnDestroy()
{
	return TRUE;
}
