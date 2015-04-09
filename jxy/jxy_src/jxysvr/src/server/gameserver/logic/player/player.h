
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <sdtime.h>
#include <sdstring.h>
#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <common/client/errdef.h>

#include <logic/battle/battleobj.h>
#include <logic/bag/bag.h>
#include <logic/instance/instancemgr.h>


#include <logic/enemy/enemy.h>
#include <logic/skill/skillmgr.h>
#include <logic/base/logicfunc.h>


#include <logic/shmem/shmemmap.h>

#include <logic/faction/factionplayer.h>
#include <logic/friendsys/sendflowerpropmgr.h>
#include <logic/faction/faction.h>
#include <logic/equipmaster/equipmaster.h>


#include "playergetdatamgr.h"
#include <logic/mark/mark.h>
using namespace std ;


struct tagUpdateData;
typedef tagUpdateData SUpdateData;

class CEatBread;
class CCourage;
class CBossBattle;
class CTask;
class CCourageGroup;
class CRace;
class CPlunder;
class CBuild;
class CHero;
class CFriends;
class CBegBlue;
class CRdChallenge;
class CCollectGas;
class CWorship;
class CClimbTower;
class CScore;
class CGatherScience;
class CPhystrength;
class CFormation;
class CVipExtData;
class CYabiaoInfo;
class CJingJie;
class CBuild;
class CMoneyTree;
class CCrop;
class CScienceTree;
class CLuaActivity;
class CActivityEncourage;
class CEquipCompse;
class CLocale;
class CEquipMaster;
class CCrop;
class CFewParam;
class CState;
class COnhook;
class CAwaken;
class COnlineEncourageRecord;
class CLoginEncourageRecord;
class CJewelry;
class CPlayerGVG;
class CPlayerSoul;
class CPlayerMedal;
class CPlayerPet;
struct	SMaintownLevyProp;

struct	tagDT_BATTLE_ATTRIBUTE;
typedef	tagDT_BATTLE_ATTRIBUTE DT_BATTLE_ATTRIBUTE;

#define COACHHERO	1 //主将

typedef HashMap<UINT16, UINT16> CItemKind2IdxMap; //key为道具类型，value为该类型道具的序号
typedef CItemKind2IdxMap::iterator CItemKind2IdxMapItr;


typedef HashMap<UINT16, CEquip*>CIdx2EquipMap;//key为Idx
typedef CIdx2EquipMap::iterator CIdx2EquipMapItr;
typedef HashMap<UINT16, CIdx2EquipMap> CKindID2EquipMMap; //key为装备类型
typedef CKindID2EquipMMap::iterator CKindID2EquipMMapItr;

typedef map<UINT32, CEnemy*> CEnemyID2EnemyMap;	//用map排序，方便迭代器++获取仇敌，不需每个查找
typedef CEnemyID2EnemyMap::iterator CEnemyID2EnemyMapItr;
typedef vector<CEnemy*> CEnemyVec;
typedef CEnemyVec::iterator CEnemyVecItr;

typedef HashMap<UINT16, CSkill*>CSKillID2SkillMap;//key为技能ID
typedef CSKillID2SkillMap::iterator CSKillID2SkillMapItr;

typedef CShmemMap<UINT8, CTask> CLineIdx2TaskShmemMap; //key为TaskLine
typedef CShmemMap<UINT32, CEquip> CPlayerEquipShmemMap;//key为KindID+Idx
typedef CShmemMap<UINT32, CJewelry> CPlayerJewelryShmemMap;

typedef CShmemMap<UINT16, CHero> CKindID2HeroShmemMap;
typedef CShmemMap<UINT8, CHero> CFormationHeroShmemMap; //有交换，size+1

class	CUser;
struct	SGetPlayerDataAck;
struct	SSceneInstanceRecord;
class	COnhookMgr;
class CPlayer: public CBattleObj
{
    typedef VOID (CPlayer::*DestoryObj) ();
    typedef vector<DestoryObj>		CDestoryVec;

