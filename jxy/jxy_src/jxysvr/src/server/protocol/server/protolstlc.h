/*       Version Number: 3          */

#ifndef PROTOLSTLC_H_INCLUDE_VERSION_3
#define PROTOLSTLC_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define MAX_CONTEXT_LENGTH       512
#define NAME_LEN                 41
#define PWD_LEN                  17
#define PROTOLSTLC_MSGID_RANGE 2
#define PROTOLSTLC_MSGID_OFFSET 900
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_LSTLC_AUTH_REQ{
	tagPKT_LSTLC_AUTH_REQ() { memset(this, 0, sizeof(tagPKT_LSTLC_AUTH_REQ)); }
	UINT8            byAuthType; //认证类型
	UINT8            byAuthMethod; //认证方式
	TCHAR            aszDeviceID[NAME_LEN]; //设备号
	TCHAR            aszUserName[NAME_LEN]; //用户名
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[PWD_LEN]; //密码
	UINT32           dwParam1; //扩展字段
	UINT32           dwParam2; //扩展字段
	UINT32           dwParam3; //扩展字段
	UINT64           qwParam4; //扩展字段
	TCHAR            aszExtContext[MAX_CONTEXT_LENGTH]; //扩展字段
}PKT_LSTLC_AUTH_REQ;

typedef struct tagPKT_LSTLC_AUTH_ACK{
	tagPKT_LSTLC_AUTH_ACK() { memset(this, 0, sizeof(tagPKT_LSTLC_AUTH_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            byAuthType; //认证类型
	UINT8            byAuthMethod; //认证方式
	TCHAR            aszDeviceID[NAME_LEN]; //设备号
	TCHAR            aszUserName[NAME_LEN]; //用户名
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[PWD_LEN]; //密码
	UINT32           dwParam1; //扩展字段
	UINT32           dwParam2; //扩展字段
	UINT32           dwParam3; //扩展字段
	UINT64           qwParam4; //扩展字段
	TCHAR            aszExtContext[MAX_CONTEXT_LENGTH]; //扩展字段
}PKT_LSTLC_AUTH_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoLSTLC_MessageID
{
	LSTLC_AUTH_REQ                  = 900         ,
	LSTLC_AUTH_ACK                  = 901         ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_LSTLC_AUTH_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_LSTLC_AUTH_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_LSTLC_AUTH_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_LSTLC_AUTH_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoLSTLC)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoLSTLC)(void *pHost, CNetData* poNetData);

class CProtoLSTLC : public CSDProtocol
{
public:
    static CProtoLSTLC * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoLSTLC FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoLSTLC FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoLSTLC();
	virtual ~CProtoLSTLC();
	EnFuncCProtoLSTLC m_EncodeFuncArray[PROTOLSTLC_MSGID_RANGE];
	DeFuncCProtoLSTLC m_DecodeFuncArray[PROTOLSTLC_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
