

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sjfc_def.h"
#include "Common.h"

#ifdef  WIN32
#pragma warning(disable:4786)
#include <Winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif


namespace SJFCToolkit
{
#ifndef WIN32
    /*
     * All Windows Sockets error constants are biased by WSABASEERR from
     * the "normal"
     */
    #define WSABASEERR              10000
    /*
     * Windows Sockets definitions of regular Microsoft C error constants
     */
    #define WSAEINTR                (WSABASEERR+4)
    #define WSAEBADF                (WSABASEERR+9)
    #define WSAEACCES               (WSABASEERR+13)
    #define WSAEFAULT               (WSABASEERR+14)
    #define WSAEINVAL               (WSABASEERR+22)
    #define WSAEMFILE               (WSABASEERR+24)

    /*
     * Windows Sockets definitions of regular Berkeley error constants
     */
    #define WSAEWOULDBLOCK          (WSABASEERR+35)
    #define WSAEINPROGRESS          (WSABASEERR+36)
    #define WSAEALREADY             (WSABASEERR+37)
    #define WSAENOTSOCK             (WSABASEERR+38)
    #define WSAEDESTADDRREQ         (WSABASEERR+39)
    #define WSAEMSGSIZE             (WSABASEERR+40)
    #define WSAEPROTOTYPE           (WSABASEERR+41)
    #define WSAENOPROTOOPT          (WSABASEERR+42)
    #define WSAEPROTONOSUPPORT      (WSABASEERR+43)
    #define WSAESOCKTNOSUPPORT      (WSABASEERR+44)
    #define WSAEOPNOTSUPP           (WSABASEERR+45)
    #define WSAEPFNOSUPPORT         (WSABASEERR+46)
    #define WSAEAFNOSUPPORT         (WSABASEERR+47)
    #define WSAEADDRINUSE           (WSABASEERR+48)
    #define WSAEADDRNOTAVAIL        (WSABASEERR+49)
    #define WSAENETDOWN             (WSABASEERR+50)
    #define WSAENETUNREACH          (WSABASEERR+51)
    #define WSAENETRESET            (WSABASEERR+52)
    #define WSAECONNABORTED         (WSABASEERR+53)
    #define WSAECONNRESET           (WSABASEERR+54)
    #define WSAENOBUFS              (WSABASEERR+55)
    #define WSAEISCONN              (WSABASEERR+56)
    #define WSAENOTCONN             (WSABASEERR+57)
    #define WSAESHUTDOWN            (WSABASEERR+58)
    #define WSAETOOMANYREFS         (WSABASEERR+59)
    #define WSAETIMEDOUT            (WSABASEERR+60)
    #define WSAECONNREFUSED         (WSABASEERR+61)
    #define WSAELOOP                (WSABASEERR+62)
    #define WSAENAMETOOLONG         (WSABASEERR+63)
    #define WSAEHOSTDOWN            (WSABASEERR+64)
    #define WSAEHOSTUNREACH         (WSABASEERR+65)
    #define WSAENOTEMPTY            (WSABASEERR+66)
    #define WSAEPROCLIM             (WSABASEERR+67)
    #define WSAEUSERS               (WSABASEERR+68)
    #define WSAEDQUOT               (WSABASEERR+69)
    #define WSAESTALE               (WSABASEERR+70)
    #define WSAEREMOTE              (WSABASEERR+71)

    /*
     * Extended Windows Sockets error constant definitions
     */
    #define WSASYSNOTREADY          (WSABASEERR+91)
    #define WSAVERNOTSUPPORTED      (WSABASEERR+92)
    #define WSANOTINITIALISED       (WSABASEERR+93)
    #define WSAEDISCON              (WSABASEERR+101)
    #define WSAENOMORE              (WSABASEERR+102)
    #define WSAECANCELLED           (WSABASEERR+103)
    #define WSAEINVALIDPROCTABLE    (WSABASEERR+104)
    #define WSAEINVALIDPROVIDER     (WSABASEERR+105)
    #define WSAEPROVIDERFAILEDINIT  (WSABASEERR+106)
    #define WSASYSCALLFAILURE       (WSABASEERR+107)
    #define WSASERVICE_NOT_FOUND    (WSABASEERR+108)
    #define WSATYPE_NOT_FOUND       (WSABASEERR+109)
    #define WSA_E_NO_MORE           (WSABASEERR+110)
    #define WSA_E_CANCELLED         (WSABASEERR+111)
    #define WSAEREFUSED             (WSABASEERR+112)

    /* Authoritative Answer: Host not found */
    //#define WSAHOST_NOT_FOUND       (WSABASEERR+1001)
    //#define HOST_NOT_FOUND          WSAHOST_NOT_FOUND

    /* Non-Authoritative: Host not found, or SERVERFAIL */
    //#define WSATRY_AGAIN            (WSABASEERR+1002)
    //#define TRY_AGAIN               WSATRY_AGAIN

    /* Non-recoverable errors, FORMERR, REFUSED, NOTIMP */
    //#define WSANO_RECOVERY          (WSABASEERR+1003)
    //#define NO_RECOVERY             WSANO_RECOVERY

