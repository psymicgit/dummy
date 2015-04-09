
#ifndef _AWAKENBASEPROPPROPMGR_H_2
#define _AWAKENBASEPROPPROPMGR_H_2

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>
#include <logic/base/basedefine.h>
#include <protocommondata.h>
#include <set>
using namespace SGDP;
using namespace std;


#define GOLD_RUDING_PROP		110		//悟道等级配置元宝悟道入定状态配置
#define GOLD_CHEWU_PROP			111		//悟道等级配置元宝悟道彻悟状态配置

typedef set<UINT16> CSetResolveItemID;
typedef CSetResolveItemID::iterator CSetResolveItemIDItr;
//基本配置
struct SAwakenBaseProp2
{
	UINT32	dwGoldAwaken; //元宝悟道进入入定状态所需元宝
	UINT32	dwGenStudyCoin; //悟道心得获得铜钱时的铜钱
	UINT32	dwGenStudyClip;	//悟道心得获得心得碎片时的心得碎片
	UINT16	wGenStudyClipLevel;	// 获得心得碎片所需玩家等级
	UINT8	byMaxStudyLevel; //心得最大等级
	UINT8	byCallStudyColorKind;	// 召唤赠送心得的颜色类型
	UINT8	byCallStudyAttrKind1;	// 召唤赠送心得的属性类型1
	UINT8	byCallStudyAttrKind2;	// 召唤赠送心得的属性类型2
	UINT8	byCallStudyLevel;		// 召唤赠送心得的等级
};

//悟道等级配置
struct SAwakenLevelProp2
{
	UINT16	wStartPlayerLevel;		// 玩家起始等级
	UINT8	byAwakenLevel;			// 悟到道别(从1开始，共5级(瞌睡/神游/冥思/入定/彻悟), (配置中110为元宝悟道入定状态配置，111为彻悟)
	UINT32	dwNeedCoin;				// 悟道所需铜币
	UINT8	byNextLevelRate;		// 跳到下一级概率(失败则返回第一级)
	UINT8	byCoinRate;				// 铜钱概率
	UINT8	byClipRate;				// 碎片概率
	UINT8	byBuleStudyRate;		// 蓝色心得碎片概率
	UINT8	byPurpleStudyRate;		// 紫色心得碎片概率
	UINT8	byRedStudyRate;			// 红色心得碎片概率
	UINT8	byOrangeStudyRate;		// 橙色心得碎片概率

	CRandomVec vecRandom; //心得概率,按颜色插入顺序
};


//心得生成概率及解锁配置(该等级有则表示已解锁)
struct SAwakenStudyGenUnlockProp2
{
	UINT16	wPlayerLevel;				// 玩家等级
	UINT8	byStudyColorKind;			// 心得颜色类型
	UINT8	byStudyAttrKind1;			// 心得属性类型1
	UINT8	byStudyAttrKind2;			// 心得属性类型2
	UINT8	byGenRate;					// 生成概率
};
typedef vector<SAwakenStudyGenUnlockProp2> CAwakenStudyGenUnlockPropVec2; 
//玩家每等级心得生成概率及解锁配置
struct  SPlayerAwakenStudyGenUnlockProp2
{
	UINT16 wPlayerLevel;
	CAwakenStudyGenUnlockPropVec2 avecAwakenStudyGenUnlockProp[EC_ORANGE+1]; //4种颜色的配置,下标为颜色
	CRandomVec avecRandom[EC_ORANGE+1]; //4种颜色生成概率，按vecAwakenStudyGenUnlockProp插入顺序
};
//心得生成概率及解锁配置(该等级有的心得则表示已解锁)
typedef HashMap<UINT16, SPlayerAwakenStudyGenUnlockProp2> CLevel2UnlockStudyColorProMap2; //key为玩家等级wStartPlayerLevel
typedef CLevel2UnlockStudyColorProMap2::iterator CLevel2UnlockStudyColorProMapItr2;


