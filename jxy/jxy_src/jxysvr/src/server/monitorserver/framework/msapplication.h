

#ifndef _NS_APPLICATION_H
#define _NS_APPLICATION_H

#include <sdtype.h>
#include <sdframework.h>
#include "logic/msconfig.h"
#include "common/server/packetparser.h"
#include "logic/keepaliveevent.h"
#include "common/client/commondef.h"
#include "logic/monitor.h"
#include "net/gm/gmprocessor.h"

#include <map>
using namespace std;


class CMSApplication : public CSDApplication
{
public:
	CMSApplication();
	~CMSApplication();
    virtual BOOL			Init(const TCHAR* pszConfigFilename);
    virtual VOID			UnInit();
    virtual BOOL			Run();
    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);
    virtual VOID			OnPipeChannelDisconnected(CSDPipeChannel* poChannel);
    virtual VOID			UpdateSvrStatus(UINT32 dwID,UINT32 dwIP, BOOL bConnect);

    //////////////////////////////
    CKSConfig*			GetKSConfig() { return &m_oKSConfig; }

	BOOL					SendMsg2GMServer(UINT32 dwMsgID, const CHAR* pBuf);
protected:
    virtual VOID			OnHandleUserCmd(const TCHAR* szCmd);
	BOOL					DealUserCmd(const TCHAR* szCmd);
private:    
	CGMProcessor					m_oGmProcessor;
	CSDPipeChannel*				m_poGmPipeChannel;
    CKSConfig					m_oKSConfig;
	CEventReactor<CKeepAliveEvent>	m_oKeepAliveReactor;
};


#endif

