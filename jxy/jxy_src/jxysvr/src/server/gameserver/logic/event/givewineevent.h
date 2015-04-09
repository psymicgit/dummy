#ifndef _GIVE_WINE_EVENT_H_
#define _GIVE_WINE_EVENT_H_
#include <sdframework.h>

class CGiveWineEvent:public CEvent
{
public:
	CGiveWineEvent();
	virtual ~CGiveWineEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();
	VOID SetParam( UINT32 dwFactionID );
public:
	UINT32 m_dwFactionID;
};

#endif