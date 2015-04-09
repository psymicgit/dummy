
#ifndef _BATTLEATTROBJ_H_
#define _BATTLEATTROBJ_H_


#include <sdtype.h>
#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <logic/skill/skill.h>
#include <logic/skill/buffmgr.h>
#include <logic/battle/otherlife.h>
#include "logic/base/basedefine.h"
class CSpecialEffect;
typedef HashMap<UINT8, CBuff*> CBuffID2BuffMap; //key为BuffKindID
typedef CBuffID2BuffMap::iterator CBuffID2BuffMapItr;

typedef HashMap<UINT32, DT_BUFF_DATA> CBuffDataMap; //key为skillID+buffID
typedef CBuffDataMap::iterator CBuffDataMapItr;

typedef map<UINT32, CSpecialEffect*> CMapID2SpecialEffect;
typedef CMapID2SpecialEffect::iterator CMapID2SpecialEffectItr;

#define ROUND_ANGRY 50	//回合增加的怒气
#define FULL_ANGRY	100 //满怒气值

class CBattleObj;
struct DT_BATTLE_OBJ_HP;

//战斗属性类，有战斗属性的类都继承该类
class CBattleAttrObj:public CBaseObj
{
	friend class CBattleObj;
public:
	CBattleAttrObj();
	~CBattleAttrObj();
public:	
	DT_BATTLE_BASE_ATTRIBUTE&	GetDT_BATTLE_ATTRIBUTE() 
	{ 
		return m_stBattleAttribute;
	}
	DT_HIDE_BATTLE_ATTRIBUTE& GetDT_HIDE_BATTLE_ATTRIBUTE() {  return m_stHideBattleAttr; }
	DT_EXT_BATTLE_ATTRIBUTE& GetDT_EXT_BATTLE_ATTRIBUTE() {  return m_stExtBattleAttr; }

	virtual inline UINT8 GetFormationIdx() = 0;
protected:
	virtual VOID InitBattleAttribute() = 0;//给m_stBattleAttribute赋值		
protected:
	DT_BATTLE_BASE_ATTRIBUTE			m_stBattleAttribute;	//每个生物的战斗属性(只存储在内存，不存储到数据库)
	DT_BATTLE_BASE_ATTRIBUTE			m_stCurBattleAttribute;	//当前战斗属性值，每次进入副本重新算,战斗后结算才+buff影响值。血量和气势直接+，攻击力和防御力只在buff回合内有效				
	DT_HIDE_BATTLE_ATTRIBUTE		m_stHideBattleAttr;		//每个生物的隐藏战斗属性
	DT_EXT_BATTLE_ATTRIBUTE		m_stExtBattleAttr;		//每个生物的扩展战斗属性
	
	CBuffID2BuffMap					m_mapKindID2Buff;		//用于查找是否存在该buff（无叠加效果查询）
	CSkill*							m_apoSlotSKill[MAX_SKILL_SLOT_NUM+1];//技能槽中的技能	
	CSkill**						m_ppoFormationSkill; //先使用第1个技能槽的技能，后续扩展再修改
	COtherLife*						m_poOtherLife;//其他的生命生物
protected:
	VOID InitBattleMidParam();//初始化战斗中间变量
public:
	inline DT_BATTLE_BASE_ATTRIBUTE& GetCurDT_BATTLE_ATTRIBUTE() 
	{
		return m_stCurBattleAttribute;
	}
	inline BOOL IsDead() { return (0 == m_stCurBattleAttribute.qwHP) ? TRUE : FALSE ; }
	inline BOOL& CanGetData() { return m_bCanGetData; }
	inline CSkill* GetFormationSkill() { return *m_ppoFormationSkill; }
	VOID InitCurBattleAttr(DT_BATTLE_OBJ_HP *pstInitHP = NULL);

    void BeStrong(UINT16 wStrongPercent);
	VOID BuffRoundDec(BOOL bEndBigRound);

