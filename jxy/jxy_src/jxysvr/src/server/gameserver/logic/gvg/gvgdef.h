///<------------------------------------------------------------------------------
//< @file:   logic\gvg\gvgdef.h
//< @author: hongkunan
//< @date:   2014年1月23日 9:44:23
//< @brief:  门派跨服战的通用定义
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvgdef_h_
#define _gvgdef_h_

#include "commondef.h"
#include "protocol/server/protocommondata.h"
#include <map>
#include <set>
#include <vector>
#include "logic/base/basedefine.h"
#include <string>

// 前置声明
class CPKFaction;
class CPKZone;
class CGvGArena;
class CZvZYard;
class CZvvZYard;
class CZvZvZvZYard;
class CGvGPullGroup;
class CPullPlayer;
class CPlayer;
class CPullGroup;
class CFaction;

struct SGvGVideo;

// 自定义类型
typedef UINT32 GroupID;  // 组ID
typedef UINT32 PlayerID; // 玩家ID
typedef UINT32 ZoneID;   // 区ID

typedef UINT16 ArenaID; // 门派跨服战擂台ID
typedef UINT64 VideoID;

typedef std::set<VideoID> VideoIDSet;
typedef std::vector<DT_GVG_PK_VIDEO> SPvPVideoVec;
typedef std::map<UINT16, DT_GVG_PK_VIDEO> SPvPVideoMap; // 门派对战中的玩家录像map，键 = 场数，值 = 录像数据

typedef std::map<ZoneID, ArenaID> Zone2ArenaMap;

typedef std::map<ZoneID, CPKZone*> CPKZoneMap;
typedef std::map<UINT32, CPKFaction*> CPKFactionMap; // key = 门派ID

typedef std::vector<CPKZone*> PKZoneVec;

typedef std::vector<CPlayer*> CPlayerVec;
typedef std::set<PlayerID> CPlayerIDSet;
typedef std::map<PlayerID, ZoneID> Player2ZoneMap;

typedef std::map<ZoneID, CPlayerIDSet> ZoneToPlayersMap;

typedef std::vector<CGvGArena*> CGvGArenaVec;

typedef std::map<PlayerID, CPullPlayer*> CPullPlayerMap;    // 参战玩家列表
typedef std::map<GroupID, CPullGroup*> CPullGroupMap;       // 帮派vs怪物组map

typedef std::vector<CFaction*> CFactionVec; // 门派列表

typedef std::vector<UINT32> FactionIDVec; // 门派ID列表
typedef std::map<ZoneID, FactionIDVec> ZoneID2FactionIDMap; // 存储对应区的门派id列表

typedef std::map<UINT32, UINT32> FactionSupportMap; // 键 = 门派ID, 值 = 未同步到跨服战服务器的各门派押注数

typedef std::set<UINT32> FactionIDSet; // 门派ID列表
typedef std::map<UINT32, FactionIDSet> EachZoneFactionIDListMap; // 各区门派列表，键 = 区ID，值 = 对应区的门派ID列表

typedef std::map<UINT8, ZoneIDVec> GroupZoneIDMap; // key = 组ID, value = 该组对应的区列表

// 跨服战类别
enum EPullGroupType
{
    EPullType_GvG = 1, // 拉取玩家用来gvg淘汰赛
    EPullType_PvP = 2, // 拉取玩家用于pvp
};

// 当前跨服战处于哪个阶段
enum GVG_STAGE
{
    GVG_0_NOT_START = 0,     // 未开启阶段

    // 第一轮：各服报名
    GVG_1_APPLY,             // 报名阶段

    // 第二轮：本服第一轮（财力大比拼）
	GVG_2_MONEY,			 // 开始财力比拼阶段

    // 第三轮：本服第二轮（门派大乱斗）
    GVG_3_CHAOS_WAR_BEGIN,   // 进入门派大乱斗阶段
    GVG_3_CHAOS_WAR_END,     // 门派大乱斗已结束，公布结果

    // 第四轮：双服16进8淘汰赛
    GVG_4_16_TO_8_BEGIN,     // 进入16进8淘汰赛阶段
    GVG_4_16_TO_8_GO,        // 服务端内部开始进行16进8淘汰赛计算（开始禁止布阵）
    GVG_4_16_TO_8_END,       // 公布16进8获胜名单

    // 第五轮：双服8进4淘汰赛
    GVG_5_8_TO_4_BEGIN,      // 进入8进4淘汰赛阶段
    GVG_5_8_TO_4_GO,         // 服务端内部开始进行8进4淘汰赛计算（开始禁止布阵）
    GVG_5_8_TO_4_END,        // 公布8进4获胜名单

    // 第六轮：四服积分总决赛
    GVG_6_FINAL_WAR_BEGIN,   // 进入四服积分总决赛阶段
    GVG_6_FINAL_WAR_GO,      // 开始进行4服积分赛（开始禁止布阵）

    GVG_7_END,               // 本次跨服战已结束
    GVG_8_SLEEPING,          // 进入下一届的休眠阶段
    GVG_STAGE_MAX
};

// 门派和门派的对战结果
enum GvGResult
{
    GR_NOT_START, // 未开始
    GR_LEFT_WIN,  // 左边门派赢
    GR_RIGHT_WIN, // 右边门派赢
    GR_NO_ENEMY   // 轮空
};

namespace gvgutil
{
    // 获取指定阶段对应的轮次
    GvGRound Stage2Round(GVG_STAGE eStage);

    // 获取下一阶段
    GVG_STAGE GetNextStage(GVG_STAGE);

    // 查找指定轮次的门派战况
    DT_FACTION_GVG_SINGLE_LOG* FindGvGLog(DT_FACTION_GVG_LOG_LIST &stLogList, DT_GVG_ROUND &stRound);

    // 获取今天的凌晨0点，单位：秒
    UINT64 GetTodayZero();

    // 根据区号获取其所属的双服对战组ID
    BYTE GetGroupIDByZone(ZoneID zoneID, const DT_GROUP_ZONE_ID_LIST &stGroupZoneIDList);

    // 将传入的时间转成<时:分:秒>的格式，如传入3701，则返回<01:00:01>
    std::string TimeToDayHourMinuteSec(UINT32 dwTime);
}

#endif //_gvgdef_h_