#ifndef __DB_PACKETPROCESSOR_INCLUDE_H_
#define __DB_PACKETPROCESSOR_INCLUDE_H_


#include <protocol/server/protogsdb.h>
#include <db/getprivatemsglog.h>
#include <dll/sdframework/sdpacketprocessor.h>

using namespace SGDP;

class CPlayerSaveData;
class CPlayersPlunderSaveData;
class CPlayersRaceSaveData;

struct STongBuPayProcessed;
struct STongBuPayInfo;

struct SGetPlayerData;
struct SGetPlayerData2;

class CUser;

class CDBProcessor : public CSDPacketProcessor
{
public:
    CDBProcessor();
    ~CDBProcessor();
public:
    virtual BOOL  Init() ;
    virtual CSDProtocol* GetProtocol() ;
public:
    static  BOOL  OnPlayerDataAck(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);

    static  BOOL  OnPlayerDataAck2(SGetPlayerData2* pstGetPlayerData, UINT32 dwTransID);

    static  BOOL  OnCreatePlayerAck(DT_GSDB_CREATE_PALYER_DATA_ACK& stAck, UINT32 dwTransID);

    static  BOOL  OnSavePlayerDataAck(CPlayerSaveData* poPlayerSaveData, UINT32 dwTransID);

    static  BOOL  OnSavePlayersPlunderDataAck(CPlayersPlunderSaveData* poPlayersPlunderSaveData, UINT32 dwTransID);

    static  BOOL  OnSavePlayersRaceDataAck(CPlayersRaceSaveData* poPlayersRaceSaveData, UINT32 dwTransID);

    static  BOOL  OnBattleDataAck(UINT32 dwPlayerID, const DT_BATTLE_DATA5 &battleData, BOOL bHaveData);

    static  BOOL  OnSaveBossBDataAck(BOOL bSuccess, UINT32 dwTransID);

    static	BOOL  OnSaveTongBuPayInfo(STongBuPayInfo& stPayInfo);

    static	BOOL  OnProcessedTongBuPayInfo(STongBuPayProcessed& stPayInfo);

    static	BOOL OnCreateFactionReq( UINT32 dwPlayerID, UINT16 byCreateFactionType, TCHAR aszFactionName[MAX_FACTION_NUM], TCHAR aszFactionDescName[MAX_FACTION_DESC_NUM], UINT32 dwFactionID, UINT16 wErrCode);

	static	BOOL OnGetPrivateMsg(GET_PRIVATEMSGLOG stInfo, SPrivateMsgList listPrivateMsg, UINT16 wErrCode);

public:
    static	BOOL  OnPlayerDataAck_SyncData(INT16 wErrCode, DT_PLAYER_DATA& stDT_PLAYER_DATA, UINT32 dwTransID);
    static  CPlayer* InitPlayer(DT_PLAYER_DATA& stPlayerData, CUser* poUser, BOOL &bAleadyLogin);

protected:
    static	BOOL  OnPlayerDataAck_Login(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_Challenge(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_Plunder(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_Plunder2(SGetPlayerData2* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnCreatePlayerAck_GM(DT_GSDB_CREATE_PALYER_DATA_ACK& stAck, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_DriveAway(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_HeroInfo(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_GM(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_PK(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnRDChallenge(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnGetTongbuPayPlayer(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnGmGetPlayerInfo(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_Exchange( SGetPlayerData* pstGetPlayerData, UINT32 dwTransID );
    static	BOOL  OnPlayerDataAck_RecvFlowerLog(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_SendFlower(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_SendPrivateMsg(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static	BOOL  OnKickedPlayerOutFaction(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	VOID  OnPlayerDataAck_ReCalculatePower(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static	BOOL  OnPlayerDataAck_AddFriends(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static	BOOL  OnPlayerDataAck_Retreate( SGetPlayerData* pstGetPlayerData, UINT32 dwTransID );
	static	BOOL  OnPlayerDataAck_SendFlowerEx(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static	BOOL  OnPlayerDataAck_AddToBlack(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static	BOOL  OnPlayerDataAck_GetFriendsNum(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_AddHero(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static  BOOL  OnPlayerInfoAwaken(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static  BOOL  OnPlayerInfoGodWeapon(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static  BOOL  OnPlayerInfoSkill(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static  BOOL  OnPlayerInfoPandect(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static  BOOL  OnPlayerInfoJewelry(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static BOOL OnPlayerInfoEquip(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
    static	BOOL  OnPlayerDataAck_RemotePK(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID); // 取出玩家数据后发送给跨服战服务器用于跨服pk
	static BOOL OnPlayerFormationTip(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static BOOL OnPlayerSoul(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static BOOL OnPlayerPet(SGetPlayerData* pstGetPlayerData, UINT32 dwTransID);
	static BOOL OnPlayerSnapshot(SGetPlayerData* pstGetPlayerData);
    static VOID PassAllCommonInstance(CPlayer* poPlayer);
private:
};



#endif

