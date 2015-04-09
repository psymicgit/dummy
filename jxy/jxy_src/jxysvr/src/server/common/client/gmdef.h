#ifndef _GMDEF_H_
#define _GMDEF_H_

//GM功能
enum EGMType
{
    EGMT_ADDCOIN = 1,	//铜钱
    EGMT_ADDGOLD = 2,	//元宝
    EGMT_ADDSTORY = 3,	//阅历
    EGMT_ADDSCIENCE = 4,//器魂
    EGMT_ADDPHYSTRENGTH = 5,//体力
    EGMT_ADDBUILE = 6,//建筑
    EGMT_ADDITEM = 7,//物品
    EGMT_RELOADCFG	=	8,//重配置
    EGMT_SETLEVEL	=	9,//等级
    EGMT_PASSALLCOMMINSTANCE	=	10,//副本
    EGMT_PASSALLGUIDE = 11,//引导
    EGMT_ADDBLUE = 12,//蓝气
    EGMT_ADDPURPLE = 13,//紫气
    EGMT_SETVIPLEVEL = 14, //VIP
    EGMT_ADDSTUDY = 15, //卦象
    EGMT_ADDSTUDYEXP = 16, //卦象经验
    EGMT_ADDPAYGOLD = 17, //金额
    EGMT_DEC_CLIP = 18,//扣除卦像碎片
    EGMT_ALL_STORY = 19,//全体增加阅历
    EGMT_ALL_SCIENCE = 20,//全体增加器魂
    EGMT_ALL_PHYSTRENGTH = 21,//全体增加体力
    EGMT_ALL_ITEM = 22,//全体增加物品
    EGMT_ADDJINGJIE = 23, //境界
    EGMT_DEC_ITEM = 24,//删除物品,测试用，删除宝石
    EGMT_SET_JINGJIE_LEVEL = 25, //设置境界等级
    EGMT_SET_JINGJIE = 26, //境界值
    EGMT_GET_PLAYER_INFO = 27, //获取玩家信息
    EGMT_INSTANCE_BATTLE = 28, //副本战斗
    EGMT_PLAYER_OL_NUM = 29, //在线人数
    EGMT_SILENCE_CHAT = 30, //屏蔽发言
    EGMT_WORD_FILTER = 31, //关键字
    EGMT_GET_CHAT_MSG = 32, //获取聊天记录
    EGMT_DECCOIN = 33,	//扣除铜钱
    EGMT_DECGOLD = 34,	//扣除元宝
    EGMT_DECRECHARGERMB = 35,	//扣除充值金额，同时修改vip等级/充值记录/元宝
    EGMT_GET_SLIENCE_LST = 36, //获取禁言列表
    EGMT_GET_PLAYER_RES = 37, //获取玩家资源
    EGMT_GET_ACTIVITY_ENCOURAGE = 38, //获取活动奖励操作接口，
    EGMT_RESET_PWD = 39, //重置玩家密码
    EGMT_FORBIB_LOGIN = 40, //禁止玩家登录
    EGMT_FORBID_REGISTERED = 41, //禁止玩家注册
    EGMT_SETACCOUNTTYPE = 42, //设置账号类型
    EGMT_SETGMINFO = 43, //设置玩家为GM账号
    EGMT_SYSCHATDATA = 44, //设置系统聊天公告
    EGMT_KICKOUT = 45, //踢人下线
    EGMT_ACTIVITY_ENCOURAGE = 46, //活动奖励操作接口，包括（新增/删除活动）
    EGMT_CKITEMNUM = 47, //检查道具个数
    EGMT_GETGIVERES = 48, //扣除资源及给资源
    EGMT_STARTSERVER = 49, //设置开服时间
    EGMT_OPRATINGACTIVITYBATTLE = 50, //运营活动战斗
    EGMT_GETPLAYERLEVEL = 51, //获得玩家等级
    EGMT_HTTPDECGOLD = 52,	//扣除元宝，元宝不足返回错误码
    EGMT_GETCONSUME = 53,	//查找消费记录
    EGMT_ADD_MEDAL = 54, // 增加勇气勋章
	EGMT_ADD_FACTION_FUNDS = 55,//增加门派资金
	EGMT_GET_ONLINE_PLAYER_INFO = 56,//获得所有在线玩家的PLAYER_ID,NAME,LEVEL
	EGMT_ADD_DOORSTRIBUTE = 57,//增加门派门贡
	EGMT_SET_FACTION_LEVEL = 58,//设置门派等级
	EGMT_GET_CLIVER = 59,//获得当前玩家版本号
	EGMT_RETREATE = 60,//玩家回档接口
	EGMT_RESET_FACTION_NAME = 61,//重置帮派名称
    EGMT_ADD_COURAGE_TIMES = 62, //增加勇气试炼次数
    EGMT_GET_RANK_BYLVEX = 63,//获得玩家等级排名
    EGMT_GET_VIP_LEVEL = 64,//获得玩家VIP等级
    EGMT_GET_FRIENDS_NUM = 65,//获得玩家好友数量
	EGMT_IS_JOIN_IN_FACTION = 66,//是否加入门派
	EGMT_QUERY_SUM_DOORSTRIBUTE = 67,//查询累积门贡
    EGMT_ADD_HERO = 68,//给指定玩家添加伙伴
    EGMT_CHANGE_NOTICE = 69,//更改公告内容
	EGMT_GET_POWER_RANK = 70, // 获取战斗力排行
	EGMT_ADD_JEWELRY = 71,//添加指定属性饰品
	EGMT_CHAGNE_NAME = 72,//更换名字
	EGMT_ADD_GIFT = 73,//添加礼包
    EGMT_OPERATING_ACTIVITY = 74, //活动控制
    EGMT_SHOW_GVG_PAIR = 75, // 显示跨服战16进8选手的对决名单
	EGMT_SET_GVG_PAIR = 76, // 调整跨服战16进8选手的对决名单

