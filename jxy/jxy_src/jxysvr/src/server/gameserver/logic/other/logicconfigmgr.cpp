#include "logicconfigmgr.h"
#include <common/server/utility.h>
#include <logic/base/basepropmgr.h>
#include <logic/bag/bagpropmgr.h>
#include <logic/build/buildpropmgr.h>
#include <logic/chat/chatmgr.h>
#include <logic/crop/croppropmgr.h>
#include <logic/encourage/loginencouragemgr.h>
#include <logic/encourage/onlineencouragemgr.h>
#include <logic/formation/formationpropmgr.h>
#include <logic/godweapon/godweaponbasepropmgr.h>
#include <logic/godweapon/godweaponpropmgr.h>
#include <logic/hero/heromgr.h>
#include <logic/instance/instancepropmgr.h>
#include <logic/item/itemmgr.h>

#include <logic/monster/monstermgr.h>
#include <logic/onhook/onhookmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/other/hpgridmgr.h>
#include <logic/other/phystrengthmgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/plunder/plundermgr.h>
#include <logic/race/racemgr.h>
#include <logic/science/scienceupdatepropmgr.h>
#include <logic/shop/shopmgr.h>
#include <logic/skill/skillmgr.h>
#include <logic/talent/drugmgr.h>
#include <logic/task/taskmgr.h>
#include <logic/updateattr/updateattrpropmgr.h>
#include <logic/yabiao/yabiaomgr.h>
#include <logic/jingjie/jingjiepropmgr.h>
#include <logic/other/xiangqianmgr.h>
#include <logic/awaken/awakenpropmgr.h>
#include <logic/activity/eatbreadpropmgr.h>
#include <logic/item/itemcomposepropmgr.h>
#include <logic/item/equip/strengthenpropmgr.h>
#include <logic/moneytree/moneytreepropbasemgr.h>
#include <logic/moneytree/moneytreepropmgr.h>
#include <logic/race/racepropmgr.h>


BOOL CLogicConfigMgr::ReloadCfg(EConfigModule eModuleKind)
{
	if(ECM_ALL == eModuleKind)
	{
		for(map<UINT16, CBasePopMgr*>::iterator itr = CBasePopMgr::m_mapKind2Mgr.begin(); itr != CBasePopMgr::m_mapKind2Mgr.end(); itr++)
		{
			itr->second->ReLoadProp();
		}
	}
	else
	{
		map<UINT16, CBasePopMgr*>::iterator itr = CBasePopMgr::m_mapKind2Mgr.find(eModuleKind);
		if(itr != CBasePopMgr::m_mapKind2Mgr.end())
		{
			return itr->second->ReLoadProp();
		}
	}
	
	return FALSE;
}
