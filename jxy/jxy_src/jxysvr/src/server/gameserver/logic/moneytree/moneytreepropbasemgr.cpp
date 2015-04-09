#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdutil.h>
#include <sdstring.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/lua/luamgr.h>
#include <framework/gsapi.h>
#include "moneytreepropmgr.h"
#include "moneytreepropbasemgr.h"

using namespace SGDP;

#define SQL_READ_MONEYTREEBASEPROP_STRING "select * from moneytreebaseprop"


IMPLEMENT_SINGLETON_PROPMGR(CMoneyTreeBasePropMgr);

CMoneyTreeBasePropMgr::CMoneyTreeBasePropMgr()
{
}

CMoneyTreeBasePropMgr::~CMoneyTreeBasePropMgr()
{
}

BOOL CMoneyTreeBasePropMgr::Init()
{
    return LoadFromDB();
}

VOID CMoneyTreeBasePropMgr::UnInit()
{

}



// 从数据源获取数据
BOOL CMoneyTreeBasePropMgr::LoadFromDB()
{
	//SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	//if(NULL == pDBSession)
	//{
	//	return FALSE;
	//}
	//SGDP::ISDDBRecordSet* pRes = NULL;
	//UINT32 dwErrID = 0;
	//string strErr;
	//INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("moneytreebaseprop").c_str(), &pRes, &dwErrID,  &strErr);
	//if(SDDB_HAS_RECORDSET != nRet)
	//{
	//	SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
	//	return FALSE;
	//}		
 //   // 打印查询的表结果
 //   if(pRes->GetRecord())// 当有行结果时
 //   {
 //       memset(&m_stMoneyTreeBaseProp, 0, sizeof(SMoneyTreeBaseProp));
 //       m_stMoneyTreeBaseProp.wRockNumPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("RockNumPerDay"));
 //   }
 //   else
 //   {
 //       SYS_CRITICAL(_SDT("[%s: %d]: moneytreeprop no record!"), MSG_MARK);
 //       return FALSE;
 //   }
 //   if(NULL != pRes)
 //   {
 //       pRes->Release();
 //       pRes = NULL;
 //   }
    return TRUE;
}


UINT32 CMoneyTreeBasePropMgr::GetProduceCoin(UINT32 dwPlayerLevel, UINT8 byMoodValue)
{
    UINT32 dwCoin = CLuamgr::Instance()->GetMoneyTreeProduceCoin(dwPlayerLevel);
    dwCoin = dwCoin * (100 + CMoneyTreePropMgr::Instance()->GetMoneyTreeIncMoneyRate(byMoodValue)) / 100;
    return dwCoin;
}

UINT32	CMoneyTreeBasePropMgr::GetRockNeedGold(CPlayer* poPlayer, UINT16 wCurRockNum)
{
	return CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_MONEYTREE, wCurRockNum );
}

