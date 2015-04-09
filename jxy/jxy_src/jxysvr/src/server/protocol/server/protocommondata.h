/*       Version Number: 10          */

#ifndef PROTOCOMMONDATA_H_INCLUDE_VERSION_10
#define PROTOCOMMONDATA_H_INCLUDE_VERSION_10

#include "netdata.h"
#include "sdtype.h"
#include "sdstring.h"
#include <cjson.h>
#include <dll/sdframework/sdutil.h>
#pragma pack(push, 1)

/*Defines Begin*/
#define SUCCESS                  0
#define FAIL                     1
#define REFUSE_LOGIN             2
#define SESSIONID_LEN            8
#define DEVICEID_LEN             41
#define NOTIFYID_LEN             65
#define USERNAME_LEN             41
#define ZONENAME_LEN             41
#define USERTYPE_LEN             41
#define USERPWD_LEN              17
#define MSG_LEN                  1024
#define IPSTR_LEN                17
#define BEGBLUE_CLIMB_TOWER_PROMPT 61
#define BEGBLUE_LUCK_PROMPT      21
#define WANTED_DSPNAME_LEN       21
#define WANTED_DESC_LEN          128
#define CLUE_TASK_NAVI_DESC_LEN  128
#define LOGIN_TYPE_USER          0
#define LOGIN_TYPE_IPHONE        1
#define MAX_HERO_NUM             90
#define NEED                     1
#define NONNEED                  0
#define MAX_BAG_OPEN_NUM         200
#define MAX_BAG_OPEN_NUM_NEW     600
#define CURRENCY_TYPE_COIN       0
#define CURRENCY_TYPE_GOLD       1
#define MAX_SKILL_NUM            12
#define MAX_GAS_BALL_NUM         8
#define MAX_SKILL_SLOT_NUM       3
#define MAX_SKILL_BUFF_NUM       3
#define MAX_EFFECT_RECOMMEND_NUM 2
#define MAX_BAG_NUM              5
#define MAX_BUILD_NUM_BEFORE_v410 50
#define MAX_BUILD_NUM            100
#define MAX_HANG_NUM             50
#define MAX_BATTLE_ROUND_NUM     60
#define MAX_NEW_BATTLE_ROUND_NUM 120
#define MAX_ENCOURAGE_ITEM_NUM   10
#define MAX_INSTANCE_MONSTER_NUM 10
#define PICKUP_KIND_ITEM         0
#define PICKUP_KIND_HANG         1
#define MAX_SCENCE_NUM           2
#define MAX_TOWN_PER_SCENE_NUM   200
#define MAX_INSTANCE_PER_TOWN_NUM 19
#define MAX_INSTANCE_PER_SCENE_NUM 600
#define INSTANCE_INDEX_STR_LEN   15
#define MAX_HP_CHANGE_TIMES_PER_ROUND 30
#define MAX_ANGRY_CHANGE_TIMES_PER_ROUND 10
#define MAX_NEW_ANGRY_CHANGE_TIMES_PER_ROUND 30
#define MAX_BUFF_CHANGE_TIMES_PER_ROUND 10
#define MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND 30
#define MAX_SCIENCE_ATTR_NUM     20
#define MAX_TASK_LINE_NUM        30
#define MAX_KILL_MONSTER_INSTANCE_NUM 5
#define MAX_ITEM_KIND_COLLECT_NUM 5
#define MAX_UPDATE_ATTR_NUM      20
#define MAX_XIANGQIAN_NUM        8
#define INVALID                  0
#define CROWN                    1
#define RECRUITHERO              2
#define UPGRADESKILL             3
#define UPGRADEJINGJIE           4
#define FIRSTPASSINSTANCE        5
#define SCOREPASSINSTANCE        6
#define TIMESPASSINSTANCE        7
#define KILLMONSTERINSTANCE      8
#define STRENGTHENEQUIPLEVEL     9
#define STRENGTHENEQUIPNUM       10
#define MOSAICGEM                11
#define ACTIVATEGODWEAPON        12
#define UPGRADEGODWEAPON         13
#define CHALLENGE                14
#define PLUNDER                  15
#define CLIMBTOWER               16
#define DRESSEQUIP               17
#define COMPOSEGOOD              18
#define ELITEINSTANCE            19
#define UPGRADSCIENCE            20
#define STRENGTHATTACK           21
#define STRENGTHLIFE             22
#define CROP                     23
#define USEEXPRIENCEDRUG         24
#define MELTEQUIP                25
#define STUDY                    26
#define RECRUITHERONUM           27
#define OPENLIGHT                28
#define MAX_TASK_DSPNAME_LEN     40
#define MAX_TASK_DESC_LEN        100
#define ETS_GIVEN                0
#define ETS_ACCEPTED             1
#define ETS_ENCOURAGE            2
#define ETS_FINISHED             3
#define ETS_INVALID              4
#define MAX_CHALLENGE_NUM        10
#define MAX_SHOP_ITEM_NUM        100
#define MAX_ENCOURAGE_ITEM_KIND_NUM 20
#define MAX_ENCOURAGE_BOX_GROUP_NUM 3
#define MAX_GVE_ENCOURAGE_BOX_GROUP_NUM 6
#define MAX_UNLOCK_HERO_NUM      3
#define EPL_HOME                 0
#define EPL_INSTANCE             1
#define EPL_ONHOOK               2
#define EPL_PAGODA               3
#define EPL_EATBREAD             4
#define EPL_BOSSB                5
#define EPL_HOT_SPRING           6
#define EPL_COURAGE              7
#define EPL_FACTION              8
#define EPL_COURAGE_BATTLE       9
#define EPL_SOUL                 10
#define EPL_PET                  11
#define EPL_NEIDAN               12
#define EIK_EQUIP                0
#define EIK_GOODS                1
#define EIK_JEWELRY              2
#define MAX_DRUG_LEVEL           6
#define MAX_FORMATION_NUM        9
#define MAX_FORMATION_IDX_NUM    6
#define MAX_ITEM_MAKER_NEED_NUM  6
#define EEIK_EQUIP               0
#define EEIK_DRUG                1
#define MAX_INSTANCE_RUN_TIMES   40
#define EIT_COMMON               1
#define EIT_ELITE                2
#define EIT_TOWER                3
#define EIT_OAI                  4
#define EIT_COURAGE              5
#define EIT_OUT_BOUND            6
#define EIT_NEIDAN               7
#define GET_ZONE_NUM             20
#define MAX_LOGIN_ENCOURAGE_ITEM_NUM 8
#define MAX_RANK_SHOW_NUM        100
#define MAX_RACE_BATTLELOG_NUM   50
#define MAX_YABIAO_BATTLELOG_NUM 50
#define MAX_BIAO_QUALITY         5
#define MAX_COMPOSE_ITEM_NUM     5
#define MAX_PLUNDER_BATTLELOG_NUM 50
#define MAX_CAPTURE_NUM          4
#define MAX_GET_ENEMY_NUM        5
#define MAX_GET_PLUNDER_ENEMY_NUM 5
#define MAX_GET_RDC_NUM          5
#define MAX_ROBOT_NUM            10
#define MAX_ENEMY_NUM            200
#define MAX_GODWEAPON_QUALITY_NUM 50
#define MAX_BUFF_NUM             27
#define MAX_INSTANCE_NUM         200
#define EQUIP_POS_NUM            4
#define MAX_CROP_NUM             9
#define MAX_CROP_SEED_NUM        50
#define MAX_SCIENCE_KIND_NUM     15
#define MAX_CONTENT_SIZE         1024
#define MAX_NOTICE_CONTENT_LEN   4096
#define MAX_ORDER_ID_LEN         255
#define MAX_RECEIPT_DATA_LEN     2550
#define AUTH_TYPE_DEVECEID       0
#define AUTH_TYPE_TONGBU         1
#define AUTH_TYPE_91             2
#define AUTH_TYPE_MAXNET         3
#define AUTH_TYPE_PP             4
#define MAX_ACTIVITY_NUM         10
#define MAX_ACTIVITY_TIME_NUM    10
#define MAX_CLIMB_TOWER_NUM      7
#define MAX_BOOK_DATA_NUM        7
#define MAX_TOKEN_NUM            64
#define MAX_JINGJIE_ATTR_INC_NUM 7
#define MAX_JINGJIE_SUBLEVEL_NUM 13
#define MAX_SKILL_ACTION_LEVEL_NUM 90
#define MAX_BIND_EFFECT_NUM_PER_ACTION 3
#define MAX_PARM_LEN             1024
#define MAX_VIP_LEVEL            20
#define MAX_VIP_FUNC             100
#define MAX_AWAKEN_LEVEL         5
#define MAX_AWAKEN_NUM           100
#define MAX_BAG_STUDY_GRID_NUM   16
#define MAX_HERO_STUDY_GRID_NUM  4
#define MAX_EXCHANGE_STUDY_NUM   20
#define MAX_INSTANCE_CLIMB_TOWER_NUM 255
#define MAX_GEM_LEVEL            12
#define MAX_GEM_LEVEL_EX         32
#define EQUIP_GEM_NUM            4
#define MAX_GUIDE_NUM            128
#define MAX_LUAACTIVITY_RECORD_NUM 100
#define MAX_ELITE_INSTANCE_TOWN_NUM 15
#define MAX_COLOR_NUM            4
#define MAX_EQUIP_XIANGQIAN_NUM  255
#define MAX_BOSS_ENC_TOP_NUM     6
#define MAX_BOSSB_SHOW_PLAYER_NUM 50
#define BOSSB_POS_TYPE_ENTER     0
#define BOSSB_POS_TYPE_LEAVE     1
#define BOSSB_POS_TYPE_MOVE      2
#define MAX_BOSSB_NUM_PER_DAY    3
#define MAX_BATTLE_LOG_TYPE_NUM  20
#define MAX_BOSSB_BATTLE_LOG_NUM 20
#define MAX_ABSORB_NUM           56
#define MAX_WORSHIP_NUM          20
#define EVERY_CHAPTER_NUM        9
#define MAX_EXT_FUN_NUM          10
#define MAX_ACTIVITY_ENCOURAGE_INFO_NUM 512
#define MAX_EQUIPLEVEL_NUM       50
#define MAX_FACTION_NUM          64
#define MAX_FACTION_DESC_NUM     255
#define DT_FACTION_PLAYER_NUM    100
#define MAX_FACTION_LOG_NUM      100
#define FOR_NEW_BEGBLUE          1
#define FOR_TO_COMFIRM_BEGBLUE   2
#define MAX_MOBILE_LENGTH        20
#define MAX_WANTED_NUM           5
#define MAX_CLUE_TASK_NUM        100
#define KILL_MONSTER_NAVI        1
#define ITEM_BOX_NAVI            2
#define FORTUNE_GRID_NAVI        3
#define MAX_FRIEND_SYS_PLAYER_LIST_ELEM_NUM 10
#define MAX_SEND_FLOWER_PROP_NUM 3
#define MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM 20
#define MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM 128
#define MAX_RELATION_LIST_ELEM_NUM 200
#define MAX_FINALLY_CONTACT_DATA_NUM 50
#define MAX_FLOWER_RANKING_LIST_ELEM_NUM 20
#define PRIVATE_MSG_CONTENT_LEN  200
#define MAX_PRIVATE_MSG_LIST_ELEM_NUM 10
#define MAX_NEW_PRIVATE_MSG_NOTICE_NUM 50
#define MAX_NEW_RECV_FLOWER_NOTICE_NUM 50
#define MAX_SEND_FLOWER_DATA_NUM 5
#define MAX_SEND_OFFLINE_MSG_PLAYER_LIST_ELEM_NUM 100
#define MAX_RAREITEM_RES_NUM     100
#define MAX_RAREITEM_ROOM_GRADE  100
#define MAX_UPGRADE_INFO_NUM     6
#define FACTION_UPGRADE_MSG_LEN  64
#define MAX_GYMNASIUM_SKILL_NUM  24
#define SKILL_MSG_LEN            32
#define SKILL_NAME_LEN           18
#define NORMAL_MSG_LEN           50
#define TASK_DESCRIBE_MSG_LEN    128
#define HUNTING_REWARD_MGS_NUM   3
#define SHADE_UPGRADE_MSG_NUM    4
#define MONSTER_TYPE_NUM         4
#define MAX_HOT_SPRING_SHOW_PLAYER_NUM 50
#define HOT_SPRING_WINE_NUM      3
#define GIVE_WINE_DRINK          1
#define GIVE_WINE_NOT_DRINK      2
#define GIVE_WINE_FINISH         3
#define NOT_GIVE_WINE_DRINK      4
#define NOT_GIVE_WINE_NOT_DRINK  5
#define SYNC_INTERVAL_HOT_SPRING 1000
#define WINE_MSG_LEN             128
#define MAX_FACTION_UPGRADE_NUM  5
#define MAX_COURAGE_GROUP_PLAYER_NUM 3
#define MAX_COURAGE_GROUP_NUM    1024
#define EQUIP_KIND_COUNT         4
#define MASTER_RANK_COUNT        4
#define MASTER_EXP_BOOK_COUNT_PER_KIND 3
#define MASTER_EXP_BOOK_COUNT    12
#define MASTER_MAX_LEVEL_UP      10
#define FACTION_LOG_LIST_MAX_NUM 10
#define HELP_COURAGE_LEN         80
#define COURAGE_HERO_NUM         4
#define COURAGE_NO_GROUP         1
#define COURAGE_HAVE_GROUP       2
#define MAX_FACTION_BATTLE_LOG_NUM 50
#define MAX_ACTIVI_SIDE_PROMPT_NUM 36
#define MAX_HUTING_POT_NUM       5
#define MAX_HUNTING_POT_NUM_NEW  64
#define HUNTING_POT_NAME         32
#define MAX_FACTION_NAME_LEN     12
#define MAX_LEAVE_HOT_SPRING_PROMPT_MSG 125
#define MONSTER_GROUP_NUM        3
#define MAX_RESOURCE_NUM         20
#define MAX_MONSTER_NUM          9
#define MAX_COURAGE_MEMBER_NUM   3
#define MAX_VIP_GIFT_ITEM_NUM    10
#define MAX_FACTION_MANAGER_NUM  64
#define MAX_VIP_GIFT             3
#define MAX_NEW_FORMATION_NUM    20
#define MAX_PLAYER_RES_NUM       32
#define BEGBLUE_ENCOURAGE_PROMPT 61
#define BEGBLUE_CONSUME_TIMES    30
#define MAX_JEWELRY_POS          4
#define MAX_JEWELRY_STRENG_NUM   10
#define MAX_GVG_PK_VIDEO_CNT     100
#define MAX_GVG_TOP_FACTION_CNT  8
#define GVG_16_TO_8_FACTION_CNT  16
#define MAX_GVG_ARENA_FACTION_CNT 32
#define GVG_16_TO_8_WIN_CNT      8
#define GVG_8_TO_4_WIN_CNT       4
#define GVG_FINAL_WAR_FACTION_CNT 8
#define MAX_GVG_LOG_CNT          25
#define MAX_GVG_SIMPLE_LOG_CNT   12
#define MAX_GVG_ARENA_ZONE_CNT   8
#define MAX_GVG_MVP_CNT          100
#define MAX_RANK_DISPLAY         100
#define MAX_GVG_FORMATION_PLAYER_CNT 80
#define MAX_GAMBLE_NUM           9
#define MAX_GVG_RULE             4096
#define MAX_FACTION_APPLAY_NUM   500
#define MAX_FACTION_AREA_NUM     4
#define MAX_GVG_GROUP_NUM        2
#define MAX_FACTION_BATTLE_NUM   16
#define MAX_GVG_FACTION_NUM      32
#define MAX_GVG_GIFT_NUM         200
#define MAX_GIFT_GOODS_NUM       10
#define MAX_BATTLE_LOG_LEN       256
#define MAX_BATTLE_LOG_NUM       100
#define MAX_BATTLE_LOG_PARAM_NUM 20
#define MAX_BATTLE_LOG_PARAM_STR 10
#define MAX_SOUL_BAG_NUM         255
#define MAX_SOUL_EQUIP_NUM       2
#define MAX_SOUL_ATTR_NUM        16
#define MAX_SOUL_PRODUCT_TYPE    4
#define MAX_NAME_LEN             64
#define MAX_MEDAL_DESC_LEN       128
#define MAX_MEDAL_NUM            512
#define MAX_SKILL_SOUL_NUM       8
#define MAX_OUT_BOUND_NUM        255
#define MAX_OUT_BOUND_INSTANCE_NUM 32
#define MAX_DISPALY_GOODS_NUM    32
#define MAX_PET_NUM              150
#define MAX_PET_NAME_LENGTH      32
#define MAX_OUT_BOUND_DESC_LEN   128
#define MAX_PET_FEED_FRUIT_CNT   5
#define MAX_PET_WASH_PSYCHIC_CELL_CNT 5
#define MAX_PET_WASH_PSYCHIC_QUALITY_CNT 10
#define MAX_PET_WASH_PSYCHIC_ITEMTYPE_CNT 10
#define RECRUIT_FLY_HERO_MATERIAL_NUM 5
#define UPGRADE_HERO_MATERIAL_NUM 5
#define MAX_MATERIAL_NUM         10
#define MAX_SERIES_NUM           50
#define MAKE_EQUIP_MATERIAL_NUM  4
#define MAX_MAKE_BOOKS_NUM_OF_1_SERIES 40
#define FLY_GOAL_DESC_LENGTH     100
#define FLY_COST_MATERIAL_NUM    4
#define MAX_FLY_GOAL_NUM         8
#define MAX_EQUIP_SERIES_NAME_LEN 50
#define MAX_JEWELRY_SUIT_ATTR_NUM 20
#define MAX_PET_NEIDAN_NUM       5
#define MAX_NEI_DAN_BAG_NUM      200
#define PROTOCOMMONDATA_MSGID_RANGE 1
#define PROTOCOMMONDATA_MSGID_OFFSET 0
#define PROTOCOL_VERSION 10
/*Defines End*/



#define INDEX_NUM_HEAD_SIZE	1	//索引个数头大小
#define ARRAY_NUM_HEAD_SIZE	2	//数组个数头大小

struct SIndex
{
	UINT8	byIdx;
	UINT32	dwContextLen;
};

