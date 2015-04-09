

#ifndef _BUFFAFFECT_H_
#define _BUFFAFFECT_H_


#include <sdtype.h>
#include <common/client/commondef.h>

using namespace std ;


//buff作用阶段
//目前所有buff的作用阶段全部设为回合开始阶段！！！
enum EBuffAffectPhase
{
	EBAP_ROUND_BEGIN = 1, //回合开始阶段
	//EBAP_BEFORE_HIT = 2, //命中前
	//EBAP_BEFORE_COUNT_HURT = 3, //命中后，计算伤害前
	//EBAP_AFTER_COUNT_HURT = 4, //命中后，计算伤害后
};



//buff作用目标
enum EBuffTarget
{
	EBT_MYSELF = 1, //自己
	EBT_ENEMY = 2, //敌人
	EBT_MYALL = 3, //自己全体
	EBT_MY_FIRST = 4,//我方一手
};

class CBattleAttrObj;
class CBuffAffectHandler;

/**
* @brief buff效果作用接口基类
*/
class CBuffAffect
{
public:
	CBuffAffect(){};
    virtual ~CBuffAffect(){};

public:


    /**
    * @brief
    * 效果作用函数接口，所有的操作都是调用该接口
    * @return VOID
    */
    virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue) = 0;

	virtual EBuffKind GetBuffState() = 0;
	virtual EBuffTarget GetBuffTarget() = 0;
	virtual BOOL BigRound() = 0; 
	// 去掉防御技能及被动触发技能，增益buff目标为自己，触发技能时即可添加buff；debuff目标为对方，命中后添加
	//virtual	BOOL WantHit() = 0; //是否需命中才能添加buff 
protected:


protected:

};



