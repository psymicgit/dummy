#include "monstergroup.h"
#include "monstermgr.h"
#include <common/client/errdef.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/player/player.h>
#include <logic/instance/instancepropmgr.h>
#include "logic/record/recordmgr.h"
#include "logic/other/state.h"
#include "logic/hero/hero.h"
#include "common/server/ngstring.h"
using namespace SGDP;



CMonsterGroup::CMonsterGroup()
{
    m_bGenBattleAttribute = FALSE;
    memset(&m_stBattleGroupData, 0, sizeof(m_stBattleGroupData));
    m_poMainMonster = NULL;
    m_mapID2Monster.clear();
    m_dwEncourageCoin = 0;
    m_dwEncourageStory = 0;
    m_dwEncourageJingJie = 0;
    m_dwExperience = 0;
    m_wSceneIdx = 0;
    m_byTownIdx = 0;
    m_byInstanceIdx = 0;
    m_wMonsterIdx = 0;
}

CMonsterGroup::~CMonsterGroup()
{
    for(CID2MonsterMapItr itr = m_mapID2Monster.begin(); itr != m_mapID2Monster.end(); itr++)
    {
        CMonster* poMonster = itr->second;
        CMonsterMgr::Instance()->RemoveMonster(poMonster);
        poMonster = NULL;
    }
    m_mapID2Monster.clear();
}

VOID CMonsterGroup::StartBattle(CBattleObj* poEnemy)
{
    m_poEnemy = poEnemy;
	SetPetBattleState( FALSE );
    ResetBattleRadom();
    InitCurBattleAttr();
}

BOOL CMonsterGroup::Init(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx)
{
    m_wSceneIdx = wSceneIdx;
    m_byTownIdx = byTownIdx;
    m_byInstanceIdx = byInstanceIdx;
    m_wMonsterIdx = wMonsterIdx;

    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        return NULL;
    }
    UINT16 wSize = pstInstanceProp->vecWholeMonsterProp.size();
    //没有这波怪(wMonsterIdx从1开始）
    if(wMonsterIdx > wSize)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Instance[%d.%d.%d], wMonsterIdx > wSize[%d > %d]!"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx, wMonsterIdx, wSize);
        return NULL;
    }
    SWholeMonsterProp* pstWholeMonsterProp = pstInstanceProp->vecWholeMonsterProp[wMonsterIdx - 1];
    if(NULL == pstWholeMonsterProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Instance[%d.%d.%d], pstWholeMonsterProp is NULL >, MonsterIdx[%d]!"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx, wMonsterIdx);
        return NULL;
    }

    //初始化怪物
    wSize = pstWholeMonsterProp->vecMonster.size();
    if(0 == wSize)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Instance[%d.%d.%d], Monster size is 0 !"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx);
        return FALSE;
    }

    SMonsterProp* pstMainMonsterProp = NULL;
    for(UINT16 wIdx = 0 ; wIdx < wSize; wIdx++)
    {
        UINT16 wKindID = pstWholeMonsterProp->vecMonster[wIdx].wMonsterID;
        CMonster* poMonster = CMonsterMgr::Instance()->CreateMonster();
        if(!poMonster->Init(wKindID, pstWholeMonsterProp->vecMonster[wIdx].byFormationIdx))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CMonsterGroup poMonster::Init failed!"), MSG_MARK);
            CMonsterMgr::Instance()->RemoveMonster(poMonster);
            poMonster = NULL;
            return FALSE;
        }

        SMonsterProp* pstMonsterProp = poMonster->GetMonsterProp();
        m_apoFormationObj[poMonster->GetFormationIdx()] = poMonster;

        SMonsterUpgradeProp* pstMonsterUpgradeProp = poMonster->GetMonsterUpgradeProp();
        m_stMaxBattleAttribute.qwHP += pstMonsterProp->qwHP;
        m_stMaxBattleAttribute.dwAttack += pstMonsterProp->dwAttack;


        DT_BATTLE_OBJ_DATA2& stDT_BATTLE_OBJ_DATA = m_stBattleGroupData.astBattleObjInfo[m_stBattleGroupData.byObjNum++];
        stDT_BATTLE_OBJ_DATA.byFomationIdx = poMonster->GetFormationIdx();
        stDT_BATTLE_OBJ_DATA.wKindID = pstMonsterProp->wFakeID;//使用假的客户端能显示的ID
        stDT_BATTLE_OBJ_DATA.wLevel = pstMonsterProp->wLevel;
        stDT_BATTLE_OBJ_DATA.byCareerID = pstMonsterProp->byCareerID;
        stDT_BATTLE_OBJ_DATA.stBattleAttr.dwHP = static_cast<UINT32>(pstMonsterProp->qwHP);
        stDT_BATTLE_OBJ_DATA.stBattleAttr.dwAttack = pstMonsterProp->dwAttack;

        CSkill* poSKill = poMonster->GetFormationSkill();
        if(poSKill)
        {
            //stDT_BATTLE_OBJ_DATA.wSkillActionID = poSKill->GetDT_SKILL_DATA().wActionID;
            stDT_BATTLE_OBJ_DATA.wSkillActionID = poMonster->GetFakeSkillActionID();
            stDT_BATTLE_OBJ_DATA.wSkillActionLevel = poSKill->GetDT_SKILL_DATA().wActionLevel;
        }
        else
        {
            stDT_BATTLE_OBJ_DATA.wSkillActionID = 0;
            stDT_BATTLE_OBJ_DATA.wSkillActionLevel = 0;
        }
        stDT_BATTLE_OBJ_DATA.wNeedAngry = FULL_ANGRY;
        stDT_BATTLE_OBJ_DATA.wInitAngry = poMonster->GetDT_HIDE_BATTLE_ATTRIBUTE().wAngryValue;

        m_mapID2Monster[poMonster->GetMemID()] = poMonster;
        if(0 == wIdx)
        {
            m_poMainMonster = poMonster;

            //只给主怪的奖励
            m_dwEncourageCoin = poMonster->GetMonsterUpgradeProp()->dwEncourageCoin;
            m_dwEncourageStory =  poMonster->GetMonsterUpgradeProp()->dwEncourageStory;
            m_dwExperience = poMonster->GetMonsterUpgradeProp()->dwEncourageExp;
            m_dwEncourageJingJie =  poMonster->GetMonsterUpgradeProp()->dwEncourageJingJie;
        }

        ////赋值奖励铜币，不需每次计算
        //m_dwEncourageCoin += poMonster->GetMonsterUpgradeProp()->dwEncourageCoin;
        //m_dwEncourageStory +=  poMonster->GetMonsterUpgradeProp()->dwEncourageStory;
        //m_dwExperience += poMonster->GetMonsterUpgradeProp()->dwEncourageExp;

        poMonster->GetDT_GODWEAPON_BATTLE_DATA(stDT_BATTLE_OBJ_DATA.stGodweaponInfo);
    }
    m_stCurBattleAttribute = m_stMaxBattleAttribute;
    return TRUE;
}

