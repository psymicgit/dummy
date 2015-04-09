
#include "dbmgr.h"
#include <sdutil.h>
#include <framework/lsapplication.h>
#include <db/logincmd.h>
#include <db/loginexcmd.h>
#include <db/playerlevelupgragecmd.h>
#include <db/registerusernamecmd.h>
#include <db/playerdspnameupgragecmd.h>
#include <db/playbindcmd.h>
#include <db/playunbindcmd.h>
#include <db/logoutcmd.h>
#include <db/activitynotifycmd.h>
#include <db/changepwdcmd.h>
#include <db/syncdata.h>
#include <db/bindmobile.h>
#include <db/bindcheckvercode.h>
#include <db/unbindmobile.h>
#include <db/unbindcheckvercode.h>
#include <db/getuserexpand.h>


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
    if(!((CLSApplication*)SDGetApp())->GetDBConfig()->GetDBAccount(account))
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
                   ((CLSApplication*)SDGetApp())->GetDBConfig()->GetPoolCoreSize(),
                   ((CLSApplication*)SDGetApp())->GetDBConfig()->GetPoolMaxSize());
    if(NULL == m_pDBSession)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBSession failed"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}

VOID CDBMgr::Release()
{
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
    case EDBCT_LOGIN:
    {
        poBaseDBCMD = m_oLoginCMDPool.Alloc();
    }
    break;
    case EDBCT_LVLUP:
    {
        poBaseDBCMD = m_oPlayUpgrateCMDPool.Alloc();
    }
    break;
    case EDBCT_REGNAME:
    {
        poBaseDBCMD = m_oRegisterUserNameCMDPool.Alloc();
    }
    break;
    case EDBCT_DSPNAMEUP:
    {
        poBaseDBCMD = m_oPlayDspNameUpgradeCMDPool.Alloc();
    }
    break;
    case EDBCT_BIND:
    {
        poBaseDBCMD = m_oPlayBindCMDPool.Alloc();
    }
    break;
    case EDBCT_UNBIND:
    {
        poBaseDBCMD = m_oPlayUnBindCMDPool.Alloc();
    }
    break;
    case EDBCT_LOGIN_EX:
    {
        poBaseDBCMD = m_oLoginExCMDPool.Alloc();
    }
    break;
    case EDBCT_LOGOUT:
    {
        poBaseDBCMD = m_oLogoutCMDPool.Alloc();
    }
    break;
    case EDBCT_ACTIVITY_NOTIFY:
    {
        poBaseDBCMD = m_oActivityNotifyCMDPool.Alloc();
    }
    break;
    case EDBCT_CHANGE_PWD:
    {
        poBaseDBCMD = m_oChangePwdCMDPool.Alloc();
    }
    break;
    case EDBCT_SYNC_DATA:
    {
        poBaseDBCMD = m_oSyncDataCMDPool.Alloc();
    }
    break;
    case EDBCT_BINDMOBILE:
    {
        poBaseDBCMD = m_oBindMobileCMDPool.Alloc();
    }
    break;
    case EDBCT_BINDCHECKVERCODE:
    {
        poBaseDBCMD = m_oBindCheckVerCodeCMDPool.Alloc();
    }
    break;
    case EDBCT_UNBINDMOBILE:
    {
        poBaseDBCMD = m_oUnBindMobileCMDPool.Alloc();
    }
    break;
    case EDBCT_UNBINDCHECKVERCODE:
    {
        poBaseDBCMD = m_oUnBindCheckVerCodeCMDPool.Alloc();
    }
    break;
    case EDBCT_GETUSEREXPAND:
    {
        poBaseDBCMD = m_oGetUserExpandCMDPool.Alloc();
    }
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
    case EDBCT_LOGIN:
    {
        m_oLoginCMDPool.Free((CLoginCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_LVLUP:
    {
        m_oPlayUpgrateCMDPool.Free((CPlayUpgrateCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_REGNAME:
    {
        m_oRegisterUserNameCMDPool.Free((CRegisterUserNameCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_DSPNAMEUP:
    {
        m_oPlayDspNameUpgradeCMDPool.Free((CPlayDspNameUpgradeCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_BIND:
    {
        m_oPlayBindCMDPool.Free((CPlayBindCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_UNBIND:
    {
        m_oPlayUnBindCMDPool.Free((CPlayUnBindCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_LOGIN_EX:
    {
        m_oLoginExCMDPool.Free((CLoginExCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_LOGOUT:
    {
        m_oLogoutCMDPool.Free((CLogoutCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_ACTIVITY_NOTIFY:
    {
        m_oActivityNotifyCMDPool.Free((CActivityNotifyCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_CHANGE_PWD:
    {
        m_oChangePwdCMDPool.Free((CChangePwdCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_SYNC_DATA:
    {
        m_oSyncDataCMDPool.Free((CSyncDataCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_BINDMOBILE:
    {
        m_oBindMobileCMDPool.Free((CBindMobileCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_BINDCHECKVERCODE:
    {
        m_oBindCheckVerCodeCMDPool.Free((CBindCheckVerCodeCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_UNBINDMOBILE:
    {
        m_oUnBindMobileCMDPool.Free((CUnBindMobileCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_UNBINDCHECKVERCODE:
    {
        m_oUnBindCheckVerCodeCMDPool.Free((CUnBindCheckVerCodeCMD*)poBaseDBCMD);
    }
    break;
    case EDBCT_GETUSEREXPAND:
    {
        m_oGetUserExpandCMDPool.Free((CGetUserExpandCMD*)poBaseDBCMD);
    }
    break;
    default:
        break;
    }
}
