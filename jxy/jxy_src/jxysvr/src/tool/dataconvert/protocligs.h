/*       Version Number: 3          */

#ifndef PROTOCLIGS_H_INCLUDE_VERSION_3
#define PROTOCLIGS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#include <json/json.h>
#pragma pack(push, 1)

/*Defines Begin*/
#define MAX_EXT_FUN_NUM          10
#define MAX_CHAT_CONTENT_NUM     512
#define MAX_CHAT_NUM             10
#define PROTOCLIGS_MSGID_RANGE 2265
#define PROTOCLIGS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_CLIGS_ENTERGS_REQ{
	tagPKT_CLIGS_ENTERGS_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTERGS_REQ)); }
	UINT16           wZoneID; //区ID
	TCHAR            aszDeviceID[DEVICEID_LEN]; //区ID
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //区ID
	TCHAR            aszUserName[USERNAME_LEN]; //区ID
	UINT32           dwPlayerID; //玩家ID，老用户登录有效
	UINT16           wCareerID; //职业，新游戏有效
}PKT_CLIGS_ENTERGS_REQ;

typedef struct tagDT_EXT_FUNC_PARAM{
	UINT8            byParam1; //参数1
	UINT16           wParam2; //参数2
	UINT32           dwParam3; //参数3
}DT_EXT_FUNC_PARAM;

typedef struct tagDT_EXT_FUNC_PARAM_LST{
	UINT8            byFuncNum; //扩展功能数
	DT_EXT_FUNC_PARAM astExtFuncParaInfo[MAX_EXT_FUN_NUM]; //扩展功能信息
}DT_EXT_FUNC_PARAM_LST;

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

typedef struct tagDT_SKILL_EFFECT_DATA{
	UINT8            byEffectKindID; //技能效果ID
	UINT8            byEffectHurtRate; //效果伤害倍率(百分比)
	UINT32           dwExtHurtValue; //附加伤害值
	UINT16           wEffectParam; //效果参数
}DT_SKILL_EFFECT_DATA;

typedef struct tagDT_ATTACK_RANG_DATA{
	UINT8            byAttackRangKind; //攻击范围类型
	UINT8            byTraceBuffKind; //追踪的buff类型(只对AttackRangKind为EARK_BUFF有效)
}DT_ATTACK_RANG_DATA;

typedef struct tagDT_SKILL_DATA_CLI{
	DT_SKILL_DATA    stSkillBaseInfo; //技能基本信息，等级为0表示该技能已开放但未学习
	DT_SKILL_EFFECT_DATA stEffectInfo; //技能效果
	DT_ATTACK_RANG_DATA stAttackRang; //技能目标范围
	UINT8            byBuffNum; //技能BUFF
	DT_SKILL_BUFF_DATA astBuffInfo[MAX_SKILL_BUFF_NUM]; //技能BUFF
}DT_SKILL_DATA_CLI;

typedef struct tagDT_SKILL_DATA_LIST_CLI{
	UINT8            bySkillNum; //技能个数
	DT_SKILL_DATA_CLI astSkillInfo[MAX_SKILL_NUM]; //技能列表
}DT_SKILL_DATA_LIST_CLI;

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
	UINT16           wSelectSkillID; //所选技能
	UINT16           wSelectSkillLevel; //所选技能等级
}DT_HERO_BASE_DATA_CLI;

typedef struct tagDT_HERO_UPGRADE_DATA{
	UINT16           wHeroKindID; //武将类型ID
	UINT8            byUpgradeFlag; //是否升级
	UINT16           wLevel; //当前等级
	UINT32           dwExperience; //当前经验
	UINT32           dwUpgradeExperience; //下一级升级经验
}DT_HERO_UPGRADE_DATA;

typedef struct tagDT_EQUIP_DATA_STRENGTHEN_CLI{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT16           wHeroKindID; //装备的武将ID，0为未装备
	UINT16           wLevel; //装备等级
	UINT16           wDressLevel; //穿戴等级
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //当前战斗属性
	UINT8            byCanStrengthenFlag; //是否可以强化，1可以强化，0不可强化
	UINT32           dwStrengthenCoin; //强化金币
	UINT32           dwStrengthenGold; //强化元宝
	UINT16           wStrengthenRate; //强化成功率
	DT_BATTLE_ATTRIBUTE stNextBattleAttribute; //下一级战斗属性
	UINT32           dwRecoverPrice; //回收价格
}DT_EQUIP_DATA_STRENGTHEN_CLI;

typedef struct tagDT_EQUIP_DATA_CLI{
	UINT16           wKindID; //装备类型
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
	UINT8            byDressPos; //穿戴位置
	UINT16           wLevel; //装备等级
	UINT16           wDressLevel; //装备穿戴等级
	DT_BATTLE_ATTRIBUTE stBattleAttribute; //当前战斗属性
	UINT32           dwRecoverPrice; //回收价格
	UINT32           dwScience; //回收器魂
}DT_EQUIP_DATA_CLI;

typedef struct tagDT_EQUIP_DATA_LST_CLI{
	UINT16           wEquipNum; //装备个数
	DT_EQUIP_DATA_CLI astEquipList[MAX_BAG_OPEN_NUM]; //装备信息
}DT_EQUIP_DATA_LST_CLI;

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
	DT_ESTIMATE_EQUIP_DATA stEquip; //进入游戏请求
	DT_ESTIMATE_DRUG_DATA stDrug; //进入游戏请求
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
	DT_EQUIP_DATA_STRENGTHEN_CLI astEquipStrengthenInfo[MAX_BAG_OPEN_NUM]; //装备强化信息
	DT_EQUIP_COMPOSE_DATA_CLI astEquipComposeInfo[MAX_BAG_OPEN_NUM]; //装备合成信息
}DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI;

typedef struct tagDT_SEED_DATA_CLI{
	UINT8            byType; //种子类型
	UINT16           wValue; //增加值
	UINT16           wKindID; //获取的物品ID
}DT_SEED_DATA_CLI;

typedef struct tagDT_GOODS_DATA_CLI{
	UINT16           wKindID; //物品类型
	UINT8            byGoodsMainKindID; //物品大类
	UINT16           wUseLevel; //使用等级
	UINT16           wPileCount; //叠加数
	UINT32           dwRecoverPrice; //回收价格
	UINT32           dwValue; //使用效果值(不同物品不同含义)
	UINT16           wComposeCount; //物品合成个数
	UINT16           wComposeKindID; //物品合成ID
	UINT8            byComposeItemNum; //材料合成的道具/礼包中的道具
	UINT16           awComposeItem[MAX_COMPOSE_ITEM_NUM]; //道具信息
	UINT8            bySeedFlag; //种子标志
	DT_SEED_DATA_CLI astSeedData[1]; //道具信息
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
	DT_EQUIP_DATA_CLI stEquip; //进入游戏请求
	DT_GOODS_DATA_CLI stGoods; //进入游戏请求
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
	DT_GIVEN_TASK_DATA stGivenTask; //进入游戏请求
	DT_ACCEPT_TASK_DATA stAcceptTask; //进入游戏请求
	DT_ENCOURAGE_TASK_DATA stEncourageTask; //进入游戏请求
	DT_INVALID_TASK_DATA stInvalidTask; //进入游戏请求
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
	UINT32           dwGuide; //当前需要做的引导
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

typedef struct tagPKT_CLIGS_ENTERGS_ACK{
	tagPKT_CLIGS_ENTERGS_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTERGS_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
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
	UINT8            byUnreadRaceBattleLogNum; //未观看竞技场战报个数
	UINT8            byUnreadPlunderBattleLogNum; //未观看掠夺战报个数
	UINT16           wJuniorLevel; //新手等级
	DT_EXT_FUNC_PARAM_LST stExtFuncParamLst; //扩展功能参数列表
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
	DT_SKILL_DATA_LIST_CLI astSkillInfo[MAX_HERO_NUM]; //武将技能信息
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
	DT_PLAYER_BATTLE_INFO stPlayerBattleInfo; //玩家战斗信息
	DT_HERO_DATA_CLI stHeroInfo; //武将信息
	UINT8            byEquipBagFlag; //是否返装备背包信息
	DT_EQUIP_BAG_DATA_CLI astEquipBagDataInfo[1]; //未装备的装备信息
	UINT8            byGoodsCellNum; //物品占用格子数
}PKT_CLIGS_GET_HERO_INFO_ACK;

typedef struct tagDT_FUNC_COMMON_PROMPT{
	UINT8            byBuildKindID; //建筑类型
	UINT8            byFuncFlag; //建筑功能(0不需提示，>0不同的值不同建筑表示不同功能)
	UINT32           dwFuncCountDown; //功能的倒计时
	UINT8            byExt; //扩展字段
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
	UINT8            byUnreadRaceBattleLogNum; //未观看竞技场战报个数
	UINT8            byUnreadPlunderBattleLogNum; //未观看掠夺战报个数
	UINT16           awUnlockHeroID[MAX_UNLOCK_HERO_NUM]; //解锁伙伴提示(0表示没有)
	UINT16           wUnlockEquipLevel; //解锁装备提示(0表示没有)
	UINT32           dwUnlockElite; //解锁精英副本提示(0表示没有，第1个字节表示精英副本城镇idx，第2/3个字节表示主怪物ID)
	UnlockScienceInfo stUnlockScienceList; //解锁科技提示
	UINT32           dwGuide; //当前需要做的引导
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
	DT_OTHER_PROMPT_DATA stOtherPromptInfo; //其他提示信息
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
	UINT16           wEliteUnBattleNum; //未打的精英副本个数
}PKT_CLIGS_ENTER_HOME_ACK;

typedef struct tagDT_BATTLE_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwExperience; //经验
	UINT32           dwStory; //阅历
}DT_BATTLE_ENCOURAGE_DATA;

typedef struct tagDT_DATA_AFTER_BATTLE_ENCOURAGE{
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurStory; //当前阅历
	UINT8            byHeroNum; //武将个数
	DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息
}DT_DATA_AFTER_BATTLE_ENCOURAGE;

typedef struct tagDT_RACE_CHALLENGE_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwScience; //器魂点
}DT_RACE_CHALLENGE_ENCOURAGE_DATA;

typedef struct tagDT_RACE_RANK_ENCOURAGE_DATA{
	UINT32           dwCoin; //铜钱
	UINT32           dwScience; //器魂点
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
	UINT32           dwExperience; //奖励经验
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
}DT_PASS_ENCOURAGE_BOX_GROUP_DATA;

typedef struct tagDT_PASS_ENCOURAGE_BOX_DATA{
	UINT8            byGroupNum; //宝箱组个数
	DT_PASS_ENCOURAGE_BOX_GROUP_DATA astBoxGroupInfo[MAX_ENCOURAGE_BOX_GROUP_NUM]; //宝箱组信息
}DT_PASS_ENCOURAGE_BOX_DATA;

typedef struct tagPKT_CLIGS_ENTER_INSTANCE_ACK{
	tagPKT_CLIGS_ENTER_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_ENTER_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_PHYSTRENGTH_DATA_CLI stPhyStrengthInfo; //体力信息
	UINT32           dwMaxHP; //总HP（包含所有上阵武将属性）
	UINT32           dwCurHP; //当前HP（包含所有上阵武将属性）
	DT_MONSTER_DATA_LIST stMonsterInfo; //怪物信息
	UINT8            byHaveBoxFlag; //是否有奖励宝箱，1为有宝箱
	DT_PASS_ENCOURAGE_BOX_DATA astBoxInfo[1]; //奖励宝箱信息
	UINT16           wDoubleExpNum; //经验双倍符个数，大于0表示有使用双倍符
}PKT_CLIGS_ENTER_INSTANCE_ACK;

typedef struct tagPKT_CLIGS_BATTLE_REQ{
	tagPKT_CLIGS_BATTLE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_BATTLE_REQ)); }
	UINT8            byBattleIndex; //战斗序号(第几波怪，从1开始)
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
	UINT32           dwGuide; //当前需要做的引导
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

typedef struct tagPKT_CLIGS_GET_SCENE_INSTANCE_ACK{
	tagPKT_CLIGS_GET_SCENE_INSTANCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_GET_SCENE_INSTANCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wIndex; //场景序号
	UINT8            byEnterPhyStrength; //进入体力要求
	DT_INSTANCE_ID   stMaxInstanceData; //最大副本信息
	DT_INSTANCE_ID   stMaxUnlockInstance; //解锁副本信息
	UINT8            byTownNum; //城镇数
	DT_TOWN_INSTANCE_RECORD_DATA_CLI astTownInstanceInfo[MAX_TOWN_PER_SCENE_NUM]; //城镇副本信息
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
	DT_EQUIP_DATA_STRENGTHEN_CLI stEquipStrengthenInfo; //装备强化信息
	DT_EQUIP_COMPOSE_DATA_CLI stEquipComposeInfo; //装备合成信息
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
}PKT_CLIGS_OPEN_SMITHY_ACK;

typedef struct tagPKT_CLIGS_OPEN_RECRUIT_REQ{
	tagPKT_CLIGS_OPEN_RECRUIT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RECRUIT_REQ)); }
	UINT8            byBuildKindID; //建筑类型
}PKT_CLIGS_OPEN_RECRUIT_REQ;

typedef struct tagDT_HERO_ATTR_ADD_CLI{
	UINT32           dwAddAttack; //攻击
	UINT32           dwAddHP; //血量
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
	UINT8            byRecuitUnLock; //是否可博弈
}DT_RECRUIT_HERO_DATA;

typedef struct tagPKT_CLIGS_OPEN_RECRUIT_ACK{
	tagPKT_CLIGS_OPEN_RECRUIT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_RECRUIT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
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
	UINT32           dwGuide; //当前需要做的引导
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
	UINT32           dwGuide; //当前需要做的引导
	UINT64           qwStory; //阅历
	DT_NEW_EQUIP_PROMPT_DATA stNewEquipPromptInfo; //新装备更强提示信息
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
	UINT32           dwPower; //战力
	UINT32           dwFirstAttack; //先攻值
	UINT8            byHaveRecvEncourageFlag; //今日是否领取过排名奖励
	DT_RACE_RANK_ENCOURAGE_DATA stTodayRankEncourage; //今日可领取的排名奖励(未领取有效)
	DT_RACE_RANK_ENCOURAGE_DATA stTomorrowRankEncourage; //明日可领取的排名奖励
	UINT16           wCanChallengeNum; //玩家当前可挑战次数
	UINT16           wFreeChallengeNum; //今日可免费挑战次数
	UINT32           dwChallengeNeedGold; //收费挑战需元宝数
	UINT16           wChallengeCountDown; //挑战倒计时，单位秒
	UINT32           dwClrCDGold; //清除CD所需元宝(每分钟)
	UINT8            byChallengeNum; //挑战玩家个数
	DT_CHALLENGE_PLAYER_DATA astChallengePlayerList[MAX_CHALLENGE_NUM]; //挑战玩家信息列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //功能提示信息
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
	UINT16           wErrCode; //进入游戏请求
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
	UINT8            bySkillNum; //技能数
	DT_SKILL_DATA_CLI astSkillInfo[MAX_SKILL_SLOT_NUM]; //技能信息
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
	UINT32           dwGuide; //当前需要做的引导
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
	UINT32           dwExperience; //当前经验
	UINT32           dwUpgradeExperience; //下一级升级经验
}DT_HERO_UPGRADE_DATA2;

typedef struct tagDT_DATA_AFTER_ONHOOK{
	UINT8            byUpgradeHeroNum; //升级武将个数
	DT_HERO_UPGRADE_DATA2 astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]; //武将升级信息
	UINT64           qwCurCoin; //当前铜钱
	UINT64           qwCurGold; //当前元宝
	UINT64           qwCurStory; //阅历
	DT_PHYSTRENGTH_DATA_CLI stCurPhyStrengthInfo; //当前玩家体力信息
	DT_EXPERIENCE_DATA stExperienceInfo; //经验信息
	UINT16           wUnlockEquipLevel; //解锁装备提示(0表示没有)
	UnlockScienceInfo stUnlockScienceList; //解锁科技提示
	UINT8            byUnlockNewInstanceFlag; //是否解锁新副本
	DT_INSTANCE_ID   astUnlockCommonInstanceID[1]; //解锁的普通副本ID
	UINT8            byHaveNewInstanceFlag; //是否有新可打普通副本,1有效
	DT_INSTANCE_DATA_CLI astNewCommonInstanceID[1]; //新可打普通副本ID
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
	TCHAR            aszDspName[USERNAME_LEN]; //进入游戏请求
}PKT_CLILS_REGISTER_DSPNAME_REQ;

typedef struct tagPKT_CLILS_REGISTER_DSPNAME_ACK{
	tagPKT_CLILS_REGISTER_DSPNAME_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_REGISTER_DSPNAME_ACK)); }
	UINT16           wErrCode; //进入游戏请求
	TCHAR            aszDspName[USERNAME_LEN]; //进入游戏请求
}PKT_CLILS_REGISTER_DSPNAME_ACK;

typedef struct tagPKT_CLILS_USE_GOODS_REQ{
	tagPKT_CLILS_USE_GOODS_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_USE_GOODS_REQ)); }
	UINT16           wGoodsKindID; //物品类型ID
}PKT_CLILS_USE_GOODS_REQ;

typedef struct tagPKT_CLILS_USE_GOODS_ACK{
	tagPKT_CLILS_USE_GOODS_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_USE_GOODS_ACK)); }
	UINT16           wErrCode; //进入游戏请求
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
	UINT16           wErrCode; //进入游戏请求
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
	UINT16           wErrCode; //进入游戏请求
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
	UINT16           wCanPlunderNum; //玩家当前可掠夺次数
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
	UINT16           wCareerID; //职业
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

typedef struct tagDT_GODWEAPON_ATTR_INC_DATA{
	UINT8            byAttrKind; //属性类型
	UINT32           dwAttrValue; //属性值
}DT_GODWEAPON_ATTR_INC_DATA;

typedef struct tagDT_GODWEAPON_ATTR_DATA{
	DT_BATTLE_ATTRIBUTE stBattleAttr; //战斗属性
	DT_HIDE_BATTLE_ATTRIBUTE stHideBattleAttr; //隐藏的战斗属性
	DT_EXT_BATTLE_ATTRIBUTE stExtBattleAttr; //扩展的战斗属性
}DT_GODWEAPON_ATTR_DATA;

typedef struct tagDT_GODWEAPON_DATA_CLI{
	UINT16           wHeroKindID; //武将类型ID
	UINT16           wQuality; //品质
	UINT16           wLevel; //等级
	DT_GODWEAPON_ATTR_DATA stIncAttr; //神器提升的属性
	DT_GODWEAPON_ATTR_INC_DATA stNextLevelIncAttr; //下一级提升的属性
	UINT16           wQualityRequireLevel; //升阶神器玩家等级要求
	UINT32           dwUpgrageScience; //下一升级所需器魂(科技)点
	UINT64           qwRecoverScience; //释放神器返还的器魂(科技)点
	UINT16           wUpgradeQualityNeedItemNum; //进阶需要的进阶道具个数
	UINT8            byMaxLevelFlag; //是达到最大等级(1为达到最大等级)
	DT_GODWEAPON_ATTR_INC_DATA astQualityUpgradeEncAttr[MAX_GODWEAPON_QUALITY_NUM]; //所有品的升级奖励属性
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
}PKT_CLIGS_FREE_GODWEAPON_REQ;

typedef struct tagPKT_CLIGS_FREE_GODWEAPON_ACK{
	tagPKT_CLIGS_FREE_GODWEAPON_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_FREE_GODWEAPON_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将类型ID
	UINT64           qwPlayerScience; //玩家当前的器魂(科技)点
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
	DT_GODWEAPON_DATA_CLI stGodweaponInfo; //神器信息
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_FREE_GODWEAPON_ACK;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_REQ{
	tagPKT_CLIGS_OPEN_AWAKEN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_AWAKEN_REQ;

typedef struct tagPKT_CLIGS_OPEN_AWAKEN_ACK{
	tagPKT_CLIGS_OPEN_AWAKEN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_AWAKEN_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_AWAKEN_DATA   stAwakenInfo; //悟道信息
	UINT32           dwAwakenCoin; //悟道所需铜币
	UINT32           dwStudyClipColorExp; //心得经验碎片颜色的经验值
	UINT8            abyGridStudyClipColor[MAX_GRID_STUDY_CLIP_NUM]; //存放格子的心得碎片信息(颜色为0则没有)
	UINT16           awPickupStudyClipNum[STUDY_COLOR_NUM]; //已拾取的心得碎片信息(数量为0则没有)
}PKT_CLIGS_OPEN_AWAKEN_ACK;

typedef struct tagPKT_CLIGS_AWAKEN_REQ{
	tagPKT_CLIGS_AWAKEN_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_AWAKEN_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_AWAKEN_REQ;

typedef struct tagPKT_CLIGS_AWAKEN_ACK{
	tagPKT_CLIGS_AWAKEN_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_AWAKEN_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byAwakenLevel; //悟道等级
	UINT8            byStudyColor; //悟道所得心得(心得颜色)
	UINT32           dwStudyExp; //悟道所得心得经验
	UINT32           dwAwakenCoin; //悟道所需铜币
	UINT64           qwPlayerCoin; //玩家当前的铜币
}PKT_CLIGS_AWAKEN_ACK;

typedef struct tagPKT_CLIGS_PICKUP_STUDY_CLIP_REQ{
	tagPKT_CLIGS_PICKUP_STUDY_CLIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_PICKUP_STUDY_CLIP_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_PICKUP_STUDY_CLIP_REQ;

typedef struct tagPKT_CLIGS_PICKUP_STUDY_CLIP_ACK{
	tagPKT_CLIGS_PICKUP_STUDY_CLIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_PICKUP_STUDY_CLIP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwStudyExp; //玩家当前的心得经验
	UINT16           awPickupStudyClipNum[STUDY_COLOR_NUM]; //已拾取的心得碎片信息(数量为0则没有)
}PKT_CLIGS_PICKUP_STUDY_CLIP_ACK;

typedef struct tagPKT_CLIGS_OPEN_STUDY_REQ{
	tagPKT_CLIGS_OPEN_STUDY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STUDY_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_STUDY_REQ;

typedef struct tagDT_STUDY_DATA_CLI{
	UINT8            byKind; //心得类型
	UINT8            byColor; //颜色
	UINT8            byLevel; //等级
	UINT8            byPos; //存放位置
	UINT8            byAttrKind; //心得影响属性类型
	UINT32           dwAttrValue; //心得影响属性值
}DT_STUDY_DATA_CLI;

typedef struct tagDT_STUDY_DATA_LST_CLI{
	UINT8            byNum; //心得个数
	DT_STUDY_DATA_CLI astStudyLst[MAX_STUDY_STORE_NUM]; //心得信息列表
}DT_STUDY_DATA_LST_CLI;

typedef struct tagDT_HERO_STUDY_DATA_CLI{
	UINT16           wHeroID; //武将ID
	DT_STUDY_DATA_LST_CLI stStudyInfo; //心得信息
}DT_HERO_STUDY_DATA_CLI;

typedef struct tagDT_HERO_STUDY_DATA_LST_CLI{
	UINT8            byNum; //武将个数
	DT_HERO_STUDY_DATA_CLI astHeroStudyLst[MAX_FORMATION_IDX_NUM]; //武将心得信息
}DT_HERO_STUDY_DATA_LST_CLI;

typedef struct tagPKT_CLIGS_OPEN_STUDY_ACK{
	tagPKT_CLIGS_OPEN_STUDY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STUDY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwStudyExp; //玩家当前的心得经验
	UINT16           awPickupStudyClipNum[STUDY_COLOR_NUM]; //已拾取的心得碎片信息(数量为0则没有)
	DT_STUDY_DATA_LST_CLI stStoreStudyInfo; //心得存储格子信息
	DT_HERO_STUDY_DATA_LST_CLI stHeroStudyInfo; //武将心得信息
}PKT_CLIGS_OPEN_STUDY_ACK;

typedef struct tagPKT_CLIGS_DRESS_STUDY_REQ{
	tagPKT_CLIGS_DRESS_STUDY_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_STUDY_REQ)); }
	UINT8            byOpPos; //操作区域(0武将心得，1存储心得，2武将和存储交互)
	UINT16           wHeroKindID; //武将ID
	UINT8            byDressType; //穿戴方式(穿上(SrcPos为存储心得)/卸下(SrcPos为武将心得)/替换(SrcPos为存储心得))
	UINT8            bySrcPos; //源位置
	UINT8            byDstPos; //目标位置
}PKT_CLIGS_DRESS_STUDY_REQ;

typedef struct tagPKT_CLIGS_DRESS_STUDY_ACK{
	tagPKT_CLIGS_DRESS_STUDY_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_STUDY_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将ID
	UINT8            byDressType; //穿戴方式(穿上(SrcPos为存储心得)/卸下(SrcPos为武将心得)/替换(SrcPos为存储心得))
	UINT8            bySrcPos; //源位置
	UINT8            byDstPos; //目标位置
}PKT_CLIGS_DRESS_STUDY_ACK;

typedef struct tagPKT_CLIGS_STUDY_CLIP_TO_EXP_REQ{
	tagPKT_CLIGS_STUDY_CLIP_TO_EXP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_CLIP_TO_EXP_REQ)); }
	UINT8            byColor; //心得颜色
	UINT16           wNum; //心得个数
}PKT_CLIGS_STUDY_CLIP_TO_EXP_REQ;

