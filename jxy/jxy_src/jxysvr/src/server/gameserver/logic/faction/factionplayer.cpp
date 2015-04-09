#include "factionplayer.h"
#include <logic/faction/factionmgr.h>
#include <logic/player/player.h>
#include <logic/other/locale.h>
#include "logic/lua/luamgr.h"
#include "logic/other/singleprammgr.h"

IMPLEMENT_CLASS_POOL_MGR(CFactionPlayer)
BOOL CFactionPlayer::Init(DT_PLAYER_FACTION_DATA& stData, CPlayer* poOwner)
{
	
	BOOL bRet = InitRareItem(stData.stRareItemGoodsData, poOwner);
	if ( !bRet )
	{
		return FALSE;
	}
	bRet = InitGymnasium(stData.stGymnasiumSkillData, poOwner);
	if ( !bRet )
	{
		return FALSE;
	}

	bRet = InitShade(stData.stShadeData, poOwner);
	if ( !bRet )
	{
		return FALSE;
	}

	bRet = InitHotSpring(stData.stHotSpringData, poOwner, stData.stHotSpringExt);
	if ( !bRet )
	{
		return FALSE;
	}

	bRet = InitFactionBattleLog(stData.stPlayerFactionLog, poOwner);
	if ( !bRet )
	{
		return FALSE;
	}

	bRet = InitPlayerMark( stData.stPlayerMark, poOwner);
	if ( !bRet )
	{
		return FALSE;
	}

	bRet = InitFactionDonate(stData.stPlayerDoNate, poOwner);
	if ( !bRet)
	{
		return FALSE;
	}

	m_dwFindBackDoorsTribute = stData.dwFindBackDoorsTribute;
	
	m_poOwner = poOwner;
	return TRUE;
}

BOOL CFactionPlayer::InitPlayerMark(DT_PLAYER_MARK_DATA& stPlayerMark, CPlayer* poOwner)
{
	if(!m_oPlayerMark.Init(stPlayerMark, poOwner))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CFactionPlayer::InitRareItem(DT_RAREITEM_GOODS_DATA &stDT_SHADE_DATA, CPlayer* poOwner)
{
	return m_oRareItem.Init(stDT_SHADE_DATA, poOwner);
}

BOOL CFactionPlayer::InitGymnasium(DT_GYMNASIUM_DATA &stDT_GYMNASIUM_SKILL_DATA, CPlayer* poOwner)
{
	return m_oGymnasium.Init(stDT_GYMNASIUM_SKILL_DATA, poOwner);
}

BOOL CFactionPlayer::InitShade(DT_SHADE_DATA &stDT_SHADE_DATA, CPlayer* poOwner)
{
	return m_oShade.Init(stDT_SHADE_DATA, poOwner);
}

BOOL CFactionPlayer::InitHotSpring(DT_HOT_SPRING_DATA &m_stHotSpringData, CPlayer* poOwner, DT_HOT_SPRING_EXT_DATA& stExt)
{
	return m_oHotSpring.Init(m_stHotSpringData, poOwner, stExt);
}

BOOL CFactionPlayer::InitFactionBattleLog(DT_PLAYER_FACTION_BATTLELOG_DATA_LIST& stPlayerFactionBattleLog, CPlayer* poOwner)
{
	return m_oFactionBattleLog.Init( stPlayerFactionBattleLog, poOwner);
}

BOOL CFactionPlayer::InitFactionDonate(DT_PLAYER_DONATE_INFO& stDonate, CPlayer* poOwer)
{
	return m_oDonate.Init( stDonate, poOwer);
}
/////////////////////////////珍品坊/////////////////////////////////////////////
DT_RAREITEM_GOODS_DATA& CFactionPlayer::GetDT_RAREITEM_GOODS_DATA()
{
	return m_oRareItem.GetDT_RAREITEM_GOODS_DATA();
}

UINT64 CFactionPlayer::GetRareItemUpdateTime()
{
	return m_oRareItem.GetUpdateTime();
}

VOID CFactionPlayer::SetRareItemUpdateTime(UINT64 qwUpdataTime)
{
	m_oRareItem.SetUpdateTime(qwUpdataTime);
}

CMapRareItemGoods& CFactionPlayer::GetRareItemMap()
{
	return m_oRareItem.GetRareItemMap();
}


/////////////////////////////武堂/////////////////////////////////////////////
DT_GYMNASIUM_DATA& CFactionPlayer::GetDT_GYMNASIUM_SKILL_DATA()
{
	return m_oGymnasium.GetDT_GYMNASIUM_SKILL_DATA();
}

CMapMySkill& CFactionPlayer::GetMySkillMap()
{
	return m_oGymnasium.GetMySkillMap();
}

UINT16 CFactionPlayer::OpenFactionSkill(PKT_CLIGS_OPEN_FACTION_SKILL_ACK& stAck)
{
	return m_oGymnasium.OpenFactionSkill(stAck);
}

/////////////////////////////暗部/////////////////////////////////////////////
DT_SHADE_DATA& CFactionPlayer::GetDT_SHADE_DATA()
{
	return m_oShade.GetDT_SHADE_DATA();
}


DT_PLAYER_MARK_DATA& CFactionPlayer::GetDT_PLAYER_MARK_DATA()
{
	return m_oPlayerMark.GetDT_PLAYER_MARK_DATA();
}

/////////////////////////////温泉/////////////////////////////////////////////////
DT_HOT_SPRING_DATA& CFactionPlayer::GetDT_HOT_SPRING_DATA()
{
	return m_oHotSpring.GetDT_HOT_SPRING_DATA();
}

DT_PLAYER_FACTION_BATTLELOG_DATA_LIST& CFactionPlayer::GetDT_BATTLE_LOG_LST()
{
	return m_oFactionBattleLog.GetDT_PLAYER_FACTION_BATTLELOG_DATA_LIST();
}

DT_PLAYER_FACTION_DATA& CFactionPlayer::GetDT_PLAYER_FACTION_DATA()
{
	memset(&m_stPlayerFaction, 0, sizeof(DT_PLAYER_FACTION_DATA));
	m_stPlayerFaction.byHaveDataFlag = 1;
	m_stPlayerFaction.stRareItemGoodsData = GetDT_RAREITEM_GOODS_DATA();
	m_stPlayerFaction.stGymnasiumSkillData = GetDT_GYMNASIUM_SKILL_DATA();
	m_stPlayerFaction.stShadeData = GetDT_SHADE_DATA();
	m_stPlayerFaction.stHotSpringData = GetDT_HOT_SPRING_DATA();
	m_stPlayerFaction.stPlayerFactionLog = GetDT_BATTLE_LOG_LST();
	m_stPlayerFaction.stPlayerMark = GetDT_PLAYER_MARK_DATA();
	m_stPlayerFaction.stPlayerDoNate = GetFactionDonate();
	m_stPlayerFaction.dwFindBackDoorsTribute = m_dwFindBackDoorsTribute;
	m_stPlayerFaction.stHotSpringExt = m_oHotSpring.GetDT_HOT_SPRING_EXT();
	return m_stPlayerFaction;
}

UINT32 CFactionPlayer::GetOpenRareItem(DT_RAREITEM_ROOM_INFO_CLI& stRareItemRoomInfo, DT_RAREITEM_INFO_LIST_CLI& stRareItemInfo)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oRareItem.GetOpenRareItem(stRareItemRoomInfo, stRareItemInfo);
}

