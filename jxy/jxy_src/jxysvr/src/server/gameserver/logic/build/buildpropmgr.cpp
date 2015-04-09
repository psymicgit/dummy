#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <logic/item/equip/equippropmgr.h>
#include "buildpropmgr.h"

#include <db/dbmgr.h>
#include <framework/gsapi.h>

#define SQL_READ_BUILD_SORT_STRING "select * from buildsortprop"
#define SQL_READ_MAINTOWNCROWNPROP_STRING "select * from maintowncrownprop order by PlayerLevel asc"
#define SQL_READ_MAINTOWNLEVYPROP_STRING "select * from maintownlevyprop"
#define SQL_READ_SMITHYPROP_STRING "select * from smithyprop order by EquipPos asc "


IMPLEMENT_SINGLETON_PROPMGR(CBuildPropMgr);

CBuildPropMgr::CBuildPropMgr()
{
	memset(&m_stMaintownLevyProp, 0, sizeof(m_stMaintownLevyProp));
}

CBuildPropMgr::~CBuildPropMgr()
{
}

BOOL CBuildPropMgr::Init()
{
	if(!LoadSortPropFromDB())
	{
		return FALSE;
	}
	if(!LoadMaintownCrownPropFromDB())
	{
		return FALSE;
	}
	if(!LoadMaintownLevyPropFromDB())
	{
		return FALSE;
	}
	if(!LoadSmithyPropFromDB())
	{
		return FALSE;
	}	
	if(!InitBuyEquipCliData())
	{
		return FALSE;
	}

	return TRUE;
}

VOID CBuildPropMgr::UnInit()
{
	m_mapBuildKind2Sort.clear();
	m_mapPlayerLevel2CrownCoin.clear();
	m_mapPlayerLevel2CrownGold.clear();
	m_mapPlayerLevel2LevyCoin.clear();
	m_mapPos2EquipID.clear();
	memset(m_astByEquipInfo, 0, sizeof(DT_BUY_EQUIP_DATA) * EQUIP_POS_NUM);
	memset(&m_stMaintownLevyProp, 0, sizeof(m_stMaintownLevyProp));
}


// 从数据源获取数据
BOOL CBuildPropMgr::LoadSortPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("buildsortprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT8 byKindID = SGDP::SDAtou(pRes->GetFieldValueByName("Kind"));
		UINT8 bySort = SGDP::SDAtou(pRes->GetFieldValueByName("Sort"));

		m_mapBuildKind2Sort[byKindID] = bySort;
	}
	pRes->Release();
	pRes = NULL;

	return TRUE;
}

BOOL CBuildPropMgr::LoadMaintownCrownPropFromDB()
{
	return TRUE;//使用lua
	UINT16 dwMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("maintowncrownprop ", "order by PlayerLevel asc").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	UINT16 wLastLevel = 0;
	UINT32 dwCrownCoin = 0;
	UINT16 dwCrownGold = 0;
	UINT32 dwLevyCoin = 0;
	UINT16 wLevelTmp = 0;
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT16 dwPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("PlayerLevel"));
		dwCrownCoin = SGDP::SDAtou(pRes->GetFieldValueByName("CrownCoin"));
		dwCrownGold = SGDP::SDAtou(pRes->GetFieldValueByName("CrownGold"));
		dwLevyCoin = SGDP::SDAtou(pRes->GetFieldValueByName("LevyCoin"));
		m_mapPlayerLevel2CrownCoin[dwPlayerLevel] = dwCrownCoin;
		m_mapPlayerLevel2CrownGold[dwPlayerLevel] = dwCrownGold;
		m_mapPlayerLevel2LevyCoin[dwPlayerLevel] = dwLevyCoin;

		//补全
		wLevelTmp = wLastLevel+1;
		for(; wLevelTmp <= dwPlayerLevel; wLevelTmp++)
		{
			m_mapPlayerLevel2CrownCoin[wLevelTmp] = dwCrownCoin;
		}
		wLevelTmp = wLastLevel+1;
		for(; wLevelTmp <= dwPlayerLevel; wLevelTmp++)
		{
			m_mapPlayerLevel2CrownGold[wLevelTmp] = dwCrownGold;
		}
		wLevelTmp = wLastLevel+1;
		for(; wLevelTmp <= dwPlayerLevel; wLevelTmp++)
		{
			m_mapPlayerLevel2LevyCoin[wLevelTmp] = dwLevyCoin;
		}

		wLastLevel = dwPlayerLevel;
	}
	//补全
	wLevelTmp = wLastLevel+1;
	for(; wLevelTmp <= dwMaxPlayerLevel; wLevelTmp++)
	{
		m_mapPlayerLevel2CrownCoin[wLevelTmp] = dwCrownCoin;
	}
	wLevelTmp = wLastLevel+1;
	for(; wLevelTmp <= dwMaxPlayerLevel; wLevelTmp++)
	{
		m_mapPlayerLevel2CrownGold[wLevelTmp] = dwCrownGold;
	}
	wLevelTmp = wLastLevel+1;
	for(; wLevelTmp <= dwMaxPlayerLevel; wLevelTmp++)
	{
		m_mapPlayerLevel2LevyCoin[wLevelTmp] = dwLevyCoin;
	}
	pRes->Release();
	pRes = NULL;

	return TRUE;
}

