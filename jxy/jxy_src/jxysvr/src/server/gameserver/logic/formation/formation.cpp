#include "formation.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/formation/formationpropmgr.h>
#include <logic/hero/hero.h>
#include <sdloggerimpl.h>
#include "logic/science/scienceupdatepropmgr.h"
#include "logic/formation/formationmgr.h"
#include "logic/hero/heropropmgr.h"
IMPLEMENT_CLASS_POOL_MGR(CFormation)

CFormation::CFormation()
{
    memset(&m_stDT_FORMATION_DATA, 0 , sizeof(m_stDT_FORMATION_DATA));
	m_bySelectFormationIdx = 0;
}

CFormation::~CFormation()
{
	m_bySelectFormationIdx = 0;
}

DT_NEW_FORMATION_DATA& CFormation::GetDT_FORMATION_DATA()
{
	m_stDT_FORMATION_DATA.byNewSelectIdx = m_bySelectFormationIdx;
	CMapNewFormationInfo mapUnlockFormation = m_mapUnLockFormationInfo;
	m_stDT_FORMATION_DATA.stNewFormationLst.byFormationNum = mapUnlockFormation.size();
	UINT8 byIdx = 0;
	CMapNewFormationInfoItr itr = mapUnlockFormation.begin();	//已解锁
	for ( ; itr != mapUnlockFormation.end(); itr++, byIdx++)
	{
		m_stDT_FORMATION_DATA.stNewFormationLst.astFormationArray[byIdx] = itr->second;
	}
	CMapNewFormationInfo maplockFormation = m_mapLockFormationInfo;
	itr = maplockFormation.begin();//未解锁
	byIdx = 0;
	m_stDT_FORMATION_DATA.stLockFormationLst.byFormationNum = maplockFormation.size();
	for ( ; itr != maplockFormation.end(); itr++, byIdx++)
	{
		m_stDT_FORMATION_DATA.stLockFormationLst.astFormationArray[byIdx] = itr->second;
	}

	return m_stDT_FORMATION_DATA;
}

BOOL CFormation::Init(DT_NEW_FORMATION_DATA& stDT_FORMATION_DATA, CPlayer* poOwner)
{
	m_poOwner = poOwner;
	if(NULL == poOwner)
	{
		return FALSE;
	}
	memcpy(&m_stDT_FORMATION_DATA, &stDT_FORMATION_DATA, sizeof(DT_NEW_FORMATION_DATA));

	m_mapUnLockFormationInfo.clear();
	m_mapLockFormationInfo.clear();
	m_bySelectFormationIdx = m_stDT_FORMATION_DATA.byNewSelectIdx >= m_stDT_FORMATION_DATA.stNewFormationLst.byFormationNum ? 0 : m_stDT_FORMATION_DATA.byNewSelectIdx;
	UINT8 bySelectFormationID = m_stDT_FORMATION_DATA.stNewFormationLst.astFormationArray[m_bySelectFormationIdx].byFormationID;
	UINT8 byIdx = 0;
	for ( ; byIdx < m_stDT_FORMATION_DATA.stNewFormationLst.byFormationNum; byIdx++)
	{
		AddUnLockFormation(m_stDT_FORMATION_DATA.stNewFormationLst.astFormationArray[byIdx], bySelectFormationID);
	}

	byIdx = 0;
	for ( ; byIdx < m_stDT_FORMATION_DATA.stLockFormationLst.byFormationNum; byIdx++)
	{
		AddLockFormation(m_stDT_FORMATION_DATA.stLockFormationLst.astFormationArray[byIdx]);
	}

	if( !CFormationMgr::Instance()->CheckConvertFormation(m_poOwner) )
	{
		if(stDT_FORMATION_DATA.bySelectIdx >= stDT_FORMATION_DATA.stFormationList.byFormationKindNum)
		{
			stDT_FORMATION_DATA.bySelectIdx = 0;
		}

		DT_FORMATION_DATA_LIST& stFormationList = m_stDT_FORMATION_DATA.stFormationList;

		UINT8 byFormationKindNum = m_stDT_FORMATION_DATA.stFormationList.byFormationKindNum;
		//保存阵型类型对应的下标
		map<UINT8, UINT8> mapHaveFormation;
		for(UINT8 byIdx = 0; byIdx < byFormationKindNum; byIdx++)
		{
			DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[byIdx];
			mapHaveFormation[stDT_FORMATION_IDX_DATA.byFormationKind] = byIdx;
		}

		UINT16 wCoachHeroID = poOwner->GetCoachHero()->GetDT_HERO_BASE_DATA().wKindID;
		//加新整型
		CFormationKindMap& mapFormationInfo = CFormationPropMgr::Instance()->GetFormationKindMap();
		//阵型数据无效，则去除
		if(mapHaveFormation.size() > mapFormationInfo.size())
		{
			memset(&m_stDT_FORMATION_DATA, 0, sizeof(m_stDT_FORMATION_DATA));
			mapHaveFormation.clear();
		}
		for(CFormationKindMapItr itr = mapFormationInfo.begin(); ((itr != mapFormationInfo.end()) && (stFormationList.byFormationKindNum < MAX_FORMATION_NUM)); itr++)
		{
			map<UINT8, UINT8>::iterator itrIdx = mapHaveFormation.find(itr->first);
			if(itrIdx != mapHaveFormation.end())
			{
				BOOL bHaveCoach = FALSE;

				//通过之前保存的阵型对应的下标信息获取相关属性值
				DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[itrIdx->second];

				//检测是否存在主武将在阵上。若不在镇上，完了，不知道出啥问题了。
				for(UINT8 byOpenIdx = 0; byOpenIdx < stDT_FORMATION_IDX_DATA.byOpenIdxNum; byOpenIdx++)
				{
					DT_FORMATION_HERO_DATA& stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdx];
					if(0 == stDT_FORMATION_HERO_DATA.wHeroID)
					{
						continue;
					}
					if(wCoachHeroID == stDT_FORMATION_HERO_DATA.wHeroID)
					{
						bHaveCoach = TRUE;
						break;
					}
				}
				//每一个阵型都必须有主角，而且重新始初化该阵型
				if(bHaveCoach)
				{
					continue;
				}
				memcpy(&stDT_FORMATION_IDX_DATA, &(itr->second.GetDT_FORMATION_IDX_DATA()), sizeof(DT_FORMATION_IDX_DATA));
				stDT_FORMATION_IDX_DATA.astOpenIdxInfo[itr->second.byCoachFormationLstIdx].wHeroID = wCoachHeroID;

				DBG_INFO(_SDT("[%s: %d]: PlayerID[%u] Formation init Faild!"), MSG_MARK, m_poOwner->GetID());
			}
			else
			{
				DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[stFormationList.byFormationKindNum];
				memcpy(&stDT_FORMATION_IDX_DATA, &(itr->second.GetDT_FORMATION_IDX_DATA()), sizeof(DT_FORMATION_IDX_DATA));
				stDT_FORMATION_IDX_DATA.astOpenIdxInfo[itr->second.byCoachFormationLstIdx].wHeroID = wCoachHeroID; //
				stFormationList.byFormationKindNum++;
			}
		}

		CheckFormation();
	}
	else
	{
		UINT8 byMaxFormation = CFormationPropMgr::Instance()->GetFormationLevelAllMap().size();
		if( stDT_FORMATION_DATA.byNewSelectIdx >= byMaxFormation )
		{
			stDT_FORMATION_DATA.byNewSelectIdx = 0;
		}

		m_bySelectFormationIdx = m_stDT_FORMATION_DATA.byNewSelectIdx;
		if ( ESM_YES != stDT_FORMATION_DATA.byIsUseNewFormation )
		{
			return UpdateFormation();
		}
		
	}
	
	if( !Embattle())
	{
		return FALSE;
	}
    return TRUE;
}
VOID CFormation::ResetFormation(DT_FORMATION_IDX_DATA& stInfo, UINT8 byFormationID)
{
	const CMapFormationLevelProp* poLevelProp = CFormationPropMgr::Instance()->GetFormationLevelMap(byFormationID);
	if ( NULL != poLevelProp)
	{
		CMapFormationLevelPropConstItr propItr = poLevelProp->begin();
		stInfo.byOpenIdxNum = poLevelProp->size();	//位置数
		UINT8 byPosIdx = 0;
		for ( ; propItr != poLevelProp->end(); propItr++)
		{
			if( ESM_YES == propItr->second.byCoachIdxFlag )
			{

				stInfo.astOpenIdxInfo[byPosIdx].wHeroID = m_poOwner->GetCoachHeroKindID();	//主将位置
			}
			else
			{
				stInfo.astOpenIdxInfo[byPosIdx].wHeroID = 0;

			}
			byPosIdx++;
		}
	}
}

