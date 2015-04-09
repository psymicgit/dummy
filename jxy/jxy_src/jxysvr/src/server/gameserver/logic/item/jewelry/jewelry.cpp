#include "jewelry.h"
#include "jewelrypropmgr.h"
#include "logic/hero/hero.h"
#include "logic/other/singleprammgr.h"
#include "logic/item/itemmgr.h"
#include "logic/player/player.h"
CJewelry::CJewelry()
{
	memset(&m_stDT_JEWELRY_DATA, 0, sizeof(DT_JEWELRY_DATA));
	m_poHero = NULL;
}

CJewelry::~CJewelry()
{
	memset(&m_stDT_JEWELRY_DATA, 0, sizeof(DT_JEWELRY_DATA));
	m_poHero = NULL;
}

UINT16&	CJewelry::GetBagPos()
{
	return m_stDT_JEWELRY_DATA.wBagPos;
}

BOOL CJewelry::Init(VOID* pData, CPlayer* poOwner)
{
	if ( NULL == poOwner)
	{
		return FALSE;
	}
	m_poOwner = poOwner;
	memcpy(&m_stDT_JEWELRY_DATA, pData, sizeof(DT_JEWELRY_DATA));
	
	return TRUE;
}

UINT16 CJewelry::Sell(UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI &stRecvItemNumList) 
{
	return ERR_SELL_ITEM::ID_SELL_PROHIBIT;
}

void CJewelry::GetRecoverScience(UINT32 & dwMinScience, UINT32 & dwMaxScience)
{
	dwMaxScience = 0;
	dwMinScience = 0;
}

DT_JEWELRY_DATA& CJewelry::GetDT_JEWELRY_DATA()
{
	return m_stDT_JEWELRY_DATA;
}

SJewelryProp* CJewelry::GetProp()
{
	return CJewelryPropMgr::Instance()->GetProp(m_stDT_JEWELRY_DATA.wJewelryID);
}

VOID CJewelry::SetHero(CHero* poHero)
{ 
	m_poHero = poHero; 
	if ( NULL == poHero)
	{
		m_stDT_JEWELRY_DATA.wHeroKindID = 0;	
	}
	else
	{
		m_stDT_JEWELRY_DATA.wHeroKindID = poHero->GetHeroID();
	}
	
}

VOID CJewelry::GetJewelryDataCli(DT_JEWELRY_DATA_CLI& stJewelryCli)
{
	stJewelryCli.wJewelryID = m_stDT_JEWELRY_DATA.wJewelryID;
	stJewelryCli.wJewelryLevel = m_stDT_JEWELRY_DATA.wJewelryLevel;
	stJewelryCli.byJewelryDressPos = (NULL == GetProp() ? EJDP_NECKLACE :GetProp()->byPosKindID);
	stJewelryCli.stBaseAttrLst = m_stDT_JEWELRY_DATA.stBaseAttrLst;
	stJewelryCli.wIdx = m_stDT_JEWELRY_DATA.wIdx;
	stJewelryCli.wStrengLevel = m_stDT_JEWELRY_DATA.wStrengLevel;
	stJewelryCli.wHeroID = ((NULL == m_poHero) ? 0 : m_poHero->GetDT_HERO_BASE_DATA().wKindID);
	stJewelryCli.stExtAttrLst.byExtAttrNum = m_stDT_JEWELRY_DATA.stExtAttrLst.byExtAttrNum;
	for ( UINT8 byIdx = 0; byIdx < stJewelryCli.stExtAttrLst.byExtAttrNum; byIdx++)
	{
		//ID
		stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].byKindID = m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID;
		//基础值
		stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = CJewelryPropMgr::Instance()->GetExtAttrValue( m_stDT_JEWELRY_DATA.wJewelryID, m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID, 0);
		m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = CJewelryPropMgr::Instance()->GetExtAttrValue( m_stDT_JEWELRY_DATA.wJewelryID, m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID, 
			m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byLevel);
		//强化等级
		stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].byExtLevel = m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byLevel;
		//强化加成值
		stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].dwExtValue = m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue < stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue ? 0 : 
			m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue - stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue;
		//值类型
		stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].dwValueType = CJewelryPropMgr::Instance()->GetValueType(stJewelryCli.stExtAttrLst.astExtAttrInfo[byIdx].byKindID);
	}
	stJewelryCli.byStrengExtLevel = CSinglePramMgr::Instance()->GetJewelryStrengIntervalLevel();
	stJewelryCli.byExtStrengNum = GetExtStrengLeftNum();
}

