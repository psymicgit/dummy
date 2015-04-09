#ifndef _RECORD_MGR_H_
#define _RECORD_MGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <sdstring.h>
#include <dll/sdu/sdthread.h>
#include <deque>
#include <map>
#include <list>
#include "recordmodule.h"
#include <dll/sddb/sddb.h>


#define ADD_STORY_STR "addstory"
#define DEC_STORY_STR "decstory"
#define ADD_COIN_STR "addcoin"
#define DEC_COIN_STR "deccoin"
#define ADD_GOLD_STR "addgold"
#define DEC_GOLD_STR "decgold"
#define ADD_SCIENCE_STR "addscience"
#define DEC_SCIENCE_STR "decscience"
#define ADD_PHYSTRENGTH_STR "addphystrength"
#define DEC_PHYSTRENGTH_STR "decphystrength"
#define ADD_BLUEGAS_STR "addbluegas"
#define DEC_BLUEGAS_STR "decbluegas"
#define ADD_PURPLEGAS_STR "addpurplegas"
#define DEC_PURPLEGAS_STR "decpurplegas"
#define ADD_ITEM_STR "additem"
#define DEC_ITEM_STR "decitem"
#define ADD_STUDY_EXP_STR "addstudyexp"
#define DEC_STUDY_EXP_STR "decstudyexp"
#define ADD_STUDY_STR "addstudy"
#define DEC_STUDY_STR "decstudy"
#define ADD_JINGJIE_STR "addjingjie"
#define DEC_JINGJIE_STR "decjingjie"

#define RECORD_CHART_MAX_VIP   12       // 在做资源消耗统计报表时，玩家vip如果超过12级，则归入vip12+那类
#define RECORD_CHART_MAX_LEVEL 120      // 在做资源消耗统计报表时，玩家如果超过120级，则归入120+级那类

#define RECORD_CHART_LEVEL_SECTION_CNT ((RECORD_CHART_MAX_LEVEL / 10) + 1)  // 在做资源消耗统计时，玩家的等级共分为13个区间：[1-9]、[10-19]、[20-29].........[110-119]、[120+]
#define RECORD_CHART_VIP_SECTION_CNT   (RECORD_CHART_MAX_VIP + 2)           // 在做资源消耗统计时，vip等级分为14个级别：vip0、vip1、vip2、vip3.....vip12、vip12+


class CNGString;

//模块的枚举
enum ERecordModule
{
    //阅历
    ERM_ADDSTORY = 0,
    ERM_DECSTORY = 1,

    //铜钱
    ERM_ADDCOIN = 2,
    ERM_DECCOIN = 3,

    //器魂
    ERM_ADDSCIENCE = 4,
    ERM_DECSCIENCE = 5,

    //体力
    ERM_ADDPHYSTRENGTH = 6,
    ERM_DECPHYSTRENGTH = 7,

    //蓝气
    ERM_ADDBLUEGAS = 8,
    ERM_DECBLUEGAS = 9,

    //紫气
    ERM_ADDPURPLEGAS = 10,
    ERM_DECPURPLEGAS = 11,

    //元宝操作
    ERM_ADDGOLD = 12,
    ERM_DECGOLD = 13,

    //物品操作
    ERM_ADDITEM = 14,
    ERM_DECITEM = 15,

    //境界
    ERM_ADDJINGJIE = 16,
    ERM_DECJINGJIE = 17,

    //悟道心得经验
    ERM_ADDSTUDYEXP = 18,
    ERM_DECSTUDYEXP = 19,

    //悟道心得
    ERM_ADDSTUDY = 20,
    ERM_DECSTUDY = 21,

    //悟道碎片
    ERM_ADDSTUDYCLIP = 22,
    ERM_DECSTUDYCLIP = 23,

    // 勇气勋章
    ERM_ADDMEDAL = 24,
    ERM_DECMEDAL = 25,

    //门贡
    ERM_ADDDOORSTRIBUTE = 26,
    ERM_DECDOORSTRIBUTE = 27,

    //门派资金
    ERM_ADDFACTIONFUNDS = 28,
    ERM_DECFACTIONFUNDS = 29,

    // 玩家个人操作（比如，记载玩家退出哪个门派，或者玩家改名之类的）
    ERM_PLAYER_OPERATE = 30,

    // 门派跨服战
    ERM_GVG_ACTIVITY = 31,

	// 战魂
	ERM_ADDSOUL = 32,
	ERM_DECSOUL = 33,
	ERM_REFLASHSOUL = 34,

	// 宠物
    ERM_PETREP_ADDPET = 35, // 使用灵宠卡片唤出灵宠
    ERM_REFINE_ADDPET = 36, // 炼化添加灵宠（炼化功能，即合成）
    ERM_USEITEM_ADDPET = 37, // 使用灵宠蛋增加灵宠
    ERM_REFINE_REMOVEPET = 38, // 炼化移除灵宠（炼化功能，即合成）
    ERM_PETMERGE_REMOVEPET = 39, // 融合移除灵宠（融合功能，即吞噬）
    ERM_PET_UPSTAR = 40, // 灵宠升星
    ERM_PET_FIRSTDRAW = 41, // 灵宠喂食拉杆（喂食功能）
    ERM_PET_RESETDRAW = 42, // 灵宠喂食重摇（喂食功能）
    ERM_PET_CONFIRMFEED = 43, // 灵宠确认喂食（喂食功能）
    ERM_PET_MERGE = 44, // 融合灵宠（融合功能，即吞噬）
    ERM_PET_BUYSTOREITEM = 45, // 购买灵宠商城道具
    ERM_PET_WASHQUALITY = 46, // 灵宠洗资质
    ERM_PET_INHERIT = 47, // 灵宠继承
    ERM_GM_ADDPET = 48, // gm接口添加灵宠

