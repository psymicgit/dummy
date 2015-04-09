
#include "battleobj.h"


#include <sdutil.h>
#include <errdef.h>
#include <common/server/utility.h>
#include <logic/skill/skillmgr.h>
#include <logic/base/logicfunc.h>
#include <logic/other/ratemgr.h>
#include <logic/battle/battleattrobj.h>
#include <dll/sdframework/sdloggerimpl.h>
#include "logic/pet/pet.h"
#include "logic/lua/luamgr.h"
CBattleObj::CBattleObj()
{
    memset(&m_stCurBattleAttribute, 0, sizeof(DT_BATTLE_ATTRIBUTE));
    memset(&m_stMaxBattleAttribute, 0, sizeof(m_stMaxBattleAttribute));
    memset(&m_stHideBattleAttr, 0, sizeof(m_stHideBattleAttr));
    memset(&m_stExtBattleAttr, 0, sizeof(m_stExtBattleAttr));

    for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        m_apoFormationObj[byIdx] = NULL;
    }
    m_poEnemy = NULL;


    m_byCurAttackObjIdx = 0;
}

CBattleObj::~CBattleObj()
{
}


//释放战斗过程中的buff
VOID CBattleObj::ReleaseBuff()
{
    //释放buff
    for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if(poObj)
        {
            poObj->ReleaseBuff();
        }
    }
}

VOID CBattleObj::ReleaseSpecialEffect()
{
	//释放buff
	for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
	{
		CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
		if(poObj)
		{
			poObj->ReleaseSpecialEffect();
		}
	}
}

VOID CBattleObj::ResetBattleRadom()
{
    for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if(poObj)
        {
            poObj->ResetBattleRadom();
        }
    }
}

VOID CBattleObj::ResetSpecialEffects()
{

	for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
	{
		CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
		if(poObj)
		{
			poObj->ResetSpecialEffects();
		}
	}
}

CBattleAttrObj* CBattleObj::GetChaosBattleObj(CBattleObj* poAttack, CBattleObj* poBeAttack, CBattleAttrObj* poCurAttack)
{
	vector<CBattleAttrObj*> vecObj;
	UINT8 byChaosState = poCurAttack->GetChaos();
	if ( EBK_NONE == byChaosState)
	{
		return NULL;
	}
	else if (EBK_CHAOS == byChaosState)
	{
		//混乱攻击敌我双方
		for ( UINT8 byIdx = 1; byIdx <= 6; byIdx++)
		{
			CBattleAttrObj* poCurObj = poAttack->m_apoFormationObj[byIdx];
			if ( (poCurObj) && (!poCurObj->IsDead()) && (poCurAttack != poCurObj))
			{
				vecObj.push_back(poCurObj);
			}
			
			poCurObj = poBeAttack->m_apoFormationObj[byIdx];
			if ( (poCurObj) && (!poCurObj->IsDead()) && (poCurAttack != poCurObj))
			{
				vecObj.push_back(poCurObj);
			}

		}
	}
	else if ( EBK_DREAMLAND == byChaosState)
	{
		//攻击我方
		for ( UINT8 byIdx = 1; byIdx <= 6; byIdx++)
		{
			CBattleAttrObj* poCurObj = poAttack->m_apoFormationObj[byIdx];
			if ( (poCurObj) && (!poCurObj->IsDead()) && (poCurAttack != poCurObj))
			{
				vecObj.push_back(poCurObj);
			}
		}

		if ( 0 == vecObj.size())
		{
			CSkillMgr::Instance()->AddSleepBuff(poCurAttack, poCurAttack);
			return NULL;
		}
	}

	UINT32 dwSize = vecObj.size();
	if ( 0 == dwSize)
	{
		return NULL;
	}
	UINT32 dwIdx = RandomReseed(10000) % dwSize;

	return vecObj[dwIdx];
}

