#include "bag.h"
#include <net/gt/gtpktbuilder.h>
#include <sdutil.h>
#include <errdef.h>
#include <logic/godweapon/godweaponbasepropmgr.h>
#include <db/autosqlbuf.h>
#include <logic/record/recordmgr.h>
#include <logic/other/xiangqianmgr.h>
#include <logic/item/itemmgr.h>
#include <logic/player/player.h>

#include <logic/base/logicfunc.h>
#include <dll/sdframework/sdloggerimpl.h>

IMPLEMENT_CLASS_POOL_MGR(CBag)

CBag::CBag()
{
    m_byOpenNum = 0;
    m_wItemPos = 0;
    m_mapItem.Clear();
    m_mapItemPos.Clear();
    m_mapKindID2Goods.Clear();
	m_mapKindID2Jewery.Clear();
	m_mapKindID2Equip.Clear();
}


CBag::~CBag()
{
    //释放背包中的道具
    CItem* poItem = m_mapItem.GetFistData();
    while(poItem)
    {
        CItemMgr::Instance()->RemoveItem(poItem);

        poItem = m_mapItem.GetNextData();
    }
    m_mapItem.Clear();
    m_mapItemPos.Clear();
    m_mapKindID2Goods.Clear();
    m_mapKindID2Equip.Clear();
	m_mapKindID2Jewery.Clear();
}


//初始化相关对象
BOOL CBag::Init(UINT8 byOpenNum, CPlayer* poOwner)
{
    //
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_byOpenNum = byOpenNum;
    m_poOwner = poOwner;

    return TRUE;
}

BOOL CBag::AddItem(CItem* poItem, BOOL bNewItem)
{
    //已经加入过，直接返回成功
    if(NULL != m_mapItem.GetData(poItem->GetMemID()))
    {
        return TRUE;
    }

    SBagProp& pstBagProp = CBagPropMgr::Instance()->GetBagProp();
    //背包已满
    if(IsBagFull())
    {
        DBG_INFO(_SDT("[%s: %d]: PlayerID[%u] could not add item bag full, itemkind = %u, itemkindid = %u, usedsize = %u, maxsize=%u!"), MSG_MARK, 
            m_poOwner->GetID(), poItem->GetItemKind(), poItem->GetItemKindID(), (UINT16)(m_mapItem.Size()), pstBagProp.wTotalBagNum);
        return FALSE;
    }

    m_mapItem.AddData(poItem->GetMemID(), poItem);

    if(EIK_EQUIP == poItem->GetItemKind())
    {
        CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
        if(NULL == poEquip)
        {
            DBG_INFO(_SDT("[%s: %d]: dynamic_cast CEquip failed!"), MSG_MARK);
            return FALSE;
        }
        m_mapKindID2Equip.AddData(CLogicFunc::Get1616Key(poEquip->GetItemKindID(), poEquip->GetDT_EQUIP_DATA().wIdx), poEquip);
        if(bNewItem)
        {
            m_poOwner->GetAllEquip().AddData(CLogicFunc::Get1616Key(poEquip->GetDT_EQUIP_DATA().wKindID, poEquip->GetDT_EQUIP_DATA().wIdx), poEquip);
        }
    }
    else if (EIK_GOODS == poItem->GetItemKind() )
    {
        CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
        if(NULL == poGoods)
        {
            DBG_INFO(_SDT("[%s: %d]: dynamic_cast CGoods failed!"), MSG_MARK);
            return FALSE;
        }
        m_mapKindID2Goods.AddData(poItem->GetItemKindID(), poGoods);
    }
	else if ( EIK_JEWELRY == poItem->GetItemKind())
	{
		CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
		if(NULL == poJewelry)
		{
			DBG_INFO(_SDT("[%s: %d]: dynamic_cast poJewelry failed!"), MSG_MARK);
			return FALSE;
		}
		m_mapKindID2Jewery.AddData(CLogicFunc::Get1616Key(poJewelry->GetItemKindID(), poJewelry->GetDT_JEWELRY_DATA().wIdx), poJewelry);
		if(bNewItem)
		{
			m_poOwner->GetAllJewelry().AddData(CLogicFunc::Get1616Key(poJewelry->GetItemKindID(), poJewelry->GetDT_JEWELRY_DATA().wIdx), poJewelry);
		}
	}
	else
	{
		m_mapItem.DelData(poItem->GetMemID());
		return TRUE;
	}
    //赋值背包类型及背包位置
    poItem->GetBagPos() = ++m_wItemPos;//每次重新给排序位置，该位置不是真正位置，只是排序的大小
    m_mapItemPos.AddData(poItem->GetBagPos(), poItem);

    //SetSaveState(ESS_WANT_SAVE);

    return TRUE;
}

UINT16 CBag::GetIdleSize()
{
    SBagProp& pstBagProp = CBagPropMgr::Instance()->GetBagProp();
    //UINT16 wTotalOpenNum = pstBagProp->wInitOpenNum + m_wOpenNum;
	UINT16 wTotalOpenNum = pstBagProp.wTotalBagNum;
	if ( VER_PET > m_poOwner->GetCliVer())
	{
		wTotalOpenNum = 255;
	}
    UINT16 wUsedSize = m_mapItem.Size();
    if(wTotalOpenNum >= wUsedSize)
    {
        return wTotalOpenNum - wUsedSize;
    }

    return 0;
}

BOOL CBag::DelItem(CItem* poItem, BOOL bDeleteItem /* = TRUE */, CRecordMgr::EDecItemType eRecordType /* = 0 */)
{
    if(NULL == poItem)
    {
        return FALSE;
    }

    m_mapItem.DelData(poItem->GetMemID());
    if(EIK_GOODS == poItem->GetItemKind())
    {
        m_mapKindID2Goods.DelData(poItem->GetItemKindID());
    }
    else if(EIK_EQUIP == poItem->GetItemKind())
    {
        CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
        if(poEquip)
        {
            m_mapKindID2Equip.DelData(CLogicFunc::Get1616Key(poEquip->GetItemKindID(), poEquip->GetDT_EQUIP_DATA().wIdx));
        }
        if(bDeleteItem)
        {
            m_poOwner->GetAllEquip().DelData(CLogicFunc::Get1616Key(poEquip->GetDT_EQUIP_DATA().wKindID, poEquip->GetDT_EQUIP_DATA().wIdx));
        }
    }
	else if ( EIK_JEWELRY == poItem->GetItemKind())
	{
		CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
		if ( NULL != poJewelry)
		{
			m_mapKindID2Jewery.DelData(CLogicFunc::Get1616Key(poJewelry->GetItemKindID(), poJewelry->GetDT_JEWELRY_DATA().wIdx));
		}
		if(bDeleteItem)
		{
			m_poOwner->GetAllJewelry().DelData(CLogicFunc::Get1616Key(poJewelry->GetItemKindID(), poJewelry->GetDT_JEWELRY_DATA().wIdx));
		}
		
	}

    if (eRecordType != 0){
        CRecordMgr::Instance()->RecordInfo(m_poOwner->GetID(), ERM_DECITEM, eRecordType, poItem->GetItemKindID(), poItem->GetCurPileNum(), 
            m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, 0, 0, 0, 0);
    }

    m_mapItemPos.DelData(poItem->GetBagPos());
    return TRUE;
}

