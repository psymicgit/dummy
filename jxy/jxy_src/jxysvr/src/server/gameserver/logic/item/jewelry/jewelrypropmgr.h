#ifndef _JEWELRY_PROP_MGR_H_
#define _JEWELRY_PROP_MGR_H_
#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/basepropmgr.h>
#include <logic/base/propcontainer.h>
#include <map>
#include <vector>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include "common/server/utility.h"
#include "logic/base/basedefine.h"
using namespace SGDP;
using namespace std;

enum
{ 
	EJS_NORMAL = 0, //正常
	EJS_ALREAD_STRENG = 1,//已经强化过 
	EJS_COIN_NOT_ENOUGH = 2,//铜钱不足
	EJS_JEWELRY_IS_NULL = 3,//饰品没放全
};

class CJewelry;
class CPlayer;
struct SJewelryProp
{
	UINT16 wKindID;
	UINT16 wDressLevel;//佩戴需要等级
	UINT8 byColorType;//品质，见 commondef.h中的EColor
	TCHAR aszName[NORMAL_MSG_LEN];
	UINT32 dwDefend;
	UINT32 dwHP;
	UINT32 dwAttack;
	UINT32 dwHitRate;
	UINT32 dwDodgeRate;
	UINT32 dwCriRate;
	UINT32 dwDecRitRate;
	UINT8 byPosKindID;//佩带位置
	UINT32 dwFirstBase;
	UINT32 dwSecondBase;
};

typedef map<UINT16, SJewelryProp> CMapJewelryProp;
typedef CMapJewelryProp::iterator CMapJewelryPropItr;

typedef map<UINT32, SJewelryProp> CMapJewelryPropByLevelColor;
typedef CMapJewelryPropByLevelColor::iterator CMapJewelryPropByLevelColorItr;


struct SJewelryUpgradeProp
{
	UINT16 wJewelryLevel;	//饰品等级
	UINT16 wStrengLevel;	//强化等级
	UINT32 dwUpgradeItemID;	//消耗物品ID
	UINT16 wUpgradeNum;	//消耗物品个数
};

typedef map<UINT32, SJewelryUpgradeProp> CMapJewelryUpgradeProp;
typedef CMapJewelryUpgradeProp::iterator CMapJewelryUpgradePropItr;

struct SJewelryResolveItemProp
{
	UINT16 wItemProbility;
	UINT16 wItemNum;
	UINT16 wItemID;
};

typedef map<UINT32, SJewelryResolveItemProp> CMapJewelryResolveItemProp;
typedef CMapJewelryResolveItemProp::iterator CMapJewelryResolveItemPropItr;
struct SJewelryResolveProp
{
	UINT16 wJewelryLevel;
	UINT8 byColorType;
	CMapJewelryResolveItemProp mapItemProp;
	C3232Map mapItemRandom;
	UINT32 dwCostRes;
};

typedef map<UINT32, SJewelryResolveProp> CMapJewelryResolveProp;
typedef CMapJewelryResolveProp::iterator CMapJewelryResolvePropItr;

typedef map<UINT16, UINT32> CMapUpgradeItemNum;	//ID为KEY, VALUE物品数量 
typedef CMapUpgradeItemNum::iterator CMapUpgradeItemNumItr;

typedef map<UINT32, CMapUpgradeItemNum> CMapLevelUpgradeItemProp;//饰品等级与饰品强化等级为KEY， VALUECMapUpgradeItemNum
typedef CMapLevelUpgradeItemProp::iterator CMapLevelUpgradeItemPropItr;


struct SJewelryStrengProp
{
	UINT16 wJewelryLevel;	//饰品等级
	UINT8 byColorType;		//饰品品质	
	UINT8 byExtAttrID;		//附加属性I
	UINT16 wStrengLevel;	//强化等级
	UINT32 dwExtValue;		//附加总值
};

struct SJewelrySuitAttrProp
{
    SJewelrySuitAttrProp(){
        memset(this, 0, sizeof(*this));
    }

    UINT16 wJewelryStrengLvl;	 // 饰品精炼等级

    JewelryExtAttr eAttrType; // 属性类别
    UINT32 dwAttrVal;   // 属性提升的值
    ValueType eValType; // 值的类型：固定值、百分比
};

typedef std::vector<SJewelrySuitAttrProp> JewelrySuitAttrVec;

// 饰品套装信息
struct SJewelrySuitProp
{
    SJewelrySuitProp(){
        Clear();
    }

    void Clear(){
        wJewelryDressLvl = 0;
        vecAttr.clear();
        memset(&stSuitCli, 0, sizeof(stSuitCli));
    }

    UINT16 wJewelryDressLvl;	 // 饰品穿戴等级

