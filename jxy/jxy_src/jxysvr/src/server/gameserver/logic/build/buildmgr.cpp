
#include "build.h"
#include "buildmgr.h"
#include <sdstring.h>
#include "buildpropmgr.h"
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/base/logicfunc.h>

IMPLEMENT_SINGLETON(CBuildMgr)


CBuildMgr::CBuildMgr()
{

}

CBuildMgr::~CBuildMgr()
{
	CBuildPropMgr::DestroyInstance();
}

BOOL CBuildMgr::Init()
{
	//读取配置文件

	// 初始化CBuildPropMgr管理器
	if(FALSE == CBuildPropMgr::CreateInstance())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CBuildPropMgr::CreateBuild failed!"), MSG_MARK);        
		return FALSE;
	}
	if(FALSE == CBuildPropMgr::Instance()->Init())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CBuildPropMgr Init failed!"), MSG_MARK);        
		return FALSE;
	}

	
	return TRUE;

}


VOID CBuildMgr::UnInit()
{
	m_mapID2Build.clear();

	//////////////////////////////
	
	CBuildPropMgr::Instance()->UnInit();
	CBuildPropMgr::Instance()->UnInit();

}

CBuild* CBuildMgr::CreateBuild(UINT32 dwPlayerID, UINT16 wKindID)
{
	CBuild* poBuild = m_oBuildShmemObjMgr.Alloc();
	if(NULL == poBuild)
	{
		return NULL;
	}
	UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wKindID);
	m_mapID2Build[qwKey] = poBuild;

	return poBuild;
}

VOID CBuildMgr::RemoveBuild(UINT32 dwPlayerID, UINT16 wKindID)
{
	UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wKindID);
	CID2BuildMapItr itr = m_mapID2Build.find(qwKey);
	UINT32 dwDBID = 0;
	if (m_mapID2Build.end() != itr)
	{
		CBuild* poBuild = itr->second;
		m_oBuildShmemObjMgr.Free(poBuild);	
		m_mapID2Build.erase(itr);
	}
}
