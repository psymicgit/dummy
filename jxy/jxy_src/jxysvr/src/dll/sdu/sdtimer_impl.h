#ifndef SDTIMER_IMPL_H
#define SDTIMER_IMPL_H
#include "sdhashmap.h"
#include "sdobjectpool.h"
#include "sdtimer_detail.h"

namespace SGDP
{
    /**
    *@brief 定时器模块类，采用类似LINUX的分层的定时器管理方法，通过在线程当中循环调用Run函数来驱动定时器
    */

    class CSDTimerImpl
    {
        typedef HashMap<UINT32, CTimerItem*> CTimerMap;

    public:
        CSDTimerImpl();
        ~CSDTimerImpl();

        /**
        * @brief
        * 创建一个时钟并设置time-out值
        * @param poTimerHandler : 用户传入时钟回调接口类
        * @param dwTimerId : 设置时钟ID
        * @param dwElapse : 设置time-out值，以毫秒为单位
        * @param dwLoop : 每个加入的定时器都会执行一次，dwLoop表示定时器超时后继续超时的次数，如果为0xFFFFFFFF，表示永久循环定时器
        * @return 返回一个bool值,true表示成功,false表示失败
        */
        BOOL  SetTimer(ISDTimer* poTimerHandler, UINT32 dwTimerId, UINT32 dwElapse, UINT32 dwLoop= SDINFINITE);

        /**
        * @brief
        * 取消时钟
        * @param dwTimerId : UINT32 时钟ID，表示要取消的时钟
        * @return 返回一个bool值,true表示成功,false表示失败
        */
        BOOL  KillTimer(UINT32 dwTimerId);

        /**
        * @brief
        * 时钟消息处理
        * @param nCount : 表示处理验证事件的数量上限，-1表示处理所有的事件
        * @return 如果还有事件没处理完返回true，如果所有事件都处理完了，返回false
        */
        BOOL  Run();

        /**
        * @brief
        * 打印时钟信息
        * @return void
        */
		std::string  DumpTimerInfo();

    private:

        friend class CTimerItem;
        /**
        * @brief
        * 得到某个时钟轮结构
        * @param nLevel : 时钟轮的级别
        * @return void
        */
        inline CTimerSlotLevel& GetSlotLevel( UINT32 nLevel )
        {
            return m_aoSlotLevels[nLevel];
        }

        /**
        * @brief
        * 用下一个循环将本层时钟填充
        * @param dwLevel : 填充的目标层
        * @return 成功返回true，失败返回false
        */
        BOOL ReloadSlotLevel(UINT8 dwLevel = 0);

        /**
        * @brief
        * 处理定时器超时
        */
        UINT32 TrigerTimersForCurSlot();

        /**
        * @brief
        * 根据当前时间和超时时间，将定时器放置到合适的位置
        * @param poTimer : 指向timer结构的指针
        * @return 成功返回true，失败返回false
        */
        BOOL AddTimerForNextTrigger(CTimerItem *pTimer);

        UINT32 GetMsFromLast();
        UINT32 GetMsFromLast(UINT32 dwLastTime);

        CTimerItem* LookupTimer( UINT32 dwTimerId );

        CSDObjectPool<CTimerItem, CSDMutex > m_oTimerPool;	/**<定时器的缓冲池*/
        CTimerSlotLevel m_aoSlotLevels[TIMERARRAY_COUNT]; /**<存放TimerSlot的数组*/
        CTimerMap   m_oTimerMap;						/**<存放定时器ID和定时器缓冲池指针的hashmap*/
        UINT32      m_dwLastTickCount;					/**<上一次调用Run时的tick*/
        UINT32      m_dwMsPerTick;						/**<系统当中每秒的CPU TICK数*/
    };
}

#endif // 




