#ifndef __SDF_GS_HANDLER_INCLUDE_H_
#define __SDF_GS_HANDLER_INCLUDE_H_
#include "sdframework.h"

class CGSProcessor :public CSDPacketProcessor
{
public:
	CGSProcessor();
public:
	~CGSProcessor();
    virtual BOOL Init();
    virtual CSDProtocol* GetProtocol();
private:
	static BOOL OnPlayerDefaultNTF(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
};

#endif

