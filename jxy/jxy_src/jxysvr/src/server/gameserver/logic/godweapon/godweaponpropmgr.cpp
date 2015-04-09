#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/hero/heropropmgr.h>
#include <framework/gsapi.h>
#include "godweaponbasepropmgr.h"
#include "godweaponpropmgr.h"

#include <db/dbmgr.h>

using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(CGodweaponPropMgr);

CGodweaponPropMgr::CGodweaponPropMgr()
{
    m_byQualityUpdateNum = 0;
}

CGodweaponPropMgr::~CGodweaponPropMgr()
{

}

BOOL CGodweaponPropMgr::Init()
{
    m_byQualityUpdateNum = CGodweaponBasePropMgr::Instance()->GetQualityUpdateNum();

    if(!LoadQualityUpgradeFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadQualityUpgradeFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadLevelUpgradeFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadLevelUpgradeFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitUpgradeTotalScience())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitUpgradeTotalScience failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitQualityLevelHaveAttr())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitQualityLevelHaveAttr failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CGodweaponPropMgr::UnInit()
{
    m_mapGodweaponQualityUpgradeProp.clear();
    m_mapGodweaponLevelUpgradeProp.clear();
    m_mapUpgradeScience.clear();
    m_mapUpgradeTotalScience.clear();
    m_mapHeroFullGodweaponIncAttr.clear();
    m_mapHeroFullGodweaponEncAttr.clear();
    m_mapHeroID2EncAttrProp.clear();

}


BOOL CGodweaponPropMgr::LoadQualityUpgradeFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("godweaponqualityupgradeprop", " order by HeroKindID, QualityLevel").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SGodweaponQualityUpgradeProp stQualityUpgradeProp;
        memset(&stQualityUpgradeProp, 0, sizeof(stQualityUpgradeProp));
        stQualityUpgradeProp.wHeroKindID =  SGDP::SDAtou(pRes->GetFieldValueByName("HeroKindID"));
        stQualityUpgradeProp.wQualityLevel = SGDP::SDAtou(pRes->GetFieldValueByName("QualityLevel"));

        _SDTStrncpy(stQualityUpgradeProp.szQualityName, _SDTUTF82T(pRes->GetFieldValueByName("QualityName")), QUALITYNAME_LEN_MAX);
        stQualityUpgradeProp.wRequirePlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("RequirePlayerLevel"));
        stQualityUpgradeProp.wUpgradeQualityItemNum = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeQualityItemNum"));
        for(UINT8 byIdx = 0; byIdx < MAX_UPGRADE_QUALITY_ATTR_NUM; byIdx++)
        {
            CHAR szTmp[32] = {0};
            sprintf(szTmp, "BaseAttrKind%d", byIdx + 1);
            stQualityUpgradeProp.abyBaseAttrKind[byIdx] = SGDP::SDAtou(pRes->GetFieldValueByName(szTmp));
            sprintf(szTmp, "BaseAttrValue%d", byIdx + 1);
            stQualityUpgradeProp.adwBaseAttrValue[byIdx] = SGDP::SDAtou(pRes->GetFieldValueByName(szTmp));
        }
        stQualityUpgradeProp.byQualityUpgradeEncAttrKind = SGDP::SDAtou(pRes->GetFieldValueByName("QualityUpgradeEncAttrKind"));
        stQualityUpgradeProp.dwUpgradeQualityEncAttrValue = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeQualityEncAttrValue"));
        m_mapGodweaponQualityUpgradeProp[CLogicFunc::Get1616Key(stQualityUpgradeProp.wHeroKindID, stQualityUpgradeProp.wQualityLevel)] = stQualityUpgradeProp;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

