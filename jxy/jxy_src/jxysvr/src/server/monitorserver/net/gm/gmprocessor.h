#ifndef __SDF_MS_HANDLER_INCLUDE_H_
#define __SDF_MS_HANDLER_INCLUDE_H_
#include "sdframework.h"

class CGMProcessor :public CSDPacketProcessor
{
public:
	CGMProcessor();
public:
	~CGMProcessor();
    virtual BOOL Init();
    virtual CSDProtocol* GetProtocol();
private:
	static BOOL OnControlReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnGetServerInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
};

#endif

