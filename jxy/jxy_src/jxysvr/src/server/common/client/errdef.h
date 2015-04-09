#ifndef __ERRDEF_H__
#define __ERRDEF_H__

namespace ERR_COMMON 
{
	static const unsigned int ID_OTHER_ERR = 110;
	static const char* STR_OTHER_ERR = "其他错误";

	static const unsigned int ID_VIP_LEVEL_NOT_REACH = 111;
	static const char* STR_VIP_LEVEL_NOT_REACH = "未达到开放该功能的VIP等级";

	static const unsigned int ID_COIN_NOT_ENOUGH = 112;
	static const char* STR_COIN_NOT_ENOUGH = "铜钱不足";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 113;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_FACTION_IS_EXCEPTION = 114;
	static const char* STR_FACTION_IS_EXCEPTION = "该门派异常，请联系GM进行解决";

	static const char* STR_NAMESPACE = "ERR_COMMON";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_OTHER_ERR
	,STR_VIP_LEVEL_NOT_REACH
	,STR_COIN_NOT_ENOUGH
	,STR_GOLD_NOT_ENOUGH
	,STR_FACTION_IS_EXCEPTION
	};
}

namespace ERR_GM_PARM_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLAYER_INIT_ERR = 1;
	static const char* STR_PLAYER_INIT_ERR = "玩家初始化失败";

	static const unsigned int ID_PLAYER_NOT_EXIT_ERR = 2;
	static const char* STR_PLAYER_NOT_EXIT_ERR = "玩家不存在";

	static const unsigned int ID_TYPE_NOT_EXIT_ERR = 3;
	static const char* STR_TYPE_NOT_EXIT_ERR = "类型不存在";

	static const unsigned int ID_OTHER_ERR = 4;
	static const char* STR_OTHER_ERR = "其他错误";

	static const unsigned int ID_PARAM_ERR = 5;
	static const char* STR_PARAM_ERR = "参数错误";

	static const unsigned int ID_FIND_NO = 6;
	static const char* STR_FIND_NO = "命令没有找到";

	static const unsigned int ID_NOT_DATA = 7;
	static const char* STR_NOT_DATA = "数据为空";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 8;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_PLAYER_IS_NOT_ONLINE = 9;
	static const char* STR_PLAYER_IS_NOT_ONLINE = "玩家当前不在线";

	static const unsigned int ID_INSTANCE_PASS = 10;
	static const char* STR_INSTANCE_PASS = "副本通关";

	static const unsigned int ID_BUILD_NO_OPEN = 11;
	static const char* STR_BUILD_NO_OPEN = "建筑没有开放";

	static const unsigned int ID_BAG_IS_NOT_ENOUTH = 12;
	static const char* STR_BAG_IS_NOT_ENOUTH = "背包空间不足";

	static const unsigned int ID_STUDY_IS_NOT_OPEN = 13;
	static const char* STR_STUDY_IS_NOT_OPEN = "卦象未开放";

	static const unsigned int ID_STUDY_BAG_IS_NOT_ENOUGH = 14;
	static const char* STR_STUDY_BAG_IS_NOT_ENOUGH = "卦象背包空间不足";

	static const unsigned int ID_DEL_ITEM_NO_ENOUGH = 15;
	static const char* STR_DEL_ITEM_NO_ENOUGH = "要删除的道具数量不足";

	static const unsigned int ID_FACTION_NO_OPEN = 16;
	static const char* STR_FACTION_NO_OPEN = "未加入帮派";

	static const unsigned int ID_OVER_TIME = 17;
	static const char* STR_OVER_TIME = "请求超时";

	static const unsigned int ID_REDUPLICATIVE = 18;
	static const char* STR_REDUPLICATIVE = "重复请求";

	static const unsigned int ID_NO_ACTIVITY = 19;
	static const char* STR_NO_ACTIVITY = "当前没有活动";

	static const unsigned int ID_NO_THIS_ACTIVITY = 20;
	static const char* STR_NO_THIS_ACTIVITY = "当前不是这个活动";

	static const unsigned int ID_ACTIVITY_NO_START_UP = 21;
	static const char* STR_ACTIVITY_NO_START_UP = "活动还未开启";

	static const unsigned int ID_ACTIVITY_OVER_TIME = 22;
	static const char* STR_ACTIVITY_OVER_TIME = "活动已经结束";

	static const unsigned int ID_INSTANCE_ID_FAULT = 23;
	static const char* STR_INSTANCE_ID_FAULT = "副本ID错误";

	static const unsigned int ID_INNER_BATTLE_FAULT = 24;
	static const char* STR_INNER_BATTLE_FAULT = "进入战斗失败";

	static const unsigned int ID_PLAYER_NOT_MEMORT = 25;
	static const char* STR_PLAYER_NOT_MEMORT = "玩家不在内存中";

	static const unsigned int ID_SOURCE_INVALID = 26;
	static const char* STR_SOURCE_INVALID = "资源合法性检查失败";

	static const unsigned int ID_FILE_NO_EXIST = 27;
	static const char* STR_FILE_NO_EXIST = "回档的文件不存在";

	static const unsigned int ID_DATA_GET_ERR = 28;
	static const char* STR_DATA_GET_ERR = "数据解析错误";

	static const unsigned int ID_DATA_SAVE_ERR = 29;
	static const char* STR_DATA_SAVE_ERR = "数据保存错误";

	static const unsigned int ID_DATA_IS_NULL = 30;
	static const char* STR_DATA_IS_NULL = "没有要回档的数据";

	static const unsigned int ID_HERO_NOT_EXIST_ERR = 31;
	static const char* STR_HERO_NOT_EXIST_ERR = "伙伴不存在";

	static const unsigned int ID_RECUIT_NOT_EXIST_ERR = 32;
	static const char* STR_RECUIT_NOT_EXIST_ERR = "招募馆不存在";

	static const unsigned int ID_UPPER_LIMIT_ERR = 33;
	static const char* STR_UPPER_LIMIT_ERR = "所携带伙伴数量达到上限";

	static const unsigned int ID_NOT_ENOUGH_PLAYER_LEVEL = 34;
	static const char* STR_NOT_ENOUGH_PLAYER_LEVEL = "主角等级未达到";

	static const unsigned int ID_CANNOT_RECUIT_COACH = 35;
	static const char* STR_CANNOT_RECUIT_COACH = "不能招募主角";

	static const unsigned int ID_TOO_LONG_MESS = 36;
	static const char* STR_TOO_LONG_MESS = "公告内容太长,不能超过包括标点符号在内的250个汉字";

	static const char* STR_NAMESPACE = "ERR_GM_PARM_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLAYER_INIT_ERR
	,STR_PLAYER_NOT_EXIT_ERR
	,STR_TYPE_NOT_EXIT_ERR
	,STR_OTHER_ERR
	,STR_PARAM_ERR
	,STR_FIND_NO
	,STR_NOT_DATA
	,STR_GOLD_NOT_ENOUGH
	,STR_PLAYER_IS_NOT_ONLINE
	,STR_INSTANCE_PASS
	,STR_BUILD_NO_OPEN
	,STR_BAG_IS_NOT_ENOUTH
	,STR_STUDY_IS_NOT_OPEN
	,STR_STUDY_BAG_IS_NOT_ENOUGH
	,STR_DEL_ITEM_NO_ENOUGH
	,STR_FACTION_NO_OPEN
	,STR_OVER_TIME
	,STR_REDUPLICATIVE
	,STR_NO_ACTIVITY
	,STR_NO_THIS_ACTIVITY
	,STR_ACTIVITY_NO_START_UP
	,STR_ACTIVITY_OVER_TIME
	,STR_INSTANCE_ID_FAULT
	,STR_INNER_BATTLE_FAULT
	,STR_PLAYER_NOT_MEMORT
	,STR_SOURCE_INVALID
	,STR_FILE_NO_EXIST
	,STR_DATA_GET_ERR
	,STR_DATA_SAVE_ERR
	,STR_DATA_IS_NULL
	,STR_HERO_NOT_EXIST_ERR
	,STR_RECUIT_NOT_EXIST_ERR
	,STR_UPPER_LIMIT_ERR
	,STR_NOT_ENOUGH_PLAYER_LEVEL
	,STR_CANNOT_RECUIT_COACH
	,STR_TOO_LONG_MESS
	};
}

namespace ERR_LOGIN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PWD_ERR = 1;
	static const char* STR_PWD_ERR = "密码错误";

	static const unsigned int ID_ALL_EMPTY_ERR = 2;
	static const char* STR_ALL_EMPTY_ERR = "用户名和设备号都为空";

	static const unsigned int ID_INVALID_CAREER = 3;
	static const char* STR_INVALID_CAREER = "无效职业";

	static const unsigned int ID_INVALID_CHARACTER = 4;
	static const char* STR_INVALID_CHARACTER = "无效字符";

	static const unsigned int ID_INVALID_ZONEID = 5;
	static const char* STR_INVALID_ZONEID = "无效区号";

	static const unsigned int ID_REACH_MAX_PLAYER_NUM = 6;
	static const char* STR_REACH_MAX_PLAYER_NUM = "玩家个数已达上限";

	static const unsigned int ID_PLAYER_NOT_EXIT = 7;
	static const char* STR_PLAYER_NOT_EXIT = "玩家不存在";

	static const unsigned int ID_VERSION_NEED_UPDATE = 8;
	static const char* STR_VERSION_NEED_UPDATE = "版本需更新";

	static const unsigned int ID_INVALID_USER = 9;
	static const char* STR_INVALID_USER = "无效用户";

	static const unsigned int ID_LS_SERVER_NOT_STARTUP = 10;
	static const char* STR_LS_SERVER_NOT_STARTUP = "认证服务器未开启";

	static const unsigned int ID_GS_SERVER_NOT_STARTUP = 11;
	static const char* STR_GS_SERVER_NOT_STARTUP = "游戏服务器未开启";

	static const unsigned int ID_PLAYER_NOT_BIND = 12;
	static const char* STR_PLAYER_NOT_BIND = "账号未绑定";

	static const unsigned int ID_FORBID_CREATE_NEW_USER = 13;
	static const char* STR_FORBID_CREATE_NEW_USER = "该区禁止创建新用户";

	static const unsigned int ID_INVALID_USER_TYPE = 14;
	static const char* STR_INVALID_USER_TYPE = "无效用户类型";

	static const unsigned int ID_SYSTEM_MAINTAIN = 15;
	static const char* STR_SYSTEM_MAINTAIN = "系统正在维护，请稍候再试";

	static const unsigned int ID_INVALID_STATE = 16;
	static const char* STR_INVALID_STATE = "状态无效";

	static const unsigned int ID_USERNAME_NOT_EXIT = 17;
	static const char* STR_USERNAME_NOT_EXIT = "用户不存在";

	static const unsigned int ID_FORBID_LOGIN = 18;
	static const char* STR_FORBID_LOGIN = "玩家被禁止登录";

	static const unsigned int ID_FORBID_REGISTERED = 19;
	static const char* STR_FORBID_REGISTERED = "禁止注册新用户";

	static const unsigned int ID_LOGIN_TIME_OUT = 20;
	static const char* STR_LOGIN_TIME_OUT = "登录超时，请重试";

	static const char* STR_NAMESPACE = "ERR_LOGIN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PWD_ERR
	,STR_ALL_EMPTY_ERR
	,STR_INVALID_CAREER
	,STR_INVALID_CHARACTER
	,STR_INVALID_ZONEID
	,STR_REACH_MAX_PLAYER_NUM
	,STR_PLAYER_NOT_EXIT
	,STR_VERSION_NEED_UPDATE
	,STR_INVALID_USER
	,STR_LS_SERVER_NOT_STARTUP
	,STR_GS_SERVER_NOT_STARTUP
	,STR_PLAYER_NOT_BIND
	,STR_FORBID_CREATE_NEW_USER
	,STR_INVALID_USER_TYPE
	,STR_SYSTEM_MAINTAIN
	,STR_INVALID_STATE
	,STR_USERNAME_NOT_EXIT
	,STR_FORBID_LOGIN
	,STR_FORBID_REGISTERED
	,STR_LOGIN_TIME_OUT
	};
}

namespace ERR_ENTER_GS 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ENTER_FAILED = 1;
	static const char* STR_ENTER_FAILED = "进入游戏失败";

	static const unsigned int ID_INVALID_CAREER = 2;
	static const char* STR_INVALID_CAREER = "无效职业";

	static const unsigned int ID_UNMATCH_PARAM = 3;
	static const char* STR_UNMATCH_PARAM = "参数不匹配";

	static const unsigned int ID_INVALID_STATE = 4;
	static const char* STR_INVALID_STATE = "无效状态";

	static const unsigned int ID_SAVE_DATA_ERROR = 5;
	static const char* STR_SAVE_DATA_ERROR = "账号出现异常，错误码1，请联系GM";

	static const char* STR_NAMESPACE = "ERR_ENTER_GS";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ENTER_FAILED
	,STR_INVALID_CAREER
	,STR_UNMATCH_PARAM
	,STR_INVALID_STATE
	,STR_SAVE_DATA_ERROR
	};
}

namespace ERR_ENTER_INSTANCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INDEX_ERR = 1;
	static const char* STR_INDEX_ERR = "错误序号";

	static const unsigned int ID_REENTER_ERR = 2;
	static const char* STR_REENTER_ERR = "重复进入";

	static const unsigned int ID_LEVEL_ERR = 3;
	static const char* STR_LEVEL_ERR = "等级不足";

	static const unsigned int ID_PHYSTRENGTH_ERR = 4;
	static const char* STR_PHYSTRENGTH_ERR = "体力不足";

	static const unsigned int ID_NOT_UNLOCK_ERR = 5;
	static const char* STR_NOT_UNLOCK_ERR = "未解锁";

	static const unsigned int ID_ELITE_ONLY_ONE_PER_DAY_ERR = 6;
	static const char* STR_ELITE_ONLY_ONE_PER_DAY_ERR = "已挑战";

	static const unsigned int ID_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR = 7;
	static const char* STR_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR = "你已通关最后一层塔";

	static const unsigned int ID_NOT_UNLOCK_LEVEL_ERR = 8;
	static const char* STR_NOT_UNLOCK_LEVEL_ERR = "等级未达到，未开放";

	static const unsigned int ID_NOT_TIMES_OUT_BOUND = 9;
	static const char* STR_NOT_TIMES_OUT_BOUND = "该副本已无挑战次数";

	static const unsigned int ID_NOT_REFREASH = 10;
	static const char* STR_NOT_REFREASH = "请新转生";

	static const char* STR_NAMESPACE = "ERR_ENTER_INSTANCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INDEX_ERR
	,STR_REENTER_ERR
	,STR_LEVEL_ERR
	,STR_PHYSTRENGTH_ERR
	,STR_NOT_UNLOCK_ERR
	,STR_ELITE_ONLY_ONE_PER_DAY_ERR
	,STR_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR
	,STR_NOT_UNLOCK_LEVEL_ERR
	,STR_NOT_TIMES_OUT_BOUND
	,STR_NOT_REFREASH
	};
}

namespace ERR_BATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INDEX_ERR = 1;
	static const char* STR_INDEX_ERR = "错误序号";

	static const unsigned int ID_HAVE_BATTLE_ERR = 2;
	static const char* STR_HAVE_BATTLE_ERR = "已经打过";

	static const unsigned int ID_MYSELF_DEAD_ERR = 3;
	static const char* STR_MYSELF_DEAD_ERR = "我方已死亡";

	static const unsigned int ID_ENEMY_DEAD_ERR = 4;
	static const char* STR_ENEMY_DEAD_ERR = "敌人已死亡";

	static const unsigned int ID_NOT_IN_INSTANCE_ERR = 5;
	static const char* STR_NOT_IN_INSTANCE_ERR = "不在副本中";

	static const unsigned int ID_CLIMBTOWER_ONE_PER_DAY_ERR = 6;
	static const char* STR_CLIMBTOWER_ONE_PER_DAY_ERR = "爬塔每天只能爬一次";

	static const unsigned int ID_REENTER_ERR = 7;
	static const char* STR_REENTER_ERR = "重复进入";

	static const unsigned int ID_LEVEL_ERR = 8;
	static const char* STR_LEVEL_ERR = "等级不足";

	static const unsigned int ID_PHYSTRENGTH_ERR = 9;
	static const char* STR_PHYSTRENGTH_ERR = "体力不足";

	static const unsigned int ID_NOT_UNLOCK_ERR = 10;
	static const char* STR_NOT_UNLOCK_ERR = "未解锁";

	static const unsigned int ID_ELITE_ONLY_ONE_PER_DAY_ERR = 11;
	static const char* STR_ELITE_ONLY_ONE_PER_DAY_ERR = "已挑战";

	static const unsigned int ID_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR = 12;
	static const char* STR_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR = "你已通关最后一层塔";

	static const unsigned int ID_NOT_UNLOCK_LEVEL_ERR = 13;
	static const char* STR_NOT_UNLOCK_LEVEL_ERR = "等级未达到，未开放";

	static const unsigned int ID_BAG_FULL = 14;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_MAX_HIGH_FLOOR = 15;
	static const char* STR_MAX_HIGH_FLOOR = "已达到最顶层";

	static const char* STR_NAMESPACE = "ERR_BATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INDEX_ERR
	,STR_HAVE_BATTLE_ERR
	,STR_MYSELF_DEAD_ERR
	,STR_ENEMY_DEAD_ERR
	,STR_NOT_IN_INSTANCE_ERR
	,STR_CLIMBTOWER_ONE_PER_DAY_ERR
	,STR_REENTER_ERR
	,STR_LEVEL_ERR
	,STR_PHYSTRENGTH_ERR
	,STR_NOT_UNLOCK_ERR
	,STR_ELITE_ONLY_ONE_PER_DAY_ERR
	,STR_CLIMBTOWER_ONLY_ONE_PER_DAY_ERR
	,STR_NOT_UNLOCK_LEVEL_ERR
	,STR_BAG_FULL
	,STR_MAX_HIGH_FLOOR
	};
}

namespace ERR_EXIT_INSTANCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NONIN_ERR = 1;
	static const char* STR_NONIN_ERR = "不在副本中";

	static const char* STR_NAMESPACE = "ERR_EXIT_INSTANCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NONIN_ERR
	};
}

namespace ERR_STRENGTHEN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_REACH_MAX_LEVEL_ERR = 1;
	static const char* STR_REACH_MAX_LEVEL_ERR = "已经达到最大等级";

	static const unsigned int ID_NOT_EQUIP_ERR = 2;
	static const char* STR_NOT_EQUIP_ERR = "未装备";

	static const unsigned int ID_BUILD_NOT_EXIST_ERR = 3;
	static const char* STR_BUILD_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_IN_CD = 4;
	static const char* STR_IN_CD = "强化CD中";

	static const unsigned int ID_NOT_EXIST_ERR = 5;
	static const char* STR_NOT_EXIST_ERR = "装备不存在";

	static const unsigned int ID_OTHER_OWNER_ERR = 6;
	static const char* STR_OTHER_OWNER_ERR = "物品是别人的";

	static const unsigned int ID_HAVE_UPGRADE_ERR = 7;
	static const char* STR_HAVE_UPGRADE_ERR = "已经升过该等级";

	static const unsigned int ID_LEVEL_ERR = 8;
	static const char* STR_LEVEL_ERR = "等级错误(每次只能升1级，也不可以降级)";

	static const unsigned int ID_CANNT_OVER_PLAYER_LEVEL_ERR = 9;
	static const char* STR_CANNT_OVER_PLAYER_LEVEL_ERR = "不能超过玩家等级";

	static const unsigned int ID_LEVEL_MAX = 10;
	static const char* STR_LEVEL_MAX = "强化等级已经达到上限";

	static const unsigned int ID_CANNT_OVER_LEVEL_120 = 11;
	static const char* STR_CANNT_OVER_LEVEL_120 = "无法继续强化，最多只能强化到120级";

	static const char* STR_NAMESPACE = "ERR_STRENGTHEN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_REACH_MAX_LEVEL_ERR
	,STR_NOT_EQUIP_ERR
	,STR_BUILD_NOT_EXIST_ERR
	,STR_IN_CD
	,STR_NOT_EXIST_ERR
	,STR_OTHER_OWNER_ERR
	,STR_HAVE_UPGRADE_ERR
	,STR_LEVEL_ERR
	,STR_CANNT_OVER_PLAYER_LEVEL_ERR
	,STR_LEVEL_MAX
	,STR_CANNT_OVER_LEVEL_120
	};
}

namespace ERR_EQUIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_POS_HAVE_EQUIP_ERR = 1;
	static const char* STR_POS_HAVE_EQUIP_ERR = "该位置已有装备";

	static const unsigned int ID_NOT_REACH_LEVEL_ERR = 2;
	static const char* STR_NOT_REACH_LEVEL_ERR = "未达到装备等级要求";

	static const unsigned int ID_EQUIP_NOT_EXIST_ERR = 3;
	static const char* STR_EQUIP_NOT_EXIST_ERR = "装备不存在";

	static const unsigned int ID_HAVE_EQUIP_ERR = 4;
	static const char* STR_HAVE_EQUIP_ERR = "该装备已经穿戴";

	static const unsigned int ID_NOT_EQUIP_ERR = 5;
	static const char* STR_NOT_EQUIP_ERR = "未装备";

	static const unsigned int ID_POS_NOT_MATCH_ERR = 6;
	static const char* STR_POS_NOT_MATCH_ERR = "替换的装备位置不符";

	static const unsigned int ID_HERO_NOT_EXIST_ERR = 7;
	static const char* STR_HERO_NOT_EXIST_ERR = "伙伴不存在";

	static const unsigned int ID_BAG_FULL_ERR = 8;
	static const char* STR_BAG_FULL_ERR = "背包空间不足";

	static const unsigned int ID_NOT_IN_EQUIP_BAG_ERR = 9;
	static const char* STR_NOT_IN_EQUIP_BAG_ERR = "不是装备背包的装备";

	static const unsigned int ID_EQUIP_NUM_MISMATCH_ERR = 10;
	static const char* STR_EQUIP_NUM_MISMATCH_ERR = "穿戴个数不匹配";

	static const char* STR_NAMESPACE = "ERR_EQUIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_POS_HAVE_EQUIP_ERR
	,STR_NOT_REACH_LEVEL_ERR
	,STR_EQUIP_NOT_EXIST_ERR
	,STR_HAVE_EQUIP_ERR
	,STR_NOT_EQUIP_ERR
	,STR_POS_NOT_MATCH_ERR
	,STR_HERO_NOT_EXIST_ERR
	,STR_BAG_FULL_ERR
	,STR_NOT_IN_EQUIP_BAG_ERR
	,STR_EQUIP_NUM_MISMATCH_ERR
	};
}

namespace ERR_GET_SCENEINSTANCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SCENE_NOT_EXIST_ERR = 1;
	static const char* STR_SCENE_NOT_EXIST_ERR = "场景不存在";

	static const char* STR_NAMESPACE = "ERR_GET_SCENEINSTANCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SCENE_NOT_EXIST_ERR
	};
}

namespace ERR_OPEN_CLIMB_TOWER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_CLIMB_TOWER_NOT_OPEN_ERR = 1;
	static const char* STR_CLIMB_TOWER_NOT_OPEN_ERR = "场景不存在";

	static const unsigned int ID_CLIMB_TOWER_NOT_EXIST_ERR = 2;
	static const char* STR_CLIMB_TOWER_NOT_EXIST_ERR = "场景不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_CLIMB_TOWER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_CLIMB_TOWER_NOT_OPEN_ERR
	,STR_CLIMB_TOWER_NOT_EXIST_ERR
	};
}

namespace ERR_CROWN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_NOT_PRODUCE_ERR = 2;
	static const char* STR_NOT_PRODUCE_ERR = "不是生产建筑";

	static const unsigned int ID_TIME_ERR = 3;
	static const char* STR_TIME_ERR = "未到收获时间";

	static const unsigned int ID_Level0_ERR = 4;
	static const char* STR_Level0_ERR = "0级不可收获";

	static const char* STR_NAMESPACE = "ERR_CROWN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	,STR_NOT_PRODUCE_ERR
	,STR_TIME_ERR
	,STR_Level0_ERR
	};
}

namespace ERR_UPGRADE_BUILD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_REACH_MAX_LEVEL_ERR = 2;
	static const char* STR_REACH_MAX_LEVEL_ERR = "已达到最大等级";

	static const unsigned int ID_TIME_ERR = 3;
	static const char* STR_TIME_ERR = "未到升级时间";

	static const unsigned int ID_HAVE_UPGRADE_ERR = 4;
	static const char* STR_HAVE_UPGRADE_ERR = "已经升过该等级";

	static const unsigned int ID_LEVEL_ERR = 5;
	static const char* STR_LEVEL_ERR = "等级错误(每次只能升1级，也不可以降级)";

	static const unsigned int ID_CANNT_OVER_MAINTOWN_LEVEL_ERR = 6;
	static const char* STR_CANNT_OVER_MAINTOWN_LEVEL_ERR = "不能超过主城等级";

	static const unsigned int ID_CANNT_OVER_PLAYER_LEVEL_ERR = 7;
	static const char* STR_CANNT_OVER_PLAYER_LEVEL_ERR = "不能超过玩家等级";

	static const unsigned int ID_LEVEL_NOT_UNLOCK_ERR = 8;
	static const char* STR_LEVEL_NOT_UNLOCK_ERR = "该主城等级未解锁";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_BUILD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	,STR_REACH_MAX_LEVEL_ERR
	,STR_TIME_ERR
	,STR_HAVE_UPGRADE_ERR
	,STR_LEVEL_ERR
	,STR_CANNT_OVER_MAINTOWN_LEVEL_ERR
	,STR_CANNT_OVER_PLAYER_LEVEL_ERR
	,STR_LEVEL_NOT_UNLOCK_ERR
	};
}

namespace ERR_OPEN_SMITHY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_KIND_ERR = 2;
	static const char* STR_KIND_ERR = "建筑类型不对";

	static const unsigned int ID_OWNER_ERR = 3;
	static const char* STR_OWNER_ERR = "建筑主人不是自己";

	static const char* STR_NAMESPACE = "ERR_OPEN_SMITHY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	,STR_KIND_ERR
	,STR_OWNER_ERR
	};
}

namespace ERR_OPEN_RECUIT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_KIND_ERR = 2;
	static const char* STR_KIND_ERR = "建筑类型不对";

	static const unsigned int ID_OWNER_ERR = 3;
	static const char* STR_OWNER_ERR = "建筑主人不是自己";

	static const char* STR_NAMESPACE = "ERR_OPEN_RECUIT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	,STR_KIND_ERR
	,STR_OWNER_ERR
	};
}

namespace ERR_RECUIT_HERO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_NOT_EXIST_ERR = 1;
	static const char* STR_HERO_NOT_EXIST_ERR = "伙伴不存在";

	static const unsigned int ID_RECUIT_NOT_EXIST_ERR = 2;
	static const char* STR_RECUIT_NOT_EXIST_ERR = "招募馆不存在";

	static const unsigned int ID_UPPER_LIMIT_ERR = 3;
	static const char* STR_UPPER_LIMIT_ERR = "所携带伙伴数量达到上限";

	static const unsigned int ID_NOT_REACH_VISIT_NUM = 4;
	static const char* STR_NOT_REACH_VISIT_NUM = "好感度未达到";

	static const unsigned int ID_CANNOT_RECUIT_COACH = 5;
	static const char* STR_CANNOT_RECUIT_COACH = "不能招募主角";

	static const unsigned int ID_NOT_ENOUGH_JINGJIE_LEVEL = 6;
	static const char* STR_NOT_ENOUGH_JINGJIE_LEVEL = "境界等级未达到";

	static const unsigned int ID_NOT_ENOUGH_PLAYER_LEVEL = 7;
	static const char* STR_NOT_ENOUGH_PLAYER_LEVEL = "主角等级未达到";

	static const unsigned int ID_MUST_UPGRADE = 8;
	static const char* STR_MUST_UPGRADE = "该伙伴必须从已招募伙伴进阶，无法直接招募";

	static const unsigned int ID_MATERIAL_NOT_ENOUGH = 9;
	static const char* STR_MATERIAL_NOT_ENOUGH = "招募材料不足";

	static const char* STR_NAMESPACE = "ERR_RECUIT_HERO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_NOT_EXIST_ERR
	,STR_RECUIT_NOT_EXIST_ERR
	,STR_UPPER_LIMIT_ERR
	,STR_NOT_REACH_VISIT_NUM
	,STR_CANNOT_RECUIT_COACH
	,STR_NOT_ENOUGH_JINGJIE_LEVEL
	,STR_NOT_ENOUGH_PLAYER_LEVEL
	,STR_MUST_UPGRADE
	,STR_MATERIAL_NOT_ENOUGH
	};
}

namespace ERR_DISBAND_HERO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_NOT_EXIST_ERR = 1;
	static const char* STR_HERO_NOT_EXIST_ERR = "伙伴不存在";

	static const unsigned int ID_RECUIT_NOT_EXIST_ERR = 2;
	static const char* STR_RECUIT_NOT_EXIST_ERR = "招募馆不存在";

	static const unsigned int ID_COACH_CANNOT_DISBAND_ERR = 3;
	static const char* STR_COACH_CANNOT_DISBAND_ERR = "主角不可遣散";

	static const char* STR_NAMESPACE = "ERR_DISBAND_HERO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_NOT_EXIST_ERR
	,STR_RECUIT_NOT_EXIST_ERR
	,STR_COACH_CANNOT_DISBAND_ERR
	};
}

namespace ERR_OPEN_PRODUCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_KIND_ERR = 2;
	static const char* STR_KIND_ERR = "建筑类型不对";

	static const unsigned int ID_OWNER_ERR = 3;
	static const char* STR_OWNER_ERR = "建筑主人不是自己";

	static const char* STR_NAMESPACE = "ERR_OPEN_PRODUCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	,STR_KIND_ERR
	,STR_OWNER_ERR
	};
}

namespace ERR_OPEN_BAG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "背包不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_BAG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	};
}

namespace ERR_OPEN_SCIENCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_KIND_ERR = 2;
	static const char* STR_KIND_ERR = "建筑类型不对";

	static const unsigned int ID_OWNER_ERR = 3;
	static const char* STR_OWNER_ERR = "建筑主人不是自己";

	static const unsigned int ID_SERVER_INNER_ERR = 4;
	static const char* STR_SERVER_INNER_ERR = "数据错误";

	static const unsigned int ID_SCIENCE_UNLOCK = 5;
	static const char* STR_SCIENCE_UNLOCK = "科技未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_SCIENCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	,STR_KIND_ERR
	,STR_OWNER_ERR
	,STR_SERVER_INNER_ERR
	,STR_SCIENCE_UNLOCK
	};
}

namespace ERR_UPGRADE_SCIENCE_ATTREXT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_REACH_MAX_LEVEL_ERR = 1;
	static const char* STR_REACH_MAX_LEVEL_ERR = "该科技已达到最高级";

	static const unsigned int ID_NOT_ENOUGH_SCIENCE_ERR = 2;
	static const char* STR_NOT_ENOUGH_SCIENCE_ERR = "阅历点不足";

	static const unsigned int ID_ATTR_NOT_EXIST_ERR = 3;
	static const char* STR_ATTR_NOT_EXIST_ERR = "科技不存在";

	static const unsigned int ID_ATTR_NOT_UNLOCK_ERR = 4;
	static const char* STR_ATTR_NOT_UNLOCK_ERR = "科技未解锁";

	static const unsigned int ID_SCIENCE_NOT_EXIST_ERR = 5;
	static const char* STR_SCIENCE_NOT_EXIST_ERR = "科技不存在";

	static const unsigned int ID_HAVE_UPGRADE_ERR = 6;
	static const char* STR_HAVE_UPGRADE_ERR = "该等级科技已研究";

	static const unsigned int ID_LEVEL_ERR = 7;
	static const char* STR_LEVEL_ERR = "等级错误(每次只能升1级，也不可以降级)";

	static const unsigned int ID_NOT_ENOUGH_PLAYER_LEVEL_ERR = 8;
	static const char* STR_NOT_ENOUGH_PLAYER_LEVEL_ERR = "主角等级不足";

	static const unsigned int ID_SERVER_INNER_ERR = 9;
	static const char* STR_SERVER_INNER_ERR = "数据错误";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_SCIENCE_ATTREXT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_REACH_MAX_LEVEL_ERR
	,STR_NOT_ENOUGH_SCIENCE_ERR
	,STR_ATTR_NOT_EXIST_ERR
	,STR_ATTR_NOT_UNLOCK_ERR
	,STR_SCIENCE_NOT_EXIST_ERR
	,STR_HAVE_UPGRADE_ERR
	,STR_LEVEL_ERR
	,STR_NOT_ENOUGH_PLAYER_LEVEL_ERR
	,STR_SERVER_INNER_ERR
	};
}

namespace ERR_RECV_TASK_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_TASK_NOT_EXIT_ERR = 1;
	static const char* STR_TASK_NOT_EXIT_ERR = "任务不存在";

	static const unsigned int ID_STATE_ERR = 2;
	static const char* STR_STATE_ERR = "任务状态不对";

	static const unsigned int ID_HAVE_RECV_ERR = 3;
	static const char* STR_HAVE_RECV_ERR = "已经领取";

	static const unsigned int ID_BAG_FULL_ERR = 4;
	static const char* STR_BAG_FULL_ERR = "背包空间不足";

	static const char* STR_NAMESPACE = "ERR_RECV_TASK_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_TASK_NOT_EXIT_ERR
	,STR_STATE_ERR
	,STR_HAVE_RECV_ERR
	,STR_BAG_FULL_ERR
	};
}

namespace ERR_OPEN_RACE_BUILD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_EXIST_ERR = 1;
	static const char* STR_NOT_EXIST_ERR = "建筑不存在";

	static const unsigned int ID_KIND_ERR = 2;
	static const char* STR_KIND_ERR = "建筑类型不对";

	static const unsigned int ID_OWNER_ERR = 3;
	static const char* STR_OWNER_ERR = "建筑主人不是自己";

	static const char* STR_NAMESPACE = "ERR_OPEN_RACE_BUILD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_EXIST_ERR
	,STR_KIND_ERR
	,STR_OWNER_ERR
	};
}

namespace ERR_CHALLENGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ENEMY_NOT_EXIST_ERR = 1;
	static const char* STR_ENEMY_NOT_EXIST_ERR = "挑战目标不存在";

	static const unsigned int ID_IN_CD_ERR = 2;
	static const char* STR_IN_CD_ERR = "CD时间内";

	static const unsigned int ID_REACH_MAX_TIMES_ERR = 3;
	static const char* STR_REACH_MAX_TIMES_ERR = "已达到最大挑战次数";

	static const unsigned int ID_UNLOCK_ERR = 4;
	static const char* STR_UNLOCK_ERR = "竞技场未解锁";

	static const unsigned int ID_ENEMY_CANNOT_MYSELF_ERR = 5;
	static const char* STR_ENEMY_CANNOT_MYSELF_ERR = "挑战目标不能是自己";

	static const unsigned int ID_OUT_CHALLENGE_RANG_ERR = 6;
	static const char* STR_OUT_CHALLENGE_RANG_ERR = "超出挑战范围";

	static const char* STR_NAMESPACE = "ERR_CHALLENGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ENEMY_NOT_EXIST_ERR
	,STR_IN_CD_ERR
	,STR_REACH_MAX_TIMES_ERR
	,STR_UNLOCK_ERR
	,STR_ENEMY_CANNOT_MYSELF_ERR
	,STR_OUT_CHALLENGE_RANG_ERR
	};
}

namespace ERR_RECV_RANK_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RACE_NOT_EXIST_ERR = 1;
	static const char* STR_RACE_NOT_EXIST_ERR = "排名不存在";

	static const unsigned int ID_HAVE_RECVED_ERR = 2;
	static const char* STR_HAVE_RECVED_ERR = "今天已经领取过";

	static const unsigned int ID_TIME_ERR = 3;
	static const char* STR_TIME_ERR = "未到领取时间";

	static const char* STR_NAMESPACE = "ERR_RECV_RANK_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RACE_NOT_EXIST_ERR
	,STR_HAVE_RECVED_ERR
	,STR_TIME_ERR
	};
}

namespace ERR_OPEN_SHOP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_TAB_NOT_EXIST_ERR = 1;
	static const char* STR_TAB_NOT_EXIST_ERR = "商店分类不存在";

	static const unsigned int ID_SHOP_NOT_EXIST_ERR = 2;
	static const char* STR_SHOP_NOT_EXIST_ERR = "商店不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_SHOP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_TAB_NOT_EXIST_ERR
	,STR_SHOP_NOT_EXIST_ERR
	};
}