BOOL CGodweaponPropMgr::LoadLevelUpgradeFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("godweaponlevelupgradeprop", " order by HeroKindID, QualityLevel, UpgradeLevel").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SGodweaponLevelUpgradeProp stGodweaponLevelUpgradeProp;
        memset(&stGodweaponLevelUpgradeProp, 0, sizeof(stGodweaponLevelUpgradeProp));

        stGodweaponLevelUpgradeProp.wHeroKindID =  SGDP::SDAtou(pRes->GetFieldValueByName("HeroKindID"));
        stGodweaponLevelUpgradeProp.wQualityLevel = SGDP::SDAtou(pRes->GetFieldValueByName("QualityLevel"));
        _SDTStrncpy(stGodweaponLevelUpgradeProp.szQualityName, _SDTUTF82T(pRes->GetFieldValueByName("QualityName")), QUALITYNAME_LEN_MAX);
        stGodweaponLevelUpgradeProp.wUpgradeLevel = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeLevel"));
        stGodweaponLevelUpgradeProp.dwUpgradeScience = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeScience"));
        stGodweaponLevelUpgradeProp.byUpgradeAttrKind = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeAttrKind"));
        stGodweaponLevelUpgradeProp.dwUpgradeAttrValue = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeAttrValue"));
        m_mapGodweaponLevelUpgradeProp[CLogicFunc::Get161616Key(stGodweaponLevelUpgradeProp.wHeroKindID, stGodweaponLevelUpgradeProp.wQualityLevel, stGodweaponLevelUpgradeProp.wUpgradeLevel)] = stGodweaponLevelUpgradeProp;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

SGodweaponQualityUpgradeProp* CGodweaponPropMgr::GetGodweaponQualiryUpgradeProp(UINT16 wHeroKindID, UINT16 wQuality)
{
    CGodweaponQualityUpgradePropMapItr itr = m_mapGodweaponQualityUpgradeProp.find(CLogicFunc::Get1616Key(wHeroKindID, wQuality));
    if(itr != m_mapGodweaponQualityUpgradeProp.end())
    {
        return &itr->second;
    }

    return NULL;
}

SGodweaponLevelUpgradeProp* CGodweaponPropMgr::GetGodweaponLevelUpgradeProp(UINT16 wHeroKindID, UINT16 wQuality, UINT16 wLevel)
{
    CGodweaponLevelUpgradePropMapItr itr = m_mapGodweaponLevelUpgradeProp.find(CLogicFunc::Get161616Key(wHeroKindID, wQuality, wLevel));
    if(itr != m_mapGodweaponLevelUpgradeProp.end())
    {
        return &itr->second;
    }

    return NULL;
}

//初始化升级总共所需魂器(科技)点
BOOL CGodweaponPropMgr::InitUpgradeTotalScience()
{
    UINT16 wHeroKindID = 0;
    CHeroPropPriMap& mapHero = CHeroPropMgr::Instance()->GetHeroPropMap();
    for(CHeroPropPriMapItr itr = mapHero.begin(); itr != mapHero.end(); itr++)
    {
        wHeroKindID = itr->second.wKindID;
        UINT64	qwScience = 0;
        UINT32	dwMaxQuality = GetMaxQuality(wHeroKindID);
        for(UINT16 wQuality = 1; wQuality <= dwMaxQuality; wQuality++)
        {
            //升级每个品质的0级所需的器魂点为上一个品质的最大等级
            m_mapUpgradeTotalScience[CLogicFunc::Get161616Key(wHeroKindID, wQuality, 0)] = qwScience;

            for(UINT16 wLevel = 1; wLevel <= m_byQualityUpdateNum; wLevel++)
            {
                SGodweaponLevelUpgradeProp* pstProp = GetGodweaponLevelUpgradeProp(wHeroKindID, wQuality, wLevel);
                if(NULL == pstProp)
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: SGodweaponLevelUpgradeProp is NULL! HeroID[%d],Quality[%d],Level[%d]."), MSG_MARK, wHeroKindID, wQuality, wLevel);
                    return FALSE;
                }

                qwScience += pstProp->dwUpgradeScience;
                m_mapUpgradeTotalScience[CLogicFunc::Get161616Key(wHeroKindID, wQuality, wLevel)] = qwScience;
            }
        }
    }
    return TRUE;
}

//获取升级所需魂器(科技)点
UINT64 CGodweaponPropMgr::GetUpgradeScience(UINT16 wHeroKindID, UINT16 wCurQuality, UINT16 wCurLevel)
{
    SGodweaponLevelUpgradeProp* pstProp = GetGodweaponLevelUpgradeProp(wHeroKindID, wCurQuality, wCurLevel);
    if(pstProp)
    {
        return pstProp->dwUpgradeScience;
    }

    return 0xFFFFFFFFFFFFFFFF;
}


