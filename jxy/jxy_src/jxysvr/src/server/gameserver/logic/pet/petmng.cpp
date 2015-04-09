#include "petmng.h"
#include "petpropmng.h"
#include "logic/pet/playerpet.h"
#include "logic/base/randomodds.h"
#include "logic/item/itemmgr.h"
#include "logic/other/locale.h"
#include "pet.h"
#include "logic/lua/luamgr.h"
#include "logic/base/protocaltool.h"
#include "logic/vip/vippropmgr.h"
#include <sstream>
#include "logic/player/player.h"
#include "logic/pet/neidan.h"

CPetMng::CPetMng() {

}

CPetMng::~CPetMng() {

}

UINT8 CPetMng::OpenPetRep(CPlayer* pPlayer, const PKT_CLIGS_PETREP_INFO_REQ& rMsgReq, PKT_CLIGS_PETREP_INFO_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_PETREP_INFO_REQ::ID_UNKNOWN_ERR;
	}
	CBuild* poBuild = pPlayer->GetBuild( EBK_PET );
	if ( NULL == poBuild)
	{
		return ERR_CLIGS_PETREP_INFO_REQ::ID_IS_UNOPEN;
	}
	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_PETREP_INFO_REQ::ID_IS_UNOPEN;
	}

	DT_PLAYER_PET_DATA& rPetData = pPlayerPet->GetDT_PLAYER_PET_DATA();
	if (0 == rPetData.qwGoodLastTime) {
		rPetData.qwGoodLastTime = SDNow().GetTimeValue();
	}

    pPlayerPet->CheckCrossDay();

	pPlayer->GetLocale().OnEnterPetRep();
	pPlayer->GetSyncResource(rMsgAck.stResInfo);

	const PetProductCfg* pNormalProductCfg = CPetPropMng::Instance()->GetProductCfg(pPlayer->GetLevel(), 1);
	if (NULL == pNormalProductCfg) {
		return ERR_CLIGS_PETREP_INFO_REQ::ID_UNKNOWN_ERR;;
	}

	const PetProductCfg* pGoodProductCfg = CPetPropMng::Instance()->GetProductCfg(pPlayer->GetLevel(), 2);
	if (NULL == pGoodProductCfg) {
		return ERR_CLIGS_PETREP_INFO_REQ::ID_UNKNOWN_ERR;;
	}

	rMsgAck.dwNormalCostGold = pNormalProductCfg->unCostGold;
	rMsgAck.dwGoodCostGold = pGoodProductCfg->unCostGold;
	if ((SDNow().GetTimeValue() - rPetData.qwNormalLastTime) > (pNormalProductCfg->usFreeTimeInterval * 60)) {
		rMsgAck.dwNormalLeftTime = 0;
	} else {
		rMsgAck.dwNormalLeftTime = UINT32((pNormalProductCfg->usFreeTimeInterval * 60) - (SDNow().GetTimeValue() - rPetData.qwNormalLastTime));
	}

	if ((SDNow().GetTimeValue() - rPetData.qwGoodLastTime) > (pGoodProductCfg->usFreeTimeInterval * 60)) {
		rMsgAck.dwGoodLeftTime = 0;
	} else {
		rMsgAck.dwGoodLeftTime = UINT32((pGoodProductCfg->usFreeTimeInterval * 60) - (SDNow().GetTimeValue() - rPetData.qwGoodLastTime));
	}

	rMsgAck.dwNormalFreeCnt = rPetData.wFreeNormalProductCnt;
	rMsgAck.byRuleFlag = pPlayerPet->GetRuleState();
	return ERR_CLIGS_PETREP_INFO_REQ::ID_SUCCESS;
}

UINT8 CPetMng::ProductPet(CPlayer* pPlayer, const PKT_CLIGS_PRODUCT_PET_REQ& rMsgReq, PKT_CLIGS_PRODUCT_PET_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_PETREP_INFO_REQ::ID_IS_UNOPEN;
	}

	// 取出产出配置
	const PetProductCfg* pProductCfg = CPetPropMng::Instance()->GetProductCfg(pPlayer->GetLevel(), rMsgReq.byProductType);
	if (NULL == pProductCfg) {
		return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;;
	}

	DT_PLAYER_PET_DATA& rPetData = pPlayerPet->GetDT_PLAYER_PET_DATA();

	// 如果过了0点重置产出数（这个值为了计算获取的额外的物品）
    pPlayerPet->CheckCrossDay();

	bool bNormalProduct = 1 == rMsgReq.byProductType;
	// 额外物品配置
	const PetProductExtraCfg* pExtraCfg = CPetPropMng::Instance()->GetProductExtraCfg(
		pProductCfg->ucProductPhrase
		, !bNormalProduct
		, bNormalProduct ? rPetData.wNormalProductNum : rPetData.wGoodProductNum);

	UINT16 usGetPetCnt = (bNormalProduct ? 1 : 10);
	RandomOdds<UINT32> randomPetTypeOdds;
	for (size_t n = 0; n < pProductCfg->vGetPetType.size(); ++n) {
		randomPetTypeOdds.PushOdds(pProductCfg->vGetPetType[n], pProductCfg->vPetWeight[n]);
	}

	RandomOdds<UINT32> randomPetQualityOdds;
	for (size_t n = 0; n < pProductCfg->vGetPetQuality.size(); ++n) {
		randomPetQualityOdds.PushOdds(pProductCfg->vGetPetQuality[n], pProductCfg->vQualityWeight[n]);
	}
	
	// 宠物类型
	std::vector<UINT32> vAddPetType;
	for (size_t n = 0; n < usGetPetCnt; ++ n) {
		UINT32* pHitOdds = randomPetTypeOdds.GetHitOdds();
		if (NULL == pHitOdds) {
			return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;
		}
		vAddPetType.push_back(*pHitOdds);
	}

	// 宠物背包空间验证
	if (pPlayerPet->GetPetBagIdleSize() < vAddPetType.size()) {
		return ERR_CLIGS_PRODUCT_PET_REQ::ID_NO_ENOUGHT_PETBAG_SIZE;
	}

	// 背包空间验证
	if (pPlayer->GetBag().GetIdleSize() < 10) {
		return ERR_CLIGS_PRODUCT_PET_REQ::ID_NO_ENOUGHT_BAG_SIZE;
	}

	// 塞入背包id
	std::vector<UINT32> vAddItemId;
	if (NULL != pExtraCfg) {
		for (size_t n = 0; n < pExtraCfg->vExtraItemNum.size(); ++ n) {
			for (size_t m = 0; m < pExtraCfg->vExtraItemNum[n]; ++ m) {
				vAddItemId.push_back(pExtraCfg->vExtraItemId[n]);
			}
		}
	}

	for (size_t n = 0; n < pProductCfg->vGetItemId.size(); ++ n) {
		if (pProductCfg->vGetItemPercent[n] >= (UINT32)Random(0, 100)) {
			for (size_t m = 0; m < pProductCfg->vGetItemNum[n]; ++ m) {
				vAddItemId.push_back(pProductCfg->vGetItemId[n]);
			}
		}
	}

	// 是否免费（每个时间间隔过后都有一次免费）
	bool bFree = false;
	if (bNormalProduct) {
		if ((SDNow().GetTimeValue() - rPetData.qwNormalLastTime) > (pProductCfg->usFreeTimeInterval * 60)
			&& rPetData.wFreeNormalProductCnt > 0)
		{
			bFree = true;	
		}
	} else {
		if ((SDNow().GetTimeValue() - rPetData.qwGoodLastTime) > (pProductCfg->usFreeTimeInterval * 60)) {
			bFree = true;	
		}
	}
	
	UINT32 unCostGold = pProductCfg->unCostGold;
	if (!bFree) {
		// 元宝验证
		if (pPlayer->GetDT_PLAYER_BASE_DATA().qwGold < unCostGold) {
			return ERR_CLIGS_PRODUCT_PET_REQ::ID_NO_ENOUGHT_GOLD;
		}
	}

	// 道具塞进背包
	for (size_t n = 0; n < vAddItemId.size(); ++ n) {
		CGoods* pGoods = pPlayer->GetBag().GetGoodsItem(vAddItemId[n]);
		if (NULL == pGoods) {
			ECreateItemRet eItemRet = ECIR_SUCCESS;
			pGoods = (CGoods*)CItemMgr::Instance()->CreateItemIntoBag(pPlayer, vAddItemId[n], 1
				, eItemRet, 0, CRecordMgr::EAIT_PRODUCT_PET);

			if (ECIR_SUCCESS != eItemRet) {
				PRINT_ERR_INFO;
				return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;
			}
		} else { // 堆叠
			pGoods->AddPileNum(1, CRecordMgr::EAIT_PRODUCT_PET);
		}

		if (NULL != pGoods && n < MAX_PRODUCTPET_EXTRA_ITEM_NUM) {
			pGoods->GetDT_GOODS_DATA_CLI(rMsgAck.astPetGood[rMsgAck.byGoodNum]);
			rMsgAck.astPetGood[rMsgAck.byGoodNum].wPileCount = 1;
			++ rMsgAck.byGoodNum;
		}
	}

	std::vector<const CPet*> vNewPet;
	// 宠物塞入宠物背包
	for (size_t n = 0; n < vAddPetType.size(); ++n) {
		UINT32* pRandomPetQuality = randomPetQualityOdds.GetHitOdds();
		if (NULL == pRandomPetQuality) {
			return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;
		}

		std::vector<const PetBaseCfg*> vPetBaseCfgList;
		const PetBaseCfgList& rPetBaseCfgList = CPetPropMng::Instance()->GetPetBaseCfgList();
		for (PetBaseCfgList::const_iterator itor = rPetBaseCfgList.begin(), end = rPetBaseCfgList.end()
			; end != itor
			; ++ itor)
		{
			
			if (*pRandomPetQuality == itor->second->ucPetQuality
				&& vAddPetType[n] == itor->second->ucPetType)
			{
				vPetBaseCfgList.push_back(itor->second);
			}
		}

		if (vPetBaseCfgList.empty()) {
			return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;
		}

		const PetBaseCfg* pBaseCfg = vPetBaseCfgList[Random(0, vPetBaseCfgList.size() - 1)];
		if (NULL == pBaseCfg) {
			return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;
		}

		const CPet* pNewPet = CreateAndAddPet(pPlayer, pBaseCfg, 1);
		if (NULL == pNewPet) {
			for (size_t n = 0; n < vNewPet.size(); ++ n) {
				pPlayerPet->RemovePet(vNewPet[n]->GetDT_PET_DATA().wPetId);
			}
			return ERR_GOODS_USE::ID_UNKOWND_ERR;
		} else {
			vNewPet.push_back(pNewPet);

			CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PETREP_ADDPET, 1
				, 1, 1
				, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
				, pNewPet->GetDT_PET_DATA().wPetId, pNewPet->GetDT_PET_DATA().wPetCfgId, 0, 0, "");

		}
	}

	for (size_t n = 0; n < vNewPet.size() && n < MAX_PRODUCTPET_NUM; ++ n) {
		vNewPet[n]->FillDT_PET_CLI_DATA(rMsgAck.astPetData[n]);
	}
	rMsgAck.byPetNum = vNewPet.size();

	if (!bFree) {
		pPlayer->DecGold(unCostGold, CRecordMgr::EDGT_PRODUCT_PET);
	} else {
		if (bNormalProduct) {
			rPetData.qwNormalLastTime = SDNow().GetTimeValue();
		} else {
			rPetData.qwGoodLastTime = SDNow().GetTimeValue();
		}
	}

	rPetData.qwProductLastTime = SDNow().GetTimeValue();
	if (bNormalProduct) {
		++ rPetData.wNormalProductNum;
		if (bFree && rPetData.wFreeNormalProductCnt > 0) {
			-- rPetData.wFreeNormalProductCnt;
		}
	} else {
		++ rPetData.wGoodProductNum;
	}
	
	// 加积分
	rPetData.dwPetScore += pProductCfg->unScore;
	pPlayerPet->UpdatePetLstData();

	const PetProductCfg* pNormalProductCfg = CPetPropMng::Instance()->GetProductCfg(pPlayer->GetLevel(), 1);
	if (NULL == pNormalProductCfg) {
		return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;;
	}

	const PetProductCfg* pGoodProductCfg = CPetPropMng::Instance()->GetProductCfg(pPlayer->GetLevel(), 2);
	if (NULL == pGoodProductCfg) {
		return ERR_CLIGS_PRODUCT_PET_REQ::ID_UNKNOWN_ERR;;
	}
	rMsgAck.dwNormalCostGold = pNormalProductCfg->unCostGold;
	rMsgAck.dwGoodCostGold = pGoodProductCfg->unCostGold;
	if ((SDNow().GetTimeValue() - rPetData.qwNormalLastTime) > (pNormalProductCfg->usFreeTimeInterval * 60)) {
		rMsgAck.dwNormalLeftTime = 0;
	} else {
		rMsgAck.dwNormalLeftTime = UINT32((pNormalProductCfg->usFreeTimeInterval * 60) - (SDNow().GetTimeValue() - rPetData.qwNormalLastTime));
	}

	if ((SDNow().GetTimeValue() - rPetData.qwGoodLastTime) > (pGoodProductCfg->usFreeTimeInterval * 60)) {
		rMsgAck.dwGoodLeftTime = 0;
	} else {
		rMsgAck.dwGoodLeftTime = UINT32((pGoodProductCfg->usFreeTimeInterval * 60) - (SDNow().GetTimeValue() - rPetData.qwGoodLastTime));
	}

	rMsgAck.dwNormalFreeCnt = rPetData.wFreeNormalProductCnt;

	pPlayer->GetSyncResource(rMsgAck.stResInfo);

	pPlayer->AddGuideRecord(EGF_PET);
	pPlayer->CKGuide(EGF_PET_FIGHT);
	return ERR_CLIGS_PRODUCT_PET_REQ::ID_SUCCESS;
}

