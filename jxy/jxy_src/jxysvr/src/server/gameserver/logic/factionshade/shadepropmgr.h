#ifndef _SHADEPROPMGR_H_
#define _SHADEPROPMGR_H_

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

struct SUpgradeMsg
{
    UINT32 dwUpgradeValue;
    TCHAR  aszUpgradeMsg[32];
};

struct SHuntingReward
{
    UINT32 dwMonsterType;
    UINT32 dwRewardType;
    UINT32 dwRewardValue;
    UINT32 dwOutProbability;
};

struct SHuntingMonsterProp
{
    UINT32 dwMonsterType;
    TCHAR aszMonsterTypeName[NORMAL_MSG_LEN];
    UINT32 dwProbability;
};

struct SHuntingPotNameProp
{
    UINT8 byScheduleLevel;
    TCHAR aszHuntingPotName[HUNTING_POT_NAME];
};

typedef map<UINT8, SHuntingPotNameProp> CMapHuntingPotName;
typedef CMapHuntingPotName::iterator CMapHuntingPotNameItr;

typedef map<UINT16, UINT8> CShadeGroupMap; //怪物ID为KEY， 站位为VALUE
typedef CShadeGroupMap::iterator CShadeGroupMapItr;

typedef map<UINT16, CShadeGroupMap> CShadeGroupIDMap;
typedef CShadeGroupIDMap::iterator  CShadeGroupIDMapItr;

struct SShadeUpgradeProp
{
    UINT32 dwShadeLevel;
    UINT32 dwNeedFactionFunds;
    UINT32 dwNeedFactionLevel;
    UINT8 byHuntingTask;
    UINT32 dwHuntingMonsterMaxNum;
    UINT32 dwHuntingMonsterSchedule;
};
typedef map<UINT32, SShadeUpgradeProp> SShadeUpgradePropMap;
typedef SShadeUpgradePropMap::iterator SShadeUpgradePropItr;
typedef multimap<UINT32, SHuntingReward> SHuntingRewardMultiMap;
typedef multimap<UINT32, SHuntingReward>::iterator SHuntingRewardMultiItr;
typedef pair<SHuntingRewardMultiItr, SHuntingRewardMultiItr> PairHuntingReward;
typedef map<UINT32, SHuntingMonsterProp> SHuntingMonsterPropMap;
typedef map<UINT32, SHuntingMonsterProp>::iterator SHuntingMonsterPropItr;


class CShadePropMgr : public CBasePopMgr
{
    DECLARE_SINGLETON_PROPMGR( CShadePropMgr );
public:
    CShadePropMgr()
    {
        m_bHuntingOpenFlag = FALSE;
    }
    ~CShadePropMgr() {}

protected:
    BOOL LoadMonsterTypePropFromDB();
    BOOL LoadRewardFromDB();
    BOOL LoadShadeUpgradeProp();
    BOOL LoadHuntingHotName();
public:
    TCHAR* GetHuntingPotName(UINT8 byScheduleLevel);
    BOOL GetShadeUpgradeProp(UINT32 dwShadeLevel, SShadeUpgradeProp& stShadeUpgradeProp);
    virtual EConfigModule GetConfigModuleType()
    {
        return ECM_SHADE;
    }
    SHuntingRewardMultiMap&  GetHuntingReward()
    {
        return m_multimapHuntingReward;
    }
    UINT32 GetHuntingMonsterPropNum()
    {
        return m_mapHuntingMonsterProp.size();
    }
    BOOL GetHuntingMonsterProp(UINT32 dwMonsterType, SHuntingMonsterProp& stHuntingMonsterProp);
    //当前暗部等级与任务总档数的关系
    UINT32 GetHuntingMaxSchedule(UINT32 dwShadeLevel);
    //参数1：上次品质
    //参数2：是否有放弃的怪
    UINT8 GetMonsterType(UINT8& byLastType, BOOL bChoice = FALSE);
    UINT8 GetMonsterType();
    BOOL Init();
    VOID UnInit();
    BOOL GetOpenFlag()
    {
        return m_bHuntingOpenFlag;
    }
    VOID SetOpenFlag(BOOL bFlag)
    {
        m_bHuntingOpenFlag = bFlag;
    }
private:
    BOOL m_bHuntingOpenFlag;
    SHuntingRewardMultiMap m_multimapHuntingReward;
    SHuntingMonsterPropMap m_mapHuntingMonsterProp;
    SShadeUpgradePropMap  m_mapShadeUpgradeProp;
    CMapHuntingPotName m_mapHuntingPotName;
    CRandomVec m_vecRandom;
    CShadeGroupIDMap m_mapGroupID2ShadeGrpup;

};

#endif