#include "vippropmgr.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <sqlite3db.h>
#include <logic/base/dbconfig.h>
#include <logic/moneytree/moneytreepropbasemgr.h>
#include <logic/race/racepropmgr.h>
#include <logic/plunder/plunderbasepropmgr.h>
#include <logic/other/phystrengthmgr.h>
#include <framework/gsapi.h>
#include <logic/other/singleprammgr.h>
#include <logic/lua/luamgr.h>
#include "logic/player/player.h"
#include "logic/other/errmsgmgr.h"
#include "framework/gsconfig.h"
#include <db/dbmgr.h>

IMPLEMENT_SINGLETON_PROPMGR(CVipPropMgr)

#define SQL_READ_GOLD_EXPERIENCE_DRUG_STRING "select * from goldexperienceprop"
CVipPropMgr::CVipPropMgr()
{
    m_mapVipLevelProp.clear();
    m_mapGold2VipLevelProp.clear();
    m_dwMaxVipLevelRechargeGold = 0;
    m_byMaxVipLevel = 0;
    m_mapFuncKind2OpenFuncProp.clear();
    m_mapVipLevel2OpenFunc.clear();
    m_mapVipIncNumProp.clear();
    m_mapVipLevel2IncNum.clear();
    memset(m_astVipLevelInfo, 0, sizeof(m_astVipLevelInfo));
    m_mapGoldCostProp.clear();
}

CVipPropMgr::~CVipPropMgr()
{

}


