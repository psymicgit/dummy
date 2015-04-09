#include "goods.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/item/itemcomposepropmgr.h>
#include <sdrandom.h>
#include <common/server/utility.h>
#include <logic/crop/cropseedpropmgr.h>
#include "logic/record/recordmgr.h"
#include "goodspropmgr.h"
#include <logic/base/basedefine.h>
#include <logic/lua/luamgr.h>
#include <logic/other/state.h>
#include <logic/bag/bag.h>
#include <logic/item/itemmgr.h>
#include <logic/task/taskmgr.h>
#include <dll/sdframework/sdloggerimpl.h>
#include "logic/faction/faction.h"
#include "logic/pet/petmng.h"
CGoods::CGoods()
{
    memset(&m_stDT_GOODS_DATA, 0, sizeof(m_stDT_GOODS_DATA));
}

CGoods::~CGoods()
{

}

VOID CGoods::AddPileNum(UINT16 wPileNum, CRecordMgr::EAddItemType emOpType)
{
    UINT32 dwNum = m_stDT_GOODS_DATA.wPileCount + wPileNum;
    m_stDT_GOODS_DATA.wPileCount = dwNum >= 0xFFFF ? 0xFFFF : dwNum;
    m_stDT_GOODS_DATA.qwItemNewTime = SDTimeSecs();

    SGoodsProp* poGoods = GetGoodsProp();
    if (NULL == poGoods)
    {
        return;
    }

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_ADDITEM, emOpType, GetItemKindID(), GetCurPileNum(), 
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, wPileNum);
}

//删减叠加数
VOID CGoods::DecPileNum(UINT16 wPileNum, CRecordMgr::EDecItemType emOpType, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4)
{
    if (m_stDT_GOODS_DATA.wPileCount > wPileNum)
    {
        m_stDT_GOODS_DATA.wPileCount -= wPileNum;
    }
    else
    {
        m_stDT_GOODS_DATA.wPileCount = 0;
    }

    SGoodsProp* poGoods = GetGoodsProp();
    if (NULL == poGoods)
    {
        return;
    }

    CRecordMgr::Instance()->RecordInfo( m_poOwner->GetID(), ERM_DECITEM, emOpType, GetItemKindID(), GetCurPileNum(), 
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, wPileNum, qwParam2, qwParam3, qwParam4);
}


BOOL CGoods::Init(VOID* pData, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    SGoodsProp*	pstGoodsProp =  CGoodsPropMgr::Instance()->GetPropByPrimaryKey(((DT_GOODS_DATA*)(pData))->wKindID);
    if(NULL == pstGoodsProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstGoodsProp is NULL,KindID[%d]!"), MSG_MARK, ((DT_GOODS_DATA*)(pData))->wKindID);
        return FALSE;
    }
    memcpy(&m_stDT_GOODS_DATA, pData, sizeof(DT_GOODS_DATA));

    m_poOwner = poOwner;

    return TRUE;
}

UINT16 CGoods::Use(UINT8& byUseBoxFlag, DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo, UINT16 wUseNum)
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        RETURN_OTHER_ERR;
    }

    if(0 == m_stDT_GOODS_DATA.wPileCount)
    {
        return ERR_GOODS_USE::ID_GOODS_NOT_EXIST;
    }

    if(m_stDT_GOODS_DATA.wPileCount < wUseNum)
    {
        return ERR_GOODS_USE::ID_NOT_ENOUGH_NUM;
    }

    if(m_poOwner->GetLevel() < pstGoodsProp->wUseLevel)
    {
        return ERR_GOODS_USE::ID_NOT_REACH_USE_LEVEL;
    }

    BOOL bTodayFlag = FALSE;
    //每天使用次数判断
    if(IsToday(m_stDT_GOODS_DATA.qwLastUseTime))
    {
        bTodayFlag = TRUE;
        if((0 != pstGoodsProp->wUseTimesPerDay) && (m_stDT_GOODS_DATA.wLastUseNum >= pstGoodsProp->wUseTimesPerDay))
        {
            return ERR_GOODS_USE::ID_REACH_MAX_TODAY;
        }
    }
    //CD判断
    INT64 nPassTime = SDTimeSecs() - m_stDT_GOODS_DATA.qwLastUseTime;
    if(nPassTime < 0)//异常
    {
        nPassTime = pstGoodsProp->wUseCD;
    }
    if(nPassTime < pstGoodsProp->wUseCD)
    {
        return ERR_GOODS_USE::ID_IN_CD;
    }

    //使用
    UINT16 wErrCode = UseByMainKind(byUseBoxFlag, stBoxItemInfo, wUseNum);
    if(ERR_GOODS_USE::ID_SUCCESS == wErrCode)
    {
        m_stDT_GOODS_DATA.qwLastUseTime = SDTimeSecs();
        m_stDT_GOODS_DATA.qwLastUseTime++;
        m_stDT_GOODS_DATA.wPileCount -= wUseNum;

        CRecordMgr::Instance()->RecordInfo(m_poOwner->GetID(), ERM_DECITEM, CRecordMgr::EDIT_USE, m_stDT_GOODS_DATA.wKindID, m_stDT_GOODS_DATA.wPileCount, 
            m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, wUseNum);

        if(0 == m_stDT_GOODS_DATA.wPileCount)
        {
            m_poOwner->OnDeleteItem(this);
        }
    }

    return wErrCode;
}

