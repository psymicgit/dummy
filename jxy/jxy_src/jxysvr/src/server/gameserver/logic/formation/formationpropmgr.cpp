#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/commondef.h>
#include <protocol/server/protocommondata.h>
#include <framework/gsapi.h>
#include "formationpropmgr.h"

#include <db/dbmgr.h>

#define SQL_READ_FORMATION_STRING "select * from formationprop order by FormationKindID asc"


IMPLEMENT_SINGLETON_PROPMGR(CFormationPropMgr);

CFormationPropMgr::CFormationPropMgr()
{
}

CFormationPropMgr::~CFormationPropMgr()
{
}

BOOL CFormationPropMgr::Init()
{
    if(!LoadFromDB())
	{
		return FALSE;
	}

	if ( !LoadFromDBProp2())
	{
		return FALSE;
	}

	if( !LoadFormationUpgrade())
	{
		return FALSE;
	}

	if ( !LoadFromationUnlock())
	{
		return FALSE;
	}
	InitData();

	return TRUE;
}

VOID CFormationPropMgr::UnInit()
{
	m_mapFormationInfo.clear();
}


// 从数据源获取数据
BOOL CFormationPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("formationprop", " order by FormationKindID asc").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	BOOL bHaveDataFlag = FALSE;
	SFormationProp stFormationProp;
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT8 byFormationKindID = SGDP::SDAtou(pRes->GetFieldValueByName("FormationKindID"));
		if(0 == stFormationProp.byFormationKindID)
		{
			stFormationProp.byFormationKindID = byFormationKindID;
		}
		UINT8 byOpenIdx = SGDP::SDAtou(pRes->GetFieldValueByName("OpenIdx"));
		if((byOpenIdx == 0) || (byOpenIdx > MAX_FORMATION_IDX_NUM))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: OpenIdx[%d] is invalid!"), MSG_MARK, byOpenIdx);        
			return FALSE;
		}
		UINT8 byCoachIdxFlag = SGDP::SDAtou(pRes->GetFieldValueByName("CoachIdxFlag"));		
		if((byFormationKindID != stFormationProp.byFormationKindID) && (0 != stFormationProp.byFormationKindID))
		{
			if(stFormationProp.mapOpenIdx.size() < 3)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: OpenIdx Num < 3!"), MSG_MARK, byOpenIdx);        
				return FALSE;
			}
			m_mapFormationInfo[stFormationProp.byFormationKindID] = stFormationProp;
			stFormationProp.byFormationKindID = byFormationKindID;			
			stFormationProp.mapOpenIdx.clear();
		}
		stFormationProp.mapOpenIdx[byOpenIdx] = byOpenIdx;	
		if(1 == byCoachIdxFlag)
		{
			stFormationProp.byCoachFormationIdx = byOpenIdx;
		}		
	}
	if(0 != stFormationProp.byFormationKindID)
	{
		m_mapFormationInfo[stFormationProp.byFormationKindID] = stFormationProp;
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
	if((m_mapFormationInfo.find(ATTACK_FORMATION_KIND) == m_mapFormationInfo.end()) || (m_mapFormationInfo.find(DEFENCE_FORMATION_KIND) == m_mapFormationInfo.end()))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: No default formation!"), MSG_MARK);  
		return FALSE;
	}

    return TRUE;
}

BOOL CFormationPropMgr::LoadFromDBProp2()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("formationprop2", " order by FormationKindID").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	

	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SFormationLevelProp stProp = {0};
		stProp.byFormationID = SGDP::SDAtou(pRes->GetFieldValueByName("FormationKindID"));
		stProp.byOpenIdx = SGDP::SDAtou(pRes->GetFieldValueByName("OpenIdx"));
		stProp.byCoachIdxFlag = SGDP::SDAtou(pRes->GetFieldValueByName("CoachIdxFlag"));
		stProp.byVisableLevel = SGDP::SDAtou(pRes->GetFieldValueByName("VisableLevel"));
		stProp.byUnlockLevel = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockLevel"));

		CMapFormationLevelMapItr itr = m_mapFormationLevelProp.find(stProp.byFormationID);
		if ( itr == m_mapFormationLevelProp.end()) //
		{
			CMapFormationLevelProp mapLevel;
			mapLevel[stProp.byOpenIdx] = stProp;
			m_mapFormationLevelProp[stProp.byFormationID] = mapLevel;
		}
		else
		{
			itr->second[stProp.byOpenIdx] = stProp;
		}

		//m_mapFormationLevelProp.insert(make_pair(stProp.byFormationID, stProp));
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CFormationPropMgr::LoadFormationUpgrade()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("formationupgradeprop", " order by FormationID").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	


	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SFormationUpgradeProp stProp = {0};
		stProp.byFormationID = SGDP::SDAtou(pRes->GetFieldValueByName("FormationID"));
		stProp.byAttrKindID = SGDP::SDAtou(pRes->GetFieldValueByName("AttrKindID"));
		stProp.byAttrLevel = SGDP::SDAtou(pRes->GetFieldValueByName("AttrLevel"));
		stProp.dwAttrValue = SGDP::SDAtou(pRes->GetFieldValueByName("AttrKindValue"));
		stProp.dwUpgradeCostValue = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeSciencePoint"));
		stProp.byNeedPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedPlayerLevel"));
		stProp.byUnlockPosNum = GetUnlockPosNum(stProp.byFormationID, stProp.byAttrLevel);
		CMapFormationUpgradeMapItr itr = m_mapFormationUpgradeProp.find(stProp.byFormationID);
		if ( itr == m_mapFormationUpgradeProp.end()) //
		{
			CMapFormationUpgradeProp mapProp;
			mapProp[stProp.byAttrLevel] = stProp;
			m_mapFormationUpgradeProp[stProp.byFormationID] = mapProp;
		}
		else
		{
			itr->second[stProp.byAttrLevel] = stProp;
		}
		
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CFormationPropMgr::LoadFromationUnlock()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("formationunlock", " order by FormationID").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	


	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SFormationUnlockProp stProp = {0};
		stProp.byFormationID = SGDP::SDAtou(pRes->GetFieldValueByName("FormationID"));
		stProp.byVisableLevel = SGDP::SDAtou(pRes->GetFieldValueByName("VisibleLevel"));
		stProp.byUnlockLevel = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockLevel"));

		m_mapFormationUnlockProp[stProp.byFormationID] = stProp;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

