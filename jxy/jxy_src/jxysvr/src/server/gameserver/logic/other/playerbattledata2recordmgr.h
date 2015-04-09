#ifndef _PLAYER_BATTLE_DATA2_RECORD_MGR_H_
#define _PLAYER_BATTLE_DATA2_RECORD_MGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>
#include <sddb.h>
#include <vector>
#include <db/recordbattledata2.h>

class CPlayerBattleData2RecordMgr
{
public:
	DECLARE_SINGLETON(CPlayerBattleData2RecordMgr);
	BOOL Init();

	VOID UnInit();

	BOOL RecordBattle( const DT_BATTLE_DATA5 &battleData, UINT64 &qwIndex, UINT32 dwPlayerSelf, UINT32 dwPlayerEnemy );
	BOOL GetBattleData( UINT32 dwPlayerID, UINT64 qwBattleIndex );
protected:
	CPlayerBattleData2RecordMgr();
	~CPlayerBattleData2RecordMgr();

	BOOL InitDataFromDB();
private:
	UINT64 m_qwIndex;
};

#endif