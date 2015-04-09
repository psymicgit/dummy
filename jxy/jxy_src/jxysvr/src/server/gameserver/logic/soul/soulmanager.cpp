#include "soulmanager.h"
#include <assert.h>
#include "souldef.h"
#include "playersoul.h"
#include "sdtime.h"
#include "soulpropmgr.h"
#include "logic/item/itemmgr.h"
#include "sdutil.h"
#include <sdloggerimpl.h> 
#include "logic/player/player.h"
#include "logic/base/randomodds.h"
#include "soul.h"
#include "logic/hero/hero.h"
#include "errdef.h"
#include "logic/lua/luamgr.h"
#include "logic/specialeffects/specialeffectsmgr.h"
#include "../activityencourage/oprateingactivitymgr.h"
#include <sstream>
#include "logic/other/fewparam.h"

CSoulManager::CSoulManager() {
	
}

CSoulManager::~CSoulManager() {

}

UINT8 CSoulManager::GetSoulRepInfo(CPlayer* pPlayer, PKT_CLIGS_SOULREP_INFO_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_SOULREP_INFO::ID_IS_UNOPEN;
	}
	
	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_SOULREP_INFO::ID_IS_UNOPEN;
	}

	DT_PLAYER_SOUL_DATA& rSoulData = pPlayerSoul->GetDT_PLAYER_SOUL_DATA();
	FillSoulRepInfo(pPlayer, rMsgAck.stSoulRepInfo);
	rSoulData.qwLastTime = SDNow().GetTimeValue();
	rMsgAck.byOpenActivity = IsSoulRepActivity(pPlayer) ? 1 : 0;
	return ERR_SOULREP_INFO::ID_SUCCESS;
}

UINT8 CSoulManager::ProductSoul(CPlayer* pPlayer, PKT_CLIGS_PRODUCT_SOUL_REQ& rMsgReq, PKT_CLIGS_PRODUCT_SOUL_ACK& msgAck) {
	UINT8 ucProductType = rMsgReq.byProductType;
	if (NULL == pPlayer
		|| ucProductType < ePST_Low || ucProductType >= ePST_Cnt)
	{
		PRINT_ERR_INFO;
		return ERR_PRODUCT_SOUL::ID_UNKNOWN_ERROR;
	}

	if (ePST_Activity == ucProductType && !IsSoulRepActivity(pPlayer)) {
		return ERR_PRODUCT_SOUL::ID_NO_IN_ACTIVITY;
	}

	const SoulProductCfg* pSoulProducfCfg = CSoulPropMgr::Instance()->GetSoulProductCfg(ucProductType);
	if (NULL == pSoulProducfCfg) {
		PRINT_ERR_INFO;
		return ERR_PRODUCT_SOUL::ID_UNKNOWN_ERROR;
	}

	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_PRODUCT_SOUL::ID_IS_UNOPEN;
	}

	DT_PLAYER_SOUL_DATA& rSoulData = pPlayerSoul->GetDT_PLAYER_SOUL_DATA();
	if (!IsToday(rSoulData.qwLastTime)) {
		rSoulData.abyTodayCoinCnt[ucProductType] = 0;
	}

	// 探索次数
	UINT32 unCnt = rSoulData.abyTodayCoinCnt[ucProductType];
	INT8 ucCostType = CLuamgr::Instance()->GetProcuctSoulCostType(ucProductType, rMsgReq.bySearchType, unCnt);
	UINT32 unCostNum = CLuamgr::Instance()->GetProcuctSoulCostNum(ucProductType, rMsgReq.bySearchType, unCnt);

	UINT8 ucMaxStar = 1;

	// 有剩余次数，使用铜钱
	if (eSCT_Coin == ucCostType) {
		// 判断是否有足够的铜钱
		if (pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < unCostNum) {
			return ERR_PRODUCT_SOUL::ID_NO_ENOUGHT_COIN;
		}
	} else if (eSCT_Gold == ucCostType) {
		// 判断是否有足够的元宝
		if (pPlayer->GetDT_PLAYER_BASE_DATA().qwGold < unCostNum) {
			return ERR_PRODUCT_SOUL::ID_NO_ENOUGHT_GOLD;
		}
	}

	// 连续探索次数
	UINT8 ucSearchNum = rMsgReq.bySearchType == 0 ? 1 : 10;
	// 武魂能否塞入武魂背包
	if (pPlayerSoul->GetSoulBagLeftSize() < ucSearchNum) {
		return ERR_PRODUCT_SOUL::ID_SOUL_BAG_FULL;
	}
	// 材料能够塞入背包
	if (pPlayer->GetBag().GetIdleSize() < ucSearchNum) {
		return ERR_PRODUCT_SOUL::ID_ITEM_BAG_FULL;
	}

	// 连续探索
	for (size_t n = 0; n < ucSearchNum; ++n) {
		++ rSoulData.wFiveStarCnt;
		++ rSoulData.wSixStarCnt;

		// 获得材料或者魂
		BOOL bGetSoul = Random(0, 100) <= pSoulProducfCfg->ucSoulPercent;
		if (bGetSoul) {
			//随机 获取什么样的魂
			RandomOdds<UINT32> randomStarOdds;
			for (size_t n = 0; n < pSoulProducfCfg->vPossibleStar.size(); ++n) {
				if (5 == pSoulProducfCfg->vPossibleStar[n] && rSoulData.wFiveStarCnt < FIVE_SATR_CNT) {
					continue;
				} 
				if (6 == pSoulProducfCfg->vPossibleStar[n] && rSoulData.wSixStarCnt < SIX_SATR_CNT) {
					continue;
				}
				randomStarOdds.PushOdds(pSoulProducfCfg->vPossibleStar[n], pSoulProducfCfg->vStarWeight[n]);
			}
			UINT32* pHitOdds = randomStarOdds.GetHitOdds();
			if (NULL == pHitOdds) {
				PRINT_ERR_INFO;
				// return ERR_PRODUCT_SOUL::ID_UNKNOWN_ERROR;
				continue;
			}
			INT8 ucStar = (INT8)*pHitOdds;
			CSoul* pSoul = GenerateSoul(ucStar);
			if (NULL == pSoul) {
				PRINT_ERR_INFO;
				// return ERR_PRODUCT_SOUL::ID_UNKNOWN_ERROR;
				continue;
			}

			if (5 == ucStar) {
				rSoulData.wFiveStarCnt = 0;
			}

			if (6 == ucStar) {
				rSoulData.wSixStarCnt = 0;
			}

			if (ucStar > ucMaxStar) {
				ucMaxStar = ucStar;
			}

			pPlayerSoul->AddSoulToBag(pSoul);
			pSoul->SetOwner(pPlayer);
			pSoul->GetSoulCli(msgAck.astSoulData[msgAck.bySoulNum]);
			++ msgAck.bySoulNum;

			DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();

			ostringstream oss;
			for (size_t n = 0; n < rSoulData.byBaseNum; ++n) {
				oss << (UINT32)(rSoulData.astSoulAttr[n].byKindID) << 
					"," << rSoulData.astSoulAttr[n].dwKindValue <<
					"," << (UINT32)(rSoulData.astSoulAttr[n].byKindPrecent) << ";";
			}

			CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_ADDSOUL, CRecordMgr::EAST_Product
				, pSoul->GetExperience(), 1
				, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
				, pSoul->GetDT_SOUL_DATA().wSoulID, pSoul->GetDT_SOUL_DATA().stSpecialEffect.wEffectID, 0, 0, oss.str());			

		} else {
			UINT16 usMeterialGoodId = CItemMgr::Instance()->RandomItemInnerBox(pSoulProducfCfg->unMeterialGoodId);
			//如果获取材料
			CGoods* pGoods = pPlayer->GetBag().GetGoodsItem(usMeterialGoodId);
			if (NULL == pGoods) {
				ECreateItemRet eItemRet = ECIR_SUCCESS;
				pGoods = (CGoods*)CItemMgr::Instance()->CreateItemIntoBag(pPlayer, usMeterialGoodId, 1
					, eItemRet, 0, CRecordMgr::EAIT_SOUL_METERIAL);

				if (ECIR_SUCCESS != eItemRet) {
					PRINT_ERR_INFO;
					// return ERR_PRODUCT_SOUL::ID_UNKNOWN_ERROR;
					continue;
				}
			} else { // 堆叠
				if (!pPlayer->GetBag().AddItem(pGoods)) {
					PRINT_ERR_INFO;
					// return ERR_PRODUCT_SOUL::ID_UNKNOWN_ERROR;
					continue;
				}
			}
			if (NULL != pGoods) {
				pGoods->GetDT_GOODS_DATA_CLI(msgAck.astMeterialGood[msgAck.byMeterialNum]);
				msgAck.astMeterialGood[msgAck.byMeterialNum].wPileCount = 1;
				SGoodsProp* pGoodProp = pGoods->GetGoodsProp();
				if (NULL != pGoodProp && pGoodProp->dwParam1 > ucMaxStar) {
					ucMaxStar = pGoodProp->dwParam1;
				}
			}
			++ msgAck.byMeterialNum;
		}
	}

	if (eSCT_Coin == ucCostType) {
		// 扣铜钱
		pPlayer->DecCoin(unCostNum, CRecordMgr::EDGT_PRODUCT_SOUL);
		// 扣使用铜钱次数
		++ rSoulData.abyTodayCoinCnt[ucProductType];
	} else if (eSCT_Gold == ucCostType) {
		// 扣元宝
		pPlayer->DecGold(unCostNum, CRecordMgr::EDGT_PRODUCT_SOUL);
	}
	CFewParam& oFewParam = pPlayer->GetFewParam();
	if ( ePST_Low == ucProductType)
	{
		oFewParam.AddHellPassNum(ucSearchNum);
	}
	else if ( ePST_Middle == ucProductType)
	{
		oFewParam.AddWorldPassNum(ucSearchNum);
	}
	else if ( ePST_High == ucProductType)
	{
		oFewParam.AddHeavenPassNum(ucSearchNum);
	}
	FillSoulRepInfo(pPlayer, msgAck.stSoulRepInfo);
	rSoulData.qwLastTime = SDNow().GetTimeValue();
	
	msgAck.byBoxType = GetSoulBoxType(ucMaxStar);
	return ERR_PRODUCT_SOUL::ID_SUCCESS;
}

