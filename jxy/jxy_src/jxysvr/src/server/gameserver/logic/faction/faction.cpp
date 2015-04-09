#include "factionmgr.h"
#include "factionprop.h"
#include <logic/player/playermgr.h>
#include <logic/monster/monster.h>
#include <logic/battle/battlemgr.h>
#include <db/autosqlbuf.h>
#include <db/dbmgr.h>
#include <logic/event/timereventmgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/lua/luamgr.h>
#include <net/db/dbpktbuilder.h>
#include <logic/record/recordmgr.h>
#include <algorithm>
#include <functional>
#include <net/cli/usermgr.h>
#include <logic/base/basedefine.h>
#include <logic/activityencourage/activityencouragemgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/item/itemmgr.h>
#include <logic/hero/hero.h>
#include <logic/player/player.h>
#include <logic/chat/chatmgr.h>
#include <logic/faction/factionplayer.h>
#include "logic/hotspring/hotspringpropmgr.h"
#include "factionauthorityprop.h"
#include <logic/vip/vippropmgr.h>
#include "logic/chat/chatmgr.h"
#include "logic/other/locale.h"
#include "framework/gsapi.h"
#include "logic/player/playermgr.h"
#include "common/server/utility.h"
#include "logic/other/singleprammgr.h"
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/gvg/gvgpropmgr.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/player/playersnapshotmgr.h"
#include "logic/gvg/local/gvgfactionmgr.h"
#include "logic/gvg/local/gvggiftmgr.h"
#include "logic/player/playerslogmgr.h"
#include "logic/medal/playermedal.h"
using namespace std;

#define	FACTION_SAVE_TIME	10

CMapFactionPlayerID				CFaction::s_mapFactionPlayerID;
CMultiMapApplyFactionPlayer	CFaction::s_multimapApplyFactionPlayer;

struct SFormationPlayerSort
{
    SFormationPlayerSort(){ memset(this, 0, sizeof(*this)); }

    DT_FACTION_PLAYER_DATA *pstFactionPlayer;
    SPlayerBaseData *pstBaseData;
};

// 根据玩家职位和战力由高到低进行排序
bool SortByJobPowerDesc(const SFormationPlayerSort &first, const SFormationPlayerSort &second)
{
    if(first.pstFactionPlayer->byJobType < second.pstFactionPlayer->byJobType)
    {
        return true;
    }
    else if(first.pstFactionPlayer->byJobType > second.pstFactionPlayer->byJobType)
    {
        return false;
    }
    else
    {
        return first.pstBaseData->dwPower > second.pstBaseData->dwPower;
    }
}

class CFactionPlayerSort
{
public:
	BOOL operator()(const DT_FACTION_PLAYER_DATA* first,const DT_FACTION_PLAYER_DATA* second)
	{
		BOOL bFirstLogin = FALSE;
		BOOL bSecondLogin = FALSE;
		CPlayer* poFirst = CPlayerMgr::Instance()->FindPlayer(first->dwJoinPlayerID);
		CPlayer* poSecond = CPlayerMgr::Instance()->FindPlayer(second->dwJoinPlayerID);

		if ( NULL != poFirst)
		{
			if ( poFirst->CkLogin())
			{
				bFirstLogin = TRUE;
			}
		}

		if ( NULL != poSecond)
		{
			if ( poSecond->CkLogin())
			{
				bSecondLogin = TRUE;
			}
		}

		if ( ( bSecondLogin && bFirstLogin) || ( !bFirstLogin && !bSecondLogin))
		{
			if (first->byJobType < second->byJobType)
			{
				return TRUE;
			}
			else if (first->byJobType == second->byJobType)
			{
				if (second->qwDoorsTributeSum < first->qwDoorsTributeSum)
				{
					return TRUE;
				}
				else if ( first->qwDoorsTributeSum == second->qwDoorsTributeSum)
				{
					const SPlayerBaseData* poPlayerBaseData1 = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(first->dwJoinPlayerID);
					const SPlayerBaseData* poPlayerBaseData2 = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(second->dwJoinPlayerID);

					if (NULL != poPlayerBaseData1 && NULL == poPlayerBaseData2)
					{
						return TRUE;
					}
					if (poPlayerBaseData1->wLevel > poPlayerBaseData2->wLevel)
					{
						return TRUE;
					}
				}
			}
		}
		else if ( bFirstLogin && !bSecondLogin)
		{
			return TRUE;
		}

		return FALSE;
	}
};

class CManagerSort
{
public:
	BOOL operator()(const DT_FACTION_PLAYER_DATA* first,const DT_FACTION_PLAYER_DATA* second)
	{
		if (first->byJobType < second->byJobType)
		{
			return TRUE;
		}
		else if (first->byJobType == second->byJobType)
		{
			if (second->qwDoorsTributeSum < first->qwDoorsTributeSum)
			{
				return TRUE;
			}
			else if ( first->qwDoorsTributeSum == second->qwDoorsTributeSum)
			{
				const SPlayerBaseData* poPlayerBaseData1 = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(first->dwJoinPlayerID);
				const SPlayerBaseData* poPlayerBaseData2 = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(second->dwJoinPlayerID);

				if (NULL != poPlayerBaseData1 && NULL == poPlayerBaseData2)
				{
					return TRUE;
				}
				if (poPlayerBaseData1->wLevel > poPlayerBaseData2->wLevel)
				{
					return TRUE;
				}
				else if ( poPlayerBaseData1->wLevel == poPlayerBaseData2->wLevel)
				{
					if( first->qwJoinTime < second->qwJoinTime )
					{
						return TRUE;
					}
				}
			}
		}
		return FALSE;
	}
};

BOOL	CFaction::CkFactionPlayerData(UINT32 &dwFactionID, UINT32 dwJoinPlayerID)
{
    CMapFactionPlayerIDItr itr = s_mapFactionPlayerID.find(dwJoinPlayerID);
    if (itr != s_mapFactionPlayerID.end())
    {
        dwFactionID = itr->second;
        return TRUE;
    }
    else
    {
        dwFactionID = 0;
        return FALSE;
    }
    return FALSE;
}

BOOL	CFaction::CkFactionApplyPlayerData(UINT32 dwJoinPlayerID, list<UINT32> &lst)
{
    if (s_multimapApplyFactionPlayer.count(dwJoinPlayerID))
    {
        Range   range = s_multimapApplyFactionPlayer.equal_range(dwJoinPlayerID);
        for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second; ++itr)
        {
            lst.push_back(itr->second);
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void CFaction::Init(DT_FACTION_PLAYER_DATA_LST &stDT_FACTION_PLAYER_DATA_LST)
{
    for (UINT32 dwFactionNum = 0; dwFactionNum < stDT_FACTION_PLAYER_DATA_LST.dwFactionNum &&
            dwFactionNum < DT_FACTION_PLAYER_NUM; dwFactionNum++)
    {
        DT_FACTION_PLAYER_DATA	 &stDT_FACTION_PLAYER_DATA = stDT_FACTION_PLAYER_DATA_LST.astFactionPlayerData[dwFactionNum];

        if (1 == stDT_FACTION_PLAYER_DATA.byJoinState)
        {
            m_mapApplyFactionPlayerID[stDT_FACTION_PLAYER_DATA.dwJoinPlayerID] = stDT_FACTION_PLAYER_DATA.dwJoinPlayerID;

            s_multimapApplyFactionPlayer.insert(make_pair(stDT_FACTION_PLAYER_DATA.dwJoinPlayerID, m_stDT_FACTION_BASE_DATA.dwFactionID));
        }
        else
        {
            m_mapFactionPlayerData[stDT_FACTION_PLAYER_DATA.dwJoinPlayerID] = stDT_FACTION_PLAYER_DATA;
            s_mapFactionPlayerID[stDT_FACTION_PLAYER_DATA.dwJoinPlayerID] = m_stDT_FACTION_BASE_DATA.dwFactionID;
        }
    }
}


void CFaction::Init(DT_FACTION_DATA &stDT_FACTION_DATA)
{
    memcpy(&m_stDT_FACTION_BASE_DATA, &stDT_FACTION_DATA.stFactionBaseData, sizeof(m_stDT_FACTION_BASE_DATA));
    for (UINT32 dwFactionNum = 0; dwFactionNum < stDT_FACTION_DATA.stFactionPlayerDataLst.dwFactionNum &&
            dwFactionNum < DT_FACTION_PLAYER_NUM; dwFactionNum++)
    {
        DT_FACTION_PLAYER_DATA	 &stDT_FACTION_PLAYER_DATA = stDT_FACTION_DATA.stFactionPlayerDataLst.astFactionPlayerData[dwFactionNum];

        if (1 == stDT_FACTION_PLAYER_DATA.byJoinState)
        {
            m_mapApplyFactionPlayerID[stDT_FACTION_PLAYER_DATA.dwJoinPlayerID] = stDT_FACTION_PLAYER_DATA.dwJoinPlayerID;

            s_multimapApplyFactionPlayer.insert(make_pair(stDT_FACTION_PLAYER_DATA.dwJoinPlayerID, m_stDT_FACTION_BASE_DATA.dwFactionID));
        }
        else
        {
            m_mapFactionPlayerData[stDT_FACTION_PLAYER_DATA.dwJoinPlayerID] = stDT_FACTION_PLAYER_DATA;
            s_mapFactionPlayerID[stDT_FACTION_PLAYER_DATA.dwJoinPlayerID] = m_stDT_FACTION_BASE_DATA.dwFactionID;
        }
    }
    for (UINT32 wFactionLogNum = 0; wFactionLogNum < stDT_FACTION_DATA.stFactionLogDataLst.wFactionLogNum &&
            wFactionLogNum < MAX_FACTION_LOG_NUM; wFactionLogNum++)
    {
        DT_FACTION_LOG_DATA &stDT_FACTION_LOG_DATA = stDT_FACTION_DATA.stFactionLogDataLst.astFactionLogData[wFactionLogNum];
        m_dequeFactionLogData.push_back(stDT_FACTION_LOG_DATA);
    }

	//GVG数据
    m_stFactionGvGData = stDT_FACTION_DATA.stGvGData;

    //珍品阁数据
    if (m_stDT_FACTION_BASE_DATA.dwRareItemRoomGrade != 0)
    {
        SetFactionRareInfo();
    }
    if (m_stDT_FACTION_BASE_DATA.dwGymnasiumLevel != 0)
    {
        SetFactionSkillInfo();
    }

    memset(&m_stDT_FACTION_COMMON_DATA, 0, sizeof(DT_FACTION_COMMON_DATA));
    memcpy(&m_stDT_FACTION_COMMON_DATA, &stDT_FACTION_DATA.stFactionCommonData, sizeof(DT_FACTION_COMMON_DATA));
    if (ESM_YES == m_stDT_FACTION_COMMON_DATA.stHotSpringData.byGiveWineOpenState)
    {
        SGiveWineProp stGiveWineProp = {0};
        CHotSpringPropMgr::Instance()->GetGiveWinePropByFactionLevel(m_stDT_FACTION_BASE_DATA.dwFactionLevel, stGiveWineProp);
        UINT32 dwKeepTime = stGiveWineProp.dwKeepTime;
        if ( dwKeepTime < SGDP::SDTimeSecs() - m_stDT_FACTION_COMMON_DATA.stHotSpringData.qwGiveWineOpenTime )
        {
            //延迟1秒
            CTimerEventMgr::Instance()->AddGiveWineEvent(m_stDT_FACTION_BASE_DATA.dwFactionID, 1);
        }
        else
        {
            UINT64 qwTime = dwKeepTime + m_stDT_FACTION_COMMON_DATA.stHotSpringData.qwGiveWineOpenTime - SGDP::SDTimeSecs();
            CTimerEventMgr::Instance()->AddGiveWineEvent(m_stDT_FACTION_BASE_DATA.dwFactionID, static_cast<UINT32>(qwTime));
        }
    }
	m_byTodayPassNum = stDT_FACTION_DATA.byTodayPassNum;
	m_qwLastAuditTime = stDT_FACTION_DATA.qwLastAuditTime;
    m_byExt = stDT_FACTION_DATA.byExt;

    //检测数据是否正常
    if(gsapi::GetDBMgr()->IsExistErrorInfo(ERROR_FACTION, m_stDT_FACTION_BASE_DATA.dwFactionID ))
    {
        m_bExceptionFlag = TRUE;
    }
    else
    {
        m_bExceptionFlag = FALSE;
    }
}

void CFaction::GetDT_FACTION_LOG_DATA_LST(DT_FACTION_LOG_DATA_LST &stDT_FACTION_LOG_DATA_LST)
{
    CDequeFactionLogDataItr itr;
    memset(&stDT_FACTION_LOG_DATA_LST, 0x00, sizeof(stDT_FACTION_LOG_DATA_LST));

    for (itr = m_dequeFactionLogData.begin(); itr != m_dequeFactionLogData.end() &&
            stDT_FACTION_LOG_DATA_LST.wFactionLogNum < MAX_FACTION_LOG_NUM; itr++)
    {
        DT_FACTION_LOG_DATA &stDT_FACTION_LOG_DATA = stDT_FACTION_LOG_DATA_LST.astFactionLogData[stDT_FACTION_LOG_DATA_LST.wFactionLogNum++];
        memcpy(&stDT_FACTION_LOG_DATA, (void*) & (*itr), sizeof(DT_FACTION_LOG_DATA));
    }
}

VOID CFaction::GetFactionGvGData(DT_FACTION_GVG_DATA& stGvGData)
{
	memcpy(&stGvGData, &m_stFactionGvGData, sizeof(DT_FACTION_GVG_DATA));
}

UINT16 CFaction::GetOriZoneID()
{
    UINT16 wZoneID = (UINT16)(GetFactionID() / 1000000);
    if (0 == wZoneID)
    {
        wZoneID = gsapi::GetZoneID();
    }

    return wZoneID;
}

void CFaction::GetDT_FACTION_PLAYER_DATA_LST(DT_FACTION_PLAYER_DATA_LST &stDT_FACTION_PLAYER_DATA_LST)
{
    memset(&stDT_FACTION_PLAYER_DATA_LST, 0x00, sizeof(DT_FACTION_PLAYER_DATA_LST));
    for (CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end() &&
            stDT_FACTION_PLAYER_DATA_LST.dwFactionNum < DT_FACTION_PLAYER_NUM; itr++)
    {
        DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = stDT_FACTION_PLAYER_DATA_LST.astFactionPlayerData[stDT_FACTION_PLAYER_DATA_LST.dwFactionNum++];
        memcpy(&stDT_FACTION_PLAYER_DATA, &itr->second, sizeof(DT_FACTION_PLAYER_DATA));
        stDT_FACTION_PLAYER_DATA.byJoinState = 0;
    }

    for (CMapFactionPlayerIDItr applyItr = m_mapApplyFactionPlayerID.begin(); applyItr != m_mapApplyFactionPlayerID.end() &&
            stDT_FACTION_PLAYER_DATA_LST.dwFactionNum < DT_FACTION_PLAYER_NUM; applyItr++)
    {
        DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = stDT_FACTION_PLAYER_DATA_LST.astFactionPlayerData[stDT_FACTION_PLAYER_DATA_LST.dwFactionNum++];
        stDT_FACTION_PLAYER_DATA.dwJoinPlayerID = applyItr->second;
        stDT_FACTION_PLAYER_DATA.byJoinState = 1;
    }

}

//////////////////////////////////////////////////////////////////////////
void CFaction::GetDT_FACTION_PLAYER_CLI_LST(DT_FACTION_PLAYER_CLI_LST &stDT_FACTION_PLAYER_CLI_LST)
{
    CMapFactionPlayerDataItr itr;
    memset(&stDT_FACTION_PLAYER_CLI_LST, 0x00, sizeof(DT_FACTION_PLAYER_CLI_LST));

    m_listSoryByJob.clear();
    for (itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
    {
        m_listSoryByJob.push_back(&itr->second);
    }
    m_listSoryByJob.sort(CFactionPlayerSort());

    CListPlayerItr sortItr = m_listSoryByJob.begin();
    for( ; sortItr != m_listSoryByJob.end() && stDT_FACTION_PLAYER_CLI_LST.wFactionNum < DT_FACTION_PLAYER_NUM; sortItr++)
    {
		GetDT_FACTION_PLAYER_CLI(stDT_FACTION_PLAYER_CLI_LST.astFactionPlayerCli[stDT_FACTION_PLAYER_CLI_LST.wFactionNum++], (*sortItr)->dwJoinPlayerID);
	}
}

void CFaction::GetDT_FACTION_PLAYER_CLI(DT_FACTION_PLAYER_CLI& stCli, UINT32 dwPlayerID)
{
	memset(&stCli, 0, sizeof(DT_FACTION_PLAYER_CLI));
	CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwPlayerID);
	if ( itr != m_mapFactionPlayerData.end())
	{
		DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = itr->second;
		const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stDT_FACTION_PLAYER_DATA.dwJoinPlayerID);
		if( NULL == poPlayerBaseData )
		{
			return ;
		}
		stCli.wHeroKindID = poPlayerBaseData->wMainHeroKindID;
		stCli.dwPlayerID = stDT_FACTION_PLAYER_DATA.dwJoinPlayerID;
		strcpy( stCli.aszDispName, poPlayerBaseData->aszDispName );
		stCli.wLevel = poPlayerBaseData->wLevel;
		stCli.wRank = poPlayerBaseData->dwRank;

		stCli.byJobType = stDT_FACTION_PLAYER_DATA.byJobType;
		stCli.dwDoorsTributeValues = stDT_FACTION_PLAYER_DATA.dwDoorsTribute;
		stCli.qwDoorsTributeSum = stDT_FACTION_PLAYER_DATA.qwDoorsTributeSum;
		CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(stDT_FACTION_PLAYER_DATA.dwJoinPlayerID);
		if (NULL == poPlayer)
		{
			stCli.dwOffLineSeconds = (UINT32)(SDTimeSecs() - poPlayerBaseData->qwLogoffTime);
		}
		else
		{
			if (poPlayer->CkLogin())
			{
				stCli.dwOffLineSeconds = 0;
			}
			else
			{
				stCli.dwOffLineSeconds = (UINT32)(SDTimeSecs() - poPlayerBaseData->qwLogoffTime);
			}
		}
	}
	
}

void CFaction::GetDT_FACTION_PLAYER_AUDIT_CLI_LST(DT_FACTION_PLAYER_AUDIT_CLI_LST &stDT_FACTION_PLAYER_AUDIT_CLI_LST)
{
    CMapFactionPlayerIDItr itr;
    memset(&stDT_FACTION_PLAYER_AUDIT_CLI_LST, 0x00, sizeof(stDT_FACTION_PLAYER_AUDIT_CLI_LST));
    for (itr = m_mapApplyFactionPlayerID.begin(); itr != m_mapApplyFactionPlayerID.end() &&
            stDT_FACTION_PLAYER_AUDIT_CLI_LST.wFactionNum < DT_FACTION_PLAYER_NUM; itr++)
    {
        //
        UINT32  dwJoinPlayerID = itr->second;
        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwJoinPlayerID);
        if( NULL == poPlayerBaseData )
        {
            continue;
        }

        DT_FACTION_PLAYER_AUDIT_CLI &stDT_FACTION_PLAYER_AUDIT_CLI = stDT_FACTION_PLAYER_AUDIT_CLI_LST.astFactionPlayerAuditCli[stDT_FACTION_PLAYER_AUDIT_CLI_LST.wFactionNum++];
        stDT_FACTION_PLAYER_AUDIT_CLI.dwID = dwJoinPlayerID;
        stDT_FACTION_PLAYER_AUDIT_CLI.wHeroKindID = poPlayerBaseData->wMainHeroKindID;
        SDStrcpy( stDT_FACTION_PLAYER_AUDIT_CLI.aszDispName, poPlayerBaseData->aszDispName );
        stDT_FACTION_PLAYER_AUDIT_CLI.wLevel = poPlayerBaseData->wLevel;
        stDT_FACTION_PLAYER_AUDIT_CLI.dwRank = poPlayerBaseData->dwRank;
        stDT_FACTION_PLAYER_AUDIT_CLI.dwPower = poPlayerBaseData->dwPower;
        stDT_FACTION_PLAYER_AUDIT_CLI.dwFirstAttack = poPlayerBaseData->dwFirstAttack;
    }
}

