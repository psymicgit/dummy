#ifndef __SDF_LOGIN_SERVER_HANDLER_INCLUDE_H_
#define __SDF_LOGIN_SERVER_HANDLER_INCLUDE_H_

#include "sdframework.h"


class CLSProcessor :public CSDPacketProcessor
{
public:
	CLSProcessor();
public:
	~CLSProcessor();
	virtual BOOL  Init();
	virtual CSDProtocol* GetProtocol();
protected:
	static BOOL         OnAuthReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
};

#endif

