

#ifndef _GATE_SERVER_EVENT_H_
#define _GATE_SERVER_EVENT_H_

#include <sdframework.h>


#define EVENTCHECK_SESSION		0x01
#define EVENT_NOTICE			0x02

class CGTEvent : public CEvent
{
public:
    CGTEvent();
    virtual ~CGTEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32	OnEvent();
    virtual INT32		OnDestroy();
    VOID				SetType(const UINT32 dwType);
private:
    VOID				LoadNotice();
    VOID				CheckSession();
    UINT32			m_dwType;
};
#endif

