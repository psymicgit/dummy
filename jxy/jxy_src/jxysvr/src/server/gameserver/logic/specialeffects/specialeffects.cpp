#include "logic/specialeffects/specialeffects.h"
#include "logic/specialeffects/specialeffectsmgr.h"
#include "sdloggerimpl.h"
#include "logic/battle/battleattrobj.h"
#include "logic/soul/soul.h"

IMPLEMENT_CLASS_POOL_MGR(CContinueAttackEffect)
IMPLEMENT_CLASS_POOL_MGR(CSuckBloodEffect)
IMPLEMENT_CLASS_POOL_MGR(CPhysicalImmunityEffect)
IMPLEMENT_CLASS_POOL_MGR(CMagicImmunityEffect)
IMPLEMENT_CLASS_POOL_MGR(CAddBloodEffect)
IMPLEMENT_CLASS_POOL_MGR(CReboundEffect)
IMPLEMENT_CLASS_POOL_MGR(CBeatBack)
IMPLEMENT_CLASS_POOL_MGR(CDodgeEffect)

BOOL CSpecialEffect::Init(CPlayer* poPlayer, CBattleAttrObj* poMySelf, const DT_SPECIAL_EFFECT_DATA& stData, UINT8 bySelectIdx)
{
	if ( NULL == poPlayer)
	{
		return FALSE;
	}
	m_poOwner = poPlayer;
	m_poMySelf = poMySelf;
	m_bySelectIdx = bySelectIdx;
	memcpy(&m_stSpecialEffectData, &stData, sizeof(DT_SPECIAL_EFFECT_DATA));
	m_wLimitNum = 0;
	m_wExtLimitNum = 0;
	m_wExtEffectPercent = 0;
	m_wExtTrigerProb = 0;
	return TRUE;
}

//检验是否能升级
BOOL CSpecialEffect::IsActivity()
{
	return m_stSpecialEffectData.byIsActivity;
}
//升级
BOOL CSpecialEffect::SetActivity()
{
	m_stSpecialEffectData.byIsActivity = ESM_YES;
	return TRUE;
}

VOID CSpecialEffect::ResetData()
{
	//作用次数
	m_wLimitNum = 0;
	m_wExtLimitNum = 0;
	m_wExtEffectPercent = 0;
	m_wExtTrigerProb = 0;

}
//增加作用次数
VOID CSpecialEffect::AddEffectNum()
{
	m_wLimitNum += 1;
}

UINT16 CSpecialEffect::GetEffectID()
{
	return m_stSpecialEffectData.wEffectID;
}

UINT8 CSpecialEffect::GetEffectBigType()
{
	const SSpecialEffectProp* poProp = GetProp();
	if ( NULL == poProp)
	{
		return 0;
	}

	return poProp->byEffectBigType;
}

UINT8 CSpecialEffect::GetEffectSmallType()
{
	const SSpecialEffectProp* poProp = GetProp();
	if ( NULL == poProp)
	{
		return 0;
	}
	
	return poProp->byEffectSmallType;
}

UINT16 CSpecialEffect::GetLeftEffectNum()
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return 0;
	}
	
	UINT16 wMaxNum = CSpecialEffectPropMgr::Instance()->GetLimitNum(m_stSpecialEffectData.wEffectID, m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx]) + m_wExtLimitNum;
	return wMaxNum < m_wLimitNum ? 0 : wMaxNum - m_wLimitNum;
}

UINT32 CSpecialEffect::GetTrigerProb()
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return 0;
	}
	return CSpecialEffectPropMgr::Instance()->GetTriggerProb(m_stSpecialEffectData.wEffectID, m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx]) + m_wExtTrigerProb;
}

UINT16 CSpecialEffect::GetEffectPercent()
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return 0;
	}
	return CSpecialEffectPropMgr::Instance()->GetEffectPercent(m_stSpecialEffectData.wEffectID, m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx]) + m_wExtEffectPercent;
}

