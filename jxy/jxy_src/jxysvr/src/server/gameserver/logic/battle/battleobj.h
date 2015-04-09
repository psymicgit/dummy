
#ifndef _BATTLEOBJ_H_
#define _BATTLEOBJ_H_

#include <protocol/server/protocommondata.h>
#include "common/client/commondef.h"
using namespace SGDP;
using namespace std ;



enum EBattleObjType
{
    EBOT_PLAYER = 1,//玩家
    EBOT_MONSTERGROUP = 2,//怪物组
	EBOT_PLAYERGROUP = 2,//玩家组队
};


#define CRITRATE_UPPERLIMIT 80	//暴击上限百分比
#define CRITRATE_LOWERLIMIT 10	//暴击下限百分比

#define PARRY_UPPERLIMIT	80	//格挡上限百分比
#define PARRY_LOWERLIMIT	10	//格挡下限百分比


#define NORMAL_CRIT_RATE	150	//普通暴击率

#define PARRY_HURT_RATE	50		//格挡伤害率



//用于狗血的评分系统
struct DT_BATTLE_OBJ_HP
{
    UINT32 dwHP; //血量
    UINT32 byFormationIdx; //阵位

    DT_BATTLE_OBJ_HP()
    {
        memset(this, 0x00, sizeof(DT_BATTLE_OBJ_HP));
    }
};

struct DT_BATTLE_OBJ_GROUP_HP
{
    UINT8				byObjNum; //战斗者信息
    DT_BATTLE_OBJ_HP	astBattleObjInfo[MAX_FORMATION_IDX_NUM]; //战斗者信息

    DT_BATTLE_OBJ_GROUP_HP()
    {
        memset(this, 0x00, sizeof(DT_BATTLE_OBJ_GROUP_HP));
    }

    UINT32 CalcTotalHP()
    {
        UINT32 dwTotalHP = 0;
        for (UINT8 i = 0; i < MAX_FORMATION_IDX_NUM; i++)
        {
            dwTotalHP += astBattleObjInfo[i].dwHP;
        }

        return dwTotalHP;
    }

    void CopyHPTo(DT_BATTLE_OBJ_GROUP_DATA2 &stObjGroup)
    {
        for (UINT8 byIdx = 0; byIdx < stObjGroup.byObjNum; byIdx++)
        {
            DT_BATTLE_OBJ_DATA2 &stObj = stObjGroup.astBattleObjInfo[byIdx];

            for (UINT8 i = 0 ; i < byObjNum; i++)
            {
                DT_BATTLE_OBJ_HP &stCurHP = astBattleObjInfo[i];
                if(stCurHP.byFormationIdx == stObj.byFomationIdx)
                {
                    stObj.stBattleAttr.dwHP = stCurHP.dwHP;
                    break;
                }
            }            
        }
    }
};

class CBattleAttrObj;
class CSkill;
class CPet;
class CBattleObj
{
public:
    CBattleObj();
    ~CBattleObj();

public:
	virtual CPet* GetBattlePet() { return NULL; };
    virtual VOID StartBattle(CBattleObj* poEnemy) = 0;//每次战斗前(计算战斗属性等等)
    virtual EBattleObjType GetBattleObjType() = 0; //是哪种战斗生物
    virtual VOID EndBattle() = 0; //每次战斗后(清除战斗过程中的产出 buff/hang等)
	virtual VOID GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA) = 0;//赋值战斗组战斗者属性

    virtual VOID GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA) = 0;//赋值战斗组战斗者属性