DT_BATTLE_ATTR_LST_INFO& CJewelry::GetBaseAttribute()
{
	return m_stDT_JEWELRY_DATA.stBaseAttrLst;
}

SJewelryUpgradeProp* CJewelry::GetUpgradeProp()
{
	if ( IsMaxLevel() )
	{
		return NULL;
	}
	return CJewelryPropMgr::Instance()->GetUpgradeProp(m_stDT_JEWELRY_DATA.wJewelryLevel, m_stDT_JEWELRY_DATA.wStrengLevel);
}

UINT16 CJewelry::OnUpgrade(PKT_CLIGS_JEWELRY_STRENGTH_ACK& stAck)
{
	SJewelryUpgradeProp* poUpgradeProp = GetUpgradeProp();
	if ( NULL == poUpgradeProp)
	{
		return ERR_JEWELRY_STRENGTH::ID_IS_MAX_LEVEL;
	}

	if ( 0 == poUpgradeProp->dwUpgradeItemID)
	{
		return ERR_JEWELRY_STRENGTH::ID_IS_MAX_LEVEL;
	}

	CBag& oBag = m_poOwner->GetBag();
	CGoods* poGoods = oBag.GetGoodsItem( poUpgradeProp->dwUpgradeItemID);
	if ( NULL == poGoods)
	{
		GetStrengItemInfo( stAck.stStringth );
		return ERR_JEWELRY_STRENGTH::ID_ITEM_NOT_ENOUGH;//升级道具不足
	}

	if( poGoods->GetCurPileNum() < poUpgradeProp->wUpgradeNum )
	{
		GetStrengItemInfo( stAck.stStringth );
		return ERR_JEWELRY_STRENGTH::ID_ITEM_NOT_ENOUGH;//升级道具不足
	}

	stAck.stPowerInfo.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	
	if ( UpgradeJewelry())
	{
		poGoods->DecPileNum(poUpgradeProp->wUpgradeNum, CRecordMgr::EDIT_UPGRADE_JEWELRY, m_stDT_JEWELRY_DATA.wJewelryID, m_stDT_JEWELRY_DATA.wStrengLevel);
		
		if (0 == poGoods->GetCurPileNum())
		{
			m_poOwner->OnDeleteItem(poGoods);
		}
	}
	else
	{
		return ERR_JEWELRY_STRENGTH::ID_IS_MAX_LEVEL;
	}
	
	stAck.stPowerInfo.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	GetJewelryStrength(stAck.stAfterJewelryInfo, stAck.stStrengthJewelry);
	if ( !GetStrengItemInfo(stAck.stStringth))
	{
		stAck.byMaxMark = ESM_YES;
	}

	CJewelryPropMgr::Instance()->GetStrentItemInfo(m_poOwner, stAck.stJewelryStrengResSync);
	
	return ERR_JEWELRY_STRENGTH::ID_SUCCESS;
}

