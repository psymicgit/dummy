#include "battleattrobj.h"
#include <logic/skill/skillmgr.h>
#include <common/server/utility.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/battle/battleobj.h>
#include <logic/monster/monster.h>
#include <logic/skill/skillpropmgr.h>

using namespace SGDP;



#include "battleobj.h"


#include <db/autosqlbuf.h>
#include <net/gt/gtpktbuilder.h>
#include <sdutil.h>
#include <errdef.h>
#include <common/server/utility.h>
#include <logic/skill/skillmgr.h>
#include <logic/base/logicfunc.h>
#include <logic/other/ratemgr.h>
#include "logic/specialeffects/specialeffects.h"
#include "logic/specialeffects/specialeffectsmgr.h"
CBattleAttrObj::CBattleAttrObj()
{
    memset(&m_stBattleAttribute, 0, sizeof(DT_BATTLE_ATTRIBUTE));
    memset(&m_stCurBattleAttribute, 0, sizeof(DT_BATTLE_ATTRIBUTE));
    memset(&m_stHideBattleAttr, 0, sizeof(m_stHideBattleAttr));
    memset(&m_stExtBattleAttr, 0, sizeof(m_stExtBattleAttr));

    //m_mapBuffState.clear();
    //m_mapID2BuffMap.clear();
    m_mapKindID2Buff.clear();

    for(UINT8 byIdx = 0; byIdx < MAX_SKILL_SLOT_NUM + 1; byIdx++)
    {
        m_apoSlotSKill[byIdx] = NULL;
    }
    m_ppoFormationSkill = &m_apoSlotSKill[1];//先使用第1个技能槽的技能，后续扩展再修改

    m_wCurAngry = 0;
    m_bCanGetData = TRUE;

    InitBattleMidParam();

    m_poOtherLife = NULL;

    m_bMonster = FALSE;
	m_bPhysicalImmunity = FALSE;
	m_bMagicImmunity = FALSE;
	m_mapSpecialEffect.clear();
	m_bContinueAttack = NULL;
	m_bBeatBack = NULL;
}

CBattleAttrObj::~CBattleAttrObj()
{
    //释放buff
    ReleaseBuff();

    //释放技能
    for(UINT8 byIdx = 0; byIdx < MAX_SKILL_SLOT_NUM + 1; byIdx++)
    {
        if(NULL == m_apoSlotSKill[byIdx])
        {
            continue;
        }
        CSkillMgr::Instance()->RemoveSkill(m_apoSlotSKill[byIdx]);
    }
	m_mapSpecialEffect.clear();
	m_bContinueAttack = NULL;
	m_bBeatBack = NULL;
}


//释放战斗过程中的buff
VOID CBattleAttrObj::ReleaseBuff()
{
    for(CBuffID2BuffMapItr itrBuff  = m_mapKindID2Buff.begin(); itrBuff != m_mapKindID2Buff.end(); itrBuff++)
    {
        CBuffMgr::Instance()->RemoveBuff(itrBuff->second->GetMemID());
    }
    m_mapKindID2Buff.clear();
}


//添加buff
BOOL CBattleAttrObj::AddBuff(CBuff* poBuff)
{
    if(NULL == poBuff)
    {
        return FALSE;
    }

    CBuffID2BuffMapItr itr = m_mapKindID2Buff.find(poBuff->GetKindID());
    //该buff已经存在，使用新buff参数替换旧buff
    if(itr != m_mapKindID2Buff.end())
    {
        CBuffMgr::Instance()->RemoveBuff(itr->second->GetMemID());
        m_mapKindID2Buff.erase(itr);
    }
    else
    {
        DT_BUFF_DATA stDT_BUFF_DATA;
        stDT_BUFF_DATA.byFormationIdx = GetFormationIdx();
        stDT_BUFF_DATA.wAddDelFlag = 0;//新增
        stDT_BUFF_DATA.byBuffID = poBuff->GetKindID();
        m_vecBuffChange.push_back(stDT_BUFF_DATA);
    }

    m_mapKindID2Buff[poBuff->GetKindID()] = poBuff;

    return TRUE;
}

BOOL CBattleAttrObj::AddSkillEffectID(UINT16 wSkillEffectID)//添加技能效果ID
{
    DT_SKILL_EFFECT_ID_DATA stDT_SKILL_EFFECT_ID_DATA;
    stDT_SKILL_EFFECT_ID_DATA.byFormationIdx = GetFormationIdx();
    stDT_SKILL_EFFECT_ID_DATA.wEffectID = wSkillEffectID;
    m_vecfSkillEffectIDChange.push_back(stDT_SKILL_EFFECT_ID_DATA);
    return TRUE;
}

