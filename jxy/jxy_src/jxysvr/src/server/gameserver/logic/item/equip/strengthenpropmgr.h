
#ifndef _STRENGTHENZMONEYMGR_H_
#define _STRENGTHENZMONEYMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <logic/base/basepropmgr.h>
#include <logic/base/basepropmgr.h>
#include "logic/fly/flydef.h"

using namespace std;
using namespace SGDP;


// 强化、附魔配置
class CEquipStrengthenPropMgr : public CBasePopMgr
{
public:
    typedef vector<UINT32> CEquipStrengthenMoneyVec;
    typedef CEquipStrengthenMoneyVec::iterator CEquipStrengthenMoneyVecItr;

	typedef std::map<UINT32, UINT32> CEquipStrengthenMoneyMap; 
	typedef CEquipStrengthenMoneyMap::iterator CEquipStrengthenMoneyMapItr;

    typedef vector<GoodsID> EnchantStoneIDVec;
    typedef std::map<UINT32, UINT32> EnchantStoneMap; 

    DECLARE_SINGLETON_PROPMGR(CEquipStrengthenPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_EQUIP_STRENGTHEN ; }

    BOOL Init();

    VOID UnInit();

    UINT32 GetStrengthenCoin(UINT16 wStrenthenLevel, UINT16 wDressLevel);
	UINT32 GetAllStrengthenCoin(UINT16 wStrenthenLevel, UINT16 wDressLevel);
    UINT16 GetConvertLevel(UINT16 wOldStrenthenLevel, UINT16 wOldDressLevel, UINT16 wNewDressLevel);//返回转换等级
    //返回总CD
    UINT16 GetTotalCD(){ return 0; }; 

    //返回装备强化CD
    UINT16 GetCD(UINT16 wDressLevel, UINT8 Color){ return 0; }

    //返回清除强化CD所需元宝
    UINT32 GetClrStrengthenCDGold(){ return 10; }

    GoodsID GetEnchantStoneID(UINT16 wEnchantLevel);
    UINT32 GetEnchantStone(UINT16 wEnchantLevel, UINT16 wDressLevel);

public:
    CEquipStrengthenPropMgr();
    virtual ~CEquipStrengthenPropMgr();

protected:

    BOOL  InitStrengthenMoney();
    BOOL  InitStrengthenCoin(UINT16 wLevel, UINT16 wDressLevel);

    BOOL  InitEnchantStone();
    BOOL  InitEnchantStoneCost(UINT16 wLevel, UINT16 wDressLevel);


    static BOOL MyCompare(const UINT32 &t1, const UINT32 &t2);
private:
    UINT16							m_wMaxStrengthenLevel;
    CEquipStrengthenMoneyVec		m_vecEquipStrengthenMoney;
	CEquipStrengthenMoneyMap		m_mapEquipStrengthenMoney;

    EnchantStoneIDVec m_vecEnchantStoneID;
    EnchantStoneMap m_mapEnchantStone;
};

#endif // #ifndef _EQUIPPROPMGR_H_
