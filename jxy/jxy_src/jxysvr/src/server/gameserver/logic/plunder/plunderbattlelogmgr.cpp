
#include "plunderbattlelogmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <protocol/server/protocommondata.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playermgr.h>
#include <logic/event/timereventmgr.h>

using namespace SGDP;



IMPLEMENT_SINGLETON(CPlunderBattleLogMgr)

CPlunderBattleLogMgr::CPlunderBattleLogMgr()
{

}

CPlunderBattleLogMgr::~CPlunderBattleLogMgr()
{

}


BOOL CPlunderBattleLogMgr::Init()
{

	return TRUE;
}


VOID CPlunderBattleLogMgr::UnInit()
{

}

CPlunderBattleLog* CPlunderBattleLogMgr::CreateBattleLog()
{
	return m_oPlunderBattleLogShmemObjMgr.Alloc();
}

VOID CPlunderBattleLogMgr::RemoveBattleLog(CPlunderBattleLog* poPlunderBattleLog)
{
	m_oPlunderBattleLogShmemObjMgr.Free(poPlunderBattleLog);
}


