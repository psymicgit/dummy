
#include "skill.h"
#include "buff.h"
#include "buffaffecthandler.h"
#include <net/gt/gtpktbuilder.h>
#include <sdutil.h>
#include <errdef.h>
#include <logic/battle/battleobj.h>
#include <logic/battle/battleattrobj.h>
#include <common/server/utility.h>
#include <sdrandom.h>
#include <logic/record/recordmgr.h>
#include <logic/base/logicfunc.h>
#include <logic/task/taskmgr.h>
#include <logic/player/player.h>
#include "skillaffecthandler.h"
#include "skillaffect.h"
#include "skillpropmgr.h"
#include <sdloggerimpl.h>
#include "logic/jingjie/jingjie.h"
#include "logic/pet/pet.h"
static SSkillEffectProp	stDefaultSkillEffectProp;

IMPLEMENT_CLASS_POOL_MGR(CSkill);

CSkill::CSkill()
{
    //m_poSkillOwner = NULL;
    memset(&m_stDT_SKILL_DATA, 0, sizeof(m_stDT_SKILL_DATA));
}

CSkill::~CSkill()
{

}


//初始化
BOOL CSkill::Init(DT_SKILL_DATA& stDT_SKILL_DATA, CBattleObj* poOwner, BOOL bPlayer)
{
    memcpy(&m_stDT_SKILL_DATA, &stDT_SKILL_DATA, sizeof(stDT_SKILL_DATA));

    SSkillActionProp* pstSkillActionProp = GetSkillActionProp();
    if(NULL == pstSkillActionProp)
    {
        return FALSE;
    }

    if(bPlayer)
    {
        if(NULL == poOwner)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: poOwner is NULL!"), MSG_MARK);
            return FALSE;
        }
        m_poOwner = dynamic_cast<CPlayer*>(poOwner);
        if(NULL == m_poOwner)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: poOwner is not CPlayer!"), MSG_MARK);
            return FALSE;
        }
    }
	
    return TRUE;
}


BOOL CSkill::Use(ESkillAffectPhase ePhase, CBattleObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, CBattleAttrObj* poSkillOwner,
                 vector<CBattleAttrObj*>& vecEnemyTargetObj, CBattleAttrObj* poSingleTarget)
{

    SSkillActionProp* pstSkillActionProp = GetSkillActionProp();
    if(NULL == pstSkillActionProp)
    {
        return FALSE;
    }
    SSkillEffectProp* pstSkillEffectProp = GetSkillEffectProp();
    if(NULL == pstSkillEffectProp)
    {
        stDefaultSkillEffectProp.byOnceEffectKindID = ESEK_HURT; // 默认存伤害效果
        pstSkillEffectProp = &stDefaultSkillEffectProp;
    }

    CSkillOnceAffect* poSkillOnceAffect = CSkillOnceAffectHandler::Instance()->GetSkillAffect(pstSkillEffectProp->byOnceEffectKindID);
    if(NULL == poSkillOnceAffect)
    {
        DBG_INFO(_SDT("[%s: %d]: GetSkillAffect failed, EffectKindID[%d]!"), MSG_MARK, pstSkillEffectProp->byOnceEffectKindID);
        return FALSE;
    }

    if(ESAP_BEFORE_COUNT_HURT == ePhase)
    {
        poSkillOnceAffect->Affect(ePhase, poSkillOwner, poMyself, poEnemy, pstSkillActionProp->wEffectHurtRate, pstSkillActionProp->dwExtHurtValue, pstSkillEffectProp->wOnceEffectParam, pstSkillEffectProp->wOnceEffectRate, NULL);
    }
    else if(ESAP_HIT_HURT == ePhase)
    {
        UINT8 bySize = vecEnemyTargetObj.size();
        for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
        {
            CBattleAttrObj* poTarget = vecEnemyTargetObj[byIdx];
            poSkillOnceAffect->Affect(ePhase, poSkillOwner, poMyself, poEnemy, pstSkillActionProp->wEffectHurtRate, pstSkillActionProp->dwExtHurtValue, pstSkillEffectProp->wOnceEffectParam, pstSkillEffectProp->wOnceEffectRate, poTarget);
        }
    }

    //添加buff
    for(UINT8 byBuffIdx = 0; byBuffIdx < MAX_SKILL_BUFF_NUM; byBuffIdx++)
    {
        SSkillOnceBuffProp& stBuffProp = pstSkillEffectProp->astSkillBuffrop[byBuffIdx];
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
            CSkillMgr::Instance()->AddBuff(poSkillOwner, poSkillOwner, pstSkillActionProp->wID, stBuffProp);
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
                   CSkillMgr::Instance()->AddBuff(poSkillOwner, poObj, pstSkillActionProp->wID, stBuffProp);
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
                CSkillMgr::Instance()->AddBuff(poSkillOwner, poSingleTarget, pstSkillActionProp->wID, stBuffProp);
            }
            else
            {
                UINT8 bySize = vecEnemyTargetObj.size();
                for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
                {
                    CSkillMgr::Instance()->AddBuff(poSkillOwner, vecEnemyTargetObj[byIdx], pstSkillActionProp->wID, stBuffProp);
                }
            }
        }
    }

    return TRUE;
}

