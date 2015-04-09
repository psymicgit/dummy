#ifndef SD_TIMER_DETAIL_H
#define SD_TIMER_DETAIL_H
#include "sdtimer.h"
#include <list>
#include <sstream>
namespace SGDP
{

// 时间序列每层有几格
    static const UINT32 TIMER_ARRAY_LEN = 1000;

// 时间序列层数,时间粒度最细层为0层
    static const UINT32 TIMERARRAY_COUNT = 5;

    class CSDTimerImpl;
    class CTimerItem
    {
    public:
        UINT8       m_timerLevel;          // 时钟当前位于哪一层时间序列中

        /**
        * @brief
        * 初始化单个定时器
        * @param poTimerModule : 输入的定时器组件
        * @param poTimerHandler : 输入的定时器处理函数接口
        * @param dwElapse : 定时器的间隔
        * @param dwTimerId : 定时器id
        * @return void
        */
        VOID Init( CSDTimerImpl* poTimerCtrl,
                   ISDTimer* poTimerHandler,
                   UINT32 dwElapse,
                   UINT32 dwTimerID,
                   UINT32 loopCount );


        UINT8 SetupForNextTrigger(UINT32 delay);


        inline VOID SetLastExpdTime(UINT32 dwExpTime)
        {
            m_dwLastExpTime = dwExpTime;
        }

        inline UINT32 GetLastExpdTime()
        {
            return 	m_dwLastExpTime;
        }

        /**
        * @brief
        * 获取下一个触发层级
        * @param level : 定时器层级
        * @return 下一个触发层级
        */
        UINT32 GetSlotIndexForNextTrigger(UINT8 level);

        /**
        * @brief
        * 定时器触发处理
        * @return void
        */
        VOID OnTimer();


        inline BOOL CheckLoop()
        {
            if (m_dwLoopCount == 0xFFFFFFFF)
            {
                return TRUE;
            }
            return m_dwLoopCount --;
        }

    private:
        UINT32      m_dwTimerID;		// 定时器编号
        UINT32      m_dwElapse;			// 定时器触发间隔
        ISDTimer*   m_poTimerHandler;   // 用户设置的定时器超时处理函数
        UINT32      m_adwSlotIndexes[TIMERARRAY_COUNT]; // Timer的下次超时时间
        UINT32		m_dwLastExpTime; //上次超时时间
        UINT32      m_dwLoopCount;
    };


    /**
    *@brief 对应在某一时刻超时的所有定时器链表
    */
    typedef std::list<CTimerItem*> TimerSlot;

    /**
    *@brief 定时器层级管理类
    */
    class CTimerSlotLevel
    {
    public:
        CTimerSlotLevel();

        BOOL NextSlot(BOOL bRewindOnEnd);

        void ReloadFromLevel( CTimerSlotLevel& oSlotLevel );

        void Init(UINT8 level)
        {
            m_timerLevel = level;
        }

        inline UINT32 GetCurSlotIndex()
        {
            return m_dwCurSlot;
        }

        TimerSlot& GetCurSlot()
        {
            return m_aoTimerSlots[m_dwCurSlot];
        }

        inline void AddTimer( CTimerItem* poTimer )
        {
            poTimer->m_timerLevel = m_timerLevel;
            m_aoTimerSlots[poTimer->GetSlotIndexForNextTrigger(m_timerLevel)]
            .push_back(poTimer);
        }

        inline void DelTimer( CTimerItem* poTimer )
        {
            if(poTimer->m_timerLevel != m_timerLevel)
                return;
            m_aoTimerSlots[poTimer->GetSlotIndexForNextTrigger(m_timerLevel)]
            .remove(poTimer);
        }

		std::string DumpTimerInfo()
        {
			std::stringstream info; 
            for (UINT32 slot=0; slot<TIMER_ARRAY_LEN; slot++)
            {
                if (!m_aoTimerSlots[slot].empty())
                {
                    // 显示时钟所在的位置
					info << "Timer in ArrayLevel " << m_timerLevel << ", Slot " << slot << std::endl; 
                }
            }
			return info.str(); 
        }

    private:
        UINT8   m_timerLevel;
        UINT32  m_dwCurSlot;
        TimerSlot   m_aoTimerSlots[TIMER_ARRAY_LEN];

    };

}

#endif


