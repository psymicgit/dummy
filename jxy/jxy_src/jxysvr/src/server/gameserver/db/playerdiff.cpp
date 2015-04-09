#include "playerdiff.h"

#include "dbmgr.h"
#include "logic/base/logicfunc.h"
#include <dll/sdframework/sdloggerimpl.h>

IMPLEMENT_CLASS_POOL_MGR(CPlayerDiff)
IMPLEMENT_CLASS_POOL_MGR(CFactionDiff)

CPlayerDiff::CPlayerDiff()
{
    memset(&m_stOld, 0, sizeof(DT_PLAYER_NOPVP_DATA));
    memset(&m_stCur, 0, sizeof(DT_PLAYER_NOPVP_DATA));
    m_mapOldBuild.clear();
    m_mapOldHero.clear();
	m_mapOldHeroJewelry.clear();
    m_mapOldInstance.clear();
    m_mapOldTask.clear();
    m_mapCurBuild.clear();
    m_mapCurHero.clear();
    m_mapCurInstance.clear();
    m_mapCurTask.clear();
	m_mapCurHeroJewelry.clear();

	m_mapOldHeroSoul.clear();
	m_mapCurHeroSoul.clear();
}


CPlayerDiff::~CPlayerDiff()
{
    m_mapOldBuild.clear();
    m_mapOldHero.clear();
    m_mapOldInstance.clear();
    m_mapOldTask.clear();
    m_mapCurBuild.clear();
    m_mapCurHero.clear();
    m_mapCurInstance.clear();
    m_mapCurTask.clear();
	m_mapCurHeroJewelry.clear();
	m_mapOldHeroJewelry.clear();
}

VOID CPlayerDiff::SetStartData(const DT_PLAYER_NOPVP_DATA& stPlayerData)
{
    memcpy(&m_stOld, &stPlayerData, sizeof(DT_PLAYER_NOPVP_DATA));

    ForamtDataMap(m_stOld, TRUE);
}

VOID CPlayerDiff::StartDiff(const DT_PLAYER_NOPVP_DATA& stPlayerData)
{
    memcpy(&m_stCur, &stPlayerData, sizeof(DT_PLAYER_NOPVP_DATA));

    ForamtDataMap(m_stCur, FALSE);
}

VOID CPlayerDiff::EndDiff()
{
    memcpy(&m_stOld, &m_stCur, sizeof(DT_PLAYER_NOPVP_DATA));
    ForamtDataMap(m_stOld, TRUE);
}

