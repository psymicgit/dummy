
#include "itemmgr.h"
#include "itemcomposepropmgr.h"
#include <sdstring.h>
#include <logic/item/equip/equippropmgr.h>
#include <logic/item/equip/strengthenpropmgr.h>
#include <logic/item/equip/openlightpropmgr.h>
#include <logic/item/goods/goodspropmgr.h>
#include <logic/item/itemmakemgr.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/errdef.h>
#include <logic/hero/heromgr.h>
#include <logic/player/player.h>
#include <common/server/utility.h>
#include <logic/crop/cropseedpropmgr.h>
#include <logic/record/recordmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/base/basedefine.h>
#include <logic/base/logicfunc.h>
#include <logic/bag/bag.h>
#include <logic/task/taskmgr.h>
#include <logic/item/jewelry/jewelrypropmgr.h>
#include <logic/item/equip/equipmakemgr.h>
#include "logic/fly/flypropmgr.h"

IMPLEMENT_SINGLETON(CItemMgr)

CItemMgr::CItemMgr()
{

}

CItemMgr::~CItemMgr()
{
    //CBoxPropMgr::DestroyInstance();
    CItemComposePropMgr::DestroyInstance();
    CEquipPropMgr::DestroyInstance();
    CGoodsPropMgr::DestroyInstance();
    CEquipStrengthenPropMgr::DestroyInstance();
}

BOOL CItemMgr::Init()
{
    //读取配置文件

    // 初始化CEquipPropMgr管理器
    if(FALSE == CEquipPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CEquipPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CEquipPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CEquipPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }


    // 初始化CGoodsPropMgr管理器
    if(FALSE == CGoodsPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGoodsPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CGoodsPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGoodsPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    // 初始化CEquipStrengthenPropMgr管理器
    if(FALSE == CEquipStrengthenPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CEquipStrengthenPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CEquipStrengthenPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CEquipStrengthenPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    // 初始化开光配置
    if(FALSE == COpenLightPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: COpenLightPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == COpenLightPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: COpenLightPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    // 初始化CItemComposePropMgr管理器
    if(FALSE == CItemComposePropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CItemComposePropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CItemComposePropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CItemComposePropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    // 初始化CItemMakeMgr管理器
    if(FALSE == CItemMakeMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CItemMakeMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CItemMakeMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CItemMakeMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

	if ( FALSE == CJewelryPropMgr::CreateInstance())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CJewelryPropMgr::CreateInstance failed!"), MSG_MARK);
		return FALSE;
	}
	if(FALSE == CJewelryPropMgr::Instance()->Init())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CJewelryPropMgr Init failed!"), MSG_MARK);
		return FALSE;
	}
    return TRUE;

}


VOID CItemMgr::UnInit()
{
    m_mapID2Item.clear();

    //////////////////////////////////////////////////////////////////////////


    //释放各个管理器

    //CBoxPropMgr::Instance()->UnInit();
    CItemComposePropMgr::Instance()->UnInit();
    CEquipPropMgr::Instance()->UnInit();
    CGoodsPropMgr::Instance()->UnInit();
    CEquipStrengthenPropMgr::Instance()->UnInit();
    COpenLightPropMgr::Instance()->UnInit();
    CItemMakeMgr::Instance()->UnInit();

}



CItem* CItemMgr::FindItem(UINT32 dwPlayerID, UINT16 wKindID, UINT16 wIdx)
{
    UINT64 qwKey = CLogicFunc::GetItemKey(dwPlayerID, wKindID, wIdx);
    CID2ItemMapItr itr = m_mapID2Item.find(qwKey);
    if(itr != m_mapID2Item.end())
    {
        return itr->second;
    }

    return NULL;
}

/*
CEquip* CItemMgr::FindEquip(UINT32 dwPlayerID, UINT16 wKindID, UINT16 wIdx)
{
    UINT64 qwKey = CLogicFunc::GetItemKey(dwPlayerID, wKindID, wIdx);
    CID2ItemMapItr itr = m_mapID2Item.find(qwKey);
    if(itr != m_mapID2Item.end())
    {
        return dynamic_cast<CEquip*>(itr->second);
    }

    return NULL;
}
*/

//创建道具，并放入背包
CItem* CItemMgr::CreateItemIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, UINT16 wLevel, CRecordMgr::EAddItemType emOpType, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4)
{
    if((NULL == poPlayer) || (0 == wPileNum))
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }

    UINT8 byItemKind = GetItemKindBYKindID(wKindID);

    switch(byItemKind)
    {
    case EIK_EQUIP:
    {
        return CreateEquipIntoBag(poPlayer, wKindID, wPileNum, eRet, wLevel, emOpType, qwParam2, qwParam3, qwParam4);
    }
    break;
    case EIK_GOODS:
    {
        return CreateGoodsIntoBag(poPlayer, wKindID, wPileNum, eRet, emOpType, qwParam2, qwParam3, qwParam4);
    }
    break;
	case EIK_JEWELRY:
	{
		return CreateJewelryIntoBag(poPlayer, wKindID, wPileNum, eRet, wLevel, emOpType);//kindID为品质
	}
	break;
    default:
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }

    eRet = ECIR_FAILED_OTHER;
    return NULL;
}


CItem* CItemMgr::CreateEquipIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, UINT16 wLevel,  CRecordMgr::EAddItemType emOpType, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4)
{
    if(NULL == poPlayer)
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }
    CItem* poItem = m_oEquipShmemObjMgr.Alloc();

    DT_EQUIP_DATA stEquip;
    memset(&stEquip, 0, sizeof(stEquip));

    SEquipProperty sEquipProperty;
    sEquipProperty.pstEquip = &stEquip;

    stEquip.wKindID = wKindID;
    stEquip.wLevel = wLevel;
    stEquip.wIdx = poPlayer->GetNextEquipIdx(wKindID);
    UINT16 wItemIdx = stEquip.wIdx;
    VOID* pData = (VOID*)(&sEquipProperty);

    if(NULL == poItem)
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }
    UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wKindID, wItemIdx);
    poItem->SetMemID(qwKey);
    
    if(FALSE == poItem->Init(pData, poPlayer))
    {
        //清除已经创建的ITEM
        RemoveItem(poItem);
        poPlayer->ReturnNextEquipIdx(wKindID, wItemIdx);
        eRet = ECIR_FAILED_KIND_INVALID;
        return NULL;
    }

    //放入背包中
    CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
    if(NULL == poEquip)
    {
        //清除已经创建的ITEM
        RemoveItem(poItem);
        poPlayer->ReturnNextEquipIdx(wKindID, wItemIdx);
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }
    CBag& oBag = poPlayer->GetBag();
    if(!oBag.AddItem(poItem))
    {
        //清除已经创建的ITEM
        RemoveItem(poItem);
        poPlayer->ReturnNextEquipIdx(wKindID, wItemIdx);
        eRet = ECIR_FAILED_BAG_FULL;
        return NULL;
    }
    poPlayer->OnAddEquip(poEquip);
    CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_ADDITEM, emOpType, poEquip->GetItemKindID(), poEquip->GetCurPileNum(), 
        poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1, qwParam2, qwParam3, qwParam4);

	m_mapID2Item[poItem->GetMemID()] = poItem;
    eRet = ECIR_SUCCESS;
    return poItem;
}

