
#ifndef _HEROPROPMGR_H_
#define _HEROPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>
#include "logic/fly/flydef.h"

#include <stddef.h>

/// 武将名称名称最大长度
#define NAME_LEN_MAX	32

struct tagDT_HERO_BASE_DATA_CLI;
typedef tagDT_HERO_BASE_DATA_CLI DT_HERO_BASE_DATA_CLI;

struct tagDT_SKILL_DATA_CLI;
typedef tagDT_SKILL_DATA_CLI DT_SKILL_DATA_CLI;

/**
 * @brief
 */
typedef struct tagHeroProp
{
    tagHeroProp(){
        memset(this, 0, offsetof(tagHeroProp, wUpgradeFromHeroID));

        vecCostMaterial.clear();
        vecCostMaterial.clear();
    }

    UINT16	wKindID;		///< 武将类型
    TCHAR	szName[NAME_LEN_MAX];		///< 武将名称
    UINT8	byCareerID;		///< 职业ID
    UINT16   wTalent;		///< 武将资质
    UINT16   wHitRate;//命中(百分比)
    UINT16	wDodgeRate;//闪避(百分比)
    UINT16   wCritRate;//暴击(百分比)
    UINT16	wDeCritRate;//抗暴(百分比)
    UINT16	wAngryValue;//怒气值
    UINT32	dwVisitCoin;//拜访所需铜币
    UINT8	byVisitNum;//需拜访次数
	UINT16	wVisitGold;//需拜访元宝
	UINT16	dwAttack;		//攻击
	UINT16	dwHP;			//血量
	UINT16	wProbability;		//概率
	UINT16	wGoldLeopardProbability;		//花元宝出豹子概率
	UINT16	wCoinLeopardProbability;		//花铜币出豹子概率
    UINT16	wSkillID;//拥有的技能ID
    UINT16	wSkillLevel;//拥有的技能等级
	UINT16	wSkillActionID; ///< 技能招式ID
	UINT16	wSkillActionLevel; ///< 技能招式等级
	UINT16	wSkillEffectID; ///< 技能效果ID

    UINT8	byIndex;//开放序号
	UINT16	wRecuitLevel;//可招募等级
	UINT16	wRequireJingjieLevel;//需要的境界阶等级

	UINT16	wMaxAbsorbNum;//最大可传递次数
	UINT8	byRecommend; //是否推荐

    BOOL bIsLegendHero; // 是否是外传英雄
    UINT16 wUpgradeToHeroID; // 进阶后的伙伴ID
    UINT16 wUpgradeFromHeroID; // 该伙伴从哪个前传伙伴升阶而来

    MaterialVec vecCostMaterial; // 进阶材料列表
    MaterialVec vecFlyMaterial; // 该伙伴飞升所消耗的材料
} SHeroProp;


typedef std::map<UINT16, SHeroProp> CHeroPropPriMap;
typedef std::map<UINT16, SHeroProp>::iterator CHeroPropPriMapItr;

/**
 * @brief 
 */
typedef struct tagHeroUpgradeProp
{
	UINT16	wLevel;		///< 武将等级
	UINT64	qwExperience;		///< 经验(总经验)
}SHeroUpgradeProp;

typedef std::map<UINT16, SHeroUpgradeProp> CHeroUpgradePropPriMap;
typedef std::map<UINT16, SHeroUpgradeProp>::iterator CHeroUpgradePropPriMapItr;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CHeroPropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CHeroPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_HERO ; }

    BOOL Init();

    VOID UnInit();

	BOOL LoadFromDB();

public:
	SHeroProp* GetHeroProp(UINT16 wHeroKindID);
	SHeroUpgradeProp* GetHeroUpgradeProp(UINT16 wLevel);
	CHeroPropPriMap& GetHeroPropMap() { return m_mapHeroProp; }


	VOID GetDT_HERO_BASE_DATA_CLI(DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI, DT_SKILL_DATA_CLI &stSkillInfo, UINT16 wHeroKindID);

	UINT16 GetLevel(UINT64	dwExperience);

public:
    CHeroPropMgr();
    virtual ~CHeroPropMgr();

protected:


    BOOL LoadHeroPropFromDB();
	BOOL LoadHeroUpgradePropFromDB();

private:
	CHeroPropPriMap			m_mapHeroProp;
	CHeroUpgradePropPriMap	m_mapHeroUpgradeProp;
};

#endif // #ifndef _HEROPROPMGR_H_
