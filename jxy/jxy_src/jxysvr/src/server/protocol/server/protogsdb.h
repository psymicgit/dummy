/*       Version Number: 3          */

#ifndef PROTOGSDB_H_INCLUDE_VERSION_3
#define PROTOGSDB_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define GET_NODATA               99
#define SAVE_RET_SUCCESS         0
#define SAVE_RET_FAILED_NOTRY    1
#define SAVE_RET_FAILED_CANTRY   2
#define GET_PLAYERDATATYPE_LOGIN 0
#define GET_PLAYERDATATYPE_CHALLENGE 1
#define GET_PLAYERDATATYPE_PLUNDER 2
#define GET_PLAYERDATATYPE_SYNCDATA 3
#define GET_PLAYERDATATYPE_DRIVEAWAY 4
#define GET_PLAYERDATATYPE_HEROLIST 5
#define GET_PLAYERDATATYPE_HEROINFO 6
#define GET_PLAYERDATATYPE_GM    7
#define GET_PLAYERDATATYPE_PK    8
#define GET_PLAYERDATATYPE_EXCHANGE 9
#define GET_RANDOM_CHALLENGE     10
#define GET_PLAYERDATATYPE_TONGBU 11
#define GET_PLAYERDATATYPE_GMPLAYERINFO 12
#define GET_PLAYERDATATYPE_RECVFLOWERLOG 13
#define GET_PLAYERDATATYPE_SENDFLOWER 14
#define GET_PLAYERDATATYPE_SEND_PRIVATE_MSG 15
#define KICKED_PLAYER_OUT_FACTION 16
#define GET_PLAYERDATATYPE_RECALC_POWER 17
#define GET_PLAYERDATATYPE_ADD_FRIENDS 18
#define GET_PLAYERDATATYPE_RETREATE 19
#define GET_PLAYERDATATYPE_SENDFLOWEREX 20
#define GET_PLAYERDATATYPE_ADD_TO_BLACK 21
#define GET_PLAYERDATATYPE_GET_FRIENDS_NUM 22
#define GET_PLAYERDATATYPE_ADD_HERO 23
#define GET_PLAYER_INFO_AWAKEN   24
#define GET_PLAYER_INFO_SKILL    25
#define GET_PLAYER_INFO_GODWEAPON 26
#define GET_PLAYER_INFO_JEWELRY  27
#define GET_PLAYER_INFO_PANDECT  28
#define GET_PLAYER_INFO_EQUIP    29
#define GET_PLAYERDATATYPE_REMOTE_PK 30
#define GET_PLAYER_FORMATION_TIP 31
#define GET_PLAYER_SNAPSHOT      32
#define GET_PLAYER_SOUL          33
#define GET_PLAYER_PET           34
#define PROTOGSDB_MSGID_RANGE 1
#define PROTOGSDB_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagDT_GSDB_GET_PALYER_DATA_REQ{
	UINT8            byGetKind; //获取玩家信息
	TCHAR            aszUserName[USERNAME_LEN]; //获取玩家信息类型，0为玩家登录
	TCHAR            aszDeviceID[DEVICEID_LEN]; //获取玩家信息类型，0为玩家登录
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //获取玩家信息类型，0为玩家登录
	UINT32           dwPlayerID; //获取玩家信息类型，0为玩家登录
}DT_GSDB_GET_PALYER_DATA_REQ;

typedef struct tagDT_GSDB_GET_PALYER_DATA_ACK{
	UINT16           wErrCode; //获取玩家信息
	UINT8            byGetKind; //获取玩家信息
	TCHAR            aszUserName[USERNAME_LEN]; //获取玩家信息类型，0为玩家登录
	TCHAR            aszDeviceID[DEVICEID_LEN]; //获取玩家信息类型，0为玩家登录
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //获取玩家信息类型，0为玩家登录
	DT_PLAYER_DATA   stPlayerData; //获取玩家信息类型，0为玩家登录
}DT_GSDB_GET_PALYER_DATA_ACK;

typedef struct tagDT_GSDB_CREATE_PALYER_DATA_REQ{
	TCHAR            aszDeviceID[DEVICEID_LEN]; //获取玩家信息
	TCHAR            aszUserName[USERNAME_LEN]; //获取玩家信息
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //获取玩家信息
	UINT32           dwUserID; //获取玩家信息
	UINT8            byGmFlag; //获取玩家信息
	DT_PLAYER_DATA   stPlayerData; //获取玩家信息
}DT_GSDB_CREATE_PALYER_DATA_REQ;

typedef struct tagDT_GSDB_CREATE_PALYER_DATA_ACK{
	UINT16           wErrCode; //获取玩家信息
	UINT8            byGmFlag; //获取玩家信息
}DT_GSDB_CREATE_PALYER_DATA_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoGSDB_MessageID
{
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodeDT_GSDB_GET_PALYER_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GSDB_GET_PALYER_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GSDB_GET_PALYER_DATA_ACK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GSDB_GET_PALYER_DATA_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GSDB_CREATE_PALYER_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GSDB_CREATE_PALYER_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GSDB_CREATE_PALYER_DATA_ACK(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GSDB_CREATE_PALYER_DATA_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGSDB)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGSDB)(void *pHost, CNetData* poNetData);

class CProtoGSDB : public CSDProtocol
{
public:
    static CProtoGSDB * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGSDB FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGSDB FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGSDB();
	virtual ~CProtoGSDB();
	EnFuncCProtoGSDB m_EncodeFuncArray[PROTOGSDB_MSGID_RANGE];
	DeFuncCProtoGSDB m_DecodeFuncArray[PROTOGSDB_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
