#ifndef __SDF_CLI_HANDLER_INCLUDE_H_
#define __SDF_CLI_HANDLER_INCLUDE_H_
#include "sdframework.h"
#include <json/json.h>


class CCliProcessor
{
public:
	CCliProcessor();
public:
	~CCliProcessor();
    virtual BOOL Init();
	BOOL ProcessPacket(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	
	BOOL GetOauthStringReq(VOID* poReceiver, Json::Value & json_object);
	BOOL OauthUserInfoReq(VOID* poReceiver, Json::Value & json_object);
	BOOL NoticeInfoAllReq(VOID* poReceiver, Json::Value & json_object);
};

#endif

