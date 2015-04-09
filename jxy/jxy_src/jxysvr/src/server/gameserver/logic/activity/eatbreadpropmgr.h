
#ifndef _EATBREADPROPMGR_H
#define _EATBREADPROPMGR_H


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdobjectpool.h>
#include <map>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>

using namespace SGDP;

using namespace SGDP;

//吃包子活动时间配置
struct SEatBreadActivityTimeProp
{
	UINT8	byActivityKind;
	UINT8	byStartHour; //活动开始时间小时
	UINT8	byStartMin; //活动开始时间分	
	UINT16	wKeepTime; //活动持续时间，单位秒

	UINT32	dwStartTime; //开始时间(byStartHour+byStartMin的秒数)
	UINT32	dwEndTime; //结束时间(dwStartTime+wKeepTime)
};

typedef map<UINT32, SEatBreadActivityTimeProp> CEatBreakActivityTimePropMap;//key为dwEndTime
typedef CEatBreakActivityTimePropMap::iterator CEatBreakActivityTimePropMapItr;
typedef CEatBreakActivityTimePropMap::const_iterator CEatBreakActivityTimePropMapCItr;

class CEatBreadPropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CEatBreadPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_EATBREAD ; }

    BOOL Init();

    VOID UnInit();
public:
	inline UINT8	GetMaxObtainPhyStrengthen() { return m_byMaxObtainPhyStrengthen; }
	inline UINT32	GetMaxObtainCoin() { return m_dwMaxObtainCoin; }
	inline UINT16	GetTimePerPhyStrengthen () { return m_wTimePerPhyStrengthen; }
	inline UINT16	GetOarNumPerPhyStrengthen() { return m_wOarNumPerPhyStrengthen; }
	inline UINT32	GetOnceSpeedCoin() { return m_dwOnceSpeedCoin; }

	const SEatBreadActivityTimeProp* GetActivityTimeProp(UINT64 qwEnterTime = SDTimeSecs()); //获取进入时间处在的活动时间段配置，若当前不在活动时间内则返回NULL
	const CEatBreakActivityTimePropMap& GetEatBreadStartTimeProp() { return m_mapEatBreadStartTimeProp; }
	const CEatBreakActivityTimePropMap& GetEatBreadEndTimeProp() { return m_mapEatBreadEndTimeProp; }
	const SEatBreadActivityTimeProp* GetNextActivityTimeProp(UINT64 qwStartTime = SDTimeSecs()); //获取指定时候后开始的活动时间段配置, 当天的已经没有活动则返回明天的第一个活动
public:
    CEatBreadPropMgr();
    virtual ~CEatBreadPropMgr();
protected:

    BOOL LoadActivityTimeFromDB();
	BOOL LoadBasePropFromDB();

private:	
	UINT8	m_byMaxObtainPhyStrengthen; //最大获得体力
	UINT32	m_dwMaxObtainCoin; //最大获得铜币
	UINT16	m_wTimePerPhyStrengthen; //多久可获得1点体力
	UINT16	m_wOarNumPerPhyStrengthen; //划几次可获得1点体力
	UINT32	m_dwOnceSpeedCoin; //1倍数可获得铜钱

	CEatBreakActivityTimePropMap	m_mapEatBreadStartTimeProp;
	CEatBreakActivityTimePropMap	m_mapEatBreadEndTimeProp;

};

#endif // 
