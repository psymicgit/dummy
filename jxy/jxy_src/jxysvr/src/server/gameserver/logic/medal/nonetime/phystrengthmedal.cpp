///<------------------------------------------------------------------------------
//< @file:   medal\resource\phystrengthmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014年6月23日 15:37:52
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/phystrengthmedal.h"
#include "logic/medal/playermedal.h"
#include "logic/medal/medalpropmgr.h"
BOOL CPhystrengthMedal::CheckActivate()
{
	//已激活
	if ( 1 == m_stMedalData.byIsActivate)
	{
		return TRUE;
	}

	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		return FALSE;
	}

	if ( 0 == poMedalProp->wLevel)
	{
		return FALSE;
	}

	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL == poPlayerMedal)
	{
		return FALSE;
	}

	if ( poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwBuyPhystrength)
	{
		SetActivateState(TRUE);
		return TRUE;
	}
	return FALSE;
}

VOID CPhystrengthMedal::GetMedalCli(DT_MEDAL_DATA_CLI& stCli)
{
	const SMedalProp* poProp = NULL;
	if( !GetProp(poProp) )
	{
		return ;
	}
	
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL == poPlayerMedal)
	{
		return ;
	}

	SDStrncpy(stCli.aszMedalName, poProp->strMedalName.c_str(), MAX_NAME_LEN - 1);
	SDStrncpy(stCli.aszMedalIconName, poProp->strMedalIconName.c_str(), MAX_NAME_LEN -1);
	stCli.byIsActivate = m_stMedalData.byIsActivate;
	stCli.byMedalType = GetBigType();
	stCli.byMedalColor = poProp->byMedalColor;
	stCli.wSmallType = GetSmallType();
	stCli.wMedalLevel = poProp->wLevel;
	stCli.byIsLevelMedal = poProp->byIsLevelMedal;
	UINT8 byIdx = 0;
	for(CVecJsonAttrBaseItr itr = poProp->vecAttr.begin(); itr != poProp->vecAttr.end(); itr++, byIdx++)
	{
		stCli.stCurAttrInfo.astAttrInfo[byIdx] = *itr;
	}
	stCli.stCurAttrInfo.byAttrNum = byIdx;
	const SMedalProp* poNextProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(stCli.wSmallType, stCli.wMedalLevel + 1, poNextProp))
	{
		byIdx = 0;
		for(CVecJsonAttrBaseItr itr = poNextProp->vecAttr.begin(); itr != poNextProp->vecAttr.end(); itr++, byIdx++)
		{
			stCli.stNextAttrInfo.astAttrInfo[byIdx] = *itr;
		}

		//获得提示
		UINT32 dwLeft = 0;
		if ( poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwBuyPhystrength < poNextProp->qwFirstCondition )
		{
			dwLeft = static_cast<UINT32>(poNextProp->qwFirstCondition - poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwBuyPhystrength);
		}

		SDSnprintf(stCli.aszFinishDesc,  MAX_MEDAL_DESC_LEN - 1, poProp->strFinishDesc.c_str(), dwLeft);
	}
	else
	{
		SDStrncpy(stCli.aszFinishDesc, poProp->strFinishDesc.c_str(), MAX_MEDAL_DESC_LEN - 1);
	}
	stCli.stNextAttrInfo.byAttrNum = byIdx;
}