BOOL CPlayerDiff::HaveChanged(UINT8 eTableType, UINT16 wKey1, UINT16 wKey2, UINT16 wKey3)
{
    switch(eTableType)
    {
    case EDT_PlayerBase:
    {
        return (0 != memcmp(&m_stOld.stBaseData, &m_stCur.stBaseData, sizeof(DT_PLAYER_BASE_DATA)));
    }
    break;
    case EDT_PlayerExt:
    {
        return CkPlayerExt(wKey1);
    }
    break;
    case EDT_PlayerBag:
    {
        return ((0 != memcmp(&m_stOld.stBagEquipData, &m_stCur.stBagEquipData, sizeof(DT_BAG_EQUIP_DATA_LST))) ||
                (0 != memcmp(&m_stOld.stBagGoodsData, &m_stCur.stBagGoodsData, sizeof(DT_BAG_GOODS_DATA_LST))) ||
				(0 != memcmp(&m_stOld.stBagJewelryInfo, &m_stCur.stBagJewelryInfo, sizeof(DT_BAG_JEWELRY_DATA_LST)))
				);
    }
    break;
    case EDT_PlayerBuildAll:
    {
        return (0 != memcmp(&m_stOld.stBuildData, &m_stCur.stBuildData, sizeof(DT_BUILD_DATA_LIST)));
    }
    break;
    case EDT_PlayerBuild:
    {
        return CkBuild((UINT8)wKey1);
    }
    break;
    case EDT_PlayerHeroAll:
    {
        return (0 != memcmp(&m_stOld.stHeroData, &m_stCur.stHeroData, sizeof(DT_HERO_DATA_LIST)));
    }
    break;
    case EDT_PlayerHeroBase:
    {
        return CkHeroBase(wKey1);
    }
    break;
    case EDT_PlayerHeroExt:
    {
        return CkHeroExt(wKey1, wKey2);
    }
    break;
    case EDT_PlayerInstanceAll:
    {
        return ((0 != memcmp(&m_stOld.stInstanceData.stCommonInstanceData, &m_stCur.stInstanceData.stCommonInstanceData, sizeof(DT_INSTANCE_DATA_LIST))) ||
                (0 != memcmp(&m_stOld.stInstanceData.stEliteInstanceData, &m_stCur.stInstanceData.stEliteInstanceData, sizeof(DT_INSTANCE_DATA_LIST))) ||
                (0 != memcmp(&m_stOld.stInstanceData.stClimbTowerInstanceData, &m_stCur.stInstanceData.stClimbTowerInstanceData, sizeof(DT_INSTANCE_DATA_LIST))) ||
				( 0 != memcmp(&m_stOld.stInstanceData.stOutBoundInstanceData, &m_stCur.stInstanceData.stOutBoundInstanceData, sizeof(DT_INSTANCE_DATA_LIST)))
				);
    }
    break;
    case EDT_PlayerInstance:
    {
        return CkInstance(wKey1, (UINT8)wKey2, (UINT8)wKey3);
    }
    break;
    case EDT_PlayerTaskAll:
    {
        return (0 != memcmp(&m_stOld.stTaskData, &m_stCur.stTaskData, sizeof(DT_TASK_DATA_LIST)));
    }
    break;
    case EDT_PlayerTask:
    {
        return CkTask((UINT8)wKey1);
    }
    break;
    default:
        return FALSE;
        break;
    }
}


