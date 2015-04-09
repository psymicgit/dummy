
#ifndef _CHALLENGEPROP_H_
#define _CHALLENGEPROP_H_

#include <sdtype.h>
#include <sdhashmap.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>
#include <logic/base/basedefine.h>

using namespace SGDP;

//道具概率
struct SCItemRate
{
    UINT16	wRate;//爆出概率(万分比)
};
typedef map<UINT16, UINT16> CItemCRateMap;//key为道具kindid，value为SItemRate
typedef CItemCRateMap::iterator CItemCRateMapItr;

// 求仙露配置
struct SChallengeProp
{
    UINT16   wMylevel; // 当前笑脸数量
    UINT16   wEnemyLevel;

    CItemCRateMap		mapComposeItemWin;
    CItemCRateMap		mapComposeItemLose; 
    SChallengeProp()
    {
        wMylevel = 0;
        wEnemyLevel = 0;
        mapComposeItemLose.clear();
        mapComposeItemWin.clear();
    }
};


typedef HashMap<UINT16, SChallengeProp> CChallengePropMap; // 
typedef CChallengePropMap::iterator CChallengePropMapItr;



class CChallengePropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CChallengePropMgr);

public:
    virtual EConfigModule GetConfigModuleType() { return ECM_CHALLERGE; }

    BOOL Init();

    VOID UnInit();

public:	
    UINT16  RateItemID(UINT16 wMyLevel, UINT16 wEnemyLevel, UINT8 byWin);
public:
    CChallengePropMgr();
    virtual ~CChallengePropMgr();

protected:
    BOOL LoadChallengePropFromDB();

private:
    CChallengePropMap m_mapChallengePropMap; 
};

#endif // 