/*Define Structs and Unions        Begin*/
typedef struct tagDT_RES_DATA{
	UINT8            byResKind; //资源类型
	UINT64           qwResValue; //资源值

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RES_DATA;

typedef struct tagDT_ITEM_DATA{
	UINT16           wKindID; //道具类型ID
	UINT16           wNum; //个数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ITEM_DATA;

typedef struct tagDT_RES_NUM_ITEM_DATA{
	UINT8            byResNum; //资源个数
	DT_RES_DATA      astResList[MAX_PLAYER_RES_NUM]; //资源信息
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA     astItemList[MAX_PLAYER_RES_NUM]; //道具信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RES_NUM_ITEM_DATA;

typedef struct tagDT_GUIDE_RECORD_DATA{
	UINT8            abyGuideRecordData[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GUIDE_RECORD_DATA;

typedef struct tagDT_PLAYER_BASE_DATA{
	UINT32           dwID; //玩家唯一ID
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
	UINT64           qwStory; //阅历
	UINT16           wPhyStrength; //当前体力
	UINT64           qwScience; //当前科技点
	DT_GUIDE_RECORD_DATA stGuideRecordData; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
	UINT8            byRobotFunc; //机器人功能,>0则为机器人，不同的值为不同的功能。0为玩家
	UINT32           dwPower; //战力(玩家init时重新赋值)
	UINT32           dwFirstAttack; //先攻值
	UINT64           qwTodayFirstLoginTime; //今天第一次登陆时间
	UINT8            byBagOpenNum; //背包打开格子数(在原来基础上)
	UINT64           qwBlueGas; //蓝气
	UINT64           qwPurpleGas; //紫气
	UINT8            byVipLevel; //VIP等级
	UINT32           dwTotalAmount; //总共充值元宝数
	UINT64           qwJingJie; //境界点
	UINT64           qwTotalRMB; //总共充值金额
	UINT64           qwStudyExp; //卦象经验
	UINT64           qwRecvRankEncTime; //领取排名奖励时间
	UINT32           dwExtData; //扩展字段，用于内部计算记录，如1表示0829给玩家补偿仙露/器魂需求
	UINT32           dwFlowerSum; //鲜花统计
	UINT64           qwMedal; //勇气勋章
	UINT32           dwRMBGold; //当前人民币元宝（人民币元宝 = 玩家人民币1:10兑换的元宝，比如玩家充了6元，获得了75元宝，则其中的<6 * 10 = 60>元宝属于人民币元宝，剩下的15元宝属于充值赠送元宝）
	UINT32           dwRMBGoldGift; //充值赠送元宝 = 充值时获得的元宝 - 该次充值的人民币（元） * 10
	UINT32           dwGmGold; //系统赠送元宝：通过gm工具赠送给玩家的充值元宝，注意，GM可以用2种方式给玩家添加元宝，1.添加充值元宝 2.添加普通游戏元宝
	UINT16           wOriZoneID; //玩家创建角色时所在的区（用于合区后的元宝统计用，元宝日志要区分出玩家原本来自于哪个区）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_BASE_DATA;

typedef struct tagDT_BATTLE_ATTRIBUTE{
	UINT32           dwHP; //血量
	UINT32           dwAttack; //攻击力

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ATTRIBUTE;

typedef struct tagDT_BATTLE_ATTRIBUTE2{
	UINT32           dwHP; //血量
	UINT32           dwAttack; //攻击力
	UINT32           dwDefend; //防御

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ATTRIBUTE2;

typedef struct tagDT_BATTLE_BASE_ATTRIBUTE{
	UINT64           qwHP; //血量
	UINT32           dwAttack; //攻击力
	UINT32           dwDefend; //防御

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_BASE_ATTRIBUTE;

typedef struct tagDT_HIDE_BATTLE_ATTRIBUTE{
	UINT16           wHitRate; //命中率
	UINT16           wDodgeRate; //闪避率
	UINT16           wCritRate; //暴击率
	UINT16           wDeCritRate; //抗暴率
	UINT16           wAngryValue; //怒气值

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HIDE_BATTLE_ATTRIBUTE;

typedef struct tagDT_EXT_BATTLE_ATTRIBUTE{
	UINT16           wIncHurtRate; //增伤率(增加的伤害百分比)
	UINT16           wDecHurtRate; //减伤率(减少的伤害百分比)
	UINT16           wOddAngry; //怒气剩余值(技能释放后剩余的怒气值)
	UINT32           dwFirstAttack; //先攻
	UINT16           wCoachTalent; //主角资质

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EXT_BATTLE_ATTRIBUTE;

typedef struct tagDT_HERO_UPDATE_ATTR_DATA{
	UINT16           wAttrHP; //血量
	UINT16           wAttrAttack; //攻击

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_UPDATE_ATTR_DATA;

typedef struct tagDT_HERO_ATTR_DATA{
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //战斗属性
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttr; //隐藏战斗属性
	DT_HERO_UPDATE_ATTR_DATA stUpdateAttrInfo; //属性升级信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_ATTR_DATA;

typedef struct tagDT_SPECIAL_EFFECT_DATA{
	UINT16           wEffectID; //特效ID
	UINT8            byIsActivity; //是否激活
	UINT8            bySkillNum; //特效个数
	UINT8            abyEffectSmallType[MAX_SKILL_SOUL_NUM]; //特效类型

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SPECIAL_EFFECT_DATA;

typedef struct tagDT_SOUL_ATTR_DATA{
	UINT8            byKindID; //属性ID
	UINT8            byKindPrecent; //属性所占百分比
	UINT32           dwKindValue; //属性值

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SOUL_ATTR_DATA;

typedef struct tagDT_SOUL_POTENCY_DATA{
	UINT8            byPotencyLevel; //潜能等阶
	UINT8            byPotencySubLevel; //潜能等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SOUL_POTENCY_DATA;

typedef struct tagDT_SOUL_POTENCY_LST_DATA{
	UINT16           wSoulNum; //战魂个数
	DT_SOUL_POTENCY_DATA astSoulPotencyLstData[MAX_SOUL_BAG_NUM]; //战魂信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SOUL_POTENCY_LST_DATA;

typedef struct tagDT_SOUL_DATA{
	UINT16           wSoulID; //战魂ID
	UINT32           dwSoulIdx; //战魂Idx(区分同名战魂)
	UINT8            bySoulStar; //战魂星级
	UINT8            bySoulType; //战魂类型 1代表攻击类 2代表防御类
	UINT8            bySoulLevel; //战魂等级
	UINT32           dwSoulExperience; //战魂经验
	UINT8            byIsNormalSoul; //0 代表吞噬魂 1 代表普通魂
	UINT32           dwDevourExp; //吞噬经验
	UINT8            byBaseNum; //战魂基本属性个数
	DT_SOUL_ATTR_DATA astSoulAttr[MAX_SOUL_ATTR_NUM]; //战魂属性
	UINT8            byBaseAfterNum; //战魂基本属性个数（洗炼后）
	DT_SOUL_ATTR_DATA astSoulAfterAttr[MAX_SOUL_ATTR_NUM]; //战魂属性（洗炼后）
	UINT8            byHaveEffect; //是否有特效
	DT_SPECIAL_EFFECT_DATA stSpecialEffect; //特殊技能
	UINT8            abySoulAttrLock[MAX_SOUL_ATTR_NUM]; //锁定魂

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SOUL_DATA;

typedef struct tagDT_HERO_SOUL_DATA{
	UINT16           wSoulNum; //战魂个数
	DT_SOUL_DATA     astSoulLstData[MAX_SOUL_EQUIP_NUM]; //装备战魂信息
	DT_SOUL_POTENCY_DATA astSoulPotencyLstData[MAX_SOUL_EQUIP_NUM]; //潜能

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_SOUL_DATA;

typedef struct tagDT_HERO_BASE_DATA{
	UINT16           wKindID; //武将类型
	UINT8            byCoach; //主武将标志,1为主武将
	UINT8            byPos; //位置标志,0携带，1为出战，2在招募所中
	UINT8            byFormationIdx; //阵型位置
	UINT16           wLevel; //等级
	UINT16           wTalent; //资质
	UINT64           qwExperience; //经验
	UINT8            byRecuitedFlag; //是否招募过
	UINT8            byVisitNum; //拜访次数
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //战斗属性
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttr; //隐藏战斗属性
	UINT8            byLastUseDrugLevel; //上次使用的丹药等级(品)
	UINT64           qwLastUseDrugTime; //上次使用丹药时间
	UINT8            byUseExperienceDrugPerDay; //每天服用升级丹个数
	UINT64           qwLastUseExperienceDrugTime; //上次使用经验丹时间
	DT_HERO_UPDATE_ATTR_DATA stUpdateAttrInfo; //属性升级信息
	UINT8            byIsFly; //是否已飞升
	UINT32           dwFlyExp; //伙伴等级已到120级卡在飞升期间时累积的经验

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_BASE_DATA;

typedef struct tagDT_XIANGQIAN_DATA{
	UINT16           wItemID; //物品ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_XIANGQIAN_DATA;

typedef struct tagDT_EQUIP_DATA{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT16           wLevel; //装备等级
	UINT16           wBagPos; //所在背包位置
	UINT64           qwItemNewTime; //道具生产时间
	DT_XIANGQIAN_DATA astXiangqianList[MAX_XIANGQIAN_NUM]; //镶嵌列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EQUIP_DATA;

typedef struct tagDT_HERO_EQUIP_DATA_LST{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA    astEquipList[EQUIP_POS_NUM]; //装备信息
	UINT16           awEnchantLevel[EQUIP_POS_NUM]; //各装备附魔等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_EQUIP_DATA_LST;

typedef struct tagDT_BAG_EQUIP_DATA_LST{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA    astEquipList[MAX_BAG_OPEN_NUM_NEW]; //装备信息
	UINT16           awEnchantLevel[MAX_BAG_OPEN_NUM_NEW]; //各装备附魔等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BAG_EQUIP_DATA_LST;

typedef struct tagDT_GOODS_DATA{
	UINT16           wKindID; //物品类型
	UINT16           wPileCount; //叠加数
	UINT64           qwLastUseTime; //上次使用时间
	UINT16           wLastUseNum; //上次使用次数
	UINT16           wBagPos; //所在背包位置
	UINT64           qwItemNewTime; //道具生产时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GOODS_DATA;

typedef struct tagDT_BAG_GOODS_DATA_LST{
	UINT16           wGoodsNum; //装备个数
	DT_GOODS_DATA    astGoodsList[MAX_BAG_OPEN_NUM_NEW]; //装备信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BAG_GOODS_DATA_LST;

typedef struct tagDT_DRUG_DATA{
	UINT8            byLevel; //丹药等级
	UINT8            byNum; //丹药个数
	UINT16           wHeroKindID; //使用的武将ID，必须存在

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DRUG_DATA;

typedef struct tagDT_DRUG_DATA_LST{
	UINT8            byDrugLevelNum; //丹药级别个数
	DT_DRUG_DATA     astDrugList[MAX_DRUG_LEVEL]; //丹药信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DRUG_DATA_LST;

typedef struct tagDT_GODWEAPON_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT16           wQuality; //品质
	UINT16           wLevel; //等级
	UINT16           wHeroKindID; //拥有的武将类型ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GODWEAPON_DATA;

typedef struct tagDT_ATTACK_RANG_DATA{
	UINT8            byAttackRangKind; //攻击范围类型
	UINT8            byTraceBuffKind; //追踪的buff类型(只对AttackRangKind为EARK_BUFF有效)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_RANG_DATA;

typedef struct tagDT_SKILL_DATA{
	UINT16           wActionID; //使用招式
	UINT16           wActionLevel; //使用招式等级
	UINT16           wEffectID; //使用效果
	DT_ATTACK_RANG_DATA stAttackRangInfo; //攻击范围
	UINT32           dwActionExp; //招式经验

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SKILL_DATA;

typedef struct tagDT_SKILL_DATA_LST{
	UINT8            bySkillNum; //技能个数
	DT_SKILL_DATA    astSkillInfo[MAX_SKILL_SLOT_NUM]; //技能信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SKILL_DATA_LST;

typedef struct tagDT_STUDY_DATA{
	UINT8            byStudyColorKind; //心得颜色，从1开始，0表示没有数据
	UINT8            byStudyAttrKind1; //心得类型(对应属性类型,0表示只能转换经验)
	UINT8            byStudyLevel; //心得等级
	UINT8            byStudyAttrKind2; //心得类型(对应属性类型)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_STUDY_DATA;

typedef struct tagDT_AWAKEN_BASE_DATA{
	UINT8            byLevel; //悟道等级（0则没有数据）
	UINT8            byGoldFlag; //是否元宝悟道标志（0不是，1是）
	UINT64           qwStudyExp; //心得经验
	UINT16           wHaveCallNum; //已召唤次数
	UINT64           qwlastCallTime; //上次召唤时间
	UINT64           qwStudyClip; //心得碎片

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_AWAKEN_BASE_DATA;

typedef struct tagDT_AWAKEN_BASE_DATA1{
	UINT8            byLevel; //悟道等级（0则没有数据）
	UINT8            byGoldFlag; //是否元宝悟道标志（0不是，1是）
	UINT64           qwStudyExp; //心得经验
	UINT16           wHaveCallNum; //已召唤次数
	UINT64           qwlastCallTime; //上次召唤时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_AWAKEN_BASE_DATA1;

typedef struct tagDT_AWAKEN_DATA{
	DT_AWAKEN_BASE_DATA stBaseInfo; //悟道基本信息
	DT_STUDY_DATA    astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT8            byZTSUseTime; //张天师不出黄卦次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_AWAKEN_DATA;

typedef struct tagDT_AWAKEN_DATA_LST{
	DT_STUDY_DATA    astStudyInfo[MAX_HERO_STUDY_GRID_NUM]; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_AWAKEN_DATA_LST;

typedef struct tagDT_ABSORB_DATA_LST{
	UINT8            byAbsorbNum; //当前吸收次数
	UINT16           awKindIDLst[MAX_ABSORB_NUM]; //次数 武将信息
	UINT16           wAbsorbKindID; //吸收过当前武将的武将ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ABSORB_DATA_LST;

typedef struct tagDT_ATTR_INC_DATA{
	UINT8            byAttrKind; //属性类型
	UINT32           dwAttrValue; //属性值

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTR_INC_DATA;

typedef struct tagDT_BATTLE_ATTR_LST_INFO{
	UINT8            byAttrNum; //属性数量
	DT_ATTR_INC_DATA astAttrInfo[MAX_SCIENCE_ATTR_NUM]; //属性信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ATTR_LST_INFO;

typedef struct tagDT_BATTLE_ATTR_EXT_INFO{
	UINT8            byKindID; //作用目标(commondef.h)
	UINT32           dwKindValue; //作用值
	UINT8            byLevel; //附加等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ATTR_EXT_INFO;

typedef struct tagDT_BATTLE_ATTR_EXT_LST_INFO{
	UINT8            byExtAttrNum; //附加属性数量
	DT_BATTLE_ATTR_EXT_INFO astExtAttrInfo[MAX_SCIENCE_ATTR_NUM]; //附加属性信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ATTR_EXT_LST_INFO;

typedef struct tagDT_JEWELRY_DATA{
	UINT16           wJewelryID; //饰品ID
	UINT16           wJewelryLevel; //饰品Level
	UINT8            byJewelryDressPos; //饰品类型(穿戴位置)
	DT_BATTLE_ATTR_LST_INFO stBaseAttrLst; //饰品基本属性
	DT_BATTLE_ATTR_EXT_LST_INFO stExtAttrLst; //饰品附加属性
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT16           wBagPos; //所在背包位置
	UINT64           qwItemNewTime; //道具生产时间
	UINT16           wStrengLevel; //饰品强化等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_JEWELRY_DATA;

typedef struct tagDT_JEWELRY_DATA_LST{
	UINT16           wJewelryNum; //首饰个数
	DT_JEWELRY_DATA  astJewelryInfo[MAX_JEWELRY_POS]; //饰品信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_JEWELRY_DATA_LST;

typedef struct tagDT_BAG_JEWELRY_DATA_LST{
	UINT16           wJewelryNum; //首饰个数
	DT_JEWELRY_DATA  astJewelryInfo[MAX_BAG_OPEN_NUM_NEW]; //装备信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BAG_JEWELRY_DATA_LST;

typedef struct tagDT_HERO_DATA{
	DT_HERO_BASE_DATA stHeroBaseData; //武将基本信息
	DT_HERO_EQUIP_DATA_LST stEquipDataInfo; //武将装备信息
	DT_DRUG_DATA_LST stDrugDataInfo; //丹药信息
	DT_GODWEAPON_DATA stGodweaponInfo; //神器信息
	DT_SKILL_DATA_LST stSkillInfo; //技能信息
	DT_AWAKEN_DATA_LST stStudyInfo; //心得信息
	DT_ABSORB_DATA_LST stAbsorbInfo; //吸收信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_DATA;

typedef struct tagDT_HERO_DATA_LIST{
	UINT16           wHeroNum; //武将数目
	DT_HERO_DATA     astHeroInfoLst[MAX_HERO_NUM]; //武将信息
	DT_JEWELRY_DATA_LST astjewelryInfoLst[MAX_HERO_NUM]; //饰品信息
	DT_HERO_SOUL_DATA astSoulInfoLst[MAX_HERO_NUM]; //战魂信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_DATA_LIST;

typedef struct tagDT_BUILD_BASE_DATA{
	UINT8            byKindID; //建筑类型
	UINT16           wLevel; //等级
	UINT64           qwValue1; //对于主城为收获时间
	UINT64           qwValue2; //对于主城为征收时间
	UINT8            byValue3; //对于主城为征收的次数
	UINT8            bySort; //建筑排序
	UINT8            byHaveOpenFlag; //建筑是否打开过，1为打开过

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BUILD_BASE_DATA;

typedef struct tagDT_BUILD_DATA_LIST{
	UINT16           wBuildNum; //建筑数目
	DT_BUILD_BASE_DATA astBuildInfoLst[MAX_BUILD_NUM]; //建筑信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BUILD_DATA_LIST;

typedef struct tagDT_INSTANCE_ID{
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
	UINT8            byInstanceIdx; //副本序号

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_INSTANCE_ID;

typedef struct tagDT_INSTANCE_DATA{
	DT_INSTANCE_ID   stInstanceID; //副本序号
	UINT64           qwLastPassTime; //上次副本通关时间
	UINT8            byScore; //副本通关得分
	UINT32           dwSuccessNum; //通关成功次数
	UINT32           dwFailedNum; //通关失败次数
	UINT16           wLastConFailedNum; //上次打副本失败次数
	UINT16           wMaxConFailedNum; //打副本最大失败次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_INSTANCE_DATA;

typedef struct tagDT_INSTANCE_DATA_LIST{
	UINT16           wSceneIdx; //场景序号
	UINT16           wInstanceNum; //副本数目
	DT_INSTANCE_DATA astInstanceInfo[MAX_INSTANCE_PER_SCENE_NUM]; //副本信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_INSTANCE_DATA_LIST;

typedef struct tagDT_SCIENCE_ATTREXT{
	UINT8            byAttrKindID; //属性类型
	UINT16           wAttrLevel; //科技属性等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SCIENCE_ATTREXT;

typedef struct tagDT_SCIENCE_ATTREXT_LIST{
	UINT8            byAttrNum; //科技馆属性数目
	DT_SCIENCE_ATTREXT astBuildInfoLst[MAX_SCIENCE_ATTR_NUM]; //科技馆属性信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SCIENCE_ATTREXT_LIST;

typedef struct tagDT_CROWN_TASK_CONDITION{
	UINT16           wCrownTimes; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CROWN_TASK_CONDITION;

typedef struct tagDT_BUILDUPGRADE_TASK_CONDITION{
	UINT8            byBuildKindID; //
	UINT16           wLevel; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BUILDUPGRADE_TASK_CONDITION;

typedef struct tagDT_RECRUITHERO_TASK_CONDITION{
	UINT16           wRecruitHeroKindID1; //
	UINT16           wRecruitHeroKindID2; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RECRUITHERO_TASK_CONDITION;

typedef struct tagDT_CROWN_HEROCOLOR_CONDITION{
	UINT16           wHeroColor; //
	UINT8            byNum; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CROWN_HEROCOLOR_CONDITION;

typedef struct tagDT_CROWN_HEROUPGRADE_CONDITION{
	UINT16           wHeroLevel; //
	UINT8            byNum; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CROWN_HEROUPGRADE_CONDITION;

typedef struct tagDT_FIRSTPASSINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FIRSTPASSINSTANCE_TASK_CONDITION;

typedef struct tagDT_SCOREPASSINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byScore; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SCOREPASSINSTANCE_TASK_CONDITION;

typedef struct tagDT_TIMESPASSINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byTimes; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_TIMESPASSINSTANCE_TASK_CONDITION;

typedef struct tagDT_KILLMONSTERINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT16           wMonsterKindID; //
	UINT8            byKillNum; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_KILLMONSTERINSTANCE_TASK_CONDITION;

typedef struct tagDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION{
	UINT16           wEquipKindID; //
	UINT16           wLevel; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION;

typedef struct tagDT_STRENGTHENEQUIPNUM_TASK_CONDITION{
	UINT16           wEquipKindID; //
	UINT16           wNum; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_STRENGTHENEQUIPNUM_TASK_CONDITION;

typedef struct tagDT_ITEMCOLLECT_TASK_CONDITION{
	UINT16           wItemKindNum; //
	UINT8            abyItemCollectInfo[MAX_ITEM_KIND_COLLECT_NUM]; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ITEMCOLLECT_TASK_CONDITION;

typedef struct tagDT_ACTIVATEGODWEAPON_TASK_CONDITION{
	UINT16           wHeroKindID; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ACTIVATEGODWEAPON_TASK_CONDITION;

typedef struct tagDT_UPGRADEGODWEAPON_TASK_CONDITION{
	UINT16           wHeroKindID; //
	UINT16           wQuality; //
	UINT16           wLevel; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_UPGRADEGODWEAPON_TASK_CONDITION;

typedef struct tagDT_UPGRADEDRUG_TASK_CONDITION{
	UINT16           wUpgradeNum; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_UPGRADEDRUG_TASK_CONDITION;

typedef struct tagDT_CHALLENGE_TASK_CONDITION{
	UINT16           wChallengeNum; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CHALLENGE_TASK_CONDITION;

typedef struct tagDT_PLUNDER_TASK_CONDITION{
	UINT16           wPlunderNum; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLUNDER_TASK_CONDITION;

typedef struct tagDT_DRESSSKILL_TASK_CONDITION{
	UINT16           wSkillID; //
	UINT8            byDressFlag; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DRESSSKILL_TASK_CONDITION;

typedef struct tagDT_DRESSEQUIP_TASK_CONDITION{
	UINT16           wEquipID; //
	UINT8            byDressFlag; //

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DRESSEQUIP_TASK_CONDITION;

typedef struct tagDT_UPGRADEEQUIP_TASK_CONDITION{
	UINT16           wEquipID; //装备ID
	UINT8            byUpgradeFlag; //完成标志

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_UPGRADEEQUIP_TASK_CONDITION;

typedef struct tagDT_USEEXPRIENCEDRUG_TASK_CONDITION{
	UINT16           wNum; //服用次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_USEEXPRIENCEDRUG_TASK_CONDITION;

typedef struct tagDT_MELT_EQUIP_TASK_CONDITION{
	UINT16           wNum; //服用次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MELT_EQUIP_TASK_CONDITION;

typedef struct tagDT_COMPOSEGOOD_TASK_CONDITION{
	UINT16           wMaterialID; //原料ID
	UINT16           wMaterialNum; //原料合成个数
	UINT16           wKindID; //合成的物品ID
	UINT16           wNum; //合成次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COMPOSEGOOD_TASK_CONDITION;

typedef struct tagDT_ELITEINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byUpgradeFlag; //完成标志

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ELITEINSTANCE_TASK_CONDITION;

typedef struct tagDT_UPGRADSCIENCE_TASK_CONDITION{
	UINT16           wNum; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_UPGRADSCIENCE_TASK_CONDITION;

typedef struct tagDT_STRENGTHATTACK_TASK_CONDITION{
	UINT16           wNum; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_STRENGTHATTACK_TASK_CONDITION;

typedef struct tagDT_STRENGTHLIFE_TASK_CONDITION{
	UINT16           wNum; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_STRENGTHLIFE_TASK_CONDITION;

typedef struct tagDT_CROP_TASK_CONDITION{
	UINT16           wNum; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CROP_TASK_CONDITION;

typedef struct tagDT_UPGRADESKILL_TASK_CONDITION{
	UINT16           wSkillActionID; //数值资源信息
	UINT16           wLevel; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_UPGRADESKILL_TASK_CONDITION;

typedef struct tagDT_UPGRADEJINGJIE_TASK_CONDITION{
	UINT16           wLevel; //升级境界 阶
	UINT16           wSubLevel; //升级境界 级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_UPGRADEJINGJIE_TASK_CONDITION;

typedef struct tagDT_MOSAICGEM_TASK_CONDITION{
	UINT16           wNum; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MOSAICGEM_TASK_CONDITION;

typedef struct tagDT_CLIMBTOWER_TASK_CONDITION{
	UINT16           wNum; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CLIMBTOWER_TASK_CONDITION;

typedef struct tagDT_STUDY_TASK_CONDITION{
	UINT8            byLevel; //数值资源信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_STUDY_TASK_CONDITION;

typedef struct tagDT_RECRUITHERO_NUM_TASK_CONDITION{
	UINT16           wNum; //个数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RECRUITHERO_NUM_TASK_CONDITION;

typedef struct tagDT_OPENLIGHT_TASK_CONDITION{
	UINT8            byExt; //次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_OPENLIGHT_TASK_CONDITION;

typedef union tagDT_TASK_CONDITION{
	DT_CROWN_TASK_CONDITION stCrown; //数值资源信息
	DT_RECRUITHERO_TASK_CONDITION stRecruitHero; //数值资源信息
	DT_UPGRADESKILL_TASK_CONDITION stUpgradeSkill; //数值资源信息
	DT_UPGRADEJINGJIE_TASK_CONDITION stUpgradeJingJie; //数值资源信息
	DT_FIRSTPASSINSTANCE_TASK_CONDITION stFirstPassInstance; //数值资源信息
	DT_SCOREPASSINSTANCE_TASK_CONDITION stScorePassInstance; //数值资源信息
	DT_TIMESPASSINSTANCE_TASK_CONDITION stTimesPassInstance; //数值资源信息
	DT_KILLMONSTERINSTANCE_TASK_CONDITION stKillMonstersInstance; //数值资源信息
	DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION stStrengthenEquipLevel; //数值资源信息
	DT_STRENGTHENEQUIPNUM_TASK_CONDITION stStrengthenEquipNum; //数值资源信息
	DT_MOSAICGEM_TASK_CONDITION stMosaicGem; //数值资源信息
	DT_ACTIVATEGODWEAPON_TASK_CONDITION stActivateGodweapon; //数值资源信息
	DT_UPGRADEGODWEAPON_TASK_CONDITION stUpgradeGodweapon; //数值资源信息
	DT_CHALLENGE_TASK_CONDITION stChallenge; //数值资源信息
	DT_PLUNDER_TASK_CONDITION stPlunder; //数值资源信息
	DT_CLIMBTOWER_TASK_CONDITION stClimbTower; //数值资源信息
	DT_DRESSEQUIP_TASK_CONDITION stDressEquip; //数值资源信息
	DT_USEEXPRIENCEDRUG_TASK_CONDITION stUseExprienceDrug; //数值资源信息
	DT_COMPOSEGOOD_TASK_CONDITION stComposeGood; //数值资源信息
	DT_ELITEINSTANCE_TASK_CONDITION stEliteInstance; //数值资源信息
	DT_UPGRADSCIENCE_TASK_CONDITION stUpgradeScience; //数值资源信息
	DT_STRENGTHATTACK_TASK_CONDITION stStrengthAttack; //数值资源信息
	DT_STRENGTHLIFE_TASK_CONDITION stStrengthLife; //数值资源信息
	DT_CROP_TASK_CONDITION stCrop; //数值资源信息
	DT_MELT_EQUIP_TASK_CONDITION stMeltEquip; //数值资源信息
	DT_STUDY_TASK_CONDITION stStudy; //数值资源信息
	DT_RECRUITHERO_NUM_TASK_CONDITION stRecruitHeroNum; //数值资源信息
	DT_OPENLIGHT_TASK_CONDITION stOpenLight; //数值资源信息

	INT32 EncodeSaveData(const UINT8 bySelectType, CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const UINT8 bySelectType, const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_TASK_CONDITION;

typedef struct tagDT_TASK_DATA{
	UINT8            byTaskLine; //任务线
	UINT32           dwTaskIdx; //任务序号(在该任务线内的)
	UINT64           qwStateUpdateTime; //状态更新时间
	UINT8            byTaskState; //任务状态，0未接受，1已接受未完成
	UINT8            byTaskType; //任务完成条件记录
	DT_TASK_CONDITION stTaskConditionInfo; //任务完成条件记录

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_TASK_DATA;

typedef struct tagDT_TASK_DATA_LIST{
	UINT8            byTaskNum; //任务数(每线1个)
	DT_TASK_DATA     astTaskInfoList[MAX_TASK_LINE_NUM]; //任务信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_TASK_DATA_LIST;

typedef struct tagDT_RACE_BASE_DATA{
	UINT32           dwRank; //排名
	UINT16           wContiWin; //当前连胜次数
	UINT64           qwRecvEncourageTime; //领取过排名奖励时间
	UINT64           qwLastChallTime; //最新挑战时间
	UINT8            byChallTimes; //当天挑战次数
	UINT8            byFailedFlag; //是否主动挑战输
	UINT8            byUnreadBattleLogNum; //未观看战报个数
	UINT32           dwRecvEncourageRank; //领取奖励排名(上一次(昨天或更早)的排名)
	UINT64           qwNotifyRankTime; //最后交换排名时间
	UINT32           dwNotifyRank; //最后交换排名
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RACE_BASE_DATA;

typedef struct tagDT_LOCALE_HOME{
	UINT8            byExt; //扩展字段，暂时不用

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCALE_HOME;

typedef struct tagDT_LOCALE_INSTANCE{
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byBattleIdx; //战斗序号
	UINT8            bySuccessFlag; //战斗输赢，0为数，1为赢

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCALE_INSTANCE;

typedef struct tagDT_LOCALE_ONHOOK{
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
	UINT8            byInstanceIdx; //副本序号

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCALE_ONHOOK;

typedef struct tagDT_LOCALE_MARK{
	UINT8            byExt; //扩展字段，暂时不用

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCALE_MARK;

typedef union tagDT_LOCALE_DATA_UNION{
	DT_LOCALE_HOME   stHome; //数值资源信息
	DT_LOCALE_INSTANCE stInstance; //数值资源信息
	DT_LOCALE_ONHOOK stOnhook; //数值资源信息
	DT_LOCALE_MARK   stEatBread; //数值资源信息
	DT_LOCALE_MARK   stBossB; //数值资源信息
	DT_LOCALE_MARK   stHotSpring; //数值资源信息
	DT_LOCALE_MARK   stCourage; //数值资源信息
	DT_LOCALE_MARK   stFaction; //数值资源信息
	DT_LOCALE_MARK   stCourageBattle; //数值资源信息
	DT_LOCALE_MARK   stSoul; //数值资源信息
	DT_LOCALE_MARK   stPet; //数值资源信息
	DT_LOCALE_MARK   stNeiDan; //数值资源信息

	INT32 EncodeSaveData(const UINT8 bySelectType, CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const UINT8 bySelectType, const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCALE_DATA_UNION;

typedef struct tagDT_LOCALE_DATA{
	UINT8            byLocaleType; //位置类型
	DT_LOCALE_DATA_UNION stLocaleInfo; //位置信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCALE_DATA;

typedef struct tagUnlockScienceInfo{
	UINT8            byUnlockNum; //解锁科技数
	UINT8            abyUnlockScienceID[MAX_SCIENCE_ATTR_NUM]; //科技ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}UnlockScienceInfo;

typedef struct tagDT_HERO_UPGRADE_DATA{
	UINT16           wHeroKindID; //武将类型ID
	UINT8            byUpgradeFlag; //是否升级
	UINT16           wLevel; //当前等级
	UINT32           dwEncExperience; //奖励经验
	UINT32           dwExperience; //当前经验
	UINT32           dwUpgradeExperience; //下一级升级经验
	UINT16           wCanUseBoxGoodsID; //可使用礼包物品ID,0为没有

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_UPGRADE_DATA;

typedef struct tagDT_STATE_DATA{
	UINT32           dwInstanceMAXHP; //HP
	UINT32           dwInstanceCurHP; //HP
	UINT16           wDoubleExpNum; //经验倍率副本等次数
	UINT8            byOpenBoxFlag; //是否打开过奖励宝箱
	UINT8            abyHitItemIdx[MAX_ENCOURAGE_BOX_GROUP_NUM]; //命中的宝箱物品序号
	UINT16           awUnlockHeroID[MAX_UNLOCK_HERO_NUM]; //解锁伙伴提示
	UINT16           wUnlockSkillID; //解锁技能提示
	UINT16           wUnlockEquipLevel; //解锁装备提示
	UnlockScienceInfo stUnlockScienceList; //解锁科技提示
	UINT64           qwRegisterDispNameTime; //注册昵称时间，0表示未注册
	UINT32           dwUnlockElite; //解锁精英副本提示(0表示没有，第1个字节表示精英副本城镇idx，第2/3个字节表示主怪物ID)
	UINT8            byUpgradeHeroNum; //升级武将个数
	DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_STATE_DATA;

typedef struct tagDT_MONSTER_BASE_DATA{
	UINT16           wKindID; //主怪物类型
	UINT16           wLevel; //主怪物等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MONSTER_BASE_DATA;

typedef struct tagDT_MONSTER_DATA_LIST{
	UINT16           wMonsterNum; //怪物个数
	DT_MONSTER_BASE_DATA astMonsterInfListo[MAX_INSTANCE_MONSTER_NUM]; //怪物信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MONSTER_DATA_LIST;

typedef struct tagDT_GODWEAPON_BATTLE_DATA{
	UINT16           wQuality; //神器阶级品质，0表示没有
	UINT16           wLevel; //神器等级
	DT_BATTLE_ATTRIBUTE stBattleAttr; //战斗属性
	UINT8            byHPGridNum; //HP格子数
	UINT16           wMaxQuality; //神器最高阶级品质

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GODWEAPON_BATTLE_DATA;

typedef struct tagDT_BATTLE_OBJ_DATA{
	UINT16           wKindID; //类型ID
	UINT8            byCareerID; //职业ID
	UINT16           wLevel; //等级
	UINT8            byFomationIdx; //整形位置(从1开始)
	DT_BATTLE_ATTRIBUTE stBattleAttr; //战斗属性
	UINT16           wSkillActionID; //技能招式ID(0表示没有技能)
	UINT16           wSkillActionLevel; //技能招式等级
	UINT16           wNeedAngry; //释放所需怒气
	UINT16           wInitAngry; //初始怒气
	UINT8            byHPGridNum; //HP格子数
	UINT8            byColor; //颜色

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_OBJ_DATA;

typedef struct tagDT_BATTLE_OBJ_DATA2{
	UINT16           wKindID; //类型ID
	UINT8            byCareerID; //职业ID
	UINT16           wLevel; //等级
	UINT8            byFomationIdx; //整形位置(从1开始)
	DT_BATTLE_ATTRIBUTE stBattleAttr; //战斗属性
	UINT16           wSkillActionID; //技能招式ID(0表示没有技能)
	UINT16           wSkillActionLevel; //技能招式等级
	UINT16           wNeedAngry; //释放所需怒气
	UINT16           wInitAngry; //初始怒气
	UINT8            byHPGridNum; //HP格子数
	UINT8            byColor; //颜色
	DT_GODWEAPON_BATTLE_DATA stGodweaponInfo; //神器战斗信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_OBJ_DATA2;

typedef struct tagDT_BATTLE_OBJ_GROUP_DATA{
	UINT8            byObjNum; //战斗者信息
	DT_BATTLE_OBJ_DATA astBattleObjInfo[MAX_FORMATION_IDX_NUM]; //战斗者信息
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT32           dwPlayerID; //玩家ID（怪物为0）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_OBJ_GROUP_DATA;

typedef struct tagDT_BATTLE_OBJ_GROUP_DATA2{
	UINT8            byObjNum; //战斗者信息
	DT_BATTLE_OBJ_DATA2 astBattleObjInfo[MAX_FORMATION_IDX_NUM]; //战斗者信息
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT32           dwPlayerID; //玩家ID（怪物为0）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_OBJ_GROUP_DATA2;

typedef struct tagDT_BUFF_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT16           wAddDelFlag; //新增/删除标志(0,新增，1删除)
	UINT8            byBuffID; //BuffID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BUFF_DATA;

typedef struct tagDT_HP_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	INT32            nHP; //HP, >0则加血，<0则去血

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HP_DATA;

typedef struct tagDT_ANGRY_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	INT16            shAngry; //怒气, >0则加，<0则去

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ANGRY_DATA;

typedef struct tagDT_PARRY_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT8            byParryFlag; //格挡标志，1格挡,
	UINT8            byBeCritFlag; //被暴击标志，1被暴击,

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PARRY_DATA;

typedef struct tagDT_SKILL_EFFECT_ID_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT16           wEffectID; //效果ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SKILL_EFFECT_ID_DATA;

typedef struct tagDT_ATTACK_OBJ_DATA{
	UINT8            byBuffChangeNum; //buff变化
	DT_BUFF_DATA     astBuffChangeInfo[MAX_BUFF_CHANGE_TIMES_PER_ROUND]; //buff变化
	UINT8            byHpChangeNum; //血量变化,>0则加血，<0则去血
	DT_HP_DATA       astHpChangeInfo[MAX_HP_CHANGE_TIMES_PER_ROUND]; //血量变化,>0则加血，<0则去血
	UINT8            byAngryChangeNum; //怒气变化,>0则加血，<0则去血
	DT_ANGRY_DATA    astAngryChangeInfo[MAX_ANGRY_CHANGE_TIMES_PER_ROUND]; //怒气变化,>0则加血，<0则去血
	UINT8            bySkillEffectIDChangeNum; //技能效果变化
	DT_SKILL_EFFECT_ID_DATA astSkillEffectIDChangeInfo[MAX_BUFF_CHANGE_TIMES_PER_ROUND]; //技能效果变化

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_OBJ_DATA;

typedef struct tagDT_ATTACK_OBJ_DATA2{
	UINT8            byBuffChangeNum; //buff变化
	DT_BUFF_DATA     astBuffChangeInfo[MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND]; //buff变化
	UINT8            byHpChangeNum; //血量变化,>0则加血，<0则去血
	DT_HP_DATA       astHpChangeInfo[MAX_HP_CHANGE_TIMES_PER_ROUND]; //血量变化,>0则加血，<0则去血
	UINT8            byAngryChangeNum; //怒气变化,>0则加血，<0则去血
	DT_ANGRY_DATA    astAngryChangeInfo[MAX_NEW_ANGRY_CHANGE_TIMES_PER_ROUND]; //怒气变化,>0则加血，<0则去血
	UINT8            bySkillEffectIDChangeNum; //技能效果变化
	DT_SKILL_EFFECT_ID_DATA astSkillEffectIDChangeInfo[MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND]; //技能效果变化

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_OBJ_DATA2;

typedef struct tagDT_ATTACK_DATA{
	DT_ATTACK_OBJ_DATA stMyselfObjInfo; //我方信息
	DT_ATTACK_OBJ_DATA stEnemyObjInfo; //敌方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_DATA;

typedef struct tagDT_ATTACK_DATA2{
	DT_ATTACK_OBJ_DATA2 stMyselfObjInfo; //我方信息
	DT_ATTACK_OBJ_DATA2 stEnemyObjInfo; //敌方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_DATA2;

typedef struct tagDT_GODWEAPON_ATTACK_DATA{
	UINT8            byAttackFlag; //攻击标志，0不攻击，1为攻击
	UINT32           dwAttackHurtValue; //攻击对敌方的伤害
	UINT8            byAngryChangeNum; //怒气变化,>0则加血，<0则去血
	DT_ANGRY_DATA    astAngryChangeInfo[MAX_ANGRY_CHANGE_TIMES_PER_ROUND]; //怒气变化,>0则加血，<0则去血

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GODWEAPON_ATTACK_DATA;

typedef struct tagDT_GODWEAPON_ATTACK_DATA2{
	UINT8            byAttackFlag; //攻击标志，0不攻击，1为攻击
	UINT32           dwAttackHurtValue; //攻击对敌方的伤害
	UINT8            byEnemyFormationIdx; //对方阵型位置
	UINT8            byAngryChangeNum; //怒气变化,>0则加血，<0则去血
	DT_ANGRY_DATA    astAngryChangeInfo[MAX_ANGRY_CHANGE_TIMES_PER_ROUND]; //怒气变化,>0则加血，<0则去血

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GODWEAPON_ATTACK_DATA2;

typedef struct tagDT_ATTACK_ALL_DATA{
	UINT8            byAttackerIdx; //攻击方整型序号（从1开始)
	UINT16           wAttackSkillID; //攻击技能招式KindID(0为普通攻击，否则为技能招式ID)
	UINT8            bySkipFlag; //跳过攻击标志(1跳过攻击)
	UINT8            byBeAttackerIdxNum; //受击方格挡/暴击信息（从1开始)
	DT_PARRY_DATA    astBeAttackerIdxInfo[MAX_FORMATION_IDX_NUM]; //受击方格挡/暴击信息（从1开始)
	DT_ATTACK_DATA   stBeforeAttack; //受击前信息
	DT_ATTACK_DATA   stAfterAttack; //受击后信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_ALL_DATA;

typedef struct tagDT_ATTACK_ALL_DATA2{
	UINT8            byAttackerIdx; //攻击方整型序号（从1开始)
	UINT16           wAttackSkillID; //攻击技能招式KindID(0为普通攻击，否则为技能招式ID)
	UINT8            bySkipFlag; //跳过攻击标志(1跳过攻击)
	UINT8            byBeAttackerIdxNum; //受击方格挡/暴击信息（从1开始)
	DT_PARRY_DATA    astBeAttackerIdxInfo[MAX_FORMATION_IDX_NUM]; //受击方格挡/暴击信息（从1开始)
	DT_ATTACK_DATA   stBeforeAttack; //受击前信息
	DT_ATTACK_DATA   stAfterAttack; //受击后信息
	DT_GODWEAPON_ATTACK_DATA stGodweaponAttackInfo; //神器攻击信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_ALL_DATA2;

typedef struct tagDT_ATTACK_ALL_DATA3{
	UINT8            byAttackerIdx; //攻击方整型序号（从1开始)
	UINT16           wAttackSkillID; //攻击技能招式KindID(0为普通攻击，否则为技能招式ID)
	UINT8            bySkipFlag; //跳过攻击标志(1跳过攻击)
	UINT8            byBeAttackerIdxNum; //受击方格挡/暴击信息（从1开始)
	DT_PARRY_DATA    astBeAttackerIdxInfo[MAX_FORMATION_IDX_NUM]; //受击方格挡/暴击信息（从1开始)
	DT_ATTACK_DATA2  stBeforeAttack; //受击前信息
	DT_ATTACK_DATA2  stAfterAttack; //受击后信息
	DT_GODWEAPON_ATTACK_DATA stGodweaponAttackInfo; //神器攻击信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_ALL_DATA3;

typedef struct tagDT_BATTLE_ANGRY_DATA{
	UINT8            bySlot; //技能槽(从0开始)
	UINT16           wCurAngry; //当前怒气

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ANGRY_DATA;

typedef struct tagDT_BATTLE_ROUND_DATA{
	UINT8            byAttackerFlag; //攻击者标志，0为我方
	DT_ATTACK_ALL_DATA stAttackData; //攻击信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ROUND_DATA;

typedef struct tagDT_BATTLE_ROUND_DATA2{
	UINT8            byAttackerFlag; //攻击者标志，0为我方
	DT_ATTACK_ALL_DATA2 stAttackData; //攻击信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ROUND_DATA2;

typedef struct tagDT_BATTLE_ROUND_DATA3{
	UINT8            byAttackerFlag; //攻击者标志，0为我方
	DT_ATTACK_ALL_DATA3 stAttackData; //攻击信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ROUND_DATA3;

typedef struct tagDT_BATTLE_DATA{
	DT_BATTLE_OBJ_GROUP_DATA stMyselfInfo; //我方信息(发起方/请求者)
	DT_BATTLE_OBJ_GROUP_DATA stEnemyInfo; //敌方信息
	UINT8            byBattleResult; //挑战战斗结果，0为我方赢，1对方赢，2我方赢并副本通关
	UINT8            byScore; //战斗评分
	UINT8            byBattleRoundNum; //战斗回合数
	DT_BATTLE_ROUND_DATA astBattleRoundInfo[MAX_BATTLE_ROUND_NUM]; //战斗回合信息
	DT_BATTLE_ATTRIBUTE stMyselfInfoAfterBattle; //战斗后我方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_DATA;

typedef struct tagDT_BATTLE_DATA2{
	DT_BATTLE_OBJ_GROUP_DATA2 stMyselfInfo; //我方信息(发起方/请求者)
	DT_BATTLE_OBJ_GROUP_DATA2 stEnemyInfo; //敌方信息
	UINT8            byBattleResult; //挑战战斗结果，0为我方赢，1对方赢，2我方赢并副本通关
	UINT8            byScore; //战斗评分
	UINT8            byBattleRoundNum; //战斗回合数
	DT_BATTLE_ROUND_DATA2 astBattleRoundInfo[MAX_BATTLE_ROUND_NUM]; //战斗回合信息
	DT_BATTLE_ATTRIBUTE stMyselfInfoAfterBattle; //战斗后我方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_DATA2;

typedef struct tagDT_BATTLE_DATA3{
	DT_BATTLE_OBJ_GROUP_DATA2 stMyselfInfo; //我方信息(发起方/请求者)
	DT_BATTLE_OBJ_GROUP_DATA2 stEnemyInfo; //敌方信息
	UINT8            byBattleResult; //挑战战斗结果，0为我方赢，1对方赢，2我方赢并副本通关
	UINT8            byScore; //战斗评分
	UINT8            byBattleRoundNum; //战斗回合数
	DT_BATTLE_ROUND_DATA3 astBattleRoundInfo[MAX_NEW_BATTLE_ROUND_NUM]; //战斗回合信息
	DT_BATTLE_ATTRIBUTE stMyselfInfoAfterBattle; //战斗后我方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_DATA3;

typedef struct tagDT_SHIELD_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT8            byAddDelFlag; //新增/删除标志(0,新增，1删除)
	UINT8            bySheldID; //SheldID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SHIELD_DATA;

typedef struct tagDT_HP_DATA2{
	UINT8            byFormationIdx; //整型序号（从1开始)
	INT32            nHP; //HP, >0则加血，<0则去血
	UINT8            byEnemyIdx; //作用对象
	UINT8            byReason; //掉血原因

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HP_DATA2;

typedef struct tagDT_ATTACK_OBJ_DATA3{
	UINT8            byBuffChangeNum; //buff变化
	DT_BUFF_DATA     astBuffChangeInfo[MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND]; //buff变化
	UINT8            byHpChangeNum; //血量变化,>0则加血，<0则去血
	DT_HP_DATA2      astHpChangeInfo[MAX_HP_CHANGE_TIMES_PER_ROUND]; //血量变化,>0则加血，<0则去血
	UINT8            byAngryChangeNum; //怒气变化,>0则加血，<0则去血
	DT_ANGRY_DATA    astAngryChangeInfo[MAX_NEW_ANGRY_CHANGE_TIMES_PER_ROUND]; //怒气变化,>0则加血，<0则去血
	UINT8            bySkillEffectIDChangeNum; //技能效果变化
	DT_SKILL_EFFECT_ID_DATA astSkillEffectIDChangeInfo[MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND]; //技能效果变化
	UINT8            byShieldChangeNum; //防护罩变化
	DT_SHIELD_DATA   astShieldChangeInfo[MAX_FORMATION_IDX_NUM]; //防护罩变化

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_OBJ_DATA3;

typedef struct tagDT_ATTACK_DATA3{
	DT_ATTACK_OBJ_DATA3 stMyselfObjInfo; //我方信息
	DT_ATTACK_OBJ_DATA3 stEnemyObjInfo; //敌方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_DATA3;

typedef struct tagDT_PARRY_DATA2{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT8            byParryFlag; //格挡标志，1格挡,
	UINT8            byBeCritFlag; //被暴击标志，1被暴击,
	UINT8            byDodge; //闪避标志，1闪避

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PARRY_DATA2;

typedef struct tagDT_ATTACK_ALL_DATA4{
	UINT8            byAttackerIdx; //攻击方整型序号（从1开始)
	UINT16           wAttackSkillID; //攻击技能招式KindID(0为普通攻击，否则为技能招式ID)
	UINT8            bySkipFlag; //跳过攻击标志(1跳过攻击)
	UINT8            byBeAttackerIdxNum; //受击方格挡/暴击信息（从1开始)
	DT_PARRY_DATA2   astBeAttackerIdxInfo[MAX_FORMATION_IDX_NUM]; //受击方格挡/暴击信息（从1开始)
	DT_ATTACK_DATA3  stBeforeAttack; //受击前信息
	DT_ATTACK_DATA3  stAfterAttack; //受击后信息
	DT_GODWEAPON_ATTACK_DATA2 stGodweaponAttackInfo; //神器攻击信息
	UINT8            byAttackSpecialEffectFlag; //特殊攻击标志EBattleSpecialEffects
	UINT8            byEnemySpecialEffectFlag; //特殊攻击标志EBattleSpecialEffects

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_ALL_DATA4;

typedef struct tagDT_BATTLE_ROUND_DATA4{
	UINT8            byAttackerFlag; //攻击者标志，0为我方
	DT_ATTACK_ALL_DATA4 stAttackData; //攻击信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ROUND_DATA4;

typedef struct tagDT_BATTLE_DATA4{
	DT_BATTLE_OBJ_GROUP_DATA2 stMyselfInfo; //我方信息(发起方/请求者)
	DT_BATTLE_OBJ_GROUP_DATA2 stEnemyInfo; //敌方信息
	UINT8            byBattleResult; //挑战战斗结果，0为我方赢，1对方赢，2我方赢并副本通关
	UINT8            byScore; //战斗评分
	UINT8            byBattleRoundNum; //战斗回合数
	DT_BATTLE_ROUND_DATA4 astBattleRoundInfo[MAX_NEW_BATTLE_ROUND_NUM]; //战斗回合信息
	DT_BATTLE_ATTRIBUTE stMyselfInfoAfterBattle; //战斗后我方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_DATA4;

typedef struct tagDT_ATTACK_ALL_DATA5{
	UINT8            byAttackerIdx; //攻击方整型序号（从1开始)
	UINT16           wAttackSkillID; //攻击技能招式KindID(0为普通攻击，否则为技能招式ID)
	UINT8            bySkipFlag; //跳过攻击标志(1跳过攻击)
	UINT8            byBeAttackerIdxNum; //受击方格挡/暴击信息（从1开始)
	DT_PARRY_DATA2   astBeAttackerIdxInfo[MAX_FORMATION_IDX_NUM]; //受击方格挡/暴击信息（从1开始)
	DT_ATTACK_DATA3  stBeforeAttack; //受击前信息
	DT_ATTACK_DATA3  stAfterAttack; //受击后信息
	DT_GODWEAPON_ATTACK_DATA2 stGodweaponAttackInfo; //神器攻击信息
	UINT8            byAttackSpecialEffectFlag; //特殊攻击标志EBattleSpecialEffects
	UINT8            byEnemySpecialEffectFlag; //特殊攻击标志EBattleSpecialEffects
	UINT8            byPetAttack; //是否是宠物攻击0不是，1宠物攻击
	UINT16           wPetSkillID; //宠物攻击技能ID
	UINT8            byAttackObj; //攻击乱方还是已方commdef.h AttackTarget

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ATTACK_ALL_DATA5;

typedef struct tagDT_BATTLE_ROUND_DATA5{
	UINT8            byAttackerFlag; //攻击者标志，0为我方
	DT_ATTACK_ALL_DATA5 stAttackData; //攻击信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_ROUND_DATA5;

typedef struct tagDT_PET_BATTLE_DATA{
	UINT16           wPetID; //宠物ID
	UINT16           wPetLevel; //宠物Level
	UINT8            byPetQuality; //宠物品质

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PET_BATTLE_DATA;

typedef struct tagDT_BATTLE_OBJ_GROUP_DATA3{
	UINT8            byObjNum; //战斗者信息
	DT_BATTLE_OBJ_DATA2 astBattleObjInfo[MAX_FORMATION_IDX_NUM]; //战斗者信息
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT32           dwPlayerID; //玩家ID（怪物为0）
	DT_PET_BATTLE_DATA stPetInfo; //宠物信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_OBJ_GROUP_DATA3;

typedef struct tagDT_BATTLE_DATA5{
	DT_BATTLE_OBJ_GROUP_DATA3 stMyselfInfo; //我方信息(发起方/请求者)
	DT_BATTLE_OBJ_GROUP_DATA3 stEnemyInfo; //敌方信息
	UINT8            byBattleResult; //挑战战斗结果，0为我方赢，1对方赢，2我方赢并副本通关
	UINT8            byScore; //战斗评分
	UINT8            byBattleRoundNum; //战斗回合数
	DT_BATTLE_ROUND_DATA5 astBattleRoundInfo[MAX_NEW_BATTLE_ROUND_NUM]; //战斗回合信息
	DT_BATTLE_ATTRIBUTE stMyselfInfoAfterBattle; //战斗后我方信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_DATA5;

typedef struct tagDT_BATTLE_OBJ_MAX_HP{
	UINT8            byFomationIdx; //阵型位置(从1开始)
	UINT32           dwMaxHP; //满血HP

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_OBJ_MAX_HP;

typedef struct tagDT_BATTLE_OBJ_MAX_HP_INFO{
	UINT8            byObjNum; //战斗者个数
	DT_BATTLE_OBJ_MAX_HP astBattleObjMaxHPInfo[MAX_FORMATION_IDX_NUM]; //战斗者信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_OBJ_MAX_HP_INFO;

typedef struct tagDT_EMPTY_HP_VIDEO_v400{
	DT_BATTLE_OBJ_MAX_HP_INFO stMyselfMaxHPInfo; //本方阵上各伙伴的最大血量
	DT_BATTLE_OBJ_MAX_HP_INFO stEnemyMaxHPInfo; //敌方阵上各伙伴的最大血量
	DT_BATTLE_OBJ_MAX_HP_INFO stMyselfBeforeRecoverHPInfo; //本方回血前阵上各伙伴的当前血量
	DT_BATTLE_OBJ_MAX_HP_INFO stEnemyBeforeRecoverHPInfo; //敌方回血前阵上各伙伴的当前血量
	DT_BATTLE_DATA4  stEmptyHPVideo; //录像数据

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EMPTY_HP_VIDEO_v400;

typedef struct tagDT_EMPTY_HP_VIDEO{
	DT_BATTLE_OBJ_MAX_HP_INFO stMyselfMaxHPInfo; //本方阵上各伙伴的最大血量
	DT_BATTLE_OBJ_MAX_HP_INFO stEnemyMaxHPInfo; //敌方阵上各伙伴的最大血量
	DT_BATTLE_OBJ_MAX_HP_INFO stMyselfBeforeRecoverHPInfo; //本方回血前阵上各伙伴的当前血量
	DT_BATTLE_OBJ_MAX_HP_INFO stEnemyBeforeRecoverHPInfo; //敌方回血前阵上各伙伴的当前血量
	DT_BATTLE_DATA5  stEmptyHPVideo; //录像数据

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EMPTY_HP_VIDEO;

typedef struct tagDT_FINAL_ROUND_BATTLE_OBJ_INFO{
	DT_BATTLE_OBJ_GROUP_DATA3 stMyselfObj; //本方阵上各伙伴的最大血量
	DT_BATTLE_OBJ_GROUP_DATA3 stEnemyObj; //敌方阵上各伙伴的最大血量

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FINAL_ROUND_BATTLE_OBJ_INFO;

typedef struct tagDT_ONLINE_ENCOURAGE_RECORD_DATA{
	UINT8            byRecvDays; //已领取天数
	UINT64           qwLastRecvTime; //上次领取时间
	UINT8            byLastRecvIndex; //上次领取到的序号

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ONLINE_ENCOURAGE_RECORD_DATA;

typedef struct tagDT_LOGIN_ENCOURAGE_RECORD_DATA{
	UINT16           wKeepDays; //连续登录天数
	UINT8            byLastRecvNum; //上次领取次数
	UINT64           qwLastRecvTime; //上次领取时间
	UINT8            byLastSelectIndex; //上次选中的序号(从1开始)
	UINT8            byStateFlag; //状态(LastSelectIndex不为0时有效)(0已下发，1已选择，2已入包)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOGIN_ENCOURAGE_RECORD_DATA;

typedef struct tagDT_RACE_BATTLELOG_DATA{
	UINT8            byChallengeFlag; //挑战标志,0为主动挑战，1为被挑战
	UINT64           qwChallengeTime; //挑战时间
	UINT32           dwEnemyPlayerID; //对方玩家ID
	UINT8            byResult; //挑战结果，0为我方赢，1为对方赢
	UINT32           dwBeginChallengeRank; //挑战前排名
	UINT32           dwAfterChallengeRank; //挑战后排名
	UINT8            byNewFlag; //是否新战报，1为新

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RACE_BATTLELOG_DATA;

typedef struct tagDT_RACE_BATTLELOG_DATA_LIST{
	UINT8            byBattleLogNum; //战报个数
	DT_RACE_BATTLELOG_DATA astBattleLogInfo[MAX_RACE_BATTLELOG_NUM]; //战报信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RACE_BATTLELOG_DATA_LIST;

typedef struct tagDT_CAPTURE_DATA{
	UINT32           dwCapturePlayerID; //被俘虏的玩家ID
	UINT64           qwCaptureTime; //俘虏的时间
	UINT8            byCaptureGridIdx; //俘虏格子序号(从0开始)
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	UINT64           qwLastCollectTime; //上次收取收益时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CAPTURE_DATA;

typedef struct tagDT_CAPTURE_DATA_LIST{
	UINT8            byCaptureNum; //俘虏个数
	DT_CAPTURE_DATA  astCaptureInfo[MAX_CAPTURE_NUM]; //俘虏信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CAPTURE_DATA_LIST;

typedef struct tagDT_PLUNDER_BATTLELOG_DATA{
	UINT8            byPlunderFlag; //掠夺标志
	UINT64           qwPlunderTime; //掠夺时间(标志7/8为自动释放时间)
	UINT32           dwActivePlayerID; //主动方玩家ID(标志7/8为释放俘虏的俘虏主ID)
	UINT32           dwPassivePlayerID; //被动方玩家ID标志7/8为被释放的俘虏ID)
	UINT32           dwCapturePlayerID; //俘虏玩家ID
	UINT8            byResult; //结果，0为主动方赢，1为被动方赢
	UINT64           qwPlunderCoin; //掠夺铜币
	UINT64           qwPlunderScience; //掠夺器魂点
	UINT8            byNewFlag; //是否新战报，1为新

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLUNDER_BATTLELOG_DATA;

typedef struct tagDT_PLUNDER_BATTLELOG_DATA_LIST{
	UINT8            byBattleLogNum; //战报个数
	DT_PLUNDER_BATTLELOG_DATA astBattleLogInfo[MAX_PLUNDER_BATTLELOG_NUM]; //战报信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLUNDER_BATTLELOG_DATA_LIST;

typedef struct tagDT_PLUNDER_GRID_DATA{
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	UINT32           dwIncomeValue; //收益值

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLUNDER_GRID_DATA;

typedef struct tagDT_PLUNDER_BASE_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT32           dwCaptureOwnerPlayerID; //俘虏你的玩家ID，0表示没有被俘虏
	UINT64           qwBeCaptureTime; //被俘虏的时间
	UINT16           wExtPlunderNum; //额外增加的掠夺次数
	UINT64           qwLastPlunderTime; //上次掠夺时间
	UINT16           wLastPlunderNum; //上次(当天)掠夺次数
	UINT8            byUnreadBattleLogNum; //未观看战报个数
	UINT64           qwLastDriveAwayTime; //上次驱赶俘虏主时间
	UINT16           wLastDriveAwayNum; //上次(当天)驱赶俘虏主次数
	DT_PLUNDER_GRID_DATA astCaptureGridInfo[MAX_CAPTURE_NUM]; //俘虏格子信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLUNDER_BASE_DATA;

typedef struct tagDT_ENEMY_DATA{
	UINT32           dwEnemyPlayerID; //仇敌玩家ID
	UINT16           wBattleNum; //交手次数
	UINT64           qwAddTime; //添加时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ENEMY_DATA;

typedef struct tagDT_ENEMY_DATA_LIST{
	UINT8            byEnemyNum; //战报个数
	DT_ENEMY_DATA    astEnemyInfo[MAX_ENEMY_NUM]; //战报信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ENEMY_DATA_LIST;

typedef struct tagDT_MONEY_TREE_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT16           wRockNum; //上次已摇次数
	UINT64           qwRockTime; //上次摇的时间
	UINT8            byMoodValue; //上次心情值
	UINT64           qwMoodStartTime; //上次心情值开始时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MONEY_TREE_DATA;

typedef struct tagDT_INSTANCE_BATTLE_DATA{
	DT_INSTANCE_ID   stInstanceID; //副本序号
	UINT8            byBattleIndex; //战斗序号
	UINT64           qwBattleTime; //上次战斗时间
	UINT16           wMaxRounds; //最大攻击回合数
	UINT16           wMaxPercentageHp; //最大剩余血量百分比

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_INSTANCE_BATTLE_DATA;

typedef struct tagDT_PHYSTRENGTH_DATA{
	UINT64           qwLastPhyStrengthRefreshTime; //上次体力刷新时间
	UINT64           qwLastBuyPhyStrengthTime; //上次购买体力时间
	UINT16           wLastBuyPhyStrengthNum; //上次购买体力次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PHYSTRENGTH_DATA;

typedef struct tagDT_ONHOOK_RESULT{
	UINT8            byCurRunTimes; //当前扫荡次数
	UINT8            byAllRunTimes; //总共扫荡次数
	UINT32           dwStory; //阅历
	UINT32           dwExperience; //奖励经验
	UINT32           dwCoin; //铜钱
	UINT32           dwGold; //元宝
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA     astItemInfoList[MAX_ENCOURAGE_ITEM_KIND_NUM]; //道具信息
	UINT32           dwBlueGas; //蓝气

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ONHOOK_RESULT;

typedef struct tagDT_ONHOOK_RCD_DATA{
	UINT8            byOnhookFlag; //是否挂机中
	DT_ONHOOK_RESULT stOnhookResult; //挂机结果记录
	DT_INSTANCE_ID   stOnhookInstanceID; //挂机副本,wSceneIdx为0表示没有挂机
	UINT64           qwLastOnhookResultReqTime; //上次请求挂机结果时间
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ONHOOK_RCD_DATA;

typedef struct tagDT_FEW_PARAM_DATA{
	UINT64           qwLastStrengthenTime; //上次强化时间
	UINT16           wLastStrengthenCD; //上次强化累计CD
	UINT64           qwLastRefreshEliteTime; //上次刷新精英副本时间
	UINT16           wLastRefreshEliteNum; //上次刷新精英副本次数
	UINT64           qwCountTime; //上次统计刷新时间
	UINT16           wGVETimes; //勇气试练通关次数
	UINT16           wRaceSuccess; //竞技场成功次数
	UINT16           wRaceFailed; //竞技场失败次数
	UINT16           wTrialSuccess; //试炼场成功次数
	UINT16           wTrialFailed; //试炼场失败次数
	UINT16           wInterceptSuccess; //西天取经拦截成功次数
	UINT16           wInterceptFailed; //西天取经拦截失败次数
	UINT16           wAwakenNum; //精魄卦象个数
	UINT16           wResetTowerNum; //重置爬塔
	UINT16           wWorldPassNum; //人间通关次数
	UINT16           wHellPassNum; //地狱通关次数
	UINT16           wHeavenPassNum; //天堂通关次数
	UINT16           wXiJingHorse; //草泥马
	UINT16           wXiJingThird; //沙僧
	UINT16           wXiJingSecond; //猪八戒
	UINT16           wXiJingFirst; //大师兄
	UINT16           wXiJingMaster; //师父
	UINT16           wExperienceDrug; //3级经验丹
	UINT16           wPassNaiDanNum; //通关斩兽次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FEW_PARAM_DATA;

typedef struct tagDT_CROP_INFO_DATA{
	UINT8            byOpen; //种植ID
	UINT16           wCropKindID; //种植ID
	UINT64           qwLastCropTime; //种植时间
	UINT8            byFieldLevel; //田地等级
	UINT32           dwSeedNum; //种子数量

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CROP_INFO_DATA;

typedef struct tagDT_CROP_DATA{
	UINT8            byHaveDataFlag; //是否保存数据库
	DT_CROP_INFO_DATA astCropInfoList[MAX_CROP_NUM]; //种植信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CROP_DATA;

typedef struct tagDT_SCIENCE_TREE_DATA{
	UINT16           wAttractLevel; //神力
	UINT16           wHPLevel; //长寿
	UINT16           wCritLevel; //暴击
	UINT16           wShieldLevel; //护盾
	UINT16           wHitLevel; //鹰眼
	UINT16           wDodgeLevel; //闪避
	UINT32           dwFirstAttack; //先攻
	UINT16           wCoachTalent; //主角资质
	UINT16           wAttractLevel2; //神力2
	UINT16           wHPLevel2; //长寿2
	UINT8            byHaveDataFlag; //是否保存数据库
	UINT16           wQingLogLevel; //青龙阵
	UINT16           wBaiHuLevel; //白虎阵
	UINT16           wZhuQueLevel; //朱雀阵
	UINT16           wXuanWuLevel; //玄武阵

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SCIENCE_TREE_DATA;

typedef struct tagDT_FORMATION_HERO_DATA{
	UINT8            byFormationIdx; //阵型位置(从1开始，1～6)
	UINT16           wHeroID; //武将ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FORMATION_HERO_DATA;

typedef struct tagDT_FORMATION_IDX_DATA{
	UINT8            byFormationKind; //阵型类型
	UINT8            byOpenIdxNum; //阵型开启位置数
	DT_FORMATION_HERO_DATA astOpenIdxInfo[MAX_FORMATION_IDX_NUM]; //阵型开启位置信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FORMATION_IDX_DATA;

typedef struct tagDT_FORMATION_DATA_LIST{
	UINT8            byFormationKindNum; //阵型类型数
	DT_FORMATION_IDX_DATA astFormationInfo[MAX_FORMATION_NUM]; //阵型信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FORMATION_DATA_LIST;

typedef struct tagDT_FORMATION_DATA{
	UINT8            bySelectIdx; //已选阵型序号（从0开始）
	DT_FORMATION_DATA_LIST stFormationList; //阵型信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FORMATION_DATA;

typedef struct tagDT_NEW_FORMATION_POSTION_INFO{
	UINT8            byPostionIndex; //位置序号
	UINT16           wHeroID; //HeroID
	UINT8            byIsUnlock; //解锁状态（0未解锁, 1解锁）
	UINT8            byNeedLevel; //解锁需要等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NEW_FORMATION_POSTION_INFO;

typedef struct tagDT_NEW_FORMATION_INFO{
	UINT8            byFormationID; //阵型ID
	UINT8            byPostionNum; //位置个数
	DT_NEW_FORMATION_POSTION_INFO astPostionInfo[MAX_FORMATION_IDX_NUM]; //位置信息
	UINT8            byFormationLevel; //阵型等级信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NEW_FORMATION_INFO;

typedef struct tagDT_NEW_FORMATION_DATA_LST{
	UINT8            byFormationNum; //阵型个数
	DT_NEW_FORMATION_INFO astFormationArray[MAX_NEW_FORMATION_NUM]; //阵型数组信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NEW_FORMATION_DATA_LST;

typedef struct tagDT_NEW_FORMATION_DATA{
	UINT8            bySelectIdx; //已选阵型序号（从0开始）
	DT_FORMATION_DATA_LIST stFormationList; //阵型信息
	UINT8            byNewSelectIdx; //已选新阵型序号
	DT_NEW_FORMATION_DATA_LST stNewFormationLst; //新阵型信息（从0开始）
	DT_NEW_FORMATION_DATA_LST stLockFormationLst; //未解锁阵型
	UINT8            byIsUseNewFormation; //是否使用新阵型（1为已经使用）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NEW_FORMATION_DATA;

typedef struct tagDT_VERSION_DATA{
	UINT32           dwVersionID; //版本ID,取2位小数点(/100)
	UINT8            byForceUpdateFlag; //是否强制更新，1强制更新
	TCHAR            aszUpdateContent[MAX_CONTENT_SIZE]; //更新内容
	TCHAR            aszUpdateURL[MAX_CONTENT_SIZE]; //更新地址

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_VERSION_DATA;

typedef struct tagDT_NOTICE_DATA{
	UINT32           dwNoticeID; //公告ID
	UINT8            byForcePopFlag; //是否必须弹,1为必须弹出
	UINT8            byUrlFlag; //公告为url标志
	TCHAR            aszNoticeContent[MAX_CONTENT_SIZE]; //公告内容

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NOTICE_DATA;

typedef struct tagDT_NOTICE_DATA_DB{
	UINT32           dwNoticeID; //公告ID
	UINT8            byForcePopFlag; //是否必须弹,1为必须弹出
	UINT8            byUrlFlag; //公告为url标志
	TCHAR            aszNoticeContent[MAX_NOTICE_CONTENT_LEN]; //公告内容
	UINT64           qwExpiresTime; //过期时间
	UINT32           dwBeginChannelID; //开始渠道ID
	UINT32           dwEndChannelID; //结束渠道ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NOTICE_DATA_DB;

typedef struct tagDT_NOTICE_DATA_NEW{
	UINT32           dwNoticeID; //公告ID
	UINT8            byForcePopFlag; //是否必须弹,1为必须弹出
	UINT8            byUrlFlag; //公告为url标志
	TCHAR            aszNoticeContent[MAX_NOTICE_CONTENT_LEN]; //公告内容

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NOTICE_DATA_NEW;

typedef struct tagDT_EAT_BREAD_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT64           qwEnterTime; //进入活动时间，0表示没有
	UINT8            byHaveObtainPhyStrengthen; //已得到的体力(从活动开始到当前)
	UINT32           dwHaveObtainCoin; //已得到的铜钱(从活动开始到当前)
	UINT8            byHaveLeaveFlag; //是否已离开
	UINT16           wHaveUseTime; //已用时间
	UINT8            byHaveGivePhyStrengthen; //已结算的体力
	UINT32           dwHaveGiveCoin; //已结算的铜钱
	UINT8            byNotity; //是否打开通知

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EAT_BREAD_DATA;

typedef struct tagDT_YABIAO_BASE_DATA{
	UINT64           qwLastYabiaoTime; //上次开始押镖的时间（可以用来判断是否换天可以刷新了）
	UINT8            byAlreadyYabiaoCount; //已经押镖的次数
	UINT8            byQuality; //镖的品质
	UINT8            byBeJiebiaoCount; //被劫镖的次数
	UINT16           wBiaoRunTime; //镖在跑的时间(秒为单位)
	UINT16           wBiaoFlushCount; //当前镖已经刷的次数
	UINT8            byYabiaoFlag; //当前押镖状态(0不在押镖，1在押镖，2押镖完成)
	UINT8            byAlreadyJiebiaoCount; //已经劫镖次数
	UINT64           qwLastJiebiaoTime; //上次劫镖时间
	UINT64           qwLastEncourageCoin; //上次押镖的铜钱奖励
	UINT64           qwLastEncourageScience; //上次押镖的器魂奖励
	UINT64           qwBeJiebiaoTime; //被劫镖的时间
	UINT64           qwLastEncourageJingjie; //镖的境界奖励
	UINT32           dwJiebiaoDecCoin; //劫镖丢失的金钱
	UINT32           dwJiebiaoDecScience; //劫镖丢失的器魂
	UINT32           dwAlreadyFlushBiaoCnt; //已刷新镖的总次数
	UINT64           qwLastFlushTime; //上次刷新镖的时间
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_YABIAO_BASE_DATA;

typedef struct tagDT_YABIAO_BATTLELOG_DATA{
	UINT8            byFlag; //劫镖标志,0为主动劫镖，1为被劫镖, 2为押镖成功
	UINT64           qwBattleTime; //战斗时间
	UINT32           dwEnemyPlayerID; //对方玩家ID
	UINT8            byResult; //结果，0为我方赢，1为对方赢
	UINT32           dwCoinChange; //获得或者损失铜钱（看劫镖标志）
	UINT32           dwScienceChange; //获得或者损失器魂（看劫镖标志）
	UINT8            byNewFlag; //是否新战报，1为新
	UINT64           qwBattleDataIndex; //战斗索引
	UINT8            byQuality; //镖的品质
	UINT32           dwJingjieChange; //获得或者损失境界点（看劫镖标志）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_YABIAO_BATTLELOG_DATA;

typedef struct tagDT_YABIAO_BATTLELOG_DATA_LIST{
	UINT8            byBattleLogNum; //战报个数
	DT_YABIAO_BATTLELOG_DATA astPaoshangBattleLogInfo[MAX_YABIAO_BATTLELOG_NUM]; //战报信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_YABIAO_BATTLELOG_DATA_LIST;

typedef struct tagDT_SKILL_ACTION_DATA{
	UINT16           wID; //ID
	UINT16           wLevel; //等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SKILL_ACTION_DATA;

typedef struct tagDT_SKILL_ACTION_DATA_LIST{
	UINT8            byActionNum; //技能招式个数
	DT_SKILL_ACTION_DATA astActionInfo[MAX_SKILL_SLOT_NUM]; //技能招式信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SKILL_ACTION_DATA_LIST;

typedef struct tagDT_JINGJIE_BASE_DATA{
	UINT8            byCareerID; //职业
	UINT16           wLevel; //等级
	UINT16           wSubLevel; //子等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_JINGJIE_BASE_DATA;

typedef struct tagDT_GAS_BALL_DATA{
	UINT8            byKind; //精气球类型
	UINT8            byLevel; //精气球类型
	UINT32           dwGasValue; //精气值

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GAS_BALL_DATA;

typedef struct tagDT_COLLECT_GAS_DATA{
	UINT8            byCollectState; //收集状态,0生成，1收取
	DT_GAS_BALL_DATA astGasBollInfo[MAX_GAS_BALL_NUM]; //精气球信息
	UINT16           wLastCoinGenNum; //上次铜钱生产次数
	UINT64           qwLastCoinGenTime; //上次铜钱生产时间
	UINT16           wLastCoinRandomUpNum; //上次铜钱随机提升次数
	UINT64           qwLastCoinRandomUpTime; //上次铜钱随机提升时间
	UINT8            byLastCoinFullNum; //上次铜钱一键成长次数
	UINT64           qwLastCoinFullTime; //上次铜钱一键成长时间
	UINT16           wLastGoldGenNum; //上次元宝生产次数
	UINT64           qwLastGoldGenTime; //上次元宝生产时间
	UINT16           wLastGoldRandomUpNum; //上次元宝随机提升次数
	UINT64           qwLastGoldRandomUpTime; //上次元宝随机提升时间
	UINT8            byLastGoldFullUpNum; //上次元宝一键成长次数
	UINT64           qwLastGoldFullUpTime; //上次元宝一键成长时间
	UINT8            byHaveDataFlag; //是否保存数据库

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COLLECT_GAS_DATA;

typedef struct tagDT_JINGJIE_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	DT_JINGJIE_BASE_DATA stBaseData; //境界基本信息
	DT_COLLECT_GAS_DATA stCollectData; //精气收集信息
	DT_SKILL_ACTION_DATA_LIST stSkillActionData; //技能招式信息列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_JINGJIE_DATA;

typedef struct tagDT_CLIMB_TOWER_DATA{
	UINT16           wResetClimbTownNum; //可重置次数
	UINT64           qwResetClimbTownTime; //最后重置时间
	UINT16           wBuyResetClimbTownNum; //每天可购买次数
	UINT64           qwBuyResetClimbTownTime; //每天最后购买时间
	UINT8            byClimbTownInstance; //最后玩家层数
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CLIMB_TOWER_DATA;

typedef struct tagDT_ELITE_INSTANCE_RESET_DATA{
	UINT16           wResetEliteInstanceNum; //重置精英副本次数
	UINT64           qwLastResetEliteInstanceTime; //上次重置精英副本的时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ELITE_INSTANCE_RESET_DATA;

typedef struct tagDT_VIP_GET_NUM_EACH_DATA{
	UINT16           wVipHaveNum; //每一VIP等级对应的可领取的次数
	UINT16           wVipGetNum; //每一VIP等级对应的已领取的次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_VIP_GET_NUM_EACH_DATA;

typedef struct tagDT_VIP_GET_NUM_LEVEL_DATA{
	UINT16           wMaxVipGift; //每一VIP等级对应的可领取的次数
	DT_VIP_GET_NUM_EACH_DATA astVipGetNum[MAX_VIP_GIFT]; //每一VIP等级对应的已领取的次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_VIP_GET_NUM_LEVEL_DATA;

typedef struct tagDT_VIP_EXT_DATA{
	UINT16           wEatGoldExperienceCnt; //伙伴服用元宝经验丹次数
	UINT64           qwLastEatTime; //上次服用元宝经验丹的时间
	UINT16           wEliteInstanceTownNum; //精英副本城镇数
	DT_ELITE_INSTANCE_RESET_DATA astEliteInstanceData[MAX_ELITE_INSTANCE_TOWN_NUM]; //精英副本重置数据
	UINT64           qwLastGetTime; //最后一次领取礼包的时间
	UINT8            byVipMaxLevel; //VIP最大等级
	DT_VIP_GET_NUM_LEVEL_DATA astVipGiftNumByLevel[MAX_VIP_LEVEL]; //每一VIP等级对应的可领取的次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_VIP_EXT_DATA;

typedef struct tagDT_LUAACTIVITY_RECORD_DATA{
	UINT32           dwID; //ID
	UINT16           wFinishNum; //完成次数
	UINT64           qwFinishTime; //上次完成时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LUAACTIVITY_RECORD_DATA;

typedef struct tagDT_LUAACTIVITY_RECORD_LIST_DATA{
	UINT8            byRecordNum; //记录数
	DT_LUAACTIVITY_RECORD_DATA astHaveFinishActivityInfo[MAX_LUAACTIVITY_RECORD_NUM]; //已完成活动ID记录信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LUAACTIVITY_RECORD_LIST_DATA;

typedef struct tagDT_BOSSB_HURT_RATE{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT32           dwHurtValue; //玩家对Boss的伤害(若伤害通知为当时单次伤害值)
	UINT32           dwHurtRate; //玩家对Boss的伤害比例（万分比）
	UINT32           dwHurtRank; //玩家对Boss的伤害排名

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSSB_HURT_RATE;

typedef struct tagDT_BOSS_BATTLE_ENC_DATA{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT32           dwCoin; //铜钱
	UINT32           dwJingJie; //境界

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSS_BATTLE_ENC_DATA;

typedef struct tagDT_BOSS_BATTLE_PLAYER_DATA{
	UINT64           qwLastEnterActivityTime; //上次进入活动时间
	UINT64           qwLastLeaveActivityTime; //上次离开活动时间
	UINT16           wIncPowerRate; //提升战力百分比
	UINT32           dwTotalHurtValue; //已经造成的伤害值
	UINT32           dwHurtRank; //玩家对Boss的伤害排名
	UINT32           dwEncCoin; //伤害铜钱
	UINT32           dwEncJingJie; //伤害境界
	UINT16           wPosX; //位置信息X
	UINT16           wPosY; //位置信息Y
	UINT32           dwSyncStartIdx; //同步的起始Idx
	UINT8            byLastReliveType; //上次复活方式，0普通复活，1元宝复活
	UINT64           qwLastReliveTime; //上次复活时间
	UINT64           qwLastDeadTime; //上次死亡时间
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自身攻击伤害
	DT_BOSS_BATTLE_ENC_DATA stHurtEncInfo; //伤害奖励信息
	DT_BOSS_BATTLE_ENC_DATA stHeroEncInfo; //英雄奖励信息
	UINT8            byUnreadBattleLogNum; //未观看战报个数
	UINT8            byNotity; //是否打开通知
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT64           qwLastKillBossTime; //上次击杀BOSS时间
	UINT64           qwBossHurtTime; //上次伤害超过1%时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSS_BATTLE_PLAYER_DATA;

typedef struct tagDT_BOSS_BATTLE_DATA{
	UINT64           qwOverTime; //活动结束时间
	UINT64           qwStartTime; //活动开始时间
	UINT64           qwSaveTime; //活动保存更新时间
	UINT8            byActivityID; //活动ID
	UINT16           wBossID; //BOSSID
	UINT16           wLastLevel; //上次的等级
	UINT8            byNeedUpgrade; //是否需升级
	UINT32           dwTotalHP; //BOSS总血量
	UINT32           dwCurHP; //BOSS当时血量
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	DT_BOSS_BATTLE_ENC_DATA astBossBTopEncInfo[MAX_BOSS_ENC_TOP_NUM]; //top奖励信息
	DT_BOSS_BATTLE_ENC_DATA stBossBKillEncInfo; //最后1击奖励信息
	UINT64           qwTotalHP; //BOSS总血量
	UINT64           qwCurHP; //BOSS当时血量

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSS_BATTLE_DATA;

typedef struct tagDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA{
	UINT64           qwOverTime; //活动结束时间
	UINT8            byActivityID; //活动ID
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自身攻击伤害
	DT_BOSS_BATTLE_ENC_DATA stHurtEncInfo; //伤害奖励信息
	DT_BOSS_BATTLE_ENC_DATA stHeroEncInfo; //英雄奖励信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA;

typedef struct tagDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST{
	UINT8            byNum; //战报个数
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA astBossBattleBL[MAX_BOSSB_BATTLE_LOG_NUM]; //战报信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST;

typedef struct tagDT_BOSS_BATTLE_MODULE_DATA{
	UINT8            byNum; //数量
	DT_BOSS_BATTLE_DATA astBossBattleInfo[MAX_BOSSB_NUM_PER_DAY]; //信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSS_BATTLE_MODULE_DATA;

typedef struct tagDT_ACTIVITY_ENCOURAGE_INFO{
	UINT32           dwActivityID; //奖励ID
	UINT64           qwActivityTime; //领取奖励时间
	UINT8            byActivityTimes; //领取奖励次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ACTIVITY_ENCOURAGE_INFO;

typedef struct tagDT_ACTIVITY_ENCOURAGE_INFO2{
	UINT64           qwActivityID; //奖励ID
	UINT64           qwActivityTime; //领取奖励时间
	UINT8            byActivityTimes; //领取奖励次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ACTIVITY_ENCOURAGE_INFO2;

typedef struct tagDT_ACTIVITY_ENCOURAGE_DATA{
	UINT8            byRechargeFlag; //充值奖励标志
	UINT64           qwRechargeRecvTime; //领取充值奖励时间
	UINT8            byScoreFlag; //评分奖励标志
	UINT64           qwScoreRecvTime; //领取评分奖励时间
	UINT16           wInfoNumOld; //奖励个数
	DT_ACTIVITY_ENCOURAGE_INFO astActivityInfoOld[MAX_ACTIVITY_ENCOURAGE_INFO_NUM]; //奖励信息
	UINT16           wInfoNum; //奖励个数
	DT_ACTIVITY_ENCOURAGE_INFO2 astActivityInfo[MAX_ACTIVITY_ENCOURAGE_INFO_NUM]; //奖励信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ACTIVITY_ENCOURAGE_DATA;

typedef struct tagDT_RDC_PLAYER_DATA{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT16           wMainHeroKindID; //主伙伴ID
	UINT8            byRobotFunc; //机器人功能,>0则为机器人，不同的值为不同的功能。0为玩家
	UINT16           wLevel; //等级
	UINT32           dwPower; //战力
	UINT8            byHaveChallengeFlag; //是否已挑战，1为已挑战

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RDC_PLAYER_DATA;

typedef struct tagDT_RANDOM_CHALLENGE_DATA{
	UINT16           wCurChallengePoint; //当前挑战点
	UINT64           qwLastRCPoinRefreshTime; //上次挑战点刷新时间
	UINT64           qwLastBuyRCPoinTime; //上次购买挑战点时间
	UINT16           wLastBuyRCPoinNum; //上次购买挑战点次数
	UINT64           qwLastRCTime; //上次挑战时间
	UINT8            byLastRCRet; //上次挑战结果，0赢1输
	UINT8            byClrCDFlag; //是否已清除CD，1已清除CD
	UINT8            byPlayerNum; //当前返回玩家个数
	DT_RDC_PLAYER_DATA astPlayerInfo[MAX_GET_RDC_NUM]; //玩家信息
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RANDOM_CHALLENGE_DATA;

typedef struct tagDT_WORSHIP_DATA{
	UINT8            byWorshipType; //膜拜类型
	UINT8            byWorshipTypeLevel; //膜拜级别，1为最高等级
	UINT64           qwLastWorshipTime; //膜拜时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_WORSHIP_DATA;

typedef struct tagDT_WORSHIP_DATA_LST{
	UINT16           wNum; //个数
	DT_WORSHIP_DATA  astWorshipInfo[MAX_WORSHIP_NUM]; //信息
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_WORSHIP_DATA_LST;

typedef struct tagDT_CHAPTER_SCORE_DATA{
	UINT16           wSceneidx; //场景ID
	UINT16           wChapterIdx; //章节ID
	UINT64           qwtime; //领取奖励时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CHAPTER_SCORE_DATA;

typedef struct tagDT_ACCUMULATED_SCORE_DATA{
	UINT16           wgrade; //档次ID
	UINT64           qwtime; //领取奖励时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ACCUMULATED_SCORE_DATA;

typedef struct tagDT_SCORE_DATA_LST{
	UINT8            byChapterNum; //章数
	DT_CHAPTER_SCORE_DATA astChapterData[MAX_TOWN_PER_SCENE_NUM]; //领取奖励时间
	UINT8            byAccumulatedNum; //积累个数
	DT_ACCUMULATED_SCORE_DATA astAccumulatedData[MAX_TOWN_PER_SCENE_NUM]; //领取奖励时间
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SCORE_DATA_LST;

typedef struct tagDT_EQUIP_LEVEL_COMPOSE_DATA{
	UINT16           wEquipLevel; //装备等级
	UINT8            byCurLowColor; //当前最低颜色
	UINT16           wHitKindID; //当前命中装备ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EQUIP_LEVEL_COMPOSE_DATA;

typedef struct tagDT_EQUIP_COMPOSE_DATA{
	UINT8            byEquipLevelNum; //等级装备数
	DT_EQUIP_LEVEL_COMPOSE_DATA astEquipLevelInfo[MAX_EQUIPLEVEL_NUM]; //装备等级合成信息
	UINT64           qwLastComposeTime; //上次合成时间
	UINT16           wLastComposeEquipLevel; //上次合成装备等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EQUIP_COMPOSE_DATA;

typedef struct tagDT_GATHER_SCIENCE_DATA{
	UINT16           wCoinNum; //铜钱当前使用聚魂次数
	UINT64           qwCoinLastTime; //使用铜钱时间
	UINT16           wGoldNum; //元宝当前使用聚魂次数
	UINT64           qwGoldLastTime; //使用元宝时间
	UINT8            byIncrease; //倍率
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GATHER_SCIENCE_DATA;

typedef struct tagDT_BEGBLUE_RES{
	UINT16           wSmileNum; //笑脸个数
	UINT32           dwBlueUnit; //单倍仙露
	UINT32           dwStoryUnit; //单倍阅历
	UINT16           wXnMultiple; //倍率
	TCHAR            aszLuckPrompt[BEGBLUE_LUCK_PROMPT]; //运势提示
	UINT64           qwCreateTime; //生成的时间
	UINT64           qwUpdateTime; //改运更新的时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BEGBLUE_RES;

typedef struct tagDT_CONSUME_BEGBLUE_SMILE_LST{
	UINT16           wConsumeTimes; //消耗的次数
	UINT16           awConsumeSmileNum[BEGBLUE_CONSUME_TIMES]; //消耗的笑脸个数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CONSUME_BEGBLUE_SMILE_LST;

typedef struct tagDT_BEGBLUE_DATA{
	UINT64           qwBegBlueLastTime; //最后求仙露时间
	TCHAR            aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT]; //爬塔提示
	UINT32           dwMaxBegTimes; //祈求次数上限
	UINT32           dwUsedBegTimes; //已用祈求次数
	UINT32           dwFreeImproveLuckMaxTimes; //当日免费改运次数上限
	UINT32           dwUsedFreeImproveLuckTimes; //已用免费改运次数
	UINT32           dwGoldImproveLuckIdx; //下次元宝改运序号(下次求仙露该值将清零)
	UINT32           dwNextImproveLuckGold; //下次改运需花费的元宝
	UINT8            byIsHaveToComfirmBegBlueInfo; //是否有待确认的求仙露结果(0木有，1有)
	DT_BEGBLUE_RES   stBegBlueRes; //祈求得到的资源
	UINT8            byIsIgnoreXnMultiple; //是否忽略求仙露倍率(0不忽略，1忽略)
	UINT8            byHaveDataFlag; //是否保存数据库
	UINT16           wSmileNum; //笑脸个数
	DT_CONSUME_BEGBLUE_SMILE_LST stDtConsumeSmile; //已消耗笑脸

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BEGBLUE_DATA;

typedef struct tagDT_SEND_FLOWER_DATA{
	UINT32           dwSendFlowerNum; //送花的数量
	UINT8            byUsedSendTimes; //当日已使用的送鲜花次数(针对不同的送花数量，送花次数上限各不同)
	UINT8            byExt; //扩展字段

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SEND_FLOWER_DATA;

typedef struct tagDT_FINALLY_CONTACT_DATA{
	UINT32           dwPlayerID; //玩家ID
	UINT64           qwLastContactTime; //最后联系时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FINALLY_CONTACT_DATA;

typedef struct tagDT_FINALLY_CONTACT_DATA_LST{
	UINT8            byFinallyContactNum; //记录个数
	DT_FINALLY_CONTACT_DATA astFinallyContactData[MAX_FINALLY_CONTACT_DATA_NUM]; //玩家关系列表信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FINALLY_CONTACT_DATA_LST;

typedef struct tagDT_RECV_FLOWER_DATA_EX_LOG{
	UINT32           dwSendPlayerID; //送花的玩家ID
	UINT32           dwKindID; //鲜花ID
	UINT64           qwSendTime; //送花的时间
	UINT8            byFlag; //是否已读 0：收取， 1：未收取， 2：未读
	DT_RES_NUM_ITEM_DATA stResNumItemData; //获取资源列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RECV_FLOWER_DATA_EX_LOG;

typedef struct tagDT_FRIENDS_BASE_DATA{
	UINT64           qwLastSendFlowerTime; //最后送花时间
	UINT8            byElemNum; //元素个数
	DT_SEND_FLOWER_DATA astListData[MAX_SEND_FLOWER_DATA_NUM]; //列表信息
	UINT8            byHaveDataFlag; //是否保存数据库
	DT_FINALLY_CONTACT_DATA_LST stFinallyContactDataList; //最后联系列表
	UINT64           qwLastRecvFlowerTime; //最后收花时间
	UINT32           dwRecvFlowerTimes; //收花的次数
	UINT32           dwSendFlowerTimes; //送花的次数
	UINT32           dwLogNum; //记录个数
	DT_RECV_FLOWER_DATA_EX_LOG astLogData[MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM]; //列表信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FRIENDS_BASE_DATA;

typedef struct tagDT_RAREITEM_GOODS_INFO{
	UINT16           wRareItemID; //物品ID
	UINT32           dwRareItemHaveExchangeNum; //物品已兑换次数
	UINT64           qwLastResetTime; //物品上次兑换时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RAREITEM_GOODS_INFO;

typedef struct tagDT_RAREITEM_GOODS_DATA{
	UINT8            byRareItemNum; //物品数量
	DT_RAREITEM_GOODS_INFO astRareItemGoodInfo[MAX_RAREITEM_RES_NUM]; //物品信息
	UINT64           qwLastUpdataTime; //上次更新时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RAREITEM_GOODS_DATA;

typedef struct tagDT_GYMNASIUM_SKILL_INFO{
	UINT8            bySkillID; //技能ID
	UINT8            byCurSkillLevel; //当前等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GYMNASIUM_SKILL_INFO;

typedef struct tagDT_GYMNASIUM_DATA{
	UINT8            bySkillNum; //技能数量
	DT_GYMNASIUM_SKILL_INFO astGymnasiumSkillInfo[MAX_GYMNASIUM_SKILL_NUM]; //技能信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GYMNASIUM_DATA;

typedef struct tagDT_HUNTING_INFO{
	UINT32           dwHuntionNum; //已捉妖次数
	UINT8            byHuntingChangeNum; //捉妖更换次数
	UINT8            byAbandomMonster; //有放弃的妖怪状态 0没有 1 有放弃的
	UINT8            byCurMonsterType; //当前妖怪的品质
	UINT8            byGoldModeFlag; //黄金模式开户状态
	UINT64           qwGoldModeTime; //黄金模式开启时间
	UINT64           qwLastSyncIntegral; //上次同步积分时间
	UINT8            byIsIgnorePrompt; //是否忽略提示信息 0 不忽略 1 忽略
	UINT8            byMonsterNum; //怪物组中怪物个数
	UINT16           awMonsterGroup[MONSTER_GROUP_NUM]; //当前怪物组

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HUNTING_INFO;

typedef struct tagDT_SHADE_DATA{
	DT_HUNTING_INFO  stHuntingInfo; //捉妖信息
	UINT64           qwLastUpdataTime; //上次更新时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SHADE_DATA;

typedef struct tagDT_FACTION_HUNTING_SCHEDULE_DATA{
	UINT8            byTaskID; //任务ID
	UINT8            byOpenHuntingFlag; //捉妖开启状态
	UINT32           dwMaxHuntingNum; //捉妖次数上限
	UINT8            byCollectiveTaskSchedule; //任务总档数
	UINT8            byCurCollectiveTaskSchedule; //当前任务档数
	UINT32           dwCurIntegral; //当前积分
	UINT32           dwMaxIntegral; //当前积分上限
	UINT64           qwLastUpdateTime; //上去更新时间
	UINT64           qwLastChangeScheduleTime; //上次换档时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_HUNTING_SCHEDULE_DATA;

typedef struct tagDT_HOT_SPRING_TIME_DATA{
	UINT8            byGiveWineOpenState; //开户状态
	UINT64           qwGiveWineOpenTime; //开启时间
	UINT8            byTodayOpenNum; //今日开启次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HOT_SPRING_TIME_DATA;

typedef struct tagDT_SHADE_TASK_OPEN_MARK{
	UINT8            byHuntingUnlockMark; //捉妖任务解锁标识

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SHADE_TASK_OPEN_MARK;

typedef struct tagDT_FACTION_ICON_DATA{
	UINT8            byIcon; //头像ID
	UINT8            byChangeNum; //更换次数
	UINT64           qwlastChangeTime; //更换时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_ICON_DATA;

typedef struct tagDT_FACTION_COMMON_DATA{
	DT_FACTION_HUNTING_SCHEDULE_DATA stHuntingTaskSchedule; //捉妖任务进度
	DT_HOT_SPRING_TIME_DATA stHotSpringData; //温泉状态数据
	DT_FACTION_ICON_DATA stIconData; //头像信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_COMMON_DATA;

typedef struct tagDT_WINE_DATA_CLI{
	UINT32           dwWineID; //酒的ID
	UINT8            byCostType; //消耗类型
	UINT32           dwCostValue; //消耗值
	UINT32           dwDrinkWineCD; //酒CD值
	UINT8            byLeftDrinkNum; //酒的剩余次数
	UINT8            byRewardID1; //奖励ID1
	UINT32           dwRewardValue1; //奖励值1
	UINT8            byRewardID2; //奖励ID2
	UINT32           dwRewardValue2; //奖励值2
	UINT8            byNeedVIPLevel; //VIP等级条件
	UINT32           dwEachRewardValue1; //每次奖励1
	UINT32           dwEachRewardValue2; //每次奖励2
	UINT32           dwCountDown; //饮酒倒时
	UINT32           dwDrinkMaxTime; //总饮酒次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_WINE_DATA_CLI;

typedef struct tagDT_PLAYER_POS_INFO{
	UINT16           wPosX; //位置X
	UINT16           wPosY; //位置Y
	UINT8            byMoveState; //是否有移动 0没有移动 1 有移动
	UINT64           qwSyncTimeLast; //上次同步时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_POS_INFO;

typedef struct tagDT_OVER_GIVE_WINE_REWARD{
	UINT8            byRewardID1; //奖励ID1
	UINT32           dwRewardValue1; //奖励值1
	UINT8            byRewardID2; //奖励ID2
	UINT32           dwRewardValue2; //奖励值2

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_OVER_GIVE_WINE_REWARD;

typedef struct tagDT_HOT_SPRING_DATA{
	UINT8            byLastDrinkWineState; //是否有酒未饮 0没有未饮的酒, 1有未饮的酒
	UINT8            byDrinkWineState; //饮酒状态 0未在饮酒， 1 正在饮酒
	DT_WINE_DATA_CLI stWineInfo; //酒的信息
	DT_PLAYER_POS_INFO stPlayerPosInfo; //玩家位置信息
	UINT64           qwFirstDrinkTime; //第一次饮酒时间
	UINT32           dwDrinkKeepTime; //已喝时间(秒)
	UINT8            byIsJoinCurActivity; //是否参加本次活动
	UINT8            byTodayBackTime; //今天找回次数
	UINT64           qwUpgradeTime; //更新时间
	UINT8            byActivityStat; //活动类别
	UINT8            byHaveOverReward; //是否有过期奖励
	DT_OVER_GIVE_WINE_REWARD stOverReward; //过期奖励
	UINT8            byIsIgnorePrompt; //是否忽略提示 0不忽略 1忽略

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HOT_SPRING_DATA;

typedef struct tagDT_LEAVE_FACTION_MARK{
	UINT8            byLeaveMark; //退出标识
	UINT64           qwLeaveTime; //退出时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LEAVE_FACTION_MARK;

typedef struct tagDT_PLAYER_DONATE_INFO{
	UINT64           qwLastCoinDoorsTributeTime; //铜钱门贡时间
	UINT16           wTodaysCoinDoorsTributeTimes; //今天铜钱门贡次数
	UINT8            byTodaysCoinDoorsTributeEncourage; //今天铜钱门贡领取奖励标志
	UINT64           qwLastGoldDoorsTributeTime; //元宝门贡时间
	UINT16           wTodaysGoldDoorsTributeTimes; //今天元宝门贡次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_DONATE_INFO;

typedef struct tagDT_PLAYER_MARK_DATA{
	UINT8            byFactionPlayerMark; //玩家门派人员变动标识
	DT_LEAVE_FACTION_MARK stLeaveFactionMark; //退出门派标识

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_MARK_DATA;

typedef struct tagDT_PLAYER_FACTION_BATTLELOG_DATA{
	UINT8            byFactionLogType; //日志类型 em_Faction_BattleLog_Type
	UINT64           qwLogTime; //发生时间
	UINT8            byNewFlag; //是否是未阅读0 已阅读 1 未阅读
	TCHAR            aszDescMsg[NORMAL_MSG_LEN]; //描述数据,补踢出门派时是被谁踢，加入门派时是门派名称
	UINT8            byParamNum; //参数个数
	UINT8            abyparamList[FACTION_LOG_LIST_MAX_NUM]; //参数数组

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_FACTION_BATTLELOG_DATA;

typedef struct tagDT_PLAYER_FACTION_BATTLELOG_DATA_LIST{
	UINT8            byLogNum; //战报个数
	DT_PLAYER_FACTION_BATTLELOG_DATA astPlayerFactionLog[MAX_FACTION_BATTLE_LOG_NUM]; //日志信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_FACTION_BATTLELOG_DATA_LIST;

typedef struct tagDT_HOT_SPRING_EXT_DATA{
	UINT64           qwLastEnterTime; //上次进入温泉时间
	UINT64           qwExprienceTime; //累积时间
	UINT8            byDrinkTime; //今日泡温泉次数
	UINT8            byFindBackTime; //今日找回温泉次数
	UINT8            byEnterHotSpringType; //进入温泉方式（0从后台登入， 1重新进入）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HOT_SPRING_EXT_DATA;

typedef struct tagDT_PLAYER_FACTION_DATA{
	UINT8            byHaveDataFlag; //是否保存数据库
	DT_RAREITEM_GOODS_DATA stRareItemGoodsData; //珍品阁信息
	DT_GYMNASIUM_DATA stGymnasiumSkillData; //武堂信息
	DT_SHADE_DATA    stShadeData; //暗部信息
	DT_HOT_SPRING_DATA stHotSpringData; //温泉信息
	DT_PLAYER_FACTION_BATTLELOG_DATA_LIST stPlayerFactionLog; //个人门派日志信息记录
	DT_PLAYER_MARK_DATA stPlayerMark; //玩家标志信息
	DT_PLAYER_DONATE_INFO stPlayerDoNate; //门派捐献信息
	UINT32           dwFindBackDoorsTribute; //可找回门贡
	DT_HOT_SPRING_EXT_DATA stHotSpringExt; //温泉扩展信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_FACTION_DATA;

typedef struct tagDT_PLAYER_TODAY_CONSUME_DATA{
	UINT32           dwID; //玩家ID
	UINT64           qwUTCLastConsumeTime; //消费时间时间
	UINT64           qwAddExperience; //经验
	UINT64           qwAfterExperience; //经验
	UINT16           wAddLevel; //等级
	UINT16           wAfterLevel; //等级
	UINT8            byAddVipLevel; //VIP等级
	UINT8            byAfterVipLevel; //VIP等级
	UINT64           qwAddCoin; //铜钱
	UINT64           qwDecCoin; //铜钱
	UINT64           qwAfterCoin; //铜钱
	UINT64           qwAddGold; //元宝
	UINT64           qwDecGold; //元宝
	UINT64           qwAfterGold; //元宝
	UINT64           qwAddStory; //阅历
	UINT64           qwDecStory; //阅历
	UINT64           qwAfterStory; //阅历
	UINT16           wAddPhyStrength; //当前体力
	UINT16           wDecPhyStrength; //当前体力
	UINT16           wAfterPhyStrength; //当前体力
	UINT64           qwAddScience; //当前科技点
	UINT64           qwDecScience; //当前科技点
	UINT64           qwAfterScience; //当前科技点
	UINT64           qwAddBlueGas; //蓝气
	UINT64           qwDecBlueGas; //蓝气
	UINT64           qwAfterBlueGas; //蓝气
	UINT64           qwAddPurpleGas; //紫气
	UINT64           qwDecPurpleGas; //紫气
	UINT64           qwAfterPurpleGas; //紫气
	UINT64           qwAddJingJie; //境界点
	UINT64           qwDecJingJie; //境界点
	UINT64           qwAfterJingJie; //境界点
	UINT64           qwAddStudyExp; //卦象经验
	UINT64           qwDecStudyExp; //卦象经验
	UINT64           qwAfterStudyExp; //卦象经验
	UINT64           qwAddMedal; //勇气勋章
	UINT64           qwDecMedal; //勇气勋章
	UINT64           qwAfterMedal; //勇气勋章
	UINT32           dwAddAmount; //充值元宝
	UINT32           dwAfterAmount; //充值元宝
	UINT64           qwAddTotalRMB; //充值金额
	UINT64           qwAfterTotalRMB; //充值金额
	TCHAR            aszLastConsumeTime[USERNAME_LEN]; //消费时间时间
	UINT64           qwAddDoorsTribute; //门派门贡
	UINT64           qwDecDoorsTribute; //门派门贡
	UINT64           qwAfterDoorsTribute; //门派门贡
	UINT64           qwAddRedStudyNum; //生成卦象
	UINT64           qwAddTaijiStudyNum; //生成卦象
	UINT64           qwBuyPhyStrengthNum; //购买体力

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_TODAY_CONSUME_DATA;

typedef struct tagDT_RECV_FLOWER_DATA_LOG{
	UINT32           dwSendPlayerID; //送花的玩家ID
	UINT32           dwFlowerNum; //鲜花数
	UINT64           qwSendTime; //送花的时间
	UINT8            byIsRead; //是否已读 0：收取， 1：未读， 2：已读

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RECV_FLOWER_DATA_LOG;

typedef struct tagDT_RECV_FLOWER_LOG_LIST{
	UINT32           dwLogNum; //记录个数
	DT_RECV_FLOWER_DATA_LOG astListData[MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM]; //列表信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RECV_FLOWER_LOG_LIST;

typedef struct tagDT_RELATION_DATA{
	UINT32           dwPlayerIDB; //玩家ID B
	UINT8            byrelation; //关系类型
	UINT64           qwAddTime; //增加时间
	UINT8            byisRead; //消息是否已读

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RELATION_DATA;

typedef struct tagDT_RELATION_DATA_LIST{
	UINT32           dwRelationNum; //记录个数
	DT_RELATION_DATA astListData[MAX_RELATION_LIST_ELEM_NUM]; //列表信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RELATION_DATA_LIST;

typedef struct tagDT_RELATION_DATA_EVEN{
	UINT32           dwPlayerIDA; //玩家ID
	UINT8            byisRead; //消息是否已读

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RELATION_DATA_EVEN;

typedef struct tagDT_UNREAD_DATA_EVEN{
	UINT32           dwSendPid; //玩家ID
	UINT32           dwUnReadNum; //消息个数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_UNREAD_DATA_EVEN;

typedef struct tagDT_DATA_EVEN_LIST{
	UINT32           dwRelationNum; //记录个数
	DT_RELATION_DATA_EVEN astRelationEvenData[MAX_RELATION_LIST_ELEM_NUM]; //玩家关系列表信息
	UINT32           dwUnReadNum; //未读记录个数
	DT_UNREAD_DATA_EVEN astUnReadEvenData[MAX_RELATION_LIST_ELEM_NUM]; //未读列表信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DATA_EVEN_LIST;

typedef struct tagDT_COURAGE_PLAYER_HEROID_DATA{
	UINT32           dwPlayID; //玩家ID
	UINT16           wHeroID; //伙伴ID
	UINT16           wKindID; //玩家职业ID
	UINT8            byBattleType; //战斗类型 0：加入战斗， 1：协助战斗
	UINT32           dwExperience; //玩家经验
	UINT16           wLevel; //玩家等级
	TCHAR            aszDispPlayerName[USERNAME_LEN]; //玩家昵称
	UINT8            byCurAttackObjIdx; //阵型ID
	UINT32           dwUpgradeExperience; //玩家升级经验
	UINT8            byColor; //玩家颜色

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_PLAYER_HEROID_DATA;

typedef struct tagDT_COURAGE_COMMON_PLAYER_BATTLE_DATA{
	UINT8            byPlayerNum; //玩家数量
	DT_COURAGE_PLAYER_HEROID_DATA astPlayerInfo[MAX_COURAGE_MEMBER_NUM]; //玩家信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_COMMON_PLAYER_BATTLE_DATA;

typedef struct tagDT_COURAGE_ITEM_DATA_CLI{
	UINT16           wKindID; //类型
	UINT8            byGoodsMainKindID; //物品主类型，只对物品类型道具有效
	UINT16           wPileNum; //叠加数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_ITEM_DATA_CLI;

typedef struct tagDT_COURAGE_ITEM_DATA_LIST{
	UINT8            byItemNum; //道具个数
	DT_COURAGE_ITEM_DATA_CLI astItemList[MAX_BAG_OPEN_NUM]; //道具信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_ITEM_DATA_LIST;

typedef struct tagDT_COURAGE_ENCOURAGE_BOX_GROUP_DATA{
	UINT32           dwCoin; //铜钱(非0则转盘命中铜钱)
	UINT32           dwGold; //元宝(非0则转盘命中元宝)
	UINT8            byHitItemIdx; //命中道具序号(铜钱/元宝非0时有效，0表示未命中,从1开始,<=ItemNum)
	DT_COURAGE_ITEM_DATA_LIST stItemInfo; //道具信息
	UINT32           dwBlueGas; //蓝气(非0则转盘命中蓝气)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_ENCOURAGE_BOX_GROUP_DATA;

typedef struct tagDT_COURAGE_ENCOURAGE_BOX_DATA2{
	UINT8            byGroupNum; //宝箱组个数
	DT_COURAGE_ENCOURAGE_BOX_GROUP_DATA astBoxGroupInfo[MAX_GVE_ENCOURAGE_BOX_GROUP_NUM]; //宝箱组信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_ENCOURAGE_BOX_DATA2;

typedef struct tagDT_COURAGE_DATA{
	UINT64           qwLastTime; //最后访问的时间
	UINT32           dwBuyTimes; //购买次数
	UINT32           dwBuyAddTimes; //购买增加的次数
	UINT32           dwTodayJoinTimes; //今天剩余的次数
	UINT32           dwGroupID; //玩家的组ID
	UINT16           wGroupIndex; //玩家在组中索引 0,1,2
	UINT32           dwTodayHelpTimes; //今天协助的次数
	UINT32           dwGiveFreeTimes; //今天是否赠送了免费试炼次数 0:没有，1有
	UINT32           dwTotalTimes; //今天总共试炼次数
	UINT8            byNotity; //是否打开通知
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT16           wAverageLevel; //队伍平均等级用来领奖
	UINT8            byHavePass; //是否通关 0未通关 1通关
	UINT8            byHaveAward; //是否领取奖励 0领取了 1未领取
	DT_COURAGE_COMMON_PLAYER_BATTLE_DATA stGroupPlayerInfo; //队伍信息
	DT_INSTANCE_ID   stSceneData; //场景信息
	DT_BAG_JEWELRY_DATA_LST stjewelryInfoLst; //饰品信息
	UINT8            bySelectGroupType; //筛选条件（CourageGroupType）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_DATA;

typedef struct tagDT_EQUIP_MASTER_DETAIL{
	UINT8            byMasterKind; //装备类别（头盔、战袍、武器、战靴）
	UINT8            byMasterRank; //精通阶级（初1、中2、高3、极4）
	UINT16           wMasterLevel; //精通等级
	UINT32           dwExp; //当前精通经验

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EQUIP_MASTER_DETAIL;

typedef struct tagDT_MASTER_EXP_BOOK_DETAIL{
	UINT8            byBigExpBookCnt; //大经验书有几本（0本、1本、2本、3本）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MASTER_EXP_BOOK_DETAIL;

typedef struct tagDT_EQUIP_MASTER{
	DT_EQUIP_MASTER_DETAIL astMasterInfo[EQUIP_KIND_COUNT]; //4种类别的装备精通数据
	UINT8            byCultivatePanelState; //培养精通之书面板状态：未开启面板0、打开但未刮开1、刮开但未确认2，见EMasterBookPanelState
	DT_MASTER_EXP_BOOK_DETAIL astExpBookPanel[EQUIP_KIND_COUNT]; //培养精通之书面板内容
	UINT8            byCheatTimes; //出千次数
	UINT8            byHaveDataFlag; //是否曾经开启过本功能(0从未开启过/1曾经开启过)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EQUIP_MASTER;

typedef struct tagDT_FRIENDS_DATA{
	DT_FRIENDS_BASE_DATA stFriendsBaseData; //好友基本数据
	DT_RECV_FLOWER_LOG_LIST stRecvFlowerLogList; //收花日志
	DT_RELATION_DATA_LIST stRelationDataList; //玩家关系列表
	DT_DATA_EVEN_LIST stDataEvenList; //事件列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FRIENDS_DATA;

typedef struct tagDT_INSTANCE_DB_DATA{
	DT_INSTANCE_DATA_LIST stCommonInstanceData; //玩家普通副本信息
	DT_INSTANCE_DATA_LIST stEliteInstanceData; //玩家精英副本信息
	DT_INSTANCE_DATA_LIST stClimbTowerInstanceData; //玩家爬塔副本信息
	DT_INSTANCE_BATTLE_DATA stCommonBattleRcd; //普通副本战斗记录
	DT_INSTANCE_BATTLE_DATA stEliteBattleRcd; //精英副本战斗记录
	DT_INSTANCE_BATTLE_DATA stClimbTowerBattleRcd; //爬塔副本战斗记录
	DT_INSTANCE_DATA_LIST stOutBoundInstanceData; //外传副本信息
	DT_INSTANCE_DATA_LIST stNeiDanInstanceData; //内丹副本信息
	DT_INSTANCE_BATTLE_DATA stNeiDanBattleRcd; //内丹战斗记录

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_INSTANCE_DB_DATA;

typedef struct tagDT_RACE_DATA{
	DT_RACE_BASE_DATA stRaceBaseData; //玩家竞技场信息
	DT_RACE_BATTLELOG_DATA_LIST stRaceBattleLog; //玩家竞技场战报信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_RACE_DATA;

typedef struct tagDT_PLUNDER_DATA{
	DT_PLUNDER_BASE_DATA stPlunderBaseData; //掠夺基本信息
	DT_PLUNDER_BATTLELOG_DATA_LIST stPlunderBattleLog; //掠夺战报信息
	DT_CAPTURE_DATA_LIST stCaptureData; //掠夺俘虏信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLUNDER_DATA;

typedef struct tagDT_YABIAO_DATA{
	DT_YABIAO_BASE_DATA stYabiaoBaseInfo; //押镖信息
	DT_YABIAO_BATTLELOG_DATA_LIST stYabiaoBattleLog; //玩家跑商战报记录

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_YABIAO_DATA;

typedef struct tagDT_BOSS_BATTLE_PLAYER_DATA_INFO{
	DT_BOSS_BATTLE_PLAYER_DATA stBossBattleData; //Boss战信息
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST stBossBBLData; //boss战战报

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BOSS_BATTLE_PLAYER_DATA_INFO;

typedef struct tagDT_EXCHANGE_RESULT_LOG{
	UINT8            byNtfFlag; //推送标志
	UINT32           dwExchangeGold; //充值元宝数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EXCHANGE_RESULT_LOG;

typedef struct tagDT_GAMBLE_DATA{
	UINT64           qwGameID; //押注场次ID
	UINT32           dwSupportFactionID; //押注的门派ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GAMBLE_DATA;

typedef struct tagDT_GVG_GIFT_REWARD_DATA{
	UINT16           wSessionID; //第几届跨服门派战
	UINT8            byGvGRound; //跨战轮次（commondef.h GvGRound）
	UINT8            byBattleIdx; //当前轮次中的第几场战斗
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //对战门派名称
	UINT8            byBattleResult; //对战结果（0输，1胜利）
	UINT8            byGiftID; //礼包ID
	UINT32           dwPlayerID; //玩家ID
	UINT8            byGiftType; //礼包类型（GvGiftType）
	UINT8            byRank; //排名

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_GIFT_REWARD_DATA;

typedef struct tagDT_GVG_GIFT_REWARD_LST_DATA{
	UINT8            byGiftNum; //礼包数
	DT_GVG_GIFT_REWARD_DATA astGiftInfo[MAX_GVG_GIFT_NUM]; //礼包信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_GIFT_REWARD_LST_DATA;

typedef struct tagDT_GVG_GIFT_KEY_LST_DATA{
	UINT8            byGiftNum; //礼包数
	UINT64           aqwGiftKey[MAX_GVG_GIFT_NUM]; //礼包KEY

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_GIFT_KEY_LST_DATA;

typedef struct tagDT_PLAYER_GVG_DATA{
	UINT8            byHaveDataFlag; //是否曾经开启过本功能(0从未开启过/1曾经开启过)
	DT_GAMBLE_DATA   astGambleLstInfo[MAX_GAMBLE_NUM]; //押注信息队列
	DT_GVG_GIFT_KEY_LST_DATA stGetGiftData; //已领取礼包Key(由4个关键字组成)
	UINT16           wSessionID; //本届门派跨服战届数
	UINT16           wReadLogCnt; //本届已读本派战况数
	UINT8            byIsGetGamble; //领取押注奖励标识（0未领取， 1已领取）

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_GVG_DATA;

typedef struct tagDT_GVG_FORMATION_DATA{
	UINT8            byPlayerNum; //上阵人数
	UINT32           adwPlayerIDList[MAX_GVG_FORMATION_PLAYER_CNT]; //挑战标志

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_FORMATION_DATA;

typedef struct tagDT_FACTION_BASE_DATA_CLI2{
	UINT32           dwFactionID; //门派ID
	UINT8            byIconID; //门派头像ID
	TCHAR            aszHeadName[USERNAME_LEN]; //掌门名字
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT16           wCurPlayerNum; //门派当前人数
	UINT16           wMaxPlayerNum; //门派上限人数
	UINT32           dwFactionRank; //门派排名(等级与累积资金排名)
	UINT32           dwFactionLevel; //门派等级
	UINT32           dwFactionFunds; //门派剩余资金
	UINT64           qwFactionFundsSum; //门派累积资金
	TCHAR            aszFactionAnnouncement[MAX_FACTION_DESC_NUM]; //门派公告
	TCHAR            aszZoneName[ZONENAME_LEN]; //区名

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_BASE_DATA_CLI2;

typedef struct tagDT_GAMBLE_FACTION_INFO{
	DT_FACTION_BASE_DATA_CLI2 stFactionInfo; //门派信息
	UINT16           wZoneID; //所有区ID
	UINT8            byIsWin; //是否获胜 0输， 1赢
	UINT8            byIsGamble; //是否押注0未押注， 1押注

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GAMBLE_FACTION_INFO;

typedef struct tagDT_GVG_ROUND{
	UINT8            byBigRound; //本次战斗所处的大轮次，见 GvGRound
	UINT8            bySmallRound; //大轮次中的的小轮次

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_ROUND;

typedef struct tagDT_FACTION_GVG_SINGLE_LOG{
	DT_GVG_ROUND     stRound; //轮次信息
	UINT64           qwVideoID; //录像ID
	UINT8            byResult; //战况结果:，见 GvGLogResult
	UINT16           wScore; //积分：为0时不显示
	DT_GAMBLE_FACTION_INFO stG1; //门派1
	DT_GAMBLE_FACTION_INFO stG2; //门派2

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_GVG_SINGLE_LOG;

typedef struct tagDT_FACTION_SIMPLE_GVG_LOG{
	UINT32           dwFactionID1; //左边门派ID
	UINT32           dwFactionID2; //右边门派ID
	DT_GVG_ROUND     stRound; //轮次信息
	UINT64           qwVideoID; //录像ID
	UINT8            byLogResult; //战况结果：未开始、赢、输，见 GvGLogResult
	UINT8            byGvGResult; //战斗结果：未开始、左边门派赢、右边门派赢，见 GvGResult
	UINT16           wAddScore; //获得积分：为0时不显示

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_SIMPLE_GVG_LOG;

typedef struct tagDT_FACTION_GVG_LOG_LIST{
	UINT16           wLogCnt; //战况记录数
	DT_FACTION_GVG_SINGLE_LOG astLogs[MAX_GVG_LOG_CNT]; //战况记录列表
	UINT16           wSessionID; //当前届数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_GVG_LOG_LIST;

typedef struct tagDT_FACTION_SIMPLE_GVG_LOG_LIST{
	UINT32           dwFactionID; //门派ID
	UINT16           wLogCnt; //战况记录数
	DT_FACTION_SIMPLE_GVG_LOG astLogs[MAX_GVG_SIMPLE_LOG_CNT]; //战况记录列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_SIMPLE_GVG_LOG_LIST;

typedef struct tagDT_FACTION_LIST_SIMPLE_GVG_LOG{
	UINT16           wFactionCnt; //战况记录数
	DT_FACTION_SIMPLE_GVG_LOG_LIST astLogList[MAX_GVG_ARENA_FACTION_CNT]; //战况记录列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_LIST_SIMPLE_GVG_LOG;

typedef struct tagDT_GVG_LOG_FACTION_INFO{
	UINT32           dwFactionID; //门派ID
	UINT16           wFactionIconID; //门派图标
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	TCHAR            aszHeadName[USERNAME_LEN]; //掌门名称
	UINT32           dwScore; //积分
	UINT16           wFactionLevel; //门派等级
	UINT64           qwSumPower; //总战力
	UINT16           wJoinNum; //参战人数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_LOG_FACTION_INFO;

typedef struct tagDT_FACTION_GVG_LOG{
	DT_GVG_LOG_FACTION_INFO stFlag; //门派战况中左边的战旗
	DT_FACTION_GVG_LOG_LIST stLogList; //战况记录

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_GVG_LOG;

typedef struct tagDT_FACTION_GVG_DATA{
	DT_FACTION_GVG_LOG_LIST stGvGLogList; //门派战报
	DT_GVG_FORMATION_DATA stGvGFormation; //门派布阵信息
	UINT64           qwGVGFactionFunds; //GVG累积资金
	UINT64           qwSumPower; //门派总战力

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_GVG_DATA;

typedef struct tagDT_GVG_FORMATION_PLAYER_INFO{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszPlayerName[USERNAME_LEN]; //玩家名称
	UINT8            byPlayerJob; //玩家职位，见commondef.h的 em_Faction_Job
	UINT8            byKillCombo; //连斩数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_FORMATION_PLAYER_INFO;

typedef struct tagDT_GVG_FORMATION_PLAYER_LIST{
	UINT8            byPlayerCnt; //玩家数量
	DT_GVG_FORMATION_PLAYER_INFO astPlayerList[MAX_GVG_FORMATION_PLAYER_CNT]; //各玩家职位

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_FORMATION_PLAYER_LIST;

typedef struct tagDT_DOORSTRIBUTE_LEVEL_DATA{
	UINT8            byCrownNum; //皇冠个数
	UINT8            bySunNum; //太阳个数
	UINT8            byMoonNum; //月亮个数
	UINT8            byStarNum; //星星个数
	UINT32           dwNextUpgradeNeed; //下一级升级所需累积门贡
	UINT16           wDoorsTributeLevel; //门贡等级

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DOORSTRIBUTE_LEVEL_DATA;

typedef struct tagDT_DOORSTRIBUTE_LEVE_DATA_LST{
	UINT16           wDoorsTributeNum; //贡献度个数
	DT_DOORSTRIBUTE_LEVEL_DATA astDoorsTributeInfo[DT_FACTION_PLAYER_NUM]; //贡献度信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DOORSTRIBUTE_LEVE_DATA_LST;

typedef struct tagDT_FACTION_MEMBER_INFO{
	UINT32           dwPlayerID; //成员名称
	UINT16           wHeroKindID; //主武将类型
	TCHAR            aszPlayerName[USERNAME_LEN]; //成员名称
	UINT16           wPlayerLevel; //玩家职位，见commondef.h的 em_Faction_Job
	UINT8            byPlayerJob; //玩家职务
	UINT32           dwPower; //玩家战力
	UINT32           dwRank; //竞技场排名

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_MEMBER_INFO;

typedef struct tagDT_FACTION_MEMBER_INFO_LIST{
	UINT8            byMemberCnt; //玩家数量
	DT_FACTION_MEMBER_INFO astMembers[DT_FACTION_PLAYER_NUM]; //玩家数量

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_MEMBER_INFO_LIST;

typedef struct tagDT_GVG_FACTION{
	UINT16           wZoneID; //区ID
	DT_FACTION_BASE_DATA_CLI2 stBaseData; //门派基础信息
	UINT8            byJoinCnt; //参战人数
	UINT64           qwSumPower; //参战战力
	UINT16           wScore; //当前积分
	UINT32           dwGambleCnt; //押注人数
	UINT8            byWinCnt; //当前胜利场数
	UINT8            byLoseCnt; //当前失败场数
	UINT16           wOriZoneID; //该门派合区前所在的区
	DT_FACTION_MEMBER_INFO_LIST stMembers; //门派成员信息列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_FACTION;

typedef struct tagDT_PK_FACTION{
	DT_GVG_FACTION   stBaseData; //门派基础信息
	DT_GVG_FORMATION_DATA stFormation; //门派布阵
	DT_GVG_FORMATION_PLAYER_LIST stPlayerList; //参战信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PK_FACTION;

typedef struct tagDT_PVP_VIDEO_DATA{
	UINT32           dwVideoID; //录像ID
	UINT64           qwVideoTime; //录像时间
	DT_BATTLE_DATA3  stPvPVideo; //录像数据

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PVP_VIDEO_DATA;

typedef struct tagDT_GVG_MVP{
	UINT8            byFactionIconID; //门派图标ID
	TCHAR            aszPlayerName[USERNAME_LEN]; //MVP玩家
	UINT8            byKillCombo; //连斩数
	UINT8            byScore; //积分贡献
	UINT32           dwAward; //奖励
	UINT8            byWinOrLose; //战斗结果：1胜/0负

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_MVP;

typedef struct tagDT_GVG_MVP_LIST{
	UINT8            byMVPCnt; //MVP数量
	DT_GVG_MVP       astMVPList[MAX_GVG_MVP_CNT]; //MVP列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_MVP_LIST;

typedef struct tagDT_GVG_VIDEO_FACTION{
	UINT16           wZoneID; //本门派所在的区号
	UINT32           dwFactionID; //门派ID
	UINT8            byFactionIconID; //门派图标ID
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	DT_GVG_FORMATION_PLAYER_LIST stPlayerList; //本门派上阵玩家列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_VIDEO_FACTION;

typedef struct tagDT_GVG_PK_VIDEO{
	UINT8            byPlayerIdx1; //玩家1是门派1中第几个上场的
	UINT8            byPlayerIdx2; //玩家2是门派2中第几个上场的
	UINT8            byKillCombo1; //玩家1连斩数
	UINT8            byKillCombo2; //玩家2连斩数
	DT_EMPTY_HP_VIDEO stPKVideo; //玩家PK录像

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_PK_VIDEO;

typedef struct tagDT_GVG_PK_VIDEO_LIST{
	UINT8            byPKVideoCnt; //pk场数
	DT_GVG_PK_VIDEO  astPKVideoList[MAX_GVG_PK_VIDEO_CNT]; //两门派间的玩家pk录像ID列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_PK_VIDEO_LIST;

typedef struct tagDT_GVG_VIDEO{
	UINT64           qwVideoID; //录像ID
	UINT64           qwTime; //录像时间
	DT_GVG_ROUND     stRound; //轮次信息
	DT_GVG_VIDEO_FACTION stG1; //门派1信息
	DT_GVG_VIDEO_FACTION stG2; //门派2信息
	DT_FINAL_ROUND_BATTLE_OBJ_INFO stFinalRoundInfo; //最后一轮的玩家信息
	UINT8            byBattleResult; //战斗结果：未开始、左边门派赢、右边门派赢，见 GvGResult
	UINT8            byPKVideoCnt; //pk场数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_VIDEO;

typedef struct tagDT_GVG_ZONE_MEMBER_DATA{
	UINT8            byZoneCnt; //区数量
	UINT32           adwZoneIDList[MAX_GVG_ARENA_ZONE_CNT]; //区ID列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_ZONE_MEMBER_DATA;

typedef struct tagDT_ZONE_FACTION_ID{
	UINT32           dwZoneID; //该门派属于哪个区
	UINT32           dwFactionID; //门派ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ZONE_FACTION_ID;

typedef struct tagDT_DIFF_ZONE_FACTION_ID_LIST{
	UINT8            byFactionCnt; //有多少个门派
	DT_ZONE_FACTION_ID astFactionList[GVG_16_TO_8_FACTION_CNT]; //门派列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_DIFF_ZONE_FACTION_ID_LIST;

typedef struct tagDT_ONE_ZONE_FACTION_ID_LIST{
	UINT16           wZoneID; //区ID
	UINT8            byFactionCnt; //有多少个门派
	UINT32           adwFactionList[GVG_16_TO_8_FACTION_CNT]; //门派列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ONE_ZONE_FACTION_ID_LIST;

typedef struct tagDT_ZONE_LIST{
	UINT8            byZoneCnt; //区ID
	DT_ONE_ZONE_FACTION_ID_LIST astZoneList[MAX_GVG_ARENA_ZONE_CNT]; //门派列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ZONE_LIST;

typedef struct tagDT_ZONE_ID_LIST{
	UINT8            byZoneCnt; //区ID
	UINT16           awZoneIDList[MAX_GVG_ARENA_ZONE_CNT]; //门派列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ZONE_ID_LIST;

typedef struct tagDT_FACTION_LIST_GVG_LOG{
	UINT16           wFactionCnt; //门派数
	DT_FACTION_GVG_LOG astFactionLogs[GVG_16_TO_8_FACTION_CNT]; //各门派的战况

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_LIST_GVG_LOG;

typedef struct tagDT_VIDEO_ID{
	UINT64           qwVideoID; //录像ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_VIDEO_ID;

typedef struct tagDT_FINAL_WAR_EACH_ROUND_FACTION_INFO{
	UINT8            byEnemyPos; //该门派的对手在门派列表中的位置
	UINT16           wScore; //当前积分
	UINT64           qwVideoID; //录像ID
	UINT8            byResult; //结果，见 GvGLogResult

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FINAL_WAR_EACH_ROUND_FACTION_INFO;

typedef struct tagDT_FINAL_WAR_ROUND_INFO{
	UINT8            byFactionCnt; //本轮门派数
	UINT8            abyRankList[GVG_FINAL_WAR_FACTION_CNT]; //排名列表
	DT_FINAL_WAR_EACH_ROUND_FACTION_INFO astFactionList[GVG_FINAL_WAR_FACTION_CNT]; //本轮各门派的信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FINAL_WAR_ROUND_INFO;

typedef struct tagDT_16_TO_8_TO_4_WAR_INFO{
	UINT8            byFactionCnt; //双服门派选手数
	DT_GVG_FACTION   astFactionList[GVG_16_TO_8_FACTION_CNT]; //双服门派选手列表
	UINT8            byWin8Cnt; //16进8的胜出门派数
	UINT8            abyWin8List[GVG_16_TO_8_WIN_CNT]; //16进8的胜出门派序号列表(序号:1-16)
	UINT8            byWin4Cnt; //8进4的胜出门派数
	UINT8            abyWin4List[GVG_8_TO_4_WIN_CNT]; //8进4的胜出门派序号列表(序号:1-16)
	UINT64           aqwVideoIDList16To8[GVG_16_TO_8_WIN_CNT]; //16进8的录像ID列表
	UINT64           aqwVideoIDList8To4[GVG_8_TO_4_WIN_CNT]; //8进4的录像ID列表
	UINT8            byRolled; //是否已配对
	UINT8            byReady16To8; //16进8赛是否已经就绪
	UINT8            byReady8To4; //8进4赛是否已经就绪

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_16_TO_8_TO_4_WAR_INFO;

typedef struct tagDT_FINAL_WAR_INFO{
	UINT8            byTotalRound; //总轮次
	UINT8            byFactionCnt; //四服门派选手数
	DT_GVG_FACTION   astFactionList[GVG_FINAL_WAR_FACTION_CNT]; //四服门派选手列表
	UINT8            byTopCnt; //名次列表中的门派数，未决出结果时则为0
	UINT8            abyTopList[GVG_FINAL_WAR_FACTION_CNT]; //各门派名次列表，按积分排名，数组各元素内容分别是：第一名门派序号、第二名门派序号、.....第八名门派序号
	DT_FINAL_WAR_ROUND_INFO astEachRoundInfo[GVG_FINAL_WAR_FACTION_CNT]; //各个轮次的录像ID列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FINAL_WAR_INFO;

typedef struct tagDT_GVG_STAGE_FINISH_FLAG_LIST{
	UINT8            abyFlagList[30]; //标记各个轮次是否完成

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_STAGE_FINISH_FLAG_LIST;

typedef struct tagDT_GROUP_ZONE{
	UINT8            byGroupID; //组别ID
	UINT16           awZoneIDList[2]; //门派列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GROUP_ZONE;

typedef struct tagDT_GROUP_ZONE_ID_LIST{
	UINT8            byGroupCnt; //组数
	DT_GROUP_ZONE    astGroupList[MAX_GVG_GROUP_NUM]; //门派列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GROUP_ZONE_ID_LIST;

typedef struct tagDT_WAR_16_TO_8_LIST{
	UINT8            byWarCnt; //双服对战组数
	DT_16_TO_8_TO_4_WAR_INFO astWar16To8To4Info[MAX_GVG_GROUP_NUM]; //多个双服对战组的对战信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_WAR_16_TO_8_LIST;

typedef struct tagDT_GVG_ARENA_DATA{
	UINT16           wArenaID; //该擂台的ID
	UINT8            byCurStage; //当前阶段
	UINT8            byIsDone; //该赛区是否已完成当前小轮次的任务
	DT_ZONE_ID_LIST  stZoneIDList; //该擂台的区ID列表
	DT_GROUP_ZONE_ID_LIST stGroupZoneIDList; //该擂台的区ID列表
	DT_WAR_16_TO_8_LIST stWar16To8List; //多个双服的对战信息列表
	DT_FINAL_WAR_INFO stFinalWarInfo; //四服积分总决赛信息
	DT_FACTION_LIST_SIMPLE_GVG_LOG stFactionsLog; //当前各门派的战况
	DT_GVG_STAGE_FINISH_FLAG_LIST stStageDoneFlagList; //标记各个阶段是否已完成任务

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_ARENA_DATA;

typedef struct tagDT_ZONE_FACTION_MEMBER{
	UINT32           dwZoneID; //本区ID
	UINT8            byFactionCnt; //本区有几个门派参与
	UINT32           adwFactionIDList[MAX_GVG_TOP_FACTION_CNT]; //门派列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_ZONE_FACTION_MEMBER;

typedef struct tagDT_GVG_ACTIVITY_DATA{
	UINT16           wSessionID; //本次跨服战的届数
	UINT64           qwStartTime; //本次跨服战的开始时间
	UINT8            byCurStage; //当前进展到第几轮，见 GVG_STAGE
	UINT8            byDone; //本轮的工作是否已完成，可以进入到下一轮：是1/否0
	UINT8            byArenaCnt; //赛场数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_ACTIVITY_DATA;

typedef struct tagDT_GVG_LOCAL_ACTIVITY_DATA{
	UINT16           wSessionID; //本次跨服战的届数
	UINT64           qwStartTime; //本次跨服战的开始时间
	UINT8            byCurStage; //当前进展到第几轮，见 GVG_STAGE
	UINT8            byDone; //本轮的工作是否已完成，可以进入到下一轮：是1/否0
	DT_GVG_ARENA_DATA stArena; //本区所处赛场的信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_LOCAL_ACTIVITY_DATA;

typedef struct tagDT_TSTRING_MSG{
	TCHAR            aszTstringMsg[NORMAL_MSG_LEN]; //信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_TSTRING_MSG;

typedef struct tagDT_BATTLE_LOG_MSG_DATA{
	UINT32           dwPlayerID; //玩家ID
	UINT8            byLogBigType; //战报大类型
	UINT8            byLogSmallType; //战报小类型
	UINT64           qwLogTime; //战报时间
	UINT8            byParamNum; //参数个数
	UINT64           aqwParamLst[MAX_BATTLE_LOG_PARAM_NUM]; //参数
	UINT8            byStrNum; //字符串个数
	DT_TSTRING_MSG   astStrLst[MAX_BATTLE_LOG_PARAM_STR]; //字符串

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_LOG_MSG_DATA;

typedef struct tagDT_BATTLE_LOG_MSG_LST_DATA{
	UINT8            byLogNum; //战报个数
	DT_BATTLE_LOG_MSG_DATA astLogData[MAX_BATTLE_LOG_NUM]; //战报信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_BATTLE_LOG_MSG_LST_DATA;

typedef struct tagDT_SOUL_LST_DATA{
	UINT16           wSoulNum; //战魂个数
	DT_SOUL_DATA     astSoulLstData[MAX_SOUL_BAG_NUM]; //战魂信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SOUL_LST_DATA;

typedef struct tagDT_PLAYER_SOUL_DATA{
	UINT8            byHaveDataFlag; //是否保存数据库
	DT_SOUL_LST_DATA stSoulBagData; //战魂背包
	UINT64           qwLastTime; //上次操作产出将魂时间
	UINT8            abyTodayCoinCnt[MAX_SOUL_PRODUCT_TYPE]; //今天产出魂剩余使用铜钱数
	UINT16           wFiveStarCnt; //五星卡必须探索大于这个数字才能获取
	UINT16           wSixStarCnt; //六星卡必须探索大于这个数字才能获取
	DT_SOUL_POTENCY_LST_DATA stSoulPotencyBagData; //战魂潜能

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_SOUL_DATA;

typedef struct tagDT_MEDAL_DATA{
	UINT16           wMedalID; //勋章ID
	UINT64           qwActivateTime; //激活时间
	UINT8            byIsActivate; //是否激活(0未激活，1激活)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MEDAL_DATA;

typedef struct tagDT_MEDAL_LST_DATA{
	UINT16           wMedalNum; //勋章个数
	DT_MEDAL_DATA    astMedalLstData[MAX_MEDAL_NUM]; //勋章

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MEDAL_LST_DATA;

typedef struct tagDT_MEDAL_BASE_DATA{
	UINT32           dwLoginDayNum; //玩家登录天数
	UINT64           qwLoginTime; //玩家上次登入时间
	UINT32           dwBuyPhystrength; //购买体力数
	UINT32           dwInterceptNum; //拦截次数
	UINT32           dwEscortNum; //护送次数
	UINT32           dwRockMoneyNum; //招财次数
	UINT32           dwNormalGatherNum; //普通聚魂次数
	UINT32           dwGoldGatherNum; //元宝聚魂次数
	UINT32           dwWorshipPlayer1Num; //膜拜1级别次数
	UINT32           dwWorshipPlayer2Num; //膜拜2级别次数
	UINT32           dwWorshipPlayer3Num; //膜拜3级别次数
	UINT32           dwWorshipGod1Num; //拜神1级别次数
	UINT32           dwWorshipGod2Num; //拜神2级别次数
	UINT32           dwWorshipGod3Num; //拜神3级别次数
	UINT32           dwZTSNum; //张天师算卦次数
	UINT32           dwSmileNum; //笑脸个数
	UINT32           dwArrestNum; //小黑屋抓捕次数
	UINT32           dwDriveNum; //小黑层驱赶次数
	UINT32           dwTowerResetNum; //爬塔重置次数
	UINT32           dwEliteInstanceResetNum; //精英副本重置次数
	UINT32           dwGVEJoinNum; //试练次数
	UINT32           dwRaceRank; //排名
	UINT64           qwLastRaceTime; //排名更新时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_MEDAL_BASE_DATA;

typedef struct tagDT_PLAYER_MEDAL_DATA{
	UINT8            byHaveDataFlag; //是否保存数据库
	DT_MEDAL_LST_DATA stMealData; //勋章数据
	UINT64           qwLastUpdateTime; //更新时间
	DT_MEDAL_BASE_DATA stMedalBaseData; //勋章基本次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_MEDAL_DATA;

typedef struct tagDT_PET_WASHQUALITY_DATA{
	UINT8            byIsCanWash; //是否可以洗
	UINT8            byCurWashQuality; //当前品质
	UINT32           dwPsychicVal; //资质值
	UINT32           dwMaxPsychicVal; //资质值最大值

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PET_WASHQUALITY_DATA;

typedef struct tagDT_PET_DATA{
	UINT16           wPetId; //id唯一标志
	UINT16           wPetCfgId; //配置id
	UINT8            byStarLv; //星阶
	UINT16           wLv; //等级
	UINT32           dwLvExp; //等级经验
	TCHAR            aszPetName[MAX_PET_NAME_LENGTH]; //名字
	UINT8            abyFeedFruit[MAX_PET_FEED_FRUIT_CNT]; //宠物喂养信息记录
	UINT8            byPetState; //宠物跟随记录
	DT_PET_WASHQUALITY_DATA astWashQualityData[MAX_PET_WASH_PSYCHIC_CELL_CNT]; //宠物洗练格子信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PET_DATA;

typedef struct tagDT_NEIDAN_DATA{
	UINT16           wNeiDanID; //内丹ID
	UINT16           wNeiDanIdx; //内丹区分IDX
	UINT16           wNeiDanLevel; //内丹等级
	UINT64           qwNeiDanExcpetion; //内丹经验

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NEIDAN_DATA;

typedef struct tagDT_PET_NEIDAN_DATA_LST{
	DT_NEIDAN_DATA   astNeiDanInfoLst[MAX_PET_NEIDAN_NUM]; //内丹信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PET_NEIDAN_DATA_LST;

typedef struct tagDT_PLAYER_NEIDAN_DATA_LST{
	UINT16           wPetNum; //宠物数量
	DT_PET_NEIDAN_DATA_LST astNeiDanInfo[MAX_PET_NUM]; //内丹信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_NEIDAN_DATA_LST;

typedef struct tagDT_NEIDAN_DATA_LST{
	UINT16           wNeiDanNum; //内丹数量
	DT_NEIDAN_DATA   astNeiDanInfo[MAX_NEI_DAN_BAG_NUM]; //内丹信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_NEIDAN_DATA_LST;

typedef struct tagDT_PET_DATA_LIST{
	UINT16           wPetNum; //宠物数量
	DT_PET_DATA      astPetData[MAX_PET_NUM]; //宠物数据

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PET_DATA_LIST;

typedef struct tagDT_PET_CALL_DATA{
	UINT16           wPetCfgID; //宠物配置ID
	UINT32           dwPetCallNum; //宠物召唤次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PET_CALL_DATA;

typedef struct tagDT_PET_CALL_LST_DATA{
	UINT16           wPetNum; //数量
	DT_PET_CALL_DATA astCallInfo[MAX_PET_NUM]; //召唤信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PET_CALL_LST_DATA;

typedef struct tagDT_HERO_HP_DATA{
	UINT16           wHeroID; //heroID
	UINT32           dwHeroHP; //HP
	UINT8            byFormationIdx; //位置

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_HP_DATA;

typedef struct tagDT_HERO_HP_DATA_LST{
	UINT8            byHeroNum; //英雄个数
	DT_HERO_HP_DATA  astHeroHPLst[MAX_FORMATION_IDX_NUM]; //英雄血量

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_HP_DATA_LST;

typedef struct tagDT_PLAYER_PET_DATA{
	UINT8            byHaveDataFlag; //是否保存数据库
	DT_PET_DATA_LIST stPetDataLst; //宠物数据
	UINT64           qwNormalLastTime; //普通召唤上次操作时间
	UINT64           qwGoodLastTime; //连续召唤上次操作时间
	UINT16           wNormalProductNum; //今日普通召唤次数
	UINT16           wGoodProductNum; //今日连续召唤次数
	UINT64           qwProductLastTime; //上次操作时间
	UINT32           dwPetScore; //宠物积分
	UINT64           qwLastFeedTime; //上次喂养时间
	UINT32           dwLeftFreeFeedCnt; //今日免费喂养次数
	UINT32           dwVIPFeedCnt; //今日VIP喂养次数
	DT_PET_CALL_LST_DATA stPetCallData; //召唤过的宠物类型与次数
	UINT16           wFreeNormalProductCnt; //普通召唤剩余次数
	UINT8            byCallFlag; //是否有新召唤标识
	DT_PLAYER_NEIDAN_DATA_LST stPetNeiDan; //宠物内丹信息
	DT_NEIDAN_DATA_LST stPetNeiDanBag; //内丹背包
	DT_HERO_HP_DATA_LST stHeroLstInfo; //英雄血量

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_PET_DATA;

typedef struct tagDT_PLAYER_NOPVP_DATA{
	DT_PLAYER_BASE_DATA stBaseData; //玩家基本信息
	DT_HERO_DATA_LIST stHeroData; //玩家武将信息列表
	DT_BAG_EQUIP_DATA_LST stBagEquipData; //玩家背包装备道具信息(存放于背包的装备，不包含已装备的装备)
	DT_BAG_GOODS_DATA_LST stBagGoodsData; //玩家背包物品道具信息
	DT_BUILD_DATA_LIST stBuildData; //玩家建筑信息
	DT_TASK_DATA_LIST stTaskData; //玩家任务信息
	DT_LOCALE_DATA   stLocaleData; //玩家当前位置信息
	DT_STATE_DATA    stCurState; //玩家当前状态信息
	DT_ONLINE_ENCOURAGE_RECORD_DATA stOnlineEncRecdData; //在线奖励信息
	DT_LOGIN_ENCOURAGE_RECORD_DATA stLoginEncRecdData; //登陆奖励信息
	DT_INSTANCE_DB_DATA stInstanceData; //玩通副本信息
	DT_MONEY_TREE_DATA stMoneyTreeData; //摇钱树信息
	DT_ONHOOK_RCD_DATA stOnhookRcdData; //挂机记录信息
	DT_JINGJIE_DATA  stJingJieData; //境界信息
	DT_PHYSTRENGTH_DATA stPhystrengthData; //体力信息
	DT_FEW_PARAM_DATA stFewParamData; //少参数信息
	DT_CROP_DATA     stCropData; //种植信息
	DT_SCIENCE_TREE_DATA stScienceTree; //科技树信息
	DT_NEW_FORMATION_DATA stFormationInfo; //阵型信息
	CHAR             szLastOrderID[MAX_ORDER_ID_LEN]; //上次处理的订单号（用于避免重复处理）
	DT_EAT_BREAD_DATA stEatBreadInfo; //吃包子信息
	DT_YABIAO_DATA   stYabiaoInfo; //押镖信息
	UINT8            abyNoticeID[MAX_TOKEN_NUM]; //玩家消息推送ID
	UINT64           qwLoginTime; //最后登录时间
	UINT64           qwLogoffTime; //最后登出时间
	DT_CLIMB_TOWER_DATA stClimbTowerData; //玩家爬塔副本信息
	DT_AWAKEN_DATA   stAwakenData; //悟道信息
	DT_VIP_EXT_DATA  stVipExtData; //VIP增加的信息
	DT_LUAACTIVITY_RECORD_LIST_DATA stLuaActivityRecordData; //lua脚本活动记录
	DT_COLLECT_GAS_DATA stGasCollectData; //灵气收集信息
	DT_BOSS_BATTLE_PLAYER_DATA_INFO stBossBattlePlayerDataInfo; //Boss战信息
	DT_ACTIVITY_ENCOURAGE_DATA stActivityEncourageData; //奖励信息
	DT_RANDOM_CHALLENGE_DATA stRdChallengeData; //随机挑战信息
	DT_WORSHIP_DATA_LST stWorshipData; //膜拜信息
	DT_SCORE_DATA_LST stScoreData; //副本评分信息
	DT_EQUIP_COMPOSE_DATA stEquipComposeData; //装备合成信息
	DT_GATHER_SCIENCE_DATA stGatherScienceData; //聚魂信息
	DT_BEGBLUE_DATA  stBegBlueData; //求仙露信息
	DT_PLAYER_TODAY_CONSUME_DATA stTodayConsume; //今天的消费
	DT_FRIENDS_DATA  stFriendsData; //送鲜花数据
	DT_PLAYER_FACTION_DATA stPlayerFactionData; //玩家门派信息
	DT_COURAGE_DATA  stCourageData; //玩家勇气信息
	DT_EQUIP_MASTER  stEquipMasterData; //装备精通信息
	DT_EXCHANGE_RESULT_LOG stExchangeResultLog; //推送日志
	DT_BAG_JEWELRY_DATA_LST stBagJewelryInfo; //玩家饰品信息
	DT_PLAYER_GVG_DATA stGvGData; //玩家gvg信息
	DT_PLAYER_SOUL_DATA stSoulData; //玩家战魂背包
	DT_PLAYER_MEDAL_DATA stMedalData; //玩家勋章信息
	DT_PLAYER_PET_DATA stPetData; //玩家宠物信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_NOPVP_DATA;

typedef struct tagDT_PLAYER_PVP_DATA{
	DT_RACE_DATA     stRaceData; //竞技场信息
	DT_PLUNDER_DATA  stPlunderData; //掠夺信息
	DT_ENEMY_DATA_LIST stEnemyData; //仇敌信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_PVP_DATA;

typedef struct tagDT_PLAYER_DATA{
	DT_PLAYER_NOPVP_DATA stPlayerNoPvpData; //玩家非PVP信息
	DT_PLAYER_PVP_DATA stPlayerPvpData; //玩家PVP信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_DATA;

typedef struct tagDT_EXT_FUNC_PARAM{
	UINT8            byParam1; //参数1
	UINT16           wParam2; //参数2
	UINT32           dwParam3; //参数3

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EXT_FUNC_PARAM;

typedef struct tagDT_EXT_FUNC_PARAM_LST{
	UINT8            byFuncNum; //扩展功能数
	DT_EXT_FUNC_PARAM astExtFuncParaInfo[MAX_EXT_FUN_NUM]; //扩展功能信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_EXT_FUNC_PARAM_LST;

typedef struct tagDT_CONTINUE_KILL_DATA{
	UINT8            byNum; //奖励数
	UINT64           aqwKeyLst[MAX_FACTION_BATTLE_NUM]; //奖励ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CONTINUE_KILL_DATA;

typedef struct tagDT_FACTION_PLAYER_DATA{
	UINT32           dwJoinPlayerID; //玩家ID
	UINT8            byJoinState; //玩家当前状态
	UINT8            byJobType; //职业类型
	UINT32           dwDoorsTribute; //剩余门贡
	UINT64           qwDoorsTributeSum; //累积门贡
	UINT8            byDrinkFlag; //饮酒事件重启标识
	UINT8            byNewGoodFlag; //新货物
	UINT8            byNewSkillFlag; //新技能
	DT_PLAYER_FACTION_BATTLELOG_DATA_LIST stUnLoginBattleLogList; //门派未读战报
	UINT64           qwJoinTime; //加入门派时间
	DT_CONTINUE_KILL_DATA stContinueKillKey; //领取的连斩礼包

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_PLAYER_DATA;

typedef struct tagDT_FACTION_PLAYER_DATA_LST{
	UINT32           dwFactionNum; //用户数
	DT_FACTION_PLAYER_DATA astFactionPlayerData[DT_FACTION_PLAYER_NUM]; //门派用户信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_PLAYER_DATA_LST;

typedef struct tagDT_FACTION_LOG_DATA{
	UINT32           dwPlayerID1; //玩家ID
	UINT8            byPlayerJobType1; //玩家职位
	UINT32           dwPlayerID2; //玩家ID
	UINT8            byPlayerJobType2; //玩家职位
	UINT8            byAction; //事件ID
	UINT32           adwParamList[FACTION_LOG_LIST_MAX_NUM]; //参数数组
	UINT64           qwLogTime; //时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_LOG_DATA;

typedef struct tagDT_FACTION_LOG_DATA_LST{
	UINT16           wFactionLogNum; //用户数
	DT_FACTION_LOG_DATA astFactionLogData[MAX_FACTION_LOG_NUM]; //门派用户信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_LOG_DATA_LST;

typedef struct tagDT_FACTION_BASE_DATA{
	UINT32           dwFactionID; //门派ID
	UINT32           dwFactionPlayerID; //帮主ID
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT32           dwFactionRank; //门派排名
	UINT32           dwFactionLevel; //门派等级
	TCHAR            aszFactionNotice[MAX_FACTION_DESC_NUM]; //门派通知
	TCHAR            aszFactionAnnouncement[MAX_FACTION_DESC_NUM]; //门派公告
	UINT64           qwFactionCreateTime; //门派创建时间
	UINT32           dwRareItemRoomGrade; //珍宝阁等级
	UINT32           dwGymnasiumLevel; //武堂等级
	UINT32           dwShadeLevel; //暗部等级
	UINT32           dwFactionFunds; //门派资金
	UINT32           dwHotSpringLevel; //温泉等级
	UINT32           dwColdPrisonLevel; //冰牢等级
	UINT32           dwFactionTowerLevel; //昊天塔等级
	UINT64           qwFactionFundsSum; //门派累积资金

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_BASE_DATA;

typedef struct tagDT_FACTION_DATA{
	DT_FACTION_BASE_DATA stFactionBaseData; //门派基础数据
	DT_FACTION_PLAYER_DATA_LST stFactionPlayerDataLst; //门派玩家数据信息
	DT_FACTION_LOG_DATA_LST stFactionLogDataLst; //门派日志数据信息
	DT_FACTION_COMMON_DATA stFactionCommonData; //门派状态信息
	UINT8            byFactionException; //门派数据异常标识（0 为正常， 1 为异常）
	UINT8            byTodayPassNum; //今天审核通过人数
	UINT64           qwLastAuditTime; //最后一次审核时间
	UINT8            byExt; //扩展位信息
	DT_FACTION_GVG_DATA stGvGData; //门派gvg数据

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_DATA;

typedef struct tagDT_FACTION_AUTHORITY{
	UINT8            byKickedNormal; //踢出普通弟子
	UINT8            byKickedElite; //踢出精英弟子
	UINT8            byPromotion; //升职
	UINT8            byDemotion; //降职
	UINT8            byBuild; //升级门派建筑
	UINT8            byActivity; //开启门派特殊活动
	UINT8            bySignUp; //报名门派战
	UINT8            byAudit; //审核
	UINT8            byEditSetting; //修改门派设置
	UINT8            byShowLog; //查看日志
	UINT8            byChangeOther; //退位让贤
	UINT8            byBecomeHead; //继位帮主

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_AUTHORITY;

typedef struct tagDT_FACTION_AUTHORITY_EX{
	UINT8            byKickedNormal; //踢出普通弟子
	UINT8            byKickedElite; //踢出精英弟子
	UINT8            byPromotion; //升职
	UINT8            byDemotion; //降职
	UINT8            byBuild; //升级门派建筑
	UINT8            byActivity; //开启门派特殊活动
	UINT8            bySignUp; //报名门派战
	UINT8            byAudit; //审核
	UINT8            byEditSetting; //修改门派设置
	UINT8            byShowLog; //查看日志
	UINT8            byChangeOther; //退位让贤
	UINT8            byBecomeHead; //继位帮主
	UINT8            byChangeIcon; //改变头像

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_AUTHORITY_EX;

typedef struct tagDT_COURAGE_PLAYER_DATA{
	TCHAR            aszDispPlayerName[USERNAME_LEN]; //玩家昵称
	UINT16           wLevel; //玩家等级
	UINT32           dwPlayerID; //玩家ID
	UINT8            byMemberType; //队员类型 0：队员， 1：队长
	UINT8            byBattleType; //战斗类型 0：加入战斗， 1：协助战斗
	UINT8            byReadyType; //准备类型 0：准备中， 1：已经准备
	UINT16           wHeroID; //伙伴ID
	UINT16           wKindID; //玩家职业ID
	UINT16           wHeroLevel; //伙伴等级
	UINT8            byColor; //武将颜色信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_PLAYER_DATA;

typedef struct tagDT_COURAGE_GROUP_DATA{
	UINT32           dwGroupID; //组ID
	UINT8            byJoinType; //加入类型 0：全部 1：帮派，2：好友
	UINT16           wMinLevel; //加入最底等级
	UINT16           wPlayerNum; //玩家个数
	DT_COURAGE_PLAYER_DATA astPlayerData[MAX_COURAGE_GROUP_PLAYER_NUM]; //玩家勇气信息
	UINT32           dwLeaderID; //队长ID
	UINT16           wAverageLevel; //队伍平均等级
	DT_INSTANCE_ID   stInstanceID; //副本序号
	UINT16           wPlayerMaxNum; //玩家最大个数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_GROUP_DATA;

typedef struct tagDT_COURAGE_GROUP_DATA_LST{
	UINT64           qwOverTime; //活动结束时间
	UINT64           qwStartTime; //活动开始时间
	UINT64           qwSaveTime; //活动保存更新时间
	UINT8            byActivityID; //活动ID
	UINT16           wGroupNum; //玩家个数
	DT_COURAGE_GROUP_DATA astGroupData[MAX_COURAGE_GROUP_NUM]; //玩家信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COURAGE_GROUP_DATA_LST;

typedef struct tagDT_FACTION_APPLAY_DATA{
	UINT64           qwApplayTime; //报名时间
	UINT32           dwFactionID; //门派ID
	UINT8            byWinCnt; //获胜次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_APPLAY_DATA;

typedef struct tagDT_FACTION_APPLAY_LST_DATA{
	UINT16           wApplayNum; //报名个数
	DT_FACTION_APPLAY_DATA astFactionApplayLst[MAX_FACTION_APPLAY_NUM]; //报名列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_APPLAY_LST_DATA;

typedef struct tagDT_SIMPLE_FACTION_INFO{
	DT_FACTION_BASE_DATA_CLI2 stFactionBaseInfo; //门派基本数据
	UINT32           dwZoneID; //所有区ID
	UINT16           wRank; //排名（有排名时有效）
	UINT8            byPosID; //位置ID(只有在双服战时有效)

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SIMPLE_FACTION_INFO;

typedef struct tagDT_SIMPLE_FACTION_LST_INFO{
	UINT16           wFactionNum; //门派数量
	DT_SIMPLE_FACTION_INFO astFactionInfo[MAX_RANK_DISPLAY]; //门派信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SIMPLE_FACTION_LST_INFO;

typedef struct tagDT_GVG_FACTION_RANK_DATA{
	UINT16           wRank; //排名
	DT_FACTION_BASE_DATA_CLI2 stFactionBaseInfo; //门派基础数据
	UINT64           qwSumPower; //总战力
	UINT32           dwSumIntegral; //总积分
	UINT8            byWinNum; //胜利数
	UINT8            byLostNum; //失利数
	UINT16           wZoneID; //区号

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_FACTION_RANK_DATA;

typedef struct tagDT_GVG_FACTION_RANK_LST_DATA{
	UINT16           wRankNum; //排名个数
	DT_GVG_FACTION_RANK_DATA astFactionRank[MAX_RANK_DISPLAY]; //排名数据
	DT_GVG_FACTION   stNo1; //冠军

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_FACTION_RANK_LST_DATA;

typedef struct tagDT_FACTION_BATTLE_RESULT{
	UINT32           dwFactionID1; //对战门派ID1
	UINT32           dwFactionID2; //对战门派ID2
	UINT32           dwWinFactionID; //胜利门派ID

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_BATTLE_RESULT;

typedef struct tagDT_FACTION_BATTLE_RESULT_LST{
	UINT8            byResultNum; //信息个数
	DT_FACTION_BATTLE_RESULT astBattleInfo[MAX_FACTION_BATTLE_NUM]; //对战信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_FACTION_BATTLE_RESULT_LST;

typedef struct tagDT_LOCAL_WAR_FACTION_SEED{
	UINT32           dwFactionID; //门派ID
	UINT8            byWinCnt; //在门派大乱斗中的获胜次数

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCAL_WAR_FACTION_SEED;

typedef struct tagDT_LOCAL_WAR_FACTION_SEED_LIST{
	UINT16           wFactionCnt; //门派选手数量
	UINT32           adwFactionIDList[MAX_FACTION_APPLAY_NUM]; //对战信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_LOCAL_WAR_FACTION_SEED_LIST;

typedef struct tagDT_CHAOS_WAR{
	UINT8            byChaosRound; //本次大乱斗共有几个轮次
	DT_SIMPLE_FACTION_LST_INFO stChaosResult; //大乱斗排行名单
	DT_LOCAL_WAR_FACTION_SEED_LIST stFactionSeedList; //第一轮种子选手配对表：配对的规则为<第1个门派 vs 第2个门派>、<第3个门派 vs 第4个门派>...

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_CHAOS_WAR;

typedef struct tagDT_GVG_BUILD_DATA{
	DT_FACTION_APPLAY_LST_DATA stApplayInfo; //门派报名信息
	DT_GVG_FACTION_RANK_LST_DATA stLastResult; //上届信息
	DT_SIMPLE_FACTION_LST_INFO stMoneyResult; //财力排行名单
	DT_CHAOS_WAR     stChaosWar; //大乱斗排行名单
	DT_16_TO_8_TO_4_WAR_INFO stWar16To8To4Info; //本服所处的双服组对战信息
	DT_WAR_16_TO_8_LIST stWar16To8List; //多个双服的对战信息列表
	DT_FINAL_WAR_INFO stFinalWarInfo; //四服积分总决赛信息
	DT_FACTION_LIST_SIMPLE_GVG_LOG stFactionsLog; //各门派的战况
	DT_GVG_STAGE_FINISH_FLAG_LIST stStageFlagList; //标记各个阶段是否已完成任务
	UINT8            byCurStage; //当前进展到第几轮，见 GVG_STAGE
	UINT16           wSessionID; //本次跨服战的届数
	UINT16           wArenaID; //本区所属赛区ID
	UINT64           qwStartTime; //本次跨服战的开启时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_BUILD_DATA;

typedef struct tagDT_GVG_FACTION_LIST{
	UINT8            byFactionNum; //门派个数
	DT_GVG_FACTION   astFactionLstData[MAX_GVG_FACTION_NUM]; //门派信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_GVG_FACTION_LIST;

/*Define Structs and Unions        End*/


/*Define MaxEnLen MAC Begin*/
#define  MAXENLEN_DT_RES_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64) )
#define  MAXENLEN_DT_ITEM_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_RES_NUM_ITEM_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+2+MAX_PLAYER_RES_NUM*MAXENLEN_DT_RES_DATA+sizeof(UINT8)+2+MAX_PLAYER_RES_NUM*MAXENLEN_DT_ITEM_DATA )
#define  MAXENLEN_DT_GUIDE_RECORD_DATA	( 1+1*sizeof(SIndex)+2+MAX_GUIDE_NUM*sizeof(UINT8) )
#define  MAXENLEN_DT_PLAYER_BASE_DATA	( 1+28*sizeof(SIndex)+sizeof(UINT32)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+MAXENLEN_DT_GUIDE_RECORD_DATA+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT16) )
#define  MAXENLEN_DT_BATTLE_ATTRIBUTE	( 1+2*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_BATTLE_ATTRIBUTE2	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_BATTLE_BASE_ATTRIBUTE	( 1+3*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_HIDE_BATTLE_ATTRIBUTE	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_EXT_BATTLE_ATTRIBUTE	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT16) )
#define  MAXENLEN_DT_HERO_UPDATE_ATTR_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_HERO_ATTR_DATA	( 1+3*sizeof(SIndex)+MAXENLEN_DT_BATTLE_ATTRIBUTE+MAXENLEN_DT_HIDE_BATTLE_ATTRIBUTE+MAXENLEN_DT_HERO_UPDATE_ATTR_DATA )
#define  MAXENLEN_DT_SPECIAL_EFFECT_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_SKILL_SOUL_NUM*sizeof(UINT8) )
#define  MAXENLEN_DT_SOUL_ATTR_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_SOUL_POTENCY_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_SOUL_POTENCY_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_SOUL_BAG_NUM*MAXENLEN_DT_SOUL_POTENCY_DATA )
#define  MAXENLEN_DT_SOUL_DATA	( 1+15*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+2+MAX_SOUL_ATTR_NUM*MAXENLEN_DT_SOUL_ATTR_DATA+sizeof(UINT8)+2+MAX_SOUL_ATTR_NUM*MAXENLEN_DT_SOUL_ATTR_DATA+sizeof(UINT8)+MAXENLEN_DT_SPECIAL_EFFECT_DATA+2+MAX_SOUL_ATTR_NUM*sizeof(UINT8) )
#define  MAXENLEN_DT_HERO_SOUL_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT16)+2+MAX_SOUL_EQUIP_NUM*MAXENLEN_DT_SOUL_DATA+2+MAX_SOUL_EQUIP_NUM*MAXENLEN_DT_SOUL_POTENCY_DATA )
#define  MAXENLEN_DT_HERO_BASE_DATA	( 1+18*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_BATTLE_ATTRIBUTE+MAXENLEN_DT_HIDE_BATTLE_ATTRIBUTE+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT64)+MAXENLEN_DT_HERO_UPDATE_ATTR_DATA+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_XIANGQIAN_DATA	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_EQUIP_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64)+2+MAX_XIANGQIAN_NUM*MAXENLEN_DT_XIANGQIAN_DATA )
#define  MAXENLEN_DT_HERO_EQUIP_DATA_LST	( 1+3*sizeof(SIndex)+sizeof(UINT16)+2+EQUIP_POS_NUM*MAXENLEN_DT_EQUIP_DATA+2+EQUIP_POS_NUM*sizeof(UINT16) )
#define  MAXENLEN_DT_BAG_EQUIP_DATA_LST	( 1+3*sizeof(SIndex)+sizeof(UINT16)+2+MAX_BAG_OPEN_NUM_NEW*MAXENLEN_DT_EQUIP_DATA+2+MAX_BAG_OPEN_NUM_NEW*sizeof(UINT16) )
#define  MAXENLEN_DT_GOODS_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_BAG_GOODS_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_BAG_OPEN_NUM_NEW*MAXENLEN_DT_GOODS_DATA )
#define  MAXENLEN_DT_DRUG_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_DRUG_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_DRUG_LEVEL*MAXENLEN_DT_DRUG_DATA )
#define  MAXENLEN_DT_GODWEAPON_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_ATTACK_RANG_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_SKILL_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+MAXENLEN_DT_ATTACK_RANG_DATA+sizeof(UINT32) )
#define  MAXENLEN_DT_SKILL_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_SKILL_SLOT_NUM*MAXENLEN_DT_SKILL_DATA )
#define  MAXENLEN_DT_STUDY_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_AWAKEN_BASE_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64) )
#define  MAXENLEN_DT_AWAKEN_BASE_DATA1	( 1+5*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_AWAKEN_DATA	( 1+4*sizeof(SIndex)+MAXENLEN_DT_AWAKEN_BASE_DATA+2+MAX_BAG_STUDY_GRID_NUM*MAXENLEN_DT_STUDY_DATA+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_AWAKEN_DATA_LST	( 1+1*sizeof(SIndex)+2+MAX_HERO_STUDY_GRID_NUM*MAXENLEN_DT_STUDY_DATA )
#define  MAXENLEN_DT_ABSORB_DATA_LST	( 1+3*sizeof(SIndex)+sizeof(UINT8)+2+MAX_ABSORB_NUM*sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_ATTR_INC_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_BATTLE_ATTR_LST_INFO	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_SCIENCE_ATTR_NUM*MAXENLEN_DT_ATTR_INC_DATA )
#define  MAXENLEN_DT_BATTLE_ATTR_EXT_INFO	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_BATTLE_ATTR_EXT_LST_INFO	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_SCIENCE_ATTR_NUM*MAXENLEN_DT_BATTLE_ATTR_EXT_INFO )
#define  MAXENLEN_DT_JEWELRY_DATA	( 1+10*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+MAXENLEN_DT_BATTLE_ATTR_LST_INFO+MAXENLEN_DT_BATTLE_ATTR_EXT_LST_INFO+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16) )
#define  MAXENLEN_DT_JEWELRY_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_JEWELRY_POS*MAXENLEN_DT_JEWELRY_DATA )
#define  MAXENLEN_DT_BAG_JEWELRY_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_BAG_OPEN_NUM_NEW*MAXENLEN_DT_JEWELRY_DATA )
#define  MAXENLEN_DT_HERO_DATA	( 1+7*sizeof(SIndex)+MAXENLEN_DT_HERO_BASE_DATA+MAXENLEN_DT_HERO_EQUIP_DATA_LST+MAXENLEN_DT_DRUG_DATA_LST+MAXENLEN_DT_GODWEAPON_DATA+MAXENLEN_DT_SKILL_DATA_LST+MAXENLEN_DT_AWAKEN_DATA_LST+MAXENLEN_DT_ABSORB_DATA_LST )
#define  MAXENLEN_DT_HERO_DATA_LIST	( 1+4*sizeof(SIndex)+sizeof(UINT16)+2+MAX_HERO_NUM*MAXENLEN_DT_HERO_DATA+2+MAX_HERO_NUM*MAXENLEN_DT_JEWELRY_DATA_LST+2+MAX_HERO_NUM*MAXENLEN_DT_HERO_SOUL_DATA )
#define  MAXENLEN_DT_BUILD_BASE_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_BUILD_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_BUILD_NUM*MAXENLEN_DT_BUILD_BASE_DATA )
#define  MAXENLEN_DT_INSTANCE_ID	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_INSTANCE_DATA	( 1+7*sizeof(SIndex)+MAXENLEN_DT_INSTANCE_ID+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_INSTANCE_DATA_LIST	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+2+MAX_INSTANCE_PER_SCENE_NUM*MAXENLEN_DT_INSTANCE_DATA )
#define  MAXENLEN_DT_SCIENCE_ATTREXT	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_SCIENCE_ATTREXT_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_SCIENCE_ATTR_NUM*MAXENLEN_DT_SCIENCE_ATTREXT )
#define  MAXENLEN_DT_CROWN_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_BUILDUPGRADE_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_RECRUITHERO_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_CROWN_HEROCOLOR_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_CROWN_HEROUPGRADE_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_FIRSTPASSINSTANCE_TASK_CONDITION	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_SCOREPASSINSTANCE_TASK_CONDITION	( 1+4*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_TIMESPASSINSTANCE_TASK_CONDITION	( 1+4*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_KILLMONSTERINSTANCE_TASK_CONDITION	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_STRENGTHENEQUIPNUM_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_ITEMCOLLECT_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_ITEM_KIND_COLLECT_NUM*sizeof(UINT8) )
#define  MAXENLEN_DT_ACTIVATEGODWEAPON_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_UPGRADEGODWEAPON_TASK_CONDITION	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_UPGRADEDRUG_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_CHALLENGE_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_PLUNDER_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_DRESSSKILL_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_DRESSEQUIP_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_UPGRADEEQUIP_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_USEEXPRIENCEDRUG_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_MELT_EQUIP_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_COMPOSEGOOD_TASK_CONDITION	( 1+4*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_ELITEINSTANCE_TASK_CONDITION	( 1+4*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_UPGRADSCIENCE_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_STRENGTHATTACK_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_STRENGTHLIFE_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_CROP_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_UPGRADESKILL_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_UPGRADEJINGJIE_TASK_CONDITION	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_MOSAICGEM_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_CLIMBTOWER_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_STUDY_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT8) )
#define  MAXENLEN_DT_RECRUITHERO_NUM_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT16) )
#define  MAXENLEN_DT_OPENLIGHT_TASK_CONDITION	( 1+1*sizeof(SIndex)+sizeof(UINT8) )
#define  MAXENLEN_DT_TASK_CONDITION ( 10*sizeof(DT_TASK_CONDITION) )
#define  MAXENLEN_DT_TASK_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(DT_TASK_CONDITION) )
#define  MAXENLEN_DT_TASK_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_TASK_LINE_NUM*MAXENLEN_DT_TASK_DATA )
#define  MAXENLEN_DT_RACE_BASE_DATA	( 1+11*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_LOCALE_HOME	( 1+1*sizeof(SIndex)+sizeof(UINT8) )
#define  MAXENLEN_DT_LOCALE_INSTANCE	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_LOCALE_ONHOOK	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_LOCALE_MARK	( 1+1*sizeof(SIndex)+sizeof(UINT8) )
#define  MAXENLEN_DT_LOCALE_DATA_UNION ( 10*sizeof(DT_LOCALE_DATA_UNION) )
#define  MAXENLEN_DT_LOCALE_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(DT_LOCALE_DATA_UNION) )
#define  MAXENLEN_UnlockScienceInfo	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_SCIENCE_ATTR_NUM*sizeof(UINT8) )
#define  MAXENLEN_DT_HERO_UPGRADE_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT16) )
#define  MAXENLEN_DT_STATE_DATA	( 1+13*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT8)+2+MAX_ENCOURAGE_BOX_GROUP_NUM*sizeof(UINT8)+2+MAX_UNLOCK_HERO_NUM*sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+MAXENLEN_UnlockScienceInfo+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_HERO_UPGRADE_DATA )
#define  MAXENLEN_DT_MONSTER_BASE_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_MONSTER_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_INSTANCE_MONSTER_NUM*MAXENLEN_DT_MONSTER_BASE_DATA )
#define  MAXENLEN_DT_GODWEAPON_BATTLE_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+MAXENLEN_DT_BATTLE_ATTRIBUTE+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_BATTLE_OBJ_DATA	( 1+11*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+MAXENLEN_DT_BATTLE_ATTRIBUTE+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_BATTLE_OBJ_DATA2	( 1+12*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+MAXENLEN_DT_BATTLE_ATTRIBUTE+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_GODWEAPON_BATTLE_DATA )
#define  MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_BATTLE_OBJ_DATA+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT32) )
#define  MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA2	( 1+4*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_BATTLE_OBJ_DATA2+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT32) )
#define  MAXENLEN_DT_BUFF_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_HP_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(INT32) )
#define  MAXENLEN_DT_ANGRY_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(INT16) )
#define  MAXENLEN_DT_PARRY_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_SKILL_EFFECT_ID_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_ATTACK_OBJ_DATA	( 1+8*sizeof(SIndex)+sizeof(UINT8)+2+MAX_BUFF_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_BUFF_DATA+sizeof(UINT8)+2+MAX_HP_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_HP_DATA+sizeof(UINT8)+2+MAX_ANGRY_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_ANGRY_DATA+sizeof(UINT8)+2+MAX_BUFF_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_SKILL_EFFECT_ID_DATA )
#define  MAXENLEN_DT_ATTACK_OBJ_DATA2	( 1+8*sizeof(SIndex)+sizeof(UINT8)+2+MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_BUFF_DATA+sizeof(UINT8)+2+MAX_HP_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_HP_DATA+sizeof(UINT8)+2+MAX_NEW_ANGRY_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_ANGRY_DATA+sizeof(UINT8)+2+MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_SKILL_EFFECT_ID_DATA )
#define  MAXENLEN_DT_ATTACK_DATA	( 1+2*sizeof(SIndex)+MAXENLEN_DT_ATTACK_OBJ_DATA+MAXENLEN_DT_ATTACK_OBJ_DATA )
#define  MAXENLEN_DT_ATTACK_DATA2	( 1+2*sizeof(SIndex)+MAXENLEN_DT_ATTACK_OBJ_DATA2+MAXENLEN_DT_ATTACK_OBJ_DATA2 )
#define  MAXENLEN_DT_GODWEAPON_ATTACK_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+2+MAX_ANGRY_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_ANGRY_DATA )
#define  MAXENLEN_DT_GODWEAPON_ATTACK_DATA2	( 1+5*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_ANGRY_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_ANGRY_DATA )
#define  MAXENLEN_DT_ATTACK_ALL_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_PARRY_DATA+MAXENLEN_DT_ATTACK_DATA+MAXENLEN_DT_ATTACK_DATA )
#define  MAXENLEN_DT_ATTACK_ALL_DATA2	( 1+8*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_PARRY_DATA+MAXENLEN_DT_ATTACK_DATA+MAXENLEN_DT_ATTACK_DATA+MAXENLEN_DT_GODWEAPON_ATTACK_DATA )
#define  MAXENLEN_DT_ATTACK_ALL_DATA3	( 1+8*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_PARRY_DATA+MAXENLEN_DT_ATTACK_DATA2+MAXENLEN_DT_ATTACK_DATA2+MAXENLEN_DT_GODWEAPON_ATTACK_DATA )
#define  MAXENLEN_DT_BATTLE_ANGRY_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_BATTLE_ROUND_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_ATTACK_ALL_DATA )
#define  MAXENLEN_DT_BATTLE_ROUND_DATA2	( 1+2*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_ATTACK_ALL_DATA2 )
#define  MAXENLEN_DT_BATTLE_ROUND_DATA3	( 1+2*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_ATTACK_ALL_DATA3 )
#define  MAXENLEN_DT_BATTLE_DATA	( 1+7*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_BATTLE_ROUND_NUM*MAXENLEN_DT_BATTLE_ROUND_DATA+MAXENLEN_DT_BATTLE_ATTRIBUTE )
#define  MAXENLEN_DT_BATTLE_DATA2	( 1+7*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA2+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA2+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_BATTLE_ROUND_NUM*MAXENLEN_DT_BATTLE_ROUND_DATA2+MAXENLEN_DT_BATTLE_ATTRIBUTE )
#define  MAXENLEN_DT_BATTLE_DATA3	( 1+7*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA2+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA2+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_NEW_BATTLE_ROUND_NUM*MAXENLEN_DT_BATTLE_ROUND_DATA3+MAXENLEN_DT_BATTLE_ATTRIBUTE )
#define  MAXENLEN_DT_SHIELD_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_HP_DATA2	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(INT32)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_ATTACK_OBJ_DATA3	( 1+10*sizeof(SIndex)+sizeof(UINT8)+2+MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_BUFF_DATA+sizeof(UINT8)+2+MAX_HP_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_HP_DATA2+sizeof(UINT8)+2+MAX_NEW_ANGRY_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_ANGRY_DATA+sizeof(UINT8)+2+MAX_NEW_BUFF_CHANGE_TIMES_PER_ROUND*MAXENLEN_DT_SKILL_EFFECT_ID_DATA+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_SHIELD_DATA )
#define  MAXENLEN_DT_ATTACK_DATA3	( 1+2*sizeof(SIndex)+MAXENLEN_DT_ATTACK_OBJ_DATA3+MAXENLEN_DT_ATTACK_OBJ_DATA3 )
#define  MAXENLEN_DT_PARRY_DATA2	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_ATTACK_ALL_DATA4	( 1+10*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_PARRY_DATA2+MAXENLEN_DT_ATTACK_DATA3+MAXENLEN_DT_ATTACK_DATA3+MAXENLEN_DT_GODWEAPON_ATTACK_DATA2+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_BATTLE_ROUND_DATA4	( 1+2*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_ATTACK_ALL_DATA4 )
#define  MAXENLEN_DT_BATTLE_DATA4	( 1+7*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA2+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA2+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_NEW_BATTLE_ROUND_NUM*MAXENLEN_DT_BATTLE_ROUND_DATA4+MAXENLEN_DT_BATTLE_ATTRIBUTE )
#define  MAXENLEN_DT_ATTACK_ALL_DATA5	( 1+13*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_PARRY_DATA2+MAXENLEN_DT_ATTACK_DATA3+MAXENLEN_DT_ATTACK_DATA3+MAXENLEN_DT_GODWEAPON_ATTACK_DATA2+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_BATTLE_ROUND_DATA5	( 1+2*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_ATTACK_ALL_DATA5 )
#define  MAXENLEN_DT_PET_BATTLE_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA3	( 1+5*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_BATTLE_OBJ_DATA2+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT32)+MAXENLEN_DT_PET_BATTLE_DATA )
#define  MAXENLEN_DT_BATTLE_DATA5	( 1+7*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA3+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA3+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_NEW_BATTLE_ROUND_NUM*MAXENLEN_DT_BATTLE_ROUND_DATA5+MAXENLEN_DT_BATTLE_ATTRIBUTE )
#define  MAXENLEN_DT_BATTLE_OBJ_MAX_HP	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_BATTLE_OBJ_MAX_HP )
#define  MAXENLEN_DT_EMPTY_HP_VIDEO_v400	( 1+5*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_DATA4 )
#define  MAXENLEN_DT_EMPTY_HP_VIDEO	( 1+5*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_OBJ_MAX_HP_INFO+MAXENLEN_DT_BATTLE_DATA5 )
#define  MAXENLEN_DT_FINAL_ROUND_BATTLE_OBJ_INFO	( 1+2*sizeof(SIndex)+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA3+MAXENLEN_DT_BATTLE_OBJ_GROUP_DATA3 )
#define  MAXENLEN_DT_ONLINE_ENCOURAGE_RECORD_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_LOGIN_ENCOURAGE_RECORD_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_RACE_BATTLELOG_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_RACE_BATTLELOG_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_RACE_BATTLELOG_NUM*MAXENLEN_DT_RACE_BATTLELOG_DATA )
#define  MAXENLEN_DT_CAPTURE_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64) )
#define  MAXENLEN_DT_CAPTURE_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_CAPTURE_NUM*MAXENLEN_DT_CAPTURE_DATA )
#define  MAXENLEN_DT_PLUNDER_BATTLELOG_DATA	( 1+9*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_PLUNDER_BATTLELOG_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_PLUNDER_BATTLELOG_NUM*MAXENLEN_DT_PLUNDER_BATTLELOG_DATA )
#define  MAXENLEN_DT_PLUNDER_GRID_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_PLUNDER_BASE_DATA	( 1+10*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16)+2+MAX_CAPTURE_NUM*MAXENLEN_DT_PLUNDER_GRID_DATA )
#define  MAXENLEN_DT_ENEMY_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_ENEMY_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_ENEMY_NUM*MAXENLEN_DT_ENEMY_DATA )
#define  MAXENLEN_DT_MONEY_TREE_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT64) )
#define  MAXENLEN_DT_INSTANCE_BATTLE_DATA	( 1+5*sizeof(SIndex)+MAXENLEN_DT_INSTANCE_ID+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_PHYSTRENGTH_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT16) )
#define  MAXENLEN_DT_ONHOOK_RESULT	( 1+9*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8)+2+MAX_ENCOURAGE_ITEM_KIND_NUM*MAXENLEN_DT_ITEM_DATA+sizeof(UINT32) )
#define  MAXENLEN_DT_ONHOOK_RCD_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_ONHOOK_RESULT+MAXENLEN_DT_INSTANCE_ID+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_FEW_PARAM_DATA	( 1+24*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_CROP_INFO_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_CROP_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_CROP_NUM*MAXENLEN_DT_CROP_INFO_DATA )
#define  MAXENLEN_DT_SCIENCE_TREE_DATA	( 1+15*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_FORMATION_HERO_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_FORMATION_IDX_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_FORMATION_HERO_DATA )
#define  MAXENLEN_DT_FORMATION_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FORMATION_NUM*MAXENLEN_DT_FORMATION_IDX_DATA )
#define  MAXENLEN_DT_FORMATION_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_FORMATION_DATA_LIST )
#define  MAXENLEN_DT_NEW_FORMATION_POSTION_INFO	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_NEW_FORMATION_INFO	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_NEW_FORMATION_POSTION_INFO+sizeof(UINT8) )
#define  MAXENLEN_DT_NEW_FORMATION_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_NEW_FORMATION_NUM*MAXENLEN_DT_NEW_FORMATION_INFO )
#define  MAXENLEN_DT_NEW_FORMATION_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_FORMATION_DATA_LIST+sizeof(UINT8)+MAXENLEN_DT_NEW_FORMATION_DATA_LST+MAXENLEN_DT_NEW_FORMATION_DATA_LST+sizeof(UINT8) )
#define  MAXENLEN_DT_VERSION_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+2+MAX_CONTENT_SIZE*sizeof(UINT16)+2+MAX_CONTENT_SIZE*sizeof(UINT16) )
#define  MAXENLEN_DT_NOTICE_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_CONTENT_SIZE*sizeof(UINT16) )
#define  MAXENLEN_DT_NOTICE_DATA_DB	( 1+7*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_NOTICE_CONTENT_LEN*sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_NOTICE_DATA_NEW	( 1+4*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_NOTICE_CONTENT_LEN*sizeof(UINT16) )
#define  MAXENLEN_DT_EAT_BREAD_DATA	( 1+9*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_YABIAO_BASE_DATA	( 1+18*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_YABIAO_BATTLELOG_DATA	( 1+10*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_YABIAO_BATTLELOG_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_YABIAO_BATTLELOG_NUM*MAXENLEN_DT_YABIAO_BATTLELOG_DATA )
#define  MAXENLEN_DT_SKILL_ACTION_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_SKILL_ACTION_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_SKILL_SLOT_NUM*MAXENLEN_DT_SKILL_ACTION_DATA )
#define  MAXENLEN_DT_JINGJIE_BASE_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_GAS_BALL_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_COLLECT_GAS_DATA	( 1+15*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GAS_BALL_NUM*MAXENLEN_DT_GAS_BALL_DATA+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_JINGJIE_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_JINGJIE_BASE_DATA+MAXENLEN_DT_COLLECT_GAS_DATA+MAXENLEN_DT_SKILL_ACTION_DATA_LIST )
#define  MAXENLEN_DT_CLIMB_TOWER_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_ELITE_INSTANCE_RESET_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_VIP_GET_NUM_EACH_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16) )
#define  MAXENLEN_DT_VIP_GET_NUM_LEVEL_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_VIP_GIFT*MAXENLEN_DT_VIP_GET_NUM_EACH_DATA )
#define  MAXENLEN_DT_VIP_EXT_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+2+MAX_ELITE_INSTANCE_TOWN_NUM*MAXENLEN_DT_ELITE_INSTANCE_RESET_DATA+sizeof(UINT64)+sizeof(UINT8)+2+MAX_VIP_LEVEL*MAXENLEN_DT_VIP_GET_NUM_LEVEL_DATA )
#define  MAXENLEN_DT_LUAACTIVITY_RECORD_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_LUAACTIVITY_RECORD_LIST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_LUAACTIVITY_RECORD_NUM*MAXENLEN_DT_LUAACTIVITY_RECORD_DATA )
#define  MAXENLEN_DT_BOSSB_HURT_RATE	( 1+5*sizeof(SIndex)+sizeof(UINT32)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_BOSS_BATTLE_ENC_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT32)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_BOSS_BATTLE_PLAYER_DATA	( 1+21*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64)+MAXENLEN_DT_BOSSB_HURT_RATE+MAXENLEN_DT_BOSS_BATTLE_ENC_DATA+MAXENLEN_DT_BOSS_BATTLE_ENC_DATA+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64) )
#define  MAXENLEN_DT_BOSS_BATTLE_DATA	( 1+14*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+2+MAX_BOSS_ENC_TOP_NUM*MAXENLEN_DT_BOSSB_HURT_RATE+2+MAX_BOSS_ENC_TOP_NUM*MAXENLEN_DT_BOSS_BATTLE_ENC_DATA+MAXENLEN_DT_BOSS_BATTLE_ENC_DATA+sizeof(UINT64)+sizeof(UINT64) )
#define  MAXENLEN_DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT8)+MAXENLEN_DT_BOSSB_HURT_RATE+MAXENLEN_DT_BOSS_BATTLE_ENC_DATA+MAXENLEN_DT_BOSS_BATTLE_ENC_DATA )
#define  MAXENLEN_DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_BOSSB_BATTLE_LOG_NUM*MAXENLEN_DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA )
#define  MAXENLEN_DT_BOSS_BATTLE_MODULE_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_BOSSB_NUM_PER_DAY*MAXENLEN_DT_BOSS_BATTLE_DATA )
#define  MAXENLEN_DT_ACTIVITY_ENCOURAGE_INFO	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_ACTIVITY_ENCOURAGE_INFO2	( 1+3*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_ACTIVITY_ENCOURAGE_DATA	( 1+8*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16)+2+MAX_ACTIVITY_ENCOURAGE_INFO_NUM*MAXENLEN_DT_ACTIVITY_ENCOURAGE_INFO+sizeof(UINT16)+2+MAX_ACTIVITY_ENCOURAGE_INFO_NUM*MAXENLEN_DT_ACTIVITY_ENCOURAGE_INFO2 )
#define  MAXENLEN_DT_RDC_PLAYER_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT32)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_RANDOM_CHALLENGE_DATA	( 1+10*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_GET_RDC_NUM*MAXENLEN_DT_RDC_PLAYER_DATA+sizeof(UINT8) )
#define  MAXENLEN_DT_WORSHIP_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64) )
#define  MAXENLEN_DT_WORSHIP_DATA_LST	( 1+3*sizeof(SIndex)+sizeof(UINT16)+2+MAX_WORSHIP_NUM*MAXENLEN_DT_WORSHIP_DATA+sizeof(UINT8) )
#define  MAXENLEN_DT_CHAPTER_SCORE_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_ACCUMULATED_SCORE_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_SCORE_DATA_LST	( 1+5*sizeof(SIndex)+sizeof(UINT8)+2+MAX_TOWN_PER_SCENE_NUM*MAXENLEN_DT_CHAPTER_SCORE_DATA+sizeof(UINT8)+2+MAX_TOWN_PER_SCENE_NUM*MAXENLEN_DT_ACCUMULATED_SCORE_DATA+sizeof(UINT8) )
#define  MAXENLEN_DT_EQUIP_LEVEL_COMPOSE_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_EQUIP_COMPOSE_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+2+MAX_EQUIPLEVEL_NUM*MAXENLEN_DT_EQUIP_LEVEL_COMPOSE_DATA+sizeof(UINT64)+sizeof(UINT16) )
#define  MAXENLEN_DT_GATHER_SCIENCE_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_BEGBLUE_RES	( 1+7*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT16)+2+BEGBLUE_LUCK_PROMPT*sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64) )
#define  MAXENLEN_DT_CONSUME_BEGBLUE_SMILE_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+BEGBLUE_CONSUME_TIMES*sizeof(UINT16) )
#define  MAXENLEN_DT_BEGBLUE_DATA	( 1+14*sizeof(SIndex)+sizeof(UINT64)+2+BEGBLUE_CLIMB_TOWER_PROMPT*sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8)+MAXENLEN_DT_BEGBLUE_RES+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+MAXENLEN_DT_CONSUME_BEGBLUE_SMILE_LST )
#define  MAXENLEN_DT_SEND_FLOWER_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_FINALLY_CONTACT_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT64) )
#define  MAXENLEN_DT_FINALLY_CONTACT_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FINALLY_CONTACT_DATA_NUM*MAXENLEN_DT_FINALLY_CONTACT_DATA )
#define  MAXENLEN_DT_RECV_FLOWER_DATA_EX_LOG	( 1+5*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8)+MAXENLEN_DT_RES_NUM_ITEM_DATA )
#define  MAXENLEN_DT_FRIENDS_BASE_DATA	( 1+10*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT8)+2+MAX_SEND_FLOWER_DATA_NUM*MAXENLEN_DT_SEND_FLOWER_DATA+sizeof(UINT8)+MAXENLEN_DT_FINALLY_CONTACT_DATA_LST+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+2+MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM*MAXENLEN_DT_RECV_FLOWER_DATA_EX_LOG )
#define  MAXENLEN_DT_RAREITEM_GOODS_INFO	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT64) )
#define  MAXENLEN_DT_RAREITEM_GOODS_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+2+MAX_RAREITEM_RES_NUM*MAXENLEN_DT_RAREITEM_GOODS_INFO+sizeof(UINT64) )
#define  MAXENLEN_DT_GYMNASIUM_SKILL_INFO	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_GYMNASIUM_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GYMNASIUM_SKILL_NUM*MAXENLEN_DT_GYMNASIUM_SKILL_INFO )
#define  MAXENLEN_DT_HUNTING_INFO	( 1+10*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+2+MONSTER_GROUP_NUM*sizeof(UINT16) )
#define  MAXENLEN_DT_SHADE_DATA	( 1+2*sizeof(SIndex)+MAXENLEN_DT_HUNTING_INFO+sizeof(UINT64) )
#define  MAXENLEN_DT_FACTION_HUNTING_SCHEDULE_DATA	( 1+9*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT64) )
#define  MAXENLEN_DT_HOT_SPRING_TIME_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_SHADE_TASK_OPEN_MARK	( 1+1*sizeof(SIndex)+sizeof(UINT8) )
#define  MAXENLEN_DT_FACTION_ICON_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64) )
#define  MAXENLEN_DT_FACTION_COMMON_DATA	( 1+3*sizeof(SIndex)+MAXENLEN_DT_FACTION_HUNTING_SCHEDULE_DATA+MAXENLEN_DT_HOT_SPRING_TIME_DATA+MAXENLEN_DT_FACTION_ICON_DATA )
#define  MAXENLEN_DT_WINE_DATA_CLI	( 1+14*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_PLAYER_POS_INFO	( 1+4*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT64) )
#define  MAXENLEN_DT_OVER_GIVE_WINE_REWARD	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_HOT_SPRING_DATA	( 1+13*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_WINE_DATA_CLI+MAXENLEN_DT_PLAYER_POS_INFO+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_OVER_GIVE_WINE_REWARD+sizeof(UINT8) )
#define  MAXENLEN_DT_LEAVE_FACTION_MARK	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64) )
#define  MAXENLEN_DT_PLAYER_DONATE_INFO	( 1+5*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16) )
#define  MAXENLEN_DT_PLAYER_MARK_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_LEAVE_FACTION_MARK )
#define  MAXENLEN_DT_PLAYER_FACTION_BATTLELOG_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+2+NORMAL_MSG_LEN*sizeof(UINT16)+sizeof(UINT8)+2+FACTION_LOG_LIST_MAX_NUM*sizeof(UINT8) )
#define  MAXENLEN_DT_PLAYER_FACTION_BATTLELOG_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FACTION_BATTLE_LOG_NUM*MAXENLEN_DT_PLAYER_FACTION_BATTLELOG_DATA )
#define  MAXENLEN_DT_HOT_SPRING_EXT_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_PLAYER_FACTION_DATA	( 1+10*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_RAREITEM_GOODS_DATA+MAXENLEN_DT_GYMNASIUM_DATA+MAXENLEN_DT_SHADE_DATA+MAXENLEN_DT_HOT_SPRING_DATA+MAXENLEN_DT_PLAYER_FACTION_BATTLELOG_DATA_LIST+MAXENLEN_DT_PLAYER_MARK_DATA+MAXENLEN_DT_PLAYER_DONATE_INFO+sizeof(UINT32)+MAXENLEN_DT_HOT_SPRING_EXT_DATA )
#define  MAXENLEN_DT_PLAYER_TODAY_CONSUME_DATA	( 1+49*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT64)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64) )
#define  MAXENLEN_DT_RECV_FLOWER_DATA_LOG	( 1+4*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_RECV_FLOWER_LOG_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT32)+2+MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM*MAXENLEN_DT_RECV_FLOWER_DATA_LOG )
#define  MAXENLEN_DT_RELATION_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_RELATION_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT32)+2+MAX_RELATION_LIST_ELEM_NUM*MAXENLEN_DT_RELATION_DATA )
#define  MAXENLEN_DT_RELATION_DATA_EVEN	( 1+2*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_UNREAD_DATA_EVEN	( 1+2*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_DATA_EVEN_LIST	( 1+4*sizeof(SIndex)+sizeof(UINT32)+2+MAX_RELATION_LIST_ELEM_NUM*MAXENLEN_DT_RELATION_DATA_EVEN+sizeof(UINT32)+2+MAX_RELATION_LIST_ELEM_NUM*MAXENLEN_DT_UNREAD_DATA_EVEN )
#define  MAXENLEN_DT_COURAGE_PLAYER_HEROID_DATA	( 1+10*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT16)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_COURAGE_COMMON_PLAYER_BATTLE_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_COURAGE_MEMBER_NUM*MAXENLEN_DT_COURAGE_PLAYER_HEROID_DATA )
#define  MAXENLEN_DT_COURAGE_ITEM_DATA_CLI	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_COURAGE_ITEM_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_BAG_OPEN_NUM*MAXENLEN_DT_COURAGE_ITEM_DATA_CLI )
#define  MAXENLEN_DT_COURAGE_ENCOURAGE_BOX_GROUP_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8)+MAXENLEN_DT_COURAGE_ITEM_DATA_LIST+sizeof(UINT32) )
#define  MAXENLEN_DT_COURAGE_ENCOURAGE_BOX_DATA2	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVE_ENCOURAGE_BOX_GROUP_NUM*MAXENLEN_DT_COURAGE_ENCOURAGE_BOX_GROUP_DATA )
#define  MAXENLEN_DT_COURAGE_DATA	( 1+18*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_COURAGE_COMMON_PLAYER_BATTLE_DATA+MAXENLEN_DT_INSTANCE_ID+MAXENLEN_DT_BAG_JEWELRY_DATA_LST+sizeof(UINT8) )
#define  MAXENLEN_DT_EQUIP_MASTER_DETAIL	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT32) )
#define  MAXENLEN_DT_MASTER_EXP_BOOK_DETAIL	( 1+1*sizeof(SIndex)+sizeof(UINT8) )
#define  MAXENLEN_DT_EQUIP_MASTER	( 1+5*sizeof(SIndex)+2+EQUIP_KIND_COUNT*MAXENLEN_DT_EQUIP_MASTER_DETAIL+sizeof(UINT8)+2+EQUIP_KIND_COUNT*MAXENLEN_DT_MASTER_EXP_BOOK_DETAIL+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_FRIENDS_DATA	( 1+4*sizeof(SIndex)+MAXENLEN_DT_FRIENDS_BASE_DATA+MAXENLEN_DT_RECV_FLOWER_LOG_LIST+MAXENLEN_DT_RELATION_DATA_LIST+MAXENLEN_DT_DATA_EVEN_LIST )
#define  MAXENLEN_DT_INSTANCE_DB_DATA	( 1+9*sizeof(SIndex)+MAXENLEN_DT_INSTANCE_DATA_LIST+MAXENLEN_DT_INSTANCE_DATA_LIST+MAXENLEN_DT_INSTANCE_DATA_LIST+MAXENLEN_DT_INSTANCE_BATTLE_DATA+MAXENLEN_DT_INSTANCE_BATTLE_DATA+MAXENLEN_DT_INSTANCE_BATTLE_DATA+MAXENLEN_DT_INSTANCE_DATA_LIST+MAXENLEN_DT_INSTANCE_DATA_LIST+MAXENLEN_DT_INSTANCE_BATTLE_DATA )
#define  MAXENLEN_DT_RACE_DATA	( 1+2*sizeof(SIndex)+MAXENLEN_DT_RACE_BASE_DATA+MAXENLEN_DT_RACE_BATTLELOG_DATA_LIST )
#define  MAXENLEN_DT_PLUNDER_DATA	( 1+3*sizeof(SIndex)+MAXENLEN_DT_PLUNDER_BASE_DATA+MAXENLEN_DT_PLUNDER_BATTLELOG_DATA_LIST+MAXENLEN_DT_CAPTURE_DATA_LIST )
#define  MAXENLEN_DT_YABIAO_DATA	( 1+2*sizeof(SIndex)+MAXENLEN_DT_YABIAO_BASE_DATA+MAXENLEN_DT_YABIAO_BATTLELOG_DATA_LIST )
#define  MAXENLEN_DT_BOSS_BATTLE_PLAYER_DATA_INFO	( 1+2*sizeof(SIndex)+MAXENLEN_DT_BOSS_BATTLE_PLAYER_DATA+MAXENLEN_DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST )
#define  MAXENLEN_DT_EXCHANGE_RESULT_LOG	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT32) )
#define  MAXENLEN_DT_GAMBLE_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT32) )
#define  MAXENLEN_DT_GVG_GIFT_REWARD_DATA	( 1+9*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+2+MAX_FACTION_NUM*sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_GVG_GIFT_REWARD_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_GIFT_NUM*MAXENLEN_DT_GVG_GIFT_REWARD_DATA )
#define  MAXENLEN_DT_GVG_GIFT_KEY_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_GIFT_NUM*sizeof(UINT64) )
#define  MAXENLEN_DT_PLAYER_GVG_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GAMBLE_NUM*MAXENLEN_DT_GAMBLE_DATA+MAXENLEN_DT_GVG_GIFT_KEY_LST_DATA+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_GVG_FORMATION_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_FORMATION_PLAYER_CNT*sizeof(UINT32) )
#define  MAXENLEN_DT_FACTION_BASE_DATA_CLI2	( 1+12*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+2+USERNAME_LEN*sizeof(UINT16)+2+MAX_FACTION_NUM*sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64)+2+MAX_FACTION_DESC_NUM*sizeof(UINT16)+2+ZONENAME_LEN*sizeof(UINT16) )
#define  MAXENLEN_DT_GAMBLE_FACTION_INFO	( 1+4*sizeof(SIndex)+MAXENLEN_DT_FACTION_BASE_DATA_CLI2+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_GVG_ROUND	( 1+2*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_FACTION_GVG_SINGLE_LOG	( 1+6*sizeof(SIndex)+MAXENLEN_DT_GVG_ROUND+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT16)+MAXENLEN_DT_GAMBLE_FACTION_INFO+MAXENLEN_DT_GAMBLE_FACTION_INFO )
#define  MAXENLEN_DT_FACTION_SIMPLE_GVG_LOG	( 1+7*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+MAXENLEN_DT_GVG_ROUND+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_FACTION_GVG_LOG_LIST	( 1+3*sizeof(SIndex)+sizeof(UINT16)+2+MAX_GVG_LOG_CNT*MAXENLEN_DT_FACTION_GVG_SINGLE_LOG+sizeof(UINT16) )
#define  MAXENLEN_DT_FACTION_SIMPLE_GVG_LOG_LIST	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT16)+2+MAX_GVG_SIMPLE_LOG_CNT*MAXENLEN_DT_FACTION_SIMPLE_GVG_LOG )
#define  MAXENLEN_DT_FACTION_LIST_SIMPLE_GVG_LOG	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_GVG_ARENA_FACTION_CNT*MAXENLEN_DT_FACTION_SIMPLE_GVG_LOG_LIST )
#define  MAXENLEN_DT_GVG_LOG_FACTION_INFO	( 1+8*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT16)+2+MAX_FACTION_NUM*sizeof(UINT16)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT16) )
#define  MAXENLEN_DT_FACTION_GVG_LOG	( 1+2*sizeof(SIndex)+MAXENLEN_DT_GVG_LOG_FACTION_INFO+MAXENLEN_DT_FACTION_GVG_LOG_LIST )
#define  MAXENLEN_DT_FACTION_GVG_DATA	( 1+4*sizeof(SIndex)+MAXENLEN_DT_FACTION_GVG_LOG_LIST+MAXENLEN_DT_GVG_FORMATION_DATA+sizeof(UINT64)+sizeof(UINT64) )
#define  MAXENLEN_DT_GVG_FORMATION_PLAYER_INFO	( 1+4*sizeof(SIndex)+sizeof(UINT32)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_GVG_FORMATION_PLAYER_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_FORMATION_PLAYER_CNT*MAXENLEN_DT_GVG_FORMATION_PLAYER_INFO )
#define  MAXENLEN_DT_DOORSTRIBUTE_LEVEL_DATA	( 1+6*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT16) )
#define  MAXENLEN_DT_DOORSTRIBUTE_LEVE_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+DT_FACTION_PLAYER_NUM*MAXENLEN_DT_DOORSTRIBUTE_LEVEL_DATA )
#define  MAXENLEN_DT_FACTION_MEMBER_INFO	( 1+7*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT16)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_FACTION_MEMBER_INFO_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+DT_FACTION_PLAYER_NUM*MAXENLEN_DT_FACTION_MEMBER_INFO )
#define  MAXENLEN_DT_GVG_FACTION	( 1+10*sizeof(SIndex)+sizeof(UINT16)+MAXENLEN_DT_FACTION_BASE_DATA_CLI2+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+MAXENLEN_DT_FACTION_MEMBER_INFO_LIST )
#define  MAXENLEN_DT_PK_FACTION	( 1+3*sizeof(SIndex)+MAXENLEN_DT_GVG_FACTION+MAXENLEN_DT_GVG_FORMATION_DATA+MAXENLEN_DT_GVG_FORMATION_PLAYER_LIST )
#define  MAXENLEN_DT_PVP_VIDEO_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT64)+MAXENLEN_DT_BATTLE_DATA3 )
#define  MAXENLEN_DT_GVG_MVP	( 1+6*sizeof(SIndex)+sizeof(UINT8)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_GVG_MVP_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_MVP_CNT*MAXENLEN_DT_GVG_MVP )
#define  MAXENLEN_DT_GVG_VIDEO_FACTION	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT8)+2+MAX_FACTION_NUM*sizeof(UINT16)+MAXENLEN_DT_GVG_FORMATION_PLAYER_LIST )
#define  MAXENLEN_DT_GVG_PK_VIDEO	( 1+5*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_EMPTY_HP_VIDEO )
#define  MAXENLEN_DT_GVG_PK_VIDEO_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_PK_VIDEO_CNT*MAXENLEN_DT_GVG_PK_VIDEO )
#define  MAXENLEN_DT_GVG_VIDEO	( 1+8*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT64)+MAXENLEN_DT_GVG_ROUND+MAXENLEN_DT_GVG_VIDEO_FACTION+MAXENLEN_DT_GVG_VIDEO_FACTION+MAXENLEN_DT_FINAL_ROUND_BATTLE_OBJ_INFO+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_GVG_ZONE_MEMBER_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_ARENA_ZONE_CNT*sizeof(UINT32) )
#define  MAXENLEN_DT_ZONE_FACTION_ID	( 1+2*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_DIFF_ZONE_FACTION_ID_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+GVG_16_TO_8_FACTION_CNT*MAXENLEN_DT_ZONE_FACTION_ID )
#define  MAXENLEN_DT_ONE_ZONE_FACTION_ID_LIST	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+2+GVG_16_TO_8_FACTION_CNT*sizeof(UINT32) )
#define  MAXENLEN_DT_ZONE_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_ARENA_ZONE_CNT*MAXENLEN_DT_ONE_ZONE_FACTION_ID_LIST )
#define  MAXENLEN_DT_ZONE_ID_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_ARENA_ZONE_CNT*sizeof(UINT16) )
#define  MAXENLEN_DT_FACTION_LIST_GVG_LOG	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+GVG_16_TO_8_FACTION_CNT*MAXENLEN_DT_FACTION_GVG_LOG )
#define  MAXENLEN_DT_VIDEO_ID	( 1+1*sizeof(SIndex)+sizeof(UINT64) )
#define  MAXENLEN_DT_FINAL_WAR_EACH_ROUND_FACTION_INFO	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_FINAL_WAR_ROUND_INFO	( 1+3*sizeof(SIndex)+sizeof(UINT8)+2+GVG_FINAL_WAR_FACTION_CNT*sizeof(UINT8)+2+GVG_FINAL_WAR_FACTION_CNT*MAXENLEN_DT_FINAL_WAR_EACH_ROUND_FACTION_INFO )
#define  MAXENLEN_DT_16_TO_8_TO_4_WAR_INFO	( 1+11*sizeof(SIndex)+sizeof(UINT8)+2+GVG_16_TO_8_FACTION_CNT*MAXENLEN_DT_GVG_FACTION+sizeof(UINT8)+2+GVG_16_TO_8_WIN_CNT*sizeof(UINT8)+sizeof(UINT8)+2+GVG_8_TO_4_WIN_CNT*sizeof(UINT8)+2+GVG_16_TO_8_WIN_CNT*sizeof(UINT64)+2+GVG_8_TO_4_WIN_CNT*sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_FINAL_WAR_INFO	( 1+6*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+2+GVG_FINAL_WAR_FACTION_CNT*MAXENLEN_DT_GVG_FACTION+sizeof(UINT8)+2+GVG_FINAL_WAR_FACTION_CNT*sizeof(UINT8)+2+GVG_FINAL_WAR_FACTION_CNT*MAXENLEN_DT_FINAL_WAR_ROUND_INFO )
#define  MAXENLEN_DT_GVG_STAGE_FINISH_FLAG_LIST	( 1+1*sizeof(SIndex)+2+30*sizeof(UINT8) )
#define  MAXENLEN_DT_GROUP_ZONE	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+2*sizeof(UINT16) )
#define  MAXENLEN_DT_GROUP_ZONE_ID_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_GROUP_NUM*MAXENLEN_DT_GROUP_ZONE )
#define  MAXENLEN_DT_WAR_16_TO_8_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_GROUP_NUM*MAXENLEN_DT_16_TO_8_TO_4_WAR_INFO )
#define  MAXENLEN_DT_GVG_ARENA_DATA	( 1+9*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_ZONE_ID_LIST+MAXENLEN_DT_GROUP_ZONE_ID_LIST+MAXENLEN_DT_WAR_16_TO_8_LIST+MAXENLEN_DT_FINAL_WAR_INFO+MAXENLEN_DT_FACTION_LIST_SIMPLE_GVG_LOG+MAXENLEN_DT_GVG_STAGE_FINISH_FLAG_LIST )
#define  MAXENLEN_DT_ZONE_FACTION_MEMBER	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+2+MAX_GVG_TOP_FACTION_CNT*sizeof(UINT32) )
#define  MAXENLEN_DT_GVG_ACTIVITY_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_GVG_LOCAL_ACTIVITY_DATA	( 1+5*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_GVG_ARENA_DATA )
#define  MAXENLEN_DT_TSTRING_MSG	( 1+1*sizeof(SIndex)+2+NORMAL_MSG_LEN*sizeof(UINT16) )
#define  MAXENLEN_DT_BATTLE_LOG_MSG_DATA	( 1+8*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+2+MAX_BATTLE_LOG_PARAM_NUM*sizeof(UINT64)+sizeof(UINT8)+2+MAX_BATTLE_LOG_PARAM_STR*MAXENLEN_DT_TSTRING_MSG )
#define  MAXENLEN_DT_BATTLE_LOG_MSG_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_BATTLE_LOG_NUM*MAXENLEN_DT_BATTLE_LOG_MSG_DATA )
#define  MAXENLEN_DT_SOUL_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_SOUL_BAG_NUM*MAXENLEN_DT_SOUL_DATA )
#define  MAXENLEN_DT_PLAYER_SOUL_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_SOUL_LST_DATA+sizeof(UINT64)+2+MAX_SOUL_PRODUCT_TYPE*sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+MAXENLEN_DT_SOUL_POTENCY_LST_DATA )
#define  MAXENLEN_DT_MEDAL_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT8) )
#define  MAXENLEN_DT_MEDAL_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_MEDAL_NUM*MAXENLEN_DT_MEDAL_DATA )
#define  MAXENLEN_DT_MEDAL_BASE_DATA	( 1+23*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64) )
#define  MAXENLEN_DT_PLAYER_MEDAL_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_MEDAL_LST_DATA+sizeof(UINT64)+MAXENLEN_DT_MEDAL_BASE_DATA )
#define  MAXENLEN_DT_PET_WASHQUALITY_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_PET_DATA	( 1+9*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT32)+2+MAX_PET_NAME_LENGTH*sizeof(UINT16)+2+MAX_PET_FEED_FRUIT_CNT*sizeof(UINT8)+sizeof(UINT8)+2+MAX_PET_WASH_PSYCHIC_CELL_CNT*MAXENLEN_DT_PET_WASHQUALITY_DATA )
#define  MAXENLEN_DT_NEIDAN_DATA	( 1+4*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_PET_NEIDAN_DATA_LST	( 1+1*sizeof(SIndex)+2+MAX_PET_NEIDAN_NUM*MAXENLEN_DT_NEIDAN_DATA )
#define  MAXENLEN_DT_PLAYER_NEIDAN_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_PET_NUM*MAXENLEN_DT_PET_NEIDAN_DATA_LST )
#define  MAXENLEN_DT_NEIDAN_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_NEI_DAN_BAG_NUM*MAXENLEN_DT_NEIDAN_DATA )
#define  MAXENLEN_DT_PET_DATA_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_PET_NUM*MAXENLEN_DT_PET_DATA )
#define  MAXENLEN_DT_PET_CALL_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT32) )
#define  MAXENLEN_DT_PET_CALL_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_PET_NUM*MAXENLEN_DT_PET_CALL_DATA )
#define  MAXENLEN_DT_HERO_HP_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_HERO_HP_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FORMATION_IDX_NUM*MAXENLEN_DT_HERO_HP_DATA )
#define  MAXENLEN_DT_PLAYER_PET_DATA	( 1+17*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_PET_DATA_LIST+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+MAXENLEN_DT_PET_CALL_LST_DATA+sizeof(UINT16)+sizeof(UINT8)+MAXENLEN_DT_PLAYER_NEIDAN_DATA_LST+MAXENLEN_DT_NEIDAN_DATA_LST+MAXENLEN_DT_HERO_HP_DATA_LST )
#define  MAXENLEN_DT_PLAYER_NOPVP_DATA	( 1+49*sizeof(SIndex)+MAXENLEN_DT_PLAYER_BASE_DATA+MAXENLEN_DT_HERO_DATA_LIST+MAXENLEN_DT_BAG_EQUIP_DATA_LST+MAXENLEN_DT_BAG_GOODS_DATA_LST+MAXENLEN_DT_BUILD_DATA_LIST+MAXENLEN_DT_TASK_DATA_LIST+MAXENLEN_DT_LOCALE_DATA+MAXENLEN_DT_STATE_DATA+MAXENLEN_DT_ONLINE_ENCOURAGE_RECORD_DATA+MAXENLEN_DT_LOGIN_ENCOURAGE_RECORD_DATA+MAXENLEN_DT_INSTANCE_DB_DATA+MAXENLEN_DT_MONEY_TREE_DATA+MAXENLEN_DT_ONHOOK_RCD_DATA+MAXENLEN_DT_JINGJIE_DATA+MAXENLEN_DT_PHYSTRENGTH_DATA+MAXENLEN_DT_FEW_PARAM_DATA+MAXENLEN_DT_CROP_DATA+MAXENLEN_DT_SCIENCE_TREE_DATA+MAXENLEN_DT_NEW_FORMATION_DATA+2+MAX_ORDER_ID_LEN*sizeof(UINT8)+MAXENLEN_DT_EAT_BREAD_DATA+MAXENLEN_DT_YABIAO_DATA+2+MAX_TOKEN_NUM*sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT64)+MAXENLEN_DT_CLIMB_TOWER_DATA+MAXENLEN_DT_AWAKEN_DATA+MAXENLEN_DT_VIP_EXT_DATA+MAXENLEN_DT_LUAACTIVITY_RECORD_LIST_DATA+MAXENLEN_DT_COLLECT_GAS_DATA+MAXENLEN_DT_BOSS_BATTLE_PLAYER_DATA_INFO+MAXENLEN_DT_ACTIVITY_ENCOURAGE_DATA+MAXENLEN_DT_RANDOM_CHALLENGE_DATA+MAXENLEN_DT_WORSHIP_DATA_LST+MAXENLEN_DT_SCORE_DATA_LST+MAXENLEN_DT_EQUIP_COMPOSE_DATA+MAXENLEN_DT_GATHER_SCIENCE_DATA+MAXENLEN_DT_BEGBLUE_DATA+MAXENLEN_DT_PLAYER_TODAY_CONSUME_DATA+MAXENLEN_DT_FRIENDS_DATA+MAXENLEN_DT_PLAYER_FACTION_DATA+MAXENLEN_DT_COURAGE_DATA+MAXENLEN_DT_EQUIP_MASTER+MAXENLEN_DT_EXCHANGE_RESULT_LOG+MAXENLEN_DT_BAG_JEWELRY_DATA_LST+MAXENLEN_DT_PLAYER_GVG_DATA+MAXENLEN_DT_PLAYER_SOUL_DATA+MAXENLEN_DT_PLAYER_MEDAL_DATA+MAXENLEN_DT_PLAYER_PET_DATA )
#define  MAXENLEN_DT_PLAYER_PVP_DATA	( 1+3*sizeof(SIndex)+MAXENLEN_DT_RACE_DATA+MAXENLEN_DT_PLUNDER_DATA+MAXENLEN_DT_ENEMY_DATA_LIST )
#define  MAXENLEN_DT_PLAYER_DATA	( 1+2*sizeof(SIndex)+MAXENLEN_DT_PLAYER_NOPVP_DATA+MAXENLEN_DT_PLAYER_PVP_DATA )
#define  MAXENLEN_DT_EXT_FUNC_PARAM	( 1+3*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT32) )
#define  MAXENLEN_DT_EXT_FUNC_PARAM_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_EXT_FUN_NUM*MAXENLEN_DT_EXT_FUNC_PARAM )
#define  MAXENLEN_DT_CONTINUE_KILL_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FACTION_BATTLE_NUM*sizeof(UINT64) )
#define  MAXENLEN_DT_FACTION_PLAYER_DATA	( 1+11*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+MAXENLEN_DT_PLAYER_FACTION_BATTLELOG_DATA_LIST+sizeof(UINT64)+MAXENLEN_DT_CONTINUE_KILL_DATA )
#define  MAXENLEN_DT_FACTION_PLAYER_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT32)+2+DT_FACTION_PLAYER_NUM*MAXENLEN_DT_FACTION_PLAYER_DATA )
#define  MAXENLEN_DT_FACTION_LOG_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+2+FACTION_LOG_LIST_MAX_NUM*sizeof(UINT32)+sizeof(UINT64) )
#define  MAXENLEN_DT_FACTION_LOG_DATA_LST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_FACTION_LOG_NUM*MAXENLEN_DT_FACTION_LOG_DATA )
#define  MAXENLEN_DT_FACTION_BASE_DATA	( 1+16*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+2+MAX_FACTION_NUM*sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT32)+2+MAX_FACTION_DESC_NUM*sizeof(UINT16)+2+MAX_FACTION_DESC_NUM*sizeof(UINT16)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT64) )
#define  MAXENLEN_DT_FACTION_DATA	( 1+9*sizeof(SIndex)+MAXENLEN_DT_FACTION_BASE_DATA+MAXENLEN_DT_FACTION_PLAYER_DATA_LST+MAXENLEN_DT_FACTION_LOG_DATA_LST+MAXENLEN_DT_FACTION_COMMON_DATA+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT64)+sizeof(UINT8)+MAXENLEN_DT_FACTION_GVG_DATA )
#define  MAXENLEN_DT_FACTION_AUTHORITY	( 1+12*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_FACTION_AUTHORITY_EX	( 1+13*sizeof(SIndex)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8) )
#define  MAXENLEN_DT_COURAGE_PLAYER_DATA	( 1+10*sizeof(SIndex)+2+USERNAME_LEN*sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_COURAGE_GROUP_DATA	( 1+9*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+2+MAX_COURAGE_GROUP_PLAYER_NUM*MAXENLEN_DT_COURAGE_PLAYER_DATA+sizeof(UINT32)+sizeof(UINT16)+MAXENLEN_DT_INSTANCE_ID+sizeof(UINT16) )
#define  MAXENLEN_DT_COURAGE_GROUP_DATA_LST	( 1+6*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT64)+sizeof(UINT8)+sizeof(UINT16)+2+MAX_COURAGE_GROUP_NUM*MAXENLEN_DT_COURAGE_GROUP_DATA )
#define  MAXENLEN_DT_FACTION_APPLAY_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_FACTION_APPLAY_LST_DATA	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_FACTION_APPLAY_NUM*MAXENLEN_DT_FACTION_APPLAY_DATA )
#define  MAXENLEN_DT_SIMPLE_FACTION_INFO	( 1+4*sizeof(SIndex)+MAXENLEN_DT_FACTION_BASE_DATA_CLI2+sizeof(UINT32)+sizeof(UINT16)+sizeof(UINT8) )
#define  MAXENLEN_DT_SIMPLE_FACTION_LST_INFO	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_RANK_DISPLAY*MAXENLEN_DT_SIMPLE_FACTION_INFO )
#define  MAXENLEN_DT_GVG_FACTION_RANK_DATA	( 1+7*sizeof(SIndex)+sizeof(UINT16)+MAXENLEN_DT_FACTION_BASE_DATA_CLI2+sizeof(UINT64)+sizeof(UINT32)+sizeof(UINT8)+sizeof(UINT8)+sizeof(UINT16) )
#define  MAXENLEN_DT_GVG_FACTION_RANK_LST_DATA	( 1+3*sizeof(SIndex)+sizeof(UINT16)+2+MAX_RANK_DISPLAY*MAXENLEN_DT_GVG_FACTION_RANK_DATA+MAXENLEN_DT_GVG_FACTION )
#define  MAXENLEN_DT_FACTION_BATTLE_RESULT	( 1+3*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT32)+sizeof(UINT32) )
#define  MAXENLEN_DT_FACTION_BATTLE_RESULT_LST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_FACTION_BATTLE_NUM*MAXENLEN_DT_FACTION_BATTLE_RESULT )
#define  MAXENLEN_DT_LOCAL_WAR_FACTION_SEED	( 1+2*sizeof(SIndex)+sizeof(UINT32)+sizeof(UINT8) )
#define  MAXENLEN_DT_LOCAL_WAR_FACTION_SEED_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT16)+2+MAX_FACTION_APPLAY_NUM*sizeof(UINT32) )
#define  MAXENLEN_DT_CHAOS_WAR	( 1+3*sizeof(SIndex)+sizeof(UINT8)+MAXENLEN_DT_SIMPLE_FACTION_LST_INFO+MAXENLEN_DT_LOCAL_WAR_FACTION_SEED_LIST )
#define  MAXENLEN_DT_GVG_BUILD_DATA	( 1+13*sizeof(SIndex)+MAXENLEN_DT_FACTION_APPLAY_LST_DATA+MAXENLEN_DT_GVG_FACTION_RANK_LST_DATA+MAXENLEN_DT_SIMPLE_FACTION_LST_INFO+MAXENLEN_DT_CHAOS_WAR+MAXENLEN_DT_16_TO_8_TO_4_WAR_INFO+MAXENLEN_DT_WAR_16_TO_8_LIST+MAXENLEN_DT_FINAL_WAR_INFO+MAXENLEN_DT_FACTION_LIST_SIMPLE_GVG_LOG+MAXENLEN_DT_GVG_STAGE_FINISH_FLAG_LIST+sizeof(UINT8)+sizeof(UINT16)+sizeof(UINT16)+sizeof(UINT64) )
#define  MAXENLEN_DT_GVG_FACTION_LIST	( 1+2*sizeof(SIndex)+sizeof(UINT8)+2+MAX_GVG_FACTION_NUM*MAXENLEN_DT_GVG_FACTION )
/*DDefine MaxEnLen MAC End*/


enum EN_ProtoCommonData_MessageID
{
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodeDT_RES_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RES_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RES_DATA( DT_RES_DATA &data );
INT32 EncodeDT_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ITEM_DATA( DT_ITEM_DATA &data );
INT32 EncodeDT_RES_NUM_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RES_NUM_ITEM_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RES_NUM_ITEM_DATA( DT_RES_NUM_ITEM_DATA &data );
INT32 EncodeDT_GUIDE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GUIDE_RECORD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GUIDE_RECORD_DATA( DT_GUIDE_RECORD_DATA &data );
INT32 EncodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_BASE_DATA( DT_PLAYER_BASE_DATA &data );
INT32 EncodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ATTRIBUTE( DT_BATTLE_ATTRIBUTE &data );
INT32 EncodeDT_BATTLE_ATTRIBUTE2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTRIBUTE2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ATTRIBUTE2( DT_BATTLE_ATTRIBUTE2 &data );
INT32 EncodeDT_BATTLE_BASE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_BASE_ATTRIBUTE(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_BASE_ATTRIBUTE( DT_BATTLE_BASE_ATTRIBUTE &data );
INT32 EncodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HIDE_BATTLE_ATTRIBUTE( DT_HIDE_BATTLE_ATTRIBUTE &data );
INT32 EncodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EXT_BATTLE_ATTRIBUTE( DT_EXT_BATTLE_ATTRIBUTE &data );
INT32 EncodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_UPDATE_ATTR_DATA( DT_HERO_UPDATE_ATTR_DATA &data );
INT32 EncodeDT_HERO_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_ATTR_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_ATTR_DATA( DT_HERO_ATTR_DATA &data );
INT32 EncodeDT_SPECIAL_EFFECT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SPECIAL_EFFECT_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SPECIAL_EFFECT_DATA( DT_SPECIAL_EFFECT_DATA &data );
INT32 EncodeDT_SOUL_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_ATTR_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SOUL_ATTR_DATA( DT_SOUL_ATTR_DATA &data );
INT32 EncodeDT_SOUL_POTENCY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_POTENCY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SOUL_POTENCY_DATA( DT_SOUL_POTENCY_DATA &data );
INT32 EncodeDT_SOUL_POTENCY_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_POTENCY_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SOUL_POTENCY_LST_DATA( DT_SOUL_POTENCY_LST_DATA &data );
INT32 EncodeDT_SOUL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SOUL_DATA( DT_SOUL_DATA &data );
INT32 EncodeDT_HERO_SOUL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_SOUL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_SOUL_DATA( DT_HERO_SOUL_DATA &data );
INT32 EncodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_BASE_DATA( DT_HERO_BASE_DATA &data );
INT32 EncodeDT_XIANGQIAN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_XIANGQIAN_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_XIANGQIAN_DATA( DT_XIANGQIAN_DATA &data );
INT32 EncodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EQUIP_DATA( DT_EQUIP_DATA &data );
INT32 EncodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_EQUIP_DATA_LST( DT_HERO_EQUIP_DATA_LST &data );
INT32 EncodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BAG_EQUIP_DATA_LST( DT_BAG_EQUIP_DATA_LST &data );
INT32 EncodeDT_GOODS_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GOODS_DATA( DT_GOODS_DATA &data );
INT32 EncodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BAG_GOODS_DATA_LST( DT_BAG_GOODS_DATA_LST &data );
INT32 EncodeDT_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DRUG_DATA( DT_DRUG_DATA &data );
INT32 EncodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DRUG_DATA_LST( DT_DRUG_DATA_LST &data );
INT32 EncodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GODWEAPON_DATA( DT_GODWEAPON_DATA &data );
INT32 EncodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_RANG_DATA( DT_ATTACK_RANG_DATA &data );
INT32 EncodeDT_SKILL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SKILL_DATA( DT_SKILL_DATA &data );
INT32 EncodeDT_SKILL_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SKILL_DATA_LST( DT_SKILL_DATA_LST &data );
INT32 EncodeDT_STUDY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_STUDY_DATA( DT_STUDY_DATA &data );
INT32 EncodeDT_AWAKEN_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_AWAKEN_BASE_DATA( DT_AWAKEN_BASE_DATA &data );
INT32 EncodeDT_AWAKEN_BASE_DATA1(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_BASE_DATA1(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_AWAKEN_BASE_DATA1( DT_AWAKEN_BASE_DATA1 &data );
INT32 EncodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_AWAKEN_DATA( DT_AWAKEN_DATA &data );
INT32 EncodeDT_AWAKEN_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_AWAKEN_DATA_LST( DT_AWAKEN_DATA_LST &data );
INT32 EncodeDT_ABSORB_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ABSORB_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ABSORB_DATA_LST( DT_ABSORB_DATA_LST &data );
INT32 EncodeDT_ATTR_INC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTR_INC_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTR_INC_DATA( DT_ATTR_INC_DATA &data );
INT32 EncodeDT_BATTLE_ATTR_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTR_LST_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ATTR_LST_INFO( DT_BATTLE_ATTR_LST_INFO &data );
INT32 EncodeDT_BATTLE_ATTR_EXT_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTR_EXT_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ATTR_EXT_INFO( DT_BATTLE_ATTR_EXT_INFO &data );
INT32 EncodeDT_BATTLE_ATTR_EXT_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTR_EXT_LST_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ATTR_EXT_LST_INFO( DT_BATTLE_ATTR_EXT_LST_INFO &data );
INT32 EncodeDT_JEWELRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_JEWELRY_DATA( DT_JEWELRY_DATA &data );
INT32 EncodeDT_JEWELRY_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_JEWELRY_DATA_LST( DT_JEWELRY_DATA_LST &data );
INT32 EncodeDT_BAG_JEWELRY_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_JEWELRY_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BAG_JEWELRY_DATA_LST( DT_BAG_JEWELRY_DATA_LST &data );
INT32 EncodeDT_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_DATA( DT_HERO_DATA &data );
INT32 EncodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_DATA_LIST( DT_HERO_DATA_LIST &data );
INT32 EncodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BUILD_BASE_DATA( DT_BUILD_BASE_DATA &data );
INT32 EncodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BUILD_DATA_LIST( DT_BUILD_DATA_LIST &data );
INT32 EncodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_INSTANCE_ID( DT_INSTANCE_ID &data );
INT32 EncodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_INSTANCE_DATA( DT_INSTANCE_DATA &data );
INT32 EncodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_INSTANCE_DATA_LIST( DT_INSTANCE_DATA_LIST &data );
INT32 EncodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SCIENCE_ATTREXT( DT_SCIENCE_ATTREXT &data );
INT32 EncodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SCIENCE_ATTREXT_LIST( DT_SCIENCE_ATTREXT_LIST &data );
INT32 EncodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CROWN_TASK_CONDITION( DT_CROWN_TASK_CONDITION &data );
INT32 EncodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BUILDUPGRADE_TASK_CONDITION( DT_BUILDUPGRADE_TASK_CONDITION &data );
INT32 EncodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RECRUITHERO_TASK_CONDITION( DT_RECRUITHERO_TASK_CONDITION &data );
INT32 EncodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CROWN_HEROCOLOR_CONDITION( DT_CROWN_HEROCOLOR_CONDITION &data );
INT32 EncodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CROWN_HEROUPGRADE_CONDITION( DT_CROWN_HEROUPGRADE_CONDITION &data );
INT32 EncodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION( DT_FIRSTPASSINSTANCE_TASK_CONDITION &data );
INT32 EncodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SCOREPASSINSTANCE_TASK_CONDITION( DT_SCOREPASSINSTANCE_TASK_CONDITION &data );
INT32 EncodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_TIMESPASSINSTANCE_TASK_CONDITION( DT_TIMESPASSINSTANCE_TASK_CONDITION &data );
INT32 EncodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION( DT_KILLMONSTERINSTANCE_TASK_CONDITION &data );
INT32 EncodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION( DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION &data );
INT32 EncodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION( DT_STRENGTHENEQUIPNUM_TASK_CONDITION &data );
INT32 EncodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ITEMCOLLECT_TASK_CONDITION( DT_ITEMCOLLECT_TASK_CONDITION &data );
INT32 EncodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION( DT_ACTIVATEGODWEAPON_TASK_CONDITION &data );
INT32 EncodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_UPGRADEGODWEAPON_TASK_CONDITION( DT_UPGRADEGODWEAPON_TASK_CONDITION &data );
INT32 EncodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_UPGRADEDRUG_TASK_CONDITION( DT_UPGRADEDRUG_TASK_CONDITION &data );
INT32 EncodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CHALLENGE_TASK_CONDITION( DT_CHALLENGE_TASK_CONDITION &data );
INT32 EncodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLUNDER_TASK_CONDITION( DT_PLUNDER_TASK_CONDITION &data );
INT32 EncodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DRESSSKILL_TASK_CONDITION( DT_DRESSSKILL_TASK_CONDITION &data );
INT32 EncodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DRESSEQUIP_TASK_CONDITION( DT_DRESSEQUIP_TASK_CONDITION &data );
INT32 EncodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_UPGRADEEQUIP_TASK_CONDITION( DT_UPGRADEEQUIP_TASK_CONDITION &data );
INT32 EncodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION( DT_USEEXPRIENCEDRUG_TASK_CONDITION &data );
INT32 EncodeDT_MELT_EQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MELT_EQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MELT_EQUIP_TASK_CONDITION( DT_MELT_EQUIP_TASK_CONDITION &data );
INT32 EncodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COMPOSEGOOD_TASK_CONDITION( DT_COMPOSEGOOD_TASK_CONDITION &data );
INT32 EncodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ELITEINSTANCE_TASK_CONDITION( DT_ELITEINSTANCE_TASK_CONDITION &data );
INT32 EncodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_UPGRADSCIENCE_TASK_CONDITION( DT_UPGRADSCIENCE_TASK_CONDITION &data );
INT32 EncodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_STRENGTHATTACK_TASK_CONDITION( DT_STRENGTHATTACK_TASK_CONDITION &data );
INT32 EncodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_STRENGTHLIFE_TASK_CONDITION( DT_STRENGTHLIFE_TASK_CONDITION &data );
INT32 EncodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CROP_TASK_CONDITION( DT_CROP_TASK_CONDITION &data );
INT32 EncodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_UPGRADESKILL_TASK_CONDITION( DT_UPGRADESKILL_TASK_CONDITION &data );
INT32 EncodeDT_UPGRADEJINGJIE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEJINGJIE_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_UPGRADEJINGJIE_TASK_CONDITION( DT_UPGRADEJINGJIE_TASK_CONDITION &data );
INT32 EncodeDT_MOSAICGEM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MOSAICGEM_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MOSAICGEM_TASK_CONDITION( DT_MOSAICGEM_TASK_CONDITION &data );
INT32 EncodeDT_CLIMBTOWER_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIMBTOWER_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CLIMBTOWER_TASK_CONDITION( DT_CLIMBTOWER_TASK_CONDITION &data );
INT32 EncodeDT_STUDY_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_STUDY_TASK_CONDITION( DT_STUDY_TASK_CONDITION &data );
INT32 EncodeDT_RECRUITHERO_NUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUITHERO_NUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RECRUITHERO_NUM_TASK_CONDITION( DT_RECRUITHERO_NUM_TASK_CONDITION &data );
INT32 EncodeDT_OPENLIGHT_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OPENLIGHT_TASK_CONDITION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_OPENLIGHT_TASK_CONDITION( DT_OPENLIGHT_TASK_CONDITION &data );
INT32 EncodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_TASK_DATA( DT_TASK_DATA &data );
INT32 EncodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_TASK_DATA_LIST( DT_TASK_DATA_LIST &data );
INT32 EncodeDT_RACE_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RACE_BASE_DATA( DT_RACE_BASE_DATA &data );
INT32 EncodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOCALE_HOME( DT_LOCALE_HOME &data );
INT32 EncodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOCALE_INSTANCE( DT_LOCALE_INSTANCE &data );
INT32 EncodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOCALE_ONHOOK( DT_LOCALE_ONHOOK &data );
INT32 EncodeDT_LOCALE_MARK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_MARK(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOCALE_MARK( DT_LOCALE_MARK &data );
INT32 EncodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOCALE_DATA( DT_LOCALE_DATA &data );
INT32 EncodeUnlockScienceInfo(void *pHost, CNetData* poNetData);
INT32 DecodeUnlockScienceInfo(void *pHost, CNetData* poNetData);
cJSON* GetJsonUnlockScienceInfo( UnlockScienceInfo &data );
INT32 EncodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_UPGRADE_DATA( DT_HERO_UPGRADE_DATA &data );
INT32 EncodeDT_STATE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STATE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_STATE_DATA( DT_STATE_DATA &data );
INT32 EncodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MONSTER_BASE_DATA( DT_MONSTER_BASE_DATA &data );
INT32 EncodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MONSTER_DATA_LIST( DT_MONSTER_DATA_LIST &data );
INT32 EncodeDT_GODWEAPON_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_BATTLE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GODWEAPON_BATTLE_DATA( DT_GODWEAPON_BATTLE_DATA &data );
INT32 EncodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_OBJ_DATA( DT_BATTLE_OBJ_DATA &data );
INT32 EncodeDT_BATTLE_OBJ_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_OBJ_DATA2( DT_BATTLE_OBJ_DATA2 &data );
INT32 EncodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_OBJ_GROUP_DATA( DT_BATTLE_OBJ_GROUP_DATA &data );
INT32 EncodeDT_BATTLE_OBJ_GROUP_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_GROUP_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_OBJ_GROUP_DATA2( DT_BATTLE_OBJ_GROUP_DATA2 &data );
INT32 EncodeDT_BUFF_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUFF_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BUFF_DATA( DT_BUFF_DATA &data );
INT32 EncodeDT_HP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HP_DATA( DT_HP_DATA &data );
INT32 EncodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ANGRY_DATA( DT_ANGRY_DATA &data );
INT32 EncodeDT_PARRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PARRY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PARRY_DATA( DT_PARRY_DATA &data );
INT32 EncodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SKILL_EFFECT_ID_DATA( DT_SKILL_EFFECT_ID_DATA &data );
INT32 EncodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_OBJ_DATA( DT_ATTACK_OBJ_DATA &data );
INT32 EncodeDT_ATTACK_OBJ_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_OBJ_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_OBJ_DATA2( DT_ATTACK_OBJ_DATA2 &data );
INT32 EncodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_DATA( DT_ATTACK_DATA &data );
INT32 EncodeDT_ATTACK_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_DATA2( DT_ATTACK_DATA2 &data );
INT32 EncodeDT_GODWEAPON_ATTACK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_ATTACK_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GODWEAPON_ATTACK_DATA( DT_GODWEAPON_ATTACK_DATA &data );
INT32 EncodeDT_GODWEAPON_ATTACK_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_ATTACK_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GODWEAPON_ATTACK_DATA2( DT_GODWEAPON_ATTACK_DATA2 &data );
INT32 EncodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_ALL_DATA( DT_ATTACK_ALL_DATA &data );
INT32 EncodeDT_ATTACK_ALL_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_ALL_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_ALL_DATA2( DT_ATTACK_ALL_DATA2 &data );
INT32 EncodeDT_ATTACK_ALL_DATA3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_ALL_DATA3(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_ALL_DATA3( DT_ATTACK_ALL_DATA3 &data );
INT32 EncodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ANGRY_DATA( DT_BATTLE_ANGRY_DATA &data );
INT32 EncodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ROUND_DATA( DT_BATTLE_ROUND_DATA &data );
INT32 EncodeDT_BATTLE_ROUND_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ROUND_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ROUND_DATA2( DT_BATTLE_ROUND_DATA2 &data );
INT32 EncodeDT_BATTLE_ROUND_DATA3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ROUND_DATA3(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ROUND_DATA3( DT_BATTLE_ROUND_DATA3 &data );
INT32 EncodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_DATA( DT_BATTLE_DATA &data );
INT32 EncodeDT_BATTLE_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_DATA2( DT_BATTLE_DATA2 &data );
INT32 EncodeDT_BATTLE_DATA3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_DATA3(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_DATA3( DT_BATTLE_DATA3 &data );
INT32 EncodeDT_SHIELD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SHIELD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SHIELD_DATA( DT_SHIELD_DATA &data );
INT32 EncodeDT_HP_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HP_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HP_DATA2( DT_HP_DATA2 &data );
INT32 EncodeDT_ATTACK_OBJ_DATA3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_OBJ_DATA3(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_OBJ_DATA3( DT_ATTACK_OBJ_DATA3 &data );
INT32 EncodeDT_ATTACK_DATA3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_DATA3(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_DATA3( DT_ATTACK_DATA3 &data );
INT32 EncodeDT_PARRY_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PARRY_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PARRY_DATA2( DT_PARRY_DATA2 &data );
INT32 EncodeDT_ATTACK_ALL_DATA4(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_ALL_DATA4(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_ALL_DATA4( DT_ATTACK_ALL_DATA4 &data );
INT32 EncodeDT_BATTLE_ROUND_DATA4(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ROUND_DATA4(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ROUND_DATA4( DT_BATTLE_ROUND_DATA4 &data );
INT32 EncodeDT_BATTLE_DATA4(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_DATA4(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_DATA4( DT_BATTLE_DATA4 &data );
INT32 EncodeDT_ATTACK_ALL_DATA5(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_ALL_DATA5(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ATTACK_ALL_DATA5( DT_ATTACK_ALL_DATA5 &data );
INT32 EncodeDT_BATTLE_ROUND_DATA5(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ROUND_DATA5(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_ROUND_DATA5( DT_BATTLE_ROUND_DATA5 &data );
INT32 EncodeDT_PET_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_BATTLE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PET_BATTLE_DATA( DT_PET_BATTLE_DATA &data );
INT32 EncodeDT_BATTLE_OBJ_GROUP_DATA3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_GROUP_DATA3(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_OBJ_GROUP_DATA3( DT_BATTLE_OBJ_GROUP_DATA3 &data );
INT32 EncodeDT_BATTLE_DATA5(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_DATA5(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_DATA5( DT_BATTLE_DATA5 &data );
INT32 EncodeDT_BATTLE_OBJ_MAX_HP(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_MAX_HP(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_OBJ_MAX_HP( DT_BATTLE_OBJ_MAX_HP &data );
INT32 EncodeDT_BATTLE_OBJ_MAX_HP_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_MAX_HP_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_OBJ_MAX_HP_INFO( DT_BATTLE_OBJ_MAX_HP_INFO &data );
INT32 EncodeDT_EMPTY_HP_VIDEO_v400(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EMPTY_HP_VIDEO_v400(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EMPTY_HP_VIDEO_v400( DT_EMPTY_HP_VIDEO_v400 &data );
INT32 EncodeDT_EMPTY_HP_VIDEO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EMPTY_HP_VIDEO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EMPTY_HP_VIDEO( DT_EMPTY_HP_VIDEO &data );
INT32 EncodeDT_FINAL_ROUND_BATTLE_OBJ_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FINAL_ROUND_BATTLE_OBJ_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FINAL_ROUND_BATTLE_OBJ_INFO( DT_FINAL_ROUND_BATTLE_OBJ_INFO &data );
INT32 EncodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ONLINE_ENCOURAGE_RECORD_DATA( DT_ONLINE_ENCOURAGE_RECORD_DATA &data );
INT32 EncodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOGIN_ENCOURAGE_RECORD_DATA( DT_LOGIN_ENCOURAGE_RECORD_DATA &data );
INT32 EncodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RACE_BATTLELOG_DATA( DT_RACE_BATTLELOG_DATA &data );
INT32 EncodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RACE_BATTLELOG_DATA_LIST( DT_RACE_BATTLELOG_DATA_LIST &data );
INT32 EncodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CAPTURE_DATA( DT_CAPTURE_DATA &data );
INT32 EncodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CAPTURE_DATA_LIST( DT_CAPTURE_DATA_LIST &data );
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLUNDER_BATTLELOG_DATA( DT_PLUNDER_BATTLELOG_DATA &data );
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLUNDER_BATTLELOG_DATA_LIST( DT_PLUNDER_BATTLELOG_DATA_LIST &data );
INT32 EncodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLUNDER_GRID_DATA( DT_PLUNDER_GRID_DATA &data );
INT32 EncodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLUNDER_BASE_DATA( DT_PLUNDER_BASE_DATA &data );
INT32 EncodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ENEMY_DATA( DT_ENEMY_DATA &data );
INT32 EncodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ENEMY_DATA_LIST( DT_ENEMY_DATA_LIST &data );
INT32 EncodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MONEY_TREE_DATA( DT_MONEY_TREE_DATA &data );
INT32 EncodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_INSTANCE_BATTLE_DATA( DT_INSTANCE_BATTLE_DATA &data );
INT32 EncodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PHYSTRENGTH_DATA( DT_PHYSTRENGTH_DATA &data );
INT32 EncodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ONHOOK_RESULT( DT_ONHOOK_RESULT &data );
INT32 EncodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ONHOOK_RCD_DATA( DT_ONHOOK_RCD_DATA &data );
INT32 EncodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FEW_PARAM_DATA( DT_FEW_PARAM_DATA &data );
INT32 EncodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CROP_INFO_DATA( DT_CROP_INFO_DATA &data );
INT32 EncodeDT_CROP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CROP_DATA( DT_CROP_DATA &data );
INT32 EncodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SCIENCE_TREE_DATA( DT_SCIENCE_TREE_DATA &data );
INT32 EncodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FORMATION_HERO_DATA( DT_FORMATION_HERO_DATA &data );
INT32 EncodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FORMATION_IDX_DATA( DT_FORMATION_IDX_DATA &data );
INT32 EncodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FORMATION_DATA_LIST( DT_FORMATION_DATA_LIST &data );
INT32 EncodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FORMATION_DATA( DT_FORMATION_DATA &data );
INT32 EncodeDT_NEW_FORMATION_POSTION_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_FORMATION_POSTION_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NEW_FORMATION_POSTION_INFO( DT_NEW_FORMATION_POSTION_INFO &data );
INT32 EncodeDT_NEW_FORMATION_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_FORMATION_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NEW_FORMATION_INFO( DT_NEW_FORMATION_INFO &data );
INT32 EncodeDT_NEW_FORMATION_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_FORMATION_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NEW_FORMATION_DATA_LST( DT_NEW_FORMATION_DATA_LST &data );
INT32 EncodeDT_NEW_FORMATION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_FORMATION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NEW_FORMATION_DATA( DT_NEW_FORMATION_DATA &data );
INT32 EncodeDT_VERSION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VERSION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_VERSION_DATA( DT_VERSION_DATA &data );
INT32 EncodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NOTICE_DATA( DT_NOTICE_DATA &data );
INT32 EncodeDT_NOTICE_DATA_DB(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NOTICE_DATA_DB(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NOTICE_DATA_DB( DT_NOTICE_DATA_DB &data );
INT32 EncodeDT_NOTICE_DATA_NEW(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NOTICE_DATA_NEW(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NOTICE_DATA_NEW( DT_NOTICE_DATA_NEW &data );
INT32 EncodeDT_EAT_BREAD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EAT_BREAD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EAT_BREAD_DATA( DT_EAT_BREAD_DATA &data );
INT32 EncodeDT_YABIAO_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_YABIAO_BASE_DATA( DT_YABIAO_BASE_DATA &data );
INT32 EncodeDT_YABIAO_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_YABIAO_BATTLELOG_DATA( DT_YABIAO_BATTLELOG_DATA &data );
INT32 EncodeDT_YABIAO_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_YABIAO_BATTLELOG_DATA_LIST( DT_YABIAO_BATTLELOG_DATA_LIST &data );
INT32 EncodeDT_SKILL_ACTION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_ACTION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SKILL_ACTION_DATA( DT_SKILL_ACTION_DATA &data );
INT32 EncodeDT_SKILL_ACTION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_ACTION_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SKILL_ACTION_DATA_LIST( DT_SKILL_ACTION_DATA_LIST &data );
INT32 EncodeDT_JINGJIE_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JINGJIE_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_JINGJIE_BASE_DATA( DT_JINGJIE_BASE_DATA &data );
INT32 EncodeDT_GAS_BALL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GAS_BALL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GAS_BALL_DATA( DT_GAS_BALL_DATA &data );
INT32 EncodeDT_COLLECT_GAS_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COLLECT_GAS_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COLLECT_GAS_DATA( DT_COLLECT_GAS_DATA &data );
INT32 EncodeDT_JINGJIE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JINGJIE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_JINGJIE_DATA( DT_JINGJIE_DATA &data );
INT32 EncodeDT_CLIMB_TOWER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIMB_TOWER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CLIMB_TOWER_DATA( DT_CLIMB_TOWER_DATA &data );
INT32 EncodeDT_ELITE_INSTANCE_RESET_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITE_INSTANCE_RESET_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ELITE_INSTANCE_RESET_DATA( DT_ELITE_INSTANCE_RESET_DATA &data );
INT32 EncodeDT_VIP_GET_NUM_EACH_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_GET_NUM_EACH_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_VIP_GET_NUM_EACH_DATA( DT_VIP_GET_NUM_EACH_DATA &data );
INT32 EncodeDT_VIP_GET_NUM_LEVEL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_GET_NUM_LEVEL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_VIP_GET_NUM_LEVEL_DATA( DT_VIP_GET_NUM_LEVEL_DATA &data );
INT32 EncodeDT_VIP_EXT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_EXT_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_VIP_EXT_DATA( DT_VIP_EXT_DATA &data );
INT32 EncodeDT_LUAACTIVITY_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LUAACTIVITY_RECORD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LUAACTIVITY_RECORD_DATA( DT_LUAACTIVITY_RECORD_DATA &data );
INT32 EncodeDT_LUAACTIVITY_RECORD_LIST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LUAACTIVITY_RECORD_LIST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LUAACTIVITY_RECORD_LIST_DATA( DT_LUAACTIVITY_RECORD_LIST_DATA &data );
INT32 EncodeDT_BOSSB_HURT_RATE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_HURT_RATE(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSSB_HURT_RATE( DT_BOSSB_HURT_RATE &data );
INT32 EncodeDT_BOSS_BATTLE_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_ENC_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSS_BATTLE_ENC_DATA( DT_BOSS_BATTLE_ENC_DATA &data );
INT32 EncodeDT_BOSS_BATTLE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSS_BATTLE_PLAYER_DATA( DT_BOSS_BATTLE_PLAYER_DATA &data );
INT32 EncodeDT_BOSS_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSS_BATTLE_DATA( DT_BOSS_BATTLE_DATA &data );
INT32 EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA( DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA &data );
INT32 EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST( DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST &data );
INT32 EncodeDT_BOSS_BATTLE_MODULE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_MODULE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSS_BATTLE_MODULE_DATA( DT_BOSS_BATTLE_MODULE_DATA &data );
INT32 EncodeDT_ACTIVITY_ENCOURAGE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_ENCOURAGE_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ACTIVITY_ENCOURAGE_INFO( DT_ACTIVITY_ENCOURAGE_INFO &data );
INT32 EncodeDT_ACTIVITY_ENCOURAGE_INFO2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_ENCOURAGE_INFO2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ACTIVITY_ENCOURAGE_INFO2( DT_ACTIVITY_ENCOURAGE_INFO2 &data );
INT32 EncodeDT_ACTIVITY_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ACTIVITY_ENCOURAGE_DATA( DT_ACTIVITY_ENCOURAGE_DATA &data );
INT32 EncodeDT_RDC_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RDC_PLAYER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RDC_PLAYER_DATA( DT_RDC_PLAYER_DATA &data );
INT32 EncodeDT_RANDOM_CHALLENGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RANDOM_CHALLENGE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RANDOM_CHALLENGE_DATA( DT_RANDOM_CHALLENGE_DATA &data );
INT32 EncodeDT_WORSHIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WORSHIP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_WORSHIP_DATA( DT_WORSHIP_DATA &data );
INT32 EncodeDT_WORSHIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WORSHIP_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_WORSHIP_DATA_LST( DT_WORSHIP_DATA_LST &data );
INT32 EncodeDT_CHAPTER_SCORE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHAPTER_SCORE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CHAPTER_SCORE_DATA( DT_CHAPTER_SCORE_DATA &data );
INT32 EncodeDT_ACCUMULATED_SCORE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCUMULATED_SCORE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ACCUMULATED_SCORE_DATA( DT_ACCUMULATED_SCORE_DATA &data );
INT32 EncodeDT_SCORE_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCORE_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SCORE_DATA_LST( DT_SCORE_DATA_LST &data );
INT32 EncodeDT_EQUIP_LEVEL_COMPOSE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_LEVEL_COMPOSE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EQUIP_LEVEL_COMPOSE_DATA( DT_EQUIP_LEVEL_COMPOSE_DATA &data );
INT32 EncodeDT_EQUIP_COMPOSE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_COMPOSE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EQUIP_COMPOSE_DATA( DT_EQUIP_COMPOSE_DATA &data );
INT32 EncodeDT_GATHER_SCIENCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GATHER_SCIENCE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GATHER_SCIENCE_DATA( DT_GATHER_SCIENCE_DATA &data );
INT32 EncodeDT_BEGBLUE_RES(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_RES(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BEGBLUE_RES( DT_BEGBLUE_RES &data );
INT32 EncodeDT_CONSUME_BEGBLUE_SMILE_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CONSUME_BEGBLUE_SMILE_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CONSUME_BEGBLUE_SMILE_LST( DT_CONSUME_BEGBLUE_SMILE_LST &data );
INT32 EncodeDT_BEGBLUE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BEGBLUE_DATA( DT_BEGBLUE_DATA &data );
INT32 EncodeDT_SEND_FLOWER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SEND_FLOWER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SEND_FLOWER_DATA( DT_SEND_FLOWER_DATA &data );
INT32 EncodeDT_FINALLY_CONTACT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FINALLY_CONTACT_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FINALLY_CONTACT_DATA( DT_FINALLY_CONTACT_DATA &data );
INT32 EncodeDT_FINALLY_CONTACT_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FINALLY_CONTACT_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FINALLY_CONTACT_DATA_LST( DT_FINALLY_CONTACT_DATA_LST &data );
INT32 EncodeDT_RECV_FLOWER_DATA_EX_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECV_FLOWER_DATA_EX_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RECV_FLOWER_DATA_EX_LOG( DT_RECV_FLOWER_DATA_EX_LOG &data );
INT32 EncodeDT_FRIENDS_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIENDS_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FRIENDS_BASE_DATA( DT_FRIENDS_BASE_DATA &data );
INT32 EncodeDT_RAREITEM_GOODS_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RAREITEM_GOODS_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RAREITEM_GOODS_INFO( DT_RAREITEM_GOODS_INFO &data );
INT32 EncodeDT_RAREITEM_GOODS_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RAREITEM_GOODS_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RAREITEM_GOODS_DATA( DT_RAREITEM_GOODS_DATA &data );
INT32 EncodeDT_GYMNASIUM_SKILL_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GYMNASIUM_SKILL_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GYMNASIUM_SKILL_INFO( DT_GYMNASIUM_SKILL_INFO &data );
INT32 EncodeDT_GYMNASIUM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GYMNASIUM_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GYMNASIUM_DATA( DT_GYMNASIUM_DATA &data );
INT32 EncodeDT_HUNTING_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HUNTING_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HUNTING_INFO( DT_HUNTING_INFO &data );
INT32 EncodeDT_SHADE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SHADE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SHADE_DATA( DT_SHADE_DATA &data );
INT32 EncodeDT_FACTION_HUNTING_SCHEDULE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_HUNTING_SCHEDULE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_HUNTING_SCHEDULE_DATA( DT_FACTION_HUNTING_SCHEDULE_DATA &data );
INT32 EncodeDT_HOT_SPRING_TIME_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOT_SPRING_TIME_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HOT_SPRING_TIME_DATA( DT_HOT_SPRING_TIME_DATA &data );
INT32 EncodeDT_SHADE_TASK_OPEN_MARK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SHADE_TASK_OPEN_MARK(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SHADE_TASK_OPEN_MARK( DT_SHADE_TASK_OPEN_MARK &data );
INT32 EncodeDT_FACTION_ICON_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_ICON_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_ICON_DATA( DT_FACTION_ICON_DATA &data );
INT32 EncodeDT_FACTION_COMMON_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_COMMON_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_COMMON_DATA( DT_FACTION_COMMON_DATA &data );
INT32 EncodeDT_WINE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WINE_DATA_CLI(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_WINE_DATA_CLI( DT_WINE_DATA_CLI &data );
INT32 EncodeDT_PLAYER_POS_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_POS_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_POS_INFO( DT_PLAYER_POS_INFO &data );
INT32 EncodeDT_OVER_GIVE_WINE_REWARD(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OVER_GIVE_WINE_REWARD(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_OVER_GIVE_WINE_REWARD( DT_OVER_GIVE_WINE_REWARD &data );
INT32 EncodeDT_HOT_SPRING_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOT_SPRING_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HOT_SPRING_DATA( DT_HOT_SPRING_DATA &data );
INT32 EncodeDT_LEAVE_FACTION_MARK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LEAVE_FACTION_MARK(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LEAVE_FACTION_MARK( DT_LEAVE_FACTION_MARK &data );
INT32 EncodeDT_PLAYER_DONATE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_DONATE_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_DONATE_INFO( DT_PLAYER_DONATE_INFO &data );
INT32 EncodeDT_PLAYER_MARK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_MARK_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_MARK_DATA( DT_PLAYER_MARK_DATA &data );
INT32 EncodeDT_PLAYER_FACTION_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_FACTION_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_FACTION_BATTLELOG_DATA( DT_PLAYER_FACTION_BATTLELOG_DATA &data );
INT32 EncodeDT_PLAYER_FACTION_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_FACTION_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_FACTION_BATTLELOG_DATA_LIST( DT_PLAYER_FACTION_BATTLELOG_DATA_LIST &data );
INT32 EncodeDT_HOT_SPRING_EXT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOT_SPRING_EXT_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HOT_SPRING_EXT_DATA( DT_HOT_SPRING_EXT_DATA &data );
INT32 EncodeDT_PLAYER_FACTION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_FACTION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_FACTION_DATA( DT_PLAYER_FACTION_DATA &data );
INT32 EncodeDT_PLAYER_TODAY_CONSUME_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_TODAY_CONSUME_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_TODAY_CONSUME_DATA( DT_PLAYER_TODAY_CONSUME_DATA &data );
INT32 EncodeDT_RECV_FLOWER_DATA_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECV_FLOWER_DATA_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RECV_FLOWER_DATA_LOG( DT_RECV_FLOWER_DATA_LOG &data );
INT32 EncodeDT_RECV_FLOWER_LOG_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECV_FLOWER_LOG_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RECV_FLOWER_LOG_LIST( DT_RECV_FLOWER_LOG_LIST &data );
INT32 EncodeDT_RELATION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RELATION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RELATION_DATA( DT_RELATION_DATA &data );
INT32 EncodeDT_RELATION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RELATION_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RELATION_DATA_LIST( DT_RELATION_DATA_LIST &data );
INT32 EncodeDT_RELATION_DATA_EVEN(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RELATION_DATA_EVEN(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RELATION_DATA_EVEN( DT_RELATION_DATA_EVEN &data );
INT32 EncodeDT_UNREAD_DATA_EVEN(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UNREAD_DATA_EVEN(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_UNREAD_DATA_EVEN( DT_UNREAD_DATA_EVEN &data );
INT32 EncodeDT_DATA_EVEN_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_EVEN_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DATA_EVEN_LIST( DT_DATA_EVEN_LIST &data );
INT32 EncodeDT_COURAGE_PLAYER_HEROID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYER_HEROID_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_PLAYER_HEROID_DATA( DT_COURAGE_PLAYER_HEROID_DATA &data );
INT32 EncodeDT_COURAGE_COMMON_PLAYER_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_COMMON_PLAYER_BATTLE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_COMMON_PLAYER_BATTLE_DATA( DT_COURAGE_COMMON_PLAYER_BATTLE_DATA &data );
INT32 EncodeDT_COURAGE_ITEM_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_ITEM_DATA_CLI(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_ITEM_DATA_CLI( DT_COURAGE_ITEM_DATA_CLI &data );
INT32 EncodeDT_COURAGE_ITEM_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_ITEM_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_ITEM_DATA_LIST( DT_COURAGE_ITEM_DATA_LIST &data );
INT32 EncodeDT_COURAGE_ENCOURAGE_BOX_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_ENCOURAGE_BOX_GROUP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_ENCOURAGE_BOX_GROUP_DATA( DT_COURAGE_ENCOURAGE_BOX_GROUP_DATA &data );
INT32 EncodeDT_COURAGE_ENCOURAGE_BOX_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_ENCOURAGE_BOX_DATA2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_ENCOURAGE_BOX_DATA2( DT_COURAGE_ENCOURAGE_BOX_DATA2 &data );
INT32 EncodeDT_COURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_DATA( DT_COURAGE_DATA &data );
INT32 EncodeDT_EQUIP_MASTER_DETAIL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_MASTER_DETAIL(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EQUIP_MASTER_DETAIL( DT_EQUIP_MASTER_DETAIL &data );
INT32 EncodeDT_MASTER_EXP_BOOK_DETAIL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MASTER_EXP_BOOK_DETAIL(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MASTER_EXP_BOOK_DETAIL( DT_MASTER_EXP_BOOK_DETAIL &data );
INT32 EncodeDT_EQUIP_MASTER(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_MASTER(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EQUIP_MASTER( DT_EQUIP_MASTER &data );
INT32 EncodeDT_FRIENDS_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIENDS_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FRIENDS_DATA( DT_FRIENDS_DATA &data );
INT32 EncodeDT_INSTANCE_DB_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DB_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_INSTANCE_DB_DATA( DT_INSTANCE_DB_DATA &data );
INT32 EncodeDT_RACE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_RACE_DATA( DT_RACE_DATA &data );
INT32 EncodeDT_PLUNDER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLUNDER_DATA( DT_PLUNDER_DATA &data );
INT32 EncodeDT_YABIAO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_YABIAO_DATA( DT_YABIAO_DATA &data );
INT32 EncodeDT_BOSS_BATTLE_PLAYER_DATA_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_DATA_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BOSS_BATTLE_PLAYER_DATA_INFO( DT_BOSS_BATTLE_PLAYER_DATA_INFO &data );
INT32 EncodeDT_EXCHANGE_RESULT_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXCHANGE_RESULT_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EXCHANGE_RESULT_LOG( DT_EXCHANGE_RESULT_LOG &data );
INT32 EncodeDT_GAMBLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GAMBLE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GAMBLE_DATA( DT_GAMBLE_DATA &data );
INT32 EncodeDT_GVG_GIFT_REWARD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_GIFT_REWARD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_GIFT_REWARD_DATA( DT_GVG_GIFT_REWARD_DATA &data );
INT32 EncodeDT_GVG_GIFT_REWARD_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_GIFT_REWARD_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_GIFT_REWARD_LST_DATA( DT_GVG_GIFT_REWARD_LST_DATA &data );
INT32 EncodeDT_GVG_GIFT_KEY_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_GIFT_KEY_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_GIFT_KEY_LST_DATA( DT_GVG_GIFT_KEY_LST_DATA &data );
INT32 EncodeDT_PLAYER_GVG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_GVG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_GVG_DATA( DT_PLAYER_GVG_DATA &data );
INT32 EncodeDT_GVG_FORMATION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_FORMATION_DATA( DT_GVG_FORMATION_DATA &data );
INT32 EncodeDT_FACTION_BASE_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_DATA_CLI2(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_BASE_DATA_CLI2( DT_FACTION_BASE_DATA_CLI2 &data );
INT32 EncodeDT_GAMBLE_FACTION_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GAMBLE_FACTION_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GAMBLE_FACTION_INFO( DT_GAMBLE_FACTION_INFO &data );
INT32 EncodeDT_GVG_ROUND(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_ROUND(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_ROUND( DT_GVG_ROUND &data );
INT32 EncodeDT_FACTION_GVG_SINGLE_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_GVG_SINGLE_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_GVG_SINGLE_LOG( DT_FACTION_GVG_SINGLE_LOG &data );
INT32 EncodeDT_FACTION_SIMPLE_GVG_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_SIMPLE_GVG_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_SIMPLE_GVG_LOG( DT_FACTION_SIMPLE_GVG_LOG &data );
INT32 EncodeDT_FACTION_GVG_LOG_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_GVG_LOG_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_GVG_LOG_LIST( DT_FACTION_GVG_LOG_LIST &data );
INT32 EncodeDT_FACTION_SIMPLE_GVG_LOG_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_SIMPLE_GVG_LOG_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_SIMPLE_GVG_LOG_LIST( DT_FACTION_SIMPLE_GVG_LOG_LIST &data );
INT32 EncodeDT_FACTION_LIST_SIMPLE_GVG_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LIST_SIMPLE_GVG_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_LIST_SIMPLE_GVG_LOG( DT_FACTION_LIST_SIMPLE_GVG_LOG &data );
INT32 EncodeDT_GVG_LOG_FACTION_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_LOG_FACTION_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_LOG_FACTION_INFO( DT_GVG_LOG_FACTION_INFO &data );
INT32 EncodeDT_FACTION_GVG_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_GVG_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_GVG_LOG( DT_FACTION_GVG_LOG &data );
INT32 EncodeDT_FACTION_GVG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_GVG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_GVG_DATA( DT_FACTION_GVG_DATA &data );
INT32 EncodeDT_GVG_FORMATION_PLAYER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_PLAYER_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_FORMATION_PLAYER_INFO( DT_GVG_FORMATION_PLAYER_INFO &data );
INT32 EncodeDT_GVG_FORMATION_PLAYER_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_PLAYER_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_FORMATION_PLAYER_LIST( DT_GVG_FORMATION_PLAYER_LIST &data );
INT32 EncodeDT_DOORSTRIBUTE_LEVEL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DOORSTRIBUTE_LEVEL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DOORSTRIBUTE_LEVEL_DATA( DT_DOORSTRIBUTE_LEVEL_DATA &data );
INT32 EncodeDT_DOORSTRIBUTE_LEVE_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DOORSTRIBUTE_LEVE_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DOORSTRIBUTE_LEVE_DATA_LST( DT_DOORSTRIBUTE_LEVE_DATA_LST &data );
INT32 EncodeDT_FACTION_MEMBER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_MEMBER_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_MEMBER_INFO( DT_FACTION_MEMBER_INFO &data );
INT32 EncodeDT_FACTION_MEMBER_INFO_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_MEMBER_INFO_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_MEMBER_INFO_LIST( DT_FACTION_MEMBER_INFO_LIST &data );
INT32 EncodeDT_GVG_FACTION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FACTION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_FACTION( DT_GVG_FACTION &data );
INT32 EncodeDT_PK_FACTION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PK_FACTION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PK_FACTION( DT_PK_FACTION &data );
INT32 EncodeDT_PVP_VIDEO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PVP_VIDEO_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PVP_VIDEO_DATA( DT_PVP_VIDEO_DATA &data );
INT32 EncodeDT_GVG_MVP(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_MVP(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_MVP( DT_GVG_MVP &data );
INT32 EncodeDT_GVG_MVP_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_MVP_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_MVP_LIST( DT_GVG_MVP_LIST &data );
INT32 EncodeDT_GVG_VIDEO_FACTION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_VIDEO_FACTION(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_VIDEO_FACTION( DT_GVG_VIDEO_FACTION &data );
INT32 EncodeDT_GVG_PK_VIDEO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_PK_VIDEO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_PK_VIDEO( DT_GVG_PK_VIDEO &data );
INT32 EncodeDT_GVG_PK_VIDEO_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_PK_VIDEO_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_PK_VIDEO_LIST( DT_GVG_PK_VIDEO_LIST &data );
INT32 EncodeDT_GVG_VIDEO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_VIDEO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_VIDEO( DT_GVG_VIDEO &data );
INT32 EncodeDT_GVG_ZONE_MEMBER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_ZONE_MEMBER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_ZONE_MEMBER_DATA( DT_GVG_ZONE_MEMBER_DATA &data );
INT32 EncodeDT_ZONE_FACTION_ID(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ZONE_FACTION_ID(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ZONE_FACTION_ID( DT_ZONE_FACTION_ID &data );
INT32 EncodeDT_DIFF_ZONE_FACTION_ID_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DIFF_ZONE_FACTION_ID_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_DIFF_ZONE_FACTION_ID_LIST( DT_DIFF_ZONE_FACTION_ID_LIST &data );
INT32 EncodeDT_ONE_ZONE_FACTION_ID_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONE_ZONE_FACTION_ID_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ONE_ZONE_FACTION_ID_LIST( DT_ONE_ZONE_FACTION_ID_LIST &data );
INT32 EncodeDT_ZONE_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ZONE_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ZONE_LIST( DT_ZONE_LIST &data );
INT32 EncodeDT_ZONE_ID_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ZONE_ID_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ZONE_ID_LIST( DT_ZONE_ID_LIST &data );
INT32 EncodeDT_FACTION_LIST_GVG_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LIST_GVG_LOG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_LIST_GVG_LOG( DT_FACTION_LIST_GVG_LOG &data );
INT32 EncodeDT_VIDEO_ID(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIDEO_ID(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_VIDEO_ID( DT_VIDEO_ID &data );
INT32 EncodeDT_FINAL_WAR_EACH_ROUND_FACTION_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FINAL_WAR_EACH_ROUND_FACTION_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FINAL_WAR_EACH_ROUND_FACTION_INFO( DT_FINAL_WAR_EACH_ROUND_FACTION_INFO &data );
INT32 EncodeDT_FINAL_WAR_ROUND_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FINAL_WAR_ROUND_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FINAL_WAR_ROUND_INFO( DT_FINAL_WAR_ROUND_INFO &data );
INT32 EncodeDT_16_TO_8_TO_4_WAR_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_16_TO_8_TO_4_WAR_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_16_TO_8_TO_4_WAR_INFO( DT_16_TO_8_TO_4_WAR_INFO &data );
INT32 EncodeDT_FINAL_WAR_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FINAL_WAR_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FINAL_WAR_INFO( DT_FINAL_WAR_INFO &data );
INT32 EncodeDT_GVG_STAGE_FINISH_FLAG_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_STAGE_FINISH_FLAG_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_STAGE_FINISH_FLAG_LIST( DT_GVG_STAGE_FINISH_FLAG_LIST &data );
INT32 EncodeDT_GROUP_ZONE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GROUP_ZONE(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GROUP_ZONE( DT_GROUP_ZONE &data );
INT32 EncodeDT_GROUP_ZONE_ID_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GROUP_ZONE_ID_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GROUP_ZONE_ID_LIST( DT_GROUP_ZONE_ID_LIST &data );
INT32 EncodeDT_WAR_16_TO_8_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WAR_16_TO_8_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_WAR_16_TO_8_LIST( DT_WAR_16_TO_8_LIST &data );
INT32 EncodeDT_GVG_ARENA_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_ARENA_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_ARENA_DATA( DT_GVG_ARENA_DATA &data );
INT32 EncodeDT_ZONE_FACTION_MEMBER(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ZONE_FACTION_MEMBER(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_ZONE_FACTION_MEMBER( DT_ZONE_FACTION_MEMBER &data );
INT32 EncodeDT_GVG_ACTIVITY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_ACTIVITY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_ACTIVITY_DATA( DT_GVG_ACTIVITY_DATA &data );
INT32 EncodeDT_GVG_LOCAL_ACTIVITY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_LOCAL_ACTIVITY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_LOCAL_ACTIVITY_DATA( DT_GVG_LOCAL_ACTIVITY_DATA &data );
INT32 EncodeDT_TSTRING_MSG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TSTRING_MSG(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_TSTRING_MSG( DT_TSTRING_MSG &data );
INT32 EncodeDT_BATTLE_LOG_MSG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_LOG_MSG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_LOG_MSG_DATA( DT_BATTLE_LOG_MSG_DATA &data );
INT32 EncodeDT_BATTLE_LOG_MSG_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_LOG_MSG_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_BATTLE_LOG_MSG_LST_DATA( DT_BATTLE_LOG_MSG_LST_DATA &data );
INT32 EncodeDT_SOUL_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SOUL_LST_DATA( DT_SOUL_LST_DATA &data );
INT32 EncodeDT_PLAYER_SOUL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_SOUL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_SOUL_DATA( DT_PLAYER_SOUL_DATA &data );
INT32 EncodeDT_MEDAL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MEDAL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MEDAL_DATA( DT_MEDAL_DATA &data );
INT32 EncodeDT_MEDAL_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MEDAL_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MEDAL_LST_DATA( DT_MEDAL_LST_DATA &data );
INT32 EncodeDT_MEDAL_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MEDAL_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_MEDAL_BASE_DATA( DT_MEDAL_BASE_DATA &data );
INT32 EncodeDT_PLAYER_MEDAL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_MEDAL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_MEDAL_DATA( DT_PLAYER_MEDAL_DATA &data );
INT32 EncodeDT_PET_WASHQUALITY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_WASHQUALITY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PET_WASHQUALITY_DATA( DT_PET_WASHQUALITY_DATA &data );
INT32 EncodeDT_PET_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PET_DATA( DT_PET_DATA &data );
INT32 EncodeDT_NEIDAN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEIDAN_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NEIDAN_DATA( DT_NEIDAN_DATA &data );
INT32 EncodeDT_PET_NEIDAN_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_NEIDAN_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PET_NEIDAN_DATA_LST( DT_PET_NEIDAN_DATA_LST &data );
INT32 EncodeDT_PLAYER_NEIDAN_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_NEIDAN_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_NEIDAN_DATA_LST( DT_PLAYER_NEIDAN_DATA_LST &data );
INT32 EncodeDT_NEIDAN_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEIDAN_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_NEIDAN_DATA_LST( DT_NEIDAN_DATA_LST &data );
INT32 EncodeDT_PET_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_DATA_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PET_DATA_LIST( DT_PET_DATA_LIST &data );
INT32 EncodeDT_PET_CALL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_CALL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PET_CALL_DATA( DT_PET_CALL_DATA &data );
INT32 EncodeDT_PET_CALL_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_CALL_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PET_CALL_LST_DATA( DT_PET_CALL_LST_DATA &data );
INT32 EncodeDT_HERO_HP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_HP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_HP_DATA( DT_HERO_HP_DATA &data );
INT32 EncodeDT_HERO_HP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_HP_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_HERO_HP_DATA_LST( DT_HERO_HP_DATA_LST &data );
INT32 EncodeDT_PLAYER_PET_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_PET_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_PET_DATA( DT_PLAYER_PET_DATA &data );
INT32 EncodeDT_PLAYER_NOPVP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_NOPVP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_NOPVP_DATA( DT_PLAYER_NOPVP_DATA &data );
INT32 EncodeDT_PLAYER_PVP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_PVP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_PVP_DATA( DT_PLAYER_PVP_DATA &data );
INT32 EncodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_PLAYER_DATA( DT_PLAYER_DATA &data );
INT32 EncodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EXT_FUNC_PARAM( DT_EXT_FUNC_PARAM &data );
INT32 EncodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_EXT_FUNC_PARAM_LST( DT_EXT_FUNC_PARAM_LST &data );
INT32 EncodeDT_CONTINUE_KILL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CONTINUE_KILL_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CONTINUE_KILL_DATA( DT_CONTINUE_KILL_DATA &data );
INT32 EncodeDT_FACTION_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_PLAYER_DATA( DT_FACTION_PLAYER_DATA &data );
INT32 EncodeDT_FACTION_PLAYER_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_PLAYER_DATA_LST( DT_FACTION_PLAYER_DATA_LST &data );
INT32 EncodeDT_FACTION_LOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LOG_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_LOG_DATA( DT_FACTION_LOG_DATA &data );
INT32 EncodeDT_FACTION_LOG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LOG_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_LOG_DATA_LST( DT_FACTION_LOG_DATA_LST &data );
INT32 EncodeDT_FACTION_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_BASE_DATA( DT_FACTION_BASE_DATA &data );
INT32 EncodeDT_FACTION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_DATA( DT_FACTION_DATA &data );
INT32 EncodeDT_FACTION_AUTHORITY(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_AUTHORITY(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_AUTHORITY( DT_FACTION_AUTHORITY &data );
INT32 EncodeDT_FACTION_AUTHORITY_EX(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_AUTHORITY_EX(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_AUTHORITY_EX( DT_FACTION_AUTHORITY_EX &data );
INT32 EncodeDT_COURAGE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYER_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_PLAYER_DATA( DT_COURAGE_PLAYER_DATA &data );
INT32 EncodeDT_COURAGE_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_GROUP_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_GROUP_DATA( DT_COURAGE_GROUP_DATA &data );
INT32 EncodeDT_COURAGE_GROUP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_GROUP_DATA_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_COURAGE_GROUP_DATA_LST( DT_COURAGE_GROUP_DATA_LST &data );
INT32 EncodeDT_FACTION_APPLAY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_APPLAY_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_APPLAY_DATA( DT_FACTION_APPLAY_DATA &data );
INT32 EncodeDT_FACTION_APPLAY_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_APPLAY_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_APPLAY_LST_DATA( DT_FACTION_APPLAY_LST_DATA &data );
INT32 EncodeDT_SIMPLE_FACTION_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SIMPLE_FACTION_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SIMPLE_FACTION_INFO( DT_SIMPLE_FACTION_INFO &data );
INT32 EncodeDT_SIMPLE_FACTION_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SIMPLE_FACTION_LST_INFO(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_SIMPLE_FACTION_LST_INFO( DT_SIMPLE_FACTION_LST_INFO &data );
INT32 EncodeDT_GVG_FACTION_RANK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FACTION_RANK_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_FACTION_RANK_DATA( DT_GVG_FACTION_RANK_DATA &data );
INT32 EncodeDT_GVG_FACTION_RANK_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FACTION_RANK_LST_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_FACTION_RANK_LST_DATA( DT_GVG_FACTION_RANK_LST_DATA &data );
INT32 EncodeDT_FACTION_BATTLE_RESULT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BATTLE_RESULT(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_BATTLE_RESULT( DT_FACTION_BATTLE_RESULT &data );
INT32 EncodeDT_FACTION_BATTLE_RESULT_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BATTLE_RESULT_LST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_FACTION_BATTLE_RESULT_LST( DT_FACTION_BATTLE_RESULT_LST &data );
INT32 EncodeDT_LOCAL_WAR_FACTION_SEED(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCAL_WAR_FACTION_SEED(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOCAL_WAR_FACTION_SEED( DT_LOCAL_WAR_FACTION_SEED &data );
INT32 EncodeDT_LOCAL_WAR_FACTION_SEED_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCAL_WAR_FACTION_SEED_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_LOCAL_WAR_FACTION_SEED_LIST( DT_LOCAL_WAR_FACTION_SEED_LIST &data );
INT32 EncodeDT_CHAOS_WAR(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHAOS_WAR(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_CHAOS_WAR( DT_CHAOS_WAR &data );
INT32 EncodeDT_GVG_BUILD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_BUILD_DATA(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_BUILD_DATA( DT_GVG_BUILD_DATA &data );
INT32 EncodeDT_GVG_FACTION_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FACTION_LIST(void *pHost, CNetData* poNetData);
cJSON* GetJsonDT_GVG_FACTION_LIST( DT_GVG_FACTION_LIST &data );
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoCommonData)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoCommonData)(void *pHost, CNetData* poNetData);

class CProtoCommonData : public CSDProtocol
{
public:
    static CProtoCommonData * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoCommonData FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoCommonData FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoCommonData();
	virtual ~CProtoCommonData();
	EnFuncCProtoCommonData m_EncodeFuncArray[PROTOCOMMONDATA_MSGID_RANGE];
	DeFuncCProtoCommonData m_DecodeFuncArray[PROTOCOMMONDATA_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