UINT8 CSoulManager::EquipSoul(CPlayer* pPlayer, UINT16 unHeroId, UINT16 usSoulID, UINT32 unIndex) {
	if (NULL == pPlayer) {
		PRINT_ERR_INFO;
		return ERR_HERO_EQUIP_SOUL::ID_UNKNOWN_ERROR;
	}

	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_HERO_EQUIP_SOUL::ID_IS_UNOPEN;
	}

	CHero* pHero = pPlayer->GetHero(unHeroId);
	if (NULL == pHero) {
		return ERR_HERO_EQUIP_SOUL::ID_NO_HERO;
	}

	if (pHero->IsSoulAllEquip()) {
		return ERR_HERO_EQUIP_SOUL::ID_HERO_ALL_EQUIP;
	}

	// 判断位置是否有魂
	CSoul* pSoul = pPlayerSoul->GetSoul(usSoulID, unIndex);
	if (NULL == pSoul) {
		PRINT_ERR_INFO;
		return ERR_HERO_EQUIP_SOUL::ID_NO_THIS_SOUL;
	}

	if (eDevourSoul == pSoul->GetDT_SOUL_DATA().byIsNormalSoul) {
		return ERR_HERO_EQUIP_SOUL::ID_DEVOUR_SOUL_NO_EQUIP;
	}

	// 身上是否有相同战魂
	if (pHero->IsHaveSameSoulType(pSoul)) {
		return ERR_HERO_EQUIP_SOUL::ID_UNKNOWN_ERROR;
	}

	CSoul bagTmpSoul = *pSoul;

	// 从魂背包脱下来
	pPlayerSoul->DelSoulFromBag(&bagTmpSoul);
	// 穿上去
	pHero->EquipSoul(&bagTmpSoul);

	// 更改属性
	pPlayer->ResetBattleAttr(TRUE);
	return ERR_HERO_EQUIP_SOUL::ID_SUCCESS;
}

UINT8 CSoulManager::UnDressSoul(CPlayer* pPlayer, UINT16 unHeroId, UINT16 usSoulID, UINT32 unIndex) {
	if (NULL == pPlayer) {
		PRINT_ERR_INFO;
		return ERR_HERO_UNDRESS_SOUL::ID_UNKNOWN_ERROR;
	}

	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_HERO_UNDRESS_SOUL::ID_IS_UNOPEN;
	}

	CHero* pHero = pPlayer->GetHero(unHeroId);
	if (NULL == pHero) {
		return ERR_HERO_UNDRESS_SOUL::ID_NO_HERO;
	}

	// 武魂能否塞入武魂背包
	if (pPlayerSoul->IsSoulBagFull()) {
		return ERR_HERO_UNDRESS_SOUL::ID_SOUL_BAG_FULL;
	}

	// 判断位置是否有魂
	CSoul* pSoul = pHero->GetEquipSoul(usSoulID, unIndex);
	if (NULL == pSoul) {
		PRINT_ERR_INFO;
		return ERR_HERO_UNDRESS_SOUL::ID_NO_THIS_SOUL;
	}

	CSoul bagTmpSoul = *pSoul;
	pHero->UnDressSoul(&bagTmpSoul);
	pPlayerSoul->AddSoulToBag(&bagTmpSoul);

	// 更改属性
	pPlayer->ResetBattleAttr(TRUE);
	return ERR_HERO_UNDRESS_SOUL::ID_SUCCESS;
}

