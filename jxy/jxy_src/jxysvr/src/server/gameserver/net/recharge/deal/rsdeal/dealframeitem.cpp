// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframeitem.h"

#include "logic/item/item.h"
#include "logic/item/itemmgr.h"
#include "logic/bag/bag.h"
#include <logic/player/player.h>

#include <sdloggerimpl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameItem::CDealFrameItem()
{

}

CDealFrameItem::~CDealFrameItem()
{

}

void CDealFrameItem::Release()
{

}

string CDealFrameItem::GetFrameType()
{
    return "item";
}

BOOL CDealFrameItem::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    //参数出错
    if (vecOpValue.size() < 2)
    {
        return FALSE;
    }

    UINT32 dwOpValue	= SDAtou(vecOpValue[0].c_str());
    UINT32 dwNum		= SDAtou(vecOpValue[1].c_str());


    if(dwOpValue > 0xFFFF)
    {
        return FALSE;
    }
    UINT16 wItemID = (UINT16)dwOpValue;
    switch(byOpType)
    {
    case RESOPTYPE_CKNUM:
    {
        if(dwOpValue < EQUIP_END_KINDID)
        {
            UINT16 wNum = 0;
            CPlayerEquipShmemMap& mapEquip = poPlayer->GetAllEquip();
            CEquip* poEquip = mapEquip.GetFistData();
            while(poEquip)
            {
                if(wItemID == poEquip->GetDT_EQUIP_DATA().wKindID)
                {
                    wNum++;
                }
                poEquip = mapEquip.GetNextData();
            }
            return wNum >= dwNum ? TRUE : FALSE;
        }
		else if ( dwOpValue < JEWELRY_END_KINDID && dwOpValue >= JEWELRY_BEGIN_KINDID)
		{
			UINT16 wNum = 0;
			CPlayerJewelryShmemMap& mapJewelry = poPlayer->GetAllJewelry();
			CJewelry* poJewelry = mapJewelry.GetFistData();
			while(poJewelry)
			{
				if(wItemID == poJewelry->GetDT_JEWELRY_DATA().wJewelryID)
				{
					wNum++;
				}
				poJewelry = mapJewelry.GetNextData();
			}
			return wNum >= dwNum ? TRUE : FALSE;
		}
        else
        {
            return poPlayer->GetBag().GetGoodsPileNum(wItemID) >= dwNum ? TRUE : FALSE;
        }
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        if(dwOpValue < EQUIP_END_KINDID)
        {
            CPlayerEquipShmemMap& mapEquip = poPlayer->GetAllEquip();
            CEquip* poEquip = mapEquip.GetFistData();
            while(poEquip)
            {
                if(0 == dwNum)
                {
                    break;
                }
                if(wItemID == poEquip->GetDT_EQUIP_DATA().wKindID)
                {
                    dwNum--;
					CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM , CRecordMgr::EDIT_GETGIVEGM, poEquip->GetItemKindID(), 0, 
						poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

					poPlayer->OnDeleteEquip(poEquip);
                }

                
                poEquip = mapEquip.GetNextData();
            }
        }
		else if ( dwOpValue < JEWELRY_END_KINDID && dwOpValue >= JEWELRY_BEGIN_KINDID)
		{
			UINT16 wNum = 0;
			CPlayerJewelryShmemMap& mapJewelry = poPlayer->GetAllJewelry();
			CJewelry* poJewelry = mapJewelry.GetFistData();
			while(poJewelry)
			{
				if(0 == dwNum)
				{
					break;
				}
				if(wItemID == poJewelry->GetDT_JEWELRY_DATA().wJewelryID)
				{
					dwNum--;
					CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM , CRecordMgr::EDIT_GETGIVEGM, poJewelry->GetDT_JEWELRY_DATA().wJewelryID, 0, 
						poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

					poPlayer->OnDeleteJewelry(poJewelry);
				}
				poJewelry = mapJewelry.GetNextData();
			}
		}
        else
        {
            CGoods* poGoods = poPlayer->GetBag().GetGoodsItem(wItemID);
            if(poGoods)
            {
                //UINT16 wCurNum = poGoods->GetCurPileNum();
                //poGoods->GetDT_GOODS_DATA().wPileCount = wCurNum >= dwNum ? wCurNum - dwNum1 : 0;
                //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM , CRecordMgr::EDIT_GETGIVEGM, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetDT_GOODS_DATA().wPileCount,
                //                                    poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

                //扣除操作
                poGoods->DecPileNum(dwNum, CRecordMgr::EDIT_MAKEGOODS);

                if(0 == poGoods->GetDT_GOODS_DATA().wPileCount)
                {
                    poPlayer->OnDeleteItem(poGoods);
                }
            }
        }

        return TRUE;
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        ECreateItemRet eRet;
        CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, wItemID, (UINT16)dwNum, eRet, 1, CRecordMgr::EAIT_GETGIVEGM);
        if(NULL == poItem)
        {
            SYS_CRITICAL(_SDT("[%s:%d]: CreateItemIntoBag failed, player[%u] ItemID[%u] ret[%d]\n"), MSG_MARK, poPlayer->GetID(), wItemID, eRet);
            return FALSE;
        }
        CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM , CRecordMgr::EDIT_GETGIVEGM, poItem->GetItemKindID(), poItem->GetCurPileNum(), 
            poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);
        return TRUE;
    }
    break;
	case RESOPTYPE_CK_ADD_LIMIT:
		{
			return CkAddLimit(vecOpValue[0],dwOpValue);
		}
		break;
    default:
        return FALSE;
        break;
    }


    return FALSE;
}

