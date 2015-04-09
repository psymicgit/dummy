#ifndef _CLIMBTOWER_H_
#define _CLIMBTOWER_H_


#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <common/server/utility.h>

using namespace SGDP;

class CPlayer;
class CClimbTower: public CBaseObj
{
public:
    CClimbTower();
    ~CClimbTower();

public:
    
	//初始化相关对象
    BOOL Init(DT_CLIMB_TOWER_DATA stDT_CLIMB_TOWER_DATA, CPlayer* poOwner);

    DT_CLIMB_TOWER_DATA& GetDT_CLIMB_TOWER_DATA(){return m_stDT_CLIMB_TOWER_DATA;};

	void			GetClimbTowerBaseInfo(UINT16 &wCanResetClimbTownNum, UINT16 & wResetClimbTownNumPerDay);
	UINT16		GetResetClimbTowerGold(UINT8 byResetInstanceNum);
	UINT16		ResetClimbTowerChallengeTime(UINT8 byTownIdx);//重置挑战时间
protected:
	//BOOL			InitClimbTower(DT_CLIMB_TOWER_DATA& stDT_CLIMB_TOWER_DATA);//初始化信息
	//DECLARE_DESTORY(ClimbTower)
	void			RecoveryResetClimbTownNum();
private:
	UINT16		 m_wResetClimbTownNumPerDay;

private:
    DT_CLIMB_TOWER_DATA			m_stDT_CLIMB_TOWER_DATA;//
};

DECLARE_CLASS_POOL_MGR(CClimbTower);

#endif