UINT8 CPetMng::OpenRefinePetPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_REFINEPET_REQ& rMsgReq, PKT_CLIGS_OPEN_REFINEPET_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_REFINEPET_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_OPEN_REFINEPET_REQ::ID_IS_UNOPEN;
	}

	const MapPet& mpPet = pPlayerPet->GetAllPet();
	for (MapPet::const_iterator itor = mpPet.begin(), end = mpPet.end()
		; end != itor && rMsgAck.stAllPetInfo.wPetNum <= MAX_PET_NUM
		; ++ itor)
	{
		if (NULL == itor->second) {
			return ERR_CLIGS_OPEN_REFINEPET_REQ::ID_UNKNOWN_ERR;
		}

		const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(itor->second->GetDT_PET_DATA().wPetCfgId);
		if (NULL == pBaseCfg 
			|| pBaseCfg->ucPetQuality <= ePQ_NormalPet
			|| pBaseCfg->ucPetQuality > ePQ_VariantPet
			|| itor->second->GetDT_PET_DATA().byStarLv > 1
			|| itor->second->GetDT_PET_DATA().wLv > 1)
		{
			continue;
		}

		itor->second->FillDT_PET_CLI_DATA(rMsgAck.stAllPetInfo.astPetData[rMsgAck.stAllPetInfo.wPetNum]);
		++ rMsgAck.stAllPetInfo.wPetNum;
	}
	return ERR_CLIGS_OPEN_REFINEPET_REQ::ID_SUCCESS;
}

UINT8 CPetMng::RefinePet(CPlayer* pPlayer, const PKT_CLIGS_REFINEPET_REQ& rMsgReq, PKT_CLIGS_REFINEPET_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_REFINEPET_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet1 = pPlayerPet->GetPet(rMsgReq.wPetId1);
	CPet* pPet2 = pPlayerPet->GetPet(rMsgReq.wPetId2);
	if (NULL == pPet1 || NULL == pPet2) {
		return ERR_CLIGS_REFINEPET_REQ::ID_NO_THIS_PET;
	}

	const PetBaseCfg* pPetCfg1 = CPetPropMng::Instance()->GetPetBaseCfg(pPet1->GetDT_PET_DATA().wPetCfgId);
	const PetBaseCfg* pPetCfg2 = CPetPropMng::Instance()->GetPetBaseCfg(pPet2->GetDT_PET_DATA().wPetCfgId);
	if (NULL == pPetCfg1 || NULL == pPetCfg2
		|| pPetCfg1->ucPetQuality <= ePQ_NormalPet || pPetCfg2->ucPetQuality > ePQ_VariantPet
		|| pPetCfg2->ucPetQuality <= ePQ_NormalPet || pPetCfg2->ucPetQuality > ePQ_VariantPet
		|| GetPetTypeLevel(pPetCfg1->ucPetType) != GetPetTypeLevel(pPetCfg2->ucPetType)
		|| pPetCfg1->ucPetQuality != pPetCfg2->ucPetQuality)
	{
		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
	}

	const PetRefineCfg* pRefineCfg = CPetPropMng::Instance()->GetRefineCfg(GetPetTypeLevel(pPetCfg1->ucPetType), pPetCfg1->ucPetQuality);
	if (NULL == pRefineCfg) {
		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
	}

	RandomOdds<UINT32> randomPetRefineOdds;
	for (size_t n = 0; n < pRefineCfg->vGetPetData.size(); ++ n) {
		randomPetRefineOdds.PushOdds(n, pRefineCfg->vGetPetWeight[n]);
	}

	UINT32* pIndex = randomPetRefineOdds.GetHitOdds();
	if (NULL == pIndex || *pIndex >= pRefineCfg->vGetPetData.size()) {
		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;;
	}

	UINT8 ucRefinePetType = getRandomPetTypeByTypeLevel(pRefineCfg->vGetPetData[*pIndex].ucPetTypeLevel);
	UINT8 ucRefinePetQuality = pRefineCfg->vGetPetData[*pIndex].ucPetQuality;

	std::vector<const PetBaseCfg*> vPetBaseCfgList;
	const PetBaseCfgList& rPetBaseCfgList = CPetPropMng::Instance()->GetPetBaseCfgList();
	for (PetBaseCfgList::const_iterator itor = rPetBaseCfgList.begin(), end = rPetBaseCfgList.end()
		; end != itor
		; ++ itor)
	{
		if (ucRefinePetQuality == itor->second->ucPetQuality
			&& ucRefinePetType == itor->second->ucPetType)
		{
			vPetBaseCfgList.push_back(itor->second);
		}
	}

	if (vPetBaseCfgList.empty()) {
		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
	}
	const PetBaseCfg* pBaseCfg = vPetBaseCfgList[Random(0, vPetBaseCfgList.size() - 1)];
	if (NULL == pBaseCfg) {
		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
	}

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_REFINE_REMOVEPET, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, pPet1->GetDT_PET_DATA().wPetId, pPet1->GetDT_PET_DATA().wPetCfgId, 0, 0, "");
	pPlayerPet->RemovePet(rMsgReq.wPetId1);
	
	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_REFINE_REMOVEPET, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, pPet1->GetDT_PET_DATA().wPetId, pPet1->GetDT_PET_DATA().wPetCfgId, 0, 0, "");
	pPlayerPet->RemovePet(rMsgReq.wPetId2);

	const CPet* pNewPet = CreateAndAddPet(pPlayer, pBaseCfg, 1);
	if (NULL == pNewPet) {
		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
	}

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_REFINE_ADDPET, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, pNewPet->GetDT_PET_DATA().wPetId, pNewPet->GetDT_PET_DATA().wPetCfgId, 0, 0, "");


	pNewPet->FillDT_PET_CLI_DATA(rMsgAck.stRefineGetPet);
	pPlayerPet->UpdatePetLstData();
// 	if (!fillPlayerData(pPlayerPet, rMsgAck.stAllPetInfo)) {
// 		return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
// 	}

	const MapPet& mpPet = pPlayerPet->GetAllPet();
	for (MapPet::const_iterator itor = mpPet.begin(), end = mpPet.end()
		; end != itor && rMsgAck.stAllPetInfo.wPetNum <= MAX_PET_NUM
		; ++ itor)
	{
		if (NULL == itor->second) {
			return ERR_CLIGS_REFINEPET_REQ::ID_UNKNOWN_ERR;
		}

		const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(itor->second->GetDT_PET_DATA().wPetCfgId);
		if (NULL == pBaseCfg 
			|| pBaseCfg->ucPetQuality <= ePQ_NormalPet
			|| pBaseCfg->ucPetQuality > ePQ_VariantPet
			|| itor->second->GetDT_PET_DATA().byStarLv > 1
			|| itor->second->GetDT_PET_DATA().wLv > 1)
		{
			continue;
		}

		itor->second->FillDT_PET_CLI_DATA(rMsgAck.stAllPetInfo.astPetData[rMsgAck.stAllPetInfo.wPetNum]);
		++ rMsgAck.stAllPetInfo.wPetNum;
	}
	return ERR_CLIGS_REFINEPET_REQ::ID_SUCCESS;
}

UINT8 CPetMng::OpenPlayerPetPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ& rMsgReq, PKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_PLAYERPET_PANEL_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_OPEN_PLAYERPET_PANEL_REQ::ID_IS_UNOPEN;
	}

	const CPet* pFightPet = pPlayerPet->GetFightPet();
	if (NULL == pFightPet) {
		return ERR_CLIGS_OPEN_PLAYERPET_PANEL_REQ::ID_NO_FIGHT_PET;
	}

	const PetBaseCfg* pPetBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(pFightPet->GetDT_PET_DATA().wPetCfgId);
	if (NULL == pPetBaseCfg) {
		return ERR_CLIGS_OPEN_PLAYERPET_PANEL_REQ::ID_UNKNOWN_ERR;
	}

	pFightPet->FillDT_PET_CLI_DATA(rMsgAck.stPetInfo);

	return ERR_CLIGS_OPEN_PLAYERPET_PANEL_REQ::ID_SUCCESS;
}

