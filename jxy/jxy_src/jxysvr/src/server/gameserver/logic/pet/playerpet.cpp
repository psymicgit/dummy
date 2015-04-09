#include "playerpet.h"
#include "pet.h"
#include "petobjmng.h"
#include "petdef.h"
#include "sdloggerimpl.h"
#include "logic/player/player.h"
#include "logic/pet/petpropmng.h"
#include "logic/pet/neidan.h"
#include "logic/hero/hero.h"

IMPLEMENT_CLASS_POOL_MGR(CPlayerPet);

CPlayerPet::CPlayerPet() {
	memset(&m_PlayerPetData, 0, sizeof(m_PlayerPetData));
}

CPlayerPet::~CPlayerPet() {
	for (MapPet::iterator itor = m_mpPet.begin(), end = m_mpPet.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			REMOVE_PET_OBJ(m_poOwner->GetPlayerID(), itor->second->GetDT_PET_DATA().wPetId);
		}
	}
	m_mpPet.clear();

	for ( map<UINT32, CPetNeiDan*>::iterator itr = m_mapNeiDanBag.begin(); itr != m_mapNeiDanBag.end(); itr++)
	{
		CPetNeiDanPoolMgr::Instance()->Remove(itr->second);
	}
	m_mapNeiDanBag.clear();
}

BOOL CPlayerPet::Init(const DT_PLAYER_PET_DATA& rPetData, CPlayer *pPlayer) {
	if (NULL == pPlayer) {
		return FALSE;
	}

	m_poOwner = pPlayer;
	m_PlayerPetData = rPetData;
	m_PlayerPetData.stPetNeiDan.wPetNum = m_PlayerPetData.stPetDataLst.wPetNum;
	const DT_PET_DATA_LIST& rPetDataLst = rPetData.stPetDataLst;
	for (size_t n = 0; n < rPetDataLst.wPetNum && n < MAX_PET_NUM; ++n) 
	{
		const DT_PET_DATA& rPetData = rPetDataLst.astPetData[n];
		CPet* pPet = CREATE_PET_OBJ(pPlayer->GetPlayerID(), rPetData.wPetId);
		if (NULL == pPet) {
			return FALSE;
		}
		pPet->Init(rPetData, pPlayer, m_PlayerPetData.stPetNeiDan.astNeiDanInfo[n]);
		m_mpPet.insert(make_pair(rPetData.wPetId, pPet));
	}

	m_PlayerPetData.byHaveDataFlag = 1;
	for ( UINT16 wIdx = 0; wIdx < rPetData.stPetCallData.wPetNum; wIdx++)
	{
		m_mapCallInfo[rPetData.stPetCallData.astCallInfo[wIdx].wPetCfgID] =  rPetData.stPetCallData.astCallInfo[wIdx];
	}

    CheckCrossDay();
	return TRUE;
}

void CPlayerPet::CheckCrossDay()
{
    if (!IsToday(m_PlayerPetData.qwProductLastTime)){
        m_PlayerPetData.wNormalProductNum = 0;
        m_PlayerPetData.wGoodProductNum = 0;
        m_PlayerPetData.wFreeNormalProductCnt = CPetPropMng::Instance()->GetPetSysCfg().usNormalFreeProductCnt;
    }
}

DT_PLAYER_PET_DATA& CPlayerPet::GetDT_PLAYER_PET_DATA() {
	UINT16 wIdx = 0;
	for(map<UINT16, DT_PET_CALL_DATA>::iterator itr = m_mapCallInfo.begin(); itr != m_mapCallInfo.end(); itr++, wIdx++)
	{
		m_PlayerPetData.stPetCallData.astCallInfo[wIdx] = itr->second;
	}
	m_PlayerPetData.stPetCallData.wPetNum = wIdx;
	return m_PlayerPetData;
}

CPet* CPlayerPet::GetPet(UINT16 usPetId) {
	MapPet::iterator itor = m_mpPet.find(usPetId);
	if (m_mpPet.end() == itor) {
		return NULL;
	}
	return itor->second;
}

