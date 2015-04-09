#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <framework/gsapi.h>
#include "equippropmgr.h"

#include <db/dbmgr.h>

#define SQL_READ_EQUIP_STRING "select * from equipprop order by KindID"


IMPLEMENT_SINGLETON_PROPMGR(CEquipPropMgr);

CEquipPropMgr::CEquipPropMgr()
{
}

CEquipPropMgr::~CEquipPropMgr()
{
}

BOOL CEquipPropMgr::Init()
{
    if(!LoadFromDB())
	{
		return FALSE;
	}

	InitUnlockEquipDressLevel();

	return TRUE;
}

VOID CEquipPropMgr::UnInit()
{
	m_mapEquipProp.clear();
	m_mapUnlockEquipDressLevel.clear();
}

// 从数据源获取数据
BOOL CEquipPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("equipprop", " order by KindID").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
    // 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SEquipProp stEquipProp;
		memset(&stEquipProp, 0, sizeof(SEquipProp));
		stEquipProp.wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
		_SDTStrncpy(stEquipProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX-1);
		//_SDTStrncpy(stEquipProp.szIntroduction, _SDTUTF82T(pRes->GetFieldValueByName("Introduction").c_str()), INTRODUCTION_LEN_MAX);
		stEquipProp.byPosKindID = SGDP::SDAtou(pRes->GetFieldValueByName("PosKindID"));
		stEquipProp.byEquipColor = SGDP::SDAtou(pRes->GetFieldValueByName("EquipColor"));
		stEquipProp.wDressLevel = SGDP::SDAtou(pRes->GetFieldValueByName("DressLevel"));
		stEquipProp.dwBuyCoin = SGDP::SDAtou(pRes->GetFieldValueByName("BuyCoin"));
		stEquipProp.dwBuyGold = SGDP::SDAtou(pRes->GetFieldValueByName("BuyGold"));
		stEquipProp.dwRecoverPrice = SGDP::SDAtou(pRes->GetFieldValueByName("RecoverPrice"));
		stEquipProp.dwHP = SGDP::SDAtou(pRes->GetFieldValueByName("HP"));
		stEquipProp.dwAttack = SGDP::SDAtou(pRes->GetFieldValueByName("Attack"));
        stEquipProp.dwDefend = SGDP::SDAtou(pRes->GetFieldValueByName("Defend"));
		stEquipProp.wStrengthenHPPerLvl = SGDP::SDAtou(pRes->GetFieldValueByName("StrengthenHPPerLvl"));
		stEquipProp.wStrengthenAttackPerLvl = SGDP::SDAtou(pRes->GetFieldValueByName("StrengthenAttackPerLvl"));
		stEquipProp.wUpgradeToEquipID	= SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeToEquipID"));;		
		stEquipProp.wUpgradeSuccessRate = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeSuccessRate"));		//升级成功率
		stEquipProp.wUpgradeSuccessGold = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeSuccessGold"));
		stEquipProp.dwScience = SGDP::SDAtou(pRes->GetFieldValueByName("Science"));
        stEquipProp.dwStrengthenScience = SGDP::SDAtou(pRes->GetFieldValueByName("StrengthenScience"));
        stEquipProp.dwEnchantSciencePerLvl = 0; //SGDP::SDAtou(pRes->GetFieldValueByName("EnchantSciencePerLvl"));
        stEquipProp.wEnchantHPPerLvl = SGDP::SDAtou(pRes->GetFieldValueByName("EnchantHPPerLvl"));
        stEquipProp.wEnchantAttackPerLvl = SGDP::SDAtou(pRes->GetFieldValueByName("EnchantAttackPerLvl"));
        stEquipProp.wEnchantDefendPerLvl = SGDP::SDAtou(pRes->GetFieldValueByName("EnchantDefendPerLvl"));
		stEquipProp.bIsLegendEquip = (stEquipProp.wDressLevel > FLY_LEVEL);
		
		if (0 != stEquipProp.dwBuyCoin && stEquipProp.dwRecoverPrice > stEquipProp.dwBuyCoin)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: CEquipPropMgr RecoverPrice Error!"), MSG_MARK);
			continue;
		}
		m_mapEquipProp[stEquipProp.wKindID] = stEquipProp;
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


VOID CEquipPropMgr::InitUnlockEquipDressLevel()
{	 
	for(CEquipPropMapItr itr = m_mapEquipProp.begin(); itr !=m_mapEquipProp.end(); itr++)
	{
		SEquipProp* pstToEquipProp = GetProp(itr->second.wUpgradeToEquipID);
		if(pstToEquipProp)
		{
			m_mapUnlockEquipDressLevel[pstToEquipProp->wDressLevel] = pstToEquipProp->wDressLevel;
		}		
	}
}


SEquipProp* CEquipPropMgr::GetProp(UINT16 wKindID)
{
    CEquipPropMapItr itr = m_mapEquipProp.find(wKindID);
    if( itr != m_mapEquipProp.end())
    {
        return &(itr->second);
    }
    return NULL;
}

VOID CEquipPropMgr::GetDT_ESTIMATE_EQUIP_DATA(DT_ESTIMATE_EQUIP_DATA & stDT_ESTIMATE_EQUIP_DATA, UINT16 wKindID)
{
	CEquipPropMapItr itr = m_mapEquipProp.find(wKindID);
	if( itr != m_mapEquipProp.end())
	{
		stDT_ESTIMATE_EQUIP_DATA.wKindID		= itr->second.wKindID;
		stDT_ESTIMATE_EQUIP_DATA.wLevel			= itr->second.wDressLevel;
		stDT_ESTIMATE_EQUIP_DATA.wDressLevel	= itr->second.wDressLevel;
		stDT_ESTIMATE_EQUIP_DATA.stBattleAttribute.dwHP = itr->second.dwHP;
		stDT_ESTIMATE_EQUIP_DATA.stBattleAttribute.dwAttack = itr->second.dwAttack;
	}
}

UINT16 CEquipPropMgr::GetUnlockEquipDressLevel(UINT16 wPlayerLevel)
{
	CUnlockEquipDressLevelMapItr itr = m_mapUnlockEquipDressLevel.find(wPlayerLevel);
	if(itr != m_mapUnlockEquipDressLevel.end())
	{
		return itr->second;
	}

	return 0;
}

