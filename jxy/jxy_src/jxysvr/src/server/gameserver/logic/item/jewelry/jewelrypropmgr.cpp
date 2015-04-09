#include "jewelrypropmgr.h"
#include "jewelry.h"
#include <framework/gsapi.h>
#include "logic/base/logicfunc.h"
#include "logic/other/singleprammgr.h"
#include <db/dbmgr.h>
IMPLEMENT_SINGLETON_PROPMGR(CJewelryPropMgr);
CJewelryPropMgr::CJewelryPropMgr()
{
	m_mapJewelryProp.clear();

	m_mapJewelryUpgradeProp.clear();
}

CJewelryPropMgr::~CJewelryPropMgr()
{
	m_mapJewelryProp.clear();

	m_mapJewelryUpgradeProp.clear();
}

BOOL CJewelryPropMgr::Init()
{
	if ( ! LoadExtRandomFromDB())
	{
		return FALSE;
	}
	if( !LoadBaseAttrFromDB() )
	{
		return FALSE;
	}

	if ( !LoadRandomFromDB())
	{
		return FALSE;
	}

	if ( !LoadStrengAttrFromDB())
	{
		return FALSE;
	}

	if ( !LoadUpgradeFromDB())
	{
		return FALSE;
	}

	if ( !LoadResolveFromDB() )
	{
		return FALSE;
	}

    if ( !LoadSuitAttFromDB() ){
        return FALSE;
    }

	return TRUE;
}

VOID CJewelryPropMgr::UnInit()
{
	m_mapJewelryProp.clear();
	m_mapJewelryUpgradeProp.clear();
}
BOOL CJewelryPropMgr::LoadExtRandomFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jewelryextrandomprop", " order by ExtAttrID").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT8 byExtAttrID = SDAtou(pRes->GetFieldValueByName("ExtAttrID"));
		UINT8 byRandomValue = SDAtou(pRes->GetFieldValueByName("RandomValue"));
		UINT8 byAddValueType = SDAtou(pRes->GetFieldValueByName("ValueType"));
		m_mapExtIDRandom[byExtAttrID] = byRandomValue;
		m_mapExtIDValueType[byExtAttrID] = byAddValueType;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CJewelryPropMgr::LoadSuitAttFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession){
        return FALSE;
    }

    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;

    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jewelrysuitprop", " order by dresslvl asc, suitlvl asc").c_str(), &pRes, &dwErrID,  &strErr);

    if(SDDB_HAS_RECORDSET != nRet){
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
        return FALSE;
    }	

    m_mapSuit.clear();

    // 打印查询的表结果
    while(pRes->GetRecord()){
        UINT16 wDressLvl = SDAtou(pRes->GetFieldValueByName("dresslvl"));

        SJewelrySuitAttrProp stAttrProp;

        stAttrProp.wJewelryStrengLvl = SDAtou(pRes->GetFieldValueByName("suitlvl"));
        stAttrProp.eAttrType         = (JewelryExtAttr)SDAtou(pRes->GetFieldValueByName("attrtype"));
        stAttrProp.dwAttrVal         = SDAtou(pRes->GetFieldValueByName("attrval"));
        stAttrProp.eValType          = (ValueType)SDAtou(pRes->GetFieldValueByName("valtype"));

        SJewelrySuitProp &stSuitProp = m_mapSuit[wDressLvl];
        stSuitProp.vecAttr.push_back(stAttrProp);
        stSuitProp.wJewelryDressLvl = wDressLvl;
    }

    for(CMapJewelrySuit::iterator itr = m_mapSuit.begin(); itr != m_mapSuit.end(); ++itr){
        SJewelrySuitProp &stProp = itr->second;

        for ( UINT8 i = 0; i < stProp.vecAttr.size(); i++){
            SJewelrySuitAttrProp &stAttrProp = stProp.vecAttr[i];
            DT_JEWELRY_SUIT_ATTR &stAttrCli  = stProp.stSuitCli.astSuitAttrLst[i];

            stAttrCli.wSuitLvl = stAttrProp.wJewelryStrengLvl;
            stAttrCli.stLvlAttr.byAttrType = stAttrProp.eAttrType;
            stAttrCli.stLvlAttr.dwAttrVal  = stAttrProp.dwAttrVal;
            stAttrCli.stLvlAttr.byValType  = stAttrProp.eValType;
        }

        stProp.stSuitCli.bySuitAttrNum = stProp.vecAttr.size();
    }

    if(pRes){
        pRes->Release();
    }

    return TRUE;
}

