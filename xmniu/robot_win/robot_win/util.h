#ifndef util_h__
#define util_h__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>

#include "types.h"

#define oops(msg) { perror("oops:" msg); }

#undef min
#undef max

/*
#define min(x,y) ({ \
    __typeof(x) _x = (x); \
    __typeof(y) _y = (y); \
    (void) (&_x == &_y); \
    _x < _y ? _x : _y; })

#define max(x,y) ({ \
    __typeof(x) _x = (x); \
    __typeof(y) _y = (y); \
    (void) (&_x == &_y); \
    _x > _y ? _x : _y; })
*/

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

namespace log
{
    extern bool log_or_not; // 是否打印

    void enable_log();

    void disable_log();

#define LOG_TRACE(xxx, ...) if(log_or_not){fprintf(stdout, xxx,  __VA_ARGS__);}
#define LOG_ERR(xxx, ...) if(log_or_not){fprintf(stderr, xxx,  __VA_ARGS__);}
}

using namespace log;

void setnonblocking(int sock);

int gettimeofday(struct timeval *tp, void *tzp);

// 获取时间（「新纪元时间」Epoch，1970年1月1日凌晨零点零分零秒距今的毫秒数）
time_in_ms now();

// 设置发送超时时间，使得send时能在超过一定时间得不到结果即返回
void set_socket_send_timeout(int sockfd, long tv_sec, long tv_usec);

// 设置接收超时时间
void set_socket_recv_timeout(int sockfd, long tv_sec, long tv_usec);

void print_host_ent(hostent *host_ent, const char *host_name);

// 解析服务器地址，支持127.0.0.1的ip格式和www.xxxxxx.com的url格式
// 失败返回unsigned long(-1)
unsigned long name_resolve(const char *host_name);

void str_err(int err, char buf[], int size);

#endif // util_h__