UINT8 CPetMng::RequestAllPetInfo(CPlayer* pPlayer, const PKT_CLIGS_REQUEST_ALL_PET_INFO_REQ& rMsgReq, PKT_CLIGS_REQUEST_ALL_PET_INFO_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_REQUEST_ALL_PET_INFO_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_REQUEST_ALL_PET_INFO_REQ::ID_IS_UNOPEN;
	}

	if (!fillPlayerData(pPlayerPet, rMsgAck.stAllPetInfo)) {
		return ERR_CLIGS_REQUEST_ALL_PET_INFO_REQ::ID_UNKNOWN_ERR;
	}

	return ERR_CLIGS_REQUEST_ALL_PET_INFO_REQ::ID_SUCCESS;
}

UINT8 CPetMng::UsePetItem(CPlayer* pPlayer, const SGoodsProp* pGoodsProp) {
	if (NULL == pPlayer || NULL == pGoodsProp) {
		return ERR_GOODS_USE::ID_UNKOWND_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_GOODS_USE::ID_UNKOWND_ERR;
	}

	if (pPlayerPet->IsPetBagFull()) {
		return ERR_GOODS_USE::ID_PET_BAG_FULL;
	}

	std::vector<const PetBaseCfg*> vPetBaseCfgList;
	const PetBaseCfgList& rPetBaseCfgList = CPetPropMng::Instance()->GetPetBaseCfgList();
	for (PetBaseCfgList::const_iterator itor = rPetBaseCfgList.begin(), end = rPetBaseCfgList.end()
		; end != itor
		; ++ itor)
	{
		if (pGoodsProp->dwParam1 == itor->second->ucPetQuality
			&& pGoodsProp->dwParam2 == itor->second->ucPetType)
		{
			vPetBaseCfgList.push_back(itor->second);
		}
	}

	if (vPetBaseCfgList.empty()) {
		return ERR_GOODS_USE::ID_UNKOWND_ERR;
	}

	const PetBaseCfg* pBaseCfg = vPetBaseCfgList[Random(0, vPetBaseCfgList.size() - 1)];
	if (NULL == pBaseCfg) {
		return ERR_GOODS_USE::ID_UNKOWND_ERR;
	}

	const CPet* pNewPet = CreateAndAddPet(pPlayer, pBaseCfg, 1);
	if (NULL == pNewPet) {
		return ERR_GOODS_USE::ID_UNKOWND_ERR;
	}

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_USEITEM_ADDPET, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, pNewPet->GetDT_PET_DATA().wPetId, pNewPet->GetDT_PET_DATA().wPetCfgId, 0, 0, "");

	pPlayerPet->UpdatePetLstData();
	return ERR_GOODS_USE::ID_SUCCESS;
}

UINT8 CPetMng::OpenPetInformationPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PET_PANEL_REQ& rMsgReq, PKT_CLIGS_OPEN_PET_PANEL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_PET_PANEL_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_OPEN_PET_PANEL_REQ::ID_IS_UNOPEN;
	}
	
	if (!fillPlayerData(pPlayerPet, rMsgAck.stAllPetInfo)) {
		return ERR_CLIGS_OPEN_PET_PANEL_REQ::ID_UNKNOWN_ERR;
	}
	
	pPlayer->GetSyncResource(rMsgAck.stResInfo);
	return ERR_CLIGS_OPEN_PET_PANEL_REQ::ID_SUCCESS;
}

const CPet* CPetMng::CreateAndAddPet(CPlayer* pPlayer, const PetBaseCfg* pPetBaseCfg, UINT8 ucPetLv) {
	if (NULL == pPlayer || NULL == pPetBaseCfg) {
		return NULL;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return NULL;
	}

	DT_PET_DATA petData = { 0 };
	for (size_t n = 0; n < MAX_PET_WASH_PSYCHIC_CELL_CNT; ++ n) {
		petData.astWashQualityData[n].byIsCanWash = 0;
		petData.astWashQualityData[n].byCurWashQuality = 1;
		petData.astWashQualityData[n].dwPsychicVal = 0;
		petData.astWashQualityData[n].dwMaxPsychicVal = pPetBaseCfg->unPhyChicMax / MAX_PET_WASH_PSYCHIC_CELL_CNT;
	}

	petData.wPetCfgId = pPetBaseCfg->usPetCfgId;
	petData.byStarLv = 0;
	petData.wLv = ucPetLv;
	memcpy(petData.aszPetName, pPetBaseCfg->strPetName.c_str(), sizeof(petData.aszPetName));

	UINT32 unRandamVal = Random(pPetBaseCfg->ucPhyChicValPerMin, pPetBaseCfg->ucPhyChicValPerMax);
	petData.astWashQualityData[0].byIsCanWash = 1;
	petData.astWashQualityData[0].dwPsychicVal = unRandamVal;
	petData.astWashQualityData[0].byCurWashQuality = CPetPropMng::Instance()->GetWashQuality(pPetBaseCfg->ucPetType, pPetBaseCfg->ucPetQuality, unRandamVal);

	const PetLvCfg* pLvCfg = CPetPropMng::Instance()->GetPetLvCfg(ucPetLv);
	if (NULL != pLvCfg) {
		petData.dwLvExp = pLvCfg->unExp;
	}
		

	return pPlayerPet->AddPet(petData);
}

UINT8 CPetMng::OpenStarPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_STAR_PANEL_REQ& rMsgReq, PKT_CLIGS_OPEN_STAR_PANEL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_STAR_PANEL_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet
		|| NULL == pPlayer->GetBuild(EBK_PETUPSTAR))
	{
		return ERR_CLIGS_OPEN_STAR_PANEL_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.byPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_OPEN_STAR_PANEL_REQ::ID_NO_THIS_PET;
	}

	const DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(rPetData.wPetCfgId);
	if (NULL == pBaseCfg) {
		return ERR_CLIGS_OPEN_STAR_PANEL_REQ::ID_UNKNOWN_ERR;
	}

	const PetStarCfg* pStarCfg = CPetPropMng::Instance()->GetPetStarCfg(pBaseCfg->ucPetType
										, pBaseCfg->ucPetQuality
										, 1 + rPetData.byStarLv);
	if (NULL == pStarCfg) {
		rMsgAck.byIsFullStartLv = 1;
	} else {
		const PetStarCfg* pCurStarCfg = pPet->GetPetStarCfg();
		if (NULL != pCurStarCfg) {
			rMsgAck.stCurUpStarInfo.wLingliPer = pCurStarCfg->unAddLingliPer;
			rMsgAck.stCurUpStarInfo.wLvLimit = pCurStarCfg->usPetMaxLv;
			rMsgAck.stCurUpStarInfo.wStarLv = rPetData.byStarLv;
		}

		rMsgAck.stNextUpStarInfo.wStarLv = rPetData.byStarLv + 1;
		rMsgAck.stNextUpStarInfo.wLingliPer = pStarCfg->unAddLingliPer;
		rMsgAck.stNextUpStarInfo.wLvLimit = pStarCfg->usPetMaxLv;

		for (size_t n = 0; n < pStarCfg->vCostItemId.size() && n < MAX_STAR_DRAGON_ITEMTYPE_NUM; ++ n) {
			rMsgAck.astNeedDragonInfo[n].dwItemID = pStarCfg->vCostItemId[n];
			rMsgAck.astNeedDragonInfo[n].dwItemNum = pStarCfg->vCostItemNum[n];
			++ rMsgAck.byNeedDragonNum;
		}
	}

	CID2GoodsShmemMap& rGoodMap = pPlayer->GetBag().GetGoodsMap();
	for (size_t n = 0; n < rGoodMap.Size() && rMsgAck.byBagDragonItemNum < MAX_BAG_OPEN_NUM; ++n) {
		CGoods* pGood = rGoodMap.GetDataByIndex(n);
		if (NULL != pGood) {
			SGoodsProp *pGoodsProp = pGood->GetGoodsProp();
			if (NULL == pGoodsProp) {
				continue;
			}

			if (EGMK_DRAGON_BALL == pGoodsProp->byMainKindID) {
				pGood->GetDT_GOODS_DATA_CLI(rMsgAck.astBagDragonItemInfo[rMsgAck.byBagDragonItemNum]);
				++ rMsgAck.byBagDragonItemNum;
			}
		}
	}

	return ERR_CLIGS_OPEN_STAR_PANEL_REQ::ID_SUCCESS;
}

UINT8 CPetMng::UpPetStar(CPlayer* pPlayer, const PKT_CLIGS_UP_STAR_LV_REQ& rMsgReq, PKT_CLIGS_UP_STAR_LV_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_UP_STAR_LV_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet
		|| NULL == pPlayer->GetBuild(EBK_PETUPSTAR))
	{
		return ERR_CLIGS_UP_STAR_LV_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.byPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_UP_STAR_LV_REQ::ID_NO_THIS_PET;
	}

	DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(rPetData.wPetCfgId);
	if (NULL == pBaseCfg) {
		return ERR_CLIGS_UP_STAR_LV_REQ::ID_UNKNOWN_ERR;
	}

	const PetStarCfg* pStarCfg = CPetPropMng::Instance()->GetPetStarCfg(pBaseCfg->ucPetType
										, pBaseCfg->ucPetQuality
										, rPetData.byStarLv);
	if (NULL == pStarCfg) {
		return ERR_CLIGS_UP_STAR_LV_REQ::ID_PET_SATR_FULL;
	}

	// 宠物等级校验
	if (rPetData.wLv != pStarCfg->usPetMaxLv) {
		return ERR_CLIGS_UP_STAR_LV_REQ::ID_PET_LEVEL_LOW;
	}
	// 玩家等级校验
	if (pPlayer->GetLevel() < pStarCfg->ucPlayerLvLimit) {
		return ERR_CLIGS_UP_STAR_LV_REQ::ID_PLAYER_LEVEL_LOW;
	}

	// 校验是否有足够龙珠
	for (size_t n = 0; n < pStarCfg->vCostItemId.size(); ++n) {
		if (pStarCfg->vCostItemNum[n] > pPlayer->GetBag().GetGoodsPileNum(pStarCfg->vCostItemId[n])) {
			return ERR_CLIGS_UP_STAR_LV_REQ::ID_DRAGON_ITEM_NO_ENOUGHT;
		}
	}

	++ rPetData.byStarLv;
	pPlayerPet->UpdatePetLstData();
	pPet->FillDT_PET_CLI_DATA(rMsgAck.stPetData);

	// 扣掉龙珠
	for (size_t n = 0; n < pStarCfg->vCostItemId.size(); ++n) {
		if (!pPlayer->GetBag().DelGoodItem(pStarCfg->vCostItemId[n], pStarCfg->vCostItemNum[n])) {
			return ERR_CLIGS_UP_STAR_LV_REQ::ID_DRAGON_ITEM_NO_ENOUGHT;
		}
	}

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_UPSTAR, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.byPetId, rPetData.byStarLv);

	return ERR_CLIGS_UP_STAR_LV_REQ::ID_SUCCESS;
}

