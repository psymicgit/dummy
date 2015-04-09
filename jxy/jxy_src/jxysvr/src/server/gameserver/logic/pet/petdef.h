///<------------------------------------------------------------------------------
//< @file:   logic\pet\petdef.h
//< @author: yewenhui10@qq.com
//< @date:   2014年8月6日 16:58:17
//< @brief:  宠物相关数据结构定义
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _PET_DEF_H_ 
#define _PET_DEF_H_

#include "sdtype.h"
#include <vector>
#include <map>
#include "protocol/server/protocommondata.h"
#include "protocol/server/protocligs.h"
using namespace std;
enum ePetType {
	ePT_Wind		= 1,	// 风
	ePT_Water		,		// 水
	ePT_Fire		,		// 火
	ePT_Land		,		// 土
	ePT_Ice			,		// 冰
	ePT_Sand		,		// 砂
	ePT_Hot			,		// 炎
	ePT_Soul		,		// 阴魂
	ePT_Dream		,		// 梦
	ePT_Legend		,		// 传说
	
	ePT_Cnt = ePT_Legend,
};

enum ePetQuality {
	ePQ_NormalPet		= 1,//野生
	ePQ_BabyPet			,	//普通
	ePQ_VariantPet		,	//变异

	ePQ_Cnt = ePQ_VariantPet ,
};

//宠物档次
enum ePetLevel
{
	EPL_FIRST = 1,//第一档
	EPL_SECOND = 2,//第二档
	EPL_THIRD = 3,//第三档
	EPL_FOURTH = 4,//第四档
};

// enum ePetWashQualityType{
// 	ePWQT_Ten		= 1, 
// 	ePWQT_Hundred	,
// 	ePWQT_Thousand	,
// 	ePWQT_TenThousand,
// };

struct PetSysCfg {
	UINT16		usMaxCarryPet;			// 上阵宠物最大数目
	UINT8		ucFreeFeedCnt;			// 一天喂养免费抽取次数
	UINT32		unResetFruitCost;		// 重置抽奖花费的元宝
	UINT32		unMaxFruitCost;			// 一键把水果弄成最好花费的元宝

	UINT8		ucTriggerWashQuality;	// 触发下一格子的品质

	UINT32		unPerfectInheritCostGold;	// 完美继承花费的元宝
	UINT32		unNormalInheritCostCoin;	// 普通继承花费的铜钱

	UINT8		ucNormalInheritExp;		// 普通继承经验百分比
	UINT8		ucGoodInheritExp;		// 完美继承经验百分比

	string		strWashDesc;
	string		strRuleDesc;
	string		strLowDesc;
	string		strMiddleDesc;
	string		strHighDesc;
	string		strItemDesc;
	vector<UINT32>		vecItemID;//规则面板显示的道具

	UINT16		usNormalFreeProductCnt;

// 	UINT32		unJinliuluItemId;
// 	UINT32		unSuperJinliuluItemId;
// 	std::vector<UINT32> vJinliuluRange;
// 	std::vector<UINT32>	vSuperJinliuluRange;

	PetSysCfg() 
		: usMaxCarryPet(0)
		, ucFreeFeedCnt(0)
		, unResetFruitCost(0)
		, unMaxFruitCost(0)
		, ucTriggerWashQuality(0)
		, unPerfectInheritCostGold(0)
		, unNormalInheritCostCoin(0)
		, usNormalFreeProductCnt(0)
// 		, unJinliuluItemId(0)
// 		, unSuperJinliuluItemId(0)
	{
	}
};

struct PetBaseCfg {
	UINT16		usPetCfgId;
	std::string	strPetName;
	UINT8		ucPetType;
	UINT8		ucPetQuality;
	UINT32		unDevourExp;
	UINT32		unSkillId;
	UINT32		unPhyChicMax;
	vector<UINT16> vecSkill;
	UINT8		ucPhyChicValPerMin;
	UINT8		ucPhyChicValPerMax;
	UINT32		dwBaseLiLing;
	UINT8		ucPetColor;

	std::string	strPetPic;
	std::string	strPetDesc;

	PetBaseCfg() 
		: usPetCfgId(0) 
		, ucPetType(0)
		, ucPetQuality(0)
		, unDevourExp(0)
		, unSkillId(0)
		, unPhyChicMax(0)
		, ucPhyChicValPerMin(0)
		, ucPhyChicValPerMax(0)
		, ucPetColor(1)
	{
		vecSkill.clear();
	}
};

