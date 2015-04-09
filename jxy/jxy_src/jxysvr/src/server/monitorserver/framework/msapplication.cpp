

#include "sdutil.h"
#include "framework/msapplication.h"
#include "common/server/utility.h"
#include "protocol/server/protomsgm.h"


CSDApplication* SDGetApp()
{
    static CMSApplication oNSApplication;
    return &oNSApplication;
}

CMSApplication::CMSApplication():
    m_oKeepAliveReactor(1, EXECUTE_LOOSE),
	m_poGmPipeChannel(NULL)
{

}

CMSApplication::~CMSApplication()
{

}

BOOL CMSApplication::Init(const TCHAR* pszConfigFilename)
{
    if (FALSE == CSDApplication::Init(pszConfigFilename))
    {
        return FALSE;
    }
    printf(_SDT("monitorserver::Init successful\n"));
    if (FALSE == m_oKSConfig.Init(pszConfigFilename))
    {
        return FALSE;
    }

    CKeepAliveEvent* poKeepAliveEvent = m_oKeepAliveReactor.CreateEvent();
    m_oKeepAliveReactor.SetEvent(poKeepAliveEvent, m_oKSConfig.GetKeepAlive(), SDTimeSecs() + 20); //5秒检测1次

	if(FALSE == CMonitor::CreateInstance()) 
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CMonitor CreateInstance failed"), MSG_MARK);
		return FALSE;
	}

    if (FALSE == CMonitor::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CMonitor Init failed"), MSG_MARK);
        return FALSE;
    }

	printf(_SDT("CMSApplication::Init successful\n"));
	//获取传入的参数
	//CSDCommandLine* poCommandLine = GetCmdLine();
	//if (NULL == poCommandLine)
	//{
	//	return TRUE;
	//}

	//CSDCommandLine::CCommand * poCommand = NULL;
	//poCommand = poCommandLine->GetFirstCommand();
	//while (NULL != poCommand)
	//{
	//	if (0 == SDStrcmp(poCommand->GetCommand(), "-q"))
	//	{
	//		CMonitor::Instance()->Quit();
	//		return FALSE;
	//	}
	//	poCommand = poCommandLine->GetNextCommand();
	//}

    return TRUE;
}

VOID CMSApplication::UnInit()
{
    CMonitor::Instance()->UnInit();
    CSDApplication::UnInit();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();

}

BOOL CMSApplication::Run()
{
    CSDApplication::Run();
    m_oKeepAliveReactor.Run();
    SDSleep(1);
    return TRUE;
}

CSDPipeChannel* CMSApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel*	poPipeChannel = NULL;
    CSDServerID		oServerID(dwPipeID);
    if (TYPE_GM_SERVER == oServerID.GetServerType())
    {
        poPipeChannel = SDNew CSDPipeChannel;
		poPipeChannel->SetPacketProcessor(&m_oGmProcessor);
		m_poGmPipeChannel = poPipeChannel;
        printf(_SDT("monitorserver:recv gm server pipe connect\n"));
        SYS_CRITICAL(_SDT("monitorserver:recv gm server pipe connect\n"));
    }
    else
    {
        printf("monitorserver:recv unknown pipe connect(Type:%d)\n", oServerID.GetServerType());
        SYS_CRITICAL(_SDT("monitorserver:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
    }

    return poPipeChannel;
}


VOID CMSApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
	if (poChannel)
	{
		CSDServerID		oServerID(poChannel->GetPipeID());
		if (TYPE_GM_SERVER == oServerID.GetServerType())
		{
			m_poGmPipeChannel = NULL;
		}
		SDDelete poChannel;
		poChannel = NULL;
	}
}

VOID CMSApplication::OnHandleUserCmd(const TCHAR* szCmd)
{
    if(!DealUserCmd(szCmd))
    {
        CMSApplication::OnHandleUserCmd(szCmd);
    }
}

VOID CMSApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
    CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}

BOOL	CMSApplication::DealUserCmd(const TCHAR* szCmd)
{
    SYS_CRITICAL(_SDT("[%s: %d]: DealUserCmd[%s]"), MSG_MARK, szCmd);
    return TRUE;
}

BOOL CMSApplication::SendMsg2GMServer(UINT32 dwMsgID, const CHAR* pBuf)
{
	if (NULL != m_poGmPipeChannel)
	{
		return m_poGmPipeChannel->SendMsg(0x00, dwMsgID, pBuf);
	}
	return FALSE;
}




