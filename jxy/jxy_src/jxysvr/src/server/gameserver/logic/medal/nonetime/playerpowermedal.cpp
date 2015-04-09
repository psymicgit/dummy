///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\playerpowermedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 17:10:26
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/playerpowermedal.h"
#include "logic/player/player.h"
BOOL CPlayerPowerMedal::CheckActivate()
{
	return FALSE;
	if ( m_stMedalData.byIsActivate)
	{
		return TRUE;
	}

	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		return FALSE;
	}


	if ( (poMedalProp->qwFirstCondition <= m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower) && (m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}