
#include "rechargeclient.h"
#include "rechargeprocessor.h"
#include "rechargepktbuilder.h"
#include <db/autosqlbuf.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/server/packetparser.h>
#include <sdnet.h>
#include <sdapplication.h>
#include <framework/gsconfig.h>
#include <framework/gsapi.h>

CRechargeClient::CRechargeClient():
            m_dwCliSessionID(0),
            m_poConnection(NULL)
{
    m_qwLastTime = SDTimeSecs();
    m_poPacketProcessor = NULL;
	m_bConnect=FALSE;
	m_bConnecting=FALSE;

}

CRechargeClient::~CRechargeClient()
{

}


VOID CRechargeClient::SetConnection(ISDConnection* poConnection)
{
    m_poConnection = poConnection;
}

bool CRechargeClient::OnError(INT32 nSDError, INT32 nSysError)
{
	//DBG_INFO(_SDT("[%s:%d]: OnError: SDError: %d, SysError: %d\n"), MSG_MARK, nSDError, nSysError);

	if( m_bConnecting )
	{
		m_bConnecting=FALSE;
	}

	if( NULL==m_poConnection )
	{
		return true;
	}
    DBG_INFO(_SDT("[%s:%d]: Connection error: IP = %s, Port = %d, SDError: %d, SysError: %d\n"), 
        MSG_MARK, m_poConnection->GetRemoteIPStr(), m_poConnection->GetRemotePort(), nSDError, nSysError);

    return true;
}

VOID CRechargeClient::OnEstablish()
{	
	m_bConnect=TRUE;
	m_bConnecting=FALSE;
	DBG_INFO(_SDT("[%s:%d]: OnEstablish \n"), MSG_MARK);
}


VOID CRechargeClient::OnRecv(const CHAR* pBuf, UINT32 nLen)
{
    SDNetMsgHeader* pNetHeader = (SDNetMsgHeader*)pBuf;
    UINT16 wMsgID = SDNtohs(pNetHeader->wMsgID);
    UINT32 dwDataLen = SDNtohl(pNetHeader->dwDataLen);
    CHAR* pData = (CHAR*)pBuf + sizeof(SDNetMsgHeader);
	m_qwLastTime = SDTimeSecs();

    CSDPipeChannel* poSDPipeChannel = NULL;
    BOOL bProccess = TRUE;

    if(m_poPacketProcessor && m_poPacketProcessor->LookupProcessFunc(wMsgID))
    {
		static CHAR  szDecodeData[PACKET_LENGTH] = {0};
		CSDProtocol* poProtocol = m_poPacketProcessor->GetProtocol();
		if (poProtocol)
		{
			INT32 nDecodeLen = poProtocol->Decode( wMsgID,
				pData, 
				dwDataLen,
				szDecodeData,
				PACKET_LENGTH);
			if ( nDecodeLen > 0)
			{
				SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pBuf;
				pPipeHeader->dwTransID = m_dwCliSessionID;
				pPipeHeader->wMsgID    = wMsgID;
				bProccess = m_poPacketProcessor->ProcessPacket(this,
					pPipeHeader, 
					szDecodeData,
					nDecodeLen);
			}
		}
    }

}

BOOL CRechargeClient::IsConnect()
{
	//DBG_INFO(_SDT("[%s:%d]: IsConnect \n"), MSG_MARK);
	return m_bConnect;
}

VOID CRechargeClient::OnTerminate()
{
	m_bConnect=FALSE;
	USR_INFO( "disconnect from exchange server" );
}

VOID CRechargeClient::Release()
{
	DBG_INFO(_SDT("[%s:%d]: Release \n"), MSG_MARK);
    //((CGSApplication*)SDGetApp())->GetRechargeClientMgr()->ReleaseCliSession(this);
}

ISDConnection* CRechargeClient::GetConnection()
{
	DBG_INFO(_SDT("[%s:%d]: GetConnection \n"), MSG_MARK);
    return m_poConnection;
}



VOID CRechargeClient::SetCliSessionID(UINT32 dwCliSessionID)
{
    m_dwCliSessionID = dwCliSessionID;
}

UINT32 CRechargeClient::GetCliSessionID()
{
    return m_dwCliSessionID;
}


VOID CRechargeClient::Disconnect()
{
    if (m_poConnection)
    {
        
        m_poConnection->Disconnect();
		OnTerminate();
    }
	DBG_INFO(_SDT("[%s:%d]: Disconnect \n"), MSG_MARK);
}

VOID CRechargeClient::SendHeart()
{
	UINT16 wMsg=CRechargePktBuilder::Instance()->SendHeart();
	SendMsg( wMsg, CRechargePktBuilder::Instance()->GetPacketBuffer() );
}

BOOL CRechargeClient::Send(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen)
{
	if (m_poConnection)
	{
		static CHAR acNetBuf[PACKET_LENGTH];
		memset( acNetBuf, 0, sizeof(acNetBuf) );
		UINT32 dwOffset = sizeof(SDNetMsgHeader);
		if(nLen > PACKET_LENGTH)
		{
			return FALSE;
		}
		else
		{
			memcpy(acNetBuf + dwOffset, pBuf, nLen);				
		}

		SDNetMsgHeader* pHeader = (SDNetMsgHeader*)acNetBuf;
		INT32 nPackageLen = SDBuildNetMsgHeader(pHeader, wMsgID, nLen);
        m_poConnection->Send(acNetBuf,nPackageLen);

        return TRUE;
		
    }

    return FALSE;
}

