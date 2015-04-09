
#ifndef _STATE_H_
#define _STATE_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>
using namespace std ;

class CPlayer;
class CState:public CBaseObj
{
public:
	CState();
	~CState();
public:
	//初始化相关对象
	BOOL Init(DT_STATE_DATA& stCurState, CPlayer* poOwner);
	DT_STATE_DATA& GetDT_STATE_DATA() { return m_stDT_STATE_DATA; }

	VOID  GetUnlockInfo(UINT16 wUnlockHeroID[MAX_UNLOCK_HERO_NUM],UINT16& wUnlockEquipLevel, 
		UINT32& dwUnlockElite, UnlockScienceInfo &oUnlockScienceInfo );	

	///////////////////////////////////////////////////
	VOID OnStartBattle();
	VOID OnEndBattle();
	VOID OnOpenEncourageBox();
	VOID OnEnterNotLastInstance(UINT64 dwInstanceMaxHP);//进入不是上次的副本	
	VOID OnExitInstance(UINT64 qwInstanceMaxHP);
	VOID OnUseDoubleExpGoods(UINT16 wDoubleExpNum);
	VOID OnOnhook();//每次挂机回调
	VOID OnMainUpgrade(UINT16 wPlayerLevel, UINT64 dweMaxHP, UINT64 dwCurHP);//主将升级,满血
	VOID OnRatePassBox(UINT8 abyHitItemIdx[MAX_ENCOURAGE_BOX_GROUP_NUM]);//通关宝箱计算命中物品ID
	VOID OnUnlockHero(UINT16 wUnlockHeroID);
	VOID ResetUnlockHero();
	VOID OnUnlockSkill(UINT16 wUnlockSkillID);
	VOID OnUnlockEliteInstanceID(UINT32 dwUnlockElite);
	VOID OnEnterHome();
	VOID OnPassInstance(UINT8& byUpgradeHeroNum, DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]);
private:
	VOID AddUnlockScienceList( UINT16 wLevel );
private:
	DT_STATE_DATA	m_stDT_STATE_DATA;

};
DECLARE_CLASS_POOL_MGR(CState)
#endif //#ifndef 

