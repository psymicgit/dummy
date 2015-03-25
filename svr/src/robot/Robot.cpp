#include "Robot.h"

#include <assert.h>
#include <iostream>
#include <sys/types.h>
#include <io.h>

#include <event2\event.h>
#include <boost/cstdint.hpp>

#include <net/Endian.h>

#include "../packet/LoginReqMsg.pb.h"

#define oops(msg) { perror("oops:" msg); }

using namespace boost;

#define HEADER_LENGTH (2 * sizeof int16_t)
#define TAIL_LENGTH sizeof uint32_t

namespace protobufapi
{
    const int MOD_ADLER = 65521;

    uint32_t adler32(const unsigned char *data, int32_t len) /* where data is the location of the data in physical memory and 
                                                           len is the length of the data in bytes */
    {
        uint32_t a = 1, b = 0;
        int32_t index;
     
        /* Process each byte of the data in order */
        for (index = 0; index < len; ++index)
        {
            a = (a + data[index]) % MOD_ADLER;
            b = (b + a) % MOD_ADLER;
        }
     
        return (b << 16) | a;
    }
}

void Robot::Connect(const char addrStr[], int port)
{
#ifdef WIN32    //windows
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,0),&wsa) != 0){  
        std::cout << "Ì×½Ó×Ö³õÊ¼»¯Ê§°Ü!";
        return;
    }
#endif

    evutil_socket_t client_socket = socket(AF_INET, SOCK_STREAM, 0);  
    //assert(socket_fd > 0);
    if(client_socket == INVALID_SOCKET)
    {
        oops("socket initiating error...");
        return;
    }

    struct sockaddr_in svr_addr;  
    memset(&svr_addr, 0, sizeof(sockaddr));

    svr_addr.sin_family = AF_INET;  
    svr_addr.sin_addr.s_addr = inet_addr(addrStr);  
    svr_addr.sin_port =  htons(port); 

    int connect_result = connect(client_socket, (struct sockaddr*)&svr_addr, sizeof(svr_addr));  
    if(connect_result == SOCKET_ERROR)
    {
        oops("connect error...");
        printf("errno=%d\n", WSAGetLastError());
        return;
    }

    LoginReq loginReq;;
        loginReq.set_name("hello world");
        loginReq.set_pwd("123456");
        loginReq.set_id(11111);

    typedef ::google::protobuf::uint8 gluint8;
    gluint8 serializeBuf[256];

    uint16_t packetLen = loginReq.ByteSize() + TAIL_LENGTH;
    // packetLen = endian::hostToNetwork16(packetLen);
    memcpy(serializeBuf, &packetLen, sizeof int16_t);

    uint16_t msgType = 20;
    // msgType = endian::hostToNetwork16(msgType);
    memcpy(serializeBuf + sizeof(int16_t), (const void*)&msgType, sizeof int16_t);

    gluint8 *end = loginReq.SerializeWithCachedSizesToArray(serializeBuf + HEADER_LENGTH);

    uint32_t checksum = protobufapi::adler32(serializeBuf + HEADER_LENGTH, packetLen - TAIL_LENGTH);
    //checksum = endian::hostToNetwork32(checksum);
    memcpy(serializeBuf + HEADER_LENGTH + packetLen - TAIL_LENGTH, &checksum, sizeof uint32_t);

    size_t write_length = send(client_socket, (const char*)serializeBuf, HEADER_LENGTH + packetLen, 0);
    if(write_length != HEADER_LENGTH + packetLen)
    {
        oops("error: write ...");
        return;
    }

    /*
    char buffer[] = "hello world";
     
    //write to socket_fd  
    size_t write_length = send(client_socket, buffer, strlen(buffer) + 1, 0);
    if(write_length != strlen(buffer) + 1)
    {
        oops("error: write ...");
        return;
    }
    */

    char buffer[256] = {0};

    size_t read_length = recv(client_socket, buffer, sizeof(buffer), 0);  
    if(read_length == -1)
    {
        oops("error: read data from socket...");
        return;
    }


    std::cout << "The ret msg is: " << buffer << std::endl;

    closesocket(client_socket);  
    WSACleanup();
}
