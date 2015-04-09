
#ifndef _BUILDPROPMGR_H_
#define _BUILDPROPMGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <common/client/commondef.h>
#include <protocol/server/protocligs.h>
#include <logic/base/basepropmgr.h>

using namespace SGDP;
using namespace std;

namespace SGDP
{
    class CSDXMLNode;
}

//战斗奖励
class SBattleEncourageProp
{
public:
    SBattleEncourageProp()
    {
        Clear();
    }
    VOID Clear()
    {
        dwCoin  = 0 ;
        dwExperience = 0;
        dwStory = 0;
        dwJingJie = 0;
    }
public:
    UINT32          dwCoin;
    UINT32			dwExperience;//经验,每个武将都一样
    UINT32          dwStory;
    UINT32          dwJingJie;
};


//单体怪配置
struct SSingleMonsterProp
{
    UINT16 wMonsterID;//怪物KindID组合
    UINT8  byFormationIdx;//整型序号
};

//整体怪配置
struct SWholeMonsterProp
{
    vector<SSingleMonsterProp> vecMonster;//怪物KindID组合，第一个为主怪

    SWholeMonsterProp()
    {
        vecMonster.clear();
    }
};

typedef HashMap<UINT8, UINT8> CUnlockBuildKindMap;//key/value为建筑类型
typedef CUnlockBuildKindMap::iterator CUnlockBuildKindMapItr;

typedef HashMap<UINT16, DT_INSTANCE_ID> CUnlockEliteInstanceIDMap;//key,key/value为建筑类型
typedef CUnlockEliteInstanceIDMap::iterator CUnlockEliteInstanceIDMapItr;


//武将解锁配置
struct SHeroUnlockProp
{
    UINT8  byCarryNum;//解锁携带个数(当前副本未配置取上次的配置，保证有值)
    UINT16 wUnlockHeroKindID[MAX_UNLOCK_HERO_NUM];//解锁武将类型，0为没有解锁
    SHeroUnlockProp()
    {
        byCarryNum = 0;
        for (int i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
        {
            wUnlockHeroKindID[i] = 0;
        }
        //memset(this, 0x00, sizeof(SHeroUnlockProp));
    }
};

#define  MAX_INSTANCE_NAME 64



//副本通关奖励配置
class SPassEncourageProp
{
public:
    SPassEncourageProp(): dwCoin(0), dwStory(0), dwExperience(0) {};
public:
    UINT32			 dwCoin;
    UINT32            dwStory;
    UINT32			 dwExperience;//经验,每个武将都一样
};

//每组物品信息
struct SItemGroupProp
{
    SItemGroupProp(): wFirstItemKindID(0), wFirstItemPileNum(0)
    {
        mapItem.clear();
        memset(&stBoxItem, 0, sizeof(stBoxItem));
    };

    CItemRateMap	mapItem;			//物品
    UINT16			wFirstItemKindID; //第一次通关给的物品,0表示没有首次必爆物品
    UINT16			wFirstItemPileNum;//第一次通关给的物品的叠加数
    UINT8			byFirstItemHitIdx;//第一次通关给的物品的在mapItem中的位置，从1开始，0表示未命中

    DT_ITEM_DATA_LIST_CLI stBoxItem;//配置通关转盘物品(内部宝箱展开),用于返回给客户端，不需每次都重新计算
};

typedef map<UINT8, SItemGroupProp> CItemGroupMap; //key为组序号，从1开始
typedef CItemGroupMap::iterator CItemGroupMapItr;



//副本通关宝箱配置
class SPassBoxProp
{
public:
    SPassBoxProp()
    {
        mapItemGroup.clear();
        memset(&stDT_BURST_ITEM_DATA, 0, sizeof(stDT_BURST_ITEM_DATA));
    };
public:
    CItemGroupMap	mapItemGroup; ///< 奖励的道具组及对应概率/个数,内部宝箱全部展开

