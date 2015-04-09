/*       Version Number: 10          */

#ifndef PROTOCOMMONDATA_H_INCLUDE_VERSION_10
#define PROTOCOMMONDATA_H_INCLUDE_VERSION_10

#include "netdata.h"
#include "sdtype.h"
#include "sdstring.h"
#include <json/json.h>
#pragma pack(push, 1)

/*Defines Begin*/
#define SUCCESS                  0
#define FAIL                     1
#define SESSIONID_LEN            8
#define DEVICEID_LEN             41
#define NOTIFYID_LEN             65
#define USERNAME_LEN             41
#define USERTYPE_LEN             41
#define USERPWD_LEN              17
#define MSG_LEN                  1024
#define IPSTR_LEN                17
#define LOGIN_TYPE_USER          0
#define LOGIN_TYPE_IPHONE        1
#define MAX_HERO_NUM             30
#define NEED                     1
#define NONNEED                  0
#define MAX_BAG_OPEN_NUM         100
#define CURRENCY_TYPE_COIN       0
#define CURRENCY_TYPE_GOLD       1
#define MAX_SKILL_NUM            12
#define MAX_SKILL_SLOT_NUM       3
#define MAX_SKILL_BUFF_NUM       3
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
#define MAX_INSTANCE_PER_TOWN_NUM 3
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
#define CROWN                    1
#define RECRUITHERO              2
#define FIRSTPASSINSTANCE        5
#define SCOREPASSINSTANCE        6
#define TIMESPASSINSTANCE        7
#define KILLMONSTERINSTANCE      8
#define STRENGTHENEQUIPLEVEL     9
#define STRENGTHENEQUIPNUM       10
#define ACTIVATEGODWEAPON        12
#define UPGRADEGODWEAPON         13
#define CHALLENGE                14
#define PLUNDER                  15
#define DRESSEQUIP               19
#define COMPOSEGOOD              20
#define ELITEINSTANCE            21
#define UPGRADSCIENCE            22
#define STRENGTHATTACK           23
#define STRENGTHLIFE             24
#define CROP                     25
#define USEEXPRIENCEDRUG         26
#define MULT                     27
#define MAX_TASK_DSPNAME_LEN     40
#define MAX_TASK_DESC_LEN        100
#define ETS_GIVEN                0
#define ETS_ACCEPTED             1
#define ETS_ENCOURAGE            2
#define ETS_FINISHED             3
#define ETS_INVALID              4
#define MAX_CHALLENGE_NUM        10
#define MAX_SHOP_ITEM_NUM        100
#define MAX_ENCOURAGE_ITEM_KIND_NUM 10
#define MAX_ENCOURAGE_BOX_GROUP_NUM 3
#define MAX_UNLOCK_HERO_NUM      3
#define EPL_HOME                 0
#define EPL_INSTANCE             1
#define EPL_ONHOOK               2
#define EPL_PAGODA               3
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
#define GET_ZONE_NUM             20
#define MAX_LOGIN_ENCOURAGE_ITEM_NUM 8
#define MAX_RANK_SHOW_NUM        100
#define MAX_RACE_BATTLELOG_NUM   100
#define MAX_COMPOSE_ITEM_NUM     5
#define MAX_PLUNDER_BATTLELOG_NUM 100
#define MAX_CAPTURE_NUM          4
#define MAX_GET_ENEMY_NUM        5
#define MAX_GET_PLUNDER_ENEMY_NUM 5
#define MAX_ROBOT_NUM            10
#define MAX_ENEMY_NUM            200
#define STUDY_COLOR_NUM          4
#define MAX_GRID_STUDY_CLIP_NUM  16
#define MAX_STUDY_STORE_NUM      6
#define MAX_STUDY_KIND_NUM       8
#define MAX_HERO_STUDY_NUM       8
#define MAX_GODWEAPON_QUALITY_NUM 6
#define MAX_BUFF_NUM             27
#define MAX_INSTANCE_NUM         200
#define EQUIP_POS_NUM            4
#define MAX_CROP_NUM             6
#define MAX_CROP_SEED_NUM        50
#define MAX_SCIENCE_KIND_NUM     15
#define MAX_CONTENT_SIZE         255
#define MAX_ORDER_ID_LEN         255
#define AUTH_TYPE_MAXNET         0
#define AUTH_TYPE_TONGBU         1
#define AUTH_TYPE_91             2
#define PROTOCOMMONDATA_MSGID_RANGE 1
#define PROTOCOMMONDATA_MSGID_OFFSET 0
#define PROTOCOL_VERSION 10
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagDT_PLAYER_BASE_DATA{
	UINT32           dwID; //玩家唯一ID
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
	UINT64           qwStory; //阅历
	UINT16           wPhyStrength; //当前体力
	UINT64           qwScience; //当前科技点
	UINT64           qwGuideRecord; //引导记录
	UINT8            byRobotFunc; //机器人功能,>0则为机器人，不同的值为不同的功能。0为玩家
	UINT32           dwPower; //战力(玩家init时重新赋值)
	UINT32           dwFirstAttack; //先攻值
	UINT64           qwTodayFirstLoginTime; //今天第一次登陆时间
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
	UINT16           wSelectSkillID; //所选技能
	UINT16           wSelectSkillLevel; //所选技能等级
}DT_HERO_BASE_DATA;

typedef struct tagDT_EQUIP_DATA{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT16           wLevel; //装备等级
	UINT16           wBagPos; //所在背包位置
	UINT64           qwItemNewTime; //道具生产时间
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

typedef struct tagDT_SKILL_DATA{
	UINT16           wID; //技能类型ID
	UINT16           wLevel; //技能等级
	UINT8            byTrainNum; //技能培养次数
	UINT8            bySlotIdx; //技能槽(0表示未使用，1表示使用(目前1个伙伴只支持穿戴1个技能)
}DT_SKILL_DATA;

typedef struct tagDT_SKILL_DATA_LIST{
	UINT8            bySkillNum; //技能个数
	DT_SKILL_DATA    astSkillInfo[MAX_SKILL_NUM]; //技能信息
}DT_SKILL_DATA_LIST;

typedef struct tagDT_HERO_DATA{
	DT_HERO_BASE_DATA stHeroBaseData; //武将基本信息
	DT_HERO_EQUIP_DATA_LST stEquipDataInfo; //武将装备信息
	DT_DRUG_DATA_LST stDrugDataInfo; //丹药信息
	DT_GODWEAPON_DATA stGodweaponInfo; //神器信息
	DT_SKILL_DATA_LIST stSkillData; //技能信息
}DT_HERO_DATA;

typedef struct tagDT_HERO_DATA_LIST{
	UINT16           wHeroNum; //武将数目
	DT_HERO_DATA     astHeroInfoLst[MAX_HERO_NUM]; //武将信息
}DT_HERO_DATA_LIST;

typedef struct tagDT_BAG_DATA{
	UINT8            byOpenNum; //打开格子数(在原来基础上)
}DT_BAG_DATA;

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
	UINT8            byScore; //副本评分
	UINT16           wPassTimes; //副本通关次数(0则表示未通关)
	UINT64           qwLastPassTime; //上次副本通关时间
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
	UINT16           wRecruitHeroKindID; //
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

typedef struct tagDT_UPGRADESKILL_TASK_CONDITION{
	UINT16           wSkillID; //技能ID
	UINT16           wUpgradeNum; //完成标志
}DT_UPGRADESKILL_TASK_CONDITION;

typedef struct tagDT_UPGRADEEQUIP_TASK_CONDITION{
	UINT16           wEquipID; //装备ID
	UINT8            byUpgradeFlag; //完成标志
}DT_UPGRADEEQUIP_TASK_CONDITION;

typedef struct tagDT_USEEXPRIENCEDRUG_TASK_CONDITION{
	UINT16           wNum; //服用次数
}DT_USEEXPRIENCEDRUG_TASK_CONDITION;

typedef struct tagDT_MULT_TASK_CONDITION{
	UINT16           wNum; //服用次数
}DT_MULT_TASK_CONDITION;

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
	UINT16           wNum; //玩家基本信息
}DT_UPGRADSCIENCE_TASK_CONDITION;

