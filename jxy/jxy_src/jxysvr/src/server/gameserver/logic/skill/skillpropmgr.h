
#ifndef _SKILLPROPMGR_H_
#define _SKILLPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <logic/base/basepropmgr.h>
#include <common/client/commondef.h>
#include <common/server/utility.h>
#include <map>
#include <vector>
#include <protocommondata.h>

struct tagDT_SKILL_ACTION_DATA_CLI;
typedef tagDT_SKILL_ACTION_DATA_CLI DT_SKILL_ACTION_DATA_CLI;

struct tagDT_SKILL_EFFECT_DATA_CLI;
typedef tagDT_SKILL_EFFECT_DATA_CLI DT_SKILL_EFFECT_DATA_CLI;

struct tagDT_ATTACK_RANG_DATA_CLI;
typedef tagDT_ATTACK_RANG_DATA_CLI DT_ATTACK_RANG_DATA_CLI;

struct tagDT_EFFECT_RECOMMEND_CLI;
typedef tagDT_EFFECT_RECOMMEND_CLI DT_EFFECT_RECOMMEND_CLI;




using namespace std;

/// 技能名称名称最大长度
#define NAME_LEN_MAX	32
/// 技能介绍名称最大长度
#define INTRODUCTION_LEN_MAX	255

//技能基本信息
struct SSkillBaseProp
{
	//UINT16	wMaxGenNumPerDay;	//每天最大生成精气次数
	UINT32	dwGenGasCoin;		//产气所需铜币，第一次免费
	UINT32	dwUpFullGasBallLevelGold;	//提升所有精气球到满级所需元宝	
	UINT16	wAddExpPerUpgrade ;// 每次升级增加的经验值
	UINT16	wCostGasPerUpgrade;// 每次升级消耗的灵气
};


//技能招式
struct SSkillActionProp
{
	UINT16	wID;				// ID
	UINT16	wLevel;				// 等级

	UINT16	wEffectHurtRate;	// 效果伤害倍率(百分比)
	UINT32	dwExtHurtValue;	// 附加伤害值

	UINT32	dwExp;			// 升级带该等级所拥有的经验

	UINT16	wUnlockSkillEffectID; //解锁的效果ID
	UINT16  wDspUnlockSkillEffectIDPlayerLevel;//显示解锁效果所需的玩家等级
	
	UINT8	byUnlockAttackRangKind;	// 攻击范围类型
	UINT8	byUnlockTraceBuffKind;	// 追踪buff类型		

	UINT16	wAttackRangHurtRate;	// 攻击范围的伤害倍率（招式伤害基础上）
	UINT16	wDspUnlockAttackRangPlayerLevel; //显示解锁范围所需的玩家等级

	vector<UINT16> vecCurUnlockSkillEffectID; //当前所有解锁的效果ID
	vector<DT_ATTACK_RANG_DATA> vecCurUnlockAttackRang; //当前所有解锁的攻击范围
};
typedef map<UINT16, UINT16> CMaxSkillActionLevelMap; //招式最大等级
typedef CMaxSkillActionLevelMap::iterator CMaxSkillActionLevelMapItr;
typedef map<UINT32, SSkillActionProp> CID2SkillActionPropMap; //key为ID+Level
typedef CID2SkillActionPropMap::iterator CID2SkillActionPropMapItr;
typedef map<UINT32, SSkillActionProp> CRang2ActionPropMap; //key为ID+byUnlockAttackRangKind+byUnlockTraceBuffKind
typedef CRang2ActionPropMap::iterator CRang2ActionPropMapItr;
typedef map<UINT32, SSkillActionProp> CEffect2ActionPropMap; //key为ID+wUnlockSkillEffectID
typedef CEffect2ActionPropMap::iterator CEffect2ActionPropMapItr;
typedef map<UINT16, vector<DT_ATTACK_RANG_DATA> >CActionID2AllRangMap;//key为ID
typedef CActionID2AllRangMap::iterator CActionID2AllRangMapItr;
typedef map<UINT16, vector<UINT16> >CActionID2AllEffectMap;//key为ID
typedef CActionID2AllEffectMap::iterator CActionID2AllEffectMapItr;

//
//技能单个buff效果配置
struct SSkillOnceBuffProp
{
	UINT8	byBuffKindID;		// buff类型
	UINT8	byBuffRound;		// buff持续回合数
	UINT16	wBuffParam;			// buff参数
	UINT8	byBuffRate;			// buff概率
};

//技能效果配置
struct SSkillEffectProp
{
	SSkillEffectProp() { wID = 0; byOnceEffectKindID = 0; wOnceEffectParam = 0; wOnceEffectRate = 0; memset(astSkillBuffrop, 0, sizeof(astSkillBuffrop)); }

	UINT16	wID;					// ID

