#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/item/goods/goodspropmgr.h>
#include <logic/item/equip/equippropmgr.h>
#include <logic/item/equip/strengthenpropmgr.h>
#include "itemmakemgr.h"
#include "itemcomposepropmgr.h"
#include <db/autosqlbuf.h>
#include <logic/record/recordmgr.h>
#include <logic/base/basedefine.h>
#include <logic/other/errmsgmgr.h>
#include <logic/item/itemmgr.h>
#include <logic/bag/bag.h>
#include <logic/talent/drugpropmgr.h>
#include <logic/task/taskmgr.h>
#include <logic/hero/heromgr.h>

#include <logic/instance/instance.h>
#include <logic/player/player.h>

using namespace SGDP;

IMPLEMENT_SINGLETON(CItemMakeMgr);

CItemMakeMgr::CItemMakeMgr()
{
}

CItemMakeMgr::~CItemMakeMgr()
{

}

BOOL CItemMakeMgr::Init()
{
    return TRUE;
}

VOID CItemMakeMgr::UnInit()
{

}


UINT16 CItemMakeMgr::EstimateItem(UINT16 wProduceItemKindID, DT_ESTIMATE_ITEM_DATA& stEstimateItemInfo, UINT16 wStrenthenLevel, UINT8 byColor)
{
    if(EIK_EQUIP == CItemMgr::Instance()->GetItemKindBYKindID(wProduceItemKindID))
    {
        SEquipProp* pstProduceProp = CEquipPropMgr::Instance()->GetProp(wProduceItemKindID);
        if(NULL == pstProduceProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: pstProduceProp is NULL!"), MSG_MARK);
            RETURN_OTHER_ERR;
        }

        stEstimateItemInfo.byItemType = EEIK_EQUIP;
        DT_ESTIMATE_EQUIP_DATA& stEquip = stEstimateItemInfo.stItemInfo.stEquip;
        stEquip.wKindID = wProduceItemKindID;
        if((0 == wStrenthenLevel) && (0 == byColor))
        {
            stEquip.wLevel = 0;
        }
        else
        {
            stEquip.wLevel = CEquipStrengthenPropMgr::Instance()->GetConvertLevel(wStrenthenLevel, 1, pstProduceProp->wDressLevel);
        }

        stEquip.wDressLevel = pstProduceProp->wDressLevel;
        stEquip.stBattleAttribute = CEquip::BattleAttCountByLevel(stEquip.wLevel, 0, pstProduceProp);
    }
    else
    {
        SGoodsProp* pstProduceProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wProduceItemKindID);
        if(NULL == pstProduceProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: pstProduceProp is NULL!"), MSG_MARK);
            RETURN_OTHER_ERR;
        }

        if(EGMK_DRUG == pstProduceProp->byMainKindID)
        {
            UINT8 byDrugLevel = (UINT8)(pstProduceProp->dwValue);
            SDrugProp* pstDrugProp = CDrugPropMgr::Instance()->GetDrugProp(byDrugLevel);
            if(NULL == pstDrugProp)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pstDrugProp is NULL!"), MSG_MARK);
                RETURN_OTHER_ERR;
            }
            stEstimateItemInfo.byItemType = EEIK_DRUG;
            DT_ESTIMATE_DRUG_DATA& stDrug = stEstimateItemInfo.stItemInfo.stDrug;
            stDrug.wKindID = wProduceItemKindID;
            stDrug.wBaseTalent = pstDrugProp->wBaseTalent;
            stDrug.wDecTalent = pstDrugProp->wDecTalent;
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EGMK_DRUG != pstProduceProp->byMainKindID!"), MSG_MARK);
            return ERR_OPEN_ITEM_MAKER::ID_MAKER_CONFIG_ERR;
        }

    }

    return ERR_OPEN_ITEM_MAKER::ID_SUCCESS;
}



