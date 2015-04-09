/*       Version Number: 3          */

#ifndef PROTOCLILS_H_INCLUDE_VERSION_3
#define PROTOCLILS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define INFO_LEN                 512
#define PROTOCLILS_MSGID_RANGE 1068
#define PROTOCLILS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_CLILS_LOGIN_REQ{
	tagPKT_CLILS_LOGIN_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_LOGIN_REQ)); }
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
}PKT_CLILS_LOGIN_REQ;

typedef struct tagPKT_CLILS_LOGIN_ACK{
	tagPKT_CLILS_LOGIN_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_LOGIN_ACK)); }
	UINT16           wErrCode; //用户身份认证
	DT_VERSION_DATA  stVersionInfo; //版本信息
	DT_NOTICE_DATA   stNoticeInfo; //公告信息
	UINT8            byOtherDeviceLoginFlag; //是否其他设备登录过 
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号 
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //通知号 
	TCHAR            aszUserName[USERNAME_LEN]; //用户名 
	UINT16           wZoneID; //区ID
	TCHAR            aszZoneName[USERNAME_LEN]; //区名称 
	UINT32           dwPlayerID; //玩家ID，老用户登录有效 
	UINT16           wCareerID; //职业，创建新玩家(PlayerID为0)有效
	DT_EXT_FUNC_PARAM_LST stExtFuncParamLst; //扩展功能参数列表
}PKT_CLILS_LOGIN_ACK;

typedef struct tagPKT_CLILS_LOGIN_EX_REQ{
	tagPKT_CLILS_LOGIN_EX_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_LOGIN_EX_REQ)); }
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
}PKT_CLILS_LOGIN_EX_REQ;

typedef struct tagPKT_CLILS_LOGIN_EX_ACK{
	tagPKT_CLILS_LOGIN_EX_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_LOGIN_EX_ACK)); }
	UINT16           wErrCode; //用户身份认证
	DT_VERSION_DATA  stVersionInfo; //版本信息
	DT_NOTICE_DATA   stNoticeInfo; //公告信息
	UINT8            byOtherDeviceLoginFlag; //是否其他设备登录过 
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号 
	TCHAR            aszNotifyID[NOTIFYID_LEN]; //通知号 
	TCHAR            aszUserName[USERNAME_LEN]; //用户名 
	UINT16           wZoneID; //区ID
	TCHAR            aszZoneName[USERNAME_LEN]; //区名称 
	UINT32           dwPlayerID; //玩家ID，老用户登录有效 
	UINT16           wCareerID; //职业，创建新玩家(PlayerID为0)有效
}PKT_CLILS_LOGIN_EX_ACK;

typedef struct tagPKT_CLILS_REGISTER_REQ{
	tagPKT_CLILS_REGISTER_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_REGISTER_REQ)); }
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号 
	TCHAR            aszUserName[USERNAME_LEN]; //用户名 
	TCHAR            aszUserPwd[USERPWD_LEN]; //密码
	UINT32           dwUserID; //客户端不需填写
}PKT_CLILS_REGISTER_REQ;

typedef struct tagPKT_CLILS_REGISTER_ACK{
	tagPKT_CLILS_REGISTER_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_REGISTER_ACK)); }
	UINT16           wErrCode; //错误码
	TCHAR            aszUserName[USERNAME_LEN]; //用户名 
}PKT_CLILS_REGISTER_ACK;

typedef struct tagPKT_CLILS_BIND_REQ{
	tagPKT_CLILS_BIND_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_BIND_REQ)); }
	UINT8            byAuthType; //认证类型
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszUserName[USERNAME_LEN]; //用户名 
	TCHAR            aszNickName[USERNAME_LEN]; //用户昵称
	UINT8            byTokenLen; //用户TokenKey长度
	UINT8            abyTokenKey[USERNAME_LEN]; //用户TokenKey
}PKT_CLILS_BIND_REQ;

typedef struct tagPKT_CLILS_BIND_ACK{
	tagPKT_CLILS_BIND_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_BIND_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLILS_BIND_ACK;

typedef struct tagPKT_CLILS_BIND_EX_REQ{
	tagPKT_CLILS_BIND_EX_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_BIND_EX_REQ)); }
	UINT8            byAuthType; //认证类型
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszUserName[USERNAME_LEN]; //用户名 
	TCHAR            aszNickName[USERNAME_LEN]; //用户昵称
	UINT8            byTokenLen; //用户TokenKey长度
	UINT8            abyTokenKey[USERNAME_LEN]; //用户TokenKey
}PKT_CLILS_BIND_EX_REQ;

typedef struct tagPKT_CLILS_BIND_EX_ACK{
	tagPKT_CLILS_BIND_EX_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_BIND_EX_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLILS_BIND_EX_ACK;

typedef struct tagPKT_CLILS_UNBIND_REQ{
	tagPKT_CLILS_UNBIND_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_UNBIND_REQ)); }
	UINT8            byAuthType; //认证类型
	UINT32           dwPlayerID; //玩家ID
	TCHAR            aszUserName[USERNAME_LEN]; //用户名 
}PKT_CLILS_UNBIND_REQ;

typedef struct tagPKT_CLILS_UNBIND_ACK{
	tagPKT_CLILS_UNBIND_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_UNBIND_ACK)); }
	UINT16           wErrCode; //用户身份认证
}PKT_CLILS_UNBIND_ACK;

