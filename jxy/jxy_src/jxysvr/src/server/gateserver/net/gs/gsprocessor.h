#ifndef __SDF_GAME_SERVER_HANDLER_INCLUDE_H_
#define __SDF_GAME_SERVER_HANDLER_INCLUDE_H_

#include "sdframework.h"
#include "protocol/server/protogtgs.h"

using namespace std;

class CGSProcessor :public CSDPacketProcessor
{
public:
    CGSProcessor();
public:
    ~CGSProcessor();
    virtual BOOL  Init();
    virtual CSDProtocol* GetProtocol();
protected:
	//处理GSCT_ENTERGS_ACK消息
	static BOOL			OnEnterGSAck (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
    //处理GSGT_CLR_CACHE_RPT消息
    static BOOL			OnClrPlayerCache(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL			OnRegisterUserNameNtf(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL			OnRegisterUserNameAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL			OnPlayerUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL			OnDspNameUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	
	static BOOL			OnActivityNotify(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

	static BOOL			OnKickOutReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
	
};

#endif

