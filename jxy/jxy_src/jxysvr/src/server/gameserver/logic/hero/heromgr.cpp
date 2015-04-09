
#include "heromgr.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <sdstring.h>
#include "heropropmgr.h"
#include <sdutil.h>
#include <sdloggerimpl.h>

#include <logic/base/logicfunc.h>

IMPLEMENT_SINGLETON(CHeroMgr)

CHeroMgr::CHeroMgr()
{
	
}

CHeroMgr::~CHeroMgr()
{
	CHeroPropMgr::DestroyInstance();	

}

BOOL CHeroMgr::Init()
{
    //读取配置文件

	// 初始化CHeroPropMgr管理器
	if(FALSE == CHeroPropMgr::CreateInstance())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr::CreateInstance failed!"), MSG_MARK);        
		return FALSE;
	}
	if(FALSE == CHeroPropMgr::Instance()->Init())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr Init failed!"), MSG_MARK);        
		return FALSE;
	}


	return TRUE;

}

VOID CHeroMgr::UnInit()
{
	CHeroPropMgr::Instance()->UnInit();
}


CHero* CHeroMgr::CreateHero(UINT32 dwPlayerID, UINT16 wKindID)
{
	CHero* poHero = m_oHeroShmemObjMgr.Alloc();
	if(NULL == poHero)
	{
		return NULL;
	}
	UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wKindID);
	m_mapID2Hero[qwKey] = poHero;

	return poHero;
}

VOID CHeroMgr::RemoveHero(UINT32 dwPlayerID, UINT16 wKindID)
{
	UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wKindID);

	CID2HeroMapItr itr = m_mapID2Hero.find(qwKey);
	if (m_mapID2Hero.end() != itr)
    {
        CHero* poHero = itr->second;
		m_oHeroShmemObjMgr.Free(poHero);
        m_mapID2Hero.erase(itr);
    }	   
}


CHero* CHeroMgr::FindHero(UINT32 dwPlayerID, UINT16 wKindID)
{
	UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wKindID);

	CID2HeroMapItr itr = m_mapID2Hero.find(qwKey);
	if (m_mapID2Hero.end() != itr)
	{
		return itr->second;
	}

	return NULL;
}

BOOL CHeroMgr::SwapHero(UINT32 dwPlayerID, UINT16 wHeroID1, UINT16 wHeroID2)
{
    CHero *poHero1 = FindHero(dwPlayerID, wHeroID1);
    CHero *poHero2 = FindHero(dwPlayerID, wHeroID2);
    if(NULL == poHero1 || NULL == poHero2){
        return FALSE;
    }

    UINT64 qwKey1 = CLogicFunc::GetObjKey(dwPlayerID, wHeroID1);
    UINT64 qwKey2 = CLogicFunc::GetObjKey(dwPlayerID, wHeroID2);

    m_mapID2Hero[qwKey1] = poHero2;
    m_mapID2Hero[qwKey2] = poHero1;

    return TRUE;
}