//遣散回调
VOID CFormation::OnDisband(UINT16 wHeroID)
{
    DT_FORMATION_DATA_LIST& stFormationList = m_stDT_FORMATION_DATA.stFormationList;
    UINT8 byFormationKindNum = m_stDT_FORMATION_DATA.stFormationList.byFormationKindNum;
    for(UINT8 byIdx = 0; byIdx < byFormationKindNum; byIdx++)
    {
        DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[byIdx];
        UINT8 byOpenIdxNum = stDT_FORMATION_IDX_DATA.byOpenIdxNum;
        for(UINT8 byOpenIdx = 0; byOpenIdx < byOpenIdxNum; byOpenIdx++)
        {
            DT_FORMATION_HERO_DATA& stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdx];
            if(stDT_FORMATION_HERO_DATA.wHeroID == wHeroID)
            {
                stDT_FORMATION_HERO_DATA.wHeroID = 0;
            }
        }
    }
}

//打开布阵面板
UINT16 CFormation::OpenEmbattleTab(UINT16& wHeroNum, DT_HERO_EMBATLE_DATA astHeroInfoLst[MAX_HERO_NUM], DT_FORMATION_DATA& stFormationInfo)
{
    //若没有开阵型，则开阵型
    if(NULL == m_poOwner->GetBuild(EBK_FROMATION))
    {
        return ERR_OPEN_EMBATTLE::ID_FORMATION_NOT_OPEN;
    }

	if ( ESM_YES == m_stDT_FORMATION_DATA.byIsUseNewFormation)
	{
		NewToOldFormation();
		m_stDT_FORMATION_DATA.byIsUseNewFormation = 0;
		Embattle();
	}

    for( std::size_t nIndex = 0; nIndex < m_poOwner->GetAllCarryHeroMap().Size(); ++nIndex )
    {
        CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetDataByIndex( nIndex );
        if(NULL == poHero)
        {
            continue;
        }
        poHero->GetDT_HERO_EMBATLE_DATA(astHeroInfoLst[wHeroNum++]);
    }

	stFormationInfo.bySelectIdx = m_stDT_FORMATION_DATA.bySelectIdx;
	stFormationInfo.stFormationList = m_stDT_FORMATION_DATA.stFormationList;

    return ERR_OPEN_EMBATTLE::ID_SUCCESS;
}

//打开布阵面板
UINT16 CFormation::OpenEmbattleTab2( PKT_CLIGS_OPEN_EMBATTLE_ACK2& stAck )
{
	UpdateFormation();
	for( std::size_t nIndex = 0; nIndex < m_poOwner->GetAllCarryHeroMap().Size(); ++nIndex )
	{
		CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetDataByIndex( nIndex );
		if(NULL == poHero)
		{
			continue;
		}
		
		poHero->GetDT_NEW_HERO_EMBATLE_DATA(stAck.astHeroInfoLst[stAck.wHeroNum++]);	
	}

	if ( !GetDT_NEW_FORMATION_LST_CLIByUnlock( stAck.stFormationInfo))
	{
		return ERR_OPEN_EMBATTLE::ID_FORMATION_ID_INVALID;
	}
	
	stAck.stCurRes.byResKind = ACTE_STORY;
	stAck.stCurRes.qwResValue = m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory;
	return ERR_OPEN_EMBATTLE::ID_SUCCESS;
}

UINT16 CFormation::Embattle(DT_FORMATION_DATA& stFormationInfo)
{
	if ( CFormationMgr::Instance()->CheckConvertFormation(m_poOwner) )
	{
		return NewEmbattle(stFormationInfo);
	}
	else
	{
		return OldEmbattle(stFormationInfo);
	}
	return ERR_EMBATTLE::ID_SUCCESS;
}

//布阵
BOOL CFormation::Embattle()
{
	DT_FORMATION_IDX_DATA stDT_FORMATION_IDX_DATA = {0};
	if ( CFormationMgr::Instance()->CheckConvertFormation(m_poOwner) )
	{	//没阵法数据
		if ( 0  == m_mapUnLockFormationInfo.size() )
		{
			UpdateFormation();
		}
		DT_NEW_FORMATION_INFO& stNewInfo = m_mapUnLockFormationInfo[m_bySelectFormationIdx];
		CFormationMgr::Instance()->ConvertFormationIdx(stDT_FORMATION_IDX_DATA, stNewInfo);
	}
	else
	{
		stDT_FORMATION_IDX_DATA = m_stDT_FORMATION_DATA.stFormationList.astFormationInfo[m_stDT_FORMATION_DATA.bySelectIdx];
	}
	
    //初始化原先的数据
    memset(m_poOwner->m_apoFormationObj, 0, sizeof(m_poOwner->m_apoFormationObj));

    //先全部下阵
    CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetFistData();
    do
    {
        if(NULL == poHero)
        {
            break;
        }
        poHero->OnDown();
    }
    while (poHero = m_poOwner->GetAllCarryHeroMap().GetNextData());
    m_poOwner->GetFormationHeroMap().Clear();
    //m_vecCostarIndexByAllCarry.clear();
    //上阵
    UINT8 byOpenIdxNum = stDT_FORMATION_IDX_DATA.byOpenIdxNum;
    map<UINT16, CHero*> mapHaveHero;
    for(UINT8 byOpenIdx = 0; byOpenIdx < byOpenIdxNum; byOpenIdx++)
    {
        DT_FORMATION_HERO_DATA stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdx];
        CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetData(stDT_FORMATION_HERO_DATA.wHeroID);
        if(poHero)
        {
            UINT8 byFormationIdx = stDT_FORMATION_HERO_DATA.byFormationIdx;
            if(byFormationIdx > MAX_FORMATION_IDX_NUM)
            {
                return FALSE;
            }
            //矫正重复武将的问题
            if(mapHaveHero.find(stDT_FORMATION_HERO_DATA.wHeroID) == mapHaveHero.end())
            {
                poHero->OnUp(byFormationIdx);
                m_poOwner->GetFormationHeroMap().AddData(byFormationIdx, poHero);
                m_poOwner->m_apoFormationObj[byFormationIdx] = poHero;

                //if( m_poCoachHero->GetDT_HERO_BASE_DATA().wKindID!=stDT_FORMATION_HERO_DATA.wHeroID )
                //{
                //	m_vecCostarIndexByAllCarry.push_back( stDT_FORMATION_HERO_DATA.wHeroID );
                //}

            }

            mapHaveHero[stDT_FORMATION_HERO_DATA.wHeroID] = poHero;
        }
    }
	m_poOwner->ResetBattleAttr(TRUE);
    ////武将排序
    //std::sort( m_vecCostarIndexByAllCarry.begin(), m_vecCostarIndexByAllCarry.end(),
    //	std::bind( &CPlayer::FormationHeroSortByCarryIndex,
    //			this, std::placeholders::_1, std::placeholders::_2 ) );

    return TRUE;
}

