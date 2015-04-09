#include "gtprocessor.h"
#include <net/db/dbpktbuilder.h>
#include <net/gt/gtpktbuilder.h>
#include <net/recharge/rcclient.h>
#include <net/cli/usermgr.h>
#include <framework/gsconfig.h>
#include <framework/gsapi.h>
#include <commondef.h>
#include <sdutil.h>
#include <errdef.h>
#include <logic/player/playermgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/player/playergetdatamgr.h>
#include <common/server/utility.h>
#include <logic/other/phystrengthmgr.h>
#include <logic/race/racepropmgr.h>
#include "net/recharge/rechargeclient.h"
#include "net/recharge/rechargepktbuilder.h"
#include "net/recharge/rechargeclient.h"
#include "logic/other/playerbattledatarecordmgr.h"
#include "logic/other/playerbattledata2recordmgr.h"
#include <logic/activity/bossbattlemgr.h>
#include <logic/activityencourage/activityencouragemgr.h>
#include <logic/faction/faction.h>
#include <logic/friendsys/privatemsgmgr.h>
#include <logic/instance/instancemgr.h>
#include <logic/other/state.h>
#include <logic/race/race.h>
#include <logic/race/racemgr.h>
#include <logic/plunder/plunder.h>
#include <logic/yabiao/yabiaoinfo.h>
#include <logic/build/build.h>
#include <logic/activityencourage/activityencourage.h>
#include <logic/item/itemmgr.h>
#include <logic/player/player.h>
#include <logic/faction/factionmgr.h>
#include <logic/activity/couragemgr.h>
#include "logic/other/phystrength.h"
#include <logic/http/chatencouragehttpcmd.h>
#include "logic/http/httpmgr.h"
#include "logic/lua/luamgr.h"
#include "logic/chat/wordfilter.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/soul/playersoul.h"

#define CHECK_ON_REQ(PACKET) \
    if( sizeof(PACKET) != dwLen )   \
    {\
        return FALSE;\
    }\
\
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;\
    UINT32 dwTransID = pPipeHeader->dwTransID;\
\
    PACKET* pReq = (PACKET*)pBody;\
    CUser* poUser = (CUser*)pUser;\
    CPlayer* poPlayer = poUser->GetPlayer();\
    if(!poPlayer)\
    {\
        return FALSE;\
    }

#define SEND_ACK \
    if(INVALID_MSGID == wMsgID)\
    {\
        return FALSE;\
    }\
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

CGTProcessor::CGTProcessor()
{
    Init() ;
}

CGTProcessor::~CGTProcessor()
{

}

CSDProtocol* CGTProcessor::GetProtocol()
{
    return CProtoGTGS::Instance();
}