UINT8 CSoulManager::OnReplaceSoul(CPlayer* pPlayer, PKT_CLIGS_HERO_REPLACE_SOUL_REQ& rMsgReq, PKT_CLIGS_HERO_REPLACE_SOUL_ACK& msgAck) {
	if (NULL == pPlayer) {
		PRINT_ERR_INFO;
		return ERR_HERO_REPLACE_SOUL::ID_UNKNOWN_ERROR;
	}

	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_HERO_REPLACE_SOUL::ID_IS_UNOPEN;
	}

	CHero* pHero = pPlayer->GetHero(rMsgReq.wHeroId);
	if (NULL == pHero) {
		return ERR_HERO_REPLACE_SOUL::ID_NO_HERO;
	}

	// 判断位置是否有魂
	CSoul* pHeroSoul = pHero->GetEquipSoul(rMsgReq.wHeroSoulID, rMsgReq.dwHeroSoulIdx);
	if (NULL == pHeroSoul) {
		PRINT_ERR_INFO;
		return ERR_HERO_REPLACE_SOUL::ID_NO_THIS_SOUL;
	}

	// 判断位置是否有魂
	CSoul* pBagSoul = pPlayerSoul->GetSoul(rMsgReq.wBagSoulID, rMsgReq.dwBagSoulIdx);
	if (NULL == pBagSoul) {
		PRINT_ERR_INFO;
		return ERR_HERO_REPLACE_SOUL::ID_NO_THIS_SOUL;
	}

	if (eDevourSoul == pBagSoul->GetDT_SOUL_DATA().byIsNormalSoul) {
		return ERR_HERO_REPLACE_SOUL::ID_DEVOUR_SOUL_NO_REPLACE;
	}

	// 类型不一样
	if (pHeroSoul->GetDT_SOUL_DATA().bySoulType != pBagSoul->GetDT_SOUL_DATA().bySoulType) {
		return ERR_HERO_REPLACE_SOUL::ID_UNKNOWN_ERROR;
	}

	CSoul bagTmpSoul = *pBagSoul;
	CSoul heroTmpSoul = *pHeroSoul;
	// 删
	pPlayerSoul->DelSoulFromBag(&bagTmpSoul);
	pHero->UnDressSoul(&heroTmpSoul);

	// 加
	pPlayerSoul->AddSoulToBag(&heroTmpSoul);
	pHero->EquipSoul(&bagTmpSoul);

	// 更改属性
	pPlayer->ResetBattleAttr(TRUE);

	pHero->GetHeroPanelInfo(msgAck.stHeroInfo);
	return ERR_HERO_REPLACE_SOUL::ID_SUCCESS;
}

UINT8 CSoulManager::OpenDevourSoulAck(CPlayer* pPlayer, PKT_CLIGS_OPEN_DEVOUR_SOUL_REQ& rMsgReq, PKT_CLIGS_OPEN_DEVOUR_SOUL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_OPEN_DEVOUR_SOUL::ID_IS_UNOPEN;
	}

	CSoul* pSoul = GetSoul(pPlayer, rMsgReq.stSoulID);
	if (NULL == pSoul) {
		return ERR_OPEN_DEVOUR_SOUL::ID_NOT_FIND_SOUL;
	}

	pSoul->GetSoulCli(rMsgAck.stMainSoul);
	FillSoulBagData(pPlayer, rMsgAck.stSoulBag);

	rMsgAck.stResData.astPlayerRes[0].byResKind = ACTE_COIN;
	rMsgAck.stResData.astPlayerRes[0].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	rMsgAck.stResData.astPlayerRes[1].byResKind = ACTE_GOLD;
	rMsgAck.stResData.astPlayerRes[1].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	rMsgAck.stResData.byResNum = 2;

	rMsgAck.stCostRes.byResKind = ACTE_COIN;
	rMsgAck.stCostRes.qwResValue = CLuamgr::Instance()->GetSoulDevourCost(0);

	DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
	const SoulLvCfg* pSoulLv = CSoulPropMgr::Instance()->GetSoulLvCfg(rSoulData.bySoulStar, rSoulData.bySoulLevel);
	const SoulLvCfg* pSoulNextLv = CSoulPropMgr::Instance()->GetSoulLvCfg(rSoulData.bySoulStar, rSoulData.bySoulLevel + 1);
	if (NULL != pSoulLv && NULL != pSoulNextLv) {
		rMsgAck.byAttrUpPer = pSoulNextLv->unAddAttrPercent - pSoulLv->unAddAttrPercent;
	}

	return ERR_OPEN_DEVOUR_SOUL::ID_SUCCESS; 
}