UINT8 CPetMng::OpenPetFeedFanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PETFEED_REQ& rMsgReq, PKT_CLIGS_OPEN_PETFEED_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_PETFEED_REQ::ID_UNKNOWN_ERR;
	}
	 
	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_OPEN_PETFEED_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.byPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_OPEN_PETFEED_REQ::ID_NO_THIS_PET;
	}

	const DT_PLAYER_PET_DATA& rPlayerPetData = pPlayerPet->GetDT_PLAYER_PET_DATA();
	const DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	rMsgAck.byPetId = rMsgReq.byPetId;
	
	UINT16 usVipFeedCnt =CVipPropMgr::Instance()->GetIncNum(EVINF_PET_FEED, pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);
	if (!IsToday(rPlayerPetData.qwLastFeedTime)) {
		rMsgAck.dwLeftFreeFeedCnt = CPetPropMng::Instance()->GetPetSysCfg().ucFreeFeedCnt;
		rMsgAck.dwTotalFeedCnt = rMsgAck.dwLeftFreeFeedCnt + usVipFeedCnt;
	} else {
		rMsgAck.dwLeftFreeFeedCnt = rPlayerPetData.dwLeftFreeFeedCnt;
		rMsgAck.dwTotalFeedCnt = rMsgAck.dwLeftFreeFeedCnt + usVipFeedCnt - rPlayerPetData.dwVIPFeedCnt;
	}

	if (0 == rMsgAck.dwLeftFreeFeedCnt) {
		if (rPlayerPetData.dwVIPFeedCnt < usVipFeedCnt) {
			rMsgAck.dwVipFeedCostGold = CLuamgr::Instance()->GetPetFeedVIPCostGold(
				pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, rPlayerPetData.dwVIPFeedCnt);
		}
	}

	if (0 != rMsgAck.dwLeftFreeFeedCnt || 0 != rMsgAck.dwVipFeedCostGold) {
		rMsgAck.dwIsCanFeed = TRUE;
	} else {
		rMsgAck.dwIsCanFeed = FALSE;
	}

	memcpy(rMsgAck.abyCurFruitType, rPetData.abyFeedFruit, sizeof(rPetData.abyFeedFruit));
	rMsgAck.dwFruitScore = calcFruitSocre(rPetData.abyFeedFruit);

	rMsgAck.dwReStartCost = CPetPropMng::Instance()->GetPetSysCfg().unResetFruitCost;
	rMsgAck.dwAMaxCost = CPetPropMng::Instance()->GetPetSysCfg().unMaxFruitCost;

	pPlayer->GetSyncResource(rMsgAck.stResInfo);
	return ERR_CLIGS_OPEN_PETFEED_REQ::ID_SUCCESS;
}

UINT8 CPetMng::FirstDraw(CPlayer* pPlayer, const PKT_CLIGS_FIRST_DRAW_REQ& rMsgReq, PKT_CLIGS_FIRST_DRAW_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_FIRST_DRAW_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet
		|| NULL == pPlayer->GetBuild(EBK_PETFEED))
	{
		return ERR_CLIGS_FIRST_DRAW_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.byPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_FIRST_DRAW_REQ::ID_NO_THIS_PET;
	}

	DT_PLAYER_PET_DATA& rPlayerPetData = pPlayerPet->GetDT_PLAYER_PET_DATA();
	DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	for (size_t n = 0; n < MAX_PET_FEED_FRUIT_CNT; ++ n) {
		if (0 < rPetData.abyFeedFruit[n] && rPetData.abyFeedFruit[n] <= ePetFruitCnt) {
			return ERR_CLIGS_FIRST_DRAW_REQ::ID_EAT_CUR_FRUIT;
		}
	}

	if (!IsToday(rPlayerPetData.qwLastFeedTime)) {
		rPlayerPetData.dwLeftFreeFeedCnt = CPetPropMng::Instance()->GetPetSysCfg().ucFreeFeedCnt;
		rPlayerPetData.dwVIPFeedCnt = 0;
	}

	UINT16 usVipFeedCnt =CVipPropMgr::Instance()->GetIncNum(EVINF_PET_FEED, pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);
	if (rPlayerPetData.dwLeftFreeFeedCnt <= 0
		&& rPlayerPetData.dwVIPFeedCnt > usVipFeedCnt) {
		return ERR_CLIGS_FIRST_DRAW_REQ::ID_NO_TODAY_FIRST_DRAW;
	}

	UINT32 unCostGold = CLuamgr::Instance()->GetPetFeedVIPCostGold(pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, rPlayerPetData.dwVIPFeedCnt);
	if (rPlayerPetData.dwLeftFreeFeedCnt <= 0
		&& (rPlayerPetData.dwVIPFeedCnt <= usVipFeedCnt && pPlayer->GetGold() < unCostGold))
	{
		return ERR_CLIGS_FIRST_DRAW_REQ::ID_VIPFEED_NO_ENOUGHT_GOLD;
	}

	for (size_t n = 0; n < MAX_PET_FEED_FRUIT_CNT; ++ n) {
		rPetData.abyFeedFruit[n] = getRandomFruit();
	}

	rMsgAck.dwFruitScore = calcFruitSocre(rPetData.abyFeedFruit);
	memcpy(rMsgAck.abyCurFruitType, rPetData.abyFeedFruit, sizeof(rPetData.abyFeedFruit));
	rPlayerPetData.qwLastFeedTime = SDNow().GetTimeValue();

	if (rPlayerPetData.dwLeftFreeFeedCnt > 0) {
		-- rPlayerPetData.dwLeftFreeFeedCnt;
	} else {
		++ rPlayerPetData.dwVIPFeedCnt;
		pPlayer->DecGold(unCostGold, CRecordMgr::EDGT_VIP_PET_FEED);
	}

	rMsgAck.dwLeftFreeFeedCnt = rPlayerPetData.dwLeftFreeFeedCnt;
	rMsgAck.dwTotalFeedCnt = rPlayerPetData.dwLeftFreeFeedCnt + usVipFeedCnt - rPlayerPetData.dwVIPFeedCnt;
	if (0 == rMsgAck.dwLeftFreeFeedCnt) {
		if (rPlayerPetData.dwVIPFeedCnt < usVipFeedCnt) {
			rMsgAck.dwVipFeedCostGold = CLuamgr::Instance()->GetPetFeedVIPCostGold(
				pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, rPlayerPetData.dwVIPFeedCnt);
		}
	}

	if (0 != rMsgAck.dwLeftFreeFeedCnt || 0 != rMsgAck.dwVipFeedCostGold) {
		rMsgAck.dwIsCanFeed = TRUE;
	} else {
		rMsgAck.dwIsCanFeed = FALSE;
	}

	pPlayerPet->UpdatePetLstData();

	ostringstream oss;
	for (size_t n = 0; n < MAX_PET_FEED_FRUIT_CNT; ++ n) {
		oss << rPetData.abyFeedFruit[n] << ",";
	}
	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_FIRSTDRAW, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.byPetId, 0, 0, 0, oss.str());

	pPlayer->GetSyncResource(rMsgAck.stResInfo);

	return ERR_CLIGS_FIRST_DRAW_REQ::ID_SUCCESS;
}

UINT8 CPetMng::ResetDraw(CPlayer* pPlayer, const PKT_CLIGS_RESET_DRAW_REQ& rMsgReq, PKT_CLIGS_RESET_DRAW_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_RESET_DRAW_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet
		|| NULL == pPlayer->GetBuild(EBK_PETFEED))
	{
		return ERR_CLIGS_RESET_DRAW_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.byPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_RESET_DRAW_REQ::ID_NO_THIS_PET;
	}

	DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();

	BOOL bBestFruitCombination = TRUE;
	BOOL bHaveFirstDrawed = FALSE;
	for (size_t n = 0; n < MAX_PET_FEED_FRUIT_CNT; ++ n) {
		if (0 != rPetData.abyFeedFruit[n]) {
			bHaveFirstDrawed = TRUE;
		}
		if (ePFT_Sapodilla != rPetData.abyFeedFruit[n]) {
			bBestFruitCombination = FALSE;
		}
	}

	if (!bHaveFirstDrawed) {
		return ERR_CLIGS_RESET_DRAW_REQ::ID_NO_FIRST_DRAW;
	}

	if (bBestFruitCombination) {
		return ERR_CLIGS_RESET_DRAW_REQ::ID_IS_BEST_RESULT;
	}

	BOOL bMaxDraw = 1 == rMsgReq.byResetType;
	const PetSysCfg& rPetSysCfg = CPetPropMng::Instance()->GetPetSysCfg();
	// 元宝验证
	if (bMaxDraw) {
		if (pPlayer->GetDT_PLAYER_BASE_DATA().qwGold < rPetSysCfg.unMaxFruitCost) {
			return ERR_CLIGS_RESET_DRAW_REQ::ID_NOT_ENOUGHT_GOLD;
		}
	} else {
		if (pPlayer->GetDT_PLAYER_BASE_DATA().qwGold < rPetSysCfg.unResetFruitCost) {
			return ERR_CLIGS_RESET_DRAW_REQ::ID_NOT_ENOUGHT_GOLD;
		}
	}

	// 抽取水果
	for (size_t n = 0; n < MAX_PET_FEED_FRUIT_CNT; ++ n) {
		if (ePFT_Sapodilla == rPetData.abyFeedFruit[n]) {
			continue;
		}
		if (!bMaxDraw) {
			rPetData.abyFeedFruit[n] = getRandomFruit();
		} else {
			rPetData.abyFeedFruit[n] = ePFT_Sapodilla;
		}
	}

	// 扣元宝
	if (bMaxDraw) {
		pPlayer->DecGold(rPetSysCfg.unMaxFruitCost, CRecordMgr::EDGT_FRUIT_MAX_DRAW);
	} else {
		pPlayer->DecGold(rPetSysCfg.unResetFruitCost, CRecordMgr::EDGT_FRUIT_RESET_DRAW);
	}

	rMsgAck.dwFruitScore = calcFruitSocre(rPetData.abyFeedFruit);
	pPlayer->GetSyncResource(rMsgAck.stResInfo);

	memcpy(rMsgAck.abyCurFruitType, rPetData.abyFeedFruit, sizeof(rPetData.abyFeedFruit));
	pPlayerPet->UpdatePetLstData();

	ostringstream oss;
	for (size_t n = 0; n < MAX_PET_FEED_FRUIT_CNT; ++ n) {
		oss << rPetData.abyFeedFruit[n] << ",";
	}
	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_RESETDRAW, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.byPetId, rMsgReq.byResetType, 0, 0, oss.str());

	return ERR_CLIGS_RESET_DRAW_REQ::ID_SUCCESS;
}