UINT16 CItemMakeMgr::MakeItem(CPlayer* poPlayer, UINT16 wMakerItemKindID, UINT16 wEquipKindID, UINT16 wEquipIdx, DT_ITEM_DATA_CLI2& stItemInfo, TCHAR aszErrInfo[MAX_COMMON_TXT_NUM])
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    CBag& oBag = poPlayer->GetBag();
    CItem* poMakerItem = oBag.GetGoodsItem(wMakerItemKindID);
    if(NULL == poMakerItem)
    {
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_ITEM_NOT_EXIST).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("ITEM_NOT_EXIST") .c_str());
        return ERR_MAKE_ITEM::ID_ITEM_NOT_EXIST;
    }
    CGoods* poMakerGoods = dynamic_cast<CGoods*>(poMakerItem);
    if(NULL == poMakerItem)
    {
        RETURN_OTHER_ERR;
    }
    if(0 == poMakerGoods->GetCurPileNum())
    {
        poPlayer->OnDeleteItem(poMakerGoods);
        //poMakerGoods->SetSaveState(ESS_WANT_SAVE);
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_ITEM_NOT_EXIST).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("ITEM_NOT_EXIST") .c_str());
        return ERR_MAKE_ITEM::ID_ITEM_NOT_EXIST;
    }

    SGoodsProp* pstMakerGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wMakerItemKindID);
    if(NULL == pstMakerGoodsProp)
    {
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_MAKER_CONFIG_ERR).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("MAKER_CONFIG_ERR") .c_str());
        return ERR_MAKE_ITEM::ID_MAKER_CONFIG_ERR;
    }

    if((EGMK_MAKER != pstMakerGoodsProp->byMainKindID) && (EGMK_GEMMAKER != pstMakerGoodsProp->byMainKindID))
    {
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_NOT_ITEM_MAKER).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("ITEM_NOT_EXIST") .c_str());
        return ERR_MAKE_ITEM::ID_NOT_ITEM_MAKER;
    }
    //宝石制作特别处理
    if(EGMK_GEMMAKER == pstMakerGoodsProp->byMainKindID)
    {
        return MakeGem(poPlayer, poMakerGoods, pstMakerGoodsProp, stItemInfo, aszErrInfo);
    }

    SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wMakerItemKindID);
    if(NULL == pstItemComposeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL!"), MSG_MARK);
        RETURN_OTHER_ERR;
    }

    //防止配置所需道具为0，则制造失败
    CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
    if(0 == mapComposeItem.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem is 0!"), MSG_MARK);
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_MAKER_CONFIG_ERR).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("MAKER_CONFIG_ERR") .c_str());
        return ERR_MAKE_ITEM::ID_MAKER_CONFIG_ERR;
    }

    CGoods2NumMap mapGoods2Num;//所需的道具，判断可制作后，制作，并清除材料
    UINT8 byNeedItemIdx = 0;
    CEquip* poNeedEquip = NULL;//所需的装备（若需要的话）,判断可制作后，制作，并清除材料
    for(CItemRateMapItr itr = mapComposeItem.begin(); ((itr != mapComposeItem.end()) && (byNeedItemIdx < MAX_ITEM_MAKER_NEED_NUM)); itr++)
    {
        UINT16 wNeedItemKindID = itr->first;
        UINT16 wNeedNum = itr->second.wItemNum;
        switch(CItemMgr::Instance()->GetItemKindBYKindID(wNeedItemKindID))
        {
        case EIK_EQUIP:
        {
            if(wNeedItemKindID != wEquipKindID)
            {
                //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_SELECTED_EQUIP_NOT_MATCH).c_str(), MAX_COMMON_TXT_NUM - 1);
                SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("SELECTED_EQUIP_NOT_MATCH") .c_str());
                return ERR_MAKE_ITEM::ID_SELECTED_EQUIP_NOT_MATCH;
            }
            poNeedEquip = poPlayer->GetEquip(wEquipKindID, wEquipIdx);
            if(NULL == poNeedEquip)
            {
                //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_SELECTED_EQUIP_NOT_EXIST).c_str(), MAX_COMMON_TXT_NUM - 1);
                SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("SELECTED_EQUIP_NOT_MATCH") .c_str());
                return ERR_MAKE_ITEM::ID_SELECTED_EQUIP_NOT_EXIST;
            }
        }
        break;
        case EIK_GOODS:
        {
            //
            //先去材料背包找，找不到则去物品背包找
            CItem* poItem = oBag.GetGoodsItem(wNeedItemKindID);
            UINT8 byNum = 0;
            CGoods* poGoods = NULL;
            if(NULL == poItem)
            {
                //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_NEED_ITEM_NOT_ENOUGH).c_str(), MAX_COMMON_TXT_NUM - 1);
                SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("NEED_ITEM_NOT_ENOUGH") .c_str());
                return ERR_MAKE_ITEM::ID_NEED_ITEM_NOT_ENOUGH;
            }
            else
            {
                CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
                if(NULL == poGoods)
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: poGoods is NULL!"), MSG_MARK);
                    RETURN_OTHER_ERR;
                }
                if(poGoods->GetCurPileNum() < wNeedNum)
                {
                    //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_NEED_ITEM_NOT_ENOUGH).c_str(), MAX_COMMON_TXT_NUM - 1);
                    SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("NEED_ITEM_NOT_ENOUGH") .c_str());
                    return ERR_MAKE_ITEM::ID_NEED_ITEM_NOT_ENOUGH;
                }
                mapGoods2Num[poGoods] = wNeedNum;
            }
        }
        break;
        default:
            RETURN_OTHER_ERR;
            break;

        }
    }

    UINT16 wProduceItemKindID = (UINT16)(pstMakerGoodsProp->dwValue);
    switch(CItemMgr::Instance()->GetItemKindBYKindID(wProduceItemKindID))
    {
    case EIK_EQUIP:
    {
        SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(wProduceItemKindID);
        //制作装备
        return MakeEquip(poPlayer, poMakerGoods, pstEquipProp, poNeedEquip, mapGoods2Num, stItemInfo, aszErrInfo);
    }
    case EIK_GOODS:
    {
        SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wProduceItemKindID);
        //制作物品
        return MakeGoods(poPlayer, poMakerGoods, pstGoodsProp, mapGoods2Num, stItemInfo, aszErrInfo);
    }
    default:
        RETURN_OTHER_ERR;
    }

}