//心得属性配置
struct SAwakenStudyAttrProp2
{
	UINT8	byStudyColorKind;			// 心得颜色类型
	UINT8	byStudyAttrKind1;			// 心得属性类型1
	UINT8	byStudyAttrKind2;			// 心得属性类型2
	UINT8	byStudyLevel;				// 心得等级
	UINT32	dwAttrValue1;				// 心得属性值1	
	UINT32	dwAttrValue2;				// 心得属性值2	
	UINT32	dwHaveExp;					// 拥有的经验
};
typedef HashMap<UINT32, SAwakenStudyAttrProp2> CAwakenStudyAttrPropMap2; //key为byStudyColorKind+byStudyAttrKind1/2+byStudyLevel
typedef CAwakenStudyAttrPropMap2::iterator CAwakenStudyAttrPropMapItr2;


//心得碎片兑换心得配置
struct SAwakenClipToStudyProp2
{
	UINT8	byStudyColorKind;			// 心得颜色类型
	UINT8	byStudyAttrKind1;			// 心得属性类型1
	UINT8	byStudyAttrKind2;			// 心得属性类型2
	UINT8	byStudyLevel;				// 心得等级
	UINT32	dwNeedCoin;					// 所需铜钱
	UINT32	dwNeedClip;					// 所需碎片
	UINT16	wNeedPlayerLevel;			// 所需玩家等级
};
typedef HashMap<UINT32, SAwakenClipToStudyProp2> CAwakenClipToStudyPropMap2; //key为byStudyColorKind+byStudyAttrKind1/2+byStudyLevel
typedef CAwakenClipToStudyPropMap2::iterator CAwakenClipToStudyPropMapItr2;
typedef vector<SAwakenClipToStudyProp2>CAwakenClipToStudyPropVec2;//

struct SPlayerAwakenLevelProp2
{
	SAwakenLevelProp2		m_astAwakenLevelProp[MAX_AWAKEN_LEVEL+1];//悟道等级配置，下标为悟道等级
	SAwakenLevelProp2		m_stRudingGoldAwakenLevelProp; //元宝悟道入定状态悟道配置
	SAwakenLevelProp2		m_stChewuGoldAwakenLevelProp;	//元宝悟道彻悟状态悟道配置
};

typedef map<UINT16, SPlayerAwakenLevelProp2> CPlayerAwakenLevelPropMap2; //key为玩家起始等级wStartPlayerLeve
typedef CPlayerAwakenLevelPropMap2::iterator CPlayerAwakenLevelPropMapItr2;

//分解碎片配置
struct  SAwakenResolveProp
{
    UINT8   byStudyColorKind;        //颜色属性 双属性对应配置为8
    UINT8   byIfResolve;             //是否可以分解
    UINT16  wCommonNum;              //普通分解可得碎片数量
    UINT16  wSpecialNum;             //元宝分解可得碎片数量
    UINT32  dwCommonCoin;            //普通分解消耗的铜钱
    UINT32  dwSpecialGold;           //元宝分解消耗的铜钱
};

typedef map<UINT8, SAwakenResolveProp> CAwakenResolvePropMap;
typedef CAwakenResolvePropMap::iterator CAwakenResolvePropMapItr;


class CAwakenPropMgr2 : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CAwakenPropMgr2);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_AWAKEN2 ; }

    BOOL Init();

    VOID UnInit();

