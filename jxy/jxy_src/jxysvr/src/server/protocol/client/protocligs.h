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
#define NOTIN                    0
#define OWNER                    1
#define MAX_INCREASE             4
#define PROTOCLIGS_MSGID_RANGE 2494
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
	UINT8            byXiangqianNum; //镶嵌数量
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

typedef struct tagDT_PHYSTRENGTH_DATA_CLI{
	UINT16           wPhyStrength; //当前体力
	UINT16           wMaxPhyStrength; //本级体力
	UINT16           wPhyStrengthRefreshCD; //体力刷新间隔,单位s
	UINT16           wPhyStrengthCountDown; //体力刷新倒计时
}DT_PHYSTRENGTH_DATA_CLI;

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

typedef struct tagDT_NEW_EQUIP_PROMPT_DATA{
	UINT8            byEquipNum; //装备个数
	UINT16           awEquipIDInfo[EQUIP_POS_NUM]; //装备类型信息
	UINT16           wHeroID; //可提升的第一个伙伴(装备个数大于0有效)，0表示没有
}DT_NEW_EQUIP_PROMPT_DATA;

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
}PKT_CLIGS_GET_HERO_INFO_ACK;

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

typedef struct tagPKT_CLIGS_BATTLE_ACK{
	tagPKT_CLIGS_BATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BATTLE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_BATTLE_DATA   stBattleInfo; //战斗信息
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
}PKT_CLIGS_BATTLE_ACK;

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
	UINT16           wCurPhyStrength; //当前体力
}PKT_CLIGS_GET_SCENE_INSTANCE_ACK;

typedef struct tagPKT_CLIGS_EQUIP_STRENGTHEN_REQ{
	tagPKT_CLIGS_EQUIP_STRENGTHEN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_EQUIP_STRENGTHEN_REQ)); }
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备 
	UINT8            byStrengthenType; //强化方式，0金币，1元宝
	UINT16           wLevel; //要升到的等级(每次只能升1级)
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
}PKT_CLIGS_OPEN_BAG_ACK;

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

typedef struct tagPKT_CLIGS_CHALLENGE_ACK{
	tagPKT_CLIGS_CHALLENGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CHALLENGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_CHALLENGE_PLAYER_DATA stEnemyPlayerData; //对方玩家信息
	UINT8            byEnemyHeroNum; //对方信息
	DT_ENEMY_HERO_DATA astEnemyInfoList[MAX_FORMATION_IDX_NUM]; //对方信息
	DT_BATTLE_DATA   stBattleInfo; //战斗信息
	DT_RACE_CHALLENGE_ENCOURAGE_DATA stChallengeEncourage; //挑战奖励
	UINT64           qwCurCoin; //玩家当前的铜钱 
	UINT64           qwCurScience; //玩家当前的器魂点
	UINT64           qwCurGold; //玩家当前的元宝点
	UINT64           qwCurJingJie; //玩家当前的境界点
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表 
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息 
}PKT_CLIGS_CHALLENGE_ACK;

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

typedef struct tagPKT_CLIGS_OPEN_ITEM_MAKER_REQ{
	tagPKT_CLIGS_OPEN_ITEM_MAKER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ITEM_MAKER_REQ)); }
	UINT16           wItemKindID; //道具制作卷类型 
}PKT_CLIGS_OPEN_ITEM_MAKER_REQ;

typedef struct tagDT_EQUIP_DATA_MAKER_NEED{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备(0表示没有该装备)
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT16           wLevel; //装备等级
	DT_INSTANCE_ID   stBurstInstance; //可能掉落的副本 
}DT_EQUIP_DATA_MAKER_NEED;

typedef struct tagPKT_CLIGS_OPEN_ITEM_MAKER_ACK{
	tagPKT_CLIGS_OPEN_ITEM_MAKER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_ITEM_MAKER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT8            byEquipNum; //装备个数
	DT_EQUIP_DATA_MAKER_NEED astEquipList[MAX_BAG_OPEN_NUM]; //装备信息
	UINT8            byItemNum; //所需道具个数
	DT_NEED_ITEM_DATA astItemInfoList[MAX_ITEM_MAKER_NEED_NUM]; //所需道具信息
	UINT8            byEstimateItemNum; //道具个数
	DT_ESTIMATE_ITEM_DATA astEstimateItemList[MAX_BAG_OPEN_NUM]; //道具信息列表
}PKT_CLIGS_OPEN_ITEM_MAKER_ACK;

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
}PKT_CLIGS_MAKE_ITEM_ACK;

typedef struct tagDT_DRUG_DATA_CLI{
	UINT8            byLevel; //丹药等级(品)
	UINT8            byUsedNum; //已服用个数 
	UINT8            byOddNum; //剩余服用个数
	UINT16           wBaseTalent; //修为基础加成值 
	UINT16           wDecTalent; //每多使用1个修为加成递减值 
}DT_DRUG_DATA_CLI;

typedef struct tagDT_DRUG_DATA_LST_CLI{
	UINT8            byNum; //丹药个数
	DT_DRUG_DATA_CLI astDrugList[MAX_DRUG_LEVEL]; //丹药信息
}DT_DRUG_DATA_LST_CLI;

