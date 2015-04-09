#include "sdthread.h"
#if (defined(WIN32) || defined(WIN64))
#include <process.h>
#else
#include <pthread.h>
#endif

namespace SGDP
{

    SDTHREAD_DECLARE(CSDThread::SDThreadFunc) (void * arg)
    {
        CSDThread *pThread = static_cast<CSDThread*>(arg);
		if (pThread != NULL)
		{
			pThread->ThrdProc();
		}
        return 0;
    }


    SDTHREADID  SDGetThreadId()
    {
#if (defined(WIN32) || defined(WIN64))
        return ::GetCurrentThreadId();
#else
        return pthread_self();
#endif  //WIN32
    }

    CSDThread::CSDThread()
        :m_bstart(FALSE)
    {
        m_handle = SDINVALID_HANDLE;
        m_tid = 0;
    }

    CSDThread::~CSDThread()
    {
        if (m_handle != SDINVALID_HANDLE)
        {
            //SDThreadCloseHandle(m_handle);
            m_handle = SDINVALID_HANDLE;
        }
    }

    BOOL  CSDThread::Start(PFThrdProc pthrdProc, void * pArg, BOOL bSuspend /*= false*/, UINT32 dwStackSize)
    {
		if (!m_bstart)
		{
			m_handle = SDCreateThread(NULL, pthrdProc, pArg, &m_tid, bSuspend, dwStackSize);
			if (SDINVALID_HANDLE == m_handle)
			{
				m_bstart = FALSE;
				return FALSE;
			}
			m_bstart = TRUE;
		}
        return TRUE;
    }

    BOOL  CSDThread::Start(BOOL bSuspend, UINT32 dwStackSize)
    {
        if (!m_bstart)
        {
            m_handle = SDCreateThread(NULL, SDThreadFunc, this, &m_tid, bSuspend, dwStackSize);
            if (SDINVALID_HANDLE == m_handle)
            {
                return FALSE;
            }
            m_bstart = TRUE;
        }
        return m_bstart;
    }

    void  CSDThread::Wait()
    {
        if(m_bstart)
        {
            m_bstart = FALSE;
            SDThreadWait(m_handle);
        }
    }


    void  CSDThread::Terminate()
    {
        if(m_bstart)
        {
            SDThreadTerminate(m_handle);
            this->OnTerminated();
            m_bstart = FALSE;
        }
    }

    void  CSDThread::Suspend()
    {
        SDThreadSuspend(m_handle);
    }

    void  CSDThread::Resume()
    {
        SDThreadResume(m_handle);
    }

    SDHANDLE  SDCreateThread(SThreadAttr * pAttr, PFThrdProc pThrdProc,
                                  void * pParam, SDTHREADID * pThreadId/* = NULL*/, BOOL bSuspend /*= false*/, UINT32 dwStackSize)
    {
#if (defined(WIN32) || defined(WIN64))
        int thrdFlag = bSuspend ? CREATE_SUSPENDED : 0;
        // CreateThread(NULL, 0, pThrdProc, pParam, thrdFlag, pThreadId);
        return (SDHANDLE)_beginthreadex(NULL, dwStackSize, pThrdProc, pParam, thrdFlag, pThreadId);
#else
        if (0 != pthread_create((pthread_t*)pThreadId, NULL, pThrdProc, pParam))
        {
            return SDINVALID_HANDLE;
        }
        return *pThreadId;
#endif // 
    }

    void  CSDThread::SetAttribute(SThreadAttr * pAttr)
    {
        SDSetThreadAttr(m_handle, pAttr);
    }

    SThreadAttr *   CSDThread::GetAttribute()
    {
        return SDGetThreadAttr(m_handle);
    }
    void  SDThreadCloseHandle(SDHANDLE handle)
    {
        SDCloseHandle(handle);
    }

    INT32   SDThreadWait(SDHANDLE handle)
    {
#if (defined(WIN32) || defined(WIN64))
        return ::WaitForSingleObject(handle, INFINITE);
#else
        return pthread_join(handle,NULL);
#endif //
    }

    void  SDThreadTerminate(SDHANDLE handle)
    {
#if (defined(WIN32) || defined(WIN64))
        TerminateThread(handle, 0);
        CloseHandle(handle);
#else
        pthread_cancel(handle);
#endif
    }

    void  SDThreadSuspend(SDHANDLE handle)
    {
#if (defined(WIN32) || defined(WIN64))
        SuspendThread(handle);
#endif // 
    }

    void  SDThreadResume(SDHANDLE handle)
    {
#if (defined(WIN32) || defined(WIN64))
        ResumeThread(handle);
#endif // 
    }


    void  SDSetThreadAttr(SDHANDLE,SThreadAttr * pAttr)
    {

    }

    SThreadAttr*  SDGetThreadAttr(SDHANDLE)
    {
        return NULL;
    }



}  //namespace SGDP