VOID CPlayerDiff::ForamtDataMap(DT_PLAYER_NOPVP_DATA& stPlayerData, BOOL bOld)
{
    if(bOld)
    {

        m_mapOldHero.clear();
        m_mapOldInstance.clear();
        m_mapOldTask.clear();
		m_mapOldHeroJewelry.clear();
		m_mapOldHeroSoul.clear();
    }
    else
    {
        m_mapCurBuild.clear();
        m_mapCurHero.clear();
        m_mapCurInstance.clear();
        m_mapCurTask.clear();
		m_mapCurHeroJewelry.clear();
		m_mapCurHeroSoul.clear();
    }

    DT_BUILD_DATA_LIST& stBuildData = stPlayerData.stBuildData;
    for(UINT16 wIdx = 0; wIdx < stBuildData.wBuildNum && wIdx < MAX_BUILD_NUM; wIdx++)
    {
        DT_BUILD_BASE_DATA* pstData = &stBuildData.astBuildInfoLst[wIdx];
        if(bOld)
        {
            m_mapOldBuild.insert(make_pair(pstData->byKindID, pstData));
        }
        else
        {
            m_mapCurBuild.insert(make_pair(pstData->byKindID, pstData));
        }
    }

    DT_HERO_DATA_LIST& stHeroData = stPlayerData.stHeroData;
    for(UINT16 wIdx = 0; wIdx < stHeroData.wHeroNum && wIdx < MAX_HERO_NUM; wIdx++)
    {
        DT_HERO_DATA* pstData = &stHeroData.astHeroInfoLst[wIdx];
		DT_JEWELRY_DATA_LST* pstJewelryData = &stHeroData.astjewelryInfoLst[wIdx];
		DT_HERO_SOUL_DATA* pstSoulData = &stHeroData.astSoulInfoLst[wIdx];
        if(bOld)
        {
            m_mapOldHero.insert(make_pair(pstData->stHeroBaseData.wKindID, pstData));
			m_mapOldHeroJewelry.insert( make_pair(pstData->stHeroBaseData.wKindID, pstJewelryData));
			m_mapOldHeroSoul.insert(make_pair(pstData->stHeroBaseData.wKindID, pstSoulData));
        }
        else
        {
            m_mapCurHero.insert(make_pair(pstData->stHeroBaseData.wKindID, pstData));
			m_mapCurHeroJewelry.insert( make_pair(pstData->stHeroBaseData.wKindID, pstJewelryData));
			m_mapCurHeroSoul.insert(make_pair(pstData->stHeroBaseData.wKindID, pstSoulData));
        }
    }

    DT_INSTANCE_DATA_LIST& stCommonInstanceData = stPlayerData.stInstanceData.stCommonInstanceData;
    for(UINT16 wIdx = 0; wIdx < stCommonInstanceData.wInstanceNum && wIdx < MAX_INSTANCE_PER_SCENE_NUM; wIdx++)
    {
        DT_INSTANCE_DATA* pstData = &stCommonInstanceData.astInstanceInfo[wIdx];
        if(bOld)
        {
            m_mapOldInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
        }
        else
        {
            m_mapCurInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
        }
    }
    DT_INSTANCE_DATA_LIST& stEliteInstanceData = stPlayerData.stInstanceData.stEliteInstanceData;
    for(UINT16 wIdx = 0; wIdx < stEliteInstanceData.wInstanceNum && wIdx < MAX_INSTANCE_PER_SCENE_NUM; wIdx++)
    {
        DT_INSTANCE_DATA* pstData = &stEliteInstanceData.astInstanceInfo[wIdx];
        if(bOld)
        {
            m_mapOldInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
        }
        else
        {
            m_mapCurInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
        }
    }
    DT_INSTANCE_DATA_LIST& stClimbTowerInstanceData = stPlayerData.stInstanceData.stClimbTowerInstanceData;
    for(UINT16 wIdx = 0; wIdx < stClimbTowerInstanceData.wInstanceNum && wIdx < MAX_INSTANCE_PER_SCENE_NUM; wIdx++)
    {
        DT_INSTANCE_DATA* pstData = &stClimbTowerInstanceData.astInstanceInfo[wIdx];
        if(bOld)
        {
            m_mapOldInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
        }
        else
        {
            m_mapCurInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
        }
    }

	DT_INSTANCE_DATA_LIST& stOutBoundInstanceData = stPlayerData.stInstanceData.stOutBoundInstanceData;
	for(UINT16 wIdx = 0; wIdx < stOutBoundInstanceData.wInstanceNum && wIdx < MAX_INSTANCE_PER_SCENE_NUM; wIdx++)
	{
		DT_INSTANCE_DATA* pstData = &stOutBoundInstanceData.astInstanceInfo[wIdx];
		if(bOld)
		{
			m_mapOldInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
		}
		else
		{
			m_mapCurInstance.insert(make_pair(CLogicFunc::Get1688Key(pstData->stInstanceID.wSceneIdx, pstData->stInstanceID.byTownIdx, pstData->stInstanceID.byInstanceIdx), pstData));
		}
	}

    DT_TASK_DATA_LIST& stTaskData = stPlayerData.stTaskData;
    for(UINT16 wIdx = 0; wIdx < stTaskData.byTaskNum && wIdx < MAX_TASK_LINE_NUM; wIdx++)
    {
        DT_TASK_DATA* pstData = &stTaskData.astTaskInfoList[wIdx];
        if(bOld)
        {
            m_mapOldTask.insert(make_pair(pstData->byTaskLine, pstData));
        }
        else
        {
            m_mapCurTask.insert(make_pair(pstData->byTaskLine, pstData));
        }
    }

}

