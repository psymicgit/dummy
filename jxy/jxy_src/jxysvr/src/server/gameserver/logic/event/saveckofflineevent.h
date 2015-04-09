
#ifndef _CSAVECKOFFLINEEVENT_H
#define _CSAVECKOFFLINEEVENT_H


#include "eventreactor.h"

using namespace SGDP;

///保存/检测非登录玩家玩家数据事件,需保存则保持，过期则清除内存
class CSaveCkOfflineEvent: public CEvent
{
public:
	CSaveCkOfflineEvent();
	virtual ~CSaveCkOfflineEvent();
	virtual UINT32 OnEvent();
	virtual BOOL OnDestroy();
public:
	VOID SetParam(UINT32 dwPlayerID);
private:
	UINT32	m_dwPlayerID;
};

#endif


