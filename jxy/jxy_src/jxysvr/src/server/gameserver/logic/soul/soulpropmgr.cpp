#include "soulpropmgr.h"
#include "framework/gsapi.h"
#include "soul.h"
#include "commondef.h"
#include "../base/stringtool.h"
#include <assert.h>
#include <sddb.h>
#include <db/dbmgr.h>
#include "logic/item/itemmgr.h"

IMPLEMENT_SINGLETON_PROPMGR(CSoulPropMgr)

CSoulPropMgr::CSoulPropMgr() 
{
	m_byMaxSubLevel = 0;
}

CSoulPropMgr::~CSoulPropMgr() {

}

BOOL CSoulPropMgr::Init()
{
	if ( !LoadAttrProp())
	{
		return FALSE;
	}
	if ( !LoadAttrRandomProp())
	{
		return FALSE;
	}
	if ( !LoadRandomProp() )
	{
		return FALSE;
	}

	if (!LoadSoulBaseCfg()
		|| !LoadSoulProductCfg()
		|| !LoadSoulLvCfg()
		|| !LoadSoulExchangeLvCfg()
		|| !LoadSoulPotencyProp())
	{
		PRINT_ERR_INFO;
		return FALSE;
	}

	return TRUE;
}

BOOL CSoulPropMgr::LoadSoulPotencyProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from soulpotencyprop order by SoulID, PotencyLevel, PotencySubLevel", &pRecordSet, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CSoulPropMgr::LoadAttrProp has no data!"), MSG_MARK);
		return TRUE; //
	}

	while (pRecordSet->GetRecord()) {
		SSoulPotencyProp stProp;

		stProp.wSoulID = SDAtou(pRecordSet->GetFieldValueByName("SoulID"));
		stProp.byPotencyLevel = SDAtou(pRecordSet->GetFieldValueByName("PotencyLevel"));
		stProp.byPotencySubLevel = SDAtou(pRecordSet->GetFieldValueByName("PotencySubLevel"));
		string strPotencyKindID = pRecordSet->GetFieldValueByName("PotencyKindID");
		string strPotencyValue = pRecordSet->GetFieldValueByName("PotencyValue");
		C32Vec vecPotencyKindID;
		C32Vec vecPotencyValue;
		StringTool::SDSplitStringToUInt(vecPotencyKindID, strPotencyKindID, ',');
		StringTool::SDSplitStringToUInt(vecPotencyValue, strPotencyValue, ',');
		if ( vecPotencyValue.size() != vecPotencyKindID.size() || 0 == vecPotencyKindID.size())
		{
			SYS_CRITICAL(_SDT("[%s: %d]: Potency ID and Value !"), MSG_MARK);
			DB_READ_END;
			return FALSE;
		}
		if ( 1 != stProp.byPotencyLevel || 1 != stProp.byPotencySubLevel)
		{
			if ( 1 == stProp.byPotencySubLevel)
			{
				stProp.mapCountAttrValue = m_mapSoulPotencyProp[CLogicFunc::Get1688Key(stProp.wSoulID, stProp.byPotencyLevel - 1, m_byMaxSubLevel)].mapCountAttrValue;
			}
			else
			{
				stProp.mapCountAttrValue = m_mapSoulPotencyProp[CLogicFunc::Get1688Key(stProp.wSoulID, stProp.byPotencyLevel, stProp.byPotencySubLevel - 1)].mapCountAttrValue;
			}
			
		}
		
		for ( UINT32 dwSize = 0; dwSize < vecPotencyKindID.size(); dwSize++)
		{
			DT_POTENCY_ATTR_INFO stData = {0};
			stData.byPotencyID = vecPotencyKindID[dwSize];
			stData.dwPotencyValue = vecPotencyValue[dwSize];
			stProp.vecPotency.push_back(stData);
			stProp.mapCountAttrValue[stData.byPotencyID] += stData.dwPotencyValue;
		}


		stProp.wNeedSoulLevel = SDAtou(pRecordSet->GetFieldValueByName("NeedSoulLevel"));
		string strCostItemID = pRecordSet->GetFieldValueByName("CostItemID");
		string strCostItemNum = pRecordSet->GetFieldValueByName("CostItemNum");

		C32Vec vecCostItemID;
		C32Vec vecCostItemNum;
		StringTool::SDSplitStringToUInt(vecCostItemID, strCostItemID, ',');
		StringTool::SDSplitStringToUInt(vecCostItemNum, strCostItemNum, ',');

		if ( vecCostItemID.size() != vecCostItemNum.size() || 0 == vecCostItemNum.size())
		{
			SYS_CRITICAL(_SDT("[%s: %d]: Potency CostID and Num !"), MSG_MARK);
			DB_READ_END;
			return FALSE;
		}

		for ( UINT32 dwSize = 0; dwSize < vecCostItemID.size(); dwSize++)
		{
			DT_ITEM_DATA stData = {0};
			stData.wKindID = vecCostItemID[dwSize];
			stData.wNum = vecCostItemNum[dwSize];
			stProp.vecItemData.push_back(stData);
		}

		m_byMaxSubLevel = m_byMaxSubLevel < stProp.byPotencySubLevel ? stProp.byPotencySubLevel : m_byMaxSubLevel;
		m_mapSoulPotencyProp[CLogicFunc::Get1688Key(stProp.wSoulID, stProp.byPotencyLevel, stProp.byPotencySubLevel)] = stProp;
	}
	DB_READ_END;

	return TRUE;
}


