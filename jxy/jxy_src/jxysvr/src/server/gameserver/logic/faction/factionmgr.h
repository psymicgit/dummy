
#ifndef _FACTION_MGR_H_
#define _FACTION_MGR_H_

#include <protocol/server/protocommondata.h>
#include <sdsingleton.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include "factionauthorityprop.h"
#include "faction.h"
#include "factionprop.h"
#include "factionplayer.h"
#include <map>
#include <list>

using namespace SGDP;
using namespace std;


//玩家
typedef map<UINT32, CFaction*>	CMapFaction;
typedef CMapFaction::iterator	CMapFactionItr;

typedef map<UINT32, CFactionPlayer*> CMapFactionPlayer;
typedef CMapFactionPlayer::iterator CMapFactionPlayerItr;

typedef list<CFaction*> CListRankFaction; //key为rank
typedef CListRankFaction::iterator CListRankFactionItr;


typedef map<string, CFaction*> CFactionNameMap;
typedef CFactionNameMap::iterator CFactionNameMapItr;



/**
* @brief 管理类
*/
class CFactionMgr
{

public:
    DECLARE_SINGLETON(CFactionMgr)

    BOOL Init();

    VOID UnInit();

public:
    //保存数据
    void OnSave(BOOL bAll = FALSE);


public:
    CFactionMgr();
    ~CFactionMgr();
    //解散门派
    VOID DissolveFaction(CFaction* poFaction);

    BOOL Run();
    //获取帮派信息
    CFaction *	GetFaction(UINT32 dwFactionID);
	CFaction* GetFactionByPlayerID(UINT32 dwPlayerID);
    //创建帮派
    UINT16	CreateFactionReq(CPlayer* poPlayer, UINT16 byCreateFactionType, TCHAR aszFactionName[MAX_FACTION_NUM], TCHAR aszFactionDescName[MAX_FACTION_DESC_NUM], UINT32 dwFactionID, DT_FACTION_OWNER_DATA &stFactionOwnerData);

    //申请加入帮派
    UINT16	ApplyFactionDataReq(UINT32 dwFactionID, UINT32 dwJoinPlayerID, UINT32& dwJoinFactionCDTime);

    //取消申请加入帮派
    UINT16	CancelApplyFactionDataReq(UINT32 dwFactionID, UINT32 dwJoinPlayerID);

    //踢出玩家帮派
    UINT16	KickedFactionReq(UINT32 dwJoinPlayerID, UINT32 dwKickedPlayerID, PKT_CLIGS_KICKED_FACTION_ACK& stAck);

    UINT16	OpenApplyFactionReq(DT_FACTION_BASE_DATA_CLI_LST &ststDT_FACTION_BASE_DATA_CLI_LST, UINT32 dwJoinPlayerID);

    UINT16	GetRankFactionCliLst(DT_FACTION_BASE_DATA_CLI_LST &ststDT_FACTION_BASE_DATA_CLI_LST, UINT32 dwJoinPlayerID);

    UINT16	GetSearchFactionCliLst(DT_FACTION_BASE_DATA_CLI_LST &ststDT_FACTION_BASE_DATA_CLI_LST, TCHAR aszFactionName[MAX_FACTION_NUM], UINT32 dwJoinPlayerID);

    UINT16	OnOpenOwnerFactionReq(PKT_CLIGS_OPEN_OWNER_FACTION_ACK &stAck, UINT32 dwJoinPlayerID);

    UINT16	OpenDoorsTributeFactionReq(PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK& stAck, UINT32 dwJoinPlayerID);

    UINT16	DoorsTributeFactionReq(PKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK& stAck, UINT8 byDoorsTributeFlag, CPlayer* poPlayer);

    UINT16	RecvDoorsTributeFactionReq( CPlayer* poPlayer , PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK& stAck);

    UINT16	JobFactionReq(UINT32 dwJoinPlayerID, UINT32 dwJobPlayerID, UINT8 byFlag, PKT_CLIGS_JOB_FACTION_ACK& stAck );

    // 设置帮派基本信息
    UINT16	SetNoticeFactionReq( UINT32 dwJoinPlayerID, UINT8 byFlag, TCHAR aszFactionNotice[MAX_FACTION_DESC_NUM]);

    UINT16	DissolveFactionReq( UINT32 dwJoinPlayerID );

    UINT16	OpenAuditFaction(PKT_CLIGS_OPEN_AUDIT_FACTION_ACK &stAck, UINT32 dwJoinPlayerID);

