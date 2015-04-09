#include "sdtime.h"
#include "sdtimer_impl.h"
#include <math.h>


static const UINT32 MAX_ENABLE_TIMER = 0xFFFFFFFF - 1000; /**<允许用户设置的超时时间最大值*/

namespace SGDP
{


/////////////////////////////////////////////////////////////////////////////

    CSDTimerImpl::CSDTimerImpl( ) : m_oTimerPool()
    {
        m_oTimerMap.clear();
        for(UINT32 i=0; i<TIMERARRAY_COUNT; ++i)
        {
            m_aoSlotLevels[i].Init(i);
        }


#if (defined(WIN32) || defined(WIN64))
        // WIN32不使用该变量
        m_dwMsPerTick = 1;
#else
        m_dwMsPerTick = 1000 / sysconf(_SC_CLK_TCK);
#endif

        m_dwLastTickCount = SDGetTickCount();
    }

    CSDTimerImpl::~CSDTimerImpl()
    {
        m_oTimerMap.clear();
    }

    BOOL  CSDTimerImpl::SetTimer(ISDTimer* poTimerHandler, UINT32 dwTimerId, UINT32 dwElapse, UINT32 dwLoopCount)
    {
        if (NULL == poTimerHandler || (dwElapse > MAX_ENABLE_TIMER) || (0 == dwElapse))
        {
            return false;
        }

        if (LookupTimer(dwTimerId) != NULL)
        {
            return false;
        }

        CTimerItem* poTimer = m_oTimerPool.Alloc();
        if (NULL == poTimer)
        {
            return false;
        }

        poTimer->Init(this, poTimerHandler, dwElapse, dwTimerId,dwLoopCount);
        m_oTimerMap[dwTimerId] = poTimer;
        return AddTimerForNextTrigger(poTimer);
    }

    BOOL CSDTimerImpl::AddTimerForNextTrigger(CTimerItem * poTimer)
    {
		UINT32 dwMsFromLast = GetMsFromLast(m_dwLastTickCount);
		UINT32 dwLevel = poTimer->SetupForNextTrigger(dwMsFromLast);
		m_aoSlotLevels[dwLevel].AddTimer(poTimer);
        return true;
    }

    BOOL  CSDTimerImpl::KillTimer(UINT32 dwTimerId)
    {
        CTimerMap::iterator iter = m_oTimerMap.find(dwTimerId);
        if (iter == m_oTimerMap.end())
            return false;

        CTimerItem* poTimer = iter->second;
        m_oTimerMap.erase(iter);
        m_aoSlotLevels[poTimer->m_timerLevel].DelTimer(poTimer);
        m_oTimerPool.Free(poTimer);

        return true;
    }

    BOOL  CSDTimerImpl::Run()
    {
        for ( UINT32 dwMsFromLast = GetMsFromLast(); dwMsFromLast>0; --dwMsFromLast)
        {
            TrigerTimersForCurSlot();
        }
        return TRUE;
    }

    UINT32 CSDTimerImpl::TrigerTimersForCurSlot()
    {
	    TimerSlot& oCurSlot = m_aoSlotLevels[0].GetCurSlot();
        UINT32 dwTimeOutCount = 0;

        while( ! oCurSlot.empty() )
        {
            CTimerItem * poTimer = oCurSlot.front();
            poTimer->SetLastExpdTime(SDGetTickCount());
            poTimer->OnTimer();

            // 判断是否在OnTimer里面删除了该Timer
            if (oCurSlot.empty() || (oCurSlot.front() != poTimer))
                continue;

            // 循环定时器，需要再次插入到合适位置
            oCurSlot.pop_front();
            if (poTimer->CheckLoop())
            {
                AddTimerForNextTrigger(poTimer);
            }
            dwTimeOutCount++;
        }
        if( !m_aoSlotLevels[0].NextSlot(true) )
            ReloadSlotLevel();

        return dwTimeOutCount;
    }

    BOOL CSDTimerImpl::ReloadSlotLevel(UINT8 dwLevel)
    {
        if(dwLevel >= TIMERARRAY_COUNT-1)
            return false;

        CTimerSlotLevel& oNextLevel = m_aoSlotLevels[dwLevel+1];

        if( !oNextLevel.NextSlot(true) )
            ReloadSlotLevel(dwLevel+1);

        m_aoSlotLevels[dwLevel].ReloadFromLevel(oNextLevel);
        return true;
    }

	std::string CSDTimerImpl::DumpTimerInfo()
    {
		std::stringstream info; 
        for (UINT8 level=0; level< TIMERARRAY_COUNT; level++)
        {
			info << m_aoSlotLevels[level].DumpTimerInfo() << std::endl;
        }
		return info.str(); 
    }

    UINT32 CSDTimerImpl::GetMsFromLast()
    {
        UINT32 dwCurTick = SDGetTickCount();
        UINT32 dwTickFromLast;
        if (dwCurTick < m_dwLastTickCount)
        {
#if (defined(WIN32) || defined(WIN64))
            dwTickFromLast = (UINT32)0xFFFFFFFF - m_dwLastTickCount + dwCurTick + 1;
#else
            dwTickFromLast = (UINT32)0x7FFFFFFF - m_dwLastTickCount + dwCurTick + 1;
#endif
        }
        else
        {
            dwTickFromLast = dwCurTick - m_dwLastTickCount;
        }
        m_dwLastTickCount = dwCurTick;
        return dwTickFromLast * m_dwMsPerTick;
    }

    UINT32 CSDTimerImpl::GetMsFromLast(UINT32 dwLastTime)
    {
        UINT32 dwCurTick = SDGetTickCount();
        UINT32 dwTickFromLast;
        if (dwCurTick < dwLastTime)
        {
#if (defined(WIN32) || defined(WIN64))
            dwTickFromLast = (UINT32)0xFFFFFFFF - dwLastTime + dwCurTick + 1;
#else
            dwTickFromLast = (UINT32)0x7FFFFFFF - dwLastTime + dwCurTick + 1;
#endif
        }
        else
        {
            dwTickFromLast = dwCurTick - dwLastTime;
        }

        return dwTickFromLast * m_dwMsPerTick;

    }


    CTimerItem* CSDTimerImpl::LookupTimer( UINT32 dwTimerId )
    {
        CTimerMap::iterator iter = m_oTimerMap.find(dwTimerId);
        if( iter == m_oTimerMap.end())
            return NULL;
        return iter->second;
    }

} // end of namespace SGDP