namespace ERR_BUG_ITEM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ITEM_NOT_EXIST_ERR = 1;
	static const char* STR_ITEM_NOT_EXIST_ERR = "道具不存在";

	static const unsigned int ID_BAG_FULL_ERR = 2;
	static const char* STR_BAG_FULL_ERR = "背包空间不足";

	static const unsigned int ID_TABID_ERR = 3;
	static const char* STR_TABID_ERR = "商店分类不存在";

	static const unsigned int ID_PILE_NOT_MATCH_TAB_ERR = 4;
	static const char* STR_PILE_NOT_MATCH_TAB_ERR = "叠加数和商店分类不匹配";

	static const unsigned int ID_SHOP_NOT_EXIST_ERR = 5;
	static const char* STR_SHOP_NOT_EXIST_ERR = "商店不存在";

	static const unsigned int ID_PILENUM0 = 6;
	static const char* STR_PILENUM0 = "叠加数不能为0";

	static const unsigned int ID_LEVEL_NOT_REACH = 7;
	static const char* STR_LEVEL_NOT_REACH = "等级不足";

	static const char* STR_NAMESPACE = "ERR_BUG_ITEM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ITEM_NOT_EXIST_ERR
	,STR_BAG_FULL_ERR
	,STR_TABID_ERR
	,STR_PILE_NOT_MATCH_TAB_ERR
	,STR_SHOP_NOT_EXIST_ERR
	,STR_PILENUM0
	,STR_LEVEL_NOT_REACH
	};
}

namespace ERR_SELL_ITEM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ITEM_NOT_EXIST_ERR = 1;
	static const char* STR_ITEM_NOT_EXIST_ERR = "道具不存在";

	static const unsigned int ID_OVER_CUR_NUM = 2;
	static const char* STR_OVER_CUR_NUM = "出售个数超过实际个数";

	static const unsigned int ID_SELL_PROHIBIT = 3;
	static const char* STR_SELL_PROHIBIT = "该道具无法出售";

	static const unsigned int ID_BAG_FULL = 4;
	static const char* STR_BAG_FULL = "背包空间不足，请先整理背包";

	static const char* STR_NAMESPACE = "ERR_SELL_ITEM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ITEM_NOT_EXIST_ERR
	,STR_OVER_CUR_NUM
	,STR_SELL_PROHIBIT
	,STR_BAG_FULL
	};
}

namespace ERR_REGISTER_USERNAME 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_USERNAME_EMPTY_ERR = 1;
	static const char* STR_USERNAME_EMPTY_ERR = "用户名不能为空";

	static const unsigned int ID_PASSWORD_EMPTY_ERR = 2;
	static const char* STR_PASSWORD_EMPTY_ERR = "密码不能为空";

	static const unsigned int ID_USERNAME_EXIST_ERR = 3;
	static const char* STR_USERNAME_EXIST_ERR = "该用户名已经被注册";

	static const unsigned int ID_REGISTERED_ERR = 4;
	static const char* STR_REGISTERED_ERR = "已经注册过用户名";

	static const unsigned int ID_USER_NOT_EXIST_ERR = 5;
	static const char* STR_USER_NOT_EXIST_ERR = "用户不存在";

	static const unsigned int ID_INVALID_CHARACTER = 6;
	static const char* STR_INVALID_CHARACTER = "无效字符";

	static const unsigned int ID_REGISTER_SUCCESS = 7;
	static const char* STR_REGISTER_SUCCESS = "注册成功";

	static const char* STR_NAMESPACE = "ERR_REGISTER_USERNAME";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_USERNAME_EMPTY_ERR
	,STR_PASSWORD_EMPTY_ERR
	,STR_USERNAME_EXIST_ERR
	,STR_REGISTERED_ERR
	,STR_USER_NOT_EXIST_ERR
	,STR_INVALID_CHARACTER
	,STR_REGISTER_SUCCESS
	};
}

namespace ERR_EMBATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_NOT_EXIST_ERR = 1;
	static const char* STR_HERO_NOT_EXIST_ERR = "伙伴不存在";

	static const unsigned int ID_FORMATION_INDEX_INVALID_ERR = 2;
	static const char* STR_FORMATION_INDEX_INVALID_ERR = "阵型位置无效";

	static const unsigned int ID_FORMATION_INDEX_HAVE_HERO_ERR = 3;
	static const char* STR_FORMATION_INDEX_HAVE_HERO_ERR = "位置已经有伙伴";

	static const unsigned int ID_FORMATION_KIND_NOT_EXIST = 4;
	static const char* STR_FORMATION_KIND_NOT_EXIST = "阵型不存在";

	static const unsigned int ID_COACH_NOT_IN_FORMATION = 5;
	static const char* STR_COACH_NOT_IN_FORMATION = "主角不在阵上";

	static const unsigned int ID_FORMATION_EXCEPTION = 6;
	static const char* STR_FORMATION_EXCEPTION = "阵型位置异常";

	static const unsigned int ID_POSTION_NOT_PLAYER = 7;
	static const char* STR_POSTION_NOT_PLAYER = "布阵异常，未解锁的位置上有伙伴";

	static const char* STR_NAMESPACE = "ERR_EMBATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_NOT_EXIST_ERR
	,STR_FORMATION_INDEX_INVALID_ERR
	,STR_FORMATION_INDEX_HAVE_HERO_ERR
	,STR_FORMATION_KIND_NOT_EXIST
	,STR_COACH_NOT_IN_FORMATION
	,STR_FORMATION_EXCEPTION
	,STR_POSTION_NOT_PLAYER
	};
}

namespace ERR_OPEN_EMBATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FORMATION_NOT_OPEN = 1;
	static const char* STR_FORMATION_NOT_OPEN = "布阵尚未开放";

	static const unsigned int ID_FORMATION_ID_INVALID = 2;
	static const char* STR_FORMATION_ID_INVALID = "布阵数据出现异常请联系客服";

	static const char* STR_NAMESPACE = "ERR_OPEN_EMBATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FORMATION_NOT_OPEN
	,STR_FORMATION_ID_INVALID
	};
}

namespace ERR_OPEN_ITEM_MAKER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ITEM_NOT_EXIST = 1;
	static const char* STR_ITEM_NOT_EXIST = "道具不存在";

	static const unsigned int ID_NOT_ITEM_MAKER = 2;
	static const char* STR_NOT_ITEM_MAKER = "道具不是制作卷";

	static const unsigned int ID_NEED_EQUIP_NULL = 3;
	static const char* STR_NEED_EQUIP_NULL = "没有所需装备";

	static const unsigned int ID_MAKER_CONFIG_ERR = 4;
	static const char* STR_MAKER_CONFIG_ERR = "制作卷配置错误";

	static const char* STR_NAMESPACE = "ERR_OPEN_ITEM_MAKER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ITEM_NOT_EXIST
	,STR_NOT_ITEM_MAKER
	,STR_NEED_EQUIP_NULL
	,STR_MAKER_CONFIG_ERR
	};
}

namespace ERR_MAKE_ITEM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ITEM_NOT_EXIST = 1;
	static const char* STR_ITEM_NOT_EXIST = "道具不存在";

	static const unsigned int ID_NOT_ITEM_MAKER = 2;
	static const char* STR_NOT_ITEM_MAKER = "该道具不是制作卷";

	static const unsigned int ID_NEED_EQUIP_NULL = 3;
	static const char* STR_NEED_EQUIP_NULL = "没有所需装备";

	static const unsigned int ID_SELECTED_EQUIP_NOT_EXIST = 4;
	static const char* STR_SELECTED_EQUIP_NOT_EXIST = "所选装备不存在";

	static const unsigned int ID_SELECTED_EQUIP_NOT_MATCH = 5;
	static const char* STR_SELECTED_EQUIP_NOT_MATCH = "所选装备不匹配";

	static const unsigned int ID_MAKER_CONFIG_ERR = 6;
	static const char* STR_MAKER_CONFIG_ERR = "制作卷配置错误";

	static const unsigned int ID_BAG_FULL = 7;
	static const char* STR_BAG_FULL = "背包已满";

	static const unsigned int ID_LEVEL_NOT_MATCH_FULL = 8;
	static const char* STR_LEVEL_NOT_MATCH_FULL = "装备穿戴等级不符，且背包已满";

	static const unsigned int ID_NEED_ITEM_NOT_ENOUGH = 9;
	static const char* STR_NEED_ITEM_NOT_ENOUGH = "所需道具不足";

	static const unsigned int ID_NEED_BLUEGAS_NOT_ENOUGH = 10;
	static const char* STR_NEED_BLUEGAS_NOT_ENOUGH = "所需仙露不足";

	static const unsigned int ID_MAKER_LEVEL_UNLOCK = 11;
	static const char* STR_MAKER_LEVEL_UNLOCK = "制作等级未开放";

	static const char* STR_NAMESPACE = "ERR_MAKE_ITEM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ITEM_NOT_EXIST
	,STR_NOT_ITEM_MAKER
	,STR_NEED_EQUIP_NULL
	,STR_SELECTED_EQUIP_NOT_EXIST
	,STR_SELECTED_EQUIP_NOT_MATCH
	,STR_MAKER_CONFIG_ERR
	,STR_BAG_FULL
	,STR_LEVEL_NOT_MATCH_FULL
	,STR_NEED_ITEM_NOT_ENOUGH
	,STR_NEED_BLUEGAS_NOT_ENOUGH
	,STR_MAKER_LEVEL_UNLOCK
	};
}

namespace ERR_GET_HERO_DRUG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_NOT_EXIST = 1;
	static const char* STR_HERO_NOT_EXIST = "伙伴不存在";

	static const char* STR_NAMESPACE = "ERR_GET_HERO_DRUG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_NOT_EXIST
	};
}

namespace ERR_RECV_ONLINE_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BAG_FULL = 1;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_IN_CD = 2;
	static const char* STR_IN_CD = "时间未到";

	static const unsigned int ID_HAVE_RECV_ALL = 3;
	static const char* STR_HAVE_RECV_ALL = "当天已经领取完";

	static const unsigned int ID_HAVE_NOT_ENCOURAGE = 4;
	static const char* STR_HAVE_NOT_ENCOURAGE = "已经没有奖励";

	static const char* STR_NAMESPACE = "ERR_RECV_ONLINE_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BAG_FULL
	,STR_IN_CD
	,STR_HAVE_RECV_ALL
	,STR_HAVE_NOT_ENCOURAGE
	};
}

namespace ERR_OPEN_INSTANCE_ENCOURAGE_BOX 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BAG_FULL = 1;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_HAVE_OPEN = 2;
	static const char* STR_HAVE_OPEN = "已经打开过";

	static const unsigned int ID_NOT_PASS = 3;
	static const char* STR_NOT_PASS = "未通关";

	static const char* STR_NAMESPACE = "ERR_OPEN_INSTANCE_ENCOURAGE_BOX";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BAG_FULL
	,STR_HAVE_OPEN
	,STR_NOT_PASS
	};
}

namespace ERR_INCREASE_SIZE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HAVE_REACH_MAX_SIZE = 1;
	static const char* STR_HAVE_REACH_MAX_SIZE = "已经达到容量上限";

	static const unsigned int ID_SIZE_SMALL_EQUAL_THAN_CUR = 2;
	static const char* STR_SIZE_SMALL_EQUAL_THAN_CUR = "请求容量小于等于当然容量";

	static const char* STR_NAMESPACE = "ERR_INCREASE_SIZE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HAVE_REACH_MAX_SIZE
	,STR_SIZE_SMALL_EQUAL_THAN_CUR
	};
}

namespace ERR_ONHOOK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PHYSTRENGTH_NOT_ENOUGH = 1;
	static const char* STR_PHYSTRENGTH_NOT_ENOUGH = "体力不足";

	static const unsigned int ID_BAG_FULL = 2;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_INSTANCE_NOT_EXIST = 3;
	static const char* STR_INSTANCE_NOT_EXIST = "副本不存在";

	static const unsigned int ID_INSTANCE_NOT_BATTLED = 4;
	static const char* STR_INSTANCE_NOT_BATTLED = "该副本尚未通关，无法挂机";

	static const unsigned int ID_OUT_MAX_RUN_RANG = 5;
	static const char* STR_OUT_MAX_RUN_RANG = "挂机最大不能超过40次";

	static const unsigned int ID_RUN_TIMES_0 = 6;
	static const char* STR_RUN_TIMES_0 = "挂机次数不能为0";

	static const unsigned int ID_NOT_IN_ONHOOK = 7;
	static const char* STR_NOT_IN_ONHOOK = "没有处于挂机状态";

	static const unsigned int ID_ONHOOK_ING = 8;
	static const char* STR_ONHOOK_ING = "正在挂机中";

	static const unsigned int ID_ONHOOK_UNLOCK = 9;
	static const char* STR_ONHOOK_UNLOCK = "挂机未解锁";

	static const char* STR_NAMESPACE = "ERR_ONHOOK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PHYSTRENGTH_NOT_ENOUGH
	,STR_BAG_FULL
	,STR_INSTANCE_NOT_EXIST
	,STR_INSTANCE_NOT_BATTLED
	,STR_OUT_MAX_RUN_RANG
	,STR_RUN_TIMES_0
	,STR_NOT_IN_ONHOOK
	,STR_ONHOOK_ING
	,STR_ONHOOK_UNLOCK
	};
}

namespace ERR_CLR_CHALLENGE_CD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RACE_NOT_EXIST_ERR = 1;
	static const char* STR_RACE_NOT_EXIST_ERR = "竞技场不存在";

	static const unsigned int ID_CHALLENGE_TIMES_OVER_ERR = 2;
	static const char* STR_CHALLENGE_TIMES_OVER_ERR = "今天挑战次数已经用完";

	static const char* STR_NAMESPACE = "ERR_CLR_CHALLENGE_CD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RACE_NOT_EXIST_ERR
	,STR_CHALLENGE_TIMES_OVER_ERR
	};
}

namespace ERR_GET_ZONE_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ZONE_NOT_EXIST_ERR = 1;
	static const char* STR_ZONE_NOT_EXIST_ERR = "区不存在";

	static const char* STR_NAMESPACE = "ERR_GET_ZONE_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ZONE_NOT_EXIST_ERR
	};
}

namespace ERR_SELECT_ZONE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ZONE_NOT_EXIST_ERR = 1;
	static const char* STR_ZONE_NOT_EXIST_ERR = "区不存在";

	static const unsigned int ID_USERNAME_NOT_EXIST = 2;
	static const char* STR_USERNAME_NOT_EXIST = "用户名不存在";

	static const unsigned int ID_PWD_ERR = 3;
	static const char* STR_PWD_ERR = "用户密码错误";

	static const unsigned int ID_INVALID_AUTH_TYPE = 4;
	static const char* STR_INVALID_AUTH_TYPE = "无效认证方式";

	static const unsigned int ID_INVALID_CHARACTER = 5;
	static const char* STR_INVALID_CHARACTER = "无效字符";

	static const unsigned int ID_FORBIDLOGIN = 6;
	static const char* STR_FORBIDLOGIN = "该账号禁止登录";

	static const char* STR_NAMESPACE = "ERR_SELECT_ZONE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ZONE_NOT_EXIST_ERR
	,STR_USERNAME_NOT_EXIST
	,STR_PWD_ERR
	,STR_INVALID_AUTH_TYPE
	,STR_INVALID_CHARACTER
	,STR_FORBIDLOGIN
	};
}

namespace ERR_REGISTER_DSPNAME 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_CHARACTER = 1;
	static const char* STR_INVALID_CHARACTER = "昵称只能使用中文、英文和数字";

	static const unsigned int ID_DSPNAME_CONFLICT = 2;
	static const char* STR_DSPNAME_CONFLICT = "该昵称已被别的玩家使用";

	static const unsigned int ID_TOO_LONG = 3;
	static const char* STR_TOO_LONG = "昵称太长，不能超过6个汉字或12个字母（1个汉字 = 2个字母）";

	static const unsigned int ID_SENSITIVE_WORD = 4;
	static const char* STR_SENSITIVE_WORD = "昵称中含有敏感词汇，请重新输入";

	static const unsigned int ID_HAVE_DONE = 5;
	static const char* STR_HAVE_DONE = "已经注册过昵称";

	static const char* STR_NAMESPACE = "ERR_REGISTER_DSPNAME";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_CHARACTER
	,STR_DSPNAME_CONFLICT
	,STR_TOO_LONG
	,STR_SENSITIVE_WORD
	,STR_HAVE_DONE
	};
}

namespace ERR_GOODS_USE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GOODS_NOT_EXIST = 1;
	static const char* STR_GOODS_NOT_EXIST = "物品不存在";

	static const unsigned int ID_BAG_FULL = 2;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_IN_CD = 3;
	static const char* STR_IN_CD = "在CD中";

	static const unsigned int ID_REACH_MAX_TODAY = 4;
	static const char* STR_REACH_MAX_TODAY = "已达到今天使用次数";

	static const unsigned int ID_NOT_ENOUGH_NUM = 5;
	static const char* STR_NOT_ENOUGH_NUM = "物品数量不足";

	static const unsigned int ID_NOT_REACH_USE_LEVEL = 6;
	static const char* STR_NOT_REACH_USE_LEVEL = "未达到使用等级";

	static const unsigned int ID_CAN_NOT_USE_IN_BAG = 7;
	static const char* STR_CAN_NOT_USE_IN_BAG = "该物品不能在背包中使用";

	static const unsigned int ID_POS_CANNOT_XIANGQIAN = 8;
	static const char* STR_POS_CANNOT_XIANGQIAN = "该位置不能镶嵌";

	static const unsigned int ID_PET_BAG_FULL = 9;
	static const char* STR_PET_BAG_FULL = "宠物背包已满";

	static const unsigned int ID_UNKOWND_ERR = 10;
	static const char* STR_UNKOWND_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_GOODS_USE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GOODS_NOT_EXIST
	,STR_BAG_FULL
	,STR_IN_CD
	,STR_REACH_MAX_TODAY
	,STR_NOT_ENOUGH_NUM
	,STR_NOT_REACH_USE_LEVEL
	,STR_CAN_NOT_USE_IN_BAG
	,STR_POS_CANNOT_XIANGQIAN
	,STR_PET_BAG_FULL
	,STR_UNKOWND_ERR
	};
}

namespace ERR_LOGIN_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_USE_OUT_NUM = 1;
	static const char* STR_USE_OUT_NUM = "今天领取次数已用完";

	static const unsigned int ID_STATE_INVALID = 2;
	static const char* STR_STATE_INVALID = "状态不正确";

	static const unsigned int ID_BAG_FULL = 3;
	static const char* STR_BAG_FULL = "背包已满";

	static const char* STR_NAMESPACE = "ERR_LOGIN_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_USE_OUT_NUM
	,STR_STATE_INVALID
	,STR_BAG_FULL
	};
}

namespace ERR_GET_RANK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_GET_RANK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_GET_RACE_BATTLE_LOG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK_RACE_ERR = 1;
	static const char* STR_UNLOCK_RACE_ERR = "竞技场未解锁";

	static const char* STR_NAMESPACE = "ERR_GET_RACE_BATTLE_LOG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK_RACE_ERR
	};
}

namespace ERR_ACCEPT_TASK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_STATE_NOT_MATCH = 1;
	static const char* STR_STATE_NOT_MATCH = "任务状态不匹配";

	static const unsigned int ID_TASK_LINE_INVALID = 2;
	static const char* STR_TASK_LINE_INVALID = "无效任务线";

	static const unsigned int ID_TASK_NOT_EXIST = 3;
	static const char* STR_TASK_NOT_EXIST = "任务不存在";

	static const unsigned int ID_HAVEN_ACCEPT_TASK = 4;
	static const char* STR_HAVEN_ACCEPT_TASK = "当前已有接受的任务";

	static const unsigned int ID_NOT_RACHE_LEVEL = 5;
	static const char* STR_NOT_RACHE_LEVEL = "未达到指定等级";

	static const unsigned int ID_NOT_PASS_INSTANCE = 6;
	static const char* STR_NOT_PASS_INSTANCE = "未通过指定副本";

	static const unsigned int ID_NOT_RACHE_BUILD_LEVEL = 7;
	static const char* STR_NOT_RACHE_BUILD_LEVEL = "未达到指定建筑等级";

	static const unsigned int ID_NOT_PASS_MAINTASK = 8;
	static const char* STR_NOT_PASS_MAINTASK = "未通过指定主线任务";

	static const char* STR_NAMESPACE = "ERR_ACCEPT_TASK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_STATE_NOT_MATCH
	,STR_TASK_LINE_INVALID
	,STR_TASK_NOT_EXIST
	,STR_HAVEN_ACCEPT_TASK
	,STR_NOT_RACHE_LEVEL
	,STR_NOT_PASS_INSTANCE
	,STR_NOT_RACHE_BUILD_LEVEL
	,STR_NOT_PASS_MAINTASK
	};
}

namespace ERR_GIVEUP_TASK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_STATE_NOT_MATCH = 1;
	static const char* STR_STATE_NOT_MATCH = "任务状态不匹配";

	static const unsigned int ID_TASK_LINE_INVALID = 2;
	static const char* STR_TASK_LINE_INVALID = "无效任务线";

	static const unsigned int ID_TASK_NOT_EXIST = 3;
	static const char* STR_TASK_NOT_EXIST = "任务不存在";

	static const unsigned int ID_CAN_NOT_GIVE_UP_MAIN_TASK = 4;
	static const char* STR_CAN_NOT_GIVE_UP_MAIN_TASK = "主线任务无法放弃";

	static const char* STR_NAMESPACE = "ERR_GIVEUP_TASK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_STATE_NOT_MATCH
	,STR_TASK_LINE_INVALID
	,STR_TASK_NOT_EXIST
	,STR_CAN_NOT_GIVE_UP_MAIN_TASK
	};
}

namespace ERR_PLUNDER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_IN_CD = 2;
	static const char* STR_IN_CD = "抓捕CD中";

	static const unsigned int ID_REACH_MAX_TIMES = 3;
	static const char* STR_REACH_MAX_TIMES = "已达到最大次数";

	static const unsigned int ID_ENEMY_CANNOT_MYSELF = 4;
	static const char* STR_ENEMY_CANNOT_MYSELF = "抓捕目标不能是自己";

	static const unsigned int ID_ENEMY_UNLOCK = 5;
	static const char* STR_ENEMY_UNLOCK = "对方小黑屋未解锁";

	static const unsigned int ID_OUT_PLUNDER_RANG = 6;
	static const char* STR_OUT_PLUNDER_RANG = "超出可抓捕的等级范围";

	static const unsigned int ID_ENEMY_AVOID_BATTLE = 7;
	static const char* STR_ENEMY_AVOID_BATTLE = "对方免战中";

	static const unsigned int ID_ITEM_NOT_ENOUGH = 8;
	static const char* STR_ITEM_NOT_ENOUGH = "道具数量不足";

	static const unsigned int ID_ENEMY_NOT_EXIST = 9;
	static const char* STR_ENEMY_NOT_EXIST = "抓捕目标不存在";

	static const unsigned int ID_YOUR_CAPTURE = 10;
	static const char* STR_YOUR_CAPTURE = "抓捕目标已经是你的俘虏";

	static const unsigned int ID_CAN_NOT_REPLUNDER = 11;
	static const char* STR_CAN_NOT_REPLUNDER = "不能短时间内重复抓捕该玩家";

	static const unsigned int ID_GRID_NOT_IDLE = 12;
	static const char* STR_GRID_NOT_IDLE = "俘虏牢笼已被占用";

	static const unsigned int ID_GRID_NOT_UNLOCK = 13;
	static const char* STR_GRID_NOT_UNLOCK = "俘虏牢笼未解锁";

	static const unsigned int ID_NOT_BECAPTURE = 14;
	static const char* STR_NOT_BECAPTURE = "自己没有被该玩家俘虏";

	static const unsigned int ID_HAVE_BECAPTURE = 15;
	static const char* STR_HAVE_BECAPTURE = "抓捕目标已是其它玩家的俘虏";

	static const unsigned int ID_NOT_THIS_OWNER = 16;
	static const char* STR_NOT_THIS_OWNER = "抓捕目标的俘虏主是其他玩家";

	static const unsigned int ID_NO_OWNER = 17;
	static const char* STR_NO_OWNER = "抓捕目标没有俘虏主";

	static const char* STR_NAMESPACE = "ERR_PLUNDER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	,STR_IN_CD
	,STR_REACH_MAX_TIMES
	,STR_ENEMY_CANNOT_MYSELF
	,STR_ENEMY_UNLOCK
	,STR_OUT_PLUNDER_RANG
	,STR_ENEMY_AVOID_BATTLE
	,STR_ITEM_NOT_ENOUGH
	,STR_ENEMY_NOT_EXIST
	,STR_YOUR_CAPTURE
	,STR_CAN_NOT_REPLUNDER
	,STR_GRID_NOT_IDLE
	,STR_GRID_NOT_UNLOCK
	,STR_NOT_BECAPTURE
	,STR_HAVE_BECAPTURE
	,STR_NOT_THIS_OWNER
	,STR_NO_OWNER
	};
}

namespace ERR_OPEN_ENEMY_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IDX_INVALID = 1;
	static const char* STR_IDX_INVALID = "无效序号";

	static const char* STR_NAMESPACE = "ERR_OPEN_ENEMY_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IDX_INVALID
	};
}

namespace ERR_OPEN_PLUNDER_RECOMMEND_PLAYER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_PLUNDER_RECOMMEND_PLAYER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	};
}

namespace ERR_FREE_CAPTURE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_CAPTURE_NOT_EXIST = 2;
	static const char* STR_CAPTURE_NOT_EXIST = "俘虏不存在";

	static const char* STR_NAMESPACE = "ERR_FREE_CAPTURE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	,STR_CAPTURE_NOT_EXIST
	};
}

namespace ERR_OPEN_MONEY_TREE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_MONEY_TREE_UNLOCK = 1;
	static const char* STR_MONEY_TREE_UNLOCK = "招财未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_MONEY_TREE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_MONEY_TREE_UNLOCK
	};
}

namespace ERR_ROCK_MONEY_TREE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_MONEY_TREE_UNLOCK = 1;
	static const char* STR_MONEY_TREE_UNLOCK = "招财未解锁";

	static const unsigned int ID_USE_OUT_NUM = 2;
	static const char* STR_USE_OUT_NUM = "招财次数已经用完";

	static const char* STR_NAMESPACE = "ERR_ROCK_MONEY_TREE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_MONEY_TREE_UNLOCK
	,STR_USE_OUT_NUM
	};
}

namespace ERR_GET_PLUNDER_BATTLE_LOG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const char* STR_NAMESPACE = "ERR_GET_PLUNDER_BATTLE_LOG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	};
}

namespace ERR_OPEN_GODWEAPON 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GODWEAPON_UNLOCK = 1;
	static const char* STR_GODWEAPON_UNLOCK = "神器未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_GODWEAPON";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GODWEAPON_UNLOCK
	};
}

namespace ERR_UPGRADE_GODWEAPON 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GODWEAPON_UNLOCK = 1;
	static const char* STR_GODWEAPON_UNLOCK = "神器未解锁";

	static const unsigned int ID_QUALITY_INVALID = 2;
	static const char* STR_QUALITY_INVALID = "品质无效";

	static const unsigned int ID_LEVEL_INVALID = 3;
	static const char* STR_LEVEL_INVALID = "等级无效";

	static const unsigned int ID_HERO_NOT_EXIST = 4;
	static const char* STR_HERO_NOT_EXIST = "伙伴不存在";

	static const unsigned int ID_NOT_REACH_MAX_LEVEL = 5;
	static const char* STR_NOT_REACH_MAX_LEVEL = "未达到当前品质最大等级，不能进阶";

	static const unsigned int ID_NOT_ENOUGH_SCIENCE = 6;
	static const char* STR_NOT_ENOUGH_SCIENCE = "器魂点不足";

	static const unsigned int ID_HERO_LEVEL_NOT_REACH = 7;
	static const char* STR_HERO_LEVEL_NOT_REACH = "伙伴等级没达到进阶要求";

	static const unsigned int ID_GODWEAPON_HAVE_EXIST = 8;
	static const char* STR_GODWEAPON_HAVE_EXIST = "神器已开启";

	static const unsigned int ID_ITEM_NOT_EXIST = 9;
	static const char* STR_ITEM_NOT_EXIST = "进阶符不存在";

	static const unsigned int ID_ITEM_NOT_ENOUGH = 10;
	static const char* STR_ITEM_NOT_ENOUGH = "进阶符不够";

	static const unsigned int ID_LEVEL_NOT_REACH = 11;
	static const char* STR_LEVEL_NOT_REACH = "主角等级未达到进阶要求";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_GODWEAPON";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GODWEAPON_UNLOCK
	,STR_QUALITY_INVALID
	,STR_LEVEL_INVALID
	,STR_HERO_NOT_EXIST
	,STR_NOT_REACH_MAX_LEVEL
	,STR_NOT_ENOUGH_SCIENCE
	,STR_HERO_LEVEL_NOT_REACH
	,STR_GODWEAPON_HAVE_EXIST
	,STR_ITEM_NOT_EXIST
	,STR_ITEM_NOT_ENOUGH
	,STR_LEVEL_NOT_REACH
	};
}

namespace ERR_FREE_GODWEAPON 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GODWEAPON_UNLOCK = 1;
	static const char* STR_GODWEAPON_UNLOCK = "神器未解锁";

	static const unsigned int ID_HERO_NOT_EXIST = 2;
	static const char* STR_HERO_NOT_EXIST = "伙伴不存在";

	static const unsigned int ID_VIP_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_VIP_LEVEL_NOT_ENOUGH = "VIP等级不够，不能进行完美释放";

	static const unsigned int ID_COACH_HERO_NOT_FREE = 4;
	static const char* STR_COACH_HERO_NOT_FREE = "主角不允许释放神器";

	static const char* STR_NAMESPACE = "ERR_FREE_GODWEAPON";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GODWEAPON_UNLOCK
	,STR_HERO_NOT_EXIST
	,STR_VIP_LEVEL_NOT_ENOUGH
	,STR_COACH_HERO_NOT_FREE
	};
}

namespace ERR_DEL_ENEMY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IDX_INVALID = 1;
	static const char* STR_IDX_INVALID = "无效序号";

	static const unsigned int ID_ENEMY_NOT_EXIST = 2;
	static const char* STR_ENEMY_NOT_EXIST = "仇敌不存在";

	static const char* STR_NAMESPACE = "ERR_DEL_ENEMY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IDX_INVALID
	,STR_ENEMY_NOT_EXIST
	};
}

namespace ERR_OPEN_AWAKEN  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_AWAKEN ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	};
}

namespace ERR_AWAKEN  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const unsigned int ID_STUDY_BAG_FULL = 2;
	static const char* STR_STUDY_BAG_FULL = "卦象背包已满";

	static const unsigned int ID_INVALID_STATE = 3;
	static const char* STR_INVALID_STATE = "当前状态不可一键算卦";

	static const unsigned int ID_REACH_CALL_NUM = 4;
	static const char* STR_REACH_CALL_NUM = "今天召唤次数已经用完";

	static const char* STR_NAMESPACE = "ERR_AWAKEN ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	,STR_STUDY_BAG_FULL
	,STR_INVALID_STATE
	,STR_REACH_CALL_NUM
	};
}

namespace ERR_OPEN_DRESS_STUDY_TAB  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_DRESS_STUDY_TAB ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	};
}

namespace ERR_DRESS_STUDY  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const unsigned int ID_IDX_INVALID = 2;
	static const char* STR_IDX_INVALID = "无效序号";

	static const unsigned int ID_STUDY_NOT_EXIST = 3;
	static const char* STR_STUDY_NOT_EXIST = "卦象不存在";

	static const unsigned int ID_HERO_NOT_EXIST = 4;
	static const char* STR_HERO_NOT_EXIST = "伙伴不存在";

	static const unsigned int ID_POS_NOT_UNLOCK = 5;
	static const char* STR_POS_NOT_UNLOCK = "穿戴位置尚未解锁";

	static const unsigned int ID_EXIST_SAME_STUDY_ATTR_KIND = 6;
	static const char* STR_EXIST_SAME_STUDY_ATTR_KIND = "已存在该属性卦象";

	static const unsigned int ID_CANNOT_DRESS_THIS_STUDY = 7;
	static const char* STR_CANNOT_DRESS_THIS_STUDY = "该卦象不能穿戴";

	static const unsigned int ID_STUDY_BAG_FULL = 8;
	static const char* STR_STUDY_BAG_FULL = "卦象背包满";

	static const char* STR_NAMESPACE = "ERR_DRESS_STUDY ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	,STR_IDX_INVALID
	,STR_STUDY_NOT_EXIST
	,STR_HERO_NOT_EXIST
	,STR_POS_NOT_UNLOCK
	,STR_EXIST_SAME_STUDY_ATTR_KIND
	,STR_CANNOT_DRESS_THIS_STUDY
	,STR_STUDY_BAG_FULL
	};
}

namespace ERR_STUDY_TO_EXP  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const unsigned int ID_IDX_INVALID = 2;
	static const char* STR_IDX_INVALID = "无效序号";

	static const unsigned int ID_STUDY_NOT_EXIST = 3;
	static const char* STR_STUDY_NOT_EXIST = "卦象不存在";

	static const char* STR_NAMESPACE = "ERR_STUDY_TO_EXP ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	,STR_IDX_INVALID
	,STR_STUDY_NOT_EXIST
	};
}

namespace ERR_UPGRADE_STUDY  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const unsigned int ID_IDX_INVALID = 2;
	static const char* STR_IDX_INVALID = "无效序号";

	static const unsigned int ID_STUDY_NOT_EXIST = 3;
	static const char* STR_STUDY_NOT_EXIST = "卦象不存在";

	static const unsigned int ID_HERO_NOT_EXIST = 4;
	static const char* STR_HERO_NOT_EXIST = "伙伴不存在";

	static const unsigned int ID_NOT_ENOUGH_STUDYEXP = 5;
	static const char* STR_NOT_ENOUGH_STUDYEXP = "卦象经验不足";

	static const unsigned int ID_CANNOT_JMP_LEVEL = 6;
	static const char* STR_CANNOT_JMP_LEVEL = "不能跳级";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_STUDY ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	,STR_IDX_INVALID
	,STR_STUDY_NOT_EXIST
	,STR_HERO_NOT_EXIST
	,STR_NOT_ENOUGH_STUDYEXP
	,STR_CANNOT_JMP_LEVEL
	};
}

namespace ERR_REFRESH_PLUNDER_RECOMMEND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const char* STR_NAMESPACE = "ERR_REFRESH_PLUNDER_RECOMMEND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	};
}

namespace ERR_STUDY_CLIP_AUTO2EXP  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const char* STR_NAMESPACE = "ERR_STUDY_CLIP_AUTO2EXP ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	};
}

namespace ERR_VISIT_HERO  
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RECUIT_NOT_EXIST_ERR = 1;
	static const char* STR_RECUIT_NOT_EXIST_ERR = "聚贤馆不存在";

	static const unsigned int ID_HERO_NOT_EXIST = 2;
	static const char* STR_HERO_NOT_EXIST = "伙伴不存在";

	static const unsigned int ID_ALREADY_RECUIT = 3;
	static const char* STR_ALREADY_RECUIT = "已招募";

	static const unsigned int ID_NOT_ENOUGH_JINGJIE_LEVEL = 4;
	static const char* STR_NOT_ENOUGH_JINGJIE_LEVEL = "境界等级未达到";

	static const unsigned int ID_NOT_ENOUGH_PLAYER_LEVEL = 5;
	static const char* STR_NOT_ENOUGH_PLAYER_LEVEL = "主角等级未达到";

	static const char* STR_NAMESPACE = "ERR_VISIT_HERO ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RECUIT_NOT_EXIST_ERR
	,STR_HERO_NOT_EXIST
	,STR_ALREADY_RECUIT
	,STR_NOT_ENOUGH_JINGJIE_LEVEL
	,STR_NOT_ENOUGH_PLAYER_LEVEL
	};
}

namespace ERR_BUY_PHYSTRENGTH 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_REACH_MAX_NUM = 1;
	static const char* STR_REACH_MAX_NUM = "已达到最大购买次数";

	static const char* STR_NAMESPACE = "ERR_BUY_PHYSTRENGTH";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_REACH_MAX_NUM
	};
}

namespace ERR_CLR_PLUNDER_CD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_PLUNDER_TIMES_OVER_ERR = 2;
	static const char* STR_PLUNDER_TIMES_OVER_ERR = "今天抓捕次数已经用完";

	static const char* STR_NAMESPACE = "ERR_CLR_PLUNDER_CD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	,STR_PLUNDER_TIMES_OVER_ERR
	};
}

namespace ERR_OPEN_CAPTURE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_CAPTURE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	};
}

namespace ERR_COLLECT_INCOME 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_CAPTURE_NOT_EXIST = 2;
	static const char* STR_CAPTURE_NOT_EXIST = "俘虏不存在";

	static const unsigned int ID_WORK_TYPE_NOT_SET = 3;
	static const char* STR_WORK_TYPE_NOT_SET = "工作类型未指定";

	static const char* STR_NAMESPACE = "ERR_COLLECT_INCOME";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	,STR_CAPTURE_NOT_EXIST
	,STR_WORK_TYPE_NOT_SET
	};
}

namespace ERR_BLEED_CAPTURE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_CAPTURE_NOT_EXIST = 2;
	static const char* STR_CAPTURE_NOT_EXIST = "俘虏不存在";

	static const unsigned int ID_WORK_TYPE_NOT_SET = 3;
	static const char* STR_WORK_TYPE_NOT_SET = "工作类型未指定";

	static const char* STR_NAMESPACE = "ERR_BLEED_CAPTURE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	,STR_CAPTURE_NOT_EXIST
	,STR_WORK_TYPE_NOT_SET
	};
}

