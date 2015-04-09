#ifndef _COMMONDEF_H_
#define _COMMONDEF_H_


#define  INVALID_MSGID 0 //无效的协议号

///////// 装备强化方式
#define EQUIP_STRENGTHEN_COIN 0 //金币
#define EQUIP_STRENGTHEN_GOID 1 //元宝

#define NULL_FACTION_ICON_ID 255 // 表示没有门派图标（图标0已被占用，代表默认图标）
#define FLY_LEVEL 120 // 玩家飞升等级，超过飞升等级用1转xx级表示，如130级 = 1转10级


enum AttackTarget
{
	EAT_MYSELF = 1,//己方
	EAT_ENEMY = 2,//敌方
};

enum EWU_XING
{
	EWX_GOLD = 1,//金
	EWX_WOOD = 2,//木
	EWX_WATER = 3,//水
	EWX_FIRE = 4,//火
	EWX_SOIL = 5,//土
};

//颜色
enum EColor
{
	EC_BLUE = 1,//蓝色
	EC_PURPLE = 2,//紫色
	EC_RED = 3,//红色
	EC_ORANGE = 4,//橙色
	EC_DOUBLE_COLOR = 5,//双属性
};

enum COLOR_TYPE
{
	ECT_NONE = 0,//无
	ECT_BULE = 1,//蓝色
	ECT_PURPLE = 2,//紫色
	ECT_RED = 3,//红色
	ECT_ORANGE = 4,//橙色
	ECT_YELLOW = 5,//黄色
	ECT_GREEN = 6,//绿色
	ECT_CYAN = 7,//青色
};

//资源类型
enum ACTE_KIND_TYPE
{
	ACTE_GOOD				= 0,//物品
	ACTE_HERO				= 1,	//武将
	ACTE_COIN				= 2,	//铜钱
	ACTE_GOLD				= 3,	//元宝
	ACTE_PHYSTRENGTH		= 4,	//体力
	ACTE_SCIENCE			= 5,	//器魂点
	ACTE_STORY				= 6,	//阅历
	ACTE_BLUEGAS			= 7,	//仙露
	ACTE_PRUPLEGAS			= 8,	//功力
	ACTE_JINGJIE			= 9,	//境界
	ACTE_DoorsTribute		= 10,	//门贡
	ACTE_FactionFunds		= 11,	//门派资金
	ACTE_FactionTaskIntegral = 12,	//门派任务积分
	ACTE_StudyClip			= 13,	//乾坤碎片
	ACTE_SOUL				= 14,	//战魂
	ACTE_STUDYEXP			= 15,	//卦象经验
	ACTE_HERO_EXP			= 16,	//人物经验
	ACTE_PETSCORE			,	// 宠物积分
};


//战斗属性类型
enum EBattleAttributeKind
{
    EBAK_INVALID = 0,	//无效
    EBAK_HP = 1,		//HP
    EBAK_ATTACK = 2,	//攻击力
    EBAK_HITRATE = 3,		//破击率
    EBAK_DODGERATE = 4,		//格挡率
    EBAK_CRITRATE = 5,		//暴击率
    EBAK_DECRITRATE = 6,	//抗暴率
    EBAK_INCHURTRATE = 7,	//增伤率(增加的伤害百分比)
    EBAK_DECHURTRATE = 8,	//减伤率(减少的伤害百分比)
    EBAK_ANGRY = 9,			//怒气值
    EBAK_ODDANGRY = 10,		//剩余怒气值
    EBAK_FIRSTATTACK = 11,	//先攻
    EBAK_COACHTALENT = 12,	//资质
    EBAK_HP2 = 13, //HP2(只有科技馆有用)
    EBAK_ATTACK2 = 14, //攻击力2(只有科技馆有用)
	EBAK_DEFEND = 15,//防御
    EBAK_MAX, //最大
};

//战斗特效
enum EBattleSpecialEffects
{
	EBSE_NONE = 0,//无
	EBSE_CONTINUE_ATTACK = 1,//连击1
	EBSE_SUCK_BLOOD = 2,//吸血1
	EBSE_PHYSICAL_IMMUNITY = 3,//物免
	EBSE_MAGIC_IMMUNITY = 4,//魔免
	EBSE_ADD_BLOOD = 5,//治疗
	EBSE_REBOUND = 6,//反弹
	EBSE_BEAT_BACK = 7,//反击
	EBSE_DODGE = 8,//闪避
	EBSE_PROTECT_BODY = 9,//霸气护体
	EBSE_IRRESISTIBLE_FORCE = 10,//势如破竹
	EBSE_NARROW_ESCAPE = 11,//绝境逢生
	EBSE_BAGUS_FLYER = 12,//天马行空
	EBSE_TIGER = 13,//虎虎生威
	EBSE_AVALANCHE = 14,//排山倒海
	EBSE_BURN_BOATS = 15,//破釜沉舟
	EBSE_LAST_STAND = 16,//背水一战

	EBSE_MAX, //无效
};

enum EBattleSpecialEffectUpgradeCase
{
	EBSEUC_PROBABILITY = 1,//升级概率
	EBSEUC_EFFECT = 2,//效果百分比
	EBSEUC_LIMIT_NUM = 3,//限制的次数
};
/**
 * @brief
 * 爆出物品类型
 */
enum EBurstKind
{
    EBKIND_INVALID = 0, //无效
    EBKIND_COIN = 1, //钱币
    EBKIND_GOLD = 2, //元宝
    EBKIND_EXPERIENCE = 3, //经验
    EBKIND_PHYSTRENGTH = 4, //体力
    EBKIND_ITEM = 5, //道具
    EBKIND_ATTRIBUTE = 6, //属性
    EBKIND_SCIENCE = 7, //科技点
    EBKIND_STORY = 8, //阅历点
};


//建筑类型
enum EBuildKind
{
    EBK_MAINTOWN = 1, //主城
    //EBK_PRODUCE = 2, //生产建筑
    EBK_SCIENCE = 3, //科技建筑
    EBK_SMITHY = 4, //铁匠铺建筑
    EBK_RECRUIT	= 5, //招募馆
    EBK_PVP = 6, //比武
    EBK_GODWEAPON = 9, //神器建筑
    EBK_AWAKEN = 10, //悟道
    EBK_PLUNDER = 11, //掠夺
    //EBK_TALENT = 12, //修为馆
    EBK_CROP = 13, //种植场
    EBK_ADVENTURE = 15,	//冒险
    //EBK_BOOK = 16, //爬塔
    EBK_JINGJIE = 17, //境界
    EBK_FACTION = 18, //门派
	EBK_JEWELRY = 19, //首饰
	EBK_PET			 = 20, // 宠物系统

