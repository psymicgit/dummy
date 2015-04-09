#include "petpropmng.h"
#include "framework/gsapi.h"
#include "commondef.h"
#include <sddb.h>
#include <db/dbmgr.h>
#include "../base/stringtool.h"
#include "logic/pet/petobjmng.h"
#include "logic/pet/playerpet.h"
CPetPropMng::CPetPropMng() {

}

CPetPropMng::~CPetPropMng() {

}

BOOL CPetPropMng::Init() {
	if (!LoadPetSysCfg()
		|| !LoadPetBaseCfg()
		|| !LoadPetLvCfg()
		|| !LoadPetProductCfg()
		|| !LoadPetProductExtraCfg()
		|| !LoadPetStarCfgList()
		|| !LoadFeedCfg()
		|| !LoadStoreCfg()
		|| !LoadWashCfg()
		|| !LoadPetRefineCfg()) 
	{
		PRINT_ERR_INFO;
		return FALSE;
	}

	return TRUE;
}

VOID CPetPropMng::UnInit() {
	
	for (PetBaseCfgList::iterator itor = m_mpPetBaseCfg.begin(), end = m_mpPetBaseCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpPetBaseCfg.clear();

	for (size_t n = 0; n < m_vPetProductCfg.size(); ++n) {
		if (NULL != m_vPetProductCfg[n]) {
			delete m_vPetProductCfg[n];
		}
	}
	m_vPetProductCfg.clear();

	for (size_t n = 0; n < m_vProductExtraCfg.size(); ++n) {
		if (NULL != m_vProductExtraCfg[n]) {
			delete m_vProductExtraCfg[n];
		}
	}
	m_vProductExtraCfg.clear();

	for (MapPetLvCfg::const_iterator itor = m_mpPetLvCfg.begin(), end = m_mpPetLvCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpPetLvCfg.clear();

	for (MapPetStarCfg::const_iterator itor = m_mpPetStarCfg.begin(), end = m_mpPetStarCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpPetStarCfg.clear();

	for (MapPetFeedCfg::const_iterator itor = m_mpFeedCfg.begin(), end = m_mpFeedCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpFeedCfg.clear();

	for (MapPetStoreCfg::const_iterator itor = m_mpStoreCfg.begin(), end = m_mpStoreCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpFeedCfg.clear();

	for (MapPetWashCfg::const_iterator itor = m_mpWashCfg.begin(), end = m_mpWashCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpWashCfg.clear();

	for (MapRefineCfg::const_iterator itor = m_mpRefineCfg.begin(), end = m_mpRefineCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpRefineCfg.clear();
}

const PetSysCfg& CPetPropMng::GetPetSysCfg() const {
	return m_PetSysCfg;
}

const PetBaseCfg* CPetPropMng::GetPetBaseCfg(UINT16 usPetCfgId) {
	PetBaseCfgList::const_iterator itor = m_mpPetBaseCfg.find(usPetCfgId);
	if (m_mpPetBaseCfg.end() != itor) {
		return itor->second;
	}

	return NULL;
}

const PetProductCfg* CPetPropMng::GetProductCfg(UINT16 usPlayerLv, UINT8 ucProductType) {
	for (size_t n = 0; n < m_vPetProductCfg.size(); ++n) {
		if (m_vPetProductCfg[n]->usPlayerMinLv <= usPlayerLv
			&& m_vPetProductCfg[n]->usPlayerMaxLv >= usPlayerLv
			&& ucProductType == m_vPetProductCfg[n]->ucProductType)
		{
			return m_vPetProductCfg[n];
		}
	}
	return NULL;
}

const PetProductExtraCfg* CPetPropMng::GetProductExtraCfg(UINT8 ucProductPhrase, BOOL bIsGoodProduct, UINT16 usProductCnt) {
	const PetProductExtraCfg* pRetCfg = NULL;
	for (size_t n = 0; n < m_vProductExtraCfg.size(); ++ n) {
		if (ucProductPhrase == m_vProductExtraCfg[n]->ucProductPhrase
			&& bIsGoodProduct == m_vProductExtraCfg[n]->bIsGoodProduct
			&& usProductCnt == m_vProductExtraCfg[n]->usProductCnt)
		{
			if (NULL != pRetCfg) {
				pRetCfg = m_vProductExtraCfg[n];
			}
		}
	}

	return pRetCfg;
}

const PetStarCfg* CPetPropMng::GetPetStarCfg(UINT8 ucPetType, UINT8 ucPetQuality, UINT8 ucStarLv) const {
	PetStarKey key;
	key.ucPetType = ucPetType;
	key.ucPetQuality = ucPetQuality;
	key.usPetStarLv = ucStarLv;
	MapPetStarCfg::const_iterator itor = m_mpPetStarCfg.find(key);
	if (m_mpPetStarCfg.end() != itor) {
		return itor->second;
	}
	return NULL;
}

const PetLvCfg* CPetPropMng::GetPetLvCfg(UINT16 usPetLv) {
	PetLvKey key;
	key.usPetLv = usPetLv;
	MapPetLvCfg::const_iterator itor = m_mpPetLvCfg.find(key);
	if (m_mpPetLvCfg.end() == m_mpPetLvCfg.find(key)) {
		return 0;
	}
	return itor->second;
}

const PetFeedCfg* CPetPropMng::GetPetFeedCfg(UINT8 ucFruitType) const {
	MapPetFeedCfg::const_iterator itor = m_mpFeedCfg.find(ucFruitType);
	if (m_mpFeedCfg.end() != itor) {
		return itor->second;
	}
	return NULL;
}

const PetStoreCfg* CPetPropMng::GetPetStoreCfg(UINT16 usStoreId) {
	MapPetStoreCfg::const_iterator itor = m_mpStoreCfg.find(usStoreId);
	if (m_mpStoreCfg.end() != itor) {
		return itor->second;
	}
	return NULL;
}

const PetWashCfg* CPetPropMng::GetPetWashCfg(UINT8 ucPetType, UINT8 ucPetQuality, UINT8 ucWashQuality) {
	PetWashKey key;
	key.ucPetQuality = ucPetQuality;
	key.ucPetType = ucPetType;
	key.ucWashQuality = ucWashQuality;
	MapPetWashCfg::const_iterator itor = m_mpWashCfg.find(key);
	if (m_mpWashCfg.end() != itor) {
		return itor->second;
	}
	return NULL;
}

BOOL CPetPropMng::GetLowPetCli(CPlayer* poPlayer, DT_PET_BOOK_DATA_LST& stBookData)
{
	if ( NULL == poPlayer)
	{
		return FALSE;
	}
	SDStrncpy(stBookData.aszTitle, m_PetSysCfg.strLowDesc.c_str(), NORMAL_MSG_LEN - 1);
	CMultiMapBaseProp::iterator beginItr = m_mapBaseCfgByTypeQulity.lower_bound(CLogicFunc::Get88Key(ePT_Wind, ePQ_NormalPet));
	CMultiMapBaseProp::iterator endItr = m_mapBaseCfgByTypeQulity.upper_bound(CLogicFunc::Get88Key(ePT_Land, ePQ_VariantPet));
	UINT16 wPetNum = 0;
	for ( CMultiMapBaseProp::iterator itr = beginItr; itr != endItr; itr++, wPetNum++)
	{
		GetPetCli(poPlayer, itr->second, stBookData.astPetData[wPetNum]);
	}
	stBookData.wPetNum = wPetNum;
	return TRUE;
}

BOOL CPetPropMng::GetMiddlePetCli(CPlayer* poPlayer, DT_PET_BOOK_DATA_LST& stBookData)
{
	if ( NULL == poPlayer)
	{
		return FALSE;
	}

	SDStrncpy(stBookData.aszTitle, m_PetSysCfg.strMiddleDesc.c_str(), NORMAL_MSG_LEN - 1);
	CMultiMapBaseProp::iterator beginItr = m_mapBaseCfgByTypeQulity.lower_bound(CLogicFunc::Get88Key(ePT_Ice, ePQ_NormalPet));
	CMultiMapBaseProp::iterator endItr = m_mapBaseCfgByTypeQulity.upper_bound(CLogicFunc::Get88Key(ePT_Hot, ePQ_VariantPet));
	UINT16 wPetNum = 0;
	for ( CMultiMapBaseProp::iterator itr = beginItr; itr != endItr; itr++, wPetNum++)
	{
		GetPetCli(poPlayer, itr->second, stBookData.astPetData[wPetNum]);
	}
	stBookData.wPetNum = wPetNum;
	return TRUE;
}

BOOL CPetPropMng::GetHighPetCli(CPlayer* poPlayer, DT_PET_BOOK_DATA_LST& stBookData)
{
	if ( NULL == poPlayer)
	{
		return FALSE;
	}
	SDStrncpy(stBookData.aszTitle, m_PetSysCfg.strHighDesc.c_str(), NORMAL_MSG_LEN - 1);
	CMultiMapBaseProp::iterator beginItr = m_mapBaseCfgByTypeQulity.lower_bound(CLogicFunc::Get88Key(ePT_Soul, ePQ_NormalPet));
	CMultiMapBaseProp::iterator endItr = m_mapBaseCfgByTypeQulity.upper_bound(CLogicFunc::Get88Key(ePT_Legend, ePQ_VariantPet));
	UINT16 wPetNum = 0;
	for ( CMultiMapBaseProp::iterator itr = beginItr; itr != endItr; itr++, wPetNum++)
	{
		GetPetCli(poPlayer, itr->second, stBookData.astPetData[wPetNum]);
		stBookData.astPetData[wPetNum].byPetState = (stBookData.astPetData[wPetNum].byPetState == 1 ? 2 : 0);
	}
	stBookData.wPetNum = wPetNum;
	return TRUE;
}

VOID CPetPropMng::GetPetSkillCli(UINT16 wSkillID, UINT16 wPetLevel, DT_SKILL_INFO& stSkillInfo)
{
	const SPetSkillEffectProp* poSkillProp = CSkillPropMgr::Instance()->GetPetSkillProp( wSkillID, 1);
	if ( NULL != poSkillProp)
	{
		stSkillInfo.wSkillID = wSkillID;
		stSkillInfo.wHurtRate = poSkillProp->wHurtRate;
		stSkillInfo.wFinalHurtRate = poSkillProp->wFinalHurtValue;
		stSkillInfo.wSpecialParam = poSkillProp->wOnceEffectParam;
		
		stSkillInfo.byHurtPeople = CSkillPropMgr::Instance()->GetPetSkillTrigerNum( wSkillID, wPetLevel);
		if ( 0 != poSkillProp->vecBuffProp.size())
		{
			stSkillInfo.byBuffRound = poSkillProp->vecBuffProp[0].byBuffRound;
			stSkillInfo.wBuffParam = poSkillProp->vecBuffProp[0].wBuffParam;
		}
	}
	else 
	{
		const SRandomPetSkillProp* poRandomProp = CSkillPropMgr::Instance()->GetPetRandomSkillProp(wSkillID, 1);
		if ( NULL != poRandomProp)
		{
			stSkillInfo.wSkillID = wSkillID;
			stSkillInfo.wHurtRate = poRandomProp->stMainProp.wHurtRate;
			stSkillInfo.wFinalHurtRate = poRandomProp->stMainProp.wFinalHurtValue;
			stSkillInfo.byHurtPeople = CSkillPropMgr::Instance()->GetPetSkillTrigerNum( wSkillID, wPetLevel);
		}
	}
}

VOID CPetPropMng::GetPetCli(CPlayer* poPlayer, const PetBaseCfg* poBaseCgf, DT_PET_BOOK_DATA& stPetBookData)
{

	if ( NULL == poPlayer || NULL == poBaseCgf)
	{
		return ;
	}
	CPlayerPet* poPlayerPet = poPlayer->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return ;
	}
	stPetBookData.stPetInfo.wPetCfgID = poBaseCgf->usPetCfgId;
	stPetBookData.byPetState = poPlayerPet->HaveCall( poBaseCgf->usPetCfgId) ? 0 : 1;
	SDStrncpy(stPetBookData.stPetInfo.aszPetName, poBaseCgf->strPetName.c_str(), MAX_PET_NAME_LENGTH - 1);
	SDStrncpy(stPetBookData.stPetInfo.aszPetPic, poBaseCgf->strPetPic.c_str(), MAX_PET_NAME_LENGTH - 1);
	stPetBookData.stPetInfo.byIsUpStar = 0;
	stPetBookData.stPetInfo.byPetColor = poBaseCgf->ucPetColor;
	stPetBookData.stPetInfo.byPetQuality = poBaseCgf->ucPetQuality;
	stPetBookData.stPetInfo.byPetType = poBaseCgf->ucPetType;
	stPetBookData.stPetInfo.byPetState = 0;
	stPetBookData.stPetInfo.byStarLv = 0;
	stPetBookData.stPetInfo.dwCurLingli = 0;
	stPetBookData.stPetInfo.wLv = 1;
	stPetBookData.stPetInfo.dwMaxPhychicVal = poBaseCgf->unPhyChicMax;
	const PetStarCfg* pStarCfg = CPetPropMng::Instance()->GetPetStarCfg(poBaseCgf->ucPetType
		, poBaseCgf->ucPetQuality
		, 0);
	if (NULL != pStarCfg) {
		stPetBookData.stPetInfo.wMaxLv = pStarCfg->usPetMaxLv;
	}
	//下级经验
	const PetLvCfg* pNextLvCfg = CPetPropMng::Instance()->GetPetLvCfg(2);
	if (NULL != pNextLvCfg) {
		stPetBookData.stPetInfo.dwNextLvExp = pNextLvCfg->unExp ;
	}
	//技能描述
	UINT8 bySkill = 0;
	for ( vector<UINT16>::const_iterator itr = poBaseCgf->vecSkill.begin(); itr != poBaseCgf->vecSkill.end(); itr++)
	{
		GetPetSkillCli(*itr, 1, stPetBookData.stPetInfo.stSkillInfo.astSkillInfo[bySkill]);
		stPetBookData.stPetInfo.stSkillInfo.astSkillInfo[bySkill].byBeginRound = bySkill + 1;
		bySkill++;
	}
	stPetBookData.stPetInfo.stSkillInfo.bySkillNum = bySkill;
}

BOOL CPetPropMng::GetLevel(UINT32 dwExp, UINT8 byMaxStar, UINT16* pwPetLevel, UINT32* pdwOutExp)
{

	const PetStarCfg* poStarCfg = GetPetStarCfg( ePQ_NormalPet, ePT_Wind, byMaxStar);
	if ( NULL == poStarCfg)
	{
		return FALSE;
	}

	const PetLvCfg* poLvCfg = GetPetLvCfg(poStarCfg->usPetMaxLv);
	if ( NULL == poLvCfg)
	{
		return FALSE;
	}
	UINT32 dwMaxExp = 0;
	//最大经验
	dwMaxExp = poLvCfg->unExp;
	dwMaxExp = dwExp > dwMaxExp ? dwMaxExp : dwExp; 
	if ( pdwOutExp )
	{
		*pdwOutExp = dwMaxExp;
	}
	UINT16 wLevel = 0;
	C3232MapItr itr = m_mapExpToLevel.lower_bound(dwExp);
	if ( itr == m_mapExpToLevel.end())
	{
		wLevel = m_mapExpToLevel.rbegin()->second;
	}
	else
	{
		wLevel = itr->first == dwExp ? itr->second : itr->second - 1;
	}
	if ( pwPetLevel )
	{
		*pwPetLevel = wLevel;
	}
	
	return TRUE;
	
}

UINT8 CPetPropMng::GetWashQuality(UINT8 ucPetType, UINT8 ucPetQuality, UINT32 unQualityVal) {
	std::vector<MapPetWashCfg::const_iterator> vWashCfg;
	PetWashKey key;
	key.ucPetQuality = ucPetQuality;
	key.ucPetType = ucPetType;
	for (size_t n = 1; n <= MAX_PET_WASH_PSYCHIC_QUALITY_CNT; ++ n) {
		key.ucWashQuality = n;
		MapPetWashCfg::const_iterator itor = m_mpWashCfg.find(key);
		if (m_mpWashCfg.end() != itor) {
			vWashCfg.push_back(itor);
		} 
	}

	UINT8 ucRetVal = 1;
	for (size_t n = 0; n < vWashCfg.size(); ++ n) {
		if (unQualityVal >= vWashCfg[n]->second->totalRange.unMinVal
			&& unQualityVal <= vWashCfg[n]->second->totalRange.unMaxVal)
		{
			return vWashCfg[n]->first.ucWashQuality;
		}
	}

	return ucRetVal;
}

const PetRefineCfg* CPetPropMng::GetRefineCfg(UINT8 ucPetTypeLevel, UINT8 ucPetQuality) {
	PetRefineKey key(ucPetTypeLevel, ucPetQuality);
	MapRefineCfg::const_iterator itor = m_mpRefineCfg.find(key);
	if (m_mpRefineCfg.end() != itor) {
		return itor->second;
	}
	return NULL;
}

BOOL CPetPropMng::LoadPetSysCfg() {
	DB_READ_BEGIN("petsysprop");

	while (pRecordSet->GetRecord()) {
		m_PetSysCfg.usMaxCarryPet = SGDP::SDAtou(pRecordSet->GetFieldValueByName("maxcarrypetcnt"));
		m_PetSysCfg.ucFreeFeedCnt = SGDP::SDAtou(pRecordSet->GetFieldValueByName("freefeeddrawcnt"));
		m_PetSysCfg.unResetFruitCost = SGDP::SDAtou(pRecordSet->GetFieldValueByName("resetdrawcostgold"));
		m_PetSysCfg.unMaxFruitCost = SGDP::SDAtou(pRecordSet->GetFieldValueByName("maxfruitcostgold"));
		m_PetSysCfg.ucTriggerWashQuality = SGDP::SDAtou(pRecordSet->GetFieldValueByName("triigerwashquality"));
		m_PetSysCfg.unNormalInheritCostCoin = SGDP::SDAtou(pRecordSet->GetFieldValueByName("normalinheritcostcoin"));
		m_PetSysCfg.unPerfectInheritCostGold = SGDP::SDAtou(pRecordSet->GetFieldValueByName("perfectinheritcostgold"));
		m_PetSysCfg.usNormalFreeProductCnt = SGDP::SDAtou(pRecordSet->GetFieldValueByName("freenormalproductcnt"));
		m_PetSysCfg.ucNormalInheritExp = SGDP::SDAtou(pRecordSet->GetFieldValueByName("normalinheritexp"));
		m_PetSysCfg.ucGoodInheritExp = SGDP::SDAtou(pRecordSet->GetFieldValueByName("goodinheritexp"));
		m_PetSysCfg.strWashDesc = pRecordSet->GetFieldValueByName("washdesc");
		m_PetSysCfg.strRuleDesc = pRecordSet->GetFieldValueByName("rule");
		string strItemID = pRecordSet->GetFieldValueByName("displayitemid");
		StringTool::SDSplitStringToUInt(m_PetSysCfg.vecItemID, strItemID, ',');
		m_PetSysCfg.strLowDesc = pRecordSet->GetFieldValueByName("LowDesc");
		m_PetSysCfg.strMiddleDesc = pRecordSet->GetFieldValueByName("MiddleDesc");
		m_PetSysCfg.strHighDesc = pRecordSet->GetFieldValueByName("HighDesc");
		m_PetSysCfg.strItemDesc = pRecordSet->GetFieldValueByName("ItemDesc");

	}
	DB_READ_END;
	return TRUE;
}
BOOL CPetPropMng::LoadPetSkill(vector<UINT16>& vecSkill, const string& strSkillValue)
{
	if ( strSkillValue == "")
	{
		return TRUE;
	}
	Json::Value root;
	Json::Reader reader;
	if( !reader.parse( strSkillValue, root ) )
	{
		SYS_CRITICAL( _SDT( "json parse error, result:%s" ), strSkillValue.c_str() );
		return FALSE;
	}

	//检测数据个数，如果为零刚为出错，防止崩溃
	if (!root.isObject())
	{
		SYS_CRITICAL( _SDT( "json size error, result:%s" ), strSkillValue.c_str() );
		return FALSE;
	}

	//解析物品的个数
	if (root["SkillID"].isArray())
	{
		Json::Value skills = root["SkillID"];
		for (UINT32 idx = 0; idx < skills.size(); idx++)
		{
			if ( skills[idx].isInt())
			{
				vecSkill.push_back(skills[idx].asInt());
			}
		}
	}
	return TRUE;
}

BOOL CPetPropMng::LoadPetBaseCfg() {
	DB_READ_BEGIN("petbaseprop");

	while (pRecordSet->GetRecord()) {
		PetBaseCfg* pBaseCfg = new PetBaseCfg;
		pBaseCfg->usPetCfgId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("petcfgid"));
		pBaseCfg->strPetName = pRecordSet->GetFieldValueByName("name");
		pBaseCfg->ucPetType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("pettype"));
		pBaseCfg->ucPetColor = SGDP::SDAtou(pRecordSet->GetFieldValueByName("petcolor"));
		pBaseCfg->ucPetQuality = SGDP::SDAtou(pRecordSet->GetFieldValueByName("petquality"));
		pBaseCfg->unDevourExp = SGDP::SDAtou(pRecordSet->GetFieldValueByName("devourexp"));
		pBaseCfg->unPhyChicMax = SGDP::SDAtou(pRecordSet->GetFieldValueByName("phychicmax"));
		pBaseCfg->ucPhyChicValPerMin = SGDP::SDAtou(pRecordSet->GetFieldValueByName("initphychicmin"));
		pBaseCfg->ucPhyChicValPerMax = SGDP::SDAtou(pRecordSet->GetFieldValueByName("initphychicmax"));
		pBaseCfg->strPetPic = pRecordSet->GetFieldValueByName("picture");
		pBaseCfg->strPetDesc = pRecordSet->GetFieldValueByName("petdesc");
		pBaseCfg->dwBaseLiLing = SDAtou(pRecordSet->GetFieldValueByName("baseliling"));
		if( !LoadPetSkill(pBaseCfg->vecSkill, pRecordSet->GetFieldValueByName("skillprop")))
		{
			USR_INFO("[%s:%d] Pet Skill Prop failed", MSG_MARK);
			return FALSE;
		}
		UINT8 byBook = SDAtou(pRecordSet->GetFieldValueByName("bookflag"));
		m_mpPetBaseCfg.insert(make_pair(pBaseCfg->usPetCfgId, pBaseCfg));
		if ( byBook)
		{
			m_mapBaseCfgByTypeQulity.insert(make_pair(CLogicFunc::Get88Key(pBaseCfg->ucPetType, pBaseCfg->ucPetQuality), pBaseCfg));
		}
		
	}

	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadPetLvCfg() {
	DB_READ_BEGIN("petlvprop");

	while (pRecordSet->GetRecord()) {
		PetLvKey key;
		PetLvCfg* pLvCfg = new PetLvCfg;
		key.usPetLv = SGDP::SDAtou(pRecordSet->GetFieldValueByName("level"));
		
		pLvCfg->unExp = SGDP::SDAtou(pRecordSet->GetFieldValueByName("experience"));

		m_mapExpToLevel[pLvCfg->unExp] = key.usPetLv;
		m_mpPetLvCfg.insert(make_pair(key, pLvCfg));
	}

	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadPetProductCfg() {
	DB_READ_BEGIN("petproductprop");

	while (pRecordSet->GetRecord()) {
		PetProductCfg* pProductCfg = new PetProductCfg;
		pProductCfg->ucProductPhrase = SGDP::SDAtou(pRecordSet->GetFieldValueByName("productphrase"));
		pProductCfg->ucProductType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("producttype"));
		pProductCfg->unScore = SGDP::SDAtou(pRecordSet->GetFieldValueByName("score"));
		pProductCfg->unCostGold = SGDP::SDAtou(pRecordSet->GetFieldValueByName("costgold"));
		pProductCfg->usFreeTimeInterval = SGDP::SDAtou(pRecordSet->GetFieldValueByName("freetimeinterval"));
		std::string strPlayerLv = pRecordSet->GetFieldValueByName("playerlv");
		std::vector<UINT32> vPlayerLv;
		StringTool::SDSplitStringToUInt(vPlayerLv, strPlayerLv, ',');
		if (2 != vPlayerLv.size() || vPlayerLv[0] >= vPlayerLv[1]) {
			assert(FALSE);
			return FALSE;
		}
		pProductCfg->usPlayerMinLv = vPlayerLv[0];
		pProductCfg->usPlayerMaxLv = vPlayerLv[1];

		std::string strPetType = pRecordSet->GetFieldValueByName("getpettype");
		std::string strPetTypeWeight = pRecordSet->GetFieldValueByName("getpetweight");
		StringTool::SDSplitStringToUInt(pProductCfg->vGetPetType, strPetType, ',');
		StringTool::SDSplitStringToUInt(pProductCfg->vPetWeight, strPetTypeWeight, ',');
		if (pProductCfg->vGetPetType.empty() 
			|| pProductCfg->vGetPetType.size() != pProductCfg->vPetWeight.size()) 
		{
			assert(FALSE);
			return FALSE;
		}

		std::string strGetQuality = pRecordSet->GetFieldValueByName("getpetquality");
		std::string strQualityWeight = pRecordSet->GetFieldValueByName("qualityweight");
		StringTool::SDSplitStringToUInt(pProductCfg->vGetPetQuality, strGetQuality, ',');
		StringTool::SDSplitStringToUInt(pProductCfg->vQualityWeight, strQualityWeight, ',');
		if (pProductCfg->vGetPetQuality.empty() 
			|| pProductCfg->vGetPetQuality.size() != pProductCfg->vQualityWeight.size()) 
		{
			assert(FALSE);
			return FALSE;
		}

		std::string strItemId = pRecordSet->GetFieldValueByName("getitemid");
		std::string strItemNum = pRecordSet->GetFieldValueByName("getitemnum");
		std::string strItemPercent = pRecordSet->GetFieldValueByName("getitempercent");
		StringTool::SDSplitStringToUInt(pProductCfg->vGetItemId, strItemId, ',');
		StringTool::SDSplitStringToUInt(pProductCfg->vGetItemNum, strItemNum, ',');
		StringTool::SDSplitStringToUInt(pProductCfg->vGetItemPercent, strItemPercent, ',');
		if (pProductCfg->vGetItemId.empty() 
			|| pProductCfg->vGetItemId.size() != pProductCfg->vGetItemNum.size()
			|| pProductCfg->vGetItemId.size() != pProductCfg->vGetItemPercent.size()) 
		{
			assert(FALSE);
			return FALSE;
		}

		m_vPetProductCfg.push_back(pProductCfg);
	}

	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadPetProductExtraCfg() {
	DB_READ_BEGIN("petproductextraprop");
	while (pRecordSet->GetRecord()) {
		PetProductExtraCfg* pProductExtraCfg = new PetProductExtraCfg;
		pProductExtraCfg->ucProductPhrase = SGDP::SDAtou(pRecordSet->GetFieldValueByName("productphrase"));
		pProductExtraCfg->bIsGoodProduct = 0 != SGDP::SDAtou(pRecordSet->GetFieldValueByName("isgoodproduct"));
		pProductExtraCfg->usProductCnt = SGDP::SDAtou(pRecordSet->GetFieldValueByName("productcnt"));

		std::string strExtraItemId = pRecordSet->GetFieldValueByName("extraitemid");
		std::string strExtraItemNum = pRecordSet->GetFieldValueByName("extraitemnum");
		StringTool::SDSplitStringToUInt(pProductExtraCfg->vExtraItemId, strExtraItemId, ',');
		StringTool::SDSplitStringToUInt(pProductExtraCfg->vExtraItemNum, strExtraItemNum, ',');
		if (pProductExtraCfg->vExtraItemId.empty() 
			|| pProductExtraCfg->vExtraItemId.size() != pProductExtraCfg->vExtraItemNum.size()) 
		{
			assert(FALSE);
			return FALSE;
		}

		m_vProductExtraCfg.push_back(pProductExtraCfg);
	}
	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadPetStarCfgList() {
	DB_READ_BEGIN("petstarprop");

	while (pRecordSet->GetRecord()) {
 		PetStarKey key;
		PetStarCfg* pPetStarCfg = new PetStarCfg;
		key.ucPetType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("pettype"));
		key.ucPetQuality = SGDP::SDAtou(pRecordSet->GetFieldValueByName("petquality"));
		key.usPetStarLv = SGDP::SDAtou(pRecordSet->GetFieldValueByName("starlv"));

		pPetStarCfg->usPetMaxLv = SGDP::SDAtou(pRecordSet->GetFieldValueByName("petmaxlv"));
		pPetStarCfg->ucPlayerLvLimit = SGDP::SDAtou(pRecordSet->GetFieldValueByName("playerlvlimit"));
		pPetStarCfg->unAddLingliPer = SGDP::SDAtou(pRecordSet->GetFieldValueByName("addlingliper"));
		std::string strCostItemId = pRecordSet->GetFieldValueByName("costitemid");
		std::string strCostItemNum = pRecordSet->GetFieldValueByName("costitemnum");
		StringTool::SDSplitStringToUInt(pPetStarCfg->vCostItemId, strCostItemId, ',');
		StringTool::SDSplitStringToUInt(pPetStarCfg->vCostItemNum, strCostItemNum, ',');
		if (pPetStarCfg->vCostItemId.size() != pPetStarCfg->vCostItemNum.size()) {
			assert(FALSE);
			return FALSE;
		}
		m_mpPetStarCfg.insert(make_pair(key, pPetStarCfg));
	}
	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadFeedCfg() {
	DB_READ_BEGIN("petfeedprop");

	while (pRecordSet->GetRecord()) {
		PetFeedCfg* pFeedCfg = new PetFeedCfg;
		pFeedCfg->ucFruitType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("fruittype"));
		pFeedCfg->ucPercent = SGDP::SDAtou(pRecordSet->GetFieldValueByName("getpercent"));
		pFeedCfg->unBaseExp = SGDP::SDAtou(pRecordSet->GetFieldValueByName("baseexp"));
		pFeedCfg->vExpTimes[0] = SGDP::SDAtou(pRecordSet->GetFieldValueByName("onetimes"));
		pFeedCfg->vExpTimes[1] = SGDP::SDAtou(pRecordSet->GetFieldValueByName("twotimes"));
		pFeedCfg->vExpTimes[2] = SGDP::SDAtou(pRecordSet->GetFieldValueByName("threetimes"));
		pFeedCfg->vExpTimes[3] = SGDP::SDAtou(pRecordSet->GetFieldValueByName("fourtimes"));
		pFeedCfg->vExpTimes[4] = SGDP::SDAtou(pRecordSet->GetFieldValueByName("fivetimes"));

		m_mpFeedCfg.insert(make_pair(pFeedCfg->ucFruitType, pFeedCfg));
	}

	assert(ePetFruitCnt == m_mpFeedCfg.size());
	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadStoreCfg() {
	DB_READ_BEGIN("petstoreprop");
	
	while (pRecordSet->GetRecord()) {
		PetStoreCfg* pStoreCfg = new PetStoreCfg;
		UINT16 usStoreId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("storeid"));
		pStoreCfg->unItemId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("itemid"));
		pStoreCfg->unCostPetScore = SGDP::SDAtou(pRecordSet->GetFieldValueByName("costpetscore"));
		pStoreCfg->unCostGold = SGDP::SDAtou(pRecordSet->GetFieldValueByName("costgold"));
		pStoreCfg->usPlayerLvLimit = SGDP::SDAtou(pRecordSet->GetFieldValueByName("playerlvlimit"));
		pStoreCfg->usShowLv = SGDP::SDAtou(pRecordSet->GetFieldValueByName("showlv"));

		pStoreCfg->ucItemType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("itemtype"));

		m_mpStoreCfg.insert(make_pair(usStoreId, pStoreCfg));
	}

	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadWashCfg() {
	DB_READ_BEGIN("petwashprop");

	while (pRecordSet->GetRecord()) {
		PetWashKey key;
		PetWashCfg* pWashCfg = new PetWashCfg;

		pWashCfg->unLockCostGold = SGDP::SDAtou(pRecordSet->GetFieldValueByName("lockcostgold"));

		key.ucPetType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("pettype"));
		key.ucPetQuality = SGDP::SDAtou(pRecordSet->GetFieldValueByName("petquality"));
		key.ucWashQuality = SGDP::SDAtou(pRecordSet->GetFieldValueByName("washquality"));
		string strPossibleQuality = pRecordSet->GetFieldValueByName("possiblewashquality");
		string strQualityWeight = pRecordSet->GetFieldValueByName("washqualityweight");
		StringTool::SDSplitStringToUInt(pWashCfg->vPossibleWashQuality, strPossibleQuality, ',');
		StringTool::SDSplitStringToUInt(pWashCfg->vWashQualityWeight, strQualityWeight, ',');
		if (pWashCfg->vPossibleWashQuality.empty()
			|| pWashCfg->vPossibleWashQuality.size() != pWashCfg->vWashQualityWeight.size()) 
		{
			assert(FALSE);
			return FALSE;
		}
		
		string strUseItemId = pRecordSet->GetFieldValueByName("useitemid");
		string strUseItemNum = pRecordSet->GetFieldValueByName("useitemnum");
		StringTool::SDSplitStringToUInt(pWashCfg->vUseItemid, strUseItemId, ',');
		StringTool::SDSplitStringToUInt(pWashCfg->vUseItemNum, strUseItemNum, ',');
		if (pWashCfg->vUseItemid.empty()
			|| pWashCfg->vUseItemid.size() != pWashCfg->vUseItemNum.size()) 
		{
			assert(FALSE);
			return FALSE;
		}

		std::vector<string> vSplitStr = SDSplitStringsA(pRecordSet->GetFieldValueByName("psychicrange"), ';');
		for (size_t n = 0; n < vSplitStr.size(); ++n) {
			vector<UINT32> vSplitVal;
			StringTool::SDSplitStringToUInt(vSplitVal, vSplitStr[n], ',');
			if (2 != vSplitVal.size()
				|| vSplitVal[0] > vSplitVal[1])
			{
				assert(FALSE);
				return FALSE;
			}
			PetPsychicRange range;
			range.unMinVal = vSplitVal[0];
			range.unMaxVal = vSplitVal[1];

			if (0 == pWashCfg->totalRange.unMinVal
				|| range.unMinVal < pWashCfg->totalRange.unMinVal)
			{
				pWashCfg->totalRange.unMinVal = range.unMinVal;
			}
			if (0 == pWashCfg->totalRange.unMaxVal
				|| range.unMaxVal > pWashCfg->totalRange.unMaxVal)
			{
				pWashCfg->totalRange.unMaxVal = range.unMaxVal;
			}

			pWashCfg->vPsychicRange.push_back(range);
		}

		string strPsychiRangeWeight = pRecordSet->GetFieldValueByName("psychicrangeweight");
		StringTool::SDSplitStringToUInt(pWashCfg->vPsychicWeight, strPsychiRangeWeight, ',');
		if (pWashCfg->vPsychicRange.empty()
			|| pWashCfg->vPsychicRange.size() != pWashCfg->vPsychicWeight.size()) 
		{
			assert(FALSE);
			return FALSE;
		}

		m_mpWashCfg.insert(make_pair(key, pWashCfg));
	}

	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadPetNeiDanCfg()
{
	DB_READ_BEGIN("petneidan");

	while (pRecordSet->GetRecord()) 
	{
		SPetNeiDanProp stProp;
		stProp.wNeiDanID = SDAtou(pRecordSet->GetFieldValueByName("ID"));
		stProp.strNeiDanName = pRecordSet->GetFieldValueByName("Name");
		stProp.byWuXing = SDAtou(pRecordSet->GetFieldValueByName("WuXing"));
		stProp.byQuality = SDAtou(pRecordSet->GetFieldValueByName("Quality"));
		stProp.byType = SDAtou(pRecordSet->GetFieldValueByName("Type"));
		stProp.dwBaseLingLi = SDAtou(pRecordSet->GetFieldValueByName("BaseLingLi"));
		stProp.dwIncLingLi = SDAtou(pRecordSet->GetFieldValueByName("IncLingLi"));
		stProp.wDressReq = SDAtou(pRecordSet->GetFieldValueByName("DressReq"));
		stProp.dwCostCoin = SDAtou(pRecordSet->GetFieldValueByName("DevorCoin"));
		stProp.dwBaseExperience = SDAtou(pRecordSet->GetFieldValueByName("BaseExperience"));
		m_mapPetNeiDanProp[stProp.wNeiDanID] = stProp;
	}

	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadPetNeiDanUpdateCfg()
{
	DB_READ_BEGIN("petneidanupdateprop");

	while (pRecordSet->GetRecord()) 
	{
		SPetNeiDanUpdateProp stProp;
		stProp.byQuality = SDAtou(pRecordSet->GetFieldValueByName("Quality"));
		stProp.wLevel = SDAtou(pRecordSet->GetFieldValueByName("Level"));
		UINT64 qwExperience1 = SDAtou(pRecordSet->GetFieldValueByName("Type1"));
		UINT64 qwExperience2 = SDAtou(pRecordSet->GetFieldValueByName("Type2"));
		UINT64 qwExperience3 = SDAtou(pRecordSet->GetFieldValueByName("Type3"));
		UINT64 qwExperience4 = SDAtou(pRecordSet->GetFieldValueByName("Type4"));

		stProp.byType = EC_BLUE;
		stProp.qwExperience = qwExperience1;
		m_mapNeiDanUpdateProp[CLogicFunc::Get1688Key(stProp.wLevel, stProp.byQuality, stProp.byType)] = stProp;
		m_mapNeiDanExperience[CLogicFunc::Get88Key(stProp.byQuality, stProp.byType)].insert(make_pair(stProp.qwExperience, stProp.wLevel));
		
		stProp.byType = EC_PURPLE;
		stProp.qwExperience = qwExperience2;
		m_mapNeiDanUpdateProp[CLogicFunc::Get1688Key(stProp.wLevel, stProp.byQuality, stProp.byType)] = stProp;
		m_mapNeiDanExperience[CLogicFunc::Get88Key(stProp.byQuality, stProp.byType)].insert(make_pair(stProp.qwExperience, stProp.wLevel));
		
		stProp.byType = EC_RED;
		stProp.qwExperience = qwExperience3;
		m_mapNeiDanUpdateProp[CLogicFunc::Get1688Key(stProp.wLevel, stProp.byQuality, stProp.byType)] = stProp;
		m_mapNeiDanExperience[CLogicFunc::Get88Key(stProp.byQuality, stProp.byType)].insert(make_pair(stProp.qwExperience, stProp.wLevel));

		stProp.byType = EC_ORANGE;
		stProp.qwExperience = qwExperience4;
		m_mapNeiDanUpdateProp[CLogicFunc::Get1688Key(stProp.wLevel, stProp.byQuality, stProp.byType)] = stProp;
		m_mapNeiDanExperience[CLogicFunc::Get88Key(stProp.byQuality, stProp.byType)].insert(make_pair(stProp.qwExperience, stProp.wLevel));
	}

	DB_READ_END;
	return TRUE;
}

BOOL CPetPropMng::LoadPetRefineCfg() {
	DB_READ_BEGIN("petrefineprop");

	while (pRecordSet->GetRecord()) {
		PetRefineKey key;
		key.first = SGDP::SDAtou(pRecordSet->GetFieldValueByName("refinepettypelevel"));
		key.second = SGDP::SDAtou(pRecordSet->GetFieldValueByName("refinepetquality"));

		PetRefineCfg* pRefineCfg = new PetRefineCfg;

		string strGetPet = pRecordSet->GetFieldValueByName("getpet");
		string strGetPetWeightWeight = pRecordSet->GetFieldValueByName("getpetweight");
		std::vector<string> vSplitStr = SDSplitStringsA(strGetPet, ';');
		for (size_t n = 0; n < vSplitStr.size(); ++n) {
			vector<UINT32> vSplitVal;
			StringTool::SDSplitStringToUInt(vSplitVal, vSplitStr[n], ',');
			if (2 != vSplitVal.size()) {
				assert(FALSE);
				return FALSE;
			}
			PetRefineData refineData;
			refineData.ucPetTypeLevel = vSplitVal[0];
			refineData.ucPetQuality = vSplitVal[1];

			pRefineCfg->vGetPetData.push_back(refineData);
		}
		StringTool::SDSplitStringToUInt(pRefineCfg->vGetPetWeight, strGetPetWeightWeight, ',');
		if (pRefineCfg->vGetPetData.empty()
			|| pRefineCfg->vGetPetData.size() != pRefineCfg->vGetPetWeight.size()) 
		{
			assert(FALSE);
			return FALSE;
		}

		m_mpRefineCfg.insert(make_pair(key, pRefineCfg));
	}

	DB_READ_END;
	return TRUE;
}

SPetNeiDanProp* CPetPropMng::GetPetNeiDanProp(UINT16 wNeiDanID)
{
	CMapPetNeiDanPropItr itr = m_mapPetNeiDanProp.find(wNeiDanID);
	if ( itr == m_mapPetNeiDanProp.end())
	{
		return NULL;
	}
	return &itr->second;
}

SPetNeiDanUpdateProp* CPetPropMng::GetPetNeiDanUpdataProp(UINT16 wLevel, UINT8 byQuality, UINT8 byType)
{
	CMapPetNeiDanUpdatePropItr itr = m_mapNeiDanUpdateProp.find(CLogicFunc::Get1688Key(wLevel, byQuality, byType));
	if ( itr == m_mapNeiDanUpdateProp.end())
	{
		return NULL;
	}
	return &itr->second;
}

UINT16 CPetPropMng::GetNeiDanLevel(UINT64 qwExperience, UINT8 byQuality, UINT8 byType)
{
	map<UINT16, map<UINT64, UINT16>>::iterator itr = m_mapNeiDanExperience.find(CLogicFunc::Get88Key(byQuality, byType));
	if ( itr == m_mapNeiDanExperience.end() )
	{
		return 0;
	}
	map<UINT64, UINT16>::iterator experienceItr = itr->second.lower_bound(qwExperience);
	if ( experienceItr == itr->second.end())
	{
		return 0;
	}
	return experienceItr->second;
}