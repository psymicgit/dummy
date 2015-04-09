/******************************************************************************
			

	sdtimer.h - 定时器组件
******************************************************************************/


#ifndef _SGDP_TIMER_H_
#define _SGDP_TIMER_H_
/**
* @file sdtimer.h

* @brief 定时器系列
*
**/
#include "sdtype.h"
#include <string>
using namespace std;
namespace SGDP
{
    /**
    * @defgroup grouptimer 定时器组件
    * @ingroup  SGDP
    * @{
    */
    class CSDTimerImpl;

    /**
    * @brief The ISDTimer class is an application-defined timer completion
    *       routine. Specify this address when calling the SetTimer function.
    *       then it will be called when the timer was timeout.
    *       Especially this OnTimer function can't be blocked.
    */
    class ISDTimer
    {
    public:

        /**
        * @brief
        * 超时函数。当超时时，调用此定时器。
        * @param dwTimerID : 超时时的定时器IDTimer
        * @return VOID
        */
        virtual VOID  OnTimer(UINT32 dwTimerID) = 0;
    };

    /**
    * @brief 定时器模块类，采用类似LINUX的分层的定时器管理方法，通过在线程当中循环调用Run函数来驱动定时器
    */
    class CSDTimer
    {

    public:

		/**
		* @brief 构造函数，初始化CSDTimeImpl对象
		*/
        CSDTimer();

		/**
		* @brief 析构函数，释放CSDTimeImpl对象
		*/
        ~CSDTimer();

        /**
        * @brief
        * 创建一个时钟并设置time-out值
        * @param pTimer : 用户传入时钟回调接口类
        * @param dwTimerID : 设置时钟ID
        * @param dwElapse : 设置time-out值，以毫秒为单位
        * @param dwLoop : 每个加入的定时器都会执行一次，dwLoop表示定时器超时后继续超时的次数，如果为0xFFFFFFFF，表示永久循环定时器
        * @return 返回一个bool值,true表示成功,false表示失败
        */
        BOOL  SetTimer(ISDTimer *pTimer, UINT32 dwTimerID, UINT32 dwElapse, UINT32 dwLoop = 0xFFFFFFFF);

        /**
        * @brief
        * 取消时钟
        * @param dwTimerID : UINT32 时钟ID，表示要取消的时钟
        * @return 返回一个bool值,true表示成功,false表示失败
        */
        BOOL  KillTimer(UINT32 dwTimerID);

        /**
        * @brief
        * 时钟消息处理
        * @return 如果还有事件没处理完返回true，如果所有事件都处理完了，返回false
        */
        BOOL  Run();

        /**
        * @brief
        * 打印定时器信息
        * @return VOID
        */
		std::string  DumpTimerInfo();

    protected:

		/**
		* @brief 拷贝构造函数
		* @param CSDTimer对象的引用
		*/
		CSDTimer(const CSDTimer & ){};

		/**
		* @brief 重载=操作符，实现CSDTimer的赋值
		* @param CSDTimer对象的引用
		* @return CSDTimer对象的引用
		*/
        CSDTimer& operator = (const CSDTimer & );
    private:
        CSDTimerImpl *  m_pTimerImpl;
    };

    /** @} */
}

#endif

