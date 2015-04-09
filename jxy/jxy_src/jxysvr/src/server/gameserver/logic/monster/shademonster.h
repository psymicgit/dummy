
#ifndef _SHADE_MONSTER_H_
#define _SHADE_MONSTER_H_

#include <sdtime.h>
#include <sdstring.h>
#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include "monsterpropmgr.h"
#include <logic/instance/instancepropmgr.h>
#include <logic/battle/battleattrobj.h>
#include "monsterpropmgr.h"
#include "monsterupgradepropmgr.h"
#include "monstergodweapon.h"


using namespace std ;
class CPlayer;
class CShadeMonster: public CBattleAttrObj
{
public:
	CShadeMonster();
	~CShadeMonster();

public:
	//初始化
	BOOL Init(UINT16 wKindID, UINT8 byFormationIdx,const SMonsterProp* poProp);
	SMonsterProp* GetMonsterProp();
	SMonsterUpgradeProp* GetMonsterUpgradeProp();
	virtual inline UINT8 GetFormationIdx() { return m_byFormationIdx; }
	VOID		GetDT_GODWEAPON_BATTLE_DATA(DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo);
	UINT16 GetMonsterID() { return m_wKindID;}
	UINT16	GetFakeSkillActionID();
protected:
	virtual VOID InitBattleAttribute();//给m_stBattleAttribute赋值	
	BOOL			InitSkill();	
	BOOL			InitGodweapon();
public:
	UINT32	GetPower();
	VOID SetKindID(UINT16 wKindID);
	VOID SetFormation(UINT8 byFormation);
private:
	UINT16	m_wKindID;
	UINT8	m_byFormationIdx;//阵型序号
	BOOL		m_bBossBMonster;
	UINT16	m_wLevel;
	CMonsterGodWeapon* m_poGodWeapon;
	SMonsterProp m_stMonsterProp;

};


#endif
