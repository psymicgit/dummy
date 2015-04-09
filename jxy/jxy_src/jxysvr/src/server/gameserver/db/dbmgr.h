

#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#pragma once

#include <sddb.h>
#include <sdobjectpool.h>
#include <map>
#include <sdmutex.h>
#include <logic/player/playermgr.h>
#include <set>


using namespace std;
using namespace SGDP;

class CBaseDBCMD;
class CClrPlayerDataCMD;
class CGetPlayerDataCMD;
class CGetPlayerDataCMD2;
class CSavePlayerDataCMD;
class CCreatePlayerCMD;
class CRecordPlayerLoginoff;
class CSavePlayersPlunderDataCMD;
class CSavePlayersRaceDataCMD;
class CRecordBattleData;
class CRecordBattleData2;
class CGetBattleData;
class CGetBattleData2;
class CGetChatCMD;
class CGetPlayerDataCMD_OLD; //fortest
class CSaveBossBDataCMD;
class CSetAppPayInfo;
class CSetAppPayProcessed;//DB命令类型
class CSetTongBuPayInfo;
class CSetTongBuPayProcessed;
class CActivityEncourageCMD;
class CUpdateCdKeyCmd;
class CSetMyPayProcessed;
class CSaveFactionDataCMD;
class CGetGsNoticeCMD;
class CGetOprateingActivityCMD;
class CCreateFactionDataCMD;
class CSavePlayerConsume;
class CGetPlayerConsume;
class CSetCenterPayProcessed;
class CGetPrivateMsgLogCmd;
class CSavePrivateMsgLogCmd;
class CSaveCourageDataCMD;
class CPlayer;
class CLeaveFactionDataCMD;
class CSetRecreatePayProcessed;
class CSaveGvGVideoCMD;
class CSaveGvGActivityCMD;
class CSaveGvGArenaCMD;
class CSavePlayerSnapShotCMD;
class CSaveGvGBuildCMD;
class CDeleteOverGvGift;
class CAddGvGiftCMD;
class CAddPlayersLogCMD;
class CDelPlayerLogCMD;
class CClearGvGActivityCMD;
class CClearGvGVideoCMD;
class CDelPlayerSnapShotCMD;
class CSetReadPrivateMsgCMD;
class CAddPlayerGiftCMD;
class COperatingActivityCMD;

typedef struct tagDT_PLAYER_NOPVP_DATA DT_PLAYER_NOPVP_DATA;

struct tagDT_FACTION_DATA;
typedef tagDT_FACTION_DATA DT_FACTION_DATA;

