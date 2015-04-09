#ifndef __GS_PACKETPROCESSOR_INCLUDE_H_
#define __GS_PACKETPROCESSOR_INCLUDE_H_

#include "sdframework.h"
#include <protocol/server/protogsgs.h>
#include <logic/player/playermgr.h>

using namespace SGDP;


class CGSProcessor :public CSDPacketProcessor
{
public:
	static CGSProcessor* Instance()
	{
		static CGSProcessor oProcessor;
		return &oProcessor;
	}
protected:
	CGSProcessor();
	~CGSProcessor();

public:
    virtual BOOL  Init() ;
    virtual CSDProtocol* GetProtocol() ;

public:
	BOOL	StartSyncPlayer(UINT64 qwServerStartTime);

protected:
	 static BOOL OnStartSyncPlayer(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	 static BOOL OnSyncPlayer(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	 static BOOL OnSyncPlayerRpt(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);	 
	 static BOOL OnSyncDspNameReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);	 
	 static BOOL OnSyncDspNameRpt(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);	 
	 static BOOL OnSyncBossBReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);	 
	 static BOOL OnSyncBossBRpt(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);	 

protected:
	 VOID SyncPlayer(BOOL bStart = FALSE);
	 static BOOL SyncBossB();
private:
	CID2PlayerMap		m_mapSyncPlayer;
	CID2PlayerMapItr	m_itrDeal;
};


#endif

