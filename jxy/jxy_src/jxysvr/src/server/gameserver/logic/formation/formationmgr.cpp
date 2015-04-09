
#include "formationmgr.h"
#include "formationpropmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/base/basedefine.h>
#include <common/client/errdef.h>
#include "common/client/commondef.h"
#include "logic/player/player.h"


IMPLEMENT_SINGLETON(CFormationMgr)


CFormationMgr::CFormationMgr()
{

}

CFormationMgr::~CFormationMgr()
{
	CFormationPropMgr::DestroyInstance();
}

BOOL CFormationMgr::Init()
{
	// 初始化CFormationPropMgr管理器
	if(FALSE == CFormationPropMgr::CreateInstance())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CFormationPropMgr::CreateInstance failed!"), MSG_MARK);        
		return FALSE;
	}
	if(FALSE == CFormationPropMgr::Instance()->Init())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CFormationPropMgr Init failed!"), MSG_MARK);        
		return FALSE;
	}

	return TRUE;
}


VOID CFormationMgr::UnInit()
{
	CFormationPropMgr::Instance()->UnInit();
}

/*bChoice=FALSE为旧转换为旧版本， TRUE为新版本*/
VOID CFormationMgr::ConvertFormation(DT_FORMATION_DATA& stOld, DT_NEW_FORMATION_DATA& stNew, CPlayer* poPlayer)
{
	if ( CheckConvertFormation(poPlayer) )	//新版本阵型
	{
		stOld.bySelectIdx = stNew.byNewSelectIdx;
		stOld.stFormationList.byFormationKindNum = stNew.stNewFormationLst.byFormationNum;
		for(UINT8 byIndex = 0; byIndex < stNew.stNewFormationLst.byFormationNum; byIndex++)
		{
			DT_NEW_FORMATION_INFO& stNewInfo = stNew.stNewFormationLst.astFormationArray[byIndex];
			DT_FORMATION_IDX_DATA& stOldInfo = stOld.stFormationList.astFormationInfo[byIndex];
			stOldInfo.byFormationKind = stNewInfo.byFormationID;
			for ( UINT8 byIdx = 0; byIdx < stNewInfo.byPostionNum; byIdx++)
			{
				if( ESM_YES == stNewInfo.astPostionInfo[byIdx].byIsUnlock) //位置解锁
				{
					stOldInfo.astOpenIdxInfo[byIdx].byFormationIdx = stNewInfo.astPostionInfo[byIdx].byPostionIndex;
					stOldInfo.astOpenIdxInfo[byIdx].wHeroID = stNewInfo.astPostionInfo[byIdx].wHeroID;
				}
				else
				{
					stOldInfo.astOpenIdxInfo[byIdx].byFormationIdx = stNewInfo.astPostionInfo[byIdx].byPostionIndex;
					stOldInfo.astOpenIdxInfo[byIdx].wHeroID = 0;
					stNewInfo.astPostionInfo[byIdx].wHeroID = 0;
				}
				
			}
		}
	}
	else	//旧版本阵型
	{
		stOld.bySelectIdx = stNew.bySelectIdx;
		stOld.stFormationList = stNew.stFormationList;
	}
}

VOID CFormationMgr::ConvertOldToNew(DT_NEW_FORMATION_DATA& stNew, DT_FORMATION_DATA& stOld, DT_NEW_FORMATION_DATA& stSource)
{
	memcpy(&stNew, &stSource, sizeof(DT_NEW_FORMATION_DATA));

	stNew.bySelectIdx = stOld.bySelectIdx;
	stNew.stFormationList = stOld.stFormationList;
	
}

VOID CFormationMgr::ConvertIdxDataToInfo(DT_NEW_FORMATION_INFO& stTarget, DT_FORMATION_IDX_DATA& stChange)
{
	//stTarget.byFormationID = stChange.byFormationKind;
	//stTarget.byPostionNum = stChange.byOpenIdxNum;

	for ( UINT8 byIdx = 0; byIdx < stChange.byOpenIdxNum; byIdx++)
	{
		SetIdxHeroID(stTarget, stChange.astOpenIdxInfo[byIdx].wHeroID, stChange.astOpenIdxInfo[byIdx].byFormationIdx);
	}
}

