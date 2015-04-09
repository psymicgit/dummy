

#ifndef _PLAYERDIFF_H_
#define _PLAYERDIFF_H_

#pragma once

#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>
#include "common/server/utility.h"
#include <map>

using std::map;
using namespace SGDP;


class CPlayerDiff
{
public:
    CPlayerDiff();
    ~CPlayerDiff();

    const DT_PLAYER_NOPVP_DATA& GetLastData()
    {
        return m_stOld;
    }

    VOID SetStartData(const DT_PLAYER_NOPVP_DATA& stPlayerData);

    VOID StartDiff(const DT_PLAYER_NOPVP_DATA& stPlayerData);

    VOID EndDiff();

    BOOL HaveChanged(UINT8 eTableType, UINT16 wKey1, UINT16 wKey2, UINT16 wKey3);

protected:

    VOID ForamtDataMap(DT_PLAYER_NOPVP_DATA& stPlayerData, BOOL bOld);

    BOOL CkPlayerExt(UINT16 wKey);

    BOOL CkBuild(UINT8 byKey);

    BOOL CkHeroBase(UINT16 wKey);

    BOOL CkHeroExt(UINT16 wKey1, UINT16 wKey2);

    BOOL CkInstance(UINT16 wKey1, UINT8 byKey2, UINT8 byKey3);

    BOOL CkTask(UINT8 byKey);

private:
    map<UINT8, DT_BUILD_BASE_DATA*> m_mapOldBuild;
    map<UINT8, DT_BUILD_BASE_DATA*> m_mapCurBuild;

    map<UINT16, DT_HERO_DATA*> m_mapOldHero;
    map<UINT16, DT_HERO_DATA*> m_mapCurHero;

    map<UINT32, DT_INSTANCE_DATA*> m_mapOldInstance;
    map<UINT32, DT_INSTANCE_DATA*> m_mapCurInstance;

    map<UINT8, DT_TASK_DATA*> m_mapOldTask;
    map<UINT8, DT_TASK_DATA*> m_mapCurTask;

	map<UINT16, DT_JEWELRY_DATA_LST*> m_mapOldHeroJewelry;
	map<UINT16, DT_JEWELRY_DATA_LST*> m_mapCurHeroJewelry;

	map<UINT16, DT_HERO_SOUL_DATA*> m_mapOldHeroSoul;
	map<UINT16, DT_HERO_SOUL_DATA*> m_mapCurHeroSoul;

    DT_PLAYER_NOPVP_DATA	m_stOld;
    DT_PLAYER_NOPVP_DATA	m_stCur;
};

DECLARE_CLASS_POOL_MGR(CPlayerDiff)


class CFactionDiff
{
public:
    CFactionDiff();
    ~CFactionDiff();
    BOOL HaveChanged(const DT_FACTION_DATA &stDT_FACTION_DATA);

    BOOL Changed(const DT_FACTION_DATA &stDT_FACTION_DATA);
private:
    DT_FACTION_DATA		m_stDT_FACTION_DATA;
};

DECLARE_CLASS_POOL_MGR(CFactionDiff)

#endif