BOOL CMonsterGroup::Init(vector<SSingleMonsterProp> vecMonster)
{
    SMonsterProp* pstMainMonsterProp = NULL;
    for(UINT16 wIdx = 0 ; wIdx < vecMonster.size(); wIdx++)
    {
        UINT16 wKindID = vecMonster[wIdx].wMonsterID;
        CMonster* poMonster = CMonsterMgr::Instance()->CreateMonster();
        if(!poMonster->Init(wKindID, vecMonster[wIdx].byFormationIdx))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CMonsterGroup poMonster::Init failed!"), MSG_MARK);
            CMonsterMgr::Instance()->RemoveMonster(poMonster);
            poMonster = NULL;
            return FALSE;
        }

        SMonsterProp* pstMonsterProp = poMonster->GetMonsterProp();
        m_apoFormationObj[poMonster->GetFormationIdx()] = poMonster;

        SMonsterUpgradeProp* pstMonsterUpgradeProp = poMonster->GetMonsterUpgradeProp();
        m_stMaxBattleAttribute.qwHP += pstMonsterProp->qwHP;
        m_stMaxBattleAttribute.dwAttack += pstMonsterProp->dwAttack;


        DT_BATTLE_OBJ_DATA2& stDT_BATTLE_OBJ_DATA = m_stBattleGroupData.astBattleObjInfo[m_stBattleGroupData.byObjNum++];
        stDT_BATTLE_OBJ_DATA.byFomationIdx = poMonster->GetFormationIdx();
        stDT_BATTLE_OBJ_DATA.wKindID = pstMonsterProp->wFakeID;//使用假的客户端能显示的ID
        stDT_BATTLE_OBJ_DATA.wLevel = pstMonsterProp->wLevel;
        stDT_BATTLE_OBJ_DATA.byCareerID = pstMonsterProp->byCareerID;
        stDT_BATTLE_OBJ_DATA.stBattleAttr.dwHP = static_cast<UINT32>(pstMonsterProp->qwHP);
        stDT_BATTLE_OBJ_DATA.stBattleAttr.dwAttack = pstMonsterProp->dwAttack;

        CSkill* poSKill = poMonster->GetFormationSkill();
        if(poSKill)
        {
            //stDT_BATTLE_OBJ_DATA.wSkillActionID = poSKill->GetDT_SKILL_DATA().wActionID;
            stDT_BATTLE_OBJ_DATA.wSkillActionID = poMonster->GetFakeSkillActionID();
            stDT_BATTLE_OBJ_DATA.wSkillActionLevel = poSKill->GetDT_SKILL_DATA().wActionLevel;
        }
        else
        {
            stDT_BATTLE_OBJ_DATA.wSkillActionID = 0;
            stDT_BATTLE_OBJ_DATA.wSkillActionLevel = 0;
        }
        stDT_BATTLE_OBJ_DATA.wNeedAngry = FULL_ANGRY;
        stDT_BATTLE_OBJ_DATA.wInitAngry = poMonster->GetDT_HIDE_BATTLE_ATTRIBUTE().wAngryValue;

        m_mapID2Monster[poMonster->GetMemID()] = poMonster;
        if(0 == wIdx)
        {
            m_poMainMonster = poMonster;

            //只给主怪的奖励
            m_dwEncourageCoin = poMonster->GetMonsterUpgradeProp()->dwEncourageCoin;
            m_dwEncourageStory =  poMonster->GetMonsterUpgradeProp()->dwEncourageStory;
            m_dwExperience = poMonster->GetMonsterUpgradeProp()->dwEncourageExp;
            m_dwEncourageJingJie =  poMonster->GetMonsterUpgradeProp()->dwEncourageJingJie;
        }
        poMonster->GetDT_GODWEAPON_BATTLE_DATA(stDT_BATTLE_OBJ_DATA.stGodweaponInfo);
    }
    m_stCurBattleAttribute = m_stMaxBattleAttribute;
    return TRUE;
}