    EBK_SHOP = 101, //商店
    EBK_ELITEINSTANCE = 102, //精英副本
    EBK_ONHOOK = 103, //挂机房
    EBK_EQUPUPGRADE = 104, //装备升阶馆
    EBK_MONEYTREE = 105, //招财树
    //EBK_JINGJIE=106, //境界
    EBK_EXPRIENCE = 107,	//经验丹
    EBK_UPDATEATTR = 108, //属性升级
    EBK_LEVY = 109, //征收建筑
    EBK_FROMATION = 110, //阵型建筑
    EBK_MELTING = 111,		//熔炼建筑
    EBK_EATBREAD = 112,		//吃包子
    EBK_XIANGQIAN = 113,		//镶嵌
    EBK_COLLECTGAS = 114,		//收集灵气
    EBK_CLIMBTOWER = 115,		//爬塔
    EBK_ACTIVITY = 116, //活动中心
    EBK_YABIAO = 117, //押镖
    EBK_SKILL = 118,		//技能
    EBK_BOSSBATTLE = 119,		//BOSS战
    EBK_RECHARGE = 120,		//充值活动
    EBK_SCORE = 121,			//评分活动奖励
    EBK_EXPTRAN = 122,		//经验传递建筑
    EBK_RDCHALLENGE = 123,		//随机挑战
    EBK_RACEBUILD = 124,	//竞技场
    EBK_ACTIVITY_ENCOURAGE = 125,	//活动奖励技场
    //EBK_FACTION = 126,	//帮派
    EBK_EQUIPCOMPOSE = 127,	//装备合成
    EBK_STUDYEXCHANGE = 128,	//卦象兑换 卦象分解
    EBK_SCORERATING = 129,		//完美通关建筑
    EBK_BEGBLUE = 130,  //求仙露
    EBK_OPENLIGHT = 131,    //装备开光
    EBK_ITEMEXCHANGE = 132,    //道具兑换
    EBK_VIEWCHAT = 133,    //观看聊天
    EBK_SNDCHAT      = 134, //发送聊天
    EBK_FRIENDS      = 135,	//好友系统
    EBK_COURAGE      = 136, //勇气试炼
    EBK_EQUIP_MASTER = 137, // 装备精通（此功能已废弃）
	EBK_GVG			 = 138,//跨服战
	EBK_MEDAL        = 139,//勋章
	EBK_SOUL         = 140,//战魂
	EBK_PETFEED		 = 142, // 宠物喂养
	EBK_PETMERGE	 = 143, // 宠物融合
	EBK_PETUPSTAR	 = 144, // 宠物升星
	EBK_PETWASH		 = 145, // 宠物洗资质
	EBK_PETLIANYAOHU = 146, // 炼妖壶

    EBK_ENCHANT		 = 147, // 装备附魔
    EBK_EQUIP_MAKE	 = 148, // 装备制作
    EBK_FLY	         = 149, // 飞升功能
	EBK_NEIDAN_INSTANCE = 150,//斩兽炼
    EBK_OUT_BOUND = 151, // 外传副本
};

//战斗结果
enum EBattleResult
{
    EBT_SUCCESS = 0, //成功
    EBT_FAILED = 1, //失败
    EBT_INSTANCE_PASS = 2,//副本通关
    EBT_COURAGE_PASS = 3, //勇气通关
};


//武将位置
enum EHeroPos
{
    EHP_CARRY = 0,//携带
    EHP_BATTLE = 1,//上阵，包含携带
    EHP_RECRUIT = 2,//在招募馆中
};

//物品大类型
enum EGoodsMainKind
{
    EGMK_PHYSTRENGTH = 1, //加体力
    EGMK_GOLD = 2, //元宝
    EGMK_DOUBLEEXPERIENCE = 3, //双倍经验
    EGMK_ADDEXPERIENCE = 4, //加经验,增加主将经验
    EGMK_MATERIAL = 5, //材料
    EGMK_RATEBOX = 6, //神秘(随机)礼包
    EGMK_COIN = 7, //铜钱
    EGMK_BOX = 8, //宝箱
    EGMK_DRUG = 9, //丹药
    EGMK_MAKER = 10, //制作卷
    //EGMK_PLUNDERLOWERLEVEL = 11, //掠夺低于5级玩家的道具
    //EGMK_ADDPLUNDERTIMES = 12,//增加掠夺次数道具
    //EGMK_AVOIDPLUNDERBATTLE = 13,//掠夺免战道具
    //EGMK_GODWEAPONQUALITYUPGRADE = 14,//神器进阶符
    EGMK_SCIENCE = 11, //器魂
    EGMK_INNERBOX = 12, //内部宝箱，给该物品时系统直接给玩家使用
    EGMK_EXPERIENCE = 13, //伙伴经验丹
    EGMK_CROP = 14, //种植种子
    EGMK_XIANGQIANGEM = 15, //镶嵌宝石
    EGMK_XIANGQIANCHIP = 16, //镶嵌碎片
    EGMK_BLUEGAS = 17, //蓝气
    EGMK_PURPLEGAS = 18, //紫气
    EGMK_JINGJIE = 19, //境界
    EGMK_GEMMAKER = 20, //宝石制作卷
    EGMK_EQUIP_DEBRIS = 21, //装备碎片
    EGMK_STORY = 22, //阅历
    EGMK_DOORSTRIBUTE = 23, //门贡
	EGMK_JEWELRY_ADVANCED = 24,//饰品进阶
	EGMK_EQUIP_ADVANCED = 25,//装备进阶石
	EGMK_ROSE = 26,//玫瑰
	EGMK_SOUL_METERIAL = 27,//战魂材料
	EGMK_NORMAL_PET_BAG = 28, // 普通宠物袋
	EGMK_PET_NORMAL_BABY_EGG = 29, // 宝宝蛋
	EGMK_PET_UNNORMAL_BABY_EGG = 30, // 变异宝宝蛋
	EGMK_DRAGON_BALL = 31, // 龙珠
    EGMK_JINLIULU = 32,	// 洗资质物品
	EGMK_EQUIP_MAKE_BOOK = 33,	// 外传装备制作卷
    EGMK_HERO_MATERIAL = 34,	// 伙伴材料
    EGMK_EQUIP_MATERIAL = 35,	// 装备材料