    // 飞升功能
    ERM_FLY = 60,

	//勋章
	ERM_ADD_MEDAL = 61,//增加勋章
	ERM_DEC_MEDAL = 62,//勋章失效


};

typedef std::vector<UINT64> DiffVipHistoryVec;   // 不同vip等级的资源汇总, 对应{vip0级, vip1级, vip2级..... vip12+}的资源消耗
typedef std::vector<UINT64> DiffLevelHistoryVec; // 不同玩家等级区间的资源汇总, 对应{1-9级, 10级区, 20级区....., 120级+}的资源消耗

typedef std::vector<std::string> CSqlVec;

// 针对某个资源操作的消费统计
struct ConsumeRow
{
    ConsumeRow()
        : wModuleType(0)
        , wOpType(0)
        , qwOpTotal(0)
        , dwOpCnt(0)
        , dwMaxOpVal(0)
        , dwMinOpVal(0)
        , bSavedToDB(TRUE)
    {
        vecVipSumUp.resize(RECORD_CHART_VIP_SECTION_CNT);
        vecLevelSumUp.resize(RECORD_CHART_LEVEL_SECTION_CNT);
    }

    DiffVipHistoryVec vecVipSumUp;     // 各vip等级玩家的消费
    DiffLevelHistoryVec vecLevelSumUp; // 各等级区间玩家的消费

    UINT16 wModuleType;
    UINT16 wOpType;

    UINT64 qwOpTotal;       // 总额
    UINT32 dwOpCnt;         // 当天所有玩家在该类资源上共操作了多少次
    UINT32 dwMaxOpVal;      // 当天玩家的单笔最高记录，比如最高的一次得到了xxx元宝，或消费了xxx元宝
    UINT32 dwMinOpVal;      // 当天玩家的单笔最低记录，比如最少的一次得到了xxx元宝，或消费了xxx元宝

    BOOL bSavedToDB;         // 用于标识数据是否已同步到数据库
};

typedef std::map<UINT64, ConsumeRow> ConsumeChartMap;          // 资源消费map, key = 消费的资源类型 + 对应操作类型, val = 对应的资源消费
typedef std::map<UINT64, ConsumeChartMap> Day2ConsumeChartMap; // 各天的资源消费map, key = 天, val = 当天的消费统计情况

// 玩家消费统计报表，统计每天玩家对各类资源的消耗和获得情况，以及玩家都是从哪些途径获得这些资源
class CConsumeChartMgr
{
    DECLARE_SINGLETON(CConsumeChartMgr);

public:
    CConsumeChartMgr();

    BOOL Init();

public:
    // 统计玩家获得或消费资源的情况
    VOID CountUp(UINT16 wPlayerLevel, BYTE byVipLevel, UINT16 wModuleType, UINT16 wOpType, UINT32 dwOpValue, CSDDateTime &oOpTime);

    // 从数据库中取出今天的资源消费统计数据
    BOOL LoadFromDB();

    // 成功后把所有的未同步标志设为已同步，并清除昨天的统计记录
    void OnSaveDBSuccess();

    // 把消费统计存入数据库表（仅更新变动过的数据）
    BOOL SaveToDB();

    // 重置上次保存到db的时间为当前时间
    void ResetLastSaveTime();
    UINT64 GetLastSaveTime()
    {
        return m_qwLastSaveTime;
    }

private:
    // 获取更新所需的数据库字段sql语句，返回的结果形如：replace into ophistory(Date, ModuleType, OpType, OpValue...... vip0, vip1, vip2, ......, level_1_9, level_10, .......) values
    string& BuildInsertFieldSql();

    // 把需要更新的消费数据取出来拼接成sql语句，返回的字符串结果形如：("2013/12/10 00:00:00", 4, 5, 0, 0, 0.......), ("2013/12/10 00:00:00", 4, 5, 0, 0, 0.......),
    string BuildInsertValueSql();

    // 清除昨天的统计记录
    void ClearYesterday();

private:
    // 资源统计map（这个map的内存占用估计了下，在69KB上下，完全可以承受）
    Day2ConsumeChartMap m_mapRecord;

    // 上次存储到数据库的时间
    UINT64 m_qwLastSaveTime;

    // 锁，用于在拼接sql语句期间阻止数据更新
    CSDMutex m_mutex;
};


class CPlayer;


class CRecordMgr: public CSDThread
{
    DECLARE_SINGLETON(CRecordMgr);

    typedef map<INT32, CRecordModule*> MAP_RECORD_MODULE_TYPE;
public:
    CRecordMgr();
    virtual ~CRecordMgr();
    BOOL Init();
    VOID UnInit();

    	

    // 记录元宝操作
    BOOL RecordGold(UINT16 wZoneID, UINT32 dwPlayerID, UINT16 wPlayerLevel, UINT16 wVipLevel, ERecordModule wModuleType, UINT16 wOpType,
        UINT32 dwOpRMBGold, UINT32 dwOpRMBGoldGift, UINT32 dwOpGmGold, UINT32 dwOpGameGold, 
        UINT32 dwAfterRMBGold, UINT32 dwAfterRMBGoldGift, UINT32 dwAfterGmGold, UINT64 qwAfterGameGold, 
        UINT64 qwParam1 = 0, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0, string strComment = "");

