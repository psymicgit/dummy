///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\worshipplayermedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ26ÈÕ 21:51:15
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/worshipplayermedal.h"
#include "logic/medal/playermedal.h"

BOOL CWorshipPlayerMedal::CheckActivate()
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
	UINT32 dwSumNum = poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipPlayer1Num + poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipGod1Num;
	if ( dwSumNum >= poMedalProp->qwFirstCondition)
	{
		return TRUE;
	}

// 	switch (poMedalProp->qwFirstCondition)
// 	{
// 	case 1:
// 		if ( poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipPlayer1Num >= poMedalProp->qwSecondCondition)
// 		{
// 			return TRUE;
// 		}
// 		break;
// 	case 2:
// 		if ( poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipPlayer2Num >= poMedalProp->qwSecondCondition)
// 		{
// 			return TRUE;
// 		}
// 		break;
// 	case 3:
// 		if ( poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwWorshipPlayer3Num >= poMedalProp->qwSecondCondition)
// 		{
// 			return TRUE;
// 		}
// 		break;
// 	}

	return FALSE;
}