#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "factionauthorityprop.h"
#include "logic/base/basepropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>
#include <logic/faction/factionprop.h>
#include <db/dbmgr.h>

#define	MAX_FACTION_PROXY_NUM			1
#define	MAX_FACTION_ELDER_NUM			2
#define	MAX_FACTION_CUSTODIAN_NUM		5
#define	MAX_FACTION_ELITE_NUM			10

IMPLEMENT_SINGLETON(CFactorAuthorityPropMgr);

CFactorAuthorityPropMgr::CFactorAuthorityPropMgr()
{
}

CFactorAuthorityPropMgr::~CFactorAuthorityPropMgr()
{

}

BOOL CFactorAuthorityPropMgr::Init()
{
    return LoadFromDB();
}

VOID CFactorAuthorityPropMgr::UnInit()
{

}

// 从数据源获取数据
BOOL CFactorAuthorityPropMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("factionauthority").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }


    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT8	byJobType = SGDP::SDAtou(pRes->GetFieldValueByName("JobType"));
        DT_FACTION_AUTHORITY_EX stDT_FACTION_AUTHORITY;
        stDT_FACTION_AUTHORITY.byKickedNormal	=	SGDP::SDAtou(pRes->GetFieldValueByName("KickedNormal"));
        stDT_FACTION_AUTHORITY.byKickedElite	=	SGDP::SDAtou(pRes->GetFieldValueByName("KickedElite"));
        stDT_FACTION_AUTHORITY.byPromotion		=	SGDP::SDAtou(pRes->GetFieldValueByName("Promotion"));
        stDT_FACTION_AUTHORITY.byDemotion		=	SGDP::SDAtou(pRes->GetFieldValueByName("Demotion"));
        stDT_FACTION_AUTHORITY.byBuild			=	SGDP::SDAtou(pRes->GetFieldValueByName("Build"));
        stDT_FACTION_AUTHORITY.byActivity		=	SGDP::SDAtou(pRes->GetFieldValueByName("Activity"));
        stDT_FACTION_AUTHORITY.bySignUp			=	SGDP::SDAtou(pRes->GetFieldValueByName("SignUp"));
        stDT_FACTION_AUTHORITY.byAudit			=	SGDP::SDAtou(pRes->GetFieldValueByName("Audit"));
        stDT_FACTION_AUTHORITY.byEditSetting	=	SGDP::SDAtou(pRes->GetFieldValueByName("EditSetting"));
        stDT_FACTION_AUTHORITY.byShowLog		=	SGDP::SDAtou(pRes->GetFieldValueByName("ShowLog"));
		stDT_FACTION_AUTHORITY.byChangeOther    =   SGDP::SDAtou(pRes->GetFieldValueByName("ChangeOther"));
		stDT_FACTION_AUTHORITY.byBecomeHead    =   SGDP::SDAtou(pRes->GetFieldValueByName("BecomeHead"));
		stDT_FACTION_AUTHORITY.byChangeIcon    =   SGDP::SDAtou(pRes->GetFieldValueByName("ChangeIcon"));
        m_mapFactionAuthority[byJobType]		=	stDT_FACTION_AUTHORITY;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


DT_FACTION_AUTHORITY CFactorAuthorityPropMgr::GetAuthority(em_Faction_Job myJob)
{
	DT_FACTION_AUTHORITY stEmpty = {0};
	CFactionAuthorityMapItr itr = m_mapFactionAuthority.find(myJob);
	if (itr == m_mapFactionAuthority.end())
	{
		return stEmpty;
	}
	return ToAuthority(&itr->second);
}

DT_FACTION_AUTHORITY_EX* CFactorAuthorityPropMgr::GetAuthorityEx(em_Faction_Job myJob)
{
	CFactionAuthorityMapItr itr = m_mapFactionAuthority.find(myJob);
	if (itr == m_mapFactionAuthority.end())
	{
		return NULL;
	}
	return &itr->second;
}

DT_FACTION_AUTHORITY CFactorAuthorityPropMgr::ToAuthority(DT_FACTION_AUTHORITY_EX* stEx)
{
	DT_FACTION_AUTHORITY stAuthority = {0};
	if ( NULL == stEx)
	{
		return stAuthority;
	}
	memcpy(&stAuthority, stEx, sizeof(DT_FACTION_AUTHORITY));
	return stAuthority;
}

