#include "pet.h"
#include "petpropmng.h"
#include "logic/skill/skillpropmgr.h"
#include "logic/player/player.h"
#include "logic/pet/neidan.h"

CPet::CPet() 
{
	memset(&m_PetData, 0, sizeof(m_PetData));
	memset(&m_stNeiDanData, 0, sizeof(m_stNeiDanData));
}

CPet::~CPet() {
	for ( vector<CPetSkill*>::iterator itr = m_vecSkill.begin(); itr != m_vecSkill.end(); itr++)
	{
		CPetSkillPoolMgr::Instance()->Remove(*itr);
	}

	for ( map<UINT8, CPetNeiDan*>::iterator itr = m_mapNeiDan.begin(); itr != m_mapNeiDan.end(); itr++)
	{
		CPetNeiDanPoolMgr::Instance()->Remove(itr->second);
	}
	m_mapNeiDan.clear();

}

BOOL CPet::Init(const DT_PET_DATA& rPetData, CPlayer* pPlayer, const DT_PET_NEIDAN_DATA_LST& stNeiDanData) 
{
	if ( NULL == pPlayer)
	{
		return FALSE;
	}
	m_poOwner = pPlayer;
	m_PetData = rPetData;
	m_stNeiDanData = stNeiDanData;
	InitSkill();
	return TRUE;
}

DT_PET_DATA& CPet::GetDT_PET_DATA() {
	return m_PetData;
}

const DT_PET_DATA& CPet::GetDT_PET_DATA() const {
	return m_PetData;
}

void CPet::FillDT_PET_CLI_DATA(DT_PET_CLI_DATA& rCliPetData) const {
	rCliPetData.wPetCfgID = m_PetData.wPetCfgId;
	rCliPetData.wPetId = m_PetData.wPetId;
	rCliPetData.byStarLv = m_PetData.byStarLv;
	rCliPetData.wLv = m_PetData.wLv;
	memcpy(rCliPetData.aszPetName, m_PetData.aszPetName, sizeof(rCliPetData.aszPetName));
 	rCliPetData.dwMaxPhychicVal = GetMaxPsychic();
	rCliPetData.dwCurLingli = GetLingli();
	rCliPetData.dwBeAteExp = GetBeAteExp();
	rCliPetData.byPetState = m_PetData.byPetState;
	memcpy(rCliPetData.astWashQualityData, m_PetData.astWashQualityData, sizeof(rCliPetData.astWashQualityData));
	rCliPetData.dwCurPhychicVal = GetCurPsychic();
	
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(m_PetData.wPetCfgId);
	if (NULL == pBaseCfg) { 
		return;
	}

	rCliPetData.byPetQuality = pBaseCfg->ucPetQuality;
	rCliPetData.byPetColor = pBaseCfg->ucPetColor;
	rCliPetData.byPetType = pBaseCfg->ucPetType;
	memcpy(rCliPetData.aszPetPic, pBaseCfg->strPetPic.c_str(), sizeof(rCliPetData.aszPetPic));

	const PetStarCfg* pStarCfg = CPetPropMng::Instance()->GetPetStarCfg(pBaseCfg->ucPetType
															, pBaseCfg->ucPetQuality
															, m_PetData.byStarLv);
	if (NULL != pStarCfg) {
		rCliPetData.wMaxLv = pStarCfg->usPetMaxLv;
	}

	const PetLvCfg* pLvCfg = CPetPropMng::Instance()->GetPetLvCfg(m_PetData.wLv);
	if (NULL != pLvCfg) {
		rCliPetData.dwLvExp = m_PetData.dwLvExp - pLvCfg->unExp;
	}
	else
	{
		USR_INFO("[%s:%u] Lv = %u, failed", MSG_MARK, m_PetData.wLv);
		return;
	}

	const PetLvCfg* pNextLvCfg = CPetPropMng::Instance()->GetPetLvCfg(m_PetData.wLv + 1);
	if (NULL != pNextLvCfg) {
		rCliPetData.dwNextLvExp = pNextLvCfg->unExp - pLvCfg->unExp;
	}
	
	const PetStarCfg* pNextStarCfg = CPetPropMng::Instance()->GetPetStarCfg(pBaseCfg->ucPetType
														, pBaseCfg->ucPetQuality
														, 1 + m_PetData.byStarLv);

	if (NULL != pNextStarCfg
		&& m_PetData.wLv >= pStarCfg->usPetMaxLv
		&& m_poOwner->GetLevel() >= pNextStarCfg->ucPlayerLvLimit) 
	{
		rCliPetData.byIsUpStar = 1;
	} else {
		rCliPetData.byIsUpStar = 0;
	}
	UINT8 bySkill = 0;
	for ( vector<CPetSkill*>::const_iterator itr = m_vecSkill.begin(); itr != m_vecSkill.end(); itr++, bySkill++)
	{
		CPetPropMng::Instance()->GetPetSkillCli((*itr)->GetID(), GetLevel(), rCliPetData.stSkillInfo.astSkillInfo[bySkill]);
		rCliPetData.stSkillInfo.astSkillInfo[bySkill].byBeginRound = bySkill + 1;
	}
	rCliPetData.stSkillInfo.bySkillNum = bySkill;

	//宠物评分
	rCliPetData.qwScore = GetPetScore();
}

