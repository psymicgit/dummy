#ifndef _COURAGE_FIRE_EVENT_H
#define _COURAGE_FIRE_EVENT_H
#include <sdframework.h>

class CCourageFireEvent:public CEvent
{
public:
	CCourageFireEvent();
	virtual ~CCourageFireEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();
    VOID		SetParam(UINT32 dwPlayerID)
    {
        m_dwPlayerID = dwPlayerID;
    }
	UINT32	GetInterval();
public:
	const static int ms_nInterval = 1;
private:
    UINT32  m_dwPlayerID;
};

#endif