typedef struct tagPKT_CLIGS_GET_HERO_DRUG_REQ{
	tagPKT_CLIGS_GET_HERO_DRUG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_HERO_DRUG_REQ)); }
	UINT16           wHeroKindID; //武将类型ID
	UINT8            byNeedDrugItemFlag; //是否需要返回丹药道具信息,1为需要 
}PKT_CLIGS_GET_HERO_DRUG_REQ;

typedef struct tagDT_DRUG_ITEM_DATA{
	UINT16           wKindID; //物品类型
	UINT16           wUseLevel; //使用等级
	UINT16           wPileCount; //叠加数 
	UINT16           wDrugLevel; //丹药等级
	UINT16           wBaseTalent; //修为基础加成值 
	UINT16           wDecTalent; //每多使用1个修为加成递减值 
}DT_DRUG_ITEM_DATA;

typedef struct tagDT_DRUG_ITEM_DATA_LST{
	UINT16           wItemNum; //个数
	DT_DRUG_ITEM_DATA astDrugItemLst[MAX_BAG_OPEN_NUM]; //信息
}DT_DRUG_ITEM_DATA_LST;

typedef struct tagPKT_CLIGS_GET_HERO_DRUG_ACK{
	tagPKT_CLIGS_GET_HERO_DRUG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_HERO_DRUG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT16           wHeroKindID; //武将类型ID
	DT_DRUG_DATA_LST_CLI stDrugInfo; //武将丹药信息
	UINT8            byHaveDrugItemFlag; //是否有丹药道具信息，1为有
	DT_DRUG_ITEM_DATA_LST astDrugItemInfo[1]; //丹药道具信息
}PKT_CLIGS_GET_HERO_DRUG_ACK;

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

typedef struct tagPKT_CLIGS_PLUNDER_ACK{
	tagPKT_CLIGS_PLUNDER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PLUNDER_ACK)); }
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
	DT_BATTLE_DATA   stBattleInfo; //战斗信息
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
}PKT_CLIGS_PLUNDER_ACK;

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

typedef struct tagDT_ATTR_INC_DATA{
	UINT8            byAttrKind; //属性类型 
	UINT32           dwAttrValue; //属性值 
}DT_ATTR_INC_DATA;

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

typedef struct tagPKT_CLIGS_OPEN_DRUG_USE_TAB_REQ{
	tagPKT_CLIGS_OPEN_DRUG_USE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DRUG_USE_TAB_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_DRUG_USE_TAB_REQ;

typedef struct tagDT_TALENT_DATA{
	UINT16           wKindID; //伙伴类型ID
	UINT16           wLevel; //伙伴等级
	UINT16           wTalent; //修为
	UINT16           wUseCountDown; //服用丹药倒计时 
}DT_TALENT_DATA;

typedef struct tagPKT_CLIGS_OPEN_DRUG_USE_TAB_ACK{
	tagPKT_CLIGS_OPEN_DRUG_USE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_DRUG_USE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT8            byHeroNum; //伙伴个数信息
	DT_TALENT_DATA   astHeroInfo[MAX_HERO_NUM]; //伙伴信息
	UINT16           awDrugNum[MAX_DRUG_LEVEL]; //当前各品丹药个数信息
	UINT16           wNoCDGoldPerMin; //立即完成每分钟消耗元宝 
	UINT8            byBagFullFlag; //背包是否满，1为满
}PKT_CLIGS_OPEN_DRUG_USE_TAB_ACK;

typedef struct tagPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ{
	tagPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ)); }
	UINT16           wKindID; //伙伴类型ID
}PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK{
	tagPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT16           wKindID; //伙伴类型ID
	DT_DRUG_DATA_CLI astDrugInfo[MAX_DRUG_LEVEL]; //伙伴丹药信息
	UINT8            byBagFullFlag; //背包是否满，1为满
}PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK;

typedef struct tagPKT_CLIGS_CLR_USE_DRUG_CD_REQ{
	tagPKT_CLIGS_CLR_USE_DRUG_CD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_USE_DRUG_CD_REQ)); }
	UINT16           wKindID; //伙伴类型ID
}PKT_CLIGS_CLR_USE_DRUG_CD_REQ;

typedef struct tagPKT_CLIGS_CLR_USE_DRUG_CD_ACK{
	tagPKT_CLIGS_CLR_USE_DRUG_CD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CLR_USE_DRUG_CD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT64           qwCurGold; //当前玩家元宝
	DT_TALENT_DATA   stHeroTalentInfo; //伙伴修为信息
}PKT_CLIGS_CLR_USE_DRUG_CD_ACK;

typedef struct tagPKT_CLIGS_USE_DRUG_REQ{
	tagPKT_CLIGS_USE_DRUG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_USE_DRUG_REQ)); }
	UINT16           wHeroKindID; //武将类型ID
	UINT16           wUseLevel; //丹药等级(品)
}PKT_CLIGS_USE_DRUG_REQ;

typedef struct tagPKT_CLIGS_USE_DRUG_ACK{
	tagPKT_CLIGS_USE_DRUG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_USE_DRUG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT16           wUseLevel; //丹药等级(品)
	UINT16           wCurDrugNum; //当前该等级(品)丹药个数
	DT_TALENT_DATA   stHeroTalentInfo; //伙伴修为信息
	UINT8            byBagFullFlag; //背包是否满，1为满
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
}PKT_CLIGS_USE_DRUG_ACK;