VOID CSkill::GetDT_SKILL_DATA_CLI(DT_SKILL_DATA_CLI& stDT_SKILL_DATA_CLI)
{
    memset(&stDT_SKILL_DATA_CLI, 0, sizeof(stDT_SKILL_DATA_CLI));
    SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionInfoCli(m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wActionLevel, stDT_SKILL_DATA_CLI.stActionInfo);
    DT_SKILL_ACTION_DATA_CLI& stActionInfo = stDT_SKILL_DATA_CLI.stActionInfo;
    if(NULL == pstSkillActionProp)
    {
        return;
    }
    stActionInfo.dwCurExp = m_stDT_SKILL_DATA.dwActionExp - pstSkillActionProp->dwExp;
    SSkillActionProp* pstSkillActionPropNext = GetNextSkillActionProp();
    if(pstSkillActionPropNext)
    {
        stActionInfo.dwUpgradeExp = pstSkillActionPropNext->dwExp - pstSkillActionProp->dwExp;
    }
    else
    {
        stActionInfo.dwUpgradeExp = 0xFFFFFFFF;
    }

    SSkillEffectProp* pstSkillEffectProp = CSkillPropMgr::Instance()->GetSkillEffectInfoCli(m_stDT_SKILL_DATA.wEffectID, stDT_SKILL_DATA_CLI.stEffectInfo);
    if(pstSkillEffectProp)
    {
        stDT_SKILL_DATA_CLI.stEffectInfo.wUnlockLevel = CSkillPropMgr::Instance()->GetEffectUnlockLevel(m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wEffectID);
    }

    SSkillActionProp *poSkillActionProp = NULL;
    //兼容客户端计算错误
    poSkillActionProp = CSkillPropMgr::Instance()->GetSkillAttackRangInfoCli(m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.stAttackRangInfo.byAttackRangKind, m_stDT_SKILL_DATA.stAttackRangInfo.byTraceBuffKind, stDT_SKILL_DATA_CLI.stRangInfo);
    if (NULL != poSkillActionProp)
    {
        stDT_SKILL_DATA_CLI.stActionInfo.wEffectHurtRate = stDT_SKILL_DATA_CLI.stActionInfo.wEffectHurtRate * poSkillActionProp->wAttackRangHurtRate / 100;
        stDT_SKILL_DATA_CLI.stActionInfo.dwExtHurtValue = stDT_SKILL_DATA_CLI.stActionInfo.dwExtHurtValue * poSkillActionProp->wAttackRangHurtRate / 100;
    }
}