VOID CFormationMgr::ConvertFormationIdx(DT_FORMATION_IDX_DATA& stOldInfo, DT_NEW_FORMATION_INFO& stNewInfo)
{
	stOldInfo.byFormationKind = stNewInfo.byFormationID;
	UINT8 byIdx = 0;
	for ( ; byIdx < stNewInfo.byPostionNum; byIdx++)
	{
		if( ESM_YES == stNewInfo.astPostionInfo[byIdx].byIsUnlock) //位置解锁
		{
			stOldInfo.astOpenIdxInfo[byIdx].byFormationIdx = stNewInfo.astPostionInfo[byIdx].byPostionIndex;
			stOldInfo.astOpenIdxInfo[byIdx].wHeroID = stNewInfo.astPostionInfo[byIdx].wHeroID;
		}
		else
		{
			stOldInfo.astOpenIdxInfo[byIdx].byFormationIdx = stNewInfo.astPostionInfo[byIdx].byPostionIndex;
			stOldInfo.astOpenIdxInfo[byIdx].wHeroID = 0;
		}
	}
	stOldInfo.byOpenIdxNum = byIdx;
}

BOOL CFormationMgr::CheckConvertFormation(CPlayer* poPlayer)
{
	if ( poPlayer->GetCliVer() >= 305)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFormationMgr::SetIdxHeroID(DT_NEW_FORMATION_INFO& stNewInfo, UINT16 wHeroID, UINT8 byOldIdx)
{
	UINT8 byTargetPos = 0;
	switch ( byOldIdx)
	{
	case 1:
		byTargetPos = 1;
		break;
	case 2:
		byTargetPos = 1;
		break;
	case 3:
		byTargetPos = 3;
		break;
	case 4:
		byTargetPos = 4;
		break;
	case 5:
		byTargetPos = 4;
		break;
	case 6:
		byTargetPos = 6;
		break;
	default:
		byTargetPos = 0;
		break;
	}
	if ( 0 == byTargetPos)
	{
		return FALSE;
	}
	for ( UINT8 byIdx = 0; byIdx < stNewInfo.byPostionNum; byIdx++)
	{
		if( byTargetPos == stNewInfo.astPostionInfo[byIdx].byPostionIndex)
		{
			if( stNewInfo.astPostionInfo[byIdx].byIsUnlock == ESM_YES)
			{
				stNewInfo.astPostionInfo[byIdx].wHeroID = wHeroID;
				break;
			}
			else
			{
				stNewInfo.astPostionInfo[byIdx].wHeroID = 0;
				return FALSE;
				break;
			}

		}
	}
	return TRUE;
}

BOOL CFormationMgr::SetIdxHeroID(DT_FORMATION_IDX_DATA& stOldInfo, DT_NEW_FORMATION_INFO& stNewInfo, UINT16 wHeroID)
{
	BOOL bMain = FALSE;
	UINT8 byIdx = 0;
	UINT8 byHeroNum = 0;
	if ( EBAK_BAIHU == stNewInfo.byFormationID || EBAK_QINGLONG == stNewInfo.byFormationID)
	{
		for ( ; byIdx < stNewInfo.byPostionNum && byIdx < MAX_FORMATION_IDX_NUM; byIdx++)
		{
			stOldInfo.astOpenIdxInfo[byHeroNum].wHeroID = stNewInfo.astPostionInfo[byIdx].wHeroID;
			if ( wHeroID == stNewInfo.astPostionInfo[byIdx].wHeroID)
			{
				bMain = TRUE;
			}
			if ( 0 != stNewInfo.astPostionInfo[byIdx].wHeroID )
			{
				byHeroNum++;
				if ( stOldInfo.byOpenIdxNum <= byHeroNum )
				{
					break;
				}
			}
		}

		if ( !bMain )
		{
			byHeroNum--;
			stOldInfo.astOpenIdxInfo[byHeroNum].wHeroID = wHeroID;
		}
		UINT8 byIdx = 0;
		for ( ; byIdx < stNewInfo.byPostionNum && byIdx < MAX_FORMATION_IDX_NUM; byIdx++)
		{
			stNewInfo.astPostionInfo[byIdx].wHeroID = 0;
		}
		return TRUE;
	}
	
	return FALSE;
}
//DT_NEW_FORMATION_INFO* CFormationMgr::CreateFormation()
//{
//    return m_stFormationPool.Alloc();
//}
//
//VOID CFormationMgr::RemoveFormation(DT_NEW_FORMATION_INFO* poFormation)
//{
//    if(NULL == poFormation)
//    {
//        return;
//    }
//
//    m_stFormationPool.Free(poFormation);
//}


