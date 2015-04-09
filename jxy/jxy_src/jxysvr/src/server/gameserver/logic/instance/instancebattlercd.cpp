#include "instancebattlercd.h"
#include "instance.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>


CInstanceBattleRcd::CInstanceBattleRcd()
{
	memset(&m_stBattleRcd, 0, sizeof(m_stBattleRcd));
}

CInstanceBattleRcd::~CInstanceBattleRcd()
{

}

BOOL CInstanceBattleRcd::Init(DT_INSTANCE_BATTLE_DATA& stDT_INSTANCE_BATTLE_DATA, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}

	memcpy(&m_stBattleRcd, &stDT_INSTANCE_BATTLE_DATA, sizeof(DT_INSTANCE_BATTLE_DATA));
	m_poOwner = poOwner;

	return TRUE;
}

VOID CInstanceBattleRcd::OnInstanceBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx,UINT8 byBattleIdx)
{

	if((wSceneIdx != m_stBattleRcd.stInstanceID.wSceneIdx) && (0 != m_stBattleRcd.stInstanceID.wSceneIdx))
	{
		return;
	}

	//只记录最大的(比当前最大记录还大的)
	//第一次战斗
	if(CkFirstBattle(wSceneIdx, byTownIdx, byInstanceIdx, byBattleIdx))
	{
		m_stBattleRcd.stInstanceID.wSceneIdx = wSceneIdx;
		m_stBattleRcd.stInstanceID.byTownIdx = byTownIdx;
		m_stBattleRcd.stInstanceID.byInstanceIdx = byInstanceIdx;
		m_stBattleRcd.byBattleIndex = byBattleIdx;
		m_stBattleRcd.qwBattleTime = SDTimeSecs();
		//SetSaveState(ESS_WANT_SAVE);
	}
	
}



BOOL CInstanceBattleRcd::CkFirstBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIndex)
{
	if((wSceneIdx != m_stBattleRcd.stInstanceID.wSceneIdx) && (0 != m_stBattleRcd.stInstanceID.wSceneIdx))
	{
		return FALSE;
	}

	//第一次战斗
	if((byTownIdx > m_stBattleRcd.stInstanceID.byTownIdx) || 
		((byTownIdx == m_stBattleRcd.stInstanceID.byTownIdx) && (byInstanceIdx > m_stBattleRcd.stInstanceID.byInstanceIdx)) || 
		((byTownIdx == m_stBattleRcd.stInstanceID.byTownIdx) && (byInstanceIdx == m_stBattleRcd.stInstanceID.byInstanceIdx) && (byBattleIndex > m_stBattleRcd.byBattleIndex)))
	{
		return TRUE;
	}		


	return FALSE;
}