VOID CFormation::GetFormationLevel(DT_FORMATION_DATA_LEVEL& stLevel, UINT8 byFormationID, UINT8 byFormationLevel)
{
	
	const SFormationUpgradeProp* poProp = CFormationPropMgr::Instance()->GetFormationUpgrade(byFormationID, byFormationLevel);
	if ( NULL == poProp)
	{
		return ;
	}
	stLevel.stUpgradeCost.byResKind = ACTE_STORY;
	stLevel.stUpgradeCost.qwResValue = poProp->dwUpgradeCostValue;
	stLevel.byFormationLevel = byFormationLevel;
	stLevel.byFormationID = byFormationID;
	stLevel.byFormationType = poProp->byAttrKindID;
	stLevel.byNeedPlayerLevel = poProp->byNeedPlayerLevel;
	stLevel.dwFormationValue = poProp->dwAttrValue;
	stLevel.byMaxPlayerNum = poProp->byUnlockPosNum;
	UINT8 byVisableLevel = 0;
	UINT8 byUnlockLevel = 0;
	if ( 0 == poProp->dwUpgradeCostValue)
	{
		stLevel.byIsUpgrade = 6;//满级
		return ;
	}
	
	CFormationPropMgr::Instance()->GetFormationUnlock(byFormationID, byVisableLevel, byUnlockLevel);
	//（0 正常， 1科技未解锁， 2资源不足，3 玩家等级不足升级, 4未显示阵法， 5未解锁阵法， 6满级）
	if ( NULL == m_poOwner->GetBuild(EBK_SCIENCE))
	{
		stLevel.byIsUpgrade = 1;
	}
	else if ( m_poOwner->GetLevel() < byVisableLevel)
	{
		stLevel.byIsUpgrade = 4;
	}
	else if ( m_poOwner->GetLevel() < byUnlockLevel)
	{
		stLevel.byIsUpgrade = 5;
		stLevel.byNeedPlayerLevel = byUnlockLevel;
	}
	else if (  m_poOwner->GetLevel() < poProp->byNeedPlayerLevel )
	{
		stLevel.byIsUpgrade = 3;
	}
	else if ( m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory < poProp->dwUpgradeCostValue)
	{
		stLevel.byIsUpgrade = 2;
	}
	else
	{
		stLevel.byIsUpgrade = 0;
	}
}

BOOL CFormation::GetDT_NEW_FORMATION_LST_CLIByAll(DT_NEW_FORMATION_LST_CLI& stCli)
{
	stCli.bySelectIndex = m_bySelectFormationIdx;
	stCli.byFormationNum = m_mapUnLockFormationInfo.size();
	UINT8 byIndex = 0;
	for ( ; byIndex < m_mapUnLockFormationInfo.size(); byIndex++)
	{
		stCli.astFormationArray[byIndex].byFormationID = m_mapUnLockFormationInfo[byIndex].byFormationID;
		const CMapFormationLevelProp* poLevelProp = CFormationPropMgr::Instance()->GetFormationLevelMap(stCli.astFormationArray[byIndex].byFormationID);
		if ( NULL == poLevelProp)
		{
			return FALSE;
		}
		//阵型等级信息
		GetFormationLevel(stCli.astFormationArray[byIndex].stFormationLevel, m_mapUnLockFormationInfo[byIndex].byFormationID, m_mapUnLockFormationInfo[byIndex].byFormationLevel);
		stCli.astFormationArray[byIndex].byPostionNum = poLevelProp->size();
		CMapFormationLevelPropConstItr constItr = poLevelProp->begin();

		//位置信息
		for ( UINT8 byIdx = 0; constItr != poLevelProp->end(); byIdx++)
		{
			if (constItr->second.byUnlockLevel <= stCli.astFormationArray[byIndex].stFormationLevel.byFormationLevel)
			{
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byIsUnlock = ESM_YES;
			}
			else
			{
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byIsUnlock = ESM_NO;
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byNeedLevel = constItr->second.byUnlockLevel;
			}
			stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byPostionIndex = constItr->second.byOpenIdx;
			CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetData(m_mapUnLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID);
			if(NULL == poHero)
			{
				m_mapUnLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID = 0;
			}
			stCli.astFormationArray[byIndex].astPostionInfo[byIdx].wHeroID = m_mapUnLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID;
		}
	}

	stCli.byFormationNum += m_mapLockFormationInfo.size();
	UINT8 byIdx = 0;
	for ( ; byIndex < stCli.byFormationNum ; byIndex++)
	{
		stCli.astFormationArray[byIndex].byFormationID = m_mapLockFormationInfo[byIdx].byFormationID;
		const CMapFormationLevelProp* poLevelProp = CFormationPropMgr::Instance()->GetFormationLevelMap(stCli.astFormationArray[byIndex].byFormationID);
		if ( NULL == poLevelProp)
		{
			return FALSE;
		}
		//阵型等级信息
		GetFormationLevel(stCli.astFormationArray[byIndex].stFormationLevel, m_mapLockFormationInfo[byIndex].byFormationID, m_mapLockFormationInfo[byIdx].byFormationLevel);
		stCli.astFormationArray[byIndex].byPostionNum = poLevelProp->size();
		CMapFormationLevelPropConstItr constItr = poLevelProp->begin();

		//位置信息
		for ( UINT8 byIdx = 0; constItr != poLevelProp->end(); byIdx++)
		{
			if (constItr->second.byUnlockLevel <= stCli.astFormationArray[byIndex].stFormationLevel.byFormationLevel)
			{
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byIsUnlock = ESM_YES;
			}
			else
			{
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byIsUnlock = ESM_NO;
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byNeedLevel = constItr->second.byUnlockLevel;
			}
			stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byPostionIndex = constItr->second.byOpenIdx;
			CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetData(m_mapLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID);
			if(NULL == poHero)
			{
				m_mapUnLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID = 0;
			}
			stCli.astFormationArray[byIndex].astPostionInfo[byIdx].wHeroID = m_mapLockFormationInfo[byIdx].astPostionInfo[byIdx].wHeroID;
		}
	}
	return TRUE;
}