UINT64 CGodweaponPropMgr::GetNextUpgradeScience(UINT16 wHeroKindID, UINT16 wCurQuality, UINT16 wCurLevel)////获取升级所需魂器(科技)点
{
    if (m_byQualityUpdateNum < wCurLevel + 1)
    {
        wCurLevel = 1;
        wCurQuality++;
    }
    else
    {
        wCurLevel++;
    }

    SGodweaponLevelUpgradeProp* pstProp = GetGodweaponLevelUpgradeProp(wHeroKindID, wCurQuality, wCurLevel);
    if(pstProp)
    {
        return pstProp->dwUpgradeScience;
    }
    return 0xFFFFFFFFFFFFFFFF;
}



//获取升级到该品质等级总共所需魂器(科技)点
UINT64 CGodweaponPropMgr::GetUpgradeTotalScience(UINT16 wHeroKindID, UINT16 wCurQuality, UINT16 wCurLevel)
{
    CQualityUpgradeNum2ScienceItr itr = m_mapUpgradeTotalScience.find(CLogicFunc::Get161616Key(wHeroKindID, wCurQuality, wCurLevel));
    if(itr != m_mapUpgradeTotalScience.end())
    {
        return itr->second;
    }
    return 0;
}


VOID CGodweaponPropMgr::GetGodweaponAttr(UINT32 dwCliVer, UINT16 wHeroKindID, INT16 wQuality, UINT16 wLevel,	DT_GODWEAPON_ATTR_DATA& stIncAttr,  UINT8 &byEncAttrNum, DT_ATTR_INC_DATA astQualityUpgradeEncAttr[MAX_GODWEAPON_QUALITY_NUM],
        UINT16& wQualityRequireLevel, UINT32& dwUpgrageScience, UINT16& wUpgradeQualityNeedItemNum, DT_ATTR_INC_DATA& stNextLevelIncAttr)
{
    dwUpgrageScience = 0;
    wUpgradeQualityNeedItemNum = 0;
    memset(&stIncAttr, 0, sizeof(DT_GODWEAPON_ATTR_DATA));
    memset(astQualityUpgradeEncAttr, 0, sizeof(DT_ATTR_INC_DATA) * MAX_GODWEAPON_QUALITY_NUM);
    memset(&stNextLevelIncAttr, 0, sizeof(DT_ATTR_INC_DATA));

    SGodweaponQualityUpgradeProp* pstQualityProp = GetGodweaponQualiryUpgradeProp(wHeroKindID, wQuality);
    if(NULL == pstQualityProp)
    {
        return;
    }

    if(0 == wLevel)
    {
        memcpy(&stIncAttr, &pstQualityProp->stIncAttr, sizeof(DT_GODWEAPON_ATTR_DATA));
    }
    else
    {
        if((wQuality == GetMaxQuality(wHeroKindID)) && (wLevel == m_byQualityUpdateNum))
        {
            SAllAttrProp* pstFulIncAttr = GetFullIncAttr(wHeroKindID);
            if(pstFulIncAttr)
            {
                memcpy(&stIncAttr, pstFulIncAttr, sizeof(DT_GODWEAPON_ATTR_DATA));
            }
        }
        else
        {
            SGodweaponLevelUpgradeProp* pstProp = GetGodweaponLevelUpgradeProp(wHeroKindID, wQuality, wLevel);
            if(pstProp)
            {
                memcpy(&stIncAttr, &pstProp->stIncAttr, sizeof(DT_GODWEAPON_ATTR_DATA));
            }
        }
    }

    wUpgradeQualityNeedItemNum = pstQualityProp->wUpgradeQualityItemNum;

    SGodweaponQualityUpgradeProp* pstNextQualityProp = GetGodweaponQualiryUpgradeProp(wHeroKindID, wQuality + 1);
    if(NULL != pstNextQualityProp)
    {
        wQualityRequireLevel = pstNextQualityProp->wRequirePlayerLevel;
    }

    SGodweaponLevelUpgradeProp* pstNextLevelProp = GetGodweaponLevelUpgradeProp(wHeroKindID, wQuality, wLevel + 1);
    if(pstNextLevelProp)
    {
        dwUpgrageScience = pstNextLevelProp->dwUpgradeScience;
        stNextLevelIncAttr.byAttrKind = pstNextLevelProp->byUpgradeAttrKind;
        stNextLevelIncAttr.dwAttrValue = pstNextLevelProp->dwUpgradeAttrValue;

    }

    CHeroID2EncAttrPropMapItr itr = m_mapHeroID2EncAttrProp.find(wHeroKindID);
    if(itr != m_mapHeroID2EncAttrProp.end())
    {
        if (dwCliVer <= 295)
        {
            byEncAttrNum = itr->second.byEncAttrNum;
            memcpy(astQualityUpgradeEncAttr, &itr->second.astQualityUpgradeEncAttr, sizeof(DT_ATTR_INC_DATA) * MAX_GODWEAPON_QUALITY_NUM);
        }
        else
        {
			byEncAttrNum = 1;
			memcpy(&astQualityUpgradeEncAttr[0], &itr->second.astQualityUpgradeEncAttr[wQuality - 1], sizeof(DT_ATTR_INC_DATA));
        }
    }
}


