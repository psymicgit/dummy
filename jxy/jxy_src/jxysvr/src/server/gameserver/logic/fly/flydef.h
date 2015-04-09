///<------------------------------------------------------------------------------
//< @file:   logic\fly\flydef.h
//< @author: hongkunan
//< @date:   2014年8月27日 20:24:47
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _flydef_h_
#define _flydef_h_
#include "sdtype.h"
#include <vector>
#include <memory.h>
using namespace std;
typedef UINT16 GoodsID; // 物品ID，对应数据库goodsprop表
typedef GoodsID EquipMakeBookID; // 制作卷的物品ID

typedef UINT16 EquipID; // 装备ID，对应数据库equipprop表
typedef UINT16 SeriesID; // 装备系列ID，对应数据库equipseriesprop表的SeriesID字段

// 材料配置
struct SMaterialProp
{
    SMaterialProp(){ memset(this, 0, sizeof(*this)); }

    UINT8 byMaterialType; // 材料类型，分为：EIK_EQUIP、EIK_GOODS
    UINT16 wMaterialID; // 物品ID或是装备ID，根据材料类型决定
    UINT16 wMaterialCnt; // 所需材料数
};

typedef std::vector<SMaterialProp> MaterialVec; // 材料列表

#define FLY_REQUIRE_JINGJIE_LVL 25 // 飞升所需境界等级
#define ENCHANT_STONE_ITEM_ID 10000 // 附魔石的物品ID

#endif //_flydef_h_