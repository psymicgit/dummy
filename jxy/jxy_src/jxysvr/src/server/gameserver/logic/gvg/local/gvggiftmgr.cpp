#include "logic/gvg/local/gvggiftmgr.h"
#include "framework/gsapi.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/player/player.h"
#include "logic/item/itemmgr.h"
#include "logic/awaken/awaken.h"
#include "logic/faction/factionmgr.h"
#include "logic/player/playerslogmgr.h"
#include "db/basedbccmd.h"

IMPLEMENT_SINGLETON(CGvGGiftMgr)
BOOL CGvGGiftMgr::Init()
{
	if ( !LoadGiftFromDB() )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGvGGiftMgr::LoadGiftFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CHAR szSql[1024] = {0};
	sprintf(szSql, "select * from gvggift order by SessionID, PlayerID ;");


	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	string strError;
	INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
	if(SDDB_HAS_RECORDSET == nRet)
	{
		while (pRecordSet->GetRecord())
		{
			DT_GVG_GIFT_REWARD_DATA stGift = {0};
			stGift.dwPlayerID = SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
			stGift.wSessionID = SDAtou(pRecordSet->GetFieldValueByName("SessionID"));
			stGift.byGvGRound = SDAtou(pRecordSet->GetFieldValueByName("GvGRound"));
			stGift.byBattleIdx = SDAtou(pRecordSet->GetFieldValueByName("BattleIdx"));
			stGift.byBattleResult = SDAtou(pRecordSet->GetFieldValueByName("BattleResult"));
			SDStrncpy(stGift.aszFactionName, pRecordSet->GetFieldValueByName("FactionName"), MAX_FACTION_NUM - 1);
			stGift.byGiftID = SDAtou(pRecordSet->GetFieldValueByName("GiftID"));
			stGift.byGiftType = SDAtou(pRecordSet->GetFieldValueByName("GiftType"));
			stGift.byRank = SDAtou(pRecordSet->GetFieldValueByName("Rank"));
			m_mapGiftReward[GetGiftKey(stGift.dwPlayerID, stGift.wSessionID, stGift.byGvGRound, stGift.byBattleIdx)] = stGift;
		}
	}

	if ( pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}
	return TRUE;

}

UINT64 CGvGGiftMgr::GetGiftKey(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvGRound, UINT8 byBattleIdx)
{
	UINT16 wKey = CLogicFunc::Get88Key(byGvGRound, byBattleIdx);
	UINT32 dwKey = CLogicFunc::Get1616Key(wSessionID, wKey);
	return CLogicFunc::Get3232Key(dwPlayerID, dwKey);
}

UINT64 CGvGGiftMgr::GetContinueKillKey(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvgRound, UINT8 byBattleIdx)
{
	return GetGiftKey(dwPlayerID, wSessionID, byGvgRound, byBattleIdx);
}

//添加礼包
VOID CGvGGiftMgr::AddGiftReward(DT_GVG_GIFT_REWARD_DATA& stData)
{
	UINT64 qwKey = GetGiftKey(stData.dwPlayerID, stData.wSessionID, stData.byGvGRound, stData.byBattleIdx);
	if ( m_mapGiftReward.end() != m_mapGiftReward.find(qwKey))
	{
		return ;
	}
	m_mapGiftReward[qwKey] = stData;

}

//去掉过期礼包
VOID CGvGGiftMgr::RemoveOverGift()
{
	UINT8 byOver = 12;
	UINT16 wCurSession = CGvGBuild::Instance()->GetCurSession();
	UINT16 wOverSession = wCurSession < byOver ? 0 : wCurSession - byOver;
	
	UINT64 qwOverKey = GetGiftKey(0, wOverSession, 0, 0);
	UINT64 qwKey = GetGiftKey(0, 0xFFFF, 0, 0);
	for ( CMapGvGGiftRewardItr itr = m_mapGiftReward.begin(); itr != m_mapGiftReward.end(); )
	{
		if ( (itr->first & qwKey) ==  qwOverKey)
		{
			m_mapGiftReward.erase(itr++);
		}
		else
		{
			itr++;
		}
	}

	//数据库删除数据
	CBaseDBCMD* poDBCmd = gsapi::GetDBMgr()->CreateCmd(EDBCT_DELETE_GVG_GIFT);
	if ( NULL == poDBCmd )
	{
		USR_INFO("[%s : %d] Delete GvG Gift Failed", MSG_MARK);
		return ;
	}
	poDBCmd->SetUserData( &wOverSession);
	gsapi::GetDBMgr()->AddCommand(poDBCmd);
}