    BOOL RecordInfo(UINT32 dwPlayerID, UINT16 wModuleType, UINT16 wOpType, UINT32 dwOpValue, UINT64 qwAfterOpValue, UINT16 wPlayerLevel, UINT16 wVipLevel,
                    UINT64 qwParam1 = 0, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0, string strComment = "");

    ISDDBSession* GetRecordDBSession()
    {
        return m_pDBRecordSession;
    }

private:
    const static UINT32 ms_dwSleepTime = 1; //睡眠的时间间隔
protected:
    //线程
    virtual VOID  ThrdProc();

private:
    VOID Stop();

    BOOL WriteDataToSqlServer();

    // 拆分出新的元宝记录表
    BOOL SplitNewGoldRecordTable(std::string &strNewTableName);

    // 将元宝操作记录存入数据库
    BOOL SaveGoldRecordToDB();    

    VOID AddSaveErrorSql(const string& strSql, CHAR szErrFileName[], BOOL bTrunc = FALSE);
    list<string> GetErrorSql(CHAR szErrFileName[]);

    VOID ExeErrFileSql();
    VOID ExeGoldErrFileSql();

private:
    BOOL m_bStop;
    MAP_RECORD_MODULE_TYPE m_mapRecordModule;

    time_t m_tLastWriteTime; //上次写入数据库的时间
    SGDP::ISDDBModule*  m_pDBRecordModule; //数据库记录模块
    SGDP::ISDDBSession* m_pDBRecordSession; //数据库连接
    CRecordModule*		m_poRecordModule;

    SGDP::ISDDBModule*  m_poGameDBModule;  // 游戏数据库的模块，用于创建和关闭与游戏数据库的会话
    SGDP::ISDDBSession* m_poGameDBSession; // 与游戏数据库的会话连接（注意，这个和CDBMgr中的游戏数据库连接是不一样的，而且也不能合用）

    CSqlVec m_vecGoldSql; // 存储玩家元宝消费记录的sql语句列表

    UINT32 m_dwCachedYear; // 现在的年月，如201407
public:

    //获得阅历
    enum EAddStoryType
    {
        EASTT_CROP = 1,		//种植
        EASTT_INSTANCE = 2, //副本获得
        EASTT_TASK = 3,		//任务奖励
        EASTT_GMCOMMAND = 4, //GM命令
        EASTT_ENCOURAGE = 5, //活动奖励
        EASTT_RATING_SCORE = 6, //完美通关
        EASTT_RATING_SCORE_ACCUMULATE = 7, //累积奖励
        EASTT_BEG_BLUE_GAS = 8, //求仙露所得
        EASTT_GETGIVEGM = 9, //资源操作GM

		EASTT_VIP_GIFT = 10,//VIP礼包

        EASTT_COURAGE = 11, //勇气试炼所得
		EASTT_GVG_GIFT = 12,//跨服战礼包 

    };

    //使用阅历
    enum EDecStoryType
    {
        EDST_UPDATESCIENCE = 1, //升级科技点,p2表示科技的类型,p3表示科技的等级
        EDST_BOSSB_INSPIREPOWER = 2, //boss战鼓舞
        EDST_GETGIVEGM = 3, //资源操作GM
		EDST_UPGRADE_FORMATION = 4, //升级阵法
		EDST_EXCHANGE_STORE = 5,//商店
    };

    //增加铜币
    enum EAddCoinType
    {
        EACT_USEGOODS = 1,			//使用物品获得获得
        EACT_ONLINEENCOURAGE = 2,	//在线奖励获得
        EACT_GMCOMMAND = 3,			//GM命令获得
        EACT_INSTANCE = 4,			//副本获得获得
        EACT_INSTANCEGOODS = 5,		//副本直接使用物品获得
        EACT_SELLGOODS = 6,			//出售物品获得获得
        EACT_SELLEQUIP = 7,			//出售装备获得
        EACT_GUIDEROCKMONEY = 8,		//招财进宝引导获得铜钱
        EACT_ROCKMONEY = 9,			//招财进宝获得铜钱
        EACT_CROP = 10,				//招财进宝获得铜钱
        EACT_PLUNDER = 11,			//俘虏
        EACT_RACECHALLENGE = 12,		//竞技场
        EACT_RANK = 13,				//排名奖励
        EACT_TASK = 14,				//任务
        EACT_YABIAO = 15,			//押镖
        EACT_EATBREAD = 16,			//吃包子
        EACT_USELOGINGOODS = 17,		//登录奖励使用物品
        EACT_AWAKEN = 18,			//悟道获得
        EACT_JIEBIAO = 19,			//劫镖
        EACT_LEVY = 20,				//征收
        EACT_BOSSB_HURTENC = 21,				//BOSS战 伤害奖励
        EACT_BOSSB_HEROENC = 22,				//BOSS战 英雄奖励
        EACT_BOSSB_TOPENC = 23,				//BOSS战 排名奖励
        EACT_BOSSB_KILLENC = 24,				//BOSS战 击杀奖励
        EACT_ENCOURAGE = 25,				//活动奖励
        EACT_RATING_SCORE = 26,				//完美通关奖励
        EACT_RATING_SCORE_ACCUMULATE = 27,	//星星累积奖励
        EACT_GETGIVEGM = 28,			//资源操作GM
		EACT_VIP_GIFT = 29,				//VIP礼包
		EACT_GVG_GIFT = 30,//跨服战礼包
		EACT_GVG_GAMBLE = 31,//押注
    };