//每次战斗后(清除战斗过程中的产出 buff/hang等)
VOID CMonsterGroup::EndBattle()
{
    ReleaseBuff();
}

VOID CMonsterGroup::Encourage(CPlayer* poPlayer, BOOL bFirst, UINT8 byRunTimes)
{
    if(NULL == poPlayer)
    {
        return;
    }



    //用于记录增加的次数
    CNGString strComment;
    strComment << byRunTimes;
	
	UINT32 dwCoin = ((100 + poPlayer->GetJewelryAttr(EJEA_INSTANCE_COIN)) * (m_dwEncourageCoin * byRunTimes) / 100);
    poPlayer->AddCoin(dwCoin, CRecordMgr::EACT_INSTANCE, m_wSceneIdx, m_byTownIdx, m_byInstanceIdx, m_wMonsterIdx, strComment.str());
    poPlayer->GetBattleEncourage().dwCoin += dwCoin;
    if(poPlayer->GetState().GetDT_STATE_DATA().wDoubleExpNum > 0)
    {
        m_dwExperience = m_dwExperience * 2;
    }

    if (EIT_TOWER != m_wSceneIdx)
    {
		UINT32 dwExperience = m_dwExperience * byRunTimes;
        poPlayer->AllotExperience(dwExperience);
		CHero* poCoachHero = poPlayer->GetCoachHero();
		if ( NULL != poCoachHero)
		{
			dwExperience = ((100 + poCoachHero->GetJewelryAttr(EJEA_INSTANCE_EXPERIENCE)) *  dwExperience / 100);
		}
        poPlayer->GetBattleEncourage().dwExperience += (dwExperience);
    }

    if(poPlayer->GetBuild(EBK_SCIENCE))
    {
		UINT32 dwStory = ((100 + poPlayer->GetJewelryAttr(EJEA_INSTANCE_STORY)) * (m_dwEncourageStory * byRunTimes) / 100);
        poPlayer->GetBattleEncourage().dwStory += dwStory;
        poPlayer->AddStory( dwStory, CRecordMgr::EASTT_INSTANCE, m_wSceneIdx, m_byTownIdx, m_byInstanceIdx, m_wMonsterIdx, strComment.str());
    }

    if(poPlayer->GetBuild(EBK_JINGJIE) && bFirst)
    {
        poPlayer->GetBattleEncourage().dwJingJie += (m_dwEncourageJingJie * byRunTimes);
        poPlayer->AddJingJie(m_dwEncourageJingJie * byRunTimes, CRecordMgr::EAJT_INSTANCE, m_wSceneIdx, m_byTownIdx, m_byInstanceIdx, m_wMonsterIdx, strComment.str());
    }
}

VOID CMonsterGroup::GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA)
{
	memcpy(&stDT_BATTLE_OBJ_GROUP_DATA, &m_stBattleGroupData, sizeof(DT_BATTLE_OBJ_GROUP_DATA2));
}

VOID CMonsterGroup::GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA)//赋值战斗组战斗者属性
{

}