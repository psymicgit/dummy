

//  Defines the entry point for the console application.
//  Ê±¼ä£º2010-12-27¡£
//

#ifndef _GT_APPLICATION_H
#define _GT_APPLICATION_H

#include <sdtype.h>
#include <sdframework.h>
#include "net/cli/cliprocessor.h"
#include "net/ls/lsprocessor.h"
#include "net/gs/gsprocessor.h"
#include "framework/gtclient.h"

#include "logic/gtconfig.h"
#include "logic/gtevent.h"
#include "common/server/packetparser.h"

enum EFixLineIdx
{
	EFLI_CONNECT_NUM = 4,
};

class CGTApplication : public CSDApplication
{
public:
	CGTApplication();
	~CGTApplication();
    virtual BOOL        Init(const TCHAR* pszConfigFilename);
    virtual VOID        UnInit();
    virtual BOOL        Run();
    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);
    virtual VOID			OnPipeChannelDisconnected(CSDPipeChannel* poChannel);
    virtual VOID			UpdateSvrStatus(UINT32 dwID,UINT32 dwIP, BOOL bConnect);
    CGTClientMgr*			GetClientMgr();

    //////////////////////////////
    CGTConfig*          GetGTConfig() { return &m_oGTConfig; }
    CCliPacketParser*   GetCliPacketParser() { return &m_oCliPacketParser; }
    CSDPipeChannel*     GetGSServer(){return m_poGSPipeChannel;};
    //CSDPipeChannel*     GetLSServer(){return m_poLSPipeChannel;};
	
	//////////////////////////////////////
	VOID IncConnectNum();
	VOID DecConnectNum();

protected:
    virtual VOID        OnHandleUserCmd(const TCHAR* szCmd);
private:    
    //CLSProcessor				m_oLSPacketProcessor;
    CGSProcessor				m_oGSPacketProcessor;
    CGTClientMgr				m_oGTClientMgr;
    ISDListener*				m_poListener;
    CCliPacketParser			m_oCliPacketParser;
    CGTConfig				m_oGTConfig;
    CEventReactor<CGTEvent>	m_oReactor;
    CSDPipeChannel*			m_poGSPipeChannel;
	CSDPipeChannel*			m_poGSPipeChannel_1;
	CSDPipeChannel*			m_poGSPipeChannel_2;
    //CSDPipeChannel*			m_poLSPipeChannel;
	volatile INT32			m_nConnectNum;
};


#endif

