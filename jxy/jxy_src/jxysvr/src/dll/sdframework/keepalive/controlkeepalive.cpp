
#include "controlkeepalive.h"
#include "sdloggerimpl.h"
#include <sdprocess.h>

IMPLEMENT_SINGLETON(CControlKeepAlive)

CControlKeepAlive::CControlKeepAlive()
{
    m_poProcessDaemon = NULL;
}

CControlKeepAlive::~CControlKeepAlive()
{

}

BOOL CControlKeepAlive::Init()
{
    m_oShmem.handle = NULL;
    CHAR szFile[MAX_PATH] = {0};
    _SDTsnprintf(szFile, MAX_PATH, _SDT("%s%s"), _SDTT2A(SDGetModulePath()), "KeepAlive");

	//检测是否存在。
    if(SDShmemExist(szFile))
    {
		//如果存在，与之前的共享内存连接
        if(!SDAttachShm(&m_oShmem, szFile))
		{
			//不管是否存在都close
			SDCloseShmemByPath(szFile, sizeof(SProcessDaemon));
		}
    }

    if(NULL == m_oShmem.handle)
    {
        m_oShmem = SDOpenShmem(szFile, TRUE, sizeof(SProcessDaemon));
    }
    if(NULL == m_oShmem.handle || NULL == m_oShmem.mem)
    {
		printf("NULL == m_oShmem.handle || NULL == m_oShmem.mem\n");
        return FALSE;
    }

    m_poProcessDaemon = (SProcessDaemon*)m_oShmem.mem;
    if (NULL == m_poProcessDaemon)
    {
        return FALSE;
    }
	m_poProcessDaemon->useFlag = 1;
	m_poProcessDaemon->alive = 1;
	m_poProcessDaemon->starttimer = SDTimeSecs();
	m_poProcessDaemon->lasttimer = SDTimeSecs();
	m_poProcessDaemon->pid = SDGetCurrentProcessId();


    return TRUE;
}

VOID CControlKeepAlive::UnInit()
{
	m_poProcessDaemon = NULL;
	if (NULL == m_oShmem.mem)
	{
		SDCloseShmem(&m_oShmem);
	}
}

VOID CControlKeepAlive::OnAlive()
{
    if (NULL != m_poProcessDaemon)
    {
        m_poProcessDaemon->alive = 1;
        m_poProcessDaemon->lasttimer = SDTimeSecs();
    }
}

BOOL CControlKeepAlive::IsStop()
{
    if (NULL != m_poProcessDaemon && 0 == m_poProcessDaemon->useFlag)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


BOOL CControlKeepAlive::SyncStop()
{
	if (NULL != m_poProcessDaemon)
	{
		m_poProcessDaemon->alive = 0;
		m_poProcessDaemon->useFlag = 0;
		return TRUE;
	}

	return FALSE;
}

