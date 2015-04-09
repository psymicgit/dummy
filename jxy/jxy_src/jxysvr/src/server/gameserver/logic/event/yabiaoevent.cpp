#include "yabiaoevent.h"
#include <logic/yabiao/yabiaomgr.h>

CYabiaoEvent::CYabiaoEvent()
{
	m_dwYabaoPlayerID=0;
}

CYabiaoEvent::~CYabiaoEvent()
{

}

UINT32 CYabiaoEvent::OnEvent()
{
	CYabiaoMgr::Instance()->DeletePlayer( m_dwYabaoPlayerID );
	return 0;
}

INT32  CYabiaoEvent::OnDestroy()
{
	return 0;
}

VOID CYabiaoEvent::SetParam( UINT32 dwYabiaoPlayerID )
{
	m_dwYabaoPlayerID=dwYabiaoPlayerID;
}