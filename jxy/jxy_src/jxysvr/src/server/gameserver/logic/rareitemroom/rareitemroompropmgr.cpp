#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapplication.h>
#include "rareitemroompropmgr.h"
#include "logic/item/goods/goodspropmgr.h"

IMPLEMENT_SINGLETON_PROPMGR(CRareItemRoomPropMgr);

BOOL CRareItemRoomPropMgr::Init()
{
	if (FALSE == LoadFromDB())
	{
		return FALSE;
	}

	if (FALSE == LoadUpgradeInfoFromDB())
	{
		return FALSE;
	}

	if ( !LoadRareItemNumberFromDB())
	{
		return FALSE;
	}
	UpdataNumberInfo();
	return  TRUE;
}

VOID CRareItemRoomPropMgr::UnInit()
{
	m_mapRareItemProp.clear();
	m_mapRareItemRoomProp.clear();
	m_mapUpgradeRareItemList.clear();
	m_mapRareItemNumber.clear();
}


BOOL CRareItemRoomPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

    UINT32 dwErrID = 0;
	string strErr;
    
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("rareitemprop", " order by KindID ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    
	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
	    SRareItemProp stProp;
        stProp.dwKindID = static_cast<UINT32>(SGDP::SDAtou(pRes->GetFieldValueByName("KindID")));
		stProp.dwBuyCost = static_cast<UINT32>(SGDP::SDAtou(pRes->GetFieldValueByName("BuyCost")));
		stProp.dwSumDoorsTributeRequire = static_cast<UINT32>(SGDP::SDAtou(pRes->GetFieldValueByName("SumDoorsTributeRequire")));
		stProp.byRareItemType = static_cast<UINT8>(SGDP::SDAtou(pRes->GetFieldValueByName("RareItemType")));
		stProp.byRareItemExchangeRule = static_cast<UINT8>(SGDP::SDAtou(pRes->GetFieldValueByName("RareItemRule")));
		stProp.byBuyNeedLevel = SGDP::SDAtou(pRes->GetFieldValueByName("BuyNeedLevel"));
        m_mapRareItemProp[stProp.dwKindID] = stProp;
        
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
    
	return TRUE;
}

BOOL CRareItemRoomPropMgr::LoadUpgradeInfoFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("rareitemupgradeprop", " order by RareItemRoomGrade ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;
	TCHAR aszRareItemNum[MSG_COL_LEN] = {0};
	TCHAR aszKindID[MSG_COL_LEN] = {0};
	while(pRes->GetRecord()) // 当有行结果时
	{
		SRareItemRoomProp stProp;
		stProp.byRareItemGrade = SGDP::SDAtou(pRes->GetFieldValueByName("RareItemRoomGrade"));
		stProp.qwUnitedFunds = SGDP::SDAtou(pRes->GetFieldValueByName("UnitedFunds"));

		m_mapRareItemRoomProp[stProp.byRareItemGrade] = stProp;

		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CRareItemRoomPropMgr::LoadRareItemNumberFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("rareitemnumberinfo", " order by  RareItemLevel").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;
	while(pRes->GetRecord()) // 当有行结果时
	{
		SRareItemNumberInfo stProp;
		stProp.byRareItemLevel = SGDP::SDAtou(pRes->GetFieldValueByName("RareItemLevel"));
		stProp.wRareItemID = SGDP::SDAtou(pRes->GetFieldValueByName("GoodsID"));
		stProp.dwCurMaxRareItem = SGDP::SDAtou(pRes->GetFieldValueByName("CurMaxGoodsNum"));

		m_mapRareItemNumber.insert(make_pair(stProp.byRareItemLevel, stProp));

		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

UINT8 CRareItemRoomPropMgr::GetRareItemRule(UINT32 dwRareItemID)
{
	SRareItemPropItr itRareItem = m_mapRareItemProp.find(dwRareItemID);
	if (itRareItem == m_mapRareItemProp.end())
	{
		return 0;
	}

	return itRareItem->second.byRareItemExchangeRule;
}

BOOL CRareItemRoomPropMgr::GetConfig(SRareItemProp& stRareItem,UINT32 dwRareItemID)
{
	SRareItemPropItr itRareItem = m_mapRareItemProp.find(dwRareItemID);
	if ( itRareItem == m_mapRareItemProp.end() )
	{
		return FALSE;
	}
	stRareItem = itRareItem->second;
	return TRUE;
}

BOOL CRareItemRoomPropMgr::GetUpgradeInfo(UINT32 dwRareItemGrade, UINT32& dwConsumptionMoney)
{
	SRareItemRoomPropItr itRoomProp = m_mapRareItemRoomProp.find(dwRareItemGrade); //珍品阁升级配置
	if (itRoomProp == m_mapRareItemRoomProp.end())
	{
		return FALSE;
	}
	dwConsumptionMoney = itRoomProp->second.qwUnitedFunds;
	return TRUE;
}

const UpgradeRareItemMap* CRareItemRoomPropMgr::GetRareItemInfoByFaction(UINT32 dwRareItemLevel)
{
	UpgradeRareItemListItr itr = m_mapUpgradeRareItemList.find(dwRareItemLevel);
	if (itr != m_mapUpgradeRareItemList.end())
	{
		return &itr->second;
	}
	return NULL;
}

//分类整理
VOID CRareItemRoomPropMgr::UpdataNumberInfo()
{
	CMapRareItemNumber::reverse_iterator itr = m_mapRareItemNumber.rbegin();
	UINT8 byMaxRareItemLevel = itr->second.byRareItemLevel;	//最高等级
	for ( UINT8 byIndex = 1; byIndex <= byMaxRareItemLevel; byIndex++)
	{
		CMapRareItemNumberPair pairItr = m_mapRareItemNumber.equal_range(byIndex);
		UpgradeRareItemMap mapRareItem;
		for ( CMapRareItemNumberItr tmpItr = pairItr.first; tmpItr != pairItr.second; tmpItr++)
		{
			mapRareItem[tmpItr->second.wRareItemID] = tmpItr->second;
		}
		m_mapUpgradeRareItemList[byIndex] = mapRareItem;
	}
}