UINT8 CPetMng::ConfirmFeed(CPlayer* pPlayer, const PKT_CLIGS_CONFIRM_FEED_REQ& rMsgReq, PKT_CLIGS_CONFIRM_FEED_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_CONFIRM_FEED_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet
		|| NULL == pPlayer->GetBuild(EBK_PETFEED))
	{
		return ERR_CLIGS_CONFIRM_FEED_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.byPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_CONFIRM_FEED_REQ::ID_NO_THIS_PET;
	}
	DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();

	UINT32 unAddExp = calcFruitSocre(rPetData.abyFeedFruit);
	pPet->AddPetExp(unAddExp);
	pPet->FillDT_PET_CLI_DATA(rMsgAck.stPetInfo);

	memset(rPetData.abyFeedFruit, 0, sizeof(rPetData.abyFeedFruit));
	pPlayerPet->UpdatePetLstData();

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_CONFIRMFEED, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.byPetId);

	return ERR_CLIGS_CONFIRM_FEED_REQ::ID_SUCCESS;
}

UINT8 CPetMng::PetMerge(CPlayer* pPlayer, const PKT_CLIGS_MERGE_PET_REQ& rMsgReq, PKT_CLIGS_MERGE_PET_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_MERGE_PET_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet
		|| NULL == pPlayer->GetBuild(EBK_PETMERGE))
	{
		return ERR_CLIGS_MERGE_PET_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.byEatPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_MERGE_PET_REQ::ID_NO_THIS_PET;
	}

	std::vector<CPet*> vAtePet;
	for (size_t n = 0; n < rMsgReq.byBeAtePetNum; ++n) {
		CPet* pAddedPet = pPlayerPet->GetPet(rMsgReq.abyBeAtePetId[n]);
		if (NULL == pAddedPet) {
			return ERR_CLIGS_MERGE_PET_REQ::ID_NO_THIS_PET;
		}
		vAtePet.push_back(pAddedPet);
	}

	for (size_t n = 0; n < vAtePet.size(); ++ n) {
		pPet->AddPetExp(vAtePet[n]->GetBeAteExp());

		CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PETMERGE_REMOVEPET, 1
			, 1, 1
			, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
			, vAtePet[n]->GetDT_PET_DATA().wPetId, vAtePet[n]->GetDT_PET_DATA().wPetCfgId, 0, 0, "");

		pPlayerPet->RemovePet(vAtePet[n]->GetDT_PET_DATA().wPetId);
	}

	pPlayerPet->UpdatePetLstData();

	const MapPet& mpPet = pPlayerPet->GetAllPet();
	for (MapPet::const_iterator itor = mpPet.begin(), end = mpPet.end()
		; end != itor && rMsgAck.stAllPetInfo.wPetNum <= MAX_PET_NUM
		; ++ itor)
	{
		if (NULL == itor->second) {
			return ERR_CLIGS_OPEN_REFINEPET_REQ::ID_UNKNOWN_ERR;
		}

		itor->second->FillDT_PET_CLI_DATA(rMsgAck.stAllPetInfo.astPetData[rMsgAck.stAllPetInfo.wPetNum]);
		++ rMsgAck.stAllPetInfo.wPetNum;
	}

	ostringstream oss;
	for (size_t n = 0; n < rMsgReq.byBeAtePetNum; ++ n) {
		oss << rMsgReq.abyBeAtePetId[n] << ",";
	}
	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_MERGE, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.byEatPetId, 0, 0, 0, oss.str());

	return ERR_CLIGS_MERGE_PET_REQ::ID_SUCCESS;
}

UINT8 CPetMng::OnOpenPetStore(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PET_STORE_REQ& rMsgReq, PKT_CLIGS_OPEN_PET_STORE_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_PET_STORE_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_OPEN_PET_STORE_REQ::ID_IS_UNOPEN;
	}

	const MapPetStoreCfg& rPetAllStoreCfg = CPetPropMng::Instance()->GetAllPetStoreCfg();
	rMsgAck.wStoreItemNum = rPetAllStoreCfg.size();
	size_t n = 0;
	for (MapPetStoreCfg::const_iterator itor = rPetAllStoreCfg.begin(), end = rPetAllStoreCfg.end()
		; end != itor && n < MAX_PET_STORE_ITEM_NUM
		; ++ itor)
	{
		const PetStoreCfg* pStoreCfg = itor->second;
		if (pPlayer->GetLevel() < pStoreCfg->usShowLv) {
			continue;
		}

		DT_PET_STORE_INFO& rDtStoreInfo = rMsgAck.astStoreItems[n];
		rDtStoreInfo.wStoreId = itor->first;
		rDtStoreInfo.byItemType = pStoreCfg->ucItemType;
		if (pPlayer->GetLevel() >= (UINT8)pStoreCfg->usPlayerLvLimit) {
			rDtStoreInfo.byLvLimit = 0;
		} else {
			rDtStoreInfo.byLvLimit = (UINT8)pStoreCfg->usPlayerLvLimit;
		}

		if (pStoreCfg->unCostPetScore != 0) {
			rDtStoreInfo.byCostType = ACTE_PETSCORE;
			rDtStoreInfo.dwCostNum = pStoreCfg->unCostPetScore;
		} else {
			rDtStoreInfo.byCostType = ACTE_GOLD;
			rDtStoreInfo.dwCostNum = pStoreCfg->unCostGold;
		}
		
		CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(pStoreCfg->unItemId, 1, rDtStoreInfo.stStoreGood);
		++ n;
	}

	pPlayer->GetSyncResource(rMsgAck.stResInfo);
	return ERR_CLIGS_OPEN_PET_STORE_REQ::ID_SUCCESS;
}

UINT8 CPetMng::BuyStoreItem(CPlayer* pPlayer, const PKT_CLIGS_BUY_PET_STORE_ITEM_REQ& rMsgReq, PKT_CLIGS_BUY_PET_STORE_ITEM_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_IS_UNOPEN;
	}

	const PetStoreCfg* pStoreCfg = CPetPropMng::Instance()->GetPetStoreCfg(rMsgReq.wStoreItemId);
	if (NULL == pStoreCfg) {
		return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_UNKNOWN_ERR;
	}

	if (pStoreCfg->ucItemType != ePSIT_CanBuy) {
		return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_Cant_BUY;
	}

	DT_PLAYER_PET_DATA& rPlayerPetData = pPlayerPet->GetDT_PLAYER_PET_DATA();
	// 消耗品验证
	if (0 != pStoreCfg->unCostPetScore) {
		if (pStoreCfg->unCostPetScore > rPlayerPetData.dwPetScore) {
			return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_NO_ENOUGHT_PETSOTRE;
		}
	} else {
		if (pStoreCfg->unCostGold > pPlayer->GetDT_PLAYER_BASE_DATA().qwGold) {
			return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_NO_ENOUGHT_GOLD;
		}
	}

	// 背包空间验证
	if (pPlayer->GetBag().GetIdleSize() < 1) {
		return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_ON_ENOUGHT_BAG_SIZE;
	}

	// 塞入购买的物品
	CGoods* pGoods = pPlayer->GetBag().GetGoodsItem(pStoreCfg->unItemId);
	if (NULL == pGoods) {
		ECreateItemRet eItemRet = ECIR_SUCCESS;
		pGoods = (CGoods*)CItemMgr::Instance()->CreateItemIntoBag(pPlayer, pStoreCfg->unItemId, 1
			, eItemRet, 0, CRecordMgr::EAIT_BUY_PET_STORE);

		if (ECIR_SUCCESS != eItemRet) {
			PRINT_ERR_INFO;
			return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_UNKNOWN_ERR;
		}
	} else { // 堆叠
		pGoods->AddPileNum(1, CRecordMgr::EAIT_BUY_PET_STORE);
// 		if (!pPlayer->GetBag().AddItem(pGoods)) {
// 			PRINT_ERR_INFO;
// 			return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_UNKNOWN_ERR;
// 		}
	}

	// 扣除消耗品
	if (0 != pStoreCfg->unCostPetScore) {
		rPlayerPetData.dwPetScore -= pStoreCfg->unCostPetScore;
	} else {
		pPlayer->DecGold(pStoreCfg->unCostGold, CRecordMgr::EDGT_BUY_PET_STORE_ITEM);
	}

	pPlayer->GetSyncResource(rMsgAck.stResInfo);

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_BUYSTOREITEM, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.wStoreItemId);

	return ERR_CLIGS_BUY_PET_STORE_ITEM_REQ::ID_SUCCESS;
}

UINT8 CPetMng::OpenWashQuality(CPlayer* pPlayer, const PKT_CLIGS_OPEN_WASH_QUALITY_REQ& rMsgReq, PKT_CLIGS_OPEN_WASH_QUALITY_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_OPEN_WASH_QUALITY_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.wPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_OPEN_WASH_QUALITY_REQ::ID_NO_THIS_PET;
	}

	const PetBaseCfg* pPetBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(pPet->GetDT_PET_DATA().wPetCfgId);
	if (NULL == pPetBaseCfg) {
		return ERR_CLIGS_OPEN_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}

	const PetSysCfg& rPetSysCfg = CPetPropMng::Instance()->GetPetSysCfg();
	rMsgAck.dwSlotVal = pPetBaseCfg->unPhyChicMax / MAX_PET_WASH_PSYCHIC_CELL_CNT;
	memcpy(rMsgAck.aszWashDesc, rPetSysCfg.strWashDesc.c_str(), sizeof(rMsgAck.aszWashDesc));

	for (size_t n = 0; n < MAX_PET_WASH_PSYCHIC_CELL_CNT; ++ n) {
		DT_PET_WASHQUALITY_DATA& rPetWashData = pPet->GetDT_PET_DATA().astWashQualityData[n];
		
		const PetWashCfg* pWashCfg = CPetPropMng::Instance()->GetPetWashCfg(pPetBaseCfg->ucPetType
			, pPetBaseCfg->ucPetQuality
			, rPetWashData.byCurWashQuality);

		if (NULL == pWashCfg) {
			return ERR_CLIGS_OPEN_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
		}
		for (size_t m = 0
			; m < pWashCfg->vUseItemid.size() && m < MAX_PET_WASH_PSYCHIC_ITEMTYPE_CNT
			; ++m)
		{
			rMsgAck.astCostItemInfo[n].astItemInfo[m].dwItemNum = pWashCfg->vUseItemNum[m];
			rMsgAck.astCostItemInfo[n].astItemInfo[m].dwItemId = pWashCfg->vUseItemid[m];
			++ rMsgAck.astCostItemInfo[n].byNum;
		}
		rMsgAck.astCostItemInfo[n].dwLockCostGold = pWashCfg->unLockCostGold;
	}
	
	CBag& rBag = pPlayer->GetBag();
	rMsgAck.dwBagItemNum = 0;
	CID2GoodsShmemMap& rGoodMap = rBag.GetGoodsMap();
	for (size_t n = 0; n < rGoodMap.Size() && rMsgAck.dwBagItemNum < MAX_BAG_OPEN_NUM; ++n) {
		CGoods* pGood = rGoodMap.GetDataByIndex(n);
		if (NULL != pGood) {
			SGoodsProp *pGoodsProp = pGood->GetGoodsProp();
			if (NULL == pGoodsProp) {
				continue;
			}

			if (EGMK_JINLIULU == pGoodsProp->byMainKindID) {
				pGood->GetDT_GOODS_DATA_CLI(rMsgAck.astBagItemInfo[rMsgAck.dwBagItemNum]);
				++ rMsgAck.dwBagItemNum;
			}
		}
	}

	pPlayer->GetSyncResource(rMsgAck.stResInfo);
	return ERR_CLIGS_OPEN_WASH_QUALITY_REQ::ID_SUCCESS;
}

