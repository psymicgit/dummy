#include "shademonstergroup.h"
#include "shademonstermgr.h"
#include <common/client/errdef.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/player/player.h>
#include "logic/record/recordmgr.h"
#include "logic/other/state.h"
#include "common/server/ngstring.h"
#include "logic/factionshade/shadepropmgr.h"
#include <logic/monster/monsterpropmgr.h>
using namespace SGDP;

CShadeMonsterGroup::CShadeMonsterGroup()
{
	m_bGenBattleAttribute = FALSE;
	memset(&m_stBattleGroupData, 0, sizeof(m_stBattleGroupData));
	m_poMainShadeMonster = NULL;
}

CShadeMonsterGroup::~CShadeMonsterGroup()
{
	for (CID2ShadeMonsterMapItr itr = m_mapShadeMonster.begin(); itr != m_mapShadeMonster.end(); itr++)
	{
		CShadeMonsterMgr::Instance()->RemoveShadeMonster(itr->second);
	}
}

VOID CShadeMonsterGroup::StartBattle(CBattleObj* poEnemy)
{
	m_poEnemy = poEnemy;

	ResetBattleRadom();
	InitCurBattleAttr();
	SetPetBattleState( FALSE );
}

BOOL CShadeMonsterGroup::Init(UINT16 wLevel, UINT8 byType, const vector<UINT16>& vecMonsterID )
{
	CVecPShadeMonster vecMonster;

	UINT8 byForType = CShadeMonsterMgr::Instance()->GetShadeMonsterVec(wLevel, byType, vecMonster);
	UINT8 abyFor[3] = {0};
	if (byForType == 1)
	{
		abyFor[0] = 1;
		abyFor[1] = 3;
		abyFor[2] = 5;
	}
	else
	{
		abyFor[0] = 2;
		abyFor[1] = 6;//第三个是防守放前面 
		abyFor[2] = 4;
	}
	UINT8 byIndex = 0;

	CVecMonsterFormation vecFormation;
	vector<UINT16>::const_iterator intItr = vecMonsterID.begin();
	CVecPShadeMonsterItr itr = vecMonster.begin();
	for ( ; itr != vecMonster.end() && intItr != vecMonsterID.end(); itr++, intItr++)
	{
		SShadeMonsterFormation stFormation = {0};
		stFormation.byFormation = abyFor[byIndex];
		CShadeMonster* poShadeMonster = CShadeMonsterMgr::Instance()->CreateShadeMonster();
		if ( NULL == poShadeMonster)
		{
			CShadeMonsterMgr::Instance()->RemoveShadeMonster(poShadeMonster);
			return FALSE;
		}
		poShadeMonster->Init(*intItr, stFormation.byFormation, *itr);
		poShadeMonster->SetKindID(*intItr);
		poShadeMonster->SetFormation(stFormation.byFormation);
		stFormation.poShadeMonster = poShadeMonster;
		vecFormation.push_back(stFormation);
		m_mapShadeMonster[*intItr] = poShadeMonster;
		byIndex++;
	}

	for ( CVecMonsterFormationItr vecItr = vecFormation.begin(); vecItr != vecFormation.end(); vecItr++)
	{
		CShadeMonster* poMonster = vecItr->poShadeMonster;
		const SMonsterProp* pstMonsterProp = poMonster->GetMonsterProp();
		m_apoFormationObj[poMonster->GetFormationIdx()] = poMonster;

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

		if(vecItr == vecFormation.begin())
		{
			m_poMainShadeMonster = poMonster;

		}

		poMonster->GetDT_GODWEAPON_BATTLE_DATA(stDT_BATTLE_OBJ_DATA.stGodweaponInfo);
	}
	m_stCurBattleAttribute = m_stMaxBattleAttribute;
	return TRUE;
}

//每次战斗后(清除战斗过程中的产出 buff/hang等)
VOID CShadeMonsterGroup::EndBattle()
{
	ReleaseBuff();
}

VOID CShadeMonsterGroup::GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA)
{
	memcpy(&stDT_BATTLE_OBJ_GROUP_DATA, &m_stBattleGroupData, sizeof(DT_BATTLE_OBJ_GROUP_DATA3));
}

VOID CShadeMonsterGroup::GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA)
{

}