BOOL CPlayerDiff::CkPlayerExt(UINT16 wKey)
{
    switch(wKey)
    {
    case EMDT_LocaleData:
    {
        return (0 != memcmp(&m_stOld.stLocaleData, &m_stCur.stLocaleData, sizeof(m_stCur.stLocaleData)));
    }
    break;
    case EMDT_CurState:
    {
        return (0 != memcmp(&m_stOld.stCurState, &m_stCur.stCurState, sizeof(m_stCur.stCurState)));
    }
    break;
    case EMDT_OnlineEncRecdData:
    {
        return (0 != memcmp(&m_stOld.stOnlineEncRecdData, &m_stCur.stOnlineEncRecdData, sizeof(m_stCur.stOnlineEncRecdData)));
    }
    break;
    case EMDT_LoginEncRecdData:
    {
        return (0 != memcmp(&m_stOld.stLoginEncRecdData, &m_stCur.stLoginEncRecdData, sizeof(m_stCur.stLoginEncRecdData)));
    }
    break;
    case EMDT_MoneyTreeData:
    {
        return (0 != memcmp(&m_stOld.stMoneyTreeData, &m_stCur.stMoneyTreeData, sizeof(m_stCur.stMoneyTreeData)));
    }
    break;
    case EMDT_CommonBattleRcd:
    {
        return (0 != memcmp(&m_stOld.stInstanceData.stCommonBattleRcd, &m_stCur.stInstanceData.stCommonBattleRcd, sizeof(m_stCur.stInstanceData.stCommonBattleRcd)));
    }
    break;
    case EMDT_EliteBattleRcd:
    {
        return (0 != memcmp(&m_stOld.stInstanceData.stEliteBattleRcd, &m_stCur.stInstanceData.stEliteBattleRcd, sizeof(m_stCur.stInstanceData.stEliteBattleRcd)));
    }
    break;
    case EMDT_JingJieData:
    {
        return (0 != memcmp(&m_stOld.stJingJieData, &m_stCur.stJingJieData, sizeof(m_stCur.stJingJieData)));
    }
    break;
    case EMDT_PhystrengthData:
    {
        return (0 != memcmp(&m_stOld.stPhystrengthData, &m_stCur.stPhystrengthData, sizeof(m_stCur.stPhystrengthData)));
    }
    break;
    case EMDT_FewParamData:
    {
        return (0 != memcmp(&m_stOld.stFewParamData, &m_stCur.stFewParamData, sizeof(m_stCur.stFewParamData)));
    }
    break;
    case EMDT_CropData:
    {
        return (0 != memcmp(&m_stOld.stCropData, &m_stCur.stCropData, sizeof(m_stCur.stCropData)));
    }
    break;
    case EMDT_ScienceTree:
    {
        return (0 != memcmp(&m_stOld.stScienceTree, &m_stCur.stScienceTree, sizeof(m_stCur.stScienceTree)));
    }
    break;
    case EMDT_FormationInfo:
    {
        return (0 != memcmp(&m_stOld.stFormationInfo, &m_stCur.stFormationInfo, sizeof(m_stCur.stFormationInfo)));
    }
    break;
    case EMDT_EatBreadInfo:
    {
        return (0 != memcmp(&m_stOld.stEatBreadInfo, &m_stCur.stEatBreadInfo, sizeof(m_stCur.stEatBreadInfo)));
    }
    break;
    case EMDT_YabiaoInfo:
    {
        return (0 != memcmp(&m_stOld.stYabiaoInfo.stYabiaoBaseInfo, &m_stCur.stYabiaoInfo.stYabiaoBaseInfo, sizeof(m_stCur.stYabiaoInfo.stYabiaoBaseInfo)));
    }
    break;
    case EMDT_ClimbTowerBattleRcd:
    {
        return (0 != memcmp(&m_stOld.stInstanceData.stClimbTowerBattleRcd, &m_stCur.stInstanceData.stClimbTowerBattleRcd, sizeof(m_stCur.stInstanceData.stClimbTowerBattleRcd)));
    }
    break;
    case EMDT_YabiaoBattleLog:
    {
        return (0 != memcmp(&m_stOld.stYabiaoInfo.stYabiaoBattleLog, &m_stCur.stYabiaoInfo.stYabiaoBattleLog, sizeof(m_stCur.stYabiaoInfo.stYabiaoBattleLog)));
    }
    break;
    case EMDT_ClimbTowerData:
    {
        return (0 != memcmp(&m_stOld.stClimbTowerData, &m_stCur.stClimbTowerData, sizeof(m_stCur.stClimbTowerData)));
    }
    break;
    case EMDT_AwakenData:
    {
        return (0 != memcmp(&m_stOld.stAwakenData, &m_stCur.stAwakenData, sizeof(m_stCur.stAwakenData)));
    }
    break;
    case EMDT_VipExtData:
    {
        return (0 != memcmp(&m_stOld.stVipExtData, &m_stCur.stVipExtData, sizeof(m_stCur.stVipExtData)));
    }
    break;
    case EMDT_LuaActivityRecordData:
    {
        return (0 != memcmp(&m_stOld.stLuaActivityRecordData, &m_stCur.stLuaActivityRecordData, sizeof(m_stCur.stLuaActivityRecordData)));
    }
    break;
    case EMDT_GasCollectData:
    {
        return (0 != memcmp(&m_stOld.stGasCollectData, &m_stCur.stGasCollectData, sizeof(m_stCur.stGasCollectData)));
    }
    break;
    case EMDT_BossBattleData:
    {
        return (0 != memcmp(&m_stOld.stBossBattlePlayerDataInfo.stBossBattleData, &m_stCur.stBossBattlePlayerDataInfo.stBossBattleData, sizeof(m_stCur.stBossBattlePlayerDataInfo.stBossBattleData)));
    }
    break;
    case EMDT_ACTIVITY_ENCOURAGE:
    {
        return (0 != memcmp(&m_stOld.stActivityEncourageData, &m_stCur.stActivityEncourageData, sizeof(m_stCur.stActivityEncourageData)));
    }
    break;
    case EMDT_BossBBL:
    {
        return (0 != memcmp(&m_stOld.stBossBattlePlayerDataInfo.stBossBBLData, &m_stCur.stBossBattlePlayerDataInfo.stBossBBLData, sizeof(m_stCur.stBossBattlePlayerDataInfo.stBossBBLData)));
    }
    break;
    case EMDT_RdChallengeData:
    {
        return (0 != memcmp(&m_stOld.stRdChallengeData, &m_stCur.stRdChallengeData, sizeof(m_stCur.stRdChallengeData)));
    }
    break;
    case EMDT_WorshipData:
    {
        return (0 != memcmp(&m_stOld.stWorshipData, &m_stCur.stWorshipData, sizeof(m_stCur.stWorshipData)));
    }
    break;
    case EMDT_ScoreData:
    {
        return (0 != memcmp(&m_stOld.stScoreData, &m_stCur.stScoreData, sizeof(m_stCur.stScoreData)));
    }
    break;
    case EMDT_EquipComposeData:
    {
        return (0 != memcmp(&m_stOld.stEquipComposeData, &m_stCur.stEquipComposeData, sizeof(m_stCur.stEquipComposeData)));
    }
    break;
    case EMDT_GatherScienceData:
    {
        return (0 != memcmp(&m_stOld.stGatherScienceData, &m_stCur.stGatherScienceData, sizeof(m_stCur.stGatherScienceData)));
    }
    break;
    case EMDT_BegBlueData:
    {
        return (0 != memcmp(&m_stOld.stBegBlueData, &m_stCur.stBegBlueData, sizeof(m_stCur.stBegBlueData)));
    }
    break;
    case EMDT_FriendsBaseData:
    {
        return (0 != memcmp(&m_stOld.stFriendsData.stFriendsBaseData, &m_stCur.stFriendsData.stFriendsBaseData, sizeof(m_stCur.stFriendsData.stFriendsBaseData)));
    }
    break;
    case EMDT_Courage:
    {
        return (0 != memcmp(&m_stOld.stCourageData, &m_stCur.stCourageData, sizeof(m_stCur.stCourageData)));
    }
    break;
    case EMDT_PlayerFactionData:
    {
        return (0 != memcmp(&m_stOld.stPlayerFactionData, &m_stCur.stPlayerFactionData, sizeof(m_stCur.stPlayerFactionData)));
    }
    break;
    case EMDT_EquipMaster:
    {
        return (0 != memcmp(&m_stOld.stEquipMasterData, &m_stCur.stEquipMasterData, sizeof(m_stCur.stEquipMasterData)));
    }
    break;
    case EMDT_ExchangeResultLog:
    {
        return (0 != memcmp(&m_stOld.stExchangeResultLog, &m_stCur.stExchangeResultLog, sizeof(m_stCur.stExchangeResultLog)));
    }
    break;
	case EMDT_GVGData:
	{
		return ( 0 != memcmp(&m_stOld.stGvGData, &m_stCur.stGvGData, sizeof(m_stCur.stGvGData)));
	}
	break;
	case EMDT_SoulData:
	{
		return ( 0 != memcmp(&m_stOld.stSoulData, &m_stCur.stSoulData, sizeof(m_stCur.stSoulData)) );
	}
	break;
	case EMDT_MedalData:
	{
		return ( 0 != memcmp(&m_stOld.stMedalData, &m_stCur.stMedalData, sizeof(m_stCur.stMedalData) ));
	}
	break;
	case EMDT_PetData: {
		return ( 0 != memcmp(&m_stOld.stPetData, &m_stCur.stPetData, sizeof(m_stCur.stPetData) ));
	}
	break;
    default:
    {
        SYS_CRITICAL( _SDT("CkPlayerExt key[%d] invalid"), wKey );
        break;
    }
    }
    return FALSE;

}

