#include "factiondonate.h"

BOOL CFactionDonate::Init(DT_PLAYER_DONATE_INFO &stPlayerDonate, CPlayer* poOwner)
{
	memset(&m_stDoateInfo, 0, sizeof(DT_PLAYER_DONATE_INFO));
	memcpy(&m_stDoateInfo, &stPlayerDonate, sizeof(DT_PLAYER_DONATE_INFO));
	m_poOwner = poOwner;
	return TRUE;
}