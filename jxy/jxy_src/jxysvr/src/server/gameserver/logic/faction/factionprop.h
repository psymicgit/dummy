
#ifndef _FACTORPROP_H_
#define _FACTORPROP_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <string>
#include <protocligs.h>
#include "common/client/commondef.h"
#include "logic/base/basedefine.h"
#define ICON_CROWN 4
#define ICON_SUN 3
#define ICON_MOON 2
#define ICON_STAR 1

//活动配置
struct SShadeTaskInfoProp
{
    UINT32 dwTaskID;		//活动ID
    TCHAR  aszTaskName[16];	//活动名称
    UINT8	byStartHour; //活动开始时间小时
    UINT8	byStartMin; //活动开始时间分
    UINT16	wKeepTime; //活动持续时间，单位秒

    UINT32	dwStartTime; //开始时间(byStartHour+byStartMin的秒数)
    UINT32	dwEndTime; //结束时间(dwStartTime+wKeepTime)
    TCHAR   aszTaskDesc[TASK_DESCRIBE_MSG_LEN];
};

//任务奖励
struct STaskRewardProp
{
    UINT8 byShadeLevel;
    UINT8 byCollectiveTaskSchedule;			//档数
    UINT32 dwMaxIntegral;					//最大积分
    UINT8 byRewardType;						//物品类型
    UINT32 dwReardValue;					//值
};

struct SFactionUpgradeProp
{
    UINT32 dwFactionLevel;
    UINT32 dwNeedFactionFunds;
    UINT32 dwFactionPlayerMaxNum;
    UINT8 byRareitemMark;
    UINT8 byGymnasiumMark;
    UINT8 byShadeMark;
    UINT8 byHotSpringMark;
    UINT8 byColdPrison;
    UINT8 byFactionTower;
    UINT8 byCoinTimeByDay;
};
//捐献奖励
struct SDonateRewardProp
{
    UINT8 byFactionLevel;
    UINT8 byRewardType;
    UINT32 dwRewardValue;
    UINT8 byNum;
};

struct SFactinJobNumProp
{
    UINT8 byFactionLevel;
    UINT8 byMartialHead;
    UINT8 byProxyMartialHead;
    UINT8 byElder;
    UINT8 byCustodian;
    UINT8 byElite;
    UINT8 byNormal;
};

struct SDoorsTributeProp
{
	UINT16 wDoorsTributeLevel;
	UINT64 qwDoorsTributeValue;
};

typedef map<UINT64, UINT16> CMapDoorsTributeProp;
typedef CMapDoorsTributeProp::iterator CMapDoorsTributePropItr;

typedef map<UINT8 , SFactinJobNumProp> CMapFactionJobNum;
typedef CMapFactionJobNum::const_iterator CMapFactionJobNumItr;

typedef multimap<UINT8, SDonateRewardProp> CMapDonateReward;
typedef CMapDonateReward::const_iterator CMapDonateRewardItr;
typedef map<UINT32, SFactionUpgradeProp> SFactionUpgradePropMap;
typedef SFactionUpgradePropMap::iterator SFactionUpgradePropItr;
typedef map<UINT32, SShadeTaskInfoProp> SShadeTaskInfoPropMap;
typedef map<UINT32, SShadeTaskInfoProp>::iterator SShadeTaskInfoPropItr;
typedef multimap<UINT8, STaskRewardProp> STaskRewardPropMap;
typedef multimap<UINT8, STaskRewardProp>::iterator STaskRewardPropItr;

class CFactorPropMgr
{
public:
    DECLARE_SINGLETON(CFactorPropMgr);

public:

    BOOL Init();

    VOID UnInit();

public:
    //铜钱每天最多可领取的次数
    UINT16	GetCoinMaxTimeEveryDay(UINT32 dwFactionLevel);

    //元宝每天最多可领取的次数
    UINT16	GetGoldMaxTimeEveryDay(UINT32 dwVIPLevel);

    string GetShadeTime()
    {
        return m_strShadeActiviTime;
    }
    UINT32 GetFactionPlayerMaxNum( UINT32 dwFactionLevel);
    BOOL GetTaskRewardProp(UINT8 byShadeLevel, UINT32 dwSchedule, STaskRewardProp& stTaskRewardProp);
    BOOL GetTaskInfoProp(UINT32 dwTaskID, SShadeTaskInfoProp& stShadeTaskInfo) ;
    BOOL GetFactionUpgradeProp(UINT32 dwFactionLevel, SFactionUpgradeProp& stFactionUpgrade);
    VOID InitEvent();
    const STaskRewardPropMap& GetTaskRewardMap()
    {
        return m_mapTaskRewardProp;
    }
    const CMapDonateReward& GetDonateReward()
    {
        return m_mapDonateReward;
    }
    BOOL GetFactionJobNum(UINT8 byFactionLevel, SFactinJobNumProp& stProp);
    UINT8 GetUnLockLevel(em_Faction_Build emFactionType);
	VOID GetDoorsTributeInfo(UINT64 qwDoorsTribute, DT_DOORSTRIBUTE_LEVEL_DATA& stLevelData);
	VOID GetDoorsTributeInfo(UINT64 dwValue, UINT16& wLevel, UINT32& dwNextValue);
protected:
    CFactorPropMgr();
    virtual ~CFactorPropMgr();
protected:
    BOOL LoadTaskInfoPropFromDB();
    BOOL LoadTaskRewardPropFromDB();
    BOOL LoadFactionUpgradeFromDB();
    BOOL LoadGiveWineProp();
    BOOL LoadDonateRewardProp();
    BOOL LoadFactionJobNumProp();
	BOOL LoadDoorsTributeProp();
	BOOL LoadDoorsTributeLevelProp();
private:
    CMapDonateReward m_mapDonateReward;
    CMapFactionJobNum m_mapFactionJobNum;
    string m_strShadeActiviTime;

    SShadeTaskInfoPropMap m_mapShadeTaskInfoProp;
    STaskRewardPropMap m_mapTaskRewardProp;	//以ShadeLevel为KEY
    SFactionUpgradePropMap m_mapFactionUpgradeProp;
	CMapDoorsTributeProp m_mapDoorsTributeProp;
	C88Map m_mapDoorsTributeLevelProp;//星星月亮等级配置

    UINT8			m_byUnlockLevelByRareItem;
    UINT8			m_byUnlockLevelByGymnasium;
    UINT8			m_byUnlockLevelByShade;
    UINT8			m_byUnlockLevelByHotSpring;
    UINT8			m_byUnlockLevelByColdPrison;
    UINT8			m_byUnlockLevelByFactionTower;
};




#endif //#ifndef _ROLEMGR_H_

