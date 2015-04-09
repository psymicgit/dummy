#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapi.h>

#include "awakenpropmgr.h"

#include <db/dbmgr.h>

using namespace SGDP;


IMPLEMENT_SINGLETON_PROPMGR(CAwakenPropMgr);

CAwakenPropMgr::CAwakenPropMgr()
{
    memset(m_awStudyUnlockLevelInfo, 0, sizeof(m_awStudyUnlockLevelInfo));

    memset(m_adwAwakenCoin, 0, sizeof(m_adwAwakenCoin));
}

CAwakenPropMgr::~CAwakenPropMgr()
{
}

BOOL CAwakenPropMgr::Init()
{
    if(!LoadBaseFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadBaseFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadAwakenLevelFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadAwakenLevelFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadUnlockHeroStudyGridFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadUnlockHeroStudyGridFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadLevel2UnlockAwakenStudyFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadLevel2UnlockAwakenStudyFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadAwakenStudyAttrFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadAwakenStudyAttrFromDB failed!"), MSG_MARK);
        return FALSE;
    }



    return TRUE;
}

VOID CAwakenPropMgr::UnInit()
{
    for(UINT8 byIdx = 0; byIdx < MAX_AWAKEN_LEVEL + 1; byIdx++)
    {
        m_astAwakenLevelProp[byIdx].vecRandom.clear();
    }
    m_stRudingGoldAwakenLevelProp.vecRandom.clear();
    m_stChewuGoldAwakenLevelProp.vecRandom.clear();

    m_mapLevel2AwakenStudyGenUnlockPropMap.clear();

    m_mapAwakenStudyAttrProp.clear();

    memset(m_awStudyUnlockLevelInfo, 0, sizeof(m_awStudyUnlockLevelInfo));

    memset(m_adwAwakenCoin, 0, sizeof(m_adwAwakenCoin));


    m_mapLevel2OpenNumProp.clear();

}


// 从数据源获取数据
BOOL CAwakenPropMgr::LoadBaseFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenbaseprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    if(pRes->GetRecord())// 当有行结果时
    {
        memset(&m_stAwakenBaseProp, 0, sizeof(SAwakenBaseProp));
        m_stAwakenBaseProp.dwGoldAwaken = SGDP::SDAtou(pRes->GetFieldValueByName("GoldAwaken"));
        m_stAwakenBaseProp.dwGenStudyCoin = SGDP::SDAtou(pRes->GetFieldValueByName("GenStudyCoin"));
        if(0 == m_stAwakenBaseProp.dwGenStudyCoin)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GenStudyCoin can not 0!"), MSG_MARK);
            return FALSE;
        }
        m_stAwakenBaseProp.byMaxStudyLevel = SGDP::SDAtou(pRes->GetFieldValueByName("MaxStudyLevel"));
        if(0 == m_stAwakenBaseProp.byMaxStudyLevel)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: MaxStudyLevel can not 0!"), MSG_MARK);
            return FALSE;
        }
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: awakenbaseprop have no data!"), MSG_MARK);
        return FALSE;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