BOOL CJewelry::UpgradeJewelry()
{
	SJewelryUpgradeProp* poProp = GetUpgradeProp();
	if ( NULL == poProp){	//没有升级信息，满级
		return FALSE;
	}

    if ( 0 == poProp->dwUpgradeItemID || 0 == poProp->wUpgradeNum){
		return FALSE;
	}

    CHero *poHero = m_poOwner->GetHero(m_stDT_JEWELRY_DATA.wHeroKindID);
    if (NULL == poHero){
        return FALSE;
    }

	//强化等级+1
	m_stDT_JEWELRY_DATA.wStrengLevel += 1;
	UINT8 byBaseNum = m_stDT_JEWELRY_DATA.stBaseAttrLst.byAttrNum;
	UINT8 byIdx = 0;
	for ( byIdx = 0; byIdx < byBaseNum; byIdx++)
	{
		m_stDT_JEWELRY_DATA.stBaseAttrLst.astAttrInfo[byIdx].dwAttrValue = 
			CJewelryPropMgr::Instance()->GetBaseAttrValue(m_stDT_JEWELRY_DATA.wJewelryID, m_stDT_JEWELRY_DATA.stBaseAttrLst.astAttrInfo[byIdx].byAttrKind, m_stDT_JEWELRY_DATA.wStrengLevel);
	}

    // 精炼成功则重新检查饰品套装
    poHero->ReCheckJewelrySuit();
	m_poOwner->ResetBattleAttr(TRUE);
	return TRUE;
}

UINT8 CJewelry::ResolveJewelry(DT_GOODS_DATA_LST_CLI& stGoodsCli, UINT32& dwRes)
{
	if( 0 == m_poOwner->GetBag().GetIdleSize() )
	{
		return ECIR_FAILED_BAG_FULL;
	}
	//检查消耗
	SJewelryResolveProp* poResolveProp = CJewelryPropMgr::Instance()->GetResolveProp(m_stDT_JEWELRY_DATA.wJewelryLevel, GetColorType());
	if ( NULL == poResolveProp)
	{
		USR_INFO("[%s : %u] JewelryLevel = %u, JewelryColor = %u", m_stDT_JEWELRY_DATA.wJewelryLevel, GetColorType());
		return ECIR_FAILED_OTHER;
	}
	if (m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < poResolveProp->dwCostRes )
	{
		return ECIR_COIN_NOT_ENOUGH;
	}
	dwRes = poResolveProp->dwCostRes;
	UINT16 wNeedSpace = 0;
	UINT16 wBaseItemID = GetRandomIdxMustHit(poResolveProp->mapItemRandom);//基本值 
	UINT16 wBaseItemNum = poResolveProp->mapItemProp[wBaseItemID].wItemNum;

	C3232Map mapItem;
	mapItem[wBaseItemID] = wBaseItemNum;
	//查看背包空间
	CMapUpgradeItemNum* poMapUpgradeItemNum = CJewelryPropMgr::Instance()->GetResolveReturn(m_stDT_JEWELRY_DATA.wJewelryLevel, m_stDT_JEWELRY_DATA.wStrengLevel);
	if ( NULL != poMapUpgradeItemNum)
	{
		CBag& oBag = m_poOwner->GetBag();
		CMapUpgradeItemNumItr itr = poMapUpgradeItemNum->begin();
		for ( ; itr != poMapUpgradeItemNum->end(); itr++)
		{
			mapItem[itr->first] += (itr->second * CSinglePramMgr::Instance()->GetJewelryResolveReturn() / 100);
		}
	}
	UINT8 byItemNum = 0;
	CBag& oBag = m_poOwner->GetBag();
	for ( C3232MapItr itr = mapItem.begin(); itr != mapItem.end(); itr++)
	{
		DT_ITEM_DATA_CLI2 stItemCli = {0};
		CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->first, itr->second, stItemCli);
		stGoodsCli.astGoodsList[byItemNum] = stItemCli.stItemInfo.stGoods;
		byItemNum++;
		CGoods* poGoods = oBag.GetGoodsItem(itr->first);
		if ( NULL == poGoods)
		{
			wNeedSpace++;
		}
	}

	stGoodsCli.wGoodsNum = byItemNum;
	if ( oBag.GetIdleSize() < wNeedSpace)
	{
		return ECIR_FAILED_BAG_FULL;
	}
	
	return ECIR_SUCCESS;
}

