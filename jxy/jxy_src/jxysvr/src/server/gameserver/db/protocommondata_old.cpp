#include "protocommondata_old.h"



INT32 DT_COLLECT_GAS_DATA_OLD::EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = 8;
	*pBuff = byIdxNum;
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	CHAR *pDataBuff = pBuff + wIndexAddrLen;
	UINT32 dwOffset = 0;
	UINT32 dwContextLenTmp = 0;
	UINT32 dwOffsetTmp = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);

	pstIndex->byIdx = 1;
	pstIndex->dwContextLen = sizeof(byCollectState);
	memcpy(pDataBuff + dwOffset, &byCollectState, sizeof(byCollectState));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 2;
	dwContextLenTmp = 0;
	*(UINT16*)(pDataBuff + dwOffset) = MAX_GAS_BALL_NUM;
	dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
	for(UINT16 i = 0; i < MAX_GAS_BALL_NUM; i++)
	{
		dwContextLenTmp += astGasBollInfo[i].EncodeSaveData(pDataBuff + dwOffsetTmp, dwBuffLen-wIndexAddrLen-dwOffsetTmp);
		dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE+ dwContextLenTmp;
	}
	pstIndex->dwContextLen = dwContextLenTmp + ARRAY_NUM_HEAD_SIZE;
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 3;
	pstIndex->dwContextLen = sizeof(wLastRandomGenNum);
	memcpy(pDataBuff + dwOffset, &wLastRandomGenNum, sizeof(wLastRandomGenNum));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 4;
	pstIndex->dwContextLen = sizeof(qwLastRandomGenTime);
	memcpy(pDataBuff + dwOffset, &qwLastRandomGenTime, sizeof(qwLastRandomGenTime));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 5;
	pstIndex->dwContextLen = sizeof(wLastPurpleGenNum);
	memcpy(pDataBuff + dwOffset, &wLastPurpleGenNum, sizeof(wLastPurpleGenNum));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 6;
	pstIndex->dwContextLen = sizeof(qwLastGenPurpleTime);
	memcpy(pDataBuff + dwOffset, &qwLastGenPurpleTime, sizeof(qwLastGenPurpleTime));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 7;
	pstIndex->dwContextLen = sizeof(wLastRandomUpNum);
	memcpy(pDataBuff + dwOffset, &wLastRandomUpNum, sizeof(wLastRandomUpNum));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 8;
	pstIndex->dwContextLen = sizeof(qwLastRandomUpTime);
	memcpy(pDataBuff + dwOffset, &qwLastRandomUpTime, sizeof(qwLastRandomUpTime));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;
	return wIndexAddrLen + dwOffset;
}

