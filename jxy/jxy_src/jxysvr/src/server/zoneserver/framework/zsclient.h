
#ifndef __GATEESERVER_CLIENT_CHANNEL_H
#define __GATEESERVER_CLIENT_CHANNEL_H

#include <sdframework.h>
#include <sdhashmap.h>
#include <sdtype.h>
#include "net/cli/cliprocessor.h"
#include "mgencrypt.h"
/**
* @brief 控制Client消息传输状态的枚举
*/
enum EClientState
{
    CLIENT_STATE_UNAUTHED,
    CLIENT_STATE_AUTHING,
    CLIENT_STATE_AUTHED,
    CLIENT_STATE_ENTERING_GAME,
    CLIENT_STATE_IN_GS,
    CLIENT_STATE_LEAVEING_GAME,
};

/**
* @brief 控制Client消息数据转发方向的枚举
*/
enum ETransferDirection
{
    CLIENT_DIRECTION_NONE,
    CLIENT_DIRECTION_LS,
    CLIENT_DIRECTION_GS,
};


class CZSClient: public ISDSession
{
public:
    CZSClient();
    virtual ~CZSClient();
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

    BOOL SDAPI  SendMsg(UINT16 wMsgID , const CHAR* pData);


    VOID SDAPI  SetPacketProcessor(CSDPacketProcessor*    pPacketProcessor);

    MGEncrypt&	GetMGEncrypt()
    {
        return m_oMGEncrypt;
    }

    UINT64	GetLastTime()
    {
        return m_qwLastTime;
    }

    const string GetRemoteIPStr()
    {
        string v_strip;
        if (NULL != m_poConnection)
        {
            v_strip =  m_poConnection->GetRemoteIPStr();
        }
        return v_strip;
    }



private:
    ISDConnection*				m_poConnection;		/** 与client之间的连接 */
    CSDPacketProcessor*			m_poPacketProcessor;
    UINT32						m_dwCliSessionID;		/** Gate Server给client分配的事务ID */
    MGEncrypt					m_oMGEncrypt;//用于加解密
    UINT64						m_qwLastTime;//上次收到包的时间

};


class CZSClientMgr: public ISDSessionFactory
{
public:
    typedef HashMap<UINT32, CZSClient*>	    CZSClientMap;
    typedef CZSClientMap::iterator          CZSClientMapItr;
    CZSClientMgr();
    ~CZSClientMgr();
    virtual								ISDSession* SDAPI CreateSession(ISDConnection* poConnection);
public:
    BOOL								Init(UINT32 nMaxClient = 1000);
    VOID								UnInit();
    VOID								ReleaseCliSession(CZSClient* pSession);
    VOID								AddCliSession(CZSClient* poSession);
    CZSClient*						    FindCliSession(UINT32 dwClientID);
    UINT32								GenerateClientID();
    UINT32								GetCliSessionCount();
    VOID								ChkSessionOnTimer();

private:
    CSDObjectPool<CZSClient, CSDMutex>*	m_poCliSessionPool;
    CZSClientMap			            m_mapCliSession;
    CSDMutex*							m_poLock;
    UINT32								m_dwCurClientID;
    CCliProcessor                       m_oCliProcessor;
};


#endif