SJewelryProp* CJewelryPropMgr::GetProp(UINT16 wKindID)
{
	CMapJewelryPropItr itr = m_mapJewelryProp.find(wKindID);
	if ( itr == m_mapJewelryProp.end() )
	{
		return NULL;
	}
	return &itr->second;
}

BOOL CJewelryPropMgr::LoadResolveFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jewelryresolveprop", " order by JewelryLevel, ColorType").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SJewelryResolveProp stProp;
		stProp.wJewelryLevel = SDAtou(pRes->GetFieldValueByName("JewelryLevel"));
		stProp.byColorType = SDAtou(pRes->GetFieldValueByName("ColorType"));
		

		SJewelryResolveItemProp stItemProp;
		stItemProp.wItemID = SDAtou(pRes->GetFieldValueByName("FirstItemID"));
		stItemProp.wItemProbility = SDAtou(pRes->GetFieldValueByName("FirstItemProbability"));
		stItemProp.wItemNum = SDAtou(pRes->GetFieldValueByName("FirstItemNum"));
		stProp.mapItemProp[stItemProp.wItemID] = stItemProp;
		stProp.mapItemRandom[stItemProp.wItemID] = stItemProp.wItemProbility;

		stItemProp.wItemID = SDAtou(pRes->GetFieldValueByName("SecondItemID"));
		stItemProp.wItemProbility = SDAtou(pRes->GetFieldValueByName("SecondItemProbability"));
		stItemProp.wItemNum = SDAtou(pRes->GetFieldValueByName("SecondItemNum"));
		stProp.mapItemProp[stItemProp.wItemID] = stItemProp;
		stProp.mapItemRandom[stItemProp.wItemID] = stItemProp.wItemProbility;

		stItemProp.wItemID = SDAtou(pRes->GetFieldValueByName("ThirdItemID"));
		stItemProp.wItemProbility = SDAtou(pRes->GetFieldValueByName("ThirdItemProbability"));
		stItemProp.wItemNum = SDAtou(pRes->GetFieldValueByName("ThirdItemNum"));
		stProp.mapItemProp[stItemProp.wItemID] = stItemProp;
		stProp.mapItemRandom[stItemProp.wItemID] = stItemProp.wItemProbility;

		stItemProp.wItemID = SDAtou(pRes->GetFieldValueByName("FourthItemID"));
		stItemProp.wItemProbility = SDAtou(pRes->GetFieldValueByName("FourthItemProbability"));
		stItemProp.wItemNum = SDAtou(pRes->GetFieldValueByName("FourthItemNum"));
		stProp.mapItemProp[stItemProp.wItemID] = stItemProp;
		stProp.mapItemRandom[stItemProp.wItemID] = stItemProp.wItemProbility;

		stItemProp.wItemID = SDAtou(pRes->GetFieldValueByName("FifthItemID"));
		stItemProp.wItemProbility = SDAtou(pRes->GetFieldValueByName("FifthItemProbability"));
		stItemProp.wItemNum = SDAtou(pRes->GetFieldValueByName("FifthItemNum"));
		stProp.mapItemProp[stItemProp.wItemID] = stItemProp;
		stProp.mapItemRandom[stItemProp.wItemID] = stItemProp.wItemProbility;

		stProp.dwCostRes = SDAtou(pRes->GetFieldValueByName("CostRes"));
		
		m_mapJewelryResoleProp[CLogicFunc::Get1616Key(stProp.wJewelryLevel, stProp.byColorType)] = stProp;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CJewelryPropMgr::LoadUpgradeFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jewelryupgradecost", " order by JewelryLevel, Strenglevel").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SJewelryUpgradeProp stProp;
		stProp.wJewelryLevel = SDAtou(pRes->GetFieldValueByName("JewelryLevel"));
		stProp.wStrengLevel = SDAtou(pRes->GetFieldValueByName("StrengLevel"));
		stProp.dwUpgradeItemID = SDAtou(pRes->GetFieldValueByName("UpgradeItemID"));
		stProp.wUpgradeNum = SDAtou(pRes->GetFieldValueByName("UpgradeNum"));
		m_mapJewelryUpgradeProp[CLogicFunc::Get1616Key(stProp.wJewelryLevel, stProp.wStrengLevel)] = stProp;
		if ( 1 != stProp.wStrengLevel )
		{
			CMapUpgradeItemNum mapItemNum = m_mapLevelToItemProp[CLogicFunc::Get1616Key(stProp.wJewelryLevel, stProp.wStrengLevel - 1)];
			mapItemNum[stProp.dwUpgradeItemID] += stProp.wUpgradeNum;
			m_mapLevelToItemProp[CLogicFunc::Get1616Key(stProp.wJewelryLevel, stProp.wStrengLevel)] = mapItemNum;
		}
		else
		{
			CMapUpgradeItemNum mapItemNum;
			mapItemNum[stProp.dwUpgradeItemID] = stProp.wUpgradeNum;
			m_mapLevelToItemProp[CLogicFunc::Get1616Key(stProp.wJewelryLevel, stProp.wStrengLevel)] = mapItemNum;
		}
		
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CJewelryPropMgr::LoadStrengAttrFromDB()
{

	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jewelrystrengprop", " order by JewelryLevel, ColorType").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SJewelryStrengProp stProp;
		stProp.wJewelryLevel = SDAtou(pRes->GetFieldValueByName("JewelryLevel"));
		stProp.wStrengLevel = SDAtou(pRes->GetFieldValueByName("StrengLevel"));
		stProp.byColorType = SDAtou(pRes->GetFieldValueByName("ColorType"));
		stProp.byExtAttrID = SDAtou(pRes->GetFieldValueByName("ExtAttrID"));
		stProp.dwExtValue = SDAtou(pRes->GetFieldValueByName("ExtValue"));

		UINT32 dwRandomKey = CLogicFunc::Get1616Key(stProp.wJewelryLevel, stProp.byColorType);
		if ( m_mapAttrRandom.find(dwRandomKey) == m_mapAttrRandom.end())
		{
			C3232Map mapRandom;
			mapRandom[stProp.byExtAttrID] = m_mapExtIDRandom[stProp.byExtAttrID];
			m_mapAttrRandom[dwRandomKey] = mapRandom;
		}
		else
		{
			m_mapAttrRandom[dwRandomKey].insert( make_pair(stProp.byExtAttrID, m_mapExtIDRandom[stProp.byExtAttrID]) );
		}
		UINT32 dwSecondKey = CLogicFunc::Get1616Key(stProp.wStrengLevel, stProp.byExtAttrID);
		m_mapJewelryStrengProp[CLogicFunc::Get3232Key(dwRandomKey, dwSecondKey)] = stProp;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CJewelryPropMgr::LoadRandomFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jewelryposrandomprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT32 dwPosID = SDAtou(pRes->GetFieldValueByName("PosID"));
		UINT32 dwRandomValue = SDAtou(pRes->GetFieldValueByName("RandomValue"));

		m_mapPosRandom[dwPosID] = dwRandomValue;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CJewelryPropMgr::LoadBaseAttrFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("jewelryprop", " order by KindID").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SJewelryProp stProp = {0};
		
		stProp.wKindID = SDAtou(pRes->GetFieldValueByName("KindID"));
		stProp.wDressLevel = SDAtou(pRes->GetFieldValueByName("DressLevel"));
		stProp.byColorType = SDAtou(pRes->GetFieldValueByName("ColorType"));
		SDStrncpy(stProp.aszName, pRes->GetFieldValueByName("Name"), NORMAL_MSG_LEN - 1);
		stProp.dwHP = SDAtou(pRes->GetFieldValueByName("HP"));
		stProp.dwAttack = SDAtou(pRes->GetFieldValueByName("Attack"));
		stProp.dwDefend = SDAtou(pRes->GetFieldValueByName("Defend"));
		stProp.dwCriRate = SDAtou(pRes->GetFieldValueByName("CriRate"));
		stProp.dwDecRitRate = SDAtou(pRes->GetFieldValueByName("DecRitRate"));
		stProp.dwDodgeRate = SDAtou(pRes->GetFieldValueByName("DodgeRate"));
		stProp.dwHitRate = SDAtou(pRes->GetFieldValueByName("HitRate"));
		stProp.byPosKindID = SDAtou(pRes->GetFieldValueByName("PosKindID"));
		stProp.dwFirstBase = SDAtou(pRes->GetFieldValueByName("FirstBase"));
		stProp.dwSecondBase = SDAtou(pRes->GetFieldValueByName("SecondBase"));
		m_mapJewelryProp[stProp.wKindID] = stProp;
		m_mapJewelryPropByLevel[CLogicFunc::Get1616Key( stProp.wDressLevel, CLogicFunc::Get88Key( stProp.byColorType, stProp.byPosKindID))] = stProp;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}