BOOL CFactorAuthorityPropMgr::CkJobAuthority(em_Faction_Job myJob, em_Faction_Authority myAuth)
{
    CFactionAuthorityMapItr itr = m_mapFactionAuthority.find(myJob);
    if (itr == m_mapFactionAuthority.end())
    {
        return FALSE;
    }
    switch(myAuth)
    {
    case EFA_KickedNormal	:
        return itr->second.byKickedNormal;
        break;
    case	EFA_KickedElite:
        return itr->second.byKickedElite;
        break;
    case	EFA_Promotion:
        return itr->second.byPromotion;
        break;
    case EFA_Demotion:
        return itr->second.byDemotion;
        break;
    case	EFA_Build:
        return itr->second.byBuild;
        break;
    case	EFA_Activity:
        return itr->second.byActivity;
        break;
    case	EFA_SignUp:
        return itr->second.bySignUp;
        break;
    case	EFA_Audit:
        return itr->second.byAudit;
        break;
    case	EFA_EditSetting:
        return itr->second.byEditSetting;
        break;
    case	EFA_ShowLog:
        return itr->second.byShowLog;
        break;
	case    EFA_ChangeOther:
		return itr->second.byChangeOther;
		break;
	case    EFA_BecomeHead:
		return itr->second.byBecomeHead;
		break;
	case EFA_ChangeIcon:
		return itr->second.byChangeIcon;
		break;
    default:
        return FALSE;
    }
    return FALSE;
}


em_Faction_Job CFactorAuthorityPropMgr::UpJob(em_Faction_Job myJob)
{
    em_Faction_Job v_myJob = em_Faction_Normal;
    switch (myJob)
    {
    case em_Faction_MartialHead:
        v_myJob = em_Faction_MartialHead;
        break;
    case em_Faction_ProxyMartialHead:
        v_myJob = em_Faction_ProxyMartialHead;
        break;
    case em_Faction_Elder:
        v_myJob = em_Faction_ProxyMartialHead;
        break;
    case em_Faction_Custodian:
        v_myJob = em_Faction_Elder;
        break;
    case em_Faction_Elite:
        v_myJob = em_Faction_Custodian;
        break;
    case em_Faction_Normal:
        v_myJob = em_Faction_Elite;
        break;
    default:
        break;
    }
    return v_myJob;
}

em_Faction_Job CFactorAuthorityPropMgr::DownJob(em_Faction_Job myJob)
{
    em_Faction_Job v_myJob = em_Faction_Normal;
    switch (myJob)
    {
    case em_Faction_MartialHead:
        v_myJob = em_Faction_MartialHead;
        break;
    case em_Faction_ProxyMartialHead:
        v_myJob = em_Faction_Elder;
        break;
    case em_Faction_Elder:
        v_myJob = em_Faction_Custodian;
        break;
    case em_Faction_Custodian:
        v_myJob = em_Faction_Elite;
        break;
    case em_Faction_Elite:
        v_myJob = em_Faction_Normal;
        break;
    case em_Faction_Normal:
        v_myJob = em_Faction_Normal;
        break;
    default:
        break;
    }
    return v_myJob;
}

BOOL CFactorAuthorityPropMgr::CkJobNum(em_Faction_Job myJob, UINT16 wJobNum, UINT32 dwFactionLevel)
{
    BOOL		v_OK = FALSE;
	SFactinJobNumProp stProp;
	BOOL bRet = CFactorPropMgr::Instance()->GetFactionJobNum(dwFactionLevel, stProp);
	if (!bRet)
	{
		return FALSE;
	}
    switch (myJob)
    {
    case em_Faction_MartialHead:
        if (wJobNum < stProp.byMartialHead || 0 == stProp.byMartialHead)
            return TRUE;
        break;
    case em_Faction_ProxyMartialHead:
        if (wJobNum < stProp.byProxyMartialHead || 0 == stProp.byProxyMartialHead)
            return TRUE;
        break;
    case em_Faction_Elder:
        if (wJobNum < stProp.byElder || 0 == stProp.byElder)
            return TRUE;
        break;
    case em_Faction_Custodian:
        if (wJobNum < stProp.byCustodian || 0 == stProp.byCustodian)
            return TRUE;
        break;
	case em_Faction_Elite:
		if (wJobNum < stProp.byElite || 0 == stProp.byElite)
			return TRUE;
		break;
    case em_Faction_Normal:
		if (wJobNum < stProp.byNormal || 0 == stProp.byNormal)
			return TRUE;
        break;
    default:
        break;
    }
    return v_OK;
}



