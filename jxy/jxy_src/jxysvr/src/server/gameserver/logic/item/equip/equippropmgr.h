
#ifndef _EQUIPPROPMGR_H_
#define _EQUIPPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <protocol/server/protogtgs.h>
#include <logic/base/basepropmgr.h>

using namespace std;
using namespace SGDP;

/// 装备名称名称最大长度
#define NAME_LEN_MAX	32
/// 装备介绍名称最大长度
#define INTRODUCTION_LEN_MAX	255


/**
 * @brief
 */
typedef struct tagEquipProp
{
    UINT16	wKindID;					///< 装备类型
    TCHAR	szName[NAME_LEN_MAX];		///< 装备名称
    TCHAR	szIntroduction[INTRODUCTION_LEN_MAX];		///< 装备介绍
    UINT8	byPosKindID;				///< 装备位置类型(如头/身/左手/右手/左脚/右脚)，见 EEquipPosType
    UINT8	byEquipColor;				///< 装备颜色
    UINT16	wDressLevel;				///< 穿戴等级要求
    UINT32	dwBuyCoin;				 ///< 购买金币价格，二选一，0表示不支持，两个都配置以铜币为主
    UINT32	dwBuyGold;				 ///< 购买元宝价格，二选一，0表示不支持，两个都配置以铜币为主
    UINT32	dwRecoverPrice;			 ///< 回收价格(要加已强化价格，回收全按金币)
    UINT32	dwHP;					 ///< 生命
    UINT32	dwAttack;				 ///< 攻击力
    UINT32	dwDefend;				 ///< 防御
    UINT16	wStrengthenHPPerLvl;	 ///< 每强化一次加的生命
    UINT16	wStrengthenAttackPerLvl; ///< 每强化一次加的攻击力
    UINT16	wEnchantHPPerLvl;		 ///< 每附魔一次加的生命
    UINT16	wEnchantAttackPerLvl;	 ///< 每附魔一次加的攻击力
    UINT16	wEnchantDefendPerLvl;	 ///< 每附魔一次加的防御
	UINT16	wUpgradeToEquipID;		 ///< 升阶到的下一装备
	UINT16	wUpgradeSuccessRate;	 ///< 升级成功率
	UINT16	wUpgradeSuccessGold;	 ///< 增加成功率需要的元宝数
	UINT32	dwScience;				 ///< 魂器
	UINT32	dwStrengthenScience;	 ///< 强化增加的器魂点
    UINT32	dwEnchantSciencePerLvl;	 ///< 附魔增加的器魂点
    BOOL    bIsLegendEquip;		     ///< 是否是外传装备

	UINT8   byComposeItemNum;					//升阶所需材料
	UINT16  awComposeItem[MAX_COMPOSE_ITEM_NUM];	//升阶所需材料
} SEquipProp;

typedef HashMap<UINT16, UINT16> CUnlockEquipDressLevelMap; //升级解锁装备穿戴等级, 
typedef CUnlockEquipDressLevelMap::iterator CUnlockEquipDressLevelMapItr;


typedef HashMap<UINT16, SEquipProp> CEquipPropMap;		//key?awKindID
typedef CEquipPropMap::iterator CEquipPropMapItr;
/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CEquipPropMgr : public CBasePopMgr
{
public:
    //typedef HashMap<UINT32, SEquipProp> CEquipPropMap;		//key为wKindID+wQuality

    DECLARE_SINGLETON_PROPMGR(CEquipPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_EQUIP ; }

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
    //SEquipProp* GetProp(UINT16 wKindID, UINT16 wQuality);
    SEquipProp* GetProp(UINT16 wKindID);
	CEquipPropMap& GetEquipProp() { return m_mapEquipProp; }

	VOID GetDT_ESTIMATE_EQUIP_DATA(DT_ESTIMATE_EQUIP_DATA & stDT_ESTIMATE_EQUIP_DATA, UINT16 wKindID);

	UINT16 GetUnlockEquipDressLevel(UINT16 wPlayerLevel); 

public:
    CEquipPropMgr();
    virtual ~CEquipPropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadFromDB();

	VOID InitUnlockEquipDressLevel(); //初始化所有升级解锁装备穿戴等级

private:
    CEquipPropMap	m_mapEquipProp;
	CUnlockEquipDressLevelMap m_mapUnlockEquipDressLevel; //升级解锁装备穿戴等级。key/value为穿戴等级
};

#endif // #ifndef _EQUIPPROPMGR_H_