const SSpecialEffectProp* CSpecialEffect::GetProp()
{
	return CSpecialEffectPropMgr::Instance()->GetSpecialEffectProp(m_stSpecialEffectData.wEffectID, m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx]);
}
VOID CContinueAttackEffect::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return ;
	}
	if ( NULL == poEnemy || NULL == m_poMySelf)
	{
		return ;
	}
	
	m_poMySelf->SetContinueAttack(this);
	if ( NULL != bySpecialEffectFlag)
	{
		*bySpecialEffectFlag = EBSE_NONE;
	}
	AddEffectNum();

}

VOID CSuckBloodEffect::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return ;
	}
	if ( NULL == poEnemy || NULL == m_poMySelf)
	{
		return ;
	}
	if ( NULL != poEnemyEffect)
	{
		if( EBSE_DODGE == poEnemyEffect->GetEffectSmallType() )
		{
			return ;
		}
	}
		
	//满血时不触发
	if ( m_poMySelf->GetCurDT_BATTLE_ATTRIBUTE().qwHP != m_poMySelf->GetDT_BATTLE_ATTRIBUTE().qwHP)
	{
		UINT64 dwAddValue = dwHurtValue * GetEffectPercent() / 100;
		m_poMySelf->AddHPChange((INT32)dwAddValue, ET_SPECIAL_EFFECT, EHPCR_SPECIAL_SUCK_BLOOD, poEnemy->GetFormationIdx());

		if ( NULL != bySpecialEffectFlag)
		{
			*bySpecialEffectFlag = EBSE_SUCK_BLOOD;
		}
		AddEffectNum();
	}
}

VOID CPhysicalImmunityEffect::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return ;
	}
	if ( NULL == poEnemy || NULL == m_poMySelf)
	{
		return ;
	}

	m_poMySelf->SetPhysicalImmunity(TRUE);
	DT_SHIELD_DATA stData = {0};
	stData.byAddDelFlag = 0;
	stData.byFormationIdx = m_poMySelf->GetFormationIdx();
	stData.bySheldID = EBSE_PHYSICAL_IMMUNITY;
	m_poMySelf->GetShiledChange().push_back(stData);
	if ( NULL != bySpecialEffectFlag)
	{
		*bySpecialEffectFlag = EBSE_PHYSICAL_IMMUNITY;
	}
	AddEffectNum();
}

VOID CMagicImmunityEffect::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return ;
	}
	if ( NULL == poEnemy || NULL == m_poMySelf)
	{
		return ;
	}

	m_poMySelf->SetMagicImmumity(TRUE);
	DT_SHIELD_DATA stData = {0};
	stData.byAddDelFlag = 0;
	stData.byFormationIdx = m_poMySelf->GetFormationIdx();
	stData.bySheldID = EBSE_MAGIC_IMMUNITY;
	m_poMySelf->GetShiledChange().push_back(stData);
	if ( NULL != bySpecialEffectFlag)
	{
		*bySpecialEffectFlag = EBSE_MAGIC_IMMUNITY;
	}
	AddEffectNum();
}

VOID CAddBloodEffect::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{

	if ( NULL == poEnemy || NULL == m_poMySelf)
	{
		return ;
	}

	m_poMySelf->AddHPChange(-(INT32)dwHurtValue, bIsSkill ? ET_SKILL : ET_NORMAL, EHPCR_NORMAL_SKILL, poEnemy->GetFormationIdx());
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity || !m_poMySelf->GetCanNormalAttack())
	{
		return ;
	}
	//满血时不触发
	if ( m_poMySelf->GetCurDT_BATTLE_ATTRIBUTE().qwHP != m_poMySelf->GetDT_BATTLE_ATTRIBUTE().qwHP)
	{
		if ( !m_poMySelf->IsDead())
		{
			UINT32 dwAddValue = static_cast<UINT32>(m_poMySelf->GetDT_BATTLE_ATTRIBUTE().qwHP * GetEffectPercent() / 100);
			m_poMySelf->AddHPChange((INT32)dwAddValue, ET_SPECIAL_EFFECT, EHPCR_SPECIAL_ADD_BLOOD, m_poMySelf->GetFormationIdx());
			if ( NULL != bySpecialEffectFlag)
			{
				*bySpecialEffectFlag = EBSE_ADD_BLOOD;
			}
		}
		else
		{
			if ( NULL != bySpecialEffectFlag)
			{
				*bySpecialEffectFlag = EBSE_NONE;
			}
		}
		AddEffectNum();
	}
}