const CPet* CPlayerPet::AddPet(const DT_PET_DATA& rPetData) {
	if (m_mpPet.size() >= MAX_PET_NUM) {
		return NULL;
	}

	UINT16 usPetId = generatePetId();
	CPet* pNewPet = CREATE_PET_OBJ(m_poOwner->GetPlayerID(), usPetId);
	if (NULL == pNewPet) {
		return NULL;
	}

	DT_PET_NEIDAN_DATA_LST stNeiDan = {0};
	pNewPet->Init(rPetData, m_poOwner, stNeiDan);
	pNewPet->SetPetId(usPetId);

	m_mpPet.insert(make_pair(usPetId, pNewPet));
	UpdatePetLstData();
	map<UINT16, DT_PET_CALL_DATA>::iterator itr = m_mapCallInfo.find(rPetData.wPetCfgId);
	if ( itr == m_mapCallInfo.end())
	{
		DT_PET_CALL_DATA stData = {0};
		stData.wPetCfgID = rPetData.wPetCfgId;
		stData.dwPetCallNum = 1;
		m_mapCallInfo[stData.wPetCfgID] = stData;
		SetRuleState(TRUE);
	}
	else
	{
		itr->second.dwPetCallNum++;
	}
	return pNewPet;
}

BOOL CPlayerPet::RemovePet(UINT16 usPetId) {
	MapPet::iterator itor = m_mpPet.find(usPetId);
	if (m_mpPet.end() == itor) {
		return FALSE;
	}

	m_mpPet.erase(itor);
	REMOVE_PET_OBJ(m_poOwner->GetPlayerID(), usPetId);
	UpdatePetLstData();
	return TRUE;
}

UINT16 CPlayerPet::generatePetId() {
	UINT16 usRetVal = 1;
	for (; usRetVal < 0xFFFF; ++ usRetVal) {
		if (m_mpPet.end() == m_mpPet.find(usRetVal)) {
			return usRetVal;
		}
	}

	return usRetVal;
}

UINT16 CPlayerPet::GenerateNeiDanId(UINT16 wNeiDanID)
{
	UINT16 wNeiDanIdx = m_mapNeiDanID[wNeiDanID];
	m_mapNeiDanID[wNeiDanID]++;
	return wNeiDanIdx;
}

BOOL CPlayerPet::HaveCall(UINT16 wCfgID)
{
	map<UINT16, DT_PET_CALL_DATA>::iterator itr = m_mapCallInfo.find(wCfgID);
	if ( itr == m_mapCallInfo.end())
	{
		return FALSE;
	}
	return TRUE;
}

VOID CPlayerPet::SetRuleState(BOOL bState)
{
	m_PlayerPetData.byCallFlag = bState;
}

VOID CPlayerPet::UpdatePetLstData() {
	DT_PET_DATA_LIST& rPetDataLst = m_PlayerPetData.stPetDataLst;
	memset(&rPetDataLst, 0, sizeof(rPetDataLst));

	rPetDataLst.wPetNum = m_mpPet.size();
	size_t n = 0;
	for (MapPet::iterator itor = m_mpPet.begin(), end = m_mpPet.end()
		; end != itor && n < MAX_PET_NUM
		; ++ itor, ++ n)
	{
		rPetDataLst.astPetData[n] = itor->second->GetDT_PET_DATA();
	}
}

const CPet* CPlayerPet::GetFightPet() const {
	size_t n = 0;
	for (MapPet::const_iterator itor = m_mpPet.begin(), end = m_mpPet.end()
		; end != itor && n < MAX_PET_NUM
		; ++ itor, ++ n)
	{
		if (ePS_FIGHT == itor->second->GetDT_PET_DATA().byPetState) {
			return itor->second;
		}
	}

	return NULL;
}

vector<const CPet*> CPlayerPet::GetFollowPet() const {
	size_t n = 0;
	vector<const CPet*> vPet;
	for (MapPet::const_iterator itor = m_mpPet.begin(), end = m_mpPet.end()
		; end != itor && n < MAX_PET_NUM
		; ++ itor, ++ n)
	{
		if (ePS_FOLLOW == itor->second->GetDT_PET_DATA().byPetState) {
			vPet.push_back(itor->second);
		}
	}
	return vPet;
}