BOOL CPlayerDiff::CkBuild(UINT8 byKey)
{
    map<UINT8, DT_BUILD_BASE_DATA*>::iterator itrOld = m_mapOldBuild.find(byKey);
    if(itrOld == m_mapOldBuild.end()) //新增
    {
        return TRUE;
    }

    map<UINT8, DT_BUILD_BASE_DATA*>::iterator itrCur = m_mapCurBuild.find(byKey);
    if(itrCur == m_mapCurBuild.end())  //删除(不应该走到这)，不更新数据
    {
        return FALSE;
    }

    return (0 != memcmp(itrOld->second, itrCur->second, sizeof(DT_BUILD_BASE_DATA)));
}

BOOL CPlayerDiff::CkHeroBase(UINT16 wKey)
{
    map<UINT16, DT_HERO_DATA*>::iterator itrOld = m_mapOldHero.find(wKey);
    if(itrOld == m_mapOldHero.end()) //新增
    {
        return TRUE;
    }

    map<UINT16, DT_HERO_DATA*>::iterator itrCur = m_mapCurHero.find(wKey);
    if(itrCur == m_mapCurHero.end()) //删除(不应该走到这)，不更新数据
    {
        return FALSE;
    }

    return (0 != memcmp(&itrOld->second->stHeroBaseData, &itrCur->second->stHeroBaseData, sizeof(DT_HERO_BASE_DATA)));
}

