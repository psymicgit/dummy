#ifndef _BAG_H_
#define _BAG_H_

#include <logic/base/baseobj.h>
#include <sdhashmap.h>
#include "bagpropmgr.h"
#include <logic/item/item.h>
#include <logic/item/equip/equip.h>
#include <logic/item/goods/goods.h>
#include <common/client/commondef.h>
#include <logic/shmem/shmemmap.h>
#include <common/server/utility.h>

typedef HashMap<UINT64, CItem*> CItemMap;
typedef CItemMap::iterator CItemMapItr;

typedef map<UINT16, CItem*> CItemPosMap;//注意，以key排序需使用map，HashMap的key排序不是有序的(8)
typedef CItemPosMap::iterator CItemPosMapItr;

class CGoods;//
class CJewelry;
typedef HashMap<UINT16, CItem*> CKindID2ItemMap;
typedef CKindID2ItemMap::iterator CKindID2ItemMapItr;

typedef CShmemMap<UINT64, CItem> CID2ItemShmemMap;
typedef CShmemMap<UINT16, CItem> CPos2ItemShmemMap;
typedef CShmemMap<UINT32, CEquip> CID2EquipShmemMap;
typedef CShmemMap<UINT16, CGoods> CID2GoodsShmemMap;
typedef CShmemMap<UINT32, CJewelry> CID2JewelryShmemMap;

class CPlayer;
class CBag:public CBaseObj
{
public:
	CBag();
	~CBag();
public:
	//初始化相关对象
	BOOL Init(UINT8 byBagOpenNum, CPlayer*	poOwner);
	//添加道具,其中做道具叠加数运算合并
	BOOL AddItem(CItem* poItem, BOOL bNewItem = TRUE); //bNewItem是否新增的道具（注意，装备穿戴该值为FALSE）
	//清除道具
	BOOL DelItem(CItem* poItem, BOOL bDeleteItem = TRUE, CRecordMgr::EDecItemType wRecordType = (CRecordMgr::EDecItemType)0);//bDeleteItem是否需清除的道具（注意，装备穿戴该值为FALSE）
	//替换道具
	BOOL ReplaceItem(CItem* poItem_IN, CItem* poItem_OUT);

	// 删除物品
	BOOL DelGoodItem(UINT16 usItemKindID, UINT32 unDelNum);

	// 背包是否已满
	BOOL IsBagFull();

	//背包扩容
	UINT16 IncreaseSize(UINT16 wInc2Size, UINT8 byGoldFlag);

	UINT8			GetOpenNum() { return m_byOpenNum; }
	UINT16			GetUsedSize() { return m_mapItem.Size(); }
	UINT16			GetIdleSize();
	CID2ItemShmemMap& GetItemMap() { return m_mapItem; }
    CGoods*			GetGoodsItem(UINT16 wItemKindID);
	CEquip*			GetEquipItem(UINT16 wItemKindID);
	UINT16			GetGoodsPileNum(UINT16 wItemKindID);
	CItem*			GetItem();
	CItem*			GetItem(UINT64 qwMemID);

    CID2GoodsShmemMap& GetGoodsMap() { return m_mapKindID2Goods; }
	CID2EquipShmemMap& GetEquipMap() { return m_mapKindID2Equip; }

	VOID				GetDT_BAG_DATA_CLI(DT_BAG_DATA_CLI& stDT_BAG_DATA_CLI);//获取给客户端的背包基本信息
	VOID				GetDT_BAG_DATA_CLI(DT_BAG_DATA_CLI2& stDT_BAG_DATA_CLI);//获取给客户端的背包基本信息
	VOID				GetDT_EQUIP_DATA_LST_CLI(DT_EQUIP_DATA_LST_CLI& stDT_EQUIP_DATA_LST_CLI);//获取给客户端的背包装备信息
    VOID				GetDT_EQUIP_DATA_LST_STRENGTHEN_COMPOSE_CLI(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stDT_EQUIP_DATA_LST_CLI);//获取给客户端的背包装备信息
	VOID				GetEquipStrengthenEnchantCli(DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI &stEquips); //获取给客户端的背包装备信息
	VOID				GetDT_GOODS_DATA_LST_CLI(DT_GOODS_DATA_LST_CLI& stDT_GOODS_DATA_LST_CLI);//获取给客户端的背包装备信息	
	VOID				GetBagItem(DT_ITEM_DATA_LIST_CLI2& stBagItemInfo);
	VOID				GetBagItem(DT_ITEM_DATA_LIST_CLI3& stBagItemInfo);
	VOID				GetBagItem(DT_ITEM_DATA_LIST_CLI4& stBagItemInfo);
    VOID				GetBagItem(DT_ITEM_DATA_CLI2& stDT_ITEM_DATA_CLI2, UINT16 KindID);
	VOID				GetBagItemNum(DT_ITEM_NUM_DATA_LIST_CLI&);
	VOID				GetXiangqianGemList( DT_ITEM_DATA_LIST_CLI2& stBagItemInfo );
	VOID				GetXiangqianGemList( DT_ITEM_DATA_LIST_CLI3& stBagItemInfo );
	VOID				GetDT_EQUIP_MELT_DATA_LST_CLI(DT_EQUIP_MELT_DATA_LST_CLI& stDT_EQUIP_MELT_DATA_LST_CLI);
	VOID				GetDT_GOOD_MELT_DATA_LST_CLI(DT_GOOD_MELT_DATA_LST_CLI &stDT_GOOD_MELT_DATA_LST_CLI );
	UINT8			GetGoodsCellNum() { return m_mapKindID2Goods.Size(); } //获取物品占用格子数
	VOID				GetEquipByPlayerPanel(DT_ITEM_DATA_LIST_CLI3& stItemData);
	VOID				GetEquipByPlayerPanel(DT_ITEM_DATA_LIST_CLI4& stItemData);
	VOID				GetDT_JEWELRY_DATA_LST_CLI(DT_JEWELRY_DATA_LST_CLI& stItemData);
	VOID				GetDT_JEWELRY_DATA_LST_CLI(DT_ITEM_DATA_LIST_CLI3& stItemData);
	VOID				GetAboutJewelryLstCli(DT_ITEM_DATA_LIST_CLI3& stItemData);
	VOID				GetAboutJewelryLstCli(DT_ITEM_DATA_LIST_CLI4& stItemData);
	CID2JewelryShmemMap& GetJewelryMap(){ return m_mapKindID2Jewery; }
	VOID				GetJewelryStrengthInfo(DT_BAG_JEWELRY_STRENGTH_INFO& stIfno);
	//获取装备的最大镶嵌宝石个数。
	UINT8			GetMaxMosaicGemNum();

	//获取是背包否有装备 
	BOOL				HaveEquip();

	
protected:
private:
	UINT8				m_byOpenNum;			//背包信息	
	CID2ItemShmemMap		m_mapItem;			//背包中的道具

	CPos2ItemShmemMap		m_mapItemPos;			//背包中的道具,按顺序保存道具，为了给客户端按顺序下发
	CID2GoodsShmemMap		m_mapKindID2Goods;	//背包中的物品
	CID2EquipShmemMap		m_mapKindID2Equip;	//背包中的装备
	CID2JewelryShmemMap		m_mapKindID2Jewery; //背包中的饰品
	UINT16				m_wItemPos;			//道具存放位置，下发给客户端时按排序下发。该位置不是真正位置，该位置只用于大小排序，中间可能会有空缺
};
DECLARE_CLASS_POOL_MGR(CBag)
#endif

