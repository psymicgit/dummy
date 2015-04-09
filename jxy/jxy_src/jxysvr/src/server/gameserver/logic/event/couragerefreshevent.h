#ifndef _COURAGE_REFRESH_EVENT_H
#define _COURAGE_REFRESH_EVENT_H
#include <sdframework.h>

class CCourageRefreshEvent:public CEvent
{
public:
	CCourageRefreshEvent();
	virtual ~CCourageRefreshEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();

	UINT32	GetInterval();
public:
	const static int ms_nInterval = 1;
};

#endif