namespace ERR_OPEN_MAINTOWN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUILD_NOT_EXIST = 1;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_MAINTOWN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUILD_NOT_EXIST
	};
}

namespace ERR_RECV_MAINTOWN_INCOME 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUILD_NOT_EXIST = 1;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const unsigned int ID_HAVE_RECV = 2;
	static const char* STR_HAVE_RECV = "今日已领取";

	static const char* STR_NAMESPACE = "ERR_RECV_MAINTOWN_INCOME";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUILD_NOT_EXIST
	,STR_HAVE_RECV
	};
}

namespace ERR_LEVY_MAINTOWN_INCOME 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUILD_NOT_EXIST = 1;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const unsigned int ID_REACH_MAX_LEVY_NUM = 2;
	static const char* STR_REACH_MAX_LEVY_NUM = "今日征收次数已用完";

	static const unsigned int ID_IN_CD = 3;
	static const char* STR_IN_CD = "征收CD中";

	static const char* STR_NAMESPACE = "ERR_LEVY_MAINTOWN_INCOME";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUILD_NOT_EXIST
	,STR_REACH_MAX_LEVY_NUM
	,STR_IN_CD
	};
}

namespace ERR_SET_WORK_TYPE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_CAPTURE_NOT_EXIST = 2;
	static const char* STR_CAPTURE_NOT_EXIST = "俘虏不存在";

	static const unsigned int ID_HAVE_SET = 3;
	static const char* STR_HAVE_SET = "已经指定过";

	static const unsigned int ID_INVALID_TYPE = 4;
	static const char* STR_INVALID_TYPE = "工作类型无效";

	static const char* STR_NAMESPACE = "ERR_SET_WORK_TYPE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	,STR_CAPTURE_NOT_EXIST
	,STR_HAVE_SET
	,STR_INVALID_TYPE
	};
}

namespace ERR_DRIVE_AWAY_OWNER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_OWNER_NOT_EXIST = 2;
	static const char* STR_OWNER_NOT_EXIST = "俘虏主不存在";

	static const char* STR_NAMESPACE = "ERR_DRIVE_AWAY_OWNER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	,STR_OWNER_NOT_EXIST
	};
}

namespace ERR_OPEN_DRUG_USE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUILD_NOT_EXIST = 1;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_DRUG_USE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUILD_NOT_EXIST
	};
}

namespace ERR_OPEN_HERO_DRUG_USE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERONOTEXIST = 1;
	static const char* STR_HERONOTEXIST = "伙伴不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_HERO_DRUG_USE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERONOTEXIST
	};
}

namespace ERR_CLR_USE_DRUG_CD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERONOTEXIST = 1;
	static const char* STR_HERONOTEXIST = "伙伴不存在";

	static const char* STR_NAMESPACE = "ERR_CLR_USE_DRUG_CD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERONOTEXIST
	};
}

namespace ERR_USE_DRUG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERONOTEXIST = 1;
	static const char* STR_HERONOTEXIST = "伙伴不存在";

	static const unsigned int ID_DRUGNOTEXIST = 2;
	static const char* STR_DRUGNOTEXIST = "丹药不存在";

	static const unsigned int ID_BAGDRUGNOTEXIST = 3;
	static const char* STR_BAGDRUGNOTEXIST = "背包中不存在该丹药";

	static const char* STR_NAMESPACE = "ERR_USE_DRUG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERONOTEXIST
	,STR_DRUGNOTEXIST
	,STR_BAGDRUGNOTEXIST
	};
}

namespace ERR_OPEN_UPGRADE_DRUG_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_UPGRADE_DRUG_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_UPGRADE_DRUG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_DRUGNOTEXIST = 1;
	static const char* STR_DRUGNOTEXIST = "丹药不存在";

	static const unsigned int ID_BAGDRUGNOTEXIST = 2;
	static const char* STR_BAGDRUGNOTEXIST = "背包中不存在该丹药";

	static const unsigned int ID_BAGDRUGNOTENOUGH = 3;
	static const char* STR_BAGDRUGNOTENOUGH = "背包中丹药个数不足";

	static const unsigned int ID_DRUGMIXBAGFULL = 4;
	static const char* STR_DRUGMIXBAGFULL = "背包已满";

	static const unsigned int ID_DRUGMIXERROR = 5;
	static const char* STR_DRUGMIXERROR = "丹药合成失败";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_DRUG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_DRUGNOTEXIST
	,STR_BAGDRUGNOTEXIST
	,STR_BAGDRUGNOTENOUGH
	,STR_DRUGMIXBAGFULL
	,STR_DRUGMIXERROR
	};
}

namespace ERR_COMPOSE_EQUIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_EQUIPNOTEXIST = 1;
	static const char* STR_EQUIPNOTEXIST = "装备不存在";

	static const unsigned int ID_MATERIALNOTENOUGH = 2;
	static const char* STR_MATERIALNOTENOUGH = "材料不够";

	static const unsigned int ID_COMPOSEERROR = 3;
	static const char* STR_COMPOSEERROR = "合成失败";

	static const unsigned int ID_HERONOTENOUGHLEAVER = 4;
	static const char* STR_HERONOTENOUGHLEAVER = "伙伴等级不够";

	static const unsigned int ID_BAGFULL = 5;
	static const char* STR_BAGFULL = "背包已满";

	static const char* STR_NAMESPACE = "ERR_COMPOSE_EQUIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_EQUIPNOTEXIST
	,STR_MATERIALNOTENOUGH
	,STR_COMPOSEERROR
	,STR_HERONOTENOUGHLEAVER
	,STR_BAGFULL
	};
}

namespace ERR_OPEN_STRENGTH 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_STRENGTH";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_RECV_LEFT_INCOME 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLUNDER_UNLOCK = 1;
	static const char* STR_PLUNDER_UNLOCK = "小黑屋未解锁";

	static const unsigned int ID_GRID_INVALID = 2;
	static const char* STR_GRID_INVALID = "牢笼序号无效";

	static const unsigned int ID_NO_INCOME = 3;
	static const char* STR_NO_INCOME = "没有收益";

	static const char* STR_NAMESPACE = "ERR_RECV_LEFT_INCOME";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLUNDER_UNLOCK
	,STR_GRID_INVALID
	,STR_NO_INCOME
	};
}

namespace ERR_GET_HERO_LIST 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLAY_NOT_EXIT = 1;
	static const char* STR_PLAY_NOT_EXIT = "玩家不存在";

	static const char* STR_NAMESPACE = "ERR_GET_HERO_LIST";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLAY_NOT_EXIT
	};
}

namespace ERR_GET_HERO_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLAY_NOT_EXIT = 1;
	static const char* STR_PLAY_NOT_EXIT = "玩家不存在";

	static const unsigned int ID_HERO_NOT_EXIT = 2;
	static const char* STR_HERO_NOT_EXIT = "伙伴不存在";

	static const char* STR_NAMESPACE = "ERR_GET_HERO_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLAY_NOT_EXIT
	,STR_HERO_NOT_EXIT
	};
}

namespace ERR_SMITHY_BUY_EQUIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SMITHY_NOT_EXIT = 1;
	static const char* STR_SMITHY_NOT_EXIT = "铁匠铺不存在";

	static const unsigned int ID_HERO_NOT_EXIT = 2;
	static const char* STR_HERO_NOT_EXIT = "伙伴不存在";

	static const unsigned int ID_INVALID_EQUIP_POS = 3;
	static const char* STR_INVALID_EQUIP_POS = "无效穿戴位置";

	static const unsigned int ID_HAVE_EQUIP = 4;
	static const char* STR_HAVE_EQUIP = "该位置已有装备";

	static const char* STR_NAMESPACE = "ERR_SMITHY_BUY_EQUIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SMITHY_NOT_EXIT
	,STR_HERO_NOT_EXIT
	,STR_INVALID_EQUIP_POS
	,STR_HAVE_EQUIP
	};
}

namespace ERR_CLR_STRENGTHEN_CD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_CLR_STRENGTHEN_CD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_CROP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_CROP_IS_NOT_OPEN = 1;
	static const char* STR_CROP_IS_NOT_OPEN = "农场未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_CROP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_CROP_IS_NOT_OPEN
	};
}

namespace ERR_CROP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FIELD_NOT_EXIT = 1;
	static const char* STR_FIELD_NOT_EXIT = "地块不存在";

	static const unsigned int ID_FIELD_NOT_OPEN = 2;
	static const char* STR_FIELD_NOT_OPEN = "地块未开放";

	static const unsigned int ID_FIELD_HAVE_CROP = 3;
	static const char* STR_FIELD_HAVE_CROP = "地块已种值";

	static const unsigned int ID_SEED_NOT_EXIT = 4;
	static const char* STR_SEED_NOT_EXIT = "种子不存在";

	static const unsigned int ID_CROP_IS_NOT_OPEN = 5;
	static const char* STR_CROP_IS_NOT_OPEN = "农场未开放";

	static const unsigned int ID_CROP_IS_NOT_POWER = 6;
	static const char* STR_CROP_IS_NOT_POWER = "当前田地还不能种植如此多的种子";

	static const unsigned int ID_CROP_IS_NOT_SEED = 7;
	static const char* STR_CROP_IS_NOT_SEED = "种子数量不足";

	static const char* STR_NAMESPACE = "ERR_CROP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FIELD_NOT_EXIT
	,STR_FIELD_NOT_OPEN
	,STR_FIELD_HAVE_CROP
	,STR_SEED_NOT_EXIT
	,STR_CROP_IS_NOT_OPEN
	,STR_CROP_IS_NOT_POWER
	,STR_CROP_IS_NOT_SEED
	};
}

namespace ERR_GAIN_CROP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FIELD_NOT_EXIT = 1;
	static const char* STR_FIELD_NOT_EXIT = "地块不存在";

	static const unsigned int ID_FIELD_NOT_OPEN = 2;
	static const char* STR_FIELD_NOT_OPEN = "地块未开放";

	static const unsigned int ID_FIELD_NOT_CROP = 3;
	static const char* STR_FIELD_NOT_CROP = "地块未种值";

	static const unsigned int ID_NOT_TIME_OUT = 4;
	static const char* STR_NOT_TIME_OUT = "地块不可收获，时间未到达";

	static const unsigned int ID_BAG_FULL = 5;
	static const char* STR_BAG_FULL = "背包空间不够";

	static const unsigned int ID_CROP_IS_NOT_OPEN = 6;
	static const char* STR_CROP_IS_NOT_OPEN = "农场未开放";

	static const char* STR_NAMESPACE = "ERR_GAIN_CROP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FIELD_NOT_EXIT
	,STR_FIELD_NOT_OPEN
	,STR_FIELD_NOT_CROP
	,STR_NOT_TIME_OUT
	,STR_BAG_FULL
	,STR_CROP_IS_NOT_OPEN
	};
}

namespace ERR_CROP_FIELD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FIELD_NOT_EXIT = 1;
	static const char* STR_FIELD_NOT_EXIT = "地块不存在";

	static const unsigned int ID_FIELD_IS_OPEN = 2;
	static const char* STR_FIELD_IS_OPEN = "地块已开放";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_LEVEL_NOT_ENOUGH = "主角等级不够";

	static const unsigned int ID_CROP_IS_NOT_OPEN = 4;
	static const char* STR_CROP_IS_NOT_OPEN = "农场未开放";

	static const char* STR_NAMESPACE = "ERR_CROP_FIELD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FIELD_NOT_EXIT
	,STR_FIELD_IS_OPEN
	,STR_LEVEL_NOT_ENOUGH
	,STR_CROP_IS_NOT_OPEN
	};
}

namespace ERR_CLR_CROP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FIELD_NOT_EXIT = 1;
	static const char* STR_FIELD_NOT_EXIT = "地块不存在";

	static const unsigned int ID_FIELD_NOT_OPEN = 2;
	static const char* STR_FIELD_NOT_OPEN = "地块未开放";

	static const unsigned int ID_FIELD_NOT_CROP = 3;
	static const char* STR_FIELD_NOT_CROP = "地块未种值";

	static const unsigned int ID_CROP_IS_NOT_OPEN = 4;
	static const char* STR_CROP_IS_NOT_OPEN = "农场未开放";

	static const char* STR_NAMESPACE = "ERR_CLR_CROP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FIELD_NOT_EXIT
	,STR_FIELD_NOT_OPEN
	,STR_FIELD_NOT_CROP
	,STR_CROP_IS_NOT_OPEN
	};
}

namespace ERR_OPEN_ECPERIENCE_DRUG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ECPERIENCE_NOT_EXIT = 1;
	static const char* STR_ECPERIENCE_NOT_EXIT = "经验丹不存在";

	static const unsigned int ID_BAG_ECPERIENCE_NOT_EXIT = 2;
	static const char* STR_BAG_ECPERIENCE_NOT_EXIT = "背包中不存在该类经验丹";

	static const unsigned int ID_HERO_NOT_EXIT = 3;
	static const char* STR_HERO_NOT_EXIT = "伙伴不存在";

	static const unsigned int ID_TAKING_FULL = 4;
	static const char* STR_TAKING_FULL = "今日服用次数已满";

	static const unsigned int ID_HERO_IS_COACH = 5;
	static const char* STR_HERO_IS_COACH = "主角无法服用经验丹";

	static const unsigned int ID_ECPERIENCE_UNLOCK = 6;
	static const char* STR_ECPERIENCE_UNLOCK = "服用经验丹功能未解锁";

	static const unsigned int ID_CUR_INDEX_ERROR = 7;
	static const char* STR_CUR_INDEX_ERROR = "当前次数不对";

	static const char* STR_NAMESPACE = "ERR_OPEN_ECPERIENCE_DRUG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ECPERIENCE_NOT_EXIT
	,STR_BAG_ECPERIENCE_NOT_EXIT
	,STR_HERO_NOT_EXIT
	,STR_TAKING_FULL
	,STR_HERO_IS_COACH
	,STR_ECPERIENCE_UNLOCK
	,STR_CUR_INDEX_ERROR
	};
}

namespace ERR_USER_ECPERIENCE_DRUG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ECPERIENCE_NOT_EXIT = 1;
	static const char* STR_ECPERIENCE_NOT_EXIT = "经验丹不存在";

	static const unsigned int ID_BAG_ECPERIENCE_NOT_EXIT = 2;
	static const char* STR_BAG_ECPERIENCE_NOT_EXIT = "背包中不存在该类经验丹";

	static const unsigned int ID_HERO_NOT_EXIT = 3;
	static const char* STR_HERO_NOT_EXIT = "伙伴不存在";

	static const unsigned int ID_TAKING_FULL = 4;
	static const char* STR_TAKING_FULL = "今日服用次数已满";

	static const unsigned int ID_HERO_IS_COACH = 5;
	static const char* STR_HERO_IS_COACH = "主角无法服用经验丹";

	static const unsigned int ID_HERO_ENOUGH_LEAVER = 6;
	static const char* STR_HERO_ENOUGH_LEAVER = "伙伴等级无法超过主角等级";

	static const unsigned int ID_GOOD_TYPE_ERR = 7;
	static const char* STR_GOOD_TYPE_ERR = "物品类型出错";

	static const unsigned int ID_NOT_REMAIN_GOLD_EXPERIENCE_CNT = 8;
	static const char* STR_NOT_REMAIN_GOLD_EXPERIENCE_CNT = "今日次数用尽";

	static const unsigned int ID_CUR_INDEX_ERROR = 9;
	static const char* STR_CUR_INDEX_ERROR = "当前次数不对";

	static const unsigned int ID_NEED_FLY = 10;
	static const char* STR_NEED_FLY = "伙伴等级已满，需要飞升后才能继续使用经验丹";

	static const char* STR_NAMESPACE = "ERR_USER_ECPERIENCE_DRUG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ECPERIENCE_NOT_EXIT
	,STR_BAG_ECPERIENCE_NOT_EXIT
	,STR_HERO_NOT_EXIT
	,STR_TAKING_FULL
	,STR_HERO_IS_COACH
	,STR_HERO_ENOUGH_LEAVER
	,STR_GOOD_TYPE_ERR
	,STR_NOT_REMAIN_GOLD_EXPERIENCE_CNT
	,STR_CUR_INDEX_ERROR
	,STR_NEED_FLY
	};
}

namespace ERR_OPEN_MELT_EQUIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUILD_NOT_OPEN = 1;
	static const char* STR_BUILD_NOT_OPEN = "熔炼未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_MELT_EQUIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUILD_NOT_OPEN
	};
}

namespace ERR_MELT_EQUIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BAG_EQUIP_NOT_EXIT = 1;
	static const char* STR_BAG_EQUIP_NOT_EXIT = "背包装备不存在";

	static const unsigned int ID_BUILD_NOT_OPEN = 2;
	static const char* STR_BUILD_NOT_OPEN = "熔炼未开放";

	static const unsigned int ID_EQUIP_HAVE_MOSAIC_GEM = 3;
	static const char* STR_EQUIP_HAVE_MOSAIC_GEM = "装备上镶嵌有宝石,请先卸下";

	static const unsigned int ID_BAG_GOOD_NOT_EXIT = 4;
	static const char* STR_BAG_GOOD_NOT_EXIT = "背包装备碎片不存在";

	static const unsigned int ID_BAG_GOOD_NOT_ENOUGH = 5;
	static const char* STR_BAG_GOOD_NOT_ENOUGH = "背包装备碎片个数不够";

	static const unsigned int ID_EQUIP_ENCHANT_ERR = 6;
	static const char* STR_EQUIP_ENCHANT_ERR = "装备已被附魔，无法投入熔炼炉";

	static const char* STR_NAMESPACE = "ERR_MELT_EQUIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BAG_EQUIP_NOT_EXIT
	,STR_BUILD_NOT_OPEN
	,STR_EQUIP_HAVE_MOSAIC_GEM
	,STR_BAG_GOOD_NOT_EXIT
	,STR_BAG_GOOD_NOT_ENOUGH
	,STR_EQUIP_ENCHANT_ERR
	};
}

namespace ERR_OPEN_UPDATE_ATTR_PANNEL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_CANNOT_FIND = 1;
	static const char* STR_HERO_CANNOT_FIND = "伙伴不存在";

	static const unsigned int ID_ATTR_CANNOT_FIND = 2;
	static const char* STR_ATTR_CANNOT_FIND = "属性不存在";

	static const unsigned int ID_ATTR_PARAM_ERROR = 3;
	static const char* STR_ATTR_PARAM_ERROR = "参数错误";

	static const unsigned int ID_ATTR_UPDATE_ATTR_UNLOCK = 4;
	static const char* STR_ATTR_UPDATE_ATTR_UNLOCK = "属性强化未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_UPDATE_ATTR_PANNEL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_CANNOT_FIND
	,STR_ATTR_CANNOT_FIND
	,STR_ATTR_PARAM_ERROR
	,STR_ATTR_UPDATE_ATTR_UNLOCK
	};
}

namespace ERR_UPDATE_ATTR 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_CANNOT_FIND = 1;
	static const char* STR_HERO_CANNOT_FIND = "伙伴不存在";

	static const unsigned int ID_ATTR_CANNOT_FIND = 2;
	static const char* STR_ATTR_CANNOT_FIND = "属性不存在";

	static const unsigned int ID_ATTR_UPDATE_FULL = 3;
	static const char* STR_ATTR_UPDATE_FULL = "属性升级已达到最大次数";

	static const unsigned int ID_ATTR_PARAM_ERROR = 4;
	static const char* STR_ATTR_PARAM_ERROR = "参数错误";

	static const unsigned int ID_CANNOT_FIND_UPDATE_ITEM_ERROR = 5;
	static const char* STR_CANNOT_FIND_UPDATE_ITEM_ERROR = "无法找到升级所需物品";

	static const unsigned int ID_NOT_ENOUGH_ITEM = 6;
	static const char* STR_NOT_ENOUGH_ITEM = "没有足够的物品";

	static const char* STR_NAMESPACE = "ERR_UPDATE_ATTR";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_CANNOT_FIND
	,STR_ATTR_CANNOT_FIND
	,STR_ATTR_UPDATE_FULL
	,STR_ATTR_PARAM_ERROR
	,STR_CANNOT_FIND_UPDATE_ITEM_ERROR
	,STR_NOT_ENOUGH_ITEM
	};
}

namespace ERR_COMPOSE_GOODS 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GOODS_NOT_COMPOSE = 1;
	static const char* STR_GOODS_NOT_COMPOSE = "物品不可合成";

	static const unsigned int ID_BAG_NOT_EXIST = 2;
	static const char* STR_BAG_NOT_EXIST = "背包中物品不存在";

	static const unsigned int ID_BAD_TYPE = 3;
	static const char* STR_BAD_TYPE = "物品类型出错";

	static const unsigned int ID_BAG_NOT_ENOUGH = 4;
	static const char* STR_BAG_NOT_ENOUGH = "物品个数不足";

	static const unsigned int ID_BAG_FULL = 5;
	static const char* STR_BAG_FULL = "背包已满";

	static const unsigned int ID_BAG_BOX_HIT = 6;
	static const char* STR_BAG_BOX_HIT = "合成失败";

	static const char* STR_NAMESPACE = "ERR_COMPOSE_GOODS";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GOODS_NOT_COMPOSE
	,STR_BAG_NOT_EXIST
	,STR_BAD_TYPE
	,STR_BAG_NOT_ENOUGH
	,STR_BAG_FULL
	,STR_BAG_BOX_HIT
	};
}

namespace ERR_GET_ITEM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_GET_ITEM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_BIND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_PARM = 1;
	static const char* STR_INVALID_PARM = "参数不正确";

	static const unsigned int ID_PLAYERID_NOT_EXIT = 2;
	static const char* STR_PLAYERID_NOT_EXIT = "玩家不存在";

	static const unsigned int ID_USERID_IS_BINDED = 3;
	static const char* STR_USERID_IS_BINDED = "该用户账号已绑定";

	static const unsigned int ID_ACCOUNT_IS_BINDED = 4;
	static const char* STR_ACCOUNT_IS_BINDED = "该账号已绑定其它用户";

	static const unsigned int ID_AUTHTYPE_NOT_EXIT = 5;
	static const char* STR_AUTHTYPE_NOT_EXIT = "绑定的账号类型不支持";

	static const char* STR_NAMESPACE = "ERR_BIND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_PARM
	,STR_PLAYERID_NOT_EXIT
	,STR_USERID_IS_BINDED
	,STR_ACCOUNT_IS_BINDED
	,STR_AUTHTYPE_NOT_EXIT
	};
}

namespace ERR_UNBIND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PLAYERID_NOT_EXIT = 1;
	static const char* STR_PLAYERID_NOT_EXIT = "玩家不存在";

	static const unsigned int ID_AUTHTYPE_NOT_EXIT = 2;
	static const char* STR_AUTHTYPE_NOT_EXIT = "绑定的账号类型不支持";

	static const unsigned int ID_PLAYERID_NOT_BIND = 3;
	static const char* STR_PLAYERID_NOT_BIND = "玩家账号未绑定";

	static const unsigned int ID_PLAYERID_IS_COMPETENCE = 4;
	static const char* STR_PLAYERID_IS_COMPETENCE = "用户没有权限";

	static const char* STR_NAMESPACE = "ERR_UNBIND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PLAYERID_NOT_EXIT
	,STR_AUTHTYPE_NOT_EXIT
	,STR_PLAYERID_NOT_BIND
	,STR_PLAYERID_IS_COMPETENCE
	};
}

namespace ERR_GET_ORDER_ID 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ERR_DISCONNECT_EXCHANGE_SERVER = 1;
	static const char* STR_ERR_DISCONNECT_EXCHANGE_SERVER = "充值失败，错误码：1";

	static const unsigned int ID_ERR_SEND_TO_EXCHANGE_SERVER = 2;
	static const char* STR_ERR_SEND_TO_EXCHANGE_SERVER = "充值失败，错误码：2";

	static const unsigned int ID_ERR_PARSE_DATA_FROM_EXCHANGE_SERVER = 3;
	static const char* STR_ERR_PARSE_DATA_FROM_EXCHANGE_SERVER = "充值失败，错误码：3";

	static const char* STR_NAMESPACE = "ERR_GET_ORDER_ID";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ERR_DISCONNECT_EXCHANGE_SERVER
	,STR_ERR_SEND_TO_EXCHANGE_SERVER
	,STR_ERR_PARSE_DATA_FROM_EXCHANGE_SERVER
	};
}

namespace ERR_EXCHANGE_RESULT_ID 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_EXCHANGE_RESULT_ID";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_COMPOSE_SEED 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GOODS_NOT_COMPOSE = 1;
	static const char* STR_GOODS_NOT_COMPOSE = "种子不可合成";

	static const unsigned int ID_BAG_NOT_EXIST = 2;
	static const char* STR_BAG_NOT_EXIST = "背包中种子不存在";

	static const unsigned int ID_BAD_TYPE = 3;
	static const char* STR_BAD_TYPE = "种子类型出错";

	static const unsigned int ID_BAG_NOT_ENOUGH = 4;
	static const char* STR_BAG_NOT_ENOUGH = "种子个数不足";

	static const unsigned int ID_BAG_FULL = 5;
	static const char* STR_BAG_FULL = "背包满已";

	static const char* STR_NAMESPACE = "ERR_COMPOSE_SEED";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GOODS_NOT_COMPOSE
	,STR_BAG_NOT_EXIST
	,STR_BAD_TYPE
	,STR_BAG_NOT_ENOUGH
	,STR_BAG_FULL
	};
}

namespace ERR_CHAT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_CHAT_PLAYERID_ERROR = 1;
	static const char* STR_CHAT_PLAYERID_ERROR = "用户ID出错";

	static const unsigned int ID_CHAT_TYPE_ERROR = 2;
	static const char* STR_CHAT_TYPE_ERROR = "聊天类型不支持";

	static const unsigned int ID_CHAT_DATA_ERROR = 3;
	static const char* STR_CHAT_DATA_ERROR = "聊天内容太长";

	static const unsigned int ID_CHAT_SILENCE_ERROR = 4;
	static const char* STR_CHAT_SILENCE_ERROR = "玩家被禁言，请联系管理员";

	static const unsigned int ID_CHAT_VERIFY_ERROR = 5;
	static const char* STR_CHAT_VERIFY_ERROR = "聊天内容含有非法内容，禁止";

	static const unsigned int ID_CHAT_BUSY_ERROR = 6;
	static const char* STR_CHAT_BUSY_ERROR = "服务器繁忙，请稍后再试";

	static const unsigned int ID_CHAT_EMPTY = 7;
	static const char* STR_CHAT_EMPTY = "聊天内容不能为空";

	static const unsigned int ID_CHAT_NOT_ENTER_FACTION = 8;
	static const char* STR_CHAT_NOT_ENTER_FACTION = "未进入门派";

	static const char* STR_NAMESPACE = "ERR_CHAT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_CHAT_PLAYERID_ERROR
	,STR_CHAT_TYPE_ERROR
	,STR_CHAT_DATA_ERROR
	,STR_CHAT_SILENCE_ERROR
	,STR_CHAT_VERIFY_ERROR
	,STR_CHAT_BUSY_ERROR
	,STR_CHAT_EMPTY
	,STR_CHAT_NOT_ENTER_FACTION
	};
}

namespace ERR_OPEN_YABIAO_BUILD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUILD_UNLOCK = 1;
	static const char* STR_BUILD_UNLOCK = "建筑未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_YABIAO_BUILD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUILD_UNLOCK
	};
}

namespace ERR_BEGIN_YABIAO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_MAX_YABIAO_COUNT_ERROR = 1;
	static const char* STR_MAX_YABIAO_COUNT_ERROR = "取经次数用尽";

	static const unsigned int ID_YABIAOING = 2;
	static const char* STR_YABIAOING = "正在取经中";

	static const char* STR_NAMESPACE = "ERR_BEGIN_YABIAO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_MAX_YABIAO_COUNT_ERROR
	,STR_YABIAOING
	};
}

namespace ERR_OPEN_JIEBIAO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_QUERY_PLAYER_NUM_ERROR = 1;
	static const char* STR_QUERY_PLAYER_NUM_ERROR = "请求玩家数量错误";

	static const char* STR_NAMESPACE = "ERR_OPEN_JIEBIAO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_QUERY_PLAYER_NUM_ERROR
	};
}

namespace ERR_JIEBIAO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_MAX_JIEBIAO_COUNT_ERROR = 1;
	static const char* STR_MAX_JIEBIAO_COUNT_ERROR = "拦截次数用尽";

	static const unsigned int ID_PLAYER_NOT_IN_YABIAO_ERROR = 2;
	static const char* STR_PLAYER_NOT_IN_YABIAO_ERROR = "该玩家不在取经";

	static const unsigned int ID_PLAYER_MAX_BE_JIEBIAO_COUNT_ERROR = 3;
	static const char* STR_PLAYER_MAX_BE_JIEBIAO_COUNT_ERROR = "该玩家被拦截太多次，请手下留情";

	static const unsigned int ID_CANNOT_FIND_PLAYER = 4;
	static const char* STR_CANNOT_FIND_PLAYER = "无法找到该玩家";

	static const unsigned int ID_BATTLE_ERROR = 5;
	static const char* STR_BATTLE_ERROR = "战斗发生未知错误";

	static const unsigned int ID_JIEBIAO_IN_CD = 6;
	static const char* STR_JIEBIAO_IN_CD = "拦截还在冷却中";

	static const unsigned int ID_CANNOT_JIEBIAO_SELF = 7;
	static const char* STR_CANNOT_JIEBIAO_SELF = "不能拦截自己";

	static const unsigned int ID_CANNOT_REPEAT_JIEBIAO_PLAYER = 8;
	static const char* STR_CANNOT_REPEAT_JIEBIAO_PLAYER = "你已经拦截过该玩家";

	static const char* STR_NAMESPACE = "ERR_JIEBIAO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_MAX_JIEBIAO_COUNT_ERROR
	,STR_PLAYER_NOT_IN_YABIAO_ERROR
	,STR_PLAYER_MAX_BE_JIEBIAO_COUNT_ERROR
	,STR_CANNOT_FIND_PLAYER
	,STR_BATTLE_ERROR
	,STR_JIEBIAO_IN_CD
	,STR_CANNOT_JIEBIAO_SELF
	,STR_CANNOT_REPEAT_JIEBIAO_PLAYER
	};
}

namespace ERR_OPNE_ACTIVITY_CENTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "未开放";

	static const char* STR_NAMESPACE = "ERR_OPNE_ACTIVITY_CENTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	};
}

namespace ERR_OPNE_EAT_BREAD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "未开放";

	static const char* STR_NAMESPACE = "ERR_OPNE_EAT_BREAD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	};
}

namespace ERR_OBTAIN_PHYSTRENGTHEN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "未开放";

	static const unsigned int ID_NOT_IN_ACTIVITY = 2;
	static const char* STR_NOT_IN_ACTIVITY = "未参加活动";

	static const unsigned int ID_ACTIVITY_OVER = 3;
	static const char* STR_ACTIVITY_OVER = "活动已结束";

	static const char* STR_NAMESPACE = "ERR_OBTAIN_PHYSTRENGTHEN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	,STR_NOT_IN_ACTIVITY
	,STR_ACTIVITY_OVER
	};
}

namespace ERR_LEAVE_EAT_BREAD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "未开放";

	static const char* STR_NAMESPACE = "ERR_LEAVE_EAT_BREAD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	};
}

namespace ERR_FLUSH_BIAO_QUALITY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_MAX_QUALITY_LEVEL = 1;
	static const char* STR_MAX_QUALITY_LEVEL = "经的品质已经是最好的";

	static const unsigned int ID_YABIAOING = 2;
	static const char* STR_YABIAOING = "取经时不能刷新品质";

	static const char* STR_NAMESPACE = "ERR_FLUSH_BIAO_QUALITY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_MAX_QUALITY_LEVEL
	,STR_YABIAOING
	};
}

namespace ERR_CLIMB_TOWER_RESET 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_SCIENCE = 1;
	static const char* STR_NOT_SCIENCE = "场景不存在";

	static const unsigned int ID_NOT_TOWER = 2;
	static const char* STR_NOT_TOWER = "城镇不存在";

	static const unsigned int ID_NOT_ENOUGH_RESET_TIMES = 3;
	static const char* STR_NOT_ENOUGH_RESET_TIMES = "今天免费重置次数已用光";

	static const unsigned int ID_BUY_TIMES_NOT_ENOUGH = 4;
	static const char* STR_BUY_TIMES_NOT_ENOUGH = "今天可购买重置次数已用光";

	static const unsigned int ID_UNDER_NO_RESET = 5;
	static const char* STR_UNDER_NO_RESET = "当前处于底层，无需重置";

	static const char* STR_NAMESPACE = "ERR_CLIMB_TOWER_RESET";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_SCIENCE
	,STR_NOT_TOWER
	,STR_NOT_ENOUGH_RESET_TIMES
	,STR_BUY_TIMES_NOT_ENOUGH
	,STR_UNDER_NO_RESET
	};
}

namespace ERR_BUY_CLIMB_TOWER_RESET 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUY_TIMES_NOT_ENOUGH = 1;
	static const char* STR_BUY_TIMES_NOT_ENOUGH = "今天购买次数已用光";

	static const char* STR_NAMESPACE = "ERR_BUY_CLIMB_TOWER_RESET";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUY_TIMES_NOT_ENOUGH
	};
}

namespace ERR_CLIMB_TOWER_ONHOOK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PHYSTRENGTH_NOT_ENOUGH = 1;
	static const char* STR_PHYSTRENGTH_NOT_ENOUGH = "体力不足";

	static const unsigned int ID_CLIMBTOWER_ONE_PER_DAY_ERR = 2;
	static const char* STR_CLIMBTOWER_ONE_PER_DAY_ERR = "你今日已通关过最顶层";

	static const unsigned int ID_BAG_FULL = 3;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_INSTANCE_NOT_EXIST = 4;
	static const char* STR_INSTANCE_NOT_EXIST = "该层不存在";

	static const unsigned int ID_INSTANCE_NOT_BATTLED = 5;
	static const char* STR_INSTANCE_NOT_BATTLED = "本层尚未通关，不能自动爬塔";

	static const unsigned int ID_OUT_MAX_RUN_RANG = 6;
	static const char* STR_OUT_MAX_RUN_RANG = "最大只能自动爬塔40次层";

	static const unsigned int ID_RUN_TIMES_0 = 7;
	static const char* STR_RUN_TIMES_0 = "自动爬塔次数不能为0";

	static const unsigned int ID_NOT_IN_ONHOOK = 8;
	static const char* STR_NOT_IN_ONHOOK = "没有处于自动爬塔状态";

	static const unsigned int ID_ONHOOK_ING = 9;
	static const char* STR_ONHOOK_ING = "正在自动爬塔中";

	static const unsigned int ID_ONHOOK_UNLOCK = 10;
	static const char* STR_ONHOOK_UNLOCK = "自动爬塔未解锁";

	static const unsigned int ID_NOT_SCIENCE = 11;
	static const char* STR_NOT_SCIENCE = "场景不存在";

	static const unsigned int ID_NOT_TOWER = 12;
	static const char* STR_NOT_TOWER = "该层目标不存在";

	static const char* STR_NAMESPACE = "ERR_CLIMB_TOWER_ONHOOK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PHYSTRENGTH_NOT_ENOUGH
	,STR_CLIMBTOWER_ONE_PER_DAY_ERR
	,STR_BAG_FULL
	,STR_INSTANCE_NOT_EXIST
	,STR_INSTANCE_NOT_BATTLED
	,STR_OUT_MAX_RUN_RANG
	,STR_RUN_TIMES_0
	,STR_NOT_IN_ONHOOK
	,STR_ONHOOK_ING
	,STR_ONHOOK_UNLOCK
	,STR_NOT_SCIENCE
	,STR_NOT_TOWER
	};
}

namespace ERR_GET_YABIAO_BATTLE_LOG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK_YABIAO_ERR = 1;
	static const char* STR_UNLOCK_YABIAO_ERR = "取经功能尚未解锁";

	static const char* STR_NAMESPACE = "ERR_GET_YABIAO_BATTLE_LOG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK_YABIAO_ERR
	};
}

namespace ERR_GET_YABIAO_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK_YABIAO_ERR = 1;
	static const char* STR_UNLOCK_YABIAO_ERR = "取经功能尚未解锁";

	static const unsigned int ID_CANNOT_FIND_ENCOURAGE = 2;
	static const char* STR_CANNOT_FIND_ENCOURAGE = "没有奖励";

	static const unsigned int ID_TIME_NOT_DOWN = 3;
	static const char* STR_TIME_NOT_DOWN = "取经未完成，不能领取奖励";

	static const char* STR_NAMESPACE = "ERR_GET_YABIAO_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK_YABIAO_ERR
	,STR_CANNOT_FIND_ENCOURAGE
	,STR_TIME_NOT_DOWN
	};
}

namespace ERR_GET_BATTLE_DATA 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BATTLE_NOT_EXIST = 1;
	static const char* STR_BATTLE_NOT_EXIST = "获得该战斗信息失败";

	static const char* STR_NAMESPACE = "ERR_GET_BATTLE_DATA";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BATTLE_NOT_EXIST
	};
}

