
#ifndef _BOSSBBATTLELOG_H_
#define _BOSSBBATTLELOG_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocol/server/protocligs.h>

class CPlayer;
class CBossBattle;
class CBossBBattleLog:public CBaseObj
{
public:
	CBossBBattleLog();
	~CBossBBattleLog();
public:
	BOOL Init(const DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA* pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA, CPlayer* poOwner, CBossBattle* poBossBattle);
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA& GetDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA() { return m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA; }
	VOID GetDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI(DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI& stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI);
protected:

private:
	DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA	m_stDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA;
	CBossBattle*							m_poBossBattle;
};


#endif

