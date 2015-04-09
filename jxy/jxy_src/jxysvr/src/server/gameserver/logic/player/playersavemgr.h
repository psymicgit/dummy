
#ifndef _PLAYERSAVEMGR_H_
#define _PLAYERSAVEMGR_H_

#include "player.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/basedefine.h>

using namespace SGDP;


#define MAX_PLAYERSAVEDATA_POOL_NUM	100

class CPlayerSaveData
{
public:
	CPlayerSaveData()
	{ 
		m_wErrCode = 0; 
		memset(&m_stPlayerData, 0, sizeof(m_stPlayerData));
		//memset(&m_stLastPlayerData, 0, sizeof(m_stLastPlayerData));
		m_bAddOnlineTime=FALSE;
	}
	~CPlayerSaveData(){};
public:
	DT_PLAYER_DATA		m_stPlayerData;
	//DT_PLAYER_DATA		m_stLastPlayerData;
	UINT16				m_wErrCode;
	BOOL				m_bAddOnlineTime;
};

typedef HashMap<UINT32, CPlayerSaveData*> CID2PlayerSaveDataMap; //key为playerid
typedef CID2PlayerSaveDataMap::iterator CID2PlayerSaveDataMapItr;

struct SPlunderSaveData
{
	SPlunderSaveData() { dwPlayerID = 0; memset(&stPlunderBaseData, 0, sizeof(stPlunderBaseData)); memset(&stPlunderBattleLog, 0, sizeof(stPlunderBattleLog)); 
		memset(&stCaptureData, 0, sizeof(stCaptureData)); memset(&stEnemyData, 0, sizeof(stEnemyData)); }
	~SPlunderSaveData(){};

	UINT32 dwPlayerID;
	DT_PLUNDER_BASE_DATA stPlunderBaseData; //掠夺基本信息
	DT_PLUNDER_BATTLELOG_DATA_LIST stPlunderBattleLog; //掠夺战报信息
	DT_CAPTURE_DATA_LIST stCaptureData; //掠夺俘虏信息
	DT_ENEMY_DATA_LIST stEnemyData; //仇敌信息	
};

class CPlayersPlunderSaveData
{
public:
	CPlayersPlunderSaveData(){ m_wErrCode = 0; m_vecPlunderSaveData.clear();}; 
	~CPlayersPlunderSaveData(){};
public:
	vector<SPlunderSaveData*> m_vecPlunderSaveData;

	UINT16				m_wErrCode;
};


struct SRaceSaveData
{
	SRaceSaveData() { dwPlayerID = 0; memset(&stRaceData, 0, sizeof(stRaceData)); memset(&stRaceBattleLog, 0, sizeof(stRaceBattleLog)); }
	~SRaceSaveData(){};

	UINT32 dwPlayerID;
	DT_RACE_BASE_DATA     stRaceData; //玩家竞技场信息
	DT_RACE_BATTLELOG_DATA_LIST stRaceBattleLog; //玩家竞技场战报信息
};

class CPlayersRaceSaveData
{
public:
	CPlayersRaceSaveData(){ m_wErrCode = 0; m_vecRaceSaveData.clear();}; 
	~CPlayersRaceSaveData(){};
public:
	vector<SRaceSaveData*> m_vecRaceSaveData;

	UINT16				m_wErrCode;
};


//指定存储的数据库类型
enum ESaveDBType
{
	ESDBT_GAME=0, //游戏数据库
	ESDBT_RECORD, //采集数据库
};

class CPlayerSaveMgr
{
public:
	DECLARE_SINGLETON(CPlayerSaveMgr)

protected:
	CPlayerSaveMgr();
	~CPlayerSaveMgr();
public:
	BOOL Init();
	VOID UnInit();

public:
	BOOL	SavePlayerData( CPlayer* poPlayer, ESaveDBType type = ESDBT_GAME, DT_PLAYER_DATA *pstDT_PLAYER_DATA = NULL);//保存玩家基本信息
	
	BOOL	SavePlayerNoPvPData(CPlayer* poPlayer, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);

    BOOL	SavePlayerPvPData(CPlayer* poPlayer, DT_PLAYER_PVP_DATA& stPlayerPvpData);