BOOL CJewelryPropMgr::GenerateJewelryData(CPlayer* poPlayer, UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData)
{
	memset(&stJewelryData, 0, sizeof(DT_JEWELRY_DATA));

	if( !SetBaseInof(wKindID, stJewelryData))
	{
		return FALSE;
	}

	if( !SetBaseBattleAttr(stJewelryData.wJewelryID, stJewelryData))
	{
		return FALSE;
	}

	if ( !SetHideBattleAttr(stJewelryData.wJewelryID, stJewelryData))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CJewelryPropMgr::GenerateJewelryData(CPlayer* poPlayer, UINT16 wKindID, C32Vec& vec32, DT_JEWELRY_DATA& stJewelryData)
{
	memset(&stJewelryData, 0, sizeof(DT_JEWELRY_DATA));

	if( !SetBaseInof(wKindID, stJewelryData))
	{
		return FALSE;
	}

	if( !SetBaseBattleAttr(stJewelryData.wJewelryID, stJewelryData))
	{
		return FALSE;
	}

	if ( !SetHideBattleAttr(stJewelryData.wJewelryID, vec32, stJewelryData))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CJewelryPropMgr::SetHideBattleAttr(UINT16 wKindID, C32Vec& vec32, DT_JEWELRY_DATA& stJewelryData)
{
	CMapJewelryPropItr itr = m_mapJewelryProp.find(wKindID);
	if ( itr == m_mapJewelryProp.end())
	{
		return FALSE;
	}
	UINT8 byRandom = 0;
	if ( itr->second.byColorType == EC_BLUE)
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryBlueNum();
	}else if( itr->second.byColorType == EC_PURPLE )
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryPurpleNum();
	}
	else if ( EC_RED == itr->second.byColorType)
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryRedNum();
	}
	else if ( EC_ORANGE == itr->second.byColorType )
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryOrangeNum();
	}
	stJewelryData.stExtAttrLst.byExtAttrNum = byRandom;
	CRandomMap mapRandom = m_mapAttrRandom[CLogicFunc::Get1616Key( itr->second.wDressLevel, itr->second.byColorType)];
	for ( UINT8 byIdx = 0; byIdx < byRandom; byIdx++)
	{
		UINT8 byKindID = GetExtAttrRandom(mapRandom);;
		if ( byIdx < vec32.size())
		{
			if( mapRandom.end() != mapRandom.find(vec32[byIdx]))
			{
				byKindID = vec32[byIdx];
			}
			
		}
		stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].byKindID = byKindID;
		stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].byLevel = 0;
		stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = GetExtAttrValue(wKindID, byKindID, stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].byLevel);

		mapRandom.erase(byKindID);
	}
	return TRUE;
}

