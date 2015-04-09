/*       Version Number: 3          */

#ifndef PROTOCLIZS_H_INCLUDE_VERSION_3
#define PROTOCLIZS_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define ENDEC_KEY_LEN            32
#define INFO_LEN                 512
#define MAX_PLAYER_NUM_PER_ZONE  9
#define MAX_DOMAIN_NAME_LEN      128
#define MAX_USER_INFO_NUM        32
#define MAX_USER_DEVIDE_INFO_LEN 1024
#define MAX_INFO_NUM             128
#define MAX_RESET_PASSWD_TOKEN_NUM 128
#define MAX_ZONE_NUM             255
#define PROTOCLIZS_MSGID_RANGE 1030
#define PROTOCLIZS_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagPKT_CLIZS_ENDEC_KEY_NTF{
	tagPKT_CLIZS_ENDEC_KEY_NTF() { memset(this, 0, sizeof(tagPKT_CLIZS_ENDEC_KEY_NTF)); }
	UINT8            abyKeyInfo[ENDEC_KEY_LEN]; //加解密密钥
}PKT_CLIZS_ENDEC_KEY_NTF;

typedef struct tagPKT_CLIZS_GET_ZONE_INFO_REQ{
	tagPKT_CLIZS_GET_ZONE_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_ZONE_INFO_REQ)); }
	UINT16           wID; //加解密密钥
}PKT_CLIZS_GET_ZONE_INFO_REQ;

typedef struct tagPKT_CLIZS_GET_ZONE_INFO_ACK{
	tagPKT_CLIZS_GET_ZONE_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_ZONE_INFO_ACK)); }
	UINT32           dwErrCode; //错误码
	UINT8            byDomainLen; //域名长度
	UINT8            abyDomainName[MAX_DOMAIN_NAME_LEN]; //域名
	UINT16           wZonePort; //端口
}PKT_CLIZS_GET_ZONE_INFO_ACK;

typedef struct tagDT_ZONE_PLAYER_DATA{
	UINT32           dwPlayerID; //玩家ID
	UINT8            byPlayerCareer; //玩家职业
	UINT16           wPlayerLevel; //玩家等级
	TCHAR            aszPlayerDspName[USERNAME_LEN]; //玩家泥称
}DT_ZONE_PLAYER_DATA;

typedef struct tagDT_ZONE_DATA{
	UINT16           wID; //区ID
	UINT8            byDomainLen; //域名长度
	UINT8            abyDomainName[MAX_DOMAIN_NAME_LEN]; //域名
	UINT16           wZonePort; //端口
	TCHAR            aszZoneName[USERNAME_LEN]; //选区名称
	UINT8            byBusyFlag; //区繁忙程度(0流畅，1繁忙，2火爆, 3拒绝进入,4维护中)
	UINT8            byPlayerNum; //玩家个数
	DT_ZONE_PLAYER_DATA astPlayerInfo[MAX_PLAYER_NUM_PER_ZONE]; //玩家个数
}DT_ZONE_DATA;

typedef struct tagPKT_CLIGT_SELECT_ZONE_REQ{
	tagPKT_CLIGT_SELECT_ZONE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGT_SELECT_ZONE_REQ)); }
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[USERNAME_LEN]; //密码
	UINT16           wZoneIdx; //请求的区个数序号
	UINT8            byAuthMethod; //认证方式
	TCHAR            aszNickName[USERNAME_LEN]; //用户昵称
	UINT8            byBindType; //绑定类型 0：不绑定， 1：绑定
	UINT32           dwCliVersion; //客户端版本号,取2位小数点(/100)
}PKT_CLIGT_SELECT_ZONE_REQ;

typedef struct tagPKT_CLIGT_SELECT_ZONE_ACK{
	tagPKT_CLIGT_SELECT_ZONE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGT_SELECT_ZONE_ACK)); }
	UINT16           wErrCode; //加解密密钥
	UINT8            byZoneNum; //加解密密钥
	DT_ZONE_DATA     astZoneList[MAX_ZONE_NUM]; //加解密密钥
}PKT_CLIGT_SELECT_ZONE_ACK;

