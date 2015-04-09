
#ifndef _ITEMMGR_H_
#define _ITEMMGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <logic/item/item.h>
//#include <logic/item/box/box.h>
#include <logic/item/equip/equip.h>
#include <logic/item/goods/goods.h>
#include "logic/item/jewelry/jewelry.h"
#include <logic/record/recordmgr.h>
#include <vector>

using namespace std;
using namespace SGDP;

typedef HashMap<UINT64, CItem*> CID2ItemMap;
typedef CID2ItemMap::iterator	CID2ItemMapItr;

typedef vector<CItem*>	CItemVec;
typedef CItemVec::iterator CItemVecItr;


//创建道具结果
enum ECreateItemRet
{
    ECIR_SUCCESS = 0,//创建成功
    ECIR_FAILED_BAG_FULL = 1,//背包满
    ECIR_FAILED_KIND_INVALID = 2,//道具类型无效
    ECIR_FAILED_OTHER = 3,//其他错误
	ECIR_FAILED_HIT = 4,	//未命中宝箱
	ECIR_COIN_NOT_ENOUGH = 5, //铜钱不足
};

/**
* @brief 道具管理类
*/
class CItemMgr
{
public:
    DECLARE_SINGLETON(CItemMgr)

    /**
    * @brief
    * 初始化函数,读取配置文件等
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    */
    BOOL Init();

    /**
    * @brief
    * 卸载函数
    */
    VOID UnInit();

public:

	CItem*  FindItem(UINT32 dwPlayerID, UINT16 wKindID, UINT16 wIdx);
	//CEquip* FindEquip(UINT32 dwPlayerID, UINT16 wKindID, UINT16 wIdx);

    /**
    * @brief
    * 创建道具，并放入背包
    * @param wKindID		: 要创建的道具类型ID
    * @param wPileNum		: 要创建的道具个数(装备必须为1，物品>=1)，物品时有效
    * @param eRet			: 创建结果
    * @param bGive			: 是否系统给的，若系统给的背包满时加到临时背包中，否则创建失败
    * @param wLevel			: 要创建的道具等级(装备有，新装备默认初始等级1)
    */
    CItem*			CreateItemIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, UINT16 wLevel, CRecordMgr::EAddItemType emOpType, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0);
    CItem*			AttachItem(CPlayer* poPlayer, UINT16 wKindID, VOID* pData); //数据还原

    //创建装备，不 放入背包
    CEquip*			CreateEquip(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, UINT16 wLevel);
		/**
    * @brief
    * 移除道具，从管理器中移除
    * @return VOID
    */
    VOID            RemoveItem(CItem* poItem);

	//删除物品
	VOID            RemoveItem(CItem* poItem, CPlayer* poPlayer, CRecordMgr::EDecItemType eDecType, UINT64 qwParam1 = 0, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0);

    	 /**
    * @brief
    * 创建首饰，并放入背包
    * @param stDT_JEWELRY_DATA		: 要创建的道具的属性
    * @param wPileNum		        : 要创建的道具个数(装备必须为1，物品>=1)，物品时有效
    * @param wRet			        : 创建结果
    */
    CItem*			CreateJewelryIntoBag(CPlayer* poPlayer, DT_JEWELRY_DATA& stDT_JEWELRY_DATA, UINT16 wPileNum,ECreateItemRet& eRet, UINT16 wLevel,  CRecordMgr::EAddItemType emOpType);
	CItem*			CreateJewelryIntoBagByGM(CPlayer* poPlayer, UINT16 wItemID, C32Vec& vec32, ECreateItemRet& eRet );

	// 随机出礼包物品
	UINT16			RandomItemInnerBox(UINT16 wKindID);

public:
    UINT8			GetItemKindBYKindID(UINT16 wKindID);
    VOID				GetDT_ITEM_DATA_CLI2(CItem* poItem, DT_ITEM_DATA_CLI2& stDT_ITEM_DATA_CLI2);
    BOOL				GetDT_ITEM_DATA_CLI2(UINT16 wKindID, UINT16 wPileCount, DT_ITEM_DATA_CLI2& stDT_ITEM_DATA_CLI2);
	BOOL				GetDT_ITEM_DATA_CLI3(UINT16 wKindID, UINT16 wPileCount, DT_ITEM_DATA_CLI3& stDT_ITEM_DATA_CLI3);

