#ifndef _RECHAGE_PROCESSOR_INCLUDE_H_
#define _RECHAGE_PROCESSOR_INCLUDE_H_
#include "sdframework.h"

class CRechargeProcessor :public CSDPacketProcessor
{
public:
	CRechargeProcessor();
public:
	~CRechargeProcessor();
    virtual BOOL Init();
    virtual CSDProtocol* GetProtocol();
private:
	static BOOL OnRechargeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnHeartReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	static BOOL OnRecvOrderIDAckFromExchange( VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen );
	static BOOL OnExchangeResultReq( VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen );
};

#endif

