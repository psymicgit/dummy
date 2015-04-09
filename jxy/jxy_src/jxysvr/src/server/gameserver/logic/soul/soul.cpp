#include "logic/soul/soul.h"
#include "logic/soul/soulpropmgr.h"
#include "logic/specialeffects/specialeffects.h"
#include "logic/specialeffects/specialeffectsmgr.h"
#include "logic/player/player.h"
IMPLEMENT_CLASS_POOL_MGR(CSoul)
CSoul::CSoul() 
{
	Clear();
}

CSoul::~CSoul() {
	Clear();
}

BOOL CSoul::Init(CPlayer* poPlayer, const DT_SOUL_DATA& stData, const DT_SOUL_POTENCY_DATA& stSoulPotency, CBattleAttrObj* poMySelf) {
	
	memset(&m_stSoulData, 0, sizeof(DT_SOUL_DATA));
	if (NULL == poPlayer) {
		return FALSE;
	}
	m_poOwner = poPlayer;
	m_stSoulData = stData;
	m_stSoulPotencyData = stSoulPotency;

	//重新赋值技能
	if( m_stSoulData.byHaveEffect )
	{
		const SoulBaseCfg* pBaseSoulCfg = CSoulPropMgr::Instance()->GetSoulBaseCfg(m_stSoulData.wSoulID);
		if (NULL == pBaseSoulCfg) {
			PRINT_ERR_INFO;
			return TRUE;
		}
		CSpecialEffectPropMgr::Instance()->GenerateSpecialEffectData( pBaseSoulCfg->unEffectId, m_stSoulData.stSpecialEffect);
		for ( UINT8 byIdx = 0; byIdx < m_stSoulData.stSpecialEffect.bySkillNum && byIdx < MAX_SKILL_SOUL_NUM; byIdx++)
		{
			const SSpecialEffectProp* poProp = CSpecialEffectPropMgr::Instance()->GetSpecialEffectProp(m_stSoulData.stSpecialEffect.wEffectID, m_stSoulData.stSpecialEffect.abyEffectSmallType[byIdx]);
			if ( NULL != poProp)
			{
				if ( poProp->byUnlockLevel <= m_stSoulData.bySoulLevel)
				{
					m_stSoulData.stSpecialEffect.byIsActivity = ESM_YES;
				}

			}
		}

	}
	return TRUE;
}

DT_SOUL_DATA& CSoul::GetDT_SOUL_DATA() {
	
	return m_stSoulData;
}

DT_SOUL_POTENCY_DATA& CSoul::GetDT_SOUL_POTENCY_DATA()
{
	return m_stSoulPotencyData;
}

BOOL CSoul::GetSoulPotencyCli(DT_SOUL_POTENCY_CLI& stCli)
{
	stCli.byPotencyLevel = m_stSoulPotencyData.byPotencyLevel;
	stCli.byPotencySubLevel = m_stSoulPotencyData.byPotencySubLevel;
	return TRUE;
}

