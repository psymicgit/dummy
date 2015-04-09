/*       Version Number: 3          */

#ifndef PROTOGTLS_H_INCLUDE_VERSION_3
#define PROTOGTLS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "protogtcommon.h"
#include "protoclils.h"
#include "protogsls.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define MAX_IP_LENGTH            56
#define PROTOGTLS_MSGID_RANGE 1404
#define PROTOGTLS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_GTLS_LOGIN_REQ{
	tagPKT_GTLS_LOGIN_REQ() { memset(this, 0, sizeof(tagPKT_GTLS_LOGIN_REQ)); }
	UINT32           dwCliVersion; //客户端版本号,取2位小数点(/100)
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //通知号
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[USERNAME_LEN]; //密码
	UINT16           wZoneID; //区ID
	UINT32           dwPlayerID; //玩家ID，老用户登录有效
	UINT16           wCareerID; //职业，创建新玩家(PlayerID为0)有效
	UINT8            byAuthMethod; //认证方式
	UINT32           dwSerialNumber; //流水号
	DT_EXT_FUNC_PARAM_LST stExtFuncParamLst; //扩展功能参数列表
	TCHAR            aszIP[MAX_IP_LENGTH]; //IP
}PKT_GTLS_LOGIN_REQ;

typedef struct tagPKT_GTLS_LOGIN_ACK{
	tagPKT_GTLS_LOGIN_ACK() { memset(this, 0, sizeof(tagPKT_GTLS_LOGIN_ACK)); }
	UINT16           wErrCode; //用户身份认证
	DT_VERSION_DATA  stVersionInfo; //版本信息
	DT_NOTICE_DATA   stNoticeInfo1; //公告信息
	DT_NOTICE_DATA   stNoticeInfo2; //公告信息
	UINT8            byOtherDeviceLoginFlag; //是否其他设备登录过
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //通知号
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	UINT16           wZoneID; //区ID
	TCHAR            aszZoneName[USERNAME_LEN]; //区名称
	UINT32           dwPlayerID; //玩家ID，老用户登录有效
	UINT16           wCareerID; //职业，创建新玩家(PlayerID为0)有效
	UINT32           dwUserID; //用户ID
	DT_EXT_FUNC_PARAM_LST stExtFuncParamLst; //扩展功能参数列表
}PKT_GTLS_LOGIN_ACK;

typedef struct tagPKT_GTLS_LOGIN_EX_REQ{
	tagPKT_GTLS_LOGIN_EX_REQ() { memset(this, 0, sizeof(tagPKT_GTLS_LOGIN_EX_REQ)); }
	UINT32           dwCliVersion; //客户端版本号,取2位小数点(/100)
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //通知号
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[USERNAME_LEN]; //密码
	UINT16           wZoneID; //区ID
	UINT32           dwPlayerID; //玩家ID，老用户登录有效
	UINT16           wCareerID; //职业，创建新玩家(PlayerID为0)有效
	UINT8            byAuthMethod; //认证方式
	TCHAR            aszIP[MAX_IP_LENGTH]; //IP
}PKT_GTLS_LOGIN_EX_REQ;

typedef struct tagPKT_GTLS_LOGIN_EX_ACK{
	tagPKT_GTLS_LOGIN_EX_ACK() { memset(this, 0, sizeof(tagPKT_GTLS_LOGIN_EX_ACK)); }
	UINT16           wErrCode; //用户身份认证
	DT_VERSION_DATA  stVersionInfo; //版本信息
	DT_NOTICE_DATA   stNoticeInfo1; //公告信息
	DT_NOTICE_DATA   stNoticeInfo2; //公告信息
	UINT8            byOtherDeviceLoginFlag; //是否其他设备登录过
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //通知号
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	UINT16           wZoneID; //区ID
	TCHAR            aszZoneName[USERNAME_LEN]; //区名称
	UINT32           dwPlayerID; //玩家ID，老用户登录有效
	UINT16           wCareerID; //职业，创建新玩家(PlayerID为0)有效
	UINT32           dwUserID; //用户ID
}PKT_GTLS_LOGIN_EX_ACK;

typedef struct tagPKT_GTLS_CHANGE_PWD_REQ{
	tagPKT_GTLS_CHANGE_PWD_REQ() { memset(this, 0, sizeof(tagPKT_GTLS_CHANGE_PWD_REQ)); }
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[USERNAME_LEN]; //密码
	UINT8            byNewPwdLen; //新密码长度
	UINT8            abyNewUserPwd[USERNAME_LEN]; //新密码
	UINT32           dwUserID; //用户ID
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备ID
}PKT_GTLS_CHANGE_PWD_REQ;

/*Define Structs and Unions        End*/


enum EN_ProtoGTLS_MessageID
{
	GTLS_LOGIN_REQ                  = 1351        ,
	GTLS_LOGIN_ACK                  = 1352        ,
	GTLS_LOGIN_EX_REQ               = 1353        ,
	GTLS_LOGIN_EX_ACK               = 1354        ,
	GTLS_CHANGE_PWD_REQ             = 1355        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_GTLS_LOGIN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GTLS_LOGIN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GTLS_LOGIN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GTLS_LOGIN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GTLS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GTLS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GTLS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GTLS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GTLS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GTLS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGTLS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGTLS)(void *pHost, CNetData* poNetData);

class CProtoGTLS : public CSDProtocol
{
public:
    static CProtoGTLS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGTLS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGTLS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGTLS();
	virtual ~CProtoGTLS();
	EnFuncCProtoGTLS m_EncodeFuncArray[PROTOGTLS_MSGID_RANGE];
	DeFuncCProtoGTLS m_DecodeFuncArray[PROTOGTLS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
