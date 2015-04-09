#include "player.h"
#include "sdrandom.h"
#include <logic/item/equip/strengthenpropmgr.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <logic/other/singleprammgr.h>
#include <logic/item/itemcomposepropmgr.h>
#include <framework/gsapi.h>
#include <logic/record/recordmgr.h>
#include <logic/base/basedefine.h>
#include <logic/talent/drugpropmgr.h>
#include <logic/talent/drugupgradepropmgr.h>
#include "logic/hero/hero.h"
#include "logic/build/build.h"
#include "logic/other/fewparam.h"
#include "logic/lua/luamgr.h"
#include "logic/item/jewelry/jewelrypropmgr.h"
#include "logic/item/jewelry/jewelry.h"

//获取所有空闲装备
VOID CPlayer::GetIdleEquip(DT_EQUIP_DATA_LST_CLI& stEquipList)
{
    //玩家背包中空闲装备
    m_oBag.GetDT_EQUIP_DATA_LST_CLI(stEquipList);

}

VOID CPlayer::GetAllEquip(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stEquipList)
{
    CHero* poHero = m_mapKind2Hero.GetFistData();
    while(poHero)
    {
        poHero->GetEquipStrengthenCompose(stEquipList);
        poHero = m_mapKind2Hero.GetNextData();
    }

    //玩家背包中空闲装备
    m_oBag.GetDT_EQUIP_DATA_LST_STRENGTHEN_COMPOSE_CLI(stEquipList);
}

VOID CPlayer::GetAllEquip(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stEquipList, UINT32 adwStrengthen10CoinInfo[MAX_BAG_OPEN_NUM])
{
    GetAllEquip(stEquipList);
    UINT16 wEquipNum = stEquipList.wEquipNum;
    UINT16 wPlayerLevel = GetLevel();
    for(UINT16 wIdx = 0; wIdx < wEquipNum; wIdx++)
    {
        DT_EQUIP_DATA_CLI& stEquipInfo = stEquipList.astEquipInfo[wIdx];
        adwStrengthen10CoinInfo[wIdx] = CEquip::GetUpLevelCoin(stEquipInfo.wLevel, 10, stEquipInfo.wDressLevel, wPlayerLevel);
    }
}

void CPlayer::GetAllEquip_v410(DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI &stEquipList)
{
    CHero* poHero = m_mapKind2Hero.GetFistData();
    while(poHero)
    {
        poHero->GetEquipsStrengthenEnchantInfo_v410(stEquipList);
        poHero = m_mapKind2Hero.GetNextData();
    }

    //玩家背包中空闲装备
    m_oBag.GetEquipStrengthenEnchantCli(stEquipList);

    UINT16 wPlayerLevel = GetLevel();
    for(UINT16 wIdx = 0; wIdx < stEquipList.wEquipNum && wIdx < MAX_BAG_OPEN_NUM; wIdx++)
    {
        DT_EQUIP_DATA_CLI& stEquipInfo = stEquipList.astEquipInfo[wIdx].stEquipData;
        stEquipList.adwStrengthen10CoinInfo[wIdx] = CEquip::GetUpLevelCoin(stEquipInfo.wLevel, 10, stEquipInfo.wDressLevel, wPlayerLevel);
    }
}

//获取装备的最大镶嵌宝石个数。
UINT8 CPlayer::GetMaxMosaicGemNum()
{
    UINT8 byMosaicGemNum = 0;
    UINT8 byMaxMosaicGemNum = 0;
    CHero* poHero = m_mapKind2Hero.GetFistData();
    while(poHero)
    {
        byMosaicGemNum = poHero->GetMaxMosaicGemNum();
        if (byMosaicGemNum > byMaxMosaicGemNum)
        {
            byMaxMosaicGemNum = byMosaicGemNum;
        }
        poHero = m_mapKind2Hero.GetNextData();
    }

    //玩家背包中空闲装备
    byMosaicGemNum = m_oBag.GetMaxMosaicGemNum();
    if (byMosaicGemNum > byMaxMosaicGemNum)
    {
        byMaxMosaicGemNum = byMosaicGemNum;
    }
    return byMaxMosaicGemNum;
}


UINT16 CPlayer::GetNextEquipIdx(UINT16 wItemKindID)
{
    CItemKind2IdxMapItr itr = m_mapEquipKind2Idx.find(wItemKindID);
    if(itr != m_mapEquipKind2Idx.end())
    {
        return ++(itr->second);
    }
    else
    {
        m_mapEquipKind2Idx[wItemKindID] = 1;
        return 1;
    }

    return 1;
}

UINT16 CPlayer::CkNextEquipIdx(UINT16 wItemKindID)
{
    CItemKind2IdxMapItr itr = m_mapEquipKind2Idx.find(wItemKindID);
    if(itr != m_mapEquipKind2Idx.end())
    {
        return itr->second + 1;
    }

    return 1;
}