UINT16 CItemMakeMgr::ComposeGoods(CPlayer* poPlayer, UINT16 wItemKindID, DT_ITEM_DATA_CLI2 & stDT_ITEM_DATA_CLI2)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //获取物品属性。
    SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemKindID);
    if(NULL == pstGoodsProp)
    {
        return ERR_COMPOSE_GOODS::ID_GOODS_NOT_COMPOSE;
    }

    CBag& oBag = poPlayer->GetBag();

    //获取物品背包信息
    CGoods* poGoods = oBag.GetGoodsItem(wItemKindID);
    if(NULL == poGoods)
    {
        return ERR_COMPOSE_GOODS::ID_BAG_NOT_EXIST;;
    }

    //检测物品类型
    if (EIK_GOODS != poGoods->GetItemKind())
    {
        return ERR_COMPOSE_GOODS::ID_BAD_TYPE;
    }
    //检测物品个数，防出错。
    if(0 == poGoods->GetCurPileNum())
    {
        poPlayer->OnDeleteItem(poGoods);
        return ERR_COMPOSE_GOODS::ID_BAG_NOT_ENOUGH;
    }

    //检测物品个数，是否达到
    if (poGoods->GetCurPileNum() < pstGoodsProp->wComposeCount)
    {
        return ERR_COMPOSE_GOODS::ID_BAG_NOT_ENOUGH;
    }

    //特殊处理，如果剩下的个数刚好等于背包的个数，先清掉背包数据，防此背包满。
    //   if (poGoods->GetCurPileNum() != pstGoodsProp->wComposeCount)
    //{
    //	//背包满
    //	if(0 == oBag.GetIdleSize())
    //	{
    //		return ERR_COMPOSE_GOODS::ID_BAG_FULL;
    //	}
    //}

    CTaskMgr::Instance()->OnComposeGood(poPlayer, wItemKindID);
    //生成物品
    ECreateItemRet eRet;
    CItem * poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, pstGoodsProp->wComposeKindID, 1, eRet, 0, CRecordMgr::EAIT_COMPOSEGOODS);
    //背包满
    if(NULL == poItem && ECIR_FAILED_BAG_FULL == eRet)
    {
        return ERR_COMPOSE_GOODS::ID_BAG_FULL;
    }
    else if (NULL == poItem && ECIR_FAILED_HIT == eRet)
    {
        if (NULL != poGoods)
        {
            //日志
            //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEGOODS, pstGoodsProp->wKindID, SDTimeToString(SDNow()), poGoods->GetDT_GOODS_DATA().wPileCount,
            //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, pstGoodsProp->wComposeCount);


            //         poGoods->GetDT_GOODS_DATA().wPileCount -= pstGoodsProp->wComposeCount;

            poGoods->DecPileNum(pstGoodsProp->wComposeCount, CRecordMgr::EDIT_MAKEGOODS);
            if(0 == poGoods->GetCurPileNum())
            {
                poPlayer->OnDeleteItem(poGoods);
            }
        }
        return ERR_COMPOSE_GOODS::ID_BAG_BOX_HIT;
    }
    else if(NULL == poItem)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateItemIntoBag failed! KindID[%d],eRet[%d]"), MSG_MARK, pstGoodsProp->wKindID, eRet);
        RETURN_OTHER_ERR;
    }

    //先检测一下背包是否已清除过数据，再减掉使用掉的个数。
    if (NULL != poGoods)
    {
        //poGoods->GetDT_GOODS_DATA().wPileCount -= pstGoodsProp->wComposeCount;

        //日志
        //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEGOODS, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetDT_GOODS_DATA().wPileCount,
        //                                    poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, pstGoodsProp->wComposeCount);

        poGoods->DecPileNum(pstGoodsProp->wComposeCount, CRecordMgr::EDIT_MAKEGOODS);

        if(0 == poGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poGoods);
        }
    }

    //根据生成的类型获取对应的信息
    switch(poItem->GetItemKind())
    {
    case EIK_EQUIP:
    {
        CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
        if(NULL == poEquip)
        {
            break;
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
            break;
        }
        stDT_ITEM_DATA_CLI2.byItemType = EIK_GOODS;
        DT_GOODS_DATA_CLI& stGoods = stDT_ITEM_DATA_CLI2.stItemInfo.stGoods;
        poGoods->GetDT_GOODS_DATA_CLI(stGoods);
    }
    break;
    default:
        break;
    }
    return ERR_COMPOSE_GOODS::ID_SUCCESS;
}