void CFaction::GetDT_FACTION_PLAYER_AUDIT_CLI_LST2(DT_FACTION_PLAYER_AUDIT_CLI_LST2 &stDT_FACTION_PLAYER_AUDIT_CLI_LST)
{
	CMapFactionPlayerIDItr itr;
	memset(&stDT_FACTION_PLAYER_AUDIT_CLI_LST, 0x00, sizeof(stDT_FACTION_PLAYER_AUDIT_CLI_LST));
	for (itr = m_mapApplyFactionPlayerID.begin(); itr != m_mapApplyFactionPlayerID.end() &&
		stDT_FACTION_PLAYER_AUDIT_CLI_LST.wPlayerNum < DT_FACTION_PLAYER_NUM; itr++)
	{
		UINT32  dwJoinPlayerID = itr->second;
		const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwJoinPlayerID);
		if( NULL == poPlayerBaseData )
		{
			continue;
		}

		DT_FACTION_PLAYER_AUDIT_CLI2 &stDT_FACTION_PLAYER_AUDIT_CLI = stDT_FACTION_PLAYER_AUDIT_CLI_LST.astFactionPlayerAuditCli[stDT_FACTION_PLAYER_AUDIT_CLI_LST.wPlayerNum++];
		stDT_FACTION_PLAYER_AUDIT_CLI.dwID = dwJoinPlayerID;
		stDT_FACTION_PLAYER_AUDIT_CLI.wHeroKindID = poPlayerBaseData->wMainHeroKindID;
		SDStrcpy( stDT_FACTION_PLAYER_AUDIT_CLI.aszDispName, poPlayerBaseData->aszDispName );
		stDT_FACTION_PLAYER_AUDIT_CLI.wLevel = poPlayerBaseData->wLevel;
		stDT_FACTION_PLAYER_AUDIT_CLI.dwPower = poPlayerBaseData->dwPower;
		stDT_FACTION_PLAYER_AUDIT_CLI.byVIPLevel = poPlayerBaseData->byVipLevel;
	}
}

//获取玩家帮派的基本信息
void CFaction::GetDT_FACTION_BASE_OWNER_DATA_CLI(DT_FACTION_BASE_OWNER_DATA_CLI &stDT_FACTION_BASE_OWNER_DATA_CLI, UINT32 dwJoinPlayerID)
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwJoinPlayerID);
    if(itr == m_mapFactionPlayerData.end())
    {
        return;
    }
    DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = itr->second;
    //获取帮主名称
    const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_FACTION_BASE_DATA.dwFactionPlayerID);
    if( NULL != poPlayerBaseData )
    {
        SDStrcpy( stDT_FACTION_BASE_OWNER_DATA_CLI.aszHeadName, poPlayerBaseData->aszDispName );
    }
    GetFactionPlayerNum(stDT_FACTION_BASE_OWNER_DATA_CLI.wFactionNumberPeople, stDT_FACTION_BASE_OWNER_DATA_CLI.wMaxFactionNumberPeople);
    stDT_FACTION_BASE_OWNER_DATA_CLI.dwFactionFunds = m_stDT_FACTION_BASE_DATA.dwFactionFunds;
    stDT_FACTION_BASE_OWNER_DATA_CLI.dwMyDoorsTribute = stDT_FACTION_PLAYER_DATA.dwDoorsTribute;
    stDT_FACTION_BASE_OWNER_DATA_CLI.qwMySumDoorsTribute = stDT_FACTION_PLAYER_DATA.qwDoorsTributeSum;
    memcpy(&stDT_FACTION_BASE_OWNER_DATA_CLI.stFactionBaseData, &m_stDT_FACTION_BASE_DATA, sizeof(DT_FACTION_BASE_DATA));
    DT_FACTION_AUTHORITY oDT_FACTION_AUTHORITY = CFactorAuthorityPropMgr::Instance()->GetAuthority((em_Faction_Job)stDT_FACTION_PLAYER_DATA.byJobType);
    
    memcpy(&stDT_FACTION_BASE_OWNER_DATA_CLI.stFactionAuthority, &oDT_FACTION_AUTHORITY, sizeof(DT_FACTION_AUTHORITY));
    
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwJoinPlayerID);
	if (NULL != poPlayer)
	{
		CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
		if ( NULL != poFactionPlayer)
		{
			DT_PLAYER_DONATE_INFO& stDonateInfo = poFactionPlayer->GetFactionDonate();
			stDT_FACTION_BASE_OWNER_DATA_CLI.byRecvFlag = (ESM_YES == stDonateInfo.byTodaysCoinDoorsTributeEncourage ? ESM_YES : ESM_NO);
		}		
	}

}
//获取玩家自己的信息
VOID CFaction::GetDT_FACTION_PLAYER_CLI_SELF(DT_FACTION_PLAYER_CLI& stDTFACTION_PLAYER_CLI, UINT32 dwPlayerID)
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwPlayerID);
    if ( itr != m_mapFactionPlayerData.end())
    {
        const SPlayerBaseData* poBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
        if (poBaseData == NULL)
        {
            return ;
        }
        SDStrncpy(stDTFACTION_PLAYER_CLI.aszDispName, poBaseData->aszDispName, USERNAME_LEN - 1);
        stDTFACTION_PLAYER_CLI.byJobType = itr->second.byJobType;
        stDTFACTION_PLAYER_CLI.qwDoorsTributeSum = itr->second.qwDoorsTributeSum;
        stDTFACTION_PLAYER_CLI.dwDoorsTributeValues = itr->second.dwDoorsTribute;

        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if (NULL == poPlayer)
        {
            stDTFACTION_PLAYER_CLI.dwOffLineSeconds = (UINT32)(SDTimeSecs() - poBaseData->qwLogoffTime);
        }
        else
        {
            if (poPlayer->CkLogin())
            {
                stDTFACTION_PLAYER_CLI.dwOffLineSeconds = 0;
            }
            else
            {
                stDTFACTION_PLAYER_CLI.dwOffLineSeconds = (UINT32)(SDTimeSecs() - poBaseData->qwLogoffTime);
            }
        }
        stDTFACTION_PLAYER_CLI.dwPlayerID = dwPlayerID;
        stDTFACTION_PLAYER_CLI.wHeroKindID = poBaseData->wMainHeroKindID;
        stDTFACTION_PLAYER_CLI.wLevel = poBaseData->wLevel;
        stDTFACTION_PLAYER_CLI.wRank = poBaseData->dwRank;
    }
    //DT_FACTION_PLAYER_CLI_LST stDT_FACTIION_PLAYER_CLI_LST = {0};
    //GetDT_FACTION_PLAYER_CLI_LST(stDT_FACTIION_PLAYER_CLI_LST);
    //UINT16 wPlayerNum = stDT_FACTIION_PLAYER_CLI_LST.wFactionNum;
    //for (UINT16 wIndex = 0; wIndex < wPlayerNum; wIndex++)
    //{
    //	if (dwPlayerID == stDT_FACTIION_PLAYER_CLI_LST.astFactionPlayerCli[wIndex].dwPlayerID)
    //	{
    //		memcpy( &stDTFACTION_PLAYER_CLI, &stDT_FACTIION_PLAYER_CLI_LST.astFactionPlayerCli[wIndex], sizeof(DT_FACTION_PLAYER_CLI));
    //		return;
    //	}
    //}
}

UINT16 CFaction::ApplyFactionData(UINT32 dwJoinPlayerID)
{
    //检查门派是否异常
    if ( m_bExceptionFlag)
    {
        return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
    }
	UINT16 wCurPlayerNum = 0;
	UINT16 wMaxPlayerNum = 0;
	GetFactionPlayerNum(wCurPlayerNum, wMaxPlayerNum);

	if ( wCurPlayerNum == wMaxPlayerNum)
	{
		return ERR_APPLY_FACTION::ID_FACTION_PLAYER_IS_MAX;
	}
	
    //先查找一下是否已加入帮派
    CMapFactionPlayerIDItr s_itr = s_mapFactionPlayerID.find(dwJoinPlayerID);
    if(s_itr != s_mapFactionPlayerID.end())
    {
        return ERR_APPLY_FACTION::ID_HAVE_FACTION;
    }

    //检测当前帮派中是否正在申请加入
    CMapFactionPlayerIDItr v_itr = m_mapApplyFactionPlayerID.find(dwJoinPlayerID);
    if(v_itr != m_mapApplyFactionPlayerID.end())
    {
        return ERR_APPLY_FACTION::ID_HAVE_APPLY_FACTION;
    }

    //检测当前玩家个数是否超过上限
    if (m_mapFactionPlayerData.size() + m_mapApplyFactionPlayerID.size() >= DT_FACTION_PLAYER_NUM)
    {
        return ERR_APPLY_FACTION::ID_APPLY_NUM_TOO_MUCH;
    }

    //往队列里加入记录信息
    m_mapApplyFactionPlayerID[dwJoinPlayerID] = dwJoinPlayerID;
    s_multimapApplyFactionPlayer.insert(make_pair(dwJoinPlayerID, m_stDT_FACTION_BASE_DATA.dwFactionID));
    return ERR_APPLY_FACTION::ID_SUCCESS;
}

UINT16 CFaction::CancelApplyFactionPlayerData(UINT32 dwJoinPlayerID)
{
    if ( m_bExceptionFlag)
    {
        return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
    }
    CMapFactionPlayerIDItr itr = m_mapApplyFactionPlayerID.find(dwJoinPlayerID);
    if(itr == m_mapApplyFactionPlayerID.end())
    {
        return ERR_CANCEL_APPLY_FACTION::ID_NOT_APPLY_FACTION;
    }
    //删除信息
    m_mapApplyFactionPlayerID.erase(dwJoinPlayerID);
    Range   range = s_multimapApplyFactionPlayer.equal_range(dwJoinPlayerID);
    for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second;)
    {
        if (itr->second == m_stDT_FACTION_BASE_DATA.dwFactionID)
        {
            s_multimapApplyFactionPlayer.erase(itr++);
            break;
        }
        else
        {
            itr++;
        }
    }
    return ERR_CANCEL_APPLY_FACTION::ID_SUCCESS;
}


//创建帮派
UINT16 CFaction::CreateFactionPlayerData(UINT32 dwJoinPlayerID, DT_FACTION_DATA &stDT_FACTION_DATA, vector<UINT32> &vecFactionID)
{
    CMapFactionPlayerIDItr itr = s_mapFactionPlayerID.find(dwJoinPlayerID);
    if (itr != s_mapFactionPlayerID.end())
    {
        return ERR_CREATE_FACTION::ID_PLAYER_HAVE_FACTION;
    }

    Init(stDT_FACTION_DATA);
    //珍品阁数据
    if (m_stDT_FACTION_BASE_DATA.dwRareItemRoomGrade != 0)
    {
        SetFactionRareInfo();
    }
    if (m_stDT_FACTION_BASE_DATA.dwGymnasiumLevel != 0)
    {
        SetFactionSkillInfo();
    }
    //删除全局数据
    Range   range = s_multimapApplyFactionPlayer.equal_range(dwJoinPlayerID);
    for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second;)
    {
        vecFactionID.push_back(itr->second);
        s_multimapApplyFactionPlayer.erase(itr++);
    }
    return ERR_CREATE_FACTION::ID_SUCCESS;
}


UINT16 CFaction::KickedFaction(UINT32 dwJoinPlayerID, UINT32 dwKickedPlayerID, PKT_CLIGS_KICKED_FACTION_ACK& stAck)
{
    if ( m_bExceptionFlag )
    {
        return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
    }
    CMapFactionPlayerDataItr v_itr1 = m_mapFactionPlayerData.find(dwJoinPlayerID);
    if(v_itr1 == m_mapFactionPlayerData.end())
    {
        return ERR_KICKED_FACTION::ID_NOT_APPLY_FACTION;
    }

    CMapFactionPlayerDataItr v_itr2 = m_mapFactionPlayerData.find(dwKickedPlayerID);
    if(v_itr2 == m_mapFactionPlayerData.end())
    {
        return ERR_KICKED_FACTION::ID_NOT_APPLY_FACTION;
    }

    DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA1 = v_itr1->second;
    DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA2 = v_itr2->second;

    if ( em_Faction_Normal == stDT_FACTION_PLAYER_DATA2.byJobType)
    {
        if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stDT_FACTION_PLAYER_DATA1.byJobType, EFA_KickedNormal))
        {
            return ERR_KICKED_FACTION::ID_PLAYER_NOT_AUTHOR;
        }
    }
    else if(em_Faction_Elite == stDT_FACTION_PLAYER_DATA2.byJobType)
    {
        if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stDT_FACTION_PLAYER_DATA1.byJobType, EFA_KickedElite))
        {
            if ( em_Faction_ProxyMartialHead == stDT_FACTION_PLAYER_DATA1.byJobType )
            {
                return ERR_KICKED_FACTION::ID_ONLY_KICKED_NORMAL;
            }
            return ERR_KICKED_FACTION::ID_PLAYER_NOT_AUTHOR;
        }
    }
    else
    {
        if ( em_Faction_MartialHead == stDT_FACTION_PLAYER_DATA1.byJobType)
        {
            return ERR_KICKED_FACTION::ID_ONLY_KICKED_ELITE_AND_NORMAL;
        }
        else if( em_Faction_ProxyMartialHead == stDT_FACTION_PLAYER_DATA1.byJobType )
        {
            return ERR_KICKED_FACTION::ID_ONLY_KICKED_NORMAL;
        }
        return ERR_KICKED_FACTION::ID_PLAYER_NOT_AUTHOR;
    }

    UINT32 dwFactionID = GetFactionID();
    if (!CGvGBuild::Instance()->CanLeaveFaction(dwFactionID))
    {
        return ERR_KICKED_FACTION::ID_IN_GVG_ACTIVITY_CAN_NOT_KICK;
    }

    DT_FACTION_LOG_DATA stFactionLogData;
    memset(&stFactionLogData, 0, sizeof(DT_FACTION_LOG_DATA));
    stFactionLogData.byAction = EFLID_Kicked;
    stFactionLogData.byPlayerJobType1 = stDT_FACTION_PLAYER_DATA1.byJobType;
    stFactionLogData.byPlayerJobType2 = stDT_FACTION_PLAYER_DATA2.byJobType;
    stFactionLogData.dwPlayerID1 = dwJoinPlayerID;
    stFactionLogData.dwPlayerID2 = dwKickedPlayerID;
	stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
    AddFactionLogData(stFactionLogData);

    CPlayer* poKickedPlayer = CPlayerMgr::Instance()->FindPlayer(dwKickedPlayerID);
    if (NULL != poKickedPlayer)
    {
        poKickedPlayer->GetLocale().OnEnterHome();
    }
    CFactionPlayer* poFactionPlayer = poKickedPlayer->GetFactionPlayer();
    if (NULL != poFactionPlayer)
    {
        poFactionPlayer->GetHotSpring().AfterGiveWineProc();
		poFactionPlayer->SetFindBackDoorsTribute();
    }

	m_mapFactionPlayerData.erase(dwKickedPlayerID);
	s_mapFactionPlayerID.erase(dwKickedPlayerID);
	//人员变动标识
	SetPlayerChangeToAll();
    GetDT_FACTION_PLAYER_CLI_LST(stAck.stFactionPlayer);
	stAck.stDoorsTributeLst.wDoorsTributeNum = stAck.stFactionPlayer.wFactionNum;

	for ( UINT16 wIndex = 0; wIndex < stAck.stFactionPlayer.wFactionNum; wIndex++)
	{
		CFactorPropMgr::Instance()->GetDoorsTributeInfo(stAck.stFactionPlayer.astFactionPlayerCli[wIndex].qwDoorsTributeSum, stAck.stDoorsTributeLst.astDoorsTributeInfo[wIndex]);
	}
	UINT16 wCurPlayerNum = 0;
	GetFactionPlayerNum( wCurPlayerNum, stAck.wMaxPlayerNum);
    return ERR_KICKED_FACTION::ID_SUCCESS;
}




//打开帮派门贡面板
UINT16 CFaction::OpenDoorsTributeFaction(DT_DOORS_TRIBUTE_TODAY_DATA &stDT_DOORS_TRIBUTE_BASE_DATA, UINT32 dwJoinPlayerID)
{
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwJoinPlayerID);
	if ( NULL == poPlayer)
	{
		return ERR_OPEN_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_IN_FACTION;
	}

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL == poFactionPlayer)
	{
		return ERR_OPEN_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_IN_FACTION;
	}
	DT_PLAYER_DONATE_INFO& stDonateInfo = poFactionPlayer->GetFactionDonate();
	
    if (!IsToday(stDonateInfo.qwLastCoinDoorsTributeTime))
    {
        stDonateInfo.wTodaysCoinDoorsTributeTimes = 0;
        stDonateInfo.byTodaysCoinDoorsTributeEncourage = 0;//奖励清零
        stDonateInfo.qwLastCoinDoorsTributeTime = SGDP::SDTimeSecs();
    }

    if (!IsToday(stDonateInfo.qwLastGoldDoorsTributeTime))
    {
        stDonateInfo.wTodaysGoldDoorsTributeTimes = 0;
        stDonateInfo.qwLastGoldDoorsTributeTime = SGDP::SDTimeSecs();
    }
    stDT_DOORS_TRIBUTE_BASE_DATA.wCoinTodaysDoorsTributeTimes = stDonateInfo.wTodaysCoinDoorsTributeTimes;
    stDT_DOORS_TRIBUTE_BASE_DATA.byCoinRecvFlag = stDonateInfo.byTodaysCoinDoorsTributeEncourage;
    stDT_DOORS_TRIBUTE_BASE_DATA.wGoldTodaysDoorsTributeTimes = stDonateInfo.wTodaysGoldDoorsTributeTimes;
    stDT_DOORS_TRIBUTE_BASE_DATA.byGoldRecvFlag = 0;
    return ERR_OPEN_DOORS_TRIBUTE_FACTION::ID_SUCCESS;
}

