#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "bagpropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>

#include <db/dbmgr.h>

#define SQL_READ_BAG_STRING "select * from bagprop"


IMPLEMENT_SINGLETON_PROPMGR(CBagPropMgr);

CBagPropMgr::CBagPropMgr()
{
	
}

CBagPropMgr::~CBagPropMgr()
{
}

BOOL CBagPropMgr::Init()
{
    return LoadFromDB();
}

VOID CBagPropMgr::UnInit()
{
	memset(&m_stBagProp, 0, sizeof(m_stBagProp));
}

// 从数据源获取数据
BOOL CBagPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("bagprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	if(pRes->GetRecord())// 当有行结果时
	{
		m_stBagProp.wMaxOpenNum = SGDP::SDAtou(pRes->GetFieldValueByName("MaxOpenNum"));
		m_stBagProp.wInitOpenNum = SGDP::SDAtou(pRes->GetFieldValueByName("InitOpenNum"));
		m_stBagProp.dwBaseOpenCoin = SGDP::SDAtou(pRes->GetFieldValueByName("BaseOpenCoin"));
		m_stBagProp.dwOpenCoinIncrease = SGDP::SDAtou(pRes->GetFieldValueByName("OpenCoinIncrease"));
		m_stBagProp.dwBaseOpenGold = SGDP::SDAtou(pRes->GetFieldValueByName("BaseOpenGold"));
		m_stBagProp.dwOpenGoldIncrease = SGDP::SDAtou(pRes->GetFieldValueByName("OpenGoldIncrease"));
		m_stBagProp.wTotalBagNum = SGDP::SDAtou(pRes->GetFieldValueByName("TotalBagNum"));
		m_stBagProp.strGiftName = pRes->GetFieldValueByName("GiftName");
		m_stBagProp.strGiftDesc = pRes->GetFieldValueByName("GiftDesc");
	}
	else
	{
		if(NULL != pRes)
		{
			pRes->Release();
			pRes = NULL;
		}
		return FALSE;
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

UINT32 CBagPropMgr::GetEnourageGold(UINT8 byOpenNum)
{
	if ( 0 == byOpenNum)
	{
		return 0;
	}
	UINT32 dwFirstGold = m_stBagProp.dwBaseOpenGold;
	UINT32 dwLastGold = m_stBagProp.dwBaseOpenGold + m_stBagProp.dwOpenGoldIncrease * (byOpenNum - 1);
	if ( 1 == byOpenNum)
	{
		return dwFirstGold;
	}
	
	return (dwFirstGold + dwLastGold ) * byOpenNum / 2;
}