UINT8 CSoulManager::DevourSoul(CPlayer* pPlayer, PKT_CLIGS_DEVOUR_SOUL_REQ& rMsgReq, PKT_CLIGS_DEVOUR_SOUL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_DEVOUR_SOUL::ID_IS_UNOPEN;
	}

	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_DEVOUR_SOUL::ID_IS_UNOPEN;
	}

	CSoul* pSoul = GetSoul(pPlayer, rMsgReq.stMainSoul);
	if (NULL == pSoul) {
		return ERR_DEVOUR_SOUL::ID_NOT_FIND_SOUL;
	}

	if (eDevourSoul == pSoul->GetDT_SOUL_DATA().byIsNormalSoul) {
		return ERR_DEVOUR_SOUL::ID_DEVOUR_SOUL_NO_DEVOUR;
	}

	rMsgAck.stPowerInfo.dwOldPower = pPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

	UINT64 ullExp = 0;
	set<CSoul*> setDevourSoul;
	for (size_t n = 0; n < rMsgReq.stDevourSoul.wSoulNum; ++n) {
		// 不是背包战魂
		if (0 != rMsgReq.stDevourSoul.astSoulLstMark[n].wHeroId) {
			return ERR_DEVOUR_SOUL::ID_NOT_FIND_SOUL;
		}
		CSoul* pDevourSoul = GetSoul(pPlayer, rMsgReq.stDevourSoul.astSoulLstMark[n]);
		if (NULL == pDevourSoul) {
			return ERR_DEVOUR_SOUL::ID_NOT_FIND_SOUL;
		}
		if (setDevourSoul.end() != setDevourSoul.find(pDevourSoul) || pSoul == pDevourSoul) {
			return ERR_DEVOUR_SOUL::ID_IS_SAME;
		}

		//排除数据异常
		if ( CSoulPropMgr::Instance()->GetSoulBaseCfg(pDevourSoul->GetDT_SOUL_DATA().wSoulID) )
		{
			ullExp += pDevourSoul->GetExperience();
			setDevourSoul.insert(pDevourSoul);
		}
		
	}
	
	UINT32 unDevourCostCoin = (CLuamgr::Instance()->GetSoulDevourCost((UINT8)rMsgReq.stDevourSoul.wSoulNum));

	//检验消耗
	if (pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < unDevourCostCoin) {
		return ERR_COMMON::ID_COIN_NOT_ENOUGH;
	}

	UINT8 bySoulOldLevel = pSoul->GetDT_SOUL_DATA().bySoulLevel;
	UINT32 dwSoulOldExperience = pSoul->GetExperience();
	pSoul->AddExeprience(ullExp);
	UpdateSoulData(pPlayer, rMsgReq.stMainSoul, pSoul);

	//去除战魂
	for (set<CSoul*>::iterator itor = setDevourSoul.begin(), end = setDevourSoul.end()
		; end != itor
		; ++itor)
	{
		CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_DECSOUL, 1
			, dwSoulOldExperience, pSoul->GetExperience()
			, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
			, (*itor)->GetDT_SOUL_DATA().wSoulID, pSoul->GetDT_SOUL_DATA().wSoulID, bySoulOldLevel, pSoul->GetDT_SOUL_DATA().bySoulLevel);

		pPlayerSoul->DelSoulFromBag(*itor);
	}

	//扣除资源
	pPlayer->DecCoin(static_cast<UINT32>(unDevourCostCoin)
		, CRecordMgr::EDCT_DEVOUR_SOUL, rMsgReq.stDevourSoul.wSoulNum, ullExp);

	pSoul->GetSoulCli(rMsgAck.stMainSoul);
	FillSoulBagData(pPlayer, rMsgAck.stSoulBag);

	rMsgAck.stResData.astPlayerRes[0].byResKind = ACTE_COIN;
	rMsgAck.stResData.astPlayerRes[0].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	rMsgAck.stResData.astPlayerRes[1].byResKind = ACTE_GOLD;
	rMsgAck.stResData.astPlayerRes[1].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	rMsgAck.stResData.byResNum = 2;

	rMsgAck.stCostRes.byResKind = ACTE_COIN;
	rMsgAck.stCostRes.qwResValue = CLuamgr::Instance()->GetSoulDevourCost(0);

	pPlayer->ResetBattleAttr(TRUE);
	rMsgAck.stPowerInfo.dwCurPower = pPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

	if (0 == rMsgReq.stMainSoul.wHeroId) {
		rMsgAck.stPowerInfo.dwCurPower = rMsgAck.stPowerInfo.dwOldPower;
	}

	DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
	const SoulLvCfg* pSoulLv = CSoulPropMgr::Instance()->GetSoulLvCfg(rSoulData.bySoulStar, rSoulData.bySoulLevel);
	const SoulLvCfg* pSoulNextLv = CSoulPropMgr::Instance()->GetSoulLvCfg(rSoulData.bySoulStar, rSoulData.bySoulLevel + 1);
	if (NULL != pSoulLv && NULL != pSoulNextLv) {
		rMsgAck.byAttrUpPer = pSoulNextLv->unAddAttrPercent - pSoulLv->unAddAttrPercent;
	}

	return ERR_DEVOUR_SOUL::ID_SUCCESS; 
}

CSoul* CSoulManager::GenerateSoul(UINT8 ucStar) {
	RandomOdds<UINT32> randomSoulIdOdds;
	const MapSoulCfg& mpSoulCfg = CSoulPropMgr::Instance()->GetMapSoulCfg();
	for (MapSoulCfg::const_iterator itor = mpSoulCfg.begin(), end = mpSoulCfg.end()
		; end != itor
		; ++ itor) 
	{
		const SoulBaseCfg* pSoulCfg = itor->second;
		if (NULL != pSoulCfg && pSoulCfg->ucStar == ucStar) {
			randomSoulIdOdds.PushOdds(pSoulCfg->unSoulId, pSoulCfg->unWeight);
		}
	}
	UINT32* pHitSoulIdOdds = randomSoulIdOdds.GetHitOdds();
	if (NULL == pHitSoulIdOdds) {
		PRINT_ERR_INFO;
		return NULL;
	}

	return GenerateSoulById(*pHitSoulIdOdds);
}

VOID CSoulManager::FillSoulBagData(CPlayer* pPlayer, DT_SOUL_DATA_LST_CLI& rBagSoulData) {
	memset(&rBagSoulData, 0, sizeof(rBagSoulData));
	if (NULL == pPlayer) {
		return;
	}
	CPlayerSoul* pSoul = pPlayer->GetPlayerSoul();
	if (NULL == pSoul) {
		return;
	}
	
	pSoul->GetSoulBagDataCli(rBagSoulData);
}

CSoul* CSoulManager::GetSoul(CPlayer* pPlayer, DT_CLIGS_SOUL_MARK_DATA& rSoulMarkData) {
	if (NULL == pPlayer) {
		return NULL;
	}
	
	if (0 == rSoulMarkData.wHeroId) {
		CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
		if (NULL == pPlayer) {
			return NULL;
		}
		return pPlayerSoul->GetSoul(rSoulMarkData.wSoulID, rSoulMarkData.dwSoulIdx);
	} else {
		CHero* pHero = pPlayer->GetHero(rSoulMarkData.wHeroId);
		if (NULL == pHero) {
			return NULL;
		}
		return pHero->GetEquipSoul(rSoulMarkData.wSoulID, rSoulMarkData.dwSoulIdx);
	}

	return NULL;
}

VOID CSoulManager::UpdateSoulData(CPlayer* pPlayer, DT_CLIGS_SOUL_MARK_DATA& rSoulMarkData, CSoul* pSoul) {
	if (NULL == pPlayer || NULL == pSoul) {
		return;
	}

	if (0 == rSoulMarkData.wHeroId) {
		CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
		if (NULL == pPlayer) {
			return;
		}
		pPlayerSoul->UpdateSoulData(pSoul);
	} else {
		CHero* pHero = pPlayer->GetHero(rSoulMarkData.wHeroId);
		if (NULL == pHero) {
			return;
		}
		pHero->UpdateSoulData(pSoul);
	}
}

