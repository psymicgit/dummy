

#include "ppclient.h"

////////////////////////////////////////////////////////////////////////////////
//        NAME: CPP
// DESCRIPTION: Constructor of CPP class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: m_iXPriority, m_iPPSrvPort, RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR:
// AUTHOR/DATE: J
//
////////////////////////////////////////////////////////////////////////////////
CPPClient::CPPClient()
{
    nLen = 0;
    m_iPPSrvPort = 0;
    memset(szRecvBuf, 0x00, sizeof(szRecvBuf));
    memset(szSendBuf, 0x00, sizeof(szSendBuf));
#if defined(WIN32) || defined(WIN64)
    // Initialize WinSock
    WSADATA wsaData;
    WORD wVer = MAKEWORD(2, 2);
    if (WSAStartup(wVer, &wsaData) != NO_ERROR)
        throw ECPP(ECPP::WSA_STARTUP);
    if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )
    {
        WSACleanup();
        throw ECPP(ECPP::WSA_VER);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: CPP
// DESCRIPTION: Destructor of CPP class.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf, SendBuf
// MODIFIES GL: RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR:
// AUTHOR/DATE: J
//
////////////////////////////////////////////////////////////////////////////////
CPPClient::~CPPClient()
{
#if defined(WIN32) || defined(WIN64)
    WSACleanup();
#endif
}


////////////////////////////////////////////////////////////////////////////////
//        NAME: Send
// DESCRIPTION: Sending the mail. .
//   ARGUMENTS: none
// USES GLOBAL: m_sPPSrvName, m_iPPSrvPort, SendBuf, RecvBuf, m_sLogin,
//              m_sPassword, m_sMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_sMsgBody, Attachments,
// MODIFIES GL: SendBuf
//     RETURNS: void
//      AUTHOR:
// AUTHOR/DATE: J
//
////////////////////////////////////////////////////////////////////////////////
bool  CPPClient::Do(char token_key[16], char szUserName[64], UINT64 &qwUserID)
{
    bool bAccepted;
    // ***** CONNECTING TO PP SERVER *****

    // connecting to remote host:
    if( (hSocket = ConnectRemoteServer(m_sPPSrvName.c_str(), m_iPPSrvPort)) == INVALID_SOCKET )
        throw ECPP(ECPP::WSA_INVALID_SOCKET);

    memset(szSendBuf, 0x00, sizeof(szSendBuf));
    RECVUCLOGINDATA *pRECVUCLOGINDATA = (RECVUCLOGINDATA *)szSendBuf;
    pRECVUCLOGINDATA->SetLen(sizeof(RECVUCLOGINDATA));
    pRECVUCLOGINDATA->SetCommand(0xAA000021);
    pRECVUCLOGINDATA->SetTokenKey(token_key);
    SendData(szSendBuf, sizeof(RECVUCLOGINDATA));

    bAccepted = false;
    do
    {
        ReceiveData();
        RECVPPPSDATA *pRECVPPPSDATA = (RECVPPPSDATA*)szRecvBuf;
        switch(pRECVPPPSDATA->GetCommand())
        {
        case 0xAA00F022:
            bAccepted = true;
			if (1 == pRECVPPPSDATA->status)
			{
				memcpy(szUserName, pRECVPPPSDATA->username, pRECVPPPSDATA->GetLen() - sizeof(RESPONSEUCDATA));
			}
			else
			{
				return false;
			}
            break;
        default:
            throw ECPP(ECPP::SERVER_NOT_READY);
        }
    }
    while(!bAccepted);

#if defined(WIN32) || defined(WIN64)
    closesocket(hSocket);
#else
    close(hSocket);
#endif
    hSocket = NULL;
	return bAccepted;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ConnectRemoteServer
// DESCRIPTION: Connecting to the service running on the remote server.
//   ARGUMENTS: const char *server - service name
//              const unsigned short port - service port
// USES GLOBAL: m_pcPPSrvName, m_iPPSrvPort, SendBuf, RecvBuf, m_pcLogin,
//              m_pcPassword, m_pcMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_pcMsgBody, Attachments,
// MODIFIES GL: m_oError
//     RETURNS: socket of the remote service
//      AUTHOR:
// AUTHOR/DATE: J
////////////////////////////////////////////////////////////////////////////////
SOCKET CPPClient::ConnectRemoteServer(const char *szServer, const unsigned short nPort_)
{
    unsigned short nPort = 0;
    SOCKADDR_IN sockAddr;
    unsigned long ul = 1;
    fd_set fdwrite, fdexcept;
    timeval timeout;
    int res = 0;

    timeout.tv_sec = TIME_IN_SEC;
    timeout.tv_usec = 0;

    SOCKET hSocket = INVALID_SOCKET;

    if((hSocket = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        throw ECPP(ECPP::WSA_INVALID_SOCKET);

    if(nPort_ != 0)
        nPort = htons(nPort_);
    else
        throw ECPP(ECPP::WSA_HOSTNAME);

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = nPort;
    if((sockAddr.sin_addr.s_addr = inet_addr(szServer)) == INADDR_NONE)
    {
        LPHOSTENT host;

        host = gethostbyname(szServer);
        if (host)
            memcpy(&sockAddr.sin_addr, host->h_addr_list[0], host->h_length);
        else
        {
#if defined(WIN32) || defined(WIN64)
            closesocket(hSocket);
#else
            close(hSocket);
#endif
            throw ECPP(ECPP::WSA_GETHOSTBY_NAME_ADDR);
        }
    }

    // start non-blocking mode for socket:
#if defined(WIN32) || defined(WIN64)
    if(ioctlsocket(hSocket, FIONBIO, (unsigned long*)&ul) == SOCKET_ERROR)
#else
    if(ioctl(hSocket, FIONBIO, (unsigned long*)&ul) == SOCKET_ERROR)
#endif
    {

#if defined(WIN32) || defined(WIN64)
        closesocket(hSocket);
#else
        close(hSocket);
#endif
        throw ECPP(ECPP::WSA_IOCTLSOCKET);
    }

    if(connect(hSocket, (LPSOCKADDR)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
    {
#if defined(WIN32) || defined(WIN64)
        if(WSAGetLastError() != WSAEWOULDBLOCK)
#else
        if(errno != EINPROGRESS)
#endif
        {
#if defined(WIN32) || defined(WIN64)
            closesocket(hSocket);
#else
            close(hSocket);
#endif
            throw ECPP(ECPP::WSA_CONNECT);
        }
    }
    else
        return hSocket;

    while(true)
    {
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcept);

        FD_SET(hSocket, &fdwrite);
        FD_SET(hSocket, &fdexcept);

        if((res = select(hSocket + 1, NULL, &fdwrite, &fdexcept, &timeout)) == SOCKET_ERROR)
        {
#if defined(WIN32) || defined(WIN64)
            closesocket(hSocket);
#else
            close(hSocket);
#endif
            throw ECPP(ECPP::WSA_SELECT);
        }

        if(!res)
        {
#if defined(WIN32) || defined(WIN64)
            closesocket(hSocket);
#else
            close(hSocket);
#endif
            throw ECPP(ECPP::SELECT_TIMEOUT);
        }
        if(res && FD_ISSET(hSocket, &fdwrite))
            break;
        if(res && FD_ISSET(hSocket, &fdexcept))
        {
#if defined(WIN32) || defined(WIN64)
            closesocket(hSocket);
#else
            close(hSocket);
#endif
            throw ECPP(ECPP::WSA_SELECT);
        }
    } // while

    FD_CLR(hSocket, &fdwrite);
    FD_CLR(hSocket, &fdexcept);

    return hSocket;
}



////////////////////////////////////////////////////////////////////////////////
//        NAME: ReceiveData
// DESCRIPTION: Receives a row terminated '\n'.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf
// MODIFIES GL: RecvBuf
//     RETURNS: void
//      AUTHOR:
// AUTHOR/DATE: J
//
////////////////////////////////////////////////////////////////////////////////
void CPPClient::ReceiveData()
{
    int res;
    fd_set fdread;
    timeval time;

    time.tv_sec = TIME_IN_SEC;
    time.tv_usec = 0;

    while(1)
    {
        FD_ZERO(&fdread);

        FD_SET(hSocket, &fdread);

        if((res = select(hSocket + 1, &fdread, NULL, NULL, &time)) == SOCKET_ERROR)
        {
            FD_CLR(hSocket, &fdread);
            throw ECPP(ECPP::WSA_SELECT);
        }

        if(!res)
        {
            //timeout
            FD_CLR(hSocket, &fdread);
            throw ECPP(ECPP::SERVER_NOT_RESPONDING);
        }

        if(res && FD_ISSET(hSocket, &fdread))
        {
            if(nLen >= BUFFER_SIZE)
            {
                FD_CLR(hSocket, &fdread);
                throw ECPP(ECPP::LACK_OF_MEMORY);
            }

            int v_nLen = recv(hSocket, &szRecvBuf[nLen], sizeof(szRecvBuf) - nLen, 0);
            if(v_nLen == SOCKET_ERROR)
            {
                FD_CLR(hSocket, &fdread);
                throw ECPP(ECPP::WSA_RECV);
            }
			nLen += v_nLen;
            RECVDATA *pRECVDATA = (RECVDATA*)szRecvBuf;
            if (nLen >= pRECVDATA->GetLen() && nLen != 0)
            {
                break;
            }
        }
    }

    FD_CLR(hSocket, &fdread);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SendData
// DESCRIPTION: Sends data from SendBuf buffer.
//   ARGUMENTS: none
// USES GLOBAL: SendBuf
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR:
// AUTHOR/DATE: J
////////////////////////////////////////////////////////////////////////////////
void CPPClient::SendData(const char szSendBuf[BUFFER_SIZE], const int nLen)
{
    int idx = 0, res, nLeft = nLen;
    fd_set fdwrite;
    timeval time;

    time.tv_sec = TIME_IN_SEC;
    time.tv_usec = 0;


    while(1)
    {
        FD_ZERO(&fdwrite);

        FD_SET(hSocket, &fdwrite);

        if((res = select(hSocket + 1, NULL, &fdwrite, NULL, &time)) == SOCKET_ERROR)
        {
            FD_CLR(hSocket, &fdwrite);
            throw ECPP(ECPP::WSA_SELECT);
        }

        if(!res)
        {
            //timeout
            FD_CLR(hSocket, &fdwrite);
            throw ECPP(ECPP::SERVER_NOT_RESPONDING);
        }

        if(res && FD_ISSET(hSocket, &fdwrite))
        {
            if(nLeft > 0)
            {
                if((res = send(hSocket, &szSendBuf[idx], nLeft, 0)) == SOCKET_ERROR)
                {
                    FD_CLR(hSocket, &fdwrite);
                    throw ECPP(ECPP::WSA_SEND);
                }
                if(!res)
                    break;
                nLeft -= res;
                idx += res;
            }
            else
                break;
        }
    }

    FD_CLR(hSocket, &fdwrite);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetLocalHostName
// DESCRIPTION: Returns local host name.
//   ARGUMENTS: none
// USES GLOBAL: m_pcLocalHostName
// MODIFIES GL: m_oError, m_pcLocalHostName
//     RETURNS: socket of the remote service
//      AUTHOR:
// AUTHOR/DATE: J
////////////////////////////////////////////////////////////////////////////////
const char* CPPClient::GetLocalHostName() const
{
    char* str = NULL;
    if((str = new char[255]) == NULL)
        throw ECPP(ECPP::LACK_OF_MEMORY);
    if(gethostname(str, 255) == SOCKET_ERROR)
    {
        delete[] str;
        throw ECPP(ECPP::WSA_HOSTNAME);
    }
    delete[] str;
    return m_sLocalHostName.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetPPServer
// DESCRIPTION: Setting the PP service name and port.
//   ARGUMENTS: const char* SrvName - PP service name
//              const unsigned short SrvPort - SMTO service port
// USES GLOBAL: m_sPPSrvName
// MODIFIES GL: m_sPPSrvName
//     RETURNS: none
//      AUTHOR:
// AUTHOR/DATE: J
//							JO 2010-0708
////////////////////////////////////////////////////////////////////////////////
void CPPClient::SetPPServer(const char* SrvName, const unsigned short SrvPort)
{
    m_iPPSrvPort = SrvPort;
    m_sPPSrvName.erase();
    m_sPPSrvName.insert(0, SrvName);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetErrorText (friend function)
// DESCRIPTION: Returns the string for specified error code.
//   ARGUMENTS: CPPError ErrorId - error code
// USES GLOBAL: none
// MODIFIES GL: none
//     RETURNS: error string
//      AUTHOR:
// AUTHOR/DATE: J
////////////////////////////////////////////////////////////////////////////////
std::string ECPP::GetErrorText() const
{
    switch(ErrorCode)
    {
    case ECPP::WSA_STARTUP:
        return "Unable to initialise winsock2";
    case ECPP::WSA_VER:
        return "Wrong version of the winsock2";
    case ECPP::WSA_SEND:
        return "Function send() failed";
    case ECPP::WSA_RECV:
        return "Function recv() failed";
    case ECPP::WSA_CONNECT:
        return "Function connect failed";
    case ECPP::WSA_GETHOSTBY_NAME_ADDR:
        return "Unable to determine remote server";
    case ECPP::WSA_INVALID_SOCKET:
        return "Invalid winsock2 socket";
    case ECPP::WSA_HOSTNAME:
        return "Function hostname() failed";
    case ECPP::WSA_IOCTLSOCKET:
        return "Function ioctlsocket() failed";
    case ECPP::SERVER_NOT_READY:
        return "Server is not ready";
    case ECPP::SERVER_NOT_RESPONDING:
        return "Server not responding";
    case ECPP::LACK_OF_MEMORY:
        return "Lack of memory";
    case ECPP::RECVBUF_IS_EMPTY:
        return "RecvBuf is empty";
    case ECPP::SENDBUF_IS_EMPTY:
        return "SendBuf is empty";
    default:
        return "Undefined error id";
    }
}