//门贡请求
UINT16 CFaction::DoorsTributeFaction(DT_DOORS_TRIBUTE_TODAY_DATA &stDT_DOORS_TRIBUTE_TODAY_DATA, UINT8 byDoorsTributeFlag, CPlayer* poPlayer)
{

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL == poFactionPlayer)
	{
		return ERR_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_IN_FACTION;
	}
	DT_PLAYER_DONATE_INFO& stDonateInfo = poFactionPlayer->GetFactionDonate();

	if (!IsToday(stDonateInfo.qwLastCoinDoorsTributeTime))
	{
		stDonateInfo.wTodaysCoinDoorsTributeTimes = 0;
		stDonateInfo.byTodaysCoinDoorsTributeEncourage = 0;//奖励清零
		stDonateInfo.qwLastCoinDoorsTributeTime = SGDP::SDTimeSecs();
	}

	if (!IsToday(stDonateInfo.qwLastGoldDoorsTributeTime))
	{
		stDonateInfo.wTodaysGoldDoorsTributeTimes = 0;
		stDonateInfo.qwLastGoldDoorsTributeTime = SGDP::SDTimeSecs();
	}

    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(poPlayer->GetID());
    if(itr == m_mapFactionPlayerData.end())
    {
        return ERR_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }
    UINT32 dwFactionLevel = GetDT_FACTION_BASE_DATA().dwFactionLevel;
    DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = itr->second;
    UINT32 dwVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
    if (!IsToday(stDonateInfo.qwLastCoinDoorsTributeTime))
    {
        stDonateInfo.wTodaysCoinDoorsTributeTimes = 0;
        stDonateInfo.byTodaysCoinDoorsTributeEncourage = 0;//奖励清零
        stDonateInfo.qwLastCoinDoorsTributeTime = SGDP::SDTimeSecs();
    }

    if (!IsToday(stDonateInfo.qwLastGoldDoorsTributeTime))
    {
        stDonateInfo.wTodaysGoldDoorsTributeTimes = 0;
        stDonateInfo.qwLastGoldDoorsTributeTime = SGDP::SDTimeSecs();
    }

    DT_FACTION_LOG_DATA stFactionLogData;
    memset(&stFactionLogData, 0, sizeof(stFactionLogData));
    stFactionLogData.dwPlayerID1 = poPlayer->GetID();
    stFactionLogData.byPlayerJobType1 = stDT_FACTION_PLAYER_DATA.byJobType;

    //铜钱
    if (0 == byDoorsTributeFlag)
    {
        if ( CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(dwFactionLevel) <= stDonateInfo.wTodaysCoinDoorsTributeTimes)
        {
            return ERR_DOORS_TRIBUTE_FACTION::ID_COIN_NOT_TIME;
        }
        if (poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin < CLuamgr::Instance()->GetCostCoinByDonate(dwFactionLevel))
        {
            return ERR_DOORS_TRIBUTE_FACTION::ID_COIN_NOT_ENOUGH;
        }
        poPlayer->DecCoin(CLuamgr::Instance()->GetCostCoinByDonate(dwFactionLevel), CRecordMgr::EDCT_FACTION_DOORSTRIBUTE);

        stDonateInfo.wTodaysCoinDoorsTributeTimes++;
        //加门贡
        AddDoorsTribute(poPlayer->GetID(), CLuamgr::Instance()->GetDoorsTributeByDonateCoin(dwFactionLevel), CRecordMgr::EAT_DONATE_COIN);
        //加资金
        AddFactionFunds(CLuamgr::Instance()->GetFundsByDonateCoin(dwFactionLevel), CRecordMgr::EAFF_DONATE_COIN);
        //兑换门贡值
        if (stDonateInfo.wTodaysCoinDoorsTributeTimes == CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(dwFactionLevel) && 0 == stDonateInfo.byTodaysCoinDoorsTributeEncourage)
        {
            stDonateInfo.byTodaysCoinDoorsTributeEncourage = 1;//可领取
        }

        stDonateInfo.qwLastCoinDoorsTributeTime = SGDP::SDTimeSecs();
        stFactionLogData.byAction = EFLID_DonateCoin;
        stFactionLogData.adwParamList[0] = CLuamgr::Instance()->GetCostCoinByDonate(dwFactionLevel);
        stFactionLogData.adwParamList[1] = CLuamgr::Instance()->GetFundsByDonateCoin(dwFactionLevel);

        stFactionLogData.qwLogTime = SGDP::SDTimeSecs();

        AddFactionLogData(stFactionLogData);
		if (CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(dwFactionLevel) <= stDonateInfo.wTodaysCoinDoorsTributeTimes)
		{
			PKT_CLIGS_HOME_BUILD_PROMPT_NTF stNtf;
			memset(&stNtf, 0, sizeof(PKT_CLIGS_HOME_BUILD_PROMPT_NTF));
			poPlayer->GetFactionPrompt(stNtf.stPromptInfo.astBuildPromptInfo[stNtf.stPromptInfo.byBuildNum++]);
			poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_HOME_BUILD_PROMPT_NTF);

			PKT_CLIGS_FACTION_PROMPT_NTF stFactionNtf;
			memset(&stFactionNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
			poFactionPlayer->GetFactionPrompt(stFactionNtf.stFactionPrompt);
			poPlayer->SendMsg((TCHAR*)&stFactionNtf, CLIGS_FACTION_PROMPT_NTF);
		}
    }
    else
    {
        UINT32 dwTest = CFactorPropMgr::Instance()->GetGoldMaxTimeEveryDay(dwVipLevel);
        if (CFactorPropMgr::Instance()->GetGoldMaxTimeEveryDay(dwVipLevel) <= stDonateInfo.wTodaysGoldDoorsTributeTimes)
        {
            UINT8 byMaxVIPLevel = CVipPropMgr::Instance()->GetMaxVipLevel();
            if ( byMaxVIPLevel <= dwVipLevel)
            {
                return ERR_DOORS_TRIBUTE_FACTION::ID_GOLD_NOT_TIME;
            }
            else
            {
                return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
            }

        }
        if (poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < CLuamgr::Instance()->GetCostGoldByDonate(dwFactionLevel))
        {
            return ERR_DOORS_TRIBUTE_FACTION::ID_GOLD_NOT_ENOUGH;
        }
        poPlayer->DecGold(CLuamgr::Instance()->GetCostGoldByDonate(dwFactionLevel), CRecordMgr::EDGT_FACTION_DOORSTRIBUTE);

        stDonateInfo.wTodaysGoldDoorsTributeTimes++;
        //加门贡
        AddDoorsTribute(poPlayer->GetID(), CLuamgr::Instance()->GetDoorsTributeByDonateGold(dwFactionLevel), CRecordMgr::EAT_DONATE_GOLD);
        //加资金
        AddFactionFunds(CLuamgr::Instance()->GetFundsByDonateGold(dwFactionLevel), CRecordMgr::EAFF_DONATE_COIN);
        //锐换门贡值

        stDonateInfo.qwLastGoldDoorsTributeTime = SGDP::SDTimeSecs();

        stFactionLogData.byAction = EFLID_DonateGold;
        stFactionLogData.adwParamList[0] = CLuamgr::Instance()->GetCostGoldByDonate(dwFactionLevel);
        stFactionLogData.adwParamList[1] = CLuamgr::Instance()->GetFundsByDonateGold(dwFactionLevel);
        stFactionLogData.qwLogTime = SGDP::SDTimeSecs();

        AddFactionLogData(stFactionLogData);
    }

    stDT_DOORS_TRIBUTE_TODAY_DATA.wCoinTodaysDoorsTributeTimes = stDonateInfo.wTodaysCoinDoorsTributeTimes;
    stDT_DOORS_TRIBUTE_TODAY_DATA.byCoinRecvFlag = stDonateInfo.byTodaysCoinDoorsTributeEncourage;
    stDT_DOORS_TRIBUTE_TODAY_DATA.wGoldTodaysDoorsTributeTimes = stDonateInfo.wTodaysGoldDoorsTributeTimes;
    stDT_DOORS_TRIBUTE_TODAY_DATA.byGoldRecvFlag = 0;

    return ERR_DOORS_TRIBUTE_FACTION::ID_SUCCESS;
}


//领取奖励
UINT16 CFaction::RecvDoorsTributeFactionReq( CPlayer * poPlayer, PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK& stAck )
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(poPlayer->GetID());
    if(itr == m_mapFactionPlayerData.end())
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_PLAYER_NOT_HAVE_FACTION;
    }
    DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = itr->second;
	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL == poFactionPlayer)
	{
		return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_PLAYER_NOT_HAVE_FACTION;
	}
	DT_PLAYER_DONATE_INFO& stDonate = poFactionPlayer->GetFactionDonate();
    if ( stDonate.wTodaysCoinDoorsTributeTimes < CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(m_stDT_FACTION_BASE_DATA.dwFactionLevel) )
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_CONDITION_NOT_ENOUGH;
    }
    if( 2 == stDonate.byTodaysCoinDoorsTributeEncourage)
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_REWARD_IS_GET;
    }
    if ( 1 != stDonate.byTodaysCoinDoorsTributeEncourage)
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_GET_REWARD_NOT_ENOUGH;
    }
    //检测背包是否满了
    vector<UINT16> vecItemID;

    const CMapDonateReward& mapReward = CFactorPropMgr::Instance()->GetDonateReward();
    pair<CMapDonateRewardItr, CMapDonateRewardItr> pairItr = mapReward.equal_range(m_stDT_FACTION_BASE_DATA.dwFactionLevel);
    CMapDonateRewardItr mapItr;
    for (mapItr = pairItr.first; mapItr != pairItr.second; mapItr++)
    {
        if (ACTE_GOOD == mapItr->second.byRewardType)
        {
            vecItemID.push_back(mapItr->second.dwRewardValue);
        }
    }

    if(CItemMgr::Instance()->CkBagFull(poPlayer, vecItemID))
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_BAG_FULL;
    }

    CItem* poItem	 = NULL;
    ECreateItemRet ItemRet = ECIR_SUCCESS;
    for (mapItr = pairItr.first; mapItr != pairItr.second; mapItr++)
    {
        switch(mapItr->second.byRewardType)
        {
        case	ACTE_GOOD://物品
        {
            ItemRet = ECIR_SUCCESS;
            poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, mapItr->second.dwRewardValue, mapItr->second.byNum, ItemRet, 0, CRecordMgr::EAIT_RATING_SCORE);
            if(NULL == poItem)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CreateItem failed[%d], ItemKindID=%d !"), MSG_MARK, ItemRet, mapItr->second.dwRewardValue);
                return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_GOODS_INFO_EXCEPTION;
            }
        }
        break;
        default:
            break;
        }
    }
    stDonate.byTodaysCoinDoorsTributeEncourage = 2;//已领取过
    return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_SUCCESS;
}


//增加日志
void CFaction::AddFactionLogData(DT_FACTION_LOG_DATA stDT_FACTION_LOG_DATA)
{
	CSDDateTime oDataTime(stDT_FACTION_LOG_DATA.qwLogTime);
	oDataTime.IncHour( 8 );
	stDT_FACTION_LOG_DATA.qwLogTime = oDataTime.GetTimeValue();
    if (m_dequeFactionLogData.size() >= MAX_FACTION_LOG_NUM)
    {
        m_dequeFactionLogData.pop_back();
    }
    m_dequeFactionLogData.push_front(stDT_FACTION_LOG_DATA);
}

//查看玩家是否在成功加入
BOOL	CFaction::CkFactionPlayerData(UINT32 dwJoinPlayerID)
{
    if (m_mapFactionPlayerData.find(dwJoinPlayerID) != m_mapFactionPlayerData.end())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//查看玩家是否在正在早请加入
BOOL	CFaction::CkFactionApplyPlayerData(UINT32 dwJoinPlayerID)
{
    if (m_mapApplyFactionPlayerID.find(dwJoinPlayerID) != m_mapApplyFactionPlayerID.end())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//查看玩家职业个数
UINT16	CFaction::GetJobNum(em_Faction_Job ckjob)
{
    UINT16 wNum = 0;
    CMapFactionPlayerDataItr itr;
    for (itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
    {
        DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = itr->second;
        if(stDT_FACTION_PLAYER_DATA.byJobType == ckjob)
        {
            wNum++;
        }
    }
    return wNum;
}

UINT16	CFaction::JobFaction(UINT32 dwJoinPlayerID, UINT32 dwJobPlayerID, UINT8 byJobFlag )
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwJoinPlayerID);
    if (itr == m_mapFactionPlayerData.end())
    {
        return ERR_JOB_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    const DT_FACTION_PLAYER_DATA& stDT_FACTION_PLAYER_DATA = itr->second;
    if (byJobFlag)
    {
        if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stDT_FACTION_PLAYER_DATA.byJobType, EFA_Promotion))
        {
            return ERR_JOB_FACTION::ID_PLAYER_NOT_AUTHOR;
        }
    }
    else
    {
        if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stDT_FACTION_PLAYER_DATA.byJobType, EFA_Demotion))
        {
            return ERR_JOB_FACTION::ID_PLAYER_NOT_AUTHOR;
        }
    }


    //升职者信息
    CMapFactionPlayerDataItr joinitr = m_mapFactionPlayerData.find(dwJobPlayerID);
    if (joinitr == m_mapFactionPlayerData.end())
    {
        return ERR_JOB_FACTION::ID_JOIN_PLAYER_NOT_IN_FACTION;
    }
    DT_FACTION_PLAYER_DATA& stJoinDT_FACTION_PLAYER_DATA = joinitr->second;
    if (em_Faction_MartialHead == stJoinDT_FACTION_PLAYER_DATA.byJobType)
    {
        return ERR_JOB_FACTION::ID_JOIN_PLAYER_JOB_LIMIT;
    }


    //记入日志
    DT_FACTION_LOG_DATA stFactionLogData;
    memset(&stFactionLogData, 0, sizeof(DT_FACTION_LOG_DATA));
    //获取下阶职位
    em_Faction_Job emNextJob = em_Faction_Normal;
    if (1 == byJobFlag)
    {
        if (em_Faction_ProxyMartialHead == stJoinDT_FACTION_PLAYER_DATA.byJobType)
        {
            return ERR_JOB_FACTION::ID_JOB_IS_MAX;
        }
        emNextJob = CFactorAuthorityPropMgr::Instance()->UpJob((em_Faction_Job)stJoinDT_FACTION_PLAYER_DATA.byJobType);
        stFactionLogData.byAction = EFLID_Promotion;
    }
    else
    {

        if (em_Faction_Normal == stJoinDT_FACTION_PLAYER_DATA.byJobType)
        {
            return ERR_JOB_FACTION::ID_JOB_IS_MIN;
        }
        emNextJob = CFactorAuthorityPropMgr::Instance()->DownJob((em_Faction_Job)stJoinDT_FACTION_PLAYER_DATA.byJobType);
        stFactionLogData.byAction = EFLID_Demotion;
    }

    //获取相同职业个数
    UINT16 wJobNum = GetJobNum(emNextJob);
    if(!CFactorAuthorityPropMgr::Instance()->CkJobNum(emNextJob, wJobNum, m_stDT_FACTION_BASE_DATA.dwFactionLevel))
    {
        return ERR_JOB_FACTION::ID_JOIN_PLAYER_JOB_LIMIT;
    }

    stFactionLogData.byPlayerJobType1 = stDT_FACTION_PLAYER_DATA.byJobType;
    stFactionLogData.byPlayerJobType2 = stJoinDT_FACTION_PLAYER_DATA.byJobType;
    stFactionLogData.dwPlayerID1 = dwJoinPlayerID;
    stFactionLogData.dwPlayerID2 = dwJobPlayerID;
    stFactionLogData.adwParamList[0] = emNextJob;
    stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
    AddFactionLogData(stFactionLogData);


    //重设置职业
    stJoinDT_FACTION_PLAYER_DATA.byJobType = emNextJob;
    SetPlayerChangeToAll();
    return ERR_JOB_FACTION::ID_SUCCESS;
}

UINT16	CFaction::SetNoticeFaction(UINT32 dwJoinPlayerID, UINT8 bySetFlag, TCHAR aszFactionNotice[MAX_FACTION_DESC_NUM])
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwJoinPlayerID);
    if (itr == m_mapFactionPlayerData.end())
    {
        return ERR_SET_NOTICE_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    DT_FACTION_PLAYER_DATA stDT_FACTION_PLAYER_DATA = itr->second;
    if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stDT_FACTION_PLAYER_DATA.byJobType, EFA_EditSetting))
    {
        return ERR_SET_NOTICE_FACTION::ID_PLAYER_NOT_AUTHOR;
    }
	
    if (bySetFlag)
    {
        SDStrncpy(m_stDT_FACTION_BASE_DATA.aszFactionNotice, aszFactionNotice, MAX_FACTION_DESC_NUM - 1);
    }
    else
    {
        SDStrncpy(m_stDT_FACTION_BASE_DATA.aszFactionAnnouncement, aszFactionNotice, MAX_FACTION_DESC_NUM - 1);
    }
    return ERR_SET_NOTICE_FACTION::ID_SUCCESS;
}

UINT16	CFaction::DissolveFaction( UINT32 dwJoinPlayerID )
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwJoinPlayerID);
    if (itr == m_mapFactionPlayerData.end())
    {
        return ERR_DISSOLVE_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    //////DT_FACTION_PLAYER_DATA stDT_FACTION_PLAYER_DATA = itr->second;

    //删除全局数据
    for (itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end();)
    {
        s_mapFactionPlayerID.erase(itr->second.dwJoinPlayerID);
    }

    //删除全局数据
    Range   range = s_multimapApplyFactionPlayer.equal_range(dwJoinPlayerID);
    for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second;)
    {
        if (itr->second == m_stDT_FACTION_BASE_DATA.dwFactionID)
        {
            s_multimapApplyFactionPlayer.erase(itr++);
        }
    }
    return ERR_SET_NOTICE_FACTION::ID_SUCCESS;
}