BOOL CBuildPropMgr::LoadMaintownLevyPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("maintownlevyprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	if(pRes->GetRecord())// 当有行结果时
	{
		m_stMaintownLevyProp.byLevyNumPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("LevyNumPerDay"));
		m_stMaintownLevyProp.dwLevyCD = SGDP::SDAtou(pRes->GetFieldValueByName("LevyCD"));
	}
	pRes->Release();
	pRes = NULL;

	return TRUE;
}


BOOL CBuildPropMgr::LoadSmithyPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("smithyprop", " order by EquipPos asc").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	UINT8 byEquipPos = 1;
	while(pRes->GetRecord())
	{
		if(byEquipPos  != SGDP::SDAtou(pRes->GetFieldValueByName("EquipPos")))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: EquipPos[%d] no data!"), MSG_MARK, byEquipPos);        
			return FALSE;
		}
		UINT16 wEquipID = SGDP::SDAtou(pRes->GetFieldValueByName("EquipID"));
		SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(wEquipID);
		if(NULL == pstProp)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: pstProp is NULL[EquipID=%d]!"), MSG_MARK, wEquipID);        
			return FALSE;
		}
		if(pstProp->byPosKindID != byEquipPos)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: PosKindID invalid[%d != %d]!"), MSG_MARK, pstProp->byPosKindID, byEquipPos);        
			return FALSE;
		}
		m_mapPos2EquipID[byEquipPos] = wEquipID;
		byEquipPos++;		
	}

	if(m_mapPos2EquipID.size() < 4)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: smithyprop record != 4!"), MSG_MARK);        
		return FALSE;
	}

	pRes->Release();
	pRes = NULL;

	return TRUE;
}


BOOL CBuildPropMgr::InitBuyEquipCliData()
{
	UINT8 byEquipPos = 1;
	UINT8 byIdx = 0;
	for(CPos2EquipIDMapItr itr = m_mapPos2EquipID.begin(); ((itr != m_mapPos2EquipID.end()) && (byIdx < EQUIP_POS_NUM)); itr++)
	{
		DT_BUY_EQUIP_DATA& stByEquipInfo = m_astByEquipInfo[byIdx++];
		UINT16 wEquipID = itr->second;
		stByEquipInfo.wEquipID = wEquipID;
		SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(wEquipID);
		if(NULL == pstProp)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: pstProp is NULL[EquipID=%d]!"), MSG_MARK, wEquipID);        
			return FALSE;
		}
		stByEquipInfo.dwBuyCoin = pstProp->dwBuyCoin;
	}

	return TRUE;
}


UINT8 CBuildPropMgr::GetBuildSort(UINT8 byBuildKindID)
{
	CBuildKind2SortMapItr itr = m_mapBuildKind2Sort.find(byBuildKindID);
	if(itr == m_mapBuildKind2Sort.end())
	{
		return 100;
	}

	return itr->second;
}

UINT32 CBuildPropMgr::GetMaintownCrownCoin(UINT16 wPlayerLevel)
{
	return m_mapPlayerLevel2CrownCoin[wPlayerLevel];
}
UINT32 CBuildPropMgr::GetMaintownCrownGold(UINT16 wPlayerLevel)
{
	return m_mapPlayerLevel2CrownGold[wPlayerLevel];
}
UINT32 CBuildPropMgr::GetMaintownLevyCoin(UINT16 wPlayerLevel)
{
	return m_mapPlayerLevel2LevyCoin[wPlayerLevel];
}