//静止,暂定拥有该状态者本回合内攻击，属于负面状态
class CBuffAffect_Static: public CBuffAffect
{
public:
	static CBuffAffect_Static* Instance() { static CBuffAffect_Static oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_Static(){};        
	~CBuffAffect_Static(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_STATIC; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return TRUE; } ;
	//virtual	BOOL WantHit() { return TRUE; }

};


//沉默，暂定拥有该状态者本回合内使用技能，属于负面状态
class CBuffAffect_NoSkill: public CBuffAffect
{
public:
	static CBuffAffect_NoSkill* Instance() { static CBuffAffect_NoSkill oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_NoSkill(){};        
	~CBuffAffect_NoSkill(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_NO_SKILL; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return TRUE; } ;
	//virtual	BOOL WantHit() { return TRUE; }
	
};


//中毒，使得拥有该状态者本回合内掉百分比血量，属于负面状态
class CBuffAffect_Poisoning: public CBuffAffect
{
public:
	static CBuffAffect_Poisoning* Instance() { static CBuffAffect_Poisoning oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_Poisoning(){};        
	~CBuffAffect_Poisoning(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_POISONING; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; } ;
	//virtual	BOOL WantHit() { return TRUE; }
	
};

//撕裂，使得拥有该状态者本回合内掉百分比血量，属于负面状态
class CBuffAffect_Tear: public CBuffAffect
{
public:
	static CBuffAffect_Tear* Instance() { static CBuffAffect_Tear oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_Tear(){};        
	~CBuffAffect_Tear(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_TEAR; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; } ;
	//virtual	BOOL WantHit() { return TRUE; }

};

//残废，使得拥有该状态者本回合内攻击力按百分比降低，属于负面状态
class CBuffAffect_DecAttack: public CBuffAffect
{
public:
	static CBuffAffect_DecAttack* Instance() { static CBuffAffect_DecAttack oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_DecAttack(){};        
	~CBuffAffect_DecAttack(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_DEC_ATTACK; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return TRUE; } ;
	//virtual	BOOL WantHit() { return TRUE; }
	
};

//嗜血(单体)， 使得拥有该状态者本回合内攻击力按百分比提升，属于增益状态	
class CBuffAffect_BloodthirstySingle: public CBuffAffect
{
public:
	static CBuffAffect_BloodthirstySingle* Instance() { static CBuffAffect_BloodthirstySingle oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_BloodthirstySingle(){};        
	~CBuffAffect_BloodthirstySingle(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_BLOODTHIRSTY_S; }
	virtual EBuffTarget GetBuffTarget() { return EBT_MYSELF; }
	virtual BOOL BigRound() { return TRUE; } ;
	//virtual	BOOL WantHit() { return FALSE; }
};


//嗜血(全体)， 使得拥有该状态者本回合内攻击力按百分比提升，属于增益状态	
class CBuffAffect_BloodthirstyAll: public CBuffAffect
{
public:
	static CBuffAffect_BloodthirstyAll* Instance() { static CBuffAffect_BloodthirstyAll oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_BloodthirstyAll(){};        
	~CBuffAffect_BloodthirstyAll(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_BLOODTHIRSTY_A; }
	virtual EBuffTarget GetBuffTarget() { return EBT_MYALL; }
	virtual BOOL BigRound() { return TRUE; } ;
	//virtual	BOOL WantHit() { return FALSE; }
};

//减伤(单体)，使得拥有该状态者受击命中时减少伤害值，属于增益状态
class CBuffAffect_DecHurtSingle: public CBuffAffect
{
public:
	static CBuffAffect_DecHurtSingle* Instance() { static CBuffAffect_DecHurtSingle oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_DecHurtSingle(){};        
	~CBuffAffect_DecHurtSingle(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_DECHURT_S; }
	virtual EBuffTarget GetBuffTarget() { return EBT_MYSELF; }
	virtual BOOL BigRound() { return FALSE; } ;
	//virtual	BOOL WantHit() { return TRUE; }
};


//减伤(全体)，使得拥有该状态者受击命中时减少伤害值，属于增益状态
class CBuffAffect_DecHurtAll: public CBuffAffect
{
public:
	static CBuffAffect_DecHurtAll* Instance() { static CBuffAffect_DecHurtAll oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_DecHurtAll(){};        
	~CBuffAffect_DecHurtAll(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_DECHURT_A; }
	virtual EBuffTarget GetBuffTarget() { return EBT_MYALL; }
	virtual BOOL BigRound() { return FALSE; } ;
	//virtual	BOOL WantHit() { return TRUE; }
};

class CBuffAffect_AddBlood: public CBuffAffect
{
public:
	static CBuffAffect_AddBlood* Instance() { static CBuffAffect_AddBlood oBuffAffet; return &oBuffAffet;}
protected:
	CBuffAffect_AddBlood(){};        
	~CBuffAffect_AddBlood(){};
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_ADD_BLOOD; }
	virtual EBuffTarget GetBuffTarget() { return EBT_MY_FIRST; }
	virtual BOOL BigRound() { return FALSE; } ;
};

//增加防御（百分比）,为前排单体增加防御，属于增益状态
class CBuffAffect_DefendPercent : public CBuffAffect
{
public:
	static CBuffAffect_DefendPercent* Instance()
	{ 
		static CBuffAffect_DefendPercent oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_DefendPercent(){}
	~CBuffAffect_DefendPercent(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_DEFEND_PERCENT; }
	virtual EBuffTarget GetBuffTarget() { return EBT_MY_FIRST; }
	virtual BOOL BigRound() { return FALSE; }
};

//冻结，使目标冻结，暂停该状态者回合内攻击，属于负面状态
class CBuffAffect_Frost : public CBuffAffect
{
public:
	static CBuffAffect_Frost* Instance()
	{ 
		static CBuffAffect_Frost oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_Frost(){}
	~CBuffAffect_Frost(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_FROST; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; }
};

//混乱，使该状态者不分敌我选择目标攻击，属于负面状态
class CBuffAffect_Chaos : public CBuffAffect
{
public:
	static CBuffAffect_Chaos* Instance()
	{ 
		static CBuffAffect_Chaos oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_Chaos(){}
	~CBuffAffect_Chaos(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_CHAOS; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; }
};

//灼烧，使目标损失生命，属于负面状态
class CBuffAffect_Firing : public CBuffAffect
{
public:
	static CBuffAffect_Firing* Instance()
	{ 
		static CBuffAffect_Firing oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_Firing(){}
	~CBuffAffect_Firing(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_FIRING; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; }
};

//干扰，降低攻击伤害，属于负面状态
class CBuffAffect_Disturb : public CBuffAffect
{
public:
	static CBuffAffect_Disturb* Instance()
	{ 
		static CBuffAffect_Disturb oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_Disturb(){}
	~CBuffAffect_Disturb(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_DISTURB; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; }
};

//失去斗志，使目标在作用期间内无法获得怒气，属于负面状态
class CBuffAffect_NotAnger : public CBuffAffect
{
public:
	static CBuffAffect_NotAnger* Instance()
	{ 
		static CBuffAffect_NotAnger oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_NotAnger(){}
	~CBuffAffect_NotAnger(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_NOT_ANGER; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; }
};

//沉睡，使目标沉睡回合内无法攻击，属于负面状态
class CBuffAffect_Sleep : public CBuffAffect
{
public:
	static CBuffAffect_Sleep* Instance()
	{ 
		static CBuffAffect_Sleep oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_Sleep(){}
	~CBuffAffect_Sleep(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_SLEEP; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; }
};

class CBuffAffect_Dreamland : public CBuffAffect
{
public:
	static CBuffAffect_Dreamland* Instance()
	{ 
		static CBuffAffect_Dreamland oBuffAffect; return &oBuffAffect;
	}
protected:
	CBuffAffect_Dreamland(){}
	~CBuffAffect_Dreamland(){}
public:
	virtual BOOL Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, CBattleAttrObj* poNormalAttackTarget, UINT32 dwExtValue);
	virtual EBuffKind GetBuffState() { return EBK_DREAMLAND; }
	virtual EBuffTarget GetBuffTarget() { return EBT_ENEMY; }
	virtual BOOL BigRound() { return FALSE; }
};
#endif //#ifndef _BUFFAFFECT_H_

