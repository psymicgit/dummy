#ifndef _HOT_SPRING_H_
#define _HOT_SPRING_H_
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>

enum EnterType
{
	Enter_Normal = 0, //1重新进入
	Enter_Back = 1, //0从后台登入，

};
class CPlayer;
class CFaction;
class CHotSpring:public CBaseObj
{
public:
	CHotSpring(){ 
		memset(&m_stHotSpringData, 0, sizeof(DT_HOT_SPRING_DATA));
		memset(&m_stHotSpringExt, 0, sizeof(DT_HOT_SPRING_EXT_DATA));
	}
	~CHotSpring(){}

	VOID WriteDownPosition(UINT16 wPosX, UINT16 wPosY);
	UINT16 OnOpenHotSpring(PKT_CLIGS_OPEN_HOT_SPRING_ACK& stAck);
	UINT16 OnSelectWine(UINT32 dwWineID, UINT64& qwCoin, UINT64& qwGold, DT_WINE_DATA_CLI& stWineDataCli, UINT8& byState);
	//移动记录
	UINT16 OnDrinkWineMove(UINT16 wPosX, UINT16 wPosY);
	VOID LeaveHotSpring();
	UINT16 GiveWine( DT_DIDNOT_GIVE_WINE_CLI& stGiveWineInfo);
	UINT16 ClickSelectWineGiveWine( UINT8 byDrinkWineState, DT_DIDNOT_DRINGK_WINE_LIST_CLI& stWineDataList);
	UINT16 SureGiveWine(UINT64& qwActiviTime);
	UINT16 OpenFindDrink();
	VOID GetDrinkReward(PKT_CLIGS_HOT_SPRING_REWARD_NTF* stNtf);
	VOID AddDrinkEvent();
	VOID AfterGiveWineProc();
	UINT16 OverRewardAck();
	VOID WorkOutDrinkPassTime();
	//每次更新检查
	VOID CheckByEveryTime();
	VOID GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt);
	UINT16 GetOpenLeaveHotSpringMsg(TCHAR* aszPromptMsg);
	UINT16 GetExprienceHotSpring(PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK& stAck, UINT32 dwExprienceValue);
protected:
	CFaction* GetFaction();
	UINT32 GetFactionID();
	UINT16 OnSelectWineMain(UINT32 dwWineID, UINT64& qwCoin, UINT64& qwGold);
	//活动时间内未开启赐酒
	UINT16 SetGiveWineData(DT_DIDNOT_GIVE_WINE_CLI& stDidnotGiveWine);
	//自己数据
	VOID SetHotSpringSelfData(DT_HOT_SPRING_SELF_DATA_CLI& stPlayerSelf);
	//玩家数据
	VOID SetHotSpringPlayerList(DT_HOT_SPRING_PLAYER_DATA_CLI_LST& stPlayerInfo);
	//赋值在活动时间内已饮酒数据
	VOID SetDrinkWineData(DT_WINE_DATA_CLI& stDrinkWineData);
	//所有酒的数据 
	VOID SetWineInfoAll(DT_DIDNOT_DRINGK_WINE_LIST_CLI& stDidDrinkWineData);

	VOID ResetTimeByWine();
	BOOL IsOpen();
	VOID GetExprience(DT_HOT_SPRING_EXPERIENCE& stExprience, UINT64 qwNowTime);
	VOID ResetExprienceTime(UINT64 qwNowTime);
public:
	// 初始化相关对象
	BOOL Init(DT_HOT_SPRING_DATA &m_stHotSpringData, CPlayer* poOwner, DT_HOT_SPRING_EXT_DATA& stExt);
	DT_HOT_SPRING_DATA& GetDT_HOT_SPRING_DATA() { return m_stHotSpringData; }
	DT_HOT_SPRING_EXT_DATA& GetDT_HOT_SPRING_EXT(){ return m_stHotSpringExt; }
private:
	DT_HOT_SPRING_DATA m_stHotSpringData;
	DT_HOT_SPRING_EXT_DATA m_stHotSpringExt;
};

#endif