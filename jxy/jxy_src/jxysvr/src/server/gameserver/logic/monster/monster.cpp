#include "monster.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/skill/skillmgr.h>
#include <logic/skill/skillpropmgr.h>
#include "framework/gsapi.h"
#include "logic/other/singleprammgr.h"
#include "logic/activity/bossbattlemgr.h"
using namespace SGDP;

CMonster::CMonster()
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

CMonster::~CMonster()
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

BOOL CMonster::Init(UINT16 wKindID, UINT8 byFormationIdx, BOOL bBossBMonster, UINT16 wLevel)
{
    m_wKindID = wKindID;
    m_wLevel = wLevel;
    m_bBossBMonster = bBossBMonster;
    SMonsterProp* pstMonsterProp = NULL;
    if(m_bBossBMonster)
    {
        pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(wKindID, wLevel);
    }
    else
    {
        pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(wKindID);
    }

    if(NULL == pstMonsterProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstMonsterProp is NULL,KindID[%d] Level[%d]!"), MSG_MARK, wKindID, m_wLevel);
        return FALSE;
    }

    SMonsterUpgradeProp* pstMonsterUpgradeProp = CMonsterUpgradePropMgr::Instance()->GetMonsterUpgradeProp(pstMonsterProp->wLevel, pstMonsterProp->byType);
    if(NULL == pstMonsterUpgradeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstMonsterUpgradeProp is NULL,Level[%d], Type[%d]!"), MSG_MARK, pstMonsterProp->wLevel, pstMonsterProp->byType);
        return FALSE;
    }

    if(!InitSkill())
    {
        return FALSE;
    }

    if(!InitGodweapon())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstMonsterUpgradeProp is NULL,Level[%d], Type[%d]!"), MSG_MARK, pstMonsterProp->wLevel, pstMonsterProp->byType);
        return FALSE;
    }

    m_byFormationIdx = byFormationIdx;

    //赋值战斗属性
    InitBattleAttribute();

    return TRUE;
}

BOOL CMonster::InitGodweapon()
{
    SMonsterProp* pstMonsterProp = GetMonsterProp();
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


VOID CMonster::InitBattleAttribute()
{
    SMonsterProp* pstMonsterProp = NULL;
    if(m_bBossBMonster)
    {
        pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(m_wKindID, m_wLevel);
    }
    else
    {
        pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(m_wKindID);
    }
    if(NULL == pstMonsterProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstMonsterProp is NULL,KindID[%d] Level[%d]!"), MSG_MARK, m_wKindID, m_wLevel);
        return;
    }

    memset(&m_stBattleAttribute, 0, sizeof(m_stBattleAttribute));
    m_stBattleAttribute.qwHP += pstMonsterProp->qwHP;
	if ( m_bBossBMonster )
	{
		 UINT16 wZoneID = gsapi::GetZoneID();
		 if ( CBossBattleMgr::Instance()->IsCloseArea( wZoneID ))
		 {
			 m_stBattleAttribute.qwHP *= CSinglePramMgr::Instance()->GetCloseAreaBoosMulti();
		 }
	}
    m_stBattleAttribute.dwAttack += pstMonsterProp->dwAttack;
	m_stBattleAttribute.dwDefend += pstMonsterProp->dwDefend;

    m_stHideBattleAttr.wHitRate = pstMonsterProp->wHitRate;
    m_stHideBattleAttr.wDodgeRate = pstMonsterProp->wDodgeRate;
    m_stHideBattleAttr.wCritRate = pstMonsterProp->wCritRate;
    m_stHideBattleAttr.wDeCritRate = pstMonsterProp->wDeCritRate;
    m_stHideBattleAttr.wAngryValue = pstMonsterProp->wAngryValue;

    m_stCurBattleAttribute = m_stBattleAttribute;
}

BOOL CMonster::InitSkill()
{
    SMonsterProp* pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(m_wKindID);
    if(NULL == pstMonsterProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstShadeMonsterProp is NULL,KindID[%d]!"), MSG_MARK, m_wKindID);
        return FALSE;
    }
    //怪物可能没技能
    if(0 == pstMonsterProp->wSkillActionID)
    {
        return TRUE;
    }

    SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(pstMonsterProp->wSkillActionID, pstMonsterProp->wSkillActionLevel);
    if(NULL == pstSkillActionProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstSkillActionProp[%d.%d] is NULL!"), MSG_MARK, pstMonsterProp->wSkillActionID, pstMonsterProp->wSkillActionLevel);
        return FALSE;
    }

    DT_SKILL_DATA stInitSKill;
    memset(&stInitSKill, 0, sizeof(stInitSKill));
    //stInitSKill.wID = pstMonsterProp->wSkillID;
    //stInitSKill.wLevel = pstMonsterProp->wSkillLevel;
    stInitSKill.wActionID = pstMonsterProp->wSkillActionID;
    stInitSKill.wActionLevel = pstMonsterProp->wSkillActionLevel;
    stInitSKill.wEffectID = pstMonsterProp->wSkillEffectID;
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


SMonsterProp* CMonster::GetMonsterProp()
{
    SMonsterProp* pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(m_wKindID);
    if(NULL == pstMonsterProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstGoodsProp is NULL,KindID[%d]!"), MSG_MARK, m_wKindID);
    }

    return pstMonsterProp;
}

UINT32	CMonster::GetPower()
{
    SMonsterProp *pstMonsterProp = NULL;
    if(m_bBossBMonster)
    {
        pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(m_wKindID, m_wLevel);
    }
    else
    {
        pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(m_wKindID);
    }

    if(NULL == pstMonsterProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstMonsterProp is NULL,KindID[%d] Level[%d]!"), MSG_MARK, m_wKindID, m_wLevel);
        return 0;
    }
    return static_cast<UINT32>(pstMonsterProp->qwHP + pstMonsterProp->dwAttack * 5 + pstMonsterProp->dwDefend * 3);
}


SMonsterUpgradeProp* CMonster::GetMonsterUpgradeProp()
{
    SMonsterUpgradeProp* pstMonsterUpgradeProp = NULL;
    SMonsterProp* pstMonsterProp = GetMonsterProp();
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
VOID CMonster::GetDT_GODWEAPON_BATTLE_DATA(DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo)
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

UINT16 CMonster::GetFakeSkillActionID()
{
    SMonsterProp* pstProp = GetMonsterProp();
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

BOOL CMonster::InitBattleAttribute(UINT16 wPlayerLevel)
{
    return TRUE;
}

UINT8 CMonster::GetMonsterType()
{
    SMonsterProp* pMonsterProp = GetMonsterProp();
    if ( NULL == pMonsterProp)
    {
        return 0;
    }
    return pMonsterProp->byType;
}