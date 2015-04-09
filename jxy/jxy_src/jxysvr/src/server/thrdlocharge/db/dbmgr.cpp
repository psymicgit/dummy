#include "dbmgr.h"
#include <sdutil.h>
#include "sdxmlfile.h"
#include "framework/tlcconfig.h"



#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define ATTRI_UNIXSOCKET		"UnixSocket"


IMPLEMENT_SINGLETON(CDBMgr);

CDBMgr::CDBMgr() :
    m_pDBModule(NULL),
    m_pDBSession(NULL)
{
}

CDBMgr::~CDBMgr()
{
}

BOOL CDBMgr::Init(const CHAR* pszConfig)
{	
	const SDDBAccount& stDBAccount = CTLCConfig::Instance()->GetDBAccount(); 
	UINT32 dwPoolCoreSize = CTLCConfig::Instance()->GetPoolCoreSize();
	UINT32 dwPoolMaxSize = CTLCConfig::Instance()->GetPoolMaxSize();

    m_pDBModule = SGDP::SDDBGetModule(&(SGDP::SGDP_VERSION));
    if(NULL == m_pDBModule)
    {
        printf(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBSession = m_pDBModule->GetDBSession((SDDBAccount*)&stDBAccount,
                   dwPoolCoreSize,
                   dwPoolMaxSize);
    if(NULL == m_pDBSession)
    {
        printf(_SDT("[%s: %d]:GetDBSession failed"), MSG_MARK);
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

BOOL CDBMgr::AddAddOrderIDCmd(SAddOrderInfo& stAddOrderInfo, UINT16 wTransID)
{
	CAddOrderIDCMD* poCMD = m_oAddOrderIDCMDPool.Alloc();
	poCMD->SetProp(stAddOrderInfo, wTransID);
	return m_pDBSession->AddDBCommand(poCMD);
}

VOID CDBMgr::FreeAddOrderIDCmd(CAddOrderIDCMD* poCMD)
{
	m_oAddOrderIDCMDPool.Free(poCMD);
	poCMD = NULL;
}