BOOL CBag::ReplaceItem(CItem* poItem_IN, CItem* poItem_OUT)
{
    //已经加入过，直接返回成功
    if(NULL == m_mapItem.GetData(poItem_IN->GetMemID()))
    {
        return FALSE;
    }
    //检测类型是否一至
    if (poItem_IN->GetItemKind() != poItem_OUT->GetItemKind())
    {
        return FALSE;
    }

    if(EIK_EQUIP == poItem_IN->GetItemKind())
    {
        CEquip* poEquip_IN = dynamic_cast<CEquip*>(poItem_IN);
        if(NULL == poEquip_IN)
        {
            DBG_INFO(_SDT("[%s: %d]: dynamic_cast CEquip failed!"), MSG_MARK);
            return FALSE;
        }

        CEquip* poEquip_OUT = dynamic_cast<CEquip*>(poItem_OUT);
        if(NULL == poEquip_OUT)
        {
            DBG_INFO(_SDT("[%s: %d]: dynamic_cast CEquip failed!"), MSG_MARK);
            return FALSE;
        }

        m_mapKindID2Equip.DelData(CLogicFunc::Get1616Key(poEquip_IN->GetItemKindID(), poEquip_IN->GetDT_EQUIP_DATA().wIdx));
        m_mapKindID2Equip.AddData(CLogicFunc::Get1616Key(poEquip_OUT->GetItemKindID(), poEquip_OUT->GetDT_EQUIP_DATA().wIdx), poEquip_OUT);
    }
    else if(EIK_GOODS == poItem_IN->GetItemKind())
    {
        CGoods* poGoods_IN = dynamic_cast<CGoods*>(poItem_IN);
        if(NULL == poGoods_IN)
        {
            DBG_INFO(_SDT("[%s: %d]: dynamic_cast CGoods failed!"), MSG_MARK);
            return FALSE;
        }

        CGoods* poGoods_OUT = dynamic_cast<CGoods*>(poItem_OUT);
        if(NULL == poGoods_OUT)
        {
            DBG_INFO(_SDT("[%s: %d]: dynamic_cast CGoods failed!"), MSG_MARK);
            return FALSE;
        }

        m_mapKindID2Goods.DelData(poGoods_IN->GetItemKindID());
        m_mapKindID2Goods.AddData(poGoods_OUT->GetItemKindID(), poGoods_OUT);
    }
	else if (EIK_JEWELRY == poItem_IN->GetItemKind())
	{
		CJewelry* poJewelry_IN = dynamic_cast<CJewelry*>(poItem_IN);
		if(NULL == poJewelry_IN)
		{
			DBG_INFO(_SDT("[%s: %d]: dynamic_cast CJewelry failed!"), MSG_MARK);
			return FALSE;
		}

		CJewelry* poJewelry_OUT = dynamic_cast<CJewelry*>(poItem_OUT);
		if(NULL == poJewelry_OUT)
		{
			DBG_INFO(_SDT("[%s: %d]: dynamic_cast CJewelry failed!"), MSG_MARK);
			return FALSE;
		}

		m_mapKindID2Jewery.DelData(CLogicFunc::Get1616Key(poJewelry_IN->GetItemKindID(), poJewelry_IN->GetDT_JEWELRY_DATA().wIdx));
		m_mapKindID2Jewery.AddData(CLogicFunc::Get1616Key(poJewelry_OUT->GetItemKindID(), poJewelry_OUT->GetDT_JEWELRY_DATA().wIdx), poJewelry_OUT);
	}

    poItem_OUT->GetBagPos()	= poItem_IN->GetBagPos();
    poItem_IN->GetBagPos()		= 0;
    m_mapItem.DelData(poItem_IN->GetMemID());
    m_mapItem.AddData(poItem_OUT->GetMemID(), poItem_OUT);
    m_mapItemPos.AddData(poItem_OUT->GetBagPos(), poItem_OUT);
    return TRUE;
}

UINT16 CBag::IncreaseSize(UINT16 wInc2Size, UINT8 byGoldFlag)
{
	return 0;
    SBagProp& stProp = CBagPropMgr::Instance()->GetBagProp();
	
    //判断容量
    if(wInc2Size > stProp.wTotalBagNum)
    {
        return ERR_INCREASE_SIZE::ID_HAVE_REACH_MAX_SIZE;
    }
    UINT16 wCurSize = m_byOpenNum + stProp.wInitOpenNum;
    if(wInc2Size <= wCurSize)
    {
        return ERR_INCREASE_SIZE::ID_SIZE_SMALL_EQUAL_THAN_CUR;
    }

    UINT16 wIncSize = wInc2Size - wCurSize;
    UINT32 dwCost = 0;
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = m_poOwner->GetDT_PLAYER_BASE_DATA();
    //判断/扣除金钱
    if(1 == byGoldFlag)
    {
        for(UINT16 wIdx = 0; wIdx < wIncSize; wIdx++)//第一个没有递增，则byIdx从0开始
        {
            dwCost += stProp.dwBaseOpenGold + (wCurSize + wIdx - stProp.wInitOpenNum) * stProp.dwOpenGoldIncrease;
        }

        if(stDT_PLAYER_BASE_DATA.qwGold < dwCost)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }
        //stDT_PLAYER_BASE_DATA.qwGold -= dwCost;
        m_poOwner->DecGold(dwCost, CRecordMgr::EDGT_INCREASESIZE);
    }
    else
    {
        for(UINT16 wIdx = 0; wIdx < wIncSize; wIdx++)//第一个没有递增，则byIdx从0开始
        {
            dwCost += stProp.dwBaseOpenCoin + (wCurSize + wIdx - stProp.wInitOpenNum) * stProp.dwOpenCoinIncrease;
        }

        if(stDT_PLAYER_BASE_DATA.qwCoin < dwCost)
        {
            return ERR_COMMON::ID_COIN_NOT_ENOUGH;
        }
        //stDT_PLAYER_BASE_DATA.qwCoin -= dwCost;
        m_poOwner->DecCoin(dwCost, CRecordMgr::EDCT_INCREASESIZE, m_byOpenNum);
    }

    //扩容
    m_byOpenNum = wInc2Size - stProp.wInitOpenNum;
    m_poOwner->SetBagOpenNum(m_byOpenNum);


    //需保存
    //SetSaveState(ESS_WANT_SAVE);

    return 0;
}