BOOL CAwakenPropMgr::LoadAwakenLevelFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenlevelprop", " order by AwakenLevel").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    UINT8 byRowNum = 0;
    SAwakenLevelProp* pstProp = NULL;
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT8 byAwakenLevel = SGDP::SDAtou(pRes->GetFieldValueByName("AwakenLevel"));
        if(byAwakenLevel == GOLD_RUDING_PROP)
        {
            pstProp = &m_stRudingGoldAwakenLevelProp;
        }
        else if(byAwakenLevel == GOLD_CHEWU_PROP)
        {
            pstProp = &m_stChewuGoldAwakenLevelProp;
        }
        else if((byAwakenLevel <= MAX_AWAKEN_LEVEL) && (0 != byAwakenLevel))
        {
            pstProp = &m_astAwakenLevelProp[byAwakenLevel];
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s: %d]: awakenlevelprop invalid AwakenLevel[%d]!"), MSG_MARK, byAwakenLevel);
            return FALSE;
        }

        pstProp->byAwakenLevel = byAwakenLevel;
        pstProp->dwNeedCoin = SGDP::SDAtou(pRes->GetFieldValueByName("NeedCoin"));
        pstProp->byNextLevelRate = SGDP::SDAtou(pRes->GetFieldValueByName("NextLevelRate"));
        pstProp->byCoinRate = SGDP::SDAtou(pRes->GetFieldValueByName("CoinRate"));
        pstProp->byBuleStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("BuleStudyRate"));
        pstProp->byPurpleStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("PurpleStudyRate"));
        pstProp->byRedStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("RedStudyRate"));
        pstProp->byOrangeStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("OrangeStudyRate"));
        pstProp->vecRandom.push_back(pstProp->byCoinRate);
        pstProp->vecRandom.push_back(pstProp->byBuleStudyRate);
        pstProp->vecRandom.push_back(pstProp->byPurpleStudyRate);
        pstProp->vecRandom.push_back(pstProp->byRedStudyRate);
        pstProp->vecRandom.push_back(pstProp->byOrangeStudyRate);

        if((byAwakenLevel < MAX_AWAKEN_NUM) && (byAwakenLevel > 0))
        {
            m_adwAwakenCoin[byAwakenLevel - 1] = pstProp->dwNeedCoin;
        }

        byRowNum++;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    if(byRowNum != MAX_AWAKEN_LEVEL + 2)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: awakenlevelprop row[%d] not match[%d] !"), MSG_MARK, byRowNum, MAX_AWAKEN_LEVEL + 2);
        return FALSE;
    }

    return TRUE;
}


BOOL CAwakenPropMgr::LoadUnlockHeroStudyGridFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenunlockherostudygridprop", " order by UnlockGridNum").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    m_mapOpenNum2LevelProp.clear();
    BOOL bHaveData = FALSE;
    // 打印查询的表结果
    UINT16 wLastLevel = 0;
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT16 wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedHeroLevel"));
        if(wLevel <= wLastLevel)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: wLevel <= wLastLevel !"), MSG_MARK);
            return FALSE;
        }
        UINT8 byUnlockGridNum = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockGridNum"));
        if((byUnlockGridNum > MAX_HERO_STUDY_GRID_NUM) || (0 == byUnlockGridNum))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: UnlockGridNum[%d] invalid !"), MSG_MARK, byUnlockGridNum);
            return FALSE;
        }
        //填充之间的数据
        for(UINT16 wIdx = wLastLevel + 1; wIdx < wLevel; wIdx++)
        {
            m_mapLevel2OpenNumProp[wIdx] = byUnlockGridNum;
        }
        m_mapLevel2OpenNumProp[wLevel] = byUnlockGridNum;
        m_awStudyUnlockLevelInfo[byUnlockGridNum - 1] = wLevel;

        m_mapOpenNum2LevelProp[byUnlockGridNum] = wLevel;

        wLastLevel = wLevel;

        bHaveData = TRUE;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    if(!bHaveData)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: awakenunlockherostudygridprop have no data!"), MSG_MARK);
        return FALSE;
    }

    m_wMaxOpenNumHeroLevel = wLastLevel;

    return TRUE;
}



