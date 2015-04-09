#include "protocaltool.h"
#include "logic/player/player.h"
#include "logic/item/itemmgr.h"
#include "logic/pet/playerpet.h"


namespace ProtocalTool
{
	VOID FillBagItemData(CPlayer* pPlayer, UINT32 unItemId, DT_BAG_ITEM_INFO& rBagItemInfo) {
		if (NULL == pPlayer) {
			return;
		}

		CGoods* pGoods = pPlayer->GetBag().GetGoodsItem(unItemId);
		if (NULL == pGoods) {
			CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(unItemId, 0, rBagItemInfo.stItemData);
		} else {
			CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(unItemId, pGoods->GetCurPileNum(), rBagItemInfo.stItemData);
		}
	}

	VOID FillPetModuleRes(CPlayer* pPlayer, DT_PLAYER_RES_LST_DATA& rResLstData) {
		if (NULL == pPlayer) {
			return;
		}

		rResLstData.byResNum = 3;
		rResLstData.astPlayerRes[0].byResKind = ACTE_COIN;
		rResLstData.astPlayerRes[0].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
		rResLstData.astPlayerRes[1].byResKind = ACTE_GOLD;
		rResLstData.astPlayerRes[1].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
		rResLstData.astPlayerRes[2].byResKind = ACTE_PETSCORE;
		CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
		if (NULL == pPlayerPet) {
			return;
		}
		rResLstData.astPlayerRes[2].qwResValue = pPlayerPet->GetDT_PLAYER_PET_DATA().dwPetScore;
	}

}
