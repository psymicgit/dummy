/******************************************************************************
		

	sdthreadpool.h - 线程池操作
******************************************************************************/


#ifndef SDTHREADPOOL_H
#define SDTHREADPOOL_H
/**
* @file sdthreadpool.h

* @brief 线程池的实现
**/
#include <deque>
#include <vector>
#include <map>
#include "sdthread.h"
#include "sdmutex.h"
#include "sdcondition.h"

namespace SGDP
{
    /**
    * @defgroup groupthread 多进程多线程(包括线程池)
    * @ingroup  SGDP
    * @{
    */

    /**
    *@brief 线程池任务接口，没一个任务都必须继承并实现该接口
    */
    class ISDRunable
    {
    public:
		virtual ~ISDRunable(){}
        /**
        * @brief
        * 任务接口，每一个任务都要实现该接口
        * @return VOID
        **/
        virtual VOID Run() = 0;
    };

    /**
    *@brief 线程池操作类
    */
    class CSDThreadPool
    {
        typedef std::deque<ISDRunable*> JobContainer;

        typedef std::vector<CSDThread*> ThreadContainer;

        typedef struct _tagThreadArg
        {
            CSDThreadPool* pThreadPool;
            CSDThread* pThread;
            volatile BOOL keepWorking;
        } ThreadArg;

        typedef std::vector<ThreadArg*> ThreadArgContainer;

        typedef std::map<ThreadArg*, ThreadArg*> ThreadArgMap;

        typedef std::pair<ThreadArg*, ThreadArg*> ThreadArgPair;

    public:
        CSDThreadPool();
        ~CSDThreadPool();

        /**
        * @brief
        * 初始化线程池
        * @param dwMinThrds : 线程池最小线程数
        * @param dwMaxThrds : 线程池最大线程数
        * @param dwMaxPendJobs : 允许的为执行最大任务数
        * @return  创建成功返回true，失败返回false
        **/
        BOOL Init(UINT32 dwMinThrds, UINT32 dwMaxThrds, UINT32 dwMaxPendJobs);

        /**
        * @brief
        * 添加任务到线程池
        * @param pJob : 工作任务
        * @return  成功返回true，失败返回false
        **/
        BOOL ScheduleJob(ISDRunable *pJob);

        /**
        * @brief
        * 结束线程池所有执行线程，不等待未执行的任务执行完成，用于销毁的时候
        * @return VOID
        **/
        VOID TerminateQuick();

        /**
        * @brief
        * 结束线程池所有执行线程，等待现有任务执行完成再结束线程，用于销毁的时候
        * @return VOID
        **/
        VOID TerminateWaitJobs();

        /**
        * @brief
        * 得到线程池中工作线程的数目
        * @return 线程池中工作线程的数目
        **/
        UINT32 GetThreadNum();

        /**
        * @brief
        * 得到线程池中等待执行的任务数目
        * @return 线程池中等待执行的任务数目
        **/
        UINT32 GetJobPending();


    private:
        static SDTHREAD_DECLARE( WorkThreadFunc)(VOID *pArg);

        CSDThreadPool(const CSDThreadPool &other);              // no implementation
        VOID operator = (const CSDThreadPool &other);       // no implementation

    private:
        static const UINT32 WAITTIME = 32;
        static const UINT32 WAITCOUNT = 16384;

        JobContainer	   m_jobContainer;
        ThreadArgContainer m_threadArgContainer;

        ThreadArgMap  m_threadArgMap;
        CSDMutex	  m_jobMutex;
        CSDMutex	  m_threadMutex;
        CSDCondition  m_jobCondition;

        UINT32 m_minThreads;
        UINT32 m_maxThreads;
        UINT32 m_maxPendingJobs;

        volatile BOOL m_waitTerminate;
        volatile BOOL m_quickTerminate;
    };


    /** @} */
}//

#endif