    EGMK_OTHER = 99, //其他
};


//科技属性类型
enum EScienceKind
{
    ESK_INVALID = 0,//无效的
    ESK_HP = 1,		//HP
    ESK_ATTACK = 2,	//攻击力
    ESK_MAX			//最大的
};

//穿戴类型
enum EDressType
{
    EEDT_DRESS = 0,  //穿上
    EEDT_UNDRESS = 1,  //卸下
    EEDT_REPLACE = 2,  //替换
};



//装备位置类型
enum EEquipPosType
{
    EEPT_HEAD = 1, //头(头盔)
    EEPT_BODY = 2, //身(战袍)
    EEPT_HAND = 3, //手(武器)
    EEPT_FOOT = 4, //脚(战靴)
};


//布阵类型
enum EEmbattleType
{
    EET_UP = 0,  //上阵
    EET_DOWN = 1,  //下阵
    EET_REPLACE = 2,  //替换
    EET_EXCHANGE = 3,  //交换
};

//建筑功能类型
enum EBuildFunctionType
{
    EBFT_OPEN = 0x1, //打开
    EBFT_UPGRADE = 0x1 << 1, //升级
    EBFT_CROWN = 0x1 << 2, //收获
};

//引导功能
enum EGuideFunction
{
    EGF_INVALID = 0x0, //无效
    EGF_EQUIP = 1, //装备穿戴
    EGF_RECUIT = 2, //招募武将
    EGF_STRENGTHEN  = 3, //装备强化
    EGF_EMBATTLE = 4, //布阵
    EGF_CROWN  = 5, //收获
    EGF_DRUG  = 6, //服用丹药
    EGF_PLUNDER  = 7, //掠夺
    EGF_CAPTUREWORK  = 8, //俘虏工作
    EGF_ELITEINSTANCE  = 9, //精英副本
    EGF_ROCKMONEY  = 10, //摇钱
    EGF_GODWEAPON   = 11, //神器
    EGF_RACE  = 12, //竞技场
    EGF_SCIENCETREE = 13, //科技树
    EGF_CROP = 14, //种植
    EGF_EXPRIENCE  = 15, //服用经验丹
    EGF_UPDATEATTR  = 16, //属性升级
    EGF_LEVY = 17, //征收
    EGF_MELTING = 18, //熔炼
    EGF_SECOND_EMBATTLE = 19, //二次布阵
    EGF_YABIAO = 20, //押镖
    EGF_CLIMBTOWER  = 21, //爬塔
    EGF_XIANGQIAN  = 22,//镶嵌
    EGF_JINGJIEUPGRADE = 23,//境界升级
    EGF_GASCOLLECT = 24,//灵气气收集
    EGF_SKILLUPGRADE = 25,//技能升级
    EGF_AWAKEN = 26,//悟道
    EGF_RDC = 27,//随机挑战
    EGF_OPENLIGHT = 28,//开光
    EGF_FRIENDS = 28,//好友
    EGF_FACTION = 29,//门派
    EGF_BEGBLUE = 30,//求仙露
	EGF_PET = 31,//求宠物
	EGF_PETFEED	= 32,	
	EGF_PETMERGE = 33,	
	EGF_PETUPSTAR = 34,	
	EGF_PETWASH	= 35,	
	EGF_PETLIANYAOHU = 36,
	EGF_PET_FIGHT = 37,	// 宠物出战
    EGF_MAX		//上限128
};



//怪物类型
enum EMonsterType
{
    EMT_NORMAL = 1, //普通
    EMT_SBOSS = 2, //小BOSS
    EMT_BBOSS = 3, //大BOSS
    EMT_ELITE = 4, //精英
};

//技能效果类型，最多32种类型
enum ESkillEffectKind
{
    ESEK_HURT = 1, //伤害，攻击时触发
    ESEK_SINGLECUER = 2, //单体回血，攻击时回血
    ESEK_WHOLECUER = 3, //全体回血，攻击时回血
    ESEK_DECANGRY = 4, //恐吓，攻击时降低敌方怒气
	ESEK_SECOND_ATTACK = 5,//二次伤害(风咒)
	ESEK_CRITICAL = 6,//暴击（火咒）
	ESEK_WATER = 7,//水咒
	ESEK_SOIL = 8,//土咒
	ESEK_ICE = 9,//冰封术
	ESEK_SAND = 10,//砂爆术
	ESEK_SPARK = 11,//流焱术
	ESEK_HAUNTING = 12,//阴魂不散
	ESEK_DERAM = 13,//梦语幻境
	ESEK_SOUL = 14,//魂牵梦绕
	ESEK_CHARM = 15,//上古灵符

	ESEK_MAX,//无效
};


//buff类型，最多32种类型
enum EBuffKind
{
	EBK_NONE = 0,//无，没有BUFF
    EBK_STATIC = 1, //静止,暂定拥有该状态者本回合内攻击，属于负面状态
    EBK_NO_SKILL = 2, //沉默，暂定拥有该状态者本回合内使用技能，属于负面状态
    EBK_POISONING  = 3, //中毒，使得拥有该状态者本回合内掉生命百分比血量，属于负面状态
    EBK_DEC_ATTACK = 4, //残废，使得拥有该状态者本回合内攻击力按百分比降低，属于负面状态
    EBK_TEAR  = 5, //撕裂，使得拥有该状态者本回合内掉生命百分比血量，属于负面状态
    EBK_BLOODTHIRSTY_S = 11, //嗜血(单体)， 使得拥有该状态者本回合内攻击力按百分比提升，属于增益状态
    EBK_BLOODTHIRSTY_A = 12, //嗜血(全体)， 使得拥有该状态者本回合内攻击力按百分比提升，属于增益状态
    EBK_DECHURT_S = 13, //减伤(单体)，使得拥有该状态者受击命中时减少伤害值，属于增益状态
    EBK_DECHURT_A = 14, //减伤(全体)，使得拥有该状态者受击命中时减少伤害值，属于增益状态
	EBK_ADD_BLOOD = 15,//回春术，回复生命值，增益状态
	EBK_DEFEND_PERCENT = 16,//增加防御（百分比）,为前排单体增加防御，属于增益状态
	EBK_FROST = 17,//冻结，使目标冻结，暂停该状态者回合内攻击，属于负面状态
	EBK_CHAOS = 18,//混乱，使该状态者不分敌我选择目标攻击，属于负面状态
	EBK_FIRING = 19,//灼烧，使目标损失生命，属于负面状态
	EBK_DISTURB = 20,//干扰，降低攻击伤害，属于负面状态
	EBK_NOT_ANGER = 21,//失去斗志，使目标在作用期间内无法获得怒气，属于负面状态
	EBK_SLEEP = 22,//沉睡，使目标沉睡回合内无法攻击，属于负面状态
	EBK_DREAMLAND = 23,//幻境,期间精神将被我方控制并陷入狂暴，无法释放技能，伤害上升100%，并随机攻击对方单体(如果对方只剩1人在场，则只陷入沉睡1回合)
};


