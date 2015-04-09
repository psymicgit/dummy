///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\playerlevelmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 17:6:47
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/playerlevelmedal.h"

BOOL CPlayerLevelMedal::CheckActivate()
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


	if ( poMedalProp->qwFirstCondition <= m_poOwner->GetLevel())
	{
		return TRUE;
	}

	return FALSE;
}