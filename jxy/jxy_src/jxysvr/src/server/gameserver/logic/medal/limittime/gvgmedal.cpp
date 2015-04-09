///<------------------------------------------------------------------------------
//< @file:   medal\limittime\gvgmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014年6月30日 10:37:36
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/limittime/gvgmedal.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/faction/faction.h"
#include "logic/medal/medalpropmgr.h"

namespace gvgmedal
{
    // 当前是否需要屏蔽掉跨服战勋章
    BOOL IsInGvGActivity()
    {
        if (!CGvGBuild::Instance()->IsActivityTime())
        {
            return FALSE;
        }

        BOOL bInAct = TRUE;

        GVG_STAGE eStage = CGvGBuild::Instance()->GetCurStage();
        if (GVG_6_FINAL_WAR_GO == eStage)
        {
            bInAct = !CGvGBuild::Instance()->IsFinalWarEnd();
        }

        return bInAct;
    }
}

BOOL CGvGMedal::IsOverTime()
{
    const SMedalProp* poMedalProp = NULL;
    if ( !GetProp(poMedalProp))
    {
        return TRUE;
    }
    UINT64 qwCurTime = SDTimeSecs();
    UINT64 qwOverTime = CGvGBuild::Instance()->GetEndTime() + poMedalProp->dwOverTime;
    if ( qwCurTime > qwOverTime)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL CGvGMedal::CheckActivate()
{

	if ( IsOverTime() )
	{
		return FALSE;
	}

	//已激活
	if ( 1 == m_stMedalData.byIsActivate)
	{
		return TRUE;
	}

	if ( gvgmedal::IsInGvGActivity() || CGvGBuild::Instance()->GetCurStage() == GVG_0_NOT_START)
	{
        SetActivateState(FALSE);
		return FALSE;
	}

	if ( 0 == GetLevel())
	{
		return FALSE;
	}

	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CGvGMedal::CheckActivate GetProp of %u failed , PlayerID=%u"), MSG_MARK, m_stMedalData.wMedalID, m_poOwner->GetID());
		SetActivateState(FALSE);
		return FALSE;
	}

	const DT_GVG_BUILD_DATA& stGvGData = CGvGBuild::Instance()->GetDT_GVG_BUILD_DATA();
	UINT16 wRankNum = stGvGData.stLastResult.wRankNum;
	if ( 0 == wRankNum)
	{
        SetActivateState(FALSE);
		return FALSE;
	}

	CFaction* poFaction = m_poOwner->GetFaction();
	if ( NULL == poFaction)
	{
		SetActivateState(FALSE);
		return FALSE;
	}
	DT_FACTION_PLAYER_DATA* poPlayerData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if ( NULL == poPlayerData)
	{
		SetActivateState(FALSE);
		return FALSE;
	}
	
    // 如果该玩家财力比拼阶段开始之后才加入门派，则无法获得勋章
	UINT64 qwStartTime = CGvGBuild::Instance()->GetStageTime(GVG_2_MONEY);
	CSDDateTime oStartTime(qwStartTime);
	CSDDateTime oJoinTime(poPlayerData->qwJoinTime);
	if( qwStartTime <= poPlayerData->qwJoinTime)
	{
        SYS_CRITICAL(_SDT("[%s: %d]: CGvGMedal::CheckActivate CGvGBuild::Instance()->GetStartTime() : %s <= poPlayerData->qwJoinTime : %s, PlayerID=%u"), MSG_MARK, oStartTime.ToString("YYYY-mm-dd hh:mm:ss").c_str(), oJoinTime.ToString("YYYY-mm-dd hh:mm:ss").c_str(), m_poOwner->GetID());
		SetActivateState(FALSE);
		return FALSE;
	}
	
	for ( UINT16 wIdx = 0; wIdx < wRankNum; wIdx++)
	{
		if( stGvGData.stLastResult.astFactionRank[wIdx].stFactionBaseInfo.dwFactionID == poFaction->GetDT_FACTION_BASE_DATA().dwFactionID)
		{
			if ( (poMedalProp->qwFirstCondition <= stGvGData.stLastResult.astFactionRank[wIdx].wRank) && (stGvGData.stLastResult.astFactionRank[wIdx].wRank <= poMedalProp->qwSecondCondition))
			{
				em_Faction_Job eJob = poFaction->GetJob( m_poOwner->GetID());
				if ( eJob == poMedalProp->qwThirdCondition)
				{
					SetActivateState(TRUE, CGvGBuild::Instance()->GetEndTime());
					 SYS_CRITICAL(_SDT("[%s: %d]: CGvGMedal::CheckActivate activate Medeal, eJob=%u, PlayerID=%u"), MSG_MARK, eJob, m_poOwner->GetID());
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CGvGMedal::SetActivateState(BOOL bIsActivate, UINT64 qwActivateTime)
{
	if ( bIsActivate)
	{
		return CMedal::SetActivateState(bIsActivate, qwActivateTime);
	}
	else
	{
        const SMedalProp* poMedalProp = NULL;
        if( !GetProp(poMedalProp) )
        {
            return FALSE;
        }
        UINT64 qwCurTime = SDTimeSecs();
        UINT64 qwEndTime = CGvGBuild::Instance()->GetEndTime();
        UINT64 qwOverTime = qwEndTime + poMedalProp->dwOverTime;
		if (gvgmedal::IsInGvGActivity() || qwCurTime >= qwOverTime )
		{
			return CMedal::SetActivateState(bIsActivate, qwActivateTime);
		}
		else
		{
			CSDDateTime oOverTime(qwOverTime);
			CSDDateTime oEndTime(qwEndTime);
			SYS_CRITICAL(_SDT("[%s: %d]:EndTime = %s, OverTime = %s, Left OverTime Value = %llu, PlayerID=%u"), MSG_MARK, oEndTime.ToString("YYYY-mm-dd hh:mm:ss").c_str(), oOverTime.ToString("YYYY-mm-dd hh:mm:ss").c_str(), qwOverTime - qwCurTime, m_poOwner->GetID());
		}
	}
	return FALSE;
}