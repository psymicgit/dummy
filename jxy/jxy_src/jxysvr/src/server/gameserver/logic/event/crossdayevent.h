#ifndef _CCROSSDAYEVENT_H
#define _CCROSSDAYEVENT_H
#include <sdframework.h>

class CCrossDayEvent:public CEvent
{
public:
	CCrossDayEvent();
	virtual ~CCrossDayEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();
public:	
	static UINT32  GetInterval();
};

#endif