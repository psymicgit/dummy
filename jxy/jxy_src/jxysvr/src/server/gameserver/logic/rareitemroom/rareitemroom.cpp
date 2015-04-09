#include "rareitemroom.h"
#include "rareitemroompropmgr.h"
#include <logic/faction/factionmgr.h>
#include <logic/player/playermgr.h>
#include <logic/other/errmsgmgr.h>
#include <logic/item/itemmgr.h>
CRareItemRoom::CRareItemRoom()
{
	memset( &m_stDT_RAREITEM_GOODS_DATA, 0, sizeof(DT_RAREITEM_GOODS_DATA));
	m_mapRareItemGoods.clear();
}
CRareItemRoom::~CRareItemRoom()
{
	m_mapRareItemGoods.clear();
}

BOOL CRareItemRoom::Init(DT_RAREITEM_GOODS_DATA &stDT_RAREITEM_GOODS_DATA, CPlayer* poOwner)
{
	m_poOwner = poOwner;
	m_qwUpdateTime = stDT_RAREITEM_GOODS_DATA.qwLastUpdataTime;
	UINT8 byGoodsNum = stDT_RAREITEM_GOODS_DATA.byRareItemNum;
	for (UINT8 byIndex = 0; byIndex < byGoodsNum; byIndex++)
	{
		m_mapRareItemGoods[stDT_RAREITEM_GOODS_DATA.astRareItemGoodInfo[byIndex].wRareItemID] = stDT_RAREITEM_GOODS_DATA.astRareItemGoodInfo[byIndex];
	}
	m_poOwner = poOwner;
	GetFaction();
	return TRUE;
}

DT_RAREITEM_GOODS_DATA& CRareItemRoom::GetDT_RAREITEM_GOODS_DATA()
{

	m_stDT_RAREITEM_GOODS_DATA.byRareItemNum = m_mapRareItemGoods.size();
	m_stDT_RAREITEM_GOODS_DATA.qwLastUpdataTime = m_qwUpdateTime;
	UINT8 byIndex = 0;
	for (CMapRareItemGoodsItr it = m_mapRareItemGoods.begin(); it != m_mapRareItemGoods.end(); it++, byIndex++)
	{
		m_stDT_RAREITEM_GOODS_DATA.astRareItemGoodInfo[byIndex] = it->second;
	}
	return m_stDT_RAREITEM_GOODS_DATA;
}

CFaction* CRareItemRoom::GetFaction()
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
	if ( !bRet )
	{
		return NULL;
	}
	return  CFactionMgr::Instance()->GetFaction(dwFactionID);
}

UINT32 CRareItemRoom::GetFactionID()
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
	if ( !bRet )
	{
		return 0 ;
	}
	return dwFactionID;
}

UINT32 CRareItemRoom::GetOpenRareItem(DT_RAREITEM_ROOM_INFO_CLI& stRareItemRoomInfo, DT_RAREITEM_INFO_LIST_CLI& stRareItemInfo)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	//获取门派ID
	if ( 0 == poFaction)
	{
		return ERR_OPEN_RAREITEMROOM::ID_FACTION_NOT_EXIST;
	}

	DT_FACTION_PLAYER_DATA* pstData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if (NULL == pstData)
	{
		return ERR_OPEN_RAREITEMROOM::ID_FACTION_NOT_EXIST;
	}
	
	BOOL bRet = IsOpen();
	if (!bRet)
	{
		return ERR_OPEN_RAREITEMROOM::ID_RAREITEM_ROOM_NOT_EXIST;
	}
	UINT32 dwRareItemGrade = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem);

	//重置已兑换次数
	UpdateDataByTime();

	//珍品阁的数据
	GetRareItemRoomInfo(stRareItemRoomInfo);
	//物品的数据

	memset(&stRareItemInfo, 0, sizeof(tagDT_RAREITEM_INFO_LIST_CLI));

	SetRareItemInfo(stRareItemInfo);
	if (pstData->byNewGoodFlag)
	{
		pstData->byNewGoodFlag = ESM_NO;
		PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
		memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));		
		GetPrompt(stNtf.stFactionPrompt);
		m_poOwner->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
	}
	
	
	return ERR_OPEN_RAREITEMROOM::ID_SUCCESS;
}