UINT32 CFactionPlayer::ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oRareItem.ExchangeRareItem(stRareItemInfo, dwLeftDoorsTributes, dwGoodsID);
}

UINT32 CFactionPlayer::ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID, UINT8 byLeftTime)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oRareItem.ExchangeRareItem(stRareItemInfo, dwLeftDoorsTributes, dwGoodsID, byLeftTime);
}

UINT32 CFactionPlayer::OpenUpgradeRareItem(DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeCli)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oRareItem.OpenUpgradeRareItem(stUpgradeCli);
}

UINT32 CFactionPlayer::UpgradeRareItem(PKT_CLIGS_UPGRADE_RAREITEM_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oRareItem.UpgradeRareItem(stAck);
}


CFaction* CFactionPlayer::GetFaction()
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
	if ( !bRet )
	{
		return NULL;
	}
	return CFactionMgr::Instance()->GetFaction(dwFactionID);
}

UINT32 CFactionPlayer::GetFactionID()
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
	if ( !bRet )
	{
		return NULL;
	}
	return dwFactionID;
}


UINT16 CFactionPlayer::OpenGymnasium( PKT_CLIGS_OPEN_GYMNASIUM_ACK& stAck )
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oGymnasium.OpenGymnasium(stAck.stGymnasiumInfo, stAck.bySkillNum, stAck.astSkillInfo, stAck.adwSkillSumDoorsTribute);
}

UINT16 CFactionPlayer::LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oGymnasium.LearnGymnasiumSkill(stAck);
}

UINT16 CFactionPlayer::LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2& stAck, UINT8 byCurLevel )
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oGymnasium.LearnGymnasiumSkill(stAck, byCurLevel);
}

UINT16 CFactionPlayer::OpenGymnasiumUpgrade( DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo )
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oGymnasium.OpenGymnasiumUpgrade(stUpgradeRequireInfo);
}

UINT16 CFactionPlayer::GymnasiumUpgrade(PKT_CLIGS_GYMNASIUM_UPGRADE_ACK& stAck )
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oGymnasium.GymnasiumUpgrade(stAck);
}

UINT32 CFactionPlayer::GetGymnasiumAttrLevel(UINT8 byKindID)
{
	return m_oGymnasium.GetGymnasiumAttrLevel(byKindID);
}

VOID CFactionPlayer::WriteDownPosition(UINT16 wPosX, UINT16 wPosY)
{
	m_oHotSpring.WriteDownPosition(wPosX, wPosY);
}

UINT16 CFactionPlayer::OnOpenHotSpring(PKT_CLIGS_OPEN_HOT_SPRING_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.OnOpenHotSpring(stAck);
}

UINT16 CFactionPlayer::OnSelectWine(UINT32 dwWineID, UINT64& qwCoin, UINT64& qwGold, DT_WINE_DATA_CLI& stWineDataCli, UINT8& byState)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.OnSelectWine(dwWineID, qwCoin, qwGold, stWineDataCli, byState);
}

//移动记录
UINT16 CFactionPlayer::OnDrinkWineMove(UINT16 wPosX, UINT16 wPosY)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.OnDrinkWineMove(wPosX, wPosY);
}