BOOL CFormation::GetDT_NEW_FORMATION_LST_CLIByUnlock(DT_NEW_FORMATION_LST_CLI& stCli)
{
	stCli.bySelectIndex = m_bySelectFormationIdx;
	stCli.byFormationNum = m_mapUnLockFormationInfo.size();

	for ( UINT8 byIndex = 0; byIndex < stCli.byFormationNum; byIndex++)
	{
		stCli.astFormationArray[byIndex].byFormationID = m_mapUnLockFormationInfo[byIndex].byFormationID;
		const CMapFormationLevelProp* poLevelProp = CFormationPropMgr::Instance()->GetFormationLevelMap(stCli.astFormationArray[byIndex].byFormationID);
		if ( NULL == poLevelProp)
		{
			return FALSE;
		}
		//阵型等级信息
		GetFormationLevel(stCli.astFormationArray[byIndex].stFormationLevel, m_mapUnLockFormationInfo[byIndex].byFormationID, m_mapUnLockFormationInfo[byIndex].byFormationLevel);
		stCli.astFormationArray[byIndex].byPostionNum = poLevelProp->size();
		CMapFormationLevelPropConstItr constItr = poLevelProp->begin();

		//位置信息
		for ( UINT8 byIdx = 0; constItr != poLevelProp->end(); constItr++, byIdx++)
		{
			if (constItr->second.byUnlockLevel <= stCli.astFormationArray[byIndex].stFormationLevel.byFormationLevel)
			{
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byIsUnlock = ESM_YES;
			}
			else
			{
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byIsUnlock = ESM_NO;
				stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byNeedLevel = constItr->second.byUnlockLevel;
			}
			stCli.astFormationArray[byIndex].astPostionInfo[byIdx].byPostionIndex = constItr->second.byOpenIdx;
			CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetData(m_mapUnLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID);
			if(NULL == poHero)
			{
				m_mapUnLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID = 0;
			}
			stCli.astFormationArray[byIndex].astPostionInfo[byIdx].wHeroID = m_mapUnLockFormationInfo[byIndex].astPostionInfo[byIdx].wHeroID;
		}
	}
	return TRUE;
}

UINT16 CFormation::OnOpenFormationScience(PKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK& stAck)
{
	UINT8 byFormationNum = m_mapUnLockFormationInfo.size() + m_mapLockFormationInfo.size();
	UINT8 byCliNum = 0;
	UINT8 byIndex = 0;
	UpdateFormation();
	UINT8 byUnlockNum = m_mapUnLockFormationInfo.size();
	for ( ; byIndex < byUnlockNum; byIndex++)
	{
		DT_NEW_FORMATION_INFO& stNewInfo = m_mapUnLockFormationInfo[byIndex];
		GetFormationLevel(stAck.astFormationInfo[byCliNum], stNewInfo.byFormationID, stNewInfo.byFormationLevel);
		if ( 4 != stAck.astFormationInfo[byCliNum].byIsUpgrade)
		{
			byCliNum++;
		}
	}
	UINT8 byLockNum = m_mapLockFormationInfo.size();
	byIndex = 0;
	for ( ; byIndex < byLockNum; byIndex++)
	{
		DT_NEW_FORMATION_INFO& stNewInfo = m_mapLockFormationInfo[byIndex];
		GetFormationLevel(stAck.astFormationInfo[byCliNum], stNewInfo.byFormationID, stNewInfo.byFormationLevel);
		if ( 4 != stAck.astFormationInfo[byCliNum].byIsUpgrade)
		{
			byCliNum++;
		}
	}
	stAck.byFormationNum = byCliNum;

	stAck.stCurRes.byResKind = ACTE_STORY;
	stAck.stCurRes.qwResValue = m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory;
	return ERR_OPEN_FORMATION_SCIENCE::ID_SUCCESS;
}

UINT16 CFormation::OnUpgradeFormation(PKT_CLIGS_UPGRADE_FORMATION_ACK& stAck, UINT8 byFormationID, DT_FORMATION_DATA& stFormation )
{

	if( NULL == m_poOwner->GetBuild(EBK_SCIENCE))
	{
		return ERR_UPGRADE_FORMATION::ID_FORMATION_NOT_UPGRADE;
	}

	C88MapItr posItr = m_mapUnLockPos.find(byFormationID);
	if ( posItr == m_mapUnLockPos.end())
	{
		C88MapItr lockPosItr = m_mapLockPos.find(byFormationID);
		if ( lockPosItr == m_mapLockPos.end())
		{
			return ERR_UPGRADE_FORMATION::ID_FORMATION_UNLOCK;	//未显示也未解锁
		}
		return ERR_UPGRADE_FORMATION::ID_FORMATION_UNLOCK;//显示但未解锁
	}
	CMapNewFormationInfoItr itr = m_mapUnLockFormationInfo.find(posItr->second);
	if (itr == m_mapUnLockFormationInfo.end())
	{
		return ERR_UPGRADE_FORMATION::ID_FORMATION_ID_INVALIAD;	//未显示也未解锁
	}

	DT_FORMATION_DATA_LEVEL stLevel = {0};
	GetFormationLevel(stLevel, itr->second.byFormationID, itr->second.byFormationLevel);
	if ( 1 == stLevel.byIsUpgrade )
	{
		return ERR_UPGRADE_FORMATION::ID_FORMATION_NOT_UPGRADE;//科技馆未解锁，不能升级
	}
	else if ( 2 == stLevel.byIsUpgrade )
	{
		return ERR_UPGRADE_FORMATION::ID_STORY_NOT_ENOUGH;//阅历不够
	}
	else if ( 3 == stLevel.byIsUpgrade )
	{
		return ERR_UPGRADE_FORMATION::ID_PLAYER_LEVEL_NOT_ENOUGH;//玩家等级不够
	}
	else if ( 4 == stLevel.byIsUpgrade || 5 == stLevel.byIsUpgrade)
	{
		return ERR_UPGRADE_FORMATION::ID_FORMATION_UNLOCK;//技能还未解锁
	}
	else if ( 6 == stLevel.byIsUpgrade)
	{
		return ERR_UPGRADE_FORMATION::ID_MAX_FORMATION_LEVEL; //技能满级
	}
	const SFormationUpgradeProp* poProp = CFormationPropMgr::Instance()->GetFormationUpgrade( itr->second.byFormationID, itr->second.byFormationLevel);
	if ( NULL == poProp)
	{
		return ERR_UPGRADE_FORMATION::ID_MAX_FORMATION_LEVEL;
	}
	UINT16 wErrCode = NewEmbattle( stFormation );
	if( 0 != wErrCode)
	{
		return wErrCode;
	}
	//扣除阅历
	m_poOwner->DecStory(poProp->dwUpgradeCostValue, CRecordMgr::EDST_UPGRADE_FORMATION, poProp->byFormationID);

	itr->second.byFormationLevel += 1;	//等级+1
	UnLockPos(itr->second);//解锁位置
	m_bySelectFormationIdx = posItr->second;
	for( std::size_t nIndex = 0; nIndex < m_poOwner->GetAllCarryHeroMap().Size(); ++nIndex )
	{
		CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetDataByIndex( nIndex );
		if(NULL == poHero)
		{
			continue;
		}
		poHero->GetDT_NEW_HERO_EMBATLE_DATA(stAck.astHeroInfoLst[stAck.wHeroNum++]);	
	}

	if ( !GetDT_NEW_FORMATION_LST_CLIByUnlock(stAck.stFormationInfo))
	{
		return ERR_UPGRADE_FORMATION::ID_FORMATION_ID_INVALIAD;
	}
	
	

	m_poOwner->GetSciencePrompt(stAck.stPromptInfo);//提示更新

	stAck.stSyncRes.byResKind = ACTE_STORY;
	stAck.stSyncRes.qwResValue = m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory;

	stAck.stPower.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	m_poOwner->ResetBattleAttr(TRUE);
	stAck.stPower.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	
	return ERR_UPGRADE_FORMATION::ID_SUCCESS;
}

