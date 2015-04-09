#include "mark.h"

BOOL CPlayerMark::Init(DT_PLAYER_MARK_DATA& stPlayerMarkData, CPlayer* poPlayer)
{
	memset(&m_stPlayerMarkData, 0, sizeof(DT_PLAYER_MARK_DATA));
	memcpy(&m_stPlayerMarkData, &stPlayerMarkData, sizeof(DT_PLAYER_MARK_DATA));
	m_poOwner = poPlayer;
	return TRUE;
}