

#ifndef _SKILLAFFECT_H_
#define _SKILLAFFECT_H_


#include <sdtype.h>
#include <common/client/commondef.h>

using namespace std ;

class CBattleObj;
class CBattleAttrObj;




//技能效果作用阶段，1个回合内会各个阶段被调用2次，我方/敌方攻击时
//目前肯定命中
enum ESkillAffectPhase
{
	ESAP_BEFORE_COUNT_HURT = 1, //计算伤害前
	//ESAP_AFTER_COUNT_HURT = 2, //计算伤害后
	ESAP_HIT_HURT = 3, //伤害命中(有伤害且命中)
	ESAP_SECOND_ATTACK = 4,//二次伤害
};


class CSkillOnceAffectHandler;
/**
* @brief 技能效果作用接口基类
*/
class CSkillOnceAffect
{
public:
	CSkillOnceAffect(){};
    virtual ~CSkillOnceAffect(){};

public:

	//参数作用并添加Buff(若有)
    virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget) = 0;

	virtual ESkillEffectKind GetSkillEffectKind() = 0;
	
	
protected:
	
protected:
};

//伤害
class CSkillOnceAffect_Hurt: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Hurt* Instance() { static CSkillOnceAffect_Hurt oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Hurt(){};        
	~CSkillOnceAffect_Hurt(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_HURT; } 
};



//单体回血
class CSkillOnceAffect_SingleCure: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_SingleCure* Instance() { static CSkillOnceAffect_SingleCure oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_SingleCure(){};        
	~CSkillOnceAffect_SingleCure(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_SINGLECUER; } 
};


//全体回血
class CSkillOnceAffect_WholeCure: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_WholeCure* Instance() { static CSkillOnceAffect_WholeCure oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_WholeCure(){};        
	~CSkillOnceAffect_WholeCure(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_WHOLECUER; } 
};

//恐吓
class CSkillOnceAffect_DecAngry: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_DecAngry* Instance() { static CSkillOnceAffect_DecAngry oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_DecAngry(){};        
	~CSkillOnceAffect_DecAngry(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_DECANGRY; } 
};

//风咒两连击
class CSkillOnceAffect_SecondAttack: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_SecondAttack* Instance() { static CSkillOnceAffect_SecondAttack oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_SecondAttack(){};        
	~CSkillOnceAffect_SecondAttack(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_SECOND_ATTACK; } 
};
//火系暴击

class CSkillOnceAffect_Critial : public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Critial* Instance() { static CSkillOnceAffect_Critial oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Critial(){};        
	~CSkillOnceAffect_Critial(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_CRITICAL; } 
};

//水咒
class CSkillOnceAffect_Water: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Water* Instance() { static CSkillOnceAffect_Water oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Water(){};        
	~CSkillOnceAffect_Water(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_WATER; } 
};
//土咒
class CSkillOnceAffect_Soil: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Soil* Instance() { static CSkillOnceAffect_Soil oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Soil(){};        
	~CSkillOnceAffect_Soil(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_SOIL; } 
};
//冰封术
class CSkillOnceAffect_Ice: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Ice* Instance() { static CSkillOnceAffect_Ice oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Ice(){};        
	~CSkillOnceAffect_Ice(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_ICE; } 
};
//砂暴术
class CSkillOnceAffect_Sand: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Sand* Instance() { static CSkillOnceAffect_Sand oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Sand(){};        
	~CSkillOnceAffect_Sand(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_SAND; } 
};
//流焱术
class CSkillOnceAffect_Spark: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Spark* Instance() { static CSkillOnceAffect_Spark oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Spark(){};        
	~CSkillOnceAffect_Spark(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_SPARK; } 
};
//阴魂不散
class CSkillOnceAffect_Haunting: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Haunting* Instance() { static CSkillOnceAffect_Haunting oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Haunting(){};        
	~CSkillOnceAffect_Haunting(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_HAUNTING; } 
};
//梦语幻境
class CSkillOnceAffect_Drame: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Drame* Instance() { static CSkillOnceAffect_Drame oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Drame(){};        
	~CSkillOnceAffect_Drame(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_DERAM; } 
};
//魂牵梦绕
class CSkillOnceAffect_Soul: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Soul* Instance() { static CSkillOnceAffect_Soul oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Soul(){};        
	~CSkillOnceAffect_Soul(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_SOUL; } 
};
//上古灵符
class CSkillOnceAffect_Charm: public CSkillOnceAffect
{
public:
	static CSkillOnceAffect_Charm* Instance() { static CSkillOnceAffect_Charm oSkillAffet; return &oSkillAffet;}
protected:
	CSkillOnceAffect_Charm(){};        
	~CSkillOnceAffect_Charm(){};
public:
	virtual BOOL Affect (ESkillAffectPhase ePhase, CBattleAttrObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, UINT16 wHurtRate, UINT32 dwExtHurtValue, UINT16 wParamValue, UINT16 wEffectRate, CBattleAttrObj* poTarget);
	virtual ESkillEffectKind GetSkillEffectKind() { return ESEK_CHARM; }

	ESkillEffectKind m_eLastEffect;

};


#endif //#ifndef _BUFFAFFECT_H_

