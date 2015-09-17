#include "util.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>
#include <stdio.h>
#include <atlconv.h>

namespace log
{
    bool log_or_not = true; // 是否打印

    void enable_log()
    {
        log_or_not = true;
    }

    void disable_log()
    {
        log_or_not = false;
    }
}

void setnonblocking(int sock)
{
    unsigned long ul = 1;
    int n = ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);                 //设置套接字非阻塞模式
    if (n == SOCKET_ERROR)
    {
        //设置套接字非阻塞模式，失败处理
        perror("fcntl(sock,GETFL)");
        return;
    }
}

int gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = (long)clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}

// 获取时间（「新纪元时间」Epoch，1970年1月1日凌晨零点零分零秒距今的毫秒数）
time_in_ms now()
{
    struct timeval time;
    gettimeofday(&time, NULL);

    return time.tv_sec * 1000 + time.tv_usec / 1000;
}


// 设置发送超时时间，使得send时能在超过一定时间得不到结果即返回
void set_socket_send_timeout(int sockfd, long tv_sec, long tv_usec)
{
    struct timeval timeout;             //超时时间
    timeout.tv_sec = tv_sec;
    timeout.tv_usec = tv_usec;

    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(struct timeval)); 
}

// 设置接收超时时间
void set_socket_recv_timeout(int sockfd, long tv_sec, long tv_usec)
{
    struct timeval timeout;             //超时时间
    timeout.tv_sec = tv_sec;
    timeout.tv_usec = tv_usec;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(struct timeval)); 
}


void print_host_ent(hostent *host_ent, const char *host_name)
{
    char **pptr;

    char str[32];

    /* 将主机的规范名打出来 */  
    LOG_TRACE("resolve url<%s> success, the official hostname = <%s>, ", host_name, host_ent->h_name);

    /* 主机可能有多个别名，将所有别名分别打出来 */
    for (pptr = host_ent->h_aliases; *pptr != NULL; pptr++)   
    {
        LOG_TRACE("alias:%s\n", *pptr);
    }

    /* 根据地址类型，将地址打出来 */ 
    switch(host_ent->h_addrtype)   
    {   
    case AF_INET:   
    case AF_INET6:   
        pptr = host_ent->h_addr_list;   

        /* 将得到的所有地址都打出来 */  
        for(;*pptr!=NULL;pptr++) 
        {
            printf("ip = <%s>\n", inet_ntop(host_ent->h_addrtype, *pptr, str, sizeof(str)));   
        }

        break;   

    default:   
        printf("unknown address type\n");   
        break;   
    }
}

// 解析服务器地址，支持127.0.0.1的ip格式和www.xxxxxx.com的url格式
// 失败返回unsigned long(-1)
unsigned long name_resolve(const char *host_name)  
{  
    struct in_addr addr;  

    if((addr.s_addr=inet_addr(host_name)) == INADDR_NONE ) 
    {  
        struct hostent *host_ent = gethostbyname(host_name);  
        if(host_ent==NULL)
        {
            LOG_ERR("resolve svr addr<%s> fail at gethostbyname!\n", host_name);
            return INADDR_NONE ;
        }

        memcpy((char *)&addr.s_addr, host_ent->h_addr, host_ent->h_length);  
        print_host_ent(host_ent, host_name);
    }

    return addr.s_addr;
}

void str_err(int err, char buf[], int size)
{
    USES_CONVERSION;
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,(void*)FORMAT_MESSAGE_FROM_STRING, err,0,(buf),size,0);   
}