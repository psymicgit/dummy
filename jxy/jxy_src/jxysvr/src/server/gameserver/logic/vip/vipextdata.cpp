#include "vipextdata.h"
#include "logic/vip/vippropmgr.h"
#include "common/client/commondef.h"
#include "logic/player/player.h"
// #include "logic/vip/vipgiftpropmgr.h"
// #include "logic/item/itemmgr.h"
// #include "logic/task/taskmgr.h"
// #include "logic/hero/hero.h"
// #include "logic/hero/heromgr.h"
// #include "logic/awaken/awaken.h"

IMPLEMENT_CLASS_POOL_MGR(CVipExtData);
CVipExtData::CVipExtData()
{
	memset( &m_stDT_VIP_EXT_DATA, 0, sizeof(m_stDT_VIP_EXT_DATA) );
}

CVipExtData::~CVipExtData()
{

}

BOOL CVipExtData::Init(DT_VIP_EXT_DATA &stDT_VIP_EXT_DATA, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}
	m_poOwner = poOwner;
	memcpy(&m_stDT_VIP_EXT_DATA, &stDT_VIP_EXT_DATA, sizeof(DT_VIP_EXT_DATA));
	return TRUE;
}

DT_VIP_EXT_DATA &CVipExtData::GetDT_VIP_EXT_DATA()
{
	return m_stDT_VIP_EXT_DATA;
}

VOID CVipExtData::Check4ResetExperience()
{
	if( !IsToday( m_stDT_VIP_EXT_DATA.qwLastEatTime ) )
	{
		m_stDT_VIP_EXT_DATA.wEatGoldExperienceCnt=0;
	}
}

VOID CVipExtData::Check4ResetEliteInstance()
{
	for( INT32 nIndex=0; nIndex<m_stDT_VIP_EXT_DATA.wEliteInstanceTownNum && nIndex<MAX_ELITE_INSTANCE_TOWN_NUM; ++nIndex )
	{
		if( !IsToday( m_stDT_VIP_EXT_DATA.astEliteInstanceData[nIndex].qwLastResetEliteInstanceTime ) )
		{
			m_stDT_VIP_EXT_DATA.astEliteInstanceData[nIndex].wResetEliteInstanceNum=0;
		}
	}
}

UINT16 CVipExtData::GetRemainGoldExperienceNum()
{
	return CVipPropMgr::Instance()->GetIncNum( EVINF_HEROEATJINGYANDAN, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel ) < GetDT_VIP_EXT_DATA().wEatGoldExperienceCnt ?
		0 : CVipPropMgr::Instance()->GetIncNum( EVINF_HEROEATJINGYANDAN, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel ) - GetDT_VIP_EXT_DATA().wEatGoldExperienceCnt;
}

UINT16 CVipExtData::GetRemainResetEliteInstanceNum(UINT8 byIndex)
{
	//@2013-08-26 精英副本的重置修改
	//UINT16 wResetNum = GetResetEliteInstanceNum(byIndex);

	UINT16 wResetNum = GetResetEliteInstanceNum();
	//@2013-08-26 end
	return CVipPropMgr::Instance()->GetIncNum( EVINF_RESETELITEINSTANCE, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel ) < wResetNum ?
		0 : CVipPropMgr::Instance()->GetIncNum( EVINF_RESETELITEINSTANCE, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel ) - wResetNum;
}

UINT16 CVipExtData::GetResetEliteInstanceNum( UINT8 byIndex )
{
	UINT16 wMaxNum = GetDT_VIP_EXT_DATA().wEliteInstanceTownNum;
	for( INT32 nIndex = 0; nIndex < wMaxNum && nIndex < MAX_ELITE_INSTANCE_TOWN_NUM; ++nIndex )
	{
		if( nIndex == byIndex - 1 )
		{
			//@2013-08-15修改重置精英副本次数一至
			return GetDT_VIP_EXT_DATA().astEliteInstanceData[nIndex].wResetEliteInstanceNum;
			//@2013-08-15 end
		}
	}

	return 0;
}

