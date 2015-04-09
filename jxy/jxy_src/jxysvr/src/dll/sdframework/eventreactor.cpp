#include "eventreactor.h"
#include "sdtime.h"

namespace SGDP { 
    CEvent::CEvent()
    {
	    m_eEventStatus = EVENT_READY;
	    m_qwNextTick = 0;
    }

    CEvent::~CEvent()
    {
	    m_eEventStatus = EVENT_READY;
	    m_qwNextTick = 0;
    }

    CEventMeter::CEventMeter()
    {

    }

    CEventMeter::~CEventMeter()
    {

    }


    CCommonEventMeter::CCommonEventMeter()
    {
        m_qwLastTickCount = 0;
        m_dwLastEventCount = 0;
        m_qwTotalTickCount = 0;
        m_dwTotalEventCount = 0;
        m_dwTotalLoopCount = 0;
        m_qwRecentTickCount = 0;
        m_dwRecentEventCount = 0;
        m_dwRecentLoopCount = 0;

        memset(m_aoElementMeterInfo, 0, sizeof(m_aoElementMeterInfo));
	    m_bStatus = FALSE;
    }

    CCommonEventMeter::~CCommonEventMeter()
    {
        m_qwLastTickCount = 0;
        m_dwLastEventCount = 0;
        m_qwTotalTickCount = 0;
        m_dwTotalEventCount = 0;
        m_dwTotalLoopCount = 0;
        m_qwRecentTickCount = 0;
        m_dwRecentEventCount = 0;
        m_dwRecentLoopCount = 0;
        memset(m_aoElementMeterInfo, 0, sizeof(m_aoElementMeterInfo));

	    m_bStatus = FALSE;
    }

    /**
     * @brief 开始记录泵的周期循环
     * @param dwCurTick : 当前系统Tick
     */
    VOID CCommonEventMeter::BeginLoop(UINT64 qwCurTick)
    {
	    if(FALSE == m_bStatus)
	    {
		    return;
	    }
	    m_qwBeginTick = qwCurTick;
        memset(m_aoElementMeterInfo, 0, sizeof(m_aoElementMeterInfo));
    }

    /**
     * @brief 结束记录泵的周期循环
     * @param dwEventCount : 泵执行周期中执行了多少有效元素周期
     * @param dwCurTick : 当前系统Tick
     */
    VOID CCommonEventMeter::EndLoop(UINT32 dwEventCount, UINT64 qwCurTick)
    {
	    if(FALSE == m_bStatus)
	    {
		    return;
	    }
	    m_qwLastTickCount = qwCurTick - m_qwBeginTick;
	    m_dwLastEventCount = dwEventCount;
	    m_qwTotalTickCount += m_qwLastTickCount;
	    m_dwTotalEventCount += m_dwLastEventCount;
	    m_dwTotalLoopCount++;
	    m_qwRecentTickCount += m_qwLastTickCount;
	    m_dwRecentEventCount += m_dwLastEventCount;
	    m_dwRecentLoopCount++;
    }

    /**
     * @brief 对单个元素周期进行抽样
     * @param dwElementType : 元素类型，由用户返回
     * @param qwTickCount : 本地元素周期持续时间
     */
    INT32 CCommonEventMeter::AddSamplePoint(UINT32 dwElementType, UINT64 qwTickCount)
    {
	    if(FALSE == m_bStatus)
	    {
		    return -1;
	    }
        (m_aoElementMeterInfo[dwElementType].dwEventCount)++;
        m_aoElementMeterInfo[dwElementType].qwEventTick += qwTickCount;
        if(m_aoElementMeterInfo[dwElementType].qwEventMaxTick < qwTickCount)
        {
            m_aoElementMeterInfo[dwElementType].qwEventMaxTick = qwTickCount;
        }
        if(m_aoElementMeterInfo[dwElementType].qwHistoryMaxTick < m_aoElementMeterInfo[dwElementType].qwEventMaxTick)
        {
            m_aoElementMeterInfo[dwElementType].qwHistoryMaxTick = m_aoElementMeterInfo[dwElementType].qwEventMaxTick;
        }
	    return 0;
    }

    /**
     * @brief 重置最近统计数据
     */
    VOID CCommonEventMeter::Reset()
    {
	    if(FALSE == m_bStatus)
	    {
		    return;
	    }
	    m_qwRecentTickCount = 0;
	    m_dwRecentEventCount = 0;
	    m_dwRecentLoopCount = 0;
    }

