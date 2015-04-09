
#ifndef _COURAGEPROPMGR_H
#define _COURAGEPROPMGR_H


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdobjectpool.h>
#include <map>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>
#include <sdxmlfile.h>
#include "logic/base/jsonencourage.h"
#include "logic/instance/instancepropmgr.h"
using namespace SGDP;

//活动时间配置
struct SCourageActivityTimeProp
{
    UINT8	byActivityKind;
    UINT8	byStartHour; //活动开始时间小时
    UINT8	byStartMin; //活动开始时间分
    UINT16	wKeepTime; //活动持续时间，单位秒

    UINT32	dwStartTime; //开始时间(byStartHour+byStartMin的秒数)
    UINT32	dwEndTime; //结束时间(dwStartTime+wKeepTime)
    UINT16  wMaxGroupNum; //客户端接收的队伍数量
};

//通关奖励配置
struct SCouragePassEncourageProp
{
    UINT16   wLevel;      //队伍平均等级
    UINT32   dwMedal;     //勇气勋章
    UINT32   dwStory;     //阅历
    //UINT32   dwCoin;      //铜钱
    //UINT32   dwExperience;//经验,每个武将都一样
    //UINT32   dwGold;      //金币
    //UINT32   dwBlueGas;   //蓝气
};

typedef struct _tagCourageEncourage
{
    _tagCourageEncourage()
    {
        wLevel = 0;
    }
    UINT16   wLevel;
    CJsonResItemVectorInfo	vecJsonEncourageItemInfo;
} SCourageEncourage;

typedef vector<SCourageEncourage> CCouragePropVector; 
typedef CCouragePropVector::iterator CCouragePropVectorItr;
typedef CCouragePropVector::reverse_iterator CCouragePropVectorRItr;


//怪物组配置信息
struct SGroupMonsterProp
{
    UINT16	wLevel;//进入等级要求
    UINT8	byRoundNum;//战斗回合数
    UINT8	byPercentageHp;//血量百分比
    vector<SWholeMonsterProp*> vecWholeMonsterProp;//怪物配置
    UINT16 wMainMonsterKindID;//主要怪物ID，最后1波怪物的主怪
    UINT32 dwMaxMonsterPower; //最大的怪物战力

    SGroupMonsterProp()
    {
        wLevel = 0;
    }
    //~SGroupMonsterProp();
};
//以下旧配置
struct SPassCourageEncourageProp
{
    UINT32	 dwCoin;
    UINT32   dwStory;
    UINT32   dwExperience;//经验,每个武将都一样
};

//每组物品信息
struct SItemCourageGroupProp
{
    SItemCourageGroupProp(): wFirstItemKindID(0), wFirstItemPileNum(0)
    {
        mapItem.clear();
        memset(&stCourageBoxItem, 0, sizeof(stCourageBoxItem));
    };

    CItemRateMap	mapItem;			//物品
    UINT16			wFirstItemKindID; //第一次通关给的物品,0表示没有首次必爆物品
    UINT16			wFirstItemPileNum;//第一次通关给的物品的叠加数
    UINT8			byFirstItemHitIdx;//第一次通关给的物品的在mapItem中的位置，从1开始，0表示未命中

    DT_ITEM_DATA_LIST_CLI stCourageBoxItem;//配置通关转盘物品(内部宝箱展开),用于返回给客户端，不需每次都重新计算
};

typedef map<UINT8, SItemCourageGroupProp> CItemCourageGroupMap; //key为组序号，从1开始
typedef CItemCourageGroupMap::iterator CItemCourageGroupMapItr;

//副本通关宝箱配置
struct SPassCourageBoxProp
{
    CItemCourageGroupMap	mapItemGroup; ///< 奖励的道具组及对应概率/个数,内部宝箱全部展开
    DT_BURST_ITEM_DATA stDT_BURST_ITEM_DATA;//配置的可能爆出的物品(内部宝箱不展开),用于返回给客户端，不需每次都重新计算
};

//配置信息 暂时只有奖励 怪物没配  
struct SCourageInfoProp
{
    UINT16	wLevel;	//序列
    SPassCourageEncourageProp stPassCourageEncourageProp; //副本通关奖励配置
    SPassCourageBoxProp stPassCourageBoxProp; //副本通关宝箱配置
};

typedef map<UINT16, SCourageInfoProp> CCourageInfoPropMap;
typedef CCourageInfoPropMap::iterator CCourageInfoPropMapItr;