VOID CSoulPropMgr::UnInit()
{
	m_mapSoulPotencyProp.clear();
	for (MapSoulCfg::iterator itor = m_mpSoulCfg.begin(), end = m_mpSoulCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
    m_mpSoulCfg.clear();
	m_mpStarWeight.clear();

	for (MapSoulProductCfg::iterator itor = m_mpProfuctCfg.begin(), end = m_mpProfuctCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
    m_mpProfuctCfg.clear();

	for (size_t n = 0; n < m_vecSoulLvCfg.size(); ++n) {
		if (NULL != m_vecSoulLvCfg[n]) {
			delete m_vecSoulLvCfg[n];
		}
	}
	m_vecSoulLvCfg.clear();

	for (MapSoulExchangeCfg::iterator itor = m_mpSoulExchangeCfg.begin(), end = m_mpSoulExchangeCfg.end()
		; end != itor
		; ++ itor)
	{
		if (NULL != itor->second) {
			delete itor->second;
		}
	}
	m_mpSoulExchangeCfg.clear();
}

//属性基本值
BOOL CSoulPropMgr::LoadAttrProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from soulattrprop", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CSoulPropMgr::LoadAttrProp has no data!"), MSG_MARK);
		return TRUE; //
	}

	while(pRes->GetRecord())
	{
		SSoulAttrProp stProp = {0};
		stProp.byStarLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StartLevel"));
		stProp.byAttrNum       = SGDP::SDAtou(pRes->GetFieldValueByName("AttrNum"));
		stProp.byKindID          = SGDP::SDAtou(pRes->GetFieldValueByName("AttrKindID"));
		stProp.dwValue         = SGDP::SDAtou(pRes->GetFieldValueByName("AttrValue"));
		m_mapSoulAttrProp[CLogicFunc::Get1688Key(stProp.byAttrNum, stProp.byStarLevel, stProp.byKindID)] = stProp;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

//属性随机范围
BOOL CSoulPropMgr::LoadAttrRandomProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from soulattrrandomprop", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CSoulPropMgr::LoadAttrRandomProp has no data!"), MSG_MARK);
		return TRUE; //
	}

	while (pRes->GetRecord())
	{
		SSoulAttrRandomProp stProp = {0};
		stProp.byStarLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StarLevel"));
		stProp.byAttrNum       = SGDP::SDAtou(pRes->GetFieldValueByName("AttrNum"));
		stProp.byCurAttrIdx          = SGDP::SDAtou(pRes->GetFieldValueByName("CurAttrIdx"));
		stProp.byBeginRandom         = SGDP::SDAtou(pRes->GetFieldValueByName("BeginRandom"));
		stProp.byEndRandom = SGDP::SDAtou(pRes->GetFieldValueByName("EndRandom"));
		
		m_mapSoulAttrRandom[CLogicFunc::Get8888Key(stProp.byStarLevel, stProp.byAttrNum, stProp.byCurAttrIdx, 0)] = stProp;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

//属性条数随机
BOOL CSoulPropMgr::LoadRandomProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from soulrandomprop", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CSoulPropMgr::LoadRandomProp has no data!"), MSG_MARK);
		return TRUE; //
	}

	while (pRes->GetRecord())
	{
		SSoulRandomProp stProp = {0};
		stProp.byStarLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StarLevel"));
		stProp.byCurAttrNum   = SGDP::SDAtou(pRes->GetFieldValueByName("CurAttrNum"));
		stProp.byOneAttrProb          = SGDP::SDAtou(pRes->GetFieldValueByName("OneAttr"));
		stProp.byTwoAttrProb         = SGDP::SDAtou(pRes->GetFieldValueByName("TwoAttr"));
		stProp.byThreeAttrProb = SGDP::SDAtou(pRes->GetFieldValueByName("ThreeAttr"));
		stProp.byFourAttrPropb = SGDP::SDAtou(pRes->GetFieldValueByName("FourAttr"));
		stProp.byFiveAttrPropb  = SGDP::SDAtou(pRes->GetFieldValueByName("FiveAttr"));
		stProp.bySixAttrPropb  = SGDP::SDAtou(pRes->GetFieldValueByName("SixAttr"));

		CRandomMap mapRandom;
		mapRandom[1] = stProp.byOneAttrProb;
		mapRandom[2] = stProp.byTwoAttrProb;
		mapRandom[3] = stProp.byThreeAttrProb;
		mapRandom[4] = stProp.byFourAttrPropb;
		mapRandom[5] = stProp.byFiveAttrPropb;
		mapRandom[6] = stProp.bySixAttrPropb;

		m_mapSoulRandom[CLogicFunc::Get88Key(stProp.byStarLevel, stProp.byCurAttrNum)] = mapRandom;

	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

//获取属性基本值
UINT32 CSoulPropMgr::GetAttrBaseValue(UINT8 byStarLevel, UINT16 wStrengLevel, UINT8 byKindID)
{
	CMapSoulAttrPropItr itr = m_mapSoulAttrProp.find(CLogicFunc::Get1688Key(wStrengLevel, byStarLevel, byKindID));
	if ( itr == m_mapSoulAttrProp.end())
	{
		return 0;
	}
	return itr->second.dwValue;
}
//获取属性条数配置
const CRandomMap* CSoulPropMgr::GetAttrNumRandom(UINT8 byStarLevel, UINT8 byCurNum)
{
	CMapSoulRandomItr itr = m_mapSoulRandom.find(CLogicFunc::Get88Key(byStarLevel, byCurNum));
	if ( itr == m_mapSoulRandom.end())
	{
		return NULL;
	}
	return &itr->second;
}
//获取属性值配置
BOOL CSoulPropMgr::GetAttrValueRandom(UINT8 byStarLevel, UINT8 byAttrNum, UINT8 byCurIdx, const SSoulAttrRandomProp*& pstProp)
{
	CMapSoulAttrRandomItr itr = m_mapSoulAttrRandom.find(CLogicFunc::Get8888Key(byStarLevel, byAttrNum, byCurIdx, 0));
	if ( itr == m_mapSoulAttrRandom.end())
	{
		pstProp = NULL;
		return FALSE;
	}
	pstProp = &itr->second;
	return TRUE;
}

//生成战魂结构体
DT_SOUL_DATA CSoulPropMgr::GenerateSoulData(UINT16 wSoulID)
{
	DT_SOUL_DATA stData = {0};
	stData.wSoulID = wSoulID;
	
	return stData;
}

BOOL CSoulPropMgr::LoadSoulBaseCfg() {
	DB_READ_BEGIN("soulprop");

	while (pRecordSet->GetRecord()) {
		SoulBaseCfg* pSoulBaseCfg = new SoulBaseCfg;
		
		pSoulBaseCfg->unSoulId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("soulid"));
		pSoulBaseCfg->ucStar = SGDP::SDAtou(pRecordSet->GetFieldValueByName("star"));
		pSoulBaseCfg->strSoulName = pRecordSet->GetFieldValueByName("name");
		pSoulBaseCfg->unWeight = SGDP::SDAtou(pRecordSet->GetFieldValueByName("weight"));
		pSoulBaseCfg->ucSoulType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("soultype"));
		pSoulBaseCfg->unDevourExp = SGDP::SDAtou(pRecordSet->GetFieldValueByName("devourexp"));
		pSoulBaseCfg->strSoulPic = pRecordSet->GetFieldValueByName("soulpic");
		pSoulBaseCfg->unEffectId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("effectid"));
		pSoulBaseCfg->ucEffectOdd = SGDP::SDAtou(pRecordSet->GetFieldValueByName("effectodd"));
		pSoulBaseCfg->strNamePic = pRecordSet->GetFieldValueByName("namepic");

		UINT32& rStarWeight = m_mpStarWeight[pSoulBaseCfg->ucStar];
		rStarWeight += pSoulBaseCfg->unWeight;
		
		m_mpSoulCfg.insert(make_pair(pSoulBaseCfg->unSoulId, pSoulBaseCfg));
	}
	DB_READ_END;

	return TRUE;
}