typedef struct tagPKT_CLIGT_SELECT_ZONE_EX_REQ{
	tagPKT_CLIGT_SELECT_ZONE_EX_REQ() { memset(this, 0, sizeof(tagPKT_CLIGT_SELECT_ZONE_EX_REQ)); }
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[USERNAME_LEN]; //密码
	UINT16           wZoneIdx; //请求的区个数序号
	UINT8            byAuthMethod; //认证方式
	TCHAR            aszNickName[USERNAME_LEN]; //用户昵称
}PKT_CLIGT_SELECT_ZONE_EX_REQ;

typedef struct tagPKT_CLIGT_SELECT_ZONE_EX_ACK{
	tagPKT_CLIGT_SELECT_ZONE_EX_ACK() { memset(this, 0, sizeof(tagPKT_CLIGT_SELECT_ZONE_EX_ACK)); }
	UINT16           wErrCode; //加解密密钥
	UINT8            byZoneNum; //加解密密钥
	DT_ZONE_DATA     astZoneList[GET_ZONE_NUM]; //加解密密钥
}PKT_CLIGT_SELECT_ZONE_EX_ACK;

typedef struct tagPKT_CLIGT_SELECT_ZONE_EX2_REQ{
	tagPKT_CLIGT_SELECT_ZONE_EX2_REQ() { memset(this, 0, sizeof(tagPKT_CLIGT_SELECT_ZONE_EX2_REQ)); }
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备号
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	UINT8            byPwdLen; //密码长度
	UINT8            abyUserPwd[USERNAME_LEN]; //密码
	UINT16           wZoneIdx; //请求的区个数序号
	UINT8            byAuthMethod; //认证方式
	TCHAR            aszNickName[USERNAME_LEN]; //用户昵称
	UINT8            byBindType; //绑定类型 0：不绑定， 1：绑定
}PKT_CLIGT_SELECT_ZONE_EX2_REQ;

typedef struct tagPKT_CLIGT_SELECT_ZONE_EX2_ACK{
	tagPKT_CLIGT_SELECT_ZONE_EX2_ACK() { memset(this, 0, sizeof(tagPKT_CLIGT_SELECT_ZONE_EX2_ACK)); }
	UINT16           wErrCode; //加解密密钥
	UINT8            byZoneNum; //加解密密钥
	DT_ZONE_DATA     astZoneList[GET_ZONE_NUM]; //加解密密钥
}PKT_CLIGT_SELECT_ZONE_EX2_ACK;

typedef struct tagPKT_CLIGT_FIND_ZONE_REQ{
	tagPKT_CLIGT_FIND_ZONE_REQ() { memset(this, 0, sizeof(tagPKT_CLIGT_FIND_ZONE_REQ)); }
	TCHAR            aszDeviceID[DEVICEID_LEN]; //加解密密钥
	TCHAR            aszUserName[USERNAME_LEN]; //加解密密钥
	TCHAR            aszUserPwd[USERPWD_LEN]; //加解密密钥
	TCHAR            aszZoneName[USERNAME_LEN]; //加解密密钥
}PKT_CLIGT_FIND_ZONE_REQ;

typedef struct tagPKT_CLIGT_FIND_ZONE_ACK{
	tagPKT_CLIGT_FIND_ZONE_ACK() { memset(this, 0, sizeof(tagPKT_CLIGT_FIND_ZONE_ACK)); }
	UINT16           wErrCode; //加解密密钥
	UINT8            byZoneNum; //加解密密钥
	DT_ZONE_DATA     astZoneList[GET_ZONE_NUM]; //加解密密钥
}PKT_CLIGT_FIND_ZONE_ACK;

typedef struct tagPKT_CLIZS_GET_VERSION_NOTICE_REQ{
	tagPKT_CLIZS_GET_VERSION_NOTICE_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_VERSION_NOTICE_REQ)); }
	UINT32           dwCliVersion; //客户端版本号,取2位小数点(/100)
}PKT_CLIZS_GET_VERSION_NOTICE_REQ;