UINT16	CVipExtData::GetResetEliteInstanceNum()
{
	UINT16	wResetEliteInstanceNum	= 0;
	UINT16	wEliteInstanceTownNum	= GetDT_VIP_EXT_DATA().wEliteInstanceTownNum;
	for( INT32 nIndex = 0; nIndex < wEliteInstanceTownNum && nIndex < MAX_ELITE_INSTANCE_TOWN_NUM; ++nIndex )
	{
		wResetEliteInstanceNum += GetDT_VIP_EXT_DATA().astEliteInstanceData[nIndex].wResetEliteInstanceNum;
	}
	return wResetEliteInstanceNum;
}

BOOL CVipExtData::ResetEliteInstance( UINT8 byIndex )
{

	//查找并且更新所有可以重置的精英副本城镇
	UINT8 byTownIdx = 1;
	while(( m_poOwner->GetInstance().CkEliteUnlock(byTownIdx, 1)) && (byTownIdx <= MAX_TOWN_PER_SCENE_NUM))
	{
		byTownIdx++;
	}

	if( byTownIdx - 1 > GetDT_VIP_EXT_DATA().wEliteInstanceTownNum )
	{
		GetDT_VIP_EXT_DATA().wEliteInstanceTownNum = byTownIdx - 1;
	}

	UINT16 wMaxNum = GetDT_VIP_EXT_DATA().wEliteInstanceTownNum;

	BOOL bFind = FALSE;
	for( INT32 nIndex = 0; nIndex <= wMaxNum && nIndex <= MAX_ELITE_INSTANCE_TOWN_NUM; ++nIndex )
	{
		if( nIndex == byIndex - 1 )
		{
			if( GetRemainResetEliteInstanceNum( byIndex ) == 0 )
			{
				USR_INFO( _SDT( "[%s %d]reset player:%u remain eliteInstance num==0, Index:%d" ), MSG_MARK, m_poOwner->GetID(), byIndex );
				return FALSE;
			}

			//@2013-08-15修改重置精英副本次数一至
			GetDT_VIP_EXT_DATA().astEliteInstanceData[nIndex].wResetEliteInstanceNum += 1;
			GetDT_VIP_EXT_DATA().astEliteInstanceData[nIndex].qwLastResetEliteInstanceTime = SDTimeSecs();
			//@2013-08-15 end
			bFind = TRUE;
			break;
		}
	}

	if( !bFind )
	{
		SYS_CRITICAL( _SDT( "[%s %d]reset player:%u can't find elite instance, Index:%d" ), MSG_MARK, m_poOwner->GetID(), byIndex );
		return FALSE;
	}

	INT32 nInstanceIndex = 1;
	while((m_poOwner->GetInstance().CkEliteUnlock(byIndex, nInstanceIndex)) && (nInstanceIndex <= MAX_INSTANCE_PER_TOWN_NUM))
	{
		CInstanceRecord* poInstanceRecord = m_poOwner->GetInstance().GetSingleInstaceRecord(EIT_ELITE, byIndex, nInstanceIndex);
		//没有记录，跳过
		if(NULL == poInstanceRecord)
		{
			nInstanceIndex++;
			continue;
		}
		//记录存在清空挑战时间
		else
		{
			DT_INSTANCE_DATA& stDT_INSTANCE_DATA = poInstanceRecord->GetDT_INSTANCE_DATA();
			//设置挑战时间为0
			stDT_INSTANCE_DATA.qwLastPassTime = 0;
		}
		nInstanceIndex++;
	}


	return TRUE;
}