// 初始化每品每级所提升的属性
BOOL CGodweaponPropMgr::InitQualityLevelHaveAttr()
{
    UINT16 wHeroKindID = 0;
    CHeroPropPriMap& mapHero = CHeroPropMgr::Instance()->GetHeroPropMap();
    for(CHeroPropPriMapItr itr = mapHero.begin(); itr != mapHero.end(); itr++)
    {
        wHeroKindID = itr->second.wKindID;
        SAllAttrProp stIncAttr;					//提升的属性(不包括奖励)
        SAllAttrProp stEncAttr;					//提升的奖励属性
        memset(&stIncAttr, 0, sizeof(stIncAttr));
        memset(&stEncAttr, 0, sizeof(stEncAttr));

        SHeroGodweaponAllEncAttrProp stHeroGodweaponAllAttrProp;
        memset(&stHeroGodweaponAllAttrProp, 0, sizeof(stHeroGodweaponAllAttrProp));

        UINT16	wMaxQuality =  GetMaxQuality(wHeroKindID);

        for(UINT16 wQuality = 1; wQuality <= wMaxQuality; wQuality++)
        {
            SGodweaponQualityUpgradeProp* pstGodweaponQualityProp = GetGodweaponQualiryUpgradeProp(wHeroKindID, wQuality);
            if(NULL == pstGodweaponQualityProp)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pstQualityProp is NULL! HeroID[%d],Quality[%d]."), MSG_MARK, wHeroKindID, wQuality);
                return FALSE;
            }
            //检测一下数据大小，是否放得下
            if (wQuality >= MAX_GODWEAPON_QUALITY_NUM_EX )
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pstQualityProp is NULL! HeroID[%d],Quality[%d]."), MSG_MARK, wHeroKindID, wQuality);
                return FALSE;
            }

            DT_ATTR_INC_DATA& stDT_ATTR_INC_DATA = stHeroGodweaponAllAttrProp.astQualityUpgradeEncAttr[wQuality - 1];
            stDT_ATTR_INC_DATA.byAttrKind	= pstGodweaponQualityProp->byQualityUpgradeEncAttrKind;
            stDT_ATTR_INC_DATA.dwAttrValue	= pstGodweaponQualityProp->dwUpgradeQualityEncAttrValue;

            SetGodweaponQualityAttr(wHeroKindID, wQuality, stIncAttr, stEncAttr);

            for(UINT16 wLevel = 1; wLevel <= m_byQualityUpdateNum; wLevel++)
            {
                SGodweaponLevelUpgradeProp* pstGodweaponLevelUpgradeProp = GetGodweaponLevelUpgradeProp(wHeroKindID, wQuality, wLevel);
                if(NULL == pstGodweaponLevelUpgradeProp)
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: pstQualityProp is NULL! HeroID[%d],Quality[%d],Level[%d]."), MSG_MARK, wHeroKindID, wQuality, wLevel);
                    return FALSE;
                }
                SetGodweaponLevelAttr(pstGodweaponLevelUpgradeProp, stIncAttr, stEncAttr);
            }
        }
        stHeroGodweaponAllAttrProp.byEncAttrNum = (UINT8)wMaxQuality;
        m_mapHeroID2EncAttrProp[wHeroKindID] = stHeroGodweaponAllAttrProp;
        SGodweaponLevelUpgradeProp* pstMaxLevelProp = GetGodweaponLevelUpgradeProp(wHeroKindID, wMaxQuality, m_byQualityUpdateNum);
        if(pstMaxLevelProp)
        {
            SAllAttrProp stFullIncAttr;
            memcpy(&stFullIncAttr, &pstMaxLevelProp->stIncAttr, sizeof(SAllAttrProp));
            m_mapHeroFullGodweaponIncAttr[wHeroKindID] = stFullIncAttr;
        }
    }
    return TRUE;
}