typedef struct tagPKT_CLIZS_GET_VERSION_NOTICE_ACK{
	tagPKT_CLIZS_GET_VERSION_NOTICE_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_VERSION_NOTICE_ACK)); }
	DT_VERSION_DATA  stVersionInfo; //版本信息
	DT_NOTICE_DATA   stNoticeInfo; //公告信息
}PKT_CLIZS_GET_VERSION_NOTICE_ACK;

typedef struct tagPKT_CLIZS_GET_USERINFO_REQ{
	tagPKT_CLIZS_GET_USERINFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_USERINFO_REQ)); }
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备ID
}PKT_CLIZS_GET_USERINFO_REQ;

typedef struct tagDT_USERINFO{
	UINT8            byType; //用户类型
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	TCHAR            aszNickName[USERNAME_LEN]; //用户昵称
}DT_USERINFO;

typedef struct tagPKT_CLIZS_GET_USERINFO_ACK{
	tagPKT_CLIZS_GET_USERINFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_USERINFO_ACK)); }
	UINT32           dwErrCode; //错误码
	UINT8            byNum; //用户信息个数
	DT_USERINFO      astUserInfo[MAX_USER_INFO_NUM]; //域名
}PKT_CLIZS_GET_USERINFO_ACK;

typedef struct tagPKT_CLIZS_REGISTER_USERNAME_REQ{
	tagPKT_CLIZS_REGISTER_USERNAME_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_REGISTER_USERNAME_REQ)); }
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备ID
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
	TCHAR            aszUserPwd[USERPWD_LEN]; //密码
}PKT_CLIZS_REGISTER_USERNAME_REQ;

typedef struct tagPKT_CLIZS_REGISTER_USERNAME_ACK{
	tagPKT_CLIZS_REGISTER_USERNAME_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_REGISTER_USERNAME_ACK)); }
	UINT16           wErrCode; //加解密密钥
}PKT_CLIZS_REGISTER_USERNAME_ACK;

typedef struct tagPKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ{
	tagPKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ)); }
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备ID
	TCHAR            aszPlatform[MAX_INFO_NUM]; //安装平台
	TCHAR            aszGameVersion[MAX_INFO_NUM]; //游戏版本号
	TCHAR            aszOSType[MAX_INFO_NUM]; //系统类型
	TCHAR            aszOSVersion[MAX_INFO_NUM]; //系统版本
	TCHAR            aszHardwareVersion[MAX_INFO_NUM]; //硬件平台
	TCHAR            aszExtInfo[MAX_USER_DEVIDE_INFO_LEN]; //扩展信息
}PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ;

typedef struct tagPKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK{
	tagPKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK;

typedef struct tagPKT_CLIZS_UN_BIND_DEVICE_REQ{
	tagPKT_CLIZS_UN_BIND_DEVICE_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_UN_BIND_DEVICE_REQ)); }
	UINT8            byAuthType; //认证类型
	TCHAR            aszDeviceID[DEVICEID_LEN]; //设备ID
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
}PKT_CLIZS_UN_BIND_DEVICE_REQ;

typedef struct tagPKT_CLIZS_UN_BIND_DEVICE_ACK{
	tagPKT_CLIZS_UN_BIND_DEVICE_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_UN_BIND_DEVICE_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIZS_UN_BIND_DEVICE_ACK;

typedef struct tagPKT_CLIZS_GET_VERSION_NOTICE_REQ2{
	tagPKT_CLIZS_GET_VERSION_NOTICE_REQ2() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_VERSION_NOTICE_REQ2)); }
	UINT32           dwCliVersion; //客户端版本号,取2位小数点(/100)
}PKT_CLIZS_GET_VERSION_NOTICE_REQ2;

typedef struct tagDT_VERSION_DATA2{
	UINT32           dwVersionID; //版本ID,取2位小数点(/100)
	UINT8            byForceUpdateFlag; //是否强制更新，1强制更新
	UINT32           dwResVer; //资源版本
	UINT32           dwUIVer; //UI版本
	TCHAR            aszUpdateContent[MAX_CONTENT_SIZE]; //更新内容
	TCHAR            aszUpdateURL[MAX_CONTENT_SIZE]; //更新地址
}DT_VERSION_DATA2;

