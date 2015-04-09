#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <common/client/commondef.h>
#include "worshippropmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

IMPLEMENT_SINGLETON_PROPMGR(CWorshipPropMgr);

CWorshipPropMgr::CWorshipPropMgr()
{
	m_mapRaceWorshipProp.clear();
	m_mapRDCWorshipProp.clear();
}

CWorshipPropMgr::~CWorshipPropMgr()
{
}

BOOL CWorshipPropMgr::Init()
{
    return LoadFromDB();
}

VOID CWorshipPropMgr::UnInit()
{
	m_mapRaceWorshipProp.clear();
	m_mapRDCWorshipProp.clear();
}


// 从数据源获取数据
BOOL CWorshipPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("worshipprop", " order by WorshipType, WorshipTypeLevel").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    // 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SWorshipProp stPop;
		stPop.byWorshipType = SGDP::SDAtou(pRes->GetFieldValueByName("WorshipType"));
		stPop.byWorshipTypeLevel = SGDP::SDAtou(pRes->GetFieldValueByName("WorshipTypeLevel"));
		stPop.dwNeedCoin = SGDP::SDAtou(pRes->GetFieldValueByName("NeedCoin"));
		stPop.dwNeedGold = SGDP::SDAtou(pRes->GetFieldValueByName("NeedGold"));
		stPop.byNeedVipLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedVipLevel"));
		stPop.dwEncJingJie = SGDP::SDAtou(pRes->GetFieldValueByName("EncJingJie"));
		stPop.dwEncPurpleGas = SGDP::SDAtou(pRes->GetFieldValueByName("EncPurpleGas"));
		if(EWT_RACE == stPop.byWorshipType)
		{
			m_mapRaceWorshipProp[stPop.byWorshipTypeLevel] = stPop;
		}
		else if(EWT_RDC == stPop.byWorshipType)
		{
			m_mapRDCWorshipProp[stPop.byWorshipTypeLevel] = stPop;
		}
		else
		{
			SYS_CRITICAL(_SDT("[%s: %d]: WorshipType[%d] is ivalid!"), MSG_MARK, stPop.byWorshipType);  
			return FALSE;
		}
	}
   
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


SWorshipProp* CWorshipPropMgr::GetWorshipProp(UINT8 byWorshipType, UINT8 byWorshipTypeLevel)
{
	if(EWT_RACE == byWorshipType)
	{
		CLevel2WorshipPropMapItr itr = m_mapRaceWorshipProp.find(byWorshipTypeLevel);
		if(itr != m_mapRaceWorshipProp.end())
		{
			return &itr->second;
		}
	}
	else if(EWT_RDC == byWorshipType)
	{
		CLevel2WorshipPropMapItr itr = m_mapRDCWorshipProp.find(byWorshipTypeLevel);
		if(itr != m_mapRDCWorshipProp.end())
		{
			return &itr->second;
		}
	}
	else
	{
		return NULL;
	}

	return NULL;
}