VOID CGvGGiftMgr::GetGiftInfo(UINT32 dwPlayerID, C64Set& setKey, DT_GVG_GIFT_DATA_LST_CLI& stGiftCli)
{
	UINT64 qwStartKey = GetGiftKey(dwPlayerID, 0x00, 0x00, 0x00);
	UINT64 qwEndKey = GetGiftKey(dwPlayerID, 0xFFFF, 0xFF, 0xFF);
	CMapGvGGiftRewardItr startItr = m_mapGiftReward.lower_bound( qwStartKey );
	CMapGvGGiftRewardItr endItr = m_mapGiftReward.upper_bound( qwEndKey );
	UINT8 byIdx = 0;
	for ( ;startItr != endItr && byIdx < MAX_GVG_GIFT_NUM; startItr++, byIdx++)
	{
		SDStrncpy( stGiftCli.astGiftInfo[byIdx].aszFactionName, startItr->second.aszFactionName, MAX_FACTION_NUM - 1);
		stGiftCli.astGiftInfo[byIdx].byBattleIdx = startItr->second.byBattleIdx;
		stGiftCli.astGiftInfo[byIdx].byBattleResult = startItr->second.byBattleResult;
		if( setKey.end() != setKey.find(startItr->first))
		{
			stGiftCli.astGiftInfo[byIdx].byGiftGetState = ESM_YES;
		}
		else
		{
			stGiftCli.astGiftInfo[byIdx].byGiftGetState = ESM_NO;
		}
		stGiftCli.astGiftInfo[byIdx].byGvGRound = startItr->second.byGvGRound;
		stGiftCli.astGiftInfo[byIdx].qwGiftID = startItr->first;
		stGiftCli.astGiftInfo[byIdx].wSessionID = startItr->second.wSessionID;
		stGiftCli.astGiftInfo[byIdx].byGiftType = startItr->second.byGiftType;
		stGiftCli.astGiftInfo[byIdx].byRank = startItr->second.byRank;
		SGvGGiftProp stProp;
		if( !CGvGPropMgr::Instance()->GetGiftProp(startItr->second.byGiftID, stProp))
		{
			continue;
		}
		CJsonResItemVectorItr vecItr = stProp.stGiftValue.vecJsonResItem.begin();
		UINT8 byGoodsIdx = 0;
		for ( ; vecItr != stProp.stGiftValue.vecJsonResItem.end(); vecItr++, byGoodsIdx++ )
		{
			stGiftCli.astGiftInfo[byIdx].stGiftInfo.astGoodsInfo[byGoodsIdx].byKindType = vecItr->byKindType;
			if ( ACTE_GOOD == vecItr->byKindType)
			{
				stGiftCli.astGiftInfo[byIdx].stGiftInfo.astGoodsInfo[byGoodsIdx].wKindID = vecItr->dwKindValue;
				stGiftCli.astGiftInfo[byIdx].stGiftInfo.astGoodsInfo[byGoodsIdx].dwKindNum = vecItr->byNum;
			}
			else
			{
				stGiftCli.astGiftInfo[byIdx].stGiftInfo.astGoodsInfo[byGoodsIdx].dwKindNum = vecItr->dwKindValue;
			}
		}
		stGiftCli.astGiftInfo[byIdx].stGiftInfo.byGoodsNum = byGoodsIdx;
	}
	stGiftCli.byGiftNum = byIdx;
}

BOOL CGvGGiftMgr::GetGiftInfo(UINT64 qwKey, DT_GVG_GIFT_REWARD_DATA& stData)
{
	if ( m_mapGiftReward.end() == m_mapGiftReward.find(qwKey))
	{
		return FALSE;
	}
	stData = m_mapGiftReward[qwKey];
	return TRUE;
}