INT32 DT_COLLECT_GAS_DATA_OLD::DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = (UINT8)(*pBuff);
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	if(wIndexAddrLen >= dwBuffLen)
	{
		return -1;
	}
	CHAR *pDataBuff = (CHAR*)pBuff + wIndexAddrLen;

	UINT32 dwOffset = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);
	while(byIdxNum-- > 0)
	{
		switch(pstIndex->byIdx)
		{
		case 1:
			{
				if(sizeof(byCollectState) >= pstIndex->dwContextLen)
				{
					memcpy(&byCollectState, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&byCollectState, 0, sizeof(byCollectState));
				}
			}
			break;
		case 2:
			{
				UINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);
				UINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
				UINT32 dwPassContextLen = 0;
				wArrNum = wArrNum <= MAX_GAS_BALL_NUM ? wArrNum : MAX_GAS_BALL_NUM;
				for(UINT16 i = 0; i < wArrNum; i++)
				{
					dwPassContextLen += astGasBollInfo[i].DecodeSaveData(pDataBuff + dwOffsetTmp, pstIndex->dwContextLen - dwPassContextLen);
					if(dwPassContextLen >= pstIndex->dwContextLen)
					{
						memset(astGasBollInfo, 0, sizeof(astGasBollInfo));
						break;
					}
					dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE + dwPassContextLen;
				}
			}
			break;
		case 3:
			{
				if(sizeof(wLastRandomGenNum) >= pstIndex->dwContextLen)
				{
					memcpy(&wLastRandomGenNum, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wLastRandomGenNum, 0, sizeof(wLastRandomGenNum));
				}
			}
			break;
		case 4:
			{
				if(sizeof(qwLastRandomGenTime) >= pstIndex->dwContextLen)
				{
					memcpy(&qwLastRandomGenTime, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwLastRandomGenTime, 0, sizeof(qwLastRandomGenTime));
				}
			}
			break;
		case 5:
			{
				if(sizeof(wLastPurpleGenNum) >= pstIndex->dwContextLen)
				{
					memcpy(&wLastPurpleGenNum, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wLastPurpleGenNum, 0, sizeof(wLastPurpleGenNum));
				}
			}
			break;
		case 6:
			{
				if(sizeof(qwLastGenPurpleTime) >= pstIndex->dwContextLen)
				{
					memcpy(&qwLastGenPurpleTime, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwLastGenPurpleTime, 0, sizeof(qwLastGenPurpleTime));
				}
			}
			break;
		case 7:
			{
				if(sizeof(wLastRandomUpNum) >= pstIndex->dwContextLen)
				{
					memcpy(&wLastRandomUpNum, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wLastRandomUpNum, 0, sizeof(wLastRandomUpNum));
				}
			}
			break;
		case 8:
			{
				if(sizeof(qwLastRandomUpTime) >= pstIndex->dwContextLen)
				{
					memcpy(&qwLastRandomUpTime, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwLastRandomUpTime, 0, sizeof(qwLastRandomUpTime));
				}
			}
			break;
		default:
			break;
		}

		dwOffset += pstIndex->dwContextLen;
		pstIndex++;
	}

	return wIndexAddrLen + dwOffset;
}

INT32 DT_PLAYER_BASE_DATA_OLD::EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = 17;
	*pBuff = byIdxNum;
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	CHAR *pDataBuff = pBuff + wIndexAddrLen;
	UINT32 dwOffset = 0;
	UINT32 dwContextLenTmp = 0;
	UINT32 dwOffsetTmp = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);

	pstIndex->byIdx = 1;
	pstIndex->dwContextLen = sizeof(dwID);
	memcpy(pDataBuff + dwOffset, &dwID, sizeof(dwID));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 2;
	pstIndex->dwContextLen = sizeof(aszDispName) + ARRAY_NUM_HEAD_SIZE;
	*(UINT16*)(pDataBuff + dwOffset) = USERNAME_LEN;
	memcpy(pDataBuff + dwOffset + ARRAY_NUM_HEAD_SIZE, &aszDispName, sizeof(aszDispName));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 3;
	pstIndex->dwContextLen = sizeof(qwCoin);
	memcpy(pDataBuff + dwOffset, &qwCoin, sizeof(qwCoin));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 4;
	pstIndex->dwContextLen = sizeof(qwGold);
	memcpy(pDataBuff + dwOffset, &qwGold, sizeof(qwGold));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 5;
	pstIndex->dwContextLen = sizeof(qwStory);
	memcpy(pDataBuff + dwOffset, &qwStory, sizeof(qwStory));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 6;
	pstIndex->dwContextLen = sizeof(wPhyStrength);
	memcpy(pDataBuff + dwOffset, &wPhyStrength, sizeof(wPhyStrength));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 7;
	pstIndex->dwContextLen = sizeof(qwScience);
	memcpy(pDataBuff + dwOffset, &qwScience, sizeof(qwScience));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 8;
	pstIndex->dwContextLen = sizeof(abyGuideRecordData) + ARRAY_NUM_HEAD_SIZE;
	*(UINT16*)(pDataBuff + dwOffset) = MAX_GUIDE_NUM;
	memcpy(pDataBuff + dwOffset + ARRAY_NUM_HEAD_SIZE, &abyGuideRecordData, sizeof(abyGuideRecordData));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 9;
	pstIndex->dwContextLen = sizeof(byRobotFunc);
	memcpy(pDataBuff + dwOffset, &byRobotFunc, sizeof(byRobotFunc));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 10;
	pstIndex->dwContextLen = sizeof(dwPower);
	memcpy(pDataBuff + dwOffset, &dwPower, sizeof(dwPower));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 11;
	pstIndex->dwContextLen = sizeof(dwFirstAttack);
	memcpy(pDataBuff + dwOffset, &dwFirstAttack, sizeof(dwFirstAttack));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 12;
	pstIndex->dwContextLen = sizeof(qwTodayFirstLoginTime);
	memcpy(pDataBuff + dwOffset, &qwTodayFirstLoginTime, sizeof(qwTodayFirstLoginTime));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 13;
	pstIndex->dwContextLen = sizeof(byBagOpenNum);
	memcpy(pDataBuff + dwOffset, &byBagOpenNum, sizeof(byBagOpenNum));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 14;
	pstIndex->dwContextLen = sizeof(qwBlueGas);
	memcpy(pDataBuff + dwOffset, &qwBlueGas, sizeof(qwBlueGas));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 15;
	pstIndex->dwContextLen = sizeof(qwPurpleGas);
	memcpy(pDataBuff + dwOffset, &qwPurpleGas, sizeof(qwPurpleGas));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 16;
	pstIndex->dwContextLen = sizeof(byVipLevel);
	memcpy(pDataBuff + dwOffset, &byVipLevel, sizeof(byVipLevel));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 17;
	pstIndex->dwContextLen = sizeof(dwTotalAmount);
	memcpy(pDataBuff + dwOffset, &dwTotalAmount, sizeof(dwTotalAmount));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;
	return wIndexAddrLen + dwOffset;
}