UINT16 CFormation::OnUpgradeFormationScience(PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK& stAck, UINT8 byFormationID)
{
	if( NULL == m_poOwner->GetBuild(EBK_SCIENCE))
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_NOT_UPGRADE;
	}
	UpdateFormation();
	C88MapItr posItr = m_mapUnLockPos.find(byFormationID);
	if ( posItr == m_mapUnLockPos.end())
	{
		C88MapItr lockPosItr = m_mapLockPos.find(byFormationID);
		if ( lockPosItr == m_mapLockPos.end())
		{
			return ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_UNLOCK;	//未显示也未解锁
		}
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_UNLOCK;//显示但未解锁
	}
	CMapNewFormationInfoItr itr = m_mapUnLockFormationInfo.find(posItr->second);
	if (itr == m_mapUnLockFormationInfo.end())
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_ID_INVALIAD;	//未显示也未解锁
	}

	const SFormationUpgradeProp* poProp = CFormationPropMgr::Instance()->GetFormationUpgrade( itr->second.byFormationID, itr->second.byFormationLevel);
	if ( NULL == poProp)
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_ID_INVALIAD;
	}
	DT_FORMATION_DATA_LEVEL stLevel = {0};
	GetFormationLevel(stLevel, itr->second.byFormationID, itr->second.byFormationLevel);
	if ( 1 == stLevel.byIsUpgrade )
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_NOT_UPGRADE;//科技馆未解锁，不能升级
	}
	else if ( 2 == stLevel.byIsUpgrade )
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_STORY_NOT_ENOUGH;//阅历不够
	}
	else if ( 3 == stLevel.byIsUpgrade )
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_PLAYER_LEVEL_NOT_ENOUGH;//玩家等级不够
	}
	else if ( 4 == stLevel.byIsUpgrade || 5 == stLevel.byIsUpgrade)
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_FORMATION_UNLOCK;//技能还未解锁
	}
	else if ( 6 == stLevel.byIsUpgrade)
	{
		return ERR_UPGRADE_FORMATION_SCIENCE::ID_MAX_FORMATION_LEVEL; //技能满级
	}

	//扣除阅历
	m_poOwner->DecStory(poProp->dwUpgradeCostValue, CRecordMgr::EDST_UPGRADE_FORMATION, poProp->byFormationID);

	itr->second.byFormationLevel += 1;	//等级+1
	UnLockPos(itr->second);//解锁位置
	UINT8 byFormationNum = m_mapUnLockFormationInfo.size() + m_mapLockFormationInfo.size();
	UINT8 byCliNum = 0;
	UINT8 byIndex = 0;
	
	for ( ; byIndex < m_mapUnLockFormationInfo.size(); byIndex++)
	{
		DT_NEW_FORMATION_INFO& stNewInfo = m_mapUnLockFormationInfo[byIndex];
		GetFormationLevel(stAck.astFormationInfo[byCliNum], stNewInfo.byFormationID, stNewInfo.byFormationLevel);
		if ( 4 != stAck.astFormationInfo[byCliNum].byIsUpgrade)
		{
			byCliNum++;
		}
	}

	byIndex = 0;
	for ( ; byIndex <  m_mapLockFormationInfo.size(); byIndex++)
	{
		DT_NEW_FORMATION_INFO& stNewInfo = m_mapLockFormationInfo[byIndex];
		GetFormationLevel(stAck.astFormationInfo[byCliNum], stNewInfo.byFormationID, stNewInfo.byFormationLevel);
		if ( 4 != stAck.astFormationInfo[byCliNum].byIsUpgrade)
		{
			byCliNum++;
		}
	}
	stAck.byFormationNum = byCliNum;

	m_poOwner->GetSciencePrompt(stAck.stPromptInfo);//提示更新

	stAck.stSyncRes.byResKind = ACTE_STORY;
	stAck.stSyncRes.qwResValue = m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory;

	stAck.stPower.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	m_poOwner->ResetBattleAttr(TRUE);
	stAck.stPower.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;

	return ERR_UPGRADE_FORMATION_SCIENCE::ID_SUCCESS;
}

UINT16 CFormation::NewEmbattle(DT_FORMATION_DATA& stFormationInfo)
{
	UpdateFormation();
	if(stFormationInfo.bySelectIdx >= stFormationInfo.stFormationList.byFormationKindNum)
	{
		return ERR_EMBATTLE::ID_FORMATION_KIND_NOT_EXIST;
	}

	if( m_mapUnLockFormationInfo.size() != stFormationInfo.stFormationList.byFormationKindNum)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Embattle req Num invalid!"), MSG_MARK);
		return ERR_EMBATTLE::ID_FORMATION_INDEX_INVALID_ERR;
	}

	BOOL bHaveCoach = FALSE;
	UINT8 byFormationKindNum = stFormationInfo.stFormationList.byFormationKindNum;
	DT_FORMATION_DATA_LIST& stFormationList = stFormationInfo.stFormationList;
	for(UINT8 byIdx = 0; byIdx < byFormationKindNum; byIdx++)
	{
		DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[byIdx];
		const DT_NEW_FORMATION_INFO& stFormationInfo = m_mapUnLockFormationInfo[byIdx];
		BOOL bHaveCoach = FALSE;

		const CMapFormationLevelProp* poLevelProp = CFormationPropMgr::Instance()->GetFormationLevelMap(stDT_FORMATION_IDX_DATA.byFormationKind);
		if(NULL == poLevelProp)
		{
			return ERR_EMBATTLE::ID_FORMATION_KIND_NOT_EXIST; 
		}

		if ( m_mapUnLockFormationInfo[byIdx].byFormationID != stDT_FORMATION_IDX_DATA.byFormationKind)//同位置下的阵型不一致
		{
			return ERR_EMBATTLE::ID_FORMATION_EXCEPTION;
		}

		set<UINT16> setHaveHero;
		UINT8 byOpenIdxNum = stDT_FORMATION_IDX_DATA.byOpenIdxNum;
		for(UINT8 byOpenIdx = 0; byOpenIdx < byOpenIdxNum && byOpenIdx < poLevelProp->size(); byOpenIdx++)
		{
			DT_FORMATION_HERO_DATA& stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdx];
			if(poLevelProp->end() == poLevelProp->find(stDT_FORMATION_HERO_DATA.byFormationIdx))
			{
				return ERR_EMBATTLE::ID_FORMATION_INDEX_INVALID_ERR;
			}
			if(0 == stDT_FORMATION_HERO_DATA.wHeroID)
			{
				continue;
			}

			if ( !IsUnlock(m_mapUnLockFormationInfo[byIdx].byFormationID, stDT_FORMATION_HERO_DATA.byFormationIdx ))
			{
				return ERR_EMBATTLE::ID_POSTION_NOT_PLAYER;
			}

			CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetData(stDT_FORMATION_HERO_DATA.wHeroID);
			if(NULL == poHero)
			{
				//return ERR_EMBATTLE::ID_HERO_NOT_EXIST_ERR;
				stDT_FORMATION_HERO_DATA.wHeroID = 0;//不应该能走到这。避免布阵失败，不存在的伙伴直接下阵
				continue;
			}
			if(poHero == m_poOwner->GetCoachHero())
			{
				bHaveCoach = TRUE;
			}
			if(setHaveHero.end() != setHaveHero.find(stDT_FORMATION_HERO_DATA.wHeroID))
			{
				//return ERR_EMBATTLE::ID_FORMATION_INDEX_HAVE_HERO_ERR;
				stDT_FORMATION_HERO_DATA.wHeroID = 0;//不应该能走到这。避免布阵失败，重复在不同位置的伙伴直接去掉重复
				continue;
			}
			setHaveHero.insert(stDT_FORMATION_HERO_DATA.wHeroID);
			
		}
		//每一个阵型都必须有主角，而且失败
		if(!bHaveCoach)
		{
			ResetFormation(stDT_FORMATION_IDX_DATA, m_mapUnLockFormationInfo[byIdx].byFormationID);
			//return ERR_EMBATTLE::ID_COACH_NOT_IN_FORMATION;
		}
		CFormationMgr::Instance()->ConvertIdxDataToInfo(m_mapUnLockFormationInfo[byIdx], stDT_FORMATION_IDX_DATA);
	}

	m_bySelectFormationIdx = stFormationInfo.bySelectIdx;
	//上阵
	Embattle();


	//重置玩家属性
	m_poOwner->ResetBattleAttr(TRUE);

	m_poOwner->AddGuideRecord( EGF_EMBATTLE);
	//3个武将时2次布阵引导
	if(m_poOwner->GetFormationHeroMap().Size() == 3)
	{
		m_poOwner->AddGuideRecord( EGF_SECOND_EMBATTLE);
	}

	return ERR_EMBATTLE::ID_SUCCESS;
}

