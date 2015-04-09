
#ifndef _CSAVEONLINEEVENT_H 
#define _CSAVEONLINEEVENT_H


#include "eventreactor.h"

using namespace SGDP;

///保存登录玩家玩家数据事件
class CSaveOnlineEvent: public CEvent
{
public:
	CSaveOnlineEvent();
	virtual ~CSaveOnlineEvent();
	virtual UINT32 OnEvent();
	virtual BOOL OnDestroy();
public:
	VOID SetParam(UINT32 dwPlayerID);
private:
	UINT32	m_dwPlayerID;
};

#endif