typedef struct tagDT_STRENGTHATTACK_TASK_CONDITION{
	UINT16           wNum; //玩家基本信息
}DT_STRENGTHATTACK_TASK_CONDITION;

typedef struct tagDT_STRENGTHLIFE_TASK_CONDITION{
	UINT16           wNum; //玩家基本信息
}DT_STRENGTHLIFE_TASK_CONDITION;

typedef struct tagDT_CROP_TASK_CONDITION{
	UINT16           wNum; //玩家基本信息
}DT_CROP_TASK_CONDITION;

typedef union tagDT_TASK_CONDITION{
	DT_CROWN_TASK_CONDITION stCrown; //玩家基本信息
	DT_RECRUITHERO_TASK_CONDITION stRecruitHero; //<item name="BuildUpgrade" type="DT_BUILDUPGRADE_TASK_CONDITION" id="BUILDUPGRADE" />
	DT_FIRSTPASSINSTANCE_TASK_CONDITION stFirstPassInstance; //<item name="HeroUpgrade" type="DT_CROWN_HEROUPGRADE_CONDITION" id="HEROUPGRADE" />
	DT_SCOREPASSINSTANCE_TASK_CONDITION stScorePassInstance; //<item name="HeroUpgrade" type="DT_CROWN_HEROUPGRADE_CONDITION" id="HEROUPGRADE" />
	DT_TIMESPASSINSTANCE_TASK_CONDITION stTimesPassInstance; //<item name="HeroUpgrade" type="DT_CROWN_HEROUPGRADE_CONDITION" id="HEROUPGRADE" />
	DT_KILLMONSTERINSTANCE_TASK_CONDITION stKillMonstersInstance; //<item name="HeroUpgrade" type="DT_CROWN_HEROUPGRADE_CONDITION" id="HEROUPGRADE" />
	DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION stStrengthenEquipLevel; //<item name="HeroUpgrade" type="DT_CROWN_HEROUPGRADE_CONDITION" id="HEROUPGRADE" />
	DT_STRENGTHENEQUIPNUM_TASK_CONDITION stStrengthenEquipNum; //<item name="HeroUpgrade" type="DT_CROWN_HEROUPGRADE_CONDITION" id="HEROUPGRADE" />
	DT_ACTIVATEGODWEAPON_TASK_CONDITION stActivateGodweapon; //<item name="ItemCollect" type="DT_ITEMCOLLECT_TASK_CONDITION" id="ITEMCOLLECT" />
	DT_UPGRADEGODWEAPON_TASK_CONDITION stUpgradeGodweapon; //<item name="ItemCollect" type="DT_ITEMCOLLECT_TASK_CONDITION" id="ITEMCOLLECT" />
	DT_CHALLENGE_TASK_CONDITION stChallenge; //<item name="ItemCollect" type="DT_ITEMCOLLECT_TASK_CONDITION" id="ITEMCOLLECT" />
	DT_PLUNDER_TASK_CONDITION stPlunder; //<item name="ItemCollect" type="DT_ITEMCOLLECT_TASK_CONDITION" id="ITEMCOLLECT" />
	DT_DRESSEQUIP_TASK_CONDITION stDressEquip; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_USEEXPRIENCEDRUG_TASK_CONDITION stUseExprienceDrug; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_COMPOSEGOOD_TASK_CONDITION stComposeGood; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_ELITEINSTANCE_TASK_CONDITION stEliteInstance; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_UPGRADSCIENCE_TASK_CONDITION stUpgradeScience; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_STRENGTHATTACK_TASK_CONDITION stStrengthAttack; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_STRENGTHLIFE_TASK_CONDITION stStrengthLife; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_CROP_TASK_CONDITION stCrop; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
	DT_MULT_TASK_CONDITION stMult; //<item name="UpgradeEquip" type="DT_UPGRADEEQUIP_TASK_CONDITION" id="UPGRADEEQUIP" />
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

typedef union tagDT_LOCALE_DATA_UNION{
	DT_LOCALE_HOME   stHome; //玩家基本信息
	DT_LOCALE_INSTANCE stInstance; //玩家基本信息
	DT_LOCALE_ONHOOK stOnhook; //玩家基本信息
}DT_LOCALE_DATA_UNION;

typedef struct tagDT_LOCALE_DATA{
	UINT8            byLocaleType; //位置类型
	DT_LOCALE_DATA_UNION stLocaleInfo; //位置信息
}DT_LOCALE_DATA;

typedef struct tagUnlockScienceInfo{
	UINT8            byUnlockNum; //解锁科技数
	UINT8            abyUnlockScienceID[MAX_SCIENCE_ATTR_NUM]; //科技ID
}UnlockScienceInfo;

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
	UINT16           wSkillID; //技能ID(0表示没有技能)
	UINT16           wSkillLevel; //技能等级
	UINT16           wNeedAngry; //释放所需怒气
	UINT16           wInitAngry; //初始怒气
	UINT8            byHPGridNum; //HP格子数
}DT_BATTLE_OBJ_DATA;

