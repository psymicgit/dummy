

//  Defines the entry point for the console application.
//  时间：2010-12-27。
//

#ifndef _DB_APPLICATION_H
#define _DB_APPLICATION_H

#include <sdtype.h>
#include <sdframework.h>
#include <sdmutex.h>
#include <sdobjectpool.h>

#include "db/dbmgr.h"
#include "logic/dbconfig.h"
#include "net/gt/gtprocessor.h"
#include "net/tlc/tlcprocessor.h"
#include "logic/lsevent.h"
#include "protocol/server/protogtls.h"


typedef struct _tagPKT_GTLS_LOGIN_ACK
{
    UINT64				qwTime;
    PKT_GTLS_LOGIN_ACK	stAck;

    _tagPKT_GTLS_LOGIN_ACK()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(_tagPKT_GTLS_LOGIN_ACK));
    }
} CPKT_GTLS_LOGIN_ACK, *pPKT_GTLS_LOGIN_ACK;


typedef std::map<string, CPKT_GTLS_LOGIN_ACK*>	CPKT_GTLS_LOGIN_ACK_MAP;
typedef CPKT_GTLS_LOGIN_ACK_MAP::iterator		CPKT_GTLS_LOGIN_ACK_MAP_Itr;




class CLSApplication : public CSDApplication
{
public:
    CLSApplication();
    ~CLSApplication();
    virtual BOOL        Init(const TCHAR* pszConfigFilename);
    virtual VOID        UnInit();
    virtual BOOL        Run();
    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID) ;
    virtual VOID        OnPipeChannelDisconnected(CSDPipeChannel* poChannel) ;
    virtual VOID        OnHandleUserCmd(const TCHAR* szCmd);
    virtual VOID			UpdateSvrStatus(UINT32 dwID, UINT32 dwIP, BOOL bConnect);

    VOID*				GetNetBufMemory();
    VOID					FreeSQL(VOID* p);
    VOID					FreeNetBuf(VOID* p);
    VOID*				GetSQLMemory();

    CSDPipeChannel*		GetGTServer()  { return m_poGTPipeChannel; };
	CSDPipeChannel*		GetTLCServer()  { return m_poTLCPipeChannel; };

    CDBMgr*				GetDBMgr();
    CDBConfig*		    GetDBConfig();
    VOID					OnEvent();
    VOID					AddLoginAck(const string key, const PKT_GTLS_LOGIN_ACK	&stAck);
    BOOL					GetLoginAck(const string key, PKT_GTLS_LOGIN_ACK &stAck);
	BOOL					CkNetData();
	BOOL					AddData( const string &strInfo);
	VOID					SaveData();


private:
    CDBMgr						m_oDBMgr;
    ISDListener*					m_pListener;
    CDBConfig					m_oDBConfig;

    CSDFixMemoryPool*				m_pSQLMemoryPool;
    CSDFixMemoryPool*				m_pNetBufMemoryPool;
    CSDMutex						m_oNetBufLock;
    CSDMutex						m_oSQLLock;

	CSDPipeChannel*				m_poGTPipeChannel;
    CSDPipeChannel*				m_poTLCPipeChannel;
    CGTProcessor				m_oGTProcessor;
	CTLCProcessor				m_oLTCProcessor;
    CEventReactor<CLSEvent>		m_oReactor;

    CSDMutex										m_oMutex;
    CPKT_GTLS_LOGIN_ACK_MAP						m_mapPKT_GTLS_LOGIN_ACK;
    CSDObjectPool<CPKT_GTLS_LOGIN_ACK, CSDMutex>		m_oPKT_GTLS_LOGIN_ACKPool;

    UINT32						ms_nMaxDataNum; //最大数据缓存数量
    deque<string>					ms_deqData;
    CSDMutex						ms_deqDataMutex;
};

#endif