    //使用铜币
    enum EDecCoinType
    {
        EDCT_CROP = 1,				//种植地块开启消耗铜钱，	param2表示地块的ID
        EDCT_UPDATEATTR = 2,		//属性升级消耗铜钱，		param2表示属性类型，param3表示强化后的属性级别
        EDCT_INCREASESIZE = 3,		//背包扩容消耗铜钱
        EDCT_SHOPBUY = 4,			//商店购买消耗铜钱，		p2表示TabID，p3表示ItemKindID，p4表示PileCount
        EDCT_AWAKEN = 5,			//悟道消耗铜钱
        EDCT_GODWEAPON = 6,			//器魂开启和升级消耗铜钱,p2表示伙伴ID，p3品质，p4表示level
        EDCT_STRENGTHEN = 7,		//装备强化消耗铜钱，p2表示伙伴ID，p3表示装备ID，p4表示强化后的等级
        EDCT_UPGASLEVEL = 8,		//境界升级气的等级消耗铜钱
        EDCT_GENGAS = 9,			//生成器魂消耗铜钱
        EDCT_RDCREFRESH = 10,		//随机挑战刷新消耗铜钱
        EDCT_WORSHIP = 11,			//膜拜消耗铜钱
        EDCT_ENCOURAGE = 12,		//活动奖励消耗铜钱
        EDCT_EXCHANGESTUDY = 13,		//兑换卦象消耗铜钱
        EDCT_GM = 14,				//GM消耗铜钱
        EDCT_GATHERSCIENCE = 15,		//聚魂消耗铜钱
        EDCT_GETGIVEGM = 16,		//资源操作GM
        EDCT_FACTION = 17,		//创建帮派消耗铜钱
        EDCT_VISITHERO = 21,		//招募武将
        EDCT_FACTION_DOORSTRIBUTE  = 22, //门贡
        EDCT_SEND_FLOWER  = 23, //送鲜花
        EDCT_DRINK_WINE = 24,		//饮酒
		EDCT_FIND_BACK_DOORSTRUBUTE = 25,	//找回门贡
		EDCT_VIP_GIFT = 26,//VIP礼包
		EDCT_GENERATE_JEWELRY = 27,//生成饰品
		EDCT_RESOLVE_JEWELRY = 28,//分解饰品
		EDCT_DEVOUR_SOUL = 29,//吞噬战魂卡
        EDCT_EQUIP_MAKE = 30, // 装备制作
		EDCT_EXCHANGE_STORE = 31,//商店
		EDCT_NORMAL_INHERIT = 32,// 普通继承
		EDCT_DEVOR_NEIDAN = 33,//吞噬内丹
    };

    //获得器魂
    enum EAddScienceType
    {
        EAST_CROP = 1, //种植获得
        EAST_TASK = 2, //任务获得
        EAST_RACECHALLENGE = 3, //竞技场挑战获得
        EAST_RACERANK = 4, //竞技场排名奖励
        EAST_USEGOODS = 5, //使用物品,p2为物品ID
        EAST_ONLINEENCOURAGE = 6, //在线奖励
        EAST_PLUNDER = 7, //俘虏
        EAST_GMCOMMAND = 8, //GM命令
        EAST_FREEGODWEAPON = 9, //释放神器
        EAST_USELOGINGOODS = 10, //自动使用登录物品奖励
        EAST_MELT = 11, //装备熔炼
        EAST_YABIAO = 12, //押镖
        EAST_ENCOURAGE = 13, //活动奖励
        EAST_RATING_SCORE = 14, //完美通关
        EAST_RATING_SCORE_ACCUMULATE = 15, //累积奖励
        EAST_GATHERSCIENCE = 16,//聚魂
        EAST_GAS = 17, //聚气
        EAST_GETGIVEGM = 18, //资源操作GM
		EAST_VIP_GIFT = 19, //VIP礼包
		EAST_GVG_GIFT = 20,//跨服战礼包
    };

    //使用器魂
    enum EDecScienceType
    {
        EDSCT_GODWEAPONUPGRADE = 1, //神器升级
        EDSCT_GETGIVEGM = 2, //资源操作GM
    };

    //获得体力
    enum EAddPhyStrengthType
    {
        EAPST_RECOVER = 1, //自动恢复体力
        EAPST_TASK = 2, //任务恢复体力
        EAPST_ONLINEENCOURAGE = 3, //在线奖励
        EAPST_ONLINEUSEGOODS = 4, //在线奖励直接使用物品
        EAPST_GMCOMMAND = 5, //GM命令
        EAPST_USEGOOD = 6, //使用物品
        EAPST_EATBREAD = 7, //吃包子
        EAPST_BUY = 8, //购买体力
        EAPST_LOGINUSRGOOD = 9, //登录奖励直接使用物品
        EAPST_ENCOURAGE = 10, //活动奖励
        EAPST_RATING_SCORE = 11, //完美通关
        EAPST_RATING_SCORE_ACCUMULATE = 12, //累积奖励
        EAPST_GETGIVEGM = 13, //资源操作GM
        EAPST_DRINKWINE = 14, //饮酒
		EAPST_VIP_GIFT = 15, //VIP礼包
		EAPST_GVG_GIFT = 16, //跨服战礼包 
    };

