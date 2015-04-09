

#ifndef _ACTIVITY_NOTIFY_EVENT_CMD_H_
#define _ACTIVITY_NOTIFY_EVENT_CMD_H_

#include <sdframework.h>


class CActivityNotifyEvent : public CEvent
{
public:
    CActivityNotifyEvent();
    virtual ~CActivityNotifyEvent();
    /**
     * @brief
     * 超时处理函数
     */
    virtual UINT32	OnEvent();
    virtual INT32		OnDestroy();
	UINT32			GetInterval(){return ms_nInterval;};

	
public:
	const static int ms_nInterval = 60;


};
#endif