VOID CFactionPlayer::LeaveHotSpring()
{
	m_oHotSpring.LeaveHotSpring();
}

UINT16 CFactionPlayer::GiveWine( DT_DIDNOT_GIVE_WINE_CLI& stGiveWineInfo)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.GiveWine(stGiveWineInfo);
}


UINT16 CFactionPlayer::ClickSelectWineGiveWine( UINT8 byDrinkWineState, DT_DIDNOT_DRINGK_WINE_LIST_CLI& stWineDataList)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.ClickSelectWineGiveWine(byDrinkWineState, stWineDataList);
}

UINT16 CFactionPlayer::SureGiveWine(UINT64& qwActiviTime)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.SureGiveWine(qwActiviTime);
}

UINT16 CFactionPlayer::OpenFindDrink()
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.OpenFindDrink();
}

VOID CFactionPlayer::WorkOutDrinkPassTime()
{
	m_oHotSpring.WorkOutDrinkPassTime();
}

VOID CFactionPlayer::GetDrinkReward(PKT_CLIGS_HOT_SPRING_REWARD_NTF* stNtf)
{
	m_oHotSpring.GetDrinkReward(stNtf);
}

VOID CFactionPlayer::AddDrinkEvent()
{
	m_oHotSpring.AddDrinkEvent();
}

VOID CFactionPlayer::AfterGiveWineProc()
{
	m_oHotSpring.AfterGiveWineProc();
}

UINT16 CFactionPlayer::OverRewardAck()
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oHotSpring.OverRewardAck();
}

UINT16 CFactionPlayer::OpenShade(PKT_CLIGS_OPEN_SHADE_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.OpenShade(stAck);
}

UINT16 CFactionPlayer::OpenGoldMode(DT_MONSTER_INFO& stMonsterInfo, UINT32& dwSyncGold)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.OpenGoldMode(stMonsterInfo, dwSyncGold);
}

UINT16 CFactionPlayer::OpenShadeUpgrade(DT_SHADE_UPGRADE_INFO& stShadeUpgradeInfo)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.OpenShadeUpgrade(stShadeUpgradeInfo);
}

UINT16 CFactionPlayer::ShadeUpgrade(PKT_CLIGS_SHADE_UPGRADE_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.ShadeUpgrade(stAck);
}

UINT16 CFactionPlayer::OpenHunting(PKT_CLIGS_OPEN_HUNTING_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.OpenHunting(stAck);
}

UINT16 CFactionPlayer::FindMonster(vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo )
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.FindMonster(vecMonsterOrange, stMonsterInfo);
}

UINT16 CFactionPlayer::ChangeMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed, 
	vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo, UINT32& dwGoldNum )
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.ChangeMonster(vecMonsterBlue, vecMonsterPurple, vecMonsterRed, vecMonsterOrange, stMonsterInfo, dwGoldNum);

}

UINT16 CFactionPlayer::OnHuntingBattle(PKT_CLIGS_HUNTING_BATTLE_ACK5& stAck )
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.OnHuntingBattle(stAck);
}

UINT16 CFactionPlayer::OnHuntingBattle2(PKT_CLIGS_HUNTING_BATTLE_ACK2& stAck )
{
    CFaction* poFaction = GetFaction();
    if ( NULL != poFaction)
    {
        BOOL bRet = poFaction->GetExceptionFlag();
        if(bRet)
        {
            return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
        }
    }
    return m_oShade.OnHuntingBattle2(stAck);
}
UINT16 CFactionPlayer::OnHuntingBattle3(PKT_CLIGS_HUNTING_BATTLE_ACK3& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.OnHuntingBattle3(stAck);
}
UINT16 CFactionPlayer::FirstFindMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed, 
	vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.FirstFindMonster(vecMonsterBlue, vecMonsterPurple, vecMonsterRed, vecMonsterOrange, stMonsterInfo);
}

UINT16 CFactionPlayer::ClosedMonster(UINT8 byChoice, PKT_CLIGS_CLOSED_MONSTER_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.ClosedMonster(byChoice, stAck);
}

VOID CFactionPlayer::OnChangeSchedule(PKT_CLIGS_CHANGE_SCHEDEULE_NTF& stChangeNtf)
{
	//return m_oShade.OnChangeSchedule(stChangeNtf);
}

UINT16 CFactionPlayer::GetHuntingIntegral(PKT_CLIGS_HUNTING_ITEGRAL_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
	return m_oShade.GetHuntingIntegral(stAck);
}

UINT32 CFactionPlayer::GetUnReadFactionBattleLogNum()
{
	return m_oFactionBattleLog.GetUnReadLogNum();
}

UINT16 CFactionPlayer::GetFactinBattleLogByCli(UINT8&byLogNum, DT_PLAYER_FACTION_BATTLELOG_DATA astPlayerFactionLogList[MAX_FACTION_BATTLE_LOG_NUM])
{
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		BOOL bRet = poFaction->GetExceptionFlag();
		if(bRet)
		{
			return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
		}
	}
    m_oFactionBattleLog.GetBattleLogByCli(byLogNum, astPlayerFactionLogList);
    return ERR_PLAYER_FACTION_READ::ID_SUCCESS;
}

VOID CFactionPlayer::AddFactionBattleLog(DT_PLAYER_FACTION_BATTLELOG_DATA& stData)
{
	m_oFactionBattleLog.AddFactionBattleLog(stData);
}

