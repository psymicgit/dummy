#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include "experiencedrugpropmgr.h"

#include <dll/sdframework/sdloggerimpl.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>

#define SQL_READ_EXPERIENCE_DRUG_STRING "select * from experiencedrugprop"


IMPLEMENT_SINGLETON_PROPMGR(CExperienceDrugPropMgr);

CExperienceDrugPropMgr::CExperienceDrugPropMgr()
{
}

CExperienceDrugPropMgr::~CExperienceDrugPropMgr()
{
}

BOOL CExperienceDrugPropMgr::Init()
{
    return LoadFromDB();
}

VOID CExperienceDrugPropMgr::UnInit()
{
	m_mapExperienceDrugProp.clear();
}


// 从数据源获取数据
BOOL CExperienceDrugPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("experiencedrugprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SExperienceDrugProp stExperienceDrugProp;
        memset(&stExperienceDrugProp, 0, sizeof(stExperienceDrugProp));
		stExperienceDrugProp.wExperienceDrugItemKindID = SGDP::SDAtou(pRes->GetFieldValueByName("ExperienceKindID"));
        stExperienceDrugProp.byLevel			= SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        _SDTStrncpy(stExperienceDrugProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX);
        m_mapExperienceDrugProp[stExperienceDrugProp.wExperienceDrugItemKindID] = stExperienceDrugProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}