VOID CBattleAttrObj::ResetBattleRadom()
{
    for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        avecCritRadom[byIdx].clear();
        abyLastCritRadomIdx[byIdx] = 0;
        abyLastInitCritRate[byIdx] = 0;
        avecParryRadom[byIdx].clear();
        abyLastParryRadomIdx[byIdx] = 0;
        abyLastParryHitRate[byIdx] = 0;
    }
}


VOID CBattleAttrObj::BuffRoundDec(BOOL bEndBigRound)
{
    for(CBuffID2BuffMapItr itr = m_mapKindID2Buff.begin(); itr != m_mapKindID2Buff.end(); )
    {
        CBuff* poBuff = itr->second;
        if(NULL == poBuff)
        {
            continue;
        }
        poBuff->EndRound(bEndBigRound);

        //持续回合数已过，清除
        if(0 == poBuff->GetRoundNum())
        {
            DT_BUFF_DATA stDT_BUFF_DATA;
            stDT_BUFF_DATA.byFormationIdx = GetFormationIdx();
            stDT_BUFF_DATA.wAddDelFlag = 1;//删除
            stDT_BUFF_DATA.byBuffID = poBuff->GetKindID();
            m_vecBuffChange.push_back(stDT_BUFF_DATA);

            m_mapKindID2Buff.erase(itr++);
            CBuffMgr::Instance()->RemoveBuff(poBuff->GetMemID());
        }
        else
        {
            itr++;
        }
    }
}

/*
//添加buff状态
VOID CBattleAttrObj::AddBuffState(UINT16 wSkillID, EBuffKind eBuffKind)
{
	DT_BUFF_DATA stDT_BUFF_DATA;
	stDT_BUFF_DATA.wSkillID = wSkillID;
	stDT_BUFF_DATA.byBuffID = eBuffKind;
	m_mapBuffState[CLogicFunc::Get1616Key(wSkillID, eBuffKind)] = stDT_BUFF_DATA;
}

VOID CBattleAttrObj::GetBuffState(UINT8& byBuffNum, DT_BUFF_DATA astBuffState[MAX_BUFF_NUM])
{
	byBuffNum = 0;
	UINT8 bySize = m_mapBuffState.size();
	for(CBuffDataMapItr itr = m_mapBuffState.begin(); ((itr != m_mapBuffState.end()) && (byBuffNum < MAX_BUFF_NUM)); itr++, byBuffNum++)
	{
		astBuffState[byBuffNum].wSkillID = itr->second.wSkillID;
		astBuffState[byBuffNum].byBuffID = itr->second.byBuffID;
	}
}
*/


//获得当前的攻击力
UINT32 CBattleAttrObj::GetCurAttack(CBattleAttrObj* poTarget, UINT16 wAttackScale )
{
    //0906攻击＊增伤－防御
    UINT32  dwAttack = m_stCurBattleAttribute.dwAttack * wAttackScale / 100;
    UINT16 wHurtRate = m_wAttackHurtRate + m_stExtBattleAttr.wIncHurtRate;
    dwAttack = (UINT64)dwAttack * wHurtRate  / 100 ;
    //神器HP算成直接的伤害防御
    if( poTarget )
    {
        UINT32 dwDefense = poTarget->GetCurDT_BATTLE_ATTRIBUTE().dwDefend;
		dwDefense = dwDefense * GetAddDefendRate() / 100;
        dwAttack = dwAttack > dwDefense ? dwAttack - dwDefense : 0;
    }

    return dwAttack ;
}



