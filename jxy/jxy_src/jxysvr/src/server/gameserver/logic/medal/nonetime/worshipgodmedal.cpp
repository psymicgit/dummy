///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\worshipgodmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ26ÈÕ 21:51:18
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/worshipgodmedal.h"
#include "logic/medal/playermedal.h"

BOOL CWorshipGodMedal::CheckActivate()
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
	switch (poMedalProp->qwFirstCondition)
	{
	case 1:
		if ( poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipGod1Num >= poMedalProp->qwSecondCondition)
		{
			return TRUE;
		}
		break;
	case 2:
		if ( poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipGod2Num >= poMedalProp->qwSecondCondition)
		{
			return TRUE;
		}
		break;
	case 3:
		if ( poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipGod2Num >= poMedalProp->qwSecondCondition)
		{
			return TRUE;
		}
		break;
	}

	return FALSE;
}