#include "releasecaptureevent.h"
#include <logic/player/playermgr.h>
#include <logic/plunder/plundermgr.h>

CReleaseCaptureEvent::CReleaseCaptureEvent()
{
	m_dwCaptureID = 0;
	m_qwCaptureTime = 0;
}

CReleaseCaptureEvent::~CReleaseCaptureEvent()
{
}

UINT32 CReleaseCaptureEvent::OnEvent()
{

	//DBG_INFO( _SDT( "[%s: %d]: ¶¨Ê±Æ÷ Íæ¼ÒID:%u, ·ýÂ²:%u *****************" ), MSG_MARK, m_dwOwnerID, m_dwCaptureID );

	CPlunderMgr::Instance()->OnReleaseCaptureEvent(m_dwOwnerID, m_dwCaptureID);

	return 0;
}

BOOL CReleaseCaptureEvent::OnDestroy()
{
	return TRUE;
}

VOID CReleaseCaptureEvent::SetParam(UINT32 dwOwnerID, UINT32 dwCaptureID)
{
	m_dwOwnerID = dwOwnerID;
	m_dwCaptureID = dwCaptureID;
}

