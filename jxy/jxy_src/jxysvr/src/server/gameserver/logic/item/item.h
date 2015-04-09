#ifndef _ITEM_H_
#define _ITEM_H_


#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <logic/record/recordmgr.h>

using namespace std ;


/*
enum UINT8
{
	EIK_EQUIP = 1, //装备
	EIK_GOODS = 2, //物品
	EIK_BOX = 3, //宝箱
};
*/

#define EQUIP_BEGIN_KINDID	1	//装备类型ID号 起始
#define EQUIP_END_KINDID	10000	//装备类型ID号 结束

#define JEWELRY_BEGIN_KINDID 10001 //饰品类型ID
#define JEWELRY_END_KINDID 19999 //饰品类型ID

#define GOODS_BEGIN_KINDID	20000	//物品类型ID号 起始
#define GOODS_END_KINDID	0XFFFF	//物品类型ID号 结束


class CPlayer;

class CItem:public CBaseObj
{
public:
	CItem();
	virtual ~CItem();
public:
	virtual UINT8		GetItemKind() = 0;//道具主类型(装备/物品)
	virtual UINT16	GetItemKindID() = 0;//道具子类型
	virtual UINT16	GetCurPileNum() = 0;//当前叠加数
	virtual VOID		AddPileNum(UINT16 wPileNum, CRecordMgr::EAddItemType emOpType) = 0;//添加叠加数
	virtual VOID		DecPileNum(UINT16 wPileNum, CRecordMgr::EDecItemType emOpType, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0) = 0;//删减叠加数	
	virtual UINT16&	GetBagPos() = 0;//获取道具道具存放的背包位置
	virtual BOOL		Init(VOID* pData, CPlayer* poOwner) = 0;//生产道具时Init
	virtual UINT16	Sell(UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI &stRecvItemNumList/*这个用来存放出售后所获得的道具数量*/) = 0;//出售道具
	virtual	void		GetRecoverScience(UINT32 & dwMinScience, UINT32 & dwMaxScience) = 0;//回收器魂

public:
	VOID		SetMemID(UINT64 qwMemID) { m_qwMemID = qwMemID; }
	UINT64	GetMemID() { return m_qwMemID; }
private:
	UINT64	m_qwMemID;//内存ID，便于管理器map管理

private:	
	
};



#endif //#ifndef 

