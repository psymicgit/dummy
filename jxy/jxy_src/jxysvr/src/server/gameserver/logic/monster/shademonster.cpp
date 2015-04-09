#include "shademonster.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/skill/skillmgr.h>
#include <logic/player/player.h>
#include <logic/skill/skillpropmgr.h>

using namespace SGDP;

CShadeMonster::CShadeMonster()
{
	m_wKindID = 0;
	m_byFormationIdx = 0;
	m_bBossBMonster = FALSE;
	m_wLevel = 0;
	m_poGodWeapon = NULL;
	m_bMonster = TRUE;

	for (UINT32 i = 0; i <= MAX_SKILL_SLOT_NUM; i++)
	{
		m_apoSlotSKill[i] = NULL;
	}
}

CShadeMonster::~CShadeMonster()
{
	if(m_poGodWeapon)
	{
		CMonsterGodWeaponPoolMgr::Instance()->Remove(m_poGodWeapon);
		m_poGodWeapon = NULL;
	}
	for (UINT32 i = 0; i <= MAX_SKILL_SLOT_NUM; i++)
	{
		if (NULL != m_apoSlotSKill[i])
		{
			CSkillPoolMgr::Instance()->Remove(m_apoSlotSKill[i]);
			m_apoSlotSKill[i] = NULL;
		}
	}
}

BOOL CShadeMonster::Init(UINT16 wKindID, UINT8 byFormationIdx, const SMonsterProp* poProp)
{
	m_stMonsterProp = *poProp;
	m_wKindID = wKindID;
	m_wLevel = poProp->byType;
	m_bBossBMonster = FALSE;


	if(!InitSkill())
	{
		return FALSE;
	}

	if(!InitGodweapon())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: pstMonsterUpgradeProp is NULL,Level[%d], Type[%d]!"), MSG_MARK, m_stMonsterProp.wLevel, m_stMonsterProp.byType);
		return FALSE;
	}

	m_byFormationIdx = byFormationIdx;

	//赋值战斗属性
	InitBattleAttribute();

	return TRUE;
}

BOOL CShadeMonster::InitGodweapon()
{
	const SMonsterProp* pstMonsterProp = GetMonsterProp();
	if(NULL == pstMonsterProp)
	{
		return FALSE;
	}

	if(0 == pstMonsterProp->dwGodweaponHP || 0 == pstMonsterProp->wGodweaponQuality)
	{
		return TRUE;
	}

	//怪物不会被销毁，则不需对象池，直接new
	m_poGodWeapon = CMonsterGodWeaponPoolMgr::Instance()->Create();
	if(!m_poGodWeapon->Init(pstMonsterProp->wGodweaponQuality, pstMonsterProp->wGodweaponLevel, pstMonsterProp->dwGodweaponHP, pstMonsterProp->dwGodweaponAttack, this))
	{
		CMonsterGodWeaponPoolMgr::Instance()->Remove(m_poGodWeapon);
		m_poGodWeapon = NULL;
		return FALSE;
	}
	SetOtherLife(m_poGodWeapon);

	return TRUE;
}


VOID CShadeMonster::InitBattleAttribute()
{

	memset(&m_stBattleAttribute, 0, sizeof(m_stBattleAttribute));
	m_stBattleAttribute.qwHP = m_stMonsterProp.qwHP;
	m_stBattleAttribute.dwAttack = m_stMonsterProp.dwAttack;
	m_stBattleAttribute.dwDefend = m_stMonsterProp.dwDefend;

	m_stHideBattleAttr.wHitRate = m_stMonsterProp.wHitRate;
	m_stHideBattleAttr.wDodgeRate = m_stMonsterProp.wDodgeRate;
	m_stHideBattleAttr.wCritRate = m_stMonsterProp.wCritRate;
	m_stHideBattleAttr.wDeCritRate = m_stMonsterProp.wDeCritRate;
	m_stHideBattleAttr.wAngryValue = m_stMonsterProp.wAngryValue;

	m_stCurBattleAttribute = m_stBattleAttribute;
}