UINT8 CSoulManager::OpenReflash(CPlayer* pPlayer, PKT_CLIGS_OPEN_REFLASH_SOUL_REQ& rMsgReq, PKT_CLIGS_OPEN_REFLASH_SOUL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_OPEN_REFLASH_SOUL::ID_IS_UNOPEN;
	}

	CSoul* pSoul = GetSoul(pPlayer, rMsgReq.stMainSoul);
	if (NULL == pSoul) {
		return ERR_OPEN_REFLASH_SOUL::ID_NOT_FIND_SOUL;
	}

	pSoul->GetSoulCli(rMsgAck.stSoulData);
	rMsgAck.stCostRes.byResKind = ACTE_GOLD;
	rMsgAck.stCostRes.qwResValue = CLuamgr::Instance()->GetSoulRefleshCost(0);
	rMsgAck.stResData.astPlayerRes[0].byResKind = ACTE_COIN;
	rMsgAck.stResData.astPlayerRes[0].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	rMsgAck.stResData.astPlayerRes[1].byResKind = ACTE_GOLD;
	rMsgAck.stResData.astPlayerRes[1].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	rMsgAck.stResData.byResNum = 2;
	return ERR_OPEN_REFLASH_SOUL::ID_SUCCESS;
}

UINT8 CSoulManager::ReflashSoul(CPlayer* pPlayer, PKT_CLIGS_REFLASH_SOUL_REQ& rMsgReq, tagPKT_CLIGS_REFLASH_SOUL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_REFLASH_SOUL::ID_IS_UNOPEN;
	}

	CSoul* pSoul = GetSoul(pPlayer, rMsgReq.stMainSoul);
	if (NULL == pSoul) {
		return ERR_REFLASH_SOUL::ID_NOT_FIND_SOUL;
	}

	if (eDevourSoul == pSoul->GetDT_SOUL_DATA().byIsNormalSoul) {
		return ERR_REFLASH_SOUL::ID_DEVOUR_SOUL_NO_REFLASH;
	}

	C8Set lockSet;
	DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
	//if (rSoulData.bySoulStar == rSoulData.byBaseNum) {
		for (size_t n = 0; n < rMsgReq.byLockNum && n < MAX_SOUL_ATTR_NUM; n++) {
			lockSet.insert(rMsgReq.abyLockIDLst[n]);
		}
	//}

	for (UINT8 n = 0; n < MAX_SOUL_ATTR_NUM; n ++) {
		rSoulData.abySoulAttrLock[n] = (lockSet.end() != lockSet.find(n));
	}

	UINT32 unCostNum = CLuamgr::Instance()->GetSoulRefleshCost(lockSet.size());
	if (unCostNum > pPlayer->GetDT_PLAYER_BASE_DATA().qwGold) {
		return ERR_REFLASH_SOUL::ID_NO_ENOUGHT_GOLD;
	}

	memset(rSoulData.astSoulAfterAttr, 0, sizeof(rSoulData.astSoulAfterAttr));
	rSoulData.byBaseAfterNum = 0;
	//对属性个数进行随机
	const CRandomMap* poRandom = CSoulPropMgr::Instance()->GetAttrNumRandom(rSoulData.bySoulStar, rSoulData.byBaseNum);
	if (NULL != poRandom) {
		rSoulData.byBaseAfterNum = GetRandomIdxMustHit(*poRandom);
	}
	rSoulData.byBaseAfterNum = rSoulData.byBaseAfterNum == 0 ? 1 : rSoulData.byBaseAfterNum;

	for (size_t n = 0; n < rSoulData.byBaseNum; ++ n) {
		rSoulData.astSoulAfterAttr[n].byKindID = rSoulData.astSoulAttr[n].byKindID;
	}

	FillSoulAttrData(rSoulData.bySoulStar, rSoulData.bySoulLevel
		, rSoulData.byBaseAfterNum, rSoulData.astSoulAfterAttr, lockSet);

	//扣除资源
	pPlayer->DecGold(static_cast<UINT32>(unCostNum)
		, CRecordMgr::EDGT_REFLASH_SOUL, rSoulData.bySoulStar, rSoulData.byBaseNum);

	pSoul->GetSoulCli(rMsgAck.stSoulData);
	rMsgAck.stResData.astPlayerRes[0].byResKind = ACTE_COIN;
	rMsgAck.stResData.astPlayerRes[0].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	rMsgAck.stResData.astPlayerRes[1].byResKind = ACTE_GOLD;
	rMsgAck.stResData.astPlayerRes[1].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	rMsgAck.stResData.byResNum = 2;

	UpdateSoulData(pPlayer, rMsgReq.stMainSoul, pSoul);

	ostringstream oss;
	for (size_t n = 0; n < rSoulData.byBaseAfterNum; ++n) {
		oss << (UINT32)(rSoulData.astSoulAfterAttr[n].byKindID) << 
			"," << rSoulData.astSoulAfterAttr[n].dwKindValue <<
			"," << (UINT32)(rSoulData.astSoulAfterAttr[n].byKindPrecent) << "#";
	}

	CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_REFLASHSOUL, 1
		, 1, 1
		, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
		, pSoul->GetDT_SOUL_DATA().wSoulID, pSoul->GetDT_SOUL_DATA().stSpecialEffect.wEffectID, 0, 0, oss.str());

	return ERR_REFLASH_SOUL::ID_SUCCESS;
}