em_Faction_Job CFaction::GetJob(UINT32 dwPlayerID)
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwPlayerID);
    if (itr == m_mapFactionPlayerData.end())
    {
        return em_Faction_Other;
    }

    DT_FACTION_PLAYER_DATA &stPlayerData = itr->second;
    return (em_Faction_Job)stPlayerData.byJobType;
}

//审核玩家信息
UINT16	CFaction::AuditFaction( UINT32 dwJoinPlayerID, UINT8 byAuditFlag, UINT32 dwPlayerID, vector<UINT32> &vec, UINT32& dwPassNum, UINT32& dwNoPass)
{
	if (!IsToday(m_qwLastAuditTime))
	{
		m_qwLastAuditTime = SGDP::SDTimeSecs();//隔天重置时间
		m_byTodayPassNum = 0;
	}
   
    DT_FACTION_LOG_DATA stFactionLogData;
    memset(&stFactionLogData, 0, sizeof(DT_FACTION_LOG_DATA));
    if (byAuditFlag)
    {
		DT_FACTION_PLAYER_DATA stJoin_DT_FACTION_PLAYER_DATA; //帮派数据信息
		memset(&stJoin_DT_FACTION_PLAYER_DATA, 0x00, sizeof(stJoin_DT_FACTION_PLAYER_DATA));
		stJoin_DT_FACTION_PLAYER_DATA.dwJoinPlayerID	=	dwPlayerID;
		stJoin_DT_FACTION_PLAYER_DATA.byJobType		=	em_Faction_Normal;
		stJoin_DT_FACTION_PLAYER_DATA.qwJoinTime = SGDP::SDTimeSecs();
		//审核通过
        m_mapFactionPlayerData[stJoin_DT_FACTION_PLAYER_DATA.dwJoinPlayerID] = stJoin_DT_FACTION_PLAYER_DATA;
        s_mapFactionPlayerID[stJoin_DT_FACTION_PLAYER_DATA.dwJoinPlayerID] = m_stDT_FACTION_BASE_DATA.dwFactionID;
		m_byTodayPassNum += 1;//今日审核人数加1

        Range   range = s_multimapApplyFactionPlayer.equal_range(dwPlayerID);
        for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second;)
        {
            vec.push_back(itr->second);
            s_multimapApplyFactionPlayer.erase(itr++);
        }
        CMapFactionPlayerIDItr applyItr = m_mapApplyFactionPlayerID.find(dwPlayerID);
        if (applyItr != m_mapApplyFactionPlayerID.end())
        {
            m_mapApplyFactionPlayerID.erase(dwPlayerID);
        }

        dwPassNum++;
        stFactionLogData.byPlayerJobType1 = em_Faction_Other;
        stFactionLogData.byPlayerJobType2 = 0;
        stFactionLogData.dwPlayerID1 = dwPlayerID;
        stFactionLogData.dwPlayerID2 = 0;
        stFactionLogData.byAction = EFLID_JoinFaction;
        stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
        AddFactionLogData(stFactionLogData);


        DT_PLAYER_FACTION_BATTLELOG_DATA stLogData;
        stLogData.byFactionLogType = EFBT_AddFaction;
        stLogData.byNewFlag = ESM_YES;
        SDStrncpy(stLogData.aszDescMsg, m_stDT_FACTION_BASE_DATA.aszFactionName, NORMAL_MSG_LEN - 1);
        stLogData.qwLogTime = SGDP::SDTimeSecs();

        CPlayer* poAddPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if ( NULL == poAddPlayer)
        {
            CMapFactionPlayerDataItr addItr = m_mapFactionPlayerData.find(dwPlayerID);
            if ( addItr != m_mapFactionPlayerData.end())
            {
                if (addItr->second.stUnLoginBattleLogList.byLogNum < MAX_FACTION_BATTLE_LOG_NUM)
                {
                    addItr->second.stUnLoginBattleLogList.astPlayerFactionLog[addItr->second.stUnLoginBattleLogList.byLogNum++] = stLogData;
                }
            }
        }
		else
		{
			CFactionPlayer* poFactionPlayer = poAddPlayer->GetFactionPlayer();
			if (NULL != poFactionPlayer)
			{
				poFactionPlayer->AddFactionBattleLog(stLogData);
			}
			PKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF stNtf;
			stNtf.byUnReadLogNum = poFactionPlayer->GetUnReadFactionBattleLogNum();
			stNtf.byLastOperateType = EFBT_AddFaction;
			poAddPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_PLAYER_FACTION_BATTLELOG_NTF);

			if (NULL != poFactionPlayer)
			{
				PKT_CLIGS_HOME_BUILD_PROMPT_NTF stPromptNtf;
				memset(&stPromptNtf, 0, sizeof(PKT_CLIGS_HOME_BUILD_PROMPT_NTF));
				poAddPlayer->GetFactionPrompt(stPromptNtf.stPromptInfo.astBuildPromptInfo[stPromptNtf.stPromptInfo.byBuildNum]);
				stPromptNtf.stPromptInfo.byBuildNum++;
				poAddPlayer->SendMsg((TCHAR*)&stPromptNtf, CLIGS_HOME_BUILD_PROMPT_NTF);
			}
		}
    }
    else
    {
        //删除全局数据
        Range   range = s_multimapApplyFactionPlayer.equal_range(dwPlayerID);
        for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second; itr++)
        {
            if (itr->second == m_stDT_FACTION_BASE_DATA.dwFactionID)
            {
                s_multimapApplyFactionPlayer.erase(itr);
                break;
            }
        }
        CMapFactionPlayerIDItr applyItr = m_mapApplyFactionPlayerID.find(dwPlayerID);
        if (applyItr != m_mapApplyFactionPlayerID.end())
        {
            m_mapApplyFactionPlayerID.erase(dwPlayerID);
        }
        dwNoPass++;
    }

    return ERR_AUDIT_FACTION::ID_SUCCESS;
}

UINT16	CFaction::AuditFaction( UINT8 byAuditFlag, UINT32 dwPlayerID, vector<UINT32> &vec)
{
	if (!IsToday(m_qwLastAuditTime))
	{
		m_qwLastAuditTime = SGDP::SDTimeSecs();//隔天重置时间
		m_byTodayPassNum = 0;
	}

	UINT16 wErrCode = CKAuditFaction(byAuditFlag, dwPlayerID);
	if ( ERR_AUDIT_FACTION::ID_SUCCESS != wErrCode)
	{
		return wErrCode;
	}

	DT_FACTION_LOG_DATA stFactionLogData;
	memset(&stFactionLogData, 0, sizeof(DT_FACTION_LOG_DATA));
	if (byAuditFlag)
	{
		DT_FACTION_PLAYER_DATA stJoin_DT_FACTION_PLAYER_DATA; //帮派数据信息
		memset(&stJoin_DT_FACTION_PLAYER_DATA, 0x00, sizeof(stJoin_DT_FACTION_PLAYER_DATA));
		stJoin_DT_FACTION_PLAYER_DATA.dwJoinPlayerID	=	dwPlayerID;
		stJoin_DT_FACTION_PLAYER_DATA.byJobType		=	em_Faction_Normal;
		stJoin_DT_FACTION_PLAYER_DATA.qwJoinTime = SGDP::SDTimeSecs();
		//审核通过
		m_mapFactionPlayerData[stJoin_DT_FACTION_PLAYER_DATA.dwJoinPlayerID] = stJoin_DT_FACTION_PLAYER_DATA;
		s_mapFactionPlayerID[stJoin_DT_FACTION_PLAYER_DATA.dwJoinPlayerID] = m_stDT_FACTION_BASE_DATA.dwFactionID;
		m_byTodayPassNum += 1;//今日审核人数加1

		Range   range = s_multimapApplyFactionPlayer.equal_range(dwPlayerID);
		for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second;)
		{
			vec.push_back(itr->second);
			s_multimapApplyFactionPlayer.erase(itr++);
		}
		CMapFactionPlayerIDItr applyItr = m_mapApplyFactionPlayerID.find(dwPlayerID);
		if (applyItr != m_mapApplyFactionPlayerID.end())
		{
			m_mapApplyFactionPlayerID.erase(dwPlayerID);
		}

		stFactionLogData.byPlayerJobType1 = em_Faction_Other;
		stFactionLogData.byPlayerJobType2 = 0;
		stFactionLogData.dwPlayerID1 = dwPlayerID;
		stFactionLogData.dwPlayerID2 = 0;
		stFactionLogData.byAction = EFLID_JoinFaction;
		stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
		AddFactionLogData(stFactionLogData);
		SetPlayerChangeToAll();

		DT_PLAYER_FACTION_BATTLELOG_DATA stLogData;
		stLogData.byFactionLogType = EFBT_AddFaction;
		stLogData.byNewFlag = ESM_YES;
		SDStrncpy(stLogData.aszDescMsg, m_stDT_FACTION_BASE_DATA.aszFactionName, NORMAL_MSG_LEN - 1);
		stLogData.qwLogTime = SGDP::SDTimeSecs();

		CPlayer* poAddPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
		if ( NULL == poAddPlayer)
		{
			CMapFactionPlayerDataItr addItr = m_mapFactionPlayerData.find(dwPlayerID);
			if ( addItr != m_mapFactionPlayerData.end())
			{
				if (addItr->second.stUnLoginBattleLogList.byLogNum < MAX_FACTION_BATTLE_LOG_NUM)
				{
					addItr->second.stUnLoginBattleLogList.astPlayerFactionLog[addItr->second.stUnLoginBattleLogList.byLogNum++] = stLogData;
				}
			}
		}
		else
		{
			CFactionPlayer* poFactionPlayer = poAddPlayer->GetFactionPlayer();
			if (NULL != poFactionPlayer)
			{
				poFactionPlayer->AddFactionBattleLog(stLogData);
			}
			PKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF stNtf;
			stNtf.byUnReadLogNum = poFactionPlayer->GetUnReadFactionBattleLogNum();
			stNtf.byLastOperateType = EFBT_AddFaction;
			poAddPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_PLAYER_FACTION_BATTLELOG_NTF);

			if (NULL != poFactionPlayer)
			{
				PKT_CLIGS_HOME_BUILD_PROMPT_NTF stPromptNtf;
				memset(&stPromptNtf, 0, sizeof(PKT_CLIGS_HOME_BUILD_PROMPT_NTF));
				poAddPlayer->GetFactionPrompt(stPromptNtf.stPromptInfo.astBuildPromptInfo[stPromptNtf.stPromptInfo.byBuildNum]);
				stPromptNtf.stPromptInfo.byBuildNum++;
				poAddPlayer->SendMsg((TCHAR*)&stPromptNtf, CLIGS_HOME_BUILD_PROMPT_NTF);
			}
		}
	}
	else
	{
		//删除全局数据
		Range   range = s_multimapApplyFactionPlayer.equal_range(dwPlayerID);
		for(CMultiMapApplyFactionPlayerItr itr = range.first; itr != range.second; itr++)
		{
			if (itr->second == m_stDT_FACTION_BASE_DATA.dwFactionID)
			{
				s_multimapApplyFactionPlayer.erase(itr);
				break;
			}
		}
		CMapFactionPlayerIDItr applyItr = m_mapApplyFactionPlayerID.find(dwPlayerID);
		if (applyItr != m_mapApplyFactionPlayerID.end())
		{
			m_mapApplyFactionPlayerID.erase(dwPlayerID);
		}
	}

	return ERR_AUDIT_FACTION::ID_SUCCESS;
}

UINT16 CFaction::CKAuditFaction( UINT8 byAuditFlag, UINT32 dwPlayerID )
{
	if (!IsToday(m_qwLastAuditTime))
	{
		m_qwLastAuditTime = SGDP::SDTimeSecs();//隔天重置时间
		m_byTodayPassNum = 0;
	}

	if (byAuditFlag)
	{
		CMapFactionPlayerIDItr applyItr = m_mapApplyFactionPlayerID.find(dwPlayerID);
		if (applyItr == m_mapApplyFactionPlayerID.end())
		{
			UINT32 dwFactionID = 0;
			if(CFaction::CkFactionPlayerData(dwFactionID, dwPlayerID))
			{
				return ERR_AUDIT_FACTION::ID_ALREADY_AUDIT_OTHER;//已被审核
			}
			else
			{
				return ERR_AUDIT_FACTION::ID_ALREAD_CANCEL_APPLY;
			}
		}
	}
	return ERR_AUDIT_FACTION::ID_SUCCESS;
}

UINT16	CFaction::GetDT_FACTION_LOG_DATA_CLI_LST(DT_FACTION_LOG_DATA_CLI_LST &stDT_FACTION_LOG_DATA_CLI_LST)
{
    CDequeFactionLogDataItr itr;
    memset(&stDT_FACTION_LOG_DATA_CLI_LST, 0x00, sizeof(DT_FACTION_LOG_DATA_CLI_LST));
    for (itr = m_dequeFactionLogData.begin(); itr != m_dequeFactionLogData.end() && stDT_FACTION_LOG_DATA_CLI_LST.wFactionLogNum < DT_FACTION_PLAYER_NUM; itr++)
    {
        DT_FACTION_LOG_DATA &stDT_FACTION_LOG_DATA = *itr;

        DT_FACTION_LOG_DATA_CLI &stDT_FACTION_LOG_DATA_CLI = stDT_FACTION_LOG_DATA_CLI_LST.astFactionLogDataCli[stDT_FACTION_LOG_DATA_CLI_LST.wFactionLogNum++];
        const SPlayerBaseData* poPlayerBaseData = NULL;
        if ( 0 != stDT_FACTION_LOG_DATA.dwPlayerID1 )
        {
            poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stDT_FACTION_LOG_DATA.dwPlayerID1);
            if( NULL != poPlayerBaseData )
            {
                SDStrcpy( stDT_FACTION_LOG_DATA_CLI.aszDispName1, poPlayerBaseData->aszDispName );
            }
        }

        poPlayerBaseData = NULL;
        if ( 0 != stDT_FACTION_LOG_DATA.dwPlayerID2)
        {
            poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stDT_FACTION_LOG_DATA.dwPlayerID2);
            if( NULL != poPlayerBaseData )
            {
                SDStrcpy( stDT_FACTION_LOG_DATA_CLI.aszDispName2, poPlayerBaseData->aszDispName );
            }
        }
        stDT_FACTION_LOG_DATA_CLI.byJobType1 = stDT_FACTION_LOG_DATA.byPlayerJobType1;
        stDT_FACTION_LOG_DATA_CLI.byJobType2 = stDT_FACTION_LOG_DATA.byPlayerJobType2;
        stDT_FACTION_LOG_DATA_CLI.byAction = stDT_FACTION_LOG_DATA.byAction;
        stDT_FACTION_LOG_DATA_CLI.qwLogTime = stDT_FACTION_LOG_DATA.qwLogTime;
        memcpy(stDT_FACTION_LOG_DATA_CLI.adwParamList, stDT_FACTION_LOG_DATA.adwParamList, sizeof(stDT_FACTION_LOG_DATA.adwParamList));

    }
    return ERR_OPEN_LOG_FACTION::ID_SUCCESS;
}

VOID CFaction::GetDT_FACTION_COMMON_DATA(DT_FACTION_COMMON_DATA& stDT_FACTION_COMMON_DATA)
{
    memset(&stDT_FACTION_COMMON_DATA, 0, sizeof(DT_FACTION_COMMON_DATA));

    memcpy(&stDT_FACTION_COMMON_DATA, &m_stDT_FACTION_COMMON_DATA, sizeof(DT_FACTION_COMMON_DATA));
}
DT_FACTION_COMMON_DATA& CFaction::GetDT_FACTION_COMMON_DATA()
{
    return m_stDT_FACTION_COMMON_DATA;
}

//VOID CFaction::Init(DT_FACTION_COMMON_DATA& stDT_FACTION_COMMON_DATA)
//{
//    memset(&m_stDT_FACTION_COMMON_DATA, 0, sizeof(DT_FACTION_COMMON_DATA));
//    memcpy(&m_stDT_FACTION_COMMON_DATA, &stDT_FACTION_COMMON_DATA, sizeof(DT_FACTION_COMMON_DATA));
//
//    if (ESM_YES == m_stDT_FACTION_COMMON_DATA.stHotSpringData.byGiveWineOpenState)
//    {
//        SGiveWineProp stGiveWineProp = {0};
//        CHotSpringPropMgr::Instance()->GetGiveWinePropByFactionLevel(m_stDT_FACTION_BASE_DATA.dwFactionLevel, stGiveWineProp);
//        UINT32 dwKeepTime = stGiveWineProp.dwKeepTime;
//        if ( dwKeepTime < SGDP::SDTimeSecs() - m_stDT_FACTION_COMMON_DATA.stHotSpringData.qwGiveWineOpenTime )
//        {
//            //延迟1秒
//            CTimerEventMgr::Instance()->AddGiveWineEvent(m_stDT_FACTION_BASE_DATA.dwFactionID, 1);
//        }
//        else
//        {
//            UINT64 qwTime = dwKeepTime + m_stDT_FACTION_COMMON_DATA.stHotSpringData.qwGiveWineOpenTime - SGDP::SDTimeSecs();
//            CTimerEventMgr::Instance()->AddGiveWineEvent(m_stDT_FACTION_BASE_DATA.dwFactionID, static_cast<UINT32>(qwTime));
//        }
//    }
//    //m_bExceptionFlag = bExceptionFlag;
//    return ;
//}


UINT32 CFaction::GetDoorsTribute(UINT32 dwPlayerID)
{
	CMapFactionPlayerDataItr it = m_mapFactionPlayerData.find(dwPlayerID);
	if (it == m_mapFactionPlayerData.end())
	{
		return 0;
	}
	return it->second.dwDoorsTribute;
}

UINT64 CFaction::GetSumDoorsTribute(UINT32 dwPlayerID)
{
	CMapFactionPlayerDataItr it = m_mapFactionPlayerData.find(dwPlayerID);
	if (it == m_mapFactionPlayerData.end())
	{
		return 0;
	}
	return it->second.qwDoorsTributeSum;
}

