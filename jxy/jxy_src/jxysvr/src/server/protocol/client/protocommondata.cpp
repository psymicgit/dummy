#include "protocommondata.h"
CProtoCommonData::CProtoCommonData()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoCommonData * CProtoCommonData::Instance()
{
   static CProtoCommonData oInst;
   return &oInst;
}

CProtoCommonData::~CProtoCommonData()
{
}

INT32 CProtoCommonData::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 2);

	EnFuncCProtoCommonData pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoCommonData::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 2);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoCommonData pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoCommonData CProtoCommonData::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCOMMONDATA_MSGID_OFFSET >= PROTOCOMMONDATA_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOCOMMONDATA_MSGID_OFFSET];
}

DeFuncCProtoCommonData CProtoCommonData::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCOMMONDATA_MSGID_OFFSET >= PROTOCOMMONDATA_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOCOMMONDATA_MSGID_OFFSET];
}

BOOL CProtoCommonData::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	return true;
}

INT32 EncodeDT_GUIDE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_GUIDE_RECORD_DATA *pstIn = (DT_GUIDE_RECORD_DATA*)pHost;

	INT32 i;
	for(i = 0; i < MAX_GUIDE_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyGuideRecordData[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GUIDE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_GUIDE_RECORD_DATA *pstOut = (DT_GUIDE_RECORD_DATA*)pHost;

	INT32 i;
	for(i = 0; i < MAX_GUIDE_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyGuideRecordData[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_BASE_DATA *pstIn = (DT_PLAYER_BASE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDispName, sizeof(pstIn->aszDispName)))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwCoin))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwGold))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStory))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPhyStrength))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwScience))
		return -1;

	if(-1 == EncodeDT_GUIDE_RECORD_DATA(&pstIn->stGuideRecordData, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byRobotFunc))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPower))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFirstAttack))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwTodayFirstLoginTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBagOpenNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBlueGas))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwPurpleGas))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byVipLevel))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwTotalAmount))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwJingJie))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwTotalRMB))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStudyExp))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRecvRankEncTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_BASE_DATA *pstOut = (DT_PLAYER_BASE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDispName, sizeof(pstOut->aszDispName)))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwCoin))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwGold))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStory))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPhyStrength))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwScience))
		return -1;

	if(-1 == DecodeDT_GUIDE_RECORD_DATA(&pstOut->stGuideRecordData, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byRobotFunc))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPower))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFirstAttack))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwTodayFirstLoginTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBagOpenNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBlueGas))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwPurpleGas))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byVipLevel))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwTotalAmount))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwJingJie))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwTotalRMB))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStudyExp))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRecvRankEncTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ATTRIBUTE *pstIn = (DT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwHP))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAttack))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ATTRIBUTE *pstOut = (DT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwHP))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAttack))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_HIDE_BATTLE_ATTRIBUTE *pstIn = (DT_HIDE_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHitRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDodgeRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCritRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDeCritRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAngryValue))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_HIDE_BATTLE_ATTRIBUTE *pstOut = (DT_HIDE_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHitRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDodgeRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCritRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDeCritRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAngryValue))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_EXT_BATTLE_ATTRIBUTE *pstIn = (DT_EXT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wIncHurtRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDecHurtRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wOddAngry))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFirstAttack))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCoachTalent))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_EXT_BATTLE_ATTRIBUTE *pstOut = (DT_EXT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wIncHurtRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDecHurtRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wOddAngry))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFirstAttack))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCoachTalent))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_UPDATE_ATTR_DATA *pstIn = (DT_HERO_UPDATE_ATTR_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wAttrHP))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttrAttack))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_UPDATE_ATTR_DATA *pstOut = (DT_HERO_UPDATE_ATTR_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wAttrHP))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttrAttack))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_HERO_ATTR_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_ATTR_DATA *pstIn = (DT_HERO_ATTR_DATA*)pHost;

	if(-1 == EncodeDT_BATTLE_ATTRIBUTE(&pstIn->stBattleAttribute, poNetData))
		return -1;

	if(-1 == EncodeDT_HIDE_BATTLE_ATTRIBUTE(&pstIn->stHideBattleAttr, poNetData))
		return -1;

	if(-1 == EncodeDT_HERO_UPDATE_ATTR_DATA(&pstIn->stUpdateAttrInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_ATTR_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_ATTR_DATA *pstOut = (DT_HERO_ATTR_DATA*)pHost;

	if(-1 == DecodeDT_BATTLE_ATTRIBUTE(&pstOut->stBattleAttribute, poNetData))
		return -1;

	if(-1 == DecodeDT_HIDE_BATTLE_ATTRIBUTE(&pstOut->stHideBattleAttr, poNetData))
		return -1;

	if(-1 == DecodeDT_HERO_UPDATE_ATTR_DATA(&pstOut->stUpdateAttrInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_BASE_DATA *pstIn = (DT_HERO_BASE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCoach))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPos))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wTalent))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwExperience))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byRecuitedFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byVisitNum))
		return -1;

	if(-1 == EncodeDT_BATTLE_ATTRIBUTE(&pstIn->stBattleAttribute, poNetData))
		return -1;

	if(-1 == EncodeDT_HIDE_BATTLE_ATTRIBUTE(&pstIn->stHideBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastUseDrugLevel))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastUseDrugTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUseExperienceDrugPerDay))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastUseExperienceDrugTime))
		return -1;

	if(-1 == EncodeDT_HERO_UPDATE_ATTR_DATA(&pstIn->stUpdateAttrInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_BASE_DATA *pstOut = (DT_HERO_BASE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCoach))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPos))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wTalent))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwExperience))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byRecuitedFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byVisitNum))
		return -1;

	if(-1 == DecodeDT_BATTLE_ATTRIBUTE(&pstOut->stBattleAttribute, poNetData))
		return -1;

	if(-1 == DecodeDT_HIDE_BATTLE_ATTRIBUTE(&pstOut->stHideBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastUseDrugLevel))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastUseDrugTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUseExperienceDrugPerDay))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastUseExperienceDrugTime))
		return -1;

	if(-1 == DecodeDT_HERO_UPDATE_ATTR_DATA(&pstOut->stUpdateAttrInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_XIANGQIAN_DATA(void *pHost, CNetData* poNetData)
{
	DT_XIANGQIAN_DATA *pstIn = (DT_XIANGQIAN_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wItemID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_XIANGQIAN_DATA(void *pHost, CNetData* poNetData)
{
	DT_XIANGQIAN_DATA *pstOut = (DT_XIANGQIAN_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wItemID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_DATA *pstIn = (DT_EQUIP_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBagPos))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwItemNewTime))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_XIANGQIAN_NUM; i++)
	{
		if(-1 == EncodeDT_XIANGQIAN_DATA(&pstIn->astXiangqianList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_DATA *pstOut = (DT_EQUIP_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBagPos))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwItemNewTime))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_XIANGQIAN_NUM; i++)
		{
			if(-1 == DecodeDT_XIANGQIAN_DATA(&pstOut->astXiangqianList[i], poNetData))
				return -1;
		}

	return sizeof(*pstOut);
}

