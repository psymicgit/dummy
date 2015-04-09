#ifndef _ACTIVITY_ENCOURAGE_EVENT_H
#define _ACTIVITY_ENCOURAGE_EVENT_H
#include <sdframework.h>

class CCActivityEncourageEvent:public CEvent
{
public:
	CCActivityEncourageEvent();
	virtual ~CCActivityEncourageEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();

	UINT32	GetInterval();
public:
	const static int ms_nInterval = 5;
};

#endif