BOOL CSoulPropMgr::LoadSoulProductCfg() {
	DB_READ_BEGIN("soulproductprop");

	while (pRecordSet->GetRecord()) {
		SoulProductCfg* pCfg = new SoulProductCfg;

		pCfg->ucId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("id"));
		pCfg->ucSoulPercent = SGDP::SDAtou(pRecordSet->GetFieldValueByName("soulpercent"));
		pCfg->unMeterialGoodId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("meterialgoodid"));
		
		if (pCfg->ucId < ePST_Low || pCfg->ucId >= ePST_Cnt || pCfg->ucSoulPercent > 100) {
			assert(FALSE); // 
			return FALSE;
		}

		string strPossibleStar = pRecordSet->GetFieldValueByName("possiblestar");
		string strStarWeight = pRecordSet->GetFieldValueByName("starweight");
		StringTool::SDSplitStringToUInt(pCfg->vPossibleStar, strPossibleStar, ',');
		StringTool::SDSplitStringToUInt(pCfg->vStarWeight, strStarWeight, ',');
		if (pCfg->vPossibleStar.empty() || pCfg->vPossibleStar.size() != pCfg->vStarWeight.size()) {
			assert(FALSE);
			return FALSE;
		}

		m_mpProfuctCfg.insert(make_pair(pCfg->ucId, pCfg));
	}

	DB_READ_END;
	return TRUE;
}