VOID CBag::GetDT_BAG_DATA_CLI(DT_BAG_DATA_CLI& stDT_BAG_DATA_CLI)
{
    SBagProp& pstBagProp = CBagPropMgr::Instance()->GetBagProp();
    stDT_BAG_DATA_CLI.byCurOpenNum = (255 < (pstBagProp.wTotalBagNum)) ? 255 : (pstBagProp.wTotalBagNum);
    stDT_BAG_DATA_CLI.byMaxOpenNum = ( 255 < pstBagProp.wTotalBagNum) ? 255 : pstBagProp.wTotalBagNum;
    stDT_BAG_DATA_CLI.byInitOpenNum = ( 255 < pstBagProp.wInitOpenNum) ? 255 : pstBagProp.wInitOpenNum;
    stDT_BAG_DATA_CLI.dwBaseOpenCoin = pstBagProp.dwBaseOpenCoin;
    stDT_BAG_DATA_CLI.dwOpenCoinIncrease = pstBagProp.dwOpenCoinIncrease;
    stDT_BAG_DATA_CLI.dwBaseOpenGold = pstBagProp.dwBaseOpenGold;
    stDT_BAG_DATA_CLI.dwOpenGoldIncrease = pstBagProp.dwOpenGoldIncrease;
}

VOID CBag::GetDT_BAG_DATA_CLI(DT_BAG_DATA_CLI2& stDT_BAG_DATA_CLI)
{
	SBagProp& pstBagProp = CBagPropMgr::Instance()->GetBagProp();
	stDT_BAG_DATA_CLI.wCurOpenNum = pstBagProp.wTotalBagNum;
	stDT_BAG_DATA_CLI.wMaxOpenNum = pstBagProp.wTotalBagNum;
	stDT_BAG_DATA_CLI.wInitOpenNum = pstBagProp.wInitOpenNum;
	stDT_BAG_DATA_CLI.dwBaseOpenCoin = pstBagProp.dwBaseOpenCoin;
	stDT_BAG_DATA_CLI.dwOpenCoinIncrease = pstBagProp.dwOpenCoinIncrease;
	stDT_BAG_DATA_CLI.dwBaseOpenGold = pstBagProp.dwBaseOpenGold;
	stDT_BAG_DATA_CLI.dwOpenGoldIncrease = pstBagProp.dwOpenGoldIncrease;
}

VOID CBag::GetDT_EQUIP_DATA_LST_CLI(DT_EQUIP_DATA_LST_CLI& stDT_EQUIP_DATA_LST_CLI)
{
    //memset(&stDT_EQUIP_DATA_LST_CLI, 0, sizeof(stDT_EQUIP_DATA_LST_CLI));

    UINT16 wNum = stDT_EQUIP_DATA_LST_CLI.wEquipNum;
    CEquip* poEquip = m_mapKindID2Equip.GetFistData();
    while((poEquip) && (wNum < MAX_BAG_OPEN_NUM))
    {
        DT_EQUIP_DATA_CLI& stDT_EQUIP_DATA_CLI = stDT_EQUIP_DATA_LST_CLI.astEquipList[wNum++];
        poEquip->GetDT_EQUIP_DATA_CLI(stDT_EQUIP_DATA_CLI);

        poEquip = m_mapKindID2Equip.GetNextData();
    }
    stDT_EQUIP_DATA_LST_CLI.wEquipNum = wNum;
}

VOID CBag::GetDT_EQUIP_MELT_DATA_LST_CLI(DT_EQUIP_MELT_DATA_LST_CLI& stDT_EQUIP_MELT_DATA_LST_CLI)
{
    //memset(&stDT_EQUIP_DATA_LST_CLI, 0, sizeof(stDT_EQUIP_DATA_LST_CLI));

    UINT16 wNum = stDT_EQUIP_MELT_DATA_LST_CLI.wEquipNum;
    CEquip* poEquip = m_mapKindID2Equip.GetFistData();
    while((poEquip) && (wNum < MAX_BAG_OPEN_NUM))
    {
        DT_EQUIP_MELT_DATA_CLI& stDT_EQUIP_MELT_DATA_CLI = stDT_EQUIP_MELT_DATA_LST_CLI.astEquipList[wNum++];
        poEquip->GetDT_EQUIP_MELT_DATA_CLI(stDT_EQUIP_MELT_DATA_CLI);

        poEquip = m_mapKindID2Equip.GetNextData();
    }
    stDT_EQUIP_MELT_DATA_LST_CLI.wEquipNum = wNum;
}

VOID CBag::GetDT_GOOD_MELT_DATA_LST_CLI(DT_GOOD_MELT_DATA_LST_CLI &stDT_GOOD_MELT_DATA_LST_CLI )
{
    CGoods* poGoods = m_mapKindID2Goods.GetFistData();
    while((poGoods) && (stDT_GOOD_MELT_DATA_LST_CLI.wGoodNum < MAX_BAG_OPEN_NUM))
    {
        DT_GOOD_MELT_DATA_CLI& stDT_GOOD_MELT_DATA_CLI = stDT_GOOD_MELT_DATA_LST_CLI.astGoodList[stDT_GOOD_MELT_DATA_LST_CLI.wGoodNum];
        SGoodsProp *poGoodsProp = poGoods->GetGoodsProp();
        if (NULL == poGoodsProp)
        {
            break;
        }
        //装备碎片
        if (EGMK_EQUIP_DEBRIS == poGoodsProp->byMainKindID )
        {
            stDT_GOOD_MELT_DATA_LST_CLI.wGoodNum++;
            poGoods->GetDT_GOOD_MELT_DATA_CLI(stDT_GOOD_MELT_DATA_CLI);
        }
        poGoods = m_mapKindID2Goods.GetNextData();
    }
}