VOID CFormationPropMgr::InitData()
{
	for(CFormationKindMapItr itrKind = m_mapFormationInfo.begin(); itrKind != m_mapFormationInfo.end(); itrKind++)
	{
		SFormationProp& stProp = itrKind->second;
		DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stProp.stDT_FORMATION_IDX_DATA;
		stDT_FORMATION_IDX_DATA.byFormationKind = stProp.byFormationKindID;		
		for(CFormationIdxMapItr itrProp = stProp.mapOpenIdx.begin(); ((itrProp != stProp.mapOpenIdx.end()) && (stDT_FORMATION_IDX_DATA.byOpenIdxNum < MAX_FORMATION_IDX_NUM)); itrProp++)
		{
			stDT_FORMATION_IDX_DATA.astOpenIdxInfo[stDT_FORMATION_IDX_DATA.byOpenIdxNum].wHeroID = 0;
			if(itrProp->second == stProp.byCoachFormationIdx)
			{
				stProp.byCoachFormationLstIdx = stDT_FORMATION_IDX_DATA.byOpenIdxNum;
			}
			stDT_FORMATION_IDX_DATA.astOpenIdxInfo[stDT_FORMATION_IDX_DATA.byOpenIdxNum++].byFormationIdx = itrProp->second;
		}
	}
}

SFormationProp* CFormationPropMgr::GetFormationProp(UINT8 byFormationKind)
{
	CFormationKindMapItr itrKind = m_mapFormationInfo.find(byFormationKind);
	if(itrKind != m_mapFormationInfo.end())
	{
		return &itrKind->second;
	}

	return NULL;
}

const SFormationLevelProp* CFormationPropMgr::GetFormationLevelProp(UINT8 byFormationID, UINT8 byPostionIdx)
{
	CMapFormationLevelMapItr itr = m_mapFormationLevelProp.find(byFormationID);
	if ( itr != m_mapFormationLevelProp.end())
	{
		CMapFormationLevelPropItr propItr = itr->second.find(byPostionIdx);
		if (  propItr != itr->second.end())
		{
			return &propItr->second;
		}
	}
	return NULL;
}

const CMapFormationLevelProp* CFormationPropMgr::GetFormationLevelMap(UINT8 byFormationID)
{
	CMapFormationLevelMapItr itr = m_mapFormationLevelProp.find(byFormationID);
	if ( itr != m_mapFormationLevelProp.end())
	{
		return &itr->second;
	}
	return NULL;
}

const SFormationUpgradeProp* CFormationPropMgr::GetFormationUpgrade(UINT8 byFormationID, UINT8 byFormationLevel)
{
	CMapFormationUpgradeMapItr itr = m_mapFormationUpgradeProp.find(byFormationID);
	if ( itr != m_mapFormationUpgradeProp.end())
	{
		CMapFormationUpgradePropItr propItr = itr->second.find(byFormationLevel);
		if ( propItr != itr->second.end())
		{
			return &propItr->second;
		}
	}
	return NULL;
}

UINT8 CFormationPropMgr::GetUnlockPosNum(UINT8 byFormationID, UINT8 byFormationLevel)
{
	UINT8 byUnlockNum = 0;
	CMapFormationLevelMapItr itr = m_mapFormationLevelProp.find(byFormationID);
	if ( itr != m_mapFormationLevelProp.end())
	{
		CMapFormationLevelPropItr propItr = itr->second.begin();
		for ( ; propItr != itr->second.end(); propItr++)
		{
			if ( propItr->second.byUnlockLevel <= byFormationLevel )
			{
				byUnlockNum++;
			}
		}
	}
	return byUnlockNum;
}

BOOL CFormationPropMgr::GetFormationUnlock(UINT8 byFormationID, UINT8& byVisableLevel, UINT8& byUnlockLevel)
{
	CMapFormationUnlockPropItr itr = m_mapFormationUnlockProp.find( byFormationID );
	if ( itr == m_mapFormationUnlockProp.end())
	{
		return FALSE;
	}

	byVisableLevel = itr->second.byVisableLevel;
	byUnlockLevel = itr->second.byUnlockLevel;
	return TRUE;
}