#include "racebattlelog.h"
#include <logic/player/player.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/client/errdef.h>


CRaceBattleLog::CRaceBattleLog()
{
	memset(&m_stDT_RACE_BATTLELOG_DATA, 0, sizeof(m_stDT_RACE_BATTLELOG_DATA));
}


CRaceBattleLog::~CRaceBattleLog()
{

}

BOOL CRaceBattleLog::Init(DT_RACE_BATTLELOG_DATA* pstDT_RACE_BATTLELOG_DATA, CPlayer* poOwner)
{
	if((NULL == pstDT_RACE_BATTLELOG_DATA) || (NULL == poOwner))
	{
		return FALSE;
	}
	memcpy(&m_stDT_RACE_BATTLELOG_DATA, pstDT_RACE_BATTLELOG_DATA, sizeof(m_stDT_RACE_BATTLELOG_DATA));

	m_poOwner = poOwner;

	return TRUE;
}


VOID CRaceBattleLog::GetDT_RACE_BATTLELOG_DATA_CLI(DT_RACE_BATTLELOG_DATA_CLI& stDT_RACE_BATTLELOG_DATA_CLI)
{
	memcpy(&stDT_RACE_BATTLELOG_DATA_CLI.stBattlelogInfo, &m_stDT_RACE_BATTLELOG_DATA, sizeof(DT_RACE_BATTLELOG_DATA));
	const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_RACE_BATTLELOG_DATA.dwEnemyPlayerID);
	if(pstPlayerBaseData)
	{
		SDStrcpy(stDT_RACE_BATTLELOG_DATA_CLI.aszEnemyDispName, pstPlayerBaseData->aszDispName);
	}
	
}