typedef struct tagPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ{
	tagPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK{
	tagPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT8            abyDrugNum[MAX_DRUG_LEVEL]; //升级丹药所需各品个数信息
}PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK;

typedef struct tagPKT_CLIGS_UPGRADE_DRUG_REQ{
	tagPKT_CLIGS_UPGRADE_DRUG_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_DRUG_REQ)); }
	UINT8            byUseDrugLevel; //使用的丹药等级(品)
}PKT_CLIGS_UPGRADE_DRUG_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_DRUG_ACK{
	tagPKT_CLIGS_UPGRADE_DRUG_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_DRUG_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT8            byUseDrugLevel; //使用的丹药等级(品)
	UINT16           wUseDrugNum; //当前使用的丹药)个数
	UINT8            byProduceDrugLevel; //生成的丹药等级(品)
	UINT16           wProduceDrugNum; //当前生成的丹药个数 
	UINT8            byBagFullFlag; //背包是否满，1为满
}PKT_CLIGS_UPGRADE_DRUG_ACK;

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

typedef struct tagPKT_CLIGS_MELT_EQUIP_REQ{
	tagPKT_CLIGS_MELT_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MELT_EQUIP_REQ)); }
	UINT16           wEquipNum; //装备个数
	DT_MELT_EQUIP_DATA astMeltEquipDataList[MAX_BAG_OPEN_NUM]; //装备信息
	UINT8            byGoldFlag; //是否元宝熔炼,1元宝熔炼
}PKT_CLIGS_MELT_EQUIP_REQ;

typedef struct tagPKT_CLIGS_MELT_EQUIP_ACK{
	tagPKT_CLIGS_MELT_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MELT_EQUIP_ACK)); }
	UINT16           wErrorCode; //错误类型
	UINT32           dwAddScience; //增加器魂点 
	UINT64           qwScience; //当前器魂点 
	DT_EQUIP_MELT_DATA_LST_CLI stEquipInfo; //装备信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表 
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息 
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息 
	UINT64           qwCurGold; //玩家当前的元宝点
}PKT_CLIGS_MELT_EQUIP_ACK;

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
	UINT8            byComposeFlag; //合成类型标识，1为镶嵌面板合成,0为普通合成 
}PKT_CLIGS_COMPOSE_REQ;

typedef struct tagPKT_CLIGS_COMPOSE_ACK{
	tagPKT_CLIGS_COMPOSE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_COMPOSE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_ITEM_DATA_CLI2 stItemInfo; //生成的道具信息 
	DT_ITEM_DATA_LIST_CLI2 stGoodsBagItemInfo; //物品背包信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表 
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息 
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
}PKT_CLIGS_CHAT_ACK;

typedef struct tagPKT_CLIGS_CHAT_NTF{
	tagPKT_CLIGS_CHAT_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_CHAT_NTF)); }
	UINT8            byChatNum; //聊天记录个数
	CHAT_DATA        astChatData[MAX_CHAT_NUM]; //聊天内容
}PKT_CLIGS_CHAT_NTF;

typedef struct tagPKT_CLIGS_CROSS_DAY_NTF{
	tagPKT_CLIGS_CROSS_DAY_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_CROSS_DAY_NTF)); }
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	UINT8            byOnlineEncourageFlag; //是否存在在线奖励信息
	DT_ONLINE_ENCOURAGE_DATA astOnlineEncourage[1]; //当前在线奖励信息
	UINT16           awRecvCountDown[1]; //领取在线奖励倒计时，单位秒 
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
}PKT_CLIGS_OPEN_JIEBIAO_ACK;

typedef struct tagPKT_CLIGS_BEGIN_JIEBIAO_REQ{
	tagPKT_CLIGS_BEGIN_JIEBIAO_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_JIEBIAO_REQ)); }
	UINT32           dwPlayerID; //对象ID
	UINT8            byCurCount; //当前次数
}PKT_CLIGS_BEGIN_JIEBIAO_REQ;

typedef struct tagPKT_CLIGS_BEGIN_JIEBIAO_ACK{
	tagPKT_CLIGS_BEGIN_JIEBIAO_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_BEGIN_JIEBIAO_ACK)); }
	UINT16           wErrCode; //错误码 
	UINT8            byYabiaoFlag; //押镖标识(0为不在押镖，1为正在押镖中，2为押镖完毕，可以领取奖励)
	DT_BATTLE_DATA   stBattleData; //战斗信息
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
}PKT_CLIGS_BEGIN_JIEBIAO_ACK;

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

typedef struct tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK{
	tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_QUERY_BATTLE_RECORD_ACK)); }
	UINT16           wErrCode; //错误码 
	DT_BATTLE_DATA   stBattleData; //战斗信息
}PKT_CLIGS_QUERY_BATTLE_RECORD_ACK;

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

typedef struct tagDT_GEN_SCIENCE_DATA_CLI{
	UINT8            byCollectState; //收集状态,0生成，1收取
	DT_GAS_BALL_DATA astGasBollInfo[MAX_GAS_BALL_NUM]; //精气球信息 
	UINT32           dwGenGasCoin; //练气所需铜币
	UINT32           dwGenGasGold; //练气所需元宝
	UINT32           dwCommonUpGasCoin; //普通升级精气球所需铜币
	UINT32           dwFullLevelUpGasGold; //满级升级精气球所需元宝
	UINT16           wTotalGenNum; //可炼气总次数 
	UINT16           wCanGenNum; //可炼气次数 
}DT_GEN_SCIENCE_DATA_CLI;