UINT8 CPetMng::WashQuality(CPlayer* pPlayer, const PKT_CLIGS_WASH_QUALITY_REQ& rMsgReq, PKT_CLIGS_WASH_QUALITY_ACK& rMsgAck) {
	if (NULL == pPlayer
		|| rMsgReq.byWashCell < 1 || rMsgReq.byWashCell > MAX_PET_WASH_PSYCHIC_CELL_CNT)
	{
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet
		|| NULL == pPlayer->GetBuild(EBK_PETWASH))
	{
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.wPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_NO_THIS_PET;
	}

	DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	const PetBaseCfg* pPetBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(rPetData.wPetCfgId);
	if (NULL == pPetBaseCfg) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}
	// 一格的最大值
	UINT32 unOneCellVal = pPetBaseCfg->unPhyChicMax / MAX_PET_WASH_PSYCHIC_CELL_CNT;

	DT_PET_WASHQUALITY_DATA& rPetWashData = rPetData.astWashQualityData[rMsgReq.byWashCell - 1];
	if (0 == rPetWashData.byIsCanWash) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_CANT_WASH;
	}

	const PetWashCfg* pWashCfg = CPetPropMng::Instance()->GetPetWashCfg(pPetBaseCfg->ucPetType, pPetBaseCfg->ucPetQuality, rPetWashData.byCurWashQuality);
	if (NULL == pWashCfg) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}

	BOOL bUseGold = 0 != rMsgReq.byIsLock;
	// 验证元宝
	if (bUseGold && pPlayer->GetDT_PLAYER_BASE_DATA().qwGold < pWashCfg->unLockCostGold) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_NO_ENOUGHT_GOLD;
	}
	// todo
	// 验证洗资质物品
	for (size_t n = 0; n < pWashCfg->vUseItemid.size(); ++ n) {
		if (pPlayer->GetBag().GetGoodsPileNum(pWashCfg->vUseItemid[n]) < pWashCfg->vUseItemNum[n]) {
			return ERR_CLIGS_WASH_QUALITY_REQ::ID_NO_ENOUGHT_WASHITEM;
		}
	}

	RandomOdds<UINT8> washQualityOdds;
	for (size_t n = 0; n < pWashCfg->vPossibleWashQuality.size(); ++ n) {
		washQualityOdds.PushOdds(pWashCfg->vPossibleWashQuality[n], pWashCfg->vWashQualityWeight[n]);
	}
	UINT8* pWashQuality = washQualityOdds.GetHitOdds();
	if (NULL == pWashCfg) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}
	UINT8 ucRandQuality = *pWashQuality;
	if (bUseGold && rPetWashData.byCurWashQuality > ucRandQuality) {
		ucRandQuality = rPetWashData.byCurWashQuality;
	}

	const PetWashCfg* pNextWashCfg = CPetPropMng::Instance()->GetPetWashCfg(pPetBaseCfg->ucPetType, pPetBaseCfg->ucPetQuality, ucRandQuality);
	if (NULL == pWashCfg) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}

	RandomOdds<PetPsychicRange> washValOdds;
	for (size_t n = 0; n < pNextWashCfg->vPsychicRange.size(); ++ n) {
		washValOdds.PushOdds(pNextWashCfg->vPsychicRange[n], pNextWashCfg->vPsychicWeight[n]);
	}
	PetPsychicRange* pRandomRange = washValOdds.GetHitOdds();
	if (NULL == pRandomRange) {
		return ERR_CLIGS_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
	}

	rPetWashData.dwPsychicVal = Random(pRandomRange->unMinVal, pRandomRange->unMaxVal);
	rPetWashData.byCurWashQuality = ucRandQuality;

 	const PetSysCfg& rPetSysCfg = CPetPropMng::Instance()->GetPetSysCfg();
	if (rMsgReq.byWashCell != MAX_PET_WASH_PSYCHIC_CELL_CNT
		&& rPetSysCfg.ucTriggerWashQuality <= ucRandQuality)
	{
		rPetData.astWashQualityData[rMsgReq.byWashCell].byIsCanWash = 1;
	}

	// 扣元宝
	if (bUseGold) {
		pPlayer->DecGold(pWashCfg->unLockCostGold, CRecordMgr::EDGT_WASH_PSYCHIC);
	}

	// todo
	// 扣物品
	for (size_t n = 0; n < pWashCfg->vUseItemid.size(); ++ n) {
		pPlayer->GetBag().DelGoodItem(pWashCfg->vUseItemid[n], pWashCfg->vUseItemNum[n]);
	}
	pPlayerPet->UpdatePetLstData();
	pPet->FillDT_PET_CLI_DATA(rMsgAck.stPetData);

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_WASHQUALITY, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.wPetId, rMsgReq.byIsLock, rMsgReq.byWashCell);


	for (size_t n = 0; n < MAX_PET_WASH_PSYCHIC_CELL_CNT; ++ n) {
		DT_PET_WASHQUALITY_DATA& rPetWashData = pPet->GetDT_PET_DATA().astWashQualityData[n];

		const PetWashCfg* pWashCfg = CPetPropMng::Instance()->GetPetWashCfg(pPetBaseCfg->ucPetType
			, pPetBaseCfg->ucPetQuality
			, rPetWashData.byCurWashQuality);

		if (NULL == pWashCfg) {
			return ERR_CLIGS_WASH_QUALITY_REQ::ID_UNKNOWN_ERR;
		}
		for (size_t m = 0
			; m < pWashCfg->vUseItemid.size() && m < MAX_PET_WASH_PSYCHIC_ITEMTYPE_CNT
			; ++m)
		{
			rMsgAck.astCostItemInfo[n].astItemInfo[m].dwItemNum = pWashCfg->vUseItemNum[m];
			rMsgAck.astCostItemInfo[n].astItemInfo[m].dwItemId = pWashCfg->vUseItemid[m];
			++ rMsgAck.astCostItemInfo[n].byNum;
		}
		rMsgAck.astCostItemInfo[n].dwLockCostGold = pWashCfg->unLockCostGold;
	}

	CBag& rBag = pPlayer->GetBag();
	rMsgAck.dwBagItemNum = 0;
	CID2GoodsShmemMap& rGoodMap = rBag.GetGoodsMap();
	for (size_t n = 0; n < rGoodMap.Size() && rMsgAck.dwBagItemNum < MAX_BAG_OPEN_NUM; ++n) {
		CGoods* pGood = rGoodMap.GetDataByIndex(n);
		if (NULL != pGood) {
			SGoodsProp *pGoodsProp = pGood->GetGoodsProp();
			if (NULL == pGoodsProp) {
				continue;
			}

			if (EGMK_JINLIULU == pGoodsProp->byMainKindID) {
				pGood->GetDT_GOODS_DATA_CLI(rMsgAck.astBagItemInfo[rMsgAck.dwBagItemNum]);
				++ rMsgAck.dwBagItemNum;
			}
		}
	}

	pPlayer->GetSyncResource(rMsgAck.stResInfo);
	return ERR_CLIGS_WASH_QUALITY_REQ::ID_SUCCESS;
}

UINT8 CPetMng::ChangePetState(CPlayer* pPlayer, const PKT_CLIGS_CHANGE_PET_STATE_REQ& rMsgReq, PKT_CLIGS_CHANGE_PET_STATE_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.wPetId);
	if (NULL == pPet) {
		return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_NO_THIS_PET;
	}

	if (rMsgReq.byPetState < ePS_Normal || rMsgReq.byPetState > ePS_FIGHT) {
		return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_UNKNOWN_ERR;
	}

	pPlayer->AddGuideRecord(EGF_PET_FIGHT);

	DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	if (rMsgReq.byPetState == rPetData.byPetState
		|| rMsgReq.byPetState - rPetData.byPetState >= 2
		|| rMsgReq.byPetState - rPetData.byPetState < -2)
	{
		return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_UNKNOWN_ERR;
	}

	if (ePS_FOLLOW == rMsgReq.byPetState) {
		vector<const CPet*> vPet = pPlayerPet->GetFollowPet();
		if (vPet.size() >= CPetPropMng::Instance()->GetPetSysCfg().usMaxCarryPet) {
			return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_OVER_MAX_FOLLOW;
		}
	} else if (ePS_FIGHT == rMsgReq.byPetState) {
		if (NULL != pPlayerPet->GetFightPet()) {
			CPet* pFightPet = pPlayerPet->GetPet(pPlayerPet->GetFightPet()->GetDT_PET_DATA().wPetId);
			if (NULL == pFightPet) {
				return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_OVER_MAX_FIGHT;
			}
			pFightPet->GetDT_PET_DATA().byPetState = ePS_FOLLOW;
		}
	}

	rPetData.byPetState = rMsgReq.byPetState;
	pPet->FillDT_PET_CLI_DATA(rMsgAck.stPetData);
	pPlayerPet->UpdatePetLstData();
	return ERR_CLIGS_CHANGE_PET_STATE_REQ::ID_SUCCESS;
}

UINT8 CPetMng::OpenPetInherit(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PET_INHERIT_REQ& rMsgReq, PKT_CLIGS_OPEN_PET_INHERIT_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_OPEN_PET_INHERIT_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_OPEN_PET_INHERIT_REQ::ID_IS_UNOPEN;
	}

	UINT16 wRet = getInheritPetInfo(pPlayerPet, rMsgReq.wPetId, rMsgAck.wPetNum, rMsgAck.astNormalAfterInheritData, rMsgAck.astPerfectAfterInheritData);
	
	if ( P_OTHER_ERR == wRet || P_NOT_PET_CFG == wRet)
	{
		return ERR_CLIGS_OPEN_PET_INHERIT_REQ::ID_UNKNOWN_ERR;
	}
	else if ( P_NOT_HAVE_PET == wRet)
	{
		return ERR_CLIGS_OPEN_PET_INHERIT_REQ::ID_NO_THIS_PET;
	}

	pPlayer->GetSyncResource(rMsgAck.stResInfo);
	rMsgAck.dwCostGold = CPetPropMng::Instance()->GetPetSysCfg().unPerfectInheritCostGold;
	rMsgAck.dwCostCoin = CPetPropMng::Instance()->GetPetSysCfg().unNormalInheritCostCoin;
	return ERR_CLIGS_OPEN_PET_INHERIT_REQ::ID_SUCCESS;
}