UINT16 CItemMakeMgr::MakeEquip(CPlayer* poPlayer, CGoods* poMakerGoods, SEquipProp* pstProduceEquipProp, CEquip* poNeedEquip, CGoods2NumMap& mapGoods2Num, DT_ITEM_DATA_CLI2& stItemInfo,  TCHAR aszErrInfo[MAX_COMMON_TXT_NUM])
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    if(NULL == pstProduceEquipProp)
    {
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_MAKER_CONFIG_ERR).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("MAKER_CONFIG_ERR").c_str());
        return ERR_MAKE_ITEM::ID_MAKER_CONFIG_ERR;
    }

    CBag& oBag = poPlayer->GetBag();

    CEquip* poNewEquip = NULL;

    //UINT16 wGoodsKind = poMakerGoods->GetItemKindID();
    UINT16 wPileCount = 0;
    //需要旧装备
    if(NULL != poNeedEquip)
    {
        SEquipProp* poNeedEquipProp = poNeedEquip->GetEquipProp();
        if (NULL == poNeedEquipProp)
        {
            return ERR_MAKE_ITEM::ID_MAKER_CONFIG_ERR;
        }

        if(poNeedEquipProp->byPosKindID != pstProduceEquipProp->byPosKindID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: new byPosKindID != old byPosKindID!"), MSG_MARK);
            //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_MAKER_CONFIG_ERR).c_str(), MAX_COMMON_TXT_NUM - 1);
            SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("MAKER_CONFIG_ERR").c_str());
            return ERR_MAKE_ITEM::ID_MAKER_CONFIG_ERR;
        }
        //判断旧装备是否穿戴
        UINT16 wHeroKindID = poNeedEquip->GetDT_EQUIP_DATA().wHeroKindID;
        BOOL bDress = FALSE;////需穿戴且可穿戴标志
        CHero* poHero = NULL;
        if(0 != wHeroKindID)
        {
            poHero = CHeroMgr::Instance()->FindHero(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wHeroKindID);
            if(NULL == poHero)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: poHero is NULL!"), MSG_MARK);
                RETURN_OTHER_ERR;
            }

            //判断旧装备穿戴的武将是否能穿戴新装备
            if(poHero->GetDT_HERO_BASE_DATA().wLevel >= pstProduceEquipProp->wDressLevel)
            {
                //需穿戴且可穿戴
                bDress = TRUE;
            }
        }

        //生成新装备
        ECreateItemRet eRet;
        UINT16 wLevel = CEquipStrengthenPropMgr::Instance()->GetConvertLevel(poNeedEquip->GetDT_EQUIP_DATA().wLevel, poNeedEquipProp->wDressLevel, pstProduceEquipProp->wDressLevel);
        poNewEquip = CItemMgr::Instance()->CreateEquip(poPlayer, pstProduceEquipProp->wKindID, 1, eRet, wLevel);
        if(NULL == poNewEquip)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateEquip failed! KindID[%d],eRet[%d]"), MSG_MARK, pstProduceEquipProp->wKindID, eRet);
            RETURN_OTHER_ERR;
        }
        //先清除旧装备，才能放入背包，防止原先背包满无法插入问题
        poPlayer->OnDeleteEquip(poNeedEquip);
        //需要穿戴，给武将穿戴装备,不需要穿戴，设置背包信息，放背包
        if(bDress)
        {
            //穿戴新装备
            poHero->DressMakeEquip(poNewEquip);
        }
        poPlayer->OnAddEquip(poNewEquip);

        //先清除制作卷，避免可能无法放入背包
        //poMakerGoods->GetDT_GOODS_DATA().wPileCount--;

        //日志
        //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEEQUIP, poMakerGoods->GetItemKindID(), SDTimeToString(SDNow()), poMakerGoods->GetDT_GOODS_DATA().wPileCount,
        //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

        poMakerGoods->DecPileNum(1, CRecordMgr::EDIT_MAKEEQUIP);

        wPileCount = poMakerGoods->GetDT_GOODS_DATA().wPileCount;
        if(0 == poMakerGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poMakerGoods);
        }

    }
    //不需要旧装备
    else
    {
        //判断背包是否已满
        if((0 == oBag.GetIdleSize()) && (poMakerGoods->GetCurPileNum() > 1))
        {
            //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_BAG_FULL).c_str(), MAX_COMMON_TXT_NUM - 1);
            SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("BAG_FULL").c_str());
            return ERR_MAKE_ITEM::ID_BAG_FULL;
        }

        //生成新装备
        ECreateItemRet eRet;
        UINT16 wLevel = 0;

        CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, pstProduceEquipProp->wKindID, 1, eRet, wLevel, CRecordMgr::EAIT_MAKEEQUIP);
        if(NULL == poItem)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateItemIntoBag failed! KindID[%d],eRet[%d]"), MSG_MARK, pstProduceEquipProp->wKindID, eRet);
            RETURN_OTHER_ERR;
        }
        poNewEquip = dynamic_cast<CEquip*>(poItem);
        if(NULL == poNewEquip)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: dynamic_cast<CEquip*>(poItem) is NULL"), MSG_MARK);
            RETURN_OTHER_ERR;
        }

        //先清除制作卷，避免可能无法放入背包
        //poMakerGoods->GetDT_GOODS_DATA().wPileCount--;

        //日志
        //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEEQUIP, poMakerGoods->GetItemKindID(), SDTimeToString(SDNow()), poMakerGoods->GetDT_GOODS_DATA().wPileCount,
        //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

        poMakerGoods->DecPileNum(1, CRecordMgr::EDIT_MAKEEQUIP);

        wPileCount = poMakerGoods->GetDT_GOODS_DATA().wPileCount;
        if(0 == poMakerGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poMakerGoods);
        }

        poPlayer->OnAddEquip(poNewEquip);
    }

    //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEEQUIP, wGoodsKind, SDTimeToString(SDNow()), wPileCount,
    //                                    poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

    //清除所需道具
    for(CGoods2NumMapItr itr = mapGoods2Num.begin(); itr != mapGoods2Num.end(); itr++)
    {
        CGoods* poGoods = itr->first;
        if(poGoods->GetDT_GOODS_DATA().wPileCount > itr->second)
        {
            //日志
            //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEEQUIP, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetDT_GOODS_DATA().wPileCount,
            //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, itr->second);

            //poGoods->GetDT_GOODS_DATA().wPileCount -= itr->second;

            poGoods->DecPileNum(itr->second, CRecordMgr::EDIT_MAKEEQUIP);

        }
        else
        {
            //日志
            //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEEQUIP, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetDT_GOODS_DATA().wPileCount,
            //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, poGoods->GetDT_GOODS_DATA().wPileCount);

            poGoods->DecPileNum(poGoods->GetDT_GOODS_DATA().wPileCount, CRecordMgr::EDIT_MAKEEQUIP);

            //清除
            poPlayer->OnDeleteItem(poGoods);
        }
    }


    stItemInfo.byItemType = EIK_EQUIP;
    poNewEquip->GetDT_EQUIP_DATA_CLI(stItemInfo.stItemInfo.stEquip);

    return ERR_MAKE_ITEM::ID_SUCCESS;
}