BOOL CGTProcessor::Init()
{
    //使用CGTPipeChannel处理的消息,需要在CGTPipeChannel::OnRecvz中特别处理
    RegisterCommand(GTGS_ENTERGS_REQ, CGTProcessor::OnEnterGSReq);

    //使用CUser处理的消息,需要在CGTPipeChannel::OnRecvz中特别处理
    RegisterCommand(GTCOM_CLIENT_DOWN_RPT, CGTProcessor::OnUserDisconnect);
    RegisterCommand(CLIGS_GET_HERO_INFO_REQ, CGTProcessor::OnGetHeroInfoReq);
    RegisterCommand(CLIGS_ENTER_HOME_REQ, CGTProcessor::OnEnterHomeReq);
    RegisterCommand(CLIGS_ENTER_INSTANCE_REQ, CGTProcessor::OnEnterInstanceReq);
    RegisterCommand(CLIGS_BATTLE_REQ, CGTProcessor::OnBattleReq);
    //RegisterCommand(CLIGS_EXIT_INSTANCE_REQ, CGTProcessor::OnExitInstanceReq);
    RegisterCommand(CLIGS_GET_SCENE_INSTANCE_REQ, CGTProcessor::OnGetSceneInstanceReq);
    RegisterCommand(CLIGS_OPEN_SMITHY_REQ, CGTProcessor::OnOpenSmithyReq);
    RegisterCommand(CLIGS_SMITHY_BUY_EQUIP_REQ, CGTProcessor::OnSmithyBuyEquipReq);
    RegisterCommand(CLIGS_EQUIP_STRENGTHEN_REQ, CGTProcessor::OnStrengthenReq);
    RegisterCommand(CLIGS_GET_OPENLIGHT_INFO_REQ, CGTProcessor::OnGetOpenLightInfoReq);
    RegisterCommand(CLIGS_OPENLIGHT_INFO_REQ, CGTProcessor::OnOpenLightReq);
    RegisterCommand(CLIGS_OPEN_RECRUIT_REQ, CGTProcessor::OnOpenRecruitReq);
    RegisterCommand(CLIGS_RECRUIT_HERO_REQ, CGTProcessor::OnRecruitHeroReq);
    RegisterCommand(CLIGS_OPEN_BAG_REQ, CGTProcessor::OnOpenBagReq);
    RegisterCommand(CLIGS_RECV_TASK_ENCOURAGE_REQ, CGTProcessor::OnRecvTaskEncourage);
    RegisterCommand(CLIGS_OPEN_RACE_BUILD_REQ, CGTProcessor::OnOpenRaceBuildReq);
    RegisterCommand(CLIGS_CHALLENGE_REQ, CGTProcessor::OnChallengeReq);
    RegisterCommand(CLIGS_RECV_RANK_ENCOURAGE_REQ, CGTProcessor::OnRecvRankEncourageReq);
    RegisterCommand(CLIGS_OPEN_SHOP_REQ, CGTProcessor::OnOpenShopReq);
    RegisterCommand(CLIGS_BUY_ITEM_REQ, CGTProcessor::OnBuyItemReq);
    RegisterCommand(CLIGS_SELL_ITEM_REQ, CGTProcessor::OnSellItemReq);
    RegisterCommand(CLIGS_EQUIP_REQ, CGTProcessor::OnEquipReq);
    RegisterCommand(CLIGS_QUICK_EQUIP_REQ, CGTProcessor::OnQuickEquipReq);
    RegisterCommand(CLIGS_CLR_PLAYER_DATA_REQ, CGTProcessor::OnClrPlayerDataReq);
    RegisterCommand(CLIGS_EMBATTLE_REQ, CGTProcessor::OnEmbattleReq);
    RegisterCommand(CLIGS_OPEN_EMBATTLE_REQ, CGTProcessor::OnOpenEmbattleReq);
    RegisterCommand(CLIGS_MAKE_ITEM_REQ, CGTProcessor::OnMakeItemReq);
    RegisterCommand(CLIGS_DISBAND_HERO_REQ, CGTProcessor::OnDisbandHeroReq);
    RegisterCommand(CLIGS_INCREASE_BAG_SIZE_REQ, CGTProcessor::OnIncreaseBagSize);
    RegisterCommand(CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ, CGTProcessor::OnOpenEncourageInstanceBoxReq);
    RegisterCommand(CLIGS_ONHOOK_REQ, CGTProcessor::OnHookReq);
    //RegisterCommand(CLIGS_INTERRUPT_ONHOOK_REQ, CGTProcessor::OnInterruptOnhook);
    //RegisterCommand(CLIGS_NOCD_ONHOOK_REQ, CGTProcessor::OnNoCDOnhook);
    //RegisterCommand(CLIGS_ONHOOK_RESULT_REQ, CGTProcessor::OnOnhookResult);
    RegisterCommand(CLIGS_CLR_CHALLENGE_CD_REQ, CGTProcessor::OnClrChallengeCD);
    RegisterCommand(CLIGS_RECV_ONLINE_ENCOURAGE_REQ, CGTProcessor::OnRecvOnlineEncourage);
    RegisterCommand(CLIGS_SYNC_TIME_REQ, CGTProcessor::OnSyncTime);
    RegisterCommand(CLILS_REGISTER_DSPNAME_REQ, CGTProcessor::OnRegisterDspName);
    RegisterCommand(CLILS_USE_GOODS_REQ, CGTProcessor::OnUseGoodsReq);
    RegisterCommand(CLILS_OPEN_LOGIN_ENCOURAGE_REQ, CGTProcessor::OnOpenLoginEncourageReq);
    RegisterCommand(CLILS_RATE_LOGIN_ENCOURAGE_REQ, CGTProcessor::OnRateLoginEncourageReq);
    RegisterCommand(CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ, CGTProcessor::OnPutLoginEncourageIntoBagReq);
    RegisterCommand(CLIGS_GET_RANK_REQ, CGTProcessor::OnGetRankReq);
    RegisterCommand(CLIGS_ACCEPT_TASK_REQ, CGTProcessor::OnAcceptTaskReq);
    RegisterCommand(CLIGS_GIVEUP_TASK_REQ, CGTProcessor::OnGiveUpTaskReq);
    RegisterCommand(CLIGS_GET_RACE_BATTLELOG_REQ, CGTProcessor::OnGetRaceBattleLogReq);

    RegisterCommand(CLIGS_OPEN_CAPTURE_TAB_REQ, CGTProcessor::OnOpenCaptureTabReq);
    RegisterCommand(CLIGS_OPEN_ENEMY_TAB_REQ, CGTProcessor::OnOpenEnemyTabReq);
    RegisterCommand(CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ, CGTProcessor::OnOpenPlunderRecommendPlayerReq);
    RegisterCommand(CLIGS_FREE_CAPTURE_REQ, CGTProcessor::OnFreeCaptureReq);
    RegisterCommand(CLIGS_GET_PLUNDER_BATTLELOG_REQ, CGTProcessor::OnGetPlunderBattleLogReq);
    RegisterCommand(CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ, CGTProcessor::OnRefreshPlunderRecommendReq);
    RegisterCommand(CLIGS_PLUNDER_REQ, CGTProcessor::OnPlunderReq);
    RegisterCommand(CLIGS_DEL_ENEMY_REQ, CGTProcessor::OnDelEnemyReq);
    RegisterCommand(CLIGS_COLLECT_INCOME_REQ, CGTProcessor::OnCollectIncomeReq);
    RegisterCommand(CLIGS_BLEED_CAPTURE_REQ, CGTProcessor::OnBleedCaptureReq);
    RegisterCommand(CLIGS_SET_WORK_TYPE_REQ, CGTProcessor::OnSetWorkTypeReq);
    RegisterCommand(CLIGS_DRIVE_AWAY_OWNER_REQ, CGTProcessor::OnDriveAwayReq);
    RegisterCommand(CLIGS_RECV_LEFT_INCOME_REQ, CGTProcessor::OnRecvLeftIncomeReq);

    RegisterCommand(CLIGS_OPEN_GODWEAPON_REQ, CGTProcessor::OnOpenGodweaponReq);
    RegisterCommand(CLIGS_UPGRADE_GODWEAPON_REQ, CGTProcessor::OnUpgradeGodweaponReq);
    RegisterCommand(CLIGS_FREE_GODWEAPON_REQ, CGTProcessor::OnFreeGodweaponReq);

    RegisterCommand(CLIGS_OPEN_MONEY_TREE_REQ, CGTProcessor::OnOpenMoneyTreeReq);
    RegisterCommand(CLIGS_ROCK_MONEY_TREE_REQ, CGTProcessor::OnRockMoneyTreeReq);

    RegisterCommand(CLIGS_OPEN_AWAKEN_TAB_REQ, CGTProcessor::OnOpenAwakenTabReq);
    RegisterCommand(CLIGS_AWAKEN_REQ, CGTProcessor::OnAwakenReq);
    RegisterCommand(CLIGS_OPEN_DRESS_STUDY_TAB_REQ, CGTProcessor::OnOpenDressStudyTabReq);
    RegisterCommand(CLIGS_DRESS_STUDY_REQ, CGTProcessor::OnDressStudyReq);
    RegisterCommand(CLIGS_STUDY_TO_EXP_REQ, CGTProcessor::OnStudyToExpReq);
    RegisterCommand(CLIGS_STUDY_UPGRADE_REQ, CGTProcessor::OnStudyUpgradeReq);

    RegisterCommand(CLIGS_VISIT_HERO_REQ, CGTProcessor::OnVisitHeroReq);

    RegisterCommand(CLIGS_BUY_PHYSTRENGTH_REQ, CGTProcessor::OnBuyPhystrengthReq);

    RegisterCommand(CLIGS_OPEN_MAINTOWN_REQ, CGTProcessor::OnOpenMaintownReq);
    RegisterCommand(CLIGS_RECV_MAINTOWN_INCOME_REQ, CGTProcessor::OnRecvIncomeReq);
    RegisterCommand(CLIGS_LEVY_MAINTOWN_INCOME_REQ, CGTProcessor::OnLevyIncomeReq);


    //////////////////////////////////////////////////////////////////////////

    RegisterCommand(CLIGS_OPEN_PHYSTRENGTH_REQ, CGTProcessor::OnOpenPhyStrengthReq);

    RegisterCommand(CLIGS_CLR_STRENGTHEN_CD_REQ, CGTProcessor::OnClrStrengthenCDReq);

    //旧版种植
    RegisterCommand(CLIGS_OPEN_CROP_REQ, CGTProcessor::OnOpenCropReq);
    RegisterCommand(CLIGS_OPEN_CROP_FIELD_REQ, CGTProcessor::OnOpenCropFieldReq);
    RegisterCommand(CLIGS_CROP_REQ, CGTProcessor::OnCropReq);
    RegisterCommand(CLIGS_GAIN_CROP_REQ, CGTProcessor::OnGainCropReq);
    RegisterCommand(CLIGS_CLR_CROP_REQ, CGTProcessor::OnClrCropReq);


    RegisterCommand(CLIGS_OPEN_EXPERIENCE_REQ, CGTProcessor::OnOpenExperienceDrugReq);
    RegisterCommand(CLIGS_USER_EXPERIENCE_REQ, CGTProcessor::OnUserExperienceDrugReq);

    RegisterCommand(CLIGS_OPEN_SCIENCE_BUILD_REQ, CGTProcessor::OnOpenScienceBuildReq);
    RegisterCommand(CLIGS_UPDATE_SCIENCE_REQ, CGTProcessor::OnUpdateScienceReq);

    RegisterCommand(CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ, CGTProcessor::OnOpenUpdateAttrPannelReq);
    RegisterCommand(CLIGS_UPDATE_ATTR_REQ, CGTProcessor::OnUpdateAttrReq);


    //////////////////////////////////////////////////////////////////////////
    RegisterCommand(CLIGS_OPEN_MELT_EQUIP_REQ, CGTProcessor::OnOpenMeltEquipReq);
    RegisterCommand(CLIGS_MELT_REQ, CGTProcessor::OnMeltReq);
    RegisterCommand(CLIGS_COMPOSE_REQ, CGTProcessor::OnComposeReq);
    RegisterCommand(CLIGS_GET_ITEM_REQ, CGTProcessor::OnGetItemReq);
    RegisterCommand( CLIGS_GET_ORDER_ID_REQ, CGTProcessor::OnGetOrderIDReq );

    RegisterCommand(CLIGS_SEED_COMPOSE_REQ, CGTProcessor::OnComposeSeedReq);
    RegisterCommand(CLIGS_CHAT_REQ, CGTProcessor::OnChatReq);

    RegisterCommand(CLIGS_OPNE_ACTIVITY_CENTER_REQ, CGTProcessor::OnOpenActivityCenterReq);
    RegisterCommand(CLIGS_OPNE_EAT_BREAD_REQ, CGTProcessor::OnOpneEatBreadReq);
    RegisterCommand(CLIGS_OBTAIN_PHYSTRENGTHEN_REQ, CGTProcessor::OnObtainPhystrengthenReq);
    RegisterCommand(CLIGS_LEAVE_EAT_BREAD_REQ, CGTProcessor::OnOLeaveEatBreadReq);


    //////////////////////////////////////////////////////////////////////////
    //重置爬塔
    RegisterCommand(CLIGS_CLIMB_TOWER_RESET_REQ, CGTProcessor::OnClimbTowerResetReq);
    //爬塔挂机
    RegisterCommand(CLIGS_CLIMB_TOWER_ONHOOK_REQ, CGTProcessor::OnClimbTowerOnHookReq);

    RegisterCommand(CLIGS_OPEN_PAOSHANG_BUILD_REQ, CGTProcessor::OnOpenPaoshangBuildReq);
    RegisterCommand(CLIGS_OPEN_YABIAO_REQ, CGTProcessor::OnOpenYabiaoReq);
    RegisterCommand(CLIGS_BEGIN_YABIAO_REQ, CGTProcessor::OnBeginYabiaoReq);
    RegisterCommand(CLIGS_FLUSH_BIAO_QUALITY_REQ, CGTProcessor::OnFlushBiaoQualityReq);
    RegisterCommand(CLIGS_OPEN_JIEBIAO_REQ, CGTProcessor::OnOpenJiebiaoReq);
    RegisterCommand(CLIGS_BEGIN_JIEBIAO_REQ, CGTProcessor::OnBeginJiebiaoReq);
    RegisterCommand(CLIGS_QUERY_BATTLE_RECORD_REQ, CGTProcessor::OnQueryBattleRecordReq);

    //押镖战报请求
    RegisterCommand(CLIGS_QUERY_YABIAO_BATTLELOG_REQ, CGTProcessor::OnQueryYabiaoBattleLogReq);
    RegisterCommand(CLIGS_QUERY_YABIAO_ENCOURAGE_REQ, CGTProcessor::OnQueryYabiaoEncourageReq);
    RegisterCommand(CLIGS_CLEAR_JIEBIAO_CD_REQ, CGTProcessor::OnClearJiebiaoCDReq);

    ////////////////////////
    RegisterCommand(CLIGS_UPGRADE_JINGJIE_REQ,  CGTProcessor::OnUpgradeJingjieReq);
    RegisterCommand(CLIGS_GEN_GAS_BALL_REQ,  CGTProcessor::OnGenGasBallReq);
    RegisterCommand(CLIGS_UP_GAS_BALL_LEVEL_REQ,  CGTProcessor::OnUpGasBallLevelReq);
    RegisterCommand(CLIGS_COLLECT_GAS_REQ,  CGTProcessor::OnCollectGasReq);
    RegisterCommand(CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ,  CGTProcessor::OnUpgradeSkillActionLevelReq);
    RegisterCommand(CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ,  CGTProcessor::OnSelectSkillActionEffectReq);
    RegisterCommand(CLIGS_OPEN_JINGJIE_TAB_REQ,  CGTProcessor::OnOpenJingjieTabReq);
    RegisterCommand(CLIGS_OPEN_GEN_GAS_TAB_REQ,  CGTProcessor::OnOpenGenGasTabReq);
    RegisterCommand(CLIGS_OPEN_SKILL_TAB_REQ,  CGTProcessor::OnOpenSkillTabReq);
    RegisterCommand(CLIGS_OPEN_VIP_TAB_REQ,  CGTProcessor::OnOpenVipTabReq);
    RegisterCommand(CLIGS_OPEN_CLIMB_TOWER_REQ,  CGTProcessor::OnOpenClimbTowerReq);
    //RegisterCommand(CLIGS_BUY_CLIMB_TOWER_RESET_REQ,  CGTProcessor::OnBuyClimbTowerResetReq);
    RegisterCommand(CLIGS_OPEN_XIANGQIAN_TAB_REQ,  CGTProcessor::OnOpenXiangqianTabReq);
    RegisterCommand( CLIGS_XIANGQIAN_REQ, CGTProcessor::OnXiangqianReq );
    RegisterCommand( CLIGS_CHECK_APPSTORE_ORDER_ID_REQ, CGTProcessor::OnAppExchangeCheckReq );
    RegisterCommand( CLIGS_REMOVE_XIANGQIAN_REQ, CGTProcessor::OnRemoveXiangqianReq );
    RegisterCommand( CLIGS_RESET_ELITE_INSTANCE_REQ, CGTProcessor::OnResetEliteInstanceReq );
    RegisterCommand( CLIGS_GET_ELITE_INSTANCE_REQ, CGTProcessor::OnGetEliteInstanceReq );
    RegisterCommand( CLIGS_ELITE_INSTANCE_ON_HOOK_REQ, CGTProcessor::OnEliteInstanceOnHookReq );

    RegisterCommand( CLIGS_GET_ACTIVITY_PROMPT_REQ, CGTProcessor::OnGetActivityPromptReq );
    RegisterCommand( CLIGS_OPEN_CHAT_REQ, CGTProcessor::OnOpenChatReq );
    RegisterCommand( CLIGS_BLUE_TO_PURPLE_REQ, CGTProcessor::OnBlueToPurpleReq );

    RegisterCommand( CLIGS_PKBATTLE_REQ, CGTProcessor::OnPKBattleReq );
    RegisterCommand( CLIGS_RESOURCES_PROMPT_REQ, CGTProcessor::OnResourcesPromptReq );

    RegisterCommand( ENTER_BOSSB_REQ, CGTProcessor::OnEnterBossbReq );
    RegisterCommand( LEAVE_BOSSB_REQ, CGTProcessor::OnLeaveBossbReq );
    RegisterCommand( BOSSB_MOVE_REQ, CGTProcessor::OnBossbMoveReq );
    RegisterCommand( BOSSB_HURT_REQ, CGTProcessor::OnBossbHurtReq );
    RegisterCommand( OPEN_INSPIRE_TAB_REQ, CGTProcessor::OnOpenInspireTabReq );
    RegisterCommand( INSPIRE_POWER_REQ, CGTProcessor::OnInspirePowerReq );
    RegisterCommand( BOSSB_RELIVE_REQ, CGTProcessor::OnBossBReliveReq );
    RegisterCommand( BOSSB_GET_LAST_BOSSB_INFO_REQ, CGTProcessor::OnGetLastBossBInfoReq );
    RegisterCommand( CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ, CGTProcessor::OnOpenBossBBLReq );

    RegisterCommand( CLILS_OPEN_RECHARGE_ENCOURAGE_REQ, CGTProcessor::OnOpenRechargeEncourageReq );
    RegisterCommand( CLIGS_RECV_RECHARGE_ENCOURAGE_REQ, CGTProcessor::OnRecvRechargeEncourageReq );
    RegisterCommand( CLILS_OPEN_SCORE_ENCOURAGE_REQ, CGTProcessor::OnOpenScoreEncourageReq);
    RegisterCommand( CLIGS_RECV_SCORE_ENCOURAGE_REQ, CGTProcessor::OnRecvScoreEncourageReq );

    RegisterCommand( CLIGS_GET_RANDOM_DSPNAME_REQ, CGTProcessor::OnGetRandomDspNameReq );

    RegisterCommand( CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ, CGTProcessor::OnOpenExperienceTransferReq );
    RegisterCommand( CLIGS_EXPERIENCE_TRANSFER_REQ, CGTProcessor::OnExperienceTransferReq );

    RegisterCommand( CLIGS_OPEN_RDCHALLENGE_TAB_REQ, CGTProcessor::OnOpenRdchallengeTabReq );
    RegisterCommand( CLIGS_RDCHALLENGE_REQ, CGTProcessor::OnRdchallengeReq );
    RegisterCommand( CLIGS_CLR_RDCHALLENGE_CD_REQ, CGTProcessor::OnClrRDChallengeCDReq );

    RegisterCommand( CLIGS_OPEN_WORSHIP_TAB_REQ, CGTProcessor::OnOpenWorshipTabReq );
    RegisterCommand( CLIGS_WORSHIP_REQ, CGTProcessor::OnWorshipReq );

    RegisterCommand( CLIGS_ACTIVITY_NOTIFY_REQ, CGTProcessor::OnActivityNotifyReq );
    RegisterCommand( CLIGS_OPEN_DOGFECES_REQ, CGTProcessor::OnOpenDogFecesReq );

    RegisterCommand( CLIGS_GET_ACTIVITY_CENTER_REQ, CGTProcessor::OnGetActivityCenterReq );


    RegisterCommand( CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ, CGTProcessor::OnGetActivityEncourageInfoReq );
    RegisterCommand( CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ, CGTProcessor::OnRecvActivityEncourageReq );
	RegisterCommand( CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2, CGTProcessor::OnGetActivityEncourageInfoReq2 );
	RegisterCommand( CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2, CGTProcessor::OnRecvActivityEncourageReq2 );

    RegisterCommand( CLIGS_OPEN_GEM_MAKER_TAB_REQ, CGTProcessor::OnOpenGemMakerTabReq );
    RegisterCommand( CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ, CGTProcessor::OnOpenGemMakerTabExReq );

    RegisterCommand( CLIGS_RECV_RATING_ENCOURAGE_REQ, CGTProcessor::OnRecvRatingEncourageReq );


    RegisterCommand( CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ, CGTProcessor::OnOpenAwakenExchangeTabReq );
    RegisterCommand( CLIGS_EXCHANGE_STUDY_REQ, CGTProcessor::OnExchangeStudyReq );

    RegisterCommand( CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ, CGTProcessor::OnOpenEquipComposeTabReq );
    RegisterCommand( CLIGS_EQUIP_COMPOSE_REQ, CGTProcessor::OnEquipComposeReq );
    RegisterCommand( CLIGS_RECV_COMPOSE_EQUIP_REQ, CGTProcessor::OnRecvComposeEquipReq );

    RegisterCommand( CLIGS_OPEN_FACTION_REQ, CGTProcessor::OnOpenFactionReq );
    RegisterCommand( CLIGS_APPLY_FACTION_REQ, CGTProcessor::OnApplyFactionReq );

    RegisterCommand( CLIGS_CANCEL_APPLY_FACTION_REQ, CGTProcessor::OnCancelApplyFactionReq );
    RegisterCommand( CLIGS_CREATE_FACTION_REQ, CGTProcessor::OnCreateFactionReq );

    RegisterCommand( CLIGS_SEARCH_FACTION_REQ, CGTProcessor::OnSearchFactionReq );
    RegisterCommand( CLIGS_OPEN_OWNER_FACTION_REQ, CGTProcessor::OnOpenOwnerFactionReq );

    RegisterCommand( CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ, CGTProcessor::OnOpenDoorsTributeFactionReq );
    RegisterCommand( CLIGS_DOORS_TRIBUTE_FACTION_REQ, CGTProcessor::OnDoorsTributeFactionReq );

    RegisterCommand( CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ, CGTProcessor::OnRecvDoorsTributeFactionReq );
    RegisterCommand( CLIGS_KICKED_FACTION_REQ, CGTProcessor::OnKickedFactionReq );

    RegisterCommand( CLIGS_JOB_FACTION_REQ, CGTProcessor::OnJobFactionReq );
    RegisterCommand( CLIGS_SET_NOTICE_FACTION_REQ, CGTProcessor::OnSetNoticeFactionReq );

    RegisterCommand( CLIGS_DISSOLVE_FACTION_REQ, CGTProcessor::OnDissolveFactionReq );
    RegisterCommand( CLIGS_AUDIT_FACTION_REQ, CGTProcessor::OnAuditFactionReq );
    RegisterCommand( CLIGS_AUDIT_FACTION_REQ2, CGTProcessor::OnAuditFactionReq2);
    RegisterCommand( CLIGS_OPEN_APPLY_FACTION_REQ, CGTProcessor::OnOpenApplyFactionReq );

    RegisterCommand( CLIGS_OPEN_RANK_FACTION_REQ, CGTProcessor::OnOpenRankFactionReq );
    RegisterCommand( CLIGS_OPEN_AUDIT_FACTION_REQ, CGTProcessor::OnOpenAuditFactionReq );
    RegisterCommand( CLIGS_OPEN_AUDIT_FACTION_REQ2, CGTProcessor::OnOpenAuditFactionReq2 );
    RegisterCommand( CLIGS_OPEN_LOG_FACTION_REQ, CGTProcessor::OnOpenLogFactionReq );

    RegisterCommand( CLIGS_OPEN_GATHER_SCIENCE_REQ, CGTProcessor::OnOpenGetherScienceReq );
    RegisterCommand( CLIGS_GATHER_SCIENCE_REQ, CGTProcessor::OnGetherScienceReq );

    //////////////////
    RegisterCommand(CLIGS_OPEN_AWAKEN_TAB_REQ2, CGTProcessor::OnOpenAwakenTabReq2);
    RegisterCommand(CLIGS_AWAKEN_REQ2, CGTProcessor::OnAwakenReq2);
    RegisterCommand(CLIGS_OPEN_DRESS_STUDY_TAB_REQ2, CGTProcessor::OnOpenDressStudyTabReq2);
    RegisterCommand(CLIGS_DRESS_STUDY_REQ2, CGTProcessor::OnDressStudyReq2);
    RegisterCommand(CLIGS_STUDY_TO_EXP_REQ2, CGTProcessor::OnStudyToExpReq2);
    RegisterCommand(CLIGS_STUDY_UPGRADE_REQ2, CGTProcessor::OnStudyUpgradeReq2);
    RegisterCommand(CLIGS_OPEN_RESOLVE_TRIGRAM_REQ, CGTProcessor::OnOpenResolveTrigramReq);
    RegisterCommand(CLIGS_RESOLVE_TRIGRAM_REQ, CGTProcessor::OnResolveTrigramReq);

    RegisterCommand(CLIGS_OPEN_BLUE_PAVILION_REQ, CGTProcessor::OnOpenBluePavilionReq);
    RegisterCommand(CLIGS_BEGBLUE_REQ, CGTProcessor::OnBegBlueReq);
    RegisterCommand(CLIGS_IMPROVELUCK_REQ, CGTProcessor::OnImproveLuckReq);
    RegisterCommand(CLIGS_COMFIRM_BEGBLUE_REQ, CGTProcessor::OnComfirmBegBlueReq);
    RegisterCommand(CLIGS_GET_BLUE_ENCOURAGE_REQ, CGTProcessor::OnGetBlueEncourage);

    RegisterCommand(CLIGS_CHANGE_CAREER_REQ, CGTProcessor::OnChangeCareerReq);
    RegisterCommand(CLIGS_CHALLENGE_MYSELF_REQ, CGTProcessor::OnChallengeMyselfReq);

    RegisterCommand(CLIGS_OPEN_ITEM_EXCHANGE_REQ, CGTProcessor::OnOpenItemExchangeReq);
    RegisterCommand(CLIGS_ITEM_EXCHANGE_REQ, CGTProcessor::OnItemExchangeReq);

    RegisterCommand(GSGT_KICK_OUT_ACK, CGTProcessor::OnKickOutAck);

    RegisterCommand(CLIGS_OPERATING_ACTIVITY_BATTLE_REQ, CGTProcessor::OnOperatingActivityBattleReq);

    RegisterCommand(CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ, CGTProcessor::OnGetFriendSysPlayerListReq);
    RegisterCommand(CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2, CGTProcessor::OnGetFriendSysPlayerListReq2);
    RegisterCommand(CLIGS_DEL_FRIEND_REQ, CGTProcessor::OnDelFriendReq);
    RegisterCommand(CLIGS_SEARCH_PLAYER_REQ, CGTProcessor::OnSearchPlayerReq);
    RegisterCommand(CLIGS_SEARCH_PLAYER_REQ2, CGTProcessor::OnSearchPlayerReq2);
    RegisterCommand(CLIGS_VIEW_PLAYER_REQ, CGTProcessor::OnViewPlayerReq);
    RegisterCommand(CLIGS_ADD_FRIEND_REQ, CGTProcessor::OnAddFriendReq);
    RegisterCommand(CLIGS_OPEN_SEND_FLOWER_TAB_REQ, CGTProcessor::OnOpenSendFlowerTabReq);
    RegisterCommand(CLIGS_SEND_FLOWER_REQ, CGTProcessor::OnSendFlowerReq);
    RegisterCommand(CLIGS_SEND_FLOWER_EX_REQ, CGTProcessor::OnSendFlowerExReq);
    RegisterCommand(CLIGS_GET_FLOWER_BY_PLAYERID_REQ, CGTProcessor::OnGetFlowerByPlayerIDReq);
    RegisterCommand(CLIGS_GET_FLOWER_RANKING_REQ, CGTProcessor::OnGetFlowerRankingReq);

    RegisterCommand(CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ, CGTProcessor::OnGetPrivateMsgLogListReq);
    RegisterCommand(CLIGS_SEND_PRIVATE_MSG_REQ, CGTProcessor::OnSendPrivateMsgReq);
    RegisterCommand(CLIGS_DEL_PRIVATE_MSG_LOG_REQ, CGTProcessor::OnDelPrivateMsgReq);
    RegisterCommand(CLIGS_ADD_TO_BLACKLIST_REQ, CGTProcessor::OnAddToBlacklistReq);
    RegisterCommand(CLIGS_MOVE_FROM_BLACKLIST_REQ, CGTProcessor::OnMoveFromBlacklistReq);
    RegisterCommand(CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ, CGTProcessor::OnGetSendOfflineMsgPlayerListReq);
    RegisterCommand(CLIGS_READ_OFFLINE_MSG_REQ, CGTProcessor::OnReadOfflineMsgReq);

    //勇气试炼
    RegisterCommand(CLIGS_OPEN_COURAGE_REQ, CGTProcessor::OnOpenCourageReq);
    RegisterCommand(CLIGS_COURAGE_CREATE_GROUP_REQ, CGTProcessor::OnCourageCreateGroupReq);
    RegisterCommand(CLIGS_COURAGE_JOIN_GROUP_REQ, CGTProcessor::OnCourageJoinGroupReq);
    RegisterCommand(CLIGS_COURAGE_OPERATE_HERO_REQ, CGTProcessor::OnCourageOperateHeroReq);
    RegisterCommand(CLIGS_COURAGE_CONFIRM_HERO_REQ, CGTProcessor::OnCourageConfirmHeroReq);
    RegisterCommand(CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ, CGTProcessor::OnCourageFireGroupMemberReq);
    RegisterCommand(CLIGS_OPEN_COURAGE_SHOP_REQ, CGTProcessor::OnCouragOpenShopReq);
    RegisterCommand(CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ, CGTProcessor::OnCouragBuyPractiseTimesReq);
    RegisterCommand(CLIGS_COURAGE_PLAYER_READY_REQ, CGTProcessor::OnCouragPlayerReadyReq);
    RegisterCommand(CLIGS_COURAGE_PLAYER_START_REQ, CGTProcessor::OnCouragPlayerStartReq);
    RegisterCommand(CLIGS_COURAGE_LEAVE_GROUP_REQ, CGTProcessor::OnCouragLeaveGroupReq);
    RegisterCommand(CLIGS_COURAGE_GROUP_REFRESH_REQ, CGTProcessor::OnCourageRefreshGroupReq);
    RegisterCommand(CLIGS_COURAGE_ENTER_GROUP_REQ, CGTProcessor::OnCourageEnterGroupReq);
    RegisterCommand(CLIGS_LEAVE_COURAGE_REQ, CGTProcessor::OnLeaveCourageReq);
    RegisterCommand(CLIGS_COURAGE_GET_AWARD_REQ, CGTProcessor::OnCourageGetAwardReq);
    RegisterCommand(CLIGS_COURAGE_GET_BATTLE_RESULT_REQ, CGTProcessor::OnCouragGetBattleResultReq);
    RegisterCommand(CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ, CGTProcessor::OnCourageOpenCreateGroupReq);
    RegisterCommand(CLIGS_LEAVE_COURAGE_BATTLE_REQ, CGTProcessor::OnCourageLeaveBattleReq);
    RegisterCommand(CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ, CGTProcessor::OnCourageRandomJoinGroupReq);
    

    // 装备精通
    RegisterCommand(CLIGS_OPEN_EQUIP_MASTER_REQ,   CGTProcessor::OnOpenEquipMasterReq);
    RegisterCommand(CLIGS_ACTIVATE_MASTER_REQ,     CGTProcessor::OnActivateEquipMasterReq);
    RegisterCommand(CLIGS_BREAK_MASTER_REQ,        CGTProcessor::OnBreakMasterReq);
    RegisterCommand(CLIGS_GET_MASTER_BOOK_REQ,     CGTProcessor::OnGetMasterBookReq);
    RegisterCommand(CLIGS_WIPE_MASTER_BOOK_REQ,    CGTProcessor::OnWipeMasterBookReq);
    RegisterCommand(CLIGS_CONFIRM_MASTER_BOOK_REQ, CGTProcessor::OnConfirmMasterBookReq);
    RegisterCommand(CLIGS_MASTER_CHEAT_REQ,        CGTProcessor::OnCheatMasterBookReq);

    //珍品阁
    RegisterCommand(CLIGS_OPEN_RAREITEMROOM_REQ, CGTProcessor::OnOpenRareItemReq );
    RegisterCommand(CLIGS_BUY_RAREITEM_REQ, CGTProcessor::OnExchangeRareItemReq );
    RegisterCommand(CLIGS_OPEN_UPGRADERAREITEM_REQ, CGTProcessor::OnOpenUpgradeRareItemReq );
    RegisterCommand(CLIGS_UPGRADE_RAREITEM_REQ, CGTProcessor::OnUpgradeRareItemReq );
    //武堂
    RegisterCommand(CLIGS_OPEN_GYMNASIUM_REQ, CGTProcessor::OnOpenGymnasiumReq );
    RegisterCommand(CLIGS_LEARN_GYMNASIUM_SKILL_REQ, CGTProcessor::OnLearnGymnasiumSkillReq );
    RegisterCommand(CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ, CGTProcessor::OnOpenGymnasiumUpgradeReq );
    RegisterCommand(CLIGS_GYMNASIUM_UPGRADE_REQ, CGTProcessor::OnGymnasiumUpgradeReq );
    //暗部
    RegisterCommand(CLIGS_OPEN_SHADE_REQ, CGTProcessor::OnOpenShadeReq);
    RegisterCommand(CLIGS_OPEN_GOLDMODE_REQ, CGTProcessor::OnOpenGoldModeReq);
    RegisterCommand(CLIGS_OPEN_SHADE_UPGRADE_REQ, CGTProcessor::OnOpenShadeUpgradeReq);
    RegisterCommand(CLIGS_SHADE_UPGRADE_REQ, CGTProcessor::OnShadeUpgradeReq);
    RegisterCommand(CLIGS_OPEN_HUNTING_REQ, CGTProcessor::OnOpenHuntingReq);
    RegisterCommand(CLIGS_FIND_MONSTER_REQ, CGTProcessor::OnFindMonsterReq);
    RegisterCommand(CLIGS_HUNTING_BATTLE_REQ, CGTProcessor::OnHuntingBattleReq);
    //温泉
    RegisterCommand(CLIGS_OPEN_HOT_SPRING_REQ, CGTProcessor::OnOpenHotSpringReq);
    RegisterCommand(CLIGS_SELECT_WINE_REQ, CGTProcessor::OnSelectWineReq);
    RegisterCommand(CLIGS_DRINK_WINE_REQ, CGTProcessor::OnDrinkWineReq);
    RegisterCommand(CLIGS_DRINK_WINE_MOVE_REQ, CGTProcessor::OnDrinkWineMoveReq);
    RegisterCommand(CLIGS_LEAVE_HOT_SPRING_REQ, CGTProcessor::OnLeaveHotSpringReq);
    //退位让贤
    RegisterCommand(CLIGS_CHANGE_OTHER_REQ, CGTProcessor::OnChangeOtherReq);
    RegisterCommand(CLIGS_GIVE_WINE_REQ, CGTProcessor::OnGiveWineReq);
    RegisterCommand(CLIGS_CLICK_SELECT_WINE_REQ, CGTProcessor::OnClickSelectWineReq);
    RegisterCommand(CLIGS_SURE_GIVE_WINE_REQ, CGTProcessor::OnSureGiveWineReq);
    RegisterCommand(CLIGS_CLOSED_MONSTER_REQ, CGTProcessor::OnClosedMonsterReq);
    RegisterCommand(CLIGS_FACTION_PLAYER_CHANGE_REQ, CGTProcessor::OnFactionPlayerMarkReq);

    RegisterCommand(CLIGS_OPEN_LEAVE_FACTION_REQ, CGTProcessor::OnOpenLeaveFactionReq);
    RegisterCommand(CLIGS_LEAVE_FACTION_REQ, CGTProcessor::OnLeaveFactionReq);
    RegisterCommand(CLIGS_OPEN_FACTION_UPGRADE_REQ, CGTProcessor::OnOpenFactionUpgradeReq);
    RegisterCommand(CLIGS_FACTION_UPGRADE_REQ, CGTProcessor::OnFactionUpgradeReq);
    RegisterCommand(CLIGS_PLAYER_FACTION_READ_REQ, CGTProcessor::OnGetFactionBattleLogReq);
    RegisterCommand(CLIGS_OPEN_FIND_DRINK_REQ, CGTProcessor::OnOpenFindDrinkReq);

    RegisterCommand(CLIGS_COURAGE_BATTLE_REQ, CGTProcessor::OnCourageBattleReq);
    RegisterCommand(CLIGS_OPEN_FACTION_BUILD_GROUP_REQ, CGTProcessor::OnOpenFactionBuildGroupReq);
    RegisterCommand(CLIGS_HOT_SPRING_OVER_REWARD_REQ, CGTProcessor::OnHotSpringOverRewardReq);

    RegisterCommand(CLIGS_HUNTING_ITEGRAL_REQ, CGTProcessor::OnHuntingIntegralReq);
    RegisterCommand(CLIGS_PROMPT_IGNORE_REQ, CGTProcessor::OnIgnorePromptReq);
    RegisterCommand(CLIGS_OPEN_LEAVE_HOT_SPRING_REQ, CGTProcessor::OnOpenLevelHotSpring);

    RegisterCommand(CLIGS_AUDIT_ALL_REFUSE_REQ, CGTProcessor::OnAuditAllRefuseReq);

    RegisterCommand(CLIGS_OPEN_RECV_FLOWER_TAB_REQ, CGTProcessor::OnOpenRecvFlowerTabReq);
    RegisterCommand(CLIGS_ACCEPT_FLOWER_REQ, CGTProcessor::OnAcceptFlowerReq);
    RegisterCommand(CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ, CGTProcessor::OnOpenSendFlowerTabExReq);

    RegisterCommand(CLIGS_BUY_RAREITEM_REQ2, CGTProcessor::OnBuyRareItemReq);
    RegisterCommand(CLIGS_LEARN_GYMNASIUM_SKILL_REQ2, CGTProcessor::OnLearnGymnasiumSkillReq2);
	RegisterCommand(CLIGS_FACTION_PLAYERS_INFO_REQ, CGTProcessor::OnFactionPlayerInfoReq);
	RegisterCommand(CLIGS_CHANGE_FACTION_ICON_REQ, CGTProcessor::OnChangeIconReq);
	RegisterCommand(CLIGS_OPEN_CHANGE_FACTION_ICON_REQ, CGTProcessor::OnOpenChangeIconReq);
	RegisterCommand(CLIGS_FIND_BACK_DOORSTRIBUTE_REQ, CGTProcessor::OnFindBackDoorsTributeReq);
	RegisterCommand(CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ, CGTProcessor::OnOpenFindBackDoorsTributeReq);

	RegisterCommand(CLIGS_OPEN_EMBATTLE_REQ2, CGTProcessor::OnOpenEmbattleReq2);
	RegisterCommand(CLIGS_UPGRADE_FORMATION_REQ, CGTProcessor::OnUpgradeFormationReq);
	RegisterCommand(CLILS_REGISTER_FACTION_REQ, CGTProcessor::OnRegisterFactionName);
	RegisterCommand(CLIGS_OPEN_FORMATION_SCIENCE_REQ, CGTProcessor::OnOpenFormationScienceReq);


	//VIP礼包功能
	//RegisterCommand(CLIGS_OPEN_VIP_GIFT_REQ, CGTProcessor::OnOpenVipGift);
	//RegisterCommand(CLIGS_GET_VIP_GIFT_REQ, CGTProcessor::OnGetVipGift);

	RegisterCommand(CLIGS_UPGRADE_FORMATION_SCIENCE_REQ, CGTProcessor::OnUpgradeFormationScienceReq);
	RegisterCommand(CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ, CGTProcessor::OnGetExprienceHotSpringReq);
	RegisterCommand(CLIGS_OPEN_FACTION_SKILL_REQ, CGTProcessor::OnOpenFactionSkillReq);

	//人物面板 
	RegisterCommand(CLIGS_OPEN_PLAYER_PANEL_REQ, CGTProcessor::OnOpenPlayerPanelReq);
	RegisterCommand(CLIGS_PLAYER_INFO_AWAKEN_REQ, CGTProcessor::OnOpenPlayerInfoAwakenReq);
	RegisterCommand(CLIGS_PLAYER_INFO_SKILL_REQ, CGTProcessor::OnOpenPlayerInfoSkillReq);
	RegisterCommand(CLIGS_PLAYER_INFO_GODWEAPON_REQ, CGTProcessor::OnOpenPlayerInfoGodWeaponReq);
	RegisterCommand(CLIGS_PLAYER_INFO_EQUIP_REQ, CGTProcessor::OnOpenPlayerInfoEquipReq);
	RegisterCommand(CLIGS_PLAYER_INFO_JEWELRY_REQ, CGTProcessor::OnOpenPlayerInfoJewelryReq);
	RegisterCommand(CLIGS_PLAYER_INFO_SOUL_REQ, CGTProcessor::OnOpenPlayerInfoSoulReq);
	RegisterCommand(CLIGS_DRESS_JEWELRY_REQ, CGTProcessor::OnDressJewelryReq);
	RegisterCommand(CLIGS_OPEN_HERO_SELECT_PANEL_REQ, CGTProcessor::OnOpenHeroSelectPanelReq);
	RegisterCommand(CLIGS_PLAYER_INFO_PET_REQ, CGTProcessor::OnOpenPlayerInfoPetReq);
	//饰品
	RegisterCommand(CLIGS_OPEN_JEWELRY_GENERATE_REQ, CGTProcessor::OnOpenJewelryGenerateReq);
	RegisterCommand(CLIGS_JEWELRY_GENERATE_REQ, CGTProcessor::OnJewelryGenerateReq);
	RegisterCommand(CLIGS_JEWELRY_STRENGTH_REQ, CGTProcessor::OnJewelryStrengthReq);
	RegisterCommand(CLIGS_JEWELRY_GENERATE_INFO_REQ, CGTProcessor::OnJewelryGenerateInfoReq);
	RegisterCommand(CLIGS_OPEN_JEWELRY_STRENGTH_REQ, CGTProcessor::OnOpenJewelryStrengthReq);

	RegisterCommand(CLIGS_JEWELRY_COMPARE_REQ, CGTProcessor::OnCompareJewelryReq);
	RegisterCommand(CLIGS_JEWELRY_EXT_UPGRADE_REQ, CGTProcessor::OnJewelryExtStrengReq);
	RegisterCommand(CLIGS_OPEN_JEWELRY_RESOLVE_REQ, CGTProcessor::OnOpenJewelryResolveReq);
	RegisterCommand(CLIGS_JEWELRY_RESOLVE_REQ, CGTProcessor::OnJewelryResolveReq);

	//GVG
	RegisterCommand(CLIGS_OPEN_GVG_FORMATION_REQ, CGTProcessor::OnOpenGVGFormationReq);
	RegisterCommand(CLIGS_OPEN_GVG_ACTIVITY_REQ, CGTProcessor::OnOpenGVGActivityReq);
	RegisterCommand(CLIGS_GVG_LAST_RESULT_REQ, CGTProcessor::OnOpenGVGLastResultReq);
	RegisterCommand(CLIGS_OPEN_ALREAD_APPLAY_REQ, CGTProcessor::OnOpenAlreadApplayReq);
	RegisterCommand(CLIGS_GVG_OPEN_MY_GAMBLE_REQ, CGTProcessor::OnOpenGVGMyGambleReq);
	RegisterCommand(CLIGS_GVG_OPEN_GAMBLE_REQ, CGTProcessor::OnGVGOpenGambleReq);
	RegisterCommand(CLIGS_OPEN_GVG_RULE_REQ, CGTProcessor::OnGVGOpenRuleReq);
	RegisterCommand(CLIGS_GVG_GAMBLE_REQ, CGTProcessor::OnGVGGambleReq);
    RegisterCommand(CLIGS_OPEN_FACTION_MONEY_RANK_REQ, CGTProcessor::OnOpenGVGMoneyRankReq);
    RegisterCommand(CLIGS_OPEN_GVG_LOG_REQ, CGTProcessor::OnOpenGvGLogReq);
	RegisterCommand(CLIGS_SEE_GVG_VIDEO_REQ, CGTProcessor::OnSeeGvGVideoReq);

    RegisterCommand(CLIGS_END_GVG_VIDEO_REQ, CGTProcessor::OnEndGvGVideoReq);
    RegisterCommand(CLIGS_SHOW_GVG_MVP_REQ, CGTProcessor::OnShowGvGMVPReq);
	RegisterCommand(CLIGS_READ_FORMATION_PLAYER_TIP_REQ, CGTProcessor::OnReadFormationPlayerTipReq);
	RegisterCommand(CLIGS_GVG_FACTION_EMBATTLE_REQ, CGTProcessor::OnGvGFactiomEmbattleReq);

	RegisterCommand(CLIGS_OPEN_GVG_GIFT_REQ, CGTProcessor::OnOpenGvGiftReq);
	RegisterCommand(CLIGS_GET_GVG_GIFT_REQ, CGTProcessor::OnGetGvGiftReq);
	RegisterCommand(CLIGS_SELECT_GROUP_TYPE_REQ, CGTProcessor::OnSelectGroupTypeReq);
	RegisterCommand(CLIGS_GVG_FACTION_APPLAY_REQ, CGTProcessor::OnGvGFactionApplayReq);
	RegisterCommand(CLIGS_READ_BATTLE_LOG_REQ, CGTProcessor::OnReadBattleLogReq);

	RegisterCommand(CLIGS_OPEN_DEVOUR_SOUL_REQ, CGTProcessor::OnOpenDevourSoulReq);
	RegisterCommand(CLIGS_DEVOUR_SOUL_REQ, CGTProcessor::OnDevourSoulReq);
	RegisterCommand(CLIGS_OPEN_REFLASH_SOUL_REQ, CGTProcessor::OnOpenReflashSoulReq);
	RegisterCommand(CLIGS_REFLASH_SOUL_REQ, CGTProcessor::OnReflashSoulReq);
	RegisterCommand(CLIGS_SAVE_REFLASH_SOUL_REQ, CGTProcessor::OnSaveReflashSoulReq);

	RegisterCommand(CLIGS_SOULREP_INFO_REQ,		CGTProcessor::OnSoulRepInfoReq);
	RegisterCommand(CLIGS_PRODUCT_SOUL_REQ,		CGTProcessor::OnProductSoulReq);
	RegisterCommand(CLIGS_HERO_EQUIP_SOUL_REQ,	CGTProcessor::OnEquipSoulReq);
	RegisterCommand(CLIGS_HERO_UNDRESS_SOUL_REQ,CGTProcessor::OnUnDressSoulReq);
	RegisterCommand(CLIGS_HERO_REPLACE_SOUL_REQ,CGTProcessor::OnReplaceSoulReq);
	
	RegisterCommand(CLIGS_SOULEXCHANGE_INFO_REQ,CGTProcessor::OnSoulExchangeInfoReq);
	RegisterCommand(CLIGS_EXCHANGE_SOUL_REQ,	CGTProcessor::OnExchangeSoulReq);

	RegisterCommand(CLIGS_OPEN_MY_MEDAL_REQ, CGTProcessor::OnOpenMyMedalReq);
	//外传
    RegisterCommand(CLIGS_VIEW_FACTION_MEMBERS_REQ, CGTProcessor::OnViewFactionMembersReq);
	RegisterCommand(CLIGS_OPEN_OUT_BOUND_REQ, CGTProcessor::OnOpenMapReq);
	RegisterCommand(CLIGS_OPEN_OUT_BOUND_MAP_REQ, CGTProcessor::OnOpenOutBoundMapReq);
	RegisterCommand(CLIGS_OUT_BOUND_BATTLE_REQ, CGTProcessor::OnOutBoundBattleReq);
    RegisterCommand(CLIGS_VIEW_FACTION_MEMBERS_REQ, CGTProcessor::OnViewFactionMembersReq);
	RegisterCommand(CLIGS_TEST_REQ, CGTProcessor::OnTestReq);
	RegisterCommand(CLIGS_OPEN_STORE_REQ, CGTProcessor::OnOpenStoreReq);
	RegisterCommand(CLIGS_EXCHANGE_GOODS_REQ, CGTProcessor::OnExchangeGoodsReq);

	RegisterCommand(CLIGS_PETREP_INFO_REQ, CGTProcessor::OnOpenPetRepReq);
	RegisterCommand(CLIGS_PRODUCT_PET_REQ, CGTProcessor::OnProductPetReq);
	RegisterCommand(CLIGS_OPEN_REFINEPET_REQ, CGTProcessor::OnOpenRefinePetReq);
	RegisterCommand(CLIGS_REFINEPET_REQ, CGTProcessor::OnRefinePetReq);
	RegisterCommand(CLIGS_OPEN_PET_PANEL_REQ, CGTProcessor::OnOpenPetPanelReq);
	RegisterCommand(CLIGS_OPEN_STAR_PANEL_REQ, CGTProcessor::OnOpenStarPanelReq);
	RegisterCommand(CLIGS_UP_STAR_LV_REQ, CGTProcessor::OnUpPetStarReq);
	RegisterCommand(CLIGS_OPEN_PETFEED_REQ, CGTProcessor::OnOpenPetFeedPanelReq);
	RegisterCommand(CLIGS_FIRST_DRAW_REQ, CGTProcessor::OnPetFristDrawReq);
	RegisterCommand(CLIGS_RESET_DRAW_REQ, CGTProcessor::OnPetResetDrawReq);
	RegisterCommand(CLIGS_CONFIRM_FEED_REQ, CGTProcessor::OnConfirmFeedReq);
	RegisterCommand(CLIGS_MERGE_PET_REQ, CGTProcessor::OnPetMerge);
	RegisterCommand(CLIGS_OPEN_PET_STORE_REQ, CGTProcessor::OnOpenPetStoreReq);
	RegisterCommand(CLIGS_BUY_PET_STORE_ITEM_REQ, CGTProcessor::OnBuyPetStoreItem);
	RegisterCommand(CLIGS_OPEN_WASH_QUALITY_REQ, CGTProcessor::OnOpenWashQuality);
	RegisterCommand(CLIGS_WASH_QUALITY_REQ, CGTProcessor::OnWashQuality);
	RegisterCommand(CLIGS_CHANGE_PET_STATE_REQ, CGTProcessor::OnChangePetState);
	RegisterCommand(CLIGS_OPEN_PET_INHERIT_REQ, CGTProcessor::OnOpenPetInherit);
	RegisterCommand(CLIGS_PET_INHERIT_REQ, CGTProcessor::OnInheritPet);
	RegisterCommand(CLIGS_OPEN_PET_RULE_REQ, CGTProcessor::OnOpenPetRule);
	RegisterCommand(CLIGS_PLAYER_INFO_PET_REQ, CGTProcessor::OnOpenPlayerInfoPetReq);
	RegisterCommand(CLIGS_REQUEST_ALL_PET_INFO_REQ, CGTProcessor::OnRequestAllPetInfo);

    // 飞升
    RegisterCommand(CLIGS_EQUIP_ENCHANT_REQ, CGTProcessor::OnEquipEnchantReq);
    RegisterCommand(CLIGS_EQUIP_MAKE_REQ, CGTProcessor::OnEquipMakeReq);
    RegisterCommand(CLIGS_OPEN_EQUIP_MAKE_REQ, CGTProcessor::OnOpenEquipMakeReq);
    RegisterCommand(CLIGS_UPGRADE_HERO_REQ, CGTProcessor::OnUpgradeHeroReq);
    RegisterCommand(CLIGS_OPEN_FLY_REQ, CGTProcessor::OnOpenFlyReq);
    RegisterCommand(CLIGS_FLY_REQ, CGTProcessor::OnFlyReq);

	RegisterCommand(CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ, CGTProcessor::OnOpenSoulUpdateTypeReq);
	RegisterCommand(CLIGS_SOUL_UPDATE_TYPE_REQ, CGTProcessor::OnSoulUpdateTypeReq);
	RegisterCommand(CLIGS_OPEN_NEIDAN_REQ, CGTProcessor::OnOpenNeiDanReq);
	RegisterCommand(CLIGS_UPDATA_NEIDAN_REQ, CGTProcessor::OnUpdataNeiDanReq);
	RegisterCommand(CLIGS_DRESS_NEIDAN_REQ, CGTProcessor::OnDressNeiDanReq);
	RegisterCommand(CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ, CGTProcessor::OnOpenNeiDanInstanceReq);
	RegisterCommand(CLIGS_NEIDAN_BATTLE_REQ, CGTProcessor::OnNeiDanBattleReq);
	RegisterCommand(CLIGS_RECV_NEIDAN_AWARD_REQ, CGTProcessor::OnRecvNeiDanAwardReq);
	RegisterCommand(CLIGS_SKIP_NEIDAN_BATTLE_REQ, CGTProcessor::OnSkipNeiDanBattleReq);
	return TRUE;
}


BOOL CGTProcessor::OnEnterGSReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GTGS_ENTERGS_REQ* pReq = (PKT_GTGS_ENTERGS_REQ*)pBody;

    CUser* poUser = CUserMgr::Instance()->FindUser(dwTransID);
    if (poUser)
    {
        CPlayer* poPlayer = poUser->GetPlayer();
        if(!poPlayer)
        {
            //释放User
            CUserMgr::Instance()->ReleaseUser(poUser);
            poUser = NULL;
        }
        else
        {
            //判断是否存在数据保存错误，如果存在错误，则不让进入游戏
            if( gsapi::GetDBMgr()->IsExistErrorInfo(ERROR_PLAYER, poPlayer->GetID() ) )
            {
                UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck( ERR_ENTER_GS::ID_SAVE_DATA_ERROR, poUser->GetPlayer() );
                if(INVALID_MSGID == wMsgID)
                {
                    return FALSE;
                }
                BOOL bRet = poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
                return TRUE;
            }

            poPlayer->OnLogin();//上线
            poPlayer->ResetLoginTime(); //重置上线时间
            poPlayer->RecordPlayerOnOffLine( TRUE );//记录上线日志
            poPlayer->SetOriZoneID(pReq->wOriZoneID);

            CPlayerBaseDataCacheMgr::Instance()->ReSetLoginTime(dwTransID);//记录绑在最后登录时间
            //重新设置User参数
            poUser->SetUserProp(pReq->dwUserID, pReq->dwPlayerID, pReq->aszUserName, pReq->aszDeviceID,
                                pReq->aszNotifyID, pReq->wCareerID, pReq->wZoneID, pReq->wOriZoneID, pReq->dwLoginIP, pReq->dwCliVer, pReq->byAuthType, pReq->byDaiChong);
            //下发应答信息
            UINT16 wMsgID = CGTPktBuilder::Instance()->EnterGSAck(SUCCESS, poUser->GetPlayer());
            if(INVALID_MSGID == wMsgID)
            {
                return FALSE;
            }
            BOOL bRet = poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            poUser->SetUserStat(CLIS_IN_GAME);

            //玩家已经正常处理成功，通知玩家
            if (1 == poPlayer->GetDT_EXCHANGE_RESULT_LOG().byNtfFlag)
            {
                PKT_CLIGS_EXCHANGE_RESULT stAck;
                stAck.wErrCode = ERR_EXCHANGE_RESULT_ID::ID_SUCCESS;
                stAck.qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
                stAck.dwExchangeGold = poPlayer->GetDT_EXCHANGE_RESULT_LOG().dwExchangeGold;
                stAck.byVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
                stAck.dwTotalExchangeGold = poPlayer->GetDT_PLAYER_BASE_DATA().dwTotalAmount;
                poPlayer->SendMsg( (const CHAR*)(&stAck), CLIGS_EXCHANGE_RESULT );
                poPlayer->GetDT_EXCHANGE_RESULT_LOG().byNtfFlag = 0;
            }
            return TRUE;
        }
    }

    poUser = CUserMgr::Instance()->CreateUser(pReq->dwUserID, pReq->dwPlayerID, pReq->wCareerID, pReq->aszUserName, pReq->aszDeviceID,
             pReq->aszNotifyID, pReq->wZoneID, pReq->wOriZoneID, pReq->dwLoginIP, pReq->dwCliVer, pReq->byAuthType) ;

    if (NULL == poUser)
    {
        return FALSE;
    }

    //缓存中没有，则去DB获取数据
    CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_LOGIN, pReq->dwPlayerID);

    //客户端进入了获取用户信息的状态
    poUser->SetUserStat(CLIS_GET_USER_DATA_ING);

    return TRUE;
}

