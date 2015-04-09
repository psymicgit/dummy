#include "shopmgr.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <common/client/errdef.h>
#include <logic/item/itemmgr.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/dbconfig.h>
#include "json/json.h"
#include <logic/other/singleprammgr.h>
#include <logic/record/recordmgr.h>
#include <framework/gsapi.h>
#include <logic/plunder/plunderbasepropmgr.h>
#include <logic/godweapon/godweaponbasepropmgr.h>
#include <logic/base/basedefine.h>
#include <utility.h>
#include "logic/item/jewelry/jewelrypropmgr.h"
#include <db/dbmgr.h>

using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(CShopMgr)


	CShopMgr::CShopMgr()
{

}

CShopMgr::~CShopMgr()
{

}

BOOL CShopMgr::Init()
{
	if(!LoadFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);    
		return FALSE;
	}

	return TRUE;
}


VOID CShopMgr::UnInit()
{
	for(UINT8 byIdx = 0; byIdx < ESTT_MAX; byIdx++)
	{
		m_amapShopTabProp[byIdx].clear();
		m_avecShopTabProp[byIdx].clear();
		m_amapLevelItemCliData[byIdx].clear();
	}
	m_mapAllSellItem.clear();
	m_mapAllSellItemTab.clear();
}

BOOL CShopMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("shopprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return TRUE; //商店可以没有数据
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT8 byTabID = SGDP::SDAtou(pRes->GetFieldValueByName("TabID"));
		if(byTabID > ESTT_MAX)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: TabID[%d] invalid!"), MSG_MARK, byTabID);      
			return FALSE;
		}
		UINT16 wItemKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
		switch(CItemMgr::Instance()->GetItemKindBYKindID(wItemKindID))
		{
		case EIK_EQUIP:
			{
				SEquipProp* poEquipProp = CEquipPropMgr::Instance()->GetProp(wItemKindID);
				if(NULL == poEquipProp)
				{
					//异常
					SYS_CRITICAL(_SDT("[%s: %d]: poEquipProp is NULL, ItemKindID[%d]!"), MSG_MARK, wItemKindID);    
					return FALSE;
				}
				if((0 == poEquipProp->dwBuyCoin) && (0 == poEquipProp->dwBuyGold))
				{
					SYS_CRITICAL(_SDT("[%s: %d]: BuyCoin and BuyGold is 0, ItemKindID[%d]!"), MSG_MARK, wItemKindID);    
					return FALSE;
				}
			}
			break;
		case EIK_GOODS:
			{
				SGoodsProp* poGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemKindID);
				if(NULL == poGoodsProp)
				{
					//异常
					SYS_CRITICAL(_SDT("[%s: %d]: poGoodsProp is NULL, ItemKindID[%d]!"), MSG_MARK, wItemKindID);    
					return FALSE;
				}
				if((0 == poGoodsProp->dwBuyCoin) && (0 == poGoodsProp->dwBuyGold))
				{
					SYS_CRITICAL(_SDT("[%s: %d]: BuyCoin and BuyGold is 0, ItemKindID[%d]!"), MSG_MARK, wItemKindID);    
					return FALSE;
				}
			}
			break;
		default:
			SYS_CRITICAL(_SDT("[%s: %d]: invalid item kind, ItemKindID[%d]!"), MSG_MARK, wItemKindID);    
			return FALSE;
		}
		SShopPileLevel stShopPileLevel;
		stShopPileLevel.wPileNum = SGDP::SDAtou(pRes->GetFieldValueByName("PileNum"));	
		stShopPileLevel.wRequireLevel = SGDP::SDAtou(pRes->GetFieldValueByName("LevelRequire"));		
		if(ESTT_PILENUM == byTabID)
		{
			m_amapShopTabProp[byTabID][wItemKindID] = stShopPileLevel;//指定购买叠加数，则key为道具类型ID即可			
		}
		else
		{
			m_amapShopTabProp[byTabID][CLogicFunc::Get1616Key(wItemKindID, stShopPileLevel.wPileNum)] = stShopPileLevel;
			SShopItemLevel stProp;
			stProp.wItemID = wItemKindID;
			stProp.wPileNum = stShopPileLevel.wPileNum;
			stProp.wRequireLevel = stShopPileLevel.wRequireLevel;
			m_avecShopTabProp[byTabID].push_back(stProp);
		}			
		m_mapAllSellItem[wItemKindID] = wItemKindID;
		m_mapAllSellItemTab[wItemKindID] = byTabID;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
		
	return TRUE;
}


