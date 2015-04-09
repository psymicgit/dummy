///<------------------------------------------------------------------------------
//< @file:   logic\fly\flypropmgr.h
//< @author: hongkunan
//< @date:   2014年8月27日 19:49:46
//< @brief:  飞升配置
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _flypropmgr_h_
#define _flypropmgr_h_

#include "logic/base/singleton.h"
#include "logic/base/basepropmgr.h"
#include "logic/fly/flydef.h"

// 飞升配置
struct SFlyProp
{
    SFlyProp(){ Clear(); }
    void Clear(){ wFlyRequiredJingjieLevel = 0; byEquipMakeBuyMaterialVipLvl = 0; byFlyAttrPromote = 0; fFlyAttrPromoteCoef = 0.0f; }

    UINT16 wFlyRequiredJingjieLevel;    // 所需境界等级
    UINT8 byFlyAttrPromote;             // 飞升后基础属性提升百分比，50代表50%
    float fFlyAttrPromoteCoef;          // 飞升后基础属性提升系数 = (1 + 提升百分比) / 100
    UINT8 byEquipMakeBuyMaterialVipLvl; // 装备制作购买材料所需vip等级
    
    UINT8 bySellEnchantStonePercent;    // 售出附魔装备时返还的附魔石百分比，50代表50%
    float fSellEnchantStonePercent;     // 售出附魔装备时返还的附魔石百分比
};

// 飞升配置
class CFlyPropMgr
    : public Singleton<CFlyPropMgr>
    , public CBasePopMgr
{
public:
    ///< @impl >
    BOOL ReLoadProp(){ return TRUE; }

    EConfigModule GetConfigModuleType() { return ECM_FLY; }

    BOOL Init();
    VOID UnInit(){}

    const SFlyProp& GetFlyProp(){ return m_stFlyProp; }

private:
    BOOL LoadFlyProp();

private:
    SFlyProp m_stFlyProp;
};

#endif //_flypropmgr_h_