EM_RESULT CGvGGiftMgr::AddGiftGoods(CPlayer* poPlayer, UINT64 qwKey)
{
	CMapGvGGiftRewardItr itr = m_mapGiftReward.find(qwKey);
	if ( m_mapGiftReward.end() == itr )
	{
		return GIFT_ID_NOT_EXIST;
	}

    DT_GVG_GIFT_REWARD_DATA &stData = itr->second;

	SGvGGiftProp stProp;
	if (!CGvGPropMgr::Instance()->GetGiftProp( itr->second.byGiftID, stProp))
	{
		return GIFT_ID_NOT_EXIST;
	}

	CJsonResItemVectorItr vecItr = stProp.stGiftValue.vecJsonResItem.begin();
	UINT16 wLeftNum = poPlayer->GetBag().GetIdleSize();
	UINT16 wItemNum = 0;
	for ( ; vecItr != stProp.stGiftValue.vecJsonResItem.end(); vecItr++)
	{
		if ( ACTE_GOOD == vecItr->byKindType)
		{
			//占用的格子数
			wItemNum += poPlayer->GetItemUseBagNum(vecItr->dwKindValue);
		}
	}

	if ( wLeftNum < wItemNum)
	{
		return GIFT_BAG_FULL;
	}

    CRecordMgr::Instance()->RecordInfo(poPlayer->GetPlayerID(), ERM_GVG_ACTIVITY, CRecordMgr::EGA_RECV_GIFT,
        0, 0, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 
        stData.wSessionID, stData.byGvGRound, stData.byGiftID, stData.byGiftType, stData.aszFactionName);

	vecItr = stProp.stGiftValue.vecJsonResItem.begin();
	for ( ; vecItr != stProp.stGiftValue.vecJsonResItem.end(); vecItr++)
	{
		switch(vecItr->byKindType)
		{
		case ACTE_GOOD:
			{
				for ( UINT8 byIdx = 0; byIdx < vecItr->byNum; byIdx++)
				{
					UINT16 wPileNum = 1;//物品个数
					ECreateItemRet Ret; //结果
					CItemMgr::Instance()->CreateItemIntoBag(poPlayer, vecItr->dwKindValue, wPileNum, Ret, 0, CRecordMgr::EAIT_GVG_GIFT, itr->second.byGiftID);
				}
			}
		case ACTE_COIN:
			{
				poPlayer->AddCoin(vecItr->dwKindValue, CRecordMgr::EACT_GVG_GIFT);
			}
			break;
		case ACTE_GOLD:
			{
				poPlayer->AddGold( vecItr->dwKindValue, CRecordMgr::EGGT_GVG_GIFT);
			}
			break;
		case ACTE_PHYSTRENGTH:
			{
				poPlayer->AddPhyStrength(vecItr->dwKindValue, CRecordMgr::EAPST_GVG_GIFT);
			}
			break;
		case ACTE_SCIENCE:
			{
				poPlayer->AddScience(vecItr->dwKindValue, CRecordMgr::EAST_GVG_GIFT);
			}
			break;
		case ACTE_STORY:
			{
				poPlayer->AddStory(vecItr->dwKindValue, CRecordMgr::EASTT_GVG_GIFT);
			}
			break;
		case ACTE_BLUEGAS:
			{
				poPlayer->AddBlueGas(vecItr->dwKindValue, CRecordMgr::EABGT_GVG_GIFT);
			}
			break;
		case ACTE_PRUPLEGAS:
			{
				poPlayer->AddPurpleGas(vecItr->dwKindValue, CRecordMgr::EAPGT_GVG_GIFT);
			}
			break;
		case ACTE_JINGJIE:
			{
				poPlayer->AddJingJie(vecItr->dwKindValue, CRecordMgr::EAJT_GVG_GIFT);
			}
			break;
		case ACTE_DoorsTribute:
			{
				CFaction* poFaction = poPlayer->GetFaction();
				if (NULL == poFaction)
				{
					continue;
				}
				poFaction->AddDoorsTribute(poPlayer->GetID(), vecItr->dwKindValue,CRecordMgr::EAT_GVG_GIFT);
			}
			break;
		case ACTE_FactionFunds:
			{
				CFaction* poFaction = poPlayer->GetFaction();
				if (NULL == poFaction)
				{
					continue;
				}
				poFaction->AddFactionFunds( vecItr->dwKindValue,CRecordMgr::EAFF_GVG_GIFT);
			}
			break;
		case ACTE_StudyClip://
			{
				CAwaken *poAwaken = poPlayer->GetAwaken();
				if ( NULL != poAwaken )
				{
					poAwaken->AddStudyClip(vecItr->dwKindValue, CRecordMgr::EASCTUDYTCLIP_GVG_GIFT);
				}
			}
			break;
		}
	}

	return GIFT_SUCCESS;
}