CPet* CPlayerPet::CreateTestPet()
{
	
	DT_PET_DATA stData;
	memset(&stData, 0, sizeof(DT_PET_DATA));
	stData.wPetId = 0;
	stData.byStarLv = 1;
	stData.wLv = 1;
	stData.byPetState = ePS_FIGHT;
	stData.wPetCfgId = 42;
	AddPet(stData);
	return NULL;
}

VOID CPlayerPet::GetNeiDanByBag(DT_BAG_NEIDAN_CLI& stBagNeiDan)
{
	UINT8 byNum = 0;
	for (map<UINT32, CPetNeiDan*>::iterator itr = m_mapNeiDanBag.begin(); itr != m_mapNeiDanBag.end(); itr++, byNum++)
	{
		itr->second->GetNeiDanCli(stBagNeiDan.astNeiDanInfo[byNum]);
	}
	stBagNeiDan.byNum = byNum;
}

CPetNeiDan* CPlayerPet::GetNeiDanByBag(UINT16 wID, UINT16 wIdx)
{
	map<UINT32, CPetNeiDan*>::iterator itr = m_mapNeiDanBag.find(CLogicFunc::Get1616Key(wID, wIdx));
	if ( itr == m_mapNeiDanBag.end())
	{
		return NULL;
	}
	return itr->second;
}

VOID CPlayerPet::GetNeiDanMarkByBag(DT_NEIDAN_BAG_MARK_LST_DATA& stBagNeiDan)
{
	UINT8 byNum = 0;
	for (map<UINT32, CPetNeiDan*>::iterator itr = m_mapNeiDanBag.begin(); itr != m_mapNeiDanBag.end(); itr++, byNum++)
	{
		itr->second->GetNeiDanMark(stBagNeiDan.astNeiDanInfo[byNum]);
	}
	stBagNeiDan.byNum = byNum;
}

CPetNeiDan* CPlayerPet::GetNeiDan(UINT16 wID, UINT16 wIdx)
{
	map<UINT32, CPetNeiDan*>::iterator itr = m_mapAllNeiDan.find(CLogicFunc::Get1616Key(wID, wIdx));
	if ( itr == m_mapAllNeiDan.end())
	{
		return NULL;
	}
	return itr->second;
}

BOOL CPlayerPet::AddNeiDanToBag(CPetNeiDan* poNeiDan)
{
	if ( NULL == poNeiDan)
	{
		return FALSE;
	}

	if ( MAX_NEI_DAN_BAG_NUM <= m_mapNeiDanBag.size())
	{
		return FALSE;
	}

	m_mapNeiDanBag[CLogicFunc::Get1616Key(poNeiDan->GetDT_NEIDAN_DATA().wNeiDanID, poNeiDan->GetDT_NEIDAN_DATA().wNeiDanIdx)] = poNeiDan;
	
	return TRUE;
}

BOOL CPlayerPet::DelNeiDanByBag(CPetNeiDan* poNeiDan, BOOL bIsRemove )
{
	if ( NULL == poNeiDan)
	{
		return FALSE;
	}

	map<UINT32, CPetNeiDan*>::iterator itr = m_mapNeiDanBag.find(CLogicFunc::Get1616Key(poNeiDan->GetDT_NEIDAN_DATA().wNeiDanID, poNeiDan->GetDT_NEIDAN_DATA().wNeiDanIdx));
	if ( itr == m_mapNeiDanBag.end())
	{
		return FALSE;
	}

	m_mapNeiDanBag.erase(CLogicFunc::Get1616Key(poNeiDan->GetDT_NEIDAN_DATA().wNeiDanID, poNeiDan->GetDT_NEIDAN_DATA().wNeiDanIdx));

	if ( bIsRemove )
	{
		CPetNeiDanPoolMgr::Instance()->Remove(poNeiDan);
	}

	return TRUE;
}

