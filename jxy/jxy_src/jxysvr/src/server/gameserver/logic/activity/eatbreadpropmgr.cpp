#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <framework/gsapi.h>

#include "eatbreadpropmgr.h"

#include <db/dbmgr.h>

IMPLEMENT_SINGLETON_PROPMGR(CEatBreadPropMgr);

CEatBreadPropMgr::CEatBreadPropMgr()
{
	m_byMaxObtainPhyStrengthen = 0;
	m_dwMaxObtainCoin = 0;
	m_wTimePerPhyStrengthen = 0;
	m_wOarNumPerPhyStrengthen = 0;
	m_dwOnceSpeedCoin = 0;
}

CEatBreadPropMgr::~CEatBreadPropMgr()
{

}

BOOL CEatBreadPropMgr::Init()
{

	if(!LoadBasePropFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadBasePropFromDB failed!"), MSG_MARK);
		return FALSE;
	}

	if(!LoadActivityTimeFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadActivityTimeFromDB failed!"), MSG_MARK);
		return FALSE;
	}
	

	return TRUE;
}

VOID CEatBreadPropMgr::UnInit()
{
	m_mapEatBreadStartTimeProp.clear();
	m_mapEatBreadEndTimeProp.clear();

}



// 从数据源获取数据
BOOL CEatBreadPropMgr::LoadActivityTimeFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("eatbreadactivitytimeprop",  "order by ActivityKind, StartTime asc").c_str(), &pRes, &dwErrID,  &strErr);
    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
		SEatBreadActivityTimeProp stProp;
		stProp.byActivityKind = SDAtou(pRes->GetFieldValueByName("ActivityKind"));
		CHAR szTmp[5] = {0};
		SDStrncpy(szTmp, pRes->GetFieldValueByName("StartTime"), 4);		
		CHAR szTmp2[3] = {0};
		memcpy(szTmp2, szTmp, 2);		
		stProp.byStartHour = SDAtou(szTmp2);
		stProp.byStartMin = SDAtou(szTmp+2);
		if((stProp.byStartHour > 23) || (stProp.byStartMin > 59))
		{
			DBG_INFO(_SDT("[%s: %d]: StartTime invalid, Hour[%d], Min[%d]!"), MSG_MARK, stProp.byStartHour, stProp.byStartMin);
			return FALSE;
		}
		stProp.wKeepTime = SDAtou(pRes->GetFieldValueByName("KeepTime"));

		stProp.dwStartTime = stProp.byStartHour * 3600 + stProp.byStartMin * 60;
		stProp.dwEndTime = stProp.dwStartTime + stProp.wKeepTime;

		m_mapEatBreadStartTimeProp[stProp.dwStartTime] = stProp;
		m_mapEatBreadEndTimeProp[stProp.dwEndTime] = stProp;
    }
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}		

	if(m_mapEatBreadEndTimeProp.size() == 0)
	{
		DBG_INFO(_SDT("[%s: %d]: activitytimeprop no data!"), MSG_MARK);
		return FALSE;
	}

    return TRUE;
}


const SEatBreadActivityTimeProp* CEatBreadPropMgr::GetActivityTimeProp(UINT64 qwEnterTime)
{	
	UINT32 dwDayPassSecond = GetDayPassSecond(qwEnterTime);
	CEatBreakActivityTimePropMapItr itr = m_mapEatBreadEndTimeProp.upper_bound(dwDayPassSecond); //key为结束时间dwEndTime
	if(itr != m_mapEatBreadEndTimeProp.end())
	{
		SEatBreadActivityTimeProp* pstProp = &itr->second;
		if(pstProp->dwStartTime <= dwDayPassSecond) 
		{
			return pstProp;
		}		
	}

	return NULL;

}

//获取指定时候后开始的活动时间段配置, 当天的已经没有活动则返回明天的第一个活动
const SEatBreadActivityTimeProp* CEatBreadPropMgr::GetNextActivityTimeProp(UINT64 qwStartTime)
{
	UINT32 dwDayPassSecond = GetDayPassSecond(qwStartTime);
	CEatBreakActivityTimePropMapItr itr = m_mapEatBreadStartTimeProp.upper_bound(dwDayPassSecond); //key为结束时间dwStartTime
	if(itr != m_mapEatBreadStartTimeProp.end())
	{
		return 	&itr->second;
	}

	if(0 == m_mapEatBreadStartTimeProp.size())
	{
		return NULL;
	}

	return &m_mapEatBreadStartTimeProp.begin()->second;	
}


// 从数据源获取数据
BOOL CEatBreadPropMgr::LoadBasePropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("eatbreadprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
	// 打印查询的表结果
	if(pRes->GetRecord())// 当有行结果时
	{
		m_byMaxObtainPhyStrengthen = SDAtou(pRes->GetFieldValueByName("MaxObtainPhyStrengthen"));
		m_dwMaxObtainCoin = SDAtou(pRes->GetFieldValueByName("MaxObtainCoin"));
		m_wTimePerPhyStrengthen = SDAtou(pRes->GetFieldValueByName("TimePerPhyStrengthen"));
		m_wOarNumPerPhyStrengthen = SDAtou(pRes->GetFieldValueByName("OarNumPerPhyStrengthen"));
		m_dwOnceSpeedCoin = SDAtou(pRes->GetFieldValueByName("OnceSpeedCoin"));
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