UINT32 CRareItemRoom::ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if ( 0 == dwFactionID )
	{
		return ERR_BUY_RAREITEM::ID_NOT_JOIN_FACTION;
	}

	BOOL bRet = IsOpen();
	if ( !bRet )
	{
		return ERR_BUY_RAREITEM::ID_RAREITEM_ROOM_NOT_EXIST;
	}
	//根据时间信息更新数据
	UpdateDataByTime();

	//检验兑换条件	
	UINT32 dwRet = CheckExchangeInfo(dwGoodsID);
	//返回CheckExchangeInfo返回的  非成功   错误码
	if ( ERR_BUY_RAREITEM::ID_SUCCESS != dwRet)
	{
		return dwRet;
	}

	SetSingleRareItemInfo( dwGoodsID, stRareItemInfo);

	////获取剩余门贡
	DT_RAREITEM_ROOM_INFO_CLI stRareItemRoomInfo = {0};
	GetRareItemRoomInfo(stRareItemRoomInfo);
	dwLeftDoorsTributes = stRareItemRoomInfo.dwRemainMoney;

	return ERR_BUY_RAREITEM::ID_SUCCESS;
}

UINT32 CRareItemRoom::ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID, UINT8 byLeftTime)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if ( 0 == dwFactionID )
	{
		return ERR_BUY_RAREITEM::ID_NOT_JOIN_FACTION;
	}

	BOOL bRet = IsOpen();
	if ( !bRet )
	{
		return ERR_BUY_RAREITEM::ID_RAREITEM_ROOM_NOT_EXIST;
	}
	//根据时间信息更新数据
	UpdateDataByTime();

	bRet = TRUE; //303版本以后比较购买次数是否一致

	const UpgradeRareItemMap& factionRareItemMap = poFaction->GetRareItemMap();
	UpgradeRareItemConstItr factionItr = factionRareItemMap.find(dwGoodsID);
	CMapRareItemGoodsItr myItr = m_mapRareItemGoods.find(dwGoodsID);
	if ( factionItr == factionRareItemMap.end())
	{
		return ERR_BUY_RAREITEM::ID_KINDID_INVALID;
	}

	UINT32 dwBuyTime = 0;
	if ( myItr != m_mapRareItemGoods.end())
	{
		dwBuyTime = myItr->second.dwRareItemHaveExchangeNum;
	}

	if ( factionItr->second.dwCurMaxRareItem - dwBuyTime != byLeftTime)
	{
		bRet = FALSE;
	}
	
	if (bRet)
	{
		//检验兑换条件	
		UINT32 dwRet = CheckExchangeInfo(dwGoodsID);
		//返回CheckExchangeInfo返回的  非成功   错误码
		if ( ERR_BUY_RAREITEM::ID_SUCCESS != dwRet)
		{
			return dwRet;
		}
	}

	SetSingleRareItemInfo( dwGoodsID, stRareItemInfo);

	////获取剩余门贡
	DT_RAREITEM_ROOM_INFO_CLI stRareItemRoomInfo = {0};
	GetRareItemRoomInfo(stRareItemRoomInfo);
	dwLeftDoorsTributes = stRareItemRoomInfo.dwRemainMoney;

	return ERR_BUY_RAREITEM::ID_SUCCESS;
}