    EGMT_MYSELF = 200, // 我自己的gm指令
    EGMT_RESET_EQUIP_MASTER, // 重置装备精通
    EGMT_ACTIVITY_CHAT, // 添加活动公告
    EGMT_SYS_CHAT,      // 添加系统公告
    EGMT_WORLD_CHAT,      // 添加聊天
    EGMT_RECORD_CHART,      // 消耗统计
    EGMT_SEND_FACTION_ACTIVITY_CHAT, // 发送门派活动公告
    EGMT_APPLE_PAY_CHECK,      // 苹果充值验证
    EGMT_TONGBUTUI_PAY_CHECK1, // 同步推验证第一步
    EGMT_TONGBUTUI_PAY_CHECK2, // 同步推验证第二步
    EGMT_ROBOT_FACTION, // 创建机器人帮派
    EGMT_ADD_CHAT_WITH_FACTION, // 门派内的人聊天，用于测试显示门派图标
    EGMT_GVG_NEXT_ROUND, // 跨服战
    EGMT_GVG_STOP, // 跨服战

	EGMT_MAX,
};

#define ADD_COIN_DESC "增加铜钱"
#define ADD_GOLD_DESC "增加元宝"
#define ADD_STORY_DESC "增加阅历"
#define ADD_SCIENCE_DESC "增加器魂"
#define ADD_PHYSTRENGTH_DESC "增加体力"
#define ADD_BUILD_DESC "增加建筑"
#define ADD_ITEM_DESC "增加物品"
#define RELOAD_CFG_DESC "重新配置"
#define SET_LEVEL_DESC "设置等级"
#define PASS_ALL_COMMINSTANCE_DESC "通关副本"
#define PASS_ALL_GUIDE_DESC "通关引导"
#define ADD_BLUE_DESC "添加蓝气"
#define ADD_PURPLE_DESC "添加紫气"
#define SET_VIP_LEVEL_DESC "设置VIP等级"
#define ADD_STUDY_DESC "增加卦象"
#define ADD_STUDY_EXP_DESC "增加卦象经验"
#define ADD_PAY_GOLD_DESC "增加充值元宝"
#define ALL_GOLD_DESC "全体增加元宝"
#define ALL_STORY_DESC "全体增加阅历"
#define ALL_SCIENCE_DESC "全体增加器魂"
#define ALL_PHYSTRENGTH_DESC "全体增加体力"
#define ALL_ITEM_DESC  "全体增加物品"
#define ADD_JING_JIE_DESC "增加境界"
#define DEC_ITEM_DESC "删除物品"
#define SET_JINGJIE_LEVEL_DESC "设置境界等级"
#define SET_JINGJIE_DESC "设置境界值"
#define GET_PLAYER_INFO_DESC "获得玩家信息"
#define INSTANCE_BATTLE_DESC "副本战斗"
#define	PLAYER_OL_NUM_DESC "在线人数"
#define SILENCE_CHAT_DESC "屏蔽发言"
#define WORD_FILTER_DESC "关键字"
#define GET_CHAT_MSG_DESC "获取聊天记录"
#define DEC_COIN_DESC "扣除铜钱"
#define DEC_GOLD_DESC "扣除元宝"
#define DEC_RECHARG_ERMB_DESC "扣除充值金额"
#define GET_SLIENCE_LST_DESC "获取禁言列表"
#define GET_PLAYER_RES_DESC "获取玩家资源"
#define GET_ACTIVITY_ENCOURAGE_DESC "获取玩家奖励接口"
#define RESET_PWD_DESC "重置玩家密码"
#define FORBIB_LOGIN_DESC "禁止玩家登陆"
#define FORBID_REGISTERED_DESC "禁止玩家注册"
#define SET_ACCOUNT_TYPE_DESC "设置帐号类型"
#define SET_GM_INFO_DESC "设置玩家为GM号"
#define SYS_CHATDATA_DESC "设置系统聊天公告"
#define KICK_OUT_DESC "踢人下线"
#define ACTIVITY_ENCOURAGE_DESC "活动奖励操作接口"
#define CK_ITEM_NUM_DESC "检查道具个数"
#define GET_GIVE_RES_DESC "扣除以及给资源"
#define START_SERVER_DESC "设置开服时间"
#define OPRATING_ACTIVITY_BATTLE_DESC "运营活动战斗"
#define GET_PLAYER_LEVEL_DESC	"获取玩家等级"
#define HTTP_DEC_GOLD_DESC "扣除元宝"
#define GET_CONSUME_DESC "查找消费记录"
#define ADD_MEDAL_DESC "增加勇气勋章"
#define ADD_FACTION_FUNDS_DESC "增加门派资金"
#define GET_ONLINE_PLAYER_INFO_DESC "获得在线玩家的信息"
#define ADD_DOORSTRIBUTE_DESC "增加门贡"
#define SET_FACTION_LEVEL_DESC "设置门派等级"
#define GET_CLIVER_DESC "获得当前玩家版本号"
#define RETREATE_DESC "玩家回档"
#define RESET_FACTION_NAME_DESC "重置帮派名称"
#define ADD_COURAGE_TIMES_DESC "增加勇气试炼次数"
#define GET_RANK_DESC "获得等级排名前三的玩家"
#define GET_VIP_LEVEL_DESC "获得玩家vip等级"
#define GET_FRIEDS_NUM_DESC "获得玩家好友数量"
#define IS_JOIN_IN_FACTION_DESC "是否加入门派"
#define QUERY_SUM_DOORSTRIBUTE_DESC "查询累积门贡"
#define ADD_HERO_DESC "给指定玩家添加英雄"
#define CHANGE_NOTICE_DESC "更改公告内容"
#define GET_POWER_RANK_DESC "获得全服玩家战斗力排行"
#define ADD_JEWELRY_DESC "添加指定属性的饰品"
#define CHANGE_PLAYER_NAME_DESC "更改玩家名字"
#define ADD_GIFT_DESC "给玩家添加礼包"
#define OPERATING_ACTIVITY_DESC "操作活动时间"
#define SHOW_GVG_PAIR_DESC "显示跨服战16进8选手配对"
#define SET_GVG_PAIR_DESC "设置跨服战16进8选手配对"