UINT16 CFactionPlayer::LeaveFaction()
{

	//清空门派相关数据
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ERR_LEAVE_FACTION::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
	
	DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_poOwner->GetID());

	DT_FACTION_LOG_DATA stFactionLogData;
	memset(&stFactionLogData, 0, sizeof(DT_FACTION_LOG_DATA));
	stFactionLogData.byAction = EFLID_LeaveFaction;
	stFactionLogData.dwPlayerID1 = m_poOwner->GetID();
	stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
	stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
	poFaction->AddFactionLogData(stFactionLogData);
	SetFindBackDoorsTribute();

	poFaction->DeletePlayerDataByID( m_poOwner->GetID());

	//记录退出时间，已状态
	LeaveFactionTime();

	return ERR_LEAVE_FACTION::ID_SUCCESS;
}
VOID CFactionPlayer::GetUnDisplayerBuild(UINT8& byBuildNum, UINT8* abyBuildID)
{
	byBuildNum = 0;
	if ( ESM_NO == CSinglePramMgr::Instance()->GetHotSpringFlag())
	{
		abyBuildID[byBuildNum] = EFB_HotSpring;
		byBuildNum++;
	}
}
UINT16 CFactionPlayer::GetFactionBuileGroup(PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if ( NULL == poFaction)
	{
		return  ERR_OPEN_FACTION_BUILD_GROUP::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	SDStrncpy(stAck.aszFactionName, poFaction->GetDT_FACTION_BASE_DATA().aszFactionName, MAX_FACTION_NUM - 1);
	UINT32 dwFactionLevel = poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel;
	CFactionMgr::Instance()->GetFactionBuileGroupByLevel(dwFactionLevel, stAck.stBuildFlag);
	GetPrompt(stAck.stPromptInfo);
	stAck.byFactionLevel = static_cast<UINT8>(dwFactionLevel);
	stAck.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
	DT_FACTION_PLAYER_CLI stCli;
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, m_poOwner->GetID());
	stAck.dwMyDoorTribute = stCli.dwDoorsTributeValues;
	m_poOwner->GetLocale().OnEnterFaction();
	stAck.stCurBuildLevel.byRareitemLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem);
	stAck.stCurBuildLevel.byGymnasiumLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Gymnasium);
	stAck.stCurBuildLevel.byShadeLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Shade);
	stAck.stCurBuildLevel.byHotSpringLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_HotSpring);
	SDStrncpy(stAck.aszNewFactionName, poFaction->GetDT_FACTION_BASE_DATA().aszFactionName, MAX_FACTION_NUM - 1);
	DT_FACTION_ICON_DATA& stIconData = poFaction->GetDT_FACTION_COMMON_DATA().stIconData;
	stAck.byFactionIconID = stIconData.byIcon;
	if(1 == poFaction->GetExt() && m_poOwner->GetID() == poFaction->GetDT_FACTION_BASE_DATA().dwFactionPlayerID)
	{
		stAck.byEditFacName = 1;
	}
	else
	{
		stAck.byEditFacName = 0;
	}
	GetUnDisplayerBuild(stAck.byUnDisplayBuildNum, stAck.abyUnDisplayBuildID);
	return  ERR_OPEN_FACTION_BUILD_GROUP::ID_SUCCESS;
}

VOID CFactionPlayer::GetPrompt(DT_FUNC_PROMPT_DATA& stPrompt)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return;
	}
	GetFactionPrompt(stPrompt.astBuildPromptInfo[stPrompt.byBuildNum++]);
	DT_FACTION_BASE_DATA& stData = poFaction->GetDT_FACTION_BASE_DATA();
	if ( stData.dwRareItemRoomGrade != 0)
	{
		m_oRareItem.GetPrompt(stPrompt.astBuildPromptInfo[stPrompt.byBuildNum++]);
	}

	if (stData.dwGymnasiumLevel != 0)
	{
		m_oGymnasium.GetPrompt(stPrompt.astBuildPromptInfo[stPrompt.byBuildNum++]);
	}
	
	if (stData.dwShadeLevel != 0)
	{
		m_oShade.GetPrompt(stPrompt.astBuildPromptInfo[stPrompt.byBuildNum++]);
	}
	
	if ( stData.dwHotSpringLevel != 0)
	{
		m_oHotSpring.GetPrompt(stPrompt.astBuildPromptInfo[stPrompt.byBuildNum++]);
	}
	
}

VOID CFactionPlayer::GetFactionPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ;
	}
	stPrompt.byBuildKindID = EFB_Faction;

	UINT32 dwFactionLevel = poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel;

	DT_FACTION_PLAYER_CLI stCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, m_poOwner->GetID());
	BOOL bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority(static_cast<em_Faction_Job>(stCli.byJobType), EFA_Audit);
	if ( bRet )
	{
		UINT32 dwApplyNum = poFaction->GetApplyFactionNum();
		if (dwApplyNum != 0)
		{
			UINT16 wCurPlayerNum = 0;
			UINT16 wMaxPlayerNum = 0;
			poFaction->GetFactionPlayerNum(wCurPlayerNum, wMaxPlayerNum);
			if ( wCurPlayerNum != wMaxPlayerNum)
			{
				stPrompt.byFuncFlag = EFPID_HaveApply;//有申请
				stPrompt.dwFuncCountDown = 0;
				return ;
			}
		}
	}

	UINT16 wMaxTime = CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(dwFactionLevel);
	DT_PLAYER_DONATE_INFO& stDonateInfo = GetFactionDonate();
	if ( 1 == stDonateInfo.byTodaysCoinDoorsTributeEncourage )
	{
		stPrompt.byFuncFlag = EFPID_Get;//可领奖
		stPrompt.dwFuncCountDown = 0;
		return ;
	}

	if ( stDonateInfo.wTodaysCoinDoorsTributeTimes < wMaxTime )
	{
		stPrompt.byFuncFlag = EFPID_Donate;//可捐献
		stPrompt.dwFuncCountDown = 0;
		return ;
	}
}