BOOL CAwakenPropMgr::LoadLevel2UnlockAwakenStudyFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenstudygenunlockprop", " order by PlayerLevel, StudyColorKind, StudyAttrKind").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    UINT16 wLastLevel = 0; //从0开始才对，否则等级1检查不到
    while(pRes->GetRecord())// 当有行结果时
    {
        SAwakenStudyGenUnlockProp stProp;
        stProp.wPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("PlayerLevel"));

        //如果配置中出现了不连贯的等级配置
        if(stProp.wPlayerLevel > wLastLevel + 1)
        {
            //等级1未配置，报错
            if (wLastLevel == 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: awakenstudygenunlockprop player level[%d] miss!"), MSG_MARK, wLastLevel + 1);
                return FALSE;
            }

            //配置中两个不连续的等级之间的配置需要填充
            SPlayerAwakenStudyGenUnlockProp& stLastLevelProp = m_mapLevel2AwakenStudyGenUnlockPropMap[wLastLevel];
            for(UINT16 wLevelIdx = wLastLevel + 1; wLevelIdx < stProp.wPlayerLevel; wLevelIdx ++)
            {
                m_mapLevel2AwakenStudyGenUnlockPropMap[wLevelIdx] = stLastLevelProp;
            }
        }

        stProp.byStudyColorKind = SGDP::SDAtou(pRes->GetFieldValueByName("StudyColorKind"));
        stProp.byStudyAttrKind = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind"));
        stProp.byGenRate = SGDP::SDAtou(pRes->GetFieldValueByName("GenRate"));

        SPlayerAwakenStudyGenUnlockProp& stPlayerAwakenStudyGenUnlockProp = m_mapLevel2AwakenStudyGenUnlockPropMap[stProp.wPlayerLevel];
        stPlayerAwakenStudyGenUnlockProp.wPlayerLevel = stProp.wPlayerLevel;
        stPlayerAwakenStudyGenUnlockProp.avecAwakenStudyGenUnlockProp[stProp.byStudyColorKind].push_back(stProp);
        stPlayerAwakenStudyGenUnlockProp.avecRandom[stProp.byStudyColorKind].push_back(stProp.byGenRate);

        wLastLevel = stProp.wPlayerLevel;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    UINT16 wMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();

    //配置中的最后等级和玩家的最大等级之间的这部分数据需要填充
    if(wLastLevel < wMaxPlayerLevel)
    {
        SPlayerAwakenStudyGenUnlockProp stLastLevelProp = m_mapLevel2AwakenStudyGenUnlockPropMap[wLastLevel];
        for(UINT16 wLevelIdx = wLastLevel + 1; wLevelIdx <= wMaxPlayerLevel; wLevelIdx ++)
        {
            m_mapLevel2AwakenStudyGenUnlockPropMap[wLevelIdx] = stLastLevelProp;
        }
    }


    return TRUE;
}


BOOL CAwakenPropMgr::LoadAwakenStudyAttrFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenstudyattrprop", " order by StudyColorKind, StudyAttrKind, StudyLevel").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while(pRes->GetRecord())// 当有行结果时
    {
        SAwakenStudyAttrProp stProp;
        stProp.byStudyColorKind = SGDP::SDAtou(pRes->GetFieldValueByName("StudyColorKind"));
        stProp.byStudyAttrKind = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind"));
        stProp.byStudyLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StudyLevel"));
        stProp.dwAttrValue = SGDP::SDAtou(pRes->GetFieldValueByName("AttrValue"));
        stProp.dwHaveExp = SGDP::SDAtou(pRes->GetFieldValueByName("HaveExp"));

        //有效性检查,todo
        m_mapAwakenStudyAttrProp[CLogicFunc::Get1688Key(stProp.byStudyColorKind, stProp.byStudyAttrKind, stProp.byStudyLevel)] = stProp;
    }


    return TRUE;
}

UINT32 CAwakenPropMgr::GetAwakenCoin(UINT8 byGoldFlag, UINT8 byAwakenLevel)
{
    if((0 == byAwakenLevel) || (byAwakenLevel > MAX_AWAKEN_LEVEL))
    {
        return 0xFFFFFFFF;
    }
    if((1 == byGoldFlag) && (EALK_RUDING == byAwakenLevel))
    {
        return m_stRudingGoldAwakenLevelProp.dwNeedCoin;
    }
    else if((1 == byGoldFlag) && (EALK_CHEWU == byAwakenLevel))
    {
        return m_stChewuGoldAwakenLevelProp.dwNeedCoin;
    }
    return m_astAwakenLevelProp[byAwakenLevel].dwNeedCoin;
}


