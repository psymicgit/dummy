/******************************************************************************


sdcondition.h - 条件变量,用作线程同步

******************************************************************************/


#ifndef SDCONDITION_H
#define SDCONDITION_H
/**
* @file sdcondition.h

* @brief 条件变量类
*
**/
//#ifndef WIN32
#if !(defined(WIN32) || defined(WIN64))
#  include <pthread.h>
#  include <errno.h>
#  include <sys/time.h>
#endif
#include "sdtype.h"

namespace  SGDP
{
    /**
    * @defgroup groupsync 线程同步
    * @ingroup  SGDP
    * @{
    */

// Attempt to resolve POSIX behaviour conformance for win32 build.
#define CONDITION_WIN32_CONFORMANCE_TO_POSIX

    class CSDMutex;

    /**
    *@brief 条件变量，用于线程同步
    */
    class CSDCondition
    {
    public:

	/**
	* @brief 构造函数，初始化条件
	*/
        CSDCondition();

	/**
	* @brief 析构函数，关闭条件句柄
	*/
        ~CSDCondition();


        /**
        * @brief
        * 等待条件被触发，这里有三种情况，一为条件改变，二为被中断，三为超时。
        * 当条件被触发后，原子的进入互斥量的互斥范围内。
        * @param mutex : 和条件变量相关的互斥量的引用
        * @param dwMs : 等待超时时间，单位为毫秒
        * @return TRUE 条件被触发; FALSE 超时或者被中断
        **/
        BOOL  Wait (CSDMutex &mutex, UINT32 dwMs = SDINFINITE);



        /**
        * @brief
        * 等待条件被触发，这里有三种情况，一为条件改变，二为被中断，三为超时.
        * 当条件被触发后，原子的进入互斥量的互斥范围内。
        * @param mutex : 和条件变量相关的互斥量的指针
        * @param dwMs : 等待超时时间，单位为毫秒
        * @return TRUE 条件被触发; FALSE 超时或者被中断
        **/
        BOOL  Wait (CSDMutex *mutex, UINT32 dwMs = SDINFINITE);

        /**
        * @brief
        * 触发所有等待中的随机一个等待的Wait。
        * @return VOID
        **/
        VOID  Signal();

        /**
        * @brief
        * 触发所有的等待的Wait。
        * @return VOID
        **/
        VOID  Broadcast();

    private:

        //  no value sematics, therefore private and not implemented.
	/**
	* @brief 拷贝构造函数
	* @param CSDCondition对象的引用
	*/
        CSDCondition (const CSDCondition&);

	/**
	* @brief 重载=操作符,用于对象的赋值
	* @param CSDCondition对象的引用
	* @return CSDCondition对象的引用
	*/
        CSDCondition& operator = (const CSDCondition&);

    private:
#if (defined(WIN32) || defined(WIN64))
#  ifdef CONDITION_WIN32_CONFORMANCE_TO_POSIX

        // boost clone with modification
        // licesnse text below

	/**
	* @brief 进入事件的信号状态等待中
	* @return VOID
	*/
        VOID EnterWait ();

        VOID* m_gate;
        VOID* m_queue;
        VOID* m_mutex;
        UINT32 m_gone;  // # threads that timed out and never made it to m_queue
        ULONG m_blocked; // # threads blocked on the condition
        UINT32 m_waiting; // # threads no longer waiting for the condition but
        // still waiting to be removed from m_queue
#  else
        HANDLE mId;
#  endif

#else
        mutable  pthread_cond_t mId;
#endif
    };

    /** @} */
}

#endif