BOOL CSoulPropMgr::LoadSoulLvCfg() {
	DB_READ_BEGIN("soulattraddprop");

	while (pRecordSet->GetRecord()) {
		SoulLvCfg* pCfg = new SoulLvCfg;

		pCfg->ucStarLv = SGDP::SDAtou(pRecordSet->GetFieldValueByName("starlv"));
		pCfg->ucStrengthLv = SGDP::SDAtou(pRecordSet->GetFieldValueByName("strengthlv"));
		pCfg->unAddAttrPercent = SGDP::SDAtou(pRecordSet->GetFieldValueByName("attraddpercent"));
		pCfg->unExp = SGDP::SDAtou(pRecordSet->GetFieldValueByName("adddevourexp"));

		m_vecSoulLvCfg.push_back(pCfg);
	}

	DB_READ_END;
	return TRUE;
}

BOOL CSoulPropMgr::LoadSoulExchangeLvCfg() {
	DB_READ_BEGIN("soulexchangeprop");
	while (pRecordSet->GetRecord()) {
		SoulExchangeCfg* pCfg = new SoulExchangeCfg;
		pCfg->unId = SGDP::SDAtou(pRecordSet->GetFieldValueByName("id"));
		pCfg->strName = pRecordSet->GetFieldValueByName("name");
		string strMeterialId = pRecordSet->GetFieldValueByName("meterialid");
		string strMeterialNum = pRecordSet->GetFieldValueByName("meterialnum");
		StringTool::SDSplitStringToUInt(pCfg->vMeterialId, strMeterialId, ',');
		StringTool::SDSplitStringToUInt(pCfg->vMeterialNum, strMeterialNum, ',');
		if (pCfg->vMeterialId.empty() || pCfg->vMeterialId.size() != pCfg->vMeterialNum.size()) {
			assert(FALSE);
			return FALSE;
		}

		string strGetStar = pRecordSet->GetFieldValueByName("getstar");
		string strGetStarWeight = pRecordSet->GetFieldValueByName("getstarweight");
		StringTool::SDSplitStringToUInt(pCfg->vGetStar, strGetStar, ',');
		StringTool::SDSplitStringToUInt(pCfg->vGetStarWeight, strGetStarWeight, ',');
		if (pCfg->vGetStar.empty() || pCfg->vGetStar.size() != pCfg->vGetStarWeight.size()) {
			assert(FALSE);
			return FALSE;
		}

		m_mpSoulExchangeCfg.insert(make_pair(pCfg->unId, pCfg));
	}
	DB_READ_END;
	return TRUE;
}