    UINT16	AuditFactionReq( UINT32 dwJoinPlayerID, UINT32 dwFactionNum, DT_FACTION_AUDIT_PLAYER_DATA astAuditPlayerData[DT_FACTION_PLAYER_NUM], PKT_CLIGS_AUDIT_FACTION_ACK& stAck );

    UINT16	GetDT_FACTION_LOG_DATA_CLI_LST(DT_FACTION_LOG_DATA_CLI_LST &stDT_FACTION_LOG_DATA_CLI_LST, UINT32 dwJoinPlayerID);
    //扣除玩家门贡
    BOOL  DeductDoorsTribute( UINT32 dwPlayerID, UINT32 dwBuyCost, UINT16 wOpType);

    VOID AddDoorsTribute(UINT32 dwPlayerID, UINT32 dwAddValue, UINT16 wOpType);

    //检验升级权限　　　0为没权
    UINT8 CheckPlayerAuthority(UINT32 dwPlayerID, em_Faction_Authority emAuth);

    //增加门派捉跃积分
    VOID AddHuntingIntegral(UINT32 dwPlayerID, UINT32 dwValue);

    UINT32 ResetMaxTaskIntegral(UINT32 dwPlayerID);
    //增加门派协议
    VOID AddFactionFunds(UINT32 dwPlayerID, UINT32 dwValue, UINT16 wOpType);

    //给一个门派的所有玩家添加门贡
    VOID AddDoorsTributeToAll(UINT32 dwFactionID, UINT32 dwDoorsTribute);

    VOID AddSetHotSpringPlayerID(UINT32 dwPlayerID);
    VOID DelSetHotSpringPlayerID(UINT32 dwPlayerID);
    VOID SetHotSpringPlayerList(DT_HOT_SPRING_PLAYER_DATA_CLI_LST& stPlayerInfo, UINT32 dwFactionID);

    UINT16 ChangeOther(UINT32 dwPlayerID, UINT32 dwSendPlayerID, PKT_CLIGS_CHANGE_OTHER_ACK& stAck);

    UINT16 OpenFactionUpgrade(UINT32 dwPlayerID, DT_FACTION_UPGRADE_ALL_INFO& stUpgradeAllInfo);

    UINT16 FactionUpgrade(UINT32 dwPlayerID,  PKT_CLIGS_FACTION_UPGRADE_ACK& stAck);

    // 添加门派活动公告
    void AddFactionActivityChat(const string &strChatContent);

    BOOL AddChatData(UINT32 dwPlayerID, const ChatInfo& stChatData);

    //门派建筑解锁
    BOOL GetFactionBuileGroupByLevel(UINT32 dwFactionLevel, DT_FACTION_BUILD_LEVEL& stBuildFlag);
    UINT32 GetFactionBuildLevel(UINT32 dwFactionID, em_Faction_Build emBuildType);

    void TakeTopFactions(CListRankFaction&, UINT16 byTakeCnt);

    CListRankFaction& GetRankFactions();

    CMapFaction& GetFactions()
    {
        return m_mapFaction;
    }

    VOID  DecreaseRank()
    {
        --m_dwMaxFactionRank;
		FactionSort();
    }

    BOOL IsExitFactionName(TCHAR* aszFactionName);

	BOOL RegFactionName(TCHAR* aszAddFactionName, CFaction *poFaction, TCHAR* aszDelFactionName = NULL);

    VOID SendHuntingActiviEndNtf();
    //////////////////////////////////////////////////////////////////////////
    VOID FactionSort();

	UINT16 GetApplyFactionNum(UINT32 dwPlayerID);

protected:
    BOOL		InitDataFromDB();
    void		GetDoorsTributeBaseData(DT_DOORS_TRIBUTE_BASE_DATA &stDoorsTributeData, DT_ITEM_LIST_INFO &stEncourageData, UINT32 dwVipLevel, UINT32 dwFactionLevel);
    BOOL		GetFactionUpgradeInfo(UINT32 dwFactionLevel, DT_FACTION_UPGRADE_ALL_INFO& stUpgradeAllInfo);
private:
    CSDObjectPool<CFaction, CSDMutex>		m_oFactionShmemObjMgr;
    CMapFaction							m_mapFaction;
	CFactionNameMap						m_mapFactionName;
    CListRankFaction						m_listRankFaction;
    UINT32								m_dwMaxFactionRank;

	UINT64								m_qwSaveTime;
};

#endif //#ifndef _ROLEMGR_H_