    enum EDecPhyStrengthType
    {
        EDPST_INSTANCE = 1, //过副本消耗
        EDPST_ONHOOK = 2, //挂机消耗体力
        EDPST_GETGIVEGM = 3, //资源操作GM
		EDPST_OUT_BOUND = 4,//过外传
    };

    //获得蓝气
    enum EAddBlueGasType
    {
        EABGT_JINGJIECOLLECT = 1, //境界中收集
        EABGT_GM = 2, //境界中收集
        EABGT_TASK = 3, //任务奖励
        EABGT_ENCOURAGE = 4, //活动奖励
        EABGT_GEMCONVERT = 5, //宝石转换
        EABGT_USEGOODS = 6, //物品使用
        EABGT_INSTANCEBURST = 7, //副本爆出
        EABGT_RATING_SCORE = 8, //完美通关
        EABGT_RATING_SCORE_ACCUMULATE = 9, //累积奖励
        EABGT_BEG_BLUE_GAS = 10, //求仙露所得
        EABGT_GETGIVEGM = 11, //资源操作GM
		EABGT_VIP_GIFT = 12,//VIP礼包
		EABGT_GVG_GIFT = 13,//跨服战礼包 
    };

    //使用蓝气
    enum EDecBueGasType
    {
        EDBGT_UPGRADESKILLACTION = 1,//升级技能
        EDBGT_TASK = 2,			//任务消耗
        EDBGT_CONVERT2PURPLE = 3, //转换紫气
        EDBGT_MAKERGEM = 4, //制作宝石
        EDBGT_GETGIVEGM = 5, //资源操作GM
    };

    //获得紫气
    enum EAddPurpleGasType
    {
        EAPGT_JINGJIECOLLECT = 1, //境界中收集
        EAPGT_GM = 2, //境界中收集
        EAPGT_CONVERT2PURPLE = 3, //转换紫气
        EAPGT_RDCHALLENGE = 4, //随机挑战奖励
        EAPGT_WORSHIP = 5, //膜拜
        EAPGT_ENCOURAGE = 6, //活动奖励
        EAPGT_USEGOODS = 7, //使用物品
        EAPGT_RATING_SCORE = 8, //完美通关
        EAPGT_RATING_SCORE_ACCUMULATE = 9, //累积奖励
        EAPGT_GETGIVEGM = 10, //资源操作GM
        EAPGT_TASK = 11, //任务奖励
        EAPGT_DRINK_WINE = 12, //喝酒
		EAPGT_VIP_GIFT = 13,//VIP礼包
		EAPGT_GVG_GIFT = 14,//跨服战礼包
    };

    //使用紫气
    enum EDecPurpleGasType
    {
        EDPGT_UPGRADESKILLACTION = 1,//升级技能
        EDPGT_GETGIVEGM = 2, //资源操作GM
		EDPGT_EXCHANGE_STORE = 3,//商店
    };

    //获得元宝
    enum EGetGoldType
    {
        EGGT_ADDFLAGEXCHANGE = 1,			//充值获得元宝
        EGGT_ADDFLAGCROWN = 2,				//征收获得元宝
        EGGT_ADDFLAGGM = 3,					//通过GM工具获得元宝
        EGGT_ADDGOLDTASK = 4,				//通过任务获得元宝
        EGGT_ADDGOLDINSTANCE = 5,			//副本获得元宝
        EGGT_ADDGOLDINSTANCEGOODS = 6,		//通过副本获得获得元宝
        EGGT_USEGOODS = 7,					//使用物品获得元宝
        EGGT_ONLINEENCOURAGE = 8,			//在线奖励获得元宝
        EGGT_USELOGINGOODS = 9,				//登录奖励使用物品获得元宝
        EGGT_ENCOURAGE = 10,				//活动奖励获得元宝
        EGGT_RATING_SCORE = 11,				//完美通关获得元宝
        EGGT_RATING_SCORE_ACCUMULATE = 12,	//累积奖励获得元宝
        EGGT_GETGIVEGM = 13,				//资源操作GM
        EGGT_GM_FLAGEXCHANGE = 14,			//GM做的充值效果
		EGGT_VIP_GIFT =  15,				//VIP礼包
		EGGT_GVG_GIFT = 16,//跨服战礼包
		EGGT_BAG_GIFT = 17,//背包补偿礼包
    };