    /* Valid name, no data record of requested type */
    //#define WSANO_DATA              (WSABASEERR+1004)
    //#define NO_DATA                 WSANO_DATA

    /* no address, look for MX record */
    //#define WSANO_ADDRESS           WSANO_DATA
    //#define NO_ADDRESS              WSANO_ADDRESS

    /*
     * Define QOS related error return codes
     *
     */
    #define  WSA_QOS_RECEIVERS               (WSABASEERR + 1005)
             /* at least one Reserve has arrived */
    #define  WSA_QOS_SENDERS                 (WSABASEERR + 1006)
             /* at least one Path has arrived */
    #define  WSA_QOS_NO_SENDERS              (WSABASEERR + 1007)  
             /* there are no senders */
    #define  WSA_QOS_NO_RECEIVERS            (WSABASEERR + 1008) 
             /* there are no receivers */
    #define  WSA_QOS_REQUEST_CONFIRMED       (WSABASEERR + 1009)
             /* Reserve has been confirmed */
    #define  WSA_QOS_ADMISSION_FAILURE       (WSABASEERR + 1010)
             /* error due to lack of resources */
    #define  WSA_QOS_POLICY_FAILURE          (WSABASEERR + 1011)
             /* rejected for administrative reasons - bad credentials */
    #define  WSA_QOS_BAD_STYLE               (WSABASEERR + 1012)
             /* unknown or conflicting style */
    #define  WSA_QOS_BAD_OBJECT              (WSABASEERR + 1013)
             /* problem with some part of the filterspec or providerspecific
              * buffer in general */
    #define  WSA_QOS_TRAFFIC_CTRL_ERROR      (WSABASEERR + 1014)
             /* problem with some part of the flowspec */
    #define  WSA_QOS_GENERIC_ERROR           (WSABASEERR + 1015) 
             /* general error */
#else // #ifdef WIN32
    #ifndef socklen_t
    #define socklen_t       int 
    #endif
#endif //#ifndef WIN32

    /*
     * Level number for (get/set)sockopt() to apply to socket itself.
     */
    #ifndef SOL_SOCKET
        #define SOL_SOCKET      0xffff          /* options for socket level */
    #endif


    //#ifdef  WIN32
    #define ip_ltostr(pwIP) ((char*)inet_ntoa(*(struct in_addr*)(unsigned long*)pwIP))
    //#else

    class SJFC_API CSjSocket  
    {
    public:
	    SOCKET  m_hSocket;
        long    m_lLastErr ;
        CString m_strLastErr ;
    //	SOCKADDR_IPX m_sockAddr;

    public:
	    CSjSocket();
	    virtual ~CSjSocket();
        BOOL    IsValidSocket() ;
	    void	Close() ;
        SOCKET  Attach(SOCKET hSocket);
        SOCKET  Detach();