const SoulProductCfg* CSoulPropMgr::GetSoulProductCfg(UINT8 ucType) {
	MapSoulProductCfg::const_iterator itor = m_mpProfuctCfg.find(ucType);
	if (m_mpProfuctCfg.end() != itor) {
		return itor->second;
	}
	return NULL;
}

DT_PLAYER_RES_DATA& CSoulPropMgr::GetReflashCost()
{
	return m_stReflashCost;
}

UINT8 CSoulPropMgr::GetFreeReflashNum()
{
	return m_byFreeReflashNum;
}

const SoulBaseCfg* CSoulPropMgr::GetSoulBaseCfg(UINT32 unSoulId) {
	MapSoulCfg::const_iterator itor = m_mpSoulCfg.find(unSoulId);
	if (m_mpSoulCfg.end() != itor) {
		return itor->second;
	}
	return NULL;
}

const MapSoulCfg& CSoulPropMgr::GetMapSoulCfg() const {
	return m_mpSoulCfg;
}

const SoulLvCfg* CSoulPropMgr::GetSoulLvCfg(UINT8 ucStarLv, UINT8 ucStrengthLv) {
	for (size_t n = 0; n < m_vecSoulLvCfg.size(); ++n) {
		if (ucStarLv == m_vecSoulLvCfg[n]->ucStarLv
			&& ucStrengthLv == m_vecSoulLvCfg[n]->ucStrengthLv)
		{
			return m_vecSoulLvCfg[n];
		}
	}

	return NULL;
}

UINT8 CSoulPropMgr::GetSoulLv(UINT8 ucStarLv, UINT32 unExp) {
	UINT8 ucLv = 1;
	for (size_t n = 0; n < m_vecSoulLvCfg.size(); ++n) {
		if (ucStarLv == m_vecSoulLvCfg[n]->ucStarLv
			&& unExp >= m_vecSoulLvCfg[n]->unExp
			&& ucLv < m_vecSoulLvCfg[n]->ucStrengthLv)
		{
			ucLv = m_vecSoulLvCfg[n]->ucStrengthLv;
		}
	}

	return ucLv;
}

UINT32 CSoulPropMgr::NextSoulLvExp(UINT8 ucStarLv, UINT8 ucStrengthLv) {
	for (size_t n = 0; n < m_vecSoulLvCfg.size(); ++n) {
		if (ucStarLv == m_vecSoulLvCfg[n]->ucStarLv
			&& (ucStrengthLv + 1) == m_vecSoulLvCfg[n]->ucStrengthLv)
		{
			return m_vecSoulLvCfg[n]->unExp;
		}
	}

	return 0;
}

const SoulExchangeCfg* CSoulPropMgr::GetSoulExchangeCfg(UINT8 unId) {
	MapSoulExchangeCfg::const_iterator itor = m_mpSoulExchangeCfg.find(unId);
	if (m_mpSoulExchangeCfg.end() != itor) {
		return itor->second;
	}

	return NULL;
}

const MapSoulExchangeCfg& CSoulPropMgr::GetSoulExchangeAllCfg() {
	return m_mpSoulExchangeCfg;
}

