///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\gvecouragemedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ30ÈÕ 13:54:11
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/gvecouragemedal.h"
#include "logic/medal/playermedal.h"
BOOL CGVECourageMedal::CheckActivate()
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
	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwGVEJoinNum) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwGVEJoinNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}