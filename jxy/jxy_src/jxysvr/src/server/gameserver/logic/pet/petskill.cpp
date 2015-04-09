///<------------------------------------------------------------------------------
//< @file:   logic\pet\petskill.cpp
//< @author: Kang_zhiquan
//< @date:   2014年8月27日 10:3:58
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "petskill.h"
#include "logic/skill/skillaffecthandler.h"
#include "logic/skill/buffaffecthandler.h"
#include "sdloggerimpl.h"
#include "logic/pet/pet.h"
#include "logic/player/player.h"
#include "logic/skill/skillpropmgr.h"
IMPLEMENT_CLASS_POOL_MGR(CPetSkill);

CPetSkill::CPetSkill()
{
	m_stProp.clear();
	m_wSkillID = 0;
}
CPetSkill::~CPetSkill()
{
	m_stProp.clear();
	m_wSkillID = 0;
}

BOOL CPetSkill::Init(UINT16 wSkillID, CPlayer* poPlayer, CPet* poPet)
{
	if ( NULL == poPlayer || NULL == poPet)
	{
		return FALSE;
	}
	m_wSkillID = wSkillID;
	m_poOwner = poPlayer;
	m_poPet = poPet;
	SetPetSkillProp();
	return TRUE;
}

UINT16 CPetSkill::GetID()
{
	return m_wSkillID;
}

UINT8 CPetSkill::GetAttackType()
{
	return m_stProp.byAttackRangType;
}

UINT8 CPetSkill::GetSkillType()
{
	return m_stProp.byOnceEffectID;
}


UINT16 CPetSkill::GetHurtRate()
{
	return m_stProp.wHurtRate;
}

UINT16 CPetSkill::GetFinalHurtRate()
{
	return m_stProp.wFinalHurtValue;
}

UINT32 CPetSkill::GetOnceParam()
{
	return m_stProp.wOnceEffectParam;
}

VOID CPetSkill::GetTriger(vector<CBattleAttrObj*>& vecTargetObj, CBattleObj* poTarget)
{
	if ( NULL == poTarget)
	{
		return ;
	}
	
	UINT32 dwSkillNum = CSkillPropMgr::Instance()->GetPetSkillTrigerNum(m_wSkillID, m_poPet->GetLevel());
	CRandomMap mapRandom;
	for(UINT8 byIdx = 1; byIdx <= 6; byIdx++)
	{
		CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
		if((poObj) && (!poObj->IsDead()))
		{
			mapRandom[byIdx] = 100;
		}
	}
	while( ( 0 != dwSkillNum) && (0 != mapRandom.size()))
	{
		UINT32 dwIdx = GetRandomIdxMustHit(mapRandom);
		vecTargetObj.push_back(poTarget->m_apoFormationObj[dwIdx]);
		mapRandom.erase(dwIdx);
		dwSkillNum--;
	}
		

}


