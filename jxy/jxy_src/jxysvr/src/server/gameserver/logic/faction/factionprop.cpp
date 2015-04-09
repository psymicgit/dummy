#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "factionprop.h"
#include <dll/sdframework/sdloggerimpl.h>
#include <common/client/commondef.h>
#include <logic/other/errmsgmgr.h>
#include <logic/vip/vippropmgr.h>
#include <db/dbmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/event/timereventmgr.h>
IMPLEMENT_SINGLETON(CFactorPropMgr);

CFactorPropMgr::CFactorPropMgr()
{
	m_byUnlockLevelByColdPrison = 0;
	m_byUnlockLevelByFactionTower = 0;
	m_byUnlockLevelByGymnasium = 0;
	m_byUnlockLevelByHotSpring = 0;
	m_byUnlockLevelByRareItem = 0;
	m_byUnlockLevelByShade = 0;
	m_mapShadeTaskInfoProp.clear();
	m_mapTaskRewardProp.clear();
}

CFactorPropMgr::~CFactorPropMgr()
{
	m_byUnlockLevelByColdPrison = 0;
	m_byUnlockLevelByFactionTower = 0;
	m_byUnlockLevelByGymnasium = 0;
	m_byUnlockLevelByHotSpring = 0;
	m_byUnlockLevelByRareItem = 0;
	m_byUnlockLevelByShade = 0;
	m_mapShadeTaskInfoProp.clear();
	m_mapTaskRewardProp.clear();
}

BOOL CFactorPropMgr::Init()
{
	if ( !LoadTaskInfoPropFromDB())
	{
		return FALSE;
	}

	if ( !LoadTaskRewardPropFromDB())
	{
		return FALSE;
	}

	if ( !LoadFactionUpgradeFromDB())
	{
		return FALSE;
	}
	if( !LoadDonateRewardProp())
	{
		return FALSE;
	}
	if ( !LoadFactionJobNumProp())
	{
		return FALSE;
	}

	if ( !LoadDoorsTributeProp())
	{
		return FALSE;
	}

	if ( !LoadDoorsTributeLevelProp())
	{
		return FALSE;
	}
	InitEvent();
    return TRUE;
}

VOID CFactorPropMgr::InitEvent()
{
	CTimerEventMgr::Instance()->AddHuntingEvent(TRUE);
	CTimerEventMgr::Instance()->AddHuntingEvent(FALSE);
}

VOID CFactorPropMgr::UnInit()
{
	m_mapShadeTaskInfoProp.clear();
	m_mapTaskRewardProp.clear();
}

