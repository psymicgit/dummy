

#ifndef _BOSSBATLE_H_
#define _BOSSBATLE_H_

#include <sdhashmap.h>
#include <logic/base/baseobj.h>
#include <logic/shmem/shmemmap.h>
#include <protocommondata.h>
#include "bossbattlepropmgr.h"
#include "activity.h"
#include <logic/base/basedefine.h>
#include <common/server/utility.h>

using namespace std ;

class CPlayer;
class CBossBBattleLog;

typedef map<UINT64, CBossBBattleLog*> CBossBBattleLogMap;
typedef CBossBBattleLogMap::iterator CBossBBattleLogMapItr;
typedef CBossBBattleLogMap::reverse_iterator CBossBBattleLogMapRitr;

struct tagDT_FUNC_COMMON_PROMPT;
typedef tagDT_FUNC_COMMON_PROMPT DT_FUNC_COMMON_PROMPT;


struct tagDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI;
typedef tagDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI;

class CBossBattle:public CActivity
{
public:
	CBossBattle();
	~CBossBattle();
public:	
	BOOL	Init(DT_BOSS_BATTLE_PLAYER_DATA_INFO& stDT_BOSS_BATTLE_PLAYER_DATA_INFO, CPlayer* poOwner);
	inline DT_BOSS_BATTLE_PLAYER_DATA& GetDT_BOSS_BATTLE_PLAYER_DATA() { return m_stDT_BOSS_BATTLE_PLAYER_DATA; }
	inline UINT64& GetLastMoveTime() { return m_qwLastMoveTime; }
	inline C3232Map& GetBeattentionIDMap() { return m_mapBeattentionID; }
	//VOID	OnAddBeattaction(UINT32 dwPlayerID) { m_mapBeattentionID[dwPlayerID] = dwPlayerID; }
	VOID	OnAddBeattaction(UINT32 dwPlayerID, BOOL bFirst = FALSE);
	VOID	OnDelBeattaction(UINT32 dwPlayerID);
	virtual VOID GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA);
	BOOL	GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt); //有且未完成活动则返回TRUE

	BOOL	AddBattleLog(DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA* pstDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA);
	CBossBBattleLogMap& GetBattleLogMap() { return m_mapBossBBattleLog; }
	UINT16	GetShowBossBBattleLog(UINT8& byNum, DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_CLI astBossBattleBL[MAX_BOSSB_BATTLE_LOG_NUM]);

	///////////
	VOID	MoveID(UINT32 dwPlayerID);
	VOID	HurtID(UINT32 dwPlayerID, UINT32 dwHurtValue);
	VOID	LeaveID(UINT32 dwPlayerID);
	VOID	InitEnterData();
	VOID	EndSync();
	inline UINT64&	GetLastDealTime() { return m_qwLastDealTime; }
	inline C3232Map& GetEnterID() { return m_mapEnterID; }
	inline C3232Map& GetLeaveID() { return m_mapLeaveID; }
	inline C3232Map& GetMoveID() { return m_mapMoveID; }
	inline C3232Map& GetHurtID() { return m_mapHurtID; }

protected:	
	BOOL InitBattleLog(const DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST& stBossBBattleLog);
private:
	DT_BOSS_BATTLE_PLAYER_DATA	m_stDT_BOSS_BATTLE_PLAYER_DATA;

	CBossBBattleLogMap			m_mapBossBBattleLog;
	UINT64		m_qwLastMoveTime;//上次移动时间
	C3232Map	m_mapBeattentionID;//关注ta的玩家
	C3232Map	m_mapEnterID;//进入的ID
	C3232Map	m_mapLeaveID;//离开的ID
	C3232Map	m_mapMoveID;//移动的ID	
	C3232Map	m_mapHurtID;//伤害的ID，key为ID，value为伤害值
	UINT64		m_qwLastDealTime;//单位毫秒
	
};
DECLARE_CLASS_POOL_MGR(CBossBattle);

#endif //#ifndef 