VOID CBag::GetDT_EQUIP_DATA_LST_STRENGTHEN_COMPOSE_CLI(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stDT_EQUIP_DATA_LST_CLI)
{
    //memset(&stDT_EQUIP_DATA_LST_CLI, 0, sizeof(stDT_EQUIP_DATA_LST_CLI));

    UINT16 wEquipNum = stDT_EQUIP_DATA_LST_CLI.wEquipNum;
    CEquip* poEquip = m_mapKindID2Equip.GetFistData();
    while((poEquip) && (wEquipNum < MAX_BAG_OPEN_NUM))
    {
        DT_EQUIP_DATA_STRENGTHEN_CLI& stDT_EQUIP_DATA_CLI = stDT_EQUIP_DATA_LST_CLI.astEquipStrengthenInfo[wEquipNum];
        poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(stDT_EQUIP_DATA_CLI);
        DT_EQUIP_DATA_CLI& pstDT_EQUIP_DATA_CLI = stDT_EQUIP_DATA_LST_CLI.astEquipInfo[wEquipNum];
        poEquip->GetDT_EQUIP_DATA_CLI(pstDT_EQUIP_DATA_CLI);

        poEquip = m_mapKindID2Equip.GetNextData();
        wEquipNum++;
    }
    stDT_EQUIP_DATA_LST_CLI.wEquipNum = wEquipNum;
}

VOID CBag::GetEquipStrengthenEnchantCli(DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI &stEquipList)
{
    CEquip* poEquip = m_mapKindID2Equip.GetFistData();
    while(poEquip){
        if (stEquipList.wEquipNum >= MAX_BAG_OPEN_NUM){
            break;
        }

        DT_EQUIP_DATA_CLI_v410* pstEquip = &(stEquipList.astEquipInfo[stEquipList.wEquipNum]);
        DT_EQUIP_DATA_STRENGTHEN_CLI* pstEquipStrengthen = &(stEquipList.astEquipStrengthenInfo[stEquipList.wEquipNum]);
        DT_EQUIP_ENCHANT_DATA_CLI* pstEquipEnchant = &(stEquipList.astEquipEnchantInfo[stEquipList.wEquipNum]);

        poEquip->GetDT_EQUIP_DATA_CLI_v410(*pstEquip);
        poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(*pstEquipStrengthen);
        poEquip->GetDT_EQUIP_DATA_ENCHANT_CLI(*pstEquipEnchant);

        stEquipList.wEquipNum++;
        poEquip = m_mapKindID2Equip.GetNextData();
    }

    if(m_poOwner->GetBuild(EBK_ENCHANT)){
        stEquipList.wEnchantEquipNum = stEquipList.wEquipNum;
    }
}

//获取装备的最大镶嵌宝石个数。
UINT8 CBag::GetMaxMosaicGemNum()
{
    UINT8	byMosaicGemNum	=	0;
    UINT8	byMaxMosaicGemNum =	0;
    CEquip*	poEquip = m_mapKindID2Equip.GetFistData();
    while(poEquip)
    {
        byMosaicGemNum = poEquip->GetMosaicGemNum();
        if (byMosaicGemNum > byMaxMosaicGemNum )
        {
            byMaxMosaicGemNum = byMosaicGemNum;
        }
        poEquip = m_mapKindID2Equip.GetNextData();
    }
    return byMaxMosaicGemNum;
}

VOID CBag::GetDT_GOODS_DATA_LST_CLI(DT_GOODS_DATA_LST_CLI& stDT_GOODS_DATA_LST_CLI)
{
    //memset(&stDT_GOODS_DATA_LST_CLI, 0, sizeof(stDT_GOODS_DATA_LST_CLI));

    UINT16 wNum = stDT_GOODS_DATA_LST_CLI.wGoodsNum;
    CGoods* poGoods = m_mapKindID2Goods.GetFistData();
    while((poGoods) && (wNum < MAX_BAG_OPEN_NUM))
    {
        DT_GOODS_DATA_CLI& stDT_GOODS_DATA_CLI = stDT_GOODS_DATA_LST_CLI.astGoodsList[wNum++];
        poGoods->GetDT_GOODS_DATA_CLI(stDT_GOODS_DATA_CLI);

        poGoods = m_mapKindID2Goods.GetNextData();
    }
    stDT_GOODS_DATA_LST_CLI.wGoodsNum = wNum;
}


VOID CBag::GetBagItem(DT_ITEM_DATA_LIST_CLI2& stBagItemInfo)
{
    memset(&stBagItemInfo, 0, sizeof(stBagItemInfo));

    UINT8 byItemNum = 0;
    CItem* poItem = m_mapItemPos.GetFistData();
    do
    {
        if(NULL == poItem)
        {
            break;
        }

        switch(poItem->GetItemKind())
        {
        case EIK_EQUIP:
        {
            CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
            if(NULL == poEquip)
            {
                continue;
            }
            stBagItemInfo.astItemList[byItemNum].byItemType = EIK_EQUIP;
            DT_EQUIP_DATA_CLI& stEquip = stBagItemInfo.astItemList[byItemNum].stItemInfo.stEquip ;
            poEquip->GetDT_EQUIP_DATA_CLI(stEquip);
            byItemNum++;
        }
        break;
        case EIK_GOODS:
        {
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(NULL == poGoods)
            {
                continue;
            }
            stBagItemInfo.astItemList[byItemNum].byItemType = EIK_GOODS;
            DT_GOODS_DATA_CLI& stGoods = stBagItemInfo.astItemList[byItemNum].stItemInfo.stGoods ;
            poGoods->GetDT_GOODS_DATA_CLI(stGoods);
            byItemNum++;
        }
        break;
        }
    }
    while ((poItem = m_mapItemPos.GetNextData()) && (byItemNum < MAX_BAG_OPEN_NUM));

    stBagItemInfo.byItemNum = byItemNum;
}

