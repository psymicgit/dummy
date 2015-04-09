
#include "plundercapturemgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <protocol/server/protocommondata.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playermgr.h>
#include <logic/event/timereventmgr.h>
#include <logic/base/logicfunc.h>

using namespace SGDP;


IMPLEMENT_SINGLETON(CPlunderCaptureMgr)

CPlunderCaptureMgr::CPlunderCaptureMgr()
{

}

CPlunderCaptureMgr::~CPlunderCaptureMgr()
{

}


BOOL CPlunderCaptureMgr::Init()
{
	
	return TRUE;

}

VOID CPlunderCaptureMgr::UnInit()
{
	
}



CPlunderCapture* CPlunderCaptureMgr::CreatePlunderCapture(UINT32 dwPlayerID, UINT32 dwCapturePlayerID)
{
	CPlunderCapture* poPlunderCapture = m_oPlunderCaptureShmemObjMgr.Alloc();
	if(NULL == poPlunderCapture)
	{
		return NULL;
	}
	m_mapID2PlunderCapture[CLogicFunc::Get3232Key(dwPlayerID, dwCapturePlayerID)] = poPlunderCapture;

	return poPlunderCapture;
}

VOID CPlunderCaptureMgr::RemovePlunderCapture(UINT32 dwPlayerID, UINT32 dwCapturePlayerID)
{
	CID2PlunderCaptureMapItr itr = m_mapID2PlunderCapture.find(CLogicFunc::Get3232Key(dwPlayerID, dwCapturePlayerID));
	if (m_mapID2PlunderCapture.end() != itr)
	{
		m_oPlunderCaptureShmemObjMgr.Free(itr->second);
		m_mapID2PlunderCapture.erase(itr);
	}	   
}