UINT16 CItemMgr::GetInnerBoxRateKindID(UINT16 wInnerBoxKindID)
{
    SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wInnerBoxKindID);
    if(NULL == pstItemComposeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL!"), MSG_MARK);
        RETURN_OTHER_ERR;
    }

    CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
    if(0 == mapComposeItem.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, wInnerBoxKindID);
        RETURN_OTHER_ERR;
    }

    CRandomVec vecRandom;
    for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++)
    {
        vecRandom.push_back(itr->second.wRate);
    }

    //随机
    INT32 nRandomHitIdx = GetRandomIdx(vecRandom, 10000);
    if(RANDOM_UNHIT == nRandomHitIdx)
    {
        return 0;
    }

    UINT8 byIdx = 0;
    for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++, byIdx++)
    {
        if(byIdx == nRandomHitIdx)
        {
            return itr->first;
        }
    }

    return 0;
}


CItem* CItemMgr::CreateGoodsIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet,  CRecordMgr::EAddItemType emOpType, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4)
{
    if(NULL == poPlayer)
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }
    eRet = ECIR_SUCCESS;
    VOID* pData = NULL;
    SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
    if(NULL == pstGoodsProp)
    {
        eRet = ECIR_FAILED_KIND_INVALID;
        return NULL;
    }
    //若是内部宝箱，则直接打开获取给的物品
    if(EGMK_INNERBOX == pstGoodsProp->byMainKindID)
    {
        wKindID = GetInnerBoxRateKindID(wKindID);
        if(0 == wKindID)
        {
            eRet = ECIR_FAILED_HIT;
            return NULL;
        }

        if(EIK_EQUIP == GetItemKindBYKindID(wKindID))
        {
            return CreateEquipIntoBag(poPlayer, wKindID, wPileNum, eRet, 0, emOpType, qwParam2, qwParam3, qwParam4);
        }

		if ( EIK_JEWELRY == GetItemKindBYKindID(wKindID))
		{
			return CreateJewelryIntoBag(poPlayer, wKindID, wPileNum, eRet, 0, emOpType);
		}
        pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
        if(NULL == pstGoodsProp)
        {
            eRet = ECIR_FAILED_KIND_INVALID;
            return NULL;
        }
    }

    CBag& oBag = poPlayer->GetBag();
    CItem* poItem = oBag.GetGoodsItem(wKindID);
    //该类型在背包中存在，则只+叠加数即可
    if(poItem)
    {
        //增加操作
        poItem->AddPileNum(wPileNum, emOpType);
        eRet = ECIR_SUCCESS;

        return poItem;
    }
    else
    {
        //查找是否存在管理器中而不在背包中(正在被删除物品)
        CItem* poItem = NULL;
        BOOL bAllocFlag = TRUE;
        UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wKindID);
        poItem = FindItem(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wKindID, 1);
        if(NULL == poItem)
        {
            poItem = m_oGoodsShmemObjMgr.Alloc();
            bAllocFlag = TRUE;
        }
        if(NULL == poItem)
        {
            eRet = ECIR_FAILED_OTHER;
            return NULL;
        }
        DT_GOODS_DATA stDT_GOODS_DATA;
        memset(&stDT_GOODS_DATA, 0, sizeof(DT_GOODS_DATA));
        stDT_GOODS_DATA.wKindID = wKindID;
        stDT_GOODS_DATA.wPileCount = wPileNum;
        stDT_GOODS_DATA.qwLastUseTime = 0;
        pData = (VOID*)(&stDT_GOODS_DATA);

        poItem->SetMemID(qwKey);
        if(FALSE == poItem->Init(pData, poPlayer))
        {
            //清除已经创建的ITEM
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(poGoods && bAllocFlag)
            {
                m_oGoodsShmemObjMgr.Free(poGoods);
            }
            eRet = ECIR_FAILED_KIND_INVALID;
            return NULL;
        }
        //背包满
        if(!oBag.AddItem(poItem))
        {
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(poGoods && bAllocFlag)
            {
                m_oGoodsShmemObjMgr.Free(poGoods);
            }
            eRet = ECIR_FAILED_BAG_FULL;
            return NULL;
        }
        //放背包成功
        else
        {
            //poItem->SetObjDataSaveType(EODST_INSERT);
            //poItem->SetSaveState(ESS_WANT_SAVE);

            if(bAllocFlag)
            {
                m_mapID2Item[qwKey] = poItem;
            }

            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if( poGoods )
            {
                CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_ADDITEM, emOpType, poGoods->GetItemKindID(), poGoods->GetCurPileNum(),
                                                    poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, wPileNum, qwParam2, qwParam3, qwParam4);

            }
            else
            {
                SYS_CRITICAL( "[%s: %d] cast CGoods type error", MSG_MARK );
            }

            return poItem;

        }

        return NULL;
    } // else //该类型在背包中不存在

    return NULL;
}


//数据还原
CItem* CItemMgr::AttachItem(CPlayer* poPlayer, UINT16 wKindID, VOID* pData)
{
    UINT8 byItemKind = GetItemKindBYKindID(wKindID);
    CItem* poItem = NULL;
    UINT16 wItemIdx = 0;
    switch(byItemKind)
    {
    case EIK_EQUIP:
    {
        poItem = m_oEquipShmemObjMgr.Alloc();
        DT_EQUIP_DATA* pstDT_EQUIP_DATA = ((SEquipProperty*)pData)->pstEquip;
		pstDT_EQUIP_DATA->wIdx = poPlayer->GetNextEquipIdx(wKindID);
        wItemIdx = pstDT_EQUIP_DATA->wIdx;
    }
    break;
    case EIK_GOODS:
    {
        poItem = m_oGoodsShmemObjMgr.Alloc();
        DT_GOODS_DATA* pstDT_GOODS_DATA = (DT_GOODS_DATA*)pData;
        wItemIdx = 1;
    }
    break;
	case EIK_JEWELRY:
	{
		poItem = m_oJewelryShmemObjMgr.Alloc();
		DT_JEWELRY_DATA* pstDT_JEWELRY_DATA = (DT_JEWELRY_DATA*)pData;
		pstDT_JEWELRY_DATA->wIdx = poPlayer->GetNextJeweryIdx(wKindID);
		wItemIdx = pstDT_JEWELRY_DATA->wIdx;
	}
	break;
    default:
        return NULL;
    }

    if(NULL == poItem)
    {
        return NULL;
    }
    UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wKindID, wItemIdx);
    poItem->SetMemID(qwKey);
    m_mapID2Item[qwKey] = poItem;
    if(FALSE == poItem->Init(pData, poPlayer))
    {
        //清除已经创建的ITEM
        RemoveItem(poItem);
        return NULL;
    }

    return poItem;
}
VOID CItemMgr::RemoveItem(CItem* poItem, CPlayer* poPlayer, CRecordMgr::EDecItemType eDecType, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4)
{
	RemoveItem( poItem);
	CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, eDecType, poItem->GetItemKindID(), 0, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 
		qwParam1, qwParam2, qwParam3, qwParam4);
}

