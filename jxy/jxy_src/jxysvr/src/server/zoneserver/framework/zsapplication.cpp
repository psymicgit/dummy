

#include "sdutil.h"
#include "framework/zsapplication.h"
#include "net/cli/cliprocessor.h"
#include "usermgr.h"
#include "logic/zsevent.h"
#include "http/httpmgr.h"
#include "http/basehttpcmd.h"

#include <fstream>
#include <sstream>
#include <ostream>
using namespace std;
#define MAX_DATA_NUM		10000
#define MAX_SQL_LEN		10240000 //sql最大长度

CZSApplication::CZSApplication(): m_oReactor(1, EXECUTE_LOOSE)
{
    m_poListener = NULL;
    ms_nMaxDataNum = MAX_DATA_NUM;
}

CZSApplication::~CZSApplication()
{

}

BOOL CZSApplication::Init(const TCHAR* pszConfigFilename)
{
    if(FALSE == CUserMgr::CreateInstance())
    {
        printf(_SDT("[%s: %d]: CUserMgr CreateInstance failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CUserMgr CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }

    if (FALSE == CHttpMgr::CreateInstance())
    {
        printf(_SDT("[%s: %d]: CHttpMgr CreateInstance failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CHttpMgr CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }


    if (FALSE == CSDApplication::Init(pszConfigFilename))
    {
        printf(_SDT("[%s: %d]: CSDApplication Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CSDApplication Init failed!"), MSG_MARK);

        return FALSE;
    }

    if (FALSE == m_oZSConfig.Init(pszConfigFilename))
    {
        printf(_SDT("[%s: %d]: CZSConfig Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CZSConfig Init failed!"), MSG_MARK);

        return FALSE;
    }

    //客户端连接
#if defined(WIN32) || defined (WIN64)
    m_poListener = GetNetModule()->CreateListener(NETIO_COMPLETIONPORT);
#else
    m_poListener = GetNetModule()->CreateListener(NETIO_EPOLL);
#endif
    m_poListener->SetPacketParser(&m_oCliPacketParser);
    m_poListener->SetSessionFactory(&m_oZSClientMgr);
    m_poListener->SetBufferSize(GetConfig()->GetRecvBufferSize(), GetConfig()->GetSendBufferSize());
    if (m_poListener->Start(GetConfig()->GetListenIp().c_str(),
                            GetConfig()->GetListenPort()) != TRUE)
    {
        printf(_SDT("[%s: %d]: Listener Start Failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: Listener Start Failed!"), MSG_MARK);
        return FALSE;
    }
    m_oZSClientMgr.Init();


    //GM连接
#if defined(WIN32) || defined (WIN64)
    m_poGmListener = GetNetModule()->CreateListener(NETIO_COMPLETIONPORT);
#else
    m_poGmListener = GetNetModule()->CreateListener(NETIO_EPOLL);
#endif
    m_poGmListener->SetPacketParser(&m_oRCPacketParser);
    m_poGmListener->SetSessionFactory(&m_oRCClientMgr);
    m_poGmListener->SetBufferSize(m_oZSConfig.GetGMListenerInfo()->m_dwRecvBuf, m_oZSConfig.GetGMListenerInfo()->m_dwSendBuf);
    if (m_poGmListener->Start(m_oZSConfig.GetGMListenerInfo()->m_szIP, m_oZSConfig.GetGMListenerInfo()->m_wPort) != TRUE)
    {
        printf(_SDT("[%s: %d]: GM Listener Start Failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: GM Listener Start Failed!"), MSG_MARK);
        return FALSE;
    }
    m_oRCClientMgr.Init();


    //if(FALSE == m_oDBMgr.Init())
    //{
    //    printf(_SDT("[%s: %d]: CDBMgr Init failed!"), MSG_MARK);
    //    SYS_CRITICAL(_SDT("[%s: %d]: CDBMgr Init failed!"), MSG_MARK);
    //    return FALSE;
    //}


    if(FALSE == CUserMgr::Instance()->Init())
    {
        printf(_SDT("[%s: %d]: CUserMgr Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CUserMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    CZSEvent* poCheckSessionEvent = m_oReactor.CreateEvent();
    poCheckSessionEvent->SetType(CHK_SESSIONON_TIMER);
    m_oReactor.SetEvent(poCheckSessionEvent, 5); //5秒检测1次

    CZSEvent* poSyncDBEvent = m_oReactor.CreateEvent();
    poSyncDBEvent->SetType(CHK_SYNC_DB_TIMER);
    m_oReactor.SetEvent(poSyncDBEvent, 20);		//20秒检测1次
    poSyncDBEvent->SyncDB();					//马上执行一次同步数据

    CZSEvent* poCheckUserEvent = m_oReactor.CreateEvent();
    poCheckUserEvent->SetType(CHK_USER_TIMER);
    m_oReactor.SetEvent(poCheckUserEvent, 30);	//20秒检测1次


    CZSEvent* poSaveDbEvent = m_oReactor.CreateEvent();
    poSaveDbEvent->SetType(CHK_SAVE_DB_TIMER);
    m_oReactor.SetEvent(poSaveDbEvent, 30);		//20秒检测1次

    CZSEvent* poMobileEvent = m_oReactor.CreateEvent();
    poMobileEvent->SetType(CHK_MOBILE_TIMER);
    m_oReactor.SetEvent(poMobileEvent, 1);		//20秒检测1次


    if(FALSE == CHttpMgr::Instance()->Init())
    {
        printf(_SDT("[%s: %d]: CHttpMgr Init failed!"), MSG_MARK);
        SYS_CRITICAL(_SDT("[%s: %d]: CHttpMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    PrintScreen(_SDT("zoneserver Init successful"));
    printf(_SDT("zoneserver Init successful"));


    //测试专用
    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_REPORTUSERDEVICEINFO);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ stReq;
    SDStrncpy(stReq.aszDeviceID, "123456789987654321", DEVICEID_LEN);
    SDStrncpy(stReq.aszPlatform, "123456789987654321", MAX_INFO_NUM);
    SDStrncpy(stReq.aszGameVersion, "2.9.3", MAX_INFO_NUM);
    SDStrncpy(stReq.aszOSType, "aszOSType", MAX_INFO_NUM);
    SDStrncpy(stReq.aszOSVersion, "aszOSVersion", MAX_INFO_NUM);
    SDStrncpy(stReq.aszHardwareVersion, "aszHardwareVersion", MAX_INFO_NUM);
    stReq.aszExtInfo[0] = 1;
    stReq.aszExtInfo[1] = strlen("123456789");
    SDStrncpy(stReq.aszExtInfo + 2, "123456789", MAX_USER_DEVIDE_INFO_LEN - 2);

    poHttpCmd->SetUserData((void*)&stReq, sizeof(stReq), 0);
    poHttpCmd->Init();
    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }

	PKT_CLIZS_GET_VERSION_NOTICE_REQ stReq1;
	poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_GET_VERSION_NOTICE);
	if(NULL == poHttpCmd)
	{
		DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
		return FALSE;
	}

	poHttpCmd->SetUserData((void*)&stReq1, sizeof(stReq1), 0);
	poHttpCmd->Init();

	if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
	{
		DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
		CHttpMgr::Instance()->FreeCmd(poHttpCmd);
		return FALSE;
	}

    return TRUE;
}

VOID CZSApplication::UnInit()
{
    SaveData();

    if(m_poListener)
        m_poListener->Stop();
    CSDApplication::UnInit();
    //m_oDBMgr.Release();
    CSDApplication::UnInitLog();
    CSDApplication::UnInitConsole();
}

BOOL CZSApplication::Run()
{
    CSDApplication::Run();
    //m_oDBMgr.Run();
    m_oReactor.Run();
    CHttpMgr::Instance()->Process();
    SDSleep(1);
    return TRUE;
}

CSDPipeChannel* CZSApplication::OnPipeChannelConnected(UINT32 dwPipeID)
{
    CSDPipeChannel* poPipeChannel = NULL;
    poPipeChannel = SDNew CSDPipeChannel;
    CSDServerID oServerID(dwPipeID);
    printf(_SDT("loginserver:recv unknown pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
    DBG_INFO(_SDT("loginserver:recv unknown pipe connect(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
    return poPipeChannel;
}


VOID CZSApplication::OnPipeChannelDisconnected(CSDPipeChannel* poChannel)
{
    if (poChannel)
    {
        CSDServerID		oServerID(poChannel->GetPipeID());
        printf(_SDT("loginserver:recv gate server pipe disconnected(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
        DBG_INFO(_SDT("loginserver:recv gate server pipe disconnected(Type:%u, AreaID:%u)\n"), oServerID.GetServerType(), oServerID.GetAreaID());
        SDDelete poChannel;
        poChannel = NULL;
    }
}

VOID CZSApplication::OnHandleUserCmd(const TCHAR* szCmd)
{
    CSDApplication::OnHandleUserCmd(szCmd);
}

VOID CZSApplication::UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect)
{
    CSDApplication::UpdateSvrStatus(dwID, dwIP, bConnect);
}


CZSClientMgr* CZSApplication::GetClientMgr()
{
    return &m_oZSClientMgr;
}

CRCClientMgr*	CZSApplication::GetRCClientMgr()
{
    return &m_oRCClientMgr;
}

CSDApplication* SDGetApp()
{
    static CZSApplication oGTApplication;
    return &oGTApplication;
}

BOOL CZSApplication::AddData( const string &strInfo)
{
    CSDMutexLock lock(ms_deqDataMutex);
    if( ms_deqData.size() > ms_nMaxDataNum )
    {
        return FALSE;
    }
    ms_deqData.push_back( strInfo );
    return TRUE;
}


VOID CZSApplication::SaveData()
{

    static char s_szSQL[MAX_SQL_LEN] = {0};
    memset(s_szSQL, 0x00, sizeof(s_szSQL));
    strcat(s_szSQL, "insert into loginlog (UserID, PlayerID, DeviceID, LoginIP, LoginTime, Type) values ");

    BOOL bHaveData = FALSE;
    ms_deqDataMutex.Lock();
    if( ms_deqData.size() )
    {
        bHaveData = TRUE;
        strcat(s_szSQL, ms_deqData[0].c_str());
    }

    for( UINT32 dwIndex = 1; dwIndex < ms_deqData.size(); ++dwIndex )
    {
        strcat(s_szSQL, ",");
        strcat(s_szSQL, ms_deqData[dwIndex].c_str());
    }
    ms_deqData.clear();
    ms_deqDataMutex.Unlock();


    if( !bHaveData )
    {
        return;
    }

    //UINT32 dwErr = 0;
    //string strError;
    //INT32 nRet;
    //ISDDBSession *m_pDBSession = GetDBMgr()->GetDBSession();
    //if (NULL == m_pDBSession)
    //{
    //    return;
    //}

    //nRet = m_pDBSession->ExecuteSql( s_szSQL);
    //if( nRet < 0 )
    //{
    //    SYS_CRITICAL( _SDT( "[%s %d]exec sql error,error:%s SQL:%s" ), MSG_MARK, strError.c_str(), s_szSQL );
    //}
}