INT32 DT_PLAYER_BASE_DATA_OLD::DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = (UINT8)(*pBuff);
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	if(wIndexAddrLen >= dwBuffLen)
	{
		return -1;
	}
	CHAR *pDataBuff = (CHAR*)pBuff + wIndexAddrLen;

	UINT32 dwOffset = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);
	while(byIdxNum-- > 0)
	{
		switch(pstIndex->byIdx)
		{
		case 1:
			{
				if(sizeof(dwID) >= pstIndex->dwContextLen)
				{
					memcpy(&dwID, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&dwID, 0, sizeof(dwID));
				}
			}
			break;
		case 2:
			{
				UINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);
				UINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
				UINT32 dwContextLen = pstIndex->dwContextLen - ARRAY_NUM_HEAD_SIZE;
				if((sizeof(aszDispName) >= dwContextLen) && (wArrNum <= USERNAME_LEN))
				{
					memcpy(&aszDispName, (pDataBuff + dwOffsetTmp), dwContextLen);
				}
				else
				{
					memset(&aszDispName, 0, sizeof(aszDispName));
				}
			}
			break;
		case 3:
			{
				if(sizeof(qwCoin) >= pstIndex->dwContextLen)
				{
					memcpy(&qwCoin, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwCoin, 0, sizeof(qwCoin));
				}
			}
			break;
		case 4:
			{
				if(sizeof(qwGold) >= pstIndex->dwContextLen)
				{
					memcpy(&qwGold, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwGold, 0, sizeof(qwGold));
				}
			}
			break;
		case 5:
			{
				if(sizeof(qwStory) >= pstIndex->dwContextLen)
				{
					memcpy(&qwStory, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwStory, 0, sizeof(qwStory));
				}
			}
			break;
		case 6:
			{
				if(sizeof(wPhyStrength) >= pstIndex->dwContextLen)
				{
					memcpy(&wPhyStrength, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wPhyStrength, 0, sizeof(wPhyStrength));
				}
			}
			break;
		case 7:
			{
				if(sizeof(qwScience) >= pstIndex->dwContextLen)
				{
					memcpy(&qwScience, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwScience, 0, sizeof(qwScience));
				}
			}
			break;
		case 8:
			{
				UINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);
				UINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
				UINT32 dwContextLen = pstIndex->dwContextLen - ARRAY_NUM_HEAD_SIZE;
				if((sizeof(abyGuideRecordData) >= dwContextLen) && (wArrNum <= MAX_GUIDE_NUM))
				{
					memcpy(&abyGuideRecordData, (pDataBuff + dwOffsetTmp), dwContextLen);
				}
				else
				{
					memset(&abyGuideRecordData, 0, sizeof(abyGuideRecordData));
				}
			}
			break;
		case 9:
			{
				if(sizeof(byRobotFunc) >= pstIndex->dwContextLen)
				{
					memcpy(&byRobotFunc, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&byRobotFunc, 0, sizeof(byRobotFunc));
				}
			}
			break;
		case 10:
			{
				if(sizeof(dwPower) >= pstIndex->dwContextLen)
				{
					memcpy(&dwPower, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&dwPower, 0, sizeof(dwPower));
				}
			}
			break;
		case 11:
			{
				if(sizeof(dwFirstAttack) >= pstIndex->dwContextLen)
				{
					memcpy(&dwFirstAttack, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&dwFirstAttack, 0, sizeof(dwFirstAttack));
				}
			}
			break;
		case 12:
			{
				if(sizeof(qwTodayFirstLoginTime) >= pstIndex->dwContextLen)
				{
					memcpy(&qwTodayFirstLoginTime, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwTodayFirstLoginTime, 0, sizeof(qwTodayFirstLoginTime));
				}
			}
			break;
		case 13:
			{
				if(sizeof(byBagOpenNum) >= pstIndex->dwContextLen)
				{
					memcpy(&byBagOpenNum, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&byBagOpenNum, 0, sizeof(byBagOpenNum));
				}
			}
			break;
		case 14:
			{
				if(sizeof(qwBlueGas) >= pstIndex->dwContextLen)
				{
					memcpy(&qwBlueGas, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwBlueGas, 0, sizeof(qwBlueGas));
				}
			}
			break;
		case 15:
			{
				if(sizeof(qwPurpleGas) >= pstIndex->dwContextLen)
				{
					memcpy(&qwPurpleGas, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&qwPurpleGas, 0, sizeof(qwPurpleGas));
				}
			}
			break;
		case 16:
			{
				if(sizeof(byVipLevel) >= pstIndex->dwContextLen)
				{
					memcpy(&byVipLevel, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&byVipLevel, 0, sizeof(byVipLevel));
				}
			}
			break;
		case 17:
			{
				if(sizeof(dwTotalAmount) >= pstIndex->dwContextLen)
				{
					memcpy(&dwTotalAmount, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&dwTotalAmount, 0, sizeof(dwTotalAmount));
				}
			}
			break;
		default:
			break;
		}

		dwOffset += pstIndex->dwContextLen;
		pstIndex++;
	}

	return wIndexAddrLen + dwOffset;
}


