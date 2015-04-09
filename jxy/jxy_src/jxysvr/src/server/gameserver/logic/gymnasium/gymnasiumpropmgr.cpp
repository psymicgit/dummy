#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapplication.h>
#include "gymnasiumpropmgr.h"
#include "logic/item/goods/goodspropmgr.h"


IMPLEMENT_SINGLETON_PROPMGR(CGymnasiumPropMgr);

BOOL CGymnasiumPropMgr::Init()
{
	if ( !LoadPropFromDB())
	{
		return FALSE;
	}
	if ( !LoadInfoFromDB())
	{
		return FALSE;
	}
	if ( !LoadUpgradeFromDB())
	{
		return FALSE;
	}
	return TRUE;
}

VOID CGymnasiumPropMgr::UnInit()
{
	m_multiMapSkillProp.clear();
	m_mapGymnasiumUpgradeProp.clear();
	m_mapSkillInfo.clear();
}


BOOL CGymnasiumPropMgr::LoadPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("gymnasiumskillprop", " order by SkillKindID ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		SSkillProp stProp;
		stProp.bySkillKindID = SGDP::SDAtou(pRes->GetFieldValueByName("SkillKindID"));
		stProp.SkillLevel = SGDP::SDAtou(pRes->GetFieldValueByName("SkillLevel"));
		stProp.SkillValue = SGDP::SDAtou(pRes->GetFieldValueByName("SkillValue"));
		stProp.LearnDoorsTribute = SGDP::SDAtou(pRes->GetFieldValueByName("LearnDoorsTribute"));
		stProp.dwSumDoorsTribute = SGDP::SDAtou(pRes->GetFieldValueByName("SumDoorsTribute"));
		m_multiMapSkillProp.insert(make_pair(stProp.bySkillKindID, stProp));

		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CGymnasiumPropMgr::LoadInfoFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("gymnasiumskillinfo", " order by SkillKindID ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		SSkillInfo stProp;
		stProp.bySkillKindID = SGDP::SDAtou(pRes->GetFieldValueByName("SkillKindID"));
		SDStrcpy(stProp.SKillName, pRes->GetFieldValueByName("SkillName"));
		stProp.byUnlockLevelByGymnasium = SGDP::SDAtou(pRes->GetFieldValueByName("UnLockLevelByGymnasium"));
		stProp.byDisplayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("DisplayByGymnasium"));
		m_mapSkillInfo[stProp.bySkillKindID] = stProp;
		m_mapSkillInfoByUnLock.insert(make_pair(stProp.byUnlockLevelByGymnasium, stProp));
		byRowNum++;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CGymnasiumPropMgr::GetSkillInfoProp(SSkillProp& stSkillProp, UINT8 bySkillID, UINT32 dwSkillLevel)
{
	SkillPropPair skillPair = m_multiMapSkillProp.equal_range(bySkillID);
	for ( SkillPropMultiItr it = skillPair.first; it != skillPair.second; it++)
	{
		if (it->second.SkillLevel == dwSkillLevel)
		{
			stSkillProp.bySkillKindID = it->second.bySkillKindID;
			stSkillProp.SkillLevel = dwSkillLevel;
			stSkillProp.SkillValue = it->second.SkillValue;
			stSkillProp.LearnDoorsTribute = it->second.LearnDoorsTribute;
			stSkillProp.dwSumDoorsTribute = it->second.dwSumDoorsTribute;
			return TRUE;
		}
	}
	return FALSE;
}

UINT8 CGymnasiumPropMgr::GetUnlockSkillLevel(UINT8 bySkillID)
{
	SkillInfoItr it = m_mapSkillInfo.find(bySkillID);
	if ( it == m_mapSkillInfo.end())
	{
		return 0;
	}
	return it->second.byUnlockLevelByGymnasium;
}

TCHAR* CGymnasiumPropMgr::GetSkillName(UINT32 dwSkillKindID)
{
	SkillInfoItr it = m_mapSkillInfo.find(dwSkillKindID);
	if ( it == m_mapSkillInfo.end())
	{
		return NULL;
	}
	return it->second.SKillName;
}

UINT32 CGymnasiumPropMgr::GetSkillNum()
{
	return m_mapSkillInfo.size();
}

BOOL CGymnasiumPropMgr::LoadUpgradeFromDB()
{
	SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;

	UINT32 dwErrID = 0;
	string strErr;

	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("gymnasiumupgradeprop", " order by GymnasiumLevel ").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	UINT8 byRowNum = 0;

	while(pRes->GetRecord()) // 当有行结果时
	{
		SGymnasiumUpgradeProp stProp;
		stProp.dwGymnasiumLevel = SGDP::SDAtou(pRes->GetFieldValueByName("GymnasiumLevel"));
		stProp.dwNeedFactionFunds = SGDP::SDAtou(pRes->GetFieldValueByName("NeedFactionFunds"));
		stProp.byCurMaxLevel = SGDP::SDAtou(pRes->GetFieldValueByName("SkillMaxLevel"));
		m_mapGymnasiumUpgradeProp[stProp.dwGymnasiumLevel] = stProp;
		byRowNum++;
	}
	SGymnasiumUpgradePropMap::reverse_iterator it = m_mapGymnasiumUpgradeProp.rbegin();
	m_dwMaxSkillLevel = it->second.byCurMaxLevel;

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL  CGymnasiumPropMgr::GetGymnasiumUpgradeProp(SGymnasiumUpgradeProp& stGymnasiumUpgradeProp, UINT32 dwGymnasiumLevel)
{
	SGymnasiumUpgradePropItr it = m_mapGymnasiumUpgradeProp.find(dwGymnasiumLevel);
	if ( it == m_mapGymnasiumUpgradeProp.end())
	{
		return FALSE;
	}
	stGymnasiumUpgradeProp = it->second;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID CGymnasiumPropMgr::AddDiffAttribute(UINT8 byKindID, UINT32 dwSkillLevel ,UINT32& dwAddAttrValue)
{
	if (dwSkillLevel == 0)
	{
		dwAddAttrValue = 0;
		return ;
	}
	SkillPropPair skillPair = m_multiMapSkillProp.equal_range(byKindID);
	UINT32 dwPreValue = 0;
	UINT32 dwCurValue = 0;
	BOOL bRet = FALSE;
	for ( SkillPropMultiItr it = skillPair.first; it != skillPair.second; it++)
	{
		if (it->second.SkillLevel == dwSkillLevel - 1)
		{
			dwPreValue = it->second.SkillValue;
		}
		if (it->second.SkillLevel == dwSkillLevel)
		{
			dwCurValue = it->second.SkillValue;
			bRet = TRUE;
			return ;
		}
	}
	if (bRet)
	{
		dwAddAttrValue = dwCurValue - dwPreValue;
	}
	else
	{
		dwAddAttrValue = 0;
	}
}


UINT8 CGymnasiumPropMgr::GetCurMaxSkillLevel(UINT32 dwGymnasiumLevel)
{
	SGymnasiumUpgradePropItr it = m_mapGymnasiumUpgradeProp.find(dwGymnasiumLevel);
	if ( it == m_mapGymnasiumUpgradeProp.end())
	{
		return 0;
	}
	return it->second.byCurMaxLevel;
}

CMapFactionSkill CGymnasiumPropMgr::GetFactionSkill(UINT32 dwGymnasiumLevel)
{
	SFactionSkill stSkill = {0};
	CMapFactionSkill mapFactionSkill;
	UINT8 bySkillCurMax = GetCurMaxSkillLevel(dwGymnasiumLevel);
	for(SkillInfoItr it = m_mapSkillInfo.begin(); it != m_mapSkillInfo.end(); it++)
	{
		if (it->second.byDisplayerLevel <= dwGymnasiumLevel)
		{
			memset( &stSkill, 0, sizeof(SFactionSkill));
			stSkill.bySkillID = it->second.bySkillKindID;
			SDStrncpy(stSkill.aszSkillName, it->second.SKillName, SKILL_NAME_LEN - 1);
			stSkill.byUnlockFlag = dwGymnasiumLevel < it->second.byUnlockLevelByGymnasium ? ESM_NO : ESM_YES;
			stSkill.bySkillCurMax = bySkillCurMax;
			mapFactionSkill[stSkill.bySkillID] = stSkill;
		}
		
	}
	return mapFactionSkill;
}

BOOL CGymnasiumPropMgr::AddAttribute(UINT8 byKindID, UINT32 dwSkillLevel ,UINT32& dwAddAttrValue)
{
	SkillPropPair skillPair = m_multiMapSkillProp.equal_range(byKindID);
	for ( SkillPropMultiItr it = skillPair.first; it != skillPair.second; it++)
	{
		if (it->second.SkillLevel == dwSkillLevel)
		{
			dwAddAttrValue = it->second.SkillValue;
			return TRUE;
		}
	}
	dwAddAttrValue = 0;
	return FALSE;
}

UINT32 CGymnasiumPropMgr::GetRequireGymnasiumLevel(UINT8 bySkillLevel)
{
	SGymnasiumUpgradePropItr it = m_mapGymnasiumUpgradeProp.begin();
	for ( ; it != m_mapGymnasiumUpgradeProp.end(); it++)
	{
		if (bySkillLevel < it->second.byCurMaxLevel)
		{
			return it->second.dwGymnasiumLevel;
		}
	}
	return 0;
}