VOID CBag::GetBagItem(DT_ITEM_DATA_LIST_CLI3& stBagItemInfo)
{
	memset(&stBagItemInfo, 0, sizeof(stBagItemInfo));

	UINT8 byItemNum = 0;
	CItem* poItem = m_mapItem.GetFistData();
	do
	{
		if(NULL == poItem)
		{
			break;
		}

		switch(poItem->GetItemKind())
		{
		case EIK_EQUIP:
			{
				CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
				if(NULL == poEquip)
				{
					continue;
				}
				stBagItemInfo.astItemList[byItemNum].byItemType = EIK_EQUIP;
				DT_EQUIP_DATA_CLI& stEquip = stBagItemInfo.astItemList[byItemNum].stItemInfo.stEquip ;
				poEquip->GetDT_EQUIP_DATA_CLI(stEquip);
				byItemNum++;
			}
			break;
		case EIK_GOODS:
			{
				CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
				if(NULL == poGoods)
				{
					continue;
				}
				stBagItemInfo.astItemList[byItemNum].byItemType = EIK_GOODS;
				DT_GOODS_DATA_CLI& stGoods = stBagItemInfo.astItemList[byItemNum].stItemInfo.stGoods ;
				poGoods->GetDT_GOODS_DATA_CLI(stGoods);
				byItemNum++;
			}
			break;
		case EIK_JEWELRY:
			{
				CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
				if(NULL == poJewelry)
				{
					continue;
				}
				stBagItemInfo.astItemList[byItemNum].byItemType = EIK_JEWELRY;
				DT_JEWELRY_DATA_CLI& stJewelry = stBagItemInfo.astItemList[byItemNum].stItemInfo.stJewelry ;
				poJewelry->GetJewelryDataCli(stJewelry);
				byItemNum++;
			}
			break;
		}
	}
	while ((poItem = m_mapItem.GetNextData()) && (byItemNum < MAX_BAG_OPEN_NUM));

	stBagItemInfo.byItemNum = byItemNum;
}

VOID CBag::GetBagItem(DT_ITEM_DATA_LIST_CLI4& stBagItemInfo)
{
	memset(&stBagItemInfo, 0, sizeof(stBagItemInfo));

	UINT16 wItemNum = 0;
	CItem* poItem = m_mapItem.GetFistData();
	do
	{
		if(NULL == poItem)
		{
			break;
		}

		switch(poItem->GetItemKind())
		{
		case EIK_EQUIP:
			{
				CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
				if(NULL == poEquip)
				{
					continue;
				}
				stBagItemInfo.astItemList[wItemNum].byItemType = EIK_EQUIP;
				DT_EQUIP_DATA_CLI& stEquip = stBagItemInfo.astItemList[wItemNum].stItemInfo.stEquip ;
				poEquip->GetDT_EQUIP_DATA_CLI_with_enchant_lvl(stEquip);

				wItemNum++;
			}
			break;
		case EIK_GOODS:
			{
				CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
				if(NULL == poGoods)
				{
					continue;
				}
				stBagItemInfo.astItemList[wItemNum].byItemType = EIK_GOODS;
				DT_GOODS_DATA_CLI& stGoods = stBagItemInfo.astItemList[wItemNum].stItemInfo.stGoods ;
				poGoods->GetDT_GOODS_DATA_CLI(stGoods);
				wItemNum++;
			}
			break;
		case EIK_JEWELRY:
			{
				CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
				if(NULL == poJewelry)
				{
					continue;
				}
				stBagItemInfo.astItemList[wItemNum].byItemType = EIK_JEWELRY;
				DT_JEWELRY_DATA_CLI& stJewelry = stBagItemInfo.astItemList[wItemNum].stItemInfo.stJewelry ;
				poJewelry->GetJewelryDataCli(stJewelry);
				wItemNum++;
			}
			break;
		}
	}
	while ((poItem = m_mapItem.GetNextData()) && (wItemNum < MAX_BAG_OPEN_NUM_NEW));

	stBagItemInfo.wItemNum = wItemNum;
}

VOID CBag::GetBagItemNum(DT_ITEM_NUM_DATA_LIST_CLI& stBagItemNumList)
{
    memset(&stBagItemNumList, 0, sizeof(stBagItemNumList));

    UINT16 wItemNum = 0;
    CItem* poItem = m_mapItem.GetFistData();
    do
    {
        if(NULL == poItem){
            break;
        }

        DT_ITEM_NUM_DATA_CLI &stItemNum = stBagItemNumList.astItemList[wItemNum];
        switch(poItem->GetItemKind())
        {
        case EIK_EQUIP:
            {
                CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
                if(NULL == poEquip){
                    continue;
                }

                stItemNum.wItemID = poEquip->GetItemKindID();
                stItemNum.wItemIdx = poEquip->GetEquipIdx();
                stItemNum.wItemNum = 1;
                wItemNum++;
            }
            break;
        case EIK_GOODS:
            {
                CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
                if(NULL == poGoods)
                {
                    continue;
                }
                stItemNum.wItemID = poGoods->GetItemKindID();
                stItemNum.wItemNum = poGoods->GetCurPileNum();
                wItemNum++;
            }
            break;
        case EIK_JEWELRY:
            {
                CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
                if(NULL == poJewelry){
                    continue;
                }

                stItemNum.wItemID = poJewelry->GetItemKindID();
                stItemNum.wItemIdx = poJewelry->GetJewelryIdx();
                stItemNum.wItemNum = 1;
                wItemNum++;
            }
            break;
        }
    }
    while ((poItem = m_mapItem.GetNextData()) && (wItemNum < MAX_BAG_OPEN_NUM_NEW));
    stBagItemNumList.wItemNum = wItemNum;
}

//获取是背包否有装备
BOOL	CBag::HaveEquip()
{
    CItem* poItem = m_mapItemPos.GetFistData();
    do
    {
        if(NULL == poItem)
        {
            break;
        }
        switch(poItem->GetItemKind())
        {
        case EIK_EQUIP:
        {
            CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
            if(NULL == poEquip)
            {
                continue;
            }
            return TRUE;
        }
        break;
        }
    }
    while (poItem = m_mapItemPos.GetNextData());
    return FALSE;
}

