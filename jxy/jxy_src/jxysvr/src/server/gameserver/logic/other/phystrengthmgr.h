
#ifndef _PHYSTRENGTHMGR_H_
#define _PHYSTRENGTHMGR_H_

#include "phystrength.h"


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>

#include <sdobjectpool.h>
#include <sdmutex.h>

//购买体力配置
struct  SBuyPhystrengthProp
{
    UINT8	byBuyPhyStrength;//购买的体力值
    //UINT8	byBuyMaxNumPerDay;//每天最多购买次数
};

class CPlayer;
class CPhystrengthMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CPhystrengthMgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_PHYSTRENGTH ;
    }

    BOOL Init();

    VOID UnInit();
public:
    UINT8 GetCommonPhyStrength()
    {
        return m_byCommonPhyStrength ;
    }
    UINT8 GetElitePhyStrength()
    {
        return m_byElitePhyStrength ;
    }
    UINT16 GetPhyStrengthRefreshCD()
    {
        return m_wPhyStrengthRefreshCD;
    }
    SBuyPhystrengthProp& GetBuyPhystrengthProp()
    {
        return m_stBuyPhystrengthProp;
    }
public:
    CPhystrengthMgr();
    virtual ~CPhystrengthMgr();
protected:

    BOOL LoadFromDB();


private:
    UINT8	m_byCommonPhyStrength;//进入普通副本所需体力
    UINT8	m_byElitePhyStrength;//进入精英副本所需体力
    UINT16	m_wPhyStrengthRefreshCD;//体力恢复1点所需时间(单位秒)
    SBuyPhystrengthProp m_stBuyPhystrengthProp;//购买体力配置
};

#endif // 
