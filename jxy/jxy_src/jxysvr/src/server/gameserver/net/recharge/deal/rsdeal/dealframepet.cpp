#include "dealframepet.h"
#include "logic/player/player.h"
#include "logic/pet/playerpet.h"
#include "logic/pet/petmng.h"
#include "logic/pet/pet.h"

#include <sstream>
#include "logic/pet/petpropmng.h"

using namespace SGDP;


CDealframePet::~CDealframePet() {

}

string CDealframePet::GetFrameType() {
	return "pet";
}

BOOL CDealframePet::Deal(CPlayer* pPlayer, vector<string> vecOpValue, UINT8 byOpType 
	, UINT32 dwNum1 /*= 0*/, UINT32 dwNum2 /*= 0*/, UINT32 dwNum3 /*= 0 */, TCHAR *pszDesc /*= NULL*/
	, DT_PLAYER_RES_DATA* pstResInfo1 /*= NULL*/, DT_PLAYER_RES_DATA* pstResInfo2 /*= NULL*/) 
{
	if (NULL == pPlayer) {
		return FALSE;
	}

	if (vecOpValue.size() < 2) {
		return FALSE;
	}

	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return FALSE;
	}

	UINT32 unPetNum = 1;
	UINT32 unPetCfgId = SDAtou(vecOpValue[1].c_str());
	if (vecOpValue.size() > 2) {
		unPetNum = SDAtou(vecOpValue[2].c_str());
	}

	switch (byOpType) {
	case RESOPTYPE_CKNUM: {
		return pPlayerPet->GetPetBagIdleSize() >= unPetNum;
	}
		break;
	case RESOPTYPE_DECNUM: {
		return FALSE;
	}
		break;
	case RESOPTYPE_ADDNUM: {
		if (pPlayerPet->GetPetBagIdleSize() >= unPetNum) {
			const PetBaseCfg* pPetBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg((unsigned short)unPetCfgId);
			if (NULL == pPetBaseCfg) {
				return FALSE;
			}
			for (size_t n = 0; n < unPetNum; ++n) {
				const CPet* pNewPet = CPetMng::Instance()->CreateAndAddPet(pPlayer, pPetBaseCfg, 1);

				CRecordMgr::Instance()->RecordInfo(pPlayer->GetID(), ERM_GM_ADDPET, 1
					, 1, 1
					, pPlayer->GetLevel(), pPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel
					, pNewPet->GetDT_PET_DATA().wPetId, pNewPet->GetDT_PET_DATA().wPetCfgId, 0, 0, "");
			}
		}
	}
		break;
	case RESOPTYPE_CK_ADD_LIMIT: {
		return pPlayerPet->GetPetBagIdleSize() >= unPetNum;
	}
		break;
	default:
		break;
	}

	return FALSE;
}