enum EDBCmdType
{
    EDBCT_CREATE = 1,//创建玩家信息
    EDBCT_GET1 = 2,//获取1个玩家信息
    EDBCT_GET2 = 3,//获取2个玩家信息
    EDBCT_SAVE = 4,//保存玩家信息
    EDBCT_CLR = 5,//清楚玩家信息
    EDBCT_REG = 6,//注册用户名信息
    EDBCT_LOGINOFFINFO = 7, //玩家的登入登出信息
    EDBCT_SAVEPLUNDER = 10 , //保存玩家掠夺信息
    EDBCT_SAVERACE = 11 , //保存玩家竞技场信息
    EDBCT_SAVEBATTLEDATA = 12, //保存战斗信息
    EDBCT_GETBATTLEDATA = 13, //获得战斗信息
    EDBCT_CHAT = 14, //获取聊天公告信息
    //EDBCT_GET_OLD = 15,//OLD获取玩家信息
    EDBCT_SAVEBOSSB = 16,//保存boss战模块信息
    EDBCT_SETPAYINFO = 17, //设置充值信息
    EDBCT_SETPAYPPROCESSED = 18, //设置充值完毕;
    EDBCT_SAVEBOSSBDATA = 19, //保存boss战模块信息
    EDBCT_SETTONGBUPAYINFO = 20, //设置同步充值信息
    EDBCT_SETTONGBUPAYPPROCESSED = 21, //设置同步充值完毕;
    EDBCT_ACTIVITYENCOURAGE = 22, //活动奖励定时器;
    EDBCT_UPDATE_CK_KEY = 23, //更改CdKey
    EDBCT_SETMYPAYPPROCESSED = 24, //处理自己的充值
    EDBCT_FACTION = 25,		//帮派
    EDBCT_GETGSNOTICE = 26,	//获取gs公告内容
    EDBCT_GETOPRATEINGACTIVITY = 27,	//获取运营活动内容
    EDBCT_CREATE_FACTION = 28,		//创建帮派
    EDBCT_SAVE_PLAYER_CONSUME = 29, //保存玩家消费记录
    EDBCT_GET_PLAYER_CONSUME = 30, //获取玩家消费记录
    EDBCT_SETCENTERPAYPPROCESSED = 31, //处理充值中心的充值
    EDBCT_GET_PRIVATEMSGLOG = 32, //获取私聊信息
    EDBCT_SAVEPRIVATEMSGLOG = 33, //保存聊天日志
    EDBCT_SAVE_COURAGE_DATA = 34, //保存勇气相关
    EDBCT_LEAVE_FACTION = 35,	//退出门派
    EDBCT_SETRECREATEPAYPROCESSED = 36,//处理回档时捞回数据的添加
    EDBCT_SAVE_GVG_VIDEO          = 37, // 批量保存gvg中的战斗录像
	EDBCT_SAVE_GVG_ACTIVITY       = 38, // 保存门派跨服战活动数据
    EDBCT_SAVE_GVG_BUILD          = 39, // 保存各区的门派跨服战数据
    EDBCT_SAVE_PLAYER_SNAP_SHOT   = 40, // 保存玩家的快照
    EDBCT_GET_PLAYER_SNAP_SHOT    = 41, // 获取玩家的快照
    EDBCT_SAVEBATTLEDATA2 = 42, //保存战斗信息2
    EDBCT_GETBATTLEDATA2 = 43, //获得战斗信息2
	EDBCT_DELETE_GVG_GIFT = 44,//删除过期礼包
	EDBCT_ADD_GVG_GIFT = 45,//增加礼包
	EDBCT_DEL_PLAYER_LOG = 46,//删除玩家战报
	EDBCT_ADD_PLAYER_LOG = 47,//添加玩家战报
    EDBCT_SAVE_GVG_ARENA = 48, // 保存门派跨服战中单个赛区的数据
    EDBCT_CLEAR_GVG_ACTIVITY = 49, // 清除门派跨服战的活动数据
    EDBCT_CLEAR_GVG_VIDEO = 50, // 批量保存gvg中的战斗录像
    EDBCT_DEL_PLAYER_SNAP_SHOT    = 51, // 删除玩家的快照
	EDBCT_SET_READ_PRIVATE_MSG	  = 52,	// 设置已读信息
	EDBCT_ADD_PLAYER_GIFT = 53,//添加个人礼包
	EDBCT_OPERATING_ACTIVITY = 54,//操作活动时间
};

//PlayerExt各个模块枚举类型
enum EPlayerExtDataType
{
    EMDT_CurState = 1, // 基本 信息
    EMDT_CommonBattleRcd = 2, // 基本 信息
    EMDT_EliteBattleRcd = 3, // 基本 信息
    EMDT_LocaleData = 4, // 基本 信息
    EMDT_LoginEncRecdData = 5, // 基本 信息
    EMDT_MoneyTreeData = 6, // 基本 信息
    EMDT_OnlineEncRecdData = 7, // 基本 信息
    EMDT_PhystrengthData = 8, // 基本 信息
    EMDT_JingJieData = 9, // 基本 信息
    EMDT_CropData = 10, // 基本 信息
    EMDT_ScienceTree = 11, // 基本 信息
    EMDT_FewParamData = 12, // 基本 信息
    EMDT_FormationInfo = 13, // 基本 信息
    EMDT_YabiaoInfo = 14, // 基本 信息
    EMDT_ClimbTowerBattleRcd = 15, // 基本 信息
    EMDT_ClimbTowerData = 16, // 基本 信息
    EMDT_EatBreadInfo = 17, // 基本 信息
    EMDT_YabiaoBattleLog = 18, // 基本 信息
    EMDT_AwakenData = 19, // 基本 信息
    EMDT_VipExtData = 20, // 基本 信息
    EMDT_LuaActivityRecordData = 21, // 基本 信息
    EMDT_GasCollectData = 22, // 基本 信息
    EMDT_BossBattleData = 23, // 基本 信息
    EMDT_ACTIVITY_ENCOURAGE = 24, //充值奖励
    EMDT_BossBBL = 25, //boss战战报
    EMDT_RdChallengeData = 26, //随机挑战信息
    EMDT_WorshipData = 27, //膜拜信息
    EMDT_ScoreData = 28, //副本评分信息
    EMDT_EquipComposeData = 29, //装备合成信息
    EMDT_GatherScienceData = 30, //装备合成信息
    EMDT_BegBlueData = 31, // 求仙露信息
    EMDT_FriendsBaseData = 32, // 好友基本信息
    EMDT_PlayerFactionData = 33,// 玩家门派信息
    EMDT_Courage = 34, // 勇气相关信息
    EMDT_EquipMaster = 35, // 装备精通信息
	EMDT_ExchangeResultLog = 36, // 充值推送信息
	EMDT_GVGData = 37,//GVG信息
	EMDT_SoulData = 38,//战魂
	EMDT_MedalData = 39,//勋章
	EMDT_PetData = 40,//宠物
};