#define  MAIN_LINE_TASK		1 //主线任务
//#define  BRANCH_LINE_TASK	2 //支线任务

//掠夺战报类型
enum EPlunderBattleLogType
{
    EPBLT_PLUNDER = 0,//掠夺
    EPBLT_RESIST = 1,//反抗
    EPBLT_RAP = 2,//争夺
    EPBLT_AUTOFREE = 3,//自动释放俘虏，时间到自动释放
    EPBLT_MANUALFREE = 4,//手动释放俘虏(俘虏主手动释放)
    EPBLT_DRIVEAWAY = 5,//驱赶俘虏主
    EPBLT_INCOME = 6,//正常收益完成
    EPBLT_BLEED = 7,//榨干收益
};

//掠夺工作报类型
enum EPlunderWorkLogType
{
    EPWLT_CAPTURE_WORK = 0,//俘虏工作
    EPWLT_FOR_OWNER = 1,//为俘虏主工作
    EPWLT_CAPTURE_HANDIN = 2,//俘虏工作收益上缴

};

//悟道心得类型
enum EAwakenLevelKind
{
    EALK_KESHUI = 1, //瞌睡
    EALK_SHENGYOU = 2, //神游
    EALK_MINGSI = 3, //冥思
    EALK_RUDING = 4, //入定
    EALK_CHEWU = 5, //彻悟
};

//心得穿戴操作位置
enum EStutyDressOpPos
{
    ESDOP_HERO = 0, //武将心得
    ESDOP_STORE = 1, //存储心得
    ESDOP_EXCHANGE = 2, //武将和存储交互
};

//技能级别
enum ESkillGrade
{
    ESG_NORMAL = 1, //一般
    ESG_RARE = 2,//稀有
};


//掠夺俘虏工作类型
enum EPlunderWorkType
{
    EPWT_UNSET = 0, //未设置
    EPWT_COIN = 1, //铜币
    EPWT_SCIENCE = 2, //器魂
};


//职业类型
enum ECareerKind
{
    ECK_INVALID = 0,//无效
    ECK_ATTACK = 1, //攻击
    ECK_DEFENCE = 2, //防御
};


//强化属性类型
enum EAttrStrengthType
{
    EAST_HP = 0, //血量
    EAST_ATTACK = 1, //攻击
	EAST_DEFEND = 2,//防御
};

//强化属性方式
enum EUpdateAttrByType
{
    EUABT_COIN = 0, //铜币
    EUABT_ITEM = 1, //物品
};

//收获类型
enum ECropKind
{
    ECPK_INVALID = 0,		//无效
    ECPK_COIN = 1,		//铜钱
    ECPK_SCIENCE = 2,		//器魂
    ECPK_EXPRIENCE = 3,	//经验丹
    ECPK_STORY = 4,		//阅历
    ECPK_JINGJIE = 5,		//境界
};


//技能攻击范围类型
enum EAttackRangKind
{
    EARK_INVALID = 0, //无效
    EARK_FRONTONE = 1, //前排单体
    EARK_BACKONE , //= 2, //后排单体
    EARK_FRONTALL , //= 3, //前排全体
    EARK_BACKALL, //= 4, //后排全体
    EARK_ALL, // = 5, //所有
    EARK_BUFF, // = 6, //追踪buff
    EARK_MIXBLOOD, //= 7, //追踪最少血量
    EARK_RAMDOM_COMBAT_TWO_ENEMY, // = 8, //随机打击两个敌人
	EARK_RANDOM_BY_PLAYER_LEVEL = 9,//根据等级来确定打击人数(随机选中)
    EARK_MAX, //最大
};


//活动类型
enum EActivityKind
{
    EAK_EATBREAD = 1, //吃包子
    EAK_BOSSBATTLE = 2, //BOSS战
    EAK_COURAGE = 3, //勇气试炼
    EAK_HUNTING = 4, //捉妖
    EAK_GVG     = 5, //门派跨服战
};

//聊天类型
enum EChatType
{
    ECT_SYS		 = 0, // 系统消息
    ECT_USER	 = 1, // 用户消息
    ECT_NOTIFY	 = 2, // 通知 在屏显里的优先级比ECT_SYS高
    ECT_GM		 = 3, // GM客服
    ECT_FACTION  = 4, // 门派频道(客户端版本需>=301才支持)
    ECT_ACTIVITY = 5, // 活动频道(客户端版本需>=301才支持)
};

//精气类型
enum EGapType
{
    EGT_BLUE = 0, //蓝气
    EGT_PURPLE = 1, //紫气
};

//通知类型
enum ENoticeType
{
    ENT_OUTLINE = 0, //玩家不在线
    ENT_RACE = 1, //竞技场通知
    ENT_EATBREAD = 2, //吃包子
    ENT_BOSSBATTLE = 3, //BOSS战
    ENT_COURAGETTLE = 4, //勇气试炼
    ENT_OTHER = 5, //其它通知
};

//vip增加次数功能
enum EVipIncNumFunc
{
    EVINF_INVALID = 0x0, //无效
    EVINF_MONEYTREE = 1, //招财
    EVINF_RACEBUYNUM = 2, //竞技场购买挑战次数
    EVINF_PLUNDERBUYNUM = 3, //掠夺购买挑战次数
    EVINF_PHYSTRENGTHBUYNUM = 4, //购买体力次数
    EVINF_CLIMBTOWERRESET = 5,		//爬塔每天可重置次数
    //EVINF_GENRANDOMGAS=6, //随机聚气
    EVINF_HEROEATJINGYANDAN = 7, //伙伴服用经验丹
    EVINF_RESETELITEINSTANCE = 8, //重置精英副本
    //EVINF_GENPURPLEGAS = 9, //紫色聚气
    EVINF_AWAKENCALL = 11 , //悟道召唤
    EVINF_GATHER_SCIENCE_COIN = 12 , //铜钱聚魂
    EVINF_GATHER_SCIENCE_GOLD = 13 , //元宝聚魂
    EVINF_BEG_BLUE_GAS = 14 , //求仙露
    EVINF_GOLD_DONATE = 15,		//元宝捐献次数
    EVINF_COURAGE = 16,   //勇气试炼次数
	EVINF_PET_FEED = 17,   //宠物喂养摇把子
    EVINF_MAX, //最大
};