UINT16 CFormation::OldEmbattle(DT_FORMATION_DATA& stFormationInfo)
{
	if(stFormationInfo.bySelectIdx >= stFormationInfo.stFormationList.byFormationKindNum)
	{
		return ERR_EMBATTLE::ID_FORMATION_KIND_NOT_EXIST;
	}

	if(CFormationPropMgr::Instance()->GetFormationKindMap().size() != stFormationInfo.stFormationList.byFormationKindNum)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Embattle req Num invalid!"), MSG_MARK);
		return ERR_EMBATTLE::ID_FORMATION_INDEX_INVALID_ERR;
	}

	BOOL bHaveCoach = FALSE;
	UINT8 byFormationKindNum = stFormationInfo.stFormationList.byFormationKindNum;
	DT_FORMATION_DATA_LIST& stFormationList = stFormationInfo.stFormationList;
	for(UINT8 byIdx = 0; byIdx < byFormationKindNum; byIdx++)
	{
		DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[byIdx];
		SFormationProp* pstProp = CFormationPropMgr::Instance()->GetFormationProp(stDT_FORMATION_IDX_DATA.byFormationKind);
		if(NULL == pstProp)
		{
			return ERR_EMBATTLE::ID_FORMATION_KIND_NOT_EXIST;
		}
		CFormationIdxMap& mapOpenIdx = pstProp->mapOpenIdx;
		map<UINT16, UINT16> mapHaveHero;
		UINT8 byOpenIdxNum = stDT_FORMATION_IDX_DATA.byOpenIdxNum;
		for(UINT8 byOpenIdx = 0; byOpenIdx < byOpenIdxNum; byOpenIdx++)
		{
			DT_FORMATION_HERO_DATA& stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdx];
			if(mapOpenIdx.end() == mapOpenIdx.find(stDT_FORMATION_HERO_DATA.byFormationIdx))
			{
				return ERR_EMBATTLE::ID_FORMATION_INDEX_INVALID_ERR;
			}
			if(0 == stDT_FORMATION_HERO_DATA.wHeroID)
			{
				continue;
			}
			CHero* poHero = m_poOwner->GetAllCarryHeroMap().GetData(stDT_FORMATION_HERO_DATA.wHeroID);
			if(NULL == poHero)
			{
				//return ERR_EMBATTLE::ID_HERO_NOT_EXIST_ERR;
				stDT_FORMATION_HERO_DATA.wHeroID = 0;//不应该能走到这。避免布阵失败，不存在的伙伴直接下阵
				continue;
			}
			if(poHero == m_poOwner->GetCoachHero())
			{
				bHaveCoach = TRUE;
			}
			if(mapHaveHero.end() != mapHaveHero.find(stDT_FORMATION_HERO_DATA.wHeroID))
			{
				//return ERR_EMBATTLE::ID_FORMATION_INDEX_HAVE_HERO_ERR;
				stDT_FORMATION_HERO_DATA.wHeroID = 0;//不应该能走到这。避免布阵失败，重复在不同位置的伙伴直接去掉重复
				continue;
			}
			mapHaveHero[stDT_FORMATION_HERO_DATA.wHeroID] = stDT_FORMATION_HERO_DATA.wHeroID;
		}
		//每一个阵型都必须有主角，而且失败
		if(!bHaveCoach)
		{
			return ERR_EMBATTLE::ID_COACH_NOT_IN_FORMATION;
		}
	}
	////上阵
	if(!OldEmbattleOnFormation(stFormationInfo))
	{
		RETURN_OTHER_ERR;
	}


	//重置玩家属性
	m_poOwner->ResetBattleAttr();

	m_poOwner->AddGuideRecord( EGF_EMBATTLE);
	//3个武将时2次布阵引导
	if(m_poOwner->GetFormationHeroMap().Size() == 3)
	{
		m_poOwner->AddGuideRecord( EGF_SECOND_EMBATTLE);
	}

	return ERR_EMBATTLE::ID_SUCCESS;
}

BOOL CFormation::UpdateFormation()
{

	UINT16 wPlayerLevel = m_poOwner->GetLevel();
	const CMapFormationLevelMap& newFormationMap = CFormationPropMgr::Instance()->GetFormationLevelAllMap();
	CMapFormationLevelMapConstItr constItr = newFormationMap.begin();
	for ( ; constItr != newFormationMap.end(); constItr++)
	{
		UINT8 byUnlockLevel = 0;
		UINT8 byVisableLevel = 0;
		BOOL bRet = CFormationPropMgr::Instance()->GetFormationUnlock(constItr->first, byVisableLevel, byUnlockLevel);

		
		if ( bRet )
		{
			if ( byVisableLevel <= wPlayerLevel)	//可显示
			{
				if ( byUnlockLevel <= wPlayerLevel) //已解锁
				{
					C88MapItr unlockItr = m_mapUnLockPos.find( constItr->first );
					if (unlockItr != m_mapUnLockPos.end())	//已存在不用改变
					{
						continue;
					}
					else//新解锁
					{
						C88MapItr lockItr = m_mapLockPos.find(constItr->first);
						if ( lockItr != m_mapLockPos.end())	//已存在
						{
							AddUnLockFormation( m_mapLockFormationInfo[lockItr->second], m_mapUnLockFormationInfo[m_bySelectFormationIdx].byFormationID);
							m_mapLockFormationInfo.erase(lockItr->second);
							m_mapLockPos.erase(lockItr);
							continue;
						}
						else
						{
							DT_NEW_FORMATION_INFO stInfo = {0};
							CreateFormation(stInfo, constItr->first);
							AddUnLockFormation( stInfo, m_mapUnLockFormationInfo[m_bySelectFormationIdx].byFormationID);
							continue;
						}
					}
				}
				else	//可显示但还未解锁
				{
					C88MapItr lockItr = m_mapLockPos.find(constItr->first);
					if ( lockItr != m_mapLockPos.end() )
					{
						continue;	////已存在不用改变
					}
				}
			}
			else
			{
				continue;
			}
		}
		else	//找不到阵法
		{
			continue;
		}

		DT_NEW_FORMATION_INFO stInfo = {0};
		CreateFormation(stInfo, constItr->first);
		AddLockFormation( stInfo);
	}
	if ( ESM_YES != m_stDT_FORMATION_DATA.byIsUseNewFormation)
	{
		m_stDT_FORMATION_DATA.byIsUseNewFormation = ESM_YES;
		m_stDT_FORMATION_DATA.byNewSelectIdx = m_stDT_FORMATION_DATA.bySelectIdx;
		m_bySelectFormationIdx = m_stDT_FORMATION_DATA.byNewSelectIdx;
		//旧阵型转新阵型
		if( FALSE == OldToNewFormation())
		{
			return FALSE;
		}

		if( !Embattle())
		{
			return FALSE;
		}
	}
	return TRUE;
}

