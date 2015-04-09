

#ifndef _LS_EVENT_H_
#define _LS_EVENT_H_

#include <sdframework.h>

//#define EVENT_REPORT_LOAD	0x1000
//#define EVENTCHECK_SESSION	0x1001

class CLSEvent : public CEvent
{
public:
    CLSEvent();
    virtual ~CLSEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();
};
#endif