INT32 DT_HERO_DATA_OLD::EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = 6;
	*pBuff = byIdxNum;
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	CHAR *pDataBuff = pBuff + wIndexAddrLen;
	UINT32 dwOffset = 0;
	UINT32 dwContextLenTmp = 0;
	UINT32 dwOffsetTmp = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);

	pstIndex->byIdx = 1;
	pstIndex->dwContextLen = stHeroBaseData.EncodeSaveData(pDataBuff + dwOffset, dwBuffLen-wIndexAddrLen-dwOffset);
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 2;
	pstIndex->dwContextLen = stEquipDataInfo.EncodeSaveData(pDataBuff + dwOffset, dwBuffLen-wIndexAddrLen-dwOffset);
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 3;
	pstIndex->dwContextLen = stDrugDataInfo.EncodeSaveData(pDataBuff + dwOffset, dwBuffLen-wIndexAddrLen-dwOffset);
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 4;
	pstIndex->dwContextLen = stGodweaponInfo.EncodeSaveData(pDataBuff + dwOffset, dwBuffLen-wIndexAddrLen-dwOffset);
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 5;
	pstIndex->dwContextLen = stSkillInfo.EncodeSaveData(pDataBuff + dwOffset, dwBuffLen-wIndexAddrLen-dwOffset);
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 6;
	dwContextLenTmp = 0;
	*(UINT16*)(pDataBuff + dwOffset) = MAX_HERO_STUDY_GRID_NUM;
	dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
	for(UINT16 i = 0; i < MAX_HERO_STUDY_GRID_NUM; i++)
	{
		dwContextLenTmp += astStudyInfo[i].EncodeSaveData(pDataBuff + dwOffsetTmp, dwBuffLen-wIndexAddrLen-dwOffsetTmp);
		dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE+ dwContextLenTmp;
	}
	pstIndex->dwContextLen = dwContextLenTmp + ARRAY_NUM_HEAD_SIZE;
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;
	return wIndexAddrLen + dwOffset;
}