VOID CPetSkill::SetPetSkillProp()
{
	m_stProp.clear();
	SPetSkillEffectProp * poSkillEffectProp = CSkillPropMgr::Instance()->GetPetSkillProp(m_wSkillID, 1);
	if ( NULL != poSkillEffectProp)
	{
		m_stProp = *poSkillEffectProp;
		return ;
	}
	SRandomPetSkillProp* poRandomProp = CSkillPropMgr::Instance()->GetPetRandomSkillProp(m_wSkillID, 1);
	if ( NULL != poRandomProp)
	{
		UINT32 dwSize = poRandomProp->vecRandomProp.size();
		if ( 0 == dwSize)
		{
			return ;
		}
		UINT32 dwIdx = RandomReseed(1000) % dwSize;
		m_stProp = poRandomProp->vecRandomProp[dwIdx];
		m_stProp.wSkillLevel = poRandomProp->stMainProp.wSkillLevel;
		m_stProp.strSkillName = poRandomProp->stMainProp.strSkillName;
		m_stProp.strSkillDesc = poRandomProp->stMainProp.strSkillDesc;
		m_stProp.byMaxTargetNum = poRandomProp->stMainProp.byMaxTargetNum;
	}

}
BOOL CPetSkill::Use(ESkillAffectPhase ePhase, CBattleObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, CPet* poSkillOwner,
	vector<CBattleAttrObj*>& vecEnemyTargetObj, CBattleAttrObj* poSingleTarget)
{
	CSkillOnceAffect* poSkillOnceAffect = CSkillOnceAffectHandler::Instance()->GetSkillAffect(m_stProp.byOnceEffectID);
	if(NULL == poSkillOnceAffect)
	{
		DBG_INFO(_SDT("[%s: %d]: GetSkillAffect failed, EffectKindID[%d]!"), MSG_MARK, m_stProp.byOnceEffectID);
		return FALSE;
	}
	if ( (ESAP_SECOND_ATTACK != ePhase) && (ESEK_SECOND_ATTACK == poSkillOnceAffect->GetSkillEffectKind()) )
	{
		UINT32 dwValue = RandomReseed(1000) % 100;
		if ( dwValue < m_stProp.wOnceEffectRate)
		{
			poSkillOwner->GetSecondHrut() = this;
		}
	}
	poSkillOwner->GetFinalAddHurt() += m_stProp.wFinalHurtValue;
	if(ESAP_BEFORE_COUNT_HURT == ePhase)
	{
		poSkillOnceAffect->Affect(ePhase, poSkillOwner, poMyself, poEnemy, m_stProp.wHurtRate, m_stProp.dwExtHurtValue, m_stProp.wOnceEffectParam, m_stProp.wOnceEffectRate, NULL);
	}
	else if(ESAP_HIT_HURT == ePhase)
	{
		UINT8 bySize = vecEnemyTargetObj.size();
		for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
		{
			CBattleAttrObj* poTarget = vecEnemyTargetObj[byIdx];
			poSkillOnceAffect->Affect(ePhase, poSkillOwner, poMyself, poEnemy, m_stProp.wHurtRate, m_stProp.dwExtHurtValue, m_stProp.wOnceEffectParam, m_stProp.wOnceEffectRate, poTarget);
		}
	}
	else if ( ESAP_SECOND_ATTACK == ePhase)
	{
		UINT8 bySize = vecEnemyTargetObj.size();
		for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
		{
			CBattleAttrObj* poTarget = vecEnemyTargetObj[byIdx];
			poSkillOnceAffect->Affect(ePhase, poSkillOwner, poMyself, poEnemy, m_stProp.wHurtRate, m_stProp.dwExtHurtValue, m_stProp.wOnceEffectParam, m_stProp.wOnceEffectRate, poTarget);
		}
	}
	UINT8 byBuffSize = m_stProp.vecBuffProp.size();
	//添加buff
	for(UINT8 byBuffIdx = 0; byBuffIdx < byBuffSize; byBuffIdx++)
	{
		const SSkillOnceBuffProp& stBuffProp = m_stProp.vecBuffProp[byBuffIdx];
		if(0 == stBuffProp.byBuffKindID)
		{
			continue;
		}
		CBuffAffect* pstBuffAffect = CBuffAffectHandler::Instance()->GetBuffAffect(stBuffProp.byBuffKindID);
		if(NULL == pstBuffAffect)
		{
			continue;
		}
		CBattleObj* poTarget = NULL;
		//作用自身不需命中,开始就有效
		if(EBT_MYSELF == pstBuffAffect->GetBuffTarget())
		{
			if(ESAP_BEFORE_COUNT_HURT != ePhase)
			{
				continue;
			}
			CSkillMgr::Instance()->AddBuff(poSkillOwner, poSkillOwner, m_stProp.wSkillID, stBuffProp);
		}
		else if ( EBT_MY_FIRST == pstBuffAffect->GetBuffTarget())
		{
			if(ESAP_HIT_HURT != ePhase)
			{
				continue;
			}
			for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
			{
				CBattleAttrObj* poObj = poMyself->GetFormationObj(byIdx);
				if((poObj) && (!poObj->IsDead()))
				{
					CSkillMgr::Instance()->AddBuff(poSkillOwner, poObj, m_stProp.wSkillID, stBuffProp);
					return TRUE;
				}
			}
		}
		else if(EBT_MYALL == pstBuffAffect->GetBuffTarget())
		{
			if(ESAP_BEFORE_COUNT_HURT != ePhase)
			{
				continue;
			}
			for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
			{
				CBattleAttrObj* poObj = poMyself->GetFormationObj(byIdx);
				if((poObj) && (!poObj->IsDead()))
				{
					CSkillMgr::Instance()->AddBuff(poSkillOwner, poObj, m_stProp.wSkillID, stBuffProp);
				}
			}
		}
		else
		{
			//给敌方的buff都是攻击后
			if(ESAP_HIT_HURT != ePhase)
			{
				continue;
			}
			if(poSingleTarget)
			{
				CSkillMgr::Instance()->AddBuff(poSkillOwner, poSingleTarget, m_stProp.wSkillID, stBuffProp);
			}
			else
			{
				UINT8 bySize = vecEnemyTargetObj.size();
				for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
				{
					CSkillMgr::Instance()->AddBuff(poSkillOwner, vecEnemyTargetObj[byIdx], m_stProp.wSkillID, stBuffProp);
				}
			}
		}
	}

	return TRUE;
}