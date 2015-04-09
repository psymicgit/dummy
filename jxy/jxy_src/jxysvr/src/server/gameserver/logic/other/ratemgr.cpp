#include "ratemgr.h"



IMPLEMENT_SINGLETON(CRateMgr);

CRateMgr::CRateMgr()
{

}

CRateMgr::~CRateMgr()
{
}

BOOL CRateMgr::Init()
{
	InitRate();

	return TRUE;
}

VOID CRateMgr::UnInit()
{
}


VOID CRateMgr::InitRate()
{
	for(UINT8 byNum = 0; byNum < MAX_RATEVEC_NUM; byNum++)
	{
		for(UINT8 byIdxRate = 0; byIdxRate <= 100; byIdxRate++)
		{
			vector<UINT8>& vecRadom = m_aavecRadom[byNum][byIdxRate];
			InitRadomQue(byIdxRate, vecRadom);
		}
	}
}


vector<UINT8>& CRateMgr::GetRateVec(UINT16 wRate)
{
	wRate = wRate > 100 ? 100 : wRate;
	UINT8 byRate = (UINT8)wRate;

	UINT8 byHit = RandomReseed(MAX_RATEVEC_NUM);
	return m_aavecRadom[byHit][byRate];
}