VOID CItemMgr::RemoveItem(CItem* poItem)
{
    if(NULL == poItem)
    {
        return;
    }


    CID2ItemMapItr itr = m_mapID2Item.find(poItem->GetMemID());
    if (m_mapID2Item.end() != itr)
    {
        CItem* poItem = itr->second;
        switch(poItem->GetItemKind())
        {
        case EIK_EQUIP:
        {
            CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
            if(poEquip)
            {
                m_oEquipShmemObjMgr.Free(poEquip);
                m_mapID2Item.erase(itr);
            }
        }
        break;
        case EIK_GOODS:
        {
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(poGoods)
            {
                m_oGoodsShmemObjMgr.Free(poGoods);
                m_mapID2Item.erase(itr);
            }
        }
        break;
		case EIK_JEWELRY:
		{
			CJewelry* pojewelry = dynamic_cast<CJewelry*>(poItem);
			if(pojewelry)
			{
				m_oJewelryShmemObjMgr.Free(pojewelry);
				m_mapID2Item.erase(itr);
			}
		}
		break;
        default:
            break;
        }
    }
}


UINT8 CItemMgr::GetItemKindBYKindID(UINT16 wKindID)
{
    if((wKindID >= EQUIP_BEGIN_KINDID) && (wKindID <= EQUIP_END_KINDID))
    {
        return EIK_EQUIP;
    }
    else if ((wKindID >= GOODS_BEGIN_KINDID) && (wKindID <= GOODS_END_KINDID))
    {
        return EIK_GOODS;
    }
    else if ( (wKindID >= JEWELRY_BEGIN_KINDID) && (wKindID <= JEWELRY_END_KINDID) )
    {
		return EIK_JEWELRY;
    }

    return EIK_GOODS;
}

VOID CItemMgr::GetDT_ITEM_DATA_CLI2(CItem* poItem, DT_ITEM_DATA_CLI2& stDT_ITEM_DATA_CLI2)
{
    memset(&stDT_ITEM_DATA_CLI2, 0, sizeof(stDT_ITEM_DATA_CLI2));
    if(NULL == poItem)
    {
        return;
    }

    /*
    if(EODST_DELETE == poItem->GetObjDataSaveType())
    {
    	return;
    }
    */
    switch(poItem->GetItemKind())
    {
    case EIK_EQUIP:
    {
        CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
        if(NULL == poEquip)
        {
            return;
        }
        stDT_ITEM_DATA_CLI2.byItemType = EIK_EQUIP;
        DT_EQUIP_DATA_CLI& stEquip = stDT_ITEM_DATA_CLI2.stItemInfo.stEquip ;
        poEquip->GetDT_EQUIP_DATA_CLI(stEquip);
    }
    break;
    case EIK_GOODS:
    {
        CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
        if(NULL == poGoods)
        {
            return;
        }
        stDT_ITEM_DATA_CLI2.byItemType = EIK_GOODS;
        DT_GOODS_DATA_CLI& stGoods = stDT_ITEM_DATA_CLI2.stItemInfo.stGoods ;
        poGoods->GetDT_GOODS_DATA_CLI(stGoods);
    }
    break;
    }
}

BOOL CItemMgr::GetDT_ITEM_DATA_CLI2(UINT16 wKindID, UINT16 wPileCount, DT_ITEM_DATA_CLI2& stDT_ITEM_DATA_CLI2)
{
    memset(&stDT_ITEM_DATA_CLI2, 0, sizeof(stDT_ITEM_DATA_CLI2));

    switch(GetItemKindBYKindID(wKindID))
    {
    case EIK_EQUIP:
    {
        SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(wKindID);
        if(NULL == pstProp)
        {
            return FALSE;
        }
        stDT_ITEM_DATA_CLI2.byItemType = EIK_EQUIP;
        DT_EQUIP_DATA_CLI& stEquip = stDT_ITEM_DATA_CLI2.stItemInfo.stEquip;
        stEquip.byDressPos = pstProp->byPosKindID;
        stEquip.wKindID = pstProp->wKindID;
        stEquip.wIdx = 0;
        stEquip.wLevel = 0;
        stEquip.wDressLevel = pstProp->wDressLevel;
        stEquip.stBattleAttribute = CEquip::BattleAttCountByLevel(0, 0, pstProp);
        stEquip.dwRecoverPrice = pstProp->dwRecoverPrice;
    }
    break;
    case EIK_GOODS:
    {
        SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
        if(NULL == pstProp)
        {
            return FALSE;
        }
        stDT_ITEM_DATA_CLI2.byItemType = EIK_GOODS;
        DT_GOODS_DATA_CLI& stGoods = stDT_ITEM_DATA_CLI2.stItemInfo.stGoods ;
        stGoods.wKindID = pstProp->wKindID;
        stGoods.wUseLevel = pstProp->wUseLevel;
        stGoods.wPileCount = wPileCount;
        stGoods.dwRecoverPrice = pstProp->dwRecoverPrice * wPileCount;
        stGoods.byGoodsMainKindID = pstProp->byMainKindID;
        stGoods.dwValue = pstProp->dwValue;
        stGoods.byComposeItemNum = pstProp->byComposeItemNum;
        memcpy(&stGoods.awComposeItem, &(pstProp->awComposeItem), sizeof(stGoods.awComposeItem));
        stGoods.wComposeCount = pstProp->wComposeCount;
        stGoods.wComposeKindID = pstProp->wComposeKindID;
        stGoods.dwParam1 = pstProp->dwParam1;
        stGoods.dwParam2 = pstProp->dwParam2;
        stGoods.dwParam3 = pstProp->dwParam3;
        stGoods.dwParam4 = pstProp->dwParam4;
        SCropProp *poCropProp = CCropSeedPropMgr::Instance()->GetCropProp(wKindID);
        if (NULL != poCropProp)
        {
            stGoods.bySeedFlag = 1;
            stGoods.astSeedData[0].byType = poCropProp->byType;
            stGoods.astSeedData[0].dwValue = poCropProp->dwObtainValue;
            stGoods.astSeedData[0].wKindID = poCropProp->wObtainKindID;
        }
    }
    break;
    }

    return TRUE;
}