VOID CSkill::GetDT_ATTACK_RANG_DATA_LST(DT_ATTACK_RANG_DATA_LST& stAttackRangInfo)
{
    memset(&stAttackRangInfo, 0, sizeof(stAttackRangInfo));
    SSkillActionProp* pstProp = GetSkillActionProp();
    if(NULL == pstProp)
    {
        return;
    }
    vector<DT_ATTACK_RANG_DATA>& vecCurUnlockAttackRang = pstProp->vecCurUnlockAttackRang;
    UINT8 bySize = vecCurUnlockAttackRang.size();
    UINT16 wActionID = m_stDT_SKILL_DATA.wActionID;
    map<UINT16, UINT16> mapTmp;
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        DT_ATTACK_RANG_DATA_CLI& stRangInfo = stAttackRangInfo.astInfo[stAttackRangInfo.byNum++];
        DT_ATTACK_RANG_DATA& stDT_ATTACK_RANG_DATA = vecCurUnlockAttackRang[byIdx];
        CSkillPropMgr::Instance()->GetSkillAttackRangInfoCli(wActionID, stDT_ATTACK_RANG_DATA.byAttackRangKind, stDT_ATTACK_RANG_DATA.byTraceBuffKind, stRangInfo);
        mapTmp[CLogicFunc::Get88Key(stDT_ATTACK_RANG_DATA.byAttackRangKind, stDT_ATTACK_RANG_DATA.byTraceBuffKind)] = 1;
    }

    vector<DT_ATTACK_RANG_DATA>& vecAllRang = CSkillPropMgr::Instance()->GetActionAllRang(wActionID);
    bySize = vecAllRang.size();
    UINT16 wPlayerLevel = m_poOwner->GetLevel();
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        DT_ATTACK_RANG_DATA& stDT_ATTACK_RANG_DATA = vecAllRang[byIdx];
        if(mapTmp.end() != mapTmp.find(CLogicFunc::Get88Key(stDT_ATTACK_RANG_DATA.byAttackRangKind, stDT_ATTACK_RANG_DATA.byTraceBuffKind)))
        {
            continue;
        }
        if(wPlayerLevel < CSkillPropMgr::Instance()->GetAttackRangDspLevel(wActionID, stDT_ATTACK_RANG_DATA.byAttackRangKind, stDT_ATTACK_RANG_DATA.byTraceBuffKind))
        {
            //按顺序，跳出，后面的更不可能
            break;
        }
        DT_ATTACK_RANG_DATA_CLI& stRangInfo = stAttackRangInfo.astInfo[stAttackRangInfo.byNum++];
        CSkillPropMgr::Instance()->GetSkillAttackRangInfoCli(wActionID, stDT_ATTACK_RANG_DATA.byAttackRangKind, stDT_ATTACK_RANG_DATA.byTraceBuffKind, stRangInfo);
    }
}


VOID CSkill::GetDT_EFFECT_DATA_LST2(DT_EFFECT_DATA_LST2& stEffectInfo)
{
    memset(&stEffectInfo, 0, sizeof(stEffectInfo));
    SSkillActionProp* pstProp = GetSkillActionProp();
    if(NULL == pstProp)
    {
        return;
    }
    vector<UINT16>& vecCurUnlockSkillEffectID = pstProp->vecCurUnlockSkillEffectID;
    UINT8 bySize = vecCurUnlockSkillEffectID.size();
    UINT16 wActionID = m_stDT_SKILL_DATA.wActionID;


    UINT16 wCurJingJieLevel = 0;
    CJingJie *poJingJie = m_poOwner->GetJingJie();
    if(poJingJie)
    {
        wCurJingJieLevel = poJingJie->GetJingJieLevel();
    }

    map<UINT16, UINT16> mapTmp;
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        UINT16 wEffectID = vecCurUnlockSkillEffectID[byIdx];
        DT_SKILL_EFFECT_DATA_CLI2& stEffect = stEffectInfo.astInfo[stEffectInfo.byNum++];
        SSkillEffectProp* pstSkillEffectProp = CSkillPropMgr::Instance()->GetSkillEffectInfoCli(wEffectID, stEffect.stEffectInfo);
        if(pstSkillEffectProp)
        {
            stEffect.stEffectInfo.wUnlockLevel = CSkillPropMgr::Instance()->GetEffectUnlockLevel(wActionID, wEffectID);
        }
        CSkillPropMgr::Instance()->GetSkillEffectRecommendCli(wEffectID, wCurJingJieLevel, stEffect.astRecommendInfo);

        mapTmp[wEffectID] = 1;
    }

    vector<UINT16>& vecAllEffect = CSkillPropMgr::Instance()->GetActionAllEffect(wActionID);
    bySize = vecAllEffect.size();
    UINT16 wPlayerLevel = m_poOwner->GetLevel();
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        UINT16 wEffectID = vecAllEffect[byIdx];
        if(mapTmp.end() != mapTmp.find(wEffectID))
        {
            continue;
        }
        if(wPlayerLevel < CSkillPropMgr::Instance()->GetEffectDspLevel(wActionID, wEffectID))
        {
            //按顺序，跳出，后面的更不可能
            break;
        }
        DT_SKILL_EFFECT_DATA_CLI2& stEffect = stEffectInfo.astInfo[stEffectInfo.byNum++];
        SSkillEffectProp* pstSkillEffectProp = CSkillPropMgr::Instance()->GetSkillEffectInfoCli(wEffectID, stEffect.stEffectInfo);
        if(pstSkillEffectProp)
        {
            stEffect.stEffectInfo.wUnlockLevel = CSkillPropMgr::Instance()->GetEffectUnlockLevel(wActionID, wEffectID);
        }

        CSkillPropMgr::Instance()->GetSkillEffectRecommendCli(wEffectID, wCurJingJieLevel, stEffect.astRecommendInfo);
    }
}