UINT8 CAwakenPropMgr::AwakenStudy(UINT16 wPlayerLevel, UINT8 byAwakenLevel, UINT8 byGoldFlag, UINT8& byStudyColorKind, UINT8& byStudyAttrKind, UINT32& dwGenCoin)
{
    UINT8 byNextAwakenLevel = EALK_KESHUI;
    byStudyColorKind = 0;
    byStudyAttrKind = 0;
    dwGenCoin = m_stAwakenBaseProp.dwGenStudyCoin;
    if(((0 == byAwakenLevel) || (byAwakenLevel > MAX_AWAKEN_LEVEL) || (0 == wPlayerLevel)
            || (wPlayerLevel > CSinglePramMgr::Instance()->GetMaxPlayerLevel())))
    {
        return byNextAwakenLevel;
    }
    SAwakenLevelProp* pstAwakenLevelProp = NULL;
    if((1 == byGoldFlag) && (EALK_RUDING == byAwakenLevel))
    {
        pstAwakenLevelProp = &m_stRudingGoldAwakenLevelProp;
    }
    else if((1 == byGoldFlag) && (EALK_CHEWU == byAwakenLevel))
    {
        pstAwakenLevelProp = &m_stChewuGoldAwakenLevelProp;
    }
    else
    {
        pstAwakenLevelProp = &m_astAwakenLevelProp[byAwakenLevel];
    }

    //计算悟道下一级的概率
    if(RandomReseed(100) < pstAwakenLevelProp->byNextLevelRate)
    {
        byNextAwakenLevel = byAwakenLevel + 1;
        if(byNextAwakenLevel > EALK_CHEWU)
        {
            byNextAwakenLevel = EALK_KESHUI;
        }
    }
    else
    {
        byNextAwakenLevel = EALK_KESHUI;
    }

    //计算生成的心得颜色
    byStudyColorKind = GetRandomIdxMustHit(pstAwakenLevelProp->vecRandom);
    if((0 == byStudyColorKind) || (byStudyColorKind > EC_ORANGE))
    {
        byStudyColorKind = 0;
        return byNextAwakenLevel;
    }

    //计算生成的心得类型
    SPlayerAwakenStudyGenUnlockProp& stColor = m_mapLevel2AwakenStudyGenUnlockPropMap[wPlayerLevel];
    CRandomVec* poRandomVec = NULL;
    poRandomVec = &(stColor.avecRandom[byStudyColorKind]);
    if(0 == poRandomVec->size()) //没有解锁该颜色，则取蓝色
    {
        byStudyColorKind = EC_BLUE;
        poRandomVec = &(stColor.avecRandom[byStudyColorKind]);
    }
    if(0 == poRandomVec->size())
    {
        byStudyColorKind = 0;
        return byNextAwakenLevel;
    }
    byStudyAttrKind;
    INT32 nHit = GetRandomIdxMustHit(*poRandomVec);
    byStudyAttrKind = stColor.avecAwakenStudyGenUnlockProp[byStudyColorKind][nHit].byStudyAttrKind;

    return byNextAwakenLevel;
}


UINT32 CAwakenPropMgr::GetStudyExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel)
{
    CAwakenStudyAttrPropMapItr itr = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get1688Key(byStudyColorKind, byStudyAttrKind, byStudyLevel));
    if(itr != m_mapAwakenStudyAttrProp.end())
    {
        return itr->second.dwHaveExp;
    }

    return 0;
}

UINT32 CAwakenPropMgr::GetStudyUpgradeNeedExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyCurLevel)
{
    UINT32 dwCurLevelExp = 0;
    UINT32 dwNextLevelExp = 0;
    CAwakenStudyAttrPropMapItr itrCur = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get1688Key(byStudyColorKind, byStudyAttrKind, byStudyCurLevel));
    if(itrCur == m_mapAwakenStudyAttrProp.end())
    {
        return 0xFFFFFFFF;
    }
    dwCurLevelExp = itrCur->second.dwHaveExp;
    CAwakenStudyAttrPropMapItr itrNext = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get1688Key(byStudyColorKind, byStudyAttrKind, byStudyCurLevel + 1));
    if(itrNext == m_mapAwakenStudyAttrProp.end())
    {
        return 0xFFFFFFFF;
    }
    dwNextLevelExp = itrNext->second.dwHaveExp;

    UINT32 dwCostExp = dwNextLevelExp < dwCurLevelExp ? 0 : dwNextLevelExp - dwCurLevelExp;
    return dwCostExp;
}

BOOL CAwakenPropMgr::CkUnlockHeroGrid(UINT16 wHeroLevel, UINT8 byGridIdx)
{
    C1616MapItr itr = m_mapOpenNum2LevelProp.find(byGridIdx + 1);
    if(itr != m_mapOpenNum2LevelProp.end())
    {
        if(itr->second <= wHeroLevel)
        {
            return TRUE;
        }
    }

    return FALSE;
}


UINT32 CAwakenPropMgr::GetStudyAttrValue(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel)
{
    CAwakenStudyAttrPropMapItr itr = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get1688Key(byStudyColorKind, byStudyAttrKind, byStudyLevel));
    if(itr != m_mapAwakenStudyAttrProp.end())
    {
        return itr->second.dwAttrValue;
    }

    return 0;
}