	UINT8	byOnceEffectKindID;		// 效果类型
	UINT16	wOnceEffectParam;		// 效果参数
	UINT16  wOnceEffectRate;		//效果概率

	SSkillOnceBuffProp astSkillBuffrop[MAX_SKILL_BUFF_NUM]; //技能buff效果
};
typedef map<UINT16, SSkillEffectProp> CID2SkillEffectPropMap; //key为ID
typedef CID2SkillEffectPropMap::iterator CID2SkillEffectPropMapItr;



////技能每个等级的配置
//struct SSkillProp
//{
//	UINT16	wID;		// 技能ID
//	TCHAR	szName[NAME_LEN_MAX];		// 技能名称
//	TCHAR	szIntroduction[INTRODUCTION_LEN_MAX];		// 技能介绍
//	UINT8	byColor;		// 技能颜色，不参与计算，只用于标识
//	UINT16	wSkillLevel;	// 技能等级，不参与计算，只用于标识	
//	UINT16	wSkillActionID; // 技能招式ID, 不能为0
//	UINT16	wSkillActionLevel; // 技能招式等级
//	UINT16	wSkillEffectID; // 技能效果ID，可以为0
//};

//技能效果推荐组合配置
struct SSkillEffectRecommendProp
{
	UINT16 wEffectID; //效果ID
	UINT16 wStartRecommendJingJieLevel; //开始推荐境界等级
	UINT16 wRecommendHeroID1; //推荐伙伴ID
	UINT8  wRecommendHeroEffectID1; //推荐搭配效果ID(0表示没有)
	UINT8 byRecommendHeroRangKind1;
	UINT8 byRecommendHeroRangBuff1;
	UINT16 wRecommendHeroID2;
	UINT16 wRecommendHeroEffectID2;
	UINT8 byRecommendHeroRangKind2;
	UINT8 byRecommendHeroRangBuff2;

};
typedef map<UINT16, SSkillEffectRecommendProp> CJingJieLevel2RecommendPropMap; // key为wStartRecommendJingJieLevel
typedef CJingJieLevel2RecommendPropMap::iterator CJingJieLevel2RecommendPropMapItr;
typedef map<UINT16, CJingJieLevel2RecommendPropMap> CEffect2RecommendPropMapMap; // key为wEffectID
typedef CEffect2RecommendPropMapMap::iterator CEffect2RecommendPropMapMapItr;

struct SPetSkillEffectProp
{
	UINT16 wSkillID;		//技能ID
	string strSkillName;	//技能名称
	string strSkillDesc;	//技能描述
	string strSkillBeginRound;//开始回合描述
	UINT16 wSkillLevel;		//技能等级
	UINT16 wHurtRate;		//技能攻击伤害范围
	UINT8  byAttackRangType;	//攻击范围类型
	UINT32 dwExtHurtValue;	//技能额外伤害
	UINT16 wFinalHurtValue; //最终伤害比
	UINT8  byMaxTargetNum;	//最多作用人数
	UINT8 byOnceEffectID;	//特效ID
	UINT16 wOnceEffectRate;	//特效概率
	UINT16 wOnceEffectParam;	//特效参数

	vector<SSkillOnceBuffProp> vecBuffProp;

	SPetSkillEffectProp()
	{
		clear();
	}
	void clear()
	{
		wSkillID = 0;
		strSkillDesc = "";
		strSkillName = "";
		wSkillLevel = 0;
		wHurtRate = 0;
		byAttackRangType = 0;
		dwExtHurtValue = 0;
		byOnceEffectID = 0;
		wOnceEffectRate = 0;
		wOnceEffectParam = 0;
		vecBuffProp.clear();
		wFinalHurtValue = 100;
	}
};

struct SRandomPetSkillProp
{
	SPetSkillEffectProp stMainProp;
	vector<SPetSkillEffectProp> vecRandomProp;
};

typedef map<UINT32, SPetSkillEffectProp> CMapPetSkillEffectProp;
typedef CMapPetSkillEffectProp::iterator CMapPetSkillEffectPropItr;

typedef map<UINT32, SRandomPetSkillProp> CMapVecRandomSkillProp;
typedef CMapVecRandomSkillProp::iterator CMapVecRandomSkillPropItr;

class CPlayer;

//typedef map<UINT32, SSkillProp> CIDLevel2SkillPropMap; //key为ID+Level
//typedef CIDLevel2SkillPropMap::iterator CIDLevel2SkillPropMapItr;




/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CSkillPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CSkillPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_SKILL ; }
    BOOL Init();
    VOID UnInit();
