#include "logicfunc.h"
#include <logic/player/player.h>
#include <common/server/utility.h>


BOOL CLogicFunc::Encourage(CPlayer* poOwner, SBurst* pstBurst, BOOL bFirstFlag)
{	
	/*
	SEncouragePropValue stEncouragePropValue;
	if(FALSE == FirmPropValue(stEncouragePropValue, pstBurst, bFirstFlag))
	{
		return FALSE;
	}

	switch(stEncouragePropValue.eBurstKind)
	{
	case EBKIND_COIN:		
		{
			poOwner->GetDT_PLAYER_BASE_DATA().qwCoin += stEncouragePropValue.dwValue;
			stDT_ENCOURAGE_DATA.dwCoin += stEncouragePropValue.dwValue;
		}
		break;
	case EBKIND_GOLD:
		{
			poOwner->GetDT_PLAYER_BASE_DATA().qwGold += stEncouragePropValue.dwValue;
			stDT_ENCOURAGE_DATA.dwGold += stEncouragePropValue.dwValue;
		}
		break;
	case EBKIND_EXPERIENCE:
		{
			poOwner->AllotExperience(stEncouragePropValue.dwValue);
		}
		break;
	case EBKIND_PHYSTRENGTH:
		{
			UINT16 wTmp = poOwner->GetDT_PLAYER_BASE_DATA().wPhyStrength + stEncouragePropValue.dwValue;
			if(wTmp > MAX_PHYSTRENGTH)
			{
				wTmp = MAX_PHYSTRENGTH - poOwner->GetDT_PLAYER_BASE_DATA().wPhyStrength;
			}
			poOwner->GetDT_PLAYER_BASE_DATA().wPhyStrength += wTmp;			
			stDT_ENCOURAGE_DATA.wPhyStrength += wTmp;
		}
		break;	
	case EBKIND_ITEM:
		{
			return EncourageItem(poOwner, stEncouragePropValue.wSubKind, stEncouragePropValue.wNum);
		}
		break;
	case EBKIND_ATTRIBUTE:
		{
			//属性，不是奖励，只添加到当前属性上
			switch(stEncouragePropValue.wSubKind)
			{
			case EBAK_HP:
				{
					INT32 nTmp = (INT32)(poOwner->GetCurDT_BATTLE_ATTRIBUTE().dwHP) + (INT32)(stEncouragePropValue.wNum);
					if(nTmp > (INT32)poOwner->GetMaxDT_BATTLE_ATTRIBUTE().dwHP )
					{
						nTmp = poOwner->GetMaxDT_BATTLE_ATTRIBUTE().dwHP;
					}
					poOwner->GetCurDT_BATTLE_ATTRIBUTE().dwHP = nTmp;
				}
				break;
			case EBAK_ATTACK:
				{
					poOwner->GetCurDT_BATTLE_ATTRIBUTE().dwAttack += stEncouragePropValue.wNum;
				}
				break;
			case EBAK_DEFENSE:
				{
					poOwner->GetCurDT_BATTLE_ATTRIBUTE().wDefense += stEncouragePropValue.wNum;
				}
				break;
			case EBAK_SPEED:
				{
					poOwner->GetCurDT_BATTLE_ATTRIBUTE().wSpeed += stEncouragePropValue.wNum;
				}
				break;
			case EBAK_ANGRY:
				{
					INT32 nTmp = (INT32)(poOwner->GetCurDT_BATTLE_ATTRIBUTE().wAngry) + (INT32)(stEncouragePropValue.wNum);
					if(nTmp > (INT32)poOwner->GetMaxDT_BATTLE_ATTRIBUTE().wAngry )
					{
						nTmp = poOwner->GetMaxDT_BATTLE_ATTRIBUTE().wAngry;
					}
					poOwner->GetCurDT_BATTLE_ATTRIBUTE().wAngry = nTmp;
				}
				break;
			default:
				break;
			}			
		}
		break;
	default:
		return FALSE;		
	}

	return TRUE;
	*/

	return TRUE;
}