VOID CFormation::CreateFormation(DT_NEW_FORMATION_INFO& stInfo, UINT8 byFormationID)
{

	const CMapFormationLevelProp* poLevelProp = CFormationPropMgr::Instance()->GetFormationLevelMap(byFormationID);
	if ( NULL != poLevelProp)
	{
		CMapFormationLevelPropConstItr propItr = poLevelProp->begin();
		stInfo.byPostionNum = poLevelProp->size();	//位置数
		UINT8 byPosIdx = 0;
		for ( ; propItr != poLevelProp->end(); propItr++)
		{
			if( ESM_YES == propItr->second.byCoachIdxFlag )
			{
				stInfo.byFormationID = propItr->second.byFormationID;	//阵型ID
				
				stInfo.astPostionInfo[byPosIdx].wHeroID = m_poOwner->GetCoachHeroKindID();	//主将位置
				stInfo.astPostionInfo[byPosIdx].byPostionIndex = propItr->second.byOpenIdx;
				stInfo.astPostionInfo[byPosIdx].byIsUnlock = ESM_YES;
				stInfo.astPostionInfo[byPosIdx].byNeedLevel = 0;
				stInfo.byFormationLevel = 1;
			}
			else
			{
				stInfo.astPostionInfo[byPosIdx].wHeroID = 0;
				stInfo.astPostionInfo[byPosIdx].byPostionIndex = propItr->second.byOpenIdx;
				stInfo.astPostionInfo[byPosIdx].byIsUnlock = ( propItr->second.byUnlockLevel <= m_poOwner->GetLevel() ? ESM_YES : ESM_NO);
				stInfo.astPostionInfo[byPosIdx].byNeedLevel = propItr->second.byUnlockLevel;
				stInfo.byFormationLevel = 1;
			}
			byPosIdx++;
		}
	}
	
}

VOID CFormation::AddUnLockFormation( DT_NEW_FORMATION_INFO& stInfo, UINT8 bySelectFormationID)
{
	if ( stInfo.byFormationID < BAIHU_FORMATION_KIND || ZHUQUE_FORMATION_KIND < stInfo.byFormationID )//阵法过滤
	{
		return ;
	}
	C88MapItr itr = m_mapUnLockPos.find(stInfo.byFormationID);
	if ( itr != m_mapUnLockPos.end())
	{
		return ;
	}
	if ( bySelectFormationID == stInfo.byFormationID)
	{
		m_bySelectFormationIdx = stInfo.byFormationID - 1;
	}
	m_mapUnLockFormationInfo[stInfo.byFormationID - 1] = stInfo;
	m_mapUnLockPos[stInfo.byFormationID] = stInfo.byFormationID - 1;
}

VOID CFormation::AddLockFormation( DT_NEW_FORMATION_INFO& stInfo)
{
	UINT8 byIdx = m_mapLockFormationInfo.size();
	for ( UINT8 i = 0; i < m_mapLockFormationInfo.size(); i++)
	{
		if ( m_mapLockFormationInfo.end() == m_mapLockFormationInfo.find(i))
		{
			byIdx = i;
			break;
		}
	}
	C88MapItr itr = m_mapLockPos.find(stInfo.byFormationID);
	if ( itr != m_mapLockPos.end())
	{
		return ;
	}
	m_mapLockFormationInfo[ byIdx ] = stInfo;
	m_mapLockPos[stInfo.byFormationID] = byIdx;
}

BOOL CFormation::OldToNewFormation()
{
	for( UINT8 byIdx = 0; byIdx < m_stDT_FORMATION_DATA.stFormationList.byFormationKindNum; byIdx++)
	{
		DT_FORMATION_IDX_DATA& stIdxData = m_stDT_FORMATION_DATA.stFormationList.astFormationInfo[byIdx];
		C88MapItr posItr = m_mapUnLockPos.find(stIdxData.byFormationKind);
		if ( posItr == m_mapUnLockPos.end())
		{
			//不可能出现找不到阵型
			return FALSE;
		}
		DT_NEW_FORMATION_INFO& stNewInfo = m_mapUnLockFormationInfo[posItr->second];
		UINT8 byPos = 0;
		for ( ; byPos < stIdxData.byOpenIdxNum && byPos < MAX_FORMATION_IDX_NUM; byPos++)
		{
			CFormationMgr::Instance()->SetIdxHeroID(stNewInfo, stIdxData.astOpenIdxInfo[byPos].wHeroID, stIdxData.astOpenIdxInfo[byPos].byFormationIdx);
		}
	}
	
	return TRUE;
}
BOOL CFormation::NewToOldFormation()
{
	CFormationKindMap& mapFormationInfo = CFormationPropMgr::Instance()->GetFormationKindMap();
	for( UINT8 byIdx = 0; byIdx < mapFormationInfo.size(); byIdx++)
	{
		DT_FORMATION_IDX_DATA& stIdxData = m_stDT_FORMATION_DATA.stFormationList.astFormationInfo[byIdx];
		C88MapItr posItr = m_mapUnLockPos.find(stIdxData.byFormationKind);
		if (0 != stIdxData.byFormationKind )
		{
			if ( posItr == m_mapUnLockPos.end())
			{
				//不可能出现找不到阵型
				return FALSE;
			}
			DT_NEW_FORMATION_INFO& stNewInfo = m_mapUnLockFormationInfo[posItr->second];
			
			CFormationMgr::Instance()->SetIdxHeroID(stIdxData, stNewInfo, m_poOwner->GetCoachHeroKindID());
		}
		else
		{
			DT_NEW_FORMATION_INFO& stNewInfo = m_mapUnLockFormationInfo[byIdx];
			CFormationMgr::Instance()->SetIdxHeroID(stIdxData, stNewInfo, m_poOwner->GetCoachHeroKindID());
		}
		

	}
	m_stDT_FORMATION_DATA.stFormationList.byFormationKindNum = mapFormationInfo.size();

	UINT8 byFormationKindID = 0;
	SHeroProp* pstHeroProp  = CHeroPropMgr::Instance()->GetHeroProp(m_poOwner->GetCoachHeroKindID());
	if(NULL == pstHeroProp)
	{
		return FALSE;
	}
	switch(pstHeroProp->byCareerID)
	{
	case ECK_ATTACK:
		byFormationKindID = ATTACK_FORMATION_KIND;
		break;
	case ECK_DEFENCE:
		byFormationKindID = DEFENCE_FORMATION_KIND;
		break;
	default:
		byFormationKindID = ATTACK_FORMATION_KIND;
		break;
	}
	//如果朱雀又职业为准
	if ( EBAK_ZHUQUE == m_mapUnLockFormationInfo[m_bySelectFormationIdx].byFormationID )
	{

		///////////////////////////////旧版阵型///////////////////////////////////////////
		m_stDT_FORMATION_DATA.bySelectIdx = byFormationKindID - 1;
	}
	else
	{
		m_stDT_FORMATION_DATA.bySelectIdx = m_bySelectFormationIdx > mapFormationInfo.size() ? byFormationKindID - 1 : m_bySelectFormationIdx;
	}
	return TRUE;
}