UINT32 CPet::GetBeAteExp() const {
	UINT32 unRetVal = m_PetData.dwLvExp;
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(m_PetData.wPetCfgId);
	if (NULL != pBaseCfg) { 
		unRetVal += pBaseCfg->unDevourExp;
	}

	return unRetVal;
}

VOID CPet::SetPetId(UINT16 usPetId) {
	m_PetData.wPetId = usPetId;
}

UINT8 CPet::GetFormationIdx() {
	return 1;
}

VOID CPet::InitBattleAttribute() {

}


VOID CPet::InitSkill()
{
	const PetBaseCfg* poBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg( m_PetData.wPetCfgId);
	if ( NULL == poBaseCfg)
	{
		return ;
	}
	for ( vector<UINT16>::const_iterator itr = poBaseCfg->vecSkill.begin(); itr != poBaseCfg->vecSkill.end(); itr++)
	{
		CPetSkill* poPetSkill = CPetSkillPoolMgr::Instance()->Create();
		if ( NULL == poPetSkill)
		{
			continue;
		}
		if(!poPetSkill->Init( *itr, m_poOwner, this))
		{
			CPetSkillPoolMgr::Instance()->Remove(poPetSkill);
			continue;
		}
		m_vecSkill.push_back(poPetSkill);
	}
}

VOID CPet::AddPetExp(UINT32 unAddPetExp, BOOL bUpStarLv/* = FALSE*/) {
	DT_PET_DATA& rPetData = GetDT_PET_DATA();
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(rPetData.wPetCfgId);
	if (NULL == pBaseCfg) {
		return;
	}

	while (TRUE) {
		const PetStarCfg* pStarCfg = CPetPropMng::Instance()->GetPetStarCfg(pBaseCfg->ucPetType
																, pBaseCfg->ucPetQuality
																, rPetData.byStarLv);
		if (NULL == pStarCfg) {
			return;
		}

		if (0 == unAddPetExp
			|| (!bUpStarLv && rPetData.wLv == pStarCfg->usPetMaxLv)) // 到达当前最大等级
		{
			break;
		}

		const PetLvCfg* pLvCfg = CPetPropMng::Instance()->GetPetLvCfg(rPetData.wLv + 1);
		if (NULL == pLvCfg) {
			break;
		}

		if ((rPetData.dwLvExp + unAddPetExp) >= pLvCfg->unExp) { //升级
			++ rPetData.wLv;
			unAddPetExp = rPetData.dwLvExp + unAddPetExp - pLvCfg->unExp;
			rPetData.dwLvExp = pLvCfg->unExp;
			//unAddPetExp -= (pLvCfg->unExp - rPetData.dwLvExp);
		} else { // 没升级
			rPetData.dwLvExp += unAddPetExp;
			unAddPetExp = 0;
		}

		if (bUpStarLv && rPetData.wLv == pStarCfg->usPetMaxLv) {
			const PetStarCfg* pNextStarCfg = CPetPropMng::Instance()->GetPetStarCfg(pBaseCfg->ucPetType
				, pBaseCfg->ucPetQuality
				, rPetData.byStarLv + 1);
			if (NULL != pNextStarCfg) {
				++ rPetData.byStarLv;
			}
		}
	}
}

VOID CPet::StartPetBattle()
{
	m_dwLastHurt = 0;
	m_bSkillCritical = FALSE;
	m_poSecondHurt = NULL;
	m_dwSkillIdx = 0;
	m_dwRoundIdx = 1;
	UINT8 byIdx = 0;
	for ( ; byIdx < MAX_PET_WASH_PSYCHIC_CELL_CNT; byIdx++)
	{
		if ( 0 == m_PetData.astWashQualityData[byIdx].byIsCanWash )
		{
			break;
		}
	}
	m_dwCurLingMaiNum = byIdx;
	InitBattleMidParam();
}
UINT32 CPet::GetLingli() const {
	UINT32 unRetVal = m_PetData.wLv * GetCurPsychic();
	const PetStarCfg* pStarCfg = GetPetStarCfg();
	if (NULL == pStarCfg) {
		return unRetVal;
	}

	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(m_PetData.wPetCfgId);
	if (NULL == pBaseCfg) {
		return unRetVal;
	}
	return (UINT32)(unRetVal * (1 + pStarCfg->unAddLingliPer / 100.0) / 50) + pBaseCfg->dwBaseLiLing;
}

const PetStarCfg* CPet::GetPetStarCfg() const {
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(m_PetData.wPetCfgId);
	if (NULL == pBaseCfg) {
		return NULL;
	}

	const PetStarCfg* pStarCfg = CPetPropMng::Instance()->GetPetStarCfg(pBaseCfg->ucPetType
		, pBaseCfg->ucPetQuality
		, m_PetData.byStarLv);

	return pStarCfg;
}

