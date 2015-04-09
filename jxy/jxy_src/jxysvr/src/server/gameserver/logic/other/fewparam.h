
#ifndef _FEWPARAM_H_
#define _FEWPARAM_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>
using namespace std ;

class CPlayer;
class CFewParam:public CBaseObj
{
public:
	CFewParam();
	~CFewParam();
public:
	//初始化相关对象
	BOOL Init(DT_FEW_PARAM_DATA &stDT_FEW_PARAM_DATA, CPlayer* poOwner);
	DT_FEW_PARAM_DATA& GetDT_FEW_PARAM_DATA();
	VOID CheckCountTime();
	VOID AddGVEPassNum();
	VOID AddRaceSuccess();
	VOID AddRaceFailed();
	VOID AddTrialSuccess();
	VOID AddTrialFailed();
	VOID AddInterceptSuccess();
	VOID AddInterceptFailed();
	VOID AddAwakenNum();
	VOID AddResetTowerNum();
	VOID AddWorldPassNum(UINT8);
	VOID AddHellPassNum(UINT8);
	VOID AddHeavenPassNum(UINT8);
	VOID AddXiJingMaster();
	VOID AddExperienceDrug();
	VOID AddPassNeiDanInstance();
private:
	DT_FEW_PARAM_DATA	m_stDT_FEW_PARAM_DATA;

};
DECLARE_CLASS_POOL_MGR(CFewParam)

#endif //#ifndef 