UINT16 CShopMgr::OpenShopByTab(UINT16 wPlayerLevel, UINT8 byTabID, UINT8& byItemNum, DT_ITEM_DATA_SHOP astItemDataList[])
{
	byItemNum =0;

	if((byTabID > ESTT_OTHER) ||(byTabID <= ESTT_INVALID))
	{
		return ERR_OPEN_SHOP::ID_TAB_NOT_EXIST_ERR;
	}
	CItemIDLevelVector& vecItem = m_avecShopTabProp[byTabID];
	for(CItemIDLevelVectorItr itr = vecItem.begin(); itr != vecItem.end(); itr++)
	{
		SShopItemLevel& stProp = *itr;
		UINT16 wKindID = stProp.wItemID;	
		if(wPlayerLevel < stProp.wRequireLevel)
		{
			continue;
		}
		UINT16 wPileNum = stProp.wPileNum;
		DT_ITEM_DATA_SHOP& stDT_ITEM_DATA_SHOP = astItemDataList[byItemNum++];
		CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(wKindID, wPileNum, stDT_ITEM_DATA_SHOP.stItemInfo);	
		switch(CItemMgr::Instance()->GetItemKindBYKindID(wKindID))
		{
		case EIK_EQUIP:
			{
				SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(wKindID);
				if(NULL == pstProp)
				{
					continue;
				}
				stDT_ITEM_DATA_SHOP.dwBuyCoin = pstProp->dwBuyCoin * wPileNum;
				stDT_ITEM_DATA_SHOP.dwBuyGold = pstProp->dwBuyGold * wPileNum;
			}
			break;
		case EIK_GOODS:
			{
				SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
				if(NULL == pstProp)
				{
					continue;
				}
				stDT_ITEM_DATA_SHOP.dwBuyCoin = pstProp->dwBuyCoin * wPileNum;
				stDT_ITEM_DATA_SHOP.dwBuyGold = pstProp->dwBuyGold * wPileNum;
			}
			break;
		}
		
	}

	return ERR_OPEN_SHOP::ID_SUCCESS;
}