// VOID CVipExtData::InitGift()
// {
// 	CVipGiftPropMgr::Instance()->GetMaxGiftNum(m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel);
// 	m_stDT_VIP_EXT_DATA.qwLastGetTime = SDTimeSecs();
// 	m_stDT_VIP_EXT_DATA.byVipMaxLevel =  CVipPropMgr::Instance()->GetMaxVipLevel();
// }
// 
// BOOL CVipExtData::ChkGiftDataIsToday()
// {
// 	if (!IsToday(m_stDT_VIP_EXT_DATA.qwLastGetTime))
// 	{
// 		this->InitGift();
// 
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }
// 
// UINT8 CVipExtData::GetOpenData(UINT32 &dwMaxVipLevel,UINT32 &dwCurVipLevel,DT_VIP_GIFT_DATA astVipGetNum[MAX_VIP_LEVEL])
// {
// 	this->ChkGiftDataIsToday();
// 
// 	dwMaxVipLevel = m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel + 1;
// 	dwCurVipLevel = m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel;
// 
// 	if (dwMaxVipLevel > CVipPropMgr::Instance()->GetMaxVipLevel())
// 	{
// 		dwMaxVipLevel = CVipPropMgr::Instance()->GetMaxVipLevel();
// 	}
// 
// 	for (UINT8 i = 0; i < dwMaxVipLevel; i++)
// 	{   
// 		for (UINT8 j = 0; j < MAX_VIP_GIFT; j++)
// 		{
// 			astVipGetNum[i].byMaxVipGift = MAX_VIP_GIFT;
// 			astVipGetNum[i].astVipGiftData[j].wVipGiftGetNum = m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[i].astVipGetNum[j].wVipHaveNum - m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[i].astVipGetNum[j].wVipGetNum;
// 		}
// 
// 		CVipGiftPropMgr::Instance()->GetVipGiftItemData(i+1,&astVipGetNum[i]);
// 	}
// 
// 	return ERR_OPEN_VIP_GIFT::ID_SUCCESS;
// }
// 
// UINT8 CVipExtData::GetGift(TCHAR aszSeriesNo[MAX_SERIES_NO_NUM],SVipGiftData *pVipGiftItem,DT_RES_ITEM_DATA &stResItemData,DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData,const UINT8 &byIndex,UINT16 &wElseCount,const UINT8 &byVipLevel)
// {
// 
// 	if (pVipGiftItem == NULL)
// 	{
// 		RETURN_OTHER_ERR;
// 	}
// 
// 	DT_RES_NUM_ITEM_DATA stResNumItemData;
// 
// 	UINT16	wErrorCode = JsonGetResItemVec(pVipGiftItem->vecJsonEncourageItem,m_poOwner,stResItemData,stRsyncResItemData,stResNumItemData);
// 
// 	if (wErrorCode == BAG_FULL_ERR)
// 	{
// 		return ERR_GET_VIP_GIFT::ID_BAG_FULL;
// 	}
// 
// 	if (ERR_GET_VIP_GIFT::ID_SUCCESS != wErrorCode)
// 	{
// 		RETURN_OTHER_ERR;
// 	}
// 
// 	this->DecRequireSource(aszSeriesNo,pVipGiftItem,stRsyncResItemData);
// 
// 	m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[byVipLevel - 1].astVipGetNum[byIndex].wVipGetNum = m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[byVipLevel - 1].astVipGetNum[byIndex].wVipGetNum + 1;
// 	wElseCount = m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[byVipLevel - 1].astVipGetNum[byIndex].wVipHaveNum - m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[byVipLevel - 1].astVipGetNum[byIndex].wVipGetNum;
// 
// 	return ERR_GET_VIP_GIFT::ID_SUCCESS;
// }
// 
// BOOL CVipExtData::ChkRequireSource(UINT8 &byErrorCode,TCHAR aszSeriesNo[MAX_SERIES_NO_NUM],const SVipGiftData *pVipGiftItem)
// {
// 	if (NULL == pVipGiftItem)
// 	{
// 		SET_OTHER_ERR(byErrorCode);
// 		return FALSE;
// 	}
// 
// 	UINT32 dwRequireNum = pVipGiftItem->dwResourceNewNum;
// 
// 	if (dwRequireNum == 0)
// 	{
// 		return TRUE;
// 	}
// 
// 	if (dwRequireNum > m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold)
// 	{
// 		byErrorCode = ERR_COMMON::ID_GOLD_NOT_ENOUGH;
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }
// 
// UINT8 CVipExtData::GetGiftRes(const UINT8 &byVipLevel,TCHAR aszSeriesNo[MAX_SERIES_NO_NUM],DT_RES_ITEM_DATA &stResItemData,DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData,UINT8 &byIndex,UINT16 &wElseCount)
// {
// 	UINT8 byError = 0;
// 
// 	if (m_poOwner == NULL)
// 	{
// 		RETURN_OTHER_ERR;
// 	}
// 
// 	if (byVipLevel > CVipPropMgr::Instance()->GetMaxVipLevel())
// 	{
// 		RETURN_OTHER_ERR;
// 	}
// 
// 	if (byIndex >= MAX_VIP_GIFT)
// 	{
// 		RETURN_OTHER_ERR;
// 	}
// 
// 	this->ChkGiftDataIsToday();
// 
// 	if (m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel < byVipLevel)
// 	{
// 		return ERR_GET_VIP_GIFT::ID_VIP_LEVEL_NOT_AFFORD;
// 	}
// 
// 	SVipGiftData stVipGiftItem;
// 
// 	BOOL bRet = CVipGiftPropMgr::Instance()->GetGiftItem(byVipLevel,byIndex,stVipGiftItem);
// 
// 	if (FALSE == bRet)
// 	{
// 		RETURN_OTHER_ERR;
// 	}
// 
// 	if (byIndex >= MAX_VIP_GIFT)
// 	{
// 		RETURN_OTHER_ERR;
// 	}
// 
// 	if (m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[byVipLevel - 1].astVipGetNum[byIndex].wVipGetNum >= m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[byVipLevel - 1].astVipGetNum[byIndex].wVipHaveNum)
// 	{
// 		return ERR_GET_VIP_GIFT::ID_GET_NUM_IS_NULL;
// 	}
// 	
// 
// 	BOOL bAffordSource = this->ChkRequireSource(byError,aszSeriesNo,&stVipGiftItem);
// 
// 	if (!bAffordSource)
// 	{
// 		return byError;
// 	}
// 
// 	byError = this->GetGift(aszSeriesNo,&stVipGiftItem,stResItemData,stRsyncResItemData,byIndex,wElseCount,byVipLevel);
// 
// 	return byError;
// }
// 
// BOOL CVipExtData::HaveVipGift()
// {
// 	if(NULL == m_poOwner)
// 	{
// 		return FALSE;
// 	}
// 
// 	if (m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel == 0)
// 	{
// 		return FALSE;
// 	}
// 
// 	for (UINT8 i = 0;i < MAX_VIP_LEVEL && i < m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel;i++)
// 	{
// 		
// 		for (UINT8 j = 0; j < MAX_VIP_GIFT; j++)
// 		{
// 			if (m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[i].astVipGetNum[j].wVipGetNum < m_stDT_VIP_EXT_DATA.astVipGiftNumByLevel[i].astVipGetNum[j].wVipHaveNum)
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 
// 	return FALSE;
// }
// 
// VOID CVipExtData::DecRequireSource( TCHAR aszSeriesNo[MAX_SERIES_NO_NUM] ,const SVipGiftData *pVipGiftItem,DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData)
// {
// 	if (NULL == pVipGiftItem)
// 	{
// 		return;
// 	}
// 
// 	UINT32 dwRequireNum = pVipGiftItem->dwResourceNewNum;
// 
// 	if (dwRequireNum == 0)
// 	{
// 		return;
// 	}
// 
// 	if (dwRequireNum > m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold)
// 	{
// 		return;
// 	}
// 
// 	m_poOwner->DecGold(dwRequireNum, CRecordMgr::EDGT_VIP_GIFT, 0, 0, 0, 0, aszSeriesNo);
// 
// 	//同步资源
// 	DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
// 	stRSYNC_DT_RES_DATA.byResKind	= ACTE_GOLD;
// 	stRSYNC_DT_RES_DATA.qwResValue	= m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
// }