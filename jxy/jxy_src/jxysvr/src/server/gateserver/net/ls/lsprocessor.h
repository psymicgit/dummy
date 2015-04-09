#ifndef __SDF_LOGIN_SERVER_HANDLER_INCLUDE_H_
#define __SDF_LOGIN_SERVER_HANDLER_INCLUDE_H_

#include "sdframework.h"
#include "protocol/server/protogtls.h"
#include "framework/gtclient.h"

class CLSProcessor :public CSDPacketProcessor
{
public:
	CLSProcessor();
public:
	~CLSProcessor();
	virtual BOOL  Init();
	virtual CSDProtocol* GetProtocol();
protected:
	static BOOL         OnLoginAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL         OnLoginExAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
};

#endif

