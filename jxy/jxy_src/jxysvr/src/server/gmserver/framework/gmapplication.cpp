

#include "sdutil.h"
#include "framework/gmapplication.h"
#include "net/gs/gsprocessor.h"
#include "common/server/utility.h"
#include "protocol/server/protogmgs.h"
#include "protocol/server/protomsgm.h"

CSDApplication* SDGetApp()
{
    static CGMApplication oNSApplication;
    return &oNSApplication;
}

CGMApplication::CGMApplication():
    m_oReactor(1, EXECUTE_LOOSE)
{

}

CGMApplication::~CGMApplication()
{

}

BOOL CGMApplication::Init(const TCHAR* pszConfigFilename)
{
    if (FALSE == CSDApplication::Init(pszConfigFilename))
    {
        return FALSE;
    }
    printf(_SDT("GmServer::Init successful\n"));
    if (FALSE == m_oKSConfig.Init(pszConfigFilename))
    {
        return FALSE;
    }

#if defined(WIN32) || defined (WIN64)
    m_poListener = GetNetModule()->CreateListener(NETIO_COMPLETIONPORT);
#else
    m_poListener = GetNetModule()->CreateListener(NETIO_EPOLL);
#endif
    m_poListener->SetPacketParser(&m_oJsonPacketParser);
    m_poListener->SetSessionFactory(&m_oGmClientMgr);
    m_poListener->SetBufferSize(GetConfig()->GetRecvBufferSize(), GetConfig()->GetSendBufferSize());
    if (m_poListener->Start(GetConfig()->GetListenIp().c_str(),
                            GetConfig()->GetListenPort()) != TRUE)
    {
        SYS_CRITICAL(_SDT("Listener Start Failed"));
        return FALSE;
    }

    //if(FALSE == m_oDBMgr.Init())
    //{
    //    return FALSE;
    //}

    if(FALSE == m_oGmClientMgr.Init())
    {
        return FALSE;
    }

    CGMEvent* poCheckEvent = m_oReactor.CreateEvent();
    m_oReactor.SetEvent(poCheckEvent, 5); //5秒检测1次

    return TRUE;
}

VOID CGMApplication::UnInit()
{
    CSDApplication::UnInit();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();
}

BOOL CGMApplication::Run()
{
    CSDApplication::Run();
    m_oReactor.Run();
    SDSleep(1);
    return TRUE;
}

CSDPipeChannel* CGMApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel*	poPipeChannel = NULL;
    CSDServerID		oServerID(dwPipeID);

    if (TYPE_GAME_SERVER == oServerID.GetServerType() || TYPE_PK_SERVER == oServerID.GetServerType())	//游戏服务器
    {
        poPipeChannel = SDNew CSDPipeChannel;
        poPipeChannel->SetPacketProcessor(&m_oGSProcessor);
        CPipeChannelMapItr itr = m_mapPipeChannel.find(dwPipeID);
        if (itr != m_mapPipeChannel.end())
        {
            SDDelete itr->second;
        }
        m_mapPipeChannel[dwPipeID] = poPipeChannel;

        printf("GmServer:recv game server pipe connect, AreaID:%d, GroupID:%d\n", oServerID.GetAreaID(), oServerID.GetGroupID());
        SYS_CRITICAL(_SDT("GmServer:recv game server pipe connect\n"));
    }
    else if (TYPE_MS_SERVER == oServerID.GetServerType()) //监控服务器
    {
        poPipeChannel = SDNew CSDPipeChannel;
        poPipeChannel->SetPacketProcessor(&m_oMsProcessor);
        CPipeChannelMapItr itr = m_mapPipeChannel.find(dwPipeID);
        if (itr != m_mapPipeChannel.end())
        {
            SDDelete itr->second;
        }
        m_mapPipeChannel[dwPipeID] = poPipeChannel;

        printf("GmServer:recv monitor server pipe connect, AreaID:%d, GroupID:%d\n", oServerID.GetAreaID(), oServerID.GetGroupID());
        SYS_CRITICAL(_SDT("GmServer:recv monitor server pipe connect\n"), MSG_MARK);
    }
    else
    {
        printf("GmServer:recv unknown pipe connect(Type:%d)\n", oServerID.GetServerType());
        SYS_CRITICAL(_SDT("GmServer:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
    }
    return poPipeChannel;
}


VOID CGMApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
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

VOID CGMApplication::OnHandleUserCmd(const TCHAR* szCmd)
{
    if(!DealUserCmd(szCmd))
    {
        CGMApplication::OnHandleUserCmd(szCmd);
    }
}

VOID CGMApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
    CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}

BOOL	CGMApplication::DealUserCmd(const TCHAR* szCmd)
{
    SYS_CRITICAL(_SDT("[%s: %d]: DealUserCmd[%s]"), MSG_MARK, szCmd);
    return TRUE;
}

BOOL CGMApplication::SendMsg2GameServer(UINT32 dwArea, UINT32 dwPlayerID, const CHAR* pBuf)
{
    CPipeChannelMapItr itr = m_mapPipeChannel.begin();
    for (itr; itr != m_mapPipeChannel.end(); itr++)
    {
        CSDServerID oServerID(itr->first);
        if((TYPE_GAME_SERVER == oServerID.GetServerType() || TYPE_PK_SERVER == oServerID.GetServerType()) && dwArea == oServerID.GetAreaID())
        {
            CSDPipeChannel* poChannel = itr->second;
            if (NULL != poChannel)
            {
                return poChannel->SendMsg(dwPlayerID, GMGS_PARM_INFO_REQ, pBuf);
            }
        }
    }
    return FALSE;
}

BOOL CGMApplication::SendMsg2MonitorServer(UINT32 dwMsgID, UINT32 dwArea, const CHAR* pBuf)
{
    CPipeChannelMapItr itr = m_mapPipeChannel.begin();
    for (itr; itr != m_mapPipeChannel.end(); itr++)
    {
        CSDServerID oServerID(itr->first);
        if(TYPE_MS_SERVER == oServerID.GetServerType() && oServerID.GetAreaID() == dwArea)
        {
            CSDPipeChannel* poChannel = itr->second;
            if (NULL != poChannel)
            {
                return poChannel->SendMsg(dwArea, dwMsgID, pBuf);
            }
        }
    }
    return FALSE;
}