    typedef CDestoryVec::iterator	CDestoryVecItr;
public:
    CPlayer();
    ~CPlayer();

////////////////////////////////////继承相关////////////////////////////////////
public:

    virtual VOID StartBattle(CBattleObj* poEnemy);//每次战斗前(清除奖励记录等等)
    virtual EBattleObjType GetBattleObjType()
    {
        return EBOT_PLAYER;
    }
    virtual VOID EndBattle(); //每次战斗后(清除战斗过程中的产出 buff,给战胜等)

    virtual VOID GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA);//赋值战斗组战斗者属性


    virtual VOID GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA);//赋值战斗组战斗者属性

    UINT32 GetPlayerID();
////////////////////////////////////玩家相关////////////////////////////////////

public:
	CPet* GetBattlePet();
    UINT16 EnterInstance(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byNewFlag = 0); //每次进入副本前的条件检查及各种属性计算，产生悬挂物/怪物，返回0进入成功，非0错误
    VOID	ExitInstance();//每次退出副本前的各种属性计算(清除为处理的悬挂物等等)

public:
    BOOL Init(DT_PLAYER_DATA& stDT_PLAYER_DATA);//初始化相关对象
    BOOL InitDT_PLAYER_TODAY_CONSUME_DATA();
    BOOL GetDT_PLAYER_BASE_DATA_CLI(DT_PLAYER_BASE_DATA_CLI& stPlayerInfo); //获取玩家基本数据，用于各种协议，玩家数据变化通知

    const DT_PLAYER_BASE_DATA& GetDT_PLAYER_BASE_DATA()
    {
        return m_stDT_PLAYER_BASE_DATA;
    }
	DT_PLAYER_TODAY_CONSUME_DATA& GetDT_PLAYER_TODAY_CONSUME_DATA()
	{
		return m_stDT_PLAYER_TODAY_CONSUME_DATA;
	}
    DT_EXCHANGE_RESULT_LOG &GetDT_EXCHANGE_RESULT_LOG()
    {
        return m_stDT_EXCHANGE_RESULT_LOG;
    }
    CUser*	GetUser();
    BOOL		SendMsg(const CHAR* pData, UINT16 wMsgID);
    VOID		OnLogin(BOOL bLoginFlag = TRUE);
    UINT64	GetUpdateTime()
    {
        return m_qwUpdateTime;
    }
    VOID    SetUpdateTime(UINT64 qwUpdateTime = SDTimeSecs())
    {
        m_qwUpdateTime = qwUpdateTime;
    }
    BOOL		CkLogin()
    {
        return m_bLoginFlag;
    }
    VOID		SetDisconnectTime()
    {
        m_qwLastDisconnectTime = SDTimeSecs();
    }
    UINT16	RegisterDspName(CHAR* pszDspName);//注册昵称，并通知LS更新User表
    UINT64	GetTodayFirstLoginTime()
    {
        return m_stDT_PLAYER_BASE_DATA.qwTodayFirstLoginTime;
    }
    BOOL		IsRobot()
    {
        return m_stDT_PLAYER_BASE_DATA.byRobotFunc > 0 ? TRUE : FALSE;
    }
    VOID		SetSaveState(ESaveState eSaveState )
    {
        m_eSaveState = eSaveState;
        if(ESS_WANT_SAVE == eSaveState) SetUpdateTime();
    };
    ESaveState GetSaveState()
    {
        return m_eSaveState;
    }
    UINT32	GetID()
    {
        return m_dwPlayerID;
    }

    UINT32	GetOriginalZoneID()
    {
        return m_stDT_PLAYER_BASE_DATA.wOriZoneID;
    }

    void	SetOriZoneID(UINT16 wOriZoneID)
    {
        m_stDT_PLAYER_BASE_DATA.wOriZoneID = wOriZoneID;
    }

    VOID		RecordPlayerOnOffLine( BOOL bLogin );

    BOOL		CkOpenFunc();//检查开发功能是否完整，不完整则补全

    CHAR *GetLastOrderID()
    {
        return m_aszOrderID;    //获得上次处理完毕的订单号
    }
    UINT8 *GetNoticeID()
    {
        return m_abyNoticeID;
    }
    UINT64 GetLoginTime()
    {
        return m_qwLoginTime;
    }
    UINT64 GetLogoffTime()
    {
        return m_qwLogoffTime;
    }
    void ResetLoginTime(UINT64 qwLoginTime = SDTimeSecs());
    void ResetLogoffTime(UINT64 qwLogoffTime = SDTimeSecs());
    BOOL IsAddOnlineTime()
    {
        return m_bAddOnlineTime;
    }

    VOID GetCliVerInfo(UINT32& dwCliVer, UINT32& dwResVer, UINT32& dwUIVer, TCHAR aszResVer[MAX_RES_VER_LEN]);
    VOID GetDT_EXT_FUNC_PARAM_LST(DT_EXT_FUNC_PARAM_LST& stExtFuncParamLst);

    BOOL& GetNewPlayerFlag()
    {
        return m_bNewPlayerFlag;
    }
    UINT64& GetLastInsertTime()
    {
        return m_qwLastInsertTime;
    }

    UINT32 GetCliVer();

    UINT32 GetAuthType();
    UINT32 GetFactionID();

    // 获取玩家所在门派的图标ID，未加入门派则返回：NULL_FACTION_ICON_ID
    BYTE GetFactionIconID();
	UINT32 GetDaiChong();
	//GM功能：改名
	VOID ChangeNameByGM(TCHAR* aszDisName);
    DT_PLAYER_POS_INFO* GetPositionHotSpring();
    CFaction* GetFaction();
	///////////////////////////////////////////////////////////////////////////资源操作
    VOID AddReward(UINT8 byRewardType, UINT32 dwRewardValue, UINT16 wOpType);
	BOOL CheckRes(UINT8 byResType, UINT64 qwResValue);
	///////////////////////////////////////////////////////////////////////////
    UINT64 GetSyncReward(UINT8 byRewardType);
    UINT32 GetFactionID(UINT32 dwPlayerID);
	VOID GetSyncResource(DT_RSYNC_RES_ITEM_DATA& stData);
	UINT16 OpenPlayerPanel(DT_PLAYER_INFO_PANEL& stPanel, DT_BAG_DATA_CLI& stBagInfo);

	UINT16 OpenPlayerPanel_v410(PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410&);
	UINT16 OpenHeroSelectPanel(PKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK& stAck);

    // 主角是否已飞升
    BOOL IsFly();

    // 飞升指定伙伴（包括主角）
    UINT16 Fly(UINT16 wHeroID, PKT_CLIGS_FLY_ACK&);

    UINT64 GetCoin(){ return m_stDT_PLAYER_BASE_DATA.qwCoin;}
    UINT64 GetGold(){ return m_stDT_PLAYER_BASE_DATA.qwGold;}

