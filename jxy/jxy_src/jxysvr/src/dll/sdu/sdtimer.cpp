
#include "sdtimer.h"
#include "sdtimer_impl.h"
#include "sddebug.h"

namespace SGDP
{

    CSDTimer::CSDTimer()
    {
        m_pTimerImpl = SDNew CSDTimerImpl();
    }
    CSDTimer::~CSDTimer()
    {
        delete m_pTimerImpl;
    }

    BOOL CSDTimer::SetTimer(ISDTimer* poTimerHandler, UINT32 dwTimerID, UINT32 dwElapse, UINT32 dwLoop/* = 0xFFFFFFFF */)
    {
        return m_pTimerImpl->SetTimer(poTimerHandler,dwTimerID,dwElapse,dwLoop);
    }

    BOOL CSDTimer::KillTimer(UINT32 dwTimerId)
    {
        return m_pTimerImpl->KillTimer(dwTimerId);
    }

    /**
    * @brief
    * 时钟消息处理
    * @return 如果还有事件没处理完返回true，如果所有事件都处理完了，返回false
    */
    BOOL CSDTimer::Run()
    {
        return  m_pTimerImpl->Run();
    }

    /**
    * @brief
    * 打印时钟信息
    * @return void
    */
	std::string CSDTimer::DumpTimerInfo()
    {
        return m_pTimerImpl->DumpTimerInfo();
    }
};


