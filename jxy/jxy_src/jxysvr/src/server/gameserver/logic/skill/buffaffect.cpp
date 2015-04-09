#include "buffaffect.h"
#include <logic/battle/battleattrobj.h>
#include <common/client/commondef.h>
#include <db/autosqlbuf.h>


//静止
BOOL CBuffAffect_Static::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		poActor->GetCanNormalAttack() = FALSE;
		poActor->GetCanSkillAttack() = FALSE;

		return TRUE;
	}

	//被静止了，整个回合(攻击+受击回合)不能闪避,不算回合生效
	poActor->GetCanDodge() = FALSE;

	return FALSE;
}


//沉默
BOOL CBuffAffect_NoSkill::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		poActor->GetCanSkillAttack() = FALSE;

		return TRUE;
	}

	return FALSE;
}


//中毒
BOOL CBuffAffect_Poisoning::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{	
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		//掉血
		poActor->AddHPChange(-(INT32)(poGiver->GetDT_BATTLE_ATTRIBUTE().dwAttack * 	wParamValue / 100), ET_BUFF, EHPCR_BUFF, 0);

		return TRUE;
	}
	return FALSE;
}


//撕裂
BOOL CBuffAffect_Tear::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{	
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		//掉血
		poActor->AddHPChange(-(INT32)(poGiver->GetDT_BATTLE_ATTRIBUTE().dwAttack * 	wParamValue / 100), ET_BUFF, EHPCR_BUFF, 0);

		return TRUE;
	}
	return FALSE;
}

//降低攻击
BOOL CBuffAffect_DecAttack::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		//减少攻击百分比(减少伤害百分比)
		if(wParamValue <= 100 )
		{
			poActor->GetAttackHurtRate() = poActor->GetAttackHurtRate() * (100 - wParamValue) / 100;
		}
		else
		{
			poActor->GetAttackHurtRate() = 0;
		}
		//SYS_CRITICAL(_SDT("[%s: %d]: m_wAttackHurtRate:%d"), MSG_MARK, poActor->GetAttackHurtRate());

		

		return TRUE;
	}

	return FALSE;
}


//嗜血， 使得拥有该状态者本回合内攻击力按百分比提升，属于增益状态	
BOOL CBuffAffect_BloodthirstySingle::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		//提升攻击百分比
		poActor->GetAttackHurtRate() = poActor->GetAttackHurtRate() * (100 + wParamValue) / 100;
		//SYS_CRITICAL(_SDT("[%s: %d]: m_wAttackHurtRate:%d"), MSG_MARK, poActor->GetAttackHurtRate());
		return TRUE;
	}

	return FALSE;
}


//嗜血， 使得拥有该状态者本回合内攻击力按百分比提升，属于增益状态	
BOOL CBuffAffect_BloodthirstyAll::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		//提升攻击百分比
		poActor->GetAttackHurtRate() = poActor->GetAttackHurtRate() * (100 + wParamValue) / 100;
		//SYS_CRITICAL(_SDT("[%s: %d]: m_wAttackHurtRate:%d"), MSG_MARK, poActor->GetAttackHurtRate());
		return TRUE;
	}

	return FALSE;
}


//减伤，使得拥有该状态者受击命中时减少伤害值，属于增益状态
BOOL CBuffAffect_DecHurtSingle::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为受击者时生效
	if(poNormalAttackTarget == poActor)
	{
		//减少伤害百分比	
		poActor->GetDecHurtRate() = wParamValue;

		return TRUE;
	}

	return FALSE;
}

//减伤，使得拥有该状态者受击命中时减少伤害值，属于增益状态
BOOL CBuffAffect_DecHurtAll::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为受击者时生效
	if(poNormalAttackTarget == poActor)
	{
		//减少伤害百分比	
		poActor->GetDecHurtRate() = wParamValue;

		return TRUE;
	}

	return FALSE;
}

//增加血量BUFF
BOOL CBuffAffect_AddBlood::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为受击者时生效
	if(poAttacker == poActor)
	{
		poActor->AddHPChange( ( dwExtValue * wParamValue / 100), ET_BUFF, EHPCR_BUFF, 0);

		return TRUE;
	}

	return FALSE;
}

//加防
BOOL CBuffAffect_DefendPercent::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为受击者时生效
	if(poNormalAttackTarget == poActor)
	{
		//增加防御（百分比）
		poActor->GetAddDefendRate() = wParamValue;

		return TRUE;
	}

	return FALSE;
}

BOOL CBuffAffect_Frost::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		poActor->GetCanNormalAttack() = FALSE;
		poActor->GetCanSkillAttack() = FALSE;

		return TRUE;
	}

	//被静止了，整个回合(攻击+受击回合)不能闪避,不算回合生效
	poActor->GetCanDodge() = FALSE;

	return FALSE;
}

BOOL CBuffAffect_Chaos::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		if( EBK_NONE == poActor->GetChaos())
		{
			poActor->GetChaos() = EBK_CHAOS;
		}
		poActor->GetCanSkillAttack() = FALSE;
		return TRUE;
	}

	return FALSE;
}

BOOL CBuffAffect_Firing::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		//掉血
		poActor->AddHPChange(-(INT32)(dwExtValue * 	wParamValue / 100), ET_BUFF, EHPCR_BUFF, 0);

		return TRUE;
	}
	return FALSE;
}

BOOL CBuffAffect_Disturb::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		poActor->GetFinalDecHurt() = wParamValue;

		return TRUE;
	}
	return FALSE;
}

BOOL CBuffAffect_NotAnger::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		poActor->GetNotAnger() = TRUE;

		return TRUE;
	}
	return FALSE;
}

BOOL CBuffAffect_Sleep::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		poActor->GetCanNormalAttack() = FALSE;
		poActor->GetCanSkillAttack() = FALSE;

		return TRUE;
	}
	return FALSE;
}

BOOL CBuffAffect_Dreamland::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue)
{
	if(EBAP_ROUND_BEGIN != ePhase)
	{
		return FALSE;
	}

	//自己为攻击者时生效
	if(poActor == poAttacker)
	{
		poActor->GetChaos() = EBK_DREAMLAND;
		poActor->GetCanSkillAttack() = FALSE;
		poActor->GetFinalAddHurt() += wParamValue;	
		return TRUE;
	}

	return FALSE;
}

