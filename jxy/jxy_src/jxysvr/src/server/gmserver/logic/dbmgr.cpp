
#include "dbmgr.h"
#include "basedbccmd.h"
#include "recordinfo.h"
#include <framework/gmapplication.h>


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
    if(!((CGMApplication*)SDGetApp())->GetGmConfig()->GetDBAccount(account))
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBAccount failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBModule = SGDP::SDDBGetModule();
    if(NULL == m_pDBModule)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBSession = m_pDBModule->GetDBSession(&account,
                   ((CGMApplication*)SDGetApp())->GetGmConfig()->GetPoolCoreSize(),
                   ((CGMApplication*)SDGetApp())->GetGmConfig()->GetPoolMaxSize());
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

BOOL CDBMgr::AddCommand(CBaseDBCMD* poDBCommand)
{
    return m_pDBSession->AddDBCommand(poDBCommand);
}


CBaseDBCMD*	CDBMgr::CreateCmd(EDBCmdType eDbCmdType)
{
    CBaseDBCMD* poBaseDBCMD = NULL;
    switch(eDbCmdType)
    {
    case EDBCT_RECORD_INFO:
        poBaseDBCMD = m_oRecordInfoCMDPool.Alloc();
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
    case EDBCT_RECORD_INFO:
        m_oRecordInfoCMDPool.Free((CRecordInfoCMD*)poBaseDBCMD);
        break;
    default:
        break;
    }
}