VOID CGvGGiftMgr::GetUnGetGiftKey(UINT32 dwPlayerID, C64Set& getKey, C64Set& ungetKey)
{
	UINT64 qwStartKey = GetGiftKey(dwPlayerID, 0x00, 0x00, 0x00);
	UINT64 qwEndKey = GetGiftKey(dwPlayerID, 0xFFFF, 0xFF, 0xFF);
	CMapGvGGiftRewardItr startItr = m_mapGiftReward.lower_bound( qwStartKey );
	CMapGvGGiftRewardItr endItr = m_mapGiftReward.upper_bound( qwEndKey );
	UINT8 byIdx = 0;
	for ( ;startItr != endItr && byIdx < MAX_GVG_GIFT_NUM; startItr++, byIdx++)
	{
		if( getKey.end() == getKey.find(startItr->first))
		{
			ungetKey.insert(startItr->first);
		}
	}
}

BOOL CGvGGiftMgr::AddContinueKill(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvGRound, UINT8 byBattleIdx, UINT8 byKillNum)
{
	CFaction* poFaction = CFactionMgr::Instance()->GetFactionByPlayerID(dwPlayerID);
	if ( NULL == poFaction)
	{
		return FALSE;
	}
	poFaction->AddContinueKill(dwPlayerID, wSessionID, byGvGRound, byBattleIdx, byKillNum);
	return TRUE;
}

//添加战斗礼包
VOID CGvGGiftMgr::AddBattleRewardByPlayerID(UINT32 dwPlayerID, UINT8 byRound, UINT8 byBattleIdx, const TCHAR* aszFactionName, UINT8 byResult)
{
	DT_GVG_GIFT_REWARD_DATA stData = {0};
	SDStrncpy(stData.aszFactionName, aszFactionName, MAX_FACTION_NUM - 1);
	stData.byBattleIdx = byBattleIdx;
	stData.byBattleResult = byResult;
	stData.byGiftID = CGvGGiftMgr::Instance()->GetBattleGiftID(byRound, byResult);
	stData.byGvGRound = byRound;
	stData.dwPlayerID = dwPlayerID;
	stData.wSessionID = CGvGBuild::Instance()->GetCurSession();
	stData.byGiftType = EGGT_BATTLE;
	m_vecAddToDBGift.push_back(stData);

    CRecordMgr::Instance()->RecordInfo(dwPlayerID, ERM_GVG_ACTIVITY, CRecordMgr::EGA_GIVE_GIFT, 0, 0, 0, 0, stData.wSessionID, byRound, stData.byGiftID, stData.byGiftType, aszFactionName);
}

//添加名次奖励
VOID CGvGGiftMgr::AddRankRewardByPlayerID(UINT32 dwPlayerID, UINT8 byRound, UINT8 byRank)
{
	UINT8 byGiftID = CGvGGiftMgr::Instance()->GetRankGiftID(byRound, byRank);
	if ( EGR_NONE == byGiftID )
	{
		return ;
	}
	DT_GVG_GIFT_REWARD_DATA stData = {0};
	stData.byGiftID = byGiftID;
	stData.byGvGRound = byRound;
	stData.dwPlayerID = dwPlayerID;
	stData.wSessionID = CGvGBuild::Instance()->GetCurSession();
	stData.byGiftType = EGGT_RANK;
	stData.byRank = byRank;
	m_vecAddToDBGift.push_back(stData);

    CRecordMgr::Instance()->RecordInfo(dwPlayerID, ERM_GVG_ACTIVITY, CRecordMgr::EGA_GIVE_GIFT, 0, 0, 0, 0, stData.wSessionID, byRound, stData.byGiftID, stData.byGiftType);
}