enum EPlayerPvpDataType
{
    EMDT_RaceData = 100,
    EMDT_RaceBattleLog = 101,
    EMDT_PlunderBaseData = 102,
    EMDT_PlunderBattleLog = 103,
    EMDT_CaptureData = 104,
    EMDT_EnemyData = 105,
};


//伙伴扩展信息类型
enum EHeroExtDataType
{
    EHEDT_Equip = 1, //装备 信息
    EHEDT_Drug = 2, //丹药 信息
    EHEDT_Godweapon = 3, //神器 信息
    EHEDT_Skill = 4, //技能 信息
    EHEDT_Study = 5, //心得 信息
    EHEDT_Absorb = 6, //经验传递信息
	EHEDT_Jewelry = 7,//饰品信息
	EHEDT_SOUL = 8, // 装备战魂信息
};

//模块类型
enum EModuleDataType
{
    EMDT_BossBattle = 1, //boss战
    EMDT_COURAGE = 3,	 //勇气
    EMDT_GVG = 4,	     //跨服战
};


enum ESaveDataType
{
    EDT_PlayerBase = 1,
    EDT_PlayerExt ,
    EDT_PlayerBag ,
    EDT_PlayerBuildAll ,
    EDT_PlayerBuild ,
    EDT_PlayerHeroAll ,
    EDT_PlayerHeroBase ,
    EDT_PlayerHeroExt ,
    EDT_PlayerInstanceAll ,
    EDT_PlayerInstance ,
    EDT_PlayerTaskAll,
    EDT_PlayerTask,
};

class CPlayerDiff;
typedef map<UINT32, CPlayerDiff*> CPlayerDiffMap;
typedef CPlayerDiffMap::iterator CPlayerDiffMapItr;

class CFactionDiff;
typedef map<UINT32, CFactionDiff*> CFactionDiffMap;
typedef CFactionDiffMap::iterator CFactionDiffMapItr;

typedef map<UINT32, UINT64> CMapSavePlayerTime;
typedef CMapSavePlayerTime::iterator CMapSavePlayerTimeItr;




#define DB_ERROR_DIR_MAIN_NAME "db_error"

struct SErrorInfo
{
    SErrorInfo()
    {
		qwFileTimes = 0;
        strFilePath = "";
    }

    ~SErrorInfo()
    {

    }
	UINT64	qwFileTimes;
    string strFilePath; //文件路径
};

#define ERROR_FILE_PREFIX_PLAYER	"E"
#define ERROR_FILE_PREFIX_FACTION	"F"

enum ERROR_SQL_MODULE
{
    ERROR_PLAYER = 1,
    ERROR_FACTION = 2,
};

