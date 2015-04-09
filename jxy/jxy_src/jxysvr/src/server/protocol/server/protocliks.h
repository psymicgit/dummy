/*       Version Number: 3          */

#ifndef PROTOCLIKS_H_INCLUDE_VERSION_3
#define PROTOCLIKS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define INFO_LEN                 512
#define PROTOCLIKS_MSGID_RANGE 3003
#define PROTOCLIKS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_CLIKS_LOGIN_REQ{
	tagPKT_CLIKS_LOGIN_REQ() { memset(this, 0, sizeof(tagPKT_CLIKS_LOGIN_REQ)); }
	UINT16           wID; //获取区信息
}PKT_CLIKS_LOGIN_REQ;

typedef struct tagPKT_CLIKS_LOGIN_ACK{
	tagPKT_CLIKS_LOGIN_ACK() { memset(this, 0, sizeof(tagPKT_CLIKS_LOGIN_ACK)); }
	UINT32           dwErrCode; //获取区信息
	UINT32           dwZoneIP; //获取区信息
	UINT16           wZonePort; //获取区信息
}PKT_CLIKS_LOGIN_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoCliKS_MessageID
{
	CLIKS_LOGIN_REQ                 = 3001        ,
	CLIKS_LOGIN_ACK                 = 3002        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_CLIKS_LOGIN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIKS_LOGIN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIKS_LOGIN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIKS_LOGIN_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoCliKS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoCliKS)(void *pHost, CNetData* poNetData);

class CProtoCliKS : public CSDProtocol
{
public:
    static CProtoCliKS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoCliKS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoCliKS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoCliKS();
	virtual ~CProtoCliKS();
	EnFuncCProtoCliKS m_EncodeFuncArray[PROTOCLIKS_MSGID_RANGE];
	DeFuncCProtoCliKS m_DecodeFuncArray[PROTOCLIKS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
