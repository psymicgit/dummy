#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include "phystrengthmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

#define SQL_READ_SINGLEPRAM_STRING "select * from phystrengthprop"


IMPLEMENT_SINGLETON_PROPMGR(CPhystrengthMgr);

CPhystrengthMgr::CPhystrengthMgr()
{
    m_byCommonPhyStrength = 0;
    m_byElitePhyStrength = 0;
	memset(&m_stBuyPhystrengthProp, 0, sizeof(m_stBuyPhystrengthProp));
}

CPhystrengthMgr::~CPhystrengthMgr()
{
}

BOOL CPhystrengthMgr::Init()
{
    return LoadFromDB();
}

VOID CPhystrengthMgr::UnInit()
{

}


// 从数据源获取数据
BOOL CPhystrengthMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("phystrengthprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    // 打印查询的表结果
	if(pRes->GetRecord())// 当有行结果时
	{
		m_byCommonPhyStrength = SGDP::SDAtou(pRes->GetFieldValueByName("CommonPhyStrength"));
		m_byElitePhyStrength = SGDP::SDAtou(pRes->GetFieldValueByName("ElitePhyStrength"));
		m_wPhyStrengthRefreshCD = SGDP::SDAtou(pRes->GetFieldValueByName("PhyStrengthRefreshCD"));

		m_stBuyPhystrengthProp.byBuyPhyStrength = SGDP::SDAtou(pRes->GetFieldValueByName("BuyPhyStrength"));
		//m_stBuyPhystrengthProp.byBuyMaxNumPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("BuyMaxNumPerDay"));
	}
    else
    {
        return FALSE;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


