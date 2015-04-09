
#ifndef _EQUIPCOMPOSE_H_
#define _EQUIPCOMPOSE_H_


#include <logic/base/baseobj.h>
#include <logic/base/basedefine.h>
#include <common/server/utility.h>
#include "protocol/server/protocligs.h"

struct tagDT_EQUIP_COMPOSE_LEVEL_DATA_CLI;
typedef tagDT_EQUIP_COMPOSE_LEVEL_DATA_CLI DT_EQUIP_COMPOSE_LEVEL_DATA_CLI;

struct SComposeEquipProp;

using namespace std ;


class CPlayer;
class CEquipCompse: public CBaseObj
{
public:
    CEquipCompse();
    ~CEquipCompse();
public:
    //初始化相关对象
    BOOL Init(DT_EQUIP_COMPOSE_DATA& stDT_EQUIP_COMPOSE_DATA, CPlayer* poOwner);
    DT_EQUIP_COMPOSE_DATA& GetDT_EQUIP_COMPOSE_DATA() { return m_stDT_EQUIP_COMPOSE_DATA; }
	DT_EQUIP_LEVEL_COMPOSE_DATA* GetDT_EQUIP_LEVEL_COMPOSE_DATA(UINT16 wEquipLevel);
	DT_EQUIP_LEVEL_COMPOSE_DATA* SetNewDT_EQUIP_LEVEL_COMPOSE_DATA(UINT16 wEquipLevel);
	VOID GetDT_EQUIP_COMPOSE_LEVEL_DATA_CLI(SComposeEquipProp& stComposeEquipProp, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI& stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI);
	UINT16 GetComposeCountDown();

	UINT16 OpenEquipComposeTab(UINT8& byEquipLevelNum, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI astEquipLevelInfo[MAX_EQUIPLEVEL_NUM], UINT16& wComposeCountDown);
	UINT16 EquipCompose(UINT16 wEquipLevel, UINT8 byCurColorFlag, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI& stEquipLevelInfo, UINT16& wComposeCountDown);
	UINT16 RecvComposeEquip(UINT16 wEquipLevel, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI& stEquipLevelInfo, DT_EQUIP_DATA_CLI& stRecvEquipInfo, DT_EQUIP_DATA_STRENGTHEN_CLI& stRecvEquipStrengthenInfo, UINT32& dwStrengthen10Coin);

protected:	

private:
    DT_EQUIP_COMPOSE_DATA	m_stDT_EQUIP_COMPOSE_DATA;
	map<UINT16, DT_EQUIP_LEVEL_COMPOSE_DATA*> m_mapDT_EQUIP_LEVEL_COMPOSE_DATA;
};
DECLARE_CLASS_POOL_MGR(CEquipCompse)

#endif //#ifndef 