//vip功能
enum EVipFunc
{
    EVF_VISITGOLD = 1, //招募馆出千功能
    EVF_ONHOOKETELITEINSTANCE = 2, //扫荡精英副本
    EVF_GODWEAPONPERFECTRELEASE = 3, //神器完美释放
    EVF_AUTOCLIMBTOWER = 5, //自动爬塔
    EVF_BOSSBGOLDINSPIREPOWER = 6, //boss战元宝鼓舞战力
    EVF_GOLDMELTEQUIP = 7, //元宝熔炼获得最大值器魂
    EVF_QUJINGCALLTANGSENG = 8, //取经召唤唐僧
    EVF_BOSSBAUTOBATTLE = 9, //BOSS战自动战斗
    EVF_GENGASFULL = 10, //一键聚气
    EVF_GENGAS = 11, //元宝聚气
    EVF_FINDMONSTER = 12,  //筋斗云
    EVF_OPENGOLDMODE = 13, //开启黄金模式
};

//功能消耗
enum ECostFunc
{
    EGCF_INVALID = 0x0, //无效
    EGCF_MONEYTREE = 1, //招财
    EGCF_RACEBUYNUM = 2, //竞技场购买挑战次数
    EGCF_PLUNDERBUYNUM = 3, //掠夺购买挑战次数
    EGCF_PHYSTRENGTHBUYNUM = 4, //购买体力次数
    EGCF_CLIMBTOWERRESET = 5,		//爬塔每天可免费重置次数
    //EGCF_GENRANDOMGAS=6, //随机聚气
    //EGCF_HEROEATJINGYANDAN = 7, //伙伴服用经验丹
    EGCF_RESETELITEINSTANCE = 8, //重置精英副本
    //EGCF_GENPURPLEGAS = 9, //紫色聚气
    //EGCF_AWAKENCALL = 11 , //悟道召唤
    EGCF_GATHER_SCIENCE_COIN = 12 , //铜钱聚魂
    EGCF_GATHER_SCIENCE_GOLD = 13 , //元宝聚魂
    EGCF_BEG_BLUE_IMPRV_LUCK = 14,  //求仙露改运消费
    EGCF_SHADE_CHANGE_MONSTER = 15,		//暗部换怪
    EGCF_BUY_ENCOURAGE_TIMES = 16, //购买勇气试炼次数
    EGCF_FACTION_OPEN_GOLD = 17, //开启黄金模式
    EGCF_FACTION_CLOSE_MONSTER = 18,//收妖
    EGCF_FACTION_CREATE_BY_COIN = 19,//铜钱创建门派
    EGCF_FACTION_CREATE_BY_GOLD = 20,//元宝创建门派
	EGCF_FACTION_CHANGE_ICON = 21, //更换门派头像

    EGCF_NOTVIP	=	100,
    //小于100的根VIP功能无关
    EGCF_PLUNDERDRIVEAWAY = 101, //驱逐
    EGCF_GENCOIN = 102, //聚魂铜钱
    EGCF_GENGOLD = 103, //聚魂元宝
    EGCF_GENUPGOLD = 104, //聚魂元宝随机
    EGCF_GENUPGOLDFULL = 105, //聚魂元宝加满
    EGCF_MAX,//最大
};


//功能获取
enum EObtainFunc
{
    EOF_GATHER_SCIENCE_COIN = 1 , //铜钱聚魂
    EOF_GATHER_SCIENCE_GOLD = 2 , //元宝聚魂
    //EOF_BLUE = 3, //蓝气
    //EOF_PURPLE = 4, //紫气
    EOF_MAX,//最大
};

enum ERSType
{
    ERS_COIN = 1,//铜钱
    ERS_STORY = 2,//阅历
    ERS_SCIENCE = 3, //器魂
    ERS_JINGJIE = 4,//境界
};


enum ERS_COIN_Type
{
    ERS_COIN_MAIN = 1,	//主城征收
    ERS_COIN_CROP = 2,	//种植
    ERS_COIN_MONEYTREE = 3,//招财
    ERS_COIN_SELL = 4,//卖物品
    ERS_COIN_MONSTER = 5,//杀怪
    ERS_COIN_BUDDHISTSUTRAS = 6,//西天取经
};

enum ERS_STORY_Type
{
    ERS_STORY_INSTANCE = 1,	//副本掉落
    ERS_STORY_CROP = 2,	//种植
};

enum ERS_SCIENCE_Type
{
    ERS_SCIENCE_MELT = 1,	//熔炼
    ERS_SCIENCE_CROP = 2,	//种植
};

enum ERS_JINGJIE_Type
{
    ERS_JINGJIE_BOSS = 1,			//挑战BOSS获得
    ERS_JINGJIE_XTQJ = 2,			//取经获得
    ERS_JINGJIE_RACE = 3,			//竞技场可获得
    ERS_JINGJIE_CROP = 4,			//种植境界
    ERS_JINGJIE_BAISHEN = 5,		//拜神
    ERS_JINGJIE_SHAGNXIAN = 6,		//上香
};

//战报类型
enum EBattleLogType
{
    EBLT_RACE = 1,		//竞技场
    EBLT_PLUNDER = 2,	//小黑屋
    EBLT_YABIAO = 3,	//押镖
    EBLT_BOSSB = 4,		//boss战
    EBLT_FACTION = 5,	//门派
	EBLT_GVG = 6, //跨服战

	EBLT_MAX,//最大
};

enum EEnterGSPARAMType
{
    EEGSPT_MAX_PLAYER_LEVEL = 1,//最大等级，使用wParam2
	EEGSPT_RECHARGE = 2,//充值界面
	EEGSPT_ADVERTISEMENT = 3,//广告
};

//膜拜类型
enum EWorshipType
{
    EWT_RACE = 1,		//竞技场
    EWT_RDC = 2,		//随机挑战
};