BOOL CJewelry::GetJewelryStrength(DT_JEWELRY_DATA_CLI& stCurInfo, DT_JEWELRY_DATA_CLI& stStrengthInfo)
{
	//当前信息
	GetJewelryDataCli(stCurInfo);

	UINT8 byIdx = 0;
	stStrengthInfo = stCurInfo;
	//强化上限判断
	if ( !IsMaxLevel() )
	{
		//强化后的信息
		stStrengthInfo.wStrengLevel += 1;

		UINT8 byBaseNum = stStrengthInfo.stBaseAttrLst.byAttrNum;
		
		for ( byIdx = 0; byIdx < byBaseNum; byIdx++)
		{
			stStrengthInfo.stBaseAttrLst.astAttrInfo[byIdx].dwAttrValue = CJewelryPropMgr::Instance()->GetBaseAttrValue( m_stDT_JEWELRY_DATA.wJewelryID, 
				stStrengthInfo.stBaseAttrLst.astAttrInfo[byIdx].byAttrKind, stStrengthInfo.wStrengLevel);
		}
	}
	
	if (0 != GetExtStrengLeftNum())
	{
		//附加属性强化后的信息
		UINT8 byExtNum = stStrengthInfo.stExtAttrLst.byExtAttrNum;
		for ( byIdx = 0; byIdx < byExtNum; byIdx++)
		{
			if ( m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byLevel < CSinglePramMgr::Instance()->GetMaxJewelryStrengNum()    )
			{
				stStrengthInfo.stExtAttrLst.astExtAttrInfo[byIdx].byExtLevel += 1;
				stStrengthInfo.stExtAttrLst.astExtAttrInfo[byIdx].byKindID = m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID;
				UINT32 dwExtBaseValue = CJewelryPropMgr::Instance()->GetExtAttrValue( m_stDT_JEWELRY_DATA.wJewelryID, m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID, 0);
				UINT32 dwExtSumValue = CJewelryPropMgr::Instance()->GetExtAttrValue( m_stDT_JEWELRY_DATA.wJewelryID, m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID, stStrengthInfo.stExtAttrLst.astExtAttrInfo[byIdx].byExtLevel);
				stStrengthInfo.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = dwExtBaseValue;
				stStrengthInfo.stExtAttrLst.astExtAttrInfo[byIdx].dwExtValue = dwExtSumValue < dwExtBaseValue ? 0 : dwExtSumValue - dwExtBaseValue;
			}
		}
	}
	

	
	return TRUE;
}