//计算回合结束，怒气结算，清除该回合的各种效果，持续多回合的由buff解决
VOID CBattleObj::OnRoundEnd(BOOL bAttacker)
{
    CBattleAttrObj* poAttacker = NULL;
    CBattleAttrObj* poBettacker = NULL;
    if(bAttacker)
    {
        poAttacker = GetCurAttacker();
        poBettacker = m_poEnemy->GetTarget(m_byCurAttackObjIdx);
    }
    else
    {
        poAttacker = m_poEnemy->GetCurAttacker();
        poBettacker = GetTarget(m_byCurAttackObjIdx);
    }

    for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if(poObj)
        {
            poObj->OnRoundEnd(poAttacker, poBettacker);
        }
    }
}



VOID CBattleObj::InitCurBattleAttr(DT_BATTLE_OBJ_GROUP_HP *pstInitHP)
{
    for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if(poObj)
        {
            if(pstInitHP)
            {
                for(UINT16 i = 0; i < pstInitHP->byObjNum; i++)
                {
                    DT_BATTLE_OBJ_HP &stObjHP = pstInitHP->astBattleObjInfo[i];
                    if(byIdx == stObjHP.byFormationIdx)
                    {
                        poObj->InitCurBattleAttr(&stObjHP);
                        break;
                    }
                }
            }
            else
            {
                poObj->InitCurBattleAttr();
            }
        }
    }

    // 重新赋值当前属性
    m_stCurBattleAttribute = m_stMaxBattleAttribute;
    m_byCurAttackObjIdx = 0;

    if(pstInitHP)
    {
        m_stCurBattleAttribute.qwHP = pstInitHP->CalcTotalHP();
    }
}

UINT32 CBattleObj::GetPower()
{
	return static_cast<UINT32>(m_stCurBattleAttribute.qwHP + m_stCurBattleAttribute.dwAttack * 5);
}
void CBattleObj::BeStronger(UINT16 wStrongPercent)
{
    if (100 == wStrongPercent)
    {
        return;
    }

    for(UINT8 byIdx = 0; byIdx < MAX_FORMATION_IDX_NUM + 1; byIdx++)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if(poObj)
        {
            poObj->BeStrong(wStrongPercent);
        }
    }
}

//获取回合内的下一个出手者
CBattleAttrObj* CBattleObj::GetRoundNextObj(UINT8* pbyIdx)
{
    if(m_byCurAttackObjIdx > MAX_FORMATION_IDX_NUM)
    {
        return NULL;
    }

    do
    {
        CBattleAttrObj* poObj = m_apoFormationObj[++m_byCurAttackObjIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = m_byCurAttackObjIdx;
            }
            return poObj;
        }
    }
    while(m_byCurAttackObjIdx < MAX_FORMATION_IDX_NUM);

    return NULL;
}

//回合内是否有下一个出手者
BOOL CBattleObj::RoundHaveNextObj()
{
    UINT8 byIdx = m_byCurAttackObjIdx;
    byIdx++;
    while(byIdx <= MAX_FORMATION_IDX_NUM)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            return TRUE;
        }
        byIdx++;
    }

    return FALSE;
}


//获取第一个出手者
CBattleAttrObj* CBattleObj::GetFirstAttacker(UINT8* pbyIdx)
{
    UINT8 byIdx = 1;
    while(byIdx <= MAX_FORMATION_IDX_NUM)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byIdx;
            }
            return poObj;
        }
        byIdx++;
    }

    return NULL;
}

//获取当前出手者
CBattleAttrObj* CBattleObj::GetCurAttacker(UINT8* pbyIdx)
{
	//宠物攻击
	if ( m_bIsPetBattle )
	{
		return GetBattlePet();
	}
    if(pbyIdx)
    {
        *pbyIdx = m_byCurAttackObjIdx;
    }
    return m_apoFormationObj[m_byCurAttackObjIdx];
}

