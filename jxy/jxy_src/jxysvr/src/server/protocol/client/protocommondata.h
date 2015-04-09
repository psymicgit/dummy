/*       Version Number: 10          */

#ifndef PROTOCOMMONDATA_H_INCLUDE_VERSION_10
#define PROTOCOMMONDATA_H_INCLUDE_VERSION_10

#include "netdata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define SUCCESS                  0
#define FAIL                     1
#define REFUSE_LOGIN             2
#define SESSIONID_LEN            8
#define DEVICEID_LEN             41
#define NOTIFYID_LEN             65
#define USERNAME_LEN             41
#define USERTYPE_LEN             41
#define USERPWD_LEN              17
#define MSG_LEN                  1024
#define IPSTR_LEN                17
#define BEGBLUE_CLIMB_TOWER_PROMPT 61
#define BEGBLUE_LUCK_PROMPT      21
#define LOGIN_TYPE_USER          0
#define LOGIN_TYPE_IPHONE        1
#define MAX_HERO_NUM             50
#define NEED                     1
#define NONNEED                  0
#define MAX_BAG_OPEN_NUM         200
#define CURRENCY_TYPE_COIN       0
#define CURRENCY_TYPE_GOLD       1
#define MAX_SKILL_NUM            12
#define MAX_GAS_BALL_NUM         8
#define MAX_SKILL_SLOT_NUM       3
#define MAX_SKILL_BUFF_NUM       3
#define MAX_EFFECT_RECOMMEND_NUM 2
#define MAX_BAG_NUM              5
#define MAX_BUILD_NUM            50
#define MAX_HANG_NUM             50
#define MAX_BATTLE_ROUND_NUM     60
#define MAX_ENCOURAGE_ITEM_NUM   10
#define MAX_INSTANCE_MONSTER_NUM 10
#define PICKUP_KIND_ITEM         0
#define PICKUP_KIND_HANG         1
#define MAX_SCENCE_NUM           2
#define MAX_TOWN_PER_SCENE_NUM   200
#define MAX_INSTANCE_PER_TOWN_NUM 19
#define MAX_INSTANCE_PER_SCENE_NUM 300
#define INSTANCE_INDEX_STR_LEN   15
#define MAX_HP_CHANGE_TIMES_PER_ROUND 30
#define MAX_ANGRY_CHANGE_TIMES_PER_ROUND 10
#define MAX_BUFF_CHANGE_TIMES_PER_ROUND 10
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
#define MAX_UNLOCK_HERO_NUM      3
#define EPL_HOME                 0
#define EPL_INSTANCE             1
#define EPL_ONHOOK               2
#define EPL_PAGODA               3
#define EPL_EATBREAD             4
#define EPL_BOSSB                5
#define EIK_EQUIP                0
#define EIK_GOODS                1
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
#define DT_FACTION_PLAYER_NUM    255
#define MAX_FACTION_LOG_NUM      255
#define FOR_NEW_BEGBLUE          1
#define FOR_TO_COMFIRM_BEGBLUE   2
#define MAX_MOBILE_LENGTH        20
#define PROTOCOMMONDATA_MSGID_RANGE 1
#define PROTOCOMMONDATA_MSGID_OFFSET 0
#define PROTOCOL_VERSION 10
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagDT_GUIDE_RECORD_DATA{
	UINT8            abyGuideRecordData[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
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
}DT_PLAYER_BASE_DATA;

typedef struct tagDT_BATTLE_ATTRIBUTE{
	UINT32           dwHP; //血量 
	UINT32           dwAttack; //攻击力 
}DT_BATTLE_ATTRIBUTE;

typedef struct tagDT_HIDE_BATTLE_ATTRIBUTE{
	UINT16           wHitRate; //命中率 
	UINT16           wDodgeRate; //闪避率
	UINT16           wCritRate; //暴击率
	UINT16           wDeCritRate; //抗暴率 
	UINT16           wAngryValue; //怒气值 
}DT_HIDE_BATTLE_ATTRIBUTE;

typedef struct tagDT_EXT_BATTLE_ATTRIBUTE{
	UINT16           wIncHurtRate; //增伤率(增加的伤害百分比)
	UINT16           wDecHurtRate; //减伤率(减少的伤害百分比)
	UINT16           wOddAngry; //怒气剩余值(技能释放后剩余的怒气值)
	UINT32           dwFirstAttack; //先攻
	UINT16           wCoachTalent; //主角资质
}DT_EXT_BATTLE_ATTRIBUTE;

typedef struct tagDT_HERO_UPDATE_ATTR_DATA{
	UINT16           wAttrHP; //血量 
	UINT16           wAttrAttack; //攻击
}DT_HERO_UPDATE_ATTR_DATA;

typedef struct tagDT_HERO_ATTR_DATA{
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //战斗属性 
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttr; //隐藏战斗属性 
	DT_HERO_UPDATE_ATTR_DATA stUpdateAttrInfo; //属性升级信息 
}DT_HERO_ATTR_DATA;

typedef struct tagDT_HERO_BASE_DATA{
	UINT16           wKindID; //武将类型
	UINT8            byCoach; //主武将标志,1为主武将
	UINT8            byPos; //位置标志,0携带，1为出战，2在招募所中 
	UINT8            byFormationIdx; //阵型位置
	UINT16           wLevel; //等级
	UINT16           wTalent; //资质
	UINT32           dwExperience; //经验
	UINT8            byRecuitedFlag; //是否招募过 
	UINT8            byVisitNum; //拜访次数
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //战斗属性 
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttr; //隐藏战斗属性 
	UINT8            byLastUseDrugLevel; //上次使用的丹药等级(品)
	UINT64           qwLastUseDrugTime; //上次使用丹药时间
	UINT8            byUseExperienceDrugPerDay; //每天服用升级丹个数 
	UINT64           qwLastUseExperienceDrugTime; //上次使用经验丹时间 
	DT_HERO_UPDATE_ATTR_DATA stUpdateAttrInfo; //属性升级信息 
}DT_HERO_BASE_DATA;

typedef struct tagDT_XIANGQIAN_DATA{
	UINT16           wItemID; //物品ID
}DT_XIANGQIAN_DATA;

typedef struct tagDT_EQUIP_DATA{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备 
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT16           wLevel; //装备等级
	UINT16           wBagPos; //所在背包位置 
	UINT64           qwItemNewTime; //道具生产时间
	DT_XIANGQIAN_DATA astXiangqianList[MAX_XIANGQIAN_NUM]; //镶嵌列表
}DT_EQUIP_DATA;

typedef struct tagDT_HERO_EQUIP_DATA_LST{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA    astEquipList[EQUIP_POS_NUM]; //装备信息
}DT_HERO_EQUIP_DATA_LST;

typedef struct tagDT_BAG_EQUIP_DATA_LST{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA    astEquipList[MAX_BAG_OPEN_NUM]; //装备信息
}DT_BAG_EQUIP_DATA_LST;

typedef struct tagDT_GOODS_DATA{
	UINT16           wKindID; //物品类型
	UINT16           wPileCount; //叠加数 
	UINT64           qwLastUseTime; //上次使用时间
	UINT16           wLastUseNum; //上次使用次数
	UINT16           wBagPos; //所在背包位置 
	UINT64           qwItemNewTime; //道具生产时间
}DT_GOODS_DATA;

typedef struct tagDT_BAG_GOODS_DATA_LST{
	UINT16           wGoodsNum; //装备个数
	DT_GOODS_DATA    astGoodsList[MAX_BAG_OPEN_NUM]; //装备信息
}DT_BAG_GOODS_DATA_LST;

typedef struct tagDT_DRUG_DATA{
	UINT8            byLevel; //丹药等级
	UINT8            byNum; //丹药个数
	UINT16           wHeroKindID; //使用的武将ID，必须存在 
}DT_DRUG_DATA;

typedef struct tagDT_DRUG_DATA_LST{
	UINT8            byDrugLevelNum; //丹药级别个数
	DT_DRUG_DATA     astDrugList[MAX_DRUG_LEVEL]; //丹药信息
}DT_DRUG_DATA_LST;

typedef struct tagDT_GODWEAPON_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT16           wQuality; //品质
	UINT16           wLevel; //等级
	UINT16           wHeroKindID; //拥有的武将类型ID
}DT_GODWEAPON_DATA;

