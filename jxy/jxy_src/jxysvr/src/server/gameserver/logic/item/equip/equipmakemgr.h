///<------------------------------------------------------------------------------
//< @file:   item\equip\equipmakemgr.h
//< @author: hongkunan
//< @date:   2014年8月25日 17:15:33
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _equipmakemgr_h_
#define _equipmakemgr_h_

#include "logic/base/singleton.h"
#include "logic/base/basepropmgr.h"
#include "logic/fly/flydef.h"
#include "logic/record/recordmgr.h"
#include "protocol/server/protocligs.h"
#include <set>

// 对应于每个制作卷的装备和材料列表
struct SEquipMakeProp
{
    SEquipMakeProp(){ wMakeBookID = 0; wSeriesID = 0; wEquipID = 0; vecMaterial.clear(); dwCostCoin = 0; }

    EquipMakeBookID wMakeBookID; // 制作卷ID
    SeriesID wSeriesID;          // 装备所属系列
    EquipID wEquipID;            // 制作出的装备ID
    MaterialVec vecMaterial;     // 所需材料列表
    
    UINT32 dwCostCoin;           // 所需铜钱
};

// 每个装备系列的配置
struct SSeriesProp
{
    typedef std::set<EquipID> EquipList;

    SeriesID wSeriesID; // 该系列的ID
    UINT16 wLevel; // 该系列所属等级
    std::string m_strSeriesName; // 该系列名称

    EquipList m_equips; // 该系列的装备列表
};

class CPlayer;

// 
class CEquipMakePropMgr
    : public Singleton<CEquipMakePropMgr>
    , public CBasePopMgr
{
public:
    typedef std::map<EquipMakeBookID, SEquipMakeProp> EquipMakeMap; // <装备制作卷道具ID，对应的配置>
    typedef std::map<SeriesID, SSeriesProp> SeriesMap;

public:
    ///< @impl >
    BOOL ReLoadProp(){ return TRUE; }

    EConfigModule GetConfigModuleType() { return ECM_PET; }

    BOOL Init();
    VOID UnInit(){}

public:
    const SEquipMakeProp* GetMakeProp(EquipMakeBookID);
    SeriesID GetSeriesIDOfEquip(EquipID);

    UINT32 GetCostGoldOfMaterial(GoodsID);
    UINT32 GetCostCoinOfBook(GoodsID);

    SSeriesProp* GetSeriesProp(SeriesID);

    const EquipMakeMap& GetEquipMakeCfg(){ return m_mapMakeCfg; }

private:
    BOOL LoadEquipMakeProp();
    BOOL LoadSeriesProp();

private:
    EquipMakeMap m_mapMakeCfg; // 装备制作卷配置表
    SeriesMap m_mapSeries; // 装备系列表
};

class CEquipMakeMgr
{
public:
    static BOOL TakeEquipMakeBookInfo(CPlayer &oPlayer, GoodsID makeBookID, DT_EQUIP_MAKE_BOOK&);
    static BOOL TakeMaterialListCli(CPlayer &oPlayer, const MaterialVec&, DT_MATERIAL_LIST&);

    static BOOL CostMaterialList(CPlayer &oPlayer, const DT_MATERIAL_LIST&, CRecordMgr::EDecItemType wRecordType, BOOL bBuyMaterial = FALSE/*是否用元宝补齐剩余不足的材料*/);
};

#endif //_equipmakemgr_h_