UINT8 CSoulManager::SaveReflashSoul(CPlayer* pPlayer, PKT_CLIGS_SAVE_REFLASH_SOUL_REQ& rMsgReq, PKT_CLIGS_SAVE_REFLASH_SOUL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		return ERR_SAVE_REFLASH_SOUL::ID_IS_UNOPEN;
	}

	CSoul* pSoul = GetSoul(pPlayer, rMsgReq.stMainSoul);
	if (NULL == pSoul) {
		return ERR_SAVE_REFLASH_SOUL::ID_NOT_FIND_SOUL;
	}
	rMsgAck.stPowerInfo.dwOldPower = pPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	pSoul->SaveReflash(rMsgReq.byIsSave);
	pSoul->GetSoulCli(rMsgAck.stSoulData);

	rMsgAck.stResData.astPlayerRes[0].byResKind = ACTE_COIN;
	rMsgAck.stResData.astPlayerRes[0].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	rMsgAck.stResData.astPlayerRes[1].byResKind = ACTE_GOLD;
	rMsgAck.stResData.astPlayerRes[1].qwResValue = pPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
	rMsgAck.stResData.byResNum = 2;

	for (UINT8 n = 0; n < MAX_SOUL_ATTR_NUM; n ++) {
		pSoul->GetDT_SOUL_DATA().abySoulAttrLock[n] = 0;
	}
	UpdateSoulData(pPlayer, rMsgReq.stMainSoul, pSoul);

	rMsgAck.stPowerInfo.dwOldPower = pPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	pPlayer->ResetBattleAttr(TRUE);
	rMsgAck.stPowerInfo.dwCurPower = pPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

	if (0 == rMsgReq.stMainSoul.wHeroId) {
		rMsgAck.stPowerInfo.dwCurPower = rMsgAck.stPowerInfo.dwOldPower;
	}

	return ERR_SAVE_REFLASH_SOUL::ID_SUCCESS;
}

VOID CSoulManager::FillSoulAttrData(UINT8 ucStarLv, UINT8 ucStrengthLv
	, UINT8& rAttrNum, DT_SOUL_ATTR_DATA* pAttrData, C8Set setLock) 
{
	if (NULL == pAttrData) {
		return;
	}
	
	UINT8 byLeftKindValue = 100;
	for (UINT8 n = 0; n < rAttrNum; n ++) {
		if ( setLock.end() == setLock.find(n)) {
			pAttrData[n].byKindID = RandomAttrId();
		}

		const SSoulAttrRandomProp* pRandom = NULL;
		if (CSoulPropMgr::Instance()->GetAttrValueRandom(
			ucStarLv, rAttrNum, n + 1, pRandom)) 
		{
			if (NULL == pRandom) {
				PRINT_ERR_INFO;
				break;
			}

			UINT8 ucKindPercent = static_cast<UINT8>(Random(pRandom->byBeginRandom, pRandom->byEndRandom));
			if (ucKindPercent >= byLeftKindValue) {
				pAttrData[n].byKindPrecent = byLeftKindValue;
				byLeftKindValue = 0;
			} else {
				if (n + 1 == rAttrNum) {
					pAttrData[n].byKindPrecent = byLeftKindValue;
				} else {
					pAttrData[n].byKindPrecent = ucKindPercent;
				}
				byLeftKindValue -= pAttrData[n].byKindPrecent;
			}

			UINT32 unAttrBaseVal = CSoulPropMgr::Instance()->GetAttrBaseValue(
				ucStarLv, rAttrNum, pAttrData[n].byKindID);

			pAttrData[n].dwKindValue = unAttrBaseVal * pAttrData[n].byKindPrecent / 100;

			const SoulLvCfg* pSoulLv = CSoulPropMgr::Instance()->GetSoulLvCfg(ucStarLv, ucStrengthLv);
			if (NULL == pSoulLv) {
				continue;
			}
			pAttrData[n].dwKindValue = (UINT32)(pAttrData[n].dwKindValue * float(((float)100 + (float)pSoulLv->unAddAttrPercent) / (float)100));
		}
	}
}

UINT8 CSoulManager::SoulExchangeInfo(CPlayer* pPlayer, PKT_CLIGS_SOULEXCHANGE_INFO_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		PRINT_ERR_INFO;
		return ERR_SOULEXCHANGE_INFO::ID_UNKNOWN_ERROR;
	}

	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_SOULEXCHANGE_INFO::ID_IS_UNOPEN;
	}
	
	CBag& rBag = pPlayer->GetBag();
	FillBagMeterialInfo(rBag, rMsgAck.byMeterialNum, rMsgAck.astMeterialInfo);
	// pPlayer->GetBag().GetBagItem(rMsgAck.stBagData);

	const MapSoulExchangeCfg& rSoulExchangeCfg = CSoulPropMgr::Instance()->GetSoulExchangeAllCfg();
	rMsgAck.byExchangeCardNum = rSoulExchangeCfg.size();
	size_t n = 0;
	for (MapSoulExchangeCfg::const_iterator itor = rSoulExchangeCfg.begin(), end = rSoulExchangeCfg.end()
		; end != itor
		; ++ itor, ++ n)
	{
		const SoulExchangeCfg* pCfg = itor->second;
		if (NULL != pCfg) {
			rMsgAck.astExchangeCardInfo[n].dwID = pCfg->unId;
			strcpy(rMsgAck.astExchangeCardInfo[n].aszName, pCfg->strName.c_str());
			for (size_t idx = 0; idx < pCfg->vMeterialId.size(); ++ idx) {
				++ rMsgAck.astExchangeCardInfo[n].byMeterialNum;
				rMsgAck.astExchangeCardInfo[n].astMeterialInfo[idx].dwMeterialID
					= pCfg->vMeterialId[idx];
				rMsgAck.astExchangeCardInfo[n].astMeterialInfo[idx].dwMeterialNum
					= pCfg->vMeterialNum[idx];
			}
		}
	}
	return ERR_SOULEXCHANGE_INFO::ID_SUCCESS;
}