UINT16 CShopMgr::Buy(CPlayer* poPlayer, UINT8 byTabID, UINT16 wItemKindID, UINT16 wPileCount)
{
	if(NULL == poPlayer)
	{
		RETURN_OTHER_ERR;
	}

	if((wItemKindID == CPlunderBasePropMgr::Instance()->GetPlunderLowerLevelItemID()) ||  //跨级掠夺符
		(wItemKindID == CGodweaponBasePropMgr::Instance()->GetUpgradeQualityItemID() ||
		CJewelryPropMgr::Instance()->IsStrengItemID( wItemKindID)) //神器进阶符
		)
	{
		//不需判断商店是否开放
	}
	else
	{
		if(NULL == poPlayer->GetBuild(EBK_SHOP))
		{
			return ERR_BUG_ITEM::ID_SHOP_NOT_EXIST_ERR;
		}
	}
	
	if((byTabID >= ESTT_MAX) ||(byTabID <= ESTT_INVALID))
	{
		return ERR_BUG_ITEM::ID_TABID_ERR;
	}

	//不需判断，tab判断即可，影响性
	/*
	CKindID2KindIDMapItr itr = m_mapAllSellItem.find(wItemKindID);
	if(itr == m_mapAllSellItem.end())
	{
		return ERR_BUG_ITEM::ID_ITEM_NOT_EXIST_ERR;	
	}
	*/

	if(0 == wPileCount)
	{
		return ERR_BUG_ITEM::ID_PILENUM0;
	}

	UINT16 wRequireLvl = 0;
	//可指定购买数量，直接判断是否存在即可
	if(ESTT_PILENUM == byTabID)
	{
		CKey2PileLevelMap& mapItem = m_amapShopTabProp[byTabID];
		CKey2PileLevelMapItr itr= mapItem.find(wItemKindID);
		if(itr == mapItem.end())
		{
			return ERR_BUG_ITEM::ID_ITEM_NOT_EXIST_ERR;	
		}
		wRequireLvl = itr->second.wRequireLevel;
	}
	//不为指定购买叠加数类型，则判断是否一致
	else
	{
		CKey2PileLevelMap& mapItem = m_amapShopTabProp[byTabID];
		CKey2PileLevelMapItr itr2 = mapItem.find(CLogicFunc::Get1616Key(wItemKindID, wPileCount));
		if(itr2 == mapItem.end())
		{
			return ERR_BUG_ITEM::ID_PILE_NOT_MATCH_TAB_ERR;
		}
		wRequireLvl = itr2->second.wRequireLevel;
	}
	if(poPlayer->GetLevel() < wRequireLvl)
	{
		return ERR_BUG_ITEM::ID_LEVEL_NOT_REACH;
	}

	UINT32 dwCostCoin = 0;
	UINT32 dwCostGold = 0;
	switch(CItemMgr::Instance()->GetItemKindBYKindID(wItemKindID))
	{
	case EIK_EQUIP:
		{
			SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(wItemKindID);
			if(NULL == pstProp)
			{
				RETURN_OTHER_ERR;
			}
			wPileCount = 1;
			dwCostCoin = pstProp->dwBuyCoin * wPileCount;
			dwCostGold = pstProp->dwBuyGold * wPileCount;
		
		}
		break;
	case EIK_GOODS:
		{
			SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemKindID);
			if(NULL == pstProp)
			{
				RETURN_OTHER_ERR;
			}
			dwCostCoin = pstProp->dwBuyCoin * wPileCount;
			dwCostGold = pstProp->dwBuyGold * wPileCount;			
		}
		break;
	default:
		RETURN_OTHER_ERR;
	}

	//以铜币为准
	if(0 != dwCostCoin)
	{
		if(poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < dwCostCoin)
		{
			return ERR_COMMON::ID_COIN_NOT_ENOUGH;
		}
	}
	//没有配置铜币价格，以元宝
	else if(0 != dwCostGold)
	{
		if(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
		{
			return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
		}
	}
	//两个都是0，配置错误，无法购买
	else
	{
		SYS_CRITICAL(_SDT("[%s: %d]: buycoin and buygold is 0!"), MSG_MARK);   
		RETURN_OTHER_ERR;
	}

	//生成道具
	ECreateItemRet Ret = ECIR_SUCCESS;
	if(NULL == CItemMgr::Instance()->CreateItemIntoBag(poPlayer, wItemKindID, wPileCount, Ret, 0, CRecordMgr::EAIT_BUY ))
	{
		//背包满
		if(ECIR_FAILED_BAG_FULL == Ret)
		{
			return ERR_BUG_ITEM::ID_BAG_FULL_ERR;
		}
		SYS_CRITICAL(_SDT("[%s: %d]: CreateItem failed[%d], ItemKindID=%d !"), MSG_MARK, Ret, wItemKindID);    
		RETURN_OTHER_ERR;
	}

	//扣除金钱...
	if(0 != dwCostCoin)
	{
		//异常
		if(poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < dwCostCoin)
		{
			SYS_CRITICAL( _SDT( "[%s:%d] player[%d] cost coin abnormal" ), MSG_MARK, poPlayer->GetID() );
			//poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin = 0;
			poPlayer->DecCoin( (UINT32)(poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin), CRecordMgr::EDCT_SHOPBUY, wItemKindID, wPileCount );
		}
		//正常
		else
		{
			//poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin -= dwCostCoin;
			poPlayer->DecCoin(dwCostCoin, CRecordMgr::EDCT_SHOPBUY, wItemKindID, wPileCount);
		}
	}
	//没有配置铜币价格，以元宝
	else if(0 != dwCostGold)
	{
		//异常
		if(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
		{
			SYS_CRITICAL( _SDT( "[%s:%d] player[%d] cost gold abnormal" ), MSG_MARK, poPlayer->GetID() );
			//poPlayer->GetDT_PLAYER_BASE_DATA().qwGold = 0;
			poPlayer->DecGold( (UINT32)(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold), CRecordMgr::EDGT_BUY,  wItemKindID, wPileCount);
		}
		//正常
		else
		{
			//poPlayer->GetDT_PLAYER_BASE_DATA().qwGold -= dwCostGold;
			poPlayer->DecGold(dwCostGold, CRecordMgr::EDGT_BUY, wItemKindID, wPileCount);
		}
	}

	return ERR_BUG_ITEM::ID_SUCCESS;
}

UINT8  CShopMgr::GetTabID(UINT16 wItemKindID, UINT8 & byTabID)
{
	CKindID2KindIDMapItr itr = m_mapAllSellItemTab.find(wItemKindID);
	if (itr != m_mapAllSellItemTab.end())
	{
		byTabID =  (UINT8)itr->second;
		return TRUE;
	}
	return FALSE;
}

