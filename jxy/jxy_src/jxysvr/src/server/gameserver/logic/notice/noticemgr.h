#ifndef NOTICEMGR_H_
#define NOTICEMGR_H_

#include <sdsingleton.h>

using namespace SGDP;


class CNoticeMgr
{
public:
    DECLARE_SINGLETON(CNoticeMgr)

    BOOL Init();
    VOID UnInit();
	VOID OnRankNotify();
	VOID OnActivityNotifyEatBread(UINT8 byStartHour, UINT8 byStartMin);
	VOID OnActivityNotifyBossBattle(UINT8 byStartHour, UINT8 byStartMin);
	VOID OnActivityNotifyCourage(UINT8 byStartHour, UINT8 byStartMin);

protected:
    CNoticeMgr();
    ~CNoticeMgr();
};
#endif