#include "gtclient.h"
#include "gtapplication.h"
#include "net/cli/cliprocessor.h"
#include "net/common/compktbuilder.h"
#include "net/cli/clipktbuilder.h"
#include <sdutil.h>
#include <common/server/utility.h>


CGTClient::CGTClient():
    m_dwCliSessionID(0),
    m_poConnection(NULL),
    m_eState(CLIENT_STATE_UNAUTHED)
{
    m_qwLastTime = SDTimeSecs();
    m_bTimeOutClose = FALSE;
    m_poPacketProcessor = NULL;
    m_poUser = NULL;
}

CGTClient::~CGTClient()
{
    //m_poUser释放该链接，下次该用户新链接上来得重新赋值CGTClient链接
    if(m_poUser)
    {
        m_poUser->ReleaseGtClient(this);
    }
}


VOID CGTClient::SetConnection(ISDConnection* poConnection)
{
    m_poConnection = poConnection;
}

bool CGTClient::OnError(INT32 nSDError, INT32 nSysError)
{
    if(NULL != m_poConnection)
    {
        DBG_INFO(_SDT("[%s:%d]: Connection error: IP = %s, Port = %d, SDError: %d, SysError: %d\n"),
                 MSG_MARK, m_poConnection->GetRemoteIPStr(), m_poConnection->GetRemotePort(), nSDError, nSysError);
    }
    return true;
}

VOID CGTClient::OnEstablish()
{
    //CAutoCycle oAutoCycle("OnEstablish");

    ((CGTApplication*)SDGetApp())->GetClientMgr()->AddCliSession(this);

    //判断是否需加解密
    //if(((CGTApplication*)SDGetApp())->GetGTConfig()->GetEncryptFlag()) //目前都加密
    {
        //发送加解密密钥
        UINT8 abyKeyInfo[ENDEC_KEY_LEN + 1] = {0};
        if(RandomNumbercode(abyKeyInfo, ENDEC_KEY_LEN))
        {
            UINT16 wMsgID = CCliPktBuilder::Instance()->EncryptDecryptKeyNtf(abyKeyInfo);
            //USR_INFO(_SDT("[%s:%d]: KeyInfo: %s\n"), MSG_MARK, abyKeyInfo);
            SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
            m_oMGEncrypt.setKey(abyKeyInfo, ENDEC_KEY_LEN);
        }
    }

    ((CGTApplication*)SDGetApp())->IncConnectNum();
}

VOID CGTClient::HeartbeatAck()
{
    UINT16 wMsgID = CCliPktBuilder::Instance()->HeartbeatAck();
    SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
}


VOID CGTClient::OnRecv(const CHAR* pBuf, UINT32 nLen)
{
    //CAutoCycle oAutoCycle("OnRecv");

    SDNetMsgHeader* pNetHeader = (SDNetMsgHeader*)pBuf;
    UINT16 wMsgID = SDNtohs(pNetHeader->wMsgID);
    UINT32 dwDataLen = SDNtohl(pNetHeader->dwDataLen);
    CHAR* pData = (CHAR*)pBuf + sizeof(SDNetMsgHeader);
    m_qwLastTime = SDTimeSecs();
    if (wMsgID == CLIGT_HEARTBEAT_REQ)
    {
        HeartbeatAck();
        return;
    }
    CSDPipeChannel* poSDPipeChannel = NULL;
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
    BOOL bProccess = TRUE;

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

    //gate优先处理
    if(m_poPacketProcessor && m_poPacketProcessor->LookupProcessFunc(wMsgID))
    {
        static CHAR  szDecodeData[PACKET_LENGTH] = {0};
        memset(szDecodeData, 0x00, sizeof(PACKET_LENGTH));
        INT32 nDecodeLen = CProtoCliGT::Instance()->Decode( wMsgID,
                           pData,
                           dwDataLen,
                           szDecodeData,
                           PACKET_LENGTH);
        if ( nDecodeLen > 0)
        {

            SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pBuf;
            pPipeHeader->wMsgID    = wMsgID;
            if(m_poUser)
            {
                pPipeHeader->dwTransID = m_poUser->GetUserSessionID();
            }
            else
            {
                pPipeHeader->dwTransID = SDHtonl(m_dwCliSessionID);
            }

            bProccess = m_poPacketProcessor->ProcessPacket(this,
                        pPipeHeader,
                        szDecodeData,
                        nDecodeLen);
        }
    }
    if (FALSE == bProccess)
    {
        return;
    }

    if (CLIENT_DIRECTION_LS == GetTransferTarget(wMsgID))
    {
        //poSDPipeChannel = poGTApplication->GetLSServer();
        //if (poSDPipeChannel)
        //{
        //    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pBuf;
        //    pPipeHeader->dwTransID = SDHtonl(m_dwCliSessionID);
        //    pPipeHeader->wMsgID    = SDHtons(wMsgID);
        //    poSDPipeChannel->Send(pBuf, dwDataLen + sizeof(SDPipeMsgHeader));
        //}
        //else
        //{
        //    UINT16 wMsgID = CCliPktBuilder::Instance()->ServerNotStartupNtf(LOGIN_SERVER);
        //    SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
        //}
    }
    else if ( CLIENT_DIRECTION_GS == GetTransferTarget(wMsgID))
    {
        poSDPipeChannel = poGTApplication->GetGSServer();
        if (poSDPipeChannel)
        {
            SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pBuf;
            //pPipeHeader->dwTransID = SDHtonl(m_dwCliSessionID);
            if(NULL == m_poUser)
            {
                //todo,异常,log
                return ;
            }
            pPipeHeader->dwTransID = SDHtonl(m_poUser->GetUserSessionID());
            pPipeHeader->wMsgID    = SDHtons(wMsgID);
            poSDPipeChannel->Send(pBuf, dwDataLen + sizeof(SDPipeMsgHeader));
        }
        else
        {
            UINT16 wMsgID = CCliPktBuilder::Instance()->ServerNotStartupNtf(GAME_SERVER);
            SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
        }
    }
}