public:
	SSkillBaseProp&	  GetSkillBaseProp() { return m_stSkillBaseProp; }
	SSkillActionProp* GetSkillActionProp(UINT16 wID, UINT16 wLevel);
	SSkillEffectProp* GetSkillEffectProp(UINT16 wID);
	UINT16 GetMaxSkillActionLevel(UINT16 wID);
	SSkillActionProp* GetSkillActionInfoCli(UINT16 wActionID, UINT16 wLevel, DT_SKILL_ACTION_DATA_CLI& stActionData);
	SSkillEffectProp* GetSkillEffectInfoCli(UINT16 wEffectID, DT_SKILL_EFFECT_DATA_CLI& stEffectData);
	SSkillActionProp* GetSkillAttackRangInfoCli(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind, DT_ATTACK_RANG_DATA_CLI& stRangInfo);
	SSkillActionProp* GetAttackRangActionProp(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind);
	UINT16 GetAttackRangHurtRate(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind);
	UINT16 GetAttackRangUnlockLevel(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind);
	UINT16 GetAttackRangDspLevel(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind);
	UINT16 GetEffectUnlockLevel(UINT16 wActionID, UINT16 wEffectID);
	UINT16 GetEffectDspLevel(UINT16 wActionID, UINT16 wEffectID);
	vector<DT_ATTACK_RANG_DATA>& GetActionAllRang(UINT16 wActionID);
	vector<UINT16>& GetActionAllEffect(UINT16 wActionID);
	SPetSkillEffectProp* GetPetSkillProp(UINT16 wSkillID, UINT16 wLevel);
	SRandomPetSkillProp* GetPetRandomSkillProp(UINT16 wSkillID, UINT16 wLevel);
	///////////////////////////////
	UINT32 GetJingJieSubLevelUpgradeGas(UINT8 byCareerID, UINT16 wLevel, UINT16 wSubLevel);
	VOID GenGas(UINT8 byPurpleFlag, DT_GAS_BALL_DATA& stDT_GAS_BALL_DATA);
	VOID UpGasBallLevel(UINT8 byFullLevelFlag, UINT8& byLevel);
	UINT32 GetGas(UINT8 byGasType, UINT8 byLevel);
	UINT32 GetGuidePurpleGas();
	UINT32 GetCommonUpGasCoin(UINT16 wUpNum);
	//inline UINT16 GetMaxGenNumPerDay()	{ return m_stSkillBaseProp.wMaxGenNumPerDay; }
	inline UINT16 GetGenGasCoin()	{ return m_stSkillBaseProp.dwGenGasCoin; }
	inline UINT32 GetUpFullGasBallLevelGold()	{ return m_stSkillBaseProp.dwUpFullGasBallLevelGold; }
	inline UINT32 GetAddExpPerUpgrade() { return m_stSkillBaseProp.wAddExpPerUpgrade; }
	inline UINT32 GetCostGasPerUpgrade() { return m_stSkillBaseProp.wCostGasPerUpgrade; }
	///////////////////////////////

	SSkillEffectRecommendProp* GetSkillEffectRecommendCli(UINT16 wEffectID, UINT16 wCurJingJieLevel, DT_EFFECT_RECOMMEND_CLI astRecommendInfo[2]);
	SSkillEffectRecommendProp* GetSkillEffectRecommendProp(UINT16 wEffectID, UINT16 wJingJieLevel);
	UINT32 GetPetSkillTrigerNum(UINT16 wSkillID, UINT16 wPetLevel);
public:
    CSkillPropMgr();
    virtual ~CSkillPropMgr();

	BOOL LoadSkillBaseFromDB();
	BOOL LoadSkillEffectRecommendPropFromDB();
	BOOL LoadSkillActionPropFromDB();
	BOOL LoadSkillEffectPropFromDB();
	BOOL LoadSkillPropFromDB();
	BOOL LoadPetSkillFromDB();

protected:
	BOOL ParsePetSkill(const string& strXMLValue);
	BOOL ParseRandomPetSkill(const string& strXMLValue);
private:
	SSkillBaseProp			m_stSkillBaseProp;
	//CIDLevel2SkillPropMap	m_mapIDLevel2SkillProp; 
	CID2SkillActionPropMap	m_mapIDLevel2SkillActionProp;
	CID2SkillEffectPropMap	m_mapIDLevel2SkillEffectProp;
	CMaxSkillActionLevelMap	m_mapMaxSkillActionLevel;
	CEffect2RecommendPropMapMap m_mapSkillEffectRecommendProp;
	CRang2ActionPropMap		m_mapRang2ActionProp;
	CEffect2ActionPropMap	m_mapEffect2ActionProp;
	CActionID2AllRangMap	m_mapActionID2AllRang;
	CActionID2AllEffectMap	m_mapActionID2AllEffect;

	CMapPetSkillEffectProp  m_mapPetSkillProp;
	CMapVecRandomSkillProp  m_mapRandomPetSkillProp;
};

#endif // #ifndef _SKILLPROPMGR_H_