//创建道具失败后，归还idx
VOID CPlayer::ReturnNextEquipIdx(UINT16 wItemKindID, UINT16 wIdx)
{
    CItemKind2IdxMapItr itr = m_mapEquipKind2Idx.find(wItemKindID);
    if(itr != m_mapEquipKind2Idx.end())
    {
        if((itr->second > 0) || (itr->second = wIdx))
        {
            --itr->second;
        }
    }
}


//初始化背包
BOOL CPlayer::InitBag(UINT8 byBagOpenNum)
{
    if(FALSE == m_oBag.Init(byBagOpenNum, this))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: bag init failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

//初始化背包装备道具
BOOL CPlayer::InitBagEquipItem(DT_BAG_EQUIP_DATA_LST& stBagEquipData)
{
    UINT16 wEquipNum = stBagEquipData.wEquipNum;
    for(UINT16 wIdx = 0; wIdx < wEquipNum && wIdx < MAX_BAG_OPEN_NUM_NEW; wIdx++)
    {
        DT_EQUIP_DATA* pstDT_EQUIP_DATA = &(stBagEquipData.astEquipList[wIdx]);

        SEquipProperty stEquipProperty;
        stEquipProperty.pstEquip = pstDT_EQUIP_DATA;
        stEquipProperty.wEnchantLevel = stBagEquipData.awEnchantLevel[wIdx];

        CItem *poItem = CItemMgr::Instance()->AttachItem(this, pstDT_EQUIP_DATA->wKindID, &stEquipProperty);
        if(NULL == poItem)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: AttachItem failed!playerid:%d."), MSG_MARK, m_dwPlayerID);
            //跳过有问题数据
            continue;
        }

        CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
        if(NULL == poEquip)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: equip init failed!playerid:%d."), MSG_MARK, m_dwPlayerID);
            //跳过有问题数据
            continue;
        }
        if(!m_oBag.AddItem(poEquip))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: add item failed!playerid:%d."), MSG_MARK, m_dwPlayerID);
            CItemMgr::Instance()->RemoveItem(poEquip);
            //跳过有问题数据
            continue;
        }
    }

    return TRUE;
}
//
//VOID CPlayer::SetStrengthenPromptEquip(CEquip* poEquip)
//{
//	if(NULL == m_poMinStrengthenLevelEquip)
//	{
//		m_poMinStrengthenLevelEquip = poEquip;
//	}
//	else
//	{
//		if(poEquip->GetDT_EQUIP_DATA().wLevel < m_poMinStrengthenLevelEquip->GetDT_EQUIP_DATA().wLevel)
//		{
//			m_poMinStrengthenLevelEquip = poEquip;
//		}
//	}
//}

//初始化背包物品道具
BOOL CPlayer::InitBagGoodsItem(DT_BAG_GOODS_DATA_LST& stBagGoodsData)
{
    UINT16 wGoodsNum = stBagGoodsData.wGoodsNum;
    for(UINT16 wIdx = 0; wIdx < wGoodsNum && wIdx < MAX_BAG_OPEN_NUM_NEW; wIdx++)
    {
        DT_GOODS_DATA* pstDT_GOODS_DATA = &(stBagGoodsData.astGoodsList[wIdx]);
        CGoods* poGoods = dynamic_cast<CGoods*>(CItemMgr::Instance()->AttachItem(this, pstDT_GOODS_DATA->wKindID, pstDT_GOODS_DATA));
        if(NULL == poGoods)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: goods[%d] init failed!playerid:%d."), MSG_MARK, pstDT_GOODS_DATA->wKindID, m_dwPlayerID);
            //跳过有问题数据
            continue;
        }
        if(!m_oBag.AddItem(poGoods))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: add item failed!playerid:%d."), MSG_MARK, m_dwPlayerID);
            CItemMgr::Instance()->RemoveItem(poGoods);
            //跳过有问题数据
            continue;
        }
    }

    return TRUE;
}


CEquip* CPlayer::GetEquip(UINT16 wEquipKindID, UINT16 wEquipIdx)
{
    return m_mapAllEquip.GetData(CLogicFunc::Get1616Key(wEquipKindID, wEquipIdx));
}

VOID CPlayer::OnDeleteJewelry(CJewelry* poJewelry)
{
	if(NULL == poJewelry)
	{
		return;
	}

	m_mapAllJewelry.DelData(CLogicFunc::Get1616Key(poJewelry->GetDT_JEWELRY_DATA().wJewelryID, poJewelry->GetDT_JEWELRY_DATA().wIdx));

	CHero* poHero = poJewelry->GetHero();
	if(poHero)
	{
		poHero->OnDeleteJewelry(poJewelry);
	}

	OnDeleteItem(poJewelry);
}

