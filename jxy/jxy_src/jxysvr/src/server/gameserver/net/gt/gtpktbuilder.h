

#ifndef _GT_SERVER_PACKAGE_BUILDER_H_
#define _GT_SERVER_PACKAGE_BUILDER_H_

#include "sdpkg.h"
#include <common/client/errdef.h>
#include <protocligs.h>

using namespace SGDP;

class CPlayer;

class CGTPktBuilder : public CSDPacketBuilder
{
public:
    static CGTPktBuilder* Instance()
    {
        static CGTPktBuilder oGTPkgBuilder;
        return &oGTPkgBuilder;
    }
protected:
    CGTPktBuilder();
    ~CGTPktBuilder();
public:
    CHAR*  EncodeToCliBuffer(const UINT16 wMsgID) ;
    INT32  EncodedToCliLength() ;
private:
    CHAR    m_szEncodeBuffer[MAX_PACKET_LEN];
    INT32   m_nEncodedLen;
public:

    UINT16 EnterGSAck(UINT16 wErrCode, CPlayer* poPlayer = NULL);

    UINT16 GetHeroInfoErrorAck( UINT32 dwError );
    UINT16 GetHeroInfoAck(CPlayer* poPlayer, UINT8 byEquipBagFlag, BOOL bSelfQuery);

    UINT16 OnEnterHomeAck(CPlayer* poPlayer, UINT8 byKind);

    UINT16 EnterInstanceAck(UINT16 wErrCode, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, CPlayer* poPlayer);

    //UINT16 BattleAck(CPlayer* poPlayer, UINT8 byMonsterIdx, UINT16 wHangNum, UINT32 adwHangIDList[]);
    //UINT16 BattleAck(PKT_CLIGS_BATTLE_ACK* pstPKT_CLIGS_BATTLE_ACK);

    UINT16 PickUpAck(UINT16 wErrCode);

    UINT16 ExitInstanceAck(CPlayer* poPlayer);

    UINT16 GetInstanceRecordAck(PKT_CLIGS_GET_SCENE_INSTANCE_ACK* pstPKT_CLIGS_GET_SCENE_INSTANCE_ACK);

    UINT16 OpenSmithyAck(CPlayer* poPlayer, UINT8 byBuildKindID);

