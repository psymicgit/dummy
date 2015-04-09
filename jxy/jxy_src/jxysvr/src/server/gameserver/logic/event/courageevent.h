
#ifndef _COURAGEEVENT_H_
#define _COURAGEEVENT_H_


#include "eventreactor.h"

using namespace SGDP;

class CCourageEvent: public CEvent
{
public:
    CCourageEvent();
    virtual ~CCourageEvent();
    virtual UINT32 OnEvent();
    virtual BOOL OnDestroy();
public:
    VOID		SetParam(BOOL bStartFlag, UINT8 byActivityID)
    {
        m_bStartFlag = bStartFlag;
        m_byActivityID = byActivityID;
    }
    UINT32	GetInterval();
private:
    BOOL		m_bStartFlag;//TRUE启动活动，FALSE关闭活动
    UINT8	m_byActivityID;
    BOOL		m_bFirstFlag;
};

#endif


