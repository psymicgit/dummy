

#include "sdutil.h"
#include "framework/gtapplication.h"
#include "net/common/svrpipechannel.h"
#include "net/cli/cliprocessor.h"
#include "net/ls/lsprocessor.h"
#include "net/gs/gsprocessor.h"
#include <common/server/utility.h>
#include "logic/httpmgr.h"
#include "logic/basehttpcmd.h"
#include "logic/noticehttpcmd.h"
#include "logic/noticemgr.h"


CGTApplication::CGTApplication():
    m_oReactor(1, EXECUTE_LOOSE)
{
    m_poListener = NULL;
    //m_poLSPipeChannel = NULL;
    m_poGSPipeChannel = NULL;
    m_poGSPipeChannel_1 = NULL;
    m_poGSPipeChannel_2 = NULL;
    m_nConnectNum = 0;
}

CGTApplication::~CGTApplication()
{

}

BOOL CGTApplication::Init(const TCHAR* pszConfigFilename)
{
    if (FALSE == CSDApplication::Init(pszConfigFilename))
    {
        return FALSE;
    }
    PrintScreen(_SDT("CSDApplication::Init successful"));
    if (FALSE == m_oGTConfig.Init(pszConfigFilename))
    {
        return FALSE;
    }
	////////////////////////初始化管理类//////////////////////////////////////////////////
	m_oGTClientMgr.Init(GetConfig()->GetMaxUsers());
	//////////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined (WIN64)
    m_poListener = GetNetModule()->CreateListener(NETIO_COMPLETIONPORT);
#else
    m_poListener = GetNetModule()->CreateListener(NETIO_EPOLL);
#endif
    m_poListener->SetPacketParser(&m_oCliPacketParser);
    m_poListener->SetSessionFactory(&m_oGTClientMgr);
    m_poListener->SetBufferSize(GetConfig()->GetRecvBufferSize(), GetConfig()->GetSendBufferSize());
    if (m_poListener->Start(GetConfig()->GetListenIp().c_str(),
                            GetConfig()->GetListenPort()) != TRUE)
    {
        SYS_CRITICAL(_SDT("Listener Start Failed"));
        return FALSE;
    }

    CGTEvent* poCheckEvent = m_oReactor.CreateEvent();
    poCheckEvent->SetType(EVENTCHECK_SESSION);
    m_oReactor.SetEvent(poCheckEvent, m_oGTConfig.GetCkSessionTimerElapse());

    //每30秒取一次公告
    CGTEvent* poNoticeEvent = m_oReactor.CreateEvent();
    poNoticeEvent->SetType(EVENT_NOTICE);
    m_oReactor.SetEvent(poNoticeEvent, 30);


    //
    if(FALSE == CNoticeMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("CNoticeMgr CreateInstance Failed"));
        return FALSE;
    }
    if(FALSE == CNoticeMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("CNoticeMgr Init Failed"));
        return FALSE;
    }

    //
    if(FALSE == CHttpMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("CHttpMgr CreateInstance Failed"));
        return FALSE;
    }
    if(FALSE == CHttpMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("CHttpMgr Init Failed"));
        return FALSE;
    }

    //CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_LOGIN);
    //if(NULL == poHttpCmd)
    //{
    //    DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
    //    return FALSE;
    //}

    //SLoginInfo stLoginInfo;
    //poHttpCmd->SetUserData((void*)&stLoginInfo, sizeof(SLoginInfo), 0);
    //if(!poHttpCmd->Init())
    //{
    //    DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
    //    CHttpMgr::Instance()->FreeCmd(poHttpCmd);
    //    return FALSE;
    //}
    //if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    //{
    //    DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
    //    CHttpMgr::Instance()->FreeCmd(poHttpCmd);
    //    return FALSE;
    //}

    CBaseHttpCMD *poNoticeCmd = CHttpMgr::Instance()->CreateCmd(EHCT_NOTICE);
    if(NULL == poNoticeCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    }

    SNoticeInfo stNoticeInfo;
	CSDServerID	oServerID(((CGTApplication*)SDGetApp())->GetLocalID());
	stNoticeInfo.wZoneID = oServerID.GetAreaID();
    poNoticeCmd->SetUserData((void*)&stNoticeInfo, sizeof(SNoticeInfo), 0);
    poNoticeCmd->Init();
    if(FALSE == CHttpMgr::Instance()->AddCommand(poNoticeCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poNoticeCmd);
        return FALSE;
    }

    return TRUE;
}

VOID CGTApplication::UnInit()
{
    if(m_poListener) m_poListener->Stop();
    CSDApplication::UnInit();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();
}

BOOL CGTApplication::Run()
{
    CSDApplication::Run();
    m_oReactor.Run();
    CHttpMgr::Instance()->Process();
    SDSleep(1);
    return TRUE;
}