BOOL CSoulPropMgr::GetSoulPotencyProp(CPlayer* poPlayer, UINT16 wSoulID, UINT8 byLevel, DT_POTENCY_ATTR_DATA_LST& stAttrData  )
{
	if ( NULL == poPlayer)
	{
		return FALSE;
	}
	CBag& oBag = poPlayer->GetBag();

	UINT32 byMinKey = CLogicFunc::Get1688Key(wSoulID, byLevel, 0);
	UINT32 byMaxKey = CLogicFunc::Get1688Key(wSoulID, byLevel, 0xFF);
	UINT8 byIdx = 0;
	for ( CMapSoulPotencyPropItr itr = m_mapSoulPotencyProp.lower_bound(byMinKey); itr != m_mapSoulPotencyProp.upper_bound(byMaxKey); itr++, byIdx++)
	{
		for ( UINT8 byAttrIdx = 0; byAttrIdx < itr->second.vecPotency.size(); byAttrIdx++)
		{
			stAttrData.astAttrValue[byIdx].astAttrInfo[byAttrIdx] = itr->second.vecPotency[byAttrIdx];
		}
		stAttrData.astAttrValue[byIdx].byAttrNum = itr->second.vecPotency.size();
		
		//需要等级
		stAttrData.astAttrValue[byIdx].wNeedSoulLevel = itr->second.wNeedSoulLevel;
		//材料
		DT_ITEM_EXCHANGE_INFO& stItemExchange = stAttrData.astAttrValue[byIdx].stItemsInfo;
		UINT8 byItemNum = 0;
		for ( vector<DT_ITEM_DATA>::iterator ItemItr = itr->second.vecItemData.begin(); ItemItr != itr->second.vecItemData.end(); ItemItr++, byItemNum++)
		{
			UINT16 wCurItemNum = 0;
			CGoods* poGoods = oBag.GetGoodsItem( ItemItr->wKindID);
			if ( NULL != poGoods)
			{
				wCurItemNum = poGoods->GetCurPileNum();
			}
			CItemMgr::Instance()->GetDT_ITEM_DATA_CLI3(ItemItr->wKindID, wCurItemNum, stItemExchange.astItemList[byItemNum]);
			stItemExchange.awItemNeedNum[byItemNum] = ItemItr->wNum;
		}
		stItemExchange.byItemNum = byItemNum;

	}
	stAttrData.byNum = byIdx;
	if ( 0 == byIdx)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSoulPropMgr::GetSoulPotencyCountAttr(UINT16 wSoulID, UINT8 byLevel, UINT8 bySubLevel, DT_POTENCY_ATTR_LST_INFO& stAttrLstInfo)
{
	CMapSoulPotencyPropItr itr = m_mapSoulPotencyProp.find(CLogicFunc::Get1688Key(wSoulID, byLevel, bySubLevel));
	if ( itr == m_mapSoulPotencyProp.end())
	{
		return FALSE;
	}
	UINT8 byIdx = 0;
	for ( C3232MapItr mapItr = itr->second.mapCountAttrValue.begin(); mapItr != itr->second.mapCountAttrValue.end(); mapItr++, byIdx++)
	{
		stAttrLstInfo.astAttrInfo[byIdx].byPotencyID = mapItr->first;
		stAttrLstInfo.astAttrInfo[byIdx].dwPotencyValue = mapItr->second;
	}
	stAttrLstInfo.byAttrNum = byIdx;
	return TRUE;
}

BOOL CSoulPropMgr::GetSoulPotencyCountAttr(UINT16 wSoulID, UINT8 byLevel, UINT8 bySubLevel, C3232Map &poAttrMap)
{
	CMapSoulPotencyPropItr itr = m_mapSoulPotencyProp.find(CLogicFunc::Get1688Key(wSoulID, byLevel, bySubLevel));
	if ( itr == m_mapSoulPotencyProp.end())
	{
		return FALSE;
	}
	poAttrMap = itr->second.mapCountAttrValue;
	return TRUE;
}

SSoulPotencyProp* CSoulPropMgr::GetSoulPotencyProp(UINT16 wSoulID, UINT8 byLevel, UINT8 bySubLevel)
{
	CMapSoulPotencyPropItr itr = m_mapSoulPotencyProp.find(CLogicFunc::Get1688Key(wSoulID, byLevel, bySubLevel));
	if ( itr == m_mapSoulPotencyProp.end())
	{
		return NULL;
	}
	return &itr->second;
}