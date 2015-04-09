#include "playergroup.h"
#include <common/client/errdef.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/player/player.h>
#include <logic/instance/instancepropmgr.h>
#include "logic/record/recordmgr.h"
#include "common/server/ngstring.h"
#include <logic/hero/heropropmgr.h>

using namespace SGDP;



CPlayerGroup::CPlayerGroup()
{
    m_bGenBattleAttribute = FALSE;
    memset(&m_stBattleGroupData, 0, sizeof(m_stBattleGroupData));
}

CPlayerGroup::~CPlayerGroup()
{

}

VOID CPlayerGroup::StartBattle(CBattleObj* poEnemy)
{
    m_poEnemy = poEnemy;

    ResetBattleRadom();
    InitCurBattleAttr();
	ResetSpecialEffects();
	//组队副本中，在战斗结束后，要把指针置为当前自己。
	for(UINT16 wIdx = 1 ; wIdx < MAX_FORMATION_IDX_NUM + 1; wIdx++)
	{
		if (NULL != m_apoFormationObj[wIdx])
		{
			((CHero*)m_apoFormationObj[wIdx])->SetPlayerGroup(this);
		}
	}
}

BOOL CPlayerGroup::Init(vector<CHero*> vecHero)
{
    //if(vecHero.size() != 6)
    //{
    //    return FALSE;
    //}
    for(UINT16 wIdx = 1 ; wIdx < vecHero.size() && wIdx < MAX_FORMATION_IDX_NUM+1; wIdx++)
    {
        if(vecHero[wIdx] == NULL)
        {
            continue;
        }
        m_apoFormationObj[wIdx] = vecHero[wIdx];

        m_stMaxBattleAttribute.qwHP		+= vecHero[wIdx]->GetDT_BATTLE_ATTRIBUTE().qwHP;
        m_stMaxBattleAttribute.dwAttack		+= vecHero[wIdx]->GetDT_BATTLE_ATTRIBUTE().dwAttack;

        SHeroProp *poHeroProp = CHeroPropMgr::Instance()->GetHeroProp(vecHero[wIdx]->GetDT_HERO_BASE_DATA().wKindID);
        if (NULL == poHeroProp)
        {
            continue;
        }


        DT_BATTLE_OBJ_DATA2& stDT_BATTLE_OBJ_DATA	=	m_stBattleGroupData.astBattleObjInfo[m_stBattleGroupData.byObjNum++];
        stDT_BATTLE_OBJ_DATA.byFomationIdx			=	(UINT8)(wIdx);
        stDT_BATTLE_OBJ_DATA.wKindID				=	vecHero[wIdx]->GetDT_HERO_BASE_DATA().wKindID;//使用假的客户端能显示的ID
        stDT_BATTLE_OBJ_DATA.wLevel				=	vecHero[wIdx]->GetDT_HERO_BASE_DATA().wLevel;
        stDT_BATTLE_OBJ_DATA.byCareerID				=	poHeroProp->byCareerID;
        stDT_BATTLE_OBJ_DATA.stBattleAttr.dwHP		=	static_cast<UINT32>(vecHero[wIdx]->GetDT_BATTLE_ATTRIBUTE().qwHP);
        stDT_BATTLE_OBJ_DATA.stBattleAttr.dwAttack	=	vecHero[wIdx]->GetDT_BATTLE_ATTRIBUTE().dwAttack;

        CSkill* poSKill = vecHero[wIdx]->GetFormationSkill();
        if(poSKill)
        {
            stDT_BATTLE_OBJ_DATA.wSkillActionID		= poSKill->GetDT_SKILL_DATA().wActionID;
            stDT_BATTLE_OBJ_DATA.wSkillActionLevel	= poSKill->GetDT_SKILL_DATA().wActionLevel;
        }
        else
        {
            stDT_BATTLE_OBJ_DATA.wSkillActionID = 0;
            stDT_BATTLE_OBJ_DATA.wSkillActionLevel = 0;
        }
        stDT_BATTLE_OBJ_DATA.wNeedAngry = FULL_ANGRY;
        stDT_BATTLE_OBJ_DATA.wInitAngry = vecHero[wIdx]->GetDT_HIDE_BATTLE_ATTRIBUTE().wAngryValue;
        vecHero[wIdx]->GetDT_GODWEAPON_BATTLE_DATA(stDT_BATTLE_OBJ_DATA.stGodweaponInfo);
    }
    m_stCurBattleAttribute = m_stMaxBattleAttribute;
    return TRUE;
}

//每次战斗后(清除战斗过程中的产出 buff/hang等)
VOID CPlayerGroup::EndBattle()
{
    ReleaseBuff();


	//组队副本中，在战斗结束后，要把指针置空。
	for(UINT16 wIdx = 1 ; wIdx < MAX_FORMATION_IDX_NUM + 1; wIdx++)
	{
		if (NULL != m_apoFormationObj[wIdx])
		{
			((CHero*)m_apoFormationObj[wIdx])->SetPlayerGroup(NULL);
		}
	}
}

VOID CPlayerGroup::GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA)
{
    memcpy(&stDT_BATTLE_OBJ_GROUP_DATA, &m_stBattleGroupData, sizeof(DT_BATTLE_OBJ_GROUP_DATA3));
}


VOID CPlayerGroup::GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA)//赋值战斗组战斗者属性
{

}