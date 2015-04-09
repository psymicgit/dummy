#include "otherlife.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <logic/hero/heromgr.h>
#include <logic/record/recordmgr.h>

using namespace SGDP;

COtherLife::COtherLife()
{
    memset(&m_stDT_BATTLE_ATTRIBUTE, 0, sizeof(m_stDT_BATTLE_ATTRIBUTE));
    memset(&m_stCurDT_BATTLE_ATTRIBUTE, 0, sizeof(m_stCurDT_BATTLE_ATTRIBUTE));
	memset(&m_stHideBattleAttr, 0, sizeof(m_stHideBattleAttr));
	m_wCurAngry = 50; //默认初始50
}


COtherLife::~COtherLife()
{

}

INT32 COtherLife::AddHPChange(INT32 nHP)
{
	INT32 nRet= 0;
	if(0 == nHP)
	{
		return nRet;
	}
	if(0 == m_stCurDT_BATTLE_ATTRIBUTE.dwHP)
	{
		return nRet;
	}

	nRet = m_stCurDT_BATTLE_ATTRIBUTE.dwHP + nHP  < 0 ? m_stCurDT_BATTLE_ATTRIBUTE.dwHP : nHP;

	INT32 nHPTmp = (INT32)m_stCurDT_BATTLE_ATTRIBUTE.dwHP + nHP;
	if(nHPTmp < 0)
	{
		m_stCurDT_BATTLE_ATTRIBUTE.dwHP = 0;
	}
	else
	{
		m_stCurDT_BATTLE_ATTRIBUTE.dwHP = (UINT32)nHPTmp;
	}
	m_stCurDT_BATTLE_ATTRIBUTE.dwHP = m_stCurDT_BATTLE_ATTRIBUTE.dwHP > m_stDT_BATTLE_ATTRIBUTE.dwHP ? m_stDT_BATTLE_ATTRIBUTE.dwHP : m_stCurDT_BATTLE_ATTRIBUTE.dwHP;

	return nRet;
}

VOID COtherLife::InitCurBattleAttr()
{
	memcpy(&m_stCurDT_BATTLE_ATTRIBUTE, &m_stDT_BATTLE_ATTRIBUTE, sizeof(m_stCurDT_BATTLE_ATTRIBUTE));
	m_wCurAngry = m_stHideBattleAttr.wAngryValue;
	m_wCurAngry = 50; //默认初始50
	m_vecAngryChange.clear();
}


VOID COtherLife::AddAngryChange(INT16 shAngry)
{	
	if(0 == shAngry)
	{
		return;
	}
	DT_ANGRY_DATA stDT_ANGRY_DATA;
	stDT_ANGRY_DATA.byFormationIdx = 0;
	stDT_ANGRY_DATA.shAngry = shAngry;
	m_vecAngryChange.push_back(stDT_ANGRY_DATA);

	INT16 shAngryTmp = (INT16)m_wCurAngry + shAngry;
	if(shAngryTmp < 0)
	{
		m_wCurAngry = 0;
	}
	else
	{
		m_wCurAngry = (UINT16)shAngryTmp;
	}
}