BOOL CPlayerDiff::CkHeroExt(UINT16 wKey1, UINT16 wKey2)
{
    map<UINT16, DT_HERO_DATA*>::iterator itrOld = m_mapOldHero.find(wKey1);
    if(itrOld == m_mapOldHero.end()) //新增
    {
        return TRUE;
    }

	map<UINT16, DT_JEWELRY_DATA_LST*>::iterator itrOldJewelry = m_mapOldHeroJewelry.find(wKey1);
	if ( itrOldJewelry == m_mapOldHeroJewelry.end())
	{
		return TRUE;
	}

	map<UINT16, DT_HERO_SOUL_DATA*>::iterator itrOldSoul = m_mapOldHeroSoul.find(wKey1);
	if ( itrOldSoul == m_mapOldHeroSoul.end())
	{
		return TRUE;
	}

    map<UINT16, DT_HERO_DATA*>::iterator itrCur = m_mapCurHero.find(wKey1);
    if(itrCur == m_mapCurHero.end()) //删除(不应该走到这)，不更新数据
    {
        return FALSE;
    }

	map<UINT16, DT_JEWELRY_DATA_LST*>::iterator itrCurJewelry = m_mapCurHeroJewelry.find(wKey1);
	if ( itrCurJewelry == m_mapCurHeroJewelry.end())
	{
		return FALSE;
	}

	map<UINT16, DT_HERO_SOUL_DATA*>::iterator itrCurSoul = m_mapCurHeroSoul.find(wKey1);
	if ( itrCurSoul == m_mapCurHeroSoul.end())
	{
		return FALSE;
	}

    switch(wKey2)
    {
    case EHEDT_Equip:
    {
        return (0 != memcmp(&itrOld->second->stEquipDataInfo, &itrCur->second->stEquipDataInfo, sizeof(DT_HERO_EQUIP_DATA_LST)));
    }
    break;
    case EHEDT_Drug:
    {
        return (0 != memcmp(&itrOld->second->stDrugDataInfo, &itrCur->second->stDrugDataInfo, sizeof(DT_DRUG_DATA_LST)));
    }
    break;
    case EHEDT_Godweapon:
    {
        return (0 != memcmp(&itrOld->second->stGodweaponInfo, &itrCur->second->stGodweaponInfo, sizeof(DT_GODWEAPON_DATA)));
    }
    break;
    case EHEDT_Skill:
    {
        return (0 != memcmp(&itrOld->second->stSkillInfo, &itrCur->second->stSkillInfo, sizeof(DT_SKILL_DATA)));
    }
    break;
    case EHEDT_Study:
    {
        return (0 != memcmp(&itrOld->second->stStudyInfo, &itrCur->second->stStudyInfo, sizeof(DT_AWAKEN_DATA_LST)));
    }
    break;
    case EHEDT_Absorb:
    {
        return (0 != memcmp(&itrOld->second->stAbsorbInfo, &itrCur->second->stAbsorbInfo, sizeof(DT_ABSORB_DATA_LST)));
    }
    break;
	case EHEDT_Jewelry:
	{
		return (0 != memcmp(itrOldJewelry->second, itrCurJewelry->second, sizeof(DT_JEWELRY_DATA_LST)));
	}
	break;
	case EHEDT_SOUL:
		{
			 return (0 != memcmp(itrOldSoul->second, itrCurSoul->second, sizeof(DT_HERO_SOUL_DATA)));
		}
		break;
    default:
        SYS_CRITICAL( _SDT("CkHeroExt key[%d] invalid"), wKey2 );
        return FALSE;
    }

    return FALSE;
}