namespace ERR_OPEN_YABIAO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_YABIAO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_UPGRADE_JINGJIE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_JINGJIE_UNLOCK = 1;
	static const char* STR_JINGJIE_UNLOCK = "境界尚未解锁";

	static const unsigned int ID_QUALITY_INVALID = 2;
	static const char* STR_QUALITY_INVALID = "阶无效";

	static const unsigned int ID_LEVEL_INVALID = 3;
	static const char* STR_LEVEL_INVALID = "等级无效";

	static const unsigned int ID_NOT_REACH_MAX_LEVEL = 4;
	static const char* STR_NOT_REACH_MAX_LEVEL = "未达到当前阶的最大等级，不可升阶";

	static const unsigned int ID_NOT_ENOUGH_JINGJIE = 5;
	static const char* STR_NOT_ENOUGH_JINGJIE = "境界点不足";

	static const unsigned int ID_MAX_QUALITY_LEVEL = 6;
	static const char* STR_MAX_QUALITY_LEVEL = "境界已达到顶阶";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_JINGJIE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_JINGJIE_UNLOCK
	,STR_QUALITY_INVALID
	,STR_LEVEL_INVALID
	,STR_NOT_REACH_MAX_LEVEL
	,STR_NOT_ENOUGH_JINGJIE
	,STR_MAX_QUALITY_LEVEL
	};
}

namespace ERR_GEN_GAS_BALL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_REACH_MAX_NUM = 1;
	static const char* STR_REACH_MAX_NUM = "已经达到最大次数";

	static const unsigned int ID_COLLECT_GAS_UNLOCK = 2;
	static const char* STR_COLLECT_GAS_UNLOCK = "聚魂功能尚未解锁";

	static const unsigned int ID_VIP_NOT_ENOUGH_UNLOCK = 3;
	static const char* STR_VIP_NOT_ENOUGH_UNLOCK = "未达到VIP等级，尚未解锁";

	static const char* STR_NAMESPACE = "ERR_GEN_GAS_BALL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_REACH_MAX_NUM
	,STR_COLLECT_GAS_UNLOCK
	,STR_VIP_NOT_ENOUGH_UNLOCK
	};
}

namespace ERR_UP_GAS_BALL_LEVEL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_VIP_NOT_ENOUGH_UNLOCK = 1;
	static const char* STR_VIP_NOT_ENOUGH_UNLOCK = "未达到VIP等级，尚未解锁";

	static const unsigned int ID_UN_GEN = 2;
	static const char* STR_UN_GEN = "尚未聚魂";

	static const unsigned int ID_COLLECT_GAS_UNLOCK = 3;
	static const char* STR_COLLECT_GAS_UNLOCK = "聚魂功能尚未解锁";

	static const unsigned int ID_COLLECT_GAS_FULL = 4;
	static const char* STR_COLLECT_GAS_FULL = "已使用一键满成长";

	static const char* STR_NAMESPACE = "ERR_UP_GAS_BALL_LEVEL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_VIP_NOT_ENOUGH_UNLOCK
	,STR_UN_GEN
	,STR_COLLECT_GAS_UNLOCK
	,STR_COLLECT_GAS_FULL
	};
}

namespace ERR_COLLECT_GAS 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UN_GEN = 1;
	static const char* STR_UN_GEN = "尚未聚魂";

	static const unsigned int ID_COLLECT_GAS_UNLOCK = 2;
	static const char* STR_COLLECT_GAS_UNLOCK = "聚魂功能尚未解锁";

	static const char* STR_NAMESPACE = "ERR_COLLECT_GAS";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UN_GEN
	,STR_COLLECT_GAS_UNLOCK
	};
}

namespace ERR_UPGRADE_SKILL_ACTION_LEVEL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SKILL_UNLOCK = 1;
	static const char* STR_SKILL_UNLOCK = "技能未解锁";

	static const unsigned int ID_ACTION_NOT_EXIST = 2;
	static const char* STR_ACTION_NOT_EXIST = "招式不存在";

	static const unsigned int ID_LEVEL_UNLOCK = 3;
	static const char* STR_LEVEL_UNLOCK = "当前无法升级，升级主角可提高招式等级上限";

	static const unsigned int ID_LEVEL_INVALID = 4;
	static const char* STR_LEVEL_INVALID = "等级无效";

	static const unsigned int ID_REACH_MAX_LEVEL = 5;
	static const char* STR_REACH_MAX_LEVEL = "已经达到最大等级";

	static const unsigned int ID_CAN_NOT_GREATE_PRE = 6;
	static const char* STR_CAN_NOT_GREATE_PRE = "不能大于前置招式等级";

	static const unsigned int ID_NOT_ENOUGH_PURPLEGAS = 7;
	static const char* STR_NOT_ENOUGH_PURPLEGAS = "功力不足";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_SKILL_ACTION_LEVEL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SKILL_UNLOCK
	,STR_ACTION_NOT_EXIST
	,STR_LEVEL_UNLOCK
	,STR_LEVEL_INVALID
	,STR_REACH_MAX_LEVEL
	,STR_CAN_NOT_GREATE_PRE
	,STR_NOT_ENOUGH_PURPLEGAS
	};
}

namespace ERR_SELECT_SKILL_ACTION_EFFECT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SKILL_UNLOCK = 1;
	static const char* STR_SKILL_UNLOCK = "境界未解锁";

	static const unsigned int ID_ACTION_UNLOCK = 2;
	static const char* STR_ACTION_UNLOCK = "招式未解锁";

	static const unsigned int ID_EFFECT_UNLOCK = 3;
	static const char* STR_EFFECT_UNLOCK = "效果未解锁";

	static const unsigned int ID_RANG_UNLOCK = 4;
	static const char* STR_RANG_UNLOCK = "范围未解锁";

	static const char* STR_NAMESPACE = "ERR_SELECT_SKILL_ACTION_EFFECT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SKILL_UNLOCK
	,STR_ACTION_UNLOCK
	,STR_EFFECT_UNLOCK
	,STR_RANG_UNLOCK
	};
}

namespace ERR_OPEN_JINGJIE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_JINGJIE_UNLOCK = 1;
	static const char* STR_JINGJIE_UNLOCK = "境界尚未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_JINGJIE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_JINGJIE_UNLOCK
	};
}

namespace ERR_OPEN_GEN_GAS_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_COLLECT_GAS_UNLOCK = 1;
	static const char* STR_COLLECT_GAS_UNLOCK = "收集器魂未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_GEN_GAS_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_COLLECT_GAS_UNLOCK
	};
}

namespace ERR_OPEN_SKILL_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SKILL_UNLOCK = 1;
	static const char* STR_SKILL_UNLOCK = "境界尚未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_SKILL_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SKILL_UNLOCK
	};
}

namespace ERR_CLEAR_JIEBIAO_CD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_CLEAR_JIEBIAO_CD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_VIP_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_VIP_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_XIANGQIAN_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_XIANGQIAN_UNLOCK = 1;
	static const char* STR_XIANGQIAN_UNLOCK = "镶嵌功能尚未解锁";

	static const unsigned int ID_XIANGQIAN_FUNC_NOT_EXIST = 2;
	static const char* STR_XIANGQIAN_FUNC_NOT_EXIST = "镶嵌功能不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_XIANGQIAN_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_XIANGQIAN_UNLOCK
	,STR_XIANGQIAN_FUNC_NOT_EXIST
	};
}

namespace ERR_XIANGQIAN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_EQUIP_NOT_EXIST = 1;
	static const char* STR_EQUIP_NOT_EXIST = "装备不存在";

	static const unsigned int ID_GOODS_NOT_EXIST = 2;
	static const char* STR_GOODS_NOT_EXIST = "物品不存在";

	static const unsigned int ID_HOLE_LOCK = 3;
	static const char* STR_HOLE_LOCK = "镶嵌孔未解锁，无法镶嵌";

	static const unsigned int ID_NOT_EXIST_HOLE = 4;
	static const char* STR_NOT_EXIST_HOLE = "该镶嵌孔不存在";

	static const unsigned int ID_ALREADY_XIANGQIAN = 5;
	static const char* STR_ALREADY_XIANGQIAN = "该镶嵌孔已经镶嵌仙石";

	static const unsigned int ID_BUILD_UNLOCK = 6;
	static const char* STR_BUILD_UNLOCK = "镶嵌功能尚未解锁";

	static const unsigned int ID_ALREADY_XIANGQIAN_THIS_TYPE = 7;
	static const char* STR_ALREADY_XIANGQIAN_THIS_TYPE = "该位置不能镶嵌";

	static const unsigned int ID_XIANGQIAN_GEM_CHANGE = 8;
	static const char* STR_XIANGQIAN_GEM_CHANGE = "替换仙石失败，请检查背包是否已满";

	static const unsigned int ID_EQUIP_POS_CANNOT_XIANGQIAN = 9;
	static const char* STR_EQUIP_POS_CANNOT_XIANGQIAN = "该阶装备不能镶嵌";

	static const char* STR_NAMESPACE = "ERR_XIANGQIAN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_EQUIP_NOT_EXIST
	,STR_GOODS_NOT_EXIST
	,STR_HOLE_LOCK
	,STR_NOT_EXIST_HOLE
	,STR_ALREADY_XIANGQIAN
	,STR_BUILD_UNLOCK
	,STR_ALREADY_XIANGQIAN_THIS_TYPE
	,STR_XIANGQIAN_GEM_CHANGE
	,STR_EQUIP_POS_CANNOT_XIANGQIAN
	};
}

namespace ERR_CHECK_APPSTORE_ORDER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PROCESS_ERROR_WITH_APPLE = 1;
	static const char* STR_PROCESS_ERROR_WITH_APPLE = "与苹果服务器交互过程中发生错误";

	static const unsigned int ID_ORDER_ID_ERROR = 2;
	static const char* STR_ORDER_ID_ERROR = "充值订单非法";

	static const unsigned int ID_PLAYER_ID_ERROR = 3;
	static const char* STR_PLAYER_ID_ERROR = "玩家ID错误，充值失败";

	static const unsigned int ID_SERVER_INNER_ERROR = 4;
	static const char* STR_SERVER_INNER_ERROR = "服务器内部错误，充值失败，请联系GM";

	static const char* STR_NAMESPACE = "ERR_CHECK_APPSTORE_ORDER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PROCESS_ERROR_WITH_APPLE
	,STR_ORDER_ID_ERROR
	,STR_PLAYER_ID_ERROR
	,STR_SERVER_INNER_ERROR
	};
}

namespace ERR_REMOVE_XIANGQIAN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_EQUIP_NOT_EXIST = 1;
	static const char* STR_EQUIP_NOT_EXIST = "装备不存在";

	static const unsigned int ID_EQUIP_NOT_XIANGQIAN = 2;
	static const char* STR_EQUIP_NOT_XIANGQIAN = "该镶嵌孔没有仙石";

	static const unsigned int ID_BUILD_UNLOCK = 3;
	static const char* STR_BUILD_UNLOCK = "镶嵌功能尚未解锁";

	static const unsigned int ID_CHECK_BAG_AREA = 4;
	static const char* STR_CHECK_BAG_AREA = "摘除失败，请检查背包空间";

	static const char* STR_NAMESPACE = "ERR_REMOVE_XIANGQIAN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_EQUIP_NOT_EXIST
	,STR_EQUIP_NOT_XIANGQIAN
	,STR_BUILD_UNLOCK
	,STR_CHECK_BAG_AREA
	};
}

namespace ERR_RESET_ELITE_INSTANCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_TOWN_NOT_EXIST = 1;
	static const char* STR_TOWN_NOT_EXIST = "该组副本不存在";

	static const unsigned int ID_INDEX_ERROR = 2;
	static const char* STR_INDEX_ERROR = "重置次数有误";

	static const unsigned int ID_RESET_ERROR = 3;
	static const char* STR_RESET_ERROR = "重置发生错误";

	static const char* STR_NAMESPACE = "ERR_RESET_ELITE_INSTANCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_TOWN_NOT_EXIST
	,STR_INDEX_ERROR
	,STR_RESET_ERROR
	};
}

namespace ERR_ELITE_INSTANCE_ON_HOOK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_PHYSTRENGTH_NOT_ENOUGH = 1;
	static const char* STR_PHYSTRENGTH_NOT_ENOUGH = "体力不足";

	static const unsigned int ID_BAG_FULL = 2;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_INSTANCE_NOT_EXIST = 3;
	static const char* STR_INSTANCE_NOT_EXIST = "副本不存在";

	static const unsigned int ID_INSTANCE_NOT_BATTLED = 4;
	static const char* STR_INSTANCE_NOT_BATTLED = "该副本尚未通关，无法扫荡";

	static const unsigned int ID_NOT_TOWER = 5;
	static const char* STR_NOT_TOWER = "该组副本不存在";

	static const unsigned int ID_NOT_SCIENCE = 6;
	static const char* STR_NOT_SCIENCE = "场景不存在";

	static const unsigned int ID_ONHOOK_UNLOCK = 7;
	static const char* STR_ONHOOK_UNLOCK = "扫荡未解锁";

	static const unsigned int ID_TODAY_ALREADY_BATTLED = 8;
	static const char* STR_TODAY_ALREADY_BATTLED = "今日已经挑战";

	static const char* STR_NAMESPACE = "ERR_ELITE_INSTANCE_ON_HOOK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_PHYSTRENGTH_NOT_ENOUGH
	,STR_BAG_FULL
	,STR_INSTANCE_NOT_EXIST
	,STR_INSTANCE_NOT_BATTLED
	,STR_NOT_TOWER
	,STR_NOT_SCIENCE
	,STR_ONHOOK_UNLOCK
	,STR_TODAY_ALREADY_BATTLED
	};
}

namespace ERR_GET_ACTIVITY_PROMPT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_GET_ACTIVITY_PROMPT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_BLUE_TO_PURPLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_JINGJIE_UNLOCK = 1;
	static const char* STR_JINGJIE_UNLOCK = "境界尚未解锁";

	static const unsigned int ID_BLUE_NOT_ENOUGH = 2;
	static const char* STR_BLUE_NOT_ENOUGH = "蓝气不足";

	static const char* STR_NAMESPACE = "ERR_BLUE_TO_PURPLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_JINGJIE_UNLOCK
	,STR_BLUE_NOT_ENOUGH
	};
}

namespace ERR_REPORT_USER_DEVICE_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_REPORT_USER_DEVICE_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_PKBATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ENEMY_NOT_EXIST = 1;
	static const char* STR_ENEMY_NOT_EXIST = "目标不存在";

	static const unsigned int ID_ENEMY_MYSELF = 2;
	static const char* STR_ENEMY_MYSELF = "不可和自己切磋";

	static const char* STR_NAMESPACE = "ERR_PKBATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ENEMY_NOT_EXIST
	,STR_ENEMY_MYSELF
	};
}

namespace ERR_DEVICE_UN_BIND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_DEVICE_UN_BIND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_ENTER_BOSSB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const unsigned int ID_BOSSB_UNSTART = 2;
	static const char* STR_BOSSB_UNSTART = "活动未开始";

	static const char* STR_NAMESPACE = "ERR_ENTER_BOSSB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	,STR_BOSSB_UNSTART
	};
}

namespace ERR_LEAVE_BOSSB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const char* STR_NAMESPACE = "ERR_LEAVE_BOSSB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	};
}

namespace ERR_BOSSB_MOVE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const unsigned int ID_BOSSB_UNSTART = 2;
	static const char* STR_BOSSB_UNSTART = "活动未开始";

	static const unsigned int ID_RELIVE_CD = 3;
	static const char* STR_RELIVE_CD = "复活CD中";

	static const unsigned int ID_MOVE_FREQUENTLY = 4;
	static const char* STR_MOVE_FREQUENTLY = "移动过于频繁";

	static const unsigned int ID_MOVE_LEN_INVALID = 5;
	static const char* STR_MOVE_LEN_INVALID = "移动距离非法";

	static const unsigned int ID_NOT_IN_ACTIVITY = 6;
	static const char* STR_NOT_IN_ACTIVITY = "未进入活动中";

	static const unsigned int ID_READY_CD = 7;
	static const char* STR_READY_CD = "战斗尚未开始，不可离开准备区";

	static const char* STR_NAMESPACE = "ERR_BOSSB_MOVE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	,STR_BOSSB_UNSTART
	,STR_RELIVE_CD
	,STR_MOVE_FREQUENTLY
	,STR_MOVE_LEN_INVALID
	,STR_NOT_IN_ACTIVITY
	,STR_READY_CD
	};
}

namespace ERR_BOSSB_HURT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const unsigned int ID_BOSSB_UNSTART = 2;
	static const char* STR_BOSSB_UNSTART = "活动未开始";

	static const unsigned int ID_RELIVE_CD = 3;
	static const char* STR_RELIVE_CD = "复活CD中";

	static const unsigned int ID_NOT_IN_ACTIVITY = 4;
	static const char* STR_NOT_IN_ACTIVITY = "未进入活动中";

	static const unsigned int ID_READY_CD = 5;
	static const char* STR_READY_CD = "战斗尚未开始，不可离开准备区";

	static const char* STR_NAMESPACE = "ERR_BOSSB_HURT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	,STR_BOSSB_UNSTART
	,STR_RELIVE_CD
	,STR_NOT_IN_ACTIVITY
	,STR_READY_CD
	};
}

namespace ERR_BOSSB_POS_NTF 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const char* STR_NAMESPACE = "ERR_BOSSB_POS_NTF";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	};
}

namespace ERR_BOSSB_PLAYER_HRUT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const char* STR_NAMESPACE = "ERR_BOSSB_PLAYER_HRUT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	};
}

namespace ERR_OPEN_INSPIRE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const unsigned int ID_NOT_IN_ACTIVITY = 2;
	static const char* STR_NOT_IN_ACTIVITY = "未进入活动中";

	static const unsigned int ID_BOSSB_UNSTART = 3;
	static const char* STR_BOSSB_UNSTART = "活动未开始";

	static const char* STR_NAMESPACE = "ERR_OPEN_INSPIRE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	,STR_NOT_IN_ACTIVITY
	,STR_BOSSB_UNSTART
	};
}

namespace ERR_INSPIRE_POWER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const unsigned int ID_STORY_NOT_ENOUGH = 2;
	static const char* STR_STORY_NOT_ENOUGH = "阅历不足";

	static const unsigned int ID_NOT_IN_ACTIVITY = 3;
	static const char* STR_NOT_IN_ACTIVITY = "未进入活动中";

	static const unsigned int ID_BOSSB_UNSTART = 4;
	static const char* STR_BOSSB_UNSTART = "活动未开始";

	static const unsigned int ID_REACH_MAX = 5;
	static const char* STR_REACH_MAX = "战力加成已达到上限";

	static const char* STR_NAMESPACE = "ERR_INSPIRE_POWER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	,STR_STORY_NOT_ENOUGH
	,STR_NOT_IN_ACTIVITY
	,STR_BOSSB_UNSTART
	,STR_REACH_MAX
	};
}

namespace ERR_BOSSB_RELIVE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const unsigned int ID_NOT_IN_ACTIVITY = 2;
	static const char* STR_NOT_IN_ACTIVITY = "未进入活动中";

	static const unsigned int ID_BOSSB_UNSTART = 3;
	static const char* STR_BOSSB_UNSTART = "活动未开始";

	static const unsigned int ID_GOLD_RELIVE_CD = 4;
	static const char* STR_GOLD_RELIVE_CD = "元宝复活CD中";

	static const char* STR_NAMESPACE = "ERR_BOSSB_RELIVE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	,STR_NOT_IN_ACTIVITY
	,STR_BOSSB_UNSTART
	,STR_GOLD_RELIVE_CD
	};
}

namespace ERR_GET_LAST_BOSSB_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const unsigned int ID_BOSSB_UNOVER = 2;
	static const char* STR_BOSSB_UNOVER = "boss战尚未结束";

	static const char* STR_NAMESPACE = "ERR_GET_LAST_BOSSB_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	,STR_BOSSB_UNOVER
	};
}

namespace ERR_OPEN_RECHARGE_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RECEVED_RECHARGE_ENCOURAGE_ERR = 1;
	static const char* STR_RECEVED_RECHARGE_ENCOURAGE_ERR = "已领取充值奖励";

	static const unsigned int ID_NOT_EXIST_BUILD_ERR = 2;
	static const char* STR_NOT_EXIST_BUILD_ERR = "功能未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_RECHARGE_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RECEVED_RECHARGE_ENCOURAGE_ERR
	,STR_NOT_EXIST_BUILD_ERR
	};
}

namespace ERR_RECV_RECHARGE_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_RECHARGE_ENCOURAGE_ERR = 1;
	static const char* STR_NO_RECHARGE_ENCOURAGE_ERR = "未充值,不能领取奖励";

	static const unsigned int ID_RECEVED_RECHARGE_ENCOURAGE_ERR = 2;
	static const char* STR_RECEVED_RECHARGE_ENCOURAGE_ERR = "已领取充值奖励";

	static const unsigned int ID_BAG_FULL_ERR = 3;
	static const char* STR_BAG_FULL_ERR = "背包已满，请先整理背包";

	static const unsigned int ID_NOT_EXIST_BUILD_ERR = 4;
	static const char* STR_NOT_EXIST_BUILD_ERR = "功能尚未开放";

	static const unsigned int ID_UPPER_LIMIT_ERR = 5;
	static const char* STR_UPPER_LIMIT_ERR = "所携带伙伴数量达到上限";

	static const char* STR_NAMESPACE = "ERR_RECV_RECHARGE_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_RECHARGE_ENCOURAGE_ERR
	,STR_RECEVED_RECHARGE_ENCOURAGE_ERR
	,STR_BAG_FULL_ERR
	,STR_NOT_EXIST_BUILD_ERR
	,STR_UPPER_LIMIT_ERR
	};
}

namespace ERR_OPEN_SCORE_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RECEVED_RECHARGE_ENCOURAGE_ERR = 1;
	static const char* STR_RECEVED_RECHARGE_ENCOURAGE_ERR = "已领取评分奖励";

	static const unsigned int ID_NOT_EXIST_BUILD_ERR = 2;
	static const char* STR_NOT_EXIST_BUILD_ERR = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_SCORE_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RECEVED_RECHARGE_ENCOURAGE_ERR
	,STR_NOT_EXIST_BUILD_ERR
	};
}

namespace ERR_RECV_SCORE_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RECEVED_RECHARGE_ENCOURAGE_ERR = 1;
	static const char* STR_RECEVED_RECHARGE_ENCOURAGE_ERR = "已领取评分奖励";

	static const unsigned int ID_BAG_FULL_ERR = 2;
	static const char* STR_BAG_FULL_ERR = "背包已满，请先整理背包";

	static const unsigned int ID_NOT_EXIST_BUILD_ERR = 3;
	static const char* STR_NOT_EXIST_BUILD_ERR = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_RECV_SCORE_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RECEVED_RECHARGE_ENCOURAGE_ERR
	,STR_BAG_FULL_ERR
	,STR_NOT_EXIST_BUILD_ERR
	};
}

namespace ERR_GET_RANDOM_DSPNAME 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_GET_RANDOM_DSPNAME";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_BOSSB_BATTLE_LOG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BOSSB_UNLOCK = 1;
	static const char* STR_BOSSB_UNLOCK = "boss战尚未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_BOSSB_BATTLE_LOG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BOSSB_UNLOCK
	};
}

namespace ERR_OPEN_EXPERIENCE_TRANSFER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_NOT_EXIT = 1;
	static const char* STR_HERO_NOT_EXIT = "伙伴不存在";

	static const unsigned int ID_BUILD_NOT_EXIT = 2;
	static const char* STR_BUILD_NOT_EXIT = "经验传承尚未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_EXPERIENCE_TRANSFER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_NOT_EXIT
	,STR_BUILD_NOT_EXIT
	};
}

namespace ERR_EXPERIENCE_TRANSFER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_NOT_EXIT = 1;
	static const char* STR_HERO_NOT_EXIT = "传承经验的伙伴不存在";

	static const unsigned int ID_TRANS_HERO_NOT_EXIT = 2;
	static const char* STR_TRANS_HERO_NOT_EXIT = "接受经验传承的伙伴不存在";

	static const unsigned int ID_TRANS_HERO_EQUIL = 3;
	static const char* STR_TRANS_HERO_EQUIL = "无法接受自己的经验传承";

	static const unsigned int ID_TRANS_HERO_OVER_STEP = 4;
	static const char* STR_TRANS_HERO_OVER_STEP = "该伙伴已接受过经验传承";

	static const unsigned int ID_TRANS_HERO_COACH = 5;
	static const char* STR_TRANS_HERO_COACH = "主角不可接受经验传承";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 6;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_BUILD_NOT_EXIT = 7;
	static const char* STR_BUILD_NOT_EXIT = "经验传承尚未开放";

	static const unsigned int ID_HERO_TWICE_ERR = 8;
	static const char* STR_HERO_TWICE_ERR = "伙伴只可接受一次经验传承";

	static const unsigned int ID_HERO_MUTUAL_TRANSFER = 9;
	static const char* STR_HERO_MUTUAL_TRANSFER = "两个伙伴间不可互相接受经验传承";

	static const char* STR_NAMESPACE = "ERR_EXPERIENCE_TRANSFER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_NOT_EXIT
	,STR_TRANS_HERO_NOT_EXIT
	,STR_TRANS_HERO_EQUIL
	,STR_TRANS_HERO_OVER_STEP
	,STR_TRANS_HERO_COACH
	,STR_GOLD_NOT_ENOUGH
	,STR_BUILD_NOT_EXIT
	,STR_HERO_TWICE_ERR
	,STR_HERO_MUTUAL_TRANSFER
	};
}

namespace ERR_OPEN_RDCHALLENGE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "随机挑战未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_RDCHALLENGE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	};
}

namespace ERR_RDCHALLENGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "随机挑战功能尚未解锁";

	static const unsigned int ID_ENEMY_NOT_EXIST = 2;
	static const char* STR_ENEMY_NOT_EXIST = "目标不存在";

	static const unsigned int ID_NUM_OVER = 3;
	static const char* STR_NUM_OVER = "挑战次数已用完";

	static const unsigned int ID_IN_CD = 4;
	static const char* STR_IN_CD = "挑战CD中";

	static const unsigned int ID_HAVE_CHALLENGE = 5;
	static const char* STR_HAVE_CHALLENGE = "已挑战";

	static const unsigned int ID_NOT_REFRESH_PLAYER = 6;
	static const char* STR_NOT_REFRESH_PLAYER = "不可挑战非正常刷新出现的目标";

	static const unsigned int ID_NOT_BAG = 7;
	static const char* STR_NOT_BAG = "背包已满请整理背包";

	static const char* STR_NAMESPACE = "ERR_RDCHALLENGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	,STR_ENEMY_NOT_EXIST
	,STR_NUM_OVER
	,STR_IN_CD
	,STR_HAVE_CHALLENGE
	,STR_NOT_REFRESH_PLAYER
	,STR_NOT_BAG
	};
}

namespace ERR_CLR_RDCHALLENGE_CD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "随机挑战功能尚未解锁";

	static const char* STR_NAMESPACE = "ERR_CLR_RDCHALLENGE_CD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	};
}

namespace ERR_OPEN_WORSHIP_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "膜拜功能尚未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_WORSHIP_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	};
}

namespace ERR_WORSHIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "膜拜功能尚未解锁";

	static const unsigned int ID_HAVE_DONE = 2;
	static const char* STR_HAVE_DONE = "今日已膜拜";

	static const unsigned int ID_NO_DATA = 3;
	static const char* STR_NO_DATA = "该膜拜信息不存在";

	static const char* STR_NAMESPACE = "ERR_WORSHIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	,STR_HAVE_DONE
	,STR_NO_DATA
	};
}

namespace ERR_LOGOUT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_USERNAME_EMPTY_ERR = 1;
	static const char* STR_USERNAME_EMPTY_ERR = "用户名不能为空";

	static const unsigned int ID_INVALID_CHARACTER = 2;
	static const char* STR_INVALID_CHARACTER = "无效字符";

	static const unsigned int ID_UNBIND_ACCOUNT = 3;
	static const char* STR_UNBIND_ACCOUNT = "尚未注册账号，不能进行解绑操作";

	static const char* STR_NAMESPACE = "ERR_LOGOUT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_USERNAME_EMPTY_ERR
	,STR_INVALID_CHARACTER
	,STR_UNBIND_ACCOUNT
	};
}

namespace ERR_CHANGE_PWD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_CHARACTER = 1;
	static const char* STR_INVALID_CHARACTER = "无效字符";

	static const unsigned int ID_USER_PWD_ERR = 2;
	static const char* STR_USER_PWD_ERR = "用户密码出错";

	static const unsigned int ID_USER_AUTH_TYPE_ERR = 3;
	static const char* STR_USER_AUTH_TYPE_ERR = "用户类型出错";

	static const char* STR_NAMESPACE = "ERR_CHANGE_PWD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_CHARACTER
	,STR_USER_PWD_ERR
	,STR_USER_AUTH_TYPE_ERR
	};
}

namespace ERR_UNBIND_DEVICE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_USERNAME_EMPTY_ERR = 1;
	static const char* STR_USERNAME_EMPTY_ERR = "用户名不能为空";

	static const unsigned int ID_INVALID_CHARACTER = 2;
	static const char* STR_INVALID_CHARACTER = "无效字符";

	static const unsigned int ID_UNBIND_ACCOUNT = 3;
	static const char* STR_UNBIND_ACCOUNT = "尚未注册账号，不能进行解绑操作";

	static const char* STR_NAMESPACE = "ERR_UNBIND_DEVICE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_USERNAME_EMPTY_ERR
	,STR_INVALID_CHARACTER
	,STR_UNBIND_ACCOUNT
	};
}

namespace ERR_ACTIVITY_NOTIFY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ACTIVITY_TYPE_NOT_EXIT = 1;
	static const char* STR_ACTIVITY_TYPE_NOT_EXIT = "活动类型不存在";

	static const unsigned int ID_ACTIVITY_NOT_OPEN = 2;
	static const char* STR_ACTIVITY_NOT_OPEN = "活动尚未开放";

	static const char* STR_NAMESPACE = "ERR_ACTIVITY_NOTIFY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ACTIVITY_TYPE_NOT_EXIT
	,STR_ACTIVITY_NOT_OPEN
	};
}

namespace ERR_OPEN_DOGFECES 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_DOGFECES";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_GET_ACTIVITY_CENTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNLOCK = 1;
	static const char* STR_UNLOCK = "活动中心尚未开放";

	static const char* STR_NAMESPACE = "ERR_GET_ACTIVITY_CENTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNLOCK
	};
}

namespace ERR_GET_USERINFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FORBID = 1;
	static const char* STR_FORBID = "该设备被禁用";

	static const char* STR_NAMESPACE = "ERR_GET_USERINFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FORBID
	};
}

namespace ERR_GET_ACTIVITY_ENCOURRAGE_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UPDATE_NEW_VERSION = 1;
	static const char* STR_UPDATE_NEW_VERSION = "请更新新版本客户端";

	static const char* STR_NAMESPACE = "ERR_GET_ACTIVITY_ENCOURRAGE_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UPDATE_NEW_VERSION
	};
}

namespace ERR_RECV_ACTIVITY_ENCOURRAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BAGFUL = 1;
	static const char* STR_BAGFUL = "背包已满";

	static const unsigned int ID_ENCOURRAGE_RECVD = 2;
	static const char* STR_ENCOURRAGE_RECVD = "奖励已领取";

	static const unsigned int ID_ACTIVITY_NOT_EXIT = 3;
	static const char* STR_ACTIVITY_NOT_EXIT = "奖励信息不存在";

	static const unsigned int ID_ACTIVITY_NEED_CD_KEY = 4;
	static const char* STR_ACTIVITY_NEED_CD_KEY = "需要输入CD_KEY才可领取";

	static const unsigned int ID_CD_KEY_NOT_EXIT = 5;
	static const char* STR_CD_KEY_NOT_EXIT = "CD_KEY不存在";

	static const unsigned int ID_CD_KEY_IS_USERD = 6;
	static const char* STR_CD_KEY_IS_USERD = "CD_KEY已被使用";

	static const unsigned int ID_HERO_CARRY_FULL = 7;
	static const char* STR_HERO_CARRY_FULL = "武将携带数量已达到上限";

	static const unsigned int ID_UPDATE_NEW_VERSION = 8;
	static const char* STR_UPDATE_NEW_VERSION = "请更新新版本客户端";

	static const char* STR_NAMESPACE = "ERR_RECV_ACTIVITY_ENCOURRAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BAGFUL
	,STR_ENCOURRAGE_RECVD
	,STR_ACTIVITY_NOT_EXIT
	,STR_ACTIVITY_NEED_CD_KEY
	,STR_CD_KEY_NOT_EXIT
	,STR_CD_KEY_IS_USERD
	,STR_HERO_CARRY_FULL
	,STR_UPDATE_NEW_VERSION
	};
}

namespace ERR_OPEN_GEM_MAKER_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_GEM_MAKER_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_RECV_RATING_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SCREEN_NOT_EXIT = 1;
	static const char* STR_SCREEN_NOT_EXIT = "场景不存在";

	static const unsigned int ID_TYPE_NOT_EXIT = 2;
	static const char* STR_TYPE_NOT_EXIT = "不存在该类型";

	static const unsigned int ID_CHAPTER_NOT_EXIT = 3;
	static const char* STR_CHAPTER_NOT_EXIT = "章节不存在";

	static const unsigned int ID_RAGING_NOT_ENOUGH = 4;
	static const char* STR_RAGING_NOT_ENOUGH = "星星数量不足";

	static const unsigned int ID_ENCOURAGE_RECEVED = 5;
	static const char* STR_ENCOURAGE_RECEVED = "奖励已领取";

	static const unsigned int ID_NOT_THROUGH = 6;
	static const char* STR_NOT_THROUGH = "未全部通关";

	static const unsigned int ID_BAGFUL = 7;
	static const char* STR_BAGFUL = "背包已满";

	static const char* STR_NAMESPACE = "ERR_RECV_RATING_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SCREEN_NOT_EXIT
	,STR_TYPE_NOT_EXIT
	,STR_CHAPTER_NOT_EXIT
	,STR_RAGING_NOT_ENOUGH
	,STR_ENCOURAGE_RECEVED
	,STR_NOT_THROUGH
	,STR_BAGFUL
	};
}

namespace ERR_OPEN_AWAKEN_EXCHANGE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const unsigned int ID_STUDY_EXCHANGE_UNLOCK = 2;
	static const char* STR_STUDY_EXCHANGE_UNLOCK = "卦象兑换未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_AWAKEN_EXCHANGE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	,STR_STUDY_EXCHANGE_UNLOCK
	};
}

namespace ERR_EXCHANGE_STUDY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_AWAKEN_UNLOCK = 1;
	static const char* STR_AWAKEN_UNLOCK = "算卦未解锁";

	static const unsigned int ID_NOT_ENOUGH_CLIP = 2;
	static const char* STR_NOT_ENOUGH_CLIP = "乾坤碎片不足";

	static const unsigned int ID_PLAYER_LEVEL_NOT_REACH = 3;
	static const char* STR_PLAYER_LEVEL_NOT_REACH = "未达到开放兑换的等级";

	static const unsigned int ID_STUDY_NOT_EXIST = 4;
	static const char* STR_STUDY_NOT_EXIST = "要兑换的卦象不存在";

	static const unsigned int ID_STUDY_BAG_FULL = 5;
	static const char* STR_STUDY_BAG_FULL = "卦象背包已满";

	static const unsigned int ID_STUDY_EXCHANGE_UNLOCK = 6;
	static const char* STR_STUDY_EXCHANGE_UNLOCK = "卦象兑换未解锁";

	static const char* STR_NAMESPACE = "ERR_EXCHANGE_STUDY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_AWAKEN_UNLOCK
	,STR_NOT_ENOUGH_CLIP
	,STR_PLAYER_LEVEL_NOT_REACH
	,STR_STUDY_NOT_EXIST
	,STR_STUDY_BAG_FULL
	,STR_STUDY_EXCHANGE_UNLOCK
	};
}

namespace ERR_OPEN_EQUIP_COMPOSE_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_EQUIP_COMPOSE_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_EQUIP_COMPOSE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_EQUIP_LEVEL_INVALID = 1;
	static const char* STR_EQUIP_LEVEL_INVALID = "请求装备等级无效";

	static const unsigned int ID_CLIP_NOT_ENOUGH = 2;
	static const char* STR_CLIP_NOT_ENOUGH = "碎片不足";

	static const unsigned int ID_IN_CD = 3;
	static const char* STR_IN_CD = "合成CD中";

	static const char* STR_NAMESPACE = "ERR_EQUIP_COMPOSE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_EQUIP_LEVEL_INVALID
	,STR_CLIP_NOT_ENOUGH
	,STR_IN_CD
	};
}

namespace ERR_RECV_COMPOSE_EQUIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNCOMPOSE = 1;
	static const char* STR_UNCOMPOSE = "尚未合成装备";

	static const unsigned int ID_BAG_FULL = 2;
	static const char* STR_BAG_FULL = "背包已满";

	static const unsigned int ID_EQUIP_LEVEL_INVALID = 3;
	static const char* STR_EQUIP_LEVEL_INVALID = "请求装备等级无效";

	static const char* STR_NAMESPACE = "ERR_RECV_COMPOSE_EQUIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNCOMPOSE
	,STR_BAG_FULL
	,STR_EQUIP_LEVEL_INVALID
	};
}

