#ifndef __TLC_PROCESSOR_INCLUDE_H_
#define __TLC_PROCESSOR_INCLUDE_H_

#include "sdframework.h"
#include "protolstlc.h"

class CTLCProcessor :public CSDPacketProcessor
{
public:
	CTLCProcessor();
public:
	~CTLCProcessor();
	virtual BOOL  Init() ;
	virtual CSDProtocol* GetProtocol() ;
		
private:	
	static BOOL	OnAuthAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

};

#endif