typedef struct tagPKT_CLIGS_OPEN_GEN_GAS_TAB_REQ{
	tagPKT_CLIGS_OPEN_GEN_GAS_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEN_GAS_TAB_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_CLIGS_OPEN_GEN_GAS_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK{
	tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK)); }
	UINT16           wErrCode; //错误码 
	DT_GEN_SCIENCE_DATA_CLI stGenGasInfo; //练气面板信息
}PKT_CLIGS_OPEN_GEN_GAS_TAB_ACK;

typedef struct tagPKT_CLIGS_GEN_GAS_BALL_REQ{
	tagPKT_CLIGS_GEN_GAS_BALL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_GEN_GAS_BALL_REQ)); }
	UINT8            byFlag; //生产标志
}PKT_CLIGS_GEN_GAS_BALL_REQ;

typedef struct tagPKT_CLIGS_GEN_GAS_BALL_ACK{
	tagPKT_CLIGS_GEN_GAS_BALL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GEN_GAS_BALL_ACK)); }
	UINT16           wErrCode; //错误码 
	DT_GEN_SCIENCE_DATA_CLI stGenGasInfo; //练气面板信息
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
	DT_GEN_SCIENCE_DATA_CLI stGenGasInfo; //练气面板信息
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
	DT_GEN_SCIENCE_DATA_CLI stGenGasInfo; //练气面板信息
	UINT32           dwScience; //收集到的精气
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
}PKT_CLIGS_QUICK_EQUIP_ACK;

typedef struct tagPKT_CLIGS_PKBATTLE_REQ{
	tagPKT_CLIGS_PKBATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PKBATTLE_REQ)); }
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_PKBATTLE_REQ;

typedef struct tagPKT_CLIGS_PKBATTLE_ACK{
	tagPKT_CLIGS_PKBATTLE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PKBATTLE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_BATTLE_DATA   stBattleInfo; //战斗信息
}PKT_CLIGS_PKBATTLE_ACK;

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
}PKT_ENTER_BOSSB_ACK;

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

typedef struct tagPKT_BOSSB_POS_NTF{
	tagPKT_BOSSB_POS_NTF() { memset(this, 0, sizeof(tagPKT_BOSSB_POS_NTF)); }
	UINT8            bySyncType; //同步类型
	DT_BOSSB_PLAYER_DATA_CLI stPlayerInfo; //信息
	UINT32           dwPlayerNum; //当前人数
}PKT_BOSSB_POS_NTF;

typedef struct tagPKT_BOSSB_PLAYER_HRUT_NTF{
	tagPKT_BOSSB_PLAYER_HRUT_NTF() { memset(this, 0, sizeof(tagPKT_BOSSB_PLAYER_HRUT_NTF)); }
	DT_BOSSB_HURT_RATE stCurHurtInfo; //当前玩家攻击伤害
	DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]; //玩家伤害统计排名
	UINT32           dwTotalBossHP; //当前Boss总血量 
	UINT32           dwCurBossHP; //当前Boss血量 
}PKT_BOSSB_PLAYER_HRUT_NTF;

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

typedef struct tagPKT_BOSSB_OVER_NTF{
	tagPKT_BOSSB_OVER_NTF() { memset(this, 0, sizeof(tagPKT_BOSSB_OVER_NTF)); }
	DT_BOSSB_OVER_INFO stBossOverInfo; //boss战活动结束信息 
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //当前阅历
	UINT64           qwCurJingJie; //当前境界
	UINT8            byUnreadBattleLogNum; //未读战报个数
}PKT_BOSSB_OVER_NTF;

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

typedef struct tagPKT_CLIGS_RDCHALLENGE_ACK{
	tagPKT_CLIGS_RDCHALLENGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RDCHALLENGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_BATTLE_DATA   stBattleInfo; //战斗信息
	DT_RDC_TAB_DATA  stTabInfo; //随机挑战面板信息
	DT_RDC_ENC_DATA  stEncInfo; //随机挑战奖励信息
	UINT64           qwCurJingJie; //当前境界点 
	UINT64           qwCurPurpleGas; //当前紫气
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息 
}PKT_CLIGS_RDCHALLENGE_ACK;

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
	UINT16           wActivityID; //活动类型
	DT_RSYNC_RECV_ENCOURAGE_DATA stRsyncRecvEncourageData; //同步领取奖励后玩家数据变化 
	UINT8            byHide; //隐藏
}PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK;

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
}PKT_CLIGS_RECV_COMPOSE_EQUIP_ACK;

