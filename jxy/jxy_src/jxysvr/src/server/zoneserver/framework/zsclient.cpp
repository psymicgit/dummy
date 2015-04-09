#include "zsclient.h"
#include "zsapplication.h"
#include "net/cli/cliprocessor.h"
#include <sdutil.h>
#include <common/server/utility.h>
#include <protocol/server/protocligt.h>
#include <net/cli/clipktbuilder.h>


CZSClient::CZSClient():
    m_dwCliSessionID(0),
    m_poConnection(NULL)
{
    m_poPacketProcessor = NULL;
    m_qwLastTime = SDTimeSecs();
}

CZSClient::~CZSClient()
{

}


VOID CZSClient::SetConnection(ISDConnection* poConnection)
{
    m_poConnection = poConnection;
}

bool CZSClient::OnError(INT32 nSDError, INT32 nSysError)
{
    if (NULL != m_poConnection )
    {
        USR_INFO(_SDT("[%s:%d]: Connection error: IP = %s, Port = %d, SDError: %d, SysError: %d\n"),
                 MSG_MARK, m_poConnection->GetRemoteIPStr(), m_poConnection->GetRemotePort(), nSDError, nSysError);
    }
    return true;
}

VOID CZSClient::OnEstablish()
{
    ((CZSApplication*)SDGetApp())->GetClientMgr()->AddCliSession(this);

    //判断是否需加解密
    //if(((CZSApplication*)SDGetApp())->GetZSConfig()->GetEncryptFlag()) //目前都加密
    {
        //发送加解密密钥
        UINT8 abyKeyInfo[ENDEC_KEY_LEN + 1] = {0};
        if(RandomNumbercode(abyKeyInfo, ENDEC_KEY_LEN))
        {
            UINT16 wMsgID = CCliPktBuilder::Instance()->EncryptDecryptKeyNtf(abyKeyInfo);
            SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
            m_oMGEncrypt.setKey(abyKeyInfo, ENDEC_KEY_LEN);
        }
    }
}

VOID CZSClient::OnRecv(const CHAR* pBuf, UINT32 nLen)
{
    SDNetMsgHeader* pNetHeader = (SDNetMsgHeader*)pBuf;
    UINT16 wMsgID = SDNtohs(pNetHeader->wMsgID);
    UINT32 dwDataLen = SDNtohl(pNetHeader->dwDataLen);
    CHAR* pData = (CHAR*)pBuf + sizeof(SDNetMsgHeader);
    CSDPipeChannel* poSDPipeChannel = NULL;
    CZSApplication* poGTApplication = (CZSApplication*)SDGetApp();
    BOOL bProccess = TRUE;
    m_qwLastTime = SDTimeSecs();

    //先解密
    if(m_oMGEncrypt.getKeyLength() > 0)
    {
        UINT8* pszEncryptBuff =  (UINT8*)(pBuf + sizeof(SDNetMsgHeader));
        INT32 nEncryptBuffLen = nLen - sizeof(SDNetMsgHeader);
        if(nEncryptBuffLen > 0)
        {
            if(!m_oMGEncrypt.decryptionDatas(pszEncryptBuff, nEncryptBuffLen))
            {
                return ;
            }
            dwDataLen = dwDataLen - ENCRYPT_HEAD_LEN - ENCRYPT_TAIL_LEN;
            pData = pData + ENCRYPT_HEAD_LEN;
            pBuf = pData - sizeof(SDNetMsgHeader);
        }
    }

    if(m_poPacketProcessor)
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

VOID CZSClient::OnTerminate()
{
    //((CZSApplication*)SDGetApp())->GetClientMgr()->ReleaseCliSession(this);
    //m_poConnection = NULL;
}

VOID CZSClient::Release()
{
    ((CZSApplication*)SDGetApp())->GetClientMgr()->ReleaseCliSession(this);
    m_poConnection = NULL;
}

ISDConnection* CZSClient::GetConnection()
{
    return m_poConnection;
}


VOID CZSClient::SetCliSessionID(UINT32 dwCliSessionID)
{
    m_dwCliSessionID = dwCliSessionID;
}

UINT32 CZSClient::GetCliSessionID()
{
    return m_dwCliSessionID;
}


VOID CZSClient::Disconnect()
{
    if (m_poConnection)
    {
        OnTerminate();
        m_poConnection->Disconnect();
    }
}

BOOL CZSClient::Send(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen)
{
    if (m_poConnection)
    {
        static CHAR acNetBuf[PACKET_LENGTH] = {0};
        UINT32 dwOffset = sizeof(SDNetMsgHeader);
        //先加密，若已经设置key
        if(m_oMGEncrypt.getKeyLength() > 0)
        {
            if(nLen > PACKET_LENGTH)
            {
                return FALSE;
            }
            else
            {
                memcpy(acNetBuf + dwOffset + ENCRYPT_HEAD_LEN, pBuf, nLen);
            }

            UINT8* pszDecryptBuff =  (UINT8*)(acNetBuf + dwOffset);
            INT32 nDecryptBuffLen = nLen + ENCRYPT_HEAD_LEN + ENCRYPT_TAIL_LEN;//添加加解密头尾

            if(nDecryptBuffLen > 0)
            {
                m_oMGEncrypt.encryptionDatas(pszDecryptBuff, nDecryptBuffLen);
                nLen = nDecryptBuffLen; //添加加解密头尾
            }
        }
        else
        {
            if(nLen > PACKET_LENGTH)
            {
                return FALSE;
            }
            else
            {
                memcpy(acNetBuf + dwOffset, pBuf, nLen);
            }
        }

        SDNetMsgHeader* pHeader = (SDNetMsgHeader*)acNetBuf;
        INT32 nPackageLen = SDBuildNetMsgHeader(pHeader, wMsgID, nLen);
        m_poConnection->Send(acNetBuf, nPackageLen);

        return TRUE;

    }

    return FALSE;
}

BOOL CZSClient::SendMsg(UINT16 wMsgID , const CHAR* pData)
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
    CHAR acMsgData[PACKET_LENGTH] = {0};
    INT32 nEncodeRet = pProtocol->Encode(wMsgID, (VOID *)pData, acMsgData, PACKET_LENGTH);
    if (nEncodeRet < 0)
    {
        return FALSE;
    }

    return Send(wMsgID, acMsgData, nEncodeRet);
}


