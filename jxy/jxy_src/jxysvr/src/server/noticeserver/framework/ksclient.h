
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


class CKSClient: public ISDSession
{
public:
    CKSClient();
    virtual ~CKSClient();
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

	UINT8 & GetLoginFlay(){return m_byLoginFlag;}

	char	 * GetRandStr();

	BOOL Oauth(char *szUserName, char *ncrypt);

	void SetLoginFlag(UINT8 byLoginFlag){m_byLoginFlag = byLoginFlag;}

private:
    ISDConnection*				m_poConnection;		/** 与client之间的连接 */
    CCliProcessor*				m_poPacketProcessor;
    UINT32						m_dwCliSessionID;		/** Gate Server给client分配的事务ID */    
	UINT64						m_qwLastTime;			//上次收到包的时间
	UINT8						m_byLoginFlag;
	char							m_szRandStr[56];
};





class CKSClientMgr: public ISDSessionFactory
{
public:
    typedef HashMap<UINT32, CKSClient*>	     CKSClientMap;
    typedef CKSClientMap::iterator          CKSClientMapItr;
    CKSClientMgr();
    ~CKSClientMgr();
    virtual								ISDSession* SDAPI CreateSession(ISDConnection* poConnection);
public:
    BOOL								Init(UINT32 nMaxClient = 1000);
    VOID								UnInit();
    VOID								ReleaseCliSession(CKSClient* pSession);
    VOID								AddCliSession(CKSClient* poSession);
    CKSClient*						FindCliSession(UINT32 dwClientID);
    UINT32							GenerateClientID();
    UINT32							GetCliSessionCount();
	VOID								ChkSessionOnTimer();

	UINT32							SetCliSerialNumber(CKSClient* poSession);
	CKSClient	*					GetCliSerialNumber(UINT32 dwSerialNumber);
	void								EarseCliSerialNumber(CKSClient* poSession);
private:
    CSDObjectPool<CKSClient,CSDMutex>*	m_poCliSessionPool;
    CKSClientMap			            m_mapCliSession;
    CSDMutex*						m_poLock;
    UINT32							m_dwCurClientID;
	CCliProcessor                     m_oCliProcessor;

	CKSClientMap						m_mapCliSerialNumber;
	UINT32							m_dwSerialNumber;


};


#endif