BOOL CGTProcessor::OnKickOutAck(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_GSGT_KICK_OUT_ACK* pstAck = (PKT_GSGT_KICK_OUT_ACK*)pBody;
    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(dwTransID);
    if(NULL == poClinet)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: OnKickOutAck find CRCClient[%d] failed!"), MSG_MARK, dwTransID);
        return FALSE;
    }
    poClinet->Rspone(pstAck->aszErrMsg);
    return TRUE;
}

BOOL CGTProcessor::OnUserDisconnect(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    //修改玩家状态为断开连接
    CUser* poUser = (CUser*)pUser;
    poUser->SetUserStat(CLIS_DISCONNECT);
    poUser->SetDisconnectTime();
    CPlayer* poPlayer = poUser->GetPlayer();
    if(poPlayer)
    {
        DBG_INFO(_SDT("[%s:%d]: player[%u : %s] disconnected!"), MSG_MARK, poPlayer->GetDT_PLAYER_BASE_DATA().dwID, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);

        poPlayer->OnLogin(FALSE);//下线
        poPlayer->ResetLogoffTime(); //重置下线时间
        CPlayerBaseDataCacheMgr::Instance()->ReSetLogoffTime(poPlayer->GetID());//记录绑在最后登录时间
        CPlayerMgr::Instance()->OnSaveOnlineEvent(poPlayer->GetDT_PLAYER_BASE_DATA().dwID);//下线保存
        poPlayer->SetDisconnectTime();
        poPlayer->SetUpdateTime();
        poPlayer->RecordPlayerOnOffLine( FALSE );

        CBossBattleMgr::Instance()->OnLeave(poPlayer, TRUE);//下线则从boss战中退出
        //CCourageMgr::Instance()->OnLeave(poPlayer,TRUE);
    }
    return TRUE;
}

BOOL CGTProcessor::OnGetHeroInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_HERO_INFO_REQ* pReq = (PKT_CLIGS_GET_HERO_INFO_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GET_HERO_INFO_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    if( NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData( pReq->dwPlayerID ) )
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->GetHeroInfoErrorAck(ERR_GET_HERO_INFO::ID_PLAY_NOT_EXIT);
        SendMsg( poUser, wMsgID );
        return FALSE;
    }

    //真实需要获得信息的玩家
    CPlayer *poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
    if( NULL == poTargetPlayer )
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, GET_PLAYERDATATYPE_HEROINFO,
                pReq->dwPlayerID, pReq->byEquipBagFlag );
        return TRUE;
    }

    if( NULL == poUser )
    {
        SYS_CRITICAL( _SDT( "[%s %d] poUser is NULL" ), MSG_MARK );
        return FALSE;
    }

    CPlayer* pPlayer = poUser->GetPlayer();
    if( NULL == pPlayer )
    {
        SYS_CRITICAL( _SDT( "[%s %d]GetPlayer return is NULL" ), MSG_MARK );
        return FALSE;
    }

    //获得英雄信息
    UINT16 wMsgID = CGTPktBuilder::Instance()->GetHeroInfoAck(poTargetPlayer,
                    pReq->byEquipBagFlag, poTargetPlayer->GetID() == pPlayer->GetID() );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}

BOOL CGTProcessor::OnEnterHomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ENTER_HOME_REQ* pReq = (PKT_CLIGS_ENTER_HOME_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_ENTER_HOME_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnEnterHomeAck(poPlayer, pReq->byKind);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;

}


BOOL CGTProcessor::OnEnterInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ENTER_INSTANCE_REQ* pReq = (PKT_CLIGS_ENTER_INSTANCE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_ENTER_INSTANCE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wErrCode =  poPlayer->EnterInstance(pReq->wSceneIdx, pReq->byTownIdx, pReq->byInstanceIdx, pReq->byNewFlag);

    UINT16 wMsgID = CGTPktBuilder::Instance()->EnterInstanceAck(wErrCode, pReq->wSceneIdx, pReq->byTownIdx, pReq->byInstanceIdx, poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;

}


//////////////////////////////////////////////////////////////////////////
//进入场景的错误码转战斗错误码
UINT16 ExChangeBattleErrCode(UINT16 wErrorValue)
{
    UINT16 wError = ERR_BATTLE::ID_SUCCESS;
    switch(wErrorValue)
    {
    case ERR_ENTER_INSTANCE::ID_INDEX_ERR:
        wError = ERR_BATTLE::ID_INDEX_ERR;
        break;
    case ERR_ENTER_INSTANCE::ID_REENTER_ERR:
        wError = ERR_BATTLE::ID_REENTER_ERR;
        break;
    case ERR_ENTER_INSTANCE::ID_LEVEL_ERR:
        wError = ERR_BATTLE::ID_LEVEL_ERR;
        break;
    case ERR_ENTER_INSTANCE::ID_PHYSTRENGTH_ERR:
        wError = ERR_BATTLE::ID_PHYSTRENGTH_ERR;
        break;
    case ERR_ENTER_INSTANCE::ID_NOT_UNLOCK_ERR:
        wError = ERR_BATTLE::ID_NOT_UNLOCK_ERR;
        break;
    case ERR_ENTER_INSTANCE::ID_ELITE_ONLY_ONE_PER_DAY_ERR:
        wError = ERR_BATTLE::ID_ELITE_ONLY_ONE_PER_DAY_ERR;
        break;
    case ERR_ENTER_INSTANCE::ID_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR:
        wError = ERR_BATTLE::ID_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR;
        break;
    case ERR_ENTER_INSTANCE::ID_NOT_UNLOCK_LEVEL_ERR:
        wError = ERR_BATTLE::ID_NOT_UNLOCK_LEVEL_ERR;
        break;
    default:
        break;
    }
    return wError;
}


BOOL CGTProcessor::OnBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CUser* poUser = (CUser*)pUser;
	if ( (poUser) && poUser->GetCliVer() >= VERSION_PET)
	{
		return OnBattleReq5(pUser, pHeader, pBody, dwLen);
	}
	else if ( (poUser) && poUser->GetCliVer() >= VERSION_SOUL)
	{
		return OnBattleReq4(poUser, pHeader, pBody, dwLen); 
	}
	else if ((poUser) && (poUser->GetCliVer() > 310))
	{
		return OnBattleReq3(pUser, pHeader, pBody, dwLen);
	}
    
    return OnBattleReq5(pUser, pHeader, pBody, dwLen);
}
//
//BOOL CGTProcessor::OnBattleReq1(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
//{
//    //CAutoCycle oAutoCycle("CGTProcessor::OnBattleReq");
//
//    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
//    UINT32 dwTransID            = pPipeHeader->dwTransID;
//
//    PKT_CLIGS_BATTLE_REQ* pReq = (PKT_CLIGS_BATTLE_REQ*)pBody;
//    if ( sizeof(PKT_CLIGS_BATTLE_REQ) != dwLen )
//        return FALSE;
//
//    CUser* poUser = (CUser*)pUser;
//
//    CPlayer* poPlayer = poUser->GetPlayer();
//    if(!poPlayer)
//    {
//        return FALSE;
//    }
//
//    static PKT_CLIGS_BATTLE_ACK stAck;
//    memset(&stAck, 0, sizeof(PKT_CLIGS_BATTLE_ACK));
//    CInstance& oInstance = poPlayer->GetInstance();
//    //////////////////////////////////////////////////////////////////////////
//    //对于是塔副本的，没有先发送进入场景协议，由战斗的地方进入场景，
//    // 同理在战斗结束也没有退出场景协议，也要清除掉
//    //@ 2013-02-23
//    BOOL		bGuide			=	FALSE;
//    UINT8	byCurInstance		=	1;
//    if (EIT_TOWER == pReq->wSceneIdx)
//    {
//        pReq->byTownIdx = 1;
//        pReq->byBattleIndex = 1;
//
//        if(poPlayer->CKGuide( EGF_CLIMBTOWER ))
//        {
//            bGuide = TRUE;
//        }
//
//        //增加引导功能
//        poPlayer->AddGuideRecord( EGF_CLIMBTOWER );
//
//        //获取城镇相关属性信息
//        STownInstanceProp * pstTownInstanceProp = NULL;
//        pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(EIT_TOWER, pReq->byTownIdx);
//        if (NULL == pstTownInstanceProp)
//        {
//            stAck.wErrCode = ERR_BATTLE::ID_INDEX_ERR;
//            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//        }
//
//        UINT64	qwLastPassTime	=	1;
//        UINT16	wErrCode = ERR_BATTLE::ID_SUCCESS;
//        for (UINT8 byInstanceIdx = 1; byInstanceIdx <= pstTownInstanceProp->m_mapInstance.size(); byInstanceIdx++)
//        {
//            //查看城镇的副本是否有打通关记录。
//            CInstanceRecord* poInstanceRecord = NULL;
//            poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byInstanceIdx);
//            //没有记录，赋值最初值
//            if(NULL != poInstanceRecord)
//            {
//                if (qwLastPassTime <= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime)
//                {
//                    byCurInstance		= byInstanceIdx + 1;
//                    qwLastPassTime	= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime;
//                }
//            }
//            else
//            {
//                break;
//            }
//        }
//
//        //检测塔是否已全部打过。
//        if (byCurInstance > pstTownInstanceProp->m_mapInstance.size())
//        {
//            stAck.wErrCode = ERR_BATTLE::ID_MAX_HIGH_FLOOR;
//            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//        }
//
//        wErrCode =  poPlayer->EnterInstance(pReq->wSceneIdx, pReq->byTownIdx, byCurInstance, 1);
//        if (ERR_ENTER_INSTANCE::ID_SUCCESS != wErrCode)
//        {
//            stAck.wErrCode = ExChangeBattleErrCode(wErrCode);
//            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//        }
//    }
//    //@ end 2013-02-23
//    //////////////////////////////////////////////////////////////////////////
//    stAck.wErrCode = oInstance.EnterBattle(pReq->byBattleIndex);
//    if(ERR_BATTLE::ID_HAVE_BATTLE_ERR == stAck.wErrCode)
//    {
//        PKT_CLIGS_BATTLE_ACK* pstAck = poPlayer->GetLastBattleAck();
//        if(pstAck)
//        {
//            return poUser->SendMsg((CHAR*)(pstAck), CLIGS_BATTLE_ACK);
//        }
//        else
//        {
//            return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//        }
//    }
//    else if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
//    {
//        poPlayer->ExitInstance();
//        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//    }
//
//    SCurBattleInstance& stCurBattleInstance = oInstance.GetCurBattleInstance();
//    CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, pReq->byBattleIndex);
//    if(NULL == poMonsterGroup)
//    {
//        poPlayer->ExitInstance();
//        SET_OTHER_ERR(stAck.wErrCode);
//        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//    }
//
//    stAck.stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
//    DT_BATTLE_DATA4   stBattleInfo = {0}; //战斗信息
//    stAck.wErrCode = CInstanceMgr::Instance()->InstanceBattle(poPlayer, poMonsterGroup, stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx,
//                     pReq->byBattleIndex, stBattleInfo, stAck.byPassFlag, stAck.astUnlockInfo[0], stAck.stBattleEncourage, stAck.stAfterBattleEncourage,
//                     stAck.astPassEncourage[0], stAck.astAfterPassEncourage[0], stAck.astPassEncourageBox[0], bGuide);
//    if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
//    {
//        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//    }
//    CBattleMgr::Instance()->BattleData221(stBattleInfo, stAck.stBattleInfo);
//
//    ////战斗失败，则退出副本回家园
//    //if(EBT_FAILED == stAck.stBattleInfo.byBattleResult)
//    //{
//    //    poPlayer->ExitInstance();
//    //}
//
//    //副本通关且为普通副本，赋值下一个副本信息
//    if((EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult) && (EIT_COMMON == stCurBattleInstance.wSceneIdx))
//    {
//        //赋值当前副本的下一个副本
//        SNextInstance stNextInstance;
//        CInstancePropMgr::Instance()->GetNextInstance(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, stNextInstance);
//        DT_INSTANCE_ID stMaxUnlockCommonInstanceID;
//        poPlayer->GetMaxUnlockCommonInstanceID(stMaxUnlockCommonInstanceID);//获取任务解锁到的副本
//        if((stNextInstance.bHaveNext) &&
//                (TRUE == CInstance::CkLessEqualID(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx, stMaxUnlockCommonInstanceID.byTownIdx, stMaxUnlockCommonInstanceID.byInstanceIdx)))
//        {
//            stAck.byHaveNextInstance = 1;
//            stAck.astNextInstanceInfo[0].wSceneIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx;
//            stAck.astNextInstanceInfo[0].byTownIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx;
//            stAck.astNextInstanceInfo[0].byInstanceIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx;
//            SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx);
//            if(pstInstanceProp)
//            {
//                DT_NEXT_INSTANCE_DATA& stNextInstanceInfo = stAck.astNextInstanceInfo[0];
//                stNextInstanceInfo.wEnterLevel = 1;//
//                stNextInstanceInfo.wEnterPhyStrength = CPhystrengthMgr::Instance()->GetCommonPhyStrength();
//            }
//            else
//            {
//                SYS_CRITICAL(_SDT("[%s: %d]: pstInstanceProp is nULL!"), MSG_MARK);
//            }
//        }
//
//        poPlayer->GetState().OnPassInstance(stAck.astAfterPassEncourage[0].byHeroNum, stAck.astAfterPassEncourage[0].astHeroUpgradeInfo);
//    }
//
//    //@2013-02-23
//    //直接给奖励，并退出场景，特殊处理
//    if (EIT_TOWER == stCurBattleInstance.wSceneIdx)
//    {
//        //////////////////////////////////////////////////////////////////////////
//        //爬塔通关直接领取奖励
//
//        if(EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult)
//        {
//            DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage; //通关奖励宝箱信息
//            DT_DATA_AFTER_PASS_ENCOURAGE_BOX stAfterPassEncourage; //领取通关奖励宝箱后玩家信息
//            memset(&stPassEncourage, 0x00, sizeof(stPassEncourage));
//            memset(&stAfterPassEncourage, 0x00, sizeof(stAfterPassEncourage));
//
//
//            UINT16 wErrCode =  poPlayer->GetInstance().OpenEncourageBox(stPassEncourage, &stAfterPassEncourage);
//            //检测背包是否满了
//            if(ERR_OPEN_INSTANCE_ENCOURAGE_BOX::ID_BAG_FULL == wErrCode)
//            {
//                //查看城镇的副本是否有打通关记录。
//                CInstanceRecord* poInstanceRecord = NULL;
//                poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byCurInstance);
//                if (NULL != poInstanceRecord)
//                {
//                    poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
//                }
//                stAck.wErrCode = ERR_BATTLE::ID_BAG_FULL;
//            }
//        }
//        poPlayer->ExitInstance();
//    }
//    //@ end 2013-02-23
//
//    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(stAck.stTaskInfoList);
//    poPlayer->GetDT_TASK_COND_RECORD_LIST(stAck.stTaskRecordList);
//    poPlayer->GetPhystrength().GetPhyStrengthInfo(stAck.stPhyStrengthInfo);
//    memcpy(stAck.abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);
//    stAck.stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
//
//    poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK);
//
//    poPlayer->RecordLastBattleAck(stAck);
//
//    return TRUE;
//
//}

BOOL CGTProcessor::OnBattleReq5(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    //CAutoCycle oAutoCycle("CGTProcessor::OnBattleReq");

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BATTLE_REQ* pReq = (PKT_CLIGS_BATTLE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_BATTLE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    static PKT_CLIGS_BATTLE_ACK5 stAck;
    memset(&stAck, 0, sizeof(PKT_CLIGS_BATTLE_ACK5));
    CInstance& oInstance = poPlayer->GetInstance();
    //////////////////////////////////////////////////////////////////////////
    //对于是塔副本的，没有先发送进入场景协议，由战斗的地方进入场景，
    // 同理在战斗结束也没有退出场景协议，也要清除掉
    //@ 2013-02-23
    BOOL		bGuide			=	FALSE;
    UINT8	byCurInstance		=	1;
    if (EIT_TOWER == pReq->wSceneIdx)
    {
        pReq->byTownIdx = 1;
        pReq->byBattleIndex = 1;

        if(poPlayer->CKGuide( EGF_CLIMBTOWER ))
        {
            bGuide = TRUE;
        }

        //增加引导功能
        poPlayer->AddGuideRecord( EGF_CLIMBTOWER );

        //获取城镇相关属性信息
        STownInstanceProp * pstTownInstanceProp = NULL;
        pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(EIT_TOWER, pReq->byTownIdx);
        if (NULL == pstTownInstanceProp)
        {
            stAck.wErrCode = ERR_BATTLE::ID_INDEX_ERR;
            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);
        }

        UINT64	qwLastPassTime	=	1;
        UINT16	wErrCode = ERR_BATTLE::ID_SUCCESS;
        for (UINT8 byInstanceIdx = 1; byInstanceIdx <= pstTownInstanceProp->m_mapInstance.size(); byInstanceIdx++)
        {
            //查看城镇的副本是否有打通关记录。
            CInstanceRecord* poInstanceRecord = NULL;
            poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byInstanceIdx);
            //没有记录，赋值最初值
            if(NULL != poInstanceRecord)
            {
                if (qwLastPassTime <= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime)
                {
                    byCurInstance		= byInstanceIdx + 1;
                    qwLastPassTime	= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime;
                }
            }
            else
            {
                break;
            }
        }

        //检测塔是否已全部打过。
        if (byCurInstance > pstTownInstanceProp->m_mapInstance.size())
        {
            stAck.wErrCode = ERR_BATTLE::ID_MAX_HIGH_FLOOR;
            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);
        }

        wErrCode =  poPlayer->EnterInstance(pReq->wSceneIdx, pReq->byTownIdx, byCurInstance, 1);
        if (ERR_ENTER_INSTANCE::ID_SUCCESS != wErrCode)
        {
            stAck.wErrCode = ExChangeBattleErrCode(wErrCode);
            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);
        }
    }
    //@ end 2013-02-23
    //////////////////////////////////////////////////////////////////////////
    stAck.wErrCode = oInstance.EnterBattle(pReq->byBattleIndex);
    if(ERR_BATTLE::ID_HAVE_BATTLE_ERR == stAck.wErrCode)
    {
        PKT_CLIGS_BATTLE_ACK5* pstAck = poPlayer->GetLastBattleAck5();
        if(pstAck)
        {
            return poUser->SendMsg((CHAR*)(pstAck), CLIGS_BATTLE_ACK5);
        }
        else
        {
            return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);
        }
    }
    else if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
    {
        poPlayer->ExitInstance();
        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);
    }

    SCurBattleInstance& stCurBattleInstance = oInstance.GetCurBattleInstance();
    CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, pReq->byBattleIndex);
    if(NULL == poMonsterGroup)
    {
        poPlayer->ExitInstance();
        SET_OTHER_ERR(stAck.wErrCode);
        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);
    }

    stAck.stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    stAck.wErrCode = CInstanceMgr::Instance()->InstanceBattle(poPlayer, poMonsterGroup, stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx,
                     pReq->byBattleIndex, stAck.stBattleInfo, stAck.byPassFlag, stAck.astUnlockInfo[0], stAck.stBattleEncourage, stAck.stAfterBattleEncourage,
                     stAck.astPassEncourage[0], stAck.astAfterPassEncourage[0], stAck.astPassEncourageBox[0], bGuide);
    if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
    {
        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);
    }

    ////战斗失败，则退出副本回家园
    //if(EBT_FAILED == stAck.stBattleInfo.byBattleResult)
    //{
    //    poPlayer->ExitInstance();
    //}

    //副本通关且为普通副本，赋值下一个副本信息
    if((EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult) && (EIT_COMMON == stCurBattleInstance.wSceneIdx))
    {
        //赋值当前副本的下一个副本
        SNextInstance stNextInstance;
        CInstancePropMgr::Instance()->GetNextInstance(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, stNextInstance);
        DT_INSTANCE_ID stMaxUnlockCommonInstanceID;
        poPlayer->GetMaxUnlockCommonInstanceID(stMaxUnlockCommonInstanceID);//获取任务解锁到的副本
        if((stNextInstance.bHaveNext) &&
                (TRUE == CInstance::CkLessEqualID(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx, stMaxUnlockCommonInstanceID.byTownIdx, stMaxUnlockCommonInstanceID.byInstanceIdx)))
        {
            stAck.byHaveNextInstance = 1;
            stAck.astNextInstanceInfo[0].wSceneIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx;
            stAck.astNextInstanceInfo[0].byTownIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx;
            stAck.astNextInstanceInfo[0].byInstanceIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx;
            SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx);
            if(pstInstanceProp)
            {
                DT_NEXT_INSTANCE_DATA& stNextInstanceInfo = stAck.astNextInstanceInfo[0];
                stNextInstanceInfo.wEnterLevel = 1;//
                stNextInstanceInfo.wEnterPhyStrength = CPhystrengthMgr::Instance()->GetCommonPhyStrength();
            }
            else
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pstInstanceProp is nULL!"), MSG_MARK);
            }
        }

        poPlayer->GetState().OnPassInstance(stAck.astAfterPassEncourage[0].byHeroNum, stAck.astAfterPassEncourage[0].astHeroUpgradeInfo);
    }

    //@2013-02-23
    //退出场景，特殊处理
    if (EIT_TOWER == stCurBattleInstance.wSceneIdx)
    {
        //////////////////////////////////////////////////////////////////////////
        //爬塔通关直接领取奖励

        if(EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult)
        {
            DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage; //通关奖励宝箱信息
            DT_DATA_AFTER_PASS_ENCOURAGE_BOX stAfterPassEncourage; //领取通关奖励宝箱后玩家信息
            memset(&stPassEncourage, 0x00, sizeof(stPassEncourage));
            memset(&stAfterPassEncourage, 0x00, sizeof(stAfterPassEncourage));


            UINT16 wErrCode =  poPlayer->GetInstance().OpenEncourageBox(stPassEncourage, &stAfterPassEncourage);
            //检测背包是否满了
            if(ERR_OPEN_INSTANCE_ENCOURAGE_BOX::ID_BAG_FULL == wErrCode)
            {
                //查看城镇的副本是否有打通关记录。
                CInstanceRecord* poInstanceRecord = NULL;
                poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byCurInstance);
                if (NULL != poInstanceRecord)
                {
                    poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
                }
                stAck.wErrCode = ERR_BATTLE::ID_BAG_FULL;
            }
        }
        poPlayer->ExitInstance();
    }
    //@ end 2013-02-23

    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(stAck.stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(stAck.stTaskRecordList);
    poPlayer->GetPhystrength().GetPhyStrengthInfo(stAck.stPhyStrengthInfo);
    memcpy(stAck.abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);
    stAck.stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK5);

    poPlayer->RecordLastBattleAck5(stAck);

    return TRUE;

}

BOOL CGTProcessor::OnBattleReq3(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    //CAutoCycle oAutoCycle("CGTProcessor::OnBattleReq");

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BATTLE_REQ* pReq = (PKT_CLIGS_BATTLE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_BATTLE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    static PKT_CLIGS_BATTLE_ACK3 stAck;
    memset(&stAck, 0, sizeof(PKT_CLIGS_BATTLE_ACK3));
    CInstance& oInstance = poPlayer->GetInstance();
    //////////////////////////////////////////////////////////////////////////
    //对于是塔副本的，没有先发送进入场景协议，由战斗的地方进入场景，
    // 同理在战斗结束也没有退出场景协议，也要清除掉
    //@ 2013-02-23
    BOOL		bGuide			=	FALSE;
    UINT8	byCurInstance		=	1;
    if (EIT_TOWER == pReq->wSceneIdx)
    {
        pReq->byTownIdx = 1;
        pReq->byBattleIndex = 1;

        if(poPlayer->CKGuide( EGF_CLIMBTOWER ))
        {
            bGuide = TRUE;
        }

        //增加引导功能
        poPlayer->AddGuideRecord( EGF_CLIMBTOWER );

        //获取城镇相关属性信息
        STownInstanceProp * pstTownInstanceProp = NULL;
        pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(EIT_TOWER, pReq->byTownIdx);
        if (NULL == pstTownInstanceProp)
        {
            stAck.wErrCode = ERR_BATTLE::ID_INDEX_ERR;
            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);
        }

        UINT64	qwLastPassTime	=	1;
        UINT16	wErrCode = ERR_BATTLE::ID_SUCCESS;
        for (UINT8 byInstanceIdx = 1; byInstanceIdx <= pstTownInstanceProp->m_mapInstance.size(); byInstanceIdx++)
        {
            //查看城镇的副本是否有打通关记录。
            CInstanceRecord* poInstanceRecord = NULL;
            poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byInstanceIdx);
            //没有记录，赋值最初值
            if(NULL != poInstanceRecord)
            {
                if (qwLastPassTime <= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime)
                {
                    byCurInstance		= byInstanceIdx + 1;
                    qwLastPassTime	= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime;
                }
            }
            else
            {
                break;
            }
        }

        //检测塔是否已全部打过。
        if (byCurInstance > pstTownInstanceProp->m_mapInstance.size())
        {
            stAck.wErrCode = ERR_BATTLE::ID_MAX_HIGH_FLOOR;
            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);
        }

        wErrCode =  poPlayer->EnterInstance(pReq->wSceneIdx, pReq->byTownIdx, byCurInstance, 1);
        if (ERR_ENTER_INSTANCE::ID_SUCCESS != wErrCode)
        {
            stAck.wErrCode = ExChangeBattleErrCode(wErrCode);
            return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);
        }
    }
    //@ end 2013-02-23
    //////////////////////////////////////////////////////////////////////////
    stAck.wErrCode = oInstance.EnterBattle(pReq->byBattleIndex);
    if(ERR_BATTLE::ID_HAVE_BATTLE_ERR == stAck.wErrCode)
    {
        PKT_CLIGS_BATTLE_ACK3* pstAck = poPlayer->GetLastBattleAck3();
        if(pstAck)
        {
            return poUser->SendMsg((CHAR*)(pstAck), CLIGS_BATTLE_ACK3);
        }
        else
        {
            return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);
        }
    }
    else if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
    {
        poPlayer->ExitInstance();
        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);
    }

    SCurBattleInstance& stCurBattleInstance = oInstance.GetCurBattleInstance();
    CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, pReq->byBattleIndex);
    if(NULL == poMonsterGroup)
    {
        poPlayer->ExitInstance();
        SET_OTHER_ERR(stAck.wErrCode);
        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);
    }

    stAck.stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
    stAck.wErrCode = CInstanceMgr::Instance()->InstanceBattle2(poPlayer, poMonsterGroup, stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx,
        pReq->byBattleIndex, stAck.stBattleInfo, stAck.byPassFlag, stAck.astUnlockInfo[0], stAck.stBattleEncourage, stAck.stAfterBattleEncourage,
        stAck.astPassEncourage[0], stAck.astAfterPassEncourage[0], stAck.astPassEncourageBox[0], bGuide);
    if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
    {
        return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);
    }

    ////战斗失败，则退出副本回家园
    //if(EBT_FAILED == stAck.stBattleInfo.byBattleResult)
    //{
    //    poPlayer->ExitInstance();
    //}

    //副本通关且为普通副本，赋值下一个副本信息
    if((EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult) && (EIT_COMMON == stCurBattleInstance.wSceneIdx))
    {
        //赋值当前副本的下一个副本
        SNextInstance stNextInstance;
        CInstancePropMgr::Instance()->GetNextInstance(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, stNextInstance);
        DT_INSTANCE_ID stMaxUnlockCommonInstanceID;
        poPlayer->GetMaxUnlockCommonInstanceID(stMaxUnlockCommonInstanceID);//获取任务解锁到的副本
        if((stNextInstance.bHaveNext) &&
            (TRUE == CInstance::CkLessEqualID(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx, stMaxUnlockCommonInstanceID.byTownIdx, stMaxUnlockCommonInstanceID.byInstanceIdx)))
        {
            stAck.byHaveNextInstance = 1;
            stAck.astNextInstanceInfo[0].wSceneIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx;
            stAck.astNextInstanceInfo[0].byTownIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx;
            stAck.astNextInstanceInfo[0].byInstanceIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx;
            SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx);
            if(pstInstanceProp)
            {
                DT_NEXT_INSTANCE_DATA& stNextInstanceInfo = stAck.astNextInstanceInfo[0];
                stNextInstanceInfo.wEnterLevel = 1;//
                stNextInstanceInfo.wEnterPhyStrength = CPhystrengthMgr::Instance()->GetCommonPhyStrength();
            }
            else
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pstInstanceProp is nULL!"), MSG_MARK);
            }
        }

        poPlayer->GetState().OnPassInstance(stAck.astAfterPassEncourage[0].byHeroNum, stAck.astAfterPassEncourage[0].astHeroUpgradeInfo);
    }

    //@2013-02-23
    //退出场景，特殊处理
    if (EIT_TOWER == stCurBattleInstance.wSceneIdx)
    {
        //////////////////////////////////////////////////////////////////////////
        //爬塔通关直接领取奖励

        if(EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult)
        {
            DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage; //通关奖励宝箱信息
            DT_DATA_AFTER_PASS_ENCOURAGE_BOX stAfterPassEncourage; //领取通关奖励宝箱后玩家信息
            memset(&stPassEncourage, 0x00, sizeof(stPassEncourage));
            memset(&stAfterPassEncourage, 0x00, sizeof(stAfterPassEncourage));


            UINT16 wErrCode =  poPlayer->GetInstance().OpenEncourageBox(stPassEncourage, &stAfterPassEncourage);
            //检测背包是否满了
            if(ERR_OPEN_INSTANCE_ENCOURAGE_BOX::ID_BAG_FULL == wErrCode)
            {
                //查看城镇的副本是否有打通关记录。
                CInstanceRecord* poInstanceRecord = NULL;
                poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byCurInstance);
                if (NULL != poInstanceRecord)
                {
                    poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
                }
                stAck.wErrCode = ERR_BATTLE::ID_BAG_FULL;
            }
        }
        poPlayer->ExitInstance();
    }
    //@ end 2013-02-23

    poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(stAck.stTaskInfoList);
    poPlayer->GetDT_TASK_COND_RECORD_LIST(stAck.stTaskRecordList);
    poPlayer->GetPhystrength().GetPhyStrengthInfo(stAck.stPhyStrengthInfo);
    memcpy(stAck.abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);
    stAck.stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

    poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK3);

    poPlayer->RecordLastBattleAck3(stAck);

    return TRUE;

}