SSkillActionProp* CSkill::GetSkillActionProp(BOOL bBattle)
{
    //是玩家数据
    SSkillActionProp* pstSkillActionProp = NULL;
    if (TRUE == bBattle && NULL != m_poOwner)
    {
        UINT16 wActionLevel = m_stDT_SKILL_DATA.wActionLevel > m_poOwner->GetLevel() ? m_poOwner->GetLevel() : m_stDT_SKILL_DATA.wActionLevel;
        pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(m_stDT_SKILL_DATA.wActionID, wActionLevel);
        if(NULL == pstSkillActionProp)
        {
            DBG_INFO(_SDT("[%s: %d]: pstSkillActionProp is NULL! Skill Action[%d.%d]."), MSG_MARK, m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wActionLevel);
        }
    }
    else
    {
        pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wActionLevel);
        if(NULL == pstSkillActionProp)
        {
            DBG_INFO(_SDT("[%s: %d]: pstSkillActionProp is NULL! Skill Action[%d.%d]."), MSG_MARK, m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wActionLevel);
        }
    }

// 	static SSkillActionProp stProp = {0};
// 	stProp = *pstSkillActionProp;
    return pstSkillActionProp;
}

SSkillActionProp* CSkill::GetNextSkillActionProp()
{
    SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wActionLevel + 1);
    if(NULL == pstSkillActionProp)
    {
        // DBG_INFO(_SDT("[%s: %d]: GetNextSkillActionProp is NULL! Skill Action[%d.%d]."), MSG_MARK, m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wActionLevel + 1);
    }
    return pstSkillActionProp;
}



SSkillEffectProp* CSkill::GetSkillEffectProp()
{
    SSkillEffectProp* pstSkillEffectProp = CSkillPropMgr::Instance()->GetSkillEffectProp(m_stDT_SKILL_DATA.wEffectID);

    return pstSkillEffectProp;
}

UINT16 CSkill::GetAttackRangHurtRate()
{
    return CSkillPropMgr::Instance()->GetAttackRangHurtRate(m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.stAttackRangInfo.byAttackRangKind, m_stDT_SKILL_DATA.stAttackRangInfo.byTraceBuffKind);
}


