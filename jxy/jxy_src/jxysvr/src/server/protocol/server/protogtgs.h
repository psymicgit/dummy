/*       Version Number: 3          */

#ifndef PROTOGTGS_H_INCLUDE_VERSION_3
#define PROTOGTGS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "protogtcommon.h"
#include "protocligs.h"
#include "protogsls.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define PROTOGTGS_MSGID_RANGE 3194
#define PROTOGTGS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_GSGT_CLR_CACHE_RPT{
	tagPKT_GSGT_CLR_CACHE_RPT() { memset(this, 0, sizeof(tagPKT_GSGT_CLR_CACHE_RPT)); }
	UINT8            byExt; //扩展字段，0为状态判断是否重新登录，1为强制踢下线
}PKT_GSGT_CLR_CACHE_RPT;

typedef struct tagPKT_GSGT_REGISTER_USERNAME_REQ{
	tagPKT_GSGT_REGISTER_USERNAME_REQ() { memset(this, 0, sizeof(tagPKT_GSGT_REGISTER_USERNAME_REQ)); }
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
}PKT_GSGT_REGISTER_USERNAME_REQ;

typedef struct tagPKT_GSGT_REGISTER_USERNAME_ACK{
	tagPKT_GSGT_REGISTER_USERNAME_ACK() { memset(this, 0, sizeof(tagPKT_GSGT_REGISTER_USERNAME_ACK)); }
	UINT16           wErrCode; //错误码
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
}PKT_GSGT_REGISTER_USERNAME_ACK;

typedef struct tagPKT_GTGS_ENTERGS_REQ{
	tagPKT_GTGS_ENTERGS_REQ() { memset(this, 0, sizeof(tagPKT_GTGS_ENTERGS_REQ)); }
	TCHAR            aszDeviceID[41]; //设备号
	TCHAR            aszNotifyID[65]; //通知号
	TCHAR            aszUserName[41]; //用户名
	UINT16           wZoneID; //区ID
	UINT16           wOriZoneID; //玩家原来所在的区（即玩家创建角色时所在的区，这个区ID是用于不会变的）
	UINT32           dwUserID; //用户ID
	UINT32           dwPlayerID; //玩家ID
	UINT16           wCareerID; //职业，新游戏有效
	UINT32           dwLoginIP; //登录IP
	UINT32           dwCliVer; //客户端版本
	UINT8            byAuthType; //认证类型
	UINT8            byDaiChong; //代充标志
}PKT_GTGS_ENTERGS_REQ;

typedef struct tagPKT_GSGT_KICK_OUT_REQ{
	tagPKT_GSGT_KICK_OUT_REQ() { memset(this, 0, sizeof(tagPKT_GSGT_KICK_OUT_REQ)); }
	UINT32           dwPlayerID; //PlayerID
	UINT8            byKickOutType; //T下线类型 0为GM 1用于回档
}PKT_GSGT_KICK_OUT_REQ;

typedef struct tagPKT_GSGT_KICK_OUT_ACK{
	tagPKT_GSGT_KICK_OUT_ACK() { memset(this, 0, sizeof(tagPKT_GSGT_KICK_OUT_ACK)); }
	TCHAR            aszErrMsg[128]; //错误信息
}PKT_GSGT_KICK_OUT_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoGTGS_MessageID
{
	GSGT_CLR_CACHE_RPT              = 1300        ,
	GSGT_REGISTER_USERNAME_REQ      = 1302        ,
	GSGT_REGISTER_USERNAME_ACK      = 1303        ,
	GTGS_ENTERGS_REQ                = 1304        ,
	GSGT_KICK_OUT_REQ               = 1306        ,
	GSGT_KICK_OUT_ACK               = 1307        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_GSGT_CLR_CACHE_RPT(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSGT_CLR_CACHE_RPT(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSGT_REGISTER_USERNAME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSGT_REGISTER_USERNAME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSGT_REGISTER_USERNAME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSGT_REGISTER_USERNAME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GTGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GTGS_ENTERGS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSGT_KICK_OUT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSGT_KICK_OUT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSGT_KICK_OUT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSGT_KICK_OUT_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGTGS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGTGS)(void *pHost, CNetData* poNetData);

class CProtoGTGS : public CSDProtocol
{
public:
    static CProtoGTGS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGTGS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGTGS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGTGS();
	virtual ~CProtoGTGS();
	EnFuncCProtoGTGS m_EncodeFuncArray[PROTOGTGS_MSGID_RANGE];
	DeFuncCProtoGTGS m_DecodeFuncArray[PROTOGTGS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
