///<------------------------------------------------------------------------------
//< @file:   logic\medal\medal.cpp
//< @author: Kang_zhiquan
//< @date:   2014年6月18日 21:44:42
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/medal.h"
#include "logic/medal/medalpropmgr.h"
CMedal::CMedal()
{
	memset(&m_stMedalData, 0, sizeof(DT_MEDAL_DATA));
}
CMedal::~CMedal()
{
	memset(&m_stMedalData, 0, sizeof(DT_MEDAL_DATA));
}

BOOL CMedal::Init(CPlayer* poPlayer, const DT_MEDAL_DATA& stData)
{
	if ( NULL == poPlayer)
	{
		return FALSE;
	}
	m_poOwner = poPlayer;
	memcpy(&m_stMedalData, &stData, sizeof(DT_MEDAL_DATA));
	return TRUE;
}

UINT16 CMedal::GetSmallType()
{
	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		return poProp->wSmallType;
	}
	return 0;
}

BOOL CMedal::GetIsOverlay()
{
	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		return (1 == poProp->byIsOverly);
	}
	return FALSE;
}

//是否是等级勋章
BOOL CMedal::IsLevelMedal()
{
	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		return (1 == poProp->byIsLevelMedal);
	}
	return FALSE;
}

const DT_MEDAL_DATA& CMedal::GetMedalData()
{ 
	return m_stMedalData; 
}

BOOL CMedal::GetMedalAttr(const CVecJsonAttrBase*& pvecAttr)
{
	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		pvecAttr = &poProp->vecAttr;
		return TRUE;
	}
	return FALSE;
}

BOOL CMedal::GetProp(const SMedalProp*& poProp)
{
	return CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp);
}

//设置激活
BOOL CMedal::SetActivateState(BOOL bIsActivate, UINT64 qwActivateTime)
{
	UINT8 byOpType = ERM_ADD_MEDAL;
	if ( bIsActivate)
	{
		m_stMedalData.byIsActivate = ESM_YES;
		m_stMedalData.qwActivateTime = qwActivateTime;
		
	}
	else
	{
		byOpType = ERM_DEC_MEDAL;
		m_stMedalData.byIsActivate = ESM_NO;
        m_stMedalData.qwActivateTime = 0;
	}

	CRecordMgr::Instance()->RecordInfo(m_poOwner->GetID(), byOpType, 1, m_stMedalData.wMedalID,0, m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);	
	return TRUE;
}

VOID CMedal::GetMedalCli(DT_MEDAL_DATA_CLI& stCli)
{
	const SMedalProp* poProp = NULL;
	if( !GetProp(poProp) )
	{
		return ;
	}
	SDStrncpy(stCli.aszFinishDesc, poProp->strFinishDesc.c_str(), MAX_MEDAL_DESC_LEN - 1);
	SDStrncpy(stCli.aszMedalName, poProp->strMedalName.c_str(), MAX_NAME_LEN - 1);
	SDStrncpy(stCli.aszMedalIconName, poProp->strMedalIconName.c_str(), MAX_NAME_LEN - 1);
	stCli.byIsActivate = m_stMedalData.byIsActivate;
	stCli.byMedalType = GetBigType();
	stCli.byMedalColor = poProp->byMedalColor;
	stCli.wSmallType = GetSmallType();
	stCli.qwValidityTime = poProp->dwOverTime;
	UINT64 qwCurTime = SDTimeSecs();
	UINT64 qwRunTime = qwCurTime < m_stMedalData.qwActivateTime ? 0 : qwCurTime - m_stMedalData.qwActivateTime;
	stCli.qwOverTime = poProp->dwOverTime < qwRunTime ? 0 : poProp->dwOverTime - qwRunTime;
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
		
	}
	stCli.stNextAttrInfo.byAttrNum = byIdx;
}

UINT16 CMedal::GetMedalID()
{
	return m_stMedalData.wMedalID;
}

UINT16 CMedal::GetLevel()
{
	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		return poProp->wLevel;
	}
	return 0;
}

string CMedal::GetMedalIconID()
{
	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		return poProp->strMedalIconName;
	}
	return "";
}

UINT16 CMedal::GetBuildID()
{
	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		return poProp->byOpenBuildID;
	}
	return 0;
}

BOOL CMedal::IsActivate()
{
	return 1 == m_stMedalData.byIsActivate;
}

BOOL CTimeMedal::IsOverTime()
{
    if ( !IsActivate() )
    {
        return FALSE;
    }

	const SMedalProp* poProp = NULL;
	if( CMedalPropMgr::Instance()->GetMedalProp(m_stMedalData.wMedalID, poProp) )
	{
		//当前时间
        UINT64 qwCurTime = SGDP::SDTimeSecs();
        UINT64 qwOverTime = m_stMedalData.qwActivateTime + poProp->dwOverTime;
		if( qwCurTime <= qwOverTime )
		{
            USR_INFO("[%s:%d] LeftTime=%llu, MedalID = %u", MSG_MARK, qwOverTime - qwCurTime, m_stMedalData.wMedalID);
			return FALSE;
		}
	}
	SetActivateState(FALSE);
	// USR_INFO("[%s:%d] IsOver Time ", MSG_MARK);
	return TRUE;
}

UINT8 CTimeMedal::GetBigType()
{
	return EMBT_LIMIT_TIME;
}

UINT8 CNoneTimeMedal::GetBigType()
{
	return EMBT_NONE_TIME;
}