BOOL CItemMgr::GetDT_ITEM_DATA_CLI3(UINT16 wKindID, UINT16 wPileCount, DT_ITEM_DATA_CLI3& stDT_ITEM_DATA_CLI3)
{
	memset(&stDT_ITEM_DATA_CLI3, 0, sizeof(stDT_ITEM_DATA_CLI3));

	switch(GetItemKindBYKindID(wKindID))
	{
	case EIK_EQUIP:
		{
			SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(wKindID);
			if(NULL == pstProp)
			{
				return FALSE;
			}
			stDT_ITEM_DATA_CLI3.byItemType = EIK_EQUIP;
			DT_EQUIP_DATA_CLI& stEquip = stDT_ITEM_DATA_CLI3.stItemInfo.stEquip;
			stEquip.byDressPos = pstProp->byPosKindID;
			stEquip.wKindID = pstProp->wKindID;
			stEquip.wIdx = 0;
			stEquip.wLevel = 0;
			stEquip.wDressLevel = pstProp->wDressLevel;
			stEquip.stBattleAttribute = CEquip::BattleAttCountByLevel(0, 0, pstProp);
			stEquip.dwRecoverPrice = pstProp->dwRecoverPrice;
		}
		break;
	case EIK_GOODS:
		{
			SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
			if(NULL == pstProp)
			{
				return FALSE;
			}
			stDT_ITEM_DATA_CLI3.byItemType = EIK_GOODS;
			DT_GOODS_DATA_CLI& stGoods = stDT_ITEM_DATA_CLI3.stItemInfo.stGoods ;
			stGoods.wKindID = pstProp->wKindID;
			stGoods.wUseLevel = pstProp->wUseLevel;
			stGoods.wPileCount = wPileCount;
			stGoods.dwRecoverPrice = pstProp->dwRecoverPrice * wPileCount;
			stGoods.byGoodsMainKindID = pstProp->byMainKindID;
			stGoods.dwValue = pstProp->dwValue;
			stGoods.byComposeItemNum = pstProp->byComposeItemNum;
			memcpy(&stGoods.awComposeItem, &(pstProp->awComposeItem), sizeof(stGoods.awComposeItem));
			stGoods.wComposeCount = pstProp->wComposeCount;
			stGoods.wComposeKindID = pstProp->wComposeKindID;
			stGoods.dwParam1 = pstProp->dwParam1;
			stGoods.dwParam2 = pstProp->dwParam2;
			stGoods.dwParam3 = pstProp->dwParam3;
			stGoods.dwParam4 = pstProp->dwParam4;
			SCropProp *poCropProp = CCropSeedPropMgr::Instance()->GetCropProp(wKindID);
			if (NULL != poCropProp)
			{
				stGoods.bySeedFlag = 1;
				stGoods.astSeedData[0].byType = poCropProp->byType;
				stGoods.astSeedData[0].dwValue = poCropProp->dwObtainValue;
				stGoods.astSeedData[0].wKindID = poCropProp->wObtainKindID;
			}
		}
		break;
	case EIK_JEWELRY:
		{
			//不确定
			SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(wKindID);
			if(NULL == pstProp)
			{
				return FALSE;
			}
			stDT_ITEM_DATA_CLI3.byItemType = EIK_JEWELRY;
			DT_JEWELRY_DATA_CLI& stJewelry = stDT_ITEM_DATA_CLI3.stItemInfo.stJewelry;
			stJewelry.wJewelryID = wKindID;
			
		}
		break;
	}

	return TRUE;
}

VOID CItemMgr::RemoveItem(CItemVec& vecItem)
{
    UINT16 wSize = vecItem.size();
    for(UINT16 wIdx = 0; wIdx < wSize; wIdx++)
    {
        CItem* poItem = vecItem[wIdx];
        if(NULL == poItem)
        {
            continue;
        }

        RemoveItem(poItem);
    }
    vecItem.clear();
}


UINT16 CItemMgr::Equip(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byDressType, UINT16 wDressItemKindID, UINT16 wDressItemIdx, UINT16 wUndressItemKindID, UINT16 wUndressItemIdx, DT_POWER_CHANGE_DATA& stHeroPowerInfo)
{
    //
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    CHero* poHero = poPlayer->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_EQUIP::ID_HERO_NOT_EXIST_ERR;
    }

    stHeroPowerInfo.dwOldPower = poHero->GetPower();
    UINT16 wErrCode = 0;
    switch(byDressType)
    {
    case EEDT_DRESS:
    {
        wErrCode = poHero->OnDressEquip(wDressItemKindID, wDressItemIdx);
    }
    break;
    case EEDT_UNDRESS:
    {
        wErrCode = poHero->OnUndressEquip(wUndressItemKindID, wUndressItemIdx);
    }
    break;
    case EEDT_REPLACE:
    {
        wErrCode = poHero->OnReplaceEquip(wDressItemKindID, wDressItemIdx, wUndressItemKindID, wUndressItemIdx);
    }
    break;
    default:
        SET_OTHER_ERR(wErrCode);

    }

    //记录引导
    if(ERR_EQUIP::ID_SUCCESS == wErrCode)
    {
        //重置玩家属性
        poPlayer->ResetBattleAttr();

        poPlayer->AddGuideRecord( EGF_EQUIP);
    }
    stHeroPowerInfo.dwCurPower = poHero->GetPower();

    return wErrCode;
}