typedef struct tagDT_ATTACK_RANG_DATA{
	UINT8            byAttackRangKind; //攻击范围类型
	UINT8            byTraceBuffKind; //追踪的buff类型(只对AttackRangKind为EARK_BUFF有效)
}DT_ATTACK_RANG_DATA;

typedef struct tagDT_SKILL_DATA{
	UINT16           wActionID; //使用招式
	UINT16           wActionLevel; //使用招式等级
	UINT16           wEffectID; //使用效果
	DT_ATTACK_RANG_DATA stAttackRangInfo; //攻击范围
	UINT32           dwActionExp; //招式经验
}DT_SKILL_DATA;

typedef struct tagDT_SKILL_DATA_LST{
	UINT8            bySkillNum; //技能个数 
	DT_SKILL_DATA    astSkillInfo[MAX_SKILL_SLOT_NUM]; //技能信息 
}DT_SKILL_DATA_LST;

typedef struct tagDT_STUDY_DATA{
	UINT8            byStudyColorKind; //心得颜色，从1开始，0表示没有数据
	UINT8            byStudyAttrKind1; //心得类型(对应属性类型,0表示只能转换经验)
	UINT8            byStudyLevel; //心得等级
	UINT8            byStudyAttrKind2; //心得类型(对应属性类型)
}DT_STUDY_DATA;

typedef struct tagDT_AWAKEN_BASE_DATA{
	UINT8            byLevel; //悟道等级（0则没有数据）
	UINT8            byGoldFlag; //是否元宝悟道标志（0不是，1是）
	UINT64           qwStudyExp; //心得经验
	UINT16           wHaveCallNum; //已召唤次数 
	UINT64           qwlastCallTime; //上次召唤时间
	UINT64           qwStudyClip; //心得碎片
}DT_AWAKEN_BASE_DATA;

typedef struct tagDT_AWAKEN_BASE_DATA1{
	UINT8            byLevel; //悟道等级（0则没有数据）
	UINT8            byGoldFlag; //是否元宝悟道标志（0不是，1是）
	UINT64           qwStudyExp; //心得经验
	UINT16           wHaveCallNum; //已召唤次数 
	UINT64           qwlastCallTime; //上次召唤时间
}DT_AWAKEN_BASE_DATA1;

typedef struct tagDT_AWAKEN_DATA{
	DT_AWAKEN_BASE_DATA stBaseInfo; //悟道基本信息
	DT_STUDY_DATA    astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
}DT_AWAKEN_DATA;

typedef struct tagDT_AWAKEN_DATA_LST{
	DT_STUDY_DATA    astStudyInfo[MAX_HERO_STUDY_GRID_NUM]; //引导信息
}DT_AWAKEN_DATA_LST;

typedef struct tagDT_ABSORB_DATA_LST{
	UINT8            byAbsorbNum; //当前吸收次数
	UINT16           awKindIDLst[MAX_ABSORB_NUM]; //次数 武将信息
	UINT16           wAbsorbKindID; //吸收过当前武将的武将ID
}DT_ABSORB_DATA_LST;

typedef struct tagDT_HERO_DATA{
	DT_HERO_BASE_DATA stHeroBaseData; //武将基本信息
	DT_HERO_EQUIP_DATA_LST stEquipDataInfo; //武将装备信息
	DT_DRUG_DATA_LST stDrugDataInfo; //丹药信息
	DT_GODWEAPON_DATA stGodweaponInfo; //神器信息
	DT_SKILL_DATA_LST stSkillInfo; //技能信息 
	DT_AWAKEN_DATA_LST stStudyInfo; //心得信息
	DT_ABSORB_DATA_LST stAbsorbInfo; //吸收信息
}DT_HERO_DATA;

typedef struct tagDT_HERO_DATA_LIST{
	UINT16           wHeroNum; //武将数目
	DT_HERO_DATA     astHeroInfoLst[MAX_HERO_NUM]; //武将信息
}DT_HERO_DATA_LIST;

typedef struct tagDT_BUILD_BASE_DATA{
	UINT8            byKindID; //建筑类型
	UINT16           wLevel; //等级
	UINT64           qwValue1; //对于主城为收获时间 
	UINT64           qwValue2; //对于主城为征收时间 
	UINT8            byValue3; //对于主城为征收的次数
	UINT8            bySort; //建筑排序
	UINT8            byHaveOpenFlag; //建筑是否打开过，1为打开过 
}DT_BUILD_BASE_DATA;

typedef struct tagDT_BUILD_DATA_LIST{
	UINT16           wBuildNum; //建筑数目
	DT_BUILD_BASE_DATA astBuildInfoLst[MAX_BUILD_NUM]; //建筑信息
}DT_BUILD_DATA_LIST;

typedef struct tagDT_INSTANCE_ID{
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
	UINT8            byInstanceIdx; //副本序号
}DT_INSTANCE_ID;

typedef struct tagDT_INSTANCE_DATA{
	DT_INSTANCE_ID   stInstanceID; //副本序号
	UINT64           qwLastPassTime; //上次副本通关时间
	UINT8            byScore; //副本通关得分
}DT_INSTANCE_DATA;

typedef struct tagDT_INSTANCE_DATA_LIST{
	UINT16           wSceneIdx; //场景序号
	UINT16           wInstanceNum; //副本数目
	DT_INSTANCE_DATA astInstanceInfo[MAX_INSTANCE_PER_SCENE_NUM]; //副本信息
}DT_INSTANCE_DATA_LIST;

typedef struct tagDT_SCIENCE_ATTREXT{
	UINT8            byAttrKindID; //属性类型 
	UINT16           wAttrLevel; //科技属性等级 
}DT_SCIENCE_ATTREXT;

typedef struct tagDT_SCIENCE_ATTREXT_LIST{
	UINT8            byAttrNum; //科技馆属性数目 
	DT_SCIENCE_ATTREXT astBuildInfoLst[MAX_SCIENCE_ATTR_NUM]; //科技馆属性信息 
}DT_SCIENCE_ATTREXT_LIST;

typedef struct tagDT_CROWN_TASK_CONDITION{
	UINT16           wCrownTimes; //
}DT_CROWN_TASK_CONDITION;

typedef struct tagDT_BUILDUPGRADE_TASK_CONDITION{
	UINT8            byBuildKindID; //
	UINT16           wLevel; //
}DT_BUILDUPGRADE_TASK_CONDITION;

typedef struct tagDT_RECRUITHERO_TASK_CONDITION{
	UINT16           wRecruitHeroKindID1; //
	UINT16           wRecruitHeroKindID2; //
}DT_RECRUITHERO_TASK_CONDITION;

typedef struct tagDT_CROWN_HEROCOLOR_CONDITION{
	UINT16           wHeroColor; //
	UINT8            byNum; //
}DT_CROWN_HEROCOLOR_CONDITION;