UINT16 CSkill::SetActionEffectRangProp(UINT16 wEffectID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind)
{
    //效果可以为0
    if((0 != wEffectID) && (!CSkillPropMgr::Instance()->GetSkillEffectProp(wEffectID)))
    {
        return ERR_SELECT_SKILL_ACTION_EFFECT::ID_EFFECT_UNLOCK;
    }
    if((byAttackRangKind <= EARK_INVALID) || (byAttackRangKind >= EARK_MAX))
    {
        DBG_INFO(_SDT("[%s: %d]: byAttackRangKind is invalid[%d]."), MSG_MARK, byAttackRangKind);
        return 1;
    }
    if((EARK_BUFF == byAttackRangKind) && (0 == byTraceBuffKind))
    {
        DBG_INFO(_SDT("[%s: %d]: EARK_BUFF byTraceBuffKind is 0[%d]."), MSG_MARK);
        return 1;
    }
    if(EARK_BUFF != byAttackRangKind)
    {
        byTraceBuffKind = 0;
    }

    SSkillActionProp* pstProp = CSkillPropMgr::Instance()->GetAttackRangActionProp(m_stDT_SKILL_DATA.wActionID, byAttackRangKind, byTraceBuffKind);
    if(NULL == pstProp)
    {
        return ERR_SELECT_SKILL_ACTION_EFFECT::ID_RANG_UNLOCK;
    }

    m_stDT_SKILL_DATA.wEffectID = wEffectID;
    m_stDT_SKILL_DATA.stAttackRangInfo.byAttackRangKind = byAttackRangKind;
    m_stDT_SKILL_DATA.stAttackRangInfo.byTraceBuffKind = byTraceBuffKind;

    return 0;
}


UINT16 CSkill::Upgrade(UINT16 wActionLevelCli)
{
    FLOAT fGasPerExp = (FLOAT)(CSkillPropMgr::Instance()->GetAddExpPerUpgrade()) / (FLOAT)(CSkillPropMgr::Instance()->GetCostGasPerUpgrade());

    UINT16 wAddLevel = 0;
    UINT32 dwCostGas = 0;
    UINT32 dwAddActionExp = 0;
    SSkillActionProp* pstNextProp = GetNextSkillActionProp();
    if(NULL == pstNextProp)
    {
        return ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_REACH_MAX_LEVEL;
    }

    if(m_poOwner->CKGuide(EGF_SKILLUPGRADE)) //引导阶段，不足也给过
    {
        //引导时，直接升级到2级,扣600
        dwCostGas = 600;
        dwCostGas = dwCostGas < (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwPurpleGas) ? dwCostGas : (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwPurpleGas);

        dwAddActionExp = pstNextProp->dwExp;
        wAddLevel = 1;
    }
    else
    {
        SSkillActionProp* pstNextProp = GetNextSkillActionProp();
        if(NULL == pstNextProp)
        {
            return ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_LEVEL_INVALID;
        }
        if(m_stDT_SKILL_DATA.dwActionExp >= pstNextProp->dwExp) //修改配置有可能...
        {
            dwAddActionExp = 0;
            dwCostGas = 0;
        }
        else
        {
            dwCostGas = pstNextProp->dwExp - m_stDT_SKILL_DATA.dwActionExp;
            dwCostGas = m_poOwner->GetDT_PLAYER_BASE_DATA().qwPurpleGas >= dwCostGas ? dwCostGas : (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwPurpleGas);
            if((FLOAT)dwCostGas < fGasPerExp)
            {
                return ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_NOT_ENOUGH_PURPLEGAS;
            }
            dwAddActionExp = UINT32((FLOAT)dwCostGas / fGasPerExp);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    if((m_stDT_SKILL_DATA.dwActionExp + dwAddActionExp) >= pstNextProp->dwExp)
    {
        //技能等级不能超过主角等级
        if(m_stDT_SKILL_DATA.wActionLevel >= m_poOwner->GetLevel())
        {
            return ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_LEVEL_UNLOCK;
        }
        m_stDT_SKILL_DATA.wActionLevel++;
    }
    m_stDT_SKILL_DATA.dwActionExp += dwAddActionExp;

    m_poOwner->DecPurpleGas(dwCostGas, CRecordMgr::EDPGT_UPGRADESKILLACTION, m_stDT_SKILL_DATA.wActionID, m_stDT_SKILL_DATA.wActionLevel);
    m_poOwner->AddGuideRecord(EGF_SKILLUPGRADE);

    CTaskMgr::Instance()->OnUpgradeSkill(m_poOwner);

    return ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_SUCCESS;
}