BOOL CVipPropMgr::Init()
{
    if(!LoadFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    InitVipTabInfo();

    return TRUE;
}

VOID CVipPropMgr::UnInit()
{
    m_mapVipLevelProp.clear();
    m_mapGold2VipLevelProp.clear();
    m_dwMaxVipLevelRechargeGold = 0;
    m_byMaxVipLevel = 0;
    m_mapFuncKind2OpenFuncProp.clear();
    m_mapVipLevel2OpenFunc.clear();
    m_mapVipIncNumProp.clear();
    m_mapVipLevel2IncNum.clear();
    memset(m_astVipLevelInfo, 0, sizeof(m_astVipLevelInfo));
    m_mapGoldCostProp.clear();
    m_mapAuthBuyGoldProp.clear();
}


BOOL CVipPropMgr::CkOpenFunc(UINT8 byFuncKind, UINT8 byVipLevel)
{
    CVipOpenFuncPropMapItr itr = m_mapFuncKind2OpenFuncProp.find(byFuncKind);
    if((itr != m_mapFuncKind2OpenFuncProp.end()) && (byVipLevel >= itr->second.byNeedVipLevel))
    {
        return TRUE;
    }

    return FALSE;
}

UINT8 CVipPropMgr::GetOpenLevel( UINT8 byFuncKind )
{
    return m_mapFuncKind2OpenFuncProp[byFuncKind].byNeedVipLevel;
}

UINT16 CVipPropMgr::GetIncNum(UINT8 byFuncKind)
{
    CVipIncNumPropMapItr itr;
    itr = m_mapVipIncNumProp.find(CLogicFunc::Get88Key(byFuncKind, 0));
    if(itr != m_mapVipIncNumProp.end())
    {
        return itr->second.wIncNum;
    }
    return 0;
}

UINT16 CVipPropMgr::GetIncNum(UINT8 byFuncKind, UINT8 byVipLevel)
{
    CVipIncNumPropMapItr itr;
    itr = m_mapVipIncNumProp.find(CLogicFunc::Get88Key(byFuncKind, byVipLevel));
    if(itr != m_mapVipIncNumProp.end())
    {
        return itr->second.wIncNum;
    }
    return 0;
}

BOOL CVipPropMgr::CkCostVIP(UINT8 byFuncKind)
{
    if (byFuncKind < EGCF_NOTVIP)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


UINT16 CVipPropMgr::GetLowVipIncLevel( UINT8 byFuncKind )
{
    for( UINT8 byVipLevel = 0; byVipLevel < MAX_VIP_LEVEL; ++byVipLevel )
    {
        CVipIncNumPropMapItr itr = m_mapVipIncNumProp.find(CLogicFunc::Get88Key(byFuncKind, byVipLevel));
        if(itr != m_mapVipIncNumProp.end())
        {
            return itr->second.byVipLevel;
        }
    }
    return 0;
}

UINT8 CVipPropMgr::GetVipLevel(UINT32 wRechargeGold)
{
    if(0 == wRechargeGold)
    {
        return 0;
    }

    if(wRechargeGold >= m_dwMaxVipLevelRechargeGold)
    {
        return m_byMaxVipLevel;
    }

    CGold2VipLevelPropMapItr itr = m_mapGold2VipLevelProp.lower_bound(wRechargeGold);
    if(itr != m_mapGold2VipLevelProp.end())
    {
        if(itr->first == wRechargeGold)
        {
            return itr->second.byVipLevel;
        }
        else // qwRechargeGold > itr->first
        {
            if(itr->second.byVipLevel > 1)
            {
                return itr->second.byVipLevel - 1;
            }
        }
    }

    return 0;
}

BOOL CVipPropMgr::LoadFromDB()
{
    //if(!LoadVipLevelFromDB())
    //{
    //    SYS_CRITICAL(_SDT("[%s: %d]: LoadVipLevelFromDB failed!"), MSG_MARK);
    //    return FALSE;
    //}

    if(!LoadOpenFuncFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadOpenFuncFromDB failed!"), MSG_MARK);
        return FALSE;
    }
    if(!LoadIncNumFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadIncNumFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if( !LoadFuncCostGoldFromDB() )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadFuncCostGoldFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if( !LoadBuyGoldFromDB() )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadBuyGoldFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if( !LoadVipBaseFromDB() )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadVipBaseFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    //InitVipTabInfo();

    return TRUE;
}

BOOL CVipPropMgr::LoadVipBaseFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("vipbaseprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET == nRet && pRes->GetRecord())
    {
        UINT8	byAuthType = SGDP::SDAtou(pRes->GetFieldValueByName("AuthType"));
        SDStrncpy(m_mapAuthBuyGoldProp[byAuthType].m_aszProContextHead, pRes->GetFieldValueByName("ProContextHead"), MAX_COMMON_TXT_NUM - 1);
        SDStrncpy(m_mapAuthBuyGoldProp[byAuthType].m_aszProContext, pRes->GetFieldValueByName("ProContext"), MAX_COMMON_TXT_NUM - 1);
        SDStrncpy(m_mapAuthBuyGoldProp[byAuthType].m_aszBuyUrl, pRes->GetFieldValueByName("BuyUrl"), MAX_BUY_URL_LEN - 1);
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}



//BOOL CVipPropMgr::LoadVipLevelFromDB()
//{
//    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
//    if(NULL == pDBSession)
//    {
//        return FALSE;
//    }
//    SGDP::ISDDBRecordSet* pRes = NULL;
//    UINT32 dwErrID = 0;
//    string strErr;
//    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("viplevelprop", " order by VipLevel asc").c_str(), &pRes, &dwErrID,  &strErr);
//    if(SDDB_HAS_RECORDSET != nRet)
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
//        return FALSE;
//    }
//    m_byMaxVipLevel = 0;
//    m_dwMaxVipLevelRechargeGold = 0;
//    UINT8 byLastLevel = 0;
//    while(pRes->GetRecord())
//    {
//        SVipLevelProp stProp;
//        stProp.byVipLevel = SGDP::SDAtou(pRes->GetFieldValueByName("VipLevel"));
//        if(stProp.byVipLevel != byLastLevel + 1)
//        {
//            SYS_CRITICAL(_SDT("[%s: %d]: VipLevel[%d] miss!"), MSG_MARK, byLastLevel + 1);
//            return FALSE;
//        }
//        stProp.dwNeedRechargeGold = SGDP::SDAtou(pRes->GetFieldValueByName("NeedRechargeGold"));
//
//        if(stProp.byVipLevel > MAX_VIP_LEVEL)
//        {
//            SYS_CRITICAL(_SDT("[%s: %d]: VipLevel[%u] out rang[%u] miss!"), MSG_MARK, stProp.byVipLevel, MAX_VIP_LEVEL);
//            return FALSE;
//        }
//
//        if(stProp.byVipLevel > m_byMaxVipLevel)
//        {
//            m_byMaxVipLevel = stProp.byVipLevel;
//        }
//
//        if(stProp.dwNeedRechargeGold > m_dwMaxVipLevelRechargeGold)
//        {
//            m_dwMaxVipLevelRechargeGold = stProp.dwNeedRechargeGold;
//        }
//
//        byLastLevel = stProp.byVipLevel;
//
//        m_mapGold2VipLevelProp[stProp.dwNeedRechargeGold] = stProp;
//        m_mapVipLevelProp[stProp.byVipLevel] = stProp;
//    }
//
//    if(NULL != pRes)
//    {
//        pRes->Release();
//        pRes = NULL;
//    }
//
//    if(0 == m_mapVipLevelProp.size())
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: viplevelprop has no data!"), MSG_MARK);
//        return FALSE;
//    }
//
//    return TRUE;
//}

BOOL CVipPropMgr::LoadOpenFuncFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("vipopenfuncprop", " order by NeedVipLevel asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while(pRes->GetRecord())
    {
        SVipOpenFuncProp stProp;
        stProp.byOpenFuncKind = SGDP::SDAtou(pRes->GetFieldValueByName("OpenFuncKind"));
        stProp.byNeedVipLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedVipLevel"));

        m_mapFuncKind2OpenFuncProp[stProp.byOpenFuncKind] = stProp;
        m_mapVipLevel2OpenFunc[stProp.byNeedVipLevel].push_back(stProp.byOpenFuncKind);
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

BOOL CVipPropMgr::LoadIncNumFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("vipincnumprop", " order by FuncKind, VipLevel asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    SVipIncNumProp stLastProp;
    memset(&stLastProp, 0, sizeof(stLastProp));
    while(pRes->GetRecord())
    {
        SVipIncNumProp stProp;
        stProp.byFuncKind = SGDP::SDAtou(pRes->GetFieldValueByName("FuncKind"));
        stProp.byVipLevel = SGDP::SDAtou(pRes->GetFieldValueByName("VipLevel"));
        stProp.wIncNum = SGDP::SDAtou(pRes->GetFieldValueByName("IncNum"));
        if((stProp.byFuncKind != stLastProp.byFuncKind) && (0 != stLastProp.byFuncKind)) //补满所有vip等级，方便查找
        {
            UINT8		byFuncKind = stLastProp.byFuncKind;
            UINT16		wIncNum = 0; //
            for(UINT8 byVipLevel = 1; byVipLevel <= m_byMaxVipLevel; byVipLevel++)
            {
                CVipIncNumPropMapItr itr = m_mapVipIncNumProp.find(CLogicFunc::Get88Key(byFuncKind, byVipLevel));
                if(itr != m_mapVipIncNumProp.end())
                {
                    wIncNum = itr->second.wIncNum;
                }
                else
                {
                    SVipIncNumProp	stTmp;
                    stTmp.byFuncKind	= byFuncKind;
                    stTmp.byVipLevel	= byVipLevel;
                    stTmp.wIncNum		= wIncNum;
                    m_mapVipIncNumProp[CLogicFunc::Get88Key(stTmp.byFuncKind, stTmp.byVipLevel)] = stTmp;
                }
            }
        }
        stLastProp = stProp;

        m_mapVipIncNumProp[CLogicFunc::Get88Key(stProp.byFuncKind, stProp.byVipLevel)] = stProp;

        m_mapVipLevel2IncNum[stProp.byVipLevel].push_back(stProp);
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }


    return TRUE;
}

BOOL CVipPropMgr::LoadBuyGoldFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("buygoldinfo", " order by AuthType, gold asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRes->GetRecord())
    {
        SBuyGoldProp stProp;
        UINT8		byAuthType = 0;
        byAuthType = SGDP::SDAtou(pRes->GetFieldValueByName("AuthType"));
        stProp.dwGold	= SGDP::SDAtou(pRes->GetFieldValueByName("Gold"));
        stProp.wRMB = SGDP::SDAtou(pRes->GetFieldValueByName("RMB"));
        stProp.dwProductID = SGDP::SDAtou( pRes->GetFieldValueByName("ProductID"));
        SDStrncpy(stProp.aszProductID, pRes->GetFieldValueByName("ProductID"), MAX_PRODUCTID_LEN - 1);
        SDStrncpy(stProp.aszBuyGoldInfo, pRes->GetFieldValueByName("BuyGoldInfo"), MAX_COMMON_TXT_NUM - 1);
        m_mapAuthBuyGoldProp[byAuthType].m_vecBuyGoldProp.push_back(stProp);
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}



VOID CVipPropMgr::OpenVipTab(UINT8 byAuthType, UINT8& byVipLevelNum, DT_VIP_LEVEL_DATA astVipLevelInfo[MAX_VIP_LEVEL])
{
    byVipLevelNum = m_byMaxVipLevel;
    memcpy(astVipLevelInfo, m_astVipLevelInfo, sizeof(m_astVipLevelInfo));
}


VOID CVipPropMgr::OpenVipTab(CPlayer *poPlayer, UINT8 byAuthType, UINT8& byVipLevelNum, DT_VIP_LEVEL_DATA astVipLevelInfo[MAX_VIP_LEVEL],
                             DT_BUY_GOLD_TXT astBuyGoldTxtLst[MAX_BUY_GOLD_NUM], TCHAR aszProContextHead[MAX_COMMON_TXT_NUM],
                             TCHAR aszProContext[MAX_VIP_PRO_CONTEXT_LEN], TCHAR aszBuyUrl[MAX_BUY_URL_LEN])
{
    byVipLevelNum = m_byMaxVipLevel;
    memcpy(astVipLevelInfo, m_astVipLevelInfo, sizeof(m_astVipLevelInfo));

	SAuthBuyGoldProp* poProp = GetBuyGoldInfoByAuthType(byAuthType);

    CAuthBuyGoldPropMapItr itr = m_mapAuthBuyGoldProp.find(byAuthType);
    if (NULL == poProp)
    {
        return;
    }

    memcpy(aszProContextHead, poProp->m_aszProContextHead, MAX_COMMON_TXT_NUM);
    memcpy(aszProContext, poProp->m_aszProContext, MAX_VIP_PRO_CONTEXT_LEN);
    memcpy(aszBuyUrl, poProp->m_aszBuyUrl, MAX_BUY_URL_LEN);
    for( UINT32 dwIndex = 0; dwIndex < poProp->m_vecBuyGoldProp.size() && dwIndex < MAX_BUY_GOLD_NUM; ++dwIndex )
    {
        if (poPlayer->GetDaiChong())
        {
            string strDaiChong	=  CMsgDefMgr::Instance()->GetErrMsg("DAICHONG");
            memcpy(&astBuyGoldTxtLst[dwIndex], strDaiChong.c_str(), strDaiChong.size());
        }
        else
        {
            memcpy(&astBuyGoldTxtLst[dwIndex], poProp->m_vecBuyGoldProp[dwIndex].aszBuyGoldInfo, MAX_COMMON_TXT_NUM);
        }
    }

}


VOID CVipPropMgr::InitVipTabInfo()
{
    //初始功能次数
    for(UINT8 byVipLevel = 1; byVipLevel <= m_byMaxVipLevel; byVipLevel++)
    {
        DT_VIP_LEVEL_DATA& stDT_VIP_LEVEL_DATA = m_astVipLevelInfo[byVipLevel - 1];
        stDT_VIP_LEVEL_DATA.byVipLevel = byVipLevel;
        stDT_VIP_LEVEL_DATA.byExtNumFuncNum = 0;
        CVipLevel2IncNumMapItr itr = m_mapVipLevel2IncNum.find(byVipLevel);
        if(itr != m_mapVipLevel2IncNum.end())
        {
            CVipIncNumVec& vecVipIncNum = itr->second;
            UINT8 bySize = vecVipIncNum.size();

            for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
            {
                SVipIncNumProp& stProp = vecVipIncNum[byIdx];
                DT_VIP_NUM_FUNC_DATA& stDT_VIP_NUM_FUNC_DATA = stDT_VIP_LEVEL_DATA.astVipNumFuncInfo[stDT_VIP_LEVEL_DATA.byExtNumFuncNum];
                stDT_VIP_NUM_FUNC_DATA.byNumFuncKind = stProp.byFuncKind;

				stDT_VIP_NUM_FUNC_DATA.dwCommonNum	=	CVipPropMgr::Instance()->GetIncNum(stDT_VIP_NUM_FUNC_DATA.byNumFuncKind);
				stDT_VIP_NUM_FUNC_DATA.dwIncNum		=	stProp.wIncNum - CVipPropMgr::Instance()->GetIncNum(stDT_VIP_NUM_FUNC_DATA.byNumFuncKind);

                //次数为零，直接下一个。
                if(0 != stDT_VIP_NUM_FUNC_DATA.dwIncNum )
                {
                    stDT_VIP_LEVEL_DATA.byExtNumFuncNum++;
                }
            }
        }
    }

    for(UINT8 byVipLevel = 1; byVipLevel <= m_byMaxVipLevel; byVipLevel++)
    {
        DT_VIP_LEVEL_DATA& stDT_VIP_LEVEL_DATA = m_astVipLevelInfo[byVipLevel - 1];
        stDT_VIP_LEVEL_DATA.byVipLevel = byVipLevel;
        stDT_VIP_LEVEL_DATA.dwNeedRechargeGold = m_mapVipLevelProp[byVipLevel].dwNeedRechargeGold;
        CVipLevel2OpenFuncMapItr itr = m_mapVipLevel2OpenFunc.find(byVipLevel);

        if(itr != m_mapVipLevel2OpenFunc.end())
        {
            CVipFuncVec& vecOpenFunc = itr->second;
            stDT_VIP_LEVEL_DATA.byOpenVipFuncNum = vecOpenFunc.size();

            UINT8 bySize = stDT_VIP_LEVEL_DATA.byOpenVipFuncNum;
            for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
            {
                stDT_VIP_LEVEL_DATA.abyOpenVipFuncInfo[byIdx] = vecOpenFunc[byIdx];
            }
        }
        else
        {
            stDT_VIP_LEVEL_DATA.byOpenVipFuncNum  = 0;
        }

    }
}

BOOL CVipPropMgr::LoadFuncCostGoldFromDB()
{
    return TRUE; //废弃

    SGDP::ISDDBSession*	pGoldDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pGoldDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pGoldRes = NULL;
    UINT32 dwGoldErrID = 0;
    string strGoldErr;
    INT32 nGoldRet = pGoldDBSession->ExecuteSqlRs(GetPropTableSelectSql("vipextcostprop").c_str(), &pGoldRes, &dwGoldErrID,  &strGoldErr);
    if(SDDB_HAS_RECORDSET != nGoldRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    //查询的表结果
    while(pGoldRes->GetRecord())// 当有行结果时
    {
        SGoldCostProp stGoldExperienceDrugProp;
        memset(&stGoldExperienceDrugProp, 0, sizeof(stGoldExperienceDrugProp));
        stGoldExperienceDrugProp.nFuncID = SGDP::SDAtou(pGoldRes->GetFieldValueByName("FuncID"));
        stGoldExperienceDrugProp.wIndex = SGDP::SDAtou(pGoldRes->GetFieldValueByName("Index"));
        stGoldExperienceDrugProp.wCost = SGDP::SDAtou(pGoldRes->GetFieldValueByName("Cost"));
        m_mapGoldCostProp[stGoldExperienceDrugProp.nFuncID].push_back( stGoldExperienceDrugProp );
    }
    if(NULL != pGoldRes)
    {
        pGoldRes->Release();
        pGoldRes = NULL;
    }
    return TRUE;
}

// BOOL CVipPropMgr::GetGoldCostByIndex( INT32 nFuncID, UINT16 wIndex, UINT16 &wCostGold )
// {
// 	wCostGold=0;
// 	CGoldCostPropMapItr itr=m_mapGoldCostProp.find( nFuncID );
// 	if( itr==m_mapGoldCostProp.end() )
// 	{
// 		return FALSE;
// 	}
//
// 	for( UINT32 dwIndex=0; dwIndex<itr->second.size(); ++dwIndex )
// 	{
// 		if( itr->second[dwIndex].wIndex==wIndex )
// 		{
// 			//查到
// 			wCostGold=itr->second[dwIndex].wCost;
// 			return TRUE;
// 		}
// 	}
// 	return FALSE;
// }


SAuthBuyGoldProp* CVipPropMgr::GetBuyGoldInfoByAuthType(UINT8 byAuthType)
{
	CAuthBuyGoldPropMapItr itr = m_mapAuthBuyGoldProp.find(byAuthType);
	if (itr != m_mapAuthBuyGoldProp.end())
	{
		return &itr->second;
	}
	else
	{
		if ( IOS_AND_BOUND < byAuthType)
		{
			itr = m_mapAuthBuyGoldProp.find(AND_DEFAULT_NOTICE);
		}
		else
		{
			itr = m_mapAuthBuyGoldProp.find(IOS_DEFAULT_NOTICE);
		}
		if ( itr != m_mapAuthBuyGoldProp.end())
		{
			return &itr->second;
		}
		return NULL;
	}
}

BOOL CVipPropMgr::GetBuyPropByProductID(UINT8 byAuthType, INT32 nProductID, SBuyGoldProp &buyGoldProp )
{
	SAuthBuyGoldProp* poProp = GetBuyGoldInfoByAuthType(byAuthType);
	if ( NULL == poProp)
	{
		return FALSE;
	}

    for( UINT32 dwIndex = 0; dwIndex < poProp->m_vecBuyGoldProp.size(); ++dwIndex )
    {
        if( poProp->m_vecBuyGoldProp[dwIndex].dwProductID == nProductID )
        {
            buyGoldProp = poProp->m_vecBuyGoldProp[dwIndex];
            return TRUE;
        }
    }
    return FALSE;
}

//根据产品ID获得该产品的属性
BOOL CVipPropMgr::GetBuyPropByProductID(UINT8 byAuthType, string strProductID, SBuyGoldProp &buyGoldProp )
{
	SAuthBuyGoldProp* poProp = GetBuyGoldInfoByAuthType(byAuthType);
	if ( NULL == poProp)
	{
		return FALSE;
	}

    for( UINT32 dwIndex = 0; dwIndex < poProp->m_vecBuyGoldProp.size(); ++dwIndex )
    {
        //检测一下产品ID是否相等
        if(strProductID == poProp->m_vecBuyGoldProp[dwIndex].aszProductID)
        {
            buyGoldProp = poProp->m_vecBuyGoldProp[dwIndex];
            return TRUE;
        }
    }
    return FALSE;
}


//根据充值金额获得该产品的属性
BOOL CVipPropMgr::GetBuyPropByRMB(UINT8 byAuthType, INT32 nRMB, SBuyGoldProp &buyGoldProp )
{
	SAuthBuyGoldProp* poProp = GetBuyGoldInfoByAuthType(byAuthType);
	if ( NULL == poProp)
	{
		return FALSE;
	}

    for( UINT32 dwIndex = 0; dwIndex < poProp->m_vecBuyGoldProp.size(); ++dwIndex )
    {
        if( poProp->m_vecBuyGoldProp[dwIndex].wRMB == nRMB )
        {
            buyGoldProp = poProp->m_vecBuyGoldProp[dwIndex];
            return TRUE;
        }
    }
    return FALSE;
}

UINT32 CVipPropMgr::GetVipTotalAmount(UINT8 byVipLevel)
{
    if(byVipLevel > m_byMaxVipLevel)
    {
        byVipLevel = m_byMaxVipLevel;
    }

    CVipLevelPropMapItr itr = m_mapVipLevelProp.find(byVipLevel);
    if(itr != m_mapVipLevelProp.end())
    {
        return itr->second.dwNeedRechargeGold;
    }

    return 0;
}


