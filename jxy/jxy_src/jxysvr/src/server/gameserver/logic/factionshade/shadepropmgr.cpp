#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapplication.h>
#include "shadepropmgr.h"
#include <deque>
#include <logic/lua/luamgr.h>
using namespace std;

IMPLEMENT_SINGLETON_PROPMGR(CShadePropMgr);

BOOL CShadePropMgr::Init()
{
	UnInit();
	
	if( !LoadShadeUpgradeProp())
	{
		return FALSE;
	}

	if ( !LoadRewardFromDB())
	{
		return FALSE;
	}

	if ( !LoadMonsterTypePropFromDB())
	{
		return FALSE;
	}
	if ( !LoadHuntingHotName())
	{
		return FALSE;
	}
	m_bHuntingOpenFlag = FALSE;
	m_vecRandom.push_back(0);
	for (UINT32 dwIndex = 1; dwIndex <= 4; dwIndex++)
	{
		m_vecRandom.push_back(m_mapHuntingMonsterProp.find(dwIndex)->second.dwProbability);
	}
	
	return TRUE;
}

VOID CShadePropMgr::UnInit()
{
	m_mapShadeUpgradeProp.clear();
	m_mapHuntingMonsterProp.clear();
	m_multimapHuntingReward.clear();
	m_vecRandom.clear();
}


BOOL CShadePropMgr::LoadRewardFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("huntingrewardprop", " order by MonsterType ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		SHuntingReward stProp;
		stProp.dwMonsterType = SGDP::SDAtou(pRes->GetFieldValueByName("MonsterType"));
		stProp.dwRewardType = SGDP::SDAtou(pRes->GetFieldValueByName("RewardType"));
		stProp.dwRewardValue = SGDP::SDAtou(pRes->GetFieldValueByName("RewardValue"));
		stProp.dwOutProbability = SGDP::SDAtou(pRes->GetFieldValueByName("OutProbability"));
		
		m_multimapHuntingReward.insert(make_pair(stProp.dwMonsterType, stProp));
		byRowNum++;
		if (stProp.dwRewardType == ACTE_DoorsTribute)
		{
			stProp.dwRewardType = ACTE_FactionTaskIntegral;
			stProp.dwRewardValue /= CLuamgr::Instance()->GetDoorsTributeIntegral();
			m_multimapHuntingReward.insert(make_pair(stProp.dwMonsterType, stProp));
		}
		
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CShadePropMgr::LoadMonsterTypePropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("huntingmonsterprobability", " order by MonsterType ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		SHuntingMonsterProp stProp;
		stProp.dwMonsterType = SGDP::SDAtou(pRes->GetFieldValueByName("MonsterType"));
		SDStrcpy( stProp.aszMonsterTypeName,pRes->GetFieldValueByName("MonsterTypeName"));
		stProp.dwProbability = SGDP::SDAtou(pRes->GetFieldValueByName("Probability"));

		m_mapHuntingMonsterProp[stProp.dwMonsterType] = stProp;
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CShadePropMgr::LoadShadeUpgradeProp()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("shadeupgradeprop", " order by ShadeLevel ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		SShadeUpgradeProp stProp;
		stProp.dwShadeLevel = SGDP::SDAtou(pRes->GetFieldValueByName("ShadeLevel"));
		stProp.dwNeedFactionFunds = SGDP::SDAtou(pRes->GetFieldValueByName("NeedFactionFunds"));
		stProp.byHuntingTask = SGDP::SDAtou(pRes->GetFieldValueByName("HuntingMonsterTask"));
		stProp.dwHuntingMonsterMaxNum = SGDP::SDAtou(pRes->GetFieldValueByName("HuntingMonsterMaxNum"));
		stProp.dwHuntingMonsterSchedule = SGDP::SDAtou(pRes->GetFieldValueByName("HuntingMonsterSchedule"));

		m_mapShadeUpgradeProp[stProp.dwShadeLevel] = stProp;
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CShadePropMgr::LoadHuntingHotName()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("huntingpotname", " order by ScheduleLevel ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		SHuntingPotNameProp stProp;
		stProp.byScheduleLevel = SGDP::SDAtou(pRes->GetFieldValueByName("ScheduleLevel"));
		SDStrcpy(stProp.aszHuntingPotName, pRes->GetFieldValueByName("HuntingPotName"));

		m_mapHuntingPotName[stProp.byScheduleLevel] = stProp;
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

TCHAR* CShadePropMgr::GetHuntingPotName(UINT8 byScheduleLevel)
{
	CMapHuntingPotNameItr itr = m_mapHuntingPotName.find(byScheduleLevel);
	if ( itr == m_mapHuntingPotName.end())
	{
		return NULL;
	}
	return itr->second.aszHuntingPotName;
}

BOOL CShadePropMgr::GetShadeUpgradeProp(UINT32 dwShadeLevel, SShadeUpgradeProp& stShadeUpgradeProp)
{
	SShadeUpgradePropItr it = m_mapShadeUpgradeProp.find(dwShadeLevel);
	if (it == m_mapShadeUpgradeProp.end())
	{
		return FALSE;
	}

	stShadeUpgradeProp = it->second;
	return TRUE;
}

UINT32 CShadePropMgr::GetHuntingMaxSchedule(UINT32 dwShadeLevel)
{
	SShadeUpgradePropItr it = m_mapShadeUpgradeProp.find(dwShadeLevel);
	if (it == m_mapShadeUpgradeProp.end())
	{
		return 0;
	}
	return it->second.dwHuntingMonsterSchedule;
}

UINT8 CShadePropMgr::GetMonsterType(UINT8& byLastType, BOOL bChoice)
{
	if (!bChoice)
	{
		byLastType = GetRandomIdxMustHit(m_vecRandom);
	}
	else
	{
		UINT8 byType = 0;
		do 
		{
			byType = GetRandomIdxMustHit(m_vecRandom);
		} while (byLastType < byType );
		byLastType = byType;
	}
	return byLastType;
}

UINT8 CShadePropMgr::GetMonsterType()
{
	return GetRandomIdxMustHit(m_vecRandom);
}

BOOL CShadePropMgr::GetHuntingMonsterProp(UINT32 dwMonsterType, SHuntingMonsterProp& stHuntingMonsterProp)
{
	SHuntingMonsterPropItr it = m_mapHuntingMonsterProp.find(dwMonsterType);
	if ( it == m_mapHuntingMonsterProp.end())
	{
		return FALSE;
	}
	stHuntingMonsterProp = it->second;
	return TRUE;
}