VOID CFactionPlayer::LeaveFactionTime()
{
	m_oPlayerMark.GetDT_PLAYER_MARK_DATA().stLeaveFactionMark.byLeaveMark = ESM_YES;
	m_oPlayerMark.GetDT_PLAYER_MARK_DATA().stLeaveFactionMark.qwLeaveTime = SGDP::SDTimeSecs();
}

VOID CFactionPlayer::GetFreeMsg(DT_CHANGE_MONSTER_INFO& stInfo)
{
	m_oShade.GetFreeMsg(stInfo);
}

UINT16 CFactionPlayer::OpenLeaveFaction(UINT32& dwIntervalTime, UINT8& byLeaveFactionMark)
{
	//清空门派相关数据
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ERR_OPEN_LEAVE_FACTION::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_poOwner->GetID());
	if( em_Faction_MartialHead == stFactionPlayerCli.byJobType)
	{
		UINT16 wCurPlayerNum = 0;
		UINT16 wMaxPlayerNum = 0;
		poFaction->GetFactionPlayerNum(wCurPlayerNum, wMaxPlayerNum);
		if (1 == wCurPlayerNum)
		{
			byLeaveFactionMark = ESM_YES;
			return ERR_OPEN_LEAVE_FACTION::ID_SUCCESS;
		}
		else
		{
			return ERR_OPEN_LEAVE_FACTION::ID_HEAD_NOT_LEAVE;
		}
	}
	byLeaveFactionMark = ESM_NO;
	dwIntervalTime = CLuamgr::Instance()->GetJoinFactionInterval();
	return ERR_OPEN_LEAVE_FACTION::ID_SUCCESS;
}

UINT16 CFactionPlayer::AuditFactionReq( UINT32 dwPlayerID, UINT8 byAuditWay, PKT_CLIGS_AUDIT_FACTION_ACK2& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_AUDIT_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
	}
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	DT_FACTION_PLAYER_CLI stPlayerCLi = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stPlayerCLi, m_poOwner->GetID());
	if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stPlayerCLi.byJobType, EFA_Audit))
	{
		return ERR_AUDIT_FACTION::ID_PLAYER_NOT_AUTHOR;
	}

	if( 1 == byAuditWay )
	{
		poFaction->GetFactionPlayerNum(stAck.wFactionCurPlayerNum, stAck.wFactionMaxPlayerNum);

		if ( stAck.wFactionMaxPlayerNum <= stAck.wFactionCurPlayerNum)
		{
			return ERR_AUDIT_FACTION::ID_PLAYER_NUM_IS_MAX;
		}

		if (  CSinglePramMgr::Instance()->GetEveryDayAuditMaxNum() <=  poFaction->GetTodayPassNum())
		{	
			return ERR_AUDIT_FACTION::ID_AUDIT_NUM_IS_MAX;
		}
	}

	vector<UINT32> vecFactionID;
	UINT16 wErrCode = poFaction->AuditFaction( byAuditWay, dwPlayerID, vecFactionID);
	if(ERR_AUDIT_FACTION::ID_SUCCESS == wErrCode)
	{
		//清掉所有申请记录
		vector<UINT32>::iterator vecitr;
		for (vecitr = vecFactionID.begin(); vecitr != vecFactionID.end(); vecitr++)
		{
			CFaction* poVecFaction = CFactionMgr::Instance()->GetFaction(*vecitr);
			if ( NULL != poVecFaction)
			{
				poVecFaction->CancelApplyFactionPlayerData(dwPlayerID);
			}
		}
	}

	stAck.wLeftAuditPlayerNum = poFaction->GetTodayPassNum() < CSinglePramMgr::Instance()->GetEveryDayAuditMaxNum()  ? CSinglePramMgr::Instance()->GetEveryDayAuditMaxNum() - poFaction->GetTodayPassNum() : 0;
	poFaction->GetFactionPlayerNum(stAck.wFactionCurPlayerNum, stAck.wFactionMaxPlayerNum);
	stAck.wLeftApplyNum = poFaction->GetApplyFactionNum();
	if ( 0 == stAck.wLeftApplyNum )
	{
		poFaction->SendMessageByAuthPrompt();
	}
	return wErrCode;
}