//比武提示类型
enum EPvpPrompt
{
    EPP_RECVENC = 1, //可领奖
    EPP_WORSHIP = 2, //可膜拜
    EPP_LIGHTING = 3, //可上香
    EPP_CHALLENGE = 4, //可挑战
    EPP_RDC = 5, //可随机挑战
};

//打开面板建筑信息
enum EPDFB
{
    EPDFB_RACE = 1, //竞技
    EPDFB_ADVENTURE = 2, //冒险
    EPDFB_SMITHY = 3,    // 铁匠铺
};

enum em_Faction_Job
{
    em_Faction_MartialHead			=	1,//门派掌门
    em_Faction_ProxyMartialHead		=	2,//大长老
    em_Faction_Elder				=	3,//长老
    em_Faction_Custodian			=	4,//护法
    em_Faction_Elite				=	5,//精武弟子成员
    em_Faction_Normal				=	6,//普通弟子成员
    em_Faction_Other				=	7,//非本派人员
};

enum em_Faction_Authority
{
    EFA_KickedNormal		=	1,	//踢出普通弟子
    EFA_KickedElite		=	2,	//踢出精英弟子
    EFA_Promotion			=	3,	//升职
    EFA_Demotion			=	4,	//降职
    EFA_Build			=	5,	//升级门派建筑
    EFA_Activity			=	6,	//开启门派特殊活动
    EFA_SignUp			=	7,	//报名门派战
    EFA_Audit			=	8,	//审核
    EFA_EditSetting		=	9,	//修改门派设置
    EFA_ShowLog			=	10,	//查看日志
    EFA_ChangeOther     =   11, //退位让贤
    EFA_BecomeHead		=   12,	//继位帮主
	EFA_ChangeIcon      =   13, //更换头像
};

enum em_Faction_Shade_Upgrade_Type
{
    EFSUT_UnlockTask     = 1,		//解锁任务
    EFSUT_TaskSchedule   = 2, 		//任务档数
    EFSUT_TaskTimes	    =	3,		//任务次数
};

enum em_Faction_Task_Type
{
    EFTT_HUNTING = 1,			//捉妖
};

// 好友系统，玩家关系类型
enum EFriendSysRelationType
{
    EFSRT_FRIEND = 0, // 好友
    EFSRT_STRANGER = 1, // 陌生人
    EFSRT_BLACKLIST = 2, // 黑名单
    EFSRT_OTHER = 3, // 路人甲
    EFSRT_FINALLYCONTACT = 4,//最后聊天用户
    EFSRT_BEEN_ADD_FRIEND_NOTICE = 100, // 被添加为好友通知类型
    EFSRT_BEEN_SEND_FLOWER_NOTICE = 101, // 被送花通知类型
};


enum em_State_Mark
{
    ESM_NO = 0,		//不在状态
    ESM_YES = 1,	//在状态
};

enum em_Faction_Log_ID
{
    EFLID_ChangeOther = 1,		//退位让贤
    EFLID_Promotion = 2 ,		//升职
    EFLID_Demotion = 3 ,		//降职
    EFLID_Kicked = 4,			//踢人
    EFLID_LeaveFaction = 5,		//退出门派
    EFLID_JoinFaction = 6,		//加入门派
    EFLID_DonateCoin = 7,		//捐献铜钱
    EFLID_DonateGold = 8,		//捐献元宝
    EFLID_UpgradeFaction = 9,	//升级门派
    EFLID_UpgradeFactionAndBuild = 10,	//升级门派，解锁建筑
    EFLID_UpgradeBuild = 11,		//升级门派建筑
    EFLID_OpenActivity = 12,	//开启门派活动
    EFLID_ShadeActivity = 13,	//暗部开启活动
    EFLID_CatchOrange = 14,		//捉到橙色怪
    EFLID_ChangeSchedule = 15,	//换档奖励
    EFLID_OpenGoldMode = 16,	//开启黄金模式
};


enum em_Faction_Build
{
    EFB_Faction   = 1,  //门派
    EFB_Rareitem  = 2,  //珍品阁
    EFB_Gymnasium = 3,	//武堂
    EFB_Shade     = 4,  //暗部
    EFB_HotSpring = 5,	//温泉
    EFB_ColdPrison = 6,	//冰牢
    EFB_FactionTower = 7,//昊天塔
};

enum em_Faction_Upgrade_Type
{
    EFUT_PlayerNum = 1,	//人数上升
    EFUT_Build_Delock = 2, //建筑解锁
};

enum em_Faction_BattleLog_Type
{
    EFBT_Kicked = 1,	//踢人战报
    EFBT_AddFaction = 2,//加入门派
    EFBT_FactionUpgrade = 3, //门派升级战报
};

enum em_Gymnasium_Skill_Flag
{
    EGSF_Learn = 1,	//1可学习
    EGSF_CurMax = 2,//达到当前满级
    EGSF_SumMax = 3,//达到总满级
    EGSF_DoorsTribute = 4,//门贡不足
    EGSF_Unlock = 5,//未解锁
    EGSF_SumDoorsTribute = 6,//累积门贡不足
};


// 指定类别的状态精通处于什么状态
enum EEquipMasterDetailState
{
    EQUIP_MASTER_UPGRADABLE            = 0, // 可升级
    EQUIP_MASTER_ACTIVATABLE           = 1, // 可激活
    EQUIP_MASTER_BREAKABLE             = 2, // 可突破
    EQUIP_MASTER_TOP                   = 3, // 无法继续提升
    EQUIP_MASTER_PLAYER_LVL_NOT_ENOUGH = 4, // 可突破但玩家等级不足
};

// 培养精通之书面板状态
enum EMasterBookPanelState
{
    MASTER_BOOK_PANEL_UN_OPEN                 = 0, // 尚未打开刮卡面板
    MASTER_BOOK_PANEL_GET_BUT_NOT_WIPED       = 1, // 已打开但未刮开
    MASTER_BOOK_PANEL_WIPED_BUT_NOT_CONFIRMED = 2, // 已刮开但未确认
};

// 精通经验书的大小
enum EEquipMasterExpBookType
{
    MASTER_EXP_BOOK_SMALL = 1, // 小精通经验书
    MASTER_EXP_BOOK_BIG   = 2, // 大精通经验书
};

