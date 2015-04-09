
#ifndef DEFAPNS_H
#define DEFAPNS_H

#include <string>
#include "common/server/utility.h"
#include <json/json.h>


using namespace std;

#define APNS_DEV
/* Development Connection Infos */
#if defined(APNS_DEV)
#define APPLE_HOST          	"gateway.sandbox.push.apple.com"
#define APPLE_PORT          	2195
#define APPLE_FEEDBACK_HOST 	"feedback.sandbox.push.apple.com"
#define APPLE_FEEDBACK_PORT 	2196
#else
#define APPLE_HOST          	"gateway.push.apple.com"
#define APPLE_PORT          	2195
#define APPLE_FEEDBACK_HOST 	"feedback.push.apple.com"
#define APPLE_FEEDBACK_PORT 	2196
#endif


#define PUSHDATA_PRO		0x00
#define PUSHDATA_PRO_EX	0x01




#pragma pack(push, 1)
typedef struct _tagPUSHDATA
{
    UINT8	byCommend;
    UINT16	wTokenLen;
    char		szToken[32];
    UINT16	wPayLoadLen;
    char		szPayload[256];
    _tagPUSHDATA()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(_tagPUSHDATA));
        byCommend = PUSHDATA_PRO;
    }

    void SetToken(const char *pToken)
    {
        byCommend = PUSHDATA_PRO;
        wTokenLen = SDHtons(32);
        memcpy(szToken, pToken, 32);
    }

    UINT16 SetPayLoad(const char *pMsg, const char *pSound, const UINT32 dwBadge, UINT32 dwPlayerID, UINT8 byZoneID, UINT8	byCareerID, UINT8 byAuthType, UINT8 byAction)
    {
        //sprintf(szPayload, "{\"aps\":{\"alert\":\"%s\"}}", pMsg);
        //wPayLoadLen = strlen(szPayload);
        //wPayLoadLen = SDHtons(wPayLoadLen);
        //return wPayLoadLen + sizeof(wPayLoadLen) + sizeof(byCommend) + sizeof(wTokenLen) + sizeof(szToken);

		Json::Value json_aps;      //临时对象，供如下代码使用
		json_aps["alert"] = Json::Value(pMsg);
		json_aps["sound"] = Json::Value(pSound);
		json_aps["badge"] = Json::Value(dwBadge);

		Json::Value json_root;    

		json_root["PlayerID"] = Json::Value(dwPlayerID);
		json_root["ZoneID"] = Json::Value(byZoneID);
		json_root["CareerID"] = Json::Value(byCareerID);
		json_root["AuthType"] = Json::Value(byAuthType);
		json_root["Action"] = Json::Value(byAction);
		json_root["aps"] = json_aps;

		string strjson = json_root.toStyledString();
		memset(szPayload, 0x00, sizeof(szPayload));
		memcpy(szPayload, strjson.c_str(), strjson.size());
		wPayLoadLen = SDHtons(strjson.size());
		return sizeof(byCommend) + sizeof(wTokenLen) + sizeof(szToken) + sizeof(wPayLoadLen) + strjson.size();
    }
} CPUSHDATA, *pPUSHDATA;


typedef struct _tagPUSHDATAEX
{
    UINT8	byCommend;
    UINT32	dwIdentifier;	//标识符
    UINT32	dwExpiry;	//过期值
    UINT16	wTokenLen;
    char		szToken[32];
    UINT16	wPayLoadLen;
    char		szPayload[256];
    _tagPUSHDATAEX()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(_tagPUSHDATAEX));
        byCommend = PUSHDATA_PRO_EX;
    }

    void SetIdentifier(UINT32	_dwIdentifier)
    {
        dwIdentifier = SDHtonl(_dwIdentifier);
    }

    void SetExpiry(UINT32	_dwExpiry)
    {
        dwExpiry = SDHtonl(_dwExpiry);
    }

    void SetToken(const char *pToken)
    {
        wTokenLen = SDHtons(32);
        memcpy(szToken, pToken, 32);
    }

    UINT16 SetPayLoad(const char *pMsg, const char *pSound, const UINT32 dwBadge, UINT32 dwPlayerID, UINT8 byZoneID, UINT8	byCareerID, UINT8 byAuthType, UINT8 byAction)
    {
		Json::Value json_aps;      //临时对象，供如下代码使用
		json_aps["alert"] = Json::Value(pMsg);
		json_aps["sound"] = Json::Value(pSound);
		json_aps["badge"] = Json::Value(dwBadge);

		Json::Value json_root;    

		json_root["PlayerID"] = Json::Value(dwPlayerID);
		json_root["ZoneID"] = Json::Value(byZoneID);
		json_root["CareerID"] = Json::Value(byCareerID);
		json_root["AuthType"] = Json::Value(byAuthType);
		json_root["Action"] = Json::Value(byAction);
		json_root["aps"] = json_aps;
		string strjson = json_root.toStyledString();

		memset(szPayload, 0x00, sizeof(szPayload));
		memcpy(szPayload, strjson.c_str(), strjson.size());
        wPayLoadLen = SDHtons(strjson.size());
        return sizeof(byCommend) + sizeof(dwIdentifier) + sizeof(dwExpiry) + sizeof(wTokenLen) + sizeof(szToken) + sizeof(wPayLoadLen) + strjson.size();
    }
} CPUSHDATAEX, *pPUSHDATAEX;



