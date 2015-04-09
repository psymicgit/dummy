
#ifndef _ENEMYMGR_H_
#define _ENEMYMGR_H_

#include "enemy.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <common/client/commondef.h>


using namespace SGDP;
using namespace std;

typedef HashMap<UINT64, CEnemy*> CID2EnemyMap;
typedef CID2EnemyMap::iterator	CID2EnemyMapItr;

class CEnemyMgr
{

public:
    DECLARE_SINGLETON(CEnemyMgr)

    BOOL Init();

    VOID UnInit();

public:
	CEnemy*	CreateEnemy(UINT32 dwPlayerID, UINT32 dwEnemyPlayerID);
	VOID	RemoveEnemy(UINT32 dwPlayerID, UINT32 dwEnemyPlayerID);

	
protected:

protected:
    CEnemyMgr();
    ~CEnemyMgr();


private:
	CID2EnemyMap					m_mapEnemy;
	CSDObjectPool<CEnemy, CSDMutex>	m_oEnemyShmemObjMgr;
};



#endif

