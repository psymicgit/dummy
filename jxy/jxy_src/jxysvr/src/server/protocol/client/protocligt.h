/*       Version Number: 3          */

#ifndef PROTOCLIGT_H_INCLUDE_VERSION_3
#define PROTOCLIGT_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "protoclils.h"
#include "protocligs.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define ENDEC_KEY_LEN            32
#define LOGIN_SERVER             1
#define GAME_SERVER              2
#define SAME_ACCOUNT_LOGIN       1
#define SERVER_KICK              2
#define PROTOCLIGT_MSGID_RANGE 2494
#define PROTOCLIGT_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_CLIGT_HEARTBEAT_REQ{
	tagPKT_CLIGT_HEARTBEAT_REQ() { memset(this, 0, sizeof(tagPKT_CLIGT_HEARTBEAT_REQ)); }
	UINT8            byExtInfo; //心跳信息
}PKT_CLIGT_HEARTBEAT_REQ;

typedef struct tagPKT_CLIGT_HEARTBEAT_ACK{
	tagPKT_CLIGT_HEARTBEAT_ACK() { memset(this, 0, sizeof(tagPKT_CLIGT_HEARTBEAT_ACK)); }
	UINT8            byExtInfo; //心跳信息
}PKT_CLIGT_HEARTBEAT_ACK;

typedef struct tagPKT_CLIGT_ENDEC_KEY_NTF{
	tagPKT_CLIGT_ENDEC_KEY_NTF() { memset(this, 0, sizeof(tagPKT_CLIGT_ENDEC_KEY_NTF)); }
	UINT8            abyKeyInfo[ENDEC_KEY_LEN]; //心跳信息
}PKT_CLIGT_ENDEC_KEY_NTF;

typedef struct tagPKT_CLIGT_SERVER_NOT_STARTUP_NTF{
	tagPKT_CLIGT_SERVER_NOT_STARTUP_NTF() { memset(this, 0, sizeof(tagPKT_CLIGT_SERVER_NOT_STARTUP_NTF)); }
	UINT8            byServerType; //心跳信息
}PKT_CLIGT_SERVER_NOT_STARTUP_NTF;

typedef struct tagPKT_CLIGT_KICK_OUT_NTF{
	tagPKT_CLIGT_KICK_OUT_NTF() { memset(this, 0, sizeof(tagPKT_CLIGT_KICK_OUT_NTF)); }
	UINT8            byKickOutType; //心跳信息
}PKT_CLIGT_KICK_OUT_NTF;

/*Define Structs and Unions        End*/


enum EN_ProtoCliGT_MessageID
{
	CLIGT_HEARTBEAT_REQ             = 1100        ,
	CLIGT_HEARTBEAT_ACK             = 1101        ,
	CLIGT_ENDEC_KEY_NTF             = 1102        ,
	CLIGT_SERVER_NOT_STARTUP_NTF    = 1104        ,
	CLIGT_KICK_OUT_NTF              = 1106        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_CLIGT_HEARTBEAT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_HEARTBEAT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_HEARTBEAT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_HEARTBEAT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_SERVER_NOT_STARTUP_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_SERVER_NOT_STARTUP_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_KICK_OUT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_KICK_OUT_NTF(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoCliGT)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoCliGT)(void *pHost, CNetData* poNetData);

class CProtoCliGT : public CSDProtocol
{
public:
    static CProtoCliGT * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoCliGT FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoCliGT FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoCliGT();
	virtual ~CProtoCliGT();
	EnFuncCProtoCliGT m_EncodeFuncArray[PROTOCLIGT_MSGID_RANGE];
	DeFuncCProtoCliGT m_DecodeFuncArray[PROTOCLIGT_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
