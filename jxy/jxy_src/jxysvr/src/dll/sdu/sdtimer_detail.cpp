#include "sdtimer_impl.h"
#include "sdtimer_detail.h"
#include "sdtime.h"

namespace SGDP
{
    void CTimerItem::Init( CSDTimerImpl* pTimerCtrl, ISDTimer* poTimerHandler, UINT32 dwElapse, UINT32 dwTimerId, UINT32 dwLoopCount)
    {
        m_poTimerHandler  = poTimerHandler;
        m_dwElapse        = dwElapse;
        m_dwTimerID       = dwTimerId;
        m_timerLevel      = TIMERARRAY_COUNT;
        m_dwLastExpTime   = SDGetTickCount();
        m_dwLoopCount     = dwLoopCount;
        for (UINT32 i=0; i<TIMERARRAY_COUNT; i++)
        {
            m_adwSlotIndexes[i] = pTimerCtrl->GetSlotLevel(i).GetCurSlotIndex();
        }
    }

    UINT8 CTimerItem::SetupForNextTrigger( UINT32 delay )
    {
        UINT8 level = 0;

        m_adwSlotIndexes[0] += m_dwElapse + delay;
        for (level = 0; level < TIMERARRAY_COUNT-1; ++level)
        {
            if (m_adwSlotIndexes[level] < TIMER_ARRAY_LEN)
                break;
            m_adwSlotIndexes[level+1] += m_adwSlotIndexes[level] / TIMER_ARRAY_LEN;
            m_adwSlotIndexes[level] = m_adwSlotIndexes[level] % TIMER_ARRAY_LEN;
        }

        // 针对最高一层进行特殊处理
        if (m_adwSlotIndexes[TIMERARRAY_COUNT-1] >= TIMER_ARRAY_LEN)
        {
            m_adwSlotIndexes[TIMERARRAY_COUNT-1] %= TIMER_ARRAY_LEN;
        }

        return level;
    }

    void CTimerItem::OnTimer()
    {
        return m_poTimerHandler->OnTimer(m_dwTimerID);
    }

    UINT32 CTimerItem::GetSlotIndexForNextTrigger( UINT8 level )
    {
        return m_adwSlotIndexes[level];
    }


    CTimerSlotLevel::CTimerSlotLevel() :
        m_timerLevel(0), m_dwCurSlot(0)
    {
    }

    BOOL CTimerSlotLevel::NextSlot( BOOL bRewindOnEnd)
    {
        if(TIMER_ARRAY_LEN-1 == m_dwCurSlot)
        {
            if(bRewindOnEnd)
                m_dwCurSlot = 0;
            return false;
        }
        else
        {
            m_dwCurSlot++;
        }
        return true;
    }

    void CTimerSlotLevel::ReloadFromLevel( CTimerSlotLevel& oSlotLevel )
    {
        TimerSlot& slot = oSlotLevel.m_aoTimerSlots[oSlotLevel.m_dwCurSlot];
        CTimerItem* poTimer = NULL;
        while ( ! slot.empty() )
        {
            poTimer = slot.front();
            slot.pop_front();
            AddTimer(poTimer);
        }
    }
}