BOOL CFormation::IsUnlock(UINT8 byFormationID, UINT8 byIdx)
{
	C88MapItr itr = m_mapUnLockPos.find(byFormationID);
	if ( itr == m_mapUnLockPos.end())
	{
		return FALSE;
	}

	DT_NEW_FORMATION_INFO& stInfo = m_mapUnLockFormationInfo[itr->second];
	for ( UINT8 byIndex = 0; byIndex < stInfo.byPostionNum && byIndex < MAX_FORMATION_IDX_NUM; byIndex++)
	{
		if ( stInfo.astPostionInfo[byIndex].byPostionIndex == byIdx)
		{
			return stInfo.astPostionInfo[byIndex].byIsUnlock;
		}
	}
	return FALSE;
}

VOID CFormation::UnLockPos(DT_NEW_FORMATION_INFO& stInfo)
{
	for(UINT8 byIdx = 0; byIdx < stInfo.byPostionNum; byIdx++)
	{
		if( stInfo.astPostionInfo[byIdx].byNeedLevel <= stInfo.byFormationLevel)
		{
			stInfo.astPostionInfo[byIdx].byIsUnlock = ESM_YES;
		}
	}
}

BOOL CFormation::OldEmbattleOnFormation(DT_FORMATION_DATA& stFormationInfo)
{
	if(stFormationInfo.bySelectIdx >= m_stDT_FORMATION_DATA.stFormationList.byFormationKindNum)
	{
		m_stDT_FORMATION_DATA.bySelectIdx = 0;
	}
	memcpy(&m_stDT_FORMATION_DATA.stFormationList, &stFormationInfo.stFormationList, sizeof(DT_FORMATION_DATA_LIST));
	m_stDT_FORMATION_DATA.bySelectIdx = stFormationInfo.bySelectIdx;
	DT_FORMATION_DATA_LIST& stFormationList = m_stDT_FORMATION_DATA.stFormationList;

	UINT8 byFormationKindNum = m_stDT_FORMATION_DATA.stFormationList.byFormationKindNum;
	//保存阵型类型对应的下标
	map<UINT8, UINT8> mapHaveFormation;
	for(UINT8 byIdx = 0; byIdx < byFormationKindNum; byIdx++)
	{
		DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[byIdx];
		mapHaveFormation[stDT_FORMATION_IDX_DATA.byFormationKind] = byIdx;
	}

	UINT16 wCoachHeroID = m_poOwner->GetCoachHero()->GetDT_HERO_BASE_DATA().wKindID;
	//加新整型
	CFormationKindMap& mapFormationInfo = CFormationPropMgr::Instance()->GetFormationKindMap();
	//阵型数据无效，则去除
	if(mapHaveFormation.size() > mapFormationInfo.size())
	{
		memset(&m_stDT_FORMATION_DATA, 0, sizeof(m_stDT_FORMATION_DATA));
		mapHaveFormation.clear();
	}
	for(CFormationKindMapItr itr = mapFormationInfo.begin(); ((itr != mapFormationInfo.end()) && (stFormationList.byFormationKindNum < MAX_FORMATION_NUM)); itr++)
	{
		map<UINT8, UINT8>::iterator itrIdx = mapHaveFormation.find(itr->first);
		if(itrIdx != mapHaveFormation.end())
		{
			BOOL bHaveCoach = FALSE;

			//通过之前保存的阵型对应的下标信息获取相关属性值
			DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[itrIdx->second];

			//检测是否存在主武将在阵上。若不在镇上，完了，不知道出啥问题了。
			for(UINT8 byOpenIdx = 0; byOpenIdx < stDT_FORMATION_IDX_DATA.byOpenIdxNum; byOpenIdx++)
			{
				DT_FORMATION_HERO_DATA& stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdx];
				if(0 == stDT_FORMATION_HERO_DATA.wHeroID)
				{
					continue;
				}
				if(wCoachHeroID == stDT_FORMATION_HERO_DATA.wHeroID)
				{
					bHaveCoach = TRUE;
					break;
				}
			}
			//每一个阵型都必须有主角，而且重新始初化该阵型
			if(bHaveCoach)
			{
				continue;
			}
			memcpy(&stDT_FORMATION_IDX_DATA, &(itr->second.GetDT_FORMATION_IDX_DATA()), sizeof(DT_FORMATION_IDX_DATA));
			stDT_FORMATION_IDX_DATA.astOpenIdxInfo[itr->second.byCoachFormationLstIdx].wHeroID = wCoachHeroID;

			DBG_INFO(_SDT("[%s: %d]: PlayerID[%u] Formation init Faild!"), MSG_MARK, m_poOwner->GetID());
		}
		else
		{
			DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationList.astFormationInfo[stFormationList.byFormationKindNum];
			memcpy(&stDT_FORMATION_IDX_DATA, &(itr->second.GetDT_FORMATION_IDX_DATA()), sizeof(DT_FORMATION_IDX_DATA));
			stDT_FORMATION_IDX_DATA.astOpenIdxInfo[itr->second.byCoachFormationLstIdx].wHeroID = wCoachHeroID; //
			stFormationList.byFormationKindNum++;
		}
	}

	return Embattle();
}

VOID CFormation::CheckFormation()
{
	if( !CFormationMgr::Instance()->CheckConvertFormation(m_poOwner) )
	{
		if ( ESM_YES == m_stDT_FORMATION_DATA.byIsUseNewFormation )
		{
			NewToOldFormation();
			m_stDT_FORMATION_DATA.byIsUseNewFormation = ESM_NO;
			Embattle();
		}
	}
	else
	{
		if ( ESM_YES != m_stDT_FORMATION_DATA.byIsUseNewFormation)
		{
			UpdateFormation();
		}
	}
	
}

VOID CFormation::GetSimpleFormation(DT_GVG_FORMATION_HERO_LST_INFO& stFront, DT_GVG_FORMATION_HERO_LST_INFO& stBack)
{
	CFormationHeroShmemMap& mapHero = m_poOwner->GetFormationHeroMap();
	UINT8 byIdx = 1;
	UINT8 byHeroNum = 0;
	for ( ; byIdx <= 3; byIdx++)
	{
		CHero* poHero = mapHero.GetData(byIdx);
		if ( NULL != poHero)
		{
			stFront.adwHeroID[byHeroNum] = poHero->GetDT_HERO_BASE_DATA().wKindID;
			stFront.abyHeroCareerID[byHeroNum] = ECK_ATTACK;//默认值
			SHeroProp* poProp = CHeroPropMgr::Instance()->GetHeroProp(poHero->GetDT_HERO_BASE_DATA().wKindID);
			if ( NULL != poHero)
			{
				stFront.abyHeroCareerID[byHeroNum] = poProp->byCareerID;
			}
			byHeroNum++;
		}
	}
	stFront.byHeroNum = byHeroNum;

	byIdx = 4;
	byHeroNum = 0;
	for ( ; byIdx <= 6; byIdx++)
	{
		CHero* poHero = mapHero.GetData(byIdx);
		if ( NULL != poHero)
		{
			stBack.adwHeroID[byHeroNum] = poHero->GetDT_HERO_BASE_DATA().wKindID;
			stBack.abyHeroCareerID[byHeroNum] = ECK_ATTACK;
			SHeroProp* poProp = CHeroPropMgr::Instance()->GetHeroProp(poHero->GetDT_HERO_BASE_DATA().wKindID);
			if ( NULL != poHero)
			{
				stBack.abyHeroCareerID[byHeroNum] = poProp->byCareerID;
			}
			byHeroNum++;
		}
	}
	stBack.byHeroNum = byHeroNum;
}