BOOL CGTProcessor::OnBattleReq4(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_BATTLE_REQ* pReq = (PKT_CLIGS_BATTLE_REQ*)pBody;
	if ( sizeof(PKT_CLIGS_BATTLE_REQ) != dwLen )
		return FALSE;

	CUser* poUser = (CUser*)pUser;

	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	static PKT_CLIGS_BATTLE_ACK4 stAck;
	memset(&stAck, 0, sizeof(PKT_CLIGS_BATTLE_ACK4));
	CInstance& oInstance = poPlayer->GetInstance();
	//////////////////////////////////////////////////////////////////////////
	//对于是塔副本的，没有先发送进入场景协议，由战斗的地方进入场景，
	// 同理在战斗结束也没有退出场景协议，也要清除掉
	//@ 2013-02-23
	BOOL		bGuide			=	FALSE;
	UINT8	byCurInstance		=	1;
	if (EIT_TOWER == pReq->wSceneIdx)
	{
		pReq->byTownIdx = 1;
		pReq->byBattleIndex = 1;

		if(poPlayer->CKGuide( EGF_CLIMBTOWER ))
		{
			bGuide = TRUE;
		}

		//增加引导功能
		poPlayer->AddGuideRecord( EGF_CLIMBTOWER );

		//获取城镇相关属性信息
		STownInstanceProp * pstTownInstanceProp = NULL;
		pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(EIT_TOWER, pReq->byTownIdx);
		if (NULL == pstTownInstanceProp)
		{
			stAck.wErrCode = ERR_BATTLE::ID_INDEX_ERR;
			return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
		}

		UINT64	qwLastPassTime	=	1;
		UINT16	wErrCode = ERR_BATTLE::ID_SUCCESS;
		for (UINT8 byInstanceIdx = 1; byInstanceIdx <= pstTownInstanceProp->m_mapInstance.size(); byInstanceIdx++)
		{
			//查看城镇的副本是否有打通关记录。
			CInstanceRecord* poInstanceRecord = NULL;
			poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byInstanceIdx);
			//没有记录，赋值最初值
			if(NULL != poInstanceRecord)
			{
				if (qwLastPassTime <= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime)
				{
					byCurInstance		= byInstanceIdx + 1;
					qwLastPassTime	= poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime;
				}
			}
			else
			{
				break;
			}
		}

		//检测塔是否已全部打过。
		if (byCurInstance > pstTownInstanceProp->m_mapInstance.size())
		{
			stAck.wErrCode = ERR_BATTLE::ID_MAX_HIGH_FLOOR;
			return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
		}

		wErrCode =  poPlayer->EnterInstance(pReq->wSceneIdx, pReq->byTownIdx, byCurInstance, 1);
		if (ERR_ENTER_INSTANCE::ID_SUCCESS != wErrCode)
		{
			stAck.wErrCode = ExChangeBattleErrCode(wErrCode);
			return  poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
		}
	}
	//@ end 2013-02-23
	//////////////////////////////////////////////////////////////////////////
	stAck.wErrCode = oInstance.EnterBattle(pReq->byBattleIndex);
	if(ERR_BATTLE::ID_HAVE_BATTLE_ERR == stAck.wErrCode)
	{
		PKT_CLIGS_BATTLE_ACK4* pstAck = poPlayer->GetLastBattleAck4();
		if(pstAck)
		{
			return poUser->SendMsg((CHAR*)(pstAck), CLIGS_BATTLE_ACK4);
		}
		else
		{
			return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
		}
	}
	else if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
	{
		poPlayer->ExitInstance();
		return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
	}

	SCurBattleInstance& stCurBattleInstance = oInstance.GetCurBattleInstance();
	CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, pReq->byBattleIndex);
	if(NULL == poMonsterGroup)
	{
		poPlayer->ExitInstance();
		SET_OTHER_ERR(stAck.wErrCode);
		return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
	}

	stAck.stPowerInfo.dwOldPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;
	stAck.wErrCode = CInstanceMgr::Instance()->InstanceBattle3(poPlayer, poMonsterGroup, stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx,
		pReq->byBattleIndex, stAck.stBattleInfo, stAck.byPassFlag, stAck.astUnlockInfo[0], stAck.stBattleEncourage, stAck.stAfterBattleEncourage,
		stAck.astPassEncourage[0], stAck.astAfterPassEncourage[0], stAck.astPassEncourageBox[0], bGuide);
	if(ERR_BATTLE::ID_SUCCESS != stAck.wErrCode)
	{
		return poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
	}

	////战斗失败，则退出副本回家园
	//if(EBT_FAILED == stAck.stBattleInfo.byBattleResult)
	//{
	//    poPlayer->ExitInstance();
	//}

	//副本通关且为普通副本，赋值下一个副本信息
	if((EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult) && (EIT_COMMON == stCurBattleInstance.wSceneIdx))
	{
		//赋值当前副本的下一个副本
		SNextInstance stNextInstance;
		CInstancePropMgr::Instance()->GetNextInstance(stCurBattleInstance.wSceneIdx, stCurBattleInstance.byTownIdx, stCurBattleInstance.byInstanceIdx, stNextInstance);
		DT_INSTANCE_ID stMaxUnlockCommonInstanceID;
		poPlayer->GetMaxUnlockCommonInstanceID(stMaxUnlockCommonInstanceID);//获取任务解锁到的副本
		if((stNextInstance.bHaveNext) &&
			(TRUE == CInstance::CkLessEqualID(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx, stMaxUnlockCommonInstanceID.byTownIdx, stMaxUnlockCommonInstanceID.byInstanceIdx)))
		{
			stAck.byHaveNextInstance = 1;
			stAck.astNextInstanceInfo[0].wSceneIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx;
			stAck.astNextInstanceInfo[0].byTownIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx;
			stAck.astNextInstanceInfo[0].byInstanceIdx = stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx;
			SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx, stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx);
			if(pstInstanceProp)
			{
				DT_NEXT_INSTANCE_DATA& stNextInstanceInfo = stAck.astNextInstanceInfo[0];
				stNextInstanceInfo.wEnterLevel = 1;//
				stNextInstanceInfo.wEnterPhyStrength = CPhystrengthMgr::Instance()->GetCommonPhyStrength();
			}
			else
			{
				SYS_CRITICAL(_SDT("[%s: %d]: pstInstanceProp is nULL!"), MSG_MARK);
			}
		}

		poPlayer->GetState().OnPassInstance(stAck.astAfterPassEncourage[0].byHeroNum, stAck.astAfterPassEncourage[0].astHeroUpgradeInfo);
	}

	//@2013-02-23
	//退出场景，特殊处理
	else if (EIT_TOWER == stCurBattleInstance.wSceneIdx)
	{
		//////////////////////////////////////////////////////////////////////////
		//爬塔通关直接领取奖励

		if(EBT_INSTANCE_PASS == stAck.stBattleInfo.byBattleResult)
		{
			DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage; //通关奖励宝箱信息
			DT_DATA_AFTER_PASS_ENCOURAGE_BOX stAfterPassEncourage; //领取通关奖励宝箱后玩家信息
			memset(&stPassEncourage, 0x00, sizeof(stPassEncourage));
			memset(&stAfterPassEncourage, 0x00, sizeof(stAfterPassEncourage));


			UINT16 wErrCode =  poPlayer->GetInstance().OpenEncourageBox(stPassEncourage, &stAfterPassEncourage);
			//检测背包是否满了
			if(ERR_OPEN_INSTANCE_ENCOURAGE_BOX::ID_BAG_FULL == wErrCode)
			{
				//查看城镇的副本是否有打通关记录。
				CInstanceRecord* poInstanceRecord = NULL;
				poInstanceRecord = oInstance.GetSingleInstaceRecord(EIT_TOWER, pReq->byTownIdx, byCurInstance);
				if (NULL != poInstanceRecord)
				{
					poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime = SDTimeSecs();
				}
				stAck.wErrCode = ERR_BATTLE::ID_BAG_FULL;
			}
		}
		poPlayer->ExitInstance();
	}
	//@ end 2013-02-23

	poPlayer->GetDT_TASK_DATA_LIST_CLI_Change(stAck.stTaskInfoList);
	poPlayer->GetDT_TASK_COND_RECORD_LIST(stAck.stTaskRecordList);
	poPlayer->GetPhystrength().GetPhyStrengthInfo(stAck.stPhyStrengthInfo);
	memcpy(stAck.abyGuide, poPlayer->GetDT_PLAYER_BASE_DATA().stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);
	stAck.stPowerInfo.dwCurPower = poPlayer->GetDT_PLAYER_BASE_DATA().dwPower;

	poUser->SendMsg((CHAR*)(&stAck), CLIGS_BATTLE_ACK4);
	poPlayer->RecordLastBattleAck4(stAck);

	return TRUE;
}

BOOL CGTProcessor::OnExitInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EXIT_INSTANCE_REQ* pReq = (PKT_CLIGS_EXIT_INSTANCE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_EXIT_INSTANCE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->ExitInstanceAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;

}

/*
BOOL CGTProcessor::OnPickReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_PICKUP_REQ* pReq = (PKT_CLIGS_PICKUP_REQ*)pBody;
	if ( sizeof(PKT_CLIGS_PICKUP_REQ) != dwLen )
		return FALSE;

	CUser* poUser = (CUser*)pUser;

	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wErrCode = CPlayerMgr::Instance()->PickUp(poPlayer, pReq->byKind, pReq->dwID);

	UINT16 wMsgID = CGTPktBuilder::Instance()->PickUpAck(wErrCode);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


	return TRUE;
}

*/