UINT16 CPetMng::getInheritPetInfo(CPlayerPet* pPlayerPet, UINT16 wPetID, UINT16& wPetNum
	, DT_PET_AFTER_INHERIT_DATA* astPetCoinAfterInheritData, DT_PET_AFTER_INHERIT_DATA* astPetGoldAfterInheritData)
{
	if ( NULL == pPlayerPet
		|| NULL == astPetCoinAfterInheritData 
		|| NULL == astPetGoldAfterInheritData) 
	{
		return P_OTHER_ERR;
	}

	CPet* pPet = pPlayerPet->GetPet(wPetID);
	if (NULL == pPet) {
		return P_NOT_HAVE_PET;
	}

	const PetBaseCfg* pPetBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(pPet->GetDT_PET_DATA().wPetCfgId);
	if (NULL == pPetBaseCfg) {
		return P_NOT_PET_CFG;
	}

	const DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	const PetSysCfg& rPetSysCfg = CPetPropMng::Instance()->GetPetSysCfg();
	UINT32 dwCurPsychic = pPet->GetCurPsychic();
	const MapPet& mpPet = pPlayerPet->GetAllPet();
	for (MapPet::const_iterator itor = mpPet.begin(), end = mpPet.end()
		; end != itor && wPetNum < MAX_PET_NUM
		; ++ itor)
	{
		const CPet* pInheritedPet = itor->second;
		if (NULL == pInheritedPet || wPetID == pInheritedPet->GetPetId()) {
			continue;
		}

		DT_PET_AFTER_INHERIT_DATA& rCoinAfterInheritData = astPetCoinAfterInheritData[wPetNum];
		DT_PET_AFTER_INHERIT_DATA& stGoldAfterInheritData = astPetGoldAfterInheritData[wPetNum];
		
		for (size_t m = 0; m < MAX_PET_WASH_PSYCHIC_CELL_CNT; ++ m) {
			rCoinAfterInheritData.astWashQualityData[m].dwMaxPsychicVal = pPetBaseCfg->unPhyChicMax / MAX_PET_WASH_PSYCHIC_CELL_CNT;
		}

		const DT_PET_DATA& rInheritPetData = pInheritedPet->GetDT_PET_DATA();
		BOOL bCanInherted = FALSE;
		//有一个坑比原来高
		for (size_t m = 0; m < MAX_PET_WASH_PSYCHIC_CELL_CNT; ++ m) {
			if (rPetData.astWashQualityData[m].dwPsychicVal 
				< rInheritPetData.astWashQualityData[m].dwPsychicVal) 
			{
				bCanInherted = TRUE;
				UINT32 unPsychicVal = rInheritPetData.astWashQualityData[m].dwPsychicVal;
				if (rInheritPetData.astWashQualityData[m].dwPsychicVal > 
					rCoinAfterInheritData.astWashQualityData[m].dwMaxPsychicVal) 
				{
					unPsychicVal = rCoinAfterInheritData.astWashQualityData[m].dwMaxPsychicVal;
				}

				rCoinAfterInheritData.astWashQualityData[m].dwPsychicVal = unPsychicVal;

				UINT8 ucWashQuality = CPetPropMng::Instance()->GetWashQuality(pPetBaseCfg->ucPetType
																, pPetBaseCfg->ucPetQuality
																, unPsychicVal);
				rCoinAfterInheritData.astWashQualityData[m].byCurWashQuality = ucWashQuality;
				rCoinAfterInheritData.astWashQualityData[m].byIsCanWash = rInheritPetData.astWashQualityData[m].byIsCanWash;

				if (m < (MAX_PET_WASH_PSYCHIC_CELL_CNT - 1) 
					&& 0 == rPetData.astWashQualityData[m + 1].byIsCanWash) 
				{
						
					if (rPetSysCfg.ucTriggerWashQuality <= ucWashQuality) {
						rCoinAfterInheritData.astWashQualityData[m + 1].byIsCanWash = 1;
					}
				}
			} else {
				rCoinAfterInheritData.astWashQualityData[m].byCurWashQuality = rPetData.astWashQualityData[m].byCurWashQuality;
				rCoinAfterInheritData.astWashQualityData[m].byIsCanWash = rPetData.astWashQualityData[m].byIsCanWash;
				rCoinAfterInheritData.astWashQualityData[m].dwPsychicVal = rPetData.astWashQualityData[m].dwPsychicVal;
			}
		}

		if (!bCanInherted) {
			continue;
		}
		rCoinAfterInheritData.wPetId = pInheritedPet->GetPetId();
		rCoinAfterInheritData.byStarLv = rPetData.byStarLv < rInheritPetData.byStarLv ? rInheritPetData.byStarLv : rPetData.byStarLv;
		stGoldAfterInheritData = rCoinAfterInheritData;
	
		//可添加经验总值
		UINT32 dwAddExp = pInheritedPet->GetBeAteExp();
		UINT32 dwCoinAddExp = rPetData.dwLvExp + (dwAddExp * rPetSysCfg.ucNormalInheritExp / 100);
		UINT32 dwGoldAddExp = rPetData.dwLvExp + (dwAddExp * rPetSysCfg.ucGoodInheritExp / 100);
		//普通继承
		CPetPropMng::Instance()->GetLevel(dwCoinAddExp, rCoinAfterInheritData.byStarLv, &rCoinAfterInheritData.wLv);
		//无宝继承
		CPetPropMng::Instance()->GetLevel(dwGoldAddExp, rCoinAfterInheritData.byStarLv, &stGoldAfterInheritData.wLv);
		stGoldAfterInheritData.dwCostGold = rPetSysCfg.unPerfectInheritCostGold;
		++ wPetNum;
	}
	return P_SUCCESS;
}

UINT8 CPetMng::InheritPet(CPlayer* pPlayer, const PKT_CLIGS_PET_INHERIT_REQ& rMsgReq, PKT_CLIGS_PET_INHERIT_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_CLIGS_PET_INHERIT_REQ::ID_UNKNOWN_ERR;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return ERR_CLIGS_PET_INHERIT_REQ::ID_IS_UNOPEN;
	}

	CPet* pPet = pPlayerPet->GetPet(rMsgReq.wPetId);
	CPet* pInheriedPet = pPlayerPet->GetPet(rMsgReq.wInheritedPetId);
	
	if (NULL == pPet || NULL == pInheriedPet) {
		return ERR_CLIGS_PET_INHERIT_REQ::ID_NO_THIS_PET;
	}

	DT_PET_DATA& rPetData = pPet->GetDT_PET_DATA();
	DT_PET_DATA& rInheritPetData = pInheriedPet->GetDT_PET_DATA();

	const PetBaseCfg* pPetBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(rPetData.wPetCfgId);
	if (NULL == pPetBaseCfg) {
		return ERR_CLIGS_PET_INHERIT_REQ::ID_UNKNOWN_ERR;
	}

	BOOL bPerfectInherit = 0 != rMsgReq.byIsPerfectInherit;
	const PetSysCfg& rPetSysCfg = CPetPropMng::Instance()->GetPetSysCfg();
	// 验证元宝
	if (bPerfectInherit && pPlayer->GetDT_PLAYER_BASE_DATA().qwGold < rPetSysCfg.unPerfectInheritCostGold) {
		return ERR_CLIGS_PET_INHERIT_REQ::ID_NO_ENOUGHT_GOLD;
	}
	if (!bPerfectInherit&& pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < rPetSysCfg.unNormalInheritCostCoin) {
		return ERR_CLIGS_PET_INHERIT_REQ::ID_NO_ENOUGHT_COIN;
	}
	
	for (size_t n = 0; n < MAX_PET_WASH_PSYCHIC_CELL_CNT; ++ n) {
		if (rPetData.astWashQualityData[n].dwPsychicVal < rInheritPetData.astWashQualityData[n].dwPsychicVal) {
			rPetData.astWashQualityData[n].dwPsychicVal = rInheritPetData.astWashQualityData[n].dwPsychicVal;
			
			UINT8 ucWashQuality = CPetPropMng::Instance()->GetWashQuality(pPetBaseCfg->ucPetType
				, pPetBaseCfg->ucPetQuality
				, rPetData.astWashQualityData[n].dwPsychicVal);

			rPetData.astWashQualityData[n].byCurWashQuality = ucWashQuality;

			if (n < (MAX_PET_WASH_PSYCHIC_CELL_CNT - 1) && 0 == rPetData.astWashQualityData[n + 1].byIsCanWash) 
			{
				
				if (rPetSysCfg.ucTriggerWashQuality <= ucWashQuality) {
					rPetData.astWashQualityData[n + 1].byIsCanWash = 1;
				}
			}
		}
		rInheritPetData.astWashQualityData[n].byCurWashQuality = 1;
		rInheritPetData.astWashQualityData[n].dwPsychicVal = 0;
	}

	UINT32 unAddExp = 0;
	if (bPerfectInherit) {
		unAddExp = (UINT32)(pInheriedPet->GetBeAteExp() * double(rPetSysCfg.ucGoodInheritExp) / 100.0);
	} else {
		unAddExp = (UINT32)(pInheriedPet->GetBeAteExp() * double(rPetSysCfg.ucNormalInheritExp) / 100.0);
	}
	pPet->AddPetExp(unAddExp, TRUE);
	rInheritPetData.byStarLv = 0;
	rInheritPetData.wLv = 1;
	rInheritPetData.dwLvExp = 0;

	pPet->FillDT_PET_CLI_DATA(rMsgAck.stPetData);
	pInheriedPet->FillDT_PET_CLI_DATA(rMsgAck.stInheritedPetData);
	pPlayerPet->UpdatePetLstData();

	// 扣元宝
	if (bPerfectInherit) {
		pPlayer->DecGold(rPetSysCfg.unPerfectInheritCostGold, CRecordMgr::EDGT_PERFECT_INHERIT);
	} else {
		pPlayer->DecCoin(rPetSysCfg.unNormalInheritCostCoin, CRecordMgr::EDCT_NORMAL_INHERIT);
	}

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_PET_INHERIT, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, rMsgReq.wPetId, rMsgReq.wInheritedPetId);

	getInheritPetInfo(pPlayerPet, rMsgReq.wPetId, rMsgAck.wPetNum
		, rMsgAck.astNormalAfterInheritData, rMsgAck.astPerfectAfterInheritData);

	pPlayer->GetSyncResource(rMsgAck.stResInfo);
	return ERR_CLIGS_PET_INHERIT_REQ::ID_SUCCESS;
}

UINT32 CPetMng::calcFruitSocre(const UINT8* pFruitType) {
	if (NULL == pFruitType) {
		return 0;
	}

	std::map<UINT8, UINT32> mpFruitInfo;
	for (size_t n = 0; n < MAX_PET_FEED_FRUIT_CNT; ++ n) {
		UINT32& rNum = mpFruitInfo[pFruitType[n]];
		++ rNum;
	}
	
	UINT32 unRetVal = 0;
	for (std::map<UINT8, UINT32>::const_iterator itor = mpFruitInfo.begin(), end = mpFruitInfo.end()
		; end != itor
		; ++ itor)
	{
		const PetFeedCfg* pPetFeedCfg = CPetPropMng::Instance()->GetPetFeedCfg(itor->first);	
		if (NULL == pPetFeedCfg) {
			continue;
		}
		if (0 == itor->second || itor->second > MAX_PET_FEED_FRUIT_CNT) {
			continue;
		}
		unRetVal += (pPetFeedCfg->unBaseExp * pPetFeedCfg->vExpTimes[itor->second - 1]);
	}
	
	return unRetVal;
}