//清除装备回调
VOID CPlayer::OnDeleteEquip(CEquip* poEquip)
{
    if(NULL == poEquip)
    {
        return;
    }

    m_mapAllEquip.DelData(CLogicFunc::Get1616Key(poEquip->GetDT_EQUIP_DATA().wKindID, poEquip->GetDT_EQUIP_DATA().wIdx));

    CHero* poHero = poEquip->GetHero();
    if(poHero)
    {
        poHero->OnDeleteEquip(poEquip);
    }

    //ResetStrenthenUpgradeEquipPrompt(poEquip);

    poEquip->GetDT_EQUIP_DATA().wHeroKindID = 0;
    OnDeleteItem(poEquip);
}
//
//VOID CPlayer::ResetStrenthenUpgradeEquipPrompt(CEquip* poEquip)
//{
//	BOOL bNeedSetMinStrengthenLevel = ((poEquip == m_poMinStrengthenLevelEquip) || (NULL == poEquip)) ? TRUE : FALSE;
//	if(!bNeedSetMinStrengthenLevel)
//	{
//		return;
//	}
//	m_poMinStrengthenLevelEquip = bNeedSetMinStrengthenLevel ? NULL : m_poMinStrengthenLevelEquip;
//
//
//	CHero* poHero = m_mapKind2HeroAllCarry.GetFistData();
//	while(poHero)
//	{
//		CEquipMap&	m_mapHeroEquip = poHero->GetEquipMap();
//		CEquip* poEquipTmp = m_mapHeroEquip.GetFistData();
//		while(poEquipTmp)
//		{
//			if(NULL == m_poMinStrengthenLevelEquip)
//			{
//				m_poMinStrengthenLevelEquip = poEquipTmp;
//			}
//			else
//			{
//				if(poEquipTmp->GetDT_EQUIP_DATA().wLevel < m_poMinStrengthenLevelEquip->GetDT_EQUIP_DATA().wLevel)
//				{
//					m_poMinStrengthenLevelEquip = poEquip;
//				}
//			}
//
//			poEquipTmp = m_mapHeroEquip.GetNextData();
//		}
//		poHero = m_mapKind2HeroAllCarry.GetNextData();
//	}
//}

//新增装备回调
VOID CPlayer::OnAddEquip(CEquip* poEquip, BOOL bHaveDress)
{
    if(NULL == poEquip)
    {
        return;
    }

    m_mapAllEquip.AddData(CLogicFunc::Get1616Key(poEquip->GetDT_EQUIP_DATA().wKindID, poEquip->GetDT_EQUIP_DATA().wIdx), poEquip);

    if(0 == poEquip->GetDT_EQUIP_DATA().wHeroKindID)
    {
        if(!m_oBag.AddItem(poEquip))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CPlayer::OnAddEquip add equip failed! playerid:%u, usedsize = %u, idlesize = %u"), MSG_MARK, 
                GetID(), m_oBag.GetUsedSize(), m_oBag.GetIdleSize());
        }
    }

    if(!bHaveDress)
    {
        m_vecNewEquip.push_back(poEquip);
    }
}

VOID CPlayer::OnDeleteItem(CItem* poItem)
{
    if(NULL == poItem)
    {
        return;
    }

    m_oBag.DelItem(poItem, TRUE);

    CItemMgr::Instance()->RemoveItem(poItem);

}

VOID CPlayer::ReduceItem( CGoods *poItem, CRecordMgr::EDecItemType emOpType )
{
    if( NULL == poItem )
    {
        return;
    }

    if( 0 == poItem->GetCurPileNum() )
    {
        OnDeleteItem( poItem );
        return;
    }

    //poItem->GetDT_GOODS_DATA().wPileCount--;

    //CRecordMgr::Instance()->RecordInfo( GetID(), ERM_DECITEM, wOpType, poItem->GetItemKindID(), SDTimeToString(SDNow()), poItem->GetCurPileNum(),
    //	GetLevel(), m_stDT_PLAYER_BASE_DATA.byVipLevel, 1);

    //扣除操作
    poItem->DecPileNum(1, emOpType);

    if( 0 == poItem->GetCurPileNum() )
    {
        OnDeleteItem( poItem );
        return;
    }
}


UINT8  CPlayer::GetBagFullFlag()
{
    return m_oBag.GetIdleSize() >= 1 ? 0 : 1;
}

//获取强化CD倒计时
UINT32 CPlayer::GetStrenthenCountDown()
{
    if (NULL == m_poFewParam)
    {
        return 0;
    }

    DT_FEW_PARAM_DATA& stDT_FEW_PARAM_DATA = m_poFewParam->GetDT_FEW_PARAM_DATA();
    UINT64 qwPassTime = SDTimeSecs() - stDT_FEW_PARAM_DATA.qwLastStrengthenTime;
    if(qwPassTime >= (UINT64)stDT_FEW_PARAM_DATA.wLastStrengthenCD)
    {
        return 0;
    }
    return (UINT32)((UINT64)stDT_FEW_PARAM_DATA.wLastStrengthenCD - qwPassTime);
}