UINT8 CSoulManager::ExchangeSoul(CPlayer* pPlayer, PKT_CLIGS_EXCHANGE_SOUL_REQ& rMsgReq, PKT_CLIGS_EXCHANGE_SOUL_ACK& rMsgAck) {
	if (NULL == pPlayer) {
		PRINT_ERR_INFO;
		return ERR_EXCHANGE_SOUL::ID_UNKNOWN_ERROR;
	}

	if (rMsgReq.dwExchangeNum > MAX_EXCHANGE_SOUL_NUM) {
		rMsgReq.dwExchangeNum = MAX_EXCHANGE_SOUL_NUM;
	}

	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return ERR_EXCHANGE_SOUL::ID_IS_UNOPEN;
	}

	if (rMsgReq.dwExchangeNum > pPlayerSoul->GetSoulBagLeftSize()) {
		return ERR_EXCHANGE_SOUL::ID_SOUL_BAG_FULL;
	}

	const SoulExchangeCfg* pExchgeCfg = CSoulPropMgr::Instance()->GetSoulExchangeCfg(rMsgReq.dwSoulExchangeId);
	if (NULL == pExchgeCfg) {
		return ERR_EXCHANGE_SOUL::ID_UNKNOWN_ERROR;
	}

	for (size_t n = 0; n < pExchgeCfg->vMeterialId.size(); ++n) {
		if (rMsgReq.dwExchangeNum * pExchgeCfg->vMeterialNum[n] > pPlayer->GetBag().GetGoodsPileNum(pExchgeCfg->vMeterialId[n])) {
			return ERR_EXCHANGE_SOUL::ID_NO_ENOUGHT_METERIAL;
		}
	}

	for (size_t n = 0; n < rMsgReq.dwExchangeNum; ++n) {
		//随机 获取什么样的魂
		RandomOdds<UINT32> randomStarOdds;
		for (size_t nIndex = 0; nIndex < pExchgeCfg->vGetStar.size(); ++nIndex) {
			randomStarOdds.PushOdds(pExchgeCfg->vGetStar[nIndex], pExchgeCfg->vGetStarWeight[nIndex]);
		}
		UINT32* pHitOdds = randomStarOdds.GetHitOdds();
		if (NULL == pHitOdds) {
			PRINT_ERR_INFO;
			// return ERR_EXCHANGE_SOUL::ID_UNKNOWN_ERROR;
			continue;
		}
		INT8 ucStar = (INT8)*pHitOdds;
		CSoul* pSoul = GenerateSoul(ucStar);
		if (NULL == pSoul) {
			PRINT_ERR_INFO;
			// return ERR_EXCHANGE_SOUL::ID_UNKNOWN_ERROR;
			continue;
		}

		// 加战魂
		if (!pPlayerSoul->AddSoulToBag(pSoul)) {
			// return ERR_EXCHANGE_SOUL::ID_SOUL_BAG_FULL;
			continue;
		}

		DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
		ostringstream oss;
		for (size_t nAttrIdx = 0; nAttrIdx < rSoulData.byBaseNum; ++nAttrIdx) {
			oss << (UINT32)(rSoulData.astSoulAttr[nAttrIdx].byKindID) << 
				"," << rSoulData.astSoulAttr[nAttrIdx].dwKindValue <<
				"," << (UINT32)(rSoulData.astSoulAttr[nAttrIdx].byKindPrecent) << ";";
		}

		CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_ADDSOUL, CRecordMgr::EAST_Exchange
			, pSoul->GetExperience(), 1
			, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
			, pSoul->GetDT_SOUL_DATA().wSoulID, pSoul->GetDT_SOUL_DATA().stSpecialEffect.wEffectID, 0, 0, oss.str());

		pSoul->GetSoulCli(rMsgAck.astSoulInfo[n]);
	}
	rMsgAck.bySoulNum = rMsgReq.dwExchangeNum;

	// 扣除材料
	for (size_t n = 0; n < pExchgeCfg->vMeterialId.size(); ++n) {
		if (!pPlayer->GetBag().DelGoodItem(pExchgeCfg->vMeterialId[n], rMsgReq.dwExchangeNum * pExchgeCfg->vMeterialNum[n])) {
			return ERR_EXCHANGE_SOUL::ID_NO_ENOUGHT_METERIAL;
		}
	}

	CBag& rBag = pPlayer->GetBag();
	FillBagMeterialInfo(rBag, rMsgAck.byMeterialNum, rMsgAck.astMeterialInfo);

	return ERR_EXCHANGE_SOUL::ID_SUCCESS;
}

UINT8 CSoulManager::RandomAttrId() {
	RandomOdds<UINT8> randomAttrOdds;
	for (size_t nAttrId = EBAK_HP; nAttrId <= EBAK_DECRITRATE; ++nAttrId) {
		randomAttrOdds.PushOdds(nAttrId, 10);
	}
	randomAttrOdds.PushOdds(EBAK_DEFEND, 10);
	UINT8* pAttrId = randomAttrOdds.GetHitOdds();
	if (NULL == pAttrId) {
		return EBAK_HP;
	}
	return *pAttrId;
}

VOID CSoulManager::FillBagMeterialInfo(CBag& rBag, UINT8& rMeterialNum, DT_GOODS_DATA_CLI* pMeterialInfo) {
	if (NULL == pMeterialInfo) {
		return;
	}
	rMeterialNum = 0;
	CID2GoodsShmemMap& rGoodMap = rBag.GetGoodsMap();
	for (size_t n = 0; n < rGoodMap.Size() && rMeterialNum < MAX_BAG_OPEN_NUM; ++n) {
		CGoods* pGood = rGoodMap.GetDataByIndex(n);
		if (NULL != pGood) {
			SGoodsProp *pGoodsProp = pGood->GetGoodsProp();
			if (NULL == pGoodsProp) {
				continue;
			}

			if (EGMK_SOUL_METERIAL == pGoodsProp->byMainKindID) {
				pGood->GetDT_GOODS_DATA_CLI(pMeterialInfo[rMeterialNum]);
				++ rMeterialNum;
			}
		}
	}
}

VOID CSoulManager::FillSoulRepInfo(CPlayer* pPlayer, DT_SOUL_REP_INFO& rSoulRepData) {
	if (NULL == pPlayer) {
		return;
	}

	pPlayer->GetSyncResource(rSoulRepData.stResInfo);
	CPlayerSoul* pPlayerSoul = pPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return;
	}
	DT_PLAYER_SOUL_DATA& rSoulData = pPlayerSoul->GetDT_PLAYER_SOUL_DATA();

	for (size_t n = 0; n < MAX_SOUL_PRODUCT_TYPE; ++n) {
		if (!IsToday(rSoulData.qwLastTime)) {
			rSoulData.abyTodayCoinCnt[n] = 0;
		}

		rSoulRepData.astNormalCost[n].byCostType = CLuamgr::Instance()->GetProcuctSoulCostType(n, 0, rSoulData.abyTodayCoinCnt[n]);
		rSoulRepData.astContinusCost[n].byCostType = CLuamgr::Instance()->GetProcuctSoulCostType(n, 1, rSoulData.abyTodayCoinCnt[n]);
		rSoulRepData.astNormalCost[n].dwCostNum = CLuamgr::Instance()->GetProcuctSoulCostNum(n, 0, rSoulData.abyTodayCoinCnt[n]);
		rSoulRepData.astContinusCost[n].dwCostNum = CLuamgr::Instance()->GetProcuctSoulCostNum(n, 1, rSoulData.abyTodayCoinCnt[n]);

		const SoulProductCfg* pSoulProducfCfg = CSoulPropMgr::Instance()->GetSoulProductCfg(n);
		if (NULL != pSoulProducfCfg) {
			for (size_t m = 0; m < pSoulProducfCfg->vPossibleStar.size(); ++m) {
				UINT8 ucStar = pSoulProducfCfg->vPossibleStar[m];
				if (0 == m) {
					rSoulRepData.abyMinStar[n] = rSoulRepData.abyMaxStar[n] = ucStar;
				} else {
					if (rSoulRepData.abyMinStar[n] > ucStar) {
						rSoulRepData.abyMinStar[n] = ucStar;
					}

					if (rSoulRepData.abyMaxStar[n] < ucStar) {
						rSoulRepData.abyMaxStar[n] = ucStar;
					}
				}
			}
		}
	}
}