VOID CJewelryPropMgr::GetStrentItemInfo(CPlayer* poPlayer, DT_STRENGTH_LST_INFO& stItemInfo)
{
	if ( NULL == poPlayer)
	{
		return ;
	}
	CBag& oBag = poPlayer->GetBag();
	CID2GoodsShmemMap& mapGoods = oBag.GetGoodsMap();
	CGoods* poGoods = mapGoods.GetFistData(); 
	UINT8 byItemNum = 0;
	while (poGoods)
	{
		const SGoodsProp* poProp = poGoods->GetGoodsProp();
		if ( NULL != poProp)
		{
			if ( EGMK_JEWELRY_ADVANCED == poProp->byMainKindID )
			{
				stItemInfo.astItemInfo[byItemNum].wItemID = poProp->wKindID;
				stItemInfo.astItemInfo[byItemNum].dwCurItemNum = poGoods->GetCurPileNum();
				if ( 0 != poProp->dwBuyCoin)
				{
					stItemInfo.astItemInfo[byItemNum].stCostRes.byResKind = ACTE_COIN;
					stItemInfo.astItemInfo[byItemNum].stCostRes.qwResValue = poProp->dwBuyCoin;
				}
				else if ( 0 != poProp->dwBuyGold)
				{
					stItemInfo.astItemInfo[byItemNum].stCostRes.byResKind = ACTE_GOLD;
					stItemInfo.astItemInfo[byItemNum].stCostRes.qwResValue = poProp->dwBuyGold;
				}

				byItemNum++;
			}
		}
		poGoods = mapGoods.GetNextData(); 
	}
	stItemInfo.byItemKindNum = byItemNum;
}