    DT_BURST_ITEM_DATA stDT_BURST_ITEM_DATA;//配置的可能爆出的物品(内部宝箱不展开),用于返回给客户端，不需每次都重新计算
};


//副本开放的功能
struct SInstanceOpenFunc
{
    //vector<UINT8> mapOpenBuild;//开放到的所有建筑
    set<UINT8> setOpenBuild;//开放到的所有建筑
    vector<UINT16> vecOpenHero;//开放到的所有伙伴
    //map<UINT16, UINT16> mapOpenHero;//开放到的所有伙伴
    vector<UINT16> vecUnlockSkill;//开放到的所有技能
};


//每个副本配置信息
struct SInstanceProp
{
    UINT8	byIndex;	//副本序列(按顺序开放)
    string	strName; //副本名称
	string  strInstanceDesc;//副本描述
    tstring	strDispName; //副本显示名称
    UINT16	wEnterLevel;//进入等级要求
    UINT8	byRoundNum;//战斗回合数
    UINT8	byPercentageHp;//血量百分比
    SPassEncourageProp stPassEncourageProp; //副本通关奖励配置
    SPassBoxProp stPassBoxProp; //副本通关宝箱配置
    vector<SWholeMonsterProp*> vecWholeMonsterProp;//怪物配置
    CUnlockBuildKindMap			mapUnlockBuildKind;//解锁的建筑
    CUnlockEliteInstanceIDMap		mapUnlockEliteInstanceID;
    SHeroUnlockProp stHeroUnlockProp;//武将解锁信息
    DT_INSTANCE_ID stUnlockEliteInstanceID;//当前解锁的精英副本
    DT_INSTANCE_ID stUnlock2EliteInstanceID;//当前解锁到的精英副本
    UINT16 wUnlockSKillID;//解锁的技能ID,0表示没有
	UINT8 byBattleNum;//每天可挑战次数
	string strLeveReqDesc;//等级要求描述
	vector<DT_INSTANCE_ID> vecUnlockInstance;
	DT_INSTANCE_ID stParentInstance;
	string strParentReqDesc;//副本要求描述
	BOOL bIsMajor;//是不是精副本
	string strItemDesc;//掉落装备描述
	string strHeroDesc;//解锁英雄描述
	SPassEncourageProp stOutBoundEncourage; //外传副本通关奖励配置
	vector<UINT32> vecDisplayItem;//显示的物品ID

    UINT16 wMainMonsterKindID;//主要怪物ID，最后1波怪物的主怪
    UINT32 dwAllExperience; //副本爆出的总经验，用于返回给客户端
    SInstanceOpenFunc stInstanceOpenFunc;//该副本开放到的所有功能，用于初始化玩家数据时补全缺少/新增的功能
    UINT32 dwMaxMonsterPower; //最大的怪物战力