/*
//获取攻击目标，就近原则
CBattleAttrObj* CBattleObj::GetTarget(UINT8 byAttckerFormationIdx, UINT8* pbyIdx)
{
    if(byAttckerFormationIdx > MAX_FORMATION_IDX_NUM)
    {
        return NULL;
    }
    UINT8 byIdx = 1;
    if(byAttckerFormationIdx > 3)
    {
        byIdx = byAttckerFormationIdx - 3;
    }
    else
    {
        byIdx = byAttckerFormationIdx;
    }
    UINT8 byLoopNum = 0;
    BOOL bNextRow = FALSE;
    BOOL bAddFlag = TRUE;
    UINT8 byStartIdx = byIdx;
    while(byLoopNum < 6)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byIdx;
            }
            return poObj;
        }
        byLoopNum++;
        if((byLoopNum >= 3) && (!bNextRow))
        {
            bNextRow = TRUE;
            byStartIdx += 3;
            byIdx = byStartIdx;
            bAddFlag = FALSE;
        }
        else
        {
            if(bAddFlag)
            {
                byIdx++;
            }
            else
            {
                byIdx--;
            }
            if(!bNextRow)
            {
                if(byIdx > 3)
                {
                    bAddFlag = FALSE;
                    byIdx = byStartIdx - 1;
                }
            }
            else
            {
                if(byIdx <= 3)
                {
                    byIdx = 6;
                    bAddFlag = FALSE;
                }
                if(byIdx > 6)
                {
                    bAddFlag = FALSE;
                    byIdx = byStartIdx - 1;
                }
            }
        }
    }

    if(pbyIdx)
    {
        *pbyIdx = 0;
    }

    return NULL;
}
*/

//获取攻击目标，就近原则
CBattleAttrObj* CBattleObj::GetTarget(UINT8 byAttckerFormationIdx, UINT8* pbyIdx)
{
    if(byAttckerFormationIdx > MAX_FORMATION_IDX_NUM)
    {
        return NULL;
    }
    UINT8 byIdx = 1;
    if(byAttckerFormationIdx > 3)
    {
        byIdx = byAttckerFormationIdx - 3;
    }
    else
    {
        byIdx = byAttckerFormationIdx;
    }

    CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
    if((poObj) && (!poObj->IsDead()))
    {
        if(pbyIdx)
        {
            *pbyIdx = byIdx;
        }
        return poObj;
    }

    switch(byIdx)
    {
    case 1:
    {
        UINT8 byTmpIdx = 2;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
        byTmpIdx = 3;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
    }
    break;
    case 2:
    {
        UINT8 byTmpIdx = 1;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
        byTmpIdx = 3;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
    }
    break;
    case 3:
    {
        UINT8 byTmpIdx = 1;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
        byTmpIdx = 2;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
    }
    break;
    }

    byIdx = byIdx + 3;
    poObj = m_apoFormationObj[byIdx];
    if((poObj) && (!poObj->IsDead()))
    {
        if(pbyIdx)
        {
            *pbyIdx = byIdx;
        }
        return poObj;
    }
    switch(byIdx)
    {
    case 4:
    {
        UINT8 byTmpIdx = 5;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
        byTmpIdx = 6;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
    }
    break;
    case 5:
    {
        UINT8 byTmpIdx = 4;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
        byTmpIdx = 6;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
    }
    break;
    case 6:
    {
        UINT8 byTmpIdx = 4;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
        byTmpIdx = 5;
        poObj = m_apoFormationObj[byTmpIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            if(pbyIdx)
            {
                *pbyIdx = byTmpIdx;
            }
            return poObj;
        }
    }
    break;
    }

    if(pbyIdx)
    {
        *pbyIdx = 0;
    }

    return NULL;
}