UINT16 CJewelryPropMgr::GetRandomJewelryID(UINT16 wLevel, UINT8 byColor)
{
	UINT8 byPosKind = GetPosRandom();//随机佩带位置
	UINT32 dwKey = CLogicFunc::Get1616Key(wLevel, CLogicFunc::Get88Key(byColor, byPosKind));
	CMapJewelryPropByLevelColorItr itr = m_mapJewelryPropByLevel.find(dwKey);
	if ( itr == m_mapJewelryPropByLevel.end())
	{
		return 0;
	}
	return itr->second.wKindID;
}

BOOL CJewelryPropMgr::SetHideBattleAttr(UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData)
{
	CMapJewelryPropItr itr = m_mapJewelryProp.find(wKindID);
	if ( itr == m_mapJewelryProp.end())
	{
		return FALSE;
	}
	UINT8 byRandom = 0;
	if ( itr->second.byColorType == EC_BLUE)
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryBlueNum();
	}else if( itr->second.byColorType == EC_PURPLE )
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryPurpleNum();
	}
	else if ( EC_RED == itr->second.byColorType)
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryRedNum();
	}
	else if ( EC_ORANGE == itr->second.byColorType )
	{
		byRandom = CSinglePramMgr::Instance()->GetJewelryOrangeNum();
	}
	stJewelryData.stExtAttrLst.byExtAttrNum = byRandom;
	CRandomMap mapRandom = m_mapAttrRandom[CLogicFunc::Get1616Key( itr->second.wDressLevel, itr->second.byColorType)];
	for ( UINT8 byIdx = 0; byIdx < byRandom; byIdx++)
	{
		UINT8 byKindID = GetExtAttrRandom(mapRandom);
		stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].byKindID = byKindID;
		stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].byLevel = 0;
		stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = GetExtAttrValue(wKindID, byKindID, stJewelryData.stExtAttrLst.astExtAttrInfo[byIdx].byLevel);
		
		mapRandom.erase(byKindID);
	}
	return TRUE;
}

BOOL CJewelryPropMgr::SetBaseInof(UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData)
{
	
	CMapJewelryPropItr itr = m_mapJewelryProp.find(wKindID);
	if ( itr == m_mapJewelryProp.end())
	{
		return FALSE;
	}
	return SetBaseInof(itr->second, stJewelryData);

}

