#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include "plundergridpropmgr.h"
#include "plunderbasepropmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>

using namespace SGDP;

#define SQL_READ_PLUNDERGRID_STRING "select * from plundergridunlockprop order by GridIdx asc; "


IMPLEMENT_SINGLETON_PROPMGR(CPlunderGridPropMgr);

CPlunderGridPropMgr::CPlunderGridPropMgr()
{
    m_byMaxCaptureNum = 0;
    m_wMaxPlayerLevel = 0;
	m_wMaxGridUnlockLevel = 0;
}

CPlunderGridPropMgr::~CPlunderGridPropMgr()
{
}

BOOL CPlunderGridPropMgr::Init()
{
    m_wMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();

    return LoadFromDB();
}

VOID CPlunderGridPropMgr::UnInit()
{
	m_mapGridIdx2LevelProp.clear();
	m_mapLevel2GridIdxProp.clear();
}


// 从数据源获取数据
BOOL CPlunderGridPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("plundergridunlockprop", " order by GridIdx asc").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
    UINT8 byLastIdx = 0;
    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT8 byGridIdx = SGDP::SDAtou(pRes->GetFieldValueByName("GridIdx"));
        if(byGridIdx != byLastIdx + 1)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GridIdx[%d] invalid!"), MSG_MARK, byGridIdx);
            return FALSE;
        }
        UINT16 wUnlockLevel = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockLevel"));

        m_mapGridIdx2LevelProp[byGridIdx] = wUnlockLevel;
        m_mapLevel2GridIdxProp[wUnlockLevel] = byGridIdx;
        if(byGridIdx <= MAX_CAPTURE_NUM)
        {
            m_awUnlockGridLevel[byGridIdx - 1] = wUnlockLevel;
        }
		if(wUnlockLevel > m_wMaxGridUnlockLevel)
		{
			m_wMaxGridUnlockLevel = wUnlockLevel;
		}

        byLastIdx = byGridIdx;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    m_byMaxCaptureNum = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().byMaxCaptureNum;
    if(m_mapGridIdx2LevelProp.size() != m_byMaxCaptureNum)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: grid num !=%d !"), MSG_MARK, m_byMaxCaptureNum);
        return FALSE;
    }

    return TRUE;
}

UINT16 CPlunderGridPropMgr::GetUnlockLevel(UINT8 byGridIdx)
{
    if((0 == byGridIdx) || (byGridIdx > m_byMaxCaptureNum))
    {
        return 0xFFFF;
    }

    return m_mapGridIdx2LevelProp[byGridIdx];
}


UINT8  CPlunderGridPropMgr::GetUnlockNum(UINT16 wLevel)
{
    if(wLevel >= m_wMaxPlayerLevel)
    {
        return m_byMaxCaptureNum;
    }
	if(wLevel >= m_wMaxGridUnlockLevel)
	{
		return MAX_CAPTURE_NUM;
	}

	CGridUnlockPropMapItr itr = m_mapLevel2GridIdxProp.lower_bound(wLevel);
	if(itr != m_mapLevel2GridIdxProp.end())
	{
		if(itr->first > wLevel)
		{
			UINT8 byGridNum = (UINT8)(itr->second);
			byGridNum = byGridNum > 1 ? (byGridNum - 1) : byGridNum;
			return byGridNum;
		}
		else
		{
			return (UINT8)(itr->second);
		}
	}

    //
    return 1;
}