    UINT16 SmithyBuyEquipAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byEquipPos);

    UINT16 StrengthenAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byStrengthenType, UINT16 wLevel);

    UINT16 EquipEnchantAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT16 wLevel);

	UINT16 GetOpenLightInfoAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx);

	UINT16 OpenLightAck(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byGoldFlag);

    UINT16 OpenRecruitAck(CPlayer* poPlayer, UINT8 byBuildKindID);

    UINT16 RecruitHeroAck(CPlayer* poPlayer, UINT16 wHeroKindID);

    UINT16 DisbandHeroAck(CPlayer* poPlayer, UINT16 wHeroKindID);

    UINT16 OpenProduceAck(CPlayer* poPlayer, UINT8 byBuildKindID);

    UINT16 OpenBagAck(CPlayer* poPlayer);

    UINT16 OpenScienceAck(CPlayer* poPlayer, UINT8 byBuildKindID);

    UINT16 ScienceAttrUpgrade(CPlayer* poPlayer, UINT8 byAttrKindID, UINT16 wLevel);

    UINT16 ScienceBuildUpgradeAck(UINT16 wErrCode, CPlayer* poPlayer, UINT8 byBuildKindID);

    UINT16 RecvTaskEncourageAck(CPlayer* poPlayer, UINT8 byTaskLineIdx, UINT32 dwTaskIdx);

    UINT16 OpenRaceBuildAck(CPlayer* poPlayer, UINT8 byBuildKindID);

    UINT16 ChallengeAck(UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy);

    UINT16 RecvRankEncourageAck(CPlayer* poPlayer);

    UINT16 OpenShopAck(CPlayer* poPlayer, UINT8 byTabID);

    UINT16 BuyItemAck(CPlayer* poPlayer, UINT8 byTabID, UINT16 wItemKindID, UINT16 wPileCount);

    UINT16 SellItemAck(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wIdx, UINT16 wNum);

    UINT16 ClrPlayerCache(UINT8 byExt = 0);//0为状态判断是否重新登录(正常使用)，1为强制踢下线
    UINT16 RegiUserNameNtf();//注册用户名通知

    UINT16 EquipAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byDressType, UINT16 wDressItemKindID, UINT16 wDressItemIdx, UINT16 wUndressItemKindID, UINT16 wUndressItemIdx);

    UINT16 QuickEquipAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byEquipNum, DT_EQUIP_ID astEquipIDLst[EQUIP_POS_NUM]);

    UINT16 EmbattleAck(CPlayer* poPlayer, DT_FORMATION_DATA& stFormationInfo);

    UINT16 OpenEmbattleAck(CPlayer* poPlayer);

    UINT16 OpenItemMakerAck(CPlayer* poPlayer, UINT16 wItemKindID);

    UINT16 MakeItemAck(CPlayer* poPlayer, UINT16 wItemKindID, UINT16 wEquipKindID, UINT16 wEquipIdx);

    UINT16 GetHeroBrugAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byNeedDrugItemFlag);

    UINT16 UseBrugAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT16 wKindID);

    UINT16 OnhookAck(CPlayer* poPlayer, UINT8 byRunTimes, DT_INSTANCE_ID stDT_INSTANCE_ID);

    UINT16 OnOpenInstanceEncourageBoxAck(CPlayer* poPlayer);

    UINT16 OnIncreaseBagSizeAck(CPlayer* poPlayer, UINT16 wInc2Size, UINT8 byGoldFlag);

    UINT16 OnInterruptOnhookAck(CPlayer* poPlayer);

    UINT16 OnNoCDOnhookAck(CPlayer* poPlayer);

    UINT16 OnOnhookResultAck(CPlayer* poPlayer);

    UINT16 OnClrChallengeCDAck(CPlayer* poPlayer);

    UINT16 OnRecvOnlineEncourageAck(CPlayer* poPlayer);

    UINT16 OnSyncTimeAck(UINT64 qwCliTime);

    UINT16 OnRegisterDspNameAck(CPlayer* poPlayer, CHAR* pszDspName);

    UINT16 UseGoodsAck(CPlayer* poPlayer, UINT16 wGoodsKindID, UINT8 byGetBagInfoFlag, UINT16 wUseNum);

    UINT16 OnOpenLoginEncourageAck(CPlayer* poPlayer);

    UINT16 OnRateLoginEncourageAck(CPlayer* poPlayer);

    UINT16 OnPutLoginEncourageIntoBagAck(CPlayer* poPlayer);

    UINT16 OnGetRankAck();

    UINT16 OnAcceptTaskAck(CPlayer* poPlayer, UINT8 byTaskLineIdx, UINT32 dwTaskIdx);

    UINT16 OnGiveUpTaskAck(CPlayer* poPlayer, UINT8 byTaskLineIdx, UINT32 dwTaskIdx);

    UINT16 OnGetRaceBattleLogAck(CPlayer* poPlayer);

    UINT16 OnOpenCaptureTabAck(CPlayer* poPlayer);
    UINT16 PlunderAck(UINT16 wErrCode, CPlayer* poActive, CPlayer* poPassive = NULL, CPlayer* poCapture = NULL);
    UINT16 OnPlayerUpgrqage(UINT32 dwPlayerID, UINT16 wLevel);
    UINT16 OnOpenEnemyTabAck(CPlayer* poPlayer, UINT16 wStartIdx);
    UINT16 OnDelEnemyAck(CPlayer* poPlayer, UINT32 dwEnemyID, UINT16 wStartIdx);
    UINT16 OnOpenPlunderRecommendPlayerAck(CPlayer* poPlayer);
    UINT16 OnFreeCaptureAck(CPlayer* poPlayer, UINT32 dwCapturePlayerID);
    UINT16 OnGetPlunderBattleLogAck(CPlayer* poPlayer);
    UINT16 OnRefreshPlunderRecommendAck(CPlayer* poPlayer);
    UINT16 OnCollectIncomeAck(CPlayer* poPlayer, UINT32  dwCapturePlayerID, UINT32 dwIncomeValue);
    UINT16 OnBleedCaptureAck(CPlayer* poPlayer, UINT32 dwCapturePlayerID);
    UINT16 OnSetWorkTypeAck(CPlayer* poPlayer, UINT32 dwCapturePlayerID, UINT8 byIncomeType);
    UINT16 OnDriveAwayAck(CPlayer* poPlayer, UINT16 wErrCode = 0);
    UINT16 OnRecvLeftIncomeAck(CPlayer* poPlayer, UINT8 byGridIdx);

    UINT16 OnOpenMoneyTreeAck(CPlayer* poPlayer);
    UINT16 OnRockMoneyTreeAck(CPlayer* poPlayer, UINT8 byGuideFlag);

    UINT16 OnOpenGodweaponAck(CPlayer* poPlayer);
    UINT16 OnUpgradeGodweaponAck(CPlayer* poPlayer, UINT16 wHeroKindID,	UINT16 wQuality, UINT16 wLevel);
    UINT16 OnFreeGodweaponAck(CPlayer* poPlayer, UINT16 wHeroKindID, BOOL bUseGold);

    UINT16 OnOpenAwakenTabReq(CPlayer* poPlayer);
    UINT16 OnAwakenReq(CPlayer* poPlayer, UINT8 byQuickFlag, UINT8 byGoldFlag);
    UINT16 OnOpenDressStudyTabReq(CPlayer* poPlayer);
    UINT16 OnDressStudyReq(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx);
    UINT16 OnStudyToExpReq(CPlayer* poPlayer, UINT8 byBagGridIdx, UINT8 byAllFlag, UINT8 abySelectConvertColor[MAX_COLOR_NUM]);
    UINT16 OnStudyUpgradeReq(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byLevel);
	/////////////////////////////
	UINT16 OnOpenAwakenTabReq2(CPlayer* poPlayer);
	UINT16 OnAwakenReq2(CPlayer* poPlayer, UINT8 byQuickFlag, UINT8 byGoldFlag);
	UINT16 OnOpenDressStudyTabReq2(CPlayer* poPlayer);
	UINT16 OnDressStudyReq2(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx);
	UINT16 OnStudyToExpReq2(CPlayer* poPlayer, UINT8 byBagGridIdx, UINT8 byAllFlag, UINT8 abySelectConvertColor[MAX_COLOR_NUM]);
	UINT16 OnStudyUpgradeReq2(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byLevel);
	/////////////////////////////////

    UINT16 OnOpenResolveTrigram(CPlayer* poPlayer, UINT8 byPos);
    UINT16 OnResolveTrigram(CPlayer* poPlayer, UINT8 byPos ,UINT8 byUseGold);
    UINT16 OnVisitHeroAck(CPlayer* poPlayer, UINT16 wHeroKindID, UINT8 bySizeFlag, UINT8 byGoldFlag);

    UINT16 OnBuyPhystrengthAck(CPlayer* poPlayer);

    UINT16 OnOpenMaintownAck(CPlayer* poPlayer);
    UINT16 OnRecvIncomeAck(CPlayer* poPlayer);
    UINT16 OnLevyIncomeAck(CPlayer* poPlayer);

    //////////////////////////////////////////////////////////////////////////
    UINT16 OnOpenDrugUseTabAck(CPlayer* poPlayer);
    //打开武将丹药服用面板请求
    UINT16 OnOpenHeroDrugUseTabAck(CPlayer* poPlayer, UINT16 wKindID);
    //清除丹药服用CD请求
    UINT16 OnClrUseDrugCDAck(CPlayer* poPlayer, UINT16 wKindID);
    //使用丹药请求
    UINT16 OnUseDrugAckEx(CPlayer* poPlayer, UINT16 wHeroKindID, UINT16	wUseLevel);
    //打开丹药升级面板请求
    UINT16 OnOpenUpgreadeDrugAck(CPlayer* poPlayer);
    //丹药升级请求
    UINT16 OnUpgreadeDrugAck(CPlayer* poPlayer, UINT8 byUseDrugLevel);
    //丹药合成请求
    UINT16 OnComposeEquipAck(CPlayer* poPlayer, 	UINT16 wEquipItemID, UINT16 wEquipIdx, UINT8 byGoldComposeFlag);

    //打开体力购买面板
    UINT16 OnOpenPhyStrengthAck(CPlayer* poPlayer);

    UINT16 OnClrStrengthenCDAck(CPlayer* poPlayer);


    UINT16	OnOpenCropReq(CPlayer* poPlayer);
    UINT16	OnOpenCropFieldReq(CPlayer* poPlayer, UINT8 byFieldID);
    UINT16	OnCropReq(CPlayer* poPlayer, UINT8 byFieldID, UINT16 wCropKindID);
    UINT16	OnGainCropReq(CPlayer* poPlayer, UINT8 byFieldID);
    UINT16	OnOpenScienceBuildReq(CPlayer* poPlayer);
    UINT16	OnUpdateScienceReq( CPlayer* poPlayer, UINT8 byKind, UINT16 wCurLevel );
    UINT16	OnClrCropReq(CPlayer* poPlayer, UINT8 byFieldID);
    UINT16	OnOpenExperienceDrugReq(CPlayer* poPlayer, UINT16 wHeroID);
    UINT16	OnUserExperienceDrugReq(CPlayer* poPlayer, UINT16 wHeroID, UINT16 byExperienceDrugID, UINT16 wGoldExperienceRemainCount);
    UINT16	OnOpenMeltEquipReq(CPlayer* poPlayer);
	UINT16	OnMeltReq(CPlayer* poPlayer, UINT8 byGoldFlag, UINT16 wEquipNum, DT_MELT_EQUIP_DATA stDT_MELT_EQUIP_DATA[MAX_BAG_OPEN_NUM], UINT16 wGoodNum, DT_MELT_GOOD_DATA astMeltGoodDataList[MAX_BAG_OPEN_NUM]);
    UINT16	OnOpenUpdateAttrPannelReq( CPlayer* poPlayer, UINT16 wKind, UINT8 byAttrKind );
    UINT16	OnUpdateAttrReq( CPlayer* poPlayer, UINT16 wHeroID, UINT8 byAttrKind, UINT8 byUpdateKind, UINT16 wCurCount );
    UINT16	OnComposeReq( CPlayer* poPlayer, UINT16 wItemKindID, UINT8 byFlag);
    UINT16	OnGetItemReq( CPlayer* poPlayer, UINT16 wItemKindID);
    UINT16	OnGenOrderIDReqError( UINT16 wErrorCode );
    UINT16	OnComposeSeedReq( CPlayer* poPlayer, UINT16 wItemKindID);
    UINT16	OnChatReq( CPlayer* poPlayer, UINT32 dwPlayerID, UINT8 byChatType, UINT8 abyChatContent[MAX_CHAT_CONTENT_NUM], UINT32 dwChatLen);
    UINT16	OnOpenChatReq( CPlayer* poPlayer, UINT32 dwSerialNumber);

    UINT16	OnOpenActivityCenterAck(CPlayer* poPlayer);
    UINT16	OnOpneEatBreadAck(CPlayer* poPlayer);
    UINT16	OnObtainPhystrengthenAck(CPlayer* poPlayer, UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli);
    UINT16	OnOLeaveEatBreadAck(CPlayer* poPlayer, UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli);

    //重置爬塔
    UINT16	OnClimbTowerResetReq(CPlayer* poPlayer,  UINT16 wSceneIdx, UINT8 byTownIdx);
    //爬塔挂机
    UINT16	OnClimbTowerOnHookReq(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx);

    UINT16	OnOpenPaoshangBuildReq( CPlayer* poPlayer );
    UINT16	OnOpenYabiaoReq( CPlayer* poPlayer );
    UINT16	OnBeginYabiaoReq( CPlayer* poPlayer );
    UINT16	OnFlushBiaoQualityReq( CPlayer* poPlayer, UINT16 wCurQuality, BOOL bDirectMaxLevel );
    UINT16	OnOpenJiebiaoReq( CPlayer* poPlayer, BOOL bEnemy, UINT16 wTimeToEnd, UINT8 byNum );
    UINT16	OnBeginJiebiaoReq( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat );
    UINT16	BattleDataAck5( CPlayer*poPlayer, const DT_BATTLE_DATA5 &data, BOOL bHaveData );
    UINT16	BattleDataAck2( CPlayer*poPlayer, const DT_BATTLE_DATA3 &data, BOOL bHaveData );
	UINT16	BattleDataAck4( CPlayer*poPlayer, const DT_BATTLE_DATA4 &data, BOOL bHaveData );
    UINT16	BeJiebiaoEventNtf( BOOL bRevoltSuccess, UINT64 qwIndex, UINT64 qwCoinDec, UINT32 dwScienceDec,
                               const string &strEnemyDispName, UINT16 wEnemyLevel, UINT32 dwEnemyPower, BYTE byUnreadBattleLogNum );
    UINT16	OnQueryYabiaoBattleLogReq( CPlayer *poPlayer );
    UINT16	OnQueryYabiaoEncourageReq( CPlayer *poPlayer );
    UINT16	OnClearJiebiaoCDReq( CPlayer *poPlayer );
    ////////////////////////
    //境界升级
    UINT16	OnUpgradeJingjieReq(CPlayer *poPlayer, UINT16 wLevel);
    //生产精气球
    UINT16	OnGenGasBallReq(CPlayer *poPlayer, UINT8 byFlag);
    //提升精气球等级
    UINT16	OnUpGasBallLevelReq(CPlayer *poPlayer, UINT8 byFullLevelFlag);
    //收集精气等级
    UINT16	OnCollectGasReq(CPlayer *poPlayer);
    //升级技能招式等级
    UINT16	OnUpgradeSkillActionLevelReq(CPlayer *poPlayer, UINT16 wActionID, UINT16 wActionLevelCli);
    //选择技能招式效果
    UINT16	OnSelectSkillActionEffectReq(CPlayer *poPlayer, 	UINT16 wActionID, UINT16 wEffectID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind);
    //打开境界面板
    UINT16	OnOpenJingjieTabReq(CPlayer *poPlayer);
    //打开练气面板
    UINT16	OnOpenGenGasTabReq(CPlayer *poPlayer);
    //打开技能面板
    UINT16	OnOpenSkillTabReq(CPlayer *poPlayer);

    //打开VIP面板
    UINT16	OnOpenVipTabReq(CPlayer *poPlayer);

    //打开爬塔请求
    UINT16	OnOpenClimbTowerReq(CPlayer *poPlayer);


    //购买重置次数请求
    //UINT16 OnBuyClimbTowerResetReq(CPlayer *poPlayer, UINT8 byBuyNum);

    //打开镶嵌面板
    UINT16 OnOpenXiangqianTabReq( CPlayer *poPlayer );

    //镶嵌请求
    UINT16 OnXiangqianReq( CPlayer *poPlayer, UINT16 wEquipID, UINT16 wEquipIdx, UINT16 wItemID, UINT8 byXiangqianPos, UINT16 wHeroID );

    //
    UINT16 OnAppIDCheckError( UINT16 wErrorCode, UINT32 dwPlayerID, string strTransactionID);

    //byPos 宝石的位置
    UINT16 OnRemoveXiangqianReq( CPlayer *poPlayer, UINT16 wHeroID, UINT16 wEquipID, UINT16 wIdx, UINT8 byPos, BOOL bRemoveAll );

    //获取精英副本
    UINT16 OnGetEliteInstance( CPlayer *poPlayer );

    UINT16 OnResetEliteInstance( CPlayer *poPlayer, UINT8 byIndex, UINT16 wRemainRetNum );
    //精英副本挂机
    UINT16 OnEliteInstanceOnHook( CPlayer *poPlayer, UINT8 byIndex );

    //获得活动提示
    UINT16 OnGetActivityPromptReq(CPlayer *poPlayer);

    //蓝气转紫气
    UINT16 OnBlueToPurpleReq( CPlayer *poPlayer, UINT32 dwBlueGas);

    //切磋
    UINT16 OnPKBattleReq(CPlayer *poMyself, CPlayer *poEnemy);
    //获取资源不足提示
    UINT16 OnResourcesPromptReq( CPlayer *poPlayer, UINT32 dwRsID);;
    //BOSS战
    UINT16 OnEnterBossbReq(CPlayer *poPlayer, UINT16 wPosX, UINT16 wPosY);
    UINT16 OnLeaveBossbReq(CPlayer *poPlayer);
    UINT16 OnBossbMoveReq(CPlayer *poPlayer, UINT16 wPosX, UINT16 wPosY);
    UINT16 OnBossbHurtReq(CPlayer *poPlayer);
    UINT16 OnOpenInspireTabReq(CPlayer *poPlayer);
    UINT16 OnInspirePowerReq(CPlayer *poPlayer, UINT8 byGoldInspireFlag);
    UINT16 OnBossBReliveReq(CPlayer *poPlayer);
    UINT16 OnGetLastBossBInfoReq(CPlayer *poPlayer);
    UINT16 OnOpenBossBBLReq(CPlayer *poPlayer);
    //appstore充值验证请求
    UINT32 OnAppPayCheckReq( CPlayer *poPlayer, const string &strOrderID, const string &strReceiptData, UINT32 dwTransID, UINT32 dwPayPlayerID );

    //appstore充值成功,玩家必须在线，才能完成封包推送
    UINT32	OnAppPaySuccess( UINT8 byVipLevel, UINT32 dwTotalAmount, UINT64 qwAllGold, UINT32 dwExchangeGold, const string &strTransactionID, UINT32 dwPlayerID);