    //使用元宝
    enum EDecGoldType
    {
        EDGT_CLRJIEBIAOCD = 1, //清除劫镖CD
        EDGT_OPENCROPFIELD = 2, //用元宝打开地块
        EDGT_CLRCROP = 3, //清除种植CD时间
        EDGT_BUYCLIMBTOWERRESETREQ = 4, //重置爬塔
        EDGT_CLRUSEDRUGCD = 5, //清除使用丹药CD
        EDGT_INCREASESIZE = 6, //扩展背包格子
        EDGT_CLRCHALLENGECD = 7, //清空挑战CD时间
        EDGT_ONRACECOMPLETE = 8, //收费挑战
        EDGT_BUY = 9, //商店购买
        EDGT_PLUNDER = 10, //收费掠夺
        EDGT_DRIVEAWAY = 11, //驱逐
        EDGT_BLEEDCAPTURE = 12, //榨干
        EDGT_ROCKMONEY = 13, //招财进宝
        EDGT_AWAKENONCE = 14, //悟道
        EDGT_STRENGTHENEQUIP = 15, //装备强化
        EDGT_FLUSHBIAOINFO = 16, //刷新镖信息
        EDGT_FLUSHBIAOINFOMAXLEVEL = 17, //直接刷成最好的镖品质
        EDGT_BUYPHYSTRENGTH = 18, //购买体力
        EDGT_GENGAS = 19, //境界
        EDGT_UPGASLEVEL = 20, //升级灵气等级
        EDGT_VISITHERO = 21, //招募武将
        EDGT_PERFECTFREEGODWEAPON = 22, //完美释放器魂
        EDGT_GOLDEXPERIENCE = 23, //服用元宝经验丹
        EDGT_GOLDRESETELITINSTANCE = 24, //重置精英副本
        EDGT_BOSSBINSPIRE = 25, //BOSS战鼓舞
        EDGT_BOSSBRELIVE = 26, //BOSS战复活
        EDGT_EXPTRANS = 27, //经验传承
        EDGT_CLRRDCCD = 28, //清除随机挑战CD
        EDGT_WORSHIP = 29, //膜拜
        EDGT_MELT = 30, //熔炼
        EDGT_COMPOSEEQUIP = 31, //装备合成
        EDGT_GM = 32, //GM
        EDGT_GMDECPAY = 33, //GM扣除
        EDGT_GATHER_SCIENCE = 34, //聚魂
        EDGT_BEG_BLUE_IMPRV_LUCK = 35, //求仙露改运消费
        EDGT_EQUIP_OPENLIGHT = 36, //装备开光消费
        EDGT_GETGIVEGM = 37,		//资源操作GM
        EDGT_FACTION = 38,		//创建帮派
        EDGT_FACTION_DOORSTRIBUTE  = 39, //门贡
        EDGT_SEND_FLOWER  = 40, //送鲜花
        EDGT_OPEN_GOLD_MODE = 41,   //开启黄金模式
        EDGT_CHANGE_MONSTER = 43,   //刷新捉妖的妖怪
        EDGT_DRINK_WINE = 44,		//饮酒
        EDGT_FIND_ORANGE_MONSTER = 45,  //一键橙怪
        EDGT_BUY_COURAGE_TIMES  = 46, // 购买勇气试炼
        EDGT_EQUIP_MASTER_CHEAT = 47, // 装备精通出千
        EDGT_CLOSE_MONSTER = 48, //收妖
		EDGT_CHANGE_FACTION_ICON = 49, //门派头像更换
		EDGT_FIND_BACK_DOORSTRIBUTE = 50,//找回门贡
		EDGT_VIP_GIFT = 51,//VIP礼包
        EDGT_CROPFIELDUPGRADE = 52, //用元宝升级地块
		EDGT_PRODUCT_SOUL = 53, // 将魂产出
		EDGT_REFLASH_SOUL = 54,//洗炼战魂
		EDGT_PRODUCT_PET = 55, // 宠物产出
		EDGT_FRUIT_RESET_DRAW = 56, // 水果普通抽取
		EDGT_FRUIT_MAX_DRAW = 57, // 水果一键抽取
		EDGT_BUY_PET_STORE_ITEM = 58, // 购买宠物商城物品
		EDGT_WASH_PSYCHIC = 59, // 洗资质
        EDGT_PERFECT_INHERIT = 60, // 完美继承
		EDGT_EQUIP_MAKE_BUY_MATERIAL = 61, // 装备制作时补齐材料
		EDGT_VIP_PET_FEED = 62, // 宠物VIP喂养
    };

    //增加物品
    enum EAddItemType
    {
        EAIT_MAKEGOODS = 1, //物品合成,p2表示强化物品的ID
        EAIT_CROP = 2, //种植获得
        EAIT_MIXDRUG = 3, //丹药合成
        EAIT_GMCOMMAND = 4, //GM命令
        EAIT_ONLINEENCOURAGE = 5, //在线奖励
        EAIT_LOGINENCOURAGE = 6, //登录奖励
        EAIT_BOX = 7, //宝箱奖励,p2表示宝箱ID
        EAIT_REMOVEXIANGQIAN = 8, //移除镶嵌
        EAIT_INSTANCE = 9, //副本奖励
        EAIT_TASK = 10, //任务奖励
        EAIT_BUY = 11, //商店购买获得
        EAIT_COMPOSEGOODS = 12, //物品合成
        EAIT_MAKEEQUIP = 13, //装备合成
        EAIT_REPLACEXIANGQIAN = 14, //镶嵌替换
        EAIT_RECHARGE_ENCOURAGE = 15, //充值奖励
        EAIT_SCORE_ENCOURAGE = 16, //评分奖励
        EAIT_ENCOURAGE = 17, //活动奖励
        EAIT_EQUIPCOMPOSE = 18, //装备合成
        EAIT_RATING_SCORE = 19, //完美通关
        EAIT_RATING_SCORE_ACCUMULATE = 20, //累积奖励
        EAIT_ITEMEXCHANGE = 21, //兑换物品，p1为兑换序号
        EAIT_GETGIVEGM = 22,				//资源操作GM
		EAIT_RAREITEM_EXCHANGE = 23, //门贡兑换
		EAIT_VIP_GIFT = 24,		//VIP礼包
        EAIT_RDCHALLENGE = 25,  //试炼场所得
        EAIT_SENDFLOWER = 26,  //送鲜花所得
        EAIT_COURAGE = 27,   //勇气试炼所得
		EAIT_JEWELRY = 28,//饰品合成
		EAIT_RESOLVE_JEWELRY = 29,//分解饰品
		EAIT_GVG_GIFT = 30,//跨服战礼包
		EAIT_GM = 31,//GM命令
		EAIT_SOUL_METERIAL = 32,// 将魂材料产出
		EAIT_PRODUCT_PET = 33,// 宠物产出物品
		EAIT_BUY_PET_STORE = 34,// 宠物商城购买的物品
        EAIT_EXCHANGE_STORE = 35,//商城兑换
        EAIT_EQUIP_MAKE = 36, // 装备制作
		EAIT_SELL_ENCHANT_EQUIP = 37, // 出售附魔装备
    };

