
#ifndef _HPGRIDMGR_H_
#define _HPGRIDMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <map>
#include <logic/base/basepropmgr.h>

using namespace std;

typedef map<UINT16, UINT32> CLevel2PerGridHPMap;
typedef CLevel2PerGridHPMap::iterator CLevel2PerGridHPMapItr;


class CHPGridMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CHPGridMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_HPGRID ; }

    BOOL Init();

    VOID UnInit();
public:
	VOID SetHPGridNum(DT_BATTLE_OBJ_GROUP_DATA3& stMyselfInfo, DT_BATTLE_OBJ_GROUP_DATA3& stEnemyInfo, map<UINT16, UINT16>& mapBossMonsterID);
	VOID SetGodweaponHPGridNum(DT_BATTLE_OBJ_GROUP_DATA2& stMyselfInfo, DT_BATTLE_OBJ_GROUP_DATA2& stEnemyInfo, map<UINT16, UINT16>& mapBossMonsterID);
public:
    CHPGridMgr();
    virtual ~CHPGridMgr();
protected:

    BOOL LoadFromDB();

	UINT8	GetHPGridNum(UINT16 wMaxLevel, UINT32 dwHPPerGrid, UINT32 dwHP);

private:
	CLevel2PerGridHPMap	m_mapLevel2PerGridHP;
	CLevel2PerGridHPMap	m_mapGodweaponQuality2PerGridHP;
	UINT16				m_wMaxPlayerLevel;
	UINT8				m_byMaxGridNum;
};

#endif // 