BOOL CRechargeClient::SendMsg(UINT16 wMsgID ,const CHAR* pData) 
{
    if (NULL == m_poPacketProcessor)
    {
        return FALSE;
    }

    CSDProtocol* pProtocol = m_poPacketProcessor->GetProtocol();
    if ( NULL == pProtocol )
    {
        return FALSE;
    }
	static CHAR acMsgData[PACKET_LENGTH];
	memset( acMsgData, 0 ,sizeof(acMsgData) );
	INT32 nEncodeRet = pProtocol->Encode(wMsgID, (VOID *)pData, acMsgData, PACKET_LENGTH);
	if (nEncodeRet < 0)
	{
		return FALSE;
	}

	return Send(wMsgID, acMsgData, nEncodeRet);
}

VOID CRechargeClient::SetPacketProcessor(CSDPacketProcessor*    pPacketProcessor) 
{
    m_poPacketProcessor = pPacketProcessor;
}

CRechargeClientMgr::CRechargeClientMgr()
{
    m_poLock = NULL;
	m_dwCurClientID = 0;
	m_pClientSesstion = NULL;
	m_poRechargeConnector = NULL;
}

CRechargeClientMgr::~CRechargeClientMgr()
{
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
}

BOOL CRechargeClientMgr::Init(UINT32 dwMaxClient)
{
    m_poLock = SDNew CSDMutex();
    if(NULL == m_poLock)
    {
        //SYS_CRITICAL("[%s: %d]: Create ISDLock failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }	
    m_poCliSessionPool = SDNew CSDObjectPool<CRechargeClient,CSDMutex>(dwMaxClient, dwMaxClient);
    if (NULL == m_poCliSessionPool)
    {
        //SYS_CRITICAL("[%s: %d]: CliSession pool init failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }

	//////////////
#if (defined(WIN32) || defined(WIN64))
	m_poRechargeConnector = gsapi::GetNetModule()->CreateConnector(NETIO_COMPLETIONPORT);
#else
	m_poRechargeConnector = gsapi::GetNetModule()->CreateConnector(NETIO_EPOLL);
#endif

	if(NULL == m_poRechargeConnector)
	{
		return FALSE;
	}

	m_pClientSesstion = (CRechargeClient*)(CreateSession( NULL ));
	if(NULL == m_pClientSesstion)
	{
		m_poRechargeConnector->Release();
		return FALSE;
	}
	UINT32 dwClientID = GenerateClientID();
	m_pClientSesstion->SetCliSessionID(dwClientID);
	m_pClientSesstion->SetPacketProcessor(&m_oCliProcessor);
	m_pClientSesstion->SetConnection( NULL );

	m_poRechargeConnector->SetSession(m_pClientSesstion);
	m_poRechargeConnector->SetPacketParser( &gsapi::GetPacketParser() );
	m_poRechargeConnector->SetBufferSize(65535, 65535);
    return TRUE;
}

VOID CRechargeClientMgr::UnInit()
{
	if( NULL != m_poRechargeConnector )
	{
		m_poRechargeConnector->Release();
	}

    if (NULL != m_poCliSessionPool)
    {
        SDDelete m_poCliSessionPool;
        m_poCliSessionPool = NULL;
    }
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
	
}

ISDSession* CRechargeClientMgr::CreateSession(ISDConnection* poConnection)
{
    CRechargeClient* poSession = m_poCliSessionPool->Alloc();
    if (NULL == poSession)
    {
        return NULL;
    }
    UINT32 dwClientID = GenerateClientID();
    poSession->SetCliSessionID(dwClientID);
    poSession->SetPacketProcessor(&m_oCliProcessor);
    return poSession;
}

VOID CRechargeClientMgr::ReleaseCliSession(CRechargeClient* pSession)
{
    m_poCliSessionPool->Free(pSession);
}

UINT32 CRechargeClientMgr::GenerateClientID()
{
    m_poLock->Lock();
    if (m_dwCurClientID == 0xFFFFFFFF)
    {
        m_dwCurClientID = 0;
    }
    ++m_dwCurClientID;
    m_poLock->Unlock();

    return m_dwCurClientID;
}

VOID CRechargeClientMgr::SendHeart()
{
	if( !m_pClientSesstion )
	{
		return;
	}

	if( !m_pClientSesstion->IsConnect() )
	{
		return;
	}

	m_pClientSesstion->SendHeart();
}

CRechargeClient* CRechargeClientMgr::GetClient()
{
	return m_pClientSesstion;
}

VOID CRechargeClientMgr::ConnectToPayServer()
{
	if( NULL==m_poRechargeConnector )
	{
		return;
	}

	if( m_pClientSesstion->IsConnecting() )
	{
		return;
	}
	INT32 nCurTime=INT32( time(NULL) );
	if( nCurTime-m_nLastConnectTime<ms_nConnectInterval )
	{
		return;
	}

	m_nLastConnectTime = nCurTime;
	
	//DBG_INFO( "begin connect exchange server! \n" );

	m_poRechargeConnector->Connect( CGSConfig::Instance()->GetRechargeIP().c_str(), CGSConfig::Instance()->GetRechargePort() );
	m_pClientSesstion->SetConnecting( TRUE );
}

VOID CRechargeClientMgr::Run()
{
	if( !m_pClientSesstion->IsConnect() )
	{
		if( m_pClientSesstion->IsConnecting() )
		{
			return;
		}

		ConnectToPayServer();
	}
}