typedef std::map<UINT16, PetBaseCfg*> PetBaseCfgList;
typedef multimap<UINT16, PetBaseCfg*> CMultiMapBaseProp;
struct PetLvKey {
	UINT16		usPetLv;

	PetLvKey()
		: usPetLv(0)
	{

	}

	bool operator<(const PetLvKey& rhs) const {
		return usPetLv < rhs.usPetLv;
	}

};

struct PetLvCfg {
	UINT32		unExp;

	PetLvCfg() 
		: unExp(0)
	{

	}
};

typedef std::map<PetLvKey, PetLvCfg*> MapPetLvCfg;

struct PetProductCfg {
	UINT8		ucProductPhrase;		// 
	UINT8		ucProductType;			// 
	UINT16		usPlayerMinLv;			//
	UINT16		usPlayerMaxLv;			//
	std::vector<UINT32> vGetPetType;	//
	std::vector<UINT32> vPetWeight;	//

	std::vector<UINT32> vGetPetQuality;	//
	std::vector<UINT32> vQualityWeight;	//
	UINT32		unScore;

	std::vector<UINT32> vGetItemId;		//
	std::vector<UINT32> vGetItemNum;		//
	std::vector<UINT32> vGetItemPercent;	//

	UINT32		unCostGold;
	UINT16		usFreeTimeInterval;		// 免费抽取宠物间隔时间（单位分钟）

	PetProductCfg()
		: ucProductPhrase(0)
		, ucProductType(0)
		, usPlayerMinLv(0)
		, usPlayerMaxLv(0)
		, unScore(0)
		, unCostGold(0)
		, usFreeTimeInterval(0)
	{
	}
};

typedef std::vector<PetProductCfg*> PetProductCfgList;

struct PetProductExtraCfg {
	UINT8		ucProductPhrase;		// 
	BOOL		bIsGoodProduct;
	UINT16		usProductCnt;

	std::vector<UINT32> vExtraItemId;	// 连续召唤额外获得道具id
	std::vector<UINT32>	vExtraItemNum;	// 连续召唤额外获得道具数量

	PetProductExtraCfg() 
		: ucProductPhrase(0)
		, bIsGoodProduct(FALSE)
		, usProductCnt(0)
	{

	}
};

typedef std::vector<PetProductExtraCfg*> PetProductExtraCfgList;

struct PetStarKey {
	UINT8		ucPetType;
	UINT8		ucPetQuality;
	UINT16		usPetStarLv;

	PetStarKey()
		: ucPetType(0)
		, ucPetQuality(0)
		, usPetStarLv(0)
	{

	}

	bool operator<(const PetStarKey& rhs) const {
		return ucPetType < rhs.ucPetType
			|| (ucPetType == rhs.ucPetType && ucPetQuality < rhs.ucPetQuality)
			|| (ucPetType == rhs.ucPetType && ucPetQuality == rhs.ucPetQuality && usPetStarLv < rhs.usPetStarLv);
	}
};

struct PetStarCfg {
	UINT16		usPetMaxLv;				// 宠物最大等级
	UINT8		ucPlayerLvLimit;
	vector<UINT32> vCostItemId;
	vector<UINT32> vCostItemNum;
	UINT32		unAddLingliPer;

	PetStarCfg() 
		: usPetMaxLv(0)
		, ucPlayerLvLimit(0)
		, unAddLingliPer(0)
	{
	}
};

typedef std::map<PetStarKey, PetStarCfg*> MapPetStarCfg;

enum ePetFruitType {
	ePFT_Banana			= 1,
	ePFT_Apple			,
	ePFT_Stawberry		,
	ePFT_Cherry			,
	ePFT_Sapodilla 		,			// 人参果
		
	ePetFruitCnt		= ePFT_Sapodilla,
};

struct PetFeedCfg {
	UINT8		ucFruitType;				// 图标类型
	UINT8		ucPercent;					// 出现概率
	UINT32		unBaseExp;					// 基础经验
	UINT16		vExpTimes[MAX_PET_FEED_FRUIT_CNT];	// 水果个数为n 的倍数

	PetFeedCfg() {
		memset(this, 0, sizeof(*this));
	}
};

typedef std::map<UINT8, PetFeedCfg*> MapPetFeedCfg;