public:
    UINT16	GetCoachDefaultSkillActionID();
    CSkill*	GetCoachSkill();
    UINT8	GetCoachCareerID();

    UINT16	OpenSkillTab(DT_SKILL_TAB_DATA_CLI& stSkillTabInfo);
    UINT16	UpgradeSkillAction(UINT16 ActionID, UINT16 wActionLevelCli, DT_SKILL_TAB_DATA_CLI& stSkillTabInfo);
    UINT16	SetActionEffectRangProp(UINT16 wActionID, UINT16 wEffectID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind, DT_SKILL_TAB_DATA_CLI& stSkillTabInfo);



protected:
    BOOL	SyncCacheData();//同步缓存中的数据

    //VOID CkDataFortest();
    VOID	CkData();
    //VOID	CkExtData();//判断扩展字段，计算内部数值转换等等，如1表示0829给玩家补偿仙露/器魂需求

    VOID	EncGemBulegasGodweapon();
public:
    UINT32&	GetExtData()
    {
        return m_stDT_PLAYER_BASE_DATA.dwExtData;
    }
private:
    DT_PLAYER_BASE_DATA			m_stDT_PLAYER_BASE_DATA; //玩家基本信息
    DT_PLAYER_TODAY_CONSUME_DATA	m_stDT_PLAYER_TODAY_CONSUME_DATA;//玩家的当天消费记录
    DT_EXCHANGE_RESULT_LOG			m_stDT_EXCHANGE_RESULT_LOG;
    UINT32				m_dwPlayerID;//玩家ID，用于缓存，不需每次获取
    CHAR					m_aszOrderID[MAX_ORDER_ID_LEN]; //上次处理的订单号
    BOOL					m_bLoginFlag;//是否登录在线,主要用于判断是否是被挑战Init的Player还是自己上线登录
    UINT64				m_qwUpdateTime;//更新时间(被挑战也更新)
    UINT64				m_qwLastDisconnectTime;//上次断开连接时间
    //UINT64	m_qwTodayFirstLoginTime;//今天第一次登录时间
    ESaveState			m_eSaveState;//保存状态，用于非登录玩家的保存判断
    UINT8				m_abyNoticeID[MAX_TOKEN_NUM]; //玩家消息推送ID
    UINT64				m_qwLoginTime;//最后上线时间
    UINT64				m_qwLogoffTime;//最后离线时间
    BOOL					m_bAddOnlineTime; //是否记录在线时间
    BOOL					m_bNewPlayerFlag;//是否新玩家
    UINT64				m_qwLastInsertTime;//新玩家上次数据插入时间，用于避免多次重复插入及超时重试插入判断
    UINT32				m_dwCliVer;//客户端版本