INT32 DT_HERO_DATA_OLD::DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = (UINT8)(*pBuff);
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	if(wIndexAddrLen >= dwBuffLen)
	{
		return -1;
	}
	CHAR *pDataBuff = (CHAR*)pBuff + wIndexAddrLen;

	UINT32 dwOffset = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);
	while(byIdxNum-- > 0)
	{
		switch(pstIndex->byIdx)
		{
		case 1:
			{
				stHeroBaseData.DecodeSaveData(pDataBuff + dwOffset, pstIndex->dwContextLen);
			}
			break;
		case 2:
			{
				stEquipDataInfo.DecodeSaveData(pDataBuff + dwOffset, pstIndex->dwContextLen);
			}
			break;
		case 3:
			{
				stDrugDataInfo.DecodeSaveData(pDataBuff + dwOffset, pstIndex->dwContextLen);
			}
			break;
		case 4:
			{
				stGodweaponInfo.DecodeSaveData(pDataBuff + dwOffset, pstIndex->dwContextLen);
			}
			break;
		case 5:
			{
				stSkillInfo.DecodeSaveData(pDataBuff + dwOffset, pstIndex->dwContextLen);
			}
			break;
		case 6:
			{
				UINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);
				UINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
				UINT32 dwPassContextLen = 0;
				wArrNum = wArrNum <= MAX_HERO_STUDY_GRID_NUM ? wArrNum : MAX_HERO_STUDY_GRID_NUM;
				for(UINT16 i = 0; i < wArrNum; i++)
				{
					dwPassContextLen += astStudyInfo[i].DecodeSaveData(pDataBuff + dwOffsetTmp, pstIndex->dwContextLen - dwPassContextLen);
					if(dwPassContextLen >= pstIndex->dwContextLen)
					{
						memset(astStudyInfo, 0, sizeof(astStudyInfo));
						break;
					}
					dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE + dwPassContextLen;
				}
			}
			break;
		default:
			break;
		}

		dwOffset += pstIndex->dwContextLen;
		pstIndex++;
	}

	return wIndexAddrLen + dwOffset;
}

INT32 DT_SKILL_DATA_OLD::EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = 5;
	*pBuff = byIdxNum;
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	CHAR *pDataBuff = pBuff + wIndexAddrLen;
	UINT32 dwOffset = 0;
	UINT32 dwContextLenTmp = 0;
	UINT32 dwOffsetTmp = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);

	pstIndex->byIdx = 1;
	pstIndex->dwContextLen = sizeof(wID);
	memcpy(pDataBuff + dwOffset, &wID, sizeof(wID));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 2;
	pstIndex->dwContextLen = sizeof(wLevel);
	memcpy(pDataBuff + dwOffset, &wLevel, sizeof(wLevel));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 3;
	pstIndex->dwContextLen = sizeof(wActionID);
	memcpy(pDataBuff + dwOffset, &wActionID, sizeof(wActionID));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 4;
	pstIndex->dwContextLen = sizeof(wActionLevel);
	memcpy(pDataBuff + dwOffset, &wActionLevel, sizeof(wActionLevel));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 5;
	pstIndex->dwContextLen = sizeof(wEffectID);
	memcpy(pDataBuff + dwOffset, &wEffectID, sizeof(wEffectID));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;
	return wIndexAddrLen + dwOffset;
}