typedef struct tagDT_CROWN_HEROUPGRADE_CONDITION{
	UINT16           wHeroLevel; //
	UINT8            byNum; //
}DT_CROWN_HEROUPGRADE_CONDITION;

typedef struct tagDT_FIRSTPASSINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
}DT_FIRSTPASSINSTANCE_TASK_CONDITION;

typedef struct tagDT_SCOREPASSINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byScore; //
}DT_SCOREPASSINSTANCE_TASK_CONDITION;

typedef struct tagDT_TIMESPASSINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byTimes; //
}DT_TIMESPASSINSTANCE_TASK_CONDITION;

typedef struct tagDT_KILLMONSTERINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT16           wMonsterKindID; //
	UINT8            byKillNum; //
}DT_KILLMONSTERINSTANCE_TASK_CONDITION;

typedef struct tagDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION{
	UINT16           wEquipKindID; //
	UINT16           wLevel; //
}DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION;

typedef struct tagDT_STRENGTHENEQUIPNUM_TASK_CONDITION{
	UINT16           wEquipKindID; //
	UINT16           wNum; //
}DT_STRENGTHENEQUIPNUM_TASK_CONDITION;

typedef struct tagDT_ITEMCOLLECT_TASK_CONDITION{
	UINT16           wItemKindNum; //
	UINT8            abyItemCollectInfo[MAX_ITEM_KIND_COLLECT_NUM]; //
}DT_ITEMCOLLECT_TASK_CONDITION;

typedef struct tagDT_ACTIVATEGODWEAPON_TASK_CONDITION{
	UINT16           wHeroKindID; //
}DT_ACTIVATEGODWEAPON_TASK_CONDITION;

typedef struct tagDT_UPGRADEGODWEAPON_TASK_CONDITION{
	UINT16           wHeroKindID; //
	UINT16           wQuality; //
	UINT16           wLevel; //
}DT_UPGRADEGODWEAPON_TASK_CONDITION;

typedef struct tagDT_UPGRADEDRUG_TASK_CONDITION{
	UINT16           wUpgradeNum; //
}DT_UPGRADEDRUG_TASK_CONDITION;

typedef struct tagDT_CHALLENGE_TASK_CONDITION{
	UINT16           wChallengeNum; //
}DT_CHALLENGE_TASK_CONDITION;

typedef struct tagDT_PLUNDER_TASK_CONDITION{
	UINT16           wPlunderNum; //
}DT_PLUNDER_TASK_CONDITION;

typedef struct tagDT_DRESSSKILL_TASK_CONDITION{
	UINT16           wSkillID; //
	UINT8            byDressFlag; //
}DT_DRESSSKILL_TASK_CONDITION;

typedef struct tagDT_DRESSEQUIP_TASK_CONDITION{
	UINT16           wEquipID; //
	UINT8            byDressFlag; //
}DT_DRESSEQUIP_TASK_CONDITION;

typedef struct tagDT_UPGRADEEQUIP_TASK_CONDITION{
	UINT16           wEquipID; //装备ID
	UINT8            byUpgradeFlag; //完成标志
}DT_UPGRADEEQUIP_TASK_CONDITION;

typedef struct tagDT_USEEXPRIENCEDRUG_TASK_CONDITION{
	UINT16           wNum; //服用次数
}DT_USEEXPRIENCEDRUG_TASK_CONDITION;

typedef struct tagDT_MELT_EQUIP_TASK_CONDITION{
	UINT16           wNum; //服用次数
}DT_MELT_EQUIP_TASK_CONDITION;

typedef struct tagDT_COMPOSEGOOD_TASK_CONDITION{
	UINT16           wMaterialID; //原料ID
	UINT16           wMaterialNum; //原料合成个数
	UINT16           wKindID; //合成的物品ID
	UINT16           wNum; //合成次数
}DT_COMPOSEGOOD_TASK_CONDITION;

typedef struct tagDT_ELITEINSTANCE_TASK_CONDITION{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byUpgradeFlag; //完成标志
}DT_ELITEINSTANCE_TASK_CONDITION;

typedef struct tagDT_UPGRADSCIENCE_TASK_CONDITION{
	UINT16           wNum; //引导信息
}DT_UPGRADSCIENCE_TASK_CONDITION;

typedef struct tagDT_STRENGTHATTACK_TASK_CONDITION{
	UINT16           wNum; //引导信息
}DT_STRENGTHATTACK_TASK_CONDITION;

typedef struct tagDT_STRENGTHLIFE_TASK_CONDITION{
	UINT16           wNum; //引导信息
}DT_STRENGTHLIFE_TASK_CONDITION;

typedef struct tagDT_CROP_TASK_CONDITION{
	UINT16           wNum; //引导信息
}DT_CROP_TASK_CONDITION;

typedef struct tagDT_UPGRADESKILL_TASK_CONDITION{
	UINT16           wSkillActionID; //引导信息
	UINT16           wLevel; //引导信息
}DT_UPGRADESKILL_TASK_CONDITION;

typedef struct tagDT_UPGRADEJINGJIE_TASK_CONDITION{
	UINT16           wLevel; //升级境界 阶
	UINT16           wSubLevel; //升级境界 级
}DT_UPGRADEJINGJIE_TASK_CONDITION;

typedef struct tagDT_MOSAICGEM_TASK_CONDITION{
	UINT16           wNum; //引导信息
}DT_MOSAICGEM_TASK_CONDITION;

typedef struct tagDT_CLIMBTOWER_TASK_CONDITION{
	UINT16           wNum; //引导信息
}DT_CLIMBTOWER_TASK_CONDITION;

typedef struct tagDT_STUDY_TASK_CONDITION{
	UINT8            byLevel; //引导信息
}DT_STUDY_TASK_CONDITION;

typedef struct tagDT_RECRUITHERO_NUM_TASK_CONDITION{
	UINT16           wNum; //个数
}DT_RECRUITHERO_NUM_TASK_CONDITION;

typedef union tagDT_TASK_CONDITION{
	DT_CROWN_TASK_CONDITION stCrown; //引导信息
	DT_RECRUITHERO_TASK_CONDITION stRecruitHero; //引导信息
	DT_UPGRADESKILL_TASK_CONDITION stUpgradeSkill; //引导信息
	DT_UPGRADEJINGJIE_TASK_CONDITION stUpgradeJingJie; //引导信息
	DT_FIRSTPASSINSTANCE_TASK_CONDITION stFirstPassInstance; //引导信息
	DT_SCOREPASSINSTANCE_TASK_CONDITION stScorePassInstance; //引导信息
	DT_TIMESPASSINSTANCE_TASK_CONDITION stTimesPassInstance; //引导信息
	DT_KILLMONSTERINSTANCE_TASK_CONDITION stKillMonstersInstance; //引导信息
	DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION stStrengthenEquipLevel; //引导信息
	DT_STRENGTHENEQUIPNUM_TASK_CONDITION stStrengthenEquipNum; //引导信息
	DT_MOSAICGEM_TASK_CONDITION stMosaicGem; //引导信息
	DT_ACTIVATEGODWEAPON_TASK_CONDITION stActivateGodweapon; //引导信息
	DT_UPGRADEGODWEAPON_TASK_CONDITION stUpgradeGodweapon; //引导信息
	DT_CHALLENGE_TASK_CONDITION stChallenge; //引导信息
	DT_PLUNDER_TASK_CONDITION stPlunder; //引导信息
	DT_CLIMBTOWER_TASK_CONDITION stClimbTower; //引导信息
	DT_DRESSEQUIP_TASK_CONDITION stDressEquip; //引导信息
	DT_USEEXPRIENCEDRUG_TASK_CONDITION stUseExprienceDrug; //引导信息
	DT_COMPOSEGOOD_TASK_CONDITION stComposeGood; //引导信息
	DT_ELITEINSTANCE_TASK_CONDITION stEliteInstance; //引导信息
	DT_UPGRADSCIENCE_TASK_CONDITION stUpgradeScience; //引导信息
	DT_STRENGTHATTACK_TASK_CONDITION stStrengthAttack; //引导信息
	DT_STRENGTHLIFE_TASK_CONDITION stStrengthLife; //引导信息
	DT_CROP_TASK_CONDITION stCrop; //引导信息
	DT_MELT_EQUIP_TASK_CONDITION stMeltEquip; //引导信息
	DT_STUDY_TASK_CONDITION stStudy; //引导信息
	DT_RECRUITHERO_NUM_TASK_CONDITION stRecruitHeroNum; //引导信息
}DT_TASK_CONDITION;

