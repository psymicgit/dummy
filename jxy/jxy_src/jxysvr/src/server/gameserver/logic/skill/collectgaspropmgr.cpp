#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/player/player.h>
#include <framework/gsapi.h>
#include "collectgaspropmgr.h"
#include "logic/lua/luamgr.h"

#include <db/dbmgr.h>

using namespace SGDP;


IMPLEMENT_SINGLETON_PROPMGR(CCollectGasPropMgr);

CCollectGasPropMgr::CCollectGasPropMgr()
{
    m_byGasBallLevelNum = 0;
}

CCollectGasPropMgr::~CCollectGasPropMgr()
{

}

BOOL CCollectGasPropMgr::Init()
{
    if(!LoadGenGasPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGenGasPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadGenGasNumPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGenGasPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }


    if(!LoadUpGasLevelRatePropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadUpGasLevelRatePropFromDB failed!"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}

VOID CCollectGasPropMgr::UnInit()
{

}

BOOL CCollectGasPropMgr::LoadGenGasPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("collectgengasprop", " order by GasType, Level asc").c_str(), &pRecordSet, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            SGenGasProp stGenGasProp;
            memset(&stGenGasProp, 0, sizeof(stGenGasProp));
            stGenGasProp.byGasType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("GasType"));
            stGenGasProp.byLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("Level"));
            stGenGasProp.dwValue = SGDP::SDAtou(pRecordSet->GetFieldValueByName("Value"));
            stGenGasProp.byCoinRate = SGDP::SDAtou(pRecordSet->GetFieldValueByName("CoinRate"));
            stGenGasProp.byGoldRate = SGDP::SDAtou(pRecordSet->GetFieldValueByName("GoldRate"));
            m_vecGenGasProp.push_back(stGenGasProp);
            m_vecCoinGenGasPropRandom.push_back(stGenGasProp.byCoinRate);
            m_vecGoldGenGasPropRandom.push_back(stGenGasProp.byGoldRate);
            m_mapGenGasProp[CLogicFunc::Get88Key(stGenGasProp.byGasType, stGenGasProp.byLevel)] = stGenGasProp;
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(0 == m_vecGenGasProp.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGenGasPropFromDB has no data!"), MSG_MARK);
        return FALSE;
    }

    vector<SGenGasProp>::iterator itr;
    for (itr = m_vecGenGasProp.begin(); itr != m_vecGenGasProp.end(); itr++)
    {
        if(m_byGasBallLevelNum < itr->byLevel)
            m_byGasBallLevelNum = itr->byLevel;
    }
    return TRUE;
}

