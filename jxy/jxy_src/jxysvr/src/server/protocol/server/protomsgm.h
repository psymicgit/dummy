/*       Version Number: 1          */

#ifndef PROTOMSGM_H_INCLUDE_VERSION_1
#define PROTOMSGM_H_INCLUDE_VERSION_1

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define SERVER_NAME_LEN          56
#define SERVER_COUNT             10
#define GROUP_COUNT              10
#define PROTOMSGM_MSGID_RANGE 1604
#define PROTOMSGM_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_GMGS_CONTROL_REQ{
	tagPKT_GMGS_CONTROL_REQ() { memset(this, 0, sizeof(tagPKT_GMGS_CONTROL_REQ)); }
	UINT32           dwSerialNumber; //流水号
	UINT32           dwAreaID; //区域ID
	TCHAR            aszServer[SERVER_NAME_LEN]; //服务名称
	UINT8            byUseFlag; //类型
}PKT_GMGS_CONTROL_REQ;

typedef struct tagPKT_GMGS_CONTROL_ACK{
	tagPKT_GMGS_CONTROL_ACK() { memset(this, 0, sizeof(tagPKT_GMGS_CONTROL_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwSerialNumber; //流水号
}PKT_GMGS_CONTROL_ACK;

typedef struct tagDT_SERVER_DATA{
	UINT8            byKeepAlive; //是否存活
	TCHAR            aszServer[SERVER_NAME_LEN]; //服务器名称
}DT_SERVER_DATA;

typedef struct tagDT_GROUP_DATA{
	UINT8            byUseFlag; //是否使用
	UINT8            byNum; //个数
	DT_SERVER_DATA   astServerDataList[SERVER_COUNT]; //是否存活
}DT_GROUP_DATA;

typedef struct tagPKT_GMGS_GET_SERVER_INFO_REQ{
	tagPKT_GMGS_GET_SERVER_INFO_REQ() { memset(this, 0, sizeof(tagPKT_GMGS_GET_SERVER_INFO_REQ)); }
	UINT32           dwSerialNumber; //流水号
	UINT32           dwAreaID; //区域ID
}PKT_GMGS_GET_SERVER_INFO_REQ;

typedef struct tagPKT_GMGS_GET_SERVER_INFO_ACK{
	tagPKT_GMGS_GET_SERVER_INFO_ACK() { memset(this, 0, sizeof(tagPKT_GMGS_GET_SERVER_INFO_ACK)); }
	UINT16           wErrCode; //错误码，非0为错误
	UINT32           dwSerialNumber; //流水号
	UINT32           dwAreaID; //区域ID
	UINT8            byGroupNum; //个数
	DT_GROUP_DATA    astGroupDataList[GROUP_COUNT]; //数据
}PKT_GMGS_GET_SERVER_INFO_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoMSGM_MessageID
{
	GMGS_CONTROL_REQ                = 1600        ,
	GMGS_CONTROL_ACK                = 1601        ,
	GMGS_GET_SERVER_INFO_REQ        = 1602        ,
	GMGS_GET_SERVER_INFO_ACK        = 1603        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_GMGS_CONTROL_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GMGS_CONTROL_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GMGS_CONTROL_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GMGS_CONTROL_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_SERVER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SERVER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_GROUP_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GMGS_GET_SERVER_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GMGS_GET_SERVER_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GMGS_GET_SERVER_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GMGS_GET_SERVER_INFO_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoMSGM)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoMSGM)(void *pHost, CNetData* poNetData);

class CProtoMSGM : public CSDProtocol
{
public:
    static CProtoMSGM * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoMSGM FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoMSGM FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoMSGM();
	virtual ~CProtoMSGM();
	EnFuncCProtoMSGM m_EncodeFuncArray[PROTOMSGM_MSGID_RANGE];
	DeFuncCProtoMSGM m_DecodeFuncArray[PROTOMSGM_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
