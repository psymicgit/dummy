#ifndef _RECHARGE_EVENT_H
#define _RECHARGE_EVENT_H
#include <sdframework.h>

class CRechargeEvent:public CEvent
{
public:
	CRechargeEvent();
	virtual ~CRechargeEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();

	UINT32	GetSendHeartInterval();
public:
	const static int ms_nSendHeartInterval=30;
};

#endif