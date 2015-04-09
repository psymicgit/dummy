///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\playerviplevelmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 17:13:35
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/playerviplevelmedal.h"
#include "logic/player/player.h"

BOOL CPlayerVipLevelMedal::CheckActivate()
{
	if ( m_stMedalData.byIsActivate)
	{
		return TRUE;
	}
	if ( 0 == GetLevel())
	{
		return FALSE;
	}
	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		return FALSE;
	}


	if ( poMedalProp->qwFirstCondition <= m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel)
	{
		SetActivateState(TRUE);
		return TRUE;
	}

	return FALSE;
}
