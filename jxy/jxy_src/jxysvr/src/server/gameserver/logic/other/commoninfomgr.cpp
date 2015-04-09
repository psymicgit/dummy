#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <sdloggerimpl.h>
#include "commoninfomgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <logic/player/player.h>

IMPLEMENT_SINGLETON_PROPMGR(CCommonInfoMgr);

CCommonInfoMgr::CCommonInfoMgr()
{

}

CCommonInfoMgr::~CCommonInfoMgr()
{
}

BOOL CCommonInfoMgr::Init()
{
    return LoadFromDB();
}

BOOL CCommonInfoMgr::LoadFromDB()
{
    if(!LoadCliVerInfoFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadCliVerInfoFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadDspnamePoolFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadDspnamePoolFromDB failed!"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}

VOID CCommonInfoMgr::UnInit()
{
    m_mapCliTypeVerInfo.clear();
    m_vecFirstName.clear();
    m_vecMaleName.clear();
    m_vecFemaleName.clear();
    //m_mapPlayerID2CacheName.clear();
}


// 从数据源获取数据
BOOL CCommonInfoMgr::LoadCliVerInfoFromDB()
{
    //SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    //if(NULL == pDBSession)
    //{
    //	return FALSE;
    //}
    //SGDP::ISDDBRecordSet* pRes = NULL;
    //UINT32 dwErrID = 0;
    //string strErr;
    //INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("cliverinfo", " order by CLiVer;").c_str(), &pRes, &dwErrID,  &strErr);
    //if(SDDB_HAS_RECORDSET != nRet)
    //{
    //	return TRUE;
    //}
//   while(pRes->GetRecord())// 当有行结果时
//   {
    //	SCliVerInfoProp stProp;
//       stProp.dwCliVer = SGDP::SDAtou(pRes->GetFieldValueByName("CLiVer"));
    //	stProp.dwResVer = SGDP::SDAtou(pRes->GetFieldValueByName("ResVer"));
    //	stProp.dwUIVer = SGDP::SDAtou(pRes->GetFieldValueByName("UIVer"));
    //	m_mapCliTypeVerInfo[stProp.dwCliVer] = stProp;
//   }
//   if(NULL != pRes)
//   {
//       pRes->Release();
//       pRes = NULL;
//   }

    return TRUE;
}


VOID CCommonInfoMgr::GetCliVerInfo(UINT8 byAuthType, UINT32 dwCliVer, UINT32& dwResVer, UINT32& dwUIVer, TCHAR aszResVer[MAX_RES_VER_LEN])
{
    UINT8 	byCliType = 0;
    if (byAuthType < 100)
    {
        byCliType = 0;
    }
    else
    {
        byCliType = 1;
    }

    CCliTypeVerInfoMapItr itr = m_mapCliTypeVerInfo.find(byCliType);
    if(itr == m_mapCliTypeVerInfo.end())
    {
        dwResVer = 0;
        dwUIVer = 0;
        return;
    }
    CCliVerInfoMap &	mapCliVerInfo  = itr->second.mapCliVerInfo;
    CCliVerInfoMapItr v_itr = mapCliVerInfo.find(dwCliVer);
    if (v_itr != mapCliVerInfo.end())
    {
        memcpy(aszResVer, v_itr->second.aszResVer, MAX_RES_VER_LEN);
    }
}


BOOL CCommonInfoMgr::LoadDspnamePoolFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("dspnamepool").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        return TRUE;
    }
    while(pRes->GetRecord())// 当有行结果时
    {
        TCHAR szBuff[32] = {0};
        strcpy(szBuff, pRes->GetFieldValueByName("FamilyName"));
        if('\0' != szBuff[0])
        {
            m_vecFirstName.push_back(szBuff);
        }
        strcpy(szBuff, pRes->GetFieldValueByName("MaleName"));
        if('\0' != szBuff[0])
        {
            m_vecMaleName.push_back(szBuff);
        }
        strcpy(szBuff, pRes->GetFieldValueByName("FemaleName"));
        if('\0' != szBuff[0])
        {
            m_vecFemaleName.push_back(szBuff);
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

tstring CCommonInfoMgr::GetRdDspName(CPlayer* poPlayer, UINT8 byReRandom)
{
    if(NULL == poPlayer)
    {
        return "";
    }
    UINT32 dwPlayerID = poPlayer->GetID();
    if(0 == byReRandom)
    {
        CPlayerID2NameMapItr itr = m_mapPlayerID2CacheName.find(dwPlayerID);
        if(itr != m_mapPlayerID2CacheName.end())
        {
            return itr->second;
        }
    }

    if(0 == m_vecFirstName.size() || 0 == m_vecMaleName.size() || 0 == m_vecFemaleName.size())
    {
        return "";
    }

    CDspNameVec* poSecondNameVec = &m_vecMaleName;
    switch(poPlayer->GetCoachHeroKindID())
    {
    case 1:
    case 3:
        poSecondNameVec = &m_vecMaleName;
        break;
    case 4:
    case 6:
        poSecondNameVec = &m_vecFemaleName;
        break;
    default:
        break;

    }

    //最多循环255次
    UINT32 dwFirstNameSize = m_vecFirstName.size();
    UINT32 dwSecondNameSize = poSecondNameVec->size();
    for(UINT8 byIdx = 0; byIdx < 255; byIdx++)
    {
        tstring strTmp = m_vecFirstName[RandomReseed(dwFirstNameSize)] + (*poSecondNameVec)[RandomReseed(dwSecondNameSize)];
        if((!CPlayerBaseDataCacheMgr::Instance()->CkHaveDspName(strTmp.c_str())) && (!CkDspnameInCache(strTmp.c_str(), dwPlayerID)))
        {
            m_mapPlayerID2CacheName[dwPlayerID] = strTmp;
            m_mapCacheName2PlayerID[strTmp] = dwPlayerID;

            return strTmp;
        }
    }

    return "";
}


BOOL CCommonInfoMgr::CkDspnameInCache(const TCHAR* pszDispName, UINT32 dwPlayerID)
{
    CName2PlayerIDMapItr itr = m_mapCacheName2PlayerID.find(pszDispName);
    if((itr != m_mapCacheName2PlayerID.end()) && (itr->second != dwPlayerID))
    {
        return TRUE;
    }

    return FALSE;
}


VOID CCommonInfoMgr::OnRegDspname(UINT32 dwPlayerID)
{
    m_mapPlayerID2CacheName.erase(dwPlayerID);
}