enum MonsterFuncType
{
    EMFT_NORMAL = 1,	//普通与精英本
    EMFT_TOWER = 2,		//爬塔
    EMFT_BOSS = 3,		//BOSS战
    EMFT_HUNTING = 4,	//捉妖
    EMFT_ACTIVITY = 5,	//活动
    EMFT_COURAGE = 6,	//勇气试炼
};
// 装备精通阶级
enum EEquipMasterRank
{
    EMG_GRADE_0 = 0, // 未激活
    EMG_GRADE_1 = 1, // 初级
    EMG_GRADE_2 = 2, // 中级
    EMG_GRADE_3 = 3, // 高级
    EMG_GRADE_4 = 4, // 极
};

//侧边活动功能提示ID
enum ActiviSidePromptID
{
    EASPID_HotSpring = 1,		//温泉
    EASPID_Hunting = 2,			//捉妖
};

//门派提示编号
enum FactionPromptID
{
    EFPID_Activi = 1,	//有活动
    EFPID_HaveApply = 2,	//有申请
    EFPID_Donate = 3,  //可捐献
    EFPID_Get	= 4,	//可领奖
    EFPID_CanApply = 5, //可申请
    EFPID_HaveNewRareItem = 6,//新货物
    EFPID_CanBuy = 7,//可购买
    EFPID_NewSkill = 8,//新技能
    EFPID_CanLearn = 9,//可学习
    EFPID_CanHunting = 10,//可捉妖
    EFPID_InActivi = 11,//活动中
    EFPID_FindBack = 12,//可找回
};

//铁匠铺提示类型
enum SmithyPromptID
{
    ESPID_STRENGTHEN = 1, // 可强化
    ESPID_ENCHANT = 2,	  // 可附魔
    ESPID_EQUIP_MAKE = 3, // 可制作
};

//酒馆提示类型
enum RecruitPromptID
{
    RPID_RECRUIT = 1, // 可招募
    RPID_UPGRADE = 2,	 // 可进阶
};

// 门派布阵类别
enum EGvGMatrixType
{
    GVG_MATRIX_NONE    = 0,
    GVG_MATRIX_ATTACK  = 1,
    GVG_MATRIX_DEFEND  = 2,
    GVG_MATRIX_BALANCE = 3,
};

//扩展信息位代码意思
enum emExtBit
{
	em_VIP = 0,	//打开vip面板提示信息
	em_EDIT_NAME = 1, //是否可修改名称
};
enum FormationType
{
	EBAK_BAIHU = 1, //白虎+攻击
	EBAK_QINGLONG = 2, //青龙+生命
	EBAK_ZHUQUE = 3, //朱雀+先攻
	EBAK_XUANWU = 4,//玄武+防御
};

// 私聊中对方的角色
enum PrivateChatRoleType
{
    PCRT_NORMAL = 0, // 普通玩家
    PCRT_GM     = 1, // GM
};

enum JewelryDressPos
{
	EJDP_NECKLACE = 1,  //项链
	EJDP_RING = 2,//戒指
	EJDP_BELT = 3, //腰带
	EJDP_Jade = 4,//玉佩
};

enum JewelryExtAttr
{
	EJEA_ARMS = 1,//武器
	EJEA_CLOTHING = 2,//衣服
	EJEA_HELM = 3,//头盔
	EJEA_SHOE = 4,//鞋子
	EJEA_NORMAL_ATTACK = 5,//普通攻击shanghai
	EJEA_SKILL_ATTACK = 6,//技能攻击
	EJEA_WEAPON_ATTACK = 7,//神器攻击
	EJEA_WEAPON_DEFEND = 8,//神器防御
	EJEA_INSTANCE_STORY = 9,//副本阅历
	EJEA_INSTANCE_COIN = 10,//副本铜钱
	EJEA_FIRST_STUDY = 11,//第一卦象
	EJEA_SECOND_STUDY = 12,//第二卦象
	EJEA_THIRD_STUDY = 13,//第三卦象
	EJEA_FOURTH_STUDY = 14,//第四卦象


	EJEA_ALL_EQUIP = 15,//所有装备
	EJEA_ALL_NORMAL_ATTACK = 16,//队伍普通攻击
	EJEA_ALL_SKILL_ATTACK = 17,//队伍技能攻击
	EJEA_ALL_WEAPON_ATTACK = 18,//队伍神器攻击
	EJEA_ALL_WEAPON_DEFEND = 19,//队伍神器防御
	EJEA_INSTANCE_EXPERIENCE = 20,//副本经验
	EJEA_ALL_SCIENCE_LEVEL = 21,//科技等级
	EJEA_ALL_STUDY = 22,//所有卦象
	EJEA_HP = 23,	//生命+XXXX
	EJEA_ATTACK = 24,//攻击+XXXX
	EJEA_DEFENG = 25,//防守+XXXX
	EJEA_HITRATE = 26,//破击率+XXXX
	EJEA_DODGERATE = 27,//格挡率+XXXX
	EJEA_CRITRATE = 28,//暴击率+XXXX
    EJEA_DECRITRATE = 29,//抗暴率+XXXX
	
    EJEA_MAX
};

enum ValueType
{
	EVT_FIX = 1,//固定值
	EVT_PERCENT = 2,//百分数
};

enum JewelryPrompt
{
	EJP_CAN_EXT_UPGRADE = 1,//可提升附加属性
	EJP_CAN_BASE_UPGRADE = 2,//可精炼
};

// 门派跨服战轮次
enum GvGRound
{
    GVG_SLEEPING         = 0, // 活动未开启
	GVG_APPLY            = 1, // 报名
	GVG_MONEY_WAR        = 2, // 本服第一轮（财力大比拼）
	GVG_CHAOS_WAR        = 3, // 本服第二轮（门派大乱斗）（不可押注）
	GVG_16_TO_8          = 4, // 双服16进8
	GVG_8_TO_4           = 5, // 双服8进4
	GVG_FINAL_WAR        = 6, // 四服总决赛
    GVG_END              = 7, // 已结束

    GVG_MAX // 最大值
};

// gvg战况结果
enum GvGLogResult
{
    EGLR_NOT_START  = 0, // 未开始
    EGLR_WIN        = 1, // 成功
    EGLR_LOSE       = 2, // 失败
    EGLR_NO_ENEMY   = 3  // 轮空
};

enum GvGGambleResult
{
	EGGR_LOST = 0,//失败
	EGGR_WIN = 1,//成功
	EGGR_NOT_DONE = 2,//未开始
	EGGR_NOT_GAMBLE = 3,//未押注
};

enum GvGReward
{
	EGR_NONE = 0,//无效礼包

