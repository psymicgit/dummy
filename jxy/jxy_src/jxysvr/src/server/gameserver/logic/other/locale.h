
#ifndef _LOCALE_H_
#define _LOCALE_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>
using namespace std ;

class CPlayer;
class CLocale:public CBaseObj
{
public:
	CLocale();
	~CLocale();
public:
	//初始化相关对象
	BOOL Init(DT_LOCALE_DATA &stDT_LOCALE_DATA, CPlayer* poOwner);
	DT_LOCALE_DATA& GetDT_LOCALE_DATA() { return m_stDT_LOCALE_DATA; }
	BOOL OnEnterInstance(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIdx);
	BOOL OnPassBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIdx);	
	BOOL OnEnterHome();
	BOOL OnExitInstance();
	BOOL OnOnhook(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);
	BOOL OnOnhookEnd();
	BOOL OnEatBread();
	BOOL OnEatBreadEnd();
	BOOL OnEnterBossB();
	BOOL OnLeaveBossB();
	BOOL OnOverBossB();
	BOOL OnEnterHotSpring();
	BOOL OnLeaveHotSpring();
    //进入队伍界面
    BOOL OnEnterCourage();
    BOOL OnLeaveCourage();
	BOOL OnEnterFaction();
	BOOL OnLeaveFaction();
    BOOL OnEnterCourageBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);
    BOOL OnLeaveCourageBattle();

	// 进入武魂副本
	BOOL OnEnterSoulRep();
	BOOL OnLeaveSoulRep();

	// 进入宠物副本
	BOOL OnEnterPetRep();
	BOOL OnLeavePetRep();

private:
	DT_LOCALE_DATA	m_stDT_LOCALE_DATA;

};
DECLARE_CLASS_POOL_MGR(CLocale)
#endif //#ifndef 

