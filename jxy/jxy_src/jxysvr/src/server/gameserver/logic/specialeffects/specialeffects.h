///<------------------------------------------------------------------------------
//< @file:   logic\specialeffects\specialeffects.h
//< @author: Kang_zhiquan
//< @date:   2014年6月4日 17:28:12
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _specialeffects_h_
#define _specialeffects_h_
#include "logic/base/baseobj.h"
#include "protocol/server/protocligs.h"
#include "utility.h"
#include "logic/base/basedefine.h"
#include "commondef.h"
#include "logic/specialeffects/specialeffectsmgr.h"
class CPlayer;
class CBattleAttrObj;
class CSoul;
class CSpecialEffect : public CBaseObj
{
public:
	CSpecialEffect()
	{
		memset(&m_stSpecialEffectData, 0, sizeof(DT_SPECIAL_EFFECT_DATA));
		m_wLimitNum = 0;
		m_bySelectIdx = 0;
		m_poMySelf = NULL;
		m_wExtLimitNum = 0;
		m_wExtEffectPercent = 0;
		m_wExtTrigerProb = 0;
	}
	~CSpecialEffect()
	{
		memset(&m_stSpecialEffectData, 0, sizeof(DT_SPECIAL_EFFECT_DATA));
		m_wLimitNum = 0;
		m_bySelectIdx = 0;
		m_poMySelf = NULL;
		m_wExtLimitNum = 0;
		m_wExtEffectPercent = 0;
		m_wExtTrigerProb = 0;
	}
	DT_SPECIAL_EFFECT_DATA& GetSpecialEffectData(){ return m_stSpecialEffectData; }
	BOOL GetSpecialEffectDataCli(DT_SPECIAL_EFFECT_DESC& stCli);
	virtual ATTACK_TYPE GetAttackType() = 0;

	VOID SetOwner(CBattleAttrObj* poObj);
public:
	BOOL Init(CPlayer* poPlayer, CBattleAttrObj* poMySelf, const DT_SPECIAL_EFFECT_DATA& stData, UINT8 bySelectIdx);
	//是否能激活
	BOOL IsActivity();
	//激活
	BOOL SetActivity();
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag) = 0;
	//重置战斗数据
	virtual VOID ResetData();
	//增加作用次数
	VOID AddEffectNum();
	//获取特效ID
	UINT16 GetEffectID();
	//获取特效大类型
	UINT8 GetEffectBigType();
	//获取特效小类型
	UINT8 GetEffectSmallType();
	//获取剩余作用次数
	UINT16 GetLeftEffectNum();
	//获取生效概率
	UINT32 GetTrigerProb();
	//获取效果比
	UINT16 GetEffectPercent();
	//获取配置
	const SSpecialEffectProp* GetProp();

	VOID SetExtLimit(UINT16 );
	VOID SetExtEffectPercent(UINT16);
	VOID SetExtTrigerProb(UINT16);
protected:
	CBattleAttrObj* m_poMySelf;//穿戴者
	DT_SPECIAL_EFFECT_DATA m_stSpecialEffectData;
	UINT16 m_wLimitNum;//已作用次数
	UINT8 m_bySelectIdx;

	UINT16 m_wExtLimitNum;//额外作用次数
	UINT16 m_wExtEffectPercent;//额外效果
	UINT16 m_wExtTrigerProb;//额外触发概率
};

class CAttackSpecialEffect : public CSpecialEffect
{
	
};

class CDefendSpecialEffect : public CSpecialEffect
{

};
//连击
class CContinueAttackEffect : public CAttackSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	 ATTACK_TYPE GetAttackType() { return EAT_ATTACK;}
};
//吸血
class CSuckBloodEffect : public CAttackSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	ATTACK_TYPE GetAttackType() { return EAT_ATTACK;}
};
//物免
class CPhysicalImmunityEffect : public CAttackSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	ATTACK_TYPE GetAttackType() { return EAT_ATTACK;}
};
//魔免
class CMagicImmunityEffect : public CAttackSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	ATTACK_TYPE GetAttackType() { return EAT_ATTACK;}
};
//治疗
class CAddBloodEffect : public CDefendSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	ATTACK_TYPE GetAttackType() { return EAT_DEFEND;}
};
//反弹
class CReboundEffect : public CDefendSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	ATTACK_TYPE GetAttackType() { return EAT_DEFEND;}
};
//反击
class CBeatBack : public CDefendSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	ATTACK_TYPE GetAttackType() { return EAT_DEFEND;}
};
//闪避
class CDodgeEffect : public CDefendSpecialEffect
{
public:
	//特效起作用
	virtual VOID Effect(UINT64 dwHurtValue, BOOL bIsSkill, CBattleAttrObj* poEnemy, CSpecialEffect* poEnemyEffect, UINT8* bySpecialEffectFlag);
	ATTACK_TYPE GetAttackType() { return EAT_DEFEND;}
};
DECLARE_CLASS_POOL_MGR(CContinueAttackEffect)
DECLARE_CLASS_POOL_MGR(CSuckBloodEffect)
DECLARE_CLASS_POOL_MGR(CPhysicalImmunityEffect)
DECLARE_CLASS_POOL_MGR(CMagicImmunityEffect)
DECLARE_CLASS_POOL_MGR(CAddBloodEffect)
DECLARE_CLASS_POOL_MGR(CReboundEffect)
DECLARE_CLASS_POOL_MGR(CBeatBack)
DECLARE_CLASS_POOL_MGR(CDodgeEffect)
#endif //_specialeffects_h_