namespace ERR_OPEN_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_NOT_JOIN_FACTION = 2;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const unsigned int ID_HEAD_INFO_NOT_FIND = 3;
	static const char* STR_HEAD_INFO_NOT_FIND = "找不到帮主信息，请联系GM";

	static const char* STR_NAMESPACE = "ERR_OPEN_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NOT_JOIN_FACTION
	,STR_HEAD_INFO_NOT_FIND
	};
}

namespace ERR_APPLY_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_NOT_EXIT_FACTION = 2;
	static const char* STR_NOT_EXIT_FACTION = "申请加入的门派不存在";

	static const unsigned int ID_HAVE_FACTION = 3;
	static const char* STR_HAVE_FACTION = "你已加入一个门派";

	static const unsigned int ID_HAVE_APPLY_FACTION = 4;
	static const char* STR_HAVE_APPLY_FACTION = "你已申请加入该门派";

	static const unsigned int ID_CAPS_PLAYER_FACTION = 5;
	static const char* STR_CAPS_PLAYER_FACTION = "成员数量已达到上限";

	static const unsigned int ID_TIME_NOT_ENOUGH = 6;
	static const char* STR_TIME_NOT_ENOUGH = "退出门派后24小时内不能加入门派";

	static const unsigned int ID_PLAYER_NOT_EXIST = 7;
	static const char* STR_PLAYER_NOT_EXIST = "玩家不存在";

	static const unsigned int ID_APPLY_FACTION_NUM_IS_MAX = 8;
	static const char* STR_APPLY_FACTION_NUM_IS_MAX = "你申请的门派个数已达上限";

	static const unsigned int ID_FACTION_PLAYER_IS_MAX = 9;
	static const char* STR_FACTION_PLAYER_IS_MAX = "该门派人数已满，请申请其它门派";

	static const unsigned int ID_APPLY_NUM_TOO_MUCH = 10;
	static const char* STR_APPLY_NUM_TOO_MUCH = "该门派申请人数过多，请稍后再试";

	static const char* STR_NAMESPACE = "ERR_APPLY_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NOT_EXIT_FACTION
	,STR_HAVE_FACTION
	,STR_HAVE_APPLY_FACTION
	,STR_CAPS_PLAYER_FACTION
	,STR_TIME_NOT_ENOUGH
	,STR_PLAYER_NOT_EXIST
	,STR_APPLY_FACTION_NUM_IS_MAX
	,STR_FACTION_PLAYER_IS_MAX
	,STR_APPLY_NUM_TOO_MUCH
	};
}

namespace ERR_CANCEL_APPLY_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_NOT_EXIT_FACTION = 2;
	static const char* STR_NOT_EXIT_FACTION = "该门派不存在";

	static const unsigned int ID_NOT_APPLY_FACTION = 3;
	static const char* STR_NOT_APPLY_FACTION = "你未申请加入该门派";

	static const char* STR_NAMESPACE = "ERR_CANCEL_APPLY_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NOT_EXIT_FACTION
	,STR_NOT_APPLY_FACTION
	};
}

namespace ERR_KICKED_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 2;
	static const char* STR_PLAYER_NOT_IN_FACTION = "门派不存在";

	static const unsigned int ID_NOT_APPLY_FACTION = 3;
	static const char* STR_NOT_APPLY_FACTION = "该玩家不在门派中";

	static const unsigned int ID_PLAYER_NOT_AUTHOR = 4;
	static const char* STR_PLAYER_NOT_AUTHOR = "你无权将该玩家踢出门派";

	static const unsigned int ID_NOT_EXIT_FACTION = 5;
	static const char* STR_NOT_EXIT_FACTION = "门派不存在";

	static const unsigned int ID_PLAYRE_ID_NOT_REAL = 6;
	static const char* STR_PLAYRE_ID_NOT_REAL = "玩家不存在";

	static const unsigned int ID_ONLY_KICKED_NORMAL = 7;
	static const char* STR_ONLY_KICKED_NORMAL = "只能踢出普通弟子";

	static const unsigned int ID_ONLY_KICKED_ELITE_AND_NORMAL = 8;
	static const char* STR_ONLY_KICKED_ELITE_AND_NORMAL = "只能踢出精英弟子和普通弟子";

	static const unsigned int ID_IN_GVG_ACTIVITY_CAN_NOT_KICK = 9;
	static const char* STR_IN_GVG_ACTIVITY_CAN_NOT_KICK = "参加门派跨服战期间无法踢出玩家";

	static const char* STR_NAMESPACE = "ERR_KICKED_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_IN_FACTION
	,STR_NOT_APPLY_FACTION
	,STR_PLAYER_NOT_AUTHOR
	,STR_NOT_EXIT_FACTION
	,STR_PLAYRE_ID_NOT_REAL
	,STR_ONLY_KICKED_NORMAL
	,STR_ONLY_KICKED_ELITE_AND_NORMAL
	,STR_IN_GVG_ACTIVITY_CAN_NOT_KICK
	};
}

namespace ERR_CREATE_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_HAVE_FACTION = 2;
	static const char* STR_PLAYER_HAVE_FACTION = "你已加入或创建了一个门派";

	static const unsigned int ID_COIN_NOT_ENOUGH = 3;
	static const char* STR_COIN_NOT_ENOUGH = "铜钱不足";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 4;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_FACTION_NAME_SAME = 5;
	static const char* STR_FACTION_NAME_SAME = "门派名已被使用";

	static const unsigned int ID_STRING_NOT_ENOUGH = 6;
	static const char* STR_STRING_NOT_ENOUGH = "门派名称中含有敏感词汇，请重新输入";

	static const unsigned int ID_NAME_TOO_LONG = 7;
	static const char* STR_NAME_TOO_LONG = "门派名称过长，不得超过6个汉字或12英文字符";

	static const unsigned int ID_TIME_NOT_ENOUGH = 8;
	static const char* STR_TIME_NOT_ENOUGH = "退出门派后24小时内无法创建门派或申请加入其它门派";

	static const unsigned int ID_EMOJI_NOT_USE = 9;
	static const char* STR_EMOJI_NOT_USE = "门派名只能使用中文、英文和数字";

	static const char* STR_NAMESPACE = "ERR_CREATE_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_HAVE_FACTION
	,STR_COIN_NOT_ENOUGH
	,STR_GOLD_NOT_ENOUGH
	,STR_FACTION_NAME_SAME
	,STR_STRING_NOT_ENOUGH
	,STR_NAME_TOO_LONG
	,STR_TIME_NOT_ENOUGH
	,STR_EMOJI_NOT_USE
	};
}

namespace ERR_SEARCH_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_NOT_PLAYER_FACTION_HEAD = 2;
	static const char* STR_NOT_PLAYER_FACTION_HEAD = "该门派掌门数据不存在";

	static const char* STR_NAMESPACE = "ERR_SEARCH_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NOT_PLAYER_FACTION_HEAD
	};
}

namespace ERR_OPEN_OWNER_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const char* STR_NAMESPACE = "ERR_OPEN_OWNER_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	};
}

namespace ERR_OPEN_DOORS_TRIBUTE_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_NOT_FIND_PLAYER = 4;
	static const char* STR_NOT_FIND_PLAYER = "找不到玩家数据";

	static const char* STR_NAMESPACE = "ERR_OPEN_DOORS_TRIBUTE_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_NOT_FIND_PLAYER
	};
}

namespace ERR_DOORS_TRIBUTE_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_COIN_NOT_ENOUGH = 4;
	static const char* STR_COIN_NOT_ENOUGH = "铜钱不足";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 5;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_COIN_NOT_TIME = 6;
	static const char* STR_COIN_NOT_TIME = "今日铜钱捐献次数达到上限";

	static const unsigned int ID_GOLD_NOT_TIME = 7;
	static const char* STR_GOLD_NOT_TIME = "今日元宝捐献次数达到上限";

	static const char* STR_NAMESPACE = "ERR_DOORS_TRIBUTE_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_COIN_NOT_ENOUGH
	,STR_GOLD_NOT_ENOUGH
	,STR_COIN_NOT_TIME
	,STR_GOLD_NOT_TIME
	};
}

namespace ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_BAG_FULL = 4;
	static const char* STR_BAG_FULL = "背包已满";

	static const unsigned int ID_CONDITION_NOT_ENOUGH = 5;
	static const char* STR_CONDITION_NOT_ENOUGH = "铜钱捐献次数还未达到上限";

	static const unsigned int ID_REWARD_IS_GET = 6;
	static const char* STR_REWARD_IS_GET = "已领取";

	static const unsigned int ID_GOODS_INFO_EXCEPTION = 7;
	static const char* STR_GOODS_INFO_EXCEPTION = "物品数据异常，请联系GM";

	static const unsigned int ID_GET_REWARD_NOT_ENOUGH = 8;
	static const char* STR_GET_REWARD_NOT_ENOUGH = "请先把铜钱捐满";

	static const char* STR_NAMESPACE = "ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_BAG_FULL
	,STR_CONDITION_NOT_ENOUGH
	,STR_REWARD_IS_GET
	,STR_GOODS_INFO_EXCEPTION
	,STR_GET_REWARD_NOT_ENOUGH
	};
}

namespace ERR_JOB_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_PLAYER_NOT_AUTHOR = 4;
	static const char* STR_PLAYER_NOT_AUTHOR = "你无权进行升职与降职";

	static const unsigned int ID_JOIN_PLAYER_NOT_IN_FACTION = 5;
	static const char* STR_JOIN_PLAYER_NOT_IN_FACTION = "该玩家不在门派中";

	static const unsigned int ID_JOIN_PLAYER_JOB_LIMIT = 6;
	static const char* STR_JOIN_PLAYER_JOB_LIMIT = "该职位人数已达上限";

	static const unsigned int ID_JOB_IS_MAX = 7;
	static const char* STR_JOB_IS_MAX = "已升职至最高级别";

	static const unsigned int ID_JOB_IS_MIN = 8;
	static const char* STR_JOB_IS_MIN = "已降职至最低级别";

	static const char* STR_NAMESPACE = "ERR_JOB_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_PLAYER_NOT_AUTHOR
	,STR_JOIN_PLAYER_NOT_IN_FACTION
	,STR_JOIN_PLAYER_JOB_LIMIT
	,STR_JOB_IS_MAX
	,STR_JOB_IS_MIN
	};
}

namespace ERR_SET_NOTICE_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_PLAYER_NOT_AUTHOR = 4;
	static const char* STR_PLAYER_NOT_AUTHOR = "你无权修改门派设置";

	static const unsigned int ID_STRING_NOT_ENOUGH = 5;
	static const char* STR_STRING_NOT_ENOUGH = "您的输入含有敏感词汇，请重新输入";

	static const char* STR_NAMESPACE = "ERR_SET_NOTICE_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_PLAYER_NOT_AUTHOR
	,STR_STRING_NOT_ENOUGH
	};
}

namespace ERR_DISSOLVE_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_PLAYER_NOT_AUTHOR = 4;
	static const char* STR_PLAYER_NOT_AUTHOR = "你无权解散门派";

	static const unsigned int ID_IN_GVG_ACTIVITY = 5;
	static const char* STR_IN_GVG_ACTIVITY = "当前正在参加跨服门派战，无法解散门派";

	static const char* STR_NAMESPACE = "ERR_DISSOLVE_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_PLAYER_NOT_AUTHOR
	,STR_IN_GVG_ACTIVITY
	};
}

namespace ERR_OPEN_AUDIT_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_PLAYER_NOT_AUTHOR = 4;
	static const char* STR_PLAYER_NOT_AUTHOR = "你无权查看审核界面";

	static const char* STR_NAMESPACE = "ERR_OPEN_AUDIT_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_PLAYER_NOT_AUTHOR
	};
}

namespace ERR_AUDIT_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const unsigned int ID_PLAYER_NOT_IN_FACTION = 3;
	static const char* STR_PLAYER_NOT_IN_FACTION = "你不是该门派成员";

	static const unsigned int ID_PLAYER_NOT_AUTHOR = 4;
	static const char* STR_PLAYER_NOT_AUTHOR = "你无权对申请者进行审核";

	static const unsigned int ID_PLAYER_NUM_IS_MAX = 5;
	static const char* STR_PLAYER_NUM_IS_MAX = "你的门派成员人数已达到上限";

	static const unsigned int ID_AUDIT_NUM_IS_MAX = 6;
	static const char* STR_AUDIT_NUM_IS_MAX = "今日审核人数已达上限，请明日再试";

	static const unsigned int ID_ALREADY_AUDIT_OTHER = 7;
	static const char* STR_ALREADY_AUDIT_OTHER = "该玩家已加入其它门派，已为您刷新数据，请重新审核";

	static const unsigned int ID_MULTI_ALREADY_AUDIT_OTHER = 8;
	static const char* STR_MULTI_ALREADY_AUDIT_OTHER = "所选玩家中有人已加入其它门派，已为您刷新数据，请重新审核";

	static const unsigned int ID_ALREAD_CANCEL_APPLY = 9;
	static const char* STR_ALREAD_CANCEL_APPLY = "该玩家已取消申请加入本门派，已为您刷新数据，请重新审核";

	static const unsigned int ID_MULTI_ALREAD_CANCEL_APPLY = 10;
	static const char* STR_MULTI_ALREAD_CANCEL_APPLY = "所选玩家中有人已取消申请加入本门派，已为您刷新数据，请重新审核";

	static const char* STR_NAMESPACE = "ERR_AUDIT_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	,STR_PLAYER_NOT_IN_FACTION
	,STR_PLAYER_NOT_AUTHOR
	,STR_PLAYER_NUM_IS_MAX
	,STR_AUDIT_NUM_IS_MAX
	,STR_ALREADY_AUDIT_OTHER
	,STR_MULTI_ALREADY_AUDIT_OTHER
	,STR_ALREAD_CANCEL_APPLY
	,STR_MULTI_ALREAD_CANCEL_APPLY
	};
}

namespace ERR_OPEN_LOG_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const unsigned int ID_PLAYER_NOT_HAVE_FACTION = 2;
	static const char* STR_PLAYER_NOT_HAVE_FACTION = "你没有加入门派";

	static const char* STR_NAMESPACE = "ERR_OPEN_LOG_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_PLAYER_NOT_HAVE_FACTION
	};
}

namespace ERR_OPEN_APPLY_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const char* STR_NAMESPACE = "ERR_OPEN_APPLY_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_RANK_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "尚未开放门派系统";

	static const char* STR_NAMESPACE = "ERR_OPEN_RANK_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_GATHER_SCIENCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_GATHER_SCIENCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_GATHER_SCIENCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_VIP_NOT_LEVEL = 1;
	static const char* STR_VIP_NOT_LEVEL = "VIP等级不够，请提升VIP等级";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 2;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不够";

	static const unsigned int ID_COIN_NOT_ENOUGH = 3;
	static const char* STR_COIN_NOT_ENOUGH = "铜钱不够";

	static const char* STR_NAMESPACE = "ERR_GATHER_SCIENCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_VIP_NOT_LEVEL
	,STR_GOLD_NOT_ENOUGH
	,STR_COIN_NOT_ENOUGH
	};
}

namespace ERR_BIND_MOBILE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HAVE_BIND_MOBILE = 1;
	static const char* STR_HAVE_BIND_MOBILE = "已绑定过手机号";

	static const unsigned int ID_SEND_FAILD = 2;
	static const char* STR_SEND_FAILD = "发送短信失败";

	static const unsigned int ID_MOBILECHF = 3;
	static const char* STR_MOBILECHF = "非法手机号";

	static const unsigned int ID_MOBILE_HAVE_BIND = 4;
	static const char* STR_MOBILE_HAVE_BIND = "该号码已被绑定";

	static const char* STR_NAMESPACE = "ERR_BIND_MOBILE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HAVE_BIND_MOBILE
	,STR_SEND_FAILD
	,STR_MOBILECHF
	,STR_MOBILE_HAVE_BIND
	};
}

namespace ERR_BIND_CHECK_VER_CODE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_VER_CODE_FAILD = 1;
	static const char* STR_VER_CODE_FAILD = "验证失败";

	static const char* STR_NAMESPACE = "ERR_BIND_CHECK_VER_CODE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_VER_CODE_FAILD
	};
}

namespace ERR_UNBIND_MOBILE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HAVE_UNBIND_MOBILE = 1;
	static const char* STR_HAVE_UNBIND_MOBILE = "从未绑定手机号";

	static const unsigned int ID_SEND_FAILD = 2;
	static const char* STR_SEND_FAILD = "发送短信失败";

	static const char* STR_NAMESPACE = "ERR_UNBIND_MOBILE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HAVE_UNBIND_MOBILE
	,STR_SEND_FAILD
	};
}

namespace ERR_UNBIND_CHECK_VER_CODE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_VER_CODE_FAILD = 1;
	static const char* STR_VER_CODE_FAILD = "验证失败";

	static const char* STR_NAMESPACE = "ERR_UNBIND_CHECK_VER_CODE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_VER_CODE_FAILD
	};
}

namespace ERR_RESET_PASSWD_MOBILE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HAVE_NOT_BIND = 1;
	static const char* STR_HAVE_NOT_BIND = "从未绑定账号";

	static const unsigned int ID_SEND_FAILD = 2;
	static const char* STR_SEND_FAILD = "发送短信失败";

	static const unsigned int ID_MOBILECHF = 3;
	static const char* STR_MOBILECHF = "非法手机号";

	static const char* STR_NAMESPACE = "ERR_RESET_PASSWD_MOBILE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HAVE_NOT_BIND
	,STR_SEND_FAILD
	,STR_MOBILECHF
	};
}

namespace ERR_RESET_PASSWD_CHECK_VER_CODE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_VER_CODE_FAILD = 1;
	static const char* STR_VER_CODE_FAILD = "验证失败";

	static const char* STR_NAMESPACE = "ERR_RESET_PASSWD_CHECK_VER_CODE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_VER_CODE_FAILD
	};
}

namespace ERR_RESET_PASSWD_NEWPASSWD_CODE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_CHARACTER = 1;
	static const char* STR_INVALID_CHARACTER = "无效字符";

	static const unsigned int ID_UNAUTH = 2;
	static const char* STR_UNAUTH = "未验证";

	static const char* STR_NAMESPACE = "ERR_RESET_PASSWD_NEWPASSWD_CODE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_CHARACTER
	,STR_UNAUTH
	};
}

namespace ERR_GET_USER_EXPAND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_GET_USER_EXPAND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_BLUE_PAVILION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_BUILD_NOT_EXIST = 1;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_BLUE_PAVILION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BUILD_NOT_EXIST
	};
}

namespace ERR_BEGBLUE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_CURR_BEGBLUE_USEUP = 1;
	static const char* STR_CURR_BEGBLUE_USEUP = "当前求仙露机会已用完";

	static const unsigned int ID_BUILD_NOT_EXIST = 2;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_BEGBLUE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_CURR_BEGBLUE_USEUP
	,STR_BUILD_NOT_EXIST
	};
}

namespace ERR_IMPROVE_LUCK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_BEGBLUE_RES = 1;
	static const char* STR_NO_BEGBLUE_RES = "您已确认收取，不可使用逆天改运";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 2;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不够";

	static const unsigned int ID_BUILD_NOT_EXIST = 3;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_IMPROVE_LUCK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_BEGBLUE_RES
	,STR_GOLD_NOT_ENOUGH
	,STR_BUILD_NOT_EXIST
	};
}

namespace ERR_COMFIRM_BEGBLUE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_TO_COMFIRM_BEGBLUE = 1;
	static const char* STR_NO_TO_COMFIRM_BEGBLUE = "你已成功收取仙露";

	static const unsigned int ID_BUILD_NOT_EXIST = 2;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const char* STR_NAMESPACE = "ERR_COMFIRM_BEGBLUE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_TO_COMFIRM_BEGBLUE
	,STR_BUILD_NOT_EXIST
	};
}

namespace ERR_GET_BLUE_ENCOURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_SMILE_NUM = 1;
	static const char* STR_NO_SMILE_NUM = "笑脸个数不足";

	static const unsigned int ID_BUILD_NOT_EXIST = 2;
	static const char* STR_BUILD_NOT_EXIST = "建筑不存在";

	static const unsigned int ID_NO_BLUE = 3;
	static const char* STR_NO_BLUE = "今日可领取笑脸奖励已经领完";

	static const char* STR_NAMESPACE = "ERR_GET_BLUE_ENCOURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_SMILE_NUM
	,STR_BUILD_NOT_EXIST
	,STR_NO_BLUE
	};
}

namespace ERR_GET_OPENLIGHT_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_EQUIP_NOT_EXIST = 1;
	static const char* STR_EQUIP_NOT_EXIST = "装备不存在";

	static const unsigned int ID_EQUIP_HAS_OPENLIGHT = 2;
	static const char* STR_EQUIP_HAS_OPENLIGHT = "装备已进阶";

	static const unsigned int ID_EQUIP_CAN_NOT_OPENLIGHT = 3;
	static const char* STR_EQUIP_CAN_NOT_OPENLIGHT = "装备不可进阶";

	static const char* STR_NAMESPACE = "ERR_GET_OPENLIGHT_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_EQUIP_NOT_EXIST
	,STR_EQUIP_HAS_OPENLIGHT
	,STR_EQUIP_CAN_NOT_OPENLIGHT
	};
}

namespace ERR_OPENLIGHT_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_EQUIP_NOT_EXIST = 1;
	static const char* STR_EQUIP_NOT_EXIST = "装备不存在";

	static const unsigned int ID_EQUIP_HAS_OPENLIGHT = 2;
	static const char* STR_EQUIP_HAS_OPENLIGHT = "装备已进阶";

	static const unsigned int ID_EQUIP_CAN_NOT_OPENLIGHT = 3;
	static const char* STR_EQUIP_CAN_NOT_OPENLIGHT = "装备不可进阶";

	static const unsigned int ID_OPENLIGHT_STONE_NOT_ENOUGH = 4;
	static const char* STR_OPENLIGHT_STONE_NOT_ENOUGH = "进阶石不足";

	static const unsigned int ID_OPENLIGHT_RATE_NOT_HIT = 5;
	static const char* STR_OPENLIGHT_RATE_NOT_HIT = "进阶失败";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 6;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const char* STR_NAMESPACE = "ERR_OPENLIGHT_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_EQUIP_NOT_EXIST
	,STR_EQUIP_HAS_OPENLIGHT
	,STR_EQUIP_CAN_NOT_OPENLIGHT
	,STR_OPENLIGHT_STONE_NOT_ENOUGH
	,STR_OPENLIGHT_RATE_NOT_HIT
	,STR_GOLD_NOT_ENOUGH
	};
}

namespace ERR_CHANGE_CAREER_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_CAREERID_NOT_EXIST = 1;
	static const char* STR_CAREERID_NOT_EXIST = "职业不存在";

	static const char* STR_NAMESPACE = "ERR_CHANGE_CAREER_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_CAREERID_NOT_EXIST
	};
}

namespace ERR_CHALLENGE_MYSELF_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_CAREERID_NOT_EXIST = 1;
	static const char* STR_CAREERID_NOT_EXIST = "职业不存在";

	static const char* STR_NAMESPACE = "ERR_CHALLENGE_MYSELF_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_CAREERID_NOT_EXIST
	};
}

namespace ERR_GET_FRIEND_SYS_PLAYER_LIST 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_PAGE_CAPACITY = 1;
	static const char* STR_INVALID_PAGE_CAPACITY = "无效的分页容量";

	static const unsigned int ID_NO_LIST_DATA = 2;
	static const char* STR_NO_LIST_DATA = "无相关数据";

	static const unsigned int ID_NO_OPEN = 3;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_GET_FRIEND_SYS_PLAYER_LIST";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_PAGE_CAPACITY
	,STR_NO_LIST_DATA
	,STR_NO_OPEN
	};
}

namespace ERR_DEL_FRIEND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SELF_INFO_LOST = 1;
	static const char* STR_SELF_INFO_LOST = "自身信息丢失";

	static const unsigned int ID_FRIEND_INFO_LOST = 2;
	static const char* STR_FRIEND_INFO_LOST = "对方信息丢失";

	static const unsigned int ID_REF_PLAYER_NOT_YOUR_FRIEND = 3;
	static const char* STR_REF_PLAYER_NOT_YOUR_FRIEND = "对方已不再是你好友";

	static const unsigned int ID_NO_OPEN = 4;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_DEL_FRIEND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SELF_INFO_LOST
	,STR_FRIEND_INFO_LOST
	,STR_REF_PLAYER_NOT_YOUR_FRIEND
	,STR_NO_OPEN
	};
}

namespace ERR_SEARCH_PLAYER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_REF_DATA = 1;
	static const char* STR_NO_REF_DATA = "找不到相关玩家";

	static const unsigned int ID_INVALID_PAGE_CAPACITY = 2;
	static const char* STR_INVALID_PAGE_CAPACITY = "无效的分页容量";

	static const unsigned int ID_NO_OPEN = 3;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_SEARCH_PLAYER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_REF_DATA
	,STR_INVALID_PAGE_CAPACITY
	,STR_NO_OPEN
	};
}

namespace ERR_VIEW_PLAYER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_REF_DATA = 1;
	static const char* STR_NO_REF_DATA = "无相关数据";

	static const unsigned int ID_NO_OPEN = 2;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_VIEW_PLAYER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_REF_DATA
	,STR_NO_OPEN
	};
}

namespace ERR_ADD_FRIEND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SELF_INFO_LOST = 1;
	static const char* STR_SELF_INFO_LOST = "自身信息丢失";

	static const unsigned int ID_FRIEND_NUM_TO_UPPERLIMIT = 2;
	static const char* STR_FRIEND_NUM_TO_UPPERLIMIT = "你的好友已达上限200人";

	static const unsigned int ID_REF_PLAYER_INFO_LOST = 3;
	static const char* STR_REF_PLAYER_INFO_LOST = "对方信息丢失";

	static const unsigned int ID_YOU_ARE_IN_REF_PLAYER_BLACKLIST = 4;
	static const char* STR_YOU_ARE_IN_REF_PLAYER_BLACKLIST = "对方已把你拉入黑名单";

	static const unsigned int ID_REF_PLAYER_IN_YOUR_BLACKLIST = 5;
	static const char* STR_REF_PLAYER_IN_YOUR_BLACKLIST = "你已把对方拉入黑名单";

	static const unsigned int ID_REF_PLAYER_HAS_BEEN_YOUR_FRIEND = 6;
	static const char* STR_REF_PLAYER_HAS_BEEN_YOUR_FRIEND = "对方已是你的好友";

	static const unsigned int ID_REF_MYSELF = 7;
	static const char* STR_REF_MYSELF = "不能加自己为好友";

	static const unsigned int ID_NO_OPEN = 8;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const unsigned int ID_TARGET_NO_OPEN = 9;
	static const char* STR_TARGET_NO_OPEN = "对方好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_ADD_FRIEND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SELF_INFO_LOST
	,STR_FRIEND_NUM_TO_UPPERLIMIT
	,STR_REF_PLAYER_INFO_LOST
	,STR_YOU_ARE_IN_REF_PLAYER_BLACKLIST
	,STR_REF_PLAYER_IN_YOUR_BLACKLIST
	,STR_REF_PLAYER_HAS_BEEN_YOUR_FRIEND
	,STR_REF_MYSELF
	,STR_NO_OPEN
	,STR_TARGET_NO_OPEN
	};
}

namespace ERR_OPEN_SEND_FLOWER_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_SEND_FLOWER_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	};
}

namespace ERR_SEND_FLOWER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_SEND_FLOWER_NUM = 1;
	static const char* STR_INVALID_SEND_FLOWER_NUM = "无效的送花规格";

	static const unsigned int ID_SEND_CHANCE_USEUP = 2;
	static const char* STR_SEND_CHANCE_USEUP = "送花次数已用完";

	static const unsigned int ID_NO_OPEN = 3;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const unsigned int ID_OTHER_NOT_FRIEND = 4;
	static const char* STR_OTHER_NOT_FRIEND = "该玩家还不是你好友，无法赠送";

	static const unsigned int ID_BAG_NOT_EXIT = 5;
	static const char* STR_BAG_NOT_EXIT = "背包物品不存在";

	static const unsigned int ID_ITEM_TYPE_ERR = 6;
	static const char* STR_ITEM_TYPE_ERR = "类型不支持";

	static const unsigned int ID_BAG_FULL = 7;
	static const char* STR_BAG_FULL = "背包满了";

	static const unsigned int ID_OTHER_NOT_OPEN_FRIENDS = 8;
	static const char* STR_OTHER_NOT_OPEN_FRIENDS = "对方未开放好友功能，无法赠送";

	static const unsigned int ID_OTHER_RECV_FLOWER_FULL = 9;
	static const char* STR_OTHER_RECV_FLOWER_FULL = "对方收花信箱已满，无法赠送，请稍后再送。";

	static const unsigned int ID_YOU_NOT_OTHER_FRIEND = 10;
	static const char* STR_YOU_NOT_OTHER_FRIEND = "你不是对方好友，无法赠送";

	static const char* STR_NAMESPACE = "ERR_SEND_FLOWER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_SEND_FLOWER_NUM
	,STR_SEND_CHANCE_USEUP
	,STR_NO_OPEN
	,STR_OTHER_NOT_FRIEND
	,STR_BAG_NOT_EXIT
	,STR_ITEM_TYPE_ERR
	,STR_BAG_FULL
	,STR_OTHER_NOT_OPEN_FRIENDS
	,STR_OTHER_RECV_FLOWER_FULL
	,STR_YOU_NOT_OTHER_FRIEND
	};
}

namespace ERR_OPEN_SEND_FLOWER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_SEND_FLOWER_NUM = 1;
	static const char* STR_INVALID_SEND_FLOWER_NUM = "无效的送花规格";

	static const unsigned int ID_SEND_CHANCE_USEUP = 2;
	static const char* STR_SEND_CHANCE_USEUP = "送花次数已用完";

	static const unsigned int ID_NO_OPEN = 3;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const unsigned int ID_OTHER_NOT_FRIEND = 4;
	static const char* STR_OTHER_NOT_FRIEND = "该玩家还不是你好友，无法赠送";

	static const char* STR_NAMESPACE = "ERR_OPEN_SEND_FLOWER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_SEND_FLOWER_NUM
	,STR_SEND_CHANCE_USEUP
	,STR_NO_OPEN
	,STR_OTHER_NOT_FRIEND
	};
}

namespace ERR_GET_FLOWER_BY_PLAYERID 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_GET_FLOWER_BY_PLAYERID";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	};
}

namespace ERR_GET_FLOWER_RANKING 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_GET_FLOWER_RANKING";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	};
}

namespace ERR_GET_PRIVATE_MSG_LOG_LIST 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_INVALID_REQ_PARAM = 1;
	static const char* STR_INVALID_REQ_PARAM = "非法的请求参数";

	static const unsigned int ID_NO_REF_DATA = 2;
	static const char* STR_NO_REF_DATA = "无相关内容";

	static const unsigned int ID_NO_OPEN = 3;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_GET_PRIVATE_MSG_LOG_LIST";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_INVALID_REQ_PARAM
	,STR_NO_REF_DATA
	,STR_NO_OPEN
	};
}

namespace ERR_SEND_PRIVATE_MSG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_YOU_ARE_IN_REF_PLAYER_BLACKLIST = 1;
	static const char* STR_YOU_ARE_IN_REF_PLAYER_BLACKLIST = "发送失败，对方已把你拉入黑名单";

	static const unsigned int ID_SELF_INFO_LOST = 2;
	static const char* STR_SELF_INFO_LOST = "自身信息丢失";

	static const unsigned int ID_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST = 3;
	static const char* STR_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST = "发送失败，你已把对方拉入黑名单";

	static const unsigned int ID_NO_OPEN = 4;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const unsigned int ID_VERIFY_ERROR = 5;
	static const char* STR_VERIFY_ERROR = "含有非法屏蔽词";

	static const char* STR_NAMESPACE = "ERR_SEND_PRIVATE_MSG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_YOU_ARE_IN_REF_PLAYER_BLACKLIST
	,STR_SELF_INFO_LOST
	,STR_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST
	,STR_NO_OPEN
	,STR_VERIFY_ERROR
	};
}

namespace ERR_DEL_PRIVATE_MSG_LOG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_DEL_PRIVATE_MSG_LOG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	};
}

namespace ERR_ADD_TO_BLACKLIST 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SELF_INFO_LOST = 1;
	static const char* STR_SELF_INFO_LOST = "自身信息丢失";

	static const unsigned int ID_BLACKLIST_NUM_TO_UPPERLIMIT = 2;
	static const char* STR_BLACKLIST_NUM_TO_UPPERLIMIT = "你的好友已达上限200人";

	static const unsigned int ID_REF_PLAYER_INFO_LOST = 3;
	static const char* STR_REF_PLAYER_INFO_LOST = "对方信息丢失";

	static const unsigned int ID_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST = 4;
	static const char* STR_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST = "对方已在你的黑名单";

	static const unsigned int ID_NO_OPEN = 5;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const unsigned int ID_NOT_JOIN_BLACK_SELF = 6;
	static const char* STR_NOT_JOIN_BLACK_SELF = "无法将自己拉入黑名单";

	static const char* STR_NAMESPACE = "ERR_ADD_TO_BLACKLIST";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SELF_INFO_LOST
	,STR_BLACKLIST_NUM_TO_UPPERLIMIT
	,STR_REF_PLAYER_INFO_LOST
	,STR_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST
	,STR_NO_OPEN
	,STR_NOT_JOIN_BLACK_SELF
	};
}

namespace ERR_MOVE_FROM_BLACKLIST 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SELF_INFO_LOST = 1;
	static const char* STR_SELF_INFO_LOST = "自身信息丢失";

	static const unsigned int ID_REF_PLAYER_INFO_LOST = 2;
	static const char* STR_REF_PLAYER_INFO_LOST = "对方信息丢失";

	static const unsigned int ID_REF_PLAYER_NOT_YOUR_FRIEND = 3;
	static const char* STR_REF_PLAYER_NOT_YOUR_FRIEND = "对方已不在你黑名单里";

	static const unsigned int ID_NO_OPEN = 4;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_MOVE_FROM_BLACKLIST";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SELF_INFO_LOST
	,STR_REF_PLAYER_INFO_LOST
	,STR_REF_PLAYER_NOT_YOUR_FRIEND
	,STR_NO_OPEN
	};
}

namespace ERR_GET_SEND_OFFLINE_MSG_PLAYER_LIST 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_GET_SEND_OFFLINE_MSG_PLAYER_LIST";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	};
}

namespace ERR_READ_OFFLINE_MSG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_READ_OFFLINE_MSG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	};
}

namespace ERR_OPEN_RAREITEMROOM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RAREITEM_ROOM_NOT_EXIST = 1;
	static const char* STR_RAREITEM_ROOM_NOT_EXIST = "珍品坊还未开放";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "你没有加入门派";

	static const unsigned int ID_IS_UNOPEN = 3;
	static const char* STR_IS_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_RAREITEMROOM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RAREITEM_ROOM_NOT_EXIST
	,STR_FACTION_NOT_EXIST
	,STR_IS_UNOPEN
	};
}

namespace ERR_BUY_RAREITEM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_RAREITEM_ROOM_NOT_EXIST = 1;
	static const char* STR_RAREITEM_ROOM_NOT_EXIST = "珍品坊还未开放";

	static const unsigned int ID_DOORSTRIBUTE_NOT_ENOUGH = 2;
	static const char* STR_DOORSTRIBUTE_NOT_ENOUGH = "剩余门贡不足";

	static const unsigned int ID_SUMDOORSTRIBUTE_NOT_ENOUGH = 3;
	static const char* STR_SUMDOORSTRIBUTE_NOT_ENOUGH = "累计门贡不足";

	static const unsigned int ID_EXCHANGE_TIMES_NOT_ENOUGH = 4;
	static const char* STR_EXCHANGE_TIMES_NOT_ENOUGH = "您今日该物品购买次已用完, 请明日再尝试";

	static const unsigned int ID_KINDID_INVALID = 5;
	static const char* STR_KINDID_INVALID = "无效的物品ID";

	static const unsigned int ID_BAG_FULL = 6;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_NOT_JOIN_FACTION = 7;
	static const char* STR_NOT_JOIN_FACTION = "你没有加入门派";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 8;
	static const char* STR_LEVEL_NOT_ENOUGH = "你还未达到该物品的购买等级";

	static const unsigned int ID_UNOPEN = 9;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_BUY_RAREITEM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_RAREITEM_ROOM_NOT_EXIST
	,STR_DOORSTRIBUTE_NOT_ENOUGH
	,STR_SUMDOORSTRIBUTE_NOT_ENOUGH
	,STR_EXCHANGE_TIMES_NOT_ENOUGH
	,STR_KINDID_INVALID
	,STR_BAG_FULL
	,STR_NOT_JOIN_FACTION
	,STR_LEVEL_NOT_ENOUGH
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_UPGRADERAREITEM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 1;
	static const char* STR_LIMIT_NOT_ENOUGH = "你无权升级珍品坊";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_RAREITEM_LEVEL_IS_MAX = 3;
	static const char* STR_RAREITEM_LEVEL_IS_MAX = "珍品坊等级已达上限";

	static const unsigned int ID_RAREITEM_NOT_EXIST = 4;
	static const char* STR_RAREITEM_NOT_EXIST = "珍品坊还未开放";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_UPGRADERAREITEM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_LIMIT_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_RAREITEM_LEVEL_IS_MAX
	,STR_RAREITEM_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_UPGRADE_RAREITEM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 1;
	static const char* STR_LIMIT_NOT_ENOUGH = "你无权升级珍品坊";

	static const unsigned int ID_UNITED_GRADE_NOT_ENOUGH = 2;
	static const char* STR_UNITED_GRADE_NOT_ENOUGH = "门派等级不足";

	static const unsigned int ID_UNITED_FUNDS_NOT_ENOUGH = 3;
	static const char* STR_UNITED_FUNDS_NOT_ENOUGH = "门派资金不足";

	static const unsigned int ID_FACTION_NOT_EXIST = 4;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_RAREITEM_NOT_EXIST = 5;
	static const char* STR_RAREITEM_NOT_EXIST = "珍品坊还未开放";

	static const unsigned int ID_FACTION_LEVEL_IS_MAX = 6;
	static const char* STR_FACTION_LEVEL_IS_MAX = "珍品坊等级已达上限";

	static const unsigned int ID_UNOPEN = 7;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_RAREITEM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_LIMIT_NOT_ENOUGH
	,STR_UNITED_GRADE_NOT_ENOUGH
	,STR_UNITED_FUNDS_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_RAREITEM_NOT_EXIST
	,STR_FACTION_LEVEL_IS_MAX
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_GYMNASIUM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GYMNASIUM_NOT_EXIST = 1;
	static const char* STR_GYMNASIUM_NOT_EXIST = "武堂还未开放";

	static const unsigned int ID_FACTION_NOT_EXITS = 2;
	static const char* STR_FACTION_NOT_EXITS = "该门派不存在";

	static const unsigned int ID_UNOPEN = 3;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_GYMNASIUM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GYMNASIUM_NOT_EXIST
	,STR_FACTION_NOT_EXITS
	,STR_UNOPEN
	};
}