UINT16 CFactionPlayer::AuditAllRefuse()
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_AUDIT_ALL_REFUSE::ID_NOT_JOIN_FACTION;
	}
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	DT_FACTION_PLAYER_CLI stPlayerCLi = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stPlayerCLi, m_poOwner->GetID());
	if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stPlayerCLi.byJobType, EFA_Audit))
	{
		return ERR_AUDIT_ALL_REFUSE::ID_LIMIT_NOT_ENOUGH;
	}

	if ( 0 == poFaction->GetApplyFactionNum())
	{
		return ERR_AUDIT_ALL_REFUSE::ID_PLAYER_NUM_IS_ZERO;
	}
	poFaction->CleanAllApply();
	poFaction->SendMessageByFactionPrompt();
	return ERR_AUDIT_ALL_REFUSE::ID_SUCCESS;
}

UINT16 CFactionPlayer::OpenAuditFaction(PKT_CLIGS_OPEN_AUDIT_FACTION_ACK2& stAck )
{	
	CFaction * poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ERR_OPEN_AUDIT_FACTION::ID_PLAYER_NOT_IN_FACTION;
	}
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
	DT_FACTION_PLAYER_CLI stPlayerCLi = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stPlayerCLi, m_poOwner->GetID());
	if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stPlayerCLi.byJobType, EFA_Audit))
	{
		return ERR_OPEN_AUDIT_FACTION::ID_PLAYER_NOT_AUTHOR;
	}
	poFaction->GetDT_FACTION_PLAYER_AUDIT_CLI_LST2(stAck.stFactionPlayerAuditCliLst);
	stAck.wLeftAuditNum = poFaction->GetTodayPassNum() < CSinglePramMgr::Instance()->GetEveryDayAuditMaxNum()  ? CSinglePramMgr::Instance()->GetEveryDayAuditMaxNum() - poFaction->GetTodayPassNum() : 0;
	//当前人数与人数上限
	poFaction->GetFactionPlayerNum(stAck.wCurPlayerNum, stAck.wMaxPlayerNum);
	return ERR_OPEN_AUDIT_FACTION::ID_SUCCESS;
}

UINT16 CFactionPlayer::OnOpenFactionAck(PKT_CLIGS_OPEN_FACTION_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ERR_OPEN_FACTION::ID_NOT_JOIN_FACTION;
	}

	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
	
	DT_FACTION_BASE_DATA& stBaseData = poFaction->GetDT_FACTION_BASE_DATA();
	//赋值门派信息
	stAck.stFactionInfo.byIconID = poFaction->GetDT_FACTION_COMMON_DATA().stIconData.byIcon;
	SDStrncpy(stAck.stFactionInfo.aszFactionAnnouncement, stBaseData.aszFactionAnnouncement, MAX_FACTION_DESC_NUM - 1);
	SDStrncpy(stAck.stFactionInfo.aszFactionNotice, stBaseData.aszFactionNotice, MAX_FACTION_DESC_NUM - 1);
	const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stBaseData.dwFactionPlayerID);
	if ( NULL == pstPlayerBaseData)
	{
		return ERR_OPEN_FACTION::ID_HEAD_INFO_NOT_FIND;
	}
	SDStrncpy(stAck.stFactionInfo.aszHeadName, pstPlayerBaseData->aszDispName, NORMAL_MSG_LEN - 1);
	stAck.stFactionInfo.dwFactionFunds = stBaseData.dwFactionFunds;
	stAck.stFactionInfo.dwFactionRank = stBaseData.dwFactionRank;
	poFaction->GetFactionPlayerNum(stAck.stFactionInfo.wFactionNumberPeople, stAck.stFactionInfo.wMaxFactionNumberPeople);
	stAck.stFactionInfo.dwFactionLevel = stBaseData.dwFactionLevel;
	SDStrncpy(stAck.stFactionInfo.aszFactionName, stBaseData.aszFactionName, MAX_FACTION_NUM - 1);

	//赋值个人信息
	DT_FACTION_PLAYER_DATA* stPlayerData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if ( NULL == stPlayerData)
	{
		return ERR_OPEN_FACTION::ID_NOT_JOIN_FACTION;
	}
	stAck.stPlayerInfo.byRecvFlag = m_oDonate.GetDT_PLAYER_DONATE_INFO().byTodaysCoinDoorsTributeEncourage;
	stAck.stPlayerInfo.dwMyDoorsTribute = stPlayerData->dwDoorsTribute;
	stAck.stPlayerInfo.qwMySumDoorsTribute = stPlayerData->qwDoorsTributeSum;

	DT_FACTION_AUTHORITY_EX *poDT_FACTION_AUTHORITY = CFactorAuthorityPropMgr::Instance()->GetAuthorityEx(((em_Faction_Job)stPlayerData->byJobType));
	if (NULL != poDT_FACTION_AUTHORITY)
	{
		memcpy(&stAck.stPlayerInfo.stFactionAuthority, poDT_FACTION_AUTHORITY, sizeof(DT_FACTION_AUTHORITY_EX));
	}

	if ( 0 != stAck.stPlayerInfo.stFactionAuthority.byAudit )
	{
			stAck.stFactionInfo.byAuditNum = poFaction->GetApplyFactionNum();
	}

	if (0 == stPlayerData->qwJoinTime)
	{
		stPlayerData->qwJoinTime = SGDP::SDTimeSecs();
	}
	stAck.stPlayerInfo.qwJoinFactionTime = stPlayerData->qwJoinTime;
	stAck.stPlayerInfo.byFindBackDoorsTribute = (0 == m_dwFindBackDoorsTribute ? ESM_NO : ESM_YES);

	//赋值管理人员信息
	DT_FACTION_PLAYER_CLI_LST stLst = {0};
	poFaction->GetManagerInfo(stLst);
	for ( UINT8 byIndex = 0; byIndex < stLst.wFactionNum ; byIndex++)
	{
		stAck.stManagerInfo.astManagerInfo[stAck.stManagerInfo.byManagerNum].byPlayerJobType = stLst.astFactionPlayerCli[byIndex].byJobType;
		SDStrncpy(stAck.stManagerInfo.astManagerInfo[stAck.stManagerInfo.byManagerNum].aszPlayerName, stLst.astFactionPlayerCli[byIndex].aszDispName, NORMAL_MSG_LEN - 1);
		stAck.stManagerInfo.byManagerNum++;
	}
	return ERR_OPEN_FACTION::ID_SUCCESS;
}

