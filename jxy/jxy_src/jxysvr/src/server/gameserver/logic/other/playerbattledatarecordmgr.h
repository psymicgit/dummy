#ifndef _PLAYER_BATTLE_DATA_RECORD_MGR_H_
#define _PLAYER_BATTLE_DATA_RECORD_MGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>
#include <sddb.h>
#include <vector>
#include <db/recordbattledata.h>

class CPlayerBattleDataRecordMgr
{
public:
	DECLARE_SINGLETON(CPlayerBattleDataRecordMgr);
	BOOL Init();

	VOID UnInit();

	BOOL RecordBattle( const DT_BATTLE_DATA2 &battleData, UINT64 &qwIndex, UINT32 dwPlayerSelf, UINT32 dwPlayerEnemy );
	BOOL GetBattleData( UINT32 dwPlayerID, UINT64 qwBattleIndex );
protected:
	CPlayerBattleDataRecordMgr();
	~CPlayerBattleDataRecordMgr();

	BOOL InitDataFromDB();
private:
	UINT64 m_qwIndex;
};

#endif