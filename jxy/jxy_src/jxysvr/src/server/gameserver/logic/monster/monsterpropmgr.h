
#ifndef _MONSTERPROPMGR_H_
#define _MONSTERPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <protocol/server/protocommondata.h>
#include <logic/base/basepropmgr.h>
#include <map>
#include <vector>
using namespace std;


/// 怪物名称名称最大长度
#define NAME_LEN_MAX	32


/**
 * @brief 
 */
typedef struct tagMonsterProp
{
	UINT16	wKindID;		///< 怪物类型
	TCHAR	szName[NAME_LEN_MAX];		///< 怪物名称
	UINT8	byType;		//类型(普通1/boss2/精英3)
	UINT8   byFuncType;	//功能类型 1 以前普通本， 2 暗部
	UINT8	byCareerID;//职业类型
	UINT16	wLevel;	//等级
	UINT64	qwHP;	// 生命	
	UINT32	dwAttack; // 攻击力
	UINT32  dwDefend;//防御
	UINT16  wHitRate;//命中(百分比)
	UINT16	wDodgeRate;//闪避(百分比)
	UINT16  wCritRate;//暴击(百分比)
	UINT16	wDeCritRate;//抗暴(百分比)
	UINT16	wAngryValue;//怒气值
	UINT16	wSkillID;//拥有的技能ID
	UINT16	wSkillLevel;//拥有的技能等级
	UINT16	wSkillActionID; // 技能招式ID
	UINT16	wSkillActionLevel; // 技能招式等级
	UINT16	wSkillEffectID; // 技能效果ID
	UINT16	wGodweaponQuality;//神器品阶
	UINT16	wGodweaponLevel;//神器等级
	UINT32	dwGodweaponHP;//神器HP
	UINT32	dwGodweaponAttack;//神器攻击
	UINT16	wFakeID;	//怪物假的ID，只用于配合客户端显示
}SMonsterProp;

typedef multimap<UINT16, SMonsterProp>	CMapShadeMonster;
typedef CMapShadeMonster::iterator  CMapShadeMonsterItr;

typedef vector<SMonsterProp*> CVecShadeMonster;
typedef CVecShadeMonster::iterator CVecShadeMonsterItr;

typedef std::map<UINT32, SMonsterProp> CMonsterPropMap;
typedef std::map<UINT32, SMonsterProp>::iterator CMonsterPropMapItr;

class CMonsterPropMgr  : public CBasePopMgr
{
public:



    DECLARE_SINGLETON_PROPMGR(CMonsterPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_MONSTER ; }
    BOOL Init();
    VOID UnInit();

public:
	SMonsterProp* GetMonsterProp(UINT16 wKindID);
	SMonsterProp* GetMonsterProp(UINT16 wKindID, UINT16 wLevel);
	SMonsterProp* GetShadeMonsterProp(UINT16 wKindID);
	const CMonsterPropMap& GetShadeMonsterProp() { return m_mapKind2ShadeMonsterProp; }

public:
    CMonsterPropMgr();
    virtual ~CMonsterPropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
	BOOL LoadFromDB();
private:
	CMonsterPropMap m_mapKind2ShadeMonsterProp;
	CMonsterPropMap m_mapKind2MonsterProp; //
	CMonsterPropMap m_mapKindLvl2MonsterProp; //只有BOSS才有不同等级的情况，BOSS可以升级
};

#endif // #ifndef _MONSTERPROPMGR_H_
