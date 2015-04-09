
#include "racebattlelogmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <protocol/server/protocommondata.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>


using namespace SGDP;



IMPLEMENT_SINGLETON(CRaceBattleLogMgr)

CRaceBattleLogMgr::CRaceBattleLogMgr()
{

}

CRaceBattleLogMgr::~CRaceBattleLogMgr()
{
	
}


BOOL CRaceBattleLogMgr::Init()
{
	
	return TRUE;
}


VOID CRaceBattleLogMgr::UnInit()
{

}

CRaceBattleLog*	CRaceBattleLogMgr::CreateBattleLog()
{
	return m_oRaceBattleLogShmemObjMgr.Alloc();
}

VOID CRaceBattleLogMgr::RemoveBattleLog(CRaceBattleLog* poRaceBattleLog)
{
	m_oRaceBattleLogShmemObjMgr.Free(poRaceBattleLog);
}

