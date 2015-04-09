

#ifndef _TWO_DAYS_NOT_ONLINE_NOTITY_EVENT_H_
#define _TWO_DAYS_NOT_ONLINE_NOTITY_EVENT_H_

#include <sdframework.h>

using namespace SGDP;

class CTwoDaysNotOnlineNotityEvent : public CEvent
{
public:
    CTwoDaysNotOnlineNotityEvent();
    virtual ~CTwoDaysNotOnlineNotityEvent();
    /**
     * @brief
     * 超时处理函数
     */
    virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();
	UINT32		  GetInterval(){return ms_nInterval;};
public:
	const static int ms_nInterval = 3600;
private:
    UINT64 m_qwNoticeTime;
};
#endif

