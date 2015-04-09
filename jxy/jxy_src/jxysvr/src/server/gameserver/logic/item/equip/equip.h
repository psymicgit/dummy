#ifndef _EQUIP_H_
#define _EQUIP_H_

#include <logic/item/item.h>
#include "equippropmgr.h"
#include <logic/battle/battleattrobj.h>
#include "logic/base/basedefine.h"
#include "logic/fly/flydef.h"

struct SEquipProperty
{
    SEquipProperty(){ memset(this, 0, sizeof(*this)); }

    DT_EQUIP_DATA *pstEquip;  // 装备数据
    UINT16 wEnchantLevel; // 附魔等级
};

class CHero;
class CEquip:public CItem
{
public:
    typedef std::map<GoodsID, UINT32> EnchantStone2NumMap; // <附魔石ID，对应数量>

public:
	CEquip();
	~CEquip();
public:
	virtual UINT8		GetItemKind() { return EIK_EQUIP; } ; 
    virtual UINT16	GetItemKindID() { return m_stDT_EQUIP_DATA.wKindID; }
	virtual UINT16	GetCurPileNum() { return 1; };//当前叠加数，装备叠加数为1
	virtual VOID		AddPileNum(UINT16 wPileNum, CRecordMgr::EAddItemType emOpType) {};//装备叠加数只能是1，不能叠加
	//删减叠加数	
	virtual VOID		DecPileNum(UINT16 wPileNum, CRecordMgr::EDecItemType emOpType, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0){};
	virtual UINT16&	GetBagPos() { return m_stDT_EQUIP_DATA.wBagPos; };//获取道具道具存放的背包位置
	virtual BOOL		Init(VOID* pData, CPlayer* poOwner);//生产道具时Init
	virtual UINT16	Sell(UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI &stRecvItemNumList);//出售

    UINT16	GetEquipIdx() { return m_stDT_EQUIP_DATA.wIdx; }

	VOID ClearJewelryAttr();
	VOID AddJewelryAttribute(UINT32 dwKey, UINT32 dwValue);
	VOID GetJewelryAttribute(UINT32& dwHP, UINT32& dwAttack);

public:	
	VOID		SetHero(CHero* poHero); //装备/卸下必须调用该方法
	CHero*	GetHero() { return m_poHero; } //装备/卸下必须调用该方法
	INT16	Strengthen(UINT8 byStrengthenType, UINT16 wLevel);//强化

    INT16	Enchant(UINT16 wTargetEnchantLevel);//附魔到指定等级

	//UINT16  OpenLight( , ); // 装备开光 
	BOOL		Xiangqian( UINT16 wItemID, UINT8 byPos );
	UINT8	GetMosaicGemNum();

	DT_EQUIP_DATA&			GetDT_EQUIP_DATA() { return m_stDT_EQUIP_DATA; }
	SEquipProp*				GetEquipProp();
	VOID						InitBattleAttribute();//给m_stBattleAttribute赋值
	const DT_BATTLE_ATTRIBUTE2& GetBattleAttr() { return m_stBattleAttribute; }
	static DT_BATTLE_ATTRIBUTE	BattleAttCountByLevel(UINT16 wLevel, UINT16 wEnchantLevel, SEquipProp* pstEquipProp); //等级统计战斗属性，根据等级计算出该等级对应的战斗属性

    static DT_BATTLE_ATTRIBUTE2	CalcAttrByLevel(UINT16 wLevel, UINT16 wEnchantLevel/* 附魔等级 */, SEquipProp* pstEquipProp); // 根据强化等级和附魔等级计算出该等级对应的战斗属性

    UINT16 GetEnchantLevel(){ return m_wEnchantLevel; }
    VOID SetEnchantLevel(UINT16 wEnchantLevel){ m_wEnchantLevel = wEnchantLevel; }
    UINT16 GetLevel(){ return m_stDT_EQUIP_DATA.wLevel; }

    // 本装备当前是否可被附魔（不考虑附魔石是否足够）
    BOOL CanBeEnchant();

	UINT32	GetRecoverPrice();//回收价格,todo,成员变量存储，不需每次计算，由变化再算
	UINT16	GetUpgradeDressLevel();//获取该装备可升级到的装备等级
	
