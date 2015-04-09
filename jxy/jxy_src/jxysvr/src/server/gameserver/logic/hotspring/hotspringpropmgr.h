#ifndef _HOT_SPRING_PROP_MGR_
#define _HOT_SPRING_PROP_MGR_

#include <sdtype.h>
#include <sdhashmap.h>
#include <logic/base/basedefine.h>
#include <protocol/server/protocligs.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>
#include <map>
#include <protocol/server/protocommondata.h>
using namespace std;

//¥Õæ∆ ±º‰≈‰÷√
struct SGiveWineProp
{
	UINT8 byFactionLevel;
	UINT8 byTodayOpenNum;
	UINT32 dwIntegralTime;
	UINT32 dwKeepTime;
	UINT32 dwOpenCost;
};


typedef map<UINT8, DT_WINE_DATA_CLI> SWinePropMap;
typedef map<UINT8, DT_WINE_DATA_CLI>::iterator SWinePropItr;
typedef map<UINT8, SGiveWineProp> CMapGiveWineProp;
typedef CMapGiveWineProp::iterator CMapGiveWinePropItr;

class CHotSpringPropMgr : public CBasePopMgr
{
	DECLARE_SINGLETON_PROPMGR( CHotSpringPropMgr );
public:
	CHotSpringPropMgr(){}
	~CHotSpringPropMgr(){}

protected:
	BOOL LoadWinePropFromDB();
	BOOL LoadGiveWinePropFromDB();
public:
	SWinePropMap& GetWinePropMap(){ return m_mapWineProp; }
	BOOL GetGiveWinePropByFactionLevel(UINT8 byFactonLevel, SGiveWineProp& stProp);
	UINT32 GetSecond(UINT8 byHour, UINT8 byMin);
	virtual EConfigModule GetConfigModuleType() { return ECM_HOTSPRING; }
	BOOL Init();
	VOID UnInit();

private:
	CMapGiveWineProp m_mapGiveWineProp;
	SWinePropMap m_mapWineProp;

};
#endif