BOOL CFaction::DeductDoorsTribute(UINT32 dwPlayerID, UINT32 dwDoorsTribute, UINT16 wOpType)
{
    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
    if (NULL == pstBaseData)
    {
        return FALSE;//无玩家数据， 废数据
    }

    CMapFactionPlayerDataItr it = m_mapFactionPlayerData.find(dwPlayerID);
    if (it == m_mapFactionPlayerData.end())
    {
        return FALSE;
    }
    if (it->second.dwDoorsTribute < dwDoorsTribute)
    {
        return FALSE;
    }
    it->second.dwDoorsTribute -= dwDoorsTribute;

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL != poPlayer)
	{
		poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA().qwDecDoorsTribute += dwDoorsTribute;
		poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAfterDoorsTribute = it->second.dwDoorsTribute;
	}
    //记入日志
    CRecordMgr::Instance()->RecordInfo( dwPlayerID, ERM_DECDOORSTRIBUTE, wOpType, dwDoorsTribute, it->second.dwDoorsTribute, pstBaseData->wLevel, pstBaseData->byVipLevel);
    if (!IsCanBuyRareItem(dwPlayerID))
    {
        PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
        memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if (NULL != poPlayer)
        {
            CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
            if ( NULL != poFactionPlayer)
            {
                CRareItemRoom& poRareItem = poFactionPlayer->GetRareItem();

                poRareItem.GetPrompt(stNtf.stFactionPrompt);
            }
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }

    if (!IsCanLearnSkill(dwPlayerID))
    {
        PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
        memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if (NULL != poPlayer)
        {
            CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
            if ( NULL != poFactionPlayer)
            {
                CGymnasium& oGymnasium = poFactionPlayer->GetGymnasium();
                oGymnasium.GetPrompt(stNtf.stFactionPrompt);
            }
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }
    return TRUE;
}

VOID CFaction::AddDoorsTribute(UINT32 dwPlayerID, UINT32 dwDoorsTribute, UINT16 wOpType,  UINT64 qwParam1 , UINT64 qwParam2 , UINT64 qwParam3, UINT64 qwParam4)
{
    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
    if (NULL == pstBaseData)
    {
        return ;//无玩家数据， 废数据
    }
    CMapFactionPlayerDataItr it = m_mapFactionPlayerData.find(dwPlayerID);
    if (it == m_mapFactionPlayerData.end())
    {
        return ;
    }
    it->second.dwDoorsTribute += dwDoorsTribute;
    it->second.qwDoorsTributeSum += dwDoorsTribute;

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL != poPlayer)
	{
		poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAddDoorsTribute += dwDoorsTribute;
		poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAfterDoorsTribute = it->second.dwDoorsTribute;
	}
	
    //记入日志
    CRecordMgr::Instance()->RecordInfo( dwPlayerID, ERM_ADDDOORSTRIBUTE, wOpType, dwDoorsTribute, it->second.dwDoorsTribute, pstBaseData->wLevel, pstBaseData->byVipLevel,
		qwParam1, qwParam2, qwParam3, qwParam4);

    if (IsCanBuyRareItem(dwPlayerID))
    {
        PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
        memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if (NULL != poPlayer)
        {
            CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
            if ( NULL != poFactionPlayer)
            {
                CRareItemRoom& poRareItem = poFactionPlayer->GetRareItem();
                poRareItem.GetPrompt(stNtf.stFactionPrompt);
            }
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }

    if (IsCanLearnSkill(dwPlayerID))
    {
        PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
        memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if (NULL != poPlayer)
        {
            CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
            if ( NULL != poFactionPlayer)
            {
                CGymnasium& oGymnasium = poFactionPlayer->GetGymnasium();
                oGymnasium.GetPrompt(stNtf.stFactionPrompt);
            }
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }
}

//增加门派捉妖任务积分
VOID CFaction::AddHuntingIntegral(UINT32 dwPlayerID, UINT32 dwValue)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if ( NULL == poPlayer)
    {
        return;
    }
    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if ( NULL == poFactionPlayer)
    {
        return ;
    }
    CShade& oShade = poFactionPlayer->GetShade();
    if (m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral <= m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral + dwValue)
    {
        if ( m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCollectiveTaskSchedule <= m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCurCollectiveTaskSchedule)
        {
            if ( m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral != m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral)
            {
                oShade.SendRewardSchedule();
            }
            m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral = m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral;
        }
        else
        {
            oShade.SendRewardSchedule();
            m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCurCollectiveTaskSchedule += 1;
            m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral = m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral + dwValue - m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral;
            m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral = ResetMaxTaskIntegral();
            m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.qwLastChangeScheduleTime = SGDP::SDTimeMicroSec();
			while ( m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral < m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral)
			{
				
				if ( m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCollectiveTaskSchedule < m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCurCollectiveTaskSchedule )
				{
					m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCurCollectiveTaskSchedule = m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCollectiveTaskSchedule;
					m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral = ResetMaxTaskIntegral();
					m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral = m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral;
				}
				else
				{
					oShade.SendRewardSchedule();
					m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCurCollectiveTaskSchedule += 1;
					m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral = m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral - m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral;
					m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral = ResetMaxTaskIntegral();
					m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.qwLastChangeScheduleTime = SGDP::SDTimeMicroSec();
				}
				
			}
        }

    }
    else
    {
        m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwCurIntegral += dwValue;
    }
}

UINT32 CFaction::ResetMaxTaskIntegral()
{
    STaskRewardProp stTaskRewardProp = {0};
    CFactorPropMgr::Instance()->GetTaskRewardProp(m_stDT_FACTION_BASE_DATA.dwShadeLevel,
            m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.byCurCollectiveTaskSchedule,
            stTaskRewardProp);
    m_stDT_FACTION_COMMON_DATA.stHuntingTaskSchedule.dwMaxIntegral = stTaskRewardProp.dwMaxIntegral;
    return stTaskRewardProp.dwMaxIntegral;
}

VOID CFaction::AddFactionFunds(UINT32 dwValue, UINT16 wOpType)
{
	if( CGvGBuild::Instance()->IsMoneyGame(m_stDT_FACTION_BASE_DATA.dwFactionID) )
	{
		AddGvGFactionFunds(dwValue);
	}
    m_stDT_FACTION_BASE_DATA.dwFactionFunds += dwValue;
    m_stDT_FACTION_BASE_DATA.qwFactionFundsSum += dwValue;

    //记入日志
    CRecordMgr::Instance()->RecordInfo( m_stDT_FACTION_BASE_DATA.dwFactionID, ERM_ADDFACTIONFUNDS, wOpType, dwValue,
                                        m_stDT_FACTION_BASE_DATA.dwFactionFunds, m_stDT_FACTION_BASE_DATA.dwFactionLevel, 0);

    CFactionMgr::Instance()->FactionSort();
}

VOID CFaction::AddDoorsTributeToAll(UINT32 dwDoorsTribute)
{
    for (CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
    {
        itr->second.dwDoorsTribute += dwDoorsTribute;
    }
}

VOID CFaction::AddSetHotSpringPlayerID(UINT32 dwPlayerID)
{
    if ( m_setHotSpringPlayerID.find(dwPlayerID) == m_setHotSpringPlayerID.end())
    {
        m_setAddHotSpringPlayerID.insert(dwPlayerID);
        m_setHotSpringPlayerID.insert(dwPlayerID);
    }
}

VOID CFaction::DelSetHotSpringPlayerID(UINT32 dwPlayerID)
{
    if ( m_setHotSpringPlayerID.find(dwPlayerID) != m_setHotSpringPlayerID.end())
    {
        m_setHotSpringPlayerID.erase(dwPlayerID);
        m_setDelHotSpringPlayerID.insert(dwPlayerID);
    }
}

VOID CFaction::SetHotSpringPlayerList(DT_HOT_SPRING_PLAYER_DATA_CLI_LST& stPlayerInfo)
{
    stPlayerInfo.dwPlayerNum = m_setHotSpringPlayerID.size();
    UINT32 dwIndex = 0;
    for ( CHotSpringPlayerIDItr it = m_setHotSpringPlayerID.begin(); it != m_setHotSpringPlayerID.end(); it++)
    {
        if (dwIndex >= MAX_HOT_SPRING_SHOW_PLAYER_NUM)
        {
            break;
        }
        SetHotSpringPlayerSingle(stPlayerInfo.astHotSpringPlayerLst[dwIndex], *it);
        dwIndex++;
    }
}
VOID CFaction::SetHotSpringPlayerSingle(DT_HOT_SPRING_PLAYER_DATA_CLI& stPlayerInfo, UINT32 dwPlayerID)
{
    CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if ( NULL == pPlayer)
    {
        return ;
    }
    const DT_PLAYER_BASE_DATA& stPlayerBaseData = pPlayer->GetDT_PLAYER_BASE_DATA();
    const DT_HERO_BASE_DATA& stHeroBaseData = pPlayer->GetCoachHero()->GetDT_HERO_BASE_DATA();
    SDStrcpy( stPlayerInfo.aszDispName, stPlayerBaseData.aszDispName);
    stPlayerInfo.dwPlayerID = dwPlayerID;
    stPlayerInfo.wHeroKindID = stHeroBaseData.wKindID;
    stPlayerInfo.wLevel = stHeroBaseData.wLevel;
    DT_PLAYER_POS_INFO* pPos = pPlayer->GetPositionHotSpring();
    if (NULL == pPos)
    {
        return;
    }
    stPlayerInfo.wPosX = pPos->wPosX;
    stPlayerInfo.wPosY = pPos->wPosY;
    //酒3种酒
    CFactionPlayer* poFactionPlayer = pPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        return ;
    }
    stPlayerInfo.byTuhaoGoldMark = poFactionPlayer->GetDT_HOT_SPRING_DATA().stWineInfo.dwWineID == 3 ? ESM_YES : ESM_NO;
}

VOID CFaction::SendSynchInfo()
{
    //获取信息数据
    BOOL bRet = FALSE;
    if(!bRet)
    {
        return;
    }

    //删除玩家信息
    PKT_CLIGS_HOT_SPRING_POS_NTF stPosNtf;

    UINT32 dwMoveIndex = 0;
    UINT32 dwAddIndex = 0;
    UINT32 dwDelIndex = 0;
    UINT32 dwMaxPlayerNum = MAX_HOT_SPRING_SHOW_PLAYER_NUM;
    set<UINT32> delSet;

    //移动信息
    CHotSpringPlayerIDItr setIt = m_setHotSpringPlayerID.begin();
    for ( ; setIt != m_setHotSpringPlayerID.end(); setIt++)
    {
        CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(*setIt);
        if ( NULL == pPlayer)
        {
            continue;
        }

        if ( m_setAddHotSpringPlayerID.find(*setIt) == m_setAddHotSpringPlayerID.end())
        {
            DT_PLAYER_POS_INFO* pPos = pPlayer->GetPositionHotSpring();
            if (NULL == pPos)
            {
                return;
            }
            if( ESM_YES == pPos->byMoveState)
            {
                pPos->byMoveState = ESM_NO;
                SetHotSpringPlayerSingle(stPosNtf.astMovePlayerInfo[dwMoveIndex], *setIt);
                bRet = TRUE;
                dwMoveIndex++;
            }
        }
    }
    stPosNtf.byMovePlayerNum = dwMoveIndex;

    setIt = m_setDelHotSpringPlayerID.begin();
    for ( ; setIt != m_setDelHotSpringPlayerID.end(); setIt++)
    {
        SetHotSpringPlayerSingle(stPosNtf.astLeavePlayerInfo[dwDelIndex], *setIt);
        bRet = TRUE;
        dwDelIndex++;
    }
    stPosNtf.byLeavePlayerNum = dwDelIndex;

    //增加玩家信息
    stPosNtf.byEnterPalyerNum = m_setAddHotSpringPlayerID.size();
    setIt = m_setAddHotSpringPlayerID.begin();
    for ( ; setIt != m_setAddHotSpringPlayerID.end(); setIt++)
    {
        if ( dwDelIndex < dwAddIndex )
        {
            break;
        }
        SetHotSpringPlayerSingle(stPosNtf.astEnterPlayerInfo[dwAddIndex], *setIt);
        dwAddIndex++;
        bRet = TRUE;
    }


    if (FALSE == bRet)
    {
        return ;
    }
    m_setAddHotSpringPlayerID.clear();
    m_setDelHotSpringPlayerID.clear();
    stPosNtf.byEnterPalyerNum = dwAddIndex;
    stPosNtf.byLeavePlayerNum = dwDelIndex;
    stPosNtf.byMovePlayerNum = dwMoveIndex;
    setIt = delSet.begin();
    for ( ; setIt != delSet.end(); setIt++)
    {
        if( m_setHotSpringPlayerID.find(*setIt) != m_setHotSpringPlayerID.end())
        {
            m_setHotSpringPlayerID.erase(*setIt);
        }
    }
    delSet.clear();
    //遍历玩家发送同步数据  在线在温泉玩家
    CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin();

    for ( ; mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {

        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
        if (NULL == poPlayer)
        {
            continue;
        }
        if( poPlayer->GetCliVer() < 301 )
        {
            continue;
        }
        DT_PLAYER_POS_INFO* pPos = poPlayer->GetPositionHotSpring();
        if (NULL == pPos)
        {
            return;
        }
        //设计同步间隔
        if ( (SGDP::SDTimeMilliSec() - pPos->qwSyncTimeLast) < SYNC_INTERVAL_HOT_SPRING  && poPlayer->CkLogin())
        {
            continue;
        }
        poPlayer->SendMsg((const CHAR*)(&stPosNtf), CLIGS_HOT_SPRING_POS_NTF);
        pPos->qwSyncTimeLast = SGDP::SDTimeMilliSec();
    }
}

UINT16 CFaction::ChangeOther(UINT32 dwPlayerID, UINT32 dwSendPlayerID, PKT_CLIGS_CHANGE_OTHER_ACK& stAck)
{
    CMapFactionPlayerDataItr itHead = m_mapFactionPlayerData.find(dwPlayerID);
    if ( itHead == m_mapFactionPlayerData.end())
    {
        return ERR_CHANGE_OTHER::ID_PEOPLE_NOT_EXIST;
    }
    BOOL bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)itHead->second.byJobType, EFA_ChangeOther);
    if ( FALSE == bRet)
    {
        return ERR_CHANGE_OTHER::ID_NOT_MARTIAL_HEAD;
    }

    CMapFactionPlayerDataItr it = m_mapFactionPlayerData.find(dwSendPlayerID);
    if ( it == m_mapFactionPlayerData.end())
    {
        return ERR_CHANGE_OTHER::ID_PEOPLE_NOT_EXIST;
    }
    bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)it->second.byJobType, EFA_BecomeHead);
    if ( FALSE == bRet)
    {
        return ERR_CHANGE_OTHER::ID_POSITION_NOT_ENOUGH;
    }

    //人员变动标识
    SetPlayerChangeToAll();
    //记入日志
    DT_FACTION_LOG_DATA stFactionLogData;
    memset(&stFactionLogData, 0, sizeof(DT_FACTION_LOG_DATA));
    stFactionLogData.dwPlayerID1 = dwPlayerID;
    stFactionLogData.dwPlayerID2 = dwSendPlayerID;
    stFactionLogData.byPlayerJobType1 = itHead->second.byJobType;
    stFactionLogData.byPlayerJobType2 = it->second.byJobType;
    stFactionLogData.byAction = EFLID_ChangeOther;	//退位让贤
    stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
    AddFactionLogData(stFactionLogData);

    itHead->second.byJobType = em_Faction_Elite;
    it->second.byJobType = em_Faction_MartialHead;
    m_stDT_FACTION_BASE_DATA.dwFactionPlayerID = it->second.dwJoinPlayerID;

    GetDT_FACTION_PLAYER_CLI_LST( stAck.stFactionPlayerDataLst);
	stAck.stDoorsTributeLst.wDoorsTributeNum = stAck.stFactionPlayerDataLst.wFactionNum;
	for ( UINT16 wIndex = 0; wIndex < stAck.stFactionPlayerDataLst.wFactionNum; wIndex++)
	{
		CFactorPropMgr::Instance()->GetDoorsTributeInfo(stAck.stFactionPlayerDataLst.astFactionPlayerCli[wIndex].qwDoorsTributeSum, stAck.stDoorsTributeLst.astDoorsTributeInfo[wIndex]);
	}

    DT_FACTION_AUTHORITY_EX* poAuth = CFactorAuthorityPropMgr::Instance()->GetAuthorityEx(em_Faction_Elite);
	if ( NULL != poAuth)
	{
		stAck.stFactionAuthority = *poAuth;
	}
    
    return ERR_CHANGE_OTHER::ID_SUCCESS;

}

VOID CFaction::GetFactionPlayerNum(UINT16& wFactionNumberPeople, UINT16& wMaxFactionNumberPeople)
{
    UINT32 dwFactionLevel = m_stDT_FACTION_BASE_DATA.dwFactionLevel;
    wMaxFactionNumberPeople = CFactorPropMgr::Instance()->GetFactionPlayerMaxNum(dwFactionLevel);
    wFactionNumberPeople = m_mapFactionPlayerData.size();
}

VOID CFaction::SendMessageToAllFactionPlayer(TCHAR* aszSendMsg, UINT16 wMsgID, UINT32 dwPlayerID)
{
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        if ( dwPlayerID != mapIt->second.dwJoinPlayerID)
        {
            CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
            if (NULL == poPlayer)
            {
                continue;
            }

			if ( ( ESM_NO == CSinglePramMgr::Instance()->GetHotSpringFlag() ) && (CLIGS_GIVE_WINE_ACTIVITY_NTF == wMsgID) && (VER_PET <= poPlayer->GetCliVer()))
			{
				continue;
			}

            poPlayer->SendMsg(aszSendMsg, wMsgID);
        }
    }
}

VOID CFaction::SetPlayerChangeToAll()
{
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
        if (NULL == poPlayer)
        {
            continue;
        }
        CFactionPlayer* poFactonPlayer = poPlayer->GetFactionPlayer();
        if ( NULL != poFactonPlayer)
        {
            poFactonPlayer->GetDT_PLAYER_MARK_DATA().byFactionPlayerMark = ESM_YES;
        }

    }

}

VOID CFaction::DeletePlayerDataByID(UINT32 dwPlayerID)
{
    //删除当前玩家数据
    CMapFactionPlayerDataItr it1 = m_mapFactionPlayerData.find(dwPlayerID);
    if ( it1 != m_mapFactionPlayerData.end())
    {
        m_mapFactionPlayerData.erase(it1);
    }
    //删除PLAYERID与FACTIONID对应关系
    CMapFactionPlayerIDItr it2 = s_mapFactionPlayerID.find(dwPlayerID);
    if (it2 != s_mapFactionPlayerID.end())
    {
        s_mapFactionPlayerID.erase(it2);
    }

}