VOID CReboundEffect::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{

	if ( NULL == poEnemy || NULL == m_poMySelf)
	{
		return ;
	}

	m_poMySelf->AddHPChange(-(INT32)dwHurtValue, bIsSkill ? ET_SKILL : ET_NORMAL, EHPCR_NORMAL_SKILL, poEnemy->GetFormationIdx());

	if ( ESM_NO == m_stSpecialEffectData.byIsActivity || !m_poMySelf->GetCanNormalAttack())
	{
		return ;
	}
	if( !m_poMySelf->IsDead() )
	{
		//反弹伤害
		UINT32 dwReboundValue = static_cast<UINT32>(m_poMySelf->GetDT_BATTLE_ATTRIBUTE().qwHP * GetEffectPercent() / 100);
		poEnemy->AddHPChange(-(INT32)dwReboundValue, ET_SPECIAL_EFFECT, EHPCR_SPECIAL_REBOUND, m_poMySelf->GetFormationIdx());
		if ( NULL != bySpecialEffectFlag)
		{
			*bySpecialEffectFlag = EBSE_REBOUND;
		}
		AddEffectNum();
	}
}

VOID CBeatBack::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{
	
	if ( NULL == poEnemy || NULL == m_poMySelf)
	{
		return ;
	}

	m_poMySelf->AddHPChange(-(INT32)dwHurtValue, bIsSkill ? ET_SKILL : ET_NORMAL, EHPCR_NORMAL_SKILL, poEnemy->GetFormationIdx());
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity || !m_poMySelf->GetCanNormalAttack())
	{
		return ;
	}
	if( !m_poMySelf->IsDead())
	{
		m_poMySelf->SetBeatBack(this);
		if ( NULL != bySpecialEffectFlag)
		{
			*bySpecialEffectFlag = EBSE_BEAT_BACK;
		}
		AddEffectNum();
	}
	
}

VOID CDodgeEffect::Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag)
{
	if ( ESM_NO == m_stSpecialEffectData.byIsActivity )
	{
		return ;
	}
	if ( NULL == poEnemy || NULL == m_poMySelf || !m_poMySelf->GetCanNormalAttack())
	{
		return ;
	}
	if ( NULL != bySpecialEffectFlag)
	{
		*bySpecialEffectFlag = EBSE_DODGE;
	}
	m_poMySelf->GetDodge() = 1;
	AddEffectNum();
}

BOOL CSpecialEffect::GetSpecialEffectDataCli(DT_SPECIAL_EFFECT_DESC& stCli)
{
	stCli.byEffectType = m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx];

	stCli.wLimitNumValue = CSpecialEffectPropMgr::Instance()->GetLimitNum(m_stSpecialEffectData.wEffectID, m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx]);

	stCli.wTriggerProbValue = CSpecialEffectPropMgr::Instance()->GetTriggerProb(m_stSpecialEffectData.wEffectID, m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx]);

	stCli.wEffectPercentValue = CSpecialEffectPropMgr::Instance()->GetEffectPercent(m_stSpecialEffectData.wEffectID, m_stSpecialEffectData.abyEffectSmallType[m_bySelectIdx]);

	return TRUE;
}

VOID CSpecialEffect::SetOwner(CBattleAttrObj* poObj)
{
	m_poMySelf = poObj;
}

VOID CSpecialEffect::SetExtLimit(UINT16 wValue)
{
	m_wExtLimitNum = wValue;
}

VOID CSpecialEffect::SetExtEffectPercent(UINT16 wValue)
{
	m_wExtEffectPercent = wValue;
}

VOID CSpecialEffect::SetExtTrigerProb(UINT16 wValue)
{
	m_wExtTrigerProb = wValue;
}