UINT16 CItemMakeMgr::MakeGoods(CPlayer* poPlayer, CGoods* poMakerGoods, SGoodsProp* pstProduceGoodsProp, CGoods2NumMap& mapGoods2Num, DT_ITEM_DATA_CLI2& stItemInfo,  TCHAR aszErrInfo[MAX_COMMON_TXT_NUM])
{
    //判断物品背包是否已满
    CBag& oBag = poPlayer->GetBag();
	if ( 0 == oBag.GetIdleSize())
	{
		SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("BAG_FULL").c_str());
		return ERR_MAKE_ITEM::ID_BAG_FULL;
	}
    //制作卷叠加数为1，则向将制作卷从背包中清除,避免差1无法插入背包问题
    if(1 == poMakerGoods->GetCurPileNum())
    {
        oBag.DelItem(poMakerGoods, TRUE);
    }

    //生成物品
    ECreateItemRet eRet;
    CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, pstProduceGoodsProp->wKindID, 1, eRet, 0, CRecordMgr::EAIT_MAKEGOODS, poMakerGoods->GetItemKindID());
    //背包满
    if((NULL == poItem) && (ECIR_FAILED_BAG_FULL == eRet))
    {
        //没有使用制作卷，再放回背包
        if(1 == poMakerGoods->GetCurPileNum())
        {
            if(!oBag.AddItem(poMakerGoods))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: recover makegoods item into bag failed! playerid:%d, usedsize = %u, idlesize = %u"), MSG_MARK, poPlayer->GetID(), oBag.GetUsedSize(), oBag.GetIdleSize());
            }
        }

        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_BAG_FULL).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("BAG_FULL").c_str());
        return ERR_MAKE_ITEM::ID_BAG_FULL;
    }
    else if(NULL == poItem)
    {
        //没有使用制作卷，再放回背包
        if(1 == poMakerGoods->GetCurPileNum())
        {
            if(!oBag.AddItem(poMakerGoods))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: recover makegoods item into bag failed! playerid:%d, usedsize = %u, idlesize = %u"), MSG_MARK, poPlayer->GetID(), oBag.GetUsedSize(), oBag.GetIdleSize());
            }
        }

        SYS_CRITICAL(_SDT("[%s: %d]: CreateItemIntoBag failed! KindID[%d],eRet[%d]"), MSG_MARK, pstProduceGoodsProp->wKindID, eRet);
        RETURN_OTHER_ERR;
    }
    CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
    if(NULL == poGoods)
    {
        //没有使用制作卷，再放回背包
        if(1 == poMakerGoods->GetCurPileNum())
        {
            if(!oBag.AddItem(poMakerGoods))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: recover makegoods item into bag failed! playerid:%d, usedsize = %u, idlesize = %u"), MSG_MARK, poPlayer->GetID(), oBag.GetUsedSize(), oBag.GetIdleSize());
            }
        }

        SYS_CRITICAL(_SDT("[%s: %d]: dynamic_cast<poGoods*>(poItem) is NULL"), MSG_MARK);
        RETURN_OTHER_ERR;
    }

    //清除所需道具
    for(CGoods2NumMapItr itr = mapGoods2Num.begin(); itr != mapGoods2Num.end(); itr++)
    {
        CGoods* poGoods = itr->first;
        if(0 == poGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poGoods);
        }

    }
    //清除制作卷
    //poMakerGoods->GetDT_GOODS_DATA().wPileCount--;

    //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEGOODS, poMakerGoods->GetItemKindID(), SDTimeToString(SDNow()), poMakerGoods->GetCurPileNum(),
    //                                    poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

    poMakerGoods->DecPileNum(1, CRecordMgr::EDIT_MAKEGOODS);


    if(0 == poMakerGoods->GetCurPileNum())
    {
        poPlayer->OnDeleteItem(poMakerGoods);
    }
    //poMakerGoods->SetSaveState(ESS_WANT_SAVE);

    stItemInfo.byItemType = EIK_GOODS;
    poGoods->GetDT_GOODS_DATA_CLI(stItemInfo.stItemInfo.stGoods);

    return ERR_MAKE_ITEM::ID_SUCCESS;
}

UINT16 CItemMakeMgr::MakeGem(CPlayer* poPlayer, CGoods* poMakerGoods, SGoodsProp* pstMakerGoodsProp, DT_ITEM_DATA_CLI2& stItemInfo, TCHAR aszErrInfo[MAX_COMMON_TXT_NUM])
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }
    if((NULL == poMakerGoods) || (NULL == pstMakerGoodsProp))
    {
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_ITEM_NOT_EXIST).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("ITEM_NOT_EXIST").c_str());
        return ERR_MAKE_ITEM::ID_ITEM_NOT_EXIST;
    }
    SGoodsProp* pstProduceGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(pstMakerGoodsProp->dwValue);
    if(NULL == pstProduceGoodsProp)
    {
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_MAKER_CONFIG_ERR).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("MAKER_CONFIG_ERR").c_str());
        return ERR_MAKE_ITEM::ID_MAKER_CONFIG_ERR;
    }

    //判断是否开放
    if(poPlayer->GetInstance().GetMaxClimbTownInstanceID().byInstanceIdx < CItemComposePropMgr::Instance()->GetGemMakeOpenNeedTowerNum((UINT16)(pstMakerGoodsProp->dwParam2)))
    {
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_MAKER_LEVEL_UNLOCK).c_str(), MAX_COMMON_TXT_NUM - 1);
        SDStrcpy(aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("MAKER_LEVEL_UNLOCK").c_str());
        return ERR_MAKE_ITEM::ID_MAKER_LEVEL_UNLOCK;
    }

    //判断蓝气是否足够
    if(poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas < pstMakerGoodsProp->dwParam1)
    {
        return ERR_MAKE_ITEM::ID_NEED_BLUEGAS_NOT_ENOUGH;
    }

    //制作物品
    CGoods2NumMap mapGoods2Num;
    UINT16 wMakerGoodsKindID = poMakerGoods->GetItemKindID();
    UINT16 wErrCode = MakeGoods(poPlayer, poMakerGoods, pstProduceGoodsProp, mapGoods2Num, stItemInfo, aszErrInfo);
    if(ERR_MAKE_ITEM::ID_SUCCESS != wErrCode)
    {
        return wErrCode;
    }

    //清除所需蓝气
    poPlayer->DecBlueGas(pstMakerGoodsProp->dwParam1, CRecordMgr::EDBGT_MAKERGEM, wMakerGoodsKindID, pstMakerGoodsProp->wKindID);

    return ERR_MAKE_ITEM::ID_SUCCESS;
}

