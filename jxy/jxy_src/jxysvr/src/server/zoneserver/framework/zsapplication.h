

//  Defines the entry point for the console application.
//  时间：2010-12-27。
//

#ifndef _GT_APPLICATION_H
#define _GT_APPLICATION_H

#include <sdtype.h>
#include <sdframework.h>
#include "net/cli/cliprocessor.h"
#include "framework/zsclient.h"
#include "logic/dbmgr.h"
#include "logic/zsconfig.h"
#include "logic/zsevent.h"
#include "common/server/packetparser.h"
#include "net/gm/rcclient.h"
#include "net/gm/httppacketparser.h"

#include <queue>
using namespace std;


class CZSApplication : public CSDApplication
{
public:
	CZSApplication();
	~CZSApplication();
    virtual BOOL			Init(const TCHAR* pszConfigFilename);
    virtual VOID			UnInit();
    virtual BOOL			Run();
    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);
    virtual VOID			OnPipeChannelDisconnected(CSDPipeChannel* poChannel);
    virtual VOID			UpdateSvrStatus(UINT32 dwID,UINT32 dwIP, BOOL bConnect);
    CZSClientMgr*			GetClientMgr();
	CRCClientMgr*			GetRCClientMgr();

    //////////////////////////////
    CZSConfig*			GetZSConfig() { return &m_oZSConfig; }
    CCliPacketParser*		GetCliPacketParser() { return &m_oCliPacketParser; }
	//CDBMgr*				GetDBMgr() { return &m_oDBMgr; }
	BOOL					AddData( const string &strInfo);

	VOID					SaveData();
protected:
	
    virtual VOID			OnHandleUserCmd(const TCHAR* szCmd);
private:    
    CZSClientMgr				m_oZSClientMgr;
    ISDListener*				m_poListener;
    CCliPacketParser			m_oCliPacketParser;

	CRCClientMgr				m_oRCClientMgr;
	ISDListener*				m_poGmListener;
	CHttpCliPacketParser		m_oRCPacketParser;



    CZSConfig				m_oZSConfig;
	//CDBMgr					m_oDBMgr;
	CEventReactor<CZSEvent>	m_oReactor;

	UINT32					ms_nMaxDataNum; //最大数据缓存数量
	deque<string>				ms_deqData;
	CSDMutex					ms_deqDataMutex;
};


#endif

