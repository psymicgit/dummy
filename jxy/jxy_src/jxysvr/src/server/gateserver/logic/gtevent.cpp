
#include "gtevent.h"
#include "framework/gtclient.h"
#include "framework/gtapplication.h"
#include <sdutil.h>
#include "basehttpcmd.h"
#include "httpmgr.h"
#include "noticehttpcmd.h"

CGTEvent::CGTEvent()
{

}

CGTEvent::~CGTEvent()
{
}

UINT32 CGTEvent::OnEvent()
{   
    switch (m_dwType)
    {
    case EVENTCHECK_SESSION:
        CheckSession();
        break;
	case EVENT_NOTICE:
		LoadNotice();
		break;
    default:
        break;
    }
    return m_dwType;
}

INT32  CGTEvent::OnDestroy()
{
    return m_dwType;
}

VOID CGTEvent::CheckSession()
{
     CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
     poGTApplication->GetClientMgr()->ChkSessionOnTimer();
}

VOID	CGTEvent::LoadNotice()
{
	CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_NOTICE);
	if(NULL == poHttpCmd)
	{
		DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
		return;
	}

	SNoticeInfo	stNoticeInfo;
	CSDServerID	oServerID(((CGTApplication*)SDGetApp())->GetLocalID());
	stNoticeInfo.wZoneID = oServerID.GetAreaID();
	poHttpCmd->SetUserData((void*)&stNoticeInfo, sizeof(SNoticeInfo), 0);
	poHttpCmd->Init();
	if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
	{
		DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
		CHttpMgr::Instance()->FreeCmd(poHttpCmd);
		return;
	}
}

VOID CGTEvent::SetType(const UINT32 dwType)
{
    m_dwType = dwType;
}