VOID CFaction::DecFactionFunds(UINT32 dwFunds, UINT16 wOpType)
{
    UINT32& dwFactionFunds = GetDT_FACTION_BASE_DATA().dwFactionFunds;
    if ( dwFactionFunds < dwFunds )
    {
        return ;
    }
    else
    {
        dwFactionFunds -= dwFunds;
    }

    //记入日志
    CRecordMgr::Instance()->RecordInfo( m_stDT_FACTION_BASE_DATA.dwFactionID, ERM_DECFACTIONFUNDS, wOpType, dwFunds,
                                        m_stDT_FACTION_BASE_DATA.dwFactionFunds, m_stDT_FACTION_BASE_DATA.dwFactionLevel, 0);
}

VOID CFaction::AfterGiveWineProc()
{
    PKT_CLIGS_ACTIVITY_END_NTF stNtf;
    stNtf.byBuildID = EBK_FACTION;
    stNtf.byFuncTypeID = EFB_HotSpring;

    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
        if (NULL == poPlayer)
        {
            mapIt->second.byDrinkFlag = ESM_YES;
            continue;
        }
        poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_ACTIVITY_END_NTF);
        CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
        if (NULL != poFactionPlayer)
        {
            poFactionPlayer->AfterGiveWineProc();
        }

    }
}

VOID CFaction::AddChatData(const ChatInfo& stChatData)
{
    m_listChat.push_back(const_cast<ChatInfo*>(&stChatData));

    // 门派内未发送的聊天记录达到一定数量时，广播给门派内玩家
    UINT32 dwChatCnt = m_listChat.size();
    if(dwChatCnt >= MAX_CHAT_NUM)
    {
        CChatMgr::Instance()->SendChatInFaction(this);
    }
}

// 添加门派活动聊天公告
VOID CFaction::AddFactionActivityChat(const string &strChatContent)
{
    ChatInfo *pstChatInfo = CChatMgr::Instance()->BuildFactionActivityChat(m_stDT_FACTION_BASE_DATA.dwFactionID, strChatContent);
    if(NULL == pstChatInfo)
    {
        return;
    }

    AddChatData(*pstChatInfo);
}

// 清除本门派聊天记录（但保留部分聊天记录历史）
VOID CFaction::ClearChat(UINT16 wReserveCnt/* = MAX_FACTION_CHAT_HISTORY */)
{
    // 清理本门派的旧聊天记录，将其塞入聊天历史
    m_listChatPool.insert(m_listChatPool.end(), m_listChat.begin(), m_listChat.end());
    m_listChat.clear();

    // 只保留最近几条历史记录
    while (m_listChatPool.size() > wReserveCnt)
    {
        CChatDataList::iterator itr  = m_listChatPool.begin();
        if (itr != m_listChatPool.end())
        {
            CChatMgr::Instance()->FreeChat(*itr);
            m_listChatPool.pop_front();
        }
    }
}

VOID CFaction::GetFactionBuileGroup(DT_FACTION_BUILD_LEVEL& stBuildFlag)
{
    UINT32 dwFactionLevel = GetDT_FACTION_BASE_DATA().dwFactionLevel;
    SFactionUpgradeProp stProp = {0};
    CFactorPropMgr::Instance()->GetFactionUpgradeProp(dwFactionLevel, stProp);
    stBuildFlag.byRareitemLevel = stProp.byRareitemMark;
    stBuildFlag.byGymnasiumLevel = stProp.byGymnasiumMark;
    stBuildFlag.byShadeLevel = stProp.byShadeMark;
    stBuildFlag.byHotSpringLevel = stProp.byHotSpringMark;
    stBuildFlag.byColdPrisonLevel = stProp.byColdPrison;
    stBuildFlag.byFactionTowerLevel = stProp.byFactionTower;
}

UINT32 CFaction::GetFactionBuildLevel(em_Faction_Build emBuildType)
{
    switch (emBuildType)
    {
    case EFB_Faction:
        return m_stDT_FACTION_BASE_DATA.dwFactionLevel;
        break;
    case EFB_Rareitem:
        return m_stDT_FACTION_BASE_DATA.dwRareItemRoomGrade;
        break;
    case EFB_Gymnasium:
        return m_stDT_FACTION_BASE_DATA.dwGymnasiumLevel;
        break;
    case EFB_Shade:
        return m_stDT_FACTION_BASE_DATA.dwShadeLevel;
        break;
    case EFB_HotSpring:
        return m_stDT_FACTION_BASE_DATA.dwHotSpringLevel;
        break;
    case EFB_ColdPrison:
        return m_stDT_FACTION_BASE_DATA.dwColdPrisonLevel;
        break;
    case EFB_FactionTower:
        return m_stDT_FACTION_BASE_DATA.dwFactionTowerLevel;
        break;
    default:
        return 0;
        break;
    }
}

VOID CFaction::GetFactionDataToCli(DT_FACTION_DATA_TO_CLI& stFactionData)
{
    SDStrncpy(stFactionData.aszFactionName, m_stDT_FACTION_BASE_DATA.aszFactionName, MAX_FACTION_NUM - 1);
    SDStrncpy(stFactionData.aszFactionAnnouncement, m_stDT_FACTION_BASE_DATA.aszFactionAnnouncement, MAX_FACTION_DESC_NUM - 1);
    stFactionData.dwFactionFunds = m_stDT_FACTION_BASE_DATA.dwFactionFunds;
    stFactionData.dwFactionLevel = m_stDT_FACTION_BASE_DATA.dwFactionLevel;
    stFactionData.dwFactionRank = m_stDT_FACTION_BASE_DATA.dwFactionRank;
    stFactionData.dwFactionID = m_stDT_FACTION_BASE_DATA.dwFactionID;
}

UINT32 CFaction::GetFactionFunds()
{
	return m_stDT_FACTION_BASE_DATA.dwFactionFunds;
}

VOID CFaction::SetFactionRareInfo()
{

    UINT32 dwLevel = m_stDT_FACTION_BASE_DATA.dwRareItemRoomGrade;
    const UpgradeRareItemMap* pMapRareItem = CRareItemRoomPropMgr::Instance()->GetRareItemInfoByFaction(dwLevel);
    if (NULL != pMapRareItem)
    {
        m_mapRareitemInfo.clear();
        m_mapRareitemInfo = *pMapRareItem;
    }

}

VOID CFaction::SetFactionSkillInfo()
{
    UINT32 dwLevel = m_stDT_FACTION_BASE_DATA.dwGymnasiumLevel;
    m_mapFactionSkill.clear();
    m_mapFactionSkill = CGymnasiumPropMgr::Instance()->GetFactionSkill(dwLevel);
}


VOID CFaction::AfterHuntingProc()
{
    //m_stDT_FACTION_COMMON_DATA.
}


UINT32 CFaction::GetApplyFactionNum()
{
    return m_mapApplyFactionPlayerID.size();
}

DT_FACTION_PLAYER_DATA* CFaction::GetDT_Faction_Player_Data(UINT32 dwPlayerID)
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwPlayerID);
    if ( itr == m_mapFactionPlayerData.end())
    {
        return NULL;
    }
    return &itr->second;
}

UINT32 CFaction::GetMultiApplyFaction(UINT32 dwPlayerID)
{
    return s_multimapApplyFactionPlayer.count(dwPlayerID);
}

VOID CFaction::SetAllNewRareItemFlag()
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin();
    for ( ; itr != m_mapFactionPlayerData.end(); itr++)
    {
        itr->second.byNewGoodFlag = ESM_YES;
    }
}

VOID CFaction::SetAllNewSkillFlag()
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin();
    for ( ; itr != m_mapFactionPlayerData.end(); itr++)
    {
        itr->second.byNewSkillFlag = ESM_YES;
    }
}

BOOL CFaction::IsCanBuyRareItem(UINT32 dwPlayerID)
{
    CMapFactionPlayerDataItr dataItr = m_mapFactionPlayerData.find(dwPlayerID);
    if (dataItr == m_mapFactionPlayerData.end())
    {
        return FALSE;
    }
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if (NULL == poPlayer)
    {
        return FALSE;
    }
    UINT16 wLevel = poPlayer->GetLevel();
    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        return FALSE;
    }
    CMapRareItemGoods& mapMyRareItem = poFactionPlayer->GetRareItemMap();
    SRareItemProp stProp;
    for ( CMapRareItemGoodsItr itr = mapMyRareItem.begin(); itr != mapMyRareItem.end(); itr++)
    {
        UpgradeRareItemItr factionItr = m_mapRareitemInfo.find(itr->second.wRareItemID);
        if (factionItr != m_mapRareitemInfo.end())
        {
            if( itr->second.dwRareItemHaveExchangeNum < factionItr->second.dwCurMaxRareItem )
            {
                CRareItemRoomPropMgr::Instance()->GetConfig(stProp, itr->second.wRareItemID);
                if ( wLevel < stProp.byBuyNeedLevel )
                {
                    continue;
                }
                if( (stProp.dwBuyCost <= dataItr->second.dwDoorsTribute) && (stProp.dwSumDoorsTributeRequire <= dataItr->second.qwDoorsTributeSum))
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL CFaction::IsCanLearnSkill(UINT32 dwPlayerID)
{
    CMapFactionPlayerDataItr dataItr = m_mapFactionPlayerData.find(dwPlayerID);
    if (dataItr == m_mapFactionPlayerData.end())
    {
        return FALSE;
    }
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if (NULL == poPlayer)
    {
        return FALSE;
    }
    CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
    if (NULL == poFactionPlayer)
    {
        return FALSE;
    }
    CMapMySkill& mapMySkill = poFactionPlayer->GetMySkillMap();
    SSkillProp stProp;
    for ( CMapMySkillItr itr = mapMySkill.begin(); itr != mapMySkill.end(); itr++)
    {
        CMapFactionSkillItr factionItr = m_mapFactionSkill.find(itr->second.bySkillID);
        if (factionItr != m_mapFactionSkill.end())
        {
            if( itr->second.byCurSkillLevel < factionItr->second.bySkillCurMax && 1 == factionItr->second.byUnlockFlag)
            {
                CGymnasiumPropMgr::Instance()->GetSkillInfoProp(stProp, itr->second.bySkillID, itr->second.byCurSkillLevel);
				if ( poPlayer->GetCliVer() > 302)	//302版本后还要判断累积门贡
				{
					if ( stProp.dwSumDoorsTribute <= dataItr->second.qwDoorsTributeSum)
					{
						if ( stProp.LearnDoorsTribute <= dataItr->second.dwDoorsTribute )
						{
							return TRUE;
						}
					}
				}
				else
				{
					if ( stProp.LearnDoorsTribute <= dataItr->second.dwDoorsTribute )
					{
						return TRUE;
					}
				}
				
            }
        }
    }
    return FALSE;
}

VOID CFaction::SendMessage(TCHAR* aszMsg, UINT16 wMsgID, em_Faction_Authority emAuth)
{
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        if ( CFactorAuthorityPropMgr::Instance()->CkJobAuthority(static_cast<em_Faction_Job>(mapIt->second.byJobType), emAuth))
        {
            CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
            if (NULL == poPlayer)
            {
                continue;
            }
            poPlayer->SendMsg(aszMsg, wMsgID);
        }
    }
}

VOID CFaction::SendMessageByAuthPrompt()
{
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        if ( CFactorAuthorityPropMgr::Instance()->CkJobAuthority(static_cast<em_Faction_Job>(mapIt->second.byJobType), EFA_Audit))
        {
            CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
            if (NULL == poPlayer)
            {
                continue;
            }
            CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
            if ( NULL != poFactionPlayer)
            {
                PKT_CLIGS_HOME_BUILD_PROMPT_NTF stNtf;
                memset(&stNtf, 0, sizeof(PKT_CLIGS_HOME_BUILD_PROMPT_NTF));
                poPlayer->GetFactionPrompt(stNtf.stPromptInfo.astBuildPromptInfo[stNtf.stPromptInfo.byBuildNum++]);
                poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_HOME_BUILD_PROMPT_NTF);

                PKT_CLIGS_FACTION_PROMPT_NTF stFactionNtf;
                memset(&stFactionNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
                poFactionPlayer->GetFactionPrompt(stFactionNtf.stFactionPrompt);
                poPlayer->SendMsg((TCHAR*)&stFactionNtf, CLIGS_FACTION_PROMPT_NTF);
            }
        }
    }
}

VOID CFaction::SendMessageByGymnasiumPrompt()
{
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
        if (NULL == poPlayer)
        {
            continue;
        }
        CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
        if ( NULL != poFactionPlayer)
        {
            PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
            memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
            CGymnasium& oGymnasium = poFactionPlayer->GetGymnasium();
            oGymnasium.GetPrompt(stNtf.stFactionPrompt);
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }
}


VOID CFaction::SendMessageByRareItemPrompt()
{
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
        if (NULL == poPlayer)
        {
            continue;
        }
        CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
        if ( NULL != poFactionPlayer)
        {
            PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
            memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
            CRareItemRoom& oRareItem = poFactionPlayer->GetRareItem();
            oRareItem.GetPrompt(stNtf.stFactionPrompt);
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }
}

VOID CFaction::SendMessageByShadePrompt()
{
    if (0 == m_stDT_FACTION_BASE_DATA.dwShadeLevel)
    {
        return ;
    }
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
        if (NULL == poPlayer)
        {
            continue;
        }
        CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
        if ( NULL != poFactionPlayer)
        {
            PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
            memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
            CShade& oShade = poFactionPlayer->GetShade();
            oShade.GetPrompt(stNtf.stFactionPrompt);
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }
}

VOID CFaction::SendMessageByHotSpringPrompt()
{
    if (0 == m_stDT_FACTION_BASE_DATA.dwHotSpringLevel)
    {
        return ;
    }
    for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
        if (NULL == poPlayer)
        {
            continue;
        }
        CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
        if ( NULL != poFactionPlayer)
        {
            PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
            memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
            CHotSpring& oHotSpring = poFactionPlayer->GetHotSpring();
            oHotSpring.GetPrompt(stNtf.stFactionPrompt);
            poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
        }
    }
}

VOID CFaction::SendMessageByFactionPrompt()
{
	for( CMapFactionPlayerDataItr mapIt = m_mapFactionPlayerData.begin(); mapIt != m_mapFactionPlayerData.end(); mapIt++)
	{
		CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(mapIt->second.dwJoinPlayerID);
		if (NULL == poPlayer)
		{
			continue;
		}
		CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
		if ( NULL != poFactionPlayer)
		{
			PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
			memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
			poFactionPlayer->GetFactionPrompt(stNtf.stFactionPrompt);
			poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
		}
	}
}

VOID CFaction::SendMessageByLogNtf(DT_PLAYER_FACTION_BATTLELOG_DATA& stData)
{
    CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin();
    for ( ; itr != m_mapFactionPlayerData.end(); itr++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second.dwJoinPlayerID);
        if ( NULL == poPlayer)
        {
            if (itr->second.stUnLoginBattleLogList.byLogNum < MAX_FACTION_BATTLE_LOG_NUM)
            {
                itr->second.stUnLoginBattleLogList.astPlayerFactionLog[itr->second.stUnLoginBattleLogList.byLogNum++] = stData;
            }
        }
        else
        {
            CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
            if (NULL != poFactionPlayer)
            {
                poFactionPlayer->AddFactionBattleLog(stData);
                PKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF stNtf;
                stNtf.byUnReadLogNum = poFactionPlayer->GetUnReadFactionBattleLogNum();
                stNtf.byLastOperateType = EFBT_FactionUpgrade;
                poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_PLAYER_FACTION_BATTLELOG_NTF);
            }
        }

    }
}

VOID CFaction::SetFactionLevelByGM(UINT32 dwFactionLevel)
{
    if ( dwFactionLevel <= 0 || dwFactionLevel > 10)
    {
        return ;
    }
    m_stDT_FACTION_BASE_DATA.dwFactionLevel = dwFactionLevel;

    SFactionUpgradeProp stCurFactionUpgrade = {0};
    CFactorPropMgr::Instance()->GetFactionUpgradeProp(dwFactionLevel, stCurFactionUpgrade);

    if ( ESM_NO == stCurFactionUpgrade.byRareitemMark)
    {
        m_stDT_FACTION_BASE_DATA.dwRareItemRoomGrade = 0;
    }
    else
    {
        m_stDT_FACTION_BASE_DATA.dwRareItemRoomGrade = 1;
    }

    if ( ESM_YES == stCurFactionUpgrade.byGymnasiumMark)
    {
        m_stDT_FACTION_BASE_DATA.dwGymnasiumLevel = 1;
    }
    else
    {
        m_stDT_FACTION_BASE_DATA.dwGymnasiumLevel = 0;
    }


    if ( ESM_YES == stCurFactionUpgrade.byShadeMark)
    {
        m_stDT_FACTION_BASE_DATA.dwShadeLevel = 1;
    }
    else
    {
        m_stDT_FACTION_BASE_DATA.dwShadeLevel = 0;
    }

    if ( ESM_YES == stCurFactionUpgrade.byHotSpringMark)
    {
        m_stDT_FACTION_BASE_DATA.dwHotSpringLevel = 1;
    }
    else
    {
        m_stDT_FACTION_BASE_DATA.dwHotSpringLevel = 0;
    }
    SetFactionRareInfo();
    SetFactionSkillInfo();

}

VOID CFaction::GetAuditData(UINT8& byTodayPassNum, UINT64& qwLastAuditTime, UINT8 &byExt)
{
	if ( !IsToday(m_qwLastAuditTime) )
	{
		m_byTodayPassNum = 0;
		m_qwLastAuditTime = SGDP::SDTimeSecs();
	}
    byExt = m_byExt;
	byTodayPassNum = m_byTodayPassNum;
	qwLastAuditTime = m_qwLastAuditTime;
}

UINT8 CFaction::GetTodayPassNum()
{
	if ( !IsToday(m_qwLastAuditTime) )
	{
		m_byTodayPassNum = 0;
		m_qwLastAuditTime = SGDP::SDTimeSecs();
	}
	return m_byTodayPassNum;
}

VOID CFaction::CleanAllApply()
{
	CMapFactionPlayerIDItr itr = m_mapApplyFactionPlayerID.begin();
	for ( ; itr != m_mapApplyFactionPlayerID.end(); itr++)
	{
		Range   range = s_multimapApplyFactionPlayer.equal_range(itr->second);
		for(CMultiMapApplyFactionPlayerItr multiItr = range.first; multiItr != range.second; multiItr++)
		{
			if (multiItr->second == m_stDT_FACTION_BASE_DATA.dwFactionID)
			{
				s_multimapApplyFactionPlayer.erase(multiItr);
				break;
			}
		}	
	}

	m_mapApplyFactionPlayerID.clear();
}

