
#include "dbmgr.h"
#include "basedbccmd.h"
#include "savefeedback.h"
#include "getplayernotifyinfo.h"
#include "twodaysnotonlinenoticeinfo.h"
#include <framework/nsapplication.h>

extern CSDApplication* SDGetApp();

CDBMgr::CDBMgr() :
    m_pDBModule(NULL),
    m_pDBSession(NULL)
{
}

CDBMgr::~CDBMgr()
{
}

BOOL CDBMgr::Init()
{
    SGDP::SDDBAccount account;
    if(!((CNSApplication*)SDGetApp())->GetNSConfig()->GetDBAccount(account))
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBAccount failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBModule = SGDP::SDDBGetModule(&(SGDP::SGDP_VERSION));
    if(NULL == m_pDBModule)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBSession = m_pDBModule->GetDBSession(&account,
                   ((CNSApplication*)SDGetApp())->GetNSConfig()->GetPoolCoreSize(),
                   ((CNSApplication*)SDGetApp())->GetNSConfig()->GetPoolMaxSize());
    if(NULL == m_pDBSession)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBSession failed"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}

VOID CDBMgr::Release()
{
    //确保执行完所有的db命令
    while((m_pDBSession->GetExcuteCmdCount() > 0) && (m_pDBSession->GetExcutedCmdCount() > 0) )
    {
        Run();
    }

    if(NULL != m_pDBModule)
    {
        if(NULL != m_pDBSession)
        {
            m_pDBModule->Close(m_pDBSession);
            m_pDBSession = NULL;
        }
        m_pDBModule->Release();
        m_pDBModule = NULL;
    }
}

SGDP::ISDDBSession* CDBMgr::GetDBSession()
{
    return m_pDBSession;
}

BOOL CDBMgr::Run()
{
    UINT32 dwCount = 100;
    BOOL bRet = m_pDBSession->Run(dwCount);
    return bRet;
}

BOOL CDBMgr::AddCommand(SGDP::ISDDBCommand* poDBCommand)
{
    return m_pDBSession->AddDBCommand(poDBCommand);
}


CBaseDBCMD*	CDBMgr::CreateCmd(EDBCmdType eDbCmdType)
{
    CBaseDBCMD* poBaseDBCMD = NULL;
    switch(eDbCmdType)
    {
    case EDBCT_SAVE_FEED_BACK:
        poBaseDBCMD = m_oSaveFeedBackCMDPool.Alloc();
        break;
    case EDBCT_GET_PLAYER_NOTIFY_INFO:
        poBaseDBCMD = m_oGetPlayerNotifyInfoCMDPool.Alloc();
        break;
    case EDBCT_TWO_DAYS_NOT_ONLINE_NOTICE_INFO:
        poBaseDBCMD = m_oTwoDaysNotOnlineNoticeInfoCMDPool.Alloc();
        break;
    default:
        break;
    }

    return poBaseDBCMD;
}

VOID CDBMgr::FreeCmd(CBaseDBCMD* poBaseDBCMD)
{
    if(NULL == poBaseDBCMD)
    {
        return;
    }
    switch(poBaseDBCMD->GetDBCmdType())
    {
    case EDBCT_SAVE_FEED_BACK:
        m_oSaveFeedBackCMDPool.Free((CSaveFeedBackCMD*)poBaseDBCMD);
        break;
    case EDBCT_GET_PLAYER_NOTIFY_INFO:
        m_oGetPlayerNotifyInfoCMDPool.Free((CGetPlayerNotifyInfoCMD*)poBaseDBCMD);
        break;
    case EDBCT_TWO_DAYS_NOT_ONLINE_NOTICE_INFO:
        m_oTwoDaysNotOnlineNoticeInfoCMDPool.Free((CTwoDaysNotOnlineNoticeInfoCMD*)poBaseDBCMD);
		break;
    default:
        break;
    }
}