	VOID	SaveAllPlayerPvpData();

	//DT_PLAYER_DATA	GetPlayerData( CPlayer* poPlayer);
public:
	//////////////////////////////////////////////////////////
	VOID	AddPlayerPlunderSaveData(CPlayer* poPlayer1, CPlayer* poPlayer2 = NULL, CPlayer* poPlayer3 = NULL);//保存玩家掠夺基本信息
	VOID	AddPlayerRaceSaveData(CPlayer* poPlayer1, CPlayer* poPlayer2 = NULL);//保存玩家竞技场基本信息
	//////////////////////////////////////////////////////////

	BOOL	OnSavePlayerDataAck(UINT32 dwPlayerID, CPlayerSaveData* poPlayerSaveData);//保存玩家ACK处理
	BOOL	OnSavePlayerDataAck(CPlayersPlunderSaveData* poPlayersPlunderSaveData);//保存玩家ACK处理
	BOOL	OnSavePlayerDataAck(CPlayersRaceSaveData* poPlayersRaceSaveData);//保存玩家ACK处理
public:	
	BOOL	GetPlayerBaseSaveData(CPlayer* poPlayer, DT_PLAYER_BASE_DATA& stPlayerBaseData);//获取要保存玩家基本信息
	BOOL	GetPlayerTodayConsumeData(CPlayer* poPlayer, DT_PLAYER_TODAY_CONSUME_DATA &stDT_PLAYER_TODAY_CONSUME_DATA);//获取要保存玩家基本信息
	BOOL	GetExchangeResultLog (CPlayer* poPlayer, DT_EXCHANGE_RESULT_LOG& stDT_EXCHANGE_RESULT_LOG);//获取要保存玩家基本信息

