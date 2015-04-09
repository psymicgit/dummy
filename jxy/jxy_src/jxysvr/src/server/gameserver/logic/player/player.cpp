
#include "player.h"
#include "playermgr.h"
#include "playergetdatamgr.h"

#include <utility.h>

#include <logic/other/singleprammgr.h>
#include <logic/other/phystrengthmgr.h>
#include <logic/event/timereventmgr.h>
#include <framework/gsapi.h>
#include <net/gt/gtpktbuilder.h>
#include <sdutil.h>
#include <errdef.h>
#include <logic/hero/heromgr.h>
#include <logic/monster/monstermgr.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <logic/event/timereventmgr.h>
#include <framework/gsconfig.h>
#include <framework/gsapi.h>
#include "db/recorplayerloginoff.h"
#include <logic/other/commoninfomgr.h>
#include <logic/activity/bossbattlemgr.h>
#include <logic/chat/chatmgr.h>
#include <logic/base/basedefine.h>
#include <logic/activityencourage/activityencouragemgr.h>
#include <logic/http/httpmgr.h>
#include <logic/http/basehttpcmd.h>
#include <logic/http/namehttpcmd.h>
#include <logic/faction/factionmgr.h>
#include <logic/faction/faction.h>
#include <logic/hero/heropropmgr.h>
#include <logic/jingjie/jingjiepropmgr.h>
#include <net/cli/user.h>
#include <logic/player/playersavemgr.h>
#include <net/cli/usermgr.h>

#include <logic/activity/courage.h>
#include <logic/activity/couragemgr.h>
#include <logic/activity/bossbattle.h>
#include <logic/activity/eatbreadpropmgr.h>
#include <logic/task/taskmgr.h>
#include <logic/enemy/enemymgr.h>
#include <logic/plunder/plundermgr.h>
#include <logic/race/racemgr.h>
#include <logic/build/buildmgr.h>
#include <logic/yabiao/yabiaomgr.h>
#include "logic/build/build.h"
#include "logic/jingjie/jingjie.h"
#include "logic/friendsys/friends.h"
#include "logic/moneytree/moneytree.h"
#include "logic/crop/crop.h"
#include <logic/rareitemroom/rareitemroompropmgr.h>
#include <logic/gymnasium/gymnasiumpropmgr.h>
#include <logic/factionshade/shadepropmgr.h>
#include <logic/hotspring/hotspringpropmgr.h>
#include "logic/other/locale.h"
#include "logic/other/state.h"
#include "logic/climbtower/climbtower.h"
#include <logic/other/equipcompose.h>
#include "logic/other/fewparam.h"
#include "logic/formation/formation.h"
#include "logic/skill/collectgasmgr.h"
#include "logic/moneytree/moneytree.h"
#include "logic/other/locale.h"
#include "logic/other/state.h"
#include "logic/vip/vipextdata.h"
#include "logic/instance/instance.h"
#include "logic/onhook/onhook.h"
#include "logic/friendsys/friends.h"
#include "logic/other/gatherscience.h"
#include <logic/faction/factionplayer.h>
#include "logic/score/score.h"
#include "logic/science/sciencetree.h"
#include "logic/skill/skillpropmgr.h"
#include "logic/begbluegas/begblue.h"
#include "logic/awaken/awaken.h"
#include <logic/encourage/onlineencouragerecord.h>
#include <logic/encourage/loginencouragerecord.h>
#include "logic/activity/luaactivity.h"
#include "logic/activity/eatbread.h"
#include "logic/other/xiangqianmgr.h"
#include "logic/gvg/playergvg.h"
#include "logic/player/playerslogmgr.h"
#include "logic/soul/playersoul.h"
#include "logic/medal/playermedal.h"
#include "logic/soul/soulmanager.h"
#include "logic/pet/playerpet.h"
#include "logic/pet/pet.h"
using namespace SGDP;

IMPLEMENT_CLASS_POOL_MGR(CPlayer)

////////////////////////////////////技能种植////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(Crop, CCrop, DT_CROP_DATA)
////////////////////////////////////爬塔相关////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(ClimbTower, CClimbTower, DT_CLIMB_TOWER_DATA)
////////////////////////////////////装备合成相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(EquipCompse, CEquipCompse, DT_EQUIP_COMPOSE_DATA)
////////////////////////////////////稀少参数相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(FewParam, CFewParam, DT_FEW_PARAM_DATA)
////////////////////////////////////整型相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(Formation, CFormation, DT_NEW_FORMATION_DATA)


////////////////////////////////////其他乱七八糟////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(CollectGas, CCollectGas, DT_COLLECT_GAS_DATA)

IMPLEMENT_BUILD_INIT_DESTORY(MoneyTree, CMoneyTree, DT_MONEY_TREE_DATA)

IMPLEMENT_INIT_DESTORY(OnlineEncourageRecord, COnlineEncourageRecord, DT_ONLINE_ENCOURAGE_RECORD_DATA)

IMPLEMENT_INIT_DESTORY(LoginEncourageRecord, CLoginEncourageRecord, DT_LOGIN_ENCOURAGE_RECORD_DATA)

////////////////////////////////////位置相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(Locale, CLocale, DT_LOCALE_DATA)

////////////////////////////////////状态相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(State, CState, DT_STATE_DATA)

////////////////////////////////////VIP扩展相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(VipExtData, CVipExtData, DT_VIP_EXT_DATA)

////////////////////////////////////副本相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(Instance, CInstance, DT_INSTANCE_DB_DATA)

IMPLEMENT_BUILD_INIT_DESTORY(Onhook, COnhook, DT_ONHOOK_RCD_DATA)

////////////////////////////////////好友系统////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY_EX(Friends, CFriends, DT_FRIENDS_DATA, stFriendsBaseData)

////////////////////////////////////聚魂////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(GatherScience, CGatherScience, DT_GATHER_SCIENCE_DATA)

////////////////////////////////////体力相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(Phystrength, CPhystrength, DT_PHYSTRENGTH_DATA)

////////////////////////////////////评分奖励相关////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(Score, CScore, DT_SCORE_DATA_LST)

////////////////////////////////////押镖相关////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY_EX(YabiaoInfo, CYabiaoInfo, DT_YABIAO_DATA, stYabiaoBaseInfo)

////////////////////////////////////科技树相关////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(ScienceTree, CScienceTree, DT_SCIENCE_TREE_DATA)

//玩家门派数据
IMPLEMENT_BUILD_INIT_DESTORY(FactionPlayer, CFactionPlayer, DT_PLAYER_FACTION_DATA)

////////////////////////////////////境界相关////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(JingJie, CJingJie, DT_JINGJIE_DATA)

////////////////////////////////////求仙露////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(BegBlue, CBegBlue, DT_BEGBLUE_DATA)

////////////////////////////////////冲值奖励相关////////////////////////////////////
IMPLEMENT_INIT_DESTORY(ActivityEncourage, CActivityEncourage, DT_ACTIVITY_ENCOURAGE_DATA)

////////////////////////////////////悟道相关////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(Awaken, CAwaken, DT_AWAKEN_DATA)


IMPLEMENT_INIT_DESTORY(LuaActivity, CLuaActivity, DT_LUAACTIVITY_RECORD_LIST_DATA)
IMPLEMENT_BUILD_INIT_DESTORY(EatBread, CEatBread, DT_EAT_BREAD_DATA)
IMPLEMENT_BUILD_INIT_DESTORY(Courage, CCourage, DT_COURAGE_DATA)
IMPLEMENT_BUILD_INIT_DESTORY_EX(BossBattle, CBossBattle, DT_BOSS_BATTLE_PLAYER_DATA_INFO, stBossBattleData)

////////////////////////////////////装备精通////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY(EquipMaster, CEquipMaster, DT_EQUIP_MASTER)

IMPLEMENT_BUILD_INIT_DESTORY( PlayerGVG, CPlayerGVG, DT_PLAYER_GVG_DATA)
/////////////////////////////////战魂/////////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY( PlayerSoul, CPlayerSoul, DT_PLAYER_SOUL_DATA)
////////////////////////////////勋章//////////////////////////////////////////
IMPLEMENT_BUILD_INIT_DESTORY( PlayerMedal, CPlayerMedal, DT_PLAYER_MEDAL_DATA)
// 宠物
IMPLEMENT_BUILD_INIT_DESTORY(PlayerPet , CPlayerPet, DT_PLAYER_PET_DATA)