BOOL CSoul::GetSoulCli(DT_SOUL_DATA_CLI& stCli) {
	stCli.wSoulID = m_stSoulData.wSoulID;
	stCli.dwSoulIdx = m_stSoulData.dwSoulIdx;
	stCli.bySoulStar = m_stSoulData.bySoulStar;
	stCli.bySoulType = m_stSoulData.bySoulType;
	stCli.bySoulLevel = m_stSoulData.bySoulLevel;

	stCli.byIsNormalSoul = m_stSoulData.byIsNormalSoul;

	const SoulLvCfg* pSoulLv = CSoulPropMgr::Instance()->GetSoulLvCfg(m_stSoulData.bySoulStar, m_stSoulData.bySoulLevel);
	if (NULL == pSoulLv) {
		return FALSE;
	}

	if (eNormalSoul == stCli.byIsNormalSoul) {
		UINT32 unNextExp = CSoulPropMgr::Instance()->NextSoulLvExp(stCli.bySoulStar, stCli.bySoulLevel);
		if (0 == unNextExp) {
			stCli.dwMaxExperience = 0;
		} else {
			stCli.dwMaxExperience = unNextExp - pSoulLv->unExp;
		}
	} else {
		stCli.dwMaxExperience = 0;
	}
	stCli.dwSoulExperience = m_stSoulData.dwSoulExperience - pSoulLv->unExp;
	stCli.dwReturnExperience = m_stSoulData.dwDevourExp + m_stSoulData.dwSoulExperience;
	
	const SoulBaseCfg* pSoulCfg = CSoulPropMgr::Instance()->GetSoulBaseCfg(stCli.wSoulID);
	if (NULL != pSoulCfg) {
		if (pSoulCfg->strSoulPic.empty()
			|| pSoulCfg->strSoulPic.size() >= MAX_SOUL_PIC_LEN) 
		{
			strcpy(stCli.aszSoulPic, "wuhun.png");
		} else {
			strcpy(stCli.aszSoulPic, pSoulCfg->strSoulPic.c_str());
		}

		if (!pSoulCfg->strSoulName.empty()
			&& pSoulCfg->strSoulName.size() < MAX_SOUL_NAME_LEN) 
		{
			strcpy(stCli.aszSoulName, pSoulCfg->strSoulName.c_str());
		}

		if (!pSoulCfg->strNamePic.empty()
			&& pSoulCfg->strNamePic.size() < MAX_SOUL_PIC_LEN) 
		{
			strcpy(stCli.aszNamePic, pSoulCfg->strNamePic.c_str());
		}
	}

	//真实数据
	stCli.byBaseNum = m_stSoulData.byBaseNum;
	UINT8 byIdx = 0;
	for (byIdx = 0; byIdx < m_stSoulData.byBaseNum; byIdx++) {
		stCli.astSoulAttr[byIdx].byKindID = m_stSoulData.astSoulAttr[byIdx].byKindID;
		stCli.astSoulAttr[byIdx].dwKindValue = (UINT32)(m_stSoulData.astSoulAttr[byIdx].dwKindValue / float(((float)100 + (float)pSoulLv->unAddAttrPercent) / (float)100));
		stCli.astSoulAttr[byIdx].dwKindPercent = pSoulLv->unAddAttrPercent;
	}

	//洗练后的数据
	stCli.byBaseAfterNum = m_stSoulData.byBaseAfterNum;
	for (byIdx = 0; byIdx < m_stSoulData.byBaseAfterNum; byIdx++) {
		stCli.astSoulAfterAttr[byIdx].byKindID = m_stSoulData.astSoulAfterAttr[byIdx].byKindID;
		stCli.astSoulAfterAttr[byIdx].dwKindValue = (UINT32)(m_stSoulData.astSoulAfterAttr[byIdx].dwKindValue / float(((float)100 + (float)pSoulLv->unAddAttrPercent) / (float)100));
		stCli.astSoulAfterAttr[byIdx].dwKindPercent = pSoulLv->unAddAttrPercent;
	}

	memcpy(stCli.abySoulAttrLock, m_stSoulData.abySoulAttrLock, sizeof(stCli.abySoulAttrLock));
	byIdx = 0;
	for ( ; byIdx < m_stSoulData.stSpecialEffect.bySkillNum; byIdx++)
	{
		const SSpecialEffectProp* poProp = CSpecialEffectPropMgr::Instance()->GetSpecialEffectProp(m_stSoulData.stSpecialEffect.wEffectID, m_stSoulData.stSpecialEffect.abyEffectSmallType[byIdx]);
		if ( NULL != poProp)
		{
			SDStrncpy(stCli.stSpecialEffect.aszEffectName, poProp->strEffectName.c_str(), MAX_NAME_LEN - 1);
			stCli.stSpecialEffect.byUnlockLevel = poProp->byUnlockLevel;
			stCli.stSpecialEffect.byIsActivity = m_stSoulData.stSpecialEffect.byIsActivity;
			stCli.stSpecialEffect.wEffectID = m_stSoulData.stSpecialEffect.wEffectID;
		}
		CSpecialEffectPropMgr::Instance()->GetSpecialEffectDataCli(m_stSoulData.stSpecialEffect.wEffectID, m_stSoulData.stSpecialEffect.abyEffectSmallType[byIdx], stCli.stSpecialEffect.stSkillLstDesc.astSkillDesc[byIdx]);
		//USR_INFO("[%s:%u] PlayerID = %u, wEffectID = %u, SmallType = %u, EffectType = %u", MSG_MARK, m_poOwner->GetID(), m_stSoulData.stSpecialEffect.wEffectID, m_stSoulData.stSpecialEffect.abyEffectSmallType[byIdx], stCli.stSpecialEffect.stSkillLstDesc.astSkillDesc[byIdx].byEffectType);
	}
	stCli.stSpecialEffect.stSkillLstDesc.byDescNum = byIdx;

	return TRUE;
}