typedef struct tagPKT_CLIGS_OPEN_FACTION_REQ{
	tagPKT_CLIGS_OPEN_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_FACTION_REQ;

typedef struct tagDT_FACTION_BASE_DATA_CLI{
	UINT8            byJoinFlag; //加入标志0:未加入 1：申请中 2：成功加入 
	TCHAR            aszDispName[USERNAME_LEN]; //帮主名称
	UINT16           wFactionNumberPeople; //帮派人数
	DT_FACTION_BASE_DATA stFactionBaseData; //帮派信息
}DT_FACTION_BASE_DATA_CLI;

typedef struct tagDT_FACTION_BASE_DATA_LST{
	UINT16           wFactionNum; //帮派个数
	DT_FACTION_BASE_DATA astFactionBaseData[MAX_FACTION_CLI_NUM]; //帮派信息
}DT_FACTION_BASE_DATA_LST;

typedef struct tagDT_FACTION_BASE_DATA_CLI_LST{
	UINT16           wFactionNum; //帮派个数
	DT_FACTION_BASE_DATA_CLI astFactionBaseData[MAX_FACTION_CLI_NUM]; //帮派信息
}DT_FACTION_BASE_DATA_CLI_LST;

typedef struct tagDT_OPEN_FACTION_BASE_DATA{
	UINT32           dwCreateFactionCoin; //帮派花费的铜钱数
	UINT32           dwCreateFactionGold; //帮派花费的玩宝数
	DT_FACTION_BASE_DATA_CLI_LST stRankFactionBaseDataCliLst; //帮派排名
}DT_OPEN_FACTION_BASE_DATA;

typedef struct tagDT_FACTION_PLAYER_CLI{
	UINT32           dwPlayerID; //玩家ID
	UINT16           wHeroKindID; //主武将ID
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT16           wLevel; //玩家等级
	UINT16           wRank; //玩家竞技场排名 
	UINT8            byJobType; //职业类型
	UINT32           dwContribution; //贡献
	UINT8            byOnLineState; //在线状态 0：不在线， 1：在线 
}DT_FACTION_PLAYER_CLI;

typedef struct tagDT_FACTION_PLAYER_CLI_LST{
	UINT16           wFactionNum; //帮派个数
	DT_FACTION_PLAYER_CLI astFactionPlayerCli[DT_FACTION_PLAYER_NUM]; //帮派信息
}DT_FACTION_PLAYER_CLI_LST;

typedef struct tagDT_FACTION_BASE_OWNER_DATA_CLI{
	UINT8            byJobtype; //职业类型
	TCHAR            aszDispName[USERNAME_LEN]; //帮主名称
	UINT16           wFactionNumberPeople; //帮派人数
	UINT32           dwContributionValues; //门贡值 
	DT_FACTION_BASE_DATA stFactionBaseData; //帮派信息
}DT_FACTION_BASE_OWNER_DATA_CLI;

typedef struct tagDT_FACTION_OWNER_DATA{
	DT_FACTION_BASE_OWNER_DATA_CLI stFactionBaseData; //加入帮派基本信息
	DT_FACTION_PLAYER_CLI_LST stFactionPlayerCliLst; //帮派玩家信息
}DT_FACTION_OWNER_DATA;

typedef union tagDT_OPEN_FACTION_CONDITION{
	DT_OPEN_FACTION_BASE_DATA stOpenFactionBaseData; //打开帮派基本信息
	DT_FACTION_OWNER_DATA stFactionOwnerData; //打开已加入的帮派信息
}DT_OPEN_FACTION_CONDITION;

typedef struct tagPKT_CLIGS_OPEN_FACTION_ACK{
	tagPKT_CLIGS_OPEN_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT8            byOwnerFlag; //是否创建过或者加入帮派, 0：没 1：有
	DT_OPEN_FACTION_CONDITION stOpenFactionCondition; //帮派面板基本信息条件记录
}PKT_CLIGS_OPEN_FACTION_ACK;

typedef struct tagPKT_CLIGS_APPLY_FACTION_REQ{
	tagPKT_CLIGS_APPLY_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_APPLY_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
}PKT_CLIGS_APPLY_FACTION_REQ;

typedef struct tagPKT_CLIGS_APPLY_FACTION_ACK{
	tagPKT_CLIGS_APPLY_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_APPLY_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT32           dwFactionID; //帮派ID
}PKT_CLIGS_APPLY_FACTION_ACK;

typedef struct tagPKT_CLIGS_CANCEL_APPLY_FACTION_REQ{
	tagPKT_CLIGS_CANCEL_APPLY_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CANCEL_APPLY_FACTION_REQ)); }
	UINT32           dwFactionID; //装备等级
}PKT_CLIGS_CANCEL_APPLY_FACTION_REQ;

typedef struct tagPKT_CLIGS_CANCEL_APPLY_FACTION_ACK{
	tagPKT_CLIGS_CANCEL_APPLY_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CANCEL_APPLY_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	UINT32           dwFactionID; //帮派ID
}PKT_CLIGS_CANCEL_APPLY_FACTION_ACK;

typedef struct tagPKT_CLIGS_CREATE_FACTION_REQ{
	tagPKT_CLIGS_CREATE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CREATE_FACTION_REQ)); }
	UINT16           wCreateFactionType; //是否创建过帮派, 0：铜钱 1：元宝 
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //帮派名称
	TCHAR            aszFactionDescName[MAX_FACTION_DESC_NUM]; //帮派描述
}PKT_CLIGS_CREATE_FACTION_REQ;

typedef struct tagPKT_CLIGS_CREATE_FACTION_ACK{
	tagPKT_CLIGS_CREATE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CREATE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
}PKT_CLIGS_CREATE_FACTION_ACK;

typedef struct tagPKT_CLIGS_SEARCH_FACTION_REQ{
	tagPKT_CLIGS_SEARCH_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SEARCH_FACTION_REQ)); }
	TCHAR            aszFactionName[MAX_FACTION_NUM]; //帮派名称
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
	UINT16           wFactionNum; //帮派个数
	DT_FACTION_PLAYER_AUDIT_CLI astFactionPlayerAuditCli[DT_FACTION_PLAYER_NUM]; //帮派信息
}DT_FACTION_PLAYER_AUDIT_CLI_LST;

