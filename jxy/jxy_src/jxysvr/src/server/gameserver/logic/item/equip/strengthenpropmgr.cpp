#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/build/buildmgr.h>
#include "strengthenpropmgr.h"
#include <logic/other/singleprammgr.h>
#include <logic/lua/luamgr.h>
#include "sdloggerimpl.h"
IMPLEMENT_SINGLETON_PROPMGR(CEquipStrengthenPropMgr);

CEquipStrengthenPropMgr::CEquipStrengthenPropMgr()
{
	m_wMaxStrengthenLevel = 0;
}

CEquipStrengthenPropMgr::~CEquipStrengthenPropMgr()
{
    m_vecEquipStrengthenMoney.clear();
}

BOOL CEquipStrengthenPropMgr::Init()
{
    return InitStrengthenMoney() && InitEnchantStone();

}

VOID CEquipStrengthenPropMgr::UnInit()
{
	m_wMaxStrengthenLevel = 0;
	m_vecEquipStrengthenMoney.clear();
	m_mapEquipStrengthenMoney.clear();
}

UINT32 CEquipStrengthenPropMgr::GetStrengthenCoin(UINT16 wStrenthenLevel, UINT16 wDressLevel)
{
	UINT32 dwValue = 0;
	try
	{
		dwValue = m_vecEquipStrengthenMoney.at(m_wMaxStrengthenLevel * (wDressLevel - 1) + (wStrenthenLevel - 1));
	}
	catch(...)
	{
		dwValue = 0;
	}
	return dwValue;
}

GoodsID CEquipStrengthenPropMgr::GetEnchantStoneID(UINT16 wEnchantLevel)
{
    if (wEnchantLevel > m_vecEnchantStoneID.size()){
        return m_vecEnchantStoneID.back();
    }

    GoodsID wStoneID = m_vecEnchantStoneID[wEnchantLevel];
    return wStoneID;
}

UINT32 CEquipStrengthenPropMgr::GetEnchantStone(UINT16 wEnchantLevel, UINT16 wDressLevel)
{
    UINT32 dwStone = 0;
    CEquipStrengthenMoneyMapItr itr = m_mapEnchantStone.find(CLogicFunc::Get1616Key(wEnchantLevel, wDressLevel));
    if (itr != m_mapEnchantStone.end()){
        dwStone = itr->second;
    }

    return dwStone;
}

UINT32 CEquipStrengthenPropMgr::GetAllStrengthenCoin(UINT16 wStrenthenLevel, UINT16 wDressLevel)
{
    UINT32 dwValue = 0;
    try
    {
        CEquipStrengthenMoneyMapItr itr = m_mapEquipStrengthenMoney.find(CLogicFunc::Get1616Key(wStrenthenLevel, wDressLevel));
        if (itr != m_mapEquipStrengthenMoney.end())
        {
            dwValue = itr->second;
        }
    }
    catch(...)
    {
        dwValue = 0;
    }
    return dwValue;
}


BOOL CEquipStrengthenPropMgr::InitStrengthenMoney()
{
    m_wMaxStrengthenLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();
    m_vecEquipStrengthenMoney.reserve((m_wMaxStrengthenLevel + 1) * (m_wMaxStrengthenLevel + 1));

    for(UINT16 wDressLevel = 1; wDressLevel <= m_wMaxStrengthenLevel; wDressLevel++)
    {
        for(UINT16 wStrenthenLevel = 1; wStrenthenLevel <= m_wMaxStrengthenLevel; wStrenthenLevel++)
        {
            InitStrengthenCoin(wStrenthenLevel, wDressLevel);
        }
    }
    return TRUE;
}

BOOL CEquipStrengthenPropMgr::InitEnchantStone()
{
    m_vecEnchantStoneID.clear();
    m_mapEnchantStone.clear();

    UINT16 wMaxPlayerLvl = CSinglePramMgr::Instance()->GetMaxPlayerLevel();

    // 附魔石id
    m_vecEnchantStoneID.resize(wMaxPlayerLvl + 1);

    for(UINT16 wLvl = 0; wLvl <= wMaxPlayerLvl; wLvl++){
        GoodsID wStoneID = CLuamgr::Instance()->GetEnchantStoneID(wLvl);
        if (0 == wStoneID){
            SYS_CRITICAL(_SDT("[%s: %d]: CEquipStrengthenPropMgr::InitEnchantStoneID vecMaterialID.size() != vecMaterialNum.size()!"), MSG_MARK);
            return FALSE;
        }

        m_vecEnchantStoneID[wLvl] = wStoneID;
    }

    UINT32 dwStone = 0;

    // 附魔所需附魔石
    for(UINT16 wDressLevel = 1; wDressLevel <= m_wMaxStrengthenLevel; wDressLevel++){
        for(UINT16 wEnchantLevel = 1; wEnchantLevel <= m_wMaxStrengthenLevel; wEnchantLevel++){
            dwStone = CLuamgr::Instance()->GetEnchantStoneCost(wEnchantLevel, wDressLevel);	
            m_mapEnchantStone[CLogicFunc::Get1616Key(wEnchantLevel, wDressLevel)] = dwStone;
        }
    }

    return TRUE;
}

BOOL CEquipStrengthenPropMgr::InitStrengthenCoin(UINT16 wStrenthenLevel, UINT16 wDressLevel)
{
    static UINT16 s_wDressLevel = 0;
    static UINT32 s_dwRecoverCoin = 0;

    if (s_wDressLevel != wDressLevel)
    {
        s_wDressLevel = wDressLevel;
        s_dwRecoverCoin = 0;
    }

    UINT32 dwCoin = CLuamgr::Instance()->GetStrengthenCoin(wStrenthenLevel, wDressLevel);	
    m_vecEquipStrengthenMoney.push_back(dwCoin);
    s_dwRecoverCoin += dwCoin;
    m_mapEquipStrengthenMoney[CLogicFunc::Get1616Key(wStrenthenLevel, wDressLevel)] = s_dwRecoverCoin;
    return TRUE;
}


BOOL CEquipStrengthenPropMgr::MyCompare(const UINT32 &t1, const UINT32 &t2)
{
    if(t1 <= t2)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    return FALSE;
}


UINT16 CEquipStrengthenPropMgr::GetConvertLevel(UINT16 wOldStrenthenLevel, UINT16 wOldDressLevel, UINT16 wNewDressLevel)
{
    if(0 == wOldStrenthenLevel)
    {
        return 0;
    }

    UINT32 dwNewStrengthenCoin = GetAllStrengthenCoin(wOldStrenthenLevel, wOldDressLevel) * 70 / 100;
    CEquipStrengthenMoneyVecItr itr = lower_bound(m_vecEquipStrengthenMoney.begin(), m_vecEquipStrengthenMoney.end(), dwNewStrengthenCoin, MyCompare);
    if(itr != m_vecEquipStrengthenMoney.end())
    {
        if(*itr > dwNewStrengthenCoin)
        {
            UINT16 wStrenthenLevel = *itr;
            wStrenthenLevel = wStrenthenLevel > 1 ? (wStrenthenLevel - 1) : wStrenthenLevel;
            return wStrenthenLevel;
        }
        else
        {
            return *itr;
        }
    }

    return 0;
}

