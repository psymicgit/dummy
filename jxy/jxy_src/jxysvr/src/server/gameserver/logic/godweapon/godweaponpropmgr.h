
#ifndef _GODWEAPONUPGRADEPROPMGR_H_
#define _GODWEAPONUPGRADEPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <logic/base/basepropmgr.h>
#include <protocligs.h>
#include <logic/base/basedefine.h>

using namespace SGDP;

/// 品质名称名称最大长度
#define QUALITYNAME_LEN_MAX	32

/// 品质升阶属性数
#define MAX_UPGRADE_QUALITY_ATTR_NUM 4

#define MAX_GODWEAPON_QUALITY_NUM_EX	255

//神器升阶属性
typedef struct tagGodweaponQualityUpgradeProp
{
    UINT16   wHeroKindID;						///< 武将ID
    UINT16	wQualityLevel;						///< 神器品质级别
    TCHAR	szQualityName[QUALITYNAME_LEN_MAX];	///< 品质名称
    UINT16	wRequirePlayerLevel;				///< 需要玩家达到的等级
    UINT16	wUpgradeQualityItemNum;				///< 进阶到下一阶所需进阶符个数
    UINT8	abyBaseAttrKind[MAX_UPGRADE_QUALITY_ATTR_NUM];///< 当前阶提升的属性
    UINT32	adwBaseAttrValue[MAX_UPGRADE_QUALITY_ATTR_NUM];///< 当前阶提升的属性值
    UINT8	byQualityUpgradeEncAttrKind;		///< 进阶到下一阶奖励属性
    UINT32	dwUpgradeQualityEncAttrValue;		///< 进阶到下一阶奖励属性值

    SAllAttrProp stIncAttr;					///< 到该阶段所提升的属性(不包括奖励)
    SAllAttrProp stEncAttr;					///< 到该阶段所提升的奖励属性
    SAllAttrProp stAllAttr;					///< 到该阶段所提升的所有属性

} SGodweaponQualityUpgradeProp;


//神器升级属性
typedef struct tagGodweaponLevelUpgradeProp
{
    UINT16  wHeroKindID;					///< 武将ID
    UINT16	wQualityLevel;					///< 神器品质级别
    TCHAR	szQualityName[QUALITYNAME_LEN_MAX];	///< 品质名称
    UINT16	wUpgradeLevel;					///< 神器品质中的等级
    UINT32	dwUpgradeScience;				///< 升级到该级别所需魂器(科技)点
    UINT8	byUpgradeAttrKind;				///< 升级提升属性
    UINT32	dwUpgradeAttrValue;				///< 升级提升属性值

    SAllAttrProp stIncAttr;					///< 到该阶段所提升的属性(不包括奖励)
    SAllAttrProp stEncAttr;					///< 到该阶段所提升的奖励属性
    SAllAttrProp stAllAttr;					///< 到该阶段所提升的所有属性
} SGodweaponLevelUpgradeProp;

//武将所有品的升级奖励属性
typedef struct tagHeroGodweaponAllEncAttrProp
{
    UINT8			byEncAttrNum;
    DT_ATTR_INC_DATA	astQualityUpgradeEncAttr[MAX_GODWEAPON_QUALITY_NUM_EX]; //所有品的升级奖励属性
	tagHeroGodweaponAllEncAttrProp()
	{
		memset(this, 0x00, sizeof(tagHeroGodweaponAllEncAttrProp));
	}
} SHeroGodweaponAllEncAttrProp;



typedef HashMap<UINT32, SGodweaponQualityUpgradeProp> CGodweaponQualityUpgradePropMap; //key为武将ID+神器品质
typedef CGodweaponQualityUpgradePropMap::iterator CGodweaponQualityUpgradePropMapItr;

typedef HashMap<UINT64, SGodweaponLevelUpgradeProp> CGodweaponLevelUpgradePropMap; //key为武将ID+神器品质+神器等级
typedef CGodweaponLevelUpgradePropMap::iterator CGodweaponLevelUpgradePropMapItr;

typedef HashMap<UINT64, UINT64> CQualityUpgradeNum2Science; //key为神器品质+升级次数,value为魂器(科技)点
typedef CQualityUpgradeNum2Science::iterator CQualityUpgradeNum2ScienceItr;

typedef HashMap<UINT16, SAllAttrProp> CHeroFullGodweaponAttrMap; //key武将ID，value为该武将升满所有神器的属性
typedef CHeroFullGodweaponAttrMap::iterator CHeroFullGodweaponAttrMapItr;

