#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdutil.h>
#include <sdstring.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <framework/gsapi.h>

#include <db/dbmgr.h>

#include "moneytreepropmgr.h"

using namespace SGDP;

#define SQL_READ_MONEYTREEPROP_STRING "select * from moneytreeprop"


IMPLEMENT_SINGLETON_PROPMGR(CMoneyTreePropMgr);

CMoneyTreePropMgr::CMoneyTreePropMgr()
{
}

CMoneyTreePropMgr::~CMoneyTreePropMgr()
{
}

BOOL CMoneyTreePropMgr::Init()
{
    return LoadFromDB();
}

VOID CMoneyTreePropMgr::UnInit()
{
	m_MoneyTreePropMap.clear();
}



// 从数据源获取数据
BOOL CMoneyTreePropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("moneytreeprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	SMoneyTreeProp stMoneyTreeProp;
    // 打印查询的表结果
	while(NULL != pRes && pRes->GetRecord())// 当有行结果时
	{
		memset(&stMoneyTreeProp, 0, sizeof(SMoneyTreeProp));
		stMoneyTreeProp.wMoodValue = SGDP::SDAtou(pRes->GetFieldValueByName("MoodValue"));
		stMoneyTreeProp.wIncCoinRate = SGDP::SDAtou(pRes->GetFieldValueByName("IncCoinRate"));
		stMoneyTreeProp.byGetMoodRate = SGDP::SDAtou(pRes->GetFieldValueByName("GetMoodRate"));
		stMoneyTreeProp.wMoodKeepTime = SGDP::SDAtou(pRes->GetFieldValueByName("MoodKeepTime"));
		m_MoneyTreePropMap[stMoneyTreeProp.wMoodValue] = stMoneyTreeProp;
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

UINT16 CMoneyTreePropMgr::GetMoneyTreeIncMoneyRate(UINT16 wMoodValue)
{
	CMoneyTreePropMapItr itr = m_MoneyTreePropMap.find(wMoodValue);
	if (itr != m_MoneyTreePropMap.end())
	{
		return itr->second.wIncCoinRate;
	}
	
	return 0;
}

UINT8 CMoneyTreePropMgr::GetMoneyTreeMoodRate(UINT16 wMoodValue)
{
	CMoneyTreePropMapItr itr = m_MoneyTreePropMap.find(wMoodValue);
	if (itr != m_MoneyTreePropMap.end())
	{
		return itr->second.byGetMoodRate;
	}

	return 0;
}