#define PLAYER_ID_GBK "玩家ID"
#define ADD_COIN_GBK "铜钱增加数"
#define ADD_GOLD_GBK "元宝增加数"
#define ADD_STORY_GBK "阅历增加数"
#define ADD_SCIENCE_GBK "器魂增加数"
#define ADD_PHYSTRENGT_GBK "体力增加数"
#define ADD_BUILD_TYPE_GBK "添加的建筑类型"
#define ADD_BUILD_COUNT_GBK "添加的建筑数量"
#define ADD_ITEM_TYPE_GBK "添加道具的类型"
#define ADD_ITEM_COUNT_GBK "添加道具的数量"
#define SET_LEVEL_GBK "设置的等级"
#define ADD_BLUE_GBK "仙露添加数"
#define ADD_PURPLE_GBK "功力增加数"
#define SET_VIP_LEVLE_GBK "VIP等级"
#define ADD_PAY_GOLD_GBK "付费进度增加数"
#define ADD_JINGJIE_GBK "境界增加数"
#define SILENCE_CHAT_TYPE_GBK "禁言类型"
#define SILENCE_CHAT_INFO_GBK "禁言信息"
#define WORD_FILTER_TYPE_GBK "关键字类型"
#define WORD_FILTER_INFO_GBK "关键字信息"
#define DEC_COIN_GBK "扣除铜钱数"
#define DEC_GOLD_GBK "扣除元宝数"
#define DEC_RECHARGE_RMB_GBK "扣除元宝充值数"
#define PLAYER_USER_ID "玩家PLAYER_ID"
#define ADD_STUDY_GBK "卦象增加数"
#define ADD_STUDY_EXP_GBK "卦象经验增加数"
#define ALL_GOLD_GBK "全体元宝增加数"
#define ALL_STORY_GBK "全体阅历增加数"
#define ALL_SCIENCE_GBK "全体器魂增加数"
#define ALL_PHYSTRENGTH_GBK "全体体力增加数"
#define ALL_ITEM_TYPE_GBK "全体增加的道具类型"
#define ALL_ITEM_COUNT_GBK "全体增加的道具数量"
#define DEC_ITEM_TYPE_GBK "删除道具类型"
#define DEC_ITEM_COUNT_GBK "删除道具数量"
#define SET_JINGJIE_LEVEL_GBK "设置的境界等级值"
#define SET_JINGJIE_SUB_LEVEL_COUNT_GBK "设置的子境界的等级"
#define SET_JINGJIE_GBK "设置的境界值"
#define DEC_RECHARG_ERMB_GBK "扣除的充值金额"
#define RESET_PWD_GBK "重置的密码"
#define FORBID_REGISTERED_USER_ID_GBK "禁止注册的玩家ID"
#define FORBID_REGISTERED_EQU_ID_GBK "禁止注册的设备ID"
#define SET_ACCOUNT_TYPE_GBK	"玩家的帐号类型"
#define SYS_CHATDATA_GBK "聊天公告内容"
#define ACTIVITY_ENCOURAGE_GBK "活动奖励信息"
#define CK_ITEM_NUM_GBK "查询的道具ID"
#define GET_RES_GBK "获得资源的信息"
#define GIVE_RES_GBK "删除资源的信息"
#define OPRATING_ACTIVITY_BATTLE_GBK "活动ID"
#define TOWN_ID_GBK "城镇ID"
#define AREA_ID_GBK "副本ID"
#define ADD_MEDAL_GBK "增加勇气勋章的值"
#define START_TIME_GBK "开始时间"
#define END_TIME_GBK "结束时间"
#define ADD_FACTION_FUNDS_GBK "门派资金增加数"
#define SET_GM_INFO_OPTTYPE_GBK "设置GM类型"
#define SYS_CHATDATA_EXPIRESTIME_GBK "系统公告过期时间"
#define SYS_CHATDATA_TYPE_GBK "系统公告的类型"
#define HTTP_DEC_GOLD_GBK "要减去的元宝的值"
#define INSTANCE_BATTLE_SCENE_ID_GBK "场景ID"
#define INSTANCE_BATTLE_TOWN_ID_GBK "城镇ID"
#define	INSTANCE_BATTLE_INSTANCE_ID_GBK "副本ID"
#define INSTANCE_BATTLE_NUM_GBK "副本和战斗的数量"
#define RES_BUILD_TYPE_IS_OPEN_GBK "建筑类型"
#define RES_BAG_ELSE_AMOUNT_GBK "背包剩余数"
#define	RES_STUDY_BAG_ELSE_AMOUNT_GBK "卦象背包剩余数"
#define	RELOAD_CFG_PARAM_GBK "重新配置参数"
#define	START_SERVER_TIME_GBK "开服时间"
#define	ADD_STUDY_ATTR_KINT_GBK "卦像属性类型"
#define	ADD_STUDY_COLOR_KIND_GBK "卦像颜色类型"
#define ADD_STUDY_LEVEL_GBK "卦像等级"
#define ADD_DOORSTRIBUTE_COUNT_GBK "增加门贡数量"
#define SET_FACTION_LEVEL_VALUE_GBK "门派等级"
#define	SET_VIP_LEVEL_TOTAL_AMOUNT_GBK "总充值数"
#define RETREATE_TYPE_GBK "回档类型"
#define RETREATE_FILE_NAME_GBK "回档的文件名"
#define FACTION_NAME_GBK "帮派名称"
#define ADD_COURAGE_TIMES_GBK "勇气试炼次数"
#define IS_JOIN_IN_FACTION_GBK "是否加入门派"
#define POWER_GBK "战斗力"
#define POWER_RANK_GBK "战斗力排名"
#define ADD_ATTRIBUTE_1_GBK "指定属性1"
#define ADD_ATTRIBUTE_2_GBK "指定属性2"
#define ADD_ATTRIBUTE_3_GBK "指定属性3"
#define ADD_ATTRIBUTE_4_GBK "指定属性4"
#define PLAYER_NAME "玩家名称"
#define ADD_GIFT_NAME_AND_DESC_GBK "礼包名称|礼包描述"
#define ADD_GIFT_OUT_TIME_GBK "过期时间"
#define START_ZONE_ID_GBK "开始区ID"
#define END_ZONE_ID_GBK "结束区ID"
#define START_TIME_GBK "开始时间"
#define END_TIME_GBK "结束时间"