VOID CZSClient::SetPacketProcessor(CSDPacketProcessor*    pPacketProcessor)
{
    m_poPacketProcessor = pPacketProcessor;
}


CZSClientMgr::CZSClientMgr()
{
    m_poLock = NULL;
    m_mapCliSession.clear();
    m_dwCurClientID = 1;
}

CZSClientMgr::~CZSClientMgr()
{
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
}

BOOL CZSClientMgr::Init(UINT32 nMaxClient)
{
    m_poLock = SDNew CSDMutex();
    if(NULL == m_poLock)
    {
        //SYS_CRITICAL("[%s: %d]: Create ISDLock failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    m_poCliSessionPool = SDNew CSDObjectPool<CZSClient, CSDMutex>(nMaxClient, nMaxClient);
    if (NULL == m_poCliSessionPool)
    {
        //SYS_CRITICAL("[%s: %d]: CliSession pool init failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    return TRUE;
}

VOID CZSClientMgr::UnInit()
{
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

ISDSession* CZSClientMgr::CreateSession(ISDConnection* poConnection)
{
    CZSClient* poSession = m_poCliSessionPool->Alloc();
    if (NULL == poSession)
    {
        return NULL;
    }
    UINT32 dwClientID = GenerateClientID();
    poSession->SetCliSessionID(dwClientID);
    poSession->SetPacketProcessor(&m_oCliProcessor);
    return poSession;
}

VOID CZSClientMgr::ReleaseCliSession(CZSClient* pSession)
{
    if(pSession)
    {
        m_poCliSessionPool->Free(pSession);
    }
}

UINT32 CZSClientMgr::GenerateClientID()
{
    m_poLock->Lock();
    m_dwCurClientID++;
    m_poLock->Unlock();

    return m_dwCurClientID;
}

UINT32 CZSClientMgr::GetCliSessionCount()
{
    return m_mapCliSession.size();
}

VOID CZSClientMgr::AddCliSession(CZSClient *poSession)
{
    m_mapCliSession[poSession->GetCliSessionID()] = poSession;
}


CZSClient* CZSClientMgr::FindCliSession(UINT32 dwClientID)
{
    CZSClientMapItr itr
        = m_mapCliSession.find(dwClientID);
    if(itr == m_mapCliSession.end())
        return NULL;

    return itr->second;
}


VOID CZSClientMgr::ChkSessionOnTimer()
{
    for(CZSClientMapItr itr = m_mapCliSession.begin(); itr != m_mapCliSession.end();)
    {
        CZSClient* poCliSession = itr->second;
        if((poCliSession) && ((SDTimeSecs() - poCliSession->GetLastTime()) > 180)) // 180秒就断开
        {
            ISDConnection* m_poConnection = poCliSession->GetConnection();
            if(m_poConnection)
            {
                m_poConnection->Disconnect();
            }
            m_mapCliSession.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}
