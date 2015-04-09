
#include "climbtower.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>

#include "logic/crop/cropseedpropmgr.h"
#include "logic/crop/cropfieldpropmgr.h"
#include "logic/other/singleprammgr.h"
#include <framework/gsapi.h>
#include "logic/record/recordmgr.h"
#include "logic/lua/luamgr.h"
#include "logic/other/singleprammgr.h"
#include "logic/vip/vippropmgr.h"
#include "logic/climbtower/climbtower.h"
#include "logic/medal/playermedal.h"
#include "logic/other/fewparam.h"
using namespace SGDP;

IMPLEMENT_CLASS_POOL_MGR(CClimbTower);

CClimbTower::CClimbTower()
{
    memset(&m_stDT_CLIMB_TOWER_DATA, 0x00, sizeof(DT_CLIMB_TOWER_DATA));
}


CClimbTower::~CClimbTower()
{
	
}

BOOL CClimbTower::Init(DT_CLIMB_TOWER_DATA stDT_CLIMB_TOWER_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
    memcpy(&m_stDT_CLIMB_TOWER_DATA, &stDT_CLIMB_TOWER_DATA, sizeof(DT_CLIMB_TOWER_DATA));
    return TRUE;
}

void CClimbTower::RecoveryResetClimbTownNum()
{
	//获取每天可重置的次数
	m_wResetClimbTownNumPerDay = CVipPropMgr::Instance()->GetIncNum( EVINF_CLIMBTOWERRESET, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel );
	if (IsToday(m_stDT_CLIMB_TOWER_DATA.qwResetClimbTownTime))
	{
		return;
	}

	m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum = 0;
	m_stDT_CLIMB_TOWER_DATA.qwResetClimbTownTime = SDTimeSecs();
}


void CClimbTower::GetClimbTowerBaseInfo(UINT16 &wCanResetClimbTownNum, UINT16 & wResetClimbTownNumPerDay)
{
	RecoveryResetClimbTownNum();
	wResetClimbTownNumPerDay = m_wResetClimbTownNumPerDay;
	if (m_wResetClimbTownNumPerDay > m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum)
	{
		wCanResetClimbTownNum = m_wResetClimbTownNumPerDay - m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum;
	}
	else
	{
		wCanResetClimbTownNum = 0;
	}
}

UINT16 CClimbTower::GetResetClimbTowerGold(UINT8 byResetInstanceNum)
{
	if (byResetInstanceNum >= 1 )
	{
		byResetInstanceNum--;
	}
	RecoveryResetClimbTownNum();
	return CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_CLIMBTOWERRESET, m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum, byResetInstanceNum);
}



UINT16 CClimbTower::ResetClimbTowerChallengeTime(UINT8 byTownIdx)//重置挑战时间
{

	RecoveryResetClimbTownNum();

	//获取玩家当前处在的层数
	if (m_poOwner->GetInstance().GetCurrentClimbTower() <= 1)
	{
		return ERR_CLIMB_TOWER_RESET::ID_UNDER_NO_RESET;
	}

	//如果当前已重置次数为0，则今天未重置过，免费，不扣元宝。
	if (0 != m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum)
	{
		UINT16	wPassInstanceIdx = 0;

		UINT32 dwConstGold = GetResetClimbTowerGold((UINT8)m_poOwner->GetInstance().GetCurrentClimbTower());

		//UINT32 dwConstGold = CLuamgr::Instance()->GetFuncCost( EGCF_CLIMBTOWERRESET,
		//                     m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum - CSinglePramMgr::Instance()->GetResetClimbTower() );

		if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwConstGold)
		{
			return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
		}

		if (m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum >= m_wResetClimbTownNumPerDay)
		{
			return ERR_CLIMB_TOWER_RESET::ID_BUY_TIMES_NOT_ENOUGH;
		}

		//扣除元宝
		m_poOwner->DecGold(dwConstGold, CRecordMgr::EDGT_BUYCLIMBTOWERRESETREQ, m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum);
	}
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		poPlayerMedal->AddTowerResetNum();
		poPlayerMedal->CheckMedalBySmallType(EMST_RESET_TOWER);
	}
	CFewParam& oFewParam = m_poOwner->GetFewParam();
	oFewParam.AddResetTowerNum();
	//增加重置次数
	m_stDT_CLIMB_TOWER_DATA.wResetClimbTownNum++;
	return m_poOwner->GetInstance().ResetChallengeTime(byTownIdx);
}


