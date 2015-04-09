
#ifndef _CROSSDAYMGR_H_
#define _CROSSDAYMGR_H_

#include <common/server/utility.h>

class CPlayer;
class CCrossDayMgr
{
public:

	static VOID OnCrossDayEvent();
	//在线奖励
	static VOID OnOnlineEncourage(CPlayer* poPlayer);
	//限时勋章计算
	static VOID OnLimitTimeMedal(CPlayer* poPlayer);
};

#endif // 
