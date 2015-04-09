
#ifndef _WORSHIPPROPMGR_H_
#define _WORSHIPPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>
#include <map>

using namespace std;

struct SWorshipProp
{
	UINT8	byWorshipType;//膜拜类型
	UINT8	byWorshipTypeLevel;//膜拜级别

	UINT32	dwNeedCoin;//膜拜所需铜钱，0为不需要
	UINT32	dwNeedGold;//膜拜所需元宝，0为不需要
	UINT8	byNeedVipLevel;//膜拜所需VIP等级，0为不需要

	UINT32	dwEncJingJie; //膜拜可获得境界
	UINT32	dwEncPurpleGas; //膜拜可获得紫气
};

typedef map<UINT8, SWorshipProp> CLevel2WorshipPropMap;
typedef CLevel2WorshipPropMap::iterator CLevel2WorshipPropMapItr;

class CPlayer;
class CWorshipPropMgr: public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CWorshipPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_WORSHIP ; }

    BOOL Init();

    VOID UnInit();
public:
	SWorshipProp* GetWorshipProp(UINT8 byWorshipType, UINT8 byWorshipTypeLevel);
	CLevel2WorshipPropMap& GetRaceWorshipProp() { return m_mapRaceWorshipProp; }
	CLevel2WorshipPropMap& GetRDCWorshipProp() { return m_mapRDCWorshipProp; }
public:
    CWorshipPropMgr();
    virtual ~CWorshipPropMgr();
protected:

    BOOL LoadFromDB();

private:
	CLevel2WorshipPropMap	m_mapRaceWorshipProp;//竞技场膜拜
	CLevel2WorshipPropMap	m_mapRDCWorshipProp;//随机挑战膜拜
};

#endif // 