UINT16 CPlayer::SmithyBuyEquip(UINT16 wHeroKindID, UINT8 byEquipPos, DT_EQUIP_DATA_STRENGTHEN_CLI& stEquipStrengthenInfo, DT_EQUIP_DATA_CLI& stEquipInfo, UINT32& dwStrengthen10Coin)
{
    if((byEquipPos < EEPT_HEAD) ||  (byEquipPos > EEPT_FOOT))
    {
        return ERR_SMITHY_BUY_EQUIP::ID_INVALID_EQUIP_POS;
    }

    CHero* poHero = m_mapKind2Hero.GetData(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_SMITHY_BUY_EQUIP::ID_HERO_NOT_EXIT;
    }

    CEquip* poEquip = poHero->GetEquipByPos(byEquipPos);
    if(poEquip)
    {
        return ERR_SMITHY_BUY_EQUIP::ID_HAVE_EQUIP;
    }

    DT_BUY_EQUIP_DATA stDT_BUY_EQUIP_DATA;
    CBuildPropMgr::Instance()->GetDT_BUY_EQUIP_DATA(byEquipPos, stDT_BUY_EQUIP_DATA);
    if(m_stDT_PLAYER_BASE_DATA.qwCoin < stDT_BUY_EQUIP_DATA.dwBuyCoin)
    {
        return ERR_COMMON::ID_COIN_NOT_ENOUGH;
    }

    poEquip = DressSmithyBuyEquip(poHero, stDT_BUY_EQUIP_DATA.wEquipID);
    if(NULL == poEquip)
    {
        RETURN_OTHER_ERR;
    }

    m_stDT_PLAYER_BASE_DATA.qwCoin -= stDT_BUY_EQUIP_DATA.dwBuyCoin;

    poEquip->GetDT_EQUIP_DATA_CLI(stEquipInfo);
    poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(stEquipStrengthenInfo);
    dwStrengthen10Coin = poEquip->GetUpLevelCoin(10);

    return ERR_SMITHY_BUY_EQUIP::ID_SUCCESS;
}


CEquip* CPlayer::DressSmithyBuyEquip(CHero* poHero, UINT16 wEquipID)
{
    if(NULL == poHero)
    {
        return NULL;
    }

    DT_EQUIP_DATA stDT_EQUIP_DATA;
    memset(&stDT_EQUIP_DATA, 0x00, sizeof(stDT_EQUIP_DATA));
    stDT_EQUIP_DATA.wKindID = wEquipID;
    stDT_EQUIP_DATA.wIdx = GetNextEquipIdx(wEquipID);
    stDT_EQUIP_DATA.wHeroKindID = poHero->GetDT_HERO_BASE_DATA().wKindID;
    stDT_EQUIP_DATA.wLevel = 0;//默认0
    stDT_EQUIP_DATA.wBagPos = 0;
    stDT_EQUIP_DATA.qwItemNewTime = SDTimeSecs();

    SEquipProperty stEquipProperty;
    stEquipProperty.pstEquip = &stDT_EQUIP_DATA;
    stEquipProperty.wEnchantLevel = 0;

    CEquip* poEquip = dynamic_cast<CEquip*>(CItemMgr::Instance()->AttachItem(this, stDT_EQUIP_DATA.wKindID, &stEquipProperty));
    if(NULL == poEquip)
    {
        ReturnNextEquipIdx(wEquipID, stDT_EQUIP_DATA.wIdx);
        return NULL;
    }
    //SetStrengthenPromptEquip(poEquip);

    //穿上新装备
    poHero->DressMakeEquip(poEquip);

    m_mapAllEquip.AddData(CLogicFunc::Get1616Key(poEquip->GetDT_EQUIP_DATA().wKindID, poEquip->GetDT_EQUIP_DATA().wIdx), poEquip);

    return poEquip;
}

UINT16 CPlayer::ClrStrengthenCD()
{
    if (NULL == m_poFewParam)
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    UINT32 dwGostGold = CEquipStrengthenPropMgr::Instance()->GetClrStrengthenCDGold();
    if(m_stDT_PLAYER_BASE_DATA.qwGold < dwGostGold)
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    m_poFewParam->GetDT_FEW_PARAM_DATA().wLastStrengthenCD = 0;

    m_stDT_PLAYER_BASE_DATA.qwGold -= dwGostGold;

    return ERR_CLR_STRENGTHEN_CD::ID_SUCCESS;
}

UINT16 CPlayer::GetNextJeweryIdx(UINT16 wItemKindID)
{
	CItemKind2IdxMapItr itr = m_mapEquipKind2Idx.find(wItemKindID);
	if(itr != m_mapEquipKind2Idx.end())
	{
		return ++(itr->second);
	}
	else
	{
		m_mapEquipKind2Idx[wItemKindID] = 1;
		return 1;
	}

	return 1;
}