BOOL CGTProcessor::OnGetSceneInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_SCENE_INSTANCE_REQ* pReq = (PKT_CLIGS_GET_SCENE_INSTANCE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GET_SCENE_INSTANCE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }
    static PKT_CLIGS_GET_SCENE_INSTANCE_ACK stAck;
    memset(&stAck, 0, sizeof(PKT_CLIGS_GET_SCENE_INSTANCE_ACK));
    //查找是否存在记录
    poPlayer->GetInstance().GetInstanceRecord(pReq->wIndex, stAck);
    poPlayer->GetPhystrength().GetPhyStrengthInfo(stAck.stCurPhyStrengthInfo);
    poPlayer->GetEmprisePrompt(stAck.stPromptInfo.astBuildPromptInfo[stAck.stPromptInfo.byBuildNum++]);
    stAck.dwFlyExp = poPlayer->GetCoachHero()->GetFlyExp();

    poUser->SendMsg((CHAR*)(&stAck), CLIGS_GET_SCENE_INSTANCE_ACK);
    return TRUE;
}
BOOL CGTProcessor::OnOpenSmithyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SMITHY_REQ* pReq = (PKT_CLIGS_OPEN_SMITHY_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_SMITHY_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenSmithyAck(poPlayer, pReq->byBuildKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnSmithyBuyEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SMITHY_BUY_EQUIP_REQ* pReq = (PKT_CLIGS_SMITHY_BUY_EQUIP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_SMITHY_BUY_EQUIP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->SmithyBuyEquipAck(poPlayer, pReq->wHeroKindID, pReq->byEquipPos);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnStrengthenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EQUIP_STRENGTHEN_REQ* pReq = (PKT_CLIGS_EQUIP_STRENGTHEN_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_EQUIP_STRENGTHEN_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->StrengthenAck(poPlayer, pReq->wKindID, pReq->wIdx, pReq->byStrengthenType, pReq->wLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnEquipEnchantReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EQUIP_ENCHANT_REQ* pReq = (PKT_CLIGS_EQUIP_ENCHANT_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_EQUIP_ENCHANT_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->EquipEnchantAck(poPlayer, pReq->wKindID, pReq->wIdx, pReq->wLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 获取装备开光配置和所需资源请求
BOOL CGTProcessor::OnGetOpenLightInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_OPENLIGHT_INFO_REQ* pReq = (PKT_CLIGS_GET_OPENLIGHT_INFO_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GET_OPENLIGHT_INFO_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->GetOpenLightInfoAck(poPlayer, pReq->wEquipKindID, pReq->wEquipIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 装备开光请求
BOOL CGTProcessor::OnOpenLightReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPENLIGHT_INFO_REQ* pReq = (PKT_CLIGS_OPENLIGHT_INFO_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPENLIGHT_INFO_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenLightAck(poPlayer, pReq->wEquipKindID, pReq->wEquipIdx, pReq->byGoldFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}






//打开招募所请求
BOOL CGTProcessor::OnOpenRecruitReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_RECRUIT_REQ* pReq = (PKT_CLIGS_OPEN_RECRUIT_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_RECRUIT_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenRecruitAck(poPlayer, pReq->byBuildKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//招募武将请求
BOOL CGTProcessor::OnRecruitHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECRUIT_HERO_REQ* pReq = (PKT_CLIGS_RECRUIT_HERO_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_RECRUIT_HERO_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->RecruitHeroAck(poPlayer, pReq->wHeroKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnDisbandHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DISBAND_HERO_REQ* pReq = (PKT_CLIGS_DISBAND_HERO_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_DISBAND_HERO_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->DisbandHeroAck(poPlayer, pReq->wHeroKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnOpenBagReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_BAG_REQ* pReq = (PKT_CLIGS_OPEN_BAG_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_BAG_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenBagAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnRecvTaskEncourage(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ* pReq = (PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->RecvTaskEncourageAck(poPlayer, pReq->byTaskLineIdx, pReq->dwTaskIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnOpenRaceBuildReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_RACE_BUILD_REQ* pReq = (PKT_CLIGS_OPEN_RACE_BUILD_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_RACE_BUILD_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenRaceBuildAck(poPlayer, pReq->byBuildKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnChallengeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CHALLENGE_REQ* pReq = (PKT_CLIGS_CHALLENGE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_CHALLENGE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    //判断是否存在竞技
    CRace* poRace = poPlayer->GetRace();
    UINT16 wErrCode = 0;
    if(NULL == poRace)
    {
        wErrCode = ERR_CHALLENGE::ID_UNLOCK_ERR;
        UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(wErrCode, poPlayer, NULL);
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        return TRUE;
    }
    //判断条件中
    wErrCode = poRace->CkCondition();
    if (0 != wErrCode)
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(wErrCode, poPlayer, NULL);
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        return TRUE;
    }

    //查看是否超出挑战范围
    if(CRaceMgr::Instance()->CkOutChallengeRang(poRace->GetDT_RACE_BASE_DATA().dwRank, pReq->dwRank))
    {
        wErrCode = ERR_CHALLENGE::ID_OUT_CHALLENGE_RANG_ERR;
        UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(wErrCode, poPlayer, NULL);
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        return TRUE;
    }

    //查看玩家是否存在
    const SPlayerBaseData* pstPlayerBaseData = NULL;
    //挑战引导，则和机器人打
    if((pReq->dwRank == poRace->GetDT_RACE_BASE_DATA().dwRank) && (poPlayer->CKGuide( EGF_RACE)))
    {
        pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(CRacePropMgr::Instance()->GetRaceProp().dwRobotID);
    }
    else
    {
        pstPlayerBaseData = CRaceMgr::Instance()->GetPlayerBaseData(pReq->dwRank);
    }
    if(NULL == pstPlayerBaseData)
    {
        wErrCode = ERR_CHALLENGE::ID_ENEMY_NOT_EXIST_ERR;
        UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(wErrCode, poPlayer, NULL);
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        return TRUE;
    }


    //查找被挑战玩家是否在内存中，若不存在则去数据库中取，并实例化
    CPlayer* poEnemy = CPlayerMgr::Instance()->FindPlayer(pstPlayerBaseData->dwID);
    if(NULL == poEnemy)
    {
        //取对方数据
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_CHALLENGE, pstPlayerBaseData->dwID);
        return TRUE;
    }
    UINT16 wMsgID = CGTPktBuilder::Instance()->ChallengeAck(0, poPlayer, poEnemy);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnRecvRankEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ* pReq = (PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->RecvRankEncourageAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//打开商店请求
BOOL CGTProcessor::OnOpenShopReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SHOP_REQ* pReq = (PKT_CLIGS_OPEN_SHOP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_SHOP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenShopAck(poPlayer, pReq->byTabID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


//购买道具请求
BOOL CGTProcessor::OnBuyItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BUY_ITEM_REQ* pReq = (PKT_CLIGS_BUY_ITEM_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_BUY_ITEM_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->BuyItemAck(poPlayer, pReq->byTabID, pReq->wKindID, pReq->wPileCount);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnSellItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SELL_ITEM_REQ* pReq = (PKT_CLIGS_SELL_ITEM_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_SELL_ITEM_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->SellItemAck(poPlayer, pReq->wKindID, pReq->wIdx, pReq->wNum);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EQUIP_REQ* pReq = (PKT_CLIGS_EQUIP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_EQUIP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->EquipAck(poPlayer, pReq->wHeroKindID, pReq->byDressType, pReq->wDressItemKindID, pReq->wDressItemIdx, pReq->wUndressItemKindID, pReq->wUndressItemIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnQuickEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_QUICK_EQUIP_REQ* pReq = (PKT_CLIGS_QUICK_EQUIP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_QUICK_EQUIP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->QuickEquipAck(poPlayer, pReq->wHeroKindID, pReq->byEquipNum, pReq->astEquipIDLst);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnRegisterUserNameReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    //todo...

    return TRUE;
}

BOOL CGTProcessor::OnEmbattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EMBATTLE_REQ* pReq = (PKT_CLIGS_EMBATTLE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_EMBATTLE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->EmbattleAck(poPlayer, pReq->stFormationInfo);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnClrPlayerDataReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLR_PLAYER_DATA_REQ* pReq = (PKT_CLIGS_CLR_PLAYER_DATA_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_CLR_PLAYER_DATA_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //DBG_INFO("[%u]收到玩家清除数据", poPlayer->GetDT_PLAYER_BASE_DATA().dwID);
    //CPlayerMgr::Instance()->ClrPlayerData(poPlayer);

    return TRUE;
}


BOOL CGTProcessor::OnOpenEmbattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_EMBATTLE_REQ* pReq = (PKT_CLIGS_OPEN_EMBATTLE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_EMBATTLE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenEmbattleAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnMakeItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_MAKE_ITEM_REQ* pReq = (PKT_CLIGS_MAKE_ITEM_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_MAKE_ITEM_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->MakeItemAck(poPlayer, pReq->wItemKindID, pReq->wEquipKindID, pReq->wEquipIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnOpenEncourageInstanceBoxReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ* pReq = (PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenInstanceEncourageBoxAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnIncreaseBagSize(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_INCREASE_BAG_SIZE_REQ* pReq = (PKT_CLIGS_INCREASE_BAG_SIZE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_INCREASE_BAG_SIZE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnIncreaseBagSizeAck(poPlayer, pReq->byInc2Size, pReq->byGoldFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnHookReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ONHOOK_REQ* pReq = (PKT_CLIGS_ONHOOK_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_ONHOOK_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnhookAck(poPlayer, pReq->byRunTimes, pReq->stInstanceID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

/*
BOOL CGTProcessor::OnInterruptOnhook(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
UINT32 dwTransID            = pPipeHeader->dwTransID;

PKT_CLIGS_INTERRUPT_ONHOOK_REQ* pReq = (PKT_CLIGS_INTERRUPT_ONHOOK_REQ*)pBody;
if ( sizeof(PKT_CLIGS_INTERRUPT_ONHOOK_REQ) != dwLen )
    return FALSE;

CUser* poUser = (CUser*)pUser;

CPlayer* poPlayer = poUser->GetPlayer();
if(!poPlayer)
{
    return FALSE;
}

UINT16 wMsgID = CGTPktBuilder::Instance()->OnInterruptOnhookAck(poPlayer);
if(INVALID_MSGID == wMsgID)
{
    return FALSE;
}
poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

return TRUE;
}

BOOL CGTProcessor::OnNoCDOnhook(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
UINT32 dwTransID            = pPipeHeader->dwTransID;

PKT_CLIGS_INTERRUPT_ONHOOK_REQ* pReq = (PKT_CLIGS_INTERRUPT_ONHOOK_REQ*)pBody;
if ( sizeof(PKT_CLIGS_INTERRUPT_ONHOOK_REQ) != dwLen )
    return FALSE;

CUser* poUser = (CUser*)pUser;

CPlayer* poPlayer = poUser->GetPlayer();
if(!poPlayer)
{
    return FALSE;
}

UINT16 wMsgID = CGTPktBuilder::Instance()->OnNoCDOnhookAck(poPlayer);
if(INVALID_MSGID == wMsgID)
{
    return FALSE;
}
poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

return TRUE;
}

BOOL CGTProcessor::OnOnhookResult(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
UINT32 dwTransID            = pPipeHeader->dwTransID;

PKT_CLIGS_ONHOOK_RESULT_REQ* pReq = (PKT_CLIGS_ONHOOK_RESULT_REQ*)pBody;
if ( sizeof(PKT_CLIGS_ONHOOK_RESULT_REQ) != dwLen )
    return FALSE;

CUser* poUser = (CUser*)pUser;

CPlayer* poPlayer = poUser->GetPlayer();
if(!poPlayer)
{
    return FALSE;
}

UINT16 wMsgID = CGTPktBuilder::Instance()->OnOnhookResultAck(poPlayer);
if(INVALID_MSGID == wMsgID)
{
    return FALSE;
}
poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

return TRUE;
}
*/

BOOL CGTProcessor::OnClrChallengeCD(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLR_CHALLENGE_CD_REQ* pReq = (PKT_CLIGS_CLR_CHALLENGE_CD_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_CLR_CHALLENGE_CD_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClrChallengeCDAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnRecvOnlineEncourage(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ* pReq = (PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvOnlineEncourageAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnSyncTime(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SYNC_TIME_REQ* pReq = (PKT_CLIGS_SYNC_TIME_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_SYNC_TIME_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSyncTimeAck(pReq->qwCliTime);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnRegisterDspName(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLILS_REGISTER_DSPNAME_REQ* pReq = (PKT_CLILS_REGISTER_DSPNAME_REQ*)pBody;
    if ( sizeof(PKT_CLILS_REGISTER_DSPNAME_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRegisterDspNameAck(poPlayer, pReq->aszDspName);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;

}

BOOL  CGTProcessor::OnUseGoodsReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLILS_USE_GOODS_REQ* pReq = (PKT_CLILS_USE_GOODS_REQ*)pBody;
    if ( sizeof(PKT_CLILS_USE_GOODS_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->UseGoodsAck(poPlayer, pReq->wGoodsKindID, pReq->byGetBagInfoFlag, pReq->wUseNum);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnOpenLoginEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ* pReq = (PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ*)pBody;
    if ( sizeof(PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenLoginEncourageAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnRateLoginEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ* pReq = (PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ*)pBody;
    if ( sizeof(PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRateLoginEncourageAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnPutLoginEncourageIntoBagReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ* pReq = (PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ*)pBody;
    if ( sizeof(PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnPutLoginEncourageIntoBagAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnGetRankReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_RANK_REQ* pReq = (PKT_CLIGS_GET_RANK_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GET_RANK_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetRankAck();
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnAcceptTaskReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    /*
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ACCEPT_TASK_REQ* pReq = (PKT_CLIGS_ACCEPT_TASK_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_ACCEPT_TASK_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAcceptTaskAck(poPlayer, pReq->byTaskLineIdx, pReq->dwTaskIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    */

    return TRUE;
}



BOOL CGTProcessor::OnGiveUpTaskReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GIVEUP_TASK_REQ* pReq = (PKT_CLIGS_GIVEUP_TASK_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GIVEUP_TASK_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGiveUpTaskAck(poPlayer, pReq->byTaskLineIdx, pReq->dwTaskIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnGetRaceBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_RACE_BATTLELOG_REQ* pReq = (PKT_CLIGS_GET_RACE_BATTLELOG_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GET_RACE_BATTLELOG_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetRaceBattleLogAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnOpenCaptureTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_CAPTURE_TAB_REQ* pReq = (PKT_CLIGS_OPEN_CAPTURE_TAB_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_CAPTURE_TAB_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenCaptureTabAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnPlunderReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_PLUNDER_REQ* pReq = (PKT_CLIGS_PLUNDER_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_PLUNDER_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wErrCode = 0;
    CPlunder* poMyselfPlunder = poPlayer->GetPlunder();
    if(NULL == poMyselfPlunder)
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(ERR_PLUNDER::ID_UNLOCK, poPlayer);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }

    UINT32 dwMyPlayerID = poPlayer->GetDT_PLAYER_BASE_DATA().dwID;
    if(1 == pReq->byResistFlag)//若是反抗则说明自己是俘虏
    {
        pReq->dwCaptureID = dwMyPlayerID;
    }
    //记录协议信息参数
    poMyselfPlunder->SetReqParam(dwMyPlayerID, *pReq);
    if(dwMyPlayerID == pReq->dwEnemyID)
    {
        wErrCode = ERR_PLUNDER::ID_ENEMY_CANNOT_MYSELF;
        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(wErrCode, poPlayer);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }

    //查看玩家是否存在
    if((NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(pReq->dwEnemyID)) ||
            (NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(pReq->dwCaptureID)))
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(ERR_PLUNDER::ID_ENEMY_NOT_EXIST, poPlayer);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }

        DBG_INFO("OnPlunderReq dwEnemyID:%u, dwCaptureID:%u Not Exit\n", pReq->dwEnemyID, pReq->dwCaptureID);
        return TRUE;
    }

    //查找玩家是否在内存中，若不存在则去数据库中取，并实例化
    CPlayer* poEnemy = CPlayerMgr::Instance()->FindPlayer(pReq->dwEnemyID);
    if(pReq->dwCaptureID == pReq->dwEnemyID) //同一个人
    {
        if(NULL == poEnemy)
        {
            //取对方数据
            CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_PLUNDER, pReq->dwEnemyID, pReq->dwCaptureID, pReq->byResistFlag);
            return TRUE;
        }
        else
        {
            UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(0, poPlayer, poEnemy, poEnemy);
            if(INVALID_MSGID != wMsgID)
            {
                poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
            }
            return TRUE;
        }
    }

    ///////////////////////////////

    //不同的人
    CPlayer* poCapture = CPlayerMgr::Instance()->FindPlayer(pReq->dwCaptureID);
    if((NULL == poEnemy) && (NULL == poCapture))
    {
        //取对方数据
        CGetPlayerDataMgr::Instance()->GetPlayerData2(dwTransID, GET_PLAYERDATATYPE_PLUNDER, pReq->dwEnemyID, pReq->dwCaptureID, pReq->byResistFlag);
        return TRUE;
    }
    else if((NULL == poEnemy) && (NULL != poCapture))
    {
        //取对方数据
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_PLUNDER, pReq->dwEnemyID, pReq->dwCaptureID, pReq->byResistFlag);
        return TRUE;
    }
    else if((NULL != poEnemy) && (NULL == poCapture))
    {
        //取对方数据
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_PLUNDER, pReq->dwCaptureID, pReq->dwCaptureID, pReq->byResistFlag);
        return TRUE;
    }
    //数据都存在
    else
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->PlunderAck(0, poPlayer, poEnemy, poCapture);
        if(INVALID_MSGID != wMsgID)
        {
            poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
        return TRUE;
    }


    return TRUE;
}

BOOL CGTProcessor::OnOpenEnemyTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_ENEMY_TAB_REQ* pReq = (PKT_CLIGS_OPEN_ENEMY_TAB_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_ENEMY_TAB_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenEnemyTabAck(poPlayer, pReq->wStartIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnOpenPlunderRecommendPlayerReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ* pReq = (PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlunderRecommendPlayerAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnRefreshPlunderRecommendReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ* pReq = (PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRefreshPlunderRecommendAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnFreeCaptureReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_FREE_CAPTURE_REQ* pReq = (PKT_CLIGS_FREE_CAPTURE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_FREE_CAPTURE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnFreeCaptureAck(poPlayer, pReq->dwCapturePlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnOpenMoneyTreeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_MONEY_TREE_REQ* pReq = (PKT_CLIGS_OPEN_MONEY_TREE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_MONEY_TREE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenMoneyTreeAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnRockMoneyTreeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ROCK_MONEY_TREE_REQ* pReq = (PKT_CLIGS_ROCK_MONEY_TREE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_ROCK_MONEY_TREE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRockMoneyTreeAck(poPlayer, pReq->byGuideFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnGetPlunderBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ* pReq = (PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetPlunderBattleLogAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnOpenGodweaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GODWEAPON_REQ* pReq = (PKT_CLIGS_OPEN_GODWEAPON_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_GODWEAPON_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGodweaponAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnUpgradeGodweaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_UPGRADE_GODWEAPON_REQ* pReq = (PKT_CLIGS_UPGRADE_GODWEAPON_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_UPGRADE_GODWEAPON_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpgradeGodweaponAck(poPlayer, pReq->wHeroKindID, pReq->wQuality, pReq->wLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnFreeGodweaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_FREE_GODWEAPON_REQ* pReq = (PKT_CLIGS_FREE_GODWEAPON_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_FREE_GODWEAPON_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnFreeGodweaponAck(poPlayer, pReq->wHeroKindID, pReq->byUseGold);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnOpenAwakenTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_AWAKEN_TAB_REQ* pReq = (PKT_CLIGS_OPEN_AWAKEN_TAB_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_AWAKEN_TAB_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenAwakenTabReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnAwakenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_AWAKEN_REQ* pReq = (PKT_CLIGS_AWAKEN_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_AWAKEN_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAwakenReq(poPlayer, pReq->byQuickFlag, pReq->byGoldFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnOpenDressStudyTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ* pReq = (PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenDressStudyTabReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnDressStudyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DRESS_STUDY_REQ* pReq = (PKT_CLIGS_DRESS_STUDY_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_DRESS_STUDY_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDressStudyReq(poPlayer, pReq->wHeroKindID, pReq->byBagGridIdx, pReq->byHeroGridIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnStudyToExpReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_STUDY_TO_EXP_REQ* pReq = (PKT_CLIGS_STUDY_TO_EXP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_STUDY_TO_EXP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnStudyToExpReq(poPlayer, pReq->byBagGridIdx, pReq->byAllFlag, pReq->abySelectConvertColor);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnStudyUpgradeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_STUDY_UPGRADE_REQ* pReq = (PKT_CLIGS_STUDY_UPGRADE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_STUDY_UPGRADE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnStudyUpgradeReq(poPlayer, pReq->wHeroKindID, pReq->byGridIdx, pReq->byLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


//////////////////////////////////////
BOOL CGTProcessor::OnOpenAwakenTabReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_AWAKEN_TAB_REQ2* pReq = (PKT_CLIGS_OPEN_AWAKEN_TAB_REQ2*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_AWAKEN_TAB_REQ2) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenAwakenTabReq2(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnAwakenReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_AWAKEN_REQ2* pReq = (PKT_CLIGS_AWAKEN_REQ2*)pBody;
    if ( sizeof(PKT_CLIGS_AWAKEN_REQ2) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAwakenReq2(poPlayer, pReq->byQuickFlag, pReq->byGoldFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnOpenDressStudyTabReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2* pReq = (PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenDressStudyTabReq2(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnDressStudyReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DRESS_STUDY_REQ2* pReq = (PKT_CLIGS_DRESS_STUDY_REQ2*)pBody;
    if ( sizeof(PKT_CLIGS_DRESS_STUDY_REQ2) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDressStudyReq2(poPlayer, pReq->wHeroKindID, pReq->byBagGridIdx, pReq->byHeroGridIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnStudyToExpReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_STUDY_TO_EXP_REQ2* pReq = (PKT_CLIGS_STUDY_TO_EXP_REQ2*)pBody;
    if ( sizeof(PKT_CLIGS_STUDY_TO_EXP_REQ2) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnStudyToExpReq2(poPlayer, pReq->byBagGridIdx, pReq->byAllFlag, pReq->abySelectConvertColor);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnStudyUpgradeReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_STUDY_UPGRADE_REQ2* pReq = (PKT_CLIGS_STUDY_UPGRADE_REQ2*)pBody;
    if ( sizeof(PKT_CLIGS_STUDY_UPGRADE_REQ2) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnStudyUpgradeReq2(poPlayer, pReq->wHeroKindID, pReq->byGridIdx, pReq->byLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}
/////////////////////////////

BOOL CGTProcessor::OnDelEnemyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DEL_ENEMY_REQ* pReq = (PKT_CLIGS_DEL_ENEMY_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_DEL_ENEMY_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDelEnemyAck(poPlayer, pReq->dwEnemyID, pReq->wStartIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnCollectIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COLLECT_INCOME_REQ* pReq = (PKT_CLIGS_COLLECT_INCOME_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_COLLECT_INCOME_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCollectIncomeAck(poPlayer, pReq->dwCapturePlayerID, pReq->dwIncomeValue);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnBleedCaptureReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BLEED_CAPTURE_REQ* pReq = (PKT_CLIGS_BLEED_CAPTURE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_BLEED_CAPTURE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBleedCaptureAck(poPlayer, pReq->dwCapturePlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnSetWorkTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SET_WORK_TYPE_REQ* pReq = (PKT_CLIGS_SET_WORK_TYPE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_SET_WORK_TYPE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSetWorkTypeAck(poPlayer, pReq->dwCapturePlayerID, pReq->byIncomeType);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnDriveAwayReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DRIVE_AWAY_OWNER_REQ* pReq = (PKT_CLIGS_DRIVE_AWAY_OWNER_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_DRIVE_AWAY_OWNER_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->OnDriveAwayAck(poPlayer, ERR_DRIVE_AWAY_OWNER::ID_PLUNDER_UNLOCK);
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        return TRUE;
    }

    UINT32 dwCaptureOwnerPlayerID = poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID;
    CPlayer* poCaptureOwner = CPlayerMgr::Instance()->FindPlayer(dwCaptureOwnerPlayerID);
    if(NULL == poCaptureOwner)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_DRIVEAWAY, dwCaptureOwnerPlayerID);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDriveAwayAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnRecvLeftIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_LEFT_INCOME_REQ* pReq = (PKT_CLIGS_RECV_LEFT_INCOME_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_RECV_LEFT_INCOME_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }


    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvLeftIncomeAck(poPlayer, pReq->byGridIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}




BOOL CGTProcessor::OnVisitHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_VISIT_HERO_REQ* pReq = (PKT_CLIGS_VISIT_HERO_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_VISIT_HERO_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnVisitHeroAck(poPlayer, pReq->wHeroKindID, pReq->bySizeFlag, pReq->byGoldFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnBuyPhystrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BUY_PHYSTRENGTH_REQ* pReq = (PKT_CLIGS_BUY_PHYSTRENGTH_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_BUY_PHYSTRENGTH_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBuyPhystrengthAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnOpenMaintownReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_MAINTOWN_REQ* pReq = (PKT_CLIGS_OPEN_MAINTOWN_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_MAINTOWN_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenMaintownAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnRecvIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_MAINTOWN_INCOME_REQ* pReq = (PKT_CLIGS_RECV_MAINTOWN_INCOME_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_RECV_MAINTOWN_INCOME_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvIncomeAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnLevyIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ* pReq = (PKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }




    UINT16 wMsgID = CGTPktBuilder::Instance()->OnLevyIncomeAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}
//装备合成
BOOL CGTProcessor::OnComposeEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COMPOSE_EQUIP_REQ* pReq = (PKT_CLIGS_COMPOSE_EQUIP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_COMPOSE_EQUIP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnComposeEquipAck(poPlayer, pReq->wEquipItemID, pReq->wEquipIdx, pReq->byGoldComposeFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



//打开购买体力面板
BOOL  CGTProcessor::OnOpenPhyStrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_PHYSTRENGTH_REQ* pReq = (PKT_CLIGS_OPEN_PHYSTRENGTH_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_PHYSTRENGTH_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPhyStrengthAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



//清除强化CD
BOOL  CGTProcessor::OnClrStrengthenCDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLR_STRENGTHEN_CD_REQ* pReq = (PKT_CLIGS_CLR_STRENGTHEN_CD_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_CLR_STRENGTHEN_CD_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClrStrengthenCDAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::SendMsg( CUser *poUser, UINT16 wMsgID )
{
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}
BOOL	CGTProcessor::OnOpenCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_CROP_REQ* pReq = (PKT_CLIGS_OPEN_CROP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_CROP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenCropReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnOpenCropFieldReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_CROP_FIELD_REQ* pReq = (PKT_CLIGS_OPEN_CROP_FIELD_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_CROP_FIELD_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenCropFieldReq(poPlayer, pReq->byFieldID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL  CGTProcessor::OnCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CROP_REQ* pReq = (PKT_CLIGS_CROP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_CROP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCropReq(poPlayer, pReq->byFieldID, pReq->wCropKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL  CGTProcessor::OnGainCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GAIN_CROP_REQ* pReq = (PKT_CLIGS_GAIN_CROP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_GAIN_CROP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGainCropReq(poPlayer, pReq->byFieldID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}
BOOL CGTProcessor::OnClrCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLR_CROP_REQ* pReq = (PKT_CLIGS_CLR_CROP_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_CLR_CROP_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClrCropReq(poPlayer, pReq->byFieldID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL	CGTProcessor::OnOpenExperienceDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_EXPERIENCE_REQ* pReq = (PKT_CLIGS_OPEN_EXPERIENCE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_EXPERIENCE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenExperienceDrugReq(poPlayer, pReq->wHeroID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnUserExperienceDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_USER_EXPERIENCE_REQ* pReq = (PKT_CLIGS_USER_EXPERIENCE_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_USER_EXPERIENCE_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUserExperienceDrugReq(poPlayer, pReq->wHeroID, pReq->wExperienceDrugID, pReq->wRemainGoldExperienceDrugCnt);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnOpenScienceBuildReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ* pReq = (PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenScienceBuildReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnUpdateScienceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_UPDATE_SCIENCE_REQ* pReq = (PKT_CLIGS_UPDATE_SCIENCE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_UPDATE_SCIENCE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpdateScienceReq(poPlayer, pReq->byKind, pReq->wCurLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}
BOOL CGTProcessor::OnOpenMeltEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_MELT_EQUIP_REQ* pReq = (PKT_CLIGS_OPEN_MELT_EQUIP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_MELT_EQUIP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenMeltEquipReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnMeltReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_MELT_REQ* pReq = (PKT_CLIGS_MELT_REQ*)pBody;

    if( sizeof(PKT_CLIGS_MELT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnMeltReq(poPlayer, pReq->byGoldFlag, pReq->wEquipNum, pReq->astMeltEquipDataList,
                    pReq->wGoodNum, pReq->astMeltGoodDataList);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnOpenUpdateAttrPannelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ* pReq = (PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenUpdateAttrPannelReq(poPlayer, pReq->wKindID, pReq->byAttrKind);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnUpdateAttrReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    PKT_CLIGS_UPDATE_ATTR_REQ* pReq = (PKT_CLIGS_UPDATE_ATTR_REQ*)pBody;

    if( sizeof(PKT_CLIGS_UPDATE_ATTR_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpdateAttrReq( poPlayer, pReq->wKindID, pReq->byAttrType, pReq->byUpdateType, pReq->wCurCount );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}


BOOL CGTProcessor::OnComposeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COMPOSE_REQ* pReq = (PKT_CLIGS_COMPOSE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COMPOSE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnComposeReq(poPlayer, pReq->wItemID, pReq->byComposeFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//获取物品信息
BOOL CGTProcessor::OnGetItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    tagPKT_CLIGS_GET_ITEM_REQ* pReq = (tagPKT_CLIGS_GET_ITEM_REQ*)pBody;

    if( sizeof(tagPKT_CLIGS_GET_ITEM_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetItemReq(poPlayer, pReq->wItemKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


//BOOL CGTProcessor::OnGetOrderIDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
//{
//    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
//    UINT32 dwTransID            = pPipeHeader->dwTransID;
//
//    tagPKT_CLIGS_GET_ORDER_ID_REQ * pReq = (tagPKT_CLIGS_GET_ORDER_ID_REQ*)pBody;
//    if( sizeof(tagPKT_CLIGS_GET_ORDER_ID_REQ) != dwLen )
//    {
//        return FALSE;
//    }
//
//    CUser* poUser = (CUser*)pUser;
//
//    CPlayer* poPlayer = poUser->GetPlayer();
//    if(!poPlayer)
//    {
//        return FALSE;
//    }
//
//    //发送数据给兑换服务器
//    UINT16 wMsgID = CRechargePktBuilder::Instance()->QueryOrderID( poPlayer->GetDT_PLAYER_BASE_DATA().dwID,
//                    poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, poPlayer->GetLevel(), poUser->GetZoneID(), poUser->GetUserID() );
//
//    if(INVALID_MSGID == wMsgID)
//    {
//        return FALSE;
//    }
//
//    auto pClient = ( (CGSApplication *)SDGetApp() )->GetRechargeClientMgr()->GetClient();
//    if( NULL == pClient || !pClient->IsConnect() )
//    {
//        wMsgID = CGTPktBuilder::Instance()->OnGenOrderIDReqError( ERR_GET_ORDER_ID::ID_ERR_DISCONNECT_EXCHANGE_SERVER );
//        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
//        USR_IMPORTANT( _SDT( "disconnect from exchange server" ) );
//        return TRUE;
//    }
//
//    BOOL bSendRet = pClient->SendMsg( wMsgID, CRechargePktBuilder::Instance()->GetPacketBuffer() );
//    if( !bSendRet )
//    {
//        CGTPktBuilder::Instance()->OnGenOrderIDReqError( ERR_GET_ORDER_ID::ID_ERR_SEND_TO_EXCHANGE_SERVER );
//        poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
//        USR_IMPORTANT( _SDT( "send to exchange server error!" ) );
//        return TRUE;
//    }
//
//    USR_INFO( _SDT( "data process success" ) );
//    return TRUE;
//
//}

//同步新充值修改
BOOL CGTProcessor::OnGetOrderIDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    tagPKT_CLIGS_GET_ORDER_ID_REQ * pReq = (tagPKT_CLIGS_GET_ORDER_ID_REQ*)pBody;
    if( sizeof(tagPKT_CLIGS_GET_ORDER_ID_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    if(CDBPktBuilder::Instance()->OnGetOrderIDReq(poPlayer, poUser))
    {
        USR_INFO( _SDT( "data process success" ) );
    }
    else
    {
        USR_INFO( _SDT( "data process failed!" ) );
    }

    return TRUE;

}

BOOL CGTProcessor::OnComposeSeedReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SEED_COMPOSE_REQ* pReq = (PKT_CLIGS_SEED_COMPOSE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SEED_COMPOSE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnComposeSeedReq(poPlayer, pReq->wItemID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//聊天
BOOL CGTProcessor::OnChatReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CHAT_REQ* pReq = (PKT_CLIGS_CHAT_REQ*)pBody;

    if( sizeof(PKT_CLIGS_CHAT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //检测是不是是CDkey
    char cdkey[56] = {0};
    strncpy(cdkey, (char*)pReq->abyChatContent, 3);
    if((0 == SDStrcasecmp(cdkey, "cky")) && (pReq->dwChatLen >= 5) && (pReq->dwChatLen <= 15))
    {
        CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_CHATENCOURAGE);
        if (NULL == poHttpCmd)
        {
            SYS_CRITICAL( _SDT("[%s:%d] player:%u in memory"), MSG_MARK, poPlayer->GetID());
            return TRUE;
        }
        poHttpCmd->SetUserData(pReq, sizeof(PKT_CLIGS_CHAT_REQ), poPlayer->GetID());
        poHttpCmd->Init();
        if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
        {
            CHttpMgr::Instance()->FreeCmd(poHttpCmd);
            SYS_CRITICAL( _SDT("[%s:%d] can't AddCommand player:%u in memory"), MSG_MARK, poPlayer->GetID());
        }
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnChatReq(poPlayer, pReq->dwPlayerID, pReq->byChatType, pReq->abyChatContent, pReq->dwChatLen);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;

}


BOOL CGTProcessor::OnOpenChatReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_CHAT_REQ* pReq = (PKT_CLIGS_OPEN_CHAT_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_CHAT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenChatReq(poPlayer, pReq->dwSerialNumber);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


//重置爬塔
BOOL CGTProcessor::OnClimbTowerResetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    tagPKT_CLIGS_CLIMB_TOWER_RESET_REQ* pReq = (tagPKT_CLIGS_CLIMB_TOWER_RESET_REQ*)pBody;

    if( sizeof(tagPKT_CLIGS_CLIMB_TOWER_RESET_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    pReq->wSceneIdx = 3;
    pReq->byTownIdx = 1;
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClimbTowerResetReq(poPlayer, pReq->wSceneIdx, pReq->byTownIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnOpenActivityCenterReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ* pReq = (PKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenActivityCenterAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnOpneEatBreadReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPNE_EAT_BREAD_REQ* pReq = (PKT_CLIGS_OPNE_EAT_BREAD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPNE_EAT_BREAD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpneEatBreadAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}



BOOL CGTProcessor::OnObtainPhystrengthenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ* pReq = (PKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnObtainPhystrengthenAck(poPlayer, pReq->byHaveObtainPhyStrengthenCli, pReq->dwHaveObtainCoinCli);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}



BOOL CGTProcessor::OnOLeaveEatBreadReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEAVE_EAT_BREAD_REQ* pReq = (PKT_CLIGS_LEAVE_EAT_BREAD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_LEAVE_EAT_BREAD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOLeaveEatBreadAck(poPlayer, pReq->byHaveObtainPhyStrengthenCli, pReq->dwHaveObtainCoinCli);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}



//爬塔
BOOL CGTProcessor::OnClimbTowerOnHookReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ* pReq = (tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ*)pBody;

    if( sizeof(tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }
    pReq->wSceneIdx = 3;
    pReq->byTownIdx = 1;
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClimbTowerOnHookReq(poPlayer, pReq->wSceneIdx, pReq->byTownIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnOpenPaoshangBuildReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ* pReq = (PKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPaoshangBuildReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenYabiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_YABIAO_REQ* pReq = (PKT_CLIGS_OPEN_YABIAO_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_YABIAO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }


    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenYabiaoReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnFlushBiaoQualityReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_FLUSH_BIAO_QUALITY_REQ* pReq = (PKT_CLIGS_FLUSH_BIAO_QUALITY_REQ*)pBody;

    if( sizeof(PKT_CLIGS_FLUSH_BIAO_QUALITY_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnFlushBiaoQualityReq( poPlayer, pReq->wQuality, pReq->byDirectMaxLevel );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;

}



BOOL CGTProcessor::OnBeginYabiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BEGIN_YABIAO_REQ* pReq = (PKT_CLIGS_BEGIN_YABIAO_REQ*)pBody;

    if( sizeof(PKT_CLIGS_BEGIN_YABIAO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBeginYabiaoReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenJiebiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_JIEBIAO_REQ* pReq = (PKT_CLIGS_OPEN_JIEBIAO_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_JIEBIAO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenJiebiaoReq( poPlayer, pReq->byViewYabiaoType,
                    pReq->wLongestYabiaoEndTime, pReq->byNeedPlayerNum );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnBeginJiebiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BEGIN_JIEBIAO_REQ* pReq = (PKT_CLIGS_BEGIN_JIEBIAO_REQ*)pBody;

    if( sizeof(PKT_CLIGS_BEGIN_JIEBIAO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT64 qwBattleIndex = 0;
    BOOL bRepeat = FALSE;
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBeginJiebiaoReq( poPlayer, pReq->dwPlayerID, pReq->byCurCount, qwBattleIndex, bRepeat );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
	if ( (poUser) && (poUser->GetCliVer() >= VERSION_PET))
	{
		PKT_CLIGS_BEGIN_JIEBIAO_ACK5* pstAck = (PKT_CLIGS_BEGIN_JIEBIAO_ACK5*)(CGTPktBuilder::Instance()->GetPacketBuffer());

		//如果操作不成功，则跳过战报回报
		if( pstAck->wErrCode != ERR_JIEBIAO::ID_SUCCESS )
		{
			DBG_INFO( "jiebiao ret code:%d Jiebiao player:%u", pstAck->wErrCode, pstAck->dwJiebiaoPlayerID );
			return TRUE;
		}
		CPlayer* pBeJiebiaoPlayer = CPlayerMgr::Instance()->FindPlayer( pReq->dwPlayerID );
		if( NULL == pBeJiebiaoPlayer )
		{
			return FALSE;
		}

		if( bRepeat )
		{
			return TRUE;
		}

		CYabiaoInfo *poBeYabiaoInfo = pBeJiebiaoPlayer->GetYabiaoInfo();
		if (NULL == poBeYabiaoInfo)
		{
			return TRUE;
		}

		pstAck->stPromptInfo.byBuildNum = 0;
		wMsgID = CGTPktBuilder::Instance()->BeJiebiaoEventNtf( !pstAck->stBattleData.byBattleResult, qwBattleIndex, pstAck->qwCoinEncourage, pstAck->dwScienceEncourage,
			poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().dwPower, poBeYabiaoInfo->GetUnreadBattleLogNum() );
		if(INVALID_MSGID == wMsgID)
		{
			return FALSE;
		}
		pBeJiebiaoPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
		return TRUE;
	}
	else if ( (poUser) && (poUser->GetCliVer() >= VERSION_SOUL))
	{
		PKT_CLIGS_BEGIN_JIEBIAO_ACK4* pstAck = (PKT_CLIGS_BEGIN_JIEBIAO_ACK4*)(CGTPktBuilder::Instance()->GetPacketBuffer());

		//如果操作不成功，则跳过战报回报
		if( pstAck->wErrCode != ERR_JIEBIAO::ID_SUCCESS )
		{
			DBG_INFO( "jiebiao ret code:%d Jiebiao player:%u", pstAck->wErrCode, pstAck->dwJiebiaoPlayerID );
			return TRUE;
		}
		CPlayer* pBeJiebiaoPlayer = CPlayerMgr::Instance()->FindPlayer( pReq->dwPlayerID );
		if( NULL == pBeJiebiaoPlayer )
		{
			return FALSE;
		}

		if( bRepeat )
		{
			return TRUE;
		}

		CYabiaoInfo *poBeYabiaoInfo = pBeJiebiaoPlayer->GetYabiaoInfo();
		if (NULL == poBeYabiaoInfo)
		{
			return TRUE;
		}

		pstAck->stPromptInfo.byBuildNum = 0;
		wMsgID = CGTPktBuilder::Instance()->BeJiebiaoEventNtf( !pstAck->stBattleData.byBattleResult, qwBattleIndex, pstAck->qwCoinEncourage, pstAck->dwScienceEncourage,
			poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().dwPower, poBeYabiaoInfo->GetUnreadBattleLogNum() );
		if(INVALID_MSGID == wMsgID)
		{
			return FALSE;
		}
		pBeJiebiaoPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
		return TRUE;
	}
    else
    {
        PKT_CLIGS_BEGIN_JIEBIAO_ACK3* pstAck = (PKT_CLIGS_BEGIN_JIEBIAO_ACK3*)(CGTPktBuilder::Instance()->GetPacketBuffer());

        //如果操作不成功，则跳过战报回报
        if( pstAck->wErrCode != ERR_JIEBIAO::ID_SUCCESS )
        {
            DBG_INFO( "jiebiao ret code:%d Jiebiao player:%u", pstAck->wErrCode, pstAck->dwJiebiaoPlayerID );
            return TRUE;
        }
        CPlayer* pBeJiebiaoPlayer = CPlayerMgr::Instance()->FindPlayer( pReq->dwPlayerID );
        if( NULL == pBeJiebiaoPlayer )
        {
            return FALSE;
        }

        if( bRepeat )
        {
            return TRUE;
        }

        CYabiaoInfo *poBeYabiaoInfo = pBeJiebiaoPlayer->GetYabiaoInfo();
        if (NULL == poBeYabiaoInfo)
        {
            return TRUE;
        }

        pstAck->stPromptInfo.byBuildNum = 0;
        wMsgID = CGTPktBuilder::Instance()->BeJiebiaoEventNtf( !pstAck->stBattleData.byBattleResult, qwBattleIndex, pstAck->qwCoinEncourage, pstAck->dwScienceEncourage,
            poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().dwPower, poBeYabiaoInfo->GetUnreadBattleLogNum() );
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        pBeJiebiaoPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        return TRUE;
    }

    return TRUE;
}


BOOL CGTProcessor::OnQueryBattleRecordReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_QUERY_BATTLE_RECORD_REQ* pReq = (PKT_CLIGS_QUERY_BATTLE_RECORD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_QUERY_BATTLE_RECORD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }
    //if ((poUser) && (poUser->GetCliVer() <= 310))
    //{
    //    CPlayerBattleDataRecordMgr::Instance()->GetBattleData( poPlayer->GetID(), pReq->dwIndex );
    //}
    //else if ((poUser) && (poUser->GetCliVer() > 310))
    //{
    //统一使用新战队协议 将数据转换
    CPlayerBattleData2RecordMgr::Instance()->GetBattleData( poPlayer->GetID(), pReq->dwIndex );
    //}
    return TRUE;
}

BOOL CGTProcessor::OnQueryYabiaoBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ* pReq = (PKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ*)pBody;

    if( sizeof(PKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnQueryYabiaoBattleLogReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnQueryYabiaoEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ* pReq = (PKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT32 wMsgID = CGTPktBuilder::Instance()->OnQueryYabiaoEncourageReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnClearJiebiaoCDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLEAR_JIEBIAO_CD_REQ* pReq = (PKT_CLIGS_CLEAR_JIEBIAO_CD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_CLEAR_JIEBIAO_CD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClearJiebiaoCDReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


//境界升级
BOOL CGTProcessor::OnUpgradeJingjieReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_UPGRADE_JINGJIE_REQ* pReq = (PKT_CLIGS_UPGRADE_JINGJIE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_UPGRADE_JINGJIE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpgradeJingjieReq( poPlayer , pReq->wLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//打开练气面板
BOOL CGTProcessor::OnOpenGenGasTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GEN_GAS_TAB_REQ* pReq = (PKT_CLIGS_OPEN_GEN_GAS_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_GEN_GAS_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGenGasTabReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//生产精气球
BOOL CGTProcessor::OnGenGasBallReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GEN_GAS_BALL_REQ* pReq = (PKT_CLIGS_GEN_GAS_BALL_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GEN_GAS_BALL_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGenGasBallReq( poPlayer, pReq->byFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
//提升精气球等级
BOOL CGTProcessor::OnUpGasBallLevelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_UP_GAS_BALL_LEVEL_REQ* pReq = (PKT_CLIGS_UP_GAS_BALL_LEVEL_REQ*)pBody;

    if( sizeof(PKT_CLIGS_UP_GAS_BALL_LEVEL_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpGasBallLevelReq( poPlayer , pReq->byFullLevelFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
//收集精气等级
BOOL CGTProcessor::OnCollectGasReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COLLECT_GAS_REQ* pReq = (PKT_CLIGS_COLLECT_GAS_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COLLECT_GAS_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCollectGasReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
//升级技能招式等级
BOOL CGTProcessor::OnUpgradeSkillActionLevelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ* pReq = (PKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ*)pBody;

    if( sizeof(PKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpgradeSkillActionLevelReq( poPlayer , pReq->wActionID, pReq->wActionLevelCli);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
//选择技能招式效果
BOOL CGTProcessor::OnSelectSkillActionEffectReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ* pReq = (PKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSelectSkillActionEffectReq( poPlayer , pReq->wActionID, pReq->wEffectID, pReq->byAttackRangKind, pReq->byTraceBuffKind);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
//打开境界面板
BOOL CGTProcessor::OnOpenJingjieTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_JINGJIE_TAB_REQ* pReq = (PKT_CLIGS_OPEN_JINGJIE_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_JINGJIE_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenJingjieTabReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//打开技能面板
BOOL CGTProcessor::OnOpenSkillTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SKILL_TAB_REQ* pReq = (PKT_CLIGS_OPEN_SKILL_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_SKILL_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenSkillTabReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


//打开VIP面板
BOOL CGTProcessor::OnOpenVipTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_VIP_TAB_REQ* pReq = (PKT_CLIGS_OPEN_VIP_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_VIP_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenVipTabReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


//打开爬塔面板
BOOL CGTProcessor::OnOpenClimbTowerReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_CLIMB_TOWER_REQ* pReq = (PKT_CLIGS_OPEN_CLIMB_TOWER_REQ*)pBody;
    if ( sizeof(PKT_CLIGS_OPEN_CLIMB_TOWER_REQ) != dwLen )
        return FALSE;

    CUser* poUser = (CUser*)pUser;

    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenClimbTowerReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


//打开重置次数请求
//BOOL CGTProcessor::OnBuyClimbTowerResetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
//{
//    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
//    UINT32 dwTransID            = pPipeHeader->dwTransID;
//
//    PKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ* pReq = (PKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ*)pBody;
//
//    if( sizeof(PKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ) != dwLen )
//    {
//        return FALSE;
//    }
//
//    CUser* poUser = (CUser*)pUser;
//    CPlayer* poPlayer = poUser->GetPlayer();
//    if(!poPlayer)
//    {
//        return FALSE;
//    }
//
//    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBuyClimbTowerResetReq( poPlayer, pReq->byBuyNum);
//    if(INVALID_MSGID == wMsgID)
//    {
//        return FALSE;
//    }
//    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
//    return TRUE;
//}

BOOL CGTProcessor::OnOpenXiangqianTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ* pReq = (PKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenXiangqianTabReq( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//镶嵌宝石
BOOL CGTProcessor::OnXiangqianReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_XIANGQIAN_REQ* pReq = (PKT_CLIGS_XIANGQIAN_REQ*)pBody;

    if( sizeof(PKT_CLIGS_XIANGQIAN_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnXiangqianReq( poPlayer, pReq->wEquipID, pReq->wIdx, pReq->wItemID, pReq->byPos, pReq->wHeroID );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnAppExchangeCheckReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ* pReq = (PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ*)pBody;

    if( sizeof(PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //添加到处理队列
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAppPayCheckReq( poPlayer, pReq->szTransactionID, pReq->szReceiptData, dwTransID, pReq->dwPlayerID );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnRemoveXiangqianReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_REMOVE_XIANGQIAN_REQ* pReq = (PKT_CLIGS_REMOVE_XIANGQIAN_REQ*)pBody;

    if( sizeof(PKT_CLIGS_REMOVE_XIANGQIAN_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = 0;
    if( !pReq->byRemoveAll )
    {
        wMsgID = CGTPktBuilder::Instance()->OnRemoveXiangqianReq( poPlayer, pReq->wHeroID, pReq->wEquipID, pReq->wIdx, pReq->byPos, pReq->byRemoveAll );
    }
    else
    {
// 		INT32 nPos=0;
// 		CGTPktBuilder::Instance()->ZeroPacketBuffer();
// 		while( nPos<MAX_XIANGQIAN_NUM )
// 		{
        wMsgID = CGTPktBuilder::Instance()->OnRemoveXiangqianReq( poPlayer, pReq->wHeroID, pReq->wEquipID, pReq->wIdx, pReq->byPos, pReq->byRemoveAll );
        /*		}*/

    }
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnGetEliteInstanceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_ELITE_INSTANCE_REQ* pReq = (PKT_CLIGS_GET_ELITE_INSTANCE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_ELITE_INSTANCE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetEliteInstance( poPlayer );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnResetEliteInstanceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RESET_ELITE_INSTANCE_REQ* pReq = (PKT_CLIGS_RESET_ELITE_INSTANCE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RESET_ELITE_INSTANCE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnResetEliteInstance( poPlayer, pReq->byIndex, pReq->wRemainResetNum );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnEliteInstanceOnHookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ* pReq = (PKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ*)pBody;

    if( sizeof(PKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }


    UINT16 wMsgID = CGTPktBuilder::Instance()->OnEliteInstanceOnHook( poPlayer, pReq->byIndex );
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}



BOOL CGTProcessor::OnGetActivityPromptReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_ACTIVITY_PROMPT_REQ* pReq = (PKT_CLIGS_GET_ACTIVITY_PROMPT_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_ACTIVITY_PROMPT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }


    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetActivityPromptReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnBlueToPurpleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BLUE_TO_PURPLE_REQ* pReq = (PKT_CLIGS_BLUE_TO_PURPLE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_BLUE_TO_PURPLE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }


    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBlueToPurpleReq( poPlayer, pReq->dwBlueGas);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnPKBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_PKBATTLE_REQ* pReq = (PKT_CLIGS_PKBATTLE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_PKBATTLE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }
    CPlayer* poEnemy = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
    if(NULL == poEnemy)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_PK, poPlayer->GetID());
    }
    else
    {
        UINT16 wMsgID = CGTPktBuilder::Instance()->OnPKBattleReq( poPlayer, poEnemy);
        if(INVALID_MSGID == wMsgID)
        {
            return FALSE;
        }
        poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    }

    return TRUE;
}

//资源不足请求
BOOL CGTProcessor::OnResourcesPromptReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RESOURCES_PROMPT_REQ* pReq = (PKT_CLIGS_RESOURCES_PROMPT_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RESOURCES_PROMPT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }


    UINT16 wMsgID = CGTPktBuilder::Instance()->OnResourcesPromptReq( poPlayer, pReq->dwRsID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}




BOOL CGTProcessor::OnEnterBossbReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_ENTER_BOSSB_REQ* pReq = (PKT_ENTER_BOSSB_REQ*)pBody;

    if( sizeof(PKT_ENTER_BOSSB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnEnterBossbReq( poPlayer, pReq->wPosX, pReq->wPosY);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}



BOOL CGTProcessor::OnLeaveBossbReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_LEAVE_BOSSB_REQ* pReq = (PKT_LEAVE_BOSSB_REQ*)pBody;

    if( sizeof(PKT_LEAVE_BOSSB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnLeaveBossbReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}



BOOL CGTProcessor::OnBossbMoveReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_BOSSB_MOVE_REQ* pReq = (PKT_BOSSB_MOVE_REQ*)pBody;

    if( sizeof(PKT_BOSSB_MOVE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBossbMoveReq( poPlayer, pReq->wPosX, pReq->wPosY);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}



BOOL CGTProcessor::OnBossbHurtReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_BOSSB_HURT_REQ* pReq = (PKT_BOSSB_HURT_REQ*)pBody;

    if( sizeof(PKT_BOSSB_HURT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBossbHurtReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}



BOOL CGTProcessor::OnOpenInspireTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_OPEN_INSPIRE_TAB_REQ* pReq = (PKT_OPEN_INSPIRE_TAB_REQ*)pBody;

    if( sizeof(PKT_OPEN_INSPIRE_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenInspireTabReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}



BOOL CGTProcessor::OnInspirePowerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_INSPIRE_POWER_REQ* pReq = (PKT_INSPIRE_POWER_REQ*)pBody;

    if( sizeof(PKT_INSPIRE_POWER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnInspirePowerReq( poPlayer, pReq->byGoldInspireFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnBossBReliveReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_BOSSB_RELIVE_REQ* pReq = (PKT_BOSSB_RELIVE_REQ*)pBody;

    if( sizeof(PKT_BOSSB_RELIVE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBossBReliveReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnGetLastBossBInfoReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_BOSSB_GET_LAST_BOSSB_INFO_REQ* pReq = (PKT_BOSSB_GET_LAST_BOSSB_INFO_REQ*)pBody;

    if( sizeof(PKT_BOSSB_GET_LAST_BOSSB_INFO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetLastBossBInfoReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnOpenBossBBLReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ* pReq = (PKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenBossBBLReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnOpenRechargeEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ* pReq = (PKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenRechargeEncourageReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}

BOOL CGTProcessor::OnRecvRechargeEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ* pReq = (PKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvRechargeEncourageReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}

BOOL CGTProcessor::OnOpenScoreEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ* pReq = (PKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenScoreEncourageReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}

BOOL CGTProcessor::OnRecvScoreEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ* pReq = (PKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvScoreEncourageReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnGetRandomDspNameReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_RANDOM_DSPNAME_REQ* pReq = (PKT_CLIGS_GET_RANDOM_DSPNAME_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_RANDOM_DSPNAME_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetRandomDspNameReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}

BOOL	CGTProcessor::OnOpenExperienceTransferReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ* pReq = (PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenExperienceTransferReq( poPlayer, pReq->wHeroID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL	CGTProcessor::OnExperienceTransferReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EXPERIENCE_TRANSFER_REQ* pReq = (PKT_CLIGS_EXPERIENCE_TRANSFER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_EXPERIENCE_TRANSFER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnExperienceTransferReq( poPlayer, pReq->wHeroID, pReq->wTransHeroID, pReq->byGold);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnOpenRdchallengeTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ* pReq = (PKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenRdchallengeTabReq(poPlayer, pReq->byRefreshFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnRdchallengeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RDCHALLENGE_REQ* pReq = (PKT_CLIGS_RDCHALLENGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RDCHALLENGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }
    CPlayer* poEnemy = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
    if(NULL == poEnemy)
    {
        //取对方数据
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_RANDOM_CHALLENGE, pReq->dwPlayerID);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRdchallengeReq(poPlayer, poEnemy);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}



BOOL CGTProcessor::OnClrRDChallengeCDReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLR_RDCHALLENGE_CD_REQ* pReq = (PKT_CLIGS_CLR_RDCHALLENGE_CD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_CLR_RDCHALLENGE_CD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClrRDChallengeCDReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnOpenWorshipTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_WORSHIP_TAB_REQ* pReq = (PKT_CLIGS_OPEN_WORSHIP_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_WORSHIP_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenWorshipTabReq(poPlayer, pReq->byWorshipType);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnWorshipReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_WORSHIP_REQ* pReq = (PKT_CLIGS_WORSHIP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_WORSHIP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnWorshipReq(poPlayer, pReq->byWorshipType, pReq->byWorshipTypeLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnActivityNotifyReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ACTIVITY_NOTIFY_REQ* pReq = (PKT_CLIGS_ACTIVITY_NOTIFY_REQ*)pBody;

    if( sizeof(PKT_CLIGS_ACTIVITY_NOTIFY_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnActivityNotifyReq(poPlayer, pReq->byActivityType, pReq->byNotify);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenDogFecesReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_DOGFECES_REQ* pReq = (PKT_CLIGS_OPEN_DOGFECES_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_DOGFECES_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenDogFecesReq(poPlayer, pReq->byType);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnGetActivityCenterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_ACTIVITY_CENTER_REQ* pReq = (PKT_CLIGS_GET_ACTIVITY_CENTER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_ACTIVITY_CENTER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetActivityCenterReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnGetActivityEncourageInfoReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ* pReq = (PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetActivityEncourageInfoReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnGetActivityEncourageInfoReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2* pReq = (PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2*)pBody;

	if( sizeof(PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetActivityEncourageInfoAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
	return TRUE;
}

BOOL CGTProcessor::OnRecvActivityEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ* pReq = (PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvActivityEncourageAck(poPlayer, pReq->wActivityID, pReq->aszSeriesNo);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnRecvActivityEncourageReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2* pReq = (PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2*)pBody;

	if( sizeof(PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvActivityEncourageAck(poPlayer, pReq->qwActivityID, pReq->aszSeriesNo);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
	return TRUE;
}



BOOL CGTProcessor::OnOpenGemMakerTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ* pReq = (PKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGemMakerTabReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenGemMakerTabExReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ* pReq = (PKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGemMakerTabExReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}



BOOL CGTProcessor::OnRecvRatingEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_RATING_ENCOURAGE_REQ* pReq = (PKT_CLIGS_RECV_RATING_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RECV_RATING_ENCOURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvRatingEncourageReq(poPlayer, pReq->wSceneIdx, pReq->byType, pReq->wChapterID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}



BOOL CGTProcessor::OnOpenAwakenExchangeTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ* pReq = (PKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenAwakenExchangeTabReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnExchangeStudyReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EXCHANGE_STUDY_REQ* pReq = (PKT_CLIGS_EXCHANGE_STUDY_REQ*)pBody;

    if( sizeof(PKT_CLIGS_EXCHANGE_STUDY_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnExchangeStudyReq(poPlayer, pReq->byStudyColorKind, pReq->byStudyAttrKind1, pReq->byStudyAttrKind2, pReq->byStudyLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnOpenEquipComposeTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ* pReq = (PKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenEquipComposeTabReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnEquipComposeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_EQUIP_COMPOSE_REQ* pReq = (PKT_CLIGS_EQUIP_COMPOSE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_EQUIP_COMPOSE_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnEquipComposeReq(poPlayer, pReq->wEquipLevel, pReq->byCurColorFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnRecvComposeEquipReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_COMPOSE_EQUIP_REQ* pReq = (PKT_CLIGS_RECV_COMPOSE_EQUIP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RECV_COMPOSE_EQUIP_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvComposeEquipReq(poPlayer, pReq->wEquipLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


// 打开帮派面板
BOOL		  CGTProcessor::OnOpenFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_FACTION_REQ* pReq = (PKT_CLIGS_OPEN_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
// 申请加入帮派
BOOL		  CGTProcessor::OnApplyFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_APPLY_FACTION_REQ* pReq = (PKT_CLIGS_APPLY_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_APPLY_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnApplyFactionReq(poPlayer, pReq->dwFactionID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    PKT_CLIGS_APPLY_FACTION_ACK* pstAck = (PKT_CLIGS_APPLY_FACTION_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if (ERR_APPLY_FACTION::ID_SUCCESS == pstAck->wErrCode )
    {
        CFaction* poFaction = CFactionMgr::Instance()->GetFaction(pReq->dwFactionID);
        if (NULL != poFaction)
        {
            poFaction->SendMessageByAuthPrompt( );
        }

    }
    return TRUE;
}
// 波茨坦加入申请帮派
BOOL		  CGTProcessor::OnCancelApplyFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CANCEL_APPLY_FACTION_REQ* pReq = (PKT_CLIGS_CANCEL_APPLY_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_CANCEL_APPLY_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCancelApplyFactionReq(poPlayer, pReq->dwFactionID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    PKT_CLIGS_CANCEL_APPLY_FACTION_ACK* pstAck = (PKT_CLIGS_CANCEL_APPLY_FACTION_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if (ERR_CANCEL_APPLY_FACTION::ID_SUCCESS == pstAck->wErrCode )
    {
        CFaction* poFaction = CFactionMgr::Instance()->GetFaction(pReq->dwFactionID);
        if (NULL != poFaction)
        {
            poFaction->SendMessageByAuthPrompt();
        }
    }

    return TRUE;
}


// 创建帮派
BOOL		  CGTProcessor::OnCreateFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CREATE_FACTION_REQ* pReq = (PKT_CLIGS_CREATE_FACTION_REQ*)pBody;

    if( sizeof( PKT_CLIGS_CREATE_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        PKT_CLIGS_CREATE_FACTION_ACK stAck;
        stAck.wErrCode = ERR_CREATE_FACTION::ID_UNOPEN;
        poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
        return TRUE;
    }
    DT_LEAVE_FACTION_MARK& stMarkData = poFactionPlayer->GetDT_PLAYER_MARK_DATA().stLeaveFactionMark;
    if ( ESM_YES == stMarkData.byLeaveMark )
    {
        UINT32 dwIntervalTime = CLuamgr::Instance()->GetJoinFactionInterval();
        UINT64 dwCurTime = SGDP::SDTimeSecs();
        if ( dwCurTime < stMarkData.qwLeaveTime + dwIntervalTime)
        {
            PKT_CLIGS_CREATE_FACTION_ACK stAck;
            stAck.qwLeftTime = static_cast<UINT32>(stMarkData.qwLeaveTime + dwIntervalTime - dwCurTime);

            stAck.wErrCode = ERR_CREATE_FACTION::ID_TIME_NOT_ENOUGH;
            poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
            return TRUE;
        }
        stMarkData.byLeaveMark = ESM_NO;
        stMarkData.qwLeaveTime = 0;
    }
    UINT32 dwFactionNameLen = GetUTF8CharWidth(pReq->aszFactionName);
    if(MAX_FACTION_NAME_LEN < dwFactionNameLen)
    {
        PKT_CLIGS_CREATE_FACTION_ACK stAck;
        stAck.wErrCode = ERR_CREATE_FACTION::ID_NAME_TOO_LONG;
        poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
        return TRUE;
    }


    //检查参数
    if( !CWordFilter::Instance()->VerifyDspNameString(pReq->aszFactionName)
            || !CWordFilter::Instance()->VerifyDspNameString(pReq->aszFactionDescName))
    {
        PKT_CLIGS_CREATE_FACTION_ACK stAck;
        stAck.wErrCode = ERR_CREATE_FACTION::ID_STRING_NOT_ENOUGH;
        poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
        return TRUE;
    }
    //含有emoji表情字符
    if ( !ChkUTF8Char(pReq->aszFactionName) || !ChkUTF8Char( pReq->aszFactionDescName ))
    {
        PKT_CLIGS_CREATE_FACTION_ACK stAck;
        stAck.wErrCode = ERR_CREATE_FACTION::ID_EMOJI_NOT_USE;
        poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
        return TRUE;
    }

    //查找玩家是否在帮派中
    UINT32 dwFactionID = 0;
    if(CFaction::CkFactionPlayerData(dwFactionID, poPlayer->GetID()))
    {
        PKT_CLIGS_CREATE_FACTION_ACK stAck;
        stAck.wErrCode = ERR_CREATE_FACTION::ID_PLAYER_HAVE_FACTION;
        poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
        return TRUE;
    }
    if ( CFactionMgr::Instance()->IsExitFactionName(pReq->aszFactionName) )
    {
        PKT_CLIGS_CREATE_FACTION_ACK stAck;
        stAck.wErrCode = ERR_CREATE_FACTION::ID_FACTION_NAME_SAME;
        poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
        return TRUE;
    }
    //铜钱
    if (0 == pReq->wCreateFactionType)
    {
        UINT32 dwCreateFactionCoin = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_FACTION_CREATE_BY_COIN, 0);
        if (poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < dwCreateFactionCoin)
        {
            PKT_CLIGS_CREATE_FACTION_ACK stAck;
            stAck.wErrCode = ERR_CREATE_FACTION::ID_COIN_NOT_ENOUGH;
            poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
            return TRUE;
        }
    }
    else
    {
        UINT32 dwCreateFactionGold = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_FACTION_CREATE_BY_GOLD, 0);
        if (poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < dwCreateFactionGold)
        {
            PKT_CLIGS_CREATE_FACTION_ACK stAck;
            stAck.wErrCode = ERR_CREATE_FACTION::ID_GOLD_NOT_ENOUGH;
            poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_CREATE_FACTION_ACK);
            return TRUE;
        }
    }
    CkEmoji(pReq->aszFactionDescName);
    CkEmoji(pReq->aszFactionName);
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_CREATE_FACTION);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pReq, poPlayer->GetID());
    return gsapi::GetDBMgr()->AddCommand(poCMD);

    return TRUE;
}
// 查找帮派
BOOL		  CGTProcessor::OnSearchFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SEARCH_FACTION_REQ* pReq = (PKT_CLIGS_SEARCH_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SEARCH_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSearchFactionReq(poPlayer, pReq->aszFactionName);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
// 打开属于自己帮派
BOOL		  CGTProcessor::OnOpenOwnerFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_OWNER_FACTION_REQ* pReq = (PKT_CLIGS_OPEN_OWNER_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_OWNER_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenOwnerFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
// 打开帮派门供
BOOL		  CGTProcessor::OnOpenDoorsTributeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ* pReq = (PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenDoorsTributeFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
// 门供帮派
BOOL		  CGTProcessor::OnDoorsTributeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ* pReq = (PKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDoorsTributeFactionReq(poPlayer, pReq->dwDoorsTributeFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


// 获取门供帮派奖励
BOOL		  CGTProcessor::OnRecvDoorsTributeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ* pReq = (PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvDoorsTributeFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK* poAck = (PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if ( poAck->wErrCode == ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_SUCCESS)
    {
        CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
        if (NULL != poFactionPlayer)
        {
            PKT_CLIGS_HOME_BUILD_PROMPT_NTF stNtf;
            memset(&stNtf, 0, sizeof(PKT_CLIGS_HOME_BUILD_PROMPT_NTF));
            poPlayer->GetFactionPrompt(stNtf.stPromptInfo.astBuildPromptInfo[stNtf.stPromptInfo.byBuildNum++]);
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_HOME_BUILD_PROMPT_NTF);

            PKT_CLIGS_FACTION_PROMPT_NTF stFactionNtf;
            memset(&stFactionNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
            poFactionPlayer->GetFactionPrompt(stFactionNtf.stFactionPrompt);
            poPlayer->SendMsg((TCHAR*)&stFactionNtf, CLIGS_FACTION_PROMPT_NTF);

        }
    }
    return TRUE;
}
// 帮派踢出
BOOL		  CGTProcessor::OnKickedFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_KICKED_FACTION_REQ* pReq = (PKT_CLIGS_KICKED_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_KICKED_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    CPlayer* poKickedPlayer = CPlayerMgr::Instance()->FindPlayer( pReq->dwPlayerID);
    if (NULL == poKickedPlayer)
    {
        if (0 == pReq->dwPlayerID)
        {
            PKT_CLIGS_KICKED_FACTION_ACK stAck;
            stAck.wErrCode = ERR_KICKED_FACTION::ID_PLAYRE_ID_NOT_REAL;
            poPlayer->SendMsg((TCHAR*)&stAck, CLIGS_KICKED_FACTION_ACK);
            return TRUE;
        }
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, KICKED_PLAYER_OUT_FACTION, pReq->dwPlayerID);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnKickedFactionReq(poPlayer, pReq->dwPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    PKT_CLIGS_KICKED_FACTION_ACK* pAck = (PKT_CLIGS_KICKED_FACTION_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if (pAck->wErrCode == 0)
    {
        CPlayer* poKickedPlayer = CPlayerMgr::Instance()->FindPlayer( pReq->dwPlayerID);
        if (NULL != poKickedPlayer)
        {
            CFactionPlayer* poFactionPlayer = poKickedPlayer->GetFactionPlayer();
            if (NULL == poFactionPlayer)
            {
                return TRUE;
            }
            DT_PLAYER_FACTION_BATTLELOG_DATA stLogData = {0};
            const SPlayerBaseData* poPlayBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poPlayer->GetID());
            if ( NULL != poPlayBaseData)
            {
                SDStrncpy(stLogData.aszDescMsg, poPlayBaseData->aszDispName, NORMAL_MSG_LEN - 1);
            }
            else
            {
                stLogData.aszDescMsg[0] = 0;
            }
            stLogData.qwLogTime = SGDP::SDTimeSecs();
            stLogData.byFactionLogType = EFBT_Kicked;
            stLogData.byNewFlag = ESM_YES;
            poFactionPlayer->AddFactionBattleLog(stLogData);

            PKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF stNtf;
            stNtf.byUnReadLogNum = poFactionPlayer->GetUnReadFactionBattleLogNum();
            stNtf.byLastOperateType	= EFBT_Kicked;
            poKickedPlayer->SendMsg( (TCHAR*)&stNtf, CLIGS_PLAYER_FACTION_BATTLELOG_NTF);


            CFactionPlayer* poHeadPlayer = poPlayer->GetFactionPlayer();
            if (NULL == poHeadPlayer)
            {
                PKT_CLIGS_FACTION_PROMPT_NTF stFactionNtf;
                poHeadPlayer->GetFactionPrompt(stFactionNtf.stFactionPrompt);
                poPlayer->SendMsg((TCHAR*)&stFactionNtf, CLIGS_FACTION_PROMPT_NTF);
            }
        }

    }
    return TRUE;
}
// 设置帮派职位
BOOL		  CGTProcessor::OnJobFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_JOB_FACTION_REQ* pReq = (PKT_CLIGS_JOB_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_JOB_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnJobFactionReq(poPlayer, pReq->dwPlayerID, pReq->byFlag);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
// 设置帮派基本信息
BOOL		  CGTProcessor::OnSetNoticeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SET_NOTICE_FACTION_REQ* pReq = (PKT_CLIGS_SET_NOTICE_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SET_NOTICE_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSetNoticeFactionReq(poPlayer, pReq->byFlag, pReq->aszFactionNotice);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
// 帮派解散
BOOL		  CGTProcessor::OnDissolveFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DISSOLVE_FACTION_REQ* pReq = (PKT_CLIGS_DISSOLVE_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_DISSOLVE_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDissolveFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


// 帮派审核
BOOL		  CGTProcessor::OnAuditFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_AUDIT_FACTION_REQ* pReq = (PKT_CLIGS_AUDIT_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_AUDIT_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAuditFactionReq(poPlayer, pReq->dwFactionNum, pReq->astAuditPlayerData);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    PKT_CLIGS_AUDIT_FACTION_ACK* poAck = (PKT_CLIGS_AUDIT_FACTION_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if ( poAck->wErrCode == ERR_AUDIT_FACTION::ID_SUCCESS)
    {
        if(poAck->dwPassPlayerNum + poAck->dwNoPassPlayerNum == pReq->dwFactionNum)
        {
            CFaction* poFaction = poPlayer->GetFaction();
            if (NULL != poFaction)
            {
                poFaction->SendMessageByAuthPrompt();
            }
        }
    }
    return TRUE;
}

BOOL CGTProcessor::OnAuditFactionReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_AUDIT_FACTION_REQ2* pReq = (PKT_CLIGS_AUDIT_FACTION_REQ2*)pBody;

    if( sizeof(PKT_CLIGS_AUDIT_FACTION_REQ2 ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAuditFactionReq2(poPlayer, pReq->dwAuditPlayerID, pReq->byAuditWay);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 打开帮派申请列表
BOOL CGTProcessor::OnOpenApplyFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_APPLY_FACTION_REQ* pReq = (PKT_CLIGS_OPEN_APPLY_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_APPLY_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenApplyFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 打开帮派排名列表
BOOL		  CGTProcessor::OnOpenRankFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    tagPKT_CLIGS_OPEN_RANK_FACTION_REQ* pReq = (tagPKT_CLIGS_OPEN_RANK_FACTION_REQ*)pBody;

    if( sizeof(tagPKT_CLIGS_OPEN_RANK_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenRankFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


// 打开帮派审核列表
BOOL CGTProcessor::OnOpenAuditFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_AUDIT_FACTION_REQ* pReq = (PKT_CLIGS_OPEN_AUDIT_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_AUDIT_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenAuditFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenAuditFactionReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_AUDIT_FACTION_REQ2* pReq = (PKT_CLIGS_OPEN_AUDIT_FACTION_REQ2*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_AUDIT_FACTION_REQ2 ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenAuditFactionAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 打开帮派日志列表
BOOL		  CGTProcessor::OnOpenLogFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_LOG_FACTION_REQ* pReq = (PKT_CLIGS_OPEN_LOG_FACTION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_LOG_FACTION_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenLogFactionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


// 打开聚魂面板
BOOL		  CGTProcessor::OnOpenGetherScienceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GATHER_SCIENCE_REQ* pReq = (PKT_CLIGS_OPEN_GATHER_SCIENCE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_GATHER_SCIENCE_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGetherScienceReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 聚魂
BOOL		  CGTProcessor::OnGetherScienceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GATHER_SCIENCE_REQ* pReq = (PKT_CLIGS_GATHER_SCIENCE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GATHER_SCIENCE_REQ ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetherScienceReq(poPlayer, pReq->byGatherType);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 打开仙露阁
BOOL CGTProcessor::OnOpenBluePavilionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_BLUE_PAVILION_REQ* pReq = (PKT_CLIGS_OPEN_BLUE_PAVILION_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_BLUE_PAVILION_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenBluePavilionReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 求仙露
BOOL CGTProcessor::OnBegBlueReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BEGBLUE_REQ* pReq = (PKT_CLIGS_BEGBLUE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_BEGBLUE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBegBlueReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 求仙露改运
BOOL CGTProcessor::OnImproveLuckReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_IMPROVELUCK_REQ* pReq = (PKT_CLIGS_IMPROVELUCK_REQ*)pBody;

    if( sizeof(PKT_CLIGS_IMPROVELUCK_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnImproveLuckReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 求仙露确认祈求结果
BOOL CGTProcessor::OnComfirmBegBlueReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COMFIRM_BEGBLUE_REQ* pReq = (PKT_CLIGS_COMFIRM_BEGBLUE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COMFIRM_BEGBLUE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnComfirmBegBlueReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


BOOL CGTProcessor::OnChangeCareerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CHANGE_CAREER_REQ* pReq = (PKT_CLIGS_CHANGE_CAREER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_CHANGE_CAREER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnChangeCareerReq(poPlayer, pReq->byCareerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnChallengeMyselfReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CHALLENGE_MYSELF_REQ* pReq = (PKT_CLIGS_CHALLENGE_MYSELF_REQ*)pBody;

    if( sizeof(PKT_CLIGS_CHALLENGE_MYSELF_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnChallengeMyselfReq(poPlayer, pReq->byCareerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnOpenItemExchangeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ* pReq = (PKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenItemExchangeReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnItemExchangeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ITEM_EXCHANGE_REQ* pReq = (PKT_CLIGS_ITEM_EXCHANGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_ITEM_EXCHANGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnItemExchangeReq(poPlayer, pReq->wExchangeIdx);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnOperatingActivityBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ* pReq = (PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOperatingActivityBattleReq(poPlayer, pReq->aszBattleKey);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


// 获取好友系统玩家列表
BOOL CGTProcessor::OnGetFriendSysPlayerListReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ* pReq = (PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetFriendSysPlayerListAck(poPlayer, pReq->byGetListType, pReq->byPageCapacity, pReq->byPageNo);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 获取好友系统玩家列表
BOOL CGTProcessor::OnGetFriendSysPlayerListReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2* pReq = (PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2*)pBody;

    if( sizeof(PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;

    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetFriendSysPlayerListAck2(poPlayer, pReq->byGetListType, pReq->byPageCapacity, pReq->byPageNo);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 删除好友
BOOL CGTProcessor::OnDelFriendReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DEL_FRIEND_REQ* pReq = (PKT_CLIGS_DEL_FRIEND_REQ*)pBody;

    if( sizeof(PKT_CLIGS_DEL_FRIEND_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDelFriendAck(poPlayer, pReq->dwPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 搜索玩家
BOOL CGTProcessor::OnSearchPlayerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SEARCH_PLAYER_REQ* pReq = (PKT_CLIGS_SEARCH_PLAYER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SEARCH_PLAYER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSearchPlayerAck(poPlayer, pReq->aszPlayerName, pReq->byPageCapacity, pReq->byPageNo);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


// 搜索玩家
BOOL CGTProcessor::OnSearchPlayerReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SEARCH_PLAYER_REQ2* pReq = (PKT_CLIGS_SEARCH_PLAYER_REQ2*)pBody;

    if( sizeof(PKT_CLIGS_SEARCH_PLAYER_REQ2) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSearchPlayerAck2(poPlayer, pReq->aszPlayerName, pReq->byPageCapacity, pReq->byPageNo);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 查看玩家信息
BOOL CGTProcessor::OnViewPlayerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_VIEW_PLAYER_REQ* pReq = (PKT_CLIGS_VIEW_PLAYER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_VIEW_PLAYER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }
	CPlayer* poTarget = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	if ( NULL == poTarget)
	{
		//内存中查找不到，需要到数据库中去查询
		CGetPlayerDataMgr::Instance()->GetPlayerData( poPlayer->GetID(), GET_PLAYER_INFO_PANDECT,
			pReq->dwPlayerID);
		return TRUE;
	}

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnViewPlayerAck(poPlayer, pReq->dwPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 添加好友
BOOL CGTProcessor::OnAddFriendReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ADD_FRIEND_REQ* pReq = (PKT_CLIGS_ADD_FRIEND_REQ*)pBody;

    if( sizeof(PKT_CLIGS_ADD_FRIEND_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

	if ( 0 == pReq->dwPlayerID)
	{
		PKT_CLIGS_ADD_FRIEND_ACK stAck;
		memset(&stAck, 0, sizeof(PKT_CLIGS_ADD_FRIEND_ACK));
		stAck.wErrCode = ERR_ADD_FRIEND::ID_REF_PLAYER_INFO_LOST;
		poPlayer->SendMsg( (TCHAR*)&stAck, CLIGS_ADD_FRIEND_ACK);
		return TRUE;
	}
    //真实需要获得信息的玩家
    CPlayer *poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
    if( NULL == poTargetPlayer )
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, GET_PLAYERDATATYPE_ADD_FRIENDS, pReq->dwPlayerID);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAddFriendAck(poPlayer, poTargetPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 打开送花面板
BOOL CGTProcessor::OnOpenSendFlowerTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ* pReq = (PKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenSendFlowerTabAck(poPlayer, pReq->dwRecvPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 送花
BOOL CGTProcessor::OnSendFlowerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SEND_FLOWER_REQ* pReq = (PKT_CLIGS_SEND_FLOWER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SEND_FLOWER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //真实需要获得信息的玩家
    CPlayer *poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwRecvPlayerID);
    if( NULL == poTargetPlayer )
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, GET_PLAYERDATATYPE_SENDFLOWER, pReq->dwRecvPlayerID, pReq->dwSendFlowerNum);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSendFlowerAck(poPlayer, poTargetPlayer, pReq->dwSendFlowerNum);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 获取鲜花信息
BOOL CGTProcessor::OnGetFlowerByPlayerIDReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ* pReq = (PKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //真实需要获得信息的玩家
    CPlayer *poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
    if( NULL == poTargetPlayer )
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, GET_PLAYERDATATYPE_RECVFLOWERLOG, pReq->dwPlayerID);
        return TRUE;
    }

    //获得信息
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetFlowerByPlayerIDAck(poTargetPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    return poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 获取鲜花榜
BOOL CGTProcessor::OnGetFlowerRankingReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_FLOWER_RANKING_REQ* pReq = (PKT_CLIGS_GET_FLOWER_RANKING_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_FLOWER_RANKING_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetFlowerRankingAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 获取私聊记录
BOOL CGTProcessor::OnGetPrivateMsgLogListReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ* pReq = (PKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //检测玩家的私聊信息是否在内存中
    if(!CPrivateMsgMgr::Instance()->CkPrivateMsg(poPlayer->GetID(), pReq->dwPlayerID))
    {
        CDBPktBuilder::Instance()->GetPrivateMsg(poPlayer->GetID(), pReq->dwPlayerID, em_Log, "", pReq->dwStartIdx, pReq->byGetMsgNum);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetPrivateMsgLogListAck(poPlayer, pReq->dwPlayerID, pReq->dwStartIdx, pReq->byGetMsgNum);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 发送私聊消息
BOOL CGTProcessor::OnSendPrivateMsgReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SEND_PRIVATE_MSG_REQ* pReq = (PKT_CLIGS_SEND_PRIVATE_MSG_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SEND_PRIVATE_MSG_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //真实需要获得信息的玩家
    CPlayer *poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwRecvPlayerID);
    if( NULL == poTargetPlayer )
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, GET_PLAYERDATATYPE_SEND_PRIVATE_MSG, pReq->dwRecvPlayerID, 0, 0, 0, pReq->aszMsgContent);
        return TRUE;
    }

    //检测玩家的私聊信息是否在内存中
    if(!CPrivateMsgMgr::Instance()->CkPrivateMsg(poPlayer->GetID(), poTargetPlayer->GetID()))
    {
        CDBPktBuilder::Instance()->GetPrivateMsg(poPlayer->GetID(), poTargetPlayer->GetID(), em_Msg, pReq->aszMsgContent);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSendPrivateMsgAck(poPlayer, poTargetPlayer, pReq->aszMsgContent);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 删除私聊消息
BOOL CGTProcessor::OnDelPrivateMsgReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ* pReq = (PKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ*)pBody;

    if( sizeof(PKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDelPrivateMsgAck(poPlayer, pReq->dwPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 拉入黑名单
BOOL CGTProcessor::OnAddToBlacklistReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ADD_TO_BLACKLIST_REQ* pReq = (PKT_CLIGS_ADD_TO_BLACKLIST_REQ*)pBody;

    if( sizeof(PKT_CLIGS_ADD_TO_BLACKLIST_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }


    //真实需要获得信息的玩家
    CPlayer *poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
    if( NULL == poTargetPlayer )
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, GET_PLAYERDATATYPE_ADD_TO_BLACK, pReq->dwPlayerID);
        return TRUE;
    }


    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAddToBlackListAck(poPlayer, poTargetPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 从黑名单中移除
BOOL CGTProcessor::OnMoveFromBlacklistReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_MOVE_FROM_BLACKLIST_REQ* pReq = (PKT_CLIGS_MOVE_FROM_BLACKLIST_REQ*)pBody;

    if( sizeof(PKT_CLIGS_MOVE_FROM_BLACKLIST_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnMoveFromBlackListAck(poPlayer, pReq->dwPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 获取发离线消息的玩家列表
BOOL CGTProcessor::OnGetSendOfflineMsgPlayerListReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ* pReq = (PKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetSendOfflineMsgPlayerListAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 读取离线消息请求
BOOL CGTProcessor::OnReadOfflineMsgReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_READ_OFFLINE_MSG_REQ* pReq = (PKT_CLIGS_READ_OFFLINE_MSG_REQ*)pBody;

    if( sizeof(PKT_CLIGS_READ_OFFLINE_MSG_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnReadOfflineMsgAck(poPlayer, pReq->dwSendPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 装备精通：打开装备精通面板
BOOL CGTProcessor::OnOpenEquipMasterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    CHECK_ON_REQ(PKT_CLIGS_OPEN_EQUIP_MASTER_REQ);

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenEquipMasterAck(*poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 装备精通：激活装备精通
BOOL CGTProcessor::OnActivateEquipMasterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    CHECK_ON_REQ(PKT_CLIGS_ACTIVATE_MASTER_REQ);

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnActivateEquipMasterAck(*poPlayer, pReq->byEquipKind);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 装备精通：突破
BOOL CGTProcessor::OnBreakMasterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    CHECK_ON_REQ(PKT_CLIGS_BREAK_MASTER_REQ);

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBreakMasterAck(*poPlayer, pReq->byEquipKind);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 装备精通：培养精通之书
BOOL CGTProcessor::OnGetMasterBookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    CHECK_ON_REQ(PKT_CLIGS_GET_MASTER_BOOK_REQ);

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetMasterBookAck(*poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 装备精通：刮开精通经验书面板
BOOL CGTProcessor::OnWipeMasterBookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    CHECK_ON_REQ(PKT_CLIGS_WIPE_MASTER_BOOK_REQ);

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnWipeMasterBookAck(*poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 装备精通：出千
BOOL CGTProcessor::OnCheatMasterBookReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_MASTER_CHEAT_REQ);

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnMasterBookCheatAck(*poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 装备精通：确认使用精通经验书
BOOL CGTProcessor::OnConfirmMasterBookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    CHECK_ON_REQ(PKT_CLIGS_CONFIRM_MASTER_BOOK_REQ);

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnConfirmMasterBookAck(*poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 打开勇气试炼
BOOL CGTProcessor::OnOpenCourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_COURAGE_REQ* pReq = (PKT_CLIGS_OPEN_COURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_COURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenCourageReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 打开创建队伍
BOOL CGTProcessor::OnCourageOpenCreateGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ* pReq = (PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageOpenCreateGroupReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 确认创建队伍
BOOL CGTProcessor::OnCourageCreateGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_CREATE_GROUP_REQ* pReq = (PKT_CLIGS_COURAGE_CREATE_GROUP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_CREATE_GROUP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //选择的副本序号
    DT_INSTANCE_ID InstanceID;
    memcpy(&InstanceID,&pReq->stSceneData,sizeof(DT_INSTANCE_ID));

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageCreateGroupReq(poPlayer, pReq->byJoinType, InstanceID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 加入协助队伍
BOOL CGTProcessor::OnCourageJoinGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    //SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    //UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_JOIN_GROUP_REQ* pReq = (PKT_CLIGS_COURAGE_JOIN_GROUP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_JOIN_GROUP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageJoinGroupReq(poPlayer, pReq->byAddBattleType, pReq->dwGroupID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 添加更改伙伴
BOOL CGTProcessor::OnCourageOperateHeroReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_OPERATE_HERO_REQ* pReq = (PKT_CLIGS_COURAGE_OPERATE_HERO_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_OPERATE_HERO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageOperateHeroReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 确认添加更改伙伴
BOOL CGTProcessor::OnCourageConfirmHeroReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_CONFIRM_HERO_REQ* pReq = (PKT_CLIGS_COURAGE_CONFIRM_HERO_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_CONFIRM_HERO_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageConfirmHeroReq(poPlayer, pReq->wKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 踢掉队员
BOOL CGTProcessor::OnCourageFireGroupMemberReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ* pReq = (PKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageFireGroupMemberReq(poPlayer, pReq->dwGroupID, pReq->dwPlayerID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 购买试炼次数
BOOL CGTProcessor::OnCouragBuyPractiseTimesReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ* pReq = (PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageBuyPractiseTimesReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 玩家准备
BOOL CGTProcessor::OnCouragPlayerReadyReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_PLAYER_READY_REQ* pReq = (PKT_CLIGS_COURAGE_PLAYER_READY_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_PLAYER_READY_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCouragePlayerReadyReq(poPlayer, pReq->byExt);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}
// 玩家开始
BOOL CGTProcessor::OnCouragPlayerStartReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_PLAYER_START_REQ* pReq = (PKT_CLIGS_COURAGE_PLAYER_START_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_PLAYER_START_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCouragPlayerStartReq(poPlayer);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 玩家离开
BOOL CGTProcessor::OnCouragLeaveGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_LEAVE_GROUP_REQ* pReq = (PKT_CLIGS_COURAGE_LEAVE_GROUP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_LEAVE_GROUP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCouragLeaveGroupReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 打开购买试炼
BOOL CGTProcessor::OnCouragOpenShopReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_COURAGE_SHOP_REQ* pReq = (PKT_CLIGS_OPEN_COURAGE_SHOP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_COURAGE_SHOP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageOpenShopReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}


//打开珍品阁请求
BOOL CGTProcessor::OnOpenRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_RAREITEMROOM_REQ* pReq = (PKT_CLIGS_OPEN_RAREITEMROOM_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_RAREITEMROOM_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenRareItemAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnExchangeRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BUY_RAREITEM_REQ* pReq = (PKT_CLIGS_BUY_RAREITEM_REQ*)pBody;

    if( sizeof(PKT_CLIGS_BUY_RAREITEM_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnExchangeRareItemAck( poPlayer , pReq->dwGoodsID);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnOpenUpgradeRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_UPGRADERAREITEM_REQ* pReq = (PKT_CLIGS_OPEN_UPGRADERAREITEM_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_UPGRADERAREITEM_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenUpgradeRareItemAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnUpgradeRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_UPGRADE_RAREITEM_REQ* pReq = (PKT_CLIGS_UPGRADE_RAREITEM_REQ*)pBody;

    if( sizeof(PKT_CLIGS_UPGRADE_RAREITEM_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpgradeRareItemAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    PKT_CLIGS_UPGRADE_RAREITEM_ACK* pstAck = (PKT_CLIGS_UPGRADE_RAREITEM_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if ( pstAck->wErrCode == ERR_UPGRADE_RAREITEM::ID_SUCCESS)
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if (NULL != poFaction)
        {
            poFaction->SendMessageByRareItemPrompt();
        }
    }

    return TRUE;
}

//打开武堂请求
BOOL CGTProcessor::OnOpenGymnasiumReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GYMNASIUM_REQ* pReq = (PKT_CLIGS_OPEN_GYMNASIUM_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_GYMNASIUM_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGymnasiumAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//学习武堂技能请求
BOOL CGTProcessor::OnLearnGymnasiumSkillReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ* pReq = (PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ*)pBody;

    if( sizeof(PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnLearnGymnasiumSkillAck( poPlayer , pReq->dwSkillID);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//打开武堂升级请求
BOOL CGTProcessor::OnOpenGymnasiumUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ* pReq = (PKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGymnasiumUpgradeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//确认武堂升级请求
BOOL CGTProcessor::OnGymnasiumUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GYMNASIUM_UPGRADE_REQ* pReq = (PKT_CLIGS_GYMNASIUM_UPGRADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_GYMNASIUM_UPGRADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGymnasiumUpgradeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    PKT_CLIGS_GYMNASIUM_UPGRADE_ACK* pstAck = (PKT_CLIGS_GYMNASIUM_UPGRADE_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if ( ERR_GYMNASIUM_UPGRADE::ID_SUCCESS == pstAck->wErrCode )
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if (NULL != poFaction)
        {
            poFaction->SendMessageByGymnasiumPrompt();
        }
    }
    return TRUE;
}


//打开暗部请求
BOOL CGTProcessor::OnOpenShadeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SHADE_REQ* pReq = (PKT_CLIGS_OPEN_SHADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_SHADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenShadeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//打开黄金模式请求
BOOL CGTProcessor::OnOpenGoldModeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_GOLDMODE_REQ* pReq = (PKT_CLIGS_OPEN_GOLDMODE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_GOLDMODE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGoldModeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//打开暗部升级请求
BOOL CGTProcessor::OnOpenShadeUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SHADE_UPGRADE_REQ* pReq = (PKT_CLIGS_OPEN_SHADE_UPGRADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_SHADE_UPGRADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenShadeUpgradeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//暗部升级请求
BOOL CGTProcessor::OnShadeUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SHADE_UPGRADE_REQ* pReq = (PKT_CLIGS_SHADE_UPGRADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_SHADE_UPGRADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnShadeUpgradeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//捉妖请求
BOOL CGTProcessor::OnOpenHuntingReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SHADE_UPGRADE_REQ* pReq = (PKT_CLIGS_SHADE_UPGRADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_SHADE_UPGRADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenHuntingAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//筋斗去请求
BOOL CGTProcessor::OnFindMonsterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_FIND_MONSTER_REQ* pReq = (PKT_CLIGS_FIND_MONSTER_REQ*)pBody;
    if( sizeof(PKT_CLIGS_FIND_MONSTER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID  = CGTPktBuilder::Instance()->OnFindMonsterAck( poPlayer, *pReq);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

//BOOL CGTProcessor::OnChangeMonsterReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
//{
//	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
//	UINT32 dwTransID            = pPipeHeader->dwTransID;
//
//	PKT_CLIGS_FIND_MONSTER_REQ* pReq = (PKT_CLIGS_FIND_MONSTER_REQ*)pBody;
//	if( sizeof(PKT_CLIGS_FIND_MONSTER_REQ) != dwLen )
//	{
//		return FALSE;
//	}
//
//	CUser* poUser = (CUser*)pUser;
//	CPlayer* poPlayer = poUser->GetPlayer();
//	if(!poPlayer)
//	{
//		return FALSE;
//	}
//
//	UINT16 wMsgID = CGTPktBuilder::Instance()->OnChangeMonsterAck( poPlayer );
//
//	if(INVALID_MSGID == wMsgID)
//	{
//		return FALSE;
//	}
//
//	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
//
//	return TRUE;
//}

BOOL CGTProcessor::OnHuntingBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_HUNTING_BATTLE_REQ* pReq = (PKT_CLIGS_HUNTING_BATTLE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_HUNTING_BATTLE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = INVALID_MSGID;
	UINT32 wErrCode =  ERR_HUNTING_BATTLE::ID_SUCCESS;
	UINT8 byLeftNum = 0;
	if ( (poUser) && (poUser->GetCliVer() >= VERSION_PET) )
	{
		wMsgID = CGTPktBuilder::Instance()->OnHuntingBattleAck( poPlayer );
		PKT_CLIGS_HUNTING_BATTLE_ACK5* pstAck = (PKT_CLIGS_HUNTING_BATTLE_ACK5*)CGTPktBuilder::Instance()->GetPacketBuffer();
		wErrCode = pstAck->wErrCode;
		byLeftNum = pstAck->byHuntingLeftNum;
	}
	else if ( (poUser) && (poUser->GetCliVer() >= VERSION_SOUL))
	{
		wMsgID = CGTPktBuilder::Instance()->OnHuntingBattleAck3( poPlayer );
		PKT_CLIGS_HUNTING_BATTLE_ACK3* pstAck = (PKT_CLIGS_HUNTING_BATTLE_ACK3*)CGTPktBuilder::Instance()->GetPacketBuffer();
		wErrCode = pstAck->wErrCode;
		byLeftNum = pstAck->byHuntingLeftNum;
	}
    else if ((poUser) && (poUser->GetCliVer() > 310))
    {
        wMsgID = CGTPktBuilder::Instance()->OnHuntingBattleAck2( poPlayer );
		PKT_CLIGS_HUNTING_BATTLE_ACK2* pstAck = (PKT_CLIGS_HUNTING_BATTLE_ACK2*)CGTPktBuilder::Instance()->GetPacketBuffer();
		wErrCode = pstAck->wErrCode; 
		byLeftNum = pstAck->byHuntingLeftNum;
    }
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
	
   
	if (wErrCode == ERR_HUNTING_BATTLE::ID_SUCCESS)
	{
		if (0 == byLeftNum)
		{
			CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
			if (NULL != poFactionPlayer)
			{
				PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
				memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
				CShade& oShade =  poFactionPlayer->GetShade();
				oShade.GetPrompt(stNtf.stFactionPrompt);
				poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
			}
		}
	}

    return TRUE;
}

//打开温泉请求
BOOL CGTProcessor::OnOpenHotSpringReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_HOT_SPRING_REQ* pReq = (PKT_CLIGS_OPEN_HOT_SPRING_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_HOT_SPRING_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnHotSpringAck( poPlayer, pReq->wPosX, pReq->wPosY );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}
BOOL CGTProcessor::OnSelectWineReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SELECT_WINE_REQ* pReq = (PKT_CLIGS_SELECT_WINE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_HOT_SPRING_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSelectWineAck( poPlayer, pReq->dwWineID );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    PKT_CLIGS_SELECT_WINE_ACK* pstAck = (PKT_CLIGS_SELECT_WINE_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if ( pstAck->wErrCode == ERR_SELECT_WINE::ID_SUCCESS)
    {
        if (1 == pstAck->bySelectWineState )
        {
            CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
            if (NULL != poFactionPlayer)
            {
                PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
                memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
                CHotSpring& oHotSpring = poFactionPlayer->GetHotSpring();
                oHotSpring.GetPrompt(stNtf.stFactionPrompt);
                poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
            }
        }
    }
    return TRUE;
}

BOOL CGTProcessor::OnDrinkWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DRINK_WINE_REQ* pReq = (PKT_CLIGS_DRINK_WINE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_DRINK_WINE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDrinkWineAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnDrinkWineMoveReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_DRINK_WINE_MOVE_REQ* pReq = (PKT_CLIGS_DRINK_WINE_MOVE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_DRINK_WINE_MOVE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnDrinkWineMoveAck( poPlayer, pReq->wPosX, pReq->wPosY );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnLeaveHotSpringReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEAVE_HOT_SPRING_REQ* pReq = (PKT_CLIGS_LEAVE_HOT_SPRING_REQ*)pBody;
    if( sizeof(PKT_CLIGS_LEAVE_HOT_SPRING_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnLeaveHotSpringAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//BOOL CGTProcessor::OnSelectWineNotDrink(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
//{
//	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
//	UINT32 dwTransID            = pPipeHeader->dwTransID;
//
//	PKT_CLIGS_SELECT_WINE_REQ* pReq = (PKT_CLIGS_SELECT_WINE_REQ*)pBody;
//	if( sizeof(PKT_CLIGS_OPEN_HOT_SPRING_REQ) != dwLen )
//	{
//		return FALSE;
//	}
//
//	CUser* poUser = (CUser*)pUser;
//	CPlayer* poPlayer = poUser->GetPlayer();
//	if(!poPlayer)
//	{
//		return FALSE;
//	}
//
//	UINT16 wMsgID = CGTPktBuilder::Instance()->OnSelectWineNotDrinkAck( poPlayer, pReq->dwWineID );
//
//	if(INVALID_MSGID == wMsgID)
//	{
//		return FALSE;
//	}
//
//	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
//	return TRUE;
//}

BOOL CGTProcessor::OnChangeOtherReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CHANGE_OTHER_REQ* pReq = (PKT_CLIGS_CHANGE_OTHER_REQ*)pBody;
    if( sizeof(PKT_CLIGS_CHANGE_OTHER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnChangeOtherAck( poPlayer, pReq->dwSendPlayerID );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//赐酒
BOOL CGTProcessor::OnGiveWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_GIVE_WINE_REQ* pReq = (PKT_CLIGS_GIVE_WINE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_GIVE_WINE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGiveWineAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//点击选酒
BOOL CGTProcessor::OnClickSelectWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLICK_SELECT_WINE_REQ* pReq = (PKT_CLIGS_CLICK_SELECT_WINE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_CLICK_SELECT_WINE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClickSelectWineAck( poPlayer, pReq->byDrinkWIneState);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

////寻找妖怪
//BOOL CGTProcessor::OnFirstFindMonsterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
//{
//	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
//	UINT32 dwTransID            = pPipeHeader->dwTransID;
//
//	PKT_CLIGS_FIRST_FIND_MONSTER_REQ* pReq = (PKT_CLIGS_FIRST_FIND_MONSTER_REQ*)pBody;
//	if( sizeof(PKT_CLIGS_FIRST_FIND_MONSTER_REQ) != dwLen )
//	{
//		return FALSE;
//	}
//
//	CUser* poUser = (CUser*)pUser;
//	CPlayer* poPlayer = poUser->GetPlayer();
//	if(!poPlayer)
//	{
//		return FALSE;
//	}
//
//	UINT16 wMsgID = CGTPktBuilder::Instance()->OnFirstFindMonsterAck( poPlayer );
//
//	if(INVALID_MSGID == wMsgID)
//	{
//		return FALSE;
//	}
//
//	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
//	return TRUE;
//}

BOOL CGTProcessor::OnSureGiveWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SURE_GIVE_WINE_REQ* pReq = (PKT_CLIGS_SURE_GIVE_WINE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_SURE_GIVE_WINE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSureGiveWineAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    PKT_CLIGS_SURE_GIVE_WINE_ACK* pGiveWine = (PKT_CLIGS_SURE_GIVE_WINE_ACK*)CGTPktBuilder::Instance()->GetPacketBuffer();
    if ( ERR_SURE_GIVE_WINE::ID_SUCCESS == pGiveWine->wErrCode)
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if (NULL == poFaction)
        {
            return FALSE;
        }
        PKT_CLIGS_GIVE_WINE_ACTIVITY_NTF stNtf;
        stNtf.qwCountDownTime = pGiveWine->qwActiviTime;
        poFaction->SendMessageToAllFactionPlayer( (TCHAR*)&stNtf, CLIGS_GIVE_WINE_ACTIVITY_NTF, poPlayer->GetID());
    }

    return TRUE;
}

BOOL CGTProcessor::OnClosedMonsterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_CLOSED_MONSTER_REQ* pReq = (PKT_CLIGS_CLOSED_MONSTER_REQ*)pBody;
    if( sizeof(PKT_CLIGS_CLOSED_MONSTER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnClosedMonsterAck( poPlayer, pReq->byChoice );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnFactionPlayerMarkReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_FACTION_PLAYER_CHANGE_REQ* pReq = (PKT_CLIGS_FACTION_PLAYER_CHANGE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_FACTION_PLAYER_CHANGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnFactionPlayerMarkAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

//打开退出门派
BOOL CGTProcessor::OnOpenLeaveFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen  )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_LEAVE_FACTION_REQ* pReq = (PKT_CLIGS_OPEN_LEAVE_FACTION_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_LEAVE_FACTION_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenLeaveFactionAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
//退出门派
BOOL CGTProcessor::OnLeaveFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen  )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEAVE_FACTION_REQ* pReq = (PKT_CLIGS_LEAVE_FACTION_REQ*)pBody;
    if( sizeof(PKT_CLIGS_LEAVE_FACTION_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }
    CBuild* poBuild = poPlayer->GetBuild(EBK_FACTION);
    if ( NULL == poBuild)
    {
        PKT_CLIGS_LEAVE_FACTION_ACK stLeaveFaction;
        stLeaveFaction.wErrCode = ERR_LEAVE_FACTION::ID_UNOPEN;
        poPlayer->SendMsg((TCHAR*)&stLeaveFaction, CLIGS_LEAVE_FACTION_ACK);
        return TRUE;
    }
    CFaction* poFaction =  poPlayer->GetFaction();
    if (NULL == poFaction)
    {
        PKT_CLIGS_LEAVE_FACTION_ACK stLeaveFaction;
        stLeaveFaction.wErrCode = ERR_LEAVE_FACTION::ID_FACTION_NOT_EXIST;
        poPlayer->SendMsg((TCHAR*)&stLeaveFaction, CLIGS_LEAVE_FACTION_ACK);
        return TRUE;
    }
    DT_FACTION_PLAYER_CLI stFactionPlayerCli;
    poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, poPlayer->GetID());
    //如果是掌门不能退出门派
    if ( em_Faction_MartialHead == stFactionPlayerCli.byJobType)
    {
        UINT16 wCurPlayerNum = 0;
        UINT16 wMaxPlayerNum = 0;
        poFaction->GetFactionPlayerNum(wCurPlayerNum, wMaxPlayerNum);
        //只剩帮主一人
        if ( 1 == wCurPlayerNum )
        {
            if (!CGvGBuild::Instance()->CanLeaveFaction(poFaction->GetFactionID()))
            {
                PKT_CLIGS_LEAVE_FACTION_ACK stLeaveFaction;
                stLeaveFaction.wErrCode = ERR_LEAVE_FACTION::ID_IN_GVG_ACTIVITY;
                poPlayer->SendMsg((TCHAR*)&stLeaveFaction, CLIGS_LEAVE_FACTION_ACK);
                return TRUE;
            }            

            //删除SQL记录
            CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_LEAVE_FACTION);
            if(NULL == poCMD)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
                return FALSE;
            }
            poCMD->SetUserData((VOID*)pReq, poFaction->GetDT_FACTION_BASE_DATA().dwFactionID);
            gsapi::GetDBMgr()->AddCommand(poCMD);
        }
        else
        {
            PKT_CLIGS_LEAVE_FACTION_ACK stLeaveFaction;
            stLeaveFaction.wErrCode = ERR_LEAVE_FACTION::ID_HEAD_NOT_LEAVE;
            poPlayer->SendMsg((TCHAR*)&stLeaveFaction, CLIGS_LEAVE_FACTION_ACK);
            return TRUE;
        }
    }
    else
    {
        if (!CGvGBuild::Instance()->CanLeaveFaction(poFaction->GetFactionID()))
        {
            PKT_CLIGS_LEAVE_FACTION_ACK stLeaveFaction;
            stLeaveFaction.wErrCode = ERR_LEAVE_FACTION::ID_IN_GVG_ACTIVITY_CAN_NOT_LEAVE;
            poPlayer->SendMsg((TCHAR*)&stLeaveFaction, CLIGS_LEAVE_FACTION_ACK);
            return TRUE;
        }

        PKT_CLIGS_LEAVE_FACTION_ACK stLeaveFaction;
        CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
        if ( NULL != poFactionPlayer)
        {
            stLeaveFaction.wErrCode = poFactionPlayer->LeaveFaction();
            poPlayer->SendMsg((TCHAR*)&stLeaveFaction, CLIGS_LEAVE_FACTION_ACK);
        }

        poFactionPlayer = poPlayer->GetFactionPlayer();
        if (NULL != poFactionPlayer)
        {
            PKT_CLIGS_HOME_BUILD_PROMPT_NTF stNtf;
            memset(&stNtf, 0, sizeof(PKT_CLIGS_HOME_BUILD_PROMPT_NTF));
            poPlayer->GetFactionPrompt(stNtf.stPromptInfo.astBuildPromptInfo[stNtf.stPromptInfo.byBuildNum++]);
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_HOME_BUILD_PROMPT_NTF);
        }
    }

    return TRUE;
}

BOOL CGTProcessor::OnOpenFactionUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen  )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_FACTION_UPGRADE_REQ* pReq = (PKT_CLIGS_OPEN_FACTION_UPGRADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_FACTION_UPGRADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenFactionUpgradeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnFactionUpgradeReq(  VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_FACTION_UPGRADE_REQ* pReq = (PKT_CLIGS_FACTION_UPGRADE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_FACTION_UPGRADE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnFactionUpgradeAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnGetFactionBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_PLAYER_FACTION_READ_REQ* pReq = (PKT_CLIGS_PLAYER_FACTION_READ_REQ*)pBody;
    if( sizeof(PKT_CLIGS_PLAYER_FACTION_READ_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->GetFactionBattleLogAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenFindDrinkReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_PLAYER_FACTION_READ_REQ* pReq = (PKT_CLIGS_PLAYER_FACTION_READ_REQ*)pBody;
    if( sizeof(PKT_CLIGS_PLAYER_FACTION_READ_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenFindDrinkAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 勇气战斗请求
BOOL CGTProcessor::OnCourageBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_BATTLE_REQ* pReq = (PKT_CLIGS_COURAGE_BATTLE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_COURAGE_BATTLE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageBattleReq( poPlayer , pReq->byMonsterIndex);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenFactionBuildGroupReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ* pReq = (PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OpenFactionBuildGroupAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnHotSpringOverRewardReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ* pReq = (PKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ*)pBody;
    if( sizeof(PKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnHotSpringOverRewardAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}
BOOL CGTProcessor::OnCourageRefreshGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_GROUP_REFRESH_REQ* pReq = (PKT_CLIGS_COURAGE_GROUP_REFRESH_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_GROUP_REFRESH_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCouragRefreshGroup(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnCourageEnterGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_ENTER_GROUP_REQ* pReq = (PKT_CLIGS_COURAGE_ENTER_GROUP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_ENTER_GROUP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageEnterGroupReq(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnLeaveCourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEAVE_COURAGE_REQ* pReq = (PKT_CLIGS_LEAVE_COURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_LEAVE_COURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnLeaveCourageReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}

BOOL CGTProcessor::OnHuntingIntegralReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_HUNTING_ITEGRAL_REQ* pReq = (PKT_CLIGS_HUNTING_ITEGRAL_REQ*)pBody;
    if( sizeof(PKT_CLIGS_HUNTING_ITEGRAL_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->HuntingIntegralAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnIgnorePromptReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_PROMPT_IGNORE_REQ* pReq = (PKT_CLIGS_PROMPT_IGNORE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_PROMPT_IGNORE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnIgnorePromptAck( poPlayer, pReq->byActiviID );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnOpenLevelHotSpring(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ* pReq = (PKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenLeaveHotSpring( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 领取勇气奖励
BOOL CGTProcessor::OnCourageGetAwardReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_GET_AWARD_REQ* pReq = (PKT_CLIGS_COURAGE_GET_AWARD_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_GET_AWARD_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageGetAwardReq(poPlayer, pReq->dwExt);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 勇气进入战斗场景
BOOL CGTProcessor::OnCourageEnterBattleSceneReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    return TRUE;
}

BOOL CGTProcessor::OnCouragGetBattleResultReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ* pReq = (PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCouragGetBattleResultReq(poPlayer);

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}



BOOL CGTProcessor::OnAuditAllRefuseReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_AUDIT_ALL_REFUSE_REQ* pReq = (PKT_CLIGS_AUDIT_ALL_REFUSE_REQ*)pBody;
    if( sizeof(PKT_CLIGS_AUDIT_ALL_REFUSE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAuditAllRefuseAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}


BOOL CGTProcessor::OnOpenRecvFlowerTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ* pReq = (PKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ*)pBody;
    if( sizeof(PKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenRecvFlowerTabAck( poPlayer );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

BOOL CGTProcessor::OnAcceptFlowerReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_ACCEPT_FLOWER_REQ* pReq = (PKT_CLIGS_ACCEPT_FLOWER_REQ*)pBody;
    if( sizeof(PKT_CLIGS_ACCEPT_FLOWER_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnAcceptFlowerAck( poPlayer, pReq->dwSendPlayerID, pReq->qwSendTime );

    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }

    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
    return TRUE;
}

// 打开送花面板
BOOL CGTProcessor::OnOpenSendFlowerTabExReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ* pReq = (PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ*)pBody;

    if( sizeof(PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenSendFlowerTabExAck(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL	CGTProcessor::OnSendFlowerExReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_SEND_FLOWER_EX_REQ* pReq = (PKT_CLIGS_SEND_FLOWER_EX_REQ*)pBody;

    if( sizeof(PKT_CLIGS_SEND_FLOWER_EX_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    //真实需要获得信息的玩家
    CPlayer *poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwRecvPlayerID);
    if( NULL == poTargetPlayer )
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwTransID, GET_PLAYERDATATYPE_SENDFLOWEREX, pReq->dwRecvPlayerID, pReq->dwKindID);
        return TRUE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSendFlowerExAck(poPlayer, poTargetPlayer, pReq->dwKindID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL CGTProcessor::OnBuyRareItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_BUY_RAREITEM_REQ2* pReq = (PKT_CLIGS_BUY_RAREITEM_REQ2*)pBody;

    if( sizeof(PKT_CLIGS_BUY_RAREITEM_REQ2) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnBuyRareItemAck(poPlayer, pReq->dwGoodsID, pReq->byLeftTime);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

BOOL  CGTProcessor::OnLearnGymnasiumSkillReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2* pReq = (PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2*)pBody;

    if( sizeof(PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnLearnGymnasiumSkillAck2(poPlayer, pReq->dwSkillID, pReq->bySkillCurLevel);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
}

// 战斗离开勇气试炼
BOOL CGTProcessor::OnCourageLeaveBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ* pReq = (PKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageLeaveBattleReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}

// 随机加入协助队伍
BOOL CGTProcessor::OnCourageRandomJoinGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ* pReq = (PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ*)pBody;

    if( sizeof(PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnCourageRandomJoinGroupReq( poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);


    return TRUE;
}


BOOL CGTProcessor::OnRegisterFactionName(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLILS_REGISTER_FACTION_REQ* pReq = (PKT_CLILS_REGISTER_FACTION_REQ*)pBody;
	if ( sizeof(PKT_CLILS_REGISTER_FACTION_REQ) != dwLen )
		return FALSE;

	CUser* poUser = (CUser*)pUser;

	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnRegisterFactionNameAck(poPlayer, pReq->aszDspName);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;

}
BOOL CGTProcessor::OnFactionPlayerInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_FACTION_PLAYERS_INFO_REQ* pReq = (PKT_CLIGS_FACTION_PLAYERS_INFO_REQ*)pBody;

	if( sizeof(PKT_CLIGS_FACTION_PLAYERS_INFO_REQ) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnFactionPlayerInfoAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenChangeIconReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ* pReq = (PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenChangeIconAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnChangeIconReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_CHANGE_FACTION_ICON_REQ* pReq = (PKT_CLIGS_CHANGE_FACTION_ICON_REQ*)pBody;

	if( sizeof(PKT_CLIGS_CHANGE_FACTION_ICON_REQ) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnChangeIconAck(poPlayer, pReq->byIconID);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

//打开找回门贡
BOOL CGTProcessor::OnOpenFindBackDoorsTributeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ* pReq = (PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenFindBackDoorsTributeAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

//找回门贡
BOOL CGTProcessor::OnFindBackDoorsTributeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ* pReq = (PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnFindBackDoorsTributeAck(poPlayer, pReq->byFindBackType);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenEmbattleReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_EMBATTLE_REQ2* pReq = (PKT_CLIGS_OPEN_EMBATTLE_REQ2*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_EMBATTLE_REQ2) != dwLen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenEmbattleAck2(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

//升级阵法
 BOOL CGTProcessor::OnUpgradeFormationReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_UPGRADE_FORMATION_REQ* pReq = (PKT_CLIGS_UPGRADE_FORMATION_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_UPGRADE_FORMATION_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpgradeFormationAck(poPlayer, pReq->byFormationID, pReq->stFormationInfo);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

//打开阵法（科技馆）
 BOOL CGTProcessor::OnOpenFormationScienceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ* pReq = (PKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenFormationScienceAck(poPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;

 }

BOOL CGTProcessor::OnGetBlueEncourage( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;


    PKT_CLIGS_GET_BLUE_ENCOURAGE_REQ* pReq = (PKT_CLIGS_GET_BLUE_ENCOURAGE_REQ*)pBody;

    if( sizeof(PKT_CLIGS_GET_BLUE_ENCOURAGE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetBlueEncourage(poPlayer);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
 }


//升级阵法
BOOL CGTProcessor::OnUpgradeFormationScienceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;

    PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ* pReq = (PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ*)pBody;
 
    if( sizeof(PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ) != dwLen )
    {
        return FALSE;
    }

    CUser* poUser = (CUser*)pUser;
    CPlayer* poPlayer = poUser->GetPlayer();
    if(!poPlayer)
    {
        return FALSE;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpgradeFormationScienceAck(poPlayer, pReq->byFormationID);
    if(INVALID_MSGID == wMsgID)
    {
        return FALSE;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

    return TRUE;
 }
 BOOL CGTProcessor::OnGetExprienceHotSpringReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ* pReq = (PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetExpeienceHotSpringAck(poPlayer, pReq->dwExprienceValue);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 BOOL CGTProcessor::OnOpenFactionSkillReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_OPEN_FACTION_SKILL_REQ* pReq = (PKT_CLIGS_OPEN_FACTION_SKILL_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_OPEN_FACTION_SKILL_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenFactionSkillAck(poPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 BOOL CGTProcessor::OnOpenResolveTrigramReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
 {
     SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
     UINT32 dwTransID            = pPipeHeader->dwTransID;

     PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ* pReq = (PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ*)pBody;
     if ( sizeof(PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ) != dwLen )
         return FALSE;

     CUser* poUser = (CUser*)pUser;

     CPlayer* poPlayer = poUser->GetPlayer();
     if(!poPlayer)
     {
         return FALSE;
     }

     UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenResolveTrigram(poPlayer, pReq->byBagGridIdx);
     if(INVALID_MSGID == wMsgID)
     {
         return FALSE;
     }
     poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

     return TRUE;
 }

 BOOL CGTProcessor::OnResolveTrigramReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen )
 {
     SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
     UINT32 dwTransID            = pPipeHeader->dwTransID;

     PKT_CLIGS_RESOLVE_TRIGRAM_REQ* pReq = (PKT_CLIGS_RESOLVE_TRIGRAM_REQ*)pBody;
     if ( sizeof(PKT_CLIGS_RESOLVE_TRIGRAM_REQ) != dwLen )
         return FALSE;

     CUser* poUser = (CUser*)pUser;

     CPlayer* poPlayer = poUser->GetPlayer();
     if(!poPlayer)
     {
         return FALSE;
     }

     UINT16 wMsgID = CGTPktBuilder::Instance()->OnResolveTrigram(poPlayer,pReq->byBagGridIdx,pReq->byUseGold);
     if(INVALID_MSGID == wMsgID)
     {
         return FALSE;
     }
     poUser->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

     return TRUE;
 }

 BOOL CGTProcessor::OnOpenPlayerPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_OPEN_PLAYER_PANEL_REQ* pReq = (PKT_CLIGS_OPEN_PLAYER_PANEL_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_OPEN_PLAYER_PANEL_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerPanelAck(poPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 BOOL CGTProcessor::OnOpenPlayerInfoAwakenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_PLAYER_INFO_AWAKEN_REQ* pReq = (PKT_CLIGS_PLAYER_INFO_AWAKEN_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_PLAYER_INFO_AWAKEN_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	 if (NULL == poTargetPlayer)
	 {
		 //内存中查找不到，需要到数据库中去查询
		 CGetPlayerDataMgr::Instance()->GetPlayerData( poPlayer->GetID(), GET_PLAYER_INFO_AWAKEN,
			 pReq->dwPlayerID);
		 return TRUE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoAwakenAck(poTargetPlayer );
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 //打开人物信息--技能
 BOOL CGTProcessor::OnOpenPlayerInfoSkillReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_PLAYER_INFO_SKILL_REQ* pReq = (PKT_CLIGS_PLAYER_INFO_SKILL_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_PLAYER_INFO_SKILL_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }
	 CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	 if (NULL == poTargetPlayer)
	 {
		 //内存中查找不到，需要到数据库中去查询
		 CGetPlayerDataMgr::Instance()->GetPlayerData( poPlayer->GetID(), GET_PLAYER_INFO_SKILL,
			 pReq->dwPlayerID);
		 return TRUE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoSkillAck(poTargetPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 BOOL CGTProcessor::OnOpenPlayerInfoGodWeaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ* pReq = (PKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ) != dwLen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	 if (NULL == poTargetPlayer)
	 {
		 //内存中查找不到，需要到数据库中去查询
		 CGetPlayerDataMgr::Instance()->GetPlayerData( poPlayer->GetID(), GET_PLAYER_INFO_GODWEAPON,
			 pReq->dwPlayerID);
		 return TRUE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoGodWeaponAck(poTargetPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 //打开人物信息--饰品
 BOOL CGTProcessor::OnOpenPlayerInfoJewelryReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_PLAYER_INFO_JEWELRY_REQ* pReq = (PKT_CLIGS_PLAYER_INFO_JEWELRY_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_PLAYER_INFO_JEWELRY_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	 if (NULL == poTargetPlayer)
	 {
		 //内存中查找不到，需要到数据库中去查询
		 CGetPlayerDataMgr::Instance()->GetPlayerData( poPlayer->GetID(), GET_PLAYER_INFO_JEWELRY,
			 pReq->dwPlayerID);
		 return TRUE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoJewelryAck(poTargetPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
	 return TRUE;
 }

 //打开人物信息--装备
 BOOL CGTProcessor::OnOpenPlayerInfoEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_PLAYER_INFO_EQUIP_REQ* pReq = (PKT_CLIGS_PLAYER_INFO_EQUIP_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_PLAYER_INFO_EQUIP_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	 if (NULL == poTargetPlayer)
	 {
		 //内存中查找不到，需要到数据库中去查询
		 CGetPlayerDataMgr::Instance()->GetPlayerData( poPlayer->GetID(), GET_PLAYER_INFO_EQUIP,
			 pReq->dwPlayerID);
		 return TRUE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoEquipAck(poTargetPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
	 return TRUE;
 }

BOOL CGTProcessor::OnOpenPlayerInfoSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_PLAYER_INFO_SOUL_REQ)
	CPlayer* pTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	if (NULL == pTargetPlayer) {
		//内存中查找不到，需要到数据库中去查询
		CGetPlayerDataMgr::Instance()->GetPlayerData(poPlayer->GetID(), GET_PLAYER_SOUL, pReq->dwPlayerID);
		return TRUE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoSoulAck(pTargetPlayer);
	if(INVALID_MSGID == wMsgID) {
		return FALSE;
	}
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenPlayerInfoPetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_PLAYER_INFO_PET_REQ)
		CPlayer* pTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
	if (NULL == pTargetPlayer) {
		//内存中查找不到，需要到数据库中去查询
		CGetPlayerDataMgr::Instance()->GetPlayerData(poPlayer->GetID(), GET_PLAYER_PET, pReq->dwPlayerID);
		return TRUE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerInfoPetAck(pTargetPlayer);
	if(INVALID_MSGID == wMsgID) {
		return FALSE;
	}
	SEND_ACK
		return TRUE;
}
 BOOL CGTProcessor::OnDressJewelryReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_DRESS_JEWELRY_REQ* pReq = (PKT_CLIGS_DRESS_JEWELRY_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_DRESS_JEWELRY_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnDressJewelryAck(poPlayer, *pReq);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 BOOL CGTProcessor::OnOpenHeroSelectPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ* pReq = (PKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenHeroSelectPanelAck(poPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 BOOL CGTProcessor::OnOpenJewelryGenerateReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ* pReq = (PKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenJewlryGenerateAck(poPlayer);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }
 //饰品合成
 BOOL CGTProcessor::OnJewelryGenerateReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_JEWELRY_GENERATE_REQ* pReq = (PKT_CLIGS_JEWELRY_GENERATE_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_JEWELRY_GENERATE_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnJewlryGenerateAck(poPlayer, *pReq);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }
 //饰品强化
 BOOL CGTProcessor::OnJewelryStrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_JEWELRY_STRENGTH_REQ* pReq = (PKT_CLIGS_JEWELRY_STRENGTH_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_JEWELRY_STRENGTH_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnJewlryStrengthAck(poPlayer, *pReq);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 //饰品合成信息
 BOOL CGTProcessor::OnJewelryGenerateInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ* pReq = (PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnJewlryGenerateInfoAck(poPlayer, *pReq);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

 //分解信息
 BOOL CGTProcessor::OnJewelryResolveReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
 {
	 SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	 UINT32 dwTransID            = pPipeHeader->dwTransID;

	 PKT_CLIGS_JEWELRY_RESOLVE_REQ* pReq = (PKT_CLIGS_JEWELRY_RESOLVE_REQ*)pBody;

	 if( sizeof(PKT_CLIGS_JEWELRY_RESOLVE_REQ) != dwlen )
	 {
		 return FALSE;
	 }

	 CUser* poUser = (CUser*)pUser;
	 CPlayer* poPlayer = poUser->GetPlayer();
	 if(!poPlayer)
	 {
		 return FALSE;
	 }

	 UINT16 wMsgID = CGTPktBuilder::Instance()->OnJewelryResolveAck(poPlayer, *pReq);
	 if(INVALID_MSGID == wMsgID)
	 {
		 return FALSE;
	 }
	 poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	 return TRUE;
 }

BOOL CGTProcessor::OnOpenJewelryStrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ* pReq = (PKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenJewelryStrengthAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

//首饰对比
BOOL CGTProcessor::OnCompareJewelryReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_JEWELRY_COMPARE_REQ* pReq = (PKT_CLIGS_JEWELRY_COMPARE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_JEWELRY_COMPARE_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnJewelryCompareAck(poPlayer, *pReq);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnJewelryExtStrengReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ* pReq = (PKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnJewelryExtStrengAck(poPlayer, *pReq);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenJewelryResolveReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ* pReq = (PKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenJewelryResolve(poPlayer, *pReq);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenGVGFormationReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_GVG_FORMATION_REQ* pReq = (PKT_CLIGS_OPEN_GVG_FORMATION_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_GVG_FORMATION_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGVGFormationAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenGVGActivityReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_GVG_ACTIVITY_REQ* pReq = (PKT_CLIGS_OPEN_GVG_ACTIVITY_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_GVG_ACTIVITY_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

    // USR_INFO(_SDT("[%s: %d]: CGTProcessor::OnOpenGVGActivityReq: <current stage = %u>."), MSG_MARK, CGvGBuild::Instance()->GetCurStage());

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGVGActivityAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenAlreadApplayReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_ALREAD_APPLAY_REQ* pReq = (PKT_CLIGS_OPEN_ALREAD_APPLAY_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_ALREAD_APPLAY_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenAlreadApplayAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenGVGLastResultReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_GVG_LAST_RESULT_REQ* pReq = (PKT_CLIGS_GVG_LAST_RESULT_REQ*)pBody;

	if( sizeof(PKT_CLIGS_GVG_LAST_RESULT_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGVGLastResultAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}


BOOL CGTProcessor::OnOpenGVGMyGambleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ* pReq = (PKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnGVGOpenMyGambleAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnGVGOpenGambleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_GVG_OPEN_GAMBLE_REQ* pReq = (PKT_CLIGS_GVG_OPEN_GAMBLE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_GVG_OPEN_GAMBLE_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnGVGOpenGambleAck(poPlayer, *pReq);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnGVGOpenRuleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_OPEN_GVG_RULE_REQ* pReq = (PKT_CLIGS_OPEN_GVG_RULE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_OPEN_GVG_RULE_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnGVGOpenRuleAck(poPlayer);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnGVGGambleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	PKT_CLIGS_GVG_GAMBLE_REQ* pReq = (PKT_CLIGS_GVG_GAMBLE_REQ*)pBody;

	if( sizeof(PKT_CLIGS_GVG_GAMBLE_REQ) != dwlen )
	{
		return FALSE;
	}

	CUser* poUser = (CUser*)pUser;
	CPlayer* poPlayer = poUser->GetPlayer();
	if(!poPlayer)
	{
		return FALSE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnGVGGambleAck(poPlayer, *pReq);
	if(INVALID_MSGID == wMsgID)
	{
		return FALSE;
	}
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);

	return TRUE;
}

BOOL CGTProcessor::OnOpenGvGLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_OPEN_GVG_LOG_REQ);
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGvGLogAck(poPlayer, pReq->dwFactionID);
    SEND_ACK
    return TRUE;
}

BOOL CGTProcessor::OnSeeGvGVideoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_SEE_GVG_VIDEO_REQ);
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnSeeGVGVideoAck(*poPlayer, pReq->qwVideoID, pReq->byVideoNum);
    SEND_ACK

    return TRUE;
}

BOOL CGTProcessor::OnEndGvGVideoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_END_GVG_VIDEO_REQ);
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnEndGvGVideoAck(*poPlayer, pReq->qwVideoID, pReq->byEndType);
    SEND_ACK

    return TRUE;
}

BOOL CGTProcessor::OnShowGvGMVPReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_SHOW_GVG_MVP_REQ);
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnShowGvGMVPAck(pReq->qwVideoID);
    SEND_ACK

    return TRUE;
}

BOOL CGTProcessor::OnOpenGVGMoneyRankReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_FACTION_MONEY_RANK_REQ)

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGVGMoneyRankAck(poPlayer);

	SEND_ACK

	return TRUE;
}

BOOL CGTProcessor::OnReadFormationPlayerTipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_READ_FORMATION_PLAYER_TIP_REQ)
	
	CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwReqPlayerID);
	if ( NULL == poTargetPlayer)
	{
		//内存中查找不到，需要到数据库中去查询
		CGetPlayerDataMgr::Instance()->GetPlayerData( poPlayer->GetID(), GET_PLAYER_FORMATION_TIP,
			pReq->dwReqPlayerID);
		return TRUE;
	}

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnReadFormationPlayerTipAck(poTargetPlayer);

	SEND_ACK

	return TRUE;
}

BOOL CGTProcessor::OnGvGFactiomEmbattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_GVG_FACTION_EMBATTLE_REQ)

	UINT16 wMsgID = CGTPktBuilder::Instance()->OnGvGFactionEmbattleAck(poPlayer, *pReq);

	SEND_ACK

	return TRUE;
}

//查看礼包
BOOL CGTProcessor::OnOpenGvGiftReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_GVG_GIFT_REQ)

		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenGvGiftAck(poPlayer);

	SEND_ACK

		return TRUE;
}
//领取礼包
BOOL CGTProcessor::OnGetGvGiftReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_GET_GVG_GIFT_REQ)

		UINT16 wMsgID = CGTPktBuilder::Instance()->OnGetGvGiftAck(poPlayer, *pReq);

	SEND_ACK

		return TRUE;
}

BOOL CGTProcessor::OnSelectGroupTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_SELECT_GROUP_TYPE_REQ)

		UINT16 wMsgID = CGTPktBuilder::Instance()->OnSelectGroupTypeAck(poPlayer, *pReq);

	SEND_ACK

		return TRUE;
}

BOOL CGTProcessor::OnGvGFactionApplayReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_GVG_FACTION_APPLAY_REQ)

		UINT16 wMsgID = CGTPktBuilder::Instance()->OnGvGFactionApplayAck(poPlayer);

	SEND_ACK

		return TRUE;
}

BOOL CGTProcessor::OnReadBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_READ_BATTLE_LOG_REQ)

		UINT16 wMsgID = CGTPktBuilder::Instance()->OnReadBattleLogAck(poPlayer, *pReq);

	SEND_ACK

		return TRUE;
}

//打开战魂吞噬
BOOL CGTProcessor::OnOpenDevourSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_DEVOUR_SOUL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenDeourSoulAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}
//吞噬战魂
BOOL CGTProcessor::OnDevourSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_DEVOUR_SOUL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnDeourSoulAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}
//打开战魂洗炼
BOOL CGTProcessor::OnOpenReflashSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_REFLASH_SOUL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenReflashSoulAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}
//洗炼战魂
BOOL CGTProcessor::OnReflashSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_REFLASH_SOUL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnReflashSoulAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnSoulRepInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_SOULREP_INFO_REQ);
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnSoulRepInfoAck(poPlayer, *pReq);
	SEND_ACK;
	return TRUE;
}

BOOL CGTProcessor::OnProductSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_PRODUCT_SOUL_REQ);
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnProductSoulAck(poPlayer, *pReq);
	SEND_ACK;
	return TRUE;
}

BOOL CGTProcessor::OnEquipSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_HERO_EQUIP_SOUL_REQ);
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnEquipSoulAck(poPlayer, *pReq);
	SEND_ACK;
	return TRUE;
}

BOOL CGTProcessor::OnUnDressSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_HERO_UNDRESS_SOUL_REQ);
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnUnDressSoulAck(poPlayer, *pReq);
	SEND_ACK;
	return TRUE;
}

BOOL CGTProcessor::OnReplaceSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_HERO_REPLACE_SOUL_REQ);
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnReplaceSoulAck(poPlayer, *pReq);
	SEND_ACK;
	return TRUE;
}

BOOL CGTProcessor::OnSaveReflashSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_SAVE_REFLASH_SOUL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnSaveReflashSoulAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnSoulExchangeInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_SOULEXCHANGE_INFO_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnSoulExchangeInfoReq(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnExchangeSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_EXCHANGE_SOUL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnExchangeSoulReq(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenMyMedalReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_MY_MEDAL_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenMyMedalAck(poPlayer);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnViewFactionMembersReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_VIEW_FACTION_MEMBERS_REQ)
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnViewFactionMembersAck(pReq->dwFactionID);
    SEND_ACK

    return TRUE;
}
BOOL CGTProcessor::OnOpenMapReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_OUT_BOUND_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenMapAck(poPlayer);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnOpenOutBoundMapReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenOutBoundMapAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnOutBoundBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OUT_BOUND_BATTLE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOutBoundBattleAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}
BOOL CGTProcessor::OnTestReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_TEST_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnTestAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnOpenPetRepReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_PETREP_INFO_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPetRepAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnProductPetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_PRODUCT_PET_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnProductPetAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenRefinePetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_REFINEPET_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenRefinePetAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenPetPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_PET_PANEL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPetPanel(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnRefinePetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_REFINEPET_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnRefinePetAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenStarPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_STAR_PANEL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenStarPanelAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnUpPetStarReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_UP_STAR_LV_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpPetStarAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenPetFeedPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_PETFEED_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPetFeedAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnPetFristDrawReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_FIRST_DRAW_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnFirstDrawAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnPetResetDrawReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_RESET_DRAW_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnResetDrawAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnConfirmFeedReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_CONFIRM_FEED_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnConfirmFeedAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnPetMerge(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_MERGE_PET_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnPetMerge(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenPetStoreReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_PET_STORE_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPetStoreAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnBuyPetStoreItem(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_BUY_PET_STORE_ITEM_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnBuyPetStoreItemAck(poPlayer, *pReq);
	SEND_ACK;

	PKT_CLIGS_OPEN_PET_STORE_REQ openStoreReq;
	CGTPktBuilder::Instance()->OnOpenPetStoreAck(poPlayer, openStoreReq);
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), CLIGS_OPEN_PET_STORE_REQ);

	return TRUE;
}

BOOL CGTProcessor::OnOpenWashQuality(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_WASH_QUALITY_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenWashQualityAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnWashQuality(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_WASH_QUALITY_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnWashQualityAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnChangePetState(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_CHANGE_PET_STATE_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnChangePetStateAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenPetInherit(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_PET_INHERIT_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenInheritPetAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnInheritPet(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_PET_INHERIT_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnPetInheritAck(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenEquipMakeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_OPEN_EQUIP_MAKE_REQ)
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenEquipMakeAck(poPlayer, *pReq);
    SEND_ACK
    return TRUE;
}

BOOL CGTProcessor::OnEquipMakeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_EQUIP_MAKE_REQ)
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnEquipMakeAck(poPlayer, *pReq);
    SEND_ACK
    return TRUE;
}

BOOL CGTProcessor::OnUpgradeHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_UPGRADE_HERO_REQ)
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpgradeHeroAck(poPlayer, *pReq);
    SEND_ACK
    return TRUE;
}

BOOL CGTProcessor::OnOpenFlyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_OPEN_FLY_REQ)
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenFlyAck(poPlayer, *pReq);
    SEND_ACK
    return TRUE;
}

BOOL CGTProcessor::OnFlyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    CHECK_ON_REQ(PKT_CLIGS_FLY_REQ)
    UINT16 wMsgID = CGTPktBuilder::Instance()->OnFlyAck(poPlayer, *pReq);
    SEND_ACK
    return TRUE;
}

BOOL CGTProcessor::OnOpenPlayerPetPanel(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPlayerPetPanel(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnRequestAllPetInfo(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen) {
	CHECK_ON_REQ(PKT_CLIGS_REQUEST_ALL_PET_INFO_REQ)
	UINT16 wMsgID = CGTPktBuilder::Instance()->OnRequestAllPetInfo(poPlayer, *pReq);
	SEND_ACK
	return TRUE;
}

BOOL CGTProcessor::OnOpenStoreReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_STORE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenStoreAck(poPlayer);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnExchangeGoodsReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_EXCHANGE_GOODS_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnExchangeGoodsAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnOpenPetRule(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_PET_RULE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenPetRuleAck(poPlayer);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnOpenSoulUpdateTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenSoulUpdateTypeAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnSoulUpdateTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_SOUL_UPDATE_TYPE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnSoulUpdateTypeAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnOpenNeiDanReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_NEIDAN_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenNeiDanAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnUpdataNeiDanReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_UPDATA_NEIDAN_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnUpdataNeiDanAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnDressNeiDanReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_DRESS_NEIDAN_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnDressNeiDanAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnOpenNeiDanInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnOpenNeiDanInstanceAck(poPlayer);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnNeiDanBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_NEIDAN_BATTLE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnNeiDanBattleAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnRecvNeiDanAwardReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_RECV_NEIDAN_AWARD_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvNeiDanAwardAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}

BOOL CGTProcessor::OnSkipNeiDanBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
	CHECK_ON_REQ(PKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ)
		UINT16 wMsgID = CGTPktBuilder::Instance()->OnSkipNeiDanBattleAck(poPlayer, *pReq);
	SEND_ACK
		return TRUE;
}