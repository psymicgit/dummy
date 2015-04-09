#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "godweaponbasepropmgr.h"

#include <db/dbmgr.h>

using namespace SGDP;

#define SQL_READ_GODWEAPONUBASE_STRING "select * from godweaponubaseprop"


IMPLEMENT_SINGLETON_PROPMGR(CGodweaponBasePropMgr);

CGodweaponBasePropMgr::CGodweaponBasePropMgr()
{
	m_wUpgradeQualityItemID = 0;
	m_byGodWeaponRecoverRate = 0;
	m_dwActiveCoin = 0;
	m_byQualityUpdateNum = 0;
}

CGodweaponBasePropMgr::~CGodweaponBasePropMgr()
{
}

BOOL CGodweaponBasePropMgr::Init()
{
    return LoadFromDB();
}

VOID CGodweaponBasePropMgr::UnInit()
{
	m_wUpgradeQualityItemID = 0;
	m_byGodWeaponRecoverRate = 0;
	m_dwActiveCoin = 0;
	m_byQualityUpdateNum = 0;
}


// 从数据源获取数据
BOOL CGodweaponBasePropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("godweaponubaseprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}		
	// 打印查询的表结果
	if(pRes->GetRecord())// 当有行结果时
	{
		m_dwActiveCoin = SGDP::SDAtou(pRes->GetFieldValueByName("ActiveCoin"));
		m_byGodWeaponRecoverRate = SGDP::SDAtou(pRes->GetFieldValueByName("GodWeaponRecoverRate"));	
		m_wUpgradeQualityItemID = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeQualityItemID"));
		m_byQualityUpdateNum = SGDP::SDAtou(pRes->GetFieldValueByName("QualityUpdateNum"));
	}
	else
	{
		SYS_CRITICAL(_SDT("[%s: %d]: godweaponubaseprop hava no data!"), MSG_MARK);    
		return FALSE;
	}

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}