BOOL CPlayer::InitBagJewelryItem(DT_BAG_JEWELRY_DATA_LST& stBagJewelryData)
{
	UINT16 wJewelryNum = stBagJewelryData.wJewelryNum;
	for(UINT16 wIdx = 0; wIdx < wJewelryNum && wIdx < MAX_BAG_OPEN_NUM_NEW; wIdx++)
	{
		DT_JEWELRY_DATA& stDT_JEWELRY_DATA = stBagJewelryData.astJewelryInfo[wIdx];
		CJewelry* poJewelry = dynamic_cast<CJewelry*>(CItemMgr::Instance()->AttachItem(this, stDT_JEWELRY_DATA.wJewelryID, &stDT_JEWELRY_DATA));
		if(NULL == poJewelry)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: goods[%d] init failed!playerid:%d."), MSG_MARK, stDT_JEWELRY_DATA.wJewelryID, m_dwPlayerID);
			//跳过有问题数据
			continue;
		}
		if(!m_oBag.AddItem(poJewelry))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: add item failed!playerid:%d."), MSG_MARK, m_dwPlayerID);
			CItemMgr::Instance()->RemoveItem(poJewelry);
			//跳过有问题数据
			continue;
		}
	}
	return TRUE;
}

CJewelry* CPlayer::GetJewelry(UINT16 wJewelryID, UINT16 wIdx)
{
	return m_mapAllJewelry.GetData( CLogicFunc::Get1616Key(wJewelryID, wIdx));
}
UINT8 CPlayer::OnJewelryGenerateCheck(vector<UINT32>& vecKey, UINT16& wDressLevel, UINT8& byColorType, DT_RES_DATA& stRes)
{
	vector<CJewelry*> vecJewelry;
	set<CJewelry*> setJewelry;
	for ( vector<UINT32>::iterator itr = vecKey.begin(); itr != vecKey.end(); itr++)
	{
		CJewelry* poJewelry = m_mapAllJewelry.GetData(*itr);
		if ( NULL == poJewelry)
		{
			continue;
		}
		if ( setJewelry.end() != setJewelry.find(poJewelry))
		{
			continue;
		}
		if( 0 != poJewelry->GetDT_JEWELRY_DATA().wStrengLevel)
		{
			return EJS_ALREAD_STRENG;
		}
		setJewelry.insert(poJewelry);
		vecJewelry.push_back(poJewelry);
	}
	if ( 4 != vecJewelry.size())
	{
		return EJS_JEWELRY_IS_NULL;
	}

	wDressLevel = CLuamgr::Instance()->GenerateJewelryLevel( 
		vecJewelry[0]->GetDT_JEWELRY_DATA().wJewelryLevel, 
		vecJewelry[1]->GetDT_JEWELRY_DATA().wJewelryLevel,
		vecJewelry[2]->GetDT_JEWELRY_DATA().wJewelryLevel,
		vecJewelry[3]->GetDT_JEWELRY_DATA().wJewelryLevel);

	byColorType = CLuamgr::Instance()->GenerateJewelryColor( 
		(NULL == vecJewelry[0]->GetProp() ? EC_BLUE : vecJewelry[0]->GetProp()->byColorType) ,
		(NULL == vecJewelry[1]->GetProp() ? EC_BLUE : vecJewelry[1]->GetProp()->byColorType),
		(NULL == vecJewelry[2]->GetProp() ? EC_BLUE : vecJewelry[2]->GetProp()->byColorType),
		(NULL == vecJewelry[3]->GetProp() ? EC_BLUE : vecJewelry[3]->GetProp()->byColorType));

	stRes.byResKind = ACTE_COIN;
	stRes.qwResValue = CLuamgr::Instance()->GenerateJewelryCost(wDressLevel, byColorType);;

	if ( m_stDT_PLAYER_BASE_DATA.qwCoin < stRes.qwResValue)
	{
		return EJS_COIN_NOT_ENOUGH;
	}
	return EJS_NORMAL;
}

UINT16 CPlayer::OnJewelryGenerateInfo(PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ& stReq, PKT_CLIGS_JEWELRY_GENERATE_INFO_ACK& stAck)
{
	
	vector<UINT32> vecJewelryKey;
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wFirstID, stReq.wFirstIdx) );
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wSecondID, stReq.wSecondIdx) );
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wThirdID, stReq.wThirdIdx) );
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wFourthID, stReq.wFourthIdx) );
	

	UINT8 byRet = OnJewelryGenerateCheck(vecJewelryKey, stAck.wGenerateLevel, stAck.byGenerateType, stAck.stGenerateCost);
	
	if ( EJS_NORMAL == byRet )
	{
		stAck.byState = EJS_NORMAL;
	}
	else if ( EJS_ALREAD_STRENG == byRet)
	{
		stAck.byState = EJS_ALREAD_STRENG;
	}
	else if ( EJS_JEWELRY_IS_NULL == byRet)
	{
		stAck.byState = EJS_JEWELRY_IS_NULL;
	}
	else if ( EJS_COIN_NOT_ENOUGH == byRet)
	{
		stAck.byState = EJS_JEWELRY_IS_NULL;
	}

	return ERR_JEWELRY_GENERATE_INFO::ID_SUCCESS;
}

