#ifndef	_FACTION_PLAYER_H_
#define _FACTION_PLAYER_H_
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <logic/rareitemroom/rareitemroom.h>
#include <logic/gymnasium/gymnasium.h>
#include <logic/factionshade/shade.h>
#include <logic/hotspring/hotspring.h>
#include <logic/faction/factionbattlelog.h>
#include <logic/mark/mark.h>
#include "logic/faction/factiondonate.h"
class CFactionPlayer : public CBaseObj
{
public:
	CFactionPlayer(){ 
		memset(&m_stPlayerFaction, 0, sizeof(DT_PLAYER_FACTION_DATA)); 
		m_dwFindBackDoorsTribute = 0;
	}
	BOOL Init(DT_PLAYER_FACTION_DATA& stData, CPlayer* poOwner);
	DT_PLAYER_FACTION_DATA& GetDT_PLAYER_FACTION_DATA();
	DT_HOT_SPRING_DATA& GetDT_HOT_SPRING_DATA();
	DT_SHADE_DATA& GetDT_SHADE_DATA();
	DT_PLAYER_MARK_DATA& GetDT_PLAYER_MARK_DATA();
	DT_PLAYER_FACTION_BATTLELOG_DATA_LIST& GetDT_BATTLE_LOG_LST();
    CShade& GetShade()
    {
        return m_oShade;
    }
    CHotSpring& GetHotSpring()
    {
        return m_oHotSpring;
    }
    CGymnasium& GetGymnasium()
    {
        return m_oGymnasium;
    }
    CRareItemRoom& GetRareItem()
    {
        return m_oRareItem;
    }
	CMapRareItemGoods& GetRareItemMap();
	/////////////////////////////门派/////////////////////////////////////////////
	UINT16 AuditFactionReq( UINT32 dwPlayerID, UINT8 byAuditWay, PKT_CLIGS_AUDIT_FACTION_ACK2& stAck);
	UINT16 AuditAllRefuse();
	UINT16 OpenAuditFaction(PKT_CLIGS_OPEN_AUDIT_FACTION_ACK2& stAck );
	UINT16 OnOpenFactionAck(PKT_CLIGS_OPEN_FACTION_ACK& stAck);
	UINT16 FactionPlayerInfoAck(PKT_CLIGS_FACTION_PLAYERS_INFO_ACK& stAck);
	UINT16 OnChangeIconAck(PKT_CLIGS_CHANGE_FACTION_ICON_ACK& stAck, UINT8 byIconID);
	UINT16 OnOpenChangeIconAck(PKT_CLIGS_OPEN_CHANGE_FACTION_ICON_ACK& stAck);
	UINT16 OnOpenFindBackDoorsTributeAck(PKT_CLIGS_OPEN_FIND_BACK_DOORSTRIBUTE_ACK& stAck);
	UINT16 OnFindBackDoorsTributeAck(UINT8 byFindBackType, PKT_CLIGS_FIND_BACK_DOORSTRIBUTE_ACK& stAck);
	////////////////////////////////珍品坊//////////////////////////////////////////
	UINT32 GetOpenRareItem( DT_RAREITEM_ROOM_INFO_CLI& stRareItemRoomInfo, DT_RAREITEM_INFO_LIST_CLI& stRareItemInfo);
	UINT32 ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID);
	UINT32 ExchangeRareItem( DT_RAREITEM_INFO_CLI& stRareItemInfo, UINT32& dwLeftDoorsTributes, UINT32 dwGoodsID, UINT8 byLeftTime);
	UINT32 OpenUpgradeRareItem(DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeCli);
	UINT32 UpgradeRareItem(PKT_CLIGS_UPGRADE_RAREITEM_ACK& stAck);
	////////////////////////////////武堂//////////////////////////////////////////
	UINT16 OpenGymnasium(PKT_CLIGS_OPEN_GYMNASIUM_ACK& stAck);
	UINT16 LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK& stAck );
	UINT16 LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2& stAck, UINT8 byCurLevel );
	UINT16 OpenGymnasiumUpgrade( DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo );
	UINT16 GymnasiumUpgrade( PKT_CLIGS_GYMNASIUM_UPGRADE_ACK& stAck );
	UINT32 GetGymnasiumAttrLevel(UINT8 byKindID);
	CMapMySkill& GetMySkillMap();
	UINT16 OpenFactionSkill(PKT_CLIGS_OPEN_FACTION_SKILL_ACK& stAck);
	////////////////////////////////暗部//////////////////////////////////////////
	UINT16 OpenShade(PKT_CLIGS_OPEN_SHADE_ACK& stAck);
	UINT16 OpenGoldMode(DT_MONSTER_INFO& stMonsterInfo, UINT32& dwSyncGold);
	UINT16 OpenShadeUpgrade(DT_SHADE_UPGRADE_INFO& stShadeUpgradeInfo);
	UINT16 ShadeUpgrade(PKT_CLIGS_SHADE_UPGRADE_ACK& stAck);
	UINT16 OpenHunting(PKT_CLIGS_OPEN_HUNTING_ACK& stAk);
	UINT16 FindMonster(vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo);
	UINT16 ChangeMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed, 
		vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo, UINT32& dwGoldNum  );
	UINT16 OnHuntingBattle(PKT_CLIGS_HUNTING_BATTLE_ACK5& stAck);
    UINT16 OnHuntingBattle2(PKT_CLIGS_HUNTING_BATTLE_ACK2& stAck);
	UINT16 OnHuntingBattle3(PKT_CLIGS_HUNTING_BATTLE_ACK3& stAck);
	UINT16 FirstFindMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed, 
		vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo);
	UINT16 ClosedMonster(UINT8 byChoice, PKT_CLIGS_CLOSED_MONSTER_ACK& stAck);
	VOID OnChangeSchedule(PKT_CLIGS_CHANGE_SCHEDEULE_NTF& stChangeNtf);
	UINT16 GetHuntingIntegral(PKT_CLIGS_HUNTING_ITEGRAL_ACK& stAck);
	VOID	GetFreeMsg(DT_CHANGE_MONSTER_INFO& stInfo);
	////////////////////////////////温泉//////////////////////////////////////////
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
	VOID WorkOutDrinkPassTime();
	VOID GetDrinkReward(PKT_CLIGS_HOT_SPRING_REWARD_NTF* stNtf);
	VOID AddDrinkEvent();
	VOID AfterGiveWineProc();
	UINT16 OverRewardAck();
	UINT16 GetExprienceHotSpring(PKT_CLIGS_GET_EXPRIENCE_HOT_SPRING_ACK& stAck, UINT32 dwExprienceValue);
	///////////////////////////////门派战斗日记///////////////////////////////////////////
	UINT32 GetUnReadFactionBattleLogNum();
	UINT16 GetFactinBattleLogByCli(UINT8&byLogNum, DT_PLAYER_FACTION_BATTLELOG_DATA* stFactionBattleLog);
	VOID AddFactionBattleLog(DT_PLAYER_FACTION_BATTLELOG_DATA& stData);
	///////////////////////////////////退出门派///////////////////////////////////////
	UINT16 LeaveFaction();
	VOID LeaveFactionTime();//记录退出时间与与状态
	UINT16 OpenLeaveFaction(UINT32& dwIntervalTime, UINT8& byLeaveFactionMark);
	//////////////////////////////////////////////////////////////////////////////////
	UINT16 GetFactionBuileGroup(PKT_CLIGS_OPEN_FACTION_BUILD_GROUP_ACK& stAck);
	VOID GetPrompt(DT_FUNC_PROMPT_DATA& stPrompt);
	VOID GetFactionPrompt(DT_FUNC_COMMON_PROMPT& stPrompt);
	VOID GetUnDisplayerBuild(UINT8& byBuildNum, UINT8* byBuildID);
	
	//////////////////////////////////////////////////////////////////////////
	DT_PLAYER_DONATE_INFO& GetFactionDonate()
	{
		return m_oDonate.GetDT_PLAYER_DONATE_INFO();
	}

	UINT32 GetFindBackDoorsTribute() { return m_dwFindBackDoorsTribute; }
	VOID ResetFindBackDoorsTribute() { m_dwFindBackDoorsTribute = 0; }
	VOID SetFindBackDoorsTribute();