VOID CBag::GetXiangqianGemList( DT_ITEM_DATA_LIST_CLI2& stBagItemInfo )
{
    memset(&stBagItemInfo, 0x00, sizeof(DT_ITEM_DATA_CLI2));
    CItem* poItem = m_mapItemPos.GetFistData();

    set<UINT16> setXiangqianGemType;
    do
    {
        if(NULL == poItem)
        {
            break;
        }

        switch(poItem->GetItemKind())
        {
        case EIK_GOODS:
        {
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(NULL == poGoods)
            {
                continue;
            }

            //返回宝石和宝石制作卷
            if( (poGoods->GetGoodsMainKindID() != EGMK_XIANGQIANGEM) &&
                    (poGoods->GetGoodsMainKindID() != EGMK_GEMMAKER) )
            {
                continue;
            }

            stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].byItemType = EIK_GOODS;
            DT_GOODS_DATA_CLI& stGoods = stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].stItemInfo.stGoods ;
            poGoods->GetDT_GOODS_DATA_CLI(stGoods);
            ++stBagItemInfo.byItemNum;
            setXiangqianGemType.insert( stGoods.wKindID );
        }
        break;
        default:
            break;
        }
    }
    while (poItem = m_mapItemPos.GetNextData());

    //如果有装备的宝石不存在，则添加叠加数为0的宝石,为了让客户端能方便显示镶嵌的宝石属性
    CPlayerEquipShmemMap &mapEquip = m_poOwner->GetAllEquip();

    for( UINT32 dwIndex = 0; dwIndex < mapEquip.Size(); ++dwIndex )
    {
        CEquip *pEquip = mapEquip.GetDataByIndex( dwIndex );
        if( NULL == pEquip )
        {
            continue;
        }

        DT_EQUIP_DATA_CLI data = {0};
        pEquip->GetDT_EQUIP_DATA_CLI( data );

        for( UINT32 dwXiangIndex = 0; dwXiangIndex < MAX_XIANGQIAN_NUM; ++dwXiangIndex )
        {
            UINT16 wKindID = data.astXiangqianList[dwXiangIndex].wKindID;
            if( 0 == wKindID )
            {
                continue;
            }

            SGoodsProp *pProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey( wKindID );
            if( 0 == pProp )
            {
                continue;
            }

            //如果存在则 不补充，跳过
            if( setXiangqianGemType.find( pProp->wKindID ) != setXiangqianGemType.end() )
            {
                continue;
            }

            stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].byItemType = EIK_GOODS;
            CGoodsPropMgr::Instance()->FillGoodsData( pProp, stBagItemInfo.astItemList[stBagItemInfo.byItemNum].stItemInfo.stGoods, 0 );
            setXiangqianGemType.insert( stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].stItemInfo.stGoods.wKindID );
            ++stBagItemInfo.byItemNum;
        }
    }
}

VOID CBag::GetXiangqianGemList( DT_ITEM_DATA_LIST_CLI3& stBagItemInfo )
{
	memset(&stBagItemInfo, 0x00, sizeof(DT_ITEM_DATA_CLI3));
	CItem* poItem = m_mapItemPos.GetFistData();

	set<UINT16> setXiangqianGemType;
	do
	{
		if(NULL == poItem)
		{
			break;
		}

		switch(poItem->GetItemKind())
		{
		case EIK_GOODS:
			{
				CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
				if(NULL == poGoods)
				{
					continue;
				}

				//返回宝石和宝石制作卷
				if( (poGoods->GetGoodsMainKindID() != EGMK_XIANGQIANGEM) &&
					(poGoods->GetGoodsMainKindID() != EGMK_GEMMAKER) )
				{
					continue;
				}

				stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].byItemType = EIK_GOODS;
				DT_GOODS_DATA_CLI& stGoods = stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].stItemInfo.stGoods ;
				poGoods->GetDT_GOODS_DATA_CLI(stGoods);
				++stBagItemInfo.byItemNum;
				setXiangqianGemType.insert( stGoods.wKindID );
			}
			break;
		default:
			break;
		}
	}
	while (poItem = m_mapItemPos.GetNextData());

	//如果有装备的宝石不存在，则添加叠加数为0的宝石,为了让客户端能方便显示镶嵌的宝石属性
	CPlayerEquipShmemMap &mapEquip = m_poOwner->GetAllEquip();

	for( UINT32 dwIndex = 0; dwIndex < mapEquip.Size(); ++dwIndex )
	{
		CEquip *pEquip = mapEquip.GetDataByIndex( dwIndex );
		if( NULL == pEquip )
		{
			continue;
		}

		DT_EQUIP_DATA_CLI data = {0};
		pEquip->GetDT_EQUIP_DATA_CLI( data );

		for( UINT32 dwXiangIndex = 0; dwXiangIndex < MAX_XIANGQIAN_NUM; ++dwXiangIndex )
		{
			UINT16 wKindID = data.astXiangqianList[dwXiangIndex].wKindID;
			if( 0 == wKindID )
			{
				continue;
			}

			SGoodsProp *pProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey( wKindID );
			if( 0 == pProp )
			{
				continue;
			}

			//如果存在则 不补充，跳过
			if( setXiangqianGemType.find( pProp->wKindID ) != setXiangqianGemType.end() )
			{
				continue;
			}

			stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].byItemType = EIK_GOODS;
			CGoodsPropMgr::Instance()->FillGoodsData( pProp, stBagItemInfo.astItemList[stBagItemInfo.byItemNum].stItemInfo.stGoods, 0 );
			setXiangqianGemType.insert( stBagItemInfo.astItemList[ stBagItemInfo.byItemNum ].stItemInfo.stGoods.wKindID );
			++stBagItemInfo.byItemNum;
		}
	}
}

VOID	 CBag::GetBagItem(DT_ITEM_DATA_CLI2& stDT_ITEM_DATA_CLI2, UINT16 wKindID)
{
    memset(&stDT_ITEM_DATA_CLI2, 0x00, sizeof(DT_ITEM_DATA_CLI2));
    CItem* poItem = m_mapItemPos.GetFistData();
    do
    {
        if(NULL == poItem)
        {
            break;
        }

        if(wKindID != poItem->GetItemKindID())
        {
            continue;
        }

        switch(poItem->GetItemKind())
        {
        case EIK_GOODS:
        {
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(NULL == poGoods)
            {
                continue;
            }
            stDT_ITEM_DATA_CLI2.byItemType = EIK_GOODS;
            DT_GOODS_DATA_CLI& stGoods = stDT_ITEM_DATA_CLI2.stItemInfo.stGoods ;
            poGoods->GetDT_GOODS_DATA_CLI(stGoods);
            return;
        }
        default:
            break;
        }
    }
    while (poItem = m_mapItemPos.GetNextData());

}


