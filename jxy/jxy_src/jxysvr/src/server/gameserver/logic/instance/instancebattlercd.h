
#ifndef _INSTANCEBATTLERCD_H_
#define _INSTANCEBATTLERCD_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>

using namespace std ;

class CPlayer;

//副本最大战斗记录
class CInstanceBattleRcd:public CBaseObj
{
public:
	CInstanceBattleRcd();
	~CInstanceBattleRcd();
public:
	
	BOOL Init(DT_INSTANCE_BATTLE_DATA& stDT_INSTANCE_BATTLE_DATA, CPlayer* poOwner);
	DT_INSTANCE_BATTLE_DATA& GetDT_INSTANCE_BATTLE_DATA() { return m_stBattleRcd;}
	VOID OnInstanceBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx,UINT8 byBattleIdx);
	BOOL CkFirstBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIndex);////检查该战斗是否是该副本的第一次
private:
	DT_INSTANCE_BATTLE_DATA			m_stBattleRcd;
};



#endif //#ifndef 