UINT8 CPetMng::getRandomFruit() {
	RandomOdds<UINT32> randomItemOdds;
	for (size_t n = ePFT_Banana; n <= ePFT_Sapodilla; ++n) {
		const PetFeedCfg* pPetFeedCfg = CPetPropMng::Instance()->GetPetFeedCfg(n);	
		if (NULL == pPetFeedCfg) {
			continue;
		}
		randomItemOdds.PushOdds(n, pPetFeedCfg->ucPercent);
	}

	UINT32* pHitOdds = randomItemOdds.GetHitOdds();
	if (NULL == pHitOdds) {
		return ePFT_Banana;
	}

	return *pHitOdds;
}

BOOL CPetMng::fillPlayerData(CPlayerPet* pPlayerPet, DT_PET_CLI_DATA_LST& rAllPetCliData) {
	if (NULL == pPlayerPet) {
		return FALSE;
	}
	
	const MapPet& mpPet = pPlayerPet->GetAllPet();
	for (MapPet::const_iterator itor = mpPet.begin(), end = mpPet.end()
		; end != itor && rAllPetCliData.wPetNum <= MAX_PET_NUM
		; ++ itor)
	{
		if (NULL == itor->second) {
			return FALSE;
		}

		itor->second->FillDT_PET_CLI_DATA(rAllPetCliData.astPetData[rAllPetCliData.wPetNum]);
		++ rAllPetCliData.wPetNum;
	}

	return TRUE;
}

UINT16 CPetMng::OnOpenPetRule(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_RULE_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_OPEN_PET_RULE::ID_IS_NOT_OPEN;
	}

	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ERR_OPEN_PET_RULE::ID_IS_NOT_OPEN;
	}
	
	poPlayerPet->SetRuleState(FALSE);
	//规则
	const PetSysCfg& stSysProp = CPetPropMng::Instance()->GetPetSysCfg();
	
	
	UINT8 byItemNum = 0;
	SDStrncpy(stAck.aszItemDesc, stSysProp.strItemDesc.c_str(), NORMAL_MSG_LEN - 1);
	//道具
	for ( vector<UINT32>::const_iterator itr = stSysProp.vecItemID.begin(); itr != stSysProp.vecItemID.end() && byItemNum < MAX_DISPALY_GOODS_NUM; itr++, byItemNum++)
	{
		CItemMgr::Instance()->GetDT_ITEM_DATA_CLI3(*itr, 1, stAck.stItemsInfo.astItemList[byItemNum]);
	}
	stAck.stItemsInfo.byItemNum = byItemNum;
	//1-20级宠物 //没有变异
	CPetPropMng::Instance()->GetLowPetCli( poPlayer, stAck.stLowPetInfo);
	//20级宠物
	CPetPropMng::Instance()->GetMiddlePetCli( poPlayer, stAck.stMiddlePetInfo);
	if( poPlayer->GetBuild(EBK_PETLIANYAOHU) )
	{
		SDStrncpy(stAck.aszPetRule, stSysProp.strRuleDesc.c_str(), MAX_PET_RULE_LEN - 1);
		//炼化宠物//没有普通
		CPetPropMng::Instance()->GetHighPetCli( poPlayer, stAck.stHighPetInfo);
	}
	

	return ERR_OPEN_PET_RULE::ID_SUCCESS;
}

UINT8 CPetMng::GetPetTypeLevel(UINT8 ucPetType) {
	if (ucPetType >= ePT_Wind && ucPetType <= ePT_Land) {
		return 1;
	}
	if (ucPetType >= ePT_Ice && ucPetType <= ePT_Hot) {
		return 2;
	}
	if (ucPetType >= ePT_Soul && ucPetType <= ePT_Dream) {
		return 3;
	}
	if (ucPetType == ePT_Legend) {
		return 4;
	}
	return 0;
}

UINT16 CPetMng::GetBattlePetInfo(CPlayer* poPlayer, PKT_CLIGS_PLAYER_INFO_PET_ACK& stAck )
{
	if ( NULL == poPlayer)
	{
		return ERR_PLAYER_INFO_PET::ID_NOT_OPEN;
	}
	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ERR_PLAYER_INFO_PET::ID_NOT_OPEN;
	}
	const CPet* poPet = poPlayerPet->GetFightPet();
	if ( NULL == poPet)
	{
		stAck.byHavePet = ESM_NO;
	}
	else
	{
		stAck.byHavePet = ESM_YES;
		poPet->FillDT_PET_CLI_DATA(stAck.astPetInfo[0]);
	}
	return ERR_PLAYER_INFO_PET::ID_SUCCESS;
}

UINT8 CPetMng::getRandomPetTypeByTypeLevel(UINT8 ucPetTypeLevel) {
	RandomOdds<UINT8> randomPetTypeOdds;
	if (1 == ucPetTypeLevel) {
		randomPetTypeOdds.PushOdds(1, 10);
		randomPetTypeOdds.PushOdds(2, 10);
		randomPetTypeOdds.PushOdds(3, 10);
		randomPetTypeOdds.PushOdds(4, 10);
	}
	if (2 == ucPetTypeLevel) {
		randomPetTypeOdds.PushOdds(5, 10);
		randomPetTypeOdds.PushOdds(6, 10);
		randomPetTypeOdds.PushOdds(7, 10);
	}
	if (3 == ucPetTypeLevel) {
		randomPetTypeOdds.PushOdds(8, 10);
		randomPetTypeOdds.PushOdds(9, 10);
	}
	if (4 == ucPetTypeLevel) {
		randomPetTypeOdds.PushOdds(10, 10);
	}
	UINT8* pPetType = randomPetTypeOdds.GetHitOdds();
	if (NULL == pPetType) {
		return 1;
	}
	return *pPetType;
}

UINT16 CPetMng::OnOpenNeiDan(CPlayer* poPlayer, PKT_CLIGS_OPEN_NEIDAN_REQ& stReq, PKT_CLIGS_OPEN_NEIDAN_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_OPEN_NEIDAN::ID_IS_NOT_OPEN;
	}

	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ERR_OPEN_NEIDAN::ID_IS_NOT_OPEN;
	}

	CPet* poPet = poPlayerPet->GetPet( stReq.wPetID );
	if ( NULL == poPet)
	{
		return ERR_OPEN_NEIDAN::ID_NOT_FIND_PET;
	}
	
	const PetBaseCfg* poBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg( poPet->GetDT_PET_DATA().wPetCfgId);
	if ( NULL == poBaseCfg)
	{
		return ERR_OPEN_NEIDAN::ID_NOT_FIND_PET;
	}

	SDStrncpy(stAck.aszPetPic, poBaseCfg->strPetPic.c_str(), MAX_PET_NAME_LENGTH - 1);

	poPlayerPet->GetNeiDanByBag( stAck.stBagNeiDan );
	poPet->GetNeiDanCli( stAck.stPetNeiDan );

	return ERR_OPEN_NEIDAN::ID_SUCCESS;
}

UINT16 CPetMng::OnDressNeiDan(CPlayer* poPlayer, PKT_CLIGS_DRESS_NEIDAN_REQ& stReq, PKT_CLIGS_DRESS_NEIDAN_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_DRESS_NEIDAN::ID_IS_NOT_OPEN;
	}

	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ERR_DRESS_NEIDAN::ID_IS_NOT_OPEN;
	}

	CPet* poPet = poPlayerPet->GetPet( stReq.wPetID );
	if ( NULL == poPet)
	{
		return ERR_DRESS_NEIDAN::ID_NOT_FIND_PET;
	}

	CPetNeiDan* poNeiDan = poPlayerPet->GetNeiDan( stReq.stNeiDan.wItemID, stReq.stNeiDan.wItemIdx);
	if ( NULL == poNeiDan)
	{
		return ERR_DRESS_NEIDAN::ID_NOT_FIND_NEIDAN;
	}

	stAck.stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

	if ( EEDT_DRESS == stReq.byDressType || EEDT_REPLACE == stReq.byDressType)
	{
		poNeiDan->Dress( poPet );
	}
	else if ( EEDT_UNDRESS == stReq.byDressType)
	{
		if( !poNeiDan->UnDress() )
		{
			return ERR_DRESS_NEIDAN::ID_BAG_IS_FULL;
		}
	}

	poPlayer->ResetBattleAttr(FALSE);

	stAck.stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	stAck.byDressType = stReq.byDressType;
	poPet->GetNeiDanMark(stAck.stPetNeiDan);
	poPlayerPet->GetNeiDanMarkByBag(stAck.stBagNeiDan);
	return ERR_DRESS_NEIDAN::ID_SUCCESS;
}

UINT16 CPetMng::OnUpdataNeiDan(CPlayer* poPlayer,PKT_CLIGS_UPDATA_NEIDAN_REQ& stReq, PKT_CLIGS_UPDATA_NEIDAN_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_UPDATA_NEIDAN::ID_IS_NOT_OPEN;
	}

	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ERR_UPDATA_NEIDAN::ID_IS_NOT_OPEN;
	}

	CPetNeiDan* poNeiDan = poPlayerPet->GetNeiDan( stReq.stNeiDan.wItemID, stReq.stNeiDan.wItemIdx);
	if ( NULL == poNeiDan)
	{
		return ERR_UPDATA_NEIDAN::ID_NOT_FIND_NEIDAN;
	}

	set<CPetNeiDan*> setNeiDan;
	UINT64 qwAddExperience = 0;
	UINT32 dwDecCoin = 0;
	for (UINT8 byIdx = 0; byIdx < stReq.stDevorNeiDan.byNum; byIdx++)
	{
		CPetNeiDan* poDevor = poPlayerPet->GetNeiDanByBag( stReq.stDevorNeiDan.astNeiDanInfo[byIdx].wItemID, stReq.stDevorNeiDan.astNeiDanInfo[byIdx].wItemIdx );
		if ( (NULL == poDevor) || ( setNeiDan.end() != setNeiDan.find(poDevor) ) )
		{
			continue;
		}

		qwAddExperience += poDevor->GetBackExperience();
		dwDecCoin += poDevor->GetBackCoin();

		setNeiDan.insert(poDevor);
	}

	if ( poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < dwDecCoin )
	{
		return ERR_COMMON::ID_COIN_NOT_ENOUGH;
	}
	UINT16 wOldLevel = poNeiDan->GetDT_NEIDAN_DATA().wNeiDanLevel;
	stAck.stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	poNeiDan->AddExperience(qwAddExperience);
	stAck.stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	poPlayer->DecCoin( dwDecCoin, CRecordMgr::EDCT_DEVOR_NEIDAN, poNeiDan->GetDT_NEIDAN_DATA().wNeiDanID, wOldLevel, poNeiDan->GetDT_NEIDAN_DATA().wNeiDanLevel);
	poNeiDan->GetNeiDanCli(stAck.stNeiDan);
	poPlayerPet->GetNeiDanMarkByBag(stAck.stBagNeiDan);
	stAck.stResSync.byResKind = ACTE_COIN;
	stAck.stResSync.qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

	return ERR_UPDATA_NEIDAN::ID_SUCCESS;
}