typedef struct tagPKT_CLIZS_GET_VERSION_NOTICE_ACK2{
	tagPKT_CLIZS_GET_VERSION_NOTICE_ACK2() { memset(this, 0, sizeof(tagPKT_CLIZS_GET_VERSION_NOTICE_ACK2)); }
	DT_VERSION_DATA2 stVersionInfo; //版本信息
	DT_NOTICE_DATA   stNoticeInfo; //公告信息
}PKT_CLIZS_GET_VERSION_NOTICE_ACK2;

typedef struct tagPKT_CLIZS_RESET_PASSWD_MOBILE_REQ{
	tagPKT_CLIZS_RESET_PASSWD_MOBILE_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_RESET_PASSWD_MOBILE_REQ)); }
	UINT8            abyMObile[MAX_MOBILE_LENGTH]; //用户手机号
}PKT_CLIZS_RESET_PASSWD_MOBILE_REQ;

typedef struct tagPKT_CLIZS_RESET_PASSWD_MOBILE_ACK{
	tagPKT_CLIZS_RESET_PASSWD_MOBILE_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_RESET_PASSWD_MOBILE_ACK)); }
	UINT16           wErrCode; //错误码
	UINT8            abyToken[MAX_RESET_PASSWD_TOKEN_NUM]; //随机码
	UINT16           wCountdown; //倒计时
}PKT_CLIZS_RESET_PASSWD_MOBILE_ACK;

typedef struct tagPKT_CLIZS_RESET_PASSWD_VER_CODE_REQ{
	tagPKT_CLIZS_RESET_PASSWD_VER_CODE_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_RESET_PASSWD_VER_CODE_REQ)); }
	UINT8            abyToken[MAX_RESET_PASSWD_TOKEN_NUM]; //随机码
	UINT8            abyVerCode[MAX_MOBILE_LENGTH]; //验证码
}PKT_CLIZS_RESET_PASSWD_VER_CODE_REQ;

typedef struct tagPKT_CLIZS_RESET_PASSWD_VER_CODE_ACK{
	tagPKT_CLIZS_RESET_PASSWD_VER_CODE_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_RESET_PASSWD_VER_CODE_ACK)); }
	UINT16           wErrCode; //验证码
	UINT16           wCountdown; //倒计时
	TCHAR            aszUserName[USERNAME_LEN]; //用户名
}PKT_CLIZS_RESET_PASSWD_VER_CODE_ACK;

typedef struct tagPKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ{
	tagPKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ() { memset(this, 0, sizeof(tagPKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ)); }
	UINT8            abyToken[MAX_RESET_PASSWD_TOKEN_NUM]; //随机码
	UINT8            byPwdLen; //密码长度
	UINT8            abyNewPassWd[USERNAME_LEN]; //密码
}PKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ;