typedef struct tagPKT_CLIGS_OPEN_OWNER_FACTION_ACK{
	tagPKT_CLIGS_OPEN_OWNER_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_OWNER_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_FACTION_OWNER_DATA stFactionOwnerData; //打开已加入的帮派信息
}PKT_CLIGS_OPEN_OWNER_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_CONTRIBUTION_FACTION_REQ{
	tagPKT_CLIGS_OPEN_CONTRIBUTION_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CONTRIBUTION_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
}PKT_CLIGS_OPEN_CONTRIBUTION_FACTION_REQ;

typedef struct tagDT_CONTRIBUTION_BASE_DATA{
	UINT16           wCoinMaxTimeEveryDay; //铜钱每天最多可领取的次数 
	UINT32           dwCoinEveryTimeValues; //铜钱每次贡献值 
	UINT32           dwCoinEveryTimeContributionValues; //铜钱每次能获得的门贡
	UINT16           wGoldMaxTimeEveryDay; //元宝每天最多可领取的次数 
	UINT32           dwGoldEveryTimeValues; //元宝每次贡献值 
	UINT32           dwGoldEveryTimeContributionValues; //元宝每次能获得的门贡
}DT_CONTRIBUTION_BASE_DATA;

typedef struct tagDT_CONTRIBUTION_TODAY_DATA{
	UINT8            byCoinRecvFlag; //领取奖励标志 1:可领取
	UINT16           wCoinTodaysContributionTimes; //今天铜钱贡献次数
	UINT8            byGoldRecvFlag; //领取奖励标志 1:可领取
	UINT16           wGoldTodaysContributionTimes; //今天铜钱贡献次数
}DT_CONTRIBUTION_TODAY_DATA;

typedef struct tagPKT_CLIGS_OPEN_CONTRIBUTION_FACTION_ACK{
	tagPKT_CLIGS_OPEN_CONTRIBUTION_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_CONTRIBUTION_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_CONTRIBUTION_BASE_DATA stContributionBaseData; //门贡基本信息
	DT_ENCOURAGE_DATA stEncourageData; //奖励信息
	DT_CONTRIBUTION_TODAY_DATA stContributionTodayData; //给客户端帮派贡献当天信息
}PKT_CLIGS_OPEN_CONTRIBUTION_FACTION_ACK;

typedef struct tagPKT_CLIGS_CONTRIBUTION_FACTION_REQ{
	tagPKT_CLIGS_CONTRIBUTION_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CONTRIBUTION_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
	UINT32           dwContributionFlag; //标志0：铜钱，1：元宝 
}PKT_CLIGS_CONTRIBUTION_FACTION_REQ;

typedef struct tagPKT_CLIGS_CONTRIBUTION_FACTION_ACK{
	tagPKT_CLIGS_CONTRIBUTION_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_CONTRIBUTION_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_CONTRIBUTION_BASE_DATA stContributionBaseData; //门贡基本信息
	DT_ENCOURAGE_DATA stEncourageData; //奖励信息
	DT_CONTRIBUTION_TODAY_DATA stContributionTodayData; //给客户端帮派贡献当天信息
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
}PKT_CLIGS_CONTRIBUTION_FACTION_ACK;

typedef struct tagPKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_REQ{
	tagPKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_REQ)); }
	UINT32           dwFactionID; //帮派ID
}PKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_REQ;

typedef struct tagPKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_ACK{
	tagPKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_RSYNC_RECV_ENCOURAGE_DATA stRsyncRecvEncourageData; //同步领取奖励后玩家数据变化 
}PKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_ACK;

typedef struct tagPKT_CLIGS_KICKED_FACTION_REQ{
	tagPKT_CLIGS_KICKED_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_KICKED_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
	UINT32           dwPlayerID; //玩家ID
}PKT_CLIGS_KICKED_FACTION_REQ;

typedef struct tagPKT_CLIGS_KICKED_FACTION_ACK{
	tagPKT_CLIGS_KICKED_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_KICKED_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
}PKT_CLIGS_KICKED_FACTION_ACK;

typedef struct tagPKT_CLIGS_JOB_FACTION_REQ{
	tagPKT_CLIGS_JOB_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_JOB_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
	UINT32           dwPlayerID; //玩家ID
	UINT8            byFlag; //标志0：降职1：升职 
}PKT_CLIGS_JOB_FACTION_REQ;

typedef struct tagPKT_CLIGS_JOB_FACTION_ACK{
	tagPKT_CLIGS_JOB_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_JOB_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_FACTION_PLAYER_CLI_LST stFactionPlayerDataLst; //帮派玩家信息
}PKT_CLIGS_JOB_FACTION_ACK;

typedef struct tagPKT_CLIGS_SET_NOTICE_FACTION_REQ{
	tagPKT_CLIGS_SET_NOTICE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_SET_NOTICE_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
	UINT8            byFlag; //标志0：公告1：通知
	TCHAR            aszFactionNotice[MAX_FACTION_DESC_NUM]; //帮派通知
}PKT_CLIGS_SET_NOTICE_FACTION_REQ;

typedef struct tagPKT_CLIGS_SET_NOTICE_FACTION_ACK{
	tagPKT_CLIGS_SET_NOTICE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_SET_NOTICE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
}PKT_CLIGS_SET_NOTICE_FACTION_ACK;

