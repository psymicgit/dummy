#ifndef _SHADE_H_
#define _SHADE_H_
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <logic/factionshade/shadepropmgr.h>

class CFaction;
class CPlayer ;
class CShade : public CBaseObj
{
public:
	CShade(){ memset(&m_stShadeData, 0, sizeof(DT_SHADE_DATA));}
	~CShade(){}

	UINT16 OpenShade(PKT_CLIGS_OPEN_SHADE_ACK& stAck);
	UINT16 OpenGoldMode(DT_MONSTER_INFO& stMonsterInfo, UINT32& dwSyncGold);
	UINT16 OpenShadeUpgrade(DT_SHADE_UPGRADE_INFO& stShadeUpgradeInfo);
	UINT16 ShadeUpgrade(PKT_CLIGS_SHADE_UPGRADE_ACK& stAck);
	UINT16 OpenHunting(PKT_CLIGS_OPEN_HUNTING_ACK& stAk);
	UINT16 FindMonster(vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo );
	UINT16 ChangeMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed, 
		vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo, UINT32& dwGoldNum );
	UINT16 OnHuntingBattle(PKT_CLIGS_HUNTING_BATTLE_ACK5& stAck);
    UINT16 OnHuntingBattle2(PKT_CLIGS_HUNTING_BATTLE_ACK2& stAck);
	UINT16 OnHuntingBattle3(PKT_CLIGS_HUNTING_BATTLE_ACK3& stAck);
	UINT16 FirstFindMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed, 
		vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo);
	UINT16 ClosedMonster(UINT8 byChoice, PKT_CLIGS_CLOSED_MONSTER_ACK& stAck);
	UINT16 GetHuntingIntegral(PKT_CLIGS_HUNTING_ITEGRAL_ACK& stAck);
	VOID SendRewardSchedule();
	VOID EachUpdataShade();
	VOID GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt);
	VOID GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA);
	VOID GetFreeMsg(DT_CHANGE_MONSTER_INFO& stInfo);
protected:
	VOID GetShadeInfo(DT_SHADE_INFO& stShadeInfo);
	VOID GetScheduleInfo(DT_HUNTING_SCHEDULE_INFO& stInfo, DT_HUNTING_SCHEDULE_INFO2& stNewInfo);
	VOID GetHuntingInfoCli(DT_HUNTING_INFO_CLI& stHuntingInfoCli, DT_FACTION_COMMON_DATA& stFactionCommonData, DT_HUNTING_SCHEDULE_INFO2& stInfo2);
	UINT16 GetShadeUpgradeInfo( DT_SHADE_UPGRADE_INFO& stShadeUpgradeInfo, UINT32 dwFactionID, UINT32 dwShadeLevel);
	BOOL CheckTaskOpen(UINT8 byTaskType);
	VOID UpdataUpgradeData();
	VOID GetRewardInfoAll(DT_REWARD_INFO_ALL& stRewardInfoAll);
	VOID GetHuntingOtherInfo(DT_HUNTING_OTHER_INFO& stOtherInfo);
	VOID GetRewardInfoMsg(DT_REWARD_SINGLE_INFO& stRewardInfo, SHuntingReward stHuntingReward);
	BOOL GetMonsterInfo(DT_MONSTER_INFO& stMonsterInfo);
	BOOL GetGoodsByRandom(UINT32 dwProbability);
	VOID AddRewardByShade(UINT32 dwRewardType, UINT32 dwRewardValue, BOOL bIsSchedule = FALSE);
	VOID AddRewardBySchedule(UINT32 dwGoodsID, UINT32 dwRewardType, UINT32 dwValue);
	CFaction* GetFaction();
	UINT32 GetFactionID();
	BOOL IsOpen();
public:
	// 初始化相关对象
	BOOL Init(DT_SHADE_DATA &stDT_SHADE_DATA, CPlayer* poOwner);
	DT_SHADE_DATA& GetDT_SHADE_DATA() { return m_stShadeData; }

private:
	DT_SHADE_DATA	m_stShadeData;
	UINT32 m_dwPlayerID;
};
#endif