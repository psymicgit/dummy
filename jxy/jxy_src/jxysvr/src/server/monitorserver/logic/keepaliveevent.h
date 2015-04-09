

#ifndef _KEEP_ALIVE_EVENT_H_
#define _KEEP_ALIVE_EVENT_H_

#include <sdframework.h>


class CKeepAliveEvent : public CEvent
{
public:
    CKeepAliveEvent();
    virtual ~CKeepAliveEvent();
    /**
     * @brief
     * 超时处理函数
     */
    virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();
	UINT32		  GetInterval(){return ms_nInterval;};
public:
	const static int ms_nInterval = 5;
};
#endif