VOID CGTClient::OnTerminate()
{
    SendCliDownRpt();
    m_poConnection = NULL;
    //((CGTApplication*)SDGetApp())->GetClientMgr()->RemoveCliSession(this);
}

BOOL CGTClient::SendCliDownRpt()
{
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();

    CSDPipeChannel* poPipeChannel = NULL;
    if (CLIENT_STATE_ENTERING_GAME == m_eState
            || CLIENT_STATE_IN_GS == m_eState)
    {
        poPipeChannel = poGTApplication->GetGSServer();
    }
    else if (CLIENT_STATE_UNAUTHED == m_eState
             || CLIENT_STATE_AUTHING == m_eState)
    {
        //poPipeChannel = poGTApplication->GetLSServer();
        return TRUE;
    }
    //避免多次发送下线请求，而在此之间用户异步已从新登录
    else if(CLIENT_STATE_LEAVEING_GAME == m_eState)
    {
        return TRUE;
    }
    //踢人，将之前的连接踢下线，新连接正在进入游戏中.此时旧连接断开不向GS发下线消息
    else if((CLIENT_STATE_ENTERING_GAME_KICK == m_eState)
            && (NULL != m_poUser)
            && (this != m_poUser->GetGTClient()))
    {
        return TRUE;
    }

    if ((poPipeChannel) && (NULL != m_poUser))
    {
        UINT16 wMsgID = CCOMPktBuilder::Instance()->CliDownRpt();
        if (!poPipeChannel->SendMsg( m_poUser->GetUserSessionID(), wMsgID, CCOMPktBuilder::Instance()->GetPacketBuffer()))
        {
            //TODO: 发送失败应该如何处理？
            DBG_INFO(_SDT("[%s:%d]: send packet failed"), MSG_MARK);
            return FALSE;
        }
    }

    //避免多次发送下线请求，而在此之间用户异步已从新登录
    m_eState = CLIENT_STATE_LEAVEING_GAME;

    return TRUE;
}

VOID CGTClient::Release()
{
    ((CGTApplication*)SDGetApp())->GetClientMgr()->ReleaseCliSession(this);
    m_poConnection = NULL;
    ((CGTApplication*)SDGetApp())->DecConnectNum();
}

ISDConnection* CGTClient::GetConnection()
{
    return m_poConnection;
}



VOID CGTClient::SetCliSessionID(UINT32 dwCliSessionID)
{
    m_dwCliSessionID = dwCliSessionID;
}

UINT32 CGTClient::GetCliSessionID()
{
    return m_dwCliSessionID;
}

VOID CGTClient::SetState(EClientState eState)
{
    m_eState = eState;
}

EClientState CGTClient::GetState()
{
    return m_eState;
}

ETransferDirection CGTClient::GetTransferTarget(UINT16 wMsgID )
{
    ETransferDirection eDirction = CLIENT_DIRECTION_NONE;
    if ((m_eState == CLIENT_STATE_UNAUTHED) || (CLIENT_STATE_RERI_USERNAME == m_eState))
    {
        eDirction = CLIENT_DIRECTION_LS;
    }
    else if ((m_eState == CLIENT_STATE_AUTHED) || (m_eState == CLIENT_STATE_IN_GS))
    {
        eDirction = CLIENT_DIRECTION_GS;
    }

    //需要特殊处理的协议
    switch(wMsgID)
    {
    case CLILS_LOGIN_REQ:
    case CLILS_LOGIN_EX_REQ:
    case CLILS_BIND_REQ:
    case CLILS_UNBIND_REQ:
    case CLILS_REGISTER_REQ:
        if (m_eState >= CLIENT_STATE_AUTHED)
        {
            eDirction = CLIENT_DIRECTION_LS;
        }
        break;
    default:
        break;
    }
    return eDirction;
}


VOID CGTClient::Disconnect()
{
    if (m_poConnection)
    {
        m_poConnection->Disconnect();
        OnTerminate();
    }
}