UINT16 CItemMgr::QuickEquip(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byEquipNum, DT_EQUIP_ID astEquipIDLst[EQUIP_POS_NUM], DT_POWER_CHANGE_DATA& stHeroPowerInfo)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    CHero* poHero = poPlayer->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_EQUIP::ID_HERO_NOT_EXIST_ERR;
    }

    if((byEquipNum > EQUIP_POS_NUM) || (byEquipNum > (EQUIP_POS_NUM - poHero->GetEquipMap().Size())))
    {
        return ERR_EQUIP::ID_EQUIP_NUM_MISMATCH_ERR;
    }

    UINT16 wErrCode = 0;
    for(UINT8 byIdx = 0; byIdx < byEquipNum; byIdx++)
    {
        wErrCode = poHero->OnDressEquip(astEquipIDLst[byIdx].wKindID, astEquipIDLst[byIdx].wIdx);
        if(ERR_EQUIP::ID_SUCCESS != wErrCode)
        {
            //出错，恢复之前穿戴的装备
            for(UINT8 byI22 = 0; byI22 < byIdx; byI22++)
            {
                poHero->OnUndressEquip(astEquipIDLst[byIdx].wKindID, astEquipIDLst[byIdx].wIdx);
            }
            break;
        }
    }

    if(ERR_EQUIP::ID_SUCCESS == wErrCode)
    {
        //重置玩家属性
        poPlayer->ResetBattleAttr();
    }
    stHeroPowerInfo.dwCurPower = poHero->GetPower();

    return wErrCode;
}

UINT16 CItemMgr::Strengthen(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byStrengthenType, UINT16 wLevel,
                            DT_EQUIP_DATA_STRENGTHEN_CLI& stEquipStrengthenInfo, DT_EQUIP_ENCHANT_DATA_CLI &stEquipEnchantInfo, DT_EQUIP_DATA_CLI &stEquipInfo, UINT32& dwStrengthen10Coin)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    CEquip* poEquip = poPlayer->GetEquip(wEquipKindID, wIdx);
    if(NULL == poEquip)
    {
        return ERR_STRENGTHEN::ID_NOT_EXIST_ERR;
    }

    //判断是否是自己
    if(poEquip->GetOwner() != poPlayer)
    {
        return ERR_STRENGTHEN::ID_OTHER_OWNER_ERR;
    }

    UINT16 wErrCode = poEquip->Strengthen(byStrengthenType, wLevel);
    poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(stEquipStrengthenInfo);
    poEquip->GetDT_EQUIP_DATA_ENCHANT_CLI(stEquipEnchantInfo);
    poEquip->GetDT_EQUIP_DATA_CLI(stEquipInfo);
    dwStrengthen10Coin = poEquip->GetUpLevelCoin(10);

    return wErrCode;
}

UINT16 CItemMgr::Enchant(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT16 wLevel, 
    DT_EQUIP_DATA_STRENGTHEN_CLI& stEquipStrengthenInfo, DT_EQUIP_ENCHANT_DATA_CLI &stEnchantCli, DT_EQUIP_DATA_CLI_v410 &stEquipCli)
{
    if(NULL == poPlayer){
        RETURN_OTHER_ERR;
    }

    CEquip* poEquip = poPlayer->GetEquip(wEquipKindID, wIdx);
    if(NULL == poEquip){
        return ERR_ENCHANT::ID_IS_UNOPEN;
    }

    //判断是否是自己
    if(poEquip->GetOwner() != poPlayer){
        return ERR_ENCHANT::ID_IS_UNOPEN;
    }

    UINT16 wErrCode = poEquip->Enchant(wLevel);
    if (ERR_ENCHANT::ID_SUCCESS != wErrCode){
        return wErrCode;
    }

    poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(stEquipStrengthenInfo);
    poEquip->GetDT_EQUIP_DATA_ENCHANT_CLI(stEnchantCli);
    poEquip->GetDT_EQUIP_DATA_CLI_v410(stEquipCli);
    return wErrCode;
}

UINT16 CItemMgr::MakeEquip(CPlayer* poPlayer, UINT16 wEquipMakeBookID, UINT16 wBookCnt, BOOL bIsBuyMaterial)
{
    if (NULL == poPlayer->GetBuild(EBK_EQUIP_MAKE)){
        return ERR_EQUIP_MAKE::ID_IS_UNOPEN;
    }

    const SEquipMakeProp* pstProp = CEquipMakePropMgr::Instance()->GetMakeProp(wEquipMakeBookID);
    if (NULL == pstProp){
        return ERR_EQUIP_MAKE::ID_BOOK_NOT_EXIST;
    }
    
    CBag &oBag = poPlayer->GetBag();
    CGoods *poBook = oBag.GetGoodsItem(wEquipMakeBookID);
    if (NULL == poBook){
        return ERR_EQUIP_MAKE::ID_BOOK_NOT_EXIST;
    }

    DT_EQUIP_MAKE_BOOK stBookCli;
    CEquipMakeMgr::TakeEquipMakeBookInfo(*poPlayer, poBook->GetItemKindID(), stBookCli);

    if (poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < stBookCli.dwCostCoin){
        return ERR_COMMON::ID_COIN_NOT_ENOUGH;
    }

    // 检测元宝补齐材料条件
    if (bIsBuyMaterial){
        const SFlyProp &stFlyProp = CFlyPropMgr::Instance()->GetFlyProp();
        if (poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel < stFlyProp.byEquipMakeBuyMaterialVipLvl){
            return ERR_EQUIP_MAKE::ID_VIP_LEVEL_NOT_ENOUGH;
        }

        if(poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < stBookCli.dwBuyGold){
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }
    }

    // 检测材料是否足够（装备无法使用补齐元宝功能）
    for (UINT16 i = 0; i < stBookCli.stMaterials.byMaterialCnt; i++){
        DT_MATERIAL &stMaterial = stBookCli.stMaterials.astMaterialList[i];
        if (stMaterial.wCurNum < stMaterial.wRequiredNum){
            if (stMaterial.byMaterialType == EIK_EQUIP){
                return ERR_EQUIP_MAKE::ID_EQUIP_NOT_ENOUGH;
            }

            if (!bIsBuyMaterial){
                return ERR_EQUIP_MAKE::ID_MATERIAL_NOT_ENOUGH;
            }
        }
    }

    // 检测消耗材料中是否包含装备，如果未包含，则需要检测背包空间是否足够
    BOOL bCostEquip = FALSE;
    for (UINT16 i = 0; i < stBookCli.stMaterials.byMaterialCnt; i++){
        DT_MATERIAL &stMaterial = stBookCli.stMaterials.astMaterialList[i];
        if (EIK_EQUIP == stMaterial.byMaterialType){
            bCostEquip = TRUE;
            break;
        }
    }

    if (!bCostEquip){
        if (poPlayer->GetBag().IsBagFull()){
            return ERR_EQUIP_MAKE::ID_BAG_FULL;
        }
    }

    // 消耗制作材料
    BOOL bCostSucc = CEquipMakeMgr::CostMaterialList(*poPlayer, stBookCli.stMaterials, CRecordMgr::EDIT_EQUIP_MAKE, bIsBuyMaterial);
    if (!bCostSucc){
        return ERR_EQUIP_MAKE::ID_MATERIAL_NOT_ENOUGH;
    }

    if (bIsBuyMaterial){
        // 扣除元宝
        poPlayer->DecGold(stBookCli.dwBuyGold, CRecordMgr::EDGT_EQUIP_MAKE_BUY_MATERIAL);
    }

    poPlayer->DecCoin(stBookCli.dwCostCoin, CRecordMgr::EDCT_EQUIP_MAKE);

    poPlayer->GetBag().DelGoodItem(wEquipMakeBookID, 1);

    // 生成制作卷的装备
    ECreateItemRet eRet = ECIR_SUCCESS;
    CreateItemIntoBag(poPlayer, pstProp->wEquipID, 1, eRet, 0, CRecordMgr::EAIT_EQUIP_MAKE);

    if (ECIR_SUCCESS != eRet){
        if(ECIR_FAILED_BAG_FULL == eRet){
            return ERR_EQUIP_MAKE::ID_BAG_FULL;
        }

        return ERR_EQUIP_MAKE::ID_MATERIAL_NOT_ENOUGH;
    }

    return ERR_EQUIP_MAKE::ID_SUCCESS;
}

