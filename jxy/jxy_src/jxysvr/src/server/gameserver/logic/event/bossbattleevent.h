
#ifndef _BOSSBATTLEEVENT_H_ 
#define _BOSSBATTLEEVENT_H_


#include "eventreactor.h"

using namespace SGDP;

class CBossBattleEvent: public CEvent
{
public:
	CBossBattleEvent();
	virtual ~CBossBattleEvent();
	virtual UINT32 OnEvent();
	virtual BOOL OnDestroy();
public:
	VOID		SetParam(BOOL bStartFlag, UINT8 byActivityID) { m_bStartFlag = bStartFlag; m_byActivityID = byActivityID; }
	UINT32	GetInterval();
private:
	BOOL		m_bStartFlag;//TRUE启动活动，FALSE关闭活动
	UINT8	m_byActivityID;
	BOOL		m_bFirstFlag;
};

#endif


