/*       Version Number: 3          */

#ifndef PROTOGSLS_H_INCLUDE_VERSION_3
#define PROTOGSLS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define PROTOGSLS_MSGID_RANGE 1404
#define PROTOGSLS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_GSLS_PLAYER_UPGRADE_NTF{
	tagPKT_GSLS_PLAYER_UPGRADE_NTF() { memset(this, 0, sizeof(tagPKT_GSLS_PLAYER_UPGRADE_NTF)); }
	UINT32           dwPlayerID; //GS向LS告知玩家等级更新
	UINT16           wLevel; //GS向LS告知玩家等级更新
}PKT_GSLS_PLAYER_UPGRADE_NTF;

typedef struct tagPKT_GSLS_DSPNAME_UPGRADE_NTF{
	tagPKT_GSLS_DSPNAME_UPGRADE_NTF() { memset(this, 0, sizeof(tagPKT_GSLS_DSPNAME_UPGRADE_NTF)); }
	UINT32           dwPlayerID; //GS向LS告知玩家等级更新
	TCHAR            aszDspName[USERNAME_LEN]; //GS向LS告知玩家等级更新
}PKT_GSLS_DSPNAME_UPGRADE_NTF;

typedef struct tagPKT_GSLS_ACTIVITY_NOTIFY_NTF{
	tagPKT_GSLS_ACTIVITY_NOTIFY_NTF() { memset(this, 0, sizeof(tagPKT_GSLS_ACTIVITY_NOTIFY_NTF)); }
	UINT32           dwPlayerID; //GS向LS告知玩家等级更新
	UINT8            byActivityType; //类型
	UINT8            byNotify; //通知
}PKT_GSLS_ACTIVITY_NOTIFY_NTF;

/*Define Structs and Unions        End*/


enum EN_ProtoGSLS_MessageID
{
	GSLS_PLAYER_UPGRADE_NTF         = 1400        ,
	GSLS_DSPNAME_UPGRADE_NTF        = 1402        ,
	GSLS_ACTIVITY_NOTIFY_NTF        = 1403        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_GSLS_PLAYER_UPGRADE_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSLS_PLAYER_UPGRADE_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSLS_DSPNAME_UPGRADE_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSLS_DSPNAME_UPGRADE_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSLS_ACTIVITY_NOTIFY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSLS_ACTIVITY_NOTIFY_NTF(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGSLS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGSLS)(void *pHost, CNetData* poNetData);

class CProtoGSLS : public CSDProtocol
{
public:
    static CProtoGSLS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGSLS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGSLS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGSLS();
	virtual ~CProtoGSLS();
	EnFuncCProtoGSLS m_EncodeFuncArray[PROTOGSLS_MSGID_RANGE];
	DeFuncCProtoGSLS m_DecodeFuncArray[PROTOGSLS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