    /**
     * @brief 清空全部统计数据
     */
    VOID CCommonEventMeter::Clear()
    {
	    if(FALSE == m_bStatus)
	    {
		    return;
	    }
	    m_qwLastTickCount = 0;
	    m_dwLastEventCount = 0;
	    m_qwTotalTickCount = 0;
	    m_dwTotalEventCount = 0;
	    m_dwTotalLoopCount = 0;
	    m_qwRecentTickCount = 0;
	    m_dwRecentEventCount = 0;
	    m_dwRecentLoopCount = 0;
	    memset(m_aoElementMeterInfo, 0, sizeof(m_aoElementMeterInfo));
    }

    /**
     * @brief 开始统计
     */
    INT32 CCommonEventMeter::Start()
    {
	    if(FALSE == m_bStatus)
	    {
		    m_bStatus = TRUE;
		    return 0;
	    }
	    else
	    {
		    // 已经开始
		    return -1;
	    }
    }

    /**
     * @brief 暂停统计
     */
    INT32 CCommonEventMeter::Stop()
    {
	    if(FALSE == m_bStatus)
	    {
		    return -1;
	    }
	    else
	    {
		    // 已经开始
		    m_bStatus = FALSE;
		    return 0;
	    }
    }


    /**
     * @brief 建立下一次触发
     * @param qwDelay 当前时延
     * @return 所在层级
     */
    UINT8 CWheelItem::SetupForNextTrigger(UINT64 qwDelay, UINT32 dwInterval)
    {
	    UINT8 byLevel = 0;
        if(0 == dwInterval)
        {
	        m_aqwSlotIndexes[0] += m_poElementHandler->m_dwInterval + qwDelay;
        }
        else
        {
            m_aqwSlotIndexes[0] += dwInterval + qwDelay;
        }
	    for(byLevel = 0; byLevel < WHEELARRAY_COUNT - 1; byLevel++)
	    {
		    if(m_aqwSlotIndexes[byLevel] < WHEEL_ARRAY_LEN)
		    {
			    break;
		    }
		    m_aqwSlotIndexes[byLevel + 1] += m_aqwSlotIndexes[byLevel] / WHEEL_ARRAY_LEN;
		    m_aqwSlotIndexes[byLevel] = m_aqwSlotIndexes[byLevel] % WHEEL_ARRAY_LEN;
	    }
	    // 针对最高一层进行特殊处理
	    if(m_aqwSlotIndexes[WHEELARRAY_COUNT-1] >= WHEEL_ARRAY_LEN)
	    {
		    m_aqwSlotIndexes[WHEELARRAY_COUNT-1] %= WHEEL_ARRAY_LEN;
	    }
	    return byLevel;
    }

    /**
     * @brief 获取下一个激发时间
     * @param byLevel 定时器层级
     * @return 下一个激发时间
     */
    UINT64 CWheelItem::GetSlotIndexForNextTrigger( UINT8 level )
    {
	    return m_aqwSlotIndexes[level];
    }

    /**
     * @brief 周期性处理
     */
    INT32 CWheelItem::OnEvent()
    {
        return m_poElementHandler->OnEvent();
    }

    /**
     * @brief 构造函数
     */
    CWheelLevel::CWheelLevel() :
	    m_byWheelLevel(0),
	    m_dwCurSlot(0)
    {
    }

    /**
     * @brief 时间向后移动
     * @param bRewindOnEnd 是否在终点循环到初始位置
     * @return 如果到达终点返回FALSE，否则返回TRUE
     */
    BOOL CWheelLevel::NextSlot(BOOL bRewindOnEnd)
    {
	    if(WHEEL_ARRAY_LEN - 1 == m_dwCurSlot)
	    {
		    if(TRUE == bRewindOnEnd)
		    {
			    m_dwCurSlot = 0;
		    }
		    return FALSE;
	    }
	    else
	    {
		    m_dwCurSlot++;
	    }
	    return TRUE;
    }

    /**
     * @brief 加载新层级时间槽队列
     * @param oSlotLevel 待加载的时间槽层级对象
     * @return 如果到达终点返回FALSE，否则返回TRUE
     */
    VOID CWheelLevel::ReloadFromLevel(CWheelLevel &oSlotLevel)
    {
	    std::list<CWheelItem*> &lsSlot = oSlotLevel.m_aoWheels[oSlotLevel.m_dwCurSlot];
	    CWheelItem* poWheelItem = NULL;
	    while (lsSlot.empty() == FALSE)
	    {
		    poWheelItem = lsSlot.front();
		    lsSlot.pop_front();
		    CWheelLevel::AddItem(poWheelItem);
	    }
    }

}

