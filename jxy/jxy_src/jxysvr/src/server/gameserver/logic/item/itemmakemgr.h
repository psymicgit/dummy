
#ifndef _ITEMMAKEMGR_H
#define _ITEMMAKEMGR_H

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>

using namespace std;
using namespace SGDP;

class CGoods;
class CPlayer;
class CEquip;

typedef HashMap<CGoods*, UINT16> CGoods2NumMap; //value为物品道具个数
typedef CGoods2NumMap::iterator	CGoods2NumMapItr;


class CItemMakeMgr
{
public:

    DECLARE_SINGLETON(CItemMakeMgr);

public:

    BOOL Init();
    VOID UnInit();

	UINT16 MakeItem(CPlayer* poPlayer, UINT16 wMakerItemKindID, UINT16 wEquipKindID, UINT16 wEquipIdx, DT_ITEM_DATA_CLI2& stItemInfo, TCHAR aszErrInfo[MAX_COMMON_TXT_NUM]);

	UINT16 ComposeEquip(CPlayer* poPlayer, UINT16 EquipItemID, UINT16 wEquipIdx, UINT8 byGoldComposeFlag, PKT_CLIGS_COMPOSE_EQUIP_ACK* pstAck);
	
	UINT16 ComposeGoods(CPlayer* poPlayer, UINT16 wItemKindID, DT_ITEM_DATA_CLI2 & stItemInfo);

	UINT16 ComposeSeed(CPlayer* poPlayer, UINT16 wItemKindID, DT_ITEM_DATA_CLI2 & stDT_ITEM_DATA_CLI2);

	UINT16 OpenGemMaker(CPlayer* poPlayer, UINT8& byMakerNum, DT_GEM_MAKER_DATA astGemMakerInfo[MAX_GEM_LEVEL]); 
	UINT16 OpenGemMakerEx(CPlayer* poPlayer, UINT8& byMakerNum, DT_GEM_MAKER_DATA astGemMakerInfo[MAX_GEM_LEVEL_EX]); 
public:
	const static INT32 ms_nXiangqianTabCompose=1; //在镶嵌面板发起合成请求
protected:
	UINT16 EstimateItem(UINT16 wProduceItemKindID, DT_ESTIMATE_ITEM_DATA& stEstimateItemInfo, UINT16 wStrenthenLevel = 1, UINT8 byColor = 1);//预计制造成的道具
	UINT16 MakeEquip(CPlayer* poPlayer, CGoods* poMakerGoods, SEquipProp* pstProduceEquipProp, CEquip* poNeedEquip, CGoods2NumMap& mapGoods2Num, DT_ITEM_DATA_CLI2& stItemInfo, TCHAR aszErrInfo[MAX_COMMON_TXT_NUM]);
	UINT16 MakeGoods(CPlayer* poPlayer, CGoods* poMakerGoods, SGoodsProp* pstProduceGoodsProp, CGoods2NumMap& mapGoods2Num, DT_ITEM_DATA_CLI2& stItemInfo, TCHAR aszErrInfo[MAX_COMMON_TXT_NUM]);
	UINT16 MakeGem(CPlayer* poPlayer, CGoods* poMakerGoods, SGoodsProp* pstMakerGoodsProp, DT_ITEM_DATA_CLI2& stItemInfo,  TCHAR aszErrInfo[MAX_COMMON_TXT_NUM]);
protected:
    CItemMakeMgr();
    virtual ~CItemMakeMgr();


private:
};

#endif