CGoods* CBag::GetGoodsItem(UINT16 wItemKindID)
{
    return m_mapKindID2Goods.GetData(wItemKindID);
}

CEquip*	CBag::GetEquipItem(UINT16 wItemKindID)
{
    return m_mapKindID2Equip.GetData(wItemKindID);
}

UINT16 CBag::GetGoodsPileNum(UINT16 wItemKindID)
{
    CGoods* poGoods = m_mapKindID2Goods.GetData(wItemKindID);
    if(poGoods)
    {
        return poGoods->GetCurPileNum();
    }

    return 0;
}

CItem*	CBag::GetItem(UINT64 qwMemID)
{
	return m_mapItem.GetData(qwMemID);
}

VOID CBag::GetEquipByPlayerPanel(DT_ITEM_DATA_LIST_CLI4& stItemData)
{
	//先获取装备
	CEquip* poEquip = m_mapKindID2Equip.GetFistData();
	UINT16 wItemNum = 0;
	while(poEquip && (wItemNum < MAX_BAG_OPEN_NUM_NEW))
	{
		stItemData.astItemList[wItemNum].byItemType = EIK_EQUIP;
		poEquip->GetDT_EQUIP_DATA_CLI_with_enchant_lvl(stItemData.astItemList[wItemNum].stItemInfo.stEquip);
		poEquip = m_mapKindID2Equip.GetNextData();
		wItemNum++;
	}
	vector<DT_GOODS_DATA_CLI> vecDebris; //装备碎片
	vector<DT_GOODS_DATA_CLI> vecAdvance; //进阶石
	//再获取进阶石
	CGoods* poGoods = m_mapKindID2Goods.GetFistData();
	while((poGoods) && (wItemNum < MAX_BAG_OPEN_NUM_NEW))
	{
		DT_GOODS_DATA_CLI stGoodsData = {0};

		SGoodsProp *poGoodsProp = poGoods->GetGoodsProp();
		if (NULL == poGoodsProp)
		{
			break;
		}

		//进阶石
		if ( poGoodsProp->byMainKindID == EGMK_EQUIP_ADVANCED )
		{
			poGoods->GetDT_GOODS_DATA_CLI(stGoodsData);
			vecAdvance.push_back(stGoodsData);
		}
		//装备碎片
		else if ( poGoodsProp->byMainKindID == EGMK_EQUIP_DEBRIS )
		{
			poGoods->GetDT_GOODS_DATA_CLI(stGoodsData);
			vecDebris.push_back(stGoodsData);
		}

		poGoods = m_mapKindID2Goods.GetNextData();
	}

	vector<DT_GOODS_DATA_CLI>::iterator itr;
	for ( itr = vecAdvance.begin(); itr != vecAdvance.end(); itr++)
	{
		stItemData.astItemList[wItemNum].byItemType = EIK_GOODS;
		DT_GOODS_DATA_CLI& stDT_GOOD_DATA_CLI = stItemData.astItemList[wItemNum].stItemInfo.stGoods;
		stDT_GOOD_DATA_CLI = *itr;
		wItemNum++;
	}

	for ( itr = vecDebris.begin(); itr != vecDebris.end(); itr++)
	{
		stItemData.astItemList[wItemNum].byItemType = EIK_GOODS;
		DT_GOODS_DATA_CLI& stDT_GOOD_DATA_CLI = stItemData.astItemList[wItemNum].stItemInfo.stGoods;
		stDT_GOOD_DATA_CLI = *itr;
		wItemNum++;
	}
	//获取碎片
	stItemData.wItemNum = wItemNum ;
}

VOID CBag::GetEquipByPlayerPanel(DT_ITEM_DATA_LIST_CLI3& stItemData)
{
	//先获取装备
	CEquip* poEquip = m_mapKindID2Equip.GetFistData();
	UINT8 byItemNum = 0;
	while(poEquip && (byItemNum < MAX_BAG_OPEN_NUM))
	{
		stItemData.astItemList[byItemNum].byItemType = EIK_EQUIP;
		poEquip->GetDT_EQUIP_DATA_CLI(stItemData.astItemList[byItemNum].stItemInfo.stEquip);
		poEquip = m_mapKindID2Equip.GetNextData();
		byItemNum++;
	}
	vector<DT_GOODS_DATA_CLI> vecDebris; //装备碎片
	vector<DT_GOODS_DATA_CLI> vecAdvance; //进阶石
	//再获取进阶石
	CGoods* poGoods = m_mapKindID2Goods.GetFistData();
	while((poGoods) && (byItemNum < MAX_BAG_OPEN_NUM))
	{
		DT_GOODS_DATA_CLI stGoodsData = {0};
		
		SGoodsProp *poGoodsProp = poGoods->GetGoodsProp();
		if (NULL == poGoodsProp)
		{
			break;
		}
		
		//进阶石
		if ( poGoodsProp->byMainKindID == EGMK_EQUIP_ADVANCED )
		{
			poGoods->GetDT_GOODS_DATA_CLI(stGoodsData);
			vecAdvance.push_back(stGoodsData);
		}
		//装备碎片
		else if ( poGoodsProp->byMainKindID == EGMK_EQUIP_DEBRIS )
		{
			poGoods->GetDT_GOODS_DATA_CLI(stGoodsData);
			vecDebris.push_back(stGoodsData);
		}

		poGoods = m_mapKindID2Goods.GetNextData();
	}

	vector<DT_GOODS_DATA_CLI>::iterator itr;
	for ( itr = vecAdvance.begin(); itr != vecAdvance.end(); itr++)
	{
		stItemData.astItemList[byItemNum].byItemType = EIK_GOODS;
		DT_GOODS_DATA_CLI& stDT_GOOD_DATA_CLI = stItemData.astItemList[byItemNum].stItemInfo.stGoods;
		stDT_GOOD_DATA_CLI = *itr;
		byItemNum++;
	}

	for ( itr = vecDebris.begin(); itr != vecDebris.end(); itr++)
	{
		stItemData.astItemList[byItemNum].byItemType = EIK_GOODS;
		DT_GOODS_DATA_CLI& stDT_GOOD_DATA_CLI = stItemData.astItemList[byItemNum].stItemInfo.stGoods;
		stDT_GOOD_DATA_CLI = *itr;
		byItemNum++;
	}
	//获取碎片
	stItemData.byItemNum = byItemNum ;
}

