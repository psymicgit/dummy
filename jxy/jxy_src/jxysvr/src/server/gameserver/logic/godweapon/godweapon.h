#ifndef _GODWEAPON_H_
#define _GODWEAPON_H_

#include <logic/battle/otherlife.h>
#include "godweaponpropmgr.h"
#include <common/server/utility.h>

using namespace SGDP;


class CPlayer;
class CHero;
class CGodWeapon: public COtherLife
{
public:
    CGodWeapon();
    ~CGodWeapon();

public:

    //初始化相关对象
    BOOL Init(DT_GODWEAPON_DATA* pstDT_GODWEAPON_DATA, CPlayer* poOwner, CHero*	poHero);

    DT_GODWEAPON_DATA& GetDT_GODWEAPON_DATA() { return m_stDT_GODWEAPON_DATA; };

    VOID GetDT_GODWEAPON_DATA_CLI(DT_GODWEAPON_DATA_CLI& stDT_GODWEAPON_DATA_CLI) { memcpy(&stDT_GODWEAPON_DATA_CLI, &m_stDT_GODWEAPON_DATA_CLI, sizeof(DT_GODWEAPON_DATA_CLI)); }
    DT_GODWEAPON_DATA_CLI GetDT_GODWEAPON_DATA_CLI()  {  return m_stDT_GODWEAPON_DATA_CLI; }

	const SAllAttrProp* GetCurGodWeaponAttr();

	virtual UINT32 GetHurtValue();
	UINT32 GetExtDefend(){ return m_dwExtDefend;}
	CHero* GetHero() { return m_poHero; }

    UINT16 Upgrade(UINT16 wQuality, UINT16 wLevel);		//升级科技属性加成

	VOID Recover( BOOL bUseGold );//释放神器,回收器魂点

    VOID Reupdate();//重新更新为刚开启
	//增加饰品对神器的影响
	VOID AddBattleByJewelry();

    BOOL	InitData();//初始化数据
public:
	VOID GetDT_GODWEAPON_BATTLE_DATA(UINT16 wHeroKindID, DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo);
	VOID UpgradeByScience(UINT32 dwScience);
protected:
    UINT16	UpgradeQuality();//进阶升级
    UINT16	UpgradeLevel();//普通升级

private:
    DT_GODWEAPON_DATA		m_stDT_GODWEAPON_DATA;//
    CHero*				m_poHero;//
    DT_GODWEAPON_DATA_CLI	m_stDT_GODWEAPON_DATA_CLI;//用于每次返回给客户端不需重复计算
	UINT32 m_dwExtAttack;//额外增加的攻击
	UINT32 m_dwExtDefend;//额外增加的防御 ， 即HP
};
DECLARE_CLASS_POOL_MGR(CGodWeapon)
#endif