UINT16 CItemMakeMgr::ComposeEquip(CPlayer* poPlayer, UINT16 wEquipItemID, UINT16 wEquipIdx, UINT8 byGoldComposeFlag, PKT_CLIGS_COMPOSE_EQUIP_ACK* pstAck)
{
    //获取装备信息
    SEquipProp * pstOldEquipProp = CEquipPropMgr::Instance()->GetProp(wEquipItemID);
    if (NULL == pstOldEquipProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Get ItemComposeProp failed! KindID[%d]"), MSG_MARK, wEquipItemID);
        return ERR_COMPOSE_EQUIP::ID_EQUIPNOTEXIST;
    }

    //获取装备信息
    SEquipProp * pstNewEquipProp = CEquipPropMgr::Instance()->GetProp(pstOldEquipProp->wUpgradeToEquipID);
    if (NULL == pstNewEquipProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Get ItemComposeProp failed! KindID[%d]"), MSG_MARK, pstOldEquipProp->wUpgradeToEquipID);
        return ERR_COMPOSE_EQUIP::ID_EQUIPNOTEXIST;
    }

    //获取玩家装备信息
    CEquip * poOldEquip = poPlayer->GetEquip(wEquipItemID, wEquipIdx);
    if(NULL == poOldEquip)
    {
        return ERR_COMPOSE_EQUIP::ID_EQUIPNOTEXIST;
    }

    //获取合成属性
    SItemComposeProp * pItemComposeProp = NULL;
    pItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wEquipItemID);
    if (NULL == pItemComposeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Get ItemComposeProp failed! KindID[%d]"), MSG_MARK, wEquipItemID);
        return ERR_COMPOSE_EQUIP::ID_EQUIPNOTEXIST;
    }

    //校验元宝是否够用
    if (byGoldComposeFlag && poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < pstOldEquipProp->wUpgradeSuccessGold )
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }


    //检测素材是否够用
    for (CItemRateMapItr itr = pItemComposeProp->mapComposeItem.begin(); itr != pItemComposeProp->mapComposeItem.end(); itr++)
    {
        UINT16 wNeedItemKindID = itr->first;
        UINT16 wNeedNum = itr->second.wItemNum;
        CItem* poItem = poPlayer->GetBag().GetGoodsItem(wNeedItemKindID);
        if (NULL == poItem)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GetGoodsItem failed! KindID[%d]"), MSG_MARK, wNeedItemKindID);
            return ERR_COMPOSE_EQUIP::ID_MATERIALNOTENOUGH;
        }
        CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
        if(NULL == poGoods)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GetGoodsItem failed! KindID[%d]"), MSG_MARK, wNeedItemKindID);
            return  ERR_COMPOSE_EQUIP::ID_MATERIALNOTENOUGH;
        }

        if(poGoods->GetCurPileNum() < wNeedNum)
        {
            return  ERR_COMPOSE_EQUIP::ID_MATERIALNOTENOUGH;
        }
    }


    //判断旧装备是否穿戴
    UINT16		wHeroKindID = poOldEquip->GetDT_EQUIP_DATA().wHeroKindID;
    BOOL			bDress = FALSE;////需穿戴且可穿戴标志
    CHero*		poHero = NULL;
    if(0 != wHeroKindID)
    {
        poHero = CHeroMgr::Instance()->FindHero(poPlayer->GetDT_PLAYER_BASE_DATA().dwID, wHeroKindID);
        if(NULL == poHero)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: poHero is NULL!"), MSG_MARK);
            return ERR_COMPOSE_EQUIP::ID_COMPOSEERROR;
        }

        //判断旧装备穿戴的武将是否能穿戴新装备
        if(poHero->GetDT_HERO_BASE_DATA().wLevel >= pstNewEquipProp->wDressLevel)
        {
            //需穿戴且可穿戴
            bDress = TRUE;
        }
        else
        {
            return ERR_COMPOSE_EQUIP::ID_HERONOTENOUGHLEAVER;
        }
    }

    //检测素材是否够用并清除
    for (CItemRateMapItr itr = pItemComposeProp->mapComposeItem.begin(); itr != pItemComposeProp->mapComposeItem.end(); itr++)
    {
        UINT16 wNeedItemKindID = itr->first;
        UINT16 wNeedNum = itr->second.wItemNum;
        CItem* poItem = poPlayer->GetBag().GetGoodsItem(wNeedItemKindID);
        if (NULL == poItem)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GetGoodsItem failed! KindID[%d]"), MSG_MARK, wNeedItemKindID);
            return ERR_COMPOSE_EQUIP::ID_MATERIALNOTENOUGH;
        }
        CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
        if(NULL == poGoods)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GetGoodsItem failed! KindID[%d]"), MSG_MARK, wNeedItemKindID);
            return  ERR_COMPOSE_EQUIP::ID_MATERIALNOTENOUGH;
        }

        if(poGoods->GetCurPileNum() < wNeedNum)
        {
            return  ERR_COMPOSE_EQUIP::ID_MATERIALNOTENOUGH;
        }




        //      poGoods->GetDT_GOODS_DATA().wPileCount -= wNeedNum;

        //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEGOODS, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetCurPileNum(),
        //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, wNeedNum);
        poGoods->DecPileNum(wNeedNum, CRecordMgr::EDIT_MAKEGOODS);
        if (0 == poGoods->GetDT_GOODS_DATA().wPileCount)
        {
            poPlayer->OnDeleteItem(poGoods);            //清除
        }
    }

    for (UINT8 i = 0; i < pstOldEquipProp->byComposeItemNum && i < MAX_COMPOSE_ITEM_NUM; i++)
    {
        //获取合成物品信息
        CItemComposePropMgr::Instance()->GetDT_NEED_ITEM_DATA(poPlayer, pstAck->astStrItemInfoList[i], pstOldEquipProp->wKindID, pstOldEquipProp->awComposeItem[i]);
        pstAck->bySrcItemNum++;
    }

    //开始合成
    INT32 nRandomHitIdx = 0;
    if (byGoldComposeFlag)
    {
        nRandomHitIdx = 1;
        //poPlayer->GetDT_PLAYER_BASE_DATA().qwGold -= pstOldEquipProp->wUpgradeSuccessGold;
        poPlayer->DecGold(pstOldEquipProp->wUpgradeSuccessGold, CRecordMgr::EDGT_STRENGTHENEQUIP, pstOldEquipProp->wKindID);
    }
    else
    {
        CRandomVec vecRandom;
        UINT16	 wFailRate = 100 > pstOldEquipProp->wUpgradeSuccessRate ? (100 - pstOldEquipProp->wUpgradeSuccessRate) : 0;
        vecRandom.push_back(wFailRate);
        vecRandom.push_back(pstOldEquipProp->wUpgradeSuccessRate);
        nRandomHitIdx = GetRandomIdxMustHit(vecRandom);	//命中率
    }

    if (1 != nRandomHitIdx)
    {
        return ERR_COMPOSE_EQUIP::ID_COMPOSEERROR;
    }

    CEquip*			poNewEquip = NULL;
    ECreateItemRet	eRet;    //生成新装备
    //UINT16			wLevel = CEquipStrengthenPropMgr::Instance()->GetConvertLevel(poOldEquip->GetDT_EQUIP_DATA().wLevel, poOldEquip->GetEquipProp()->wDressLevel, pstOldEquipProp->wDressLevel);
    UINT16			wLevel = CEquipStrengthenPropMgr::Instance()->GetConvertLevel(poOldEquip->GetDT_EQUIP_DATA().wLevel, pstOldEquipProp->wDressLevel, pstNewEquipProp->wDressLevel);



    poNewEquip = CItemMgr::Instance()->CreateEquip(poPlayer, pstNewEquipProp->wKindID, 1, eRet, wLevel);
    if(NULL == poNewEquip)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateEquip failed! KindID[%d],eRet[%d]"), MSG_MARK, pstNewEquipProp->wKindID, eRet);
        return ERR_COMPOSE_EQUIP::ID_COMPOSEERROR;
    }

    //先清除旧装备，才能放入背包，防止原先背包满无法插入问题
    poPlayer->OnDeleteEquip(poOldEquip);
    //需要穿戴，给武将穿戴装备,不需要穿戴，设置背包信息，放背包
    if(bDress)
    {
        poHero->DressMakeEquip(poNewEquip);        //穿戴新装备
    }
    poPlayer->OnAddEquip(poNewEquip);

    //DT_NEED_ITEM_DATA stSrcEquipNeedItem;


    poNewEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(pstAck->stEquipStrengthenInfo);
    poNewEquip->GetDT_EQUIP_DATA_CLI(pstAck->stEquipInfo);
    pstAck->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

    //CTaskMgr::Instance()->OnUpgradeEquip(poPlayer, wEquipItemID);

    poPlayer->ResetBattleAttr();

    return ERR_COMPOSE_EQUIP::ID_SUCCESS;

}

