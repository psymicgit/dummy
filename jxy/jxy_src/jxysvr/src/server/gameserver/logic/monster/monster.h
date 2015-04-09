
#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <protocol/server/protocommondata.h>
#include "monsterpropmgr.h"
#include <logic/instance/instancepropmgr.h>
#include <logic/battle/battleattrobj.h>
#include "monsterpropmgr.h"
#include "monsterupgradepropmgr.h"
#include "monstergodweapon.h"


using namespace std ;
class CPlayer;
class CMonster: public CBattleAttrObj
{
public:
    CMonster();
    ~CMonster();

public:
    //初始化
    BOOL Init(UINT16 wKindID, UINT8 byFormationIdx, BOOL bBossBMonster = FALSE, UINT16 wLevel = 1);
	SMonsterProp* GetMonsterProp();
	SMonsterUpgradeProp* GetMonsterUpgradeProp();
	virtual inline UINT8 GetFormationIdx() { return m_byFormationIdx; }
	VOID		GetDT_GODWEAPON_BATTLE_DATA(DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo);
	UINT16	GetFakeSkillActionID();
	BOOL InitBattleAttribute(UINT16 wPlayerLevel);
	UINT8 GetMonsterType();
protected:
    virtual VOID InitBattleAttribute();//给m_stBattleAttribute赋值	
    BOOL			InitSkill();	
	BOOL			InitGodweapon();
public:
    VOID		SetMemID(UINT32 dwMemID) { m_dwMemID = dwMemID; }
    UINT32	GetMemID() { return m_dwMemID; }
	UINT32	GetPower();
private:
	UINT16	m_wKindID;
    UINT32	m_dwMemID;//内存ID，便于管理器map管理
	UINT8	m_byFormationIdx;//阵型序号
	BOOL		m_bBossBMonster;
	UINT16	m_wLevel;
	CMonsterGodWeapon* m_poGodWeapon;

};


#endif
