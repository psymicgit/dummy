
#ifndef __RC_CLIENT_CHANNEL_H
#define __RC_CLIENT_CHANNEL_H

#include <sdframework.h>
#include <sdhashmap.h>
#include <sdtype.h>
#include <sdnet.h>


using namespace SGDP;

class CPlayer;
class CRCClient: public ISDSession
{
public:
    CRCClient();
    virtual ~CRCClient();
public:
    virtual VOID SDAPI SetConnection(ISDConnection* poConnection);
    virtual bool SDAPI OnError(INT32 nSDError, INT32 nSysError);

    virtual VOID SDAPI OnEstablish();

    virtual VOID SDAPI OnTerminate();

    virtual VOID SDAPI OnRecv(const CHAR* pBuf, UINT32 nLen);

    virtual VOID SDAPI Release();

    ISDConnection* GetConnection();

    VOID SetCliSessionID(UINT32 dwCliSessionID);

    UINT32 GetCliSessionID();

    VOID Disconnect();

	BOOL SDAPI Send(const CHAR* pBuf, INT32 nLen);

    BOOL SDAPI Send(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen);

    BOOL SDAPI  SendMsg(UINT16 wMsgID ,const CHAR* pData);

    UINT64	GetLastTime() { return m_qwLastTime; } 
    VOID	SetTimeOutCloseFlag(BOOL bTimeOutClose = TRUE) { m_bTimeOutClose = bTimeOutClose; }
    BOOL	GetTimeOutCloseFlag() { return m_bTimeOutClose; }

    VOID SDAPI  SetPacketProcessor(CSDPacketProcessor*    pPacketProcessor);


	inline UINT32 GetRemoteIP() { if(m_poConnection) return m_poConnection->GetRemoteIP();  return 0; }
	
	VOID SendPayRet(string strOrderID, UINT16 wErrCode, CPlayer* poPlayer, UINT32 dwExchangeGold);	
	VOID Rspone(string strRet, string strOrderID="");

protected:
	VOID DealHttpGmReq(const CHAR* pBuf, UINT32 nLen);


private:
    ISDConnection*				m_poConnection;		/** 与client之间的连接 */
    CSDPacketProcessor*			m_poPacketProcessor;
    UINT32						m_dwCliSessionID;		/** Gate Server给client分配的事务ID */
    UINT64						m_qwLastTime;//上次收到包的时间
    BOOL						m_bTimeOutClose;//是否是长时间未收到包而关闭的
};

class CRCCliProcessor :public CSDPacketProcessor
{
public:
	CRCCliProcessor() {};
	~CRCCliProcessor() {};

public:
	static CRCCliProcessor* Instance()
	{
		static CRCCliProcessor oProcessor;
		return &oProcessor;
	}

public:
	virtual BOOL  Init() { return TRUE; }
	virtual CSDProtocol* GetProtocol() { return NULL; }
};

class CRCClientMgr: public ISDSessionFactory
{
public:
    typedef HashMap<UINT32, CRCClient*>	    CRCClientMap;
    typedef CRCClientMap::iterator          CRCClientMapItr;
    CRCClientMgr();
    ~CRCClientMgr();
    virtual								ISDSession* SDAPI CreateSession(ISDConnection* poConnection);
public:
    BOOL								Init(UINT32 nMaxClient = 1000);
    VOID								UnInit();
    VOID								ReleaseCliSession(CRCClient* pSession);
    VOID								AddCliSession(CRCClient* poSession);
    VOID								RemoveCliSession(CRCClient* poSession);
    CRCClient*						FindCliSession(UINT32 dwClientID);
    UINT32							GenerateClientID();
    UINT32							GetCliSessionCount();
    VOID								ChkSessionOnTimer();
    VOID                            GSDisConnect(UINT32 dwServerID);
	inline UINT32						GetSessionNum() { return m_mapCliSession.size(); }

private:
    CSDObjectPool<CRCClient,CSDMutex>*	m_poCliSessionPool;
    CRCClientMap			            m_mapCliSession;
    CSDMutex*							m_poLock;
    UINT32								m_dwCurClientID;    
    CRCClientMapItr			            m_itrCurDeal;
    CRCCliProcessor                     m_oCliProcessor;
};


#endif

