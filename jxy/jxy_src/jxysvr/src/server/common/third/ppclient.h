// CPP.h: interface for the PP class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __PPCLIENT_H__
#define __PPCLIENT_H__


#include <vector>
#include <string.h>
#include <assert.h>

#if defined(WIN32) || defined(WIN64)
#include <winsock2.h>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

typedef unsigned short WORD;
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct hostent* LPHOSTENT;
typedef struct servent* LPSERVENT;
typedef struct in_addr* LPIN_ADDR;
typedef struct sockaddr* LPSOCKADDR;
#endif


#define TIME_IN_SEC		3		// how long client will wait for server response in non-blocking mode
#define BUFFER_SIZE		1024		// SendData and RecvData buffers sizes
#define PP_IP				"58.218.147.103"
#define PP_PORT			8080


#pragma pack(push)
#pragma pack(1)

struct RECVDATA
{
    UINT32 len;
    UINT32 command;

    UINT32 GetLen()
    {
        return len;
        return ntohl(len);
    }

    UINT32 GetCommand()
    {
        return command;
        return ntohl(command);
    }
};

struct RECVUCLOGINDATA
{
    UINT32 len;
    UINT32 command;
    char token_key[16];

    void SetLen(UINT32 _len)
    {
        len = (_len);
        //len = htonl(_len);
    }

    void SetCommand(UINT32 _command)
    {
        command = (_command);
        //command = htonl(_command);
    }

    void SetTokenKey(char _token_key[16])
    {
        memcpy(token_key, _token_key, 16);
    }
};

struct RECVPPPSDATA
{
    UINT32	len;
    UINT32	command;
    UINT32	status;
    char		username[1];
    UINT32 GetLen()
    {
        return len;
        return ntohl(len);
    }

    UINT32 GetCommand()
    {
        return command;
        return ntohl(command);
    }
};

struct RESPONSEUCDATA
{
    UINT32 len;
    UINT32 command;
    UINT32 status;
    UINT32 GetLen()
    {
        return len;
        return ntohl(len);
    }

    UINT32 GetCommand()
    {
        return command;
        return ntohl(command);
    }
};
#pragma pack(pop)


class ECPP
{
public:
    enum CPPError
    {
        WSA_STARTUP = 100, // WSAGetLastError()
        WSA_VER,
        WSA_SEND,
        WSA_RECV,
        WSA_CONNECT,
        WSA_GETHOSTBY_NAME_ADDR,
        WSA_INVALID_SOCKET,
        WSA_HOSTNAME,
        WSA_IOCTLSOCKET,
        WSA_SELECT,
        SERVER_NOT_READY, // remote server
        SERVER_NOT_RESPONDING,
        SELECT_TIMEOUT,
        LACK_OF_MEMORY,
        RECVBUF_IS_EMPTY,
        SENDBUF_IS_EMPTY,
    };
    ECPP(CPPError err_) : ErrorCode(err_) {}
    CPPError		GetErrorNum(void) const
    {
        return ErrorCode;
    }
    std::string	GetErrorText(void) const;

private:
    CPPError ErrorCode;
};

class CPPClient
{
public:
    CPPClient();
    virtual ~CPPClient();
    const char* GetLocalHostIP() const;
    const char* GetLocalHostName() const;
    bool		Do(char token_key[16], char szUserName[64], UINT64 &qwUserID);
    void		SetPPServer(const char* server, const unsigned short port = 0);

private:
    SOCKET	ConnectRemoteServer(const char* server, const unsigned short port = 0);
    void		SendData(const char szSendBuf[BUFFER_SIZE], const int nLen);
    void		ReceiveData();
private:

    std::string m_sLocalHostName;
    std::string m_sIPAddr;
    std::string m_sPPSrvName;
    unsigned short m_iPPSrvPort;
    int			nLen;
    char			szRecvBuf[BUFFER_SIZE];
    char			szSendBuf[BUFFER_SIZE];
    SOCKET		hSocket;



};


#endif // __CPP_H__
