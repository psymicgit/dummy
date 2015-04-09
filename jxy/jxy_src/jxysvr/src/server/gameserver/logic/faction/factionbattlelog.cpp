#include "factionbattlelog.h"
#include "logic/player/player.h"
#include <logic/faction/factionmgr.h>
#include "faction.h"


BOOL CFactionBattleLog::Init(DT_PLAYER_FACTION_BATTLELOG_DATA_LIST &stFactionBattleLog, CPlayer* poOwner)
{
    memset(&m_stFactionBattleLog, 0, sizeof(DT_PLAYER_FACTION_BATTLELOG_DATA_LIST));
    for (UINT32 dwIndex = 0; dwIndex < stFactionBattleLog.byLogNum && dwIndex < MAX_FACTION_BATTLE_LOG_NUM; dwIndex++)
    {
        if (stFactionBattleLog.astPlayerFactionLog[dwIndex].byNewFlag == 1)
        {
            m_byUnReadLogNum += 1;
        }
        m_deqFactionBattleLog.push_back(stFactionBattleLog.astPlayerFactionLog[dwIndex]);
    }
	m_poOwner = poOwner;
	//Î´¶ÁÕ½±¨
	CFaction* poFaction = GetFaction();
	if ( NULL != poFaction)
	{
		DT_FACTION_PLAYER_DATA* stPlayerData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
		if (NULL != stPlayerData)
		{
			for( UINT8 byIndex = 0; byIndex < stPlayerData->stUnLoginBattleLogList.byLogNum; byIndex++)
			{
				AddFactionBattleLog(stPlayerData->stUnLoginBattleLogList.astPlayerFactionLog[byIndex]);
			}
			memset(&stPlayerData->stUnLoginBattleLogList, 0, sizeof(stPlayerData->stUnLoginBattleLogList));
		}
	}
    
    return TRUE;
}

CFaction* CFactionBattleLog::GetFaction()
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
	if ( !bRet )
	{
		return NULL;
	}
	return CFactionMgr::Instance()->GetFaction(dwFactionID);
}

VOID CFactionBattleLog::AddFactionBattleLog(DT_PLAYER_FACTION_BATTLELOG_DATA& stFactionBattleLog)
{
    while ( MAX_FACTION_BATTLE_LOG_NUM < m_deqFactionBattleLog.size() )
    {
        m_deqFactionBattleLog.pop_back();
    }
    m_byUnReadLogNum += 1;
    m_deqFactionBattleLog.push_front(stFactionBattleLog);
}

DT_PLAYER_FACTION_BATTLELOG_DATA_LIST& CFactionBattleLog::GetDT_PLAYER_FACTION_BATTLELOG_DATA_LIST()
{
    memset(&m_stFactionBattleLog, 0, sizeof(m_stFactionBattleLog));
    m_stFactionBattleLog.byLogNum = m_deqFactionBattleLog.size();
    UINT32 dwIndex = 0;
    CFactionBattleDeq::iterator it = m_deqFactionBattleLog.begin();
    for ( ; it != m_deqFactionBattleLog.end() && dwIndex < MAX_FACTION_BATTLE_LOG_NUM; it++, dwIndex++)
    {
        memcpy(&m_stFactionBattleLog.astPlayerFactionLog[dwIndex], &(*it), sizeof(DT_PLAYER_FACTION_BATTLELOG_DATA));
    }
    return m_stFactionBattleLog;
}


VOID CFactionBattleLog::GetBattleLogByCli(UINT8& byLogNum, DT_PLAYER_FACTION_BATTLELOG_DATA astPlayerFactionLogList[MAX_FACTION_BATTLE_LOG_NUM])
{
    m_byUnReadLogNum = 0;
    byLogNum = m_deqFactionBattleLog.size();
    UINT32 dwIndex = 0;
    CFactionBattleDeq::iterator it = m_deqFactionBattleLog.begin();
    for ( ; it != m_deqFactionBattleLog.end() && dwIndex < MAX_FACTION_BATTLE_LOG_NUM; it++, dwIndex++)
    {
        memcpy(&astPlayerFactionLogList[dwIndex], &(*it), sizeof(DT_PLAYER_FACTION_BATTLELOG_DATA));
        it->byNewFlag = 0;
    }
}