typedef struct tagPKT_CLILS_LOGOUT_REQ{
	tagPKT_CLILS_LOGOUT_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_LOGOUT_REQ)); }
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备ID
	UINT32           dwUserID; //用户ID
}PKT_CLILS_LOGOUT_REQ;

typedef struct tagPKT_CLILS_LOGOUT_ACK{
	tagPKT_CLILS_LOGOUT_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_LOGOUT_ACK)); }
	UINT16           wErrCode; //用户身份认证
}PKT_CLILS_LOGOUT_ACK;

typedef struct tagPKT_CLILS_CHANGE_PWD_REQ{
	tagPKT_CLILS_CHANGE_PWD_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_CHANGE_PWD_REQ)); }
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[USERNAME_LEN]; //密码
	UINT8            byNewPwdLen; //新密码长度 
	UINT8            abyNewUserPwd[USERNAME_LEN]; //新密码 
	UINT32           dwUserID; //用户ID
}PKT_CLILS_CHANGE_PWD_REQ;

typedef struct tagPKT_CLILS_CHANGE_PWD_ACK{
	tagPKT_CLILS_CHANGE_PWD_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_CHANGE_PWD_ACK)); }
	UINT16           wErrCode; //用户身份认证
}PKT_CLILS_CHANGE_PWD_ACK;

typedef struct tagPKT_CLILS_BIND_MOBILE_REQ{
	tagPKT_CLILS_BIND_MOBILE_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_BIND_MOBILE_REQ)); }
	UINT8            abyMObile[MAX_MOBILE_LENGTH]; //用户手机号 
	UINT32           dwUserID; //用户ID
}PKT_CLILS_BIND_MOBILE_REQ;

typedef struct tagPKT_CLILS_BIND_MOBILE_ACK{
	tagPKT_CLILS_BIND_MOBILE_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_BIND_MOBILE_ACK)); }
	UINT16           wErrCode; //用户身份认证
	UINT16           wCountdown; //倒计时 
}PKT_CLILS_BIND_MOBILE_ACK;

typedef struct tagPKT_CLILS_VER_CODE_REQ{
	tagPKT_CLILS_VER_CODE_REQ() { memset(this, 0, sizeof(tagPKT_CLILS_VER_CODE_REQ)); }
	UINT8            abyVerCode[MAX_MOBILE_LENGTH]; //验证码 
	UINT32           dwUserID; //用户ID
}PKT_CLILS_VER_CODE_REQ;

typedef struct tagPKT_CLILS_VER_CODE_ACK{
	tagPKT_CLILS_VER_CODE_ACK() { memset(this, 0, sizeof(tagPKT_CLILS_VER_CODE_ACK)); }
	UINT16           wErrCode; //用户身份认证
}PKT_CLILS_VER_CODE_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoCliLS_MessageID
{
	CLILS_LOGIN_REQ                 = 1050        ,
	CLILS_LOGIN_ACK                 = 1051        ,
	CLILS_REGISTER_REQ              = 1052        ,
	CLILS_REGISTER_ACK              = 1053        ,
	CLILS_BIND_REQ                  = 1054        ,
	CLILS_BIND_ACK                  = 1055        ,
	CLILS_UNBIND_REQ                = 1056        ,
	CLILS_UNBIND_ACK                = 1057        ,
	CLILS_LOGIN_EX_REQ              = 1058        ,
	CLILS_LOGIN_EX_ACK              = 1059        ,
	CLILS_LOGOUT_REQ                = 1060        ,
	CLILS_LOGOUT_ACK                = 1061        ,
	CLILS_CHANGE_PWD_REQ            = 1062        ,
	CLILS_CHANGE_PWD_ACK            = 1063        ,
	CLILS_BIND_MOBILE_ACK           = 1064        ,
	CLILS_BIND_MOBILE_REQ           = 1065        ,
	CLILS_VER_CODE_REQ              = 1066        ,
	CLILS_VER_CODE_ACK              = 1067        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_CLILS_LOGIN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_LOGIN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_LOGIN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_LOGIN_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_LOGIN_EX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_LOGIN_EX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_REGISTER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_REGISTER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_REGISTER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_BIND_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_BIND_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_BIND_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_BIND_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_BIND_EX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_BIND_EX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_BIND_EX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_BIND_EX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_UNBIND_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_UNBIND_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_UNBIND_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_UNBIND_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_LOGOUT_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_LOGOUT_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_LOGOUT_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_LOGOUT_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_CHANGE_PWD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_CHANGE_PWD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_CHANGE_PWD_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_BIND_MOBILE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_BIND_MOBILE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_BIND_MOBILE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_BIND_MOBILE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_VER_CODE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_VER_CODE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLILS_VER_CODE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLILS_VER_CODE_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoCliLS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoCliLS)(void *pHost, CNetData* poNetData);

class CProtoCliLS : public CSDProtocol
{
public:
    static CProtoCliLS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoCliLS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoCliLS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoCliLS();
	virtual ~CProtoCliLS();
	EnFuncCProtoCliLS m_EncodeFuncArray[PROTOCLILS_MSGID_RANGE];
	DeFuncCProtoCliLS m_DecodeFuncArray[PROTOCLILS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
