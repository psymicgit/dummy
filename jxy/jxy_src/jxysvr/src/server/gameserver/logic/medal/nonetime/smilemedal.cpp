///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\smilemedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ26ÈÕ 21:49:17
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#include "logic/medal/nonetime/smilemedal.h"
#include "logic/medal/playermedal.h"

BOOL CSmileMedal::CheckActivate()
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
	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwSmileNum) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwSmileNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}