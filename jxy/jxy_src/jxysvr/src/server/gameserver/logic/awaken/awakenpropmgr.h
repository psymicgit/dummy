
#ifndef _AWAKENBASEPROPPROPMGR_H_
#define _AWAKENBASEPROPPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>

using namespace SGDP;


#define GOLD_RUDING_PROP		110		//悟道等级配置元宝悟道入定状态配置
#define GOLD_CHEWU_PROP			111		//悟道等级配置元宝悟道彻悟状态配置


//基本配置
struct SAwakenBaseProp
{
	UINT32	dwGoldAwaken; //元宝悟道进入入定状态所需元宝
	UINT32	dwGenStudyCoin; //悟道心得获得铜钱时的铜钱
	UINT8	byMaxStudyLevel; //心得最大等级
};

//悟道等级配置
struct SAwakenLevelProp
{
	UINT8	byAwakenLevel;			// 悟到道别(从1开始，共5级(瞌睡/神游/冥思/入定/彻悟), (配置中110为元宝悟道入定状态配置，111为彻悟)
	UINT32	dwNeedCoin;				// 悟道所需铜币
	UINT8	byNextLevelRate;		// 跳到下一级概率(失败则返回第一级)
	UINT8	byCoinRate;				// 铜钱概率
	UINT8	byBuleStudyRate;		// 蓝色心得碎片概率
	UINT8	byPurpleStudyRate;		// 紫色心得碎片概率
	UINT8	byRedStudyRate;			// 红色心得碎片概率
	UINT8	byOrangeStudyRate;		// 橙色心得碎片概率

	CRandomVec vecRandom; //心得概率,按颜色插入顺序
};


//心得生成概率及解锁配置(该等级有则表示已解锁)
struct SAwakenStudyGenUnlockProp
{
	UINT16	wPlayerLevel;				// 玩家等级
	UINT8	byStudyColorKind;			// 心得颜色类型
	UINT8	byStudyAttrKind;			// 心得属性类型
	UINT8	byGenRate;					// 生成概率
};
typedef vector<SAwakenStudyGenUnlockProp> CAwakenStudyGenUnlockPropVec; 
//玩家每等级心得生成概率及解锁配置
struct  SPlayerAwakenStudyGenUnlockProp
{
	UINT16 wPlayerLevel;
	CAwakenStudyGenUnlockPropVec avecAwakenStudyGenUnlockProp[EC_ORANGE+1]; //4种颜色的配置,下标为颜色
	CRandomVec avecRandom[EC_ORANGE+1]; //4种颜色生成概率，按vecAwakenStudyGenUnlockProp插入顺序
};
//心得生成概率及解锁配置(该等级有的心得则表示已解锁)
typedef HashMap<UINT16, SPlayerAwakenStudyGenUnlockProp> CLevel2UnlockStudyColorProMap; //key为玩家等级wPlayerLevel
typedef CLevel2UnlockStudyColorProMap::iterator CLevel2UnlockStudyColorProMapItr;


//心得属性配置
struct SAwakenStudyAttrProp
{
	UINT8	byStudyColorKind;			// 心得颜色类型
	UINT8	byStudyAttrKind;			// 心得属性类型
	UINT8	byStudyLevel;				// 心得等级
	UINT32	dwAttrValue;				// 心得属性值	
	UINT32	dwHaveExp;					// 拥有的经验
};
typedef HashMap<UINT32, SAwakenStudyAttrProp> CAwakenStudyAttrPropMap; //key为byStudyColorKind+byStudyAttrKind+byStudyLevel
typedef CAwakenStudyAttrPropMap::iterator CAwakenStudyAttrPropMapItr;



class CAwakenPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CAwakenPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_AWAKEN ; }

    BOOL Init();

    VOID UnInit();

public:	
	inline	UINT32 GetAwakenGold() { return m_stAwakenBaseProp.dwGoldAwaken; }
	inline VOID	GetwLevelStudyUnlockLevelInfo(UINT16 awStudyUnlockLevelInfo[MAX_HERO_STUDY_GRID_NUM]) { memcpy(awStudyUnlockLevelInfo, m_awStudyUnlockLevelInfo, sizeof(m_awStudyUnlockLevelInfo)); }
	inline UINT16 GetMaxStudyLevel() { return m_stAwakenBaseProp.byMaxStudyLevel; }
	//////////////////////////
	UINT32	GetAwakenCoin(UINT8 byGoldFlag, UINT8 byAwakenLevel);
	VOID	GetAwakenCoin(UINT32 adwAwakenCoin[MAX_AWAKEN_NUM]) { memcpy(adwAwakenCoin, m_adwAwakenCoin, sizeof(m_adwAwakenCoin)); }
	UINT8	AwakenStudy(UINT16 wPlayerLevel, UINT8 byAwakenLevel, UINT8 byGoldFlag, UINT8& byStudyColorKind, UINT8& byStudyAttrKind, UINT32& dwGenCoin);
	UINT32	GetStudyExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel);
	UINT32	GetStudyUpgradeNeedExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyCurLevel);
	BOOL	CkUnlockHeroGrid(UINT16 wHeroLevel, UINT8 byGridIdx);
	UINT32	GetStudyAttrValue(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel);
	
public:
    CAwakenPropMgr();
    virtual ~CAwakenPropMgr();

protected:
	BOOL LoadBaseFromDB();
	BOOL LoadAwakenLevelFromDB();
	BOOL LoadUnlockHeroStudyGridFromDB();
	BOOL LoadLevel2UnlockAwakenStudyFromDB();
	BOOL LoadAwakenStudyAttrFromDB();

private:
	SAwakenBaseProp			m_stAwakenBaseProp; //基本配置
	SAwakenLevelProp		m_astAwakenLevelProp[MAX_AWAKEN_LEVEL+1];//悟道等级配置，下标为悟道等级
	SAwakenLevelProp		m_stRudingGoldAwakenLevelProp; //元宝悟道入定状态悟道配置
	SAwakenLevelProp		m_stChewuGoldAwakenLevelProp; //元宝悟道彻悟状态悟道配置
	UINT32					m_adwAwakenCoin[MAX_AWAKEN_NUM];//悟道所需铜钱
	C1616Map				m_mapLevel2OpenNumProp; //key为伙伴等级，value为开放的心得格子数
	C1616Map				m_mapOpenNum2LevelProp; //key为开放的心得格子数, value为解锁的伙伴等级
	UINT16					m_awStudyUnlockLevelInfo[MAX_HERO_STUDY_GRID_NUM]; //心得解锁等级信息
	UINT16					m_wMaxOpenNumHeroLevel;//最大开放心得格子数的武将等级
	CLevel2UnlockStudyColorProMap m_mapLevel2AwakenStudyGenUnlockPropMap; //心得生成概率及解锁配置，key为玩家等级(该等级有的心得则表示已解锁)
	CAwakenStudyAttrPropMap	m_mapAwakenStudyAttrProp; //心得属性配置，key为byStudyColorKind+byStudyAttrKind+byStudyLevel
};

#endif // #ifndef _AWAKENBASEPROPPROPMGR_H_