	UINT16	Melt(UINT8 byGoldFlag);//熔练器魂
	VOID 	GetRecoverScience(UINT32 &dwMinScience, UINT32 &dwMaxScience);//回收器魂
	static VOID	GetRecoverScience(UINT16 wKindID, UINT32 & dwMinScience, UINT32 & dwMaxScience);//回收器魂

	VOID GetDT_EQUIP_DATA_CLI(DT_EQUIP_DATA_CLI& stDT_EQUIP_DATA_CLI);//获取装备给客户端的信息
    VOID GetDT_EQUIP_DATA_CLI_v410(DT_EQUIP_DATA_CLI_v410&);//获取装备给客户端的信息
    VOID GetDT_EQUIP_DATA_STRENGTHEN_CLI(DT_EQUIP_DATA_STRENGTHEN_CLI& stDT_EQUIP_DATA_CLI);//获取装备给客户端的信息
	VOID GetDT_EQUIP_DATA_ENCHANT_CLI(DT_EQUIP_ENCHANT_DATA_CLI&);//获取装备给客户端的信息
	VOID GetDT_EQUIP_DATA_COMPOSE_CLI(DT_EQUIP_COMPOSE_DATA_CLI& stDT_EQUIP_DATA_CLI);//获取装备合成的信息
	VOID GetDT_ENEMY_EQUIP_DATA(DT_ENEMY_EQUIP_DATA& stDT_ENEMY_EQUIP_DATA);//获取装备给客户端的信息
	VOID GetDT_EQUIP_MELT_DATA_CLI(DT_EQUIP_MELT_DATA_CLI& stDT_EQUIP_MELT_DATA_CLI);//获取装备熔炼给客户端的信息
	UINT32 GetUpLevelCoin(UINT8 byUpLevel);//获得升级返回等级铜钱

    VOID GetDT_EQUIP_DATA_CLI_with_enchant_lvl(DT_EQUIP_DATA_CLI& stDT_EQUIP_DATA_CLI);//获取装备给客户端的信息，装备里面携带附魔等级信息


    EnchantStone2NumMap GetSellEnchantStone(); // 获得出售时所返还的附魔石

	static UINT32 GetUpLevelCoin(UINT16 wCurStrengthenLevel, UINT8 byUpLevel, UINT16 wDressLevel, UINT16 wPlayerLevel);//获得升级返回等级铜钱

protected:
	UINT32 GetStrengthenCost(UINT8 byStrengthenType, UINT16 wLevel);//获取强化需消耗的金钱
    UINT16 GetEnchantCostStone(UINT16 wEnchantLevel);//获取附魔需消耗的附魔石
    UINT16 GetEnchantStoneID(UINT16 wEnchantLevel);//获取附魔需消耗的附魔石

    VOID GetBattleAttribute(DT_BATTLE_ATTRIBUTE& stBattleAttribute);//获取装备战斗属性
	VOID GetBattleAttribute2(DT_BATTLE_ATTRIBUTE2& stBattleAttribute);//获取装备战斗属性

    VOID GetBasicAttr(DT_BATTLE_ATTRIBUTE2& stAddAttr); //获取强化附加属性
    VOID GetAttrAddByStrengthen(DT_BATTLE_ATTRIBUTE2& stAddAttr); //获取强化附加属性
    VOID GetAttrAddByEnchant(DT_BATTLE_ATTRIBUTE2& stAddAttr); //获取附魔附加属性

	VOID GetNextLevelBattleAttr(DT_BATTLE_ATTRIBUTE& stBattleAttribute);//获取装备下一级的战斗属性
    VOID GetNextEnchantLevelBattleAttr(DT_BATTLE_ATTRIBUTE2& stBattleAttribute);//获取附魔到下一等级的属性

    VOID GetStrengthenInfo(UINT8& byCanStrengthenFlag, UINT32& dwStrengthenCoin, UINT32& dwStrengthenGold, UINT16& wStrengthenRate);//获取强化信息
	
private:
	DT_EQUIP_DATA			m_stDT_EQUIP_DATA; //装备信息
    UINT16 m_wEnchantLevel; // 附魔等级
	DT_BATTLE_ATTRIBUTE2 m_stBattleAttribute;//战斗属性
	CHero*				m_poHero;//若未装备，则为NULL
	C3232Map            m_mapJewelryAttr;
};

#endif