typedef struct tagDT_TASK_DATA{
	UINT8            byTaskLine; //任务线 
	UINT32           dwTaskIdx; //任务序号(在该任务线内的)
	UINT64           qwStateUpdateTime; //状态更新时间 
	UINT8            byTaskState; //任务状态，0未接受，1已接受未完成
	UINT8            byTaskType; //任务完成条件记录
	DT_TASK_CONDITION stTaskConditionInfo; //任务完成条件记录
}DT_TASK_DATA;

typedef struct tagDT_TASK_DATA_LIST{
	UINT8            byTaskNum; //任务数(每线1个)
	DT_TASK_DATA     astTaskInfoList[MAX_TASK_LINE_NUM]; //任务信息
}DT_TASK_DATA_LIST;

typedef struct tagDT_RACE_DATA{
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
}DT_RACE_DATA;

typedef struct tagDT_LOCALE_HOME{
	UINT8            byExt; //扩展字段，暂时不用 
}DT_LOCALE_HOME;

typedef struct tagDT_LOCALE_INSTANCE{
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byBattleIdx; //战斗序号
	UINT8            bySuccessFlag; //战斗输赢，0为数，1为赢
}DT_LOCALE_INSTANCE;

typedef struct tagDT_LOCALE_ONHOOK{
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
	UINT8            byInstanceIdx; //副本序号
}DT_LOCALE_ONHOOK;

typedef struct tagDT_LOCALE_EATBREAD{
	UINT8            byExt; //扩展字段，暂时不用 
}DT_LOCALE_EATBREAD;

typedef struct tagDT_LOCALE_BOSSB{
	UINT8            byExt; //扩展字段，暂时不用 
}DT_LOCALE_BOSSB;

typedef union tagDT_LOCALE_DATA_UNION{
	DT_LOCALE_HOME   stHome; //引导信息
	DT_LOCALE_INSTANCE stInstance; //引导信息
	DT_LOCALE_ONHOOK stOnhook; //引导信息
	DT_LOCALE_EATBREAD stEatBread; //引导信息
	DT_LOCALE_BOSSB  stBossB; //引导信息
}DT_LOCALE_DATA_UNION;

typedef struct tagDT_LOCALE_DATA{
	UINT8            byLocaleType; //位置类型
	DT_LOCALE_DATA_UNION stLocaleInfo; //位置信息
}DT_LOCALE_DATA;

typedef struct tagUnlockScienceInfo{
	UINT8            byUnlockNum; //解锁科技数 
	UINT8            abyUnlockScienceID[MAX_SCIENCE_ATTR_NUM]; //科技ID
}UnlockScienceInfo;

typedef struct tagDT_HERO_UPGRADE_DATA{
	UINT16           wHeroKindID; //武将类型ID
	UINT8            byUpgradeFlag; //是否升级
	UINT16           wLevel; //当前等级
	UINT32           dwEncExperience; //奖励经验
	UINT32           dwExperience; //当前经验
	UINT32           dwUpgradeExperience; //下一级升级经验 
	UINT16           wCanUseBoxGoodsID; //可使用礼包物品ID,0为没有 
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
}DT_STATE_DATA;

typedef struct tagDT_MONSTER_BASE_DATA{
	UINT16           wKindID; //主怪物类型
	UINT16           wLevel; //主怪物等级
}DT_MONSTER_BASE_DATA;

typedef struct tagDT_MONSTER_DATA_LIST{
	UINT16           wMonsterNum; //怪物个数
	DT_MONSTER_BASE_DATA astMonsterInfListo[MAX_INSTANCE_MONSTER_NUM]; //怪物信息
}DT_MONSTER_DATA_LIST;

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
}DT_BATTLE_OBJ_DATA;

typedef struct tagDT_BATTLE_OBJ_GROUP_DATA{
	UINT8            byObjNum; //战斗者信息 
	DT_BATTLE_OBJ_DATA astBattleObjInfo[MAX_FORMATION_IDX_NUM]; //战斗者信息 
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT32           dwPlayerID; //玩家ID（怪物为0） 
}DT_BATTLE_OBJ_GROUP_DATA;

typedef struct tagDT_ITEM_DATA{
	UINT16           wKindID; //道具类型ID
	UINT16           wNum; //个数
}DT_ITEM_DATA;

typedef struct tagDT_BUFF_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT16           wAddDelFlag; //新增/删除标志(0,新增，1删除)
	UINT8            byBuffID; //BuffID
}DT_BUFF_DATA;

typedef struct tagDT_HP_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	INT32            nHP; //HP, >0则加血，<0则去血
}DT_HP_DATA;

typedef struct tagDT_ANGRY_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	INT16            shAngry; //怒气, >0则加，<0则去
}DT_ANGRY_DATA;

typedef struct tagDT_PARRY_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT8            byParryFlag; //格挡标志，1格挡,
	UINT8            byBeCritFlag; //被暴击标志，1被暴击,
}DT_PARRY_DATA;

typedef struct tagDT_SKILL_EFFECT_ID_DATA{
	UINT8            byFormationIdx; //整型序号（从1开始)
	UINT16           wEffectID; //效果ID
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
}DT_ATTACK_OBJ_DATA;

typedef struct tagDT_ATTACK_DATA{
	DT_ATTACK_OBJ_DATA stMyselfObjInfo; //我方信息
	DT_ATTACK_OBJ_DATA stEnemyObjInfo; //敌方信息
}DT_ATTACK_DATA;

typedef struct tagDT_ATTACK_ALL_DATA{
	UINT8            byAttackerIdx; //攻击方整型序号（从1开始)
	UINT16           wAttackSkillID; //攻击技能招式KindID(0为普通攻击，否则为技能招式ID)
	UINT8            bySkipFlag; //跳过攻击标志(1跳过攻击)
	UINT8            byBeAttackerIdxNum; //受击方格挡信息（从1开始)
	DT_PARRY_DATA    astBeAttackerIdxInfo[MAX_FORMATION_IDX_NUM]; //受击方格挡信息（从1开始)
	DT_ATTACK_DATA   stBeforeAttack; //受击前信息 
	DT_ATTACK_DATA   stAfterAttack; //受击后信息 
}DT_ATTACK_ALL_DATA;

typedef struct tagDT_BATTLE_ANGRY_DATA{
	UINT8            bySlot; //技能槽(从0开始)
	UINT16           wCurAngry; //当前怒气
}DT_BATTLE_ANGRY_DATA;

typedef struct tagDT_BATTLE_ROUND_DATA{
	UINT8            byAttackerFlag; //攻击者标志，0为我方 
	DT_ATTACK_ALL_DATA stAttackData; //攻击信息
}DT_BATTLE_ROUND_DATA;