typedef HashMap<UINT16, SHeroGodweaponAllEncAttrProp> CHeroID2EncAttrPropMap; //key为武将ID,
typedef CHeroID2EncAttrPropMap::iterator CHeroID2EncAttrPropMapItr;



/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CGodweaponPropMgr: public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CGodweaponPropMgr);

public:

    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_GODWEAPON ;
    }

    BOOL Init();

    VOID UnInit();

    SGodweaponQualityUpgradeProp* GetGodweaponQualiryUpgradeProp(UINT16 wHeroKindID, UINT16 wQuality);

    SGodweaponLevelUpgradeProp* GetGodweaponLevelUpgradeProp(UINT16 wHeroKindID, UINT16 wQuality, UINT16 wLevel);

    UINT64 GetUpgradeScience(UINT16 wHeroKindID, UINT16 wCurQuality, UINT16 wCurLevel);//获取升级所需魂器(科技)点

    UINT64 GetNextUpgradeScience(UINT16 wHeroKindID, UINT16 wCurQuality, UINT16 wCurLevel);////获取升级所需魂器(科技)点


    //获取升级到该品质等级总共所需魂器(科技)点
    UINT64 GetUpgradeTotalScience(UINT16 wHeroKindID, UINT16 wCurQuality, UINT16 wCurLevel);

    VOID GetGodweaponAttr(UINT32 dwCliVer, UINT16 wHeroKindID, INT16 wQuality, UINT16 wLevel, DT_GODWEAPON_ATTR_DATA& stIncAttr, UINT8 &byEncAttrNum, DT_ATTR_INC_DATA astQualityUpgradeEncAttr[MAX_GODWEAPON_QUALITY_NUM],
                          UINT16& wQualityRequireLevel, UINT32& dwUpgrageScience, UINT16& wUpgradeQualityNeedItemNum, DT_ATTR_INC_DATA& stNextLevelIncAttr);//获取当前品质级别的属性

    UINT16 GetMaxQuality(UINT16 wHeroKindID);

    SHeroGodweaponAllEncAttrProp *GetGodweaponAllEncAttr( UINT16 wHeroID );
public:
    CGodweaponPropMgr();
    virtual ~CGodweaponPropMgr();

protected:

    BOOL LoadQualityUpgradeFromDB();
    BOOL LoadLevelUpgradeFromDB();

    BOOL InitUpgradeTotalScience();//初始化升级总共所需魂器(科技)点
    BOOL InitQualityLevelHaveAttr(); // 初始化每品每级所提升的属性

    VOID SetGodweaponQualityAttr(UINT16 wHeroKindID, INT16 wQuality, SAllAttrProp& stIncAttr, SAllAttrProp& stEncAttr);
    VOID SetGodweaponLevelAttr(SGodweaponLevelUpgradeProp* pstLevelProp, SAllAttrProp& stIncAttr, SAllAttrProp& stEncAttr);
    VOID AddGodweaponAttrProp(SAllAttrProp& stAllAttr, SAllAttrProp& stAttr1, SAllAttrProp& stAttr2);

    SAllAttrProp* GetFullIncAttr(UINT16 wHeroKindID);
    SAllAttrProp* GetFullEncAttr(UINT16 wHeroKindID);

private:
    CGodweaponQualityUpgradePropMap	m_mapGodweaponQualityUpgradeProp;
    CGodweaponLevelUpgradePropMap	m_mapGodweaponLevelUpgradeProp;
    CQualityUpgradeNum2Science		m_mapUpgradeScience;			//每个品质等级升级到下一级所需的器魂点
    CQualityUpgradeNum2Science		m_mapUpgradeTotalScience;		//升级到该品质等级总共所需的器魂点
    //UINT16						m_wMaxQualityLevel;			//最大品质
    UINT8						m_byQualityUpdateNum;
    CHeroFullGodweaponAttrMap		m_mapHeroFullGodweaponIncAttr;	//武将I升满所有神器的属性(不包括奖励)
    CHeroFullGodweaponAttrMap		m_mapHeroFullGodweaponEncAttr;	//武将I升满所有神器的奖励属性
    CHeroID2EncAttrPropMap			m_mapHeroID2EncAttrProp; //武将所有品的升级奖励属性
};

#endif // #ifndef _GODWEAPONUPGRADEPROPMGR_H_