class CDBMgr
{
    typedef map<UINT64, SErrorInfo> CErrorInfoMap;
    typedef CErrorInfoMap::iterator CErrorInfoMapItr;
public:
    CDBMgr();
    ~CDBMgr();

public:
    BOOL Init();
    VOID Release();
    BOOL Run(BOOL bSaveAll = FALSE);
    BOOL AddCommand(SGDP::ISDDBCommand* poDBCommand);
    BOOL AddRecordCommand( SGDP::ISDDBCommand *poDBCommand );
    BOOL AddPayCommand( SGDP::ISDDBCommand *poDBCommand );
    BOOL AddCfgCommand( SGDP::ISDDBCommand *poDBCommand );
    VOID AddDBCommandCount();
    VOID AddRecordDBCommandCount();
    VOID DecreaseDBCommandCount();
    VOID DecreaseRecordDBComandCount();
    UINT32 GetDBCommandCount();
    UINT32 GetRecordDBCommandCount();
    SGDP::ISDDBSession* GetDBSession();
    SGDP::ISDDBSession* GetRecordDBSession();

    VOID SetStartData( UINT32 dwPlayerID, const DT_PLAYER_NOPVP_DATA &data );
    const DT_PLAYER_NOPVP_DATA* GetLastData( UINT32 dwPlayerID );
    VOID StartDiff( UINT32 dwPlayerID, const DT_PLAYER_NOPVP_DATA &data );
    VOID EndDiff(UINT32 dwPlayerID);


    BOOL Changed(UINT32 dwFactionID, const DT_FACTION_DATA &stDT_FACTION_DATA);
    BOOL HaveChanged(UINT32 dwFactionID, const DT_FACTION_DATA &stDT_FACTION_DATA);


    CPlayerDiff* FindDiff(UINT32 dwPlayerID);
    BOOL HaveChanged(UINT32 dwPlayerID, ESaveDataType eTableType, UINT16 wKey1 = 0, UINT16 wKey2 = 0, UINT16 wKey3 = 0);

    VOID OnPlayerRelease(UINT32 dwPlayerID);

    VOID OnFactionRelease(UINT32 dwFactionID);

    SGDP::ISDDBSession* GetCfgDBSession();
    SGDP::ISDDBSession* GetPayDBSession();
    //SGDP::ISDDBSession* GetTmpDBSession() { return m_pTmpDBSession; }

    //增加采集信息
    BOOL AddRecordData( EDBCmdType eDbCmdType, void *pUserData );

    VOID SetSaveProcessed();

    VOID AddErrorInfo(ERROR_SQL_MODULE emModule, UINT32 dwKey, const vector<string> &vecStrErrorSql );
    BOOL IsExistErrorInfo(ERROR_SQL_MODULE emModule,  UINT32 dwKey );
    BOOL DeleteErrorInfo(ERROR_SQL_MODULE emModule,  UINT32 dwKey );
    vector<string> GetSqlList(ERROR_SQL_MODULE emModule,  UINT32 dwKey );
	BOOL ReadRetreatePlayerData(const char* FileName,UINT32 unAddGold,UINT32 unAddRMB,UINT32 dwPlayerID);//对个人数据进行回档
	BOOL	GetRetreateMaxIndex(UINT32 &dwIndex);//获得充值记录中最大的自增索引ID
	BOOL GetRechargeGold(UINT32 dwPlayerId,UINT32 &dwAddGold,UINT32 &dwAddRMB,UINT64 dwTime);//获得玩家当前丢失的元宝和人民币值
    UINT64 GetErrorFileUpdateTime(ERROR_SQL_MODULE emModule,  UINT32 dwKey );

	BOOL CheckAndProcessBackupFactionSql(UINT32 dwFactionID );
	BOOL FindPlayerSaveTime(UINT32 dwPlayerID);
public:
    /////////////////
    CBaseDBCMD*	CreateCmd(EDBCmdType eDbCmdType);
    VOID			FreeCmd(CBaseDBCMD* poBaseDBCMD);
    VOID			SaveDataToDB();
    VOID			SaveAllDataToDB();

protected:
    //VOID		SaveErrorID();

private:
    CDBMgr(const CDBMgr& rhs);
    CDBMgr& operator = (const CDBMgr& rhs);

private:
    SGDP::ISDDBModule*  m_pDBModule;
    SGDP::ISDDBSession* m_pDBSession;
    SGDP::ISDDBSession* m_pCfgDBSession;
    //SGDP::ISDDBSession* m_pTmpDBSession;