typedef struct tagDT_BATTLE_DATA{
	DT_BATTLE_OBJ_GROUP_DATA stMyselfInfo; //我方信息(发起方/请求者)
	DT_BATTLE_OBJ_GROUP_DATA stEnemyInfo; //敌方信息
	UINT8            byBattleResult; //挑战战斗结果，0为我方赢，1对方赢，2我方赢并副本通关
	UINT8            byScore; //战斗评分
	UINT8            byBattleRoundNum; //战斗回合数 
	DT_BATTLE_ROUND_DATA astBattleRoundInfo[MAX_BATTLE_ROUND_NUM]; //战斗回合信息
	DT_BATTLE_ATTRIBUTE stMyselfInfoAfterBattle; //战斗后我方信息 
}DT_BATTLE_DATA;

typedef struct tagDT_ONLINE_ENCOURAGE_RECORD_DATA{
	UINT8            byRecvDays; //已领取天数 
	UINT64           qwLastRecvTime; //上次领取时间
	UINT8            byLastRecvIndex; //上次领取到的序号
}DT_ONLINE_ENCOURAGE_RECORD_DATA;

typedef struct tagDT_LOGIN_ENCOURAGE_RECORD_DATA{
	UINT16           wKeepDays; //连续登录天数
	UINT8            byLastRecvNum; //上次领取次数
	UINT64           qwLastRecvTime; //上次领取时间
	UINT8            byLastSelectIndex; //上次选中的序号(从1开始)
	UINT8            byStateFlag; //状态(LastSelectIndex不为0时有效)(0已下发，1已选择，2已入包)
}DT_LOGIN_ENCOURAGE_RECORD_DATA;

typedef struct tagDT_RACE_BATTLELOG_DATA{
	UINT8            byChallengeFlag; //挑战标志,0为主动挑战，1为被挑战
	UINT64           qwChallengeTime; //挑战时间
	UINT32           dwEnemyPlayerID; //对方玩家ID
	UINT8            byResult; //挑战结果，0为我方赢，1为对方赢
	UINT32           dwBeginChallengeRank; //挑战前排名
	UINT32           dwAfterChallengeRank; //挑战后排名
	UINT8            byNewFlag; //是否新战报，1为新
}DT_RACE_BATTLELOG_DATA;

typedef struct tagDT_RACE_BATTLELOG_DATA_LIST{
	UINT8            byBattleLogNum; //战报个数
	DT_RACE_BATTLELOG_DATA astBattleLogInfo[MAX_RACE_BATTLELOG_NUM]; //战报信息
}DT_RACE_BATTLELOG_DATA_LIST;

typedef struct tagDT_CAPTURE_DATA{
	UINT32           dwCapturePlayerID; //被俘虏的玩家ID
	UINT64           qwCaptureTime; //俘虏的时间 
	UINT8            byCaptureGridIdx; //俘虏格子序号(从0开始)
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	UINT64           qwLastCollectTime; //上次收取收益时间
}DT_CAPTURE_DATA;

typedef struct tagDT_CAPTURE_DATA_LIST{
	UINT8            byCaptureNum; //俘虏个数
	DT_CAPTURE_DATA  astCaptureInfo[MAX_CAPTURE_NUM]; //俘虏信息
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
}DT_PLUNDER_BATTLELOG_DATA;

typedef struct tagDT_PLUNDER_BATTLELOG_DATA_LIST{
	UINT8            byBattleLogNum; //战报个数
	DT_PLUNDER_BATTLELOG_DATA astBattleLogInfo[MAX_PLUNDER_BATTLELOG_NUM]; //战报信息
}DT_PLUNDER_BATTLELOG_DATA_LIST;

typedef struct tagDT_PLUNDER_GRID_DATA{
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	UINT32           dwIncomeValue; //收益值 
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
}DT_PLUNDER_BASE_DATA;

typedef struct tagDT_ENEMY_DATA{
	UINT32           dwEnemyPlayerID; //仇敌玩家ID
	UINT16           wBattleNum; //交手次数
	UINT64           qwAddTime; //添加时间
}DT_ENEMY_DATA;

typedef struct tagDT_ENEMY_DATA_LIST{
	UINT8            byEnemyNum; //战报个数
	DT_ENEMY_DATA    astEnemyInfo[MAX_ENEMY_NUM]; //战报信息
}DT_ENEMY_DATA_LIST;

typedef struct tagDT_MONEY_TREE_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	UINT16           wRockNum; //上次已摇次数
	UINT64           qwRockTime; //上次摇的时间
	UINT8            byMoodValue; //上次心情值 
	UINT64           qwMoodStartTime; //上次心情值开始时间 
}DT_MONEY_TREE_DATA;

typedef struct tagDT_INSTANCE_BATTLE_DATA{
	DT_INSTANCE_ID   stInstanceID; //副本序号
	UINT8            byBattleIndex; //战斗序号
	UINT64           qwBattleTime; //上次战斗时间
	UINT16           wMaxRounds; //最大攻击回合数
	UINT16           wMaxPercentageHp; //最大剩余血量百分比
}DT_INSTANCE_BATTLE_DATA;

typedef struct tagDT_PHYSTRENGTH_DATA{
	UINT64           qwLastPhyStrengthRefreshTime; //上次体力刷新时间
	UINT64           qwLastBuyPhyStrengthTime; //上次购买体力时间
	UINT16           wLastBuyPhyStrengthNum; //上次购买体力次数
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
}DT_ONHOOK_RESULT;

typedef struct tagDT_ONHOOK_RCD_DATA{
	UINT8            byOnhookFlag; //是否挂机中 
	DT_ONHOOK_RESULT stOnhookResult; //挂机结果记录
	DT_INSTANCE_ID   stOnhookInstanceID; //挂机副本,wSceneIdx为0表示没有挂机
	UINT64           qwLastOnhookResultReqTime; //上次请求挂机结果时间
}DT_ONHOOK_RCD_DATA;

typedef struct tagDT_FEW_PARAM_DATA{
	UINT64           qwLastStrengthenTime; //上次强化时间
	UINT16           wLastStrengthenCD; //上次强化累计CD
	UINT64           qwLastRefreshEliteTime; //上次刷新精英副本时间
	UINT16           wLastRefreshEliteNum; //上次刷新精英副本次数
}DT_FEW_PARAM_DATA;

typedef struct tagDT_CROP_INFO_DATA{
	UINT8            byOpen; //种植ID
	UINT16           wCropKindID; //种植ID
	UINT64           qwLastCropTime; //种植时间
}DT_CROP_INFO_DATA;

typedef struct tagDT_CROP_DATA{
	UINT8            byHaveDataFlag; //是否保存数据库 
	DT_CROP_INFO_DATA astCropInfoList[MAX_CROP_NUM]; //种植信息
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
}DT_SCIENCE_TREE_DATA;

typedef struct tagDT_FORMATION_HERO_DATA{
	UINT8            byFormationIdx; //阵型位置(从1开始，1～6)
	UINT16           wHeroID; //武将ID
}DT_FORMATION_HERO_DATA;

typedef struct tagDT_FORMATION_IDX_DATA{
	UINT8            byFormationKind; //阵型类型
	UINT8            byOpenIdxNum; //阵型开启位置数
	DT_FORMATION_HERO_DATA astOpenIdxInfo[MAX_FORMATION_IDX_NUM]; //阵型开启位置信息 
}DT_FORMATION_IDX_DATA;

