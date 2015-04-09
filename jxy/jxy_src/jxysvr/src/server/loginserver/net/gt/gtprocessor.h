#ifndef __SGDP_GAME_SERVER_PROCESSOR_INCLUDE_H_
#define __SGDP_GAME_SERVER_PROCESSOR_INCLUDE_H_
#include "sdframework.h"
#include "protogtls.h"

class CGTProcessor :public CSDPacketProcessor
{
public:
	CGTProcessor();
public:
	~CGTProcessor();
	virtual BOOL  Init() ;
	virtual CSDProtocol* GetProtocol() ;
		
private:	
	static BOOL	OnLoginReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnRegisterUserNameReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnPlayerUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnDspNameUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnBindReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnUnBindReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnLoginExReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnLogoutReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnActivityNotify(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnChangePwdReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnBindMobileReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnBindCheckVerCodeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnUnBindMobileReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnUnBindCheckVerCodeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL	OnGetUserExpandReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
};

#endif

