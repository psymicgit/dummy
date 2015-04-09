#include "dealframesoul.h"
#include "logic/player/player.h"
#include "logic/soul/playersoul.h"
#include "logic/soul/soulmanager.h"
#include "logic/soul/soul.h"
#include <sstream>

using namespace SGDP;

CDealframeSoul::~CDealframeSoul() {

}

string CDealframeSoul::GetFrameType() {
	return "soul";
}

BOOL CDealframeSoul::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType 
	, UINT32 dwNum1 /*= 0*/, UINT32 dwNum2 /*= 0*/, UINT32 dwNum3 /*= 0 */
	, TCHAR *pszDesc /*= NULL*/, DT_PLAYER_RES_DATA* pstResInfo1 /*= NULL*/, DT_PLAYER_RES_DATA* pstResInfo2 /*= NULL*/) 
{
	if (NULL == poPlayer) {
		return FALSE;
	}

	if (vecOpValue.size() < 2) {
		return FALSE;
	}

	CPlayerSoul* pPlayerSoul = poPlayer->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return FALSE;
	}

	UINT32 unSoulNum = 1;
	UINT32 unSoulId = SDAtou(vecOpValue[1].c_str());
	if (vecOpValue.size() > 2) {
		 unSoulNum = SDAtou(vecOpValue[2].c_str());
	}

	vector<UINT8> vKindId;
	for (size_t n = 3; n < vecOpValue.size(); ++n) {
		vKindId.push_back(SDAtou(vecOpValue[n].c_str()));
	}


	DT_SOUL_LST_DATA& rSoulData = pPlayerSoul->GetDT_PLAYER_SOUL_DATA().stSoulBagData;
	 
	switch (byOpType) {
	case RESOPTYPE_CKNUM: {
		UINT32 unSoulLv = unSoulNum;
		for (size_t n = 0; n < rSoulData.wSoulNum; ++ n) {
			if (unSoulId == rSoulData.astSoulLstData[n].wSoulID && unSoulLv == rSoulData.astSoulLstData[n].bySoulLevel) {
				return TRUE;
			}
		}
		return FALSE;
	}
		break;
	case RESOPTYPE_DECNUM: {
		UINT32 unSoulLv = unSoulNum;
		vector<CSoul*> vSoul;
		for (size_t n = 0; n < rSoulData.wSoulNum; ++ n) {
			if (unSoulId == rSoulData.astSoulLstData[n].wSoulID && unSoulLv == rSoulData.astSoulLstData[n].bySoulLevel) {
				CSoul* pSoul = pPlayerSoul->GetSoul(unSoulId, rSoulData.astSoulLstData[n].dwSoulIdx);
				vSoul.push_back(pSoul);
				break;
			}
		}
		for (size_t n = 0; n < vSoul.size(); ++ n) {
			CRecordMgr::Instance()->RecordInfo(poPlayer->GetID(), ERM_DECSOUL, 2
				, 1, 1
				, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
				, vSoul[n]->GetDT_SOUL_DATA().wSoulID, 0, 0, 0);

			pPlayerSoul->DelSoulFromBag(vSoul[n]);
		}
		return TRUE;
	}
		break;
	case RESOPTYPE_ADDNUM: {
		if (pPlayerSoul->GetSoulBagLeftSize() >= unSoulNum) {
			CSoul* pSoul = CSoulManager::Instance()->GenerateSoulById(unSoulId);
			if (NULL == pSoul) {
				return FALSE;
			}
			DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
			
			if (rSoulData.byBaseNum < vKindId.size()) {
				rSoulData.byBaseNum = rSoulData.bySoulStar > vKindId.size() ? vKindId.size() : rSoulData.bySoulStar;
			}

			C8Set lockKindId;
			for (size_t n = 0; n < rSoulData.byBaseNum && n < vKindId.size(); ++n) {
				rSoulData.astSoulAttr[n].byKindID = vKindId[n];
				lockKindId.insert(n);
			}

			CSoulManager::Instance()->FillSoulAttrData(rSoulData.bySoulStar, rSoulData.bySoulLevel
				, rSoulData.byBaseNum, rSoulData.astSoulAttr, lockKindId);
			for (size_t n = 0; n < unSoulNum; ++ n) {
				pPlayerSoul->AddSoulToBag(pSoul);

				ostringstream oss;
				for (size_t n = 0; n < rSoulData.byBaseNum; ++n) {
					oss << (UINT32)(rSoulData.astSoulAttr[n].byKindID) << 
						"," << rSoulData.astSoulAttr[n].dwKindValue <<
						"," << (UINT32)(rSoulData.astSoulAttr[n].byKindPrecent) << ";";
				}

				CRecordMgr::Instance()->RecordInfo(poPlayer->GetID(), ERM_ADDSOUL, CRecordMgr::EAST_GM
					, 1, 1
					, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
					, pSoul->GetDT_SOUL_DATA().wSoulID, pSoul->GetDT_SOUL_DATA().stSpecialEffect.wEffectID, 0, 0, oss.str());
			}
			return TRUE;
		}
	}
		break;
	case RESOPTYPE_CK_ADD_LIMIT: {
			return pPlayerSoul->GetSoulBagLeftSize() >= unSoulNum;
	}
		break;
	default:
		break;
	}

	return FALSE;
}