BOOL CFaction::AddFindBackDoorsTribute(UINT32 dwPlayerID, UINT8 byFindType)
{
	CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwPlayerID);
	if ( itr == m_mapFactionPlayerData.end() )
	{
		return FALSE;
	}

	const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
	if (NULL == pstBaseData)
	{
		return FALSE;//无玩家数据， 废数据
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL == poPlayer)
	{
		return FALSE;
	}

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL == poFactionPlayer)
	{
		return FALSE;
	}

	UINT32 dwFindBack = poFactionPlayer->GetFindBackDoorsTribute();

	if( 0 == byFindType )//普通找回
	{
		dwFindBack = (dwFindBack * CSinglePramMgr::Instance()->GetNormalFindBackDoorsTribute()) / 100;
	}
	else
	{
		dwFindBack = (dwFindBack * CSinglePramMgr::Instance()->GetPerfectFindBackDoorsTribute()) / 100;
	}

	itr->second.dwDoorsTribute += dwFindBack;
	poFactionPlayer->ResetFindBackDoorsTribute();

	poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAddDoorsTribute += dwFindBack;
	poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAfterDoorsTribute = itr->second.dwDoorsTribute;

	//记入日志
	CRecordMgr::Instance()->RecordInfo( dwPlayerID, ERM_ADDDOORSTRIBUTE, CRecordMgr::EAT_FIND_BACK, dwFindBack, itr->second.dwDoorsTribute, pstBaseData->wLevel, pstBaseData->byVipLevel);
	
	return TRUE;
}

VOID CFaction::ChangeFactionNameByGM(TCHAR aszFactionName[MAX_FACTION_NUM])
{
	if ( NULL == aszFactionName)
	{
		return ;
	}

	BOOL bRet = IsUTF8(aszFactionName);
	if ( bRet)
	{
		CkEmoji(aszFactionName);
		SDStrncpy(m_stDT_FACTION_BASE_DATA.aszFactionName, aszFactionName, MAX_FACTION_NUM - 1);
	}
	
}

void CFaction::KickOutFormationLeavePlayers()
{
    UINT8 byCurMaxPosNum    = CGvGPropMgr::Instance()->GetFormationUnlockPosNum(m_stDT_FACTION_BASE_DATA.dwFactionLevel);
    UINT16 wNeedPlayerLevel = CGvGPropMgr::Instance()->GetJoinGVGPlayerLevel();

    C32Vec vecFormationPlayerID;

    DT_GVG_FORMATION_DATA &stGvGFormation = m_stFactionGvGData.stGvGFormation;
    for(UINT16 i = 0; i < stGvGFormation.byPlayerNum; i++)
    {
        UINT32 dwPlayerID = stGvGFormation.adwPlayerIDList[i];
        if(0 == dwPlayerID)
        {
            continue;
        }

        if(!IsJoin(dwPlayerID))
        {
            continue;
        }

        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
        if (NULL == poPlayerBaseData)
        {
            continue;
        }

        if (poPlayerBaseData->wLevel < wNeedPlayerLevel)
        {
            continue;
        }

        vecFormationPlayerID.push_back(dwPlayerID);
    }

    ResetFormationInfo();
    memset(&m_stFactionGvGData.stGvGFormation, 0, sizeof(DT_GVG_FORMATION_DATA));

    for(UINT16 i = 0; i < vecFormationPlayerID.size(); i++)
    {
        stGvGFormation.adwPlayerIDList[i] = vecFormationPlayerID[i];
        stGvGFormation.byPlayerNum++;

        if(stGvGFormation.byPlayerNum >= MAX_GVG_FORMATION_PLAYER_CNT || stGvGFormation.byPlayerNum >= byCurMaxPosNum)
        {
            break;
        }
    }
}

void CFaction::AutoFormation(UINT16 wAutoFormationCnt)
{
    KickOutFormationLeavePlayers();

    if(m_stFactionGvGData.stGvGFormation.byPlayerNum > 0)
    {
        return;
    }

    if(0 == wAutoFormationCnt)
    {
        return;
    }

    UINT16 wNeedPlayerLevel = CGvGPropMgr::Instance()->GetJoinGVGPlayerLevel();

    typedef std::vector<SFormationPlayerSort> SFormationPlayerSortVec;

    // 将本门派玩家根据职位和战力进行排序
    SFormationPlayerSortVec vecFormationPlayer;   
    for(CMapFactionPlayerData::iterator itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); ++itr)
    {
        UINT32 dwPlayerID = itr->first;
        const SPlayerBaseData *pstPlayerBase = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
        if(NULL == pstPlayerBase)
        { 
            continue;
        }

        if(0 == dwPlayerID)
        {
            continue;
        }

        SFormationPlayerSort stFormationPlayerSort;
        stFormationPlayerSort.pstBaseData      = const_cast<SPlayerBaseData*>(pstPlayerBase);
        stFormationPlayerSort.pstFactionPlayer = &itr->second;

        vecFormationPlayer.push_back(stFormationPlayerSort);
    }

    std::sort(vecFormationPlayer.begin(), vecFormationPlayer.end(), SortByJobPowerDesc);

    UINT8 byCurMaxPosNum = CGvGPropMgr::Instance()->GetFormationUnlockPosNum(m_stDT_FACTION_BASE_DATA.dwFactionLevel);

    DT_GVG_FORMATION_DATA &stGvGFormation = m_stFactionGvGData.stGvGFormation;
    stGvGFormation.byPlayerNum = 0;
    for(UINT16 i = 0; i < vecFormationPlayer.size(); i++)
    {
        UINT32 dwPlayerID = vecFormationPlayer[i].pstBaseData->dwID;

        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
        if (NULL == poPlayerBaseData)
        {
            continue;
        }

        if (poPlayerBaseData->wLevel < wNeedPlayerLevel)
        {
            continue;
        }

        stGvGFormation.adwPlayerIDList[stGvGFormation.byPlayerNum++] = dwPlayerID;

        if(stGvGFormation.byPlayerNum >= CGvGPropMgr::Instance()->GetGvGProp().wAutoFormationCnt)
        {
            break;
        }

        if(stGvGFormation.byPlayerNum >= MAX_GVG_FORMATION_PLAYER_CNT || stGvGFormation.byPlayerNum >= byCurMaxPosNum)
        {
            break;
        }
    }
}

BOOL CFaction::IsAllFormationPlayerInMemory()
{
    CPlayerVec vecPlayer;
    DT_GVG_FORMATION_DATA &stFormation = m_stFactionGvGData.stGvGFormation;

    BOOL bAllInMemory = TRUE;
    for(UINT16 i = 0; i < stFormation.byPlayerNum; i++)
    {
        UINT32 dwPlayerID = stFormation.adwPlayerIDList[i];

        CPlayer *poPlayer = CPlayerSnapShotMgr::Instance()->CreatePlayerCache(dwPlayerID);
        if(NULL == poPlayer)
        {
            poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);            
        }

        if(NULL == poPlayer)
        {
            bAllInMemory = FALSE;
            break;
        }
    }

    return bAllInMemory;
}

CPlayerVec CFaction::GetFormationPlayers()
{
    CPlayerVec vecPlayer;
    DT_GVG_FORMATION_DATA &stFormation = m_stFactionGvGData.stGvGFormation;
    
    for(UINT16 i = 0; i < stFormation.byPlayerNum; i++)
    {
        UINT32 dwPlayerID = stFormation.adwPlayerIDList[i];

        CPlayer *poPlayer = CPlayerSnapShotMgr::Instance()->CreatePlayerCache(dwPlayerID);
        if(NULL == poPlayer)
        {
            poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);            
        }

        if(poPlayer)
        {
            vecPlayer.push_back(poPlayer);
        }
    }

    return vecPlayer;
}

void CFaction::TakeFormationPlayersFromDB()
{
    CPlayerVec vecPlayer;
    DT_GVG_FORMATION_DATA &stFormation = m_stFactionGvGData.stGvGFormation;

    for(UINT16 i = 0; i < stFormation.byPlayerNum; i++)
    {
        UINT32 dwPlayerID = stFormation.adwPlayerIDList[i];

        CPlayer *poPlayer = CPlayerSnapShotMgr::Instance()->CreatePlayerCache(dwPlayerID);
        if(NULL == poPlayer)
        {
            poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);            
        }

        if(NULL == poPlayer)
        {
            // 不在内存中：从数据库中调出玩家数据，成功调出后不做任何操作
            CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_SYNCDATA, dwPlayerID);
            continue;
        }
    }
}

void CFaction::GetFactionMemberInfoList(DT_FACTION_MEMBER_INFO_LIST &stInfoList)
{
    memset(&stInfoList, 0x00, sizeof(stInfoList));

    CListSortPlayer listSoryByJob;
    for (CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
    {
        listSoryByJob.push_back(&itr->second);
    }

    listSoryByJob.sort(CManagerSort());

    for(CListPlayerItr sortItr = listSoryByJob.begin(); sortItr != listSoryByJob.end() && stInfoList.byMemberCnt < DT_FACTION_PLAYER_NUM; sortItr++)
    {
        const DT_FACTION_PLAYER_DATA &stFactionPlayer = **sortItr;
        DT_FACTION_MEMBER_INFO &stMember = stInfoList.astMembers[stInfoList.byMemberCnt++];

        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stFactionPlayer.dwJoinPlayerID);
        if(NULL == poPlayerBaseData)
        {
            return;
        }

        stMember.dwPlayerID = stFactionPlayer.dwJoinPlayerID;
        stMember.wPlayerLevel = poPlayerBaseData->wLevel;
        stMember.byPlayerJob = stFactionPlayer.byJobType;
        stMember.dwPower = poPlayerBaseData->dwPower;
        stMember.wHeroKindID = poPlayerBaseData->wMainHeroKindID;
        stMember.dwRank = poPlayerBaseData->dwRank;

        strcpy( stMember.aszPlayerName, poPlayerBaseData->aszDispName);
    }
}

// 把gvg所需的本门派基础信息存入结构体，用于客户端同步
BOOL CFaction::SaveGvGPKInfoTo(DT_PK_FACTION &stFaction)
{
    KickOutFormationLeavePlayers();
    AutoFormation(1);

    memset(&stFaction, 0, sizeof(stFaction));

    DT_GVG_FORMATION_DATA &stFormation = stFaction.stFormation;
    DT_GVG_FORMATION_PLAYER_LIST &stPlayerList = stFaction.stPlayerList;

    SaveGvGInfoTo(stFaction.stBaseData);
    stFormation = m_stFactionGvGData.stGvGFormation;

    for(UINT16 i = 0; i < stFormation.byPlayerNum; i++)
    {
        UINT32 dwPlayerID = stFormation.adwPlayerIDList[i];
        const SPlayerBaseData *pstPlayerBase = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
        if(NULL == pstPlayerBase)
        {
            SYS_CRITICAL( _SDT("[%s: %d] CFaction::SaveGvGInfoTo could not find player base data <%u>!"), MSG_MARK, dwPlayerID);
            continue;
        }

        if(0 == dwPlayerID)
        {
            continue;
        }

        if(m_mapFactionPlayerData.find(dwPlayerID) == m_mapFactionPlayerData.end())
        {
            continue;
        }

        DT_GVG_FORMATION_PLAYER_INFO &stPlayerInfo = stPlayerList.astPlayerList[stPlayerList.byPlayerCnt++];
        stPlayerInfo.dwPlayerID  = dwPlayerID;
        stPlayerInfo.byPlayerJob = GetJob(dwPlayerID);
        stPlayerInfo.byKillCombo = 0;
        SDStrncpy(stPlayerInfo.aszPlayerName, pstPlayerBase->aszDispName, USERNAME_LEN - 1);
    }

    return TRUE;
}

BOOL CFaction::SaveGvGInfoTo(DT_GVG_FACTION &stFaction)
{
    DT_FACTION_BASE_DATA_CLI2 &stBaseCli = stFaction.stBaseData;
    GetFactionBaseDataCli2(stBaseCli);

    ReCalcSumPower();

    stFaction.byJoinCnt  = m_stFactionGvGData.stGvGFormation.byPlayerNum;
    stFaction.qwSumPower = GetSumPower();
    stFaction.wZoneID    = gsapi::GetZoneID();
    stFaction.wOriZoneID = GetOriZoneID();

    GetFactionMemberInfoList(stFaction.stMembers);
    return TRUE;
}

UINT16 CFaction::RegisterFactionName(CHAR* pszDspName, CPlayer * poPlayer)//注册昵称
{
    //检测是否可修改
    if(1 != m_byExt)
    {
        return ERR_REGISTER_FACTION::ID_UNREGISTER;
    }

    //检测是否是帮主
    if(poPlayer->GetID() != m_stDT_FACTION_BASE_DATA.dwFactionPlayerID)
    {
        return ERR_REGISTER_FACTION::ID_NOTLEAD_UNREGISTER;
    }

	//检查各个参数是否有sql嵌入攻击
	if(HaveSqlToken(pszDspName))
	{
		return ERR_REGISTER_FACTION::ID_INVALID_CHARACTER;
	}

	//检测是否有特殊字符，且长度合法(昵称允许6个中文字符，12个英文)
	if(CkHaveSpeCharacterAndValidLen(pszDspName))
	{
		return ERR_REGISTER_FACTION::ID_INVALID_CHARACTER;
	}

	//检查是否有敏感词
	if(CChatMgr::Instance()->DspNameHaveSensitiveWord(poPlayer, pszDspName))
	{
		return ERR_REGISTER_FACTION::ID_SENSITIVE_WORD;
	}

    if(CFactionMgr::Instance()->IsExitFactionName(pszDspName))
    {
        return ERR_REGISTER_FACTION::ID_DSPNAME_CONFLICT;
    }

    // 记录改名日志
    CHAR szComment[512] = {0};
    sprintf(szComment, "leader[%s] change faction name [%s] to [%s]", poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, m_stDT_FACTION_BASE_DATA.aszFactionName, pszDspName);

    CRecordMgr::Instance()->RecordInfo(poPlayer->GetID(), ERM_PLAYER_OPERATE, CRecordMgr::EPO_RE_REG_FACTION_NAME, 1, 1, 
        poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 
        m_stDT_FACTION_BASE_DATA.dwFactionID, m_stDT_FACTION_BASE_DATA.dwFactionPlayerID, m_stDT_FACTION_BASE_DATA.dwFactionRank, m_stDT_FACTION_BASE_DATA.dwFactionLevel, 
        szComment);

    CFactionMgr::Instance()->RegFactionName(pszDspName, this, m_stDT_FACTION_BASE_DATA.aszFactionName);
    SDStrncpy(m_stDT_FACTION_BASE_DATA.aszFactionName, pszDspName, MAX_FACTION_NUM);
    m_byExt = 0;

    return ERR_REGISTER_FACTION::ID_SUCCESS;
}
//获取管理人员的信息
VOID CFaction::GetManagerInfo(DT_FACTION_PLAYER_CLI_LST& stPlayerLst)
{
	CMapFactionPlayerDataItr itr;
	memset(&stPlayerLst, 0x00, sizeof(DT_FACTION_PLAYER_CLI_LST));

	m_listSoryByJob.clear();
	for (itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
	{
		m_listSoryByJob.push_back(&itr->second);
	}
	m_listSoryByJob.sort(CManagerSort());

	CListPlayerItr sortItr = m_listSoryByJob.begin();
	for( ; sortItr != m_listSoryByJob.end() && stPlayerLst.wFactionNum < DT_FACTION_PLAYER_NUM; sortItr++)
	{
		if ( (*sortItr)->byJobType < em_Faction_Elite )
		{
			GetDT_FACTION_PLAYER_CLI(stPlayerLst.astFactionPlayerCli[stPlayerLst.wFactionNum++], (*sortItr)->dwJoinPlayerID);
		}
	}
}

UINT8 CFaction::GetFactionIconID()
{
	return m_stDT_FACTION_COMMON_DATA.stIconData.byIcon;
}

VOID CFaction::GetFactionBaseDataCli2(DT_FACTION_BASE_DATA_CLI2& stBaseCli)
{
	SDStrncpy(stBaseCli.aszFactionName, m_stDT_FACTION_BASE_DATA.aszFactionName, MAX_FACTION_NUM - 1);
	SDStrncpy(stBaseCli.aszFactionAnnouncement, m_stDT_FACTION_BASE_DATA.aszFactionAnnouncement, MAX_FACTION_DESC_NUM - 1);
	const SPlayerBaseData* poHeadData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_FACTION_BASE_DATA.dwFactionPlayerID);

	if ( NULL != poHeadData)
	{
		SDStrncpy(stBaseCli.aszHeadName, poHeadData->aszDispName, USERNAME_LEN - 1);
	}
	stBaseCli.byIconID = GetFactionIconID();
	stBaseCli.dwFactionFunds = m_stDT_FACTION_BASE_DATA.dwFactionFunds;
	stBaseCli.qwFactionFundsSum = m_stDT_FACTION_BASE_DATA.qwFactionFundsSum;
	stBaseCli.dwFactionID = m_stDT_FACTION_BASE_DATA.dwFactionID;
	stBaseCli.dwFactionLevel = m_stDT_FACTION_BASE_DATA.dwFactionLevel;
	stBaseCli.dwFactionRank = m_stDT_FACTION_BASE_DATA.dwFactionRank;
	GetFactionPlayerNum(stBaseCli.wCurPlayerNum, stBaseCli.wMaxPlayerNum);
    
    std::string strZoneName = CGvGPropMgr::Instance()->GetZoneName(GetOriZoneID());
    SDStrncpy(stBaseCli.aszZoneName, strZoneName.c_str(), ZONENAME_LEN - 1);
}