//添加荣誉奖励
VOID CGvGGiftMgr::AddHononRewardByPlayerID(UINT32 dwPlayerID, TCHAR* aszFactionName)
{
	DT_GVG_GIFT_REWARD_DATA stData = {0};
	SDStrncpy(stData.aszFactionName, aszFactionName, MAX_FACTION_NUM - 1);
	stData.byGiftID = EGR_ZONE_HONOR;
	stData.dwPlayerID = dwPlayerID;
	stData.wSessionID = CGvGBuild::Instance()->GetCurSession();
	stData.byGiftType = EGGT_HONOR;
	m_vecAddToDBGift.push_back(stData);

    CRecordMgr::Instance()->RecordInfo(dwPlayerID, ERM_GVG_ACTIVITY, CRecordMgr::EGA_GIVE_GIFT, 0, 0, 0, 0, stData.wSessionID, GVG_FINAL_WAR, stData.byGiftID, stData.byGiftType, aszFactionName);
}

//添加礼包进DB
VOID CGvGGiftMgr::AddGiftTODB()
{
	UINT8 byDBIdx = 0;
	for ( CVecGiftRewardDataItr itr = m_vecAddToDBGift.begin(); itr != m_vecAddToDBGift.end(); )
	{
		UINT64 qwKey = GetGiftKey(itr->dwPlayerID, itr->wSessionID, itr->byGvGRound, itr->byBattleIdx);
		CMapGvGGiftRewardItr mapItr = m_mapGiftReward.find(qwKey);
		if ( mapItr == m_mapGiftReward.end())
		{
			AddGiftReward(*itr);
			byDBIdx++;
			itr++;
		}
		else
		{
			itr = m_vecAddToDBGift.erase(itr);
		}
	}

	if ( 0 != byDBIdx)
	{
		//数据库删除数据
		CBaseDBCMD* poDBCmd = gsapi::GetDBMgr()->CreateCmd(EDBCT_ADD_GVG_GIFT);
		if ( NULL == poDBCmd )
		{
			USR_INFO("[%s : %d] Delete GvG Gift Failed", MSG_MARK);
			return ;
		}
		poDBCmd->SetUserData( &m_vecAddToDBGift);
		gsapi::GetDBMgr()->AddCommand(poDBCmd);
	}
	
	m_vecAddToDBGift.clear();
}

//要添加的礼包ID
UINT8 CGvGGiftMgr::GetBattleGiftID(UINT8 byRound, UINT8 byResult)
{
	if ( ESM_NO == byResult )
	{
		switch(byRound)
		{
		case GVG_CHAOS_WAR:
			return EGR_BATTLE_CHAOS_LOST;
			break;
		case GVG_16_TO_8:
			return EGR_BATTLE_16_TO_4_LOST;
			break;
		case GVG_8_TO_4:
			return EGR_BATTLE_16_TO_4_LOST;
			break;
		case GVG_FINAL_WAR:
			return EGR_BATTLE_FINAL_WAR_LOST;
			break;
		}
	}
	else
	{
		switch(byRound)
		{
		case GVG_CHAOS_WAR:
			return EGR_BATTLE_CHAOS_WIN;
			break;
		case GVG_16_TO_8:
			return EGR_BATTLE_16_TO_8_WIN;
			break;
		case GVG_8_TO_4:
			return EGR_BATTLE_8_TO_4_WIN;
			break;
		case GVG_FINAL_WAR:
			return EGR_BATTLE_FINAL_WAR_WIN;
			break;
		}
	}
	return EGR_NONE;
}

