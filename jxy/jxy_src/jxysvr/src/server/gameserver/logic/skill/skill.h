#ifndef _SKILL_H_
#define _SKILL_H_

#include <logic/base/baseobj.h>
#include "skillaffect.h"
#include <protocommondata.h>
#include "common/server/utility.h"

using namespace std ;

class CBattleObj;
class CBattleAttrObj;

struct SSkillOnceBuffProp;
struct SSkillActionProp;
struct SSkillEffectProp;
class CBuff;

struct tagDT_SKILL_DATA_CLI;
typedef tagDT_SKILL_DATA_CLI DT_SKILL_DATA_CLI;

struct tagDT_ATTACK_RANG_DATA_LST;
typedef tagDT_ATTACK_RANG_DATA_LST DT_ATTACK_RANG_DATA_LST;

struct tagDT_EFFECT_DATA_LST2;
typedef tagDT_EFFECT_DATA_LST2 DT_EFFECT_DATA_LST2;

class CSkill: public CBaseObj
{
public:
    CSkill();
    ~CSkill();
public:
    //初始化
    BOOL Init(DT_SKILL_DATA& stDT_SKILL_DATA, CBattleObj* poOwner, BOOL bPlayer = TRUE);
    DT_SKILL_DATA& GetDT_SKILL_DATA()
    {
        return m_stDT_SKILL_DATA;
    }
    VOID GetDT_SKILL_DATA_CLI(DT_SKILL_DATA_CLI& stDT_SKILL_DATA_CLI);
    VOID GetDT_ATTACK_RANG_DATA_LST(DT_ATTACK_RANG_DATA_LST& stAttackRangInfo);
    VOID GetDT_EFFECT_DATA_LST2(DT_EFFECT_DATA_LST2& stEffectInfo);

    /////////////////////////////////////

    BOOL Use(ESkillAffectPhase ePhase, CBattleObj* poAttacker, CBattleObj* poMyself, CBattleObj* poEnemy, CBattleAttrObj* poSkillOwner,
             vector<CBattleAttrObj*>& vecEnemyTargetObj, CBattleAttrObj* poSingleTarget = NULL);//使用技能
    UINT16 Upgrade(UINT16 wActionLevelCli);//升级/培养

    //SSkillProp* GetSkillProp(); //获取属性
    SSkillActionProp* GetSkillActionProp(BOOL bBattle = FALSE);
    SSkillActionProp* GetNextSkillActionProp();
    SSkillEffectProp* GetSkillEffectProp();
    DT_ATTACK_RANG_DATA& GetSkillAttackRang()
    {
        return m_stDT_SKILL_DATA.stAttackRangInfo;
    }
    UINT16 GetAttackRangHurtRate();

    /////////////////////////////////////
    //BOOL SetActionEffect(UINT16 wActionID, UINT16 wActionLevel, UINT16 wEffectID);
    UINT16 SetActionEffectRangProp(UINT16 wEffectID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind);

protected:
 
private:
    //CBattleAttrObj*		m_poSkillOwner;
    DT_SKILL_DATA			m_stDT_SKILL_DATA;
};
DECLARE_CLASS_POOL_MGR(CSkill);

#endif //#ifndef 