VOID CBattleAttrObj::CKParryHurt(CBattleAttrObj* poTarget)
{
    UINT8 byIdx = poTarget->GetFormationIdx();
    if(byIdx > MAX_FORMATION_IDX_NUM)
    {
        byIdx = 1;
    }
    if(TRUE == poTarget->GetCanDodge())
    {
        INT16 shDodgeRate = poTarget->m_stHideBattleAttr.wDodgeRate + poTarget->m_wAddDodgeRate;
        INT16 shParryRate = (shDodgeRate - (INT16)m_stHideBattleAttr.wHitRate) / 10; //千分比
        shParryRate = shParryRate > PARRY_UPPERLIMIT ? PARRY_UPPERLIMIT : shParryRate;
        shParryRate = shParryRate < PARRY_LOWERLIMIT ? PARRY_LOWERLIMIT : shParryRate;

        if((abyLastParryRadomIdx[byIdx] >= avecParryRadom[byIdx].size()) || ((UINT8)shParryRate != abyLastParryHitRate[byIdx]))
        {
            avecParryRadom[byIdx] = CRateMgr::Instance()->GetRateVec((UINT16)shParryRate);
            abyLastParryRadomIdx[byIdx] = 0;
            abyLastParryHitRate[byIdx] = (UINT8)shParryRate;
        }
        //格挡
        if(1 == avecParryRadom[byIdx][abyLastParryRadomIdx[byIdx]++])
        {
            poTarget->GetParryFlag() = 1;
        }
    }

    UINT16 wCritRate = m_stHideBattleAttr.wCritRate + m_wAddCritRate;
    INT16 shCritRate = (INT16)(wCritRate - poTarget->m_stHideBattleAttr.wDeCritRate) / 10;	 //千分比
    shCritRate = shCritRate > CRITRATE_UPPERLIMIT ? CRITRATE_UPPERLIMIT : shCritRate;
    shCritRate = shCritRate < CRITRATE_LOWERLIMIT ? CRITRATE_LOWERLIMIT : shCritRate;
    //
    m_byCritFlag = 0;
    if(( shCritRate > 0 ) && (m_bCanCrit))
    {
        if((abyLastCritRadomIdx[byIdx] >= avecCritRadom[byIdx].size()) || (shCritRate != abyLastInitCritRate[byIdx]))
        {
            avecCritRadom[byIdx] = CRateMgr::Instance()->GetRateVec(shCritRate);
            abyLastCritRadomIdx[byIdx] = 0;
            abyLastInitCritRate[byIdx] = (UINT8)shCritRate;
        }
        if(1 == avecCritRadom[byIdx][abyLastCritRadomIdx[byIdx]++])
        {
            //m_wAttackHurtRate = m_wAttackHurtRate * NORMAL_CRIT_RATE / 100;
            //SYS_CRITICAL(_SDT("[%s: %d]: m_wAttackHurtRate:%d"), MSG_MARK, m_wAttackHurtRate);

            //暴击
            m_byCritFlag = 1;
        }
    }
}


VOID CBattleAttrObj::InitBattleMidParam()
{
    m_bCanNormalAttack = TRUE;
    m_bCanSkillAttack = TRUE;
    m_bCanDodge = TRUE;
	m_byChaosState = EBK_NONE;
	m_bNotAnger = FALSE;

    m_wSkillActionID = 0;
    //m_wSkillLevel = 0;

    m_wAddDodgeRate = 0;
    m_wAddCritRate = 0;

    m_wAttackHurtRate = 100;
    m_dwExtHur = 0;
	m_wFinalDecHurt = 100;
	m_wFinalAddHurt = 100;
    m_wSkillHurtRate = 100;
    m_dwSkillExtHur = 0;

    m_bCanCrit = TRUE;
    m_byCritFlag = 0;
    m_byParryFlag = 0;
	m_byDodgeFlag = 0;

    m_dwDefenseHP = 0;
    m_wDecHurtRate = 0;
	m_wAddDefendRate = 100;

    m_vecfSkillEffectIDChange.clear();
}

VOID CBattleAttrObj::InitAttack()
{
    m_wAttackHurtRate = 100;
    m_dwExtHur = 0;
}

//计算回合结束，怒气结算，清除该回合的各种效果，持续多回合的由buff解决
VOID CBattleAttrObj::OnRoundEnd(CBattleAttrObj* poAttacker, CBattleAttrObj* poBettacker)
{
    //if(((this == poAttacker) || (this == poBettacker)) && (0 == m_wSkillID)) //攻击者和被攻击者而且普通攻击才加怒气
    if((this == poAttacker) && (0 == m_wSkillActionID) && (m_bCanNormalAttack) && (!m_bContinueAttack) && (!m_bBeatBack) && !m_bNotAnger)//攻击且普通攻击才加怒气
    {
        AddAngryChange(ROUND_ANGRY);
    }

    InitBattleMidParam();
}



