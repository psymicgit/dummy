///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\reseteliteinstancemedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ27ÈÕ 19:28:43
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/reseteliteinstancemedal.h"
#include "logic/medal/playermedal.h"

BOOL CResetEliteInstanceMedal::CheckActivate()
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
	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwEliteInstanceResetNum) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwEliteInstanceResetNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}