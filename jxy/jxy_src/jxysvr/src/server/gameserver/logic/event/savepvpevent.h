
#ifndef _CSAVEPVPEVENT_H
#define _CSAVEPVPEVENT_H


#include "eventreactor.h"

using namespace SGDP;

///保存玩家pvp信息事件
class CSavePvpEvent: public CEvent
{
public:
	CSavePvpEvent();
	virtual ~CSavePvpEvent();
	virtual UINT32 OnEvent();
	virtual BOOL OnDestroy();
};

#endif