typedef map<UINT8, SCourageActivityTimeProp> CCouragePropMap;
typedef CCouragePropMap::iterator CCouragePropMapItr;

typedef map<UINT32, SCourageActivityTimeProp> CCourageActivityTimePropMap;//key为dwEndTime
typedef CCourageActivityTimePropMap::iterator CCourageActivityTimePropMapItr;
typedef CCourageActivityTimePropMap::const_iterator CCourageActivityTimePropMapCItr;

typedef map<UINT16, SCouragePassEncourageProp> SCouragePassEncouragePropMap; //key为等级
typedef SCouragePassEncouragePropMap::iterator SCouragePassEncouragePropMapItr;

typedef map<UINT16, SCourageEncourage> SCourageEncouragePropMap; //key为等级
typedef SCourageEncouragePropMap::iterator SCourageEncouragePropMapItr;

typedef HashMap<UINT16 , SGroupMonsterProp*> CGroupMonsterPropMap;
typedef CGroupMonsterPropMap::iterator CGroupMonsterPropMapItr;

class CCouraPropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CCouraPropMgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_COURAGE ;
    }

    BOOL Init();

    VOID UnInit();
public:

    const SCourageActivityTimeProp* GetCouragePropByID(UINT8 byActivityId)
    {
        CCouragePropMapItr itr = m_mapCouragProp.find(byActivityId);
        if (itr != m_mapCouragProp.end())
        {
            return &itr->second;
        }
        else
        {
            return NULL;
        }
    }
    const CCourageActivityTimePropMap& GetCourageActivityTimePropMap()
    {
        return m_mapCourageActivityTimeProp;
    }
    const SCourageActivityTimeProp* GetCourageActivityTimeProp(UINT8 byActivityKind)
    {
        CCourageActivityTimePropMapItr itr = m_mapCourageActivityTimeProp.find(byActivityKind);
        if (itr != m_mapCourageActivityTimeProp.end())
        {
            return &itr->second;
        }
        else
        {
            return NULL;
        }
    }
    const SCourageActivityTimeProp* GetActivityTimeProp(UINT64 qwEnterTime = SDTimeSecs()); //获取进入时间处在的活动时间段配置，若当前不在活动时间内则返回NULL
    const CCourageActivityTimePropMap& GetCourageActivityStartTimeProp() { return m_mapCourageActivityTimeProp; }
    const CCourageActivityTimePropMap& GetCourageActivityEndTimeProp() { return m_mapCourageActivityEndTimeProp; }
    const SCourageActivityTimeProp* GetNextActivityTimeProp(UINT64 qwStartTime = SDTimeSecs()); //获取指定时候后开始的活动时间段配置, 当天的已经没有活动则返回明天的第一个活动
    const UINT16 GetMaxGroupNum(){return wMaxGroupNum;}
    SCourageInfoProp* GetCourageInfoProp(UINT16 wLevel);
    SCouragePassEncourageProp* GetCourageAwardProp(UINT16 wLevel);
    SGroupMonsterProp* GetCourageMonsterProp(UINT16 wLevel);
public:
    CCouraPropMgr();
    virtual ~CCouraPropMgr();
protected:

    BOOL LoadActivityTimeFromDB();
    //BOOL LoadBasePropFromDB();
    BOOL LoadAwardFromDB();
    BOOL LoadCourageAwardFromDB();
    BOOL LoadCourageMonsterFDB();
private:
    CCouragePropMap             m_mapCouragProp;
    CCourageActivityTimePropMap	m_mapCourageActivityTimeProp;
    CCourageActivityTimePropMap	m_mapCourageActivityEndTimeProp;
    CCourageInfoPropMap         m_mapCourageInfoProp;    // 奖励信息 怪物信息
    SCouragePassEncouragePropMap m_mapCouragePassEnProp;
    SCourageEncouragePropMap    m_mapCourageEncourageProp;
    CGroupMonsterPropMap        m_mapGroupMonsterProp;
    UINT16                      wMaxGroupNum; 
    UINT32	ReadOneValue(EBurstKind eBurstKind, CSDXMLNode& xmlNode);//读取单个属性值
    BOOL	ReadEncourageItem(SPassCourageBoxProp& stPassBoxProp, CSDXMLNode& xmlNode);//读取所有Item
    //BOOL	ReadEncourageItemGroup(SPassBoxProp& stPassBoxProp, UINT8 byGroupIdx, CSDXMLNode& xmlNode);//读取Item组
};

#endif // 