//打开升级面板
UINT32 CRareItemRoom::OpenUpgradeRareItem(DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeCli)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	//获取门派资金

	if ( NULL == poFaction)
	{
		return ERR_OPEN_UPGRADERAREITEM::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = IsOpen();
	if ( !bRet )
	{
		return ERR_OPEN_UPGRADERAREITEM::ID_RAREITEM_NOT_EXIST;
	}
	//重置已兑换次数
	UpdateDataByTime();


	UINT32 dwRareLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem);
	stUpgradeCli.byBuildLevel = dwRareLevel;//当前建筑等级
	stUpgradeCli.byUnitedGrade = dwRareLevel + 1;//比当前建筑等级+1
	stUpgradeCli.byFactionIsEnough = poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel < stUpgradeCli.byUnitedGrade ? ESM_NO : ESM_YES; //门派等级是否达到要求
	
	bRet = CRareItemRoomPropMgr::Instance()->GetUpgradeInfo(dwRareLevel, stUpgradeCli.dwConsumptionMoney);
	if (!bRet)
	{
		return ERR_OPEN_UPGRADERAREITEM::ID_RAREITEM_LEVEL_IS_MAX;
	}
	UINT32 dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
	stUpgradeCli.byFactionFundsEnough = dwFactionFunds < stUpgradeCli.dwConsumptionMoney ? ESM_NO : ESM_YES;//门派资金是否达到要求

	const UpgradeRareItemMap* poNextMapRareItem = CRareItemRoomPropMgr::Instance()->GetRareItemInfoByFaction(dwRareLevel + 1);
	if ( NULL == poNextMapRareItem)
	{
		return ERR_OPEN_UPGRADERAREITEM::ID_RAREITEM_LEVEL_IS_MAX;
	}
	const UpgradeRareItemMap& oCurMapRareItem = poFaction->GetRareItemMap();
	 
	for (UpgradeRareItemMap::const_iterator itr = poNextMapRareItem->begin(); itr != poNextMapRareItem->end(); itr++)
	{
		UpgradeRareItemMap::const_iterator findItr = oCurMapRareItem.find(itr->second.wRareItemID);
		if (findItr != oCurMapRareItem.end())
		{
			if( findItr->second.dwCurMaxRareItem < itr->second.dwCurMaxRareItem )
			{
				string strMsg = CMsgDefMgr::Instance()->GetErrMsg("RATEITEM_UPGRADE_UP_NUM", NULL);
				SDStrncpy(stUpgradeCli.astUpgradeInfoList[stUpgradeCli.byInfoListNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN-1);
				stUpgradeCli.byInfoListNum += 1;
				break;
			}
		}
	}
	DT_FACTION_PLAYER_CLI stFactionPlayer = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayer, m_poOwner->GetID());
	em_Faction_Job emMyJob = static_cast<em_Faction_Job>(stFactionPlayer.byJobType);
	bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority(emMyJob, EFA_Build);
	stUpgradeCli.byIsUpgradeLimit = bRet ? ESM_YES : ESM_NO;
	return ERR_OPEN_UPGRADERAREITEM::ID_SUCCESS;
}

VOID CRareItemRoom::UpdateDataByTime()
{
	//当今天更新过,所有都不用再更新了
	if (!IsToday(m_qwUpdateTime))
	{
		for (CMapRareItemGoodsItr itr = m_mapRareItemGoods.begin(); itr != m_mapRareItemGoods.end(); itr++ )
		{
			DT_RAREITEM_GOODS_INFO& stInfo = itr->second;
			UINT8 byRule = CRareItemRoomPropMgr::Instance()->GetRareItemRule(stInfo.wRareItemID);
			switch (byRule)
			{
			case BYDAY:
				{
					ResetByDay(stInfo);
				}
				break;
			case BYWEEK:
				{
					ResetByWeek(stInfo);
				}
				break;
			case BYMONTH:
				{
					ResetByMonth(stInfo);
				}
				break;
			case BYNONE:
				{
					//无操作
				}
				break;
			default:
				//无操作
				break;
			}
		}
	}

	m_qwUpdateTime = SGDP::SDTimeSecs();
}

VOID CRareItemRoom::ResetByDay( DT_RAREITEM_GOODS_INFO& stRareItemInfo )
{
	if (!IsToday(stRareItemInfo.qwLastResetTime))
	{
		stRareItemInfo.dwRareItemHaveExchangeNum = 0;
		stRareItemInfo.qwLastResetTime = SGDP::SDTimeSecs();
	}
}
VOID CRareItemRoom::ResetByMonth( DT_RAREITEM_GOODS_INFO& stRareItemInfo )
{
	CSDDateTime oDataTime(SGDP::SDTimeSecs());
	UINT32 dwDay = oDataTime.GetDay();
	//每个月的第一天
	if (1 == dwDay)
	{
		if (!IsToday(stRareItemInfo.qwLastResetTime))
		{
			stRareItemInfo.dwRareItemHaveExchangeNum = 0;
			stRareItemInfo.qwLastResetTime = SGDP::SDTimeSecs();
		}
	}
}
VOID CRareItemRoom::ResetByWeek( DT_RAREITEM_GOODS_INFO& stRareItemInfo )
{
	CSDDateTime oDataTime(SGDP::SDTimeSecs());
	UINT32 dwWeek = oDataTime.GetWeek();
	//星期一
	if (1 == dwWeek)
	{
		if (!IsToday(stRareItemInfo.qwLastResetTime))
		{
			stRareItemInfo.dwRareItemHaveExchangeNum = 0;
			stRareItemInfo.qwLastResetTime = SGDP::SDTimeSecs();
		}
	}
}

