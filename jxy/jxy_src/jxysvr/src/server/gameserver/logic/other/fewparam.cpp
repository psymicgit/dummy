#include "fewparam.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>

IMPLEMENT_CLASS_POOL_MGR(CFewParam)
CFewParam::CFewParam()
{
	memset(&m_stDT_FEW_PARAM_DATA, 0 ,sizeof(m_stDT_FEW_PARAM_DATA));
}

CFewParam::~CFewParam()
{

}

BOOL CFewParam::Init(DT_FEW_PARAM_DATA &stDT_FEW_PARAM_DATA, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}

	memcpy(&m_stDT_FEW_PARAM_DATA, &stDT_FEW_PARAM_DATA, sizeof(m_stDT_FEW_PARAM_DATA));
	m_poOwner = poOwner;

	return TRUE;
}
DT_FEW_PARAM_DATA& CFewParam::GetDT_FEW_PARAM_DATA()
{ 
	CheckCountTime();
	return m_stDT_FEW_PARAM_DATA; 
}
VOID CFewParam::CheckCountTime()
{
	if ( !IsToday(m_stDT_FEW_PARAM_DATA.qwCountTime))
	{
		m_stDT_FEW_PARAM_DATA.wAwakenNum = 0;
		m_stDT_FEW_PARAM_DATA.wGVETimes = 0;
		m_stDT_FEW_PARAM_DATA.wInterceptFailed = 0;
		m_stDT_FEW_PARAM_DATA.wInterceptSuccess = 0;
		m_stDT_FEW_PARAM_DATA.wTrialSuccess = 0;
		m_stDT_FEW_PARAM_DATA.wTrialFailed = 0;
		m_stDT_FEW_PARAM_DATA.wRaceSuccess = 0;
		m_stDT_FEW_PARAM_DATA.wRaceFailed = 0;
		m_stDT_FEW_PARAM_DATA.wResetTowerNum = 0;
		m_stDT_FEW_PARAM_DATA.wWorldPassNum = 0;
		m_stDT_FEW_PARAM_DATA.wHeavenPassNum = 0;
		m_stDT_FEW_PARAM_DATA.wHellPassNum = 0;
		m_stDT_FEW_PARAM_DATA.wXiJingFirst = 0;
		m_stDT_FEW_PARAM_DATA.wXiJingHorse = 0;
		m_stDT_FEW_PARAM_DATA.wXiJingMaster = 0;
		m_stDT_FEW_PARAM_DATA.wXiJingSecond = 0;
		m_stDT_FEW_PARAM_DATA.wXiJingThird = 0;
		m_stDT_FEW_PARAM_DATA.wExperienceDrug = 0;
		m_stDT_FEW_PARAM_DATA.wPassNaiDanNum = 0;
		m_stDT_FEW_PARAM_DATA.qwCountTime = SDTimeSecs();
	}
}

VOID CFewParam::AddGVEPassNum()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wGVETimes++;
}
VOID CFewParam::AddRaceSuccess()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wRaceSuccess++;
}
VOID CFewParam::AddRaceFailed()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wRaceFailed++;
}
VOID CFewParam::AddTrialSuccess()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wTrialSuccess++;
}

VOID CFewParam::AddTrialFailed()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wTrialFailed++;
}
VOID CFewParam::AddInterceptSuccess()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wInterceptSuccess++;
}
VOID CFewParam::AddInterceptFailed()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wInterceptFailed++;
}
VOID CFewParam::AddAwakenNum()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wAwakenNum++;
}

VOID CFewParam::AddResetTowerNum()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wResetTowerNum++;
}

VOID CFewParam::AddWorldPassNum(UINT8 byNum)
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wWorldPassNum += byNum;
}
VOID CFewParam::AddHellPassNum(UINT8 byNum)
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wHellPassNum += byNum;
}
VOID CFewParam::AddHeavenPassNum(UINT8 byNum)
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wHeavenPassNum += byNum;
}

VOID CFewParam::AddXiJingMaster()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wXiJingMaster += 1;
}

VOID CFewParam::AddExperienceDrug()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wExperienceDrug += 1;
}

VOID CFewParam::AddPassNeiDanInstance()
{
	CheckCountTime();
	m_stDT_FEW_PARAM_DATA.wPassNaiDanNum += 1;
}