BOOL CGTClient::Send(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen)
{
    if (m_poConnection)
    {
        CHAR acNetBuf[PACKET_LENGTH] = {0};
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

BOOL CGTClient::SendMsg(UINT16 wMsgID , const CHAR* pData)
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
    static CHAR acMsgData[PACKET_LENGTH] = {0};
    INT32 nEncodeRet = pProtocol->Encode(wMsgID, (VOID *)pData, acMsgData, PACKET_LENGTH);
    if (nEncodeRet < 0)
    {
        return FALSE;
    }

    return Send(wMsgID, acMsgData, nEncodeRet);
}

VOID CGTClient::SetPacketProcessor(CSDPacketProcessor*    pPacketProcessor)
{
    m_poPacketProcessor = pPacketProcessor;
}


CGTClientMgr::CGTClientMgr()
{
    m_poLock = NULL;
    m_mapCliSession.clear();
    m_itrCurDeal = m_mapCliSession.end();
    m_dwCurClientID = 0;
}

CGTClientMgr::~CGTClientMgr()
{
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
}

BOOL CGTClientMgr::Init(UINT32 nMaxClient)
{
    m_poLock = SDNew CSDMutex();
    if(NULL == m_poLock)
    {
        //SYS_CRITICAL("[%s: %d]: Create ISDLock failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    m_poCliSessionPool = SDNew CSDObjectPool<CGTClient, CSDMutex>(nMaxClient, nMaxClient);
    if (NULL == m_poCliSessionPool)
    {
        //SYS_CRITICAL("[%s: %d]: CliSession pool init failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    return TRUE;
}

VOID CGTClientMgr::UnInit()
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

ISDSession* CGTClientMgr::CreateSession(ISDConnection* poConnection)
{
    CGTClient* poSession = m_poCliSessionPool->Alloc();
    if (NULL == poSession)
    {
        return NULL;
    }
    UINT32 dwClientID = GenerateClientID();
    poSession->SetCliSessionID(dwClientID);
    poSession->SetPacketProcessor(&m_oCliProcessor);
    return poSession;
}

VOID CGTClientMgr::ReleaseCliSession(CGTClient* pSession)
{
    m_poCliSessionPool->Free(pSession);
}

UINT32 CGTClientMgr::GenerateClientID()
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

UINT32 CGTClientMgr::GetCliSessionCount()
{
    return m_mapCliSession.size();
}

VOID CGTClientMgr::AddCliSession(CGTClient *poSession)
{
    m_mapCliSession[poSession->GetCliSessionID()] = poSession;
}

VOID CGTClientMgr::RemoveCliSession(CGTClient* poSession)
{
    if(NULL == poSession)
    {
        return;
    }
    //长时间未收到包的检查关闭，由CGTClientMgr::CkSessionOnTimer()方法来删除
    if(poSession->GetTimeOutCloseFlag())
    {
        return;
    }

    CGTClientMapItr itr =
        m_mapCliSession.find(poSession->GetCliSessionID());

    if (itr != m_mapCliSession.end())
    {
        if( itr == m_itrCurDeal)
        {
            m_mapCliSession.erase(m_itrCurDeal++);
        }
        else
        {
            m_mapCliSession.erase(itr);
        }
    }
}

CGTClient* CGTClientMgr::FindCliSession(UINT32 dwClientID)
{
    CGTClientMapItr itr
        = m_mapCliSession.find(dwClientID);
    if(itr == m_mapCliSession.end())
        return NULL;

    return itr->second;
}


VOID CGTClientMgr::ChkSessionOnTimer()
{
    if(m_itrCurDeal == m_mapCliSession.end())
    {
        m_itrCurDeal = m_mapCliSession.begin();
    }

    UINT32 dwDealSessionNum = ((CGTApplication*)SDGetApp())->GetGTConfig()->GetDealSessionNum();
    UINT32 dwCloseSessionElapse = ((CGTApplication*)SDGetApp())->GetGTConfig()->GetCloseSessionElapse();
    UINT32 dwCount = 0;
    for(; (m_itrCurDeal != m_mapCliSession.end()) && (dwCount < dwDealSessionNum); dwCount++)
    {
        CGTClient* poCliSession = m_itrCurDeal->second;
        if((poCliSession) && ((SDTimeSecs() - poCliSession->GetLastTime()) > dwCloseSessionElapse))
        {
            ISDConnection *poConnection = poCliSession->GetConnection();
            if (NULL != poConnection)
            {
                poConnection->Disconnect();
            }
            poCliSession->SetTimeOutCloseFlag();
            poCliSession->OnTerminate();
            m_mapCliSession.erase(m_itrCurDeal++);
        }
        else
        {
            m_itrCurDeal++;
        }
    }
}

VOID CGTClientMgr::GSDisConnect(UINT32 dwServerID)
{
    CGTClientMapItr itr = m_mapCliSession.begin();
    for(; itr != m_mapCliSession.end(); )
    {
        CGTClient* poCliSession = itr->second;
        if((NULL != poCliSession) && (poCliSession->GetState() == CLIENT_STATE_IN_GS))
        {
            ISDConnection *poConnection = poCliSession->GetConnection();
            if (NULL != poConnection)
            {
                poConnection->Disconnect();
            }
            poCliSession->SendCliDownRpt();
            m_mapCliSession.erase(itr++);
        }
        else
        {
            itr++;
        }

    }
}