BOOL CLogicFunc::FirmPropValue(SEncouragePropValue& stEncouragePropValue, SBurst* pstBurst, BOOL bFirstFlag)
{
	if(NULL == pstBurst)
	{
		return FALSE;
	}

	stEncouragePropValue.eBurstKind = pstBurst->eBurstKind;
	UINT16 wSize = pstBurst->vecProp.size();
	if(wSize == 0)
	{
		return FALSE;
	}

	SProp* pstProp = pstBurst->vecProp[0];
	if(pstBurst->bRandom)
	{
		//若首次进入副本，则使用FirstFalg为1的SProp
		if(bFirstFlag)
		{
			for(INT16 wIdx = 0; wIdx < wSize; wIdx++)
			{
				pstProp = pstBurst->vecProp[wIdx];
				if(1 == pstProp->bFirstFalg)
				{
					return FixValue(stEncouragePropValue, pstBurst->eBurstKind, pstProp);
				}
			}

		}		

		//按概率赋值
		return RandomValue(stEncouragePropValue, pstBurst->eBurstKind, pstBurst->vecProp);
	}
	else
	{
		return FixValue(stEncouragePropValue, pstBurst->eBurstKind, pstProp);
	}	

	return TRUE;
}

BOOL CLogicFunc::FixValue(SEncouragePropValue& stEncouragePropValue, EBurstKind eBurstKind, SProp* pstProp)
{
	switch(eBurstKind)
	{
	case EBKIND_COIN:
	case EBKIND_GOLD:
	case EBKIND_EXPERIENCE:
	case EBKIND_PHYSTRENGTH:
	case EBKIND_SCIENCE:
		{
			stEncouragePropValue.nValue = pstProp->nValue;
		}
		break;	
	case EBKIND_ITEM:
		{
			stEncouragePropValue.wSubKind = pstProp->wKindID;
			stEncouragePropValue.wNum = pstProp->wNum;
		}
		break;
	case EBKIND_ATTRIBUTE:
		{
			stEncouragePropValue.wSubKind = pstProp->wKindID;
			stEncouragePropValue.nValue = pstProp->nValue;
		}
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

BOOL CLogicFunc::RandomValue(SEncouragePropValue& stEncouragePropValue, EBurstKind eBurstKind, vector<SProp*>& vecProp)
{
	CRandomVec vecRandom;
	UINT16 wSize = vecProp.size();
	INT16 wIdx = 0;
	for(wIdx = 0; wIdx < wSize; wIdx++)
	{
		vecRandom.push_back(vecProp[wIdx]->wRate);		
	}

	//随机取一个SProp
	INT32 nRandomHitIdx = GetRandomIdx(vecRandom);
	if(RANDOM_UNHIT == nRandomHitIdx)
	{
		return FALSE;
	}
	SProp* pstProp = vecProp[nRandomHitIdx];

	switch(eBurstKind)
	{
	case EBKIND_COIN:
	case EBKIND_GOLD:
	case EBKIND_EXPERIENCE:
	case EBKIND_PHYSTRENGTH:
	case EBKIND_SCIENCE:
		{
			//值再在范围内随机
			UINT16 wRandom = pstProp->dwMax - pstProp->wMin;
			if(0 == wRandom)
			{
				stEncouragePropValue.wNum = pstProp->wMin;
			}
			UINT32 dwHit = RandomReseed(wRandom);
			stEncouragePropValue.nValue = dwHit + pstProp->wMin;
		}
		break;	
	case EBKIND_ITEM:
		{
			stEncouragePropValue.wSubKind = pstProp->wKindID;
			stEncouragePropValue.wNum = pstProp->wNum;
		}
		break;
	case EBKIND_ATTRIBUTE:
		{
			stEncouragePropValue.wSubKind = pstProp->wKindID;
			stEncouragePropValue.nValue = pstProp->nValue;
		}
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

BOOL CLogicFunc::Encourage(CPlayer* poOwner, SEncourageProp& stEncourageProp, BOOL bFirstFlag)
{
	//一般奖励
	CBurstVec& vecBurst = stEncourageProp.vecBurst;
	UINT16 wBurstSize = vecBurst.size();
	for(UINT16 wIdx = 0; wIdx < wBurstSize; wIdx++)
	{
		CLogicFunc::Encourage(poOwner, vecBurst[wIdx], bFirstFlag);
	}
	//道具组奖励,奖励道具的配置(分组，每组1个SBurst),随机取1个
	CBurstVec& vecBurstItemGroup = stEncourageProp.vecBurstItemGroup;
	wBurstSize = vecBurstItemGroup.size();
	if(wBurstSize)
	{
		UINT32 dwHit = RandomReseed(wBurstSize);
		CLogicFunc::Encourage(poOwner, vecBurstItemGroup[dwHit], bFirstFlag);
	}

	return TRUE;
}

/*
BOOL CLogicFunc::EncourageItem(CPlayer* poOwner, UINT16 wItemKindID, UINT16 wItemNum)
{	

	CItemVec vecItem;
	if(CItemMgr::Instance()->CreateItem(poOwner, wItemKindID, wItemNum, vecItem))
	{
		UINT16 wSize = vecItem.size();
		CBag* oBag = NULL;
		CBag* poBagTmp = poOwner->GetBag(EBAGK_TMP);
		for(UINT16 wIdx = 0; wIdx < wSize; wIdx++)
		{
			CItem* poItem = vecItem[wSize];
			if(NULL == poItem)
			{
				continue;
			}
			switch(poItem->GetItemKind())
			{
			case EIK_EQUIP:
				{
					CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
					if(NULL == poEquip)
					{
						continue;
					}

					oBag = poOwner->GetBag(EBAGK_EQUIP);
				}
				break;
			case EIK_GOODS:
				{
					CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
					if(NULL == poGoods)
					{
						return 2;
					}
					//材料放材料背包中
					if(EGMK_MATERIAL == poGoods->GetGoodsProp()->wMainKindID)
					{
						oBag = poOwner->GetBag(EBAGK_MATERIAL);
					}
					else
					{
						oBag = poOwner->GetBag(EBAGK_ARTICLE);
					}					
				}
				break;
			case EIK_BOX:
				{
					CBox* poBox = dynamic_cast<CBox*>(poItem);
					if(NULL == poBox)
					{
						return 2;
					}

					oBag = poOwner->GetBag(EBAGK_ARTICLE);
				}
				break;
			}
			if((NULL == oBag) && (NULL == poBagTmp))
			{
				continue;
			}
			//背包满，存放临时背包中
			if((oBag) && (!oBag.AddItem(poItem)))
			{
				poBagTmp->AddItem(poItem);
			}
		}


		return 0;
	}

	return FALSE;
}
*/

UINT32 CLogicFunc::GetInstanceIdxKey(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
	UINT32 dwSceneIdx = wSceneIdx << 16;
	UINT32 dwyTownIdx = byTownIdx << 8;
	UINT32 dwInstanceIdx = byInstanceIdx;

	UINT32 dwKey = (dwSceneIdx & 0xFFFF0000) | (dwyTownIdx & 0x0000FF00) | (dwInstanceIdx & 0x000000FF);

	return dwKey;
}

UINT64 CLogicFunc::GetInstanceIdxKey(UINT32 dwPlayerID,UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
	//注意，64位移位必须使用64位临时变量，否则溢出(32位可以不需要，其可以默认转换)
	UINT64 qwPlayerID = dwPlayerID;
	UINT64 qwSceneIdx = wSceneIdx;
	UINT64 qwTownIdx = byTownIdx;
	UINT64 qwInstanceIdx = byInstanceIdx;
	return (qwPlayerID<<32 & 0xFFFFFFFF00000000) | (qwSceneIdx<<16 & 0x00000000FFFF0000) | (qwTownIdx<<8 & 0x000000000000FF00) | (qwInstanceIdx & 0x00000000000000FF);

}


VOID CLogicFunc::GetInctanceID(UINT32 dwID, DT_INSTANCE_ID& stInstanceID)
{
	stInstanceID.wSceneIdx = (dwID & 0xFFFF0000) >> 16;
	stInstanceID.byTownIdx = (dwID & 0x0000FF00) >> 8;
	stInstanceID.byInstanceIdx = dwID & 0x000000FF;
}

UINT64 CLogicFunc::GetObjKey(UINT32 dwPlayerID, UINT16 wKindID)
{
	UINT64 qwKey = dwPlayerID;
	qwKey = qwKey <<32;
	qwKey += wKindID;

	return qwKey;
}

UINT64 CLogicFunc::GetObjKey(UINT32 dwPlayerID, UINT16 wID1, UINT16 wID2)
{
	UINT64 qwPlayerID = dwPlayerID;
	qwPlayerID = qwPlayerID << 32;
	UINT64 qwID1 = wID1;
	qwID1 = qwID1 << 16;
	UINT64 qwID2 = wID2;	
	UINT64 qwKey = (0xFFFFFFFF00000000 & qwPlayerID) | (0x00000000FFFF0000 & qwID1) | (0x000000000000FFFF & qwID2);

	return qwKey;
}

UINT64 CLogicFunc::GetItemKey(UINT32 dwPlayerID, UINT16 wKindID, UINT16 wIdx)
{
	UINT64 qwPlayerID = dwPlayerID;
	qwPlayerID = qwPlayerID << 32;
	UINT64 qwKindID = wKindID;
	qwKindID = qwKindID << 16;
	UINT64 qwIdx = wIdx;	
	UINT64 qwKey = (0xFFFFFFFF00000000 & qwPlayerID) | (0x00000000FFFF0000 & qwKindID) | (0x000000000000FFFF & qwIdx);

	return qwKey;
}



UINT16 CLogicFunc::Get88Key(UINT8 bywValue1,UINT8 byValue2)
{
	return ((0xFF00 & (bywValue1<<8)) | (0x00FF & byValue2));
}

UINT32 CLogicFunc::Get888Key(UINT8 byValue1,UINT8 byValue2, UINT8 byValue3)
{
	return ((0x00FF0000 & (byValue1<<16)) | (0x0000FF00 & (byValue2<<8)) | (0x000000FF & byValue3));
}

UINT32 CLogicFunc::Get8888Key(UINT8 byValue1,UINT8 byValue2, UINT8 bywValue3, UINT8 byValue4)
{
	return ((0xFF000000 & (byValue1<<24)) | (0x00FF0000 & (byValue2<<16)) | (0x0000FF00 & (bywValue3<<8)) | (0x000000FF & byValue4));
}


UINT32 CLogicFunc::Get1688Key(UINT16 wValue1,UINT8 bywValue2,UINT8 byValue3)
{
	return ((0xFFFF0000 & (wValue1<<16)) | (0x0000FF00 & (bywValue2<<8)) | (0x000000FF & byValue3));
}

UINT32 CLogicFunc::Get1616Key(UINT16 wValue1,UINT16 wValue2)
{
	return ((0xFFFF0000 & (wValue1<<16)) | (0x0000FFFF & wValue2));
}

UINT64 CLogicFunc::Get161616Key(UINT16 wValue1, UINT16 wValue2, UINT16 wValue3)
{
	UINT64 qwKey = 0;
	UINT64 qwTemp = 0;

	qwTemp = wValue1;
	qwTemp = qwTemp <<32;
	qwKey += qwTemp;

	qwTemp = wValue2;
	qwTemp = qwTemp <<16;
	qwKey += qwTemp;

	qwKey += wValue3;

	return qwKey;
}

UINT64 CLogicFunc::Get3232Key(UINT32 dwValue1,UINT32 dwValue2)
{
	UINT64 qwKey = dwValue1;
	qwKey = qwKey <<32;
	qwKey += dwValue2;

	return qwKey;
}



VOID CLogicFunc::AddAttr(SAllAttrProp& stAttr, UINT8 byAttrKind, UINT32 dwAttrValue)
{
	switch(byAttrKind)
	{
	case EBAK_HP:
		{
			stAttr.stBattleAttr.qwHP += dwAttrValue;
		}
		break;
	case EBAK_ATTACK:
		{
			stAttr.stBattleAttr.dwAttack += dwAttrValue;
		}
		break;
	case EBAK_HITRATE:
		{
			stAttr.stHideBattleAttr.wHitRate += dwAttrValue;
		}
		break;
	case EBAK_DODGERATE:
		{
			stAttr.stHideBattleAttr.wDodgeRate += dwAttrValue;
		}
		break;
	case EBAK_CRITRATE:
		{
			stAttr.stHideBattleAttr.wCritRate += dwAttrValue;
		}
		break;
	case EBAK_DECRITRATE:
		{
			stAttr.stHideBattleAttr.wDeCritRate += dwAttrValue;
		}
		break;
	case EBAK_INCHURTRATE:
		{
			stAttr.stExtBattleAttr.wIncHurtRate += dwAttrValue;
		}
		break;
	case EBAK_DECHURTRATE:
		{
			stAttr.stExtBattleAttr.wDecHurtRate += dwAttrValue;
		}
		break;
	case EBAK_ANGRY:
		{
			stAttr.stHideBattleAttr.wAngryValue += dwAttrValue;
		}
		break;
	case EBAK_ODDANGRY:
		{
			stAttr.stExtBattleAttr.wOddAngry += dwAttrValue;
		}
		break;
	case EBAK_FIRSTATTACK:
		{
			stAttr.stExtBattleAttr.dwFirstAttack += dwAttrValue;
		}
		break;
	case EBAK_COACHTALENT:
		{
			stAttr.stExtBattleAttr.wCoachTalent += dwAttrValue;
		}
		break;
	default:
		break;
	}
}

VOID CLogicFunc::ParseAttr(const SAllAttrProp& stAttr, const UINT8 byMaxDstNum, UINT8& byIncAttrNum, DT_ATTR_INC_DATA astIncAttrInfo[])
{
	byIncAttrNum = 0;
	if(0 != stAttr.stBattleAttr.qwHP)
	{
		//除了BOSS其它不可能超过32位
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_HP;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = static_cast<UINT32>(stAttr.stBattleAttr.qwHP);
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stBattleAttr.dwAttack)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_ATTACK;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stBattleAttr.dwAttack;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stHideBattleAttr.wHitRate)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_HITRATE;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stHideBattleAttr.wHitRate;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stHideBattleAttr.wDodgeRate)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_DODGERATE;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stHideBattleAttr.wDodgeRate;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stHideBattleAttr.wCritRate)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_CRITRATE;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stHideBattleAttr.wCritRate;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stHideBattleAttr.wDeCritRate)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_DECRITRATE;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stHideBattleAttr.wDeCritRate;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stHideBattleAttr.wAngryValue)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_ANGRY;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stHideBattleAttr.wAngryValue;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stExtBattleAttr.wIncHurtRate)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_INCHURTRATE;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stExtBattleAttr.wIncHurtRate;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stExtBattleAttr.wDecHurtRate)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_DECHURTRATE;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stExtBattleAttr.wDecHurtRate;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stExtBattleAttr.wOddAngry)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_ODDANGRY;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stExtBattleAttr.wOddAngry;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}	
	if(0 != stAttr.stExtBattleAttr.dwFirstAttack)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_FIRSTATTACK;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stExtBattleAttr.dwFirstAttack;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
	if(0 != stAttr.stExtBattleAttr.wCoachTalent)
	{
		astIncAttrInfo[byIncAttrNum].byAttrKind = EBAK_COACHTALENT;
		astIncAttrInfo[byIncAttrNum].dwAttrValue = stAttr.stExtBattleAttr.wCoachTalent;		
		if(byIncAttrNum++ >= byMaxDstNum)
		{
			return;
		}
	}
}