CSkill* CBattleAttrObj::TrigerSkill()
{
    //被沉默
    if(FALSE == m_bCanSkillAttack)
    {
        return NULL;
    }

    if(*m_ppoFormationSkill)
    {
        SSkillActionProp* pstProp = (*m_ppoFormationSkill)->GetSkillActionProp();
        if(m_wCurAngry >= FULL_ANGRY)
        {
            //m_wAttackHurtRate = m_wAttackHurtRate * m_wCurAngry / FULL_ANGRY;
            INT16 shAngry = (INT16)m_wCurAngry - (INT16)m_stExtBattleAttr.wOddAngry;
            if(shAngry > 0)
            {
                AddAngryChange(-shAngry);
            }
            else //异常，配置留下的怒气过多
            {
                //nothing
            }

            m_wSkillActionID = pstProp->wID;
            if(m_bMonster)//适配客户端技能ID
            {
                CMonster* poMonster = dynamic_cast<CMonster*>(this);
                if(poMonster)
                {
                    m_wSkillActionID = poMonster->GetFakeSkillActionID();
                }
            }
            //m_wSkillLevel = pstProp->wSkillLevel;
            //m_bCanCrit = FALSE;//使用技能不可暴击,

            return *m_ppoFormationSkill;
        }
    }

    return NULL;
}

VOID CBattleAttrObj::InitCurBattleAttr(DT_BATTLE_OBJ_HP *pstInitHP)
{
    InitBattleMidParam();
    m_wCurAngry = m_stHideBattleAttr.wAngryValue;
    m_stCurBattleAttribute = m_stBattleAttribute;
    
    m_bCanGetData = TRUE;

    m_vecAngryChange.clear();
    m_vecBuffChange.clear();
    m_vecHPChange.clear();
	m_vecHPChange2.clear();
	
    if (pstInitHP)
    {
        m_stCurBattleAttribute.qwHP = pstInitHP->dwHP;

        //已死，下次不可再取它的数据
        if(IsDead())
        {
            m_bCanGetData = FALSE;
            return;
        }
    }

    if(m_poOtherLife)
    {
        m_poOtherLife->InitCurBattleAttr();
    }
}

void CBattleAttrObj::BeStrong(UINT16 wStrongPercent)
{
    if (100 == wStrongPercent)
    {
        return;
    }

    double fPercent = (double)wStrongPercent / 100;

    m_stCurBattleAttribute.dwAttack = (UINT32)(m_stCurBattleAttribute.dwAttack * fPercent);
    m_stCurBattleAttribute.dwDefend = (UINT32)(m_stCurBattleAttribute.dwDefend * fPercent);

    m_stHideBattleAttr.wHitRate    = (UINT16)(m_stHideBattleAttr.wHitRate * fPercent);
    m_stHideBattleAttr.wDodgeRate  = (UINT16)(m_stHideBattleAttr.wDodgeRate * fPercent);
    m_stHideBattleAttr.wCritRate   = (UINT16)(m_stHideBattleAttr.wCritRate * fPercent);
    m_stHideBattleAttr.wDeCritRate = (UINT16)(m_stHideBattleAttr.wDeCritRate * fPercent);
}

VOID CBattleAttrObj::AddHPChange(INT32 nHP, EATTACK_TYPE eAttackType, UINT8 byReason, UINT8 byEmenyIdx,BOOL bAddToVec)
{
    if(0 == nHP)
    {
        return;
    }

	if ( nHP < 0 )
	{
		if ( GetPhysicalImmunity() )
		{
			//物免
			if ( ET_NORMAL == eAttackType )
			{
				SetPhysicalImmunity(FALSE);
				DT_SHIELD_DATA stData = {0};
				stData.byAddDelFlag = 1;
				stData.byFormationIdx = GetFormationIdx();
				stData.bySheldID = EBSE_PHYSICAL_IMMUNITY;
				GetShiledChange().push_back(stData);
				return ;
			}
		}
		
		if ( GetMagicImmumity() )
		{
			//魔免
			if ( ET_SKILL == eAttackType )
			{
				SetMagicImmumity(FALSE);
				DT_SHIELD_DATA stData = {0};
				stData.byAddDelFlag = 1;
				stData.byFormationIdx = GetFormationIdx();
				stData.bySheldID = EBSE_MAGIC_IMMUNITY;
				GetShiledChange().push_back(stData);
				return ;
			}
		}
	}

    DT_HP_DATA stDT_HP_DATA;
	DT_HP_DATA2 stHP2;
    stDT_HP_DATA.byFormationIdx = GetFormationIdx();
    stDT_HP_DATA.nHP = nHP;
	stHP2.byFormationIdx = GetFormationIdx();
	stHP2.nHP = nHP;
	stHP2.byEnemyIdx = byEmenyIdx;
	stHP2.byReason = byReason;

    if(bAddToVec)
    {
        m_vecHPChange.push_back(stDT_HP_DATA);
		m_vecHPChange2.push_back(stHP2);
    }

	UINT32 dwHp = 0;

	if ( nHP < 0)
	{
		dwHp = -nHP;
		if ( m_stCurBattleAttribute.qwHP < dwHp)
		{
			 m_stCurBattleAttribute.qwHP = 0;
		}
		else
		{
			m_stCurBattleAttribute.qwHP -= dwHp;
		}
	}
	else
	{
		
		m_stCurBattleAttribute.qwHP += nHP;
	}

    m_stCurBattleAttribute.qwHP = m_stCurBattleAttribute.qwHP > m_stBattleAttribute.qwHP ? m_stBattleAttribute.qwHP : m_stCurBattleAttribute.qwHP;
}