    SInstanceProp()
    {
        wEnterLevel = 0;
        dwAllExperience = 0;
		byBattleNum = 0;
		strInstanceDesc ="";
		bIsMajor = FALSE;
		strLeveReqDesc = "";
		strParentReqDesc = "";
		strHeroDesc = "";
		strItemDesc = "";
		memset(&stParentInstance, 0, sizeof(DT_INSTANCE_ID));
		memset(&stOutBoundEncourage, 0, sizeof(SPassEncourageProp));
    }
    ~SInstanceProp();
};


typedef map<UINT16 , SInstanceProp*> CIndex2InstancePropMap;
typedef CIndex2InstancePropMap::iterator CIndex2InstancePropMapItr;

//城镇拥有的副本
struct STownInstanceProp
{
    UINT8 byTownIndex;
    CIndex2InstancePropMap m_mapInstance;
    UINT16 wOnhookLevel;
    ~STownInstanceProp();
};

typedef map<UINT16 , STownInstanceProp*> CIndex2TownInstancePropMap;
typedef CIndex2TownInstancePropMap::iterator CIndex2TownInstancePropMapItr;
//场景拥有的城镇
struct SSceneTownProp
{
    UINT16 wSceneIndex;
    CIndex2TownInstancePropMap m_mapTownInstance;
    ~SSceneTownProp();
};

typedef map<UINT16 , SSceneTownProp*> CIndex2SceneTownPropMap;
typedef CIndex2SceneTownPropMap::iterator CIndex2SceneTownPropMapItr;


//下一个副本类型
enum ENextInstanceType
{
    ENIT_NEXT_INSTANCE = 0,
    ENIT_NEXT_TOWN,
};

//下一个副本信息
struct SNextInstance
{
    BOOL bHaveNext;
    ENextInstanceType eNextInstanceType;
    DT_INSTANCE_DATA stDT_INSTANCE_DATA;
    SInstanceProp*	pstInstanceProp;
    SNextInstance()
    {
        bHaveNext = FALSE;
        memset(&stDT_INSTANCE_DATA, 0, sizeof(DT_INSTANCE_DATA));
        pstInstanceProp = NULL;
    }
};

//建筑解锁
typedef HashMap<UINT8, DT_INSTANCE_ID> CUnlockBuildKind2InstanceIDMap; //key为建筑类型
typedef CUnlockBuildKind2InstanceIDMap::iterator CUnlockBuildKind2InstanceIDMapItr;


//武将解锁
typedef HashMap<UINT32, SHeroUnlockProp> CInstanceID2HeroUnlockPropMap; //key为副本完整ID（DT_INSTANCE_ID)
typedef CInstanceID2HeroUnlockPropMap::iterator CInstanceID2HeroUnlockPropMapItr;


//主城等级解锁
typedef HashMap<UINT16, DT_INSTANCE_ID> CMainTownLevelUnlock2InstanceIDMap; //key为主城等级
typedef CMainTownLevelUnlock2InstanceIDMap::iterator CMainTownLevelUnlock2InstanceIDMapItr;


//爆出物品类型对应副本
typedef HashMap<UINT16,  DT_INSTANCE_ID> CBurstItemKind2InstanceIDMap;//key为道具kindid
typedef CBurstItemKind2InstanceIDMap::iterator CBurstItemKind2InstanceIDMapItr;

//爆出材料对应的外传副本
typedef std::vector<DT_INSTANCE_ID> InstanceVec; // 副本列表
typedef std::map<UINT16, InstanceVec> BurstItemID2InstancesMap; // <道具ID, 产出该道具的副本列表>

//解锁技能ID对应副本
typedef HashMap<UINT16,  DT_INSTANCE_ID> CUnlockSKillID2InstanceIDMap;//key为技能ID
typedef CUnlockSKillID2InstanceIDMap::iterator CUnlockSKillID2InstanceIDMapItr;

/**
 * @brief 属性管理器
 *
 * 提供了根据ID和名称查找属性的功能\n
 */

class CInstancePropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CInstancePropMgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_INSTANCE ;
    }
    BOOL Init();
    VOID UnInit();
public:
    CIndex2SceneTownPropMap& GetAllSceneInstance()
    {
        return m_mapIndex2SceneTownProp;
    };
	SSceneTownProp* GetSceneTownProp(UINT16 wSceneIdx);
    STownInstanceProp* GetSceneInstance(UINT16 wSceneIdx, UINT8 byTownIdx);
    SInstanceProp* GetInstanceProp(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);
    VOID	GetNextInstance(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, SNextInstance& stNextInstance);
    SHeroUnlockProp* GetUnlockHero(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);//获取解锁武将配置
    DT_INSTANCE_ID* GetBurstItemInstancdID(UINT16 wItemKindID);//获取物品产出的副本

    InstanceVec* GetDropInstanceListOfItem(UINT16 wItemKindID);//获取物品产出的副本列表

    vector<UINT16>& GetUnlockSkill(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);
    DT_INSTANCE_ID* GetUnlockSkillInstancdID(UINT16 wSkillID);//获取解锁技能的副本
	UINT8 GetMaxOutBoundTownIdx();
	UINT8 GetMaxOutBoundInstanceIdx(UINT8 byTownIdx);
	VOID GetNeiDanAllInstance(CPlayer* poPlayer, DT_NAIDAN_INSTANCE_LST_DATA& stInstanceLst);
	UINT8 GetMaxNaiDanInstanceIdx(){ return m_byMaxNeiDanInstanceIdx; }
public:
    CInstancePropMgr();
    virtual ~CInstancePropMgr();