BOOL CJewelryPropMgr::SetBaseInof(SJewelryProp& stProp, DT_JEWELRY_DATA& stJewelryData)
{
	stJewelryData.wJewelryID = stProp.wKindID;
	stJewelryData.wJewelryLevel = stProp.wDressLevel;
	stJewelryData.qwItemNewTime = SGDP::SDTimeSecs();
	stJewelryData.byJewelryDressPos = stProp.byPosKindID;
	
	return TRUE;
}

BOOL CJewelryPropMgr::SetBaseBattleAttr(UINT16 wKindID, DT_JEWELRY_DATA& stJewelryData)
{
	CMapJewelryPropItr itr = m_mapJewelryProp.find(wKindID);
	if ( itr == m_mapJewelryProp.end())
	{
		return FALSE;
	}
	UINT8 byNum = 0;
	if( 0 != itr->second.dwAttack)
	{
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].byAttrKind = EBAK_ATTACK;
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].dwAttrValue = itr->second.dwAttack;
		byNum++;
	}

	if ( 0 != itr->second.dwHP)
	{
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].byAttrKind = EBAK_HP;
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].dwAttrValue = itr->second.dwHP;
		byNum++;
	}
	
	if ( 0 != itr->second.dwDefend)
	{
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].byAttrKind = EBAK_DEFEND;
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].dwAttrValue = itr->second.dwDefend;
		byNum++;
	}

	if ( 0 != itr->second.dwCriRate)
	{
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].byAttrKind = EBAK_CRITRATE;
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].dwAttrValue = itr->second.dwCriRate;
		byNum++;
	}


	if ( 0 != itr->second.dwDecRitRate)
	{
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].byAttrKind = EBAK_DECRITRATE;
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].dwAttrValue = itr->second.dwDecRitRate;
		byNum++;
	}

	if ( 0 != itr->second.dwDodgeRate)
	{
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].byAttrKind = EBAK_DODGERATE;
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].dwAttrValue = itr->second.dwDodgeRate;
		byNum++;
	}

	if ( 0 != itr->second.dwHitRate)
	{
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].byAttrKind = EBAK_HITRATE;
		stJewelryData.stBaseAttrLst.astAttrInfo[byNum].dwAttrValue = itr->second.dwHitRate;
		byNum++;
	}

	stJewelryData.stBaseAttrLst.byAttrNum = byNum;
	return TRUE;
}

UINT32 CJewelryPropMgr::GetPosRandom()
{
	return GetRandomIdxMustHit(m_mapPosRandom);
}

UINT32 CJewelryPropMgr::GetExtAttrRandom( CRandomMap& mapRandom)
{
	return GetRandomIdxMustHit(mapRandom);
}

SJewelryUpgradeProp* CJewelryPropMgr::GetUpgradeProp(UINT16 wJewelryLevel, UINT16 wStrengLevel)
{
	UINT32 qwKey = CLogicFunc::Get1616Key(wJewelryLevel, wStrengLevel + 1);
	CMapJewelryUpgradePropItr itr = m_mapJewelryUpgradeProp.find(qwKey);
	if ( itr == m_mapJewelryUpgradeProp.end())
	{
		return NULL;
	}
	return &itr->second;
}

UINT32 CJewelryPropMgr::GetBaseAttrValue(UINT16 wJewelryID, UINT8 byKindID, UINT16 wStrengLevel)
{
	SJewelryProp* poProp = GetProp(wJewelryID);
	if ( NULL == poProp)
	{
		return 0;
	}

	switch ( byKindID )
	{
	case EBAK_HP:
	{
		if( 0 != poProp->dwHP )
		{
			return poProp->dwHP + poProp->dwSecondBase * wStrengLevel;
		}
	}
	break;
	case EBAK_ATTACK:
	{
		if( 0 != poProp->dwAttack )
		{
			return poProp->dwAttack + poProp->dwSecondBase * wStrengLevel;
		}
	}
		break;
	case EBAK_HITRATE:
	{
		if( 0 != poProp->dwHitRate )
		{
			return poProp->dwHitRate + poProp->dwSecondBase * wStrengLevel;
		}
	}
	break;
	case EBAK_DODGERATE:
	{
		if( 0 != poProp->dwDodgeRate )
		{
			return poProp->dwDodgeRate + poProp->dwSecondBase * wStrengLevel;
		}
	}
	break;
		
	case EBAK_CRITRATE:
	{
		if( 0 != poProp->dwCriRate )
		{
			return poProp->dwCriRate + poProp->dwSecondBase * wStrengLevel;
		}
	}
	break;
	case EBAK_DECRITRATE:
	{
		if( 0 != poProp->dwDecRitRate )
		{
			return poProp->dwDecRitRate + poProp->dwSecondBase * wStrengLevel;
		}
	}
	break;
	case EBAK_DEFEND:
	{
		if( 0 != poProp->dwDefend )
		{
			return poProp->dwDefend + poProp->dwFirstBase * wStrengLevel;
		}
	}
	break;
	default:
		return 0;
	}
	return 0;
}