typedef struct tagPKT_CLIZS_RESET_PASSWD_NEWPASSWD_ACK{
	tagPKT_CLIZS_RESET_PASSWD_NEWPASSWD_ACK() { memset(this, 0, sizeof(tagPKT_CLIZS_RESET_PASSWD_NEWPASSWD_ACK)); }
	UINT16           wErrCode; //错误码
}PKT_CLIZS_RESET_PASSWD_NEWPASSWD_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoCliZS_MessageID
{
	CLIZS_ENDEC_KEY_NTF             = 1000        ,
	CLIZS_GET_ZONE_INFO_REQ         = 1002        ,
	CLIZS_GET_ZONE_INFO_ACK         = 1003        ,
	CLIGT_SELECT_ZONE_REQ           = 1004        ,
	CLIGT_SELECT_ZONE_ACK           = 1005        ,
	CLIGT_FIND_ZONE_REQ             = 1006        ,
	CLIGT_FIND_ZONE_ACK             = 1007        ,
	CLIZS_GET_VERSION_NOTICE_REQ    = 1008        ,
	CLIZS_GET_VERSION_NOTICE_ACK    = 1009        ,
	CLIZS_GET_USERINFO_REQ          = 1010        ,
	CLIZS_GET_USERINFO_ACK          = 1011        ,
	CLIZS_REGISTER_USERNAME_REQ     = 1012        ,
	CLIZS_REGISTER_USERNAME_ACK     = 1013        ,
	CLIGT_SELECT_ZONE_EX_REQ        = 1014        ,
	CLIGT_SELECT_ZONE_EX_ACK        = 1015        ,
	CLIZS_REPORT_USER_DEVICE_INFO_REQ  = 1016        ,
	CLIZS_REPORT_USER_DEVICE_INFO_ACK  = 1017        ,
	CLIZS_UN_BIND_DEVICE_REQ        = 1018        ,
	CLIZS_UN_BIND_DEVICE_ACK        = 1019        ,
	CLIGT_SELECT_ZONE_EX2_REQ       = 1020        ,
	CLIGT_SELECT_ZONE_EX2_ACK       = 1021        ,
	CLIZS_GET_VERSION_NOTICE_REQ2   = 1022        ,
	CLIZS_GET_VERSION_NOTICE_ACK2   = 1023        ,
	CLIZS_RESET_PASSWD_MOBILE_REQ   = 1024        ,
	CLIZS_RESET_PASSWD_MOBILE_ACK   = 1025        ,
	CLIZS_RESET_PASSWD_VER_CODE_REQ  = 1026        ,
	CLIZS_RESET_PASSWD_VER_CODE_ACK  = 1027        ,
	CLIZS_RESET_PASSWD_NEWPASSWD_REQ  = 1028        ,
	CLIZS_RESET_PASSWD_NEWPASSWD_ACK  = 1029        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodePKT_CLIZS_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_ENDEC_KEY_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_ZONE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_ZONE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_ZONE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_ZONE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ZONE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ZONE_PLAYER_DATA(void *pHost, CNetData* poNetData);
INT32 EncodeDT_ZONE_DATA(void *pHost, CNetData* poNetData);
INT32 DecodeDT_ZONE_DATA(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_SELECT_ZONE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_SELECT_ZONE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_SELECT_ZONE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_SELECT_ZONE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_SELECT_ZONE_EX_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_SELECT_ZONE_EX_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_SELECT_ZONE_EX_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_SELECT_ZONE_EX_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_SELECT_ZONE_EX2_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_SELECT_ZONE_EX2_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_SELECT_ZONE_EX2_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_SELECT_ZONE_EX2_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_FIND_ZONE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_FIND_ZONE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIGT_FIND_ZONE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIGT_FIND_ZONE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_USERINFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_USERINFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_USERINFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_USERINFO(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_USERINFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_USERINFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_REGISTER_USERNAME_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_REGISTER_USERNAME_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_REGISTER_USERNAME_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_REGISTER_USERNAME_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_UN_BIND_DEVICE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_UN_BIND_DEVICE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_UN_BIND_DEVICE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_UN_BIND_DEVICE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_REQ2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_REQ2(void *pHost, CNetData* poNetData);
INT32 EncodeDT_VERSION_DATA2(void *pHost, CNetData* poNetData);
INT32 DecodeDT_VERSION_DATA2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_GET_VERSION_NOTICE_ACK2(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_GET_VERSION_NOTICE_ACK2(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_RESET_PASSWD_MOBILE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_RESET_PASSWD_MOBILE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_RESET_PASSWD_MOBILE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_RESET_PASSWD_MOBILE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_RESET_PASSWD_VER_CODE_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_RESET_PASSWD_VER_CODE_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_RESET_PASSWD_VER_CODE_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_RESET_PASSWD_VER_CODE_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_CLIZS_RESET_PASSWD_NEWPASSWD_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_CLIZS_RESET_PASSWD_NEWPASSWD_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoCliZS)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoCliZS)(void *pHost, CNetData* poNetData);

class CProtoCliZS : public CSDProtocol
{
public:
    static CProtoCliZS * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoCliZS FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoCliZS FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoCliZS();
	virtual ~CProtoCliZS();
	EnFuncCProtoCliZS m_EncodeFuncArray[PROTOCLIZS_MSGID_RANGE];
	DeFuncCProtoCliZS m_DecodeFuncArray[PROTOCLIZS_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
