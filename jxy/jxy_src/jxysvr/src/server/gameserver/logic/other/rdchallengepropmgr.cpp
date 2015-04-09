#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include "rdchallengepropmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

IMPLEMENT_SINGLETON_PROPMGR(CRdChallengePropMgr);

CRdChallengePropMgr::CRdChallengePropMgr()
{
	m_wMaxChallengePoint = 0;
	m_wRdChallengeRefreshCD = 0;
	m_wRdChallengeCD = 0;
	m_dwRefreshCoin = 0;
}

CRdChallengePropMgr::~CRdChallengePropMgr()
{
}

BOOL CRdChallengePropMgr::Init()
{
    return LoadFromDB();
}

VOID CRdChallengePropMgr::UnInit()
{

}


// 从数据源获取数据
BOOL CRdChallengePropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("rdchallengeprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    // 打印查询的表结果
	if(pRes->GetRecord())// 当有行结果时
	{
		m_wMaxChallengePoint = SGDP::SDAtou(pRes->GetFieldValueByName("MaxChallengePoint"));
		m_wRdChallengeRefreshCD = SGDP::SDAtou(pRes->GetFieldValueByName("RdChallengeRefreshCD"));
		m_wRdChallengeCD = SGDP::SDAtou(pRes->GetFieldValueByName("RdChallengeCD"));
		m_dwRefreshCoin = SGDP::SDAtou(pRes->GetFieldValueByName("RefreshCoin"));
		m_wClrRdChallengeCDGold = SGDP::SDAtou(pRes->GetFieldValueByName("ClrRdChallengeCDGold"));
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


