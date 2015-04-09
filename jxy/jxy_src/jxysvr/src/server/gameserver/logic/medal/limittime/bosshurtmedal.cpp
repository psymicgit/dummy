///<------------------------------------------------------------------------------
//< @file:   medal\limittime\bosshurtmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ21ÈÕ 16:29:40
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#include "logic/medal/limittime/bosshurtmedal.h"
#include "logic/activity/bossbattle.h"
#include "dll/sdu/sdtime.h"

CBossHurtMedal::CBossHurtMedal()
{

}

CBossHurtMedal::~CBossHurtMedal()
{

}

BOOL CBossHurtMedal::CheckActivate()
{
	return FALSE;
	if ( m_stMedalData.byIsActivate )
	{
		return TRUE;
	}

	CBossBattle* poBossBattle = m_poOwner->GetBossBattle();
	if ( NULL == poBossBattle)
	{
		return FALSE;
	}

	CSDDateTime oNowTime = SDNow();
	CSDDateTime oActivate(poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().qwBossHurtTime);
	if ( 1 == oNowTime.DiffDateDay( oActivate) )
	{
		return TRUE;
	}
	return FALSE;
}