CPetSkill* CPet::GetCurSkill()  
{
	if( m_dwRoundIdx < m_dwCurLingMaiNum )
	{
		return NULL;
	}
	UINT32 dwSize = m_vecSkill.size();
	if ( 0 == dwSize || (dwSize <= m_dwSkillIdx) )
	{
		return NULL;
	}
	m_dwSkillIdx = dwSize < m_dwSkillIdx ? dwSize - 1 : m_dwSkillIdx;
	CPetSkill* poSkill = m_vecSkill[m_dwSkillIdx];
	if ( NULL != poSkill)
	{
		m_wSkillActionID = poSkill->GetID();
	}
	return m_vecSkill[m_dwSkillIdx];
}

BOOL CPet::EnterNextSkill()
{
	//宠物攻击方式改变（由技能数变成灵脉 数）
	//UINT32 dwSize = m_vecSkill.size();
	//if ( dwSize <= m_dwSkillIdx)
	//{
	//	return FALSE;
	//}
	//m_dwSkillIdx++;
	if( m_dwRoundIdx < m_dwCurLingMaiNum )
	{
		return FALSE;
	}
	m_dwRoundIdx++;
	return TRUE;
}

UINT64 CPet::GetPetScore() const
{

	//宠物评分＝（宠物档次＋１０）／１０＊宠物灵力＊（宠物品质＋１０）／１０
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(m_PetData.wPetCfgId);
	if (NULL == pBaseCfg) {
		return 0;
	}
	//档次
	UINT8 byLevel = GetPetGrade();
	if ( 0 == byLevel)
	{
		return 0;
	}
	UINT64 qwScore = (UINT64)((byLevel + 10) / 10.f * GetLingli() * (pBaseCfg->ucPetQuality + 10) / 10.0f);
	return qwScore;
}

UINT8 CPet::GetPetGrade()const
{
	//风　水　火　土　　为第１档
	//冰　炎　砂　　　　为第２档
	//梦　魂　　　　　　为第３档
	//传说　　　　　　　为第４档
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(m_PetData.wPetCfgId);
	if (NULL == pBaseCfg) {
		return 0;
	}
	switch(pBaseCfg->ucPetType)
	{
	case ePT_Wind:// 风
	case ePT_Water:// 水
	case ePT_Fire:// 火
	case ePT_Land:// 土
		return EPL_FIRST;
	case ePT_Ice:// 冰
	case ePT_Sand:// 砂
	case ePT_Hot:// 炎
		return EPL_SECOND;
	case ePT_Soul:// 阴魂
	case ePT_Dream:// 梦
		return EPL_THIRD;
	case ePT_Legend:
		return EPL_FOURTH;
	}
	return 0;
}

UINT32 CPet::GetCurPsychic() const {
	UINT32 unRetVal = 0;
	for (size_t n = 0; n < MAX_PET_WASH_PSYCHIC_CELL_CNT; ++ n) {
		unRetVal += m_PetData.astWashQualityData[n].dwPsychicVal;
	}
	return unRetVal;
}

UINT32 CPet::GetMaxPsychic() const {
	const PetBaseCfg* pBaseCfg = CPetPropMng::Instance()->GetPetBaseCfg(m_PetData.wPetCfgId);
	if (NULL == pBaseCfg) { 
		return 0;
	}
	return pBaseCfg->unPhyChicMax;
}

UINT16	CPet::GetLevel() const 
{
	return m_PetData.wLv;
}

BOOL CPet::DressNeiDan(CPetNeiDan* poNeiDan)
{
	if ( NULL == poNeiDan)
	{
		return FALSE;
	}
	SPetNeiDanProp* poProp = poNeiDan->GetBaseProp();
	if ( NULL == poProp)
	{
		return FALSE;
	}
	
	UINT8 byPosType = poProp->byWuXing;
	m_mapNeiDan[byPosType] = poNeiDan;
	return TRUE;
}

CPetNeiDan* CPet::UnDressNeiDan(UINT8 byPosType)
{
	CPetNeiDan* poUnDressNeiDan = m_mapNeiDan[byPosType];
	m_mapNeiDan.erase(byPosType);
	return poUnDressNeiDan;
}

VOID CPet::GetNeiDanCli(DT_PET_NEIDAN_CLI& stNeiDanCli)
{
	UINT8 byNum = 0;
	for ( map<UINT8, CPetNeiDan*>::iterator itr = m_mapNeiDan.begin(); itr != m_mapNeiDan.end(); itr++, byNum++)
	{
		itr->second->GetNeiDanCli(stNeiDanCli.astNeiDanInfo[0]);
	}
	stNeiDanCli.byNum = byNum;
}

VOID CPet::GetNeiDanMark(DT_NEIDAN_PET_MARK_LST_DATA& stMark)
{
	UINT8 byNum = 0;
	for ( map<UINT8, CPetNeiDan*>::iterator itr = m_mapNeiDan.begin(); itr != m_mapNeiDan.end(); itr++, byNum++)
	{
		itr->second->GetNeiDanMark(stMark.astNeiDanInfo[0]);
	}
	stMark.byNum = byNum;
}