typedef struct tagDT_FORMATION_DATA_LIST{
	UINT8            byFormationKindNum; //阵型类型数 
	DT_FORMATION_IDX_DATA astFormationInfo[MAX_FORMATION_NUM]; //阵型信息
}DT_FORMATION_DATA_LIST;

typedef struct tagDT_FORMATION_DATA{
	UINT8            bySelectIdx; //已选阵型序号（从0开始）
	DT_FORMATION_DATA_LIST stFormationList; //阵型信息
}DT_FORMATION_DATA;

typedef struct tagDT_VERSION_DATA{
	UINT32           dwVersionID; //版本ID,取2位小数点(/100)
	UINT8            byForceUpdateFlag; //是否强制更新，1强制更新
	TCHAR            aszUpdateContent[MAX_CONTENT_SIZE]; //更新内容
	TCHAR            aszUpdateURL[MAX_CONTENT_SIZE]; //更新地址
}DT_VERSION_DATA;

typedef struct tagDT_NOTICE_DATA{
	UINT32           dwNoticeID; //公告ID
	UINT8            byForcePopFlag; //是否必须弹,1为必须弹出 
	UINT8            byUrlFlag; //公告为url标志
	TCHAR            aszNoticeContent[MAX_CONTENT_SIZE]; //公告内容
}DT_NOTICE_DATA;

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
}DT_EAT_BREAD_DATA;

typedef struct tagDT_YABIAO_DATA{
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
}DT_YABIAO_DATA;

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
}DT_YABIAO_BATTLELOG_DATA;

typedef struct tagDT_YABIAO_BATTLELOG_DATA_LIST{
	UINT8            byBattleLogNum; //战报个数
	DT_YABIAO_BATTLELOG_DATA astPaoshangBattleLogInfo[MAX_YABIAO_BATTLELOG_NUM]; //战报信息
}DT_YABIAO_BATTLELOG_DATA_LIST;

typedef struct tagDT_SKILL_ACTION_DATA{
	UINT16           wID; //ID
	UINT16           wLevel; //等级
}DT_SKILL_ACTION_DATA;

typedef struct tagDT_SKILL_ACTION_DATA_LIST{
	UINT8            byActionNum; //技能招式个数 
	DT_SKILL_ACTION_DATA astActionInfo[MAX_SKILL_SLOT_NUM]; //技能招式信息 
}DT_SKILL_ACTION_DATA_LIST;

typedef struct tagDT_JINGJIE_BASE_DATA{
	UINT8            byCareerID; //职业
	UINT16           wLevel; //等级
	UINT16           wSubLevel; //子等级 
}DT_JINGJIE_BASE_DATA;

typedef struct tagDT_GAS_BALL_DATA{
	UINT8            byKind; //精气球类型 
	UINT8            byLevel; //精气球类型 
	UINT32           dwGasValue; //精气值 
}DT_GAS_BALL_DATA;

typedef struct tagDT_COLLECT_GAS_DATA{
	UINT8            byCollectState; //收集状态,0生成，1收取
	DT_GAS_BALL_DATA astGasBollInfo[MAX_GAS_BALL_NUM]; //精气球信息 
	UINT16           wLastCoinGenNum; //上次铜钱生产次数
	UINT64           qwLastCoinGenTime; //上次铜钱生产时间
	UINT16           wLastCoinRandomUpNum; //上次铜钱随机提升次数
	UINT64           qwLastCoinRandomUpTime; //上次铜钱随机提升时间
	UINT8            byLastCoinGiveNum; //上次铜钱赠送次数 
	UINT64           qwLastCoinGiveTime; //上次铜钱赠送次数时间 
	UINT16           wLastGoldGenNum; //上次元宝生产次数
	UINT64           qwLastGoldGenTime; //上次元宝生产时间
	UINT16           wLastGoldRandomUpNum; //上次元宝随机提升次数
	UINT64           qwLastGoldRandomUpTime; //上次元宝随机提升时间
	UINT8            byLastGoldGiveNum; //上次元宝赠送次数 
	UINT64           qwLastGoldGiveTime; //上次元宝赠送次数时间 
}DT_COLLECT_GAS_DATA;

typedef struct tagDT_JINGJIE_DATA{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	DT_JINGJIE_BASE_DATA stBaseData; //境界基本信息
	DT_COLLECT_GAS_DATA stCollectData; //精气收集信息
	DT_SKILL_ACTION_DATA_LIST stSkillActionData; //技能招式信息列表 
}DT_JINGJIE_DATA;

typedef struct tagDT_CLIMB_TOWER_DATA{
	UINT16           wResetClimbTownNum; //可重置次数 
	UINT64           qwResetClimbTownTime; //最后重置时间 
	UINT16           wBuyResetClimbTownNum; //每天可购买次数 
	UINT64           qwBuyResetClimbTownTime; //每天最后购买时间 
	UINT8            byClimbTownInstance; //最后玩家层数 
}DT_CLIMB_TOWER_DATA;

typedef struct tagDT_ELITE_INSTANCE_RESET_DATA{
	UINT16           wResetEliteInstanceNum; //重置精英副本次数
	UINT64           qwLastResetEliteInstanceTime; //上次重置精英副本的时间 
}DT_ELITE_INSTANCE_RESET_DATA;

typedef struct tagDT_VIP_EXT_DATA{
	UINT16           wEatGoldExperienceCnt; //伙伴服用元宝经验丹次数 
	UINT64           qwLastEatTime; //上次服用元宝经验丹的时间
	UINT16           wEliteInstanceTownNum; //精英副本城镇数 
	DT_ELITE_INSTANCE_RESET_DATA astEliteInstanceData[MAX_ELITE_INSTANCE_TOWN_NUM]; //精英副本重置数据
}DT_VIP_EXT_DATA;

typedef struct tagDT_LUAACTIVITY_RECORD_DATA{
	UINT32           dwID; //ID
	UINT16           wFinishNum; //完成次数
	UINT64           qwFinishTime; //上次完成时间
}DT_LUAACTIVITY_RECORD_DATA;

typedef struct tagDT_LUAACTIVITY_RECORD_LIST_DATA{
	UINT8            byRecordNum; //记录数 
	DT_LUAACTIVITY_RECORD_DATA astHaveFinishActivityInfo[MAX_LUAACTIVITY_RECORD_NUM]; //已完成活动ID记录信息
}DT_LUAACTIVITY_RECORD_LIST_DATA;

typedef struct tagDT_BOSSB_HURT_RATE{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT32           dwHurtValue; //玩家对Boss的伤害(若伤害通知为当时单次伤害值)
	UINT32           dwHurtRate; //玩家对Boss的伤害比例（万分比）
	UINT32           dwHurtRank; //玩家对Boss的伤害排名 
}DT_BOSSB_HURT_RATE;

typedef struct tagDT_BOSS_BATTLE_ENC_DATA{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT32           dwCoin; //铜钱
	UINT32           dwJingJie; //境界
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
}DT_BOSS_BATTLE_DATA;

typedef struct tagDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA{
	UINT64           qwOverTime; //活动结束时间
	UINT8            byActivityID; //活动ID
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自身攻击伤害
	DT_BOSS_BATTLE_ENC_DATA stHurtEncInfo; //伤害奖励信息
	DT_BOSS_BATTLE_ENC_DATA stHeroEncInfo; //英雄奖励信息
}DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA;

typedef struct tagDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST{
	UINT8            byNum; //战报个数
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA astBossBattleBL[MAX_BOSSB_BATTLE_LOG_NUM]; //战报信息
}DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST;