VOID CRareItemRoom::GetRareItemRoomInfo(DT_RAREITEM_ROOM_INFO_CLI& stRareItemRoomInfo)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	memset( &stRareItemRoomInfo, 0, sizeof(DT_RAREITEM_ROOM_INFO_CLI));

	stRareItemRoomInfo.byRareItemGrade =  CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem);

	if ( NULL == poFaction)
	{
		return ;
	}
	DT_FACTION_PLAYER_CLI stDT_FACTION_PLAYER_CLI = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stDT_FACTION_PLAYER_CLI, m_poOwner->GetID());
	stRareItemRoomInfo.dwRemainMoney = stDT_FACTION_PLAYER_CLI.dwDoorsTributeValues;
	stRareItemRoomInfo.qwAccumulateMoney = stDT_FACTION_PLAYER_CLI.qwDoorsTributeSum;
	//权限
	em_Faction_Job emTmp = static_cast<em_Faction_Job>(stDT_FACTION_PLAYER_CLI.byJobType);

	BOOL bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority(emTmp, EFA_Build);

	stRareItemRoomInfo.byUpgradeLimit = bRet ? ESM_YES : ESM_NO;  
}

VOID CRareItemRoom::SetRareItemInfo( DT_RAREITEM_INFO_LIST_CLI& stCli)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if (NULL == poFaction)
	{
		return ;
	}
	UINT32 dwRareLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem) + 1;
	const UpgradeRareItemMap* poNextRareItemMap = CRareItemRoomPropMgr::Instance()->GetRareItemInfoByFaction(dwRareLevel);
	if ( NULL == poNextRareItemMap)
	{
		poNextRareItemMap = &poFaction->GetRareItemMap();
	}

	stCli.byRareItemNum = poNextRareItemMap->size();
	UINT8 byRareNum = 0;
	vector<DT_RAREITEM_INFO_CLI> vecLock;
	vector<DT_RAREITEM_INFO_CLI> vecLevelNotEnough;
	for (UpgradeRareItemMap::const_iterator itr = poNextRareItemMap->begin(); itr != poNextRareItemMap->end() && byRareNum < MAX_RAREITEM_RES_NUM; itr++)
	{
		BOOL bRet = SetSingleRareItemInfo(itr->second.wRareItemID, stCli.astRareItemInfoList[byRareNum]);
		if ( bRet)
		{
			if (ESM_NO == stCli.astRareItemInfoList[byRareNum].byIsUnLock)//未解锁
			{
				vecLock.push_back(stCli.astRareItemInfoList[byRareNum]);
			}
			else if (ESM_YES == stCli.astRareItemInfoList[byRareNum].byIsUnLock && ESM_NO == stCli.astRareItemInfoList[byRareNum].byNeedLeveEnough)
			{
				vecLevelNotEnough.push_back(stCli.astRareItemInfoList[byRareNum]);
			}
			else
			{
				byRareNum++;//防止配错
			}
		}
	}
	vector<DT_RAREITEM_INFO_CLI>::iterator vecItr;
	for ( vecItr = vecLevelNotEnough.begin(); vecItr !=  vecLevelNotEnough.end(); vecItr++)
	{
		stCli.astRareItemInfoList[byRareNum] = *vecItr;
		byRareNum++;
	}

	for ( vecItr = vecLock.begin(); vecItr !=  vecLock.end(); vecItr++)
	{
		stCli.astRareItemInfoList[byRareNum] = *vecItr;
		byRareNum++;
	}
}

