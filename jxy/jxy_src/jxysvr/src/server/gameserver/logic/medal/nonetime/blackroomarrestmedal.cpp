///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\blackroomarrestmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ27ÈÕ 11:2:35
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/blackroomarrestmedal.h"
#include "logic/medal/playermedal.h"

BOOL CBlackRoomArrestMedal::CheckActivate()
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
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if(NULL == poPlayerMedal)
	{
		return FALSE;
	}
	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwArrestNum) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwArrestNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}