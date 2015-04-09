
#ifndef _BOSSBATTLEROPMGR_H_
#define _BOSSBATTLEROPMGR_H_


#include <common/client/commondef.h>
#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>
#include <map>

using namespace std;

struct SBossBProp
{
	UINT8	byActivityId;		//活动ID(每天的不同时间ID)
	UINT16	wBossMonsterID;		//boss怪物ID
	UINT16	wBossStartLevel;	//boss起始等级
	UINT16	wBossMaxLevel;		//boss最大等级
	UINT8	byStartHour;		//开始时间小时
	UINT8	byStartMin;			//开始时间分	
	UINT16	wKeepTime;			//持续时间，单位秒
	
	UINT32	dwStartTime;		//开始时间
	UINT32	dwEndTime;			//结束时间	
	UINT32	dwStartEnterTime;	//开始可进入活动时间

	UINT16	wBirthPosX;			//出生点X
	UINT16	wBirthPosY;			//出生点Y
	UINT8	byShowNum;			//同屏显示的玩家个数
	UINT16	wReliveCD;			//复活CD
	UINT16	wGoldReliveCD;		//元宝复活操作CD
	UINT16	wReliveGold;		//复活元宝
	UINT16	wMoveMinCD;			//移动CD，单位毫秒	
	UINT16	wMoveMaxLen;		//每次移动最大长度，单位
	UINT16	wMaxReadyPosY;		//准备区最大Y坐标，单位
	UINT16	wReadyCD;			//准备CD，单位秒
	UINT16	wUpgradeCostMaxKillSecs; //boss升级最大击杀时间
	UINT32	wHPPerGrid;			//每个血格子血量
	UINT16  wIncCostStory;		//提升战力消耗阅历
	UINT16  wIncCostGold;		//提升战力消耗元宝
	UINT16  wCanIncPowerRate;	//可提升战力百分比
	UINT16	wDealNum;			//每次run处理个数
	UINT16	wDealInterval;		//处理间隔（单位毫秒）
	UINT16	wOpenActivityPassDay;//开服后几天开启活动
};

typedef map<UINT8, SBossBProp> CBossBPropMap;
typedef CBossBPropMap::iterator CBossBPropMapItr;

typedef map<UINT32, SBossBProp> CBossBActivityTimePropMap;//key为dwStartTime/dwEndTime
typedef CBossBActivityTimePropMap::iterator CBossBActivityTimePropMapItr;
typedef CBossBActivityTimePropMap::const_iterator CBossBActivityTimePropMapCItr;

class CBossBattlePropMgr : public CBasePopMgr
{
public:

	DECLARE_SINGLETON_PROPMGR(CBossBattlePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_BOSSB ; }

	BOOL Init();

	VOID UnInit();
public:
	SBossBProp* GetBossBPropByID(UINT8 byActivityId);
	CBossBPropMap& GetBossBPropMap() { return m_mapBossBProp; }
	const SBossBProp* GetActivityTimeProp(UINT64 qwEnterTime = SDTimeSecs()); //获取进入时间处在的活动时间段配置，若当前不在活动时间内则返回NULL
	const CBossBActivityTimePropMap& GetBossBStartTimeProp() { return m_mapBossBStartTimeProp; }
	const CBossBActivityTimePropMap& GetBossBEndTimeProp() { return m_mapBossBEndTimeProp; }
	const SBossBProp* GetNextActivityTimeProp(UINT64 qwStartTime = SDTimeSecs()); //获取指定时候后开始的活动时间段配置, 当天的已经没有活动则返回明天的第一个活动

public:
	CBossBattlePropMgr();
	virtual ~CBossBattlePropMgr();

protected:
	BOOL LoadBossBPropFromDB();
	VOID Clr();

private:

	CBossBPropMap m_mapBossBProp;
	CBossBActivityTimePropMap	m_mapBossBStartTimeProp;
	CBossBActivityTimePropMap	m_mapBossBEndTimeProp;

};

#endif // 