public:

    UINT16	GenGUIDAck2GT(string strOrderID, UINT16 wErrCode);
    UINT16	OnOpenRechargeEncourageReq( CPlayer *poPlayer );
    UINT16	OnRecvRechargeEncourageReq( CPlayer *poPlayer );
    UINT16	OnOpenScoreEncourageReq( CPlayer *poPlayer );
    UINT16	OnRecvScoreEncourageReq( CPlayer *poPlayer );
    UINT16	OnGetRandomDspNameReq( CPlayer *poPlayer );

    UINT16	OnOpenExperienceTransferReq( CPlayer *poPlayer, UINT16 wHeroID);
    UINT16	OnExperienceTransferReq( CPlayer *poPlayer, UINT16 wHeroID, UINT16 wTransHeroID, UINT8 byGold);

    UINT16	OnOpenRdchallengeTabReq( CPlayer *poPlayer, UINT8 byRefreshFlag);
    UINT16	OnRdchallengeReq(CPlayer *poPlayer, CPlayer * poEnemyPlayer);
    UINT16	OnClrRDChallengeCDReq(CPlayer *poPlayer);

    UINT16	OnOpenWorshipTabReq(CPlayer *poPlayer, UINT8 byWorshipType);
    UINT16	OnWorshipReq(CPlayer *poPlayer, UINT8 byWorshipType, UINT8 byWorshipTypeLevel);

    UINT16	OnActivityNotifyReq(CPlayer *poPlayer, UINT8 byActivityType, UINT8 byNotify);

    UINT16	OnOpenDogFecesReq(CPlayer *poPlayer, UINT8 byType);

    UINT16	OnGetActivityCenterReq(CPlayer* poPlayer);

    UINT16	OnGetActivityEncourageInfoReq(CPlayer* poPlayer);
	UINT16	OnGetActivityEncourageInfoAck(CPlayer* poPlayer);
	UINT16	OnRecvActivityEncourageAck(CPlayer* poPlayer, UINT16 wActivityID, TCHAR aszSeriesNo[MAX_SERIES_NO_NUM], UINT16 wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS);
    UINT16	OnRecvActivityEncourageAck(CPlayer* poPlayer, UINT64 qwActivityID, TCHAR aszSeriesNo[MAX_SERIES_NO_NUM], UINT16 wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS);

    UINT16	OnRecvActivityEncourageAck(UINT16 wErrCode);


    UINT16	OnOpenGemMakerTabReq(CPlayer* poPlayer);
	UINT16	OnOpenGemMakerTabExReq(CPlayer* poPlayer);

    UINT16	OnRecvRatingEncourageReq(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byType, UINT16 wChapter);


    UINT16	OnOpenAwakenExchangeTabReq(CPlayer* poPlayer);
    UINT16	OnExchangeStudyReq(CPlayer* poPlayer, UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel);

    UINT16	OnOpenEquipComposeTabReq(CPlayer* poPlayer);
    UINT16	OnEquipComposeReq(CPlayer* poPlayer, UINT16 wEquipLevel, UINT8 byCurColorFlag);
    UINT16	OnRecvComposeEquipReq(CPlayer* poPlayer, UINT16 wEquipLevel);



    // 打开帮派面板
    UINT16		  OnOpenFactionReq( CPlayer* poPlayer );
    // 申请加入帮派
    UINT16		  OnApplyFactionReq( CPlayer* poPlayer, UINT32 dwFactionID);
    // 加入申请帮派
    UINT16		  OnCancelApplyFactionReq( CPlayer* poPlayer, UINT32 dwFactionID );
    // 创建帮派
    UINT16		  OnCreateFactionReq( CPlayer* poPlayer, UINT16 byCreateFactionType, TCHAR aszFactionName[MAX_FACTION_NUM], TCHAR aszFactionDescName[MAX_FACTION_DESC_NUM], UINT32 dwFactionID, UINT16 wErrCode);
    // 查找帮派
    UINT16		  OnSearchFactionReq( CPlayer* poPlayer, TCHAR aszFactionName[MAX_FACTION_NUM]);
    // 打开属于自己帮派
    UINT16		  OnOpenOwnerFactionReq( CPlayer* poPlayer );
    // 打开帮派门供
    UINT16		  OnOpenDoorsTributeFactionReq( CPlayer* poPlayer );
    // 门供帮派
    UINT16		  OnDoorsTributeFactionReq( CPlayer* poPlayer, UINT8 byDoorsTributeFlag);
    // 获取门供帮派奖励
    UINT16		  OnRecvDoorsTributeFactionReq( CPlayer* poPlayer );
    // 帮派踢出
    UINT16		  OnKickedFactionReq( CPlayer* poPlayer, UINT32 dwJoinPlayerID);
    // 设置帮派职位
    UINT16		  OnJobFactionReq( CPlayer* poPlayer, UINT32 dwJoinPlayerID, UINT8 byFlag );
    // 设置帮派基本信息
    UINT16		  OnSetNoticeFactionReq( CPlayer* poPlayer, UINT8 byFlag, TCHAR aszFactionNotice[MAX_FACTION_DESC_NUM]);
    // 帮派解散
    UINT16		  OnDissolveFactionReq( CPlayer* poPlayer );

    // 帮派审核
    UINT16		  OnAuditFactionReq( CPlayer* poPlayer, UINT32 dwFactionNum, DT_FACTION_AUDIT_PLAYER_DATA astAuditPlayerData[DT_FACTION_PLAYER_NUM]);
	UINT16 OnAuditFactionReq2( CPlayer* poPlayer, UINT32 dwAuditPlayerID, UINT8 byAuditWay);
    // 打开帮派申请列表
    UINT16		  OnOpenApplyFactionReq( CPlayer* poPlayer);
    // 打开帮派排名列表
    UINT16		  OnOpenRankFactionReq( CPlayer* poPlayer );
    // 打开帮派审核列表
    UINT16		  OnOpenAuditFactionReq( CPlayer* poPlayer );
	UINT16		  OnOpenAuditFactionAck( CPlayer* poPlayer );
    // 打开帮派日志列表
    UINT16		  OnOpenLogFactionReq( CPlayer* poPlayer );

    // 打开聚魂面板
    UINT16		  OnOpenGetherScienceReq( CPlayer* poPlayer );
    // 聚魂
    UINT16		  OnGetherScienceReq( CPlayer* poPlayer, UINT8 byGatherType);


	// 打开仙露阁
	UINT16 OnOpenBluePavilionReq(CPlayer* poPlayer);
	// 求仙露
	UINT16 OnBegBlueReq(CPlayer* poPlayer);
	// 求仙露改运
	UINT16 OnImproveLuckReq(CPlayer* poPlayer);
	// 求仙露确认祈求结果
	UINT16 OnComfirmBegBlueReq(CPlayer* poPlayer);

    // 领取求仙露奖励
    UINT16 OnGetBlueEncourage(CPlayer* poPlayer);

	//转换职业请求
	UINT16 OnChangeCareerReq(CPlayer* poPlayer, UINT8 byCareerID);
	//自己分职业挑战请求
	UINT16 OnChallengeMyselfReq(CPlayer* poPlayer, UINT8 byCareerID);

	//打开道具兑换面板请求
	UINT16 OnOpenItemExchangeReq( CPlayer* poPlayer );
	//道具兑换请求
	UINT16 OnItemExchangeReq( CPlayer* poPlayer, UINT16 wExchangeIdx );

	//运营活动战斗请求
	UINT16 OnOperatingActivityBattleReq(CPlayer* poPlayer, TCHAR* pszBattleKey);

    // 打开勇气试炼
    UINT16 OnOpenCourageReq(CPlayer* poPlayer);

    //打开创建队伍
    UINT16  OnCourageOpenCreateGroupReq(CPlayer* poPlayer);

    //确认创建队伍
    UINT16  OnCourageCreateGroupReq(CPlayer* poPlayer,UINT8 byJoinType,DT_INSTANCE_ID wInstanceID);

    //加入队伍或者协助队伍
    UINT16  OnCourageJoinGroupReq(CPlayer* poPlayer,UINT8 byBattleType,UINT32 dwGroupID);
    
    //随机加入队伍
    UINT16  OnCourageRandomJoinGroupReq(CPlayer* poPlayer);

    //添加或者更改伙伴
    UINT16  OnCourageOperateHeroReq(CPlayer* poPlayer);

    //确认添加或者更改伙伴
    UINT16  OnCourageConfirmHeroReq(CPlayer* poPlayer,UINT16 dwKindID);

    //踢掉队员
    UINT16  OnCourageFireGroupMemberReq(CPlayer* poPlayer,UINT32 dwGroupID,UINT32 dwPlayID);

    //购买试炼
    UINT16  OnCourageBuyPractiseTimesReq(CPlayer* poPlayer);

    //打开购买试炼
    UINT16  OnCourageOpenShopReq(CPlayer* poPlayer);

    //玩家准备
    UINT16  OnCouragePlayerReadyReq(CPlayer* poPlayer,UINT8 byTpye);

    //玩家开始
    UINT16 OnCouragPlayerStartReq(CPlayer* poPlayer);

    //玩家离开
    UINT16 OnCouragLeaveGroupReq(CPlayer* poPlayer);

    //玩家刷新队伍
    UINT16 OnCouragRefreshGroup(CPlayer* poPlayer);

    //进入队伍界面
    UINT16  OnCourageEnterGroupReq(CPlayer* poPlayer);

    //离开勇气试炼
    UINT16 OnLeaveCourageReq(CPlayer *poPlayer);

    //离开勇气试炼
    UINT16 OnCourageLeaveBattleReq(CPlayer *poPlayer);

    //接收准备消息
    UINT16 OnCouragReceiveReady(CPlayer *poPlayer);

    //接收刷新消息
    UINT16 OnCouragReceiveRefresh(CPlayer *poPlayer);

    //勇气战斗请求
    UINT16 OnCourageBattleReq(CPlayer *poPlayer, UINT8 byIndex);

    //领取勇气奖励
    UINT16 OnCourageGetAwardReq(CPlayer* poPlayer, UINT32 dwIndex = 0);

    //勇气战斗结果
    UINT16 OnCouragGetBattleResultReq(CPlayer* poPlayer);

	// 获取好友系统玩家列表
	UINT16 OnGetFriendSysPlayerListAck(CPlayer* poPlayer, UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo);
	// 获取好友系统玩家列表
	UINT16 OnGetFriendSysPlayerListAck2(CPlayer* poPlayer, UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo);

	// 删除好友
	UINT16 OnDelFriendAck(CPlayer* poPlayer, UINT32 dwFirendPlayerID);

	// 查找玩家
	UINT16 OnSearchPlayerAck(CPlayer* poPlayer, TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo);

	// 查找玩家
	UINT16 OnSearchPlayerAck2(CPlayer* poPlayer, TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo);


	// 查看玩家信息
	UINT16 OnViewPlayerAck(CPlayer* poPlayer, UINT32 dwPlayerID);

	// 添加好友
	UINT16 OnAddFriendAck(CPlayer* poPlayer, CPlayer *poTargetPlayer);

	// 送鲜花
	UINT16 OnSendFlowerAck(CPlayer* poPlayer, CPlayer* poTargetPlayer, UINT32 dwSendFlowerNum);

	// 获取某玩家鲜花信息和收花记录
	UINT16 OnGetFlowerByPlayerIDAck(CPlayer* poPlayer);

	// 获取与某玩家的私聊记录
	UINT16 OnGetPrivateMsgLogListAck(CPlayer* poPlayer, UINT32 dwRecverID, UINT32 dwStartIdx, UINT8 byGetMsgNum);

	// 发送私聊消息
	UINT16 OnSendPrivateMsgAck(CPlayer* poPlayer, CPlayer* poTargetPlayer, TCHAR aszMsgContent[PRIVATE_MSG_CONTENT_LEN]);

	// 删除私聊记录
	UINT16 OnDelPrivateMsgAck(CPlayer* poPlayer, UINT32 dwPlayerID);

	// 拉入黑名单
	UINT16 OnAddToBlackListAck(CPlayer* poPlayer, CPlayer *poTargetPlayer);

	// 移除黑名单
	UINT16 OnMoveFromBlackListAck(CPlayer* poPlayer, UINT32 dwPlayerID);

    // 打开送花面板
    UINT16 OnOpenSendFlowerTabAck(CPlayer* poPlayer, UINT32 dwRecvPlayerID);

    // 获取鲜花榜
    UINT16 OnGetFlowerRankingAck(CPlayer* poPlayer);

	// 获取发离线消息的玩家列表
	UINT16 OnGetSendOfflineMsgPlayerListAck(CPlayer* poPlayer);  

	// 读取某个发方玩家的离线消息请求
	UINT16 OnReadOfflineMsgAck(CPlayer* poPlayer, UINT32 dwPlayerID); 

	//打开珍品阁请求
	UINT16 OnOpenRareItemAck(CPlayer* poPlayer );

	//兑换珍品请求
	UINT16 OnExchangeRareItemAck(CPlayer* poPlayer, UINT32 byGoodID);

	//珍品阁升级请求
	UINT16 OnOpenUpgradeRareItemAck(CPlayer* poPlayer );

	//升级确定请求
	UINT16 OnUpgradeRareItemAck(CPlayer* poPlayer );
	
	//打开武堂应答
	UINT16 OnOpenGymnasiumAck( CPlayer* poPlayer );

	//学习武堂技能应答
	UINT16 OnLearnGymnasiumSkillAck( CPlayer* poPlayer, UINT32 dwSkillID );

	//打开武堂升级应答
	UINT16 OnOpenGymnasiumUpgradeAck( CPlayer* poPlayer );

	//确认武堂升级应答
	UINT16 OnGymnasiumUpgradeAck( CPlayer* poPlayer );

	//打开暗部
	UINT16 OnOpenShadeAck( CPlayer* poPlayer );

	//打开黄金模式
	UINT16 OnOpenGoldModeAck( CPlayer* poPlayer );

	//打开暗部升级
	UINT16 OnOpenShadeUpgradeAck( CPlayer* poPlayer );

	//暗部升级
	UINT16 OnShadeUpgradeAck(CPlayer* poPlayer );
	//打开捉妖
	UINT16 OnOpenHuntingAck(CPlayer* poPlayer );
	//寻找妖怪
	UINT16 OnFindMonsterAck(CPlayer* poPlayer, PKT_CLIGS_FIND_MONSTER_REQ& oReq);
	////更换妖怪
	//UINT16 OnChangeMonsterAck( CPlayer* poPlayer);
	//进入战斗
	UINT16 OnHuntingBattleAck(CPlayer* poPlayer);
    //进入战斗
    UINT16 OnHuntingBattleAck2(CPlayer* poPlayer);
	//进入战斗
	UINT16 OnHuntingBattleAck3(CPlayer* poPlayer);
	//换档奖励通知
	UINT16 ChangeSchedule(CPlayer* poPlayer, UINT32 dwCurSchedule);
	//打开温泉应答
	UINT16 OnHotSpringAck( CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY );
	//选酒
	UINT16 OnSelectWineAck(CPlayer* poPlayer, UINT32 dwWineID);
	//喝酒
	UINT16 OnDrinkWineAck(CPlayer* poPlayer);
	//移动位置
	UINT16 OnDrinkWineMoveAck(CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY);
	//离开温泉
	UINT16 OnLeaveHotSpringAck(CPlayer* poPlayer);

	UINT16 OnChangeOtherAck( CPlayer* poPlayer, UINT32 dwSendPlayerID );
	UINT16 OnGiveWineAck( CPlayer* poPlayer );
	UINT16 OnClickSelectWineAck( CPlayer* poPlayer, UINT8 byDrinkWIneState);
	//UINT16 OnFirstFindMonsterAck( CPlayer* poPlayer );
	UINT16 OnSureGiveWineAck( CPlayer* poPlayer );
	UINT16 OnClosedMonsterAck( CPlayer* poPlayer, UINT8 byChoice );
	UINT16 OnFactionPlayerMarkAck(CPlayer* poPlayer);
	UINT16 OnOpenLeaveFactionAck(CPlayer* poPlayer);
	UINT16 OnOpenFactionUpgradeAck(CPlayer* poPlayer);
	UINT16 OnFactionUpgradeAck(CPlayer* poPlayer);
	UINT16 GetFactionBattleLogAck( CPlayer* poPlayer );
	UINT16 OpenFindDrinkAck( CPlayer* poPlayer );