    //采集数据库相关

    SGDP::ISDDBModule*  m_pDBRecordModule; //数据采集库
    SGDP::ISDDBSession* m_pDBRecordSession; //数据采集数据库的会话

    //app充值相关
    SGDP::ISDDBModule*  m_pDBPayModule; //数据采集库
    SGDP::ISDDBSession* m_pDBPaySession; //数据采集数据库的会话

    CSDObjectPool<CClrPlayerDataCMD, CSDMutex>	m_oCClrPlayerDataCMDPool;
    CSDObjectPool<CGetPlayerDataCMD, CSDMutex>	m_oCGetPlayerDataCMDPool;
    //CSDObjectPool<CGetPlayerDataCMD_OLD, CSDMutex>	m_oCGetPlayerDataCMDPoolOld;
    CSDObjectPool<CGetPlayerDataCMD2, CSDMutex>	m_oCGetPlayerDataCMD2Pool;
    CSDObjectPool<CSavePlayerDataCMD, CSDMutex>	m_oCSavePlayerDataCMDPool;
    CSDObjectPool<CCreatePlayerCMD, CSDMutex>	m_oCCreatePlayerCMDPool;
    CSDObjectPool<CRecordPlayerLoginoff, CSDMutex> m_oCCRecordPlayerLoginoffPool; //玩家登入登出采集数据池
    CSDObjectPool<CSavePlayersPlunderDataCMD, CSDMutex> m_oCSavePlayersPlunderDataCMDPool; //玩家获得元宝记录
    CSDObjectPool<CSavePlayersRaceDataCMD, CSDMutex> m_oCSavePlayersRaceDataCMDPool; //玩家获得元宝记录
    CSDObjectPool<CRecordBattleData, CSDMutex>	m_oCRecordPlayerBattleDataCMDPool; //玩家战斗记录
    CSDObjectPool<CGetBattleData, CSDMutex>		m_oCGetPlayerBattleDataCMDPool; //玩家战斗记录
    CSDObjectPool<CRecordBattleData2, CSDMutex>	m_oCRecordPlayerBattleData2CMDPool; //玩家战斗记录2
    CSDObjectPool<CGetBattleData2, CSDMutex>		m_oCGetPlayerBattleData2CMDPool; //玩家战斗记录2
    CSDObjectPool<CGetChatCMD, CSDMutex>		m_oCGetChatCMDPool; //玩家聊天公告信息
    CSDObjectPool<CSaveBossBDataCMD, CSDMutex>	m_oSaveBossBDataCMDPool; //boss战信息
    CSDObjectPool<CSetAppPayInfo, CSDMutex>		m_oCSetAppPayCMDPool; //设置充值信息
    CSDObjectPool<CSetAppPayProcessed, CSDMutex> m_oCSetAppPayProcessedCMDPool; //设置充值信息处理成功
    CSDObjectPool<CSetTongBuPayInfo, CSDMutex>		m_oCSetTongBuPayCMDPool; //设置同步充值信息
    CSDObjectPool<CSetTongBuPayProcessed, CSDMutex>	m_oCSetTongBuPayProcessedCMDPool; //设置充值信息处理成功
    CSDObjectPool<CActivityEncourageCMD, CSDMutex>	m_oCActivityEncourageCMDPool; //活动奖励信息处理成功
    CSDObjectPool<CUpdateCdKeyCmd, CSDMutex>			m_oUpdateCdKeyCMDPool; //活动奖励信息处理成功
    CSDObjectPool<CSetMyPayProcessed, CSDMutex>		m_oSetMyPayProcessedCMDPool; //我方充值
    CSDObjectPool<CSaveFactionDataCMD, CSDMutex>		m_oSaveFactionCMDPool; //活动奖励信息处理成功
    CSDObjectPool<CGetGsNoticeCMD, CSDMutex>		m_oGetGsNoticeCMDPool; //获取gs公告
    CSDObjectPool<CGetOprateingActivityCMD, CSDMutex>	m_oGetOprateingActivityCMDPool; //获取运营活动配置
    CSDObjectPool<CCreateFactionDataCMD, CSDMutex>	m_oCreateFactionDataCMDPool; //获取运营活动配置
    CSDObjectPool<CSavePlayerConsume, CSDMutex>		m_oSavePlayerConsumeCMDPool; //保存玩家消费记录
    CSDObjectPool<CGetPlayerConsume, CSDMutex>		m_oGetPlayerConsumeCMDPool; //保存玩家消费记录
    CSDObjectPool<CSetCenterPayProcessed, CSDMutex>	m_oSetCenterPayProcessedCMDPool; //充值中心
    CSDObjectPool<CGetPrivateMsgLogCmd, CSDMutex>	m_oGetPrivateMsgLogCMDPool; //获取聊天记录
    CSDObjectPool<CSavePrivateMsgLogCmd, CSDMutex>	m_oSavePrivateMsgLogCMDPool; //获取聊天记录
    CSDObjectPool<CSaveCourageDataCMD, CSDMutex>		m_oSaveCourageCMDPool; //保存勇气相关数据
    CSDObjectPool<CLeaveFactionDataCMD, CSDMutex>   m_oLeaveFactionDataCMDPool;	//退出门派
    CSDObjectPool<CSetRecreatePayProcessed, CSDMutex>   m_oSetRecreateProcessedCMDPool;	//回档
    CSDObjectPool<CSaveGvGVideoCMD, CSDMutex>       m_oSaveGvGVideoCMDPool;	//保存gvg录像
    CSDObjectPool<CSaveGvGActivityCMD, CSDMutex>            m_oSaveGvGActivityCMDPool;	//保存门派跨服战活动数据
	CSDObjectPool<CSavePlayerSnapShotCMD, CSDMutex> m_oSavePlayerSnapShotCMDPool;	//保存玩家的快照数据
    CSDObjectPool<CDelPlayerSnapShotCMD, CSDMutex> m_oDelPlayerSnapShotCMDPool;	//保存玩家的快照数据
	CSDObjectPool<CSaveGvGBuildCMD, CSDMutex>       m_oSaveGvGBuildCMDPool;//GVG建筑信息
	CSDObjectPool<CDeleteOverGvGift, CSDMutex>      m_oDeleteGvGiftCMDPool;//去除过期GIFT
	CSDObjectPool<CAddGvGiftCMD, CSDMutex>          m_oAddGvGiftCMDPool;
	CSDObjectPool<CAddPlayersLogCMD, CSDMutex>      m_oAddPlayersLogCMDPool;
	CSDObjectPool<CDelPlayerLogCMD, CSDMutex>       m_oDelPlayersLogCMDPool;
    CSDObjectPool<CSaveGvGArenaCMD, CSDMutex>       m_oSaveGvGArenaCMDPool;	//保存门派跨服战的赛区数据
    CSDObjectPool<CClearGvGVideoCMD, CSDMutex>      m_oClearGvGVideoCMDPool;	//清空门派对战录像
    CSDObjectPool<CClearGvGActivityCMD, CSDMutex>      m_oClearGvGActivityCMDPool;	//清空上一届门派跨服战的活动数据
	CSDObjectPool<CSetReadPrivateMsgCMD, CSDMutex>  m_oSetReadPrivateMsgCMDPool;//设置已读私聊信息
	CSDObjectPool<CAddPlayerGiftCMD, CSDMutex> m_oAddPlayerGiftCMDPool;//添加玩家个人礼包
	CSDObjectPool<COperatingActivityCMD, CSDMutex> m_oOperatingActivityCMDPool;//操作活动时间

    CFactionDiffMap			m_mapFactionSaveDiff;
    CPlayerDiffMap			m_mapPlayerSaveDiff; //用于比较是否需要保存
    CSDMutex					m_oDiffMutex;
    UINT64					m_qwLastFillQueueTick; // 上次保存玩家的时间
    UINT64					m_qwLastSaveTick; //上次保存玩家的tick
    BOOL						m_bSaveProcessed;
    CID2PlayerVec				m_vecPlayerCache;

	CSDMutex           m_oSaveMutex;//保存间隔
	CMapSavePlayerTime m_mapSavePlayerTime;

    //记录保存错误信息
    CSDMutex			m_muErrorInfo;
    CErrorInfoMap		m_mapErrorInfo;
};

#endif