VOID CLogicFunc::AddAttr(const SAllAttrProp* pstSrc, DT_BATTLE_BASE_ATTRIBUTE& stDstBattleAttr, DT_HIDE_BATTLE_ATTRIBUTE& stDstHideBattleAttr, DT_EXT_BATTLE_ATTRIBUTE& stDstExtBattleAttr)
{
		const DT_BATTLE_BASE_ATTRIBUTE& stBattleAttr = pstSrc->stBattleAttr;
		const DT_HIDE_BATTLE_ATTRIBUTE& stHideBattleAttr = pstSrc->stHideBattleAttr;
		const DT_EXT_BATTLE_ATTRIBUTE& stExtBattleAttr = pstSrc->stExtBattleAttr;

		stDstBattleAttr.qwHP += stBattleAttr.qwHP;
		stDstBattleAttr.dwAttack += stBattleAttr.dwAttack;
		stDstBattleAttr.dwDefend += stBattleAttr.dwDefend;

		stDstHideBattleAttr.wHitRate += stHideBattleAttr.wHitRate;
		stDstHideBattleAttr.wDodgeRate += stHideBattleAttr.wDodgeRate;
		stDstHideBattleAttr.wCritRate += stHideBattleAttr.wCritRate;
		stDstHideBattleAttr.wDeCritRate += stHideBattleAttr.wDeCritRate;
		stDstHideBattleAttr.wAngryValue += stHideBattleAttr.wAngryValue;

		stDstExtBattleAttr.wDecHurtRate += stExtBattleAttr.wDecHurtRate;
		stDstExtBattleAttr.wIncHurtRate += stExtBattleAttr.wIncHurtRate;
		stDstExtBattleAttr.dwFirstAttack += stExtBattleAttr.dwFirstAttack;
		stDstExtBattleAttr.wCoachTalent += stExtBattleAttr.wCoachTalent;
}

UINT64 CLogicFunc::Get3216Key( UINT32 dwGroupID, UINT16 wLevel )
{
    UINT64 qwKey = dwGroupID;
    qwKey = qwKey <<32;
    qwKey += wLevel;

    return qwKey;
}