BOOL CRareItemRoom::SetSingleRareItemInfo( UINT16 wRareItemID, DT_RAREITEM_INFO_CLI& stCli)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if (NULL == poFaction)
	{
		return FALSE;
	}
	UpgradeRareItemMap::const_iterator itr;
	UINT32 dwNextRareItemlevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem) + 1;
	const UpgradeRareItemMap* poNextRareItemMap = CRareItemRoomPropMgr::Instance()->GetRareItemInfoByFaction(dwNextRareItemlevel);
	if ( NULL == poNextRareItemMap)
	{
		poNextRareItemMap = &poFaction->GetRareItemMap();
	}
	itr = poNextRareItemMap->find(wRareItemID);
	if ( itr == poNextRareItemMap->end())
	{
		return FALSE;
	}
	const UpgradeRareItemMap& mapFactionRareItem = poFaction->GetRareItemMap();
	itr = mapFactionRareItem.find(wRareItemID);
	if (itr == mapFactionRareItem.end())
	{
		stCli.byIsUnLock = ESM_NO;
		stCli.byUnlockLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem) + 1;
	}
	else
	{
		stCli.byIsUnLock = ESM_YES;
		stCli.byUnlockLevel = 0;
	}

	SRareItemProp stProp;
	BOOL bRet = CRareItemRoomPropMgr::Instance()->GetConfig(stProp, wRareItemID);
	if ( !bRet )
	{
		return FALSE;
	}
	DT_FACTION_PLAYER_CLI stPlayerCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stPlayerCli, m_poOwner->GetID());
	CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(wRareItemID, 1, stCli.stRareItemInfo);
	stCli.dwRareItemSumRequire = stProp.dwSumDoorsTributeRequire;
	if (stPlayerCli.qwDoorsTributeSum < stCli.dwRareItemSumRequire)
	{
		stCli.bySumDoorsTributeEnough = ESM_NO;
	}
	else
	{
		stCli.bySumDoorsTributeEnough = ESM_YES;
	}

	stCli.byRareItemExchangeRule = stProp.byRareItemExchangeRule;
	stCli.byRareItemType = stProp.byRareItemType;
	if (stProp.dwBuyCost <= 0)
	{
		return FALSE;
	}
	stCli.dwRareItemExchangeMoney = stProp.dwBuyCost;
	
	if ( stPlayerCli.dwDoorsTributeValues < stCli.dwRareItemExchangeMoney)
	{
		stCli.byDoorsTributeEnough = ESM_NO;
	}
	else
	{
		stCli.byDoorsTributeEnough = ESM_YES;
	}
	//购买等级是否满足
	stCli.byBuyNeedLevel = stProp.byBuyNeedLevel;
	UINT16 wLevel = m_poOwner->GetLevel();
	if ( wLevel < stCli.byBuyNeedLevel)
	{
		stCli.byNeedLeveEnough = ESM_NO;
	}
	else
	{
		stCli.byNeedLeveEnough = ESM_YES;
	}

	CMapRareItemGoodsItr myItr = m_mapRareItemGoods.find(wRareItemID);
	if (myItr == m_mapRareItemGoods.end())
	{

		DT_RAREITEM_GOODS_INFO stInfo;
		stInfo.wRareItemID = wRareItemID;
		stInfo.dwRareItemHaveExchangeNum = 0;
		stInfo.qwLastResetTime = SGDP::SDTimeSecs();
		stCli.dwRareItemLeftExchange = itr->second.dwCurMaxRareItem;
		m_mapRareItemGoods[stInfo.wRareItemID] = stInfo;
	}
	else
	{
		stCli.dwRareItemLeftExchange = itr->second.dwCurMaxRareItem - myItr->second.dwRareItemHaveExchangeNum;
	}
	return TRUE;
}

