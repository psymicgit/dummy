#ifndef _RAREITEMROOM_PROP_MGR_
#define _RAREITEMROOM_PROP_MGR_

#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <protocol/server/protocligs.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>
#include <map>

using std::map;
using namespace SGDP;
#define MSG_COL_LEN 32

//物品配置
struct SRareItemProp
{
	UINT32 dwKindID;
	UINT32 dwBuyCost;
	UINT32 dwSumDoorsTributeRequire;
	UINT8  byRareItemType; //物品类型(1装备,2药品,3资源,4道具,5技能)
	UINT8  byRareItemExchangeRule; //物品兑换规则(1按日 2按周 3按月 4永久)
	UINT8  byBuyNeedLevel;
	SRareItemProp(){memset(this, 0, sizeof(SRareItemProp));}
};

struct SRareItemRoomProp
{
	UINT8 byRareItemGrade;		//珍品阁等级
	UINT32 qwUnitedFunds;		//要求门派资金
	SRareItemRoomProp() { memset( this, 0, sizeof(SRareItemRoomProp)); }
};

struct SRareItemNumberInfo
{
	UINT8 byRareItemLevel;
	UINT16 wRareItemID;
	UINT32 dwCurMaxRareItem;
};

typedef multimap<UINT8, SRareItemNumberInfo> CMapRareItemNumber;
typedef CMapRareItemNumber::iterator CMapRareItemNumberItr;
typedef pair<CMapRareItemNumberItr, CMapRareItemNumberItr> CMapRareItemNumberPair;

typedef map<UINT32, SRareItemNumberInfo> UpgradeRareItemMap;
typedef UpgradeRareItemMap::iterator UpgradeRareItemItr;
typedef UpgradeRareItemMap::const_iterator UpgradeRareItemConstItr;

typedef map<UINT32, UpgradeRareItemMap> UpgradeRareItemListMap;
typedef UpgradeRareItemListMap::iterator UpgradeRareItemListItr;

typedef map<UINT32, SRareItemProp> SRareItemPropMap;
typedef map<UINT32, SRareItemProp>::iterator SRareItemPropItr;
typedef map<UINT8, SRareItemRoomProp> SRareItemRoomPropMap;
typedef map<UINT8, SRareItemRoomProp>::iterator SRareItemRoomPropItr; 



//珍品阁管理类
class CRareItemRoomPropMgr: public CBasePopMgr
{

	DECLARE_SINGLETON_PROPMGR( CRareItemRoomPropMgr );
public:
	virtual EConfigModule GetConfigModuleType() { return ECM_RAREITEMROOM; }
	BOOL Init();
	VOID UnInit();
	//获取配置
	BOOL GetConfig(SRareItemProp& stRareItem, UINT32 dwRareItemID);
	//获取物品规则
	UINT8 GetRareItemRule(UINT32 dwRareItemID);
	//获取升级信息
	BOOL GetUpgradeInfo(UINT32 dwRareItemGrade, UINT32& dwConsumptionMoney);
	const UpgradeRareItemMap* GetRareItemInfoByFaction(UINT32 dwRareItemLevel);

	
public:
	CRareItemRoomPropMgr(){}
	virtual ~CRareItemRoomPropMgr(){}
protected:
	BOOL LoadFromDB();
	BOOL LoadUpgradeInfoFromDB();
	BOOL LoadRareItemNumberFromDB();
	VOID UpdataNumberInfo();
private:
	CMapRareItemNumber m_mapRareItemNumber;


	SRareItemPropMap m_mapRareItemProp;  //以珍品ID为KEY, 当前珍品阁等级下的物品配置
	SRareItemRoomPropMap m_mapRareItemRoomProp;	//以珍品阁等级为KEY, 当前珍品阁升级要求配置
	UpgradeRareItemListMap m_mapUpgradeRareItemList; //第一个MAP以珍品阁等级为KEY,第两个MAP以珍品ID为KEY, 各珍品阁等级下的物品上限配置

};



#endif