protected:
    BOOL LoadFromXML();
    BOOL LoadFromData(STownInstanceProp* pstTownInstanceProp, const CHAR* pszData, const UINT32 dwDataLen);
	BOOL LoadOutBound();
	BOOL LoadNeiDanInstance();
	//读取奖励
	BOOL ReadEncourage(SInstanceProp* poInstanceProp,const string& strItemID, const string& strItemNum, const string& strItemRate, const string& strFirstFlag, const string& strDisPlayItem);
	//读取武将
	BOOL ReadHero(SInstanceProp* pstInstance, const string& strJson);
	//读取建筑
	BOOL ReadBuild(SInstanceProp* pstInstanceProp, const string& strJson);
	//读取怪物
	BOOL ReadMonster(SInstanceProp* pstInstanceProp, const string& strjson);
    //读取奖励
    BOOL ReadEncourage(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode);

    //读取怪物
    BOOL ReadMonster(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode);

	//读取外传配置
	BOOL ReadOutBound(SInstanceProp* pstInsanceProp, CSDXMLNode& xmlNode);

    //读取建筑
    BOOL ReadBuild(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode);

    //读取武将
    BOOL ReadHero(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode);//为了查找招募配置信息，招募限制(携带个数等)的配置信息必须按顺序！！！

    //读取解锁精英副本
    BOOL ReadEliteInstance(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode);

    //读取解锁技能ID
    BOOL ReadUnlockSkill(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode);

	//统计副本解锁记录
	VOID ReadInstanceUnlock();
protected:
    UINT32	ReadOneValue(EBurstKind eBurstKind, CSDXMLNode& xmlNode);//读取单个属性值
    BOOL	ReadEncourageItem(SPassBoxProp& stPassBoxProp, CSDXMLNode& xmlNode);//读取所有Item
    BOOL	ReadEncourageItemGroup(SPassBoxProp& stPassBoxProp, UINT8 byGroupIdx, CSDXMLNode& xmlNode);//读取Item组

    BOOL	InitBoxBurstItem(SInstanceProp* pstInstanceProp);//初始化配置的可能爆出的宝箱物品,用于返回给客户端，不需每次都重新计算
    BOOL	InitBurstExp(SInstanceProp* pstInstanceProp);//初始化配置的可能爆出的经验,用于返回给客户端，不需每次都重新计算
    BOOL	InitMaxPower(SInstanceProp* pstInstanceProp); //初始化最大怪物战力信息

private:
    CIndex2SceneTownPropMap			m_mapIndex2SceneTownProp;
    CUnlockBuildKind2InstanceIDMap	m_mapUnlockBuildKind2InstanceID;//建筑解锁map,建筑类型解锁对应的副本序号
    CInstanceID2HeroUnlockPropMap	m_mapInstanceID2HeroUnlockProp;//武将解锁，每读取1个副本该插入1跳记录，方便查找任何副本的解锁配置
    CBurstItemKind2InstanceIDMap	m_mapBurstItemKind2InstanceID;//爆出物品类型对应副本，只需记录最后产出该道具的。一般配置也不应该同种类型在不同副本产出

    BurstItemID2InstancesMap m_mapDropItemID2InstanceList; // 爆出材料对应的外传副本列表（同一材料可能在不同外传副本内出现）

    DT_INSTANCE_ID	m_stInstanceIDTmp;//用于临时保存读取配置文件
    UINT8		m_byLastCarryNum;//解锁携带个数,用于临时保存读取配置文件,没有配置时使用上次记录
    DT_INSTANCE_ID m_stLastEliteInstanceID;//解锁到的精英副本,用于临时保存读取配置文件,没有配置时使用上次记录
    SInstanceProp* m_pstLastInstanceProp;//上一个副本，用于临时保存读取配置文件
    CUnlockSKillID2InstanceIDMap m_mapUnlockSKillID2InstanceID;// 解锁技能ID对应的副本
    UINT8		m_byCurHeroIndex; //武将的索引

	UINT8 m_byMaxNeiDanInstanceIdx;
    UINT8	m_byRoundNum;//战斗回合数
    UINT8	m_byPercentageHp;//血量百分比
};

#endif