typedef struct tagDT_BATTLE_OBJ_GROUP_DATA{
	UINT8            byObjNum; //战斗者信息
	DT_BATTLE_OBJ_DATA astBattleObjInfo[MAX_FORMATION_IDX_NUM]; //战斗者信息
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
	UINT16           wAttackSkillID; //攻击技能KindID(0为普通攻击，否则为技能ID)
	UINT16           wAttackSkillLevel; //攻击技能等级(AttackSkillID不为0有效)
	UINT8            byCritFlag; //暴击标志(攻击方：1暴击,2停止攻击)
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

typedef struct tagDT_STUDY_DATA{
	UINT16           wID; //ID(用于数据库管理)
	UINT8            byGridIdx; //存储格子序号(若已穿戴则为对应的格子，从0开始)
	UINT16           wHeroKindID; //穿戴的武将ID(0为未穿戴)
	UINT8            byKind; //心得类型
	UINT8            byColor; //颜色(0表示无数据)
	UINT8            byLevel; //等级
}DT_STUDY_DATA;

typedef struct tagDT_STUDY_CLIP_DATA{
	UINT16           wID; //ID(用于数据库管理)
	UINT8            byGridIdx; //存储格子序号(已拾取则为4个颜色对应的格子，从0开始)
	UINT8            byPickupFlag; //拾取标志
	UINT8            byColor; //颜色(0为没有数据)
	UINT16           wNum; //个数(已拾取有效)
}DT_STUDY_CLIP_DATA;

typedef struct tagDT_AWAKEN_DATA{
	UINT8            byLevel; //悟道等级（0则没有数据）
	UINT8            byAuto2ExpFlag; //自动将心得碎片转换成经验标志（0未选中，1选中）
	UINT64           qwStudyExp; //心得经验
}DT_AWAKEN_DATA;

typedef struct tagDT_INSTANCE_BATTLE_DATA{
	DT_INSTANCE_ID   stInstanceID; //副本序号
	UINT8            byBattleIndex; //战斗序号
	UINT64           qwBattleTime; //上次战斗时间
}DT_INSTANCE_BATTLE_DATA;

typedef struct tagDT_PHYSTRENGTH_DATA{
	UINT64           qwLastPhyStrengthRefreshTime; //上次体力刷新时间
	UINT64           qwLastBuyPhyStrengthTime; //上次购买体力时间
	UINT8            byLastBuyPhyStrengthNum; //上次购买体力次数
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

typedef struct tagDT_PLAYER_DATA{
	DT_PLAYER_BASE_DATA stBaseData; //玩家基本信息
	DT_HERO_DATA_LIST stHeroData; //玩家武将信息列表
	DT_BAG_DATA      stBagData; //玩家背包信息
	DT_BAG_EQUIP_DATA_LST stBagEquipData; //玩家背包装备道具信息(存放于背包的装备，不包含已装备的装备)
	DT_BAG_GOODS_DATA_LST stBagGoodsData; //玩家背包物品道具信息
	DT_BUILD_DATA_LIST stBuildData; //玩家建筑信息
	DT_TASK_DATA_LIST stTaskData; //玩家任务信息
	DT_RACE_DATA     stRaceData; //玩家竞技场信息
	DT_RACE_BATTLELOG_DATA_LIST stRaceBattleLog; //玩家竞技场战报信息
	DT_LOCALE_DATA   stLocaleData; //玩家当前位置信息
	DT_STATE_DATA    stCurState; //玩家当前状态信息
	DT_ONLINE_ENCOURAGE_RECORD_DATA stOnlineEncRecdData; //在线奖励信息
	DT_LOGIN_ENCOURAGE_RECORD_DATA stLoginEncRecdData; //登陆奖励信息
	DT_INSTANCE_DATA_LIST stCommonInstanceData; //玩家普通副本信息
	DT_INSTANCE_DATA_LIST stEliteInstanceData; //玩家精英副本信息
	DT_PLUNDER_BASE_DATA stPlunderBaseData; //掠夺基本信息
	DT_PLUNDER_BATTLELOG_DATA_LIST stPlunderBattleLog; //掠夺战报信息
	DT_CAPTURE_DATA_LIST stCaptureData; //掠夺俘虏信息
	DT_ENEMY_DATA_LIST stEnemyData; //仇敌信息
	DT_MONEY_TREE_DATA stMoneyTreeData; //摇钱树信息
	DT_ONHOOK_RCD_DATA stOnhookRcdData; //挂机记录信息
	DT_INSTANCE_BATTLE_DATA stCommonBattleRcd; //普通副本战斗记录
	DT_INSTANCE_BATTLE_DATA stEliteBattleRcd; //精英副本战斗记录
	DT_SKILL_DATA_LIST stSkillData; //技能信息
	DT_PHYSTRENGTH_DATA stPhystrengthData; //体力信息
	DT_FEW_PARAM_DATA stFewParamData; //少参数信息
	DT_CROP_DATA     stCropData; //种植信息
	DT_SCIENCE_TREE_DATA stScienceTree; //科技树信息
	DT_FORMATION_DATA stFormationInfo; //阵型信息
	CHAR             szLastOrderID[MAX_ORDER_ID_LEN]; //上次处理的订单号（用于避免重复处理）
}DT_PLAYER_DATA;

/*Define Structs and Unions        End*/


enum EN_ProtoCommonData_MessageID
{
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLAYER_BASE_DATA( DT_PLAYER_BASE_DATA &data );
BOOL FromJsonDT_PLAYER_BASE_DATA(const string &strData, DT_PLAYER_BASE_DATA &data);
BOOL FromJsonDT_PLAYER_BASE_DATA(Json::Value &root, DT_PLAYER_BASE_DATA &data);
INT32 EncodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BATTLE_ATTRIBUTE( DT_BATTLE_ATTRIBUTE &data );
BOOL FromJsonDT_BATTLE_ATTRIBUTE(const string &strData, DT_BATTLE_ATTRIBUTE &data);
BOOL FromJsonDT_BATTLE_ATTRIBUTE(Json::Value &root, DT_BATTLE_ATTRIBUTE &data);
INT32 EncodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HIDE_BATTLE_ATTRIBUTE( DT_HIDE_BATTLE_ATTRIBUTE &data );
BOOL FromJsonDT_HIDE_BATTLE_ATTRIBUTE(const string &strData, DT_HIDE_BATTLE_ATTRIBUTE &data);
BOOL FromJsonDT_HIDE_BATTLE_ATTRIBUTE(Json::Value &root, DT_HIDE_BATTLE_ATTRIBUTE &data);
INT32 EncodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EXT_BATTLE_ATTRIBUTE( DT_EXT_BATTLE_ATTRIBUTE &data );
BOOL FromJsonDT_EXT_BATTLE_ATTRIBUTE(const string &strData, DT_EXT_BATTLE_ATTRIBUTE &data);
BOOL FromJsonDT_EXT_BATTLE_ATTRIBUTE(Json::Value &root, DT_EXT_BATTLE_ATTRIBUTE &data);
INT32 EncodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_UPDATE_ATTR_DATA( DT_HERO_UPDATE_ATTR_DATA &data );
BOOL FromJsonDT_HERO_UPDATE_ATTR_DATA(const string &strData, DT_HERO_UPDATE_ATTR_DATA &data);
BOOL FromJsonDT_HERO_UPDATE_ATTR_DATA(Json::Value &root, DT_HERO_UPDATE_ATTR_DATA &data);
INT32 EncodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_BASE_DATA( DT_HERO_BASE_DATA &data );
BOOL FromJsonDT_HERO_BASE_DATA(const string &strData, DT_HERO_BASE_DATA &data);
BOOL FromJsonDT_HERO_BASE_DATA(Json::Value &root, DT_HERO_BASE_DATA &data);
INT32 EncodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_DATA( DT_EQUIP_DATA &data );
BOOL FromJsonDT_EQUIP_DATA(const string &strData, DT_EQUIP_DATA &data);
BOOL FromJsonDT_EQUIP_DATA(Json::Value &root, DT_EQUIP_DATA &data);
INT32 EncodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_EQUIP_DATA_LST( DT_HERO_EQUIP_DATA_LST &data );
BOOL FromJsonDT_HERO_EQUIP_DATA_LST(const string &strData, DT_HERO_EQUIP_DATA_LST &data);
BOOL FromJsonDT_HERO_EQUIP_DATA_LST(Json::Value &root, DT_HERO_EQUIP_DATA_LST &data);
INT32 EncodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BAG_EQUIP_DATA_LST( DT_BAG_EQUIP_DATA_LST &data );
BOOL FromJsonDT_BAG_EQUIP_DATA_LST(const string &strData, DT_BAG_EQUIP_DATA_LST &data);
BOOL FromJsonDT_BAG_EQUIP_DATA_LST(Json::Value &root, DT_BAG_EQUIP_DATA_LST &data);
INT32 EncodeDT_GOODS_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GOODS_DATA( DT_GOODS_DATA &data );
BOOL FromJsonDT_GOODS_DATA(const string &strData, DT_GOODS_DATA &data);
BOOL FromJsonDT_GOODS_DATA(Json::Value &root, DT_GOODS_DATA &data);
INT32 EncodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BAG_GOODS_DATA_LST( DT_BAG_GOODS_DATA_LST &data );
BOOL FromJsonDT_BAG_GOODS_DATA_LST(const string &strData, DT_BAG_GOODS_DATA_LST &data);
BOOL FromJsonDT_BAG_GOODS_DATA_LST(Json::Value &root, DT_BAG_GOODS_DATA_LST &data);
INT32 EncodeDT_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRUG_DATA( DT_DRUG_DATA &data );
BOOL FromJsonDT_DRUG_DATA(const string &strData, DT_DRUG_DATA &data);
BOOL FromJsonDT_DRUG_DATA(Json::Value &root, DT_DRUG_DATA &data);
INT32 EncodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRUG_DATA_LST( DT_DRUG_DATA_LST &data );
BOOL FromJsonDT_DRUG_DATA_LST(const string &strData, DT_DRUG_DATA_LST &data);
BOOL FromJsonDT_DRUG_DATA_LST(Json::Value &root, DT_DRUG_DATA_LST &data);
INT32 EncodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GODWEAPON_DATA( DT_GODWEAPON_DATA &data );
BOOL FromJsonDT_GODWEAPON_DATA(const string &strData, DT_GODWEAPON_DATA &data);
BOOL FromJsonDT_GODWEAPON_DATA(Json::Value &root, DT_GODWEAPON_DATA &data);
INT32 EncodeDT_SKILL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SKILL_DATA( DT_SKILL_DATA &data );
BOOL FromJsonDT_SKILL_DATA(const string &strData, DT_SKILL_DATA &data);
BOOL FromJsonDT_SKILL_DATA(Json::Value &root, DT_SKILL_DATA &data);
INT32 EncodeDT_SKILL_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SKILL_DATA_LIST( DT_SKILL_DATA_LIST &data );
BOOL FromJsonDT_SKILL_DATA_LIST(const string &strData, DT_SKILL_DATA_LIST &data);
BOOL FromJsonDT_SKILL_DATA_LIST(Json::Value &root, DT_SKILL_DATA_LIST &data);
INT32 EncodeDT_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_DATA( DT_HERO_DATA &data );
BOOL FromJsonDT_HERO_DATA(const string &strData, DT_HERO_DATA &data);
BOOL FromJsonDT_HERO_DATA(Json::Value &root, DT_HERO_DATA &data);
INT32 EncodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_DATA_LIST( DT_HERO_DATA_LIST &data );
BOOL FromJsonDT_HERO_DATA_LIST(const string &strData, DT_HERO_DATA_LIST &data);
BOOL FromJsonDT_HERO_DATA_LIST(Json::Value &root, DT_HERO_DATA_LIST &data);
INT32 EncodeDT_BAG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BAG_DATA( DT_BAG_DATA &data );
BOOL FromJsonDT_BAG_DATA(const string &strData, DT_BAG_DATA &data);
BOOL FromJsonDT_BAG_DATA(Json::Value &root, DT_BAG_DATA &data);
INT32 EncodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BUILD_BASE_DATA( DT_BUILD_BASE_DATA &data );
BOOL FromJsonDT_BUILD_BASE_DATA(const string &strData, DT_BUILD_BASE_DATA &data);
BOOL FromJsonDT_BUILD_BASE_DATA(Json::Value &root, DT_BUILD_BASE_DATA &data);
INT32 EncodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BUILD_DATA_LIST( DT_BUILD_DATA_LIST &data );
BOOL FromJsonDT_BUILD_DATA_LIST(const string &strData, DT_BUILD_DATA_LIST &data);
BOOL FromJsonDT_BUILD_DATA_LIST(Json::Value &root, DT_BUILD_DATA_LIST &data);
INT32 EncodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_INSTANCE_ID( DT_INSTANCE_ID &data );
BOOL FromJsonDT_INSTANCE_ID(const string &strData, DT_INSTANCE_ID &data);
BOOL FromJsonDT_INSTANCE_ID(Json::Value &root, DT_INSTANCE_ID &data);
INT32 EncodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_INSTANCE_DATA( DT_INSTANCE_DATA &data );
BOOL FromJsonDT_INSTANCE_DATA(const string &strData, DT_INSTANCE_DATA &data);
BOOL FromJsonDT_INSTANCE_DATA(Json::Value &root, DT_INSTANCE_DATA &data);
INT32 EncodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_INSTANCE_DATA_LIST( DT_INSTANCE_DATA_LIST &data );
BOOL FromJsonDT_INSTANCE_DATA_LIST(const string &strData, DT_INSTANCE_DATA_LIST &data);
BOOL FromJsonDT_INSTANCE_DATA_LIST(Json::Value &root, DT_INSTANCE_DATA_LIST &data);
INT32 EncodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SCIENCE_ATTREXT( DT_SCIENCE_ATTREXT &data );
BOOL FromJsonDT_SCIENCE_ATTREXT(const string &strData, DT_SCIENCE_ATTREXT &data);
BOOL FromJsonDT_SCIENCE_ATTREXT(Json::Value &root, DT_SCIENCE_ATTREXT &data);
INT32 EncodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SCIENCE_ATTREXT_LIST( DT_SCIENCE_ATTREXT_LIST &data );
BOOL FromJsonDT_SCIENCE_ATTREXT_LIST(const string &strData, DT_SCIENCE_ATTREXT_LIST &data);
BOOL FromJsonDT_SCIENCE_ATTREXT_LIST(Json::Value &root, DT_SCIENCE_ATTREXT_LIST &data);
INT32 EncodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROWN_TASK_CONDITION( DT_CROWN_TASK_CONDITION &data );
BOOL FromJsonDT_CROWN_TASK_CONDITION(const string &strData, DT_CROWN_TASK_CONDITION &data);
BOOL FromJsonDT_CROWN_TASK_CONDITION(Json::Value &root, DT_CROWN_TASK_CONDITION &data);
INT32 EncodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BUILDUPGRADE_TASK_CONDITION( DT_BUILDUPGRADE_TASK_CONDITION &data );
BOOL FromJsonDT_BUILDUPGRADE_TASK_CONDITION(const string &strData, DT_BUILDUPGRADE_TASK_CONDITION &data);
BOOL FromJsonDT_BUILDUPGRADE_TASK_CONDITION(Json::Value &root, DT_BUILDUPGRADE_TASK_CONDITION &data);
INT32 EncodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RECRUITHERO_TASK_CONDITION( DT_RECRUITHERO_TASK_CONDITION &data );
BOOL FromJsonDT_RECRUITHERO_TASK_CONDITION(const string &strData, DT_RECRUITHERO_TASK_CONDITION &data);
BOOL FromJsonDT_RECRUITHERO_TASK_CONDITION(Json::Value &root, DT_RECRUITHERO_TASK_CONDITION &data);
INT32 EncodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROWN_HEROCOLOR_CONDITION( DT_CROWN_HEROCOLOR_CONDITION &data );
BOOL FromJsonDT_CROWN_HEROCOLOR_CONDITION(const string &strData, DT_CROWN_HEROCOLOR_CONDITION &data);
BOOL FromJsonDT_CROWN_HEROCOLOR_CONDITION(Json::Value &root, DT_CROWN_HEROCOLOR_CONDITION &data);
INT32 EncodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROWN_HEROUPGRADE_CONDITION( DT_CROWN_HEROUPGRADE_CONDITION &data );
BOOL FromJsonDT_CROWN_HEROUPGRADE_CONDITION(const string &strData, DT_CROWN_HEROUPGRADE_CONDITION &data);
BOOL FromJsonDT_CROWN_HEROUPGRADE_CONDITION(Json::Value &root, DT_CROWN_HEROUPGRADE_CONDITION &data);
INT32 EncodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION( DT_FIRSTPASSINSTANCE_TASK_CONDITION &data );
BOOL FromJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION(const string &strData, DT_FIRSTPASSINSTANCE_TASK_CONDITION &data);
BOOL FromJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION(Json::Value &root, DT_FIRSTPASSINSTANCE_TASK_CONDITION &data);
INT32 EncodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SCOREPASSINSTANCE_TASK_CONDITION( DT_SCOREPASSINSTANCE_TASK_CONDITION &data );
BOOL FromJsonDT_SCOREPASSINSTANCE_TASK_CONDITION(const string &strData, DT_SCOREPASSINSTANCE_TASK_CONDITION &data);
BOOL FromJsonDT_SCOREPASSINSTANCE_TASK_CONDITION(Json::Value &root, DT_SCOREPASSINSTANCE_TASK_CONDITION &data);
INT32 EncodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TIMESPASSINSTANCE_TASK_CONDITION( DT_TIMESPASSINSTANCE_TASK_CONDITION &data );
BOOL FromJsonDT_TIMESPASSINSTANCE_TASK_CONDITION(const string &strData, DT_TIMESPASSINSTANCE_TASK_CONDITION &data);
BOOL FromJsonDT_TIMESPASSINSTANCE_TASK_CONDITION(Json::Value &root, DT_TIMESPASSINSTANCE_TASK_CONDITION &data);
INT32 EncodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION( DT_KILLMONSTERINSTANCE_TASK_CONDITION &data );
BOOL FromJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION(const string &strData, DT_KILLMONSTERINSTANCE_TASK_CONDITION &data);
BOOL FromJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION(Json::Value &root, DT_KILLMONSTERINSTANCE_TASK_CONDITION &data);
INT32 EncodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION( DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION &data );
BOOL FromJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(const string &strData, DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION &data);
BOOL FromJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(Json::Value &root, DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION &data);
INT32 EncodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION( DT_STRENGTHENEQUIPNUM_TASK_CONDITION &data );
BOOL FromJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION(const string &strData, DT_STRENGTHENEQUIPNUM_TASK_CONDITION &data);
BOOL FromJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION(Json::Value &root, DT_STRENGTHENEQUIPNUM_TASK_CONDITION &data);
INT32 EncodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ITEMCOLLECT_TASK_CONDITION( DT_ITEMCOLLECT_TASK_CONDITION &data );
BOOL FromJsonDT_ITEMCOLLECT_TASK_CONDITION(const string &strData, DT_ITEMCOLLECT_TASK_CONDITION &data);
BOOL FromJsonDT_ITEMCOLLECT_TASK_CONDITION(Json::Value &root, DT_ITEMCOLLECT_TASK_CONDITION &data);
INT32 EncodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION( DT_ACTIVATEGODWEAPON_TASK_CONDITION &data );
BOOL FromJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION(const string &strData, DT_ACTIVATEGODWEAPON_TASK_CONDITION &data);
BOOL FromJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION(Json::Value &root, DT_ACTIVATEGODWEAPON_TASK_CONDITION &data);
INT32 EncodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UPGRADEGODWEAPON_TASK_CONDITION( DT_UPGRADEGODWEAPON_TASK_CONDITION &data );
BOOL FromJsonDT_UPGRADEGODWEAPON_TASK_CONDITION(const string &strData, DT_UPGRADEGODWEAPON_TASK_CONDITION &data);
BOOL FromJsonDT_UPGRADEGODWEAPON_TASK_CONDITION(Json::Value &root, DT_UPGRADEGODWEAPON_TASK_CONDITION &data);
INT32 EncodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UPGRADEDRUG_TASK_CONDITION( DT_UPGRADEDRUG_TASK_CONDITION &data );
BOOL FromJsonDT_UPGRADEDRUG_TASK_CONDITION(const string &strData, DT_UPGRADEDRUG_TASK_CONDITION &data);
BOOL FromJsonDT_UPGRADEDRUG_TASK_CONDITION(Json::Value &root, DT_UPGRADEDRUG_TASK_CONDITION &data);
INT32 EncodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CHALLENGE_TASK_CONDITION( DT_CHALLENGE_TASK_CONDITION &data );
BOOL FromJsonDT_CHALLENGE_TASK_CONDITION(const string &strData, DT_CHALLENGE_TASK_CONDITION &data);
BOOL FromJsonDT_CHALLENGE_TASK_CONDITION(Json::Value &root, DT_CHALLENGE_TASK_CONDITION &data);
INT32 EncodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_TASK_CONDITION( DT_PLUNDER_TASK_CONDITION &data );
BOOL FromJsonDT_PLUNDER_TASK_CONDITION(const string &strData, DT_PLUNDER_TASK_CONDITION &data);
BOOL FromJsonDT_PLUNDER_TASK_CONDITION(Json::Value &root, DT_PLUNDER_TASK_CONDITION &data);
INT32 EncodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRESSSKILL_TASK_CONDITION( DT_DRESSSKILL_TASK_CONDITION &data );
BOOL FromJsonDT_DRESSSKILL_TASK_CONDITION(const string &strData, DT_DRESSSKILL_TASK_CONDITION &data);
BOOL FromJsonDT_DRESSSKILL_TASK_CONDITION(Json::Value &root, DT_DRESSSKILL_TASK_CONDITION &data);
INT32 EncodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRESSEQUIP_TASK_CONDITION( DT_DRESSEQUIP_TASK_CONDITION &data );
BOOL FromJsonDT_DRESSEQUIP_TASK_CONDITION(const string &strData, DT_DRESSEQUIP_TASK_CONDITION &data);
BOOL FromJsonDT_DRESSEQUIP_TASK_CONDITION(Json::Value &root, DT_DRESSEQUIP_TASK_CONDITION &data);
INT32 EncodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UPGRADESKILL_TASK_CONDITION( DT_UPGRADESKILL_TASK_CONDITION &data );
BOOL FromJsonDT_UPGRADESKILL_TASK_CONDITION(const string &strData, DT_UPGRADESKILL_TASK_CONDITION &data);
BOOL FromJsonDT_UPGRADESKILL_TASK_CONDITION(Json::Value &root, DT_UPGRADESKILL_TASK_CONDITION &data);
INT32 EncodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UPGRADEEQUIP_TASK_CONDITION( DT_UPGRADEEQUIP_TASK_CONDITION &data );
BOOL FromJsonDT_UPGRADEEQUIP_TASK_CONDITION(const string &strData, DT_UPGRADEEQUIP_TASK_CONDITION &data);
BOOL FromJsonDT_UPGRADEEQUIP_TASK_CONDITION(Json::Value &root, DT_UPGRADEEQUIP_TASK_CONDITION &data);
INT32 EncodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION( DT_USEEXPRIENCEDRUG_TASK_CONDITION &data );
BOOL FromJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION(const string &strData, DT_USEEXPRIENCEDRUG_TASK_CONDITION &data);
BOOL FromJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION(Json::Value &root, DT_USEEXPRIENCEDRUG_TASK_CONDITION &data);
INT32 EncodeDT_MULT_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MULT_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_MULT_TASK_CONDITION( DT_MULT_TASK_CONDITION &data );
BOOL FromJsonDT_MULT_TASK_CONDITION(const string &strData, DT_MULT_TASK_CONDITION &data);
BOOL FromJsonDT_MULT_TASK_CONDITION(Json::Value &root, DT_MULT_TASK_CONDITION &data);
INT32 EncodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_COMPOSEGOOD_TASK_CONDITION( DT_COMPOSEGOOD_TASK_CONDITION &data );
BOOL FromJsonDT_COMPOSEGOOD_TASK_CONDITION(const string &strData, DT_COMPOSEGOOD_TASK_CONDITION &data);
BOOL FromJsonDT_COMPOSEGOOD_TASK_CONDITION(Json::Value &root, DT_COMPOSEGOOD_TASK_CONDITION &data);
INT32 EncodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ELITEINSTANCE_TASK_CONDITION( DT_ELITEINSTANCE_TASK_CONDITION &data );
BOOL FromJsonDT_ELITEINSTANCE_TASK_CONDITION(const string &strData, DT_ELITEINSTANCE_TASK_CONDITION &data);
BOOL FromJsonDT_ELITEINSTANCE_TASK_CONDITION(Json::Value &root, DT_ELITEINSTANCE_TASK_CONDITION &data);
INT32 EncodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UPGRADSCIENCE_TASK_CONDITION( DT_UPGRADSCIENCE_TASK_CONDITION &data );
BOOL FromJsonDT_UPGRADSCIENCE_TASK_CONDITION(const string &strData, DT_UPGRADSCIENCE_TASK_CONDITION &data);
BOOL FromJsonDT_UPGRADSCIENCE_TASK_CONDITION(Json::Value &root, DT_UPGRADSCIENCE_TASK_CONDITION &data);
INT32 EncodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STRENGTHATTACK_TASK_CONDITION( DT_STRENGTHATTACK_TASK_CONDITION &data );
BOOL FromJsonDT_STRENGTHATTACK_TASK_CONDITION(const string &strData, DT_STRENGTHATTACK_TASK_CONDITION &data);
BOOL FromJsonDT_STRENGTHATTACK_TASK_CONDITION(Json::Value &root, DT_STRENGTHATTACK_TASK_CONDITION &data);
INT32 EncodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STRENGTHLIFE_TASK_CONDITION( DT_STRENGTHLIFE_TASK_CONDITION &data );
BOOL FromJsonDT_STRENGTHLIFE_TASK_CONDITION(const string &strData, DT_STRENGTHLIFE_TASK_CONDITION &data);
BOOL FromJsonDT_STRENGTHLIFE_TASK_CONDITION(Json::Value &root, DT_STRENGTHLIFE_TASK_CONDITION &data);
INT32 EncodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROP_TASK_CONDITION( DT_CROP_TASK_CONDITION &data );
BOOL FromJsonDT_CROP_TASK_CONDITION(const string &strData, DT_CROP_TASK_CONDITION &data);
BOOL FromJsonDT_CROP_TASK_CONDITION(Json::Value &root, DT_CROP_TASK_CONDITION &data);
INT32 EncodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TASK_DATA( DT_TASK_DATA &data );
BOOL FromJsonDT_TASK_DATA(const string &strData, DT_TASK_DATA &data);
BOOL FromJsonDT_TASK_DATA(Json::Value &root, DT_TASK_DATA &data);
INT32 EncodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TASK_DATA_LIST( DT_TASK_DATA_LIST &data );
BOOL FromJsonDT_TASK_DATA_LIST(const string &strData, DT_TASK_DATA_LIST &data);
BOOL FromJsonDT_TASK_DATA_LIST(Json::Value &root, DT_TASK_DATA_LIST &data);
INT32 EncodeDT_RACE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RACE_DATA( DT_RACE_DATA &data );
BOOL FromJsonDT_RACE_DATA(const string &strData, DT_RACE_DATA &data);
BOOL FromJsonDT_RACE_DATA(Json::Value &root, DT_RACE_DATA &data);
INT32 EncodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_LOCALE_HOME( DT_LOCALE_HOME &data );
BOOL FromJsonDT_LOCALE_HOME(const string &strData, DT_LOCALE_HOME &data);
BOOL FromJsonDT_LOCALE_HOME(Json::Value &root, DT_LOCALE_HOME &data);
INT32 EncodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_LOCALE_INSTANCE( DT_LOCALE_INSTANCE &data );
BOOL FromJsonDT_LOCALE_INSTANCE(const string &strData, DT_LOCALE_INSTANCE &data);
BOOL FromJsonDT_LOCALE_INSTANCE(Json::Value &root, DT_LOCALE_INSTANCE &data);
INT32 EncodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_LOCALE_ONHOOK( DT_LOCALE_ONHOOK &data );
BOOL FromJsonDT_LOCALE_ONHOOK(const string &strData, DT_LOCALE_ONHOOK &data);
BOOL FromJsonDT_LOCALE_ONHOOK(Json::Value &root, DT_LOCALE_ONHOOK &data);
INT32 EncodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_LOCALE_DATA( DT_LOCALE_DATA &data );
BOOL FromJsonDT_LOCALE_DATA(const string &strData, DT_LOCALE_DATA &data);
BOOL FromJsonDT_LOCALE_DATA(Json::Value &root, DT_LOCALE_DATA &data);
INT32 EncodeUnlockScienceInfo(void *pHost, CNetData* poNetData);
INT32 DecodeUnlockScienceInfo(void *pHost, CNetData* poNetData);
Json::Value GetJsonUnlockScienceInfo( UnlockScienceInfo &data );
BOOL FromJsonUnlockScienceInfo(const string &strData, UnlockScienceInfo &data);
BOOL FromJsonUnlockScienceInfo(Json::Value &root, UnlockScienceInfo &data);
INT32 EncodeDT_STATE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STATE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STATE_DATA( DT_STATE_DATA &data );
BOOL FromJsonDT_STATE_DATA(const string &strData, DT_STATE_DATA &data);
BOOL FromJsonDT_STATE_DATA(Json::Value &root, DT_STATE_DATA &data);
INT32 EncodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_MONSTER_BASE_DATA( DT_MONSTER_BASE_DATA &data );
BOOL FromJsonDT_MONSTER_BASE_DATA(const string &strData, DT_MONSTER_BASE_DATA &data);
BOOL FromJsonDT_MONSTER_BASE_DATA(Json::Value &root, DT_MONSTER_BASE_DATA &data);
INT32 EncodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_MONSTER_DATA_LIST( DT_MONSTER_DATA_LIST &data );
BOOL FromJsonDT_MONSTER_DATA_LIST(const string &strData, DT_MONSTER_DATA_LIST &data);
BOOL FromJsonDT_MONSTER_DATA_LIST(Json::Value &root, DT_MONSTER_DATA_LIST &data);
INT32 EncodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BATTLE_OBJ_DATA( DT_BATTLE_OBJ_DATA &data );
BOOL FromJsonDT_BATTLE_OBJ_DATA(const string &strData, DT_BATTLE_OBJ_DATA &data);
BOOL FromJsonDT_BATTLE_OBJ_DATA(Json::Value &root, DT_BATTLE_OBJ_DATA &data);
INT32 EncodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BATTLE_OBJ_GROUP_DATA( DT_BATTLE_OBJ_GROUP_DATA &data );
BOOL FromJsonDT_BATTLE_OBJ_GROUP_DATA(const string &strData, DT_BATTLE_OBJ_GROUP_DATA &data);
BOOL FromJsonDT_BATTLE_OBJ_GROUP_DATA(Json::Value &root, DT_BATTLE_OBJ_GROUP_DATA &data);
INT32 EncodeDT_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ITEM_DATA( DT_ITEM_DATA &data );
BOOL FromJsonDT_ITEM_DATA(const string &strData, DT_ITEM_DATA &data);
BOOL FromJsonDT_ITEM_DATA(Json::Value &root, DT_ITEM_DATA &data);
INT32 EncodeDT_BUFF_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUFF_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BUFF_DATA( DT_BUFF_DATA &data );
BOOL FromJsonDT_BUFF_DATA(const string &strData, DT_BUFF_DATA &data);
BOOL FromJsonDT_BUFF_DATA(Json::Value &root, DT_BUFF_DATA &data);
INT32 EncodeDT_HP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HP_DATA( DT_HP_DATA &data );
BOOL FromJsonDT_HP_DATA(const string &strData, DT_HP_DATA &data);
BOOL FromJsonDT_HP_DATA(Json::Value &root, DT_HP_DATA &data);
INT32 EncodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ANGRY_DATA( DT_ANGRY_DATA &data );
BOOL FromJsonDT_ANGRY_DATA(const string &strData, DT_ANGRY_DATA &data);
BOOL FromJsonDT_ANGRY_DATA(Json::Value &root, DT_ANGRY_DATA &data);
INT32 EncodeDT_PARRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PARRY_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PARRY_DATA( DT_PARRY_DATA &data );
BOOL FromJsonDT_PARRY_DATA(const string &strData, DT_PARRY_DATA &data);
BOOL FromJsonDT_PARRY_DATA(Json::Value &root, DT_PARRY_DATA &data);
INT32 EncodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SKILL_EFFECT_ID_DATA( DT_SKILL_EFFECT_ID_DATA &data );
BOOL FromJsonDT_SKILL_EFFECT_ID_DATA(const string &strData, DT_SKILL_EFFECT_ID_DATA &data);
BOOL FromJsonDT_SKILL_EFFECT_ID_DATA(Json::Value &root, DT_SKILL_EFFECT_ID_DATA &data);
INT32 EncodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ATTACK_OBJ_DATA( DT_ATTACK_OBJ_DATA &data );
BOOL FromJsonDT_ATTACK_OBJ_DATA(const string &strData, DT_ATTACK_OBJ_DATA &data);
BOOL FromJsonDT_ATTACK_OBJ_DATA(Json::Value &root, DT_ATTACK_OBJ_DATA &data);
INT32 EncodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ATTACK_DATA( DT_ATTACK_DATA &data );
BOOL FromJsonDT_ATTACK_DATA(const string &strData, DT_ATTACK_DATA &data);
BOOL FromJsonDT_ATTACK_DATA(Json::Value &root, DT_ATTACK_DATA &data);
INT32 EncodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ATTACK_ALL_DATA( DT_ATTACK_ALL_DATA &data );
BOOL FromJsonDT_ATTACK_ALL_DATA(const string &strData, DT_ATTACK_ALL_DATA &data);
BOOL FromJsonDT_ATTACK_ALL_DATA(Json::Value &root, DT_ATTACK_ALL_DATA &data);
INT32 EncodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BATTLE_ANGRY_DATA( DT_BATTLE_ANGRY_DATA &data );
BOOL FromJsonDT_BATTLE_ANGRY_DATA(const string &strData, DT_BATTLE_ANGRY_DATA &data);
BOOL FromJsonDT_BATTLE_ANGRY_DATA(Json::Value &root, DT_BATTLE_ANGRY_DATA &data);
INT32 EncodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BATTLE_ROUND_DATA( DT_BATTLE_ROUND_DATA &data );
BOOL FromJsonDT_BATTLE_ROUND_DATA(const string &strData, DT_BATTLE_ROUND_DATA &data);
BOOL FromJsonDT_BATTLE_ROUND_DATA(Json::Value &root, DT_BATTLE_ROUND_DATA &data);
INT32 EncodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BATTLE_DATA( DT_BATTLE_DATA &data );
BOOL FromJsonDT_BATTLE_DATA(const string &strData, DT_BATTLE_DATA &data);
BOOL FromJsonDT_BATTLE_DATA(Json::Value &root, DT_BATTLE_DATA &data);
INT32 EncodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ONLINE_ENCOURAGE_RECORD_DATA( DT_ONLINE_ENCOURAGE_RECORD_DATA &data );
BOOL FromJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(const string &strData, DT_ONLINE_ENCOURAGE_RECORD_DATA &data);
BOOL FromJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(Json::Value &root, DT_ONLINE_ENCOURAGE_RECORD_DATA &data);
INT32 EncodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_LOGIN_ENCOURAGE_RECORD_DATA( DT_LOGIN_ENCOURAGE_RECORD_DATA &data );
BOOL FromJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(const string &strData, DT_LOGIN_ENCOURAGE_RECORD_DATA &data);
BOOL FromJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(Json::Value &root, DT_LOGIN_ENCOURAGE_RECORD_DATA &data);
INT32 EncodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RACE_BATTLELOG_DATA( DT_RACE_BATTLELOG_DATA &data );
BOOL FromJsonDT_RACE_BATTLELOG_DATA(const string &strData, DT_RACE_BATTLELOG_DATA &data);
BOOL FromJsonDT_RACE_BATTLELOG_DATA(Json::Value &root, DT_RACE_BATTLELOG_DATA &data);
INT32 EncodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RACE_BATTLELOG_DATA_LIST( DT_RACE_BATTLELOG_DATA_LIST &data );
BOOL FromJsonDT_RACE_BATTLELOG_DATA_LIST(const string &strData, DT_RACE_BATTLELOG_DATA_LIST &data);
BOOL FromJsonDT_RACE_BATTLELOG_DATA_LIST(Json::Value &root, DT_RACE_BATTLELOG_DATA_LIST &data);
INT32 EncodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CAPTURE_DATA( DT_CAPTURE_DATA &data );
BOOL FromJsonDT_CAPTURE_DATA(const string &strData, DT_CAPTURE_DATA &data);
BOOL FromJsonDT_CAPTURE_DATA(Json::Value &root, DT_CAPTURE_DATA &data);
INT32 EncodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CAPTURE_DATA_LIST( DT_CAPTURE_DATA_LIST &data );
BOOL FromJsonDT_CAPTURE_DATA_LIST(const string &strData, DT_CAPTURE_DATA_LIST &data);
BOOL FromJsonDT_CAPTURE_DATA_LIST(Json::Value &root, DT_CAPTURE_DATA_LIST &data);
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_BATTLELOG_DATA( DT_PLUNDER_BATTLELOG_DATA &data );
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA(const string &strData, DT_PLUNDER_BATTLELOG_DATA &data);
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA(Json::Value &root, DT_PLUNDER_BATTLELOG_DATA &data);
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_BATTLELOG_DATA_LIST( DT_PLUNDER_BATTLELOG_DATA_LIST &data );
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA_LIST(const string &strData, DT_PLUNDER_BATTLELOG_DATA_LIST &data);
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA_LIST(Json::Value &root, DT_PLUNDER_BATTLELOG_DATA_LIST &data);
INT32 EncodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_GRID_DATA( DT_PLUNDER_GRID_DATA &data );
BOOL FromJsonDT_PLUNDER_GRID_DATA(const string &strData, DT_PLUNDER_GRID_DATA &data);
BOOL FromJsonDT_PLUNDER_GRID_DATA(Json::Value &root, DT_PLUNDER_GRID_DATA &data);
INT32 EncodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_BASE_DATA( DT_PLUNDER_BASE_DATA &data );
BOOL FromJsonDT_PLUNDER_BASE_DATA(const string &strData, DT_PLUNDER_BASE_DATA &data);
BOOL FromJsonDT_PLUNDER_BASE_DATA(Json::Value &root, DT_PLUNDER_BASE_DATA &data);
INT32 EncodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ENEMY_DATA( DT_ENEMY_DATA &data );
BOOL FromJsonDT_ENEMY_DATA(const string &strData, DT_ENEMY_DATA &data);
BOOL FromJsonDT_ENEMY_DATA(Json::Value &root, DT_ENEMY_DATA &data);
INT32 EncodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ENEMY_DATA_LIST( DT_ENEMY_DATA_LIST &data );
BOOL FromJsonDT_ENEMY_DATA_LIST(const string &strData, DT_ENEMY_DATA_LIST &data);
BOOL FromJsonDT_ENEMY_DATA_LIST(Json::Value &root, DT_ENEMY_DATA_LIST &data);
INT32 EncodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_MONEY_TREE_DATA( DT_MONEY_TREE_DATA &data );
BOOL FromJsonDT_MONEY_TREE_DATA(const string &strData, DT_MONEY_TREE_DATA &data);
BOOL FromJsonDT_MONEY_TREE_DATA(Json::Value &root, DT_MONEY_TREE_DATA &data);
INT32 EncodeDT_STUDY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STUDY_DATA( DT_STUDY_DATA &data );
BOOL FromJsonDT_STUDY_DATA(const string &strData, DT_STUDY_DATA &data);
BOOL FromJsonDT_STUDY_DATA(Json::Value &root, DT_STUDY_DATA &data);
INT32 EncodeDT_STUDY_CLIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_CLIP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STUDY_CLIP_DATA( DT_STUDY_CLIP_DATA &data );
BOOL FromJsonDT_STUDY_CLIP_DATA(const string &strData, DT_STUDY_CLIP_DATA &data);
BOOL FromJsonDT_STUDY_CLIP_DATA(Json::Value &root, DT_STUDY_CLIP_DATA &data);
INT32 EncodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_AWAKEN_DATA( DT_AWAKEN_DATA &data );
BOOL FromJsonDT_AWAKEN_DATA(const string &strData, DT_AWAKEN_DATA &data);
BOOL FromJsonDT_AWAKEN_DATA(Json::Value &root, DT_AWAKEN_DATA &data);
INT32 EncodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_INSTANCE_BATTLE_DATA( DT_INSTANCE_BATTLE_DATA &data );
BOOL FromJsonDT_INSTANCE_BATTLE_DATA(const string &strData, DT_INSTANCE_BATTLE_DATA &data);
BOOL FromJsonDT_INSTANCE_BATTLE_DATA(Json::Value &root, DT_INSTANCE_BATTLE_DATA &data);
INT32 EncodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PHYSTRENGTH_DATA( DT_PHYSTRENGTH_DATA &data );
BOOL FromJsonDT_PHYSTRENGTH_DATA(const string &strData, DT_PHYSTRENGTH_DATA &data);
BOOL FromJsonDT_PHYSTRENGTH_DATA(Json::Value &root, DT_PHYSTRENGTH_DATA &data);
INT32 EncodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ONHOOK_RESULT( DT_ONHOOK_RESULT &data );
BOOL FromJsonDT_ONHOOK_RESULT(const string &strData, DT_ONHOOK_RESULT &data);
BOOL FromJsonDT_ONHOOK_RESULT(Json::Value &root, DT_ONHOOK_RESULT &data);
INT32 EncodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ONHOOK_RCD_DATA( DT_ONHOOK_RCD_DATA &data );
BOOL FromJsonDT_ONHOOK_RCD_DATA(const string &strData, DT_ONHOOK_RCD_DATA &data);
BOOL FromJsonDT_ONHOOK_RCD_DATA(Json::Value &root, DT_ONHOOK_RCD_DATA &data);
INT32 EncodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FEW_PARAM_DATA( DT_FEW_PARAM_DATA &data );
BOOL FromJsonDT_FEW_PARAM_DATA(const string &strData, DT_FEW_PARAM_DATA &data);
BOOL FromJsonDT_FEW_PARAM_DATA(Json::Value &root, DT_FEW_PARAM_DATA &data);
INT32 EncodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROP_INFO_DATA( DT_CROP_INFO_DATA &data );
BOOL FromJsonDT_CROP_INFO_DATA(const string &strData, DT_CROP_INFO_DATA &data);
BOOL FromJsonDT_CROP_INFO_DATA(Json::Value &root, DT_CROP_INFO_DATA &data);
INT32 EncodeDT_CROP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROP_DATA( DT_CROP_DATA &data );
BOOL FromJsonDT_CROP_DATA(const string &strData, DT_CROP_DATA &data);
BOOL FromJsonDT_CROP_DATA(Json::Value &root, DT_CROP_DATA &data);
INT32 EncodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SCIENCE_TREE_DATA( DT_SCIENCE_TREE_DATA &data );
BOOL FromJsonDT_SCIENCE_TREE_DATA(const string &strData, DT_SCIENCE_TREE_DATA &data);
BOOL FromJsonDT_SCIENCE_TREE_DATA(Json::Value &root, DT_SCIENCE_TREE_DATA &data);
INT32 EncodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FORMATION_HERO_DATA( DT_FORMATION_HERO_DATA &data );
BOOL FromJsonDT_FORMATION_HERO_DATA(const string &strData, DT_FORMATION_HERO_DATA &data);
BOOL FromJsonDT_FORMATION_HERO_DATA(Json::Value &root, DT_FORMATION_HERO_DATA &data);
INT32 EncodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FORMATION_IDX_DATA( DT_FORMATION_IDX_DATA &data );
BOOL FromJsonDT_FORMATION_IDX_DATA(const string &strData, DT_FORMATION_IDX_DATA &data);
BOOL FromJsonDT_FORMATION_IDX_DATA(Json::Value &root, DT_FORMATION_IDX_DATA &data);
INT32 EncodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FORMATION_DATA_LIST( DT_FORMATION_DATA_LIST &data );
BOOL FromJsonDT_FORMATION_DATA_LIST(const string &strData, DT_FORMATION_DATA_LIST &data);
BOOL FromJsonDT_FORMATION_DATA_LIST(Json::Value &root, DT_FORMATION_DATA_LIST &data);
INT32 EncodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FORMATION_DATA( DT_FORMATION_DATA &data );
BOOL FromJsonDT_FORMATION_DATA(const string &strData, DT_FORMATION_DATA &data);
BOOL FromJsonDT_FORMATION_DATA(Json::Value &root, DT_FORMATION_DATA &data);
INT32 EncodeDT_VERSION_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VERSION_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_VERSION_DATA( DT_VERSION_DATA &data );
BOOL FromJsonDT_VERSION_DATA(const string &strData, DT_VERSION_DATA &data);
BOOL FromJsonDT_VERSION_DATA(Json::Value &root, DT_VERSION_DATA &data);
INT32 EncodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_NOTICE_DATA( DT_NOTICE_DATA &data );
BOOL FromJsonDT_NOTICE_DATA(const string &strData, DT_NOTICE_DATA &data);
BOOL FromJsonDT_NOTICE_DATA(Json::Value &root, DT_NOTICE_DATA &data);
INT32 EncodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLAYER_DATA( DT_PLAYER_DATA &data );
BOOL FromJsonDT_PLAYER_DATA(const string &strData, DT_PLAYER_DATA &data);
BOOL FromJsonDT_PLAYER_DATA(Json::Value &root, DT_PLAYER_DATA &data);
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