UINT32 CRareItemRoom::CheckExchangeInfo(UINT32 dwGoodsID)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if (poFaction == NULL)
	{
		ERR_BUY_RAREITEM::ID_NOT_JOIN_FACTION;
	}
	const UpgradeRareItemMap& mapFactionRare =  poFaction->GetRareItemMap();
	UpgradeRareItemMap::const_iterator itrFaction = mapFactionRare.find(dwGoodsID);
	if (itrFaction == mapFactionRare.end())
	{
		return ERR_BUY_RAREITEM::ID_KINDID_INVALID;
	}

	CMapRareItemGoodsItr itrMy = m_mapRareItemGoods.find(dwGoodsID);
	if (itrMy == m_mapRareItemGoods.end())
	{
		return ERR_BUY_RAREITEM::ID_KINDID_INVALID;
	}
	UINT32 dwLeft = 0;
	if (itrFaction->second.dwCurMaxRareItem <= itrMy->second.dwRareItemHaveExchangeNum)
	{
		return ERR_BUY_RAREITEM::ID_EXCHANGE_TIMES_NOT_ENOUGH;//没兑换次数
	}
	else
	{
		dwLeft = itrFaction->second.dwCurMaxRareItem - itrMy->second.dwRareItemHaveExchangeNum;
	}
	SRareItemProp stProp;
	CRareItemRoomPropMgr::Instance()->GetConfig(stProp, dwGoodsID);

	DT_RAREITEM_ROOM_INFO_CLI stDoorsTributeInfo = {0};
	GetRareItemRoomInfo(stDoorsTributeInfo);

	if (stDoorsTributeInfo.dwRemainMoney < stProp.dwBuyCost && 0 < stProp.dwBuyCost)
	{
		return ERR_BUY_RAREITEM::ID_DOORSTRIBUTE_NOT_ENOUGH;//门贡不足
	}

	if (stDoorsTributeInfo.qwAccumulateMoney < stProp.dwSumDoorsTributeRequire &&  0 < stProp.dwSumDoorsTributeRequire)
	{
		return ERR_BUY_RAREITEM::ID_SUMDOORSTRIBUTE_NOT_ENOUGH;//累积门贡不足
	}
	UINT16 wLevel = m_poOwner->GetLevel();
	if ( wLevel < stProp.byBuyNeedLevel)
	{
		return ERR_BUY_RAREITEM::ID_LEVEL_NOT_ENOUGH;
	}

	//物品添加
	UINT16 wPileNum = 1;  //购买装备个数
	ECreateItemRet Ret; //结果
	if (stProp.byRareItemType != SKILL)
	{
		CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, stProp.dwKindID, wPileNum, Ret, 0, CRecordMgr::EAIT_RAREITEM_EXCHANGE);
	}
	else//非实物
	{
		//技能未开放
	}

	if (Ret == ECIR_FAILED_BAG_FULL)
	{
		return ERR_BUY_RAREITEM::ID_BAG_FULL;
	}

	//已兑换次数+1
	itrMy->second.dwRareItemHaveExchangeNum += 1;

	//扣除门贡
	CFactionMgr::Instance()->DeductDoorsTribute(m_poOwner->GetID(), stProp.dwBuyCost, CRecordMgr::EDDT_BUY_RAREITEM);

	return ERR_BUY_RAREITEM::ID_SUCCESS;
}

