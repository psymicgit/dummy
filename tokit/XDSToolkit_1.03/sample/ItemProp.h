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

extern struct ItemProp *g_ItemProps;

#endif // _ItemProp_h_