BOOL CSoulManager::IsSoulRepActivity(CPlayer* pPlayer) {
	return COprateingActivityMgr::Instance()->IsOpenSoulAty(pPlayer);
}

UINT8 CSoulManager::GetSoulBoxType(UINT8 ucStar) {
	if (ucStar <= 2) {
		return eBT_CopperBox;
	}

	if (ucStar >= 3 && ucStar <= 4) {
		return eBT_SliverBox;
	}

	return eBT_GoldBox;
}

CSoul* CSoulManager::GenerateSoulById(UINT32 unSoulId) {
	const SoulBaseCfg* pBaseSoulCfg = CSoulPropMgr::Instance()->GetSoulBaseCfg(unSoulId);
	if (NULL == pBaseSoulCfg) {
		PRINT_ERR_INFO;
		return NULL;
	}

	const SoulLvCfg* pSoulLv = CSoulPropMgr::Instance()->GetSoulLvCfg(pBaseSoulCfg->ucStar, 1);
	if (NULL == pSoulLv) {
		PRINT_ERR_INFO;
		return NULL;
	}

	static CSoul soul;
	soul.Clear();
	DT_SOUL_DATA& rSoulData = soul.GetDT_SOUL_DATA();
	rSoulData.wSoulID = pBaseSoulCfg->unSoulId;
	rSoulData.bySoulStar = pBaseSoulCfg->ucStar;
	rSoulData.bySoulType = pBaseSoulCfg->ucSoulType;
	rSoulData.dwSoulIdx = 0;
	rSoulData.bySoulLevel = 1;
	rSoulData.dwSoulExperience = 0;
	rSoulData.byIsNormalSoul = pBaseSoulCfg->ucSoulType > 0 ? eNormalSoul : eDevourSoul;
	rSoulData.dwDevourExp = pBaseSoulCfg->unDevourExp;

	rSoulData.byHaveEffect = Random(0, 100) <= pBaseSoulCfg->ucEffectOdd;

	if (eNormalSoul == rSoulData.byIsNormalSoul) {
		//对属性个数进行随机
		const CRandomMap* poRandom = CSoulPropMgr::Instance()->GetAttrNumRandom(rSoulData.bySoulStar, rSoulData.byBaseNum);
		if (NULL != poRandom) {
			rSoulData.byBaseNum = GetRandomIdxMustHit(*poRandom);
		} else {
			rSoulData.byBaseNum = 1;
		}
		rSoulData.byBaseNum = rSoulData.byBaseNum == 0 ? 1 : rSoulData.byBaseNum;
		FillSoulAttrData(rSoulData.bySoulStar, rSoulData.bySoulLevel, rSoulData.byBaseNum, rSoulData.astSoulAttr);
	}

	if ( rSoulData.byHaveEffect )
	{
		CSpecialEffectPropMgr::Instance()->GenerateSpecialEffectData(pBaseSoulCfg->unEffectId, rSoulData.stSpecialEffect);
	}

	//生成特殊攻击
	//CSpecialEffectPropMgr::Instance()->GenerateSpecialEffectData(rSoulData.bySoulType, rSoulData.bySoulStar, rSoulData.stSpecialEffect);
	return &soul;
}

UINT16 CSoulManager::OpenSoulPotencyInfo(CPlayer* poPlayer, PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ& stReq, PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK& stAck)
{
	if (NULL == poPlayer) 
	{
		return ERR_OPEN_SOUL_UPDATE_TYPE::ID_IS_NOT_OPEN;
	}

	CPlayerSoul* pPlayerSoul = poPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) 
	{
		ERR_OPEN_SOUL_UPDATE_TYPE::ID_IS_NOT_OPEN;
	}

	CSoul* pSoul = GetSoul(poPlayer, stReq.stSoulMark);
	if (NULL == pSoul) 
	{
		return ERR_OPEN_SOUL_UPDATE_TYPE::ID_NOT_EXIST_SOUL;
	}

	pSoul->GetSoulPotencyCli(stAck.stPotencyInfo);
	pSoul->GetSoulPotencyDetailCli(stAck.stAllPotency);
	pSoul->GetSoulPotencyAddAttrInfo(stAck.stCountInfo);

	
	return ERR_OPEN_SOUL_UPDATE_TYPE::ID_SUCCESS;
}

UINT16 CSoulManager::SoulPotencyInfo(CPlayer* poPlayer, PKT_CLIGS_SOUL_UPDATE_TYPE_REQ& stReq, PKT_CLIGS_SOUL_UPDATE_TYPE_ACK& stAck)
{
	if (NULL == poPlayer) 
	{
		return ERR_SOUL_UPDATE_TYPE::ID_IS_NOT_OPEN;
	}

	CPlayerSoul* pPlayerSoul = poPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) 
	{
		return ERR_SOUL_UPDATE_TYPE::ID_IS_NOT_OPEN;
	}

	CSoul* pSoul = GetSoul(poPlayer, stReq.stSoulMark);
	if (NULL == pSoul) 
	{
		return ERR_SOUL_UPDATE_TYPE::ID_NOT_EXIST_SOUL;
	}

	stAck.stPowerChangeInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	//提升潜能
	eSoulUpdatePotency eRet = pSoul->UpdatePotency( stAck.stItemsInfo );
	if ( ESP_ITEM_NOT_ENOUGH == eRet )
	{
		return ERR_SOUL_UPDATE_TYPE::ID_ITEM_NOT_ENOUGH;
	}
	else if ( ESP_MAX_LEVEL == eRet)
	{
		return ERR_SOUL_UPDATE_TYPE::ID_MAX_POTENCY;
	}
	else if ( ESP_LEVEL_NOT_ENOUGH == eRet)
	{
		return ERR_SOUL_UPDATE_TYPE::ID_LEVEL_NOT_ENOUGH;
	}
	UpdateSoulData(poPlayer, stReq.stSoulMark, pSoul);
	poPlayer->ResetBattleAttr(TRUE);
	stAck.stPowerChangeInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

	pSoul->GetSoulPotencyCli( stAck.stPotencyInfo);
	pSoul->GetSoulPotencyAddAttrInfo(stAck.stCountInfo);


	return ERR_SOUL_UPDATE_TYPE::ID_SUCCESS;
}