UINT16 CItemMakeMgr::ComposeSeed(CPlayer* poPlayer, UINT16 wItemKindID, DT_ITEM_DATA_CLI2 & stDT_ITEM_DATA_CLI2)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //获取物品属性。
    SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemKindID);
    if(NULL == pstGoodsProp)
    {
        return ERR_COMPOSE_SEED::ID_GOODS_NOT_COMPOSE;
    }

    CBag& oBag = poPlayer->GetBag();

    //获取物品背包信息
    CGoods* poGoods = oBag.GetGoodsItem(wItemKindID);
    if(NULL == poGoods)
    {
        return ERR_COMPOSE_SEED::ID_BAG_NOT_EXIST;
    }

    //检测物品类型
    if (EIK_GOODS != poGoods->GetItemKind() || EGMK_CROP != poGoods->GetGoodsMainKindID())
    {
        return ERR_COMPOSE_SEED::ID_BAD_TYPE;
    }
    //检测物品个数，防出错。
    if(0 == poGoods->GetCurPileNum())
    {
        poPlayer->OnDeleteItem(poGoods);
        return ERR_COMPOSE_SEED::ID_BAG_NOT_ENOUGH;
    }

    //检测物品个数，是否达到
    if (poGoods->GetCurPileNum() < pstGoodsProp->wComposeCount)
    {
        return ERR_COMPOSE_SEED::ID_BAG_NOT_ENOUGH;
    }

    CTaskMgr::Instance()->OnComposeGood(poPlayer, wItemKindID);
    //生成物品
    ECreateItemRet eRet;
    CItem * poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, pstGoodsProp->wComposeKindID, 1, eRet, 0, CRecordMgr::EAIT_MAKEGOODS, pstGoodsProp->wKindID);
    //背包满
    if(NULL == poItem && ECIR_FAILED_BAG_FULL == eRet)
    {
        return ERR_COMPOSE_SEED::ID_BAG_FULL;
    }
    else if(NULL == poItem)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateItemIntoBag failed! KindID[%d],eRet[%d]"), MSG_MARK, pstGoodsProp->wKindID, eRet);
        RETURN_OTHER_ERR;
    }

    //先检测一下背包是否已清除过数据，再减掉使用掉的个数。
    if (NULL != poGoods)
    {


        //poGoods->GetDT_GOODS_DATA().wPileCount -= pstGoodsProp->wComposeCount;
        //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, , poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetCurPileNum(),
        //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, pstGoodsProp->wComposeCount);

        poGoods->DecPileNum(pstGoodsProp->wComposeCount, CRecordMgr::EDIT_MAKEGOODS);
        if(0 == poGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poGoods);
        }

    }

    //根据生成的类型获取对应的信息
    switch(poItem->GetItemKind())
    {
    case EIK_EQUIP:
    {
        CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
        if(NULL == poEquip)
        {
            break;
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
            break;
        }
        stDT_ITEM_DATA_CLI2.byItemType = EIK_GOODS;
        DT_GOODS_DATA_CLI& stGoods = stDT_ITEM_DATA_CLI2.stItemInfo.stGoods;
        poGoods->GetDT_GOODS_DATA_CLI(stGoods);
    }
    break;
    default:
        break;
    }
    return ERR_COMPOSE_SEED::ID_SUCCESS;
}