UINT8 CGvGGiftMgr::GetRankGiftID(UINT8 byRound, UINT8 byRank)
{
	switch(byRound)
	{
	case GVG_MONEY_WAR:
        if ( 1 <= byRank && byRank <= 3)
        {
            return EGR_RANK_MONEY_NO_1 + byRank - 1;
        }
        else if (4 == byRank)
        {
            return EGR_RANK_MONEY;
        }
		break;
	case GVG_CHAOS_WAR:
        if ( byRank <= 4 )
        {
            return EGR_RANK_CHAOS_WIN;
        }
		break;
	case GVG_8_TO_4:
        if ( byRank <= 4)
        {
            return EGR_RANK_8_TO_4_WIN;
        }
		break;
	case GVG_FINAL_WAR:
        if ( 1 == byRank)
        {
            return EGR_RANK_FINAL_CHAMPION;
        }
        else if ( 2 == byRank)
        {
            return EGR_RANK_FINAL_SECOND;
        }
        else if ( 3 == byRank)
        {
            return EGR_RANK_FINAL_THIRD;
        }
        else if( 4 <= byRank && byRank <= 8 )
        {
            return EGR_RANK_FINAL_4_8;
        }
		break;
	}
	return EGR_NONE;
}

//添加战斗礼包
VOID CGvGGiftMgr::AddBattleReward(UINT32 dwFactionID, UINT8 byRound, UINT8 byBattleIdx, const TCHAR* aszFactionName, UINT8 byResult)
{
	CFaction* poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
	if ( NULL == poFaction)
	{
		return ;
	}
	CMapFactionPlayerData& mapPlayer = poFaction->GetFactionPlayers();
	for ( CMapFactionPlayerDataItr itr = mapPlayer.begin(); itr != mapPlayer.end(); itr++)
	{
		AddBattleRewardByPlayerID(itr->first, byRound, byBattleIdx, aszFactionName, byResult);
	}
}

//添加名次奖励
VOID CGvGGiftMgr::AddRankReward(UINT32 dwFactionID, UINT8 byRound, UINT8 byRank)
{
	CFaction* poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
	if ( NULL == poFaction)
	{
		return ;
	}
	CMapFactionPlayerData& mapPlayer = poFaction->GetFactionPlayers();
	for ( CMapFactionPlayerDataItr itr = mapPlayer.begin(); itr != mapPlayer.end(); itr++)
	{
		AddRankRewardByPlayerID(itr->first, byRound, byRank);
	}
}

//添加荣誉奖励
VOID CGvGGiftMgr::AddHononReward( UINT32 dwFactionID)
{
	CFaction* poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
	if ( NULL == poFaction)
	{
		return ;
	}
	vector<UINT64> vec64;
	vector<string> vecStr;
	vecStr.push_back(poFaction->GetDT_FACTION_BASE_DATA().aszFactionName);
	vec64.push_back(CGvGPropMgr::Instance()->GetHonorLevel());
	SGvGGiftProp stProp;
	if( !CGvGPropMgr::Instance()->GetGiftProp(EGR_ZONE_HONOR, stProp))
	{
		return ;
	}
	CJsonResItemVectorItr vecItr = stProp.stGiftValue.vecJsonResItem.begin();
	
	for ( ; vecItr != stProp.stGiftValue.vecJsonResItem.end(); vecItr++ )
	{
		vec64.push_back(vecItr->byKindType);
		if ( ACTE_GOOD == vecItr->byKindType)
		{
			vec64.push_back(vecItr->dwKindValue); 
			vec64.push_back(vecItr->byNum);
		}
		else
		{
			vec64.push_back( vecItr->dwKindValue );
		}
	}
	const CPlayerID2PlayerBaseDataMap& mapPlayer = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
	for ( CPlayerID2PlayerBaseDataMap::const_iterator itr = mapPlayer.begin(); itr != mapPlayer.end(); itr++)
	{
		if ( itr->second->wLevel >= CGvGPropMgr::Instance()->GetHonorLevel() )
		{
			AddHononRewardByPlayerID(itr->first, poFaction->GetDT_FACTION_BASE_DATA().aszFactionName);			
			CPlayerLogMgr::Instance()->AddBattleLog(itr->first, 0, EBLT_GVG, EGLT_GVG_ALL_HONOUR, vec64, vecStr);
		}		
	}
}