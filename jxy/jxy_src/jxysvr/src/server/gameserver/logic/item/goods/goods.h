#ifndef _GOODS_H_
#define _GOODS_H_


#include <logic/item/item.h>
#include "goodspropmgr.h"
#include <protocol/server/protocligs.h>


class CGoods:public CItem
{
public:
	CGoods();
	~CGoods();
public:
	virtual UINT8		GetItemKind() { return EIK_GOODS; } ; 
	virtual UINT16	GetItemKindID() { return m_stDT_GOODS_DATA.wKindID; } ;
	virtual UINT16	GetCurPileNum() { return m_stDT_GOODS_DATA.wPileCount; };//当前叠加数，装备/宝箱叠加数为1
	virtual VOID		AddPileNum(UINT16 wPileNum,CRecordMgr::EAddItemType emOpType);//添加叠加数	
	virtual VOID		DecPileNum(UINT16 wPileNum,CRecordMgr::EDecItemType emOpType, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0);//删减叠加数	
	virtual UINT16&	GetBagPos() { return m_stDT_GOODS_DATA.wBagPos; };//获取道具道具存放的背包位置
	virtual BOOL		Init(VOID* pData, CPlayer* poOwner);//生产道具时Init
	virtual UINT16	Sell(UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI &stRecvItemNumList);//出售
	virtual	void		GetRecoverScience(UINT32 & dwMinScience, UINT32 & dwMaxScience);//回收器魂


public:
	const DT_GOODS_DATA&	GetDT_GOODS_DATA() { return m_stDT_GOODS_DATA; }
	UINT16			Use(UINT8& byUseBoxFlag, DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo, UINT16 wUseNum = 1 ); //物品使用
	SGoodsProp*		GetGoodsProp();
	VOID				GetDT_GOODS_DATA_CLI(DT_GOODS_DATA_CLI& stDT_GOODS_DATA_CLI);
	UINT8			GetGoodsMainKindID();
	VOID				GetDT_GOOD_MELT_DATA_CLI(DT_GOOD_MELT_DATA_CLI& DT_GOOD_MELT_DATA_CLI);//获取装备熔炼给客户端的信息

	UINT16			Melt(UINT8 byGoldFlag);//熔练器魂
protected:
	UINT16			UseByMainKind(UINT8& byUseBoxFlag, DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo, UINT16 wUseNum);
	UINT16			UseBox(DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo);
	UINT16			UseRateBox(DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo);
private:
	DT_GOODS_DATA		m_stDT_GOODS_DATA;//物品当前信息

};

#endif
