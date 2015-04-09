
#ifndef _BEG_BLUE_H_
#define _BEG_BLUE_H_

#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>

using namespace std ;

class CPlayer;
class CBegBlue: public CBaseObj
{
public:
    CBegBlue();
    ~CBegBlue();
public:
    // 初始化相关对象
    BOOL Init(DT_BEGBLUE_DATA &stDT_BEGBLUE_DATA, CPlayer* poOwner);
    DT_BEGBLUE_DATA& GetDT_BEGBLUE_DATA()
    {
        return m_stDT_BEGBLUE_DATA;
    }

public:


	UINT16 OpenBluePavilion(DT_BLUE_PAVILION_DATA_CLI &stBluePavilion, UINT8 &byIsIgnoreXnMultiple ,DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage);
	UINT16 BegBlue(DT_BEGBLUE_RESULT_CLI &stBegBlueResult);
	UINT16 ImproveLuck(DT_BEGBLUE_RES_DATA_CLI &stBegBlueResCli, DT_IMPROVELUCK_INFO_CLI &stImproveLuckInfo);
	UINT16 ComfirmBegBlue(DT_BEGBLUE_INFO_CLI &stBegBlueInfo, DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage);
    UINT16 GetBlueEncourage(DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage, UINT32 &dwBlue);

protected:
	//BOOL InitBegBlue(DT_BEGBLUE_DATA &stDT_BEGBLUE_DATA);// 初始化信息
	//DECLARE_DESTORY(BegBlue)

	// 对从数据库取到内存后的求仙露对象数据进行过期检查和设置初值
	BOOL ChkAndSetBegBlueInitVal(); 

	// 获取求仙露每天的初值
	BOOL GetBegBlueInitVal(UINT32& dwMaxBegTimes, 
		UINT32& dwFreeImproveLuckMaxTimes, 
		TCHAR aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT]);    

	// 获取求仙露最新的限制值
	BOOL GetBegBlueNewestVal(UINT32& dwMaxBegTimes, TCHAR aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT]);    

	UINT32 GetNextImproveLuckGold(UINT32 dwGoldImproveLuckIdx);
	BOOL GetBegBlueRes(DT_BEGBLUE_RES &stBegBlueRes);
	BOOL RefreshBegBlueRes(DT_BEGBLUE_RES &stBegBlueRes);

	//
	VOID GetBegBlueInfo(DT_BEGBLUE_INFO_CLI &stBegBlueInfo);
	VOID GetImproveLuckInfo(DT_IMPROVELUCK_INFO_CLI &stImproveLuckInfo);
	VOID GetBegBlueResCli(DT_BEGBLUE_RES_DATA_CLI &stBegBlueResCli);
	VOID GetClimbPrompt(TCHAR aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT]);
    //获取奖励信息
    VOID GetEncourageInfo(DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage);

private:
    DT_BEGBLUE_DATA	m_stDT_BEGBLUE_DATA;

};

DECLARE_CLASS_POOL_MGR(CBegBlue)

#endif // #ifndef 





