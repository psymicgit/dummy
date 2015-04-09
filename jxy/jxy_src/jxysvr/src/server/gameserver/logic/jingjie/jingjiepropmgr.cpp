#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <common/server/utility.h>
#include <framework/gsapi.h>
#include "jingjiepropmgr.h"

#include <db/dbmgr.h>

using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(CJingJiePropMgr);

CJingJiePropMgr::CJingJiePropMgr()
{
    m_wMaxQualityLevel = 0;
    m_wSubLevelNum = 0;
}

CJingJiePropMgr::~CJingJiePropMgr()
{

}

BOOL CJingJiePropMgr::Init()
{
    if(!LoadFromDB())
    {
        return FALSE;
    }

    InitIncAttr();

    return TRUE;
}

VOID CJingJiePropMgr::UnInit()
{
    m_mapAttackCarrerJingJieSubLevelUpgradeProp.clear();
    m_mapDefenseCarrerJingJieSubLevelUpgradeProp.clear();
    m_mapAttackCarrerJingJiePerLevelData.clear();
    m_mapDefenseCarrerJingJiePerLevelData.clear();

}



BOOL CJingJiePropMgr::LoadFromDB()
{
    if(!LoadJingJieBaseropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadJingJieBaseropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadJingJieSubLevelUpgradePropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadJingJieSubLevelUpgradePropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadJingJieColorPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadJingJieColorPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }


    return TRUE;
}