// 
    // 装备精通：打开装备精通面板
    UINT16 OnOpenEquipMasterAck(CPlayer& oPlayer);  

    // 装备精通：激活装备精通
    UINT16 OnActivateEquipMasterAck(CPlayer& oPlayer, BYTE byEquipKind);

    // 装备精通：突破
    UINT16 OnBreakMasterAck(CPlayer& oPlayer, BYTE byEquipKind);

    // 装备精通：培养精通之书
    UINT16 OnGetMasterBookAck(CPlayer& oPlayer);  

    // 装备精通：刮开精通经验书面板
    UINT16 OnWipeMasterBookAck(CPlayer& oPlayer);  

    // 装备精通：出千
    UINT16 OnMasterBookCheatAck(CPlayer& oPlayer);

    // 装备精通：确认使用精通经验书
    UINT16 OnConfirmMasterBookAck(CPlayer& oPlayer);

	// 勇气战斗应答
	UINT16 OnCourageBattleAck(CPlayer* poPlayer);
	
	UINT16 OpenFactionBuildGroupAck(CPlayer* poPlayer);

	UINT16 HotSpringRewardNtf(CPlayer* poPlayer);

	UINT16 OnHotSpringOverRewardAck( CPlayer* poPlayer );

	UINT16 HuntingIntegralAck(CPlayer* poPlayer);

	UINT16 OnIgnorePromptAck( CPlayer* poPlayer, UINT8 byActiviID );

	UINT16 OnOpenLeaveHotSpring(CPlayer* poPlayer);

	UINT16 OnAuditAllRefuseAck(CPlayer* poPlayer);

	UINT16 OnOpenRecvFlowerTabAck(CPlayer* poPlayer);
	UINT16 OnAcceptFlowerAck(CPlayer* poPlayer, UINT32 dwSendPlayerID, UINT64 qwSendTime);
	UINT16 OnOpenSendFlowerTabExAck(CPlayer* poPlayer);
	UINT16 OnSendFlowerExAck(CPlayer* poPlayer, CPlayer *poTargetPlayer, UINT32 dwKindID);
	UINT16 OnBuyRareItemAck(CPlayer* poPlayer, UINT32 dwGoodsID, UINT8 byLeftTime);
	UINT16 OnLearnGymnasiumSkillAck2(CPlayer* poPlayer, UINT32 dwSkillID, UINT8 bySkillCurLevel);
	UINT16 OnFactionPlayerInfoAck(CPlayer* poPlayer);
	UINT16 OnOpenChangeIconAck(CPlayer* poPlayer);
	UINT16 OnChangeIconAck(CPlayer* poPlayer, UINT8 byIconID);
	UINT16 OnOpenFindBackDoorsTributeAck(CPlayer* poPlayer);
	UINT16 OnFindBackDoorsTributeAck(CPlayer* poPlayer, UINT8 byFindBackType);
	UINT16 OnOpenEmbattleAck2(CPlayer* poPlayer);
	UINT16 OnUpgradeFormationAck(CPlayer* poPlayer, UINT8 byFormationID, DT_FORMATION_DATA& stFormation);
	UINT16 OnRegisterFactionNameAck(CPlayer* poPlayer, CHAR* pszDspName);
	UINT16 OnOpenFormationScienceAck(CPlayer* poPlayer);

    // gvg：申请参与门派跨服战
    UINT16 OnApplyGvGAck(CPlayer& oPlayer);


	UINT16 OnUpgradeFormationScienceAck(CPlayer* poPlayer, UINT8 byFormationID);
	UINT16 OnGetExpeienceHotSpringAck(CPlayer* poPlayer, UINT32 dwExprienceValue);
	UINT16 OnOpenFactionSkillAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerPanelAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerInfoAwakenAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerInfoSkillAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerInfoGodWeaponAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerInfoJewelryAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerInfoEquipAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerInfoSoulAck(CPlayer* poPlayer);
	UINT16 OnOpenPlayerInfoPetAck(CPlayer* poPlayer);
	UINT16 OnDressJewelryAck(CPlayer* poPlayer, PKT_CLIGS_DRESS_JEWELRY_REQ& pReq);
	UINT16 OnOpenHeroSelectPanelAck(CPlayer* poPlayer);
	UINT16 OnOpenJewlryGenerateAck(CPlayer* poPlayer);
	UINT16 OnJewlryGenerateAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_GENERATE_REQ& stReq);
	UINT16 OnJewlryGenerateInfoAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ& stReq);
	UINT16 OnJewlryStrengthAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_STRENGTH_REQ& stReq);
	UINT16 OnJewelryResolveAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_RESOLVE_REQ& stReq);
	UINT16 OnOpenJewelryStrengthAck(CPlayer* poPlayer);
	UINT16 OnJewelryCompareAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_COMPARE_REQ& stReq);
	UINT16 OnJewelryExtStrengAck(CPlayer* poPlayer, PKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ& stReq);
	UINT16 OnOpenJewelryResolve(CPlayer* poPlayer, PKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ& stReq);

    UINT16 OnOpenGVGActivityAck(CPlayer* poPlayer);
    UINT16 OnGVGApplyAck(CPlayer* poPlayer);
    UINT16 OnGVGMoneyGameAck(CPlayer* poPlayer);
    UINT16 OnGVGChaosGameAck(CPlayer* poPlayer);
    UINT16 OpenGVG16To8To4Ack(CPlayer* poPlayer);
    UINT16 OpenGVGFinalWarAck(CPlayer* poPlayer);
    UINT16 OpenGVGChampionAck(CPlayer* poPlayer);
    UINT16 OpenGVGNotOpenMsgAck(CPlayer* poPlayer);

	UINT16 OnOpenGVGFormationAck(CPlayer* poPlayer);	
	UINT16 OnOpenAlreadApplayAck(CPlayer* poPlayer);
	UINT16 OnOpenGVGLastResultAck(CPlayer* poPlayer);
	UINT16 OnGVGOpenMyGambleAck(CPlayer* poPlayer);
	UINT16 OnGVGOpenGambleAck(CPlayer* poPlayer, PKT_CLIGS_GVG_OPEN_GAMBLE_REQ& stReq);
	UINT16 OnGVGOpenRuleAck(CPlayer* poPlayer);
    UINT16 OnGVGGambleAck(CPlayer* poPlayer, PKT_CLIGS_GVG_GAMBLE_REQ& stReq);
    UINT16 OnOpenMyFactionGvGLogAck(CPlayer* poPlayer);
	UINT16 OnOpenGvGLogAck(CPlayer* poPlayer, UINT32 dwFactionID);
    UINT16 OnSeeGVGVideoAck(CPlayer &oPlayer, UINT64 qwVideoID, BYTE byVideoNum);
    UINT16 OnEndGvGVideoAck(CPlayer &oPlayer, UINT64 qwVideoID, BYTE byEndType);
    UINT16 OnShowGvGMVPAck(UINT64 qwVideoID);

	UINT16 OnOpenGVGMoneyRankAck(CPlayer* poPlayer);
	UINT16 OnReadFormationPlayerTipAck(CPlayer* poPlayer);
	UINT16 OnGvGFactionEmbattleAck(CPlayer* poPlayer, PKT_CLIGS_GVG_FACTION_EMBATTLE_REQ& stReq);
	UINT16 OnOpenGvGiftAck(CPlayer* poPlayer);
	UINT16 OnGetGvGiftAck(CPlayer* poPlayer, PKT_CLIGS_GET_GVG_GIFT_REQ& stReq);
	UINT16 OnSelectGroupTypeAck(CPlayer* poPlayer, PKT_CLIGS_SELECT_GROUP_TYPE_REQ& stReq);
	UINT16 OnGvGFactionApplayAck(CPlayer* poPlayer);
	UINT16 OnReadBattleLogAck(CPlayer* poPlayer, PKT_CLIGS_READ_BATTLE_LOG_REQ& stReq);

	// 战魂系统
	UINT16 OnSoulRepInfoAck(CPlayer* poPlayer, PKT_CLIGS_SOULREP_INFO_REQ& rMsgReq);
	UINT16 OnProductSoulAck(CPlayer* poPlayer, PKT_CLIGS_PRODUCT_SOUL_REQ& rMsgReq);
	UINT16 OnEquipSoulAck(CPlayer* poPlayer, PKT_CLIGS_HERO_EQUIP_SOUL_REQ& rMsgReq);
	UINT16 OnUnDressSoulAck(CPlayer* poPlayer, PKT_CLIGS_HERO_UNDRESS_SOUL_REQ& rMsgReq);
	UINT16 OnReplaceSoulAck(CPlayer* poPlayer, PKT_CLIGS_HERO_REPLACE_SOUL_REQ& rMsgReq);
	UINT16 OnOpenDeourSoulAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_DEVOUR_SOUL_REQ& stReq);
	UINT16 OnDeourSoulAck(CPlayer* poPlayer, PKT_CLIGS_DEVOUR_SOUL_REQ& stReq);
	UINT16 OnOpenReflashSoulAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_REFLASH_SOUL_REQ& stReq);
	UINT16 OnReflashSoulAck(CPlayer* poPlayer, PKT_CLIGS_REFLASH_SOUL_REQ& stReq);
	UINT16 OnSaveReflashSoulAck(CPlayer* poPlayer, PKT_CLIGS_SAVE_REFLASH_SOUL_REQ& stReq);
	UINT16 OnSoulExchangeInfoReq(CPlayer* poPlayer, PKT_CLIGS_SOULEXCHANGE_INFO_REQ& stReq);
	UINT16 OnExchangeSoulReq(CPlayer* poPlayer, PKT_CLIGS_EXCHANGE_SOUL_REQ& stReq);

	//勋章
	UINT16 OnOpenMyMedalAck(CPlayer*);
    //勋章
    UINT16 OnViewFactionMembersAck(UINT32 dwFactionID);
	//外传
	UINT16 OnOpenMapAck(CPlayer*);
	UINT16 OnOpenOutBoundMapAck(CPlayer*, PKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ& );
	UINT16 OnOutBoundBattleAck(CPlayer*, PKT_CLIGS_OUT_BOUND_BATTLE_REQ&);
	UINT16 OnTestAck(CPlayer*, PKT_CLIGS_TEST_REQ&);
	// 宠物
	UINT16 OnOpenPetRepAck(CPlayer* poPlayer, PKT_CLIGS_PETREP_INFO_REQ& stReq);
	UINT16 OnProductPetAck(CPlayer* poPlayer, PKT_CLIGS_PRODUCT_PET_REQ& stReq);
	UINT16 OnOpenRefinePetAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_REFINEPET_REQ& stReq);
	UINT16 OnRefinePetAck(CPlayer* poPlayer, PKT_CLIGS_REFINEPET_REQ& stReq);
	UINT16 OnOpenPetPanel(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_PANEL_REQ& stReq);
	UINT16 OnOpenStarPanelAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_STAR_PANEL_REQ& stReq);
	UINT16 OnUpPetStarAck(CPlayer* poPlayer, PKT_CLIGS_UP_STAR_LV_REQ& stReq);
	UINT16 OnOpenPetFeedAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_PETFEED_REQ& stReq);
	UINT16 OnFirstDrawAck(CPlayer* poPlayer, PKT_CLIGS_FIRST_DRAW_REQ& stReq);
	UINT16 OnResetDrawAck(CPlayer* poPlayer, PKT_CLIGS_RESET_DRAW_REQ& stReq);
	UINT16 OnConfirmFeedAck(CPlayer* poPlayer, PKT_CLIGS_CONFIRM_FEED_REQ& stReq);
	UINT16 OnPetMerge(CPlayer* poPlayer, PKT_CLIGS_MERGE_PET_REQ& stReq);
	UINT16 OnOpenPetStoreAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_STORE_REQ& stReq);
	UINT16 OnBuyPetStoreItemAck(CPlayer* poPlayer, PKT_CLIGS_BUY_PET_STORE_ITEM_REQ& stReq);
	UINT16 OnOpenWashQualityAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_WASH_QUALITY_REQ& stReq);
	UINT16 OnWashQualityAck(CPlayer* poPlayer, PKT_CLIGS_WASH_QUALITY_REQ& stReq);
	UINT16 OnChangePetStateAck(CPlayer* poPlayer, PKT_CLIGS_CHANGE_PET_STATE_REQ& stReq);
	UINT16 OnOpenInheritPetAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_INHERIT_REQ& stReq);
	UINT16 OnPetInheritAck(CPlayer* poPlayer, PKT_CLIGS_PET_INHERIT_REQ& stReq);
	UINT16 OnOpenPlayerPetPanel(CPlayer* poPlayer, PKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ& stReq);
	UINT16 OnRequestAllPetInfo(CPlayer* poPlayer, PKT_CLIGS_REQUEST_ALL_PET_INFO_REQ& stReq);
	UINT16 OnOpenStoreAck(CPlayer* poPlayer);
	UINT16 OnExchangeGoodsAck(CPlayer* poPlayer, PKT_CLIGS_EXCHANGE_GOODS_REQ& stReq);
	UINT16 OnOpenPetRuleAck(CPlayer* poPlayer);
	

    // 飞升
    UINT16 OnOpenEquipMakeAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_EQUIP_MAKE_REQ& stReq);
    UINT16 OnEquipMakeAck(CPlayer* poPlayer, PKT_CLIGS_EQUIP_MAKE_REQ& stReq);

    UINT16 OnUpgradeHeroAck(CPlayer* poPlayer, PKT_CLIGS_UPGRADE_HERO_REQ& stReq);
    UINT16 OnOpenFlyAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_FLY_REQ& stReq);
    UINT16 OnFlyAck(CPlayer* poPlayer, PKT_CLIGS_FLY_REQ& stReq);

	UINT16 OnOpenSoulUpdateTypeAck(CPlayer*, PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ& stReq);
	UINT16 OnSoulUpdateTypeAck(CPlayer*, PKT_CLIGS_SOUL_UPDATE_TYPE_REQ& stReq);

	UINT16 OnOpenNeiDanAck(CPlayer* poPlayer, PKT_CLIGS_OPEN_NEIDAN_REQ& stReq);
	UINT16 OnDressNeiDanAck(CPlayer* poPlayer, PKT_CLIGS_DRESS_NEIDAN_REQ& stReq);
	UINT16 OnUpdataNeiDanAck(CPlayer* poPlayer, PKT_CLIGS_UPDATA_NEIDAN_REQ& stReq);
	UINT16 OnOpenNeiDanInstanceAck(CPlayer* );
	UINT16 OnNeiDanBattleAck(CPlayer*, PKT_CLIGS_NEIDAN_BATTLE_REQ& stReq);
	UINT16 OnRecvNeiDanAwardAck(CPlayer*, PKT_CLIGS_RECV_NEIDAN_AWARD_REQ& stReq);
	UINT16 OnSkipNeiDanBattleAck(CPlayer*, PKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ& stReq);