UINT16 CItemMgr::Sell(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wIdx, UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI &stItemNumList)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

	CBag& oBag = poPlayer->GetBag();
	UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetID(), wItemKindID, wIdx);
    CItem* poItem = oBag.GetItem(qwKey);
    if(NULL == poItem)
    {
        return ERR_SELL_ITEM::ID_ITEM_NOT_EXIST_ERR;
    }

    if(poPlayer != poItem->GetOwner())
    {
        return ERR_SELL_ITEM::ID_ITEM_NOT_EXIST_ERR;
    }


    CGoods *pGoods = dynamic_cast< CGoods * >( poItem );
    UINT16 wPileCount = 0;
    if( pGoods )
    {
        wPileCount = pGoods->GetDT_GOODS_DATA().wPileCount;
        wPileCount = wPileCount > wNum ? wPileCount - wNum : 0;
    }
    else
    {
        wPileCount = 0;
    }

    UINT16  wSubItemKind = poItem->GetItemKindID();
    UINT16	wRet = poItem->Sell(wNum, stItemNumList);
    if( wRet == ERR_SELL_ITEM::ID_SUCCESS )
    {
        CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_SELL, wSubItemKind, wPileCount, 
            poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, wNum);
    }
    return wRet;
}

UINT16 CItemMgr::MeltEquip(CPlayer* poPlayer, UINT8 byGoldFlag, UINT16 wItemKindID, UINT16 wIdx)//隔合器魂
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //指引的直接增加200点器魂
    if (poPlayer->CKGuide(EGF_MELTING))
    {
        poPlayer->AddScience(200, CRecordMgr::EAST_MELT, wItemKindID, wIdx);
        poPlayer->AddGuideRecord( EGF_MELTING);
        CTaskMgr::Instance()->OnMelt(poPlayer);
        return ERR_MELT_EQUIP::ID_SUCCESS;;
    }

    CItem* poItem = FindItem(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wItemKindID, wIdx);
    if(NULL == poItem)
    {
        return ERR_MELT_EQUIP::ID_BAG_EQUIP_NOT_EXIT;
    }

    if(poPlayer != poItem->GetOwner())
    {
        return ERR_MELT_EQUIP::ID_BAG_EQUIP_NOT_EXIT;
    }

    //强制类型转化
    CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
    if(NULL == poEquip)
    {
        return ERR_MELT_EQUIP::ID_BAG_EQUIP_NOT_EXIT;
    }

    if (poEquip->GetEnchantLevel() > 0){
        return ERR_MELT_EQUIP::ID_EQUIP_ENCHANT_ERR;
    }

    return poEquip->Melt(byGoldFlag);
}

UINT16	CItemMgr::MeltGood(CPlayer* poPlayer, UINT8 byGoldFlag, UINT16 wItemKindID, UINT16 wNum)//熔炼器魂
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //指引的直接增加200点器魂
    if (poPlayer->CKGuide(EGF_MELTING))
    {
        poPlayer->AddScience(200, CRecordMgr::EAST_MELT, wItemKindID, wNum);
        poPlayer->AddGuideRecord( EGF_MELTING);
        CTaskMgr::Instance()->OnMelt(poPlayer);
        return ERR_MELT_EQUIP::ID_SUCCESS;;
    }

    CGoods* poGoods = poPlayer->GetBag().GetGoodsItem(wItemKindID);
    if(NULL == poGoods)
    {
        return ERR_MELT_EQUIP::ID_BAG_GOOD_NOT_EXIT;
    }

    if(poGoods->GetCurPileNum() < wNum)
    {
        return ERR_MELT_EQUIP::ID_BAG_GOOD_NOT_ENOUGH;
    }

    for (int i = 0; i < wNum; i++)
    {
        poGoods->Melt(byGoldFlag);
    }
    return ERR_MELT_EQUIP::ID_SUCCESS;

}

UINT16 CItemMgr::DelItem(CPlayer* poPlayer, UINT16 wItemKindID)//直接删除装备，用于GM命令
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }
    UINT16		wTotal = 0;
    CID2ItemShmemMap& mapID2ItemShmem = poPlayer->GetBag().GetItemMap();
    CItem* poItem = mapID2ItemShmem.GetFistData();
    do
    {
        if(NULL == poItem)
        {
            break;
        }
        if (EIK_GOODS == poItem->GetItemKind() )
        {
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(NULL == poGoods)
            {
                continue;
            }
            //物品类型为15，
            if (15 == poGoods->GetGoodsMainKindID() && poItem->GetItemKindID() >= wItemKindID)
            {
                CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_GM, poItem->GetItemKindID(), 0, 
                    poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, poGoods->GetCurPileNum());

                poPlayer->OnDeleteItem(poGoods);

                wTotal++;
            }
        }
    }
    while (poItem = mapID2ItemShmem.GetNextData());
    return wTotal;
}

UINT16 CItemMgr::CheckMeltEquip(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wIdx, UINT32& dwCostGold)//隔合器魂
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //未指引的直接返回成功
    if (poPlayer->CKGuide(EGF_MELTING))
    {
        return ERR_MELT_EQUIP::ID_SUCCESS;
    }

    CItem* poItem = FindItem(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wItemKindID, wIdx);
    if(NULL == poItem)
    {
        return ERR_MELT_EQUIP::ID_BAG_EQUIP_NOT_EXIT;
    }

    //强制类型转化
    CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
    if(NULL == poEquip)
    {
        return ERR_MELT_EQUIP::ID_BAG_EQUIP_NOT_EXIT;
    }

    //装备上有宝石，不让熔练
    if (poEquip->GetMosaicGemNum() > 0)
    {
        return ERR_MELT_EQUIP::ID_EQUIP_HAVE_MOSAIC_GEM;
    }

    UINT32	dwMinScience = 0;
    UINT32	dwMaxScience = 0;
    poEquip->GetRecoverScience(dwMinScience, dwMaxScience);
    dwCostGold = CLuamgr::Instance()->GetMeltGold(dwMaxScience);

    return ERR_MELT_EQUIP::ID_SUCCESS;
}

