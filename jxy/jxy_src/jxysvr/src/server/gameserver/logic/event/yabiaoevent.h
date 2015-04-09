#ifndef _YABIAO_EVENT_H
#define _YABIAO_EVENT_H

#include <sdframework.h>

class CYabiaoEvent:public CEvent
{
public:
	CYabiaoEvent();
	virtual ~CYabiaoEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
	virtual INT32  OnDestroy();

	VOID SetParam( UINT32 dwYabiaoPlayerID );
private:
	UINT32 m_dwYabaoPlayerID;

	
};


#endif