
#ifndef _SHOPMGR_H_
#define _SHOPMGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <sddb.h>
#include <logic/base/basedefine.h>
#include <vector>
#include <logic/base/basepropmgr.h>
#include <protocligs.h>

using namespace SGDP;
using namespace std;

//商店tab类型
enum EShopTabType
{
	ESTT_INVALID = 0,
	ESTT_HOTSELL = 1,
	ESTT_EQUIP	= 2,
	ESTT_GOODS	= 3,
	ESTT_COIN	= 4,
	ESTT_OTHER	= 5,
	ESTT_PILENUM= 99,//指定叠加数购买.可买所有
	ESTT_MAX,
};

//商店道具叠加数及等级
struct SShopPileLevel
{
	UINT16 wPileNum; //道具叠加数
	UINT16 wRequireLevel; //道具购买需要等级
};

struct SShopItemLevel
{
	UINT16 wItemID; //道具ID
	UINT16 wPileNum; //道具叠加数
	UINT16 wRequireLevel; //道具购买需要等级
};

//商店道具下发信息
struct SShopItemCliData
{
	UINT8				byItemNum; //道具信息
	DT_ITEM_DATA_SHOP	astItemDataList[MAX_SHOP_ITEM_NUM]; //道具信息
};

//
typedef HashMap<UINT32, SShopPileLevel> CKey2PileLevelMap;//key为道具ID+叠加数
typedef CKey2PileLevelMap::iterator CKey2PileLevelMapItr;
typedef vector<SShopItemLevel> CItemIDLevelVector;//道具ID等级
typedef CItemIDLevelVector::iterator CItemIDLevelVectorItr;

typedef HashMap<UINT16, UINT16> CKindID2KindIDMap;//key为道具ID，value也是key为道具ID
typedef CKindID2KindIDMap::iterator CKindID2KindIDMapItr;

typedef HashMap<UINT16, SShopItemCliData> CLevel2ItemCliDataMap;//key为玩家等级
typedef CLevel2ItemCliDataMap::iterator CLevel2ItemCliDataMapItr;

class CPlayer;
class CShopMgr : public CBasePopMgr
{
public:
	DECLARE_SINGLETON_PROPMGR(CShopMgr)

	virtual EConfigModule	GetConfigModuleType() { return ECM_SHOP; }

	BOOL Init();
	VOID UnInit();	
public:
	UINT16 OpenShopByTab(UINT16 wPlayerLevel, UINT8 byTabID, UINT8& byItemNum, DT_ITEM_DATA_SHOP astItemDataList[]);
	UINT16 Buy(CPlayer* poPlayer, UINT8 byTabID, UINT16 wItemKindID, UINT16 wPileCount);
	UINT8  GetTabID(UINT16 wItemKindID, UINT8 & byTabID);
public:
	CShopMgr();
	~CShopMgr();
protected:
	BOOL LoadFromDB();

	BOOL InitShopData();
private:	
	CKey2PileLevelMap		m_amapShopTabProp[ESTT_MAX];	////商店配置
	CItemIDLevelVector		m_avecShopTabProp[ESTT_MAX];	////商店配置
	CLevel2ItemCliDataMap	m_amapLevelItemCliData[ESTT_MAX];	////每个玩家等级的商店配置
	CKindID2KindIDMap		m_mapAllSellItem;				//所有的可买的道具
	CKindID2KindIDMap		m_mapAllSellItemTab;

};



#endif
