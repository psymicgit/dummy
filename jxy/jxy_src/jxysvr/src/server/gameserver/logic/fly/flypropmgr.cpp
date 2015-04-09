///<------------------------------------------------------------------------------
//< @file:   logic\fly\flypropmgr.cpp
//< @author: hongkunan
//< @date:   2014Äê8ÔÂ27ÈÕ 19:49:41
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "flypropmgr.h"
#include "framework/gsapi.h"
#include "commondef.h"
#include <sddb.h>
#include <db/dbmgr.h>
#include "logic/base/stringtool.h"

BOOL CFlyPropMgr::Init()
{
    if (!LoadFlyProp()){
        return FALSE;
    }

    return TRUE;
}

BOOL CFlyPropMgr::LoadFlyProp()
{
    DB_READ_BEGIN("flyprop");

    m_stFlyProp.Clear();

    while (pRecordSet->GetRecord()) {
        m_stFlyProp.wFlyRequiredJingjieLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("RequiredJingjieLevel"));
        m_stFlyProp.byEquipMakeBuyMaterialVipLvl = SGDP::SDAtou(pRecordSet->GetFieldValueByName("EquipMakeBuyMaterialVipLvl"));
        m_stFlyProp.byFlyAttrPromote    = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FlyAttrPromote"));
        m_stFlyProp.fFlyAttrPromoteCoef = (100.0f + m_stFlyProp.byFlyAttrPromote) / 100;

        m_stFlyProp.bySellEnchantStonePercent = SGDP::SDAtou(pRecordSet->GetFieldValueByName("SellEnchantStonePercent"));
        m_stFlyProp.fSellEnchantStonePercent  = (m_stFlyProp.bySellEnchantStonePercent + 0.0f) / 100;
        break;
    }

    DB_READ_END;
    return TRUE;
}