typedef struct _tagPUSHDATAEXRESPONCE
{
    UINT8	byCommend;
    UINT8	byStatus;
    UINT32	dwIdentifier;
    _tagPUSHDATAEXRESPONCE()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(_tagPUSHDATAEXRESPONCE));
    }
    UINT32 GetIdentifier()
    {
        return SDHtonl(dwIdentifier);
    }
} CPUSHDATAEXRESPONCE, *pPUSHDATAEXRESPONCE;


typedef struct _tagResponce
{
    UINT32	dwTime;
    UINT16	wTokenLen;
    char		szToken[32];
    _tagResponce()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(_tagResponce));
    }
    string GetTime()
    {
        return Time_ToString(SDHtonl(dwTime));
    }
    UINT16 GetTokenLen()
    {
        return SDHtons(wTokenLen);
    }

    string GetToken()
    {
        char szTemp[256] = {0};
        AsciiToHex((unsigned char*)szTemp, (unsigned char*)szToken, 32);
        string strTemp = szTemp;
        return strTemp;
    }

} CResponce, *pResponce;


#pragma pack(pop)

namespace ERR_RESPONCE
{
static const unsigned int ID_SUCCESS = 0;
static const char* STR_SUCCESS = "正确";

static const unsigned int ID_PROCESS_FAILED = 1;
static const char* STR_PROCESS_FAILED = "处理过程错误";

static const unsigned int ID_LACK_TOKEN = 2;
static const char* STR_LACK_TOKEN = "缺少设备令牌";

static const unsigned int ID_LACK_THEME = 3;
static const char* STR_LACK_THEME = "缺少主题";

static const unsigned int ID_LACK_MESSAGE = 4;
static const char* STR_LACK_MESSAGE = "缺少有效消息休";

static const unsigned int ID_TOKEN_LEN = 5;
static const char* STR_TOKEN_LEN = "令牌长度错误";

static const unsigned int ID_SIZE = 6;
static const char* STR_SIZE = "大小错误";

static const unsigned int ID_MESSAGE_BODY_LEN = 7;
static const char* STR_MESSAGE_BODY_LEN = "消息体长度错误";

static const unsigned int ID_TOKEN_ERROR = 8;
static const char* STR_TOKEN_ERROR = "令牌错误";

static const unsigned int ID_UNKNOWN_ERROR = 255;
static const char* STR_UNKNOWN_ERROR = "未知错误";

static const char* GetErrorInfo(const unsigned int _dwError)
{
    switch (_dwError)
    {
    case ID_SUCCESS:
        return STR_SUCCESS;
        break;
    case ID_PROCESS_FAILED:
        return STR_PROCESS_FAILED;
        break;
    case ID_LACK_TOKEN:
        return STR_LACK_TOKEN;
        break;
    case ID_LACK_THEME:
        return STR_LACK_THEME;
        break;
    case ID_LACK_MESSAGE:
        return STR_LACK_MESSAGE;
        break;
    case ID_TOKEN_LEN:
        return STR_TOKEN_LEN;
        break;
    case ID_SIZE:
        return STR_SIZE;
        break;
    case ID_MESSAGE_BODY_LEN:
        return STR_MESSAGE_BODY_LEN;
        break;
    case ID_TOKEN_ERROR:
        return STR_TOKEN_ERROR;
        break;
    case ID_UNKNOWN_ERROR:
        return STR_UNKNOWN_ERROR;
        break;
    default:
        return STR_UNKNOWN_ERROR;
    }
}
}

template <class MT = CSDMutex>
class CSDLockEx
{
public:
    CSDLockEx(MT & mt): m_mutex(mt), m_bOwner(FALSE)
    {
        acquire();
    }

    void acquire()
    {
        if (FALSE == m_bOwner)
        {
            m_mutex.Lock();
            m_bOwner = TRUE;
            //SYS_CRITICAL(_SDT("[%s:%d] 获得锁 \n"), MSG_MARK);
        }
    }
    void release()
    {
        if (m_bOwner)
        {
            m_mutex.Unlock();
            m_bOwner = FALSE;
            //SYS_CRITICAL(_SDT("[%s:%d] 释放锁 \n"), MSG_MARK);
        }
    }

    ~CSDLockEx()
    {
        release();
    }
private:
    MT & m_mutex;
    BOOL m_bOwner;
};



#endif