CPlayer::CPlayer()
{
    InitmAllGuildFunction();
	memset(&m_stDT_EXCHANGE_RESULT_LOG, 0x00, sizeof(m_stDT_EXCHANGE_RESULT_LOG));
    memset(&m_stDT_PLAYER_TODAY_CONSUME_DATA, 0x00, sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
    memset(&m_stDT_PLAYER_BASE_DATA, 0, sizeof(m_stDT_PLAYER_BASE_DATA));
    m_bLoginFlag = FALSE;
    m_qwUpdateTime = SDTimeSecs();
    m_qwLastDisconnectTime = 0;
    //m_qwTodayFirstLoginTime = 0;
    m_dwPlayerID = 0;
    m_eSaveState = ESS_NON_SAVE;//
    m_bNewPlayerFlag = FALSE;


    m_stBattleEncourage.Clear();

    memset(&m_stLastBattleAck5, 0, sizeof(m_stLastBattleAck5));
    memset(&m_stLastBattleAck3, 0, sizeof(m_stLastBattleAck3));
	memset(&m_stLastBattleAck4, 0, sizeof(m_stLastBattleAck4));
    m_mapKind2Build.Clear();
    m_mapEnemy.Clear();
    m_vecEnemy.clear();

    m_mapKind2Hero.Clear();
    m_mapKind2HeroAllCarry.Clear();
    m_mapFormationHero.Clear();
    m_poCoachHero = NULL;
    memset(&m_stOldCareerBeforeChange, 0, sizeof(m_stOldCareerBeforeChange));

    m_mapEquipKind2Idx.clear();
    m_mapAllEquip.Clear();
    m_vecNewEquip.clear();
    //m_poMinStrengthenLevelEquip = 0;
    //m_poMaxUpgradeLevelEquip = 0;

    m_mapAllTask.Clear();
    m_mapGiveTask.Clear();
    m_mapAcceptTask.Clear();
    m_mapEncourageTask.Clear();
    m_mapChangeTask.Clear();
    m_mapNBIEncourageTask.Clear();
    m_mapFinishTask.Clear();
    m_dwLastAcceptMainLineTaskIdx = NULL;

    m_pstMaintownLevyProp = NULL;

    memset(m_aszOrderID, 0x00, MAX_ORDER_ID_LEN);
    memset(m_abyNoticeID, 0x00, MAX_TOKEN_NUM);

    m_qwLoginTime = 0;
    m_qwLogoffTime = 0;
    m_bAddOnlineTime = FALSE;
    m_bNewPlayerFlag = FALSE;
    m_qwLastInsertTime = 0;



    m_dwCliVer = 0;
    m_poMoneyTree = NULL;
    m_poAwaken = NULL;
    m_poCrop = NULL;
    m_poRace = NULL;
    m_poPlunder = NULL;
    m_poEatBread = NULL;
    m_poJingJie = NULL;
    m_poCourage = NULL;
    m_poFactionPlayer = NULL;
    m_poInstance = NULL;
    m_poOnhook = NULL;
    m_poLocale = NULL;
    m_poState = NULL;
    m_poEquipCompse = NULL;
    m_poPhystrength = NULL;
    m_poFewParam = NULL;
    m_poScienceTree = NULL;
    m_poFormation = NULL;
    m_poYabiaoInfo = NULL;
    m_poClimbTower = NULL;
    m_poWorship = NULL;
    m_poRdChallenge = NULL;
    m_poVipExtData = NULL;
    m_poOnlineEncourageRecord = NULL;
    m_poLoginEncourageRecord = NULL;
    m_poEquipMaster = NULL;
    m_poActivityEncourage = NULL;
    m_poScore = NULL;
    m_poCollectGas = NULL;
    m_poFriends = NULL;
    m_poBegBlue = NULL;
    m_poOnlineEncourageRecord = NULL;
    m_poGatherScience = NULL;
    m_poLuaActivity = NULL;
    m_poBossBattle = NULL;
	m_poPlayerGVG = NULL;
	m_poPlayerMedal = NULL;
	m_poPlayerSoul = NULL;
    m_poPlayerPet = NULL;
}

CPlayer::~CPlayer()
{
    //释放武将
    CHero* poHero = m_mapKind2Hero.GetFistData();
    while(poHero)
    {
        CHeroMgr::Instance()->RemoveHero(m_dwPlayerID, poHero->GetDT_HERO_BASE_DATA().wKindID);

        poHero = m_mapKind2Hero.GetNextData();
    }
    m_mapKind2Hero.Clear();

    //释放建筑
    CBuild* poBuild = m_mapKind2Build.GetFistData();
    while(poBuild)
    {
        CBuildMgr::Instance()->RemoveBuild(m_dwPlayerID, poBuild->GetDT_BUILD_BASE_DATA().byKindID);
        poBuild = m_mapKind2Build.GetNextData();
    }
    m_mapKind2Build.Clear();

    //释放任务
    CTask* poTask = m_mapAllTask.GetFistData();
    while(poTask)
    {
        CTaskMgr::Instance()->RemoveTask(m_dwPlayerID, poTask->GetDT_TASK_DATA().byTaskLine);

        poTask = m_mapAllTask.GetNextData();
    }
    m_mapAllTask.Clear();
    m_mapGiveTask.Clear();
    m_mapAcceptTask.Clear();
    m_mapEncourageTask.Clear();
    m_mapChangeTask.Clear();
    m_mapNBIEncourageTask.Clear();

    //释放道具（所有道具都在武将和背包对象释放了）
    m_mapAllEquip.Clear();
    m_mapEquipKind2Idx.clear();

    //押镖
    BOOL bYabiaoing = FALSE;
    if (m_poYabiaoInfo)
    {
        bYabiaoing = m_poYabiaoInfo->GetYabiaoFlag() == CYabiaoInfo::ms_nYabiaoProcessFlagRunning;
    }

    //仇敌
    CEnemy* poEnemy = m_mapEnemy.GetFistData();
    do
    {
        if(NULL == poEnemy)
        {
            break;
        }
        CEnemyMgr::Instance()->RemoveEnemy(m_dwPlayerID, poEnemy->GetDT_ENEMY_DATA().dwEnemyPlayerID);
    }
    while (poEnemy = m_mapEnemy.GetNextData());
    m_mapEnemy.Clear();
    m_vecEnemy.clear();

    //相关定时事件, 释放俘虏定时器要保留
    CTimerEventMgr::Instance()->DelSaveOnlineEvent(m_dwPlayerID);
    CTimerEventMgr::Instance()->DelSaveCkOfflineEvent(m_dwPlayerID);
    if( bYabiaoing )
    {
        CTimerEventMgr::Instance()->DelYabiaoEvent(m_dwPlayerID);
    }
    CTimerEventMgr::Instance()->DelDrinkWineEvent(m_dwPlayerID);


    //对像数据删除的过程
    for (CDestoryVecItr itr = m_vecDestory.begin(); itr != m_vecDestory.end(); itr++)
    {
        DestoryObj pfun = *itr;
        (this->*pfun)();
    }
    m_dwPlayerID = 0;
}

UINT32 CPlayer::GetPlayerID()
{
    return m_stDT_PLAYER_BASE_DATA.dwID;
}

VOID CPlayer::CkData()
{
    AddTotalAmount(0);
}


BOOL CPlayer::InitDT_PLAYER_TODAY_CONSUME_DATA()
{
    m_stDT_PLAYER_TODAY_CONSUME_DATA.dwID	= GetID();

    CHero *poHero = GetCoachHero();
    if (poHero)
    {
        UINT32 dwUpgradeExperience = 0;
        poHero->GetExperience((UINT32&)m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterExperience, dwUpgradeExperience);
    }
    m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterLevel = GetLevel();
    m_stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel = m_stDT_PLAYER_BASE_DATA.byVipLevel;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterCoin = m_stDT_PLAYER_BASE_DATA.qwCoin;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterGold = m_stDT_PLAYER_BASE_DATA.qwGold;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStory = m_stDT_PLAYER_BASE_DATA.qwStory;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterPhyStrength = m_stDT_PLAYER_BASE_DATA.wPhyStrength;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterScience = m_stDT_PLAYER_BASE_DATA.qwScience;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterBlueGas = m_stDT_PLAYER_BASE_DATA.qwBlueGas;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterPurpleGas = m_stDT_PLAYER_BASE_DATA.qwPurpleGas;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterJingJie = m_stDT_PLAYER_BASE_DATA.qwJingJie;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStudyExp = m_stDT_PLAYER_BASE_DATA.qwStudyExp;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.dwAfterAmount = m_stDT_PLAYER_BASE_DATA.dwTotalAmount;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterMedal = m_stDT_PLAYER_BASE_DATA.qwMedal;
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterTotalRMB = m_stDT_PLAYER_BASE_DATA.qwTotalRMB;
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterDoorsTribute = poFaction->GetDoorsTribute(m_dwPlayerID);
	}
    m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime = SDTimeSecs();
	
    return TRUE;
}