	VOID ResetBattleRadom();
	//////////////////////////////////攻击特效////////////////////////////////////////
	//清空特效作用次数
	virtual VOID ResetSpecialEffects();
	virtual VOID ReleaseSpecialEffect();
	//设置物免状态
	VOID SetPhysicalImmunity(BOOL bState);
	//设置魔免状态
	VOID SetMagicImmumity(BOOL bState);
	//获取物免状态
	BOOL GetPhysicalImmunity();
	//获取魔免状态
	BOOL GetMagicImmumity();
	//添加指定特殊
	BOOL AddSpecialEffects( CSpecialEffect* poEffect);
	//删除指定特效
	 CSpecialEffect* DelSpecialEffects(UINT8 bySpecialEffectID);
	 //获取触发的攻击特效
	 //技能攻击的时候IsSkill为TURE
	 CSpecialEffect* GetSpecialEffectsByAttack(BOOL bIsSkill);
	 //获取触发的防御特效
	 CSpecialEffect* GetSpecialEffectsByDefend(BOOL bIsSkill);
	//设置连击状态
	 VOID SetContinueAttack(CSpecialEffect* poEffect);
	 //获取连击状态
	 CSpecialEffect* GetContinueAttack();
	 //设置反击状态
	 VOID SetBeatBack(CSpecialEffect* poEffect);
	 //获取反击状态
	 CSpecialEffect* GetBeatBack();
	//////////////////////////////////攻击特效////////////////////////////////////////
	VOID RemoveBuff(CBuff* poBuff);
	//释放战斗过程中的buff
	VOID ReleaseBuff();
	CSkill* TrigerSkill();//是否触发技能(有可能未命中技能)，返回NULL为未命中技能，使用普通攻击	
	BOOL AddBuff(CBuff* poBuff);//添加buff
	BOOL AddSkillEffectID(UINT16 wSkillEffectID);//添加技能效果ID

	CBuffID2BuffMap& GetAllBuff() { return  m_mapKindID2Buff; }	
	VOID AddHPChange(INT32 nHP, EATTACK_TYPE bAttackType, UINT8 byReason, UINT8 byEmenyIdx, BOOL bAddToVec = TRUE);
	VOID AddAngryChange(INT16 shAngry);
	inline BOOL HaveBuff(UINT8 byBuffKind) {  return m_mapKindID2Buff.find(byBuffKind) != m_mapKindID2Buff.end() ? TRUE : FALSE; }

	VOID SetOtherLife(COtherLife* poOtherLife) 
	{ 
		m_poOtherLife = poOtherLife; 
	}
	COtherLife* GetOtherLife() { return m_poOtherLife; }

public:	
	UINT32	GetCurAttack(CBattleAttrObj* poTarget = NULL, UINT16 wAttackScale = 100);//获得当前的攻击力
	VOID	CKParryHurt(CBattleAttrObj* poTarget);//判断格挡/伤害，格挡返回TRUE
	VOID	OnRoundEnd(CBattleAttrObj* poAttacker, CBattleAttrObj* poBettacker) ;//计算回合结束，怒气结算，清除该回合的各种效果，持续多回合的由buff解决
	UINT16	GetAttackRankHurtRate(); const //获得攻击范围的伤害倍率
	/////////////////////////////////////////////////////////////////////////	
	inline UINT16 GetSkillActionID() const { return m_wSkillActionID; }
	inline UINT16 GetCurAngry() { return m_wCurAngry; }
	inline vector<DT_ANGRY_DATA>& GetAngryChange() { return m_vecAngryChange; }
	inline BOOL& GetCanNormalAttack() { return m_bCanNormalAttack; }
	inline BOOL& GetCanSkillAttack() { return m_bCanSkillAttack; }
	inline UINT8& GetChaos(){ return m_byChaosState; }
	inline BOOL& GetCanDodge() { return m_bCanDodge; }
	inline BOOL& GetNotAnger(){ return m_bNotAnger; }
	inline vector<DT_HP_DATA>& GetHPChange() { return m_vecHPChange; }
	inline vector<DT_HP_DATA2>& GetHPChange2() { return m_vecHPChange2; }
	inline UINT16& GetAddDodgeRate() { return m_wAddDodgeRate; }
	inline UINT16& GetAddCritRate() { return m_wAddCritRate; }
	inline UINT8& GetCritFlag() { return m_byCritFlag; }
	inline UINT16& GetAttackHurtRate() { return m_wAttackHurtRate; }
	inline UINT32& GetExtHur() { return m_dwExtHur; }
	inline UINT16& GetSkillHurtRate() { return m_wSkillHurtRate; }
	inline UINT32& GetSkillExtHur() { return m_dwSkillExtHur; }
	inline UINT16& GetFinalDecHurt(){ return m_wFinalDecHurt; }
	inline UINT16& GetFinalAddHurt(){ return m_wFinalAddHurt; }