VOID CBag::GetDT_JEWELRY_DATA_LST_CLI(DT_JEWELRY_DATA_LST_CLI& stItemData)
{
	CJewelry* poJewelry = m_mapKindID2Jewery.GetFistData();
	UINT16 wJewelryNum = 0;
	while(poJewelry && wJewelryNum < MAX_BAG_OPEN_NUM)
	{
		poJewelry->GetJewelryDataCli(stItemData.astJewelryList[wJewelryNum]);
		wJewelryNum++;
		poJewelry = m_mapKindID2Jewery.GetNextData();
	}

	stItemData.wJewelryNum = wJewelryNum;
}

VOID CBag::GetDT_JEWELRY_DATA_LST_CLI(DT_ITEM_DATA_LIST_CLI3& stItemData)
{
	UINT8 byJewelryNum = 0;
	CJewelry* poJewelry = m_mapKindID2Jewery.GetFistData();
	while(poJewelry && byJewelryNum < MAX_BAG_OPEN_NUM)
	{
		stItemData.astItemList[byJewelryNum].byItemType = EIK_JEWELRY;
		poJewelry->GetJewelryDataCli(stItemData.astItemList[byJewelryNum].stItemInfo.stJewelry);
		byJewelryNum++;
		poJewelry = m_mapKindID2Jewery.GetNextData();
	}

	stItemData.byItemNum = byJewelryNum;
}

VOID CBag::GetAboutJewelryLstCli(DT_ITEM_DATA_LIST_CLI4& stItemData)
{
	CJewelry* poJewelry = m_mapKindID2Jewery.GetFistData();
	UINT16 wNum = 0;
	while((poJewelry) && (wNum < MAX_BAG_OPEN_NUM_NEW))
	{
		stItemData.astItemList[wNum].byItemType = EIK_JEWELRY;
		poJewelry->GetJewelryDataCli(stItemData.astItemList[wNum].stItemInfo.stJewelry);
		wNum++;
		poJewelry = m_mapKindID2Jewery.GetNextData();
	}

	//再获取进阶石
	CGoods* poGoods = m_mapKindID2Goods.GetFistData();
	while((poGoods) && (wNum < MAX_BAG_OPEN_NUM_NEW))
	{
		DT_GOODS_DATA_CLI stGoodsData = {0};

		SGoodsProp *poGoodsProp = poGoods->GetGoodsProp();
		if (NULL == poGoodsProp)
		{
			break;
		}

		//进阶石
		if ( poGoodsProp->byMainKindID == EGMK_JEWELRY_ADVANCED )
		{
			stItemData.astItemList[wNum].byItemType = EIK_GOODS;
			poGoods->GetDT_GOODS_DATA_CLI(stItemData.astItemList[wNum].stItemInfo.stGoods);
			wNum++;
		}
		poGoods = m_mapKindID2Goods.GetNextData();
	}

	stItemData.wItemNum = wNum;
}

VOID CBag::GetAboutJewelryLstCli(DT_ITEM_DATA_LIST_CLI3& stItemData)
{
	CJewelry* poJewelry = m_mapKindID2Jewery.GetFistData();
	UINT8 byNum = 0;
	while((poJewelry) && (byNum < MAX_BAG_OPEN_NUM))
	{
		stItemData.astItemList[byNum].byItemType = EIK_JEWELRY;
		poJewelry->GetJewelryDataCli(stItemData.astItemList[byNum].stItemInfo.stJewelry);
		byNum++;
		poJewelry = m_mapKindID2Jewery.GetNextData();
	}

	//再获取进阶石
	CGoods* poGoods = m_mapKindID2Goods.GetFistData();
	while((poGoods) && (byNum < MAX_BAG_OPEN_NUM))
	{
		DT_GOODS_DATA_CLI stGoodsData = {0};

		SGoodsProp *poGoodsProp = poGoods->GetGoodsProp();
		if (NULL == poGoodsProp)
		{
			break;
		}

		//进阶石
		if ( poGoodsProp->byMainKindID == EGMK_JEWELRY_ADVANCED )
		{
			stItemData.astItemList[byNum].byItemType = EIK_GOODS;
			poGoods->GetDT_GOODS_DATA_CLI(stItemData.astItemList[byNum].stItemInfo.stGoods);
			byNum++;
		}
		poGoods = m_mapKindID2Goods.GetNextData();
	}

	stItemData.byItemNum = byNum;
}

VOID CBag::GetJewelryStrengthInfo(DT_BAG_JEWELRY_STRENGTH_INFO& stIfno)
{
	UINT8 byJewelry = 0;
	CJewelry* poJewelry = m_mapKindID2Jewery.GetFistData();
	while(poJewelry)
	{
		poJewelry->GetJewelryStrength(stIfno.astJewelryList[byJewelry], stIfno.astStrengthJewelryList[byJewelry]);
		if( !poJewelry->GetStrengItemInfo(stIfno.astStringth[byJewelry]))
		{
			stIfno.abyMaxMark[byJewelry] = ESM_YES;
		}
		
		byJewelry++;
		poJewelry = m_mapKindID2Jewery.GetNextData();
	}
	stIfno.byJewelryNum = byJewelry;
}

BOOL CBag::IsBagFull() {
	SBagProp& pstBagProp = CBagPropMgr::Instance()->GetBagProp();
	return (UINT16)(m_mapItem.Size()) >= pstBagProp.wTotalBagNum;
}

BOOL CBag::DelGoodItem(UINT16 usItemKindID, UINT32 unDelNum) {
	CItem* pGoodItem = m_mapKindID2Goods.GetData(usItemKindID);
	if (NULL == pGoodItem) {
		return FALSE;
	}
	
	if (pGoodItem->GetCurPileNum() < unDelNum) {
		return FALSE;
	}

	if (pGoodItem->GetCurPileNum() == unDelNum) {
		if (!DelItem(pGoodItem)) {
			return FALSE;
		}
	} else {
		pGoodItem->DecPileNum(unDelNum, CRecordMgr::EDIT_SOUL_METERIAL);
	}

	return TRUE;
}
