

#ifndef _NS_EVENT_H_
#define _NS_EVENT_H_

#include <sdframework.h>


class CGMEvent : public CEvent
{
public:
    CGMEvent();
    virtual ~CGMEvent();
    /**
     * @brief
     * 超时处理函数
     */
    virtual UINT32	OnEvent();
    virtual INT32		OnDestroy();
    VOID				SetType(const UINT32 dwType);
private:
    VOID				ReportGateSvrLoad();
    VOID				CheckSession();
    UINT32			m_dwType;
};
#endif