	EGR_BATTLE_CHAOS_WIN = 1,//大乱斗前四战斗奖励
	EGR_BATTLE_16_TO_8_WIN = 2,//16进8奖励
	EGR_BATTLE_8_TO_4_WIN = 3,//8进4战斗奖励
	EGR_BATTLE_FINAL_WAR_WIN = 4,//总决赛
	EGR_BATTLE_CHAOS_LOST = 5,//大乱斗失败奖励
	EGR_BATTLE_16_TO_4_LOST = 6,//双服16进4失败奖励
	EGR_BATTLE_FINAL_WAR_LOST = 7,//4服战斗失败奖励

	EGR_RANK_MONEY = 10,//财力比拼前四奖励
	EGR_RANK_CHAOS_WIN = 11,//大乱斗前四排名奖励
	EGR_RANK_8_TO_4_WIN = 12,//本服四强礼包
	EGR_RANK_FINAL_CHAMPION = 13,//冠军奖励
	EGR_RANK_FINAL_SECOND = 14,//亚军奖励
	EGR_RANK_FINAL_THIRD = 15,//季军奖励
	EGR_RANK_FINAL_4_8 = 16,//4-8名次奖励

	EGR_ZONE_HONOR = 20,//全服荣誉奖励

    EGR_RANK_MONEY_NO_1 = 21,// 财力比拼冠军奖励
    EGR_RANK_MONEY_NO_2,// 财力比拼亚军奖励
    EGR_RANK_MONEY_NO_3 // 财力比拼季军奖励
};

enum GvGiftType
{
	EGGT_BATTLE = 1,//战斗礼包
	EGGT_RANK = 2,//名次礼包
	EGGT_HONOR = 3,//荣誉礼包
};
//加入队伍类型 
enum em_Join_Type
{
	EJT_ALL = 0,   //全部
	EJT_FACTION = 1, //帮派
	EJT_FRIEND = 2, //好友
};

enum em_GvG_Log_Type
{
	EGLT_CONTINUE_KILL = 1,//连斩战报
	EGLT_GVG_FACTION_BATTLE = 2,//门派战斗战报
	EGLT_GVG_GAMBLE = 3,//押注
	EGLT_GVG_ALL_HONOUR = 4,//全服荣誉
};

enum ATTACK_TYPE
{
	EAT_NONE = 0,//无
	EAT_ATTACK = 1,//进攻型
	EAT_DEFEND = 2,//防守型
};

enum EATTACK_TYPE
{
	ET_NORMAL = 1,//普通攻击
	ET_SKILL = 2,//技能攻击
	ET_BUFF = 3,//BUFF攻击
	ET_SPECIAL_EFFECT = 4,//特殊攻击
	ET_PET_NORMAL = 5,//宠物普通攻击
	ET_PET_SKILL = 6,//宠物技能攻击
};

enum MEDAL_BIG_TYPE
{
	EMBT_LIMIT_TIME = 1,//限时勋章
	EMBT_NONE_TIME = 2,//不限时勋章
};

enum MEDAL_SMALL_TYPE
{
	EMST_RACE = 1,//竞技场勋章
	EMST_BOSS_KILL = 2,//BOSS击杀者
	EMST_BOSS_HURT = 3,//BOSS伤害超过1%
	EMST_PHYSTRENGTH = 4,//体力勋章
	EMST_ESCORT = 5,//护送勋章
	EMST_ROCK_MONEY = 6,//摇钱树
	EMST_GATHER_SCIENCE_GOLD = 7,//元宝聚魂
//	EMST_GATHER_SCIENCE_COIN = 8,//铜钱聚魂
	EMST_WORSHIP_PLAYER = 9,//膜拜
//	EMST_WORSHIP_GOD = 10,//拜神
	EMST_AWAKEN_ZTS = 11,//算卦
	EMST_SMILE = 12,//笑脸
	EMST_RESET_TOWER = 13,//重置爬塔
	EMST_CLIMB_TOWER = 14,//爬塔层数
	EMST_PLAYER_LEVEL = 15,//玩家等级达到指定等级
	EMST_PLAYER_POWER = 16,//玩家战力
	EMST_PLAYER_VIP_LEVEL = 17,//VIP等级
	EMST_LOGIN_DAY = 18,//登陆天数
	EMST_INTERCEPT = 19,//拦截
	EMST_RESET_ELITEINSTANCE = 20,//重置精英副本
	EMST_ARREST_BLACK = 21,//小黑屋抓捕
	EMST_DRIVE_BLACK = 22,//小黑屋驱赶
	EMST_RACE_CONTINUE_KILL = 23,//竞技场连斩
	EMST_GVG_MEDAL = 24,//GVG相关勋章
	EMST_GVE_COURAGE = 25,//GVE相关勋章
};

enum HP_CHANGE_REASON
{
	EHPCR_NORMAL_SKILL = 1,//技能或普通攻击
	EHPCR_BUFF = 2,//BUFF
	EHPCR_SPECIAL_SUCK_BLOOD = 3,//吸血
	EHPCR_SPECIAL_ADD_BLOOD = 4,//治疗
	EHPCR_SPECIAL_REBOUND = 5,//反弹
};

enum ePetState {
	ePS_Normal		= 0,	// 普通
	ePS_FOLLOW		,		// 跟随
	ePS_FIGHT		,		// 出战
};

enum ePetSkillType
{
	EPST_FIX = 0,//固定
	EPST_RANDOM = 1,//随机
};

enum ePetStoreItemType {
	ePSIT_CanBuy				= 1,	// 可兑换
	ePSIT_NormalProduct			,		// 通过小召唤获得
	ePSIT_GoodProduct			,		// 通过大召唤获得
};

// 外传装备品阶
enum FlyEquipQuality
{
    FEQ_TIAN  = 1, // 天
    FEQ_DI    = 2, // 地
    FEQ_XUAN  = 3, // 玄
    FEQ_HUANG = 4, // 黄
};

enum eSoulPotency
{
	ESP_HP = 1,		//HP
	ESP_ATTACK = 2,	//攻击力
	ESP_HITRATE = 3,		//破击率
	ESP_DODGERATE = 4,		//格挡率
	ESP_CRITRATE = 5,		//暴击率
	ESP_DECRITRATE = 6,	//抗暴率
	ESP_DEFEND = 7,//防御
	ESP_SKILL_PROBABILITY = 8,//触发机率
	ESP_SKILL_EFFECT = 9,//触发效果
	ESP_SKILL_ROUND = 10,//回合数
};
#endif //#ifndef _COMMONDEF_H_