	BOOL	GetHeroSaveData(CPlayer* poPlayer, DT_HERO_DATA_LIST& stHeroData);//获取要保存武将信息
	BOOL	GetBuildSaveData(CPlayer* poPlayer, DT_BUILD_DATA_LIST& stBuildData);//获取要保存建筑信息	
	BOOL	GetInstanceRecordSaveData(CPlayer* poPlayer, DT_INSTANCE_DATA_LIST& stCommonInstanceData, DT_INSTANCE_DATA_LIST& stEliteInstanceData, 
		DT_INSTANCE_DATA_LIST& stClimbTowerInstanceData, DT_INSTANCE_DATA_LIST& stOutBoundInstanceData);//获取要保存副本记录信息
	//BOOL	GetBagSaveData(CPlayer* poPlayer, DT_BAG_DATA& stBagData);//获取要保存背包信息
	BOOL	GetBagItemSaveData(CPlayer* poPlayer, DT_BAG_EQUIP_DATA_LST& stBagEquipData, DT_BAG_GOODS_DATA_LST& stBagGoodsData, DT_BAG_JEWELRY_DATA_LST& stBagJewelryData);//获取要保存背包道具信息	
	BOOL	GetTaskSaveData(CPlayer* poPlayer, DT_TASK_DATA_LIST& stTaskData);//获取要保存任务信息
	BOOL	GetRaceSaveData(CPlayer* poPlayer, DT_RACE_BASE_DATA& stRaceData);//获取要保存竞技信息
	BOOL	GetLocaleSaveData(CPlayer* poPlayer, DT_LOCALE_DATA& stLocaleData);//获取要保存当前位置信息
	BOOL	GetStateSaveData(CPlayer* poPlayer, DT_STATE_DATA& stStateData);//获取要保存当前状态信息
	BOOL	GetOnlineEncRecd(CPlayer* poPlayer, DT_ONLINE_ENCOURAGE_RECORD_DATA& stOnlineRecd);//获取要保存在线奖励信息
	BOOL	GetLoginEncRecd(CPlayer* poPlayer, DT_LOGIN_ENCOURAGE_RECORD_DATA& stLoginRecd);//获取要保存登录奖励信息
	BOOL	GetOnhookRecd(CPlayer* poPlayer, DT_ONHOOK_RCD_DATA& stOnhookRcdData);//获取要保存挂机信息
	BOOL	GetRaceBattleLog(CPlayer* poPlayer, DT_RACE_BATTLELOG_DATA_LIST& stRaceBattleLog);//获取要保存竞技场战报信息
	BOOL	GetPlunderBase(CPlayer* poPlayer, DT_PLUNDER_BASE_DATA& stPlunderBaseData);//获取要保存掠夺基本信息
	BOOL	GetPlunderBattleLog(CPlayer* poPlayer, DT_PLUNDER_BATTLELOG_DATA_LIST& stPlunderBattleLog);//获取要保存掠夺战报信息
	BOOL	GetPlunderCapture(CPlayer* poPlayer, DT_CAPTURE_DATA_LIST& stCaptureData);//获取要保存俘虏信息
	BOOL	GetEnemy(CPlayer* poPlayer, DT_ENEMY_DATA_LIST& stEnemyData);//获取要保存仇敌信息
	BOOL	GetMoneyTree(CPlayer* poPlayer, DT_MONEY_TREE_DATA& stMoneyTreeData);//获取要保存摇钱树信息
	BOOL	GetAwaken(CPlayer* poPlayer, DT_AWAKEN_DATA& stAwaken);//获取要保存悟道信息	
	BOOL	GetMaxBattleRecord(CPlayer* poPlayer, DT_INSTANCE_BATTLE_DATA& stCommonBattleRcd, DT_INSTANCE_BATTLE_DATA& stEliteBattleRcd, DT_INSTANCE_BATTLE_DATA& stClimbTowerBattleRcd);//获取要保存最大战斗记录信息	
	BOOL	GetJingJieSaveData(CPlayer* poPlayer, DT_JINGJIE_DATA& stJingJieData);//获取要保存境界信息	
	BOOL	GetPhystrengthSaveData(CPlayer* poPlayer, DT_PHYSTRENGTH_DATA& stPhystrengthData);//获取要保存体力信息	
	BOOL	GetCropSaveData(CPlayer* poPlayer, DT_CROP_DATA& stDT_CROP_DATA);//获取种值信息
	BOOL	GetFewParamSaveData(CPlayer* poPlayer, DT_FEW_PARAM_DATA& stFewParamData);//获取要保存稀少参数信息	
	BOOL	GetScienceTreeSaveData( CPlayer *poPlayer, DT_SCIENCE_TREE_DATA &stScienceTreeData ); //获取科技树的信息
	BOOL	GetFormationSaveData( CPlayer *poPlayer, DT_NEW_FORMATION_DATA &stFormationData ); //获取要保存阵型信息
	BOOL	GetYabiaoSaveData( CPlayer *poPlayer, DT_YABIAO_BASE_DATA &stYabiaoData ); //获取要保存押镖信息
	BOOL	GetEatBreadSaveData( CPlayer *poPlayer, DT_EAT_BREAD_DATA &stEatBreadData ); //获取要保存吃包子信息
	BOOL	GetYabiaoBattleLogData( CPlayer *poPlayer, DT_YABIAO_BATTLELOG_DATA_LIST &stYabiaoBattleLogData ); //获取要保存押镖战报信息
	BOOL	GetClimbTowerSaveData(CPlayer* poPlayer, DT_CLIMB_TOWER_DATA & stDT_CLIMB_TOWER_DATA); //获取要保存爬塔信息
	BOOL	GetActivityRecordDataSaveData(CPlayer* poPlayer, DT_LUAACTIVITY_RECORD_LIST_DATA & stActivityRecordData); //获取要保存lua脚本活动记录信息
	BOOL	GetVipExtData( CPlayer *poPlayer, DT_VIP_EXT_DATA &stDT_VIP_EXT_DATA );
	BOOL	GetCollectGas( CPlayer *poPlayer, DT_COLLECT_GAS_DATA &stCollectGas ); //获取要保存集气信息	
	BOOL	GetActivityEncourageData( CPlayer *poPlayer, DT_ACTIVITY_ENCOURAGE_DATA &stActivityEncourage ); //获取要保存活动奖励信息
	BOOL	GetBossBBase(CPlayer* poPlayer, DT_BOSS_BATTLE_PLAYER_DATA& stBossBattlePlayerData); //获取要保存boss战基本信息
	BOOL	GetBossBBLData(CPlayer* poPlayer, DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST& stBossBBLData); //获取要保存boss战战报信息
	BOOL	GetRdChallengeData(CPlayer* poPlayer, DT_RANDOM_CHALLENGE_DATA& stRdChallengeData); //获取要保存随机挑战信息
	BOOL	GetWorshipData(CPlayer* poPlayer, DT_WORSHIP_DATA_LST& stWorshipData); //获取要保存膜拜信息
	BOOL	GetScoreData(CPlayer* poPlayer, DT_SCORE_DATA_LST& stScoreData); //获取要保存评分信息
	BOOL	GetEquipCompose(CPlayer* poPlayer, DT_EQUIP_COMPOSE_DATA& stEquipComposeData); //获取要保存装备合成信息
	BOOL	GetGatherScience(CPlayer* poPlayer, DT_GATHER_SCIENCE_DATA &stGatherScienceData); //获取要保存聚魂信息
	BOOL GetBegBlue(CPlayer* poPlayer, DT_BEGBLUE_DATA &stBegBlueData); //获取要保存求仙露信息
	BOOL GetFriendsBaseData(CPlayer* poPlayer, DT_FRIENDS_BASE_DATA& stDT_FRIENDS_BASE_DATA); //获取要保存的送鲜花信息 
	BOOL GetRelationDataList(CPlayer* poPlayer, DT_RELATION_DATA_LIST &stRelationDataList); //获取要保存的玩家朋友关系
	BOOL GetDataEvenList(CPlayer* poPlayer, DT_DATA_EVEN_LIST &stDataEvenList); //获取要保存的玩家朋友关系
    BOOL GetRecvFlowerLogList(CPlayer* poPlayer, DT_RECV_FLOWER_LOG_LIST &stRecvFlowerLogList); //获取玩家收花日志
	BOOL GetEquipMaster(CPlayer &oPlayer, DT_EQUIP_MASTER&); //获取装备精通信息
	BOOL GetCourageData(CPlayer* poPlayer, DT_COURAGE_DATA &stDT_COURAGE_DATA); //获取勇气信息
	BOOL GetFactionPlayer(CPlayer* poPlayer, DT_PLAYER_FACTION_DATA& stPlayerFaction);//获取门派信息
	BOOL GetPlayerMark( CPlayer* poPlayer, DT_PLAYER_MARK_DATA& stPlayerMark);	//标志信息
	BOOL GetPlayerGVG(CPlayer* poPlayer, DT_PLAYER_GVG_DATA& stGVGData);
	BOOL GetPlayerSoul(CPlayer* poPlayer, DT_PLAYER_SOUL_DATA& stSoulData);
	BOOL GetPlayerMedal(CPlayer* poPlayer, DT_PLAYER_MEDAL_DATA& stMedalData);
	BOOL GetPlayerPet(CPlayer* pPlayer, DT_PLAYER_PET_DATA& rPetData);

protected:
	VOID	SavePlayersPlunderData();
	VOID	SavePlayersRaceData();

private:
	CID2PlayerSaveDataMap							m_mapID2PlayerSaveData;
	CSDObjectPool<CPlayerSaveData, CSDMutex>			m_oPlayerSaveDataPool;	
	CSDObjectPool<SPlunderSaveData, CSDMutex>		m_oPlunderSaveDataPool;	
	CSDObjectPool<CPlayersPlunderSaveData, CSDMutex> m_oPlayersPlunderSaveDataPool;	
	CSDObjectPool<SRaceSaveData, CSDMutex>			m_oRaceSaveDataPool;	
	CSDObjectPool<CPlayersRaceSaveData, CSDMutex>	m_oPlayersRaceSaveDataPool;	
	HashMap<UINT32, UINT32> m_mapPlunderChangePlayer;
	HashMap<UINT32, UINT32> m_mapRaceChangePlayer;
	UINT32		m_dwPvpSaveNum;//pvp每次保存个数
// 	HashMap<CPlayersPlunderSaveData*, CPlayersPlunderSaveData*> m_mapSavingPlayersPlunder;
// 	HashMap<CPlayersRaceSaveData*, CPlayersRaceSaveData*> m_mapSavingPlayersRace;
};


#endif