VOID CGodweaponPropMgr::SetGodweaponQualityAttr(UINT16 wHeroKindID, INT16 wQuality, SAllAttrProp& stIncAttr, SAllAttrProp& stEncAttr)
{
    //获取当前等级的相关属性
    SGodweaponQualityUpgradeProp* pstGodweaponQualityUpgradeProp = GetGodweaponQualiryUpgradeProp(wHeroKindID, wQuality);
    if(NULL == pstGodweaponQualityUpgradeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstGodweaponQualityUpgradeProp is NULL! HeroID[%d],Quality[%d]."), MSG_MARK, wHeroKindID, wQuality);
        return;
    }

    //SGodweaponQualityUpgradeProp* pstLastQualityProp = NULL;
    //pstLastQualityProp = GetGodweaponQualiryUpgradeProp(wHeroKindID, wQuality);

    for(UINT8 byIdx = 0 ; byIdx < MAX_UPGRADE_QUALITY_ATTR_NUM; byIdx++)
    {
        CLogicFunc::AddAttr(stIncAttr, pstGodweaponQualityUpgradeProp->abyBaseAttrKind[byIdx], pstGodweaponQualityUpgradeProp->adwBaseAttrValue[byIdx]);
    }

    //获取上上等级要增加的隐藏属性值。
    SGodweaponQualityUpgradeProp* pstUpGodweaponQualityUpgradeProp = GetGodweaponQualiryUpgradeProp(wHeroKindID, wQuality - 1);
    if(pstUpGodweaponQualityUpgradeProp)
    {
        CLogicFunc::AddAttr(stEncAttr, pstUpGodweaponQualityUpgradeProp->byQualityUpgradeEncAttrKind, pstUpGodweaponQualityUpgradeProp->dwUpgradeQualityEncAttrValue);
    }

    if(wQuality == GetMaxQuality(wHeroKindID))
    {
        SAllAttrProp stFullEncAttr;
        memset(&stFullEncAttr, 0, sizeof(stFullEncAttr));
        CLogicFunc::AddAttr(stFullEncAttr, pstGodweaponQualityUpgradeProp->byQualityUpgradeEncAttrKind, pstGodweaponQualityUpgradeProp->dwUpgradeQualityEncAttrValue);
        m_mapHeroFullGodweaponEncAttr[wHeroKindID] = stFullEncAttr;
    }

    memcpy(&pstGodweaponQualityUpgradeProp->stIncAttr, &stIncAttr, sizeof(SAllAttrProp));
    memcpy(&pstGodweaponQualityUpgradeProp->stEncAttr, &stEncAttr, sizeof(SAllAttrProp));
    AddGodweaponAttrProp(pstGodweaponQualityUpgradeProp->stAllAttr, pstGodweaponQualityUpgradeProp->stIncAttr, pstGodweaponQualityUpgradeProp->stEncAttr);
}



VOID CGodweaponPropMgr::SetGodweaponLevelAttr(SGodweaponLevelUpgradeProp* pstLevelProp, SAllAttrProp& stIncAttr, SAllAttrProp& stEncAttr)
{
    CLogicFunc::AddAttr(stIncAttr, pstLevelProp->byUpgradeAttrKind, pstLevelProp->dwUpgradeAttrValue);

    memcpy(&pstLevelProp->stIncAttr, &stIncAttr, sizeof(SAllAttrProp));
    memcpy(&pstLevelProp->stEncAttr, &stEncAttr, sizeof(SAllAttrProp));

    AddGodweaponAttrProp(pstLevelProp->stAllAttr, pstLevelProp->stIncAttr, pstLevelProp->stEncAttr);
}

