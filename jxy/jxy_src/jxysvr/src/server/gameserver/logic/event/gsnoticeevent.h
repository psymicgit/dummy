#ifndef _GSNOTICE_EVENT_H
#define _GSNOTICE_EVENT_H
#include <sdframework.h>

class CGetGsNoticeEvent:public CEvent
{
public:
	CGetGsNoticeEvent();
	virtual ~CGetGsNoticeEvent();
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