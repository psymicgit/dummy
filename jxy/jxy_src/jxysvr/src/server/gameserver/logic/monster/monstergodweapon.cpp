#include "monstergodweapon.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <logic/record/recordmgr.h>
#include <logic/godweapon/godweaponmgr.h>

using namespace SGDP;

IMPLEMENT_CLASS_POOL_MGR(CMonsterGodWeapon);

CMonsterGodWeapon::CMonsterGodWeapon()
{
    m_poMonster = NULL;
}

CMonsterGodWeapon::~CMonsterGodWeapon()
{

}


BOOL CMonsterGodWeapon::Init(UINT16 wQuality, UINT16 wLevel, UINT32 dwHP, UINT32 dwAttack, CMonster* poMonster)
{
    if(NULL == poMonster)
    {
        return FALSE;
    }

    m_stDT_BATTLE_ATTRIBUTE.dwHP = dwHP;
    m_stDT_BATTLE_ATTRIBUTE.dwAttack = dwAttack;
    m_wQuality = wQuality;
    m_wLevel = wLevel;

    m_stCurDT_BATTLE_ATTRIBUTE = m_stDT_BATTLE_ATTRIBUTE;

    m_poMonster = poMonster;

    return TRUE;
}

BOOL CMonsterGodWeapon::Init(UINT16 wQuality, UINT16 wLevel, UINT32 dwHP, UINT32 dwAttack, CShadeMonster* poMonster)
{
    if(NULL == poMonster)
    {
        return FALSE;
    }

    m_stDT_BATTLE_ATTRIBUTE.dwHP = dwHP;
    m_stDT_BATTLE_ATTRIBUTE.dwAttack = dwAttack;
    m_wQuality = wQuality;
    m_wLevel = wLevel;

    m_stCurDT_BATTLE_ATTRIBUTE = m_stDT_BATTLE_ATTRIBUTE;

    m_poShadeMonster = poMonster;

    return TRUE;
}

VOID CMonsterGodWeapon::GetDT_GODWEAPON_BATTLE_DATA(DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo)
{
    stGodweaponInfo.wQuality = m_wQuality;
    stGodweaponInfo.wLevel = m_wLevel;
    stGodweaponInfo.stBattleAttr = m_stDT_BATTLE_ATTRIBUTE;
    stGodweaponInfo.byHPGridNum = 0; //在CHPGridMgr::Instance()->SetHPGridNum赋值
    stGodweaponInfo.wMaxQuality = CGodweaponPropMgr::Instance()->GetMaxQuality(1);//取任意角的
}
