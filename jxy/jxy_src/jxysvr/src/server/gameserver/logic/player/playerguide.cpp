#include "player.h"


UINT8 CPlayer::m_abyAllGuildFunction[MAX_GUIDE_NUM] = {0};

VOID CPlayer::InitmAllGuildFunction()
{
	static BOOL bHaveInit = FALSE;
	if(bHaveInit)
	{
		return;
	}

	memset(&m_abyAllGuildFunction, 0, sizeof(m_abyAllGuildFunction));

	for(UINT8 byIdx = EGF_INVALID+1; ((byIdx < EGF_MAX) && (byIdx < MAX_GUIDE_NUM)); byIdx++)
	{
		m_abyAllGuildFunction[byIdx] = 1;		
	}
	

	bHaveInit = TRUE;
}


BOOL CPlayer::CKGuide(EGuideFunction eGuideFunction)
{
	if(eGuideFunction > EGF_MAX)
	{
		return FALSE;
	}


	//该功能不需引导
	if(0 == m_abyAllGuildFunction[eGuideFunction])
	{
		return FALSE;
	}
	//若已记录过引导，则不需引导
	if(2 == m_stDT_PLAYER_BASE_DATA.stGuideRecordData.abyGuideRecordData[eGuideFunction])
	{
		return FALSE;
	}

	//记录开启记录
	m_stDT_PLAYER_BASE_DATA.stGuideRecordData.abyGuideRecordData[eGuideFunction] = 1;

	return TRUE;
}


VOID CPlayer::AddGuideRecord(EGuideFunction eGuideFunction)
{
	if(eGuideFunction > EGF_MAX)
	{
		return;
	}

	//记录已经引导
	m_stDT_PLAYER_BASE_DATA.stGuideRecordData.abyGuideRecordData[eGuideFunction] = 2;	

}