VOID CBattleAttrObj::AddAngryChange(INT16 shAngry)
{
    if(0 == shAngry)
    {
        return;
    }
    DT_ANGRY_DATA stDT_ANGRY_DATA;
    stDT_ANGRY_DATA.byFormationIdx = GetFormationIdx();
    stDT_ANGRY_DATA.shAngry = shAngry;
    m_vecAngryChange.push_back(stDT_ANGRY_DATA);

    INT16 shAngryTmp = (INT16)m_wCurAngry + shAngry;
    if(shAngryTmp < 0)
    {
        m_wCurAngry = 0;
    }
    else
    {
        m_wCurAngry = (UINT16)shAngryTmp;
    }
}


UINT16 CBattleAttrObj::GetAttackRankHurtRate()
{
    if(m_apoSlotSKill[1])
    {
        return m_apoSlotSKill[1]->GetAttackRangHurtRate();
    }

    return 100;
}

VOID CBattleAttrObj::SetPhysicalImmunity(BOOL bState)
{
	m_bPhysicalImmunity = bState;
}

VOID CBattleAttrObj::SetMagicImmumity(BOOL bState)
{
	m_bMagicImmunity = bState;
}

BOOL CBattleAttrObj::GetPhysicalImmunity()
{
	return m_bPhysicalImmunity;
}

BOOL CBattleAttrObj::GetMagicImmumity()
{
	return m_bMagicImmunity;
}

VOID CBattleAttrObj::ResetSpecialEffects()
{
	m_bPhysicalImmunity = FALSE;
	m_bMagicImmunity = FALSE;
	m_bContinueAttack = NULL;
	m_bBeatBack = NULL;
	m_mapSpecialEffect.clear();

	for ( CMapID2SpecialEffectItr itr = m_mapSpecialEffect.begin(); itr != m_mapSpecialEffect.end(); itr++)
	{
		itr->second->ResetData();
	}
}

VOID CBattleAttrObj::ReleaseSpecialEffect()
{
	for ( CMapID2SpecialEffectItr itr = m_mapSpecialEffect.begin(); itr != m_mapSpecialEffect.end(); itr++)
	{
		CSpecialEffectPropMgr::Instance()->FreeSpecialEffect(itr->second->GetEffectSmallType(), itr->second);
	}
	m_mapSpecialEffect.clear();
}

//添加指定特殊
BOOL CBattleAttrObj::AddSpecialEffects(CSpecialEffect* poEffect)
{
	if ( NULL == poEffect)
	{
		return FALSE;
	}
	CMapID2SpecialEffectItr itr = m_mapSpecialEffect.find(CLogicFunc::Get1616Key(poEffect->GetEffectID(), poEffect->GetEffectSmallType()));
	if ( itr != m_mapSpecialEffect.end() )
	{
		return FALSE;
	}
	m_mapSpecialEffect.insert(make_pair(CLogicFunc::Get1616Key(poEffect->GetEffectID(), poEffect->GetEffectSmallType() ), poEffect));
	return TRUE;
}

//删除指定特效
 CSpecialEffect* CBattleAttrObj::DelSpecialEffects(UINT8 bySpecialEffectID)
{
	CMapID2SpecialEffectItr itr = m_mapSpecialEffect.find(bySpecialEffectID);
	if ( itr == m_mapSpecialEffect.end() )
	{
		return NULL;
	}
	CSpecialEffect* poSpecialEffect = itr->second;
	m_mapSpecialEffect.erase(bySpecialEffectID);
	return poSpecialEffect;
}