CSDPipeChannel* CGTApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel*	pSDPipeChannel = NULL;
    CSDServerID		oServerID(dwPipeID);


    if (TYPE_LOGIN_SERVER == oServerID.GetServerType())
    {
        //pSDPipeChannel = SDNew CSvrPipeChannel;
        //pSDPipeChannel->SetPacketProcessor(&m_oLSPacketProcessor);
        //m_poLSPipeChannel = pSDPipeChannel;

        printf(_SDT("GateServer:recv login server pipe connect\n"));
        SYS_CRITICAL(_SDT("[%s: %d]:recv login server pipe connect\n"), MSG_MARK);
    }
    else if (TYPE_GAME_SERVER == oServerID.GetServerType())
    {
        pSDPipeChannel = SDNew CSvrPipeChannel;
        pSDPipeChannel->SetPacketProcessor(&m_oGSPacketProcessor);
        if(NULL == m_poGSPipeChannel_1)
        {
            m_poGSPipeChannel_1 = pSDPipeChannel;
        }
        else if(NULL == m_poGSPipeChannel_2)
        {
            m_poGSPipeChannel_2 = pSDPipeChannel;
        }

        if(NULL == m_poGSPipeChannel)
        {
            m_poGSPipeChannel = pSDPipeChannel;
        }

        printf(_SDT("GateServer:recv game server pipe connect\n"));
        SYS_CRITICAL(_SDT("GateServer:recv game server pipe connect\n"));
    }
    else
    {
        printf(_SDT("GateServer:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
        SYS_CRITICAL(_SDT("GateServer:recv unknown pipe connect(Type:%d)\n"), oServerID.GetServerType());
    }

    return pSDPipeChannel;
}


VOID CGTApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
    if (TYPE_LOGIN_SERVER == poChannel->GetSvrType())
    {
        //printf(_SDT("GateServer:recv login server pipe disconnect\n"));
        //SYS_CRITICAL(_SDT("[%s: %d]:recv login server pipe disconnect\n"), MSG_MARK);
        //m_poLSPipeChannel = NULL;
    }
    if (TYPE_GAME_SERVER == poChannel->GetSvrType())
    {
        printf(_SDT("GateServer:recv game server pipe disconnect\n"));
        SYS_CRITICAL(_SDT("[%s: %d]:recv game server pipe disconnect\n"), MSG_MARK);
        if(m_poGSPipeChannel_1 == poChannel)
        {
            m_poGSPipeChannel_1 = NULL;
        }
        if(m_poGSPipeChannel_2 == poChannel)
        {
            m_poGSPipeChannel_2 = NULL;
        }
        if(m_poGSPipeChannel == poChannel)
        {
            m_poGSPipeChannel = NULL;
        }
        if(NULL == m_poGSPipeChannel)
        {
            m_poGSPipeChannel = NULL != m_poGSPipeChannel_1 ? m_poGSPipeChannel_1 : m_poGSPipeChannel;
            m_poGSPipeChannel = NULL != m_poGSPipeChannel_2 ? m_poGSPipeChannel_2 : m_poGSPipeChannel;
        }
        if(NULL == m_poGSPipeChannel)
        {
            m_oGTClientMgr.GSDisConnect(poChannel->GetPipeID());
        }
    }
    if (poChannel)
    {
        SDDelete poChannel;
        poChannel = NULL;
    }
}

VOID CGTApplication::OnHandleUserCmd(const TCHAR* szCmd)
{
    CSDApplication::OnHandleUserCmd(szCmd);
}

VOID CGTApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
    CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}


CGTClientMgr* CGTApplication::GetClientMgr()
{
    return &m_oGTClientMgr;
}

CSDApplication* SDGetApp()
{
    static CGTApplication oGTApplication;
    return &oGTApplication;
}

VOID CGTApplication::IncConnectNum()
{
    CHAR szBuff[32] = {0};
    //SDAtomicInc32(&m_nConnectNum);
    //sprintf(szBuff, "ConnectNum:%d", m_nConnectNum);
    m_nConnectNum = m_oGTClientMgr.GetSessionNum();
    sprintf(szBuff, "ConnectNum:%d", m_nConnectNum);
    UpdateScreen(EFLI_CONNECT_NUM, _SDT(szBuff));
}

VOID CGTApplication::DecConnectNum()
{
    CHAR szBuff[32] = {0};
    //SDAtomicDec32(&m_nConnectNum);
    //sprintf(szBuff, "ConnectNum:%d", m_nConnectNum);
    m_nConnectNum = m_oGTClientMgr.GetSessionNum();
    sprintf(szBuff, "ConnectNum:%d", m_nConnectNum);
    UpdateScreen(EFLI_CONNECT_NUM, _SDT(szBuff));
}