#define GM_PARAM_INT "int"
#define GM_PARAM_STR "string"
#define GM_PARAM_PLAYER_ID "playerid"
#define GM_PARAM_DATATIME "data"
#define GM_PARAM_USER_ID "userid"


#define		PLAYER_ID "playerid"
#define		START_ZONE_ID "start_zoneid"
#define		END_ZONE_ID "end_zoneid"
#define		ADD_COIN "add_coin"
#define 	ADD_GOLD  "add_gold"
#define 	ADD_STORY  "add_story"
#define 	ADD_SCIENCE  "add_science"
#define 	ADD_PHYSTRENGTH "add_phystrength"
#define 	ADD_BUILD	 "add_build"
#define 	ADD_ITEM	"add_item"
#define 	RELOAD_CFG	 "reload_cfg"
#define 	SET_LEVEL	 "set_level"
#define 	PASS_ALL_COMMINSTANCE  "pass_all_comminstance"
#define 	PASS_ALL_GUIDE	 "pass_all_guide"
#define 	ADD_BLUE	 "add_blue"
#define 	ADD_PURPLE	 "add_purple"
#define 	SET_VIP_LEVEL	 "set_vip_level"
#define 	ADD_STUDY	 "add_study"
#define 	ADD_STUDY_EXP	 "add_study_exp"
#define 	ADD_PAYGOLD	 "add_pay_gold"
#define 	ALL_GOLD	 "all_gold"
#define 	ALL_STORY	 "all_story"
#define 	ALL_SCIENCE	 "all_science"
#define 	ALL_PHYSTRENGTH  "all_phystrength"
#define 	ALL_ITEM	 "all_item"
#define 	ADD_JINGJIE	 "add_jingjie"
#define 	DEC_ITEM	 "dec_item"
#define		SET_JINGJIE_LEVEL	 "set_jingjie_level"
#define		SET_JINGJIE  "set_jingjie"
#define		GET_PLAYER_INFO  "get_player_info"
#define		INSTANCE_BATTLE  "instance_battle"
#define		PLAYER_OL_NUM  "player_ol_num"
#define		SILENCE_CHAT  "silence_chat"
#define		WORD_FILTER  "word_filter"
#define		GET_CHAT_MSG  "get_chat_msg"
#define		DEC_COIN  "dec_coin"
#define		DEC_GOLD  "dec_gold"
#define		DEC_RECHARGE_RMB  "dec_recharge_rmb"
#define		GET_SLIENCE_LST  "get_slience_lst"
#define		GET_PLAYER_RES  "get_player_res"
#define		GET_ACTIVITY_ENCOURAGE  "get_activity_encourage"
#define		RESET_PWD  "reset_pwd"
#define		FORBIB_LOGIN  "forbid_login"
#define		FORBID_REGISTERED  "forbid_registered"
#define		SET_ACCOUNT_TYPE	 "set_account_type"
#define		SET_GM_INFO  "set_gm_info"
#define		SYS_CHATDATA  "sys_chat_data"
#define		KICKOUT	 "kick_out"
#define		ACTIVITY_ENCOURAGE  "activity_encourage"
#define		CK_ITEM_NUM  "ck_item_num"
#define		GET_GIVE_RES	 "get_give_res"
#define		START_SERVER	 "start_server"
#define		OPRATING_ACTIVITY_BATTLE  "oprating_activity_battle"
#define		GET_PLAYER_LEVEL	 "get_player_level"
#define		HTTP_DEC_GOLD	 "http_dec_gold"
#define		GET_CONSUME	 "get_consume"
#define		ADD_MEDAL "add_medal"
#define		ADD_FACTION_FUNDS "add_faction_funds"
#define		GET_ONLINE_PLAYER_INFO "get_online_player_info"
#define		ADD_DOORSTRIBUTE "add_doorstribute"
#define		SET_FACTION_LEVEL "set_faction_level"
#define		GET_CLIVER "get_cliver"
#define		RETREATE "retreate"
#define		RESET_FACTION_NAME "reset_faction_name"
#define     ADD_COURAGE_TIMES "add_courage_times"
#define     GET_RANK "get_rank"
#define     GET_VIP_LEVEL "get_vip_level"
#define     GET_FRIEDS_NUM "get_friends_num"
#define     IS_JOIN_IN_FACTION "is_join_in_faction"
#define     QUERY_SUM_DOORSTRIBUTE "query_sum_doorstribute"
#define     ADD_HERO "add_hero"   
#define     CHANGE_NOTICE "change_notice"   
#define     CHANGE_POWER_RANK "get_power_rank" 
#define     ADD_JEWELRY_GM "add_jewelry"
#define		CHANGE_PLAYER_NAME "change_player_name"
#define     ADD_GIFT_GM "add_gift"
#define     OPERATING_ACTIVITY_GM "Operating_Activity"
#define		START_TIME "start_time"
#define		END_TIME "end_time"
#define     SHOW_GVG_PAIR "show_gvg_pair"
#define     SET_GVG_PAIR "set_gvg_pair"
#define     ZONE_ID "zone_id"
#define     ARENA_ID "arena_id"

