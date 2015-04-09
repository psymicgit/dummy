#ifndef _TASKPROP_H
#define _TASKPROP_H

#include <map>
#include <sdtype.h>
#include <sdhashmap.h>
#include <sdobjectpool.h>
#include <logic/base/burstprop.h>
#include <logic/instance/instancepropmgr.h>
#include <common/client/commondef.h>

using namespace SGDP;
using namespace std;


class CPlayer;

//任务类型
typedef  UINT32 ETaskType;


//接受任务条件类型
enum EAcceptConditionType
{
	EACT_INVALID = 0, //无效
	EACT_PLAYER = 1, //玩家达到指定等级
	EACT_INSTANCE = 2, //玩家通过指定副本
	EACT_BUILD = 3, //玩家拥有指定建筑到指定等级
	EACT_MAINTASK = 4, //玩家完成指定主线任务
	EACT_MAX,		//最大
};

class CAcceptCondition;
class CFinishCondition;
typedef HashMap<UINT8, CAcceptCondition*> CAcceptType2CheckMap;
typedef CAcceptType2CheckMap::iterator CAcceptType2CheckMapItr;


//任务奖励配置
struct STaskEncourageProp
{
public:
	STaskEncourageProp();
	~STaskEncourageProp();
public:
	UINT32				dwCoin;
	UINT32				dwGold;
	UINT16				wPhyStrength;
	UINT32				dwScience;
	UINT32				dwFame;
	UINT32				dwStory;//经验,每个武将都一样
	UINT32				dwExperience;//经验,每个武将都一样
	UINT32				dwBlueGas; //蓝气
	UINT32				dwPurpleGas; //紫气
	UINT32				dwJingJie; //境界
	CItemKindID2NumMap	mapItemNum;//奖励的道具及对应个数
};

struct STaskProp;
typedef map<UINT32, STaskProp*> CTaskIdx2TaskPropMap;//key为任务序号dwTaskIdx,用按序map
typedef CTaskIdx2TaskPropMap::iterator CTaskIdx2TaskPropMapItr;
//任务属性
struct STaskProp
{
public:
	STaskProp();
	~STaskProp();
public:
	UINT8		byTaskLineIdx;//任务线序号
	UINT32		dwTaskIdx;//任务序号,必须按大小顺序，但不一定连续
	ETaskType	eTaskType;//任务类型
	tstring		strDispName;//任务显示名字
	tstring		strDesc;//任务简介
	CAcceptType2CheckMap mapAcceptCondition;//任务接受条件,可多个
	CFinishCondition*	 poFinishCondition;//任务完成条件
	STaskEncourageProp	 stTaskEncourageProp; //任务完成奖励	
	//CTaskIdx2TaskPropMap mapBranchTask;//支线任务,key从1开始，依次++
	//任务解锁副本，只有主线1配置
	BOOL				bNewUnlockInstanceID;//是否新解锁副本
	DT_INSTANCE_ID		stInstanceID;//当前任务解锁到的副本(主线任务才有)
	UINT32				dwNextTaskIdx;//下一个任务序号，0为该线任务没有了
	
	//辅助，读取配置时赋值
	UINT16			wAcceptPlayerLevel;//接受需要的玩家等级，用于返回给客户端，便于查找
	DT_INSTANCE_ID	stAcceptInstanceID;//接受需要通过的副本，用于返回给客户端，便于查找
};


//任务线属性配置
class CTaskLineProp
{
public:
	CTaskLineProp();
	~CTaskLineProp();
public:
	STaskProp*				GetNextTask(UINT32 dwTaskIdx); //获取dwTaskIdx的下一个任务，dwTaskIdx不一定有效，没有返回NULL
	STaskProp*				GetLastTask(UINT32 dwTaskIdx); //获取dwTaskIdx的上一个任务，dwTaskIdx一定有效
	UINT8					byTaskLineIdx;//任务线序号
	UINT32					dwFirstTaskIdx;//该任务线的第一个任务
	CTaskIdx2TaskPropMap	mapTaskProp;//该任务线的任务
};



#endif

