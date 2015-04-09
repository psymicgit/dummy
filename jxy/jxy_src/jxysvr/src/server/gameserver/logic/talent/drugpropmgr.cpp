#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include "drugpropmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

#define SQL_READ_DRUG_STRING "select * from drugprop"


IMPLEMENT_SINGLETON_PROPMGR(CDrugPropMgr);

CDrugPropMgr::CDrugPropMgr()
{
}

CDrugPropMgr::~CDrugPropMgr()
{
}

BOOL CDrugPropMgr::Init()
{
    return LoadFromDB();
}

VOID CDrugPropMgr::UnInit()
{
	m_mapDrugProp.clear();
	m_mapItemID2DrugProp.clear();
}


// 从数据源获取数据
BOOL CDrugPropMgr::LoadFromDB()
{	
	return TRUE; //废弃

	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("drugprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
	//查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SDrugProp stDrugProp;
		memset(&stDrugProp, 0, sizeof(stDrugProp));
		stDrugProp.byLevel		= SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
		_SDTStrncpy(stDrugProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX);
		stDrugProp.byCanUseNum		= SGDP::SDAtou(pRes->GetFieldValueByName("CanUseNum"));
		stDrugProp.wUseCD			= SGDP::SDAtou(pRes->GetFieldValueByName("UseCD"));
		stDrugProp.wBaseTalent		= SGDP::SDAtou(pRes->GetFieldValueByName("BaseTalent"));
		stDrugProp.wDecTalent		= SGDP::SDAtou(pRes->GetFieldValueByName("DecTalent"));
		stDrugProp.wDrugItemKindID = SGDP::SDAtou(pRes->GetFieldValueByName("DrugItemKindID"));		
		m_mapDrugProp[stDrugProp.byLevel] = stDrugProp;
		m_mapItemID2DrugProp[stDrugProp.wDrugItemKindID] = stDrugProp;
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


SDrugProp* CDrugPropMgr::GetDrugProp(UINT8 byLevel)
{
	CDrugPropMapItr itr =  m_mapDrugProp.find(byLevel);
	if(itr != m_mapDrugProp.end())
	{
		return &(itr->second);
	}
	return NULL;
}

//获取丹药等级
UINT16 CDrugPropMgr::GetDrugLevel(UINT16 wDrugItemID)
{
	CItemID2DrugPropMapItr itr = m_mapItemID2DrugProp.find(wDrugItemID); 
	if(itr != m_mapItemID2DrugProp.end())
	{
		return itr->second.byLevel;
	}

	return 0;
}


UINT8  CDrugPropMgr::GetDT_MIX_DRUG_LEAVE_BY_INDEX(const UINT8 byIndex)
{
	UINT8 l_byIndex = 0;
	UINT8 l_byLevel = 0;
	for(CDrugPropMapItr itr = m_mapDrugProp.begin(); itr != m_mapDrugProp.end(); itr++)
	{
		if (byIndex == l_byIndex)
		{
			l_byLevel = itr->second.byLevel;
			break;
		}
		l_byIndex++;
	}
	return l_byLevel;
}


UINT8 CDrugPropMgr::GetMaxDrugNum()
{
	UINT8 byMaxNum = 0;
	for(CDrugPropMapItr itr = m_mapDrugProp.begin(); itr != m_mapDrugProp.end(); itr++)
	{
		byMaxNum += itr->second.byCanUseNum;
	}

	return byMaxNum;
}

