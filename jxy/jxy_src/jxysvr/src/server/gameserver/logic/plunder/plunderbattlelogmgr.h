
#ifndef _PLUNDERBATTLELOGMGR_H_
#define _PLUNDERBATTLELOGMGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <common/client/commondef.h>
#include <logic/plunder/plunderbattlelog.h>

using namespace SGDP;
using namespace std;

typedef HashMap<UINT64, CPlunderBattleLog*> CID2PlunderBattleLogMap;
typedef CID2PlunderBattleLogMap::iterator	CID2PlunderBattleLogMapItr;


class CPlunderBattleLogMgr
{

public:
    DECLARE_SINGLETON(CPlunderBattleLogMgr)

    BOOL Init();

    VOID UnInit();

public:

	CPlunderBattleLog*	CreateBattleLog();
	VOID				RemoveBattleLog(CPlunderBattleLog* poPlunderBattleLog);

public:
	
protected:
    CPlunderBattleLogMgr();
    ~CPlunderBattleLogMgr();


private:
	CSDObjectPool<CPlunderBattleLog, CSDMutex>	m_oPlunderBattleLogShmemObjMgr;	
};



#endif