BOOL CFactorPropMgr::LoadTaskInfoPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("shadetaskprop", " order by TaskID ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // μ±óDDD?á1?ê±
	{
		SShadeTaskInfoProp stProp;
		stProp.dwTaskID = SGDP::SDAtou(pRes->GetFieldValueByName("TaskID"));
		SDStrcpy(stProp.aszTaskName, pRes->GetFieldValueByName("TaskName"));
		TCHAR aszTimeTmp[5] = {0};
		TCHAR aszTimeHour[3] = {0};
		SDStrcpy(aszTimeTmp, pRes->GetFieldValueByName("TaskBeginTime"));
		SDStrncpy(aszTimeHour, aszTimeTmp, 2);
		stProp.byStartHour = SGDP::SDAtou(aszTimeHour);
		stProp.byStartMin = SGDP::SDAtou(aszTimeTmp+2);
		stProp.wKeepTime = SGDP::SDAtou(pRes->GetFieldValueByName("TaskKeepTime"));
		stProp.dwStartTime = stProp.byStartHour * 60 * 60 + stProp.byStartMin * 60;
		stProp.dwEndTime = stProp.dwStartTime + stProp.wKeepTime;
		SDStrcpy(stProp.aszTaskDesc, pRes->GetFieldValueByName("TaskDescribe"));

		vector<string> vecMsg;
		string strTime;

		TCHAR aszActivi[16] = {0};
		SDSnprintf(aszActivi, 15,"%02u:%02u", stProp.byStartHour, stProp.byStartMin);

		vecMsg.push_back(aszActivi);
		UINT16 wMin = stProp.wKeepTime / 60;
		SDSnprintf(aszActivi, 15,"%02u:%02u", stProp.byStartHour + (wMin / 60) , stProp.byStartMin + (wMin % 60));
		vecMsg.push_back(aszActivi);
		m_strShadeActiviTime = CMsgDefMgr::Instance()->GetErrMsg("SHADE_BEGIN_TIME_MSG", &vecMsg);

		m_mapShadeTaskInfoProp[stProp.dwTaskID] = stProp;

		byRowNum++;


	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
	return TRUE;
}

BOOL CFactorPropMgr::LoadTaskRewardPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("huntingcollectiverewardprop", " order by ShadeLevel, ScheduleLevel ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // μ±óDDD?á1?ê±
	{
		STaskRewardProp stProp;
		stProp.byShadeLevel = SGDP::SDAtou( pRes->GetFieldValueByName("ShadeLevel"));
		stProp.byCollectiveTaskSchedule = SGDP::SDAtou(pRes->GetFieldValueByName("ScheduleLevel"));
		stProp.dwMaxIntegral = SGDP::SDAtou(pRes->GetFieldValueByName("MaxIntegral"));
		stProp.byRewardType = SGDP::SDAtou(pRes->GetFieldValueByName("RewardType"));
		stProp.dwReardValue = SGDP::SDAtou(pRes->GetFieldValueByName("RewardValue"));

		m_mapTaskRewardProp.insert(make_pair(stProp.byShadeLevel, stProp));

		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CFactorPropMgr::GetTaskInfoProp(UINT32 dwTaskID, SShadeTaskInfoProp& stTaskInfo) 
{
	SShadeTaskInfoPropItr it = m_mapShadeTaskInfoProp.find(dwTaskID);
	if (it == m_mapShadeTaskInfoProp.end())
	{
		return FALSE;
	}
	stTaskInfo = it->second;
	return TRUE;
}

BOOL CFactorPropMgr::GetTaskRewardProp( UINT8 byShadeLevel, UINT32 dwSchedule, STaskRewardProp& stTaskRewardProp)
{
	pair<STaskRewardPropItr, STaskRewardPropItr> it = m_mapTaskRewardProp.equal_range(byShadeLevel);
	UINT8 byIndex = 0;
	for (STaskRewardPropItr itIndex = it.first; itIndex != it.second; itIndex++)
	{
		if (itIndex->second.byShadeLevel == byShadeLevel)
		{
			if (itIndex->second.byCollectiveTaskSchedule == dwSchedule)
			{
				stTaskRewardProp.byShadeLevel = itIndex->second.byShadeLevel;
				stTaskRewardProp.byCollectiveTaskSchedule = itIndex->second.byCollectiveTaskSchedule;
				stTaskRewardProp.dwMaxIntegral = itIndex->second.dwMaxIntegral;
				stTaskRewardProp.byRewardType = itIndex->second.byRewardType;
				stTaskRewardProp.dwReardValue = itIndex->second.dwReardValue;
				byIndex++;
				break;
			}
		}
	}
	if (0 == byIndex)
	{
		return FALSE;
	}
	return TRUE;
	
}

//?a±|??ìì×??à?éáìè?μ?′?êy
UINT16	CFactorPropMgr::GetGoldMaxTimeEveryDay(UINT32 dwVIPLevel)
{
    if ( 0 == dwVIPLevel)
    {
        return CVipPropMgr::Instance()->GetIncNum(EVINF_GOLD_DONATE);
    }
    else
    {
        return CVipPropMgr::Instance()->GetIncNum(EVINF_GOLD_DONATE, dwVIPLevel);
    }
}

BOOL CFactorPropMgr::LoadFactionUpgradeFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("factionupgradeprop", " order by FactionLevel ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // μ±óDDD?á1?ê±
	{
		SFactionUpgradeProp stProp;
		stProp.dwFactionLevel = SGDP::SDAtou( pRes->GetFieldValueByName("FactionLevel"));
		stProp.dwNeedFactionFunds = SGDP::SDAtou(pRes->GetFieldValueByName("NeedFactionFunds"));
		stProp.dwFactionPlayerMaxNum = SGDP::SDAtou(pRes->GetFieldValueByName("FactionPlayerMax"));
		stProp.byRareitemMark = SGDP::SDAtou(pRes->GetFieldValueByName("RareitemMark"));
		stProp.byGymnasiumMark = SGDP::SDAtou(pRes->GetFieldValueByName("GymnasiumMark"));
		stProp.byShadeMark = SGDP::SDAtou(pRes->GetFieldValueByName("ShadeMark"));
		stProp.byHotSpringMark = SGDP::SDAtou(pRes->GetFieldValueByName("HotSpringMark"));
		stProp.byColdPrison = SGDP::SDAtou(pRes->GetFieldValueByName("ColdPrison"));
		stProp.byFactionTower = SGDP::SDAtou(pRes->GetFieldValueByName("FactionTower"));
		stProp.byCoinTimeByDay = SGDP::SDAtou(pRes->GetFieldValueByName("CoinMaxTimeEveryDay"));
		
		if ( 0 != stProp.byRareitemMark && 0 == m_byUnlockLevelByRareItem)
		{
			m_byUnlockLevelByRareItem = stProp.dwFactionLevel;
		}
		if ( 0 != stProp.byGymnasiumMark && 0 == m_byUnlockLevelByGymnasium)
		{
			m_byUnlockLevelByGymnasium = stProp.dwFactionLevel;
		}
		if ( 0 != stProp.byShadeMark && 0 == m_byUnlockLevelByShade)
		{
			m_byUnlockLevelByShade = stProp.dwFactionLevel;
		}
		if ( 0 != stProp.byHotSpringMark && 0 == m_byUnlockLevelByHotSpring)
		{
			m_byUnlockLevelByHotSpring = stProp.dwFactionLevel;
		}
		if ( 0 != stProp.byColdPrison && 0 == m_byUnlockLevelByColdPrison)
		{
			m_byUnlockLevelByColdPrison = stProp.dwFactionLevel;
		}
		if ( 0 != stProp.byFactionTower && 0 == m_byUnlockLevelByFactionTower)
		{
			m_byUnlockLevelByFactionTower = stProp.dwFactionLevel;
		}
		m_mapFactionUpgradeProp[stProp.dwFactionLevel] = stProp;
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

UINT32 CFactorPropMgr::GetFactionPlayerMaxNum( UINT32 dwFactionLevel)
{
	SFactionUpgradePropItr it = m_mapFactionUpgradeProp.find(dwFactionLevel);
	if (it != m_mapFactionUpgradeProp.end())
	{
		return it->second.dwFactionPlayerMaxNum;
	}
	return 0;
}
BOOL CFactorPropMgr::GetFactionUpgradeProp(UINT32 dwFactionLevel, SFactionUpgradeProp& stFactionUpgrade)
{
	SFactionUpgradePropItr it = m_mapFactionUpgradeProp.find(dwFactionLevel);
	if ( it == m_mapFactionUpgradeProp.end())
	{
		return FALSE;
	}

	stFactionUpgrade = it->second;
	return TRUE;
}

UINT16 CFactorPropMgr::GetCoinMaxTimeEveryDay(UINT32 dwFactionLevel)
{
	SFactionUpgradePropItr it = m_mapFactionUpgradeProp.find(dwFactionLevel);
	if ( it == m_mapFactionUpgradeProp.end())
	{
		return 0;
	}

	return it->second.byCoinTimeByDay;
}

BOOL CFactorPropMgr::LoadDonateRewardProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("donaterewardprop", " order by FactionLevel, RewardType ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // μ±óDDD?á1?ê±
	{
		SDonateRewardProp stProp;
		stProp.byFactionLevel = SGDP::SDAtou( pRes->GetFieldValueByName("FactionLevel"));
		stProp.byRewardType = SGDP::SDAtou(pRes->GetFieldValueByName("RewardType"));
		stProp.dwRewardValue = SGDP::SDAtou(pRes->GetFieldValueByName("RewardValue"));
		stProp.byNum = SGDP::SDAtou(pRes->GetFieldValueByName("Num"));
		m_mapDonateReward.insert(make_pair(stProp.byFactionLevel, stProp));
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}


BOOL CFactorPropMgr::LoadFactionJobNumProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("factionjobnumprop", " order by FactionLevel ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // μ±óDDD?á1?ê±
	{
		SFactinJobNumProp stProp;
		stProp.byFactionLevel = SGDP::SDAtou( pRes->GetFieldValueByName("FactionLevel"));
		stProp.byMartialHead = SGDP::SDAtou(pRes->GetFieldValueByName("MartialHead"));
		stProp.byProxyMartialHead = SGDP::SDAtou(pRes->GetFieldValueByName("ProxyMartialHead"));
		stProp.byElder = SGDP::SDAtou(pRes->GetFieldValueByName("Elder"));
		stProp.byCustodian = SGDP::SDAtou(pRes->GetFieldValueByName("Custodian"));
		stProp.byElite = SGDP::SDAtou(pRes->GetFieldValueByName("Elite"));
		stProp.byNormal = SGDP::SDAtou(pRes->GetFieldValueByName("Normal"));
		m_mapFactionJobNum[stProp.byFactionLevel] = stProp;
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CFactorPropMgr::LoadDoorsTributeProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("doorstributeprop", " order by DoorsTributeLevel ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // μ±óDDD?á1?ê±
	{
		SDoorsTributeProp stProp;
		stProp.wDoorsTributeLevel = SGDP::SDAtou( pRes->GetFieldValueByName("DoorsTributeLevel"));
		stProp.qwDoorsTributeValue = SGDP::SDAtou64(pRes->GetFieldValueByName("DoorsTributeValue"));
	
		m_mapDoorsTributeProp[stProp.qwDoorsTributeValue] = stProp.wDoorsTributeLevel;
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CFactorPropMgr::LoadDoorsTributeLevelProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("doorstributelevelprop", " order by IconType ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // μ±óDDD?á1?ê±
	{
		
		UINT8 byIconType = SGDP::SDAtou( pRes->GetFieldValueByName("IconType"));
		UINT8 byIconLevel = SGDP::SDAtou(pRes->GetFieldValueByName("IconLevel"));

		m_mapDoorsTributeLevelProp[byIconType] = byIconLevel;
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}
BOOL CFactorPropMgr::GetFactionJobNum(UINT8 byFactionLevel, SFactinJobNumProp& stProp)
{
	CMapFactionJobNumItr itr = m_mapFactionJobNum.find(byFactionLevel);
	if (itr == m_mapFactionJobNum.end())
	{
		return FALSE;
	}
	stProp = itr->second;
	return TRUE;
}

UINT8 CFactorPropMgr::GetUnLockLevel(em_Faction_Build emFactionType)
{
	switch(emFactionType)
	{
	case EFB_Rareitem:
		return m_byUnlockLevelByRareItem;
		break;
	case EFB_Gymnasium:
		return m_byUnlockLevelByGymnasium;
		break;
	case EFB_Shade:
		return m_byUnlockLevelByShade;
		break;
	case EFB_HotSpring:
		return m_byUnlockLevelByHotSpring;
		break;
	case EFB_ColdPrison:
		return m_byUnlockLevelByColdPrison;
		break;
	case EFB_FactionTower:
		return m_byUnlockLevelByFactionTower;
		break;
	default:
		return 0;
	}
}
VOID CFactorPropMgr::GetDoorsTributeInfo(UINT64 qwValue, UINT16& wLevel, UINT32& dwNextValue)
{
	CMapDoorsTributePropItr itr = m_mapDoorsTributeProp.lower_bound(qwValue);
	if ( m_mapDoorsTributeProp.end() == itr)//满级
	{
		itr--;
		wLevel = itr->second;
		dwNextValue = 0;
		return ;
	}

	if ( itr->first != qwValue)//不在临界值
	{
		wLevel = itr->second - 1;
		dwNextValue = static_cast<UINT32>(itr->first - qwValue);
	}
	else//在临界值
	{
		wLevel = itr->second;
		dwNextValue = 0;
		itr++;
		if ( itr != m_mapDoorsTributeProp.end())
		{
			dwNextValue = static_cast<UINT32>(itr->first - qwValue);;
		}
	}
}

VOID CFactorPropMgr::GetDoorsTributeInfo(UINT64 qwDoorsTribute, DT_DOORSTRIBUTE_LEVEL_DATA& stLevelData)
{
	memset(&stLevelData, 0, sizeof(DT_DOORSTRIBUTE_LEVEL_DATA));
	if ( 0 == qwDoorsTribute )
	{
		return ;
	}
	UINT32 dwNextUpgradeValue = 0;
	UINT16 wLevel = 0;
	GetDoorsTributeInfo(qwDoorsTribute, wLevel, dwNextUpgradeValue);
	stLevelData.wDoorsTributeLevel = wLevel;
	//皇冠
	UINT8 byLevelBase = m_mapDoorsTributeLevelProp[ICON_CROWN];
	stLevelData.byCrownNum = wLevel / byLevelBase;
	wLevel -= (byLevelBase * stLevelData.byCrownNum);
	//太阳
	byLevelBase = m_mapDoorsTributeLevelProp[ICON_SUN];
	stLevelData.bySunNum = wLevel / byLevelBase;
	wLevel -= (byLevelBase * stLevelData.bySunNum);
	//月亮
	byLevelBase = m_mapDoorsTributeLevelProp[ICON_MOON];
	stLevelData.byMoonNum = wLevel / byLevelBase;
	wLevel -= (byLevelBase * stLevelData.byMoonNum);
	//星星
	byLevelBase = m_mapDoorsTributeLevelProp[ICON_STAR];
	stLevelData.byStarNum = wLevel / byLevelBase;
	wLevel -= (byLevelBase * stLevelData.byStarNum);

	stLevelData.dwNextUpgradeNeed = dwNextUpgradeValue;
}
