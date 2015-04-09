
#ifndef _RACEBATTLELOGMGR_H_
#define _RACEBATTLELOGMGR_H_

#include "racebattlelog.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

using namespace SGDP;
using namespace std;


typedef HashMap<UINT64, CRaceBattleLog*> CID2RaceBattleLogMap;
typedef CID2RaceBattleLogMap::iterator	CID2RaceBattleLogMapItr;

class CRaceBattleLogMgr
{
public:
    DECLARE_SINGLETON(CRaceBattleLogMgr)

    BOOL Init();

    VOID UnInit();

public:
	CRaceBattleLog*	CreateBattleLog();
	VOID		RemoveBattleLog(CRaceBattleLog* poRaceBattleLog);

	
protected:
    CRaceBattleLogMgr();
    ~CRaceBattleLogMgr();


private:	
	CSDObjectPool<CRaceBattleLog, CSDMutex>	m_oRaceBattleLogShmemObjMgr;
};



#endif

