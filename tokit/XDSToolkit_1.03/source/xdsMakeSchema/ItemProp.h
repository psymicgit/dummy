#ifndef _ItemProp_h_
#define _ItemProp_h_

#include <map>

struct ItemProp
{
  unsigned char Id;
  unsigned char IsElite;
  unsigned char GoldCost;
  unsigned char Exp;
};

ItemProp* GetItemProp(unsigned char IsElite, unsigned char GoldCost, unsigned char Exp);

void LoadItemProp();

extern struct ItemProp * g_ItemProps;
extern int g_lenItemProp;

typedef Hash_Map<uint64_t, ItemProp*> ItemPropMap;
extern ItemPropMap* g_mapItemProps;

#endif // _ItemProp_h_