typedef struct tagPKT_CLIGS_DISSOLVE_FACTION_REQ{
	tagPKT_CLIGS_DISSOLVE_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DISSOLVE_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
}PKT_CLIGS_DISSOLVE_FACTION_REQ;

typedef struct tagPKT_CLIGS_DISSOLVE_FACTION_ACK{
	tagPKT_CLIGS_DISSOLVE_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DISSOLVE_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
}PKT_CLIGS_DISSOLVE_FACTION_ACK;

typedef struct tagDT_FACTION_AUDIT_PLAYER_DATA{
	UINT32           dwPlayerID; //帮派用户信息
	UINT8            byAuditFlag; //标志0：审核没通过 1：审核没通过
}DT_FACTION_AUDIT_PLAYER_DATA;

typedef struct tagPKT_CLIGS_AUDIT_FACTION_REQ{
	tagPKT_CLIGS_AUDIT_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_FACTION_REQ)); }
	UINT32           dwFactionID; //帮派ID
	UINT32           dwFactionNum; //用户数 
	DT_FACTION_AUDIT_PLAYER_DATA astAuditPlayerData[DT_FACTION_PLAYER_NUM]; //帮派用户信息
}PKT_CLIGS_AUDIT_FACTION_REQ;

typedef struct tagPKT_CLIGS_AUDIT_FACTION_ACK{
	tagPKT_CLIGS_AUDIT_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_AUDIT_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
}PKT_CLIGS_AUDIT_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_APPLY_FACTION_REQ{
	tagPKT_CLIGS_OPEN_APPLY_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_APPLY_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_APPLY_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_APPLY_FACTION_ACK{
	tagPKT_CLIGS_OPEN_APPLY_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_APPLY_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_FACTION_BASE_DATA_CLI_LST stApplyFactionBaseDataCliLst; //帮派申请加入
}PKT_CLIGS_OPEN_APPLY_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_RANK_FACTION_REQ{
	tagPKT_CLIGS_OPEN_RANK_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RANK_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_RANK_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_RANK_FACTION_ACK{
	tagPKT_CLIGS_OPEN_RANK_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RANK_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_FACTION_BASE_DATA_CLI_LST stRankFactionBaseDataCliLst; //帮派申请加入
}PKT_CLIGS_OPEN_RANK_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ{
	tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AUDIT_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_AUDIT_FACTION_REQ;

typedef struct tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK{
	tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AUDIT_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_FACTION_PLAYER_AUDIT_CLI_LST stFactionPlayerAuditCliLst; //帮派申请加入用户信息
}PKT_CLIGS_OPEN_AUDIT_FACTION_ACK;

typedef struct tagPKT_CLIGS_OPEN_LOG_FACTION_REQ{
	tagPKT_CLIGS_OPEN_LOG_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LOG_FACTION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_LOG_FACTION_REQ;

typedef struct tagDT_FACTION_LOG_DATA_CLI{
	TCHAR            aszDispName1[USERNAME_LEN]; //玩家名称
	TCHAR            aszDispName2[USERNAME_LEN]; //玩家名称
	DT_FACTION_LOG_DATA stFactionLogData; //日志基本信息
}DT_FACTION_LOG_DATA_CLI;

typedef struct tagDT_FACTION_LOG_DATA_CLI_LST{
	UINT16           wFactionLogNum; //错误码，非0为错误 
	DT_FACTION_LOG_DATA_CLI astFactionLogDataCli[MAX_FACTION_LOG_NUM]; //帮派日志信息
}DT_FACTION_LOG_DATA_CLI_LST;

typedef struct tagPKT_CLIGS_OPEN_LOG_FACTION_ACK{
	tagPKT_CLIGS_OPEN_LOG_FACTION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_LOG_FACTION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_FACTION_LOG_DATA_CLI_LST stFactionLogDataCliLst; //帮派日志信息
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

typedef struct tagPKT_CLIGS_OPEN_BLUE_PAVILION_REQ{
	tagPKT_CLIGS_OPEN_BLUE_PAVILION_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BLUE_PAVILION_REQ)); }
	UINT8            byExt; //扩展信息
}PKT_CLIGS_OPEN_BLUE_PAVILION_REQ;

typedef struct tagPKT_CLIGS_OPEN_BLUE_PAVILION_ACK{
	tagPKT_CLIGS_OPEN_BLUE_PAVILION_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_BLUE_PAVILION_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误 
	DT_BEGBLUE_PLAYER_RES_CLI stPlayerResInfo; //玩家当前资源信息
	DT_BLUE_PAVILION_DATA_CLI stBluePavilion; //仙露阁数据 
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
}PKT_CLIGS_COMFIRM_BEGBLUE_ACK;

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
	CLIGS_BATTLE_ACK                = 2011        ,
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
	CLIGS_CHALLENGE_ACK             = 2045        ,
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
	CLIGS_OPEN_ITEM_MAKER_REQ       = 2066        ,
	CLIGS_OPEN_ITEM_MAKER_ACK       = 2067        ,
	CLIGS_MAKE_ITEM_REQ             = 2068        ,
	CLIGS_MAKE_ITEM_ACK             = 2069        ,
	CLIGS_GET_HERO_DRUG_REQ         = 2070        ,
	CLIGS_GET_HERO_DRUG_ACK         = 2071        ,
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
	CLIGS_PLUNDER_ACK               = 2109        ,
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
	CLIGS_OPEN_DRUG_USE_TAB_REQ     = 2200        ,
	CLIGS_OPEN_DRUG_USE_TAB_ACK     = 2201        ,
	CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ  = 2202        ,
	CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK  = 2203        ,
	CLIGS_CLR_USE_DRUG_CD_REQ       = 2204        ,
	CLIGS_CLR_USE_DRUG_CD_ACK       = 2205        ,
	CLIGS_USE_DRUG_REQ              = 2206        ,
	CLIGS_USE_DRUG_ACK              = 2207        ,
	CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ  = 2208        ,
	CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK  = 2209        ,
	CLIGS_UPGRADE_DRUG_REQ          = 2210        ,
	CLIGS_UPGRADE_DRUG_ACK          = 2211        ,
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
	CLIGS_MELT_EQUIP_REQ            = 2242        ,
	CLIGS_MELT_EQUIP_ACK            = 2243        ,
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
	CLIGS_BEGIN_JIEBIAO_ACK         = 2291        ,
	CLIGS_BE_JIEBIAO_NTF            = 2292        ,
	CLIGS_QUERY_BATTLE_RECORD_REQ   = 2294        ,
	CLIGS_QUERY_BATTLE_RECORD_ACK   = 2295        ,
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
	CLIGS_PKBATTLE_ACK              = 2351        ,
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
	BOSSB_PLAYER_HRUT_NTF           = 2364        ,
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
	CLIGS_RDCHALLENGE_ACK           = 2395        ,
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
	CLIGS_OPEN_CONTRIBUTION_FACTION_REQ  = 2442        ,
	CLIGS_OPEN_CONTRIBUTION_FACTION_ACK  = 2443        ,
	CLIGS_CONTRIBUTION_FACTION_REQ  = 2444        ,
	CLIGS_CONTRIBUTION_FACTION_ACK  = 2445        ,
	CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_REQ  = 2446        ,
	CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_ACK  = 2447        ,
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
	CLIGS_OPEN_LOG_FACTION_REQ      = 2464        ,
	CLIGS_OPEN_LOG_FACTION_ACK      = 2465        ,
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
INT32 EncodeDT_PHYSTRENGTH_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PHYSTRENGTH_DATA_CLI(void *pHost, CNetData* poNetData);
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
INT32 EncodeDT_FUNC_COMMON_PROMPT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FUNC_COMMON_PROMPT(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FUNC_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FUNC_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_OTHER_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OTHER_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_NEW_EQUIP_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_EQUIP_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_RSYNC_RECV_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RSYNC_RECV_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_ENTERGS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTERGS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BATTLE_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_OPEN_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_BAG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAG_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_CHALLENGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_OPEN_ITEM_MAKER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ITEM_MAKER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_EQUIP_DATA_MAKER_NEED(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_MAKER_NEED(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_ITEM_MAKER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ITEM_MAKER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MAKE_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKE_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MAKE_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKE_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRUG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRUG_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_HERO_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRUG_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_DRUG_ITEM_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_ITEM_DATA_LST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_GET_HERO_DRUG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_DRUG_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_PLUNDER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodeDT_ATTR_INC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTR_INC_DATA(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_OPEN_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_TALENT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TALENT_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_USE_DRUG_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_USE_DRUG_CD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CLR_USE_DRUG_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_USE_DRUG_CD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_USE_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USE_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_USE_DRUG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USE_DRUG_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_UPGRADE_DRUG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_DRUG_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_OPEN_JIEBIAO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_JIEBIAO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_JIEBIAO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_JIEBIAO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_BEGIN_JIEBIAO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BEGIN_JIEBIAO_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_QUERY_BATTLE_RECORD_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodeDT_GEN_SCIENCE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GEN_SCIENCE_DATA_CLI(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_PKBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PKBATTLE_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodeDT_BOSSB_MY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_MY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_HURT_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HURT_ENC_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_ENTER_BOSSB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_ENTER_BOSSB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_ENTER_BOSSB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_ENTER_BOSSB_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_BOSSB_POS_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_POS_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_PLAYER_HRUT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_PLAYER_HRUT_NTF(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_POS(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_POS(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_HURT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_HURT(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_INFO_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_INFO_SYNC_NTF(void *pHost, CNetData* poNetData);
INT32 EncodeDT_BOSSB_OVER_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BOSSB_OVER_INFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_BOSSB_OVER_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_BOSSB_OVER_NTF(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_RDCHALLENGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RDCHALLENGE_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodeDT_GEM_MAKER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GEM_MAKER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEM_MAKER_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GEM_MAKER_TAB_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_OPEN_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_BASE_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_BASE_DATA_LST(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_OPEN_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_FACTION_ACK(void *pHost, CNetData* poNetData);
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
INT32 EncodePKT_CLIGS_OPEN_CONTRIBUTION_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CONTRIBUTION_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CONTRIBUTION_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CONTRIBUTION_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_CONTRIBUTION_TODAY_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CONTRIBUTION_TODAY_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_OPEN_CONTRIBUTION_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CONTRIBUTION_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CONTRIBUTION_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CONTRIBUTION_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_CONTRIBUTION_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CONTRIBUTION_FACTION_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_CONTRIBUTION_FACTION_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
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