CSkill* CBattleObj::TrigerSkill(UINT8 byAttckerFormationIdx, vector<CBattleAttrObj*>& vecTargetObj)
{
    //默认攻击第一个目标
    CBattleAttrObj* poFirstTarget = m_poEnemy->GetTarget(m_byCurAttackObjIdx);
    if(NULL == poFirstTarget) //已经没有目标，返回
    {
        return NULL;
    }
    vecTargetObj.push_back(poFirstTarget);

    CBattleAttrObj* poObj = GetCurAttacker();
    if(NULL == poObj) //异常
    {
        return  NULL;
    }

	if( poObj->GetChaos())
	{
		vecTargetObj.clear();
		CBattleAttrObj* poChaosObj = GetChaosBattleObj(this, m_poEnemy, poObj);
		if ( NULL == poChaosObj)
		{
			return NULL;
		}
		vecTargetObj.push_back(poChaosObj);
		return NULL;
	}

    //被沉默
    if(FALSE == poObj->m_bCanSkillAttack)
    {
        return NULL;
    }
    CSkill* poSKill = poObj->TrigerSkill();
    if(NULL == poSKill)
    {
        return NULL;
    }

    vecTargetObj.clear();
    GetTargetObj(byAttckerFormationIdx, poSKill->GetSkillAttackRang(), vecTargetObj, m_poEnemy);

    if (vecTargetObj.size() == 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: vecTargetObj size == 0!"), MSG_MARK);
    }

    return poSKill;
}

UINT32 CBattleObj::GetFirstAttackValue()
{
    return m_stExtBattleAttr.dwFirstAttack;
}

