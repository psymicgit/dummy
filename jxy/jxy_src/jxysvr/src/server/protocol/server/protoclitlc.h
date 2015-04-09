/*       Version Number: 3          */

#ifndef PROTOCLITLC_H_INCLUDE_VERSION_3
#define PROTOCLITLC_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define ENDEC_KEY_LEN            32
#define MAX_ORDERINFO_LENGTH     1024
#define MAX_ORDERID_LEN          255
#define COMMON_TXT_LEN           255
#define PROTOCLITLC_MSGID_RANGE 6
#define PROTOCLITLC_MSGID_OFFSET 920
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_CLITLC_ENDEC_KEY_NTF{
	tagPKT_CLITLC_ENDEC_KEY_NTF() { memset(this, 0, sizeof(tagPKT_CLITLC_ENDEC_KEY_NTF)); }
	UINT8            abyKeyInfo[ENDEC_KEY_LEN]; //加解密密钥
}PKT_CLITLC_ENDEC_KEY_NTF;

typedef struct tagPKT_CLITLC_CHECK_ORDERID_REQ{
	tagPKT_CLITLC_CHECK_ORDERID_REQ() { memset(this, 0, sizeof(tagPKT_CLITLC_CHECK_ORDERID_REQ)); }
	CHAR             szOrderInfo[MAX_ORDERINFO_LENGTH]; //订单信息,json格式,必须包含"RechargeType"
}PKT_CLITLC_CHECK_ORDERID_REQ;

typedef struct tagPKT_CLITLC_CHECK_ORDERID_ACK{
	tagPKT_CLITLC_CHECK_ORDERID_ACK() { memset(this, 0, sizeof(tagPKT_CLITLC_CHECK_ORDERID_ACK)); }
	CHAR             szErrCode[MAX_ORDERINFO_LENGTH]; //错误码
	CHAR             szOrderInfo[MAX_ORDERINFO_LENGTH]; //订单信息
}PKT_CLITLC_CHECK_ORDERID_ACK;

typedef struct tagPKT_CLITLC_GEN_ORDERID_REQ{
	tagPKT_CLITLC_GEN_ORDERID_REQ() { memset(this, 0, sizeof(tagPKT_CLITLC_GEN_ORDERID_REQ)); }
	CHAR             szFrom[COMMON_TXT_LEN]; //充值来源类型,同步"tongbu"，PP"pp"
	UINT16           wZoneID; //区ID
	UINT32           dwUserID; //UserID
	UINT32           dwPlayerID; //玩家ID
	UINT16           wPlayerLevel; //玩家等级
}PKT_CLITLC_GEN_ORDERID_REQ;

typedef struct tagPKT_CLITLC_GEN_ORDERID_ACK{
	tagPKT_CLITLC_GEN_ORDERID_ACK() { memset(this, 0, sizeof(tagPKT_CLITLC_GEN_ORDERID_ACK)); }
	CHAR             szErrCode[MAX_ORDERINFO_LENGTH]; //错误码
	CHAR             szFrom[COMMON_TXT_LEN]; //充值来源类型,同步"tongbu"，PP"pp"
	UINT16           wZoneID; //区ID
	UINT32           dwUserID; //UserID
	UINT32           dwPlayerID; //玩家ID
	UINT16           wPlayerLevel; //玩家等级
	TCHAR            aszOrderID[MAX_ORDERID_LEN]; //订单号
}PKT_CLITLC_GEN_ORDERID_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoCliTlc_MessageID
{
	CLITLC_ENDEC_KEY_NTF            = 920         ,
	CLITLC_CHECK_ORDERID_REQ        = 922         ,
	CLITLC_CHECK_ORDERID_ACK        = 923         ,
	CLITLC_GEN_ORDERID_REQ          = 924         ,
	CLITLC_GEN_ORDERID_ACK          = 925         ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_CLITLC_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLITLC_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLITLC_CHECK_ORDERID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLITLC_CHECK_ORDERID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLITLC_CHECK_ORDERID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLITLC_CHECK_ORDERID_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLITLC_GEN_ORDERID_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLITLC_GEN_ORDERID_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLITLC_GEN_ORDERID_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLITLC_GEN_ORDERID_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoCliTlc)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoCliTlc)(void *pHost, CNetData* poNetData);

class CProtoCliTlc : public CSDProtocol
{
public:
    static CProtoCliTlc * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoCliTlc FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoCliTlc FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoCliTlc();
	virtual ~CProtoCliTlc();
	EnFuncCProtoCliTlc m_EncodeFuncArray[PROTOCLITLC_MSGID_RANGE];
	DeFuncCProtoCliTlc m_DecodeFuncArray[PROTOCLITLC_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
