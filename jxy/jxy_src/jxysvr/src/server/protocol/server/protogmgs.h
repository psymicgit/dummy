/*       Version Number: 1          */

#ifndef PROTOGMGS_H_INCLUDE_VERSION_1
#define PROTOGMGS_H_INCLUDE_VERSION_1

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define PROTOGMGS_MSGID_RANGE 1502
#define PROTOGMGS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_GMGS_PARM_INFO_REQ{
	tagPKT_GMGS_PARM_INFO_REQ() { memset(this, 0, sizeof(tagPKT_GMGS_PARM_INFO_REQ)); }
	UINT32           dwSerialNumber; //流水号
	UINT8            byAction; //事件类型
	UINT32           dwAreaID; //区域ID
	UINT32           dwPlayerID; //玩家ID
	UINT32           dwParam1; //参数1
	UINT64           qwParam2; //参数2
	UINT64           qwParam3; //参数3
	TCHAR            aszParam4[MAX_PARM_LEN]; //字符参数
}PKT_GMGS_PARM_INFO_REQ;

typedef struct tagPKT_GMGS_PARM_INFO_ACK{
	tagPKT_GMGS_PARM_INFO_ACK() { memset(this, 0, sizeof(tagPKT_GMGS_PARM_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwSerialNumber; //流水号
	UINT32           dwPlayerID; //玩家ID
}PKT_GMGS_PARM_INFO_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoGMGS_MessageID
{
	GMGS_PARM_INFO_REQ              = 1500        ,
	GMGS_PARM_INFO_ACK              = 1501        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_GMGS_PARM_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GMGS_PARM_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GMGS_PARM_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GMGS_PARM_INFO_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGMGS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGMGS)(void *pHost, CNetData* poNetData);

class CProtoGMGS : public CSDProtocol
{
public:
    static CProtoGMGS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGMGS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGMGS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGMGS();
	virtual ~CProtoGMGS();
	EnFuncCProtoGMGS m_EncodeFuncArray[PROTOGMGS_MSGID_RANGE];
	DeFuncCProtoGMGS m_DecodeFuncArray[PROTOGMGS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;
};

#pragma pack(pop)
#endif
