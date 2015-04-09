#include "hotspringpropmgr.h"
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapplication.h>

IMPLEMENT_SINGLETON_PROPMGR(CHotSpringPropMgr);

BOOL CHotSpringPropMgr::Init()
{
	UnInit();
	if ( !LoadWinePropFromDB())
	{
		return FALSE;
	}

	if ( !LoadGiveWinePropFromDB())
	{
		return FALSE;
	}

	return TRUE;
}

VOID CHotSpringPropMgr::UnInit()
{
	m_mapWineProp.clear();
}

BOOL CHotSpringPropMgr::LoadWinePropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("wineprop", " order by WineID ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		DT_WINE_DATA_CLI stProp;
		stProp.dwWineID = SGDP::SDAtou(pRes->GetFieldValueByName("WineID"));
		stProp.byCostType = SGDP::SDAtou(pRes->GetFieldValueByName("CostType"));
		stProp.dwCostValue = SGDP::SDAtou(pRes->GetFieldValueByName("CostValue"));
		stProp.dwDrinkWineCD = SGDP::SDAtou(pRes->GetFieldValueByName("WineCDValue"));
		stProp.byLeftDrinkNum = SGDP::SDAtou(pRes->GetFieldValueByName("LeftDrinkNum"));
		stProp.byRewardID1 = SGDP::SDAtou(pRes->GetFieldValueByName("RewardID1"));
		stProp.dwRewardValue1 = SGDP::SDAtou(pRes->GetFieldValueByName("RewardValue1"));
		stProp.dwEachRewardValue1 =  stProp.dwRewardValue1 / stProp.byLeftDrinkNum;
		stProp.byRewardID2 = SGDP::SDAtou(pRes->GetFieldValueByName("RewardID2"));
		stProp.dwRewardValue2 = SGDP::SDAtou(pRes->GetFieldValueByName("RewardValue2"));
		stProp.dwEachRewardValue2 = stProp.dwRewardValue2 / stProp.byLeftDrinkNum;
		stProp.byNeedVIPLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedVipLevel"));
		stProp.dwDrinkMaxTime = SGDP::SDAtou( pRes->GetFieldValueByName("LeftDrinkNum") );
		m_mapWineProp[stProp.dwWineID] = stProp;

		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}



UINT32 CHotSpringPropMgr::GetSecond(UINT8 byHour, UINT8 byMin)
{
	UINT32 dwSecond = byHour * 3600 + byMin * 60;
	return dwSecond;
}

BOOL CHotSpringPropMgr::LoadGiveWinePropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("givewineprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
		return FALSE;
	}
	//查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SGiveWineProp stProp;
		stProp.byFactionLevel = SGDP::SDAtou(pRes->GetFieldValueByName("FactionLevel"));
		stProp.byTodayOpenNum = SGDP::SDAtou(pRes->GetFieldValueByName("GiveWineActiviNum"));
		stProp.dwIntegralTime = SGDP::SDAtou(pRes->GetFieldValueByName("IntervalTime"));
		stProp.dwKeepTime = SGDP::SDAtou(pRes->GetFieldValueByName("KeepTime"));
		stProp.dwOpenCost = SGDP::SDAtou(pRes->GetFieldValueByName("OpenCostFunds"));
		m_mapGiveWineProp[stProp.byFactionLevel] = stProp;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CHotSpringPropMgr::GetGiveWinePropByFactionLevel(UINT8 byFactonLevel, SGiveWineProp& stProp)
{
	CMapGiveWinePropItr itr = m_mapGiveWineProp.find(byFactonLevel);
	if (itr == m_mapGiveWineProp.end())
	{
		return FALSE;
	}
	stProp = itr->second;
	return TRUE;
}