    JewelrySuitAttrVec vecAttr;
    DT_JEWELRY_SUIT_CLI stSuitCli; // 缓存起来用于客户端显示
};

typedef map<UINT64, SJewelryStrengProp> CMapJewelryStrengProp;
typedef CMapJewelryStrengProp::iterator CMapJewelryStrengPropItr;

typedef map<UINT32, C3232Map> CMapJewelryRandom;
typedef CMapJewelryRandom::iterator CMapJewelryRandomItr;

typedef std::map<UINT16, SJewelrySuitProp> CMapJewelrySuit;

class CJewelryPropMgr : CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CJewelryPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_JEWELRY ; }

    /**
     * @brief 初始化属性管理器
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL Init();

    /**
     * @brief 结束属性管理器
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    VOID UnInit();

    /**
    * @brief 查找属性
    * @return 属性
    */
    SJewelryProp* GetProp(UINT16 wKindID);
	CMapJewelryProp& GetJewelryPropMap() { return m_mapJewelryProp; }
	UINT16 GetRandomJewelryID(UINT16 wLevel, UINT8 byColor);
	BOOL GenerateJewelryData(CPlayer* poPlayer, UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData);
	BOOL GenerateJewelryData(CPlayer* poPlayer, UINT16 wKindID, C32Vec& vec32, DT_JEWELRY_DATA& stJewelryData);
	VOID GetStrentItemInfo(CPlayer* poPlayer, DT_STRENGTH_LST_INFO& stItemInfo);
	//设置属性
	BOOL SetBaseBattleAttr(UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData);
	
	BOOL SetBaseInof(UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData);
	BOOL SetBaseInof(SJewelryProp& stProp, DT_JEWELRY_DATA& stJewelryData);
	BOOL SetHideBattleAttr(UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData);
	BOOL SetHideBattleAttr(UINT16 wKindID, C32Vec& vec32, DT_JEWELRY_DATA& stJewelryData);
	UINT32 GetPosRandom();
	UINT32 GetExtAttrRandom(CRandomMap& mapRandom);
	SJewelryUpgradeProp* GetUpgradeProp(UINT16 wJewelryLevel, UINT16 wStrengLevel);//升级配置

	UINT32 GetBaseAttrValue(UINT16 wJewelryID, UINT8 byKindID, UINT16 wStrengLevel);//基础属性值
	UINT32 GetExtAttrValue(UINT16 wKindID,  UINT8 byExtId, UINT16 wStrengLevel);//附加属性值 
	SJewelryUpgradeProp* GetUpgradeItemProp(UINT16 wJewelryLevel, UINT16 wStrengLevel);//获取强化道具ID

	SJewelryResolveProp* GetResolveProp(UINT16 wJewelryLevel, UINT8 byColorType);//基本分解配置

	CMapUpgradeItemNum* GetResolveReturn(UINT16 wJewelryLevel, UINT16 wStrengLevel);//升级道具统计
	UINT8 GetValueType(UINT8 byExtID){ return m_mapExtIDValueType[byExtID]; }

	CJewelry* CompareJewelry(CJewelry* poJewelry1, CJewelry* poJewelry2);
    VOID GetJewelryCli(DT_JEWELRY_DATA& stInData, DT_JEWELRY_DATA_CLI& stOutData);
	BOOL IsStrengItemID(UINT16 wKindID);

    SJewelrySuitProp* GetJewelrySuitProp(UINT16 wJewelryLevel, UINT16 wStrengLvl);

    UINT16 GetJewelrySuitLvl(SJewelrySuitProp&, UINT16 wStrengLvl);

public:
    CJewelryPropMgr();
    virtual ~CJewelryPropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadBaseAttrFromDB();
	BOOL LoadRandomFromDB();
	BOOL LoadStrengAttrFromDB();
	BOOL LoadUpgradeFromDB();
	BOOL LoadResolveFromDB();
    BOOL LoadExtRandomFromDB();
	BOOL LoadSuitAttFromDB();
private:
    CMapJewelryProp	m_mapJewelryProp;
	CMapJewelryPropByLevelColor m_mapJewelryPropByLevel;
	CMapJewelryUpgradeProp m_mapJewelryUpgradeProp;
	CMapJewelryResolveProp m_mapJewelryResoleProp;
	CMapLevelUpgradeItemProp m_mapLevelToItemProp;
	CMapJewelryStrengProp m_mapJewelryStrengProp;

	C3232Map m_mapPosRandom;
	C3232Map m_mapExtIDRandom;
	C88Map m_mapExtIDValueType;

    CMapJewelryRandom m_mapAttrRandom;
	CMapJewelrySuit m_mapSuit;
};

#endif