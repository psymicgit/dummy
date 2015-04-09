///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\racecontinuekillmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ30ÈÕ 10:3:49
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/racecontinuekillmedal.h"
#include "logic/race/race.h"

BOOL CRaceContinueKillMedal::CheckActivate()
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

	CRace* poRace = m_poOwner->GetRace();
	if ( NULL == poRace)
	{
		return FALSE;
	}

	DT_RACE_BASE_DATA& stBaseData = poRace->GetDT_RACE_BASE_DATA();
	if ( poMedalProp->qwFirstCondition <= stBaseData.wContiWin)
	{
		return TRUE;
	}

	return FALSE;
}