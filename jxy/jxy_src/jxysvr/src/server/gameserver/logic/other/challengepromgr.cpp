

#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include "challengepromgr.h"
#include "logic/base/basepropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>

#include <db/dbmgr.h>

#define SQL_READ_DRUG_STRING "select * from cropfieldprop"


IMPLEMENT_SINGLETON_PROPMGR(CChallengePropMgr);



CChallengePropMgr::CChallengePropMgr()
{

}

CChallengePropMgr::~CChallengePropMgr()
{

}

BOOL CChallengePropMgr::Init()
{
    if(!LoadChallengePropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadChallengePropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CChallengePropMgr::UnInit()
{
    m_mapChallengePropMap.clear();
}

BOOL CChallengePropMgr::LoadChallengePropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("chanllengeprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
        return FALSE;
    }
    //查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SChallengeProp stChallengeProp;
        
        stChallengeProp.wMylevel	= SGDP::SDAtou(pRes->GetFieldValueByName("Mylevel"));
        stChallengeProp.wEnemyLevel = SGDP::SDAtou(pRes->GetFieldValueByName("EnemyLevel"));


        UINT16 wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("WKindID1"));
        UINT16 wKindIDRate = SGDP::SDAtou(pRes->GetFieldValueByName("WKindID1Pro"));
        stChallengeProp.mapComposeItemWin[wKindID] = wKindIDRate;

        wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("WKindID2"));
        wKindIDRate = SGDP::SDAtou(pRes->GetFieldValueByName("WKindID2Pro"));
        stChallengeProp.mapComposeItemWin.insert(make_pair(wKindID,wKindIDRate));

        wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("WKindID3"));
        wKindIDRate = SGDP::SDAtou(pRes->GetFieldValueByName("WKindID3Pro"));
        stChallengeProp.mapComposeItemWin.insert(make_pair(wKindID,wKindIDRate));

        wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("LKindID1"));
        wKindIDRate = SGDP::SDAtou(pRes->GetFieldValueByName("LKindID1Pro"));
        stChallengeProp.mapComposeItemLose.insert(make_pair(wKindID,wKindIDRate));

        wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("LKindID2"));
        wKindIDRate = SGDP::SDAtou(pRes->GetFieldValueByName("LKindID2Pro"));
        stChallengeProp.mapComposeItemLose.insert(make_pair(wKindID,wKindIDRate));

        wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("LKindID3"));
        wKindIDRate = SGDP::SDAtou(pRes->GetFieldValueByName("LKindID3Pro"));
        stChallengeProp.mapComposeItemLose.insert(make_pair(wKindID,wKindIDRate));

        m_mapChallengePropMap[CLogicFunc::Get1616Key(stChallengeProp.wMylevel, stChallengeProp.wEnemyLevel)] = stChallengeProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

UINT16 CChallengePropMgr::RateItemID( UINT16 wMyLevel, UINT16 wEnemyLevel, UINT8 byWin )
{
    CChallengePropMapItr itr = m_mapChallengePropMap.find(CLogicFunc::Get1616Key(wMyLevel,wEnemyLevel));
    if (itr == m_mapChallengePropMap.end())
    {
        return 0;
    }

    SChallengeProp *pSChallengeProp = &itr->second;

    CRandomVec vecRateRandom;
    CRandomVec vecItemRandom;
    if (byWin == 0)
    {
        CItemCRateMapItr itrMap = pSChallengeProp->mapComposeItemWin.begin();
        while(itrMap != pSChallengeProp->mapComposeItemWin.end())
        {
            vecRateRandom.push_back(itrMap->second);
            vecItemRandom.push_back(itrMap->first);
            itrMap++;
        }

        INT32 RanIdx = GetRandomIdxMustHit(vecRateRandom);

        return vecItemRandom[RanIdx];
    }

    CItemCRateMapItr itrMap = pSChallengeProp->mapComposeItemLose.begin();
    while(itrMap != pSChallengeProp->mapComposeItemLose.end())
    {
        vecRateRandom.push_back(itrMap->second);
        vecItemRandom.push_back(itrMap->first);
        itrMap++;
    }

    INT32 RanIdx = GetRandomIdxMustHit(vecRateRandom);

    return vecItemRandom[RanIdx];



}