BOOL CJewelry::GetStrengItemInfo( DT_STRENGTH_INFO& stItemInfo)
{
	//强化物品信息
	SJewelryUpgradeProp* poUpgradeProp = GetUpgradeProp();
	if (NULL != poUpgradeProp)
	{
		stItemInfo.wItemID = poUpgradeProp->dwUpgradeItemID;
		stItemInfo.dwCostItemNum = poUpgradeProp->wUpgradeNum;
		CBag& oBag = m_poOwner->GetBag();
		CGoods* poGoods = oBag.GetGoodsItem(stItemInfo.wItemID);
		if ( NULL == poGoods)
		{
			stItemInfo.dwCurItemNum = 0;
		}
		else
		{
			stItemInfo.dwCurItemNum = poGoods->GetCurPileNum();
		}
		SGoodsProp* poGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stItemInfo.wItemID);
		if ( NULL != poGoodsProp)
		{
			if ( 0 != poGoodsProp->dwBuyCoin)
			{
				stItemInfo.stCostRes.byResKind = ACTE_COIN;
				stItemInfo.stCostRes.qwResValue = poGoodsProp->dwBuyCoin;
			}
			else if ( 0!= poGoodsProp->dwBuyGold)
			{
				stItemInfo.stCostRes.byResKind = ACTE_GOLD;
				stItemInfo.stCostRes.qwResValue = poGoodsProp->dwBuyGold;
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

UINT8 CJewelry::GetColorType()	//获取饰品品质
{
	SJewelryProp* poProp = GetProp();
	if ( NULL == poProp)
	{
		return EC_BLUE;
	}
	return poProp->byColorType;
}

UINT16 CJewelry::GetStrengLevel()	//获取强化等级
{
	return m_stDT_JEWELRY_DATA.wStrengLevel;
}

UINT16 CJewelry::GetDressLevel()
{
	return m_stDT_JEWELRY_DATA.wJewelryLevel;
}

UINT8 CJewelry::GetExtStrengLeftNum()
{
	UINT8 byIntervalNum = CSinglePramMgr::Instance()->GetJewelryStrengIntervalLevel();
	UINT8 byMaxStrengNum = CSinglePramMgr::Instance()->GetMaxJewelryStrengNum();
	UINT8 byCanNum = m_stDT_JEWELRY_DATA.wStrengLevel / byIntervalNum;
	UINT8 byExtNum = m_stDT_JEWELRY_DATA.stExtAttrLst.byExtAttrNum;

	if ( byMaxStrengNum * byExtNum < byCanNum)
	{
		byCanNum = byMaxStrengNum * byExtNum;
	}
	
	UINT8 byAlreadNum = 0;//已经强化次数
	for ( UINT8 byIdx = 0; byIdx < byExtNum; byIdx++)
	{
		byAlreadNum += m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byLevel;
	}

	if ( byCanNum < byAlreadNum)
	{
		return 0;
	}
	return byCanNum - byAlreadNum;
}

BOOL CJewelry::IsCanUpgrade()
{
	if( IsMaxLevel() )
	{
		return FALSE;
	}
	SJewelryUpgradeProp* poJewelryUpgradeProp = CJewelryPropMgr::Instance()->GetUpgradeItemProp( m_stDT_JEWELRY_DATA.wJewelryLevel, m_stDT_JEWELRY_DATA.wStrengLevel);
	if ( NULL != poJewelryUpgradeProp)
	{
		UINT32 dwUpgradeItemID = poJewelryUpgradeProp->dwUpgradeItemID;
		CGoods* poGoods = m_poOwner->GetBag().GetGoodsItem(dwUpgradeItemID);
		if ( NULL != poGoods)
		{
			if( poJewelryUpgradeProp->wUpgradeNum < poGoods->GetCurPileNum() )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CJewelry::IsMaxLevel()
{
	SJewelryProp* poProp = GetProp();
	if ( NULL == poProp)
	{
		return TRUE;
	}
	UINT8 byStrengIntervalLevel = CSinglePramMgr::Instance()->GetJewelryStrengIntervalLevel();
	UINT8 byMaxStrengNum = CSinglePramMgr::Instance()->GetMaxJewelryStrengNum();
	//强化上限判断
	if ( byStrengIntervalLevel * byMaxStrengNum * poProp->byColorType <= m_stDT_JEWELRY_DATA.wStrengLevel )
	{
		return TRUE;
	}
	return FALSE;
}


UINT16 CJewelry::OnStrengExt(UINT8 byExtID, PKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK& stAck)
{
	UINT8 byLeftStrengNum = GetExtStrengLeftNum();
	if ( 0 == byLeftStrengNum )
	{
		return ERR_JEWELRY_EXT_UPGRADE::ID_NOT_STRENG_NUM;
	}
	UINT8 byExtNum = m_stDT_JEWELRY_DATA.stExtAttrLst.byExtAttrNum;
	BOOL bStreng = FALSE;
	for ( UINT8 byIdx = 0; byIdx < byExtNum; byIdx++)
	{
		if ( byExtID == m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID)
		{
			bStreng = TRUE;
			if ( m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byLevel < CSinglePramMgr::Instance()->GetJewelryStrengIntervalLevel())
			{
				m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byLevel += 1;
				m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].dwKindValue = CJewelryPropMgr::Instance()->GetExtAttrValue( m_stDT_JEWELRY_DATA.wJewelryID, m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byKindID, 
					m_stDT_JEWELRY_DATA.stExtAttrLst.astExtAttrInfo[byIdx].byLevel);
			}
			else//达到强化上限
			{
				return ERR_JEWELRY_EXT_UPGRADE::ID_IS_MAX;
			}
		}
	}

	if ( !bStreng )
	{

		return ERR_JEWELRY_EXT_UPGRADE::ID_EXT_ID_NOT_FIND;
	}

	GetJewelryStrength(stAck.stJewelryInfo, stAck.stStrengthJewelry);
	
	
	if ( GetStrengItemInfo( stAck.stStrengInfo))
	{
		stAck.byMaxMark = ESM_YES;
	}
	
	stAck.stPowerInfo.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	m_poOwner->ResetBattleAttr( TRUE );
	stAck.stPowerInfo.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	return ERR_JEWELRY_EXT_UPGRADE::ID_SUCCESS;
}

UINT16 CJewelry::OnOpenJewelryResolve(PKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK& stAck)
{
	//检查消耗
	SJewelryResolveProp* poResolveProp = CJewelryPropMgr::Instance()->GetResolveProp(m_stDT_JEWELRY_DATA.wJewelryLevel, GetColorType());
	if ( NULL == poResolveProp)
	{
		return ERR_OPEN_JEWELRY_RESOLVE::ID_JEWELRY_INFO_EXCEPTION;
	}
	stAck.stCostRes.byResKind = ACTE_COIN;
	stAck.stCostRes.qwResValue = poResolveProp->dwCostRes;

	CMapJewelryResolveItemPropItr itrBase = poResolveProp->mapItemProp.begin();
	for ( ; itrBase != poResolveProp->mapItemProp.end(); itrBase++)
	{
		if ( 0 != itrBase->second.wItemProbility )
		{
			stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].wItemID = itrBase->second.wItemID;
			stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].dwCostItemNum = itrBase->second.wItemNum;
			SGoodsProp* pstGoodsProp =  CGoodsPropMgr::Instance()->GetPropByPrimaryKey(itrBase->second.wItemID);
			if ( NULL != pstGoodsProp)
			{
				if ( 0 != pstGoodsProp->dwBuyCoin)
				{
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.byResKind = ACTE_COIN;
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.qwResValue = pstGoodsProp->dwBuyCoin;
				}
				else if ( 0 != pstGoodsProp->dwBuyGold)
				{
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.byResKind = ACTE_GOLD;
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.qwResValue = pstGoodsProp->dwBuyGold;
				}
			}
			stAck.stBaseItem.byItemKindNum++;
		}
	}
	

	//
	CMapUpgradeItemNum* poMapUpgradeItemNum = CJewelryPropMgr::Instance()->GetResolveReturn(m_stDT_JEWELRY_DATA.wJewelryLevel, m_stDT_JEWELRY_DATA.wStrengLevel);
	if ( NULL != poMapUpgradeItemNum)
	{
		CMapUpgradeItemNumItr itr = poMapUpgradeItemNum->begin();
		for ( ; itr != poMapUpgradeItemNum->end(); itr++)
		{
			stAck.stReturnItem.astItemInfo[stAck.stReturnItem.byItemKindNum].wItemID = itr->first;
			stAck.stReturnItem.astItemInfo[stAck.stReturnItem.byItemKindNum].dwCostItemNum = itr->second * CSinglePramMgr::Instance()->GetJewelryResolveReturn() / 100;
			SGoodsProp* pstGoodsProp =  CGoodsPropMgr::Instance()->GetPropByPrimaryKey(itr->first);
			if ( NULL != pstGoodsProp)
			{
				if ( 0 != pstGoodsProp->dwBuyCoin)
				{
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.byResKind = ACTE_COIN;
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.qwResValue = pstGoodsProp->dwBuyCoin;
				}
				else if ( 0 != pstGoodsProp->dwBuyGold)
				{
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.byResKind = ACTE_GOLD;
					stAck.stBaseItem.astItemInfo[stAck.stBaseItem.byItemKindNum].stCostRes.qwResValue = pstGoodsProp->dwBuyGold;
				}
			}
			stAck.stReturnItem.byItemKindNum++;
		}
	}

	
	
	return ERR_OPEN_JEWELRY_RESOLVE::ID_SUCCESS;
}

BOOL CJewelry::UnDressBag()
{
	CHero* poHero = m_poHero;
	if( NULL != m_poHero)
	{
		m_poHero->UnDressJewelry(this);
	}

	if( !m_poOwner->GetBag().AddItem(this, FALSE))
	{
		if ( NULL != poHero)
		{
			poHero->DressJewelry(this);
			m_poOwner->GetBag().DelItem(this, FALSE);
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CJewelry::DressHero(CHero* poHero)
{
	if ( NULL == poHero)
	{
		return FALSE;
	}
	SJewelryProp* poProp = GetProp();
	if ( NULL == poProp)
	{
		return FALSE;
	}
	//先脱
	if (!UnDressBag())
	{
		return TRUE;
	}
	//如果位置上有物品也脱了
	CJewelry* poJewelry = poHero->GetJewelryByPos( poProp->byPosKindID);
	if ( NULL != poJewelry)
	{
		poHero->UnDressJewelry(poJewelry);
	}
	poHero->DressJewelry(this);
	m_poOwner->GetBag().DelItem(this, FALSE);
	if ( NULL != poJewelry)
	{
		m_poOwner->GetBag().AddItem(poJewelry, FALSE);
	}
	return TRUE;
}

BOOL CJewelry::IsOneHeroAttr(JewelryExtAttr eAttrType)
{
    if (eAttrType >= EJEA_MAX){
        return FALSE;
    }

    static std::vector<BOOL> s_vecAttr;
    if (s_vecAttr.empty()){
        JewelryExtAttr arrOneHeroAttrType[] = {
            EJEA_ARMS,//武器
            EJEA_CLOTHING,//衣服
            EJEA_HELM,//头盔
            EJEA_SHOE,//鞋子
            EJEA_ALL_EQUIP,//所有装备
            EJEA_NORMAL_ATTACK,//普通攻击
            EJEA_SKILL_ATTACK,//技能攻击
            EJEA_WEAPON_ATTACK,//神器攻击
            EJEA_WEAPON_DEFEND,//神器防御
            EJEA_INSTANCE_EXPERIENCE,//副本经验
            EJEA_FIRST_STUDY,//第一卦象
            EJEA_SECOND_STUDY,//第二卦象
            EJEA_THIRD_STUDY,//第三卦象
            EJEA_FOURTH_STUDY,//第四卦象
            EJEA_ALL_STUDY,//所有卦象
            EJEA_HP,//生命
            EJEA_ATTACK,//攻击
            EJEA_DEFENG,//防守+XXXX
            EJEA_HITRATE,//破击率+XXXX
            EJEA_DODGERATE,//格挡率+XXXX
            EJEA_CRITRATE,//暴击率+XXXX
            EJEA_DECRITRATE//抗暴率+XXXX
        };

        UINT16 wArrCnt = sizeof(arrOneHeroAttrType) / sizeof(JewelryExtAttr);

        s_vecAttr.resize(EJEA_MAX, FALSE);
        for(UINT16 i = 0; i < wArrCnt; i++){
            s_vecAttr[arrOneHeroAttrType[i]] = TRUE;
        }
    }

    return s_vecAttr[eAttrType];
}

BOOL CJewelry::IsAllHeroAttr(JewelryExtAttr eAttrType)
{
    if (eAttrType >= EJEA_MAX){
        return FALSE;
    }

    static std::vector<BOOL> s_vecAttr;
    if (s_vecAttr.empty()){
        JewelryExtAttr arrAllHeroAttrType[] = {
            EJEA_ALL_NORMAL_ATTACK,
            EJEA_ALL_SKILL_ATTACK,
            EJEA_ALL_WEAPON_ATTACK,
            EJEA_ALL_WEAPON_DEFEND,
            EJEA_INSTANCE_STORY,
            EJEA_INSTANCE_COIN,
            EJEA_ALL_SCIENCE_LEVEL
        };

        UINT16 wArrCnt = sizeof(arrAllHeroAttrType) / sizeof(JewelryExtAttr);

        s_vecAttr.resize(EJEA_MAX, FALSE);
        for(UINT16 i = 0; i < wArrCnt; i++){
            s_vecAttr[arrAllHeroAttrType[i]] = TRUE;
        }
    }
    
    return s_vecAttr[eAttrType];
}