public:
    DT_BATTLE_BASE_ATTRIBUTE& GetCurDT_BATTLE_ATTRIBUTE()
    {
        return m_stCurBattleAttribute;
    }
    DT_BATTLE_BASE_ATTRIBUTE& GetMaxDT_BATTLE_ATTRIBUTE()
    {
        return m_stMaxBattleAttribute;
    }
	//获取混乱状态的攻击对象
	CBattleAttrObj* GetChaosBattleObj(CBattleObj* poAttack, CBattleObj* poBeAttack, CBattleAttrObj* poCurAttack);
    VOID OnRoundEnd(BOOL bAttacker) ;//计算回合结束，怒气结算，清除该回合的各种效果，持续多回合的由buff解决
    VOID GetSkillSlotAngry(DT_BATTLE_ANGRY_DATA astAttackerAngry[MAX_SKILL_SLOT_NUM]);//获取技能槽怒气
    CBattleAttrObj* GetRoundNextObj(UINT8* pbyIdx = NULL);//获取回合内的下一个出手者
    BOOL RoundHaveNextObj();//回合内是否有下一个出手者
    CBattleAttrObj* GetFirstAttacker(UINT8* pbyIdx = NULL);//获取第一个出手者
    CBattleAttrObj* GetCurAttacker(UINT8* pbyIdx = NULL);//获取当前出手者
    CBattleAttrObj* GetTarget(UINT8 byAttckerFormationIdx, UINT8* pbyIdx = NULL);//获取攻击目标，第一排就近原则

    BOOL IsDead();//是否已死
    CSkill* TrigerSkill(UINT8 byAttckerFormationIdx, vector<CBattleAttrObj*>& vecTargetObj);//是否触发技能(有可能未命中技能)，返回NULL为未命中技能，使用普通攻击.vecTargetIdx为攻击的目标范围
    CBattleAttrObj* GetFormationObj(UINT8 byIdx)
    {
        if(byIdx > MAX_FORMATION_IDX_NUM) return NULL;
        return m_apoFormationObj[byIdx];
    }
    VOID ResetCurAttackObjIdx()
    {
        m_byCurAttackObjIdx = 0;
    }

    //获得先攻值
    UINT32 GetFirstAttackValue();

    VOID InitCurBattleAttr(DT_BATTLE_OBJ_GROUP_HP *pstInitHP = NULL);

    void BeStronger(UINT16 wStrongPercent = 100 /* 全属性（攻防+4隐藏属性）强化比例，50代表全属性变为原来的50%，150代表变为原来的150% */);
	
	VOID SetPetBattleState(BOOL bState){ m_bIsPetBattle = bState; }
	virtual UINT32 GetPower();
protected:
    VOID ResetBattleRadom();
    virtual VOID ResetSpecialEffects();
    //释放战斗过程中的buff
    VOID ReleaseBuff();
	//释放特效攻击
	virtual VOID ReleaseSpecialEffect();
    static VOID GetTargetObj(UINT8 byAttckerFormationIdx, DT_ATTACK_RANG_DATA stRangInfo, vector<CBattleAttrObj*>& vecTargetObj, CBattleObj* poTarget, ESkillEffectKind eKind = ESEK_MAX, UINT16 wLevel = 0);
protected:
    DT_BATTLE_BASE_ATTRIBUTE		m_stMaxBattleAttribute;		//最大战斗属性值（每个对象Init时赋值确定）
    DT_BATTLE_BASE_ATTRIBUTE		m_stCurBattleAttribute;		//当前战斗属性值，每次进入副本重新算,战斗后结算才+buff影响值。血量和气势直接+，攻击力和防御力只在buff回合内有效
    DT_HIDE_BATTLE_ATTRIBUTE	m_stHideBattleAttr;			//隐藏战斗属性
    DT_EXT_BATTLE_ATTRIBUTE	 m_stExtBattleAttr;			//扩展战斗属性
    CBattleObj*				 m_poEnemy;//
    UINT8					 m_byCurAttackObjIdx; //当前攻击对象的阵型序号
	BOOL m_bIsPetBattle;

public:
	CBattleAttrObj*			 m_apoFormationObj[MAX_FORMATION_IDX_NUM + 1]; //上阵的对象



};


#endif //#ifndef _PLAYER_H_

