

#ifndef _NS_APPLICATION_H
#define _NS_APPLICATION_H

#include <sdtype.h>
#include <sdframework.h>
#include "net/gs/gsprocessor.h"
#include "net/ms/msprocessor.h"
#include "logic/gmconfig.h"
#include "logic/gmevent.h"
#include "common/server/jsonpacketparser.h"
#include "common/client/commondef.h"
#include "gmclient.h"
#include "logic/dbmgr.h"

#include <map>
using namespace std;

typedef map<UINT32, CSDPipeChannel*> CPipeChannelMap; //key为武将ID+神器品质
typedef CPipeChannelMap::iterator CPipeChannelMapItr;



class CGMApplication : public CSDApplication
{
public:
	CGMApplication();
	~CGMApplication();
    virtual BOOL			Init(const TCHAR* pszConfigFilename);
    virtual VOID			UnInit();
    virtual BOOL			Run();
    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);
    virtual VOID			OnPipeChannelDisconnected(CSDPipeChannel* poChannel);
    virtual VOID			UpdateSvrStatus(UINT32 dwID,UINT32 dwIP, BOOL bConnect);

    //////////////////////////////
    CGMConfig*			GetGmConfig() { return &m_oKSConfig; }
    CJsonPacketParser*	GetJsonPacketParser() { return &m_oJsonPacketParser; }
	CGMClientMgr		&	GetClientMgr(){return m_oGmClientMgr;}		
	CPipeChannelMap	&	GetPipeChannelMap(){return m_mapPipeChannel;}	

	//CDBMgr*				GetDBMgr() { return &m_oDBMgr; }
	BOOL					SendMsg2GameServer(UINT32 dwArea, UINT32 dwPlayerID, const CHAR* pBuf);
	BOOL					SendMsg2MonitorServer( UINT32 dwMsgID, UINT32 dwArea, const CHAR* pBuf);
protected:
    virtual VOID			OnHandleUserCmd(const TCHAR* szCmd);
	BOOL					DealUserCmd(const TCHAR* szCmd);
private:    

    CJsonPacketParser				m_oJsonPacketParser;
    CGMConfig					m_oKSConfig;
	CEventReactor<CGMEvent>		m_oReactor;
	CPipeChannelMap				m_mapPipeChannel;
	CGSProcessor					m_oGSProcessor;
	CMSProcessor					m_oMsProcessor;
	ISDListener*					m_poListener;
	CGMClientMgr					m_oGmClientMgr;
	//CDBMgr						m_oDBMgr;
};


#endif

