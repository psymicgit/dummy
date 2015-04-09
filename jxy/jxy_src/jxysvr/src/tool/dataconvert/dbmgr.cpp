#include "dbmgr.h"
#include <sdstring.h>


CDBMgr::CDBMgr():
	m_pDBModule(NULL),
	m_pDBSession(NULL)
{
}

CDBMgr::~CDBMgr()
{
}

BOOL CDBMgr::Init(SDDBAccount stAccount)
{
	m_pDBModule = SGDP::SDDBGetModule(&(SGDP::SGDP_VERSION));
	if(NULL == m_pDBModule)
	{
		return FALSE;
	}
	m_pDBSession = m_pDBModule->GetDBSession(&stAccount, 1, 1);
	if(NULL == m_pDBSession)
	{
		return FALSE;
	}
	return TRUE;
}

VOID CDBMgr::UnInit()
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

ISDDBSession* CDBMgr::GetDBSession()
{
	return m_pDBSession;
}



BOOL CDBMgr::Run()
{
	UINT32 dwCount = 500;
	return m_pDBSession->Run(dwCount);
}

BOOL CDBMgr::AddCommand(SGDP::ISDDBCommand* poDBCommand)
{
	AddDBCommandCount();
	return m_pDBSession->AddDBCommand(poDBCommand);
}

VOID CDBMgr::AddDBCommandCount()
{
	m_dwCommandNum = m_dwCommandNum + 1;
}

VOID CDBMgr::DecreaseDBCommandCount()
{
	m_dwCommandNum = m_dwCommandNum - 1;
}

UINT32 CDBMgr::GetDBCommandCount()
{
	return m_dwCommandNum;
}