typedef struct tagPKT_CLIGS_STUDY_CLIP_TO_EXP_ACK{
	tagPKT_CLIGS_STUDY_CLIP_TO_EXP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_CLIP_TO_EXP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT64           qwStudyExp; //玩家当前的心得经验
	UINT16           awPickupStudyClipNum[STUDY_COLOR_NUM]; //已拾取的心得碎片信息(数量为0则没有)
}PKT_CLIGS_STUDY_CLIP_TO_EXP_ACK;

typedef struct tagPKT_CLIGS_STUDY_TO_EXP_REQ{
	tagPKT_CLIGS_STUDY_TO_EXP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_TO_EXP_REQ)); }
	UINT16           wHeroKindID; //武将ID(0则为存储的心得，非0则为武将心得)
	UINT8            byPosIdx; //位置序号
}PKT_CLIGS_STUDY_TO_EXP_REQ;

typedef struct tagPKT_CLIGS_STUDY_TO_EXP_ACK{
	tagPKT_CLIGS_STUDY_TO_EXP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_TO_EXP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将ID(0则为存储的心得，非0则为武将心得)
	UINT8            byPosIdx; //位置序号
	UINT64           qwStudyExp; //玩家当前的心得经验
	UINT16           awPickupStudyClipNum[STUDY_COLOR_NUM]; //已拾取的心得碎片信息(数量为0则没有)
	UINT8            byHeroStudyFlag; //武将心得标志，1为武将心得
	DT_STUDY_DATA_LST_CLI astHeroStudyInfo[1]; //武将心得信息
}PKT_CLIGS_STUDY_TO_EXP_ACK;

typedef struct tagPKT_CLIGS_OPEN_STUDY_CONVERT_REQ{
	tagPKT_CLIGS_OPEN_STUDY_CONVERT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STUDY_CONVERT_REQ)); }
	UINT8            byExt; //扩展信息，未使用
}PKT_CLIGS_OPEN_STUDY_CONVERT_REQ;

typedef struct tagDT_STUDY_CFG_DATA{
	UINT8            byKind; //心得类型
	UINT8            byColor; //颜色
	UINT8            byAttrKind; //心得影响属性类型
	UINT32           dwAttrValue; //心得影响属性值
	UINT32           dwIncAttrValue; //每升1级增加的属性值
	UINT16           wNeedClipNum; //兑换所需碎片个数
}DT_STUDY_CFG_DATA;

typedef struct tagPKT_CLIGS_OPEN_STUDY_CONVERT_ACK{
	tagPKT_CLIGS_OPEN_STUDY_CONVERT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_STUDY_CONVERT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_STUDY_CFG_DATA astStudyCfgInfo[MAX_STUDY_KIND_NUM]; //所有心得类型配置信息
}PKT_CLIGS_OPEN_STUDY_CONVERT_ACK;

typedef struct tagPKT_CLIGS_STUDY_CONVERT_REQ{
	tagPKT_CLIGS_STUDY_CONVERT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_CONVERT_REQ)); }
	UINT8            byKind; //心得类型
	UINT8            byColor; //颜色
}PKT_CLIGS_STUDY_CONVERT_REQ;

typedef struct tagPKT_CLIGS_STUDY_CONVERT_ACK{
	tagPKT_CLIGS_STUDY_CONVERT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_CONVERT_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_STUDY_CONVERT_ACK;

typedef struct tagPKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ{
	tagPKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ)); }
	UINT8            bySellectFlag; //选择类型,0不选择，1选择
}PKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ;

typedef struct tagPKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK{
	tagPKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            bySellectFlag; //选择类型,0不选择，1选择
}PKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK;

typedef struct tagPKT_CLIGS_STUDY_UPGRADE_REQ{
	tagPKT_CLIGS_STUDY_UPGRADE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_UPGRADE_REQ)); }
	UINT16           wHeroKindID; //武将ID
	UINT8            byPos; //源位置
	UINT8            byLevel; //要升级到的等级
}PKT_CLIGS_STUDY_UPGRADE_REQ;

typedef struct tagPKT_CLIGS_STUDY_UPGRADE_ACK{
	tagPKT_CLIGS_STUDY_UPGRADE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_STUDY_UPGRADE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wHeroKindID; //武将ID
	UINT8            byPos; //源位置
	UINT8            byLevel; //要升级到的等级
	DT_HERO_STUDY_DATA_CLI stHeroStudyInfo; //武将心得信息
}PKT_CLIGS_STUDY_UPGRADE_ACK;

typedef struct tagPKT_CLIGS_RACE_BATTLELOG_NTF{
	tagPKT_CLIGS_RACE_BATTLELOG_NTF() { memset(this, 0, sizeof(tagPKT_CLIGS_RACE_BATTLELOG_NTF)); }
	UINT8            byUnreadNum; //未观看战报个数
	DT_RACE_BATTLELOG_DATA_CLI stBattleLog; //战报
	UINT64           qwPlayerCoin; //玩家当前铜币
	UINT64           qwPlayerScience; //玩家当前器魂点
}PKT_CLIGS_RACE_BATTLELOG_NTF;

typedef struct tagDT_LOCK_SKILL_DATA{
	UINT16           wSkillID; //技能ID
	DT_INSTANCE_ID   stUnlockInstanceID; //解锁副本
}DT_LOCK_SKILL_DATA;

typedef struct tagPKT_CLIGS_OPEN_SKILL_TAB_REQ{
	tagPKT_CLIGS_OPEN_SKILL_TAB_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SKILL_TAB_REQ)); }
	UINT8            byExt; //扩展字段，暂时未用
}PKT_CLIGS_OPEN_SKILL_TAB_REQ;

typedef struct tagPKT_CLIGS_OPEN_SKILL_TAB_ACK{
	tagPKT_CLIGS_OPEN_SKILL_TAB_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SKILL_TAB_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCoachTalent; //当前主伙伴的修为
	UINT16           wCoachSkillTalent; //当前主伙伴技能提升的修为
	UINT16           wTrainNormalTalent; //主伙伴培养一次普通技能增加的修为
	UINT16           wTrainRareTalent; //主伙伴培养一次稀有技能增加的修为
	UINT16           wCurNormalCardNum; //当前拥有的普通技能卡片数
	UINT16           wCurRareCardNum; //当前拥有的稀有技能卡片数
	UINT8            bySkillNum; //技能个数
	DT_SKILL_DATA_CLI astSkillInfo[MAX_SKILL_NUM]; //技能信息
	UINT8            byLockSkillNum; //锁定(未解锁)技能个数
	DT_LOCK_SKILL_DATA astLockSkillInfo[MAX_SKILL_NUM]; //锁定(未解锁)技能信息
}PKT_CLIGS_OPEN_SKILL_TAB_ACK;

typedef struct tagPKT_CLIGS_UPGRADE_SKILL_REQ{
	tagPKT_CLIGS_UPGRADE_SKILL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_SKILL_REQ)); }
	UINT16           wSkillID; //技能ID
}PKT_CLIGS_UPGRADE_SKILL_REQ;

typedef struct tagPKT_CLIGS_UPGRADE_SKILL_ACK{
	tagPKT_CLIGS_UPGRADE_SKILL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_UPGRADE_SKILL_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCoachTalent; //当前主伙伴的修为
	UINT16           wCoachSkillTalent; //当前主伙伴技能提升的修为
	UINT16           wCurNormalCardNum; //当前拥有的普通技能卡片数
	UINT16           wCurRareCardNum; //当前拥有的稀有技能卡片数
	DT_SKILL_DATA_CLI stSkillInfo; //技能信息
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
}PKT_CLIGS_UPGRADE_SKILL_ACK;

typedef struct tagPKT_CLIGS_DRESS_SKILL_REQ{
	tagPKT_CLIGS_DRESS_SKILL_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_SKILL_REQ)); }
	UINT16           wHeroID; //武将ID
	UINT16           wSkillID; //技能ID
	UINT16           wSkillLevel; //技能等级
}PKT_CLIGS_DRESS_SKILL_REQ;

typedef struct tagPKT_CLIGS_DRESS_SKILL_ACK{
	tagPKT_CLIGS_DRESS_SKILL_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_DRESS_SKILL_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
}PKT_CLIGS_DRESS_SKILL_ACK;

typedef struct tagPKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ{
	tagPKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ)); }
	UINT8            byExt; //扩展字段，暂时未用
}PKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ;

typedef struct tagPKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK{
	tagPKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCurNormalCardNum; //当前拥有的普通技能卡片数
	UINT16           wCurRareCardMakerNum; //当前拥有的稀有技能卡合成石数
	UINT16           wNeedNormalCardNum; //合成稀有技能卡所需普通技能卡数
	UINT8            byMakerRate; //合成成功率
}PKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK;

typedef struct tagPKT_CLIGS_MAKER_SKILL_CARD_REQ{
	tagPKT_CLIGS_MAKER_SKILL_CARD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MAKER_SKILL_CARD_REQ)); }
	UINT8            byExt; //扩展字段，暂时未用
}PKT_CLIGS_MAKER_SKILL_CARD_REQ;

typedef struct tagPKT_CLIGS_MAKER_SKILL_CARD_ACK{
	tagPKT_CLIGS_MAKER_SKILL_CARD_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MAKER_SKILL_CARD_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wCurNormalCardNum; //当前拥有的普通技能卡片数
	UINT16           wCurRareCardNum; //当前拥有的稀有技能卡片数
	UINT16           wCurRareCardMakerNum; //当前拥有的稀有技能卡合成石数
}PKT_CLIGS_MAKER_SKILL_CARD_ACK;

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
	UINT32           dwLevyCountDown; //征收的倒计时
	UINT32           dwLevyCoin; //可征收的铜币
	UINT8            byLevyNum; //可征收的次数
	UINT8            byTotalLevyNum; //征收的总次数
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
	DT_EQUIP_DATA_STRENGTHEN_CLI stEquipStrengthenInfo; //装备强化信息
	DT_EQUIP_COMPOSE_DATA_CLI stEquipComposeInfo; //装备合成信息
	UINT64           qwCurGold; //玩家当前元宝
	DT_POWER_CHANGE_DATA stPowerInfo; //战力信息
}PKT_CLIGS_COMPOSE_EQUIP_ACK;

typedef struct tagPKT_CLIGS_OPEN_PHYSTRENGTH_REQ{
	tagPKT_CLIGS_OPEN_PHYSTRENGTH_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PHYSTRENGTH_REQ)); }
	UINT8            byExt; //扩展字段，暂时未用
}PKT_CLIGS_OPEN_PHYSTRENGTH_REQ;

typedef struct tagPKT_CLIGS_OPEN_PHYSTRENGTH_ACK{
	tagPKT_CLIGS_OPEN_PHYSTRENGTH_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_PHYSTRENGTH_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT8            byStrength; //体力值
	UINT8            byBuyTimes; //购买次数
	UINT32           dwBuyGold; //花费的元宝
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
	DT_EQUIP_DATA_STRENGTHEN_CLI stEquipStrengthenInfo; //装备强化信息
	DT_EQUIP_COMPOSE_DATA_CLI stEquipComposeInfo; //装备合成信息
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
	UINT16           wCropValue; //种子收益
	UINT16           wCropGainKindID; //收益的物品ID
	UINT16           wCoin; //种子铜币价格
	UINT16           wGold; //种子元宝价格
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
	UINT8            byCropType; //种植类型
	UINT32           dwCropValue; //种子收益
	UINT16           wCropKindID; //生成物品ID
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
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

typedef struct tagPKT_CLIGS_OPEN_EXPERIENCE_ACK{
	tagPKT_CLIGS_OPEN_EXPERIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_EXPERIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT16           wLevel; //武将等级
	UINT32           dwExperience; //当前经验
	UINT32           dwUpgradeExperience; //升级经验
	DT_EXPERIENCE_DRUG_DATA astExperienceDrugDataList[3]; //经验丹信息
}PKT_CLIGS_OPEN_EXPERIENCE_ACK;

typedef struct tagPKT_CLIGS_USER_EXPERIENCE_REQ{
	tagPKT_CLIGS_USER_EXPERIENCE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_USER_EXPERIENCE_REQ)); }
	UINT16           wHeroID; //武将ID
	UINT16           wExperienceDrugID; //经验丹ID
}PKT_CLIGS_USER_EXPERIENCE_REQ;

typedef struct tagPKT_CLIGS_USER_EXPERIENCE_ACK{
	tagPKT_CLIGS_USER_EXPERIENCE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_USER_EXPERIENCE_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	DT_EXPERIENCE_DRUG_DATA stExperienceDrugData; //经验丹信息
	DT_POWER_CHANGE_DATA stPowerChangeData; //武将战力信息
	DT_HERO_BASE_DATA_CLI stHeroBaseDataCli; //武将相关信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_USER_EXPERIENCE_ACK;

typedef struct tagPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ{
	tagPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ)); }
	UINT8            byExt; //扩展字段，暂时未使用
}PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ;

typedef struct tagDT_SCIENCE_ALREADY_OPEN_DATA{
	UINT8            byKind; //属性类型
	UINT16           wLevel; //级别
	UINT16           wCurValue; //当前数值
	UINT16           wNextValue; //升级后数值
	UINT16           wCostStory; //升级所需阅历
	UINT16           wNeedPlayerLevel; //升级所需人物等级
	UINT8            byMaxLevel; //是否满级
}DT_SCIENCE_ALREADY_OPEN_DATA;

typedef struct tagDT_SCIENCE_WILL_OPEN_DATA{
	UINT8            byKind; //属性类型
	UINT16           wOpenLevel; //开放等级
	UINT16           wLv1Value; //第一级的数值
	UINT16           wCostStory; //升级所需阅历
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
	DT_EQUIP_DATA_LST_CLI stEquipDataLstCli; //升级后变更的信息
}PKT_CLIGS_OPEN_MELT_EQUIP_ACK;

typedef struct tagDT_MELT_EQUIP_DATA{
	UINT16           wEquipID; //装备ID
	UINT16           wIdx; //该同类型装备的序号，用于区别相同类型的装备
}DT_MELT_EQUIP_DATA;

typedef struct tagPKT_CLIGS_MELT_EQUIP_REQ{
	tagPKT_CLIGS_MELT_EQUIP_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_MELT_EQUIP_REQ)); }
	UINT16           wEquipNum; //装备个数
	DT_MELT_EQUIP_DATA astMeltEquipDataList[MAX_BAG_OPEN_NUM]; //装备信息
}PKT_CLIGS_MELT_EQUIP_REQ;

typedef struct tagPKT_CLIGS_MELT_EQUIP_ACK{
	tagPKT_CLIGS_MELT_EQUIP_ACK() { memset(this, 0, sizeof(tagPKT_CLIGS_MELT_EQUIP_ACK)); }
	UINT16           wErrorCode; //错误类型
	UINT64           qwScience; //当前器魂点
	DT_EQUIP_DATA_LST_CLI stEquipDataLstCli; //升级后变更的信息
	DT_TASK_DATA_LIST_CLI stTaskInfoList; //变化的任务信息列表
	DT_TASK_COND_RECORD_LIST stTaskRecordList; //任务(接受状态的)完成情况记录列表
	DT_FUNC_PROMPT_DATA stPromptInfo; //影响的提示信息
}PKT_CLIGS_MELT_EQUIP_ACK;

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
	UINT64           qwExchangeGold; //充值元宝数
}PKT_CLIGS_EXCHANGE_RESULT;

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

typedef struct tagPKT_CLIGS_CHAT_REQ{
	tagPKT_CLIGS_CHAT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGS_CHAT_REQ)); }
	UINT32           dwPlayerID; //聊天的玩家ID
	UINT8            byChatType; //聊天类型
	UINT32           dwChatLen; //聊天类型
	UINT8            abyChatContent[MAX_CHAT_CONTENT_NUM]; //聊天内容
}PKT_CLIGS_CHAT_REQ;

typedef struct tagCHAT_DATA{
	UINT32           dwPlayerID; //聊天的玩家ID
	TCHAR            aszUserName[USERNAME_LEN]; //聊天的玩家名称
	UINT8            byChatType; //聊天类型
	UINT64           qwChatTime; //聊天时间
	UINT32           dwChatLen; //聊天长度
	UINT8            abyChatContent[MAX_CHAT_CONTENT_NUM]; //聊天内容
}CHAT_DATA;

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
	CLIGS_OPEN_AWAKEN_REQ           = 2148        ,
	CLIGS_OPEN_AWAKEN_ACK           = 2149        ,
	CLIGS_AWAKEN_REQ                = 2150        ,
	CLIGS_AWAKEN_ACK                = 2151        ,
	CLIGS_PICKUP_STUDY_CLIP_REQ     = 2152        ,
	CLIGS_PICKUP_STUDY_CLIP_ACK     = 2153        ,
	CLIGS_OPEN_STUDY_REQ            = 2154        ,
	CLIGS_OPEN_STUDY_ACK            = 2155        ,
	CLIGS_DRESS_STUDY_REQ           = 2156        ,
	CLIGS_DRESS_STUDY_ACK           = 2157        ,
	CLIGS_STUDY_CLIP_TO_EXP_REQ     = 2158        ,
	CLIGS_STUDY_CLIP_TO_EXP_ACK     = 2159        ,
	CLIGS_STUDY_TO_EXP_REQ          = 2160        ,
	CLIGS_STUDY_TO_EXP_ACK          = 2161        ,
	CLIGS_OPEN_STUDY_CONVERT_REQ    = 2162        ,
	CLIGS_OPEN_STUDY_CONVERT_ACK    = 2163        ,
	CLIGS_STUDY_CONVERT_REQ         = 2164        ,
	CLIGS_STUDY_CONVERT_ACK         = 2165        ,
	CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ  = 2166        ,
	CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK  = 2167        ,
	CLIGS_STUDY_CLIP_AUTO2EXP_REQ   = 2168        ,
	CLIGS_STUDY_CLIP_AUTO2EXP_ACK   = 2169        ,
	CLIGS_STUDY_UPGRADE_REQ         = 2170        ,
	CLIGS_STUDY_UPGRADE_ACK         = 2171        ,
	CLIGS_RACE_BATTLELOG_NTF        = 2172        ,
	CLIGS_OPEN_SKILL_TAB_REQ        = 2174        ,
	CLIGS_OPEN_SKILL_TAB_ACK        = 2175        ,
	CLIGS_UPGRADE_SKILL_REQ         = 2176        ,
	CLIGS_UPGRADE_SKILL_ACK         = 2177        ,
	CLIGS_DRESS_SKILL_REQ           = 2178        ,
	CLIGS_DRESS_SKILL_ACK           = 2179        ,
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
	CLIGS_OPEN_SKILL_CARD_MAKER_REQ  = 2196        ,
	CLIGS_OPEN_SKILL_CARD_MAKER_ACK  = 2197        ,
	CLIGS_MAKER_SKILL_CARD_REQ      = 2198        ,
	CLIGS_MAKER_SKILL_CARD_ACK      = 2199        ,
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
	CLIGS_SEED_COMPOSE_REQ          = 2259        ,
	CLIGS_SEED_COMPOSE_ACK          = 2260        ,
	CLIGS_CHAT_REQ                  = 2261        ,
	CLIGS_CHAT_ACK                  = 2262        ,
	CLIGS_CHAT_NTF                  = 2264        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_CLIGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ENTERGS_REQ( PKT_CLIGS_ENTERGS_REQ &data );