CSpecialEffect* CBattleAttrObj::GetSpecialEffectsByAttack(BOOL bIsSkill)
{
	CRandomMap mapEffect;
	UINT32 dwSize = m_mapSpecialEffect.size();
	UINT32 dwRandomValue = 1000;
	for ( CMapID2SpecialEffectItr itr = m_mapSpecialEffect.begin(); itr != m_mapSpecialEffect.end(); itr++)
	{
		if ( GetMagicImmumity() )
		{
			if( EBSE_MAGIC_IMMUNITY == itr->second->GetEffectSmallType() )
			{
				continue;
			}
		}
		if ( GetPhysicalImmunity() )
		{
			if ( EBSE_PHYSICAL_IMMUNITY == itr->second->GetEffectSmallType())
			{
				continue;
			}
		}
		//攻击型特效
		if ( EAT_ATTACK == itr->second->GetAttackType())
		{
			if ( bIsSkill )
			{
				//连击与吸血不会在技能攻击时触发
				if ( !(itr->second->GetEffectSmallType() == EBSE_CONTINUE_ATTACK || itr->second->GetEffectSmallType() == EBSE_SUCK_BLOOD) && (0 != itr->second->GetLeftEffectNum()) )
				{
					mapEffect[CLogicFunc::Get1616Key(itr->second->GetEffectID(), itr->second->GetEffectSmallType())] = dwRandomValue / dwSize;
				}
			}
			else
			{
				if ( 0 != itr->second->GetLeftEffectNum())
				{
					mapEffect[CLogicFunc::Get1616Key(itr->second->GetEffectID(), itr->second->GetEffectSmallType())] = dwRandomValue / dwSize;
				}
			}
		}
	}
	if ( 0 == mapEffect.size())
	{
		return NULL;
	}
	UINT32 dwEffectID = GetRandomIdxMustHit(mapEffect);
	CSpecialEffect* poEffect = m_mapSpecialEffect[dwEffectID];
	if ( NULL == poEffect)
	{
		return NULL;
	}
	dwRandomValue = RandomReseed(1000) % 100;
	if( poEffect->GetTrigerProb() < dwRandomValue)
	{
		return NULL;
	}
	return poEffect;
}

CSpecialEffect* CBattleAttrObj::GetSpecialEffectsByDefend(BOOL bIsSkill)
{
	//不能攻击时不触发防守特效
	if ( !m_bCanNormalAttack && !m_bCanNormalAttack)
	{
		return NULL;
	}
	CRandomMap mapEffect;
	UINT32 dwSize = m_mapSpecialEffect.size();
	UINT32 dwRandomValue = 1000;
	for ( CMapID2SpecialEffectItr itr = m_mapSpecialEffect.begin(); itr != m_mapSpecialEffect.end(); itr++)
	{
		//攻击型特效
		if ( (EAT_DEFEND == itr->second->GetAttackType()) && (0 != itr->second->GetLeftEffectNum()))
		{
			mapEffect[CLogicFunc::Get1616Key(itr->second->GetEffectID(), itr->second->GetEffectSmallType())] = dwRandomValue / dwSize;
		}
	}
	if ( 0 == mapEffect.size())
	{
		return NULL;
	}
	UINT32 dwEffectID = GetRandomIdxMustHit(mapEffect);
	CSpecialEffect* poEffect = m_mapSpecialEffect[dwEffectID];
	if ( NULL == poEffect)
	{
		return NULL;
	}
	dwRandomValue = RandomReseed(1000) % 100;
	if( poEffect->GetTrigerProb() < dwRandomValue)
	{
		return NULL;
	}
	return poEffect;
}

//设置连击状态
VOID CBattleAttrObj::SetContinueAttack(CSpecialEffect* poEffect)
{
	m_bContinueAttack = poEffect;
}

//获取连击状态
CSpecialEffect* CBattleAttrObj::GetContinueAttack()
{
	return m_bContinueAttack;
}

//设置反击状态
VOID CBattleAttrObj::SetBeatBack( CSpecialEffect* poEffect)
{
	m_bBeatBack = poEffect;
}

//获取反击状态
CSpecialEffect* CBattleAttrObj::GetBeatBack()
{
	return m_bBeatBack;
}