UINT16 CPlayer::OnJewelryGenerate(PKT_CLIGS_JEWELRY_GENERATE_REQ& stReq, PKT_CLIGS_JEWELRY_GENERATE_ACK& stAck)
{
	vector<UINT32> vecJewelryKey;
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wFirstID, stReq.wFirstIdx) );
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wSecondID, stReq.wSecondIdx) );
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wThirdID, stReq.wThirdIdx) );
	vecJewelryKey.push_back( CLogicFunc::Get1616Key(stReq.wFourthID, stReq.wFourthIdx) );

	UINT16 wLevel = 0;
	UINT8 byColor = 0;
	DT_RES_DATA stResData = {0};
	UINT8 byRet = OnJewelryGenerateCheck(vecJewelryKey, wLevel, byColor, stResData);


	if ( EJS_NORMAL == byRet )
	{
		UINT16 wKindID = CJewelryPropMgr::Instance()->GetRandomJewelryID(wLevel, byColor);
		if ( 0 == wKindID)
		{
			USR_INFO("[%s:%u] JewelryLevel = %u, JewelryColor = %u", MSG_MARK, wLevel, byColor);
			return ERR_JEWELRY_GENERATE::ID_IS_EXCEPTION;
		}
		ECreateItemRet ecir;
		CJewelry* poJewelry = static_cast<CJewelry*>(CItemMgr::Instance()->CreateItemIntoBag( this, wKindID, 1, ecir, wLevel, CRecordMgr::EAIT_JEWELRY));
		if ( NULL == poJewelry)
		{
			return ERR_JEWELRY_GENERATE::ID_BAG_IS_FULL;
		}

		for ( vector<UINT32>::iterator itr = vecJewelryKey.begin(); itr != vecJewelryKey.end(); itr++)
		{
			CJewelry* poJewelry = m_mapAllJewelry.GetData(*itr);
			if ( NULL == poJewelry)
			{
				continue;
			}
			 m_mapAllJewelry.DelData( *itr );
			OnDeleteItem(poJewelry);
		}
		DecCoin(static_cast<UINT32>(stResData.qwResValue), CRecordMgr::EDCT_GENERATE_JEWELRY, wKindID);

		poJewelry->GetJewelryDataCli(stAck.stJewelryInfo);
		CBag& oBag = GetBag();
		oBag.GetDT_JEWELRY_DATA_LST_CLI(stAck.stBagJewelryInfo);
		stAck.stResInfo.astResList[0].byResKind = stResData.byResKind;
		stAck.stResInfo.astResList[0].qwResValue = stResData.qwResValue;
		stAck.stResInfo.byResNum = 1;

		stAck.stResourceSnyc.astResList[0].byResKind = stResData.byResKind;
		stAck.stResourceSnyc.astResList[0].qwResValue = GetDT_PLAYER_BASE_DATA().qwCoin;
		stAck.stResourceSnyc.byResNum = 1;

		UINT8 byPosID = EJDP_NECKLACE;
		if ( NULL != poJewelry->GetProp())
		{
			byPosID = poJewelry->GetProp()->byPosKindID;
		}
		CJewelry* poLowJewelry = NULL;
		CHero* poHero = GetCoachHero();
		if ( NULL == poHero)
		{
			return ERR_JEWELRY_COMPARE::ID_COACH_HERO_LOST;
		}
		CJewelry* poCurJewelry = poHero->GetJewelryByPos( byPosID );
		if ( NULL != poCurJewelry)
		{
			poLowJewelry = poCurJewelry;
		}
		set<CHero*> setHero;
		setHero.insert(poHero);
		poHero = m_mapFormationHero.GetFistData();
		while(poHero)
		{
			if ( setHero.end() != setHero.find(poHero))
			{
				poHero = m_mapFormationHero.GetNextData();
				continue;
			}
			if ( poHero->GetDT_HERO_BASE_DATA().wLevel < poJewelry->GetDressLevel())
			{
				poHero = m_mapFormationHero.GetNextData();
				continue;
			}
			setHero.insert(poHero);
			poCurJewelry = poHero->GetJewelryByPos(byPosID);
			poLowJewelry = CJewelryPropMgr::Instance()->CompareJewelry( poLowJewelry, poCurJewelry);
			poHero = m_mapFormationHero.GetNextData();
		}


		if ( NULL != poLowJewelry)
		{
			poLowJewelry->GetJewelryDataCli(stAck.stCompareJewelry);
		}

	}
	else if ( EJS_ALREAD_STRENG == byRet)
	{
		return ERR_JEWELRY_GENERATE::ID_ALREAD_STRENG;
	}
	else if ( EJS_JEWELRY_IS_NULL == byRet)
	{
		return ERR_JEWELRY_GENERATE::ID_SELECT_JEWELRY;
	}
	else if ( EJS_COIN_NOT_ENOUGH == byRet)
	{
		return ERR_COMMON::ID_COIN_NOT_ENOUGH;
	}


	return ERR_JEWELRY_GENERATE::ID_SUCCESS;
}