VOID CGodweaponPropMgr::AddGodweaponAttrProp(SAllAttrProp& stAllAttr, SAllAttrProp& stAttr1, SAllAttrProp& stAttr2)
{
    DT_BATTLE_BASE_ATTRIBUTE& stBattleAttr = stAllAttr.stBattleAttr;
    DT_HIDE_BATTLE_ATTRIBUTE& stHideBattleAttr = stAllAttr.stHideBattleAttr;
    DT_EXT_BATTLE_ATTRIBUTE& stExtBattleAttr = stAllAttr.stExtBattleAttr;

    DT_BATTLE_BASE_ATTRIBUTE& stBattleAttr1 = stAttr1.stBattleAttr;
    DT_HIDE_BATTLE_ATTRIBUTE& stHideBattleAttr1 = stAttr1.stHideBattleAttr;
    DT_EXT_BATTLE_ATTRIBUTE& stExtBattleAttr1 = stAttr1.stExtBattleAttr;

    DT_BATTLE_BASE_ATTRIBUTE& stBattleAttr2 = stAttr2.stBattleAttr;
    DT_HIDE_BATTLE_ATTRIBUTE& stHideBattleAttr2 = stAttr2.stHideBattleAttr;
    DT_EXT_BATTLE_ATTRIBUTE& stExtBattleAttr2 = stAttr2.stExtBattleAttr;


    stBattleAttr.qwHP = stBattleAttr1.qwHP + stBattleAttr2.qwHP;
    stBattleAttr.dwAttack = stBattleAttr1.dwAttack + stBattleAttr2.dwAttack;
	stBattleAttr.dwDefend = stBattleAttr1.dwDefend + stBattleAttr2.dwDefend;

    stHideBattleAttr.wHitRate = stHideBattleAttr1.wHitRate + stHideBattleAttr2.wHitRate;
    stHideBattleAttr.wDodgeRate = stHideBattleAttr1.wDodgeRate + stHideBattleAttr2.wDodgeRate;
    stHideBattleAttr.wCritRate = stHideBattleAttr1.wCritRate + stHideBattleAttr2.wCritRate;
    stHideBattleAttr.wDeCritRate = stHideBattleAttr1.wDeCritRate + stHideBattleAttr2.wDeCritRate;
    stHideBattleAttr.wAngryValue = stHideBattleAttr1.wAngryValue + stHideBattleAttr2.wAngryValue;

    stExtBattleAttr.wDecHurtRate = stExtBattleAttr1.wDecHurtRate + stExtBattleAttr2.wDecHurtRate;
    stExtBattleAttr.wIncHurtRate = stExtBattleAttr1.wIncHurtRate + stExtBattleAttr2.wIncHurtRate;
    stExtBattleAttr.dwFirstAttack = stExtBattleAttr1.dwFirstAttack + stExtBattleAttr2.dwFirstAttack;
    stExtBattleAttr.wCoachTalent = stExtBattleAttr1.wCoachTalent + stExtBattleAttr2.wCoachTalent;
}


SAllAttrProp* CGodweaponPropMgr::GetFullIncAttr(UINT16 wHeroKindID)
{
    CHeroFullGodweaponAttrMapItr itr = m_mapHeroFullGodweaponIncAttr.find(wHeroKindID);
    if(itr != m_mapHeroFullGodweaponIncAttr.end())
    {
        return &itr->second;
    }

    return NULL;
}


SAllAttrProp* CGodweaponPropMgr::GetFullEncAttr(UINT16 wHeroKindID)
{
    CHeroFullGodweaponAttrMapItr itr = m_mapHeroFullGodweaponEncAttr.find(wHeroKindID);
    if(itr != m_mapHeroFullGodweaponEncAttr.end())
    {
        return &itr->second;
    }

    return NULL;
}

UINT16 CGodweaponPropMgr::GetMaxQuality(UINT16 wHeroKindID)
{
    UINT16	wMaxQualityLevel	=	0;
    CGodweaponQualityUpgradePropMapItr itr = m_mapGodweaponQualityUpgradeProp.begin();
    for (itr; itr != m_mapGodweaponQualityUpgradeProp.end(); itr++)
    {
        SGodweaponQualityUpgradeProp &stSGodweaponQualityUpgradeProp = itr->second;
        if (stSGodweaponQualityUpgradeProp.wHeroKindID != wHeroKindID)
        {
            continue;
        }
        if (wMaxQualityLevel < stSGodweaponQualityUpgradeProp.wQualityLevel)
        {
            wMaxQualityLevel = stSGodweaponQualityUpgradeProp.wQualityLevel;
        }
    }
    return wMaxQualityLevel;
}

SHeroGodweaponAllEncAttrProp *CGodweaponPropMgr::GetGodweaponAllEncAttr( UINT16 wHeroID )
{
    CHeroID2EncAttrPropMapItr itr = m_mapHeroID2EncAttrProp.find( wHeroID );
    if( itr == m_mapHeroID2EncAttrProp.end() )
    {
        return NULL;
    }

    return &itr->second;
}