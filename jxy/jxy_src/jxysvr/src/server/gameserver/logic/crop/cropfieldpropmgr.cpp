#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "cropfieldpropmgr.h"
#include "logic/base/basepropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>

#include <db/dbmgr.h>

#define SQL_READ_DRUG_STRING "select * from cropfieldprop"


IMPLEMENT_SINGLETON(CCropFieldPropMgr);

CCropFieldPropMgr::CCropFieldPropMgr()
{
}

CCropFieldPropMgr::~CCropFieldPropMgr()
{
}

BOOL CCropFieldPropMgr::Init()
{
    return LoadFromDB();
}

VOID CCropFieldPropMgr::UnInit()
{
	m_mapCropFieldProp.clear();
}


// 从数据源获取数据
BOOL CCropFieldPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("cropfieldprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SCropFieldProp stCropFieldProp;
        memset(&stCropFieldProp, 0, sizeof(SCropFieldProp));
        stCropFieldProp.wKindID	= SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
        _SDTStrncpy(stCropFieldProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX);
        stCropFieldProp.byLevel	= SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        stCropFieldProp.dwGold		= SGDP::SDAtou(pRes->GetFieldValueByName("Gold"));
        stCropFieldProp.dwCoin		= SGDP::SDAtou(pRes->GetFieldValueByName("Coin"));
        m_mapCropFieldProp[stCropFieldProp.wKindID] = stCropFieldProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