//////////////////////////////// 道具操作  begin //////////////////////////////////////////
public:
    UINT16			Equip(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byDressType, UINT16 wDressItemKindID, UINT16 wDressItemIdx, UINT16 wUndressItemKindID, UINT16 wUndressItemIdx, DT_POWER_CHANGE_DATA& stHeroPowerInfo); //装备
	UINT16			QuickEquip(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byEquipNum, DT_EQUIP_ID astEquipIDLst[EQUIP_POS_NUM], DT_POWER_CHANGE_DATA& stHeroPowerInfo);
    UINT16          Strengthen(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byStrengthenType, UINT16 wLevel, 
		                        DT_EQUIP_DATA_STRENGTHEN_CLI& stEquipStrengthenInfo, DT_EQUIP_ENCHANT_DATA_CLI &stEquipEnchantInfo, DT_EQUIP_DATA_CLI &stEquipInfo, UINT32& dwStrengthen10Coin);//强化

    UINT16          Enchant(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT16 wLevel, DT_EQUIP_DATA_STRENGTHEN_CLI& stEquipStrengthenInfo, DT_EQUIP_ENCHANT_DATA_CLI&, DT_EQUIP_DATA_CLI_v410&);//强化

    // 装备制作
    UINT16          MakeEquip(CPlayer* poPlayer, UINT16 wEquipMakeBookID, UINT16 wBookCnt, BOOL bIsBuyMaterial);

    UINT16			Sell(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wIdx, UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI&);//出售道具
	UINT16			MeltEquip(CPlayer* poPlayer, UINT8 byGoldFlag, UINT16 wItemKindID, UINT16 wIdx);//熔炼器魂
	UINT16			MeltGood(CPlayer* poPlayer, UINT8 byGoldFlag, UINT16 wItemKindID, UINT16 wNum);//熔炼器魂

	UINT16			CheckMeltEquip(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wIdx, UINT32& dwCostGold);//熔炼器魂
	UINT16			CheckMeltGood(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wNum, UINT32& dwCostGold);//熔炼器魂
    UINT16			UseGoods(CPlayer* poPlayer, UINT16 wGoodsKindID, UINT16 wUseNum, UINT8& byUseBoxFlag, DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo);//物品使用

	BOOL				CkBagFull(CPlayer* poPlayer, vector<UINT16> vecItemID);//检查背包放入vecItemID物品后是否满(装备只算1个叠加数有效)
	BOOL				SysUseGoods(CPlayer* poPlayer, UINT16 wItemID, UINT16 wPileNum);//系统帮忙使用，是否可以使用，若可以使用则使用
	UINT16			DelItem(CPlayer* poPlayer, UINT16 wItemKindID);//直接删除装备，用于GM命令
protected:

//////////////////////////////// 道具操作  end //////////////////////////////////////////
protected:
    VOID            RemoveItem(CItemVec& vecItem);

	/**
    * @brief
    * 创建物品，并放入背包
    * @param wKindID		: 要创建的道具类型ID
    * @param wPileNum		: 要创建的道具个数(装备必须为1，物品>=1)，物品时有效
    * @param wRet			: 创建结果
    */
	CItem*			CreateGoodsIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, CRecordMgr::EAddItemType emOpType, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0);

    /**
    * @brief
    * 创建装备，并放入背包
    * @param wKindID		: 要创建的道具类型ID
    * @param wPileNum		: 要创建的道具个数(装备必须为1，物品>=1)，物品时有效
    * @param wRet			: 创建结果
    */
    CItem*			CreateEquipIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, UINT16 wLevel,  CRecordMgr::EAddItemType emOpType, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0);

	 /**
    * @brief
    * 创建装备，并放入背包
    * @param wKindID		: 要创建的道具类型ID
    * @param wPileNum		: 要创建的道具个数(装备必须为1，物品>=1)，物品时有效
    * @param wRet			: 创建结果
    */
    CItem*			CreateJewelryIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum,ECreateItemRet& eRet, UINT16 wLevel,  CRecordMgr::EAddItemType emOpType);
    //CItem*			CreateBox(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, BOOL bGive, UINT16 wLevel);
    UINT16			GetInnerBoxRateKindID(UINT16 wInnerBoxKindID);//获取内部宝箱随机出来的物品ID

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
    CItemMgr();
    ~CItemMgr();

private:

    CID2ItemMap				m_mapID2Item;
	CSDObjectPool<CEquip, CSDMutex>	m_oEquipShmemObjMgr;
	CSDObjectPool<CGoods, CSDMutex>	m_oGoodsShmemObjMgr;
	CSDObjectPool<CJewelry, CSDMutex>	m_oJewelryShmemObjMgr;
};



#endif //#ifndef _ITEMMGR_H_

