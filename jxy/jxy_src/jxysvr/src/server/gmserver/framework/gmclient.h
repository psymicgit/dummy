
#ifndef __GATEESERVER_CLIENT_CHANNEL_H
#define __GATEESERVER_CLIENT_CHANNEL_H

#include <sdframework.h>
#include <sdhashmap.h>
#include <sdtype.h>
#include "net/cli/cliprocessor.h"

#define MAX_BUFFER_LEN 65535

/**
* @brief 控制Client消息传输状态的枚举
*/
enum EClientState
{
    CLIENT_STATE_UNAUTHED,	
	CLIENT_STATE_AUTHEDING,
    CLIENT_STATE_AUTHED,
};


class CGMClient: public ISDSession
{
public:
    CGMClient();
    virtual ~CGMClient();
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

	BOOL SDAPI  SendMsg(const CHAR* pData);


    VOID SDAPI  SetPacketProcessor(CCliProcessor*    pPacketProcessor);

	UINT64	GetLastTime() { return m_qwLastTime; } 

	UINT8 & GetLoginFlag(){return m_byLoginFlag;}

	char	 * GetRandStr();

	BOOL Oauth(char *szUserName, char *ncrypt);

	void SetLoginFlag(UINT8 byLoginFlag){m_byLoginFlag = byLoginFlag;}

private:
    ISDConnection*				m_poConnection;		/** 与client之间的连接 */
    CCliProcessor*				m_poPacketProcessor;
    UINT32						m_dwCliSessionID;		/** Gate Server给client分配的事务ID */    
	UINT64						m_qwLastTime;			//上次收到包的时间
	UINT8						m_byLoginFlag;
	char							m_szRandStr[64];
};


class CGMClientMgr: public ISDSessionFactory
{
public:
    typedef HashMap<UINT32, CGMClient*>	     CGMClientMap;
    typedef CGMClientMap::iterator          CGMClientMapItr;

	typedef HashMap<UINT32, UINT32>				CSerialNumberMap;
	typedef CSerialNumberMap::iterator			CSerialNumberMapItr;
		
    CGMClientMgr();
    ~CGMClientMgr();
    virtual							ISDSession* SDAPI CreateSession(ISDConnection* poConnection);
public:
    BOOL								Init(UINT32 nMaxClient = 1000);
    VOID								UnInit();
    VOID								ReleaseCliSession(CGMClient* pSession);
    VOID								AddCliSession(CGMClient* poSession);
    CGMClient*						FindCliSession(UINT32 dwClientID);
    UINT32							GenerateClientID();
    UINT32							GetCliSessionCount();
	VOID								ChkSessionOnTimer();

	UINT32							SetCliSerialNumber(CGMClient* poSession);
	CGMClient	*					GetCliSerialNumber(UINT32 dwSerialNumber);
	void								EarseCliSerialNumber(CGMClient* poSession);
private:
    CSDObjectPool<CGMClient,CSDMutex>*	m_poCliSessionPool;
    CGMClientMap			            m_mapCliSession;
    CSDMutex*						m_poLock;
    UINT32							m_dwCurClientID;
	CCliProcessor                     m_oCliProcessor;

	CSerialNumberMap					m_mapCliSerialNumber;
	UINT32							m_dwSerialNumber;
	CSDMutex							m_oSerialNumberLock;
};


#endif

