/*       Version Number: 3          */

#ifndef PROTORECHARGEGS_H_INCLUDE_VERSION_3
#define PROTORECHARGEGS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define MAX_RECHARGE_STR_LEN     65536
#define PROTORECHARGEGS_MSGID_RANGE 10
#define PROTORECHARGEGS_MSGID_OFFSET 90
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_RECGS_RECHARGE_REQ{
	tagPKT_RECGS_RECHARGE_REQ() { memset(this, 0, sizeof(tagPKT_RECGS_RECHARGE_REQ)); }
	CHAR             szReqStr[MAX_RECHARGE_STR_LEN]; //扩展
}PKT_RECGS_RECHARGE_REQ;

typedef struct tagPKT_RECGS_RECHARGE_ACK{
	tagPKT_RECGS_RECHARGE_ACK() { memset(this, 0, sizeof(tagPKT_RECGS_RECHARGE_ACK)); }
	CHAR             szAckStr[MAX_RECHARGE_STR_LEN]; //扩展
}PKT_RECGS_RECHARGE_ACK;

typedef struct tagPKT_RECGS_HEART_REQ{
	tagPKT_RECGS_HEART_REQ() { memset(this, 0, sizeof(tagPKT_RECGS_HEART_REQ)); }
	UINT8            byExt; //扩展
}PKT_RECGS_HEART_REQ;

typedef struct tagPKT_RECGS_HEART_ACK{
	tagPKT_RECGS_HEART_ACK() { memset(this, 0, sizeof(tagPKT_RECGS_HEART_ACK)); }
	UINT8            byExt; //扩展
}PKT_RECGS_HEART_ACK;

typedef struct tagPKT_RECGS_GEN_GUID_REQ{
	tagPKT_RECGS_GEN_GUID_REQ() { memset(this, 0, sizeof(tagPKT_RECGS_GEN_GUID_REQ)); }
	CHAR             szInfo[MAX_RECHARGE_STR_LEN]; //内容
}PKT_RECGS_GEN_GUID_REQ;

typedef struct tagPKT_RECGS_GEN_GUID_ACK{
	tagPKT_RECGS_GEN_GUID_ACK() { memset(this, 0, sizeof(tagPKT_RECGS_GEN_GUID_ACK)); }
	CHAR             szInfo[MAX_RECHARGE_STR_LEN]; //内容
}PKT_RECGS_GEN_GUID_ACK;

typedef struct tagPKT_RECGS_PROCESS_ORDER_ID_REQ{
	tagPKT_RECGS_PROCESS_ORDER_ID_REQ() { memset(this, 0, sizeof(tagPKT_RECGS_PROCESS_ORDER_ID_REQ)); }
	CHAR             szInfo[MAX_RECHARGE_STR_LEN]; //内容
}PKT_RECGS_PROCESS_ORDER_ID_REQ;

typedef struct tagPKT_RECGS_PROCESS_ORDER_ID_ACK{
	tagPKT_RECGS_PROCESS_ORDER_ID_ACK() { memset(this, 0, sizeof(tagPKT_RECGS_PROCESS_ORDER_ID_ACK)); }
	CHAR             szInfo[MAX_RECHARGE_STR_LEN]; //内容
}PKT_RECGS_PROCESS_ORDER_ID_ACK;

typedef struct tagPKT_RECGS_CHECK_APP_ORDER_ID_REQ{
	tagPKT_RECGS_CHECK_APP_ORDER_ID_REQ() { memset(this, 0, sizeof(tagPKT_RECGS_CHECK_APP_ORDER_ID_REQ)); }
	CHAR             szInfo[MAX_RECHARGE_STR_LEN]; //内容
}PKT_RECGS_CHECK_APP_ORDER_ID_REQ;

typedef struct tagPKT_RECGS_CHECK_APP_ORDER_ID_ACK{
	tagPKT_RECGS_CHECK_APP_ORDER_ID_ACK() { memset(this, 0, sizeof(tagPKT_RECGS_CHECK_APP_ORDER_ID_ACK)); }
	CHAR             szInfo[MAX_RECHARGE_STR_LEN]; //内容
}PKT_RECGS_CHECK_APP_ORDER_ID_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoRechargeGS_MessageID
{
	RECGS_RECHARGE_REQ              = 90          ,
	RECGS_RECHARGE_ACK              = 91          ,
	RECGS_HEART_REQ                 = 92          ,
	RECGS_HEART_ACK                 = 93          ,
	RECGS_GEN_GUID_REQ              = 94          ,
	RECGS_GEN_GUID_ACK              = 95          ,
	RECGS_PROCESS_ORDER_ID_REQ      = 96          ,
	RECGS_PROCESS_ORDER_ID_ACK      = 97          ,
	RECGS_CHECK_APP_ORDER_ID_REQ    = 98          ,
	RECGS_CHECK_APP_ORDER_ID_ACK    = 99          ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_RECGS_RECHARGE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_RECHARGE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_RECHARGE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_RECHARGE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_HEART_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_HEART_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_HEART_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_HEART_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_GEN_GUID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_GEN_GUID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_GEN_GUID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_GEN_GUID_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_PROCESS_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_PROCESS_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_PROCESS_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_PROCESS_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_CHECK_APP_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_CHECK_APP_ORDER_ID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_RECGS_CHECK_APP_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_RECGS_CHECK_APP_ORDER_ID_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoRechargeGS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoRechargeGS)(void *pHost, CNetData* poNetData);

class CProtoRechargeGS : public CSDProtocol
{
public:
    static CProtoRechargeGS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoRechargeGS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoRechargeGS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoRechargeGS();
	virtual ~CProtoRechargeGS();
	EnFuncCProtoRechargeGS m_EncodeFuncArray[PROTORECHARGEGS_MSGID_RANGE];
	DeFuncCProtoRechargeGS m_DecodeFuncArray[PROTORECHARGEGS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;
};

#pragma pack(pop)
#endif
