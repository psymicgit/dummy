#include "dbcommoperate.h"
#include "server/base64.h"
#include "protocommondata_old.h"
#include "logic/godweapon/godweaponpropmgr.h"
#include <sdloggerimpl.h>
//
//CDBCommOperate::CDBCommOperate()
//{
//
//}
//
//CDBCommOperate::~CDBCommOperate()
//{
//
//}
//
//BOOL CDBCommOperate::FillPlayerData( ISDDBRecordSet &recordSet, DT_PLAYER_NOPVP_DATA& stDT_PLAYER_DATA, UINT32 dwPlayerID )
//{
//
//    GetEncodeDataFromDB( "BaseData", stDT_PLAYER_DATA.stBaseData, dwPlayerID );
//
//	DT_PLAYER_BASE_DATA_OLD stDT_PLAYER_BASE_DATA_OLD;
//	GetEncodeDataFromDB( "BaseData", stDT_PLAYER_BASE_DATA_OLD, dwPlayerID );
//	memcpy(stDT_PLAYER_DATA.stBaseData.stGuideRecordData.abyGuideRecordData, stDT_PLAYER_BASE_DATA_OLD.abyGuideRecordData, sizeof(stDT_PLAYER_BASE_DATA_OLD.abyGuideRecordData));	
//
//    if(stDT_PLAYER_DATA.stBaseData.dwID != dwPlayerID)
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: PlayerID not match!"), MSG_MARK);
//        stDT_PLAYER_DATA.stBaseData.dwID = dwPlayerID;
//    }
//
//
//    GetEncodeDataFromDB( "BuildData", stDT_PLAYER_DATA.stBuildData, dwPlayerID );
//
//    GetEncodeDataFromDB( "CurState", stDT_PLAYER_DATA.stCurState, dwPlayerID );
//
//    GetEncodeDataFromDB( "TaskData", stDT_PLAYER_DATA.stTaskData, dwPlayerID );
//
//    GetEncodeDataFromDB( "BagEquipData", stDT_PLAYER_DATA.stBagEquipData, dwPlayerID );
//
//    GetEncodeDataFromDB( "BagGoodsData", stDT_PLAYER_DATA.stBagGoodsData, dwPlayerID );
//
//   //GetEncodeDataFromDB( "HeroData", stDT_PLAYER_DATA.stHeroData, dwPlayerID );
//	DT_HERO_DATA_LIST_OLD stDT_HERO_DATA_LIST_OLD;
//	memset(&stDT_HERO_DATA_LIST_OLD, 0, sizeof(DT_HERO_DATA_LIST_OLD));
//	GetEncodeDataFromDB( "HeroData", stDT_HERO_DATA_LIST_OLD, dwPlayerID );	
//	memset(&stDT_PLAYER_DATA.stHeroData, 0, sizeof(DT_HERO_DATA_LIST));
//	UINT16 wHeroNum = stDT_HERO_DATA_LIST_OLD.wHeroNum;
//	UINT16 wLevel = 1;
//	for(UINT8 byIdx = 0; byIdx < wHeroNum; byIdx++)
//	{
//		DT_HERO_DATA& stDT_HERO_DATA = stDT_PLAYER_DATA.stHeroData.astHeroInfoLst[stDT_PLAYER_DATA.stHeroData.wHeroNum++];
//		DT_HERO_DATA_OLD& stDT_HERO_DATA_OLD = stDT_HERO_DATA_LIST_OLD.astHeroInfoLst[byIdx];
//		memcpy(&stDT_HERO_DATA.stHeroBaseData, &stDT_HERO_DATA_OLD.stHeroBaseData, sizeof(DT_HERO_BASE_DATA));
//		memcpy(&stDT_HERO_DATA.stEquipDataInfo, &stDT_HERO_DATA_OLD.stEquipDataInfo, sizeof(DT_HERO_EQUIP_DATA_LST));
//		memcpy(&stDT_HERO_DATA.stDrugDataInfo, &stDT_HERO_DATA_OLD.stDrugDataInfo, sizeof(DT_DRUG_DATA_LST));
//		memcpy(&stDT_HERO_DATA.stGodweaponInfo, &stDT_HERO_DATA_OLD.stGodweaponInfo, sizeof(DT_GODWEAPON_DATA));
//		memcpy(&stDT_HERO_DATA.stStudyInfo.astStudyInfo, &stDT_HERO_DATA_OLD.astStudyInfo, sizeof(DT_AWAKEN_DATA_LST));
//		DT_SKILL_DATA& stDT_SKILL_DATA = stDT_HERO_DATA.stSkillInfo.astSkillInfo[stDT_HERO_DATA.stSkillInfo.bySkillNum++];
//
//		stDT_SKILL_DATA.wActionID = stDT_HERO_DATA_OLD.stSkillInfo.wActionID;
//		stDT_SKILL_DATA.wActionLevel = 1;//技能等级重置为1
//		stDT_SKILL_DATA.stAttackRangInfo.byAttackRangKind = 1;//
//		stDT_SKILL_DATA.stAttackRangInfo.byTraceBuffKind = 0;
//		stDT_SKILL_DATA.dwActionExp = 0;	
//	
//		if(1 == stDT_HERO_DATA.stHeroBaseData.byCoach)
//		{
//			wLevel = stDT_HERO_DATA.stHeroBaseData.wLevel;
//		}
//	}
//	if(wLevel <= 30 && 2 == stDT_PLAYER_DATA.stBaseData.stGuideRecordData.abyGuideRecordData[18]) //<=30级的玩家，若已经完成熔炼指引，则重新添加该指引
//	{
//		stDT_PLAYER_DATA.stBaseData.stGuideRecordData.abyGuideRecordData[18] = 1; 
//	}
//	
//
//    GetEncodeDataFromDB( "CommonBattleRcd", stDT_PLAYER_DATA.stInstanceData.stCommonBattleRcd, dwPlayerID );
//
//    GetEncodeDataFromDB( "EliteBattleRcd", stDT_PLAYER_DATA.stInstanceData.stEliteBattleRcd, dwPlayerID );
//
//    GetEncodeDataFromDB( "CommonInstanceData", stDT_PLAYER_DATA.stInstanceData.stCommonInstanceData, dwPlayerID );
//
//    GetEncodeDataFromDB( "EliteInstanceData", stDT_PLAYER_DATA.stInstanceData.stEliteInstanceData, dwPlayerID );
//
//	//精英副本分组修改
//	for(UINT8 byIdx = 0; byIdx < stDT_PLAYER_DATA.stInstanceData.stEliteInstanceData.wInstanceNum; byIdx++)
//	{
//		UINT16 wTmp = 0;
//		DT_INSTANCE_ID& stDT_INSTANCE_ID = stDT_PLAYER_DATA.stInstanceData.stEliteInstanceData.astInstanceInfo[byIdx].stInstanceID;
//		if(0 == stDT_INSTANCE_ID.byTownIdx)
//		{
//			continue;
//		}
//		if(stDT_INSTANCE_ID.byTownIdx < 3)
//		{
//			wTmp = (stDT_INSTANCE_ID.byTownIdx-1) * 18 + stDT_INSTANCE_ID.byInstanceIdx;
//		}
//		else
//		{
//			wTmp = 2 * 18 + (stDT_INSTANCE_ID.byTownIdx - 3) * 9 + stDT_INSTANCE_ID.byInstanceIdx;
//		}
//		stDT_INSTANCE_ID.byTownIdx = wTmp / 9 + 1;
//		stDT_INSTANCE_ID.byInstanceIdx = wTmp % 9;
//		if(0 == stDT_INSTANCE_ID.byInstanceIdx)		
//		{
//			stDT_INSTANCE_ID.byInstanceIdx = 9;
//			stDT_INSTANCE_ID.byTownIdx = stDT_INSTANCE_ID.byTownIdx - 1;
//		}
//	}	
//
//    GetEncodeDataFromDB( "LocaleData", stDT_PLAYER_DATA.stLocaleData, dwPlayerID );
//
//    GetEncodeDataFromDB( "LoginEncRecdData", stDT_PLAYER_DATA.stLoginEncRecdData, dwPlayerID );
//
//    GetEncodeDataFromDB( "MoneyTreeData", stDT_PLAYER_DATA.stMoneyTreeData, dwPlayerID );
//
//    GetEncodeDataFromDB( "OnlineEncRecdData", stDT_PLAYER_DATA.stOnlineEncRecdData, dwPlayerID );
//
//    GetEncodeDataFromDB( "PhystrengthData", stDT_PLAYER_DATA.stPhystrengthData, dwPlayerID );
//
//    GetEncodeDataFromDB( "JingJieData", stDT_PLAYER_DATA.stJingJieData, dwPlayerID );
//
//    GetEncodeDataFromDB( "CropData", stDT_PLAYER_DATA.stCropData, dwPlayerID );
//
//    GetEncodeDataFromDB( "ScienceTree", stDT_PLAYER_DATA.stScienceTree, dwPlayerID );
//
//    GetEncodeDataFromDB( "FewParamData", stDT_PLAYER_DATA.stFewParamData, dwPlayerID );
//
//    GetEncodeDataFromDB( "FormationInfo", stDT_PLAYER_DATA.stFormationInfo, dwPlayerID );
//
//    GetEncodeDataFromDB( "YabiaoInfo", stDT_PLAYER_DATA.stYabiaoInfo.stYabiaoBaseInfo, dwPlayerID );
//
//    GetEncodeDataFromDB( "ClimbTowerInstanceData", stDT_PLAYER_DATA.stInstanceData.stClimbTowerInstanceData, dwPlayerID );
//
//    GetEncodeDataFromDB( "ClimbTowerBattleRcd", stDT_PLAYER_DATA.stInstanceData.stClimbTowerBattleRcd, dwPlayerID );
//
//	GetEncodeDataFromDB( "EatBreadInfo", stDT_PLAYER_DATA.stEatBreadInfo, dwPlayerID );
//
//    GetEncodeDataFromDB( "YabiaoBattleLog", stDT_PLAYER_DATA.stYabiaoInfo.stYabiaoBattleLog, dwPlayerID );
//
//	GetEncodeDataFromDB( "ClimbTowerData", stDT_PLAYER_DATA.stClimbTowerData, dwPlayerID );
//
//	GetEncodeDataFromDB( "VipExtData", stDT_PLAYER_DATA.stVipExtData, dwPlayerID );
//
//	GetEncodeDataFromDB( "AwakenData", stDT_PLAYER_DATA.stAwakenData, dwPlayerID );
//
//	GetEncodeDataFromDB( "LuaActivityRecordData", stDT_PLAYER_DATA.stLuaActivityRecordData, dwPlayerID );
//
//	//
//
//	//使用独立的字段
//	stDT_PLAYER_DATA.stBaseData.qwGold = SDAtou( recordSet.GetFieldValueByName("Gold") );
//	stDT_PLAYER_DATA.stBaseData.qwCoin = SDAtou( recordSet.GetFieldValueByName("Coin") );
//	stDT_PLAYER_DATA.stBaseData.qwStory = SDAtou( recordSet.GetFieldValueByName("Story") );
//	stDT_PLAYER_DATA.stBaseData.wPhyStrength = SDAtou( recordSet.GetFieldValueByName("CurPhyStrength") );
//	stDT_PLAYER_DATA.stBaseData.qwScience = SDAtou( recordSet.GetFieldValueByName("Science") );
//	stDT_PLAYER_DATA.stBaseData.byVipLevel = SDAtou( recordSet.GetFieldValueByName("VipLevel") );
//	stDT_PLAYER_DATA.stBaseData.dwTotalAmount = SDAtou( recordSet.GetFieldValueByName("TotalAmount") );
//
//	stDT_PLAYER_DATA.stBaseData.qwJingJie = stDT_PLAYER_DATA.stBaseData.qwBlueGas;//蓝气转移到境界点
//
//	//神器返还器魂点，1阶0级
//	wHeroNum = stDT_PLAYER_DATA.stHeroData.wHeroNum;
//	for(UINT8 byIdx = 0; byIdx < wHeroNum; byIdx++)
//	{
//		DT_GODWEAPON_DATA& stDT_GODWEAPON_DATA = stDT_PLAYER_DATA.stHeroData.astHeroInfoLst[byIdx].stGodweaponInfo;
//		if(0 != stDT_GODWEAPON_DATA.byHaveDataFlag)
//		{
//			stDT_PLAYER_DATA.stBaseData.qwScience += CGodweaponPropMgr::Instance()->GetUpgradeTotalScience(stDT_GODWEAPON_DATA.wHeroKindID, stDT_GODWEAPON_DATA.wQuality, stDT_GODWEAPON_DATA.wLevel);
//			stDT_GODWEAPON_DATA.wQuality = 1;
//			stDT_GODWEAPON_DATA.wLevel = 0;
//		}
//	}	
//
//    UINT32 dwLen = recordSet.GetFieldLengthByName("LastOrderID");
//    dwLen = dwLen > MAX_ORDER_ID_LEN ? MAX_ORDER_ID_LEN : dwLen;
//    memcpy(stDT_PLAYER_DATA.szLastOrderID, (VOID*)(recordSet.GetFieldValueByName("LastOrderID")), dwLen);
//
//    dwLen = recordSet.GetFieldLengthByName("NotifyID");
//    dwLen = dwLen > MAX_TOKEN_NUM ? MAX_TOKEN_NUM : dwLen;
//    memcpy(stDT_PLAYER_DATA.abyNoticeID, (VOID*)(recordSet.GetFieldValueByName("NotifyID")), dwLen);
//
//	//@2013-03-07 记录上下线时间
//
//	CHAR szTime[256] = {0};
//	memset(szTime, 0x00, sizeof(szTime));
//	dwLen = recordSet.GetFieldLengthByName("LoginTime");
//	dwLen = dwLen > 255 ? 255 : dwLen;
//	memcpy(szTime, (VOID*)(recordSet.GetFieldValueByName("LoginTime")), dwLen);
//
//    CSDDateTime stDateTime;
//    tstring    strDateTime = szTime;
//
//
//    if(SDTimeFromString(strDateTime, stDateTime))
//    {
//        stDT_PLAYER_DATA.qwLoginTime = stDateTime.GetTimeValue();
//    }
//    else
//    {
//        stDT_PLAYER_DATA.qwLoginTime = SDTimeSecs();
//    }
//
//	memset(szTime, 0x00, sizeof(szTime));
//	dwLen = recordSet.GetFieldLengthByName("LoginTime");
//	dwLen = dwLen > 255 ? 255 : dwLen;
//	memcpy(szTime, (VOID*)(recordSet.GetFieldValueByName("LoginTime")), dwLen);
//    strDateTime = szTime;
//    if(SDTimeFromString(strDateTime, stDateTime))
//    {
//        stDT_PLAYER_DATA.qwLogoffTime = stDateTime.GetTimeValue();
//    }
//    else
//    {
//        stDT_PLAYER_DATA.qwLogoffTime = SDTimeSecs();
//    }
//	//@end
//    return TRUE;
//}
