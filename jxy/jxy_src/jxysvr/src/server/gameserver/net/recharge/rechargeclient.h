
#ifndef __RECHARGE_CLIENT_H
#define __RECHARGE_CLIENT_H

#include <sdframework.h>
#include <sdtype.h>
#include <sdsingleton.h>
#include "rechargeprocessor.h"

using namespace SGDP;

class CRechargeClient: public ISDSession
{
public:
    CRechargeClient();
    virtual ~CRechargeClient();
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

    BOOL SDAPI Send(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen);

    BOOL SDAPI  SendMsg(UINT16 wMsgID ,const CHAR* pData);

    UINT64	GetLastTime() { return m_qwLastTime; } 

	VOID SendHeart();

    VOID SDAPI  SetPacketProcessor(CSDPacketProcessor*    pPacketProcessor);

	BOOL IsConnect();
	BOOL IsConnecting(){ return m_bConnecting; };
	VOID SetConnecting( BOOL bConnecting ){ m_bConnecting=bConnecting; };
private:
    ISDConnection*				m_poConnection;		/** 与client之间的连接 */
    CSDPacketProcessor*			m_poPacketProcessor;
	UINT32						m_dwCliSessionID;		/** Gate Server给client分配的事务ID */
    UINT64						m_qwLastTime;//上次收到包的时间
	BOOL						m_bConnect; //是否连接
	BOOL						m_bConnecting; //正在连接
	
};


class CRechargeClientMgr: public ISDSessionFactory
{
public:
    typedef HashMap<UINT32, CRechargeClient*> CRechargeClientMap;
    typedef CRechargeClientMap::iterator      CRechargeClientMapItr;
    CRechargeClientMgr();
    ~CRechargeClientMgr();
    virtual								ISDSession* SDAPI CreateSession(ISDConnection* poConnection);
public:

    BOOL								Init(UINT32 dwMaxClient = 1000);
    VOID								UnInit();
    VOID								ReleaseCliSession(CRechargeClient* pSession);
    UINT32								GenerateClientID();
    VOID								GSDisConnect(UINT32 dwServerID);
	VOID								SendHeart();
	CRechargeClient*					GetClient(); //因为只有一个客户端所以才直接获得
	VOID								Run(); //处理
private:
	VOID								ConnectToPayServer();
private:
	const static INT32	ms_nConnectInterval=10;//s
private:
    CSDObjectPool<CRechargeClient,CSDMutex>*		m_poCliSessionPool;
    CSDMutex*								m_poLock;
    UINT32									m_dwCurClientID;    
    CRechargeProcessor						m_oCliProcessor;

	ISDConnector*								m_poRechargeConnector;
	CRechargeClient*							m_pClientSesstion;
	INT32						m_nLastConnectTime;
};


#endif

