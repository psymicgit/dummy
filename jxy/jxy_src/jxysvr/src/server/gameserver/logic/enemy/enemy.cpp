#include "enemy.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playerbasedatacachemgr.h>


CEnemy::CEnemy()
{

}

CEnemy::~CEnemy()
{

}

BOOL CEnemy::Init(DT_ENEMY_DATA* pstDT_ENEMY_DATA, CPlayer* poOwner)
{
	if((NULL == pstDT_ENEMY_DATA) || (NULL == poOwner))
	{
		return FALSE;
	}
	memcpy(&m_stDT_ENEMY_DATA, pstDT_ENEMY_DATA, sizeof(DT_ENEMY_DATA));

	m_poOwner = poOwner;

	return TRUE;
}

BOOL CEnemy::GetDT_ENEMY_DATA_CLI(DT_ENEMY_DATA_CLI& stDT_ENEMY_DATA_CLI)
{
	const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_ENEMY_DATA.dwEnemyPlayerID);
	if(NULL == pstPlayerBaseData)
	{
		return FALSE;
	}

	stDT_ENEMY_DATA_CLI.dwEnemyPlayerID = pstPlayerBaseData->dwID;
	SDStrcpy(stDT_ENEMY_DATA_CLI.aszEnemyDispName, pstPlayerBaseData->aszDispName);
	stDT_ENEMY_DATA_CLI.wCareerID = pstPlayerBaseData->wMainHeroKindID;
	stDT_ENEMY_DATA_CLI.wEnemyLevel = pstPlayerBaseData->wLevel;
	stDT_ENEMY_DATA_CLI.wBattleNum = m_stDT_ENEMY_DATA.wBattleNum;
	stDT_ENEMY_DATA_CLI.dwPower = pstPlayerBaseData->dwPower;
	if(0 != pstPlayerBaseData->dwCaptureOwner)
	{
		stDT_ENEMY_DATA_CLI.byHaveOwnerFlag = 1;
		DT_BECAPTURE_DATA& stDT_BECAPTURE_DATA = stDT_ENEMY_DATA_CLI.astOwnerInfo[0];
		const SPlayerBaseData* pstOwnerProp = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(pstPlayerBaseData->dwCaptureOwner);
		if(pstOwnerProp)
		{
			stDT_BECAPTURE_DATA.dwOwnerID = pstOwnerProp->dwID;
			SDStrcpy(stDT_BECAPTURE_DATA.aszOwnerDispName, pstOwnerProp->aszDispName);			
			stDT_BECAPTURE_DATA.wOwnerLevel = pstOwnerProp->wLevel;
			stDT_BECAPTURE_DATA.dwOwnerPower = pstOwnerProp->dwPower;
			stDT_BECAPTURE_DATA.dwReleaseCountdown = 0;//在次不关心
		}	
	}
	

	return TRUE;
}