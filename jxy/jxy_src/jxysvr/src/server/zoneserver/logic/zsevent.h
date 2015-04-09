

#ifndef _ZS_EVENT_H_
#define _ZS_EVENT_H_

#include <sdframework.h>

#define CHK_SESSIONON_TIMER		1
#define CHK_SYNC_DB_TIMER			2
#define CHK_USER_TIMER				3
#define CHK_SAVE_DB_TIMER			4
#define CHK_MOBILE_TIMER			5

class CZSEvent : public CEvent
{
public:
    CZSEvent();
    virtual ~CZSEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();

    VOID        SetType(const UINT8 byType) { m_byType = byType; }
	VOID        SyncDB();
	VOID        CkUser();
	VOID        SaveDB();
	VOID			CkMobile();
private:
    VOID        CheckSession();

    UINT8		m_byType;
};
#endif