struct PetStoreCfg {
	UINT32			unItemId;
	UINT32			unCostPetScore;
	UINT32			unCostGold;
	UINT16			usPlayerLvLimit;
	UINT16			usShowLv;

	UINT8			ucItemType;			// 商城道具类型

	PetStoreCfg() 
		: unItemId(0), unCostPetScore(0)
		, unCostGold(0), usPlayerLvLimit(0)
		, usShowLv(0)
	{
		
	}
};

typedef std::map<UINT16, PetStoreCfg*> MapPetStoreCfg;

// 洗资质
struct PetWashKey {
	UINT8		ucPetType;
	UINT8		ucPetQuality;
	UINT8		ucWashQuality;

	PetWashKey()
		: ucPetType(0)
		, ucPetQuality(0)
		, ucWashQuality(0)
	{

	}

	bool operator<(const PetWashKey& rhs) const {
		return ucPetType < rhs.ucPetType
			|| (ucPetType == rhs.ucPetType && ucPetQuality < rhs.ucPetQuality)
			|| (ucPetType == rhs.ucPetType && ucPetQuality == rhs.ucPetQuality && ucWashQuality < rhs.ucWashQuality);
	}

};

// 资质区间
struct PetPsychicRange {
	UINT32			unMinVal;
	UINT32			unMaxVal;

	PetPsychicRange() 
		: unMinVal(0), unMaxVal(0)
	{

	}
};

struct PetWashCfg {
	std::vector<UINT32>	vPossibleWashQuality;
	std::vector<UINT32>	vWashQualityWeight;
	std::vector<UINT32>	vUseItemid;
	std::vector<UINT32>	vUseItemNum;

	UINT32				unLockCostGold;

	std::vector<PetPsychicRange> vPsychicRange;			
	std::vector<UINT32>	vPsychicWeight;

	PetPsychicRange		totalRange;

	PetWashCfg()
		: unLockCostGold(0)
	{

	}

};

typedef std::map<PetWashKey, PetWashCfg*> MapPetWashCfg;

typedef std::pair<UINT8, UINT8> PetRefineKey;

struct PetRefineData {
	UINT8			ucPetTypeLevel;
	UINT8			ucPetQuality;

	PetRefineData()
		: ucPetTypeLevel(0), ucPetQuality(0)
	{

	}
};

struct PetRefineCfg {
	std::vector<PetRefineData> vGetPetData;
	std::vector<UINT32>		vGetPetWeight;

	PetRefineCfg() {

	}
};

typedef std::map<PetRefineKey, PetRefineCfg*> MapRefineCfg;

enum {
	P_SUCCESS,	//成功
	P_NOT_HAVE_PET,//找不到宠物
	P_NOT_PET_CFG,//找不宠物配置
	P_OTHER_ERR,//其它错误
};

struct SPetNeiDanProp
{
	UINT16 wNeiDanID;//内丹ID
	string strNeiDanName;//内丹名称
	UINT8 byWuXing;//五行
	UINT8 byQuality;//品阶
	UINT8 byType;//品质
	UINT32 dwBaseLingLi;//基础灵力
	UINT32 dwIncLingLi;//增量灵力
	UINT16 wDressReq;//穿戴要求等阶
	UINT32 dwCostCoin;//吞噬耗费铜钱
	UINT32 dwBaseExperience;//基础经验值
	SPetNeiDanProp()
	{
		wNeiDanID = 0;
		byWuXing = 0;
		byQuality = 0;
		byType = 0;
		dwBaseLingLi = 0;
		dwIncLingLi = 0;
		strNeiDanName = "";
		wDressReq = 0;
	}
};

typedef map<UINT16, SPetNeiDanProp> CMapPetNeiDanProp;
typedef CMapPetNeiDanProp::iterator CMapPetNeiDanPropItr;

struct SPetNeiDanUpdateProp
{
	UINT8 byType;	//品质
	UINT16 wLevel;	//等级
	UINT8 byQuality;//品阶
	UINT64 qwExperience;//经验
	SPetNeiDanUpdateProp()
	{
		byType = 0;
		wLevel = 0;
		qwExperience = 0;
		byQuality = 0;
	}
};

typedef map<UINT32, SPetNeiDanUpdateProp> CMapPetNeiDanUpdateProp;
typedef CMapPetNeiDanUpdateProp::iterator CMapPetNeiDanUpdatePropItr;

#endif // _PET_DEF_H_
