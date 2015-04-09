///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\climbotwermedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 11:43:31
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#include "logic/medal/nonetime/climbtowermedal.h"
#include "logic/instance/instance.h"

VOID CClimbTowerMedal::GetMedalCli(DT_MEDAL_DATA_CLI& stCli)
{
	
}

BOOL CClimbTowerMedal::CheckActivate()
{
	return FALSE;
	if ( m_stMedalData.byIsActivate)
	{
		return TRUE;
	}

	CInstance& oInstance = m_poOwner->GetInstance();
	DT_INSTANCE_ID& stInstance = oInstance.GetMaxClimbTownInstanceID();

	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		return FALSE;
	}


	if ( (poMedalProp->qwFirstCondition <= stInstance.byInstanceIdx) && (stInstance.byInstanceIdx <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
		
}