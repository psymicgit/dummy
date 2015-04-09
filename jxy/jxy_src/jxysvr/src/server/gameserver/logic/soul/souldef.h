///<------------------------------------------------------------------------------
//< @file:   soulmanager.h
//< @author: yewenhui
//< @date:   2014年6月13日 10点10分
//< @brief:  武魂管理
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#ifndef _SOUL_DEF_H_
#define _SOUL_DEF_H_

#include <map>
#include <include/sdtype.h>
#include <common/server/utility.h>
#include "protocol/server/protocommondata.h"

// 产出将魂三种方式
enum ProductSoulType
{
	ePST_Low		= 0,
	ePST_Middle		,
	ePST_High		,
	ePST_Activity	,
	ePST_Cnt		,
};

enum eSoulConst {
	eDevourSoul		= 0,
	eNormalSoul		= 1,
};

enum eSoulBoxType {
	eBT_CopperBox	= 0,
	eBT_SliverBox	,
	eBT_GoldBox		,
};

enum eSoulUpdatePotency
{
	ESP_SUCCESS = 0,//成功
	ESP_ITEM_NOT_ENOUGH = 1,//道具不足
	ESP_MAX_LEVEL = 2,//已到最高等级
	ESP_NOT_CONFIG = 3,//没有配置
	ESP_LEVEL_NOT_ENOUGH = 4,//战魂等级不足
};
struct SSoulRandomProp
{
	UINT8 byStarLevel;		//星级
	UINT8 byCurAttrNum;		//当前属性个数
	UINT8 byOneAttrProb;	//出现一个属性的概率
	UINT8 byTwoAttrProb;	//出现两个属性的概率
	UINT8 byThreeAttrProb;	//出现三个属性的概率
	UINT8 byFourAttrPropb;	//出现四个属性的概率
	UINT8 byFiveAttrPropb;	//出现五个属性的概率
	UINT8 bySixAttrPropb;	//出现六个属性的概率
};
typedef map<UINT16, CRandomMap> CMapSoulRandom;
typedef CMapSoulRandom::iterator CMapSoulRandomItr;

struct SSoulAttrRandomProp
{
	UINT8 byStarLevel;	//星级
	UINT8 byAttrNum;//属性个数
	UINT8 byCurAttrIdx;//当前属性编号
	UINT8 byBeginRandom;//最低随机值 
	UINT8 byEndRandom;//最高随机值 
};

typedef map<UINT32, SSoulAttrRandomProp> CMapSoulAttrRandom;
typedef CMapSoulAttrRandom::iterator CMapSoulAttrRandomItr;

struct SSoulAttrProp
{
	UINT8 byStarLevel;			//星级
	UINT8 byAttrNum;		//强化等级
	UINT8 byKindID;				//属性ID
	UINT32 dwValue;				//属性基本值
};

typedef map<UINT32, SSoulAttrProp> CMapSoulAttrProp;
typedef CMapSoulAttrProp::iterator CMapSoulAttrPropItr;

struct SoulBaseCfg {
	UINT32		unSoulId;
	UINT8		ucStar;
	string		strSoulName;
	UINT32		unWeight;//权重
	UINT8		ucSoulType; // 战魂类型 2代表防御类 1代表攻击类 0 被吞噬战魂
	UINT32		unDevourExp; // 吞噬经验
	string		strSoulPic;
	UINT32		unEffectId;		// 特效id
	UINT8		ucEffectOdd;	// 特效概率
	string		strNamePic;

	SoulBaseCfg() 
		: unSoulId(0), ucStar(0), unWeight(0), ucSoulType(0)
		, unDevourExp(0)
		, unEffectId(0), ucEffectOdd(0)
	{

	}
};

// key->魂id value->魂配置
typedef map<UINT32, SoulBaseCfg*> MapSoulCfg;
// key->魂星 value->总权重
typedef map<UINT8, UINT32> MapSoulStarWeight;

// 魂的产出配置
struct SoulProductCfg {
	UINT8		ucId;
	UINT8		ucSoulPercent;
	UINT32		unMeterialGoodId;
	vector<UINT32> vPossibleStar;
	vector<UINT32> vStarWeight;

	SoulProductCfg()
		: ucId(0)
		, ucSoulPercent(0), unMeterialGoodId(0)
	{

	}
};

// key->魂id value->魂产出配置
typedef map<UINT8, SoulProductCfg*> MapSoulProductCfg;

typedef map<UINT32, CRandomMap> CSoulRandom;
typedef CSoulRandom::iterator CSoulRandomItr;

struct SoulLvCfg {
	UINT8		ucStarLv;
	UINT8		ucStrengthLv;
	UINT32		unAddAttrPercent;
	UINT32		unExp;

	SoulLvCfg() : ucStarLv(0), ucStrengthLv(0)
		, unAddAttrPercent(0), unExp(0)
	{

	}

};

typedef vector<SoulLvCfg*> VecSoulLvCfg;

struct SoulExchangeCfg
{
	UINT32		unId;
	string		strName;
	vector<UINT32> vMeterialId;
	vector<UINT32> vMeterialNum;
	vector<UINT32> vGetStar;
	vector<UINT32> vGetStarWeight;

	SoulExchangeCfg() : unId(0) {

	}
};

typedef map<UINT32, SoulExchangeCfg*> MapSoulExchangeCfg;

struct SSoulPotencyAttrProp
{
	UINT8 byPotencyKindID;
	UINT32 dwPotencyValue;
};

struct SSoulPotencyProp
{
	UINT16 wSoulID;//潜能类别
	UINT8 byPotencyLevel;//潜能等阶
	UINT8 byPotencySubLevel;//潜能等级
	UINT16 wNeedSoulLevel;//需要的战魂等级
	vector<DT_ITEM_DATA> vecItemData;//消耗的物品
	C3232Map mapCountItemData;//累积物品

	vector<DT_POTENCY_ATTR_INFO> vecPotency;
	C3232Map mapCountAttrValue;//当前等级累积的
	
	SSoulPotencyProp()
	{
		wSoulID = 0;
		byPotencyLevel = 0;
		byPotencySubLevel = 0;
		vecPotency.clear();
		mapCountAttrValue.clear();
		vecItemData.clear();
		wNeedSoulLevel = 0;
	}
};

typedef map<UINT32, SSoulPotencyProp> CMapSoulPotencyProp;
typedef CMapSoulPotencyProp::iterator CMapSoulPotencyPropItr;
#endif // _SOUL_DEF_H_
