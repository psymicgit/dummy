#include <stdio.h>
#include <string.h>
#include "XDSliteAPI.h"

#include <ActivityProp.h>
#include <AwardProp.h>
#include <BagProp.h>
#include <EquipMasterProp.h>
#include <ItemProp.h>
#include <NickFilterProp.h>

ActivityPropMap* g_mapActivityProps;
ActivityProp*    g_ActivityProps   = NULL;
int g_lenActivityProp = 0;

AwardPropMap* g_mapAwardProps;
AwardProp*    g_AwardProps   = NULL;
int g_lenAwardProp = 0;

BagPropMap* g_mapBagProps;
BagProp*    g_BagProps   = NULL;
int g_lenBagProp = 0;

EquipMasterPropMap* g_mapEquipMasterProps;
EquipMasterProp*    g_EquipMasterProps   = NULL;
int g_lenEquipMasterProp = 0;

ItemPropMap* g_mapItemProps;
ItemProp*    g_ItemProps   = NULL;
int g_lenItemProp = 0;

NickFilterPropMap* g_mapNickFilterProps;
NickFilterProp*    g_NickFilterProps   = NULL;
int g_lenNickFilterProp = 0;


void LoadActivityProp()
{
   propapi::loadprop("ActivityProp.xds", "ActivityProp.dsd", (void**)&g_ActivityProps, g_ActivityPropCnt);
}

void LoadAwardProp()
{
   propapi::loadprop("AwardProp.xds", "AwardProp.dsd", (void**)&g_AwardProps, g_AwardPropCnt);
}

void LoadBagProp()
{
   propapi::loadprop("BagProp.xds", "BagProp.dsd", (void**)&g_BagProps, g_BagPropCnt);
}

void LoadEquipMasterProp()
{
   propapi::loadprop("EquipMasterProp.xds", "EquipMasterProp.dsd", (void**)&g_EquipMasterProps, g_EquipMasterPropCnt);
}

void LoadItemProp()
{
   propapi::loadprop("ItemProp.xds", "ItemProp.dsd", (void**)&g_ItemProps, g_ItemPropCnt);
}

void LoadNickFilterProp()
{
   propapi::loadprop("NickFilterProp.xds", "NickFilterProp.dsd", (void**)&g_NickFilterProps, g_NickFilterPropCnt);
}


ActivityProp* GetActivityProp(bool IsElite, unsigned long GoldCost, unsigned long Exp)
{
    uint64_t key = Get161616Key(IsElite, GoldCost, Exp);

    ActivityPropMap::iterator itr = g_mapActivityProp.find(key);
    if(itr == g_mapActivityProp.end()
    {
        return NULL;
    }
    
    ActivityProp *p = *itr->second;
    return p;
}

AwardProp* GetAwardProp(bool IsElite, unsigned long GoldCost, unsigned long Exp)
{
    uint64_t key = Get161616Key(IsElite, GoldCost, Exp);

    AwardPropMap::iterator itr = g_mapAwardProp.find(key);
    if(itr == g_mapAwardProp.end()
    {
        return NULL;
    }
    
    AwardProp *p = *itr->second;
    return p;
}

BagProp* GetBagProp(bool IsElite, unsigned long GoldCost, unsigned long Exp)
{
    uint64_t key = Get161616Key(IsElite, GoldCost, Exp);

    BagPropMap::iterator itr = g_mapBagProp.find(key);
    if(itr == g_mapBagProp.end()
    {
        return NULL;
    }
    
    BagProp *p = *itr->second;
    return p;
}

EquipMasterProp* GetEquipMasterProp(bool IsElite, unsigned long GoldCost, unsigned long Exp)
{
    uint64_t key = Get161616Key(IsElite, GoldCost, Exp);

    EquipMasterPropMap::iterator itr = g_mapEquipMasterProp.find(key);
    if(itr == g_mapEquipMasterProp.end()
    {
        return NULL;
    }
    
    EquipMasterProp *p = *itr->second;
    return p;
}

ItemProp* GetItemProp(bool IsElite, unsigned long GoldCost, unsigned long Exp)
{
    uint64_t key = Get161616Key(IsElite, GoldCost, Exp);

    ItemPropMap::iterator itr = g_mapItemProp.find(key);
    if(itr == g_mapItemProp.end()
    {
        return NULL;
    }
    
    ItemProp *p = *itr->second;
    return p;
}

NickFilterProp* GetNickFilterProp(bool IsElite, unsigned long GoldCost, unsigned long Exp)
{
    uint64_t key = Get161616Key(IsElite, GoldCost, Exp);

    NickFilterPropMap::iterator itr = g_mapNickFilterProp.find(key);
    if(itr == g_mapNickFilterProp.end()
    {
        return NULL;
    }
    
    NickFilterProp *p = *itr->second;
    return p;
}