UINT32 CJewelryPropMgr::GetExtAttrValue(UINT16 wKindID, UINT8 byExtId, UINT16 wStrengLevel)
{
	CMapJewelryPropItr itr = m_mapJewelryProp.find(wKindID);
	if ( itr == m_mapJewelryProp.end())
	{
		return 0;
	}

	UINT32 dwFirstKey = CLogicFunc::Get1616Key(itr->second.wDressLevel, itr->second.byColorType);
	UINT32 dwSecondKey = CLogicFunc::Get1616Key(wStrengLevel, byExtId);
	UINT64 qwKey = CLogicFunc::Get3232Key(dwFirstKey, dwSecondKey);
	CMapJewelryStrengPropItr resItr = m_mapJewelryStrengProp.find(qwKey);
	if ( m_mapJewelryStrengProp.end() == resItr)
	{
		return 0;
	}
	return resItr->second.dwExtValue;
}

SJewelryUpgradeProp* CJewelryPropMgr::GetUpgradeItemProp(UINT16 wJewelryLevel, UINT16 wStrengLevel)
{
	UINT32 dwKey = CLogicFunc::Get1616Key(wJewelryLevel, wStrengLevel + 1);
	CMapJewelryUpgradePropItr itr = m_mapJewelryUpgradeProp.find(dwKey);
	if ( itr == m_mapJewelryUpgradeProp.end())
	{
		return NULL;
	}
	return &itr->second;

}

SJewelryResolveProp* CJewelryPropMgr::GetResolveProp(UINT16 wJewelryLevel, UINT8 byColorType)
{
	UINT32 dwKey = CLogicFunc::Get1616Key(wJewelryLevel, byColorType);
	CMapJewelryResolvePropItr itr = m_mapJewelryResoleProp.find(dwKey);
	if ( m_mapJewelryResoleProp.end() == itr)
	{
		return NULL;
	}
	return &itr->second;
}

CMapUpgradeItemNum* CJewelryPropMgr::GetResolveReturn(UINT16 wJewelryLevel, UINT16 wStrengLevel)
{
	UINT32 dwKey = CLogicFunc::Get1616Key(wJewelryLevel, wStrengLevel);
	CMapLevelUpgradeItemPropItr itr = m_mapLevelToItemProp.find(dwKey);
	if ( itr == m_mapLevelToItemProp.end() )
	{
		return NULL;
	}
	return &itr->second;
}

CJewelry* CJewelryPropMgr::CompareJewelry(CJewelry* poJewelry1, CJewelry* poJewelry2)
{
	if ( NULL == poJewelry1 )
	{
		return poJewelry1;
	}

	if ( NULL == poJewelry2)
	{
		return poJewelry2;
	}

	if( poJewelry1->GetDressLevel() < poJewelry2->GetDressLevel())
	{
		return poJewelry1;
	}

	if ( poJewelry1->GetColorType() < poJewelry2->GetColorType() )
	{
		return poJewelry1;
	}

	if ( poJewelry1->GetDT_JEWELRY_DATA().wStrengLevel <= poJewelry2->GetDT_JEWELRY_DATA().wStrengLevel)
	{
		return poJewelry1;
	}

	return poJewelry2;
}

