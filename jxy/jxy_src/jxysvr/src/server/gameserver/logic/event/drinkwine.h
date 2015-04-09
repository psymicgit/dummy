#ifndef _DRINK_WINE_H_
#define _DRINK_WINE_H_
#include <sdframework.h>

class CDrinkWineEvent:public CEvent
{
public:
	CDrinkWineEvent();
	virtual ~CDrinkWineEvent();
	/**
	 * @brief
	 * 超时处理函数
	 */
	virtual UINT32 OnEvent();
    virtual INT32  OnDestroy();
	VOID SetParam( UINT32 dwPlayerID );
public:
	UINT32 m_dwPlayerID;
};

#endif