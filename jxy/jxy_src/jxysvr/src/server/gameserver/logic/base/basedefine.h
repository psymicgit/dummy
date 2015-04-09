
#ifndef _BASEDEFINE_H_
#define _BASEDEFINE_H_

#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include <map>
#include <set>

using namespace std;

#define VERSION_GVG_ACTIVITY 312 // 跨服门派战所属的版本
#define VERSION_SOUL 312         // 战魂所属的版本
#define VERSION_FLY 410         // 飞升系统所属的版本
#define VER_PET      402
#define VERSION_PET      402

#define BAG_GIFT_ID 0xFFFFFFFFFFFFFFFF

//数据保存状态，用于玩家数据保存
enum ESaveState
{
	ESS_NON_SAVE = 0 ,//不需保存
	ESS_WANT_SAVE = 1 ,//需要保存
	ESS_SAVEING = 2 ,//保存中
	//ESS_SAVEING_DIRTY = 3 ,//保存中又被修改
};


//机器人功能
enum ERobotFuncKind
{
	ERFK_PLAYER	=	0,//普通玩家数据
	ERFK_PLUNDER = 1,//用于掠夺
	ERFK_RACE = 2,//用于竞技场
	ERFK_OTHER = 99,//其他
};


//最大体力上限
#define MAXPHYSTRENGTH	48

typedef HashMap<UINT16, UINT16> CItemKindID2NumMap;//key为道具kindid，value为对应的个数
typedef CItemKindID2NumMap::iterator CItemKindID2NumMapItr;


//道具概率
struct SItemRate
{
	UINT16	wItemNum;//道具个数
	UINT16	wRate;//爆出概率(万分比)
};
typedef HashMap<UINT16, SItemRate> CItemRateMap;//key为道具kindid，value为SItemRate
typedef CItemRateMap::iterator CItemRateMapItr;


//全属性
struct SAllAttrProp
{
	SAllAttrProp()
	{
		memset(&stBattleAttr, 0, sizeof(stBattleAttr));
		memset(&stHideBattleAttr, 0, sizeof(stHideBattleAttr));
		memset(&stExtBattleAttr, 0, sizeof(stExtBattleAttr));
	};

	DT_BATTLE_BASE_ATTRIBUTE			stBattleAttr;
	DT_HIDE_BATTLE_ATTRIBUTE	stHideBattleAttr;
	DT_EXT_BATTLE_ATTRIBUTE		stExtBattleAttr;
};

//用于记录上次推荐的玩家ID
struct SRecommendPlayerRcd
{
	UINT32 dwPlayerH1;//高1级的
	UINT32 dwPlayerE;//相同等级的
	UINT32 dwPlayerL1;//低1级的
};


typedef map<UINT32, UINT32> C3232Map;
typedef C3232Map::iterator C3232MapItr;
typedef C3232Map::reverse_iterator C3232MapRitr;

typedef map<UINT16, UINT16> C1616Map;
typedef C1616Map::iterator C1616MapItr;

typedef map<UINT8, UINT8> C88Map;
typedef C88Map::iterator C88MapItr;

typedef multimap<UINT16, UINT16> C1616MultiMap;
typedef C1616MultiMap::iterator C1616MultiMapItr;

typedef vector<UINT32> C32Vec;
typedef C32Vec::iterator C32VecItr;

typedef set<UINT32> C32Set;
typedef C32Set::iterator C32SetItr;

typedef set<UINT64> C64Set;
typedef C64Set::iterator C64SetItr;

typedef set<UINT8> C8Set;
typedef C8Set::iterator C8SetItr;

typedef set<UINT16> C16Set;
typedef C16Set::iterator C16SetItr;

typedef map<string, UINT32> CStr32Map;
typedef CStr32Map::iterator CStr32MapItr;

typedef map<UINT32, C3232Map> C3232MapM;
typedef C3232MapM::iterator C3232MapMItr;
typedef C3232MapM::reverse_iterator C3232MapMIitr;

typedef map<UINT32, UINT64> C3264Map;
typedef C3264Map::iterator C3264MapItr;
typedef C3264Map::reverse_iterator C3264MapRitr;

typedef map<UINT64, UINT64> C6464Map;
typedef C6464Map::iterator C6464MapItr;
typedef C6464Map::reverse_iterator C6464MapRitr;

// 区ID列表
typedef std::vector<UINT16> ZoneIDVec;
typedef std::set<UINT16> ZoneIDSet;

#endif

