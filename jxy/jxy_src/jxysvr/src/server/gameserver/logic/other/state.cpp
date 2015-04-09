#include "state.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <logic/item/equip/equippropmgr.h>
#include <logic/instance/instance.h>
#include <logic/science/scienceupdatepropmgr.h>
#include <common/client/errdef.h>

IMPLEMENT_CLASS_POOL_MGR(CState)
CState::CState()
{
	memset(&m_stDT_STATE_DATA, 0 ,sizeof(DT_STATE_DATA));
}

CState::~CState()
{

}

BOOL CState::Init(DT_STATE_DATA& stCurState, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}

	memcpy(&m_stDT_STATE_DATA, &stCurState, sizeof(DT_STATE_DATA));
	m_poOwner = poOwner;	

	return TRUE;
}

VOID CState::GetUnlockInfo(UINT16 wUnlockHeroID[MAX_UNLOCK_HERO_NUM], UINT16& wUnlockEquipLevel,
                            UINT32& dwUnlockElite, UnlockScienceInfo &oUnlockScienceInfo )
{
    for (int i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
    {
        wUnlockHeroID[i] = m_stDT_STATE_DATA.awUnlockHeroID[i];
    }
    wUnlockEquipLevel = m_stDT_STATE_DATA.wUnlockEquipLevel;
    dwUnlockElite = m_stDT_STATE_DATA.dwUnlockElite;
    oUnlockScienceInfo = m_stDT_STATE_DATA.stUnlockScienceList;
}


VOID CState::OnStartBattle()
{
	m_stDT_STATE_DATA.byOpenBoxFlag = 0; //通过是否成功发生战斗来区别是否已经打开过宝箱

	//SetSaveState(ESS_WANT_SAVE);
}

VOID CState::OnEndBattle()
{
	m_stDT_STATE_DATA.byOpenBoxFlag = 0;

	//SetSaveState(ESS_WANT_SAVE);
}


VOID CState::OnOpenEncourageBox()
{
	m_stDT_STATE_DATA.byOpenBoxFlag = 1;

	//SetSaveState(ESS_WANT_SAVE);
}


VOID CState::OnEnterNotLastInstance(UINT64 qwInstanceMaxHP)
{
	m_stDT_STATE_DATA.dwInstanceMAXHP = (UINT32)qwInstanceMaxHP;
	m_stDT_STATE_DATA.dwInstanceCurHP = (UINT32)qwInstanceMaxHP;

	//不是上次的副本，则宝箱奖励重新给
	m_stDT_STATE_DATA.byOpenBoxFlag = 0;
	memset(&m_stDT_STATE_DATA.abyHitItemIdx, 0, sizeof(m_stDT_STATE_DATA.abyHitItemIdx));

	//SetSaveState(ESS_WANT_SAVE);
}

VOID CState::OnExitInstance(UINT64 qwInstanceMaxHP)
{
	m_stDT_STATE_DATA.dwInstanceMAXHP = (UINT32)qwInstanceMaxHP;
	m_stDT_STATE_DATA.dwInstanceCurHP = (UINT32)qwInstanceMaxHP;

	//退出的副本，则宝箱奖励重新给
	m_stDT_STATE_DATA.byOpenBoxFlag = 0;
	memset(&m_stDT_STATE_DATA.abyHitItemIdx, 0, sizeof(m_stDT_STATE_DATA.abyHitItemIdx));

	if(m_stDT_STATE_DATA.wDoubleExpNum > 0)
	{
		m_stDT_STATE_DATA.wDoubleExpNum--;
	}

	//SetSaveState(ESS_WANT_SAVE);
}

VOID CState::OnUseDoubleExpGoods(UINT16 wDoubleExpNum)
{
	m_stDT_STATE_DATA.wDoubleExpNum += wDoubleExpNum;

	//SetSaveState(ESS_WANT_SAVE);
}

//每次挂机回调
VOID CState::OnOnhook()
{
	if(m_stDT_STATE_DATA.wDoubleExpNum > 0)
	{
		m_stDT_STATE_DATA.wDoubleExpNum--;
	}

	//SetSaveState(ESS_WANT_SAVE);
}

//主将升级,满血
VOID CState::OnMainUpgrade(UINT16 wPlayerLevel, UINT64 qweMaxHP, UINT64 qwCurHP)
{
	m_stDT_STATE_DATA.dwInstanceMAXHP = static_cast<UINT32>(qweMaxHP);
	m_stDT_STATE_DATA.dwInstanceCurHP = static_cast<UINT32>(qwCurHP);

	if(0 == m_stDT_STATE_DATA.wUnlockEquipLevel)
	{
		m_stDT_STATE_DATA.wUnlockEquipLevel = CEquipPropMgr::Instance()->GetUnlockEquipDressLevel(wPlayerLevel);

		AddUnlockScienceList( wPlayerLevel );
		
	}

	//SetSaveState(ESS_WANT_SAVE);
}

VOID CState::AddUnlockScienceList( UINT16 wLevel )
{
	//如果科技建筑本身未解锁，则不能升级
	if( NULL==m_poOwner->GetBuild( EBK_SCIENCE ) )
	{
		return ;
	}

	vector< UINT8 > vecUnlockScienceList=CScienceUpdatePropMgr::Instance()->GetUnlockKindListAtLevel( wLevel ) ;

	for( std::size_t nIndex=0; nIndex<vecUnlockScienceList.size(); ++nIndex )
	{
		m_stDT_STATE_DATA.stUnlockScienceList.abyUnlockScienceID[ m_stDT_STATE_DATA.stUnlockScienceList.byUnlockNum++ ] = 
			vecUnlockScienceList[nIndex];
	}
}


//通关宝箱计算命中物品ID
VOID CState::OnRatePassBox(UINT8 abyHitItemIdx[MAX_ENCOURAGE_BOX_GROUP_NUM])
{
	memcpy(&m_stDT_STATE_DATA.abyHitItemIdx, abyHitItemIdx, sizeof(m_stDT_STATE_DATA.abyHitItemIdx));
}


VOID CState::OnUnlockHero(UINT16 wUnlockHeroID)
{
	if(0 == wUnlockHeroID)
	{
		return;
	}

	for (int i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
	{
		if (m_stDT_STATE_DATA.awUnlockHeroID[i] == 0)
		{
			m_stDT_STATE_DATA.awUnlockHeroID[i] = wUnlockHeroID;
			break;
		}
	}
}

VOID CState::ResetUnlockHero()
{
	for (int i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
	{
		m_stDT_STATE_DATA.awUnlockHeroID[i] = 0;
	}
}

VOID CState::OnUnlockSkill(UINT16 wUnlockSkillID)
{
	if(0 == wUnlockSkillID)
	{
		return;
	}
	m_stDT_STATE_DATA.wUnlockSkillID = wUnlockSkillID;
}


VOID CState::OnUnlockEliteInstanceID(UINT32 dwUnlockElite)
{
	if(0 == dwUnlockElite)
	{
		return;
	}
	m_stDT_STATE_DATA.dwUnlockElite = dwUnlockElite;
}


VOID CState::OnEnterHome()
{
	for (int i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
	{
		m_stDT_STATE_DATA.awUnlockHeroID[i] = 0;
	}
	m_stDT_STATE_DATA.wUnlockSkillID = 0;
	m_stDT_STATE_DATA.wUnlockEquipLevel = 0;
	m_stDT_STATE_DATA.dwUnlockElite = 0;
	memset( &m_stDT_STATE_DATA.stUnlockScienceList, 0, sizeof( m_stDT_STATE_DATA.stUnlockScienceList ) );
}

VOID CState::OnPassInstance(UINT8& byUpgradeHeroNum, DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM])
{
	m_stDT_STATE_DATA.byUpgradeHeroNum = byUpgradeHeroNum;
	memcpy(m_stDT_STATE_DATA.astHeroUpgradeInfo, astHeroUpgradeInfo, sizeof(m_stDT_STATE_DATA.astHeroUpgradeInfo));
}