UINT16 CFactionPlayer::FactionPlayerInfoAck(PKT_CLIGS_FACTION_PLAYERS_INFO_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_FACTION_PLAYERS_INFO::ID_NOT_JOIN_FACTION;
	}

	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	poFaction->GetDT_FACTION_PLAYER_CLI_LST(stAck.stFactionPlayerCliLst);
	stAck.stDoorsTributeLst.wDoorsTributeNum = stAck.stFactionPlayerCliLst.wFactionNum;
	stAck.stPlayerPowerInfo.wPlayerNum = stAck.stFactionPlayerCliLst.wFactionNum;
	for ( UINT16 wIndex = 0; wIndex < stAck.stFactionPlayerCliLst.wFactionNum; wIndex++)
	{
		CFactorPropMgr::Instance()->GetDoorsTributeInfo(stAck.stFactionPlayerCliLst.astFactionPlayerCli[wIndex].qwDoorsTributeSum, stAck.stDoorsTributeLst.astDoorsTributeInfo[wIndex]);
		const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stAck.stFactionPlayerCliLst.astFactionPlayerCli[wIndex].dwPlayerID);
		if ( NULL != poPlayerBaseData)
		{
			stAck.stPlayerPowerInfo.adwPowerInfo[wIndex] = poPlayerBaseData->dwPower;
		}
	}

	DT_FACTION_PLAYER_DATA* stPlayerData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if ( NULL == stPlayerData)
	{
		return ERR_FACTION_PLAYERS_INFO::ID_NOT_JOIN_FACTION;
	}
	DT_FACTION_AUTHORITY_EX *poDT_FACTION_AUTHORITY = CFactorAuthorityPropMgr::Instance()->GetAuthorityEx(((em_Faction_Job)stPlayerData->byJobType));
	if (NULL != poDT_FACTION_AUTHORITY)
	{
		memcpy(&stAck.stFactionAuthority, poDT_FACTION_AUTHORITY, sizeof(DT_FACTION_AUTHORITY_EX));
	}
	return ERR_FACTION_PLAYERS_INFO::ID_SUCCESS;
}

UINT16 CFactionPlayer::OnChangeIconAck(PKT_CLIGS_CHANGE_FACTION_ICON_ACK& stAck, UINT8 byIconID)
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_CHANGE_FACTION_ICON::ID_NOT_JOIN_FACTION;
	}
	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	DT_FACTION_PLAYER_DATA* poPlayerData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if ( NULL == poPlayerData)
	{
		return ERR_CHANGE_FACTION_ICON::ID_NOT_JOIN_FACTION;
	}

	bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)poPlayerData->byJobType, EFA_ChangeIcon);
	if ( !bRet)
	{
		return ERR_CHANGE_FACTION_ICON::ID_LIMIT_NOT_ENOUGH;
	}

	DT_FACTION_ICON_DATA& stIconData = poFaction->GetDT_FACTION_COMMON_DATA().stIconData;
	if ( byIconID == stIconData.byIcon)
	{
		return ERR_CHANGE_FACTION_ICON::ID_SUCCESS;
	}

	if( stIconData.byChangeNum < CSinglePramMgr::Instance()->GetFreeChangeIconNum() )
	{
		stIconData.byIcon = byIconID;
		stIconData.byChangeNum += 1;
		stIconData.qwlastChangeTime = SGDP::SDTimeSecs();
	}
	else
	{
		//INT32 nCount = stIconData.byChangeNum - CSinglePramMgr::Instance()->GetFreeChangeIconNum() + 1;
		UINT32 dwCostGold = CSinglePramMgr::Instance()->GetChangeFactionIcon();
		if ( m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
		{
			return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
		}
		m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_CHANGE_FACTION_ICON);
		stIconData.byIcon = byIconID;
		if ( stIconData.byChangeNum < 100)
		{
			stIconData.byChangeNum += 1;
		}
		stIconData.qwlastChangeTime = SGDP::SDTimeSecs();
	}

	
	stAck.qwGoldSync = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
	stAck.byIconID = stIconData.byIcon;
	return ERR_CHANGE_FACTION_ICON::ID_SUCCESS;
}