UINT16 CPlayer::OnGetPlayerInfoJewelry(PKT_CLIGS_PLAYER_INFO_JEWELRY_ACK& stAck)
{
    typedef set<CHero*> HeroSet;
	HeroSet setHero;

	UINT8 byHeroNum = 0;
	CHero* poHero = GetCoachHero();
	if ( NULL == poHero)	{
		return ERR_PLAYER_INFO_JEWELRY::ID_COACH_HERO_LOST;
	}

	setHero.insert(poHero);

	CFormationHeroShmemMap& mapFormationHero = GetFormationHeroMap();
	poHero = mapFormationHero.GetFistData();
	while ( poHero){
		if(setHero.find(poHero) != setHero.end()){
			poHero = mapFormationHero.GetNextData();
			continue;
		}
		setHero.insert(poHero);
		poHero = mapFormationHero.GetNextData();
	}

	CKindID2HeroShmemMap& mapKind2HeroAllCarry = GetAllCarryHeroMap();
	poHero = mapKind2HeroAllCarry.GetFistData();
	while ( poHero){
		if(setHero.find(poHero) != setHero.end())
		{
			poHero = mapKind2HeroAllCarry.GetNextData();
			continue;
		}

		setHero.insert(poHero);
		poHero = mapKind2HeroAllCarry.GetNextData();
	}

    for(HeroSet::iterator itr = setHero.end(); itr != setHero.end(); ++itr){
        CHero *pHero = *itr;
        poHero->GetJeweryCli( stAck.astJewelryInfo[byHeroNum]);
        poHero->GetJewerySuitCli(stAck.astJewelrySuit[byHeroNum]);
        byHeroNum++;
    }

	stAck.byHeroNum = byHeroNum;
	return ERR_PLAYER_INFO_JEWELRY::ID_SUCCESS;
}
UINT16 CPlayer::OnJewelryResolve(PKT_CLIGS_JEWELRY_RESOLVE_REQ& stReq, PKT_CLIGS_JEWELRY_RESOLVE_ACK2& stAck)
{
	CJewelry* poJewelry = GetJewelry(stReq.wJewelryID, stReq.wJewelryIdx);
	if ( NULL == poJewelry)
	{
		return ERR_JEWELRY_RESOLVE::ID_NOT_HAVE_JEWELRY;
	}

	UINT32 dwRes = 0;
	//分解成功
	UINT8 eRet = poJewelry->ResolveJewelry(stAck.stItemInfo, dwRes);
	if ( ECIR_SUCCESS == eRet)
	{
		//扣除消耗
		DecCoin(dwRes, CRecordMgr::EDCT_RESOLVE_JEWELRY, poJewelry->GetDT_JEWELRY_DATA().wJewelryID, poJewelry->GetDT_JEWELRY_DATA().wStrengLevel );

		//删除物品
		OnDeleteItem(poJewelry);

		for ( UINT32 dwIdx = 0; dwIdx < stAck.stItemInfo.wGoodsNum; dwIdx++)
		{
			ECreateItemRet eCRet ;
			//增加物品--强化道具
			CItemMgr::Instance()->CreateItemIntoBag(this, 
				stAck.stItemInfo.astGoodsList[dwIdx].wKindID, 
				stAck.stItemInfo.astGoodsList[dwIdx].wPileCount,
				eCRet, 0, CRecordMgr::EAIT_RESOLVE_JEWELRY);
		}

	}
	else if (ECIR_FAILED_BAG_FULL == eRet )
	{
		return ERR_JEWELRY_RESOLVE::ID_BAG_IS_FULL;
	}
	else if ( ECIR_COIN_NOT_ENOUGH == eRet)
	{
		return ERR_COMMON::ID_COIN_NOT_ENOUGH;
	}
	else
	{
		return ERR_JEWELRY_RESOLVE::ID_JEWELRY_INFO_UNNORMAL;
	}
	stAck.stResourceSnyc.byResNum = 1;
	stAck.stResourceSnyc.astResList[0].byResKind = ACTE_COIN;
	stAck.stResourceSnyc.astResList[0].qwResValue = m_stDT_PLAYER_BASE_DATA.qwCoin;

	return ERR_JEWELRY_RESOLVE::ID_SUCCESS;
}