UINT16 CGoods::UseByMainKind(UINT8& byUseBoxFlag, DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo, UINT16 wUseNum)
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        RETURN_OTHER_ERR;
    }

    switch(pstGoodsProp->byMainKindID)
    {
    case EGMK_PHYSTRENGTH:
    {
        m_poOwner->AddPhyStrength(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EAPST_USEGOOD, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_GOLD:
    {
        m_poOwner->AddGold(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EGGT_USEGOODS, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_COIN:
    {
        m_poOwner->AddCoin(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EACT_USEGOODS, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_DOUBLEEXPERIENCE:
    {
        m_poOwner->GetState().OnUseDoubleExpGoods(pstGoodsProp->dwValue * wUseNum);
    }
    break;
    case EGMK_ADDEXPERIENCE:
    {
        m_poOwner->AllotExperience(pstGoodsProp->dwValue * wUseNum);
    }
    break;
    case EGMK_RATEBOX:
    {
        byUseBoxFlag = 1;
        return UseRateBox(stBoxItemInfo);
    }
    break;
    case EGMK_BOX:
    {
        byUseBoxFlag = 1;
        return UseBox(stBoxItemInfo);
    }
    break;
    case EGMK_SCIENCE:
    {
        m_poOwner->AddScience(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EAST_USEGOODS, pstGoodsProp->wKindID, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_XIANGQIANGEM:
    {
        m_poOwner->AddBlueGas(pstGoodsProp->dwParam4 * wUseNum, CRecordMgr::EABGT_GEMCONVERT, pstGoodsProp->wKindID, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_BLUEGAS:
    {
        m_poOwner->AddBlueGas(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EABGT_USEGOODS, pstGoodsProp->wKindID, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_PURPLEGAS:
    {
        m_poOwner->AddPurpleGas(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EAPGT_USEGOODS, pstGoodsProp->wKindID, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_JINGJIE:
    {
        m_poOwner->AddJingJie(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EAJT_USEGOODS, pstGoodsProp->wKindID, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_GEMMAKER:
    {
        m_poOwner->AddBlueGas(pstGoodsProp->dwParam3 * wUseNum, CRecordMgr::EABGT_USEGOODS, pstGoodsProp->wKindID, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_STORY:
    {
        m_poOwner->AddStory(pstGoodsProp->dwValue * wUseNum, CRecordMgr::EABGT_USEGOODS, pstGoodsProp->wKindID, pstGoodsProp->wKindID, wUseNum);
    }
    break;
    case EGMK_DOORSTRIBUTE:
    {
        CFaction* poFaction = m_poOwner->GetFaction();
        if (NULL != poFaction)
        {
            poFaction->AddDoorsTribute(m_poOwner->GetID(), pstGoodsProp->dwValue * wUseNum, CRecordMgr::EAT_USE_GOODS);
            DT_FACTION_PLAYER_CLI stCli = {0};
            poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, m_poOwner->GetID());
            PKT_CLIGS_DOORSTRIBUTE_SYNC_NTF stNtf;
            stNtf.dwCurDoorTribute = stCli.dwDoorsTributeValues;
            stNtf.qwSumDoorsTribute = stCli.qwDoorsTributeSum;
            m_poOwner->SendMsg((TCHAR*)&stNtf, CLIGS_DOORSTRIBUTE_SYNC_NTF);
        }
    }
    break;
	case EGMK_NORMAL_PET_BAG:
	case EGMK_PET_NORMAL_BABY_EGG:
	case EGMK_PET_UNNORMAL_BABY_EGG:
		return CPetMng::Instance()->UsePetItem(m_poOwner, pstGoodsProp);
	break;
    default:
        //异常
        return ERR_GOODS_USE::ID_CAN_NOT_USE_IN_BAG;
        break;
    }

    return ERR_GOODS_USE::ID_SUCCESS;
}

UINT16 CGoods::UseBox(DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo)
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        RETURN_OTHER_ERR;
    }

    SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(pstGoodsProp->wKindID);
    if(NULL == pstItemComposeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL!"), MSG_MARK);
        RETURN_OTHER_ERR;
    }

    CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
    if(0 == mapComposeItem.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, pstGoodsProp->wKindID);
        RETURN_OTHER_ERR;
    }

    INT32 nGap = (INT32)(m_poOwner->GetBag().GetIdleSize()) - (INT32)(mapComposeItem.size());
    if(nGap < 0)
    {
        //判断背包中是否存在该物品，若存在则可少使用一个格子
        CBag& oBag = m_poOwner->GetBag();
        for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++)
        {
            //只有物品有效，装备叠加数为1，则只找Goods
            if(oBag.GetGoodsItem(itr->first))
            {
                nGap++;
            }
        }
    }

    //背包容量不足
    if(nGap < 0)
    {
        return ERR_GOODS_USE::ID_BAG_FULL;
    }

    for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++)
    {
        //若是系统可以直接使用的物品，则使用
        if(CItemMgr::Instance()->SysUseGoods(m_poOwner, itr->first, itr->second.wItemNum))
        {
            CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->first, itr->second.wItemNum, stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum]);
        }
        else
        {
            ECreateItemRet eRet;
            CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, itr->first, itr->second.wItemNum, eRet, 0, CRecordMgr::EAIT_BOX, m_stDT_GOODS_DATA.wKindID);
            if(NULL == poItem)
            {
                //异常
                SYS_CRITICAL(_SDT("[%s: %d]: CreateItemIntoBag failed[%d]!"), MSG_MARK, itr->first);
            }
            //装备没有叠加数，可以直接poEquip->GetDT_EQUIP_DATA_CLI(stEquip);
            if(EIK_EQUIP == poItem->GetItemKind())
            {
                CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
                if(NULL == poEquip)
                {
                    //异常
                    continue;
                }
                stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum].byItemType = EIK_EQUIP;
                DT_EQUIP_DATA_CLI& stEquip = stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum].stItemInfo.stEquip ;
                poEquip->GetDT_EQUIP_DATA_CLI(stEquip);
            }
            else
            {
                //内部礼包，返回的是礼包的物品
                SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(itr->first);
                if((pstGoodsProp) && (EGMK_INNERBOX == pstGoodsProp->byMainKindID))
                {
                    CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(poItem->GetItemKindID(), 1, stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum]);
                }
                else
                {
                    CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->first, itr->second.wItemNum, stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum]);
                }
            }
        }

        stBoxItemInfo.byItemNum++;
    }

    return ERR_GOODS_USE::ID_SUCCESS;
}

UINT16 CGoods::UseRateBox(DT_ITEM_DATA_LIST_CLI2& stBoxItemInfo)
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        RETURN_OTHER_ERR;
    }

    stBoxItemInfo.byItemNum = 0;

    SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(pstGoodsProp->wKindID);
    if(NULL == pstItemComposeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL!"), MSG_MARK);
        RETURN_OTHER_ERR;
    }

    CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
    if(0 == mapComposeItem.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, pstGoodsProp->wKindID);
        RETURN_OTHER_ERR;
    }

    //计算随机值
    CRandomVec vecRandom;
    for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++)
    {
        vecRandom.push_back(itr->second.wRate);
    }

    //随机
    INT32 nRandomHitIdx = GetRandomIdxMustHit(vecRandom);
    if(RANDOM_UNHIT == nRandomHitIdx)
    {
        //随机不到物品也算使用成功
        return ERR_GOODS_USE::ID_SUCCESS;
    }

    stBoxItemInfo.byItemNum = 0;
    UINT8 byIdx = 0;
    for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++, byIdx++)
    {
        if(byIdx == nRandomHitIdx)
        {
            //若是系统可以直接使用的物品，则使用
            if(CItemMgr::Instance()->SysUseGoods(m_poOwner, itr->first, itr->second.wItemNum))
            {
                CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->first, itr->second.wItemNum, stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum++]);
            }
            else
            {
                UINT16 wItemKindID = itr->first;
                ECreateItemRet eRet;

                CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, wItemKindID, itr->second.wItemNum, eRet, 0, CRecordMgr::EAIT_BOX, m_stDT_GOODS_DATA.wKindID );
                if(NULL == poItem)
                {
                    if(ECIR_FAILED_BAG_FULL == eRet)
                    {
                        return ERR_GOODS_USE::ID_BAG_FULL;
                    }
                    else
                    {
                        //异常
                        SYS_CRITICAL(_SDT("[%s: %d]: CreateItemIntoBag failed[%d]!"), MSG_MARK, wItemKindID);
                        RETURN_OTHER_ERR;
                    }
                }

                //内部礼包，返回的是礼包的物品
                SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(itr->first);
                if((pstGoodsProp) && (EGMK_INNERBOX == pstGoodsProp->byMainKindID))
                {
                    CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(poItem->GetItemKindID(), 1, stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum++]);
                }
                else
                {
                    CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->first, itr->second.wItemNum, stBoxItemInfo.astItemList[stBoxItemInfo.byItemNum++]);
                }
            }

            break; //只爆出1个物品
        }
    }

    return ERR_GOODS_USE::ID_SUCCESS;
}