namespace ERR_LEARN_GYMNASIUM_SKILL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_DOORSTRIBUTE_NOT_ENOUGH = 1;
	static const char* STR_DOORSTRIBUTE_NOT_ENOUGH = "门贡不足";

	static const unsigned int ID_GYMNASIUM_LEVEL_NOT_ENOUGH = 2;
	static const char* STR_GYMNASIUM_LEVEL_NOT_ENOUGH = "请先升级武堂";

	static const unsigned int ID_FACTION_NOT_EXIST = 3;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_IS_FULL_LEVEL = 4;
	static const char* STR_IS_FULL_LEVEL = "技能已满级";

	static const unsigned int ID_SKILL_NOT_EXIST = 5;
	static const char* STR_SKILL_NOT_EXIST = "技能不存在";

	static const unsigned int ID_GYMNASIUM_NOT_EXIST = 6;
	static const char* STR_GYMNASIUM_NOT_EXIST = "武堂还未开放";

	static const unsigned int ID_SKILL_NOT_UNLOCK = 7;
	static const char* STR_SKILL_NOT_UNLOCK = "技能未解锁";

	static const unsigned int ID_PLAYERID_NOT_FIND = 8;
	static const char* STR_PLAYERID_NOT_FIND = "找不到玩家数据";

	static const unsigned int ID_UNOPEN = 9;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const unsigned int ID_SUM_DOORSTRINUTE_NOT_ENOUGH = 10;
	static const char* STR_SUM_DOORSTRINUTE_NOT_ENOUGH = "你的累计门贡未达到要求，无法学习该技能";

	static const char* STR_NAMESPACE = "ERR_LEARN_GYMNASIUM_SKILL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_DOORSTRIBUTE_NOT_ENOUGH
	,STR_GYMNASIUM_LEVEL_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_IS_FULL_LEVEL
	,STR_SKILL_NOT_EXIST
	,STR_GYMNASIUM_NOT_EXIST
	,STR_SKILL_NOT_UNLOCK
	,STR_PLAYERID_NOT_FIND
	,STR_UNOPEN
	,STR_SUM_DOORSTRINUTE_NOT_ENOUGH
	};
}

namespace ERR_OPEN_GYMNASIUM_UPGRADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GYMNASIUM_LEVEL_IS_FULL = 1;
	static const char* STR_GYMNASIUM_LEVEL_IS_FULL = "武堂等级已达上限";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 3;
	static const char* STR_LIMIT_NOT_ENOUGH = "你无权升级武堂";

	static const unsigned int ID_GYMNASIUM_NOT_EXIST = 4;
	static const char* STR_GYMNASIUM_NOT_EXIST = "武堂还未开放";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_GYMNASIUM_UPGRADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GYMNASIUM_LEVEL_IS_FULL
	,STR_FACTION_NOT_EXIST
	,STR_LIMIT_NOT_ENOUGH
	,STR_GYMNASIUM_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_GYMNASIUM_UPGRADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_LEVEL_NOT_ENOUGH = 1;
	static const char* STR_FACTION_LEVEL_NOT_ENOUGH = "门派等级不够";

	static const unsigned int ID_FACTION_FUNDS_NOT_ENOUGH = 2;
	static const char* STR_FACTION_FUNDS_NOT_ENOUGH = "门派资金不足";

	static const unsigned int ID_FACTION_NOT_EXIST = 3;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 4;
	static const char* STR_LIMIT_NOT_ENOUGH = "你无权升级武堂";

	static const unsigned int ID_GYMNASIUM_LEVEL_IS_FULL = 5;
	static const char* STR_GYMNASIUM_LEVEL_IS_FULL = "武堂等级已达上限";

	static const unsigned int ID_GYMNASIUM_NOT_EXIST = 6;
	static const char* STR_GYMNASIUM_NOT_EXIST = "武堂还未开放";

	static const unsigned int ID_UNOPEN = 7;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_GYMNASIUM_UPGRADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_LEVEL_NOT_ENOUGH
	,STR_FACTION_FUNDS_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_LIMIT_NOT_ENOUGH
	,STR_GYMNASIUM_LEVEL_IS_FULL
	,STR_GYMNASIUM_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_SHADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_SHADE_NOT_EXIST = 2;
	static const char* STR_SHADE_NOT_EXIST = "暗部未开放";

	static const unsigned int ID_UNOPEN = 3;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_SHADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_SHADE_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_SHADE_UPGRADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 1;
	static const char* STR_LIMIT_NOT_ENOUGH = "你无权升级暗部";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_SHADE_LEVEL_IS_FULL = 3;
	static const char* STR_SHADE_LEVEL_IS_FULL = "暗部等级已达上限";

	static const unsigned int ID_SHADE_NOT_EXIST = 4;
	static const char* STR_SHADE_NOT_EXIST = "暗部还未开放";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_SHADE_UPGRADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_LIMIT_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_SHADE_LEVEL_IS_FULL
	,STR_SHADE_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_SHADE_UPGRADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_LEVEL_NOT_ENOUGH = 1;
	static const char* STR_FACTION_LEVEL_NOT_ENOUGH = "门派等级不够";

	static const unsigned int ID_FACTION_FUNDS_NOT_ENOUGH = 2;
	static const char* STR_FACTION_FUNDS_NOT_ENOUGH = "门派资金不足";

	static const unsigned int ID_FACTION_NOT_EXIST = 3;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 4;
	static const char* STR_LIMIT_NOT_ENOUGH = "你无权升级暗部";

	static const unsigned int ID_SHADE_LEVEL_IS_FULL = 5;
	static const char* STR_SHADE_LEVEL_IS_FULL = "暗部等级已达上限";

	static const unsigned int ID_ACTIVI_NOT_UPGRADE = 6;
	static const char* STR_ACTIVI_NOT_UPGRADE = "捉妖活动期间，暗部不能升级";

	static const unsigned int ID_SHADE_NOT_OPEN = 7;
	static const char* STR_SHADE_NOT_OPEN = "暗部还未解锁";

	static const unsigned int ID_UNOPEN = 8;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_SHADE_UPGRADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_LEVEL_NOT_ENOUGH
	,STR_FACTION_FUNDS_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_LIMIT_NOT_ENOUGH
	,STR_SHADE_LEVEL_IS_FULL
	,STR_ACTIVI_NOT_UPGRADE
	,STR_SHADE_NOT_OPEN
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_GOLDMODE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 1;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_STATE_IS_OPEN = 2;
	static const char* STR_STATE_IS_OPEN = "福神显灵已开启";

	static const unsigned int ID_FACTION_NOT_EXIST = 3;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_VIP_LEVEL_NOT_ENOUGH = 4;
	static const char* STR_VIP_LEVEL_NOT_ENOUGH = "VIP等级不足";

	static const unsigned int ID_TIME_NOT_ENOUGH = 5;
	static const char* STR_TIME_NOT_ENOUGH = "当前非捉妖任务时段";

	static const unsigned int ID_SHADE_NOT_EXIST = 6;
	static const char* STR_SHADE_NOT_EXIST = "暗部还未开放";

	static const unsigned int ID_UNOPEN = 7;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const unsigned int ID_HUNTING_NUM_NOT_ENOUGH = 8;
	static const char* STR_HUNTING_NUM_NOT_ENOUGH = "捉妖次数不足，无法使用福神显灵";

	static const char* STR_NAMESPACE = "ERR_OPEN_GOLDMODE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GOLD_NOT_ENOUGH
	,STR_STATE_IS_OPEN
	,STR_FACTION_NOT_EXIST
	,STR_VIP_LEVEL_NOT_ENOUGH
	,STR_TIME_NOT_ENOUGH
	,STR_SHADE_NOT_EXIST
	,STR_UNOPEN
	,STR_HUNTING_NUM_NOT_ENOUGH
	};
}

namespace ERR_OPEN_HUNTING 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_NOT_OPEN_HUNTING = 2;
	static const char* STR_NOT_OPEN_HUNTING = "今日已没有捉妖次数";

	static const unsigned int ID_TIME_NOT_ENOUGH = 3;
	static const char* STR_TIME_NOT_ENOUGH = "当前非捉妖任务时段";

	static const unsigned int ID_SHADE_NOT_EXIST = 4;
	static const char* STR_SHADE_NOT_EXIST = "暗部还未开放";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_HUNTING";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_NOT_OPEN_HUNTING
	,STR_TIME_NOT_ENOUGH
	,STR_SHADE_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_FIND_MONSTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 2;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_VIP_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_VIP_LEVEL_NOT_ENOUGH = "VIP等级不足";

	static const unsigned int ID_MONSTER_IS_EXIST = 4;
	static const char* STR_MONSTER_IS_EXIST = "当前已有妖怪";

	static const unsigned int ID_NOT_FIND_MONSTER_NUM = 5;
	static const char* STR_NOT_FIND_MONSTER_NUM = "没有捉妖次数";

	static const unsigned int ID_IS_MAX_MONSTER = 6;
	static const char* STR_IS_MAX_MONSTER = "当前已是最高品质妖怪";

	static const unsigned int ID_SHADE_NOT_EXIST = 7;
	static const char* STR_SHADE_NOT_EXIST = "暗部还未开放";

	static const unsigned int ID_TIME_NOT_ENOUGH = 8;
	static const char* STR_TIME_NOT_ENOUGH = "当前非捉妖任务时段";

	static const unsigned int ID_UNOPEN = 9;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const unsigned int ID_HUNTING_NUM_NOT_ENOUGH_FIND = 10;
	static const char* STR_HUNTING_NUM_NOT_ENOUGH_FIND = "捉妖次数不足，无法使用千里眼";

	static const unsigned int ID_HUNTING_NUM_NOT_ENOUGH_CHANGE = 11;
	static const char* STR_HUNTING_NUM_NOT_ENOUGH_CHANGE = "捉妖次数不足，无法使用换一只";

	static const char* STR_NAMESPACE = "ERR_FIND_MONSTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_GOLD_NOT_ENOUGH
	,STR_VIP_LEVEL_NOT_ENOUGH
	,STR_MONSTER_IS_EXIST
	,STR_NOT_FIND_MONSTER_NUM
	,STR_IS_MAX_MONSTER
	,STR_SHADE_NOT_EXIST
	,STR_TIME_NOT_ENOUGH
	,STR_UNOPEN
	,STR_HUNTING_NUM_NOT_ENOUGH_FIND
	,STR_HUNTING_NUM_NOT_ENOUGH_CHANGE
	};
}

namespace ERR_HUNTING_BATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_MONSTER_NOT_EXIST = 1;
	static const char* STR_MONSTER_NOT_EXIST = "该妖怪不存在";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_SHADE_NOT_EXIST = 3;
	static const char* STR_SHADE_NOT_EXIST = "暗部还未开放";

	static const unsigned int ID_TIME_NOT_ENOUGH = 4;
	static const char* STR_TIME_NOT_ENOUGH = "当前非捉妖任务时段";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_HUNTING_BATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_MONSTER_NOT_EXIST
	,STR_FACTION_NOT_EXIST
	,STR_SHADE_NOT_EXIST
	,STR_TIME_NOT_ENOUGH
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_HOT_SPRING 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 2;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_PLAYER_ID_NOT_FIND = 3;
	static const char* STR_PLAYER_ID_NOT_FIND = "玩家数据异常";

	static const unsigned int ID_UNOPEN = 4;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_HOT_SPRING";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_HOT_SPRING_NOT_EXIST
	,STR_PLAYER_ID_NOT_FIND
	,STR_UNOPEN
	};
}

namespace ERR_SELECT_WINE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_WINE_IS_DRINKING = 1;
	static const char* STR_WINE_IS_DRINKING = "你正在享受服务中";

	static const unsigned int ID_COIN_NOT_ENOUGH = 2;
	static const char* STR_COIN_NOT_ENOUGH = "铜钱不足";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 3;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_VIP_LEVEL_NOT_ENOUGH = 4;
	static const char* STR_VIP_LEVEL_NOT_ENOUGH = "VIP等级不够";

	static const unsigned int ID_WINE_NOT_EXIST = 5;
	static const char* STR_WINE_NOT_EXIST = "服务不存在";

	static const unsigned int ID_FACTION_NOT_EXIST = 6;
	static const char* STR_FACTION_NOT_EXIST = "未加入门派";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 7;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_PLAYER_ID_NOT_FIND = 8;
	static const char* STR_PLAYER_ID_NOT_FIND = "玩家数据异常";

	static const unsigned int ID_UNOPEN = 9;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const unsigned int ID_ALREAD_DRINK = 10;
	static const char* STR_ALREAD_DRINK = "你今日已使用过温泉服务，欢迎明日再来";

	static const char* STR_NAMESPACE = "ERR_SELECT_WINE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_WINE_IS_DRINKING
	,STR_COIN_NOT_ENOUGH
	,STR_GOLD_NOT_ENOUGH
	,STR_VIP_LEVEL_NOT_ENOUGH
	,STR_WINE_NOT_EXIST
	,STR_FACTION_NOT_EXIST
	,STR_HOT_SPRING_NOT_EXIST
	,STR_PLAYER_ID_NOT_FIND
	,STR_UNOPEN
	,STR_ALREAD_DRINK
	};
}

namespace ERR_DRINK_WINE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_WINE_NOT_EXIST = 1;
	static const char* STR_WINE_NOT_EXIST = "服务不存在";

	static const unsigned int ID_IS_EMPTY = 2;
	static const char* STR_IS_EMPTY = "服务已结束";

	static const unsigned int ID_TIME_NOT_ENOUGH = 3;
	static const char* STR_TIME_NOT_ENOUGH = "你还在享受服务";

	static const unsigned int ID_WINE_IS_BAD = 4;
	static const char* STR_WINE_IS_BAD = "所选的服务已超出时限";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_DRINK_WINE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_WINE_NOT_EXIST
	,STR_IS_EMPTY
	,STR_TIME_NOT_ENOUGH
	,STR_WINE_IS_BAD
	,STR_UNOPEN
	};
}

namespace ERR_DRINK_WINE_MOVE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 1;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_UNOPEN = 2;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_DRINK_WINE_MOVE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HOT_SPRING_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_CHANGE_OTHER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_MARTIAL_HEAD = 1;
	static const char* STR_NOT_MARTIAL_HEAD = "你不是掌门";

	static const unsigned int ID_POSITION_NOT_ENOUGH = 2;
	static const char* STR_POSITION_NOT_ENOUGH = "只能将掌门之位传给大长老或长老";

	static const unsigned int ID_FACTION_NOT_EXIST = 3;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_PEOPLE_NOT_EXIST = 4;
	static const char* STR_PEOPLE_NOT_EXIST = "玩家不存在";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_CHANGE_OTHER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_MARTIAL_HEAD
	,STR_POSITION_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_PEOPLE_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_GIVE_WINE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 1;
	static const char* STR_LIMIT_NOT_ENOUGH = "只有长老以上职位才能开启休闲时光";

	static const unsigned int ID_NOT_ACTIVI_TIME = 2;
	static const char* STR_NOT_ACTIVI_TIME = "今日开启次数已用完，请明日再试";

	static const unsigned int ID_GIVE_WINE_IS_OPEN = 3;
	static const char* STR_GIVE_WINE_IS_OPEN = "休闲时光已开启";

	static const unsigned int ID_FACTION_NOT_EXIST = 4;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_TIME_NOT_ENOUGH = 5;
	static const char* STR_TIME_NOT_ENOUGH = "享受额外服务中";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 6;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_UNOPEN = 7;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_GIVE_WINE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_LIMIT_NOT_ENOUGH
	,STR_NOT_ACTIVI_TIME
	,STR_GIVE_WINE_IS_OPEN
	,STR_FACTION_NOT_EXIST
	,STR_TIME_NOT_ENOUGH
	,STR_HOT_SPRING_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_CLICK_SELECT_WINE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_WINE_IS_DRINKING = 1;
	static const char* STR_WINE_IS_DRINKING = "你正在享受服务中";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "还未加入门派";

	static const unsigned int ID_NOW_CAN_NOT_SELECT = 3;
	static const char* STR_NOW_CAN_NOT_SELECT = "休闲时光期间无法找回上次服务机会";

	static const unsigned int ID_NOT_FIND_DRINK = 4;
	static const char* STR_NOT_FIND_DRINK = "不符合找回上次服务机会的条件";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 5;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_UNOPEN = 6;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_CLICK_SELECT_WINE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_WINE_IS_DRINKING
	,STR_FACTION_NOT_EXIST
	,STR_NOW_CAN_NOT_SELECT
	,STR_NOT_FIND_DRINK
	,STR_HOT_SPRING_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_SURE_GIVE_WINE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ACTIVITY_TIME_NOT_ENOUGH = 1;
	static const char* STR_ACTIVITY_TIME_NOT_ENOUGH = "不在活动时间内";

	static const unsigned int ID_GIVE_WINE_NOT_TIMES = 2;
	static const char* STR_GIVE_WINE_NOT_TIMES = "今日休闲时光的次数已用完";

	static const unsigned int ID_FACTION_FUNDS_NOT_ENOUGH = 3;
	static const char* STR_FACTION_FUNDS_NOT_ENOUGH = "门派资金不足";

	static const unsigned int ID_FACTION_NOT_EXIST = 4;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_GIVE_WINE_IS_OPEN = 5;
	static const char* STR_GIVE_WINE_IS_OPEN = "休闲时光已开启";

	static const unsigned int ID_CD_TIME_NOT_ENOUGH = 6;
	static const char* STR_CD_TIME_NOT_ENOUGH = "享受额外服务中";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 7;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_UNOPEN = 8;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_SURE_GIVE_WINE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ACTIVITY_TIME_NOT_ENOUGH
	,STR_GIVE_WINE_NOT_TIMES
	,STR_FACTION_FUNDS_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_GIVE_WINE_IS_OPEN
	,STR_CD_TIME_NOT_ENOUGH
	,STR_HOT_SPRING_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_CLOSED_MONSTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 1;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_MONSTER_NOT_EXIST = 2;
	static const char* STR_MONSTER_NOT_EXIST = "该妖怪不存在";

	static const unsigned int ID_FACTION_NOT_EXIST = 3;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_SHADE_NOT_EXIST = 4;
	static const char* STR_SHADE_NOT_EXIST = "暗部还未开放";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_CLOSED_MONSTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_GOLD_NOT_ENOUGH
	,STR_MONSTER_NOT_EXIST
	,STR_FACTION_NOT_EXIST
	,STR_SHADE_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_LEAVE_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_HEAD_NOT_LEAVE = 2;
	static const char* STR_HEAD_NOT_LEAVE = "本门成员数量大于1人，无法解散门派";

	static const unsigned int ID_UNOPEN = 3;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const unsigned int ID_IN_GVG_ACTIVITY = 4;
	static const char* STR_IN_GVG_ACTIVITY = "当前正在参加跨服门派战，无法解散门派";

	static const unsigned int ID_IN_GVG_ACTIVITY_CAN_NOT_LEAVE = 5;
	static const char* STR_IN_GVG_ACTIVITY_CAN_NOT_LEAVE = "本门派当前正在参加跨服门派战，无法离开门派";

	static const char* STR_NAMESPACE = "ERR_LEAVE_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_HEAD_NOT_LEAVE
	,STR_UNOPEN
	,STR_IN_GVG_ACTIVITY
	,STR_IN_GVG_ACTIVITY_CAN_NOT_LEAVE
	};
}

namespace ERR_OPEN_LEAVE_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_HEAD_NOT_LEAVE = 2;
	static const char* STR_HEAD_NOT_LEAVE = "本门成员数量大于1人，掌门无法退出门派";

	static const unsigned int ID_UNOPEN = 3;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_LEAVE_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_HEAD_NOT_LEAVE
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_FACTION_UPGRADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_FULL_LEVEL = 1;
	static const char* STR_IS_FULL_LEVEL = "门派已满级";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_UPGRADE_LIMIT_NOT_ENOUGH = 3;
	static const char* STR_UPGRADE_LIMIT_NOT_ENOUGH = "只有掌门才能升级门派";

	static const unsigned int ID_UNOPEN = 4;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_FACTION_UPGRADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_FULL_LEVEL
	,STR_FACTION_NOT_EXIST
	,STR_UPGRADE_LIMIT_NOT_ENOUGH
	,STR_UNOPEN
	};
}

namespace ERR_FACTION_UPGRADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_UPGRADE_LIMIT_NOT_ENOUGH = 2;
	static const char* STR_UPGRADE_LIMIT_NOT_ENOUGH = "只有掌门才能升级门派";

	static const unsigned int ID_FACTION_FUNDS_NOT_ENOUGH = 3;
	static const char* STR_FACTION_FUNDS_NOT_ENOUGH = "门派资金不够";

	static const unsigned int ID_FACTION_LEVEL_IS_FULL = 4;
	static const char* STR_FACTION_LEVEL_IS_FULL = "门派已满级";

	static const unsigned int ID_UNOPEN = 5;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_FACTION_UPGRADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_UPGRADE_LIMIT_NOT_ENOUGH
	,STR_FACTION_FUNDS_NOT_ENOUGH
	,STR_FACTION_LEVEL_IS_FULL
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_EQUIP_MASTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "装备精通未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_EQUIP_MASTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_ACTIVATE_MASTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "装备精通未开放";

	static const unsigned int ID_MEDAL_NOT_ENOUGH = 2;
	static const char* STR_MEDAL_NOT_ENOUGH = "勇气勋章不足";

	static const unsigned int ID_PLAYER_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_PLAYER_LEVEL_NOT_ENOUGH = "玩家等级不足";

	static const char* STR_NAMESPACE = "ERR_ACTIVATE_MASTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_MEDAL_NOT_ENOUGH
	,STR_PLAYER_LEVEL_NOT_ENOUGH
	};
}

namespace ERR_BREAK_MASTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "装备精通未开放";

	static const unsigned int ID_MEDAL_NOT_ENOUGH = 2;
	static const char* STR_MEDAL_NOT_ENOUGH = "勇气勋章不足";

	static const unsigned int ID_PLAYER_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_PLAYER_LEVEL_NOT_ENOUGH = "玩家等级不足";

	static const char* STR_NAMESPACE = "ERR_BREAK_MASTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_MEDAL_NOT_ENOUGH
	,STR_PLAYER_LEVEL_NOT_ENOUGH
	};
}

namespace ERR_GET_MASTER_BOOK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "装备精通未开放";

	static const unsigned int ID_NOT_ACTIVATE_ALL = 2;
	static const char* STR_NOT_ACTIVATE_ALL = "你还没全部激活，全部激活后才可以培养";

	static const unsigned int ID_MEDAL_NOT_ENOUGH = 3;
	static const char* STR_MEDAL_NOT_ENOUGH = "勇气勋章不足";

	static const unsigned int ID_MASTER_MAX = 4;
	static const char* STR_MASTER_MAX = "你的精通技能已经登峰造极，不用再培养了";

	static const unsigned int ID_UPGRADE_MASTER_PLEASE = 5;
	static const char* STR_UPGRADE_MASTER_PLEASE = "你还没突破，请先突破再培养";

	static const char* STR_NAMESPACE = "ERR_GET_MASTER_BOOK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NOT_ACTIVATE_ALL
	,STR_MEDAL_NOT_ENOUGH
	,STR_MASTER_MAX
	,STR_UPGRADE_MASTER_PLEASE
	};
}

namespace ERR_WIPE_MASTER_BOOK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "装备精通未开放";

	static const char* STR_NAMESPACE = "ERR_WIPE_MASTER_BOOK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_MASTER_CHEAT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "装备精通未开放";

	static const unsigned int ID_CHEAT_UNOPEN = 2;
	static const char* STR_CHEAT_UNOPEN = "等级不足，出千未开放";

	static const unsigned int ID_GOLD_NOT_ENOUGH = 3;
	static const char* STR_GOLD_NOT_ENOUGH = "元宝不足";

	static const unsigned int ID_NOT_NEED_TO_CHEAT = 4;
	static const char* STR_NOT_NEED_TO_CHEAT = "精通之书都已达到最高级，不需要出千";

	static const char* STR_NAMESPACE = "ERR_MASTER_CHEAT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_CHEAT_UNOPEN
	,STR_GOLD_NOT_ENOUGH
	,STR_NOT_NEED_TO_CHEAT
	};
}

namespace ERR_CONFIRM_MASTER_BOOK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "装备精通未开放";

	static const char* STR_NAMESPACE = "ERR_CONFIRM_MASTER_BOOK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_BAD_REWARD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_WINE_NOT_BAD = 1;
	static const char* STR_WINE_NOT_BAD = "还未变质";

	static const unsigned int ID_FACTION_NOT_EXIST = 2;
	static const char* STR_FACTION_NOT_EXIST = "还未加入门派";

	static const unsigned int ID_WINE_NOT_EXIST = 3;
	static const char* STR_WINE_NOT_EXIST = "酒不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_BAD_REWARD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_WINE_NOT_BAD
	,STR_FACTION_NOT_EXIST
	,STR_WINE_NOT_EXIST
	};
}

namespace ERR_GET_BAD_REWARD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_WINE_NOT_BAD = 1;
	static const char* STR_WINE_NOT_BAD = "还未变质";

	static const unsigned int ID_COIN_NOT_ENOUGH = 2;
	static const char* STR_COIN_NOT_ENOUGH = "铜钱不足";

	static const unsigned int ID_FACTION_NOT_EXIST = 3;
	static const char* STR_FACTION_NOT_EXIST = "还未加入门派";

	static const unsigned int ID_WINT_NOT_EXIST = 4;
	static const char* STR_WINT_NOT_EXIST = "酒不存在";

	static const char* STR_NAMESPACE = "ERR_GET_BAD_REWARD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_WINE_NOT_BAD
	,STR_COIN_NOT_ENOUGH
	,STR_FACTION_NOT_EXIST
	,STR_WINT_NOT_EXIST
	};
}

namespace ERR_OPEN_COURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_GROUP = 2;
	static const char* STR_NO_GROUP = "没有合适的队伍";

	static const unsigned int ID_LOCK = 3;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_COURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NO_GROUP
	,STR_LOCK
	};
}

namespace ERR_COURAGE_OPEN_CREATE_GROUP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const char* STR_NAMESPACE = "ERR_COURAGE_OPEN_CREATE_GROUP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_COURAGE_CREATE_GROUP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_TIMES = 2;
	static const char* STR_NO_TIMES = "今日试炼次数已用完，不能创建队伍";

	static const unsigned int ID_LOCK = 3;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_MAX_GROUP = 4;
	static const char* STR_MAX_GROUP = "队伍已达最大数量，队伍创建失败";

	static const unsigned int ID_NO_HERO = 5;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_FACTION = 6;
	static const char* STR_NO_FACTION = "您还没有加入帮派，不能创建帮派队伍";

	static const unsigned int ID_NO_SCENE = 7;
	static const char* STR_NO_SCENE = "加载场景出错";

	static const unsigned int ID_NO_LEVEL = 8;
	static const char* STR_NO_LEVEL = "不能创建比自己等级高的副本";

	static const char* STR_NAMESPACE = "ERR_COURAGE_CREATE_GROUP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NO_TIMES
	,STR_LOCK
	,STR_MAX_GROUP
	,STR_NO_HERO
	,STR_NO_FACTION
	,STR_NO_SCENE
	,STR_NO_LEVEL
	};
}

namespace ERR_COURAGE_BATTLE_ENTER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_ENOUGH_LEVEL = 2;
	static const char* STR_NO_ENOUGH_LEVEL = "等级尚未达到加入的条件";

	static const unsigned int ID_HAS_FULL = 3;
	static const char* STR_HAS_FULL = "您太慢了队伍已经满人了";

	static const unsigned int ID_NO_GROUP = 4;
	static const char* STR_NO_GROUP = "您选择的队伍被解散了";

	static const unsigned int ID_LOCK = 5;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 6;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_RIGHT_GROUP = 7;
	static const char* STR_NO_RIGHT_GROUP = "没有合适的队伍，创建一个吧";

	static const unsigned int ID_NO_HELP_TIMES = 8;
	static const char* STR_NO_HELP_TIMES = "您今日可协助次数已经用完";

	static const unsigned int ID_NO_HELP_TIMES_NOON = 9;
	static const char* STR_NO_HELP_TIMES_NOON = "午间时段的协助次数已达上限";

	static const unsigned int ID_NO_HELP_TIMES_NIGHT = 10;
	static const char* STR_NO_HELP_TIMES_NIGHT = "晚间时段的协助次数已达上限";

	static const char* STR_NAMESPACE = "ERR_COURAGE_BATTLE_ENTER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NO_ENOUGH_LEVEL
	,STR_HAS_FULL
	,STR_NO_GROUP
	,STR_LOCK
	,STR_NO_HERO
	,STR_NO_RIGHT_GROUP
	,STR_NO_HELP_TIMES
	,STR_NO_HELP_TIMES_NOON
	,STR_NO_HELP_TIMES_NIGHT
	};
}

namespace ERR_COURAGE_RANDOM_JOIN_GROUP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_ENOUGH_LEVEL = 2;
	static const char* STR_NO_ENOUGH_LEVEL = "等级尚未达到加入的条件";

	static const unsigned int ID_HAS_FULL = 3;
	static const char* STR_HAS_FULL = "您太慢了队伍已经满人了";

	static const unsigned int ID_NO_GROUP = 4;
	static const char* STR_NO_GROUP = "您选择的队伍被解散了";

	static const unsigned int ID_LOCK = 5;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 6;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_RIGHT_GROUP = 7;
	static const char* STR_NO_RIGHT_GROUP = "没有合适的队伍，创建一个吧";

	static const unsigned int ID_NO_HELP_TIMES = 8;
	static const char* STR_NO_HELP_TIMES = "您今日可协助次数已经用完";

	static const unsigned int ID_NO_HELP_TIMES_NOON = 9;
	static const char* STR_NO_HELP_TIMES_NOON = "午间时段的协助次数已达上限";

	static const unsigned int ID_NO_HELP_TIMES_NIGHT = 10;
	static const char* STR_NO_HELP_TIMES_NIGHT = "晚间时段的协助次数已达上限";

	static const char* STR_NAMESPACE = "ERR_COURAGE_RANDOM_JOIN_GROUP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NO_ENOUGH_LEVEL
	,STR_HAS_FULL
	,STR_NO_GROUP
	,STR_LOCK
	,STR_NO_HERO
	,STR_NO_RIGHT_GROUP
	,STR_NO_HELP_TIMES
	,STR_NO_HELP_TIMES_NOON
	,STR_NO_HELP_TIMES_NIGHT
	};
}

namespace ERR_COURAGE_OPERATE_HERO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_BATTLEING = 2;
	static const char* STR_BATTLEING = "战斗状态不能进行此操作";

	static const unsigned int ID_HAS_REANY = 3;
	static const char* STR_HAS_REANY = "准备后不能进行此操作";

	static const unsigned int ID_LOCK = 4;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 5;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_IN_GROUP = 6;
	static const char* STR_NO_IN_GROUP = "尚未加入队伍不能进行此操作";

	static const char* STR_NAMESPACE = "ERR_COURAGE_OPERATE_HERO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_BATTLEING
	,STR_HAS_REANY
	,STR_LOCK
	,STR_NO_HERO
	,STR_NO_IN_GROUP
	};
}

namespace ERR_COURAGE_CONFIRM_HERO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_BATTLEING = 2;
	static const char* STR_BATTLEING = "战斗状态不能进行此操作";

	static const unsigned int ID_HAS_REANY = 3;
	static const char* STR_HAS_REANY = "准备后不能进行此操作";

	static const unsigned int ID_NO_HERO_INFO = 4;
	static const char* STR_NO_HERO_INFO = "武将信息不存在";

	static const unsigned int ID_LOCK = 5;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 6;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_IN_GROUP = 7;
	static const char* STR_NO_IN_GROUP = "尚未加入队伍不能进行此操作";

	static const char* STR_NAMESPACE = "ERR_COURAGE_CONFIRM_HERO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_BATTLEING
	,STR_HAS_REANY
	,STR_NO_HERO_INFO
	,STR_LOCK
	,STR_NO_HERO
	,STR_NO_IN_GROUP
	};
}

namespace ERR_COURAGE_FIRE_GROUP_MEMBER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_BATTLEING = 2;
	static const char* STR_BATTLEING = "战斗状态不能进行此操作";

	static const unsigned int ID_HAS_REANY = 3;
	static const char* STR_HAS_REANY = "准备后不能进行此操作";

	static const unsigned int ID_NO_GROUP = 4;
	static const char* STR_NO_GROUP = "尚未加入队伍不能进行此操作";

	static const unsigned int ID_NO_POWER = 5;
	static const char* STR_NO_POWER = "没有权限";

	static const unsigned int ID_NO_PLAYER = 6;
	static const char* STR_NO_PLAYER = "您下手慢了，此人自己跑了";

	static const unsigned int ID_NO_MYSELF = 7;
	static const char* STR_NO_MYSELF = "不能踢自己";

	static const unsigned int ID_LOCK = 8;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_COURAGE_FIRE_GROUP_MEMBER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_BATTLEING
	,STR_HAS_REANY
	,STR_NO_GROUP
	,STR_NO_POWER
	,STR_NO_PLAYER
	,STR_NO_MYSELF
	,STR_LOCK
	};
}

namespace ERR_COURAGE_PLAYER_READY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_BATTLEING = 2;
	static const char* STR_BATTLEING = "战斗状态不能进行此操作";

	static const unsigned int ID_NO_GROUP = 3;
	static const char* STR_NO_GROUP = "尚未加入队伍不能进行此操作";

	static const unsigned int ID_LOCK = 4;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 5;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const char* STR_NAMESPACE = "ERR_COURAGE_PLAYER_READY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_BATTLEING
	,STR_NO_GROUP
	,STR_LOCK
	,STR_NO_HERO
	};
}

namespace ERR_COURAGE_PLAYER_START 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_BATTLEING = 2;
	static const char* STR_BATTLEING = "战斗状态不能进行此操作";

	static const unsigned int ID_NO_GROUP = 3;
	static const char* STR_NO_GROUP = "尚未加入队伍不能进行此操作";

	static const unsigned int ID_NO_ALL_READY = 4;
	static const char* STR_NO_ALL_READY = "还有队员未准备，不能开始游戏";

	static const unsigned int ID_NO_POWER = 5;
	static const char* STR_NO_POWER = "只有队长才能开始游戏哦";

	static const unsigned int ID_LOCK = 6;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 7;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_FULL = 8;
	static const char* STR_NO_FULL = "人数不够不能开始游戏哦";

	static const char* STR_NAMESPACE = "ERR_COURAGE_PLAYER_START";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_BATTLEING
	,STR_NO_GROUP
	,STR_NO_ALL_READY
	,STR_NO_POWER
	,STR_LOCK
	,STR_NO_HERO
	,STR_NO_FULL
	};
}

namespace ERR_COURAGE_BUY_PRACTISE_TIMES 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_MAX_BUY_TIMES = 2;
	static const char* STR_MAX_BUY_TIMES = "超过今日可购买的次数";

	static const unsigned int ID_NO_GOLD = 3;
	static const char* STR_NO_GOLD = "元宝不足";

	static const unsigned int ID_LOCK = 4;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_COURAGE_BUY_PRACTISE_TIMES";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_MAX_BUY_TIMES
	,STR_NO_GOLD
	,STR_LOCK
	};
}