VOID CBattleObj::GetTargetObj(UINT8 byAttckerFormationIdx, DT_ATTACK_RANG_DATA stRangInfo, vector<CBattleAttrObj*>& vecTargetObj, CBattleObj* poTarget, ESkillEffectKind eKind, UINT16 wLevel)
{
    if(byAttckerFormationIdx > MAX_FORMATION_IDX_NUM)
    {
        return;
    }
    vecTargetObj.clear();

    switch(stRangInfo.byAttackRangKind)
    {
        //前排单体
    case EARK_FRONTONE:
    {
        CRandomVec	vecRandom;
        CBattleAttrObj* poObj = NULL;
        for(UINT8 byIdx = 1; byIdx <= 6; byIdx++)
        {
            poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()))
            {
                vecRandom.push_back(1);
            }
            else
            {
                vecRandom.push_back(0);
            }
        }

        INT32 dwHit = GetRandomIdxMustHit(vecRandom, 0, 3);
        //DBG_INFO( _SDT("@1返回序号:%d"), dwHit );
        if (dwHit >= 0 && dwHit <= 5)
        {
            poObj = poTarget->m_apoFormationObj[dwHit + 1];
            if((poObj) && (!poObj->IsDead()))
            {
                vecTargetObj.push_back(poObj);
            }
        }
    }
    break;
    //后排单体
    case EARK_BACKONE:
    {
        CRandomVec	vecRandom;
        CBattleAttrObj* poObj = NULL;
        for(UINT8 byIdx = 1; byIdx <= 6; byIdx++)
        {
            poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()))
            {
                vecRandom.push_back(1);
            }
            else
            {
                vecRandom.push_back(0);
            }
        }

        INT32 dwHit = GetRandomIdxMustHit(vecRandom, 3, 6);
        //DBG_INFO( _SDT("@2返回序号:%d"), dwHit );
        if (dwHit >= 0 && dwHit <= 5)
        {
            poObj = poTarget->m_apoFormationObj[dwHit + 1];
            if((poObj) && (!poObj->IsDead()))
            {
                vecTargetObj.push_back(poObj);
            }
        }
    }
    break;
    //前排全体
    case EARK_FRONTALL:
    {
        for(UINT8 byIdx = 1; byIdx <= 3; byIdx++)
        {
            CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()))
            {
                vecTargetObj.push_back(poObj);
            }
        }
        if(0 == vecTargetObj.size())
        {
            for(UINT8 byIdx = 4; byIdx <= 6; byIdx++)
            {
                CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
                if((poObj) && (!poObj->IsDead()))
                {
                    vecTargetObj.push_back(poObj);
                }
            }
        }
    }
    break;
    //后排全体
    case EARK_BACKALL:
    {
        for(UINT8 byIdx = 4; byIdx <= 6; byIdx++)
        {
            CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()))
            {
                vecTargetObj.push_back(poObj);
            }
        }
        if(0 == vecTargetObj.size())
        {
            for(UINT8 byIdx = 1; byIdx <= 3; byIdx++)
            {
                CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
                if((poObj) && (!poObj->IsDead()))
                {
                    vecTargetObj.push_back(poObj);
                }
            }
        }
    }
    break;
    //所有
    case EARK_ALL:
    {
        for(UINT8 byIdx = 1; byIdx <= 6; byIdx++)
        {
            CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()))
            {
                vecTargetObj.push_back(poObj);
            }
        }
    }
    break;
    //追踪buff
    case EARK_BUFF:
    {
        UINT8 byTraceBuffKind = stRangInfo.byTraceBuffKind;
        for(UINT8 byIdx = 1; byIdx <= 6; byIdx++)
        {
            CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()) && (poObj->HaveBuff(byTraceBuffKind)))
            {
                vecTargetObj.push_back(poObj);
            }
        }

        //没有目标，则取第一个玩家
        if(0 == vecTargetObj.size())
        {
            for(UINT8 byIdx = 1; byIdx <= 6; byIdx++)
            {
                CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
                if((poObj) && (!poObj->IsDead()))
                {
                    vecTargetObj.push_back(poObj);
                    break;
                }
            }
        }
    }
    break;
    case EARK_MIXBLOOD:
    {
        CBattleAttrObj* poObj = NULL;
        for(UINT8 byIdx = 1; byIdx <= 6; byIdx++)
        {
            CBattleAttrObj* poTemp = poTarget->m_apoFormationObj[byIdx];
            if(NULL != poTemp)
            {
                if ((!poTemp->IsDead()) && (NULL == poObj))
                {
                    poObj = poTemp;
                }
                if((NULL != poObj) && (poTemp->GetCurDT_BATTLE_ATTRIBUTE().qwHP < poObj->GetCurDT_BATTLE_ATTRIBUTE().qwHP) && (!poTemp->IsDead()))
                {
                    poObj = poTemp;
                }
            }
        }
        if ((NULL != poObj) && (!poObj->IsDead()))
        {
            vecTargetObj.push_back(poObj);
        }
    }
    break;
    case EARK_RAMDOM_COMBAT_TWO_ENEMY:
    {
        int nNum = 0;
        CBattleAttrObj* poObj = NULL;
        UINT32 dwIndex = RandomReseed(6);
        for(UINT8 byIdx = dwIndex; byIdx <= 6 && nNum < 2; byIdx++)
        {
            CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()))
            {
                vecTargetObj.push_back(poObj);
                nNum++;
            }
        }

        for(UINT8 byIdx = 1; byIdx < dwIndex && byIdx <= 6 && nNum < 2; byIdx++)
        {
            CBattleAttrObj* poObj = poTarget->m_apoFormationObj[byIdx];
            if((poObj) && (!poObj->IsDead()))
            {
                vecTargetObj.push_back(poObj);
                nNum++;
            }
        }
    }
    break;
	case EARK_RANDOM_BY_PLAYER_LEVEL:
	{
		UINT32 dwSkillNum = CLuamgr::Instance()->GetSkillNumByPlayerLevel(eKind, wLevel);
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
    default:
        break;
    }
}

//是否已死
BOOL CBattleObj::IsDead()
{
    UINT8 byIdx = 1;
    while(byIdx <= MAX_FORMATION_IDX_NUM)
    {
        CBattleAttrObj* poObj = m_apoFormationObj[byIdx];
        if((poObj) && (!poObj->IsDead()))
        {
            return FALSE;
        }
        byIdx++;
    }

    return TRUE;
}


