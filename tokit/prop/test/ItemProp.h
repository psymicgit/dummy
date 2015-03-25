#ifndef _ItemProp_h_
#define _ItemProp_h_

#include <map>
#include <vector>

struct ItemProp
{
    ItemProp();
    word KindID;
    std::string Name;
    std::string Introduction;
    word MainKindID;
    word UseLevel;
    dword BuyCoin;
    dword BuyGold;
    dword RecoverPrice;
    word UseTimesPerDay;
    word UseCD;
    dword Value;
    word ComposeCount;
    word ComposeKindID;
    dword Param1;
    dword Param2;
    dword Param3;
    dword Param4;
};

ItemProp* GetItemProp(word KindID, word UseLevel, dword BuyCoin);

void LoadItemProp(const char* xml);

typedef std::map<uint64_t, ItemProp*> ItemPropMap;
typedef std::vector<ItemProp> ItemPropVec;

extern ItemPropVec g_vecItemProp;
extern ItemPropMap g_mapItemProp;

#endif // _ItemProp_h_

