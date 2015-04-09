///<------------------------------------------------------------------------------
//< @file:   logic\pet\petskill.h
//< @author: Kang_zhiquan
//< @date:   2014年8月27日 10:1:51
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _petskill_h_
#define _petskill_h_
#include "logic/battle/battleobj.h"
#include "common/server/utility.h"
#include "logic/skill/skillpropmgr.h"
#include "logic/skill/skillaffect.h"
#include "logic/base/baseobj.h"
class CPetSkill : public CBaseObj
{
public:
	CPetSkill();
	~CPetSkill();
	BOOL Init(UINT16 wSkillID, CPlayer* poPlayer, CPet* poPet);
	UINT16 GetID();
	const SPetSkillEffectProp& GetSkillProp()
	{
		return m_stProp;
	}
	UINT8 GetAttackType();
	//获取技能类型
	UINT8 GetSkillType();
	//获取伤害倍率
	UINT16 GetHurtRate();
	//获取最终伤害倍率
	UINT16 GetFinalHurtRate();
	//获取特殊攻击参数
	UINT32 GetOnceParam();
	VOID GetTriger(vector<CBattleAttrObj*>& vecObj, CBattleObj* poBattleObj);
	BOOL Use(ESkillAffectPhase ePhase, CBattleObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, CPet* poSkillOwner,
		vector<CBattleAttrObj*>& vecEnemyTargetObj, CBattleAttrObj* poSingleTarget = NULL);//使用技能
	VOID SetPetSkillProp();
	
protected:
	
private:
	UINT16 m_wSkillID;//技能ID
	SPetSkillEffectProp m_stProp;
	CPet* m_poPet;
};



DECLARE_CLASS_POOL_MGR(CPetSkill);
#endif //_petskill_h_