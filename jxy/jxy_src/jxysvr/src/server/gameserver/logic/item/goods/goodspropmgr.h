
#ifndef _GOODSPROPMGR_H_
#define _GOODSPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <protocol/server/protocommondata.h>
#include <logic/base/basepropmgr.h>
#include <logic/base/propcontainer.h>

/// 物品名称名称最大长度
#define NAME_LEN_MAX	32
/// 物品介绍名称最大长度
#define INTRODUCTION_LEN_MAX	255


struct tagDT_GOODS_DATA_CLI;
typedef tagDT_GOODS_DATA_CLI DT_GOODS_DATA_CLI;

/**
 * @brief
 */
typedef struct tagGoodsProp
{
    UINT16	wKindID;		///< 物品类型
    TCHAR	szName[NAME_LEN_MAX];		///< 物品名称
    TCHAR	szIntroduction[INTRODUCTION_LEN_MAX];		///< 物品介绍
    UINT8	byMainKindID;		///< 物品主类型(如+体力,+金钱,+经验等分类)，见 EGoodsMainKind
    UINT16	wUseLevel;		///< 使用等级要求
    UINT32	dwBuyCoin;		///< 购买金币价格，二选一
    UINT32	dwBuyGold;		///< 购买元宝价格，二选一
    UINT32	dwRecoverPrice;	///< 回收价格(按百分比?加已强化消耗？)
    //UINT16	wPileNum;		///< 最大叠加数,叠加数无上限
    UINT16	wUseTimesPerDay;	///< 每天使用次数，0表示无限制
    UINT16	wUseCD;			///< 使用CD时间(单位/秒,0表示无CD)
    UINT32	dwValue;			///< 使用效果值(如+经验物品+的经验值)

	UINT16	wComposeCount;	//物品合成需要个数
	UINT16	wComposeKindID;	//物品合成的ID

    UINT8   byComposeItemNum;					//用户返回给客户端，材料合成的道具/礼包中的道具
    UINT16  awComposeItem[MAX_COMPOSE_ITEM_NUM];	//用户返回给客户端，材料合成的道具/礼包中的道具

	UINT32  dwParam1; //如果属于镶嵌宝石，那么wParam1表示宝石的类型. 宝石制作卷则为制作所需蓝气
	UINT32  dwParam2; //如果是镶嵌宝石，则记录可镶嵌的装备位置. 宝石制作卷则为制作卷等级(对应宝石等级)
	UINT32  dwParam3; //如果是镶嵌宝石，则记录可镶嵌的装备位置. 宝石制作卷则为转换的蓝气
	UINT32  dwParam4; //如果是镶嵌宝石，则记录可转换的蓝气值
} SGoodsProp;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CGoodsPropMgr : public CPropContainer<SGoodsProp, UINT16, TCHAR> , CBasePopMgr
{
public:
    typedef std::map<UINT16, SGoodsProp> CGoodsPropPriMap;
    typedef std::map<UINT16, SGoodsProp>::iterator CGoodsPropPriMapItr;

    DECLARE_SINGLETON_PROPMGR(CGoodsPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_GOODS ; }

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
     * @brief 获取属性主映射
     * @return 属性主映射对象指针
     */
    CGoodsPropPriMap* GetPropPriMap();

	VOID FillGoodsData( SGoodsProp *pProp, DT_GOODS_DATA_CLI& data, UINT16 wPileCount );

	SGoodsProp* GetGemMakerProp(UINT16 wGemMakerLevel);

public:
    CGoodsPropMgr();
    virtual ~CGoodsPropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadFromDB();

private:
	CGoodsPropPriMap m_mapGemMakerProp;//key为宝石制作卷等级(对应宝石等级)

};

#endif // #ifndef _GOODSPROPMGR_H_
