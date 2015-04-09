
#ifndef _JINGJIEPROPMGR_H_
#define _JINGJIEPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <common/client/commondef.h>
#include <common/server/utility.h>
#include <protocol/server/protocligs.h>
#include <logic/base/basepropmgr.h>

using namespace std;

////生成获取的精气属性
//struct SGenGasProp
//{
//	UINT8		byType;		//精气类型
//	UINT8		byLevel;	//精气等级
//	UINT32		dwValue;	//精气值
//	UINT8		byRate;		//生成概率
//};
//
////提升精气等级属性
//struct SUpGasLevelProp
//{
//	UINT8		byIncLevel;	//提升等级
//	UINT8		byRate;		//概率
//};
//
////提升精气等级消耗铜币
//typedef map<UINT16, UINT32> CUpGasLevelCostCoinMap; //按顺序map，key为使用次数， value为使用铜币
//typedef CUpGasLevelCostCoinMap::iterator CUpGasLevelCostCoinMapItr;
//
//
////境界解锁技能招式属性
//struct SUnlockSkillActionProp
//{
//	UINT16 SkillActionID;
//	UINT16 SkillActionLevel;
//};


//满级的所有属性
struct SFullQualityLevelAttr
{
	SAllAttrProp stAllIncAttr;					///< 到该阶段时所有提升的属性
	UINT8	byIncAttrNum;						///< 到该阶段时所有提升的属性
	DT_ATTR_INC_DATA astIncAttrInfo[MAX_JINGJIE_ATTR_INC_NUM];///< 到该阶段时所有提升的属性
};


//境界子级升级属性
struct SJingJieSubLevelUpgradeProp
{
	UINT8	byCareerKind;						///< 职业ID
	UINT16	wLevel;								///< 境界品质级别
	UINT16	wSubLevel;							///< 境界子级
	UINT32	dwUpgradeGas;						///< 升级到该子级所需篮气
	UINT8	byIncAttrKind;						///< 该子级提升的属性
	UINT32	dwIncAttrValue;						///< 该子级的属性值

	SAllAttrProp stAllIncAttr;					///< 到该阶段时所有提升的属性
	UINT8	byIncAttrNum;						///< 到该阶段时所有提升的属性
	DT_ATTR_INC_DATA astIncAttrInfo[MAX_JINGJIE_ATTR_INC_NUM];///< 到该阶段时所有提升的属性
};

typedef map<UINT32, SJingJieSubLevelUpgradeProp> CJingJieSubLevelUpgradePropMap; //按顺序map，key为wLevel+wSubLevel
typedef CJingJieSubLevelUpgradePropMap::iterator CJingJieSubLevelUpgradePropMapItr;
typedef map<UINT16, DT_JINGJIE_PER_LEVEL_DATA> CJingJiePerLevelDataMap; //key为境界等级
typedef CJingJiePerLevelDataMap::iterator CJingJiePerLevelDataMapItr;

typedef map<UINT16, UINT16> CJingJieColorMap; //key为境界等级
typedef CJingJieColorMap::iterator CJingJieColorMapItr;




class CJingJiePropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CJingJiePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_JINGJIE ; }

    BOOL Init();


    VOID UnInit();

public:
	inline UINT16 GetMaxQuality()   {  return m_wMaxQualityLevel;  }
	inline UINT16 GetSubLevelNum()	{ return m_wSubLevelNum; }	

	//////////////////////////////////////////////////////////////	
	UINT32 GetJingJieSubLevelUpgradeGas(UINT8 byCareerID, UINT16 wLevel, UINT16 wSubLevel);
	const SAllAttrProp* GetAllIncAttr(UINT8	byCareerID, UINT16 wLevel, UINT16 wSubLevel);
	VOID GetAllIncAttr(UINT8 byCareerID, UINT16 wLevel, UINT16 wSubLevel, UINT8& byIncAttrNum, DT_ATTR_INC_DATA astIncAttrInfo[]);
	VOID GetJingJiePerLevelData(UINT8 byCareerID, UINT16 wLevel, DT_JINGJIE_PER_LEVEL_DATA& stDT_JINGJIE_PER_LEVEL_DATA);

	UINT16 GetJingJieColor(UINT16 wLevel);


public:
    CJingJiePropMgr();
    virtual ~CJingJiePropMgr();

protected:
	
	BOOL LoadFromDB();
	BOOL LoadJingJieBaseropFromDB();
	BOOL LoadJingJieSubLevelUpgradePropFromDB();
	BOOL LoadJingJieColorPropFromDB();
	/////////////////////
	VOID InitIncAttr();

private:
	UINT16				m_wMaxQualityLevel;			//最大品质(阶)
	UINT16				m_wSubLevelNum;				//品阶子等级数
	CJingJieSubLevelUpgradePropMap		m_mapAttackCarrerJingJieSubLevelUpgradeProp;//攻击型境界子级升级属性
	CJingJieSubLevelUpgradePropMap		m_mapDefenseCarrerJingJieSubLevelUpgradeProp;//防御型境界子级升级属性
	SFullQualityLevelAttr				m_stAttackCarrerFullAttr;	//攻击型职业满级的所有属性
	SFullQualityLevelAttr				m_stDefenseCarrerFullAttr;	//防守型职业满级的所有属性
	CJingJiePerLevelDataMap			m_mapAttackCarrerJingJiePerLevelData;	//攻击型职业每级境界信息
	CJingJiePerLevelDataMap			m_mapDefenseCarrerJingJiePerLevelData;	//防守型职业每级境界信息
	CJingJieColorMap					m_mapJingJieColor;



};

#endif // #ifndef 
