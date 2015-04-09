///<------------------------------------------------------------------------------
//< @file:   medal\limittime\racemedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014年6月18日 20:1:56
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/limittime/racemedal.h"
#include "logic/race/racemgr.h"
#include "logic/medal/playermedal.h"

CRaceMedal::CRaceMedal()
{

}

CRaceMedal::~CRaceMedal()
{

}
BOOL CRaceMedal::CheckActivate()
{
	if ( IsOverTime() )
	{
		return FALSE;
	}
	//已激活
	if ( 1 == m_stMedalData.byIsActivate)
	{
		return TRUE;
	}

	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		return FALSE;
	}

	if ( 0 == poMedalProp->wLevel)
	{
		return FALSE;
	}
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL == poPlayerMedal)
	{
		return FALSE;
	}

	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwRaceRank) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwRaceRank <= poMedalProp->qwSecondCondition))
	{
		CSDDateTime oNowTime = SDNow();
		oNowTime.SetTime(0, 0, 0);
		SetActivateState(TRUE, oNowTime.GetTimeValue());
		return TRUE;
	}
	
	return FALSE;
}

