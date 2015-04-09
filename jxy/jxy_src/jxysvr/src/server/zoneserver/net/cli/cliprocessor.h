#ifndef __SDF_CLI_HANDLER_INCLUDE_H_
#define __SDF_CLI_HANDLER_INCLUDE_H_
#include "sdframework.h"

class CCliProcessor :public CSDPacketProcessor
{
public:
	CCliProcessor();
public:
	~CCliProcessor();
    virtual BOOL Init();
    virtual CSDProtocol* GetProtocol();
private:
	static BOOL OnGetZoneInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnSelectZoneInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnGetVersionNoticReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnGetVersionNoticReq2(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnGetUserInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnRegisterUserNameReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnSelectZoneInfoExReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnReportUserDeviceInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnSelectZoneInfoEx2Req(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnUnBindDeviceReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL OnResetPasswdMobileReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnResetPasswdVerCodeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnResetPasswdNewPasswdReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
};

#endif