VOID CJewelryPropMgr::GetJewelryCli(DT_JEWELRY_DATA& stInData, DT_JEWELRY_DATA_CLI& stOutData)
{
    stOutData.wJewelryID = stInData.wJewelryID;
    stOutData.wJewelryLevel = stInData.wJewelryLevel;
    stOutData.byJewelryDressPos = (NULL == GetProp(stInData.wJewelryID) ? EJDP_NECKLACE : GetProp(stInData.wJewelryID)->byPosKindID);
    stOutData.stBaseAttrLst = stInData.stBaseAttrLst;
    stOutData.wIdx = stInData.wIdx;
    stOutData.wStrengLevel = stInData.wStrengLevel;
    stOutData.wHeroID =  0;
    stOutData.stExtAttrLst.byExtAttrNum = stInData.stExtAttrLst.byExtAttrNum;
    for ( UINT8 byIdx = 0; byIdx < stOutData.stExtAttrLst.byExtAttrNum; byIdx++)
    {
        //ID
        stOutData.stExtAttrLst.astExtAttrInfo[byIdx].byKindID = stInData.stExtAttrLst.astExtAttrInfo[byIdx].byKindID;
        //基础值
        stOutData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = CJewelryPropMgr::Instance()->GetExtAttrValue( stInData.wJewelryID, stInData.stExtAttrLst.astExtAttrInfo[byIdx].byKindID, 0);
        stInData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = CJewelryPropMgr::Instance()->GetExtAttrValue( stInData.wJewelryID, stInData.stExtAttrLst.astExtAttrInfo[byIdx].byKindID, 
            stInData.stExtAttrLst.astExtAttrInfo[byIdx].byLevel);
        //强化等级
        stOutData.stExtAttrLst.astExtAttrInfo[byIdx].byExtLevel = stInData.stExtAttrLst.astExtAttrInfo[byIdx].byLevel;
        //强化加成值
        stOutData.stExtAttrLst.astExtAttrInfo[byIdx].dwExtValue = stInData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue < stOutData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue ? 0 : 
            stInData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue - stOutData.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue;
        //值类型
        stOutData.stExtAttrLst.astExtAttrInfo[byIdx].dwValueType = CJewelryPropMgr::Instance()->GetValueType(stOutData.stExtAttrLst.astExtAttrInfo[byIdx].byKindID);
    }
    stOutData.byStrengExtLevel = CSinglePramMgr::Instance()->GetJewelryStrengIntervalLevel();
    stOutData.byExtStrengNum = 0;
}

BOOL CJewelryPropMgr::IsStrengItemID(UINT16 wKindID)
{
	SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
	if ( NULL != pstProp)
	{
		if ( EGMK_JEWELRY_ADVANCED == pstProp->byMainKindID )
		{
			return TRUE;
		}
	}
	return FALSE;
}

bool CompareJewelryStrengLvl(const SJewelrySuitAttrProp& x,const SJewelrySuitAttrProp& y)
{
    return x.wJewelryStrengLvl >= y.wJewelryStrengLvl;
}

SJewelrySuitProp* CJewelryPropMgr::GetJewelrySuitProp(UINT16 wJewelryLevel, UINT16 wStrengLvl)
{
    CMapJewelrySuit::iterator itr = m_mapSuit.find(wJewelryLevel);
    if (itr == m_mapSuit.end()){
        return NULL;
    }

    SJewelrySuitProp &stJewelrySuit = itr->second;
    return &stJewelrySuit;
}

UINT16 CJewelryPropMgr::GetJewelrySuitLvl(SJewelrySuitProp &stJewelrySuit, UINT16 wStrengLvl)
{
    JewelrySuitAttrVec &vecAttr = stJewelrySuit.vecAttr;
    if (vecAttr.empty() || 0 == wStrengLvl){
        return 0;
    }

    // 这里不用uppper_bound，因为不方便
    for (UINT8 i = vecAttr.size() - 1; i >= 0; i--){
        if (vecAttr[i].wJewelryStrengLvl <= wStrengLvl){
            return vecAttr[i].wJewelryStrengLvl;
        }
    }

    return 0;
}