#define		ADD_COIN_NUM_PARAM "add_coin_num"
#define		ADD_GOLD_NUM_PARAM "add_gold_num"
#define		ADD_STORY_NUM_PARAM "add_story_num"
#define		ADD_SCIENCE_NUM_PARAM "add_science_num"
#define		ADD_PHYSTRENGTH_NUM_PARAM "add_phystrength_num"
#define		ADD_BUILD_TYPE_PARAM "add_build_type"
#define		ADD_ITEM_TYPE_PARAM	"add_item_type"
#define		ADD_ITEM_COUNT_PARAM	"add_item_count"
#define		SET_LEVEL_COUNT_PARAM	"set_level_count"
#define		ADD_BLUE_COUNT_PARAM	"add_blue_count"
#define		ADD_PURPLE_COUNT_PARAM	"add_purple_count"
#define		SET_VIP_LEVEL_COUNT_PARAM "set_vip_level_count"
#define		ADD_STUDY_COUNT_PARAM "add_study_count"
#define		ADD_STUDY_EXP_COUNT_PARAM "add_study_exp_count"
#define		ADD_PAYGOLD_COUNT_PARAM	"add_paygold_count"
#define		ALL_GOLD_COUNT_PARAM	"all_gold_count"
#define		ALL_STORY_COUNT_PARAM	"all_story_count"
#define		ALL_SCIENCE_COUNT_PARAM	"all_science_count"
#define		ALL_PHYSTRENGTH_COUNT_PARAM	"all_phystrength_count"
#define		ALL_ITEM_TYPE_PARAM "all_item_type"
#define		ALL_ITEM_COUNT_PARAM "all_item_count"
#define		ADD_JINGJIE_COUNT_PARAM "add_jingjie_count"
#define		DEC_ITEM_TYPE_PARAM "dec_item_type"
#define		DEC_ITEM_COUNT_PARAM "dec_item_count"
#define		SET_JINGJIE_LEVLE_COUNT_PARAM "set_jingjie_level_count"
#define		SET_JINGJIE_SUB_LEVEL_COUNT_PARAM "set_jingjie_sub_level_count"
#define		SET_JINGJIE_COUNT_PARAM "set_jingjie_count"
#define		SILENCE_CHAT_TYPE_PARAM "silence_chat_type"
#define		SILENCE_CHAT_INFO_PARAM "silence_chat_info"
#define		WORD_FILTER_TYPE_PARAM	"word_filter_type"
#define		WORD_FILTER_INFO_PARAM	"word_filter_info"
#define		DEC_COIN_COUNT_PARAM "dec_coin_count"
#define		DEC_GOLD_COUNT_PARAM "dec_gold_count"
#define		DEC_RECHARGE_RMB_COUNT_PARAM "dec_recharge_rmb_count"
#define		NEW_PWD_PARAM "new_pwd"
#define		FORBID_REGISTERED_USER_ID_PARAM "forbid_registered_user_id"
#define		FORBID_REGISTERED_EQU_ID_PARAM "forbid_registered_equ_id"
#define		SET_ACCOUNT_TYPE_PARAM "set_account_type_value"
#define		ACTIVITY_ENCOURAGE_VALUE_PARAM	"activity_encourage_value"
#define		CK_ITEM_NUM_VALUE_PARAM "ck_item_num_value"
#define		GET_RES_INFO_VALUE_PARAM	"get_res_info_value"
#define		GIVE_RES_INFO_VALUE_PARAM	"give_res_info_value"
#define		START_SERVER_TIME_PARAM	"start_server_time"
#define		ACTIVITY_ID_PARAM "activity_version_id"
#define		TOWN_ID_PARAM	"town_id"
#define		COPY_ID_PARAM	"copy_id"
#define		START_TIME_PARAM "start_time"
#define		END_TIME_PARAM "end_time"
#define		ADD_MEDAL_VALUE_PARAM "add_medal_value"
#define		ADD_FACTION_FUNDS_VALUE_PARAM "add_faction_funds_value"
#define		RELOAD_CFG_TYPE_PARAM "reload_cfg_type"
#define		OPTTYPE_PARAM "opttype"
#define		SYS_CHATDATA_INFO_PARAM "sys_chatdata_info"
#define		SYS_CHATDATA_EXPIRESTIME_PARAM	"sys_chatdata_expirestime"
#define		HTTP_DEC_GOLD_VALUE_PARAM "http_dec_gold_value"
#define		INSTANCE_BATTLE_SCENE_ID_PARAM "instance_battle_scene_id"
#define		INSTANCE_BATTLE_TOWN_ID_PARAM "instance_battle_town_id"
#define		INSTANCE_BATTLE_INSTANCE_ID_PARAM "instance_battle_instance_id"
#define		INSTANCE_BATTLE_NUM_PARAM	"instance_battle_num"
#define		RES_BUILD_TYPE_IS_OPEN_PARAM "res_build_type"
#define		RES_BAG_ELSE_AMOUNT_PARAM "res_bag_else_amount"
#define		RES_STUDY_BAG_ELSE_AMOUNT_PARAM "res_study_bag_else_amount"
#define		ADD_STUDY_COLOR_KIND_PARAM "color_kind"
#define		ADD_STUDY_ATTR_KINT_PARAM "attr_kind"
#define		ADD_STUDY_LEVEL_PARAM "study_level"
#define		ADD_DOORSTRIBUTE_COUNT_PARAM "add_doorstribute_count"
#define		SET_FACTION_LEVEL_VALUE_PARAM "faction_level_value"
#define		SET_VIP_LEVEL_TOTAL_AMOUNT_PARAM "total_amount"
#define		RETREATE_TYPE_PARAM "retreate_type"
#define		RETREATE_FILE_NAME_PAMAM "retreate_file_name"
#define		RESET_FACTION_NAME_PAMAM "new_faction_name"
#define     ADD_COURAGE_TIMES_PARAM "add_courage_time_times"
#define		IS_JOIN_IN_FACTION_PARAM "is_join_in_faction"
#define     ADD_HERO_PARAM "add_hero_id"
#define     ADD_JEWELRY_KINDID "add_jewelry_kindID"
#define     ADD_JEWELRY_ATTR_NUM "add_jewelry_attr_num"
#define     ADD_JEWELRY_ATTR "add_jewelry_attr"
#define		ADD_GIFT_NAME_AND_DESC "add_gift_name_and_desc"
#define		ADD_GIFT_OUT_TIME "add_gift_out_time"
#endif //#ifndef _COMMONDEF_H_