	    BOOL	Bind(UINT nSocketPort, const char * lpszSocketAddress);
	    BOOL    Bind(const SOCKADDR* lpSockAddr, socklen_t nSockAddrLen);
	    BOOL	Create(UINT nSocketPort=0, int nType=SOCK_STREAM,const char* szIPAddress=NULL);
        BOOL    SetTimeout(UINT uTimeUSec,BOOL bRecvTimeout=TRUE);//microsecond timeout
        unsigned long CanRecvDataLen() ;
	    int		Receive(void* lpBuf, int nBufLen, int nFlags = 0);
        int		Recv(void* lpBuf, int nBufLen, int nFlags = 0);
	    int		ReceiveFrom(void* lpBuf, int nBufLen, string& rSocketAddress, UINT& rSocketPort, int nFlags=0 , bool bIsDataStream=false);
	    int		ReceiveFrom(void* lpBuf, int nBufLen, string& rSocketAddress, UINT& rSocketPort,int nTimeOut, int nFlags, bool bIsDataStream=false); 
        int     ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR& sockAddr, socklen_t& nSockAddrLen,int nFlags=0, bool bIsDataStream=false) ;
        int     RecvFrom(void* lpBuf, int nBufLen, struct sockaddr * lpsFrom, socklen_t * lpnFromLen, int nFlags=0);
        int		SendChunk(const void* lpBuf, int nBufLen, int nFlags = 0) ;
	    int		Send(const void* lpBuf, int nBufLen, int nFlags = 0) ;
	    int		SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, const char * lpszHostAddress = NULL, int nFlags = 0 );
        int	    SendTo( const void* lpBuf, int nBufLen, const struct sockaddr* pSockAddr,socklen_t nSockAddrLen, int nFlags = 0) ;
	    BOOL	Connect(const char * lpszHostAddress, UINT nHostPort,CString* lpSvrIp=NULL);
	    BOOL    Connect(const SOCKADDR* lpSockAddr, socklen_t nSockAddrLen);
	    BOOL	IOCtl(long lCommand, unsigned long* lpArgument);
	    BOOL	SetNonBlockingMode(int flag=TRUE);
	    BOOL	Listen(int nConnectionBacklog=5);
	    BOOL	Accept(CSjSocket& rConnectedSocket,SOCKADDR* lpSockAddr = NULL, socklen_t* lpSockAddrLen = NULL);
	    BOOL    GetPeerName(SOCKADDR* lpSockAddr, socklen_t* lpSockAddrLen);
	    BOOL    GetSockName(SOCKADDR* lpSockAddr, socklen_t* lpSockAddrLen);
	    BOOL    GetSockOpt(int nOptionName, void* lpOptionValue, socklen_t* lpOptionLen, int nLevel = SOL_SOCKET);

	    BOOL    GetPeerName(CStringBuffer& rPeerAddress, UINT& rPeerPort);
	    BOOL    GetSockName(CStringBuffer& rSocketAddress, UINT& rSocketPort);
	    BOOL    SetSockOpt(int nOptionName, const void* lpOptionValue, socklen_t nOptionLen, int nLevel = SOL_SOCKET);

    };

    #ifndef __AFXSOCK_H__
        typedef CSjSocket   CSocket ;
    #endif

    inline SOCKET  CSjSocket::Attach(SOCKET hSocket)
    {
        SOCKET hOldSocket = m_hSocket ;
        m_hSocket = hSocket ;
        return hOldSocket ;
    }

    inline SOCKET  CSjSocket::Detach()
    {
        SOCKET hOldSocket = m_hSocket ;
#ifdef  WIN32
        m_hSocket = INVALID_SOCKET ;
#else
        m_hSocket = -1 ;
#endif
        return hOldSocket ;
    }

    inline BOOL CSjSocket::IsValidSocket()
    {
    #ifdef  WIN32
        return (this && m_hSocket != INVALID_SOCKET) ;
    #else
	    return (this && m_hSocket >= 0) ;
    #endif
    }
    
    //设置接收或者发送的最长时间
    inline BOOL CSjSocket::SetTimeout(UINT uTimeUSec, BOOL bRecvTimeout)
    {
#ifdef WIN32
        if (uTimeUSec<1000) uTimeUSec=1000 ;//window是毫秒级
        uTimeUSec = uTimeUSec/1000 ;//window是毫秒级
        return SetSockOpt((bRecvTimeout)?SO_RCVTIMEO:SO_SNDTIMEO,&uTimeUSec,sizeof(uTimeUSec),SOL_SOCKET); 
#else
        timeval tv ;tv.tv_sec=uTimeUSec/1000000 ;tv.tv_usec=(uTimeUSec%1000000)/**1000000*/;
        return SetSockOpt((bRecvTimeout)?SO_RCVTIMEO:SO_SNDTIMEO,&tv,sizeof(tv),SOL_SOCKET); 
#endif
    }
    inline int CSjSocket::Recv(void* lpBuf, int nBufLen, int nFlags)
    {
        return recv(m_hSocket,(char*)lpBuf, nBufLen, nFlags) ;
    }

    inline int CSjSocket::RecvFrom(void* lpBuf, int nBufLen, struct sockaddr * lpsFrom, socklen_t * lpnFromLen, int nFlags)
    {
        return recvfrom(m_hSocket,(char*)lpBuf,nBufLen,nFlags,lpsFrom,lpnFromLen) ;
    }

    inline int CSjSocket::SendChunk(const void* lpBuf, int nBufLen, int nFlags) 
    {
        return send(m_hSocket, (const char*)lpBuf, nBufLen, nFlags);
    }

    inline int	CSjSocket::SendTo( const void* lpBuf, int nBufLen, const struct sockaddr* pSockAddr,socklen_t nSockAddrLen, int nFlags)
    {
	    return sendto(m_hSocket, (const char*)lpBuf, nBufLen, nFlags, (const struct sockaddr*)pSockAddr, nSockAddrLen);
    }

    inline BOOL CSjSocket::Bind(const SOCKADDR* lpSockAddr, socklen_t nSockAddrLen)
    {
        return (bind(m_hSocket, lpSockAddr,nSockAddrLen) == SOCKET_ERROR) ;
    }
    inline BOOL CSjSocket::Connect(const SOCKADDR* lpSockAddr, socklen_t nSockAddrLen)
    {
        return (connect(m_hSocket,lpSockAddr,nSockAddrLen) == SOCKET_ERROR) ;
    }

    inline BOOL CSjSocket::GetPeerName(SOCKADDR* lpSockAddr, socklen_t* lpSockAddrLen)
	{ 
        return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen)); 
    }
    inline BOOL CSjSocket::GetSockName(SOCKADDR* lpSockAddr, socklen_t* lpSockAddrLen)
    {
        return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen));
    }
    inline BOOL CSjSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, socklen_t nOptionLen, int nLevel)
	{
        return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, (socklen_t)nOptionLen)); 
    }
    inline BOOL CSjSocket::GetSockOpt(int nOptionName, void* lpOptionValue, socklen_t* lpOptionLen, int nLevel)
	{
        return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, lpOptionLen)); 
    }
};

