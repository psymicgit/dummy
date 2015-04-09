#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "cropseedpropmgr.h"
#include "logic/base/basepropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>

#include <db/dbmgr.h>

#define SQL_READ_DRUG_STRING "select * from cropprop"


IMPLEMENT_SINGLETON(CCropSeedPropMgr);

CCropSeedPropMgr::CCropSeedPropMgr()
{
}

CCropSeedPropMgr::~CCropSeedPropMgr()
{
}

BOOL CCropSeedPropMgr::Init()
{
    return LoadFromDB();
}

VOID CCropSeedPropMgr::UnInit()
{
    m_mapCropProp.clear();

}

// 从数据源获取数据
BOOL CCropSeedPropMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("cropprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SCropProp stCropProp;
        memset(&stCropProp, 0, sizeof(stCropProp));
        stCropProp.wCropKindID		= SGDP::SDAtou(pRes->GetFieldValueByName("CropKindID"));
        _SDTStrncpy(stCropProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX);
        stCropProp.byType			= SGDP::SDAtou(pRes->GetFieldValueByName("Type"));
        stCropProp.byLevel		= SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        stCropProp.wObtainCD			= SGDP::SDAtou(pRes->GetFieldValueByName("ObtainCD"));
        stCropProp.dwObtainValue	= SGDP::SDAtou(pRes->GetFieldValueByName("ObtainValue"));
        stCropProp.wObtainKindID	= SGDP::SDAtou(pRes->GetFieldValueByName("ObtainKindID"));
        m_mapCropProp[stCropProp.wCropKindID] = stCropProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}