VOID CFaction::GetFormationInfo(
	DT_FACTION_FORMATION_POS_LST_INFO& stFormationInfo,
	DT_GVG_FORMATION_SIMPLE_PLAYER_DATA_LST_CLI& stPlayerLstInfo,
	UINT16& wCurFactionLevel)
{
    KickOutFormationLeavePlayers();
    AutoFormation(1);

	wCurFactionLevel = static_cast<UINT16>(m_stDT_FACTION_BASE_DATA.dwFactionLevel);
	UINT8 byPlayerNum = 0;
	UINT16 wNeedPlayerLevel = CGvGPropMgr::Instance()->GetJoinGVGPlayerLevel();
	UINT8 byMaxPosNum = CGvGPropMgr::Instance()->GetMaxFormationPosNum();
	UINT8 byCurPosNum = CGvGPropMgr::Instance()->GetFormationUnlockPosNum(wCurFactionLevel);
	//阵型上的玩家信息
	set<UINT32> setPlayerID;
	CMapFactionPlayerDataItr itr;
	UINT8 byIdx = 0;
	for ( byIdx = 0; byIdx < m_stFactionGvGData.stGvGFormation.byPlayerNum && byIdx < byCurPosNum; byIdx++)
	{
		UINT32 dwPlayerID = m_stFactionGvGData.stGvGFormation.adwPlayerIDList[byIdx];
		itr = m_mapFactionPlayerData.find(dwPlayerID);
		if ( itr != m_mapFactionPlayerData.end() )
		{
			const SPlayerBaseData* poBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(itr->second.dwJoinPlayerID);
			if ( NULL != poBaseData )
			{
				if ( wNeedPlayerLevel <= poBaseData->wLevel )
				{
					SDStrncpy(stPlayerLstInfo.astPlayerInfo[byPlayerNum].aszDispPlayerName, poBaseData->aszDispName, USERNAME_LEN - 1);
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].dwPlayerID = itr->second.dwJoinPlayerID;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].dwPower = poBaseData->dwPower;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].dwCoachID = poBaseData->wMainHeroKindID;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].byPosIdx = byIdx + 1;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].byJobType = itr->second.byJobType;
					byPlayerNum++;
					setPlayerID.insert(dwPlayerID);
				}
			}
		}
	}
	//其它玩家信息
	for ( itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
	{
		if ( setPlayerID.find(itr->first) == setPlayerID.end())
		{
			const SPlayerBaseData* poBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(itr->second.dwJoinPlayerID);
			if ( NULL != poBaseData )
			{
				if ( wNeedPlayerLevel <= poBaseData->wLevel )
				{
					SDStrncpy(stPlayerLstInfo.astPlayerInfo[byPlayerNum].aszDispPlayerName, poBaseData->aszDispName, USERNAME_LEN - 1);
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].dwPlayerID = itr->second.dwJoinPlayerID;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].dwPower = poBaseData->dwPower;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].dwCoachID = poBaseData->wMainHeroKindID;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].byPosIdx = 0;
					stPlayerLstInfo.astPlayerInfo[byPlayerNum].byJobType = itr->second.byJobType;
					byPlayerNum++;
				}
			}
		}
		
	}
	stPlayerLstInfo.byPlayerNum = byPlayerNum;
	UINT8 byUnlockPosNum = 0;
	for ( byIdx = 0; byIdx < byMaxPosNum; byIdx++)
	{
		if ( byIdx < byCurPosNum)
		{
			stFormationInfo.astFormationPosInfo[byIdx].byLockState = ESM_YES;
			byUnlockPosNum++;
		}
		else
		{
			stFormationInfo.astFormationPosInfo[byIdx].byLockState = ESM_NO;
			stFormationInfo.astFormationPosInfo[byIdx].wNeedFactionLevel = CGvGPropMgr::Instance()->GetNeedFactionLevelByPosNum(byIdx + 1);
		}
	}
	stFormationInfo.byPosNum = byIdx;
	stFormationInfo.byUnlockPosNum = byUnlockPosNum;

    ReCalcSumPower();
}

VOID CFaction::ResetFormationInfo()
{
	memset(&m_stFactionGvGData.stGvGFormation, 0, sizeof(DT_GVG_FORMATION_DATA));
	m_stFactionGvGData.stGvGFormation.adwPlayerIDList[0] = m_stDT_FACTION_BASE_DATA.dwFactionPlayerID;
	m_stFactionGvGData.stGvGFormation.byPlayerNum = 1;
}

UINT64 CFaction::ReCalcSumPower()
{
    UINT64 qwSumPower = 0;
    DT_GVG_FORMATION_DATA &stGvGFormation = m_stFactionGvGData.stGvGFormation;

    for ( UINT8 byIdx = 0; byIdx < stGvGFormation.byPlayerNum; byIdx++)
    {
        UINT32 dwPlayerID = stGvGFormation.adwPlayerIDList[byIdx];
        if(0 == dwPlayerID)
        {
            continue;
        }

        const SPlayerBaseData* poPlayerData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
        if ( poPlayerData )
        {
            qwSumPower += poPlayerData->dwPower;
        }
    }

    m_stFactionGvGData.qwSumPower = qwSumPower;
    return qwSumPower;
}

BOOL CFaction::IsJoin(UINT32 dwPlayerID)
{
    return m_mapFactionPlayerData.find(dwPlayerID) != m_mapFactionPlayerData.end();
}

UINT16 CFaction::GvGFactionEmbattle(PKT_CLIGS_GVG_FACTION_EMBATTLE_REQ& stReq)
{
	ResetFormationInfo();
	C32Set setPlayerID;
	UINT8 byPlayerNum = 0;
	UINT32 dwHeadID = m_stDT_FACTION_BASE_DATA.dwFactionPlayerID;
	BOOL bRet = FALSE;
	for ( UINT8 byIdx = 0; byIdx < stReq.stFormation.byPlayerNum; byIdx++)
	{
		if ( dwHeadID == stReq.stFormation.adwPlayerIDForFormation[byIdx] )
		{
			bRet = TRUE;
			break;
		}
	}

	if ( !bRet)
	{
		return ERR_GVG_FACTION_EMBATTLE::ID_HEAD_MUST_IN_FORMATION;
	}

	for ( UINT8 byIdx = 0; byIdx < stReq.stFormation.byPlayerNum; byIdx++)
	{
		if( 0 != stReq.stFormation.adwPlayerIDForFormation[byIdx] 
		    && setPlayerID.find(stReq.stFormation.adwPlayerIDForFormation[byIdx]) == setPlayerID.end() //不重复
		    && IsJoin(stReq.stFormation.adwPlayerIDForFormation[byIdx]) /*本门派成员*/)
		{
			m_stFactionGvGData.stGvGFormation.adwPlayerIDList[byPlayerNum] = stReq.stFormation.adwPlayerIDForFormation[byIdx];
			setPlayerID.insert(stReq.stFormation.adwPlayerIDForFormation[byIdx]);

            byPlayerNum++;
        }
	}

    m_stFactionGvGData.stGvGFormation.byPlayerNum = byPlayerNum;
    UINT64 qwSumPower = ReCalcSumPower();

	DT_GVG_FACTION* pstGvGFaction = CGvGFactionMgr::Instance()->FindFactionInfo( m_stDT_FACTION_BASE_DATA.dwFactionID);
	if ( NULL != pstGvGFaction )
	{
		pstGvGFaction->qwSumPower = qwSumPower;
		m_stFactionGvGData.qwSumPower = qwSumPower;
		pstGvGFaction->byJoinCnt = byPlayerNum;
		GetFactionBaseDataCli2(pstGvGFaction->stBaseData);
	}

	AddPlayerTosnapshot();//把阵法上的玩家加入快照
	return ERR_GVG_FACTION_EMBATTLE::ID_SUCCESS;
}

VOID CFaction::AddGvGFactionFunds(UINT32 dwFactionFunds)
{ 
	m_stFactionGvGData.qwGVGFactionFunds += dwFactionFunds; 
}

VOID CFaction::ResetGvGFactionFunds()
{ 
	m_stFactionGvGData.qwGVGFactionFunds = 0; 
}

VOID CFaction::AddPlayerTosnapshot()
{
    if (!CGvGPropMgr::Instance()->GetGvGProp().bUsePlayerSnapShot)
    {
        return;
    }

	for ( UINT8 byIdx = 0; byIdx < m_stFactionGvGData.stGvGFormation.byPlayerNum; byIdx++)
	{
		UINT32 dwPlayerID = m_stFactionGvGData.stGvGFormation.adwPlayerIDList[byIdx];
		CPlayerSnapShotMgr::Instance()->AddPlayerSnapShot(dwPlayerID);
	}
}

VOID CFaction::CheckGvGMedal()
{
	for ( CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
	{
		CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second.dwJoinPlayerID);
		if ( NULL != poPlayer)
		{
			CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
			if ( NULL != poPlayerMedal)
			{
				poPlayerMedal->CheckLimitMedal(EMST_GVG_MEDAL);
			}
		}
	}
	
}

void CFaction::AddGvGLog(DT_FACTION_SIMPLE_GVG_LOG_LIST &stSimpleLogList)
{
    if(m_stDT_FACTION_BASE_DATA.dwFactionID != stSimpleLogList.dwFactionID)
    {
        return;
    }
    
    ClearOldGvG();
    CGvGBuild::Instance()->SimpleGvGLogToFullLog(m_stFactionGvGData.stGvGLogList, stSimpleLogList);
}

void CFaction::AddLocalGvGLogNotStarted(UINT32 dwFactionID1, UINT32 dwFactionID2, DT_GVG_ROUND &stRound, VideoID videoID)
{
    if(0 == videoID)
    {
        return;
    }

    DT_FACTION_GVG_LOG_LIST &stLogList = m_stFactionGvGData.stGvGLogList;
    if(gvgutil::FindGvGLog(stLogList, stRound))
    {
        return;
    }

    if(stLogList.wLogCnt >= MAX_GVG_LOG_CNT)
    {
        return;
    }

    CFaction *poFaction1 = CFactionMgr::Instance()->GetFaction(dwFactionID1);
    CFaction *poFaction2 = CFactionMgr::Instance()->GetFaction(dwFactionID2);
    if(NULL == poFaction1 || NULL == poFaction2)
    {
        return;
    }

    DT_FACTION_GVG_SINGLE_LOG &stNewLog = stLogList.astLogs[stLogList.wLogCnt++];
    stNewLog.byResult = EGLR_NOT_START;
    stNewLog.qwVideoID    = videoID;
    stNewLog.stRound      = stRound;
    stNewLog.stG1.wZoneID = gsapi::GetZoneID();
    stNewLog.stG2.wZoneID = stNewLog.stG1.wZoneID;
    stNewLog.stG1.byIsWin = 0;
    stNewLog.stG2.byIsWin = 0;

    poFaction1->GetFactionBaseDataCli2(stNewLog.stG1.stFactionInfo);
    poFaction2->GetFactionBaseDataCli2(stNewLog.stG2.stFactionInfo);
}

void CFaction::AddLocalGvGLog(DT_GVG_VIDEO &stVideo)
{
    if(0 == stVideo.qwVideoID)
    {
        return;
    }

    ClearOldGvG();

    CFaction *poFaction1 = CFactionMgr::Instance()->GetFaction(stVideo.stG1.dwFactionID);
    CFaction *poFaction2 = CFactionMgr::Instance()->GetFaction(stVideo.stG2.dwFactionID);
    if(NULL == poFaction1 || NULL == poFaction2)
    {
        return;
    }

    DT_FACTION_GVG_LOG_LIST &stLogList   = m_stFactionGvGData.stGvGLogList;
    DT_FACTION_GVG_SINGLE_LOG *pstOldLog = gvgutil::FindGvGLog(stLogList, stVideo.stRound);
    DT_FACTION_GVG_SINGLE_LOG *pstNewLog = NULL;
    if(pstOldLog)
    {
        pstNewLog = pstOldLog;
    }
    else
    {
        if(stLogList.wLogCnt >= MAX_GVG_LOG_CNT)
        {
            return;
        }

        pstNewLog = &stLogList.astLogs[stLogList.wLogCnt++];
    }

    UINT32 dwFactionID = GetFactionID();
    BOOL bWin = ((dwFactionID == stVideo.stG1.dwFactionID && GR_LEFT_WIN == stVideo.byBattleResult) ||
                 (dwFactionID == stVideo.stG2.dwFactionID && GR_RIGHT_WIN == stVideo.byBattleResult));

    if(bWin)
    {
        pstNewLog->byResult = EGLR_WIN;
    }
    else
    {
        pstNewLog->byResult = EGLR_LOSE;
    }

    pstNewLog->qwVideoID    = stVideo.qwVideoID;
    pstNewLog->stRound      = stVideo.stRound;
    pstNewLog->stG1.wZoneID = gsapi::GetZoneID();
    pstNewLog->stG2.wZoneID = pstNewLog->stG1.wZoneID;
    pstNewLog->stG1.byIsWin = (GR_LEFT_WIN  == stVideo.byBattleResult);
    pstNewLog->stG2.byIsWin = (GR_RIGHT_WIN == stVideo.byBattleResult);
    
    poFaction1->GetFactionBaseDataCli2(pstNewLog->stG1.stFactionInfo);
    poFaction2->GetFactionBaseDataCli2(pstNewLog->stG2.stFactionInfo);
}

void CFaction::AddNoEnemyGvGLog(DT_GVG_ROUND &stRound)
{
    ClearOldGvG();

    DT_FACTION_GVG_LOG_LIST &stLogList = m_stFactionGvGData.stGvGLogList;
    if(stLogList.wLogCnt >= MAX_GVG_LOG_CNT)
    {
        return;
    }

    // 先检测是否已有相同的轮空日志
    if(gvgutil::FindGvGLog(stLogList, stRound))
    {
        return;
    }
    
    DT_FACTION_GVG_SINGLE_LOG &stNewLog = stLogList.astLogs[stLogList.wLogCnt++];
    memset(&stNewLog, 0, sizeof(stNewLog));

    stNewLog.byResult     = EGLR_NO_ENEMY;
    stNewLog.stRound      = stRound;
    stNewLog.stG1.wZoneID = gsapi::GetZoneID();
    stNewLog.stG1.byIsWin = 1;
    GetFactionBaseDataCli2(stNewLog.stG1.stFactionInfo);
}

UINT16 CFaction::GetReadbleGvGLogCnt()
{
    return CGvGBuild::Instance()->GetReadableGvGLogCnt(m_stFactionGvGData.stGvGLogList);
}

void CFaction::ClearOldGvG()
{
    UINT16 wCurSessionID = CGvGBuild::Instance()->GetCurSession();
    if(m_stFactionGvGData.stGvGLogList.wSessionID != wCurSessionID)
    {
        memset(&m_stFactionGvGData.stGvGLogList, 0, sizeof(m_stFactionGvGData.stGvGLogList));

        ResetGvGFactionFunds();
        m_stFactionGvGData.stGvGLogList.wSessionID = wCurSessionID;
    }
}

VOID CFaction::AddContinueKill(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvGRound, UINT8 byBattleIdx, UINT8 byKillNum)
{
    if(0 == byKillNum)
    {
        return;
    }

	CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.find(dwPlayerID);
	if ( itr == m_mapFactionPlayerData.end())
	{
		return ;
	}
	UINT64 qwKey = CGvGGiftMgr::Instance()->GetContinueKillKey(dwPlayerID, wSessionID, byGvGRound, byBattleIdx);
	UINT32 dwValue = CGvGPropMgr::Instance()->GetContinuKillDoorsTribute(byKillNum);
	DT_FACTION_PLAYER_DATA& stPlayerData = itr->second;
	for ( UINT8 byIdx = 0; byIdx < MAX_FACTION_BATTLE_NUM; byIdx++)
	{
		if ( qwKey == stPlayerData.stContinueKillKey.aqwKeyLst[byIdx] )
		{
			break;
		}
		else if ( 0 == stPlayerData.stContinueKillKey.aqwKeyLst[byIdx])
		{
			stPlayerData.stContinueKillKey.aqwKeyLst[byIdx] = qwKey;
			
			stPlayerData.stContinueKillKey.byNum += 1;

            if(!CGvGPropMgr::Instance()->IsTest())
            {
                AddDoorsTribute(dwPlayerID, dwValue, CRecordMgr::EAT_GVG_CONTINUE_KILL, wSessionID, byGvGRound, byBattleIdx, byKillNum );
            }
			
            //门派战报
			vector<UINT64> vec64;
			vec64.push_back(byGvGRound);
			vec64.push_back(byBattleIdx);
			vec64.push_back(byKillNum);
			vec64.push_back(dwValue);
			vector<string> vecStr;
			CPlayerLogMgr::Instance()->AddBattleLog(dwPlayerID, 0, EBLT_GVG, EGLT_CONTINUE_KILL, vec64, vecStr);
			break;
		}
	}    
}

VOID CFaction::AddPlayerBattleLog(const UINT32& dwPlayerID, DT_PLAYER_FACTION_BATTLELOG_DATA& stLogData)
{
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL == poPlayer)
	{
		CMapFactionPlayerDataItr addItr = m_mapFactionPlayerData.find(dwPlayerID);
		if ( addItr != m_mapFactionPlayerData.end())
		{
			if (addItr->second.stUnLoginBattleLogList.byLogNum < MAX_FACTION_BATTLE_LOG_NUM)
			{
				addItr->second.stUnLoginBattleLogList.astPlayerFactionLog[addItr->second.stUnLoginBattleLogList.byLogNum++] = stLogData;
			}
		}
	}
	else
	{
		CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
		if (NULL != poFactionPlayer)
		{
			poFactionPlayer->AddFactionBattleLog(stLogData);
		}
		PKT_CLIGS_PLAYER_FACTION_BATTLELOG_NTF stNtf;
		stNtf.byUnReadLogNum = poFactionPlayer->GetUnReadFactionBattleLogNum();
		stNtf.byLastOperateType = EFBT_AddFaction;
		poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_PLAYER_FACTION_BATTLELOG_NTF);
	}
}

VOID CFaction::AddGvGFactionBattleLog(UINT8 byRound, UINT8 byBattleIdx, GvGLogResult eResult, const TCHAR* poFactionName, BOOL bIsNext)
{
	if ( EGLR_NOT_START == eResult)
	{
		return ;
	}

	vector<UINT64> vec64;
	vector<string> vecStr;
	if ( poFactionName )
	{
		vecStr.push_back(poFactionName);
	}
	vec64.push_back(byRound);
	vec64.push_back(byBattleIdx);
	vec64.push_back(eResult);
	
	if ( EGLR_NO_ENEMY == eResult)
	{
		vec64.push_back( bIsNext ? ESM_YES : ESM_NO);//ESM_YES直接进入下一轮， ESM_NO进入前4
	}
    	
	for(CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin(); itr != m_mapFactionPlayerData.end(); itr++)
	{
		CPlayerLogMgr::Instance()->AddBattleLog(itr->first, 0, EBLT_GVG,  EGLT_GVG_FACTION_BATTLE, vec64, vecStr);
	}
}

VOID CFaction::ResetContinueKill()
{
	CMapFactionPlayerDataItr itr = m_mapFactionPlayerData.begin();
	for ( ; itr != m_mapFactionPlayerData.end(); itr++)
	{
		memset(&itr->second.stContinueKillKey, 0, sizeof(itr->second.stContinueKillKey));
	}
}

//重置GVG数据
VOID CFaction::ResetFactionGvGData()
{
	ResetFormationInfo();
	ResetGvGFactionFunds();
	ResetContinueKill();
}