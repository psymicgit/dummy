#ifndef _FACTION_DONATE_H_
#define _FACTION_DONATE_H_

#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <logic/faction/faction.h>

class CPlayer;
class CFactionDonate : public CBaseObj
{
public:
	CFactionDonate(){}
	~CFactionDonate(){}
public:
	// 初始化相关对象
	BOOL Init(DT_PLAYER_DONATE_INFO &stPlayerDonate, CPlayer* poOwner);
	DT_PLAYER_DONATE_INFO& GetDT_PLAYER_DONATE_INFO()
	{
		return m_stDoateInfo;
	}
private:
	DT_PLAYER_DONATE_INFO m_stDoateInfo;
};
#endif