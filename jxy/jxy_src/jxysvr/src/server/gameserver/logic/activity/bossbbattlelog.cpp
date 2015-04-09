#include "bossbbattlelog.h"
#include "bossbattle.h"
#include "bossbattlemgr.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>


CBossBBattleLog::CBossBBattleLog()
{
	memset(&m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA, 0, sizeof(m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA));
	m_poBossBattle = NULL;
}


CBossBBattleLog::~CBossBBattleLog()
{

}

BOOL CBossBBattleLog::Init(const DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA* pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA, CPlayer* poOwner, CBossBattle* poBossBattle)
{
	if((NULL == pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA) || (NULL == poOwner) || (NULL == poBossBattle))
	{
		return FALSE;
	}
	memcpy(&m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA, pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA, sizeof(DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA));

	m_poOwner = poOwner;
	m_poBossBattle = poBossBattle;

	return TRUE;
}

VOID CBossBBattleLog::GetDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI(DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI& stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI)
{
	stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI.qwOverTime = m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.qwOverTime;
	memcpy(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI.stMyHurtInfo, &m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stMyHurtInfo, sizeof(m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stMyHurtInfo));
	memcpy(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI.stHurtEncInfo, &m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stHurtEncInfo, sizeof(m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stHurtEncInfo));
	memcpy(&stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI.stHeroEncInfo, &m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stHeroEncInfo, sizeof(m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.stHeroEncInfo));
	CBossBattleMgr::Instance()->GetBattleLog(m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA.qwOverTime, stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI.wBossID, stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI.astBossBTopEncInfo, stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI.stBossBKillEncInfo);
}