INT32 DT_SKILL_DATA_OLD::DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = (UINT8)(*pBuff);
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	if(wIndexAddrLen >= dwBuffLen)
	{
		return -1;
	}
	CHAR *pDataBuff = (CHAR*)pBuff + wIndexAddrLen;

	UINT32 dwOffset = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);
	while(byIdxNum-- > 0)
	{
		switch(pstIndex->byIdx)
		{
		case 1:
			{
				if(sizeof(wID) >= pstIndex->dwContextLen)
				{
					memcpy(&wID, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wID, 0, sizeof(wID));
				}
			}
			break;
		case 2:
			{
				if(sizeof(wLevel) >= pstIndex->dwContextLen)
				{
					memcpy(&wLevel, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wLevel, 0, sizeof(wLevel));
				}
			}
			break;
		case 3:
			{
				if(sizeof(wActionID) >= pstIndex->dwContextLen)
				{
					memcpy(&wActionID, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wActionID, 0, sizeof(wActionID));
				}
			}
			break;
		case 4:
			{
				if(sizeof(wActionLevel) >= pstIndex->dwContextLen)
				{
					memcpy(&wActionLevel, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wActionLevel, 0, sizeof(wActionLevel));
				}
			}
			break;
		case 5:
			{
				if(sizeof(wEffectID) >= pstIndex->dwContextLen)
				{
					memcpy(&wEffectID, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wEffectID, 0, sizeof(wEffectID));
				}
			}
			break;
		default:
			break;
		}

		dwOffset += pstIndex->dwContextLen;
		pstIndex++;
	}

	return wIndexAddrLen + dwOffset;
}


INT32 DT_HERO_DATA_LIST_OLD::EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = 2;
	*pBuff = byIdxNum;
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	CHAR *pDataBuff = pBuff + wIndexAddrLen;
	UINT32 dwOffset = 0;
	UINT32 dwContextLenTmp = 0;
	UINT32 dwOffsetTmp = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);

	pstIndex->byIdx = 1;
	pstIndex->dwContextLen = sizeof(wHeroNum);
	memcpy(pDataBuff + dwOffset, &wHeroNum, sizeof(wHeroNum));
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;

	pstIndex->byIdx = 2;
	dwContextLenTmp = 0;
	if(wHeroNum > MAX_HERO_NUM)
	{
		wHeroNum = MAX_HERO_NUM;
	}
	*(UINT16*)(pDataBuff + dwOffset) = wHeroNum;
	dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
	for(UINT16 i = 0; i < wHeroNum; i++)
	{
		dwContextLenTmp += astHeroInfoLst[i].EncodeSaveData(pDataBuff + dwOffsetTmp, dwBuffLen-wIndexAddrLen-dwOffsetTmp);
		dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE+ dwContextLenTmp;
	}
	pstIndex->dwContextLen = dwContextLenTmp + ARRAY_NUM_HEAD_SIZE;
	dwOffset += pstIndex->dwContextLen;
	pstIndex++;
	return wIndexAddrLen + dwOffset;
}

INT32 DT_HERO_DATA_LIST_OLD::DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen)
{
	if(NULL == pBuff)
	{
		return -1;
	}
	UINT8 byIdxNum = (UINT8)(*pBuff);
	UINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;
	if(wIndexAddrLen >= dwBuffLen)
	{
		return -1;
	}
	CHAR *pDataBuff = (CHAR*)pBuff + wIndexAddrLen;

	UINT32 dwOffset = 0;
	SIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);
	while(byIdxNum-- > 0)
	{
		switch(pstIndex->byIdx)
		{
		case 1:
			{
				if(sizeof(wHeroNum) >= pstIndex->dwContextLen)
				{
					memcpy(&wHeroNum, (pDataBuff + dwOffset), pstIndex->dwContextLen);
				}
				else
				{
					memset(&wHeroNum, 0, sizeof(wHeroNum));
				}
			}
			break;
		case 2:
			{
				UINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);
				UINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;
				UINT32 dwPassContextLen = 0;
				if(wArrNum == wHeroNum)
				{
					wArrNum = wArrNum <= MAX_HERO_NUM ? wArrNum : MAX_HERO_NUM;
					wHeroNum = (UINT16)wArrNum;
					for(UINT16 i = 0; i < wArrNum; i++)
					{
						dwPassContextLen += astHeroInfoLst[i].DecodeSaveData(pDataBuff + dwOffsetTmp, pstIndex->dwContextLen - dwPassContextLen);
						if(dwPassContextLen >= pstIndex->dwContextLen)
						{
							memset(astHeroInfoLst, 0, sizeof(astHeroInfoLst));
							break;
						}
						dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE + dwPassContextLen;
					}
				}
				else
				{
					memset(&astHeroInfoLst, 0, sizeof(astHeroInfoLst));
				}
			}
			break;
		default:
			break;
		}

		dwOffset += pstIndex->dwContextLen;
		pstIndex++;
	}

	return wIndexAddrLen + dwOffset;
}