UINT16 CFactionPlayer::OnOpenChangeIconAck(PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_OPEN_CHANGE_FACTION_ICON::ID_NOT_JOIN_FACTION;
	}

	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	DT_FACTION_PLAYER_DATA* poPlayerData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if ( NULL == poPlayerData)
	{
		return ERR_OPEN_CHANGE_FACTION_ICON::ID_NOT_JOIN_FACTION;
	}

	
	bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)poPlayerData->byJobType, EFA_ChangeIcon);
	if ( !bRet)
	{
		return ERR_OPEN_CHANGE_FACTION_ICON::ID_LIMIT_NOT_ENOUGH;
	}
	DT_FACTION_ICON_DATA& stIconData = poFaction->GetDT_FACTION_COMMON_DATA().stIconData;
	stAck.byIconID = stIconData.byIcon;

	if( stIconData.byChangeNum < CSinglePramMgr::Instance()->GetFreeChangeIconNum() )
	{
		stAck.byFreeNum = CSinglePramMgr::Instance()->GetFreeChangeIconNum() - stIconData.byChangeNum;
		stAck.dwCostGold = 0;	
	}
	else
	{
		stAck.byFreeNum = 0;
		//INT32 nCount = stIconData.byChangeNum - CSinglePramMgr::Instance()->GetFreeChangeIconNum() + 1;
		stAck.dwCostGold = CSinglePramMgr::Instance()->GetChangeFactionIcon();
	}
	stAck.qwGoldNum = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
	stAck.byChangeNum = stIconData.byChangeNum;
	return ERR_OPEN_CHANGE_FACTION_ICON::ID_SUCCESS;
}

VOID CFactionPlayer::SetFindBackDoorsTribute()
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ;
	}
	DT_FACTION_PLAYER_DATA* poData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if ( NULL != poData)
	{
		m_dwFindBackDoorsTribute = poData->dwDoorsTribute;
	}
	
}

UINT16 CFactionPlayer::OnOpenFindBackDoorsTributeAck(PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_OPEN_FIND_BACK_DOORSTRIBUTE::ID_NOT_JOIN_FACTION;
	}

	if( poFaction->GetExceptionFlag() )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	if (0 == m_dwFindBackDoorsTribute)
	{
		return ERR_OPEN_FIND_BACK_DOORSTRIBUTE::ID_NOT_DOORSTRIBUTE_FIND_BACK;
	}

	stAck.dwFindBackDoorsTribute = m_dwFindBackDoorsTribute;
	stAck.byNormalPercent = CSinglePramMgr::Instance()->GetNormalFindBackDoorsTribute();
	stAck.byPerfectPercent = CSinglePramMgr::Instance()->GetPerfectFindBackDoorsTribute();

	stAck.stNormalCost.byResKind = ACTE_COIN;
	stAck.stNormalCost.qwResValue = CLuamgr::Instance()->GetNormalFindBackDoorsTribute(m_dwFindBackDoorsTribute);
	stAck.stPerfectCost.byResKind = ACTE_GOLD;
	stAck.stPerfectCost.qwResValue = CLuamgr::Instance()->GetPerfectFindBackDoorsTribute(m_dwFindBackDoorsTribute);
	
	return ERR_OPEN_FIND_BACK_DOORSTRIBUTE::ID_SUCCESS;
}

UINT16 CFactionPlayer::OnFindBackDoorsTributeAck(UINT8 byFindBackType, PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_FIND_BACK_DOORSTRIBUTE::ID_NOT_JOIN_FACTION;
	}

	if( poFaction->GetExceptionFlag() )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	if (0 == m_dwFindBackDoorsTribute)
	{
		return ERR_FIND_BACK_DOORSTRIBUTE::ID_NOT_DOORSTRIBUTE_FIND_BACK;
	}

	if ( 0 == byFindBackType)//普通找回
	{
		UINT32 dwCostValue = CLuamgr::Instance()->GetNormalFindBackDoorsTribute(m_dwFindBackDoorsTribute);
		if( m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < dwCostValue )
		{
			return ERR_COMMON::ID_COIN_NOT_ENOUGH;
		}

		if( poFaction->AddFindBackDoorsTribute(m_poOwner->GetID(), byFindBackType ))
		{
			m_poOwner->DecCoin(dwCostValue, CRecordMgr::EDCT_FIND_BACK_DOORSTRUBUTE );
		}
	}
	else //完美找回
	{
		UINT32 dwCostValue = CLuamgr::Instance()->GetPerfectFindBackDoorsTribute(m_dwFindBackDoorsTribute);
		if( m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostValue )
		{
			return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
		}

		if ( poFaction->AddFindBackDoorsTribute(m_poOwner->GetID(), byFindBackType))
		{
			m_poOwner->DecGold(dwCostValue, CRecordMgr::EDGT_FIND_BACK_DOORSTRIBUTE );
		}
	}
	DT_FACTION_PLAYER_DATA* poPlayerData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if ( NULL != poPlayerData)
	{
		stAck.dwSyncDoorsTribute = poPlayerData->dwDoorsTribute;
	}
	
	return ERR_FIND_BACK_DOORSTRIBUTE::ID_SUCCESS;
}

UINT16 CFactionPlayer::GetExprienceHotSpring(PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK& stAck, UINT32 dwExprienceValue)
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_GET_EXPRIENCE_HOT_SPRING::ID_NOT_HAVE_FACTION;
	}
	if( poFaction->GetExceptionFlag() )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
	CHotSpring& oHotSpring = GetHotSpring();
	return oHotSpring.GetExprienceHotSpring(stAck, dwExprienceValue);
	
}