BOOL CShadeMonster::InitSkill()
{
	//怪物可能没技能
	if(0 == m_stMonsterProp.wSkillActionID)
	{
		return TRUE;
	}

	SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(m_stMonsterProp.wSkillActionID, m_stMonsterProp.wSkillActionLevel);
	if(NULL == pstSkillActionProp)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: pstSkillActionProp[%d.%d] is NULL!"), MSG_MARK, m_stMonsterProp.wSkillActionID, m_stMonsterProp.wSkillActionLevel);
		return FALSE;
	}

	DT_SKILL_DATA stInitSKill;
	memset(&stInitSKill, 0, sizeof(stInitSKill));
	stInitSKill.wActionID = m_stMonsterProp.wSkillActionID;
	stInitSKill.wActionLevel = m_stMonsterProp.wSkillActionLevel;
	stInitSKill.wEffectID = m_stMonsterProp.wSkillEffectID;
	stInitSKill.stAttackRangInfo.byAttackRangKind = pstSkillActionProp->byUnlockAttackRangKind;
	stInitSKill.stAttackRangInfo.byTraceBuffKind = pstSkillActionProp->byUnlockTraceBuffKind;
	if(EARK_INVALID == stInitSKill.stAttackRangInfo.byAttackRangKind)
	{
		stInitSKill.stAttackRangInfo.byAttackRangKind = EARK_FRONTONE;
	}

	CSkill* poSKill = CSkillPoolMgr::Instance()->Create();
	if(NULL == poSKill)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CreateSkill failed!"), MSG_MARK);
		return FALSE;
	}
	if(FALSE == poSKill->Init(stInitSKill, NULL, FALSE))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Skill init failed!"), MSG_MARK);
		CSkillPoolMgr::Instance()->Remove(poSKill);
		poSKill = NULL;
		return FALSE;
	}
	m_apoSlotSKill[1] = poSKill;

	return TRUE;
}


SMonsterProp* CShadeMonster::GetMonsterProp()
{
	return &m_stMonsterProp;
}

UINT32	CShadeMonster::GetPower()
{
	return static_cast<UINT32>(m_stMonsterProp.qwHP + m_stMonsterProp.dwAttack * 5 + m_stMonsterProp.dwDefend * 3);
}

VOID CShadeMonster::SetKindID(UINT16 wKindID)
{
	m_wKindID = wKindID;
	m_stMonsterProp.wFakeID = wKindID;
	m_stMonsterProp.wKindID = wKindID;
}

VOID CShadeMonster::SetFormation(UINT8 byFormation)
{
	m_byFormationIdx = byFormation;
}

SMonsterUpgradeProp* CShadeMonster::GetMonsterUpgradeProp()
{
	SMonsterUpgradeProp* pstMonsterUpgradeProp = NULL;
	const SMonsterProp* pstMonsterProp = GetMonsterProp();
	if(pstMonsterProp)
	{
		pstMonsterUpgradeProp = CMonsterUpgradePropMgr::Instance()->GetMonsterUpgradeProp(pstMonsterProp->wLevel, pstMonsterProp->byType);
		if(NULL == pstMonsterUpgradeProp)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: pstMonsterUpgradeProp is NULL,Level[%d], Type[%d]!"), MSG_MARK, pstMonsterProp->wLevel, pstMonsterProp->byType);
		}
	}

	return pstMonsterUpgradeProp;
}

//获得神器战斗信息
VOID CShadeMonster::GetDT_GODWEAPON_BATTLE_DATA(DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo)
{
	if (m_poGodWeapon)
	{
		m_poGodWeapon->GetDT_GODWEAPON_BATTLE_DATA(stGodweaponInfo);
	}
	else
	{
		memset(&stGodweaponInfo, 0, sizeof(stGodweaponInfo));
	}
}

UINT16 CShadeMonster::GetFakeSkillActionID()
{
	const SMonsterProp* pstProp = GetMonsterProp();
	if(pstProp)
	{
		SMonsterProp* pstFakeProp = CMonsterPropMgr::Instance()->GetMonsterProp(pstProp->wFakeID);
		if(pstFakeProp)
		{
			return pstFakeProp->wSkillActionID;
		}
		else
		{
			return pstProp->wSkillActionID;
		}
	}

	return 0;
}