public:	
	SAwakenBaseProp2& GetAwakenBaseProp() { return m_stAwakenBaseProp; }
	//////////////////////////
	inline	UINT32 GetAwakenGold() { return m_stAwakenBaseProp.dwGoldAwaken; }
	inline VOID	GetwLevelStudyUnlockLevelInfo(UINT16 awStudyUnlockLevelInfo[MAX_HERO_STUDY_GRID_NUM]) { memcpy(awStudyUnlockLevelInfo, m_awStudyUnlockLevelInfo, sizeof(m_awStudyUnlockLevelInfo)); }
	inline UINT16 GetMaxStudyLevel() { return m_stAwakenBaseProp.byMaxStudyLevel; }
	//////////////////////////
	UINT32	GetAwakenCoin(UINT16 wPlayerLevel, UINT8 byGoldFlag, UINT8 byAwakenLevel);
	VOID	GetAwakenCoin(UINT32 adwAwakenCoin[MAX_AWAKEN_NUM]) { memcpy(adwAwakenCoin, m_adwAwakenCoin, sizeof(m_adwAwakenCoin)); }
	UINT8	AwakenStudy(UINT16 wPlayerLevel, BOOL bOpenExpStudy, UINT8 byAwakenLevel, UINT8 byGoldFlag, UINT8& byStudyColorKind, UINT8& byStudyAttrKind1, UINT8& byStudyAttrKind2, 
		UINT32& dwGenCoin, UINT32& dwGenClip, UINT8& byZTSUseTime);
	UINT32	GetStudyExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel);
	UINT32	GetStudyUpgradeNeedExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyCurLevel);
	BOOL	CkUnlockHeroGrid(UINT16 wHeroLevel, UINT8 byGridIdx);
	VOID	GetStudyAttrValue(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel, UINT32& dwAttrValue1, UINT32& dwAttrValue2);
	CAwakenClipToStudyPropVec2& GetAwakenClipToStudyPropVec() { return m_vecAwakenClipToStudyProp; }
	SAwakenClipToStudyProp2* GetClipToStudyProp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel);
	CSetResolveItemID& GetResolveItemID() { return m_setResolveItemID; }
	UINT8 GetRandomCliByItemID(UINT16 wItemID);
    CAwakenResolvePropMap& GetAwakenResolveProp(){return m_mapResolveProp;}
    SAwakenResolveProp* GetAwakenResolvePropByID(UINT8 byStudyColorKind);
    UINT8 GetAwakenIfResolveByID(UINT8 byStudyColorKind);
public:
    CAwakenPropMgr2();
    virtual ~CAwakenPropMgr2();

protected:
	BOOL LoadBaseFromDB();
	BOOL LoadAwakenLevelFromDB();
	BOOL LoadUnlockHeroStudyGridFromDB();
	BOOL LoadLevel2UnlockAwakenStudyFromDB();
	BOOL LoadAwakenStudyAttrFromDB();
	BOOL LoadAwakenClipToStudyFromDB();
    BOOL LoadAwakenResolvePropFromDB();

private:
	SAwakenBaseProp2			m_stAwakenBaseProp; //基本配置
	CPlayerAwakenLevelPropMap2	m_mapPlayerAwakenLevelProp;//悟道配置
	UINT32					m_adwAwakenCoin[MAX_AWAKEN_NUM];//悟道所需铜钱
	C1616Map				m_mapLevel2OpenNumProp; //key为伙伴等级，value为开放的心得格子数
	C1616Map				m_mapOpenNum2LevelProp; //key为开放的心得格子数, value为解锁的伙伴等级
	UINT16					m_awStudyUnlockLevelInfo[MAX_HERO_STUDY_GRID_NUM]; //心得解锁等级信息
	UINT16					m_wMaxOpenNumHeroLevel;//最大开放心得格子数的武将等级
	CLevel2UnlockStudyColorProMap2 m_mapLevel2AwakenStudyGenUnlockPropMap; //心得生成概率及解锁配置，key为玩家等级(该等级有的心得则表示已解锁)
	CAwakenStudyAttrPropMap2	m_mapAwakenStudyAttrProp; //心得属性配置，key为byStudyColorKind+byStudyAttrKind+byStudyLevel
	CAwakenClipToStudyPropMap2	m_mapAwakenClipToStudyProp;//碎片兑换心得配置
	CAwakenClipToStudyPropVec2	m_vecAwakenClipToStudyProp;//碎片兑换心得配置
	CSetResolveItemID       m_setResolveItemID;
	CAwakenStudyAttrPropMap2   m_mapResolveOrangeProp;
    CAwakenResolvePropMap    m_mapResolveProp;
};

#endif // #ifndef _AWAKENBASEPROPPROPMGR_H_