BOOL CPlayerPet::DressNeiDanToPet(CPet* poPet, CPetNeiDan* poNeiDan)
{
	if ( NULL == poPet || NULL == poNeiDan)
	{
		return FALSE;
	}

	SPetNeiDanProp* poProp = poNeiDan->GetBaseProp();
	if ( NULL == poProp)
	{
		return FALSE;
	}

	CPetNeiDan* poUnDress = poPet->UnDressNeiDan( poProp->byWuXing );
	poPet->DressNeiDan( poNeiDan );



	return TRUE;

}

BOOL CPlayerPet::UnDressNeiDanByPet(CPet* poPet, CPetNeiDan* poNeiDan)
{
	if ( NULL == poPet || NULL == poNeiDan)
	{
		return FALSE;
	}

	return TRUE;
}

VOID CPlayerPet::ResetNeiDanHeroHP()
{
	memset(&m_PlayerPetData.stHeroLstInfo, 0, sizeof(m_PlayerPetData.stHeroLstInfo));
// 	CFormationHeroShmemMap& mapHero = m_poOwner->GetFormationHeroMap();
// 	CHero* poHero = mapHero.GetFistData();
// 	UINT8 byHeroNum = 0;
// 	while(poHero && byHeroNum < MAX_FORMATION_IDX_NUM)
// 	{
// 		DT_HERO_HP_DATA& stHPData = m_PlayerPetData.stHeroLstInfo.astHeroHPLst[byHeroNum];
// 		stHPData.wHeroID = poHero->GetHeroID();
// 		stHPData.dwHeroHP = (UINT32)poHero->GetDT_BATTLE_ATTRIBUTE().qwHP;
// 		stHPData.byFormationIdx = poHero->GetFormationIdx();
// 		poHero = mapHero.GetNextData();
// 		byHeroNum++;
// 	}
// 	m_PlayerPetData.stHeroLstInfo.byHeroNum = byHeroNum;
}

VOID CPlayerPet::SetInitHeroHP(DT_BATTLE_OBJ_GROUP_HP& stInitMyself)
{
	UINT8 byHeroNum = 0;
	CFormationHeroShmemMap& mapHero = m_poOwner->GetFormationHeroMap();
	for ( UINT8 byIdx = 0; byIdx < m_PlayerPetData.stHeroLstInfo.byHeroNum; byIdx++)
	{
		DT_HERO_HP_DATA& stHPData = m_PlayerPetData.stHeroLstInfo.astHeroHPLst[byIdx];
		CHero* poHero = mapHero.GetData(stHPData.byFormationIdx);
		//保证阵型不变
		if ( (poHero) && (stHPData.wHeroID == poHero->GetHeroID()))
		{
			stInitMyself.astBattleObjInfo[byHeroNum].byFormationIdx = stHPData.byFormationIdx;
			stInitMyself.astBattleObjInfo[byHeroNum].dwHP = stHPData.dwHeroHP;
			byHeroNum++;
		}
	}
	m_PlayerPetData.stHeroLstInfo.byHeroNum = byHeroNum;
}

VOID CPlayerPet::SetAfterHeroHP(DT_BATTLE_OBJ_GROUP_HP& stAfterHeroHP)
{
	memset(&m_PlayerPetData.stHeroLstInfo, 0, sizeof(m_PlayerPetData.stHeroLstInfo));
	UINT8 byHeroNum = 0;
	CFormationHeroShmemMap& mapHero = m_poOwner->GetFormationHeroMap();
	for ( ; byHeroNum < stAfterHeroHP.byObjNum; byHeroNum++)
	{
		DT_HERO_HP_DATA& stHPData = m_PlayerPetData.stHeroLstInfo.astHeroHPLst[byHeroNum];
		stHPData.byFormationIdx = stAfterHeroHP.astBattleObjInfo[byHeroNum].byFormationIdx;
		stHPData.dwHeroHP = stAfterHeroHP.astBattleObjInfo[byHeroNum].dwHP;
		CHero* poHero = mapHero.GetData(stHPData.byFormationIdx);
		if ( NULL == poHero)
		{
			stHPData.wHeroID = poHero->GetHeroID();
		}
	}
	m_PlayerPetData.stHeroLstInfo.byHeroNum = byHeroNum;
}