BOOL CCollectGasPropMgr::LoadGenGasNumPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("collectgengasnumprop", " order by VipLevel, Level, TownIdx, InstanceIdx asc").c_str(), &pRecordSet, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            SGasNumProp stGasNumProp;
            memset(&stGasNumProp, 0, sizeof(stGasNumProp));
            stGasNumProp.byVipLevel	= SGDP::SDAtou(pRecordSet->GetFieldValueByName("VipLevel"));
            stGasNumProp.byLevel		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("Level"));
            stGasNumProp.byTownIdx		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("TownIdx"));
            stGasNumProp.byInstanceIdx = SGDP::SDAtou(pRecordSet->GetFieldValueByName("InstanceIdx"));
            stGasNumProp.byValue		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("Value"));
            m_vectorGasNumProp.push_back(stGasNumProp);
        }
    }

    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(0 == m_vectorGasNumProp.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGenGasPropFromDB has no data!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


BOOL CCollectGasPropMgr::LoadUpGasLevelRatePropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("collectupgaslevelrateprop", " order by GasType, IncLevel asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while(pRes->GetRecord())
    {
        SUpGasLevelProp stUpGasLevelProp;
        stUpGasLevelProp.byType = SGDP::SDAtou(pRes->GetFieldValueByName("GasType"));
        stUpGasLevelProp.byIncLevel = SGDP::SDAtou(pRes->GetFieldValueByName("IncLevel"));
        stUpGasLevelProp.byRate = SGDP::SDAtou(pRes->GetFieldValueByName("Rate"));

        if (EGT_BLUE == stUpGasLevelProp.byType)
        {
            m_vecUpBlueGasLevelPropRandom.push_back(stUpGasLevelProp.byRate);
            m_vecUpBlueGasLevelProp.push_back(stUpGasLevelProp);
        }
        else
        {
            m_vecUpPurpleGasLevelPropRandom.push_back(stUpGasLevelProp.byRate);
            m_vecUpPurpleGasLevelProp.push_back(stUpGasLevelProp);
        }
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    if(0 == m_vecUpBlueGasLevelProp.size() ||
            0 == m_vecUpPurpleGasLevelProp.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: jingjieupgaslevelrateprop has no data!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

UINT32	CCollectGasPropMgr::GetGas(CPlayer *poPlayer, UINT8 byKind, UINT8 byLevel)
{
    HashMap<UINT16, SGenGasProp>::iterator itr = m_mapGenGasProp.find(CLogicFunc::Get88Key(byKind, byLevel));
    if (itr != m_mapGenGasProp.end())
    {
        return itr->second.dwValue;
    }
    return 0;

    //if (EGT_BLUE == byKind)
    //{
    //    return CLuamgr::Instance()->GetFuncObtain(poPlayer, EOF_BLUE, byLevel);
    //}
    //else
    //{
    //    return CLuamgr::Instance()->GetFuncObtain(poPlayer, EOF_PURPLE, byLevel);
    //}
}

VOID CCollectGasPropMgr::GenGas(CPlayer *poPlayer, UINT8 byOptFlag, DT_GAS_BALL_DATA& stDT_GAS_BALL_DATA)
{
    memset(&stDT_GAS_BALL_DATA, 0, sizeof(stDT_GAS_BALL_DATA));
    if (0 == byOptFlag)
    {
        //随机
        INT32 nRandomHitIdx = GetRandomIdxMustHit(m_vecCoinGenGasPropRandom);
        if(RANDOM_UNHIT == nRandomHitIdx)
        {
            return;
        }
        stDT_GAS_BALL_DATA.byKind		=	m_vecGenGasProp[nRandomHitIdx].byGasType;
        stDT_GAS_BALL_DATA.byLevel		=	m_vecGenGasProp[nRandomHitIdx].byLevel;
        stDT_GAS_BALL_DATA.dwGasValue	=	m_vecGenGasProp[nRandomHitIdx].dwValue;
    }
    else
    {
        //随机
        INT32 nRandomHitIdx = GetRandomIdxMustHit(m_vecGoldGenGasPropRandom);
        if(RANDOM_UNHIT == nRandomHitIdx)
        {
            return;
        }
        stDT_GAS_BALL_DATA.byKind		=	m_vecGenGasProp[nRandomHitIdx].byGasType;
        stDT_GAS_BALL_DATA.byLevel		=	m_vecGenGasProp[nRandomHitIdx].byLevel;
        stDT_GAS_BALL_DATA.dwGasValue	=	m_vecGenGasProp[nRandomHitIdx].dwValue;
    }
}

VOID CCollectGasPropMgr::UpGasBallLevel(CPlayer *poPlayer, UINT8 byFullLevelFlag, DT_GAS_BALL_DATA &stDT_GAS_BALL_DATA)
{
    if(1 == byFullLevelFlag)
    {
        stDT_GAS_BALL_DATA.byLevel		= m_byGasBallLevelNum;
        stDT_GAS_BALL_DATA.dwGasValue	= GetGas(poPlayer, stDT_GAS_BALL_DATA.byKind, stDT_GAS_BALL_DATA.byLevel);
    }
    else
    {
        if (EGT_BLUE == stDT_GAS_BALL_DATA.byKind)
        {
            //随机
            INT32 nRandomHitIdx = GetRandomIdxMustHit(m_vecUpBlueGasLevelPropRandom);
            if(RANDOM_UNHIT == nRandomHitIdx)
            {
                return;
            }
            UINT8 byIncLevel = m_vecUpBlueGasLevelProp[nRandomHitIdx].byIncLevel;
            UINT8 byTmp = (stDT_GAS_BALL_DATA.byLevel + byIncLevel) % (m_byGasBallLevelNum + 1);
            stDT_GAS_BALL_DATA.byLevel = byTmp >= stDT_GAS_BALL_DATA.byLevel ? byTmp : byTmp + 1;

            stDT_GAS_BALL_DATA.dwGasValue = GetGas(poPlayer, stDT_GAS_BALL_DATA.byKind, stDT_GAS_BALL_DATA.byLevel);
        }
        else
        {
            //随机
            INT32 nRandomHitIdx = GetRandomIdxMustHit(m_vecUpPurpleGasLevelPropRandom);
            if(RANDOM_UNHIT == nRandomHitIdx)
            {
                return;
            }
            UINT8 byIncLevel = m_vecUpPurpleGasLevelProp[nRandomHitIdx].byIncLevel;
            UINT8 byTmp = (stDT_GAS_BALL_DATA.byLevel + byIncLevel) % (m_byGasBallLevelNum + 1);
            stDT_GAS_BALL_DATA.byLevel = byTmp >= stDT_GAS_BALL_DATA.byLevel ? byTmp : byTmp + 1;

            stDT_GAS_BALL_DATA.dwGasValue = GetGas(poPlayer, stDT_GAS_BALL_DATA.byKind, stDT_GAS_BALL_DATA.byLevel);
        }
    }
}

UINT32 CCollectGasPropMgr::GetGuidePurpleGas(CPlayer *poPlayer)
{
    return 4 * GetGas(poPlayer, EGT_BLUE, m_byGasBallLevelNum) + 4 * GetGas(poPlayer, EGT_PURPLE, 6);
}

//获得聚气的次数
UINT32 CCollectGasPropMgr::GetGenGasNum(UINT8 byVipLevel, UINT8 byLevel, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    UINT8	byValue = 0;
    CGasNumPropVectorItr itr;
    for ( itr = m_vectorGasNumProp.begin(); itr != m_vectorGasNumProp.end(); itr++)
    {
        if ((byVipLevel < itr->byVipLevel || byLevel < itr->byLevel || byTownIdx < itr->byTownIdx) || (byTownIdx == itr->byTownIdx && byInstanceIdx < itr->byInstanceIdx))
        {
            return  byValue;
        }
        byValue = itr->byValue;
    }
    return byValue;
}

SGasNumProp * CCollectGasPropMgr::GetNextGenGasNumProp(UINT8 byVipLevel, UINT8 byLevel, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    UINT8	byValue = 0;
    CGasNumPropVectorItr itr;
    for ( itr = m_vectorGasNumProp.begin(); itr != m_vectorGasNumProp.end(); itr++)
    {
        byValue = itr->byValue;
        if ((byVipLevel < itr->byVipLevel || byLevel < itr->byLevel || byTownIdx < itr->byTownIdx) || (byTownIdx == itr->byTownIdx && byInstanceIdx < itr->byInstanceIdx))
        {
            itr++;
            break;
        }
    }
    if (itr != m_vectorGasNumProp.end())
    {
        return &(*itr);
    }
    else
    {
        return NULL;
    }
}



