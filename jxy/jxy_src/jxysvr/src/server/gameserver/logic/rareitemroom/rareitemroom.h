#ifndef	_RARE_ITEM_ROOM_ 
#define	_RARE_ITEM_ROOM_

#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <logic/faction/faction.h>
#include <map>
using std::map;
//重置规则
enum {BYDAY = 1, BYWEEK = 2, BYMONTH = 3, BYNONE = 4};
enum {EQUITMENT = 1, MEDICINE = 2, RESOURCE = 3, SCENE = 4, SKILL = 5};
typedef map<UINT32, DT_RAREITEM_GOODS_INFO> CMapRareItemGoods;
typedef CMapRareItemGoods::iterator CMapRareItemGoodsItr;

class CPlayer;
class CRareItemRoom:public CBaseObj
{
public:
	CRareItemRoom();
	~CRareItemRoom();
public:
	// 初始化相关对象
	BOOL Init(DT_RAREITEM_GOODS_DATA &stDT_RAREITEM_GOODS_DATA, CPlayer* poOwner);
	DT_RAREITEM_GOODS_DATA& GetDT_RAREITEM_GOODS_DATA();// { return m_stDT_RAREITEM_GOODS_DATA; }
	UINT64 GetUpdateTime(){return m_qwUpdateTime;}
	VOID SetUpdateTime(UINT64 qwUpdataTime) 
	{
		m_qwUpdateTime = qwUpdataTime;
	}
	CMapRareItemGoods& GetRareItemMap(){ return m_mapRareItemGoods; }
	//////////////////////////////////////////////////////////////////////////
	UINT32 GetOpenRareItem( DT_RAREITEM_ROOM_INFO_CLI& stRareItemRoomInfo, DT_RAREITEM_INFO_LIST_CLI& stRareItemInfo );
	UINT32 ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID);
	UINT32 ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID, UINT8 byLeftTime);
	UINT32 OpenUpgradeRareItem(DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeCli);
	UINT32 UpgradeRareItem(PKT_CLIGS_UPGRADE_RAREITEM_ACK& stAck);
	VOID GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt);

protected:

	BOOL InitRareItemRoom( DT_RAREITEM_GOODS_DATA& that  );
	UINT32 GetNextUpgradeRequireInfo( DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo);
	VOID GetAfterUpgradeInfo(DT_AFTER_UPGRADE_INFO_CLI& stAfterUpgradeInfo);
	VOID UpdateDataByTime();
	VOID ResetByDay( DT_RAREITEM_GOODS_INFO& stRareItemInfo );
	VOID ResetByMonth( DT_RAREITEM_GOODS_INFO& stRareItemInfo );
	VOID ResetByWeek( DT_RAREITEM_GOODS_INFO& stRareItemInfo );
	VOID SetRareItemInfo( DT_RAREITEM_INFO_LIST_CLI& stCli);
	BOOL SetSingleRareItemInfo(UINT16 wRareItemID, DT_RAREITEM_INFO_CLI& stCli);
	VOID GetRareItemRoomInfo( DT_RAREITEM_ROOM_INFO_CLI& stRareItemRoomInfo);
	UINT32 CheckExchangeInfo(UINT32 dwGoodsID);
	BOOL IsOpen();
	CFaction* GetFaction();
	UINT32 GetFactionID();
private:
	CMapRareItemGoods m_mapRareItemGoods;
	UINT64 m_qwUpdateTime;
	DT_RAREITEM_GOODS_DATA	m_stDT_RAREITEM_GOODS_DATA;
};
#endif