namespace ERR_COURAGE_LEAVE_GROUP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_BATTLEING = 2;
	static const char* STR_BATTLEING = "战斗状态不能进行此操作";

	static const unsigned int ID_NO_GROUP = 3;
	static const char* STR_NO_GROUP = "没有合适的队伍";

	static const unsigned int ID_LOCK = 4;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_COURAGE_LEAVE_GROUP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_BATTLEING
	,STR_NO_GROUP
	,STR_LOCK
	};
}

namespace ERR_COURAGE_BATTLE_LEAVE_GROUP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_BATTLEING = 2;
	static const char* STR_BATTLEING = "战斗状态不能进行此操作";

	static const unsigned int ID_NO_GROUP = 3;
	static const char* STR_NO_GROUP = "没有合适的队伍";

	static const unsigned int ID_LOCK = 4;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_COURAGE_BATTLE_LEAVE_GROUP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_BATTLEING
	,STR_NO_GROUP
	,STR_LOCK
	};
}

namespace ERR_OPEN_COURAGE_SHOP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_MAX_BUY_TIMES = 2;
	static const char* STR_MAX_BUY_TIMES = "超过今日可购买的次数";

	static const unsigned int ID_LOCK = 3;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_OPEN_COURAGE_SHOP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_MAX_BUY_TIMES
	,STR_LOCK
	};
}

namespace ERR_COURAGE_RCV_READY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_MAX_BUY_TIMES = 2;
	static const char* STR_MAX_BUY_TIMES = "超过今日可购买的次数";

	static const unsigned int ID_LOCK = 3;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_COURAGE_RCV_READY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_MAX_BUY_TIMES
	,STR_LOCK
	};
}

namespace ERR_COURAGE_RCV_REFRESH 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_MAX_BUY_TIMES = 2;
	static const char* STR_MAX_BUY_TIMES = "超过今日可购买的次数";

	static const unsigned int ID_LOCK = 3;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_COURAGE_RCV_REFRESH";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_MAX_BUY_TIMES
	,STR_LOCK
	};
}

namespace ERR_PLAYER_FACTION_READ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_PLAYER_FACTION_READ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_FIND_DRINK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "未加入门派";

	static const unsigned int ID_NOT_FIND_BACK_DRINK = 2;
	static const char* STR_NOT_FIND_BACK_DRINK = "不符合找回上次服务机会的条件";

	static const unsigned int ID_NOT_FIND_BACK_IN_GIVE_WINE = 3;
	static const char* STR_NOT_FIND_BACK_IN_GIVE_WINE = "休闲时光期间无法找回上次服务机会";

	static const unsigned int ID_HAVE_A_WINE = 4;
	static const char* STR_HAVE_A_WINE = "你正在享受服务中";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 5;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_UNOPEN = 6;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_FIND_DRINK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_NOT_FIND_BACK_DRINK
	,STR_NOT_FIND_BACK_IN_GIVE_WINE
	,STR_HAVE_A_WINE
	,STR_HOT_SPRING_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_OPEN_FACTION_BUILD_GROUP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "还未加入门派";

	static const unsigned int ID_UNOPEN = 2;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_FACTION_BUILD_GROUP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_COURAGE_PLAYER_FIRED 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "您被队长踢出队伍";

	static const char* STR_NAMESPACE = "ERR_COURAGE_PLAYER_FIRED";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_COURAGE_GROUP_DISMISS 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "队伍被队长解散了";

	static const char* STR_NAMESPACE = "ERR_COURAGE_GROUP_DISMISS";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_COURAGE_GROUP_REFRESH 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "刷新成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_GROUP = 2;
	static const char* STR_NO_GROUP = "没有满足条件的队伍";

	static const char* STR_NAMESPACE = "ERR_COURAGE_GROUP_REFRESH";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NO_GROUP
	};
}

namespace ERR_FACTION_PLAYER_CHANGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_EXIST = 1;
	static const char* STR_FACTION_NOT_EXIST = "该门派不存在";

	static const unsigned int ID_UNOPEN = 2;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_FACTION_PLAYER_CHANGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_EXIST
	,STR_UNOPEN
	};
}

namespace ERR_COURAGE_ENTER_GROUP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_GROUP = 2;
	static const char* STR_NO_GROUP = "队伍被队长解散了";

	static const unsigned int ID_LOCK = 3;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 4;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const char* STR_NAMESPACE = "ERR_COURAGE_ENTER_GROUP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NO_GROUP
	,STR_LOCK
	,STR_NO_HERO
	};
}

namespace ERR_LEAVE_COURAGE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_LOCK = 2;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_LEAVE_COURAGE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_LOCK
	};
}

namespace ERR_HOT_SPRING_OVER_REWARD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HOT_SPRING_NOT_EXIST = 1;
	static const char* STR_HOT_SPRING_NOT_EXIST = "温泉还未开放";

	static const unsigned int ID_NOT_OVER_REWARD = 2;
	static const char* STR_NOT_OVER_REWARD = "没有过期奖励";

	static const unsigned int ID_UNOPEN = 3;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_HOT_SPRING_OVER_REWARD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HOT_SPRING_NOT_EXIST
	,STR_NOT_OVER_REWARD
	,STR_UNOPEN
	};
}

namespace ERR_HUNTING_ITEGRAL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_SHADE_NOT_EXIST = 1;
	static const char* STR_SHADE_NOT_EXIST = "暗部还未开放";

	static const unsigned int ID_TIME_NOT_ENOUGH = 2;
	static const char* STR_TIME_NOT_ENOUGH = "当前非捉妖任务时段";

	static const unsigned int ID_UNOPEN = 3;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_HUNTING_ITEGRAL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_SHADE_NOT_EXIST
	,STR_TIME_NOT_ENOUGH
	,STR_UNOPEN
	};
}

namespace ERR_PROMPT_IGNORE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_OPEN = 1;
	static const char* STR_FACTION_NOT_OPEN = "门派还未开放";

	static const unsigned int ID_UNOPEN = 2;
	static const char* STR_UNOPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_PROMPT_IGNORE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_OPEN
	,STR_UNOPEN
	};
}

namespace ERR_COURAGE_BATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FAILED = 1;
	static const char* STR_FAILED = "战斗失败";

	static const unsigned int ID_UNOPEN = 2;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_GROUP = 3;
	static const char* STR_NO_GROUP = "尚未加入队伍不能进行此操作";

	static const unsigned int ID_LOCK = 4;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 5;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_BATTLE = 6;
	static const char* STR_NO_BATTLE = "战斗未开始";

	static const char* STR_NAMESPACE = "ERR_COURAGE_BATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FAILED
	,STR_UNOPEN
	,STR_NO_GROUP
	,STR_LOCK
	,STR_NO_HERO
	,STR_NO_BATTLE
	};
}

namespace ERR_OPEN_LEAVE_HOT_SPRING 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "功能还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_LEAVE_HOT_SPRING";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_COURAGE_GET_AWARD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HAVEGETAWARD = 1;
	static const char* STR_HAVEGETAWARD = "您已领取过奖励";

	static const unsigned int ID_LOCK = 2;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_BAG_FULL = 3;
	static const char* STR_BAG_FULL = "背包已满";

	static const char* STR_NAMESPACE = "ERR_COURAGE_GET_AWARD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HAVEGETAWARD
	,STR_LOCK
	,STR_BAG_FULL
	};
}

namespace ERR_COURAGE_ENTER_BATTLE_SCENE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "当前活动结束了，请等待下次吧";

	static const unsigned int ID_NO_GROUP = 2;
	static const char* STR_NO_GROUP = "尚未加入队伍不能进行此操作";

	static const unsigned int ID_LOCK = 3;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const unsigned int ID_NO_HERO = 4;
	static const char* STR_NO_HERO = "请至少携带一个伙伴参加活动";

	static const unsigned int ID_NO_BATTLE = 5;
	static const char* STR_NO_BATTLE = "战斗未开始";

	static const char* STR_NAMESPACE = "ERR_COURAGE_ENTER_BATTLE_SCENE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_NO_GROUP
	,STR_LOCK
	,STR_NO_HERO
	,STR_NO_BATTLE
	};
}

namespace ERR_COURAGE_GET_BATTLE_RESULT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HAVEGETAWARD = 1;
	static const char* STR_HAVEGETAWARD = "您已领取过奖励";

	static const unsigned int ID_LOCK = 2;
	static const char* STR_LOCK = "勇气试炼功能还未解锁";

	static const char* STR_NAMESPACE = "ERR_COURAGE_GET_BATTLE_RESULT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HAVEGETAWARD
	,STR_LOCK
	};
}

namespace ERR_UPGRADE_CROP_FIELD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FIELD_NOT_EXIT = 1;
	static const char* STR_FIELD_NOT_EXIT = "地块不存在";

	static const unsigned int ID_FIELD_IS_MAX = 2;
	static const char* STR_FIELD_IS_MAX = "已经升到最高级了";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_LEVEL_NOT_ENOUGH = "主角等级不够";

	static const unsigned int ID_CROP_IS_NOT_OPEN = 4;
	static const char* STR_CROP_IS_NOT_OPEN = "农场未开放";

	static const unsigned int ID_CROP_IS_NOT_GOLD = 5;
	static const char* STR_CROP_IS_NOT_GOLD = "金币不足，不能升级";

	static const unsigned int ID_CROP_IS_NOT_VIP = 6;
	static const char* STR_CROP_IS_NOT_VIP = "VIP等级不够，不能升级";

	static const unsigned int ID_FIELD_HAVE_CROP = 7;
	static const char* STR_FIELD_HAVE_CROP = "地块已种值，还不能升级";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_CROP_FIELD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FIELD_NOT_EXIT
	,STR_FIELD_IS_MAX
	,STR_LEVEL_NOT_ENOUGH
	,STR_CROP_IS_NOT_OPEN
	,STR_CROP_IS_NOT_GOLD
	,STR_CROP_IS_NOT_VIP
	,STR_FIELD_HAVE_CROP
	};
}

namespace ERR_OPEN_RESOLVE_ORANGE_AWAKEN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "分解功能还未开启";

	static const char* STR_NAMESPACE = "ERR_OPEN_RESOLVE_ORANGE_AWAKEN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	};
}

namespace ERR_RESOLVE_ORANGE_AWAKEN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "分解功能还未开启";

	static const unsigned int ID_ID_INVALID = 2;
	static const char* STR_ID_INVALID = "卦象不识别";

	static const unsigned int ID_ITEM_NUM_NOT_ENOUGH = 3;
	static const char* STR_ITEM_NUM_NOT_ENOUGH = "道具数量不足";

	static const unsigned int ID_NOT_HAVE_AWAKEN = 4;
	static const char* STR_NOT_HAVE_AWAKEN = "背包中没有此卦象";

	static const unsigned int ID_ITEM_ID_INVALID = 5;
	static const char* STR_ITEM_ID_INVALID = "道具信息异常，请联系客服";

	static const unsigned int ID_IS_NOT_ORANGE_STUDY = 6;
	static const char* STR_IS_NOT_ORANGE_STUDY = "不是黄色卦象不能分解";

	static const char* STR_NAMESPACE = "ERR_RESOLVE_ORANGE_AWAKEN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_ID_INVALID
	,STR_ITEM_NUM_NOT_ENOUGH
	,STR_NOT_HAVE_AWAKEN
	,STR_ITEM_ID_INVALID
	,STR_IS_NOT_ORANGE_STUDY
	};
}

namespace ERR_OPEN_RESOLVE_TRIGRAM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "分解功能还未开启";

	static const unsigned int ID_IS_NOT_ORANGE_STUDY = 2;
	static const char* STR_IS_NOT_ORANGE_STUDY = "此卦象不能分解";

	static const unsigned int ID_ID_INVALID = 3;
	static const char* STR_ID_INVALID = "卦象不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_RESOLVE_TRIGRAM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_IS_NOT_ORANGE_STUDY
	,STR_ID_INVALID
	};
}

namespace ERR_RESOLVE_TRIGRAM 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "分解功能还未开启";

	static const unsigned int ID_ID_INVALID = 2;
	static const char* STR_ID_INVALID = "卦象不存在";

	static const unsigned int ID_NOT_ENOUGH_GOLD = 3;
	static const char* STR_NOT_ENOUGH_GOLD = "元宝不足";

	static const unsigned int ID_IS_NOT_ORANGE_STUDY = 4;
	static const char* STR_IS_NOT_ORANGE_STUDY = "此卦象不能分解";

	static const unsigned int ID_NOT_ENOUGH_COIN = 5;
	static const char* STR_NOT_ENOUGH_COIN = "铜钱不足";

	static const char* STR_NAMESPACE = "ERR_RESOLVE_TRIGRAM";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_ID_INVALID
	,STR_NOT_ENOUGH_GOLD
	,STR_IS_NOT_ORANGE_STUDY
	,STR_NOT_ENOUGH_COIN
	};
}

namespace ERR_AUDIT_ALL_REFUSE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_UNOPEN = 1;
	static const char* STR_UNOPEN = "门派功能还未开启";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 2;
	static const char* STR_LIMIT_NOT_ENOUGH = "你没有审核权限";

	static const unsigned int ID_PLAYER_NUM_IS_ZERO = 3;
	static const char* STR_PLAYER_NUM_IS_ZERO = "当前无人申请";

	static const unsigned int ID_NOT_JOIN_FACTION = 4;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const char* STR_NAMESPACE = "ERR_AUDIT_ALL_REFUSE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_UNOPEN
	,STR_LIMIT_NOT_ENOUGH
	,STR_PLAYER_NUM_IS_ZERO
	,STR_NOT_JOIN_FACTION
	};
}

namespace ERR_OPEN_VIP_GIFT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_VIP_GIFT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_GET_VIP_GIFT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "购买成功";

	static const unsigned int ID_BAG_FULL = 1;
	static const char* STR_BAG_FULL = "背包空间不足";

	static const unsigned int ID_GET_NUM_IS_NULL = 2;
	static const char* STR_GET_NUM_IS_NULL = "今日购买次数不足，请明天再来";

	static const unsigned int ID_VIP_LEVEL_NOT_AFFORD = 3;
	static const char* STR_VIP_LEVEL_NOT_AFFORD = "VIP等级不足";

	static const char* STR_NAMESPACE = "ERR_GET_VIP_GIFT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_BAG_FULL
	,STR_GET_NUM_IS_NULL
	,STR_VIP_LEVEL_NOT_AFFORD
	};
}

namespace ERR_OPEN_RECV_FLOWER_TAB 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_RECV_FLOWER_TAB";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	};
}

namespace ERR_ACCEPT_FLOWER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NO_OPEN = 1;
	static const char* STR_NO_OPEN = "好友功能未开放";

	static const unsigned int ID_RECEVED = 2;
	static const char* STR_RECEVED = "已收取";

	static const unsigned int ID_REQ_ERR = 3;
	static const char* STR_REQ_ERR = "收取的记录不存在";

	static const unsigned int ID_TODAY_NO_TIME = 4;
	static const char* STR_TODAY_NO_TIME = "当天次数已用尽";

	static const unsigned int ID_BAG_FULL = 5;
	static const char* STR_BAG_FULL = "背包满了";

	static const char* STR_NAMESPACE = "ERR_ACCEPT_FLOWER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NO_OPEN
	,STR_RECEVED
	,STR_REQ_ERR
	,STR_TODAY_NO_TIME
	,STR_BAG_FULL
	};
}

namespace ERR_FACTION_PLAYERS_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "门派系统还未开启";

	static const unsigned int ID_NOT_JOIN_FACTION = 2;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const char* STR_NAMESPACE = "ERR_FACTION_PLAYERS_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_JOIN_FACTION
	};
}

namespace ERR_CHANGE_FACTION_ICON 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "门派系统还未开启";

	static const unsigned int ID_NOT_JOIN_FACTION = 2;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 3;
	static const char* STR_LIMIT_NOT_ENOUGH = "只有掌门才能更改门派标志";

	static const char* STR_NAMESPACE = "ERR_CHANGE_FACTION_ICON";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_JOIN_FACTION
	,STR_LIMIT_NOT_ENOUGH
	};
}

namespace ERR_OPEN_CHANGE_FACTION_ICON 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "门派系统还未开启";

	static const unsigned int ID_NOT_JOIN_FACTION = 2;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const unsigned int ID_LIMIT_NOT_ENOUGH = 3;
	static const char* STR_LIMIT_NOT_ENOUGH = "只有掌门才能更改门派标志";

	static const char* STR_NAMESPACE = "ERR_OPEN_CHANGE_FACTION_ICON";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_JOIN_FACTION
	,STR_LIMIT_NOT_ENOUGH
	};
}

namespace ERR_REGISTER_FACTION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "门派系统还未开启";

	static const unsigned int ID_UNREGISTER = 2;
	static const char* STR_UNREGISTER = "禁止修改门派名称";

	static const unsigned int ID_NOTLEAD_UNREGISTER = 3;
	static const char* STR_NOTLEAD_UNREGISTER = "非掌门禁止修改门派名称";

	static const unsigned int ID_INVALID_CHARACTER = 4;
	static const char* STR_INVALID_CHARACTER = "门派名称只能使用中文、英文和数字";

	static const unsigned int ID_DSPNAME_CONFLICT = 5;
	static const char* STR_DSPNAME_CONFLICT = "该名称已被别的门派使用";

	static const unsigned int ID_SENSITIVE_WORD = 6;
	static const char* STR_SENSITIVE_WORD = "门派名称中含有敏感词汇，请重新输入";

	static const unsigned int ID_TOO_LONG = 7;
	static const char* STR_TOO_LONG = "门派名称过长，不能超过6个汉字或12个字母";

	static const char* STR_NAMESPACE = "ERR_REGISTER_FACTION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNREGISTER
	,STR_NOTLEAD_UNREGISTER
	,STR_INVALID_CHARACTER
	,STR_DSPNAME_CONFLICT
	,STR_SENSITIVE_WORD
	,STR_TOO_LONG
	};
}

namespace ERR_OPEN_FIND_BACK_DOORSTRIBUTE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "门派系统还未开启";

	static const unsigned int ID_NOT_DOORSTRIBUTE_FIND_BACK = 2;
	static const char* STR_NOT_DOORSTRIBUTE_FIND_BACK = "你当前没有历史剩余门贡可找回";

	static const unsigned int ID_NOT_JOIN_FACTION = 3;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const char* STR_NAMESPACE = "ERR_OPEN_FIND_BACK_DOORSTRIBUTE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_DOORSTRIBUTE_FIND_BACK
	,STR_NOT_JOIN_FACTION
	};
}

namespace ERR_FIND_BACK_DOORSTRIBUTE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "门派系统还未开启";

	static const unsigned int ID_NOT_DOORSTRIBUTE_FIND_BACK = 2;
	static const char* STR_NOT_DOORSTRIBUTE_FIND_BACK = "你当前没有历史剩余门贡可找回";

	static const unsigned int ID_NOT_JOIN_FACTION = 3;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const char* STR_NAMESPACE = "ERR_FIND_BACK_DOORSTRIBUTE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_DOORSTRIBUTE_FIND_BACK
	,STR_NOT_JOIN_FACTION
	};
}

namespace ERR_UPGRADE_FORMATION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FORMATION_NOT_OPEN = 1;
	static const char* STR_FORMATION_NOT_OPEN = "布阵系统还未开放";

	static const unsigned int ID_FORMATION_ID_INVALIAD = 2;
	static const char* STR_FORMATION_ID_INVALIAD = "未找到相关阵法信息";

	static const unsigned int ID_PLAYER_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_PLAYER_LEVEL_NOT_ENOUGH = "已达到当前主角所能提升的最大阵法等级";

	static const unsigned int ID_STORY_NOT_ENOUGH = 4;
	static const char* STR_STORY_NOT_ENOUGH = "阅历不足无法升级";

	static const unsigned int ID_FORMATION_UNLOCK = 5;
	static const char* STR_FORMATION_UNLOCK = "该阵法还未解锁";

	static const unsigned int ID_FORMATION_NOT_UPGRADE = 6;
	static const char* STR_FORMATION_NOT_UPGRADE = "科技馆还未开放，不能升级阵法";

	static const unsigned int ID_MAX_FORMATION_LEVEL = 7;
	static const char* STR_MAX_FORMATION_LEVEL = "该技能已满级";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_FORMATION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FORMATION_NOT_OPEN
	,STR_FORMATION_ID_INVALIAD
	,STR_PLAYER_LEVEL_NOT_ENOUGH
	,STR_STORY_NOT_ENOUGH
	,STR_FORMATION_UNLOCK
	,STR_FORMATION_NOT_UPGRADE
	,STR_MAX_FORMATION_LEVEL
	};
}

namespace ERR_OPEN_FORMATION_SCIENCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FORMATION_NOT_OPEN = 1;
	static const char* STR_FORMATION_NOT_OPEN = "布阵系统还未开放";

	static const unsigned int ID_SCIENCE_NOT_OPEN = 2;
	static const char* STR_SCIENCE_NOT_OPEN = "科技馆还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_FORMATION_SCIENCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FORMATION_NOT_OPEN
	,STR_SCIENCE_NOT_OPEN
	};
}

namespace ERR_UPGRADE_FORMATION_SCIENCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_NOT_EXIST_ERR = 1;
	static const char* STR_HERO_NOT_EXIST_ERR = "伙伴不存在";

	static const unsigned int ID_FORMATION_INDEX_INVALID_ERR = 2;
	static const char* STR_FORMATION_INDEX_INVALID_ERR = "阵法位置无效";

	static const unsigned int ID_FORMATION_INDEX_HAVE_HERO_ERR = 3;
	static const char* STR_FORMATION_INDEX_HAVE_HERO_ERR = "该阵位已经有伙伴";

	static const unsigned int ID_FORMATION_KIND_NOT_EXIST = 4;
	static const char* STR_FORMATION_KIND_NOT_EXIST = "阵法不存在";

	static const unsigned int ID_COACH_NOT_IN_FORMATION = 5;
	static const char* STR_COACH_NOT_IN_FORMATION = "主角不在阵上";

	static const unsigned int ID_FORMATION_EXCEPTION = 6;
	static const char* STR_FORMATION_EXCEPTION = "阵法位置异常";

	static const unsigned int ID_POSTION_NOT_PLAYER = 7;
	static const char* STR_POSTION_NOT_PLAYER = "布阵异常，未解锁的阵位上有伙伴";

	static const unsigned int ID_FORMATION_NOT_OPEN = 8;
	static const char* STR_FORMATION_NOT_OPEN = "布阵系统还未开放";

	static const unsigned int ID_FORMATION_ID_INVALIAD = 9;
	static const char* STR_FORMATION_ID_INVALIAD = "未找到相关阵法信息";

	static const unsigned int ID_PLAYER_LEVEL_NOT_ENOUGH = 10;
	static const char* STR_PLAYER_LEVEL_NOT_ENOUGH = "已达到当前主角所能提升的最大阵法等级";

	static const unsigned int ID_STORY_NOT_ENOUGH = 11;
	static const char* STR_STORY_NOT_ENOUGH = "阅历不足无法升级";

	static const unsigned int ID_FORMATION_UNLOCK = 12;
	static const char* STR_FORMATION_UNLOCK = "该阵法还未解锁";

	static const unsigned int ID_FORMATION_NOT_UPGRADE = 13;
	static const char* STR_FORMATION_NOT_UPGRADE = "科技馆还未开放，不能升级阵法";

	static const unsigned int ID_MAX_FORMATION_LEVEL = 14;
	static const char* STR_MAX_FORMATION_LEVEL = "该技能已满级";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_FORMATION_SCIENCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_NOT_EXIST_ERR
	,STR_FORMATION_INDEX_INVALID_ERR
	,STR_FORMATION_INDEX_HAVE_HERO_ERR
	,STR_FORMATION_KIND_NOT_EXIST
	,STR_COACH_NOT_IN_FORMATION
	,STR_FORMATION_EXCEPTION
	,STR_POSTION_NOT_PLAYER
	,STR_FORMATION_NOT_OPEN
	,STR_FORMATION_ID_INVALIAD
	,STR_PLAYER_LEVEL_NOT_ENOUGH
	,STR_STORY_NOT_ENOUGH
	,STR_FORMATION_UNLOCK
	,STR_FORMATION_NOT_UPGRADE
	,STR_MAX_FORMATION_LEVEL
	};
}

namespace ERR_GET_EXPRIENCE_HOT_SPRING 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_OPEN = 1;
	static const char* STR_FACTION_NOT_OPEN = "门派系统还未开放";

	static const unsigned int ID_NOT_HAVE_FACTION = 2;
	static const char* STR_NOT_HAVE_FACTION = "还未加入门派";

	static const unsigned int ID_HERO_EXCEPTION = 3;
	static const char* STR_HERO_EXCEPTION = "主角数据异常，请退出界面重试";

	static const char* STR_NAMESPACE = "ERR_GET_EXPRIENCE_HOT_SPRING";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_OPEN
	,STR_NOT_HAVE_FACTION
	,STR_HERO_EXCEPTION
	};
}

namespace ERR_OPEN_FACTION_SKILL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_FACTION_NOT_OPEN = 1;
	static const char* STR_FACTION_NOT_OPEN = "门派系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_FACTION_SKILL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_FACTION_NOT_OPEN
	};
}

namespace ERR_OPEN_PLAYER_PANEL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_PLAYER_PANEL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_PLAYER_INFO_AWAKEN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "算卦系统还未开放";

	static const unsigned int ID_COACH_HERO_LOST = 2;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_PLAYER_INFO_AWAKEN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_PLAYER_INFO_SKILL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_COACH_HERO_LOST = 1;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_PLAYER_INFO_SKILL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_PLAYER_INFO_GODWEAPON 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "神器系统还未开放";

	static const unsigned int ID_NOT_PLAYER_INFO = 2;
	static const char* STR_NOT_PLAYER_INFO = "找不到玩家信息";

	static const unsigned int ID_COACH_HERO_LOST = 3;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_PLAYER_INFO_GODWEAPON";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_PLAYER_INFO
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_PLAYER_INFO_JEWELRY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_COACH_HERO_LOST = 1;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_PLAYER_INFO_JEWELRY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_DRESS_JEWELRY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_HERO_ID_NOT_FIND = 1;
	static const char* STR_HERO_ID_NOT_FIND = "找不到相关的伙伴信息";

	static const unsigned int ID_DRESS_TYPE_ERR = 2;
	static const char* STR_DRESS_TYPE_ERR = "未知操作类型";

	static const unsigned int ID_NOT_HAVE_JEWELRY = 3;
	static const char* STR_NOT_HAVE_JEWELRY = "背包里没有要穿戴的饰品";

	static const unsigned int ID_IS_ALREAD_DRESS = 4;
	static const char* STR_IS_ALREAD_DRESS = "该饰品已穿戴";

	static const unsigned int ID_IS_ALREAD_UNDRESS = 5;
	static const char* STR_IS_ALREAD_UNDRESS = "该饰品已脱下";

	static const unsigned int ID_BAG_IS_FULL = 6;
	static const char* STR_BAG_IS_FULL = "背包空间已满，请先整理背包";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 7;
	static const char* STR_LEVEL_NOT_ENOUGH = "该伙伴等级不足以佩戴此饰品，请升级后再试";

	static const char* STR_NAMESPACE = "ERR_DRESS_JEWELRY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_HERO_ID_NOT_FIND
	,STR_DRESS_TYPE_ERR
	,STR_NOT_HAVE_JEWELRY
	,STR_IS_ALREAD_DRESS
	,STR_IS_ALREAD_UNDRESS
	,STR_BAG_IS_FULL
	,STR_LEVEL_NOT_ENOUGH
	};
}

namespace ERR_OPEN_HREO_SELECT_PANEL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_HREO_SELECT_PANEL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_JEWELRY_GENERATE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_JEWELRY_GENERATE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_JEWELRY_GENERATE_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_EXCEPTION = 1;
	static const char* STR_IS_EXCEPTION = "合成信息出现异常";

	static const char* STR_NAMESPACE = "ERR_JEWELRY_GENERATE_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_EXCEPTION
	};
}

namespace ERR_JEWELRY_GENERATE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_ALREAD_STRENG = 1;
	static const char* STR_ALREAD_STRENG = "精炼过的饰品不能参与合成，可以通过分解获得精炼道具";

	static const unsigned int ID_SELECT_JEWELRY = 2;
	static const char* STR_SELECT_JEWELRY = "饰品数量不足以合成，请选择4个饰品";

	static const unsigned int ID_IS_EXCEPTION = 3;
	static const char* STR_IS_EXCEPTION = "合成信息出现异常";

	static const unsigned int ID_BAG_IS_FULL = 4;
	static const char* STR_BAG_IS_FULL = "背包已满，请先整理背包";

	static const char* STR_NAMESPACE = "ERR_JEWELRY_GENERATE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_ALREAD_STRENG
	,STR_SELECT_JEWELRY
	,STR_IS_EXCEPTION
	,STR_BAG_IS_FULL
	};
}

namespace ERR_JEWELRY_STRENGTH 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FIND_JEWELRY = 1;
	static const char* STR_NOT_FIND_JEWELRY = "未找到背包中找到饰品信息";

	static const unsigned int ID_IS_MAX_LEVEL = 2;
	static const char* STR_IS_MAX_LEVEL = "已精炼至最高级";

	static const unsigned int ID_ITEM_NOT_ENOUGH = 3;
	static const char* STR_ITEM_NOT_ENOUGH = "精炼道具不足";

	static const unsigned int ID_PALYER_LEVEL_NOT_ENOUGH = 4;
	static const char* STR_PALYER_LEVEL_NOT_ENOUGH = "玩家等级不足以精炼该饰品";

	static const unsigned int ID_NOT_STRENG_EXT = 5;
	static const char* STR_NOT_STRENG_EXT = "该饰品精炼等级还不足以精炼附加属性";

	static const unsigned int ID_IS_EXCEPTION = 6;
	static const char* STR_IS_EXCEPTION = "饰品精炼信息出现异常";

	static const char* STR_NAMESPACE = "ERR_JEWELRY_STRENGTH";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FIND_JEWELRY
	,STR_IS_MAX_LEVEL
	,STR_ITEM_NOT_ENOUGH
	,STR_PALYER_LEVEL_NOT_ENOUGH
	,STR_NOT_STRENG_EXT
	,STR_IS_EXCEPTION
	};
}

namespace ERR_PLAYER_INFO_EQUIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_COACH_HERO_LOST = 1;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_PLAYER_INFO_EQUIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_JEWELRY_RESOLVE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_HAVE_JEWELRY = 1;
	static const char* STR_NOT_HAVE_JEWELRY = "找不到该饰品信息";

	static const unsigned int ID_BAG_IS_FULL = 2;
	static const char* STR_BAG_IS_FULL = "背包空间不足，请先整理背包";

	static const unsigned int ID_JEWELRY_INFO_UNNORMAL = 3;
	static const char* STR_JEWELRY_INFO_UNNORMAL = "饰品数据异常";

	static const char* STR_NAMESPACE = "ERR_JEWELRY_RESOLVE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_HAVE_JEWELRY
	,STR_BAG_IS_FULL
	,STR_JEWELRY_INFO_UNNORMAL
	};
}

namespace ERR_OPEN_JEWELRY_STRENGTH 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_COACH_HERO_LOST = 1;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_OPEN_JEWELRY_STRENGTH";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_JEWELRY_COMPARE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FIND_JEWELRY = 1;
	static const char* STR_NOT_FIND_JEWELRY = "找不到饰品信息";

	static const unsigned int ID_COACH_HERO_LOST = 2;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_JEWELRY_COMPARE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FIND_JEWELRY
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_JEWELRY_EXT_UPGRADE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_MAX = 1;
	static const char* STR_IS_MAX = "该属性已提升至最高级";

	static const unsigned int ID_NOT_FIND_JEWELRY = 2;
	static const char* STR_NOT_FIND_JEWELRY = "找不到饰品信息";

	static const unsigned int ID_NOT_STRENG_NUM = 3;
	static const char* STR_NOT_STRENG_NUM = "附加属性已无剩余强化次数";

	static const unsigned int ID_EXT_ID_NOT_FIND = 4;
	static const char* STR_EXT_ID_NOT_FIND = "找不到相关的附加属性";

	static const char* STR_NAMESPACE = "ERR_JEWELRY_EXT_UPGRADE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_MAX
	,STR_NOT_FIND_JEWELRY
	,STR_NOT_STRENG_NUM
	,STR_EXT_ID_NOT_FIND
	};
}

namespace ERR_OPEN_JEWELRY_RESOLVE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FIND_JEWELRY = 1;
	static const char* STR_NOT_FIND_JEWELRY = "找不到饰品信息";

	static const unsigned int ID_JEWELRY_INFO_EXCEPTION = 2;
	static const char* STR_JEWELRY_INFO_EXCEPTION = "饰品信息异常";

	static const char* STR_NAMESPACE = "ERR_OPEN_JEWELRY_RESOLVE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FIND_JEWELRY
	,STR_JEWELRY_INFO_EXCEPTION
	};
}

namespace ERR_GVG_OPEN_MY_GAMBLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服门派战还未开放";

	static const char* STR_NAMESPACE = "ERR_GVG_OPEN_MY_GAMBLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_OPEN_FACTION_MONEY_RANK 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_FACTION_MONEY_RANK";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_GVG_RULE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_OPEN_GVG_RULE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_ALREAD_APPLAY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_APPLAY_TIME = 1;
	static const char* STR_NOT_APPLAY_TIME = "当前不处于报名阶段，请等待";

	static const char* STR_NAMESPACE = "ERR_OPEN_ALREAD_APPLAY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_APPLAY_TIME
	};
}

namespace ERR_OPEN_GVG_FORMATION 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_HEAD = 1;
	static const char* STR_NOT_HEAD = "只有掌门才能进行此操作";

	static const unsigned int ID_TIME_NOT_MATCH = 2;
	static const char* STR_TIME_NOT_MATCH = "现在还不能对阵型进行调控";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_LEVEL_NOT_ENOUGH = "门派等级未达到报名要求";

	static const unsigned int ID_RANK_NOT_ENOUGH = 4;
	static const char* STR_RANK_NOT_ENOUGH = "门派排名未达到报名要求";

	static const unsigned int ID_APPLAY_FIRST = 5;
	static const char* STR_APPLAY_FIRST = "请先报名";

	static const char* STR_NAMESPACE = "ERR_OPEN_GVG_FORMATION";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_HEAD
	,STR_TIME_NOT_MATCH
	,STR_LEVEL_NOT_ENOUGH
	,STR_RANK_NOT_ENOUGH
	,STR_APPLAY_FIRST
	};
}

namespace ERR_GVG_APPLAY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_GVG_APPLAY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_GVG_MONEY_CHAOS 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_GVG_MONEY_CHAOS";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_OPEN_GVG_MONEY_CHAOS 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_GVG_MONEY_CHAOS";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_OPEN_16_TO_8_TO_4 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_16_TO_8_TO_4";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_OPEN_FINAL_WAR 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_FINAL_WAR";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_GVG_LAST_RESULT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_GVG_LAST_RESULT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_OPEN_GVG_LOG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const unsigned int ID_NOT_JOIN_FACTION = 2;
	static const char* STR_NOT_JOIN_FACTION = "你还未加入门派";

	static const unsigned int ID_NOT_FIND_FACTION = 3;
	static const char* STR_NOT_FIND_FACTION = "该门派的战况已过期，无法观看";

	static const char* STR_NAMESPACE = "ERR_OPEN_GVG_LOG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_JOIN_FACTION
	,STR_NOT_FIND_FACTION
	};
}

namespace ERR_SEE_GVG_VIDEO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FIND_VIDEO = 1;
	static const char* STR_NOT_FIND_VIDEO = "该场门派战斗已经过期，无法观看";

	static const unsigned int ID_NOT_START = 2;
	static const char* STR_NOT_START = "战斗还未开始，请耐心等待";

	static const unsigned int ID_REACH_END = 3;
	static const char* STR_REACH_END = "本场玩家战斗已经是最后一场";

	static const unsigned int ID_NOT_FIND_PVP_VIDEO = 4;
	static const char* STR_NOT_FIND_PVP_VIDEO = "该场玩家战斗已过期，无法观看";

	static const unsigned int ID_IS_UNOPEN = 5;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_SEE_GVG_VIDEO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FIND_VIDEO
	,STR_NOT_START
	,STR_REACH_END
	,STR_NOT_FIND_PVP_VIDEO
	,STR_IS_UNOPEN
	};
}

namespace ERR_END_GVG_VIDEO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FIND_VIDEO = 1;
	static const char* STR_NOT_FIND_VIDEO = "该场战斗已经过期，无法观看";

	static const unsigned int ID_IS_UNOPEN = 2;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_END_GVG_VIDEO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FIND_VIDEO
	,STR_IS_UNOPEN
	};
}

namespace ERR_SHOW_GVG_MVP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FIND_VIDEO = 1;
	static const char* STR_NOT_FIND_VIDEO = "该场战斗已经过期，无法观看";

	static const unsigned int ID_IS_UNOPEN = 2;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_SHOW_GVG_MVP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FIND_VIDEO
	,STR_IS_UNOPEN
	};
}