VOID CGoods::GetDT_GOODS_DATA_CLI(DT_GOODS_DATA_CLI& stDT_GOODS_DATA_CLI)
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        return ;
    }

    CGoodsPropMgr::Instance()->FillGoodsData( pstGoodsProp, stDT_GOODS_DATA_CLI, m_stDT_GOODS_DATA.wPileCount );
}

UINT16 CGoods::Sell(UINT16 wNum, DT_ITEM_NUM_DATA_LIST_CLI &stRecvItemNumList)
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        RETURN_OTHER_ERR;
    }

    if(NULL == m_poOwner)
    {
        RETURN_OTHER_ERR;
    }

    if(wNum > m_stDT_GOODS_DATA.wPileCount)
    {
        return ERR_SELL_ITEM::ID_OVER_CUR_NUM;
    }

    if(0 == pstGoodsProp->dwRecoverPrice)
    {
        return ERR_SELL_ITEM::ID_SELL_PROHIBIT;
    }

    //给钱
    //m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin += (pstGoodsProp->wRecoverPrice * wNum);
    m_poOwner->AddCoin(pstGoodsProp->dwRecoverPrice * wNum, CRecordMgr::EACT_SELLGOODS, pstGoodsProp->wKindID, wNum);

    m_stDT_GOODS_DATA.wPileCount -= wNum;
    if(0 == m_stDT_GOODS_DATA.wPileCount)
    {
        //清除
        m_poOwner->OnDeleteItem(this);

    }
    return ERR_SELL_ITEM::ID_SUCCESS;
}