UINT16 CItemMakeMgr::OpenGemMaker(CPlayer* poPlayer, UINT8& byMakerNum, DT_GEM_MAKER_DATA astGemMakerInfo[MAX_GEM_LEVEL])
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }
    //
    UINT8	byCurTowerNum	= poPlayer->GetInstance().GetMaxClimbTownInstanceID().byInstanceIdx;
    UINT8	byMaxGemLevel	= CItemComposePropMgr::Instance()->GetMaxGemLevel();
    //////////////////////////////////////////////////////////////////////////
    //扩展协议信息
    byMakerNum = 0;
    CItemComposePropMgr::Instance()->GetInitDT_GEM_MAKER_DATA(astGemMakerInfo, MAX_GEM_LEVEL);
    UINT16 wIdx = 0;
    for(wIdx = 0; wIdx < MAX_GEM_LEVEL && wIdx < byMaxGemLevel; wIdx++)
    {
        UINT16 wGemLevel = wIdx + 1;
        DT_GEM_MAKER_DATA& stDT_GEM_MAKER_DATA = astGemMakerInfo[wIdx];
        CGoods* poMaker = poPlayer->GetBag().GetGoodsItem(stDT_GEM_MAKER_DATA.wMakerKindID);
        if(poMaker)
        {
            stDT_GEM_MAKER_DATA.wPileNum = poMaker->GetCurPileNum();
        }

        if(stDT_GEM_MAKER_DATA.byOpenNeedTowerNum <= byCurTowerNum)
        {
            byMakerNum++;
        }
        else
        {
            break;
        }
    }
    if(byMakerNum < MAX_GEM_LEVEL)
    {
        UINT16 wGemLevel = wIdx + 1;
        DT_GEM_MAKER_DATA& stDT_GEM_MAKER_DATA = astGemMakerInfo[wIdx];
        CGoods* poMaker = poPlayer->GetBag().GetGoodsItem(stDT_GEM_MAKER_DATA.wMakerKindID);
        if(poMaker)
        {
            stDT_GEM_MAKER_DATA.wPileNum = poMaker->GetCurPileNum();
        }
        byMakerNum++;
    }
    return ERR_OPEN_GEM_MAKER_TAB::ID_SUCCESS;
}


UINT16 CItemMakeMgr::OpenGemMakerEx(CPlayer* poPlayer, UINT8& byMakerNum, DT_GEM_MAKER_DATA astGemMakerInfo[MAX_GEM_LEVEL_EX])
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }
    //
    UINT8	byCurTowerNum	= poPlayer->GetInstance().GetMaxClimbTownInstanceID().byInstanceIdx;
    UINT8	byMaxGemLevel	= CItemComposePropMgr::Instance()->GetMaxGemLevel();
    //////////////////////////////////////////////////////////////////////////
    //扩展协议信息
    byMakerNum = 0;
    CItemComposePropMgr::Instance()->GetInitDT_GEM_MAKER_DATA(astGemMakerInfo, MAX_GEM_LEVEL_EX);
    UINT16 wIdx = 0;
    for(wIdx = 0; wIdx < MAX_GEM_LEVEL_EX && wIdx < byMaxGemLevel; wIdx++)
    {
        UINT16 wGemLevel = wIdx + 1;
        DT_GEM_MAKER_DATA& stDT_GEM_MAKER_DATA = astGemMakerInfo[wIdx];
        CGoods* poMaker = poPlayer->GetBag().GetGoodsItem(stDT_GEM_MAKER_DATA.wMakerKindID);
        if(poMaker)
        {
            stDT_GEM_MAKER_DATA.wPileNum = poMaker->GetCurPileNum();
        }

        if(stDT_GEM_MAKER_DATA.byOpenNeedTowerNum <= byCurTowerNum)
        {
            byMakerNum++;
        }
        else
        {
            break;
        }
    }
    if(byMakerNum < byMaxGemLevel)
    {
        UINT16 wGemLevel = wIdx + 1;
        DT_GEM_MAKER_DATA& stDT_GEM_MAKER_DATA = astGemMakerInfo[wIdx];
        CGoods* poMaker = poPlayer->GetBag().GetGoodsItem(stDT_GEM_MAKER_DATA.wMakerKindID);
        if(poMaker)
        {
            stDT_GEM_MAKER_DATA.wPileNum = poMaker->GetCurPileNum();
        }
        byMakerNum++;
    }

    return ERR_OPEN_GEM_MAKER_TAB::ID_SUCCESS;
}