UINT16 CItemMgr::CheckMeltGood(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wNum, UINT32& dwCostGold)//熔炼器魂
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //未指引的直接返回成功
    if (poPlayer->CKGuide(EGF_MELTING))
    {
        return ERR_MELT_EQUIP::ID_SUCCESS;
    }

    CGoods* poGoods = poPlayer->GetBag().GetGoodsItem(wItemKindID);
    if(NULL == poGoods)
    {
        return ERR_MELT_EQUIP::ID_BAG_EQUIP_NOT_EXIT;
    }

    UINT32	dwMinScience = 0;
    UINT32	dwMaxScience = 0;
    poGoods->GetRecoverScience(dwMinScience, dwMaxScience);
    dwCostGold = CLuamgr::Instance()->GetMeltGold(dwMaxScience) * wNum;

    return ERR_MELT_EQUIP::ID_SUCCESS;
}




CEquip* CItemMgr::CreateEquip(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum, ECreateItemRet& eRet, UINT16 wLevel)
{
    if(NULL == poPlayer)
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }
    CEquip* poEquip = m_oEquipShmemObjMgr.Alloc();

    DT_EQUIP_DATA stDT_EQUIP_DATA;
    memset(&stDT_EQUIP_DATA, 0, sizeof(DT_EQUIP_DATA));
    stDT_EQUIP_DATA.wKindID = wKindID;
    stDT_EQUIP_DATA.wLevel = wLevel;
    stDT_EQUIP_DATA.wIdx = poPlayer->GetNextEquipIdx(wKindID);
    UINT16 wItemIdx = stDT_EQUIP_DATA.wIdx;

    SEquipProperty stEquipProperty;
    stEquipProperty.pstEquip = &stDT_EQUIP_DATA;

    VOID* pData = (VOID*)(&stEquipProperty);

    if(NULL == poEquip)
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }
    UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wKindID, wItemIdx);
    poEquip->SetMemID(qwKey);
    
    if(FALSE == poEquip->Init(pData, poPlayer))
    {
        //清除已经创建的ITEM
        RemoveItem(poEquip);
        poPlayer->ReturnNextEquipIdx(wKindID, wItemIdx);
        eRet = ECIR_FAILED_KIND_INVALID;
        return NULL;
    }

	m_mapID2Item[poEquip->GetMemID()] = poEquip;
    eRet = ECIR_SUCCESS;
    return poEquip;
}

//物品使用
UINT16 CItemMgr::UseGoods(CPlayer* poPlayer, UINT16 wGoodsKindID, UINT16 wUseNum, UINT8& byUseBoxFlag, DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }
    CGoods* poGoods = poPlayer->GetBag().GetGoodsItem(wGoodsKindID);
    if(NULL == poGoods)
    {
        return ERR_GOODS_USE::ID_GOODS_NOT_EXIST;
    }

    return poGoods->Use(byUseBoxFlag, stBoxItemInfo, wUseNum);
}


//检查背包放入vecItemID物品后是否满(装备只算1个叠加数有效)
BOOL CItemMgr::CkBagFull(CPlayer* poPlayer, vector<UINT16> vecItemID)
{
    UINT8 bySize = vecItemID.size();
    UINT8 byUseNum = 0;
    CBag& oBag = poPlayer->GetBag();
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        if(!oBag.GetGoodsItem(vecItemID[byIdx]))
        {
            byUseNum++;
        }
    }
    if(oBag.GetIdleSize() < byUseNum)
    {
        return TRUE;
    }

    return FALSE;
}


BOOL CItemMgr::SysUseGoods(CPlayer* poPlayer, UINT16 wItemID, UINT16 wPileNum)
{
    SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemID);
    if(NULL == pstGoodsProp)
    {
        return FALSE;
    }

    switch(pstGoodsProp->byMainKindID)
    {
    case EGMK_PHYSTRENGTH:
    {
        poPlayer->AddPhyStrength(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EAPST_LOGINUSRGOOD, pstGoodsProp->wKindID, wPileNum);
    }
    break;
    case EGMK_GOLD:
    {
        poPlayer->AddGold(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EGGT_USELOGINGOODS, pstGoodsProp->wKindID, wPileNum);
    }
    break;
    case EGMK_COIN:
    {
        poPlayer->AddCoin(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EACT_USELOGINGOODS, pstGoodsProp->wKindID, wPileNum);
    }
    break;
    case EGMK_BLUEGAS:
    {
        poPlayer->AddBlueGas(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EABGT_USEGOODS, pstGoodsProp->wKindID, wPileNum);
    }
    break;
    case EGMK_PURPLEGAS:
    {
        poPlayer->AddPurpleGas(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EAPGT_USEGOODS, pstGoodsProp->wKindID, wPileNum);
    }
    break;
    case EGMK_JINGJIE:
    {
        poPlayer->AddJingJie(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EAJT_USEGOODS, pstGoodsProp->wKindID, wPileNum);
    }
    break;
    default:
        return FALSE;
        break;
    }

    return TRUE;
}

CItem* CItemMgr::CreateJewelryIntoBag(CPlayer* poPlayer, UINT16 wKindID, UINT16 wPileNum,ECreateItemRet& eRet, UINT16 wLevel,  CRecordMgr::EAddItemType emOpType)
{
	if(NULL == poPlayer)
	{
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}

	DT_JEWELRY_DATA stDT_JEWELRY_DATA;
	memset(&stDT_JEWELRY_DATA, 0, sizeof(DT_JEWELRY_DATA));
	if(! CJewelryPropMgr::Instance()->GenerateJewelryData(poPlayer, wKindID, stDT_JEWELRY_DATA))
	{
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}

    CItem* poItem = m_oJewelryShmemObjMgr.Alloc();
	VOID* pData = (VOID*)(&stDT_JEWELRY_DATA);

	if(NULL == poItem)
	{
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}

	UINT16 wItemIdx = poPlayer->GetNextJeweryIdx( wKindID );
	stDT_JEWELRY_DATA.wIdx = wItemIdx;
	UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, stDT_JEWELRY_DATA.wJewelryID, wItemIdx);
	poItem->SetMemID(qwKey);
	if(FALSE == poItem->Init(pData, poPlayer))
	{
		//清除已经创建的ITEM
		RemoveItem(poItem);
		eRet = ECIR_FAILED_KIND_INVALID;
		return NULL;
	}
	//放入背包中
	CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
	if(NULL == poJewelry)
	{
		//清除已经创建的ITEM
		RemoveItem(poItem);
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}


	CBag& oBag = poPlayer->GetBag();
	if(!oBag.AddItem(poItem))
	{
		//清除已经创建的ITEM
		RemoveItem(poItem);
		poPlayer->ReturnNextEquipIdx(stDT_JEWELRY_DATA.wJewelryID, wItemIdx);
		eRet = ECIR_FAILED_BAG_FULL;
		return NULL;
	}
	
	//poPlayer->OnAddEquip(poEquip);
	CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_ADDITEM, emOpType, poJewelry->GetItemKindID(), poJewelry->GetCurPileNum(), 
		poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);
	m_mapID2Item[poItem->GetMemID()] = poItem;

	eRet = ECIR_SUCCESS;
	return poItem;
}

