
#include "enemymgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <protocol/server/protocommondata.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/player/player.h>
#include <logic/base/logicfunc.h>
#include <common/client/errdef.h>


using namespace SGDP;



IMPLEMENT_SINGLETON(CEnemyMgr)

CEnemyMgr::CEnemyMgr()
{

}

CEnemyMgr::~CEnemyMgr()
{

}


BOOL CEnemyMgr::Init()
{

    return TRUE;
}


VOID CEnemyMgr::UnInit()
{
    m_mapEnemy.clear();
}

CEnemy* CEnemyMgr::CreateEnemy(UINT32 dwPlayerID, UINT32 dwEnemyPlayerID)
{
    UINT64 qwKey = CLogicFunc::Get3232Key(dwPlayerID, dwEnemyPlayerID);
	CID2EnemyMapItr itr = m_mapEnemy.find(qwKey);
	if (itr != m_mapEnemy.end())
	{
		return itr->second;
	}

    CEnemy* poEnemy = m_oEnemyShmemObjMgr.Alloc();
    if(NULL == poEnemy)
    {
        return NULL;
    }
    m_mapEnemy[qwKey] = poEnemy;
    return poEnemy;
}

VOID CEnemyMgr::RemoveEnemy(UINT32 dwPlayerID, UINT32 dwEnemyPlayerID)
{
	UINT64 qwKey = CLogicFunc::Get3232Key(dwPlayerID, dwEnemyPlayerID);
	CID2EnemyMapItr itr = m_mapEnemy.find(qwKey);
    if (m_mapEnemy.end() != itr)
    {
        m_oEnemyShmemObjMgr.Free(itr->second);
        m_mapEnemy.erase(itr);

        //DBG_INFO( _SDT( "[%s: %d]: Remove Enemy :%d OK£¡" ), MSG_MARK, dwEnemyPlayerID );

        return;
    }
    //SYS_CRITICAL( _SDT( "[%s: %d]: Remove Enemy:%d Error£¡" ), MSG_MARK, dwEnemyPlayerID );
}

