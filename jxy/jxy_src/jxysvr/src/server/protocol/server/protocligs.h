/*       Version Number: 3          */

#ifndef PROTOCLIGS_H_INCLUDE_VERSION_3
#define PROTOCLIGS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define MAX_CHAT_CONTENT_NUM     512
#define MAX_CHAT_NUM             20
#define MAX_YABIAO_PLAYER_NUM    40
#define MAX_BUY_GOLD_NUM         20
#define MAX_RESOURCES_NUM        50
#define MAX_RECHARGE_ITEM_NUM    50
#define MAX_FLOWERS_ITEM_NUM     50
#define MAX_SCORE_ITEM_NUM       50
#define MAX_ACTIVITY_ENCOURAGE_ITEM_LEN 50
#define MAX_APP_ADDRESS_NUM      512
#define MAX_DOGFECES_BUILD_TIP_NUM 56
#define MAX_ACTIVITY_ENCOURAGE_NAME_LEN 256
#define MAX_ACTIVITY_ENCOURAGE_DESC_LEN 1024
#define MAX_ACTIVITY_ENCOURAGE_NUM 10
#define MAX_COMMON_TXT_NUM       256
#define MAX_SERIES_NO_NUM        256
#define MAX_FACTION_CLI_NUM      100
#define MAX_VIP_PRO_CONTEXT_LEN  2048
#define MAX_BUY_URL_LEN          256
#define NOTIN                    0
#define OWNER                    1
#define MAX_INCREASE             4
#define MAX_COMMON_TXT_LEN       1024
#define MAX_GEN_GAS_INFO_LEN     256
#define ITEM_EXCHANGE_ITEM_NUM   5
#define MAX_PRODUCTID_LEN        128
#define COIN                     1
#define GOLD                     2
#define STORY                    3
#define PHYSTRENGTH              4
#define SCIENCE                  5
#define BLUEGAS                  6
#define PURPLEGAS                7
#define JINGJIE                  8
#define EXPERIENCE               9
#define POWER                    10
#define UPGRADEEXPERIENCE        11
#define MAX_RES_VER_LEN          512
#define MAX_FLOWER_ITEM_NUM      20
#define MAX_EDIT_LEN             256
#define MAX_SOUL_CONTINUE_SEARCH 10
#define MAX_EXCHANGE_SOUL_NUM    9
#define MAX_SOUL_CARD_NAME_LENGTH 64
#define MAX_EXCHANGE_SOUL_METERIAL_NUM 5
#define MAX_EXCHANGE_SOUL_CARD_NUM 64
#define MAX_SOUL_PIC_LEN         64
#define MAX_SOUL_NAME_LEN        64
#define MAX_GVG_ERR_MSG_LEN      512
#define MAX_PRODUCTPET_EXTRA_ITEM_NUM 20
#define MAX_PRODUCTPET_NUM       10
#define MAX_STAR_DRAGON_ITEMTYPE_NUM 7
#define MAX_PET_STORE_ITEM_NUM   48
#define MAX_PET_RULE_LEN         6000
#define MAX_PET_BOOK_NUM         32
#define MAX_PET_SKILL_NUM        8
#define MAX_SOUL_POTENCY_LEVEL   5
#define MAX_SOUL_POTENCY_SUB_LEVEL 8
#define MAX_SOUL_POTENCY_ATTR_NUM 3
#define MAX_SOUL_POTENCY_ADD_ATTR_NUM 15
#define MAX_NEIDAN_INSTANCE_NUM  50
#define MAX_GROUP_ITEM_NUM       10
#define MAX_NEIDAN_MONSTER_NUM   20
#define PROTOCLIGS_MSGID_RANGE 3194
#define PROTOCLIGS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagDT_POWER_CHANGE_DATA{
	UINT32           dwOldPower; //原始战力
	UINT32           dwCurPower; //当前战力
}DT_POWER_CHANGE_DATA;

typedef struct tagDT_SKILL_BUFF_DATA{
	UINT8            byBuffKindID; //技能效果ID
	UINT8            byBuffRound; //buff持续回合数
	UINT8            byTriggerRate; //buff触发概率
	UINT16           wEffectParam; //效果参数
}DT_SKILL_BUFF_DATA;

typedef struct tagDT_SKILL_ACTION_DATA_CLI{
	UINT16           wActionID; //招式ID
	UINT16           wActionLevel; //招式等级
	UINT16           wEffectHurtRate; //伤害倍率
	UINT32           dwExtHurtValue; //附加伤害值
	UINT32           dwCurExp; //招式当前等级经验（除去之前等级经验，计算差值）
	UINT32           dwUpgradeExp; //招式当前等级升级需经验(除去之前等级经验，计算差值)
}DT_SKILL_ACTION_DATA_CLI;

typedef struct tagDT_SKILL_EFFECT_DATA_CLI{
	UINT16           wEffectID; //效果ID
	UINT8            byOnceEffectKindID; //单次技能效果ID
	UINT16           wOnceEffectParam; //单次效果参数
	UINT8            byBuffNum; //技能BUFF数
	DT_SKILL_BUFF_DATA astBuffInfo[MAX_SKILL_BUFF_NUM]; //技能BUFF信息
	UINT16           wUnlockLevel; //解锁所需招式等级
}DT_SKILL_EFFECT_DATA_CLI;

typedef struct tagDT_EFFECT_RECOMMEND_CLI{
	UINT16           wHeroID; //搭配伙伴ID
	UINT16           wRecommendEffectKindID; //搭配单效果ID(0表示没有)
	UINT8            byRecommendHeroRangKind; //搭配技能范围
	UINT8            byRecommendHeroRangBuff; //搭配技能范围追踪buff
}DT_EFFECT_RECOMMEND_CLI;

typedef struct tagDT_SKILL_EFFECT_DATA_CLI2{
	DT_SKILL_EFFECT_DATA_CLI stEffectInfo; //效果
	DT_EFFECT_RECOMMEND_CLI astRecommendInfo[2]; //推荐搭配
}DT_SKILL_EFFECT_DATA_CLI2;

typedef struct tagDT_ATTACK_RANG_DATA_CLI{
	DT_ATTACK_RANG_DATA stRangInfo; //技能范围
	UINT16           wUnlockLevel; //解锁所需招式等级
	UINT16           wAttackRangHurtRate; //攻击范围的伤害倍率（招式伤害基础上）
}DT_ATTACK_RANG_DATA_CLI;

typedef struct tagDT_SKILL_DATA_CLI{
	DT_SKILL_ACTION_DATA_CLI stActionInfo; //技能招式
	DT_SKILL_EFFECT_DATA_CLI stEffectInfo; //技能效果
	DT_ATTACK_RANG_DATA_CLI stRangInfo; //技能范围
}DT_SKILL_DATA_CLI;

typedef struct tagDT_HERO_BASE_DATA_CLI{
	UINT16           wKindID; //武将类型
	UINT8            byCareerID; //职业ID
	UINT8            byCoach; //主武将标志,1为主武将
	UINT8            byPos; //位置标志,0携带，1为出战，2在招募所中
	UINT8            byFormationIdx; //阵型位置
	UINT16           wLevel; //等级
	UINT16           wTalent; //资质(包括丹药)
	UINT32           dwExperience; //经验
	UINT32           dwUpgradeExperience; //升级经验
	UINT8            byRecuitedFlag; //是否招募过
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //战斗属性(非扩展属性，包括所有除了装备属性加成),若为布阵面板则所有属性加成
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttribute; //隐藏战斗属性
	UINT16           wGodweaponQuality; //神器品质(0为没有神器)
	UINT16           wGodweaponLevel; //神器等级
	UINT32           dwPower; //战力
	UINT8            byMaxAbsorbNum; //最多可吸收个数
	UINT8            byAbsorbNum; //当前吸收次数数
	UINT16           awKindIDLst[MAX_ABSORB_NUM]; //武将信息
	UINT8            byColor; //武将颜色信息
	UINT16           wAbsorbKindID; //当前武将被吸收的武将ID
}DT_HERO_BASE_DATA_CLI;

typedef struct tagDT_EQUIP_DATA_STRENGTHEN_CLI{
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT8            byCanStrengthenFlag; //是否可以强化，1可以强化，0不可强化
	UINT32           dwStrengthenCoin; //强化金币
	UINT32           dwStrengthenGold; //强化元宝
	UINT16           wStrengthenRate; //强化成功率
	DT_BATTLE_ATTRIBUTE stNextBattleAttribute; //下一级战斗属性
}DT_EQUIP_DATA_STRENGTHEN_CLI;

typedef struct tagDT_EQUIP_XIANGQIAN_DES{
	TCHAR            aszAttrType[MAX_EQUIP_XIANGQIAN_NUM]; //描述
}DT_EQUIP_XIANGQIAN_DES;

typedef struct tagDT_ALL_EQUIP_XIANGQIAN_DES{
	TCHAR            aszEquipPos1Des[MAX_EQUIP_XIANGQIAN_NUM]; //描述
	TCHAR            aszEquipPos2Des[MAX_EQUIP_XIANGQIAN_NUM]; //描述
	TCHAR            aszEquipPos3Des[MAX_EQUIP_XIANGQIAN_NUM]; //描述
	TCHAR            aszEquipPos4Des[MAX_EQUIP_XIANGQIAN_NUM]; //描述
}DT_ALL_EQUIP_XIANGQIAN_DES;

typedef struct tagDT_XIANGQIAN_ATTR_DATA{
	UINT16           wKindID; //宝石ID
	UINT32           dwAttrType; //属性类型
	UINT32           dwAddValue; //增加的值
	UINT32           dwAddPercentValue; //增加的百分比
}DT_XIANGQIAN_ATTR_DATA;

typedef struct tagDT_EQUIP_DATA_CLI{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT8            byDressPos; //穿戴位置
	UINT16           wLevel; //装备等级
	UINT16           wDressLevel; //装备穿戴等级
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //当前战斗属性
	UINT32           dwRecoverPrice; //回收价格
	UINT32           dwMinScience; //回收器魂
	UINT32           dwMaxScience; //回收器魂
	UINT8            byXiangqianNum; //镶嵌数量
	DT_XIANGQIAN_ATTR_DATA astXiangqianList[MAX_XIANGQIAN_NUM]; //镶嵌列表
}DT_EQUIP_DATA_CLI;

typedef struct tagDT_EQUIP_MELT_DATA_CLI{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT8            byDressPos; //穿戴位置
	UINT16           wLevel; //装备等级
	UINT16           wDressLevel; //装备穿戴等级
	UINT32           dwMinScience; //回收器魂
	UINT32           dwMaxScience; //回收器魂
	UINT8            byMeltTip; //熔炼标识：0可熔炼、1该装备镶嵌了仙石，不能进行熔炼 2该装备已被附魔，无法投入熔炼炉，只能通过出售方式回收
	UINT32           dwMeltGold; //元宝熔炼所需元宝
}DT_EQUIP_MELT_DATA_CLI;

typedef struct tagDT_GOOD_MELT_DATA_CLI{
	UINT16           wKindID; //物品ID
	UINT16           wNum; //物品个数
	UINT32           dwMinScience; //回收器魂
	UINT32           dwMaxScience; //回收器魂
	UINT32           dwMeltGold; //元宝熔炼所需元宝
}DT_GOOD_MELT_DATA_CLI;

typedef struct tagDT_EQUIP_DATA_LST_CLI{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA_CLI astEquipList[MAX_BAG_OPEN_NUM]; //装备信息
}DT_EQUIP_DATA_LST_CLI;

typedef struct tagDT_EQUIP_MELT_DATA_LST_CLI{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_MELT_DATA_CLI astEquipList[MAX_BAG_OPEN_NUM]; //装备信息
}DT_EQUIP_MELT_DATA_LST_CLI;

typedef struct tagDT_GOOD_MELT_DATA_LST_CLI{
	UINT16           wGoodNum; //装备个数
	DT_GOOD_MELT_DATA_CLI astGoodList[MAX_BAG_OPEN_NUM]; //装备信息
}DT_GOOD_MELT_DATA_LST_CLI;

typedef struct tagDT_HERO_EQUIP_DATA_LST_CLI{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA_CLI astEquipList[EQUIP_POS_NUM]; //装备信息
}DT_HERO_EQUIP_DATA_LST_CLI;

typedef struct tagDT_EQUIP_DATA_CLI_v410{
	DT_EQUIP_DATA_CLI stEquipData; //装备类型（里面的当前战斗属性为总属性，对于新版本来说是废弃不用的）
	DT_BATTLE_ATTRIBUTE2 stBasicAttr; //装备本身基础属性
	DT_BATTLE_ATTRIBUTE2 stStrengthenAttr; //强化附加属性
	DT_BATTLE_ATTRIBUTE2 stEnchantAttr; //附魔附加属性
	DT_BATTLE_ATTRIBUTE2 stTotalAttr; //总属性
	UINT16           wEnchantLevel; //附魔等级
}DT_EQUIP_DATA_CLI_v410;

typedef struct tagDT_HERO_EQUIP_DATA_LST_CLI_v410{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA_CLI_v410 astEquipList[EQUIP_POS_NUM]; //装备信息
}DT_HERO_EQUIP_DATA_LST_CLI_v410;

typedef struct tagDT_NEED_ITEM_DATA{
	UINT16           wItemKindID; //所需道具ID
	UINT8            byNum; //所需个数
	UINT16           wCurNum; //当前个数
	DT_INSTANCE_ID   stBurstInstance; //可能掉落的副本
}DT_NEED_ITEM_DATA;

typedef struct tagDT_ESTIMATE_EQUIP_DATA{
	UINT16           wKindID; //装备类型
	UINT16           wLevel; //装备等级
	UINT16           wDressLevel; //装备穿戴等级
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //当前战斗属性
}DT_ESTIMATE_EQUIP_DATA;

typedef struct tagDT_ESTIMATE_DRUG_DATA{
	UINT16           wKindID; //物品类型
	UINT16           wBaseTalent; //修为基础加成值
	UINT16           wDecTalent; //每多使用1个修为加成递减值
}DT_ESTIMATE_DRUG_DATA;

typedef union tagDT_ESTIMATE_ITEM_DATA_UNOIN{
	DT_ESTIMATE_EQUIP_DATA stEquip; //战力变化信息
	DT_ESTIMATE_DRUG_DATA stDrug; //战力变化信息
}DT_ESTIMATE_ITEM_DATA_UNOIN;

typedef struct tagDT_ESTIMATE_ITEM_DATA{
	UINT8            byItemType; //道具类型
	DT_ESTIMATE_ITEM_DATA_UNOIN stItemInfo; //道具类型
}DT_ESTIMATE_ITEM_DATA;

typedef struct tagDT_EQUIP_COMPOSE_DATA_CLI{
	UINT8            byItemNum; //所需道具个数(0表示不可合成)
	DT_NEED_ITEM_DATA astItemInfoList[MAX_ITEM_MAKER_NEED_NUM]; //所需道具信息
	DT_ESTIMATE_EQUIP_DATA stEstimateEquip; //预计合成的装备信息
	UINT8            bySuccessRate; //合成成功率
	UINT32           dwSuccessGold; //100%成功率所需元宝
}DT_EQUIP_COMPOSE_DATA_CLI;

typedef struct tagDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA_CLI astEquipInfo[MAX_BAG_OPEN_NUM]; //装备信息
	DT_EQUIP_DATA_STRENGTHEN_CLI astEquipStrengthenInfo[MAX_BAG_OPEN_NUM]; //装备强化信息
}DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI;

typedef struct tagDT_SEED_DATA_CLI{
	UINT8            byType; //种子类型
	UINT32           dwValue; //增加值
	UINT16           wKindID; //获取的物品ID
}DT_SEED_DATA_CLI;

typedef struct tagDT_GOODS_DATA_CLI{
	UINT16           wKindID; //物品类型
	UINT8            byGoodsMainKindID; //物品大类
	UINT16           wUseLevel; //使用等级（若为宝石，则表示宝石等级）
	UINT16           wPileCount; //叠加数
	UINT32           dwRecoverPrice; //回收价格
	UINT32           dwParam1; //参数1（若为宝石，则表示宝石类型）
	UINT32           dwParam2; //参数2
	UINT32           dwParam3; //参数3
	UINT32           dwValue; //使用效果值(不同物品不同含义)
	UINT16           wComposeCount; //物品合成个数
	UINT16           wComposeKindID; //物品合成ID
	UINT8            byComposeItemNum; //材料合成的道具/礼包中的道具
	UINT16           awComposeItem[MAX_COMPOSE_ITEM_NUM]; //道具信息
	UINT8            bySeedFlag; //种子标志
	DT_SEED_DATA_CLI astSeedData[1]; //道具信息
	UINT32           dwParam4; //参数4(若为宝石，则表示可转换的蓝气)
}DT_GOODS_DATA_CLI;

typedef struct tagDT_GOODS_DATA_LST_CLI{
	UINT16           wGoodsNum; //装备个数
	DT_GOODS_DATA_CLI astGoodsList[MAX_BAG_OPEN_NUM]; //装备信息
}DT_GOODS_DATA_LST_CLI;

typedef struct tagDT_ITEM_DATA_CLI{
	UINT16           wKindID; //类型
	UINT8            byGoodsMainKindID; //物品主类型，只对物品类型道具有效
	UINT16           wPileNum; //叠加数
}DT_ITEM_DATA_CLI;

typedef struct tagDT_ITEM_DATA_LIST_CLI{
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI astItemList[MAX_BAG_OPEN_NUM]; //道具信息
}DT_ITEM_DATA_LIST_CLI;

typedef union tagDT_ITEM_DATA_UNOIN{
	DT_EQUIP_DATA_CLI stEquip; //战力变化信息
	DT_GOODS_DATA_CLI stGoods; //战力变化信息
}DT_ITEM_DATA_UNOIN;

typedef struct tagDT_ITEM_DATA_CLI2{
	UINT8            byItemType; //道具类型
	DT_ITEM_DATA_UNOIN stItemInfo; //道具类型
}DT_ITEM_DATA_CLI2;

typedef struct tagDT_PHYSTRENGTH_DATA_CLI{
	UINT16           wPhyStrength; //当前体力
	UINT16           wMaxPhyStrength; //本级体力
	UINT16           wPhyStrengthRefreshCD; //体力刷新间隔,单位s
	UINT16           wPhyStrengthCountDown; //体力刷新倒计时
}DT_PHYSTRENGTH_DATA_CLI;

typedef struct tagDT_NEW_EQUIP_PROMPT_DATA{
	UINT8            byEquipNum; //装备个数
	UINT16           awEquipIDInfo[EQUIP_POS_NUM]; //装备类型信息
	UINT16           wHeroID; //可提升的第一个伙伴(装备个数大于0有效)，0表示没有
}DT_NEW_EQUIP_PROMPT_DATA;

typedef struct tagDT_FUNC_COMMON_PROMPT{
	UINT8            byBuildKindID; //建筑类型
	UINT8            byFuncFlag; //建筑功能(0不需提示，>0不同的值不同建筑表示不同功能)
	UINT32           dwFuncCountDown; //功能的倒计时
	UINT8            byExt; //扩展字段
	UINT16           wAheadTime; //提前开放时间(单位秒)
}DT_FUNC_COMMON_PROMPT;

typedef struct tagDT_FUNC_PROMPT_DATA{
	UINT8            byBuildNum; //建筑个数
	DT_FUNC_COMMON_PROMPT astBuildPromptInfo[MAX_BUILD_NUM]; //建筑提示信息
}DT_FUNC_PROMPT_DATA;

typedef struct tagDT_OTHER_PROMPT_DATA{
	UINT8            byHaveBetterHeroFlag; //是否有更强伙伴可招募，1有，0没有
}DT_OTHER_PROMPT_DATA;

typedef struct tagDT_RES_ITEM_DATA{
	UINT8            byResNum; //资源个数
	DT_RES_DATA      astResList[MAX_PLAYER_RES_NUM]; //资源信息
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI2 astItemList[MAX_PLAYER_RES_NUM]; //道具信息
}DT_RES_ITEM_DATA;

typedef struct tagDT_RSYNC_RES_ITEM_DATA{
	UINT8            byResNum; //资源个数
	DT_RES_DATA      astResList[MAX_PLAYER_RES_NUM]; //资源信息
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI2 astItemList[MAX_PLAYER_RES_NUM]; //道具信息
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}DT_RSYNC_RES_ITEM_DATA;

typedef struct tagDT_ITEM_DATA_LIST_CLI2{
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI2 astItemList[MAX_BAG_OPEN_NUM]; //道具信息
}DT_ITEM_DATA_LIST_CLI2;

typedef struct tagDT_TASK_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwGold; //元宝
	UINT16           wPhyStrength; //体力
	UINT32           dwScience; //器魂点
	UINT32           dwExperience; //经验
	UINT32           dwStory; //阅历
	UINT32           dwBlueGas; //蓝气
	UINT32           dwPurpleGas; //紫气
	UINT32           dwJingJie; //紫气
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI astItemInfoList[MAX_ENCOURAGE_ITEM_KIND_NUM]; //道具信息
}DT_TASK_ENCOURAGE_DATA;

typedef struct tagDT_TASK_BASE_DATA{
	TCHAR            aszTaskDispName[MAX_TASK_DSPNAME_LEN]; //任务显示名称
	TCHAR            aszTaskDesc[MAX_TASK_DESC_LEN]; //任务简介
	UINT8            byTaskType; //任务类型
	DT_TASK_CONDITION stTaskConditionInfo; //任务条件
	DT_TASK_ENCOURAGE_DATA stTaskEncourage; //任务奖励
}DT_TASK_BASE_DATA;

typedef struct tagDT_GIVEN_TASK_DATA{
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
	UINT64           qwStateUpdateTime; //状态更新时间
	DT_TASK_BASE_DATA stTaskBaseData; //任务基本信息
	UINT8            byCanAccept; //当前是否可接
	UINT16           wPlayerLevel; //玩家等级(0表示不需要)
	DT_INSTANCE_ID   stInstanceID; //通过制定副本(SceneIdx为0表示不需要)
}DT_GIVEN_TASK_DATA;

typedef struct tagDT_ACCEPT_TASK_DATA{
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
	UINT64           qwStateUpdateTime; //状态更新时间
	DT_TASK_BASE_DATA stTaskBaseData; //任务基本信息
}DT_ACCEPT_TASK_DATA;

typedef struct tagDT_ENCOURAGE_TASK_DATA{
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
	UINT64           qwStateUpdateTime; //状态更新时间
	UINT8            byHaveBaseData; //是否有任务基本信息(用于接受时马上完成的任务)
	DT_TASK_BASE_DATA astTaskBaseData[1]; //任务基本信息
}DT_ENCOURAGE_TASK_DATA;

typedef struct tagDT_INVALID_TASK_DATA{
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
}DT_INVALID_TASK_DATA;

typedef union tagDT_TASK_UNION{
	DT_GIVEN_TASK_DATA stGivenTask; //战力变化信息
	DT_ACCEPT_TASK_DATA stAcceptTask; //战力变化信息
	DT_ENCOURAGE_TASK_DATA stEncourageTask; //战力变化信息
	DT_INVALID_TASK_DATA stInvalidTask; //战力变化信息
}DT_TASK_UNION;

typedef struct tagDT_TASK_DATA_CLI{
	UINT8            byTaskState; //任务状态
	DT_TASK_UNION    stTaskInfo; //任务条件
}DT_TASK_DATA_CLI;

typedef struct tagDT_TASK_DATA_LIST_CLI{
	UINT8            byTaskNum; //任务数(每线1个)
	DT_TASK_DATA_CLI astTaskInfoList[MAX_TASK_LINE_NUM]; //任务信息
}DT_TASK_DATA_LIST_CLI;

typedef struct tagDT_ACCEPT_TASK_COND_RECORD{
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
	UINT8            byTaskType; //任务类型
	DT_TASK_CONDITION stTaskCondRecord; //任务完成条件记录
}DT_ACCEPT_TASK_COND_RECORD;

typedef struct tagDT_TASK_COND_RECORD_LIST{
	UINT8            byTaskNum; //任务数(每线1个)
	DT_ACCEPT_TASK_COND_RECORD astTaskConRecordList[MAX_TASK_LINE_NUM]; //任务信息
}DT_TASK_COND_RECORD_LIST;

typedef struct tagDT_EXPERIENCE_DATA{
	UINT32           dwExperience; //经验(比本级经验多出的经验)
	UINT32           dwUpgradeExperience; //升级经验
}DT_EXPERIENCE_DATA;

typedef struct tagDT_PLAYER_BASE_DATA_CLI{
	UINT32           dwID; //玩家唯一ID
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
	DT_PHYSTRENGTH_DATA_CLI stPhyStrengthInfo; //体力信息
	UINT16           wLevel; //等级
	DT_EXPERIENCE_DATA stExperienceInfo; //经验信息
	UINT64           qwScience; //当前器魂点
	UINT32           dwPower; //战力
	DT_LOCALE_DATA   stLocaleData; //玩家当前位置信息
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息，每一字节表示一个引导，0表示不需要，1需要，2已完成
	UINT8            byVipLevel; //VIP等级
}DT_PLAYER_BASE_DATA_CLI;

typedef struct tagDT_BUILD_DATA_CLI{
	UINT8            byKindID; //建筑类型
	UINT16           wLevel; //等级
	UINT8            bySort; //建筑排序
	UINT8            byHaveOpenFlag; //建筑是否打开过，1为打开过
}DT_BUILD_DATA_CLI;

typedef struct tagDT_ONLINE_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwGold; //元宝
	UINT32           dwScience; //器魂点
	UINT32           dwExperience; //经验
	UINT16           wPhyStrength; //体力
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI astItemInfoList[MAX_ENCOURAGE_ITEM_KIND_NUM]; //道具信息
}DT_ONLINE_ENCOURAGE_DATA;

typedef struct tagDT_DATA_AFTER_ONLINE_ENCOURAGE{
	UINT64           qwCurCoin; //当前玩家铜钱
	UINT64           qwCurGold; //当前玩家元宝
	UINT64           qwCurScience; //当前玩家器魂点
	UINT16           wPhyStrength; //当前玩家体力
}DT_DATA_AFTER_ONLINE_ENCOURAGE;

typedef struct tagDT_VIP_NUM_FUNC_DATA{
	UINT8            byNumFuncKind; //功能类型
	UINT32           dwCommonNum; //功能普通次数
	UINT32           dwIncNum; //当前vip等级提升功能的次数
}DT_VIP_NUM_FUNC_DATA;

typedef struct tagDT_VIP_LEVEL_DATA{
	UINT8            byVipLevel; //VIP等级
	UINT32           dwNeedRechargeGold; //所需充值总元宝
	UINT8            byExtNumFuncNum; //当前等级提升功能次数的功能数
	DT_VIP_NUM_FUNC_DATA astVipNumFuncInfo[MAX_VIP_FUNC]; //当前等级提升功能次数的功能信息
	UINT8            byOpenVipFuncNum; //当前等级开放的vip功能数
	UINT8            abyOpenVipFuncInfo[MAX_VIP_FUNC]; //当前等级开放的vip功能信息
}DT_VIP_LEVEL_DATA;

typedef struct tagDT_VIP_TAB_DATA_CLI{
	UINT8            byCurVipLevel; //当前VIP等级
	UINT32           dwHaveRechargeGold; //已经充值总元宝
	UINT8            byVipLevelNum; //VIP等级数
	DT_VIP_LEVEL_DATA astVipLevelInfo[MAX_VIP_LEVEL]; //VIP等级信息
}DT_VIP_TAB_DATA_CLI;

typedef struct tagDT_UNREAD_BATTLE_LOG_DATA_CLI{
	UINT8            byBattleLogType; //战报类型
	UINT8            byUnreadNum; //未读个数
}DT_UNREAD_BATTLE_LOG_DATA_CLI;

typedef struct tagDT_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwGold; //元宝
	UINT16           wPhyStrength; //体力
	UINT32           dwScience; //器魂点
	UINT32           dwStory; //阅历
	UINT32           dwBlueGas; //蓝气
	UINT32           dwPurpleGas; //紫气
	UINT32           dwJingJie; //紫气
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI2 astItemInfoList[MAX_ENCOURAGE_ITEM_KIND_NUM]; //道具信息
}DT_ENCOURAGE_DATA;

typedef struct tagDT_RSYNC_RECV_ENCOURAGE_DATA{
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurGold; //玩家当前的元宝
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurStory; //玩家当前的阅历
	UINT64           qwCurBlueGas; //玩家当前的蓝气
	UINT64           qwCurPurpleGas; //玩家当前的紫气
	UINT64           qwCurJingJie; //玩家当前的境界
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}DT_RSYNC_RECV_ENCOURAGE_DATA;

typedef struct tagPKT_CLIGS_ENTERGS_REQ{
	tagPKT_CLIGS_ENTERGS_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTERGS_REQ)); }
	UINT16           wZoneID; //区ID
	TCHAR            aszDeviceID[DEVICEID_LEN]; //区ID
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //区ID
	TCHAR            aszUserName[USERNAME_LEN]; //区ID
	UINT32           dwPlayerID; //玩家ID，老用户登录有效
	UINT16           wCareerID; //职业，新游戏有效
	UINT32           dwCliVer; //客户端版本
}PKT_CLIGS_ENTERGS_REQ;

typedef struct tagDT_OPERATING_ACTIVITY_DATA{
	UINT8            byNum; //活动数
	UINT32           dwVersionID; //活动版本序号
	TCHAR            aszDesc[MAX_COMMON_TXT_LEN]; //描述
	TCHAR            aszUrl[MAX_COMMON_TXT_LEN]; //Url
	UINT8            byWebTabType; //web面板类型
	TCHAR            aszExtContext[MAX_COMMON_TXT_LEN]; //扩展信息
}DT_OPERATING_ACTIVITY_DATA;

typedef struct tagPKT_CLIGS_ENTERGS_ACK{
	tagPKT_CLIGS_ENTERGS_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTERGS_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCliVer; //客户端版本
	UINT32           dwResVer; //资源版本
	UINT32           dwUIVer; //UI版本
	DT_PLAYER_BASE_DATA_CLI stPlayerInfo; //玩家信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	UINT16           wHeroNum; //武将数目
	DT_HERO_BASE_DATA_CLI astHeroInfoLst[MAX_HERO_NUM]; //武将信息
	UINT16           wBuildNum; //建筑数目
	DT_BUILD_DATA_CLI astBuildInfo[MAX_BUILD_NUM]; //建筑信息
	UINT8            byOnlineEncourageFlag; //是否存在在线奖励信息
	DT_ONLINE_ENCOURAGE_DATA astOnlineEncourage[1]; //当前在线奖励信息
	UINT16           awRecvCountDown[1]; //领取在线奖励倒计时，单位秒
	UINT8            byBattleLogTypeNum; //战报类型个数
	DT_UNREAD_BATTLE_LOG_DATA_CLI astUnreadBattleLogInfo[MAX_BATTLE_LOG_TYPE_NUM]; //未读战报信息
	UINT16           wJuniorLevel; //新手等级
	DT_VIP_TAB_DATA_CLI stVipTabInfo; //VIP面板信息
	DT_EXT_FUNC_PARAM_LST stExtFuncParamLst; //扩展功能参数列表
	UINT64           qwServerTime; //服务器时间
	UINT16           wCanUseBoxGoodsID; //可使用礼包物品ID,0为没有
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_NOTICE_DATA   stNoticeInfo; //公告信息
	DT_OPERATING_ACTIVITY_DATA stOperatingActivityInfo; //运营活动信息
	UINT16           wUnreadOfflineMsgNum; //未读离线私聊消息总数
	TCHAR            aszResVer[MAX_RES_VER_LEN]; //版本信息
	UINT8            byHaveFaction; //是否有门派0 没有门派， 1有门派
	UINT8            byHaveVipGift; //是否有VIP礼包0 没有礼包， 1有礼包
	UINT8            byEditDspName; //1：是否可修改泥称
	TCHAR            aszEditDspName[MAX_EDIT_LEN]; //修改名称描述信息
	DT_NOTICE_DATA_NEW stNoticeNewInfo; //公告信息
	UINT8            byIsFly; //主角是否已飞升
}PKT_CLIGS_ENTERGS_ACK;

typedef struct tagDT_BAG_DATA_CLI{
	UINT8            byMaxOpenNum; //最大格子数
	UINT8            byInitOpenNum; //初始化打开格子数
	UINT8            byCurOpenNum; //当前打开格子数(包括初始值)
	UINT32           dwBaseOpenCoin; //初始打开格子需要金币，二选1
	UINT32           dwOpenCoinIncrease; //每多打开1格子递增金币
	UINT32           dwBaseOpenGold; //初始打开格子需要元宝，二选1
	UINT32           dwOpenGoldIncrease; //每多打开1格子递增元宝
}DT_BAG_DATA_CLI;

typedef struct tagDT_BAG_DATA_CLI2{
	UINT16           wMaxOpenNum; //最大格子数
	UINT16           wInitOpenNum; //初始化打开格子数
	UINT16           wCurOpenNum; //当前打开格子数(包括初始值)
	UINT32           dwBaseOpenCoin; //初始打开格子需要金币，二选1
	UINT32           dwOpenCoinIncrease; //每多打开1格子递增金币
	UINT32           dwBaseOpenGold; //初始打开格子需要元宝，二选1
	UINT32           dwOpenGoldIncrease; //每多打开1格子递增元宝
}DT_BAG_DATA_CLI2;

typedef struct tagDT_EQUIP_BAG_DATA_CLI{
	DT_BAG_DATA_CLI  stBagInfo; //背包基本信息
	DT_EQUIP_DATA_LST_CLI stBagEquipDataList; //玩家背包装备信息
}DT_EQUIP_BAG_DATA_CLI;

typedef struct tagPKT_CLIGS_GET_HERO_INFO_REQ{
	tagPKT_CLIGS_GET_HERO_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_HERO_INFO_REQ)); }
	UINT32           dwPlayerID; //玩家ID
	UINT8            byEquipBagFlag; //是否返装备背包信息
}PKT_CLIGS_GET_HERO_INFO_REQ;

typedef struct tagDT_ATTR_ENABLE_UPDATE_INFO{
	UINT8            byEnableAttrHPUpdate; //HP属性升级
	UINT8            byEnableAttrAttackUpdate; //攻击属性升级
}DT_ATTR_ENABLE_UPDATE_INFO;

typedef struct tagDT_HERO_DATA_CLI{
	UINT16           wHeroNum; //武将数目
	DT_HERO_BASE_DATA_CLI astHeroBaseData[MAX_HERO_NUM]; //武将基本信息
	DT_HERO_EQUIP_DATA_LST_CLI astEquipDataList[MAX_HERO_NUM]; //武将装备信息
	DT_ATTR_ENABLE_UPDATE_INFO astEnableAttrUpdate[MAX_HERO_NUM]; //属性升级信息
	DT_SKILL_DATA_CLI astSkillInfo[MAX_HERO_NUM]; //武将技能信息
}DT_HERO_DATA_CLI;

typedef struct tagDT_PLAYER_BATTLE_INFO{
	TCHAR            aszDisplayName[USERNAME_LEN]; //玩家姓名
	UINT32           dwCurPower; //战力
	UINT32           dwRank; //玩家武斗场排名
}DT_PLAYER_BATTLE_INFO;

typedef struct tagPKT_CLIGS_GET_HERO_INFO_ACK{
	tagPKT_CLIGS_GET_HERO_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_HERO_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwPlayerID; //玩家ID
	UINT16           wCoachExpRate; //主角经验倍数(百分比)
	DT_PLAYER_BATTLE_INFO stPlayerBattleInfo; //玩家战斗信息
	DT_HERO_DATA_CLI stHeroInfo; //武将信息
	UINT8            byEquipBagFlag; //是否返装备背包信息
	DT_EQUIP_BAG_DATA_CLI astEquipBagDataInfo[1]; //未装备的装备信息
	UINT8            byGoodsCellNum; //物品占用格子数
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
	UINT8            byVipLevel; //VIP等级
}PKT_CLIGS_GET_HERO_INFO_ACK;

typedef struct tagDT_ACTIVI_SIDE_PROMPT_DATA{
	UINT8            byActiviID; //功能ID编号
	UINT32           dwActiviCountDown; //功能倒计时
}DT_ACTIVI_SIDE_PROMPT_DATA;

typedef struct tagDT_ACTIVI_SIDE_PROMPT_DATA_LST{
	UINT8            byActiviNum; //功能提示个数
	DT_ACTIVI_SIDE_PROMPT_DATA astActiviPrompt[MAX_ACTIVI_SIDE_PROMPT_NUM]; //提示内容
}DT_ACTIVI_SIDE_PROMPT_DATA_LST;

typedef struct tagPKT_CLIGS_ENTER_HOME_REQ{
	tagPKT_CLIGS_ENTER_HOME_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTER_HOME_REQ)); }
	UINT8            byKind; //进入家园类型(客户端决定，原样返回)
}PKT_CLIGS_ENTER_HOME_REQ;

typedef struct tagPKT_CLIGS_ENTER_HOME_ACK{
	tagPKT_CLIGS_ENTER_HOME_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTER_HOME_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byKind; //进入家园类型(客户端决定，原样返回)
	UINT16           wBuildNum; //建筑数目
	DT_BUILD_DATA_CLI astBuildInfo[MAX_BUILD_NUM]; //建筑信息
	UINT8            byLoginEncourageState; //登录奖励状态(0,不需显示，1需显示，2需弹出)
	UINT8            byBattleLogTypeNum; //战报类型个数
	DT_UNREAD_BATTLE_LOG_DATA_CLI astUnreadBattleLogInfo[MAX_BATTLE_LOG_TYPE_NUM]; //未读战报信息
	UINT16           awUnlockHeroID[MAX_UNLOCK_HERO_NUM]; //解锁伙伴提示(0表示没有)
	UINT16           wUnlockEquipLevel; //解锁装备提示(0表示没有)
	UINT32           dwUnlockElite; //解锁精英副本提示(0表示没有，第1个字节表示精英副本城镇idx，第2/3个字节表示主怪物ID)
	UnlockScienceInfo stUnlockScienceList; //解锁科技提示
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_OTHER_PROMPT_DATA stOtherPromptInfo; //其他提示信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	UINT16           wEliteUnBattleNum; //未打的精英副本个数
	UINT8            byHaveFactionState; //是否有门派
	DT_ACTIVI_SIDE_PROMPT_DATA_LST stActiveSidePrompt; //侧面功能提示
}PKT_CLIGS_ENTER_HOME_ACK;

typedef struct tagDT_BATTLE_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwExperience; //经验
	UINT32           dwStory; //阅历
	UINT32           dwJingJie; //境界
}DT_BATTLE_ENCOURAGE_DATA;

typedef struct tagDT_DATA_AFTER_BATTLE_ENCOURAGE{
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurStory; //当前阅历
	UINT64           qwCurJingJie; //当前境界
	UINT8            byHeroNum; //武将个数
	DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息
}DT_DATA_AFTER_BATTLE_ENCOURAGE;

typedef struct tagDT_RACE_CHALLENGE_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwScience; //器魂点
	UINT32           dwJingJie; //境界点
}DT_RACE_CHALLENGE_ENCOURAGE_DATA;

typedef struct tagDT_RACE_RANK_ENCOURAGE_DATA{
	UINT32           dwRank; //排名
	UINT32           dwCoin; //铜钱
	UINT32           dwScience; //器魂点
	UINT32           dwJingJie; //境界点
}DT_RACE_RANK_ENCOURAGE_DATA;

typedef struct tagPKT_CLIGS_ENTER_INSTANCE_REQ{
	tagPKT_CLIGS_ENTER_INSTANCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTER_INSTANCE_REQ)); }
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT8            byNewFlag; //是否重新进入副本(用于之前在副本中时进入副本)，1为重新开启副本
}PKT_CLIGS_ENTER_INSTANCE_REQ;

typedef struct tagDT_PASS_ENCOURAGE_DATA{
	UINT32           dwStory; //阅历
	UINT32           dwCoachExperience; //奖励主角经验
	UINT32           dwExperience; //奖励伙伴经验
	UINT32           dwCoin; //铜钱
}DT_PASS_ENCOURAGE_DATA;

typedef struct tagDT_DATA_AFTER_PASS_ENCOURAGE{
	UINT8            byHeroNum; //武将个数
	DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息
}DT_DATA_AFTER_PASS_ENCOURAGE;

typedef struct tagDT_PASS_ENCOURAGE_BOX_GROUP_DATA{
	UINT32           dwCoin; //铜钱(非0则转盘命中铜钱)
	UINT32           dwGold; //元宝(非0则转盘命中元宝)
	UINT8            byHitItemIdx; //命中道具序号(铜钱/元宝非0时有效，0表示未命中,从1开始,<=ItemNum)
	DT_ITEM_DATA_LIST_CLI stItemInfo; //道具信息
	UINT32           dwBlueGas; //蓝气(非0则转盘命中蓝气)
}DT_PASS_ENCOURAGE_BOX_GROUP_DATA;

typedef struct tagDT_PASS_ENCOURAGE_BOX_DATA{
	UINT8            byGroupNum; //宝箱组个数
	DT_PASS_ENCOURAGE_BOX_GROUP_DATA astBoxGroupInfo[MAX_ENCOURAGE_BOX_GROUP_NUM]; //宝箱组信息
}DT_PASS_ENCOURAGE_BOX_DATA;

typedef struct tagDT_PASS_ENCOURAGE_BOX_DATA2{
	UINT8            byGroupNum; //宝箱组个数
	DT_PASS_ENCOURAGE_BOX_GROUP_DATA astBoxGroupInfo[MAX_GVE_ENCOURAGE_BOX_GROUP_NUM]; //宝箱组信息
}DT_PASS_ENCOURAGE_BOX_DATA2;

typedef struct tagDT_HERO_UPGRADE_DATA_LIST{
	UINT8            byUpgradeHeroNum; //升级武将个数
	DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息
}DT_HERO_UPGRADE_DATA_LIST;

typedef struct tagPKT_CLIGS_ENTER_INSTANCE_ACK{
	tagPKT_CLIGS_ENTER_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTER_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCliVer; //客户端版本
	UINT32           dwResVer; //资源版本
	UINT32           dwUIVer; //UI版本
	DT_PHYSTRENGTH_DATA_CLI stPhyStrengthInfo; //体力信息
	UINT32           dwMaxHP; //总HP（包含所有上阵武将属性）
	UINT32           dwCurHP; //当前HP（包含所有上阵武将属性）
	DT_MONSTER_DATA_LIST stMonsterInfo; //怪物信息
	UINT8            byHaveBoxFlag; //是否有奖励宝箱，1为有宝箱
	DT_PASS_ENCOURAGE_BOX_DATA astBoxInfo[1]; //奖励宝箱信息
	DT_HERO_UPGRADE_DATA_LIST astHeroUpgradeInfo[1]; //伙伴升级信息
	UINT16           wDoubleExpNum; //经验双倍符个数，大于0表示有使用双倍符
	UINT8            byScore; //副本评分
	TCHAR            aszResVer[MAX_RES_VER_LEN]; //版本信息
}PKT_CLIGS_ENTER_INSTANCE_ACK;

typedef struct tagPKT_CLIGS_BATTLE_REQ{
	tagPKT_CLIGS_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BATTLE_REQ)); }
	UINT8            byBattleIndex; //战斗序号(第几波怪，从1开始)
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
}PKT_CLIGS_BATTLE_REQ;

typedef struct tagDT_BURST_ITEM_DATA{
	UINT8            byBurstItemNum; //爆出物品个数
	UINT16           awBurstItemList[MAX_ENCOURAGE_ITEM_KIND_NUM]; //爆出物品类型信息
}DT_BURST_ITEM_DATA;

typedef struct tagDT_NEXT_INSTANCE_DATA{
	UINT16           wSceneIdx; //副本场景序号
	UINT8            byTownIdx; //副本场景城镇序号
	UINT8            byInstanceIdx; //副本序号
	UINT16           wEnterLevel; //进入等级要求
	UINT16           wEnterPhyStrength; //进入体力要求
}DT_NEXT_INSTANCE_DATA;

typedef struct tagDT_UNLOCK_DATA{
	UINT16           wUnlockMainTownLevel; //解锁主城等级,0表示没有变化
	UINT8            byUnlockBuildKindID; //建筑建筑类型，0表示没有新解锁的建筑
	UINT16           awUnlockHeroID[MAX_UNLOCK_HERO_NUM]; //解锁武将类型,0表示没有新解锁的武将
}DT_UNLOCK_DATA;

typedef struct tagPKT_CLIGS_BATTLE_ACK3{
	tagPKT_CLIGS_BATTLE_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_BATTLE_ACK3)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_DATA3  stBattleInfo; //战斗信息
	DT_BATTLE_ENCOURAGE_DATA stBattleEncourage; //战斗奖励
	DT_DATA_AFTER_BATTLE_ENCOURAGE stAfterBattleEncourage; //领取战斗奖励后玩家信息
	UINT8            byPassFlag; //是否通关
	DT_PASS_ENCOURAGE_DATA astPassEncourage[1]; //通关奖励
	DT_DATA_AFTER_PASS_ENCOURAGE astAfterPassEncourage[1]; //领取通关奖励后玩家信息
	DT_PASS_ENCOURAGE_BOX_DATA astPassEncourageBox[1]; //通关宝箱奖励信息
	DT_UNLOCK_DATA   astUnlockInfo[1]; //解锁信息
	UINT8            byHaveNextInstance; //是否有下一个副本
	DT_NEXT_INSTANCE_DATA astNextInstanceInfo[1]; //下一个副本信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_PHYSTRENGTH_DATA_CLI stPhyStrengthInfo; //体力信息
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	TCHAR            aszExtContext[MAX_COMMON_TXT_LEN]; //扩展信息
}PKT_CLIGS_BATTLE_ACK3;

typedef struct tagPKT_CLIGS_EXIT_INSTANCE_REQ{
	tagPKT_CLIGS_EXIT_INSTANCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EXIT_INSTANCE_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_EXIT_INSTANCE_REQ;

typedef struct tagPKT_CLIGS_EXIT_INSTANCE_ACK{
	tagPKT_CLIGS_EXIT_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EXIT_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_PLAYER_BASE_DATA_CLI stPlayerInfo; //当前玩家信息(副本中基本会更新玩家信息)
}PKT_CLIGS_EXIT_INSTANCE_ACK;

typedef struct tagDT_INSTANCE_RECORD_DATA_CLI{
	UINT8            byIndex; //副本序号
	UINT8            byScore; //副本评分
	UINT16           wEnterLevel; //进入等级要求
	DT_BURST_ITEM_DATA stBurstItemInfo; //爆出物品信息
	UINT32           dwBurstExperience; //爆出经验
	UINT32           dwMaxMonstorPower; //怪物最大战力
	DT_PASS_ENCOURAGE_DATA stPassEncourage; //通关奖励信息
}DT_INSTANCE_RECORD_DATA_CLI;

typedef struct tagDT_TOWN_INSTANCE_RECORD_DATA_CLI{
	UINT8            byIndex; //城镇序号
	UINT16           wMainMonsterKindID; //显示主怪物类型ID，0不需显示
	UINT8            byCanBattle; //是否可以打，1可以打
	UINT8            byInstanceNum; //副本数
	DT_INSTANCE_RECORD_DATA_CLI astInstanceInfo[MAX_INSTANCE_PER_TOWN_NUM]; //副本记录信息
}DT_TOWN_INSTANCE_RECORD_DATA_CLI;

typedef struct tagPKT_CLIGS_GET_SCENE_INSTANCE_REQ{
	tagPKT_CLIGS_GET_SCENE_INSTANCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_SCENE_INSTANCE_REQ)); }
	UINT16           wIndex; //场景序号
}PKT_CLIGS_GET_SCENE_INSTANCE_REQ;

typedef struct tagDT_CHAPTER_SCORE{
	UINT8            byFlag; //领取标志0：不可领取， 1：可领取， 2：已领取
	DT_ENCOURAGE_DATA stChapterEncourage; //评分奖励信息
}DT_CHAPTER_SCORE;

typedef struct tagDT_ACCUMULATED_SCORE_ENCOURAGE_BOX_DATA{
	UINT8            byGroupNum; //宝箱组个数
	DT_PASS_ENCOURAGE_BOX_GROUP_DATA astBoxGroupInfo[MAX_ENCOURAGE_BOX_GROUP_NUM]; //宝箱组信息
}DT_ACCUMULATED_SCORE_ENCOURAGE_BOX_DATA;

typedef struct tagDT_ACCUMULATED_SCORE_ENCOURAGE{
	UINT32           dwCurSocre; //当前总评分
	UINT32           dwEncourageSocre; //可领取奖励评分
	DT_ACCUMULATED_SCORE_ENCOURAGE_BOX_DATA stAccumulatedEncourageBox; //奖励宝箱信息
	UINT8            byFlag; //领取标志0：不可领取， 1：可领取
	DT_ENCOURAGE_DATA stAccumulatedEncourage; //奖励宝箱信息
}DT_ACCUMULATED_SCORE_ENCOURAGE;

typedef struct tagPKT_CLIGS_GET_SCENE_INSTANCE_ACK{
	tagPKT_CLIGS_GET_SCENE_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_SCENE_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wIndex; //场景序号
	UINT8            byEnterPhyStrength; //进入体力要求
	DT_INSTANCE_ID   stMaxInstanceData; //最大副本信息
	DT_INSTANCE_ID   stMaxUnlockInstance; //解锁副本信息
	UINT8            byTownNum; //城镇数
	DT_TOWN_INSTANCE_RECORD_DATA_CLI astTownInstanceInfo[MAX_TOWN_PER_SCENE_NUM]; //城镇副本信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	TCHAR            aszContent[MAX_CONTENT_SIZE]; //其他要显示内容
	UINT8            byChapterNum; //章数
	DT_CHAPTER_SCORE astChapterInfo[MAX_TOWN_PER_SCENE_NUM]; //章数评分信息
	DT_ACCUMULATED_SCORE_ENCOURAGE stAccumulatedEncourage; //积累奖励信息
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
	UINT32           dwFlyExp; //伙伴等级已到120级卡在飞升期间时累积的经验
}PKT_CLIGS_GET_SCENE_INSTANCE_ACK;

typedef struct tagDT_EQUIP_ENCHANT_DATA_CLI{
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT8            byCanEnchantFlag; //是否可附魔
	UINT16           wEnchantStoneID; //所需附魔石ID
	UINT16           wCurEnchantStone; //当前附魔石数
	UINT16           wCostEnchantStone; //所需附魔石数
	DT_BATTLE_ATTRIBUTE2 stNextBattleAttribute; //下一级战斗属性
}DT_EQUIP_ENCHANT_DATA_CLI;

typedef struct tagPKT_CLIGS_EQUIP_STRENGTHEN_REQ{
	tagPKT_CLIGS_EQUIP_STRENGTHEN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_STRENGTHEN_REQ)); }
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT8            byStrengthenType; //强化方式，0金币，1元宝
	UINT16           wLevel; //要升到的等级(每次只能升1级，0表示直接升级10级)
}PKT_CLIGS_EQUIP_STRENGTHEN_REQ;

typedef struct tagPKT_CLIGS_EQUIP_STRENGTHEN_ACK{
	tagPKT_CLIGS_EQUIP_STRENGTHEN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_STRENGTHEN_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_EQUIP_DATA_CLI stEquipInfo; //装备信息
	DT_EQUIP_DATA_STRENGTHEN_CLI stEquipStrengthenInfo; //装备强化信息
	UINT16           wStrengthenCountDown; //装备强化倒计时
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurGold; //玩家当前的元宝
	UINT32           dwPlayerMaxHP; //玩家当前总血量
	UINT32           dwPlayerCurHP; //玩家当前血量(在副本中有效)
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT32           dwStrengthen10Coin; //装备升10级所需要铜钱信息
	DT_EQUIP_ENCHANT_DATA_CLI stEquipEnchantInfo; //强化成功后本装备的附魔信息
}PKT_CLIGS_EQUIP_STRENGTHEN_ACK;

typedef struct tagPKT_CLIGS_OPEN_SMITHY_REQ{
	tagPKT_CLIGS_OPEN_SMITHY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SMITHY_REQ)); }
	UINT8            byBuildKindID; //建筑类型
}PKT_CLIGS_OPEN_SMITHY_REQ;

typedef struct tagDT_BUY_EQUIP_DATA{
	UINT16           wEquipID; //装备ID
	UINT32           dwBuyCoin; //购买铜币
}DT_BUY_EQUIP_DATA;

typedef struct tagPKT_CLIGS_OPEN_SMITHY_ACK{
	tagPKT_CLIGS_OPEN_SMITHY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SMITHY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wStrengthenCountDown; //装备强化倒计时
	UINT16           wStrengthenCD; //装备强化总CD
	UINT32           dwClrCDGold; //清除CD所需元宝
	DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI stEquipList; //装备武器列表
	DT_BUY_EQUIP_DATA astByEquipInfo[EQUIP_POS_NUM]; //可购买初级装备信息(数组下标+1为穿戴位置)
	UINT64           qwBlueGas; //当前蓝气
	UINT16           wEquipNum; //装备个数
	UINT8            abyEquipOpenLightFlagInfo[MAX_BAG_OPEN_NUM]; //装备开光标志信息(0不可开光，1可开光，2已开光)
	UINT32           adwStrengthen10CoinInfo[MAX_BAG_OPEN_NUM]; //装备升10级所需要铜钱信息
}PKT_CLIGS_OPEN_SMITHY_ACK;

typedef struct tagPKT_CLIGS_OPEN_RECRUIT_REQ{
	tagPKT_CLIGS_OPEN_RECRUIT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RECRUIT_REQ)); }
	UINT8            byBuildKindID; //建筑类型
}PKT_CLIGS_OPEN_RECRUIT_REQ;

typedef struct tagDT_HERO_ATTR_ADD_CLI{
	UINT32           dwAddAttack; //攻击
	UINT32           dwAddHP; //血量
	UINT32           dwAddTalent; //资质
}DT_HERO_ATTR_ADD_CLI;

typedef struct tagDT_RECRUIT_HERO_DATA{
	UINT8            byNeedVisitNum; //需拜访次数
	UINT8            byCurVisitNum; //当前已拜访次数
	UINT32           dwVisitCoin; //每次拜访铜币
	DT_HERO_BASE_DATA_CLI stHeroBaseInfo; //武将基本信息
	DT_HERO_ATTR_ADD_CLI stHeroAttrAddInfo; //武将属性加成信息
	DT_SKILL_DATA_CLI stSkillInfo; //武将技能信息
	UINT16           wVisitGold; //出千元宝
	UINT16           wRecuitLevel; //招募等级
	UINT16           wRequireJingjieLevel; //招募所需境界阶等级
	UINT8            byRecuitUnLock; //是否可博弈
	UINT8            byRecommend; //是否推荐
}DT_RECRUIT_HERO_DATA;

typedef struct tagPKT_CLIGS_OPEN_RECRUIT_ACK{
	tagPKT_CLIGS_OPEN_RECRUIT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RECRUIT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wJingJieLevel; //境界等级
	UINT16           wHeroNum; //武将数目
	DT_RECRUIT_HERO_DATA astHeroInfoLst[MAX_HERO_NUM]; //武将信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_OPEN_RECRUIT_ACK;

typedef struct tagPKT_CLIGS_RECRUIT_HERO_REQ{
	tagPKT_CLIGS_RECRUIT_HERO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECRUIT_HERO_REQ)); }
	UINT16           wHeroKindID; //武将类型
}PKT_CLIGS_RECRUIT_HERO_REQ;

typedef struct tagPKT_CLIGS_RECRUIT_HERO_ACK{
	tagPKT_CLIGS_RECRUIT_HERO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECRUIT_HERO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_HERO_BASE_DATA_CLI stHeroBaseInfo; //武将基本信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	UINT64           qwPlayerCoin; //玩家的铜钱
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_RECRUIT_HERO_ACK;

typedef struct tagPKT_CLIGS_VISIT_HERO_REQ{
	tagPKT_CLIGS_VISIT_HERO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_VISIT_HERO_REQ)); }
	UINT16           wHeroKindID; //武将类型
	UINT8            bySizeFlag; //大小标志
	UINT8            byGoldFlag; //出千标志
}PKT_CLIGS_VISIT_HERO_REQ;

typedef struct tagPKT_CLIGS_VISIT_HERO_ACK{
	tagPKT_CLIGS_VISIT_HERO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_VISIT_HERO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byWinFlag; //输赢标志
	UINT16           wHeroKindID; //武将类型
	UINT8            byNeedVisitNum; //需拜访次数
	UINT8            byCurVisitNum; //当前已拜访次数
	UINT64           qwPlayerCoin; //玩家的铜钱
	UINT64           qwPlayerGold; //玩家的元宝
	UINT16           wUpgradeVisitNum; //提升好感度
	UINT32           dwReturnCoin; //返还的铜钱数
}PKT_CLIGS_VISIT_HERO_ACK;

typedef struct tagDT_BATTLE_ATTR_EXT_INFO_CLI{
	UINT8            byKindID; //作用目标(commondef.h)
	UINT32           dwKindValue; //基础值
	UINT8            byExtLevel; //附加等级
	UINT32           dwExtValue; //附加值
	UINT32           dwValueType; //值类型（1固定值， 2百分数）参考commondef.h ValueType
}DT_BATTLE_ATTR_EXT_INFO_CLI;

typedef struct tagDT_BATTLE_ATTR_EXT_LST_INFO_CLI{
	UINT8            byExtAttrNum; //附加属性数量
	DT_BATTLE_ATTR_EXT_INFO_CLI astExtAttrInfo[MAX_SCIENCE_ATTR_NUM]; //附加属性信息
}DT_BATTLE_ATTR_EXT_LST_INFO_CLI;

typedef struct tagDT_JEWELRY_DATA_CLI{
	UINT16           wJewelryID; //饰品ID
	UINT16           wJewelryLevel; //饰品穿戴Level
	UINT8            byJewelryDressPos; //饰品类型(穿戴位置commondef.f JewelryDressPos)
	DT_BATTLE_ATTR_LST_INFO stBaseAttrLst; //饰品基本属性
	DT_BATTLE_ATTR_EXT_LST_INFO_CLI stExtAttrLst; //饰品附加属性
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT16           wStrengLevel; //饰品强化等级
	UINT16           wHeroID; //所属武将
	UINT8            byExtStrengNum; //当前附加属性剩余强化次数
	UINT8            byStrengExtLevel; //可强化附加属性间隔等级
}DT_JEWELRY_DATA_CLI;

typedef union tagDT_ITEM_DATA_UNOIN2{
	DT_EQUIP_DATA_CLI stEquip; //战力变化信息
	DT_GOODS_DATA_CLI stGoods; //战力变化信息
	DT_JEWELRY_DATA_CLI stJewelry; //战力变化信息
}DT_ITEM_DATA_UNOIN2;

typedef struct tagDT_ITEM_DATA_CLI3{
	UINT8            byItemType; //道具类型
	DT_ITEM_DATA_UNOIN2 stItemInfo; //道具类型
}DT_ITEM_DATA_CLI3;

typedef struct tagDT_ITEM_DATA_LIST_CLI3{
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI3 astItemList[MAX_BAG_OPEN_NUM]; //道具信息
}DT_ITEM_DATA_LIST_CLI3;

typedef struct tagDT_ITEM_DATA_LIST_CLI4{
	UINT16           wItemNum; //道具个数
	DT_ITEM_DATA_CLI3 astItemList[MAX_BAG_OPEN_NUM_NEW]; //道具信息
}DT_ITEM_DATA_LIST_CLI4;

typedef struct tagDT_ITEM_NUM_DATA_CLI{
	UINT16           wItemID; //物品ID
	UINT16           wItemNum; //物品数量
	UINT16           wItemIdx; //该同类型物品的序号，用于区别相同类型的装备（装备和饰品需要这个字段）
}DT_ITEM_NUM_DATA_CLI;

typedef struct tagDT_ITEM_NUM_DATA_LIST_CLI{
	UINT16           wItemNum; //道具个数
	DT_ITEM_NUM_DATA_CLI astItemList[MAX_BAG_OPEN_NUM_NEW]; //道具信息
}DT_ITEM_NUM_DATA_LIST_CLI;

typedef struct tagDT_ITEM_DATA_LIST_LESS_CLI3{
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI3 astItemList[MAX_DISPALY_GOODS_NUM]; //道具信息
}DT_ITEM_DATA_LIST_LESS_CLI3;

typedef struct tagDT_RES_ITEM_DATA2{
	UINT8            byResNum; //资源个数
	DT_RES_DATA      astResList[MAX_PLAYER_RES_NUM]; //资源信息
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI3 astItemList[MAX_PLAYER_RES_NUM]; //道具信息
}DT_RES_ITEM_DATA2;

typedef struct tagPKT_CLIGS_OPEN_BAG_REQ{
	tagPKT_CLIGS_OPEN_BAG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BAG_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_BAG_REQ;

typedef struct tagPKT_CLIGS_OPEN_BAG_ACK{
	tagPKT_CLIGS_OPEN_BAG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BAG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BAG_DATA_CLI  stBagInfo; //背包基本信息
	DT_ITEM_DATA_LIST_CLI2 stBagItemInfo; //背包道具信息
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
	DT_ITEM_DATA_LIST_CLI3 stBagItemInfo2; //背包道具信息
}PKT_CLIGS_OPEN_BAG_ACK;

typedef struct tagPKT_CLIGS_OPEN_BAG_ACK2{
	tagPKT_CLIGS_OPEN_BAG_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BAG_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BAG_DATA_CLI2 stBagInfo; //背包基本信息
	DT_ITEM_DATA_LIST_CLI4 stBagItemInfo; //背包道具信息
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
}PKT_CLIGS_OPEN_BAG_ACK2;

typedef struct tagPKT_CLIGS_ACCEPT_TASK_REQ{
	tagPKT_CLIGS_ACCEPT_TASK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ACCEPT_TASK_REQ)); }
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
}PKT_CLIGS_ACCEPT_TASK_REQ;

typedef struct tagDT_INSTANCE_DATA_CLI{
	DT_INSTANCE_ID   stInstanceID; //副本序号
	UINT16           wEnterLevel; //进入等级要求
	DT_BURST_ITEM_DATA stBurstItemInfo; //爆出物品信息
	UINT32           dwBurstExperience; //爆出经验
	UINT32           dwMaxMonstorPower; //怪物最大战力
}DT_INSTANCE_DATA_CLI;

typedef struct tagPKT_CLIGS_ACCEPT_TASK_ACK{
	tagPKT_CLIGS_ACCEPT_TASK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ACCEPT_TASK_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //任务信息列表(若状态不匹配下发整个任务信息)
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	UINT8            byUnlockNewInstanceFlag; //是否解锁新副本
	DT_INSTANCE_ID   astUnlockCommonInstanceID[1]; //解锁的普通副本ID
	UINT8            byHaveNewInstanceFlag; //是否有新可打普通副本,1有效
	DT_INSTANCE_DATA_CLI astNewCommonInstanceID[1]; //新可打普通副本ID
}PKT_CLIGS_ACCEPT_TASK_ACK;

typedef struct tagPKT_CLIGS_GIVEUP_TASK_REQ{
	tagPKT_CLIGS_GIVEUP_TASK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GIVEUP_TASK_REQ)); }
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
}PKT_CLIGS_GIVEUP_TASK_REQ;

typedef struct tagPKT_CLIGS_GIVEUP_TASK_ACK{
	tagPKT_CLIGS_GIVEUP_TASK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GIVEUP_TASK_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //任务信息列表(若状态不匹配下发整个任务信息)
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
}PKT_CLIGS_GIVEUP_TASK_ACK;

typedef struct tagPKT_CLIGS_RECV_TASK_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_TASK_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_TASK_ENCOURAGE_REQ)); }
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
}PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_TASK_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_TASK_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_TASK_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byTaskLineIdx; //任务线序号
	UINT32           dwTaskIdx; //任务序号
	DT_HERO_UPGRADE_DATA stMainHeroUpgradeInfo; //主将升级信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //任务信息列表(若状态改变则下发整个任务线给客户端,否则只下发变化的任务)
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurGold; //玩家当前的元宝
	DT_PHYSTRENGTH_DATA_CLI stPhyStrengthInfo; //体力信息
	UINT64           qwCurScience; //玩家当前的器魂点
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	UINT16           wUnlockEquipLevel; //解锁装备提示(0表示没有)
	UnlockScienceInfo stUnlockScienceList; //解锁科技提示
	UINT8            byUnlockNewInstanceFlag; //是否解锁新副本
	DT_INSTANCE_ID   astUnlockCommonInstanceID[1]; //解锁的普通副本ID
	UINT8            byHaveNewInstanceFlag; //是否有新可打普通副本,1有效
	DT_INSTANCE_DATA_CLI astNewCommonInstanceID[1]; //新可打普通副本ID
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
	UINT64           qwStory; //玩家当前的阅历
	UINT64           qwBlueGas; //玩家当前的蓝气
	UINT64           qwPurpleGas; //玩家当前的紫气
	UINT64           qwJingJie; //玩家当前的境界
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT8            byHeroNum; //升级武将个数
	DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息
}PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_OPEN_RACE_BUILD_REQ{
	tagPKT_CLIGS_OPEN_RACE_BUILD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RACE_BUILD_REQ)); }
	UINT8            byBuildKindID; //建筑类型
}PKT_CLIGS_OPEN_RACE_BUILD_REQ;

typedef struct tagDT_CHALLENGE_PLAYER_DATA{
	UINT32           dwID; //玩家唯一ID
	UINT16           wHeroKindID; //主武将类型
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT16           wLevel; //玩家等级
	UINT32           dwRank; //排名
	UINT32           dwPower; //战力
	UINT32           dwFirstAttack; //先攻值
}DT_CHALLENGE_PLAYER_DATA;

typedef struct tagPKT_CLIGS_OPEN_RACE_BUILD_ACK{
	tagPKT_CLIGS_OPEN_RACE_BUILD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RACE_BUILD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wLevel; //玩家等级
	UINT32           dwRank; //排名
	UINT16           wContiWin; //当前连胜次数
	UINT64           qwScience; //器魂点
	UINT64           qwJingJie; //境界点
	UINT32           dwPower; //战力
	UINT32           dwFirstAttack; //先攻值
	UINT8            byHaveRecvEncourageFlag; //今日是否领取过排名奖励
	DT_RACE_RANK_ENCOURAGE_DATA stTodayRankEncourage; //今日可领取的排名奖励(未领取有效)
	DT_RACE_RANK_ENCOURAGE_DATA stTomorrowRankEncourage; //明日可领取的排名奖励
	UINT16           wTotalChallengeNum; //玩家挑战总次数
	UINT16           wCanChallengeNum; //玩家当前可挑战次数
	UINT16           wVipExtNum; //VIP格外增加的挑战次数
	UINT16           wFreeChallengeNum; //今日可免费挑战次数
	UINT32           dwChallengeNeedGold; //收费挑战需元宝数
	UINT16           wChallengeCountDown; //挑战倒计时，单位秒
	UINT32           dwClrCDGold; //清除CD所需元宝(每分钟)
	UINT8            byChallengeNum; //挑战玩家个数
	DT_CHALLENGE_PLAYER_DATA astChallengePlayerList[MAX_CHALLENGE_NUM]; //挑战玩家信息列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT8            byHaveWorship; //是否已经膜拜，1为已经膜拜
	UINT8            byRecvEncCountDown; //可领取竞技场奖励天数倒计时
}PKT_CLIGS_OPEN_RACE_BUILD_ACK;

typedef struct tagDT_ENEMY_EQUIP_DATA{
	UINT16           wKindID; //装备类型
	UINT16           wLevel; //装备等级
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //当前属性
}DT_ENEMY_EQUIP_DATA;

typedef struct tagDT_ENEMY_EQUIP_DATA_LST{
	UINT16           wEquipNum; //装备个数
	DT_ENEMY_EQUIP_DATA astEquipList[EQUIP_POS_NUM]; //装备信息
}DT_ENEMY_EQUIP_DATA_LST;

typedef struct tagDT_ENEMY_HERO_DATA{
	UINT16           wKindID; //武将类型
	UINT8            byCoach; //主武将标志,1为主武将
	UINT16           wLevel; //等级
	UINT16           wTalent; //资质
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //战斗属性
	DT_ENEMY_EQUIP_DATA_LST stEquipList; //装备信息
}DT_ENEMY_HERO_DATA;

typedef struct tagPKT_CLIGS_CHALLENGE_REQ{
	tagPKT_CLIGS_CHALLENGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHALLENGE_REQ)); }
	UINT32           dwRank; //排名
}PKT_CLIGS_CHALLENGE_REQ;

typedef struct tagPKT_CLIGS_CHALLENGE_ACK3{
	tagPKT_CLIGS_CHALLENGE_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_CHALLENGE_ACK3)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_CHALLENGE_PLAYER_DATA stEnemyPlayerData; //对方玩家信息
	UINT8            byEnemyHeroNum; //对方信息
	DT_ENEMY_HERO_DATA astEnemyInfoList[MAX_FORMATION_IDX_NUM]; //对方信息
	DT_BATTLE_DATA3  stBattleInfo; //战斗信息
	DT_RACE_CHALLENGE_ENCOURAGE_DATA stChallengeEncourage; //挑战奖励
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝点
	UINT64           qwCurJingJie; //玩家当前的境界点
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_CHALLENGE_ACK3;

typedef struct tagPKT_CLIGS_RECV_RANK_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_RANK_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_RANK_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_RANK_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_RANK_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_RANK_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RACE_RANK_ENCOURAGE_DATA stRankEncourage; //排名奖励
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwJingJie; //境界点
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_GET_RANK_REQ{
	tagPKT_CLIGS_GET_RANK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_RANK_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_GET_RANK_REQ;

typedef struct tagDT_RANK_DATA{
	UINT32           dwRank; //排名
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT16           wHeroKindID; //主武将类型
	UINT16           wLevel; //等级
	DT_RACE_RANK_ENCOURAGE_DATA stRankEncourage; //排名奖励
	UINT32           dwPower; //战力
}DT_RANK_DATA;

typedef struct tagPKT_CLIGS_GET_RANK_ACK{
	tagPKT_CLIGS_GET_RANK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_RANK_ACK)); }
	UINT16           wErrCode; //战力变化信息
	UINT8            byShowNum; //显示排名个数
	DT_RANK_DATA     astRankShowInfo[MAX_RANK_SHOW_NUM]; //排名信息
}PKT_CLIGS_GET_RANK_ACK;

typedef struct tagPKT_CLIGS_GET_RACE_BATTLELOG_REQ{
	tagPKT_CLIGS_GET_RACE_BATTLELOG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_RACE_BATTLELOG_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_GET_RACE_BATTLELOG_REQ;

typedef struct tagDT_RACE_BATTLELOG_DATA_CLI{
	DT_RACE_BATTLELOG_DATA stBattlelogInfo; //战报信息
	TCHAR            aszEnemyDispName[USERNAME_LEN]; //对方昵称
}DT_RACE_BATTLELOG_DATA_CLI;

typedef struct tagPKT_CLIGS_GET_RACE_BATTLELOG_ACK{
	tagPKT_CLIGS_GET_RACE_BATTLELOG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_RACE_BATTLELOG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byShowNum; //显示战报个数
	DT_RACE_BATTLELOG_DATA_CLI astRankBattleLogInfo[MAX_RACE_BATTLELOG_NUM]; //战报信息
}PKT_CLIGS_GET_RACE_BATTLELOG_ACK;

typedef struct tagPKT_CLIGS_OPEN_SHOP_REQ{
	tagPKT_CLIGS_OPEN_SHOP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SHOP_REQ)); }
	UINT8            byTabID; //标签ID
}PKT_CLIGS_OPEN_SHOP_REQ;

typedef struct tagDT_ITEM_DATA_SHOP{
	DT_ITEM_DATA_CLI2 stItemInfo; //道具信息
	UINT32           dwBuyCoin; //购买铜币价格
	UINT32           dwBuyGold; //购买元宝价格
}DT_ITEM_DATA_SHOP;

typedef struct tagPKT_CLIGS_OPEN_SHOP_ACK{
	tagPKT_CLIGS_OPEN_SHOP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SHOP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byTabID; //标签ID
	UINT8            byItemNum; //道具信息
	DT_ITEM_DATA_SHOP astItemDataList[MAX_SHOP_ITEM_NUM]; //道具信息
}PKT_CLIGS_OPEN_SHOP_ACK;

typedef struct tagPKT_CLIGS_BUY_ITEM_REQ{
	tagPKT_CLIGS_BUY_ITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_ITEM_REQ)); }
	UINT8            byTabID; //标签ID
	UINT16           wKindID; //道具类型
	UINT16           wPileCount; //叠加数
}PKT_CLIGS_BUY_ITEM_REQ;

typedef struct tagPKT_CLIGS_BUY_ITEM_ACK{
	tagPKT_CLIGS_BUY_ITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_ITEM_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byTabID; //标签ID
	UINT16           wKindID; //道具类型
	UINT16           wPileCount; //叠加数
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurGold; //玩家当前的元宝
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
}PKT_CLIGS_BUY_ITEM_ACK;

typedef struct tagPKT_CLIGS_SELL_ITEM_REQ{
	tagPKT_CLIGS_SELL_ITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SELL_ITEM_REQ)); }
	UINT16           wKindID; //类型
	UINT16           wIdx; //序号，物品为1
	UINT16           wNum; //出售个数
}PKT_CLIGS_SELL_ITEM_REQ;

typedef struct tagPKT_CLIGS_SELL_ITEM_ACK{
	tagPKT_CLIGS_SELL_ITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SELL_ITEM_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wKindID; //类型
	UINT16           wIdx; //序号，装备有效
	UINT16           wNum; //出售个数
	UINT64           qwCurCoin; //玩家当前的铜钱
	DT_ITEM_NUM_DATA_LIST_CLI stItemNumList; //出售后道具后所获得物品的当前个数（某些装备出售后可以获得物品，如附魔装备出售后可以得到附魔石）
}PKT_CLIGS_SELL_ITEM_ACK;

typedef struct tagPKT_CLIGS_DISBAND_HERO_REQ{
	tagPKT_CLIGS_DISBAND_HERO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DISBAND_HERO_REQ)); }
	UINT16           wHeroKindID; //武将类型
}PKT_CLIGS_DISBAND_HERO_REQ;

typedef struct tagPKT_CLIGS_DISBAND_HERO_ACK{
	tagPKT_CLIGS_DISBAND_HERO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DISBAND_HERO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将类型
	UINT32           dwPlayerHP; //玩家总血量
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_DISBAND_HERO_ACK;

typedef struct tagPKT_CLIGS_EQUIP_REQ{
	tagPKT_CLIGS_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_REQ)); }
	UINT16           wHeroKindID; //要穿戴的武将
	UINT8            byDressType; //穿戴方式(装备/卸下/替换)
	UINT16           wDressItemKindID; //要穿戴的装备类型
	UINT16           wDressItemIdx; //要穿戴的装备序号
	UINT16           wUndressItemKindID; //要卸下的装备类型(卸下，替换时有效)
	UINT16           wUndressItemIdx; //要卸下的装备序号(卸下，替换时有效)
}PKT_CLIGS_EQUIP_REQ;

typedef struct tagDT_HERO_PANEL_CLI{
	UINT16           wKindID; //武将ID
	DT_BATTLE_ATTRIBUTE2 stBattleAttribute; //战斗属性
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttribute; //隐藏战斗属性
	UINT32           dwPower; //战力
	UINT8            byPos; //位置标志,0携带，1为出战，2在招募所中
	UINT16           wLevel; //等级
	UINT16           wTalent; //资质(包括丹药)
	UINT32           dwExperience; //经验
	UINT32           dwUpgradeExperience; //升级经验
}DT_HERO_PANEL_CLI;

typedef struct tagDT_HERO_PANEL_LST_CLI{
	UINT16           wHeroNum; //武将个数
	DT_HERO_PANEL_CLI astHeroPanelInfo[MAX_HERO_NUM]; //武将面板信息
}DT_HERO_PANEL_LST_CLI;

typedef struct tagPKT_CLIGS_EQUIP_ACK{
	tagPKT_CLIGS_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //要穿戴的武将
	UINT8            byDressType; //穿戴方式(装备/卸下/替换)
	UINT16           wDressItemKindID; //要穿戴的装备类型
	UINT16           wDressItemIdx; //要穿戴的装备序号
	UINT16           wUndressItemKindID; //要卸下的装备类型(卸下，替换时有效)
	UINT16           wUndressItemIdx; //要卸下的装备序号(卸下，替换时有效)
	UINT32           dwPlayerHP; //玩家总血量
	DT_POWER_CHANGE_DATA stHeroPowerInfo; //武将战力信息
	DT_POWER_CHANGE_DATA stPowerInfo; //玩家战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_HERO_PANEL_LST_CLI stHeroPanelInfo; //武将的面板信息
}PKT_CLIGS_EQUIP_ACK;

typedef struct tagPKT_CLIGS_REGISTER_USERNAME_NTF{
	tagPKT_CLIGS_REGISTER_USERNAME_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_REGISTER_USERNAME_NTF)); }
	UINT8            byExt; //扩展，未使用
}PKT_CLIGS_REGISTER_USERNAME_NTF;

typedef struct tagPKT_CLIGS_OPEN_EMBATTLE_REQ{
	tagPKT_CLIGS_OPEN_EMBATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EMBATTLE_REQ)); }
	UINT8            byExt; //扩展数据(未使用)
}PKT_CLIGS_OPEN_EMBATTLE_REQ;

typedef struct tagDT_HERO_EMBATLE_DATA{
	UINT16           wKindID; //武将类型
	UINT8            byCareerID; //职业ID
	UINT16           wLevel; //等级
	UINT16           wTalent; //资质
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //战斗属性
	UINT32           dwPower; //战力
	DT_SKILL_DATA_CLI stSkillInfo; //技能信息
}DT_HERO_EMBATLE_DATA;

typedef struct tagPKT_CLIGS_OPEN_EMBATTLE_ACK{
	tagPKT_CLIGS_OPEN_EMBATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EMBATTLE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroNum; //武将数目
	DT_HERO_EMBATLE_DATA astHeroInfoLst[MAX_HERO_NUM]; //武将信息
	DT_FORMATION_DATA stFormationInfo; //阵型信息
}PKT_CLIGS_OPEN_EMBATTLE_ACK;

typedef struct tagPKT_CLIGS_EMBATTLE_REQ{
	tagPKT_CLIGS_EMBATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EMBATTLE_REQ)); }
	DT_FORMATION_DATA stFormationInfo; //阵型信息
}PKT_CLIGS_EMBATTLE_REQ;

typedef struct tagPKT_CLIGS_EMBATTLE_ACK{
	tagPKT_CLIGS_EMBATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EMBATTLE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_EMBATTLE_ACK;

typedef struct tagPKT_CLIGS_CLR_PLAYER_DATA_REQ{
	tagPKT_CLIGS_CLR_PLAYER_DATA_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_PLAYER_DATA_REQ)); }
	UINT8            byExt; //扩展数据
}PKT_CLIGS_CLR_PLAYER_DATA_REQ;

typedef struct tagPKT_CLIGS_MAKE_ITEM_REQ{
	tagPKT_CLIGS_MAKE_ITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MAKE_ITEM_REQ)); }
	UINT16           wItemKindID; //道具制作卷类型
	UINT16           wEquipKindID; //装备类型
	UINT16           wEquipIdx; //该同类型装备的序号，用于区别相同类型的装备
}PKT_CLIGS_MAKE_ITEM_REQ;

typedef struct tagPKT_CLIGS_MAKE_ITEM_ACK{
	tagPKT_CLIGS_MAKE_ITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MAKE_ITEM_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_ITEM_DATA_CLI2 stItemInfo; //生成的道具信息
	DT_ITEM_DATA_LIST_CLI2 stGoodsBagItemInfo; //物品背包信息
	UINT64           qwBlueGas; //当前蓝气
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息
	DT_ITEM_DATA_LIST_CLI3 stGoodsBagItemInfo2; //物品背包信息
	DT_ITEM_NUM_DATA_LIST_CLI stBagItemNumInfo; //背包中各物品的当前数量信息
}PKT_CLIGS_MAKE_ITEM_ACK;

typedef struct tagPKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展字段，未使用
}PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wRecvCountDown; //领取在线奖励倒计时，单位秒
	UINT8            byNextOnlineEncourageFlag; //是否存在下一个在线奖励信息
	DT_ONLINE_ENCOURAGE_DATA astNextOnlineEncourage[1]; //下一个在线奖励信息
	DT_DATA_AFTER_ONLINE_ENCOURAGE stCurPlayerInfo; //当前玩家信息
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
}PKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK;

typedef struct tagDT_DATA_AFTER_PASS_ENCOURAGE_BOX{
	UINT64           qwCurCoin; //铜钱
	UINT64           qwCurGold; //元宝
	UINT64           qwCurBlueGas; //当前蓝气
}DT_DATA_AFTER_PASS_ENCOURAGE_BOX;

typedef struct tagPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ{
	tagPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ)); }
	UINT8            byExt; //扩展字段，未使用
}PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ;

typedef struct tagPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK{
	tagPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage; //通关奖励宝箱信息
	DT_DATA_AFTER_PASS_ENCOURAGE_BOX stAfterPassEncourage; //领取通关奖励宝箱后玩家信息
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
}PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK;

typedef struct tagPKT_CLIGS_INCREASE_BAG_SIZE_REQ{
	tagPKT_CLIGS_INCREASE_BAG_SIZE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_INCREASE_BAG_SIZE_REQ)); }
	UINT8            byInc2Size; //增加到的容量
	UINT8            byGoldFlag; //是否使用元宝，1为使用元宝扩容
}PKT_CLIGS_INCREASE_BAG_SIZE_REQ;

typedef struct tagPKT_CLIGS_INCREASE_BAG_SIZE_ACK{
	tagPKT_CLIGS_INCREASE_BAG_SIZE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_INCREASE_BAG_SIZE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurGold; //玩家当前的元宝
}PKT_CLIGS_INCREASE_BAG_SIZE_ACK;

typedef struct tagPKT_CLIGS_ONHOOK_REQ{
	tagPKT_CLIGS_ONHOOK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ONHOOK_REQ)); }
	DT_INSTANCE_ID   stInstanceID; //副本ID
	UINT8            byRunTimes; //副本扫荡次数
}PKT_CLIGS_ONHOOK_REQ;

typedef struct tagDT_HERO_UPGRADE_DATA2{
	UINT16           wHeroKindID; //武将类型ID
	UINT8            byUpgradeFlag; //是否升级
	UINT16           wStartLevel; //开始等级
	UINT16           wCurLevel; //当前等级
	UINT32           dwEncExperience; //奖励经验
	UINT32           dwExperience; //当前经验
	UINT32           dwUpgradeExperience; //下一级升级经验
	UINT16           wCanUseBoxGoodsID; //可使用礼包物品ID,0为没有
}DT_HERO_UPGRADE_DATA2;

typedef struct tagDT_DATA_AFTER_ONHOOK{
	UINT8            byUpgradeHeroNum; //升级武将个数
	DT_HERO_UPGRADE_DATA2 astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //当前阅历
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //当前玩家体力信息
	DT_EXPERIENCE_DATA stExperienceInfo; //经验信息
	UINT16           wUnlockEquipLevel; //解锁装备提示(0表示没有)
	UnlockScienceInfo stUnlockScienceList; //解锁科技提示
	UINT8            byUnlockNewInstanceFlag; //是否解锁新副本
	DT_INSTANCE_ID   astUnlockCommonInstanceID[1]; //解锁的普通副本ID
	UINT8            byHaveNewInstanceFlag; //是否有新可打普通副本,1有效
	DT_INSTANCE_DATA_CLI astNewCommonInstanceID[1]; //新可打普通副本ID
	UINT64           qwCurBlueGas; //当前蓝气
}DT_DATA_AFTER_ONHOOK;

typedef struct tagDT_PLAYER_RES_DATA{
	UINT8            byResKind; //资源类型
	UINT64           qwResValue; //资源值
}DT_PLAYER_RES_DATA;

typedef struct tagDT_PLAYER_RES_LST_DATA{
	UINT8            byResNum; //资源个数
	DT_PLAYER_RES_DATA astPlayerRes[MAX_PLAYER_RES_NUM]; //资源信息
}DT_PLAYER_RES_LST_DATA;

typedef struct tagPKT_CLIGS_ONHOOK_ACK{
	tagPKT_CLIGS_ONHOOK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ONHOOK_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_ONHOOK_RESULT stOnhookResult; //扫荡结果
	DT_DATA_AFTER_ONHOOK stAfterOnhookData; //扫荡玩家后信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	UINT8            byUnlockNewInstanceFlag; //是否解锁新副本
	DT_INSTANCE_ID   astUnlockCommonInstanceID[1]; //解锁的普通副本ID
	UINT8            byHaveNewInstanceFlag; //是否有新可打普通副本,1有效
	DT_INSTANCE_DATA_CLI astNewCommonInstanceID[1]; //新可打普通副本ID
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	UINT32           dwFlyExp; //伙伴等级已到120级卡在飞升期间时累积的经验
}PKT_CLIGS_ONHOOK_ACK;

typedef struct tagPKT_CLIGS_CLR_CHALLENGE_CD_REQ{
	tagPKT_CLIGS_CLR_CHALLENGE_CD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_CHALLENGE_CD_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_CLR_CHALLENGE_CD_REQ;

typedef struct tagPKT_CLIGS_CLR_CHALLENGE_CD_ACK{
	tagPKT_CLIGS_CLR_CHALLENGE_CD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_CHALLENGE_CD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurGold; //玩家当前的元宝
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_CLR_CHALLENGE_CD_ACK;

typedef struct tagPKT_CLIGS_SYNC_TIME_REQ{
	tagPKT_CLIGS_SYNC_TIME_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SYNC_TIME_REQ)); }
	UINT64           qwCliTime; //客户端当前时间
}PKT_CLIGS_SYNC_TIME_REQ;

typedef struct tagPKT_CLIGS_SYNC_TIME_ACK{
	tagPKT_CLIGS_SYNC_TIME_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SYNC_TIME_ACK)); }
	UINT64           qwCliTime; //客户端当前时间
	UINT64           qwServTime; //服务器当前时间
}PKT_CLIGS_SYNC_TIME_ACK;

typedef struct tagPKT_CLILS_REGISTER_DSPNAME_REQ{
	tagPKT_CLILS_REGISTER_DSPNAME_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_REGISTER_DSPNAME_REQ)); }
	TCHAR            aszDspName[USERNAME_LEN]; //战力变化信息
}PKT_CLILS_REGISTER_DSPNAME_REQ;

typedef struct tagPKT_CLILS_REGISTER_DSPNAME_ACK{
	tagPKT_CLILS_REGISTER_DSPNAME_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_REGISTER_DSPNAME_ACK)); }
	UINT16           wErrCode; //战力变化信息
	TCHAR            aszDspName[USERNAME_LEN]; //战力变化信息
}PKT_CLILS_REGISTER_DSPNAME_ACK;

typedef struct tagPKT_CLILS_USE_GOODS_REQ{
	tagPKT_CLILS_USE_GOODS_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_USE_GOODS_REQ)); }
	UINT16           wGoodsKindID; //物品类型ID
	UINT8            byGetBagInfoFlag; //是否返回背包信息
	UINT16           wUseNum; //使用个数
}PKT_CLILS_USE_GOODS_REQ;

typedef struct tagPKT_CLILS_USE_GOODS_ACK{
	tagPKT_CLILS_USE_GOODS_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_USE_GOODS_ACK)); }
	UINT16           wErrCode; //战力变化信息
	UINT64           qwCoin; //当前玩家铜钱
	UINT64           qwGold; //当前玩家元宝
	UINT16           wPhyStrength; //当前玩家体力
	UINT64           qwScience; //当前玩家器魂点
	UINT8            byMainHeroUpgradeFlag; //是否有主将升级信息
	DT_HERO_UPGRADE_DATA astMainHeroUpgradeInfo[1]; //主将升级信息
	UINT8            byUseBoxFlag; //是否是宝箱使用
	DT_ITEM_DATA_LIST_CLI2 astBoxItemInfo[1]; //宝箱道具信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT64           qwBlueGas; //当前玩家蓝气点
	UINT64           qwPurpleGas; //当前玩家紫气点
	UINT64           qwJingJie; //当前玩家境界点
	UINT8            byGetBagInfoFlag; //是否返回背包信息
	DT_ITEM_DATA_LIST_CLI2 astBagInfo[1]; //背包信息
	UINT16           wGoodsKindID; //物品类型ID
	UINT16           wUseNum; //使用个数
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
	DT_ITEM_DATA_LIST_CLI3 astBagInfo2[1]; //背包信息
	DT_ITEM_NUM_DATA_LIST_CLI stBagItemNumInfo; //背包中各物品的当前数量信息
}PKT_CLILS_USE_GOODS_ACK;

typedef struct tagPKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ{
	tagPKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ;

typedef struct tagDT_LOGIN_ENCOURAG_DATA{
	DT_ITEM_DATA_CLI astItemInfo[MAX_LOGIN_ENCOURAGE_ITEM_NUM]; //所有奖励的道具
	UINT8            byHitIdx; //当前选中的序号(从1开始)
	UINT8            byHaveSelectFlag; //是否已经选过
	UINT16           wKeepDays; //连续登录天数
	UINT8            byCanRecvNumToday; //当天剩余奖励次数
}DT_LOGIN_ENCOURAG_DATA;

typedef struct tagPKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK{
	tagPKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //战力变化信息
	DT_LOGIN_ENCOURAG_DATA stLoginEncourageInfo; //登录奖励信息
}PKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK;

typedef struct tagPKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ{
	tagPKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ;

typedef struct tagPKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK{
	tagPKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK;

typedef struct tagPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ{
	tagPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ;

typedef struct tagPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK{
	tagPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK)); }
	UINT16           wErrCode; //战力变化信息
	UINT8            byHaveEncourageFlag; //当天是否还有奖励
	DT_LOGIN_ENCOURAG_DATA astLoginEncourageInfo[1]; //登录奖励信息
}PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK;

typedef struct tagDT_CAPTURE_WORK_DATA_CLI{
	UINT32           dwWorkCountdown; //俘虏工作倒计时(单位秒)
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	UINT32           dwCaptureWorkSecond; //俘虏工作时间
	UINT32           dwCollectIncome; //总共收益值
	UINT32           dwUncollectIncome; //未收取收益
	UINT32           dwAllCollectIncome; //剩余所有可收取收益
}DT_CAPTURE_WORK_DATA_CLI;

typedef struct tagDT_CAPTURE_DATA_CLI{
	UINT32           dwCapturePlayerID; //被俘虏的玩家ID
	TCHAR            aszCaptureDispName[USERNAME_LEN]; //俘虏昵称
	UINT16           wCaptureCareerID; //俘虏职业
	UINT16           wCaptureLevel; //俘虏等级
	UINT8            byCaptureGridIdx; //俘虏格子序号(从0开始)
	UINT32           dwReleaseCountdown; //俘虏释放倒计时(单位秒,硬性释放时间)
	DT_CAPTURE_WORK_DATA_CLI stWorkInfo; //俘虏工作收益信息
}DT_CAPTURE_DATA_CLI;

typedef struct tagPKT_CLIGS_PLUNDER_REQ{
	tagPKT_CLIGS_PLUNDER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLUNDER_REQ)); }
	UINT32           dwEnemyID; //要战斗的玩家ID
	UINT32           dwCaptureID; //俘虏玩家ID(和EnemyID相同表示掠夺的是CaptureID玩家，不同表示抢夺CaptureID俘虏)
	UINT8            byUseItemFlag; //是否使用道具掠夺低于5级以上的玩家
	UINT8            byResistFlag; //是否反抗(1表示反抗)
	UINT8            byCaptureGridIdx; //俘虏格子序号(从0开始)
}PKT_CLIGS_PLUNDER_REQ;

typedef struct tagDT_PLUNDER_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜币
	UINT32           dwScience; //器魂点
}DT_PLUNDER_ENCOURAGE_DATA;

typedef struct tagDT_BECAPTURE_DATA{
	UINT32           dwOwnerID; //俘虏主ID(0表示没有被俘虏)
	TCHAR            aszOwnerDispName[USERNAME_LEN]; //俘虏主昵称
	UINT16           wOwnerLevel; //俘虏主等级
	UINT32           dwOwnerPower; //俘虏主战力
	UINT32           dwReleaseCountdown; //释放倒计时(单位秒,硬性释放时间)
}DT_BECAPTURE_DATA;

typedef struct tagPKT_CLIGS_PLUNDER_ACK3{
	tagPKT_CLIGS_PLUNDER_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_PLUNDER_ACK3)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwEnemyID; //要战斗的玩家ID
	UINT32           dwCaptureID; //俘虏玩家ID(和EnemyID相同表示掠夺的是CaptureID玩家，不同表示抢夺CaptureID俘虏)
	UINT8            byUseItemFlag; //是否使用道具掠夺低于5级以上的玩家
	UINT8            byResistFlag; //是否反抗(1表示反抗)
	UINT8            byCaptureGridIdx; //俘虏格子序号(从0开始)
	DT_BECAPTURE_DATA stBeCaptureInfo; //俘虏玩家的被俘信息
	DT_CHALLENGE_PLAYER_DATA stEnemyPlayerData; //战斗敌方玩家信息
	UINT8            byEnemyHeroNum; //战斗敌方信息
	DT_ENEMY_HERO_DATA astEnemyInfoList[MAX_FORMATION_IDX_NUM]; //战斗敌方信息
	DT_BATTLE_DATA3  stBattleInfo; //战斗信息
	DT_PLUNDER_ENCOURAGE_DATA stPlunderEncourage; //掠夺奖励
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝
	UINT16           wCanPlunderNum; //玩家当前可掠夺次数
	UINT16           wVipExtNum; //VIP格外增加的次数
	UINT16           wFreePlunderNum; //玩家免费掠夺次数
	UINT32           dwPlunderGold; //收费掠夺元宝(若还有免费的则为0)
	UINT16           wPlunderItemNum; //当前拥有的掠夺道具
	UINT8            byCaptureFlag; //是否俘虏玩家，1为对方成为你的俘虏
	DT_CAPTURE_DATA_CLI astCaptureInfo[1]; //俘虏信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_PLUNDER_ACK3;

typedef struct tagDT_PLUNDER_BATTLELOG_DATA_CLI{
	DT_PLUNDER_BATTLELOG_DATA stBattelogInfo; //战报信息
	TCHAR            aszActiveDispName[USERNAME_LEN]; //主动方玩家昵称
	TCHAR            aszPassiveDispName[USERNAME_LEN]; //被动方玩家昵称
	TCHAR            aszCaptureDispName[USERNAME_LEN]; //俘虏昵称
}DT_PLUNDER_BATTLELOG_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_CAPTURE_TAB_REQ{
	tagPKT_CLIGS_OPEN_CAPTURE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CAPTURE_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_CAPTURE_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_CAPTURE_TAB_ACK{
	tagPKT_CLIGS_OPEN_CAPTURE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CAPTURE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurCoin; //当前铜币
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurScience; //当前器魂点
	UINT32           dwCurPower; //当前战力
	DT_BECAPTURE_DATA stOwner; //俘虏主信息
	UINT16           wPlunderNumPerDay; //玩家每天可掠夺次数
	UINT16           wCanPlunderNum; //玩家当前可掠夺次数
	UINT16           wVipExtNum; //VIP格外增加的次数
	UINT16           wFreeCanPlunderNum; //玩家当前可免费掠夺次数
	UINT8            byCaptureNum; //俘虏个数
	DT_CAPTURE_DATA_CLI astCaptureInfo[MAX_CAPTURE_NUM]; //俘虏信息
	UINT16           awUnlockGridLevel[MAX_CAPTURE_NUM]; //俘虏格子解锁等级信息(按顺序)
	UINT8            byUnreadPlunderBattleLogNum; //未观看掠夺战报个数
	DT_PLUNDER_BATTLELOG_DATA_CLI stLastBattleLog; //最后一条战报
	UINT8            byBleedPerGoldClrMin; //榨干每元宝所清除分钟
	UINT32           dwDriveAwayGold; //驱赶所需元宝
	DT_PLUNDER_GRID_DATA astCaptureGridInfo[MAX_CAPTURE_NUM]; //俘虏格子信息(格子没俘虏时生效)
	UINT32           dwPlunderGold; //收费掠夺元宝
}PKT_CLIGS_OPEN_CAPTURE_TAB_ACK;

typedef struct tagDT_ENEMY_DATA_CLI{
	UINT32           dwEnemyPlayerID; //仇敌玩家ID
	TCHAR            aszEnemyDispName[USERNAME_LEN]; //仇敌昵称
	UINT16           wCareerID; //仇敌职业
	UINT16           wEnemyLevel; //仇敌等级
	UINT16           wBattleNum; //交手次数
	UINT32           dwPower; //战力
	UINT8            byHaveOwnerFlag; //是否有俘虏主
	DT_BECAPTURE_DATA astOwnerInfo[1]; //俘虏主信息
}DT_ENEMY_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_ENEMY_TAB_REQ{
	tagPKT_CLIGS_OPEN_ENEMY_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ENEMY_TAB_REQ)); }
	UINT16           wStartIdx; //起始仇敌序号，从0开始
}PKT_CLIGS_OPEN_ENEMY_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_ENEMY_TAB_ACK{
	tagPKT_CLIGS_OPEN_ENEMY_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ENEMY_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wStartIdx; //起始仇敌序号，从0开始
	UINT16           wTotalNum; //仇敌总个数
	UINT16           wCurNum; //当前返回仇敌个数
	DT_ENEMY_DATA_CLI astEnemyInfo[MAX_GET_ENEMY_NUM]; //仇敌信息
}PKT_CLIGS_OPEN_ENEMY_TAB_ACK;

typedef struct tagPKT_CLIGS_DEL_ENEMY_REQ{
	tagPKT_CLIGS_DEL_ENEMY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DEL_ENEMY_REQ)); }
	UINT32           dwEnemyID; //仇敌玩家ID
	UINT16           wStartIdx; //当前页起始个数序号，从0开始
}PKT_CLIGS_DEL_ENEMY_REQ;

typedef struct tagPKT_CLIGS_DEL_ENEMY_ACK{
	tagPKT_CLIGS_DEL_ENEMY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DEL_ENEMY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwEnemyID; //仇敌玩家ID
	UINT16           wStartIdx; //起始个数序号，从0开始
	UINT16           wTotalNum; //仇敌总个数
	UINT16           wCurNum; //当前返回仇敌个数
	DT_ENEMY_DATA_CLI astEnemyInfo[MAX_GET_ENEMY_NUM]; //仇敌信息
}PKT_CLIGS_DEL_ENEMY_ACK;

typedef struct tagPKT_CLIGS_PLUNDER_BATTLELOG_NTF{
	tagPKT_CLIGS_PLUNDER_BATTLELOG_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_PLUNDER_BATTLELOG_NTF)); }
	UINT8            byUnreadNum; //未观看战报个数
	DT_PLUNDER_BATTLELOG_DATA_CLI stBattleLog; //战报
	UINT64           qwPlayerCoin; //玩家当前铜币
	UINT64           qwPlayerScience; //玩家当前器魂点
}PKT_CLIGS_PLUNDER_BATTLELOG_NTF;

typedef struct tagPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ{
	tagPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ;

typedef struct tagDT_PLUNDER_PLAYER_DATA{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT16           wMainHeroKindID; //主角ID
	UINT8            byRobotFunc; //机器人功能,>0则为机器人，不同的值为不同的功能。0为玩家
	UINT16           wLevel; //等级
	UINT32           dwPower; //战力
	UINT8            byHaveOwnerFlag; //是否有俘虏主
	DT_BECAPTURE_DATA astOwnerInfo[1]; //俘虏主信息
}DT_PLUNDER_PLAYER_DATA;

typedef struct tagPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK{
	tagPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCanPlunderNum; //玩家当前可掠夺次数
	UINT16           wFreePlunderNum; //玩家免费掠夺次数
	UINT32           dwPlunderGold; //收费掠夺元宝(若还有免费的则为0)
	UINT8            byUseItemGapLevel; //相差多少级需要1个掠夺道具
	UINT16           wPlunderItemNum; //当前拥有的掠夺道具
	UINT16           wPlunderItemID; //掠夺道具ID
	UINT32           dwPlunderItemPrice; //掠夺道具价格
	UINT32           dwClrCDGold; //清除CD所需元宝(每分钟)
	UINT8            byPlunderPlayerNum; //当前返回推荐的掠夺玩家个数
	DT_PLUNDER_PLAYER_DATA astPlunderPlayerInfo[MAX_GET_PLUNDER_ENEMY_NUM]; //推荐的掠夺玩家信息
}PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK;

typedef struct tagPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ{
	tagPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ;

typedef struct tagPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK{
	tagPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byPlunderPlayerNum; //当前返回玩家个数
	DT_PLUNDER_PLAYER_DATA astPlunderPlayerInfo[MAX_GET_PLUNDER_ENEMY_NUM]; //玩家信息
}PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK;

typedef struct tagPKT_CLIGS_FREE_CAPTURE_REQ{
	tagPKT_CLIGS_FREE_CAPTURE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FREE_CAPTURE_REQ)); }
	UINT32           dwCapturePlayerID; //俘虏玩家ID
}PKT_CLIGS_FREE_CAPTURE_REQ;

typedef struct tagPKT_CLIGS_FREE_CAPTURE_ACK{
	tagPKT_CLIGS_FREE_CAPTURE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FREE_CAPTURE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCapturePlayerID; //俘虏玩家ID
	UINT32           dwCoinIncome; //收益铜币
	UINT32           dwScienceIncome; //收益器魂
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurScience; //玩家当前的器魂点
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_FREE_CAPTURE_ACK;

typedef struct tagPKT_CLIGS_COLLECT_INCOME_REQ{
	tagPKT_CLIGS_COLLECT_INCOME_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COLLECT_INCOME_REQ)); }
	UINT32           dwCapturePlayerID; //俘虏玩家ID
	UINT32           dwIncomeValue; //收取收益值
}PKT_CLIGS_COLLECT_INCOME_REQ;

typedef struct tagPKT_CLIGS_COLLECT_INCOME_ACK{
	tagPKT_CLIGS_COLLECT_INCOME_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COLLECT_INCOME_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCapturePlayerID; //俘虏玩家ID
	UINT32           dwIncomeValue; //收取收益值(返回实际收益)
	DT_CAPTURE_WORK_DATA_CLI stWorkInfo; //俘虏工作收益信息
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT8            byHalved; //收益减半标志
	UINT8            byCaptureReleaseFlag; //是否已释放俘虏，1为释放
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_COLLECT_INCOME_ACK;

typedef struct tagPKT_CLIGS_BLEED_CAPTURE_REQ{
	tagPKT_CLIGS_BLEED_CAPTURE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BLEED_CAPTURE_REQ)); }
	UINT32           dwCapturePlayerID; //俘虏玩家ID
}PKT_CLIGS_BLEED_CAPTURE_REQ;

typedef struct tagPKT_CLIGS_BLEED_CAPTURE_ACK{
	tagPKT_CLIGS_BLEED_CAPTURE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BLEED_CAPTURE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCapturePlayerID; //俘虏玩家ID
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	UINT32           dwIncomeValue; //收益值
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_BLEED_CAPTURE_ACK;

typedef struct tagPKT_CLIGS_SET_WORK_TYPE_REQ{
	tagPKT_CLIGS_SET_WORK_TYPE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SET_WORK_TYPE_REQ)); }
	UINT32           dwCapturePlayerID; //俘虏玩家ID
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
}PKT_CLIGS_SET_WORK_TYPE_REQ;

typedef struct tagPKT_CLIGS_SET_WORK_TYPE_ACK{
	tagPKT_CLIGS_SET_WORK_TYPE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SET_WORK_TYPE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCapturePlayerID; //俘虏玩家ID
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	DT_CAPTURE_WORK_DATA_CLI stWorkInfo; //俘虏工作收益信息
}PKT_CLIGS_SET_WORK_TYPE_ACK;

typedef struct tagDT_MONEY_TREE_DATA_CLI{
	UINT16           wCanRockNum; //今天剩余可摇钱次数
	UINT16           wVipExtNum; //VIP格外增加的次数
	UINT16           wTotalCanRockNum; //今天可摇钱次数
	UINT32           dwRockNeedGold; //摇钱所需元宝
	UINT8            byMoodValue; //心情值
	UINT32           dwRockBaseCoin; //摇钱基础铜币
	UINT8            byGetCoinRate; //总共收益倍率(百分比)
	UINT16           wMoodKeepTime; //心情值保持时间(单位秒)
	UINT16           wMoodOverCountDown; //心情值结束倒计时
}DT_MONEY_TREE_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_MONEY_TREE_REQ{
	tagPKT_CLIGS_OPEN_MONEY_TREE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MONEY_TREE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_MONEY_TREE_REQ;

typedef struct tagPKT_CLIGS_OPEN_MONEY_TREE_ACK{
	tagPKT_CLIGS_OPEN_MONEY_TREE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MONEY_TREE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_MONEY_TREE_DATA_CLI stMoneyTreeInfo; //摇钱树信息
	UINT16           wGetCoinRate2; //总共收益倍率(百分比)
}PKT_CLIGS_OPEN_MONEY_TREE_ACK;

typedef struct tagPKT_CLIGS_ROCK_MONEY_TREE_REQ{
	tagPKT_CLIGS_ROCK_MONEY_TREE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ROCK_MONEY_TREE_REQ)); }
	UINT8            byGuideFlag; //引导标志(1为引导请求,必中1心情值)
}PKT_CLIGS_ROCK_MONEY_TREE_REQ;

typedef struct tagPKT_CLIGS_ROCK_MONEY_TREE_ACK{
	tagPKT_CLIGS_ROCK_MONEY_TREE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ROCK_MONEY_TREE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byGuideFlag; //引导标志(1为引导请求,必中1心情值)
	DT_MONEY_TREE_DATA_CLI stMoneyTreeInfo; //摇钱树信息
	UINT32           dwGetCoin; //获得铜币
	UINT64           qwPlayerCoin; //玩家当前铜币
	UINT64           qwPlayerGold; //玩家当前元宝
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT16           wGetCoinRate2; //总共收益倍率(百分比)
}PKT_CLIGS_ROCK_MONEY_TREE_ACK;

typedef struct tagPKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ{
	tagPKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ;

typedef struct tagPKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK{
	tagPKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCaptureOwnerID; //俘虏主
	UINT8            byShowNum; //显示战报个数
	DT_PLUNDER_BATTLELOG_DATA_CLI astBattleLogInfo[MAX_PLUNDER_BATTLELOG_NUM]; //战报信息
}PKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK;

typedef struct tagDT_GODWEAPON_ATTR_DATA{
	DT_BATTLE_ATTRIBUTE stBattleAttr; //战斗属性
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttr; //隐藏的战斗属性
	DT_EXT_BATTLE_ATTRIBUTE stExtBattleAttr; //扩展的战斗属性
}DT_GODWEAPON_ATTR_DATA;

typedef struct tagDT_GODWEAPON_DATA_CLI{
	UINT16           wHeroKindID; //武将类型ID
	UINT16           wMaxQuality; //最大品质
	UINT16           wQuality; //当前品质
	UINT16           wLevel; //等级
	DT_GODWEAPON_ATTR_DATA stIncAttr; //神器提升的属性
	DT_ATTR_INC_DATA stNextLevelIncAttr; //下一级提升的属性
	UINT16           wQualityRequireLevel; //升阶神器玩家等级要求
	UINT32           dwUpgrageScience; //下一升级所需器魂(科技)点
	UINT64           qwRecoverScience; //释放神器返还的器魂(科技)点
	UINT64           qwPerfectRecoverScience; //完美释放神器返还的器魂(科技)点
	UINT16           wUpgradeQualityNeedItemNum; //进阶需要的进阶道具个数
	UINT8            byMaxLevelFlag; //是达到最大等级(1为达到最大等级)
	UINT8            byEncAttrNum; //当前品质
	DT_ATTR_INC_DATA astQualityUpgradeEncAttr[MAX_GODWEAPON_QUALITY_NUM]; //所有品的升级奖励属性
}DT_GODWEAPON_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_GODWEAPON_REQ{
	tagPKT_CLIGS_OPEN_GODWEAPON_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GODWEAPON_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_GODWEAPON_REQ;

typedef struct tagPKT_CLIGS_OPEN_GODWEAPON_ACK{
	tagPKT_CLIGS_OPEN_GODWEAPON_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GODWEAPON_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wUpgradeQualityItemNum; //当前拥有的进阶道具
	UINT32           dwUpgradeQualityItemPrice; //进阶道具价格
	UINT16           wUpgradeQualityItemID; //进阶符ID
	UINT32           dwActiveCoin; //激活所需铜币
	UINT64           qwPlayerScience; //玩家当前的器魂(科技)点
	UINT16           wGodweaponNum; //神器数目
	DT_GODWEAPON_DATA_CLI astGodweaponLst[MAX_HERO_NUM]; //神器信息
	UINT8            byOpenNeedVipLevel; //完美释放开启需要的VIP等级
	UINT8            byPerfectFreeGodweapon; //完美释放器魂是否可用,0为不可用
	UINT16           wPerfectFreeGodweaponCost; //完美释放器魂消耗元宝
	UINT64           qwCurGold; //当前元宝
	TCHAR            aszExtContext[MAX_COMMON_TXT_LEN]; //面板扩展信息
}PKT_CLIGS_OPEN_GODWEAPON_ACK;

typedef struct tagPKT_CLIGS_UPGRADE_GODWEAPON_REQ{
	tagPKT_CLIGS_UPGRADE_GODWEAPON_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_GODWEAPON_REQ)); }
	UINT16           wHeroKindID; //武将类型ID
	UINT16           wQuality; //品质
	UINT16           wLevel; //等级(进阶时无效)
}PKT_CLIGS_UPGRADE_GODWEAPON_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_GODWEAPON_ACK{
	tagPKT_CLIGS_UPGRADE_GODWEAPON_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_GODWEAPON_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwPlayerScience; //玩家当前的器魂(科技)点
	UINT16           wUpgradeQualityItemNum; //当前拥有的进阶道具
	DT_GODWEAPON_DATA_CLI stGodweaponInfo; //神器信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	UINT64           qwCurCoin; //当前铜钱
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_UPGRADE_GODWEAPON_ACK;

typedef struct tagPKT_CLIGS_FREE_GODWEAPON_REQ{
	tagPKT_CLIGS_FREE_GODWEAPON_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FREE_GODWEAPON_REQ)); }
	UINT16           wHeroKindID; //武将类型ID
	UINT8            byUseGold; //是否使用元宝释放，非0为使用
}PKT_CLIGS_FREE_GODWEAPON_REQ;

typedef struct tagPKT_CLIGS_FREE_GODWEAPON_ACK{
	tagPKT_CLIGS_FREE_GODWEAPON_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FREE_GODWEAPON_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将类型ID
	UINT64           qwPlayerScience; //玩家当前的器魂(科技)点
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_GODWEAPON_DATA_CLI stGodweaponInfo; //神器信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT64           qwCurGold; //当前元宝
}PKT_CLIGS_FREE_GODWEAPON_ACK;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_TAB_REQ{
	tagPKT_CLIGS_OPEN_AWAKEN_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_TAB_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_AWAKEN_TAB_REQ;

typedef struct tagDT_STUDY_DATA_CLI{
	UINT8            byStudyColorKind; //心得颜色，从1开始，0表示没有数据
	UINT8            byStudyAttrKind; //心得类型(对应属性类型)
	UINT8            byStudyLevel; //心得等级(从1开始，不会有0)
	UINT32           dwStudyAttrValue; //心得属性值
	UINT32           dwStudyExp; //心得经验值
	UINT32           dwNextLevelAttrValue; //下一级心得属性值
	UINT32           dwUpgradeNeedExp; //升级到下一级所需心得经验
}DT_STUDY_DATA_CLI;

typedef struct tagDT_AWAKEN_TAB_DATA{
	DT_AWAKEN_BASE_DATA1 stBaseInfo; //悟道基本信息
	DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT16           wMaxStudyLevel; //心得最大等级
	UINT32           adwAwakenCoin[MAX_AWAKEN_NUM]; //悟道所需铜币
	UINT32           dwAwakenGold; //悟道所需元宝
	UINT16           wTatalCallNum; //总共可召唤次数
	UINT16           wCanCallNum; //剩余可召唤次数
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
}DT_AWAKEN_TAB_DATA;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_TAB_ACK{
	tagPKT_CLIGS_OPEN_AWAKEN_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_AWAKEN_TAB_DATA stAwakenInfo; //悟道面板信息
}PKT_CLIGS_OPEN_AWAKEN_TAB_ACK;

typedef struct tagPKT_CLIGS_AWAKEN_REQ{
	tagPKT_CLIGS_AWAKEN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_AWAKEN_REQ)); }
	UINT8            byQuickFlag; //是否使用一键悟道，一键悟道时元宝悟道无效
	UINT8            byGoldFlag; //是否使用元宝悟道
}PKT_CLIGS_AWAKEN_REQ;

typedef struct tagDT_AWAKEN_GET_DATA{
	UINT32           dwGetCoin; //所得铜钱
	UINT8            byStudyBagIdx; //所得心得序号(对应心得背包序号，从0开始)，所得铜钱为0时有效
	UINT8            byAwakenLevel; //悟道等级
}DT_AWAKEN_GET_DATA;

typedef struct tagPKT_CLIGS_AWAKEN_ACK{
	tagPKT_CLIGS_AWAKEN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_AWAKEN_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byQuickFlag; //是否使用一键悟道，一键悟道时元宝悟道无效
	UINT8            byGoldFlag; //是否使用元宝悟道
	UINT8            byAwakenNum; //总共悟道次数
	DT_AWAKEN_GET_DATA astAwakenGetInfo[MAX_AWAKEN_NUM]; //悟道所得信息
	DT_AWAKEN_TAB_DATA stAwakenTabInfo; //悟道面板信息
}PKT_CLIGS_AWAKEN_ACK;

typedef struct tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ{
	tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ;

typedef struct tagDT_HERO_STUDY_DATA_CLI{
	UINT16           wHeroID; //武将ID
	DT_STUDY_DATA_CLI astStudyInfo[MAX_HERO_STUDY_GRID_NUM]; //心得信息
}DT_HERO_STUDY_DATA_CLI;

typedef struct tagDT_DRESS_STUDY_TAB_DATA{
	DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT16           awStudyUnlockLevelInfo[MAX_HERO_STUDY_GRID_NUM]; //心得解锁等级信息
	UINT8            byHeroNum; //武将个数
	DT_HERO_STUDY_DATA_CLI astHeroStudyInfo[MAX_FORMATION_IDX_NUM]; //武将心得信息
	UINT64           qwStudyExp; //心得经验
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
}DT_DRESS_STUDY_TAB_DATA;

typedef struct tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK{
	tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_DRESS_STUDY_TAB_DATA stDressStudyTabInfo; //穿戴心得面板信息
}PKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK;

typedef struct tagPKT_CLIGS_DRESS_STUDY_REQ{
	tagPKT_CLIGS_DRESS_STUDY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_STUDY_REQ)); }
	UINT16           wHeroKindID; //武将ID
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byHeroGridIdx; //武将心得位置，从0开始
}PKT_CLIGS_DRESS_STUDY_REQ;

typedef struct tagPKT_CLIGS_DRESS_STUDY_ACK{
	tagPKT_CLIGS_DRESS_STUDY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_STUDY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将ID
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byHeroGridIdx; //武将心得位置，从0开始
	DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	DT_HERO_STUDY_DATA_CLI stHeroStudyInfo; //武将心得信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
}PKT_CLIGS_DRESS_STUDY_ACK;

typedef struct tagPKT_CLIGS_STUDY_TO_EXP_REQ{
	tagPKT_CLIGS_STUDY_TO_EXP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_TO_EXP_REQ)); }
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byAllFlag; //转换所有心得背包中的心得,1转换所有
	UINT8            abySelectConvertColor[MAX_COLOR_NUM]; //选择转换的颜色(对应颜色下标-1，0表示不转换，1表示需转换)
}PKT_CLIGS_STUDY_TO_EXP_REQ;

typedef struct tagPKT_CLIGS_STUDY_TO_EXP_ACK{
	tagPKT_CLIGS_STUDY_TO_EXP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_TO_EXP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            abySelectConvertColor[MAX_COLOR_NUM]; //转换的颜色(对应颜色下标-1，0表示不转换，1表示需转换)
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byAllFlag; //转换所有心得背包中的心得,1转换所有
	UINT32           dwGenStudyExp; //转换所得心得经验
	DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT64           qwCurStudyExp; //当前心得经验
}PKT_CLIGS_STUDY_TO_EXP_ACK;

typedef struct tagPKT_CLIGS_STUDY_UPGRADE_REQ{
	tagPKT_CLIGS_STUDY_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_UPGRADE_REQ)); }
	UINT16           wHeroKindID; //武将ID
	UINT8            byGridIdx; //心得位置，从0开始
	UINT8            byLevel; //要升级到的等级
}PKT_CLIGS_STUDY_UPGRADE_REQ;

typedef struct tagPKT_CLIGS_STUDY_UPGRADE_ACK{
	tagPKT_CLIGS_STUDY_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将ID
	UINT8            byGridIdx; //心得位置，从0开始
	UINT8            byLevel; //要升级到的等级
	DT_HERO_STUDY_DATA_CLI stHeroStudyInfo; //武将心得信息
	UINT64           qwCurStudyExp; //当前心得经验
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
}PKT_CLIGS_STUDY_UPGRADE_ACK;

typedef struct tagPKT_CLIGS_RACE_BATTLELOG_NTF{
	tagPKT_CLIGS_RACE_BATTLELOG_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_RACE_BATTLELOG_NTF)); }
	UINT8            byUnreadNum; //未观看战报个数
	DT_RACE_BATTLELOG_DATA_CLI stBattleLog; //战报
	UINT64           qwPlayerCoin; //玩家当前铜币
	UINT64           qwPlayerScience; //玩家当前器魂点
}PKT_CLIGS_RACE_BATTLELOG_NTF;

typedef struct tagPKT_CLIGS_BUY_PHYSTRENGTH_REQ{
	tagPKT_CLIGS_BUY_PHYSTRENGTH_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_PHYSTRENGTH_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_BUY_PHYSTRENGTH_REQ;

typedef struct tagPKT_CLIGS_BUY_PHYSTRENGTH_ACK{
	tagPKT_CLIGS_BUY_PHYSTRENGTH_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_PHYSTRENGTH_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCurPhyStrength; //当前体力
	UINT64           qwCurGold; //当前元宝
	UINT8            byCanBuyNum; //剩余购买次数
	UINT16           wVipExtNum; //VIP格外增加的购买次数
	UINT32           dwNextBuyGold; //下次购买所需元宝
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
}PKT_CLIGS_BUY_PHYSTRENGTH_ACK;

typedef struct tagPKT_CLIGS_CLR_PLUNDER_CD_REQ{
	tagPKT_CLIGS_CLR_PLUNDER_CD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_PLUNDER_CD_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_CLR_PLUNDER_CD_REQ;

typedef struct tagPKT_CLIGS_CLR_PLUNDER_CD_ACK{
	tagPKT_CLIGS_CLR_PLUNDER_CD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_PLUNDER_CD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurGold; //玩家当前的元宝
}PKT_CLIGS_CLR_PLUNDER_CD_ACK;

typedef struct tagPKT_CLIGS_OPEN_MAINTOWN_REQ{
	tagPKT_CLIGS_OPEN_MAINTOWN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MAINTOWN_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_MAINTOWN_REQ;

typedef struct tagPKT_CLIGS_OPEN_MAINTOWN_ACK{
	tagPKT_CLIGS_OPEN_MAINTOWN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MAINTOWN_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCurLevel; //当前玩家等级
	UINT8            byCanCrowFlag; //是否可收获，1表示可收获
	UINT32           dwCrownCoin; //可收获铜钱
	UINT32           dwCrownGold; //可收获元宝
	UINT32           dwCrownJingJie; //可收获境界点
	UINT32           dwLevyCountDown; //征收的倒计时
	UINT32           dwLevyCoin; //可征收的铜币
	UINT32           dwLevyJingJie; //可征收的境界点
	UINT8            byLevyNum; //可征收的次数
	UINT8            byTotalLevyNum; //征收的总次数
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurJingJie; //当前境界点
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_OPEN_MAINTOWN_ACK;

typedef struct tagPKT_CLIGS_RECV_MAINTOWN_INCOME_REQ{
	tagPKT_CLIGS_RECV_MAINTOWN_INCOME_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_MAINTOWN_INCOME_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_RECV_MAINTOWN_INCOME_REQ;

typedef struct tagPKT_CLIGS_RECV_MAINTOWN_INCOME_ACK{
	tagPKT_CLIGS_RECV_MAINTOWN_INCOME_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_MAINTOWN_INCOME_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byCanCrowFlag; //是否可收获，1表示可收获
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurJingJie; //当前境界
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_RECV_MAINTOWN_INCOME_ACK;

typedef struct tagPKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ{
	tagPKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ;

typedef struct tagPKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK{
	tagPKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurJingJie; //当前境界
	UINT32           dwLevyCountDown; //征收的倒计时
	UINT8            byLevyNum; //可征收的次数
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK;

typedef struct tagPKT_CLIGS_DRIVE_AWAY_OWNER_REQ{
	tagPKT_CLIGS_DRIVE_AWAY_OWNER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRIVE_AWAY_OWNER_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_DRIVE_AWAY_OWNER_REQ;

typedef struct tagPKT_CLIGS_DRIVE_AWAY_OWNER_ACK{
	tagPKT_CLIGS_DRIVE_AWAY_OWNER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRIVE_AWAY_OWNER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurGold; //当前元宝
	UINT32           dwDriveAwayGold; //驱赶所需元宝
}PKT_CLIGS_DRIVE_AWAY_OWNER_ACK;

typedef struct tagPKT_CLIGS_COMPOSE_EQUIP_REQ{
	tagPKT_CLIGS_COMPOSE_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COMPOSE_EQUIP_REQ)); }
	UINT16           wEquipItemID; //装备ID
	UINT16           wEquipIdx; //装备ID
	UINT8            byGoldComposeFlag; //元宝合成标志,1为元宝合成
}PKT_CLIGS_COMPOSE_EQUIP_REQ;

typedef struct tagPKT_CLIGS_COMPOSE_EQUIP_ACK{
	tagPKT_CLIGS_COMPOSE_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COMPOSE_EQUIP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wSrcEquipItemID; //原装备ID
	UINT16           wSrcEquipIdx; //原装备ID
	UINT8            bySrcItemNum; //原装备材料信息
	DT_NEED_ITEM_DATA astStrItemInfoList[MAX_ITEM_MAKER_NEED_NUM]; //原装备材料信息所需道具信息
	DT_EQUIP_DATA_CLI stEquipInfo; //装备信息
	DT_EQUIP_DATA_STRENGTHEN_CLI stEquipStrengthenInfo; //装备强化信息
	UINT64           qwCurGold; //玩家当前元宝
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
}PKT_CLIGS_COMPOSE_EQUIP_ACK;

typedef struct tagDT_ACTIVITY_TIME_DATA{
	UINT8            byStartHour; //活动开启时间，小时
	UINT8            byStartMin; //活动开启时间，分钟
	UINT16           wKeepTime; //活动持续时间
	UINT16           wEndCountDown; //活动结束倒计时倒计时,在活动时间内有效
	UINT16           wAheadTime; //提前开放时间(单位秒)
	UINT8            byHaveFinishFlag; //是否已完成，1已完成
}DT_ACTIVITY_TIME_DATA;

typedef struct tagDT_ACTIVITY_BASE_DATA{
	UINT8            byKindID; //活动类型
	UINT8            byActivityTimeNum; //活动时间段数
	DT_ACTIVITY_TIME_DATA astActivityTimeInfo[MAX_ACTIVITY_TIME_NUM]; //活动时间段信息
	UINT8            byNotity; //是否打开通知
}DT_ACTIVITY_BASE_DATA;

typedef struct tagPKT_CLIGS_OPEN_PHYSTRENGTH_REQ{
	tagPKT_CLIGS_OPEN_PHYSTRENGTH_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PHYSTRENGTH_REQ)); }
	UINT8            byExt; //扩展字段，暂时未用
}PKT_CLIGS_OPEN_PHYSTRENGTH_REQ;

typedef struct tagPKT_CLIGS_OPEN_PHYSTRENGTH_ACK{
	tagPKT_CLIGS_OPEN_PHYSTRENGTH_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PHYSTRENGTH_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byStrength; //体力值
	UINT8            byTotalBuyTimes; //总共购买次数
	UINT8            byBuyTimes; //购买次数
	UINT16           wVipExtNum; //VIP格外增加的挑战次数
	UINT32           dwBuyGold; //花费的元宝
	UINT8            byActivityNum; //体力活动数
	DT_ACTIVITY_BASE_DATA astActivityInfo[MAX_ACTIVITY_NUM]; //体力活动
}PKT_CLIGS_OPEN_PHYSTRENGTH_ACK;

typedef struct tagPKT_CLIGS_RECV_LEFT_INCOME_REQ{
	tagPKT_CLIGS_RECV_LEFT_INCOME_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_LEFT_INCOME_REQ)); }
	UINT8            byGridIdx; //格子序号(从0开始)
}PKT_CLIGS_RECV_LEFT_INCOME_REQ;

typedef struct tagPKT_CLIGS_RECV_LEFT_INCOME_ACK{
	tagPKT_CLIGS_RECV_LEFT_INCOME_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_LEFT_INCOME_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byGridIdx; //格子序号(从0开始)
	UINT8            byIncomeType; //收益类型(0未设置，1铜币，2气魂)
	UINT32           dwIncomeValue; //收益值
	UINT64           qwCurCoin; //当前铜币
	UINT64           qwCurScience; //当前器魂
}PKT_CLIGS_RECV_LEFT_INCOME_ACK;

typedef struct tagPKT_CLIGS_SMITHY_BUY_EQUIP_REQ{
	tagPKT_CLIGS_SMITHY_BUY_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SMITHY_BUY_EQUIP_REQ)); }
	UINT16           wHeroKindID; //武将ID
	UINT8            byEquipPos; //装备位置
}PKT_CLIGS_SMITHY_BUY_EQUIP_REQ;

typedef struct tagPKT_CLIGS_SMITHY_BUY_EQUIP_ACK{
	tagPKT_CLIGS_SMITHY_BUY_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SMITHY_BUY_EQUIP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_EQUIP_DATA_CLI stEquipInfo; //装备信息
	DT_EQUIP_DATA_STRENGTHEN_CLI stEquipStrengthenInfo; //装备强化信息
	UINT64           qwCurCoin; //玩家当前的铜钱
	DT_POWER_CHANGE_DATA stPowerInfo; //玩家战力信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT32           dwStrengthen10Coin; //装备升10级所需要铜钱信息
}PKT_CLIGS_SMITHY_BUY_EQUIP_ACK;

typedef struct tagPKT_CLIGS_CLR_STRENGTHEN_CD_REQ{
	tagPKT_CLIGS_CLR_STRENGTHEN_CD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_STRENGTHEN_CD_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_CLR_STRENGTHEN_CD_REQ;

typedef struct tagPKT_CLIGS_CLR_STRENGTHEN_CD_ACK{
	tagPKT_CLIGS_CLR_STRENGTHEN_CD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_STRENGTHEN_CD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurGold; //玩家当前的元宝
}PKT_CLIGS_CLR_STRENGTHEN_CD_ACK;

typedef struct tagDT_CROP_SEED_INFO_DATA{
	UINT16           wCropKindID; //种子ID
	UINT16           wCropSeedNum; //种子个数
	UINT8            byCropType; //种植类型
	UINT32           dwCropTime; //种植时间
	UINT32           dwCropValue; //种子收益
	UINT16           wCropGainKindID; //收益的物品ID
	UINT32           dwCoin; //种子铜币价格
	UINT32           dwGold; //种子元宝价格
	UINT16           wComposeCount; //种子合成所需要个数
	UINT16           wComposeKindID; //合成种子ID
}DT_CROP_SEED_INFO_DATA;

typedef struct tagDT_CROP_FIELD_INFO_DATA{
	UINT8            byOpen; //田地开放
	UINT8            byLevel; //需要的等级
	UINT32           dwCoin; //花费的铜币
	UINT32           dwGold; //花费的元宝
}DT_CROP_FIELD_INFO_DATA;

typedef struct tagDT_CROP_MORE_INFO_DATA{
	UINT16           wCropKindID; //种植ID
	UINT32           dwCropTime; //收获倒计时间
	UINT8            byCropType; //种植类型
	UINT32           dwCropValue; //种子收益
	UINT16           wCropGainKindID; //收益的物品ID
}DT_CROP_MORE_INFO_DATA;

typedef struct tagPKT_CLIGS_OPEN_CROP_REQ{
	tagPKT_CLIGS_OPEN_CROP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_CROP_REQ;

typedef struct tagPKT_CLIGS_OPEN_CROP_ACK{
	tagPKT_CLIGS_OPEN_CROP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byCropSeedNum; //种子个数
	DT_CROP_SEED_INFO_DATA astCropSeedInfoList[MAX_CROP_SEED_NUM]; //种子信息
	DT_CROP_FIELD_INFO_DATA astCropFieldInfoList[MAX_CROP_NUM]; //田地信息
	DT_CROP_MORE_INFO_DATA astCropMoreInfoList[MAX_CROP_NUM]; //种植信息
	UINT16           wNoCDGoldPerMin; //立即完成每分钟消耗元宝
	UINT8            byBagFullFlag; //背包是否满，1为满
	UINT64           qwGold; //元宝
	UINT64           qwCoin; //铜钱
	UINT64           qwScience; //器魂
	UINT64           qwStory; //阅历
	UINT64           qwJingJie; //境界
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_OPEN_CROP_ACK;

typedef struct tagPKT_CLIGS_OPEN_CROP_FIELD_REQ{
	tagPKT_CLIGS_OPEN_CROP_FIELD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_FIELD_REQ)); }
	UINT8            byFieldID; //田地ID
}PKT_CLIGS_OPEN_CROP_FIELD_REQ;

typedef struct tagPKT_CLIGS_OPEN_CROP_FIELD_ACK{
	tagPKT_CLIGS_OPEN_CROP_FIELD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_FIELD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFieldID; //田地ID
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
	DT_CROP_FIELD_INFO_DATA stCropFieldInfo; //田地信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_OPEN_CROP_FIELD_ACK;

typedef struct tagPKT_CLIGS_CROP_REQ{
	tagPKT_CLIGS_CROP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CROP_REQ)); }
	UINT8            byFieldID; //田地ID
	UINT16           wCropKindID; //田子ID
}PKT_CLIGS_CROP_REQ;

typedef struct tagPKT_CLIGS_CROP_ACK{
	tagPKT_CLIGS_CROP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CROP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFieldID; //田地ID
	DT_CROP_MORE_INFO_DATA stCropMoreInfo; //种植信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_CROP_FIELD_INFO_DATA stCropFieldInfo; //田地信息
}PKT_CLIGS_CROP_ACK;

typedef struct tagPKT_CLIGS_GAIN_CROP_REQ{
	tagPKT_CLIGS_GAIN_CROP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GAIN_CROP_REQ)); }
	UINT8            byFieldID; //田地ID
}PKT_CLIGS_GAIN_CROP_REQ;

typedef struct tagPKT_CLIGS_GAIN_CROP_ACK{
	tagPKT_CLIGS_GAIN_CROP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GAIN_CROP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCoin; //铜钱
	UINT64           qwScience; //器魂
	UINT64           qwStory; //阅历
	UINT64           qwJingJie; //境界
	UINT8            byCropType; //种植类型
	UINT32           dwCropValue; //种子收益
	UINT16           wCropKindID; //生成物品ID
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_CROP_MORE_INFO_DATA stCropMoreInfo; //种植信息
	DT_CROP_FIELD_INFO_DATA stCropFieldInfo; //田地信息
}PKT_CLIGS_GAIN_CROP_ACK;

typedef struct tagPKT_CLIGS_CLR_CROP_REQ{
	tagPKT_CLIGS_CLR_CROP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_CROP_REQ)); }
	UINT8            byFieldID; //田地ID
}PKT_CLIGS_CLR_CROP_REQ;

typedef struct tagPKT_CLIGS_CLR_CROP_ACK{
	tagPKT_CLIGS_CLR_CROP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_CROP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFieldID; //田地ID
	UINT64           qwGold; //元宝
	DT_CROP_MORE_INFO_DATA stCropMoreInfo; //种植信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_CLR_CROP_ACK;

typedef struct tagPKT_CLIGS_OPEN_EXPERIENCE_REQ{
	tagPKT_CLIGS_OPEN_EXPERIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EXPERIENCE_REQ)); }
	UINT16           wHeroID; //武将ID
}PKT_CLIGS_OPEN_EXPERIENCE_REQ;

typedef struct tagDT_EXPERIENCE_DRUG_DATA{
	UINT16           wExperienceDrugID; //经验丹等级
	UINT32           dwExperience; //增加经验值
	UINT16           wBagNum; //背包经验丹个数
}DT_EXPERIENCE_DRUG_DATA;

typedef struct tagDT_GOLD_EXPERIENCE_DRUG_DATA{
	UINT16           wExperienceDrugID; //经验丹ID（99）
	UINT32           dwExperience; //增加经验值
	UINT32           dwCostGold; //消耗元宝
	UINT16           wRemainNum; //经验丹剩余使用
	UINT16           wOpenVipLevel; //开启的VIP等级
	UINT16           wMaxNum; //当前VIP总次数
	UINT16           wNextVipLevelMaxNum; //下级VIP总次数，如果VIP满级，则为0
}DT_GOLD_EXPERIENCE_DRUG_DATA;

typedef struct tagPKT_CLIGS_OPEN_EXPERIENCE_ACK{
	tagPKT_CLIGS_OPEN_EXPERIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EXPERIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wLevel; //武将等级
	UINT32           dwExperience; //当前经验
	UINT32           dwUpgradeExperience; //升级经验
	DT_EXPERIENCE_DRUG_DATA astExperienceDrugDataList[3]; //经验丹信息
	DT_GOLD_EXPERIENCE_DRUG_DATA stGoldExperienceDrugData; //元宝经验丹信息
}PKT_CLIGS_OPEN_EXPERIENCE_ACK;

typedef struct tagPKT_CLIGS_USER_EXPERIENCE_REQ{
	tagPKT_CLIGS_USER_EXPERIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_USER_EXPERIENCE_REQ)); }
	UINT16           wHeroID; //武将ID
	UINT16           wExperienceDrugID; //经验丹ID
	UINT16           wRemainGoldExperienceDrugCnt; //经验丹次数（如果是元宝经验丹，客户端则需要上传当前剩余次数）
}PKT_CLIGS_USER_EXPERIENCE_REQ;

typedef struct tagPKT_CLIGS_USER_EXPERIENCE_ACK{
	tagPKT_CLIGS_USER_EXPERIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_USER_EXPERIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_EXPERIENCE_DRUG_DATA stExperienceDrugData; //经验丹信息
	DT_GOLD_EXPERIENCE_DRUG_DATA stGoldExperienceDrugData; //元宝经验丹信息
	DT_POWER_CHANGE_DATA stPowerChangeData; //武将战力信息
	DT_HERO_BASE_DATA_CLI stHeroBaseDataCli; //武将相关信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT64           qwCurGold; //当前元宝
}PKT_CLIGS_USER_EXPERIENCE_ACK;

typedef struct tagPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ{
	tagPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ;

typedef struct tagDT_SCIENCE_ALREADY_OPEN_DATA{
	UINT8            byKind; //属性类型
	UINT16           wLevel; //级别
	UINT32           dwCurValue; //当前数值
	UINT32           dwNextValue; //升级后数值
	UINT32           dwCostStory; //升级所需阅历
	UINT16           wNeedPlayerLevel; //升级所需人物等级
	UINT8            byMaxLevel; //是否满级
}DT_SCIENCE_ALREADY_OPEN_DATA;

typedef struct tagDT_SCIENCE_WILL_OPEN_DATA{
	UINT8            byKind; //属性类型
	UINT16           wOpenLevel; //开放等级
	UINT32           dwLv1Value; //第一级的数值
	UINT32           dwCostStory; //升级所需阅历
}DT_SCIENCE_WILL_OPEN_DATA;

typedef struct tagPKT_CLIGS_OPEN_SCIENCE_BUILD_ACK{
	tagPKT_CLIGS_OPEN_SCIENCE_BUILD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SCIENCE_BUILD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurStory; //当前阅历值
	UINT8            byAlreadyOpenScienceNum; //已经在升级的科技数
	DT_SCIENCE_ALREADY_OPEN_DATA astAlreadyOpenScienceList[MAX_SCIENCE_KIND_NUM]; //已经在升级的科技列表
	UINT8            byWillOpenScienceNum; //快要可以打开的科技数
	DT_SCIENCE_WILL_OPEN_DATA astWillOpenScienceList[MAX_SCIENCE_KIND_NUM]; //快要可以打开的科技列表
}PKT_CLIGS_OPEN_SCIENCE_BUILD_ACK;

typedef struct tagPKT_CLIGS_UPDATE_SCIENCE_REQ{
	tagPKT_CLIGS_UPDATE_SCIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPDATE_SCIENCE_REQ)); }
	UINT16           wCurLevel; //当前等级
	UINT8            byKind; //具体升级的类型
}PKT_CLIGS_UPDATE_SCIENCE_REQ;

typedef struct tagPKT_CLIGS_UPDATE_SCIENCE_ACK{
	tagPKT_CLIGS_UPDATE_SCIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPDATE_SCIENCE_ACK)); }
	UINT16           wErrorCode; //错误类型
	DT_SCIENCE_ALREADY_OPEN_DATA stUpdatedData; //升级后变更的信息
	DT_POWER_CHANGE_DATA stPower; //战力改变情况
	UINT64           qwCurStory; //当前阅历值
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_UPDATE_SCIENCE_ACK;

typedef struct tagPKT_CLIGS_OPEN_MELT_EQUIP_REQ{
	tagPKT_CLIGS_OPEN_MELT_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MELT_EQUIP_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_MELT_EQUIP_REQ;

typedef struct tagPKT_CLIGS_OPEN_MELT_EQUIP_ACK{
	tagPKT_CLIGS_OPEN_MELT_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MELT_EQUIP_ACK)); }
	UINT16           wErrorCode; //错误类型
	DT_EQUIP_MELT_DATA_LST_CLI stEquipInfo; //装备信息
	DT_GOOD_MELT_DATA_LST_CLI stGoodInfo; //物品信息
}PKT_CLIGS_OPEN_MELT_EQUIP_ACK;

typedef struct tagDT_MELT_EQUIP_DATA{
	UINT16           wEquipID; //装备ID
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
}DT_MELT_EQUIP_DATA;

typedef struct tagDT_MELT_GOOD_DATA{
	UINT16           wKindID; //物品ID
	UINT16           wNum; //物品个数
}DT_MELT_GOOD_DATA;

typedef struct tagPKT_CLIGS_MELT_REQ{
	tagPKT_CLIGS_MELT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MELT_REQ)); }
	UINT16           wEquipNum; //装备个数
	DT_MELT_EQUIP_DATA astMeltEquipDataList[MAX_BAG_OPEN_NUM]; //装备信息
	UINT8            byGoldFlag; //是否元宝熔炼,1元宝熔炼
	UINT16           wGoodNum; //物品个数
	DT_MELT_GOOD_DATA astMeltGoodDataList[MAX_BAG_OPEN_NUM]; //物品信息
}PKT_CLIGS_MELT_REQ;

typedef struct tagPKT_CLIGS_MELT_ACK{
	tagPKT_CLIGS_MELT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MELT_ACK)); }
	UINT16           wErrorCode; //错误类型
	UINT32           dwAddScience; //增加器魂点
	UINT64           qwScience; //当前器魂点
	DT_EQUIP_MELT_DATA_LST_CLI stEquipInfo; //装备信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	UINT64           qwCurGold; //玩家当前的元宝点
	DT_GOOD_MELT_DATA_LST_CLI stGoodInfo; //物品信息
}PKT_CLIGS_MELT_ACK;

typedef struct tagPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ{
	tagPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ)); }
	UINT16           wKindID; //伙伴类型ID
	UINT8            byAttrKind; //属性类型ID
}PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ;

typedef struct tagDT_UPDATE_ATTR_DATA{
	UINT8            byAttrKind; //属性类型
	UINT16           wCurCount; //当前次数
	UINT16           wTotalCount; //总次数
	UINT32           dwCurLevelValue; //当前数值
	UINT32           dwNextLevelValue; //下级数值
	UINT32           dwCostCoin; //升级消耗铜钱数量
	UINT32           dwCostItemNum; //升级消耗道具数量
	UINT64           qwCurCoinNum; //铜钱
	UINT32           dwItemID; //物品ID
	UINT32           dwItemNum; //物品的数量
}DT_UPDATE_ATTR_DATA;

typedef struct tagPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK{
	tagPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK)); }
	UINT16           wErrorCode; //错误类型
	DT_UPDATE_ATTR_DATA stUpdateAttr; //升级属性
}PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK;

typedef struct tagPKT_CLIGS_UPDATE_ATTR_REQ{
	tagPKT_CLIGS_UPDATE_ATTR_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPDATE_ATTR_REQ)); }
	UINT16           wKindID; //伙伴类型ID
	UINT16           wCurCount; //当前次数
	UINT8            byAttrType; //属性类型
	UINT8            byUpdateType; //具体升级方式
}PKT_CLIGS_UPDATE_ATTR_REQ;

typedef struct tagDT_UPDATED_ATTR_BATTLE_DATA{
	UINT32           dwOldTotalPower; //旧的总战力
	UINT32           dwTotalPower; //总战力
	UINT32           dwSelfOldPower; //旧的战力
	UINT32           dwSelfPower; //战力
	UINT32           dwHPValue; //血量
	UINT32           dwAttackValue; //攻击力
}DT_UPDATED_ATTR_BATTLE_DATA;

typedef struct tagPKT_CLIGS_UPDATE_ATTR_ACK{
	tagPKT_CLIGS_UPDATE_ATTR_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPDATE_ATTR_ACK)); }
	UINT16           wErrorCode; //错误类型
	UINT16           wKindID; //武将类型
	DT_UPDATE_ATTR_DATA stAttrRet; //当前属性升级信息
	DT_UPDATED_ATTR_BATTLE_DATA stBattleInfo; //属性升级后的战斗信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
}PKT_CLIGS_UPDATE_ATTR_ACK;

typedef struct tagPKT_CLIGS_COMPOSE_REQ{
	tagPKT_CLIGS_COMPOSE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COMPOSE_REQ)); }
	UINT16           wItemID; //合成的物品ID
	UINT8            byComposeFlag; //合成类型标识，1为镶嵌面板合成,0为普通合成,2 为人物面板装备合成，3为人物面板饰品合成
}PKT_CLIGS_COMPOSE_REQ;

typedef struct tagPKT_CLIGS_COMPOSE_ACK{
	tagPKT_CLIGS_COMPOSE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COMPOSE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_ITEM_DATA_CLI2 stItemInfo; //生成的道具信息
	DT_ITEM_DATA_LIST_CLI2 stGoodsBagItemInfo; //物品背包信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	DT_ITEM_DATA_LIST_CLI3 stGoodsBagItemInfo2; //物品背包信息
	DT_ITEM_NUM_DATA_LIST_CLI stBagItemNumInfo; //背包中各物品的当前数量信息
}PKT_CLIGS_COMPOSE_ACK;

typedef struct tagPKT_CLIGS_REFRESH_ELITE_REQ{
	tagPKT_CLIGS_REFRESH_ELITE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_REFRESH_ELITE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_REFRESH_ELITE_REQ;

typedef struct tagPKT_CLIGS_REFRESH_ELITE_ACK{
	tagPKT_CLIGS_REFRESH_ELITE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_REFRESH_ELITE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurGold; //当前玩家元宝
}PKT_CLIGS_REFRESH_ELITE_ACK;

typedef struct tagPKT_CLIGS_ONHOOK_ELITE_REQ{
	tagPKT_CLIGS_ONHOOK_ELITE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ONHOOK_ELITE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_ONHOOK_ELITE_REQ;

typedef struct tagPKT_CLIGS_ONHOOK_ELITE_ACK{
	tagPKT_CLIGS_ONHOOK_ELITE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ONHOOK_ELITE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_ONHOOK_ELITE_ACK;

typedef struct tagPKT_CLIGS_GET_ITEM_REQ{
	tagPKT_CLIGS_GET_ITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ITEM_REQ)); }
	UINT16           wItemKindID; //物品ID
}PKT_CLIGS_GET_ITEM_REQ;

typedef struct tagPKT_CLIGS_GET_ITEM_ACK{
	tagPKT_CLIGS_GET_ITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ITEM_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wItemKindID; //物品ID
	UINT16           wItemNum; //背包物品个数
	UINT8            byshop; //j商店物品是否可购买
	UINT8            byTab; //商店分类
	DT_INSTANCE_ID   stBurstInstance; //可能掉落的副本
}PKT_CLIGS_GET_ITEM_ACK;

typedef struct tagPKT_CLIGS_GET_ORDER_ID_REQ{
	tagPKT_CLIGS_GET_ORDER_ID_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ORDER_ID_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_GET_ORDER_ID_REQ;

typedef struct tagPKT_CLIGS_GET_ORDER_ID_ACK{
	tagPKT_CLIGS_GET_ORDER_ID_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ORDER_ID_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	TCHAR            aszOrderID[MAX_ORDER_ID_LEN]; //订单号
}PKT_CLIGS_GET_ORDER_ID_ACK;

typedef struct tagPKT_CLIGS_EXCHANGE_RESULT{
	tagPKT_CLIGS_EXCHANGE_RESULT() { memset(this, 0, sizeof(tagPKT_CLIGS_EXCHANGE_RESULT)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwGold; //当前元宝数
	UINT32           dwExchangeGold; //充值元宝数
	UINT8            byVipLevel; //VIP等级
	UINT32           dwTotalExchangeGold; //总充值数
}PKT_CLIGS_EXCHANGE_RESULT;

typedef struct tagPKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ{
	tagPKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ)); }
	CHAR             szTransactionID[MAX_RECEIPT_DATA_LEN]; //订单号
	CHAR             szReceiptData[MAX_RECEIPT_DATA_LEN]; //客户端AppStore返回的充值字段
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ;

typedef struct tagPKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK{
	tagPKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK)); }
	UINT16           wErrCode; //错误码
	UINT32           dwExchangeGold; //充值元宝数
	UINT64           qwGold; //当前元宝数
	UINT8            byVipLevel; //VIP等级
	UINT32           dwTotalExchangeGold; //总充值数
	CHAR             szTransactionID[MAX_RECEIPT_DATA_LEN]; //订单号
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK;

typedef struct tagPKT_CLIGS_SEED_COMPOSE_REQ{
	tagPKT_CLIGS_SEED_COMPOSE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEED_COMPOSE_REQ)); }
	UINT16           wItemID; //合成的物品ID
}PKT_CLIGS_SEED_COMPOSE_REQ;

typedef struct tagPKT_CLIGS_SEED_COMPOSE_ACK{
	tagPKT_CLIGS_SEED_COMPOSE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SEED_COMPOSE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_ITEM_DATA_CLI2 stItemInfo; //生成的道具信息
	UINT8            byCropSeedNum; //种子个数
	DT_CROP_SEED_INFO_DATA astCropSeedInfoList[MAX_CROP_SEED_NUM]; //种子信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
}PKT_CLIGS_SEED_COMPOSE_ACK;

typedef struct tagCHAT_DATA{
	UINT32           dwSerialNumber; //聊天流水号
	UINT32           dwPlayerID; //聊天的玩家ID
	TCHAR            aszUserName[USERNAME_LEN]; //聊天的玩家名称
	UINT8            byChatType; //聊天类型
	UINT64           qwChatTime; //聊天时间
	UINT32           dwChatLen; //聊天长度
	UINT8            abyChatContent[MAX_CHAT_CONTENT_NUM]; //聊天内容
}CHAT_DATA;

typedef struct tagPKT_CLIGS_OPEN_CHAT_REQ{
	tagPKT_CLIGS_OPEN_CHAT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CHAT_REQ)); }
	UINT32           dwSerialNumber; //聊天流水号
}PKT_CLIGS_OPEN_CHAT_REQ;

typedef struct tagPKT_CLIGS_OPEN_CHAT_ACK{
	tagPKT_CLIGS_OPEN_CHAT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CHAT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byChatNum; //聊天记录个数
	CHAT_DATA        astChatData[MAX_CHAT_NUM]; //聊天内容
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息(有值则错误，优先于错误码)
	UINT8            abyVipLevel[MAX_CHAT_NUM]; //聊天玩家vip等级（放这里主要为与旧协议兼容）
	UINT8            abyFactionIconIDList[MAX_CHAT_NUM]; //玩家所在门派的门派图标ID，值为NULL_FACTION_ICON_ID时代表无门派（版本号<303的客户端将无视此字段）
}PKT_CLIGS_OPEN_CHAT_ACK;

typedef struct tagPKT_CLIGS_CHAT_REQ{
	tagPKT_CLIGS_CHAT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHAT_REQ)); }
	UINT32           dwPlayerID; //聊天的玩家ID
	UINT8            byChatType; //聊天类型
	UINT32           dwChatLen; //聊天类型
	UINT8            abyChatContent[MAX_CHAT_CONTENT_NUM]; //聊天内容
}PKT_CLIGS_CHAT_REQ;

typedef struct tagPKT_CLIGS_CHAT_ACK{
	tagPKT_CLIGS_CHAT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CHAT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	CHAT_DATA        stChatData; //聊天内容
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息(有值则错误，优先于错误码)
	UINT8            byVipLevel; //聊天玩家vip等级（放这里主要为与旧协议兼容）
	UINT8            byFactionIconID; //玩家所在门派的门派图标ID，值为NULL_FACTION_ICON_ID时代表无门派（版本号<303的客户端将无视此字段）
}PKT_CLIGS_CHAT_ACK;

typedef struct tagPKT_CLIGS_CHAT_NTF{
	tagPKT_CLIGS_CHAT_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_CHAT_NTF)); }
	UINT8            byChatNum; //聊天记录个数
	CHAT_DATA        astChatData[MAX_CHAT_NUM]; //聊天内容
	UINT8            abyVipLevel[MAX_CHAT_NUM]; //聊天玩家vip等级（放这里主要为与旧协议兼容）
	UINT8            abyFactionIconIDList[MAX_CHAT_NUM]; //玩家所在门派的门派图标ID，值为NULL_FACTION_ICON_ID时代表无门派（版本号<303的客户端将无视此字段）
}PKT_CLIGS_CHAT_NTF;

typedef struct tagPKT_CLIGS_CROSS_DAY_NTF{
	tagPKT_CLIGS_CROSS_DAY_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_CROSS_DAY_NTF)); }
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT8            byOnlineEncourageFlag; //是否存在在线奖励信息
	DT_ONLINE_ENCOURAGE_DATA astOnlineEncourage[1]; //当前在线奖励信息
	UINT16           awRecvCountDown[1]; //领取在线奖励倒计时，单位秒
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
}PKT_CLIGS_CROSS_DAY_NTF;

typedef struct tagPKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ{
	tagPKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ;

typedef struct tagPKT_CLIGS_OPNE_ACTIVITY_CENTER_ACK{
	tagPKT_CLIGS_OPNE_ACTIVITY_CENTER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPNE_ACTIVITY_CENTER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byActivityNum; //活动数
	DT_ACTIVITY_BASE_DATA astActivityInfo[MAX_ACTIVITY_NUM]; //活动
	UINT8            abyActivityStartCountDown[MAX_ACTIVITY_NUM]; //活动开启天数倒计时
}PKT_CLIGS_OPNE_ACTIVITY_CENTER_ACK;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_PROMPT_REQ{
	tagPKT_CLIGS_GET_ACTIVITY_PROMPT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_PROMPT_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_GET_ACTIVITY_PROMPT_REQ;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_PROMPT_ACK{
	tagPKT_CLIGS_GET_ACTIVITY_PROMPT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_PROMPT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FUNC_COMMON_PROMPT stPromptInfo; //提示信息
}PKT_CLIGS_GET_ACTIVITY_PROMPT_ACK;

typedef struct tagPKT_CLIGS_OPNE_EAT_BREAD_REQ{
	tagPKT_CLIGS_OPNE_EAT_BREAD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPNE_EAT_BREAD_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPNE_EAT_BREAD_REQ;

typedef struct tagPKT_CLIGS_OPNE_EAT_BREAD_ACK{
	tagPKT_CLIGS_OPNE_EAT_BREAD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPNE_EAT_BREAD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wEndCountDown; //活动结束倒计时
	UINT8            byMaxObtainPhyStrengthen; //可得到体力上限
	UINT16           wTimePerPhyStrengthen; //多久可获得1点体力
	UINT16           wOarNumPerPhyStrengthen; //划几次可获得1点体力
	UINT32           dwOnceSpeedCoin; //1倍数可获得铜钱
	UINT8            byHaveObtainPhyStrengthen; //已获得体力
	UINT32           dwHaveObtainCoin; //已获得铜钱
	UINT16           wHaveUseTime; //已用时间
	UINT16           wCurPhyStrength; //当前体力
	UINT64           qwCurCoin; //当前铜钱
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
}PKT_CLIGS_OPNE_EAT_BREAD_ACK;

typedef struct tagPKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ{
	tagPKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ)); }
	UINT8            byHaveObtainPhyStrengthenCli; //请求得到的体力(从活动开始到当前)
	UINT32           dwHaveObtainCoinCli; //请求得到的铜钱(从活动开始到当前)
}PKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ;

typedef struct tagPKT_CLIGS_OBTAIN_PHYSTRENGTHEN_ACK{
	tagPKT_CLIGS_OBTAIN_PHYSTRENGTHEN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OBTAIN_PHYSTRENGTHEN_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byHaveObtainPhyStrengthenCli; //请求得到的体力(从活动开始到当前)
	UINT32           dwHaveObtainCoinCli; //请求得到的铜钱(从活动开始到当前)
	UINT8            byHaveObtainPhyStrengthen; //得到的体力(从活动开始到当前)
	UINT32           dwHaveObtainCoin; //得到的铜钱(从活动开始到当前)
	UINT16           wHaveUseTime; //已用时间
}PKT_CLIGS_OBTAIN_PHYSTRENGTHEN_ACK;

typedef struct tagPKT_CLIGS_LEAVE_EAT_BREAD_REQ{
	tagPKT_CLIGS_LEAVE_EAT_BREAD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_EAT_BREAD_REQ)); }
	UINT8            byHaveObtainPhyStrengthenCli; //请求得到的体力(从活动开始到当前)
	UINT32           dwHaveObtainCoinCli; //请求得到的铜钱(从活动开始到当前)
}PKT_CLIGS_LEAVE_EAT_BREAD_REQ;

typedef struct tagPKT_CLIGS_LEAVE_EAT_BREAD_ACK{
	tagPKT_CLIGS_LEAVE_EAT_BREAD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_EAT_BREAD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byHaveObtainPhyStrengthen; //得到的体力(从活动开始到当前)
	UINT32           dwHaveObtainCoin; //得到的铜钱(从活动开始到当前)
	UINT16           wCurPhyStrength; //当前体力
	UINT64           qwCurCoin; //当前铜钱
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //体力信息
}PKT_CLIGS_LEAVE_EAT_BREAD_ACK;

typedef struct tagPKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ{
	tagPKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ;

typedef struct tagDT_BIAO_INFO{
	UINT8            byQualityLevel; //级别
	UINT32           dwCoin; //获得铜钱
	UINT32           dwScience; //获得器魂
	UINT32           dwJingjie; //获得境界点
	UINT16           wCostTime; //消耗时间（单位秒）
}DT_BIAO_INFO;

typedef struct tagBIAO_OPERATE_INFO{
	UINT8            byRemianOperateCnt; //可操作次数
	UINT8            byTotalOperateCnt; //总操作次数
}BIAO_OPERATE_INFO;

typedef struct tagPKT_CLIGS_OPEN_PAOSHANG_BUILD_ACK{
	tagPKT_CLIGS_OPEN_PAOSHANG_BUILD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PAOSHANG_BUILD_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byYabiaoFlag; //押镖标识(0为不在押镖，1为正在押镖中，2为押镖完毕，可以领取奖励)
	UINT16           wAlreadyYabiaoTime; //已经押镖时间（单位秒）
	DT_BIAO_INFO     stBiaoInfo; //镖信息
	UINT8            byRemainYabiaoCnt; //剩余押镖次数
	UINT8            byRemainJiebiaoCnt; //剩余拦截次数
	UINT16           wAllPlayerYabiaoCnt; //所有在押镖的人数
	UINT8            byEnemyYabiaoCnt; //所有在押镖的仇人数量
	UINT64           qwCoinEncourage; //铜钱
	UINT32           dwScienceEncourage; //器魂
	UINT16           wJiebiaoCD; //冷却时间
	UINT16           wClearCDCostGoldPerMin; //清除CD每分钟消耗元宝
}PKT_CLIGS_OPEN_PAOSHANG_BUILD_ACK;

typedef struct tagPKT_CLIGS_OPEN_YABIAO_REQ{
	tagPKT_CLIGS_OPEN_YABIAO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_YABIAO_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_YABIAO_REQ;

typedef struct tagPKT_CLIGS_OPEN_YABIAO_ACK{
	tagPKT_CLIGS_OPEN_YABIAO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_YABIAO_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byYabiaoing; //是否已经在押镖
	UINT16           wCurBiaoIndex; //当前镖的序号
	UINT16           wFlushCount; //刷新次数
	UINT16           wFlushCostGold; //刷新消耗元宝
	UINT16           wDirectMaxLevelCostGold; //达到最高级消耗的元宝
	UINT16           wBiaoQualityNum; //镖的品质数量
	DT_BIAO_INFO     astBiaoInfo[MAX_BIAO_QUALITY]; //镖品质的信息
	UINT8            byFreeFlushCnt; //镖的免费次数
	UINT8            byFreeFlushRemainCnt; //镖的免费已使用次数
}PKT_CLIGS_OPEN_YABIAO_ACK;

typedef struct tagPKT_CLIGS_FLUSH_BIAO_QUALITY_REQ{
	tagPKT_CLIGS_FLUSH_BIAO_QUALITY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FLUSH_BIAO_QUALITY_REQ)); }
	UINT16           wQuality; //当前品质
	UINT8            byDirectMaxLevel; //是否直接打到最大品质
}PKT_CLIGS_FLUSH_BIAO_QUALITY_REQ;

typedef struct tagPKT_CLIGS_FLUSH_BIAO_QUALITY_ACK{
	tagPKT_CLIGS_FLUSH_BIAO_QUALITY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FLUSH_BIAO_QUALITY_ACK)); }
	UINT16           wErrCode; //错误码
	DT_BIAO_INFO     stBiaoInfo; //镖信息
	UINT16           wFlushCount; //刷新次数
	UINT16           wFlushCostGold; //刷新消耗元宝
	UINT64           qwCurGold; //当前元宝
	UINT8            byDirectMaxLevel; //是否直接打到最大品质（客户端传值直接返回）
	UINT8            byFreeFlushCnt; //镖的免费次数
	UINT8            byFreeFlushRemainCnt; //镖的免费已使用次数
}PKT_CLIGS_FLUSH_BIAO_QUALITY_ACK;

typedef struct tagPKT_CLIGS_BEGIN_YABIAO_REQ{
	tagPKT_CLIGS_BEGIN_YABIAO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_YABIAO_REQ)); }
	UINT16           wQuality; //品质
}PKT_CLIGS_BEGIN_YABIAO_REQ;

typedef struct tagPKT_CLIGS_BEGIN_YABIAO_ACK{
	tagPKT_CLIGS_BEGIN_YABIAO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_YABIAO_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wAlreadyYabiaoTime; //已经押镖时间（单位秒）
	UINT16           wTotalYabiaoTime; //押镖总时间（单位秒）
	BIAO_OPERATE_INFO stBeJiebiaoInfo; //可被拦截次数
	UINT64           qwBeJiebiaoCoinEncourage; //被拦截铜钱奖励
	UINT32           dwBeJiebiaoScienceEncourage; //拦截器魂奖励
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息（1可押镖，2表示押镖中，0表示无提示）
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT8            byFactionIconID; //门派头像ID
}PKT_CLIGS_BEGIN_YABIAO_ACK;

typedef struct tagPKT_CLIGS_OPEN_JIEBIAO_REQ{
	tagPKT_CLIGS_OPEN_JIEBIAO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JIEBIAO_REQ)); }
	UINT8            byViewYabiaoType; //拦截类型(0全部，1敌人)
	UINT16           wLongestYabiaoEndTime; //所有押镖玩家的剩余最长押镖时间
	UINT8            byNeedPlayerNum; //还需角色数
}PKT_CLIGS_OPEN_JIEBIAO_REQ;

typedef struct tagDT_YABIAO_PLAYER_INFO{
	UINT32           dwID; //玩家唯一ID
	UINT16           wKindID; //玩家职业ID
	UINT16           wLevel; //玩家等级
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT32           dwPower; //战力
	UINT8            byQuality; //品质
	BIAO_OPERATE_INFO stBeJiebiaoInfo; //可被劫次数
	UINT16           wAlreadyYabiaoTime; //已经押镖时间（单位秒）
	UINT16           wTotalYabiaoTime; //押镖总时间（单位秒）
	UINT64           qwCoinEncourage; //拦截奖励
	UINT32           dwScienceEncourage; //拦截器魂奖励
	UINT8            byCanJiebiao; //是否可劫镖
}DT_YABIAO_PLAYER_INFO;

typedef struct tagDT_YABIAO_PLAYER_INFO_FACTION_NAME{
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //当前门派名称
	UINT8            bySameFaction; //是否在同一个门派中
}DT_YABIAO_PLAYER_INFO_FACTION_NAME;

typedef struct tagPKT_CLIGS_OPEN_JIEBIAO_ACK{
	tagPKT_CLIGS_OPEN_JIEBIAO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JIEBIAO_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byYabiaoPlayerNum; //玩家数
	DT_YABIAO_PLAYER_INFO astYabiaoPlayerInfo[MAX_YABIAO_PLAYER_NUM]; //玩家数据
	UINT16           wLongestYabiaoEndTime; //所有押镖玩家的剩余最长押镖时间
	UINT16           wJiebiaoCD; //冷却时间
	UINT8            byRemainJiebiaoCnt; //剩余拦截次数
	UINT16           wClearCDCostGoldPerMin; //清除CD每分钟消耗元宝
	UINT8            byYabiaoFlag; //押镖标识(0为不在押镖，1为正在押镖中，2为押镖完毕，可以领取奖励)
	UINT16           wAlreadyYabiaoTime; //已经押镖时间（单位秒）
	DT_BIAO_INFO     stBiaoInfo; //自己当前的镖信息
	UINT64           qwCurJingjie; //当前境界
	UINT8            byRemainYabiaoCnt; //剩余押镖次数
	UINT64           qwCoinEncourage; //铜钱
	UINT64           qwScienceEncourage; //器魂
	UINT64           qwJingjieEncourage; //境界
	UINT32           dwJiebiaoReduceCoin; //被拦截掉了的金钱数
	UINT32           dwJiebiaoReduceScience; //被拦截掉了的器魂数
	DT_YABIAO_PLAYER_INFO_FACTION_NAME astYaBiaoFactionName[MAX_YABIAO_PLAYER_NUM]; //押镖玩家帮派名称
	UINT8            abyFactionIconID[MAX_YABIAO_PLAYER_NUM]; //门派头像ID
	UINT8            byDisPlayJieBiao; //是否可截镖0不可截镖，1可截镖
}PKT_CLIGS_OPEN_JIEBIAO_ACK;

typedef struct tagPKT_CLIGS_BEGIN_JIEBIAO_REQ{
	tagPKT_CLIGS_BEGIN_JIEBIAO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_JIEBIAO_REQ)); }
	UINT32           dwPlayerID; //对象ID
	UINT8            byCurCount; //当前次数
}PKT_CLIGS_BEGIN_JIEBIAO_REQ;

typedef struct tagPKT_CLIGS_BEGIN_JIEBIAO_ACK3{
	tagPKT_CLIGS_BEGIN_JIEBIAO_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_JIEBIAO_ACK3)); }
	UINT16           wErrCode; //错误码
	UINT8            byYabiaoFlag; //押镖标识(0为不在押镖，1为正在押镖中，2为押镖完毕，可以领取奖励)
	DT_BATTLE_DATA3  stBattleData; //战斗信息
	UINT64           qwCoinEncourage; //奖励
	UINT32           dwScienceEncourage; //拦截器魂奖励
	UINT64           qwCurCoin; //当前铜钱数
	UINT64           qwCurScience; //当前器魂
	UINT32           dwBeJiebiaoPlayerID; //被拦截对象
	BIAO_OPERATE_INFO stBeJiebiaoInfo; //被劫信息
	UINT32           dwJiebiaoPlayerID; //拦截对象
	BIAO_OPERATE_INFO stJiebiaoInfo; //拦截信息
	UINT16           wJiebiaoCD; //冷却时间
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_BEGIN_JIEBIAO_ACK3;

typedef struct tagPKT_CLIGS_CLEAR_JIEBIAO_CD_REQ{
	tagPKT_CLIGS_CLEAR_JIEBIAO_CD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLEAR_JIEBIAO_CD_REQ)); }
	UINT8            byExt; //扩展
}PKT_CLIGS_CLEAR_JIEBIAO_CD_REQ;

typedef struct tagPKT_CLIGS_CLEAR_JIEBIAO_CD_ACK{
	tagPKT_CLIGS_CLEAR_JIEBIAO_CD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLEAR_JIEBIAO_CD_ACK)); }
	UINT16           wErrCode; //返回码
	UINT64           qwCurGold; //元宝
	UINT16           wJiebiaoCD; //CD
}PKT_CLIGS_CLEAR_JIEBIAO_CD_ACK;

typedef struct tagPKT_CLIGS_BE_JIEBIAO_NTF{
	tagPKT_CLIGS_BE_JIEBIAO_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_BE_JIEBIAO_NTF)); }
	UINT8            byResult; //战斗胜利标志(0胜利,1失败)
	UINT64           qwIndex; //战斗信息索引
	UINT64           qwCoinDec; //损失铜钱
	UINT32           dwScienceDec; //损失器魂
	TCHAR            aszEnemyDispName[USERNAME_LEN]; //对方名称
	UINT16           wEnemyLevel; //对方等级
	UINT32           dwEnemyPower; //对方战力
	UINT8            byUnreadBattleLogNum; //未观看战报个数
}PKT_CLIGS_BE_JIEBIAO_NTF;

typedef struct tagPKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ{
	tagPKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ)); }
	UINT8            byExt; //扩展
}PKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ;

typedef struct tagDT_YABIAO_BATTLELOG_DATA_CLI{
	DT_YABIAO_BATTLELOG_DATA stBattleLogData; //战报具体信息
	TCHAR            aszEnemyDispName[USERNAME_LEN]; //昵称
}DT_YABIAO_BATTLELOG_DATA_CLI;

typedef struct tagPKT_CLIGS_QUERY_YABIAO_BATTLELOG_ACK{
	tagPKT_CLIGS_QUERY_YABIAO_BATTLELOG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_YABIAO_BATTLELOG_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byLogNum; //战报数量
	DT_YABIAO_BATTLELOG_DATA_CLI astYabiaoBattleLog[MAX_YABIAO_BATTLELOG_NUM]; //战报信息
}PKT_CLIGS_QUERY_YABIAO_BATTLELOG_ACK;

typedef struct tagPKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ{
	tagPKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展
}PKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_QUERY_YABIAO_ENCOURAGE_ACK{
	tagPKT_CLIGS_QUERY_YABIAO_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_YABIAO_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码
	UINT64           qwCoinEncourage; //铜钱
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwScienceEncourage; //器魂
	UINT64           qwCurScience; //当前器魂
	UINT64           qwJingjieEncourage; //境界
	UINT64           qwCurJingjie; //当前境界
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_QUERY_YABIAO_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_QUERY_BATTLE_RECORD_REQ{
	tagPKT_CLIGS_QUERY_BATTLE_RECORD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_BATTLE_RECORD_REQ)); }
	UINT32           dwIndex; //索引
}PKT_CLIGS_QUERY_BATTLE_RECORD_REQ;

typedef struct tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK3{
	tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK3)); }
	UINT16           wErrCode; //错误码
	DT_BATTLE_DATA3  stBattleData; //战斗信息
}PKT_CLIGS_QUERY_BATTLE_RECORD_ACK3;

typedef struct tagDT_CLIMB_TOWER_INFO{
	UINT16           wIndex; //场景序号
	UINT8            byEnterPhyStrength; //进入体力要求
	UINT8            byMaxInstanceNum; //最大副本信息
	UINT8            byMaxUnlockInstanceNum; //解锁副本信息
	DT_INSTANCE_RECORD_DATA_CLI stCurInstanceInfo; //当前副本记录信息
	UINT16           wCanResetClimbTownNum; //当前可重置次数
	UINT16           wResetClimbTownNumPerDay; //每天可重置次数
	UINT16           wBuyResetClimbTownGold; //购买元宝
}DT_CLIMB_TOWER_INFO;

typedef struct tagPKT_CLIGS_OPEN_CLIMB_TOWER_REQ{
	tagPKT_CLIGS_OPEN_CLIMB_TOWER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CLIMB_TOWER_REQ)); }
	UINT16           wIndex; //场景序号
}PKT_CLIGS_OPEN_CLIMB_TOWER_REQ;

typedef struct tagPKT_CLIGS_OPEN_CLIMB_TOWER_ACK{
	tagPKT_CLIGS_OPEN_CLIMB_TOWER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CLIMB_TOWER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_CLIMB_TOWER_INFO stClimbTowerInfo; //爬塔信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT64           qwBlueGas; //当前蓝气
}PKT_CLIGS_OPEN_CLIMB_TOWER_ACK;

typedef struct tagPKT_CLIGS_CLIMB_TOWER_RESET_REQ{
	tagPKT_CLIGS_CLIMB_TOWER_RESET_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLIMB_TOWER_RESET_REQ)); }
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
}PKT_CLIGS_CLIMB_TOWER_RESET_REQ;

typedef struct tagPKT_CLIGS_CLIMB_TOWER_RESET_ACK{
	tagPKT_CLIGS_CLIMB_TOWER_RESET_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLIMB_TOWER_RESET_ACK)); }
	UINT16           wErrCode; //错误码
	UINT64           qwGold; //当前玩家元宝数
	DT_CLIMB_TOWER_INFO stClimbTowerInfo; //爬塔信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_CLIMB_TOWER_RESET_ACK;

typedef struct tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ{
	tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ)); }
	UINT16           wSceneIdx; //场景序号
	UINT8            byTownIdx; //城镇序号
}PKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ;

typedef struct tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_ACK{
	tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLIMB_TOWER_ONHOOK_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byOnhookResultNum; //扫荡塔副本数
	DT_ONHOOK_RESULT astOnhookResult[MAX_INSTANCE_CLIMB_TOWER_NUM]; //扫荡塔结果
	DT_DATA_AFTER_ONHOOK stAfterOnhookData; //扫荡玩家后信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	UINT8            byUnlockNewInstanceFlag; //是否解锁新副本
	DT_INSTANCE_ID   astUnlockCommonInstanceID[1]; //解锁的普通副本ID
	UINT8            byHaveNewInstanceFlag; //是否有新可打普通副本,1有效
	DT_INSTANCE_DATA_CLI astNewCommonInstanceID[1]; //新可打普通副本ID
	DT_CLIMB_TOWER_INFO stClimbTowerInfo; //爬塔信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_CLIMB_TOWER_ONHOOK_ACK;

typedef struct tagPKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ{
	tagPKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ)); }
	UINT8            byBuyNum; //购买次数
}PKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ;

typedef struct tagPKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK{
	tagPKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK)); }
	UINT16           wErrCode; //错误码
	UINT64           qwGold; //玩家当前元宝数
	UINT16           wCanResetClimbTownNum; //每天可重置次数
	UINT16           wCanBuyResetClimbTownNum; //每天可购买次数
	UINT16           wBuyResetClimbTownGold; //购买元宝
}PKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK;

typedef struct tagDT_JINGJIE_PER_LEVEL_DATA{
	DT_ATTR_INC_DATA astSubLevelIncAttrInfo[MAX_JINGJIE_SUBLEVEL_NUM]; //本级各子级所增加属性
	UINT32           adwSubLevelCostJingJieInfo[MAX_JINGJIE_SUBLEVEL_NUM]; //本级各子级升级所消耗的境界点
}DT_JINGJIE_PER_LEVEL_DATA;

typedef struct tagDT_JINGJIE_DATA_CLI{
	DT_JINGJIE_BASE_DATA stBaseData; //境界基本信息
	DT_JINGJIE_PER_LEVEL_DATA stJingJiePerLevelData; //境界本级信息
	UINT8            byIncAttrNum; //已提升的属性个数
	DT_ATTR_INC_DATA astIncAttrInfo[MAX_JINGJIE_ATTR_INC_NUM]; //已提升的属性信息
	UINT16           wCurCoachTalent; //当前主角资质
}DT_JINGJIE_DATA_CLI;

typedef struct tagPKT_CLIGS_UPGRADE_JINGJIE_REQ{
	tagPKT_CLIGS_UPGRADE_JINGJIE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_JINGJIE_REQ)); }
	UINT16           wLevel; //等级
}PKT_CLIGS_UPGRADE_JINGJIE_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_JINGJIE_ACK{
	tagPKT_CLIGS_UPGRADE_JINGJIE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_JINGJIE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_JINGJIE_DATA_CLI stJingJieInfo; //境界信息
	UINT64           qwCurJingJie; //当前境界点
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT8            byColor; //主角颜色
}PKT_CLIGS_UPGRADE_JINGJIE_ACK;

typedef struct tagDT_GEN_GAS_DATA_CLI{
	UINT8            byCollectState; //收集状态,0生成，1收取
	DT_GAS_BALL_DATA astGasBollInfo[MAX_GAS_BALL_NUM]; //精气球信息
	UINT32           dwGenGasCoin; //练气所需铜币
	UINT32           dwGenGasGold; //练气所需元宝
	UINT32           dwCommonUpGasGold; //普通升级精气球所需元宝
	UINT32           dwFullLevelUpGasGold; //满级升级精气球所需元宝
	UINT16           wTotalGenNum; //可炼气总次数
	UINT16           wCanGenNum; //可炼气次数
	UINT16           wTotalFreeCommonUpGasNum; //免费普通升级精气球总次数
	UINT16           wCanFreeCommonUpGasNum; //可免费普通升级精气球次数
}DT_GEN_GAS_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_GEN_GAS_TAB_REQ{
	tagPKT_CLIGS_OPEN_GEN_GAS_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEN_GAS_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_GEN_GAS_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK{
	tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GEN_GAS_DATA_CLI stGenGasInfo; //练气面板信息
	TCHAR            aszTipsInfo[MAX_GEN_GAS_INFO_LEN]; //描述信息
}PKT_CLIGS_OPEN_GEN_GAS_TAB_ACK;

typedef struct tagPKT_CLIGS_GEN_GAS_BALL_REQ{
	tagPKT_CLIGS_GEN_GAS_BALL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GEN_GAS_BALL_REQ)); }
	UINT8            byFlag; //生产标志
}PKT_CLIGS_GEN_GAS_BALL_REQ;

typedef struct tagPKT_CLIGS_GEN_GAS_BALL_ACK{
	tagPKT_CLIGS_GEN_GAS_BALL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GEN_GAS_BALL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GEN_GAS_DATA_CLI stGenGasInfo; //练气面板信息
	UINT64           qwCurCoin; //当前玩家铜钱
	UINT64           qwCurGold; //当前玩家元宝
}PKT_CLIGS_GEN_GAS_BALL_ACK;

typedef struct tagPKT_CLIGS_UP_GAS_BALL_LEVEL_REQ{
	tagPKT_CLIGS_UP_GAS_BALL_LEVEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UP_GAS_BALL_LEVEL_REQ)); }
	UINT8            byFullLevelFlag; //提升到满级标志
}PKT_CLIGS_UP_GAS_BALL_LEVEL_REQ;

typedef struct tagPKT_CLIGS_UP_GAS_BALL_LEVEL_ACK{
	tagPKT_CLIGS_UP_GAS_BALL_LEVEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UP_GAS_BALL_LEVEL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GEN_GAS_DATA_CLI stGenGasInfo; //练气面板信息
	UINT64           qwCurCoin; //当前玩家铜钱
	UINT64           qwCurGold; //当前玩家元宝
}PKT_CLIGS_UP_GAS_BALL_LEVEL_ACK;

typedef struct tagPKT_CLIGS_COLLECT_GAS_REQ{
	tagPKT_CLIGS_COLLECT_GAS_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COLLECT_GAS_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_COLLECT_GAS_REQ;

typedef struct tagPKT_CLIGS_COLLECT_GAS_ACK{
	tagPKT_CLIGS_COLLECT_GAS_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COLLECT_GAS_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GEN_GAS_DATA_CLI stGenGasInfo; //练气面板信息
	UINT64           qwScience; //收集到的精气
}PKT_CLIGS_COLLECT_GAS_ACK;

typedef struct tagPKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ{
	tagPKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ)); }
	UINT16           wActionID; //招式ID
	UINT16           wActionLevelCli; //等级
}PKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ;

typedef struct tagDT_ATTACK_RANG_DATA_LST{
	UINT8            byNum; //总个数
	DT_ATTACK_RANG_DATA_CLI astInfo[MAX_SKILL_ACTION_LEVEL_NUM]; //信息
}DT_ATTACK_RANG_DATA_LST;

typedef struct tagDT_EFFECT_DATA_LST2{
	UINT8            byNum; //总个数
	DT_SKILL_EFFECT_DATA_CLI2 astInfo[MAX_SKILL_ACTION_LEVEL_NUM]; //信息
}DT_EFFECT_DATA_LST2;

typedef struct tagDT_SKILL_TAB_DATA_CLI{
	DT_SKILL_DATA_CLI stSkillInfo; //当前技能信息
	DT_ATTACK_RANG_DATA_LST stAttackRangInfo; //技能范围信息列表
	DT_EFFECT_DATA_LST2 stEffectInfo; //技能效果信息列表
	UINT16           wAddExpPerUpgrade; //每次升级增加的经验值
	UINT16           wCostGasPerUpgrade; //每次升级消耗的灵气
	UINT16           wCurPlayerLevel; //当前玩家等级
	UINT64           qwCurCoin; //当前玩家铜钱
	UINT64           qwCurGold; //当前玩家元宝
	UINT64           qwCurPurpleGas; //当前紫气
}DT_SKILL_TAB_DATA_CLI;

typedef struct tagPKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK{
	tagPKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SKILL_TAB_DATA_CLI stSkillTabInfo; //技能面板信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_POWER_CHANGE_DATA stPowerInfo; //玩家战力信息
}PKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK;

typedef struct tagPKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ{
	tagPKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ)); }
	UINT16           wActionID; //招式ID
	UINT16           wEffectID; //效果ID
	UINT8            byAttackRangKind; //攻击范围类型
	UINT8            byTraceBuffKind; //追踪的buff类型(只对AttackRangKind为EARK_BUFF有效)
}PKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ;

typedef struct tagPKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK{
	tagPKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SKILL_TAB_DATA_CLI stSkillTabInfo; //技能面板信息
	DT_POWER_CHANGE_DATA stPowerInfo; //玩家战力信息
}PKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK;

typedef struct tagPKT_CLIGS_OPEN_JINGJIE_TAB_REQ{
	tagPKT_CLIGS_OPEN_JINGJIE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JINGJIE_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_JINGJIE_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_JINGJIE_TAB_ACK{
	tagPKT_CLIGS_OPEN_JINGJIE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JINGJIE_TAB_ACK)); }
	UINT16           wErrCode; //错误码
	DT_JINGJIE_DATA_CLI stJingJieInfo; //境界信息
	UINT16           wMaxQualityLevel; //最高境界阶等级
	UINT64           qwCurJingJie; //当前境界
}PKT_CLIGS_OPEN_JINGJIE_TAB_ACK;

typedef struct tagPKT_CLIGS_OPEN_SKILL_TAB_REQ{
	tagPKT_CLIGS_OPEN_SKILL_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SKILL_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_SKILL_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_SKILL_TAB_ACK{
	tagPKT_CLIGS_OPEN_SKILL_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SKILL_TAB_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SKILL_TAB_DATA_CLI stSkillTabInfo; //技能面板信息
}PKT_CLIGS_OPEN_SKILL_TAB_ACK;

typedef struct tagPKT_CLIGS_BLUE_TO_PURPLE_REQ{
	tagPKT_CLIGS_BLUE_TO_PURPLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BLUE_TO_PURPLE_REQ)); }
	UINT32           dwBlueGas; //要转换的蓝气
}PKT_CLIGS_BLUE_TO_PURPLE_REQ;

typedef struct tagPKT_CLIGS_BLUE_TO_PURPLE_ACK{
	tagPKT_CLIGS_BLUE_TO_PURPLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BLUE_TO_PURPLE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurBlueGas; //当前蓝气
	UINT64           qwCurPurpleGas; //当前紫气
}PKT_CLIGS_BLUE_TO_PURPLE_ACK;

typedef struct tagPKT_CLIGS_OPEN_VIP_TAB_REQ{
	tagPKT_CLIGS_OPEN_VIP_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_VIP_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_VIP_TAB_REQ;

typedef struct tagDT_BUY_GOLD_INFO{
	UINT32           dwGoldNum; //元宝
	UINT32           dwRMB; //人民币
	UINT32           dwProductID; //产品ID
}DT_BUY_GOLD_INFO;

typedef struct tagDT_BUY_GOLD_INFO_EX{
	UINT32           dwGoldNum; //元宝
	UINT32           dwRMB; //人民币
	TCHAR            aszProductID[MAX_PRODUCTID_LEN]; //产品ID
}DT_BUY_GOLD_INFO_EX;

typedef struct tagDT_BUY_GOLD_TXT{
	TCHAR            aszBuyGoldInfo[MAX_COMMON_TXT_NUM]; //VIP购买信息txt
}DT_BUY_GOLD_TXT;

typedef struct tagPKT_CLIGS_OPEN_VIP_TAB_ACK{
	tagPKT_CLIGS_OPEN_VIP_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_VIP_TAB_ACK)); }
	UINT16           wErrCode; //错误码
	DT_VIP_TAB_DATA_CLI stVipTabInfo; //VIP面板信息
	UINT8            byBuyGoldInfoNum; //列表数
	DT_BUY_GOLD_INFO astBuyGoldLst[MAX_BUY_GOLD_NUM]; //购买元宝列表
	DT_BUY_GOLD_TXT  astBuyGoldTxtLst[MAX_BUY_GOLD_NUM]; //VIP购买信息txt列表
	TCHAR            aszProContextHead[MAX_COMMON_TXT_NUM]; //VIP提示信息标题
	TCHAR            aszVipProContext[MAX_VIP_PRO_CONTEXT_LEN]; //VIP提示信息
	TCHAR            aszBuyUrl[MAX_BUY_URL_LEN]; //购买地址
	DT_BUY_GOLD_INFO_EX astBuyGoldLstEx[MAX_BUY_GOLD_NUM]; //购买元宝列表
}PKT_CLIGS_OPEN_VIP_TAB_ACK;

typedef struct tagPKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ{
	tagPKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_XIANGQIAN_TAB_ACK{
	tagPKT_CLIGS_OPEN_XIANGQIAN_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_XIANGQIAN_TAB_ACK)); }
	UINT16           wErrCode; //错误码
	DT_ITEM_DATA_LIST_CLI2 stBagItemInfo; //背包宝石道具信息
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
}PKT_CLIGS_OPEN_XIANGQIAN_TAB_ACK;

typedef struct tagPKT_CLIGS_XIANGQIAN_REQ{
	tagPKT_CLIGS_XIANGQIAN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_XIANGQIAN_REQ)); }
	UINT16           wHeroID; //武将ID
	UINT16           wEquipID; //装备ID
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT16           wItemID; //用来合成的物品ID
	UINT8            byPos; //位置
}PKT_CLIGS_XIANGQIAN_REQ;

typedef struct tagDT_XIANGQIAN_DATA_CLI{
	DT_EQUIP_DATA_CLI stEquip; //装备信息
	DT_ITEM_DATA_LIST_CLI2 stBagItemInfo; //背包宝石道具信息
}DT_XIANGQIAN_DATA_CLI;

typedef struct tagPKT_CLIGS_XIANGQIAN_ACK{
	tagPKT_CLIGS_XIANGQIAN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_XIANGQIAN_ACK)); }
	UINT16           wErrCode; //错误码
	DT_XIANGQIAN_DATA_CLI stXiangqianData; //镶嵌相关装备道具信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_POWER_CHANGE_DATA stPowerInfo; //玩家战力信息
}PKT_CLIGS_XIANGQIAN_ACK;

typedef struct tagPKT_CLIGS_REMOVE_XIANGQIAN_REQ{
	tagPKT_CLIGS_REMOVE_XIANGQIAN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_REMOVE_XIANGQIAN_REQ)); }
	UINT16           wHeroID; //武将ID
	UINT16           wEquipID; //装备ID
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT8            byRemoveAll; //一键移除全部
	UINT8            byPos; //宝石位置
}PKT_CLIGS_REMOVE_XIANGQIAN_REQ;

typedef struct tagPKT_CLIGS_REMOVE_XIANGQIAN_ACK{
	tagPKT_CLIGS_REMOVE_XIANGQIAN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_REMOVE_XIANGQIAN_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wHeroID; //武将ID
	DT_XIANGQIAN_DATA_CLI stXiangqianData; //镶嵌相关装备道具信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
}PKT_CLIGS_REMOVE_XIANGQIAN_ACK;

typedef struct tagPKT_CLIGS_RESET_ELITE_INSTANCE_REQ{
	tagPKT_CLIGS_RESET_ELITE_INSTANCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RESET_ELITE_INSTANCE_REQ)); }
	UINT8            byIndex; //城镇序号
	UINT16           wRemainResetNum; //剩余重置次数(服务器端下发给客户端的数值返回)
}PKT_CLIGS_RESET_ELITE_INSTANCE_REQ;

typedef struct tagDT_ELITE_INSTANCE_VIP_EXT_DATA_CLI{
	UINT64           qwCurGold; //当前元宝
	UINT16           wNextVipLevelNum; //下个VIP级别可重置总次数（如果为0则VIP满级了）
}DT_ELITE_INSTANCE_VIP_EXT_DATA_CLI;

typedef struct tagPKT_CLIGS_RESET_ELITE_INSTANCE_ACK{
	tagPKT_CLIGS_RESET_ELITE_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RESET_ELITE_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byIndex; //城镇序号
	UINT16           wRemainResetNum; //剩余重置次数
	UINT16           wMaxResetNum; //总重置次数
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_ELITE_INSTANCE_VIP_EXT_DATA_CLI stEliteInstanceVipExt; //精英副本VIP扩展信息
	UINT64           qwCostGold; //下一个级别花费元宝
}PKT_CLIGS_RESET_ELITE_INSTANCE_ACK;

typedef struct tagPKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ{
	tagPKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ)); }
	UINT8            byIndex; //城镇序号
}PKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ;

typedef struct tagDT_ELITE_INSTANCE_RECORD_DATA{
	UINT8            byIndex; //副本序号
	DT_BURST_ITEM_DATA stBurstItemInfo; //爆出物品信息
	UINT16           wMainMonsterKindID; //显示主怪物类型ID，0不需显示
	UINT8            byCanBattle; //是否可以打，1可以打
}DT_ELITE_INSTANCE_RECORD_DATA;

typedef struct tagDT_ELITE_INSTANCE_RECORD_DATA_CLI{
	UINT8            byIndex; //城镇序号
	UINT8            byInstanceNum; //副本数
	DT_ELITE_INSTANCE_RECORD_DATA astInstanceInfo[MAX_INSTANCE_PER_TOWN_NUM]; //副本记录信息
	UINT16           wRemainResetNum; //剩余重置次数
	UINT64           qwCostGold; //重置花费元宝
	UINT16           wOnhookLevel; //挂机等级要求
}DT_ELITE_INSTANCE_RECORD_DATA_CLI;

typedef struct tagDT_ELITE_INSTANCE_RECORD_SCORE_DATA_CLI{
	UINT8            byInstanceNum; //副本数
	UINT8            abyScore[MAX_INSTANCE_PER_TOWN_NUM]; //副本记录信息
	DT_CHAPTER_SCORE stChapterInfo; //章数评分信息
}DT_ELITE_INSTANCE_RECORD_SCORE_DATA_CLI;

typedef struct tagPKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK{
	tagPKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byOnhookResultNum; //扫荡塔副本数
	DT_ONHOOK_RESULT astOnhookResult[MAX_INSTANCE_CLIMB_TOWER_NUM]; //扫荡塔结果
	DT_DATA_AFTER_ONHOOK stAfterOnhookData; //扫荡玩家后信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT16           wAllOnhookNum; //总的可挂机数
	DT_ELITE_INSTANCE_RECORD_DATA_CLI stInstanceData; //副本信息
	DT_ELITE_INSTANCE_RECORD_SCORE_DATA_CLI stInstanceScoreData; //副本评分信息
}PKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK;

typedef struct tagPKT_CLIGS_GET_ELITE_INSTANCE_REQ{
	tagPKT_CLIGS_GET_ELITE_INSTANCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ELITE_INSTANCE_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_GET_ELITE_INSTANCE_REQ;

typedef struct tagPKT_CLIGS_GET_ELITE_INSTANCE_ACK{
	tagPKT_CLIGS_GET_ELITE_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ELITE_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_INSTANCE_ID   stMaxInstanceData; //最大副本信息
	UINT8            byTownNum; //城镇数
	DT_ELITE_INSTANCE_RECORD_DATA_CLI astTownInstanceInfo[MAX_TOWN_PER_SCENE_NUM]; //城镇副本信息
	UINT16           wNextUnlockInstanceTownID; //下一个副本未开放属于哪个分组
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_ELITE_INSTANCE_VIP_EXT_DATA_CLI stEliteInstanceVipExt; //精英副本VIP扩展信息
	UINT16           wMaxResetNum; //总重置次数
	DT_ELITE_INSTANCE_RECORD_SCORE_DATA_CLI astTownInstanceScoreInfo[MAX_TOWN_PER_SCENE_NUM]; //城镇副本评分信息
	DT_ACCUMULATED_SCORE_ENCOURAGE stAccumulatedEncourage; //积累奖励信息
	UINT16           wRemainResetNum; //剩余重置次数
}PKT_CLIGS_GET_ELITE_INSTANCE_ACK;

typedef struct tagDT_EQUIP_ID{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //装备序号
}DT_EQUIP_ID;

typedef struct tagPKT_CLIGS_QUICK_EQUIP_REQ{
	tagPKT_CLIGS_QUICK_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_QUICK_EQUIP_REQ)); }
	UINT16           wHeroKindID; //要穿戴的武将
	UINT8            byEquipNum; //装备个数
	DT_EQUIP_ID      astEquipIDLst[EQUIP_POS_NUM]; //要穿戴的装备.0表示没有
}PKT_CLIGS_QUICK_EQUIP_REQ;

typedef struct tagPKT_CLIGS_QUICK_EQUIP_ACK{
	tagPKT_CLIGS_QUICK_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_QUICK_EQUIP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //要穿戴的武将
	UINT8            byEquipNum; //装备个数
	DT_EQUIP_ID      astEquipIDLst[EQUIP_POS_NUM]; //要穿戴的装备.0表示没有
	UINT32           dwPlayerHP; //玩家总血量
	DT_POWER_CHANGE_DATA stHeroPowerInfo; //武将战力信息
	DT_POWER_CHANGE_DATA stPowerInfo; //玩家战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_HERO_PANEL_LST_CLI stHeroPanelInfo; //武将的信息
}PKT_CLIGS_QUICK_EQUIP_ACK;

typedef struct tagPKT_CLIGS_PKBATTLE_REQ{
	tagPKT_CLIGS_PKBATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PKBATTLE_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PKBATTLE_REQ;

typedef struct tagPKT_CLIGS_PKBATTLE_ACK3{
	tagPKT_CLIGS_PKBATTLE_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_PKBATTLE_ACK3)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_DATA3  stBattleInfo; //战斗信息
}PKT_CLIGS_PKBATTLE_ACK3;

typedef struct tagDT_RESOURCES_PROMPT{
	UINT8            byKindID; //类型
	UINT8            byFlag; //功能
}DT_RESOURCES_PROMPT;

typedef struct tagDT_RESOURCES_PROMPT_DATA{
	UINT8            byRsNum; //资源个数
	DT_RESOURCES_PROMPT astRsPromptInfo[MAX_RESOURCES_NUM]; //资源提示信息
}DT_RESOURCES_PROMPT_DATA;

typedef struct tagPKT_CLIGS_RESOURCES_PROMPT_REQ{
	tagPKT_CLIGS_RESOURCES_PROMPT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RESOURCES_PROMPT_REQ)); }
	UINT32           dwRsID; //资源
}PKT_CLIGS_RESOURCES_PROMPT_REQ;

typedef struct tagPKT_CLIGS_RESOURCES_PROMPT_ACK{
	tagPKT_CLIGS_RESOURCES_PROMPT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RESOURCES_PROMPT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RESOURCES_PROMPT_DATA stRsPromptData; //资源信息
}PKT_CLIGS_RESOURCES_PROMPT_ACK;

typedef struct tagDT_BOSSB_PLAYER_DATA_CLI{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT16           wHeroKindID; //主武将类型
	UINT16           wLevel; //等级
	UINT16           wPosX; //位置信息X
	UINT16           wPosY; //位置信息Y
}DT_BOSSB_PLAYER_DATA_CLI;

typedef struct tagDT_BOSSB_PLAYER_DATA_CLI_LST{
	UINT8            byNum; //个数
	DT_BOSSB_PLAYER_DATA_CLI astInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //信息
}DT_BOSSB_PLAYER_DATA_CLI_LST;

typedef struct tagDT_BOSSB_DATA_CLI{
	UINT16           wBossID; //BOSS怪ID
	UINT16           wBossLevel; //BOSS等级
	UINT32           dwTotalBossHP; //BOSS总血量
	UINT32           dwCurBossHP; //BOSS当前血量
	UINT32           dwHPPerGrid; //HP每格血量
	UINT32           dwPlayerNum; //当前人数
	UINT16           wReadyCountDown; //活动等待倒计时
	UINT16           wStopCountDown; //活动结束倒计时
	UINT16           wReliveGold; //复活元宝
}DT_BOSSB_DATA_CLI;

typedef struct tagDT_BOSSB_DATA_CLI2{
	UINT16           wBossID; //BOSS怪ID
	UINT16           wBossLevel; //BOSS等级
	UINT64           qwTotalBossHP; //BOSS总血量
	UINT64           qwCurBossHP; //BOSS当前血量
	UINT64           qwHPPerGrid; //HP每格血量
	UINT32           dwPlayerNum; //当前人数
	UINT16           wReadyCountDown; //活动等待倒计时
	UINT16           wStopCountDown; //活动结束倒计时
	UINT16           wReliveGold; //复活元宝
}DT_BOSSB_DATA_CLI2;

typedef struct tagDT_BOSSB_MY_DATA_CLI{
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurJingJie; //当前境界
	UINT64           qwCurStory; //当前阅历
	UINT16           wPosY; //位置信息Y
	UINT16           wPosX; //位置信息X
	UINT16           wIncPowerRate; //提升战力百分比
	UINT16           wReliveCountDown; //复活倒计时
	UINT16           wGoldReliveCountDown; //元宝复活操作倒计时
	UINT32           dwHurtValue; //对Boss的伤害
	UINT32           dwHurtRate; //对Boss的伤害比例（万分比）
}DT_BOSSB_MY_DATA_CLI;

typedef struct tagDT_HURT_ENC_DATA{
	UINT32           dwEncCoin; //铜钱
	UINT32           dwEncJingJie; //境界
}DT_HURT_ENC_DATA;

typedef struct tagPKT_ENTER_BOSSB_REQ{
	tagPKT_ENTER_BOSSB_REQ() { memset(this, 0, sizeof(tagPKT_ENTER_BOSSB_REQ)); }
	UINT16           wPosY; //位置信息Y
	UINT16           wPosX; //位置信息X
}PKT_ENTER_BOSSB_REQ;

typedef struct tagPKT_ENTER_BOSSB_ACK{
	tagPKT_ENTER_BOSSB_ACK() { memset(this, 0, sizeof(tagPKT_ENTER_BOSSB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCliVer; //客户端版本
	UINT32           dwResVer; //资源版本
	UINT32           dwUIVer; //UI版本
	DT_BOSSB_MY_DATA_CLI stMyBossBInfo; //自己信息
	DT_BOSSB_PLAYER_DATA_CLI_LST stPlayerInfo; //显示玩家信息列表
	DT_BOSSB_DATA_CLI stBossBInfo; //BOSS战信息
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	DT_HURT_ENC_DATA stHurtEncInfo; //已获得的伤害奖励
	TCHAR            aszResVer[MAX_RES_VER_LEN]; //版本信息
}PKT_ENTER_BOSSB_ACK;

typedef struct tagPKT_ENTER_BOSSB_ACK2{
	tagPKT_ENTER_BOSSB_ACK2() { memset(this, 0, sizeof(tagPKT_ENTER_BOSSB_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCliVer; //客户端版本
	UINT32           dwResVer; //资源版本
	UINT32           dwUIVer; //UI版本
	DT_BOSSB_MY_DATA_CLI stMyBossBInfo; //自己信息
	DT_BOSSB_PLAYER_DATA_CLI_LST stPlayerInfo; //显示玩家信息列表
	DT_BOSSB_DATA_CLI2 stBossBInfo; //BOSS战信息
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	DT_HURT_ENC_DATA stHurtEncInfo; //已获得的伤害奖励
	TCHAR            aszResVer[MAX_RES_VER_LEN]; //版本信息
}PKT_ENTER_BOSSB_ACK2;

typedef struct tagPKT_LEAVE_BOSSB_REQ{
	tagPKT_LEAVE_BOSSB_REQ() { memset(this, 0, sizeof(tagPKT_LEAVE_BOSSB_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_LEAVE_BOSSB_REQ;

typedef struct tagPKT_LEAVE_BOSSB_ACK{
	tagPKT_LEAVE_BOSSB_ACK() { memset(this, 0, sizeof(tagPKT_LEAVE_BOSSB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_LEAVE_BOSSB_ACK;

typedef struct tagPKT_BOSSB_MOVE_REQ{
	tagPKT_BOSSB_MOVE_REQ() { memset(this, 0, sizeof(tagPKT_BOSSB_MOVE_REQ)); }
	UINT16           wPosX; //位置信息X
	UINT16           wPosY; //位置信息Y
}PKT_BOSSB_MOVE_REQ;

typedef struct tagPKT_BOSSB_MOVE_ACK{
	tagPKT_BOSSB_MOVE_ACK() { memset(this, 0, sizeof(tagPKT_BOSSB_MOVE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wPosX; //位置信息X
	UINT16           wPosY; //位置信息Y
	UINT16           wOldPosX; //原位置信息X
	UINT16           wOldPosY; //原位置信息Y
}PKT_BOSSB_MOVE_ACK;

typedef struct tagPKT_BOSSB_HURT_REQ{
	tagPKT_BOSSB_HURT_REQ() { memset(this, 0, sizeof(tagPKT_BOSSB_HURT_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_BOSSB_HURT_REQ;

typedef struct tagPKT_BOSSB_HURT_ACK{
	tagPKT_BOSSB_HURT_ACK() { memset(this, 0, sizeof(tagPKT_BOSSB_HURT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCurHurtValue; //本次伤害值
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自己伤害统计
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	DT_HURT_ENC_DATA stHurtEncInfo; //已获得的伤害奖励
	UINT16           wReliveCountDown; //复活倒计时
	UINT16           wGoldReliveCountDown; //元宝复活操作倒计时
	UINT32           dwTotalBossHP; //当前Boss总血量
	UINT32           dwCurBossHP; //当前Boss血量
}PKT_BOSSB_HURT_ACK;

typedef struct tagPKT_BOSSB_HURT_ACK2{
	tagPKT_BOSSB_HURT_ACK2() { memset(this, 0, sizeof(tagPKT_BOSSB_HURT_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwCurHurtValue; //本次伤害值
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自己伤害统计
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	DT_HURT_ENC_DATA stHurtEncInfo; //已获得的伤害奖励
	UINT16           wReliveCountDown; //复活倒计时
	UINT16           wGoldReliveCountDown; //元宝复活操作倒计时
	UINT64           qwTotalBossHP; //当前Boss总血量
	UINT64           qwCurBossHP; //当前Boss血量
}PKT_BOSSB_HURT_ACK2;

typedef struct tagPKT_BOSSB_POS_NTF{
	tagPKT_BOSSB_POS_NTF() { memset(this, 0, sizeof(tagPKT_BOSSB_POS_NTF)); }
	UINT8            bySyncType; //同步类型
	DT_BOSSB_PLAYER_DATA_CLI stPlayerInfo; //信息
	UINT32           dwPlayerNum; //当前人数
}PKT_BOSSB_POS_NTF;

typedef struct tagDT_BOSSB_POS{
	UINT32           dwPlayerID; //玩家ID
	UINT16           wPosX; //位置信息X
	UINT16           wPosY; //位置信息Y
}DT_BOSSB_POS;

typedef struct tagDT_BOSSB_HURT{
	UINT32           dwPlayerID; //玩家ID
	UINT32           dwHurtValue; //伤害值
}DT_BOSSB_HURT;

typedef struct tagPKT_BOSSB_INFO_SYNC_NTF{
	tagPKT_BOSSB_INFO_SYNC_NTF() { memset(this, 0, sizeof(tagPKT_BOSSB_INFO_SYNC_NTF)); }
	UINT32           dwPlayerNum; //当前人数
	UINT32           dwTotalBossHP; //当前Boss总血量
	UINT32           dwCurBossHP; //当前Boss血量
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	UINT8            byLeaveNum; //离开人数
	UINT32           adwLeaveInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //离开信息
	UINT8            byEnterNum; //进入人数
	DT_BOSSB_PLAYER_DATA_CLI astEnterInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //进入信息
	UINT8            byMoveNum; //移动人数
	DT_BOSSB_POS     astMoveInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //移动信息
	UINT8            byHurtNum; //伤害人数
	DT_BOSSB_HURT    astHurtInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //伤害信息
}PKT_BOSSB_INFO_SYNC_NTF;

typedef struct tagPKT_BOSSB_INFO_SYNC_NTF2{
	tagPKT_BOSSB_INFO_SYNC_NTF2() { memset(this, 0, sizeof(tagPKT_BOSSB_INFO_SYNC_NTF2)); }
	UINT32           dwPlayerNum; //当前人数
	UINT64           qwTotalBossHP; //当前Boss总血量
	UINT64           qwCurBossHP; //当前Boss血量
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	UINT8            byLeaveNum; //离开人数
	UINT32           adwLeaveInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //离开信息
	UINT8            byEnterNum; //进入人数
	DT_BOSSB_PLAYER_DATA_CLI astEnterInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //进入信息
	UINT8            byMoveNum; //移动人数
	DT_BOSSB_POS     astMoveInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //移动信息
	UINT8            byHurtNum; //伤害人数
	DT_BOSSB_HURT    astHurtInfo[MAX_BOSSB_SHOW_PLAYER_NUM]; //伤害信息
}PKT_BOSSB_INFO_SYNC_NTF2;

typedef struct tagDT_BOSSB_OVER_INFO{
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自身攻击伤害
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	UINT32           dwTotalBossHP; //当前Boss总血量
	UINT32           dwCurBossHP; //当前Boss血量
	DT_BOSS_BATTLE_ENC_DATA stHurtEncInfo; //伤害奖励信息
	DT_BOSS_BATTLE_ENC_DATA stHeroEncInfo; //英雄奖励信息
	DT_BOSS_BATTLE_ENC_DATA astTopEncInfo[MAX_BOSS_ENC_TOP_NUM]; //top奖励信息
	DT_BOSS_BATTLE_ENC_DATA stKillEncInfo; //最后1击奖励信息
	UINT8            byBossDeadFlag; //BOSS是否死亡,1死亡
	UINT8            byNeedUpgrade; //BOSS是否升级,1升级
}DT_BOSSB_OVER_INFO;

typedef struct tagDT_BOSSB_OVER_INFO2{
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自身攻击伤害
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	UINT64           qwTotalBossHP; //当前Boss总血量
	UINT64           qwCurBossHP; //当前Boss血量
	DT_BOSS_BATTLE_ENC_DATA stHurtEncInfo; //伤害奖励信息
	DT_BOSS_BATTLE_ENC_DATA stHeroEncInfo; //英雄奖励信息
	DT_BOSS_BATTLE_ENC_DATA astTopEncInfo[MAX_BOSS_ENC_TOP_NUM]; //top奖励信息
	DT_BOSS_BATTLE_ENC_DATA stKillEncInfo; //最后1击奖励信息
	UINT8            byBossDeadFlag; //BOSS是否死亡,1死亡
	UINT8            byNeedUpgrade; //BOSS是否升级,1升级
}DT_BOSSB_OVER_INFO2;

typedef struct tagPKT_BOSSB_OVER_NTF{
	tagPKT_BOSSB_OVER_NTF() { memset(this, 0, sizeof(tagPKT_BOSSB_OVER_NTF)); }
	DT_BOSSB_OVER_INFO stBossOverInfo; //boss战活动结束信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //当前阅历
	UINT64           qwCurJingJie; //当前境界
	UINT8            byUnreadBattleLogNum; //未读战报个数
}PKT_BOSSB_OVER_NTF;

typedef struct tagPKT_BOSSB_OVER_NTF2{
	tagPKT_BOSSB_OVER_NTF2() { memset(this, 0, sizeof(tagPKT_BOSSB_OVER_NTF2)); }
	DT_BOSSB_OVER_INFO2 stBossOverInfo; //boss战活动结束信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //当前阅历
	UINT64           qwCurJingJie; //当前境界
	UINT8            byUnreadBattleLogNum; //未读战报个数
}PKT_BOSSB_OVER_NTF2;

typedef struct tagPKT_OPEN_INSPIRE_TAB_REQ{
	tagPKT_OPEN_INSPIRE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_OPEN_INSPIRE_TAB_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_OPEN_INSPIRE_TAB_REQ;

typedef struct tagDT_INSPIRE_TAB_DATA{
	UINT16           wHaveIncPowerRate; //已提升战力百分比
	UINT16           wIncCostStory; //提升战力消耗阅历
	UINT16           wIncCostGold; //提升战力消耗元宝
	UINT16           wStoryIncRate; //阅历提升战力百分比
	UINT16           wGoldIncRate; //元宝提升战力百分比
	UINT16           wCanIncPowerRate; //可提升战力百分比
}DT_INSPIRE_TAB_DATA;

typedef struct tagPKT_OPEN_INSPIRE_TAB_ACK{
	tagPKT_OPEN_INSPIRE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_OPEN_INSPIRE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_INSPIRE_TAB_DATA stInspireTabInfo; //鼓舞面板信息
}PKT_OPEN_INSPIRE_TAB_ACK;

typedef struct tagPKT_INSPIRE_POWER_REQ{
	tagPKT_INSPIRE_POWER_REQ() { memset(this, 0, sizeof(tagPKT_INSPIRE_POWER_REQ)); }
	UINT8            byGoldInspireFlag; //是否元宝鼓舞方式，1为使用元宝鼓舞
}PKT_INSPIRE_POWER_REQ;

typedef struct tagPKT_INSPIRE_POWER_ACK{
	tagPKT_INSPIRE_POWER_ACK() { memset(this, 0, sizeof(tagPKT_INSPIRE_POWER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_INSPIRE_TAB_DATA stInspireTabInfo; //鼓舞面板信息
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //当前阅历
}PKT_INSPIRE_POWER_ACK;

typedef struct tagPKT_BOSSB_RELIVE_REQ{
	tagPKT_BOSSB_RELIVE_REQ() { memset(this, 0, sizeof(tagPKT_BOSSB_RELIVE_REQ)); }
	UINT16           wExt; //扩展字段
}PKT_BOSSB_RELIVE_REQ;

typedef struct tagPKT_BOSSB_RELIVE_ACK{
	tagPKT_BOSSB_RELIVE_ACK() { memset(this, 0, sizeof(tagPKT_BOSSB_RELIVE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurGold; //当前元宝
}PKT_BOSSB_RELIVE_ACK;

typedef struct tagPKT_BOSSB_GET_LAST_BOSSB_INFO_REQ{
	tagPKT_BOSSB_GET_LAST_BOSSB_INFO_REQ() { memset(this, 0, sizeof(tagPKT_BOSSB_GET_LAST_BOSSB_INFO_REQ)); }
	UINT16           wExt; //扩展字段
}PKT_BOSSB_GET_LAST_BOSSB_INFO_REQ;

typedef struct tagPKT_BOSSB_GET_LAST_BOSSB_INFO_ACK{
	tagPKT_BOSSB_GET_LAST_BOSSB_INFO_ACK() { memset(this, 0, sizeof(tagPKT_BOSSB_GET_LAST_BOSSB_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BOSSB_OVER_INFO stBossOverInfo; //boss战活动结束信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //当前阅历
	UINT64           qwCurJingJie; //当前境界
}PKT_BOSSB_GET_LAST_BOSSB_INFO_ACK;

typedef struct tagPKT_BOSSB_GET_LAST_BOSSB_INFO_ACK2{
	tagPKT_BOSSB_GET_LAST_BOSSB_INFO_ACK2() { memset(this, 0, sizeof(tagPKT_BOSSB_GET_LAST_BOSSB_INFO_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BOSSB_OVER_INFO2 stBossOverInfo; //boss战活动结束信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //当前阅历
	UINT64           qwCurJingJie; //当前境界
}PKT_BOSSB_GET_LAST_BOSSB_INFO_ACK2;

typedef struct tagPKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ{
	tagPKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ)); }
	UINT16           wExt; //扩展字段
}PKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ;

typedef struct tagPKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK{
	tagPKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFlag; //标志位，0：可冲值提示， 1：可领取冲值奖励
	DT_HERO_BASE_DATA_CLI stHeroBaseInfo; //武将基本信息
	DT_SKILL_DATA_CLI stSkillInfo; //武将技能信息
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI2 astItemList[MAX_RECHARGE_ITEM_NUM]; //道具信息
}PKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展字段，未使用
}PKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK;

typedef struct tagPKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ{
	tagPKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ;

typedef struct tagPKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK{
	tagPKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byAppAddNum; //地址长度
	UINT8            abyAppAddress[MAX_APP_ADDRESS_NUM]; //地址信息
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI2 astItemList[MAX_SCORE_ITEM_NUM]; //道具信息
}PKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展字段，未使用
}PKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_GET_RANDOM_DSPNAME_REQ{
	tagPKT_CLIGS_GET_RANDOM_DSPNAME_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_RANDOM_DSPNAME_REQ)); }
	UINT8            byExt; //扩展字段，未使用
}PKT_CLIGS_GET_RANDOM_DSPNAME_REQ;

typedef struct tagPKT_CLIGS_GET_RANDOM_DSPNAME_ACK{
	tagPKT_CLIGS_GET_RANDOM_DSPNAME_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_RANDOM_DSPNAME_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	TCHAR            aszDspName[USERNAME_LEN]; //昵称
}PKT_CLIGS_GET_RANDOM_DSPNAME_ACK;

typedef struct tagDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI{
	UINT64           qwOverTime; //活动结束时间
	UINT16           wBossID; //活动BOSSID
	DT_BOSSB_HURT_RATE stMyHurtInfo; //自身攻击伤害
	DT_BOSS_BATTLE_ENC_DATA stHurtEncInfo; //伤害奖励信息
	DT_BOSS_BATTLE_ENC_DATA stHeroEncInfo; //英雄奖励信息
	DT_BOSS_BATTLE_ENC_DATA astBossBTopEncInfo[MAX_BOSS_ENC_TOP_NUM]; //top奖励信息
	DT_BOSS_BATTLE_ENC_DATA stBossBKillEncInfo; //最后1击奖励信息
}DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ{
	tagPKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ)); }
	UINT8            byExt; //扩展字段，未使用
}PKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ;

typedef struct tagPKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK{
	tagPKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byNum; //战报个数
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI astBossBattleBL[MAX_BOSSB_BATTLE_LOG_NUM]; //战报信息
}PKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK;

typedef struct tagDT_HERO_UPGRADE_LEVEL{
	UINT8            byFreeLevel; //免费可获取等级
	UINT8            byGoldLevel; //使用元宝可获取等级
}DT_HERO_UPGRADE_LEVEL;

typedef struct tagPKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ{
	tagPKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ)); }
	UINT16           wHeroID; //武将ID
}PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ;

typedef struct tagPKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK{
	tagPKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFreePercentage; //免费可获取经验百分比
	UINT8            byGoldPercentage; //使用元宝可获取经验百分比
	UINT16           wGold; //元宝数
	DT_HERO_BASE_DATA_CLI stHeroInfo; //当前武将信息
	UINT16           wHeroNum; //武将数目
	DT_HERO_BASE_DATA_CLI astHeroInfoLst[MAX_HERO_NUM]; //武将信息
	DT_HERO_UPGRADE_LEVEL astUpgradeHeroInfoLst[MAX_HERO_NUM]; //武将升级信息
}PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK;

typedef struct tagPKT_CLIGS_EXPERIENCE_TRANSFER_REQ{
	tagPKT_CLIGS_EXPERIENCE_TRANSFER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EXPERIENCE_TRANSFER_REQ)); }
	UINT16           wHeroID; //要初递的武将ID
	UINT16           wTransHeroID; //被初递的武将ID
	UINT8            byGold; //使用元宝标志：1使用元宝
}PKT_CLIGS_EXPERIENCE_TRANSFER_REQ;

typedef struct tagPKT_CLIGS_EXPERIENCE_TRANSFER_ACK{
	tagPKT_CLIGS_EXPERIENCE_TRANSFER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EXPERIENCE_TRANSFER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_POWER_CHANGE_DATA stPowerChangeData; //武将战力信息
	DT_HERO_BASE_DATA_CLI stHeroInfo; //当前武将信息
	DT_HERO_BASE_DATA_CLI stTransHeroInfo; //被吸收武将信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT64           qwCurGold; //当前元宝
}PKT_CLIGS_EXPERIENCE_TRANSFER_ACK;

typedef struct tagDT_RDCHALLENGE_POINT_DATA_CLI{
	UINT16           wCurChallengePoint; //当前挑战点
	UINT16           wMaxChallengePoint; //最大挑战点
	UINT16           wRefreshCD; //挑战点刷新间隔,单位s
	UINT16           wCountDown; //挑战点刷新倒计时
}DT_RDCHALLENGE_POINT_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ{
	tagPKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ)); }
	UINT8            byRefreshFlag; //是否刷新，1为刷新
}PKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ;

typedef struct tagDT_RDC_ENC_DATA{
	UINT32           dwJingJie; //境界点
	UINT32           dwPurpleGas; //紫气
}DT_RDC_ENC_DATA;

typedef struct tagDT_RDC_PLAYER_DATA_CLI{
	DT_RDC_PLAYER_DATA stPlayerInfo; //玩家信息
	DT_RDC_ENC_DATA  stSuccessEnc; //赢奖励信息
	DT_RDC_ENC_DATA  stFailEnc; //输奖励信息
}DT_RDC_PLAYER_DATA_CLI;

typedef struct tagDT_RDC_TAB_DATA{
	UINT8            byPlayerNum; //当前返回玩家个数
	DT_RDC_PLAYER_DATA_CLI astPlayerInfo[MAX_GET_RDC_NUM]; //玩家信息
	DT_RDCHALLENGE_POINT_DATA_CLI stChallengePointInfo; //挑战点信息
	UINT16           wChallengeCountDown; //挑战倒计时
	UINT32           dwRefreshCoin; //刷新所需铜钱
	UINT32           dwClrCDGold; //清除CD元宝
}DT_RDC_TAB_DATA;

typedef struct tagPKT_CLIGS_OPEN_RDCHALLENGE_TAB_ACK{
	tagPKT_CLIGS_OPEN_RDCHALLENGE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RDCHALLENGE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RDC_TAB_DATA  stTabInfo; //随机挑战面板信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurJingJie; //当前境界点
	UINT64           qwCurPurpleGas; //当前紫气
	UINT8            byHaveWorship; //是否已经膜拜，1为已经膜拜
}PKT_CLIGS_OPEN_RDCHALLENGE_TAB_ACK;

typedef struct tagPKT_CLIGS_RDCHALLENGE_REQ{
	tagPKT_CLIGS_RDCHALLENGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RDCHALLENGE_REQ)); }
	UINT32           dwPlayerID; //要挑战的玩家ID
}PKT_CLIGS_RDCHALLENGE_REQ;

typedef struct tagPKT_CLIGS_RDCHALLENGE_ACK3{
	tagPKT_CLIGS_RDCHALLENGE_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_RDCHALLENGE_ACK3)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_DATA3  stBattleInfo; //战斗信息
	DT_RDC_TAB_DATA  stTabInfo; //随机挑战面板信息
	DT_RDC_ENC_DATA  stEncInfo; //随机挑战奖励信息
	UINT64           qwCurJingJie; //当前境界点
	UINT64           qwCurPurpleGas; //当前紫气
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT16           wKindID; //获得的鲜花ID
}PKT_CLIGS_RDCHALLENGE_ACK3;

typedef struct tagPKT_CLIGS_CLR_RDCHALLENGE_CD_REQ{
	tagPKT_CLIGS_CLR_RDCHALLENGE_CD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_RDCHALLENGE_CD_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_CLR_RDCHALLENGE_CD_REQ;

typedef struct tagPKT_CLIGS_CLR_RDCHALLENGE_CD_ACK{
	tagPKT_CLIGS_CLR_RDCHALLENGE_CD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_RDCHALLENGE_CD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurGold; //玩家当前的元宝
}PKT_CLIGS_CLR_RDCHALLENGE_CD_ACK;

typedef struct tagDT_WORSHIP_DATA_CLI{
	UINT8            byWorshipTypeLevel; //膜拜级别，1为最高等级
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT32           dwPlayerID; //玩家ID
	UINT32           dwCoachID; //主将ID
	UINT32           dwNeedCoin; //膜拜所需铜钱，0为不需要
	UINT32           dwNeedGold; //膜拜所需元宝，0为不需要
	UINT8            byNeedVipLevel; //膜拜所需VIP等级，0为不需要
	UINT32           dwEncJingJie; //膜拜可获得境界
	UINT32           dwEncPurpleGas; //膜拜可获得紫气
	UINT8            byHaveWorship; //是否已经膜拜，1为已经膜拜
}DT_WORSHIP_DATA_CLI;

typedef struct tagDT_WORSHIP_DATA_CLI_LST{
	UINT16           wNum; //个数
	DT_WORSHIP_DATA_CLI astWorshipInfo[MAX_WORSHIP_NUM]; //信息
}DT_WORSHIP_DATA_CLI_LST;

typedef struct tagPKT_CLIGS_OPEN_WORSHIP_TAB_REQ{
	tagPKT_CLIGS_OPEN_WORSHIP_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_WORSHIP_TAB_REQ)); }
	UINT8            byWorshipType; //膜拜类型
}PKT_CLIGS_OPEN_WORSHIP_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_WORSHIP_TAB_ACK{
	tagPKT_CLIGS_OPEN_WORSHIP_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_WORSHIP_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_WORSHIP_DATA_CLI_LST stWorshipInfo; //膜拜信息列表
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurGold; //玩家当前的元宝
	UINT64           qwCurJingJie; //玩家当前的境界
	UINT64           qwCurPurpleGas; //玩家当前的紫气
}PKT_CLIGS_OPEN_WORSHIP_TAB_ACK;

typedef struct tagPKT_CLIGS_WORSHIP_REQ{
	tagPKT_CLIGS_WORSHIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_WORSHIP_REQ)); }
	UINT8            byWorshipType; //膜拜类型
	UINT8            byWorshipTypeLevel; //膜拜级别
}PKT_CLIGS_WORSHIP_REQ;

typedef struct tagPKT_CLIGS_WORSHIP_ACK{
	tagPKT_CLIGS_WORSHIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_WORSHIP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_WORSHIP_DATA_CLI_LST stWorshipInfo; //膜拜信息列表
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurGold; //玩家当前的元宝
	UINT64           qwCurJingJie; //玩家当前的境界
	UINT64           qwCurPurpleGas; //玩家当前的紫气
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_WORSHIP_ACK;

typedef struct tagPKT_CLIGS_ACTIVITY_NOTIFY_REQ{
	tagPKT_CLIGS_ACTIVITY_NOTIFY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ACTIVITY_NOTIFY_REQ)); }
	UINT8            byActivityType; //类型
	UINT8            byNotify; //通知
}PKT_CLIGS_ACTIVITY_NOTIFY_REQ;

typedef struct tagPKT_CLIGS_ACTIVITY_NOTIFY_ACK{
	tagPKT_CLIGS_ACTIVITY_NOTIFY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ACTIVITY_NOTIFY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_ACTIVITY_NOTIFY_ACK;

typedef struct tagDT_DOGFECES_BUILD_TIP{
	UINT16           wBuildID; //建筑ID
	UINT32           dwParameter1; //参数
	UINT32           dwParameter2; //参数
	UINT32           dwParameter3; //参数
	UINT32           dwParameter4; //参数
	UINT32           dwParameter5; //参数
}DT_DOGFECES_BUILD_TIP;

typedef struct tagPKT_CLIGS_OPEN_DOGFECES_REQ{
	tagPKT_CLIGS_OPEN_DOGFECES_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DOGFECES_REQ)); }
	UINT8            byType; //请求类型
}PKT_CLIGS_OPEN_DOGFECES_REQ;

typedef struct tagPKT_CLIGS_OPEN_DOGFECES_ACK{
	tagPKT_CLIGS_OPEN_DOGFECES_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DOGFECES_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byNum; //个数
	DT_DOGFECES_BUILD_TIP astDogFecesBuildTip[MAX_DOGFECES_BUILD_TIP_NUM]; //次数信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_OPEN_DOGFECES_ACK;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_CENTER_REQ{
	tagPKT_CLIGS_GET_ACTIVITY_CENTER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_CENTER_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_GET_ACTIVITY_CENTER_REQ;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_CENTER_ACK{
	tagPKT_CLIGS_GET_ACTIVITY_CENTER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_CENTER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byActivityNum; //活动数
	DT_ACTIVITY_BASE_DATA astActivityInfo[MAX_ACTIVITY_NUM]; //活动
	UINT8            abyActivityStartCountDown[MAX_ACTIVITY_NUM]; //活动开启天数倒计时
}PKT_CLIGS_GET_ACTIVITY_CENTER_ACK;

typedef struct tagDT_ACTIVITY_ENCOURAGE_DATA_CLI{
	UINT16           wActivityID; //活动类型
	UINT8            byCanReceive; //是否可领取
	UINT8            bySeriesNo; //是否有系列号标志
	TCHAR            aszActivityName[MAX_ACTIVITY_ENCOURAGE_NAME_LEN]; //活动名称
	TCHAR            aszActivityDesc[MAX_ACTIVITY_ENCOURAGE_DESC_LEN]; //活动描述
	TCHAR            aszObtainingConditions[MAX_ACTIVITY_ENCOURAGE_DESC_LEN]; //领取条件
	DT_ENCOURAGE_DATA stEncourageData; //奖励信息
}DT_ACTIVITY_ENCOURAGE_DATA_CLI;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ{
	tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK{
	tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byActivityEncourageNum; //活动数
	DT_ACTIVITY_ENCOURAGE_DATA_CLI astActivityEncourageInfo[MAX_ACTIVITY_ENCOURAGE_NUM]; //活动奖励信息
}PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK;

typedef struct tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ{
	tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ)); }
	UINT16           wActivityID; //活动类型
	TCHAR            aszSeriesNo[MAX_SERIES_NO_NUM]; //系列码
}PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK{
	tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wActivityIDOld; //活动类型
	DT_RSYNC_RECV_ENCOURAGE_DATA stRsyncRecvEncourageData; //同步领取奖励后玩家数据变化
	UINT8            byHide; //隐藏
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息
}PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;

typedef struct tagDT_ACTIVITY_ENCOURAGE_DATA_CLI2{
	UINT64           qwActivityID; //活动类型
	UINT8            byCanReceive; //是否可领取
	UINT8            bySeriesNo; //是否有系列号标志
	TCHAR            aszActivityName[MAX_ACTIVITY_ENCOURAGE_NAME_LEN]; //活动名称
	TCHAR            aszActivityDesc[MAX_ACTIVITY_ENCOURAGE_DESC_LEN]; //活动描述
	TCHAR            aszObtainingConditions[MAX_ACTIVITY_ENCOURAGE_DESC_LEN]; //领取条件
	DT_ENCOURAGE_DATA stEncourageData; //奖励信息
}DT_ACTIVITY_ENCOURAGE_DATA_CLI2;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2{
	tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2;

typedef struct tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2{
	tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byActivityEncourageNum; //活动数
	DT_ACTIVITY_ENCOURAGE_DATA_CLI2 astActivityEncourageInfo[MAX_ACTIVITY_ENCOURAGE_NUM]; //活动奖励信息
}PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2;

typedef struct tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2{
	tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2)); }
	UINT64           qwActivityID; //活动类型
	TCHAR            aszSeriesNo[MAX_SERIES_NO_NUM]; //系列码
}PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2;

typedef struct tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2{
	tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwActivityID; //活动类型
	DT_RSYNC_RECV_ENCOURAGE_DATA stRsyncRecvEncourageData; //同步领取奖励后玩家数据变化
	UINT8            byHide; //隐藏
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息
}PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2;

typedef struct tagDT_GEM_MAKER_DATA{
	UINT16           wMakerKindID; //制作卷物品ID
	UINT16           wPileNum; //个数
	UINT16           wGemKindID; //制作成的宝石物品ID
	UINT16           wGemLevel; //制作宝石等级
	UINT32           dwNeedBlueGas; //制作所需蓝气
	UINT8            byOpenNeedTowerNum; //开放所需爬塔层数
	TCHAR            aszSuccessRate[MAX_COMMON_TXT_NUM]; //成功率
	DT_ITEM_DATA_LIST_CLI2 stGemInfo; //可合成宝石信息
}DT_GEM_MAKER_DATA;

typedef struct tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ{
	tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK{
	tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurBlueGas; //当前蓝气
	UINT8            byCurTowerNum; //当前已爬塔层数
	UINT8            byMakerNum; //制作卷个数(若未全部开放，则多下发一个未开放的)
	DT_GEM_MAKER_DATA astGemMakerInfo[MAX_GEM_LEVEL]; //宝石制作卷信息
	TCHAR            aszOtherPrompt[MAX_COMMON_TXT_NUM]; //提示
}PKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK;

typedef struct tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ{
	tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ;

typedef struct tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK{
	tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurBlueGas; //当前蓝气
	UINT8            byCurTowerNum; //当前已爬塔层数
	UINT8            byMakerNum; //制作卷个数(若未全部开放，则多下发一个未开放的)
	DT_GEM_MAKER_DATA astGemMakerInfo[MAX_GEM_LEVEL_EX]; //宝石制作卷信息
	TCHAR            aszOtherPrompt[MAX_COMMON_TXT_NUM]; //提示
}PKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK;

typedef struct tagPKT_CLIGS_RECV_RATING_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_RATING_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_RATING_ENCOURAGE_REQ)); }
	UINT16           wSceneIdx; //场景类型
	UINT8            byType; //奖励类型0：完美通关，1：积累奖励
	UINT16           wChapterID; //章节ID，只有完美通关才有值，积累奖励直接为0即可
}PKT_CLIGS_RECV_RATING_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_RATING_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_RATING_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_RATING_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byType; //奖励类型0：完美通关，1：积累奖励
	UINT16           wChapterID; //章节ID，只有完美通关才有值，积累奖励直接为0即可
	DT_RSYNC_RECV_ENCOURAGE_DATA stRsyncRecvEncourageData; //同步领取奖励后玩家数据变化
	DT_ACCUMULATED_SCORE_ENCOURAGE stAccumulatedEncourage; //积累奖励信息
}PKT_CLIGS_RECV_RATING_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ{
	tagPKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ;

typedef struct tagDT_STUDY_DATA_CLI2{
	UINT8            byStudyColorKind; //心得颜色，从1开始，0表示没有数据
	UINT8            byStudyAttrKind1; //心得类型(对应属性类型,0表示只能转换经验)
	UINT8            byStudyLevel; //心得等级(从1开始，不会有0)
	UINT32           dwStudyAttrValue1; //心得属性值
	UINT32           dwStudyExp; //心得经验值
	UINT32           dwNextLevelAttrValue1; //下一级心得属性值
	UINT32           dwUpgradeNeedExp; //升级到下一级所需心得经验
	UINT8            byStudyAttrKind2; //心得类型(对应属性类型)
	UINT32           dwStudyAttrValue2; //心得属性值
	UINT32           dwNextLevelAttrValue2; //下一级心得属性值
}DT_STUDY_DATA_CLI2;

typedef struct tagDT_AWAKEN_TAB_DATA2{
	DT_AWAKEN_BASE_DATA stBaseInfo; //悟道基本信息
	DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT16           wMaxStudyLevel; //心得最大等级
	UINT32           adwAwakenCoin[MAX_AWAKEN_NUM]; //悟道所需铜币
	UINT32           dwAwakenGold; //悟道所需元宝
	UINT16           wTatalCallNum; //总共可召唤次数
	UINT16           wCanCallNum; //剩余可召唤次数
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	DT_STUDY_DATA_CLI2 stCallStudy; //召唤可得心得
}DT_AWAKEN_TAB_DATA2;

typedef struct tagDT_AWAKEN_STUDY_EXCHANGE_DATA{
	DT_STUDY_DATA_CLI2 stStudyInfo; //心得信息
	UINT32           dwNeedCoin; //兑换所需铜钱
	UINT32           dwNeedClip; //兑换所需碎片
	UINT16           wNeePlayerLevel; //开放等级
}DT_AWAKEN_STUDY_EXCHANGE_DATA;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK{
	tagPKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byExchangeNum; //兑换心得数
	DT_AWAKEN_STUDY_EXCHANGE_DATA astStudyExchangeInfo[MAX_EXCHANGE_STUDY_NUM]; //心得兑换信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurStudyClip; //当前心得碎片
}PKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK;

typedef struct tagPKT_CLIGS_EXCHANGE_STUDY_REQ{
	tagPKT_CLIGS_EXCHANGE_STUDY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EXCHANGE_STUDY_REQ)); }
	UINT8            byStudyColorKind; //心得颜色
	UINT8            byStudyAttrKind1; //心得类型
	UINT8            byStudyAttrKind2; //心得类型
	UINT8            byStudyLevel; //心得等级
}PKT_CLIGS_EXCHANGE_STUDY_REQ;

typedef struct tagPKT_CLIGS_EXCHANGE_STUDY_ACK{
	tagPKT_CLIGS_EXCHANGE_STUDY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EXCHANGE_STUDY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byStudyBagIdx; //所得心得序号(对应心得背包序号，从0开始)
	DT_AWAKEN_TAB_DATA2 stAwakenTabInfo; //悟道面板信息
}PKT_CLIGS_EXCHANGE_STUDY_ACK;

typedef struct tagPKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ{
	tagPKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ;

typedef struct tagDT_COMPOSE_EQUIP_DATA_CLI{
	UINT16           wKindID; //装备类型
	UINT8            byDressPos; //穿戴位置
	UINT16           wStrengthenLevel; //装备强化等级
	UINT16           wDressLevel; //装备（穿戴)等级
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //当前战斗属性
}DT_COMPOSE_EQUIP_DATA_CLI;

typedef struct tagDT_EQUIP_COMPOSE_LEVEL_DATA_CLI{
	UINT16           wEquipLevel; //装备等级
	UINT8            byEquipNum; //装备个数
	DT_COMPOSE_EQUIP_DATA_CLI astEquipInfo[MAX_BAG_OPEN_NUM]; //装备信息
	UINT16           wEquipClipNum; //当前等级装备碎片数
	UINT16           wComposeNeedNum; //合成该等级装备所需碎片数
	UINT8            byCurLowColor; //当前最低颜色
	UINT16           wHitKindID; //当前命中装备ID
	UINT32           dwColorComposeNeedGold; //当前颜色合成所需元宝
	UINT32           dwCommonComposeNeedGold; //普通合成所需元宝
}DT_EQUIP_COMPOSE_LEVEL_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK{
	tagPKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byEquipLevelNum; //装备等级个数
	DT_EQUIP_COMPOSE_LEVEL_DATA_CLI astEquipLevelInfo[MAX_EQUIPLEVEL_NUM]; //装备等级信息
	UINT16           wComposeCountDown; //合成倒计时
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurBlueGas; //当前蓝气
}PKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK;

typedef struct tagPKT_CLIGS_EQUIP_COMPOSE_REQ{
	tagPKT_CLIGS_EQUIP_COMPOSE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_COMPOSE_REQ)); }
	UINT16           wEquipLevel; //装备等级
	UINT8            byCurColorFlag; //当前最低颜色及以上中合成，1为选择
}PKT_CLIGS_EQUIP_COMPOSE_REQ;

typedef struct tagPKT_CLIGS_EQUIP_COMPOSE_ACK{
	tagPKT_CLIGS_EQUIP_COMPOSE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_COMPOSE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_EQUIP_COMPOSE_LEVEL_DATA_CLI stEquipLevelInfo; //装备等级信息
	UINT16           wComposeCountDown; //合成倒计时
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurBlueGas; //当前蓝气
}PKT_CLIGS_EQUIP_COMPOSE_ACK;

typedef struct tagPKT_CLIGS_RECV_COMPOSE_EQUIP_REQ{
	tagPKT_CLIGS_RECV_COMPOSE_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_COMPOSE_EQUIP_REQ)); }
	UINT16           wEquipLevel; //装备等级
}PKT_CLIGS_RECV_COMPOSE_EQUIP_REQ;

typedef struct tagPKT_CLIGS_RECV_COMPOSE_EQUIP_ACK{
	tagPKT_CLIGS_RECV_COMPOSE_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_COMPOSE_EQUIP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_EQUIP_COMPOSE_LEVEL_DATA_CLI stEquipLevelInfo; //装备等级信息
	DT_EQUIP_DATA_CLI stRecvEquipInfo; //领取的装备信息
	DT_EQUIP_DATA_STRENGTHEN_CLI stRecvEquipStrengthenInfo; //领取的装备信息（带强化）
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	UINT32           dwStrengthen10Coin; //装备升10级所需要铜钱信息
}PKT_CLIGS_RECV_COMPOSE_EQUIP_ACK;

typedef struct tagDT_FACTION_DATA_TO_CLI{
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT32           dwFactionRank; //门派排名
	UINT32           dwFactionLevel; //门派等级
	UINT32           dwFactionFunds; //门派资金
	TCHAR            aszFactionAnnouncement[MAX_FACTION_DESC_NUM]; //门派公告
	UINT32           dwFactionID; //门派ID
}DT_FACTION_DATA_TO_CLI;

typedef struct tagDT_FACTION_BASE_DATA_CLI{
	UINT8            byJoinFlag; //加入标志0:未加入 1：申请中 2：成功加入
	TCHAR            aszDispName[USERNAME_LEN]; //帮主名称
	UINT16           wFactionNumberPeople; //门派当前人数
	UINT16           wFactionMaxPeople; //门派上限人数
	DT_FACTION_DATA_TO_CLI stFactionBaseData; //门派信息
}DT_FACTION_BASE_DATA_CLI;

typedef struct tagDT_FACTION_BASE_DATA_CLI_LST{
	UINT16           wFactionNum; //门派个数
	DT_FACTION_BASE_DATA_CLI astFactionBaseData[MAX_FACTION_CLI_NUM]; //门派信息
}DT_FACTION_BASE_DATA_CLI_LST;

typedef struct tagDT_OPEN_FACTION_BASE_DATA{
	UINT32           dwCreateFactionCoin; //门派花费的铜钱数
	UINT32           dwCreateFactionGold; //门派花费的玩宝数
	DT_FACTION_BASE_DATA_CLI_LST stRankFactionBaseDataCliLst; //门派排名
}DT_OPEN_FACTION_BASE_DATA;

typedef struct tagDT_FACTION_PLAYER_CLI{
	UINT32           dwPlayerID; //玩家ID
	UINT16           wHeroKindID; //主武将ID
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT16           wLevel; //玩家等级
	UINT16           wRank; //玩家竞技场排名
	UINT8            byJobType; //职业类型
	UINT32           dwDoorsTributeValues; //剩余贡献
	UINT64           qwDoorsTributeSum; //累积门贡
	UINT32           dwOffLineSeconds; //离线经过的秒数， 0：当前在线， > 0离线
}DT_FACTION_PLAYER_CLI;

typedef struct tagDT_FACTION_PLAYER_CLI_LST{
	UINT16           wFactionNum; //门派个数
	DT_FACTION_PLAYER_CLI astFactionPlayerCli[DT_FACTION_PLAYER_NUM]; //门派信息
}DT_FACTION_PLAYER_CLI_LST;

typedef struct tagDT_FACTION_BASE_OWNER_DATA_CLI{
	DT_FACTION_BASE_DATA stFactionBaseData; //门派信息
	UINT16           wFactionNumberPeople; //门派在线人数
	UINT16           wMaxFactionNumberPeople; //门派最大人数
	UINT32           dwFactionFunds; //门派资金
	DT_FACTION_AUTHORITY stFactionAuthority; //门派权限信息
	TCHAR            aszHeadName[NORMAL_MSG_LEN]; //帮主名称
	UINT32           dwMyDoorsTribute; //我的门贡
	UINT64           qwMySumDoorsTribute; //我的累积门贡
	UINT8            byRecvFlag; //可领奖标志 0 不可领奖， 1可领奖
}DT_FACTION_BASE_OWNER_DATA_CLI;

typedef struct tagDT_FACTION_OWNER_DATA{
	DT_FACTION_BASE_OWNER_DATA_CLI stFactionBaseData; //加入门派基本信息
	DT_FACTION_PLAYER_CLI_LST stFactionPlayerCliLst; //门派玩家信息
	UINT8            byAuditNum; //审核人数
}DT_FACTION_OWNER_DATA;

typedef union tagDT_OPEN_FACTION_CONDITION{
	DT_OPEN_FACTION_BASE_DATA stOpenFactionBaseData; //打开门派基本信息
	DT_FACTION_OWNER_DATA stFactionOwnerData; //打开已加入的门派信息
}DT_OPEN_FACTION_CONDITION;

typedef struct tagPKT_CLIGS_APPLY_FACTION_REQ{
	tagPKT_CLIGS_APPLY_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_APPLY_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
}PKT_CLIGS_APPLY_FACTION_REQ;

typedef struct tagPKT_CLIGS_APPLY_FACTION_ACK{
	tagPKT_CLIGS_APPLY_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_APPLY_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwFactionID; //门派ID
	UINT32           dwJoinFactionCDTime; //加入门派CD
}PKT_CLIGS_APPLY_FACTION_ACK;

typedef struct tagPKT_CLIGS_CANCEL_APPLY_FACTION_REQ{
	tagPKT_CLIGS_CANCEL_APPLY_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CANCEL_APPLY_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
}PKT_CLIGS_CANCEL_APPLY_FACTION_REQ;

typedef struct tagPKT_CLIGS_CANCEL_APPLY_FACTION_ACK{
	tagPKT_CLIGS_CANCEL_APPLY_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CANCEL_APPLY_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwFactionID; //门派ID
}PKT_CLIGS_CANCEL_APPLY_FACTION_ACK;

typedef struct tagPKT_CLIGS_CREATE_FACTION_REQ{
	tagPKT_CLIGS_CREATE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CREATE_FACTION_REQ)); }
	UINT16           wCreateFactionType; //创建门派消耗类型, 0：铜钱 1：元宝
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	TCHAR            aszFactionDescName[MAX_FACTION_DESC_NUM]; //门派描述
}PKT_CLIGS_CREATE_FACTION_REQ;

typedef struct tagPKT_CLIGS_CREATE_FACTION_ACK{
	tagPKT_CLIGS_CREATE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CREATE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_OWNER_DATA stFactionOwnerData; //打开已加入的门派信息
	UINT64           qwLeftTime; //加入门派的CD时间
}PKT_CLIGS_CREATE_FACTION_ACK;

typedef struct tagPKT_CLIGS_SEARCH_FACTION_REQ{
	tagPKT_CLIGS_SEARCH_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEARCH_FACTION_REQ)); }
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
}PKT_CLIGS_SEARCH_FACTION_REQ;

typedef struct tagPKT_CLIGS_SEARCH_FACTION_ACK{
	tagPKT_CLIGS_SEARCH_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SEARCH_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_BASE_DATA_CLI_LST stSearchFactionBaseDataCliLst; //查询
}PKT_CLIGS_SEARCH_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_OWNER_FACTION_REQ{
	tagPKT_CLIGS_OPEN_OWNER_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_OWNER_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_OWNER_FACTION_REQ;

typedef struct tagDT_FACTION_PLAYER_AUDIT_CLI{
	UINT32           dwID; //玩家唯一ID
	UINT16           wHeroKindID; //主武将类型
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT16           wLevel; //玩家等级
	UINT32           dwRank; //排名
	UINT32           dwPower; //战力
	UINT32           dwFirstAttack; //先攻值
}DT_FACTION_PLAYER_AUDIT_CLI;

typedef struct tagDT_FACTION_PLAYER_AUDIT_CLI_LST{
	UINT16           wFactionNum; //门派个数
	DT_FACTION_PLAYER_AUDIT_CLI astFactionPlayerAuditCli[DT_FACTION_PLAYER_NUM]; //门派信息
}DT_FACTION_PLAYER_AUDIT_CLI_LST;

typedef struct tagPKT_CLIGS_OPEN_OWNER_FACTION_ACK{
	tagPKT_CLIGS_OPEN_OWNER_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_OWNER_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_OWNER_DATA stFactionOwnerData; //打开已加入的门派信息
}PKT_CLIGS_OPEN_OWNER_FACTION_ACK;

typedef struct tagDT_ITEM_LIST_INFO{
	UINT8            byItemNum; //同步物品个数
	DT_ITEM_DATA_CLI2 astItemInfo[MAX_ENCOURAGE_ITEM_KIND_NUM]; //物品信息
}DT_ITEM_LIST_INFO;

typedef struct tagPKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ{
	tagPKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
}PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ;

typedef struct tagDT_DOORS_TRIBUTE_BASE_DATA{
	UINT16           wCoinMaxTimeEveryDay; //铜钱每天最多可领取的次数
	UINT32           dwCoinEveryTimeValues; //铜钱每次贡献值
	UINT32           dwCoinEveryTimeDoorsTributeValues; //铜钱每次能获得的门贡
	UINT32           dwCoinEveryTimeFundsValues; //铜钱每次能获得的门派资金
	UINT16           wGoldMaxTimeEveryDay; //元宝每天最多可领取的次数
	UINT32           dwGoldEveryTimeValues; //元宝每次贡献值
	UINT32           dwGoldEveryTimeDoorsTributeValues; //元宝每次能获得的门贡
	UINT32           dwGoldEveryTimeFundsValues; //元宝每次能获得的门派资金
}DT_DOORS_TRIBUTE_BASE_DATA;

typedef struct tagDT_DOORS_TRIBUTE_TODAY_DATA{
	UINT8            byCoinRecvFlag; //领取奖励标志 0不可领取 1:可领取 2已领取过
	UINT16           wCoinTodaysDoorsTributeTimes; //今天铜钱贡献次数
	UINT8            byGoldRecvFlag; //领取奖励标志 1:可领取
	UINT16           wGoldTodaysDoorsTributeTimes; //今天元宝贡献次数
}DT_DOORS_TRIBUTE_TODAY_DATA;

typedef struct tagPKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK{
	tagPKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_DOORS_TRIBUTE_BASE_DATA stDoorsTributeBaseData; //门贡基本信息
	DT_ITEM_LIST_INFO stEncourageData; //奖励信息
	DT_DOORS_TRIBUTE_TODAY_DATA stDoorsTributeTodayData; //给客户端门派贡献当天信息
	UINT32           dwDoorsTribute; //玩家的门贡值
	UINT8            byCoinRequireTime; //捐多少次可领取奖励
}PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK;

typedef struct tagPKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ{
	tagPKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
	UINT32           dwDoorsTributeFlag; //标志0：铜钱，1：元宝
}PKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ;

typedef struct tagPKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK{
	tagPKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_DOORS_TRIBUTE_BASE_DATA stDoorsTributeBaseData; //门贡基本信息
	DT_DOORS_TRIBUTE_TODAY_DATA stDoorsTributeTodayData; //给客户端门派贡献当天信息
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
	UINT32           dwDoorsTribute; //玩家的门贡值
	UINT32           dwFactionFunds; //门派资金
	UINT8            byDoorsTributeFlag; //标志0：铜钱，1：元宝
	UINT64           qwSumDoorsTribute; //玩家累积门贡
	UINT8            byNeedVIPLevel; //需要的VIP等级（当捐满时有效）
}PKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK;

typedef struct tagPKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ)); }
	UINT32           dwFactionID; //门派ID
}PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_ITEM_LIST_INFO stRewardInfo; //奖励信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_KICKED_FACTION_REQ{
	tagPKT_CLIGS_KICKED_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_KICKED_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_KICKED_FACTION_REQ;

typedef struct tagPKT_CLIGS_KICKED_FACTION_ACK{
	tagPKT_CLIGS_KICKED_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_KICKED_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_PLAYER_CLI_LST stFactionPlayer; //玩家列表
	UINT16           wMaxPlayerNum; //门派人数上限
	DT_DOORSTRIBUTE_LEVE_DATA_LST stDoorsTributeLst; //贡献度列表
}PKT_CLIGS_KICKED_FACTION_ACK;

typedef struct tagPKT_CLIGS_JOB_FACTION_REQ{
	tagPKT_CLIGS_JOB_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JOB_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
	UINT32           dwPlayerID; //玩家ID
	UINT8            byFlag; //标志0：降职1：升职
}PKT_CLIGS_JOB_FACTION_REQ;

typedef struct tagPKT_CLIGS_JOB_FACTION_ACK{
	tagPKT_CLIGS_JOB_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JOB_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_PLAYER_CLI_LST stFactionPlayerDataLst; //门派玩家信息
	DT_DOORSTRIBUTE_LEVE_DATA_LST stDoorsTributeLst; //贡献度列表
}PKT_CLIGS_JOB_FACTION_ACK;

typedef struct tagPKT_CLIGS_SET_NOTICE_FACTION_REQ{
	tagPKT_CLIGS_SET_NOTICE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SET_NOTICE_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
	UINT8            byFlag; //标志0：公告1：通知
	TCHAR            aszFactionNotice[MAX_FACTION_DESC_NUM]; //门派通知
}PKT_CLIGS_SET_NOTICE_FACTION_REQ;

typedef struct tagPKT_CLIGS_SET_NOTICE_FACTION_ACK{
	tagPKT_CLIGS_SET_NOTICE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SET_NOTICE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFlag; //标志0：公告1：通知
	TCHAR            aszFactionNotice[MAX_FACTION_DESC_NUM]; //门派通知
}PKT_CLIGS_SET_NOTICE_FACTION_ACK;

typedef struct tagPKT_CLIGS_DISSOLVE_FACTION_REQ{
	tagPKT_CLIGS_DISSOLVE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DISSOLVE_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
}PKT_CLIGS_DISSOLVE_FACTION_REQ;

typedef struct tagPKT_CLIGS_DISSOLVE_FACTION_ACK{
	tagPKT_CLIGS_DISSOLVE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DISSOLVE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_DISSOLVE_FACTION_ACK;

typedef struct tagDT_FACTION_AUDIT_PLAYER_DATA{
	UINT32           dwPlayerID; //门派用户信息
	UINT8            byAuditFlag; //标志0：审核没通过 1：审核通过
}DT_FACTION_AUDIT_PLAYER_DATA;

typedef struct tagPKT_CLIGS_AUDIT_FACTION_REQ{
	tagPKT_CLIGS_AUDIT_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_FACTION_REQ)); }
	UINT32           dwFactionID; //门派ID
	UINT32           dwFactionNum; //用户数
	DT_FACTION_AUDIT_PLAYER_DATA astAuditPlayerData[DT_FACTION_PLAYER_NUM]; //门派用户信息
}PKT_CLIGS_AUDIT_FACTION_REQ;

typedef struct tagPKT_CLIGS_AUDIT_FACTION_ACK{
	tagPKT_CLIGS_AUDIT_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwPassPlayerNum; //审核通过人数
	UINT32           dwNoPassPlayerNum; //忽略人数
	UINT8            byLeftPlayerNum; //剩余人数
	UINT16           wCurPlayerNum; //门派当前人数
	UINT16           wMaxPlayerNum; //门派人数上限
	DT_FACTION_PLAYER_AUDIT_CLI_LST stFactionPlayerAuditCliLst; //门派审核用户信息
	UINT16           wLeftAuditNum; //剩余审核人数 （用于错误显示）
}PKT_CLIGS_AUDIT_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_APPLY_FACTION_REQ{
	tagPKT_CLIGS_OPEN_APPLY_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_APPLY_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_APPLY_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_APPLY_FACTION_ACK{
	tagPKT_CLIGS_OPEN_APPLY_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_APPLY_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_BASE_DATA_CLI_LST stApplyFactionBaseDataCliLst; //门派申请加入
}PKT_CLIGS_OPEN_APPLY_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_RANK_FACTION_REQ{
	tagPKT_CLIGS_OPEN_RANK_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RANK_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_RANK_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_RANK_FACTION_ACK{
	tagPKT_CLIGS_OPEN_RANK_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RANK_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_BASE_DATA_CLI_LST stRankFactionBaseDataCliLst; //门派申请加入
	UINT64           qwNeedGold; //创建门派所需元宝
	UINT64           qwNeedCoin; //创建门派所需铜钱
}PKT_CLIGS_OPEN_RANK_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ{
	tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_AUDIT_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK{
	tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_PLAYER_AUDIT_CLI_LST stFactionPlayerAuditCliLst; //门派申请加入用户信息
}PKT_CLIGS_OPEN_AUDIT_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_LOG_FACTION_REQ{
	tagPKT_CLIGS_OPEN_LOG_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LOG_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_LOG_FACTION_REQ;

typedef struct tagDT_FACTION_LOG_DATA_CLI{
	TCHAR            aszDispName1[USERNAME_LEN]; //玩家名称
	UINT8            byJobType1; //玩家职位ID
	TCHAR            aszDispName2[USERNAME_LEN]; //玩家名称
	UINT8            byJobType2; //玩家职位ID
	UINT8            byAction; //事件ID
	UINT32           adwParamList[FACTION_LOG_LIST_MAX_NUM]; //参数数组
	UINT64           qwLogTime; //时间
}DT_FACTION_LOG_DATA_CLI;

typedef struct tagDT_FACTION_LOG_DATA_CLI_LST{
	UINT16           wFactionLogNum; //门派日志个数
	DT_FACTION_LOG_DATA_CLI astFactionLogDataCli[MAX_FACTION_LOG_NUM]; //门派日志信息
}DT_FACTION_LOG_DATA_CLI_LST;

typedef struct tagPKT_CLIGS_OPEN_LOG_FACTION_ACK{
	tagPKT_CLIGS_OPEN_LOG_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LOG_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_LOG_DATA_CLI_LST stFactionLogDataCliLst; //门派日志信息
}PKT_CLIGS_OPEN_LOG_FACTION_ACK;

typedef struct tagDT_GATHER_SCIENCE_DATA_CLI{
	UINT64           qwCoinScience; //铜钱取获得器魂数
	UINT16           wCoinMaxNum; //铜钱最多可聚魂次数
	UINT16           wCoinNum; //铜钱当前使用聚魂次数
	UINT32           dwCoinValues; //消耗铜钱
	UINT64           qwGoldScience; //元宝取获得器魂数
	UINT16           wGoldMaxNum; //元宝最多可聚魂次数
	UINT16           wGoldNum; //元宝当前使用聚魂次数
	UINT32           dwGoldValues; //消耗元宝
	UINT8            byIncrease; //倍率
	UINT8            abyIncrease[MAX_INCREASE]; //倍率信息
}DT_GATHER_SCIENCE_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_GATHER_SCIENCE_REQ{
	tagPKT_CLIGS_OPEN_GATHER_SCIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GATHER_SCIENCE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_GATHER_SCIENCE_REQ;

typedef struct tagPKT_CLIGS_OPEN_GATHER_SCIENCE_ACK{
	tagPKT_CLIGS_OPEN_GATHER_SCIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GATHER_SCIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwScience; //器魂数
	DT_GATHER_SCIENCE_DATA_CLI stGatherScience; //聚魂基本信息
}PKT_CLIGS_OPEN_GATHER_SCIENCE_ACK;

typedef struct tagPKT_CLIGS_GATHER_SCIENCE_REQ{
	tagPKT_CLIGS_GATHER_SCIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GATHER_SCIENCE_REQ)); }
	UINT8            byGatherType; //聚魂类型 0：铜钱 1：元宝
}PKT_CLIGS_GATHER_SCIENCE_REQ;

typedef struct tagPKT_CLIGS_GATHER_SCIENCE_ACK{
	tagPKT_CLIGS_GATHER_SCIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GATHER_SCIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwScience; //器魂数
	UINT64           qwGold; //元宝
	UINT64           qwCoin; //铜钱
	DT_GATHER_SCIENCE_DATA_CLI stGatherScience; //聚魂基本信息
}PKT_CLIGS_GATHER_SCIENCE_ACK;

typedef struct tagPKT_CLIGS_NOTICE_NTF{
	tagPKT_CLIGS_NOTICE_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_NOTICE_NTF)); }
	DT_NOTICE_DATA   stNoticeInfo; //公告信息
}PKT_CLIGS_NOTICE_NTF;

typedef struct tagPKT_CLIGS_NOTICE_NEW_NTF{
	tagPKT_CLIGS_NOTICE_NEW_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_NOTICE_NEW_NTF)); }
	DT_NOTICE_DATA_NEW stNoticeInfo; //公告信息
}PKT_CLIGS_NOTICE_NEW_NTF;

typedef struct tagPKT_CLIGS_ACTIVITY_ENC_NTF{
	tagPKT_CLIGS_ACTIVITY_ENC_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_ACTIVITY_ENC_NTF)); }
	UINT8            byEncNum; //活动奖励个数
}PKT_CLIGS_ACTIVITY_ENC_NTF;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_TAB_REQ2{
	tagPKT_CLIGS_OPEN_AWAKEN_TAB_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_TAB_REQ2)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_AWAKEN_TAB_REQ2;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_TAB_ACK2{
	tagPKT_CLIGS_OPEN_AWAKEN_TAB_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_TAB_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_AWAKEN_TAB_DATA2 stAwakenInfo; //悟道面板信息
	UINT8            bySingleResolve; //是否可以分解单黄卦象 0不可以 1为可以
	UINT8            byDoubleResolve; //是否可以分解双黄卦象 0不可以 1为可以
}PKT_CLIGS_OPEN_AWAKEN_TAB_ACK2;

typedef struct tagPKT_CLIGS_AWAKEN_REQ2{
	tagPKT_CLIGS_AWAKEN_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_AWAKEN_REQ2)); }
	UINT8            byQuickFlag; //是否使用一键悟道，一键悟道时元宝悟道无效
	UINT8            byGoldFlag; //是否使用元宝悟道
}PKT_CLIGS_AWAKEN_REQ2;

typedef struct tagDT_AWAKEN_GET_DATA2{
	UINT32           dwGetCoin; //所得铜钱
	UINT8            byStudyBagIdx; //所得心得序号(对应心得背包序号，从0开始)，所得铜钱/碎片为0时有效
	UINT8            byAwakenLevel; //悟道等级
	UINT32           dwGetStudyClip; //所得心得碎片
}DT_AWAKEN_GET_DATA2;

typedef struct tagPKT_CLIGS_AWAKEN_ACK2{
	tagPKT_CLIGS_AWAKEN_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_AWAKEN_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byQuickFlag; //是否使用一键悟道，一键悟道时元宝悟道无效
	UINT8            byGoldFlag; //是否使用元宝悟道
	UINT8            byAwakenNum; //总共悟道次数
	DT_AWAKEN_GET_DATA2 astAwakenGetInfo[MAX_AWAKEN_NUM]; //悟道所得信息
	DT_AWAKEN_TAB_DATA2 stAwakenTabInfo; //悟道面板信息
}PKT_CLIGS_AWAKEN_ACK2;

typedef struct tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2{
	tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2;

typedef struct tagDT_HERO_STUDY_DATA_CLI2{
	UINT16           wHeroID; //武将ID
	DT_STUDY_DATA_CLI2 astStudyInfo[MAX_HERO_STUDY_GRID_NUM]; //心得信息
}DT_HERO_STUDY_DATA_CLI2;

typedef struct tagDT_DRESS_STUDY_TAB_DATA2{
	DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT16           awStudyUnlockLevelInfo[MAX_HERO_STUDY_GRID_NUM]; //心得解锁等级信息
	UINT8            byHeroNum; //武将个数
	DT_HERO_STUDY_DATA_CLI2 astHeroStudyInfo[MAX_FORMATION_IDX_NUM]; //武将心得信息
	UINT64           qwStudyExp; //心得经验
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
}DT_DRESS_STUDY_TAB_DATA2;

typedef struct tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK2{
	tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_DRESS_STUDY_TAB_DATA2 stDressStudyTabInfo; //穿戴心得面板信息
}PKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK2;

typedef struct tagPKT_CLIGS_DRESS_STUDY_REQ2{
	tagPKT_CLIGS_DRESS_STUDY_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_STUDY_REQ2)); }
	UINT16           wHeroKindID; //武将ID
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byHeroGridIdx; //武将心得位置，从0开始
}PKT_CLIGS_DRESS_STUDY_REQ2;

typedef struct tagPKT_CLIGS_DRESS_STUDY_ACK2{
	tagPKT_CLIGS_DRESS_STUDY_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_STUDY_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将ID
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byHeroGridIdx; //武将心得位置，从0开始
	DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	DT_HERO_STUDY_DATA_CLI2 stHeroStudyInfo; //武将心得信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	UINT8            byHaveSecondsUndress; //是否卸2个卦象，1为是
	UINT8            byBagGridIdx2; //第二个卸下的卦象存放的背包心得位置，从0开始，HaveSecondsUndress为1有效
	UINT8            byHeroGridIdx2; //武将第二个卸下的卦象心得位置，从0开始，HaveSecondsUndress为1有效
}PKT_CLIGS_DRESS_STUDY_ACK2;

typedef struct tagPKT_CLIGS_STUDY_TO_EXP_REQ2{
	tagPKT_CLIGS_STUDY_TO_EXP_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_TO_EXP_REQ2)); }
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byAllFlag; //转换所有心得背包中的心得,1转换所有
	UINT8            abySelectConvertColor[MAX_COLOR_NUM]; //选择转换的颜色(对应颜色下标-1，0表示不转换，1表示需转换)
}PKT_CLIGS_STUDY_TO_EXP_REQ2;

typedef struct tagPKT_CLIGS_STUDY_TO_EXP_ACK2{
	tagPKT_CLIGS_STUDY_TO_EXP_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_TO_EXP_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            abySelectConvertColor[MAX_COLOR_NUM]; //转换的颜色(对应颜色下标-1，0表示不转换，1表示需转换)
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT8            byAllFlag; //转换所有心得背包中的心得,1转换所有
	UINT32           dwGenStudyExp; //转换所得心得经验
	DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT64           qwCurStudyExp; //当前心得经验
}PKT_CLIGS_STUDY_TO_EXP_ACK2;

typedef struct tagPKT_CLIGS_STUDY_UPGRADE_REQ2{
	tagPKT_CLIGS_STUDY_UPGRADE_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_UPGRADE_REQ2)); }
	UINT16           wHeroKindID; //武将ID
	UINT8            byGridIdx; //心得位置，从0开始
	UINT8            byLevel; //要升级到的等级
}PKT_CLIGS_STUDY_UPGRADE_REQ2;

typedef struct tagPKT_CLIGS_STUDY_UPGRADE_ACK2{
	tagPKT_CLIGS_STUDY_UPGRADE_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_UPGRADE_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将ID
	UINT8            byGridIdx; //心得位置，从0开始
	UINT8            byLevel; //要升级到的等级
	DT_HERO_STUDY_DATA_CLI2 stHeroStudyInfo; //武将心得信息
	UINT64           qwCurStudyExp; //当前心得经验
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
}PKT_CLIGS_STUDY_UPGRADE_ACK2;

typedef struct tagDT_BEGBLUE_PLAYER_RES_CLI{
	UINT64           qwGold; //玩家当前元宝数
	UINT64           qwBlue; //玩家当前仙露数
	UINT64           qwStory; //玩家当前阅历数
}DT_BEGBLUE_PLAYER_RES_CLI;

typedef struct tagDT_BEGBLUE_INFO_CLI{
	UINT32           dwMaxBegTimes; //祈求次数上限
	UINT32           dwLeftBegTimes; //剩余祈求次数
}DT_BEGBLUE_INFO_CLI;

typedef struct tagDT_IMPROVELUCK_INFO_CLI{
	UINT32           dwFreeImproveLuckMaxTimes; //当日免费改运次数上限
	UINT32           dwLeftFreeImproveLuckTimes; //剩余免费改运次数
	UINT32           dwGoldImproveLuckIdx; //下次元宝改运序号(针对每次求仙露，下次求仙露将清零)
	UINT32           dwNextImproveLuckGold; //下次改运需花费的元宝
}DT_IMPROVELUCK_INFO_CLI;

typedef struct tagDT_BEGBLUE_RES_DATA_CLI{
	UINT16           wSmileNum; //笑脸个数
	UINT32           dwBlueUnit; //单倍仙露
	UINT32           dwStoryUnit; //单倍阅历
	UINT16           wXnMultiple; //倍率
	TCHAR            aszLuckPrompt[BEGBLUE_LUCK_PROMPT]; //运势提示
}DT_BEGBLUE_RES_DATA_CLI;

typedef struct tagDT_BEGBLUE_RESULT_CLI{
	DT_BEGBLUE_INFO_CLI stBegBlueInfo; //求仙露基本信息
	DT_IMPROVELUCK_INFO_CLI stImproveLuckInfo; //改运信息
	DT_BEGBLUE_RES_DATA_CLI stBegBlueResCli; //祈求得到的资源
}DT_BEGBLUE_RESULT_CLI;

typedef union tagDT_BLUE_PAVILION_UNION_DATA_CLI{
	DT_BEGBLUE_INFO_CLI stBegBlueInfo; //普通仙露阁
	DT_BEGBLUE_RESULT_CLI stBegBlueResult; //带祈求结果的仙露阁
}DT_BLUE_PAVILION_UNION_DATA_CLI;

typedef struct tagDT_BLUE_PAVILION_DATA_CLI{
	UINT8            byBluePavilionType; //仙露阁类型
	DT_BLUE_PAVILION_UNION_DATA_CLI stBluePavilionUnion; //仙露阁联合
	TCHAR            aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT]; //爬塔提示
}DT_BLUE_PAVILION_DATA_CLI;

typedef struct tagDT_BLUE_ENCOURAGEINFO{
	UINT32           dwBlue; //仙露
	UINT16           wSmileNum; //笑脸个数
}DT_BLUE_ENCOURAGEINFO;

typedef struct tagDT_BLUE_ENCOURAGEINFO_CLI{
	UINT16           wSmileNum; //累计笑脸个数
	UINT8            byGetAward; //是否可以领取奖励 0再求得……，1求得……，2今日……
	DT_BLUE_ENCOURAGEINFO stEncourageData; //奖励数据
}DT_BLUE_ENCOURAGEINFO_CLI;

typedef struct tagPKT_CLIGS_OPEN_BLUE_PAVILION_REQ{
	tagPKT_CLIGS_OPEN_BLUE_PAVILION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BLUE_PAVILION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_BLUE_PAVILION_REQ;

typedef struct tagPKT_CLIGS_OPEN_BLUE_PAVILION_ACK{
	tagPKT_CLIGS_OPEN_BLUE_PAVILION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BLUE_PAVILION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BEGBLUE_PLAYER_RES_CLI stPlayerResInfo; //玩家当前资源信息
	DT_BLUE_PAVILION_DATA_CLI stBluePavilion; //仙露阁数据
	UINT8            byIsIgnoreXnMultiple; //是否忽略求仙露倍率(0不忽略，1忽略)
	DT_BLUE_ENCOURAGEINFO_CLI stEncourage; //奖励信息
}PKT_CLIGS_OPEN_BLUE_PAVILION_ACK;

typedef struct tagPKT_CLIGS_BEGBLUE_REQ{
	tagPKT_CLIGS_BEGBLUE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGBLUE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_BEGBLUE_REQ;

typedef struct tagPKT_CLIGS_BEGBLUE_ACK{
	tagPKT_CLIGS_BEGBLUE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGBLUE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BEGBLUE_RESULT_CLI stBegBlueResult; //求仙露结果信息
}PKT_CLIGS_BEGBLUE_ACK;

typedef struct tagPKT_CLIGS_IMPROVELUCK_REQ{
	tagPKT_CLIGS_IMPROVELUCK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_IMPROVELUCK_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_IMPROVELUCK_REQ;

typedef struct tagPKT_CLIGS_IMPROVELUCK_ACK{
	tagPKT_CLIGS_IMPROVELUCK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_IMPROVELUCK_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwGold; //玩家当前元宝数
	DT_BEGBLUE_RES_DATA_CLI stBegBlueResCli; //改运后得到的资源
	DT_IMPROVELUCK_INFO_CLI stImproveLuckInfo; //改运信息
}PKT_CLIGS_IMPROVELUCK_ACK;

typedef struct tagPKT_CLIGS_COMFIRM_BEGBLUE_REQ{
	tagPKT_CLIGS_COMFIRM_BEGBLUE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COMFIRM_BEGBLUE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_COMFIRM_BEGBLUE_REQ;

typedef struct tagPKT_CLIGS_COMFIRM_BEGBLUE_ACK{
	tagPKT_CLIGS_COMFIRM_BEGBLUE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COMFIRM_BEGBLUE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BEGBLUE_PLAYER_RES_CLI stPlayerResInfo; //玩家当前资源信息
	DT_BEGBLUE_INFO_CLI stBegBlueInfo; //求仙露基本信息
	DT_BLUE_ENCOURAGEINFO_CLI stEncourage; //奖励信息
}PKT_CLIGS_COMFIRM_BEGBLUE_ACK;

typedef struct tagPKT_CLIGS_GET_OPENLIGHT_INFO_REQ{
	tagPKT_CLIGS_GET_OPENLIGHT_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_OPENLIGHT_INFO_REQ)); }
	UINT16           wEquipKindID; //装备ID
	UINT16           wEquipIdx; //装备序号
}PKT_CLIGS_GET_OPENLIGHT_INFO_REQ;

typedef struct tagPKT_CLIGS_GET_OPENLIGHT_INFO_ACK{
	tagPKT_CLIGS_GET_OPENLIGHT_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_OPENLIGHT_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wEquipKindID; //装备ID
	UINT16           wEquipIdx; //装备序号
	DT_EQUIP_DATA_CLI stAfterOpenLightInfo; //装备开光后信息
	UINT16           wOpenLightGoodsID; //当前开光石物品ID
	UINT16           wNeedOpenLightGoodsNum; //开光需要的开光石物品个数
	UINT16           wOpenLightGoodsNum; //当前开光石物品个数
	UINT8            byOpenLightRate; //开光成功率
	UINT32           dwOpenLightGold; //开光成功元宝
	DT_BATTLE_ATTRIBUTE stOldEquipInitBatlAttr; //旧装备0级战斗属性
	DT_BATTLE_ATTRIBUTE stNewEquipInitBatlAttr; //新装备0级战斗属性
	DT_BATTLE_ATTRIBUTE stOldEquipCurrBatlAttr; //旧装备当前战斗属性
}PKT_CLIGS_GET_OPENLIGHT_INFO_ACK;

typedef struct tagPKT_CLIGS_OPENLIGHT_INFO_REQ{
	tagPKT_CLIGS_OPENLIGHT_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPENLIGHT_INFO_REQ)); }
	UINT16           wEquipKindID; //装备ID
	UINT16           wEquipIdx; //装备序号
	UINT8            byGoldFlag; //元宝标志
}PKT_CLIGS_OPENLIGHT_INFO_REQ;

typedef struct tagPKT_CLIGS_OPENLIGHT_INFO_ACK{
	tagPKT_CLIGS_OPENLIGHT_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPENLIGHT_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wEquipKindID; //装备ID
	UINT16           wEquipIdx; //装备序号
	UINT8            byOpenLightFlagInfo; //装备开光标志信息(0不可开光，1可开光，2已开光)
	UINT64           qwCurGold; //当前元宝
	UINT16           wOpenLightGoodsID; //当前的开光石
	UINT16           wOpenLightGoodsNum; //当前开光石物品个数
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
}PKT_CLIGS_OPENLIGHT_INFO_ACK;

typedef struct tagPKT_CLIGS_CHANGE_CAREER_REQ{
	tagPKT_CLIGS_CHANGE_CAREER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_CAREER_REQ)); }
	UINT8            byCareerID; //职业ID
}PKT_CLIGS_CHANGE_CAREER_REQ;

typedef struct tagPKT_CLIGS_CHANGE_CAREER_ACK{
	tagPKT_CLIGS_CHANGE_CAREER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_CAREER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byCareerID; //职业ID
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
}PKT_CLIGS_CHANGE_CAREER_ACK;

typedef struct tagPKT_CLIGS_CHALLENGE_MYSELF_REQ{
	tagPKT_CLIGS_CHALLENGE_MYSELF_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHALLENGE_MYSELF_REQ)); }
	UINT8            byCareerID; //职业ID
}PKT_CLIGS_CHALLENGE_MYSELF_REQ;

typedef struct tagPKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ{
	tagPKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ;

typedef struct tagITEM_NUM_DATA{
	UINT16           wItemKindID; //道具ID
	UINT16           wNeedNum; //所需个数
	UINT16           wCurNum; //当前个数
}ITEM_NUM_DATA;

typedef struct tagITEM_EXCHANGE_DATA{
	TCHAR            aszTitle[MAX_COMMON_TXT_NUM]; //标题
	TCHAR            aszDscInfo[MAX_COMMON_TXT_NUM]; //描述
	UINT16           wExchangeIdx; //兑换项
	UINT8            byNeedItemNum; //道具数
	ITEM_NUM_DATA    astNeedItemInfo[ITEM_EXCHANGE_ITEM_NUM]; //兑换信息
	UINT16           wExchangeItemKindID; //可兑换的道具ID
	UINT16           wExchangeItemNum; //可兑换的道具数量
}ITEM_EXCHANGE_DATA;

typedef struct tagPKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK{
	tagPKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK)); }
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息
	UINT8            byExchangeNum; //兑换数
	ITEM_EXCHANGE_DATA astExchangeInfo[ITEM_EXCHANGE_ITEM_NUM]; //兑换信息
}PKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK;

typedef struct tagPKT_CLIGS_ITEM_EXCHANGE_REQ{
	tagPKT_CLIGS_ITEM_EXCHANGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ITEM_EXCHANGE_REQ)); }
	UINT16           wExchangeIdx; //兑换项
}PKT_CLIGS_ITEM_EXCHANGE_REQ;

typedef struct tagPKT_CLIGS_ITEM_EXCHANGE_ACK{
	tagPKT_CLIGS_ITEM_EXCHANGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ITEM_EXCHANGE_ACK)); }
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息
	DT_ITEM_DATA_CLI2 stExchangeItemInfo; //兑换所得的道具信息
	UINT8            byExchangeNum; //兑换数
	ITEM_EXCHANGE_DATA astExchangeInfo[ITEM_EXCHANGE_ITEM_NUM]; //兑换信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
}PKT_CLIGS_ITEM_EXCHANGE_ACK;

typedef struct tagDT_WANTED_INFO_CLI{
	UINT16           wWantedIndex; //通缉犯序号
	TCHAR            aszWantedName[WANTED_DSPNAME_LEN]; //通缉犯名
	TCHAR            aszWantedDesc[WANTED_DESC_LEN]; //通缉犯描述
	UINT8            byWantedState; //状态(1未解锁，2筹备中，3可挑战，4已击杀)
	DT_TASK_ENCOURAGE_DATA stEncourage; //奖励
	UINT32           dwScoreThreshold; //可击杀的线索积分阈值
}DT_WANTED_INFO_CLI;

typedef struct tagDT_WANTED_LIST_CLI{
	UINT8            byWantedNum; //通缉犯个数
	DT_WANTED_INFO_CLI astListData[MAX_WANTED_NUM]; //通缉犯信息列表
}DT_WANTED_LIST_CLI;

typedef struct tagPKT_CLIGS_OPEN_BIGREWARD_TAB_REQ{
	tagPKT_CLIGS_OPEN_BIGREWARD_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BIGREWARD_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_BIGREWARD_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_BIGREWARD_TAB_ACK{
	tagPKT_CLIGS_OPEN_BIGREWARD_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BIGREWARD_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurGold; //玩家当前的元宝
	DT_WANTED_LIST_CLI stWantedList; //通缉犯列表
	UINT32           dwCurClueScore; //当前线索积分
	UINT32           dwHerosRecoverCD; //英雄重伤恢复CD(0表示没有CD, 否则其值就是具体的CD)
}PKT_CLIGS_OPEN_BIGREWARD_TAB_ACK;

typedef struct tagDT_CLUE_TASK_INFO_CLI{
	UINT16           wIdx; //列表序号
	UINT16           wClueTaskType; //线索任务类型
	UINT32           dwClueScore; //线索积分
	UINT8            byState; //是否被选中过(0未被选中过，1被选中过且已完成，2被选中未完成(这个状态只可能会出现在最后一个线索任务的状态值上))
}DT_CLUE_TASK_INFO_CLI;

typedef struct tagDT_CLUE_TASK_LIST_CLI{
	UINT32           dwClueTaskNum; //任务个数
	DT_CLUE_TASK_INFO_CLI astListData[MAX_CLUE_TASK_NUM]; //列表信息
}DT_CLUE_TASK_LIST_CLI;

typedef struct tagDT_CLUE_TASK_KILL_MONSTER_NAVI{
	TCHAR            aszDesc[CLUE_TASK_NAVI_DESC_LEN]; //描述
	UINT16           wTargetNum; //击杀怪目标数量
	UINT16           wKilledNum; //已击杀怪数量
	DT_INSTANCE_ID   stInstanceID; //副本信息
	UINT32           dwClueScore; //线索积分奖励
	DT_TASK_ENCOURAGE_DATA stEncourage; //奖励(铜钱、元宝、阅历)
	UINT32           dwOnHookCoin; //立即完成花费的铜钱
}DT_CLUE_TASK_KILL_MONSTER_NAVI;

typedef struct tagDT_CLUE_TASK_ITEM_BOX_NAVI{
	TCHAR            aszDesc[CLUE_TASK_NAVI_DESC_LEN]; //描述
	DT_ITEM_DATA_LIST_CLI stItemInfo; //描述
}DT_CLUE_TASK_ITEM_BOX_NAVI;

typedef struct tagDT_CLUE_TASK_FORTUNE_GRID_NAVI{
	TCHAR            aszDesc[CLUE_TASK_NAVI_DESC_LEN]; //描述
	UINT32           dwClueScore; //线索积分奖励
	DT_TASK_ENCOURAGE_DATA stEncourage; //奖励(铜钱、元宝、阅历)
	UINT16           wNextIdx; //下一个直接前往的格子
}DT_CLUE_TASK_FORTUNE_GRID_NAVI;

typedef union tagDT_CLUE_TASK_NAVI_UNION{
	DT_CLUE_TASK_KILL_MONSTER_NAVI stKillMonster; //击杀怪物类型
	DT_CLUE_TASK_ITEM_BOX_NAVI stItemBox; //宝箱导航类型
	DT_CLUE_TASK_FORTUNE_GRID_NAVI stFortuneGrid; //幸运格导航类型
}DT_CLUE_TASK_NAVI_UNION;

typedef struct tagDT_CLUE_TASK_NAVI_CLI{
	UINT8            byClueTaskType; //类型
	DT_CLUE_TASK_NAVI_UNION stConcreteNavi; //各个任务具体的导航信息联合
}DT_CLUE_TASK_NAVI_CLI;

typedef struct tagDT_DICE_INFO_CLI{
	UINT16           wFreeShakeMaxTimes; //当天免费摇骰子次数上限
	UINT16           wLeftFreeShakeTimes; //剩余免费摇骰子次数
	UINT32           dwBuyShakeTimesGold; //下次购买摇骰子的元宝
	UINT16           wFreeRefreshMaxTimes; //免费重摇次数上限
	UINT16           wLeftFreeRefreshTimes; //剩余的重摇次数
	UINT32           dwBuyRefreshTimesGold; //下次购买重摇的元宝
	UINT32           dwCheatDiceGold; //下次出千的元宝
	UINT8            byLastDicePoint; //最近一次骰子点数
}DT_DICE_INFO_CLI;

typedef struct tagPKT_CLIGS_OPEN_COLLECT_CLUE_TAB_REQ{
	tagPKT_CLIGS_OPEN_COLLECT_CLUE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_COLLECT_CLUE_TAB_REQ)); }
	UINT16           wWantedIndex; //通缉犯序号
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_COLLECT_CLUE_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_COLLECT_CLUE_TAB_ACK{
	tagPKT_CLIGS_OPEN_COLLECT_CLUE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_COLLECT_CLUE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurGold; //玩家当前的元宝
	UINT32           dwCurClueScore; //当前线索积分
	DT_WANTED_INFO_CLI stCurWantedInfo; //当前通缉犯信息(这个接口中不需带奖励信息，奖励信息为NULL)
	DT_CLUE_TASK_LIST_CLI stClueTaskList; //线索任务列表
	UINT16           wCurClueTaskIdx; //当前线索任务的格子序号
	DT_DICE_INFO_CLI stDiceInfo; //骰子信息
}PKT_CLIGS_OPEN_COLLECT_CLUE_TAB_ACK;

typedef struct tagPKT_CLIGS_OPEN_CLUE_GRID_REQ{
	tagPKT_CLIGS_OPEN_CLUE_GRID_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CLUE_GRID_REQ)); }
	UINT16           wIdx; //列表序号
}PKT_CLIGS_OPEN_CLUE_GRID_REQ;

typedef struct tagPKT_CLIGS_OPEN_CLUE_GRID_ACK{
	tagPKT_CLIGS_OPEN_CLUE_GRID_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CLUE_GRID_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_CLUE_TASK_NAVI_CLI stClueTaskNavi; //各类线索任务导航信息(对于没有完成的导航信息将包括进度信息)
}PKT_CLIGS_OPEN_CLUE_GRID_ACK;

typedef struct tagPKT_CLIGS_SHAKE_DICE_REQ{
	tagPKT_CLIGS_SHAKE_DICE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SHAKE_DICE_REQ)); }
	UINT8            byCheatFlag; //出千标识(0不出千，n标识出千指定的点数)
}PKT_CLIGS_SHAKE_DICE_REQ;

typedef struct tagPKT_CLIGS_SHAKE_DICE_ACK{
	tagPKT_CLIGS_SHAKE_DICE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SHAKE_DICE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byDicePoint; //骰子点数
	UINT16           wFreeShakeMaxTimes; //当天免费摇骰子次数上限
	UINT16           wFreeShakeLeftTimes; //剩余免费摇骰子次数
	UINT32           dwBuyShakeTimesGold; //下次购买摇骰子的元宝
	UINT32           dwCheatDiceGold; //下次出千的元宝
	UINT64           qwCurGold; //玩家当前的元宝
}PKT_CLIGS_SHAKE_DICE_ACK;

typedef struct tagPKT_CLIGS_REFRESH_DICE_REQ{
	tagPKT_CLIGS_REFRESH_DICE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_REFRESH_DICE_REQ)); }
	UINT8            byCheatFlag; //出千标识(0不出千，n标识出千指定的点数)
}PKT_CLIGS_REFRESH_DICE_REQ;

typedef struct tagPKT_CLIGS_REFRESH_DICE_ACK{
	tagPKT_CLIGS_REFRESH_DICE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_REFRESH_DICE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byDicePoint; //骰子点数
	UINT16           wFreeRefreshMaxTimes; //免费重摇次数上限
	UINT16           wLeftFreeRefreshTimes; //剩余的免费重摇次数
	UINT32           dwBuyRefreshTimesGold; //下次购买重摇的元宝
	UINT32           dwCheatDiceGold; //下次出千的元宝
	UINT64           qwCurGold; //玩家当前的元宝
}PKT_CLIGS_REFRESH_DICE_ACK;

typedef struct tagPKT_CLIGS_COMFIRM_DICE_POINT_REQ{
	tagPKT_CLIGS_COMFIRM_DICE_POINT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COMFIRM_DICE_POINT_REQ)); }
	UINT8            byDicePoint; //骰子点数
}PKT_CLIGS_COMFIRM_DICE_POINT_REQ;

typedef struct tagPKT_CLIGS_COMFIRM_DICE_POINT_ACK{
	tagPKT_CLIGS_COMFIRM_DICE_POINT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COMFIRM_DICE_POINT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wDestClueTaskIdx; //前往的线索任务的格子序号
}PKT_CLIGS_COMFIRM_DICE_POINT_ACK;

typedef struct tagPKT_CLIGS_PLAYER_RES_NTF{
	tagPKT_CLIGS_PLAYER_RES_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_RES_NTF)); }
	UINT8            byResNum; //资源数
	DT_PLAYER_RES_DATA astResInfo[MAX_PLAYER_RES_NUM]; //武将信息
}PKT_CLIGS_PLAYER_RES_NTF;

typedef struct tagPKT_CLIGS_OPERATING_ACTIVITY_NTF{
	tagPKT_CLIGS_OPERATING_ACTIVITY_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_OPERATING_ACTIVITY_NTF)); }
	DT_OPERATING_ACTIVITY_DATA stOperatingActivityInfo; //运营活动信息
}PKT_CLIGS_OPERATING_ACTIVITY_NTF;

typedef struct tagPKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ{
	tagPKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ)); }
	TCHAR            aszBattleKey[MAX_COMMON_TXT_LEN]; //请求的战斗确认串
}PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ;

typedef struct tagDT_FRIEND_SYS_PLAYER_INFO{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszPlayerName[USERNAME_LEN]; //玩家名称
	UINT16           wMainHeroKindID; //玩家主角角色ID
	UINT8            byIsOnline; //是否在线
}DT_FRIEND_SYS_PLAYER_INFO;

typedef struct tagDT_FRIEND_SYS_PLAYER_LIST_CLI{
	UINT32           dwPlayerNum; //玩家个数
	DT_FRIEND_SYS_PLAYER_INFO astListData[MAX_FRIEND_SYS_PLAYER_LIST_ELEM_NUM]; //列表信息
}DT_FRIEND_SYS_PLAYER_LIST_CLI;

typedef struct tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ{
	tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ)); }
	UINT8            byGetListType; //获取列表的类型(0好友, 1陌生人, 2黑名单)
	UINT8            byPageCapacity; //每页的显示个数
	UINT8            byPageNo; //查询的页码(从1开始)
}PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ;

typedef struct tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK{
	tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byGetListType; //获取列表的类型(0好友, 1陌生人, 2黑名单)
	UINT32           dwTotalNum; //某类型列表玩家总个数
	UINT8            byPageNo; //查询的页码(从1开始)
	DT_FRIEND_SYS_PLAYER_LIST_CLI stPlayerList; //玩家列表
}PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK;

typedef struct tagDT_FRIEND_SYS_PLAYER_INFO2{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszPlayerName[USERNAME_LEN]; //玩家名称
	UINT16           wMainHeroKindID; //玩家主角角色ID
	UINT8            byIsOnline; //是否在线
	UINT8            byRelation; //玩家关系
}DT_FRIEND_SYS_PLAYER_INFO2;

typedef struct tagDT_FRIEND_SYS_PLAYER_LIST_CLI2{
	UINT32           dwPlayerNum; //玩家个数
	DT_FRIEND_SYS_PLAYER_INFO2 astListData[MAX_FRIEND_SYS_PLAYER_LIST_ELEM_NUM]; //列表信息
}DT_FRIEND_SYS_PLAYER_LIST_CLI2;

typedef struct tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2{
	tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2)); }
	UINT8            byGetListType; //获取列表的类型(0好友, 1陌生人, 2黑名单)
	UINT8            byPageCapacity; //每页的显示个数
	UINT8            byPageNo; //查询的页码(从1开始)
}PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2;

typedef struct tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2{
	tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byGetListType; //获取列表的类型(0好友, 1陌生人, 2黑名单)
	UINT32           dwTotalNum; //某类型列表玩家总个数
	UINT8            byPageNo; //查询的页码(从1开始)
	DT_FRIEND_SYS_PLAYER_LIST_CLI2 stPlayerList; //玩家列表
}PKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2;

typedef struct tagPKT_CLIGS_DEL_FRIEND_REQ{
	tagPKT_CLIGS_DEL_FRIEND_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DEL_FRIEND_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_DEL_FRIEND_REQ;

typedef struct tagPKT_CLIGS_DEL_FRIEND_ACK{
	tagPKT_CLIGS_DEL_FRIEND_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DEL_FRIEND_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_DEL_FRIEND_ACK;

typedef struct tagPKT_CLIGS_SEARCH_PLAYER_REQ{
	tagPKT_CLIGS_SEARCH_PLAYER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEARCH_PLAYER_REQ)); }
	TCHAR            aszPlayerName[USERNAME_LEN]; //玩家名称
	UINT8            byPageCapacity; //每页的显示个数
	UINT8            byPageNo; //查询的页码(从1开始)
}PKT_CLIGS_SEARCH_PLAYER_REQ;

typedef struct tagPKT_CLIGS_SEARCH_PLAYER_ACK{
	tagPKT_CLIGS_SEARCH_PLAYER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SEARCH_PLAYER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwTotalNum; //列表玩家总个数
	UINT8            byPageNo; //查询的页码(从1开始)
	DT_FRIEND_SYS_PLAYER_LIST_CLI stSearchRltList; //搜索结果列表
}PKT_CLIGS_SEARCH_PLAYER_ACK;

typedef struct tagPKT_CLIGS_SEARCH_PLAYER_REQ2{
	tagPKT_CLIGS_SEARCH_PLAYER_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_SEARCH_PLAYER_REQ2)); }
	TCHAR            aszPlayerName[USERNAME_LEN]; //玩家名称
	UINT8            byPageCapacity; //每页的显示个数
	UINT8            byPageNo; //查询的页码(从1开始)
}PKT_CLIGS_SEARCH_PLAYER_REQ2;

typedef struct tagPKT_CLIGS_SEARCH_PLAYER_ACK2{
	tagPKT_CLIGS_SEARCH_PLAYER_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_SEARCH_PLAYER_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwTotalNum; //列表玩家总个数
	UINT8            byPageNo; //查询的页码(从1开始)
	DT_FRIEND_SYS_PLAYER_LIST_CLI2 stSearchRltList; //搜索结果列表
}PKT_CLIGS_SEARCH_PLAYER_ACK2;

typedef struct tagDT_FRIEND_SYS_PLAYER_GENERAL_INFO_CLI{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszPlayerName[USERNAME_LEN]; //玩家名称
	UINT16           wMainHeroKindID; //玩家主角角色ID
	UINT16           wPlayerLvl; //玩家等级
	UINT32           dwPlayerPower; //玩家战力
	UINT64           qwFlowerSum; //玩家鲜花总数
	UINT8            byRelationType; //关系类型(0好友, 1陌生人, 2黑名单)
	UINT32           dwRank; //玩家竞技排名
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT8            byVipLevel; //VIP等级
}DT_FRIEND_SYS_PLAYER_GENERAL_INFO_CLI;

typedef struct tagPKT_CLIGS_VIEW_PLAYER_REQ{
	tagPKT_CLIGS_VIEW_PLAYER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_VIEW_PLAYER_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_VIEW_PLAYER_REQ;

typedef struct tagDT_HERO_ATTR_LST_INFO{
	UINT16           wHeroNum; //伙伴个数
	UINT16           awHeroID[MAX_HERO_NUM]; //伙伴ID
	UINT16           awTalent[MAX_HERO_NUM]; //资质
	DT_BATTLE_ATTRIBUTE2 astBaseAttr[MAX_HERO_NUM]; //基本属性
	DT_HIDE_BATTLE_ATTRIBUTE astHideAttr[MAX_HERO_NUM]; //隐藏属性
	UINT32           adwPower[MAX_HERO_NUM]; //战斗力
	UINT16           awLevel[MAX_HERO_NUM]; //等级
}DT_HERO_ATTR_LST_INFO;

typedef struct tagDT_SPECIAL_EFFECT_DESC{
	UINT8            byEffectType; //特效类型
	UINT16           wTriggerProbValue; //发动概率等级
	UINT16           wEffectPercentValue; //效果百分比等级
	UINT16           wLimitNumValue; //限制次数等级
}DT_SPECIAL_EFFECT_DESC;

typedef struct tagDT_SPECIAL_EFFECT_LST_DESC{
	UINT8            byDescNum; //描述个数
	DT_SPECIAL_EFFECT_DESC astSkillDesc[MAX_SKILL_SOUL_NUM]; //描述
}DT_SPECIAL_EFFECT_LST_DESC;

typedef struct tagDT_SPECIAL_EFFECT_DATA_CLI{
	UINT16           wEffectID; //特效ID
	TCHAR            aszEffectName[MAX_NAME_LEN]; //特效攻击名称
	UINT8            byIsActivity; //是否激活
	UINT8            byUnlockLevel; //特效攻击解锁等级
	DT_SPECIAL_EFFECT_LST_DESC stSkillLstDesc; //描述
}DT_SPECIAL_EFFECT_DATA_CLI;

typedef struct tagDT_SOUL_ATTR_DATA_CLI{
	UINT8            byKindID; //属性ID
	UINT32           dwKindValue; //属性值
	UINT32           dwKindPercent; //属性百分比
}DT_SOUL_ATTR_DATA_CLI;

typedef struct tagDT_SOUL_DATA_CLI{
	UINT16           wSoulID; //战魂ID
	UINT32           dwSoulIdx; //战魂Idx(区分同名战魂)
	UINT8            bySoulStar; //战魂星级
	UINT8            bySoulType; //战魂类型 1代表攻击类 2代表防御类
	UINT8            bySoulLevel; //战魂等级
	UINT32           dwSoulExperience; //战魂经验
	UINT32           dwMaxExperience; //当前级别经验上限
	UINT32           dwReturnExperience; //返还经验
	UINT8            byIsNormalSoul; //0 代表吞噬魂 1 代表普通魂
	UINT8            byBaseNum; //战魂基本属性个数
	DT_SOUL_ATTR_DATA_CLI astSoulAttr[MAX_SOUL_ATTR_NUM]; //战魂属性
	UINT8            byBaseAfterNum; //战魂基本属性个数(洗炼后)
	DT_SOUL_ATTR_DATA_CLI astSoulAfterAttr[MAX_SOUL_ATTR_NUM]; //战魂属性(洗炼后)
	TCHAR            aszSoulName[MAX_SOUL_NAME_LEN]; //战魂名字
	TCHAR            aszSoulPic[MAX_SOUL_PIC_LEN]; //战魂图片名字
	DT_SPECIAL_EFFECT_DATA_CLI stSpecialEffect; //特殊技能
	TCHAR            aszNamePic[MAX_SOUL_PIC_LEN]; //战魂名字图片名字
	UINT8            abySoulAttrLock[MAX_SOUL_ATTR_NUM]; //锁定魂
}DT_SOUL_DATA_CLI;

typedef struct tagDT_SOUL_POTENCY_CLI{
	UINT8            byPotencyLevel; //潜能等阶
	UINT8            byPotencySubLevel; //潜能等级
}DT_SOUL_POTENCY_CLI;

typedef struct tagDT_SOUL_DATA_LST_CLI{
	UINT16           wSoulNum; //战魂个数
	DT_SOUL_DATA_CLI astSoulLstData[MAX_SOUL_BAG_NUM]; //战魂信息
}DT_SOUL_DATA_LST_CLI;

typedef struct tagDT_SOUL_POTENCY_DATA_LST_CLI{
	UINT16           wSoulNum; //战魂个数
	DT_SOUL_POTENCY_CLI astSoulLstData[MAX_SOUL_BAG_NUM]; //战魂信息
}DT_SOUL_POTENCY_DATA_LST_CLI;

typedef struct tagDT_HERO_SOUL_DATA_LST_CLI{
	UINT16           wSoulEquipNum; //装备战魂个数
	DT_SOUL_DATA_CLI astSoulEquipLstData[MAX_SOUL_EQUIP_NUM]; //装备战魂信息
}DT_HERO_SOUL_DATA_LST_CLI;

typedef struct tagDT_HERO_SOUL_POTENCY_DATA_LST_CLI{
	UINT16           wSoulEquipNum; //装备战魂个数
	DT_SOUL_POTENCY_CLI astSoulEquipLstData[MAX_SOUL_EQUIP_NUM]; //装备战魂信息
}DT_HERO_SOUL_POTENCY_DATA_LST_CLI;

typedef struct tagDT_HERO_LST_SOUL_POTENCY_DATA_LST_CLI{
	UINT16           wHeroNum; //英雄个数
	DT_HERO_SOUL_POTENCY_DATA_LST_CLI astSoulEquipLstData[MAX_HERO_NUM]; //战魂信息
}DT_HERO_LST_SOUL_POTENCY_DATA_LST_CLI;

typedef struct tagDT_ATTR_BASE_DATA{
	UINT8            byKindID; //属性ID
	UINT32           dwAttrValue; //属性值
	UINT8            byValueType; //属性值类型（1固定值， 2百分数）参考commondef.h ValueType
}DT_ATTR_BASE_DATA;

typedef struct tagDT_ATTR_BASE_LST_DATA{
	UINT8            byAttrNum; //属性个数
	DT_ATTR_BASE_DATA astAttrInfo[MAX_SCIENCE_ATTR_NUM]; //属性值
}DT_ATTR_BASE_LST_DATA;

typedef struct tagDT_MEDAL_DATA_CLI{
	TCHAR            aszMedalName[MAX_NAME_LEN]; //勋章名称
	TCHAR            aszMedalIconName[MAX_NAME_LEN]; //勋章名称
	TCHAR            aszFinishDesc[MAX_MEDAL_DESC_LEN]; //勋章完成条件
	UINT8            byIsActivate; //勋章激活状态
	DT_ATTR_BASE_LST_DATA stCurAttrInfo; //勋章当前属性
	DT_ATTR_BASE_LST_DATA stNextAttrInfo; //勋章下一级属性
	UINT16           wMedalLevel; //勋章等级
	UINT64           qwValidityTime; //勋章有效期
	UINT64           qwOverTime; //勋章过期时间
	UINT8            byMedalType; //勋章类别
	UINT16           wSmallType; //勋章排序类别(按从小到大排列，可不连序)
	UINT8            byMedalColor; //名称着色
	UINT8            byIsLevelMedal; //是否是等级勋章
}DT_MEDAL_DATA_CLI;

typedef struct tagDT_MEDAL_DATA_LST_CLI{
	UINT16           wMedalNum; //勋章个数
	DT_MEDAL_DATA_CLI astMedalInfo[MAX_MEDAL_NUM]; //勋章信息
}DT_MEDAL_DATA_LST_CLI;

typedef struct tagPKT_CLIGS_VIEW_PLAYER_ACK{
	tagPKT_CLIGS_VIEW_PLAYER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_VIEW_PLAYER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FRIEND_SYS_PLAYER_GENERAL_INFO_CLI stPlayerGeneralInfo; //玩家概要信息
	UINT8            byFactionIconID; //当前门派头像ID
	DT_HERO_ATTR_LST_INFO stAttrInfo; //属性信息
	UINT8            byJewelryOpen; //饰品开放状态（0未开放， 1开放）
	UINT8            byGodWeaponOpen; //神器开放状态（0未开放， 1开放）
	UINT8            byAwakenOpen; //卦象开放状态（0未开放， 1开放）
	UINT8            bySoulOpen; //战魂开放状态（0未开放， 1开放）
	UINT16           wHeroNum; //伙伴个数
	UINT8            abyJobType[MAX_HERO_NUM]; //职业类型
	DT_MEDAL_DATA_LST_CLI stMedalInfo; //玩家勋章
	UINT8            byPetOpen; //宠物开放状态（0未开放， 1开放）
}PKT_CLIGS_VIEW_PLAYER_ACK;

typedef struct tagPKT_CLIGS_ADD_FRIEND_REQ{
	tagPKT_CLIGS_ADD_FRIEND_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ADD_FRIEND_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_ADD_FRIEND_REQ;

typedef struct tagPKT_CLIGS_ADD_FRIEND_ACK{
	tagPKT_CLIGS_ADD_FRIEND_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ADD_FRIEND_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_ADD_FRIEND_ACK;

typedef struct tagDT_SEND_FLOWER_PROP_INFO{
	UINT32           dwFlowerNum; //鲜花数
	UINT32           dwCoin; //花费的铜钱
	UINT32           dwGold; //花费的元宝
	UINT16           wGoodsKindID; //得到的物品KindID
	UINT32           dwGoodsNum; //得到的物品数量
	UINT32           dwLeftSendTimes; //剩余赠送次数
	UINT8            byNeedVipLvl; //需要的VIP等级(0表示不需要VIP等级)
}DT_SEND_FLOWER_PROP_INFO;

typedef struct tagDT_SEND_FLOWER_PROP_LIST_CLI{
	UINT32           dwPropNum; //配置个数
	DT_SEND_FLOWER_PROP_INFO astListData[MAX_SEND_FLOWER_PROP_NUM]; //列表信息
}DT_SEND_FLOWER_PROP_LIST_CLI;

typedef struct tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ{
	tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ)); }
	UINT8            byExt; //扩展字段
	UINT32           dwRecvPlayerID; //收花玩家ID
}PKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_ACK{
	tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_SEND_FLOWER_PROP_LIST_CLI stSendFlowerPropList; //送鲜花配置信息列表
}PKT_CLIGS_OPEN_SEND_FLOWER_TAB_ACK;

typedef struct tagPKT_CLIGS_SEND_FLOWER_REQ{
	tagPKT_CLIGS_SEND_FLOWER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEND_FLOWER_REQ)); }
	UINT32           dwRecvPlayerID; //收花玩家ID
	UINT32           dwSendFlowerNum; //送花的数量
}PKT_CLIGS_SEND_FLOWER_REQ;

typedef struct tagPKT_CLIGS_SEND_FLOWER_ACK{
	tagPKT_CLIGS_SEND_FLOWER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SEND_FLOWER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCurCoin; //当前的铜钱
	UINT64           qwCurGold; //当前的元宝
	UINT16           wGoodsKindID; //得到的物品KindID
	UINT64           qwCurGoodsNum; //当前的物品数量
	UINT64           qwRecverFlowerSum; //收方的鲜花总数
}PKT_CLIGS_SEND_FLOWER_ACK;

typedef struct tagDT_RECV_FLOWER_LOG{
	UINT32           dwSendPlayerID; //送花的玩家ID
	TCHAR            aszSendPlayerName[USERNAME_LEN]; //送花的玩家名称
	UINT32           dwFlowerNum; //鲜花数
	UINT64           qwSendTime; //送花的时间
}DT_RECV_FLOWER_LOG;

typedef struct tagDT_RECV_FLOWER_LOG_LIST_CLI{
	UINT32           dwLogNum; //记录个数
	DT_RECV_FLOWER_LOG astListData[MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM]; //列表信息
}DT_RECV_FLOWER_LOG_LIST_CLI;

typedef struct tagPKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ{
	tagPKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ;

typedef struct tagPKT_CLIGS_GET_FLOWER_BY_PLAYERID_ACK{
	tagPKT_CLIGS_GET_FLOWER_BY_PLAYERID_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FLOWER_BY_PLAYERID_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwFlowerSum; //我的鲜花总数
	UINT32           dwMyFlowerRanking; //我的鲜花榜排行
	DT_RECV_FLOWER_LOG_LIST_CLI stRecvFlowerLogList; //收花记录
}PKT_CLIGS_GET_FLOWER_BY_PLAYERID_ACK;

typedef struct tagDT_FLOWER_RANKING_INFO{
	UINT32           dwPlayerID; //送花的玩家ID
	TCHAR            aszPlayerName[USERNAME_LEN]; //送花的玩家名称
	UINT64           qwFlowerSum; //鲜花数
	UINT32           dwFlowerRanking; //鲜花排行
}DT_FLOWER_RANKING_INFO;

typedef struct tagDT_FLOWER_RANKING_LIST_CLI{
	UINT32           dwRankingNum; //排行个数
	DT_FLOWER_RANKING_INFO astListData[MAX_FLOWER_RANKING_LIST_ELEM_NUM]; //列表信息
}DT_FLOWER_RANKING_LIST_CLI;

typedef struct tagPKT_CLIGS_GET_FLOWER_RANKING_REQ{
	tagPKT_CLIGS_GET_FLOWER_RANKING_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FLOWER_RANKING_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_GET_FLOWER_RANKING_REQ;

typedef struct tagPKT_CLIGS_GET_FLOWER_RANKING_ACK{
	tagPKT_CLIGS_GET_FLOWER_RANKING_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_FLOWER_RANKING_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FLOWER_RANKING_LIST_CLI stFlowerRankingList; //扩展字段
}PKT_CLIGS_GET_FLOWER_RANKING_ACK;

typedef struct tagDT_PRIVATE_MSG_DATA{
	UINT32           dwMsgIdx; //消息序号(越早的消息，MsgIdx越小)
	TCHAR            aszMsgContent[PRIVATE_MSG_CONTENT_LEN]; //私聊消息内容
	UINT8            byIsRecv; //收发标识(0表自己发出的，1表对方发过来的)
	UINT64           qwSendTime; //送花的时间
}DT_PRIVATE_MSG_DATA;

typedef struct tagDT_PRIVATE_MSG_LIST_CLI{
	UINT32           dwMsgNum; //消息个数
	DT_PRIVATE_MSG_DATA astListData[MAX_PRIVATE_MSG_LIST_ELEM_NUM]; //列表信息
}DT_PRIVATE_MSG_LIST_CLI;

typedef struct tagPKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ{
	tagPKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ)); }
	UINT32           dwPlayerID; //玩家ID
	UINT32           dwStartIdx; //起始序号
	UINT8            byGetMsgNum; //获取消息个数(向更小的序号方向取)
}PKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ;

typedef struct tagPKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK{
	tagPKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwTotalNum; //消息总条数
	UINT32           dwStartIdx; //请求时的起始序号
	DT_PRIVATE_MSG_LIST_CLI stPrivateMsgList; //私聊消息记录列表
	UINT8            byPeerRole; //对方是什么角色：普通玩家0 / GM 1，见:PrivateChatRoleType
}PKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK;

typedef struct tagPKT_CLIGS_SEND_PRIVATE_MSG_REQ{
	tagPKT_CLIGS_SEND_PRIVATE_MSG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEND_PRIVATE_MSG_REQ)); }
	UINT32           dwRecvPlayerID; //收方玩家ID
	TCHAR            aszMsgContent[PRIVATE_MSG_CONTENT_LEN]; //私聊消息内容
}PKT_CLIGS_SEND_PRIVATE_MSG_REQ;

typedef struct tagPKT_CLIGS_SEND_PRIVATE_MSG_ACK{
	tagPKT_CLIGS_SEND_PRIVATE_MSG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SEND_PRIVATE_MSG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	TCHAR            aszErrInfo[MAX_COMMON_TXT_NUM]; //错误信息(有值则错误，优先于错误码)
}PKT_CLIGS_SEND_PRIVATE_MSG_ACK;

typedef struct tagPKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ{
	tagPKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ;

typedef struct tagPKT_CLIGS_DEL_PRIVATE_MSG_LOG_ACK{
	tagPKT_CLIGS_DEL_PRIVATE_MSG_LOG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DEL_PRIVATE_MSG_LOG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_DEL_PRIVATE_MSG_LOG_ACK;

typedef struct tagPKT_CLIGS_ADD_TO_BLACKLIST_REQ{
	tagPKT_CLIGS_ADD_TO_BLACKLIST_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ADD_TO_BLACKLIST_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_ADD_TO_BLACKLIST_REQ;

typedef struct tagPKT_CLIGS_ADD_TO_BLACKLIST_ACK{
	tagPKT_CLIGS_ADD_TO_BLACKLIST_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ADD_TO_BLACKLIST_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_ADD_TO_BLACKLIST_ACK;

typedef struct tagPKT_CLIGS_MOVE_FROM_BLACKLIST_REQ{
	tagPKT_CLIGS_MOVE_FROM_BLACKLIST_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MOVE_FROM_BLACKLIST_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_MOVE_FROM_BLACKLIST_REQ;

typedef struct tagPKT_CLIGS_MOVE_FROM_BLACKLIST_ACK{
	tagPKT_CLIGS_MOVE_FROM_BLACKLIST_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MOVE_FROM_BLACKLIST_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_MOVE_FROM_BLACKLIST_ACK;

typedef struct tagPKT_CLIGS_ONLINE_PRIVATE_MSG_NTF{
	tagPKT_CLIGS_ONLINE_PRIVATE_MSG_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_ONLINE_PRIVATE_MSG_NTF)); }
	UINT32           dwMsgIdx; //消息序号(越早的消息，MsgIdx越小)
	UINT32           dwSendPlayerID; //发方玩家ID
	TCHAR            aszSendPlayerName[USERNAME_LEN]; //发方玩家名称
	UINT16           wMainHeroKindID; //玩家主角角色ID
	UINT8            byRelationType; //关系类型(0好友, 1陌生人, 2黑名单,100添加好友通知)
	TCHAR            aszMsgContent[PRIVATE_MSG_CONTENT_LEN]; //私聊消息内容
	UINT64           qwSendTime; //发送时间
}PKT_CLIGS_ONLINE_PRIVATE_MSG_NTF;

typedef struct tagDT_OFFLINE_MSG_SENDER_INFO{
	UINT32           dwSendPlayerID; //发方玩家ID
	TCHAR            aszSendPlayerName[USERNAME_LEN]; //发方玩家名称
	UINT16           wMainHeroKindID; //玩家主角角色ID
	UINT8            byRelationType; //关系类型(0好友, 1陌生人, 2黑名单,100添加好友离线通知)
	UINT16           wOfflineMsgCnt; //私聊消息条数
}DT_OFFLINE_MSG_SENDER_INFO;

typedef struct tagDT_SEND_OFFLINE_MSG_PLAYER_LIST_CLI{
	UINT32           dwPlayerNum; //玩家个数
	DT_OFFLINE_MSG_SENDER_INFO astListData[MAX_SEND_OFFLINE_MSG_PLAYER_LIST_ELEM_NUM]; //列表信息
}DT_SEND_OFFLINE_MSG_PLAYER_LIST_CLI;

typedef struct tagPKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ{
	tagPKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ;

typedef struct tagPKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK{
	tagPKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_SEND_OFFLINE_MSG_PLAYER_LIST_CLI stPlayerList; //发离线消息的玩家列表
}PKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK;

typedef struct tagPKT_CLIGS_READ_OFFLINE_MSG_REQ{
	tagPKT_CLIGS_READ_OFFLINE_MSG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_READ_OFFLINE_MSG_REQ)); }
	UINT32           dwSendPlayerID; //发方玩家ID
}PKT_CLIGS_READ_OFFLINE_MSG_REQ;

typedef struct tagPKT_CLIGS_READ_OFFLINE_MSG_ACK{
	tagPKT_CLIGS_READ_OFFLINE_MSG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_READ_OFFLINE_MSG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_READ_OFFLINE_MSG_ACK;

typedef struct tagDT_RAREITEM_INFO_CLI{
	UINT8            byRareItemType; //物品类型(１装备,２药品，3资源，4道具，5技能)
	DT_ITEM_DATA_CLI2 stRareItemInfo; //物品信息
	UINT8            byRareItemExchangeRule; //物品兑换规则(1按日 2按周 3按月 4永久)
	UINT32           dwRareItemSumRequire; //累积门贡要求
	UINT32           dwRareItemExchangeMoney; //物品兑换门贡需求
	UINT32           dwRareItemLeftExchange; //物品兑换剩余次数
	UINT8            byIsUnLock; //是否解锁0未解锁 , 1解锁，
	UINT8            byUnlockLevel; //解锁等级
	UINT8            byDoorsTributeEnough; //剩余门贡满足（0 不满足， 1满足）
	UINT8            bySumDoorsTributeEnough; //累积门贡满足（0 不满足， 1满足）
	UINT8            byBuyNeedLevel; //购买等级（0没有等级要求）
	UINT8            byNeedLeveEnough; //购买等级满足（0 不满足， 1满足）
}DT_RAREITEM_INFO_CLI;

typedef struct tagDT_RAREITEM_INFO_LIST_CLI{
	UINT8            byRareItemNum; //物品个数
	DT_RAREITEM_INFO_CLI astRareItemInfoList[MAX_RAREITEM_RES_NUM]; //物品数组
}DT_RAREITEM_INFO_LIST_CLI;

typedef struct tagDT_RAREITEM_ROOM_INFO_CLI{
	UINT32           dwRemainMoney; //个人剩余门贡
	UINT64           qwAccumulateMoney; //个人累积门贡
	UINT8            byRareItemGrade; //珍品阁等级信息
	UINT8            byUpgradeLimit; //-升级权限
}DT_RAREITEM_ROOM_INFO_CLI;

typedef struct tagPKT_CLIGS_OPEN_RAREITEMROOM_REQ{
	tagPKT_CLIGS_OPEN_RAREITEMROOM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RAREITEMROOM_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_RAREITEMROOM_REQ;

typedef struct tagPKT_CLIGS_OPEN_RAREITEMROOM_ACK{
	tagPKT_CLIGS_OPEN_RAREITEMROOM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RAREITEMROOM_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RAREITEM_ROOM_INFO_CLI stRareItemRoomInfo; //珍品阁信息
	DT_RAREITEM_INFO_LIST_CLI stRareItemInfo; //珍品信息
}PKT_CLIGS_OPEN_RAREITEMROOM_ACK;

typedef struct tagPKT_CLIGS_BUY_RAREITEM_REQ{
	tagPKT_CLIGS_BUY_RAREITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_RAREITEM_REQ)); }
	UINT32           dwGoodsID; //物品ID
}PKT_CLIGS_BUY_RAREITEM_REQ;

typedef struct tagPKT_CLIGS_BUY_RAREITEM_ACK{
	tagPKT_CLIGS_BUY_RAREITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_RAREITEM_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_RAREITEM_INFO_CLI stRareItemInfo; //总后换珍品信息
	UINT32           dwLeftDoorsTributes; //玩家剩余门贡
}PKT_CLIGS_BUY_RAREITEM_ACK;

typedef struct tagPKT_CLIGS_OPEN_UPGRADERAREITEM_REQ{
	tagPKT_CLIGS_OPEN_UPGRADERAREITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_UPGRADERAREITEM_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_UPGRADERAREITEM_REQ;

typedef struct tagDT_UPGRADE_RAREITEM_INFO{
	UINT32           dwUpgradeKindID; //升级物品ID
	UINT16           wRateItemNum; //提升个数
}DT_UPGRADE_RAREITEM_INFO;

typedef struct tagDT_UPGRADE_MSG_LIST{
	TCHAR            aszUpgradeMsg[FACTION_UPGRADE_MSG_LEN]; //物品升级描述信息
}DT_UPGRADE_MSG_LIST;

typedef struct tagDT_UPGRADE_REQUIRE_INFO_CLI{
	UINT32           dwConsumptionMoney; //需要的资金
	UINT8            byFactionFundsEnough; //门派资金是否足够(0 不够 1 足够)
	UINT8            byUnitedGrade; //要求门派等级
	UINT8            byFactionIsEnough; //门派等级是否达到
	UINT8            byInfoListNum; //信息数组个数
	DT_UPGRADE_MSG_LIST astUpgradeInfoList[MAX_UPGRADE_INFO_NUM]; //升级信息数组
	UINT8            byBuildLevel; //升级前的等级
	UINT8            byIsUpgradeLimit; //升级权限0 没升级权限， 1有升级权限
}DT_UPGRADE_REQUIRE_INFO_CLI;

typedef struct tagPKT_CLIGS_OPEN_UPGRADERAREITEM_ACK{
	tagPKT_CLIGS_OPEN_UPGRADERAREITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_UPGRADERAREITEM_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_UPGRADE_REQUIRE_INFO_CLI stUpgradeRequireInfo; //升级要求数据
}PKT_CLIGS_OPEN_UPGRADERAREITEM_ACK;

typedef struct tagPKT_CLIGS_UPGRADE_RAREITEM_REQ{
	tagPKT_CLIGS_UPGRADE_RAREITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_RAREITEM_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_UPGRADE_RAREITEM_REQ;

typedef struct tagDT_AFTER_UPGRADE_INFO_CLI{
	UINT8            byRareItemGrade; //升级后的珍品阁等级
	UINT8            byIsMaxLevel; //升级后门派等级是否达到上限(0 未达到上限, 1达到上限)
	DT_RAREITEM_INFO_LIST_CLI stRareItemInfo; //珍品信息
}DT_AFTER_UPGRADE_INFO_CLI;

typedef struct tagPKT_CLIGS_UPGRADE_RAREITEM_ACK{
	tagPKT_CLIGS_UPGRADE_RAREITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_RAREITEM_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_AFTER_UPGRADE_INFO_CLI stAfterUpgradeInfo; //升级后的信息
	DT_UPGRADE_REQUIRE_INFO_CLI stUpgradeRequireInfo; //下一级升级要求
	UINT32           dwFactionFunds; //资金同步
}PKT_CLIGS_UPGRADE_RAREITEM_ACK;

typedef struct tagPKT_CLIGS_OPEN_GYMNASIUM_REQ{
	tagPKT_CLIGS_OPEN_GYMNASIUM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GYMNASIUM_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_GYMNASIUM_REQ;

typedef struct tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ{
	tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ)); }
	UINT32           dwSkillID; //技能ID
}PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ;

typedef struct tagPKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ{
	tagPKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ;

typedef struct tagPKT_CLIGS_GYMNASIUM_UPGRADE_REQ{
	tagPKT_CLIGS_GYMNASIUM_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GYMNASIUM_UPGRADE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_GYMNASIUM_UPGRADE_REQ;

typedef struct tagDT_GYMNASIUM_CLI_INFO{
	UINT8            byGymnasiumLevel; //武堂等级
	UINT32           dwDoorsTribute; //门贡
	UINT8            byUpgradeLimit; //升级权限
}DT_GYMNASIUM_CLI_INFO;

typedef struct tagDT_SKILL_CLI_INFO{
	UINT8            bySkillID; //技能ID
	TCHAR            aszGymnasiumSkillName[SKILL_NAME_LEN]; //技能名称
	UINT8            bySkillFlag; //技能状态，1可学习，2达到当前满级，3 达到总满级，4门贡不足，5 未解锁 6累积门贡不足
	UINT8            byCurMaxSkillLevel; //当前技能上限
	UINT8            bySkillLevel; //技能等级
	TCHAR            aszSkillMsg[SKILL_MSG_LEN]; //技能信息
	UINT32           dwCostDoorsTribute; //消耗门贡
	UINT32           dwGymnasiumLevelRequire; //需要武堂等级
}DT_SKILL_CLI_INFO;

typedef struct tagPKT_CLIGS_OPEN_GYMNASIUM_ACK{
	tagPKT_CLIGS_OPEN_GYMNASIUM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GYMNASIUM_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_GYMNASIUM_CLI_INFO stGymnasiumInfo; //武堂信息
	UINT8            bySkillNum; //技能个数
	DT_SKILL_CLI_INFO astSkillInfo[MAX_GYMNASIUM_SKILL_NUM]; //技能信息
	UINT32           adwSkillSumDoorsTribute[MAX_GYMNASIUM_SKILL_NUM]; //累积门贡要求
}PKT_CLIGS_OPEN_GYMNASIUM_ACK;

typedef struct tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK{
	tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_SKILL_CLI_INFO stSkillInfo; //技能信息
	UINT32           dwLeftDoorsTribute; //玩家剩余门贡
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	UINT32           dwSumDoorsTribute; //累积门贡要求
}PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK;

typedef struct tagPKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK{
	tagPKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_UPGRADE_REQUIRE_INFO_CLI stUpgradeRequireInfo; //升级要求数据
}PKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK;

typedef struct tagPKT_CLIGS_GYMNASIUM_UPGRADE_ACK{
	tagPKT_CLIGS_GYMNASIUM_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GYMNASIUM_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	UINT8            bySkillNum; //技能个数
	DT_SKILL_CLI_INFO astAfterUpgradeInfo[MAX_GYMNASIUM_SKILL_NUM]; //升级后的技能信息
	DT_UPGRADE_REQUIRE_INFO_CLI stUpgradeRequireInfo; //下一级升级要求
	UINT8            byFullLevelMark; //满级标识
	UINT8            byGymnasiumLevel; //升级后的武堂等级
	UINT32           dwFactionFunds; //资金同步
	UINT32           adwSkillSumDoorsTribute[MAX_GYMNASIUM_SKILL_NUM]; //累积门贡要求
}PKT_CLIGS_GYMNASIUM_UPGRADE_ACK;

typedef struct tagPKT_CLIGS_OPEN_SHADE_REQ{
	tagPKT_CLIGS_OPEN_SHADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SHADE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_SHADE_REQ;

typedef struct tagDT_SHADE_INFO{
	UINT32           dwShadeLevel; //暗部等级
	UINT32           dwDoorsTribute; //门贡
	UINT64           qwGold; //元宝
	UINT64           qwCoin; //铜钱
	UINT8            byUpgradeLimit; //升级权限
	UINT8            byPlayerVIPLevel; //玩家VIP等级
}DT_SHADE_INFO;

typedef struct tagDT_HUNTING_POT_INFO{
	TCHAR            aszPotName[HUNTING_POT_NAME]; //壶的名称
	UINT32           dwCurIntegral; //当前积分
	UINT32           dwMaxintegral; //积分上限
	UINT32           dwRewardFunds; //奖励的门派资金
}DT_HUNTING_POT_INFO;

typedef struct tagDT_HUNTING_POT_INFO2{
	TCHAR            aszPotName[HUNTING_POT_NAME]; //壶的名称
	UINT32           dwCurIntegral; //当前积分
	UINT32           dwMaxintegral; //积分上限
	UINT32           dwRewardFunds; //奖励的门派资金
	UINT8            byIsUnlock; //是否解锁(0未解锁， 1解锁)
	UINT8            byUnLockLevel; //解锁等级
}DT_HUNTING_POT_INFO2;

typedef struct tagDT_REWARD_SINGLE_INFO{
	UINT8            byGoodsType; //奖励类型
	UINT32           dwRewardValue; //奖励值
	UINT8            byOutProbability; //出现概率
	UINT8            byMulti; //倍数 0为正常，1为加倍
}DT_REWARD_SINGLE_INFO;

typedef struct tagDT_HUNTING_SCHEDULE_INFO{
	UINT8            byHuntingPotPos; //当前捉妖壶位置
	UINT8            byHuntingPotNum; //捉妖壶个数
	DT_HUNTING_POT_INFO astHuntingPotInfo[MAX_HUTING_POT_NUM]; //捉妖壶信息
	UINT32           dwFactionFunds; //门派资金
	UINT32           dwDoorsTribute; //个人剩余门贡
}DT_HUNTING_SCHEDULE_INFO;

typedef struct tagDT_HUNTING_SCHEDULE_INFO2{
	UINT8            byHuntingPotPos; //当前捉妖壶位置
	UINT8            byHuntingPotNum; //捉妖壶个数
	DT_HUNTING_POT_INFO2 astHuntingPotInfo[MAX_HUNTING_POT_NUM_NEW]; //捉妖壶信息
	UINT32           dwFactionFunds; //门派资金
	UINT32           dwDoorsTribute; //个人剩余门贡
	UINT8            byLastSchedule; //上次档数
}DT_HUNTING_SCHEDULE_INFO2;

typedef struct tagDT_HUNTING_INFO_CLI{
	UINT8            byTaskID; //任务ID
	TCHAR            aszTaskBeginTimeMsg[NORMAL_MSG_LEN]; //任务开启时间提示信息
	TCHAR            aszTaskDescMsg[TASK_DESCRIBE_MSG_LEN]; //任务提示信息
	DT_HUNTING_SCHEDULE_INFO stHuntingScheduleInfo; //捉妖进度信息
	UINT8            byHuntingNum; //捉妖次数信息
	UINT8            byActiviFlag; //活动开始开始标识0 未开启， 1开启
	UINT32           dwHuntingCountDown; //捉妖倒计时
	DT_ACTIVITY_BASE_DATA stActiviInfo; //活动信息
}DT_HUNTING_INFO_CLI;

typedef struct tagPKT_CLIGS_OPEN_SHADE_ACK{
	tagPKT_CLIGS_OPEN_SHADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SHADE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_SHADE_INFO    stShadeInfo; //暗部信息
	DT_HUNTING_INFO_CLI stHuntingInfo; //捉妖模块信息
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
}PKT_CLIGS_OPEN_SHADE_ACK;

typedef struct tagPKT_CLIGS_OPEN_SHADE_UPGRADE_REQ{
	tagPKT_CLIGS_OPEN_SHADE_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SHADE_UPGRADE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_SHADE_UPGRADE_REQ;

typedef struct tagDT_SHADE_UPGRADE_INFO{
	UINT8            byFactionFundsEnough; //升级所需资金是否足够
	UINT32           dwUpgradeNeedFactionFunds; //升级所需资金
	UINT8            byFactionLevelEnough; //升级所需门派等级是否达到
	UINT32           dwUpgradeNeedFactionLevel; //升级所需门派等级
	UINT8            byUpgradeMsgNum; //升级信息个数
	DT_UPGRADE_MSG_LIST astUpgradeMsgLst[SHADE_UPGRADE_MSG_NUM]; //信息数组
	UINT8            byShadelevel; //升级前的暗部等级
	UINT8            byIsUpgradeLimit; //升级权限0没有升级权限， 1有升级权限
}DT_SHADE_UPGRADE_INFO;

typedef struct tagPKT_CLIGS_OPEN_SHADE_UPGRADE_ACK{
	tagPKT_CLIGS_OPEN_SHADE_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SHADE_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_SHADE_UPGRADE_INFO stUpgradeInfo; //升级所需求的信息
}PKT_CLIGS_OPEN_SHADE_UPGRADE_ACK;

typedef struct tagPKT_CLIGS_SHADE_UPGRADE_REQ{
	tagPKT_CLIGS_SHADE_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SHADE_UPGRADE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_SHADE_UPGRADE_REQ;

typedef struct tagPKT_CLIGS_SHADE_UPGRADE_ACK{
	tagPKT_CLIGS_SHADE_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SHADE_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_HUNTING_INFO_CLI stAfterUpgradeHuntingInfo; //升级后的捉妖信息
	DT_SHADE_UPGRADE_INFO stNextUpgradeInfo; //下一级升级的信息
	UINT8            byFullLevelMark; //满级标识
	UINT8            byShadeLevel; //升级后的暗部等级
	UINT32           dwFactionFunds; //资金同步
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
}PKT_CLIGS_SHADE_UPGRADE_ACK;

typedef struct tagDT_REWARD_INFO{
	UINT8            byRewardType; //奖励品质类型 commondef.h  EColor中
	UINT8            byRewardNum; //奖励个数
	DT_REWARD_SINGLE_INFO astRewardMsg[HUNTING_REWARD_MGS_NUM]; //奖励信息
}DT_REWARD_INFO;

typedef struct tagDT_MONSTER_INFO{
	UINT16           wMonsterID; //怪物ID
	UINT32           dwMonsterPower; //怪物战力
	DT_REWARD_INFO   stMonsterReward; //怪物奖励
}DT_MONSTER_INFO;

typedef struct tagPKT_CLIGS_OPEN_GOLDMODE_REQ{
	tagPKT_CLIGS_OPEN_GOLDMODE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GOLDMODE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_GOLDMODE_REQ;

typedef struct tagPKT_CLIGS_OPEN_GOLDMODE_ACK{
	tagPKT_CLIGS_OPEN_GOLDMODE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GOLDMODE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_MONSTER_INFO  stCurMonsterInfo; //开启后的信息
	UINT32           dwSyncGold; //同步元宝数据
}PKT_CLIGS_OPEN_GOLDMODE_ACK;

typedef struct tagPKT_CLIGS_OPEN_HUNTING_REQ{
	tagPKT_CLIGS_OPEN_HUNTING_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HUNTING_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_HUNTING_REQ;

typedef struct tagDT_REWARD_INFO_ALL{
	UINT8            byTypeNum; //奖励品质个数
	DT_REWARD_INFO   astRewardInfo[MONSTER_TYPE_NUM]; //奖励信息
}DT_REWARD_INFO_ALL;

typedef struct tagDT_CHANGE_MONSTER_INFO{
	UINT8            byMaxFreeNum; //免费上限
	UINT8            byCurFreeNum; //剩余免费次数
	UINT32           dwChangeMonsterGold; //换怪所需元宝
}DT_CHANGE_MONSTER_INFO;

typedef struct tagDT_FIND_MONSTER_INFO{
	UINT8            byPlayerVIPLevel; //玩家VIP等级
	UINT8            byNeedVIPLevel; //需要VIP等级
	UINT32           dwNeedGold; //需要元宝
}DT_FIND_MONSTER_INFO;

typedef struct tagDT_HUNTING_OTHER_INFO{
	UINT64           qwGold; //元宝
	UINT32           dwDoorsTribute; //门贡
	UINT8            byCanHuntingNum; //今日可捉跃次数信息
	UINT32           dwHuntingTime; //捉妖时间
	DT_CHANGE_MONSTER_INFO stChangeMonsterMsg; //免费次数
	DT_FIND_MONSTER_INFO stFindMonsterMsg; //筋斗云使用说明
	UINT32           dwMaxTaskIntegral; //集体积分上限
	UINT32           dwCurTaskIntegral; //当前积分
	TCHAR            aszCurHuntingPotName[HUNTING_POT_NAME]; //当前炼妖壶名称
	UINT8            byCurPotNum; //当前捉妖壶位置
	UINT8            bySumPotNum; //总捉妖壶个数
}DT_HUNTING_OTHER_INFO;

typedef struct tagPKT_CLIGS_OPEN_HUNTING_ACK{
	tagPKT_CLIGS_OPEN_HUNTING_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HUNTING_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_REWARD_INFO_ALL stRewardInfoAll; //奖励信息
	DT_MONSTER_INFO  stMonsterInfo; //当前妖怪信息
	DT_HUNTING_OTHER_INFO stOtherInfo; //其它数据信息
	UINT8            byMonsterExist; //妖怪存在标识0 没有， 1 为有上次未打死妖怪
	UINT8            byOpenGoldFlag; //福神显灵开启状态 0未开启， 1 已开启
	UINT8            byVIPLevel; //开启福神显灵需要的VIP
	UINT32           dwNeedGold; //开启福神显灵需要的元宝
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //进度信息
}PKT_CLIGS_OPEN_HUNTING_ACK;

typedef struct tagPKT_CLIGS_FIND_MONSTER_REQ{
	tagPKT_CLIGS_FIND_MONSTER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FIND_MONSTER_REQ)); }
	UINT8            byFindType; //寻找类型（1 开始寻找， 2 千里眼 3 更换怪物）
	UINT8            byMonsterNum; //组中怪物的个数
	UINT16           awMonsterBlue[MONSTER_GROUP_NUM]; //蓝色怪物
	UINT16           awMonsterPurple[MONSTER_GROUP_NUM]; //紫色怪物
	UINT16           awMonsterRed[MONSTER_GROUP_NUM]; //红色怪物
	UINT16           awMonsterOrange[MONSTER_GROUP_NUM]; //橙色怪物
}PKT_CLIGS_FIND_MONSTER_REQ;

typedef struct tagPKT_CLIGS_FIND_MONSTER_ACK{
	tagPKT_CLIGS_FIND_MONSTER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FIND_MONSTER_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_CHANGE_MONSTER_INFO stFreeMsg; //免费提示信息
	UINT32           dwGoldNum; //同步元宝
	DT_MONSTER_INFO  stMonsterInfo; //怪物信息
	UINT8            byFindType; //寻找类型（1 开始寻找， 2 千里眼 3 更换怪物）
	UINT8            byGoldModeFlag; //是否处于黄金模式（0 普通状态 1 黄金模式）
}PKT_CLIGS_FIND_MONSTER_ACK;

typedef struct tagPKT_CLIGS_HUNTING_BATTLE_REQ{
	tagPKT_CLIGS_HUNTING_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_HUNTING_BATTLE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_HUNTING_BATTLE_REQ;

typedef struct tagPKT_CLIGS_HUNTING_BATTLE_ACK2{
	tagPKT_CLIGS_HUNTING_BATTLE_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_HUNTING_BATTLE_ACK2)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_BATTLE_DATA3  stBattleInfo; //战斗信息
	DT_REWARD_INFO   stBattleReward; //战斗奖励
	DT_HUNTING_SCHEDULE_INFO stTaskScheduleInfo; //任务进度信息
	UINT32           dwClosedMonsterNeedGold; //收妖需要的元宝
	TCHAR            aszBattleLostMsg[MAX_FACTION_DESC_NUM]; //战斗失败信息
	UINT32           dwSyncDoorsTribute; //同步数据
	UINT8            byScheduleChangeFlag; //档数改变标识（0 没变， 1变）
	UINT8            byHuntingLeftNum; //捉妖次数同步
	UINT8            byIsGoldMode; //是否处于黄金模式
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
}PKT_CLIGS_HUNTING_BATTLE_ACK2;

typedef struct tagPKT_CLIGS_CHANGE_SCHEDEULE_NTF{
	tagPKT_CLIGS_CHANGE_SCHEDEULE_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_SCHEDEULE_NTF)); }
	UINT8            byTaskID; //任务ID
	UINT64           qwNTFTime; //通知时间
	DT_HUNTING_SCHEDULE_INFO stScheduleInfo; //当前档数
	DT_REWARD_INFO   stBattleReward; //换档奖励
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
}PKT_CLIGS_CHANGE_SCHEDEULE_NTF;

typedef struct tagPKT_CLIGS_OPEN_HOT_SPRING_REQ{
	tagPKT_CLIGS_OPEN_HOT_SPRING_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HOT_SPRING_REQ)); }
	UINT16           wPosX; //位置X
	UINT16           wPosY; //位置Y
}PKT_CLIGS_OPEN_HOT_SPRING_REQ;

typedef struct tagDT_HOT_SPRING_PLAYER_DATA_CLI{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //昵称
	UINT16           wHeroKindID; //主武将类型
	UINT16           wLevel; //等级
	UINT16           wPosX; //位置信息X
	UINT16           wPosY; //位置信息Y
	UINT8            byTuhaoGoldMark; //土豪金标识位
}DT_HOT_SPRING_PLAYER_DATA_CLI;

typedef struct tagDT_HOT_SPRING_PLAYER_DATA_CLI_LST{
	UINT32           dwPlayerNum; //人数
	DT_HOT_SPRING_PLAYER_DATA_CLI astHotSpringPlayerLst[MAX_HOT_SPRING_SHOW_PLAYER_NUM]; //温泉玩家信息列表
}DT_HOT_SPRING_PLAYER_DATA_CLI_LST;

typedef struct tagDT_HOT_SPRING_SELF_DATA_CLI{
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurSkill; //当前功力（技能）
	UINT64           qwCurPhystrength; //当前体力
	UINT16           wPosY; //位置信息Y
	UINT16           wPosX; //位置信息X
	UINT64           qwMaxPhystrength; //体力上限
	UINT8            byGiveWineLimit; //赐酒权限 0 没有权限， 1 有权限
}DT_HOT_SPRING_SELF_DATA_CLI;

typedef struct tagDT_DIDNOT_GIVE_WINE_CLI{
	UINT8            byLeftGiveWineNum; //今日剩余赐酒次数
	UINT32           dwGiveWineCost; //赐酒消耗
	UINT32           dwGiveWineCountDown; //赐酒冷却倒计时
	UINT32           dwGiveWineCDValue; //赐酒CD
}DT_DIDNOT_GIVE_WINE_CLI;

typedef struct tagDT_DIDNOT_DRINGK_WINE_LIST_CLI{
	UINT8            byWineNum; //酒的数量
	DT_WINE_DATA_CLI astAllWineInfo[HOT_SPRING_WINE_NUM]; //三种酒
	TCHAR            aszExtMsg[WINE_MSG_LEN]; //提示信息
}DT_DIDNOT_DRINGK_WINE_LIST_CLI;

typedef struct tagDT_HOTSPRING_ACTIVITY_TIME_DATA{
	UINT8            byStartHour; //活动开启时间，小时
	UINT8            byStartMin; //活动开启时间，分钟
	UINT16           wKeepTime; //活动持续时间
}DT_HOTSPRING_ACTIVITY_TIME_DATA;

typedef struct tagDT_HOT_SPRING_EXPERIENCE{
	UINT32           dwMaxExperience; //可领取经验上限
	UINT32           dwCurExperience; //当前可领取经验
	UINT32           dwExperienceBase; //经验基数
	UINT32           dwAddInterval; //经验增加间隔时间
}DT_HOT_SPRING_EXPERIENCE;

typedef struct tagPKT_CLIGS_OPEN_HOT_SPRING_ACK{
	tagPKT_CLIGS_OPEN_HOT_SPRING_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HOT_SPRING_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_HOT_SPRING_PLAYER_DATA_CLI_LST stPlayerInfo; //玩家列表
	DT_HOT_SPRING_SELF_DATA_CLI stPlayerSelf; //自己信息
	UINT8            byBeforeWineState; //找回状态
	UINT8            byGiveWineState; //打开赐酒状态
	DT_WINE_DATA_CLI stWineDataCli; //酒的信息
	UINT64           qwActiviTime; //赐酒倒计时
	UINT8            byHaveOverReward; //是否有过期奖励
	DT_OVER_GIVE_WINE_REWARD stOverReward; //过期奖励
	DT_HOT_SPRING_EXPERIENCE stPlayerExprience; //英雄经验
}PKT_CLIGS_OPEN_HOT_SPRING_ACK;

typedef struct tagPKT_CLIGS_SELECT_WINE_REQ{
	tagPKT_CLIGS_SELECT_WINE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SELECT_WINE_REQ)); }
	UINT32           dwWineID; //酒的ID
}PKT_CLIGS_SELECT_WINE_REQ;

typedef struct tagPKT_CLIGS_SELECT_WINE_ACK{
	tagPKT_CLIGS_SELECT_WINE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SELECT_WINE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwGold; //元宝
	UINT64           qwCoin; //铜钱
	DT_WINE_DATA_CLI stWineDataCli; //酒的信息
	UINT8            bySelectWineState; //选酒状态0 为普通饮酒  1 为找回饮酒
}PKT_CLIGS_SELECT_WINE_ACK;

typedef struct tagPKT_CLIGS_DRINK_WINE_REQ{
	tagPKT_CLIGS_DRINK_WINE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRINK_WINE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_DRINK_WINE_REQ;

typedef struct tagPKT_CLIGS_DRINK_WINE_ACK{
	tagPKT_CLIGS_DRINK_WINE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRINK_WINE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_WINE_DATA_CLI stWineDataCli; //酒的信息
	UINT8            byRewardID1; //奖励ID1
	UINT32           dwSyncValue1; //同步奖励1
	UINT8            byRewardID2; //奖励ID2
	UINT32           dwSyncValue2; //同步奖励2
}PKT_CLIGS_DRINK_WINE_ACK;

typedef struct tagPKT_CLIGS_DRINK_WINE_MOVE_REQ{
	tagPKT_CLIGS_DRINK_WINE_MOVE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRINK_WINE_MOVE_REQ)); }
	UINT16           wPosX; //位置x
	UINT16           wPosY; //位置y
}PKT_CLIGS_DRINK_WINE_MOVE_REQ;

typedef struct tagPKT_CLIGS_DRINK_WINE_MOVE_ACK{
	tagPKT_CLIGS_DRINK_WINE_MOVE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRINK_WINE_MOVE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_DRINK_WINE_MOVE_ACK;

typedef struct tagPKT_CLIGS_LEAVE_HOT_SPRING_REQ{
	tagPKT_CLIGS_LEAVE_HOT_SPRING_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_HOT_SPRING_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_LEAVE_HOT_SPRING_REQ;

typedef struct tagPKT_CLIGS_LEAVE_HOT_SPRING_ACK{
	tagPKT_CLIGS_LEAVE_HOT_SPRING_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_HOT_SPRING_ACK)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_LEAVE_HOT_SPRING_ACK;

typedef struct tagPKT_CLIGS_HOT_SPRING_POS_NTF{
	tagPKT_CLIGS_HOT_SPRING_POS_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_HOT_SPRING_POS_NTF)); }
	UINT8            byEnterPalyerNum; //新增人数
	DT_HOT_SPRING_PLAYER_DATA_CLI astEnterPlayerInfo[MAX_HOT_SPRING_SHOW_PLAYER_NUM]; //新增人员信息
	UINT8            byLeavePlayerNum; //删除人数
	DT_HOT_SPRING_PLAYER_DATA_CLI astLeavePlayerInfo[MAX_HOT_SPRING_SHOW_PLAYER_NUM]; //删除人员信息
	UINT8            byMovePlayerNum; //移动人员
	DT_HOT_SPRING_PLAYER_DATA_CLI astMovePlayerInfo[MAX_HOT_SPRING_SHOW_PLAYER_NUM]; //移动人员信息
}PKT_CLIGS_HOT_SPRING_POS_NTF;

typedef struct tagPKT_CLIGS_CHANGE_OTHER_REQ{
	tagPKT_CLIGS_CHANGE_OTHER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_OTHER_REQ)); }
	UINT32           dwSendPlayerID; //战力变化信息
}PKT_CLIGS_CHANGE_OTHER_REQ;

typedef struct tagPKT_CLIGS_CHANGE_OTHER_ACK{
	tagPKT_CLIGS_CHANGE_OTHER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_OTHER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_PLAYER_CLI_LST stFactionPlayerDataLst; //门派玩家信息
	DT_FACTION_AUTHORITY_EX stFactionAuthority; //门派权限信息
	DT_DOORSTRIBUTE_LEVE_DATA_LST stDoorsTributeLst; //贡献度列表
}PKT_CLIGS_CHANGE_OTHER_ACK;

typedef struct tagPKT_CLIGS_GIVE_WINE_REQ{
	tagPKT_CLIGS_GIVE_WINE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GIVE_WINE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_GIVE_WINE_REQ;

typedef struct tagPKT_CLIGS_GIVE_WINE_ACK{
	tagPKT_CLIGS_GIVE_WINE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GIVE_WINE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_DIDNOT_GIVE_WINE_CLI stGiveWineInfo; //赐酒信息
}PKT_CLIGS_GIVE_WINE_ACK;

typedef struct tagPKT_CLIGS_CLICK_SELECT_WINE_REQ{
	tagPKT_CLIGS_CLICK_SELECT_WINE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLICK_SELECT_WINE_REQ)); }
	UINT8            byDrinkWIneState; //饮酒状态 0 饮酒 1 找回
}PKT_CLIGS_CLICK_SELECT_WINE_REQ;

typedef struct tagPKT_CLIGS_CLICK_SELECT_WINE_ACK{
	tagPKT_CLIGS_CLICK_SELECT_WINE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLICK_SELECT_WINE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_DIDNOT_DRINGK_WINE_LIST_CLI stWineDataList; //酒的信息
}PKT_CLIGS_CLICK_SELECT_WINE_ACK;

typedef struct tagPKT_CLIGS_SURE_GIVE_WINE_REQ{
	tagPKT_CLIGS_SURE_GIVE_WINE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SURE_GIVE_WINE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_SURE_GIVE_WINE_REQ;

typedef struct tagPKT_CLIGS_SURE_GIVE_WINE_ACK{
	tagPKT_CLIGS_SURE_GIVE_WINE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SURE_GIVE_WINE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwActiviTime; //活动倒计时
}PKT_CLIGS_SURE_GIVE_WINE_ACK;

typedef struct tagPKT_CLIGS_CLOSED_MONSTER_REQ{
	tagPKT_CLIGS_CLOSED_MONSTER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLOSED_MONSTER_REQ)); }
	UINT8            byChoice; //收妖选择 0不收妖， 1收妖
}PKT_CLIGS_CLOSED_MONSTER_REQ;

typedef struct tagPKT_CLIGS_CLOSED_MONSTER_ACK{
	tagPKT_CLIGS_CLOSED_MONSTER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLOSED_MONSTER_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	UINT8            byRewardNum; //奖励个数
	DT_REWARD_SINGLE_INFO astRewardMsg[HUNTING_REWARD_MGS_NUM]; //奖励信息
	UINT32           dwSyncDoorsTribute; //同步数据
	DT_HUNTING_SCHEDULE_INFO stTaskScheduleInfo; //任务进度信息
	UINT8            byScheduleChangeFlag; //档数改变标识（0 没变， 1变）
	UINT8            byHuntingLeftNum; //捉妖次数同步
	UINT8            byCloseMonsterChoice; //收妖选择
	UINT64           qwSyncGold; //元宝同步
	UINT8            byIsGoldMode; //是否处于黄金模式0不开启状态， 1开启
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
}PKT_CLIGS_CLOSED_MONSTER_ACK;

typedef struct tagPKT_CLIGS_FACTION_PLAYER_CHANGE_REQ{
	tagPKT_CLIGS_FACTION_PLAYER_CHANGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_PLAYER_CHANGE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_FACTION_PLAYER_CHANGE_REQ;

typedef struct tagPKT_CLIGS_FACTION_PLAYER_CHANGE_ACK{
	tagPKT_CLIGS_FACTION_PLAYER_CHANGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_PLAYER_CHANGE_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byChangeFlag; //门派变动标识   0 没有变动   1 有变动
}PKT_CLIGS_FACTION_PLAYER_CHANGE_ACK;

typedef struct tagPKT_CLIGS_GIVE_WINE_ACTIVITY_NTF{
	tagPKT_CLIGS_GIVE_WINE_ACTIVITY_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_GIVE_WINE_ACTIVITY_NTF)); }
	UINT64           qwCountDownTime; //倒计时
}PKT_CLIGS_GIVE_WINE_ACTIVITY_NTF;

typedef struct tagPKT_CLIGS_LEAVE_FACTION_REQ{
	tagPKT_CLIGS_LEAVE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_LEAVE_FACTION_REQ;

typedef struct tagPKT_CLIGS_LEAVE_FACTION_ACK{
	tagPKT_CLIGS_LEAVE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIGS_LEAVE_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_LEAVE_FACTION_REQ{
	tagPKT_CLIGS_OPEN_LEAVE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LEAVE_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_LEAVE_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_LEAVE_FACTION_ACK{
	tagPKT_CLIGS_OPEN_LEAVE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LEAVE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码
	UINT32           dwIntervalTime; //不能加入门派时间，单位（秒）
	UINT8            byLeaveFactionMark; //标识位（0退出门派 1 解散门派）
}PKT_CLIGS_OPEN_LEAVE_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_FACTION_UPGRADE_REQ{
	tagPKT_CLIGS_OPEN_FACTION_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_UPGRADE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_FACTION_UPGRADE_REQ;

typedef struct tagDT_FACTION_UPGRADE_ALL_INFO{
	UINT32           dwNeedFactionFunds; //要求门派资金
	UINT8            byFunds_Is_Enough; //门派资金是否足够 0 不够 1 足够
	UINT8            byUpgradeNum; //升级信息个数
	DT_UPGRADE_MSG_LIST astUpgradeInfo[MAX_FACTION_UPGRADE_NUM]; //升级信息
	UINT8            byFactionLevel; //升级前的门派等级
	UINT8            byIsUpgradeLimit; //是否有权限0 没有升级权限， 1有升级权限
}DT_FACTION_UPGRADE_ALL_INFO;

typedef struct tagPKT_CLIGS_OPEN_FACTION_UPGRADE_ACK{
	tagPKT_CLIGS_OPEN_FACTION_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_FACTION_UPGRADE_ALL_INFO stUpgradeAllInfo; //升级要求信息
}PKT_CLIGS_OPEN_FACTION_UPGRADE_ACK;

typedef struct tagDT_FACTION_BUILD_LEVEL{
	UINT8            byRareitemLevel; //珍品阁（0解锁， 非0为解锁等级）
	UINT8            byGymnasiumLevel; //武堂
	UINT8            byShadeLevel; //暗部
	UINT8            byHotSpringLevel; //温泉
	UINT8            byColdPrisonLevel; //冰牢
	UINT8            byFactionTowerLevel; //昊天塔
}DT_FACTION_BUILD_LEVEL;

typedef struct tagPKT_CLIGS_FACTION_UPGRADE_REQ{
	tagPKT_CLIGS_FACTION_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_UPGRADE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_FACTION_UPGRADE_REQ;

typedef struct tagDT_FACTION_SYNC_INFO{
	UINT32           dwFactionFunds; //门派资金同步
	UINT32           dwFactionMaxNum; //人数上限同步
	UINT8            byRareitemFlag; //珍品阁开启状态
	UINT8            byGymnasiumFlag; //武堂开启状态
	UINT8            byShadeFlag; //暗部开启状态
	UINT8            byHotFlag; //温泉开启状态
	UINT8            byColdPrisonFlag; //冰牢开启状态
	UINT8            byFactionTowerFlag; //昊天塔开启状态
	UINT8            byFactionRank; //门派当前排名
}DT_FACTION_SYNC_INFO;

typedef struct tagPKT_CLIGS_FACTION_UPGRADE_ACK{
	tagPKT_CLIGS_FACTION_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_FACTION_SYNC_INFO stSyncInfo; //升级后的同步数据
	UINT8            byIsMaxLevel; //是否达到上限
	DT_FACTION_UPGRADE_ALL_INFO stNextUpgradeAllInfo; //下一级升级效果
	UINT8            byFactionLevel; //升级后的门派等级
	UINT32           dwFactionFunds; //资金同步
	DT_FACTION_BUILD_LEVEL stCurBuildLevel; //当前建筑群等级
}PKT_CLIGS_FACTION_UPGRADE_ACK;

typedef struct tagDT_EQUIP_MASTER_DETAIL_CLI{
	UINT8            byEquipKind; //精通装备类型（头盔、战袍、武器、战靴）
	UINT8            byMasterRank; //精通阶级（未激活0、初1、中2、高3、极4）
	UINT16           wMasterLevel; //精通等级
	UINT32           dwCurMasterExp; //装备精通当前经验
	UINT32           dwMaxMasterExp; //装备精通最大经验
	UINT16           wCurPromote; //当前属性加成
	UINT16           wNextPromote; //下一级属性加成
	UINT8            byActivateOrBreak; //当前状态：可升级(0)、可激活(1)、可突破(2)、无法再提升(3)、可突破但等级不足(4)
	UINT32           dwMedalCost; //勇气勋章消耗
	UINT16           wRequiredPlayerLevel; //突破所需的玩家等级
}DT_EQUIP_MASTER_DETAIL_CLI;

typedef struct tagDT_MASTER_EXP_BOOK_DETAIL_CLI{
	UINT8            byBigExpBookCnt; //有几本大经验书（0本、1本、2本、3本）
}DT_MASTER_EXP_BOOK_DETAIL_CLI;

typedef struct tagDT_MASTER_CHEAT_INFO_CLI{
	UINT16           wCheatGold; //出千所需元宝
	UINT8            byFreeCheatCnt; //还可以免费出千几次
}DT_MASTER_CHEAT_INFO_CLI;

typedef struct tagDT_MASTER_EXP_BOOK_PANEL_CLI{
	DT_MASTER_EXP_BOOK_DETAIL_CLI astExpBooks[MASTER_EXP_BOOK_COUNT]; //面板上排列的经验书
	UINT16           wBigBookExp; //每本大经验书加多少经验
	UINT16           wSmallBookExp; //每本小经验书加多少经验
	UINT8            byIsOpenUpCheat; //出千是否已开放：已开放1、未开放0
	DT_MASTER_CHEAT_INFO_CLI stCheatInfo; //出千信息
}DT_MASTER_EXP_BOOK_PANEL_CLI;

typedef struct tagDT_MASTER_LEVEL_UP{
	UINT32           dwMaxMasterExp; //该等级的最大经验
	UINT16           wPromote; //该等级的属性加成
	UINT16           wNextPromote; //下一级时的属性加成
}DT_MASTER_LEVEL_UP;

typedef struct tagDT_MASTER_LEVEL_UP_INFO{
	UINT8            byLevelDiff; //中间隔了几级，比如从1级升到3级，则只隔了一级
	DT_MASTER_LEVEL_UP astLevelUp[MASTER_MAX_LEVEL_UP]; //中间的升级过程
	UINT16           wPreLevel; //升级前的等级
	UINT16           wUpExp; //本次加了多少经验
}DT_MASTER_LEVEL_UP_INFO;

typedef struct tagPKT_CLIGS_OPEN_EQUIP_MASTER_REQ{
	tagPKT_CLIGS_OPEN_EQUIP_MASTER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EQUIP_MASTER_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_EQUIP_MASTER_REQ;

typedef struct tagPKT_CLIGS_OPEN_EQUIP_MASTER_ACK{
	tagPKT_CLIGS_OPEN_EQUIP_MASTER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EQUIP_MASTER_ACK)); }
	UINT16           wErrCode; //错误码
	DT_EQUIP_MASTER_DETAIL_CLI astEquipMasterDetail[EQUIP_KIND_COUNT]; //面板上4种装备的精通信息
	UINT32           dwGetBookMedalCost; //培养精通之书的勋章消耗
	UINT8            byExpBookPanelState; //培养精通之书面板的显示状态：不显示0、显示但未刮开1、已刮开但未确认2
	DT_MASTER_EXP_BOOK_PANEL_CLI stExpBookPanelData; //如果上次没刮完，这次显示出面板让玩家接着刮
	UINT64           qwCoin; //当前铜钱
	UINT32           dwGold; //当前元宝
	UINT32           dwMedal; //当前勇气勋章
}PKT_CLIGS_OPEN_EQUIP_MASTER_ACK;

typedef struct tagPKT_CLIGS_ACTIVATE_MASTER_REQ{
	tagPKT_CLIGS_ACTIVATE_MASTER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ACTIVATE_MASTER_REQ)); }
	UINT8            byEquipKind; //装备类型（头盔、战袍、武器、战靴）
}PKT_CLIGS_ACTIVATE_MASTER_REQ;

typedef struct tagPKT_CLIGS_ACTIVATE_MASTER_ACK{
	tagPKT_CLIGS_ACTIVATE_MASTER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ACTIVATE_MASTER_ACK)); }
	UINT16           wErrCode; //错误码
	DT_EQUIP_MASTER_DETAIL_CLI stEquipMasterInfo; //装备的精通信息
	UINT32           dwMedal; //当前勇气勋章
	UINT32           dwOldPower; //原始战力
	UINT32           dwCurPower; //当前战力
	UINT8            byCanGetMasterBook; //是否可以培养精通之书：是1、否0
}PKT_CLIGS_ACTIVATE_MASTER_ACK;

typedef struct tagPKT_CLIGS_BREAK_MASTER_REQ{
	tagPKT_CLIGS_BREAK_MASTER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BREAK_MASTER_REQ)); }
	UINT8            byEquipKind; //装备类型（头盔、战袍、武器、战靴）
}PKT_CLIGS_BREAK_MASTER_REQ;

typedef struct tagPKT_CLIGS_BREAK_MASTER_ACK{
	tagPKT_CLIGS_BREAK_MASTER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BREAK_MASTER_ACK)); }
	UINT16           wErrCode; //错误码
	DT_EQUIP_MASTER_DETAIL_CLI stEquipMasterInfo; //装备的精通信息
	UINT32           dwMedal; //当前勇气勋章
	UINT32           dwOldPower; //原始战力
	UINT32           dwCurPower; //当前战力
}PKT_CLIGS_BREAK_MASTER_ACK;

typedef struct tagPKT_CLIGS_GET_MASTER_BOOK_REQ{
	tagPKT_CLIGS_GET_MASTER_BOOK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_MASTER_BOOK_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_GET_MASTER_BOOK_REQ;

typedef struct tagPKT_CLIGS_GET_MASTER_BOOK_ACK{
	tagPKT_CLIGS_GET_MASTER_BOOK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_MASTER_BOOK_ACK)); }
	UINT16           wErrCode; //错误码
	DT_MASTER_EXP_BOOK_PANEL_CLI stExpBookPanelData; //刮刮乐面板信息
	UINT32           dwNextMedalCost; //下次培养所需消耗的勇气勋章
	UINT32           dwMedal; //当前勇气勋章
}PKT_CLIGS_GET_MASTER_BOOK_ACK;

typedef struct tagPKT_CLIGS_WIPE_MASTER_BOOK_REQ{
	tagPKT_CLIGS_WIPE_MASTER_BOOK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_WIPE_MASTER_BOOK_REQ)); }
	DT_MASTER_EXP_BOOK_PANEL_CLI stExpBookPanelData; //刮刮乐面板信息
}PKT_CLIGS_WIPE_MASTER_BOOK_REQ;

typedef struct tagPKT_CLIGS_WIPE_MASTER_BOOK_ACK{
	tagPKT_CLIGS_WIPE_MASTER_BOOK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_WIPE_MASTER_BOOK_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIGS_WIPE_MASTER_BOOK_ACK;

typedef struct tagPKT_CLIGS_MASTER_CHEAT_REQ{
	tagPKT_CLIGS_MASTER_CHEAT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MASTER_CHEAT_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_MASTER_CHEAT_REQ;

typedef struct tagPKT_CLIGS_MASTER_CHEAT_ACK{
	tagPKT_CLIGS_MASTER_CHEAT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MASTER_CHEAT_ACK)); }
	UINT16           wErrCode; //错误码
	DT_MASTER_EXP_BOOK_DETAIL_CLI astExpBooks[EQUIP_KIND_COUNT]; //面板上排列的经验书
	UINT32           dwGold; //当前元宝
	DT_MASTER_CHEAT_INFO_CLI stCheatInfo; //下次出千信息
}PKT_CLIGS_MASTER_CHEAT_ACK;

typedef struct tagPKT_CLIGS_CONFIRM_MASTER_BOOK_REQ{
	tagPKT_CLIGS_CONFIRM_MASTER_BOOK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CONFIRM_MASTER_BOOK_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_CONFIRM_MASTER_BOOK_REQ;

typedef struct tagPKT_CLIGS_CONFIRM_MASTER_BOOK_ACK{
	tagPKT_CLIGS_CONFIRM_MASTER_BOOK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CONFIRM_MASTER_BOOK_ACK)); }
	UINT16           wErrCode; //错误码
	DT_EQUIP_MASTER_DETAIL_CLI astEquipMasterDetail[EQUIP_KIND_COUNT]; //面板上4种装备的精通信息
	DT_MASTER_LEVEL_UP_INFO astLevelUpInfo[EQUIP_KIND_COUNT]; //面板上4种装备的升级过程
	UINT16           wCurPlayerLevel; //当前玩家等级
	UINT32           dwOldPower; //原始战力
	UINT32           dwCurPower; //当前战力
}PKT_CLIGS_CONFIRM_MASTER_BOOK_ACK;

typedef struct tagPKT_CLIGS_OPEN_BAD_REWARD_REQ{
	tagPKT_CLIGS_OPEN_BAD_REWARD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BAD_REWARD_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_BAD_REWARD_REQ;

typedef struct tagPKT_CLIGS_OPEN_BAD_REWARD_ACK{
	tagPKT_CLIGS_OPEN_BAD_REWARD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BAD_REWARD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwNeedCoin; //需要铜钱数
	UINT8            byLeftTimes; //剩余次数
	UINT8            byRewardID1; //奖励ID1
	UINT32           dwRewardValue1; //奖励值1
	UINT8            byRewardID2; //奖励2
	UINT32           dwRewardValue2; //奖励值2
}PKT_CLIGS_OPEN_BAD_REWARD_ACK;

typedef struct tagPKT_CLIGS_GET_BAD_REWARD_REQ{
	tagPKT_CLIGS_GET_BAD_REWARD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_BAD_REWARD_REQ)); }
	UINT8            byExt; //按钮 0 取消 1 确定
}PKT_CLIGS_GET_BAD_REWARD_REQ;

typedef struct tagPKT_CLIGS_GET_BAD_REWARD_ACK{
	tagPKT_CLIGS_GET_BAD_REWARD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_BAD_REWARD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byRewardID1; //奖励ID1
	UINT32           dwRewardValue1; //奖励值1
	UINT32           dwSyncValue1; //同步奖励1
	UINT8            byRewardID2; //奖励ID2
	UINT32           dwRewardValue2; //奖励值2
	UINT32           dwSyncValue2; //同步奖励2
}PKT_CLIGS_GET_BAD_REWARD_ACK;

typedef struct tagDT_COURAGE_INFO_CLI{
	UINT32           dwMaxCourageTimes; //试炼次数上限
	UINT32           dwLeftCourageTimes; //剩余试炼次数
	UINT32           dwHelpCourageTimes; //已协助次数
	TCHAR            aszHelpCouragePrompt[HELP_COURAGE_LEN]; //协助描述
	UINT8            byBattleType; //战斗类型 0：加入战斗， 1：协助战斗
}DT_COURAGE_INFO_CLI;

typedef struct tagDT_COURAGE_GROUP_LEADER_DATA_CLI{
	TCHAR            aszDispPlayerName[USERNAME_LEN]; //玩家昵称
	UINT16           wLevel; //玩家等级
	UINT16           wKindID; //玩家职业ID
}DT_COURAGE_GROUP_LEADER_DATA_CLI;

typedef struct tagDT_COURAGE_GROUP_BASE_DATA_CLI{
	UINT32           dwGroupID; //组ID
	UINT8            byJoinType; //加入类型 0：全部 1：帮派，2：好友
	UINT16           wPlayerMaxNum; //玩家最大个数
	UINT16           wPlayerNum; //当前玩家个数
	DT_COURAGE_GROUP_LEADER_DATA_CLI stLeaderData; //队长信息
	UINT16           wMinLevel; //加入最底等级
	DT_INSTANCE_ID   stInstanceID; //副本序号
}DT_COURAGE_GROUP_BASE_DATA_CLI;

typedef struct tagDT_COURAGE_GROUP_DATA_CLI_LST{
	UINT16           wTeamNum; //队伍数量
	DT_COURAGE_GROUP_BASE_DATA_CLI astTeamInfo[MAX_COURAGE_GROUP_NUM]; //队伍信息
}DT_COURAGE_GROUP_DATA_CLI_LST;

typedef struct tagPKT_CLIGS_OPEN_COURAGE_REQ{
	tagPKT_CLIGS_OPEN_COURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_COURAGE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_COURAGE_REQ;

typedef union tagDT_COURAGE_GROUP_UNION_DATA_CLI{
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //玩家没有队伍
	DT_COURAGE_GROUP_DATA stPlayerGroupData; //玩家有队伍
}DT_COURAGE_GROUP_UNION_DATA_CLI;

typedef struct tagDT_COURAGE_ALL_GROUP_DATA_CLI{
	UINT8            byPlayerGroupType; //队伍信息类型
	DT_COURAGE_GROUP_UNION_DATA_CLI stCourageGroupUnion; //队伍联合数据
}DT_COURAGE_ALL_GROUP_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_COURAGE_ACK{
	tagPKT_CLIGS_OPEN_COURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_COURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwGold; //玩家当前元宝数
	UINT64           qwCopper; //玩家当前铜钱数
	DT_COURAGE_INFO_CLI stPlayerCourageData; //玩家勇气试炼信息
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍列表
	UINT16           wMinLevel; //创建队伍最底等级
	UINT64           qwStory; //阅历
	UINT64           qwMedal; //勇气勋章
	UINT8            bySelect; //筛选条件（em_Join_Type）
}PKT_CLIGS_OPEN_COURAGE_ACK;

typedef struct tagPKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ{
	tagPKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ;

typedef struct tagDT_COURAGE_CHALLENGE_NUM_DATA{
	UINT16           wChallengeGroupNum; //挑战人数
}DT_COURAGE_CHALLENGE_NUM_DATA;

typedef struct tagDT_COURAGE_CHALLENGE_NUM_CLI{
	UINT8            byInstanceNum; //副本数目
	DT_COURAGE_CHALLENGE_NUM_DATA astInstanceChallenge[MAX_INSTANCE_PER_TOWN_NUM]; //副本挑战队伍数目
}DT_COURAGE_CHALLENGE_NUM_CLI;

typedef struct tagPKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK{
	tagPKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_INSTANCE_ID   stMaxSceneData; //最大未解锁副本
	UINT16           wMaxLevel; //最大未解锁副本要求等级
	UINT8            byTownNum; //城镇数
	DT_TOWN_INSTANCE_RECORD_DATA_CLI astTownInstanceInfo[MAX_TOWN_PER_SCENE_NUM]; //城镇副本信息
	UINT16           awChallengeNums[MAX_TOWN_PER_SCENE_NUM]; //副本有多少个队伍在挑战
}PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK;

typedef struct tagPKT_CLIGS_COURAGE_CREATE_GROUP_REQ{
	tagPKT_CLIGS_COURAGE_CREATE_GROUP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_CREATE_GROUP_REQ)); }
	UINT8            byJoinType; //队员加入类型 0：全部 1：帮派，2：好友
	DT_INSTANCE_ID   stSceneData; //选择创建队伍的副本信息
}PKT_CLIGS_COURAGE_CREATE_GROUP_REQ;

typedef struct tagPKT_CLIGS_COURAGE_CREATE_GROUP_ACK{
	tagPKT_CLIGS_COURAGE_CREATE_GROUP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_CREATE_GROUP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwGroupID; //组ID
}PKT_CLIGS_COURAGE_CREATE_GROUP_ACK;

typedef struct tagDT_COURAGE_HERO_DATA{
	UINT16           wKindID; //伙伴类型ID
	UINT16           wLevel; //伙伴等级
	TCHAR            aszDispName[USERNAME_LEN]; //伙伴昵称
}DT_COURAGE_HERO_DATA;

typedef struct tagDT_COURAGE_GROUP_HERO_DATA_CLI{
	UINT16           wHeroNum; //伙伴数量
	DT_COURAGE_HERO_DATA astHeroData[MAX_HERO_NUM]; //伙伴信息
}DT_COURAGE_GROUP_HERO_DATA_CLI;

typedef struct tagPKT_CLIGS_COURAGE_OPERATE_HERO_REQ{
	tagPKT_CLIGS_COURAGE_OPERATE_HERO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_OPERATE_HERO_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_COURAGE_OPERATE_HERO_REQ;

typedef struct tagPKT_CLIGS_COURAGE_OPERATE_HERO_ACK{
	tagPKT_CLIGS_COURAGE_OPERATE_HERO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_OPERATE_HERO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_COURAGE_GROUP_HERO_DATA_CLI stHeroData; //伙伴信息
}PKT_CLIGS_COURAGE_OPERATE_HERO_ACK;

typedef struct tagPKT_CLIGS_COURAGE_CONFIRM_HERO_REQ{
	tagPKT_CLIGS_COURAGE_CONFIRM_HERO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_CONFIRM_HERO_REQ)); }
	UINT16           wKindID; //伙伴类型ID
}PKT_CLIGS_COURAGE_CONFIRM_HERO_REQ;

typedef struct tagPKT_CLIGS_COURAGE_CONFIRM_HERO_ACK{
	tagPKT_CLIGS_COURAGE_CONFIRM_HERO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_CONFIRM_HERO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_COURAGE_HERO_DATA stHeroData; //伙伴信息
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_COURAGE_CONFIRM_HERO_ACK;

typedef struct tagPKT_CLIGS_COURAGE_JOIN_GROUP_REQ{
	tagPKT_CLIGS_COURAGE_JOIN_GROUP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_JOIN_GROUP_REQ)); }
	UINT8            byAddBattleType; //加入战斗方式 0:选择加入 1：随机加入
	UINT32           dwGroupID; //组ID
}PKT_CLIGS_COURAGE_JOIN_GROUP_REQ;

typedef struct tagPKT_CLIGS_COURAGE_JOIN_GROUP_ACK{
	tagPKT_CLIGS_COURAGE_JOIN_GROUP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_JOIN_GROUP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_COURAGE_JOIN_GROUP_ACK;

typedef struct tagPKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ{
	tagPKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ)); }
	UINT32           dwPlayerID; //玩家ID
	UINT32           dwGroupID; //组ID
}PKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ;

typedef struct tagPKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK{
	tagPKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwPlayerID; //被踢队员ID
	UINT16           wAverageLevel; //队伍平均等级
}PKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK;

typedef struct tagPKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ{
	tagPKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ;

typedef struct tagPKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK{
	tagPKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byTotalTimes; //购买后总共次数
	UINT64           qwCurGold; //当前元宝
	UINT8            byCurTimes; //当前次数
	UINT32           dwNextBuyGold; //下次购买所需元宝
	UINT8            byLeftBuyTimes; //剩余购买次数
}PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_READY_REQ{
	tagPKT_CLIGS_COURAGE_PLAYER_READY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_READY_REQ)); }
	UINT8            byExt; //准备状态
}PKT_CLIGS_COURAGE_PLAYER_READY_REQ;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_READY_ACK{
	tagPKT_CLIGS_COURAGE_PLAYER_READY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_READY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byReadyType; //准备类型 0：准备中， 1：已经准备
}PKT_CLIGS_COURAGE_PLAYER_READY_ACK;

typedef struct tagDT_COURAGE_AWARD_DATA{
	UINT64           qwStory; //阅历
	UINT64           qwMedal; //勇气勋章
}DT_COURAGE_AWARD_DATA;

typedef struct tagDT_COURAGE_PLAYERID_HEROID_DATA{
	UINT32           dwPlayID; //玩家ID
	UINT16           wHeroID; //伙伴ID
	UINT16           wKindID; //玩家职业ID
	UINT8            byBattleType; //战斗类型 0：加入战斗， 1：协助战斗
	UINT32           dwExperience; //玩家当前经验
	UINT16           wLevel; //玩家等级
	TCHAR            aszDispPlayerName[USERNAME_LEN]; //玩家昵称
	UINT8            byCurAttackObjIdx; //阵型ID
	UINT32           dwUpgradeExperience; //玩家升级经验
	UINT8            byColor; //玩家颜色
}DT_COURAGE_PLAYERID_HEROID_DATA;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_START_REQ{
	tagPKT_CLIGS_COURAGE_PLAYER_START_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_START_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_COURAGE_PLAYER_START_REQ;

typedef struct tagDT_COURAGE_PLAYER_BATTLE_DATA{
	UINT8            byBattleNum; //战斗次数
	DT_BATTLE_DATA2  astBattleInfo[MAX_COURAGE_MEMBER_NUM]; //战斗信息
}DT_COURAGE_PLAYER_BATTLE_DATA;

typedef struct tagDT_COURAGE_PLAYER_BATTLE_DATA2{
	UINT8            byBattleNum; //战斗次数
	DT_BATTLE_DATA3  astBattleInfo[MAX_COURAGE_MEMBER_NUM]; //战斗信息
}DT_COURAGE_PLAYER_BATTLE_DATA2;

typedef struct tagDT_COURAGE_GROUP_PLAYER_BATTLE_DATA{
	UINT8            byPlayerNum; //玩家数量
	DT_COURAGE_PLAYERID_HEROID_DATA astPlayerInfo[MAX_COURAGE_MEMBER_NUM]; //玩家信息
}DT_COURAGE_GROUP_PLAYER_BATTLE_DATA;

typedef struct tagDT_COURAGE_JEWELRY_DATA_LST{
	UINT16           wJewelryNum; //首饰个数
	DT_JEWELRY_DATA_CLI astJewelryInfo[MAX_BAG_OPEN_NUM]; //饰品信息
}DT_COURAGE_JEWELRY_DATA_LST;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_START_ACK2{
	tagPKT_CLIGS_COURAGE_PLAYER_START_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_START_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_MONSTER_DATA_LIST stMonsterInfo; //怪物信息
	DT_COURAGE_PLAYER_BATTLE_DATA2 stBattleInfoCli; //战斗信息
	DT_COURAGE_GROUP_PLAYER_BATTLE_DATA stGroupPlayerInfo; //队伍信息
	UINT8            byPassFlag; //是否通关 0通关 1不通关
	DT_INSTANCE_ID   stInstanceID; //副本序号
	DT_COURAGE_JEWELRY_DATA_LST stjewelryInfoLst; //饰品信息
	UINT8            byHelpID; //协助描述 1再进行1次， 2协助两次送1次，3已经赠送不能在赠送了
}PKT_CLIGS_COURAGE_PLAYER_START_ACK2;

typedef struct tagPKT_CLIGS_COURAGE_LEADER_START_NTF2{
	tagPKT_CLIGS_COURAGE_LEADER_START_NTF2() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_LEADER_START_NTF2)); }
	PKT_CLIGS_COURAGE_PLAYER_START_ACK2 stBattleInfo; //战斗信息
}PKT_CLIGS_COURAGE_LEADER_START_NTF2;

typedef struct tagPKT_CLIGS_COURAGE_JOIN_GROUP_NTF{
	tagPKT_CLIGS_COURAGE_JOIN_GROUP_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_JOIN_GROUP_NTF)); }
	UINT32           dwAverageLevel; //队伍平均等级
	DT_COURAGE_PLAYER_DATA stPlayerGroupData; //加入队伍的玩家信息
}PKT_CLIGS_COURAGE_JOIN_GROUP_NTF;

typedef struct tagPKT_CLIGS_COURAGE_QUIT_GROUP_NTF{
	tagPKT_CLIGS_COURAGE_QUIT_GROUP_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_QUIT_GROUP_NTF)); }
	UINT16           wAverageLevel; //队伍平均等级
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_COURAGE_QUIT_GROUP_NTF;

typedef struct tagDT_COURAGE_PLAYER_HERO_NTF_DATA{
	UINT32           dwPlayerID; //玩家ID
	UINT32           dwHeroID; //伙伴ID
}DT_COURAGE_PLAYER_HERO_NTF_DATA;

typedef struct tagPKT_CLIGS_COURAGE_OPERATE_HERO_NTF{
	tagPKT_CLIGS_COURAGE_OPERATE_HERO_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_OPERATE_HERO_NTF)); }
	DT_COURAGE_PLAYER_DATA stPlayerGroupData; //更改伙伴的玩家信息
}PKT_CLIGS_COURAGE_OPERATE_HERO_NTF;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_READY_NTF{
	tagPKT_CLIGS_COURAGE_PLAYER_READY_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_READY_NTF)); }
	UINT32           dwPlayerID; //玩家ID
	UINT8            byReadyType; //准备类型 0：已经准备， 1：准备中
}PKT_CLIGS_COURAGE_PLAYER_READY_NTF;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_FIRED_NTF{
	tagPKT_CLIGS_COURAGE_PLAYER_FIRED_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_FIRED_NTF)); }
	UINT8            byErrCode; //被踢错误码
	DT_COURAGE_INFO_CLI stPlayerCourageData; //玩家勇气试炼信息
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍信息列表
}PKT_CLIGS_COURAGE_PLAYER_FIRED_NTF;

typedef struct tagPKT_CLIGS_COURAGE_LEAVE_GROUP_REQ{
	tagPKT_CLIGS_COURAGE_LEAVE_GROUP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_LEAVE_GROUP_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_COURAGE_LEAVE_GROUP_REQ;

typedef struct tagPKT_CLIGS_COURAGE_LEAVE_GROUP_ACK{
	tagPKT_CLIGS_COURAGE_LEAVE_GROUP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_LEAVE_GROUP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwGold; //玩家当前元宝数
	UINT64           qwCopper; //玩家当前铜钱数
	DT_COURAGE_INFO_CLI stPlayerCourageData; //玩家勇气试炼信息
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍信息列表
}PKT_CLIGS_COURAGE_LEAVE_GROUP_ACK;

typedef struct tagPKT_CLIGS_OPEN_COURAGE_SHOP_REQ{
	tagPKT_CLIGS_OPEN_COURAGE_SHOP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_COURAGE_SHOP_REQ)); }
	UINT8            byExt; //扩展字段，暂时未用
}PKT_CLIGS_OPEN_COURAGE_SHOP_REQ;

typedef struct tagPKT_CLIGS_OPEN_COURAGE_SHOP_ACK{
	tagPKT_CLIGS_OPEN_COURAGE_SHOP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_COURAGE_SHOP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byLeftBuyTimes; //剩余购买次数
	UINT32           dwBuyGold; //购买所需元宝
	UINT64           qwCurGold; //当前元宝
}PKT_CLIGS_OPEN_COURAGE_SHOP_ACK;

typedef struct tagPKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF{
	tagPKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF)); }
	UINT8            byUnReadLogNum; //玩家门派战报信息
	UINT8            byLastOperateType; //最后操作类型
}PKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF;

typedef struct tagPKT_CLIGS_PLAYER_FACTION_READ_REQ{
	tagPKT_CLIGS_PLAYER_FACTION_READ_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_FACTION_READ_REQ)); }
	UINT8            byExt; //扩展
}PKT_CLIGS_PLAYER_FACTION_READ_REQ;

typedef struct tagPKT_CLIGS_PLAYER_FACTION_READ_ACK{
	tagPKT_CLIGS_PLAYER_FACTION_READ_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_FACTION_READ_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byLogNum; //战报数量
	DT_PLAYER_FACTION_BATTLELOG_DATA astPlayerFactionLogList[MAX_FACTION_BATTLE_LOG_NUM]; //战报信息
}PKT_CLIGS_PLAYER_FACTION_READ_ACK;

typedef struct tagPKT_CLIGS_OPEN_FIND_DRINK_REQ{
	tagPKT_CLIGS_OPEN_FIND_DRINK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FIND_DRINK_REQ)); }
	UINT8            byExt; //扩展
}PKT_CLIGS_OPEN_FIND_DRINK_REQ;

typedef struct tagPKT_CLIGS_OPEN_FIND_DRINK_ACK{
	tagPKT_CLIGS_OPEN_FIND_DRINK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FIND_DRINK_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIGS_OPEN_FIND_DRINK_ACK;

typedef struct tagPKT_CLIGS_COURAGE_BATTLE_REQ{
	tagPKT_CLIGS_COURAGE_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_BATTLE_REQ)); }
	UINT8            byMonsterIndex; //第几波怪
}PKT_CLIGS_COURAGE_BATTLE_REQ;

typedef struct tagPKT_CLIGS_COURAGE_BATTLE_ACK{
	tagPKT_CLIGS_COURAGE_BATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_BATTLE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_BATTLE_DATA2  stBattleInfo; //战斗信息
	UINT8            byPassFlag; //是否通关
	DT_COURAGE_AWARD_DATA stCourageAwardInfo; //奖励信息
}PKT_CLIGS_COURAGE_BATTLE_ACK;

typedef struct tagPKT_CLIGS_COURAGE_BATTLE_NTF{
	tagPKT_CLIGS_COURAGE_BATTLE_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_BATTLE_NTF)); }
	UINT16           wErrCode; //错误码
	DT_BATTLE_DATA2  stBattleInfo; //战斗信息
	UINT8            byPassFlag; //是否通关
	DT_COURAGE_AWARD_DATA stCourageAwardInfo; //奖励信息
}PKT_CLIGS_COURAGE_BATTLE_NTF;

typedef struct tagPKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ{
	tagPKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ;

typedef struct tagPKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK{
	tagPKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_BUILD_LEVEL stBuildFlag; //门派建筑信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT8            byFactionLevel; //门派等级
	UINT32           dwFactionFunds; //门派资金
	UINT32           dwMyDoorTribute; //我的门贡
	TCHAR            aszFactionName[MAX_FACTION_NAME_LEN]; //门派名称
	DT_FACTION_BUILD_LEVEL stCurBuildLevel; //门派各建筑当前等级
	TCHAR            aszNewFactionName[MAX_FACTION_NUM]; //门派名称(以止为准)
	UINT8            byFactionIconID; //当前门派头像ID
	UINT8            byEditFacName; //是否可修改门派名称
	TCHAR            aszEditFacName[MAX_EDIT_LEN]; //修改门派名称信息
	UINT8            byUnDisplayBuildNum; //建筑个数
	UINT8            abyUnDisplayBuildID[16]; //建筑是否显示
}PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK;

typedef struct tagPKT_CLIGS_COURAGE_GROUP_DISMISS_NTF{
	tagPKT_CLIGS_COURAGE_GROUP_DISMISS_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GROUP_DISMISS_NTF)); }
	UINT8            byErrCode; //解散通知错误码
	DT_COURAGE_INFO_CLI stPlayerCourageData; //玩家勇气试炼信息
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍信息列表
}PKT_CLIGS_COURAGE_GROUP_DISMISS_NTF;

typedef struct tagPKT_CLIGS_HOT_SPRING_REWARD_NTF{
	tagPKT_CLIGS_HOT_SPRING_REWARD_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_HOT_SPRING_REWARD_NTF)); }
	UINT8            byRewardID1; //奖励ID1
	UINT16           wEachReward1; //奖励单次奖励值1
	UINT16           wSumReward1; //已领取奖励值1
	UINT8            byRewardID2; //奖励ID1
	UINT16           wEachReward2; //奖励单次奖励值1
	UINT16           wSumReward2; //已领取奖励值1
}PKT_CLIGS_HOT_SPRING_REWARD_NTF;

typedef struct tagPKT_CLIGS_COURAGE_GROUP_REFRESH_REQ{
	tagPKT_CLIGS_COURAGE_GROUP_REFRESH_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GROUP_REFRESH_REQ)); }
	UINT8            byExt; //扩展
}PKT_CLIGS_COURAGE_GROUP_REFRESH_REQ;

typedef struct tagPKT_CLIGS_COURAGE_GROUP_REFRESH_ACK{
	tagPKT_CLIGS_COURAGE_GROUP_REFRESH_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GROUP_REFRESH_ACK)); }
	UINT16           wErrCode; //错误码
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍信息列表
}PKT_CLIGS_COURAGE_GROUP_REFRESH_ACK;

typedef struct tagPKT_CLIGS_COURAGE_ENTER_GROUP_REQ{
	tagPKT_CLIGS_COURAGE_ENTER_GROUP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_ENTER_GROUP_REQ)); }
	UINT8            byExt; //扩展
}PKT_CLIGS_COURAGE_ENTER_GROUP_REQ;

typedef struct tagPKT_CLIGS_COURAGE_ENTER_GROUP_ACK{
	tagPKT_CLIGS_COURAGE_ENTER_GROUP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_ENTER_GROUP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_COURAGE_GROUP_DATA stGroupData; //勇气试炼队伍信息
}PKT_CLIGS_COURAGE_ENTER_GROUP_ACK;

typedef struct tagPKT_CLIGS_LEAVE_COURAGE_REQ{
	tagPKT_CLIGS_LEAVE_COURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_COURAGE_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_LEAVE_COURAGE_REQ;

typedef struct tagPKT_CLIGS_LEAVE_COURAGE_ACK{
	tagPKT_CLIGS_LEAVE_COURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_COURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_LEAVE_COURAGE_ACK;

typedef struct tagPKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ{
	tagPKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ;

typedef struct tagPKT_CLIGS_HOT_SPRING_OVER_REWARD_ACK{
	tagPKT_CLIGS_HOT_SPRING_OVER_REWARD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_HOT_SPRING_OVER_REWARD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_HOT_SPRING_OVER_REWARD_ACK;

typedef struct tagPKT_CLIGS_OPEN_GOLD_NTF{
	tagPKT_CLIGS_OPEN_GOLD_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GOLD_NTF)); }
	UINT8            byOpenFlag; //开启状态 0 为 关闭 1为开启
}PKT_CLIGS_OPEN_GOLD_NTF;

typedef struct tagPKT_CLIGS_HUNTING_ITEGRAL_REQ{
	tagPKT_CLIGS_HUNTING_ITEGRAL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_HUNTING_ITEGRAL_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_HUNTING_ITEGRAL_REQ;

typedef struct tagPKT_CLIGS_HUNTING_ITEGRAL_ACK{
	tagPKT_CLIGS_HUNTING_ITEGRAL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_HUNTING_ITEGRAL_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_HUNTING_SCHEDULE_INFO stScheduleInfo; //任务进度信息
	UINT8            byIsChangeFlag; //是否变档（0 没有， 1 有变档）
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
}PKT_CLIGS_HUNTING_ITEGRAL_ACK;

typedef struct tagPKT_CLIGS_ACTIVITY_END_NTF{
	tagPKT_CLIGS_ACTIVITY_END_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_ACTIVITY_END_NTF)); }
	UINT8            byBuildID; //建筑ID
	UINT8            byFuncTypeID; //功能ID
}PKT_CLIGS_ACTIVITY_END_NTF;

typedef struct tagPKT_CLIGS_PROMPT_IGNORE_REQ{
	tagPKT_CLIGS_PROMPT_IGNORE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PROMPT_IGNORE_REQ)); }
	UINT8            byActiviID; //活动功能ID
}PKT_CLIGS_PROMPT_IGNORE_REQ;

typedef struct tagPKT_CLIGS_PROMPT_IGNORE_ACK{
	tagPKT_CLIGS_PROMPT_IGNORE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PROMPT_IGNORE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byActiviID; //活动功能ID
}PKT_CLIGS_PROMPT_IGNORE_ACK;

typedef struct tagPKT_CLIGS_COURAGE_GROUP_REFRESH_NTF{
	tagPKT_CLIGS_COURAGE_GROUP_REFRESH_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GROUP_REFRESH_NTF)); }
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍信息列表
}PKT_CLIGS_COURAGE_GROUP_REFRESH_NTF;

typedef struct tagPKT_CLIGS_HOME_BUILD_PROMPT_NTF{
	tagPKT_CLIGS_HOME_BUILD_PROMPT_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_HOME_BUILD_PROMPT_NTF)); }
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_HOME_BUILD_PROMPT_NTF;

typedef struct tagPKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ{
	tagPKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ;

typedef struct tagPKT_CLIGS_OPEN_LEAVE_HOT_SPRING_ACK{
	tagPKT_CLIGS_OPEN_LEAVE_HOT_SPRING_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LEAVE_HOT_SPRING_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	TCHAR            aszPromptMsg[MAX_LEAVE_HOT_SPRING_PROMPT_MSG]; //文字提示信息
}PKT_CLIGS_OPEN_LEAVE_HOT_SPRING_ACK;

typedef struct tagPKT_CLIGS_FACTION_OPERATE_NTF{
	tagPKT_CLIGS_FACTION_OPERATE_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_OPERATE_NTF)); }
	UINT8            byFactionLogType; //日志类型 em_Faction_BattleLog_Type
}PKT_CLIGS_FACTION_OPERATE_NTF;

typedef struct tagPKT_CLIGS_FACTION_PROMPT_NTF{
	tagPKT_CLIGS_FACTION_PROMPT_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_PROMPT_NTF)); }
	DT_FUNC_COMMON_PROMPT stFactionPrompt; //门派提示内容
}PKT_CLIGS_FACTION_PROMPT_NTF;

typedef struct tagPKT_CLIGS_COURAGE_GET_AWARD_REQ{
	tagPKT_CLIGS_COURAGE_GET_AWARD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GET_AWARD_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_COURAGE_GET_AWARD_REQ;

typedef struct tagPKT_CLIGS_COURAGE_GET_AWARD_ACK{
	tagPKT_CLIGS_COURAGE_GET_AWARD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GET_AWARD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_COURAGE_GET_AWARD_ACK;

typedef struct tagPKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ{
	tagPKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ;

typedef struct tagPKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK{
	tagPKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byHavePass; //是否通关 0通关 1没通关
	DT_COURAGE_GROUP_PLAYER_BATTLE_DATA stGroupPlayerInfo; //队伍信息
	DT_COURAGE_JEWELRY_DATA_LST stjewelryInfoLst; //饰品信息
	UINT8            byHelpID; //协助描述 1再进行1次， 2协助两次送1次，3已经赠送不能在赠送了
}PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK;

typedef struct tagPKT_CLIGS_DOORSTRIBUTE_SYNC_NTF{
	tagPKT_CLIGS_DOORSTRIBUTE_SYNC_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_DOORSTRIBUTE_SYNC_NTF)); }
	UINT32           dwCurDoorTribute; //当前门贡
	UINT64           qwSumDoorsTribute; //累积门贡
}PKT_CLIGS_DOORSTRIBUTE_SYNC_NTF;

typedef struct tagPKT_CLIGS_AUDIT_FACTION_REQ2{
	tagPKT_CLIGS_AUDIT_FACTION_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_FACTION_REQ2)); }
	UINT32           dwAuditPlayerID; //玩家ID
	UINT8            byAuditWay; //审核方式（ 1 通过，0 拒绝
}PKT_CLIGS_AUDIT_FACTION_REQ2;

typedef struct tagPKT_CLIGS_AUDIT_FACTION_ACK2{
	tagPKT_CLIGS_AUDIT_FACTION_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_FACTION_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwAuditPlayerID; //玩家ID
	UINT8            byAuditWay; //审核方式（ 1 通过，0 拒绝 )
	UINT16           wFactionCurPlayerNum; //门派当前人数
	UINT16           wFactionMaxPlayerNum; //门派最大人数
	UINT16           wLeftAuditPlayerNum; //门派今天还可审核人数
	UINT16           wLeftApplyNum; //审核列表剩余人数
}PKT_CLIGS_AUDIT_FACTION_ACK2;

typedef struct tagDT_CROP_FIELD_INFO_DATA2{
	UINT8            byOpen; //田地开放
	UINT8            byLevel; //需要的等级
	UINT32           dwCoin; //花费的铜币
	UINT32           dwGold; //花费的元宝
	UINT8            byFieldLevel; //田地等级 默认为1级
	UINT8            byCanUpgrade; //是否可升级 0为不可升级 1为可升级
	UINT32           dwUpGold; //升级所需花费的元宝
	UINT32           dwCurSeedNum; //当前田地等级可播种数量
	UINT32           dwNexSeedNum; //升级田地后可播种数量
	UINT8            byNeedVipLevel; //升级所需Vip等级
}DT_CROP_FIELD_INFO_DATA2;

typedef struct tagDT_CROP_MORE_INFO_DATA2{
	UINT16           wCropKindID; //种植ID
	UINT32           dwCropTime; //收获倒计时间
	UINT8            byCropType; //种植类型
	UINT32           dwCropValue; //种子收益
	UINT16           wCropGainKindID; //收益的物品ID
	UINT32           dwCropNum; //种植数量
}DT_CROP_MORE_INFO_DATA2;

typedef struct tagPKT_CLIGS_OPEN_CROP_REQ2{
	tagPKT_CLIGS_OPEN_CROP_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_REQ2)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_CROP_REQ2;

typedef struct tagPKT_CLIGS_OPEN_CROP_ACK2{
	tagPKT_CLIGS_OPEN_CROP_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byCropSeedNum; //种子个数
	DT_CROP_SEED_INFO_DATA astCropSeedInfoList[MAX_CROP_SEED_NUM]; //种子信息
	DT_CROP_FIELD_INFO_DATA2 astCropFieldInfoList[MAX_CROP_NUM]; //田地信息
	DT_CROP_MORE_INFO_DATA2 astCropMoreInfoList[MAX_CROP_NUM]; //种植信息
	UINT16           wNoCDGoldPerMin; //立即完成每分钟消耗元宝
	UINT8            byBagFullFlag; //背包是否满，1为满
	UINT64           qwGold; //元宝
	UINT64           qwCoin; //铜钱
	UINT64           qwScience; //器魂
	UINT64           qwStory; //阅历
	UINT64           qwJingJie; //境界
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
}PKT_CLIGS_OPEN_CROP_ACK2;

typedef struct tagPKT_CLIGS_OPEN_CROP_FIELD_REQ2{
	tagPKT_CLIGS_OPEN_CROP_FIELD_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_FIELD_REQ2)); }
	UINT8            byFieldID; //田地ID
}PKT_CLIGS_OPEN_CROP_FIELD_REQ2;

typedef struct tagPKT_CLIGS_OPEN_CROP_FIELD_ACK2{
	tagPKT_CLIGS_OPEN_CROP_FIELD_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CROP_FIELD_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFieldID; //田地ID
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
	DT_CROP_FIELD_INFO_DATA2 stCropFieldInfo; //田地信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_OPEN_CROP_FIELD_ACK2;

typedef struct tagPKT_CLIGS_CROP_REQ2{
	tagPKT_CLIGS_CROP_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_CROP_REQ2)); }
	UINT8            byFieldID; //田地ID
	UINT16           wCropKindID; //田子ID
	UINT32           dwSeedNum; //种子数量
}PKT_CLIGS_CROP_REQ2;

typedef struct tagPKT_CLIGS_CROP_ACK2{
	tagPKT_CLIGS_CROP_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_CROP_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFieldID; //田地ID
	DT_CROP_MORE_INFO_DATA2 stCropMoreInfo; //种植信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_CROP_FIELD_INFO_DATA2 stCropFieldInfo; //田地信息
}PKT_CLIGS_CROP_ACK2;

typedef struct tagPKT_CLIGS_GAIN_CROP_REQ2{
	tagPKT_CLIGS_GAIN_CROP_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_GAIN_CROP_REQ2)); }
	UINT8            byFieldID; //田地ID
}PKT_CLIGS_GAIN_CROP_REQ2;

typedef struct tagPKT_CLIGS_GAIN_CROP_ACK2{
	tagPKT_CLIGS_GAIN_CROP_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_GAIN_CROP_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwCoin; //铜钱
	UINT64           qwScience; //器魂
	UINT64           qwStory; //阅历
	UINT64           qwJingJie; //境界
	UINT8            byCropType; //种植类型
	UINT32           dwCropValue; //种子收益
	UINT16           wCropKindID; //生成物品ID
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_CROP_MORE_INFO_DATA2 stCropMoreInfo; //种植信息
	DT_CROP_FIELD_INFO_DATA2 stCropFieldInfo; //田地信息
}PKT_CLIGS_GAIN_CROP_ACK2;

typedef struct tagPKT_CLIGS_CLR_CROP_REQ2{
	tagPKT_CLIGS_CLR_CROP_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_CROP_REQ2)); }
	UINT8            byFieldID; //田地ID
}PKT_CLIGS_CLR_CROP_REQ2;

typedef struct tagPKT_CLIGS_CLR_CROP_ACK2{
	tagPKT_CLIGS_CLR_CROP_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_CROP_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFieldID; //田地ID
	UINT64           qwGold; //元宝
	DT_CROP_MORE_INFO_DATA2 stCropMoreInfo; //种植信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_CLR_CROP_ACK2;

typedef struct tagPKT_CLIGS_UPGRADE_CROP_FIELD_REQ{
	tagPKT_CLIGS_UPGRADE_CROP_FIELD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_CROP_FIELD_REQ)); }
	UINT8            byFieldID; //田地ID
}PKT_CLIGS_UPGRADE_CROP_FIELD_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_CROP_FIELD_ACK{
	tagPKT_CLIGS_UPGRADE_CROP_FIELD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_CROP_FIELD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFieldID; //田地ID
	UINT64           qwGold; //当前元宝
	DT_CROP_FIELD_INFO_DATA stCropFieldInfo; //田地信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_UPGRADE_CROP_FIELD_ACK;

typedef struct tagPKT_CLIGS_AUDIT_ALL_REFUSE_REQ{
	tagPKT_CLIGS_AUDIT_ALL_REFUSE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_ALL_REFUSE_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_AUDIT_ALL_REFUSE_REQ;

typedef struct tagPKT_CLIGS_AUDIT_ALL_REFUSE_ACK{
	tagPKT_CLIGS_AUDIT_ALL_REFUSE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_ALL_REFUSE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_AUDIT_ALL_REFUSE_ACK;

typedef struct tagDT_FACTION_PLAYER_AUDIT_CLI2{
	UINT32           dwID; //玩家唯一ID
	UINT16           wHeroKindID; //主武将类型
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT16           wLevel; //玩家等级
	UINT32           dwPower; //战力
	UINT8            byVIPLevel; //VIP等级
}DT_FACTION_PLAYER_AUDIT_CLI2;

typedef struct tagDT_FACTION_PLAYER_AUDIT_CLI_LST2{
	UINT16           wPlayerNum; //玩家个数
	DT_FACTION_PLAYER_AUDIT_CLI2 astFactionPlayerAuditCli[DT_FACTION_PLAYER_NUM]; //门派信息
}DT_FACTION_PLAYER_AUDIT_CLI_LST2;

typedef struct tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ2{
	tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ2)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_AUDIT_FACTION_REQ2;

typedef struct tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK2{
	tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_PLAYER_AUDIT_CLI_LST2 stFactionPlayerAuditCliLst; //审核列表
	UINT16           wLeftAuditNum; //剩余审核人数
	UINT16           wCurPlayerNum; //门派当前人数
	UINT16           wMaxPlayerNum; //门派人数上限
}PKT_CLIGS_OPEN_AUDIT_FACTION_ACK2;

typedef struct tagDT_VIP_GIFT_EACH_DATA{
	UINT32           dwRequireOldSourceNum; //所需要消耗的旧资源数量
	UINT32           dwRequireNewSourceNum; //所需要消耗的新资源数量
	DT_RES_ITEM_DATA stVipItemData; //物品的数据
	UINT16           wVipGiftGetNum; //每一VIP等级对应的可领取礼包的次数
}DT_VIP_GIFT_EACH_DATA;

typedef struct tagDT_VIP_GIFT_DATA{
	UINT8            byMaxVipGift; //最大的礼包数量
	DT_VIP_GIFT_EACH_DATA astVipGiftData[MAX_VIP_GIFT]; //每一个礼包的物品数据
}DT_VIP_GIFT_DATA;

typedef struct tagPKT_CLIGS_OPEN_VIP_GIFT_REQ{
	tagPKT_CLIGS_OPEN_VIP_GIFT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_VIP_GIFT_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_OPEN_VIP_GIFT_REQ;

typedef struct tagPKT_CLIGS_OPEN_VIP_GIFT_ACK{
	tagPKT_CLIGS_OPEN_VIP_GIFT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_VIP_GIFT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwVipMaxLevel; //最大VIP等级
	DT_VIP_GIFT_DATA astGiftItemData[MAX_VIP_LEVEL]; //每一VIP等级礼包的数据
	UINT32           dwCurVipLevel; //当前VIP等级
}PKT_CLIGS_OPEN_VIP_GIFT_ACK;

typedef struct tagPKT_CLIGS_GET_VIP_GIFT_REQ{
	tagPKT_CLIGS_GET_VIP_GIFT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_VIP_GIFT_REQ)); }
	UINT8            byLevel; //获取的礼包等级
	UINT8            byIndex; //获取的礼包索引
}PKT_CLIGS_GET_VIP_GIFT_REQ;

typedef struct tagPKT_CLIGS_GET_VIP_GIFT_ACK{
	tagPKT_CLIGS_GET_VIP_GIFT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_VIP_GIFT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RES_ITEM_DATA stResItemData; //获得奖励的物品数据
	DT_RSYNC_RES_ITEM_DATA stRsyncResItemData; //资源列表
	UINT8            byVipLevel; //VIP礼包数据变更等级
	UINT8            byIndex; //VIP礼包数据变更索引
	UINT16           wVipGiftGetNum; //VIP礼包获取剩余次数
}PKT_CLIGS_GET_VIP_GIFT_ACK;

typedef struct tagDT_RECV_FLOWER_LOG_CLI_EX{
	UINT32           dwSendPlayerID; //送花的玩家ID
	TCHAR            aszSendPlayerName[USERNAME_LEN]; //送花的玩家名称
	UINT32           dwKindID; //鲜花ID
	UINT64           qwSendTime; //送花的时间
	UINT8            byFlag; //送花的时间0:已领取，1：未领取
	DT_RES_ITEM_DATA stResItemData; //资源列表
	UINT64           qwExpires; //过期时间
}DT_RECV_FLOWER_LOG_CLI_EX;

typedef struct tagDT_RECV_FLOWER_LOG_LIST_CLI_EX{
	UINT8            byLogNum; //记录个数
	DT_RECV_FLOWER_LOG_CLI_EX astListData[MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM]; //列表信息
	UINT8            byAlsoReceive; //还可接收
	UINT8            byUnReceiveNum; //未收花条数
}DT_RECV_FLOWER_LOG_LIST_CLI_EX;

typedef struct tagPKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ{
	tagPKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_RECV_FLOWER_TAB_ACK{
	tagPKT_CLIGS_OPEN_RECV_FLOWER_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RECV_FLOWER_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RECV_FLOWER_LOG_LIST_CLI_EX stRecvFlowerLogListCLiEx; //接收鲜花日志
}PKT_CLIGS_OPEN_RECV_FLOWER_TAB_ACK;

typedef struct tagPKT_CLIGS_ACCEPT_FLOWER_REQ{
	tagPKT_CLIGS_ACCEPT_FLOWER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ACCEPT_FLOWER_REQ)); }
	UINT32           dwSendPlayerID; //送花的玩家ID
	UINT64           qwSendTime; //送花的时间
}PKT_CLIGS_ACCEPT_FLOWER_REQ;

typedef struct tagPKT_CLIGS_ACCEPT_FLOWER_ACK{
	tagPKT_CLIGS_ACCEPT_FLOWER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ACCEPT_FLOWER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RES_ITEM_DATA stGainResItemData; //获得资源列表
	DT_RSYNC_RES_ITEM_DATA stRsyncResItemData; //同步资源列表
	DT_RECV_FLOWER_LOG_LIST_CLI_EX stRecvFlowerLogListCLiEx; //接收鲜花日志
}PKT_CLIGS_ACCEPT_FLOWER_ACK;

typedef struct tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ{
	tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ;

typedef struct tagDT_FRIENDS_CLI{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispName[USERNAME_LEN]; //玩家信息
	UINT8            byCareerID; //职业ID
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT8            bySameFaction; //是否同一门派
}DT_FRIENDS_CLI;

typedef struct tagDT_ITEM_FLOWER{
	DT_ITEM_DATA_CLI2 stItem; //鲜花信息
	DT_RES_ITEM_DATA stResItemData; //奖励资源列表
}DT_ITEM_FLOWER;

typedef struct tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK{
	tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byItemNum; //道具个数
	DT_ITEM_FLOWER   astItemList[MAX_FLOWER_ITEM_NUM]; //道具信息
	UINT8            byFriendsNum; //好友个数
	DT_FRIENDS_CLI   astFriendsList[MAX_RELATION_LIST_ELEM_NUM]; //好友信息
	UINT8            byAgainNum; //再送几次获得奖励
	DT_RES_ITEM_DATA stResItemData; //奖励资源列表
	UINT8            byUnReceiveNum; //未收花条数
}PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK;

typedef struct tagPKT_CLIGS_BUY_RAREITEM_REQ2{
	tagPKT_CLIGS_BUY_RAREITEM_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_RAREITEM_REQ2)); }
	UINT32           dwGoodsID; //物品ID
	UINT8            byLeftTime; //物品剩余次数
}PKT_CLIGS_BUY_RAREITEM_REQ2;

typedef struct tagPKT_CLIGS_BUY_RAREITEM_ACK2{
	tagPKT_CLIGS_BUY_RAREITEM_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_RAREITEM_ACK2)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_RAREITEM_INFO_CLI stRareItemInfo; //总后换珍品信息
	UINT32           dwLeftDoorsTributes; //玩家剩余门贡
}PKT_CLIGS_BUY_RAREITEM_ACK2;

typedef struct tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2{
	tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2)); }
	UINT32           dwSkillID; //技能ID
	UINT8            bySkillCurLevel; //技能当前等级
}PKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2;

typedef struct tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2{
	tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_SKILL_CLI_INFO stSkillInfo; //技能信息
	UINT32           dwLeftDoorsTribute; //玩家剩余门贡
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	UINT32           dwSumDoorsTribute; //累积门贡要求
}PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2;

typedef struct tagPKT_CLIGS_SEND_FLOWER_EX_REQ{
	tagPKT_CLIGS_SEND_FLOWER_EX_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEND_FLOWER_EX_REQ)); }
	UINT32           dwRecvPlayerID; //收花玩家ID
	UINT32           dwKindID; //送花的ID
}PKT_CLIGS_SEND_FLOWER_EX_REQ;

typedef struct tagPKT_CLIGS_SEND_FLOWER_EX_ACK{
	tagPKT_CLIGS_SEND_FLOWER_EX_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SEND_FLOWER_EX_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI2 astItemList[MAX_FLOWER_ITEM_NUM]; //道具信息
	DT_RES_ITEM_DATA stGainResItemData; //获得资源列表
	DT_RSYNC_RES_ITEM_DATA stRsyncResItemData; //同步资源列表
	UINT8            byAgainNum; //再送几次获得奖励
	DT_RES_ITEM_DATA stResItemData; //奖励资源列表
}PKT_CLIGS_SEND_FLOWER_EX_ACK;

typedef struct tagDT_FACTION_INFO_CLI{
	UINT8            byIconID; //头像ID （0 为无头像）
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT32           dwFactionLevel; //门派等级
	TCHAR            aszHeadName[NORMAL_MSG_LEN]; //帮主名称
	UINT16           wFactionNumberPeople; //门派在线人数
	UINT16           wMaxFactionNumberPeople; //门派最大人数
	UINT32           dwFactionFunds; //门派资金
	UINT8            byAuditNum; //审核面板中的申请人数
	TCHAR            aszFactionNotice[MAX_FACTION_DESC_NUM]; //门派通知
	TCHAR            aszFactionAnnouncement[MAX_FACTION_DESC_NUM]; //门派公告
	UINT32           dwFactionRank; //门派排名
}DT_FACTION_INFO_CLI;

typedef struct tagDT_FACTION_SELF_INFO_CLI{
	UINT32           dwMyDoorsTribute; //我的门贡
	UINT64           qwMySumDoorsTribute; //我的累积门贡
	UINT64           qwJoinFactionTime; //加入门派时间
	UINT8            byRecvFlag; //可领奖标志 0 不可领奖， 1可领奖, 2已领取
	UINT8            byFindBackDoorsTribute; //是否有可找回门贡 （0 没有， 1有）
	DT_FACTION_AUTHORITY_EX stFactionAuthority; //门派权限信息
}DT_FACTION_SELF_INFO_CLI;

typedef struct tagDT_FACTION_MANAGER_INFO_CLI{
	UINT8            byPlayerJobType; //玩家职位ID
	TCHAR            aszPlayerName[NORMAL_MSG_LEN]; //玩家名称
}DT_FACTION_MANAGER_INFO_CLI;

typedef struct tagDT_FACTION_MANAGER_INFO_LST_CLI{
	UINT8            byManagerNum; //管理人员个数
	DT_FACTION_MANAGER_INFO_CLI astManagerInfo[MAX_FACTION_MANAGER_NUM]; //管理人员信息
}DT_FACTION_MANAGER_INFO_LST_CLI;

typedef struct tagPKT_CLIGS_OPEN_FACTION_REQ{
	tagPKT_CLIGS_OPEN_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_FACTION_ACK{
	tagPKT_CLIGS_OPEN_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_INFO_CLI stFactionInfo; //门派信息
	DT_FACTION_MANAGER_INFO_LST_CLI stManagerInfo; //管理人员信息
	DT_FACTION_SELF_INFO_CLI stPlayerInfo; //个人信息
}PKT_CLIGS_OPEN_FACTION_ACK;

typedef struct tagDT_PLAYER_POWER_INFO_LST{
	UINT16           wPlayerNum; //玩家个数
	UINT32           adwPowerInfo[DT_FACTION_PLAYER_NUM]; //玩家战力
}DT_PLAYER_POWER_INFO_LST;

typedef struct tagPKT_CLIGS_FACTION_PLAYERS_INFO_REQ{
	tagPKT_CLIGS_FACTION_PLAYERS_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_PLAYERS_INFO_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_FACTION_PLAYERS_INFO_REQ;

typedef struct tagPKT_CLIGS_FACTION_PLAYERS_INFO_ACK{
	tagPKT_CLIGS_FACTION_PLAYERS_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FACTION_PLAYERS_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_PLAYER_CLI_LST stFactionPlayerCliLst; //门派玩家信息
	DT_DOORSTRIBUTE_LEVE_DATA_LST stDoorsTributeLst; //贡献度列表
	DT_FACTION_AUTHORITY_EX stFactionAuthority; //门派权限信息
	DT_PLAYER_POWER_INFO_LST stPlayerPowerInfo; //玩家战力
}PKT_CLIGS_FACTION_PLAYERS_INFO_ACK;

typedef struct tagPKT_CLIGS_VIEW_FACTION_MEMBERS_REQ{
	tagPKT_CLIGS_VIEW_FACTION_MEMBERS_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_VIEW_FACTION_MEMBERS_REQ)); }
	UINT32           dwFactionID; //门派ID
}PKT_CLIGS_VIEW_FACTION_MEMBERS_REQ;

typedef struct tagPKT_CLIGS_VIEW_FACTION_MEMBERS_ACK{
	tagPKT_CLIGS_VIEW_FACTION_MEMBERS_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_VIEW_FACTION_MEMBERS_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_FACTION_MEMBER_INFO_LIST stMemberList; //门派玩家信息
	UINT8            byIsLocalFaction; //是否本服门派（1是/0否），非本服门派无法点击玩家查看具体的玩家信息
}PKT_CLIGS_VIEW_FACTION_MEMBERS_ACK;

typedef struct tagPKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ{
	tagPKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ;

typedef struct tagPKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK{
	tagPKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFreeNum; //剩余免费次数(当为0时才消耗元宝)
	UINT32           dwCostGold; //需要消耗元宝
	UINT8            byIconID; //当前头像ID(0默认)
	UINT64           qwGoldNum; //玩家元宝
	UINT8            byChangeNum; //当前第几次更换
}PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK;

typedef struct tagPKT_CLIGS_CHANGE_FACTION_ICON_REQ{
	tagPKT_CLIGS_CHANGE_FACTION_ICON_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_FACTION_ICON_REQ)); }
	UINT8            byIconID; //头像ID
}PKT_CLIGS_CHANGE_FACTION_ICON_REQ;

typedef struct tagPKT_CLIGS_CHANGE_FACTION_ICON_ACK{
	tagPKT_CLIGS_CHANGE_FACTION_ICON_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_FACTION_ICON_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byIconID; //更换后的头像ID
	UINT64           qwGoldSync; //元宝同步
}PKT_CLIGS_CHANGE_FACTION_ICON_ACK;

typedef struct tagPKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ{
	tagPKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ;

typedef struct tagPKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK{
	tagPKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byNormalPercent; //普通找回的百分比
	UINT8            byPerfectPercent; //完美找回的百分比
	DT_RES_DATA      stNormalCost; //普通找回消耗
	DT_RES_DATA      stPerfectCost; //完美找回消耗
	UINT32           dwFindBackDoorsTribute; //可找回门贡
}PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK;

typedef struct tagPKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ{
	tagPKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ)); }
	UINT8            byFindBackType; //找回方式（0 普通找回， 1完美找回）
}PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ;

typedef struct tagPKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK{
	tagPKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwSyncDoorsTribute; //门贡同步
}PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK;

typedef struct tagPKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ{
	tagPKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ;

typedef struct tagPKT_CLIGS_LEAVE_COURAGE_BATTLE_ACK{
	tagPKT_CLIGS_LEAVE_COURAGE_BATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_LEAVE_COURAGE_BATTLE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_LEAVE_COURAGE_BATTLE_ACK;

typedef struct tagPKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ{
	tagPKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ)); }
	UINT32           dwExt; //扩展字段
}PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ;

typedef struct tagPKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK{
	tagPKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwGroupID; //加入组ID
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍列表
}PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK;

typedef struct tagPKT_CLIGS_OPEN_EMBATTLE_REQ2{
	tagPKT_CLIGS_OPEN_EMBATTLE_REQ2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EMBATTLE_REQ2)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_EMBATTLE_REQ2;

typedef struct tagDT_FORMATION_DATA_LEVEL{
	UINT8            byFormationID; //阵型ID
	UINT8            byFormationLevel; //阵型等级
	UINT8            byFormationType; //阵型加成类型(commondef.h EBattleAttributeKind)
	UINT32           dwFormationValue; //阵型加成值
	DT_RES_DATA      stUpgradeCost; //阵型升级消耗
	UINT8            byMaxPlayerNum; //阵型最多上阵人数(解锁位置数)
	UINT8            byIsUpgrade; //资源是否足够升级（0 正常， 1科技未解锁， 2资源不足，3 玩家等级不足升级, 4未显示阵法， 5未解锁阵法， 6满级）
	UINT8            byNeedPlayerLevel; //升级所需的人物等级
}DT_FORMATION_DATA_LEVEL;

typedef struct tagDT_NEW_FORMATION_INFO_CLI{
	UINT8            byFormationID; //阵型ID
	UINT8            byPostionNum; //位置个数
	DT_NEW_FORMATION_POSTION_INFO astPostionInfo[MAX_FORMATION_IDX_NUM]; //位置信息
	DT_FORMATION_DATA_LEVEL stFormationLevel; //阵型等级信息
}DT_NEW_FORMATION_INFO_CLI;

typedef struct tagDT_NEW_FORMATION_LST_CLI{
	UINT8            bySelectIndex; //选中的阵型
	UINT8            byFormationNum; //阵型个数
	DT_NEW_FORMATION_INFO_CLI astFormationArray[MAX_NEW_FORMATION_NUM]; //阵型数组信息
}DT_NEW_FORMATION_LST_CLI;

typedef struct tagDT_NEW_HERO_EMBATLE_DATA{
	UINT16           wKindID; //武将类型
	UINT8            byCareerID; //职业ID
	UINT16           wLevel; //等级
	UINT16           wTalent; //资质
	UINT8            byFormationNum; //阵型个数
	UINT32           adwPower[MAX_NEW_FORMATION_NUM]; //战力
	DT_BATTLE_ATTRIBUTE astBattleAttribute[MAX_NEW_FORMATION_NUM]; //战斗属性
	DT_SKILL_DATA_CLI stSkillInfo; //技能信息
}DT_NEW_HERO_EMBATLE_DATA;

typedef struct tagPKT_CLIGS_OPEN_EMBATTLE_ACK2{
	tagPKT_CLIGS_OPEN_EMBATTLE_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EMBATTLE_ACK2)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroNum; //武将数目
	DT_NEW_HERO_EMBATLE_DATA astHeroInfoLst[MAX_HERO_NUM]; //武将信息
	DT_NEW_FORMATION_LST_CLI stFormationInfo; //阵型信息
	DT_RES_DATA      stCurRes; //当前阅历
}PKT_CLIGS_OPEN_EMBATTLE_ACK2;

typedef struct tagPKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ{
	tagPKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ;

typedef struct tagPKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK{
	tagPKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFormationNum; //阵法数
	DT_FORMATION_DATA_LEVEL astFormationInfo[MAX_NEW_FORMATION_NUM]; //阵法信息
	DT_RES_DATA      stCurRes; //当前阅历
}PKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK;

typedef struct tagPKT_CLIGS_UPGRADE_FORMATION_REQ{
	tagPKT_CLIGS_UPGRADE_FORMATION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_FORMATION_REQ)); }
	UINT8            byFormationID; //阵法ID
	DT_FORMATION_DATA stFormationInfo; //阵型信息
}PKT_CLIGS_UPGRADE_FORMATION_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_FORMATION_ACK{
	tagPKT_CLIGS_UPGRADE_FORMATION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_FORMATION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroNum; //武将数目
	DT_NEW_HERO_EMBATLE_DATA astHeroInfoLst[MAX_HERO_NUM]; //武将信息
	DT_NEW_FORMATION_LST_CLI stFormationInfo; //阵型信息
	DT_POWER_CHANGE_DATA stPower; //战力改变情况
	DT_FUNC_COMMON_PROMPT stPromptInfo; //影响的提示信息
	DT_RES_DATA      stSyncRes; //阅历同步
}PKT_CLIGS_UPGRADE_FORMATION_ACK;

typedef struct tagPKT_CLILS_REGISTER_FACTION_REQ{
	tagPKT_CLILS_REGISTER_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_REGISTER_FACTION_REQ)); }
	TCHAR            aszDspName[MAX_FACTION_NUM]; //战力变化信息
}PKT_CLILS_REGISTER_FACTION_REQ;

typedef struct tagPKT_CLILS_REGISTER_FACTION_ACK{
	tagPKT_CLILS_REGISTER_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_REGISTER_FACTION_ACK)); }
	UINT16           wErrCode; //战力变化信息
	TCHAR            aszDspName[MAX_FACTION_NUM]; //战力变化信息
}PKT_CLILS_REGISTER_FACTION_ACK;

typedef struct tagPKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ{
	tagPKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ)); }
	UINT8            byFormationID; //阵法ID
}PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK{
	tagPKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byFormationNum; //阵法数
	DT_FORMATION_DATA_LEVEL astFormationInfo[MAX_NEW_FORMATION_NUM]; //阵型信息
	DT_POWER_CHANGE_DATA stPower; //战力改变情况
	DT_FUNC_COMMON_PROMPT stPromptInfo; //影响的提示信息
	DT_RES_DATA      stSyncRes; //阅历同步
}PKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK;

typedef struct tagPKT_CLIGS_GET_BLUE_ENCOURAGE_REQ{
	tagPKT_CLIGS_GET_BLUE_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_BLUE_ENCOURAGE_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_GET_BLUE_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_GET_BLUE_ENCOURAGE_ACK{
	tagPKT_CLIGS_GET_BLUE_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_BLUE_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BEGBLUE_PLAYER_RES_CLI stPlayerResInfo; //玩家当前资源信息
	DT_BLUE_ENCOURAGEINFO_CLI stEncourage; //奖励信息
	UINT32           dwBlue; //领取的仙露
}PKT_CLIGS_GET_BLUE_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ{
	tagPKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ)); }
	UINT32           dwExprienceValue; //可领取经验（参考值（误差2秒内））
}PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ;

typedef struct tagPKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK{
	tagPKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_HOT_SPRING_EXPERIENCE stPlayerExprience; //经验显示
	DT_HERO_UPGRADE_DATA stHeroUpgradeInfo; //升级信息显示
	UINT32           dwExprienceValue; //领取的经验值
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
}PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK;

typedef struct tagPKT_CLIGS_OPEN_FACTION_SKILL_REQ{
	tagPKT_CLIGS_OPEN_FACTION_SKILL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_SKILL_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_FACTION_SKILL_REQ;

typedef struct tagPKT_CLIGS_OPEN_FACTION_SKILL_ACK{
	tagPKT_CLIGS_OPEN_FACTION_SKILL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_SKILL_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            bySkillNum; //技能个数
	DT_SKILL_CLI_INFO astSkillInfo[MAX_GYMNASIUM_SKILL_NUM]; //技能信息
	UINT8            byStatu; //状态（0正常，1未加入门派未学习技能, 2加入门派，武堂开启，未学习技能， 3加入门派，武堂未开启）
}PKT_CLIGS_OPEN_FACTION_SKILL_ACK;

typedef struct tagPKT_CLIGS_RESOURCE_SYNC_NTF{
	tagPKT_CLIGS_RESOURCE_SYNC_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_RESOURCE_SYNC_NTF)); }
	DT_RSYNC_RES_ITEM_DATA stResourceSync; //资源同步
}PKT_CLIGS_RESOURCE_SYNC_NTF;

typedef struct tagPKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ{
	tagPKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ)); }
	UINT8            byBagGridIdx; //背包心得位置，从0开始
}PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ;

typedef struct tagPKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK{
	tagPKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCommonNum; //普通分解获得的碎片数量
	UINT16           wSpecialNum; //元宝分解获得的碎片数量
	UINT32           dwGoldNum; //元宝分解所需元宝
	UINT32           dwCoinNum; //普通分解所需铜钱
	UINT8            byBagGridIdx; //背包心得位置，从0开始
}PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK;

typedef struct tagPKT_CLIGS_RESOLVE_TRIGRAM_REQ{
	tagPKT_CLIGS_RESOLVE_TRIGRAM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RESOLVE_TRIGRAM_REQ)); }
	UINT8            byUseGold; //是否使用元宝 1为使用，0为不使用
	UINT8            byBagGridIdx; //背包心得位置，从0开始
}PKT_CLIGS_RESOLVE_TRIGRAM_REQ;

typedef struct tagPKT_CLIGS_RESOLVE_TRIGRAM_ACK{
	tagPKT_CLIGS_RESOLVE_TRIGRAM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RESOLVE_TRIGRAM_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byBagGridIdx; //背包心得位置，从0开始
	UINT64           qwStudyClip; //总共心得碎片
	UINT32           dwGetStudyClip; //分解所得心得碎片
	DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]; //背包心得信息
	UINT64           qwGoldNum; //玩家元宝
	UINT64           qwCoinNum; //玩家铜钱
	UINT8            byUseGold; //是否使用元宝 1为使用，0为不使用
}PKT_CLIGS_RESOLVE_TRIGRAM_ACK;

typedef struct tagDT_JEWELRY_DATA_LST_CLI{
	UINT16           wJewelryNum; //饰品个数
	DT_JEWELRY_DATA_CLI astJewelryList[MAX_BAG_OPEN_NUM]; //饰品信息
}DT_JEWELRY_DATA_LST_CLI;

typedef struct tagDT_SKILL_DATA_LST_CLI{
	UINT8            bySkillNum; //技能个数
	DT_SKILL_DATA_CLI astSkillInfo[MAX_SKILL_NUM]; //技能信息
}DT_SKILL_DATA_LST_CLI;

typedef struct tagDT_HERO_JEWELRY_DATA_LST_CLI{
	UINT16           wJewelryNum; //饰品个数
	DT_JEWELRY_DATA_CLI astJewelryList[MAX_JEWELRY_POS]; //饰品信息
}DT_HERO_JEWELRY_DATA_LST_CLI;

typedef struct tagDT_HERO_DATA_CLI2{
	UINT16           wHeroNum; //武将数目
	DT_HERO_PANEL_CLI astHeroBaseData[MAX_HERO_NUM]; //武将基本信息
	DT_HERO_EQUIP_DATA_LST_CLI astEquipDataList[MAX_HERO_NUM]; //武将装备信息
	DT_ATTR_ENABLE_UPDATE_INFO astEnableAttrUpdate[MAX_HERO_NUM]; //属性升级信息
	DT_SKILL_DATA_LST_CLI astSkillInfo[MAX_HERO_NUM]; //武将技能信息
	DT_HERO_JEWELRY_DATA_LST_CLI astJewelryInfo[MAX_HERO_NUM]; //饰品信息
	DT_HERO_SOUL_DATA_LST_CLI astSoulInfo[MAX_HERO_NUM]; //战魂信息
}DT_HERO_DATA_CLI2;

typedef struct tagDT_HERO_DATA_CLI_v410{
	UINT16           wHeroNum; //武将数目
	DT_HERO_PANEL_CLI astHeroBaseData[MAX_HERO_NUM]; //武将基本信息
	DT_HERO_EQUIP_DATA_LST_CLI_v410 astEquipDataList[MAX_HERO_NUM]; //武将装备信息
	DT_ATTR_ENABLE_UPDATE_INFO astEnableAttrUpdate[MAX_HERO_NUM]; //属性升级信息
	DT_SKILL_DATA_LST_CLI astSkillInfo[MAX_HERO_NUM]; //武将技能信息
	DT_HERO_JEWELRY_DATA_LST_CLI astJewelryInfo[MAX_HERO_NUM]; //饰品信息
	DT_HERO_SOUL_DATA_LST_CLI astSoulInfo[MAX_HERO_NUM]; //战魂信息
}DT_HERO_DATA_CLI_v410;

typedef struct tagDT_HERO_DATA_CLI2_OLD{
	UINT16           wHeroNum; //武将数目
	DT_HERO_PANEL_CLI astHeroBaseData[MAX_HERO_NUM]; //武将基本信息
	DT_HERO_EQUIP_DATA_LST_CLI astEquipDataList[MAX_HERO_NUM]; //武将装备信息
	DT_ATTR_ENABLE_UPDATE_INFO astEnableAttrUpdate[MAX_HERO_NUM]; //属性升级信息
	DT_SKILL_DATA_LST_CLI astSkillInfo[MAX_HERO_NUM]; //武将技能信息
	DT_HERO_JEWELRY_DATA_LST_CLI astJewelryInfo[MAX_HERO_NUM]; //饰品信息
}DT_HERO_DATA_CLI2_OLD;

typedef struct tagDT_JEWELRY_SUIT_LVL_ATTR{
	UINT8            byAttrType; //套装属性类别，见 JewelryExtAttr
	UINT32           dwAttrVal; //套装提升的属性值
	UINT8            byValType; //套装提升的属性类型：固定值、百分比，见 ValueType
}DT_JEWELRY_SUIT_LVL_ATTR;

typedef struct tagDT_JEWELRY_SUIT_ATTR{
	UINT16           wSuitLvl; //套装等级
	DT_JEWELRY_SUIT_LVL_ATTR stLvlAttr; //该级对应的属性
}DT_JEWELRY_SUIT_ATTR;

typedef struct tagDT_JEWELRY_SUIT_CLI{
	UINT16           wSuitLvl; //套装等级
	UINT8            bySuitAttrNum; //套装等级
	DT_JEWELRY_SUIT_ATTR astSuitAttrLst[MAX_JEWELRY_SUIT_ATTR_NUM]; //套装附加属性列表
}DT_JEWELRY_SUIT_CLI;

typedef struct tagDT_JEWELRY_SUIT_LIST_CLI{
	UINT16           wHeroNum; //伙伴信息
	DT_JEWELRY_SUIT_CLI astSuitList[MAX_HERO_NUM]; //每个伙伴身上的饰品套装信息
}DT_JEWELRY_SUIT_LIST_CLI;

typedef struct tagDT_PLAYER_INFO_PANEL{
	DT_HERO_DATA_CLI2 stHeroInfo; //伙伴信息
	DT_ITEM_DATA_LIST_CLI3 stItemInfo; //装备信息（装备，碎片，进价石）
	DT_ITEM_DATA_LIST_CLI3 stJewelryInfo; //佩饰信息
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
	UINT8            byGoodsCellNum; //装备被占用格子数 （除装备外，其它物品占用数）
	UINT8            byJewelryCellNum; //饰品被占用格子数(除饰品外，其它物品占用数)
	DT_SOUL_DATA_LST_CLI stSoulInfo; //战魂背包
}DT_PLAYER_INFO_PANEL;

typedef struct tagDT_PLAYER_INFO_PANEL_OLD{
	DT_HERO_DATA_CLI2_OLD stHeroInfo; //伙伴信息
	DT_ITEM_DATA_LIST_CLI3 stItemInfo; //装备信息（装备，碎片，进价石）
	DT_ITEM_DATA_LIST_CLI3 stJewelryInfo; //佩饰信息
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
	UINT8            byGoodsCellNum; //装备被占用格子数 （除装备外，其它物品占用数）
	UINT8            byJewelryCellNum; //饰品被占用格子数(除饰品外，其它物品占用数)
}DT_PLAYER_INFO_PANEL_OLD;

typedef struct tagPKT_CLIGS_OPEN_PLAYER_PANEL_REQ{
	tagPKT_CLIGS_OPEN_PLAYER_PANEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLAYER_PANEL_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_PLAYER_PANEL_REQ;

typedef struct tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD{
	tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_INFO_PANEL_OLD stPlayerPanel; //人物面板信息
	DT_BAG_DATA_CLI  stBagInfo; //背包基本信息
	UINT16           wCoachExpRate; //主角经验倍数(百分比)
}PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD;

typedef struct tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK{
	tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_INFO_PANEL stPlayerPanel; //人物面板信息
	DT_BAG_DATA_CLI  stBagInfo; //背包基本信息
	UINT16           wCoachExpRate; //主角经验倍数(百分比)
}PKT_CLIGS_OPEN_PLAYER_PANEL_ACK;

typedef struct tagDT_PLAYER_INFO_PANEL_v410{
	DT_HERO_DATA_CLI2 stHeroInfo; //伙伴信息
	DT_ITEM_DATA_LIST_CLI4 stItemInfo; //装备信息（装备，碎片，进价石）
	DT_ITEM_DATA_LIST_CLI4 stJewelryInfo; //佩饰信息
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
	UINT16           wGoodsCellNum; //装备被占用格子数 （除装备外，其它物品占用数）
	UINT16           wJewelryCellNum; //饰品被占用格子数(除饰品外，其它物品占用数)
	DT_SOUL_DATA_LST_CLI stSoulInfo; //战魂背包
}DT_PLAYER_INFO_PANEL_v410;

typedef struct tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410{
	tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_INFO_PANEL_v410 stPlayerPanel; //人物面板信息
	DT_BAG_DATA_CLI2 stBagInfo; //背包基本信息
	UINT16           wCoachExpRate; //主角经验倍数(百分比)
	DT_JEWELRY_SUIT_LIST_CLI stHeroJewelrySuitInfo; //每个伙伴身上的饰品套装信息
	DT_HERO_LST_SOUL_POTENCY_DATA_LST_CLI stHeroSoulPotencyInfo; //每个伙伴身上的战魂潜能信息
	DT_SOUL_POTENCY_DATA_LST_CLI stBagSoulPotencyInfo; //背包的战魂潜能信息
}PKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410;

typedef struct tagPKT_CLIGS_PLAYER_INFO_AWAKEN_REQ{
	tagPKT_CLIGS_PLAYER_INFO_AWAKEN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_AWAKEN_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PLAYER_INFO_AWAKEN_REQ;

typedef struct tagPKT_CLIGS_PLAYER_INFO_AWAKEN_ACK{
	tagPKT_CLIGS_PLAYER_INFO_AWAKEN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_AWAKEN_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           awStudyUnlockLevelInfo[MAX_HERO_STUDY_GRID_NUM]; //心得解锁等级信息
	UINT8            byHeroNum; //武将个数
	DT_HERO_STUDY_DATA_CLI2 astHeroStudyInfo[MAX_HERO_NUM]; //武将心得信息
	UINT16           awHeroLevel[MAX_HERO_NUM]; //武将等级
	UINT8            byAwakenOpenState; //心得解锁标识(0未解锁，1 解锁)
}PKT_CLIGS_PLAYER_INFO_AWAKEN_ACK;

typedef struct tagPKT_CLIGS_PLAYER_INFO_SKILL_REQ{
	tagPKT_CLIGS_PLAYER_INFO_SKILL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_SKILL_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PLAYER_INFO_SKILL_REQ;

typedef struct tagPKT_CLIGS_PLAYER_INFO_SKILL_ACK{
	tagPKT_CLIGS_PLAYER_INFO_SKILL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_SKILL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byHeroNum; //武将个数
	DT_SKILL_DATA_LST_CLI astSkillInfo[MAX_HERO_NUM]; //武将技能信息
}PKT_CLIGS_PLAYER_INFO_SKILL_ACK;

typedef struct tagPKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ{
	tagPKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ;

typedef struct tagPKT_CLIGS_PLAYER_INFO_GODWEAPON_ACK{
	tagPKT_CLIGS_PLAYER_INFO_GODWEAPON_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_GODWEAPON_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wGodweaponNum; //神器数目
	DT_GODWEAPON_DATA_CLI astGodweaponLst[MAX_HERO_NUM]; //神器信息
	UINT8            abyLockStatu[MAX_HERO_NUM]; //解锁状态(0未解锁， 1解锁)
}PKT_CLIGS_PLAYER_INFO_GODWEAPON_ACK;

typedef struct tagPKT_CLIGS_PLAYER_INFO_JEWELRY_REQ{
	tagPKT_CLIGS_PLAYER_INFO_JEWELRY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_JEWELRY_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PLAYER_INFO_JEWELRY_REQ;

typedef struct tagPKT_CLIGS_PLAYER_INFO_JEWELRY_ACK{
	tagPKT_CLIGS_PLAYER_INFO_JEWELRY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_JEWELRY_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byHeroNum; //武将个数
	DT_HERO_JEWELRY_DATA_LST_CLI astJewelryInfo[MAX_HERO_NUM]; //武将饰品信息-
	DT_JEWELRY_SUIT_CLI astJewelrySuit[MAX_HERO_NUM]; //武将饰品信息-
}PKT_CLIGS_PLAYER_INFO_JEWELRY_ACK;

typedef struct tagPKT_CLIGS_PLAYER_INFO_SOUL_REQ{
	tagPKT_CLIGS_PLAYER_INFO_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_SOUL_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PLAYER_INFO_SOUL_REQ;

typedef struct tagPKT_CLIGS_PLAYER_INFO_SOUL_ACK{
	tagPKT_CLIGS_PLAYER_INFO_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byHeroNum; //武将个数
	DT_HERO_SOUL_DATA_LST_CLI astSoulInfo[MAX_HERO_NUM]; //战魂信息
	DT_HERO_SOUL_POTENCY_DATA_LST_CLI astSoulPotencyInfo[MAX_HERO_NUM]; //战魂潜能信息
}PKT_CLIGS_PLAYER_INFO_SOUL_ACK;

typedef struct tagPKT_CLIGS_DRESS_JEWELRY_REQ{
	tagPKT_CLIGS_DRESS_JEWELRY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_JEWELRY_REQ)); }
	UINT16           wHeroKindID; //要穿戴的武将
	UINT8            byDressType; //穿戴方式(装备/卸下/替换)(commondef.h EDressType)
	UINT16           wDressJewelryID; //要穿戴的饰品ID
	UINT16           wDressItemIdx; //要穿戴的装备序号
	UINT16           wUndressJewelryID; //要卸下的饰品ID(卸下，替换时有效)
	UINT16           wUndressItemIdx; //要卸下的装备序号(卸下，替换时有效)
	UINT8            byReqPos; //请求位置（0 人物面板, 1合成面板）
}PKT_CLIGS_DRESS_JEWELRY_REQ;

typedef struct tagPKT_CLIGS_DRESS_JEWELRY_ACK{
	tagPKT_CLIGS_DRESS_JEWELRY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_JEWELRY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //要穿戴的武将
	UINT8            byDressType; //穿戴方式(装备/卸下/替换)
	UINT16           wDressJewelryID; //要穿戴的装备类型
	UINT16           wDressItemIdx; //要穿戴的装备序号
	UINT16           wUndressJewelryID; //要卸下的装备类型(卸下，替换时有效)
	UINT16           wUndressItemIdx; //要卸下的装备序号(卸下，替换时有效)
	UINT32           dwPlayerHP; //玩家总血量
	DT_POWER_CHANGE_DATA stPowerInfo; //玩家战力信息
	DT_HERO_PANEL_LST_CLI stHeroPanelInfo; //武将的面板信息
	UINT8            byIsDown; //是否下发0 不下发， 1下发
	DT_JEWELRY_DATA_LST_CLI astJewelryInfo[1]; //佩饰信息
	DT_FUNC_PROMPT_DATA stBuildPrompt; //建筑提示
	DT_JEWELRY_SUIT_CLI stJewelrySuitInfo; //佩饰信息
}PKT_CLIGS_DRESS_JEWELRY_ACK;

typedef struct tagPKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ{
	tagPKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ;

typedef struct tagPKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK{
	tagPKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wHeroNum; //武将数目
	DT_HERO_BASE_DATA_CLI astHeroBaseData[MAX_HERO_NUM]; //武将基本信息
	UINT8            abyFlyFlagList[MAX_HERO_NUM]; //是否可飞升数组：可飞升true/不可飞升false
}PKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK;

typedef struct tagPKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ{
	tagPKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ;

typedef struct tagPKT_CLIGS_OPEN_JEWELRY_GENERATE_ACK{
	tagPKT_CLIGS_OPEN_JEWELRY_GENERATE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JEWELRY_GENERATE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_JEWELRY_DATA_LST_CLI stJewelryInfo; //佩饰信息
	DT_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_OPEN_JEWELRY_GENERATE_ACK;

typedef struct tagPKT_CLIGS_JEWELRY_GENERATE_REQ{
	tagPKT_CLIGS_JEWELRY_GENERATE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_GENERATE_REQ)); }
	UINT16           wFirstID; //第一个饰品ID（值为0时没有物品）
	UINT16           wFirstIdx; //第一个饰品Idx
	UINT16           wSecondID; //第二个饰品ID
	UINT16           wSecondIdx; //第二个饰品Idx
	UINT16           wThirdID; //第三个饰品ID
	UINT16           wThirdIdx; //第三个饰品Idx
	UINT16           wFourthID; //第四个饰品ID
	UINT16           wFourthIdx; //第四个饰品Idx
}PKT_CLIGS_JEWELRY_GENERATE_REQ;

typedef struct tagPKT_CLIGS_JEWELRY_GENERATE_ACK{
	tagPKT_CLIGS_JEWELRY_GENERATE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_GENERATE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_JEWELRY_DATA_CLI stJewelryInfo; //生成饰品的信息
	DT_JEWELRY_DATA_LST_CLI stBagJewelryInfo; //同步背包饰品信息
	DT_RSYNC_RES_ITEM_DATA stResourceSnyc; //资源同步信息
	DT_RES_ITEM_DATA stResInfo; //资源信息
	DT_JEWELRY_DATA_CLI stCompareJewelry; //对比的饰品(穿戴在伙伴身上)
}PKT_CLIGS_JEWELRY_GENERATE_ACK;

typedef struct tagDT_STRENGTH_INFO{
	UINT16           wItemID; //道具ID
	UINT32           dwCostItemNum; //道具个数
	UINT32           dwCurItemNum; //道具当前个数
	DT_PLAYER_RES_DATA stCostRes; //物品购买消耗(单价)
}DT_STRENGTH_INFO;

typedef struct tagDT_STRENGTH_LST_INFO{
	UINT8            byItemKindNum; //道具种类个数
	DT_STRENGTH_INFO astItemInfo[MAX_JEWELRY_STRENG_NUM]; //强化道具信息
}DT_STRENGTH_LST_INFO;

typedef struct tagPKT_CLIGS_JEWELRY_STRENGTH_REQ{
	tagPKT_CLIGS_JEWELRY_STRENGTH_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_STRENGTH_REQ)); }
	UINT16           wJewelryID; //第一个饰品ID（值为0时没有物品）
	UINT16           wJewelryIdx; //第一个饰品Idx
}PKT_CLIGS_JEWELRY_STRENGTH_REQ;

typedef struct tagPKT_CLIGS_JEWELRY_STRENGTH_ACK{
	tagPKT_CLIGS_JEWELRY_STRENGTH_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_STRENGTH_ACK)); }
	UINT16           wErrCode; //错误码
	DT_JEWELRY_DATA_CLI stAfterJewelryInfo; //强化后的信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
	DT_JEWELRY_DATA_CLI stStrengthJewelry; //强化后的饰品信息列表
	DT_STRENGTH_INFO stStringth; //强化消耗
	UINT8            byMaxMark; //强化上限标识（0未达到上限， 1已达到上限）
	DT_FUNC_PROMPT_DATA stBuildPrompt; //建筑提示
	DT_STRENGTH_LST_INFO stJewelryStrengResSync; //饰品精练石同步
}PKT_CLIGS_JEWELRY_STRENGTH_ACK;

typedef struct tagPKT_CLIGS_JEWELRY_GENERATE_INFO_REQ{
	tagPKT_CLIGS_JEWELRY_GENERATE_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_GENERATE_INFO_REQ)); }
	UINT16           wFirstID; //第一个饰品ID（值为0时没有物品）
	UINT16           wFirstIdx; //第一个饰品Idx
	UINT16           wSecondID; //第二个饰品ID
	UINT16           wSecondIdx; //第二个饰品Idx
	UINT16           wThirdID; //第三个饰品ID
	UINT16           wThirdIdx; //第三个饰品Idx
	UINT16           wFourthID; //第四个饰品ID
	UINT16           wFourthIdx; //第四个饰品Idx
}PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ;

typedef struct tagPKT_CLIGS_JEWELRY_GENERATE_INFO_ACK{
	tagPKT_CLIGS_JEWELRY_GENERATE_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_GENERATE_INFO_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wGenerateLevel; //合成的等级
	UINT8            byGenerateType; //合成的品质
	DT_RES_DATA      stGenerateCost; //合成消耗
	UINT8            byState; //状态位（0正常， 1含有强化过的饰品， 2铜钱不足， 3饰品没放全）
	DT_RSYNC_RES_ITEM_DATA stResourceSnyc; //资源同步信息
}PKT_CLIGS_JEWELRY_GENERATE_INFO_ACK;

typedef struct tagPKT_CLIGS_JEWELRY_RESOLVE_REQ{
	tagPKT_CLIGS_JEWELRY_RESOLVE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_RESOLVE_REQ)); }
	UINT16           wJewelryID; //饰品ID
	UINT16           wJewelryIdx; //饰品idx
	UINT8            byBagKind; //请求位置（1背包， 2饰品背包， 3饰品合成）
}PKT_CLIGS_JEWELRY_RESOLVE_REQ;

typedef struct tagPKT_CLIGS_JEWELRY_RESOLVE_ACK{
	tagPKT_CLIGS_JEWELRY_RESOLVE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_RESOLVE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GOODS_DATA_LST_CLI stItemInfo; //分散出的道具信息
	DT_ITEM_DATA_LIST_CLI3 stJewelryInfo; //背包饰品信息
	DT_RSYNC_RES_ITEM_DATA stResourceSnyc; //资源同步信息
}PKT_CLIGS_JEWELRY_RESOLVE_ACK;

typedef struct tagPKT_CLIGS_JEWELRY_RESOLVE_ACK2{
	tagPKT_CLIGS_JEWELRY_RESOLVE_ACK2() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_RESOLVE_ACK2)); }
	UINT16           wErrCode; //错误码
	DT_GOODS_DATA_LST_CLI stItemInfo; //分散出的道具信息
	DT_RSYNC_RES_ITEM_DATA stResourceSnyc; //资源同步信息
}PKT_CLIGS_JEWELRY_RESOLVE_ACK2;

typedef struct tagPKT_CLIGS_PLAYER_INFO_EQUIP_REQ{
	tagPKT_CLIGS_PLAYER_INFO_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_EQUIP_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PLAYER_INFO_EQUIP_REQ;

typedef struct tagPKT_CLIGS_PLAYER_INFO_EQUIP_ACK{
	tagPKT_CLIGS_PLAYER_INFO_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_EQUIP_ACK)); }
	UINT16           wErrCode; //错误码
	UINT32           dwPlayerID; //玩家ID
	UINT8            byHeroNum; //武将个数
	DT_HERO_EQUIP_DATA_LST_CLI astEquipDataList[MAX_HERO_NUM]; //武将装备信息
	DT_ALL_EQUIP_XIANGQIAN_DES stEquipXiangqianDes; //装备镶嵌信息描述
	UINT8            byVipLevel; //VIP等级
	DT_HERO_BASE_DATA_CLI astHeroBaseData[MAX_HERO_NUM]; //武将基本信息
}PKT_CLIGS_PLAYER_INFO_EQUIP_ACK;

typedef struct tagPKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ{
	tagPKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ)); }
	UINT16           wJewelryID; //饰品ID
	UINT16           wJewelryIdx; //饰品idx
}PKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ;

typedef struct tagDT_HERO_JEWELRY_STRENGTH_INFO{
	UINT16           wHeroID; //伙伴ID
	DT_JEWELRY_DATA_CLI astJewelryList[MAX_JEWELRY_POS]; //饰品信息
	DT_JEWELRY_DATA_CLI astStrengthJewelryList[MAX_JEWELRY_POS]; //强化后的饰品信息列表
	DT_STRENGTH_INFO astStringth[MAX_JEWELRY_POS]; //强化消耗
	UINT8            abyMaxMark[MAX_JEWELRY_POS]; //强化上限标识（0未达到上限， 1已达到上限）
}DT_HERO_JEWELRY_STRENGTH_INFO;

typedef struct tagDT_BAG_JEWELRY_STRENGTH_INFO{
	UINT8            byJewelryNum; //饰品个数
	DT_JEWELRY_DATA_CLI astJewelryList[MAX_BAG_OPEN_NUM]; //饰品信息
	DT_JEWELRY_DATA_CLI astStrengthJewelryList[MAX_BAG_OPEN_NUM]; //强化后的饰品信息列表
	DT_STRENGTH_INFO astStringth[MAX_BAG_OPEN_NUM]; //强化消耗
	UINT8            abyMaxMark[MAX_BAG_OPEN_NUM]; //强化上限标识（0未达到上限， 1已达到上限）
}DT_BAG_JEWELRY_STRENGTH_INFO;

typedef struct tagPKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK{
	tagPKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byHeroNum; //伙伴个数
	DT_HERO_JEWELRY_STRENGTH_INFO astHeroJewelryStrengInfo[MAX_HERO_NUM]; //伙伴饰品强化信息
	DT_BAG_JEWELRY_STRENGTH_INFO stBagJewelryStrengInfo; //背包饰品强化信息
	DT_PLAYER_RES_LST_DATA stPlayerRes; //铜钱元宝
}PKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK;

typedef struct tagPKT_CLIGS_JEWELRY_COMPARE_REQ{
	tagPKT_CLIGS_JEWELRY_COMPARE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_COMPARE_REQ)); }
	UINT16           wJewelryID; //对比饰品ID
	UINT16           wJewelryIdx; //对比饰品Idx
}PKT_CLIGS_JEWELRY_COMPARE_REQ;

typedef struct tagPKT_CLIGS_JEWELRY_COMPARE_ACK{
	tagPKT_CLIGS_JEWELRY_COMPARE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_COMPARE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_JEWELRY_DATA_CLI stDressJewelryInfo; //穿戴饰品
	DT_JEWELRY_DATA_CLI stCompareJewelryInfo; //对比饰品
}PKT_CLIGS_JEWELRY_COMPARE_ACK;

typedef struct tagPKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ{
	tagPKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ)); }
	UINT16           wJewelryID; //饰品ID
	UINT16           wJewelryIdx; //饰品Idx
	UINT8            byExtID; //要强化附加属性ID
}PKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ;

typedef struct tagPKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK{
	tagPKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_JEWELRY_DATA_CLI stJewelryInfo; //饰品信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
	DT_JEWELRY_DATA_CLI stStrengthJewelry; //强化后的饰品信息列表
	UINT8            byMaxMark; //强化上限标识（0未达到上限， 1已达到上限）
	DT_FUNC_PROMPT_DATA stBuildPrompt; //建筑提示
	DT_STRENGTH_INFO stStrengInfo; //强化消耗
}PKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK;

typedef struct tagPKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ{
	tagPKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ)); }
	UINT16           wJewelryID; //饰品ID
	UINT16           wJewelryIdx; //饰品Idx
}PKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ;

typedef struct tagPKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK{
	tagPKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_STRENGTH_LST_INFO stBaseItem; //基本道具(随机)
	DT_STRENGTH_LST_INFO stReturnItem; //返回道具（100%）
	DT_PLAYER_RES_DATA stCostRes; //消耗
}PKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK;

typedef struct tagPKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF{
	tagPKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF;

typedef struct tagDT_GVG_FORMATION_PLAYER_ID_LST_CLI{
	UINT8            byPlayerNum; //上阵列人数
	UINT32           adwPlayerIDForFormation[DT_FACTION_PLAYER_NUM]; //上阵玩家ID（按顺序下发）
}DT_GVG_FORMATION_PLAYER_ID_LST_CLI;

typedef struct tagDT_GVG_FORMATION_HERO_LST_INFO{
	UINT8            byHeroNum; //上阵列人数
	UINT32           adwHeroID[MAX_FORMATION_IDX_NUM]; //上阵伙伴ID（按顺序下发）
	UINT8            abyHeroCareerID[MAX_FORMATION_IDX_NUM]; //-上阵伙伴职业
}DT_GVG_FORMATION_HERO_LST_INFO;

typedef struct tagDT_GVG_FORMATION_PLAYER_DATA_CLI{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispPlayerName[USERNAME_LEN]; //玩家昵称
	UINT16           wLevel; //玩家等级
	UINT32           dwCoachID; //玩家主将ID
	UINT32           dwPower; //战力
	UINT8            byPlayerPosition; //在门派职位 commondef (em_Faction_Job)
	UINT32           dwDoorsTributeLevel; //门贡等级
	DT_GVG_FORMATION_HERO_LST_INFO stFrontRow; //前排
	DT_GVG_FORMATION_HERO_LST_INFO stBackRow; //后排
	UINT8            byColor; //主角颜色
}DT_GVG_FORMATION_PLAYER_DATA_CLI;

typedef struct tagDT_GVG_FORMATION_SIMPLE_PLAYER_DATA_CLI{
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszDispPlayerName[USERNAME_LEN]; //玩家昵称
	UINT32           dwCoachID; //玩家主将ID
	UINT32           dwPower; //战力
	UINT8            byPosIdx; //位置信息(0未上阵列)
	UINT8            byJobType; //玩家职位
}DT_GVG_FORMATION_SIMPLE_PLAYER_DATA_CLI;

typedef struct tagDT_GVG_FORMATION_SIMPLE_PLAYER_DATA_LST_CLI{
	UINT8            byPlayerNum; //可选玩家个数
	DT_GVG_FORMATION_SIMPLE_PLAYER_DATA_CLI astPlayerInfo[DT_FACTION_PLAYER_NUM]; //玩家信息
}DT_GVG_FORMATION_SIMPLE_PLAYER_DATA_LST_CLI;

typedef struct tagDT_FACTION_FORMATION_POS_INFO{
	UINT8            byLockState; //解锁状态（0未解锁，1解锁）
	UINT16           wNeedFactionLevel; //解锁需要的门派等级
}DT_FACTION_FORMATION_POS_INFO;

typedef struct tagDT_FACTION_FORMATION_POS_LST_INFO{
	UINT8            byPosNum; //位置数
	DT_FACTION_FORMATION_POS_INFO astFormationPosInfo[MAX_GVG_FORMATION_PLAYER_CNT]; //阵位信息
	UINT8            byUnlockPosNum; //解锁的位置数
}DT_FACTION_FORMATION_POS_LST_INFO;

typedef struct tagPKT_CLIGS_OPEN_GVG_FORMATION_REQ{
	tagPKT_CLIGS_OPEN_GVG_FORMATION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_FORMATION_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_GVG_FORMATION_REQ;

typedef struct tagPKT_CLIGS_OPEN_GVG_FORMATION_ACK{
	tagPKT_CLIGS_OPEN_GVG_FORMATION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_FORMATION_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_FORMATION_SIMPLE_PLAYER_DATA_LST_CLI stPlayerLstInfo; //玩家信息列表
	UINT16           wCurFactionLevel; //当前门派等级
	DT_FACTION_FORMATION_POS_LST_INFO stFormationInfo; //阵型相关信息（解锁等）
}PKT_CLIGS_OPEN_GVG_FORMATION_ACK;

typedef struct tagDT_GVG_TIME_INFO{
	UINT8            byStartWeek; //开始星期（0不限星期，1星期一，7星期天）
	UINT32           dwStartTime; //开始具体时间(秒)
	UINT8            byEndWeek; //结束星期（0不限星期，1星期一，7星期天）
	UINT32           dwEndTime; //结束具体时间(秒)
}DT_GVG_TIME_INFO;

typedef struct tagDT_GVG_SCHEDULE_INFO{
	DT_GVG_TIME_INFO stApply; //报名时间
	DT_GVG_TIME_INFO stMoneyGame; //本服一轮（财力大比拼）
	DT_GVG_TIME_INFO stChaosWar; //本服二轮（大乱斗）
	DT_GVG_TIME_INFO stDouble16TO8; //双服16进8
	DT_GVG_TIME_INFO stDouble8TO4; //双服8进4
	DT_GVG_TIME_INFO stFinalWar; //总决赛
	UINT8            byCurRound; //当前轮次，见 commondef.h 的 GvGRound
	UINT32           dwRefreshTime; //距离下次刷新面板的间隔(秒)，时间到了则重新刷新整个面板
	UINT8            byGetGiftNum; //可领取礼包数
	UINT16           wSessionID; //第几届门派
	UINT8            byUnReadLogCnt; //未读本派战报条数
	UINT8            byFormationState; //布阵标识(0正常，1没有门派 2未报名，3比赛即将开始，4没权限， 5比赛进行中， 6本届跨服战已结束)
}DT_GVG_SCHEDULE_INFO;

typedef struct tagPKT_CLIGS_GVG_APPLAY_ACK{
	tagPKT_CLIGS_GVG_APPLAY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_APPLAY_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_SCHEDULE_INFO stScheduleInfo; //跨服战进度信息
	DT_GVG_FACTION_RANK_DATA stLastChampionInfo; //上届冠军信息
	UINT8            byIsApplyTime; //当前是否处于报名时段
	UINT32           dwCountDown; //距离报名结束的倒计时
}PKT_CLIGS_GVG_APPLAY_ACK;

typedef struct tagDT_GVG_RANK_MONEY_DATA{
	UINT16           wRank; //排名
	UINT64           qwFactionFunds; //资金
	UINT64           qwSumPower; //总战力
	DT_FACTION_BASE_DATA_CLI2 stFactionBaseInfo; //门派基础数据
}DT_GVG_RANK_MONEY_DATA;

typedef struct tagDT_GVG_SIMPLE_RANK_MONEY_DATA{
	UINT16           wRank; //排名
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT64           qwFactionFunds; //累积资金
}DT_GVG_SIMPLE_RANK_MONEY_DATA;

typedef struct tagDT_GVG_RANK_MONEY_LST_DATA{
	UINT16           wRankNum; //排名个数
	DT_GVG_RANK_MONEY_DATA astMoneyRank[MAX_RANK_DISPLAY]; //排名数据
}DT_GVG_RANK_MONEY_LST_DATA;

typedef struct tagDT_GVG_SIMPLE_RANK_MONEY_LST_DATA{
	UINT16           wRankNum; //排名个数
	DT_GVG_SIMPLE_RANK_MONEY_DATA astMoneyRank[MAX_RANK_DISPLAY]; //排名数据
}DT_GVG_SIMPLE_RANK_MONEY_LST_DATA;

typedef struct tagPKT_CLIGS_OPEN_ALREAD_APPLAY_REQ{
	tagPKT_CLIGS_OPEN_ALREAD_APPLAY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ALREAD_APPLAY_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_ALREAD_APPLAY_REQ;

typedef struct tagPKT_CLIGS_OPEN_ALREAD_APPLAY_ACK{
	tagPKT_CLIGS_OPEN_ALREAD_APPLAY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ALREAD_APPLAY_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_RANK_MONEY_LST_DATA stApplyInfo; //名单
}PKT_CLIGS_OPEN_ALREAD_APPLAY_ACK;

typedef struct tagPKT_CLIGS_GVG_LAST_RESULT_REQ{
	tagPKT_CLIGS_GVG_LAST_RESULT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_LAST_RESULT_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_GVG_LAST_RESULT_REQ;

typedef struct tagPKT_CLIGS_GVG_LAST_RESULT_ACK{
	tagPKT_CLIGS_GVG_LAST_RESULT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_LAST_RESULT_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_FACTION_RANK_LST_DATA stLastResult; //上届排名
}PKT_CLIGS_GVG_LAST_RESULT_ACK;

typedef struct tagPKT_CLIGS_GVG_MONEY_CHAOS_ACK{
	tagPKT_CLIGS_GVG_MONEY_CHAOS_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_MONEY_CHAOS_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_SCHEDULE_INFO stScheduleInfo; //跨服战进度信息
	DT_GVG_SIMPLE_RANK_MONEY_LST_DATA stMoneyRank; //财力信息
	DT_SIMPLE_FACTION_LST_INFO stBattleInfo; //大乱斗
	DT_SIMPLE_FACTION_LST_INFO stMoneyInfo; //财力比拼
	UINT64           qwCountDwonTime; //倒计时
	UINT8            byCurPanel; //当前面板（1财力大比拼， 2大乱斗进行中，3跨服倒计时）
	UINT8            bySmallRound; //当前小轮次
}PKT_CLIGS_GVG_MONEY_CHAOS_ACK;

typedef struct tagPKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ{
	tagPKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ)); }
	UINT16           wExt; //扩展位
}PKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ;

typedef struct tagDT_GAMBLE_GAME_INFO{
	DT_GVG_ROUND     stGameKind; //比赛类别 commondef (GvGFightLogStage)
	UINT8            byResult; //押注结果0负，1胜，2还未进行, 3未押注 commondef.h GvGGambleResult
	DT_GAMBLE_FACTION_INFO stFactionInfo1; //门派1
	DT_GAMBLE_FACTION_INFO stFactionInfo2; //门派2
	UINT64           qwVideoID; //录像ID
}DT_GAMBLE_GAME_INFO;

typedef struct tagDT_GAMBLE_GAME_LST_INFO{
	UINT8            byGambleNum; //押注场数
	DT_GAMBLE_GAME_INFO astGameInfo[MAX_GAMBLE_NUM]; //比赛信息
}DT_GAMBLE_GAME_LST_INFO;

typedef struct tagDT_GAMBLE_GIFT_DATA{
	UINT8            byGambleType; //押注结果（0负，1胜，2还未进行, 3未押注 commondef.h GvGGambleResult）
	DT_PLAYER_RES_DATA stGambleReward; //押注奖励
}DT_GAMBLE_GIFT_DATA;

typedef struct tagPKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK{
	tagPKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_RES_DATA stWinRes; //已获取的收益
	DT_GAMBLE_GIFT_DATA astGambleInfo[MAX_GAMBLE_NUM]; //押注信息（0负，1胜，2还未进行, 3未押注 commondef.h GvGGambleResult）
	DT_GAMBLE_GAME_LST_INFO stGambleGameInfo; //押注的比赛信息
}PKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK;

typedef struct tagPKT_CLIGS_GVG_OPEN_GAMBLE_REQ{
	tagPKT_CLIGS_GVG_OPEN_GAMBLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_OPEN_GAMBLE_REQ)); }
	UINT64           qwGambleID; //录像ID
}PKT_CLIGS_GVG_OPEN_GAMBLE_REQ;

typedef struct tagDT_FACTION_GAMBLE_INFO{
	UINT32           dwFactionID; //门派ID
	UINT16           wZoneID; //区ID
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	TCHAR            aszHeadName[USERNAME_LEN]; //掌门名称
	UINT16           wJoinNum; //参战人数
	UINT64           qwSumPower; //总战力
	UINT32           dwSumIntegral; //决赛积分
	UINT32           dwSupportNum; //支持人数
	UINT8            byIconID; //门派头像ID
}DT_FACTION_GAMBLE_INFO;

typedef struct tagPKT_CLIGS_GVG_OPEN_GAMBLE_ACK{
	tagPKT_CLIGS_GVG_OPEN_GAMBLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_OPEN_GAMBLE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_RES_DATA stWinRes; //胜利收益
	DT_FACTION_GAMBLE_INFO stFactionInfo1; //门派1
	DT_FACTION_GAMBLE_INFO stFactionInfo2; //门派2
	UINT64           qwVideoID; //录像ID
}PKT_CLIGS_GVG_OPEN_GAMBLE_ACK;

typedef struct tagPKT_CLIGS_OPEN_GVG_RULE_REQ{
	tagPKT_CLIGS_OPEN_GVG_RULE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_RULE_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_GVG_RULE_REQ;

typedef struct tagPKT_CLIGS_OPEN_GVG_RULE_ACK{
	tagPKT_CLIGS_OPEN_GVG_RULE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_RULE_ACK)); }
	UINT16           wErrCode; //错误码
	TCHAR            aszContent[MAX_GVG_RULE]; //内容
}PKT_CLIGS_OPEN_GVG_RULE_ACK;

typedef struct tagPKT_CLIGS_GVG_GAMBLE_REQ{
	tagPKT_CLIGS_GVG_GAMBLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_GAMBLE_REQ)); }
	UINT32           dwWinFactionID; //押注的门派ID
	UINT64           qwGambleID; //录像ID
}PKT_CLIGS_GVG_GAMBLE_REQ;

typedef struct tagPKT_CLIGS_GVG_GAMBLE_ACK{
	tagPKT_CLIGS_GVG_GAMBLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_GAMBLE_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIGS_GVG_GAMBLE_ACK;

typedef struct tagPKT_CLIGS_OPEN_GVG_ACTIVITY_REQ{
	tagPKT_CLIGS_OPEN_GVG_ACTIVITY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_ACTIVITY_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_GVG_ACTIVITY_REQ;

typedef struct tagPKT_CLIGS_OPEN_FACTION_MONEY_RANK_REQ{
	tagPKT_CLIGS_OPEN_FACTION_MONEY_RANK_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_MONEY_RANK_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_FACTION_MONEY_RANK_REQ;

typedef struct tagPKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK{
	tagPKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_RANK_MONEY_LST_DATA stMoneyRank; //财力信息
}PKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK;

typedef struct tagPKT_CLIGS_READ_FORMATION_PLAYER_TIP_REQ{
	tagPKT_CLIGS_READ_FORMATION_PLAYER_TIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_READ_FORMATION_PLAYER_TIP_REQ)); }
	UINT32           dwReqPlayerID; //请求的玩家ID
}PKT_CLIGS_READ_FORMATION_PLAYER_TIP_REQ;

typedef struct tagPKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK{
	tagPKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_FORMATION_PLAYER_DATA_CLI stTipInfo; //玩家TIP信息
}PKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK;

typedef struct tagPKT_CLIGS_GVG_FACTION_EMBATTLE_REQ{
	tagPKT_CLIGS_GVG_FACTION_EMBATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_FACTION_EMBATTLE_REQ)); }
	DT_GVG_FORMATION_PLAYER_ID_LST_CLI stFormation; //阵型信息
}PKT_CLIGS_GVG_FACTION_EMBATTLE_REQ;

typedef struct tagPKT_CLIGS_GVG_FACTION_EMBATTLE_ACK{
	tagPKT_CLIGS_GVG_FACTION_EMBATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_FACTION_EMBATTLE_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIGS_GVG_FACTION_EMBATTLE_ACK;

typedef struct tagDT_GIFT_DETAILS_INFO{
	UINT8            byKindType; //资源类型（commondef.h ACTE_KIND_TYPE）
	UINT16           wKindID; //资源ID（类型为物品时有效,Value为物品ID）
	UINT32           dwKindNum; //资源个数
}DT_GIFT_DETAILS_INFO;

typedef struct tagDT_GIFT_DETAILS_LST_INFO{
	UINT8            byGoodsNum; //物品个数
	DT_GIFT_DETAILS_INFO astGoodsInfo[MAX_GIFT_GOODS_NUM]; //物品信息
}DT_GIFT_DETAILS_LST_INFO;

typedef struct tagDT_GVG_GIFT_DATA_CLI{
	UINT16           wSessionID; //第几届跨服门派战
	UINT8            byGvGRound; //跨战轮次（commondef.h GvGRound）
	UINT8            byBattleIdx; //当前轮次中的第几场战斗
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //对战门派名称
	UINT8            byBattleResult; //对战结果（0输，1胜利）
	DT_GIFT_DETAILS_LST_INFO stGiftInfo; //礼包内容
	UINT8            byGiftGetState; //礼包领取状态（0未领取， 1领取）
	UINT64           qwGiftID; //礼包ID
	UINT8            byGiftType; //礼包类别（GvGiftType）
	UINT8            byRank; //排名
}DT_GVG_GIFT_DATA_CLI;

typedef struct tagDT_GVG_GIFT_DATA_LST_CLI{
	UINT8            byGiftNum; //礼包数
	DT_GVG_GIFT_DATA_CLI astGiftInfo[MAX_GVG_GIFT_NUM]; //礼包信息
}DT_GVG_GIFT_DATA_LST_CLI;

typedef struct tagPKT_CLIGS_OPEN_GVG_GIFT_REQ{
	tagPKT_CLIGS_OPEN_GVG_GIFT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_GIFT_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_GVG_GIFT_REQ;

typedef struct tagPKT_CLIGS_OPEN_GVG_GIFT_ACK{
	tagPKT_CLIGS_OPEN_GVG_GIFT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_GIFT_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_GIFT_DATA_LST_CLI stGiftLstInfo; //礼包信息
}PKT_CLIGS_OPEN_GVG_GIFT_ACK;

typedef struct tagPKT_CLIGS_GET_GVG_GIFT_REQ{
	tagPKT_CLIGS_GET_GVG_GIFT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_GVG_GIFT_REQ)); }
	UINT64           qwGiftID; //礼包ID(0为全部领取)
}PKT_CLIGS_GET_GVG_GIFT_REQ;

typedef struct tagPKT_CLIGS_GET_GVG_GIFT_ACK{
	tagPKT_CLIGS_GET_GVG_GIFT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_GVG_GIFT_ACK)); }
	UINT16           wErrCode; //错误码
	DT_RSYNC_RES_ITEM_DATA stSyncInfo; //信息同步（不同步物品）
	UINT64           qwGiftID; //礼包ID
	UINT8            byLeftGiftNum; //未领取礼包数
}PKT_CLIGS_GET_GVG_GIFT_ACK;

typedef struct tagPKT_CLIGS_APPLY_GVG_REQ{
	tagPKT_CLIGS_APPLY_GVG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_APPLY_GVG_REQ)); }
	UINT8            byExt; //空字段
}PKT_CLIGS_APPLY_GVG_REQ;

typedef struct tagPKT_CLIGS_APPLY_GVG_ACK{
	tagPKT_CLIGS_APPLY_GVG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_APPLY_GVG_ACK)); }
	UINT8            byExt; //空字段
}PKT_CLIGS_APPLY_GVG_ACK;

typedef struct tagDT_FINAL_WAR_FACTION_INFO_CLI{
	UINT8            byRank; //排名
	UINT32           dwFactionID; //门派ID
	UINT32           dwZoneID; //区ID
	UINT16           wFactionIconID; //门派图标
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	UINT16           wFactionLevel; //门派等级
	UINT64           qwSumPower; //总战力
	UINT16           wJoinNum; //参战人数
	UINT32           dwScore; //积分
	UINT64           qwVideoID; //录像ID
	TCHAR            aszZoneName[ZONENAME_LEN]; //门派所在的区名
}DT_FINAL_WAR_FACTION_INFO_CLI;

typedef struct tagPKT_CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF{
	tagPKT_CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF)); }
	UINT8            byUnReadCnt; //还剩多少条门派战报未读
}PKT_CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF;

typedef struct tagPKT_CLIGS_OPEN_GVG_LOG_REQ{
	tagPKT_CLIGS_OPEN_GVG_LOG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_LOG_REQ)); }
	UINT32           dwFactionID; //目标门派ID，0代表打开本门战况
}PKT_CLIGS_OPEN_GVG_LOG_REQ;

typedef struct tagPKT_CLIGS_OPEN_GVG_LOG_ACK{
	tagPKT_CLIGS_OPEN_GVG_LOG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GVG_LOG_ACK)); }
	UINT16           wErrCode; //错误码
	DT_FACTION_GVG_LOG stLog; //门派战况
	UINT32           dwRefreshTime; //距离下次刷新面板的间隔(秒)，时间到了则重新刷新门派战况面板
}PKT_CLIGS_OPEN_GVG_LOG_ACK;

typedef struct tagPKT_CLIGS_16_TO_8_TO_4_ACK{
	tagPKT_CLIGS_16_TO_8_TO_4_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_16_TO_8_TO_4_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_SCHEDULE_INFO stScheduleInfo; //跨服战进度信息
	UINT8            byNextRound; //即将开始的轮次：16进8、8进4、积分总决赛，见commondef.h的GvGRound
	UINT32           dwCountDownTime; //下一轮倒计时(秒)
	DT_FACTION_BASE_DATA_CLI2 astFactionList[GVG_16_TO_8_FACTION_CNT]; //双服前16名门派选手列表
	UINT8            byWin8Cnt; //16进8的胜出门派数
	UINT8            abyWin8List[GVG_16_TO_8_WIN_CNT]; //16进8的胜出门派序号列表(序号:1-16)
	UINT8            byWin4Cnt; //8进4的胜出门派数
	UINT8            abyWin4List[GVG_8_TO_4_WIN_CNT]; //8进4的胜出门派序号列表(序号:1-16)
	UINT64           aqwVideoIDList16To8[GVG_16_TO_8_WIN_CNT]; //16进8的录像ID列表(在未决出胜者前用作押注ID，此时押注ID为0则表示无法押注，决出胜者后用作播放录像的ID)
	UINT64           aqwVideoIDList8To4[GVG_8_TO_4_WIN_CNT]; //8进4的录像ID列表(在未决出胜者前用作押注ID，此时押注ID为0则表示无法押注，决出胜者后用作播放录像的ID)
	UINT8            byGamblePos; //已对哪个序号(1~16)的门派进行押注，为0则表示本轮未押注
}PKT_CLIGS_16_TO_8_TO_4_ACK;

typedef struct tagPKT_CLIGS_FINAL_WAR_ACK{
	tagPKT_CLIGS_FINAL_WAR_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FINAL_WAR_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_SCHEDULE_INFO stScheduleInfo; //跨服战进度信息
	UINT8            byFactionCnt; //门派数
	DT_FINAL_WAR_FACTION_INFO_CLI astFactionList[GVG_FINAL_WAR_FACTION_CNT]; //门派列表
	UINT32           dwCountDwonTime; //下一轮倒计时(秒)
	UINT8            byPassedStageCnt; //已结束轮次
	UINT8            byTotalStage; //总轮次
	UINT8            byCurStage; //当前阶段：0比赛未开始、1四服积分赛正在进行中、2积分总决赛已结束
	UINT32           dwGambleFactionID; //押注的门派ID（为0代表本轮未押注）
}PKT_CLIGS_FINAL_WAR_ACK;

typedef struct tagPKT_CLIGS_GVG_CHAMPION_ACK{
	tagPKT_CLIGS_GVG_CHAMPION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_CHAMPION_ACK)); }
	UINT16           wErrCode; //错误码
	DT_GVG_SCHEDULE_INFO stScheduleInfo; //跨服战进度信息
	DT_GVG_FACTION_RANK_LST_DATA stNo123; //本届冠亚季军信息
}PKT_CLIGS_GVG_CHAMPION_ACK;

typedef struct tagPKT_CLIGS_GVG_ERR_MSG_ACK{
	tagPKT_CLIGS_GVG_ERR_MSG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_ERR_MSG_ACK)); }
	TCHAR            aszErrMsg[MAX_GVG_ERR_MSG_LEN]; //错误提示：文字
}PKT_CLIGS_GVG_ERR_MSG_ACK;

typedef struct tagDT_GVG_VIDEO_FACTION_INFO{
	UINT8            byFactionIconID; //门派图标ID
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //门派名称
	TCHAR            aszNickName[USERNAME_LEN]; //场上玩家名称
	UINT8            byJob; //场上玩家在门派中的职位，见commondef.h的em_Faction_Job
	UINT8            byRemainPlayerCnt; //本方剩余人数
	UINT8            byKillCombo; //当前连斩数
	TCHAR            aszZoneName[ZONENAME_LEN]; //门派所在的区名
}DT_GVG_VIDEO_FACTION_INFO;

typedef struct tagPKT_CLIGS_SEE_GVG_VIDEO_REQ{
	tagPKT_CLIGS_SEE_GVG_VIDEO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEE_GVG_VIDEO_REQ)); }
	UINT64           qwVideoID; //录像ID
	UINT8            byVideoNum; //请求的场次
}PKT_CLIGS_SEE_GVG_VIDEO_REQ;

typedef struct tagPKT_CLIGS_SEE_GVG_VIDEO_ACK_v400{
	tagPKT_CLIGS_SEE_GVG_VIDEO_ACK_v400() { memset(this, 0, sizeof(tagPKT_CLIGS_SEE_GVG_VIDEO_ACK_v400)); }
	UINT16           wErrCode; //错误码
	UINT64           qwVideoID; //录像ID
	UINT8            byVideoNum; //本战斗属于第几场
	DT_GVG_VIDEO_FACTION_INFO stG1; //门派1信息
	DT_GVG_VIDEO_FACTION_INFO stG2; //门派2信息
	DT_EMPTY_HP_VIDEO_v400 stVideo; //录像数据
	UINT8            byHaveNextVideo; //是否有下一场: 1是、0否
	UINT8            byEnegy1; //能量条:0~100，值越小玩家越被削弱
	UINT8            byEnegy2; //能量条:0~100，值越小玩家越被削弱
	UINT8            byIsWeak1; //场上玩家本场是否被削弱
	UINT8            byIsWeak2; //场上玩家本场是否被削弱
}PKT_CLIGS_SEE_GVG_VIDEO_ACK_v400;

typedef struct tagPKT_CLIGS_SEE_GVG_VIDEO_ACK_v410{
	tagPKT_CLIGS_SEE_GVG_VIDEO_ACK_v410() { memset(this, 0, sizeof(tagPKT_CLIGS_SEE_GVG_VIDEO_ACK_v410)); }
	UINT16           wErrCode; //错误码
	UINT64           qwVideoID; //录像ID
	UINT8            byVideoNum; //本战斗属于第几场
	DT_GVG_VIDEO_FACTION_INFO stG1; //门派1信息
	DT_GVG_VIDEO_FACTION_INFO stG2; //门派2信息
	DT_EMPTY_HP_VIDEO stVideo; //录像数据
	UINT8            byHaveNextVideo; //是否有下一场: 1是、0否
	UINT8            byEnegy1; //能量条:0~100，值越小玩家越被削弱
	UINT8            byEnegy2; //能量条:0~100，值越小玩家越被削弱
	UINT8            byIsWeak1; //场上玩家本场是否被削弱
	UINT8            byIsWeak2; //场上玩家本场是否被削弱
}PKT_CLIGS_SEE_GVG_VIDEO_ACK_v410;

typedef struct tagPKT_CLIGS_END_GVG_VIDEO_REQ{
	tagPKT_CLIGS_END_GVG_VIDEO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_END_GVG_VIDEO_REQ)); }
	UINT64           qwVideoID; //录像ID
	UINT8            byEndType; //结束类型：自动结束0/手动点击按钮结束1
}PKT_CLIGS_END_GVG_VIDEO_REQ;

typedef struct tagPKT_CLIGS_END_GVG_VIDEO_ACK_v400{
	tagPKT_CLIGS_END_GVG_VIDEO_ACK_v400() { memset(this, 0, sizeof(tagPKT_CLIGS_END_GVG_VIDEO_ACK_v400)); }
	UINT16           wErrCode; //错误码
	UINT64           qwVideoID; //录像ID
	DT_GVG_VIDEO_FACTION_INFO stG1; //门派1信息
	DT_GVG_VIDEO_FACTION_INFO stG2; //门派2信息
	DT_EMPTY_HP_VIDEO_v400 stVideo; //残血录像数据
	DT_GVG_MVP_LIST  stMVPList; //各玩家表现列表
	UINT8            byEndType; //结束类型：自动结束0/手动点击按钮结束1
	DT_GVG_ROUND     stRound; //该录像所属轮次：用于客户端判断是否显示积分
}PKT_CLIGS_END_GVG_VIDEO_ACK_v400;

typedef struct tagPKT_CLIGS_END_GVG_VIDEO_ACK_v410{
	tagPKT_CLIGS_END_GVG_VIDEO_ACK_v410() { memset(this, 0, sizeof(tagPKT_CLIGS_END_GVG_VIDEO_ACK_v410)); }
	UINT16           wErrCode; //错误码
	UINT64           qwVideoID; //录像ID
	DT_GVG_VIDEO_FACTION_INFO stG1; //门派1信息
	DT_GVG_VIDEO_FACTION_INFO stG2; //门派2信息
	DT_EMPTY_HP_VIDEO stVideo; //残血录像数据
	DT_GVG_MVP_LIST  stMVPList; //各玩家表现列表
	UINT8            byEndType; //结束类型：自动结束0/手动点击按钮结束1
	DT_GVG_ROUND     stRound; //该录像所属轮次：用于客户端判断是否显示积分
}PKT_CLIGS_END_GVG_VIDEO_ACK_v410;

typedef struct tagPKT_CLIGS_SHOW_GVG_MVP_REQ{
	tagPKT_CLIGS_SHOW_GVG_MVP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SHOW_GVG_MVP_REQ)); }
	UINT64           qwVideoID; //录像ID
}PKT_CLIGS_SHOW_GVG_MVP_REQ;

typedef struct tagPKT_CLIGS_SHOW_GVG_MVP_ACK{
	tagPKT_CLIGS_SHOW_GVG_MVP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SHOW_GVG_MVP_ACK)); }
	UINT16           wErrCode; //错误码
	UINT64           qwVideoID; //录像ID
	DT_GVG_MVP_LIST  stMVPList; //录像结果
}PKT_CLIGS_SHOW_GVG_MVP_ACK;

typedef struct tagPKT_CLIGS_SELECT_GROUP_TYPE_REQ{
	tagPKT_CLIGS_SELECT_GROUP_TYPE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SELECT_GROUP_TYPE_REQ)); }
	UINT8            bySelect; //筛选条件 （em_Join_Type）
}PKT_CLIGS_SELECT_GROUP_TYPE_REQ;

typedef struct tagPKT_CLIGS_SELECT_GROUP_TYPE_ACK{
	tagPKT_CLIGS_SELECT_GROUP_TYPE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SELECT_GROUP_TYPE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_COURAGE_GROUP_DATA_CLI_LST stGroupData; //队伍信息列表
	UINT8            bySelect; //筛选条件（em_Join_Type）
}PKT_CLIGS_SELECT_GROUP_TYPE_ACK;

typedef struct tagPKT_CLIGS_GVG_FACTION_APPLAY_REQ{
	tagPKT_CLIGS_GVG_FACTION_APPLAY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_FACTION_APPLAY_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_GVG_FACTION_APPLAY_REQ;

typedef struct tagPKT_CLIGS_GVG_FACTION_APPLAY_ACK{
	tagPKT_CLIGS_GVG_FACTION_APPLAY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GVG_FACTION_APPLAY_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIGS_GVG_FACTION_APPLAY_ACK;

typedef struct tagPKT_CLIGS_READ_BATTLE_LOG_REQ{
	tagPKT_CLIGS_READ_BATTLE_LOG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_READ_BATTLE_LOG_REQ)); }
	UINT8            byKindID; //读取类型
}PKT_CLIGS_READ_BATTLE_LOG_REQ;

typedef struct tagDT_BATTLE_LOG_MSG_CLI{
	UINT8            byLogType; //战报小类型
	UINT64           qwLogTime; //战报时间
	UINT8            byParamNum; //参数个数
	UINT64           aqwParamLst[MAX_BATTLE_LOG_PARAM_NUM]; //参数
	UINT8            byStrNum; //字符串个数
	DT_TSTRING_MSG   astStrLst[MAX_BATTLE_LOG_PARAM_STR]; //字符串
}DT_BATTLE_LOG_MSG_CLI;

typedef struct tagDT_BATTLE_LOG_MSG_LST_CLI{
	UINT8            byLogNum; //战报个数
	DT_BATTLE_LOG_MSG_CLI astLogData[MAX_BATTLE_LOG_NUM]; //战报信息
}DT_BATTLE_LOG_MSG_LST_CLI;

typedef struct tagPKT_CLIGS_READ_BATTLE_LOG_ACK{
	tagPKT_CLIGS_READ_BATTLE_LOG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_READ_BATTLE_LOG_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byLogType; //战报大类型
	DT_BATTLE_LOG_MSG_LST_CLI stLogInfo; //战报信息
}PKT_CLIGS_READ_BATTLE_LOG_ACK;

typedef struct tagDT_BATTLE_LOG_UNREAD_INFO{
	UINT8            byLogType; //战报类型EBattleLogType
	UINT8            byUnReadNum; //未读个数
}DT_BATTLE_LOG_UNREAD_INFO;

typedef struct tagPKT_CLIGS_UNREAD_BATTLE_NTF{
	tagPKT_CLIGS_UNREAD_BATTLE_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_UNREAD_BATTLE_NTF)); }
	UINT8            byLogNum; //战报种类数
	DT_BATTLE_LOG_UNREAD_INFO astLogInfo[MAX_BATTLE_LOG_NUM]; //战报信息
}PKT_CLIGS_UNREAD_BATTLE_NTF;

typedef struct tagPKT_CLIGS_BATTLE_ACK4{
	tagPKT_CLIGS_BATTLE_ACK4() { memset(this, 0, sizeof(tagPKT_CLIGS_BATTLE_ACK4)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_ENCOURAGE_DATA stBattleEncourage; //战斗奖励
	DT_DATA_AFTER_BATTLE_ENCOURAGE stAfterBattleEncourage; //领取战斗奖励后玩家信息
	UINT8            byPassFlag; //是否通关
	DT_PASS_ENCOURAGE_DATA astPassEncourage[1]; //通关奖励
	DT_DATA_AFTER_PASS_ENCOURAGE astAfterPassEncourage[1]; //领取通关奖励后玩家信息
	DT_PASS_ENCOURAGE_BOX_DATA astPassEncourageBox[1]; //通关宝箱奖励信息
	DT_UNLOCK_DATA   astUnlockInfo[1]; //解锁信息
	UINT8            byHaveNextInstance; //是否有下一个副本
	DT_NEXT_INSTANCE_DATA astNextInstanceInfo[1]; //下一个副本信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_PHYSTRENGTH_DATA_CLI stPhyStrengthInfo; //体力信息
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	TCHAR            aszExtContext[MAX_COMMON_TXT_LEN]; //扩展信息
	DT_BATTLE_DATA4  stBattleInfo; //战斗信息
}PKT_CLIGS_BATTLE_ACK4;

typedef struct tagPKT_CLIGS_CHALLENGE_ACK4{
	tagPKT_CLIGS_CHALLENGE_ACK4() { memset(this, 0, sizeof(tagPKT_CLIGS_CHALLENGE_ACK4)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_CHALLENGE_PLAYER_DATA stEnemyPlayerData; //对方玩家信息
	UINT8            byEnemyHeroNum; //对方信息
	DT_ENEMY_HERO_DATA astEnemyInfoList[MAX_FORMATION_IDX_NUM]; //对方信息
	DT_RACE_CHALLENGE_ENCOURAGE_DATA stChallengeEncourage; //挑战奖励
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝点
	UINT64           qwCurJingJie; //玩家当前的境界点
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_BATTLE_DATA4  stBattleInfo; //战斗信息
}PKT_CLIGS_CHALLENGE_ACK4;

typedef struct tagPKT_CLIGS_PLUNDER_ACK4{
	tagPKT_CLIGS_PLUNDER_ACK4() { memset(this, 0, sizeof(tagPKT_CLIGS_PLUNDER_ACK4)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwEnemyID; //要战斗的玩家ID
	UINT32           dwCaptureID; //俘虏玩家ID(和EnemyID相同表示掠夺的是CaptureID玩家，不同表示抢夺CaptureID俘虏)
	UINT8            byUseItemFlag; //是否使用道具掠夺低于5级以上的玩家
	UINT8            byResistFlag; //是否反抗(1表示反抗)
	UINT8            byCaptureGridIdx; //俘虏格子序号(从0开始)
	DT_BECAPTURE_DATA stBeCaptureInfo; //俘虏玩家的被俘信息
	DT_CHALLENGE_PLAYER_DATA stEnemyPlayerData; //战斗敌方玩家信息
	UINT8            byEnemyHeroNum; //战斗敌方信息
	DT_ENEMY_HERO_DATA astEnemyInfoList[MAX_FORMATION_IDX_NUM]; //战斗敌方信息
	DT_PLUNDER_ENCOURAGE_DATA stPlunderEncourage; //掠夺奖励
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝
	UINT16           wCanPlunderNum; //玩家当前可掠夺次数
	UINT16           wVipExtNum; //VIP格外增加的次数
	UINT16           wFreePlunderNum; //玩家免费掠夺次数
	UINT32           dwPlunderGold; //收费掠夺元宝(若还有免费的则为0)
	UINT16           wPlunderItemNum; //当前拥有的掠夺道具
	UINT8            byCaptureFlag; //是否俘虏玩家，1为对方成为你的俘虏
	DT_CAPTURE_DATA_CLI astCaptureInfo[1]; //俘虏信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_BATTLE_DATA4  stBattleInfo; //战斗信息
}PKT_CLIGS_PLUNDER_ACK4;

typedef struct tagPKT_CLIGS_PKBATTLE_ACK4{
	tagPKT_CLIGS_PKBATTLE_ACK4() { memset(this, 0, sizeof(tagPKT_CLIGS_PKBATTLE_ACK4)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_DATA4  stBattleInfo; //战斗信息
}PKT_CLIGS_PKBATTLE_ACK4;

typedef struct tagPKT_CLIGS_RDCHALLENGE_ACK4{
	tagPKT_CLIGS_RDCHALLENGE_ACK4() { memset(this, 0, sizeof(tagPKT_CLIGS_RDCHALLENGE_ACK4)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RDC_TAB_DATA  stTabInfo; //随机挑战面板信息
	DT_RDC_ENC_DATA  stEncInfo; //随机挑战奖励信息
	UINT64           qwCurJingJie; //当前境界点
	UINT64           qwCurPurpleGas; //当前紫气
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT16           wKindID; //获得的鲜花ID
	DT_BATTLE_DATA4  stBattleInfo; //战斗信息
}PKT_CLIGS_RDCHALLENGE_ACK4;

typedef struct tagPKT_CLIGS_HUNTING_BATTLE_ACK3{
	tagPKT_CLIGS_HUNTING_BATTLE_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_HUNTING_BATTLE_ACK3)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_REWARD_INFO   stBattleReward; //战斗奖励
	DT_HUNTING_SCHEDULE_INFO stTaskScheduleInfo; //任务进度信息
	UINT32           dwClosedMonsterNeedGold; //收妖需要的元宝
	TCHAR            aszBattleLostMsg[MAX_FACTION_DESC_NUM]; //战斗失败信息
	UINT32           dwSyncDoorsTribute; //同步数据
	UINT8            byScheduleChangeFlag; //档数改变标识（0 没变， 1变）
	UINT8            byHuntingLeftNum; //捉妖次数同步
	UINT8            byIsGoldMode; //是否处于黄金模式
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
	DT_BATTLE_DATA4  stBattleInfo; //战斗信息
}PKT_CLIGS_HUNTING_BATTLE_ACK3;

typedef struct tagDT_COURAGE_PLAYER_BATTLE_DATA3{
	UINT8            byBattleNum; //战斗次数
	DT_BATTLE_DATA4  astBattleInfo[MAX_COURAGE_MEMBER_NUM]; //战斗信息
}DT_COURAGE_PLAYER_BATTLE_DATA3;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_START_ACK3{
	tagPKT_CLIGS_COURAGE_PLAYER_START_ACK3() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_START_ACK3)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_MONSTER_DATA_LIST stMonsterInfo; //怪物信息
	DT_COURAGE_GROUP_PLAYER_BATTLE_DATA stGroupPlayerInfo; //队伍信息
	UINT8            byPassFlag; //是否通关 0通关 1不通关
	DT_INSTANCE_ID   stInstanceID; //副本序号
	DT_COURAGE_JEWELRY_DATA_LST stjewelryInfoLst; //饰品信息
	UINT8            byHelpID; //协助描述 1再进行1次， 2协助两次送1次，3已经赠送不能在赠送了
	DT_COURAGE_PLAYER_BATTLE_DATA3 stBattleInfoCli; //战斗信息
}PKT_CLIGS_COURAGE_PLAYER_START_ACK3;

typedef struct tagPKT_CLIGS_COURAGE_LEADER_START_NTF3{
	tagPKT_CLIGS_COURAGE_LEADER_START_NTF3() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_LEADER_START_NTF3)); }
	PKT_CLIGS_COURAGE_PLAYER_START_ACK3 stBattleInfo; //战斗信息
}PKT_CLIGS_COURAGE_LEADER_START_NTF3;

typedef struct tagPKT_CLIGS_BEGIN_JIEBIAO_ACK4{
	tagPKT_CLIGS_BEGIN_JIEBIAO_ACK4() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_JIEBIAO_ACK4)); }
	UINT16           wErrCode; //错误码
	UINT8            byYabiaoFlag; //押镖标识(0为不在押镖，1为正在押镖中，2为押镖完毕，可以领取奖励)
	UINT64           qwCoinEncourage; //奖励
	UINT32           dwScienceEncourage; //拦截器魂奖励
	UINT64           qwCurCoin; //当前铜钱数
	UINT64           qwCurScience; //当前器魂
	UINT32           dwBeJiebiaoPlayerID; //被拦截对象
	BIAO_OPERATE_INFO stBeJiebiaoInfo; //被劫信息
	UINT32           dwJiebiaoPlayerID; //拦截对象
	BIAO_OPERATE_INFO stJiebiaoInfo; //拦截信息
	UINT16           wJiebiaoCD; //冷却时间
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_BATTLE_DATA4  stBattleData; //战斗信息
}PKT_CLIGS_BEGIN_JIEBIAO_ACK4;

typedef struct tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK4{
	tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK4() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK4)); }
	UINT16           wErrCode; //错误码
	DT_BATTLE_DATA4  stBattleData; //战斗信息
}PKT_CLIGS_QUERY_BATTLE_RECORD_ACK4;

typedef struct tagDT_CLIGS_SOUL_MARK_DATA{
	UINT16           wHeroId; //0代表背包的战魂 英雄id对应身上的战魂
	UINT16           wSoulID; //战魂ID
	UINT32           dwSoulIdx; //战魂IDX
}DT_CLIGS_SOUL_MARK_DATA;

typedef struct tagDT_CLIGS_SOUL_MARK_LST_DATA{
	UINT16           wSoulNum; //战魂数
	DT_CLIGS_SOUL_MARK_DATA astSoulLstMark[MAX_SOUL_BAG_NUM]; //战魂标识
}DT_CLIGS_SOUL_MARK_LST_DATA;

typedef struct tagPKT_CLIGS_OPEN_DEVOUR_SOUL_REQ{
	tagPKT_CLIGS_OPEN_DEVOUR_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DEVOUR_SOUL_REQ)); }
	DT_CLIGS_SOUL_MARK_DATA stSoulID; //战魂信息
}PKT_CLIGS_OPEN_DEVOUR_SOUL_REQ;

typedef struct tagPKT_CLIGS_OPEN_DEVOUR_SOUL_ACK{
	tagPKT_CLIGS_OPEN_DEVOUR_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DEVOUR_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_DATA_LST_CLI stSoulBag; //战魂背包
	DT_SOUL_DATA_CLI stMainSoul; //主战魂信息
	DT_PLAYER_RES_LST_DATA stResData; //资源信息
	DT_PLAYER_RES_DATA stCostRes; //吞噬消耗资源信息
	UINT8            byAttrUpPer; //属性提升百分比
	DT_SOUL_POTENCY_CLI stPotencyInfo; //潜能信息
}PKT_CLIGS_OPEN_DEVOUR_SOUL_ACK;

typedef struct tagPKT_CLIGS_DEVOUR_SOUL_REQ{
	tagPKT_CLIGS_DEVOUR_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DEVOUR_SOUL_REQ)); }
	DT_CLIGS_SOUL_MARK_DATA stMainSoul; //主战魂
	DT_CLIGS_SOUL_MARK_LST_DATA stDevourSoul; //吞噬战魂
}PKT_CLIGS_DEVOUR_SOUL_REQ;

typedef struct tagPKT_CLIGS_DEVOUR_SOUL_ACK{
	tagPKT_CLIGS_DEVOUR_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DEVOUR_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_DATA_LST_CLI stSoulBag; //战魂背包
	DT_SOUL_DATA_CLI stMainSoul; //主战魂信息
	DT_PLAYER_RES_LST_DATA stResData; //资源信息
	DT_PLAYER_RES_DATA stCostRes; //吞噬消耗资源信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
	DT_HERO_PANEL_CLI stHeroInfo; //英雄属性
	UINT8            byAttrUpPer; //属性提升百分比
}PKT_CLIGS_DEVOUR_SOUL_ACK;

typedef struct tagPKT_CLIGS_OPEN_REFLASH_SOUL_REQ{
	tagPKT_CLIGS_OPEN_REFLASH_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_REFLASH_SOUL_REQ)); }
	DT_CLIGS_SOUL_MARK_DATA stMainSoul; //主战魂
}PKT_CLIGS_OPEN_REFLASH_SOUL_REQ;

typedef struct tagPKT_CLIGS_OPEN_REFLASH_SOUL_ACK{
	tagPKT_CLIGS_OPEN_REFLASH_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_REFLASH_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_DATA_CLI stSoulData; //战魂
	DT_PLAYER_RES_LST_DATA stResData; //资源信息
	DT_PLAYER_RES_DATA stCostRes; //洗炼消耗资源信息
}PKT_CLIGS_OPEN_REFLASH_SOUL_ACK;

typedef struct tagPKT_CLIGS_REFLASH_SOUL_REQ{
	tagPKT_CLIGS_REFLASH_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_REFLASH_SOUL_REQ)); }
	DT_CLIGS_SOUL_MARK_DATA stMainSoul; //主战魂
	UINT8            byLockNum; //锁定个数
	UINT8            abyLockIDLst[MAX_SOUL_ATTR_NUM]; //锁定编号
}PKT_CLIGS_REFLASH_SOUL_REQ;

typedef struct tagPKT_CLIGS_REFLASH_SOUL_ACK{
	tagPKT_CLIGS_REFLASH_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_REFLASH_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_DATA_CLI stSoulData; //战魂
	DT_PLAYER_RES_LST_DATA stResData; //资源信息
}PKT_CLIGS_REFLASH_SOUL_ACK;

typedef struct tagPKT_CLIGS_SAVE_REFLASH_SOUL_REQ{
	tagPKT_CLIGS_SAVE_REFLASH_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SAVE_REFLASH_SOUL_REQ)); }
	DT_CLIGS_SOUL_MARK_DATA stMainSoul; //主战魂
	UINT8            byIsSave; //保留（0不保留， 1保留）
}PKT_CLIGS_SAVE_REFLASH_SOUL_REQ;

typedef struct tagPKT_CLIGS_SAVE_REFLASH_SOUL_ACK{
	tagPKT_CLIGS_SAVE_REFLASH_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SAVE_REFLASH_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_DATA_CLI stSoulData; //战魂
	DT_PLAYER_RES_LST_DATA stResData; //资源信息
	DT_PLAYER_RES_DATA stCostRes; //洗炼消耗资源信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
	DT_CLIGS_SOUL_MARK_DATA stMainSoul; //主战魂信息
	DT_HERO_PANEL_CLI stHeroInfo; //英雄属性
}PKT_CLIGS_SAVE_REFLASH_SOUL_ACK;

typedef struct tagDT_CLIGS_PRODUCTSOUL_COST{
	UINT8            byCostType; //花费类型 2代表铜钱 3代表元宝
	UINT32           dwCostNum; //花费数目
}DT_CLIGS_PRODUCTSOUL_COST;

typedef struct tagDT_SOUL_REP_INFO{
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
	DT_CLIGS_PRODUCTSOUL_COST astNormalCost[MAX_SOUL_PRODUCT_TYPE]; //普通探索三种产出类型花费
	DT_CLIGS_PRODUCTSOUL_COST astContinusCost[MAX_SOUL_PRODUCT_TYPE]; //10连探索三种产出类型花费
	UINT8            abyMinStar[MAX_SOUL_PRODUCT_TYPE]; //产出魂的最小星级
	UINT8            abyMaxStar[MAX_SOUL_PRODUCT_TYPE]; //产出魂的最大星级
}DT_SOUL_REP_INFO;

typedef struct tagPKT_CLIGS_SOULREP_INFO_REQ{
	tagPKT_CLIGS_SOULREP_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SOULREP_INFO_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_SOULREP_INFO_REQ;

typedef struct tagPKT_CLIGS_SOULREP_INFO_ACK{
	tagPKT_CLIGS_SOULREP_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SOULREP_INFO_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_REP_INFO stSoulRepInfo; //战魂副本信息
	UINT8            byOpenActivity; //0代表没开启活动 1代表开启活动
}PKT_CLIGS_SOULREP_INFO_ACK;

typedef struct tagPKT_CLIGS_PRODUCT_SOUL_REQ{
	tagPKT_CLIGS_PRODUCT_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PRODUCT_SOUL_REQ)); }
	UINT8            byProductType; //将魂产出类型 值小于MAX_SOUL_PRODUCT_TYPE
	UINT8            bySearchType; //0为普通探索 1为10连探索
}PKT_CLIGS_PRODUCT_SOUL_REQ;

typedef struct tagPKT_CLIGS_PRODUCT_SOUL_ACK{
	tagPKT_CLIGS_PRODUCT_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PRODUCT_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byMeterialNum; //产出几个战魂材料
	DT_GOODS_DATA_CLI astMeterialGood[MAX_SOUL_CONTINUE_SEARCH]; //获得的战魂材料
	UINT8            bySoulNum; //产出几个战魂
	DT_SOUL_DATA_CLI astSoulData[MAX_SOUL_CONTINUE_SEARCH]; //获得的战魂
	DT_SOUL_REP_INFO stSoulRepInfo; //战魂副本信息
	UINT8            byBoxType; //宝箱形态 0铜 1银 2金
}PKT_CLIGS_PRODUCT_SOUL_ACK;

typedef struct tagPKT_CLIGS_HERO_EQUIP_SOUL_REQ{
	tagPKT_CLIGS_HERO_EQUIP_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_HERO_EQUIP_SOUL_REQ)); }
	UINT16           wHeroId; //英雄id
	UINT16           wSoulID; //战魂背包中要给英雄穿戴战魂ID
	UINT32           dwSoulIdx; //战魂背包中要给英雄穿戴战魂index
}PKT_CLIGS_HERO_EQUIP_SOUL_REQ;

typedef struct tagPKT_CLIGS_HERO_EQUIP_SOUL_ACK{
	tagPKT_CLIGS_HERO_EQUIP_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_HERO_EQUIP_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wSoulID; //战魂背包中要给英雄穿戴战魂ID
	UINT32           dwSoulIdx; //战魂背包中要给英雄穿戴战魂index
	DT_POWER_CHANGE_DATA stPowerInfo; //战魂背包中要给英雄穿戴战魂index
	DT_HERO_PANEL_CLI stHeroInfo; //战魂背包中要给英雄穿戴战魂index
}PKT_CLIGS_HERO_EQUIP_SOUL_ACK;

typedef struct tagPKT_CLIGS_HERO_UNDRESS_SOUL_REQ{
	tagPKT_CLIGS_HERO_UNDRESS_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_HERO_UNDRESS_SOUL_REQ)); }
	UINT16           wHeroId; //英雄id
	UINT16           wSoulID; //英雄身上装备的 准备脱下战魂ID
	UINT32           dwSoulIdx; //英雄身上装备的 准备脱下战魂index
}PKT_CLIGS_HERO_UNDRESS_SOUL_REQ;

typedef struct tagPKT_CLIGS_HERO_UNDRESS_SOUL_ACK{
	tagPKT_CLIGS_HERO_UNDRESS_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_HERO_UNDRESS_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wSoulID; //英雄身上装备的 准备脱下战魂ID
	UINT32           dwSoulIdx; //英雄身上装备的 准备脱下战魂index
	DT_POWER_CHANGE_DATA stPowerInfo; //英雄身上装备的 准备脱下战魂index
	DT_HERO_PANEL_CLI stHeroInfo; //英雄身上装备的 准备脱下战魂index
}PKT_CLIGS_HERO_UNDRESS_SOUL_ACK;

typedef struct tagPKT_CLIGS_HERO_REPLACE_SOUL_REQ{
	tagPKT_CLIGS_HERO_REPLACE_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_HERO_REPLACE_SOUL_REQ)); }
	UINT16           wHeroId; //英雄id
	UINT16           wBagSoulID; //战魂背包中要给英雄穿戴战魂ID
	UINT32           dwBagSoulIdx; //战魂背包中要给英雄穿戴战魂index
	UINT16           wHeroSoulID; //英雄身上装备的 准备脱下战魂ID
	UINT32           dwHeroSoulIdx; //英雄身上装备的 准备脱下战魂index
}PKT_CLIGS_HERO_REPLACE_SOUL_REQ;

typedef struct tagPKT_CLIGS_HERO_REPLACE_SOUL_ACK{
	tagPKT_CLIGS_HERO_REPLACE_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_HERO_REPLACE_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wBagSoulID; //战魂背包中要给英雄穿戴战魂ID
	UINT32           dwBagSoulIdx; //战魂背包中要给英雄穿戴战魂index
	UINT16           wHeroSoulID; //英雄身上装备的 准备脱下战魂ID
	UINT32           dwHeroSoulIdx; //英雄身上装备的 准备脱下战魂index
	DT_POWER_CHANGE_DATA stPowerInfo; //英雄身上装备的 准备脱下战魂index
	DT_HERO_PANEL_CLI stHeroInfo; //英雄身上装备的 准备脱下战魂index
}PKT_CLIGS_HERO_REPLACE_SOUL_ACK;

typedef struct tagDT_SOUL_EXCHANGE_METERIAL{
	UINT32           dwMeterialID; //材料ID
	UINT32           dwMeterialNum; //材料数量
}DT_SOUL_EXCHANGE_METERIAL;

typedef struct tagDT_SOUL_EXCHANGE_CARD{
	UINT32           dwID; //兑换卡ID
	TCHAR            aszName[MAX_SOUL_CARD_NAME_LENGTH]; //兑换卡名字
	UINT8            byMeterialNum; //兑换需要的材料数量
	DT_SOUL_EXCHANGE_METERIAL astMeterialInfo[MAX_EXCHANGE_SOUL_METERIAL_NUM]; //兑换的材料信息
}DT_SOUL_EXCHANGE_CARD;

typedef struct tagPKT_CLIGS_SOULEXCHANGE_INFO_REQ{
	tagPKT_CLIGS_SOULEXCHANGE_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SOULEXCHANGE_INFO_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_SOULEXCHANGE_INFO_REQ;

typedef struct tagPKT_CLIGS_SOULEXCHANGE_INFO_ACK{
	tagPKT_CLIGS_SOULEXCHANGE_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SOULEXCHANGE_INFO_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byExchangeCardNum; //兑换卡数量
	DT_SOUL_EXCHANGE_CARD astExchangeCardInfo[MAX_EXCHANGE_SOUL_CARD_NUM]; //兑换卡信息
	UINT8            byMeterialNum; //背包兑换材料数量
	DT_GOODS_DATA_CLI astMeterialInfo[MAX_BAG_OPEN_NUM]; //背包兑换材料信息
}PKT_CLIGS_SOULEXCHANGE_INFO_ACK;

typedef struct tagPKT_CLIGS_EXCHANGE_SOUL_REQ{
	tagPKT_CLIGS_EXCHANGE_SOUL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EXCHANGE_SOUL_REQ)); }
	UINT32           dwSoulExchangeId; //战力变化信息
	UINT32           dwExchangeNum; //战力变化信息
}PKT_CLIGS_EXCHANGE_SOUL_REQ;

typedef struct tagPKT_CLIGS_EXCHANGE_SOUL_ACK{
	tagPKT_CLIGS_EXCHANGE_SOUL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EXCHANGE_SOUL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            bySoulNum; //兑换将魂数量
	DT_SOUL_DATA_CLI astSoulInfo[MAX_EXCHANGE_SOUL_NUM]; //兑换获得的将魂
	UINT8            byMeterialNum; //背包兑换材料数量
	DT_GOODS_DATA_CLI astMeterialInfo[MAX_BAG_OPEN_NUM]; //背包兑换材料信息
}PKT_CLIGS_EXCHANGE_SOUL_ACK;

typedef struct tagPKT_CLIGS_OPEN_MY_MEDAL_REQ{
	tagPKT_CLIGS_OPEN_MY_MEDAL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MY_MEDAL_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_MY_MEDAL_REQ;

typedef struct tagPKT_CLIGS_OPEN_MY_MEDAL_ACK{
	tagPKT_CLIGS_OPEN_MY_MEDAL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_MY_MEDAL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_ATTR_BASE_LST_DATA stAttrInfo; //属性累加
	DT_MEDAL_DATA_LST_CLI stMedalLstInfo; //勋章信息
}PKT_CLIGS_OPEN_MY_MEDAL_ACK;

typedef struct tagDT_MEDAL_ICON_NAME{
	TCHAR            aszMedalIconName[MAX_NAME_LEN]; //战力变化信息
}DT_MEDAL_ICON_NAME;

typedef struct tagDT_MEDAL_ICON_ID_LST_CLI{
	UINT16           wMedalNum; //勋章个数
	DT_MEDAL_ICON_NAME astMedalIconID[MAX_MEDAL_NUM]; //数组
}DT_MEDAL_ICON_ID_LST_CLI;

typedef struct tagPKT_CLIGS_MEDAL_NTF{
	tagPKT_CLIGS_MEDAL_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_MEDAL_NTF)); }
	DT_MEDAL_ICON_ID_LST_CLI stUnLockMedal; //解锁勋章
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
}PKT_CLIGS_MEDAL_NTF;

typedef struct tagPKT_CLIGS_OPEN_OUT_BOUND_REQ{
	tagPKT_CLIGS_OPEN_OUT_BOUND_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_OUT_BOUND_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_OUT_BOUND_REQ;

typedef struct tagDT_OUT_BOUND_DETAIL_DATA{
	DT_INSTANCE_ID   stInstanceID; //副本编号
	TCHAR            aszInstanceName[NORMAL_MSG_LEN]; //副本名称
	UINT8            byLeftCrossNum; //副本每天剩余闯关次数
	UINT8            byState; //是否可挂机0正常，1可挂机，2未解锁(等级不够)，3上一副本未解锁（等级够了）
	UINT16           wLevelReq; //解锁要求
	UINT8            byIsMajor; //是否是重要副本（精）
	DT_PLAYER_RES_LST_DATA stResInfo; //胜利获得
	DT_ITEM_DATA_LIST_LESS_CLI3 stItemInfo; //概率获得
}DT_OUT_BOUND_DETAIL_DATA;

typedef struct tagDT_OUT_BOUND_DETAIL_LST_DATA{
	UINT8            byInstanceNum; //副本个数
	DT_OUT_BOUND_DETAIL_DATA astInstanceInfo[MAX_OUT_BOUND_INSTANCE_NUM]; //副本信息
	UINT8            bySelectIdx; //当前选中第几幕
}DT_OUT_BOUND_DETAIL_LST_DATA;

typedef struct tagDT_OUT_BOUND_SIMPLE_DATA{
	UINT16           wScenceIdx; //场景IDX
	UINT8            byTownIdx; //副本IDX
	UINT8            byState; //状态(0正常，1未开启，2未解锁,3闯关成功)
	TCHAR            aszUnlockReq[MAX_OUT_BOUND_DESC_LEN]; //解锁要求
	TCHAR            aszHeroDesc[MAX_OUT_BOUND_DESC_LEN]; //英雄解锁描述
	TCHAR            aszItemDesc[MAX_OUT_BOUND_DESC_LEN]; //物品掉落描述
}DT_OUT_BOUND_SIMPLE_DATA;

typedef struct tagPKT_CLIGS_OPEN_OUT_BOUND_ACK{
	tagPKT_CLIGS_OPEN_OUT_BOUND_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_OUT_BOUND_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_RES_LST_DATA stResInfo; //资源信息
	UINT8            byOutBoundNum; //外传个数
	DT_OUT_BOUND_SIMPLE_DATA astOutBoundInfo[MAX_OUT_BOUND_NUM]; //外传信息
}PKT_CLIGS_OPEN_OUT_BOUND_ACK;

typedef struct tagPKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ{
	tagPKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ)); }
	UINT8            byTownIdx; //城镇IDX
}PKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ;

typedef struct tagPKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK{
	tagPKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK)); }
	UINT16           wErrCode; //错误码
	DT_OUT_BOUND_DETAIL_LST_DATA stInstanceLstInfo; //副本信息
}PKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK;

typedef struct tagPKT_CLIGS_OUT_BOUND_BATTLE_REQ{
	tagPKT_CLIGS_OUT_BOUND_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OUT_BOUND_BATTLE_REQ)); }
	DT_INSTANCE_ID   stInstanceID; //副本ID
	UINT8            byIsHook; //是否挂机0不挂机，1挂机
}PKT_CLIGS_OUT_BOUND_BATTLE_REQ;

typedef struct tagPKT_CLIGS_OUT_BOUND_BATTLE_ACK{
	tagPKT_CLIGS_OUT_BOUND_BATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OUT_BOUND_BATTLE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_OUT_BOUND_DETAIL_LST_DATA stInstanceLstInfo; //副本信息
	UINT8            byHaveBattleInfo; //是否需要战斗信息0不需要，1需要
	DT_BATTLE_DATA5  astBattleInfo[1]; //战斗信息
	DT_BATTLE_ENCOURAGE_DATA stBattleEncourage; //战斗奖励
	DT_DATA_AFTER_BATTLE_ENCOURAGE stAfterBattleEncourage; //领取战斗奖励后玩家信息
	UINT8            byPassFlag; //是否通关
	DT_PASS_ENCOURAGE_DATA astPassEncourage[1]; //通关奖励
	DT_DATA_AFTER_PASS_ENCOURAGE astAfterPassEncourage[1]; //领取通关奖励后玩家信息
	DT_PASS_ENCOURAGE_BOX_DATA astPassEncourageBox[1]; //通关宝箱奖励信息
	DT_PLAYER_RES_LST_DATA stResInfo; //资源信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
}PKT_CLIGS_OUT_BOUND_BATTLE_ACK;

typedef struct tagPKT_CLIGS_OPEN_STORE_REQ{
	tagPKT_CLIGS_OPEN_STORE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STORE_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_STORE_REQ;

typedef struct tagDT_GOODS_EXCHANGE_INFO{
	DT_PLAYER_RES_DATA stResReq; //兑换要求资源
	DT_ITEM_DATA_CLI3 stGoodsInfo; //物品信息
}DT_GOODS_EXCHANGE_INFO;

typedef struct tagDT_GOODS_EXCHANGE_LST_INFO{
	UINT16           wGoodsNum; //物品个数
	DT_GOODS_EXCHANGE_INFO astGoodsInfo[MAX_BAG_OPEN_NUM_NEW]; //物品信息
}DT_GOODS_EXCHANGE_LST_INFO;

typedef struct tagPKT_CLIGS_OPEN_STORE_ACK{
	tagPKT_CLIGS_OPEN_STORE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STORE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_RES_LST_DATA stResInfo; //资源信息
	DT_GOODS_EXCHANGE_LST_INFO stItemInfo; //物品信息
}PKT_CLIGS_OPEN_STORE_ACK;

typedef struct tagPKT_CLIGS_EXCHANGE_GOODS_REQ{
	tagPKT_CLIGS_EXCHANGE_GOODS_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EXCHANGE_GOODS_REQ)); }
	UINT16           wGoodsID; //物品ID
	UINT16           wGoodsNum; //物品个数
}PKT_CLIGS_EXCHANGE_GOODS_REQ;

typedef struct tagPKT_CLIGS_EXCHANGE_GOODS_ACK{
	tagPKT_CLIGS_EXCHANGE_GOODS_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EXCHANGE_GOODS_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_RES_LST_DATA stResInfo; //资源信息
	UINT16           wItemsID; //物品信息
	UINT16           wItemNum; //物品数量
}PKT_CLIGS_EXCHANGE_GOODS_ACK;

typedef struct tagDT_SKILL_INFO{
	UINT16           wSkillID; //技能ID
	UINT8            byHurtPeople; //伤害人数
	UINT16           wHurtRate; //伤害倍率
	UINT16           wSpecialParam; //特效参数1
	UINT8            byBeginRound; //开始作用回合
	UINT16           wFinalHurtRate; //最终伤害加成
	UINT16           wBuffParam; //BUFF参数
	UINT8            byBuffRound; //BUFF作用回合数
}DT_SKILL_INFO;

typedef struct tagDT_SKILL_LST_INFO{
	UINT8            bySkillNum; //技能个数
	DT_SKILL_INFO    astSkillInfo[MAX_PET_SKILL_NUM]; //技能信息
}DT_SKILL_LST_INFO;

typedef struct tagDT_PET_CLI_DATA{
	UINT16           wPetId; //id唯一标志
	UINT8            byStarLv; //星阶
	UINT16           wLv; //等级
	UINT16           wMaxLv; //等级上限
	UINT32           dwLvExp; //等级经验
	UINT32           dwNextLvExp; //下一等级经验
	TCHAR            aszPetName[MAX_PET_NAME_LENGTH]; //名字
	UINT32           dwCurPhychicVal; //当前灵力资质
	UINT32           dwMaxPhychicVal; //灵力资质上限
	UINT32           dwCurLingli; //灵力值
	UINT32           dwBeAteExp; //被吃掉经验
	TCHAR            aszPetPic[MAX_PET_NAME_LENGTH]; //宠物图片
	UINT8            byPetState; //宠物跟随记录
	DT_PET_WASHQUALITY_DATA astWashQualityData[MAX_PET_WASH_PSYCHIC_CELL_CNT]; //宠物洗练格子信息
	UINT8            byIsUpStar; //是否能升星 0否 1是
	UINT8            byPetColor; //宠物颜色
	DT_SKILL_LST_INFO stSkillInfo; //技能信息
	UINT8            byPetQuality; //宠物品质
	UINT8            byPetType; //宠物品质
	UINT16           wPetCfgID; //配置ID
	UINT64           qwScore; //评分
}DT_PET_CLI_DATA;

typedef struct tagDT_PET_CLI_DATA_LST{
	UINT16           wPetNum; //宠物数量
	DT_PET_CLI_DATA  astPetData[MAX_PET_NUM]; //宠物数据
}DT_PET_CLI_DATA_LST;

typedef struct tagPKT_CLIGS_PETREP_INFO_REQ{
	tagPKT_CLIGS_PETREP_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PETREP_INFO_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_PETREP_INFO_REQ;

typedef struct tagPKT_CLIGS_PETREP_INFO_ACK{
	tagPKT_CLIGS_PETREP_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PETREP_INFO_ACK)); }
	UINT16           wErrCode; //错误码
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
	UINT32           dwNormalCostGold; //普通召唤消耗元宝
	UINT32           dwGoodCostGold; //连续召唤消耗元宝
	UINT32           dwNormalLeftTime; //普通召唤距离免费召唤的时间
	UINT32           dwGoodLeftTime; //连续召唤距离免费召唤的时间
	UINT32           dwNormalFreeCnt; //今天普通剩余次数
	UINT8            byRuleFlag; //规则标识(0,正常，1新)
}PKT_CLIGS_PETREP_INFO_ACK;

typedef struct tagPKT_CLIGS_PRODUCT_PET_REQ{
	tagPKT_CLIGS_PRODUCT_PET_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PRODUCT_PET_REQ)); }
	UINT8            byProductType; //产出类型 1为A类 2为B类
}PKT_CLIGS_PRODUCT_PET_REQ;

typedef struct tagPKT_CLIGS_PRODUCT_PET_ACK{
	tagPKT_CLIGS_PRODUCT_PET_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PRODUCT_PET_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byGoodNum; //产出几个物品
	DT_GOODS_DATA_CLI astPetGood[MAX_PRODUCTPET_EXTRA_ITEM_NUM]; //获得的物品
	UINT8            byPetNum; //产出几只宠物
	DT_PET_CLI_DATA  astPetData[MAX_PRODUCTPET_NUM]; //产出宠物数据
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
	UINT32           dwNormalCostGold; //普通召唤消耗元宝
	UINT32           dwGoodCostGold; //连续召唤消耗元宝
	UINT32           dwNormalLeftTime; //普通召唤距离免费召唤的时间
	UINT32           dwGoodLeftTime; //连续召唤距离免费召唤的时间
	UINT32           dwNormalFreeCnt; //今天普通剩余次数
}PKT_CLIGS_PRODUCT_PET_ACK;

typedef struct tagDT_STAR_DRAGON_ITEM_INFO{
	UINT32           dwItemID; //材料ID
	UINT32           dwItemNum; //材料数量
}DT_STAR_DRAGON_ITEM_INFO;

typedef struct tagDT_UP_STAR_INFO{
	UINT16           wStarLv; //战力变化信息
	UINT16           wLingliPer; //战力变化信息
	UINT16           wLvLimit; //战力变化信息
}DT_UP_STAR_INFO;

typedef struct tagPKT_CLIGS_OPEN_STAR_PANEL_REQ{
	tagPKT_CLIGS_OPEN_STAR_PANEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STAR_PANEL_REQ)); }
	UINT8            byPetId; //宠物id
}PKT_CLIGS_OPEN_STAR_PANEL_REQ;

typedef struct tagPKT_CLIGS_OPEN_STAR_PANEL_ACK{
	tagPKT_CLIGS_OPEN_STAR_PANEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STAR_PANEL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byIsFullStartLv; //星阶是否满级
	DT_UP_STAR_INFO  stCurUpStarInfo; //当前灵力百分比
	DT_UP_STAR_INFO  stNextUpStarInfo; //升星后灵力百分比
	UINT8            byBagDragonItemNum; //背包龙珠材料数量
	DT_GOODS_DATA_CLI astBagDragonItemInfo[MAX_BAG_OPEN_NUM]; //背包龙珠道具信息
	UINT8            byNeedDragonNum; //生星需要龙珠个数
	DT_STAR_DRAGON_ITEM_INFO astNeedDragonInfo[MAX_STAR_DRAGON_ITEMTYPE_NUM]; //生星需要龙珠信息
}PKT_CLIGS_OPEN_STAR_PANEL_ACK;

typedef struct tagPKT_CLIGS_UP_STAR_LV_REQ{
	tagPKT_CLIGS_UP_STAR_LV_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UP_STAR_LV_REQ)); }
	UINT8            byPetId; //宠物id
}PKT_CLIGS_UP_STAR_LV_REQ;

typedef struct tagPKT_CLIGS_UP_STAR_LV_ACK{
	tagPKT_CLIGS_UP_STAR_LV_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UP_STAR_LV_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA  stPetData; //宠物数据
}PKT_CLIGS_UP_STAR_LV_ACK;

typedef struct tagPKT_CLIGS_OPEN_PET_PANEL_REQ{
	tagPKT_CLIGS_OPEN_PET_PANEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_PANEL_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_OPEN_PET_PANEL_REQ;

typedef struct tagPKT_CLIGS_OPEN_PET_PANEL_ACK{
	tagPKT_CLIGS_OPEN_PET_PANEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_PANEL_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA_LST stAllPetInfo; //宠物信息
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_OPEN_PET_PANEL_ACK;

typedef struct tagPKT_CLIGS_OPEN_PETFEED_REQ{
	tagPKT_CLIGS_OPEN_PETFEED_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PETFEED_REQ)); }
	UINT8            byPetId; //宠物id
}PKT_CLIGS_OPEN_PETFEED_REQ;

typedef struct tagPKT_CLIGS_OPEN_PETFEED_ACK{
	tagPKT_CLIGS_OPEN_PETFEED_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PETFEED_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byPetId; //宠物id
	UINT32           dwIsCanFeed; //是否可以喂养
	UINT32           dwLeftFreeFeedCnt; //今日免费剩余喂养次数
	UINT32           dwVipFeedCostGold; //VIP喂养消耗元宝
	UINT32           dwTotalFeedCnt; //总共喂养的次数
	UINT8            abyCurFruitType[MAX_PET_FEED_FRUIT_CNT]; //宠物喂养水果信息记录
	UINT32           dwFruitScore; //水果分数
	UINT32           dwReStartCost; //重摇元宝消耗
	UINT32           dwAMaxCost; //一键MAX元宝消耗
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_OPEN_PETFEED_ACK;

typedef struct tagPKT_CLIGS_FIRST_DRAW_REQ{
	tagPKT_CLIGS_FIRST_DRAW_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FIRST_DRAW_REQ)); }
	UINT8            byPetId; //宠物id
}PKT_CLIGS_FIRST_DRAW_REQ;

typedef struct tagPKT_CLIGS_FIRST_DRAW_ACK{
	tagPKT_CLIGS_FIRST_DRAW_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FIRST_DRAW_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byPetId; //宠物id
	UINT8            abyCurFruitType[MAX_PET_FEED_FRUIT_CNT]; //宠物喂养水果信息记录
	UINT32           dwFruitScore; //水果分数
	UINT32           dwIsCanFeed; //是否可以喂养
	UINT32           dwLeftFreeFeedCnt; //今日免费剩余喂养次数
	UINT32           dwVipFeedCostGold; //VIP喂养消耗元宝
	UINT32           dwTotalFeedCnt; //总共喂养的次数
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_FIRST_DRAW_ACK;

typedef struct tagPKT_CLIGS_RESET_DRAW_REQ{
	tagPKT_CLIGS_RESET_DRAW_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RESET_DRAW_REQ)); }
	UINT8            byPetId; //宠物id
	UINT8            byResetType; //0普通重置 1 一键Max重置
}PKT_CLIGS_RESET_DRAW_REQ;

typedef struct tagPKT_CLIGS_RESET_DRAW_ACK{
	tagPKT_CLIGS_RESET_DRAW_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RESET_DRAW_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byPetId; //宠物id
	UINT8            abyCurFruitType[MAX_PET_FEED_FRUIT_CNT]; //宠物喂养水果信息记录
	UINT32           dwFruitScore; //水果分数
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_RESET_DRAW_ACK;

typedef struct tagPKT_CLIGS_CONFIRM_FEED_REQ{
	tagPKT_CLIGS_CONFIRM_FEED_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CONFIRM_FEED_REQ)); }
	UINT8            byPetId; //宠物id
}PKT_CLIGS_CONFIRM_FEED_REQ;

typedef struct tagPKT_CLIGS_CONFIRM_FEED_ACK{
	tagPKT_CLIGS_CONFIRM_FEED_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CONFIRM_FEED_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA  stPetInfo; //宠物信息
}PKT_CLIGS_CONFIRM_FEED_ACK;

typedef struct tagPKT_CLIGS_MERGE_PET_REQ{
	tagPKT_CLIGS_MERGE_PET_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MERGE_PET_REQ)); }
	UINT8            byEatPetId; //吃的宠物id
	UINT8            byBeAtePetNum; //被吃的宠物数量
	UINT8            abyBeAtePetId[MAX_PET_NUM]; //被吃的宠物id
}PKT_CLIGS_MERGE_PET_REQ;

typedef struct tagPKT_CLIGS_MERGE_PET_ACK{
	tagPKT_CLIGS_MERGE_PET_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MERGE_PET_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA_LST stAllPetInfo; //更新所有宠物信息
}PKT_CLIGS_MERGE_PET_ACK;

typedef struct tagPKT_CLIGS_TEST_REQ{
	tagPKT_CLIGS_TEST_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_TEST_REQ)); }
	UINT8            byErrCode; //扩展位
}PKT_CLIGS_TEST_REQ;

typedef struct tagPKT_CLIGS_TEST_ACK{
	tagPKT_CLIGS_TEST_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_TEST_ACK)); }
	TCHAR            aszWenzi[1024]; //文字
}PKT_CLIGS_TEST_ACK;

typedef struct tagDT_PET_STORE_INFO{
	UINT16           wStoreId; //商城id
	UINT8            byCostType; //花费类型 根据这个字段判断在积分区 元宝区
	UINT32           dwCostNum; //花费数目
	DT_ITEM_DATA_CLI2 stStoreGood; //商城物品
	UINT8            byItemType; //类型
	UINT8            byLvLimit; //等级限制
}DT_PET_STORE_INFO;

typedef struct tagPKT_CLIGS_OPEN_PET_STORE_REQ{
	tagPKT_CLIGS_OPEN_PET_STORE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_STORE_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_OPEN_PET_STORE_REQ;

typedef struct tagPKT_CLIGS_OPEN_PET_STORE_ACK{
	tagPKT_CLIGS_OPEN_PET_STORE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_STORE_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wStoreItemNum; //商城道具数量
	DT_PET_STORE_INFO astStoreItems[MAX_PET_STORE_ITEM_NUM]; //商城道具信息
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_OPEN_PET_STORE_ACK;

typedef struct tagPKT_CLIGS_BUY_PET_STORE_ITEM_REQ{
	tagPKT_CLIGS_BUY_PET_STORE_ITEM_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_PET_STORE_ITEM_REQ)); }
	UINT16           wStoreItemId; //购买ID
}PKT_CLIGS_BUY_PET_STORE_ITEM_REQ;

typedef struct tagPKT_CLIGS_BUY_PET_STORE_ITEM_ACK{
	tagPKT_CLIGS_BUY_PET_STORE_ITEM_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BUY_PET_STORE_ITEM_ACK)); }
	UINT16           wErrCode; //错误码
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_BUY_PET_STORE_ITEM_ACK;

typedef struct tagPKT_CLIGS_OPEN_REFINEPET_REQ{
	tagPKT_CLIGS_OPEN_REFINEPET_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_REFINEPET_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_OPEN_REFINEPET_REQ;

typedef struct tagPKT_CLIGS_OPEN_REFINEPET_ACK{
	tagPKT_CLIGS_OPEN_REFINEPET_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_REFINEPET_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA_LST stAllPetInfo; //宠物信息
}PKT_CLIGS_OPEN_REFINEPET_ACK;

typedef struct tagPKT_CLIGS_REFINEPET_REQ{
	tagPKT_CLIGS_REFINEPET_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_REFINEPET_REQ)); }
	UINT16           wPetId1; //炼化宠物1
	UINT16           wPetId2; //炼化宠物2
}PKT_CLIGS_REFINEPET_REQ;

typedef struct tagPKT_CLIGS_REFINEPET_ACK{
	tagPKT_CLIGS_REFINEPET_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_REFINEPET_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA  stRefineGetPet; //炼化获得的宠物
	DT_PET_CLI_DATA_LST stAllPetInfo; //宠物信息
}PKT_CLIGS_REFINEPET_ACK;

typedef struct tagDT_BAG_ITEM_INFO{
	UINT16           wItemNum; //背包物品数量
	DT_ITEM_DATA_CLI2 stItemData; //背包物品信息物品
}DT_BAG_ITEM_INFO;

typedef struct tagPKT_CLIGS_OPEN_WASH_QUALITY_REQ{
	tagPKT_CLIGS_OPEN_WASH_QUALITY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_WASH_QUALITY_REQ)); }
	UINT16           wPetId; //宠物ID
}PKT_CLIGS_OPEN_WASH_QUALITY_REQ;

typedef struct tagDT_PET_WASHQUALITY_COST_ITEM{
	UINT32           dwItemId; //洗资质物品ID
	UINT32           dwItemNum; //洗资质物品数量
}DT_PET_WASHQUALITY_COST_ITEM;

typedef struct tagDT_PET_WASHQUALITY_COST_ITEM_LIST{
	UINT8            byNum; //数量
	DT_PET_WASHQUALITY_COST_ITEM astItemInfo[MAX_PET_WASH_PSYCHIC_ITEMTYPE_CNT]; //信息
	UINT32           dwLockCostGold; //锁住消耗的元宝
}DT_PET_WASHQUALITY_COST_ITEM_LIST;

typedef struct tagPKT_CLIGS_OPEN_WASH_QUALITY_ACK{
	tagPKT_CLIGS_OPEN_WASH_QUALITY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_WASH_QUALITY_ACK)); }
	UINT16           wErrCode; //错误码
	UINT32           dwSlotVal; //一格的最大值
	DT_PET_WASHQUALITY_COST_ITEM_LIST astCostItemInfo[MAX_PET_WASH_PSYCHIC_CELL_CNT]; //洗格子消耗的物品信息
	UINT32           dwBagItemNum; //洗资质物品数量
	DT_GOODS_DATA_CLI astBagItemInfo[MAX_BAG_OPEN_NUM]; //背包兑换洗资质物品信息
	TCHAR            aszWashDesc[128]; //洗资质描述
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_OPEN_WASH_QUALITY_ACK;

typedef struct tagPKT_CLIGS_WASH_QUALITY_REQ{
	tagPKT_CLIGS_WASH_QUALITY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_WASH_QUALITY_REQ)); }
	UINT16           wPetId; //宠物ID
	UINT8            byIsLock; //是否锁住0 不用 1 用
	UINT8            byWashCell; //洗哪个格子 取值范围 [1, MAX_PET_WASH_PSYCHIC_CELL_CNT] 注意是闭区间
}PKT_CLIGS_WASH_QUALITY_REQ;

typedef struct tagPKT_CLIGS_WASH_QUALITY_ACK{
	tagPKT_CLIGS_WASH_QUALITY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_WASH_QUALITY_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA  stPetData; //洗资质后的宠物数据
	DT_PET_WASHQUALITY_COST_ITEM_LIST astCostItemInfo[MAX_PET_WASH_PSYCHIC_CELL_CNT]; //洗格子消耗的物品信息
	UINT32           dwBagItemNum; //洗资质物品数量
	DT_GOODS_DATA_CLI astBagItemInfo[MAX_BAG_OPEN_NUM]; //背包兑换洗资质物品信息
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_WASH_QUALITY_ACK;

typedef struct tagPKT_CLIGS_CHANGE_PET_STATE_REQ{
	tagPKT_CLIGS_CHANGE_PET_STATE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_PET_STATE_REQ)); }
	UINT16           wPetId; //宠物ID
	UINT8            byPetState; //改变的状态 0 背包 1 跟随 2 出战
}PKT_CLIGS_CHANGE_PET_STATE_REQ;

typedef struct tagPKT_CLIGS_CHANGE_PET_STATE_ACK{
	tagPKT_CLIGS_CHANGE_PET_STATE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CHANGE_PET_STATE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA  stPetData; //改变后宠物数据
}PKT_CLIGS_CHANGE_PET_STATE_ACK;

typedef struct tagDT_PET_AFTER_INHERIT_DATA{
	UINT16           wPetId; //从那只宠物继承
	UINT8            byStarLv; //星阶
	UINT16           wLv; //等级
	DT_PET_WASHQUALITY_DATA astWashQualityData[MAX_PET_WASH_PSYCHIC_CELL_CNT]; //宠物洗练格子信息
	UINT32           dwCostGold; //元宝消耗
}DT_PET_AFTER_INHERIT_DATA;

typedef struct tagPKT_CLIGS_OPEN_PET_INHERIT_REQ{
	tagPKT_CLIGS_OPEN_PET_INHERIT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_INHERIT_REQ)); }
	UINT16           wPetId; //宠物ID
}PKT_CLIGS_OPEN_PET_INHERIT_REQ;

typedef struct tagPKT_CLIGS_OPEN_PET_INHERIT_ACK{
	tagPKT_CLIGS_OPEN_PET_INHERIT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_INHERIT_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wPetNum; //宠物数量
	DT_PET_AFTER_INHERIT_DATA astNormalAfterInheritData[MAX_PET_NUM]; //从普通继承的数值变化
	DT_PET_AFTER_INHERIT_DATA astPerfectAfterInheritData[MAX_PET_NUM]; //从元宝继承的数值变化
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
	UINT32           dwCostGold; //完美消耗消费的元宝
	UINT32           dwCostCoin; //普通消耗消费的铜钱
}PKT_CLIGS_OPEN_PET_INHERIT_ACK;

typedef struct tagPKT_CLIGS_PET_INHERIT_REQ{
	tagPKT_CLIGS_PET_INHERIT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PET_INHERIT_REQ)); }
	UINT16           wPetId; //宠物ID （吃别人的）
	UINT16           wInheritedPetId; //被继承宠物ID （被吃的）
	UINT8            byIsPerfectInherit; //是否为完美继承 0 否 1 是
}PKT_CLIGS_PET_INHERIT_REQ;

typedef struct tagPKT_CLIGS_PET_INHERIT_ACK{
	tagPKT_CLIGS_PET_INHERIT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PET_INHERIT_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA  stPetData; //改变后宠物数据
	DT_PET_CLI_DATA  stInheritedPetData; //改变后被继承宠物数据
	UINT16           wPetNum; //宠物数量
	DT_PET_AFTER_INHERIT_DATA astNormalAfterInheritData[MAX_PET_NUM]; //从普通继承的数值变化
	DT_PET_AFTER_INHERIT_DATA astPerfectAfterInheritData[MAX_PET_NUM]; //从元宝继承的数值变化
	DT_RSYNC_RES_ITEM_DATA stResInfo; //资源信息
}PKT_CLIGS_PET_INHERIT_ACK;

typedef struct tagPKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ{
	tagPKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ;

typedef struct tagPKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK{
	tagPKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byIsHaveFightPet; //是否有出战宠物 0 无 1有
	DT_PET_CLI_DATA  stPetInfo; //宠物信息
}PKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK;

typedef struct tagPKT_CLIGS_REQUEST_ALL_PET_INFO_REQ{
	tagPKT_CLIGS_REQUEST_ALL_PET_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_REQUEST_ALL_PET_INFO_REQ)); }
	UINT8            byPlaceholder; //扩展位，没意义
}PKT_CLIGS_REQUEST_ALL_PET_INFO_REQ;

typedef struct tagPKT_CLIGS_REQUEST_ALL_PET_INFO_ACK{
	tagPKT_CLIGS_REQUEST_ALL_PET_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_REQUEST_ALL_PET_INFO_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_CLI_DATA_LST stAllPetInfo; //宠物信息
}PKT_CLIGS_REQUEST_ALL_PET_INFO_ACK;

typedef struct tagDT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA_CLI_v410 astEquipInfo[MAX_BAG_OPEN_NUM]; //装备信息
	DT_EQUIP_DATA_STRENGTHEN_CLI astEquipStrengthenInfo[MAX_BAG_OPEN_NUM]; //装备强化信息
	UINT16           wEnchantEquipNum; //装备附魔个数（附魔建筑未开启前设置为0以节省流量）
	DT_EQUIP_ENCHANT_DATA_CLI astEquipEnchantInfo[MAX_BAG_OPEN_NUM]; //装备附魔信息
	UINT8            abyEquipOpenLightFlagInfo[MAX_BAG_OPEN_NUM]; //装备开光标志信息(0不可开光，1可开光，2已开光)
	UINT32           adwStrengthen10CoinInfo[MAX_BAG_OPEN_NUM]; //装备升10级所需要铜钱信息
}DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI;

typedef struct tagDT_HERO_ATTR_ADD_CLI_v410{
	UINT32           dwAddAttack; //攻击
	UINT32           dwAddHP; //血量
	UINT32           dwAddTalent; //资质
	UINT32           dwDefense; //防御
	UINT32           dwAddDefense; //附加防御
}DT_HERO_ATTR_ADD_CLI_v410;

typedef struct tagDT_RECRUIT_HERO_INFO{
	DT_HERO_BASE_DATA_CLI stHeroBaseInfo; //武将基本信息
	DT_HERO_ATTR_ADD_CLI_v410 stHeroAttrAddInfo; //武将属性加成信息
	DT_SKILL_DATA_CLI stSkillInfo; //武将技能信息
}DT_RECRUIT_HERO_INFO;

typedef struct tagDT_MATERIAL{
	UINT8            byMaterialType; //材料类型，分为：EIK_EQUIP、EIK_GOODS
	UINT16           wMaterialID; //材料ID
	UINT16           wCurNum; //拥有材料数
	UINT16           wRequiredNum; //所需材料数
	DT_INSTANCE_ID   stDropInstanceID; //该材料在哪个外传副本掉落
	UINT8            byGetFlag; //该材料的获得标识：0. 可获得 1. 提示：该材料所在副本未通关
}DT_MATERIAL;

typedef struct tagDT_MATERIAL_LIST{
	UINT8            byMaterialCnt; //材料类型，分为：EIK_EQUIP、EIK_GOODS
	DT_MATERIAL      astMaterialList[MAX_MATERIAL_NUM]; //招募材料列表
}DT_MATERIAL_LIST;

typedef struct tagDT_RECRUIT_HERO_DATA_v410{
	DT_RECRUIT_HERO_INFO stHeroInfo; //伙伴信息
	UINT8            byNeedVisitNum; //需拜访次数
	UINT8            byCurVisitNum; //当前已拜访次数
	UINT32           dwVisitCoin; //每次拜访铜币
	UINT16           wVisitGold; //出千元宝
	UINT16           wRecuitLevel; //招募等级
	UINT16           wRequireJingjieLevel; //招募所需境界阶等级
	UINT8            byRecuitUnLock; //是否可博弈
	UINT8            byRecommend; //是否推荐
	DT_MATERIAL_LIST stUpgradeMaterials; //进阶材料列表
	DT_MATERIAL_LIST stRecruitMaterials; //招募材料列表
	UINT8            byIsFlyHero; //是否是外传英雄
	UINT8            byCanUpgrade; //是否可进阶
	DT_RECRUIT_HERO_INFO astUpgradeHeroInfo[1]; //进阶后的伙伴信息
}DT_RECRUIT_HERO_DATA_v410;

typedef struct tagDT_EQUIP_MAKE_BOOK{
	UINT16           wBookID; //制作卷ID
	UINT16           wEquipID; //所制作的装备
	UINT16           wBookCnt; //拥有制作卷数
	UINT8            byIsMakable; //是否可制作
	UINT16           wEquipDressLvl; //装备可穿戴等级
	UINT8            byQuality; //品阶：天、地、玄、黄，见 FlyEquipQuality
	UINT8            byEquipType; //装备类型：头盔、战袍、武器、战靴，见 EEquipPosType
	DT_MATERIAL_LIST stMaterials; //制作材料列表
	UINT32           dwBuyGold; //补齐剩余材料所需元宝数
	UINT32           dwCostCoin; //制作所消耗铜钱
	DT_BATTLE_ATTRIBUTE2 stAttr; //装备属性
}DT_EQUIP_MAKE_BOOK;

typedef struct tagDT_EQUIP_MAKE_BOOK_INFO_CLI{
	UINT16           wBookID; //制作卷ID
	UINT32           dwBuyGold; //补齐剩余材料所需元宝数
}DT_EQUIP_MAKE_BOOK_INFO_CLI;

typedef struct tagDT_EQUIP_SERIES{
	UINT16           wSeriesID; //系列ID
	UINT16           wSeriesLevel; //该系列级别
	TCHAR            aszSeriesName[MAX_EQUIP_SERIES_NAME_LEN]; //该装备系列的名称
	UINT8            byMakeCnt; //该系列可制作的装备数
	UINT16           wMakeBookCnt; //该系列制作卷数
	DT_EQUIP_MAKE_BOOK astMakeBookList[MAX_MAKE_BOOKS_NUM_OF_1_SERIES]; //该系列制作卷列表
}DT_EQUIP_SERIES;

typedef struct tagDT_FLY_COST{
	UINT16           wItemID; //物品ID
	UINT16           wCurNum; //当前数量
	UINT16           wRequiredNum; //所需数量
}DT_FLY_COST;

typedef struct tagDT_FLY_GOAL{
	TCHAR            aszFlyGoalText[100]; //飞升条件文字
	UINT8            byIsAchieveFlyGoal; //是否已达成飞升条件:是true、否false
}DT_FLY_GOAL;

typedef struct tagDT_FLY_GOAL_LIST{
	UINT8            byFlyGoalCnt; //飞升条件数
	DT_FLY_GOAL      astFlyFoalList[MAX_FLY_GOAL_NUM]; //飞升条件列表
}DT_FLY_GOAL_LIST;

typedef struct tagPKT_CLIGS_EQUIP_ENCHANT_REQ{
	tagPKT_CLIGS_EQUIP_ENCHANT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_ENCHANT_REQ)); }
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT16           wLevel; //要升到的等级(每次只能升1级，0表示直接升级10级)
}PKT_CLIGS_EQUIP_ENCHANT_REQ;

typedef struct tagPKT_CLIGS_EQUIP_ENCHANT_ACK{
	tagPKT_CLIGS_EQUIP_ENCHANT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_ENCHANT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_EQUIP_DATA_CLI_v410 stEquipInfo; //装备信息
	DT_EQUIP_ENCHANT_DATA_CLI stEnchantInfo; //附魔信息
	DT_EQUIP_DATA_STRENGTHEN_CLI stEquipStrengthenInfo; //附魔之后所影响的装备强化信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT32           dwEnchantStoneNum; //当前附魔石数量
}PKT_CLIGS_EQUIP_ENCHANT_ACK;

typedef struct tagPKT_CLIGS_OPEN_SMITHY_ACK_v410{
	tagPKT_CLIGS_OPEN_SMITHY_ACK_v410() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SMITHY_ACK_v410)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wStrengthenCountDown; //装备强化倒计时
	UINT16           wStrengthenCD; //装备强化总CD
	UINT32           dwClrCDGold; //清除CD所需元宝
	DT_BUY_EQUIP_DATA astByEquipInfo[EQUIP_POS_NUM]; //可购买初级装备信息(数组下标+1为穿戴位置)
	UINT64           qwBlueGas; //当前蓝气
	DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI stEquipList; //装备武器列表
	UINT8            byBuildID; //客户端请求进入的建筑ID:强化/附魔
}PKT_CLIGS_OPEN_SMITHY_ACK_v410;

typedef struct tagPKT_CLIGS_OPEN_RECRUIT_ACK_v410{
	tagPKT_CLIGS_OPEN_RECRUIT_ACK_v410() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RECRUIT_ACK_v410)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wJingJieLevel; //境界等级
	UINT16           wHeroNum; //武将数目
	DT_RECRUIT_HERO_DATA_v410 astHeroList[MAX_HERO_NUM]; //武将信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_OPEN_RECRUIT_ACK_v410;

typedef struct tagPKT_CLIGS_UPGRADE_HERO_REQ{
	tagPKT_CLIGS_UPGRADE_HERO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_HERO_REQ)); }
	UINT16           wHeroID; //进阶伙伴ID
}PKT_CLIGS_UPGRADE_HERO_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_HERO_ACK{
	tagPKT_CLIGS_UPGRADE_HERO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_HERO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroID; //进阶伙伴ID
	DT_RECRUIT_HERO_DATA_v410 stUpgradeHero; //进阶后的伙伴信息
	DT_POWER_CHANGE_DATA stPowerChange; //战力变化信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的酒馆建筑提示信息
}PKT_CLIGS_UPGRADE_HERO_ACK;

typedef struct tagPKT_CLIGS_OPEN_EQUIP_MAKE_REQ{
	tagPKT_CLIGS_OPEN_EQUIP_MAKE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EQUIP_MAKE_REQ)); }
	UINT16           wSeriesID; //外传系列ID
}PKT_CLIGS_OPEN_EQUIP_MAKE_REQ;

typedef struct tagPKT_CLIGS_OPEN_EQUIP_MAKE_ACK{
	tagPKT_CLIGS_OPEN_EQUIP_MAKE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EQUIP_MAKE_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            bySeriesCnt; //外传系列数
	DT_EQUIP_SERIES  astSeriesList[MAX_SERIES_NUM]; //外传系列列表
	UINT8            byBuyMaterialVipLvl; //元宝补齐材料功能开放所需vip等级
}PKT_CLIGS_OPEN_EQUIP_MAKE_ACK;

typedef struct tagPKT_CLIGS_EQUIP_MAKE_REQ{
	tagPKT_CLIGS_EQUIP_MAKE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_MAKE_REQ)); }
	UINT16           wEquipBookID; //制作卷ID
	UINT16           wCurBookNum; //当前制作卷的数量(防止因网络问题造成服务器收到多次制作请求)
	UINT8            byIsBuyMaterial; //是否购买剩余材料
}PKT_CLIGS_EQUIP_MAKE_REQ;

typedef struct tagPKT_CLIGS_EQUIP_MAKE_ACK{
	tagPKT_CLIGS_EQUIP_MAKE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_MAKE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_EQUIP_MAKE_BOOK stEquipBook; //制作卷信息
	UINT64           qwCoin; //铜钱数
	UINT32           dwGold; //元宝数
	UINT16           wUpdateMakeBookCnt; //该系列更新的制作卷数
	DT_EQUIP_MAKE_BOOK_INFO_CLI astUpdateMakeBookList[MAX_MAKE_BOOKS_NUM_OF_1_SERIES]; //该系列更新的制作卷列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的铁匠铺建筑提示信息
}PKT_CLIGS_EQUIP_MAKE_ACK;

typedef struct tagPKT_CLIGS_OPEN_FLY_REQ{
	tagPKT_CLIGS_OPEN_FLY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FLY_REQ)); }
	UINT16           wHeroID; //伙伴
}PKT_CLIGS_OPEN_FLY_REQ;

typedef struct tagPKT_CLIGS_OPEN_FLY_ACK{
	tagPKT_CLIGS_OPEN_FLY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FLY_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byBasicAttrPromote; //基础属性额外加成
	DT_FLY_GOAL_LIST stFlyGoals; //飞升目标
	DT_FLY_COST      astFlyCostList[FLY_COST_MATERIAL_NUM]; //飞升消耗品列表
}PKT_CLIGS_OPEN_FLY_ACK;

typedef struct tagPKT_CLIGS_FLY_REQ{
	tagPKT_CLIGS_FLY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_FLY_REQ)); }
	UINT32           dwHeroID; //伙伴
}PKT_CLIGS_FLY_REQ;

typedef struct tagPKT_CLIGS_FLY_ACK{
	tagPKT_CLIGS_FLY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FLY_ACK)); }
	UINT16           wErrCode; //错误码
	UINT16           wHeroID; //伙伴
	UINT16           wHeroLvl; //飞升后的伙伴等级
	DT_EXPERIENCE_DATA stCoachHeroExpInfo; //主角的经验信息
	DT_POWER_CHANGE_DATA stPowerChange; //战力变化信息
}PKT_CLIGS_FLY_ACK;

typedef struct tagPKT_CLIGS_BATTLE_ACK5{
	tagPKT_CLIGS_BATTLE_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_BATTLE_ACK5)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_ENCOURAGE_DATA stBattleEncourage; //战斗奖励
	DT_DATA_AFTER_BATTLE_ENCOURAGE stAfterBattleEncourage; //领取战斗奖励后玩家信息
	UINT8            byPassFlag; //是否通关
	DT_PASS_ENCOURAGE_DATA astPassEncourage[1]; //通关奖励
	DT_DATA_AFTER_PASS_ENCOURAGE astAfterPassEncourage[1]; //领取通关奖励后玩家信息
	DT_PASS_ENCOURAGE_BOX_DATA astPassEncourageBox[1]; //通关宝箱奖励信息
	DT_UNLOCK_DATA   astUnlockInfo[1]; //解锁信息
	UINT8            byHaveNextInstance; //是否有下一个副本
	DT_NEXT_INSTANCE_DATA astNextInstanceInfo[1]; //下一个副本信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_PHYSTRENGTH_DATA_CLI stPhyStrengthInfo; //体力信息
	UINT8            abyGuide[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	TCHAR            aszExtContext[MAX_COMMON_TXT_LEN]; //扩展信息
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_BATTLE_ACK5;

typedef struct tagPKT_CLIGS_CHALLENGE_ACK5{
	tagPKT_CLIGS_CHALLENGE_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_CHALLENGE_ACK5)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_CHALLENGE_PLAYER_DATA stEnemyPlayerData; //对方玩家信息
	UINT8            byEnemyHeroNum; //对方信息
	DT_ENEMY_HERO_DATA astEnemyInfoList[MAX_FORMATION_IDX_NUM]; //对方信息
	DT_RACE_CHALLENGE_ENCOURAGE_DATA stChallengeEncourage; //挑战奖励
	UINT64           qwCurCoin; //玩家当前的铜钱
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝点
	UINT64           qwCurJingJie; //玩家当前的境界点
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_CHALLENGE_ACK5;

typedef struct tagPKT_CLIGS_PLUNDER_ACK5{
	tagPKT_CLIGS_PLUNDER_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_PLUNDER_ACK5)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwEnemyID; //要战斗的玩家ID
	UINT32           dwCaptureID; //俘虏玩家ID(和EnemyID相同表示掠夺的是CaptureID玩家，不同表示抢夺CaptureID俘虏)
	UINT8            byUseItemFlag; //是否使用道具掠夺低于5级以上的玩家
	UINT8            byResistFlag; //是否反抗(1表示反抗)
	UINT8            byCaptureGridIdx; //俘虏格子序号(从0开始)
	DT_BECAPTURE_DATA stBeCaptureInfo; //俘虏玩家的被俘信息
	DT_CHALLENGE_PLAYER_DATA stEnemyPlayerData; //战斗敌方玩家信息
	UINT8            byEnemyHeroNum; //战斗敌方信息
	DT_ENEMY_HERO_DATA astEnemyInfoList[MAX_FORMATION_IDX_NUM]; //战斗敌方信息
	DT_PLUNDER_ENCOURAGE_DATA stPlunderEncourage; //掠夺奖励
	UINT64           qwCurCoin; //玩家当前的铜币
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝
	UINT16           wCanPlunderNum; //玩家当前可掠夺次数
	UINT16           wVipExtNum; //VIP格外增加的次数
	UINT16           wFreePlunderNum; //玩家免费掠夺次数
	UINT32           dwPlunderGold; //收费掠夺元宝(若还有免费的则为0)
	UINT16           wPlunderItemNum; //当前拥有的掠夺道具
	UINT8            byCaptureFlag; //是否俘虏玩家，1为对方成为你的俘虏
	DT_CAPTURE_DATA_CLI astCaptureInfo[1]; //俘虏信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_PLUNDER_ACK5;

typedef struct tagPKT_CLIGS_PKBATTLE_ACK5{
	tagPKT_CLIGS_PKBATTLE_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_PKBATTLE_ACK5)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_PKBATTLE_ACK5;

typedef struct tagPKT_CLIGS_RDCHALLENGE_ACK5{
	tagPKT_CLIGS_RDCHALLENGE_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_RDCHALLENGE_ACK5)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_RDC_TAB_DATA  stTabInfo; //随机挑战面板信息
	DT_RDC_ENC_DATA  stEncInfo; //随机挑战奖励信息
	UINT64           qwCurJingJie; //当前境界点
	UINT64           qwCurPurpleGas; //当前紫气
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	UINT16           wKindID; //获得的鲜花ID
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_RDCHALLENGE_ACK5;

typedef struct tagPKT_CLIGS_CHALLENGE_MYSELF_ACK5{
	tagPKT_CLIGS_CHALLENGE_MYSELF_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_CHALLENGE_MYSELF_ACK5)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_CHALLENGE_MYSELF_ACK5;

typedef struct tagPKT_CLIGS_HUNTING_BATTLE_ACK5{
	tagPKT_CLIGS_HUNTING_BATTLE_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_HUNTING_BATTLE_ACK5)); }
	UINT16           wErrCode; //错误码,非0为错误
	DT_REWARD_INFO   stBattleReward; //战斗奖励
	DT_HUNTING_SCHEDULE_INFO stTaskScheduleInfo; //任务进度信息
	UINT32           dwClosedMonsterNeedGold; //收妖需要的元宝
	TCHAR            aszBattleLostMsg[MAX_FACTION_DESC_NUM]; //战斗失败信息
	UINT32           dwSyncDoorsTribute; //同步数据
	UINT8            byScheduleChangeFlag; //档数改变标识（0 没变， 1变）
	UINT8            byHuntingLeftNum; //捉妖次数同步
	UINT8            byIsGoldMode; //是否处于黄金模式
	DT_HUNTING_SCHEDULE_INFO2 stNewHuntingScheduleInfo; //捉妖进度信息
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_HUNTING_BATTLE_ACK5;

typedef struct tagDT_COURAGE_PLAYER_BATTLE_DATA5{
	UINT8            byBattleNum; //战斗次数
	DT_BATTLE_DATA5  astBattleInfo[MAX_COURAGE_MEMBER_NUM]; //战斗信息
}DT_COURAGE_PLAYER_BATTLE_DATA5;

typedef struct tagPKT_CLIGS_COURAGE_PLAYER_START_ACK5{
	tagPKT_CLIGS_COURAGE_PLAYER_START_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_PLAYER_START_ACK5)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_MONSTER_DATA_LIST stMonsterInfo; //怪物信息
	DT_COURAGE_GROUP_PLAYER_BATTLE_DATA stGroupPlayerInfo; //队伍信息
	UINT8            byPassFlag; //是否通关 0通关 1不通关
	DT_INSTANCE_ID   stInstanceID; //副本序号
	DT_COURAGE_JEWELRY_DATA_LST stjewelryInfoLst; //饰品信息
	UINT8            byHelpID; //协助描述 1再进行1次， 2协助两次送1次，3已经赠送不能在赠送了
	DT_COURAGE_PLAYER_BATTLE_DATA5 stBattleInfoCli; //战斗信息
}PKT_CLIGS_COURAGE_PLAYER_START_ACK5;

typedef struct tagPKT_CLIGS_COURAGE_LEADER_START_NTF5{
	tagPKT_CLIGS_COURAGE_LEADER_START_NTF5() { memset(this, 0, sizeof(tagPKT_CLIGS_COURAGE_LEADER_START_NTF5)); }
	PKT_CLIGS_COURAGE_PLAYER_START_ACK5 stBattleInfo; //战斗信息
}PKT_CLIGS_COURAGE_LEADER_START_NTF5;

typedef struct tagPKT_CLIGS_BEGIN_JIEBIAO_ACK5{
	tagPKT_CLIGS_BEGIN_JIEBIAO_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_JIEBIAO_ACK5)); }
	UINT16           wErrCode; //错误码
	UINT8            byYabiaoFlag; //押镖标识(0为不在押镖，1为正在押镖中，2为押镖完毕，可以领取奖励)
	UINT64           qwCoinEncourage; //奖励
	UINT32           dwScienceEncourage; //拦截器魂奖励
	UINT64           qwCurCoin; //当前铜钱数
	UINT64           qwCurScience; //当前器魂
	UINT32           dwBeJiebiaoPlayerID; //被拦截对象
	BIAO_OPERATE_INFO stBeJiebiaoInfo; //被劫信息
	UINT32           dwJiebiaoPlayerID; //拦截对象
	BIAO_OPERATE_INFO stJiebiaoInfo; //拦截信息
	UINT16           wJiebiaoCD; //冷却时间
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
	DT_BATTLE_DATA5  stBattleData; //战斗信息
}PKT_CLIGS_BEGIN_JIEBIAO_ACK5;

typedef struct tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK5{
	tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK5)); }
	UINT16           wErrCode; //错误码
	DT_BATTLE_DATA5  stBattleData; //战斗信息
}PKT_CLIGS_QUERY_BATTLE_RECORD_ACK5;

typedef struct tagPKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5{
	tagPKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5() { memset(this, 0, sizeof(tagPKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5)); }
	TCHAR            aszErrInfo[MAX_COMMON_TXT_LEN]; //错误信息
	TCHAR            aszBattleKey[MAX_COMMON_TXT_LEN]; //请求的战斗确认串
	UINT8            byPassFlag; //是否通关
	DT_PASS_ENCOURAGE_BOX_DATA astPassEncourageBox[1]; //通关宝箱奖励信息
	DT_DATA_AFTER_PASS_ENCOURAGE_BOX astResSync[1]; //通关资源同步信息
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5;

typedef struct tagPKT_CLIGS_OPEN_PET_RULE_REQ{
	tagPKT_CLIGS_OPEN_PET_RULE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_RULE_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_PET_RULE_REQ;

typedef struct tagDT_PET_BOOK_DATA{
	DT_PET_CLI_DATA  stPetInfo; //宠物信息
	UINT8            byPetState; //宠物状态(0正常，1灰掉,2问号)
}DT_PET_BOOK_DATA;

typedef struct tagDT_PET_BOOK_DATA_LST{
	UINT16           wPetNum; //宠物数量
	DT_PET_BOOK_DATA astPetData[MAX_PET_BOOK_NUM]; //宠物数据
	TCHAR            aszTitle[NORMAL_MSG_LEN]; //抬头
}DT_PET_BOOK_DATA_LST;

typedef struct tagPKT_CLIGS_OPEN_PET_RULE_ACK{
	tagPKT_CLIGS_OPEN_PET_RULE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PET_RULE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PET_BOOK_DATA_LST stLowPetInfo; //1-20级
	DT_PET_BOOK_DATA_LST stMiddlePetInfo; //20级以后
	DT_PET_BOOK_DATA_LST stHighPetInfo; //炼化宠物
	DT_ITEM_DATA_LIST_LESS_CLI3 stItemsInfo; //道具
	TCHAR            aszPetRule[MAX_PET_RULE_LEN]; //规则
	TCHAR            aszItemDesc[NORMAL_MSG_LEN]; //道具描述
}PKT_CLIGS_OPEN_PET_RULE_ACK;

typedef struct tagPKT_CLIGS_PLAYER_INFO_PET_REQ{
	tagPKT_CLIGS_PLAYER_INFO_PET_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_PET_REQ)); }
	UINT32           dwPlayerID; //对方玩家ID
}PKT_CLIGS_PLAYER_INFO_PET_REQ;

typedef struct tagPKT_CLIGS_PLAYER_INFO_PET_ACK{
	tagPKT_CLIGS_PLAYER_INFO_PET_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLAYER_INFO_PET_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byHavePet; //是否有宠物(0没有，1有)
	DT_PET_CLI_DATA  astPetInfo[1]; //宠物信息
}PKT_CLIGS_PLAYER_INFO_PET_ACK;

typedef struct tagDT_ITEM_EXCHANGE_INFO{
	UINT8            byItemNum; //道具个数
	DT_ITEM_DATA_CLI3 astItemList[MAX_DISPALY_GOODS_NUM]; //道具信息
	UINT16           awItemNeedNum[MAX_DISPALY_GOODS_NUM]; //道具需求个数
}DT_ITEM_EXCHANGE_INFO;

typedef struct tagDT_POTENCY_ATTR_INFO{
	UINT8            byPotencyID; //属性ID
	UINT32           dwPotencyValue; //属性值
}DT_POTENCY_ATTR_INFO;

typedef struct tagDT_POTENCY_ATTR_LST_INFO{
	UINT8            byAttrNum; //属性个数
	DT_POTENCY_ATTR_INFO astAttrInfo[MAX_SOUL_POTENCY_ADD_ATTR_NUM]; //属性信息
}DT_POTENCY_ATTR_LST_INFO;

typedef struct tagDT_POTENCY_ATTR_DATA{
	UINT8            byAttrNum; //属性个数
	DT_POTENCY_ATTR_INFO astAttrInfo[MAX_SOUL_POTENCY_ATTR_NUM]; //属性信息
	DT_ITEM_EXCHANGE_INFO stItemsInfo; //材料信息
	UINT16           wNeedSoulLevel; //升品需要的战魂等级
}DT_POTENCY_ATTR_DATA;

typedef struct tagDT_POTENCY_ATTR_DATA_LST{
	UINT8            byNum; //属性个数
	DT_POTENCY_ATTR_DATA astAttrValue[MAX_SOUL_POTENCY_SUB_LEVEL]; //属性值
}DT_POTENCY_ATTR_DATA_LST;

typedef struct tagDT_POTENCY_DATA{
	UINT8            byPotencyLevel; //潜能等阶
	DT_POTENCY_ATTR_DATA_LST stPotencyInfo; //潜能信息
}DT_POTENCY_DATA;

typedef struct tagDT_POTENCY_DATA_LST{
	UINT8            byPotencyNum; //潜能等阶数
	DT_POTENCY_DATA  astPotencyLst[MAX_SOUL_POTENCY_LEVEL]; //各层潜能信息
}DT_POTENCY_DATA_LST;

typedef struct tagPKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ{
	tagPKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ)); }
	DT_CLIGS_SOUL_MARK_DATA stSoulMark; //战魂标识
}PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ;

typedef struct tagPKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK{
	tagPKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_POTENCY_CLI stPotencyInfo; //当前潜能信息
	DT_POTENCY_DATA_LST stAllPotency; //所有加成信息
	DT_POTENCY_ATTR_LST_INFO stCountInfo; //统计信息
}PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK;

typedef struct tagPKT_CLIGS_SOUL_UPDATE_TYPE_REQ{
	tagPKT_CLIGS_SOUL_UPDATE_TYPE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SOUL_UPDATE_TYPE_REQ)); }
	DT_CLIGS_SOUL_MARK_DATA stSoulMark; //战魂标识
}PKT_CLIGS_SOUL_UPDATE_TYPE_REQ;

typedef struct tagPKT_CLIGS_SOUL_UPDATE_TYPE_ACK{
	tagPKT_CLIGS_SOUL_UPDATE_TYPE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SOUL_UPDATE_TYPE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SOUL_POTENCY_CLI stPotencyInfo; //当前潜能信息
	DT_POTENCY_ATTR_LST_INFO stCountInfo; //统计信息
	DT_ITEM_NUM_DATA_LIST_CLI stItemsInfo; //材料同步信息
	DT_POWER_CHANGE_DATA stPowerChangeInfo; //战力变化
}PKT_CLIGS_SOUL_UPDATE_TYPE_ACK;

typedef struct tagDT_ITEM_MARK_DATA{
	UINT16           wItemID; //物品ID
	UINT16           wItemIdx; //物品IDX
}DT_ITEM_MARK_DATA;

typedef struct tagDT_NEIDAN_BAG_MARK_LST_DATA{
	UINT8            byNum; //个数
	DT_ITEM_MARK_DATA astNeiDanInfo[MAX_NEI_DAN_BAG_NUM]; //信息
}DT_NEIDAN_BAG_MARK_LST_DATA;

typedef struct tagDT_NEIDAN_PET_MARK_LST_DATA{
	UINT8            byNum; //个数
	DT_ITEM_MARK_DATA astNeiDanInfo[MAX_PET_NEIDAN_NUM]; //信息
}DT_NEIDAN_PET_MARK_LST_DATA;

typedef struct tagPKT_CLIGS_DRESS_NEIDAN_REQ{
	tagPKT_CLIGS_DRESS_NEIDAN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_NEIDAN_REQ)); }
	UINT8            byDressType; //穿戴方式（ EDressType ）
	DT_ITEM_MARK_DATA stNeiDan; //内丹
	UINT16           wPetID; //宠物ID
}PKT_CLIGS_DRESS_NEIDAN_REQ;

typedef struct tagPKT_CLIGS_DRESS_NEIDAN_ACK{
	tagPKT_CLIGS_DRESS_NEIDAN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_NEIDAN_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byDressType; //穿戴方式
	DT_NEIDAN_PET_MARK_LST_DATA stPetNeiDan; //宠物内丹同步
	DT_NEIDAN_BAG_MARK_LST_DATA stBagNeiDan; //内丹背包同步
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
}PKT_CLIGS_DRESS_NEIDAN_ACK;

typedef struct tagDT_NEIDAN_CLI{
	DT_ITEM_MARK_DATA stNeiDan; //内丹ID
	UINT16           wNeiDanLevel; //内丹等级
	UINT32           dwCurExperience; //当前经验
	UINT32           dwMaxExperience; //内丹经验上限
	UINT64           qwBackExperience; //内丹返还经验
	UINT16           wDressReq; //穿戴条件
	UINT32           dwCostCoin; //吞噬耗费铜钱
	UINT32           dwLingLi; //加成灵力值
}DT_NEIDAN_CLI;

typedef struct tagDT_PET_NEIDAN_CLI{
	UINT8            byNum; //内丹数量
	DT_NEIDAN_CLI    astNeiDanInfo[MAX_PET_NEIDAN_NUM]; //信息
}DT_PET_NEIDAN_CLI;

typedef struct tagDT_BAG_NEIDAN_CLI{
	UINT8            byNum; //内丹数量
	DT_NEIDAN_CLI    astNeiDanInfo[MAX_NEI_DAN_BAG_NUM]; //信息
}DT_BAG_NEIDAN_CLI;

typedef struct tagPKT_CLIGS_OPEN_NEIDAN_REQ{
	tagPKT_CLIGS_OPEN_NEIDAN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_NEIDAN_REQ)); }
	UINT16           wPetID; //PetID
}PKT_CLIGS_OPEN_NEIDAN_REQ;

typedef struct tagPKT_CLIGS_OPEN_NEIDAN_ACK{
	tagPKT_CLIGS_OPEN_NEIDAN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_NEIDAN_ACK)); }
	UINT16           wErrCode; //错误码
	TCHAR            aszPetPic[MAX_PET_NAME_LENGTH]; //宠物图片
	DT_PET_NEIDAN_CLI stPetNeiDan; //宠物内丹
	DT_BAG_NEIDAN_CLI stBagNeiDan; //背包内丹
}PKT_CLIGS_OPEN_NEIDAN_ACK;

typedef struct tagPKT_CLIGS_UPDATA_NEIDAN_REQ{
	tagPKT_CLIGS_UPDATA_NEIDAN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPDATA_NEIDAN_REQ)); }
	DT_ITEM_MARK_DATA stNeiDan; //主内丹
	DT_NEIDAN_BAG_MARK_LST_DATA stDevorNeiDan; //被吞噬的内丹
}PKT_CLIGS_UPDATA_NEIDAN_REQ;

typedef struct tagPKT_CLIGS_UPDATA_NEIDAN_ACK{
	tagPKT_CLIGS_UPDATA_NEIDAN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPDATA_NEIDAN_ACK)); }
	UINT16           wErrCode; //错误码
	DT_NEIDAN_CLI    stNeiDan; //主内丹
	DT_NEIDAN_BAG_MARK_LST_DATA stBagNeiDan; //背包内丹
	DT_PLAYER_RES_DATA stResSync; //资源同步
	DT_POWER_CHANGE_DATA stPowerInfo; //战力变化
}PKT_CLIGS_UPDATA_NEIDAN_ACK;

typedef struct tagPKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ{
	tagPKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ;

typedef struct tagDT_NAIDAN_INSTANCE_DATA{
	DT_INSTANCE_ID   stInstanceID; //副本ID
	TCHAR            aszInstanceName[NORMAL_MSG_LEN]; //副本名称
	TCHAR            aszItemDesc[NORMAL_MSG_LEN]; //掉落物品
	UINT16           wPlayerLevelReq; //人物等级要求
	UINT8            byState; //副本状态（0正常（没能关），1通关，2不能挑战）
}DT_NAIDAN_INSTANCE_DATA;

typedef struct tagDT_NAIDAN_INSTANCE_LST_DATA{
	UINT8            byInstanceNum; //副本个数
	DT_NAIDAN_INSTANCE_DATA astInstanceInfo[MAX_NEIDAN_INSTANCE_NUM]; //副本信息
}DT_NAIDAN_INSTANCE_LST_DATA;

typedef struct tagPKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK{
	tagPKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PLAYER_RES_LST_DATA stResInfo; //资源信息
	UINT16           wCanBattleNum; //挑战信息
	DT_INSTANCE_ID   stSelectInstance; //锁定副本
	DT_NAIDAN_INSTANCE_LST_DATA stInstanceLst; //副本信息
}PKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK;

typedef struct tagPKT_CLIGS_NEIDAN_BATTLE_REQ{
	tagPKT_CLIGS_NEIDAN_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_NEIDAN_BATTLE_REQ)); }
	DT_INSTANCE_ID   stInstanceID; //副本ID
}PKT_CLIGS_NEIDAN_BATTLE_REQ;

typedef struct tagPKT_CLIGS_NEIDAN_BATTLE_ACK{
	tagPKT_CLIGS_NEIDAN_BATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_NEIDAN_BATTLE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage; //通关奖励宝箱信息
	DT_BATTLE_DATA5  stBattleInfo; //战斗信息
}PKT_CLIGS_NEIDAN_BATTLE_ACK;

typedef struct tagPKT_CLIGS_RECV_NEIDAN_AWARD_REQ{
	tagPKT_CLIGS_RECV_NEIDAN_AWARD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_NEIDAN_AWARD_REQ)); }
	UINT8            byExt; //扩展位
}PKT_CLIGS_RECV_NEIDAN_AWARD_REQ;

typedef struct tagPKT_CLIGS_RECV_NEIDAN_AWARD_ACK{
	tagPKT_CLIGS_RECV_NEIDAN_AWARD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_NEIDAN_AWARD_ACK)); }
	UINT16           wErrCode; //错误码
	DT_INSTANCE_ID   stNextInstance; //下一副本
	DT_PASS_ENCOURAGE_BOX_DATA stPassEncourage; //获得物品
}PKT_CLIGS_RECV_NEIDAN_AWARD_ACK;

typedef struct tagPKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ{
	tagPKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ)); }
	DT_INSTANCE_ID   stInstanceID; //副本ID
}PKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ;

typedef struct tagDT_ITEM_NUM_INFO{
	UINT16           wItemID; //物品ID
	UINT16           wItemNum; //物品数量
}DT_ITEM_NUM_INFO;

typedef struct tagDT_ITEM_NUM_INFO_LST{
	UINT8            byNum; //物品种类数
	DT_ITEM_NUM_INFO astItemInfo[MAX_GROUP_ITEM_NUM]; //物品信息
}DT_ITEM_NUM_INFO_LST;

typedef struct tagDT_SKIP_NEIDAN_AWARD_ITEM_INFO{
	UINT8            byMonsterNum; //波数
	DT_ITEM_NUM_INFO_LST astAwardItemInfoLst[MAX_NEIDAN_MONSTER_NUM]; //物品信息
}DT_SKIP_NEIDAN_AWARD_ITEM_INFO;

typedef struct tagPKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK{
	tagPKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK)); }
	UINT16           wErrCode; //错误码
	DT_SKIP_NEIDAN_AWARD_ITEM_INFO stAwardInfo; //奖励列表
}PKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoCliGS_MessageID
{
	CLIGS_ENTERGS_REQ               = 2000        ,
	CLIGS_ENTERGS_ACK               = 2001        ,
	CLIGS_GET_HERO_INFO_REQ         = 2004        ,
	CLIGS_GET_HERO_INFO_ACK         = 2005        ,
	CLIGS_ENTER_HOME_REQ            = 2006        ,
	CLIGS_ENTER_HOME_ACK            = 2007        ,
	CLIGS_ENTER_INSTANCE_REQ        = 2008        ,
	CLIGS_ENTER_INSTANCE_ACK        = 2009        ,
	CLIGS_BATTLE_REQ                = 2010        ,
	CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ  = 2012        ,
	CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK  = 2013        ,
	CLIGS_EXIT_INSTANCE_REQ         = 2014        ,
	CLIGS_EXIT_INSTANCE_ACK         = 2015        ,
	CLIGS_GET_SCENE_INSTANCE_REQ    = 2016        ,
	CLIGS_GET_SCENE_INSTANCE_ACK    = 2017        ,
	CLIGS_EQUIP_STRENGTHEN_REQ      = 2022        ,
	CLIGS_EQUIP_STRENGTHEN_ACK      = 2023        ,
	CLIGS_OPEN_SMITHY_REQ           = 2024        ,
	CLIGS_OPEN_SMITHY_ACK           = 2025        ,
	CLIGS_OPEN_RECRUIT_REQ          = 2026        ,
	CLIGS_OPEN_RECRUIT_ACK          = 2027        ,
	CLIGS_RECRUIT_HERO_REQ          = 2028        ,
	CLIGS_RECRUIT_HERO_ACK          = 2029        ,
	CLIGS_OPEN_BAG_REQ              = 2032        ,
	CLIGS_OPEN_BAG_ACK              = 2033        ,
	CLIGS_RECV_TASK_ENCOURAGE_REQ   = 2040        ,
	CLIGS_RECV_TASK_ENCOURAGE_ACK   = 2041        ,
	CLIGS_OPEN_RACE_BUILD_REQ       = 2042        ,
	CLIGS_OPEN_RACE_BUILD_ACK       = 2043        ,
	CLIGS_CHALLENGE_REQ             = 2044        ,
	CLIGS_RECV_RANK_ENCOURAGE_REQ   = 2046        ,
	CLIGS_RECV_RANK_ENCOURAGE_ACK   = 2047        ,
	CLIGS_OPEN_SHOP_REQ             = 2048        ,
	CLIGS_OPEN_SHOP_ACK             = 2049        ,
	CLIGS_BUY_ITEM_REQ              = 2050        ,
	CLIGS_BUY_ITEM_ACK              = 2051        ,
	CLIGS_SELL_ITEM_REQ             = 2052        ,
	CLIGS_SELL_ITEM_ACK             = 2053        ,
	CLIGS_DISBAND_HERO_REQ          = 2054        ,
	CLIGS_DISBAND_HERO_ACK          = 2055        ,
	CLIGS_EQUIP_REQ                 = 2056        ,
	CLIGS_EQUIP_ACK                 = 2057        ,
	CLIGS_REGISTER_USERNAME_NTF     = 2058        ,
	CLIGS_EMBATTLE_REQ              = 2060        ,
	CLIGS_EMBATTLE_ACK              = 2061        ,
	CLIGS_CLR_PLAYER_DATA_REQ       = 2062        ,
	CLIGS_OPEN_EMBATTLE_REQ         = 2064        ,
	CLIGS_OPEN_EMBATTLE_ACK         = 2065        ,
	CLIGS_MAKE_ITEM_REQ             = 2068        ,
	CLIGS_MAKE_ITEM_ACK             = 2069        ,
	CLIGS_ONHOOK_REQ                = 2074        ,
	CLIGS_ONHOOK_ACK                = 2075        ,
	CLIGS_RECV_ONLINE_ENCOURAGE_REQ  = 2078        ,
	CLIGS_RECV_ONLINE_ENCOURAGE_ACK  = 2079        ,
	CLIGS_INCREASE_BAG_SIZE_REQ     = 2080        ,
	CLIGS_INCREASE_BAG_SIZE_ACK     = 2081        ,
	CLIGS_CLR_CHALLENGE_CD_REQ      = 2086        ,
	CLIGS_CLR_CHALLENGE_CD_ACK      = 2087        ,
	CLIGS_SYNC_TIME_REQ             = 2088        ,
	CLIGS_SYNC_TIME_ACK             = 2089        ,
	CLILS_REGISTER_DSPNAME_REQ      = 2090        ,
	CLILS_REGISTER_DSPNAME_ACK      = 2091        ,
	CLILS_USE_GOODS_REQ             = 2092        ,
	CLILS_USE_GOODS_ACK             = 2093        ,
	CLILS_OPEN_LOGIN_ENCOURAGE_REQ  = 2094        ,
	CLILS_OPEN_LOGIN_ENCOURAGE_ACK  = 2095        ,
	CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ  = 2096        ,
	CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK  = 2097        ,
	CLILS_RATE_LOGIN_ENCOURAGE_REQ  = 2098        ,
	CLILS_RATE_LOGIN_ENCOURAGE_ACK  = 2099        ,
	CLIGS_GET_RANK_REQ              = 2100        ,
	CLIGS_GET_RANK_ACK              = 2101        ,
	CLIGS_GET_RACE_BATTLELOG_REQ    = 2102        ,
	CLIGS_GET_RACE_BATTLELOG_ACK    = 2103        ,
	CLIGS_ACCEPT_TASK_REQ           = 2104        ,
	CLIGS_ACCEPT_TASK_ACK           = 2105        ,
	CLIGS_GIVEUP_TASK_REQ           = 2106        ,
	CLIGS_GIVEUP_TASK_ACK           = 2107        ,
	CLIGS_PLUNDER_REQ               = 2108        ,
	CLIGS_OPEN_CAPTURE_TAB_REQ      = 2110        ,
	CLIGS_OPEN_CAPTURE_TAB_ACK      = 2111        ,
	CLIGS_COLLECT_INCOME_REQ        = 2112        ,
	CLIGS_COLLECT_INCOME_ACK        = 2113        ,
	CLIGS_BLEED_CAPTURE_REQ         = 2114        ,
	CLIGS_BLEED_CAPTURE_ACK         = 2115        ,
	CLIGS_DEL_ENEMY_REQ             = 2116        ,
	CLIGS_DEL_ENEMY_ACK             = 2117        ,
	CLIGS_OPEN_ENEMY_TAB_REQ        = 2118        ,
	CLIGS_OPEN_ENEMY_TAB_ACK        = 2119        ,
	CLIGS_PLUNDER_BATTLELOG_NTF     = 2126        ,
	CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ  = 2128        ,
	CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK  = 2129        ,
	CLIGS_FREE_CAPTURE_REQ          = 2132        ,
	CLIGS_FREE_CAPTURE_ACK          = 2133        ,
	CLIGS_OPEN_MONEY_TREE_REQ       = 2134        ,
	CLIGS_OPEN_MONEY_TREE_ACK       = 2135        ,
	CLIGS_ROCK_MONEY_TREE_REQ       = 2136        ,
	CLIGS_ROCK_MONEY_TREE_ACK       = 2137        ,
	CLIGS_GET_PLUNDER_BATTLELOG_REQ  = 2138        ,
	CLIGS_GET_PLUNDER_BATTLELOG_ACK  = 2139        ,
	CLIGS_OPEN_GODWEAPON_REQ        = 2140        ,
	CLIGS_OPEN_GODWEAPON_ACK        = 2141        ,
	CLIGS_UPGRADE_GODWEAPON_REQ     = 2142        ,
	CLIGS_UPGRADE_GODWEAPON_ACK     = 2143        ,
	CLIGS_FREE_GODWEAPON_REQ        = 2144        ,
	CLIGS_FREE_GODWEAPON_ACK        = 2145        ,
	CLIGS_OPEN_AWAKEN_TAB_REQ       = 2148        ,
	CLIGS_OPEN_AWAKEN_TAB_ACK       = 2149        ,
	CLIGS_AWAKEN_REQ                = 2150        ,
	CLIGS_AWAKEN_ACK                = 2151        ,
	CLIGS_OPEN_DRESS_STUDY_TAB_REQ  = 2154        ,
	CLIGS_OPEN_DRESS_STUDY_TAB_ACK  = 2155        ,
	CLIGS_DRESS_STUDY_REQ           = 2156        ,
	CLIGS_DRESS_STUDY_ACK           = 2157        ,
	CLIGS_STUDY_TO_EXP_REQ          = 2160        ,
	CLIGS_STUDY_TO_EXP_ACK          = 2161        ,
	CLIGS_STUDY_UPGRADE_REQ         = 2162        ,
	CLIGS_STUDY_UPGRADE_ACK         = 2163        ,
	CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ  = 2166        ,
	CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK  = 2167        ,
	CLIGS_RACE_BATTLELOG_NTF        = 2172        ,
	CLIGS_VISIT_HERO_REQ            = 2180        ,
	CLIGS_VISIT_HERO_ACK            = 2181        ,
	CLIGS_BUY_PHYSTRENGTH_REQ       = 2182        ,
	CLIGS_BUY_PHYSTRENGTH_ACK       = 2183        ,
	CLIGS_CLR_PLUNDER_CD_REQ        = 2184        ,
	CLIGS_CLR_PLUNDER_CD_ACK        = 2185        ,
	CLIGS_OPEN_MAINTOWN_REQ         = 2186        ,
	CLIGS_OPEN_MAINTOWN_ACK         = 2187        ,
	CLIGS_RECV_MAINTOWN_INCOME_REQ  = 2188        ,
	CLIGS_RECV_MAINTOWN_INCOME_ACK  = 2189        ,
	CLIGS_LEVY_MAINTOWN_INCOME_REQ  = 2190        ,
	CLIGS_LEVY_MAINTOWN_INCOME_ACK  = 2191        ,
	CLIGS_SET_WORK_TYPE_REQ         = 2192        ,
	CLIGS_SET_WORK_TYPE_ACK         = 2193        ,
	CLIGS_DRIVE_AWAY_OWNER_REQ      = 2194        ,
	CLIGS_DRIVE_AWAY_OWNER_ACK      = 2195        ,
	CLIGS_COMPOSE_EQUIP_REQ         = 2212        ,
	CLIGS_COMPOSE_EQUIP_ACK         = 2213        ,
	CLIGS_OPEN_PHYSTRENGTH_REQ      = 2214        ,
	CLIGS_OPEN_PHYSTRENGTH_ACK      = 2215        ,
	CLIGS_RECV_LEFT_INCOME_REQ      = 2216        ,
	CLIGS_RECV_LEFT_INCOME_ACK      = 2217        ,
	CLIGS_SMITHY_BUY_EQUIP_REQ      = 2218        ,
	CLIGS_SMITHY_BUY_EQUIP_ACK      = 2219        ,
	CLIGS_CLR_STRENGTHEN_CD_REQ     = 2220        ,
	CLIGS_CLR_STRENGTHEN_CD_ACK     = 2221        ,
	CLIGS_OPEN_CROP_REQ             = 2222        ,
	CLIGS_OPEN_CROP_ACK             = 2223        ,
	CLIGS_OPEN_CROP_FIELD_REQ       = 2224        ,
	CLIGS_OPEN_CROP_FIELD_ACK       = 2225        ,
	CLIGS_CROP_REQ                  = 2226        ,
	CLIGS_CROP_ACK                  = 2227        ,
	CLIGS_GAIN_CROP_REQ             = 2228        ,
	CLIGS_GAIN_CROP_ACK             = 2229        ,
	CLIGS_OPEN_SCIENCE_BUILD_REQ    = 2230        ,
	CLIGS_OPEN_SCIENCE_BUILD_ACK    = 2231        ,
	CLIGS_UPDATE_SCIENCE_REQ        = 2232        ,
	CLIGS_UPDATE_SCIENCE_ACK        = 2233        ,
	CLIGS_CLR_CROP_REQ              = 2234        ,
	CLIGS_CLR_CROP_ACK              = 2235        ,
	CLIGS_OPEN_EXPERIENCE_REQ       = 2236        ,
	CLIGS_OPEN_EXPERIENCE_ACK       = 2237        ,
	CLIGS_USER_EXPERIENCE_REQ       = 2238        ,
	CLIGS_USER_EXPERIENCE_ACK       = 2239        ,
	CLIGS_OPEN_MELT_EQUIP_REQ       = 2240        ,
	CLIGS_OPEN_MELT_EQUIP_ACK       = 2241        ,
	CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ  = 2244        ,
	CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK  = 2245        ,
	CLIGS_UPDATE_ATTR_REQ           = 2246        ,
	CLIGS_UPDATE_ATTR_ACK           = 2247        ,
	CLIGS_COMPOSE_REQ               = 2248        ,
	CLIGS_COMPOSE_ACK               = 2249        ,
	CLIGS_REFRESH_ELITE_REQ         = 2250        ,
	CLIGS_REFRESH_ELITE_ACK         = 2251        ,
	CLIGS_ONHOOK_ELITE_REQ          = 2252        ,
	CLIGS_ONHOOK_ELITE_ACK          = 2253        ,
	CLIGS_GET_ITEM_REQ              = 2254        ,
	CLIGS_GET_ITEM_ACK              = 2255        ,
	CLIGS_GET_ORDER_ID_REQ          = 2256        ,
	CLIGS_GET_ORDER_ID_ACK          = 2257        ,
	CLIGS_EXCHANGE_RESULT           = 2258        ,
	CLIGS_SEED_COMPOSE_REQ          = 2260        ,
	CLIGS_SEED_COMPOSE_ACK          = 2261        ,
	CLIGS_CHAT_REQ                  = 2262        ,
	CLIGS_CHAT_ACK                  = 2263        ,
	CLIGS_CHAT_NTF                  = 2264        ,
	CLIGS_CROSS_DAY_NTF             = 2266        ,
	CLIGS_OPNE_ACTIVITY_CENTER_REQ  = 2268        ,
	CLIGS_OPNE_ACTIVITY_CENTER_ACK  = 2269        ,
	CLIGS_OPNE_EAT_BREAD_REQ        = 2270        ,
	CLIGS_OPNE_EAT_BREAD_ACK        = 2271        ,
	CLIGS_OBTAIN_PHYSTRENGTHEN_REQ  = 2272        ,
	CLIGS_OBTAIN_PHYSTRENGTHEN_ACK  = 2273        ,
	CLIGS_LEAVE_EAT_BREAD_REQ       = 2274        ,
	CLIGS_LEAVE_EAT_BREAD_ACK       = 2275        ,
	CLIGS_OPEN_PAOSHANG_BUILD_REQ   = 2276        ,
	CLIGS_OPEN_PAOSHANG_BUILD_ACK   = 2277        ,
	CLIGS_OPEN_YABIAO_REQ           = 2278        ,
	CLIGS_OPEN_YABIAO_ACK           = 2279        ,
	CLIGS_FLUSH_BIAO_QUALITY_REQ    = 2280        ,
	CLIGS_FLUSH_BIAO_QUALITY_ACK    = 2281        ,
	CLIGS_CLIMB_TOWER_RESET_REQ     = 2282        ,
	CLIGS_CLIMB_TOWER_RESET_ACK     = 2283        ,
	CLIGS_CLIMB_TOWER_ONHOOK_REQ    = 2284        ,
	CLIGS_CLIMB_TOWER_ONHOOK_ACK    = 2285        ,
	CLIGS_BEGIN_YABIAO_REQ          = 2286        ,
	CLIGS_BEGIN_YABIAO_ACK          = 2287        ,
	CLIGS_OPEN_JIEBIAO_REQ          = 2288        ,
	CLIGS_OPEN_JIEBIAO_ACK          = 2289        ,
	CLIGS_BEGIN_JIEBIAO_REQ         = 2290        ,
	CLIGS_BE_JIEBIAO_NTF            = 2292        ,
	CLIGS_QUERY_BATTLE_RECORD_REQ   = 2294        ,
	CLIGS_QUERY_YABIAO_BATTLELOG_REQ  = 2296        ,
	CLIGS_QUERY_YABIAO_BATTLELOG_ACK  = 2297        ,
	CLIGS_QUERY_YABIAO_ENCOURAGE_REQ  = 2298        ,
	CLIGS_QUERY_YABIAO_ENCOURAGE_ACK  = 2299        ,
	CLIGS_UPGRADE_JINGJIE_REQ       = 2300        ,
	CLIGS_UPGRADE_JINGJIE_ACK       = 2301        ,
	CLIGS_GEN_GAS_BALL_REQ          = 2302        ,
	CLIGS_GEN_GAS_BALL_ACK          = 2303        ,
	CLIGS_UP_GAS_BALL_LEVEL_REQ     = 2304        ,
	CLIGS_UP_GAS_BALL_LEVEL_ACK     = 2305        ,
	CLIGS_COLLECT_GAS_REQ           = 2306        ,
	CLIGS_COLLECT_GAS_ACK           = 2307        ,
	CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ  = 2308        ,
	CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK  = 2309        ,
	CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ  = 2310        ,
	CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK  = 2311        ,
	CLIGS_OPEN_JINGJIE_TAB_REQ      = 2312        ,
	CLIGS_OPEN_JINGJIE_TAB_ACK      = 2313        ,
	CLIGS_OPEN_GEN_GAS_TAB_REQ      = 2314        ,
	CLIGS_OPEN_GEN_GAS_TAB_ACK      = 2315        ,
	CLIGS_OPEN_SKILL_TAB_REQ        = 2316        ,
	CLIGS_OPEN_SKILL_TAB_ACK        = 2317        ,
	CLIGS_CLEAR_JIEBIAO_CD_REQ      = 2318        ,
	CLIGS_CLEAR_JIEBIAO_CD_ACK      = 2319        ,
	CLIGS_OPEN_VIP_TAB_REQ          = 2320        ,
	CLIGS_OPEN_VIP_TAB_ACK          = 2321        ,
	CLIGS_OPEN_CLIMB_TOWER_REQ      = 2322        ,
	CLIGS_OPEN_CLIMB_TOWER_ACK      = 2323        ,
	CLIGS_BUY_CLIMB_TOWER_RESET_REQ  = 2324        ,
	CLIGS_BUY_CLIMB_TOWER_RESET_ACK  = 2325        ,
	CLIGS_OPEN_XIANGQIAN_TAB_REQ    = 2326        ,
	CLIGS_OPEN_XIANGQIAN_TAB_ACK    = 2327        ,
	CLIGS_XIANGQIAN_REQ             = 2330        ,
	CLIGS_XIANGQIAN_ACK             = 2331        ,
	CLIGS_CHECK_APPSTORE_ORDER_ID_REQ  = 2332        ,
	CLIGS_CHECK_APPSTORE_ORDER_ID_ACK  = 2333        ,
	CLIGS_REMOVE_XIANGQIAN_REQ      = 2334        ,
	CLIGS_REMOVE_XIANGQIAN_ACK      = 2335        ,
	CLIGS_RESET_ELITE_INSTANCE_REQ  = 2336        ,
	CLIGS_RESET_ELITE_INSTANCE_ACK  = 2337        ,
	CLIGS_ELITE_INSTANCE_ON_HOOK_REQ  = 2338        ,
	CLIGS_ELITE_INSTANCE_ON_HOOK_ACK  = 2339        ,
	CLIGS_GET_ELITE_INSTANCE_REQ    = 2340        ,
	CLIGS_GET_ELITE_INSTANCE_ACK    = 2341        ,
	CLIGS_QUICK_EQUIP_REQ           = 2342        ,
	CLIGS_QUICK_EQUIP_ACK           = 2343        ,
	CLIGS_GET_ACTIVITY_PROMPT_REQ   = 2344        ,
	CLIGS_GET_ACTIVITY_PROMPT_ACK   = 2345        ,
	CLIGS_OPEN_CHAT_REQ             = 2346        ,
	CLIGS_OPEN_CHAT_ACK             = 2347        ,
	CLIGS_BLUE_TO_PURPLE_REQ        = 2348        ,
	CLIGS_BLUE_TO_PURPLE_ACK        = 2349        ,
	CLIGS_PKBATTLE_REQ              = 2350        ,
	CLIGS_RESOURCES_PROMPT_REQ      = 2352        ,
	CLIGS_RESOURCES_PROMPT_ACK      = 2353        ,
	ENTER_BOSSB_REQ                 = 2354        ,
	ENTER_BOSSB_ACK                 = 2355        ,
	LEAVE_BOSSB_REQ                 = 2356        ,
	LEAVE_BOSSB_ACK                 = 2357        ,
	BOSSB_MOVE_REQ                  = 2358        ,
	BOSSB_MOVE_ACK                  = 2359        ,
	BOSSB_HURT_REQ                  = 2360        ,
	BOSSB_HURT_ACK                  = 2361        ,
	BOSSB_POS_NTF                   = 2362        ,
	BOSSB_OVER_NTF                  = 2366        ,
	OPEN_INSPIRE_TAB_REQ            = 2368        ,
	OPEN_INSPIRE_TAB_ACK            = 2369        ,
	INSPIRE_POWER_REQ               = 2370        ,
	INSPIRE_POWER_ACK               = 2371        ,
	BOSSB_RELIVE_REQ                = 2372        ,
	BOSSB_RELIVE_ACK                = 2373        ,
	BOSSB_GET_LAST_BOSSB_INFO_REQ   = 2374        ,
	BOSSB_GET_LAST_BOSSB_INFO_ACK   = 2375        ,
	CLILS_OPEN_RECHARGE_ENCOURAGE_REQ  = 2376        ,
	CLILS_OPEN_RECHARGE_ENCOURAGE_ACK  = 2377        ,
	CLIGS_RECV_RECHARGE_ENCOURAGE_REQ  = 2378        ,
	CLIGS_RECV_RECHARGE_ENCOURAGE_ACK  = 2379        ,
	CLILS_OPEN_SCORE_ENCOURAGE_REQ  = 2380        ,
	CLILS_OPEN_SCORE_ENCOURAGE_ACK  = 2381        ,
	CLIGS_RECV_SCORE_ENCOURAGE_REQ  = 2382        ,
	CLIGS_RECV_SCORE_ENCOURAGE_ACK  = 2383        ,
	CLIGS_GET_RANDOM_DSPNAME_REQ    = 2384        ,
	CLIGS_GET_RANDOM_DSPNAME_ACK    = 2385        ,
	CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ  = 2386        ,
	CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK  = 2387        ,
	CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ  = 2388        ,
	CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK  = 2389        ,
	CLIGS_EXPERIENCE_TRANSFER_REQ   = 2390        ,
	CLIGS_EXPERIENCE_TRANSFER_ACK   = 2391        ,
	CLIGS_OPEN_RDCHALLENGE_TAB_REQ  = 2392        ,
	CLIGS_OPEN_RDCHALLENGE_TAB_ACK  = 2393        ,
	CLIGS_RDCHALLENGE_REQ           = 2394        ,
	CLIGS_CLR_RDCHALLENGE_CD_REQ    = 2396        ,
	CLIGS_CLR_RDCHALLENGE_CD_ACK    = 2397        ,
	CLIGS_OPEN_WORSHIP_TAB_REQ      = 2398        ,
	CLIGS_OPEN_WORSHIP_TAB_ACK      = 2399        ,
	CLIGS_WORSHIP_REQ               = 2400        ,
	CLIGS_WORSHIP_ACK               = 2401        ,
	CLIGS_RECV_RATING_ENCOURAGE_REQ  = 2402        ,
	CLIGS_RECV_RATING_ENCOURAGE_ACK  = 2403        ,
	CLIGS_ACTIVITY_NOTIFY_REQ       = 2406        ,
	CLIGS_ACTIVITY_NOTIFY_ACK       = 2407        ,
	CLIGS_OPEN_DOGFECES_REQ         = 2408        ,
	CLIGS_OPEN_DOGFECES_ACK         = 2409        ,
	CLIGS_GET_ACTIVITY_CENTER_REQ   = 2410        ,
	CLIGS_GET_ACTIVITY_CENTER_ACK   = 2411        ,
	BOSSB_INFO_SYNC_NTF             = 2412        ,
	CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ  = 2414        ,
	CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK  = 2415        ,
	CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ  = 2416        ,
	CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK  = 2417        ,
	CLIGS_OPEN_GEM_MAKER_TAB_REQ    = 2418        ,
	CLIGS_OPEN_GEM_MAKER_TAB_ACK    = 2419        ,
	CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ  = 2420        ,
	CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK  = 2421        ,
	CLIGS_EXCHANGE_STUDY_REQ        = 2422        ,
	CLIGS_EXCHANGE_STUDY_ACK        = 2423        ,
	CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ  = 2424        ,
	CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK  = 2425        ,
	CLIGS_EQUIP_COMPOSE_REQ         = 2426        ,
	CLIGS_EQUIP_COMPOSE_ACK         = 2427        ,
	CLIGS_RECV_COMPOSE_EQUIP_REQ    = 2428        ,
	CLIGS_RECV_COMPOSE_EQUIP_ACK    = 2429        ,
	CLIGS_OPEN_FACTION_REQ          = 2430        ,
	CLIGS_OPEN_FACTION_ACK          = 2431        ,
	CLIGS_APPLY_FACTION_REQ         = 2432        ,
	CLIGS_APPLY_FACTION_ACK         = 2433        ,
	CLIGS_CANCEL_APPLY_FACTION_REQ  = 2434        ,
	CLIGS_CANCEL_APPLY_FACTION_ACK  = 2435        ,
	CLIGS_CREATE_FACTION_REQ        = 2436        ,
	CLIGS_CREATE_FACTION_ACK        = 2437        ,
	CLIGS_SEARCH_FACTION_REQ        = 2438        ,
	CLIGS_SEARCH_FACTION_ACK        = 2439        ,
	CLIGS_OPEN_OWNER_FACTION_REQ    = 2440        ,
	CLIGS_OPEN_OWNER_FACTION_ACK    = 2441        ,
	CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ  = 2442        ,
	CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK  = 2443        ,
	CLIGS_DOORS_TRIBUTE_FACTION_REQ  = 2444        ,
	CLIGS_DOORS_TRIBUTE_FACTION_ACK  = 2445        ,
	CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ  = 2446        ,
	CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK  = 2447        ,
	CLIGS_KICKED_FACTION_REQ        = 2448        ,
	CLIGS_KICKED_FACTION_ACK        = 2449        ,
	CLIGS_JOB_FACTION_REQ           = 2450        ,
	CLIGS_JOB_FACTION_ACK           = 2451        ,
	CLIGS_SET_NOTICE_FACTION_REQ    = 2452        ,
	CLIGS_SET_NOTICE_FACTION_ACK    = 2453        ,
	CLIGS_DISSOLVE_FACTION_REQ      = 2454        ,
	CLIGS_DISSOLVE_FACTION_ACK      = 2455        ,
	CLIGS_AUDIT_FACTION_REQ         = 2456        ,
	CLIGS_AUDIT_FACTION_ACK         = 2457        ,
	CLIGS_OPEN_APPLY_FACTION_REQ    = 2458        ,
	CLIGS_OPEN_APPLY_FACTION_ACK    = 2459        ,
	CLIGS_OPEN_RANK_FACTION_REQ     = 2460        ,
	CLIGS_OPEN_RANK_FACTION_ACK     = 2461        ,
	CLIGS_OPEN_AUDIT_FACTION_REQ    = 2462        ,
	CLIGS_OPEN_AUDIT_FACTION_ACK    = 2463        ,
	CLIGS_OPEN_GATHER_SCIENCE_REQ   = 2464        ,
	CLIGS_OPEN_GATHER_SCIENCE_ACK   = 2465        ,
	CLIGS_GATHER_SCIENCE_REQ        = 2466        ,
	CLIGS_GATHER_SCIENCE_ACK        = 2467        ,
	CLIGS_MELT_REQ                  = 2468        ,
	CLIGS_MELT_ACK                  = 2469        ,
	CLIGS_NOTICE_NTF                = 2470        ,
	CLIGS_ACTIVITY_ENC_NTF          = 2472        ,
	CLIGS_OPEN_AWAKEN_TAB_REQ2      = 2474        ,
	CLIGS_OPEN_AWAKEN_TAB_ACK2      = 2475        ,
	CLIGS_AWAKEN_REQ2               = 2476        ,
	CLIGS_AWAKEN_ACK2               = 2477        ,
	CLIGS_OPEN_DRESS_STUDY_TAB_REQ2  = 2478        ,
	CLIGS_OPEN_DRESS_STUDY_TAB_ACK2  = 2479        ,
	CLIGS_DRESS_STUDY_REQ2          = 2480        ,
	CLIGS_DRESS_STUDY_ACK2          = 2481        ,
	CLIGS_STUDY_TO_EXP_REQ2         = 2482        ,
	CLIGS_STUDY_TO_EXP_ACK2         = 2483        ,
	CLIGS_STUDY_UPGRADE_REQ2        = 2484        ,
	CLIGS_STUDY_UPGRADE_ACK2        = 2485        ,
	CLIGS_OPEN_BLUE_PAVILION_REQ    = 2486        ,
	CLIGS_OPEN_BLUE_PAVILION_ACK    = 2487        ,
	CLIGS_BEGBLUE_REQ               = 2488        ,
	CLIGS_BEGBLUE_ACK               = 2489        ,
	CLIGS_IMPROVELUCK_REQ           = 2490        ,
	CLIGS_IMPROVELUCK_ACK           = 2491        ,
	CLIGS_COMFIRM_BEGBLUE_REQ       = 2492        ,
	CLIGS_COMFIRM_BEGBLUE_ACK       = 2493        ,
	CLIGS_GET_OPENLIGHT_INFO_REQ    = 2508        ,
	CLIGS_GET_OPENLIGHT_INFO_ACK    = 2509        ,
	CLIGS_OPENLIGHT_INFO_REQ        = 2510        ,
	CLIGS_OPENLIGHT_INFO_ACK        = 2511        ,
	CLIGS_CHANGE_CAREER_REQ         = 2512        ,
	CLIGS_CHANGE_CAREER_ACK         = 2513        ,
	CLIGS_CHALLENGE_MYSELF_REQ      = 2514        ,
	CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ  = 2516        ,
	CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK  = 2517        ,
	CLIGS_OPEN_ITEM_EXCHANGE_REQ    = 2518        ,
	CLIGS_OPEN_ITEM_EXCHANGE_ACK    = 2519        ,
	CLIGS_ITEM_EXCHANGE_REQ         = 2520        ,
	CLIGS_ITEM_EXCHANGE_ACK         = 2521        ,
	CLIGS_OPEN_BIGREWARD_TAB_REQ    = 2522        ,
	CLIGS_OPEN_BIGREWARD_TAB_ACK    = 2523        ,
	CLIGS_OPEN_COLLECT_CLUE_TAB_REQ  = 2524        ,
	CLIGS_OPEN_COLLECT_CLUE_TAB_ACK  = 2525        ,
	CLIGS_OPEN_CLUE_GRID_REQ        = 2526        ,
	CLIGS_OPEN_CLUE_GRID_ACK        = 2527        ,
	CLIGS_SHAKE_DICE_REQ            = 2528        ,
	CLIGS_SHAKE_DICE_ACK            = 2529        ,
	CLIGS_REFRESH_DICE_REQ          = 2530        ,
	CLIGS_REFRESH_DICE_ACK          = 2531        ,
	CLIGS_COMFIRM_DICE_POINT_REQ    = 2532        ,
	CLIGS_COMFIRM_DICE_POINT_ACK    = 2533        ,
	CLIGS_PLAYER_RES_NTF            = 2536        ,
	CLIGS_OPERATING_ACTIVITY_NTF    = 2568        ,
	CLIGS_OPERATING_ACTIVITY_BATTLE_REQ  = 2538        ,
	CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ  = 2540        ,
	CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK  = 2541        ,
	CLIGS_DEL_FRIEND_REQ            = 2542        ,
	CLIGS_DEL_FRIEND_ACK            = 2543        ,
	CLIGS_SEARCH_PLAYER_REQ         = 2544        ,
	CLIGS_SEARCH_PLAYER_ACK         = 2545        ,
	CLIGS_VIEW_PLAYER_REQ           = 2546        ,
	CLIGS_VIEW_PLAYER_ACK           = 2547        ,
	CLIGS_ADD_FRIEND_REQ            = 2548        ,
	CLIGS_ADD_FRIEND_ACK            = 2549        ,
	CLIGS_OPEN_SEND_FLOWER_TAB_REQ  = 2550        ,
	CLIGS_OPEN_SEND_FLOWER_TAB_ACK  = 2551        ,
	CLIGS_SEND_FLOWER_REQ           = 2552        ,
	CLIGS_SEND_FLOWER_ACK           = 2553        ,
	CLIGS_GET_FLOWER_BY_PLAYERID_REQ  = 2554        ,
	CLIGS_GET_FLOWER_BY_PLAYERID_ACK  = 2555        ,
	CLIGS_GET_FLOWER_RANKING_REQ    = 2556        ,
	CLIGS_GET_FLOWER_RANKING_ACK    = 2557        ,
	CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ  = 2558        ,
	CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK  = 2559        ,
	CLIGS_SEND_PRIVATE_MSG_REQ      = 2560        ,
	CLIGS_SEND_PRIVATE_MSG_ACK      = 2561        ,
	CLIGS_DEL_PRIVATE_MSG_LOG_REQ   = 2562        ,
	CLIGS_DEL_PRIVATE_MSG_LOG_ACK   = 2563        ,
	CLIGS_ADD_TO_BLACKLIST_REQ      = 2564        ,
	CLIGS_ADD_TO_BLACKLIST_ACK      = 2565        ,
	CLIGS_MOVE_FROM_BLACKLIST_REQ   = 2566        ,
	CLIGS_MOVE_FROM_BLACKLIST_ACK   = 2567        ,
	CLIGS_ONLINE_PRIVATE_MSG_NTF    = 2570        ,
	CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ  = 2572        ,
	CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK  = 2573        ,
	CLIGS_READ_OFFLINE_MSG_REQ      = 2574        ,
	CLIGS_READ_OFFLINE_MSG_ACK      = 2575        ,
	CLIGS_OPEN_RAREITEMROOM_REQ     = 2576        ,
	CLIGS_OPEN_RAREITEMROOM_ACK     = 2577        ,
	CLIGS_BUY_RAREITEM_REQ          = 2578        ,
	CLIGS_BUY_RAREITEM_ACK          = 2579        ,
	CLIGS_OPEN_UPGRADERAREITEM_REQ  = 2580        ,
	CLIGS_OPEN_UPGRADERAREITEM_ACK  = 2581        ,
	CLIGS_UPGRADE_RAREITEM_REQ      = 2582        ,
	CLIGS_UPGRADE_RAREITEM_ACK      = 2583        ,
	CLIGS_OPEN_GYMNASIUM_REQ        = 2584        ,
	CLIGS_OPEN_GYMNASIUM_ACK        = 2585        ,
	CLIGS_LEARN_GYMNASIUM_SKILL_REQ  = 2586        ,
	CLIGS_LEARN_GYMNASIUM_SKILL_ACK  = 2587        ,
	CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ  = 2588        ,
	CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK  = 2589        ,
	CLIGS_GYMNASIUM_UPGRADE_REQ     = 2590        ,
	CLIGS_GYMNASIUM_UPGRADE_ACK     = 2591        ,
	CLIGS_OPEN_SHADE_REQ            = 2592        ,
	CLIGS_OPEN_SHADE_ACK            = 2593        ,
	CLIGS_OPEN_SHADE_UPGRADE_REQ    = 2594        ,
	CLIGS_OPEN_SHADE_UPGRADE_ACK    = 2595        ,
	CLIGS_SHADE_UPGRADE_REQ         = 2596        ,
	CLIGS_SHADE_UPGRADE_ACK         = 2597        ,
	CLIGS_OPEN_GOLDMODE_REQ         = 2598        ,
	CLIGS_OPEN_GOLDMODE_ACK         = 2599        ,
	CLIGS_OPEN_HUNTING_REQ          = 2600        ,
	CLIGS_OPEN_HUNTING_ACK          = 2601        ,
	CLIGS_FIND_MONSTER_REQ          = 2602        ,
	CLIGS_FIND_MONSTER_ACK          = 2603        ,
	CLIGS_HUNTING_BATTLE_REQ        = 2606        ,
	CLIGS_CHANGE_SCHEDEULE_NTF      = 2608        ,
	CLIGS_OPEN_HOT_SPRING_REQ       = 2610        ,
	CLIGS_OPEN_HOT_SPRING_ACK       = 2611        ,
	CLIGS_SELECT_WINE_REQ           = 2612        ,
	CLIGS_SELECT_WINE_ACK           = 2613        ,
	CLIGS_DRINK_WINE_REQ            = 2614        ,
	CLIGS_DRINK_WINE_ACK            = 2615        ,
	CLIGS_DRINK_WINE_MOVE_REQ       = 2616        ,
	CLIGS_DRINK_WINE_MOVE_ACK       = 2617        ,
	CLIGS_LEAVE_HOT_SPRING_REQ      = 2618        ,
	CLIGS_LEAVE_HOT_SPRING_ACK      = 2619        ,
	CLIGS_HOT_SPRING_POS_NTF        = 2620        ,
	CLIGS_CHANGE_OTHER_REQ          = 2622        ,
	CLIGS_CHANGE_OTHER_ACK          = 2623        ,
	CLIGS_GIVE_WINE_REQ             = 2626        ,
	CLIGS_GIVE_WINE_ACK             = 2627        ,
	CLIGS_CLICK_SELECT_WINE_REQ     = 2628        ,
	CLIGS_CLICK_SELECT_WINE_ACK     = 2629        ,
	CLIGS_SURE_GIVE_WINE_REQ        = 2630        ,
	CLIGS_SURE_GIVE_WINE_ACK        = 2631        ,
	CLIGS_CLOSED_MONSTER_REQ        = 2632        ,
	CLIGS_CLOSED_MONSTER_ACK        = 2633        ,
	CLIGS_FACTION_PLAYER_CHANGE_REQ  = 2634        ,
	CLIGS_FACTION_PLAYER_CHANGE_ACK  = 2635        ,
	CLIGS_GIVE_WINE_ACTIVITY_NTF    = 2636        ,
	CLIGS_LEAVE_FACTION_REQ         = 2638        ,
	CLIGS_LEAVE_FACTION_ACK         = 2639        ,
	CLIGS_OPEN_LEAVE_FACTION_REQ    = 2640        ,
	CLIGS_OPEN_LEAVE_FACTION_ACK    = 2641        ,
	CLIGS_OPEN_FACTION_UPGRADE_REQ  = 2642        ,
	CLIGS_OPEN_FACTION_UPGRADE_ACK  = 2643        ,
	CLIGS_FACTION_UPGRADE_REQ       = 2644        ,
	CLIGS_FACTION_UPGRADE_ACK       = 2645        ,
	CLIGS_OPEN_EQUIP_MASTER_REQ     = 2646        ,
	CLIGS_OPEN_EQUIP_MASTER_ACK     = 2647        ,
	CLIGS_ACTIVATE_MASTER_REQ       = 2648        ,
	CLIGS_ACTIVATE_MASTER_ACK       = 2649        ,
	CLIGS_BREAK_MASTER_REQ          = 2650        ,
	CLIGS_BREAK_MASTER_ACK          = 2651        ,
	CLIGS_GET_MASTER_BOOK_REQ       = 2652        ,
	CLIGS_GET_MASTER_BOOK_ACK       = 2653        ,
	CLIGS_WIPE_MASTER_BOOK_REQ      = 2654        ,
	CLIGS_WIPE_MASTER_BOOK_ACK      = 2655        ,
	CLIGS_MASTER_CHEAT_REQ          = 2722        ,
	CLIGS_MASTER_CHEAT_ACK          = 2723        ,
	CLIGS_CONFIRM_MASTER_BOOK_REQ   = 2656        ,
	CLIGS_CONFIRM_MASTER_BOOK_ACK   = 2657        ,
	CLIGS_OPEN_BAD_REWARD_REQ       = 2658        ,
	CLIGS_OPEN_BAD_REWARD_ACK       = 2659        ,
	CLIGS_GET_BAD_REWARD_REQ        = 2660        ,
	CLIGS_GET_BAD_REWARD_ACK        = 2661        ,
	CLIGS_OPEN_COURAGE_REQ          = 2662        ,
	CLIGS_OPEN_COURAGE_ACK          = 2663        ,
	CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ  = 2664        ,
	CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK  = 2665        ,
	CLIGS_COURAGE_CREATE_GROUP_REQ  = 2666        ,
	CLIGS_COURAGE_CREATE_GROUP_ACK  = 2667        ,
	CLIGS_COURAGE_JOIN_GROUP_REQ    = 2668        ,
	CLIGS_COURAGE_JOIN_GROUP_ACK    = 2669        ,
	CLIGS_COURAGE_OPERATE_HERO_REQ  = 2670        ,
	CLIGS_COURAGE_OPERATE_HERO_ACK  = 2671        ,
	CLIGS_COURAGE_CONFIRM_HERO_REQ  = 2672        ,
	CLIGS_COURAGE_CONFIRM_HERO_ACK  = 2673        ,
	CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ  = 2674        ,
	CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK  = 2675        ,
	CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ  = 2676        ,
	CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK  = 2677        ,
	CLIGS_COURAGE_PLAYER_READY_REQ  = 2678        ,
	CLIGS_COURAGE_PLAYER_READY_ACK  = 2679        ,
	CLIGS_COURAGE_PLAYER_START_REQ  = 2680        ,
	CLIGS_COURAGE_JOIN_GROUP_NTF    = 2682        ,
	CLIGS_COURAGE_QUIT_GROUP_NTF    = 2684        ,
	CLIGS_COURAGE_OPERATE_HERO_NTF  = 2686        ,
	CLIGS_COURAGE_PLAYER_READY_NTF  = 2688        ,
	CLIGS_COURAGE_PLAYER_FIRED_NTF  = 2690        ,
	CLIGS_COURAGE_LEAVE_GROUP_REQ   = 2692        ,
	CLIGS_COURAGE_LEAVE_GROUP_ACK   = 2693        ,
	CLIGS_OPEN_COURAGE_SHOP_REQ     = 2694        ,
	CLIGS_OPEN_COURAGE_SHOP_ACK     = 2695        ,
	CLIGS_PLAYER_FACTION_BATTLELOG_NTF  = 2696        ,
	CLIGS_PLAYER_FACTION_READ_REQ   = 2698        ,
	CLIGS_PLAYER_FACTION_READ_ACK   = 2699        ,
	CLIGS_OPEN_FIND_DRINK_REQ       = 2700        ,
	CLIGS_OPEN_FIND_DRINK_ACK       = 2701        ,
	CLIGS_OPEN_LOG_FACTION_REQ      = 2702        ,
	CLIGS_OPEN_LOG_FACTION_ACK      = 2703        ,
	CLIGS_COURAGE_BATTLE_REQ        = 2704        ,
	CLIGS_COURAGE_BATTLE_ACK        = 2705        ,
	CLIGS_COURAGE_BATTLE_NTF        = 2706        ,
	CLIGS_OPEN_FACTION_BUILD_GROUP_REQ  = 2708        ,
	CLIGS_OPEN_FACTION_BUILD_GROUP_ACK  = 2709        ,
	CLIGS_COURAGE_GROUP_DISMISS_NTF  = 2710        ,
	CLIGS_HOT_SPRING_REWARD_NTF     = 2712        ,
	CLIGS_COURAGE_GROUP_REFRESH_REQ  = 2714        ,
	CLIGS_COURAGE_GROUP_REFRESH_ACK  = 2715        ,
	CLIGS_COURAGE_ENTER_GROUP_REQ   = 2716        ,
	CLIGS_COURAGE_ENTER_GROUP_ACK   = 2717        ,
	CLIGS_LEAVE_COURAGE_REQ         = 2718        ,
	CLIGS_LEAVE_COURAGE_ACK         = 2719        ,
	CLIGS_HOT_SPRING_OVER_REWARD_REQ  = 2720        ,
	CLIGS_HOT_SPRING_OVER_REWARD_ACK  = 2721        ,
	CLIGS_OPEN_GOLD_NTF             = 2722        ,
	CLIGS_HUNTING_ITEGRAL_REQ       = 2724        ,
	CLIGS_HUNTING_ITEGRAL_ACK       = 2725        ,
	CLIGS_ACTIVITY_END_NTF          = 2726        ,
	CLIGS_PROMPT_IGNORE_REQ         = 2728        ,
	CLIGS_PROMPT_IGNORE_ACK         = 2729        ,
	CLIGS_COURAGE_GROUP_REFRESH_NTF  = 2730        ,
	CLIGS_HOME_BUILD_PROMPT_NTF     = 2732        ,
	CLIGS_OPEN_LEAVE_HOT_SPRING_REQ  = 2736        ,
	CLIGS_OPEN_LEAVE_HOT_SPRING_ACK  = 2737        ,
	CLIGS_FACTION_OPERATE_NTF       = 2738        ,
	CLIGS_FACTION_PROMPT_NTF        = 2740        ,
	CLIGS_COURAGE_GET_AWARD_REQ     = 2742        ,
	CLIGS_COURAGE_GET_AWARD_ACK     = 2743        ,
	CLIGS_COURAGE_GET_BATTLE_RESULT_REQ  = 2744        ,
	CLIGS_COURAGE_GET_BATTLE_RESULT_ACK  = 2745        ,
	CLIGS_DOORSTRIBUTE_SYNC_NTF     = 2746        ,
	CLIGS_AUDIT_FACTION_REQ2        = 2748        ,
	CLIGS_AUDIT_FACTION_ACK2        = 2749        ,
	CLIGS_AUDIT_ALL_REFUSE_REQ      = 2750        ,
	CLIGS_AUDIT_ALL_REFUSE_ACK      = 2751        ,
	CLIGS_OPEN_AUDIT_FACTION_REQ2   = 2752        ,
	CLIGS_OPEN_AUDIT_FACTION_ACK2   = 2753        ,
	CLIGS_OPEN_VIP_GIFT_REQ         = 2758        ,
	CLIGS_OPEN_VIP_GIFT_ACK         = 2759        ,
	CLIGS_GET_VIP_GIFT_REQ          = 2760        ,
	CLIGS_GET_VIP_GIFT_ACK          = 2761        ,
	CLIGS_UPGRADE_CROP_FIELD_REQ    = 2762        ,
	CLIGS_UPGRADE_CROP_FIELD_ACK    = 2763        ,
	CLIGS_OPEN_CROP_REQ2            = 2764        ,
	CLIGS_OPEN_CROP_ACK2            = 2765        ,
	CLIGS_OPEN_CROP_FIELD_REQ2      = 2766        ,
	CLIGS_OPEN_CROP_FIELD_ACK2      = 2767        ,
	CLIGS_CROP_REQ2                 = 2768        ,
	CLIGS_CROP_ACK2                 = 2739        ,
	CLIGS_GAIN_CROP_REQ2            = 2770        ,
	CLIGS_GAIN_CROP_ACK2            = 2771        ,
	CLIGS_CLR_CROP_REQ2             = 2772        ,
	CLIGS_CLR_CROP_ACK2             = 2773        ,
	CLIGS_OPEN_RECV_FLOWER_TAB_REQ  = 2774        ,
	CLIGS_OPEN_RECV_FLOWER_TAB_ACK  = 2775        ,
	CLIGS_ACCEPT_FLOWER_REQ         = 2776        ,
	CLIGS_ACCEPT_FLOWER_ACK         = 2777        ,
	CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ  = 2778        ,
	CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK  = 2779        ,
	CLIGS_BUY_RAREITEM_REQ2         = 2780        ,
	CLIGS_BUY_RAREITEM_ACK2         = 2781        ,
	CLIGS_LEARN_GYMNASIUM_SKILL_REQ2  = 2782        ,
	CLIGS_LEARN_GYMNASIUM_SKILL_ACK2  = 2783        ,
	CLIGS_SEND_FLOWER_EX_REQ        = 2784        ,
	CLIGS_SEND_FLOWER_EX_ACK        = 2785        ,
	CLIGS_FACTION_PLAYERS_INFO_REQ  = 2786        ,
	CLIGS_FACTION_PLAYERS_INFO_ACK  = 2787        ,
	CLIGS_CHANGE_FACTION_ICON_REQ   = 2788        ,
	CLIGS_CHANGE_FACTION_ICON_ACK   = 2789        ,
	CLIGS_OPEN_CHANGE_FACTION_ICON_REQ  = 2790        ,
	CLIGS_OPEN_CHANGE_FACTION_ICON_ACK  = 2791        ,
	CLIGS_FIND_BACK_DOORSTRIBUTE_REQ  = 2792        ,
	CLIGS_FIND_BACK_DOORSTRIBUTE_ACK  = 2793        ,
	CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ  = 2794        ,
	CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK  = 2795        ,
	CLIGS_LEAVE_COURAGE_BATTLE_REQ  = 2796        ,
	CLIGS_LEAVE_COURAGE_BATTLE_ACK  = 2797        ,
	CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ  = 2798        ,
	CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK  = 2799        ,
	CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2  = 2800        ,
	CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2  = 2801        ,
	CLIGS_SEARCH_PLAYER_REQ2        = 2802        ,
	CLIGS_SEARCH_PLAYER_ACK2        = 2803        ,
	CLIGS_OPEN_EMBATTLE_REQ2        = 2804        ,
	CLIGS_OPEN_EMBATTLE_ACK2        = 2805        ,
	CLIGS_UPGRADE_FORMATION_REQ     = 2806        ,
	CLIGS_UPGRADE_FORMATION_ACK     = 2807        ,
	CLIGS_OPEN_FORMATION_SCIENCE_REQ  = 2808        ,
	CLIGS_OPEN_FORMATION_SCIENCE_ACK  = 2809        ,
	CLILS_REGISTER_FACTION_REQ      = 2810        ,
	CLILS_REGISTER_FACTION_ACK      = 2811        ,
	CLIGS_UPGRADE_FORMATION_SCIENCE_REQ  = 2812        ,
	CLIGS_UPGRADE_FORMATION_SCIENCE_ACK  = 2813        ,
	CLIGS_GET_BLUE_ENCOURAGE_REQ    = 2814        ,
	CLIGS_GET_BLUE_ENCOURAGE_ACK    = 2815        ,
	CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ  = 2816        ,
	CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK  = 2817        ,
	CLIGS_OPEN_FACTION_SKILL_REQ    = 2818        ,
	CLIGS_OPEN_FACTION_SKILL_ACK    = 2819        ,
	CLIGS_RESOURCE_SYNC_NTF         = 2820        ,
	CLIGS_OPEN_PLAYER_PANEL_REQ     = 2822        ,
	CLIGS_OPEN_PLAYER_PANEL_ACK     = 2823        ,
	CLIGS_PLAYER_INFO_AWAKEN_REQ    = 2824        ,
	CLIGS_PLAYER_INFO_AWAKEN_ACK    = 2825        ,
	CLIGS_PLAYER_INFO_GODWEAPON_REQ  = 2826        ,
	CLIGS_PLAYER_INFO_GODWEAPON_ACK  = 2827        ,
	CLIGS_PLAYER_INFO_JEWELRY_REQ   = 2828        ,
	CLIGS_PLAYER_INFO_JEWELRY_ACK   = 2829        ,
	CLIGS_OPEN_RESOLVE_TRIGRAM_REQ  = 2830        ,
	CLIGS_OPEN_RESOLVE_TRIGRAM_ACK  = 2831        ,
	CLIGS_RESOLVE_TRIGRAM_REQ       = 2832        ,
	CLIGS_RESOLVE_TRIGRAM_ACK       = 2833        ,
	CLIGS_DRESS_JEWELRY_REQ         = 2834        ,
	CLIGS_DRESS_JEWELRY_ACK         = 2835        ,
	CLIGS_OPEN_HERO_SELECT_PANEL_REQ  = 2836        ,
	CLIGS_OPEN_HERO_SELECT_PANEL_ACK  = 2837        ,
	CLIGS_OPEN_JEWELRY_GENERATE_REQ  = 2838        ,
	CLIGS_OPEN_JEWELRY_GENERATE_ACK  = 2839        ,
	CLIGS_JEWELRY_GENERATE_INFO_REQ  = 2840        ,
	CLIGS_JEWELRY_GENERATE_INFO_ACK  = 2841        ,
	CLIGS_JEWELRY_GENERATE_REQ      = 2842        ,
	CLIGS_JEWELRY_GENERATE_ACK      = 2843        ,
	CLIGS_JEWELRY_STRENGTH_REQ      = 2844        ,
	CLIGS_JEWELRY_STRENGTH_ACK      = 2845        ,
	CLIGS_JEWELRY_RESOLVE_REQ       = 2846        ,
	CLIGS_JEWELRY_RESOLVE_ACK       = 2847        ,
	CLIGS_PLAYER_INFO_SKILL_REQ     = 2848        ,
	CLIGS_PLAYER_INFO_SKILL_ACK     = 2849        ,
	CLIGS_PLAYER_INFO_EQUIP_REQ     = 2850        ,
	CLIGS_PLAYER_INFO_EQUIP_ACK     = 2851        ,
	CLIGS_OPEN_JEWELRY_STRENGTH_REQ  = 2852        ,
	CLIGS_OPEN_JEWELRY_STRENGTH_ACK  = 2853        ,
	CLIGS_JEWELRY_COMPARE_REQ       = 2854        ,
	CLIGS_JEWELRY_COMPARE_ACK       = 2855        ,
	CLIGS_JEWELRY_EXT_UPGRADE_REQ   = 2856        ,
	CLIGS_JEWELRY_EXT_UPGRADE_ACK   = 2857        ,
	CLIGS_OPEN_JEWELRY_RESOLVE_REQ  = 2858        ,
	CLIGS_OPEN_JEWELRY_RESOLVE_ACK  = 2859        ,
	CLIGS_COURAGE_GROUP_OVERTIME_NTF  = 2860        ,
	CLIGS_BATTLE_ACK3               = 2861        ,
	CLIGS_CHALLENGE_ACK3            = 2863        ,
	CLIGS_PLUNDER_ACK3              = 2865        ,
	CLIGS_PKBATTLE_ACK3             = 2867        ,
	CLIGS_RDCHALLENGE_ACK3          = 2869        ,
	CLIGS_HUNTING_BATTLE_ACK2       = 2873        ,
	CLIGS_COURAGE_PLAYER_START_ACK2  = 2875        ,
	CLIGS_COURAGE_LEADER_START_NTF2  = 2876        ,
	CLIGS_BEGIN_JIEBIAO_ACK3        = 2877        ,
	CLIGS_QUERY_BATTLE_RECORD_ACK3  = 2879        ,
	CLIGS_OPEN_GVG_ACTIVITY_REQ     = 2882        ,
	CLIGS_GVG_APPLAY_ACK            = 2883        ,
	CLIGS_GVG_MONEY_CHAOS_ACK       = 2885        ,
	CLIGS_16_TO_8_TO_4_ACK          = 2887        ,
	CLIGS_FINAL_WAR_ACK             = 2889        ,
	CLIGS_GVG_CHAMPION_ACK          = 2927        ,
	CLIGS_GVG_ERR_MSG_ACK           = 2929        ,
	CLIGS_OPEN_GVG_FORMATION_REQ    = 2890        ,
	CLIGS_OPEN_GVG_FORMATION_ACK    = 2891        ,
	CLIGS_OPEN_ALREAD_APPLAY_REQ    = 2892        ,
	CLIGS_OPEN_ALREAD_APPLAY_ACK    = 2893        ,
	CLIGS_GVG_LAST_RESULT_REQ       = 2894        ,
	CLIGS_GVG_LAST_RESULT_ACK       = 2895        ,
	CLIGS_GVG_OPEN_MY_GAMBLE_REQ    = 2896        ,
	CLIGS_GVG_OPEN_MY_GAMBLE_ACK    = 2897        ,
	CLIGS_GVG_OPEN_GAMBLE_REQ       = 2898        ,
	CLIGS_GVG_OPEN_GAMBLE_ACK       = 2899        ,
	CLIGS_OPEN_GVG_RULE_REQ         = 2900        ,
	CLIGS_OPEN_GVG_RULE_ACK         = 2901        ,
	CLIGS_GVG_GAMBLE_REQ            = 2902        ,
	CLIGS_GVG_GAMBLE_ACK            = 2903        ,
	CLIGS_OPEN_FACTION_MONEY_RANK_REQ  = 2904        ,
	CLIGS_OPEN_FACTION_MONEY_RANK_ACK  = 2905        ,
	CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF  = 2908        ,
	CLIGS_OPEN_GVG_LOG_REQ          = 2910        ,
	CLIGS_OPEN_GVG_LOG_ACK          = 2911        ,
	CLIGS_SEE_GVG_VIDEO_REQ         = 2912        ,
	CLIGS_SEE_GVG_VIDEO_ACK_v400    = 2913        ,
	CLIGS_END_GVG_VIDEO_REQ         = 2914        ,
	CLIGS_END_GVG_VIDEO_ACK_v400    = 2915        ,
	CLIGS_SHOW_GVG_MVP_REQ          = 2916        ,
	CLIGS_SHOW_GVG_MVP_ACK          = 2917        ,
	CLIGS_READ_FORMATION_PLAYER_TIP_REQ  = 2918        ,
	CLIGS_READ_FORMATION_PLAYER_TIP_ACK  = 2919        ,
	CLIGS_GVG_FACTION_EMBATTLE_REQ  = 2920        ,
	CLIGS_GVG_FACTION_EMBATTLE_ACK  = 2921        ,
	CLIGS_OPEN_GVG_GIFT_REQ         = 2922        ,
	CLIGS_OPEN_GVG_GIFT_ACK         = 2923        ,
	CLIGS_GET_GVG_GIFT_REQ          = 2924        ,
	CLIGS_GET_GVG_GIFT_ACK          = 2925        ,
	CLIGS_BATTLE_ACK4               = 2935        ,
	CLIGS_CHALLENGE_ACK4            = 2937        ,
	CLIGS_PLUNDER_ACK4              = 2939        ,
	CLIGS_PKBATTLE_ACK4             = 2941        ,
	CLIGS_RDCHALLENGE_ACK4          = 2943        ,
	CLIGS_HUNTING_BATTLE_ACK3       = 2947        ,
	CLIGS_COURAGE_PLAYER_START_ACK3  = 2949        ,
	CLIGS_COURAGE_LEADER_START_NTF3  = 2950        ,
	CLIGS_BEGIN_JIEBIAO_ACK4        = 2953        ,
	CLIGS_QUERY_BATTLE_RECORD_ACK4  = 2955        ,
	CLIGS_SELECT_GROUP_TYPE_REQ     = 3000        ,
	CLIGS_SELECT_GROUP_TYPE_ACK     = 3001        ,
	CLIGS_GVG_FACTION_APPLAY_REQ    = 3002        ,
	CLIGS_GVG_FACTION_APPLAY_ACK    = 3003        ,
	CLIGS_READ_BATTLE_LOG_REQ       = 3004        ,
	CLIGS_READ_BATTLE_LOG_ACK       = 3005        ,
	CLIGS_UNREAD_BATTLE_NTF         = 3006        ,
	CLIGS_OPEN_DEVOUR_SOUL_REQ      = 3008        ,
	CLIGS_OPEN_DEVOUR_SOUL_ACK      = 3009        ,
	CLIGS_DEVOUR_SOUL_REQ           = 3010        ,
	CLIGS_DEVOUR_SOUL_ACK           = 3011        ,
	CLIGS_OPEN_REFLASH_SOUL_REQ     = 3012        ,
	CLIGS_OPEN_REFLASH_SOUL_ACK     = 3013        ,
	CLIGS_REFLASH_SOUL_REQ          = 3014        ,
	CLIGS_REFLASH_SOUL_ACK          = 3015        ,
	CLIGS_SAVE_REFLASH_SOUL_REQ     = 3016        ,
	CLIGS_SAVE_REFLASH_SOUL_ACK     = 3017        ,
	CLIGS_SOULREP_INFO_REQ          = 3020        ,
	CLIGS_SOULREP_INFO_ACK          = 3021        ,
	CLIGS_PRODUCT_SOUL_REQ          = 3024        ,
	CLIGS_PRODUCT_SOUL_ACK          = 3025        ,
	CLIGS_HERO_EQUIP_SOUL_REQ       = 3026        ,
	CLIGS_HERO_EQUIP_SOUL_ACK       = 3027        ,
	CLIGS_HERO_UNDRESS_SOUL_REQ     = 3028        ,
	CLIGS_HERO_UNDRESS_SOUL_ACK     = 3029        ,
	CLIGS_SOULEXCHANGE_INFO_REQ     = 3030        ,
	CLIGS_SOULEXCHANGE_INFO_ACK     = 3031        ,
	CLIGS_EXCHANGE_SOUL_REQ         = 3032        ,
	CLIGS_EXCHANGE_SOUL_ACK         = 3033        ,
	CLIGS_PLAYER_INFO_SOUL_REQ      = 3034        ,
	CLIGS_PLAYER_INFO_SOUL_ACK      = 3035        ,
	CLIGS_HERO_REPLACE_SOUL_REQ     = 3036        ,
	CLIGS_HERO_REPLACE_SOUL_ACK     = 3037        ,
	CLIGS_OPEN_MY_MEDAL_REQ         = 3038        ,
	CLIGS_OPEN_MY_MEDAL_ACK         = 3039        ,
	CLIGS_MEDAL_NTF                 = 3040        ,
	CLIGS_OPEN_PLAYER_PANEL_ACK_OLD  = 3043        ,
	CLIGS_VIEW_FACTION_MEMBERS_REQ  = 3044        ,
	CLIGS_VIEW_FACTION_MEMBERS_ACK  = 3045        ,
	CLIGS_OPEN_OUT_BOUND_REQ        = 3046        ,
	CLIGS_OPEN_OUT_BOUND_ACK        = 3047        ,
	CLIGS_OPEN_OUT_BOUND_MAP_REQ    = 3048        ,
	CLIGS_OPEN_OUT_BOUND_MAP_ACK    = 3049        ,
	CLIGS_OUT_BOUND_BATTLE_REQ      = 3050        ,
	CLIGS_OUT_BOUND_BATTLE_ACK      = 3051        ,
	CLIGS_TEST_REQ                  = 3052        ,
	CLIGS_TEST_ACK                  = 3053        ,
	CLIGS_OPEN_BAG_ACK2             = 3055        ,
	CLIGS_OPEN_PLAYER_PANEL_ACK_v410  = 3059        ,
	CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2  = 3060        ,
	CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2  = 3061        ,
	CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2  = 3062        ,
	CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2  = 3063        ,
	CLIGS_BATTLE_ACK5               = 3065        ,
	CLIGS_CHALLENGE_ACK5            = 3067        ,
	CLIGS_PLUNDER_ACK5              = 3069        ,
	CLIGS_PKBATTLE_ACK5             = 3071        ,
	CLIGS_RDCHALLENGE_ACK5          = 3073        ,
	CLIGS_CHALLENGE_MYSELF_ACK5     = 3075        ,
	CLIGS_HUNTING_BATTLE_ACK5       = 3077        ,
	CLIGS_COURAGE_PLAYER_START_ACK5  = 3079        ,
	CLIGS_COURAGE_LEADER_START_NTF5  = 3080        ,
	CLIGS_BEGIN_JIEBIAO_ACK5        = 3083        ,
	CLIGS_QUERY_BATTLE_RECORD_ACK5  = 3085        ,
	CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5  = 3087        ,
	CLIGS_OPEN_STORE_REQ            = 3088        ,
	CLIGS_OPEN_STORE_ACK            = 3089        ,
	CLIGS_EXCHANGE_GOODS_REQ        = 3090        ,
	CLIGS_EXCHANGE_GOODS_ACK        = 3091        ,
	CLIGS_NOTICE_NEW_NTF            = 3092        ,
	CLIGS_OPEN_PET_RULE_REQ         = 3094        ,
	CLIGS_OPEN_PET_RULE_ACK         = 3095        ,
	CLIGS_PETREP_INFO_REQ           = 3100        ,
	CLIGS_PETREP_INFO_ACK           = 3101        ,
	CLIGS_PRODUCT_PET_REQ           = 3102        ,
	CLIGS_PRODUCT_PET_ACK           = 3103        ,
	CLIGS_OPEN_REFINEPET_REQ        = 3104        ,
	CLIGS_OPEN_REFINEPET_ACK        = 3105        ,
	CLIGS_REFINEPET_REQ             = 3106        ,
	CLIGS_REFINEPET_ACK             = 3107        ,
	CLIGS_OPEN_STAR_PANEL_REQ       = 3108        ,
	CLIGS_OPEN_STAR_PANEL_ACK       = 3109        ,
	CLIGS_UP_STAR_LV_REQ            = 3110        ,
	CLIGS_UP_STAR_LV_ACK            = 3111        ,
	CLIGS_OPEN_PET_PANEL_REQ        = 3112        ,
	CLIGS_OPEN_PET_PANEL_ACK        = 3113        ,
	CLIGS_OPEN_PETFEED_REQ          = 3114        ,
	CLIGS_OPEN_PETFEED_ACK          = 3115        ,
	CLIGS_FIRST_DRAW_REQ            = 3116        ,
	CLIGS_FIRST_DRAW_ACK            = 3117        ,
	CLIGS_RESET_DRAW_REQ            = 3118        ,
	CLIGS_RESET_DRAW_ACK            = 3119        ,
	CLIGS_CONFIRM_FEED_REQ          = 3120        ,
	CLIGS_CONFIRM_FEED_ACK          = 3121        ,
	CLIGS_MERGE_PET_REQ             = 3122        ,
	CLIGS_MERGE_PET_ACK             = 3123        ,
	CLIGS_OPEN_PET_STORE_REQ        = 3124        ,
	CLIGS_OPEN_PET_STORE_ACK        = 3125        ,
	CLIGS_BUY_PET_STORE_ITEM_REQ    = 3126        ,
	CLIGS_BUY_PET_STORE_ITEM_ACK    = 3127        ,
	CLIGS_OPEN_WASH_QUALITY_REQ     = 3128        ,
	CLIGS_OPEN_WASH_QUALITY_ACK     = 3129        ,
	CLIGS_WASH_QUALITY_REQ          = 3130        ,
	CLIGS_WASH_QUALITY_ACK          = 3131        ,
	CLIGS_CHANGE_PET_STATE_REQ      = 3132        ,
	CLIGS_CHANGE_PET_STATE_ACK      = 3133        ,
	CLIGS_OPEN_PET_INHERIT_REQ      = 3134        ,
	CLIGS_OPEN_PET_INHERIT_ACK      = 3135        ,
	CLIGS_PET_INHERIT_REQ           = 3136        ,
	CLIGS_PET_INHERIT_ACK           = 3137        ,
	CLIGS_EQUIP_ENCHANT_REQ         = 3138        ,
	CLIGS_EQUIP_ENCHANT_ACK         = 3139        ,
	CLIGS_OPEN_SMITHY_ACK_v410      = 3141        ,
	CLIGS_OPEN_RECRUIT_ACK_v410     = 3143        ,
	CLIGS_UPGRADE_HERO_REQ          = 3144        ,
	CLIGS_UPGRADE_HERO_ACK          = 3145        ,
	CLIGS_OPEN_EQUIP_MAKE_REQ       = 3146        ,
	CLIGS_OPEN_EQUIP_MAKE_ACK       = 3147        ,
	CLIGS_EQUIP_MAKE_REQ            = 3148        ,
	CLIGS_EQUIP_MAKE_ACK            = 3149        ,
	CLIGS_OPEN_FLY_REQ              = 3150        ,
	CLIGS_OPEN_FLY_ACK              = 3151        ,
	CLIGS_FLY_REQ                   = 3152        ,
	CLIGS_FLY_ACK                   = 3153        ,
	CLIGS_OPEN_PLAYERPET_PANEL_REQ  = 3154        ,
	CLIGS_OPEN_PLAYERPET_PANEL_ACK  = 3155        ,
	CLIGS_PLAYER_INFO_PET_REQ       = 3156        ,
	CLIGS_PLAYER_INFO_PET_ACK       = 3157        ,
	CLIGS_SEE_GVG_VIDEO_ACK_v410    = 3159        ,
	CLIGS_END_GVG_VIDEO_ACK_v410    = 3161        ,
	BOSSB_HURT_ACK2                 = 3163        ,
	ENTER_BOSSB_ACK2                = 3165        ,
	BOSSB_INFO_SYNC_NTF2            = 3166        ,
	BOSSB_OVER_NTF2                 = 3168        ,
	BOSSB_GET_LAST_BOSSB_INFO_ACK2  = 3171        ,
	CLIGS_REQUEST_ALL_PET_INFO_REQ  = 3172        ,
	CLIGS_REQUEST_ALL_PET_INFO_ACK  = 3173        ,
	CLIGS_JEWELRY_RESOLVE_ACK2      = 3175        ,
	CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ  = 3176        ,
	CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK  = 3177        ,
	CLIGS_SOUL_UPDATE_TYPE_REQ      = 3178        ,
	CLIGS_SOUL_UPDATE_TYPE_ACK      = 3179        ,
	CLIGS_OPEN_NEIDAN_REQ           = 3180        ,
	CLIGS_OPEN_NEIDAN_ACK           = 3181        ,
	CLIGS_DRESS_NEIDAN_REQ          = 3182        ,
	CLIGS_DRESS_NEIDAN_ACK          = 3183        ,
	CLIGS_UPDATA_NEIDAN_REQ         = 3184        ,
	CLIGS_UPDATA_NEIDAN_ACK         = 3185        ,
	CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ  = 3186        ,
	CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK  = 3187        ,
	CLIGS_NEIDAN_BATTLE_REQ         = 3188        ,
	CLIGS_NEIDAN_BATTLE_ACK         = 3189        ,
	CLIGS_RECV_NEIDAN_AWARD_REQ     = 3190        ,
	CLIGS_RECV_NEIDAN_AWARD_ACK     = 3191        ,
	CLIGS_SKIP_NEIDAN_BATTLE_REQ    = 3192        ,
	CLIGS_SKIP_NEIDAN_BATTLE_ACK    = 3193        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodeDT_POWER_CHANGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POWER_CHANGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_BUFF_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_BUFF_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_ACTION_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_ACTION_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_EFFECT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_EFFECT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EFFECT_RECOMMEND_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EFFECT_RECOMMEND_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_EFFECT_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_EFFECT_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTACK_RANG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_RANG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_DATA_STRENGTHEN_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_STRENGTHEN_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_XIANGQIAN_DES(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_XIANGQIAN_DES(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ALL_EQUIP_XIANGQIAN_DES(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ALL_EQUIP_XIANGQIAN_DES(void *pHost, CNetData* poNetData);
INT32 EncodeDT_XIANGQIAN_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_XIANGQIAN_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_MELT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_MELT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOOD_MELT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOOD_MELT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_MELT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_MELT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOOD_MELT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOOD_MELT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_DATA_CLI_v410(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_CLI_v410(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_EQUIP_DATA_LST_CLI_v410(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EQUIP_DATA_LST_CLI_v410(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEED_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEED_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ESTIMATE_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ESTIMATE_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ESTIMATE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ESTIMATE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_ESTIMATE_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_ESTIMATE_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_ESTIMATE_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ESTIMATE_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_COMPOSE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_COMPOSE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SEED_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SEED_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOODS_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOODS_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PHYSTRENGTH_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PHYSTRENGTH_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEW_EQUIP_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_EQUIP_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FUNC_COMMON_PROMPT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FUNC_COMMON_PROMPT(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FUNC_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FUNC_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OTHER_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OTHER_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RES_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RES_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RSYNC_RES_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RSYNC_RES_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_LIST_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_LIST_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TASK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TASK_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GIVEN_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GIVEN_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACCEPT_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCEPT_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENCOURAGE_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENCOURAGE_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INVALID_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INVALID_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_TASK_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_TASK_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_TASK_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TASK_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACCEPT_TASK_COND_RECORD(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCEPT_TASK_COND_RECORD(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TASK_COND_RECORD_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_COND_RECORD_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EXPERIENCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXPERIENCE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUILD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ONLINE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONLINE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DATA_AFTER_ONLINE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_ONLINE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VIP_NUM_FUNC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_NUM_FUNC_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VIP_LEVEL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_LEVEL_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VIP_TAB_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_TAB_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UNREAD_BATTLE_LOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UNREAD_BATTLE_LOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RSYNC_RECV_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RSYNC_RECV_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OPERATING_ACTIVITY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OPERATING_ACTIVITY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTERGS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTERGS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_HERO_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTR_ENABLE_UPDATE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTR_ENABLE_UPDATE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_BATTLE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_BATTLE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_HERO_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVI_SIDE_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVI_SIDE_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVI_SIDE_PROMPT_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVI_SIDE_PROMPT_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTER_HOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_HOME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTER_HOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_HOME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DATA_AFTER_BATTLE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_BATTLE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RACE_CHALLENGE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_CHALLENGE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RACE_RANK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_RANK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTER_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PASS_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PASS_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DATA_AFTER_PASS_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_PASS_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PASS_ENCOURAGE_BOX_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PASS_ENCOURAGE_BOX_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PASS_ENCOURAGE_BOX_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PASS_ENCOURAGE_BOX_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PASS_ENCOURAGE_BOX_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PASS_ENCOURAGE_BOX_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_UPGRADE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPGRADE_DATA_LIST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTER_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BURST_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BURST_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEXT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEXT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UNLOCK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UNLOCK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BATTLE_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BATTLE_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXIT_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXIT_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXIT_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXIT_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TOWN_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TOWN_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_SCENE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_SCENE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CHAPTER_SCORE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHAPTER_SCORE(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACCUMULATED_SCORE_ENCOURAGE_BOX_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCUMULATED_SCORE_ENCOURAGE_BOX_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACCUMULATED_SCORE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCUMULATED_SCORE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_SCENE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_SCENE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_ENCHANT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_ENCHANT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_STRENGTHEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_STRENGTHEN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_STRENGTHEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_STRENGTHEN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SMITHY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SMITHY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUY_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUY_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SMITHY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SMITHY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RECRUIT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RECRUIT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_ATTR_ADD_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_ATTR_ADD_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECRUIT_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUIT_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RECRUIT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RECRUIT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECRUIT_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECRUIT_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECRUIT_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECRUIT_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_VISIT_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VISIT_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_VISIT_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VISIT_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_ATTR_EXT_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTR_EXT_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_ATTR_EXT_LST_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ATTR_EXT_LST_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JEWELRY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_ITEM_DATA_UNOIN2(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_ITEM_DATA_UNOIN2(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_CLI3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_CLI3(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_LIST_CLI3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_LIST_CLI3(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_LIST_CLI4(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_LIST_CLI4(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_NUM_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_NUM_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_NUM_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_NUM_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_LIST_LESS_CLI3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_LIST_LESS_CLI3(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RES_ITEM_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RES_ITEM_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BAG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BAG_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAG_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACCEPT_TASK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACCEPT_TASK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INSTANCE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACCEPT_TASK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACCEPT_TASK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GIVEUP_TASK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GIVEUP_TASK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GIVEUP_TASK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GIVEUP_TASK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_TASK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_TASK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_TASK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_TASK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RACE_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RACE_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CHALLENGE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHALLENGE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RACE_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RACE_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENEMY_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENEMY_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENEMY_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHALLENGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHALLENGE_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_RANK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RANK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_RANK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RANK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_RANK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RANK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RANK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RANK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_RANK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RANK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_RACE_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RACE_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RACE_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_RACE_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RACE_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SHOP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHOP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_SHOP(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_SHOP(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SHOP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHOP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELL_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELL_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELL_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELL_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DISBAND_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DISBAND_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DISBAND_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DISBAND_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_PANEL_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_PANEL_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_PANEL_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_PANEL_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REGISTER_USERNAME_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REGISTER_USERNAME_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_EMBATLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EMBATLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_PLAYER_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_PLAYER_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MAKE_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKE_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MAKE_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKE_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DATA_AFTER_PASS_ENCOURAGE_BOX(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_PASS_ENCOURAGE_BOX(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_INCREASE_BAG_SIZE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_INCREASE_BAG_SIZE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_INCREASE_BAG_SIZE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_INCREASE_BAG_SIZE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ONHOOK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_UPGRADE_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPGRADE_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DATA_AFTER_ONHOOK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_ONHOOK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_RES_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_RES_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_RES_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_RES_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ONHOOK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_CHALLENGE_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CHALLENGE_CD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_CHALLENGE_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CHALLENGE_CD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SYNC_TIME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SYNC_TIME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SYNC_TIME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SYNC_TIME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_REGISTER_DSPNAME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_DSPNAME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_REGISTER_DSPNAME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_DSPNAME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_USE_GOODS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_USE_GOODS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_USE_GOODS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_USE_GOODS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_LOGIN_ENCOURAG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOGIN_ENCOURAG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CAPTURE_WORK_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_WORK_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CAPTURE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLUNDER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BECAPTURE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BECAPTURE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLUNDER_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CAPTURE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CAPTURE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CAPTURE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CAPTURE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENEMY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_ENEMY_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ENEMY_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_ENEMY_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ENEMY_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEL_ENEMY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_ENEMY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEL_ENEMY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_ENEMY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLUNDER_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLUNDER_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FREE_CAPTURE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_CAPTURE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FREE_CAPTURE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_CAPTURE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COLLECT_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COLLECT_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COLLECT_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COLLECT_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BLEED_CAPTURE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BLEED_CAPTURE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BLEED_CAPTURE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BLEED_CAPTURE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SET_WORK_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SET_WORK_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SET_WORK_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SET_WORK_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MONEY_TREE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONEY_TREE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ROCK_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ROCK_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ROCK_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ROCK_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GODWEAPON_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GODWEAPON_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FREE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FREE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AWAKEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AWAKEN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_GET_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_GET_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AWAKEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AWAKEN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRESS_STUDY_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESS_STUDY_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_STUDY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_STUDY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_STUDY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_STUDY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_TO_EXP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_TO_EXP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_TO_EXP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_TO_EXP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RACE_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RACE_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_PLUNDER_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_PLUNDER_CD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_PLUNDER_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_PLUNDER_CD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MAINTOWN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MAINTOWN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MAINTOWN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MAINTOWN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRIVE_AWAY_OWNER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRIVE_AWAY_OWNER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRIVE_AWAY_OWNER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRIVE_AWAY_OWNER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMPOSE_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMPOSE_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVITY_TIME_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_TIME_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVITY_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_LEFT_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_LEFT_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_LEFT_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_LEFT_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SMITHY_BUY_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SMITHY_BUY_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SMITHY_BUY_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SMITHY_BUY_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_STRENGTHEN_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_STRENGTHEN_CD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_STRENGTHEN_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_STRENGTHEN_CD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_SEED_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_SEED_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_FIELD_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_FIELD_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_MORE_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_MORE_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_FIELD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_FIELD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_FIELD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_FIELD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GAIN_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GAIN_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GAIN_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GAIN_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EXPERIENCE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXPERIENCE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOLD_EXPERIENCE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOLD_EXPERIENCE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_USER_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USER_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_USER_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USER_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SCIENCE_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SCIENCE_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SCIENCE_ALREADY_OPEN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ALREADY_OPEN_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SCIENCE_WILL_OPEN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_WILL_OPEN_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SCIENCE_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SCIENCE_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPDATE_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPDATE_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MELT_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MELT_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MELT_GOOD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MELT_GOOD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MELT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MELT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MELT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MELT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPDATE_ATTR_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_ATTR_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPDATED_ATTR_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPDATED_ATTR_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPDATE_ATTR_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_ATTR_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFRESH_ELITE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_ELITE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFRESH_ELITE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_ELITE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ONHOOK_ELITE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_ELITE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ONHOOK_ELITE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_ELITE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXCHANGE_RESULT(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_RESULT(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHECK_APPSTORE_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHECK_APPSTORE_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEED_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEED_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEED_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEED_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeCHAT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeCHAT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CHAT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CHAT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CHAT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CHAT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHAT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHAT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHAT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHAT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHAT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHAT_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CROSS_DAY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CROSS_DAY_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPNE_ACTIVITY_CENTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPNE_ACTIVITY_CENTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPNE_ACTIVITY_CENTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_PROMPT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_PROMPT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_PROMPT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_PROMPT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPNE_EAT_BREAD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPNE_EAT_BREAD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPNE_EAT_BREAD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPNE_EAT_BREAD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OBTAIN_PHYSTRENGTHEN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OBTAIN_PHYSTRENGTHEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OBTAIN_PHYSTRENGTHEN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_EAT_BREAD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_EAT_BREAD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_EAT_BREAD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_EAT_BREAD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PAOSHANG_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BIAO_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BIAO_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeBIAO_OPERATE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeBIAO_OPERATE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PAOSHANG_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PAOSHANG_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_YABIAO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_YABIAO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_YABIAO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_YABIAO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FLUSH_BIAO_QUALITY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FLUSH_BIAO_QUALITY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FLUSH_BIAO_QUALITY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FLUSH_BIAO_QUALITY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_YABIAO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_YABIAO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_YABIAO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_YABIAO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JIEBIAO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JIEBIAO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_YABIAO_PLAYER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_PLAYER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_YABIAO_PLAYER_INFO_FACTION_NAME(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_PLAYER_INFO_FACTION_NAME(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JIEBIAO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JIEBIAO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_JIEBIAO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_JIEBIAO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_JIEBIAO_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_JIEBIAO_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLEAR_JIEBIAO_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLEAR_JIEBIAO_CD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLEAR_JIEBIAO_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLEAR_JIEBIAO_CD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BE_JIEBIAO_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BE_JIEBIAO_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_YABIAO_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_YABIAO_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_YABIAO_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_YABIAO_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_YABIAO_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_YABIAO_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_YABIAO_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_YABIAO_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_BATTLE_RECORD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_BATTLE_RECORD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLIMB_TOWER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIMB_TOWER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CLIMB_TOWER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CLIMB_TOWER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CLIMB_TOWER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CLIMB_TOWER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLIMB_TOWER_RESET_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLIMB_TOWER_RESET_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLIMB_TOWER_RESET_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLIMB_TOWER_RESET_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLIMB_TOWER_ONHOOK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLIMB_TOWER_ONHOOK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLIMB_TOWER_ONHOOK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_CLIMB_TOWER_RESET_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_CLIMB_TOWER_RESET_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JINGJIE_PER_LEVEL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JINGJIE_PER_LEVEL_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JINGJIE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JINGJIE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_JINGJIE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_JINGJIE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_JINGJIE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_JINGJIE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GEN_GAS_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GEN_GAS_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEN_GAS_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEN_GAS_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEN_GAS_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEN_GAS_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GEN_GAS_BALL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GEN_GAS_BALL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GEN_GAS_BALL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GEN_GAS_BALL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UP_GAS_BALL_LEVEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UP_GAS_BALL_LEVEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UP_GAS_BALL_LEVEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UP_GAS_BALL_LEVEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COLLECT_GAS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COLLECT_GAS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COLLECT_GAS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COLLECT_GAS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTACK_RANG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_RANG_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EFFECT_DATA_LST2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EFFECT_DATA_LST2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_TAB_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_TAB_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_SKILL_ACTION_LEVEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELECT_SKILL_ACTION_EFFECT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JINGJIE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JINGJIE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JINGJIE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JINGJIE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SKILL_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SKILL_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SKILL_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SKILL_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BLUE_TO_PURPLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BLUE_TO_PURPLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BLUE_TO_PURPLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BLUE_TO_PURPLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_VIP_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_VIP_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUY_GOLD_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUY_GOLD_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUY_GOLD_INFO_EX(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUY_GOLD_INFO_EX(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BUY_GOLD_TXT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUY_GOLD_TXT(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_VIP_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_VIP_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_XIANGQIAN_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_XIANGQIAN_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_XIANGQIAN_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_XIANGQIAN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_XIANGQIAN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_XIANGQIAN_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_XIANGQIAN_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_XIANGQIAN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_XIANGQIAN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REMOVE_XIANGQIAN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REMOVE_XIANGQIAN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REMOVE_XIANGQIAN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REMOVE_XIANGQIAN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESET_ELITE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESET_ELITE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ELITE_INSTANCE_VIP_EXT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITE_INSTANCE_VIP_EXT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESET_ELITE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESET_ELITE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ELITE_INSTANCE_ON_HOOK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ELITE_INSTANCE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITE_INSTANCE_RECORD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ELITE_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITE_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ELITE_INSTANCE_RECORD_SCORE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ELITE_INSTANCE_RECORD_SCORE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ELITE_INSTANCE_ON_HOOK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ELITE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ELITE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ELITE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ELITE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_ID(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_ID(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUICK_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUICK_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUICK_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUICK_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PKBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PKBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PKBATTLE_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PKBATTLE_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RESOURCES_PROMPT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RESOURCES_PROMPT(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RESOURCES_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RESOURCES_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESOURCES_PROMPT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESOURCES_PROMPT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESOURCES_PROMPT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESOURCES_PROMPT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_PLAYER_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_PLAYER_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_MY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_MY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HURT_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HURT_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_ENTER_BOSSB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_ENTER_BOSSB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_ENTER_BOSSB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_ENTER_BOSSB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_ENTER_BOSSB_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_ENTER_BOSSB_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_LEAVE_BOSSB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_LEAVE_BOSSB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_LEAVE_BOSSB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_LEAVE_BOSSB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_MOVE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_MOVE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_MOVE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_MOVE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_HURT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_HURT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_HURT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_HURT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_HURT_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_HURT_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_POS_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_POS_NTF(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_POS(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_POS(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_HURT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_HURT(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_INFO_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_INFO_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_INFO_SYNC_NTF2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_INFO_SYNC_NTF2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_OVER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_OVER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_OVER_INFO2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_OVER_INFO2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_OVER_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_OVER_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_OVER_NTF2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_OVER_NTF2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_OPEN_INSPIRE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_OPEN_INSPIRE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_INSPIRE_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSPIRE_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_OPEN_INSPIRE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_OPEN_INSPIRE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_INSPIRE_POWER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_INSPIRE_POWER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_INSPIRE_POWER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_INSPIRE_POWER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_RELIVE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_RELIVE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_RELIVE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_RELIVE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_GET_LAST_BOSSB_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_GET_LAST_BOSSB_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_GET_LAST_BOSSB_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_GET_LAST_BOSSB_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_GET_LAST_BOSSB_INFO_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_GET_LAST_BOSSB_INFO_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_RECHARGE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RECHARGE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_SCORE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_SCORE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_RANDOM_DSPNAME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RANDOM_DSPNAME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_RANDOM_DSPNAME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RANDOM_DSPNAME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BOSSB_BATTLE_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_UPGRADE_LEVEL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPGRADE_LEVEL(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXPERIENCE_TRANSFER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXPERIENCE_TRANSFER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXPERIENCE_TRANSFER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXPERIENCE_TRANSFER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RDCHALLENGE_POINT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RDCHALLENGE_POINT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RDCHALLENGE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RDC_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RDC_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RDC_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RDC_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RDC_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RDC_TAB_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RDCHALLENGE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RDCHALLENGE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RDCHALLENGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RDCHALLENGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RDCHALLENGE_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RDCHALLENGE_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_RDCHALLENGE_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_RDCHALLENGE_CD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_RDCHALLENGE_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_RDCHALLENGE_CD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_WORSHIP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WORSHIP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_WORSHIP_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WORSHIP_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_WORSHIP_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_WORSHIP_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_WORSHIP_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_WORSHIP_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_WORSHIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_WORSHIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_WORSHIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_WORSHIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACTIVITY_NOTIFY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACTIVITY_NOTIFY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACTIVITY_NOTIFY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACTIVITY_NOTIFY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DOGFECES_BUILD_TIP(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DOGFECES_BUILD_TIP(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DOGFECES_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DOGFECES_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DOGFECES_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DOGFECES_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_CENTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_CENTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_CENTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_CENTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVITY_ENCOURAGE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_ENCOURAGE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ACTIVITY_ENCOURAGE_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACTIVITY_ENCOURAGE_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GEM_MAKER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GEM_MAKER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEM_MAKER_TAB_EX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_RATING_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RATING_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_RATING_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RATING_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STUDY_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_TAB_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_TAB_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_STUDY_EXCHANGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_STUDY_EXCHANGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_EXCHANGE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXCHANGE_STUDY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_STUDY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXCHANGE_STUDY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_STUDY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COMPOSE_EQUIP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COMPOSE_EQUIP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_COMPOSE_LEVEL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_COMPOSE_LEVEL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EQUIP_COMPOSE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_COMPOSE_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_COMPOSE_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_COMPOSE_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_COMPOSE_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_DATA_TO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_DATA_TO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_BASE_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OPEN_FACTION_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OPEN_FACTION_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_BASE_OWNER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_OWNER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_OWNER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_OWNER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_OPEN_FACTION_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_OPEN_FACTION_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodePKT_CLIGS_APPLY_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_APPLY_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_APPLY_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_APPLY_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CANCEL_APPLY_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CANCEL_APPLY_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CANCEL_APPLY_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CANCEL_APPLY_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CREATE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CREATE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CREATE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CREATE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEARCH_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEARCH_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEARCH_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEARCH_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_OWNER_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_OWNER_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_AUDIT_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_AUDIT_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_AUDIT_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_AUDIT_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_OWNER_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_OWNER_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_LIST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_LIST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DOORS_TRIBUTE_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DOORS_TRIBUTE_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DOORS_TRIBUTE_TODAY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DOORS_TRIBUTE_TODAY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DOORS_TRIBUTE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_KICKED_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_KICKED_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_KICKED_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_KICKED_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JOB_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JOB_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JOB_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JOB_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SET_NOTICE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SET_NOTICE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SET_NOTICE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SET_NOTICE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DISSOLVE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DISSOLVE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DISSOLVE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DISSOLVE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_AUDIT_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_AUDIT_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AUDIT_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AUDIT_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AUDIT_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AUDIT_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_APPLY_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_APPLY_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_APPLY_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_APPLY_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RANK_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RANK_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RANK_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RANK_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AUDIT_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AUDIT_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AUDIT_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AUDIT_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_LOG_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_LOG_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_LOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_LOG_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_LOG_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_LOG_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_LOG_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GATHER_SCIENCE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GATHER_SCIENCE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GATHER_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GATHER_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GATHER_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GATHER_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GATHER_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GATHER_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GATHER_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GATHER_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_NOTICE_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_NOTICE_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_NOTICE_NEW_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_NOTICE_NEW_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACTIVITY_ENC_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACTIVITY_ENC_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_TAB_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_TAB_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_TAB_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_TAB_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AWAKEN_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AWAKEN_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AWAKEN_GET_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AWAKEN_GET_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AWAKEN_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AWAKEN_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_STUDY_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_STUDY_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRESS_STUDY_TAB_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRESS_STUDY_TAB_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRESS_STUDY_TAB_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_STUDY_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_STUDY_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_STUDY_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_STUDY_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_TO_EXP_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_TO_EXP_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_TO_EXP_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_TO_EXP_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_UPGRADE_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_UPGRADE_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_STUDY_UPGRADE_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_UPGRADE_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BEGBLUE_PLAYER_RES_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_PLAYER_RES_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BEGBLUE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_IMPROVELUCK_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_IMPROVELUCK_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BEGBLUE_RES_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_RES_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BEGBLUE_RESULT_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BEGBLUE_RESULT_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_BLUE_PAVILION_UNION_DATA_CLI(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_BLUE_PAVILION_UNION_DATA_CLI(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_BLUE_PAVILION_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BLUE_PAVILION_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BLUE_ENCOURAGEINFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BLUE_ENCOURAGEINFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BLUE_ENCOURAGEINFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BLUE_ENCOURAGEINFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BLUE_PAVILION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BLUE_PAVILION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BLUE_PAVILION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BLUE_PAVILION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGBLUE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGBLUE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGBLUE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGBLUE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_IMPROVELUCK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_IMPROVELUCK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_IMPROVELUCK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_IMPROVELUCK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMFIRM_BEGBLUE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMFIRM_BEGBLUE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMFIRM_BEGBLUE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMFIRM_BEGBLUE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_OPENLIGHT_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_OPENLIGHT_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_OPENLIGHT_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_OPENLIGHT_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPENLIGHT_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPENLIGHT_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPENLIGHT_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPENLIGHT_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_CAREER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_CAREER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_CAREER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_CAREER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHALLENGE_MYSELF_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_MYSELF_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ITEM_EXCHANGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeITEM_NUM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeITEM_NUM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeITEM_EXCHANGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeITEM_EXCHANGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ITEM_EXCHANGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ITEM_EXCHANGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ITEM_EXCHANGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ITEM_EXCHANGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_WANTED_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WANTED_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_WANTED_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_WANTED_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BIGREWARD_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BIGREWARD_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BIGREWARD_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BIGREWARD_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLUE_TASK_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLUE_TASK_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLUE_TASK_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLUE_TASK_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLUE_TASK_KILL_MONSTER_NAVI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLUE_TASK_KILL_MONSTER_NAVI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLUE_TASK_ITEM_BOX_NAVI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLUE_TASK_ITEM_BOX_NAVI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLUE_TASK_FORTUNE_GRID_NAVI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLUE_TASK_FORTUNE_GRID_NAVI(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_CLUE_TASK_NAVI_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_CLUE_TASK_NAVI_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_CLUE_TASK_NAVI_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLUE_TASK_NAVI_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DICE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DICE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_COLLECT_CLUE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_COLLECT_CLUE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_COLLECT_CLUE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_COLLECT_CLUE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CLUE_GRID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CLUE_GRID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CLUE_GRID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CLUE_GRID_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SHAKE_DICE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SHAKE_DICE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SHAKE_DICE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SHAKE_DICE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFRESH_DICE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_DICE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFRESH_DICE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_DICE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMFIRM_DICE_POINT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMFIRM_DICE_POINT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COMFIRM_DICE_POINT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMFIRM_DICE_POINT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_RES_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_RES_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPERATING_ACTIVITY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPERATING_ACTIVITY_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPERATING_ACTIVITY_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FRIEND_SYS_PLAYER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIEND_SYS_PLAYER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FRIEND_SYS_PLAYER_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIEND_SYS_PLAYER_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FRIEND_SYS_PLAYER_INFO2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIEND_SYS_PLAYER_INFO2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FRIEND_SYS_PLAYER_LIST_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIEND_SYS_PLAYER_LIST_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FRIEND_SYS_PLAYER_LIST_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEL_FRIEND_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_FRIEND_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEL_FRIEND_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_FRIEND_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEARCH_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEARCH_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEARCH_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEARCH_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEARCH_PLAYER_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEARCH_PLAYER_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEARCH_PLAYER_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEARCH_PLAYER_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FRIEND_SYS_PLAYER_GENERAL_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIEND_SYS_PLAYER_GENERAL_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_VIEW_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VIEW_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_ATTR_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_ATTR_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SPECIAL_EFFECT_DESC(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SPECIAL_EFFECT_DESC(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SPECIAL_EFFECT_LST_DESC(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SPECIAL_EFFECT_LST_DESC(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SPECIAL_EFFECT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SPECIAL_EFFECT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_ATTR_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_ATTR_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_POTENCY_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_POTENCY_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_POTENCY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_POTENCY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_SOUL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_SOUL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_SOUL_POTENCY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_SOUL_POTENCY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_LST_SOUL_POTENCY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_LST_SOUL_POTENCY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTR_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTR_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ATTR_BASE_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTR_BASE_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MEDAL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MEDAL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MEDAL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MEDAL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_VIEW_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VIEW_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ADD_FRIEND_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ADD_FRIEND_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ADD_FRIEND_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ADD_FRIEND_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SEND_FLOWER_PROP_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SEND_FLOWER_PROP_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SEND_FLOWER_PROP_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SEND_FLOWER_PROP_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEND_FLOWER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEND_FLOWER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEND_FLOWER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEND_FLOWER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECV_FLOWER_LOG(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECV_FLOWER_LOG(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECV_FLOWER_LOG_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECV_FLOWER_LOG_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FLOWER_BY_PLAYERID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FLOWER_BY_PLAYERID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FLOWER_BY_PLAYERID_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FLOWER_RANKING_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FLOWER_RANKING_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FLOWER_RANKING_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FLOWER_RANKING_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FLOWER_RANKING_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FLOWER_RANKING_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_FLOWER_RANKING_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_FLOWER_RANKING_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PRIVATE_MSG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PRIVATE_MSG_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PRIVATE_MSG_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PRIVATE_MSG_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_PRIVATE_MSG_LOG_LIST_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEND_PRIVATE_MSG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEND_PRIVATE_MSG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEND_PRIVATE_MSG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEND_PRIVATE_MSG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_PRIVATE_MSG_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEL_PRIVATE_MSG_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_PRIVATE_MSG_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ADD_TO_BLACKLIST_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ADD_TO_BLACKLIST_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ADD_TO_BLACKLIST_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ADD_TO_BLACKLIST_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MOVE_FROM_BLACKLIST_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MOVE_FROM_BLACKLIST_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MOVE_FROM_BLACKLIST_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MOVE_FROM_BLACKLIST_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ONLINE_PRIVATE_MSG_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONLINE_PRIVATE_MSG_NTF(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OFFLINE_MSG_SENDER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OFFLINE_MSG_SENDER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SEND_OFFLINE_MSG_PLAYER_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SEND_OFFLINE_MSG_PLAYER_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_SEND_OFFLINE_MSG_PLAYER_LIST_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_READ_OFFLINE_MSG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_READ_OFFLINE_MSG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_READ_OFFLINE_MSG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_READ_OFFLINE_MSG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RAREITEM_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RAREITEM_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RAREITEM_INFO_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RAREITEM_INFO_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RAREITEM_ROOM_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RAREITEM_ROOM_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RAREITEMROOM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RAREITEMROOM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RAREITEMROOM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RAREITEMROOM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_RAREITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_RAREITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_RAREITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_RAREITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_UPGRADERAREITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPGRADERAREITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADE_RAREITEM_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADE_RAREITEM_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADE_MSG_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADE_MSG_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UPGRADE_REQUIRE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPGRADE_REQUIRE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_UPGRADERAREITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPGRADERAREITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_RAREITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_RAREITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_AFTER_UPGRADE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_AFTER_UPGRADE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_RAREITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_RAREITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GYMNASIUM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GYMNASIUM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GYMNASIUM_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GYMNASIUM_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GYMNASIUM_CLI_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GYMNASIUM_CLI_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_CLI_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_CLI_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GYMNASIUM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GYMNASIUM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GYMNASIUM_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GYMNASIUM_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GYMNASIUM_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SHADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SHADE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SHADE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HUNTING_POT_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HUNTING_POT_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HUNTING_POT_INFO2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HUNTING_POT_INFO2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_REWARD_SINGLE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_REWARD_SINGLE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HUNTING_SCHEDULE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HUNTING_SCHEDULE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HUNTING_SCHEDULE_INFO2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HUNTING_SCHEDULE_INFO2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HUNTING_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HUNTING_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SHADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SHADE_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHADE_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SHADE_UPGRADE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SHADE_UPGRADE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SHADE_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHADE_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SHADE_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SHADE_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SHADE_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SHADE_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_REWARD_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_REWARD_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MONSTER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONSTER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GOLDMODE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GOLDMODE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GOLDMODE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GOLDMODE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HUNTING_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HUNTING_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_REWARD_INFO_ALL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_REWARD_INFO_ALL(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CHANGE_MONSTER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHANGE_MONSTER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FIND_MONSTER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FIND_MONSTER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HUNTING_OTHER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HUNTING_OTHER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HUNTING_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HUNTING_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FIND_MONSTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FIND_MONSTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FIND_MONSTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FIND_MONSTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HUNTING_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HUNTING_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HUNTING_BATTLE_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HUNTING_BATTLE_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_SCHEDEULE_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_SCHEDEULE_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HOT_SPRING_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOT_SPRING_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HOT_SPRING_PLAYER_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOT_SPRING_PLAYER_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HOT_SPRING_SELF_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOT_SPRING_SELF_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DIDNOT_GIVE_WINE_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DIDNOT_GIVE_WINE_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DIDNOT_DRINGK_WINE_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DIDNOT_DRINGK_WINE_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HOTSPRING_ACTIVITY_TIME_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOTSPRING_ACTIVITY_TIME_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HOT_SPRING_EXPERIENCE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HOT_SPRING_EXPERIENCE(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELECT_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELECT_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELECT_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELECT_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRINK_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRINK_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRINK_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRINK_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRINK_WINE_MOVE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRINK_WINE_MOVE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRINK_WINE_MOVE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRINK_WINE_MOVE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HOT_SPRING_POS_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HOT_SPRING_POS_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_OTHER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_OTHER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_OTHER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_OTHER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GIVE_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GIVE_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GIVE_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GIVE_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLICK_SELECT_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLICK_SELECT_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLICK_SELECT_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLICK_SELECT_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SURE_GIVE_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SURE_GIVE_WINE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SURE_GIVE_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SURE_GIVE_WINE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLOSED_MONSTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLOSED_MONSTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLOSED_MONSTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLOSED_MONSTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_PLAYER_CHANGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_PLAYER_CHANGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_PLAYER_CHANGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_PLAYER_CHANGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GIVE_WINE_ACTIVITY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GIVE_WINE_ACTIVITY_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_LEAVE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_LEAVE_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_LEAVE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_LEAVE_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_UPGRADE_ALL_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_UPGRADE_ALL_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_BUILD_LEVEL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BUILD_LEVEL(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_SYNC_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_SYNC_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_MASTER_DETAIL_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_MASTER_DETAIL_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MASTER_EXP_BOOK_DETAIL_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MASTER_EXP_BOOK_DETAIL_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MASTER_CHEAT_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MASTER_CHEAT_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MASTER_EXP_BOOK_PANEL_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MASTER_EXP_BOOK_PANEL_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MASTER_LEVEL_UP(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MASTER_LEVEL_UP(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MASTER_LEVEL_UP_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MASTER_LEVEL_UP_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EQUIP_MASTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EQUIP_MASTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EQUIP_MASTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EQUIP_MASTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACTIVATE_MASTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACTIVATE_MASTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACTIVATE_MASTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACTIVATE_MASTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BREAK_MASTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BREAK_MASTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BREAK_MASTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BREAK_MASTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_MASTER_BOOK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_MASTER_BOOK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_MASTER_BOOK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_MASTER_BOOK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_WIPE_MASTER_BOOK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_WIPE_MASTER_BOOK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_WIPE_MASTER_BOOK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_WIPE_MASTER_BOOK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MASTER_CHEAT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MASTER_CHEAT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MASTER_CHEAT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MASTER_CHEAT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CONFIRM_MASTER_BOOK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CONFIRM_MASTER_BOOK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CONFIRM_MASTER_BOOK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CONFIRM_MASTER_BOOK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BAD_REWARD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAD_REWARD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BAD_REWARD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAD_REWARD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_BAD_REWARD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_BAD_REWARD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_BAD_REWARD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_BAD_REWARD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_GROUP_LEADER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_GROUP_LEADER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_GROUP_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_GROUP_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_GROUP_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_GROUP_DATA_CLI_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_COURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_COURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeUnDT_COURAGE_GROUP_UNION_DATA_CLI(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_COURAGE_GROUP_UNION_DATA_CLI(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_COURAGE_ALL_GROUP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_ALL_GROUP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_COURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_COURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_CHALLENGE_NUM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_CHALLENGE_NUM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_CHALLENGE_NUM_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_CHALLENGE_NUM_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_CREATE_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_CREATE_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_CREATE_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_CREATE_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_GROUP_HERO_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_GROUP_HERO_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_OPERATE_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_OPERATE_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_OPERATE_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_OPERATE_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_CONFIRM_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_CONFIRM_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_CONFIRM_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_CONFIRM_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_JOIN_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_JOIN_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_JOIN_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_JOIN_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_FIRE_GROUP_MEMBER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_READY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_READY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_READY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_READY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_AWARD_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_AWARD_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_PLAYERID_HEROID_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYERID_HEROID_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_START_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_START_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_PLAYER_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYER_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_PLAYER_BATTLE_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYER_BATTLE_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_GROUP_PLAYER_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_GROUP_PLAYER_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_JEWELRY_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_JEWELRY_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_START_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_START_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_LEADER_START_NTF2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_LEADER_START_NTF2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_JOIN_GROUP_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_JOIN_GROUP_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_QUIT_GROUP_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_QUIT_GROUP_NTF(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_PLAYER_HERO_NTF_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYER_HERO_NTF_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_OPERATE_HERO_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_OPERATE_HERO_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_READY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_READY_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_FIRED_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_FIRED_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_LEAVE_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_LEAVE_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_LEAVE_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_LEAVE_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_COURAGE_SHOP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_COURAGE_SHOP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_COURAGE_SHOP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_COURAGE_SHOP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_FACTION_READ_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_FACTION_READ_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_FACTION_READ_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_FACTION_READ_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FIND_DRINK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FIND_DRINK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FIND_DRINK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FIND_DRINK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_BATTLE_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_BATTLE_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_BUILD_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GROUP_DISMISS_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GROUP_DISMISS_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HOT_SPRING_REWARD_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HOT_SPRING_REWARD_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GROUP_REFRESH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GROUP_REFRESH_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GROUP_REFRESH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GROUP_REFRESH_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_ENTER_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_ENTER_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_ENTER_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_ENTER_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_COURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_COURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_COURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_COURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HOT_SPRING_OVER_REWARD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HOT_SPRING_OVER_REWARD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HOT_SPRING_OVER_REWARD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GOLD_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GOLD_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HUNTING_ITEGRAL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HUNTING_ITEGRAL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HUNTING_ITEGRAL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HUNTING_ITEGRAL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACTIVITY_END_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACTIVITY_END_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PROMPT_IGNORE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PROMPT_IGNORE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PROMPT_IGNORE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PROMPT_IGNORE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GROUP_REFRESH_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GROUP_REFRESH_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HOME_BUILD_PROMPT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HOME_BUILD_PROMPT_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_LEAVE_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_LEAVE_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_LEAVE_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_OPERATE_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_OPERATE_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_PROMPT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_PROMPT_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GET_AWARD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GET_AWARD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GET_AWARD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GET_AWARD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GET_BATTLE_RESULT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DOORSTRIBUTE_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DOORSTRIBUTE_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AUDIT_FACTION_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AUDIT_FACTION_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AUDIT_FACTION_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AUDIT_FACTION_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_FIELD_INFO_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_FIELD_INFO_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CROP_MORE_INFO_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_MORE_INFO_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_FIELD_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_FIELD_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CROP_FIELD_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_FIELD_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GAIN_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GAIN_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GAIN_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GAIN_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CROP_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CROP_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_CROP_FIELD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_CROP_FIELD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_CROP_FIELD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_CROP_FIELD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AUDIT_ALL_REFUSE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AUDIT_ALL_REFUSE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_AUDIT_ALL_REFUSE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AUDIT_ALL_REFUSE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_AUDIT_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_AUDIT_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_PLAYER_AUDIT_CLI_LST2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_PLAYER_AUDIT_CLI_LST2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AUDIT_FACTION_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AUDIT_FACTION_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_AUDIT_FACTION_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AUDIT_FACTION_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VIP_GIFT_EACH_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_GIFT_EACH_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VIP_GIFT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VIP_GIFT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_VIP_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_VIP_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_VIP_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_VIP_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_VIP_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_VIP_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_VIP_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_VIP_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECV_FLOWER_LOG_CLI_EX(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECV_FLOWER_LOG_CLI_EX(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECV_FLOWER_LOG_LIST_CLI_EX(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECV_FLOWER_LOG_LIST_CLI_EX(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RECV_FLOWER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RECV_FLOWER_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RECV_FLOWER_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACCEPT_FLOWER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACCEPT_FLOWER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ACCEPT_FLOWER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACCEPT_FLOWER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FRIENDS_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FRIENDS_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_FLOWER(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_FLOWER(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_RAREITEM_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_RAREITEM_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_RAREITEM_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_RAREITEM_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEND_FLOWER_EX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEND_FLOWER_EX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEND_FLOWER_EX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEND_FLOWER_EX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_SELF_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_SELF_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_MANAGER_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_MANAGER_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_MANAGER_INFO_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_MANAGER_INFO_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_POWER_INFO_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_POWER_INFO_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_PLAYERS_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_PLAYERS_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FACTION_PLAYERS_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FACTION_PLAYERS_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_VIEW_FACTION_MEMBERS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VIEW_FACTION_MEMBERS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_VIEW_FACTION_MEMBERS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VIEW_FACTION_MEMBERS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CHANGE_FACTION_ICON_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_FACTION_ICON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_FACTION_ICON_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_FACTION_ICON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_FACTION_ICON_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FIND_BACK_DOORSTRIBUTE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_COURAGE_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_LEAVE_COURAGE_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEAVE_COURAGE_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EMBATTLE_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EMBATTLE_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FORMATION_DATA_LEVEL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FORMATION_DATA_LEVEL(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEW_FORMATION_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_FORMATION_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEW_FORMATION_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_FORMATION_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEW_HERO_EMBATLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_HERO_EMBATLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EMBATTLE_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EMBATTLE_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FORMATION_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FORMATION_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_FORMATION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_FORMATION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_FORMATION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_FORMATION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_REGISTER_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_REGISTER_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_FORMATION_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_FORMATION_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_BLUE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_BLUE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_BLUE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_BLUE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESOURCE_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESOURCE_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RESOLVE_TRIGRAM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESOLVE_TRIGRAM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESOLVE_TRIGRAM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESOLVE_TRIGRAM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESOLVE_TRIGRAM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JEWELRY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_JEWELRY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_JEWELRY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_DATA_CLI_v410(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_CLI_v410(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_DATA_CLI2_OLD(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_CLI2_OLD(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JEWELRY_SUIT_LVL_ATTR(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_SUIT_LVL_ATTR(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JEWELRY_SUIT_ATTR(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_SUIT_ATTR(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JEWELRY_SUIT_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_SUIT_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_JEWELRY_SUIT_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_JEWELRY_SUIT_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_INFO_PANEL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_INFO_PANEL(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_INFO_PANEL_OLD(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_INFO_PANEL_OLD(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLAYER_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLAYER_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLAYER_PANEL_ACK_OLD(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLAYER_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLAYER_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PLAYER_INFO_PANEL_v410(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_INFO_PANEL_v410(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLAYER_PANEL_ACK_v410(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_AWAKEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_AWAKEN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_AWAKEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_AWAKEN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_JEWELRY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_JEWELRY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_JEWELRY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_JEWELRY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_JEWELRY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_JEWELRY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_JEWELRY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_JEWELRY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HERO_SELECT_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HERO_SELECT_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JEWELRY_GENERATE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JEWELRY_GENERATE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JEWELRY_GENERATE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_GENERATE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_GENERATE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_GENERATE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_GENERATE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STRENGTH_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTH_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STRENGTH_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STRENGTH_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_STRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_STRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_STRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_STRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_GENERATE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_GENERATE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_GENERATE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_GENERATE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_RESOLVE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_RESOLVE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_RESOLVE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_RESOLVE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_RESOLVE_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_RESOLVE_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JEWELRY_STRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_JEWELRY_STRENGTH_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_JEWELRY_STRENGTH_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_JEWELRY_STRENGTH_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_JEWELRY_STRENGTH_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_COMPARE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_COMPARE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_COMPARE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_COMPARE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_EXT_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_JEWELRY_EXT_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JEWELRY_RESOLVE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JEWELRY_RESOLVE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_FORMATION_PLAYER_ID_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_PLAYER_ID_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_FORMATION_HERO_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_HERO_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_FORMATION_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_FORMATION_SIMPLE_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_SIMPLE_PLAYER_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_FORMATION_SIMPLE_PLAYER_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_FORMATION_SIMPLE_PLAYER_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_FORMATION_POS_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_FORMATION_POS_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_FORMATION_POS_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_FORMATION_POS_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_FORMATION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_FORMATION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_FORMATION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_FORMATION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_TIME_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_TIME_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_SCHEDULE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_SCHEDULE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_APPLAY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_APPLAY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_RANK_MONEY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_RANK_MONEY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_SIMPLE_RANK_MONEY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_SIMPLE_RANK_MONEY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_RANK_MONEY_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_RANK_MONEY_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_SIMPLE_RANK_MONEY_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_SIMPLE_RANK_MONEY_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_ALREAD_APPLAY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ALREAD_APPLAY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_ALREAD_APPLAY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ALREAD_APPLAY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_LAST_RESULT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_LAST_RESULT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_LAST_RESULT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_LAST_RESULT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_MONEY_CHAOS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_MONEY_CHAOS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_OPEN_MY_GAMBLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GAMBLE_GAME_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GAMBLE_GAME_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GAMBLE_GAME_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GAMBLE_GAME_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GAMBLE_GIFT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GAMBLE_GIFT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_OPEN_MY_GAMBLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_OPEN_GAMBLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_OPEN_GAMBLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_GAMBLE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_GAMBLE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_OPEN_GAMBLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_OPEN_GAMBLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_RULE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_RULE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_RULE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_RULE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_GAMBLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_GAMBLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_GAMBLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_GAMBLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_ACTIVITY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_ACTIVITY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_MONEY_RANK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_MONEY_RANK_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_MONEY_RANK_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_READ_FORMATION_PLAYER_TIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_READ_FORMATION_PLAYER_TIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_READ_FORMATION_PLAYER_TIP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_FACTION_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_FACTION_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_FACTION_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_FACTION_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GIFT_DETAILS_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GIFT_DETAILS_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GIFT_DETAILS_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GIFT_DETAILS_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_GIFT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_GIFT_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_GIFT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_GIFT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_GVG_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_GVG_GIFT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_GVG_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_GVG_GIFT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_APPLY_GVG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_APPLY_GVG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_APPLY_GVG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_APPLY_GVG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FINAL_WAR_FACTION_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FINAL_WAR_FACTION_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_LOG_RESET_UNREAD_CNT_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GVG_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GVG_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_16_TO_8_TO_4_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_16_TO_8_TO_4_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FINAL_WAR_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FINAL_WAR_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_CHAMPION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_CHAMPION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_ERR_MSG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_ERR_MSG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GVG_VIDEO_FACTION_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GVG_VIDEO_FACTION_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEE_GVG_VIDEO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEE_GVG_VIDEO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEE_GVG_VIDEO_ACK_v400(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEE_GVG_VIDEO_ACK_v400(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SEE_GVG_VIDEO_ACK_v410(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEE_GVG_VIDEO_ACK_v410(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_END_GVG_VIDEO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_END_GVG_VIDEO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_END_GVG_VIDEO_ACK_v400(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_END_GVG_VIDEO_ACK_v400(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_END_GVG_VIDEO_ACK_v410(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_END_GVG_VIDEO_ACK_v410(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SHOW_GVG_MVP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SHOW_GVG_MVP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SHOW_GVG_MVP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SHOW_GVG_MVP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELECT_GROUP_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELECT_GROUP_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SELECT_GROUP_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELECT_GROUP_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_FACTION_APPLAY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_FACTION_APPLAY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GVG_FACTION_APPLAY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GVG_FACTION_APPLAY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_READ_BATTLE_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_READ_BATTLE_LOG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_LOG_MSG_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_LOG_MSG_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_LOG_MSG_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_LOG_MSG_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_READ_BATTLE_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_READ_BATTLE_LOG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BATTLE_LOG_UNREAD_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_LOG_UNREAD_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UNREAD_BATTLE_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UNREAD_BATTLE_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BATTLE_ACK4(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BATTLE_ACK4(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHALLENGE_ACK4(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_ACK4(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLUNDER_ACK4(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_ACK4(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PKBATTLE_ACK4(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PKBATTLE_ACK4(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RDCHALLENGE_ACK4(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RDCHALLENGE_ACK4(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HUNTING_BATTLE_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HUNTING_BATTLE_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_PLAYER_BATTLE_DATA3(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYER_BATTLE_DATA3(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_START_ACK3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_START_ACK3(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_LEADER_START_NTF3(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_LEADER_START_NTF3(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_JIEBIAO_ACK4(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_JIEBIAO_ACK4(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK4(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK4(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLIGS_SOUL_MARK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIGS_SOUL_MARK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLIGS_SOUL_MARK_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIGS_SOUL_MARK_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DEVOUR_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DEVOUR_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DEVOUR_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DEVOUR_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEVOUR_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEVOUR_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DEVOUR_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEVOUR_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_REFLASH_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_REFLASH_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_REFLASH_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_REFLASH_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFLASH_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFLASH_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFLASH_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFLASH_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SAVE_REFLASH_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SAVE_REFLASH_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SAVE_REFLASH_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SAVE_REFLASH_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CLIGS_PRODUCTSOUL_COST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CLIGS_PRODUCTSOUL_COST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_REP_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_REP_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SOULREP_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SOULREP_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SOULREP_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SOULREP_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PRODUCT_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PRODUCT_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PRODUCT_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PRODUCT_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HERO_EQUIP_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HERO_EQUIP_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HERO_EQUIP_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HERO_EQUIP_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HERO_UNDRESS_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HERO_UNDRESS_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HERO_UNDRESS_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HERO_UNDRESS_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HERO_REPLACE_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HERO_REPLACE_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HERO_REPLACE_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HERO_REPLACE_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_EXCHANGE_METERIAL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_EXCHANGE_METERIAL(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SOUL_EXCHANGE_CARD(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SOUL_EXCHANGE_CARD(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SOULEXCHANGE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SOULEXCHANGE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SOULEXCHANGE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SOULEXCHANGE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXCHANGE_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_SOUL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXCHANGE_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_SOUL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MY_MEDAL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MY_MEDAL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_MY_MEDAL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MY_MEDAL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MEDAL_ICON_NAME(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MEDAL_ICON_NAME(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MEDAL_ICON_ID_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MEDAL_ICON_ID_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MEDAL_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MEDAL_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_OUT_BOUND_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_OUT_BOUND_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OUT_BOUND_DETAIL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OUT_BOUND_DETAIL_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OUT_BOUND_DETAIL_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OUT_BOUND_DETAIL_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OUT_BOUND_SIMPLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OUT_BOUND_SIMPLE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_OUT_BOUND_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_OUT_BOUND_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OUT_BOUND_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OUT_BOUND_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OUT_BOUND_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OUT_BOUND_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_STORE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STORE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOODS_EXCHANGE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_EXCHANGE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GOODS_EXCHANGE_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_EXCHANGE_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_STORE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STORE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXCHANGE_GOODS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_GOODS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EXCHANGE_GOODS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_GOODS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKILL_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_CLI_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_CLI_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_CLI_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_CLI_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PETREP_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PETREP_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PETREP_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PETREP_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PRODUCT_PET_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PRODUCT_PET_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PRODUCT_PET_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PRODUCT_PET_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_STAR_DRAGON_ITEM_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STAR_DRAGON_ITEM_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_UP_STAR_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UP_STAR_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_STAR_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STAR_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_STAR_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STAR_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UP_STAR_LV_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UP_STAR_LV_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UP_STAR_LV_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UP_STAR_LV_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PETFEED_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PETFEED_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PETFEED_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PETFEED_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FIRST_DRAW_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FIRST_DRAW_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FIRST_DRAW_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FIRST_DRAW_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESET_DRAW_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESET_DRAW_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RESET_DRAW_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RESET_DRAW_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CONFIRM_FEED_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CONFIRM_FEED_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CONFIRM_FEED_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CONFIRM_FEED_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MERGE_PET_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MERGE_PET_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MERGE_PET_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MERGE_PET_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_TEST_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_TEST_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_TEST_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_TEST_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_STORE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_STORE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_STORE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_STORE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_STORE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_STORE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_PET_STORE_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_PET_STORE_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BUY_PET_STORE_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_PET_STORE_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_REFINEPET_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_REFINEPET_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_REFINEPET_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_REFINEPET_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFINEPET_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFINEPET_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REFINEPET_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFINEPET_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_ITEM_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_ITEM_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_WASH_QUALITY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_WASH_QUALITY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_WASHQUALITY_COST_ITEM(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_WASHQUALITY_COST_ITEM(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_WASHQUALITY_COST_ITEM_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_WASHQUALITY_COST_ITEM_LIST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_WASH_QUALITY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_WASH_QUALITY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_WASH_QUALITY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_WASH_QUALITY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_WASH_QUALITY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_WASH_QUALITY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_PET_STATE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_PET_STATE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHANGE_PET_STATE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHANGE_PET_STATE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_AFTER_INHERIT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_AFTER_INHERIT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_INHERIT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_INHERIT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_INHERIT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_INHERIT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PET_INHERIT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PET_INHERIT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PET_INHERIT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PET_INHERIT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REQUEST_ALL_PET_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REQUEST_ALL_PET_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_REQUEST_ALL_PET_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REQUEST_ALL_PET_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HERO_ATTR_ADD_CLI_v410(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_ATTR_ADD_CLI_v410(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECRUIT_HERO_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUIT_HERO_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MATERIAL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MATERIAL(void *pHost, CNetData* poNetData);
INT32 EncodeDT_MATERIAL_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MATERIAL_LIST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RECRUIT_HERO_DATA_v410(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUIT_HERO_DATA_v410(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_MAKE_BOOK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_MAKE_BOOK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_MAKE_BOOK_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_MAKE_BOOK_INFO_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_SERIES(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_SERIES(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FLY_COST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FLY_COST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FLY_GOAL(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FLY_GOAL(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FLY_GOAL_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FLY_GOAL_LIST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_ENCHANT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_ENCHANT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_ENCHANT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_ENCHANT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SMITHY_ACK_v410(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SMITHY_ACK_v410(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_RECRUIT_ACK_v410(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RECRUIT_ACK_v410(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EQUIP_MAKE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EQUIP_MAKE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_EQUIP_MAKE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EQUIP_MAKE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_MAKE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_MAKE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_EQUIP_MAKE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_MAKE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FLY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FLY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_FLY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FLY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FLY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FLY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_FLY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FLY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHALLENGE_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLUNDER_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PKBATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PKBATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RDCHALLENGE_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RDCHALLENGE_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CHALLENGE_MYSELF_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_MYSELF_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_HUNTING_BATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_HUNTING_BATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodeDT_COURAGE_PLAYER_BATTLE_DATA5(void *pHost, CNetData* poNetData);
INT32 DecodeDT_COURAGE_PLAYER_BATTLE_DATA5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_PLAYER_START_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_PLAYER_START_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_COURAGE_LEADER_START_NTF5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COURAGE_LEADER_START_NTF5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_JIEBIAO_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_JIEBIAO_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_RULE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_RULE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_BOOK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_BOOK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_BOOK_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_BOOK_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_PET_RULE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PET_RULE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_PET_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_PET_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_PLAYER_INFO_PET_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLAYER_INFO_PET_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_EXCHANGE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_EXCHANGE_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_POTENCY_ATTR_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POTENCY_ATTR_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_POTENCY_ATTR_LST_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POTENCY_ATTR_LST_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_POTENCY_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POTENCY_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_POTENCY_ATTR_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POTENCY_ATTR_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_POTENCY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POTENCY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_POTENCY_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POTENCY_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SOUL_UPDATE_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SOUL_UPDATE_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SOUL_UPDATE_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SOUL_UPDATE_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_MARK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_MARK_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEIDAN_BAG_MARK_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEIDAN_BAG_MARK_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEIDAN_PET_MARK_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEIDAN_PET_MARK_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_NEIDAN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_NEIDAN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_DRESS_NEIDAN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_NEIDAN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEIDAN_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEIDAN_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PET_NEIDAN_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PET_NEIDAN_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_NEIDAN_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_NEIDAN_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_NEIDAN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_NEIDAN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_NEIDAN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_NEIDAN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPDATA_NEIDAN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATA_NEIDAN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPDATA_NEIDAN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATA_NEIDAN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NAIDAN_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NAIDAN_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NAIDAN_INSTANCE_LST_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NAIDAN_INSTANCE_LST_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_NEIDAN_ALL_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_NEIDAN_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_NEIDAN_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_NEIDAN_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_NEIDAN_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_NEIDAN_AWARD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_NEIDAN_AWARD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_NEIDAN_AWARD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_NEIDAN_AWARD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SKIP_NEIDAN_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_NUM_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_NUM_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ITEM_NUM_INFO_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_NUM_INFO_LST(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SKIP_NEIDAN_AWARD_ITEM_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKIP_NEIDAN_AWARD_ITEM_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SKIP_NEIDAN_BATTLE_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoCliGS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoCliGS)(void *pHost, CNetData* poNetData);

class CProtoCliGS : public CSDProtocol
{
public:
    static CProtoCliGS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoCliGS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoCliGS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoCliGS();
	virtual ~CProtoCliGS();
	EnFuncCProtoCliGS m_EncodeFuncArray[PROTOCLIGS_MSGID_RANGE];
	DeFuncCProtoCliGS m_DecodeFuncArray[PROTOCLIGS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
