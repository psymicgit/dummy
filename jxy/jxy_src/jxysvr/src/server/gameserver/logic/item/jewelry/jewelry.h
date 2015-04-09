#ifndef _JEWELRY_H_
#define _JEWELRY_H_

#include <logic/item/item.h>
#include <protocol/server/protocligs.h>

class CHero;
struct SJewelryProp;
struct SJewelryUpgradeProp;

class CJewelry : public CItem
{
public: 
	CJewelry();
	~CJewelry();
public:
	
	CHero* GetHero(){ return m_poHero; }
	VOID SetHero(CHero* poHero);

	virtual UINT8		GetItemKind() { return EIK_JEWELRY; };//道具主类型(装备/物品)
	virtual UINT16	GetItemKindID() { return m_stDT_JEWELRY_DATA.wJewelryID; }//道具子类型
	virtual UINT16	GetCurPileNum(){ return 1;} //当前叠加数}
	virtual VOID		AddPileNum(UINT16 wPileNum, CRecordMgr::EAddItemType emOpType) {};//添加叠加数
	virtual VOID		DecPileNum(UINT16 wPileNum, CRecordMgr::EDecItemType emOpType, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0) {};//删减叠加数	
	virtual UINT16&	GetBagPos();//获取道具道具存放的背包位置
	virtual BOOL		Init(VOID* pData, CPlayer* poOwner);//生产道具时Init
	virtual UINT16	Sell(UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI &stRecvItemNumList) ;//出售道具
	virtual	void		GetRecoverScience(UINT32 & dwMinScience, UINT32 & dwMaxScience) ;//回收器魂
	DT_JEWELRY_DATA& GetDT_JEWELRY_DATA();

	SJewelryProp* GetProp();
	SJewelryUpgradeProp* GetUpgradeProp();
	UINT8 GetColorType();	//获取饰品品质
	UINT16 GetStrengLevel();	//获取强化等级
	UINT16 GetDressLevel();	//获取穿戴等级
	UINT8 GetExtStrengLeftNum();	//剩余强化次数
	BOOL IsCanUpgrade();
	BOOL IsMaxLevel();

    UINT16	GetJewelryIdx() { return m_stDT_JEWELRY_DATA.wIdx; }

	BOOL UnDressBag();
	BOOL DressHero(CHero* poHero);

public:
    static BOOL IsOneHeroAttr(JewelryExtAttr);
    static BOOL IsAllHeroAttr(JewelryExtAttr);

public:
	UINT16 OnStrengExt(UINT8 byExtID, PKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK& stAck);
	UINT16 OnUpgrade(PKT_CLIGS_JEWELRY_STRENGTH_ACK& stAck);
	UINT16 OnOpenJewelryResolve(PKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK& stAck);
public:
	VOID GetJewelryDataCli(DT_JEWELRY_DATA_CLI& stJewelryCli);	//获取给客户端数据
	DT_BATTLE_ATTR_LST_INFO& GetBaseAttribute();
	UINT8 ResolveJewelry(DT_GOODS_DATA_LST_CLI& stGoodsCli, UINT32& dwRes);

	BOOL GetJewelryStrength(DT_JEWELRY_DATA_CLI& stCurInfo, DT_JEWELRY_DATA_CLI& stStrengthInfo);
	BOOL GetStrengItemInfo( DT_STRENGTH_INFO& stItemInfo);
protected:
	BOOL UpgradeJewelry();	//升级饰品
private:
	DT_JEWELRY_DATA		m_stDT_JEWELRY_DATA;//饰品当前信息
	CHero*				m_poHero;//若未装备，则为NULL
};

#endif