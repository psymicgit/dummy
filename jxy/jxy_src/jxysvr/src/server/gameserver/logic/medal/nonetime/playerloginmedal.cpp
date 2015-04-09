///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\playerloginmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 17:22:44
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/playerloginmedal.h"
#include "logic/medal/playermedal.h"
BOOL CPlayerLoginMedal::CheckActivate()
{
	return FALSE;
	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		return FALSE;
	}
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL == poPlayerMedal)
	{
		return FALSE;
	}
	DT_MEDAL_BASE_DATA& stBaseData = poPlayerMedal->GetDT_MEDAL_BASE_DATA();
	if ( (poMedalProp->qwFirstCondition <= stBaseData.dwLoginDayNum) && (stBaseData.dwLoginDayNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CPlayerLoginMedal::IsActivate()
{
	SetActivateState(FALSE);
	return FALSE;
}