UINT32 CSoul::GetExperience() {
	return m_stSoulData.dwDevourExp + m_stSoulData.dwSoulExperience;
}

BOOL CSoul::AddExeprience(UINT64 qwExperience) {
	m_stSoulData.dwSoulExperience += (UINT32)qwExperience;
	m_stSoulData.bySoulLevel = CSoulPropMgr::Instance()->GetSoulLv(m_stSoulData.bySoulStar, m_stSoulData.dwSoulExperience);

	for (UINT8 n = 0; n < m_stSoulData.byBaseNum; n ++) {
		DT_SOUL_ATTR_DATA& rAttrData = m_stSoulData.astSoulAttr[n];

		UINT32 unAttrBaseVal = CSoulPropMgr::Instance()->GetAttrBaseValue(m_stSoulData.bySoulStar, m_stSoulData.byBaseNum, rAttrData.byKindID);
		rAttrData.dwKindValue = unAttrBaseVal * rAttrData.byKindPrecent / 100;
		const SoulLvCfg* pSoulLv = CSoulPropMgr::Instance()->GetSoulLvCfg(m_stSoulData.bySoulStar, m_stSoulData.bySoulLevel);
		if (NULL == pSoulLv) {
			continue;
		}
		rAttrData.dwKindValue = (UINT32)(rAttrData.dwKindValue * float(((float)100 + (float)pSoulLv->unAddAttrPercent) / (float)100));
	}

	for ( UINT8 byIdx = 0; byIdx < m_stSoulData.stSpecialEffect.bySkillNum && byIdx < MAX_SKILL_SOUL_NUM; byIdx++)
	{
		const SSpecialEffectProp* poProp = CSpecialEffectPropMgr::Instance()->GetSpecialEffectProp(m_stSoulData.stSpecialEffect.wEffectID, m_stSoulData.stSpecialEffect.abyEffectSmallType[byIdx]);
		if ( NULL != poProp)
		{
			if ( poProp->byUnlockLevel <= m_stSoulData.bySoulLevel)
			{
				m_stSoulData.stSpecialEffect.byIsActivity = ESM_YES;
			}
			
		}
	}
	return TRUE;
}

VOID CSoul::Clear() {
	memset(&m_stSoulData, 0, sizeof(m_stSoulData));
	memset(&m_stSoulPotencyData, 0, sizeof(m_stSoulPotencyData));
}

BOOL CSoul::SaveReflash(BOOL bIsSave) {
	if ( 0 == m_stSoulData.byBaseAfterNum) {
		return FALSE;
	}
	if (bIsSave) {
		memcpy(m_stSoulData.astSoulAttr, m_stSoulData.astSoulAfterAttr, sizeof(m_stSoulData.astSoulAttr));
		m_stSoulData.byBaseNum = m_stSoulData.byBaseAfterNum;
	}
	
	memset(m_stSoulData.astSoulAfterAttr, 0, sizeof(m_stSoulData.astSoulAfterAttr));
	m_stSoulData.byBaseAfterNum = 0;
	return TRUE;
}