SGoodsProp* CGoods::GetGoodsProp()
{
    SGoodsProp* pstGoodsProp =  CGoodsPropMgr::Instance()->GetPropByPrimaryKey(m_stDT_GOODS_DATA.wKindID);
    if(NULL == pstGoodsProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstGoodsProp is NULL,KindID[%d]!"), MSG_MARK, m_stDT_GOODS_DATA.wKindID);
    }

    return pstGoodsProp;
}

UINT8 CGoods::GetGoodsMainKindID()
{
    SGoodsProp* pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        return 0;
    }
    return pstGoodsProp->byMainKindID;
}

void CGoods::GetRecoverScience(UINT32 & dwMinScience, UINT32 & dwMaxScience)//回收器魂
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        return ;
    }
    dwMinScience = (UINT32)(pstGoodsProp->dwParam1 * 0.5);
    dwMaxScience = (UINT32)(pstGoodsProp->dwParam1 * 1.5);
}


VOID CGoods::GetDT_GOOD_MELT_DATA_CLI(DT_GOOD_MELT_DATA_CLI& stDT_GOOD_MELT_DATA_CLI)//获取装备熔炼给客户端的信息
{
    SGoodsProp*	pstGoodsProp =  GetGoodsProp();
    if(NULL == pstGoodsProp)
    {
        return ;
    }

    stDT_GOOD_MELT_DATA_CLI.wKindID		=	m_stDT_GOODS_DATA.wKindID;
    stDT_GOOD_MELT_DATA_CLI.wNum		=	m_stDT_GOODS_DATA.wPileCount;
    GetRecoverScience(stDT_GOOD_MELT_DATA_CLI.dwMinScience, stDT_GOOD_MELT_DATA_CLI.dwMaxScience);
    stDT_GOOD_MELT_DATA_CLI.dwMeltGold	=	CLuamgr::Instance()->GetMeltGold(stDT_GOOD_MELT_DATA_CLI.dwMaxScience);
}

