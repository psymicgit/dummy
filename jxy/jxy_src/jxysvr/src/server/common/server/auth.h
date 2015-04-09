#ifndef _SDAUTH_H_
#define _SDAUTH_H_

#include <sdbase.h>

using namespace SGDP;


//认证错误码类型
enum EAuthErrCode
{
	EAEC_SUCCESS = 0,  //成功
	EAEC_INVALID_CHART = 1, //包含无效字符
	EAEC_NOTMATCH = 2, //用户名密码错误
	EAEC_TIMEOUT = 3, //认证超时
	EAEC_INVALID_AUTHTYPE = 4, //无效认证类型
	EAEC_INNERR = 5, //内部错误
};



class CAuthClient
{
public:
	virtual	VOID SDAPI	OnAuthSuccess()=0;	
	virtual	VOID SDAPI	OnAuthFailed(UINT16 wErrCode, TCHAR* pszErrCode)=0;
};


class CAuthClientMgr
{
public:
	virtual CAuthClient* SDAPI FindClient(UINT16 wZoneID, UINT32 dwTransID) = 0;
};

class CAuthModule
{
public:

	virtual BOOL SDAPI Init(const CHAR* pszModuleName, const CHAR* pszConfig, CAuthClientMgr* poAuthClientMgr) = 0;

	virtual BOOL SDAPI Auth(UINT16 wZoneID, UINT32 dwTransID, TCHAR* pszName, UINT8 byPwdLen, UINT8* pabyUserPwd, TCHAR* pszDeviceID, UINT32 dwParam1, UINT32 dwParam2, UINT32 dwParam3, UINT64 qwParam4, TCHAR* pszExt) = 0;

	virtual BOOL SDAPI Run(INT32 nCount = -1) = 0;

	virtual VOID SDAPI Release(VOID) = 0;
};

extern "C" CAuthModule* SDAPI GetAuthModule(void);

	
#endif 