BOOL CSoul::GetSoulPotencyDetailCli(DT_POTENCY_DATA_LST& stLstData)
{
	UINT8 byPotencyLevel = 1;
	while( CSoulPropMgr::Instance()->GetSoulPotencyProp( m_poOwner, m_stSoulData.wSoulID, byPotencyLevel, stLstData.astPotencyLst[byPotencyLevel - 1].stPotencyInfo) && byPotencyLevel < MAX_SOUL_POTENCY_LEVEL )
	{
		stLstData.astPotencyLst[byPotencyLevel - 1].byPotencyLevel = byPotencyLevel;
		byPotencyLevel++;
	}
	stLstData.byPotencyNum = byPotencyLevel - 1;
	return TRUE;
}

BOOL CSoul::GetSoulPotencyAddAttrInfo(DT_POTENCY_ATTR_LST_INFO& stAddAttr)
{
	CSoulPropMgr::Instance()->GetSoulPotencyCountAttr( m_stSoulData.wSoulID, m_stSoulPotencyData.byPotencyLevel, m_stSoulPotencyData.byPotencySubLevel, stAddAttr);
	return TRUE;
}

eSoulUpdatePotency CSoul::UpdatePotency(DT_ITEM_NUM_DATA_LIST_CLI& stItemInfo)
{
	const SoulBaseCfg* poBaseSoulCfg = CSoulPropMgr::Instance()->GetSoulBaseCfg(m_stSoulData.wSoulID);
	if (NULL == poBaseSoulCfg) 
	{	
		return ESP_NOT_CONFIG;
	}
	UINT8 byNextLevel = 0;
	UINT8 byNextSubLevel = 0;
	if ( 0 == m_stSoulPotencyData.byPotencyLevel )
	{
		byNextLevel = 1;
		byNextSubLevel = 1;
	}
	else
	{
		byNextLevel = m_stSoulPotencyData.byPotencyLevel;
		byNextSubLevel = m_stSoulPotencyData.byPotencySubLevel + 1;
		if ( byNextSubLevel > CSoulPropMgr::Instance()->GetMaxPotencySubLevel())
		{
			byNextLevel += 1;
			byNextSubLevel = 1;
		}
	}

	SSoulPotencyProp* poPotencyProp = CSoulPropMgr::Instance()->GetSoulPotencyProp( m_stSoulData.wSoulID, byNextLevel, byNextSubLevel);
	if ( NULL == poPotencyProp )
	{
		return ESP_MAX_LEVEL;
	}

	if ( m_stSoulData.bySoulLevel < poPotencyProp->wNeedSoulLevel)
	{
		return ESP_LEVEL_NOT_ENOUGH;
	}

	for ( vector<DT_ITEM_DATA>::iterator itr = poPotencyProp->vecItemData.begin(); itr != poPotencyProp->vecItemData.end(); itr++)
	{
		CGoods* poGoods = m_poOwner->GetBag().GetGoodsItem( itr->wKindID );
		if ( NULL == poGoods)
		{
			return ESP_ITEM_NOT_ENOUGH;
		}
		if ( poGoods->GetCurPileNum() < itr->wNum)
		{
			return ESP_ITEM_NOT_ENOUGH;
		}
	}

	m_stSoulPotencyData.byPotencyLevel = byNextLevel;
	m_stSoulPotencyData.byPotencySubLevel = byNextSubLevel;

	UINT8 byIdx = 0;
	for ( vector<DT_ITEM_DATA>::iterator itr = poPotencyProp->vecItemData.begin(); itr != poPotencyProp->vecItemData.end(); itr++, byIdx++)
	{
		CGoods* poGoods = m_poOwner->GetBag().GetGoodsItem( itr->wKindID );
		if ( NULL == poGoods)
		{
			return ESP_ITEM_NOT_ENOUGH;
		}
		poGoods->DecPileNum( itr->wNum, CRecordMgr::EDIT_UPDATE_POTENCY_SOUL, m_stSoulData.wSoulID);
		stItemInfo.astItemList[byIdx].wItemID = itr->wKindID;
		stItemInfo.astItemList[byIdx].wItemNum = poGoods->GetCurPileNum();
		if ( 0 == poGoods->GetCurPileNum())
		{
			m_poOwner->OnDeleteItem(poGoods);//清除
		}
	}
	stItemInfo.wItemNum = byIdx;

	return ESP_SUCCESS;
}
