#include "toluafunc.h"
#include <sdtime.h>
#include <sdstring.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/race/racemgr.h>
#include <logic/player/player.h>
#include <common/server/utility.h>


using namespace SGDP;

UINT64 GetTimeValue(const CHAR* pszDateTime)
{
	if(NULL == pszDateTime)
	{
		return SDTimeSecs();
	}

	CSDDateTime oTime;
	if(SDTimeFromString(pszDateTime, oTime))
	{
		return oTime.GetTimeValue();
	}
	
	return SDTimeSecs();
}

//获取排名，0表示没有排名
UINT32 GetRank(CPlayer* poPlayer)
{
	if(NULL == poPlayer)
	{
		return 0;
	}

	CRace* poRace = poPlayer->GetRace();
	if(NULL == poRace)
	{
		return 0;
	}

	return poRace->GetDT_RACE_BASE_DATA().dwRank;
}

INT32 LuaRandomReseed(UINT32 dwRange)
{
	return RandomReseed(dwRange);
}