BOOL FromJsonPKT_CLIGS_ENTERGS_REQ(const string &strData, PKT_CLIGS_ENTERGS_REQ &data);
BOOL FromJsonPKT_CLIGS_ENTERGS_REQ(Json::Value &root, PKT_CLIGS_ENTERGS_REQ &data);
INT32 EncodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_FUNC_PARAM(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EXT_FUNC_PARAM( DT_EXT_FUNC_PARAM &data );
BOOL FromJsonDT_EXT_FUNC_PARAM(const string &strData, DT_EXT_FUNC_PARAM &data);
BOOL FromJsonDT_EXT_FUNC_PARAM(Json::Value &root, DT_EXT_FUNC_PARAM &data);
INT32 EncodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXT_FUNC_PARAM_LST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EXT_FUNC_PARAM_LST( DT_EXT_FUNC_PARAM_LST &data );
BOOL FromJsonDT_EXT_FUNC_PARAM_LST(const string &strData, DT_EXT_FUNC_PARAM_LST &data);
BOOL FromJsonDT_EXT_FUNC_PARAM_LST(Json::Value &root, DT_EXT_FUNC_PARAM_LST &data);
INT32 EncodeDT_POWER_CHANGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_POWER_CHANGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_POWER_CHANGE_DATA( DT_POWER_CHANGE_DATA &data );
BOOL FromJsonDT_POWER_CHANGE_DATA(const string &strData, DT_POWER_CHANGE_DATA &data);
BOOL FromJsonDT_POWER_CHANGE_DATA(Json::Value &root, DT_POWER_CHANGE_DATA &data);
INT32 EncodeDT_SKILL_BUFF_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_BUFF_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SKILL_BUFF_DATA( DT_SKILL_BUFF_DATA &data );
BOOL FromJsonDT_SKILL_BUFF_DATA(const string &strData, DT_SKILL_BUFF_DATA &data);
BOOL FromJsonDT_SKILL_BUFF_DATA(Json::Value &root, DT_SKILL_BUFF_DATA &data);
INT32 EncodeDT_SKILL_EFFECT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_EFFECT_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SKILL_EFFECT_DATA( DT_SKILL_EFFECT_DATA &data );
BOOL FromJsonDT_SKILL_EFFECT_DATA(const string &strData, DT_SKILL_EFFECT_DATA &data);
BOOL FromJsonDT_SKILL_EFFECT_DATA(Json::Value &root, DT_SKILL_EFFECT_DATA &data);
INT32 EncodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTACK_RANG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ATTACK_RANG_DATA( DT_ATTACK_RANG_DATA &data );
BOOL FromJsonDT_ATTACK_RANG_DATA(const string &strData, DT_ATTACK_RANG_DATA &data);
BOOL FromJsonDT_ATTACK_RANG_DATA(Json::Value &root, DT_ATTACK_RANG_DATA &data);
INT32 EncodeDT_SKILL_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SKILL_DATA_CLI( DT_SKILL_DATA_CLI &data );
BOOL FromJsonDT_SKILL_DATA_CLI(const string &strData, DT_SKILL_DATA_CLI &data);
BOOL FromJsonDT_SKILL_DATA_CLI(Json::Value &root, DT_SKILL_DATA_CLI &data);
INT32 EncodeDT_SKILL_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SKILL_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SKILL_DATA_LIST_CLI( DT_SKILL_DATA_LIST_CLI &data );
BOOL FromJsonDT_SKILL_DATA_LIST_CLI(const string &strData, DT_SKILL_DATA_LIST_CLI &data);
BOOL FromJsonDT_SKILL_DATA_LIST_CLI(Json::Value &root, DT_SKILL_DATA_LIST_CLI &data);
INT32 EncodeDT_HERO_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_BASE_DATA_CLI( DT_HERO_BASE_DATA_CLI &data );
BOOL FromJsonDT_HERO_BASE_DATA_CLI(const string &strData, DT_HERO_BASE_DATA_CLI &data);
BOOL FromJsonDT_HERO_BASE_DATA_CLI(Json::Value &root, DT_HERO_BASE_DATA_CLI &data);
INT32 EncodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPGRADE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_UPGRADE_DATA( DT_HERO_UPGRADE_DATA &data );
BOOL FromJsonDT_HERO_UPGRADE_DATA(const string &strData, DT_HERO_UPGRADE_DATA &data);
BOOL FromJsonDT_HERO_UPGRADE_DATA(Json::Value &root, DT_HERO_UPGRADE_DATA &data);
INT32 EncodeDT_EQUIP_DATA_STRENGTHEN_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_STRENGTHEN_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_DATA_STRENGTHEN_CLI( DT_EQUIP_DATA_STRENGTHEN_CLI &data );
BOOL FromJsonDT_EQUIP_DATA_STRENGTHEN_CLI(const string &strData, DT_EQUIP_DATA_STRENGTHEN_CLI &data);
BOOL FromJsonDT_EQUIP_DATA_STRENGTHEN_CLI(Json::Value &root, DT_EQUIP_DATA_STRENGTHEN_CLI &data);
INT32 EncodeDT_EQUIP_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_DATA_CLI( DT_EQUIP_DATA_CLI &data );
BOOL FromJsonDT_EQUIP_DATA_CLI(const string &strData, DT_EQUIP_DATA_CLI &data);
BOOL FromJsonDT_EQUIP_DATA_CLI(Json::Value &root, DT_EQUIP_DATA_CLI &data);
INT32 EncodeDT_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_DATA_LST_CLI( DT_EQUIP_DATA_LST_CLI &data );
BOOL FromJsonDT_EQUIP_DATA_LST_CLI(const string &strData, DT_EQUIP_DATA_LST_CLI &data);
BOOL FromJsonDT_EQUIP_DATA_LST_CLI(Json::Value &root, DT_EQUIP_DATA_LST_CLI &data);
INT32 EncodeDT_HERO_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EQUIP_DATA_LST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_EQUIP_DATA_LST_CLI( DT_HERO_EQUIP_DATA_LST_CLI &data );
BOOL FromJsonDT_HERO_EQUIP_DATA_LST_CLI(const string &strData, DT_HERO_EQUIP_DATA_LST_CLI &data);
BOOL FromJsonDT_HERO_EQUIP_DATA_LST_CLI(Json::Value &root, DT_HERO_EQUIP_DATA_LST_CLI &data);
INT32 EncodeDT_NEED_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEED_ITEM_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_NEED_ITEM_DATA( DT_NEED_ITEM_DATA &data );
BOOL FromJsonDT_NEED_ITEM_DATA(const string &strData, DT_NEED_ITEM_DATA &data);
BOOL FromJsonDT_NEED_ITEM_DATA(Json::Value &root, DT_NEED_ITEM_DATA &data);
INT32 EncodeDT_ESTIMATE_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ESTIMATE_EQUIP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ESTIMATE_EQUIP_DATA( DT_ESTIMATE_EQUIP_DATA &data );
BOOL FromJsonDT_ESTIMATE_EQUIP_DATA(const string &strData, DT_ESTIMATE_EQUIP_DATA &data);
BOOL FromJsonDT_ESTIMATE_EQUIP_DATA(Json::Value &root, DT_ESTIMATE_EQUIP_DATA &data);
INT32 EncodeDT_ESTIMATE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ESTIMATE_DRUG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ESTIMATE_DRUG_DATA( DT_ESTIMATE_DRUG_DATA &data );
BOOL FromJsonDT_ESTIMATE_DRUG_DATA(const string &strData, DT_ESTIMATE_DRUG_DATA &data);
BOOL FromJsonDT_ESTIMATE_DRUG_DATA(Json::Value &root, DT_ESTIMATE_DRUG_DATA &data);
INT32 EncodeUnDT_ESTIMATE_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_ESTIMATE_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_ESTIMATE_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ESTIMATE_ITEM_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ESTIMATE_ITEM_DATA( DT_ESTIMATE_ITEM_DATA &data );
BOOL FromJsonDT_ESTIMATE_ITEM_DATA(const string &strData, DT_ESTIMATE_ITEM_DATA &data);
BOOL FromJsonDT_ESTIMATE_ITEM_DATA(Json::Value &root, DT_ESTIMATE_ITEM_DATA &data);
INT32 EncodeDT_EQUIP_COMPOSE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_COMPOSE_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_COMPOSE_DATA_CLI( DT_EQUIP_COMPOSE_DATA_CLI &data );
BOOL FromJsonDT_EQUIP_COMPOSE_DATA_CLI(const string &strData, DT_EQUIP_COMPOSE_DATA_CLI &data);
BOOL FromJsonDT_EQUIP_COMPOSE_DATA_CLI(Json::Value &root, DT_EQUIP_COMPOSE_DATA_CLI &data);
INT32 EncodeDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI( DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI &data );
BOOL FromJsonDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI(const string &strData, DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI &data);
BOOL FromJsonDT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI(Json::Value &root, DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI &data);
INT32 EncodeDT_SEED_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SEED_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SEED_DATA_CLI( DT_SEED_DATA_CLI &data );
BOOL FromJsonDT_SEED_DATA_CLI(const string &strData, DT_SEED_DATA_CLI &data);
BOOL FromJsonDT_SEED_DATA_CLI(Json::Value &root, DT_SEED_DATA_CLI &data);
INT32 EncodeDT_GOODS_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GOODS_DATA_CLI( DT_GOODS_DATA_CLI &data );
BOOL FromJsonDT_GOODS_DATA_CLI(const string &strData, DT_GOODS_DATA_CLI &data);
BOOL FromJsonDT_GOODS_DATA_CLI(Json::Value &root, DT_GOODS_DATA_CLI &data);
INT32 EncodeDT_GOODS_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GOODS_DATA_LST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GOODS_DATA_LST_CLI( DT_GOODS_DATA_LST_CLI &data );
BOOL FromJsonDT_GOODS_DATA_LST_CLI(const string &strData, DT_GOODS_DATA_LST_CLI &data);
BOOL FromJsonDT_GOODS_DATA_LST_CLI(Json::Value &root, DT_GOODS_DATA_LST_CLI &data);
INT32 EncodeDT_ITEM_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ITEM_DATA_CLI( DT_ITEM_DATA_CLI &data );
BOOL FromJsonDT_ITEM_DATA_CLI(const string &strData, DT_ITEM_DATA_CLI &data);
BOOL FromJsonDT_ITEM_DATA_CLI(Json::Value &root, DT_ITEM_DATA_CLI &data);
INT32 EncodeDT_ITEM_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ITEM_DATA_LIST_CLI( DT_ITEM_DATA_LIST_CLI &data );
BOOL FromJsonDT_ITEM_DATA_LIST_CLI(const string &strData, DT_ITEM_DATA_LIST_CLI &data);
BOOL FromJsonDT_ITEM_DATA_LIST_CLI(Json::Value &root, DT_ITEM_DATA_LIST_CLI &data);
INT32 EncodeUnDT_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_ITEM_DATA_UNOIN(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_ITEM_DATA_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_CLI2(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ITEM_DATA_CLI2( DT_ITEM_DATA_CLI2 &data );
BOOL FromJsonDT_ITEM_DATA_CLI2(const string &strData, DT_ITEM_DATA_CLI2 &data);
BOOL FromJsonDT_ITEM_DATA_CLI2(Json::Value &root, DT_ITEM_DATA_CLI2 &data);
INT32 EncodeDT_ITEM_DATA_LIST_CLI2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_LIST_CLI2(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ITEM_DATA_LIST_CLI2( DT_ITEM_DATA_LIST_CLI2 &data );
BOOL FromJsonDT_ITEM_DATA_LIST_CLI2(const string &strData, DT_ITEM_DATA_LIST_CLI2 &data);
BOOL FromJsonDT_ITEM_DATA_LIST_CLI2(Json::Value &root, DT_ITEM_DATA_LIST_CLI2 &data);
INT32 EncodeDT_TASK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TASK_ENCOURAGE_DATA( DT_TASK_ENCOURAGE_DATA &data );
BOOL FromJsonDT_TASK_ENCOURAGE_DATA(const string &strData, DT_TASK_ENCOURAGE_DATA &data);
BOOL FromJsonDT_TASK_ENCOURAGE_DATA(Json::Value &root, DT_TASK_ENCOURAGE_DATA &data);
INT32 EncodeDT_TASK_BASE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_BASE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TASK_BASE_DATA( DT_TASK_BASE_DATA &data );
BOOL FromJsonDT_TASK_BASE_DATA(const string &strData, DT_TASK_BASE_DATA &data);
BOOL FromJsonDT_TASK_BASE_DATA(Json::Value &root, DT_TASK_BASE_DATA &data);
INT32 EncodeDT_GIVEN_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GIVEN_TASK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GIVEN_TASK_DATA( DT_GIVEN_TASK_DATA &data );
BOOL FromJsonDT_GIVEN_TASK_DATA(const string &strData, DT_GIVEN_TASK_DATA &data);
BOOL FromJsonDT_GIVEN_TASK_DATA(Json::Value &root, DT_GIVEN_TASK_DATA &data);
INT32 EncodeDT_ACCEPT_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCEPT_TASK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ACCEPT_TASK_DATA( DT_ACCEPT_TASK_DATA &data );
BOOL FromJsonDT_ACCEPT_TASK_DATA(const string &strData, DT_ACCEPT_TASK_DATA &data);
BOOL FromJsonDT_ACCEPT_TASK_DATA(Json::Value &root, DT_ACCEPT_TASK_DATA &data);
INT32 EncodeDT_ENCOURAGE_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENCOURAGE_TASK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ENCOURAGE_TASK_DATA( DT_ENCOURAGE_TASK_DATA &data );
BOOL FromJsonDT_ENCOURAGE_TASK_DATA(const string &strData, DT_ENCOURAGE_TASK_DATA &data);
BOOL FromJsonDT_ENCOURAGE_TASK_DATA(Json::Value &root, DT_ENCOURAGE_TASK_DATA &data);
INT32 EncodeDT_INVALID_TASK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INVALID_TASK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_INVALID_TASK_DATA( DT_INVALID_TASK_DATA &data );
BOOL FromJsonDT_INVALID_TASK_DATA(const string &strData, DT_INVALID_TASK_DATA &data);
BOOL FromJsonDT_INVALID_TASK_DATA(Json::Value &root, DT_INVALID_TASK_DATA &data);
INT32 EncodeUnDT_TASK_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 DecodeUnDT_TASK_UNION(void *pHost, INT32 iSelect, CNetData* poNetData);
INT32 EncodeDT_TASK_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TASK_DATA_CLI( DT_TASK_DATA_CLI &data );
BOOL FromJsonDT_TASK_DATA_CLI(const string &strData, DT_TASK_DATA_CLI &data);
BOOL FromJsonDT_TASK_DATA_CLI(Json::Value &root, DT_TASK_DATA_CLI &data);
INT32 EncodeDT_TASK_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_DATA_LIST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TASK_DATA_LIST_CLI( DT_TASK_DATA_LIST_CLI &data );
BOOL FromJsonDT_TASK_DATA_LIST_CLI(const string &strData, DT_TASK_DATA_LIST_CLI &data);
BOOL FromJsonDT_TASK_DATA_LIST_CLI(Json::Value &root, DT_TASK_DATA_LIST_CLI &data);
INT32 EncodeDT_ACCEPT_TASK_COND_RECORD(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ACCEPT_TASK_COND_RECORD(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ACCEPT_TASK_COND_RECORD( DT_ACCEPT_TASK_COND_RECORD &data );
BOOL FromJsonDT_ACCEPT_TASK_COND_RECORD(const string &strData, DT_ACCEPT_TASK_COND_RECORD &data);
BOOL FromJsonDT_ACCEPT_TASK_COND_RECORD(Json::Value &root, DT_ACCEPT_TASK_COND_RECORD &data);
INT32 EncodeDT_TASK_COND_RECORD_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TASK_COND_RECORD_LIST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TASK_COND_RECORD_LIST( DT_TASK_COND_RECORD_LIST &data );
BOOL FromJsonDT_TASK_COND_RECORD_LIST(const string &strData, DT_TASK_COND_RECORD_LIST &data);
BOOL FromJsonDT_TASK_COND_RECORD_LIST(Json::Value &root, DT_TASK_COND_RECORD_LIST &data);
INT32 EncodeDT_PHYSTRENGTH_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PHYSTRENGTH_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PHYSTRENGTH_DATA_CLI( DT_PHYSTRENGTH_DATA_CLI &data );
BOOL FromJsonDT_PHYSTRENGTH_DATA_CLI(const string &strData, DT_PHYSTRENGTH_DATA_CLI &data);
BOOL FromJsonDT_PHYSTRENGTH_DATA_CLI(Json::Value &root, DT_PHYSTRENGTH_DATA_CLI &data);
INT32 EncodeDT_EXPERIENCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXPERIENCE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EXPERIENCE_DATA( DT_EXPERIENCE_DATA &data );
BOOL FromJsonDT_EXPERIENCE_DATA(const string &strData, DT_EXPERIENCE_DATA &data);
BOOL FromJsonDT_EXPERIENCE_DATA(Json::Value &root, DT_EXPERIENCE_DATA &data);
INT32 EncodeDT_PLAYER_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_BASE_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLAYER_BASE_DATA_CLI( DT_PLAYER_BASE_DATA_CLI &data );
BOOL FromJsonDT_PLAYER_BASE_DATA_CLI(const string &strData, DT_PLAYER_BASE_DATA_CLI &data);
BOOL FromJsonDT_PLAYER_BASE_DATA_CLI(Json::Value &root, DT_PLAYER_BASE_DATA_CLI &data);
INT32 EncodeDT_BUILD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUILD_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BUILD_DATA_CLI( DT_BUILD_DATA_CLI &data );
BOOL FromJsonDT_BUILD_DATA_CLI(const string &strData, DT_BUILD_DATA_CLI &data);
BOOL FromJsonDT_BUILD_DATA_CLI(Json::Value &root, DT_BUILD_DATA_CLI &data);
INT32 EncodeDT_ONLINE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ONLINE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ONLINE_ENCOURAGE_DATA( DT_ONLINE_ENCOURAGE_DATA &data );
BOOL FromJsonDT_ONLINE_ENCOURAGE_DATA(const string &strData, DT_ONLINE_ENCOURAGE_DATA &data);
BOOL FromJsonDT_ONLINE_ENCOURAGE_DATA(Json::Value &root, DT_ONLINE_ENCOURAGE_DATA &data);
INT32 EncodeDT_DATA_AFTER_ONLINE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_ONLINE_ENCOURAGE(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DATA_AFTER_ONLINE_ENCOURAGE( DT_DATA_AFTER_ONLINE_ENCOURAGE &data );
BOOL FromJsonDT_DATA_AFTER_ONLINE_ENCOURAGE(const string &strData, DT_DATA_AFTER_ONLINE_ENCOURAGE &data);
BOOL FromJsonDT_DATA_AFTER_ONLINE_ENCOURAGE(Json::Value &root, DT_DATA_AFTER_ONLINE_ENCOURAGE &data);
INT32 EncodePKT_CLIGS_ENTERGS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTERGS_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ENTERGS_ACK( PKT_CLIGS_ENTERGS_ACK &data );
BOOL FromJsonPKT_CLIGS_ENTERGS_ACK(const string &strData, PKT_CLIGS_ENTERGS_ACK &data);
BOOL FromJsonPKT_CLIGS_ENTERGS_ACK(Json::Value &root, PKT_CLIGS_ENTERGS_ACK &data);
INT32 EncodeDT_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BAG_DATA_CLI( DT_BAG_DATA_CLI &data );
BOOL FromJsonDT_BAG_DATA_CLI(const string &strData, DT_BAG_DATA_CLI &data);
BOOL FromJsonDT_BAG_DATA_CLI(Json::Value &root, DT_BAG_DATA_CLI &data);
INT32 EncodeDT_EQUIP_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_BAG_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_BAG_DATA_CLI( DT_EQUIP_BAG_DATA_CLI &data );
BOOL FromJsonDT_EQUIP_BAG_DATA_CLI(const string &strData, DT_EQUIP_BAG_DATA_CLI &data);
BOOL FromJsonDT_EQUIP_BAG_DATA_CLI(Json::Value &root, DT_EQUIP_BAG_DATA_CLI &data);
INT32 EncodePKT_CLIGS_GET_HERO_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_INFO_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_HERO_INFO_REQ( PKT_CLIGS_GET_HERO_INFO_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_HERO_INFO_REQ(const string &strData, PKT_CLIGS_GET_HERO_INFO_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_HERO_INFO_REQ(Json::Value &root, PKT_CLIGS_GET_HERO_INFO_REQ &data);
INT32 EncodeDT_ATTR_ENABLE_UPDATE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ATTR_ENABLE_UPDATE_INFO(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ATTR_ENABLE_UPDATE_INFO( DT_ATTR_ENABLE_UPDATE_INFO &data );
BOOL FromJsonDT_ATTR_ENABLE_UPDATE_INFO(const string &strData, DT_ATTR_ENABLE_UPDATE_INFO &data);
BOOL FromJsonDT_ATTR_ENABLE_UPDATE_INFO(Json::Value &root, DT_ATTR_ENABLE_UPDATE_INFO &data);
INT32 EncodeDT_HERO_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_DATA_CLI( DT_HERO_DATA_CLI &data );
BOOL FromJsonDT_HERO_DATA_CLI(const string &strData, DT_HERO_DATA_CLI &data);
BOOL FromJsonDT_HERO_DATA_CLI(Json::Value &root, DT_HERO_DATA_CLI &data);
INT32 EncodeDT_PLAYER_BATTLE_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLAYER_BATTLE_INFO(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLAYER_BATTLE_INFO( DT_PLAYER_BATTLE_INFO &data );
BOOL FromJsonDT_PLAYER_BATTLE_INFO(const string &strData, DT_PLAYER_BATTLE_INFO &data);
BOOL FromJsonDT_PLAYER_BATTLE_INFO(Json::Value &root, DT_PLAYER_BATTLE_INFO &data);
INT32 EncodePKT_CLIGS_GET_HERO_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_INFO_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_HERO_INFO_ACK( PKT_CLIGS_GET_HERO_INFO_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_HERO_INFO_ACK(const string &strData, PKT_CLIGS_GET_HERO_INFO_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_HERO_INFO_ACK(Json::Value &root, PKT_CLIGS_GET_HERO_INFO_ACK &data);
INT32 EncodeDT_FUNC_COMMON_PROMPT(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FUNC_COMMON_PROMPT(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FUNC_COMMON_PROMPT( DT_FUNC_COMMON_PROMPT &data );
BOOL FromJsonDT_FUNC_COMMON_PROMPT(const string &strData, DT_FUNC_COMMON_PROMPT &data);
BOOL FromJsonDT_FUNC_COMMON_PROMPT(Json::Value &root, DT_FUNC_COMMON_PROMPT &data);
INT32 EncodeDT_FUNC_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FUNC_PROMPT_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_FUNC_PROMPT_DATA( DT_FUNC_PROMPT_DATA &data );
BOOL FromJsonDT_FUNC_PROMPT_DATA(const string &strData, DT_FUNC_PROMPT_DATA &data);
BOOL FromJsonDT_FUNC_PROMPT_DATA(Json::Value &root, DT_FUNC_PROMPT_DATA &data);
INT32 EncodeDT_OTHER_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_OTHER_PROMPT_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_OTHER_PROMPT_DATA( DT_OTHER_PROMPT_DATA &data );
BOOL FromJsonDT_OTHER_PROMPT_DATA(const string &strData, DT_OTHER_PROMPT_DATA &data);
BOOL FromJsonDT_OTHER_PROMPT_DATA(Json::Value &root, DT_OTHER_PROMPT_DATA &data);
INT32 EncodeDT_NEW_EQUIP_PROMPT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEW_EQUIP_PROMPT_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_NEW_EQUIP_PROMPT_DATA( DT_NEW_EQUIP_PROMPT_DATA &data );
BOOL FromJsonDT_NEW_EQUIP_PROMPT_DATA(const string &strData, DT_NEW_EQUIP_PROMPT_DATA &data);
BOOL FromJsonDT_NEW_EQUIP_PROMPT_DATA(Json::Value &root, DT_NEW_EQUIP_PROMPT_DATA &data);
INT32 EncodePKT_CLIGS_ENTER_HOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_HOME_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ENTER_HOME_REQ( PKT_CLIGS_ENTER_HOME_REQ &data );
BOOL FromJsonPKT_CLIGS_ENTER_HOME_REQ(const string &strData, PKT_CLIGS_ENTER_HOME_REQ &data);
BOOL FromJsonPKT_CLIGS_ENTER_HOME_REQ(Json::Value &root, PKT_CLIGS_ENTER_HOME_REQ &data);
INT32 EncodePKT_CLIGS_ENTER_HOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_HOME_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ENTER_HOME_ACK( PKT_CLIGS_ENTER_HOME_ACK &data );
BOOL FromJsonPKT_CLIGS_ENTER_HOME_ACK(const string &strData, PKT_CLIGS_ENTER_HOME_ACK &data);
BOOL FromJsonPKT_CLIGS_ENTER_HOME_ACK(Json::Value &root, PKT_CLIGS_ENTER_HOME_ACK &data);
INT32 EncodeDT_BATTLE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BATTLE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BATTLE_ENCOURAGE_DATA( DT_BATTLE_ENCOURAGE_DATA &data );
BOOL FromJsonDT_BATTLE_ENCOURAGE_DATA(const string &strData, DT_BATTLE_ENCOURAGE_DATA &data);
BOOL FromJsonDT_BATTLE_ENCOURAGE_DATA(Json::Value &root, DT_BATTLE_ENCOURAGE_DATA &data);
INT32 EncodeDT_DATA_AFTER_BATTLE_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_BATTLE_ENCOURAGE(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DATA_AFTER_BATTLE_ENCOURAGE( DT_DATA_AFTER_BATTLE_ENCOURAGE &data );
BOOL FromJsonDT_DATA_AFTER_BATTLE_ENCOURAGE(const string &strData, DT_DATA_AFTER_BATTLE_ENCOURAGE &data);
BOOL FromJsonDT_DATA_AFTER_BATTLE_ENCOURAGE(Json::Value &root, DT_DATA_AFTER_BATTLE_ENCOURAGE &data);
INT32 EncodeDT_RACE_CHALLENGE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_CHALLENGE_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RACE_CHALLENGE_ENCOURAGE_DATA( DT_RACE_CHALLENGE_ENCOURAGE_DATA &data );
BOOL FromJsonDT_RACE_CHALLENGE_ENCOURAGE_DATA(const string &strData, DT_RACE_CHALLENGE_ENCOURAGE_DATA &data);
BOOL FromJsonDT_RACE_CHALLENGE_ENCOURAGE_DATA(Json::Value &root, DT_RACE_CHALLENGE_ENCOURAGE_DATA &data);
INT32 EncodeDT_RACE_RANK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_RANK_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RACE_RANK_ENCOURAGE_DATA( DT_RACE_RANK_ENCOURAGE_DATA &data );
BOOL FromJsonDT_RACE_RANK_ENCOURAGE_DATA(const string &strData, DT_RACE_RANK_ENCOURAGE_DATA &data);
BOOL FromJsonDT_RACE_RANK_ENCOURAGE_DATA(Json::Value &root, DT_RACE_RANK_ENCOURAGE_DATA &data);
INT32 EncodePKT_CLIGS_ENTER_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_INSTANCE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ENTER_INSTANCE_REQ( PKT_CLIGS_ENTER_INSTANCE_REQ &data );
BOOL FromJsonPKT_CLIGS_ENTER_INSTANCE_REQ(const string &strData, PKT_CLIGS_ENTER_INSTANCE_REQ &data);
BOOL FromJsonPKT_CLIGS_ENTER_INSTANCE_REQ(Json::Value &root, PKT_CLIGS_ENTER_INSTANCE_REQ &data);
INT32 EncodeDT_PASS_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PASS_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PASS_ENCOURAGE_DATA( DT_PASS_ENCOURAGE_DATA &data );
BOOL FromJsonDT_PASS_ENCOURAGE_DATA(const string &strData, DT_PASS_ENCOURAGE_DATA &data);
BOOL FromJsonDT_PASS_ENCOURAGE_DATA(Json::Value &root, DT_PASS_ENCOURAGE_DATA &data);
INT32 EncodeDT_DATA_AFTER_PASS_ENCOURAGE(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_PASS_ENCOURAGE(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DATA_AFTER_PASS_ENCOURAGE( DT_DATA_AFTER_PASS_ENCOURAGE &data );
BOOL FromJsonDT_DATA_AFTER_PASS_ENCOURAGE(const string &strData, DT_DATA_AFTER_PASS_ENCOURAGE &data);
BOOL FromJsonDT_DATA_AFTER_PASS_ENCOURAGE(Json::Value &root, DT_DATA_AFTER_PASS_ENCOURAGE &data);
INT32 EncodeDT_PASS_ENCOURAGE_BOX_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PASS_ENCOURAGE_BOX_GROUP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PASS_ENCOURAGE_BOX_GROUP_DATA( DT_PASS_ENCOURAGE_BOX_GROUP_DATA &data );
BOOL FromJsonDT_PASS_ENCOURAGE_BOX_GROUP_DATA(const string &strData, DT_PASS_ENCOURAGE_BOX_GROUP_DATA &data);
BOOL FromJsonDT_PASS_ENCOURAGE_BOX_GROUP_DATA(Json::Value &root, DT_PASS_ENCOURAGE_BOX_GROUP_DATA &data);
INT32 EncodeDT_PASS_ENCOURAGE_BOX_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PASS_ENCOURAGE_BOX_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PASS_ENCOURAGE_BOX_DATA( DT_PASS_ENCOURAGE_BOX_DATA &data );
BOOL FromJsonDT_PASS_ENCOURAGE_BOX_DATA(const string &strData, DT_PASS_ENCOURAGE_BOX_DATA &data);
BOOL FromJsonDT_PASS_ENCOURAGE_BOX_DATA(Json::Value &root, DT_PASS_ENCOURAGE_BOX_DATA &data);
INT32 EncodePKT_CLIGS_ENTER_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ENTER_INSTANCE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ENTER_INSTANCE_ACK( PKT_CLIGS_ENTER_INSTANCE_ACK &data );
BOOL FromJsonPKT_CLIGS_ENTER_INSTANCE_ACK(const string &strData, PKT_CLIGS_ENTER_INSTANCE_ACK &data);
BOOL FromJsonPKT_CLIGS_ENTER_INSTANCE_ACK(Json::Value &root, PKT_CLIGS_ENTER_INSTANCE_ACK &data);
INT32 EncodePKT_CLIGS_BATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BATTLE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BATTLE_REQ( PKT_CLIGS_BATTLE_REQ &data );
BOOL FromJsonPKT_CLIGS_BATTLE_REQ(const string &strData, PKT_CLIGS_BATTLE_REQ &data);
BOOL FromJsonPKT_CLIGS_BATTLE_REQ(Json::Value &root, PKT_CLIGS_BATTLE_REQ &data);
INT32 EncodeDT_BURST_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BURST_ITEM_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BURST_ITEM_DATA( DT_BURST_ITEM_DATA &data );
BOOL FromJsonDT_BURST_ITEM_DATA(const string &strData, DT_BURST_ITEM_DATA &data);
BOOL FromJsonDT_BURST_ITEM_DATA(Json::Value &root, DT_BURST_ITEM_DATA &data);
INT32 EncodeDT_NEXT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_NEXT_INSTANCE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_NEXT_INSTANCE_DATA( DT_NEXT_INSTANCE_DATA &data );
BOOL FromJsonDT_NEXT_INSTANCE_DATA(const string &strData, DT_NEXT_INSTANCE_DATA &data);
BOOL FromJsonDT_NEXT_INSTANCE_DATA(Json::Value &root, DT_NEXT_INSTANCE_DATA &data);
INT32 EncodeDT_UNLOCK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UNLOCK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UNLOCK_DATA( DT_UNLOCK_DATA &data );
BOOL FromJsonDT_UNLOCK_DATA(const string &strData, DT_UNLOCK_DATA &data);
BOOL FromJsonDT_UNLOCK_DATA(Json::Value &root, DT_UNLOCK_DATA &data);
INT32 EncodePKT_CLIGS_BATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BATTLE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BATTLE_ACK( PKT_CLIGS_BATTLE_ACK &data );
BOOL FromJsonPKT_CLIGS_BATTLE_ACK(const string &strData, PKT_CLIGS_BATTLE_ACK &data);
BOOL FromJsonPKT_CLIGS_BATTLE_ACK(Json::Value &root, PKT_CLIGS_BATTLE_ACK &data);
INT32 EncodePKT_CLIGS_EXIT_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXIT_INSTANCE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EXIT_INSTANCE_REQ( PKT_CLIGS_EXIT_INSTANCE_REQ &data );
BOOL FromJsonPKT_CLIGS_EXIT_INSTANCE_REQ(const string &strData, PKT_CLIGS_EXIT_INSTANCE_REQ &data);
BOOL FromJsonPKT_CLIGS_EXIT_INSTANCE_REQ(Json::Value &root, PKT_CLIGS_EXIT_INSTANCE_REQ &data);
INT32 EncodePKT_CLIGS_EXIT_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXIT_INSTANCE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EXIT_INSTANCE_ACK( PKT_CLIGS_EXIT_INSTANCE_ACK &data );
BOOL FromJsonPKT_CLIGS_EXIT_INSTANCE_ACK(const string &strData, PKT_CLIGS_EXIT_INSTANCE_ACK &data);
BOOL FromJsonPKT_CLIGS_EXIT_INSTANCE_ACK(Json::Value &root, PKT_CLIGS_EXIT_INSTANCE_ACK &data);
INT32 EncodeDT_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_INSTANCE_RECORD_DATA_CLI( DT_INSTANCE_RECORD_DATA_CLI &data );
BOOL FromJsonDT_INSTANCE_RECORD_DATA_CLI(const string &strData, DT_INSTANCE_RECORD_DATA_CLI &data);
BOOL FromJsonDT_INSTANCE_RECORD_DATA_CLI(Json::Value &root, DT_INSTANCE_RECORD_DATA_CLI &data);
INT32 EncodeDT_TOWN_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TOWN_INSTANCE_RECORD_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TOWN_INSTANCE_RECORD_DATA_CLI( DT_TOWN_INSTANCE_RECORD_DATA_CLI &data );
BOOL FromJsonDT_TOWN_INSTANCE_RECORD_DATA_CLI(const string &strData, DT_TOWN_INSTANCE_RECORD_DATA_CLI &data);
BOOL FromJsonDT_TOWN_INSTANCE_RECORD_DATA_CLI(Json::Value &root, DT_TOWN_INSTANCE_RECORD_DATA_CLI &data);
INT32 EncodePKT_CLIGS_GET_SCENE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_SCENE_INSTANCE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_SCENE_INSTANCE_REQ( PKT_CLIGS_GET_SCENE_INSTANCE_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_SCENE_INSTANCE_REQ(const string &strData, PKT_CLIGS_GET_SCENE_INSTANCE_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_SCENE_INSTANCE_REQ(Json::Value &root, PKT_CLIGS_GET_SCENE_INSTANCE_REQ &data);
INT32 EncodePKT_CLIGS_GET_SCENE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_SCENE_INSTANCE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_SCENE_INSTANCE_ACK( PKT_CLIGS_GET_SCENE_INSTANCE_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_SCENE_INSTANCE_ACK(const string &strData, PKT_CLIGS_GET_SCENE_INSTANCE_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_SCENE_INSTANCE_ACK(Json::Value &root, PKT_CLIGS_GET_SCENE_INSTANCE_ACK &data);
INT32 EncodePKT_CLIGS_EQUIP_STRENGTHEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_STRENGTHEN_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EQUIP_STRENGTHEN_REQ( PKT_CLIGS_EQUIP_STRENGTHEN_REQ &data );
BOOL FromJsonPKT_CLIGS_EQUIP_STRENGTHEN_REQ(const string &strData, PKT_CLIGS_EQUIP_STRENGTHEN_REQ &data);
BOOL FromJsonPKT_CLIGS_EQUIP_STRENGTHEN_REQ(Json::Value &root, PKT_CLIGS_EQUIP_STRENGTHEN_REQ &data);
INT32 EncodePKT_CLIGS_EQUIP_STRENGTHEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_STRENGTHEN_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EQUIP_STRENGTHEN_ACK( PKT_CLIGS_EQUIP_STRENGTHEN_ACK &data );
BOOL FromJsonPKT_CLIGS_EQUIP_STRENGTHEN_ACK(const string &strData, PKT_CLIGS_EQUIP_STRENGTHEN_ACK &data);
BOOL FromJsonPKT_CLIGS_EQUIP_STRENGTHEN_ACK(Json::Value &root, PKT_CLIGS_EQUIP_STRENGTHEN_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_SMITHY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SMITHY_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SMITHY_REQ( PKT_CLIGS_OPEN_SMITHY_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_SMITHY_REQ(const string &strData, PKT_CLIGS_OPEN_SMITHY_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_SMITHY_REQ(Json::Value &root, PKT_CLIGS_OPEN_SMITHY_REQ &data);
INT32 EncodeDT_BUY_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BUY_EQUIP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BUY_EQUIP_DATA( DT_BUY_EQUIP_DATA &data );
BOOL FromJsonDT_BUY_EQUIP_DATA(const string &strData, DT_BUY_EQUIP_DATA &data);
BOOL FromJsonDT_BUY_EQUIP_DATA(Json::Value &root, DT_BUY_EQUIP_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_SMITHY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SMITHY_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SMITHY_ACK( PKT_CLIGS_OPEN_SMITHY_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_SMITHY_ACK(const string &strData, PKT_CLIGS_OPEN_SMITHY_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_SMITHY_ACK(Json::Value &root, PKT_CLIGS_OPEN_SMITHY_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_RECRUIT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RECRUIT_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_RECRUIT_REQ( PKT_CLIGS_OPEN_RECRUIT_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_RECRUIT_REQ(const string &strData, PKT_CLIGS_OPEN_RECRUIT_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_RECRUIT_REQ(Json::Value &root, PKT_CLIGS_OPEN_RECRUIT_REQ &data);
INT32 EncodeDT_HERO_ATTR_ADD_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_ATTR_ADD_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_ATTR_ADD_CLI( DT_HERO_ATTR_ADD_CLI &data );
BOOL FromJsonDT_HERO_ATTR_ADD_CLI(const string &strData, DT_HERO_ATTR_ADD_CLI &data);
BOOL FromJsonDT_HERO_ATTR_ADD_CLI(Json::Value &root, DT_HERO_ATTR_ADD_CLI &data);
INT32 EncodeDT_RECRUIT_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RECRUIT_HERO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RECRUIT_HERO_DATA( DT_RECRUIT_HERO_DATA &data );
BOOL FromJsonDT_RECRUIT_HERO_DATA(const string &strData, DT_RECRUIT_HERO_DATA &data);
BOOL FromJsonDT_RECRUIT_HERO_DATA(Json::Value &root, DT_RECRUIT_HERO_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_RECRUIT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RECRUIT_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_RECRUIT_ACK( PKT_CLIGS_OPEN_RECRUIT_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_RECRUIT_ACK(const string &strData, PKT_CLIGS_OPEN_RECRUIT_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_RECRUIT_ACK(Json::Value &root, PKT_CLIGS_OPEN_RECRUIT_ACK &data);
INT32 EncodePKT_CLIGS_RECRUIT_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECRUIT_HERO_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECRUIT_HERO_REQ( PKT_CLIGS_RECRUIT_HERO_REQ &data );
BOOL FromJsonPKT_CLIGS_RECRUIT_HERO_REQ(const string &strData, PKT_CLIGS_RECRUIT_HERO_REQ &data);
BOOL FromJsonPKT_CLIGS_RECRUIT_HERO_REQ(Json::Value &root, PKT_CLIGS_RECRUIT_HERO_REQ &data);
INT32 EncodePKT_CLIGS_RECRUIT_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECRUIT_HERO_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECRUIT_HERO_ACK( PKT_CLIGS_RECRUIT_HERO_ACK &data );
BOOL FromJsonPKT_CLIGS_RECRUIT_HERO_ACK(const string &strData, PKT_CLIGS_RECRUIT_HERO_ACK &data);
BOOL FromJsonPKT_CLIGS_RECRUIT_HERO_ACK(Json::Value &root, PKT_CLIGS_RECRUIT_HERO_ACK &data);
INT32 EncodePKT_CLIGS_VISIT_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VISIT_HERO_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_VISIT_HERO_REQ( PKT_CLIGS_VISIT_HERO_REQ &data );
BOOL FromJsonPKT_CLIGS_VISIT_HERO_REQ(const string &strData, PKT_CLIGS_VISIT_HERO_REQ &data);
BOOL FromJsonPKT_CLIGS_VISIT_HERO_REQ(Json::Value &root, PKT_CLIGS_VISIT_HERO_REQ &data);
INT32 EncodePKT_CLIGS_VISIT_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_VISIT_HERO_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_VISIT_HERO_ACK( PKT_CLIGS_VISIT_HERO_ACK &data );
BOOL FromJsonPKT_CLIGS_VISIT_HERO_ACK(const string &strData, PKT_CLIGS_VISIT_HERO_ACK &data);
BOOL FromJsonPKT_CLIGS_VISIT_HERO_ACK(Json::Value &root, PKT_CLIGS_VISIT_HERO_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAG_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_BAG_REQ( PKT_CLIGS_OPEN_BAG_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_BAG_REQ(const string &strData, PKT_CLIGS_OPEN_BAG_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_BAG_REQ(Json::Value &root, PKT_CLIGS_OPEN_BAG_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_BAG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_BAG_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_BAG_ACK( PKT_CLIGS_OPEN_BAG_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_BAG_ACK(const string &strData, PKT_CLIGS_OPEN_BAG_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_BAG_ACK(Json::Value &root, PKT_CLIGS_OPEN_BAG_ACK &data);
INT32 EncodePKT_CLIGS_ACCEPT_TASK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACCEPT_TASK_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ACCEPT_TASK_REQ( PKT_CLIGS_ACCEPT_TASK_REQ &data );
BOOL FromJsonPKT_CLIGS_ACCEPT_TASK_REQ(const string &strData, PKT_CLIGS_ACCEPT_TASK_REQ &data);
BOOL FromJsonPKT_CLIGS_ACCEPT_TASK_REQ(Json::Value &root, PKT_CLIGS_ACCEPT_TASK_REQ &data);
INT32 EncodeDT_INSTANCE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_INSTANCE_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_INSTANCE_DATA_CLI( DT_INSTANCE_DATA_CLI &data );
BOOL FromJsonDT_INSTANCE_DATA_CLI(const string &strData, DT_INSTANCE_DATA_CLI &data);
BOOL FromJsonDT_INSTANCE_DATA_CLI(Json::Value &root, DT_INSTANCE_DATA_CLI &data);
INT32 EncodePKT_CLIGS_ACCEPT_TASK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ACCEPT_TASK_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ACCEPT_TASK_ACK( PKT_CLIGS_ACCEPT_TASK_ACK &data );
BOOL FromJsonPKT_CLIGS_ACCEPT_TASK_ACK(const string &strData, PKT_CLIGS_ACCEPT_TASK_ACK &data);
BOOL FromJsonPKT_CLIGS_ACCEPT_TASK_ACK(Json::Value &root, PKT_CLIGS_ACCEPT_TASK_ACK &data);
INT32 EncodePKT_CLIGS_GIVEUP_TASK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GIVEUP_TASK_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GIVEUP_TASK_REQ( PKT_CLIGS_GIVEUP_TASK_REQ &data );
BOOL FromJsonPKT_CLIGS_GIVEUP_TASK_REQ(const string &strData, PKT_CLIGS_GIVEUP_TASK_REQ &data);
BOOL FromJsonPKT_CLIGS_GIVEUP_TASK_REQ(Json::Value &root, PKT_CLIGS_GIVEUP_TASK_REQ &data);
INT32 EncodePKT_CLIGS_GIVEUP_TASK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GIVEUP_TASK_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GIVEUP_TASK_ACK( PKT_CLIGS_GIVEUP_TASK_ACK &data );
BOOL FromJsonPKT_CLIGS_GIVEUP_TASK_ACK(const string &strData, PKT_CLIGS_GIVEUP_TASK_ACK &data);
BOOL FromJsonPKT_CLIGS_GIVEUP_TASK_ACK(Json::Value &root, PKT_CLIGS_GIVEUP_TASK_ACK &data);
INT32 EncodePKT_CLIGS_RECV_TASK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_TASK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_TASK_ENCOURAGE_REQ( PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ &data );
BOOL FromJsonPKT_CLIGS_RECV_TASK_ENCOURAGE_REQ(const string &strData, PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ &data);
BOOL FromJsonPKT_CLIGS_RECV_TASK_ENCOURAGE_REQ(Json::Value &root, PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ &data);
INT32 EncodePKT_CLIGS_RECV_TASK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_TASK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_TASK_ENCOURAGE_ACK( PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK &data );
BOOL FromJsonPKT_CLIGS_RECV_TASK_ENCOURAGE_ACK(const string &strData, PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK &data);
BOOL FromJsonPKT_CLIGS_RECV_TASK_ENCOURAGE_ACK(Json::Value &root, PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_RACE_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RACE_BUILD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_RACE_BUILD_REQ( PKT_CLIGS_OPEN_RACE_BUILD_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_RACE_BUILD_REQ(const string &strData, PKT_CLIGS_OPEN_RACE_BUILD_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_RACE_BUILD_REQ(Json::Value &root, PKT_CLIGS_OPEN_RACE_BUILD_REQ &data);
INT32 EncodeDT_CHALLENGE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CHALLENGE_PLAYER_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CHALLENGE_PLAYER_DATA( DT_CHALLENGE_PLAYER_DATA &data );
BOOL FromJsonDT_CHALLENGE_PLAYER_DATA(const string &strData, DT_CHALLENGE_PLAYER_DATA &data);
BOOL FromJsonDT_CHALLENGE_PLAYER_DATA(Json::Value &root, DT_CHALLENGE_PLAYER_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_RACE_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_RACE_BUILD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_RACE_BUILD_ACK( PKT_CLIGS_OPEN_RACE_BUILD_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_RACE_BUILD_ACK(const string &strData, PKT_CLIGS_OPEN_RACE_BUILD_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_RACE_BUILD_ACK(Json::Value &root, PKT_CLIGS_OPEN_RACE_BUILD_ACK &data);
INT32 EncodeDT_ENEMY_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_EQUIP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ENEMY_EQUIP_DATA( DT_ENEMY_EQUIP_DATA &data );
BOOL FromJsonDT_ENEMY_EQUIP_DATA(const string &strData, DT_ENEMY_EQUIP_DATA &data);
BOOL FromJsonDT_ENEMY_EQUIP_DATA(Json::Value &root, DT_ENEMY_EQUIP_DATA &data);
INT32 EncodeDT_ENEMY_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_EQUIP_DATA_LST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ENEMY_EQUIP_DATA_LST( DT_ENEMY_EQUIP_DATA_LST &data );
BOOL FromJsonDT_ENEMY_EQUIP_DATA_LST(const string &strData, DT_ENEMY_EQUIP_DATA_LST &data);
BOOL FromJsonDT_ENEMY_EQUIP_DATA_LST(Json::Value &root, DT_ENEMY_EQUIP_DATA_LST &data);
INT32 EncodeDT_ENEMY_HERO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_HERO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ENEMY_HERO_DATA( DT_ENEMY_HERO_DATA &data );
BOOL FromJsonDT_ENEMY_HERO_DATA(const string &strData, DT_ENEMY_HERO_DATA &data);
BOOL FromJsonDT_ENEMY_HERO_DATA(Json::Value &root, DT_ENEMY_HERO_DATA &data);
INT32 EncodePKT_CLIGS_CHALLENGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CHALLENGE_REQ( PKT_CLIGS_CHALLENGE_REQ &data );
BOOL FromJsonPKT_CLIGS_CHALLENGE_REQ(const string &strData, PKT_CLIGS_CHALLENGE_REQ &data);
BOOL FromJsonPKT_CLIGS_CHALLENGE_REQ(Json::Value &root, PKT_CLIGS_CHALLENGE_REQ &data);
INT32 EncodePKT_CLIGS_CHALLENGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHALLENGE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CHALLENGE_ACK( PKT_CLIGS_CHALLENGE_ACK &data );
BOOL FromJsonPKT_CLIGS_CHALLENGE_ACK(const string &strData, PKT_CLIGS_CHALLENGE_ACK &data);
BOOL FromJsonPKT_CLIGS_CHALLENGE_ACK(Json::Value &root, PKT_CLIGS_CHALLENGE_ACK &data);
INT32 EncodePKT_CLIGS_RECV_RANK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RANK_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_RANK_ENCOURAGE_REQ( PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ &data );
BOOL FromJsonPKT_CLIGS_RECV_RANK_ENCOURAGE_REQ(const string &strData, PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ &data);
BOOL FromJsonPKT_CLIGS_RECV_RANK_ENCOURAGE_REQ(Json::Value &root, PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ &data);
INT32 EncodePKT_CLIGS_RECV_RANK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_RANK_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_RANK_ENCOURAGE_ACK( PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK &data );
BOOL FromJsonPKT_CLIGS_RECV_RANK_ENCOURAGE_ACK(const string &strData, PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK &data);
BOOL FromJsonPKT_CLIGS_RECV_RANK_ENCOURAGE_ACK(Json::Value &root, PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK &data);
INT32 EncodePKT_CLIGS_GET_RANK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RANK_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_RANK_REQ( PKT_CLIGS_GET_RANK_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_RANK_REQ(const string &strData, PKT_CLIGS_GET_RANK_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_RANK_REQ(Json::Value &root, PKT_CLIGS_GET_RANK_REQ &data);
INT32 EncodeDT_RANK_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RANK_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RANK_DATA( DT_RANK_DATA &data );
BOOL FromJsonDT_RANK_DATA(const string &strData, DT_RANK_DATA &data);
BOOL FromJsonDT_RANK_DATA(Json::Value &root, DT_RANK_DATA &data);
INT32 EncodePKT_CLIGS_GET_RANK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RANK_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_RANK_ACK( PKT_CLIGS_GET_RANK_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_RANK_ACK(const string &strData, PKT_CLIGS_GET_RANK_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_RANK_ACK(Json::Value &root, PKT_CLIGS_GET_RANK_ACK &data);
INT32 EncodePKT_CLIGS_GET_RACE_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RACE_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_RACE_BATTLELOG_REQ( PKT_CLIGS_GET_RACE_BATTLELOG_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_RACE_BATTLELOG_REQ(const string &strData, PKT_CLIGS_GET_RACE_BATTLELOG_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_RACE_BATTLELOG_REQ(Json::Value &root, PKT_CLIGS_GET_RACE_BATTLELOG_REQ &data);
INT32 EncodeDT_RACE_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_RACE_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_RACE_BATTLELOG_DATA_CLI( DT_RACE_BATTLELOG_DATA_CLI &data );
BOOL FromJsonDT_RACE_BATTLELOG_DATA_CLI(const string &strData, DT_RACE_BATTLELOG_DATA_CLI &data);
BOOL FromJsonDT_RACE_BATTLELOG_DATA_CLI(Json::Value &root, DT_RACE_BATTLELOG_DATA_CLI &data);
INT32 EncodePKT_CLIGS_GET_RACE_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_RACE_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_RACE_BATTLELOG_ACK( PKT_CLIGS_GET_RACE_BATTLELOG_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_RACE_BATTLELOG_ACK(const string &strData, PKT_CLIGS_GET_RACE_BATTLELOG_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_RACE_BATTLELOG_ACK(Json::Value &root, PKT_CLIGS_GET_RACE_BATTLELOG_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_SHOP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHOP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SHOP_REQ( PKT_CLIGS_OPEN_SHOP_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_SHOP_REQ(const string &strData, PKT_CLIGS_OPEN_SHOP_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_SHOP_REQ(Json::Value &root, PKT_CLIGS_OPEN_SHOP_REQ &data);
INT32 EncodeDT_ITEM_DATA_SHOP(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ITEM_DATA_SHOP(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ITEM_DATA_SHOP( DT_ITEM_DATA_SHOP &data );
BOOL FromJsonDT_ITEM_DATA_SHOP(const string &strData, DT_ITEM_DATA_SHOP &data);
BOOL FromJsonDT_ITEM_DATA_SHOP(Json::Value &root, DT_ITEM_DATA_SHOP &data);
INT32 EncodePKT_CLIGS_OPEN_SHOP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SHOP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SHOP_ACK( PKT_CLIGS_OPEN_SHOP_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_SHOP_ACK(const string &strData, PKT_CLIGS_OPEN_SHOP_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_SHOP_ACK(Json::Value &root, PKT_CLIGS_OPEN_SHOP_ACK &data);
INT32 EncodePKT_CLIGS_BUY_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_ITEM_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BUY_ITEM_REQ( PKT_CLIGS_BUY_ITEM_REQ &data );
BOOL FromJsonPKT_CLIGS_BUY_ITEM_REQ(const string &strData, PKT_CLIGS_BUY_ITEM_REQ &data);
BOOL FromJsonPKT_CLIGS_BUY_ITEM_REQ(Json::Value &root, PKT_CLIGS_BUY_ITEM_REQ &data);
INT32 EncodePKT_CLIGS_BUY_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_ITEM_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BUY_ITEM_ACK( PKT_CLIGS_BUY_ITEM_ACK &data );
BOOL FromJsonPKT_CLIGS_BUY_ITEM_ACK(const string &strData, PKT_CLIGS_BUY_ITEM_ACK &data);
BOOL FromJsonPKT_CLIGS_BUY_ITEM_ACK(Json::Value &root, PKT_CLIGS_BUY_ITEM_ACK &data);
INT32 EncodePKT_CLIGS_SELL_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELL_ITEM_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SELL_ITEM_REQ( PKT_CLIGS_SELL_ITEM_REQ &data );
BOOL FromJsonPKT_CLIGS_SELL_ITEM_REQ(const string &strData, PKT_CLIGS_SELL_ITEM_REQ &data);
BOOL FromJsonPKT_CLIGS_SELL_ITEM_REQ(Json::Value &root, PKT_CLIGS_SELL_ITEM_REQ &data);
INT32 EncodePKT_CLIGS_SELL_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SELL_ITEM_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SELL_ITEM_ACK( PKT_CLIGS_SELL_ITEM_ACK &data );
BOOL FromJsonPKT_CLIGS_SELL_ITEM_ACK(const string &strData, PKT_CLIGS_SELL_ITEM_ACK &data);
BOOL FromJsonPKT_CLIGS_SELL_ITEM_ACK(Json::Value &root, PKT_CLIGS_SELL_ITEM_ACK &data);
INT32 EncodePKT_CLIGS_DISBAND_HERO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DISBAND_HERO_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DISBAND_HERO_REQ( PKT_CLIGS_DISBAND_HERO_REQ &data );
BOOL FromJsonPKT_CLIGS_DISBAND_HERO_REQ(const string &strData, PKT_CLIGS_DISBAND_HERO_REQ &data);
BOOL FromJsonPKT_CLIGS_DISBAND_HERO_REQ(Json::Value &root, PKT_CLIGS_DISBAND_HERO_REQ &data);
INT32 EncodePKT_CLIGS_DISBAND_HERO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DISBAND_HERO_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DISBAND_HERO_ACK( PKT_CLIGS_DISBAND_HERO_ACK &data );
BOOL FromJsonPKT_CLIGS_DISBAND_HERO_ACK(const string &strData, PKT_CLIGS_DISBAND_HERO_ACK &data);
BOOL FromJsonPKT_CLIGS_DISBAND_HERO_ACK(Json::Value &root, PKT_CLIGS_DISBAND_HERO_ACK &data);
INT32 EncodePKT_CLIGS_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EQUIP_REQ( PKT_CLIGS_EQUIP_REQ &data );
BOOL FromJsonPKT_CLIGS_EQUIP_REQ(const string &strData, PKT_CLIGS_EQUIP_REQ &data);
BOOL FromJsonPKT_CLIGS_EQUIP_REQ(Json::Value &root, PKT_CLIGS_EQUIP_REQ &data);
INT32 EncodePKT_CLIGS_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EQUIP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EQUIP_ACK( PKT_CLIGS_EQUIP_ACK &data );
BOOL FromJsonPKT_CLIGS_EQUIP_ACK(const string &strData, PKT_CLIGS_EQUIP_ACK &data);
BOOL FromJsonPKT_CLIGS_EQUIP_ACK(Json::Value &root, PKT_CLIGS_EQUIP_ACK &data);
INT32 EncodePKT_CLIGS_REGISTER_USERNAME_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REGISTER_USERNAME_NTF(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_REGISTER_USERNAME_NTF( PKT_CLIGS_REGISTER_USERNAME_NTF &data );
BOOL FromJsonPKT_CLIGS_REGISTER_USERNAME_NTF(const string &strData, PKT_CLIGS_REGISTER_USERNAME_NTF &data);
BOOL FromJsonPKT_CLIGS_REGISTER_USERNAME_NTF(Json::Value &root, PKT_CLIGS_REGISTER_USERNAME_NTF &data);
INT32 EncodePKT_CLIGS_OPEN_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_EMBATTLE_REQ( PKT_CLIGS_OPEN_EMBATTLE_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_EMBATTLE_REQ(const string &strData, PKT_CLIGS_OPEN_EMBATTLE_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_EMBATTLE_REQ(Json::Value &root, PKT_CLIGS_OPEN_EMBATTLE_REQ &data);
INT32 EncodeDT_HERO_EMBATLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_EMBATLE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_EMBATLE_DATA( DT_HERO_EMBATLE_DATA &data );
BOOL FromJsonDT_HERO_EMBATLE_DATA(const string &strData, DT_HERO_EMBATLE_DATA &data);
BOOL FromJsonDT_HERO_EMBATLE_DATA(Json::Value &root, DT_HERO_EMBATLE_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_EMBATTLE_ACK( PKT_CLIGS_OPEN_EMBATTLE_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_EMBATTLE_ACK(const string &strData, PKT_CLIGS_OPEN_EMBATTLE_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_EMBATTLE_ACK(Json::Value &root, PKT_CLIGS_OPEN_EMBATTLE_ACK &data);
INT32 EncodePKT_CLIGS_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EMBATTLE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EMBATTLE_REQ( PKT_CLIGS_EMBATTLE_REQ &data );
BOOL FromJsonPKT_CLIGS_EMBATTLE_REQ(const string &strData, PKT_CLIGS_EMBATTLE_REQ &data);
BOOL FromJsonPKT_CLIGS_EMBATTLE_REQ(Json::Value &root, PKT_CLIGS_EMBATTLE_REQ &data);
INT32 EncodePKT_CLIGS_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EMBATTLE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EMBATTLE_ACK( PKT_CLIGS_EMBATTLE_ACK &data );
BOOL FromJsonPKT_CLIGS_EMBATTLE_ACK(const string &strData, PKT_CLIGS_EMBATTLE_ACK &data);
BOOL FromJsonPKT_CLIGS_EMBATTLE_ACK(Json::Value &root, PKT_CLIGS_EMBATTLE_ACK &data);
INT32 EncodePKT_CLIGS_CLR_PLAYER_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_PLAYER_DATA_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_PLAYER_DATA_REQ( PKT_CLIGS_CLR_PLAYER_DATA_REQ &data );
BOOL FromJsonPKT_CLIGS_CLR_PLAYER_DATA_REQ(const string &strData, PKT_CLIGS_CLR_PLAYER_DATA_REQ &data);
BOOL FromJsonPKT_CLIGS_CLR_PLAYER_DATA_REQ(Json::Value &root, PKT_CLIGS_CLR_PLAYER_DATA_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_ITEM_MAKER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ITEM_MAKER_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_ITEM_MAKER_REQ( PKT_CLIGS_OPEN_ITEM_MAKER_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_ITEM_MAKER_REQ(const string &strData, PKT_CLIGS_OPEN_ITEM_MAKER_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_ITEM_MAKER_REQ(Json::Value &root, PKT_CLIGS_OPEN_ITEM_MAKER_REQ &data);
INT32 EncodeDT_EQUIP_DATA_MAKER_NEED(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EQUIP_DATA_MAKER_NEED(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EQUIP_DATA_MAKER_NEED( DT_EQUIP_DATA_MAKER_NEED &data );
BOOL FromJsonDT_EQUIP_DATA_MAKER_NEED(const string &strData, DT_EQUIP_DATA_MAKER_NEED &data);
BOOL FromJsonDT_EQUIP_DATA_MAKER_NEED(Json::Value &root, DT_EQUIP_DATA_MAKER_NEED &data);
INT32 EncodePKT_CLIGS_OPEN_ITEM_MAKER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ITEM_MAKER_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_ITEM_MAKER_ACK( PKT_CLIGS_OPEN_ITEM_MAKER_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_ITEM_MAKER_ACK(const string &strData, PKT_CLIGS_OPEN_ITEM_MAKER_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_ITEM_MAKER_ACK(Json::Value &root, PKT_CLIGS_OPEN_ITEM_MAKER_ACK &data);
INT32 EncodePKT_CLIGS_MAKE_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKE_ITEM_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_MAKE_ITEM_REQ( PKT_CLIGS_MAKE_ITEM_REQ &data );
BOOL FromJsonPKT_CLIGS_MAKE_ITEM_REQ(const string &strData, PKT_CLIGS_MAKE_ITEM_REQ &data);
BOOL FromJsonPKT_CLIGS_MAKE_ITEM_REQ(Json::Value &root, PKT_CLIGS_MAKE_ITEM_REQ &data);
INT32 EncodePKT_CLIGS_MAKE_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKE_ITEM_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_MAKE_ITEM_ACK( PKT_CLIGS_MAKE_ITEM_ACK &data );
BOOL FromJsonPKT_CLIGS_MAKE_ITEM_ACK(const string &strData, PKT_CLIGS_MAKE_ITEM_ACK &data);
BOOL FromJsonPKT_CLIGS_MAKE_ITEM_ACK(Json::Value &root, PKT_CLIGS_MAKE_ITEM_ACK &data);
INT32 EncodeDT_DRUG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRUG_DATA_CLI( DT_DRUG_DATA_CLI &data );
BOOL FromJsonDT_DRUG_DATA_CLI(const string &strData, DT_DRUG_DATA_CLI &data);
BOOL FromJsonDT_DRUG_DATA_CLI(Json::Value &root, DT_DRUG_DATA_CLI &data);
INT32 EncodeDT_DRUG_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_DATA_LST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRUG_DATA_LST_CLI( DT_DRUG_DATA_LST_CLI &data );
BOOL FromJsonDT_DRUG_DATA_LST_CLI(const string &strData, DT_DRUG_DATA_LST_CLI &data);
BOOL FromJsonDT_DRUG_DATA_LST_CLI(Json::Value &root, DT_DRUG_DATA_LST_CLI &data);
INT32 EncodePKT_CLIGS_GET_HERO_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_DRUG_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_HERO_DRUG_REQ( PKT_CLIGS_GET_HERO_DRUG_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_HERO_DRUG_REQ(const string &strData, PKT_CLIGS_GET_HERO_DRUG_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_HERO_DRUG_REQ(Json::Value &root, PKT_CLIGS_GET_HERO_DRUG_REQ &data);
INT32 EncodeDT_DRUG_ITEM_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_ITEM_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRUG_ITEM_DATA( DT_DRUG_ITEM_DATA &data );
BOOL FromJsonDT_DRUG_ITEM_DATA(const string &strData, DT_DRUG_ITEM_DATA &data);
BOOL FromJsonDT_DRUG_ITEM_DATA(Json::Value &root, DT_DRUG_ITEM_DATA &data);
INT32 EncodeDT_DRUG_ITEM_DATA_LST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DRUG_ITEM_DATA_LST(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DRUG_ITEM_DATA_LST( DT_DRUG_ITEM_DATA_LST &data );
BOOL FromJsonDT_DRUG_ITEM_DATA_LST(const string &strData, DT_DRUG_ITEM_DATA_LST &data);
BOOL FromJsonDT_DRUG_ITEM_DATA_LST(Json::Value &root, DT_DRUG_ITEM_DATA_LST &data);
INT32 EncodePKT_CLIGS_GET_HERO_DRUG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_HERO_DRUG_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_HERO_DRUG_ACK( PKT_CLIGS_GET_HERO_DRUG_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_HERO_DRUG_ACK(const string &strData, PKT_CLIGS_GET_HERO_DRUG_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_HERO_DRUG_ACK(Json::Value &root, PKT_CLIGS_GET_HERO_DRUG_ACK &data);
INT32 EncodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ( PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ &data );
BOOL FromJsonPKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ(const string &strData, PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ &data);
BOOL FromJsonPKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ(Json::Value &root, PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ &data);
INT32 EncodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK( PKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK &data );
BOOL FromJsonPKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK(const string &strData, PKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK &data);
BOOL FromJsonPKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK(Json::Value &root, PKT_CLIGS_RECV_ONLINE_ENCOURAGE_ACK &data);
INT32 EncodeDT_DATA_AFTER_PASS_ENCOURAGE_BOX(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_PASS_ENCOURAGE_BOX(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DATA_AFTER_PASS_ENCOURAGE_BOX( DT_DATA_AFTER_PASS_ENCOURAGE_BOX &data );
BOOL FromJsonDT_DATA_AFTER_PASS_ENCOURAGE_BOX(const string &strData, DT_DATA_AFTER_PASS_ENCOURAGE_BOX &data);
BOOL FromJsonDT_DATA_AFTER_PASS_ENCOURAGE_BOX(Json::Value &root, DT_DATA_AFTER_PASS_ENCOURAGE_BOX &data);
INT32 EncodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ( PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ(const string &strData, PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ(Json::Value &root, PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK( PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK(const string &strData, PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK(Json::Value &root, PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK &data);
INT32 EncodePKT_CLIGS_INCREASE_BAG_SIZE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_INCREASE_BAG_SIZE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_INCREASE_BAG_SIZE_REQ( PKT_CLIGS_INCREASE_BAG_SIZE_REQ &data );
BOOL FromJsonPKT_CLIGS_INCREASE_BAG_SIZE_REQ(const string &strData, PKT_CLIGS_INCREASE_BAG_SIZE_REQ &data);
BOOL FromJsonPKT_CLIGS_INCREASE_BAG_SIZE_REQ(Json::Value &root, PKT_CLIGS_INCREASE_BAG_SIZE_REQ &data);
INT32 EncodePKT_CLIGS_INCREASE_BAG_SIZE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_INCREASE_BAG_SIZE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_INCREASE_BAG_SIZE_ACK( PKT_CLIGS_INCREASE_BAG_SIZE_ACK &data );
BOOL FromJsonPKT_CLIGS_INCREASE_BAG_SIZE_ACK(const string &strData, PKT_CLIGS_INCREASE_BAG_SIZE_ACK &data);
BOOL FromJsonPKT_CLIGS_INCREASE_BAG_SIZE_ACK(Json::Value &root, PKT_CLIGS_INCREASE_BAG_SIZE_ACK &data);
INT32 EncodePKT_CLIGS_ONHOOK_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ONHOOK_REQ( PKT_CLIGS_ONHOOK_REQ &data );
BOOL FromJsonPKT_CLIGS_ONHOOK_REQ(const string &strData, PKT_CLIGS_ONHOOK_REQ &data);
BOOL FromJsonPKT_CLIGS_ONHOOK_REQ(Json::Value &root, PKT_CLIGS_ONHOOK_REQ &data);
INT32 EncodeDT_HERO_UPGRADE_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_UPGRADE_DATA2(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_UPGRADE_DATA2( DT_HERO_UPGRADE_DATA2 &data );
BOOL FromJsonDT_HERO_UPGRADE_DATA2(const string &strData, DT_HERO_UPGRADE_DATA2 &data);
BOOL FromJsonDT_HERO_UPGRADE_DATA2(Json::Value &root, DT_HERO_UPGRADE_DATA2 &data);
INT32 EncodeDT_DATA_AFTER_ONHOOK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_DATA_AFTER_ONHOOK(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_DATA_AFTER_ONHOOK( DT_DATA_AFTER_ONHOOK &data );
BOOL FromJsonDT_DATA_AFTER_ONHOOK(const string &strData, DT_DATA_AFTER_ONHOOK &data);
BOOL FromJsonDT_DATA_AFTER_ONHOOK(Json::Value &root, DT_DATA_AFTER_ONHOOK &data);
INT32 EncodePKT_CLIGS_ONHOOK_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ONHOOK_ACK( PKT_CLIGS_ONHOOK_ACK &data );
BOOL FromJsonPKT_CLIGS_ONHOOK_ACK(const string &strData, PKT_CLIGS_ONHOOK_ACK &data);
BOOL FromJsonPKT_CLIGS_ONHOOK_ACK(Json::Value &root, PKT_CLIGS_ONHOOK_ACK &data);
INT32 EncodePKT_CLIGS_CLR_CHALLENGE_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CHALLENGE_CD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_CHALLENGE_CD_REQ( PKT_CLIGS_CLR_CHALLENGE_CD_REQ &data );
BOOL FromJsonPKT_CLIGS_CLR_CHALLENGE_CD_REQ(const string &strData, PKT_CLIGS_CLR_CHALLENGE_CD_REQ &data);
BOOL FromJsonPKT_CLIGS_CLR_CHALLENGE_CD_REQ(Json::Value &root, PKT_CLIGS_CLR_CHALLENGE_CD_REQ &data);
INT32 EncodePKT_CLIGS_CLR_CHALLENGE_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CHALLENGE_CD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_CHALLENGE_CD_ACK( PKT_CLIGS_CLR_CHALLENGE_CD_ACK &data );
BOOL FromJsonPKT_CLIGS_CLR_CHALLENGE_CD_ACK(const string &strData, PKT_CLIGS_CLR_CHALLENGE_CD_ACK &data);
BOOL FromJsonPKT_CLIGS_CLR_CHALLENGE_CD_ACK(Json::Value &root, PKT_CLIGS_CLR_CHALLENGE_CD_ACK &data);
INT32 EncodePKT_CLIGS_SYNC_TIME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SYNC_TIME_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SYNC_TIME_REQ( PKT_CLIGS_SYNC_TIME_REQ &data );
BOOL FromJsonPKT_CLIGS_SYNC_TIME_REQ(const string &strData, PKT_CLIGS_SYNC_TIME_REQ &data);
BOOL FromJsonPKT_CLIGS_SYNC_TIME_REQ(Json::Value &root, PKT_CLIGS_SYNC_TIME_REQ &data);
INT32 EncodePKT_CLIGS_SYNC_TIME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SYNC_TIME_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SYNC_TIME_ACK( PKT_CLIGS_SYNC_TIME_ACK &data );
BOOL FromJsonPKT_CLIGS_SYNC_TIME_ACK(const string &strData, PKT_CLIGS_SYNC_TIME_ACK &data);
BOOL FromJsonPKT_CLIGS_SYNC_TIME_ACK(Json::Value &root, PKT_CLIGS_SYNC_TIME_ACK &data);
INT32 EncodePKT_CLILS_REGISTER_DSPNAME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_DSPNAME_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_REGISTER_DSPNAME_REQ( PKT_CLILS_REGISTER_DSPNAME_REQ &data );
BOOL FromJsonPKT_CLILS_REGISTER_DSPNAME_REQ(const string &strData, PKT_CLILS_REGISTER_DSPNAME_REQ &data);
BOOL FromJsonPKT_CLILS_REGISTER_DSPNAME_REQ(Json::Value &root, PKT_CLILS_REGISTER_DSPNAME_REQ &data);
INT32 EncodePKT_CLILS_REGISTER_DSPNAME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_DSPNAME_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_REGISTER_DSPNAME_ACK( PKT_CLILS_REGISTER_DSPNAME_ACK &data );
BOOL FromJsonPKT_CLILS_REGISTER_DSPNAME_ACK(const string &strData, PKT_CLILS_REGISTER_DSPNAME_ACK &data);
BOOL FromJsonPKT_CLILS_REGISTER_DSPNAME_ACK(Json::Value &root, PKT_CLILS_REGISTER_DSPNAME_ACK &data);
INT32 EncodePKT_CLILS_USE_GOODS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_USE_GOODS_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_USE_GOODS_REQ( PKT_CLILS_USE_GOODS_REQ &data );
BOOL FromJsonPKT_CLILS_USE_GOODS_REQ(const string &strData, PKT_CLILS_USE_GOODS_REQ &data);
BOOL FromJsonPKT_CLILS_USE_GOODS_REQ(Json::Value &root, PKT_CLILS_USE_GOODS_REQ &data);
INT32 EncodePKT_CLILS_USE_GOODS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_USE_GOODS_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_USE_GOODS_ACK( PKT_CLILS_USE_GOODS_ACK &data );
BOOL FromJsonPKT_CLILS_USE_GOODS_ACK(const string &strData, PKT_CLILS_USE_GOODS_ACK &data);
BOOL FromJsonPKT_CLILS_USE_GOODS_ACK(Json::Value &root, PKT_CLILS_USE_GOODS_ACK &data);
INT32 EncodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ( PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ &data );
BOOL FromJsonPKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ(const string &strData, PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ &data);
BOOL FromJsonPKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ(Json::Value &root, PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ &data);
INT32 EncodeDT_LOGIN_ENCOURAG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOGIN_ENCOURAG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_LOGIN_ENCOURAG_DATA( DT_LOGIN_ENCOURAG_DATA &data );
BOOL FromJsonDT_LOGIN_ENCOURAG_DATA(const string &strData, DT_LOGIN_ENCOURAG_DATA &data);
BOOL FromJsonDT_LOGIN_ENCOURAG_DATA(Json::Value &root, DT_LOGIN_ENCOURAG_DATA &data);
INT32 EncodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK( PKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK &data );
BOOL FromJsonPKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK(const string &strData, PKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK &data);
BOOL FromJsonPKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK(Json::Value &root, PKT_CLILS_OPEN_LOGIN_ENCOURAGE_ACK &data);
INT32 EncodePKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ( PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ &data );
BOOL FromJsonPKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ(const string &strData, PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ &data);
BOOL FromJsonPKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ(Json::Value &root, PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ &data);
INT32 EncodePKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK( PKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK &data );
BOOL FromJsonPKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK(const string &strData, PKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK &data);
BOOL FromJsonPKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK(Json::Value &root, PKT_CLILS_RATE_LOGIN_ENCOURAGE_ACK &data);
INT32 EncodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ( PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ &data );
BOOL FromJsonPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ(const string &strData, PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ &data);
BOOL FromJsonPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ(Json::Value &root, PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ &data);
INT32 EncodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK( PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK &data );
BOOL FromJsonPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK(const string &strData, PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK &data);
BOOL FromJsonPKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK(Json::Value &root, PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_ACK &data);
INT32 EncodeDT_CAPTURE_WORK_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_WORK_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CAPTURE_WORK_DATA_CLI( DT_CAPTURE_WORK_DATA_CLI &data );
BOOL FromJsonDT_CAPTURE_WORK_DATA_CLI(const string &strData, DT_CAPTURE_WORK_DATA_CLI &data);
BOOL FromJsonDT_CAPTURE_WORK_DATA_CLI(Json::Value &root, DT_CAPTURE_WORK_DATA_CLI &data);
INT32 EncodeDT_CAPTURE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CAPTURE_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CAPTURE_DATA_CLI( DT_CAPTURE_DATA_CLI &data );
BOOL FromJsonDT_CAPTURE_DATA_CLI(const string &strData, DT_CAPTURE_DATA_CLI &data);
BOOL FromJsonDT_CAPTURE_DATA_CLI(Json::Value &root, DT_CAPTURE_DATA_CLI &data);
INT32 EncodePKT_CLIGS_PLUNDER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_PLUNDER_REQ( PKT_CLIGS_PLUNDER_REQ &data );
BOOL FromJsonPKT_CLIGS_PLUNDER_REQ(const string &strData, PKT_CLIGS_PLUNDER_REQ &data);
BOOL FromJsonPKT_CLIGS_PLUNDER_REQ(Json::Value &root, PKT_CLIGS_PLUNDER_REQ &data);
INT32 EncodeDT_PLUNDER_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_ENCOURAGE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_ENCOURAGE_DATA( DT_PLUNDER_ENCOURAGE_DATA &data );
BOOL FromJsonDT_PLUNDER_ENCOURAGE_DATA(const string &strData, DT_PLUNDER_ENCOURAGE_DATA &data);
BOOL FromJsonDT_PLUNDER_ENCOURAGE_DATA(Json::Value &root, DT_PLUNDER_ENCOURAGE_DATA &data);
INT32 EncodeDT_BECAPTURE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_BECAPTURE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_BECAPTURE_DATA( DT_BECAPTURE_DATA &data );
BOOL FromJsonDT_BECAPTURE_DATA(const string &strData, DT_BECAPTURE_DATA &data);
BOOL FromJsonDT_BECAPTURE_DATA(Json::Value &root, DT_BECAPTURE_DATA &data);
INT32 EncodePKT_CLIGS_PLUNDER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_PLUNDER_ACK( PKT_CLIGS_PLUNDER_ACK &data );
BOOL FromJsonPKT_CLIGS_PLUNDER_ACK(const string &strData, PKT_CLIGS_PLUNDER_ACK &data);
BOOL FromJsonPKT_CLIGS_PLUNDER_ACK(Json::Value &root, PKT_CLIGS_PLUNDER_ACK &data);
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_BATTLELOG_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_BATTLELOG_DATA_CLI( DT_PLUNDER_BATTLELOG_DATA_CLI &data );
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA_CLI(const string &strData, DT_PLUNDER_BATTLELOG_DATA_CLI &data);
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA_CLI(Json::Value &root, DT_PLUNDER_BATTLELOG_DATA_CLI &data);
INT32 EncodePKT_CLIGS_OPEN_CAPTURE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CAPTURE_TAB_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_CAPTURE_TAB_REQ( PKT_CLIGS_OPEN_CAPTURE_TAB_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_CAPTURE_TAB_REQ(const string &strData, PKT_CLIGS_OPEN_CAPTURE_TAB_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_CAPTURE_TAB_REQ(Json::Value &root, PKT_CLIGS_OPEN_CAPTURE_TAB_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_CAPTURE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CAPTURE_TAB_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_CAPTURE_TAB_ACK( PKT_CLIGS_OPEN_CAPTURE_TAB_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_CAPTURE_TAB_ACK(const string &strData, PKT_CLIGS_OPEN_CAPTURE_TAB_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_CAPTURE_TAB_ACK(Json::Value &root, PKT_CLIGS_OPEN_CAPTURE_TAB_ACK &data);
INT32 EncodeDT_ENEMY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ENEMY_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_ENEMY_DATA_CLI( DT_ENEMY_DATA_CLI &data );
BOOL FromJsonDT_ENEMY_DATA_CLI(const string &strData, DT_ENEMY_DATA_CLI &data);
BOOL FromJsonDT_ENEMY_DATA_CLI(Json::Value &root, DT_ENEMY_DATA_CLI &data);
INT32 EncodePKT_CLIGS_OPEN_ENEMY_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ENEMY_TAB_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_ENEMY_TAB_REQ( PKT_CLIGS_OPEN_ENEMY_TAB_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_ENEMY_TAB_REQ(const string &strData, PKT_CLIGS_OPEN_ENEMY_TAB_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_ENEMY_TAB_REQ(Json::Value &root, PKT_CLIGS_OPEN_ENEMY_TAB_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_ENEMY_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_ENEMY_TAB_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_ENEMY_TAB_ACK( PKT_CLIGS_OPEN_ENEMY_TAB_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_ENEMY_TAB_ACK(const string &strData, PKT_CLIGS_OPEN_ENEMY_TAB_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_ENEMY_TAB_ACK(Json::Value &root, PKT_CLIGS_OPEN_ENEMY_TAB_ACK &data);
INT32 EncodePKT_CLIGS_DEL_ENEMY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_ENEMY_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DEL_ENEMY_REQ( PKT_CLIGS_DEL_ENEMY_REQ &data );
BOOL FromJsonPKT_CLIGS_DEL_ENEMY_REQ(const string &strData, PKT_CLIGS_DEL_ENEMY_REQ &data);
BOOL FromJsonPKT_CLIGS_DEL_ENEMY_REQ(Json::Value &root, PKT_CLIGS_DEL_ENEMY_REQ &data);
INT32 EncodePKT_CLIGS_DEL_ENEMY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DEL_ENEMY_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DEL_ENEMY_ACK( PKT_CLIGS_DEL_ENEMY_ACK &data );
BOOL FromJsonPKT_CLIGS_DEL_ENEMY_ACK(const string &strData, PKT_CLIGS_DEL_ENEMY_ACK &data);
BOOL FromJsonPKT_CLIGS_DEL_ENEMY_ACK(Json::Value &root, PKT_CLIGS_DEL_ENEMY_ACK &data);
INT32 EncodePKT_CLIGS_PLUNDER_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PLUNDER_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_PLUNDER_BATTLELOG_NTF( PKT_CLIGS_PLUNDER_BATTLELOG_NTF &data );
BOOL FromJsonPKT_CLIGS_PLUNDER_BATTLELOG_NTF(const string &strData, PKT_CLIGS_PLUNDER_BATTLELOG_NTF &data);
BOOL FromJsonPKT_CLIGS_PLUNDER_BATTLELOG_NTF(Json::Value &root, PKT_CLIGS_PLUNDER_BATTLELOG_NTF &data);
INT32 EncodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ( PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ(const string &strData, PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ(Json::Value &root, PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_REQ &data);
INT32 EncodeDT_PLUNDER_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PLUNDER_PLAYER_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_PLUNDER_PLAYER_DATA( DT_PLUNDER_PLAYER_DATA &data );
BOOL FromJsonDT_PLUNDER_PLAYER_DATA(const string &strData, DT_PLUNDER_PLAYER_DATA &data);
BOOL FromJsonDT_PLUNDER_PLAYER_DATA(Json::Value &root, DT_PLUNDER_PLAYER_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK( PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK(const string &strData, PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK(Json::Value &root, PKT_CLIGS_OPEN_PLUNDER_RECOMMEND_PLAYER_ACK &data);
INT32 EncodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ( PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ &data );
BOOL FromJsonPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ(const string &strData, PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ &data);
BOOL FromJsonPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ(Json::Value &root, PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_REQ &data);
INT32 EncodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK( PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK &data );
BOOL FromJsonPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK(const string &strData, PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK &data);
BOOL FromJsonPKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK(Json::Value &root, PKT_CLIGS_REFRESH_PLUNDER_RECOMMEND_ACK &data);
INT32 EncodePKT_CLIGS_FREE_CAPTURE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_CAPTURE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_FREE_CAPTURE_REQ( PKT_CLIGS_FREE_CAPTURE_REQ &data );
BOOL FromJsonPKT_CLIGS_FREE_CAPTURE_REQ(const string &strData, PKT_CLIGS_FREE_CAPTURE_REQ &data);
BOOL FromJsonPKT_CLIGS_FREE_CAPTURE_REQ(Json::Value &root, PKT_CLIGS_FREE_CAPTURE_REQ &data);
INT32 EncodePKT_CLIGS_FREE_CAPTURE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_CAPTURE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_FREE_CAPTURE_ACK( PKT_CLIGS_FREE_CAPTURE_ACK &data );
BOOL FromJsonPKT_CLIGS_FREE_CAPTURE_ACK(const string &strData, PKT_CLIGS_FREE_CAPTURE_ACK &data);
BOOL FromJsonPKT_CLIGS_FREE_CAPTURE_ACK(Json::Value &root, PKT_CLIGS_FREE_CAPTURE_ACK &data);
INT32 EncodePKT_CLIGS_COLLECT_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COLLECT_INCOME_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_COLLECT_INCOME_REQ( PKT_CLIGS_COLLECT_INCOME_REQ &data );
BOOL FromJsonPKT_CLIGS_COLLECT_INCOME_REQ(const string &strData, PKT_CLIGS_COLLECT_INCOME_REQ &data);
BOOL FromJsonPKT_CLIGS_COLLECT_INCOME_REQ(Json::Value &root, PKT_CLIGS_COLLECT_INCOME_REQ &data);
INT32 EncodePKT_CLIGS_COLLECT_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COLLECT_INCOME_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_COLLECT_INCOME_ACK( PKT_CLIGS_COLLECT_INCOME_ACK &data );
BOOL FromJsonPKT_CLIGS_COLLECT_INCOME_ACK(const string &strData, PKT_CLIGS_COLLECT_INCOME_ACK &data);
BOOL FromJsonPKT_CLIGS_COLLECT_INCOME_ACK(Json::Value &root, PKT_CLIGS_COLLECT_INCOME_ACK &data);
INT32 EncodePKT_CLIGS_BLEED_CAPTURE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BLEED_CAPTURE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BLEED_CAPTURE_REQ( PKT_CLIGS_BLEED_CAPTURE_REQ &data );
BOOL FromJsonPKT_CLIGS_BLEED_CAPTURE_REQ(const string &strData, PKT_CLIGS_BLEED_CAPTURE_REQ &data);
BOOL FromJsonPKT_CLIGS_BLEED_CAPTURE_REQ(Json::Value &root, PKT_CLIGS_BLEED_CAPTURE_REQ &data);
INT32 EncodePKT_CLIGS_BLEED_CAPTURE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BLEED_CAPTURE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BLEED_CAPTURE_ACK( PKT_CLIGS_BLEED_CAPTURE_ACK &data );
BOOL FromJsonPKT_CLIGS_BLEED_CAPTURE_ACK(const string &strData, PKT_CLIGS_BLEED_CAPTURE_ACK &data);
BOOL FromJsonPKT_CLIGS_BLEED_CAPTURE_ACK(Json::Value &root, PKT_CLIGS_BLEED_CAPTURE_ACK &data);
INT32 EncodePKT_CLIGS_SET_WORK_TYPE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SET_WORK_TYPE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SET_WORK_TYPE_REQ( PKT_CLIGS_SET_WORK_TYPE_REQ &data );
BOOL FromJsonPKT_CLIGS_SET_WORK_TYPE_REQ(const string &strData, PKT_CLIGS_SET_WORK_TYPE_REQ &data);
BOOL FromJsonPKT_CLIGS_SET_WORK_TYPE_REQ(Json::Value &root, PKT_CLIGS_SET_WORK_TYPE_REQ &data);
INT32 EncodePKT_CLIGS_SET_WORK_TYPE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SET_WORK_TYPE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SET_WORK_TYPE_ACK( PKT_CLIGS_SET_WORK_TYPE_ACK &data );
BOOL FromJsonPKT_CLIGS_SET_WORK_TYPE_ACK(const string &strData, PKT_CLIGS_SET_WORK_TYPE_ACK &data);
BOOL FromJsonPKT_CLIGS_SET_WORK_TYPE_ACK(Json::Value &root, PKT_CLIGS_SET_WORK_TYPE_ACK &data);
INT32 EncodeDT_MONEY_TREE_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MONEY_TREE_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_MONEY_TREE_DATA_CLI( DT_MONEY_TREE_DATA_CLI &data );
BOOL FromJsonDT_MONEY_TREE_DATA_CLI(const string &strData, DT_MONEY_TREE_DATA_CLI &data);
BOOL FromJsonDT_MONEY_TREE_DATA_CLI(Json::Value &root, DT_MONEY_TREE_DATA_CLI &data);
INT32 EncodePKT_CLIGS_OPEN_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_MONEY_TREE_REQ( PKT_CLIGS_OPEN_MONEY_TREE_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_MONEY_TREE_REQ(const string &strData, PKT_CLIGS_OPEN_MONEY_TREE_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_MONEY_TREE_REQ(Json::Value &root, PKT_CLIGS_OPEN_MONEY_TREE_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_MONEY_TREE_ACK( PKT_CLIGS_OPEN_MONEY_TREE_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_MONEY_TREE_ACK(const string &strData, PKT_CLIGS_OPEN_MONEY_TREE_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_MONEY_TREE_ACK(Json::Value &root, PKT_CLIGS_OPEN_MONEY_TREE_ACK &data);
INT32 EncodePKT_CLIGS_ROCK_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ROCK_MONEY_TREE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ROCK_MONEY_TREE_REQ( PKT_CLIGS_ROCK_MONEY_TREE_REQ &data );
BOOL FromJsonPKT_CLIGS_ROCK_MONEY_TREE_REQ(const string &strData, PKT_CLIGS_ROCK_MONEY_TREE_REQ &data);
BOOL FromJsonPKT_CLIGS_ROCK_MONEY_TREE_REQ(Json::Value &root, PKT_CLIGS_ROCK_MONEY_TREE_REQ &data);
INT32 EncodePKT_CLIGS_ROCK_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ROCK_MONEY_TREE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ROCK_MONEY_TREE_ACK( PKT_CLIGS_ROCK_MONEY_TREE_ACK &data );
BOOL FromJsonPKT_CLIGS_ROCK_MONEY_TREE_ACK(const string &strData, PKT_CLIGS_ROCK_MONEY_TREE_ACK &data);
BOOL FromJsonPKT_CLIGS_ROCK_MONEY_TREE_ACK(Json::Value &root, PKT_CLIGS_ROCK_MONEY_TREE_ACK &data);
INT32 EncodePKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ( PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ(const string &strData, PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ(Json::Value &root, PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ &data);
INT32 EncodePKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK( PKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK(const string &strData, PKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK(Json::Value &root, PKT_CLIGS_GET_PLUNDER_BATTLELOG_ACK &data);
INT32 EncodeDT_GODWEAPON_ATTR_INC_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_ATTR_INC_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GODWEAPON_ATTR_INC_DATA( DT_GODWEAPON_ATTR_INC_DATA &data );
BOOL FromJsonDT_GODWEAPON_ATTR_INC_DATA(const string &strData, DT_GODWEAPON_ATTR_INC_DATA &data);
BOOL FromJsonDT_GODWEAPON_ATTR_INC_DATA(Json::Value &root, DT_GODWEAPON_ATTR_INC_DATA &data);
INT32 EncodeDT_GODWEAPON_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_ATTR_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GODWEAPON_ATTR_DATA( DT_GODWEAPON_ATTR_DATA &data );
BOOL FromJsonDT_GODWEAPON_ATTR_DATA(const string &strData, DT_GODWEAPON_ATTR_DATA &data);
BOOL FromJsonDT_GODWEAPON_ATTR_DATA(Json::Value &root, DT_GODWEAPON_ATTR_DATA &data);
INT32 EncodeDT_GODWEAPON_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GODWEAPON_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_GODWEAPON_DATA_CLI( DT_GODWEAPON_DATA_CLI &data );
BOOL FromJsonDT_GODWEAPON_DATA_CLI(const string &strData, DT_GODWEAPON_DATA_CLI &data);
BOOL FromJsonDT_GODWEAPON_DATA_CLI(Json::Value &root, DT_GODWEAPON_DATA_CLI &data);
INT32 EncodePKT_CLIGS_OPEN_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_GODWEAPON_REQ( PKT_CLIGS_OPEN_GODWEAPON_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_GODWEAPON_REQ(const string &strData, PKT_CLIGS_OPEN_GODWEAPON_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_GODWEAPON_REQ(Json::Value &root, PKT_CLIGS_OPEN_GODWEAPON_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_GODWEAPON_ACK( PKT_CLIGS_OPEN_GODWEAPON_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_GODWEAPON_ACK(const string &strData, PKT_CLIGS_OPEN_GODWEAPON_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_GODWEAPON_ACK(Json::Value &root, PKT_CLIGS_OPEN_GODWEAPON_ACK &data);
INT32 EncodePKT_CLIGS_UPGRADE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPGRADE_GODWEAPON_REQ( PKT_CLIGS_UPGRADE_GODWEAPON_REQ &data );
BOOL FromJsonPKT_CLIGS_UPGRADE_GODWEAPON_REQ(const string &strData, PKT_CLIGS_UPGRADE_GODWEAPON_REQ &data);
BOOL FromJsonPKT_CLIGS_UPGRADE_GODWEAPON_REQ(Json::Value &root, PKT_CLIGS_UPGRADE_GODWEAPON_REQ &data);
INT32 EncodePKT_CLIGS_UPGRADE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPGRADE_GODWEAPON_ACK( PKT_CLIGS_UPGRADE_GODWEAPON_ACK &data );
BOOL FromJsonPKT_CLIGS_UPGRADE_GODWEAPON_ACK(const string &strData, PKT_CLIGS_UPGRADE_GODWEAPON_ACK &data);
BOOL FromJsonPKT_CLIGS_UPGRADE_GODWEAPON_ACK(Json::Value &root, PKT_CLIGS_UPGRADE_GODWEAPON_ACK &data);
INT32 EncodePKT_CLIGS_FREE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_GODWEAPON_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_FREE_GODWEAPON_REQ( PKT_CLIGS_FREE_GODWEAPON_REQ &data );
BOOL FromJsonPKT_CLIGS_FREE_GODWEAPON_REQ(const string &strData, PKT_CLIGS_FREE_GODWEAPON_REQ &data);
BOOL FromJsonPKT_CLIGS_FREE_GODWEAPON_REQ(Json::Value &root, PKT_CLIGS_FREE_GODWEAPON_REQ &data);
INT32 EncodePKT_CLIGS_FREE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_FREE_GODWEAPON_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_FREE_GODWEAPON_ACK( PKT_CLIGS_FREE_GODWEAPON_ACK &data );
BOOL FromJsonPKT_CLIGS_FREE_GODWEAPON_ACK(const string &strData, PKT_CLIGS_FREE_GODWEAPON_ACK &data);
BOOL FromJsonPKT_CLIGS_FREE_GODWEAPON_ACK(Json::Value &root, PKT_CLIGS_FREE_GODWEAPON_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_AWAKEN_REQ( PKT_CLIGS_OPEN_AWAKEN_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_AWAKEN_REQ(const string &strData, PKT_CLIGS_OPEN_AWAKEN_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_AWAKEN_REQ(Json::Value &root, PKT_CLIGS_OPEN_AWAKEN_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_AWAKEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_AWAKEN_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_AWAKEN_ACK( PKT_CLIGS_OPEN_AWAKEN_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_AWAKEN_ACK(const string &strData, PKT_CLIGS_OPEN_AWAKEN_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_AWAKEN_ACK(Json::Value &root, PKT_CLIGS_OPEN_AWAKEN_ACK &data);
INT32 EncodePKT_CLIGS_AWAKEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AWAKEN_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_AWAKEN_REQ( PKT_CLIGS_AWAKEN_REQ &data );
BOOL FromJsonPKT_CLIGS_AWAKEN_REQ(const string &strData, PKT_CLIGS_AWAKEN_REQ &data);
BOOL FromJsonPKT_CLIGS_AWAKEN_REQ(Json::Value &root, PKT_CLIGS_AWAKEN_REQ &data);
INT32 EncodePKT_CLIGS_AWAKEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_AWAKEN_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_AWAKEN_ACK( PKT_CLIGS_AWAKEN_ACK &data );
BOOL FromJsonPKT_CLIGS_AWAKEN_ACK(const string &strData, PKT_CLIGS_AWAKEN_ACK &data);
BOOL FromJsonPKT_CLIGS_AWAKEN_ACK(Json::Value &root, PKT_CLIGS_AWAKEN_ACK &data);
INT32 EncodePKT_CLIGS_PICKUP_STUDY_CLIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PICKUP_STUDY_CLIP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_PICKUP_STUDY_CLIP_REQ( PKT_CLIGS_PICKUP_STUDY_CLIP_REQ &data );
BOOL FromJsonPKT_CLIGS_PICKUP_STUDY_CLIP_REQ(const string &strData, PKT_CLIGS_PICKUP_STUDY_CLIP_REQ &data);
BOOL FromJsonPKT_CLIGS_PICKUP_STUDY_CLIP_REQ(Json::Value &root, PKT_CLIGS_PICKUP_STUDY_CLIP_REQ &data);
INT32 EncodePKT_CLIGS_PICKUP_STUDY_CLIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_PICKUP_STUDY_CLIP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_PICKUP_STUDY_CLIP_ACK( PKT_CLIGS_PICKUP_STUDY_CLIP_ACK &data );
BOOL FromJsonPKT_CLIGS_PICKUP_STUDY_CLIP_ACK(const string &strData, PKT_CLIGS_PICKUP_STUDY_CLIP_ACK &data);
BOOL FromJsonPKT_CLIGS_PICKUP_STUDY_CLIP_ACK(Json::Value &root, PKT_CLIGS_PICKUP_STUDY_CLIP_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_STUDY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STUDY_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_STUDY_REQ( PKT_CLIGS_OPEN_STUDY_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_REQ(const string &strData, PKT_CLIGS_OPEN_STUDY_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_REQ(Json::Value &root, PKT_CLIGS_OPEN_STUDY_REQ &data);
INT32 EncodeDT_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STUDY_DATA_CLI( DT_STUDY_DATA_CLI &data );
BOOL FromJsonDT_STUDY_DATA_CLI(const string &strData, DT_STUDY_DATA_CLI &data);
BOOL FromJsonDT_STUDY_DATA_CLI(Json::Value &root, DT_STUDY_DATA_CLI &data);
INT32 EncodeDT_STUDY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STUDY_DATA_LST_CLI( DT_STUDY_DATA_LST_CLI &data );
BOOL FromJsonDT_STUDY_DATA_LST_CLI(const string &strData, DT_STUDY_DATA_LST_CLI &data);
BOOL FromJsonDT_STUDY_DATA_LST_CLI(Json::Value &root, DT_STUDY_DATA_LST_CLI &data);
INT32 EncodeDT_HERO_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_STUDY_DATA_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_STUDY_DATA_CLI( DT_HERO_STUDY_DATA_CLI &data );
BOOL FromJsonDT_HERO_STUDY_DATA_CLI(const string &strData, DT_HERO_STUDY_DATA_CLI &data);
BOOL FromJsonDT_HERO_STUDY_DATA_CLI(Json::Value &root, DT_HERO_STUDY_DATA_CLI &data);
INT32 EncodeDT_HERO_STUDY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
INT32 DecodeDT_HERO_STUDY_DATA_LST_CLI(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_HERO_STUDY_DATA_LST_CLI( DT_HERO_STUDY_DATA_LST_CLI &data );
BOOL FromJsonDT_HERO_STUDY_DATA_LST_CLI(const string &strData, DT_HERO_STUDY_DATA_LST_CLI &data);
BOOL FromJsonDT_HERO_STUDY_DATA_LST_CLI(Json::Value &root, DT_HERO_STUDY_DATA_LST_CLI &data);
INT32 EncodePKT_CLIGS_OPEN_STUDY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STUDY_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_STUDY_ACK( PKT_CLIGS_OPEN_STUDY_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_ACK(const string &strData, PKT_CLIGS_OPEN_STUDY_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_ACK(Json::Value &root, PKT_CLIGS_OPEN_STUDY_ACK &data);
INT32 EncodePKT_CLIGS_DRESS_STUDY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_STUDY_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DRESS_STUDY_REQ( PKT_CLIGS_DRESS_STUDY_REQ &data );
BOOL FromJsonPKT_CLIGS_DRESS_STUDY_REQ(const string &strData, PKT_CLIGS_DRESS_STUDY_REQ &data);
BOOL FromJsonPKT_CLIGS_DRESS_STUDY_REQ(Json::Value &root, PKT_CLIGS_DRESS_STUDY_REQ &data);
INT32 EncodePKT_CLIGS_DRESS_STUDY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_STUDY_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DRESS_STUDY_ACK( PKT_CLIGS_DRESS_STUDY_ACK &data );
BOOL FromJsonPKT_CLIGS_DRESS_STUDY_ACK(const string &strData, PKT_CLIGS_DRESS_STUDY_ACK &data);
BOOL FromJsonPKT_CLIGS_DRESS_STUDY_ACK(Json::Value &root, PKT_CLIGS_DRESS_STUDY_ACK &data);
INT32 EncodePKT_CLIGS_STUDY_CLIP_TO_EXP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_CLIP_TO_EXP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_CLIP_TO_EXP_REQ( PKT_CLIGS_STUDY_CLIP_TO_EXP_REQ &data );
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_TO_EXP_REQ(const string &strData, PKT_CLIGS_STUDY_CLIP_TO_EXP_REQ &data);
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_TO_EXP_REQ(Json::Value &root, PKT_CLIGS_STUDY_CLIP_TO_EXP_REQ &data);
INT32 EncodePKT_CLIGS_STUDY_CLIP_TO_EXP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_CLIP_TO_EXP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_CLIP_TO_EXP_ACK( PKT_CLIGS_STUDY_CLIP_TO_EXP_ACK &data );
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_TO_EXP_ACK(const string &strData, PKT_CLIGS_STUDY_CLIP_TO_EXP_ACK &data);
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_TO_EXP_ACK(Json::Value &root, PKT_CLIGS_STUDY_CLIP_TO_EXP_ACK &data);
INT32 EncodePKT_CLIGS_STUDY_TO_EXP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_TO_EXP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_TO_EXP_REQ( PKT_CLIGS_STUDY_TO_EXP_REQ &data );
BOOL FromJsonPKT_CLIGS_STUDY_TO_EXP_REQ(const string &strData, PKT_CLIGS_STUDY_TO_EXP_REQ &data);
BOOL FromJsonPKT_CLIGS_STUDY_TO_EXP_REQ(Json::Value &root, PKT_CLIGS_STUDY_TO_EXP_REQ &data);
INT32 EncodePKT_CLIGS_STUDY_TO_EXP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_TO_EXP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_TO_EXP_ACK( PKT_CLIGS_STUDY_TO_EXP_ACK &data );
BOOL FromJsonPKT_CLIGS_STUDY_TO_EXP_ACK(const string &strData, PKT_CLIGS_STUDY_TO_EXP_ACK &data);
BOOL FromJsonPKT_CLIGS_STUDY_TO_EXP_ACK(Json::Value &root, PKT_CLIGS_STUDY_TO_EXP_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_STUDY_CONVERT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STUDY_CONVERT_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_STUDY_CONVERT_REQ( PKT_CLIGS_OPEN_STUDY_CONVERT_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_CONVERT_REQ(const string &strData, PKT_CLIGS_OPEN_STUDY_CONVERT_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_CONVERT_REQ(Json::Value &root, PKT_CLIGS_OPEN_STUDY_CONVERT_REQ &data);
INT32 EncodeDT_STUDY_CFG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_STUDY_CFG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_STUDY_CFG_DATA( DT_STUDY_CFG_DATA &data );
BOOL FromJsonDT_STUDY_CFG_DATA(const string &strData, DT_STUDY_CFG_DATA &data);
BOOL FromJsonDT_STUDY_CFG_DATA(Json::Value &root, DT_STUDY_CFG_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_STUDY_CONVERT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_STUDY_CONVERT_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_STUDY_CONVERT_ACK( PKT_CLIGS_OPEN_STUDY_CONVERT_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_CONVERT_ACK(const string &strData, PKT_CLIGS_OPEN_STUDY_CONVERT_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_STUDY_CONVERT_ACK(Json::Value &root, PKT_CLIGS_OPEN_STUDY_CONVERT_ACK &data);
INT32 EncodePKT_CLIGS_STUDY_CONVERT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_CONVERT_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_CONVERT_REQ( PKT_CLIGS_STUDY_CONVERT_REQ &data );
BOOL FromJsonPKT_CLIGS_STUDY_CONVERT_REQ(const string &strData, PKT_CLIGS_STUDY_CONVERT_REQ &data);
BOOL FromJsonPKT_CLIGS_STUDY_CONVERT_REQ(Json::Value &root, PKT_CLIGS_STUDY_CONVERT_REQ &data);
INT32 EncodePKT_CLIGS_STUDY_CONVERT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_CONVERT_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_CONVERT_ACK( PKT_CLIGS_STUDY_CONVERT_ACK &data );
BOOL FromJsonPKT_CLIGS_STUDY_CONVERT_ACK(const string &strData, PKT_CLIGS_STUDY_CONVERT_ACK &data);
BOOL FromJsonPKT_CLIGS_STUDY_CONVERT_ACK(Json::Value &root, PKT_CLIGS_STUDY_CONVERT_ACK &data);
INT32 EncodePKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ( PKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ &data );
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ(const string &strData, PKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ &data);
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ(Json::Value &root, PKT_CLIGS_STUDY_CLIP_AUTO2EXP_REQ &data);
INT32 EncodePKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK( PKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK &data );
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK(const string &strData, PKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK &data);
BOOL FromJsonPKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK(Json::Value &root, PKT_CLIGS_STUDY_CLIP_AUTO2EXP_ACK &data);
INT32 EncodePKT_CLIGS_STUDY_UPGRADE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_UPGRADE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_UPGRADE_REQ( PKT_CLIGS_STUDY_UPGRADE_REQ &data );
BOOL FromJsonPKT_CLIGS_STUDY_UPGRADE_REQ(const string &strData, PKT_CLIGS_STUDY_UPGRADE_REQ &data);
BOOL FromJsonPKT_CLIGS_STUDY_UPGRADE_REQ(Json::Value &root, PKT_CLIGS_STUDY_UPGRADE_REQ &data);
INT32 EncodePKT_CLIGS_STUDY_UPGRADE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_STUDY_UPGRADE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_STUDY_UPGRADE_ACK( PKT_CLIGS_STUDY_UPGRADE_ACK &data );
BOOL FromJsonPKT_CLIGS_STUDY_UPGRADE_ACK(const string &strData, PKT_CLIGS_STUDY_UPGRADE_ACK &data);
BOOL FromJsonPKT_CLIGS_STUDY_UPGRADE_ACK(Json::Value &root, PKT_CLIGS_STUDY_UPGRADE_ACK &data);
INT32 EncodePKT_CLIGS_RACE_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RACE_BATTLELOG_NTF(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RACE_BATTLELOG_NTF( PKT_CLIGS_RACE_BATTLELOG_NTF &data );
BOOL FromJsonPKT_CLIGS_RACE_BATTLELOG_NTF(const string &strData, PKT_CLIGS_RACE_BATTLELOG_NTF &data);
BOOL FromJsonPKT_CLIGS_RACE_BATTLELOG_NTF(Json::Value &root, PKT_CLIGS_RACE_BATTLELOG_NTF &data);
INT32 EncodeDT_LOCK_SKILL_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_LOCK_SKILL_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_LOCK_SKILL_DATA( DT_LOCK_SKILL_DATA &data );
BOOL FromJsonDT_LOCK_SKILL_DATA(const string &strData, DT_LOCK_SKILL_DATA &data);
BOOL FromJsonDT_LOCK_SKILL_DATA(Json::Value &root, DT_LOCK_SKILL_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_SKILL_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SKILL_TAB_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SKILL_TAB_REQ( PKT_CLIGS_OPEN_SKILL_TAB_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_TAB_REQ(const string &strData, PKT_CLIGS_OPEN_SKILL_TAB_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_TAB_REQ(Json::Value &root, PKT_CLIGS_OPEN_SKILL_TAB_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_SKILL_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SKILL_TAB_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SKILL_TAB_ACK( PKT_CLIGS_OPEN_SKILL_TAB_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_TAB_ACK(const string &strData, PKT_CLIGS_OPEN_SKILL_TAB_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_TAB_ACK(Json::Value &root, PKT_CLIGS_OPEN_SKILL_TAB_ACK &data);
INT32 EncodePKT_CLIGS_UPGRADE_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_SKILL_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPGRADE_SKILL_REQ( PKT_CLIGS_UPGRADE_SKILL_REQ &data );
BOOL FromJsonPKT_CLIGS_UPGRADE_SKILL_REQ(const string &strData, PKT_CLIGS_UPGRADE_SKILL_REQ &data);
BOOL FromJsonPKT_CLIGS_UPGRADE_SKILL_REQ(Json::Value &root, PKT_CLIGS_UPGRADE_SKILL_REQ &data);
INT32 EncodePKT_CLIGS_UPGRADE_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_SKILL_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPGRADE_SKILL_ACK( PKT_CLIGS_UPGRADE_SKILL_ACK &data );
BOOL FromJsonPKT_CLIGS_UPGRADE_SKILL_ACK(const string &strData, PKT_CLIGS_UPGRADE_SKILL_ACK &data);
BOOL FromJsonPKT_CLIGS_UPGRADE_SKILL_ACK(Json::Value &root, PKT_CLIGS_UPGRADE_SKILL_ACK &data);
INT32 EncodePKT_CLIGS_DRESS_SKILL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_SKILL_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DRESS_SKILL_REQ( PKT_CLIGS_DRESS_SKILL_REQ &data );
BOOL FromJsonPKT_CLIGS_DRESS_SKILL_REQ(const string &strData, PKT_CLIGS_DRESS_SKILL_REQ &data);
BOOL FromJsonPKT_CLIGS_DRESS_SKILL_REQ(Json::Value &root, PKT_CLIGS_DRESS_SKILL_REQ &data);
INT32 EncodePKT_CLIGS_DRESS_SKILL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRESS_SKILL_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DRESS_SKILL_ACK( PKT_CLIGS_DRESS_SKILL_ACK &data );
BOOL FromJsonPKT_CLIGS_DRESS_SKILL_ACK(const string &strData, PKT_CLIGS_DRESS_SKILL_ACK &data);
BOOL FromJsonPKT_CLIGS_DRESS_SKILL_ACK(Json::Value &root, PKT_CLIGS_DRESS_SKILL_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ( PKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ(const string &strData, PKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ(Json::Value &root, PKT_CLIGS_OPEN_SKILL_CARD_MAKER_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK( PKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK(const string &strData, PKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK(Json::Value &root, PKT_CLIGS_OPEN_SKILL_CARD_MAKER_ACK &data);
INT32 EncodePKT_CLIGS_MAKER_SKILL_CARD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKER_SKILL_CARD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_MAKER_SKILL_CARD_REQ( PKT_CLIGS_MAKER_SKILL_CARD_REQ &data );
BOOL FromJsonPKT_CLIGS_MAKER_SKILL_CARD_REQ(const string &strData, PKT_CLIGS_MAKER_SKILL_CARD_REQ &data);
BOOL FromJsonPKT_CLIGS_MAKER_SKILL_CARD_REQ(Json::Value &root, PKT_CLIGS_MAKER_SKILL_CARD_REQ &data);
INT32 EncodePKT_CLIGS_MAKER_SKILL_CARD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MAKER_SKILL_CARD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_MAKER_SKILL_CARD_ACK( PKT_CLIGS_MAKER_SKILL_CARD_ACK &data );
BOOL FromJsonPKT_CLIGS_MAKER_SKILL_CARD_ACK(const string &strData, PKT_CLIGS_MAKER_SKILL_CARD_ACK &data);
BOOL FromJsonPKT_CLIGS_MAKER_SKILL_CARD_ACK(Json::Value &root, PKT_CLIGS_MAKER_SKILL_CARD_ACK &data);
INT32 EncodePKT_CLIGS_BUY_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BUY_PHYSTRENGTH_REQ( PKT_CLIGS_BUY_PHYSTRENGTH_REQ &data );
BOOL FromJsonPKT_CLIGS_BUY_PHYSTRENGTH_REQ(const string &strData, PKT_CLIGS_BUY_PHYSTRENGTH_REQ &data);
BOOL FromJsonPKT_CLIGS_BUY_PHYSTRENGTH_REQ(Json::Value &root, PKT_CLIGS_BUY_PHYSTRENGTH_REQ &data);
INT32 EncodePKT_CLIGS_BUY_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_BUY_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_BUY_PHYSTRENGTH_ACK( PKT_CLIGS_BUY_PHYSTRENGTH_ACK &data );
BOOL FromJsonPKT_CLIGS_BUY_PHYSTRENGTH_ACK(const string &strData, PKT_CLIGS_BUY_PHYSTRENGTH_ACK &data);
BOOL FromJsonPKT_CLIGS_BUY_PHYSTRENGTH_ACK(Json::Value &root, PKT_CLIGS_BUY_PHYSTRENGTH_ACK &data);
INT32 EncodePKT_CLIGS_CLR_PLUNDER_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_PLUNDER_CD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_PLUNDER_CD_REQ( PKT_CLIGS_CLR_PLUNDER_CD_REQ &data );
BOOL FromJsonPKT_CLIGS_CLR_PLUNDER_CD_REQ(const string &strData, PKT_CLIGS_CLR_PLUNDER_CD_REQ &data);
BOOL FromJsonPKT_CLIGS_CLR_PLUNDER_CD_REQ(Json::Value &root, PKT_CLIGS_CLR_PLUNDER_CD_REQ &data);
INT32 EncodePKT_CLIGS_CLR_PLUNDER_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_PLUNDER_CD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_PLUNDER_CD_ACK( PKT_CLIGS_CLR_PLUNDER_CD_ACK &data );
BOOL FromJsonPKT_CLIGS_CLR_PLUNDER_CD_ACK(const string &strData, PKT_CLIGS_CLR_PLUNDER_CD_ACK &data);
BOOL FromJsonPKT_CLIGS_CLR_PLUNDER_CD_ACK(Json::Value &root, PKT_CLIGS_CLR_PLUNDER_CD_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_MAINTOWN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MAINTOWN_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_MAINTOWN_REQ( PKT_CLIGS_OPEN_MAINTOWN_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_MAINTOWN_REQ(const string &strData, PKT_CLIGS_OPEN_MAINTOWN_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_MAINTOWN_REQ(Json::Value &root, PKT_CLIGS_OPEN_MAINTOWN_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_MAINTOWN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MAINTOWN_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_MAINTOWN_ACK( PKT_CLIGS_OPEN_MAINTOWN_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_MAINTOWN_ACK(const string &strData, PKT_CLIGS_OPEN_MAINTOWN_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_MAINTOWN_ACK(Json::Value &root, PKT_CLIGS_OPEN_MAINTOWN_ACK &data);
INT32 EncodePKT_CLIGS_RECV_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_MAINTOWN_INCOME_REQ( PKT_CLIGS_RECV_MAINTOWN_INCOME_REQ &data );
BOOL FromJsonPKT_CLIGS_RECV_MAINTOWN_INCOME_REQ(const string &strData, PKT_CLIGS_RECV_MAINTOWN_INCOME_REQ &data);
BOOL FromJsonPKT_CLIGS_RECV_MAINTOWN_INCOME_REQ(Json::Value &root, PKT_CLIGS_RECV_MAINTOWN_INCOME_REQ &data);
INT32 EncodePKT_CLIGS_RECV_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_MAINTOWN_INCOME_ACK( PKT_CLIGS_RECV_MAINTOWN_INCOME_ACK &data );
BOOL FromJsonPKT_CLIGS_RECV_MAINTOWN_INCOME_ACK(const string &strData, PKT_CLIGS_RECV_MAINTOWN_INCOME_ACK &data);
BOOL FromJsonPKT_CLIGS_RECV_MAINTOWN_INCOME_ACK(Json::Value &root, PKT_CLIGS_RECV_MAINTOWN_INCOME_ACK &data);
INT32 EncodePKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ( PKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ &data );
BOOL FromJsonPKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ(const string &strData, PKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ &data);
BOOL FromJsonPKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ(Json::Value &root, PKT_CLIGS_LEVY_MAINTOWN_INCOME_REQ &data);
INT32 EncodePKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK( PKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK &data );
BOOL FromJsonPKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK(const string &strData, PKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK &data);
BOOL FromJsonPKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK(Json::Value &root, PKT_CLIGS_LEVY_MAINTOWN_INCOME_ACK &data);
INT32 EncodePKT_CLIGS_DRIVE_AWAY_OWNER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRIVE_AWAY_OWNER_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DRIVE_AWAY_OWNER_REQ( PKT_CLIGS_DRIVE_AWAY_OWNER_REQ &data );
BOOL FromJsonPKT_CLIGS_DRIVE_AWAY_OWNER_REQ(const string &strData, PKT_CLIGS_DRIVE_AWAY_OWNER_REQ &data);
BOOL FromJsonPKT_CLIGS_DRIVE_AWAY_OWNER_REQ(Json::Value &root, PKT_CLIGS_DRIVE_AWAY_OWNER_REQ &data);
INT32 EncodePKT_CLIGS_DRIVE_AWAY_OWNER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_DRIVE_AWAY_OWNER_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_DRIVE_AWAY_OWNER_ACK( PKT_CLIGS_DRIVE_AWAY_OWNER_ACK &data );
BOOL FromJsonPKT_CLIGS_DRIVE_AWAY_OWNER_ACK(const string &strData, PKT_CLIGS_DRIVE_AWAY_OWNER_ACK &data);
BOOL FromJsonPKT_CLIGS_DRIVE_AWAY_OWNER_ACK(Json::Value &root, PKT_CLIGS_DRIVE_AWAY_OWNER_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_DRUG_USE_TAB_REQ( PKT_CLIGS_OPEN_DRUG_USE_TAB_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_DRUG_USE_TAB_REQ(const string &strData, PKT_CLIGS_OPEN_DRUG_USE_TAB_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_DRUG_USE_TAB_REQ(Json::Value &root, PKT_CLIGS_OPEN_DRUG_USE_TAB_REQ &data);
INT32 EncodeDT_TALENT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_TALENT_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_TALENT_DATA( DT_TALENT_DATA &data );
BOOL FromJsonDT_TALENT_DATA(const string &strData, DT_TALENT_DATA &data);
BOOL FromJsonDT_TALENT_DATA(Json::Value &root, DT_TALENT_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_DRUG_USE_TAB_ACK( PKT_CLIGS_OPEN_DRUG_USE_TAB_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_DRUG_USE_TAB_ACK(const string &strData, PKT_CLIGS_OPEN_DRUG_USE_TAB_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_DRUG_USE_TAB_ACK(Json::Value &root, PKT_CLIGS_OPEN_DRUG_USE_TAB_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ( PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ(const string &strData, PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ(Json::Value &root, PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK( PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK(const string &strData, PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK(Json::Value &root, PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_ACK &data);
INT32 EncodePKT_CLIGS_CLR_USE_DRUG_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_USE_DRUG_CD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_USE_DRUG_CD_REQ( PKT_CLIGS_CLR_USE_DRUG_CD_REQ &data );
BOOL FromJsonPKT_CLIGS_CLR_USE_DRUG_CD_REQ(const string &strData, PKT_CLIGS_CLR_USE_DRUG_CD_REQ &data);
BOOL FromJsonPKT_CLIGS_CLR_USE_DRUG_CD_REQ(Json::Value &root, PKT_CLIGS_CLR_USE_DRUG_CD_REQ &data);
INT32 EncodePKT_CLIGS_CLR_USE_DRUG_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_USE_DRUG_CD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_USE_DRUG_CD_ACK( PKT_CLIGS_CLR_USE_DRUG_CD_ACK &data );
BOOL FromJsonPKT_CLIGS_CLR_USE_DRUG_CD_ACK(const string &strData, PKT_CLIGS_CLR_USE_DRUG_CD_ACK &data);
BOOL FromJsonPKT_CLIGS_CLR_USE_DRUG_CD_ACK(Json::Value &root, PKT_CLIGS_CLR_USE_DRUG_CD_ACK &data);
INT32 EncodePKT_CLIGS_USE_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USE_DRUG_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_USE_DRUG_REQ( PKT_CLIGS_USE_DRUG_REQ &data );
BOOL FromJsonPKT_CLIGS_USE_DRUG_REQ(const string &strData, PKT_CLIGS_USE_DRUG_REQ &data);
BOOL FromJsonPKT_CLIGS_USE_DRUG_REQ(Json::Value &root, PKT_CLIGS_USE_DRUG_REQ &data);
INT32 EncodePKT_CLIGS_USE_DRUG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USE_DRUG_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_USE_DRUG_ACK( PKT_CLIGS_USE_DRUG_ACK &data );
BOOL FromJsonPKT_CLIGS_USE_DRUG_ACK(const string &strData, PKT_CLIGS_USE_DRUG_ACK &data);
BOOL FromJsonPKT_CLIGS_USE_DRUG_ACK(Json::Value &root, PKT_CLIGS_USE_DRUG_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ( PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ(const string &strData, PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ(Json::Value &root, PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK( PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK(const string &strData, PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK(Json::Value &root, PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_ACK &data);
INT32 EncodePKT_CLIGS_UPGRADE_DRUG_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_DRUG_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPGRADE_DRUG_REQ( PKT_CLIGS_UPGRADE_DRUG_REQ &data );
BOOL FromJsonPKT_CLIGS_UPGRADE_DRUG_REQ(const string &strData, PKT_CLIGS_UPGRADE_DRUG_REQ &data);
BOOL FromJsonPKT_CLIGS_UPGRADE_DRUG_REQ(Json::Value &root, PKT_CLIGS_UPGRADE_DRUG_REQ &data);
INT32 EncodePKT_CLIGS_UPGRADE_DRUG_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPGRADE_DRUG_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPGRADE_DRUG_ACK( PKT_CLIGS_UPGRADE_DRUG_ACK &data );
BOOL FromJsonPKT_CLIGS_UPGRADE_DRUG_ACK(const string &strData, PKT_CLIGS_UPGRADE_DRUG_ACK &data);
BOOL FromJsonPKT_CLIGS_UPGRADE_DRUG_ACK(Json::Value &root, PKT_CLIGS_UPGRADE_DRUG_ACK &data);
INT32 EncodePKT_CLIGS_COMPOSE_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_EQUIP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_COMPOSE_EQUIP_REQ( PKT_CLIGS_COMPOSE_EQUIP_REQ &data );
BOOL FromJsonPKT_CLIGS_COMPOSE_EQUIP_REQ(const string &strData, PKT_CLIGS_COMPOSE_EQUIP_REQ &data);
BOOL FromJsonPKT_CLIGS_COMPOSE_EQUIP_REQ(Json::Value &root, PKT_CLIGS_COMPOSE_EQUIP_REQ &data);
INT32 EncodePKT_CLIGS_COMPOSE_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_EQUIP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_COMPOSE_EQUIP_ACK( PKT_CLIGS_COMPOSE_EQUIP_ACK &data );
BOOL FromJsonPKT_CLIGS_COMPOSE_EQUIP_ACK(const string &strData, PKT_CLIGS_COMPOSE_EQUIP_ACK &data);
BOOL FromJsonPKT_CLIGS_COMPOSE_EQUIP_ACK(Json::Value &root, PKT_CLIGS_COMPOSE_EQUIP_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PHYSTRENGTH_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_PHYSTRENGTH_REQ( PKT_CLIGS_OPEN_PHYSTRENGTH_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_PHYSTRENGTH_REQ(const string &strData, PKT_CLIGS_OPEN_PHYSTRENGTH_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_PHYSTRENGTH_REQ(Json::Value &root, PKT_CLIGS_OPEN_PHYSTRENGTH_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_PHYSTRENGTH_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_PHYSTRENGTH_ACK( PKT_CLIGS_OPEN_PHYSTRENGTH_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_PHYSTRENGTH_ACK(const string &strData, PKT_CLIGS_OPEN_PHYSTRENGTH_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_PHYSTRENGTH_ACK(Json::Value &root, PKT_CLIGS_OPEN_PHYSTRENGTH_ACK &data);
INT32 EncodePKT_CLIGS_RECV_LEFT_INCOME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_LEFT_INCOME_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_LEFT_INCOME_REQ( PKT_CLIGS_RECV_LEFT_INCOME_REQ &data );
BOOL FromJsonPKT_CLIGS_RECV_LEFT_INCOME_REQ(const string &strData, PKT_CLIGS_RECV_LEFT_INCOME_REQ &data);
BOOL FromJsonPKT_CLIGS_RECV_LEFT_INCOME_REQ(Json::Value &root, PKT_CLIGS_RECV_LEFT_INCOME_REQ &data);
INT32 EncodePKT_CLIGS_RECV_LEFT_INCOME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_RECV_LEFT_INCOME_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_RECV_LEFT_INCOME_ACK( PKT_CLIGS_RECV_LEFT_INCOME_ACK &data );
BOOL FromJsonPKT_CLIGS_RECV_LEFT_INCOME_ACK(const string &strData, PKT_CLIGS_RECV_LEFT_INCOME_ACK &data);
BOOL FromJsonPKT_CLIGS_RECV_LEFT_INCOME_ACK(Json::Value &root, PKT_CLIGS_RECV_LEFT_INCOME_ACK &data);
INT32 EncodePKT_CLIGS_SMITHY_BUY_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SMITHY_BUY_EQUIP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SMITHY_BUY_EQUIP_REQ( PKT_CLIGS_SMITHY_BUY_EQUIP_REQ &data );
BOOL FromJsonPKT_CLIGS_SMITHY_BUY_EQUIP_REQ(const string &strData, PKT_CLIGS_SMITHY_BUY_EQUIP_REQ &data);
BOOL FromJsonPKT_CLIGS_SMITHY_BUY_EQUIP_REQ(Json::Value &root, PKT_CLIGS_SMITHY_BUY_EQUIP_REQ &data);
INT32 EncodePKT_CLIGS_SMITHY_BUY_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SMITHY_BUY_EQUIP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SMITHY_BUY_EQUIP_ACK( PKT_CLIGS_SMITHY_BUY_EQUIP_ACK &data );
BOOL FromJsonPKT_CLIGS_SMITHY_BUY_EQUIP_ACK(const string &strData, PKT_CLIGS_SMITHY_BUY_EQUIP_ACK &data);
BOOL FromJsonPKT_CLIGS_SMITHY_BUY_EQUIP_ACK(Json::Value &root, PKT_CLIGS_SMITHY_BUY_EQUIP_ACK &data);
INT32 EncodePKT_CLIGS_CLR_STRENGTHEN_CD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_STRENGTHEN_CD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_STRENGTHEN_CD_REQ( PKT_CLIGS_CLR_STRENGTHEN_CD_REQ &data );
BOOL FromJsonPKT_CLIGS_CLR_STRENGTHEN_CD_REQ(const string &strData, PKT_CLIGS_CLR_STRENGTHEN_CD_REQ &data);
BOOL FromJsonPKT_CLIGS_CLR_STRENGTHEN_CD_REQ(Json::Value &root, PKT_CLIGS_CLR_STRENGTHEN_CD_REQ &data);
INT32 EncodePKT_CLIGS_CLR_STRENGTHEN_CD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_STRENGTHEN_CD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_STRENGTHEN_CD_ACK( PKT_CLIGS_CLR_STRENGTHEN_CD_ACK &data );
BOOL FromJsonPKT_CLIGS_CLR_STRENGTHEN_CD_ACK(const string &strData, PKT_CLIGS_CLR_STRENGTHEN_CD_ACK &data);
BOOL FromJsonPKT_CLIGS_CLR_STRENGTHEN_CD_ACK(Json::Value &root, PKT_CLIGS_CLR_STRENGTHEN_CD_ACK &data);
INT32 EncodeDT_CROP_SEED_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_SEED_INFO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROP_SEED_INFO_DATA( DT_CROP_SEED_INFO_DATA &data );
BOOL FromJsonDT_CROP_SEED_INFO_DATA(const string &strData, DT_CROP_SEED_INFO_DATA &data);
BOOL FromJsonDT_CROP_SEED_INFO_DATA(Json::Value &root, DT_CROP_SEED_INFO_DATA &data);
INT32 EncodeDT_CROP_FIELD_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_FIELD_INFO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROP_FIELD_INFO_DATA( DT_CROP_FIELD_INFO_DATA &data );
BOOL FromJsonDT_CROP_FIELD_INFO_DATA(const string &strData, DT_CROP_FIELD_INFO_DATA &data);
BOOL FromJsonDT_CROP_FIELD_INFO_DATA(Json::Value &root, DT_CROP_FIELD_INFO_DATA &data);
INT32 EncodeDT_CROP_MORE_INFO_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_CROP_MORE_INFO_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_CROP_MORE_INFO_DATA( DT_CROP_MORE_INFO_DATA &data );
BOOL FromJsonDT_CROP_MORE_INFO_DATA(const string &strData, DT_CROP_MORE_INFO_DATA &data);
BOOL FromJsonDT_CROP_MORE_INFO_DATA(Json::Value &root, DT_CROP_MORE_INFO_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_CROP_REQ( PKT_CLIGS_OPEN_CROP_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_CROP_REQ(const string &strData, PKT_CLIGS_OPEN_CROP_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_CROP_REQ(Json::Value &root, PKT_CLIGS_OPEN_CROP_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_CROP_ACK( PKT_CLIGS_OPEN_CROP_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_CROP_ACK(const string &strData, PKT_CLIGS_OPEN_CROP_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_CROP_ACK(Json::Value &root, PKT_CLIGS_OPEN_CROP_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_CROP_FIELD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_FIELD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_CROP_FIELD_REQ( PKT_CLIGS_OPEN_CROP_FIELD_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_CROP_FIELD_REQ(const string &strData, PKT_CLIGS_OPEN_CROP_FIELD_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_CROP_FIELD_REQ(Json::Value &root, PKT_CLIGS_OPEN_CROP_FIELD_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_CROP_FIELD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_CROP_FIELD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_CROP_FIELD_ACK( PKT_CLIGS_OPEN_CROP_FIELD_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_CROP_FIELD_ACK(const string &strData, PKT_CLIGS_OPEN_CROP_FIELD_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_CROP_FIELD_ACK(Json::Value &root, PKT_CLIGS_OPEN_CROP_FIELD_ACK &data);
INT32 EncodePKT_CLIGS_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CROP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CROP_REQ( PKT_CLIGS_CROP_REQ &data );
BOOL FromJsonPKT_CLIGS_CROP_REQ(const string &strData, PKT_CLIGS_CROP_REQ &data);
BOOL FromJsonPKT_CLIGS_CROP_REQ(Json::Value &root, PKT_CLIGS_CROP_REQ &data);
INT32 EncodePKT_CLIGS_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CROP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CROP_ACK( PKT_CLIGS_CROP_ACK &data );
BOOL FromJsonPKT_CLIGS_CROP_ACK(const string &strData, PKT_CLIGS_CROP_ACK &data);
BOOL FromJsonPKT_CLIGS_CROP_ACK(Json::Value &root, PKT_CLIGS_CROP_ACK &data);
INT32 EncodePKT_CLIGS_GAIN_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GAIN_CROP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GAIN_CROP_REQ( PKT_CLIGS_GAIN_CROP_REQ &data );
BOOL FromJsonPKT_CLIGS_GAIN_CROP_REQ(const string &strData, PKT_CLIGS_GAIN_CROP_REQ &data);
BOOL FromJsonPKT_CLIGS_GAIN_CROP_REQ(Json::Value &root, PKT_CLIGS_GAIN_CROP_REQ &data);
INT32 EncodePKT_CLIGS_GAIN_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GAIN_CROP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GAIN_CROP_ACK( PKT_CLIGS_GAIN_CROP_ACK &data );
BOOL FromJsonPKT_CLIGS_GAIN_CROP_ACK(const string &strData, PKT_CLIGS_GAIN_CROP_ACK &data);
BOOL FromJsonPKT_CLIGS_GAIN_CROP_ACK(Json::Value &root, PKT_CLIGS_GAIN_CROP_ACK &data);
INT32 EncodePKT_CLIGS_CLR_CROP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CROP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_CROP_REQ( PKT_CLIGS_CLR_CROP_REQ &data );
BOOL FromJsonPKT_CLIGS_CLR_CROP_REQ(const string &strData, PKT_CLIGS_CLR_CROP_REQ &data);
BOOL FromJsonPKT_CLIGS_CLR_CROP_REQ(Json::Value &root, PKT_CLIGS_CLR_CROP_REQ &data);
INT32 EncodePKT_CLIGS_CLR_CROP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CLR_CROP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CLR_CROP_ACK( PKT_CLIGS_CLR_CROP_ACK &data );
BOOL FromJsonPKT_CLIGS_CLR_CROP_ACK(const string &strData, PKT_CLIGS_CLR_CROP_ACK &data);
BOOL FromJsonPKT_CLIGS_CLR_CROP_ACK(Json::Value &root, PKT_CLIGS_CLR_CROP_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_EXPERIENCE_REQ( PKT_CLIGS_OPEN_EXPERIENCE_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_EXPERIENCE_REQ(const string &strData, PKT_CLIGS_OPEN_EXPERIENCE_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_EXPERIENCE_REQ(Json::Value &root, PKT_CLIGS_OPEN_EXPERIENCE_REQ &data);
INT32 EncodeDT_EXPERIENCE_DRUG_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_EXPERIENCE_DRUG_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_EXPERIENCE_DRUG_DATA( DT_EXPERIENCE_DRUG_DATA &data );
BOOL FromJsonDT_EXPERIENCE_DRUG_DATA(const string &strData, DT_EXPERIENCE_DRUG_DATA &data);
BOOL FromJsonDT_EXPERIENCE_DRUG_DATA(Json::Value &root, DT_EXPERIENCE_DRUG_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_EXPERIENCE_ACK( PKT_CLIGS_OPEN_EXPERIENCE_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_EXPERIENCE_ACK(const string &strData, PKT_CLIGS_OPEN_EXPERIENCE_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_EXPERIENCE_ACK(Json::Value &root, PKT_CLIGS_OPEN_EXPERIENCE_ACK &data);
INT32 EncodePKT_CLIGS_USER_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USER_EXPERIENCE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_USER_EXPERIENCE_REQ( PKT_CLIGS_USER_EXPERIENCE_REQ &data );
BOOL FromJsonPKT_CLIGS_USER_EXPERIENCE_REQ(const string &strData, PKT_CLIGS_USER_EXPERIENCE_REQ &data);
BOOL FromJsonPKT_CLIGS_USER_EXPERIENCE_REQ(Json::Value &root, PKT_CLIGS_USER_EXPERIENCE_REQ &data);
INT32 EncodePKT_CLIGS_USER_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_USER_EXPERIENCE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_USER_EXPERIENCE_ACK( PKT_CLIGS_USER_EXPERIENCE_ACK &data );
BOOL FromJsonPKT_CLIGS_USER_EXPERIENCE_ACK(const string &strData, PKT_CLIGS_USER_EXPERIENCE_ACK &data);
BOOL FromJsonPKT_CLIGS_USER_EXPERIENCE_ACK(Json::Value &root, PKT_CLIGS_USER_EXPERIENCE_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_SCIENCE_BUILD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SCIENCE_BUILD_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ( PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ(const string &strData, PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_SCIENCE_BUILD_REQ(Json::Value &root, PKT_CLIGS_OPEN_SCIENCE_BUILD_REQ &data);
INT32 EncodeDT_SCIENCE_ALREADY_OPEN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_ALREADY_OPEN_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SCIENCE_ALREADY_OPEN_DATA( DT_SCIENCE_ALREADY_OPEN_DATA &data );
BOOL FromJsonDT_SCIENCE_ALREADY_OPEN_DATA(const string &strData, DT_SCIENCE_ALREADY_OPEN_DATA &data);
BOOL FromJsonDT_SCIENCE_ALREADY_OPEN_DATA(Json::Value &root, DT_SCIENCE_ALREADY_OPEN_DATA &data);
INT32 EncodeDT_SCIENCE_WILL_OPEN_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SCIENCE_WILL_OPEN_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_SCIENCE_WILL_OPEN_DATA( DT_SCIENCE_WILL_OPEN_DATA &data );
BOOL FromJsonDT_SCIENCE_WILL_OPEN_DATA(const string &strData, DT_SCIENCE_WILL_OPEN_DATA &data);
BOOL FromJsonDT_SCIENCE_WILL_OPEN_DATA(Json::Value &root, DT_SCIENCE_WILL_OPEN_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_SCIENCE_BUILD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_SCIENCE_BUILD_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_SCIENCE_BUILD_ACK( PKT_CLIGS_OPEN_SCIENCE_BUILD_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_SCIENCE_BUILD_ACK(const string &strData, PKT_CLIGS_OPEN_SCIENCE_BUILD_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_SCIENCE_BUILD_ACK(Json::Value &root, PKT_CLIGS_OPEN_SCIENCE_BUILD_ACK &data);
INT32 EncodePKT_CLIGS_UPDATE_SCIENCE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_SCIENCE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPDATE_SCIENCE_REQ( PKT_CLIGS_UPDATE_SCIENCE_REQ &data );
BOOL FromJsonPKT_CLIGS_UPDATE_SCIENCE_REQ(const string &strData, PKT_CLIGS_UPDATE_SCIENCE_REQ &data);
BOOL FromJsonPKT_CLIGS_UPDATE_SCIENCE_REQ(Json::Value &root, PKT_CLIGS_UPDATE_SCIENCE_REQ &data);
INT32 EncodePKT_CLIGS_UPDATE_SCIENCE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_SCIENCE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPDATE_SCIENCE_ACK( PKT_CLIGS_UPDATE_SCIENCE_ACK &data );
BOOL FromJsonPKT_CLIGS_UPDATE_SCIENCE_ACK(const string &strData, PKT_CLIGS_UPDATE_SCIENCE_ACK &data);
BOOL FromJsonPKT_CLIGS_UPDATE_SCIENCE_ACK(Json::Value &root, PKT_CLIGS_UPDATE_SCIENCE_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_MELT_EQUIP_REQ( PKT_CLIGS_OPEN_MELT_EQUIP_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_MELT_EQUIP_REQ(const string &strData, PKT_CLIGS_OPEN_MELT_EQUIP_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_MELT_EQUIP_REQ(Json::Value &root, PKT_CLIGS_OPEN_MELT_EQUIP_REQ &data);
INT32 EncodePKT_CLIGS_OPEN_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_MELT_EQUIP_ACK( PKT_CLIGS_OPEN_MELT_EQUIP_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_MELT_EQUIP_ACK(const string &strData, PKT_CLIGS_OPEN_MELT_EQUIP_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_MELT_EQUIP_ACK(Json::Value &root, PKT_CLIGS_OPEN_MELT_EQUIP_ACK &data);
INT32 EncodeDT_MELT_EQUIP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_MELT_EQUIP_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_MELT_EQUIP_DATA( DT_MELT_EQUIP_DATA &data );
BOOL FromJsonDT_MELT_EQUIP_DATA(const string &strData, DT_MELT_EQUIP_DATA &data);
BOOL FromJsonDT_MELT_EQUIP_DATA(Json::Value &root, DT_MELT_EQUIP_DATA &data);
INT32 EncodePKT_CLIGS_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MELT_EQUIP_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_MELT_EQUIP_REQ( PKT_CLIGS_MELT_EQUIP_REQ &data );
BOOL FromJsonPKT_CLIGS_MELT_EQUIP_REQ(const string &strData, PKT_CLIGS_MELT_EQUIP_REQ &data);
BOOL FromJsonPKT_CLIGS_MELT_EQUIP_REQ(Json::Value &root, PKT_CLIGS_MELT_EQUIP_REQ &data);
INT32 EncodePKT_CLIGS_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_MELT_EQUIP_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_MELT_EQUIP_ACK( PKT_CLIGS_MELT_EQUIP_ACK &data );
BOOL FromJsonPKT_CLIGS_MELT_EQUIP_ACK(const string &strData, PKT_CLIGS_MELT_EQUIP_ACK &data);
BOOL FromJsonPKT_CLIGS_MELT_EQUIP_ACK(Json::Value &root, PKT_CLIGS_MELT_EQUIP_ACK &data);
INT32 EncodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ( PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ &data );
BOOL FromJsonPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ(const string &strData, PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ &data);
BOOL FromJsonPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ(Json::Value &root, PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_REQ &data);
INT32 EncodeDT_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UPDATE_ATTR_DATA( DT_UPDATE_ATTR_DATA &data );
BOOL FromJsonDT_UPDATE_ATTR_DATA(const string &strData, DT_UPDATE_ATTR_DATA &data);
BOOL FromJsonDT_UPDATE_ATTR_DATA(Json::Value &root, DT_UPDATE_ATTR_DATA &data);
INT32 EncodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK( PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK &data );
BOOL FromJsonPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK(const string &strData, PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK &data);
BOOL FromJsonPKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK(Json::Value &root, PKT_CLIGS_OPEN_UPDATE_ATTR_PANNEL_ACK &data);
INT32 EncodePKT_CLIGS_UPDATE_ATTR_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_ATTR_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPDATE_ATTR_REQ( PKT_CLIGS_UPDATE_ATTR_REQ &data );
BOOL FromJsonPKT_CLIGS_UPDATE_ATTR_REQ(const string &strData, PKT_CLIGS_UPDATE_ATTR_REQ &data);
BOOL FromJsonPKT_CLIGS_UPDATE_ATTR_REQ(Json::Value &root, PKT_CLIGS_UPDATE_ATTR_REQ &data);
INT32 EncodeDT_UPDATED_ATTR_BATTLE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_UPDATED_ATTR_BATTLE_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonDT_UPDATED_ATTR_BATTLE_DATA( DT_UPDATED_ATTR_BATTLE_DATA &data );
BOOL FromJsonDT_UPDATED_ATTR_BATTLE_DATA(const string &strData, DT_UPDATED_ATTR_BATTLE_DATA &data);
BOOL FromJsonDT_UPDATED_ATTR_BATTLE_DATA(Json::Value &root, DT_UPDATED_ATTR_BATTLE_DATA &data);
INT32 EncodePKT_CLIGS_UPDATE_ATTR_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_UPDATE_ATTR_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_UPDATE_ATTR_ACK( PKT_CLIGS_UPDATE_ATTR_ACK &data );
BOOL FromJsonPKT_CLIGS_UPDATE_ATTR_ACK(const string &strData, PKT_CLIGS_UPDATE_ATTR_ACK &data);
BOOL FromJsonPKT_CLIGS_UPDATE_ATTR_ACK(Json::Value &root, PKT_CLIGS_UPDATE_ATTR_ACK &data);
INT32 EncodePKT_CLIGS_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_COMPOSE_REQ( PKT_CLIGS_COMPOSE_REQ &data );
BOOL FromJsonPKT_CLIGS_COMPOSE_REQ(const string &strData, PKT_CLIGS_COMPOSE_REQ &data);
BOOL FromJsonPKT_CLIGS_COMPOSE_REQ(Json::Value &root, PKT_CLIGS_COMPOSE_REQ &data);
INT32 EncodePKT_CLIGS_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_COMPOSE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_COMPOSE_ACK( PKT_CLIGS_COMPOSE_ACK &data );
BOOL FromJsonPKT_CLIGS_COMPOSE_ACK(const string &strData, PKT_CLIGS_COMPOSE_ACK &data);
BOOL FromJsonPKT_CLIGS_COMPOSE_ACK(Json::Value &root, PKT_CLIGS_COMPOSE_ACK &data);
INT32 EncodePKT_CLIGS_REFRESH_ELITE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_ELITE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_REFRESH_ELITE_REQ( PKT_CLIGS_REFRESH_ELITE_REQ &data );
BOOL FromJsonPKT_CLIGS_REFRESH_ELITE_REQ(const string &strData, PKT_CLIGS_REFRESH_ELITE_REQ &data);
BOOL FromJsonPKT_CLIGS_REFRESH_ELITE_REQ(Json::Value &root, PKT_CLIGS_REFRESH_ELITE_REQ &data);
INT32 EncodePKT_CLIGS_REFRESH_ELITE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_REFRESH_ELITE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_REFRESH_ELITE_ACK( PKT_CLIGS_REFRESH_ELITE_ACK &data );
BOOL FromJsonPKT_CLIGS_REFRESH_ELITE_ACK(const string &strData, PKT_CLIGS_REFRESH_ELITE_ACK &data);
BOOL FromJsonPKT_CLIGS_REFRESH_ELITE_ACK(Json::Value &root, PKT_CLIGS_REFRESH_ELITE_ACK &data);
INT32 EncodePKT_CLIGS_ONHOOK_ELITE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_ELITE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ONHOOK_ELITE_REQ( PKT_CLIGS_ONHOOK_ELITE_REQ &data );
BOOL FromJsonPKT_CLIGS_ONHOOK_ELITE_REQ(const string &strData, PKT_CLIGS_ONHOOK_ELITE_REQ &data);
BOOL FromJsonPKT_CLIGS_ONHOOK_ELITE_REQ(Json::Value &root, PKT_CLIGS_ONHOOK_ELITE_REQ &data);
INT32 EncodePKT_CLIGS_ONHOOK_ELITE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_ONHOOK_ELITE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_ONHOOK_ELITE_ACK( PKT_CLIGS_ONHOOK_ELITE_ACK &data );
BOOL FromJsonPKT_CLIGS_ONHOOK_ELITE_ACK(const string &strData, PKT_CLIGS_ONHOOK_ELITE_ACK &data);
BOOL FromJsonPKT_CLIGS_ONHOOK_ELITE_ACK(Json::Value &root, PKT_CLIGS_ONHOOK_ELITE_ACK &data);
INT32 EncodePKT_CLIGS_GET_ITEM_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ITEM_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_ITEM_REQ( PKT_CLIGS_GET_ITEM_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_ITEM_REQ(const string &strData, PKT_CLIGS_GET_ITEM_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_ITEM_REQ(Json::Value &root, PKT_CLIGS_GET_ITEM_REQ &data);
INT32 EncodePKT_CLIGS_GET_ITEM_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ITEM_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_ITEM_ACK( PKT_CLIGS_GET_ITEM_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_ITEM_ACK(const string &strData, PKT_CLIGS_GET_ITEM_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_ITEM_ACK(Json::Value &root, PKT_CLIGS_GET_ITEM_ACK &data);
INT32 EncodePKT_CLIGS_GET_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_ORDER_ID_REQ( PKT_CLIGS_GET_ORDER_ID_REQ &data );
BOOL FromJsonPKT_CLIGS_GET_ORDER_ID_REQ(const string &strData, PKT_CLIGS_GET_ORDER_ID_REQ &data);
BOOL FromJsonPKT_CLIGS_GET_ORDER_ID_REQ(Json::Value &root, PKT_CLIGS_GET_ORDER_ID_REQ &data);
INT32 EncodePKT_CLIGS_GET_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_GET_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_GET_ORDER_ID_ACK( PKT_CLIGS_GET_ORDER_ID_ACK &data );
BOOL FromJsonPKT_CLIGS_GET_ORDER_ID_ACK(const string &strData, PKT_CLIGS_GET_ORDER_ID_ACK &data);
BOOL FromJsonPKT_CLIGS_GET_ORDER_ID_ACK(Json::Value &root, PKT_CLIGS_GET_ORDER_ID_ACK &data);
INT32 EncodePKT_CLIGS_EXCHANGE_RESULT(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_EXCHANGE_RESULT(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_EXCHANGE_RESULT( PKT_CLIGS_EXCHANGE_RESULT &data );
BOOL FromJsonPKT_CLIGS_EXCHANGE_RESULT(const string &strData, PKT_CLIGS_EXCHANGE_RESULT &data);
BOOL FromJsonPKT_CLIGS_EXCHANGE_RESULT(Json::Value &root, PKT_CLIGS_EXCHANGE_RESULT &data);
INT32 EncodePKT_CLIGS_SEED_COMPOSE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEED_COMPOSE_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SEED_COMPOSE_REQ( PKT_CLIGS_SEED_COMPOSE_REQ &data );
BOOL FromJsonPKT_CLIGS_SEED_COMPOSE_REQ(const string &strData, PKT_CLIGS_SEED_COMPOSE_REQ &data);
BOOL FromJsonPKT_CLIGS_SEED_COMPOSE_REQ(Json::Value &root, PKT_CLIGS_SEED_COMPOSE_REQ &data);
INT32 EncodePKT_CLIGS_SEED_COMPOSE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_SEED_COMPOSE_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_SEED_COMPOSE_ACK( PKT_CLIGS_SEED_COMPOSE_ACK &data );
BOOL FromJsonPKT_CLIGS_SEED_COMPOSE_ACK(const string &strData, PKT_CLIGS_SEED_COMPOSE_ACK &data);
BOOL FromJsonPKT_CLIGS_SEED_COMPOSE_ACK(Json::Value &root, PKT_CLIGS_SEED_COMPOSE_ACK &data);
INT32 EncodePKT_CLIGS_CHAT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHAT_REQ(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CHAT_REQ( PKT_CLIGS_CHAT_REQ &data );
BOOL FromJsonPKT_CLIGS_CHAT_REQ(const string &strData, PKT_CLIGS_CHAT_REQ &data);
BOOL FromJsonPKT_CLIGS_CHAT_REQ(Json::Value &root, PKT_CLIGS_CHAT_REQ &data);
INT32 EncodeCHAT_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeCHAT_DATA(void *pHost, CNetData* poNetData);
Json::Value GetJsonCHAT_DATA( CHAT_DATA &data );
BOOL FromJsonCHAT_DATA(const string &strData, CHAT_DATA &data);
BOOL FromJsonCHAT_DATA(Json::Value &root, CHAT_DATA &data);
INT32 EncodePKT_CLIGS_CHAT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHAT_ACK(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CHAT_ACK( PKT_CLIGS_CHAT_ACK &data );
BOOL FromJsonPKT_CLIGS_CHAT_ACK(const string &strData, PKT_CLIGS_CHAT_ACK &data);
BOOL FromJsonPKT_CLIGS_CHAT_ACK(Json::Value &root, PKT_CLIGS_CHAT_ACK &data);
INT32 EncodePKT_CLIGS_CHAT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGS_CHAT_NTF(void *pHost, CNetData* poNetData);
Json::Value GetJsonPKT_CLIGS_CHAT_NTF( PKT_CLIGS_CHAT_NTF &data );
BOOL FromJsonPKT_CLIGS_CHAT_NTF(const string &strData, PKT_CLIGS_CHAT_NTF &data);
BOOL FromJsonPKT_CLIGS_CHAT_NTF(Json::Value &root, PKT_CLIGS_CHAT_NTF &data);
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
