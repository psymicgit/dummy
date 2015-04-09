

#include "sdutil.h"
#include "framework/nsapplication.h"
#include "net/gs/gsprocessor.h"
#include "common/server/utility.h"
#include "apns/notifications.h"
#include "logic/gmmgr.h"


CSDApplication* SDGetApp()
{
    static CNSApplication oNSApplication;
    return &oNSApplication;
}

CNSApplication::CNSApplication():
    m_oTwoDaysNotOnlineNotityReactor(1, EXECUTE_LOOSE),
	m_oGetPlayerNotifyInfoEventReactor(1, EXECUTE_LOOSE)
{
    m_poFeedBack = NULL;
    m_poNotifications = NULL;
}

CNSApplication::~CNSApplication()
{

}

BOOL CNSApplication::Init(const TCHAR* pszConfigFilename)
{
    if (FALSE == CSDApplication::Init(pszConfigFilename))
    {
        return FALSE;
    }
    PrintScreen(_SDT("noticeserver::Init successful"));
    if (FALSE == m_oNSConfig.Init(pszConfigFilename))
    {
		SYS_CRITICAL(_SDT("[%s: %d]:Config Init failed"), MSG_MARK);
        return FALSE;
    }

    if(FALSE == m_oDBMgr.Init())
    {
		SYS_CRITICAL(_SDT("[%s: %d]:DBMgr Init failed"), MSG_MARK);
        return FALSE;
    }

    CTwoDaysNotOnlineNotityEvent* poTwoDaysNotOnlineNotityEvent = m_oTwoDaysNotOnlineNotityReactor.CreateEvent();
    m_oTwoDaysNotOnlineNotityReactor.SetEvent(poTwoDaysNotOnlineNotityEvent, poTwoDaysNotOnlineNotityEvent->GetInterval()); //5Ãë¼ì²â1´Î

	CGetPlayerNotifyInfoEvent* poGetPlayerNotifyInfoEvent = m_oGetPlayerNotifyInfoEventReactor.CreateEvent();
	m_oGetPlayerNotifyInfoEventReactor.SetEvent(poGetPlayerNotifyInfoEvent, poGetPlayerNotifyInfoEvent->GetInterval()); //5Ãë¼ì²â1´Î

    if(FALSE == CGmMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CreateInstance CGmMgr Init failed"), MSG_MARK);
        return FALSE;
    }

    if (FALSE == CGmMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]:Init CGmMgr Init failed"), MSG_MARK);
        return FALSE;
    }

#if defined(WIN32) || defined (WIN64)
    m_poListener = GetNetModule()->CreateListener(NETIO_COMPLETIONPORT);
#else
    m_poListener = GetNetModule()->CreateListener(NETIO_EPOLL);
#endif
    m_poListener->SetPacketParser(&m_oCliPacketParser);
    m_poListener->SetSessionFactory(&m_oksclientMgr);
    m_poListener->SetBufferSize(GetConfig()->GetRecvBufferSize(), GetConfig()->GetSendBufferSize());
    if (m_poListener->Start(GetConfig()->GetListenIp().c_str(),
                            GetConfig()->GetListenPort()) != TRUE)
    {
        SYS_CRITICAL(_SDT("Listener Start Failed"));
        return FALSE;
    }
    m_oksclientMgr.Init();

    //////////////////////////////////////////////////////////////////////////
    //m_poFeedBack = new CFeedBack;
    //m_poFeedBack->SSL_Set(APPLE_FEEDBACK_HOST, APPLE_FEEDBACK_PORT,
    //                     ((CNSApplication*)SDGetApp())->GetNSConfig()->Get_RSA_CLIENT_CERT(),
    //                     ((CNSApplication*)SDGetApp())->GetNSConfig()->Get_RSA_CLIENT_KEY(),
    //                     ((CNSApplication*)SDGetApp())->GetNSConfig()->Get_CA_CERT_PATH());
    //if(FALSE == m_poFeedBack->SSL_Connect())
    //{
    //    SYS_CRITICAL(_SDT("[%s: %d]:SSL_Connect CFeedBack failed"), MSG_MARK);
    //    return FALSE;
    //}
    //if(FALSE == m_poFeedBack->OnStart())
    //{
    //    SYS_CRITICAL(_SDT("[%s: %d]:OnStart CFeedBack failed"), MSG_MARK);
    //    return FALSE;
    //}

    m_poNotifications = new CNotifications;
    m_poNotifications->SSL_Set(APPLE_HOST, APPLE_PORT,
                               ((CNSApplication*)SDGetApp())->GetNSConfig()->Get_RSA_CLIENT_CERT(),
                               ((CNSApplication*)SDGetApp())->GetNSConfig()->Get_RSA_CLIENT_KEY(),
                               ((CNSApplication*)SDGetApp())->GetNSConfig()->Get_CA_CERT_PATH());

	if(FALSE == m_poNotifications->SSL_Connect())
	{
		SYS_CRITICAL(_SDT("[%s: %d]:SSL_Connect CNotifications failed"), MSG_MARK);
		return FALSE;
	}
    if(FALSE == m_poNotifications->OnStart())
    {
        SYS_CRITICAL(_SDT("[%s: %d]:OnStart CNotifications failed"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CNSApplication::UnInit()
{
    CSDApplication::UnInit();
    m_oDBMgr.Release();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();
}

BOOL CNSApplication::Run()
{
    CSDApplication::Run();
    m_oDBMgr.Run();
    m_oTwoDaysNotOnlineNotityReactor.Run();
	m_oGetPlayerNotifyInfoEventReactor.Run();
    SDSleep(1);
    return TRUE;
}

CSDPipeChannel* CNSApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel*	poPipeChannel = NULL;
    CSDServerID		oServerID(dwPipeID);
    if (TYPE_GAME_SERVER == oServerID.GetServerType())
    {
		poPipeChannel = SDNew CSDPipeChannel;
		poPipeChannel->SetPacketProcessor(&m_oGSProcessor);
		CPipeChannelMapItr itr = m_mapPipeChannel.find(dwPipeID);
		if (itr != m_mapPipeChannel.end())
		{
			SDDelete itr->second;
		}
		m_mapPipeChannel[dwPipeID] = poPipeChannel;

		printf(_SDT("noticeserver:recv game server pipe connect\n"));
		SYS_CRITICAL(_SDT("noticeserver:recv game server pipe connect\n"));
    }
	else
	{
		printf(_SDT("noticeserver:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
		SYS_CRITICAL(_SDT("noticeserver:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
	}
    return poPipeChannel;
}


VOID CNSApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
    if (poChannel)
    {
        CSDServerID oServerID(poChannel->GetPipeID());
        CPipeChannelMapItr itr = m_mapPipeChannel.find(poChannel->GetPipeID());
        if (itr != m_mapPipeChannel.end())
        {
            m_mapPipeChannel.erase(itr);
        }
        SDDelete poChannel;
        poChannel = NULL;
    }
}

VOID CNSApplication::OnHandleUserCmd(const TCHAR* szCmd)
{
    if(!DealUserCmd(szCmd))
    {
        CSDApplication::OnHandleUserCmd(szCmd);
    }
}

VOID CNSApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
    CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}

BOOL	CNSApplication::DealUserCmd(const TCHAR* szCmd)
{
    SYS_CRITICAL(_SDT("[%s: %d]: DealUserCmd[%s]"), MSG_MARK, szCmd);
    return TRUE;
}


void	CNSApplication::SetPlayerNotifyInfoMap(CPlayerNotifyInfoMap vValues)
{
	m_mapPlayerNotifyInfo.clear();
	m_mapPlayerNotifyInfo = vValues;
}





