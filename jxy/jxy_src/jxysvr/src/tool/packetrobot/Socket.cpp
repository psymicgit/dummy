

#include "Socket.h"
#include <ctime>
#ifndef WIN32
#include <sys/ioctl.h>
#include <netdb.h>
#endif


    //////////////////////////////////////////////////////////////////////
    // Construction/Destruction
    //////////////////////////////////////////////////////////////////////
    #ifdef  WIN32
    #pragma comment(lib, "Ws2_32.lib")
    int afxInitSocket()
    {
	    WSADATA	wsd;
	    //启动socket库
	    return WSAStartup(MAKEWORD(2,2), &wsd);
    }
    void afxExitSocket()
    {
        WSACleanup();
    }

    int  g_nInitSocket = afxInitSocket() ;
    char  g_nReleaseSocket = (char)atexit(&afxExitSocket) ;
    #endif

    //    signal(SIGPIPE,SIG_IGN) ;

    //////////////////////////////////////////////////////////////////////////

    CSjSocket::CSjSocket()
    {
	    m_hSocket = INVALID_SOCKET;  
        m_lLastErr= 0 ;
    }

    CSjSocket::~CSjSocket()
    {
	    Close();
    }

    void CSjSocket::Close() 
    {
    #ifdef  WIN32
	    if (m_hSocket != INVALID_SOCKET)
	    {
		    //VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
            closesocket(m_hSocket);
            m_hSocket = INVALID_SOCKET ;
	    }
    #else
	    if (m_hSocket >= 0)
	    {
		    close(m_hSocket);
            m_hSocket = INVALID_SOCKET ;
	    }
    #endif  
    }

    BOOL CSjSocket::Create(UINT nSocketPort,int nType,const char* szIPAddress)
    {
	    // Allocate socket handle
        m_hSocket = socket(PF_INET, nType, 0);

    #ifdef  WIN32
        if(m_hSocket == INVALID_SOCKET)
    #else
	    if(m_hSocket < 0)
    #endif
	    {
            m_lLastErr      = errno ;
            m_strLastErr    = strerror((int)m_lLastErr) ;
		    return FALSE;
	    }

	    BOOL bOptVal = TRUE;
	    /* Set to enable reuse the address,or the bind() will result an error*/
	    int rc = setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptVal, sizeof(int));
	    if (rc == SOCKET_ERROR)
	    {
            m_lLastErr      = errno ;
            m_strLastErr    = strerror((int)m_lLastErr) ;
		    Close();
		    return FALSE;
	    }

	    if (Bind(nSocketPort,szIPAddress))
        {        
    #if defined(WIN32) && defined(_DEBUG)
            if (nSocketPort == 0 || szIPAddress==NULL)
            {
	            SOCKADDR_IN sockAddr;
                socklen_t nAddrLen = sizeof(sockAddr) ;
	            memset(&sockAddr,0,sizeof(sockAddr));
                getsockname(m_hSocket,(PSOCKADDR)&sockAddr,&nAddrLen) ;
                printf("localhost ip: %s - %d \n",inet_ntoa(sockAddr.sin_addr),ntohs(sockAddr.sin_port)) ;
            }
    #endif
		    return TRUE;
        }
	    else
	    {
            m_lLastErr      = errno ;
            m_strLastErr    = strerror((int)m_lLastErr) ;
		    Close();
		    return FALSE;
	    }
    }

    BOOL CSjSocket::Bind(UINT nSocketPort, const char * lpszSocketAddress)
    {
	    SOCKADDR_IN sockAddr;
	    memset(&sockAddr,0,sizeof(sockAddr));

	    char* lpszAscii = (char*)lpszSocketAddress;
	    sockAddr.sin_family = AF_INET;

	    if (lpszAscii == NULL)
		    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	    else
	    {
		    DWORD lResult = inet_addr(lpszAscii);
		    if (lResult == INADDR_NONE)
		    {
                m_lLastErr      = errno ;
                m_strLastErr    = strerror((int)m_lLastErr) ;
                return FALSE;
		    }
		    sockAddr.sin_addr.s_addr = lResult;
	    }

	    sockAddr.sin_port = htons((u_short)nSocketPort);

        // Bind to socket address
        if(bind(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
        {
            m_lLastErr      = errno ;
            m_strLastErr    = strerror((int)m_lLastErr) ;
		    Close();
		    return FALSE;
        }
   
	    socklen_t addrlen = sizeof(SOCKADDR);

        // Get full network.number.socket address
        getsockname(m_hSocket, (PSOCKADDR)&sockAddr, (socklen_t*)&addrlen) ;

	    return TRUE;
    }

    int CSjSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
    {
	    unsigned long length = 0;
	    int ret = 0 ;

    #ifdef  WIN32
	    ret = ioctlsocket(m_hSocket, FIONREAD, &length);
    #else
	    ret = ioctl(m_hSocket, FIONREAD, &length);
    #endif

        if (ret == SOCKET_ERROR)
            return -1 ;
        
	    if (/*ret == SOCKET_ERROR || */length == 0)
		    return 0;

	    return Recv((char*)lpBuf, nBufLen, nFlags);
    }

    unsigned long CSjSocket::CanRecvDataLen()
    {        
        unsigned long ulLength = 0;
#ifdef  WIN32
        if (ioctlsocket(m_hSocket, FIONREAD, &ulLength) == SOCKET_ERROR) 
#else
            if (ioctl(m_hSocket, FIONREAD, &ulLength)== SOCKET_ERROR) 
#endif
            {
                Close();
                ulLength = 0 ;
            }
            return ulLength ;
    }

    int CSjSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
    {
        //return SendChunk(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
        int nLeft, nWritten;
        PBYTE pBuf = (PBYTE)lpBuf;
        nLeft = nBufLen;
        
        while (nLeft > 0)
        {
            nWritten = SendChunk(pBuf, nLeft, nFlags);
            if (nWritten == SOCKET_ERROR)
                return nWritten;
            
            nLeft -= nWritten;
            pBuf += nWritten;
            if (nLeft>0)
            {
                Sleep(1) ;
            }
        }
	    return nBufLen - nLeft;
    }
    
    int	CSjSocket::SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, const char * lpszHostAddress, int nFlags)
    {
	    SOCKADDR_IN sockAddr;
	    memset(&sockAddr,0,sizeof(sockAddr));

	    char * lpszAscii = (char*)lpszHostAddress;
	    sockAddr.sin_family = AF_INET;

	    if (lpszAscii == NULL)
		    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	    else
	    {
		    unsigned long lResult = inet_addr(lpszAscii);
		    if (lResult == INADDR_NONE)
		    {
                m_lLastErr      = errno ;
                m_strLastErr    = strerror((int)m_lLastErr) ;
			    return FALSE;
		    }
		    sockAddr.sin_addr.s_addr = lResult;
	    }

	    sockAddr.sin_port = htons((u_short)nHostPort);

	    return SendTo((char*)lpBuf, nBufLen, (const struct sockaddr *)&sockAddr, sizeof(sockAddr), nFlags);
    
    }
    int CSjSocket::ReceiveFrom(void* lpBuf, int nBufLen, string& rSocketAddress, UINT& rSocketPort,int nTimeOut,int nFlags, bool bIsDataStream)
    {
	    int nfds =1;
	    int ret;
	    fd_set readfds;
	    timeval tTimeVal;
	    FD_ZERO(&readfds);
	    FD_SET(m_hSocket,&readfds);
	    tTimeVal.tv_usec = 0 ;
	    tTimeVal.tv_sec = nTimeOut;
    #ifndef WIN32
        nfds += m_hSocket ;
    #endif
	    ret = select(nfds,&readfds,NULL,NULL,&tTimeVal);
	    if(ret <= 0)
		    return ret; //time out or socket error
	    ret = ReceiveFrom(lpBuf,nBufLen,rSocketAddress,rSocketPort,nFlags,bIsDataStream);

	    return ret;
    }

    int CSjSocket::ReceiveFrom(void* lpBuf, int nBufLen, string& rSocketAddress, UINT& rSocketPort, int nFlags, bool bIsDataStream)
    {
	    SOCKADDR_IN sockAddr;
	    socklen_t nSockAddrLen = sizeof(sockAddr);
	    memset(&sockAddr, 0, sizeof(sockAddr));

        int nResult = ReceiveFrom(lpBuf,nBufLen,*(SOCKADDR*)&sockAddr,nSockAddrLen,nFlags,bIsDataStream);
	    if(nResult != SOCKET_ERROR)
	    {
		    rSocketPort = ntohs(sockAddr.sin_port);
		    rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	    }
	    return nResult ;
    }

    int CSjSocket::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR& sockAddr, socklen_t& nSockAddrLen, int nFlags, bool bIsDataStream)
    {
        memset(&sockAddr, 0, sizeof(sockAddr));
        nSockAddrLen = sizeof(sockAddr);
        
        if (bIsDataStream)
        {//默认如果是数据流的形式传输，那么就数据的第一个字符必须是UINT格式的包长度值
            
            int nLen = RecvFrom((char*)lpBuf, nBufLen, (SOCKADDR*)&sockAddr, (socklen_t*)&nSockAddrLen, nFlags);
            if(nLen <= 0 )
                return nLen ;
            
            int nPduLen = nLen ;
            int nTotal  = sizeof(UINT) + 1 ;
            if (nLen >= sizeof(UINT))
            {
                nTotal = ntohl(*((UINT*)lpBuf)) ;
            }
            
            time_t stCurTime ;
            time_t stOldTime = time(NULL);
            
            while (nPduLen<nTotal)
            {
                nLen = RecvFrom((char*)lpBuf+nPduLen, nBufLen-nPduLen, (SOCKADDR*)&sockAddr,(socklen_t*)&nSockAddrLen, nFlags);
                if(nLen < 0)
                {
                    return nLen ;
                }
                
                stCurTime = time(NULL);
                
                nPduLen += nLen ;
                
                if(nTotal==sizeof(UINT) + 1 && nPduLen>=sizeof(UINT))
                {
                    nTotal = ntohl(*((UINT*)lpBuf)) ;
                }
                
                if(abs((int)difftime(stCurTime,stOldTime)) > 2) //按一秒钟超时计算
                {
                    printfT(_T("recvfrom stream data time out : 2 secs.")) ;
                    //LogFile(szLogMsg) ;                
                    return FALSE ;
                }
            }
            
            return nPduLen ;
        }
        
        //非流数据传输
        return RecvFrom((char*)lpBuf, nBufLen, (SOCKADDR*)&sockAddr, (socklen_t*)&nSockAddrLen, nFlags);
    }

    BOOL CSjSocket::Connect(const char * lpszHostAddress, UINT nHostPort,CString* lpSvrIp/*=NULL*/)
    {
	    //ASSERT(lpszHostAddress != NULL);

	    SOCKADDR_IN sockAddr;
	    memset(&sockAddr,0,sizeof(sockAddr));

	    char* lpszAscii = (char *)lpszHostAddress;
	    sockAddr.sin_family = AF_INET;
	    sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
        if (lpSvrIp) *lpSvrIp = lpszAscii ;

	    if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	    {
		    LPHOSTENT lphost;
		    lphost = gethostbyname(lpszAscii);
		    if (lphost != NULL)
            {
			    sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
                if (lpSvrIp) *lpSvrIp = inet_ntoa(sockAddr.sin_addr) ;
            }
		    else
		    {
//             #ifdef WIN32
//                 m_strLastErr    = GetErrorText((LPDWORD)&m_lLastErr) ;
//             #else
                m_lLastErr      = 11001  ;
                m_strLastErr    = "域名解析失败，无此域名" ;
//            #endif
			    return FALSE;
		    }
	    }

	    sockAddr.sin_port = htons((u_short)nHostPort);

	    if(connect(m_hSocket,(SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR )
        {
//             #ifdef WIN32
//                 m_strLastErr    = GetErrorText((LPDWORD)&m_lLastErr) ;
//             #else
                m_lLastErr      = WSAECONNREFUSED  ;
                m_strLastErr    = "连接服务器失败" ;
//             #endif
		    Close();
		    return FALSE;
        }

	    return TRUE;
    }

    BOOL CSjSocket::IOCtl(long lCommand, unsigned long* lpArgument)
    { 
    #ifdef WIN32
        return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); 
    #else
        return (SOCKET_ERROR != ioctl(m_hSocket, lCommand, lpArgument)); 
    #endif
    }

    BOOL CSjSocket::SetNonBlockingMode(int flag)
    {
	    unsigned long arg = flag;
	    return IOCtl(FIONBIO, &arg);
    }

    BOOL CSjSocket::Listen(int nConnectionBacklog)
    {
	    return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog)); 
    }

    BOOL CSjSocket::Accept(CSjSocket& rConnectedSocket, SOCKADDR* lpSockAddr, socklen_t* lpSockAddrLen)
    {
	    SOCKET hTemp = accept(m_hSocket, lpSockAddr, (socklen_t*)lpSockAddrLen);

	    if (hTemp == INVALID_SOCKET)
	    {
		    rConnectedSocket.m_hSocket = INVALID_SOCKET;
	    }
	    else
	    {
		    rConnectedSocket.m_hSocket = hTemp;
	    }

	    return (hTemp != INVALID_SOCKET);
    }

    BOOL CSjSocket::GetPeerName(CStringBuffer& rPeerAddress, UINT& rPeerPort)
    {
	    SOCKADDR_IN sockAddr;
	    memset(&sockAddr, 0, sizeof(sockAddr));

	    socklen_t nSockAddrLen = sizeof(sockAddr);

        BOOL bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	    if (bResult)
	    {
		    rPeerPort = ntohs(sockAddr.sin_port);
		    rPeerAddress = inet_ntoa(sockAddr.sin_addr);
	    }
	    return bResult;
    }

    BOOL CSjSocket::GetSockName(CStringBuffer& rSocketAddress, UINT& rSocketPort)
    {
	    SOCKADDR_IN sockAddr;
	    memset(&sockAddr, 0, sizeof(sockAddr));

	    socklen_t nSockAddrLen = sizeof(sockAddr);

	    BOOL bResult = GetSockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	    if (bResult)
	    {
		    rSocketPort = ntohs(sockAddr.sin_port);
		    rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	    }
	    return bResult;
    }    



