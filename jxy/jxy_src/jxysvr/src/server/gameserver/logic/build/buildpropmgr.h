
#ifndef _BUILDSORTPROPMGR_H_
#define _BUILDSORTPROPMGR_H_



#include <common/client/commondef.h>
#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>

#include <protocligs.h>

struct SMaintownLevyProp
{
	UINT8 byLevyNumPerDay;
	UINT32 dwLevyCD;
};

typedef std::map<UINT16, UINT32> CPlayerLevel2CrownMap;
typedef CPlayerLevel2CrownMap::iterator CPlayerLevel2CrownMapItr;


typedef std::map<UINT8, UINT16> CPos2EquipIDMap; //key装备位置，value为装备ID
typedef CPos2EquipIDMap::iterator CPos2EquipIDMapItr;

//建筑顺序配置
typedef std::map<UINT8, UINT8> CBuildKind2SortMap; //key为建筑类型，value为建筑顺序
typedef CBuildKind2SortMap::iterator CBuildKind2SortMapItr;
class CBuildPropMgr : public CBasePopMgr
{
public:

	DECLARE_SINGLETON_PROPMGR(CBuildPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_BUILD ; }

	BOOL Init();

	VOID UnInit();
public:
	UINT8 GetBuildSort(UINT8 byBuildKindID);
	UINT32 GetMaintownCrownCoin(UINT16 wPlayerLevel);
	UINT32 GetMaintownCrownGold(UINT16 wPlayerLevel);
	UINT32 GetMaintownLevyCoin(UINT16 wPlayerLevel);
	SMaintownLevyProp* GetMaintownLevyProp() { return &m_stMaintownLevyProp; }
	VOID GetBuyEquipCliData(DT_BUY_EQUIP_DATA astByEquipInfo[EQUIP_POS_NUM]) { memcpy(astByEquipInfo, &m_astByEquipInfo, sizeof(m_astByEquipInfo)); }
	VOID GetDT_BUY_EQUIP_DATA(UINT8 byEquipPos, DT_BUY_EQUIP_DATA& stDT_BUY_EQUIP_DATA) { if((byEquipPos < EEPT_HEAD) ||  (byEquipPos > EEPT_FOOT)) return;  memcpy(&stDT_BUY_EQUIP_DATA, &m_astByEquipInfo[byEquipPos-1], sizeof(DT_BUY_EQUIP_DATA)); }
public:
	CBuildPropMgr();
	virtual ~CBuildPropMgr();
protected:
	
	BOOL LoadSortPropFromDB();
	BOOL LoadMaintownCrownPropFromDB();
	BOOL LoadMaintownLevyPropFromDB();
	BOOL LoadSmithyPropFromDB();

	///////////////////////////////////////////////////
	BOOL InitBuyEquipCliData();

	///////////////////////////////////////////////////
private:
	CBuildKind2SortMap	m_mapBuildKind2Sort;
	SMaintownLevyProp	m_stMaintownLevyProp;
	CPlayerLevel2CrownMap m_mapPlayerLevel2CrownCoin;
	CPlayerLevel2CrownMap m_mapPlayerLevel2CrownGold;
	CPlayerLevel2CrownMap m_mapPlayerLevel2LevyCoin;
	CPos2EquipIDMap		  m_mapPos2EquipID;
	DT_BUY_EQUIP_DATA	  m_astByEquipInfo[EQUIP_POS_NUM];
};

#endif // 
