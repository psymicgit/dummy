

#include "stdafx.h"
#include "dbmgr.h"
#include <sdstring.h>


CDBMgr::CDBMgr() :
    m_pDBModule(NULL)
{
}

CDBMgr::~CDBMgr()
{

}

BOOL CDBMgr::Init()
{
    m_pDBModule = SGDP::SDDBGetModule(&(SGDP::SGDP_VERSION));
    if(NULL == m_pDBModule)
    {
        return FALSE;
    }
    return TRUE;
}

VOID CDBMgr::UnInit()
{
    if(NULL != m_pDBModule)
    {
        for(CDBSessionMapItr itr = m_mapDBSession.begin(); itr != m_mapDBSession.end(); itr++)
        {
            m_pDBModule->Close(itr->second);
        }
        m_mapDBSession.clear();
        m_pDBModule->Release();
        m_pDBModule = NULL;
    }
}

ISDDBSession* CDBMgr::GetDBSession(string strKey, SDDBAccount stAccount)
{
    CDBSessionMapItr itr = m_mapDBSession.find(strKey);
    if (itr != m_mapDBSession.end())
    {
        return itr->second;
    }
    ISDDBSession* m_pDBSession = m_pDBModule->GetDBSession(&stAccount, 1, 1);
    if(NULL == m_pDBSession)
    {
        return NULL;
    }
    m_mapDBSession[strKey] = m_pDBSession;
    return m_pDBSession;
}

BOOL CDBMgr::CloseDBSession(string strKey)
{
	CDBSessionMapItr itr = m_mapDBSession.find(strKey);
	if (itr == m_mapDBSession.end())
	{
		return TRUE;
	}
	m_pDBModule->Close(itr->second);
	m_mapDBSession.erase(itr);
	return TRUE;
}



BOOL CDBMgr::Run()
{
    UINT32 dwCount = 500;
    for(CDBSessionMapItr itr = m_mapDBSession.begin(); itr != m_mapDBSession.end(); itr++)
    {
        itr->second->Run(dwCount);
    }
    return TRUE;
}

BOOL CDBMgr::AddCommand(string strKey, SGDP::ISDDBCommand* poDBCommand)
{
    AddDBCommandCount();
    CDBSessionMapItr itr = m_mapDBSession.find(strKey);
    if (itr != m_mapDBSession.end())
    {
        return itr->second->AddDBCommand(poDBCommand);;
    }
    return TRUE;
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
