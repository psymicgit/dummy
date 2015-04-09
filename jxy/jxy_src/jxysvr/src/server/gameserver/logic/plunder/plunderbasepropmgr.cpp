#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <logic/lua/luamgr.h>
#include "plunderbasepropmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

#define SQL_READ_PLUNDERBASE_STRING "select * from plunderbaseprop"


IMPLEMENT_SINGLETON_PROPMGR(CPlunderBasePropMgr);

CPlunderBasePropMgr::CPlunderBasePropMgr()
{
    memset(&m_stPlunderBaseProp, 0, sizeof(SPlunderBaseProp));
}

CPlunderBasePropMgr::~CPlunderBasePropMgr()
{

}

BOOL CPlunderBasePropMgr::Init()
{
	if(!LoadFromDB())
    {
        return FALSE;
    }

    //生成所有玩家等级生产铜钱/器魂总值
    UINT32 dwValues = 0;
	UINT16 wMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();
    for (UINT32 i = 1; i <= wMaxPlayerLevel; i++)
    {
		m_mapWorkCoin[i] = CLuamgr::Instance()->GetCaptureWorkCoin(i);
        m_mapWorkScience[i] = CLuamgr::Instance()->GetCaptureWorkScience(i);
    }
    return TRUE;
}

VOID CPlunderBasePropMgr::UnInit()
{
	m_mapWorkCoin.clear();
	m_mapWorkScience.clear();
	memset(&m_stPlunderBaseProp, 0, sizeof(SPlunderBaseProp));
}


// 从数据源获取数据
BOOL CPlunderBasePropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("plunderbaseprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    // 打印查询的表结果
	if(pRes->GetRecord())// 当有行结果时
	{
		m_stPlunderBaseProp.wPlunderOpenLevel = SGDP::SDAtou(pRes->GetFieldValueByName("PlunderOpenLevel"));
		m_stPlunderBaseProp.dwPlunderRobotStartID = SGDP::SDAtou(pRes->GetFieldValueByName("PlunderRobotStartID"));		
		m_stPlunderBaseProp.wPlunderLowerLevelItemID = SGDP::SDAtou(pRes->GetFieldValueByName("PlunderLowerLevelItemID"));
		m_stPlunderBaseProp.wPlunderFreeTimesPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("PlunderFreeTimesPerDay"));
		//m_stPlunderBaseProp.wPlunderMaxTimesPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("PlunderMaxTimesPerDay"));
		m_stPlunderBaseProp.dwCaptureWorkSecond = SGDP::SDAtou(pRes->GetFieldValueByName("CaptureWorkSecond"));
		m_stPlunderBaseProp.byBleedPerGoldClrMin = SGDP::SDAtou(pRes->GetFieldValueByName("BleedPerGoldClrMin"));
		m_stPlunderBaseProp.byMaxCaptureNum = SGDP::SDAtou(pRes->GetFieldValueByName("MaxCaptureNum"));
		m_stPlunderBaseProp.dwCaptureKeepSecond = SGDP::SDAtou(pRes->GetFieldValueByName("CaptureKeepSecond"));
	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}
