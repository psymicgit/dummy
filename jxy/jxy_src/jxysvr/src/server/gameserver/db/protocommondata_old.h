/*       Version Number: 10          */

#ifndef PLAYER_BASE_DATA_OLD_H_INCLUDE_VERSION_10
#define PLAYER_BASE_DATA_OLD_H_INCLUDE_VERSION_10

#include "netdata.h"
#include "sdtype.h"
#include "sdstring.h"
#include <protocol/server/protocommondata.h>
#pragma pack(push, 1)


typedef struct tagDT_COLLECT_GAS_DATA_OLD{
	UINT8            byCollectState; //收集状态,0生成，1收取
	DT_GAS_BALL_DATA astGasBollInfo[MAX_GAS_BALL_NUM]; //精气球信息
	UINT16           wLastRandomGenNum; //上次随机生产次数
	UINT64           qwLastRandomGenTime; //上次随机生产时间
	UINT16           wLastPurpleGenNum; //上次紫气生产次数
	UINT64           qwLastGenPurpleTime; //上次紫气生产次数时间
	UINT16           wLastRandomUpNum; //上次随机提升次数
	UINT64           qwLastRandomUpTime; //上次随机提升时间

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_COLLECT_GAS_DATA_OLD;

typedef struct tagDT_JINGJIE_DATA_OLD{
	UINT8            byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
	DT_JINGJIE_BASE_DATA	stBaseData; //境界基本信息
	DT_COLLECT_GAS_DATA_OLD stCollectData; //精气收集信息
	DT_SKILL_ACTION_DATA_LIST stSkillActionData; //技能招式信息列表

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_JINGJIE_DATA_OLD;


typedef struct tagDT_PLAYER_BASE_DATA_OLD{
	UINT32           dwID; //玩家唯一ID
	TCHAR            aszDispName[USERNAME_LEN]; //显示名称
	UINT64           qwCoin; //铜钱
	UINT64           qwGold; //元宝
	UINT64           qwStory; //阅历
	UINT16           wPhyStrength; //当前体力
	UINT64           qwScience; //当前科技点
	UINT8            abyGuideRecordData[MAX_GUIDE_NUM]; //引导信息(每一字节表示一个引导，0表示不需要，1需要，2已完成)
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

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_PLAYER_BASE_DATA_OLD;


typedef struct tagDT_SKILL_DATA_OLD{
	UINT16           wID; //技能类型ID
	UINT16           wLevel; //技能等级
	UINT16           wActionID; //使用招式
	UINT16           wActionLevel; //使用招式等级
	UINT16           wEffectID; //使用效果

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_SKILL_DATA_OLD;

typedef struct tagDT_HERO_DATA_OLD{
	DT_HERO_BASE_DATA stHeroBaseData; //武将基本信息
	DT_HERO_EQUIP_DATA_LST stEquipDataInfo; //武将装备信息
	DT_DRUG_DATA_LST stDrugDataInfo; //丹药信息
	DT_GODWEAPON_DATA stGodweaponInfo; //神器信息
	DT_SKILL_DATA_OLD stSkillInfo; //技能信息
	DT_STUDY_DATA    astStudyInfo[MAX_HERO_STUDY_GRID_NUM]; //心得信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_DATA_OLD;


typedef struct tagDT_HERO_DATA_LIST_OLD{
	UINT16           wHeroNum; //武将数目
	DT_HERO_DATA_OLD     astHeroInfoLst[MAX_HERO_NUM]; //武将信息

	INT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);
	INT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);
}DT_HERO_DATA_LIST_OLD;

#pragma pack(pop)
#endif