BOOL CJingJiePropMgr::LoadJingJieSubLevelUpgradePropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jingjiesublevelupgradeprop", " order by CareerKind, Level, SubLevel asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    UINT8	byLastCareerKind = 0;
    UINT16	wLastLevel = 0;
    UINT16	wLastSubLevel = 0;
    while(pRes->GetRecord())
    {
        SJingJieSubLevelUpgradeProp stProp;
        stProp.byCareerKind = SGDP::SDAtou(pRes->GetFieldValueByName("CareerKind"));
        if((stProp.byCareerKind != ECK_ATTACK) && (stProp.byCareerKind != ECK_DEFENCE))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CareerKind[%d] invalid!"), MSG_MARK, stProp.byCareerKind);
            return FALSE;
        }
        if(stProp.byCareerKind != byLastCareerKind)
        {
            wLastLevel = 0;
            wLastSubLevel = 0;
        }
        stProp.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        if(stProp.wLevel > wLastLevel + 1)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CareerKind[%d] Level[%d] miss!"), MSG_MARK, stProp.byCareerKind, wLastLevel + 1);
            return FALSE;
        }
        if(stProp.wLevel != wLastLevel)
        {
            wLastSubLevel = 0;
        }
        stProp.wSubLevel = SGDP::SDAtou(pRes->GetFieldValueByName("SubLevel"));
        if(stProp.wSubLevel != wLastSubLevel + 1)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CareerKind[%d] Level[%d] SubLevel[%d] miss!"), MSG_MARK, stProp.byCareerKind, stProp.wLevel, wLastSubLevel + 1);
            return FALSE;
        }
        stProp.dwUpgradeGas = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeGas"));
        stProp.byIncAttrKind = SGDP::SDAtou(pRes->GetFieldValueByName("IncAttrKind"));
        stProp.dwIncAttrValue = SGDP::SDAtou(pRes->GetFieldValueByName("IncAttrValue"));

        if(ECK_ATTACK == stProp.byCareerKind)
        {
            m_mapAttackCarrerJingJieSubLevelUpgradeProp[CLogicFunc::Get1616Key(stProp.wLevel, stProp.wSubLevel)] = stProp;
        }
        else
        {
            m_mapDefenseCarrerJingJieSubLevelUpgradeProp[CLogicFunc::Get1616Key(stProp.wLevel, stProp.wSubLevel)] = stProp;
        }

        if(stProp.wLevel > m_wMaxQualityLevel)
        {
            m_wMaxQualityLevel = stProp.wLevel;
        }

        byLastCareerKind = stProp.byCareerKind;
        wLastLevel = stProp.wLevel;
        wLastSubLevel = stProp.wSubLevel;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    if((0 == m_mapAttackCarrerJingJieSubLevelUpgradeProp.size()) || (0 == m_mapDefenseCarrerJingJieSubLevelUpgradeProp.size()))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: jingjiesublevelupgradeprop has no data!"), MSG_MARK);
        return FALSE;
    }
    if(m_mapAttackCarrerJingJieSubLevelUpgradeProp.size() != m_mapDefenseCarrerJingJieSubLevelUpgradeProp.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: career jingjiesublevelupgradeprop num not match!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


BOOL CJingJiePropMgr::LoadJingJieBaseropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jingjiebaseprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    if(pRes->GetRecord())
    {
        m_wSubLevelNum = SGDP::SDAtou(pRes->GetFieldValueByName("SubLevelNum"));
        if(m_wSubLevelNum > MAX_JINGJIE_SUBLEVEL_NUM)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: SubLevelNum[%d] > Proto Define[%d]!"), MSG_MARK, m_wSubLevelNum, MAX_JINGJIE_SUBLEVEL_NUM);
            return FALSE;
        }
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: jingjiequalityupgradeprop has no data!"), MSG_MARK);
        return FALSE;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

BOOL CJingJiePropMgr::LoadJingJieColorPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jingjiecolor", " order by level").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRes->GetRecord())
    {
        UINT16 wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        UINT16 wColor = SGDP::SDAtou(pRes->GetFieldValueByName("Color"));
        m_mapJingJieColor[wLevel] = wColor;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}


VOID CJingJiePropMgr::InitIncAttr()
{
    SAllAttrProp stLastAttrProp;

    memset(&stLastAttrProp, 0, sizeof(stLastAttrProp));
    for(CJingJieSubLevelUpgradePropMapItr itr = m_mapAttackCarrerJingJieSubLevelUpgradeProp.begin(); itr != m_mapAttackCarrerJingJieSubLevelUpgradeProp.end(); itr++)
    {
        SJingJieSubLevelUpgradeProp& stProp = itr->second;
        CLogicFunc::AddAttr(stLastAttrProp, stProp.byIncAttrKind, stProp.dwIncAttrValue);
        stProp.stAllIncAttr = stLastAttrProp;
        CLogicFunc::ParseAttr(stProp.stAllIncAttr, MAX_JINGJIE_ATTR_INC_NUM, stProp.byIncAttrNum, stProp.astIncAttrInfo);

        if((stProp.wLevel == m_wMaxQualityLevel) && (stProp.wSubLevel == m_wSubLevelNum))
        {
            m_stAttackCarrerFullAttr.stAllIncAttr = stLastAttrProp;
            CLogicFunc::ParseAttr(m_stAttackCarrerFullAttr.stAllIncAttr, MAX_JINGJIE_ATTR_INC_NUM, m_stAttackCarrerFullAttr.byIncAttrNum, m_stAttackCarrerFullAttr.astIncAttrInfo);
        }
    }

    memset(&stLastAttrProp, 0, sizeof(stLastAttrProp));
    for(CJingJieSubLevelUpgradePropMapItr itr = m_mapDefenseCarrerJingJieSubLevelUpgradeProp.begin(); itr != m_mapDefenseCarrerJingJieSubLevelUpgradeProp.end(); itr++)
    {
        SJingJieSubLevelUpgradeProp& stProp = itr->second;
        CLogicFunc::AddAttr(stLastAttrProp, stProp.byIncAttrKind, stProp.dwIncAttrValue);
        stProp.stAllIncAttr = stLastAttrProp;
        CLogicFunc::ParseAttr(stProp.stAllIncAttr, MAX_JINGJIE_ATTR_INC_NUM, stProp.byIncAttrNum, stProp.astIncAttrInfo);

        if((stProp.wLevel == m_wMaxQualityLevel) && (stProp.wSubLevel == m_wSubLevelNum))
        {
            m_stDefenseCarrerFullAttr.stAllIncAttr = stLastAttrProp;
            CLogicFunc::ParseAttr(m_stDefenseCarrerFullAttr.stAllIncAttr, MAX_JINGJIE_ATTR_INC_NUM, m_stDefenseCarrerFullAttr.byIncAttrNum, m_stDefenseCarrerFullAttr.astIncAttrInfo);
        }
    }

    for(UINT16 wLevel = 1; wLevel <= m_wMaxQualityLevel; wLevel++)
    {
        SAllAttrProp stAttrProp;
        memset(&stAttrProp, 0, sizeof(stAttrProp));
        DT_JINGJIE_PER_LEVEL_DATA stDT_JINGJIE_PER_LEVEL_DATA;
        memset(&stDT_JINGJIE_PER_LEVEL_DATA, 0, sizeof(DT_JINGJIE_PER_LEVEL_DATA));
        for(UINT16 wSubLevel = 1; wSubLevel <= m_wSubLevelNum; wSubLevel++)
        {
            CJingJieSubLevelUpgradePropMapItr itr = m_mapAttackCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
            if(itr != m_mapAttackCarrerJingJieSubLevelUpgradeProp.end())
            {
                SJingJieSubLevelUpgradeProp& stSubLevelProp = itr->second;
                CLogicFunc::AddAttr(stAttrProp, stSubLevelProp.byIncAttrKind, stSubLevelProp.dwIncAttrValue);
                stDT_JINGJIE_PER_LEVEL_DATA.adwSubLevelCostJingJieInfo[wSubLevel - 1] = stSubLevelProp.dwUpgradeGas;
                stDT_JINGJIE_PER_LEVEL_DATA.astSubLevelIncAttrInfo[wSubLevel - 1].byAttrKind = stSubLevelProp.byIncAttrKind;
                stDT_JINGJIE_PER_LEVEL_DATA.astSubLevelIncAttrInfo[wSubLevel - 1].dwAttrValue = stSubLevelProp.dwIncAttrValue;
            }
        }
        UINT8 byIncAttrNum = 0;
        //CLogicFunc::ParseAttr(stAttrProp, MAX_JINGJIE_ATTR_INC_NUM, byIncAttrNum, stDT_JINGJIE_PER_LEVEL_DATA.astSubLevelIncAttrInfo);
        m_mapAttackCarrerJingJiePerLevelData[wLevel] = stDT_JINGJIE_PER_LEVEL_DATA;
    }

    for(UINT16 wLevel = 1; wLevel <= m_wMaxQualityLevel; wLevel++)
    {
        SAllAttrProp stAttrProp;
        memset(&stAttrProp, 0, sizeof(stAttrProp));
        DT_JINGJIE_PER_LEVEL_DATA stDT_JINGJIE_PER_LEVEL_DATA;
        memset(&stDT_JINGJIE_PER_LEVEL_DATA, 0, sizeof(DT_JINGJIE_PER_LEVEL_DATA));
        for(UINT16 wSubLevel = 1; wSubLevel <= m_wSubLevelNum; wSubLevel++)
        {
            CJingJieSubLevelUpgradePropMapItr itr = m_mapDefenseCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
            if(itr != m_mapDefenseCarrerJingJieSubLevelUpgradeProp.end())
            {
                SJingJieSubLevelUpgradeProp& stSubLevelProp = itr->second;
                CLogicFunc::AddAttr(stAttrProp, stSubLevelProp.byIncAttrKind, stSubLevelProp.dwIncAttrValue);
                stDT_JINGJIE_PER_LEVEL_DATA.adwSubLevelCostJingJieInfo[wSubLevel - 1] = stSubLevelProp.dwUpgradeGas;
                stDT_JINGJIE_PER_LEVEL_DATA.astSubLevelIncAttrInfo[wSubLevel - 1].byAttrKind = stSubLevelProp.byIncAttrKind;
                stDT_JINGJIE_PER_LEVEL_DATA.astSubLevelIncAttrInfo[wSubLevel - 1].dwAttrValue = stSubLevelProp.dwIncAttrValue;
            }
        }
        UINT8 byIncAttrNum = 0;
        //CLogicFunc::ParseAttr(stAttrProp, MAX_JINGJIE_ATTR_INC_NUM, byIncAttrNum, stDT_JINGJIE_PER_LEVEL_DATA.astSubLevelIncAttrInfo);
        m_mapDefenseCarrerJingJiePerLevelData[wLevel] = stDT_JINGJIE_PER_LEVEL_DATA;
    }
}


UINT32 CJingJiePropMgr::GetJingJieSubLevelUpgradeGas(UINT8 byCareerID, UINT16 wLevel, UINT16 wSubLevel)
{
    switch(byCareerID)
    {
    case ECK_ATTACK:
    {
        CJingJieSubLevelUpgradePropMapItr itr = m_mapAttackCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
        if(itr != m_mapAttackCarrerJingJieSubLevelUpgradeProp.end())
        {
            return itr->second.dwUpgradeGas;
        }
    }
    break;
    case ECK_DEFENCE:
    {
        CJingJieSubLevelUpgradePropMapItr itr = m_mapDefenseCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
        if(itr != m_mapDefenseCarrerJingJieSubLevelUpgradeProp.end())
        {
            return itr->second.dwUpgradeGas;
        }
    }
    break;
    default:
        break;
    }


    return 0;
}

const SAllAttrProp* CJingJiePropMgr::GetAllIncAttr(UINT8 byCareerID, UINT16 wLevel, UINT16 wSubLevel)
{
    if((wSubLevel == 0) && (wLevel > 1)) //当前级的子级没有，则取上一级的最大子级属性
    {
        wLevel--;
        wSubLevel = m_wSubLevelNum;
    }

    switch(byCareerID)
    {
    case ECK_ATTACK:
    {
        if((wLevel == m_wMaxQualityLevel) && (wSubLevel == m_wSubLevelNum))
        {
            return &m_stAttackCarrerFullAttr.stAllIncAttr;
        }
        CJingJieSubLevelUpgradePropMapItr itr = m_mapAttackCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
        if(itr != m_mapAttackCarrerJingJieSubLevelUpgradeProp.end())
        {
            return &itr->second.stAllIncAttr;
        }
    }
    break;
    case ECK_DEFENCE:
    {
        if((wLevel == m_wMaxQualityLevel) && (wSubLevel == m_wSubLevelNum))
        {
            return &m_stDefenseCarrerFullAttr.stAllIncAttr;
        }
        CJingJieSubLevelUpgradePropMapItr itr = m_mapDefenseCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
        if(itr != m_mapDefenseCarrerJingJieSubLevelUpgradeProp.end())
        {
            return &itr->second.stAllIncAttr;
        }
    }
    break;
    default:
        return NULL;
        break;
    }

    return NULL;
}

VOID CJingJiePropMgr::GetAllIncAttr(UINT8 byCareerID, UINT16 wLevel, UINT16 wSubLevel, UINT8& byIncAttrNum, DT_ATTR_INC_DATA astIncAttrInfo[])
{
    byIncAttrNum = 0;

    if((wSubLevel == 0) && (wLevel > 1)) //当前级的子级没有，则取上一级的最大子级属性
    {
        wLevel--;
        wSubLevel = m_wSubLevelNum;
    }

    switch(byCareerID)
    {
    case ECK_ATTACK:
    {
        if((wLevel == m_wMaxQualityLevel) && (wSubLevel == m_wSubLevelNum))
        {
            byIncAttrNum = m_stAttackCarrerFullAttr.byIncAttrNum;
            memcpy(astIncAttrInfo, m_stAttackCarrerFullAttr.astIncAttrInfo, sizeof(m_stAttackCarrerFullAttr.astIncAttrInfo));
            return;
        }

        CJingJieSubLevelUpgradePropMapItr itr = m_mapAttackCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
        if(itr != m_mapAttackCarrerJingJieSubLevelUpgradeProp.end())
        {
            byIncAttrNum = itr->second.byIncAttrNum;
            memcpy(astIncAttrInfo, itr->second.astIncAttrInfo, sizeof(itr->second.astIncAttrInfo));
            return;
        }
    }
    break;
    case ECK_DEFENCE:
    {
        if((wLevel == m_wMaxQualityLevel) && (wSubLevel == m_wSubLevelNum))
        {
            byIncAttrNum = m_stDefenseCarrerFullAttr.byIncAttrNum;
            memcpy(astIncAttrInfo, m_stDefenseCarrerFullAttr.astIncAttrInfo, sizeof(m_stAttackCarrerFullAttr.astIncAttrInfo));
            return;
        }

        CJingJieSubLevelUpgradePropMapItr itr = m_mapDefenseCarrerJingJieSubLevelUpgradeProp.find(CLogicFunc::Get1616Key(wLevel, wSubLevel));
        if(itr != m_mapDefenseCarrerJingJieSubLevelUpgradeProp.end())
        {
            byIncAttrNum = itr->second.byIncAttrNum;
            memcpy(astIncAttrInfo, itr->second.astIncAttrInfo, sizeof(itr->second.astIncAttrInfo));
            return;
        }
    }
    break;
    default:
        break;
    }

}

VOID CJingJiePropMgr::GetJingJiePerLevelData(UINT8 byCareerID, UINT16 wLevel, DT_JINGJIE_PER_LEVEL_DATA& stDT_JINGJIE_PER_LEVEL_DATA)
{
    memset(&stDT_JINGJIE_PER_LEVEL_DATA, 0, sizeof(stDT_JINGJIE_PER_LEVEL_DATA));
    switch(byCareerID)
    {
    case ECK_ATTACK:
    {
        CJingJiePerLevelDataMapItr itr = m_mapAttackCarrerJingJiePerLevelData.find(wLevel);
        if(itr != m_mapAttackCarrerJingJiePerLevelData.end())
        {
            memcpy(&stDT_JINGJIE_PER_LEVEL_DATA, &itr->second, sizeof(stDT_JINGJIE_PER_LEVEL_DATA));
            return;
        }
    }
    break;
    case ECK_DEFENCE:
    {
        CJingJiePerLevelDataMapItr itr = m_mapDefenseCarrerJingJiePerLevelData.find(wLevel);
        if(itr != m_mapDefenseCarrerJingJiePerLevelData.end())
        {
            memcpy(&stDT_JINGJIE_PER_LEVEL_DATA, &itr->second, sizeof(stDT_JINGJIE_PER_LEVEL_DATA));
            return;
        }
    }
    break;
    default:
        return;
        break;
    }
}

UINT16 CJingJiePropMgr::GetJingJieColor(UINT16 wLevel)
{
    CJingJieColorMapItr itr = m_mapJingJieColor.find(wLevel);
    if (itr != m_mapJingJieColor.end())
    {
        return itr->second;
    }

    return 1;
}