protected:
	BOOL InitPlayerMark(DT_PLAYER_MARK_DATA& stPlayerMark, CPlayer* poOwner);
	BOOL InitRareItem(DT_RAREITEM_GOODS_DATA &stDT_SHADE_DATA, CPlayer* poOwner);
	BOOL InitGymnasium(DT_GYMNASIUM_DATA &stDT_GYMNASIUM_SKILL_DATA, CPlayer* poOwner);
	BOOL InitShade(DT_SHADE_DATA &stDT_SHADE_DATA, CPlayer* poOwner);
	BOOL InitHotSpring(DT_HOT_SPRING_DATA &m_stHotSpringData, CPlayer* poOwner, DT_HOT_SPRING_EXT_DATA& stExt);
	BOOL InitFactionBattleLog(DT_PLAYER_FACTION_BATTLELOG_DATA_LIST& stPlayerFactionBattleLog, CPlayer* poOwner);
	BOOL InitFactionDonate(DT_PLAYER_DONATE_INFO& stDonate, CPlayer* poOwer);
	/////////////////////////////珍品坊/////////////////////////////////////////////
	DT_RAREITEM_GOODS_DATA& GetDT_RAREITEM_GOODS_DATA();
	UINT64 GetRareItemUpdateTime();
	VOID SetRareItemUpdateTime(UINT64 qwUpdataTime);
	

	/////////////////////////////武堂/////////////////////////////////////////////
	DT_GYMNASIUM_DATA& GetDT_GYMNASIUM_SKILL_DATA();
	

	/////////////////////////////暗部/////////////////////////////////////////////
	

	/////////////////////////////温泉/////////////////////////////////////////////////


	CFaction* GetFaction();
	UINT32 GetFactionID();
private:
	CRareItemRoom m_oRareItem;
	CGymnasium m_oGymnasium;
	CShade m_oShade;
	CHotSpring m_oHotSpring;
	CFactionBattleLog m_oFactionBattleLog;
	CPlayerMark			m_oPlayerMark;
	CFactionDonate m_oDonate;
	UINT32 m_dwFindBackDoorsTribute;
	DT_PLAYER_FACTION_DATA m_stPlayerFaction;
};

DECLARE_CLASS_POOL_MGR(CFactionPlayer)
#endif