////////////////////////////////////
    CDestoryVec			m_vecDestory;





////////////////////////////////////为了方便调用及减少函数出入栈，设置friend////////////////////////////////////
    friend class COnhookMgr;
    friend class CPlayerSaveMgr;

////////////////////////////////////Set相关////////////////////////////////////
#include "playerset.h"

////////////////////////////////////武将相关////////////////////////////////////
#include "playerhero.h"

////////////////////////////////////道具/背包相关////////////////////////////////////
#include "playeritem.h"

////////////////////////////////////任务相关////////////////////////////////////
#include "playertask.h"

////////////////////////////////////战斗相关////////////////////////////////////
#include "playerbattle.h"

////////////////////////////////////建筑相关////////////////////////////////////
#include "playerbuild.h"

////////////////////////////////////仇敌相关////////////////////////////////////
#include "playerenemy.h"

////////////////////////////////////提示相关////////////////////////////////////
#include "playerprompt.h"

////////////////////////////////////属性升级相关////////////////////////////////////
#include "playerupdateattr.h"

////////////////////////////////////活动相关////////////////////////////////////
#include "playeractivity.h"

////////////////////////////////////引导相关////////////////////////////////////
#include "playerguide.h"

	////////////////////////////////////副本相关////////////////////////////////////
	DECLARE_INIT_DESTORY(Instance, CInstance, DT_INSTANCE_DB_DATA)

	////////////////////////////////////挂机存着////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(Onhook, COnhook, DT_ONHOOK_RCD_DATA)

	////////////////////////////////////位置相关////////////////////////////////////
	DECLARE_INIT_DESTORY(Locale, CLocale, DT_LOCALE_DATA)

	////////////////////////////////////状态相关////////////////////////////////////
	DECLARE_INIT_DESTORY(State, CState, DT_STATE_DATA)

	////////////////////////////////////装备合成相关////////////////////////////////////
	DECLARE_INIT_DESTORY(EquipCompse, CEquipCompse, DT_EQUIP_COMPOSE_DATA)

	////////////////////////////////////悟道相关////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(Awaken, CAwaken, DT_AWAKEN_DATA)

	////////////////////////////////////技能种植////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(Crop, CCrop, DT_CROP_DATA)

	////////////////////////////////////体力相关////////////////////////////////////
	DECLARE_INIT_DESTORY(Phystrength, CPhystrength, DT_PHYSTRENGTH_DATA)

	////////////////////////////////////稀少参数相关////////////////////////////////////
	DECLARE_INIT_DESTORY(FewParam, CFewParam, DT_FEW_PARAM_DATA)

	////////////////////////////////////科技树相关////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(ScienceTree, CScienceTree, DT_SCIENCE_TREE_DATA)

	////////////////////////////////////整型相关////////////////////////////////////
	DECLARE_INIT_DESTORY(Formation, CFormation, DT_NEW_FORMATION_DATA)

	////////////////////////////////////押镖相关////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(YabiaoInfo, CYabiaoInfo, DT_YABIAO_DATA)

	////////////////////////////////////境界相关////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(JingJie, CJingJie, DT_JINGJIE_DATA)

	////////////////////////////////////爬塔相关////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(ClimbTower, CClimbTower, DT_CLIMB_TOWER_DATA)

	////////////////////////////////////VIP扩展相关////////////////////////////////////
	DECLARE_INIT_DESTORY(VipExtData, CVipExtData, DT_VIP_EXT_DATA)

	////////////////////////////////////冲值奖励相关////////////////////////////////////
	DECLARE_INIT_DESTORY(ActivityEncourage, CActivityEncourage, DT_ACTIVITY_ENCOURAGE_DATA)

	////////////////////////////////////评分奖励相关////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(Score, CScore, DT_SCORE_DATA_LST)

	////////////////////////////////////聚气相关////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(CollectGas, CCollectGas, DT_COLLECT_GAS_DATA)

	////////////////////////////////////摇钱树////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(MoneyTree, CMoneyTree, DT_MONEY_TREE_DATA)

	////////////////////////////////////在线奖励////////////////////////////////////
	DECLARE_INIT_DESTORY(OnlineEncourageRecord, COnlineEncourageRecord, DT_ONLINE_ENCOURAGE_RECORD_DATA)

	////////////////////////////////////登录奖励////////////////////////////////////
	DECLARE_INIT_DESTORY(LoginEncourageRecord, CLoginEncourageRecord, DT_LOGIN_ENCOURAGE_RECORD_DATA)

	////////////////////////////////////聚魂////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(GatherScience, CGatherScience, DT_GATHER_SCIENCE_DATA)

	////////////////////////////////////求仙露////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(BegBlue, CBegBlue, DT_BEGBLUE_DATA)

	////////////////////////////////////好友系统////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(Friends, CFriends, DT_FRIENDS_DATA)

	////////////////////////////////////玩家门派数据////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(FactionPlayer, CFactionPlayer, DT_PLAYER_FACTION_DATA)

	////////////////////////////////////装备精通////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(EquipMaster, CEquipMaster, DT_EQUIP_MASTER)

	///////////////////// 脚本活动 /////////////////////////////
	DECLARE_INIT_DESTORY(LuaActivity, CLuaActivity, DT_LUAACTIVITY_RECORD_LIST_DATA)

	///////////////////// BOSS战 /////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(BossBattle, CBossBattle, DT_BOSS_BATTLE_PLAYER_DATA_INFO)

	///////////////////// 勇气 /////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(Courage, CCourage, DT_COURAGE_DATA)

	///////////////////// 吃包子 /////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(EatBread, CEatBread, DT_EAT_BREAD_DATA)

	/////////////////////////GVG/////////////////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(PlayerGVG , CPlayerGVG, DT_PLAYER_GVG_DATA)
	////////////////////////战魂////////////////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(PlayerSoul , CPlayerSoul, DT_PLAYER_SOUL_DATA)
	////////////////////////勋章//////////////////////////////////////////////////
	DECLARE_BUILD_INIT_DESTORY(PlayerMedal , CPlayerMedal, DT_PLAYER_MEDAL_DATA)
	// 宠物
	DECLARE_BUILD_INIT_DESTORY(PlayerPet , CPlayerPet, DT_PLAYER_PET_DATA)
};

DECLARE_CLASS_POOL_MGR(CPlayer)

#endif //#ifndef _PLAYER_H_

