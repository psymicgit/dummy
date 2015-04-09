#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapi.h>
#include "bossbattlepropmgr.h"

#include <db/dbmgr.h>

IMPLEMENT_SINGLETON_PROPMGR(CBossBattlePropMgr);

CBossBattlePropMgr::CBossBattlePropMgr()
{
    Clr();
}

CBossBattlePropMgr::~CBossBattlePropMgr()
{

}

BOOL CBossBattlePropMgr::Init()
{
    if(!LoadBossBPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadBossBPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


VOID CBossBattlePropMgr::UnInit()
{
    Clr();

}


VOID CBossBattlePropMgr::Clr()
{

}

SBossBProp* CBossBattlePropMgr::GetBossBPropByID(UINT8 byActivityId)
{
    map<UINT8, SBossBProp>::iterator itr = m_mapBossBProp.find(byActivityId);
    if(itr != m_mapBossBProp.end())
    {
        return &itr->second;
    }

    return NULL;
}



BOOL CBossBattlePropMgr::LoadBossBPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from bossbattleprop order by ActivityId", &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return TRUE; //
    }
    while(pRes->GetRecord())
    {
        SBossBProp stProp;
        memset(&stProp, 0, sizeof(stProp));
        stProp.byActivityId = SGDP::SDAtou(pRes->GetFieldValueByName("ActivityId"));
        stProp.wBossMonsterID = SGDP::SDAtou(pRes->GetFieldValueByName("BossMonsterID"));
        stProp.wBossStartLevel = SGDP::SDAtou(pRes->GetFieldValueByName("BossStartLevel"));
        stProp.wBossMaxLevel = SGDP::SDAtou(pRes->GetFieldValueByName("BossMaxLevel"));
        CHAR szTmp[5] = {0};
        SDStrncpy(szTmp, pRes->GetFieldValueByName("StartTime"), 4);
        CHAR szTmp2[3] = {0};
        memcpy(szTmp2, szTmp, 2);
        stProp.byStartHour = SDAtou(szTmp2);
        stProp.byStartMin = SDAtou(szTmp + 2);
        if((stProp.byStartHour > 23) || (stProp.byStartMin > 59))
        {
            DBG_INFO(_SDT("[%s: %d]: load bossbattleprop StartTime invalid, Hour[%d], Min[%d]!"), MSG_MARK, stProp.byStartHour, stProp.byStartMin);
            return FALSE;
        }
        stProp.wKeepTime = SDAtou(pRes->GetFieldValueByName("KeepTime"));
        stProp.dwStartTime = stProp.byStartHour * 3600 + stProp.byStartMin * 60;
        stProp.dwEndTime = stProp.dwStartTime + stProp.wKeepTime;

        stProp.wBirthPosX = SGDP::SDAtou(pRes->GetFieldValueByName("BirthPosX"));
        stProp.wBirthPosY = SGDP::SDAtou(pRes->GetFieldValueByName("BirthPosY"));
        stProp.byShowNum = SGDP::SDAtou(pRes->GetFieldValueByName("ShowNum"));
        stProp.wReliveCD = SGDP::SDAtou(pRes->GetFieldValueByName("ReliveCD"));
        stProp.wGoldReliveCD = SGDP::SDAtou(pRes->GetFieldValueByName("GoldReliveCD"));
        stProp.wReliveGold = SGDP::SDAtou(pRes->GetFieldValueByName("ReliveGold"));
        stProp.wMoveMinCD = SGDP::SDAtou(pRes->GetFieldValueByName("MoveMinCD"));
        stProp.wMoveMaxLen = SGDP::SDAtou(pRes->GetFieldValueByName("MoveMaxLen"));
        stProp.wMaxReadyPosY = SGDP::SDAtou(pRes->GetFieldValueByName("MaxReadyPosY"));
        stProp.wReadyCD = SGDP::SDAtou(pRes->GetFieldValueByName("ReadyCD"));
        //stProp.dwReadyOverTime = stProp.dwStartTime + stProp.wReadyCD;
        stProp.dwStartEnterTime = stProp.dwStartTime - stProp.wReadyCD;
        stProp.wUpgradeCostMaxKillSecs = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeCostMaxKillSecs"));
        stProp.wIncCostStory = SGDP::SDAtou(pRes->GetFieldValueByName("IncCostStory"));
        stProp.wIncCostGold = SGDP::SDAtou(pRes->GetFieldValueByName("IncCostGold"));
        stProp.wCanIncPowerRate = SGDP::SDAtou(pRes->GetFieldValueByName("CanIncPowerRate"));
        stProp.wDealNum = SGDP::SDAtou(pRes->GetFieldValueByName("DealNum"));
        stProp.wDealNum = stProp.wDealNum == 0 ? 10 : stProp.wDealNum;
        stProp.wDealInterval = SGDP::SDAtou(pRes->GetFieldValueByName("DealInterval"));
        stProp.wDealInterval = stProp.wDealInterval == 0 ? 10 : stProp.wDealInterval;
        stProp.wOpenActivityPassDay = SGDP::SDAtou(pRes->GetFieldValueByName("OpenActivityPassDay"));


        m_mapBossBProp[stProp.byActivityId] = stProp;
        m_mapBossBStartTimeProp[stProp.dwStartEnterTime] = stProp;
        m_mapBossBEndTimeProp[stProp.dwEndTime] = stProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}


const SBossBProp* CBossBattlePropMgr::GetActivityTimeProp(UINT64 qwEnterTime)
{
    UINT32 dwDayPassSecond = GetDayPassSecond(qwEnterTime);
    CBossBActivityTimePropMapItr itr = m_mapBossBEndTimeProp.upper_bound(dwDayPassSecond); //key为结束时间dwEndTime
    if(itr != m_mapBossBEndTimeProp.end())
    {
        SBossBProp* pstProp = &itr->second;
        if(pstProp->dwStartEnterTime <= dwDayPassSecond)
        {
            return pstProp;
        }
    }

    return NULL;

}

//获取指定时候后开始的活动时间段配置, 当天的已经没有活动则返回明天的第一个活动
const SBossBProp* CBossBattlePropMgr::GetNextActivityTimeProp(UINT64 qwStartTime)
{
    UINT32 dwDayPassSecond = GetDayPassSecond(qwStartTime);
    CBossBActivityTimePropMapItr itr = m_mapBossBStartTimeProp.upper_bound(dwDayPassSecond); //key为结束时间dwStartTime
    if(itr != m_mapBossBStartTimeProp.end())
    {
        return 	&itr->second;
    }

    if(0 == m_mapBossBStartTimeProp.size())
    {
        return NULL;
    }

    return &m_mapBossBStartTimeProp.begin()->second;
}