protected:
	VOID	FillBiaoAckInfo( PKT_CLIGS_FLUSH_BIAO_QUALITY_ACK &ack, CPlayer &oPlayer );
	UINT16	OnPKBattleReq5(CPlayer *poMyself, CPlayer *poEnemy);
    UINT16	OnPKBattleReq3(CPlayer *poMyself, CPlayer *poEnemy);
	UINT16	OnPKBattleReq4(CPlayer *poMyself, CPlayer *poEnemy);
	UINT16  PlunderAck5(UINT16 wErrCode, CPlayer* poActive, CPlayer* poPassive = NULL, CPlayer* poCapture = NULL);
    UINT16  PlunderAck3(UINT16 wErrCode, CPlayer* poActive, CPlayer* poPassive = NULL, CPlayer* poCapture = NULL);
	UINT16  PlunderAck4(UINT16 wErrCode, CPlayer* poActive, CPlayer* poPassive = NULL, CPlayer* poCapture = NULL);
	UINT16	OnBeginJiebiaoReq5( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat );
    UINT16	OnBeginJiebiaoReq3( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat );
	UINT16	OnBeginJiebiaoReq4( CPlayer* poPlayer, UINT32 dwEnemyID, UINT8 byCount, UINT64 &qwBattleIndex, BOOL &bRepeat );

	UINT16	OnRdchallengeReq5(CPlayer *poPlayer, CPlayer * poEnemyPlayer);
    UINT16	OnRdchallengeReq3(CPlayer *poPlayer, CPlayer * poEnemyPlayer);
	UINT16	OnRdchallengeReq4(CPlayer *poPlayer, CPlayer * poEnemyPlayer);
	UINT16	ChallengeAck5(UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy);
    UINT16	ChallengeAck3(UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy);
	UINT16	ChallengeAck4(UINT16 wErrCode, CPlayer* poPlayer, CPlayer* poEnemy);
	UINT16	BattleDataAck5( CPlayer*poPlayer, DT_BATTLE_DATA5 &data, BOOL bHaveData );
    UINT16	BattleDataAck3( CPlayer*poPlayer, DT_BATTLE_DATA3 &data, BOOL bHaveData );
	UINT16	BattleDataAck4( CPlayer*poPlayer, DT_BATTLE_DATA4 &data, BOOL bHaveData );


};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