namespace ERR_GVG_OPEN_GAMBLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_HAVE_BATTLE = 1;
	static const char* STR_NOT_HAVE_BATTLE = "没有可以押注的信息";

	static const unsigned int ID_ALREAD_BATTLE = 2;
	static const char* STR_ALREAD_BATTLE = "战斗已结束不能押注";

	static const unsigned int ID_NOT_FIND_FACTION = 3;
	static const char* STR_NOT_FIND_FACTION = "找不到门派信息";

	static const unsigned int ID_IS_UNOPEN = 4;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_GVG_OPEN_GAMBLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_HAVE_BATTLE
	,STR_ALREAD_BATTLE
	,STR_NOT_FIND_FACTION
	,STR_IS_UNOPEN
	};
}

namespace ERR_GVG_GAMBLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战功能还未开放";

	static const unsigned int ID_ALREAD_BATTLE = 2;
	static const char* STR_ALREAD_BATTLE = "战斗已结束不能押注";

	static const unsigned int ID_NOT_HAVE_BATTLE = 3;
	static const char* STR_NOT_HAVE_BATTLE = "没有可以押注的信息";

	static const unsigned int ID_ALREAD_GAMBLE = 4;
	static const char* STR_ALREAD_GAMBLE = "已押注，请等待下轮押注";

	static const unsigned int ID_GAMBLE_IS_ERR = 5;
	static const char* STR_GAMBLE_IS_ERR = "押注信息有误";

	static const char* STR_NAMESPACE = "ERR_GVG_GAMBLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_ALREAD_BATTLE
	,STR_NOT_HAVE_BATTLE
	,STR_ALREAD_GAMBLE
	,STR_GAMBLE_IS_ERR
	};
}

namespace ERR_READ_FORMATION_PLAYER_TIP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UPOPEN = 1;
	static const char* STR_IS_UPOPEN = "对方功能还未开放";

	static const unsigned int ID_NOT_JOIN_FACTION = 2;
	static const char* STR_NOT_JOIN_FACTION = "对方还未加入门派";

	static const unsigned int ID_IS_UNOPEN = 3;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_READ_FORMATION_PLAYER_TIP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UPOPEN
	,STR_NOT_JOIN_FACTION
	,STR_IS_UNOPEN
	};
}

namespace ERR_GVG_FACTION_EMBATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_HEAD = 1;
	static const char* STR_NOT_HEAD = "此操作只有掌门有权限";

	static const unsigned int ID_NOT_HAVE_FACTION = 2;
	static const char* STR_NOT_HAVE_FACTION = "还未加入门派";

	static const unsigned int ID_IS_UNOPEN = 3;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const unsigned int ID_HEAD_MUST_IN_FORMATION = 4;
	static const char* STR_HEAD_MUST_IN_FORMATION = "掌门必须在阵上";

	static const unsigned int ID_MORE_ONE = 5;
	static const char* STR_MORE_ONE = "请勾选成员上阵";

	static const char* STR_NAMESPACE = "ERR_GVG_FACTION_EMBATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_HEAD
	,STR_NOT_HAVE_FACTION
	,STR_IS_UNOPEN
	,STR_HEAD_MUST_IN_FORMATION
	,STR_MORE_ONE
	};
}

namespace ERR_OPEN_GVG_GIFT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_GVG_GIFT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_GET_GVG_GIFT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const unsigned int ID_NOT_EXIST = 2;
	static const char* STR_NOT_EXIST = "该奖励礼包不存在";

	static const unsigned int ID_ALREAD_GET = 3;
	static const char* STR_ALREAD_GET = "该奖励礼包已领取";

	static const unsigned int ID_BAG_FULL = 4;
	static const char* STR_BAG_FULL = "请先整理背包现领取奖励";

	static const char* STR_NAMESPACE = "ERR_GET_GVG_GIFT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_EXIST
	,STR_ALREAD_GET
	,STR_BAG_FULL
	};
}

namespace ERR_16_TO_8_TO_4 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_16_TO_8_TO_4";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_FINAL_WAR 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_FINAL_WAR";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_LAST_RESULT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_LAST_RESULT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_SELECT_GROUP_TYPE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const char* STR_NAMESPACE = "ERR_SELECT_GROUP_TYPE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_GVG_FACTION_APPLAY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "跨服战系统还未开放";

	static const unsigned int ID_NOT_HEAD = 2;
	static const char* STR_NOT_HEAD = "只有掌门才能进行此操作";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_LEVEL_NOT_ENOUGH = "门派等级未达到报名要求";

	static const unsigned int ID_RANK_NOT_ENOUGH = 4;
	static const char* STR_RANK_NOT_ENOUGH = "门派排名未达到报名要求";

	static const unsigned int ID_ALREAD_APPLAY = 5;
	static const char* STR_ALREAD_APPLAY = "您的门派已报名";

	static const unsigned int ID_NOT_APPLAY_TIME = 6;
	static const char* STR_NOT_APPLAY_TIME = "当前不处于报名阶段，请等待";

	static const char* STR_NAMESPACE = "ERR_GVG_FACTION_APPLAY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_HEAD
	,STR_LEVEL_NOT_ENOUGH
	,STR_RANK_NOT_ENOUGH
	,STR_ALREAD_APPLAY
	,STR_NOT_APPLAY_TIME
	};
}

namespace ERR_READ_BATTLE_LOG 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const char* STR_NAMESPACE = "ERR_READ_BATTLE_LOG";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	};
}

namespace ERR_OPEN_DEVOUR_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NOT_FIND_SOUL = 2;
	static const char* STR_NOT_FIND_SOUL = "找不到战魂卡";

	static const char* STR_NAMESPACE = "ERR_OPEN_DEVOUR_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_FIND_SOUL
	};
}

namespace ERR_DEVOUR_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NOT_FIND_SOUL = 2;
	static const char* STR_NOT_FIND_SOUL = "找不到战魂卡";

	static const unsigned int ID_IS_SAME = 3;
	static const char* STR_IS_SAME = "战魂卡异常";

	static const unsigned int ID_DEVOUR_SOUL_NO_DEVOUR = 4;
	static const char* STR_DEVOUR_SOUL_NO_DEVOUR = "吞噬战魂只能被吞噬";

	static const unsigned int ID_IS_FULL = 5;
	static const char* STR_IS_FULL = "已满级，无法吞噬";

	static const char* STR_NAMESPACE = "ERR_DEVOUR_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_FIND_SOUL
	,STR_IS_SAME
	,STR_DEVOUR_SOUL_NO_DEVOUR
	,STR_IS_FULL
	};
}

namespace ERR_OPEN_REFLASH_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NOT_FIND_SOUL = 2;
	static const char* STR_NOT_FIND_SOUL = "找不到战魂卡";

	static const char* STR_NAMESPACE = "ERR_OPEN_REFLASH_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_FIND_SOUL
	};
}

namespace ERR_REFLASH_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_DEVOUR_SOUL_NO_REFLASH = 2;
	static const char* STR_DEVOUR_SOUL_NO_REFLASH = "吞噬战魂不能洗炼";

	static const unsigned int ID_NO_ENOUGHT_GOLD = 3;
	static const char* STR_NO_ENOUGHT_GOLD = "没有足够的元宝吞噬战魂";

	static const unsigned int ID_NOT_FIND_SOUL = 4;
	static const char* STR_NOT_FIND_SOUL = "找不到战魂卡";

	static const char* STR_NAMESPACE = "ERR_REFLASH_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_DEVOUR_SOUL_NO_REFLASH
	,STR_NO_ENOUGHT_GOLD
	,STR_NOT_FIND_SOUL
	};
}

namespace ERR_SAVE_REFLASH_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NOT_FIND_SOUL = 2;
	static const char* STR_NOT_FIND_SOUL = "找不到战魂卡";

	static const char* STR_NAMESPACE = "ERR_SAVE_REFLASH_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_FIND_SOUL
	};
}

namespace ERR_SOULREP_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_ALREADY_IN_SOULREP = 2;
	static const char* STR_ALREADY_IN_SOULREP = "已经在将魂副本了";

	static const char* STR_NAMESPACE = "ERR_SOULREP_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_ALREADY_IN_SOULREP
	};
}

namespace ERR_PRODUCT_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NO_ENOUGHT_COIN = 2;
	static const char* STR_NO_ENOUGHT_COIN = "没有足够的铜钱";

	static const unsigned int ID_NO_ENOUGHT_GOLD = 3;
	static const char* STR_NO_ENOUGHT_GOLD = "没有足够的元宝";

	static const unsigned int ID_SOUL_BAG_FULL = 4;
	static const char* STR_SOUL_BAG_FULL = "将魂背包已满";

	static const unsigned int ID_ITEM_BAG_FULL = 5;
	static const char* STR_ITEM_BAG_FULL = "材料背包已满";

	static const unsigned int ID_NO_IN_ACTIVITY = 6;
	static const char* STR_NO_IN_ACTIVITY = "不是在战魂活动中";

	static const unsigned int ID_UNKNOWN_ERROR = 7;
	static const char* STR_UNKNOWN_ERROR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_PRODUCT_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NO_ENOUGHT_COIN
	,STR_NO_ENOUGHT_GOLD
	,STR_SOUL_BAG_FULL
	,STR_ITEM_BAG_FULL
	,STR_NO_IN_ACTIVITY
	,STR_UNKNOWN_ERROR
	};
}

namespace ERR_HERO_SOUL_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NO_HERO = 2;
	static const char* STR_NO_HERO = "玩家身上没有这只英雄";

	static const char* STR_NAMESPACE = "ERR_HERO_SOUL_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NO_HERO
	};
}

namespace ERR_HERO_EQUIP_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NO_HERO = 2;
	static const char* STR_NO_HERO = "玩家身上没有这只英雄";

	static const unsigned int ID_HERO_ALL_EQUIP = 3;
	static const char* STR_HERO_ALL_EQUIP = "英雄身上都装上战魂了";

	static const unsigned int ID_NO_THIS_SOUL = 4;
	static const char* STR_NO_THIS_SOUL = "玩家身上没有这个战魂";

	static const unsigned int ID_DEVOUR_SOUL_NO_EQUIP = 5;
	static const char* STR_DEVOUR_SOUL_NO_EQUIP = "吞噬战魂不能穿戴";

	static const unsigned int ID_UNKNOWN_ERROR = 6;
	static const char* STR_UNKNOWN_ERROR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_HERO_EQUIP_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NO_HERO
	,STR_HERO_ALL_EQUIP
	,STR_NO_THIS_SOUL
	,STR_DEVOUR_SOUL_NO_EQUIP
	,STR_UNKNOWN_ERROR
	};
}

namespace ERR_HERO_UNDRESS_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NO_HERO = 2;
	static const char* STR_NO_HERO = "玩家身上没有这只英雄";

	static const unsigned int ID_SOUL_BAG_FULL = 3;
	static const char* STR_SOUL_BAG_FULL = "将魂背包已满";

	static const unsigned int ID_NO_THIS_SOUL = 4;
	static const char* STR_NO_THIS_SOUL = "英雄身上没有这个战魂";

	static const unsigned int ID_UNKNOWN_ERROR = 5;
	static const char* STR_UNKNOWN_ERROR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_HERO_UNDRESS_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NO_HERO
	,STR_SOUL_BAG_FULL
	,STR_NO_THIS_SOUL
	,STR_UNKNOWN_ERROR
	};
}

namespace ERR_HERO_REPLACE_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NO_HERO = 2;
	static const char* STR_NO_HERO = "玩家身上没有这只英雄";

	static const unsigned int ID_NO_THIS_SOUL = 3;
	static const char* STR_NO_THIS_SOUL = "英雄身上没有这个战魂";

	static const unsigned int ID_DEVOUR_SOUL_NO_REPLACE = 4;
	static const char* STR_DEVOUR_SOUL_NO_REPLACE = "吞噬战魂不能兑换";

	static const unsigned int ID_UNKNOWN_ERROR = 5;
	static const char* STR_UNKNOWN_ERROR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_HERO_REPLACE_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NO_HERO
	,STR_NO_THIS_SOUL
	,STR_DEVOUR_SOUL_NO_REPLACE
	,STR_UNKNOWN_ERROR
	};
}

namespace ERR_SOULEXCHANGE_INFO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_UNKNOWN_ERROR = 2;
	static const char* STR_UNKNOWN_ERROR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_SOULEXCHANGE_INFO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERROR
	};
}

namespace ERR_EXCHANGE_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_NO_ENOUGHT_METERIAL = 2;
	static const char* STR_NO_ENOUGHT_METERIAL = "战魂材料不够";

	static const unsigned int ID_SOUL_BAG_FULL = 3;
	static const char* STR_SOUL_BAG_FULL = "战魂背包已满，不能兑换";

	static const unsigned int ID_UNKNOWN_ERROR = 4;
	static const char* STR_UNKNOWN_ERROR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_EXCHANGE_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NO_ENOUGHT_METERIAL
	,STR_SOUL_BAG_FULL
	,STR_UNKNOWN_ERROR
	};
}

namespace ERR_CLIGS_PLAYER_INFO_SOUL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "战魂系统未开放";

	static const unsigned int ID_COACH_HERO_LOST = 2;
	static const char* STR_COACH_HERO_LOST = "主角信息丢失";

	static const char* STR_NAMESPACE = "ERR_CLIGS_PLAYER_INFO_SOUL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_COACH_HERO_LOST
	};
}

namespace ERR_OPEN_MY_MEDAL 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "勋章系统未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_MY_MEDAL";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_VIEW_FACTION_MEMBER 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FOUND_FACTION = 1;
	static const char* STR_NOT_FOUND_FACTION = "找不到该门派";

	static const char* STR_NAMESPACE = "ERR_VIEW_FACTION_MEMBER";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FOUND_FACTION
	};
}

namespace ERR_OPEN_OUT_BOUND 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FRESH = 1;
	static const char* STR_NOT_FRESH = "请先转生";

	static const char* STR_NAMESPACE = "ERR_OPEN_OUT_BOUND";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FRESH
	};
}

namespace ERR_OPEN_OUT_BOUND_MAP 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FRESH = 1;
	static const char* STR_NOT_FRESH = "请先转生";

	static const unsigned int ID_NOT_OPEN = 2;
	static const char* STR_NOT_OPEN = "此外传还未开启";

	static const unsigned int ID_NOT_EXIST = 3;
	static const char* STR_NOT_EXIST = "此外传不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_OUT_BOUND_MAP";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FRESH
	,STR_NOT_OPEN
	,STR_NOT_EXIST
	};
}

namespace ERR_OUT_BOUND_BATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_FRESH = 1;
	static const char* STR_NOT_FRESH = "请先转生";

	static const unsigned int ID_NOT_OPEN = 2;
	static const char* STR_NOT_OPEN = "请先通关上一个副本";

	static const unsigned int ID_NOT_EXIST = 3;
	static const char* STR_NOT_EXIST = "此外传不存在";

	static const unsigned int ID_NOT_PHYSICAL = 4;
	static const char* STR_NOT_PHYSICAL = "体力不足，休息一下";

	static const unsigned int ID_BAG_IS_FULL = 5;
	static const char* STR_BAG_IS_FULL = "背包空间不足，请先整理下";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 6;
	static const char* STR_LEVEL_NOT_ENOUGH = "当前等级无法挑战该副本，请先升级";

	static const unsigned int ID_CONFIG_ERR = 7;
	static const char* STR_CONFIG_ERR = "战斗错误，请联系GM";

	static const unsigned int ID_HOOK_ERR = 8;
	static const char* STR_HOOK_ERR = "未通关过，无法跳过战斗";

	static const unsigned int ID_NOT_BATTLE_NUM = 9;
	static const char* STR_NOT_BATTLE_NUM = "今日挑战次数已用光，请明日再来挑战";

	static const char* STR_NAMESPACE = "ERR_OUT_BOUND_BATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_FRESH
	,STR_NOT_OPEN
	,STR_NOT_EXIST
	,STR_NOT_PHYSICAL
	,STR_BAG_IS_FULL
	,STR_LEVEL_NOT_ENOUGH
	,STR_CONFIG_ERR
	,STR_HOOK_ERR
	,STR_NOT_BATTLE_NUM
	};
}

namespace ERR_CLIGS_PETREP_INFO_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_CLIGS_PETREP_INFO_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	};
}

namespace ERR_CLIGS_PRODUCT_PET_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_ENOUGHT_GOLD = 3;
	static const char* STR_NO_ENOUGHT_GOLD = "元宝不足";

	static const unsigned int ID_NO_ENOUGHT_BAG_SIZE = 4;
	static const char* STR_NO_ENOUGHT_BAG_SIZE = "背包空间不足";

	static const unsigned int ID_NO_ENOUGHT_PETBAG_SIZE = 5;
	static const char* STR_NO_ENOUGHT_PETBAG_SIZE = "灵宠卡片的背包空间不足";

	static const char* STR_NAMESPACE = "ERR_CLIGS_PRODUCT_PET_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_ENOUGHT_GOLD
	,STR_NO_ENOUGHT_BAG_SIZE
	,STR_NO_ENOUGHT_PETBAG_SIZE
	};
}

namespace ERR_CLIGS_OPEN_BORNPET_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_BORNPET_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	};
}

namespace ERR_CLIGS_BORNPET_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_CLIGS_BORNPET_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	};
}

namespace ERR_CLIGS_OPEN_STAR_PANEL_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_STAR_PANEL_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	};
}

namespace ERR_CLIGS_UP_STAR_LV_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_PET_SATR_FULL = 3;
	static const char* STR_PET_SATR_FULL = "灵宠星阶已达到当前最高";

	static const unsigned int ID_DRAGON_ITEM_NO_ENOUGHT = 4;
	static const char* STR_DRAGON_ITEM_NO_ENOUGHT = "未集齐足够数量的龙珠";

	static const unsigned int ID_NO_THIS_PET = 5;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const unsigned int ID_PET_LEVEL_LOW = 6;
	static const char* STR_PET_LEVEL_LOW = "灵宠等级不足无法升星";

	static const unsigned int ID_PLAYER_LEVEL_LOW = 7;
	static const char* STR_PLAYER_LEVEL_LOW = "玩家等级未达到要求，灵宠无法升星";

	static const char* STR_NAMESPACE = "ERR_CLIGS_UP_STAR_LV_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_PET_SATR_FULL
	,STR_DRAGON_ITEM_NO_ENOUGHT
	,STR_NO_THIS_PET
	,STR_PET_LEVEL_LOW
	,STR_PLAYER_LEVEL_LOW
	};
}

namespace ERR_CLIGS_OPEN_PET_PANEL_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_PET_PANEL_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	};
}

namespace ERR_CLIGS_OPEN_PETFEED_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_PETFEED_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	};
}

namespace ERR_CLIGS_FIRST_DRAW_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const unsigned int ID_NO_TODAY_FIRST_DRAW = 4;
	static const char* STR_NO_TODAY_FIRST_DRAW = "今日喂食次数已用光";

	static const unsigned int ID_EAT_CUR_FRUIT = 5;
	static const char* STR_EAT_CUR_FRUIT = "请先喂食完当前食物再进行拉杆操作";

	static const unsigned int ID_VIPFEED_NO_ENOUGHT_GOLD = 6;
	static const char* STR_VIPFEED_NO_ENOUGHT_GOLD = "VIP喂养元宝不足";

	static const char* STR_NAMESPACE = "ERR_CLIGS_FIRST_DRAW_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	,STR_NO_TODAY_FIRST_DRAW
	,STR_EAT_CUR_FRUIT
	,STR_VIPFEED_NO_ENOUGHT_GOLD
	};
}

namespace ERR_CLIGS_RESET_DRAW_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const unsigned int ID_NOT_ENOUGHT_GOLD = 4;
	static const char* STR_NOT_ENOUGHT_GOLD = "没有足够的元宝";

	static const unsigned int ID_IS_BEST_RESULT = 5;
	static const char* STR_IS_BEST_RESULT = "5个人参果是最高档次的组合";

	static const unsigned int ID_NO_FIRST_DRAW = 6;
	static const char* STR_NO_FIRST_DRAW = "尚未将拉杆压下，无法进行重摇";

	static const char* STR_NAMESPACE = "ERR_CLIGS_RESET_DRAW_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	,STR_NOT_ENOUGHT_GOLD
	,STR_IS_BEST_RESULT
	,STR_NO_FIRST_DRAW
	};
}

namespace ERR_CLIGS_CONFIRM_FEED_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const char* STR_NAMESPACE = "ERR_CLIGS_CONFIRM_FEED_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	};
}

namespace ERR_CLIGS_MERGE_PET_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const char* STR_NAMESPACE = "ERR_CLIGS_MERGE_PET_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	};
}

namespace ERR_CLIGS_OPEN_PET_STORE_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_PET_STORE_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	};
}

namespace ERR_CLIGS_BUY_PET_STORE_ITEM_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_ENOUGHT_PETSOTRE = 3;
	static const char* STR_NO_ENOUGHT_PETSOTRE = "积分不足";

	static const unsigned int ID_NO_ENOUGHT_GOLD = 4;
	static const char* STR_NO_ENOUGHT_GOLD = "元宝不足";

	static const unsigned int ID_ON_ENOUGHT_BAG_SIZE = 5;
	static const char* STR_ON_ENOUGHT_BAG_SIZE = "背包空间不足";

	static const unsigned int ID_Cant_BUY = 6;
	static const char* STR_Cant_BUY = "该物品不可兑换";

	static const char* STR_NAMESPACE = "ERR_CLIGS_BUY_PET_STORE_ITEM_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_ENOUGHT_PETSOTRE
	,STR_NO_ENOUGHT_GOLD
	,STR_ON_ENOUGHT_BAG_SIZE
	,STR_Cant_BUY
	};
}

namespace ERR_CLIGS_OPEN_REFINEPET_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_REFINEPET_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	};
}

namespace ERR_CLIGS_REFINEPET_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const char* STR_NAMESPACE = "ERR_CLIGS_REFINEPET_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	};
}

namespace ERR_CLIGS_OPEN_WASH_QUALITY_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_WASH_QUALITY_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	};
}

namespace ERR_CLIGS_WASH_QUALITY_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const unsigned int ID_PSYCHIC_FULL = 4;
	static const char* STR_PSYCHIC_FULL = "该灵脉资质已满无需洗炼";

	static const unsigned int ID_NO_ENOUGHT_GOLD = 5;
	static const char* STR_NO_ENOUGHT_GOLD = "元宝不足";

	static const unsigned int ID_NO_ENOUGHT_WASHITEM = 6;
	static const char* STR_NO_ENOUGHT_WASHITEM = "缺少洗炼灵脉的道具";

	static const unsigned int ID_CANT_WASH = 7;
	static const char* STR_CANT_WASH = "该灵脉尚未解锁";

	static const char* STR_NAMESPACE = "ERR_CLIGS_WASH_QUALITY_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	,STR_PSYCHIC_FULL
	,STR_NO_ENOUGHT_GOLD
	,STR_NO_ENOUGHT_WASHITEM
	,STR_CANT_WASH
	};
}

namespace ERR_CLIGS_CHANGE_PET_STATE_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const unsigned int ID_OVER_MAX_FOLLOW = 4;
	static const char* STR_OVER_MAX_FOLLOW = "超过唤出灵宠数量上限";

	static const unsigned int ID_OVER_MAX_FIGHT = 5;
	static const char* STR_OVER_MAX_FIGHT = "超过出战灵宠数量上限";

	static const char* STR_NAMESPACE = "ERR_CLIGS_CHANGE_PET_STATE_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	,STR_OVER_MAX_FOLLOW
	,STR_OVER_MAX_FIGHT
	};
}

namespace ERR_CLIGS_OPEN_PET_INHERIT_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const unsigned int ID_NO_INHERIT_PET = 4;
	static const char* STR_NO_INHERIT_PET = "没有可被继承的灵宠";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_PET_INHERIT_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	,STR_NO_INHERIT_PET
	};
}

namespace ERR_CLIGS_PET_INHERIT_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_THIS_PET = 3;
	static const char* STR_NO_THIS_PET = "没有这只灵宠";

	static const unsigned int ID_NO_ENOUGHT_GOLD = 4;
	static const char* STR_NO_ENOUGHT_GOLD = "元宝不足";

	static const unsigned int ID_NO_ENOUGHT_COIN = 5;
	static const char* STR_NO_ENOUGHT_COIN = "铜钱不足";

	static const char* STR_NAMESPACE = "ERR_CLIGS_PET_INHERIT_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_THIS_PET
	,STR_NO_ENOUGHT_GOLD
	,STR_NO_ENOUGHT_COIN
	};
}

namespace ERR_CLIGS_OPEN_PLAYERPET_PANEL_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "灵宠系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const unsigned int ID_NO_FIGHT_PET = 3;
	static const char* STR_NO_FIGHT_PET = "没有灵宠处于出战状态";

	static const char* STR_NAMESPACE = "ERR_CLIGS_OPEN_PLAYERPET_PANEL_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	,STR_NO_FIGHT_PET
	};
}

namespace ERR_CLIGS_REQUEST_ALL_PET_INFO_REQ 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "宠物系统未开放";

	static const unsigned int ID_UNKNOWN_ERR = 2;
	static const char* STR_UNKNOWN_ERR = "未知错误";

	static const char* STR_NAMESPACE = "ERR_CLIGS_REQUEST_ALL_PET_INFO_REQ";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_UNKNOWN_ERR
	};
}

namespace ERR_OPEN_STORE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_STORE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	};
}

namespace ERR_EXCHANGE_GOODS 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const unsigned int ID_NOT_ITEM_ID = 2;
	static const char* STR_NOT_ITEM_ID = "找不到此物品";

	static const unsigned int ID_RES_NOT_ENOUGH = 3;
	static const char* STR_RES_NOT_ENOUGH = "当前资源不足以购买此道具";

	static const unsigned int ID_NUM_NOT_ENOUGH = 4;
	static const char* STR_NUM_NOT_ENOUGH = "数量不能为0";

	static const unsigned int ID_BAG_IS_FULL = 5;
	static const char* STR_BAG_IS_FULL = "背包空间已满，请整理背包";

	static const char* STR_NAMESPACE = "ERR_EXCHANGE_GOODS";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	,STR_NOT_ITEM_ID
	,STR_RES_NOT_ENOUGH
	,STR_NUM_NOT_ENOUGH
	,STR_BAG_IS_FULL
	};
}

namespace ERR_ENCHANT 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "附魔功能未开放";

	static const unsigned int ID_REACH_MAX_LEVEL_ERR = 2;
	static const char* STR_REACH_MAX_LEVEL_ERR = "附魔已达到最大等级";

	static const unsigned int ID_NOT_EQUIP_ERR = 3;
	static const char* STR_NOT_EQUIP_ERR = "先穿上该装备才能附魔";

	static const unsigned int ID_NOT_EXIST_ERR = 4;
	static const char* STR_NOT_EXIST_ERR = "装备不存在";

	static const unsigned int ID_LEVEL_ERR = 5;
	static const char* STR_LEVEL_ERR = "传入等级错误，只允许升一级或十级";

	static const unsigned int ID_CANNOT_GREATER_THAN_PLAYER_LV = 6;
	static const char* STR_CANNOT_GREATER_THAN_PLAYER_LV = "玩家等级不足（飞升后每提升一级可获得一次附魔机会）";

	static const unsigned int ID_ENCHANT_STONE_NOT_ENOUGH = 7;
	static const char* STR_ENCHANT_STONE_NOT_ENOUGH = "附魔石不足";

	static const unsigned int ID_NOT_FLY_EQUIP = 8;
	static const char* STR_NOT_FLY_EQUIP = "只有外传装备才能附魔";

	static const unsigned int ID_NOT_FLY = 9;
	static const char* STR_NOT_FLY = "飞升之后才能附魔";

	static const unsigned int ID_STRENGTHEN_LV_NOT_ENOUGH = 10;
	static const char* STR_STRENGTHEN_LV_NOT_ENOUGH = "装备强化等级不足，请先强化再附魔";

	static const char* STR_NAMESPACE = "ERR_ENCHANT";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_REACH_MAX_LEVEL_ERR
	,STR_NOT_EQUIP_ERR
	,STR_NOT_EXIST_ERR
	,STR_LEVEL_ERR
	,STR_CANNOT_GREATER_THAN_PLAYER_LV
	,STR_ENCHANT_STONE_NOT_ENOUGH
	,STR_NOT_FLY_EQUIP
	,STR_NOT_FLY
	,STR_STRENGTHEN_LV_NOT_ENOUGH
	};
}

namespace ERR_EQUIP_MAKE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "外传装备制作功能未开放";

	static const unsigned int ID_MATERIAL_NOT_ENOUGH = 2;
	static const char* STR_MATERIAL_NOT_ENOUGH = "材料不足，无法制作";

	static const unsigned int ID_EQUIP_NOT_ENOUGH = 3;
	static const char* STR_EQUIP_NOT_ENOUGH = "缺少所需装备，无法制作";

	static const unsigned int ID_BOOK_NOT_EXIST = 4;
	static const char* STR_BOOK_NOT_EXIST = "制作卷不存在，请重新打开本面板";

	static const unsigned int ID_VIP_LEVEL_NOT_ENOUGH = 5;
	static const char* STR_VIP_LEVEL_NOT_ENOUGH = "VIP等级不足，无法使用补齐材料功能";

	static const unsigned int ID_BAG_FULL = 6;
	static const char* STR_BAG_FULL = "背包已满无法制作，请先整理背包";

	static const char* STR_NAMESPACE = "ERR_EQUIP_MAKE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_MATERIAL_NOT_ENOUGH
	,STR_EQUIP_NOT_ENOUGH
	,STR_BOOK_NOT_EXIST
	,STR_VIP_LEVEL_NOT_ENOUGH
	,STR_BAG_FULL
	};
}

namespace ERR_OPEN_EQUIP_MAKE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "外传装备制作功能未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_EQUIP_MAKE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_UPGRADE_HERO 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "伙伴进阶功能未开放";

	static const unsigned int ID_NOT_FLY = 2;
	static const char* STR_NOT_FLY = "主角飞升之后才可进阶";

	static const unsigned int ID_NOT_UPGRADABLE = 3;
	static const char* STR_NOT_UPGRADABLE = "必须先招募该伙伴才能进阶";

	static const unsigned int ID_MATERIAL_NOT_ENOUGH = 4;
	static const char* STR_MATERIAL_NOT_ENOUGH = "材料不足，无法进阶";

	static const unsigned int ID_NOT_PASS_OUTBOUND = 5;
	static const char* STR_NOT_PASS_OUTBOUND = "该伙伴必须先通关对应的外传才可进阶";

	static const unsigned int ID_NOT_FOUND_HERO = 6;
	static const char* STR_NOT_FOUND_HERO = "找不到该伙伴，无法进阶";

	static const char* STR_NAMESPACE = "ERR_UPGRADE_HERO";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_NOT_FLY
	,STR_NOT_UPGRADABLE
	,STR_MATERIAL_NOT_ENOUGH
	,STR_NOT_PASS_OUTBOUND
	,STR_NOT_FOUND_HERO
	};
}

namespace ERR_OPEN_FLY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "飞升功能未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_FLY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	};
}

namespace ERR_FLY 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_UNOPEN = 1;
	static const char* STR_IS_UNOPEN = "飞升功能未开放";

	static const unsigned int ID_COACH_NOT_FLY = 2;
	static const char* STR_COACH_NOT_FLY = "主角飞升后才能飞升";

	static const unsigned int ID_LEVEL_ERR = 3;
	static const char* STR_LEVEL_ERR = "等级不足，无法飞升";

	static const unsigned int ID_JINGJIE_LEVEL_ERR = 4;
	static const char* STR_JINGJIE_LEVEL_ERR = "境界等级不足，无法飞升";

	static const unsigned int ID_MATERIAL_NOT_ENOUGH = 5;
	static const char* STR_MATERIAL_NOT_ENOUGH = "飞升材料不足，无法飞升";

	static const unsigned int ID_EXP_NOT_ENOUGH = 6;
	static const char* STR_EXP_NOT_ENOUGH = "经验必须是满值才可飞升";

	static const char* STR_NAMESPACE = "ERR_FLY";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_UNOPEN
	,STR_COACH_NOT_FLY
	,STR_LEVEL_ERR
	,STR_JINGJIE_LEVEL_ERR
	,STR_MATERIAL_NOT_ENOUGH
	,STR_EXP_NOT_ENOUGH
	};
}

namespace ERR_OPEN_PET_RULE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_PET_RULE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	};
}

namespace ERR_PLAYER_INFO_PET 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_NOT_OPEN = 1;
	static const char* STR_NOT_OPEN = "建筑未开放";

	static const char* STR_NAMESPACE = "ERR_PLAYER_INFO_PET";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_NOT_OPEN
	};
}

namespace ERR_OPEN_SOUL_UPDATE_TYPE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const unsigned int ID_NOT_EXIST_SOUL = 2;
	static const char* STR_NOT_EXIST_SOUL = "该战魂卡不存在";

	static const char* STR_NAMESPACE = "ERR_OPEN_SOUL_UPDATE_TYPE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	,STR_NOT_EXIST_SOUL
	};
}

namespace ERR_SOUL_UPDATE_TYPE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const unsigned int ID_NOT_EXIST_SOUL = 2;
	static const char* STR_NOT_EXIST_SOUL = "该战魂卡不存在";

	static const unsigned int ID_ITEM_NOT_ENOUGH = 3;
	static const char* STR_ITEM_NOT_ENOUGH = "升品所需材料不足";

	static const unsigned int ID_MAX_POTENCY = 4;
	static const char* STR_MAX_POTENCY = "已经提升到最高品，无法再法提升";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 5;
	static const char* STR_LEVEL_NOT_ENOUGH = "目前战魂等级不足以继续升品";

	static const char* STR_NAMESPACE = "ERR_SOUL_UPDATE_TYPE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	,STR_NOT_EXIST_SOUL
	,STR_ITEM_NOT_ENOUGH
	,STR_MAX_POTENCY
	,STR_LEVEL_NOT_ENOUGH
	};
}

namespace ERR_OPEN_NEIDAN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const unsigned int ID_NOT_FIND_PET = 2;
	static const char* STR_NOT_FIND_PET = "找不到宠物，请重新打开";

	static const char* STR_NAMESPACE = "ERR_OPEN_NEIDAN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	,STR_NOT_FIND_PET
	};
}

namespace ERR_DRESS_NEIDAN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const unsigned int ID_BAG_IS_FULL = 2;
	static const char* STR_BAG_IS_FULL = "内丹背包已满";

	static const unsigned int ID_NOT_FIND_PET = 3;
	static const char* STR_NOT_FIND_PET = "找不到宠物，请重新打开";

	static const unsigned int ID_NOT_FIND_NEIDAN = 4;
	static const char* STR_NOT_FIND_NEIDAN = "找不到内丹，请重新打开";

	static const char* STR_NAMESPACE = "ERR_DRESS_NEIDAN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	,STR_BAG_IS_FULL
	,STR_NOT_FIND_PET
	,STR_NOT_FIND_NEIDAN
	};
}

namespace ERR_UPDATA_NEIDAN 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const unsigned int ID_NOT_FIND_NEIDAN = 2;
	static const char* STR_NOT_FIND_NEIDAN = "找不到内丹，请重新打开";

	static const char* STR_NAMESPACE = "ERR_UPDATA_NEIDAN";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	,STR_NOT_FIND_NEIDAN
	};
}

namespace ERR_OPEN_NEIDAN_ALL_INSTANCE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const char* STR_NAMESPACE = "ERR_OPEN_NEIDAN_ALL_INSTANCE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	};
}

namespace ERR_NEIDAN_BATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const unsigned int ID_PREVIOUS_INSTANCE_NOT_PASS = 2;
	static const char* STR_PREVIOUS_INSTANCE_NOT_PASS = "请先通关前面的副本";

	static const unsigned int ID_LEVEL_NOT_ENOUGH = 3;
	static const char* STR_LEVEL_NOT_ENOUGH = "等级不足于挑战该副本";

	static const unsigned int ID_NOT_TIME_BATTLE_NUM = 4;
	static const char* STR_NOT_TIME_BATTLE_NUM = "今天已无挑战次数，请明天再试";

	static const unsigned int ID_NOT_MONSTER_CONFIG = 5;
	static const char* STR_NOT_MONSTER_CONFIG = "找不到怪物";

	static const char* STR_NAMESPACE = "ERR_NEIDAN_BATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	,STR_PREVIOUS_INSTANCE_NOT_PASS
	,STR_LEVEL_NOT_ENOUGH
	,STR_NOT_TIME_BATTLE_NUM
	,STR_NOT_MONSTER_CONFIG
	};
}

namespace ERR_RECV_NEIDAN_AWARD 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const char* STR_NAMESPACE = "ERR_RECV_NEIDAN_AWARD";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	};
}

namespace ERR_SKIP_NEIDAN_BATTLE 
{
	static const unsigned int ID_SUCCESS = 0;
	static const char* STR_SUCCESS = "操作成功";

	static const unsigned int ID_IS_NOT_OPEN = 1;
	static const char* STR_IS_NOT_OPEN = "建筑未开放";

	static const char* STR_NAMESPACE = "ERR_SKIP_NEIDAN_BATTLE";
	static void func(){ const char * psz = STR_NAMESPACE; psz = psz;}

	static const char* ErrMsg[] = {
	STR_SUCCESS
	,STR_IS_NOT_OPEN
	};
}

#endif
