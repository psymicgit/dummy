///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\jiebiaomedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ26ÈÕ 21:36:40
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/jiebiaomedal.h"
#include "logic/medal/playermedal.h"
BOOL CJieBiaoMedal::CheckActivate()
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
	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwEscortNum) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwEscortNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}