UINT16 CPlayer::OnJewelryResolve(PKT_CLIGS_JEWELRY_RESOLVE_REQ& stReq, PKT_CLIGS_JEWELRY_RESOLVE_ACK& stAck)
{
	CJewelry* poJewelry = GetJewelry(stReq.wJewelryID, stReq.wJewelryIdx);
	if ( NULL == poJewelry)
	{
		return ERR_JEWELRY_RESOLVE::ID_NOT_HAVE_JEWELRY;
	}

	UINT32 dwRes = 0;
	//分解成功
	UINT8 eRet = poJewelry->ResolveJewelry(stAck.stItemInfo, dwRes);
	if ( ECIR_SUCCESS == eRet)
	{
		//扣除消耗
		DecCoin(dwRes, CRecordMgr::EDCT_RESOLVE_JEWELRY, poJewelry->GetDT_JEWELRY_DATA().wJewelryID, poJewelry->GetDT_JEWELRY_DATA().wStrengLevel );

		//删除物品
		OnDeleteItem(poJewelry);

		for ( UINT32 dwIdx = 0; dwIdx < stAck.stItemInfo.wGoodsNum; dwIdx++)
		{
			ECreateItemRet eCRet ;
			//增加物品--强化道具
			CItemMgr::Instance()->CreateItemIntoBag(this, 
				stAck.stItemInfo.astGoodsList[dwIdx].wKindID, 
				stAck.stItemInfo.astGoodsList[dwIdx].wPileCount,
				eCRet, 0, CRecordMgr::EAIT_RESOLVE_JEWELRY);
		}
		
	}
	else if (ECIR_FAILED_BAG_FULL == eRet )
	{
		return ERR_JEWELRY_RESOLVE::ID_BAG_IS_FULL;
	}
	else if ( ECIR_COIN_NOT_ENOUGH == eRet)
	{
		return ERR_COMMON::ID_COIN_NOT_ENOUGH;
	}
	else
	{
		return ERR_JEWELRY_RESOLVE::ID_JEWELRY_INFO_UNNORMAL;
	}
	stAck.stResourceSnyc.byResNum = 1;
	stAck.stResourceSnyc.astResList[0].byResKind = ACTE_COIN;
	stAck.stResourceSnyc.astResList[0].qwResValue = m_stDT_PLAYER_BASE_DATA.qwCoin;
	CBag& oBag = GetBag();
	if ( stReq.byBagKind == 1)
	{
		oBag.GetBagItem( stAck.stJewelryInfo );
	}
	else if( stReq.byBagKind == 2)
	{
		oBag.GetAboutJewelryLstCli( stAck.stJewelryInfo );
	}
	else if ( stReq.byBagKind == 3)
	{
		oBag.GetDT_JEWELRY_DATA_LST_CLI( stAck.stJewelryInfo );
	}
	

	return ERR_JEWELRY_RESOLVE::ID_SUCCESS;
}

UINT16 CPlayer::OpenJewelryStrength(PKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK& stAck)
{

	set<CHero*> setHero;
	UINT8 byHeroNum = 0;
	CHero* poHero = GetCoachHero();
	if ( NULL == poHero)
	{
		return ERR_OPEN_JEWELRY_STRENGTH::ID_COACH_HERO_LOST;
	}
	poHero->GetJewelryStrengthInfo( stAck.astHeroJewelryStrengInfo[byHeroNum]);
	byHeroNum++;
	setHero.insert(poHero);

	poHero = m_mapFormationHero.GetFistData();
	while(poHero)
	{
		if ( setHero.end() != setHero.find(poHero) )
		{
			poHero = m_mapFormationHero.GetNextData();
			continue;
		}
		poHero->GetJewelryStrengthInfo(stAck.astHeroJewelryStrengInfo[byHeroNum]);
		setHero.insert(poHero);
		byHeroNum++;
		poHero = m_mapFormationHero.GetNextData();
	}

	poHero = m_mapKind2HeroAllCarry.GetFistData();
	while(poHero)
	{
		if ( setHero.end() != setHero.find(poHero) )
		{
			poHero = m_mapKind2HeroAllCarry.GetNextData();
			continue;
		}
		
		poHero->GetJewelryStrengthInfo(stAck.astHeroJewelryStrengInfo[byHeroNum]);
		setHero.insert(poHero);
		byHeroNum++;
		poHero = m_mapKind2HeroAllCarry.GetNextData();
	}
	CBag& oBag = GetBag();
	oBag.GetJewelryStrengthInfo(stAck.stBagJewelryStrengInfo);
	stAck.byHeroNum = byHeroNum;
	UINT8 byResNum = 0;
	stAck.stPlayerRes.astPlayerRes[byResNum].byResKind = ACTE_COIN;
	stAck.stPlayerRes.astPlayerRes[byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwCoin;
	byResNum++;
	stAck.stPlayerRes.astPlayerRes[byResNum].byResKind = ACTE_GOLD;
	stAck.stPlayerRes.astPlayerRes[byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwGold;
	byResNum++;
	stAck.stPlayerRes.byResNum = byResNum;
	return ERR_OPEN_JEWELRY_STRENGTH::ID_SUCCESS;
}

UINT16 CPlayer::GetItemUseBagNum(UINT16 wItemID, UINT8 byNum )
{
	  UINT8 byItemKind = CItemMgr::Instance()->GetItemKindBYKindID(wItemID);
	  switch ( byItemKind)
	  {
	  case EIK_EQUIP:
		  return byNum;
		  break;
	  case EIK_GOODS:
		  if(NULL == GetBag().GetGoodsItem(wItemID))
		  {
			  return 1;
		  }
		  else
		  {
			  return 0;
		  }
		  break;
	  case EIK_JEWELRY:
		  return byNum;
	  	  break;
	  default:
		  break;
	  }
	  return 0;
}