    //使用物品
    enum EDecItemType
    {
        EDIT_MAKEGOODS = 2, //物品合成
        EDIT_UPDATEATTR = 3, //属性升级
        EDIT_USE = 4, //物品使用,p3表示消耗数量
        EDIT_CROP = 5, //种植,p2表示叠加数
        EDIT_CROPGUIDE = 6, //种植，扣除引导给的种子
        EDIT_MAKEEQUIP = 7, //制作装备,p2表示叠加数,p3表示消耗数量
        EDIT_EATDRUG = 8 , //服用经验丹,p2表示叠加数
        EDIT_XIANGQIAN = 9, //镶嵌武器,p2表示叠加数
        EDIT_SELL = 10, //出售物品,p2叠加数,p3表示出售数量
        EDIT_MELT = 11, //熔炼器魂, p2叠加数,p3表示装备强化等级
        EDIT_GM = 12, //熔炼器魂, p2叠加数,p3表示装备强化等级
        EDIT_EQUIPCOMPOSE = 13, //装备合成, p2叠加数,p3表示装备强化等级
        EDIT_OPENLIGHT = 14, //开光, p2叠加数,p3表示使用个数
        EDIT_GETGIVEGM = 15,				//资源操作GM
        EDIT_GODWEAPON = 16,				//神器进阶升级
		EDIT_RESOLVE_ORANGE = 17,			//分解黄卦道具
		EDIT_SEND_FLOWER = 18,			//送花
		EDIT_UPGRADE_JEWELRY = 19,		//强化饰品
		EDIT_GENERATE_JEWELRY = 20,		//合成饰品
        EDIT_SOUL_METERIAL = 20,		//合成战魂
        EDIT_EQUIP_MAKE = 21,		//装备制作
        EDIT_FLY = 22,		// 飞升
        EDIT_RECRUIT_LEGEND_HERO = 23,		// 招募外传伙伴
		EDIT_UPGRADE_HERO = 24,		// 进阶伙伴
		EDIT_UPDATE_POTENCY_SOUL = 25,//战魂卡升品
    };

    //获得境界点
    enum EAddJingJieType
    {
        EAJT_LEVY = 1, //征收
        EAJT_YABIAO = 2, //押镖获得
        EAJT_RACECHALLENGE = 3, //竞技场获得
        EAJT_INSTANCE = 4, //打怪获得境界
        EAJT_BOSSB_HURTENC = 5, //BOSS战 伤害奖励
        EAJT_BOSSB_HEROENC = 6, //BOSS战 英雄奖励
        EAJT_BOSSB_TOPENC = 7, //BOSS战 TOP奖励
        EAJT_BOSSB_KILLENC = 8, //BOSS战 击杀奖励
        EAJT_RDCHALLENGE = 9, //随机挑战奖励
        EAJT_WORSHIP = 10, //膜拜
        EAJT_ENCOURAGE = 17, //活动奖励
        EAJT_USEGOODS = 18, //使用物品
        EAJT_GM = 19, //gm命令
        EAJT_GMSET = 20, //gm命令设置
        EAJT_RATING_SCORE = 21, //完美通关
        EAJT_RATING_SCORE_ACCUMULATE = 22, //累积奖励
        EAJT_GETGIVEGM = 23, //资源操作GM
        EAJT_TASK = 23, //任务奖励
        EAJT_SEND_FLOWER = 24, //送鲜花
		EAJT_VIP_GIFT = 25,//VIP礼包
		EAJT_RACE_RANK = 26, //竞技场排名奖励
		EAJT_GVG_GIFT = 27,//跨服战礼包
    };

    //使用境界点
    enum EDecJingJieType
    {
        EDJT_UPJINGJIE = 1, //境界升级
        EDJT_GETGIVEGM = 2, //资源操作GM
		EDJT_EXCHANGE_STORE = 3,//境界
    };

    //获得心得经验
    enum EAddStudyExpType
    {
        EASET_CONVERTSTUDYEXP = 1, //转换心得
        EASET_GMCOMMAND = 2, //GM命令
        EASET_ENCOURAGE = 3, //活动奖励
        EASET_RATING_SCORE = 4, //完美通关
        EASET_RATING_SCORE_ACCUMULATE = 5, //累积奖励
    };