BOOL CPlayerDiff::CkInstance(UINT16 wKey1, UINT8 byKey2, UINT8 byKey3)
{
    UINT32 dwKey = CLogicFunc::Get1688Key(wKey1, byKey2, byKey3);
    map<UINT32, DT_INSTANCE_DATA*>::iterator itrOld = m_mapOldInstance.find(dwKey);
    if(itrOld == m_mapOldInstance.end())//新增
    {
        return TRUE;
    }

    map<UINT32, DT_INSTANCE_DATA*>::iterator itrCur = m_mapCurInstance.find(dwKey);
    if(itrCur == m_mapCurInstance.end())//删除(不应该走到这)，不更新数据
    {
        return FALSE;
    }

    return (0 != memcmp(itrOld->second, itrCur->second, sizeof(DT_INSTANCE_DATA)));
}

BOOL CPlayerDiff::CkTask(UINT8 byKey)
{
    map<UINT8, DT_TASK_DATA*>::iterator itrOld = m_mapOldTask.find(byKey);
    if(itrOld == m_mapOldTask.end()) //新增
    {
        return TRUE;
    }

    map<UINT8, DT_TASK_DATA*>::iterator itrCur = m_mapCurTask.find(byKey);
    if(itrCur == m_mapCurTask.end())  //删除(不应该走到这)，不更新数据
    {
        return FALSE;
    }

    return (0 != memcmp(itrOld->second, itrCur->second, sizeof(DT_TASK_DATA)));
}





CFactionDiff::CFactionDiff()
{
    memset(&m_stDT_FACTION_DATA, 0x00, sizeof(m_stDT_FACTION_DATA));
}

CFactionDiff::~CFactionDiff()
{

}

BOOL CFactionDiff::HaveChanged(const DT_FACTION_DATA &stDT_FACTION_DATA)
{
    if (0 == memcmp(&m_stDT_FACTION_DATA, &stDT_FACTION_DATA, sizeof(DT_FACTION_DATA)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL CFactionDiff::Changed(const DT_FACTION_DATA &stDT_FACTION_DATA)
{
    memcpy(&m_stDT_FACTION_DATA, &stDT_FACTION_DATA, sizeof(DT_FACTION_DATA));
    return TRUE;
}

