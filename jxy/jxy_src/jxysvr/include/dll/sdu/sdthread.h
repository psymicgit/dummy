/******************************************************************************



******************************************************************************/


#ifndef SD_THREAD_H
#define SD_THREAD_H

/**
* @file sdthread.h

* @brief 线程工具类
**/

#include "sdtype.h"

namespace SGDP
{
    /**
    * @defgroup groupthread 多进程多线程(包括线程池)
    * @ingroup  SGDP
    * @{
    */

#if (defined(WIN32) || defined(WIN64))
    typedef UINT32 SDTHREADID;
#else
    typedef pthread_t  SDTHREADID;
#endif // WIN32 

    /**
    * @brief
    * 得到线程id号
    * @return 线程id号
    */
    SDTHREADID  SDGetThreadId();

    typedef struct tagSThreadAttr
    {

    } SThreadAttr;

#if (defined(WIN32) || defined(WIN64))
    typedef unsigned int (WINAPI *PFThrdProc)(VOID *);
#define SDTHREAD_DECLARE(x)  unsigned int WINAPI  x
#define SDTHREAD_RETURN(x)   return x;
#define SDTHREAD_IMP		  unsigned int WINAPI
#else
    typedef VOID *  (*PFThrdProc)(VOID *);
#define SDTHREAD_DECLARE(x)  VOID *  x
#define SDTHREAD_RETURN(x)   return x;
#define SDTHREAD_IMP		  VOID *
#endif // 


    SDHANDLE  SDCreateThread(
        SThreadAttr * pAttr,
        PFThrdProc pThrdProc,
        VOID * pArg,
        SDTHREADID *pThreadId = NULL,
        BOOL bSuspend = FALSE,
		UINT32 dwStackSize = 0);

    INT32  SDThreadWait(SDHANDLE handle);

    VOID  SDThreadCloseHandle(SDHANDLE handle);

    VOID  SDThreadTerminate(SDHANDLE handle);

    VOID  SDThreadSuspend(SDHANDLE handle);

    VOID  SDThreadResume(SDHANDLE handle);

    VOID  SDSetThreadAttr(SDHANDLE,SThreadAttr * pAttr);

    SThreadAttr*  SDGetThreadAttr(SDHANDLE);

    /**
    * @brief 线程操作类
    */
    class CSDThread
    {

    public:
        CSDThread();
        virtual ~CSDThread();

        /**
        * @brief
        * 启动一个线程
        * @param pThredProc: 线程执行处理函数，注意：使用次接口后，不会调用类的成员函数ThrdProc
        * @param pArg : 线程执行参数
        * @param bSuspend : 设置线程状态为挂起
        * @return 成功返回TRUE，失败返回FALSE
        */
        BOOL  Start(PFThrdProc pfThrdProc, VOID *pArg, BOOL bSuspend = FALSE, UINT32 dwStackSize = 0);

        /**
        * @brief
        * 启动一个线程 注意：使用此接口启动线程会将类成员函数ThrdProc作为线程执行函数，用户需继承此
        * 类来执行线程
        * @param bSuspend : 设置线程状态为挂起
        * @return 成功返回TRUE，失败返回FALSE
        */
        BOOL  Start(BOOL bSuspend = FALSE, UINT32 dwStackSize = 0);


        /**
        * @brief
        * 当线程终止时的回调函数，可以继承此函数实现用户自己的功能
        * @return VOID
        */
        virtual VOID  OnTerminated()
        {
        }

        /**
        * @brief
        * 等待线程停止
        * @return VOID
        */
        VOID  Wait();

        /**
        * @brief
        * 强制线程终止
        * @return VOID
        */
        VOID  Terminate();

        /**
        * @brief
        * 挂起线程，此方法只在Windows操作系统中可用
        * @return VOID
        */
        VOID  Suspend();

        /**
        * @brief
        * 回复线程，此方法只在Windows操作系统中可用
        * @return VOID
        */
        VOID  Resume();

        /**
        * @brief
        * 设置线程属性
        * @param pAttr : 设置的属性结构体
        * @return VOID
        */
        VOID  SetAttribute(SThreadAttr *pAttr);

        /**
        * @brief
        * 获取线程属性
        * @return 返回获取的线程属性
        */
        SThreadAttr*  GetAttribute();

		inline BOOL IsStart() { return m_bstart; }
	protected:
        /**
        * @brief
        * 线程类的线程执行函数体. 用户需要重载此函数运行自己的线程函数
        * 如果用户调用Start(bSuspend)来启动线程，CSDThread类将会调用ThrdProc执行线程
        * @return VOID
        */
        virtual VOID  ThrdProc()
        {
        }
    protected:

		static SDTHREAD_DECLARE(SDThreadFunc) (VOID * arg); 
        CSDThread (const CSDThread&);
        CSDThread& operator = (const CSDThread&);

        BOOL m_bstart;
        SDTHREADID m_tid;
        SDHANDLE m_handle;
        VOID* m_arg;			/**<线程函数参数*/
    };


    /** @} */
}//

#endif