//初始化相关对象
BOOL CPlayer::Init(DT_PLAYER_DATA& stDT_PLAYER_DATA)
{
    memcpy(&m_stDT_PLAYER_BASE_DATA, &(stDT_PLAYER_DATA.stPlayerNoPvpData.stBaseData), sizeof(DT_PLAYER_BASE_DATA));
	memcpy(&m_stDT_EXCHANGE_RESULT_LOG, &(stDT_PLAYER_DATA.stPlayerNoPvpData.stExchangeResultLog), sizeof(DT_EXCHANGE_RESULT_LOG));

    m_dwPlayerID = m_stDT_PLAYER_BASE_DATA.dwID;
    DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData = stDT_PLAYER_DATA.stPlayerNoPvpData;
    DT_PLAYER_PVP_DATA& stPlayerPvpData = stDT_PLAYER_DATA.stPlayerPvpData;

    //////////////////////////////////////////////////////////////////////////
    //同步pvp信息(竟技场的排名及俘虏主) 和 Base信息为主的模块
    const SPlayerBaseData *poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_dwPlayerID);
    if (NULL != poPlayerBaseData)
    {
        stDT_PLAYER_DATA.stPlayerPvpData.stRaceData.stRaceBaseData.dwRank = poPlayerBaseData->dwRank;
        stDT_PLAYER_DATA.stPlayerPvpData.stPlunderData.stPlunderBaseData.dwCaptureOwnerPlayerID = poPlayerBaseData->dwCaptureOwner;
    }
    if(0 != m_stDT_PLAYER_BASE_DATA.qwRecvRankEncTime)//默认值0
    {
        stDT_PLAYER_DATA.stPlayerPvpData.stRaceData.stRaceBaseData.qwRecvEncourageTime = m_stDT_PLAYER_BASE_DATA.qwRecvRankEncTime;
    }
    if(0 != m_stDT_PLAYER_BASE_DATA.qwStudyExp)//默认值0
    {
        stPlayerNoPvpData.stAwakenData.stBaseInfo.qwStudyExp = m_stDT_PLAYER_BASE_DATA.qwStudyExp;
    }
    //////////////////////////////////////////////////////////////////////////

    //初始化建筑,必须先初始化建筑，其他模块根据建筑判断是否开放
    if(!InitBuild(stPlayerNoPvpData.stBuildData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitBuild failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

	//初始化玩家
	if (!InitFactionPlayer(stPlayerNoPvpData.stPlayerFactionData))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitPlayerFaction failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
		return FALSE;
	}

    //初始化副本
    if(!InitInstance(stPlayerNoPvpData.stInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitInstance failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化科技树参数
    if( !InitScienceTree( stPlayerNoPvpData.stScienceTree ) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitScienceTree failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化境界信息
    if (!InitJingJie(stPlayerNoPvpData.stJingJieData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitJingJie failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    // 初始化装备精通，必须在hero之前初始化
    if (!InitEquipMaster(stPlayerNoPvpData.stEquipMasterData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitEquipMaster failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化悟道信息
    if(!InitAwaken(stPlayerNoPvpData.stAwakenData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitAwaken failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

	if ( !InitPlayerSoul(stPlayerNoPvpData.stSoulData))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitPlayerSoul failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
		return FALSE;
	}

    //初始化武将
    if(FALSE == InitHero(stPlayerNoPvpData.stHeroData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitHero failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化背包
    if(!InitBag(stPlayerNoPvpData.stBaseData.byBagOpenNum))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitBag failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化背包装备道具
    if(!InitBagEquipItem(stPlayerNoPvpData.stBagEquipData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitBagEquipItem failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化背包物品道具
    if(!InitBagGoodsItem(stPlayerNoPvpData.stBagGoodsData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitBagGoodsItem failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }
	//初始化背包饰品道具
	if (!InitBagJewelryItem(stPlayerNoPvpData.stBagJewelryInfo))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitBagJewelryItem failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
		return FALSE;
	}

    //初始化任务
    if(!InitTask(stPlayerNoPvpData.stTaskData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitTask failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化竞技
    if(!InitRace(stPlayerPvpData.stRaceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitRace failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化体力
    if(!InitPhystrength(stPlayerNoPvpData.stPhystrengthData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitPhystrength failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化位置信息
    if(!InitLocale(stPlayerNoPvpData.stLocaleData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitLocale failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化状态信息
    if(!InitState(stPlayerNoPvpData.stCurState))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitState failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化在线奖励信息
    if(!InitOnlineEncourageRecord(stPlayerNoPvpData.stOnlineEncRecdData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitOnlineEncourageRecord failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化登录奖励信息
    if(!InitLoginEncourageRecord(stPlayerNoPvpData.stLoginEncRecdData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitLoginEncourageRecord failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化仇敌信息
    if(!InitEnemy(stPlayerPvpData.stEnemyData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitEnemy failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始摇钱树信息
    if(!InitMoneyTree(stPlayerNoPvpData.stMoneyTreeData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitMoneyTree failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化挂机
    if(!InitOnhook(stPlayerNoPvpData.stOnhookRcdData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitOnhook failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化种植
    if(!InitCrop(stPlayerNoPvpData.stCropData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitCrop failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化稀少参数
    if(!InitFewParam(stPlayerNoPvpData.stFewParamData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitFewParam failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化阵型参数
    if(!InitFormation(stPlayerNoPvpData.stFormationInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitFormation failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化吃包子
    if(!InitEatBread(stPlayerNoPvpData.stEatBreadInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitEatBread failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化掠夺信息
    if(!InitPlunder(stPlayerPvpData.stPlunderData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitPlunder failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化押镖信息
    if(!InitYabiaoInfo(stPlayerNoPvpData.stYabiaoInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitYabiao failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化爬塔信息
    if(!InitClimbTower(stPlayerNoPvpData.stClimbTowerData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitClimbTower failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    if( !InitVipExtData( stPlayerNoPvpData.stVipExtData ) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitVipExt failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化脚本活动信息
    if (!InitLuaActivity(stPlayerNoPvpData.stLuaActivityRecordData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitLuaActivity failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化灵气收集信息
    if (!InitCollectGas(stPlayerNoPvpData.stGasCollectData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitGasCollect failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始boss战信息
    if(!InitBossBattle(stPlayerNoPvpData.stBossBattlePlayerDataInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitBossBattle failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始活动奖励信息
    if(!InitActivityEncourage(stPlayerNoPvpData.stActivityEncourageData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitRechargeEncourage failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始随机挑战信息
    if(!InitRdChallenge(stPlayerNoPvpData.stRdChallengeData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitRdChallenge failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始膜拜信息
    if(!InitWorship(stPlayerNoPvpData.stWorshipData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitWorship failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始评分信息
    if(!InitScore(stPlayerNoPvpData.stScoreData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitScore failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //初始化装备合成信息
    if(!InitEquipCompse(stPlayerNoPvpData.stEquipComposeData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitEquipCompse failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    if (!InitGatherScience(stPlayerNoPvpData.stGatherScienceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitGatherScience failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    // 初始化求仙露信息
    if (!InitBegBlue(stPlayerNoPvpData.stBegBlueData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitBegBlue failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    // 初始化好友系统信息

    if (!InitFriends(stPlayerNoPvpData.stFriendsData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitFriendSys failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    // 初始化勇气试炼信息
    if (!InitCourage(stPlayerNoPvpData.stCourageData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitCourage failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

	if ( !InitPlayerGVG(stPlayerNoPvpData.stGvGData))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitPlayerGVG failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
		return FALSE;
	}

	if ( !InitPlayerMedal(stPlayerNoPvpData.stMedalData))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitPlayerMedal failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
		return FALSE;
	}

	if (!InitPlayerPet(stPlayerNoPvpData.stPetData)) {
		SYS_CRITICAL(_SDT("[%s: %d]: InitPlayerPet failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
		return FALSE;
	}


    //重置订单号
    memset(m_aszOrderID, 0x00, MAX_ORDER_ID_LEN);
    memcpy(m_aszOrderID, stPlayerNoPvpData.szLastOrderID, strlen(stPlayerNoPvpData.szLastOrderID));

    //重置通知号
    memset(m_abyNoticeID, 0x00, MAX_TOKEN_NUM);
    memcpy(m_abyNoticeID, stPlayerNoPvpData.abyNoticeID, MAX_TOKEN_NUM);

    //修改上下线时间
    ResetLoginTime(stPlayerNoPvpData.qwLoginTime);
    ResetLogoffTime(stPlayerNoPvpData.qwLogoffTime);


    //初始化消息记录
    memcpy(&m_stDT_PLAYER_TODAY_CONSUME_DATA, &(stDT_PLAYER_DATA.stPlayerNoPvpData.stTodayConsume), sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
    if (!IsToday(m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime))
    {
        CSDDateTime	oDateTime(m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime);
        tstring		strDateTime = SDTimeToString(oDateTime);
        // DBG_INFO( _SDT( "[%s: %d]:player:%d LastConsumeTime:%llu, LastConsumeTime:%s" ), MSG_MARK, m_dwPlayerID, m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime, strDateTime.c_str());

        memset(&m_stDT_PLAYER_TODAY_CONSUME_DATA, 0x00, sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
    }
    //初始化消费记录
    InitDT_PLAYER_TODAY_CONSUME_DATA();

    //检查功能开发
    if(!CkOpenFunc())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CkOpenFunc failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //同步缓存中的数据
    if(!SyncCacheData())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SyncCacheData failed, palyerid[%d]!"), MSG_MARK, m_dwPlayerID);
        return FALSE;
    }

    //属性初始化
    ResetBattleAttr(TRUE);

    //体力回复
    GetPhystrength().PhyStrengthRecover();

    //CkDataFortest();


    CkData();
    return TRUE;
}

CUser*	CPlayer::GetUser()
{
    CUser* poUser = CUserMgr::Instance()->FindUser( GetID() );
    return poUser;
};


BOOL CPlayer::SendMsg(const CHAR* pData, UINT16 wMsgID)
{
    CUser* poUser = CUserMgr::Instance()->FindUser( m_dwPlayerID );

    if( NULL == poUser )
    {
        return FALSE;
    }

    //DBG_INFO( _SDT( "player:%d return msg:%d" ), m_dwPlayerID, wMsgID );

    return poUser->SendMsg(pData, wMsgID);
}

VOID CPlayer::OnLogin(BOOL bLoginFlag)
{
    if(!m_bLoginFlag && bLoginFlag)
    {
        CUserMgr::Instance()->IncConnectedNum();
    }
    else if(m_bLoginFlag && !bLoginFlag)
    {
        CUserMgr::Instance()->DecConnectedNum();
    }
    m_bLoginFlag = bLoginFlag;
	
    m_qwUpdateTime = SDTimeSecs();

    if(bLoginFlag)
    {
        //已从非登录变成登录，则清除该非登录保存事件
        CTimerEventMgr::Instance()->DelSaveCkOfflineEvent(m_dwPlayerID);
        //添加保存登录玩家信息事件
        CTimerEventMgr::Instance()->AddSaveOnlineEvent(m_dwPlayerID, CGSConfig::Instance()->GetOnlineSaveTimeInterval());

        //不是今天,记录今天第一次登录时间
        if(!IsToday(m_stDT_PLAYER_BASE_DATA.qwTodayFirstLoginTime))
        {
            m_stDT_PLAYER_BASE_DATA.qwTodayFirstLoginTime = SDTimeSecs();
            //当天第一次登录，是否弹出VIP面板提示信息
			set_bit(m_stDT_PLAYER_BASE_DATA.dwExtData, em_VIP);
			CPlayerMedal* poPlayerMedal = GetPlayerMedal();
			if ( NULL != poPlayerMedal)
			{
				poPlayerMedal->ContinueLogin();
				poPlayerMedal->CheckMedalByLogin();
				ResetBattleAttr(TRUE);
			}

        }
    }
    else
    {
        //已从登录变成非登录，则清除该登录保存事件
        CTimerEventMgr::Instance()->DelSaveOnlineEvent(m_dwPlayerID);
        //添加保存非登录玩家信息事件
        CTimerEventMgr::Instance()->AddSaveCkOfflineEvent(m_dwPlayerID, CGSConfig::Instance()->GetOfflineSaveCkInterval());
    }
    DBG_INFO( _SDT( "Player[ %d : %s ] login state = %s. Power = %u" ), m_dwPlayerID, GetDT_PLAYER_BASE_DATA().aszDispName, bLoginFlag ? "online" : "offline", m_stDT_PLAYER_BASE_DATA.dwPower);
}

VOID CPlayer::StartBattle(CBattleObj* poEnemy)
{
    m_poEnemy = poEnemy;

    ResetBattleRadom();
    InitCurBattleAttr();
    ResetSpecialEffects();
    //状态回调
    GetState().OnStartBattle();

	CPet* poPet = GetBattlePet();
	if ( NULL != poPet)
	{
		poPet->StartPetBattle();
	}
	SetPetBattleState( FALSE );
}

CPet* CPlayer::GetBattlePet()
{
	CPlayerPet* poPlayerPet = GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return NULL;
	}
	return const_cast<CPet*>(poPlayerPet->GetFightPet());
}

UINT16 CPlayer::EnterInstance(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byNewFlag)
{
    SInstanceProp* poInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(NULL == poInstanceProp)
    {
        return ERR_ENTER_INSTANCE::ID_INDEX_ERR;//
    }

    //先回复体力(体力由相关网络请求触发，非定时器，则先尝试回复体力)
    GetPhystrength().PhyStrengthRecover();//

	UINT16 wErrCode = m_poInstance->CkCanEnter(wSceneIdx, byTownIdx, byInstanceIdx, byNewFlag);
	if(ERR_ENTER_INSTANCE::ID_SUCCESS != wErrCode)
	{
		return wErrCode;//
	}
    //重新开启副本,则先退出之前副本
    if(1 == byNewFlag)
    {
        ExitInstance();
    }

    //当前已经在其他的副本中
    SCurBattleInstance& stCurBattleInstance = m_poInstance->GetCurBattleInstance();
    BOOL bLastInstance = FALSE;
    if((stCurBattleInstance.wSceneIdx != NONINSTANCE) && ( wSceneIdx != stCurBattleInstance.wSceneIdx) &&
            ( byTownIdx != stCurBattleInstance.byTownIdx) && ( byInstanceIdx != stCurBattleInstance.byInstanceIdx) )
    {
        return ERR_ENTER_INSTANCE::ID_REENTER_ERR;//
    }
    if((stCurBattleInstance.wSceneIdx != NONINSTANCE) && ( wSceneIdx == stCurBattleInstance.wSceneIdx) &&
            ( byTownIdx == stCurBattleInstance.byTownIdx) && ( byInstanceIdx == stCurBattleInstance.byInstanceIdx) )
    {
        bLastInstance = TRUE;
    }

    //已经不是上次的副本，则新开启副本
    if(!bLastInstance)
    {
        byNewFlag = TRUE;
    }

    //判断进入副本体力
    //上该副本是上次进入的副本和未解锁家园/精英副本，则不扣除体力
    if((!bLastInstance) && (NULL != GetBuild(EBK_MAINTOWN)) && (EIT_COMMON == wSceneIdx))
    {
        if(m_stDT_PLAYER_BASE_DATA.wPhyStrength < CPhystrengthMgr::Instance()->GetCommonPhyStrength())
        {
            return ERR_ENTER_INSTANCE::ID_PHYSTRENGTH_ERR;
        }
    }

  

    //重新当前血量
    m_stCurBattleAttribute = m_stMaxBattleAttribute;

    if(!m_poInstance->EnterInstance(wSceneIdx, byTownIdx, byInstanceIdx))
    {
        RETURN_OTHER_ERR;
    }

    //体力在第一波怪扣除

    //不是上次副本，更新状态
    if(!bLastInstance)
    {
        GetState().OnEnterNotLastInstance(m_stMaxBattleAttribute.qwHP);
    }

    return ERR_ENTER_INSTANCE::ID_SUCCESS;

}

//每次战斗后(清除战斗过程中的产出 buff等)
VOID CPlayer::EndBattle()
{
    ReleaseBuff();
	ReleaseSpecialEffect();
    //状态回调
    m_poState->OnEndBattle();
}


//每次退出副本前的各种属性计算(清除为处理的悬挂物等等)
VOID CPlayer::ExitInstance()
{
    m_poInstance->ExitInstance();

    m_poState->OnExitInstance(m_stMaxBattleAttribute.qwHP);
}

VOID CPlayer::GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA)
{
    CHero* poHero = m_mapFormationHero.GetFistData();
    do
    {
        if(NULL == poHero)
        {
            break;
        }
        if(EHP_BATTLE == poHero->GetDT_HERO_BASE_DATA().byPos)
        {
            if(poHero->IsDead())
            {
                // 门派跨服战中的伙伴可能初始就是死亡的，此时不应让其上阵
                continue;
            }

            DT_BATTLE_OBJ_DATA2* pstDT_BATTLE_OBJ_DATA = &(stDT_BATTLE_OBJ_GROUP_DATA.astBattleObjInfo[stDT_BATTLE_OBJ_GROUP_DATA.byObjNum++]);
            DT_HERO_BASE_DATA& stDT_HERO_BASE_DATA2 = poHero->GetDT_HERO_BASE_DATA();
            pstDT_BATTLE_OBJ_DATA->wKindID = stDT_HERO_BASE_DATA2.wKindID;
            pstDT_BATTLE_OBJ_DATA->wLevel = stDT_HERO_BASE_DATA2.wLevel;
            pstDT_BATTLE_OBJ_DATA->byCareerID = poHero->GetHeroProp()->byCareerID;
            pstDT_BATTLE_OBJ_DATA->byFomationIdx = stDT_HERO_BASE_DATA2.byFormationIdx;

            DT_BATTLE_ATTRIBUTE* pstBattleAttr = &(pstDT_BATTLE_OBJ_DATA->stBattleAttr);
            SHeroUpgradeProp* pstHeroUpgradeProp = poHero->GetHeroUpgradeProp();
            pstBattleAttr->dwHP = (UINT32)poHero->GetCurDT_BATTLE_ATTRIBUTE().qwHP;
            pstBattleAttr->dwAttack = poHero->GetDT_BATTLE_ATTRIBUTE().dwAttack;

            CSkill* poSkill = poHero->GetFormationSkill();
            if(poSkill)
            {
                pstDT_BATTLE_OBJ_DATA->wSkillActionID = poSkill->GetDT_SKILL_DATA().wActionID;
                pstDT_BATTLE_OBJ_DATA->wSkillActionLevel = poSkill->GetDT_SKILL_DATA().wActionLevel;
            }
            else
            {
                pstDT_BATTLE_OBJ_DATA->wSkillActionID = 0;
                pstDT_BATTLE_OBJ_DATA->wSkillActionLevel = 0;
            }
            pstDT_BATTLE_OBJ_DATA->wNeedAngry = FULL_ANGRY;
            pstDT_BATTLE_OBJ_DATA->wInitAngry = poHero->GetDT_HIDE_BATTLE_ATTRIBUTE().wAngryValue;

            //主角有颜色
            if (GetCoachHeroKindID() == poHero->GetHeroID())
            {
                UINT16 wLevel = 0;
                if (NULL != m_poJingJie)
                {
                    wLevel = m_poJingJie->GetDT_JINGJIE_DATA().stBaseData.wLevel;
                }
                pstDT_BATTLE_OBJ_DATA->byColor = (UINT8)CJingJiePropMgr::Instance()->GetJingJieColor(wLevel);
            }
            else
            {
                pstDT_BATTLE_OBJ_DATA->byColor = 0;
            }
            poHero->GetDT_GODWEAPON_BATTLE_DATA(pstDT_BATTLE_OBJ_DATA->stGodweaponInfo);
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s: %d]: poHero pos is invalid!playerid[%d],heroid[%d]"), MSG_MARK, m_dwPlayerID, poHero->GetDT_HERO_BASE_DATA().wKindID);
        }

    }
    while(poHero = m_mapFormationHero.GetNextData());
	CPet* poPet = GetBattlePet();
	if ( NULL != poPet)
	{
		stDT_BATTLE_OBJ_GROUP_DATA.stPetInfo.wPetID = poPet->GetDT_PET_DATA().wPetCfgId;
		stDT_BATTLE_OBJ_GROUP_DATA.stPetInfo.wPetLevel = poPet->GetDT_PET_DATA().wLv;
		stDT_BATTLE_OBJ_GROUP_DATA.stPetInfo.byPetQuality = poPet->GetDT_PET_DATA().byStarLv;
	}
}

VOID CPlayer::GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA)//赋值战斗组战斗者属性
{
    memset(&stDT_BATTLE_OBJ_GROUP_AFTER_DATA, 0, sizeof(stDT_BATTLE_OBJ_GROUP_AFTER_DATA));

    CHero* poHero = m_mapFormationHero.GetFistData();
    do
    {
        if(NULL == poHero)
        {
            break;
        }
        
        if(EHP_BATTLE == poHero->GetDT_HERO_BASE_DATA().byPos)
        {
            DT_BATTLE_OBJ_HP &stHP = stDT_BATTLE_OBJ_GROUP_AFTER_DATA.astBattleObjInfo[stDT_BATTLE_OBJ_GROUP_AFTER_DATA.byObjNum++];
            stHP.dwHP           = (UINT32)poHero->GetCurDT_BATTLE_ATTRIBUTE().qwHP;
            stHP.byFormationIdx = poHero->GetDT_HERO_BASE_DATA().byFormationIdx;
        }
    }
    while(poHero = m_mapFormationHero.GetNextData());
}

BOOL CPlayer::GetDT_PLAYER_BASE_DATA_CLI(DT_PLAYER_BASE_DATA_CLI& stPlayerInfo)
{
    stPlayerInfo.dwID = m_dwPlayerID;
    _SDTStrcpy(stPlayerInfo.aszDispName, m_stDT_PLAYER_BASE_DATA.aszDispName);
    stPlayerInfo.qwCoin = m_stDT_PLAYER_BASE_DATA.qwCoin;
    stPlayerInfo.qwGold = m_stDT_PLAYER_BASE_DATA.qwGold;
    stPlayerInfo.qwScience = m_stDT_PLAYER_BASE_DATA.qwScience;
    stPlayerInfo.dwPower = m_stDT_PLAYER_BASE_DATA.dwPower;
    memcpy(stPlayerInfo.abyGuide, m_stDT_PLAYER_BASE_DATA.stGuideRecordData.abyGuideRecordData, MAX_GUIDE_NUM);
    stPlayerInfo.byVipLevel = m_stDT_PLAYER_BASE_DATA.byVipLevel;

    if(NULL == m_poCoachHero)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPlayer::GetDT_PLAYER_BASE_DATA_CLI m_poCoachHero is NULL!"), MSG_MARK);
        return FALSE;
    }
    stPlayerInfo.wLevel = m_poCoachHero->GetDT_HERO_BASE_DATA().wLevel;
    m_poCoachHero->GetExperienceInfo(stPlayerInfo.stExperienceInfo);

    //获取体力信息
    if(!GetPhystrength().GetPhyStrengthInfo(stPlayerInfo.stPhyStrengthInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPlayer::GetDT_PLAYER_BASE_DATA_CLI GetPhyStrengthInfo failed!"), MSG_MARK);
        return FALSE;
    }

	DT_LOCALE_DATA& stLocaleData = GetLocale().GetDT_LOCALE_DATA();

	if( EPL_HOT_SPRING == stLocaleData.byLocaleType || EPL_FACTION == stLocaleData.byLocaleType)
	{
		CBuild* poBuild = GetBuild(EBK_FACTION);
		if ( NULL == poBuild )
		{
			GetLocale().OnEnterHome();
		}
		else
		{
			CFaction* poFaction = GetFaction();
			if ( NULL == poFaction)
			{
				GetLocale().OnEnterHome();
			}
		}
	}
	else if(EPL_INSTANCE == stLocaleData.byLocaleType)
	{
		const SInstanceProp* poInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stLocaleData.stLocaleInfo.stInstance.wSceneIdx, stLocaleData.stLocaleInfo.stInstance.byTownIdx, stLocaleData.stLocaleInfo.stInstance.byInstanceIdx );
		if(NULL == poInstanceProp)
		{
			GetLocale().OnEnterHome();
		}
	}

    //当前位置
    memcpy(&(stPlayerInfo.stLocaleData), &(GetLocale().GetDT_LOCALE_DATA()), sizeof(DT_LOCALE_DATA));
    return TRUE;
}

//注册昵称，并通知LS更新User表
UINT16 CPlayer::RegisterDspName(CHAR* pszDspName)
{
    //只能注册一次

    BOOL bReRegDspName = (get_bit(GetExtData(), em_EDIT_NAME));
    if(0 != m_stDT_PLAYER_BASE_DATA.aszDispName[0])
    {
        // 检测玩家是否需要重新注册昵称
        if(FALSE == bReRegDspName)
        {
            //return ERR_REGISTER_DSPNAME::ID_HAVE_DONE;
            return ERR_REGISTER_DSPNAME::ID_SUCCESS; //为了客户端容错处理
        }
    }

    //检查各个参数是否有sql嵌入攻击
    if(HaveSqlToken(pszDspName))
    {
        return ERR_REGISTER_DSPNAME::ID_INVALID_CHARACTER;
    }

    //检测是否有特殊字符，且长度合法(昵称允许6个中文字符，12个英文)
    if(CkHaveSpeCharacterAndValidLen(pszDspName))
    {
        return ERR_REGISTER_DSPNAME::ID_INVALID_CHARACTER;
    }

    //检查是否有敏感词
    if(CChatMgr::Instance()->DspNameHaveSensitiveWord(this, pszDspName))
    {
        return ERR_REGISTER_DSPNAME::ID_SENSITIVE_WORD;
    }

    UINT16 wErrCode = CPlayerBaseDataCacheMgr::Instance()->RegDspName(m_dwPlayerID, pszDspName);
    if(ERR_REGISTER_DSPNAME::ID_SUCCESS != wErrCode)
    {
        return wErrCode;
    }
    
    //通知LS更新User表,注意别用CGTPktBuilder方法
    PKT_GSLS_DSPNAME_UPGRADE_NTF stNtf;
    memset(&stNtf, 0, sizeof(stNtf));
    stNtf.dwPlayerID = m_dwPlayerID;
    SDStrncpy(stNtf.aszDspName, pszDspName, USERNAME_LEN - 1);
    SendMsg((CHAR*)(&stNtf), GSLS_DSPNAME_UPGRADE_NTF);

    SNameInfo stNameInfo;
    stNameInfo.dwPlayerID = m_dwPlayerID;
    stNameInfo.strName    = pszDspName;
    stNameInfo.strOldName = m_stDT_PLAYER_BASE_DATA.aszDispName;
	stNameInfo.byAuthType = GetAuthType();
    SDStrncpy(m_stDT_PLAYER_BASE_DATA.aszDispName, pszDspName, USERNAME_LEN - 1);

    if(FALSE == bReRegDspName)
    {
        m_poState->GetDT_STATE_DATA().qwRegisterDispNameTime = SDTimeSecs();
    }

	CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_NAME);
	if (NULL == poHttpCmd)
	{
		SYS_CRITICAL( _SDT("[%s:%d] player:%u in memory"), MSG_MARK, m_dwPlayerID );
		return ERR_REGISTER_DSPNAME::ID_SUCCESS;
	}

	poHttpCmd->SetUserData(&stNameInfo, sizeof(SNameInfo), 0);
	poHttpCmd->Init();
	if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
	{
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        SYS_CRITICAL( _SDT("[%s:%d] can't AddCommand player:%u in memory"), MSG_MARK, stNameInfo.dwPlayerID );
        return ERR_REGISTER_DSPNAME::ID_SUCCESS;
    }
    return ERR_REGISTER_DSPNAME::ID_SUCCESS;
}
VOID CPlayer::ChangeNameByGM(TCHAR* aszDisName)
{
	if ( NULL != aszDisName)
	{
		SDStrncpy(m_stDT_PLAYER_BASE_DATA.aszDispName, aszDisName, USERNAME_LEN - 1);
	}
	 
}

//同步缓存中的数据
BOOL CPlayer::SyncCacheData()
{
    const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_dwPlayerID);
    if(NULL == pstPlayerBaseData)
    {
        //异常,只有在reset功能时才会发生，跳过
        return TRUE;
    }

    BOOL bPlunderChange = FALSE;
    BOOL bRaceChange = FALSE;

    //赋值俘虏收益
    SSyncProp& stProp = (const_cast<SPlayerBaseData*>(pstPlayerBaseData))->stSyncProp;
    //掠夺相关
    if(m_poPlunder)
    {
        //赋值俘虏主信息
        if(m_poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID != pstPlayerBaseData->dwCaptureOwner)
        {
            m_poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID = pstPlayerBaseData->dwCaptureOwner;
            bPlunderChange = TRUE;
        }

        vector<DT_PLUNDER_BATTLELOG_DATA>& vecPlunderLog = stProp.vecPlunderLog;
        UINT8 bySize = vecPlunderLog.size();
        UINT8 byIdx = 0;
        for(byIdx = 0; byIdx < bySize; byIdx++)
        {
            if(0 != vecPlunderLog[byIdx].qwPlunderTime)
            {
                m_poPlunder->AddBattleLog(&vecPlunderLog[byIdx]);
                bPlunderChange = TRUE;
            }
        }
        vector<UINT32>& vecReleaseCaptureID = stProp.vecReleaseCaptureID;
        bySize = vecReleaseCaptureID.size();
        for(byIdx = 0; byIdx < bySize; byIdx++)
        {
            UINT32 dwCaptureID = vecReleaseCaptureID[byIdx];

            //定时器到期。保存隔子信息
            UINT32 dwCoin = 0;
            UINT32 dwScience = 0;
            CPlunderMgr::Instance()->GetLeftIncome(this, dwCaptureID, dwCoin, dwScience);

            m_poPlunder->DelCapture(dwCaptureID);
            CPlayerSaveMgr::Instance()->AddPlayerPlunderSaveData(CPlayerMgr::Instance()->FindPlayer(dwCaptureID), this);
            bPlunderChange = TRUE;
        }

    }
    stProp.Clr(); //清0

    //修改排名
    if(m_poRace)
    {
        if(m_poRace->GetDT_RACE_BASE_DATA().dwRank != pstPlayerBaseData->dwRank)
        {
            m_poRace->GetDT_RACE_BASE_DATA().dwRank = pstPlayerBaseData->dwRank;
            bRaceChange = TRUE;
        }
    }
    //pvp数据
    if(bPlunderChange)
    {
        CPlayerSaveMgr::Instance()->AddPlayerPlunderSaveData(this);
    }
    if(bRaceChange)
    {
        CPlayerSaveMgr::Instance()->AddPlayerRaceSaveData(this);
    }

    return TRUE;
}


//检查开发功能是否完整，不完整则补全
BOOL CPlayer::CkOpenFunc()
{
    //机器人，检查建筑对应的功能模块
    if(m_stDT_PLAYER_BASE_DATA.byRobotFunc)
    {
        CBuild* poBuild = m_mapKind2Build.GetFistData();
        while(poBuild)
        {
            //检测是否存在相关功能模块及引导，若不存在则添加
            if(!poBuild->AddFunctionModuleAndGuide())
            {
                SYS_CRITICAL(_SDT("[%s: %d]: Build[%d] AddFunctionModuleAndGuide failed !"), MSG_MARK, poBuild->GetDT_BUILD_BASE_DATA().byKindID);
                return NULL;
            }
            poBuild = m_mapKind2Build.GetNextData();
        }
    }

    DT_INSTANCE_ID& stMaxInstance = m_poInstance->GetMaxCommonInstanceID();
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stMaxInstance.wSceneIdx, stMaxInstance.byTownIdx, stMaxInstance.byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        //新玩家副本信息都为0
        return TRUE;
    }
    C8Set& setOpenBuild = pstInstanceProp->stInstanceOpenFunc.setOpenBuild;
    for(C8SetItr itr  = setOpenBuild.begin() ; itr != setOpenBuild.end(); itr++)
    {
        UINT8 byBuildKind = *itr;
        ////新增开发建筑或原数据遗失，则补全
        CBuild* poBuild = GetBuild((EBuildKind)(byBuildKind));
        if(NULL == poBuild)
        {
            NewBuild(byBuildKind);
        }
        //建筑没有遗失，检查建筑对应的功能模块
        else
        {
            //检测是否存在相关功能模块及引导，若不存在则添加
            if(!poBuild->AddFunctionModuleAndGuide())
            {
                SYS_CRITICAL(_SDT("[%s: %d]: Build[%d] AddFunctionModuleAndGuide failed !"), MSG_MARK, byBuildKind);
                CBuildMgr::Instance()->RemoveBuild(m_dwPlayerID, byBuildKind);
                return NULL;
            }
        }
    }
    vector<UINT16>& vecOpenHero = pstInstanceProp->stInstanceOpenFunc.vecOpenHero;
    for(vector<UINT16>::iterator itr  = vecOpenHero.begin() ; itr != vecOpenHero.end(); itr++)
    {
        //新增开放伙伴或原数据遗失，则补全
        if(NULL == GetHero(*itr))
        {
            NewHero(*itr);
        }
    }

	CInstanceRecordShmemMap& mapOutBound = GetInstance().GetOutBoundInstanceRecord();
	CInstanceRecord* poOutBoundRec = mapOutBound.GetFistData();
	while(poOutBoundRec)
	{
		SInstanceProp* poOutBoundProp = poOutBoundRec->GetInstanceProp();
		if ( poOutBoundProp )
		{
			C8Set& setOpenBuild = poOutBoundProp->stInstanceOpenFunc.setOpenBuild;
			for(C8SetItr itr  = setOpenBuild.begin() ; itr != setOpenBuild.end(); itr++)
			{
				UINT8 byBuildKind = *itr;
				////新增开发建筑或原数据遗失，则补全
				CBuild* poBuild = GetBuild((EBuildKind)(byBuildKind));
				if(NULL == poBuild)
				{
					NewBuild(byBuildKind);
				}
				//建筑没有遗失，检查建筑对应的功能模块
				else
				{
					//检测是否存在相关功能模块及引导，若不存在则添加
					if(!poBuild->AddFunctionModuleAndGuide())
					{
						SYS_CRITICAL(_SDT("[%s: %d]: Build[%d] AddFunctionModuleAndGuide failed !"), MSG_MARK, byBuildKind);
						CBuildMgr::Instance()->RemoveBuild(m_dwPlayerID, byBuildKind);
						return NULL;
					}
				}
			}
			vector<UINT16>& vecOpenHero = poOutBoundProp->stInstanceOpenFunc.vecOpenHero;
			for(vector<UINT16>::iterator itr  = vecOpenHero.begin() ; itr != vecOpenHero.end(); itr++)
			{
				//新增开放伙伴或原数据遗失，则补全
				if(NULL == GetHero(*itr))
				{
					NewHero(*itr);
				}
			}
		}
		poOutBoundRec = mapOutBound.GetNextData();
	}
    return TRUE;
}

VOID CPlayer::RecordPlayerOnOffLine( BOOL bLogin )
{
    SPlayerLogin stLogin;
    stLogin.bLogin = bLogin;
    stLogin.dwPlayerID = m_dwPlayerID;
    CUser *poUser = GetUser();
    if (NULL != poUser)
    {
        stLogin.strIP = SDInetNtoa(poUser->GetLoginIP());
    }
    gsapi::GetDBMgr()->AddRecordData( EDBCT_LOGINOFFINFO, &stLogin );
}

void CPlayer::ResetLoginTime(UINT64 qwLoginTime)
{
    m_qwLoginTime = qwLoginTime;
    m_bAddOnlineTime = FALSE;
}

void CPlayer::ResetLogoffTime(UINT64 qwLogoffTime)
{
    m_qwLogoffTime = qwLogoffTime;
    m_bAddOnlineTime = TRUE;
}

VOID CPlayer::GetCliVerInfo(UINT32& dwCliVer, UINT32& dwResVer, UINT32& dwUIVer, TCHAR aszResVer[MAX_RES_VER_LEN])
{
    dwCliVer = 0;
    dwResVer = 0;
    dwUIVer = 0;

    CUser* poUser = GetUser();
    if(poUser)
    {
        dwCliVer = poUser->GetCliVer();
        CCommonInfoMgr::Instance()->GetCliVerInfo(poUser->GetAuthType(), dwCliVer, dwResVer, dwUIVer, aszResVer);
    }
}

VOID CPlayer::GetDT_EXT_FUNC_PARAM_LST(DT_EXT_FUNC_PARAM_LST& stExtFuncParamLst)
{
    memset(&stExtFuncParamLst, 0, sizeof(stExtFuncParamLst));
    DT_EXT_FUNC_PARAM& stDT_EXT_FUNC_PARAM = stExtFuncParamLst.astExtFuncParaInfo[stExtFuncParamLst.byFuncNum++];
    stDT_EXT_FUNC_PARAM.byParam1 = EEGSPT_MAX_PLAYER_LEVEL;
    stDT_EXT_FUNC_PARAM.wParam2 = CSinglePramMgr::Instance()->GetMaxPlayerLevel();

	DT_EXT_FUNC_PARAM& stDT_EXT_FUNC_PARAM2 = stExtFuncParamLst.astExtFuncParaInfo[stExtFuncParamLst.byFuncNum++];
	stDT_EXT_FUNC_PARAM2.byParam1 = EEGSPT_RECHARGE;
	stDT_EXT_FUNC_PARAM2.wParam2 = CSinglePramMgr::Instance()->GetRechargeFlag();

	DT_EXT_FUNC_PARAM& stDT_EXT_FUNC_PARAM3 = stExtFuncParamLst.astExtFuncParaInfo[stExtFuncParamLst.byFuncNum++];
	stDT_EXT_FUNC_PARAM3.byParam1 = EEGSPT_ADVERTISEMENT;
	stDT_EXT_FUNC_PARAM3.wParam2 = CSinglePramMgr::Instance()->GetAdvertisementFlag();

}

UINT32 CPlayer::GetCliVer()
{
    CUser* poUser = GetUser();

    return poUser == NULL ? 305 : poUser->GetCliVer();
}

UINT32 CPlayer::GetAuthType()
{
    CUser* poUser = GetUser();
    return poUser == NULL ? AUTH_TYPE_MAXNET : poUser->GetAuthType();
}

UINT32 CPlayer:: GetDaiChong()
{
	CUser* poUser = GetUser();
	return poUser == NULL ? 0 : poUser->GetDaiChong();
}

UINT32 CPlayer::GetFactionID()
{
    UINT32 dwFactionID = 0;
    BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_dwPlayerID);
    if (bRet)
    {
        return dwFactionID;
    }
    return 0;
}

DT_PLAYER_POS_INFO* CPlayer::GetPositionHotSpring()
{
    CFactionPlayer* poFactionPlayer = GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        return NULL;
    }
    return &poFactionPlayer->GetDT_HOT_SPRING_DATA().stPlayerPosInfo;
}

CFaction* CPlayer::GetFaction()
{
    UINT32 dwFactionID = GetFactionID();
    if (0 == dwFactionID)
    {
        return NULL;
    }
    return CFactionMgr::Instance()->GetFaction(dwFactionID);
}


VOID CPlayer::AddReward(UINT8 byRewardType, UINT32 dwRewardValue, UINT16 wOpType )
{

    switch (byRewardType)
    {
    case ACTE_GOOD:
    {
        UINT16 wPileNum = 1;//物品个数
        ECreateItemRet Ret; //结果
        CItemMgr::Instance()->CreateItemIntoBag(this, dwRewardValue, wPileNum, Ret, 0, (CRecordMgr::EAddItemType)wOpType);
    }
    break;
    case ACTE_COIN:
    {
        AddCoin(dwRewardValue, wOpType);
    }
    break;
    case ACTE_GOLD:
    {
        AddGold( dwRewardValue, wOpType);
    }
    break;
    case ACTE_PHYSTRENGTH:
    {
        AddPhyStrength(dwRewardValue, wOpType);
    }
    break;
    case ACTE_SCIENCE:
    {
        AddScience(dwRewardValue, wOpType);
    }
    break;
    case ACTE_STORY:
    {
        AddStory(dwRewardValue, wOpType);
    }
    break;
    case ACTE_BLUEGAS:
    {
        AddBlueGas(dwRewardValue, wOpType);
    }
    break;
    case ACTE_PRUPLEGAS:
    {
        AddPurpleGas(dwRewardValue, wOpType);
    }
    break;
    case ACTE_JINGJIE:
    {
        AddJingJie(dwRewardValue, wOpType);
    }
    break;
    case ACTE_DoorsTribute:
    {
        CFaction* poFaction = GetFaction();
        if (NULL == poFaction)
        {
            return ;
        }
        poFaction->AddDoorsTribute(m_dwPlayerID, dwRewardValue, wOpType);
    }
    break;
    case ACTE_FactionFunds:
    {
        CFaction* poFaction = GetFaction();
        if (NULL == poFaction)
        {
            return ;
        }
        poFaction->AddFactionFunds(dwRewardValue, wOpType);
    }
    break;
	case ACTE_SOUL:
	{
		CPlayerSoul* pPlayerSoul = GetPlayerSoul();
		if (NULL != pPlayerSoul) {
			CSoul* pSoul = CSoulManager::Instance()->GenerateSoulById(dwRewardValue);
			if (NULL == pSoul) {
				return;
			}
			for (size_t n = 0; n < wOpType; ++ n) {
				if (!pPlayerSoul->AddSoulToBag(pSoul)) {
					break;
				}
			}
		}
	}
	break;
    default:
        break;
    }
}

BOOL CPlayer::CheckRes(UINT8 byResType, UINT64 qwResValue)
{
	UINT64 qwPlayerRes = 0;
	switch (byResType)
	{
	case ACTE_COIN:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.qwCoin;
		}
		break;
	case ACTE_GOLD:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.qwGold;
		}
		break;
	case ACTE_PHYSTRENGTH:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.wPhyStrength;
		}
		break;
	case ACTE_SCIENCE:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.qwScience;
		}
		break;
	case ACTE_STORY:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.qwStory;
		}
		break;
	case ACTE_BLUEGAS:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.qwBlueGas;
		}
		break;
	case ACTE_PRUPLEGAS:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.qwPurpleGas;
		}
		break;
	case ACTE_JINGJIE:
		{
			qwPlayerRes = m_stDT_PLAYER_BASE_DATA.qwJingJie;
		}
		break;
	case ACTE_DoorsTribute:
		{
			CFaction* poFaction = GetFaction();
			if (NULL == poFaction)
			{
				return FALSE;
			}
			qwPlayerRes = poFaction->GetDoorsTribute(GetID());
		}
		break;
	default:
		break;
	}
	if ( qwPlayerRes < qwResValue)
	{
		return FALSE;
	}
	return TRUE;
}

UINT64 CPlayer::GetSyncReward(UINT8 byRewardType)
{
    switch (byRewardType)
    {
    case ACTE_COIN:
    {
        return m_stDT_PLAYER_BASE_DATA.qwCoin;
    }
    break;
    case ACTE_GOLD:
    {
        return m_stDT_PLAYER_BASE_DATA.qwGold;
    }
    break;
    case ACTE_PHYSTRENGTH:
    {
        return m_stDT_PLAYER_BASE_DATA.wPhyStrength;
    }
    break;
    case ACTE_SCIENCE:
    {
        return m_stDT_PLAYER_BASE_DATA.qwScience;
    }
    break;
    case ACTE_STORY:
    {
        return m_stDT_PLAYER_BASE_DATA.qwStory;
    }
    break;
    case ACTE_BLUEGAS:
    {
        return m_stDT_PLAYER_BASE_DATA.qwBlueGas;
    }
    break;
    case ACTE_PRUPLEGAS:
    {
        return m_stDT_PLAYER_BASE_DATA.qwPurpleGas;
    }
    break;
    case ACTE_JINGJIE:
    {
        return m_stDT_PLAYER_BASE_DATA.qwJingJie;
    }
    break;
    case ACTE_DoorsTribute:
    {
        CFaction* poFaction = GetFaction();
        if (NULL == poFaction)
        {
            return 0;
        }
        DT_FACTION_PLAYER_CLI stFactionPlayerCli;
        poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_dwPlayerID);
        return stFactionPlayerCli.dwDoorsTributeValues;
    }
    break;
    case ACTE_FactionFunds:
    {
        CFaction* poFaction = GetFaction();
        if (NULL == poFaction)
        {
            return 0;
        }
        return poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
    }
    break;
    case ACTE_FactionTaskIntegral:
    {
        CFaction* poFaction = GetFaction();
        if (NULL == poFaction)
        {
            return 0;
        }
        return poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule.dwCurIntegral;
    }
    break;
    default:
        break;
    }
    return 0;
}

UINT32 CPlayer::GetFactionID(UINT32 dwPlayerID)
{
    UINT32 dwFaction = 0;
    BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
    if (bRet)
    {
        return dwFaction;
    }
    return 0;
}

BYTE CPlayer::GetFactionIconID()
{
    UINT32 dwFactionID = GetFactionID();
    CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
    if(NULL == poFaction)
    {
        return NULL_FACTION_ICON_ID;
    }

    return poFaction->GetFactionIconID();
}

UINT16 CPlayer::GetCoachDefaultSkillActionID()
{
    if(m_poCoachHero)
    {
        SHeroProp* pstHeroProp = m_poCoachHero->GetHeroProp();
        if(pstHeroProp)
        {
            return pstHeroProp->wSkillActionID;
        }
    }

    //异常
    return 0;
}


CSkill* CPlayer::GetCoachSkill()
{
    if(m_poCoachHero)
    {
        return m_poCoachHero->GetFormationSkill();
    }

    //异常
    return NULL;
}

UINT8 CPlayer::GetCoachCareerID()
{
    if(m_poCoachHero)
    {
        SHeroProp* pstHeroProp = m_poCoachHero->GetHeroProp();
        if(pstHeroProp)
        {
            return pstHeroProp->byCareerID;
        }
    }
    //异常
    return 0;
}


UINT16 CPlayer::OpenSkillTab(DT_SKILL_TAB_DATA_CLI& stSkillTabInfo)
{
    CSkill* poSKill = GetCoachSkill();
    if(NULL == poSKill)
    {
        RETURN_OTHER_ERR;
    }
    poSKill->GetDT_SKILL_DATA_CLI(stSkillTabInfo.stSkillInfo);
    poSKill->GetDT_ATTACK_RANG_DATA_LST(stSkillTabInfo.stAttackRangInfo);
    poSKill->GetDT_EFFECT_DATA_LST2(stSkillTabInfo.stEffectInfo);
    SSkillBaseProp& stProp = CSkillPropMgr::Instance()->GetSkillBaseProp();
    stSkillTabInfo.wAddExpPerUpgrade = stProp.wAddExpPerUpgrade;
    stSkillTabInfo.wCostGasPerUpgrade = stProp.wCostGasPerUpgrade;
    stSkillTabInfo.wCurPlayerLevel = GetLevel();
    stSkillTabInfo.qwCurCoin = GetDT_PLAYER_BASE_DATA().qwCoin;
    stSkillTabInfo.qwCurGold = GetDT_PLAYER_BASE_DATA().qwGold;
    stSkillTabInfo.qwCurPurpleGas = GetDT_PLAYER_BASE_DATA().qwPurpleGas;

    return ERR_OPEN_SKILL_TAB::ID_SUCCESS;
}


UINT16 CPlayer::UpgradeSkillAction(UINT16 ActionID, UINT16 wActionLevelCli, DT_SKILL_TAB_DATA_CLI& stSkillTabInfo)
{
    CHero* poHero = GetCoachHero();
    if(NULL == poHero)
    {
        RETURN_OTHER_ERR;
    }
    UINT16 wErrCode = poHero->UpgradeSkillAction(ActionID, wActionLevelCli);
    if(ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_SUCCESS != wErrCode)
    {
        return wErrCode;
    }
    wErrCode = OpenSkillTab(stSkillTabInfo);
    if(ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_SUCCESS != wErrCode)
    {
        SET_OTHER_ERR(wErrCode);
    }
    ResetBattleAttr();//技能对战力有影响

    return wErrCode;
}

UINT16 CPlayer::SetActionEffectRangProp(UINT16 wActionID, UINT16 wEffectID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind, DT_SKILL_TAB_DATA_CLI& stSkillTabInfo)
{
    CSkill* poSKill = GetCoachSkill();
    if(NULL == poSKill)
    {
        RETURN_OTHER_ERR;
    }

    UINT16 wErrCode = poSKill->SetActionEffectRangProp(wEffectID, byAttackRangKind, byTraceBuffKind);
    if(ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_SUCCESS != wErrCode)
    {
        return wErrCode;
    }

    wErrCode = OpenSkillTab(stSkillTabInfo);
    if(ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_SUCCESS != wErrCode)
    {
        SET_OTHER_ERR(wErrCode);
    }
    ResetBattleAttr();//技能对战力有影响

    return wErrCode;
}

VOID CPlayer::GetSyncResource(DT_RSYNC_RES_ITEM_DATA& stData)
{
	memset(&stData, 0, sizeof(DT_RSYNC_RES_ITEM_DATA));

	stData.astResList[stData.byResNum].byResKind = ACTE_COIN;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwCoin;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_GOLD;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwGold;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_PHYSTRENGTH;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.wPhyStrength;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_SCIENCE;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwScience;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_STORY;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwStory;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_BLUEGAS;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwBlueGas;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_PRUPLEGAS;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwPurpleGas;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_JINGJIE;
	stData.astResList[stData.byResNum].qwResValue = m_stDT_PLAYER_BASE_DATA.qwJingJie;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_PETSCORE;
	CPlayerPet* pPlayerPet = GetPlayerPet();
	if (NULL != pPlayerPet) {
		stData.astResList[stData.byResNum].qwResValue = pPlayerPet->GetDT_PLAYER_PET_DATA().dwPetScore;
	}
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_DoorsTribute;
	UINT64 qwDoorsTribute = 0;
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		qwDoorsTribute = 0;
	}
	else
	{
		qwDoorsTribute = poFaction->GetDoorsTribute(m_dwPlayerID);
	}
	stData.astResList[stData.byResNum].qwResValue = qwDoorsTribute;
	stData.byResNum++;

	stData.astResList[stData.byResNum].byResKind = ACTE_FactionFunds;
	UINT64 qwFactionFunds = 0;
	if ( NULL == poFaction)
	{
		qwFactionFunds = 0;
	}
	else
	{
		qwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
	}
	stData.astResList[stData.byResNum].qwResValue = qwFactionFunds;
	stData.byResNum++;


	GetPhystrength().GetPhyStrengthInfo(stData.stCurPhyStrengthInfo);

	GetPrompt(stData.stPromptInfo);
	
}


UINT16 CPlayer::OpenPlayerPanel_v410(PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410 &stAck)
{
    DT_PLAYER_INFO_PANEL_v410 &stPanel = stAck.stPlayerPanel;
    DT_BAG_DATA_CLI2 &stBagInfo = stAck.stBagInfo;

	//伙伴信息
    GetCarryHeroInfoNew( stPanel.stHeroInfo.wHeroNum, stPanel.stHeroInfo.astHeroBaseData,
        stPanel.stHeroInfo.astEquipDataList, stPanel.stHeroInfo.astEnableAttrUpdate, 
        stPanel.stHeroInfo.astSkillInfo, stPanel.stHeroInfo.astJewelryInfo
        , stPanel.stHeroInfo.astSoulInfo, stAck.stHeroSoulPotencyInfo.astSoulEquipLstData, &stAck.stHeroJewelrySuitInfo);
	stAck.stHeroSoulPotencyInfo.wHeroNum = stPanel.stHeroInfo.wHeroNum;
	m_oBag.GetDT_BAG_DATA_CLI(stBagInfo);
	/*总开放数 - 装备数 - 剩余数*/
	UINT16 wLeftNum = m_oBag.GetIdleSize();
	//获取装备物品
	m_oBag.GetEquipByPlayerPanel( stPanel.stItemInfo );
	UINT16 wOther = stPanel.stItemInfo.wItemNum + wLeftNum;
	stPanel.wGoodsCellNum = (stBagInfo.wCurOpenNum < wOther) ? 0 : (stBagInfo.wCurOpenNum - wOther);
	//获取饰品物品
	m_oBag.GetAboutJewelryLstCli(stPanel.stJewelryInfo);
	                                                                                                                                        
	wOther = (UINT8)stPanel.stJewelryInfo.wItemNum + wLeftNum;
	stPanel.wJewelryCellNum = (stBagInfo.wCurOpenNum < wOther) ? 0 : (stBagInfo.wCurOpenNum - wOther);
	CXiangqianMgr::Instance()->GetXiangqianEquipDes( stPanel.stEquipXiangqianDes.aszEquipPos1Des, stPanel.stEquipXiangqianDes.aszEquipPos2Des,
		stPanel.stEquipXiangqianDes.aszEquipPos3Des, stPanel.stEquipXiangqianDes.aszEquipPos4Des );

	if(m_poPlayerSoul)
	{
		// 战魂
		m_poPlayerSoul->GetSoulBagDataCli(stPanel.stSoulInfo, &stAck.stBagSoulPotencyInfo);
	}

	return ERR_OPEN_PLAYER_PANEL::ID_SUCCESS;
}

UINT16 CPlayer::OpenPlayerPanel(DT_PLAYER_INFO_PANEL& stPanel, DT_BAG_DATA_CLI& stBagInfo)
{
	//伙伴信息
	UINT16 wHeroNum = 0;
	GetCarryHeroInfoNew( wHeroNum, stPanel.stHeroInfo.astHeroBaseData,
	stPanel.stHeroInfo.astEquipDataList, stPanel.stHeroInfo.astEnableAttrUpdate, 
	stPanel.stHeroInfo.astSkillInfo, stPanel.stHeroInfo.astJewelryInfo
	, stPanel.stHeroInfo.astSoulInfo);

	stPanel.stHeroInfo.wHeroNum = wHeroNum;
	m_oBag.GetDT_BAG_DATA_CLI(stBagInfo);
	/*总开放数 - 装备数 - 剩余数*/
	UINT16 wLeftNum = m_oBag.GetIdleSize();
	//获取装备物品
	m_oBag.GetEquipByPlayerPanel( stPanel.stItemInfo );
	UINT8 byOther = stPanel.stItemInfo.byItemNum + (UINT8)wLeftNum;
	stPanel.byGoodsCellNum = (stBagInfo.byCurOpenNum < byOther) ? 0 : (stBagInfo.byCurOpenNum - byOther);
	//获取饰品物品
	m_oBag.GetAboutJewelryLstCli(stPanel.stJewelryInfo);

	byOther = (UINT8)stPanel.stJewelryInfo.byItemNum + (UINT8)wLeftNum;
	stPanel.byJewelryCellNum = (stBagInfo.byCurOpenNum < byOther) ? 0 : (stBagInfo.byCurOpenNum - byOther);
	CXiangqianMgr::Instance()->GetXiangqianEquipDes( stPanel.stEquipXiangqianDes.aszEquipPos1Des, stPanel.stEquipXiangqianDes.aszEquipPos2Des,
		stPanel.stEquipXiangqianDes.aszEquipPos3Des, stPanel.stEquipXiangqianDes.aszEquipPos4Des );

    if(m_poPlayerSoul)
	{
        // 战魂
	    m_poPlayerSoul->GetSoulBagDataCli(stPanel.stSoulInfo);
    }

	return ERR_OPEN_PLAYER_PANEL::ID_SUCCESS;
}

UINT16 CPlayer::OpenHeroSelectPanel(PKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK& stAck)
{
	set<CHero*> setHero;
	UINT8 byIdx = 0;
	CHero* poHero = GetCoachHero();
	poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stAck.astHeroBaseData[byIdx]);
    stAck.abyFlyFlagList[byIdx] = poHero->IsShowFly();
	byIdx++;
	setHero.insert(poHero);

	poHero = m_mapFormationHero.GetFistData();
	while(poHero)
	{
		if ( setHero.end() != setHero.find(poHero))
		{
			poHero = m_mapFormationHero.GetNextData();
			continue;
		}
		setHero.insert(poHero);
		poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stAck.astHeroBaseData[byIdx]);
        stAck.abyFlyFlagList[byIdx] = poHero->IsShowFly();
		poHero = m_mapFormationHero.GetNextData();
		byIdx++;
	}

	poHero = m_mapKind2HeroAllCarry.GetFistData();
	while(poHero)
	{
		if ( setHero.end() != setHero.find(poHero))
		{
			poHero = m_mapKind2HeroAllCarry.GetNextData();
			continue;
		}
		poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stAck.astHeroBaseData[byIdx]);
        stAck.abyFlyFlagList[byIdx] = poHero->IsShowFly();
		poHero = m_mapKind2HeroAllCarry.GetNextData();
		byIdx++;
	}

	stAck.wHeroNum = byIdx;
	return ERR_OPEN_HREO_SELECT_PANEL::ID_SUCCESS;
}

BOOL CPlayer::IsFly()
{
    CHero *poCoach = GetCoachHero();
    return poCoach->IsFly();
}

UINT16 CPlayer::Fly(UINT16 wHeroID, PKT_CLIGS_FLY_ACK &stAck)
{
    CHero *poHero = GetHero(wHeroID);
    return (poHero ? poHero->Fly(stAck) : 0);
}