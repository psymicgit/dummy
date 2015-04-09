/*       Version Number: 1          */

#ifndef PROTOGSNS_H_INCLUDE_VERSION_1
#define PROTOGSNS_H_INCLUDE_VERSION_1

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define MAX_PAYLOAD_NUM          256
#define PROTOGSNS_MSGID_RANGE 1501
#define PROTOGSNS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_GSNS_PLAYER_DEFAULT_NTF{
	tagPKT_GSNS_PLAYER_DEFAULT_NTF() { memset(this, 0, sizeof(tagPKT_GSNS_PLAYER_DEFAULT_NTF)); }
	UINT32           dwPlayerID; //GS向NS告知
	UINT8            byAction; //GS向NS告知
	UINT8            abyToken[MAX_TOKEN_NUM]; //GS向NS告知
	TCHAR            aszPayLoad[MAX_PAYLOAD_NUM]; //GS向NS告知
}PKT_GSNS_PLAYER_DEFAULT_NTF;

/*Define Structs and Unions        End*/


enum EN_ProtoGSNS_MessageID
{
	GSNS_PLAYER_DEFAULT_NTF         = 1500        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_GSNS_PLAYER_DEFAULT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSNS_PLAYER_DEFAULT_NTF(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGSNS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGSNS)(void *pHost, CNetData* poNetData);

class CProtoGSNS : public CSDProtocol
{
public:
    static CProtoGSNS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGSNS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGSNS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGSNS();
	virtual ~CProtoGSNS();
	EnFuncCProtoGSNS m_EncodeFuncArray[PROTOGSNS_MSGID_RANGE];
	DeFuncCProtoGSNS m_DecodeFuncArray[PROTOGSNS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;
};

#pragma pack(pop)
#endif
