#include "smsmgr.h"
#include <sstream>
#include <sdlock.h>
#include <sdloggerimpl.h>
#include <sddb.h>
#include <sdutil.h>
#include <sdtime.h>

IMPLEMENT_SINGLETON(CSmsMgr);


CSmsMgr::CSmsMgr()
{
    m_bStop = FALSE;
    m_tLastWriteTime = SDTimeSecs();
}

CSmsMgr::~CSmsMgr()
{

}

BOOL CSmsMgr::Init()
{

    m_bStop = FALSE;
    CURL *curl = NULL;
    CURLcode code;
    code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (code != CURLE_OK)
    {
        SYS_CRITICAL( _SDT( "[%s: %d]:Failed to global init default [%d]!" ), MSG_MARK, code );
        return FALSE;
    }
    return Start();
}


VOID CSmsMgr::UnInit()
{
    Stop();
    this->Wait();
    curl_global_cleanup();
}

VOID CSmsMgr::Stop()
{
    m_bStop = TRUE;
}

VOID  CSmsMgr::ThrdProc()
{
    CBaseSmsCMD *pBaseSmsCMD = NULL;
    while(!m_bStop)
    {
        pBaseSmsCMD = NULL;
        m_ListingMutex.Lock();
        if(!m_oBaseSmsCMDListing.empty())
        {
            pBaseSmsCMD = m_oBaseSmsCMDListing.front();
            m_oBaseSmsCMDListing.pop_front();
        }
        m_ListingMutex.Unlock();
        if (NULL != pBaseSmsCMD)
        {
            pBaseSmsCMD->OnExecute();
            //把处理的数据提交到已完成队列中
            m_ListedMutex.Lock();
            m_oBaseSmsCMDListed.push_back(pBaseSmsCMD);
            m_ListedMutex.Unlock();
        }
        else
        {
            SGDP::SDSleep( ms_dwSleepTime );
        }
    }
}

BOOL CSmsMgr::Run()
{
    CBaseSmsCMDListItr itr;
    m_ListedMutex.Lock();
    for (itr = m_oBaseSmsCMDListed.begin(); itr != m_oBaseSmsCMDListed.end(); itr++)
    {
        (*itr)->OnExecuted();
        (*itr)->Release();
    }
    m_oBaseSmsCMDListed.clear();
    m_ListedMutex.Unlock();
    return TRUE;
}

CBaseSmsCMD*	CSmsMgr::CreateCmd(enum ESMSCmdType eSmsCmdType)
{
    CBaseSmsCMD* poBaseDBCMD = NULL;
    switch(eSmsCmdType)
    {
    case ESMS_BINDMOBILE:
        poBaseDBCMD = m_oBindSmsCMDPool.Alloc();
        break;
	case ESMS_UNBINDMOBILE:
		poBaseDBCMD = m_oUnBindSmsCMDPool.Alloc();
		break;
    }
    return poBaseDBCMD;
}

VOID	 CSmsMgr::FreeCmd(CBaseSmsCMD* poBaseDBCMD)
{
    switch(poBaseDBCMD->GetSMSCmdType())
    {
    case ESMS_BINDMOBILE:
        m_oBindSmsCMDPool.Free((CBindSmsCMD*)poBaseDBCMD);
        break;
	case ESMS_UNBINDMOBILE:
		m_oUnBindSmsCMDPool.Free((CUnBindSmsCMD*)poBaseDBCMD);
		break;
    }
}


BOOL	CSmsMgr::AddCommand(CBaseSmsCMD* poDBCommand)
{
    m_ListingMutex.Lock();
    m_oBaseSmsCMDListing.push_back(poDBCommand);
    m_ListingMutex.Unlock();
    return TRUE;
}

