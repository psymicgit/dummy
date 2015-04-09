#ifndef __SDF_MS_HANDLER_INCLUDE_H_
#define __SDF_MS_HANDLER_INCLUDE_H_
#include "sdframework.h"

class CMSProcessor :public CSDPacketProcessor
{
public:
	CMSProcessor();
public:
	~CMSProcessor();
    virtual BOOL Init();
    virtual CSDProtocol* GetProtocol();
private:
	static BOOL OnControlAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnGetServerInfoAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
};

#endif