    //使用心得经验
    enum EDecStudyExpType
    {
        EDSET_UPGRADESTUDY = 1, //升级心得,p2心得类型，p3心得颜色，p4心得等级
    };

    //获得心得
    enum EAddStudyType
    {
        EASTUDYT_AWAKEN = 1, //悟道,p2为color,p3为level
        EASTUDYT_GMCOMMAND = 2, //GM命令,p2为color,p3为level
        EASTUDYT_ENCOURAGE = 3, //活动奖励
        EASTUDYT_RATING_SCORE = 4, //完美通关
        EASTUDYT_RATING_SCORE_ACCUMULATE = 5, //累积奖励
    };

    enum EDecStudyType
    {
        EDSTUDYT_CONVERT = 1, //经验转换,p2为color,p3为level
    };

    //获得心得
    enum EAddStudyClipType
    {
        EASCTUDYTCLIP_AWAKEN = 1, //悟道,p2为color,p3为level
        EASCTUDYTCLIP_GETGIVEGM	= 2, //资源操作GM
        //EASCTUDYTCLIP_HUNTING = 3,	//捉妖奖励
        EASCTUDYTCLIP_ENCOURAGE = 4,	//活动奖励
		EASCTUDYTCLIP_RESOLVE_ORANGE = 5, //分解黄色卦象
		EASCTUDYTCLIP_VIP_GIFT = 6,//VIP礼包 
		EASCTUDYTCLIP_GVG_GIFT = 7,//GVG礼包
    };

    enum EDecStudyClipType
    {
        EDSCTUDYTCLIP_EXCHANGE = 1, //经验兑换
        EDSCTUDYTCLIP_CONVERT = 2, //资源操作GM
		EASCTUDYTCLIP_HUNTING = 3,	//捉妖奖励
    };

    // 获得勇气勋章
    enum EAddMedalType
    {
        EAMT_GM = 2, // 通过GM工具获得勇气勋章
        EAMT_COURAGE = 3, //通过勇气试炼所得
    };

	// 获得勇气勋章
	enum EAddSoulType
	{
		EAST_GM			= 1,	// 通过GM工具获得勇气勋章
		EAST_Product	,		// 探索
		EAST_Exchange	,		// 兑换
	};

    // 使用勇气勋章
    enum EDecMedalType
    {
        EDMT_ACTIVATE_EQUIP_MASTER     = 1, // 装备精通激活
        EDMT_BREAK_THROUGH_MASTER      = 2, // 装备精通突破
        EDMT_GET_EQUIP_MASTER_EXP_BOOK = 3, // 装备精通培养精通之书
    };

    enum EAddDoorsTribute //获得门贡
    {
        EAT_HUNTING = 1,	//捉妖
        EAT_DONATE_COIN = 2,		//捐献铜钱
        EAT_DONATE_GOLD = 3,		//捐献元宝
        EAT_GM = 4,	//GM操作
		EAT_USE_GOODS = 5, //使用物品（门贡令牌）
		EAT_FIND_BACK = 6, //门贡找回
		EAT_VIP_GIFT = 7,//VIP礼包获得
		EAT_GVG_GIFT = 8,//GVG礼包
		EAT_GVG_CONTINUE_KILL = 9,//GVG连斩奖励
    };

    enum EDecDoorTribute	//使用门贡
    {
        EDDT_LEARN_SKILL = 1,	//学习武堂技能
        EDDT_BUY_RAREITEM = 2,	//珍品坊兑换珍品
        EDDT_GM = 3,	//GM
    };

    enum EAddFactioinFunds
    {
        EAFF_DONATE_COIN = 1,	//铜钱捐献
        EAFF_DONATE_GOLD = 2,	//元宝捐献
        EAFF_HUNTING = 3,	//捉妖
        EAFF_GM = 4,		//GM
		EAFF_VIP_GIFT = 5, //VIP礼包获得
		EAFF_GVG_GIFT = 6,//GVG礼包
    };

    enum EDecFactionFunds
    {
        EDFF_UPGRADE_FACTION = 1, //升级大殿
        EDFF_UPGRADE_RAREITEM = 2,//升级珍品
        EDFF_UPGRADE_GYMNASIUM = 3,//升级武堂
        EDFF_UPGRADE_SHADE = 4,//升级暗部
        EDFF_UPGRADE_HOTSPRING = 5,//升级温泉
        EDFF_GM = 6, //GM
		EDFF_OPEN_HOTSPRING = 7, //开启温泉
    };

    // 记载玩家个人操作
    enum EPlayerOperate
    {
        EPO_RE_REG_NICK_NAME    = 1, // 玩家改名（合区时昵称冲突的玩家将得到一次改名机会）
        EPO_RE_REG_FACTION_NAME = 2, // 掌门人对门派进行改名（合区时若发生门派名称冲突，则冲突门派将得到一次给门派改名机会：只能由掌门人来改）
    };

    // 门派跨服战活动
    enum EGvGActivity
    {
        EGA_APPLY     = 1, // 门派报名
        EGA_GIVE_GIFT = 2, // 给予排名礼包
        EGA_RECV_GIFT = 3, // 领取礼包
        EGA_RANK = 4,      // 门派排名
    };

    // 飞升版本所含功能
    enum EFly
    {
        EF_FLY, // 飞升
        EF_UPGRADE_HERO, // 伙伴进阶
        EF_RECRUIT_HERO, // 招募伙伴
    };
};




#endif