CItem* CItemMgr::CreateJewelryIntoBag( CPlayer* poPlayer, DT_JEWELRY_DATA& stDT_JEWELRY_DATA, UINT16 wPileNum,ECreateItemRet& eRet, UINT16 wLevel, CRecordMgr::EAddItemType emOpType )
{
    if(NULL == poPlayer)
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }
    
    CItem* poItem = m_oJewelryShmemObjMgr.Alloc();
    VOID* pData = (VOID*)(&stDT_JEWELRY_DATA);

    if(NULL == poItem)
    {
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }

    UINT16 wItemIdx = poPlayer->GetNextJeweryIdx( stDT_JEWELRY_DATA.wJewelryID);
    stDT_JEWELRY_DATA.wIdx = wItemIdx;
    UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, stDT_JEWELRY_DATA.wJewelryID, wItemIdx);
    poItem->SetMemID(qwKey);
    
    if(FALSE == poItem->Init(pData, poPlayer))
    {
        //清除已经创建的ITEM
        RemoveItem(poItem);
        eRet = ECIR_FAILED_KIND_INVALID;
        return NULL;
    }
    //放入背包中
    CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
    if(NULL == poJewelry)
    {
        //清除已经创建的ITEM
        RemoveItem(poItem);
        eRet = ECIR_FAILED_OTHER;
        return NULL;
    }


    CBag& oBag = poPlayer->GetBag();
    if(!oBag.AddItem(poItem))
    {
        //清除已经创建的ITEM
        RemoveItem(poItem);
        poPlayer->ReturnNextEquipIdx(stDT_JEWELRY_DATA.wJewelryID, wItemIdx);
        eRet = ECIR_FAILED_BAG_FULL;
        return NULL;
    }

    //poPlayer->OnAddEquip(poEquip);
    CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_ADDITEM, emOpType, poJewelry->GetItemKindID(), poJewelry->GetCurPileNum(), 
        poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

	m_mapID2Item[poItem->GetMemID()] = poItem;

    eRet = ECIR_SUCCESS;
    return poItem;
}

CItem* CItemMgr::CreateJewelryIntoBagByGM(CPlayer* poPlayer, UINT16 wItemID, C32Vec& vec32, ECreateItemRet& eRet )
{
	if(NULL == poPlayer)
	{
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}

	UINT8 byItemKind = GetItemKindBYKindID(wItemID);
	if ( EIK_JEWELRY != byItemKind)
	{
		eRet = ECIR_FAILED_KIND_INVALID;
		return NULL;
	}
	

	DT_JEWELRY_DATA stDT_JEWELRY_DATA;
	memset(&stDT_JEWELRY_DATA, 0, sizeof(DT_JEWELRY_DATA));
	if(! CJewelryPropMgr::Instance()->GenerateJewelryData(poPlayer, wItemID, vec32, stDT_JEWELRY_DATA))
	{
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}

	CItem* poItem = m_oJewelryShmemObjMgr.Alloc();
	VOID* pData = (VOID*)(&stDT_JEWELRY_DATA);

	if(NULL == poItem)
	{
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}

	UINT16 wItemIdx = poPlayer->GetNextJeweryIdx( wItemID );
	stDT_JEWELRY_DATA.wIdx = wItemIdx;
	UINT64 qwKey = CLogicFunc::GetItemKey(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, stDT_JEWELRY_DATA.wJewelryID, wItemIdx);
	poItem->SetMemID(qwKey);
	m_mapID2Item[qwKey] = poItem;
	if(FALSE == poItem->Init(pData, poPlayer))
	{
		//清除已经创建的ITEM
		RemoveItem(poItem);
		eRet = ECIR_FAILED_KIND_INVALID;
		return NULL;
	}
	//放入背包中
	CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
	if(NULL == poJewelry)
	{
		//清除已经创建的ITEM
		RemoveItem(poItem);
		eRet = ECIR_FAILED_OTHER;
		return NULL;
	}

	CBag& oBag = poPlayer->GetBag();
	if(!oBag.AddItem(poItem))
	{
		//清除已经创建的ITEM
		RemoveItem(poItem);
		poPlayer->ReturnNextEquipIdx(stDT_JEWELRY_DATA.wJewelryID, wItemIdx);
		eRet = ECIR_FAILED_BAG_FULL;
		return NULL;
	}

	//poPlayer->OnAddEquip(poEquip);
	CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_ADDITEM, CRecordMgr::EAIT_GM, poJewelry->GetItemKindID(), poJewelry->GetCurPileNum(), 
		poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);


	eRet = ECIR_SUCCESS;
	return poItem;
}

UINT16 CItemMgr::RandomItemInnerBox(UINT16 wKindID) {
	//验证命中的是否是内部宝箱
	SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
	if(NULL == pstGoodsProp) {
		return 0;
	}
	//为内部宝箱
	if(EGMK_INNERBOX != pstGoodsProp->byMainKindID) {
		return 0;
	}

	SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wKindID);
	if(NULL == pstItemComposeProp) {
		SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL!"), MSG_MARK);
		return 0;;
	}

	CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
	if (0 == mapComposeItem.size()) {
		SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, wKindID);
		return 0;;
	}

	CRandomVec vecRandom;
	for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++) {
		vecRandom.push_back(itr->second.wRate);
	}

	//随机
	INT32 nRandomHitIdx = GetRandomIdx(vecRandom, 10000);
	if(RANDOM_UNHIT == nRandomHitIdx) {
		return 0;
	}

	UINT8 byIdx = 0;
	for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++, byIdx++) {
		if(byIdx == nRandomHitIdx) {
			return itr->first;
		}
	}

	return 0;
}
