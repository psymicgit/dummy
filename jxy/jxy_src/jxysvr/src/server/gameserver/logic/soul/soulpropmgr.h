///<------------------------------------------------------------------------------
//< @file:   logic\soul\soulpropmgr.h
//< @author: Kang_zhiquan
//< @date:   2014年6月12日 11:58:17
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _soulpropmgr_h_
#define _soulpropmgr_h_
#include "protocol/server/protocligs.h"
#include "logic/base/basepropmgr.h"
#include "souldef.h"
#include "commondef.h"
using namespace std;

class CSoul;
class CSoulPropMgr : public CBasePopMgr
{
	DECLARE_SINGLETON_PROPMGR(CSoulPropMgr)
public:
	CSoulPropMgr();
	virtual ~CSoulPropMgr();

	// implement from CBasePopMgr
	EConfigModule	GetConfigModuleType() { return ECM_SOUL; }
	BOOL			Init();
	VOID			UnInit();

	//获取属性基本值
	UINT32 GetAttrBaseValue(UINT8 byStarLevel, UINT16 wStrengLevel, UINT8 byKindID);
	//获取属性条数配置
	const CRandomMap* GetAttrNumRandom(UINT8 byStarLevel, UINT8 byCurNum);
	//获取属性值配置
	BOOL GetAttrValueRandom(UINT8 byStarLevel, UINT8 byAttrNum, UINT8 byCurIdx, const SSoulAttrRandomProp*& pstProp);
	//生成战魂结构体
	DT_SOUL_DATA GenerateSoulData(UINT16 wSoulID);
	
	//获取洗练费用
	DT_PLAYER_RES_DATA& GetReflashCost();
	//每天洗练次数
	UINT8 GetFreeReflashNum();

	// 将魂产出配置
	const SoulProductCfg* GetSoulProductCfg(UINT8 ucType);
	// 将魂信息配置
	const MapSoulCfg& GetMapSoulCfg() const;
	const SoulBaseCfg* GetSoulBaseCfg(UINT32 unSoulId);
	// 获取战魂等级相关配置
	const SoulLvCfg* GetSoulLvCfg(UINT8 ucStarLv, UINT8 ucStrengthLv);

	UINT8 GetSoulLv(UINT8 ucStarLv, UINT32 unExp);

	// 0代表最高等级了
	UINT32 NextSoulLvExp(UINT8 ucStarLv, UINT8 ucStrengthLv);

	const SoulExchangeCfg* GetSoulExchangeCfg(UINT8 unId);
	const MapSoulExchangeCfg& GetSoulExchangeAllCfg();
	BOOL GetSoulPotencyProp(CPlayer* poPlayer, UINT16 wSoulID, UINT8 byLevel, DT_POTENCY_ATTR_DATA_LST& stAttrData );
	BOOL GetSoulPotencyCountAttr(UINT16 wSoulID, UINT8 byLevel, UINT8 bySubLevel, DT_POTENCY_ATTR_LST_INFO& stAttrLstInfo);
	BOOL GetSoulPotencyCountAttr(UINT16 wSoulID, UINT8 byLevel, UINT8 bySubLevel, C3232Map &poAttrMap);
	UINT8 GetMaxPotencySubLevel(){ return m_byMaxSubLevel; }
	SSoulPotencyProp* GetSoulPotencyProp(UINT16 wSoulID, UINT8 byLevel, UINT8 bySubLevel);
protected:
	//属性基本值
	BOOL LoadAttrProp();
	//属性随机范围
	BOOL LoadAttrRandomProp();
	//属性条数随机
	BOOL LoadRandomProp();
	//战魂经验配置
	BOOL LoadExperience();
	BOOL LoadSoulBaseCfg();
	BOOL LoadSoulProductCfg();
	BOOL LoadSoulLvCfg();
	BOOL LoadSoulExchangeLvCfg();
	BOOL LoadSoulPotencyProp();
private:
	CMapSoulRandom		m_mapSoulRandom;//属性条数随机
	CMapSoulAttrRandom	m_mapSoulAttrRandom;//属性值随机范围
	CMapSoulAttrProp	m_mapSoulAttrProp;//对应的属性基本值
	DT_PLAYER_RES_DATA	m_stReflashCost;//洗练消耗
	UINT8				m_byFreeReflashNum;//每天免费洗练次数
	CMapSoulPotencyProp   m_mapSoulPotencyProp;//潜能属性配置

	MapSoulCfg			m_mpSoulCfg;
	MapSoulStarWeight	m_mpStarWeight;
	MapSoulProductCfg	m_mpProfuctCfg;
	VecSoulLvCfg		m_vecSoulLvCfg;
	MapSoulExchangeCfg	m_mpSoulExchangeCfg;

	UINT8 m_byMaxSubLevel;//潜能最大节点
};

#endif //_soulpropmgr_h_