typedef struct tagDT_BOSS_BATTLE_MODULE_DATA{
	UINT8            byNum; //数量
	DT_BOSS_BATTLE_DATA astBossBattleInfo[MAX_BOSSB_NUM_PER_DAY]; //信息
}DT_BOSS_BATTLE_MODULE_DATA;

typedef struct tagDT_ACTIVITY_ENCOURAGE_INFO{
	UINT16           wActivityID; //奖励ID
	UINT64           qwActivityTime; //领取奖励时间
	UINT8            byActivityTimes; //领取奖励次数
}DT_ACTIVITY_ENCOURAGE_INFO;

typedef struct tagDT_ACTIVITY_ENCOURAGE_DATA{
	UINT8            byRechargeFlag; //充值奖励标志 
	UINT64           qwRechargeRecvTime; //领取充值奖励时间 
	UINT8            byScoreFlag; //评分奖励标志
	UINT64           qwScoreRecvTime; //领取评分奖励时间
	UINT16           wInfoNum; //奖励个数
	DT_ACTIVITY_ENCOURAGE_INFO astActivityInfo[MAX_ACTIVITY_ENCOURAGE_INFO_NUM]; //奖励信息
}DT_ACTIVITY_ENCOURAGE_DATA;

typedef struct tagDT_RDC_PLAYER_DATA{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT16           wMainHeroKindID; //主伙伴ID
	UINT8            byRobotFunc; //机器人功能,>0则为机器人，不同的值为不同的功能。0为玩家 
	UINT16           wLevel; //等级
	UINT32           dwPower; //战力
	UINT8            byHaveChallengeFlag; //是否已挑战，1为已挑战
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
}DT_RANDOM_CHALLENGE_DATA;

typedef struct tagDT_WORSHIP_DATA{
	UINT8            byWorshipType; //膜拜类型
	UINT8            byWorshipTypeLevel; //膜拜级别，1为最高等级 
	UINT64           qwLastWorshipTime; //膜拜时间
}DT_WORSHIP_DATA;

typedef struct tagDT_WORSHIP_DATA_LST{
	UINT16           wNum; //个数
	DT_WORSHIP_DATA  astWorshipInfo[MAX_WORSHIP_NUM]; //信息
}DT_WORSHIP_DATA_LST;

typedef struct tagDT_CHAPTER_SCORE_DATA{
	UINT16           wSceneidx; //场景ID
	UINT16           wChapterIdx; //章节ID
	UINT64           qwtime; //领取奖励时间
}DT_CHAPTER_SCORE_DATA;

typedef struct tagDT_ACCUMULATED_SCORE_DATA{
	UINT16           wgrade; //档次ID
	UINT64           qwtime; //领取奖励时间
}DT_ACCUMULATED_SCORE_DATA;

typedef struct tagDT_SCORE_DATA_LST{
	UINT8            byChapterNum; //章数
	DT_CHAPTER_SCORE_DATA astChapterData[MAX_TOWN_PER_SCENE_NUM]; //领取奖励时间
	UINT8            byAccumulatedNum; //积累个数
	DT_ACCUMULATED_SCORE_DATA astAccumulatedData[MAX_TOWN_PER_SCENE_NUM]; //领取奖励时间
}DT_SCORE_DATA_LST;

typedef struct tagDT_EQUIP_LEVEL_COMPOSE_DATA{
	UINT16           wEquipLevel; //装备等级
	UINT8            byCurLowColor; //当前最低颜色 
	UINT16           wHitKindID; //当前命中装备ID
}DT_EQUIP_LEVEL_COMPOSE_DATA;

typedef struct tagDT_EQUIP_COMPOSE_DATA{
	UINT8            byEquipLevelNum; //等级装备数 
	DT_EQUIP_LEVEL_COMPOSE_DATA astEquipLevelInfo[MAX_EQUIPLEVEL_NUM]; //装备等级合成信息
	UINT64           qwLastComposeTime; //上次合成时间
	UINT16           wLastComposeEquipLevel; //上次合成装备等级
}DT_EQUIP_COMPOSE_DATA;

typedef struct tagDT_GATHER_SCIENCE_DATA{
	UINT16           wCoinNum; //铜钱当前使用聚魂次数
	UINT64           qwCoinLastTime; //使用铜钱时间
	UINT16           wGoldNum; //元宝当前使用聚魂次数
	UINT64           qwGoldLastTime; //使用元宝时间
	UINT8            byIncrease; //倍率
}DT_GATHER_SCIENCE_DATA;

typedef struct tagDT_BEGBLUE_RES{
	UINT16           wSmileNum; //笑脸个数
	UINT32           dwBlueUnit; //单倍仙露 
	UINT32           dwStoryUnit; //单倍阅历 
	UINT16           wXnMultiple; //倍率
	TCHAR            aszLuckPrompt[BEGBLUE_LUCK_PROMPT]; //运势提示
	UINT64           qwCreateTime; //生成的时间 
	UINT64           qwUpdateTime; //改运更新的时间 
}DT_BEGBLUE_RES;

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
}DT_BEGBLUE_DATA;

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
	DT_INSTANCE_DATA_LIST stCommonInstanceData; //玩家普通副本信息 
	DT_INSTANCE_DATA_LIST stEliteInstanceData; //玩家精英副本信息
	DT_MONEY_TREE_DATA stMoneyTreeData; //摇钱树信息 
	DT_ONHOOK_RCD_DATA stOnhookRcdData; //挂机记录信息
	DT_INSTANCE_BATTLE_DATA stCommonBattleRcd; //普通副本战斗记录 
	DT_INSTANCE_BATTLE_DATA stEliteBattleRcd; //精英副本战斗记录
	DT_JINGJIE_DATA  stJingJieData; //境界信息
	DT_PHYSTRENGTH_DATA stPhystrengthData; //体力信息
	DT_FEW_PARAM_DATA stFewParamData; //少参数信息 
	DT_CROP_DATA     stCropData; //种植信息
	DT_SCIENCE_TREE_DATA stScienceTree; //科技树信息 
	DT_FORMATION_DATA stFormationInfo; //阵型信息
	CHAR             szLastOrderID[MAX_ORDER_ID_LEN]; //上次处理的订单号（用于避免重复处理）
	DT_EAT_BREAD_DATA stEatBreadInfo; //吃包子信息 
	DT_YABIAO_DATA   stYabiaoInfo; //押镖信息
	DT_INSTANCE_BATTLE_DATA stClimbTowerBattleRcd; //爬塔副本战斗记录
	DT_INSTANCE_DATA_LIST stClimbTowerInstanceData; //玩家爬塔副本信息
	DT_YABIAO_BATTLELOG_DATA_LIST stYabiaoBattleLog; //玩家跑商战报记录
	UINT8            abyNoticeID[MAX_TOKEN_NUM]; //玩家消息推送ID
	UINT64           qwLoginTime; //最后登录时间 
	UINT64           qwLogoffTime; //最后登出时间 
	DT_CLIMB_TOWER_DATA stClimbTowerData; //玩家爬塔副本信息
	DT_AWAKEN_DATA   stAwakenData; //悟道信息
	DT_VIP_EXT_DATA  stVipExtData; //VIP增加的信息 
	DT_LUAACTIVITY_RECORD_LIST_DATA stLuaActivityRecordData; //lua脚本活动记录
	DT_COLLECT_GAS_DATA stGasCollectData; //灵气收集信息
	DT_BOSS_BATTLE_PLAYER_DATA stBossBattleData; //Boss战信息 
	DT_ACTIVITY_ENCOURAGE_DATA stActivityEncourageData; //奖励信息
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST stBossBBLData; //boss战战报 
	DT_RANDOM_CHALLENGE_DATA stRdChallengeData; //随机挑战信息
	DT_WORSHIP_DATA_LST stWorshipData; //膜拜信息
	DT_SCORE_DATA_LST stScoreData; //副本评分信息
	DT_EQUIP_COMPOSE_DATA stEquipComposeData; //装备合成信息
	DT_GATHER_SCIENCE_DATA stGatherScienceData; //聚魂信息
	DT_BEGBLUE_DATA  stBegBlueData; //求仙露信息 
}DT_PLAYER_NOPVP_DATA;

