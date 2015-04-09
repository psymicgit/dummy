#include "plunderbattlelog.h"
#include "plunder.h"
#include <logic/player/player.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/client/errdef.h>


CPlunderBattleLog::CPlunderBattleLog()
{
	memset(&m_stDT_PLUNDER_BATTLELOG_DATA, 0, sizeof(m_stDT_PLUNDER_BATTLELOG_DATA));
	m_poPlunder = NULL;
}


CPlunderBattleLog::~CPlunderBattleLog()
{

}

BOOL CPlunderBattleLog::Init(const DT_PLUNDER_BATTLELOG_DATA* pstDT_PLUNDER_BATTLELOG_DATA, CPlayer* poOwner, CPlunder* poPlunder)
{
	if((NULL == pstDT_PLUNDER_BATTLELOG_DATA) || (NULL == poOwner) || (NULL == poPlunder))
	{
		return FALSE;
	}
	memcpy(&m_stDT_PLUNDER_BATTLELOG_DATA, pstDT_PLUNDER_BATTLELOG_DATA, sizeof(DT_PLUNDER_BATTLELOG_DATA));

	m_poOwner = poOwner;
	m_poPlunder = poPlunder;

	return TRUE;
}

VOID CPlunderBattleLog::GetDT_PLUNDER_BATTLELOG_DATA_CLI(DT_PLUNDER_BATTLELOG_DATA_CLI& stDT_PLUNDER_BATTLELOG_DATA_CLI)
{
	memcpy(&stDT_PLUNDER_BATTLELOG_DATA_CLI.stBattelogInfo, &m_stDT_PLUNDER_BATTLELOG_DATA, sizeof(DT_PLUNDER_BATTLELOG_DATA));
	const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_PLUNDER_BATTLELOG_DATA.dwActivePlayerID);
	if(pstPlayerBaseData)
	{
		SDStrcpy(stDT_PLUNDER_BATTLELOG_DATA_CLI.aszActiveDispName, pstPlayerBaseData->aszDispName);
	}
	const SPlayerBaseData* pstPlayerBaseData2 = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_PLUNDER_BATTLELOG_DATA.dwPassivePlayerID);
	if(pstPlayerBaseData2)
	{
		SDStrcpy(stDT_PLUNDER_BATTLELOG_DATA_CLI.aszPassiveDispName, pstPlayerBaseData2->aszDispName);
	}
	const SPlayerBaseData* pstPlayerBaseData3 = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_PLUNDER_BATTLELOG_DATA.dwCapturePlayerID);
	if(pstPlayerBaseData3)
	{
		SDStrcpy(stDT_PLUNDER_BATTLELOG_DATA_CLI.aszCaptureDispName, pstPlayerBaseData3->aszDispName);
	}
}

