

#ifndef _DB_SERVER_PACKAGE_BUILDER_H_
#define _DB_SERVER_PACKAGE_BUILDER_H_


#include "protocol/server/protogsdb.h"
#include <db/getprivatemsglog.h>
#include <logic/player/playersavemgr.h>

class CUser;

struct SMyPayProcessed;
struct SCenterPayProcessed;
struct STongBuPayProcessed;
struct SRecreatePayProcessed;

struct SGetPlayerData;
struct SGetPlayerData2;


using namespace SGDP;

//class CDBPktBuilder : public CSDPacketBuilder
class CDBPktBuilder
{
public:
    static CDBPktBuilder* Instance()
    {
        static CDBPktBuilder oDBPkgBuilder;
        return &oDBPkgBuilder;
    }
protected:
    CDBPktBuilder();
    ~CDBPktBuilder();

public:
    BOOL GetPlayerDataReq(SGetPlayerData* pstGetPlayerData);
    BOOL GetPlayerDataReq2(SGetPlayerData2* pstGetPlayerData);
    //fortest
    //BOOL GetPlayerDataReqOld(SGetPlayerData* pstGetPlayerData);

    BOOL SavePlayerDataReq(UINT32 dwTransID, CPlayerSaveData* poPlayerSaveData, ESaveDBType type, BOOL bRecordOnlineTime);

    BOOL CreatePlayerReq(UINT32 dwTransID, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT8 byGmFlag, DT_PLAYER_DATA& stPlayerDefault);

    BOOL ClrPlayerData(UINT32 dwPlayerID);

    BOOL SavePlayersPlunderDataReq(CPlayersPlunderSaveData* poPlayersPlunderSaveData);

    BOOL SavePlayersRaceDataReq(CPlayersRaceSaveData* poPlayersRaceSaveData);

    BOOL SaveBossBDataReq(DT_BOSS_BATTLE_DATA* pstDT_BOSS_BATTLE_DATA);

    BOOL SaveFactionDataReq(DT_FACTION_DATA* pstDT_FACTION_DATA);

    BOOL OnGetOrderIDReq(CPlayer* poPlayer, CUser* poUser);

    BOOL OnProcessOrderReq(STongBuPayProcessed& stInfo);

    BOOL OnProcessMyOrderReq(SMyPayProcessed& stInfo);

	BOOL OnProcessCenterOrderReq(SCenterPayProcessed& stInfo);	

    BOOL GetPrivateMsg(UINT32 dwSenderID, UINT32 dwRecverID, em_GetPrivateMsg emType, string strMsg = "", UINT32 dwStartIdx = 0, UINT8  byGetMsgNum = 0);

    BOOL SaveCourageDataReq(DT_COURAGE_GROUP_DATA_LST* pstDT_COURAGE_GROUP_DATA);

	BOOL OnProcessRechargeOrderReq(vector<SRecreatePayProcessed>& vecInfo,UINT32 dwTransID);

	BOOL SaveGvGBuildDataReq(DT_GVG_BUILD_DATA& stData);
protected:
    string GetOrderID(string strTableName, UINT32 dwPlayerID);
};


#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