typedef struct tagDT_PLAYER_PVP_DATA{
	DT_RACE_DATA     stRaceData; //玩家竞技场信息 
	DT_RACE_BATTLELOG_DATA_LIST stRaceBattleLog; //玩家竞技场战报信息 
	DT_PLUNDER_BASE_DATA stPlunderBaseData; //掠夺基本信息
	DT_PLUNDER_BATTLELOG_DATA_LIST stPlunderBattleLog; //掠夺战报信息
	DT_CAPTURE_DATA_LIST stCaptureData; //掠夺俘虏信息
	DT_ENEMY_DATA_LIST stEnemyData; //仇敌信息
}DT_PLAYER_PVP_DATA;

typedef struct tagDT_PLAYER_DATA{
	DT_PLAYER_NOPVP_DATA stPlayerNoPvpData; //玩家非PVP信息
	DT_PLAYER_PVP_DATA stPlayerPvpData; //玩家PVP信息
}DT_PLAYER_DATA;

typedef struct tagDT_EXT_FUNC_PARAM{
	UINT8            byParam1; //参数1
	UINT16           wParam2; //参数2
	UINT32           dwParam3; //参数3
}DT_EXT_FUNC_PARAM;

typedef struct tagDT_EXT_FUNC_PARAM_LST{
	UINT8            byFuncNum; //扩展功能数 
	DT_EXT_FUNC_PARAM astExtFuncParaInfo[MAX_EXT_FUN_NUM]; //扩展功能信息
}DT_EXT_FUNC_PARAM_LST;

typedef struct tagDT_FACTION_PLAYER_DATA{
	UINT32           dwJoinPlayerID; //玩家ID
	UINT8            byJoinState; //玩家当前状态 
	UINT8            byJobType; //职业类型
	UINT32           dwContribution; //贡献
	UINT64           qwLastCoinContributionTime; //铜钱贡献时间
	UINT16           wTodaysCoinContributionTimes; //今天铜钱贡献次数
	UINT8            byTodaysCoinContributionEncourage; //今天铜钱贡献领取奖励标志
	UINT64           qwLastGoldContributionTime; //元宝贡献时间
	UINT16           wTodaysGoldContributionTimes; //今天元宝贡献次数
	UINT8            byTodaysGoldContributionEncourage; //今天元宝贡献领取奖励标志
}DT_FACTION_PLAYER_DATA;

typedef struct tagDT_FACTION_PLAYER_DATA_LST{
	UINT32           dwFactionNum; //用户数 
	DT_FACTION_PLAYER_DATA astFactionPlayerData[DT_FACTION_PLAYER_NUM]; //帮派用户信息
	UINT32           dwFactionApplyNum; //申请用户数 
	UINT32           adwFactionApplyPlayerID[DT_FACTION_PLAYER_NUM]; //帮派申请用户信息
}DT_FACTION_PLAYER_DATA_LST;

typedef struct tagDT_FACTION_LOG_DATA{
	UINT32           dwPlayerID1; //玩家ID
	UINT32           dwPlayerID2; //玩家ID
	UINT8            byAction; //事件ID
	UINT32           dwParm1; //参数1
	UINT32           dwParm2; //参数2
	UINT32           dwParm3; //参数3
}DT_FACTION_LOG_DATA;

typedef struct tagDT_FACTION_LOG_DATA_LST{
	UINT16           wFactionLogNum; //用户数 
	DT_FACTION_LOG_DATA astFactionLogData[MAX_FACTION_LOG_NUM]; //帮派用户信息
}DT_FACTION_LOG_DATA_LST;

typedef struct tagDT_FACTION_BASE_DATA{
	UINT32           dwFactionID; //帮派ID
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //帮派名称
	UINT32           dwFactionRank; //帮派排名
	UINT32           dwFactionLevel; //帮派等级
	UINT32           dwFactionGold; //帮派等级
	TCHAR            aszFactionDescName[MAX_FACTION_DESC_NUM]; //帮派描述
	TCHAR            aszFactionNotice[MAX_FACTION_DESC_NUM]; //帮派通知
	TCHAR            aszFactionAnnouncement[MAX_FACTION_DESC_NUM]; //帮派公告
}DT_FACTION_BASE_DATA;

typedef struct tagDT_FACTION_DATA{
	DT_FACTION_BASE_DATA stFactionBaseData; //帮派基础数据
	DT_FACTION_PLAYER_DATA_LST stFactionPlayerDataLst; //帮派数据信息
	DT_FACTION_LOG_DATA_LST stFactionLogDataLst; //帮派数据信息
}DT_FACTION_DATA;

/*Define Structs and Unions        End*/


enum EN_ProtoCommonData_MessageID
{
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodeDT_GUIDE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GUIDE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_XIANGQIAN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_XIANGQIAN_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOODS_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STUDY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_BASE_DATA1(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_BASE_DATA1(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ABSORB_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ABSORB_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MELT_EQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MELT_EQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADEJINGJIE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEJINGJIE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MOSAICGEM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MOSAICGEM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLIMBTOWER_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIMBTOWER_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STUDY_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECRUITHERO_NUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUITHERO_NUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RACE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LOCALE_EATBREAD(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_EATBREAD(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LOCALE_BOSSB(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_BOSSB(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeUnlockScienceInfo(void *pHost, CNetData* poNetData);
INT32 DecodeUnlockScienceInfo(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STATE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STATE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUFF_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUFF_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PARRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PARRY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VERSION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VERSION_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EAT_BREAD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EAT_BREAD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_YABIAO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_YABIAO_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_YABIAO_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_ACTION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_ACTION_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_ACTION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_ACTION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JINGJIE_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JINGJIE_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GAS_BALL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GAS_BALL_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COLLECT_GAS_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COLLECT_GAS_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JINGJIE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JINGJIE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLIMB_TOWER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIMB_TOWER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ELITE_INSTANCE_RESET_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITE_INSTANCE_RESET_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VIP_EXT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_EXT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LUAACTIVITY_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LUAACTIVITY_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LUAACTIVITY_RECORD_LIST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LUAACTIVITY_RECORD_LIST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_HURT_RATE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_HURT_RATE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSS_BATTLE_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSS_BATTLE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSS_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSS_BATTLE_MODULE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_MODULE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVITY_ENCOURAGE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_ENCOURAGE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVITY_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RDC_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RDC_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RANDOM_CHALLENGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RANDOM_CHALLENGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_WORSHIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WORSHIP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_WORSHIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WORSHIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CHAPTER_SCORE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHAPTER_SCORE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACCUMULATED_SCORE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCUMULATED_SCORE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SCORE_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCORE_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_LEVEL_COMPOSE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_LEVEL_COMPOSE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_COMPOSE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_COMPOSE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GATHER_SCIENCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GATHER_SCIENCE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BEGBLUE_RES(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_RES(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BEGBLUE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_NOPVP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_NOPVP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_PVP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_PVP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_LOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LOG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_LOG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LOG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_DATA(void *pHost, CNetData* poNetData);
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