	inline vector<DT_BUFF_DATA>& GetBuffChange() { return m_vecBuffChange; }
	inline vector<DT_SKILL_EFFECT_ID_DATA>& GetSkillEffectIDChange() { return m_vecfSkillEffectIDChange; }
	inline vector<DT_SHIELD_DATA>& GetShiledChange() { return m_vecShieldChange; }
	inline UINT32& GetDefenseHP() { return m_dwDefenseHP; }
	inline VOID	AddExtHur(UINT32 dwExtHur) { m_dwExtHur += dwExtHur; }
	inline VOID	AddSkillExtHur(UINT32 dwExtHur) { m_dwSkillExtHur += dwExtHur; }
	inline UINT8& GetParryFlag() { return m_byParryFlag; }
	inline UINT16& GetDecHurtRate() { return m_wDecHurtRate; }
	inline UINT16& GetAddDefendRate(){ return m_wAddDefendRate; }
	inline UINT8& GetDodge(){ return m_byDodgeFlag; }
	VOID InitAttack();
	inline BOOL IsMoster() { return m_bMonster; }

protected:	
	BOOL	m_bMonster;//是否为怪物，适配客户端下发技能ID
	BOOL	m_bCanNormalAttack;//当前回合是否可能进行普通攻击
	BOOL	m_bCanSkillAttack;//当前回合是否可能进行技能攻击
	BOOL	m_bCanDodge;//是否可闪避
	BOOL	m_bCanCrit;//是否可暴击
	UINT8	m_byChaosState;//混乱标识//EBK_NONE 正常攻击， EBK_CHAOS混乱不分敌我，EBK_DREAMLAND梦境只攻击我方
	BOOL m_bPhysicalImmunity;//物免标识
	BOOL m_bMagicImmunity;//魔免标识
	BOOL	m_bCanGetData; //是否可取数据，用户战斗过程中取数据
	BOOL    m_bNotAnger;//失去斗志标识（怒气无变化）
	UINT8	m_byCritFlag;//暴击标志
	UINT8	m_byParryFlag;//格挡标志
	UINT8   m_byDodgeFlag;//闪避标志


	UINT16	m_wSkillActionID; //技能ID,0为普通攻击
	//UINT16	m_wSkillLevel; //技能等级
	UINT16	m_wCurAngry;	//当前怒气
	UINT16	m_wAddDodgeRate;//当前回合增加的闪避(百分比)
	UINT16  m_wAddCritRate;//当前回合增加的暴击(百分比)
	UINT16	m_wAttackHurtRate;	//攻击倍率(百分比)
	UINT32	m_dwExtHur;//附加的伤害值
	UINT16	m_wSkillHurtRate;//技能攻击倍率(百分比)
	UINT32	m_dwSkillExtHur;//附加的伤害值
	UINT32	m_dwDefenseHP;//抵挡的HP
	UINT16	m_wDecHurtRate;//减伤百分比
	UINT16  m_wAddDefendRate;//增加防御百分比
	UINT16  m_wFinalDecHurt;//消弱最终伤害比
	UINT16  m_wFinalAddHurt;//加强最终伤害比

	vector<DT_ANGRY_DATA> m_vecAngryChange;//怒气变化
	vector<DT_HP_DATA>	m_vecHPChange;	//当前回合血量变化
	vector<DT_HP_DATA2>	m_vecHPChange2;	//当前回合血量变化
	vector<DT_BUFF_DATA> m_vecBuffChange;//buff变化
	vector<DT_SKILL_EFFECT_ID_DATA> m_vecfSkillEffectIDChange;//技能效果变化
	vector<DT_SHIELD_DATA> m_vecShieldChange;//护罩变化


	//暴击闪避的随机序列
	vector<UINT8> avecCritRadom[MAX_FORMATION_IDX_NUM+1];//暴击随机序列
	UINT8 abyLastCritRadomIdx[MAX_FORMATION_IDX_NUM+1];//上次使用的暴击随机序列序号
	UINT8 abyLastInitCritRate[MAX_FORMATION_IDX_NUM+1];//上次初始化的暴击率
	vector<UINT8> avecParryRadom[MAX_FORMATION_IDX_NUM+1];//格挡随机序列
	UINT8 abyLastParryRadomIdx[MAX_FORMATION_IDX_NUM+1];//上次使用的格挡随机序列序号
	UINT8 abyLastParryHitRate[MAX_FORMATION_IDX_NUM+1];//上次初始化的格挡率	


	CSpecialEffect* m_bContinueAttack;//当前触发是否触发连击
	CSpecialEffect* m_bBeatBack;//当前是否触发反击

	//战斗目标所拥有的特殊攻击
	CMapID2SpecialEffect m_mapSpecialEffect;
};


#endif