INT32 EncodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_HERO_EQUIP_DATA_LST *pstIn = (DT_HERO_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < EQUIP_POS_NUM; i++)
	{
		if(i >= (INT32)pstIn->wEquipNum)
			break;
		if(-1 == EncodeDT_EQUIP_DATA(&pstIn->astEquipList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_HERO_EQUIP_DATA_LST *pstOut = (DT_HERO_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < EQUIP_POS_NUM; i++)
	{
		if(i >= (INT32)pstOut->wEquipNum)
			break;
		if(-1 == DecodeDT_EQUIP_DATA(&pstOut->astEquipList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_EQUIP_DATA_LST *pstIn = (DT_BAG_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstIn->wEquipNum)
			break;
		if(-1 == EncodeDT_EQUIP_DATA(&pstIn->astEquipList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_EQUIP_DATA_LST *pstOut = (DT_BAG_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstOut->wEquipNum)
			break;
		if(-1 == DecodeDT_EQUIP_DATA(&pstOut->astEquipList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_GOODS_DATA(void *pHost, CNetData* poNetData)
{
	DT_GOODS_DATA *pstIn = (DT_GOODS_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPileCount))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastUseTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastUseNum))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBagPos))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwItemNewTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GOODS_DATA(void *pHost, CNetData* poNetData)
{
	DT_GOODS_DATA *pstOut = (DT_GOODS_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPileCount))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastUseTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastUseNum))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBagPos))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwItemNewTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_GOODS_DATA_LST *pstIn = (DT_BAG_GOODS_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wGoodsNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstIn->wGoodsNum)
			break;
		if(-1 == EncodeDT_GOODS_DATA(&pstIn->astGoodsList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_GOODS_DATA_LST *pstOut = (DT_BAG_GOODS_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wGoodsNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstOut->wGoodsNum)
			break;
		if(-1 == DecodeDT_GOODS_DATA(&pstOut->astGoodsList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_DRUG_DATA(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA *pstIn = (DT_DRUG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRUG_DATA(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA *pstOut = (DT_DRUG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA_LST *pstIn = (DT_DRUG_DATA_LST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byDrugLevelNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DRUG_LEVEL; i++)
	{
		if(i >= (INT32)pstIn->byDrugLevelNum)
			break;
		if(-1 == EncodeDT_DRUG_DATA(&pstIn->astDrugList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA_LST *pstOut = (DT_DRUG_DATA_LST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byDrugLevelNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DRUG_LEVEL; i++)
	{
		if(i >= (INT32)pstOut->byDrugLevelNum)
			break;
		if(-1 == DecodeDT_DRUG_DATA(&pstOut->astDrugList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData)
{
	DT_GODWEAPON_DATA *pstIn = (DT_GODWEAPON_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wQuality))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData)
{
	DT_GODWEAPON_DATA *pstOut = (DT_GODWEAPON_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wQuality))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_RANG_DATA *pstIn = (DT_ATTACK_RANG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttackRangKind))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTraceBuffKind))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_RANG_DATA *pstOut = (DT_ATTACK_RANG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttackRangKind))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTraceBuffKind))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SKILL_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA *pstIn = (DT_SKILL_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wActionID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wActionLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wEffectID))
		return -1;

	if(-1 == EncodeDT_ATTACK_RANG_DATA(&pstIn->stAttackRangInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwActionExp))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA *pstOut = (DT_SKILL_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wActionID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wActionLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wEffectID))
		return -1;

	if(-1 == DecodeDT_ATTACK_RANG_DATA(&pstOut->stAttackRangInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwActionExp))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SKILL_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA_LST *pstIn = (DT_SKILL_DATA_LST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->bySkillNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SKILL_SLOT_NUM; i++)
	{
		if(i >= (INT32)pstIn->bySkillNum)
			break;
		if(-1 == EncodeDT_SKILL_DATA(&pstIn->astSkillInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA_LST *pstOut = (DT_SKILL_DATA_LST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->bySkillNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SKILL_SLOT_NUM; i++)
	{
		if(i >= (INT32)pstOut->bySkillNum)
			break;
		if(-1 == DecodeDT_SKILL_DATA(&pstOut->astSkillInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_STUDY_DATA(void *pHost, CNetData* poNetData)
{
	DT_STUDY_DATA *pstIn = (DT_STUDY_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byStudyColorKind))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byStudyAttrKind1))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byStudyLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byStudyAttrKind2))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STUDY_DATA(void *pHost, CNetData* poNetData)
{
	DT_STUDY_DATA *pstOut = (DT_STUDY_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byStudyColorKind))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byStudyAttrKind1))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byStudyLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byStudyAttrKind2))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_AWAKEN_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_BASE_DATA *pstIn = (DT_AWAKEN_BASE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGoldFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStudyExp))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHaveCallNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwlastCallTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStudyClip))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_AWAKEN_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_BASE_DATA *pstOut = (DT_AWAKEN_BASE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGoldFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStudyExp))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHaveCallNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwlastCallTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStudyClip))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_AWAKEN_BASE_DATA1(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_BASE_DATA1 *pstIn = (DT_AWAKEN_BASE_DATA1*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGoldFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStudyExp))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHaveCallNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwlastCallTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_AWAKEN_BASE_DATA1(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_BASE_DATA1 *pstOut = (DT_AWAKEN_BASE_DATA1*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGoldFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStudyExp))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHaveCallNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwlastCallTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_DATA *pstIn = (DT_AWAKEN_DATA*)pHost;

	if(-1 == EncodeDT_AWAKEN_BASE_DATA(&pstIn->stBaseInfo, poNetData))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_STUDY_GRID_NUM; i++)
	{
		if(-1 == EncodeDT_STUDY_DATA(&pstIn->astBagStudyInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_DATA *pstOut = (DT_AWAKEN_DATA*)pHost;

	if(-1 == DecodeDT_AWAKEN_BASE_DATA(&pstOut->stBaseInfo, poNetData))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_BAG_STUDY_GRID_NUM; i++)
		{
			if(-1 == DecodeDT_STUDY_DATA(&pstOut->astBagStudyInfo[i], poNetData))
				return -1;
		}

	return sizeof(*pstOut);
}

INT32 EncodeDT_AWAKEN_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_DATA_LST *pstIn = (DT_AWAKEN_DATA_LST*)pHost;

	INT32 i;
	for(i = 0; i < MAX_HERO_STUDY_GRID_NUM; i++)
	{
		if(-1 == EncodeDT_STUDY_DATA(&pstIn->astStudyInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_AWAKEN_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_DATA_LST *pstOut = (DT_AWAKEN_DATA_LST*)pHost;

	INT32 i;
		for(i = 0; i < MAX_HERO_STUDY_GRID_NUM; i++)
		{
			if(-1 == DecodeDT_STUDY_DATA(&pstOut->astStudyInfo[i], poNetData))
				return -1;
		}

	return sizeof(*pstOut);
}

INT32 EncodeDT_ABSORB_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_ABSORB_DATA_LST *pstIn = (DT_ABSORB_DATA_LST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAbsorbNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ABSORB_NUM; i++)
	{
		if(i >= (INT32)pstIn->byAbsorbNum)
			break;
		if(-1 == poNetData->AddWord(pstIn->awKindIDLst[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wAbsorbKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ABSORB_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_ABSORB_DATA_LST *pstOut = (DT_ABSORB_DATA_LST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAbsorbNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ABSORB_NUM; i++)
	{
		if(i >= (INT32)pstOut->byAbsorbNum)
			break;
		if(-1 == poNetData->DelWord(pstOut->awKindIDLst[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wAbsorbKindID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA *pstIn = (DT_HERO_DATA*)pHost;

	if(-1 == EncodeDT_HERO_BASE_DATA(&pstIn->stHeroBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_HERO_EQUIP_DATA_LST(&pstIn->stEquipDataInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_DRUG_DATA_LST(&pstIn->stDrugDataInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_GODWEAPON_DATA(&pstIn->stGodweaponInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_SKILL_DATA_LST(&pstIn->stSkillInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_AWAKEN_DATA_LST(&pstIn->stStudyInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_ABSORB_DATA_LST(&pstIn->stAbsorbInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA *pstOut = (DT_HERO_DATA*)pHost;

	if(-1 == DecodeDT_HERO_BASE_DATA(&pstOut->stHeroBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_HERO_EQUIP_DATA_LST(&pstOut->stEquipDataInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_DRUG_DATA_LST(&pstOut->stDrugDataInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_GODWEAPON_DATA(&pstOut->stGodweaponInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_SKILL_DATA_LST(&pstOut->stSkillInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_AWAKEN_DATA_LST(&pstOut->stStudyInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_ABSORB_DATA_LST(&pstOut->stAbsorbInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA_LIST *pstIn = (DT_HERO_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_HERO_NUM; i++)
	{
		if(i >= (INT32)pstIn->wHeroNum)
			break;
		if(-1 == EncodeDT_HERO_DATA(&pstIn->astHeroInfoLst[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA_LIST *pstOut = (DT_HERO_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_HERO_NUM; i++)
	{
		if(i >= (INT32)pstOut->wHeroNum)
			break;
		if(-1 == DecodeDT_HERO_DATA(&pstOut->astHeroInfoLst[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUILD_BASE_DATA *pstIn = (DT_BUILD_BASE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwValue1))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwValue2))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byValue3))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySort))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHaveOpenFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUILD_BASE_DATA *pstOut = (DT_BUILD_BASE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwValue1))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwValue2))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byValue3))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySort))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHaveOpenFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_BUILD_DATA_LIST *pstIn = (DT_BUILD_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wBuildNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUILD_NUM; i++)
	{
		if(i >= (INT32)pstIn->wBuildNum)
			break;
		if(-1 == EncodeDT_BUILD_BASE_DATA(&pstIn->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_BUILD_DATA_LIST *pstOut = (DT_BUILD_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wBuildNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUILD_NUM; i++)
	{
		if(i >= (INT32)pstOut->wBuildNum)
			break;
		if(-1 == DecodeDT_BUILD_BASE_DATA(&pstOut->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_ID *pstIn = (DT_INSTANCE_ID*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_ID *pstOut = (DT_INSTANCE_ID*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA *pstIn = (DT_INSTANCE_DATA*)pHost;

	if(-1 == EncodeDT_INSTANCE_ID(&pstIn->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastPassTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byScore))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA *pstOut = (DT_INSTANCE_DATA*)pHost;

	if(-1 == DecodeDT_INSTANCE_ID(&pstOut->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastPassTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byScore))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA_LIST *pstIn = (DT_INSTANCE_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wInstanceNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstIn->wInstanceNum)
			break;
		if(-1 == EncodeDT_INSTANCE_DATA(&pstIn->astInstanceInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA_LIST *pstOut = (DT_INSTANCE_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wInstanceNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstOut->wInstanceNum)
			break;
		if(-1 == DecodeDT_INSTANCE_DATA(&pstOut->astInstanceInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT *pstIn = (DT_SCIENCE_ATTREXT*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttrKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttrLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT *pstOut = (DT_SCIENCE_ATTREXT*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttrKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttrLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT_LIST *pstIn = (DT_SCIENCE_ATTREXT_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttrNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstIn->byAttrNum)
			break;
		if(-1 == EncodeDT_SCIENCE_ATTREXT(&pstIn->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT_LIST *pstOut = (DT_SCIENCE_ATTREXT_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttrNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstOut->byAttrNum)
			break;
		if(-1 == DecodeDT_SCIENCE_ATTREXT(&pstOut->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_TASK_CONDITION *pstIn = (DT_CROWN_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wCrownTimes))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_TASK_CONDITION *pstOut = (DT_CROWN_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wCrownTimes))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_BUILDUPGRADE_TASK_CONDITION *pstIn = (DT_BUILDUPGRADE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBuildKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_BUILDUPGRADE_TASK_CONDITION *pstOut = (DT_BUILDUPGRADE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBuildKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_RECRUITHERO_TASK_CONDITION *pstIn = (DT_RECRUITHERO_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wRecruitHeroKindID1))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wRecruitHeroKindID2))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_RECRUITHERO_TASK_CONDITION *pstOut = (DT_RECRUITHERO_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wRecruitHeroKindID1))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wRecruitHeroKindID2))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROCOLOR_CONDITION *pstIn = (DT_CROWN_HEROCOLOR_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroColor))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROCOLOR_CONDITION *pstOut = (DT_CROWN_HEROCOLOR_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroColor))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROUPGRADE_CONDITION *pstIn = (DT_CROWN_HEROUPGRADE_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROUPGRADE_CONDITION *pstOut = (DT_CROWN_HEROUPGRADE_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_FIRSTPASSINSTANCE_TASK_CONDITION *pstIn = (DT_FIRSTPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_FIRSTPASSINSTANCE_TASK_CONDITION *pstOut = (DT_FIRSTPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_SCOREPASSINSTANCE_TASK_CONDITION *pstIn = (DT_SCOREPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byScore))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_SCOREPASSINSTANCE_TASK_CONDITION *pstOut = (DT_SCOREPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byScore))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_TIMESPASSINSTANCE_TASK_CONDITION *pstIn = (DT_TIMESPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTimes))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_TIMESPASSINSTANCE_TASK_CONDITION *pstOut = (DT_TIMESPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTimes))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_KILLMONSTERINSTANCE_TASK_CONDITION *pstIn = (DT_KILLMONSTERINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMonsterKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byKillNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_KILLMONSTERINSTANCE_TASK_CONDITION *pstOut = (DT_KILLMONSTERINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMonsterKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byKillNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION *pstIn = (DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION *pstOut = (DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPNUM_TASK_CONDITION *pstIn = (DT_STRENGTHENEQUIPNUM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPNUM_TASK_CONDITION *pstOut = (DT_STRENGTHENEQUIPNUM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ITEMCOLLECT_TASK_CONDITION *pstIn = (DT_ITEMCOLLECT_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wItemKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ITEM_KIND_COLLECT_NUM; i++)
	{
		if(i >= (INT32)pstIn->wItemKindNum)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyItemCollectInfo[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ITEMCOLLECT_TASK_CONDITION *pstOut = (DT_ITEMCOLLECT_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wItemKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ITEM_KIND_COLLECT_NUM; i++)
	{
		if(i >= (INT32)pstOut->wItemKindNum)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyItemCollectInfo[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ACTIVATEGODWEAPON_TASK_CONDITION *pstIn = (DT_ACTIVATEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ACTIVATEGODWEAPON_TASK_CONDITION *pstOut = (DT_ACTIVATEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEGODWEAPON_TASK_CONDITION *pstIn = (DT_UPGRADEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wQuality))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEGODWEAPON_TASK_CONDITION *pstOut = (DT_UPGRADEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wQuality))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEDRUG_TASK_CONDITION *pstIn = (DT_UPGRADEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wUpgradeNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEDRUG_TASK_CONDITION *pstOut = (DT_UPGRADEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wUpgradeNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CHALLENGE_TASK_CONDITION *pstIn = (DT_CHALLENGE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wChallengeNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CHALLENGE_TASK_CONDITION *pstOut = (DT_CHALLENGE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wChallengeNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_TASK_CONDITION *pstIn = (DT_PLUNDER_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wPlunderNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_TASK_CONDITION *pstOut = (DT_PLUNDER_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wPlunderNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSSKILL_TASK_CONDITION *pstIn = (DT_DRESSSKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSkillID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDressFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSSKILL_TASK_CONDITION *pstOut = (DT_DRESSSKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSkillID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDressFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSEQUIP_TASK_CONDITION *pstIn = (DT_DRESSEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDressFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSEQUIP_TASK_CONDITION *pstOut = (DT_DRESSEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDressFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEEQUIP_TASK_CONDITION *pstIn = (DT_UPGRADEEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUpgradeFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEEQUIP_TASK_CONDITION *pstOut = (DT_UPGRADEEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUpgradeFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_USEEXPRIENCEDRUG_TASK_CONDITION *pstIn = (DT_USEEXPRIENCEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_USEEXPRIENCEDRUG_TASK_CONDITION *pstOut = (DT_USEEXPRIENCEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_MELT_EQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_MELT_EQUIP_TASK_CONDITION *pstIn = (DT_MELT_EQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MELT_EQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_MELT_EQUIP_TASK_CONDITION *pstOut = (DT_MELT_EQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_COMPOSEGOOD_TASK_CONDITION *pstIn = (DT_COMPOSEGOOD_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMaterialID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMaterialNum))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_COMPOSEGOOD_TASK_CONDITION *pstOut = (DT_COMPOSEGOOD_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMaterialID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMaterialNum))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ELITEINSTANCE_TASK_CONDITION *pstIn = (DT_ELITEINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUpgradeFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ELITEINSTANCE_TASK_CONDITION *pstOut = (DT_ELITEINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUpgradeFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADSCIENCE_TASK_CONDITION *pstIn = (DT_UPGRADSCIENCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADSCIENCE_TASK_CONDITION *pstOut = (DT_UPGRADSCIENCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHATTACK_TASK_CONDITION *pstIn = (DT_STRENGTHATTACK_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHATTACK_TASK_CONDITION *pstOut = (DT_STRENGTHATTACK_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHLIFE_TASK_CONDITION *pstIn = (DT_STRENGTHLIFE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHLIFE_TASK_CONDITION *pstOut = (DT_STRENGTHLIFE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROP_TASK_CONDITION *pstIn = (DT_CROP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROP_TASK_CONDITION *pstOut = (DT_CROP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADESKILL_TASK_CONDITION *pstIn = (DT_UPGRADESKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSkillActionID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADESKILL_TASK_CONDITION *pstOut = (DT_UPGRADESKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSkillActionID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_UPGRADEJINGJIE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEJINGJIE_TASK_CONDITION *pstIn = (DT_UPGRADEJINGJIE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSubLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADEJINGJIE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEJINGJIE_TASK_CONDITION *pstOut = (DT_UPGRADEJINGJIE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSubLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_MOSAICGEM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_MOSAICGEM_TASK_CONDITION *pstIn = (DT_MOSAICGEM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MOSAICGEM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_MOSAICGEM_TASK_CONDITION *pstOut = (DT_MOSAICGEM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CLIMBTOWER_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CLIMBTOWER_TASK_CONDITION *pstIn = (DT_CLIMBTOWER_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CLIMBTOWER_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CLIMBTOWER_TASK_CONDITION *pstOut = (DT_CLIMBTOWER_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_STUDY_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STUDY_TASK_CONDITION *pstIn = (DT_STUDY_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STUDY_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STUDY_TASK_CONDITION *pstOut = (DT_STUDY_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_RECRUITHERO_NUM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_RECRUITHERO_NUM_TASK_CONDITION *pstIn = (DT_RECRUITHERO_NUM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RECRUITHERO_NUM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_RECRUITHERO_NUM_TASK_CONDITION *pstOut = (DT_RECRUITHERO_NUM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case CROWN:
		return EncodeDT_CROWN_TASK_CONDITION(pHost, poNetData);
	case RECRUITHERO:
		return EncodeDT_RECRUITHERO_TASK_CONDITION(pHost, poNetData);
	case UPGRADESKILL:
		return EncodeDT_UPGRADESKILL_TASK_CONDITION(pHost, poNetData);
	case UPGRADEJINGJIE:
		return EncodeDT_UPGRADEJINGJIE_TASK_CONDITION(pHost, poNetData);
	case FIRSTPASSINSTANCE:
		return EncodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case SCOREPASSINSTANCE:
		return EncodeDT_SCOREPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case TIMESPASSINSTANCE:
		return EncodeDT_TIMESPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case KILLMONSTERINSTANCE:
		return EncodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPLEVEL:
		return EncodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPNUM:
		return EncodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(pHost, poNetData);
	case MOSAICGEM:
		return EncodeDT_MOSAICGEM_TASK_CONDITION(pHost, poNetData);
	case ACTIVATEGODWEAPON:
		return EncodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case UPGRADEGODWEAPON:
		return EncodeDT_UPGRADEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case CHALLENGE:
		return EncodeDT_CHALLENGE_TASK_CONDITION(pHost, poNetData);
	case PLUNDER:
		return EncodeDT_PLUNDER_TASK_CONDITION(pHost, poNetData);
	case CLIMBTOWER:
		return EncodeDT_CLIMBTOWER_TASK_CONDITION(pHost, poNetData);
	case DRESSEQUIP:
		return EncodeDT_DRESSEQUIP_TASK_CONDITION(pHost, poNetData);
	case USEEXPRIENCEDRUG:
		return EncodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(pHost, poNetData);
	case COMPOSEGOOD:
		return EncodeDT_COMPOSEGOOD_TASK_CONDITION(pHost, poNetData);
	case ELITEINSTANCE:
		return EncodeDT_ELITEINSTANCE_TASK_CONDITION(pHost, poNetData);
	case UPGRADSCIENCE:
		return EncodeDT_UPGRADSCIENCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHATTACK:
		return EncodeDT_STRENGTHATTACK_TASK_CONDITION(pHost, poNetData);
	case STRENGTHLIFE:
		return EncodeDT_STRENGTHLIFE_TASK_CONDITION(pHost, poNetData);
	case CROP:
		return EncodeDT_CROP_TASK_CONDITION(pHost, poNetData);
	case MELTEQUIP:
		return EncodeDT_MELT_EQUIP_TASK_CONDITION(pHost, poNetData);
	case STUDY:
		return EncodeDT_STUDY_TASK_CONDITION(pHost, poNetData);
	case RECRUITHERONUM:
		return EncodeDT_RECRUITHERO_NUM_TASK_CONDITION(pHost, poNetData);
	default: return -1;
	}
}

INT32 DecodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case CROWN:
		return DecodeDT_CROWN_TASK_CONDITION(pHost, poNetData);
	case RECRUITHERO:
		return DecodeDT_RECRUITHERO_TASK_CONDITION(pHost, poNetData);
	case UPGRADESKILL:
		return DecodeDT_UPGRADESKILL_TASK_CONDITION(pHost, poNetData);
	case UPGRADEJINGJIE:
		return DecodeDT_UPGRADEJINGJIE_TASK_CONDITION(pHost, poNetData);
	case FIRSTPASSINSTANCE:
		return DecodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case SCOREPASSINSTANCE:
		return DecodeDT_SCOREPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case TIMESPASSINSTANCE:
		return DecodeDT_TIMESPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case KILLMONSTERINSTANCE:
		return DecodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPLEVEL:
		return DecodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPNUM:
		return DecodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(pHost, poNetData);
	case MOSAICGEM:
		return DecodeDT_MOSAICGEM_TASK_CONDITION(pHost, poNetData);
	case ACTIVATEGODWEAPON:
		return DecodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case UPGRADEGODWEAPON:
		return DecodeDT_UPGRADEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case CHALLENGE:
		return DecodeDT_CHALLENGE_TASK_CONDITION(pHost, poNetData);
	case PLUNDER:
		return DecodeDT_PLUNDER_TASK_CONDITION(pHost, poNetData);
	case CLIMBTOWER:
		return DecodeDT_CLIMBTOWER_TASK_CONDITION(pHost, poNetData);
	case DRESSEQUIP:
		return DecodeDT_DRESSEQUIP_TASK_CONDITION(pHost, poNetData);
	case USEEXPRIENCEDRUG:
		return DecodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(pHost, poNetData);
	case COMPOSEGOOD:
		return DecodeDT_COMPOSEGOOD_TASK_CONDITION(pHost, poNetData);
	case ELITEINSTANCE:
		return DecodeDT_ELITEINSTANCE_TASK_CONDITION(pHost, poNetData);
	case UPGRADSCIENCE:
		return DecodeDT_UPGRADSCIENCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHATTACK:
		return DecodeDT_STRENGTHATTACK_TASK_CONDITION(pHost, poNetData);
	case STRENGTHLIFE:
		return DecodeDT_STRENGTHLIFE_TASK_CONDITION(pHost, poNetData);
	case CROP:
		return DecodeDT_CROP_TASK_CONDITION(pHost, poNetData);
	case MELTEQUIP:
		return DecodeDT_MELT_EQUIP_TASK_CONDITION(pHost, poNetData);
	case STUDY:
		return DecodeDT_STUDY_TASK_CONDITION(pHost, poNetData);
	case RECRUITHERONUM:
		return DecodeDT_RECRUITHERO_NUM_TASK_CONDITION(pHost, poNetData);
	default: return -1;
	}
}

INT32 EncodeDT_TASK_DATA(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA *pstIn = (DT_TASK_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byTaskLine))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwTaskIdx))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStateUpdateTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTaskState))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTaskType))
		return -1;

	if(-1 == EncodeUnDT_TASK_CONDITION(&pstIn->stTaskConditionInfo, pstIn->byTaskType, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_TASK_DATA(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA *pstOut = (DT_TASK_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byTaskLine))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwTaskIdx))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStateUpdateTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTaskState))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTaskType))
		return -1;

	if(-1 == DecodeUnDT_TASK_CONDITION(&pstOut->stTaskConditionInfo, pstOut->byTaskType, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA_LIST *pstIn = (DT_TASK_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byTaskNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TASK_LINE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byTaskNum)
			break;
		if(-1 == EncodeDT_TASK_DATA(&pstIn->astTaskInfoList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA_LIST *pstOut = (DT_TASK_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byTaskNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TASK_LINE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byTaskNum)
			break;
		if(-1 == DecodeDT_TASK_DATA(&pstOut->astTaskInfoList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_RACE_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_DATA *pstIn = (DT_RACE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwRank))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wContiWin))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRecvEncourageTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastChallTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byChallTimes))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byFailedFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwRecvEncourageRank))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwNotifyRankTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwNotifyRank))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RACE_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_DATA *pstOut = (DT_RACE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwRank))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wContiWin))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRecvEncourageTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastChallTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byChallTimes))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byFailedFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwRecvEncourageRank))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwNotifyRankTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwNotifyRank))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_HOME *pstIn = (DT_LOCALE_HOME*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byExt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_HOME *pstOut = (DT_LOCALE_HOME*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byExt))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_INSTANCE *pstIn = (DT_LOCALE_INSTANCE*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySuccessFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_INSTANCE *pstOut = (DT_LOCALE_INSTANCE*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySuccessFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_ONHOOK *pstIn = (DT_LOCALE_ONHOOK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_ONHOOK *pstOut = (DT_LOCALE_ONHOOK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_LOCALE_EATBREAD(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_EATBREAD *pstIn = (DT_LOCALE_EATBREAD*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byExt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_EATBREAD(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_EATBREAD *pstOut = (DT_LOCALE_EATBREAD*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byExt))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_LOCALE_BOSSB(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_BOSSB *pstIn = (DT_LOCALE_BOSSB*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byExt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_BOSSB(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_BOSSB *pstOut = (DT_LOCALE_BOSSB*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byExt))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case EPL_HOME:
		return EncodeDT_LOCALE_HOME(pHost, poNetData);
	case EPL_INSTANCE:
		return EncodeDT_LOCALE_INSTANCE(pHost, poNetData);
	case EPL_ONHOOK:
		return EncodeDT_LOCALE_ONHOOK(pHost, poNetData);
	case EPL_EATBREAD:
		return EncodeDT_LOCALE_EATBREAD(pHost, poNetData);
	case EPL_BOSSB:
		return EncodeDT_LOCALE_BOSSB(pHost, poNetData);
	default: return -1;
	}
}

INT32 DecodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case EPL_HOME:
		return DecodeDT_LOCALE_HOME(pHost, poNetData);
	case EPL_INSTANCE:
		return DecodeDT_LOCALE_INSTANCE(pHost, poNetData);
	case EPL_ONHOOK:
		return DecodeDT_LOCALE_ONHOOK(pHost, poNetData);
	case EPL_EATBREAD:
		return DecodeDT_LOCALE_EATBREAD(pHost, poNetData);
	case EPL_BOSSB:
		return DecodeDT_LOCALE_BOSSB(pHost, poNetData);
	default: return -1;
	}
}

INT32 EncodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_DATA *pstIn = (DT_LOCALE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLocaleType))
		return -1;

	if(-1 == EncodeUnDT_LOCALE_DATA_UNION(&pstIn->stLocaleInfo, pstIn->byLocaleType, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_DATA *pstOut = (DT_LOCALE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLocaleType))
		return -1;

	if(-1 == DecodeUnDT_LOCALE_DATA_UNION(&pstOut->stLocaleInfo, pstOut->byLocaleType, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeUnlockScienceInfo(void *pHost, CNetData* poNetData)
{
	UnlockScienceInfo *pstIn = (UnlockScienceInfo*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byUnlockNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstIn->byUnlockNum)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUnlockScienceID[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeUnlockScienceInfo(void *pHost, CNetData* poNetData)
{
	UnlockScienceInfo *pstOut = (UnlockScienceInfo*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byUnlockNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstOut->byUnlockNum)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUnlockScienceID[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_UPGRADE_DATA *pstIn = (DT_HERO_UPGRADE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUpgradeFlag))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwEncExperience))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwExperience))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUpgradeExperience))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCanUseBoxGoodsID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_UPGRADE_DATA *pstOut = (DT_HERO_UPGRADE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUpgradeFlag))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwEncExperience))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwExperience))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUpgradeExperience))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCanUseBoxGoodsID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_STATE_DATA(void *pHost, CNetData* poNetData)
{
	DT_STATE_DATA *pstIn = (DT_STATE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwInstanceMAXHP))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwInstanceCurHP))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDoubleExpNum))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOpenBoxFlag))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_BOX_GROUP_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyHitItemIdx[i]))
			return -1;
	}

	for(i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
	{
		if(-1 == poNetData->AddWord(pstIn->awUnlockHeroID[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wUnlockSkillID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wUnlockEquipLevel))
		return -1;

	if(-1 == EncodeUnlockScienceInfo(&pstIn->stUnlockScienceList, poNetData))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRegisterDispNameTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUnlockElite))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUpgradeHeroNum))
		return -1;

	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstIn->byUpgradeHeroNum)
			break;
		if(-1 == EncodeDT_HERO_UPGRADE_DATA(&pstIn->astHeroUpgradeInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STATE_DATA(void *pHost, CNetData* poNetData)
{
	DT_STATE_DATA *pstOut = (DT_STATE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwInstanceMAXHP))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwInstanceCurHP))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDoubleExpNum))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOpenBoxFlag))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_BOX_GROUP_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyHitItemIdx[i]))
			return -1;
	}

	for(i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
	{
		if(-1 == poNetData->DelWord(pstOut->awUnlockHeroID[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wUnlockSkillID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wUnlockEquipLevel))
		return -1;

	if(-1 == DecodeUnlockScienceInfo(&pstOut->stUnlockScienceList, poNetData))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRegisterDispNameTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUnlockElite))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUpgradeHeroNum))
		return -1;

	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstOut->byUpgradeHeroNum)
			break;
		if(-1 == DecodeDT_HERO_UPGRADE_DATA(&pstOut->astHeroUpgradeInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_BASE_DATA *pstIn = (DT_MONSTER_BASE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_BASE_DATA *pstOut = (DT_MONSTER_BASE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_DATA_LIST *pstIn = (DT_MONSTER_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMonsterNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_MONSTER_NUM; i++)
	{
		if(i >= (INT32)pstIn->wMonsterNum)
			break;
		if(-1 == EncodeDT_MONSTER_BASE_DATA(&pstIn->astMonsterInfListo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_DATA_LIST *pstOut = (DT_MONSTER_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMonsterNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_MONSTER_NUM; i++)
	{
		if(i >= (INT32)pstOut->wMonsterNum)
			break;
		if(-1 == DecodeDT_MONSTER_BASE_DATA(&pstOut->astMonsterInfListo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_DATA *pstIn = (DT_BATTLE_OBJ_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCareerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byFomationIdx))
		return -1;

	if(-1 == EncodeDT_BATTLE_ATTRIBUTE(&pstIn->stBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSkillActionID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSkillActionLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNeedAngry))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wInitAngry))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHPGridNum))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byColor))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_DATA *pstOut = (DT_BATTLE_OBJ_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCareerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byFomationIdx))
		return -1;

	if(-1 == DecodeDT_BATTLE_ATTRIBUTE(&pstOut->stBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSkillActionID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSkillActionLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNeedAngry))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wInitAngry))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHPGridNum))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byColor))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_GROUP_DATA *pstIn = (DT_BATTLE_OBJ_GROUP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byObjNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstIn->byObjNum)
			break;
		if(-1 == EncodeDT_BATTLE_OBJ_DATA(&pstIn->astBattleObjInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddTString(pstIn->aszDispName, sizeof(pstIn->aszDispName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_GROUP_DATA *pstOut = (DT_BATTLE_OBJ_GROUP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byObjNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstOut->byObjNum)
			break;
		if(-1 == DecodeDT_BATTLE_OBJ_DATA(&pstOut->astBattleObjInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelTString(pstOut->aszDispName, sizeof(pstOut->aszDispName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ITEM_DATA(void *pHost, CNetData* poNetData)
{
	DT_ITEM_DATA *pstIn = (DT_ITEM_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ITEM_DATA(void *pHost, CNetData* poNetData)
{
	DT_ITEM_DATA *pstOut = (DT_ITEM_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BUFF_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUFF_DATA *pstIn = (DT_BUFF_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAddDelFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBuffID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUFF_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUFF_DATA *pstOut = (DT_BUFF_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAddDelFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBuffID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_HP_DATA(void *pHost, CNetData* poNetData)
{
	DT_HP_DATA *pstIn = (DT_HP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddInt(pstIn->nHP))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HP_DATA(void *pHost, CNetData* poNetData)
{
	DT_HP_DATA *pstOut = (DT_HP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelInt(pstOut->nHP))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ANGRY_DATA *pstIn = (DT_ANGRY_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddShort(pstIn->shAngry))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ANGRY_DATA *pstOut = (DT_ANGRY_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelShort(pstOut->shAngry))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PARRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_PARRY_DATA *pstIn = (DT_PARRY_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byParryFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBeCritFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PARRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_PARRY_DATA *pstOut = (DT_PARRY_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byParryFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBeCritFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_EFFECT_ID_DATA *pstIn = (DT_SKILL_EFFECT_ID_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wEffectID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_EFFECT_ID_DATA *pstOut = (DT_SKILL_EFFECT_ID_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wEffectID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_OBJ_DATA *pstIn = (DT_ATTACK_OBJ_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBuffChangeNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->byBuffChangeNum)
			break;
		if(-1 == EncodeDT_BUFF_DATA(&pstIn->astBuffChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byHpChangeNum))
		return -1;

	for(i = 0; i < MAX_HP_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->byHpChangeNum)
			break;
		if(-1 == EncodeDT_HP_DATA(&pstIn->astHpChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byAngryChangeNum))
		return -1;

	for(i = 0; i < MAX_ANGRY_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->byAngryChangeNum)
			break;
		if(-1 == EncodeDT_ANGRY_DATA(&pstIn->astAngryChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->bySkillEffectIDChangeNum))
		return -1;

	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->bySkillEffectIDChangeNum)
			break;
		if(-1 == EncodeDT_SKILL_EFFECT_ID_DATA(&pstIn->astSkillEffectIDChangeInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_OBJ_DATA *pstOut = (DT_ATTACK_OBJ_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBuffChangeNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->byBuffChangeNum)
			break;
		if(-1 == DecodeDT_BUFF_DATA(&pstOut->astBuffChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byHpChangeNum))
		return -1;

	for(i = 0; i < MAX_HP_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->byHpChangeNum)
			break;
		if(-1 == DecodeDT_HP_DATA(&pstOut->astHpChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byAngryChangeNum))
		return -1;

	for(i = 0; i < MAX_ANGRY_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->byAngryChangeNum)
			break;
		if(-1 == DecodeDT_ANGRY_DATA(&pstOut->astAngryChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->bySkillEffectIDChangeNum))
		return -1;

	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->bySkillEffectIDChangeNum)
			break;
		if(-1 == DecodeDT_SKILL_EFFECT_ID_DATA(&pstOut->astSkillEffectIDChangeInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_DATA *pstIn = (DT_ATTACK_DATA*)pHost;

	if(-1 == EncodeDT_ATTACK_OBJ_DATA(&pstIn->stMyselfObjInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_ATTACK_OBJ_DATA(&pstIn->stEnemyObjInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_DATA *pstOut = (DT_ATTACK_DATA*)pHost;

	if(-1 == DecodeDT_ATTACK_OBJ_DATA(&pstOut->stMyselfObjInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_ATTACK_OBJ_DATA(&pstOut->stEnemyObjInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_ALL_DATA *pstIn = (DT_ATTACK_ALL_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttackerIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttackSkillID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySkipFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBeAttackerIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBeAttackerIdxNum)
			break;
		if(-1 == EncodeDT_PARRY_DATA(&pstIn->astBeAttackerIdxInfo[i], poNetData))
			return -1;
	}

	if(-1 == EncodeDT_ATTACK_DATA(&pstIn->stBeforeAttack, poNetData))
		return -1;

	if(-1 == EncodeDT_ATTACK_DATA(&pstIn->stAfterAttack, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_ALL_DATA *pstOut = (DT_ATTACK_ALL_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttackerIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttackSkillID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySkipFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBeAttackerIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBeAttackerIdxNum)
			break;
		if(-1 == DecodeDT_PARRY_DATA(&pstOut->astBeAttackerIdxInfo[i], poNetData))
			return -1;
	}

	if(-1 == DecodeDT_ATTACK_DATA(&pstOut->stBeforeAttack, poNetData))
		return -1;

	if(-1 == DecodeDT_ATTACK_DATA(&pstOut->stAfterAttack, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ANGRY_DATA *pstIn = (DT_BATTLE_ANGRY_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->bySlot))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCurAngry))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ANGRY_DATA *pstOut = (DT_BATTLE_ANGRY_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->bySlot))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCurAngry))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ROUND_DATA *pstIn = (DT_BATTLE_ROUND_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttackerFlag))
		return -1;

	if(-1 == EncodeDT_ATTACK_ALL_DATA(&pstIn->stAttackData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ROUND_DATA *pstOut = (DT_BATTLE_ROUND_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttackerFlag))
		return -1;

	if(-1 == DecodeDT_ATTACK_ALL_DATA(&pstOut->stAttackData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_DATA *pstIn = (DT_BATTLE_DATA*)pHost;

	if(-1 == EncodeDT_BATTLE_OBJ_GROUP_DATA(&pstIn->stMyselfInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_BATTLE_OBJ_GROUP_DATA(&pstIn->stEnemyInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleResult))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byScore))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleRoundNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BATTLE_ROUND_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBattleRoundNum)
			break;
		if(-1 == EncodeDT_BATTLE_ROUND_DATA(&pstIn->astBattleRoundInfo[i], poNetData))
			return -1;
	}

	if(-1 == EncodeDT_BATTLE_ATTRIBUTE(&pstIn->stMyselfInfoAfterBattle, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_DATA *pstOut = (DT_BATTLE_DATA*)pHost;

	if(-1 == DecodeDT_BATTLE_OBJ_GROUP_DATA(&pstOut->stMyselfInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_BATTLE_OBJ_GROUP_DATA(&pstOut->stEnemyInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleResult))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byScore))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleRoundNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BATTLE_ROUND_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBattleRoundNum)
			break;
		if(-1 == DecodeDT_BATTLE_ROUND_DATA(&pstOut->astBattleRoundInfo[i], poNetData))
			return -1;
	}

	if(-1 == DecodeDT_BATTLE_ATTRIBUTE(&pstOut->stMyselfInfoAfterBattle, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONLINE_ENCOURAGE_RECORD_DATA *pstIn = (DT_ONLINE_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byRecvDays))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastRecvIndex))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONLINE_ENCOURAGE_RECORD_DATA *pstOut = (DT_ONLINE_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byRecvDays))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastRecvIndex))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOGIN_ENCOURAGE_RECORD_DATA *pstIn = (DT_LOGIN_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKeepDays))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastRecvNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastSelectIndex))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byStateFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOGIN_ENCOURAGE_RECORD_DATA *pstOut = (DT_LOGIN_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKeepDays))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastRecvNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastSelectIndex))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byStateFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA *pstIn = (DT_RACE_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byChallengeFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwChallengeTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwBeginChallengeRank))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAfterChallengeRank))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNewFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA *pstOut = (DT_RACE_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byChallengeFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwChallengeTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwBeginChallengeRank))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAfterChallengeRank))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNewFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA_LIST *pstIn = (DT_RACE_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RACE_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBattleLogNum)
			break;
		if(-1 == EncodeDT_RACE_BATTLELOG_DATA(&pstIn->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA_LIST *pstOut = (DT_RACE_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RACE_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBattleLogNum)
			break;
		if(-1 == DecodeDT_RACE_BATTLELOG_DATA(&pstOut->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA *pstIn = (DT_CAPTURE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwCaptureTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCaptureGridIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byIncomeType))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCollectTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA *pstOut = (DT_CAPTURE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwCaptureTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCaptureGridIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byIncomeType))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCollectTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA_LIST *pstIn = (DT_CAPTURE_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byCaptureNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CAPTURE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byCaptureNum)
			break;
		if(-1 == EncodeDT_CAPTURE_DATA(&pstIn->astCaptureInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA_LIST *pstOut = (DT_CAPTURE_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byCaptureNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CAPTURE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byCaptureNum)
			break;
		if(-1 == DecodeDT_CAPTURE_DATA(&pstOut->astCaptureInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA *pstIn = (DT_PLUNDER_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byPlunderFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwPlunderTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwActivePlayerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPassivePlayerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwPlunderCoin))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwPlunderScience))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNewFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA *pstOut = (DT_PLUNDER_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byPlunderFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwPlunderTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwActivePlayerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPassivePlayerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwPlunderCoin))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwPlunderScience))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNewFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA_LIST *pstIn = (DT_PLUNDER_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_PLUNDER_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBattleLogNum)
			break;
		if(-1 == EncodeDT_PLUNDER_BATTLELOG_DATA(&pstIn->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA_LIST *pstOut = (DT_PLUNDER_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_PLUNDER_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBattleLogNum)
			break;
		if(-1 == DecodeDT_PLUNDER_BATTLELOG_DATA(&pstOut->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_GRID_DATA *pstIn = (DT_PLUNDER_GRID_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byIncomeType))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwIncomeValue))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_GRID_DATA *pstOut = (DT_PLUNDER_GRID_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byIncomeType))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwIncomeValue))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BASE_DATA *pstIn = (DT_PLUNDER_BASE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCaptureOwnerPlayerID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBeCaptureTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wExtPlunderNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastPlunderTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastPlunderNum))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastDriveAwayTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastDriveAwayNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CAPTURE_NUM; i++)
	{
		if(-1 == EncodeDT_PLUNDER_GRID_DATA(&pstIn->astCaptureGridInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BASE_DATA *pstOut = (DT_PLUNDER_BASE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCaptureOwnerPlayerID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBeCaptureTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wExtPlunderNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastPlunderTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastPlunderNum))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastDriveAwayTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastDriveAwayNum))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_CAPTURE_NUM; i++)
		{
			if(-1 == DecodeDT_PLUNDER_GRID_DATA(&pstOut->astCaptureGridInfo[i], poNetData))
				return -1;
		}

	return sizeof(*pstOut);
}

INT32 EncodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA *pstIn = (DT_ENEMY_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBattleNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwAddTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA *pstOut = (DT_ENEMY_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBattleNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwAddTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA_LIST *pstIn = (DT_ENEMY_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byEnemyNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENEMY_NUM; i++)
	{
		if(i >= (INT32)pstIn->byEnemyNum)
			break;
		if(-1 == EncodeDT_ENEMY_DATA(&pstIn->astEnemyInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA_LIST *pstOut = (DT_ENEMY_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byEnemyNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENEMY_NUM; i++)
	{
		if(i >= (INT32)pstOut->byEnemyNum)
			break;
		if(-1 == DecodeDT_ENEMY_DATA(&pstOut->astEnemyInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONEY_TREE_DATA *pstIn = (DT_MONEY_TREE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wRockNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRockTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byMoodValue))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwMoodStartTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONEY_TREE_DATA *pstOut = (DT_MONEY_TREE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wRockNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRockTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byMoodValue))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwMoodStartTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_BATTLE_DATA *pstIn = (DT_INSTANCE_BATTLE_DATA*)pHost;

	if(-1 == EncodeDT_INSTANCE_ID(&pstIn->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleIndex))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBattleTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMaxRounds))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMaxPercentageHp))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_BATTLE_DATA *pstOut = (DT_INSTANCE_BATTLE_DATA*)pHost;

	if(-1 == DecodeDT_INSTANCE_ID(&pstOut->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleIndex))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBattleTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMaxRounds))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMaxPercentageHp))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData)
{
	DT_PHYSTRENGTH_DATA *pstIn = (DT_PHYSTRENGTH_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwLastPhyStrengthRefreshTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastBuyPhyStrengthTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastBuyPhyStrengthNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData)
{
	DT_PHYSTRENGTH_DATA *pstOut = (DT_PHYSTRENGTH_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwLastPhyStrengthRefreshTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastBuyPhyStrengthTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastBuyPhyStrengthNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RESULT *pstIn = (DT_ONHOOK_RESULT*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byCurRunTimes))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAllRunTimes))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwStory))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwExperience))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCoin))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwGold))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byItemNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_ITEM_KIND_NUM; i++)
	{
		if(i >= (INT32)pstIn->byItemNum)
			break;
		if(-1 == EncodeDT_ITEM_DATA(&pstIn->astItemInfoList[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwBlueGas))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RESULT *pstOut = (DT_ONHOOK_RESULT*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byCurRunTimes))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAllRunTimes))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwStory))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwExperience))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCoin))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwGold))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byItemNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_ITEM_KIND_NUM; i++)
	{
		if(i >= (INT32)pstOut->byItemNum)
			break;
		if(-1 == DecodeDT_ITEM_DATA(&pstOut->astItemInfoList[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwBlueGas))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RCD_DATA *pstIn = (DT_ONHOOK_RCD_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byOnhookFlag))
		return -1;

	if(-1 == EncodeDT_ONHOOK_RESULT(&pstIn->stOnhookResult, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_ID(&pstIn->stOnhookInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastOnhookResultReqTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RCD_DATA *pstOut = (DT_ONHOOK_RCD_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byOnhookFlag))
		return -1;

	if(-1 == DecodeDT_ONHOOK_RESULT(&pstOut->stOnhookResult, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_ID(&pstOut->stOnhookInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastOnhookResultReqTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData)
{
	DT_FEW_PARAM_DATA *pstIn = (DT_FEW_PARAM_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwLastStrengthenTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastStrengthenCD))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRefreshEliteTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastRefreshEliteNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData)
{
	DT_FEW_PARAM_DATA *pstOut = (DT_FEW_PARAM_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwLastStrengthenTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastStrengthenCD))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRefreshEliteTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastRefreshEliteNum))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_INFO_DATA *pstIn = (DT_CROP_INFO_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byOpen))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCropKindID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCropTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_INFO_DATA *pstOut = (DT_CROP_INFO_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byOpen))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCropKindID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCropTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CROP_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_DATA *pstIn = (DT_CROP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CROP_NUM; i++)
	{
		if(-1 == EncodeDT_CROP_INFO_DATA(&pstIn->astCropInfoList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROP_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_DATA *pstOut = (DT_CROP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_CROP_NUM; i++)
		{
			if(-1 == DecodeDT_CROP_INFO_DATA(&pstOut->astCropInfoList[i], poNetData))
				return -1;
		}

	return sizeof(*pstOut);
}

INT32 EncodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_TREE_DATA *pstIn = (DT_SCIENCE_TREE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wAttractLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHPLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCritLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wShieldLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHitLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDodgeLevel))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFirstAttack))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCoachTalent))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttractLevel2))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHPLevel2))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_TREE_DATA *pstOut = (DT_SCIENCE_TREE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wAttractLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHPLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCritLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wShieldLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHitLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDodgeLevel))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFirstAttack))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCoachTalent))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttractLevel2))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHPLevel2))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_HERO_DATA *pstIn = (DT_FORMATION_HERO_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_HERO_DATA *pstOut = (DT_FORMATION_HERO_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_IDX_DATA *pstIn = (DT_FORMATION_IDX_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationKind))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOpenIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstIn->byOpenIdxNum)
			break;
		if(-1 == EncodeDT_FORMATION_HERO_DATA(&pstIn->astOpenIdxInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_IDX_DATA *pstOut = (DT_FORMATION_IDX_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationKind))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOpenIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstOut->byOpenIdxNum)
			break;
		if(-1 == DecodeDT_FORMATION_HERO_DATA(&pstOut->astOpenIdxInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA_LIST *pstIn = (DT_FORMATION_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_NUM; i++)
	{
		if(i >= (INT32)pstIn->byFormationKindNum)
			break;
		if(-1 == EncodeDT_FORMATION_IDX_DATA(&pstIn->astFormationInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA_LIST *pstOut = (DT_FORMATION_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_NUM; i++)
	{
		if(i >= (INT32)pstOut->byFormationKindNum)
			break;
		if(-1 == DecodeDT_FORMATION_IDX_DATA(&pstOut->astFormationInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA *pstIn = (DT_FORMATION_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->bySelectIdx))
		return -1;

	if(-1 == EncodeDT_FORMATION_DATA_LIST(&pstIn->stFormationList, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA *pstOut = (DT_FORMATION_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->bySelectIdx))
		return -1;

	if(-1 == DecodeDT_FORMATION_DATA_LIST(&pstOut->stFormationList, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_VERSION_DATA(void *pHost, CNetData* poNetData)
{
	DT_VERSION_DATA *pstIn = (DT_VERSION_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwVersionID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byForceUpdateFlag))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUpdateContent, sizeof(pstIn->aszUpdateContent)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUpdateURL, sizeof(pstIn->aszUpdateURL)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_VERSION_DATA(void *pHost, CNetData* poNetData)
{
	DT_VERSION_DATA *pstOut = (DT_VERSION_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwVersionID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byForceUpdateFlag))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUpdateContent, sizeof(pstOut->aszUpdateContent)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUpdateURL, sizeof(pstOut->aszUpdateURL)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData)
{
	DT_NOTICE_DATA *pstIn = (DT_NOTICE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwNoticeID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byForcePopFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUrlFlag))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNoticeContent, sizeof(pstIn->aszNoticeContent)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData)
{
	DT_NOTICE_DATA *pstOut = (DT_NOTICE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwNoticeID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byForcePopFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUrlFlag))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNoticeContent, sizeof(pstOut->aszNoticeContent)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_EAT_BREAD_DATA(void *pHost, CNetData* poNetData)
{
	DT_EAT_BREAD_DATA *pstIn = (DT_EAT_BREAD_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwEnterTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHaveObtainPhyStrengthen))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwHaveObtainCoin))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHaveLeaveFlag))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHaveUseTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHaveGivePhyStrengthen))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwHaveGiveCoin))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNotity))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EAT_BREAD_DATA(void *pHost, CNetData* poNetData)
{
	DT_EAT_BREAD_DATA *pstOut = (DT_EAT_BREAD_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwEnterTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHaveObtainPhyStrengthen))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwHaveObtainCoin))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHaveLeaveFlag))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHaveUseTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHaveGivePhyStrengthen))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwHaveGiveCoin))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNotity))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_YABIAO_DATA(void *pHost, CNetData* poNetData)
{
	DT_YABIAO_DATA *pstIn = (DT_YABIAO_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwLastYabiaoTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAlreadyYabiaoCount))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byQuality))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBeJiebiaoCount))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBiaoRunTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBiaoFlushCount))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byYabiaoFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAlreadyJiebiaoCount))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastJiebiaoTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastEncourageCoin))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastEncourageScience))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBeJiebiaoTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastEncourageJingjie))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwJiebiaoDecCoin))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwJiebiaoDecScience))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAlreadyFlushBiaoCnt))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastFlushTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_YABIAO_DATA(void *pHost, CNetData* poNetData)
{
	DT_YABIAO_DATA *pstOut = (DT_YABIAO_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwLastYabiaoTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAlreadyYabiaoCount))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byQuality))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBeJiebiaoCount))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBiaoRunTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBiaoFlushCount))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byYabiaoFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAlreadyJiebiaoCount))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastJiebiaoTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastEncourageCoin))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastEncourageScience))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBeJiebiaoTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastEncourageJingjie))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwJiebiaoDecCoin))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwJiebiaoDecScience))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAlreadyFlushBiaoCnt))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastFlushTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_YABIAO_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_YABIAO_BATTLELOG_DATA *pstIn = (DT_YABIAO_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBattleTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCoinChange))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwScienceChange))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNewFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBattleDataIndex))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byQuality))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwJingjieChange))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_YABIAO_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_YABIAO_BATTLELOG_DATA *pstOut = (DT_YABIAO_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBattleTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCoinChange))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwScienceChange))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNewFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBattleDataIndex))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byQuality))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwJingjieChange))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_YABIAO_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_YABIAO_BATTLELOG_DATA_LIST *pstIn = (DT_YABIAO_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_YABIAO_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBattleLogNum)
			break;
		if(-1 == EncodeDT_YABIAO_BATTLELOG_DATA(&pstIn->astPaoshangBattleLogInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_YABIAO_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_YABIAO_BATTLELOG_DATA_LIST *pstOut = (DT_YABIAO_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_YABIAO_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBattleLogNum)
			break;
		if(-1 == DecodeDT_YABIAO_BATTLELOG_DATA(&pstOut->astPaoshangBattleLogInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_SKILL_ACTION_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_ACTION_DATA *pstIn = (DT_SKILL_ACTION_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_ACTION_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_ACTION_DATA *pstOut = (DT_SKILL_ACTION_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SKILL_ACTION_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_SKILL_ACTION_DATA_LIST *pstIn = (DT_SKILL_ACTION_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byActionNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SKILL_SLOT_NUM; i++)
	{
		if(i >= (INT32)pstIn->byActionNum)
			break;
		if(-1 == EncodeDT_SKILL_ACTION_DATA(&pstIn->astActionInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_ACTION_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_SKILL_ACTION_DATA_LIST *pstOut = (DT_SKILL_ACTION_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byActionNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SKILL_SLOT_NUM; i++)
	{
		if(i >= (INT32)pstOut->byActionNum)
			break;
		if(-1 == DecodeDT_SKILL_ACTION_DATA(&pstOut->astActionInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_JINGJIE_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_JINGJIE_BASE_DATA *pstIn = (DT_JINGJIE_BASE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byCareerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSubLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_JINGJIE_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_JINGJIE_BASE_DATA *pstOut = (DT_JINGJIE_BASE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byCareerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSubLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_GAS_BALL_DATA(void *pHost, CNetData* poNetData)
{
	DT_GAS_BALL_DATA *pstIn = (DT_GAS_BALL_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byKind))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwGasValue))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GAS_BALL_DATA(void *pHost, CNetData* poNetData)
{
	DT_GAS_BALL_DATA *pstOut = (DT_GAS_BALL_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byKind))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwGasValue))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_COLLECT_GAS_DATA(void *pHost, CNetData* poNetData)
{
	DT_COLLECT_GAS_DATA *pstIn = (DT_COLLECT_GAS_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byCollectState))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GAS_BALL_NUM; i++)
	{
		if(-1 == EncodeDT_GAS_BALL_DATA(&pstIn->astGasBollInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wLastCoinGenNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCoinGenTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastCoinRandomUpNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCoinRandomUpTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastCoinGiveNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCoinGiveTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastGoldGenNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastGoldGenTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastGoldRandomUpNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastGoldRandomUpTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastGoldGiveNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastGoldGiveTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_COLLECT_GAS_DATA(void *pHost, CNetData* poNetData)
{
	DT_COLLECT_GAS_DATA *pstOut = (DT_COLLECT_GAS_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byCollectState))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_GAS_BALL_NUM; i++)
		{
			if(-1 == DecodeDT_GAS_BALL_DATA(&pstOut->astGasBollInfo[i], poNetData))
				return -1;
		}

	if(-1 == poNetData->DelWord(pstOut->wLastCoinGenNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCoinGenTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastCoinRandomUpNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCoinRandomUpTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastCoinGiveNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCoinGiveTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastGoldGenNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastGoldGenTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastGoldRandomUpNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastGoldRandomUpTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastGoldGiveNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastGoldGiveTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_JINGJIE_DATA(void *pHost, CNetData* poNetData)
{
	DT_JINGJIE_DATA *pstIn = (DT_JINGJIE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == EncodeDT_JINGJIE_BASE_DATA(&pstIn->stBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_COLLECT_GAS_DATA(&pstIn->stCollectData, poNetData))
		return -1;

	if(-1 == EncodeDT_SKILL_ACTION_DATA_LIST(&pstIn->stSkillActionData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_JINGJIE_DATA(void *pHost, CNetData* poNetData)
{
	DT_JINGJIE_DATA *pstOut = (DT_JINGJIE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == DecodeDT_JINGJIE_BASE_DATA(&pstOut->stBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_COLLECT_GAS_DATA(&pstOut->stCollectData, poNetData))
		return -1;

	if(-1 == DecodeDT_SKILL_ACTION_DATA_LIST(&pstOut->stSkillActionData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_CLIMB_TOWER_DATA(void *pHost, CNetData* poNetData)
{
	DT_CLIMB_TOWER_DATA *pstIn = (DT_CLIMB_TOWER_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wResetClimbTownNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwResetClimbTownTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBuyResetClimbTownNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBuyResetClimbTownTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byClimbTownInstance))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CLIMB_TOWER_DATA(void *pHost, CNetData* poNetData)
{
	DT_CLIMB_TOWER_DATA *pstOut = (DT_CLIMB_TOWER_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wResetClimbTownNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwResetClimbTownTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBuyResetClimbTownNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBuyResetClimbTownTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byClimbTownInstance))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ELITE_INSTANCE_RESET_DATA(void *pHost, CNetData* poNetData)
{
	DT_ELITE_INSTANCE_RESET_DATA *pstIn = (DT_ELITE_INSTANCE_RESET_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wResetEliteInstanceNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastResetEliteInstanceTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ELITE_INSTANCE_RESET_DATA(void *pHost, CNetData* poNetData)
{
	DT_ELITE_INSTANCE_RESET_DATA *pstOut = (DT_ELITE_INSTANCE_RESET_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wResetEliteInstanceNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastResetEliteInstanceTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_VIP_EXT_DATA(void *pHost, CNetData* poNetData)
{
	DT_VIP_EXT_DATA *pstIn = (DT_VIP_EXT_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEatGoldExperienceCnt))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastEatTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wEliteInstanceTownNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ELITE_INSTANCE_TOWN_NUM; i++)
	{
		if(i >= (INT32)pstIn->wEliteInstanceTownNum)
			break;
		if(-1 == EncodeDT_ELITE_INSTANCE_RESET_DATA(&pstIn->astEliteInstanceData[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_VIP_EXT_DATA(void *pHost, CNetData* poNetData)
{
	DT_VIP_EXT_DATA *pstOut = (DT_VIP_EXT_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEatGoldExperienceCnt))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastEatTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wEliteInstanceTownNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ELITE_INSTANCE_TOWN_NUM; i++)
	{
		if(i >= (INT32)pstOut->wEliteInstanceTownNum)
			break;
		if(-1 == DecodeDT_ELITE_INSTANCE_RESET_DATA(&pstOut->astEliteInstanceData[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_LUAACTIVITY_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_LUAACTIVITY_RECORD_DATA *pstIn = (DT_LUAACTIVITY_RECORD_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wFinishNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwFinishTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LUAACTIVITY_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_LUAACTIVITY_RECORD_DATA *pstOut = (DT_LUAACTIVITY_RECORD_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wFinishNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwFinishTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_LUAACTIVITY_RECORD_LIST_DATA(void *pHost, CNetData* poNetData)
{
	DT_LUAACTIVITY_RECORD_LIST_DATA *pstIn = (DT_LUAACTIVITY_RECORD_LIST_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byRecordNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_LUAACTIVITY_RECORD_NUM; i++)
	{
		if(i >= (INT32)pstIn->byRecordNum)
			break;
		if(-1 == EncodeDT_LUAACTIVITY_RECORD_DATA(&pstIn->astHaveFinishActivityInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LUAACTIVITY_RECORD_LIST_DATA(void *pHost, CNetData* poNetData)
{
	DT_LUAACTIVITY_RECORD_LIST_DATA *pstOut = (DT_LUAACTIVITY_RECORD_LIST_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byRecordNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_LUAACTIVITY_RECORD_NUM; i++)
	{
		if(i >= (INT32)pstOut->byRecordNum)
			break;
		if(-1 == DecodeDT_LUAACTIVITY_RECORD_DATA(&pstOut->astHaveFinishActivityInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_BOSSB_HURT_RATE(void *pHost, CNetData* poNetData)
{
	DT_BOSSB_HURT_RATE *pstIn = (DT_BOSSB_HURT_RATE*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDispName, sizeof(pstIn->aszDispName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwHurtValue))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwHurtRate))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwHurtRank))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BOSSB_HURT_RATE(void *pHost, CNetData* poNetData)
{
	DT_BOSSB_HURT_RATE *pstOut = (DT_BOSSB_HURT_RATE*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDispName, sizeof(pstOut->aszDispName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwHurtValue))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwHurtRate))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwHurtRank))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BOSS_BATTLE_ENC_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_ENC_DATA *pstIn = (DT_BOSS_BATTLE_ENC_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDispName, sizeof(pstIn->aszDispName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCoin))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwJingJie))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BOSS_BATTLE_ENC_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_ENC_DATA *pstOut = (DT_BOSS_BATTLE_ENC_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDispName, sizeof(pstOut->aszDispName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCoin))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwJingJie))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BOSS_BATTLE_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_PLAYER_DATA *pstIn = (DT_BOSS_BATTLE_PLAYER_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwLastEnterActivityTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastLeaveActivityTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wIncPowerRate))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwTotalHurtValue))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwHurtRank))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwEncCoin))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwEncJingJie))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPosX))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPosY))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSyncStartIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastReliveType))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastReliveTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastDeadTime))
		return -1;

	if(-1 == EncodeDT_BOSSB_HURT_RATE(&pstIn->stMyHurtInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_BOSS_BATTLE_ENC_DATA(&pstIn->stHurtEncInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_BOSS_BATTLE_ENC_DATA(&pstIn->stHeroEncInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNotity))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BOSS_BATTLE_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_PLAYER_DATA *pstOut = (DT_BOSS_BATTLE_PLAYER_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwLastEnterActivityTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastLeaveActivityTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wIncPowerRate))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwTotalHurtValue))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwHurtRank))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwEncCoin))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwEncJingJie))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPosX))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPosY))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSyncStartIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastReliveType))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastReliveTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastDeadTime))
		return -1;

	if(-1 == DecodeDT_BOSSB_HURT_RATE(&pstOut->stMyHurtInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_BOSS_BATTLE_ENC_DATA(&pstOut->stHurtEncInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_BOSS_BATTLE_ENC_DATA(&pstOut->stHeroEncInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNotity))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BOSS_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_DATA *pstIn = (DT_BOSS_BATTLE_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwOverTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStartTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwSaveTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byActivityID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBossID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNeedUpgrade))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwTotalHP))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCurHP))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BOSS_ENC_TOP_NUM; i++)
	{
		if(-1 == EncodeDT_BOSSB_HURT_RATE(&pstIn->astTopHurtInfo[i], poNetData))
			return -1;
	}

	for(i = 0; i < MAX_BOSS_ENC_TOP_NUM; i++)
	{
		if(-1 == EncodeDT_BOSS_BATTLE_ENC_DATA(&pstIn->astBossBTopEncInfo[i], poNetData))
			return -1;
	}

	if(-1 == EncodeDT_BOSS_BATTLE_ENC_DATA(&pstIn->stBossBKillEncInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BOSS_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_DATA *pstOut = (DT_BOSS_BATTLE_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwOverTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStartTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwSaveTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byActivityID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBossID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNeedUpgrade))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwTotalHP))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCurHP))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_BOSS_ENC_TOP_NUM; i++)
		{
			if(-1 == DecodeDT_BOSSB_HURT_RATE(&pstOut->astTopHurtInfo[i], poNetData))
				return -1;
		}

		for(i = 0; i < MAX_BOSS_ENC_TOP_NUM; i++)
		{
			if(-1 == DecodeDT_BOSS_BATTLE_ENC_DATA(&pstOut->astBossBTopEncInfo[i], poNetData))
				return -1;
		}

	if(-1 == DecodeDT_BOSS_BATTLE_ENC_DATA(&pstOut->stBossBKillEncInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA *pstIn = (DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwOverTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byActivityID))
		return -1;

	if(-1 == EncodeDT_BOSSB_HURT_RATE(&pstIn->stMyHurtInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_BOSS_BATTLE_ENC_DATA(&pstIn->stHurtEncInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_BOSS_BATTLE_ENC_DATA(&pstIn->stHeroEncInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA *pstOut = (DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwOverTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byActivityID))
		return -1;

	if(-1 == DecodeDT_BOSSB_HURT_RATE(&pstOut->stMyHurtInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_BOSS_BATTLE_ENC_DATA(&pstOut->stHurtEncInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_BOSS_BATTLE_ENC_DATA(&pstOut->stHeroEncInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST *pstIn = (DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BOSSB_BATTLE_LOG_NUM; i++)
	{
		if(i >= (INT32)pstIn->byNum)
			break;
		if(-1 == EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(&pstIn->astBossBattleBL[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST *pstOut = (DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BOSSB_BATTLE_LOG_NUM; i++)
	{
		if(i >= (INT32)pstOut->byNum)
			break;
		if(-1 == DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(&pstOut->astBossBattleBL[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_BOSS_BATTLE_MODULE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_MODULE_DATA *pstIn = (DT_BOSS_BATTLE_MODULE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BOSSB_NUM_PER_DAY; i++)
	{
		if(i >= (INT32)pstIn->byNum)
			break;
		if(-1 == EncodeDT_BOSS_BATTLE_DATA(&pstIn->astBossBattleInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BOSS_BATTLE_MODULE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BOSS_BATTLE_MODULE_DATA *pstOut = (DT_BOSS_BATTLE_MODULE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BOSSB_NUM_PER_DAY; i++)
	{
		if(i >= (INT32)pstOut->byNum)
			break;
		if(-1 == DecodeDT_BOSS_BATTLE_DATA(&pstOut->astBossBattleInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_ACTIVITY_ENCOURAGE_INFO(void *pHost, CNetData* poNetData)
{
	DT_ACTIVITY_ENCOURAGE_INFO *pstIn = (DT_ACTIVITY_ENCOURAGE_INFO*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wActivityID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwActivityTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byActivityTimes))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ACTIVITY_ENCOURAGE_INFO(void *pHost, CNetData* poNetData)
{
	DT_ACTIVITY_ENCOURAGE_INFO *pstOut = (DT_ACTIVITY_ENCOURAGE_INFO*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wActivityID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwActivityTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byActivityTimes))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ACTIVITY_ENCOURAGE_DATA(void *pHost, CNetData* poNetData)
{
	DT_ACTIVITY_ENCOURAGE_DATA *pstIn = (DT_ACTIVITY_ENCOURAGE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byRechargeFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRechargeRecvTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byScoreFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwScoreRecvTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wInfoNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ACTIVITY_ENCOURAGE_INFO_NUM; i++)
	{
		if(i >= (INT32)pstIn->wInfoNum)
			break;
		if(-1 == EncodeDT_ACTIVITY_ENCOURAGE_INFO(&pstIn->astActivityInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ACTIVITY_ENCOURAGE_DATA(void *pHost, CNetData* poNetData)
{
	DT_ACTIVITY_ENCOURAGE_DATA *pstOut = (DT_ACTIVITY_ENCOURAGE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byRechargeFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRechargeRecvTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byScoreFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwScoreRecvTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wInfoNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ACTIVITY_ENCOURAGE_INFO_NUM; i++)
	{
		if(i >= (INT32)pstOut->wInfoNum)
			break;
		if(-1 == DecodeDT_ACTIVITY_ENCOURAGE_INFO(&pstOut->astActivityInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_RDC_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_RDC_PLAYER_DATA *pstIn = (DT_RDC_PLAYER_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDispName, sizeof(pstIn->aszDispName)))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMainHeroKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byRobotFunc))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPower))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHaveChallengeFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RDC_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_RDC_PLAYER_DATA *pstOut = (DT_RDC_PLAYER_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDispName, sizeof(pstOut->aszDispName)))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMainHeroKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byRobotFunc))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPower))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHaveChallengeFlag))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_RANDOM_CHALLENGE_DATA(void *pHost, CNetData* poNetData)
{
	DT_RANDOM_CHALLENGE_DATA *pstIn = (DT_RANDOM_CHALLENGE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wCurChallengePoint))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRCPoinRefreshTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastBuyRCPoinTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastBuyRCPoinNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRCTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastRCRet))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byClrCDFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPlayerNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GET_RDC_NUM; i++)
	{
		if(i >= (INT32)pstIn->byPlayerNum)
			break;
		if(-1 == EncodeDT_RDC_PLAYER_DATA(&pstIn->astPlayerInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RANDOM_CHALLENGE_DATA(void *pHost, CNetData* poNetData)
{
	DT_RANDOM_CHALLENGE_DATA *pstOut = (DT_RANDOM_CHALLENGE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wCurChallengePoint))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRCPoinRefreshTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastBuyRCPoinTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastBuyRCPoinNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRCTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastRCRet))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byClrCDFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPlayerNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_GET_RDC_NUM; i++)
	{
		if(i >= (INT32)pstOut->byPlayerNum)
			break;
		if(-1 == DecodeDT_RDC_PLAYER_DATA(&pstOut->astPlayerInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_WORSHIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_WORSHIP_DATA *pstIn = (DT_WORSHIP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byWorshipType))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byWorshipTypeLevel))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastWorshipTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_WORSHIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_WORSHIP_DATA *pstOut = (DT_WORSHIP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byWorshipType))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byWorshipTypeLevel))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastWorshipTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_WORSHIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_WORSHIP_DATA_LST *pstIn = (DT_WORSHIP_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_WORSHIP_NUM; i++)
	{
		if(i >= (INT32)pstIn->wNum)
			break;
		if(-1 == EncodeDT_WORSHIP_DATA(&pstIn->astWorshipInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_WORSHIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_WORSHIP_DATA_LST *pstOut = (DT_WORSHIP_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_WORSHIP_NUM; i++)
	{
		if(i >= (INT32)pstOut->wNum)
			break;
		if(-1 == DecodeDT_WORSHIP_DATA(&pstOut->astWorshipInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_CHAPTER_SCORE_DATA(void *pHost, CNetData* poNetData)
{
	DT_CHAPTER_SCORE_DATA *pstIn = (DT_CHAPTER_SCORE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneidx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wChapterIdx))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwtime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CHAPTER_SCORE_DATA(void *pHost, CNetData* poNetData)
{
	DT_CHAPTER_SCORE_DATA *pstOut = (DT_CHAPTER_SCORE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneidx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wChapterIdx))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwtime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_ACCUMULATED_SCORE_DATA(void *pHost, CNetData* poNetData)
{
	DT_ACCUMULATED_SCORE_DATA *pstIn = (DT_ACCUMULATED_SCORE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wgrade))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwtime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ACCUMULATED_SCORE_DATA(void *pHost, CNetData* poNetData)
{
	DT_ACCUMULATED_SCORE_DATA *pstOut = (DT_ACCUMULATED_SCORE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wgrade))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwtime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_SCORE_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_SCORE_DATA_LST *pstIn = (DT_SCORE_DATA_LST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byChapterNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TOWN_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byChapterNum)
			break;
		if(-1 == EncodeDT_CHAPTER_SCORE_DATA(&pstIn->astChapterData[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byAccumulatedNum))
		return -1;

	for(i = 0; i < MAX_TOWN_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byAccumulatedNum)
			break;
		if(-1 == EncodeDT_ACCUMULATED_SCORE_DATA(&pstIn->astAccumulatedData[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCORE_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_SCORE_DATA_LST *pstOut = (DT_SCORE_DATA_LST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byChapterNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TOWN_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byChapterNum)
			break;
		if(-1 == DecodeDT_CHAPTER_SCORE_DATA(&pstOut->astChapterData[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byAccumulatedNum))
		return -1;

	for(i = 0; i < MAX_TOWN_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byAccumulatedNum)
			break;
		if(-1 == DecodeDT_ACCUMULATED_SCORE_DATA(&pstOut->astAccumulatedData[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_EQUIP_LEVEL_COMPOSE_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_LEVEL_COMPOSE_DATA *pstIn = (DT_EQUIP_LEVEL_COMPOSE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCurLowColor))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHitKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EQUIP_LEVEL_COMPOSE_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_LEVEL_COMPOSE_DATA *pstOut = (DT_EQUIP_LEVEL_COMPOSE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCurLowColor))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHitKindID))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_EQUIP_COMPOSE_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_COMPOSE_DATA *pstIn = (DT_EQUIP_COMPOSE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byEquipLevelNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_EQUIPLEVEL_NUM; i++)
	{
		if(i >= (INT32)pstIn->byEquipLevelNum)
			break;
		if(-1 == EncodeDT_EQUIP_LEVEL_COMPOSE_DATA(&pstIn->astEquipLevelInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddUint64(pstIn->qwLastComposeTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastComposeEquipLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EQUIP_COMPOSE_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_COMPOSE_DATA *pstOut = (DT_EQUIP_COMPOSE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byEquipLevelNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_EQUIPLEVEL_NUM; i++)
	{
		if(i >= (INT32)pstOut->byEquipLevelNum)
			break;
		if(-1 == DecodeDT_EQUIP_LEVEL_COMPOSE_DATA(&pstOut->astEquipLevelInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelUint64(pstOut->qwLastComposeTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastComposeEquipLevel))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_GATHER_SCIENCE_DATA(void *pHost, CNetData* poNetData)
{
	DT_GATHER_SCIENCE_DATA *pstIn = (DT_GATHER_SCIENCE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wCoinNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwCoinLastTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wGoldNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwGoldLastTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byIncrease))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GATHER_SCIENCE_DATA(void *pHost, CNetData* poNetData)
{
	DT_GATHER_SCIENCE_DATA *pstOut = (DT_GATHER_SCIENCE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wCoinNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwCoinLastTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wGoldNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwGoldLastTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byIncrease))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BEGBLUE_RES(void *pHost, CNetData* poNetData)
{
	DT_BEGBLUE_RES *pstIn = (DT_BEGBLUE_RES*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSmileNum))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwBlueUnit))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwStoryUnit))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wXnMultiple))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszLuckPrompt, sizeof(pstIn->aszLuckPrompt)))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwCreateTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwUpdateTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BEGBLUE_RES(void *pHost, CNetData* poNetData)
{
	DT_BEGBLUE_RES *pstOut = (DT_BEGBLUE_RES*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSmileNum))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwBlueUnit))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwStoryUnit))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wXnMultiple))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszLuckPrompt, sizeof(pstOut->aszLuckPrompt)))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwCreateTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwUpdateTime))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_BEGBLUE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BEGBLUE_DATA *pstIn = (DT_BEGBLUE_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwBegBlueLastTime))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszClimbTowerPrompt, sizeof(pstIn->aszClimbTowerPrompt)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwMaxBegTimes))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUsedBegTimes))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFreeImproveLuckMaxTimes))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUsedFreeImproveLuckTimes))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwGoldImproveLuckIdx))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwNextImproveLuckGold))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byIsHaveToComfirmBegBlueInfo))
		return -1;

	if(-1 == EncodeDT_BEGBLUE_RES(&pstIn->stBegBlueRes, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BEGBLUE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BEGBLUE_DATA *pstOut = (DT_BEGBLUE_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwBegBlueLastTime))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszClimbTowerPrompt, sizeof(pstOut->aszClimbTowerPrompt)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwMaxBegTimes))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUsedBegTimes))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFreeImproveLuckMaxTimes))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUsedFreeImproveLuckTimes))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwGoldImproveLuckIdx))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwNextImproveLuckGold))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byIsHaveToComfirmBegBlueInfo))
		return -1;

	if(-1 == DecodeDT_BEGBLUE_RES(&pstOut->stBegBlueRes, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLAYER_NOPVP_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_NOPVP_DATA *pstIn = (DT_PLAYER_NOPVP_DATA*)pHost;

	if(-1 == EncodeDT_PLAYER_BASE_DATA(&pstIn->stBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_HERO_DATA_LIST(&pstIn->stHeroData, poNetData))
		return -1;

	if(-1 == EncodeDT_BAG_EQUIP_DATA_LST(&pstIn->stBagEquipData, poNetData))
		return -1;

	if(-1 == EncodeDT_BAG_GOODS_DATA_LST(&pstIn->stBagGoodsData, poNetData))
		return -1;

	if(-1 == EncodeDT_BUILD_DATA_LIST(&pstIn->stBuildData, poNetData))
		return -1;

	if(-1 == EncodeDT_TASK_DATA_LIST(&pstIn->stTaskData, poNetData))
		return -1;

	if(-1 == EncodeDT_LOCALE_DATA(&pstIn->stLocaleData, poNetData))
		return -1;

	if(-1 == EncodeDT_STATE_DATA(&pstIn->stCurState, poNetData))
		return -1;

	if(-1 == EncodeDT_ONLINE_ENCOURAGE_RECORD_DATA(&pstIn->stOnlineEncRecdData, poNetData))
		return -1;

	if(-1 == EncodeDT_LOGIN_ENCOURAGE_RECORD_DATA(&pstIn->stLoginEncRecdData, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_DATA_LIST(&pstIn->stCommonInstanceData, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_DATA_LIST(&pstIn->stEliteInstanceData, poNetData))
		return -1;

	if(-1 == EncodeDT_MONEY_TREE_DATA(&pstIn->stMoneyTreeData, poNetData))
		return -1;

	if(-1 == EncodeDT_ONHOOK_RCD_DATA(&pstIn->stOnhookRcdData, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_BATTLE_DATA(&pstIn->stCommonBattleRcd, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_BATTLE_DATA(&pstIn->stEliteBattleRcd, poNetData))
		return -1;

	if(-1 == EncodeDT_JINGJIE_DATA(&pstIn->stJingJieData, poNetData))
		return -1;

	if(-1 == EncodeDT_PHYSTRENGTH_DATA(&pstIn->stPhystrengthData, poNetData))
		return -1;

	if(-1 == EncodeDT_FEW_PARAM_DATA(&pstIn->stFewParamData, poNetData))
		return -1;

	if(-1 == EncodeDT_CROP_DATA(&pstIn->stCropData, poNetData))
		return -1;

	if(-1 == EncodeDT_SCIENCE_TREE_DATA(&pstIn->stScienceTree, poNetData))
		return -1;

	if(-1 == EncodeDT_FORMATION_DATA(&pstIn->stFormationInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szLastOrderID, sizeof(pstIn->szLastOrderID)))
		return -1;

	if(-1 == EncodeDT_EAT_BREAD_DATA(&pstIn->stEatBreadInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_YABIAO_DATA(&pstIn->stYabiaoInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_BATTLE_DATA(&pstIn->stClimbTowerBattleRcd, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_DATA_LIST(&pstIn->stClimbTowerInstanceData, poNetData))
		return -1;

	if(-1 == EncodeDT_YABIAO_BATTLELOG_DATA_LIST(&pstIn->stYabiaoBattleLog, poNetData))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyNoticeID[i]))
			return -1;
	}

	if(-1 == poNetData->AddUint64(pstIn->qwLoginTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLogoffTime))
		return -1;

	if(-1 == EncodeDT_CLIMB_TOWER_DATA(&pstIn->stClimbTowerData, poNetData))
		return -1;

	if(-1 == EncodeDT_AWAKEN_DATA(&pstIn->stAwakenData, poNetData))
		return -1;

	if(-1 == EncodeDT_VIP_EXT_DATA(&pstIn->stVipExtData, poNetData))
		return -1;

	if(-1 == EncodeDT_LUAACTIVITY_RECORD_LIST_DATA(&pstIn->stLuaActivityRecordData, poNetData))
		return -1;

	if(-1 == EncodeDT_COLLECT_GAS_DATA(&pstIn->stGasCollectData, poNetData))
		return -1;

	if(-1 == EncodeDT_BOSS_BATTLE_PLAYER_DATA(&pstIn->stBossBattleData, poNetData))
		return -1;

	if(-1 == EncodeDT_ACTIVITY_ENCOURAGE_DATA(&pstIn->stActivityEncourageData, poNetData))
		return -1;

	if(-1 == EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(&pstIn->stBossBBLData, poNetData))
		return -1;

	if(-1 == EncodeDT_RANDOM_CHALLENGE_DATA(&pstIn->stRdChallengeData, poNetData))
		return -1;

	if(-1 == EncodeDT_WORSHIP_DATA_LST(&pstIn->stWorshipData, poNetData))
		return -1;

	if(-1 == EncodeDT_SCORE_DATA_LST(&pstIn->stScoreData, poNetData))
		return -1;

	if(-1 == EncodeDT_EQUIP_COMPOSE_DATA(&pstIn->stEquipComposeData, poNetData))
		return -1;

	if(-1 == EncodeDT_GATHER_SCIENCE_DATA(&pstIn->stGatherScienceData, poNetData))
		return -1;

	if(-1 == EncodeDT_BEGBLUE_DATA(&pstIn->stBegBlueData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLAYER_NOPVP_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_NOPVP_DATA *pstOut = (DT_PLAYER_NOPVP_DATA*)pHost;

	if(-1 == DecodeDT_PLAYER_BASE_DATA(&pstOut->stBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_HERO_DATA_LIST(&pstOut->stHeroData, poNetData))
		return -1;

	if(-1 == DecodeDT_BAG_EQUIP_DATA_LST(&pstOut->stBagEquipData, poNetData))
		return -1;

	if(-1 == DecodeDT_BAG_GOODS_DATA_LST(&pstOut->stBagGoodsData, poNetData))
		return -1;

	if(-1 == DecodeDT_BUILD_DATA_LIST(&pstOut->stBuildData, poNetData))
		return -1;

	if(-1 == DecodeDT_TASK_DATA_LIST(&pstOut->stTaskData, poNetData))
		return -1;

	if(-1 == DecodeDT_LOCALE_DATA(&pstOut->stLocaleData, poNetData))
		return -1;

	if(-1 == DecodeDT_STATE_DATA(&pstOut->stCurState, poNetData))
		return -1;

	if(-1 == DecodeDT_ONLINE_ENCOURAGE_RECORD_DATA(&pstOut->stOnlineEncRecdData, poNetData))
		return -1;

	if(-1 == DecodeDT_LOGIN_ENCOURAGE_RECORD_DATA(&pstOut->stLoginEncRecdData, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_DATA_LIST(&pstOut->stCommonInstanceData, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_DATA_LIST(&pstOut->stEliteInstanceData, poNetData))
		return -1;

	if(-1 == DecodeDT_MONEY_TREE_DATA(&pstOut->stMoneyTreeData, poNetData))
		return -1;

	if(-1 == DecodeDT_ONHOOK_RCD_DATA(&pstOut->stOnhookRcdData, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_BATTLE_DATA(&pstOut->stCommonBattleRcd, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_BATTLE_DATA(&pstOut->stEliteBattleRcd, poNetData))
		return -1;

	if(-1 == DecodeDT_JINGJIE_DATA(&pstOut->stJingJieData, poNetData))
		return -1;

	if(-1 == DecodeDT_PHYSTRENGTH_DATA(&pstOut->stPhystrengthData, poNetData))
		return -1;

	if(-1 == DecodeDT_FEW_PARAM_DATA(&pstOut->stFewParamData, poNetData))
		return -1;

	if(-1 == DecodeDT_CROP_DATA(&pstOut->stCropData, poNetData))
		return -1;

	if(-1 == DecodeDT_SCIENCE_TREE_DATA(&pstOut->stScienceTree, poNetData))
		return -1;

	if(-1 == DecodeDT_FORMATION_DATA(&pstOut->stFormationInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szLastOrderID, sizeof(pstOut->szLastOrderID)))
		return -1;

	if(-1 == DecodeDT_EAT_BREAD_DATA(&pstOut->stEatBreadInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_YABIAO_DATA(&pstOut->stYabiaoInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_BATTLE_DATA(&pstOut->stClimbTowerBattleRcd, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_DATA_LIST(&pstOut->stClimbTowerInstanceData, poNetData))
		return -1;

	if(-1 == DecodeDT_YABIAO_BATTLELOG_DATA_LIST(&pstOut->stYabiaoBattleLog, poNetData))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TOKEN_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyNoticeID[i]))
			return -1;
	}

	if(-1 == poNetData->DelUint64(pstOut->qwLoginTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLogoffTime))
		return -1;

	if(-1 == DecodeDT_CLIMB_TOWER_DATA(&pstOut->stClimbTowerData, poNetData))
		return -1;

	if(-1 == DecodeDT_AWAKEN_DATA(&pstOut->stAwakenData, poNetData))
		return -1;

	if(-1 == DecodeDT_VIP_EXT_DATA(&pstOut->stVipExtData, poNetData))
		return -1;

	if(-1 == DecodeDT_LUAACTIVITY_RECORD_LIST_DATA(&pstOut->stLuaActivityRecordData, poNetData))
		return -1;

	if(-1 == DecodeDT_COLLECT_GAS_DATA(&pstOut->stGasCollectData, poNetData))
		return -1;

	if(-1 == DecodeDT_BOSS_BATTLE_PLAYER_DATA(&pstOut->stBossBattleData, poNetData))
		return -1;

	if(-1 == DecodeDT_ACTIVITY_ENCOURAGE_DATA(&pstOut->stActivityEncourageData, poNetData))
		return -1;

	if(-1 == DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(&pstOut->stBossBBLData, poNetData))
		return -1;

	if(-1 == DecodeDT_RANDOM_CHALLENGE_DATA(&pstOut->stRdChallengeData, poNetData))
		return -1;

	if(-1 == DecodeDT_WORSHIP_DATA_LST(&pstOut->stWorshipData, poNetData))
		return -1;

	if(-1 == DecodeDT_SCORE_DATA_LST(&pstOut->stScoreData, poNetData))
		return -1;

	if(-1 == DecodeDT_EQUIP_COMPOSE_DATA(&pstOut->stEquipComposeData, poNetData))
		return -1;

	if(-1 == DecodeDT_GATHER_SCIENCE_DATA(&pstOut->stGatherScienceData, poNetData))
		return -1;

	if(-1 == DecodeDT_BEGBLUE_DATA(&pstOut->stBegBlueData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLAYER_PVP_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_PVP_DATA *pstIn = (DT_PLAYER_PVP_DATA*)pHost;

	if(-1 == EncodeDT_RACE_DATA(&pstIn->stRaceData, poNetData))
		return -1;

	if(-1 == EncodeDT_RACE_BATTLELOG_DATA_LIST(&pstIn->stRaceBattleLog, poNetData))
		return -1;

	if(-1 == EncodeDT_PLUNDER_BASE_DATA(&pstIn->stPlunderBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_PLUNDER_BATTLELOG_DATA_LIST(&pstIn->stPlunderBattleLog, poNetData))
		return -1;

	if(-1 == EncodeDT_CAPTURE_DATA_LIST(&pstIn->stCaptureData, poNetData))
		return -1;

	if(-1 == EncodeDT_ENEMY_DATA_LIST(&pstIn->stEnemyData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLAYER_PVP_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_PVP_DATA *pstOut = (DT_PLAYER_PVP_DATA*)pHost;

	if(-1 == DecodeDT_RACE_DATA(&pstOut->stRaceData, poNetData))
		return -1;

	if(-1 == DecodeDT_RACE_BATTLELOG_DATA_LIST(&pstOut->stRaceBattleLog, poNetData))
		return -1;

	if(-1 == DecodeDT_PLUNDER_BASE_DATA(&pstOut->stPlunderBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_PLUNDER_BATTLELOG_DATA_LIST(&pstOut->stPlunderBattleLog, poNetData))
		return -1;

	if(-1 == DecodeDT_CAPTURE_DATA_LIST(&pstOut->stCaptureData, poNetData))
		return -1;

	if(-1 == DecodeDT_ENEMY_DATA_LIST(&pstOut->stEnemyData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_DATA *pstIn = (DT_PLAYER_DATA*)pHost;

	if(-1 == EncodeDT_PLAYER_NOPVP_DATA(&pstIn->stPlayerNoPvpData, poNetData))
		return -1;

	if(-1 == EncodeDT_PLAYER_PVP_DATA(&pstIn->stPlayerPvpData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_DATA *pstOut = (DT_PLAYER_DATA*)pHost;

	if(-1 == DecodeDT_PLAYER_NOPVP_DATA(&pstOut->stPlayerNoPvpData, poNetData))
		return -1;

	if(-1 == DecodeDT_PLAYER_PVP_DATA(&pstOut->stPlayerPvpData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData)
{
	DT_EXT_FUNC_PARAM *pstIn = (DT_EXT_FUNC_PARAM*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byParam1))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wParam2))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParam3))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData)
{
	DT_EXT_FUNC_PARAM *pstOut = (DT_EXT_FUNC_PARAM*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byParam1))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wParam2))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParam3))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData)
{
	DT_EXT_FUNC_PARAM_LST *pstIn = (DT_EXT_FUNC_PARAM_LST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFuncNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_EXT_FUN_NUM; i++)
	{
		if(i >= (INT32)pstIn->byFuncNum)
			break;
		if(-1 == EncodeDT_EXT_FUNC_PARAM(&pstIn->astExtFuncParaInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData)
{
	DT_EXT_FUNC_PARAM_LST *pstOut = (DT_EXT_FUNC_PARAM_LST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFuncNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_EXT_FUN_NUM; i++)
	{
		if(i >= (INT32)pstOut->byFuncNum)
			break;
		if(-1 == DecodeDT_EXT_FUNC_PARAM(&pstOut->astExtFuncParaInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_PLAYER_DATA *pstIn = (DT_FACTION_PLAYER_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwJoinPlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byJoinState))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byJobType))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwContribution))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCoinContributionTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wTodaysCoinContributionTimes))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTodaysCoinContributionEncourage))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastGoldContributionTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wTodaysGoldContributionTimes))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTodaysGoldContributionEncourage))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_PLAYER_DATA *pstOut = (DT_FACTION_PLAYER_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwJoinPlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byJoinState))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byJobType))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwContribution))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCoinContributionTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wTodaysCoinContributionTimes))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTodaysCoinContributionEncourage))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastGoldContributionTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wTodaysGoldContributionTimes))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTodaysGoldContributionEncourage))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_PLAYER_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_FACTION_PLAYER_DATA_LST *pstIn = (DT_FACTION_PLAYER_DATA_LST*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwFactionNum))
		return -1;

	INT32 i;
	for(i = 0; i < DT_FACTION_PLAYER_NUM; i++)
	{
		if(i >= (INT32)pstIn->dwFactionNum)
			break;
		if(-1 == EncodeDT_FACTION_PLAYER_DATA(&pstIn->astFactionPlayerData[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddDword(pstIn->dwFactionApplyNum))
		return -1;

	for(i = 0; i < DT_FACTION_PLAYER_NUM; i++)
	{
		if(i >= (INT32)pstIn->dwFactionApplyNum)
			break;
		if(-1 == poNetData->AddDword(pstIn->adwFactionApplyPlayerID[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_PLAYER_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_FACTION_PLAYER_DATA_LST *pstOut = (DT_FACTION_PLAYER_DATA_LST*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwFactionNum))
		return -1;

	INT32 i;
	for(i = 0; i < DT_FACTION_PLAYER_NUM; i++)
	{
		if(i >= (INT32)pstOut->dwFactionNum)
			break;
		if(-1 == DecodeDT_FACTION_PLAYER_DATA(&pstOut->astFactionPlayerData[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelDword(pstOut->dwFactionApplyNum))
		return -1;

	for(i = 0; i < DT_FACTION_PLAYER_NUM; i++)
	{
		if(i >= (INT32)pstOut->dwFactionApplyNum)
			break;
		if(-1 == poNetData->DelDword(pstOut->adwFactionApplyPlayerID[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_LOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_LOG_DATA *pstIn = (DT_FACTION_LOG_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID1))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPlayerID2))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAction))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParm1))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParm2))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwParm3))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_LOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_LOG_DATA *pstOut = (DT_FACTION_LOG_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID1))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPlayerID2))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAction))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParm1))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParm2))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwParm3))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_LOG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_FACTION_LOG_DATA_LST *pstIn = (DT_FACTION_LOG_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wFactionLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FACTION_LOG_NUM; i++)
	{
		if(i >= (INT32)pstIn->wFactionLogNum)
			break;
		if(-1 == EncodeDT_FACTION_LOG_DATA(&pstIn->astFactionLogData[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_LOG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_FACTION_LOG_DATA_LST *pstOut = (DT_FACTION_LOG_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wFactionLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FACTION_LOG_NUM; i++)
	{
		if(i >= (INT32)pstOut->wFactionLogNum)
			break;
		if(-1 == DecodeDT_FACTION_LOG_DATA(&pstOut->astFactionLogData[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_BASE_DATA *pstIn = (DT_FACTION_BASE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwFactionID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszFactionName, sizeof(pstIn->aszFactionName)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFactionRank))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFactionLevel))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFactionGold))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszFactionDescName, sizeof(pstIn->aszFactionDescName)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszFactionNotice, sizeof(pstIn->aszFactionNotice)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszFactionAnnouncement, sizeof(pstIn->aszFactionAnnouncement)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_BASE_DATA *pstOut = (DT_FACTION_BASE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwFactionID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszFactionName, sizeof(pstOut->aszFactionName)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFactionRank))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFactionLevel))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFactionGold))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszFactionDescName, sizeof(pstOut->aszFactionDescName)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszFactionNotice, sizeof(pstOut->aszFactionNotice)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszFactionAnnouncement, sizeof(pstOut->aszFactionAnnouncement)))
		return -1;

	return sizeof(*pstOut);
}

INT32 EncodeDT_FACTION_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_DATA *pstIn = (DT_FACTION_DATA*)pHost;

	if(-1 == EncodeDT_FACTION_BASE_DATA(&pstIn->stFactionBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_FACTION_PLAYER_DATA_LST(&pstIn->stFactionPlayerDataLst, poNetData))
		return -1;

	if(-1 == EncodeDT_FACTION_LOG_DATA_LST(&pstIn->stFactionLogDataLst, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FACTION_DATA(void *pHost, CNetData* poNetData)
{
	DT_FACTION_DATA *pstOut = (DT_FACTION_DATA*)pHost;

	if(-1 == DecodeDT_FACTION_BASE_DATA(&pstOut->stFactionBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_FACTION_PLAYER_DATA_LST(&pstOut->stFactionPlayerDataLst, poNetData))
		return -1;

	if(-1 == DecodeDT_FACTION_LOG_DATA_LST(&pstOut->stFactionLogDataLst, poNetData))
		return -1;

	return sizeof(*pstOut);
}

