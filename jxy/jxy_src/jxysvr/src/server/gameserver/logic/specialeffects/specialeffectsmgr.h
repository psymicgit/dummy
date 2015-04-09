///<------------------------------------------------------------------------------
//< @file:   logic\specialeffects\specialeffectsmgr.h
//< @author: Kang_zhiquan
//< @date:   2014年6月5日 9:58:16
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _specialeffectsmgr_h_
#define _specialeffectsmgr_h_
#include "logic/base/basepropmgr.h"
#include "logic/base/basedefine.h"
#include <map>
#include <list>

typedef struct tagDT_SPECIAL_EFFECT_DESC DT_SPECIAL_EFFECT_DESC;

using namespace std;
struct SSpecialEffectProp
{
	UINT16 wEffectID;	//特效ID
	string strEffectName; //物资名称
	UINT8 byEffectBigType;	//特效大类型
	UINT8 byEffectSmallType; //物资小类型
	UINT16 wTriggerProb;	//发动概率
	UINT16 wEffectPercent;	//效果百分比
	UINT16 wLimitNum;		//限制次数
	UINT8 byUnlockLevel;//解锁等级
};

class CBattleAttrObj;
class CSpecialEffect;
class CPlayer;
class CSoul;
class CSpecialEffectPropMgr : public CBasePopMgr
{
	DECLARE_SINGLETON_PROPMGR(CSpecialEffectPropMgr)
	typedef multimap<UINT16, SSpecialEffectProp> CMapSpecialEffect; 
	typedef CMapSpecialEffect::iterator CMapSpecialEffectItr;
	typedef map<UINT32, SSpecialEffectProp> CMapTypeSpecialEffect;
	typedef CMapTypeSpecialEffect::iterator CMapTypeSpecialEffectItr;
public:
	CSpecialEffectPropMgr()
	{
		m_mapSpecialEffect.clear();
	}
	~CSpecialEffectPropMgr()
	{
		m_mapSpecialEffect.clear();
	}
	virtual EConfigModule GetConfigModuleType() { return ECM_SPECIALEFFECT; }
	BOOL Init();
	virtual	VOID UnInit();
public:
	const SSpecialEffectProp* GetSpecialEffectProp(UINT16 wEffectID, UINT8 bySmallType);
	UINT16 GetTriggerProb(UINT16 wEffectID, UINT8 bySmallType);
	UINT16 GetEffectPercent(UINT16 wEffectID, UINT8 bySmallType);
	UINT16 GetLimitNum(UINT16 wEffectID, UINT8 bySmallType);
	BOOL CreateSpecialEffect(CPlayer* poPlayer, CBattleAttrObj* poMySelf, UINT32 dwEffectID, list<CSpecialEffect*>& listSpecialEffect);
	CSpecialEffect* CreateSpecialEffect(CPlayer* poPlayer, CBattleAttrObj* poMySelf, DT_SPECIAL_EFFECT_DATA& stSpecialData, UINT8 bySelectIdx);
	VOID FreeSpecialEffect(UINT8 byEffectType, CSpecialEffect*  poEffect);
	VOID FreeSpecialEffect(list<CSpecialEffect*>& poEffect);
	BOOL GenerateSpecialEffectData(UINT16 wEffectID, DT_SPECIAL_EFFECT_DATA& stData);
	UINT8 GetUnlockLevel(UINT16 wEffectID);

	VOID GetSpecialEffectDataCli(UINT16 wEffectID, UINT8 bySmallType, DT_SPECIAL_EFFECT_DESC& stCli);
private:
	BOOL LoadPropFromDB();
private:

	CMapSpecialEffect m_mapSpecialEffect;
	CMapTypeSpecialEffect m_mapSpecialEffectByType;
};
#endif //_specialeffectsmgr_h_