UINT16	CGoods::Melt(UINT8 byGoldFlag)//熔练
{
    if(NULL == m_poOwner)
    {
        RETURN_OTHER_ERR;
    }

    UINT32	dwDisScience = 0;
    UINT32	dwMinScience = 0;
    UINT32	dwMaxScience = 0;
    GetRecoverScience(dwMinScience, dwMaxScience);

    if (dwMaxScience > dwMinScience)
    {
        dwDisScience = dwMaxScience - dwMinScience;
    }
    else
    {
        dwDisScience = 0;
    }

    UINT32	dwScience = 0;
    if(byGoldFlag)
    {
        dwScience = dwMaxScience;
    }
    else
    {
        //对数值放大100倍，防止大小被舍掉
        dwDisScience = dwDisScience * 100;
        //共分成五档
        dwDisScience = dwDisScience / 10;

        //计算随机值
        CRandomVec vecRandom;
        vecRandom.push_back(10);
        vecRandom.push_back(20);
        vecRandom.push_back(40);
        vecRandom.push_back(20);
        vecRandom.push_back(10);

        //随机
        INT32 nRandomHitIdx = GetRandomIdxMustHit(vecRandom);
        if(RANDOM_UNHIT == nRandomHitIdx)
        {
            RETURN_OTHER_ERR;
        }

        //随机的值
        UINT32 dwHitScience = 0;
        switch(nRandomHitIdx)
        {
        case 0:
            dwHitScience = SDRandom(dwDisScience);
            break;
        case 1:
            dwHitScience = dwDisScience + SDRandom(dwDisScience * 2);
            break;
        case 2:
            dwHitScience = 3 * dwDisScience + SDRandom(dwDisScience * 4);
            break;
        case 3:
            dwHitScience = 7 * dwDisScience + SDRandom(dwDisScience * 2);
            break;
        case 4:
            dwHitScience = 9 * dwDisScience + SDRandom(dwDisScience);
            break;
        default:
            break;
        }
        dwScience = dwHitScience / 100;
        dwScience += dwMinScience;
    }

    m_poOwner->AddScience(dwScience, CRecordMgr::EAST_MELT, m_stDT_GOODS_DATA.wKindID, 0, 0);
    m_poOwner->AddGuideRecord(EGF_MELTING);
    CTaskMgr::Instance()->OnMelt(m_poOwner);

    //记录熔炼装备日志
    CRecordMgr::Instance()->RecordInfo(m_poOwner->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MELT, GetItemKindID(), 0, 
        m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, 1, dwScience);
    m_stDT_GOODS_DATA.wPileCount--;
    if(0 == m_stDT_GOODS_DATA.wPileCount)
    {
        //清除
        m_poOwner->OnDeleteItem(this);
    }
    return ERR_MELT_EQUIP::ID_SUCCESS;
}

