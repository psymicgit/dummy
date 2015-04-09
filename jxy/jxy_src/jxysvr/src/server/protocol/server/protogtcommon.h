/*       Version Number: 3          */

#ifndef PROTOGTCOMMON_H_INCLUDE_VERSION_3
#define PROTOGTCOMMON_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define GTCOM_DOWN_SHUTDOWN      0
#define GTCOM_DOWN_TIMEOUT       1
#define PROTOGTCOMMON_MSGID_RANGE 1201
#define PROTOGTCOMMON_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_GTCOM_CLIENT_DOWN_RPT{
	tagPKT_GTCOM_CLIENT_DOWN_RPT() { memset(this, 0, sizeof(tagPKT_GTCOM_CLIENT_DOWN_RPT)); }
	UINT8            byDownType; //Client断线，Gate Server给其他服务器发送报告
}PKT_GTCOM_CLIENT_DOWN_RPT;

/*Define Structs and Unions        End*/


enum EN_ProtoGTCommon_MessageID
{
	GTCOM_CLIENT_DOWN_RPT           = 1200        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_GTCOM_CLIENT_DOWN_RPT(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GTCOM_CLIENT_DOWN_RPT(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGTCommon)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGTCommon)(void *pHost, CNetData* poNetData);

class CProtoGTCommon : public CSDProtocol
{
public:
    static CProtoGTCommon * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGTCommon FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGTCommon FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGTCommon();
	virtual ~CProtoGTCommon();
	EnFuncCProtoGTCommon m_EncodeFuncArray[PROTOGTCOMMON_MSGID_RANGE];
	DeFuncCProtoGTCommon m_DecodeFuncArray[PROTOGTCOMMON_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;
};

#pragma pack(pop)
#endif