//确认升级面板
UINT32 CRareItemRoom::UpgradeRareItem(PKT_CLIGS_UPGRADE_RAREITEM_ACK& stAck)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if (NULL == poFaction)
	{
		return ERR_UPGRADE_RAREITEM::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = IsOpen();
	if ( !bRet )
	{
		return ERR_UPGRADE_RAREITEM::ID_RAREITEM_NOT_EXIST;
	}

	bRet = CFactionMgr::Instance()->CheckPlayerAuthority(m_poOwner->GetID(), EFA_Build);
	if ( !bRet )
	{
		return ERR_UPGRADE_RAREITEM::ID_LIMIT_NOT_ENOUGH;
	}
	//重置已兑换次数
	UpdateDataByTime();
	UINT32 dwBeferRareItemNum = poFaction->GetRareItemMap().size();
	UINT8 byUnitedGrade = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Rareitem);
	UINT32 dwConsumptionMoney = 0;

	bRet = CRareItemRoomPropMgr::Instance()->GetUpgradeInfo( byUnitedGrade, dwConsumptionMoney);
	if ( !bRet )
	{
		ERR_UPGRADE_RAREITEM::ID_FACTION_LEVEL_IS_MAX;
	}

	DT_FACTION_BASE_DATA& stFactionBaseData = poFaction->GetDT_FACTION_BASE_DATA();
	UINT32 dwNeedFactionLevel = byUnitedGrade + 1;
	if ( stFactionBaseData.dwFactionLevel < dwNeedFactionLevel)
	{
		return ERR_UPGRADE_RAREITEM::ID_UNITED_GRADE_NOT_ENOUGH;
	}

	//检查门派资金  
	if ( stFactionBaseData.dwFactionFunds < dwConsumptionMoney && 0 <= dwConsumptionMoney)
	{
		return ERR_UPGRADE_RAREITEM::ID_UNITED_FUNDS_NOT_ENOUGH;
	}

	//珍品阁等级加1
	stFactionBaseData.dwRareItemRoomGrade += 1;

	//扣除资金
	poFaction->DecFactionFunds(dwConsumptionMoney, CRecordMgr::EDFF_UPGRADE_RAREITEM);

	//日志
	DT_FACTION_LOG_DATA stFactionLogData;
	stFactionLogData.dwPlayerID1 = m_poOwner->GetID();
	DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli,  m_poOwner->GetID());
	stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
	stFactionLogData.byAction = EFLID_UpgradeBuild;
	stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
	stFactionLogData.adwParamList[0] = dwConsumptionMoney;//需要资金
	stFactionLogData.adwParamList[1] = EFB_Rareitem;//建筑ID
	stFactionLogData.adwParamList[2] = poFaction->GetDT_FACTION_BASE_DATA().dwRareItemRoomGrade;//提升后的等级
	poFaction->AddFactionLogData(stFactionLogData);

	//更新CRareItemMgr配置
	poFaction->SetFactionRareInfo();
	UINT32 dwAfterRareItemNum = poFaction->GetRareItemMap().size();
	if (dwAfterRareItemNum != dwBeferRareItemNum)
	{
		poFaction->SetAllNewRareItemFlag();
	}
	
	//赋值升级后的信息
	GetAfterUpgradeInfo(stAck.stAfterUpgradeInfo);

	//下一级升级要求
	UINT32 dwRet = GetNextUpgradeRequireInfo(stAck.stUpgradeRequireInfo);
	if (dwRet == ERR_OPEN_UPGRADERAREITEM::ID_RAREITEM_LEVEL_IS_MAX)
	{
		stAck.stAfterUpgradeInfo.byIsMaxLevel = 1;
		memset(&stAck.stUpgradeRequireInfo, 0, sizeof(DT_UPGRADE_REQUIRE_INFO_CLI) );
	}
	else
	{
		stAck.stAfterUpgradeInfo.byIsMaxLevel = 0;
	}
	stAck.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
	return ERR_UPGRADE_RAREITEM::ID_SUCCESS;	
}

VOID CRareItemRoom::GetAfterUpgradeInfo(DT_AFTER_UPGRADE_INFO_CLI& stAfterUpgradeInfo)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ;
	}
	//升级后的珍品阁等级
	stAfterUpgradeInfo.byRareItemGrade = poFaction->GetDT_FACTION_BASE_DATA().dwRareItemRoomGrade;

	//赋值物品信息
	DT_RAREITEM_ROOM_INFO_CLI stRareItemRoomInfo;
	GetOpenRareItem(stRareItemRoomInfo, stAfterUpgradeInfo.stRareItemInfo);
}

UINT32 CRareItemRoom::GetNextUpgradeRequireInfo( DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo)
{
	return OpenUpgradeRareItem(stUpgradeRequireInfo);
}

BOOL CRareItemRoom::IsOpen()
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return FALSE;
	}
	if( 0 == poFaction->GetDT_FACTION_BASE_DATA().dwRareItemRoomGrade)
	{
		return FALSE;
	}
	return TRUE;
}

VOID CRareItemRoom::GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return;
	}
	DT_FACTION_PLAYER_DATA* stData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if (NULL == stData)
	{
		return ;
	}

	stPrompt.byBuildKindID = EFB_Rareitem;
	if (stData->byNewGoodFlag)
	{
		stPrompt.byFuncFlag = EFPID_HaveNewRareItem;//新货物
		return ;
	}

	//可购买
	if(poFaction->IsCanBuyRareItem(m_poOwner->GetID()))
	{
		stPrompt.byFuncFlag = EFPID_CanBuy;//可购买
	}
}