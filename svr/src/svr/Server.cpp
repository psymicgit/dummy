#include "Server.h"

#include <stdlib.h> 
#include <iostream> 
#include <assert.h>

#include <event2\event.h>
#include <event2\bufferevent.h>

#include <WS2tcpip.h>

#include <common\Platform.h>

#define oops(msg) { perror("oops: "msg); }

namespace
{
    void read_cb(struct bufferevent *incoming, void *arg) {  
        #define MAX_LINE    256

        char line[MAX_LINE+1];  
        int n;  
        evutil_socket_t client_socket = bufferevent_getfd(incoming);  

        while (n = bufferevent_read(incoming, line, MAX_LINE), n > 0) {  
            line[n] = '\0';  
            printf("read line from <fd=%u>: %s\n", client_socket, line);  

            bufferevent_write(incoming, line, n);  
        }  
    }

    void write_cb(struct bufferevent *bev, void *arg) {}

    void error_cb(struct bufferevent *bev, short event, void *arg) 
    {
        evutil_socket_t fd = bufferevent_getfd(bev);  

        if (event & BEV_EVENT_TIMEOUT) 
        {
            printf("connection<fd = %u> timed out\n", fd); //if bufferevent_set_timeouts() called  
        }  
        else if (event & BEV_EVENT_EOF) 
        {
            std::cout << "connection <fd = " << fd << "> closed" << std::endl;  
        }  
        else if (event & BEV_EVENT_ERROR) 
        {
            printf("connection<fd = %u> has some other error\n", fd);  
        }

        bufferevent_free(bev);  
    }  

    void do_accept_cb(evutil_socket_t svr_socket, short event_type, void *arg)
    {
        struct sockaddr_in client_addr;  
        socklen_t slen = sizeof(sockaddr);

        evutil_socket_t client_socket = accept(svr_socket, (struct sockaddr *)&client_addr, &slen);  
        if (INVALID_SOCKET == client_socket)
        {
            oops("ERROR: accept client fail: ");
            printf("errno=%d\n", WSAGetLastError());
            return;
        }

        /*
        if (client_socket > FD_SETSIZE)
        {
            oops("ERROR: fd > FD_SETSIZE");
            return;
        }
        */

        cout<<"Accept connection from "<< inet_ntoa(client_addr.sin_addr) << endl;

        struct event_base *base = (struct event_base *)arg;  
        struct bufferevent *bev = bufferevent_socket_new(base, client_socket, BEV_OPT_CLOSE_ON_FREE);  

        bufferevent_setcb(bev, read_cb, NULL, error_cb, arg);  
        bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);  
    }  
}

void Server::Init()
{

#ifdef WIN32    //windows
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){  
        std::cout << "套接字初始化失败!";
        return;
    }
#endif

// listen_socket初始化
    evutil_socket_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // assert(listen_fd > 0);
    if(listen_fd == INVALID_SOCKET)
    {
        oops("int listen_fd = socket(AF_INET, SOCK_STREAM, 0);");
        printf("errno=%d\n", WSAGetLastError());
        return;
    }

    evutil_make_listen_socket_reuseable(listen_fd);

// listen_addr初始化
    int port = 10000; //socket监听端口

    struct sockaddr_in listen_addr;

    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    listen_addr.sin_port = htons(port);

    int bind_return = bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr));
    // assert(bind_return > 0);
    if(bind_return == -1)
    {
        oops("ERROR: bind fail:");
        printf("errno=%d\n",errno);
        return;
    }

    const int listen_backlog = 1024;
    int listen_return = listen(listen_fd, listen_backlog);
    assert(listen_return != -1);
    if(listen_return == -1)
    {
        oops("ERROR: listen:");
        return;
    }

    cout<<"Server "<< inet_ntoa(listen_addr.sin_addr) <<" : "<< port<<" is listening......"<<endl;  

    // 将描述符设置为非阻塞
    int nonblock_return = evutil_make_socket_nonblocking(listen_fd);
    assert(nonblock_return != -1);
    
    //int flags = fcntl(listen_fd,, F_GETFL);
    //flags |= O_NONBLOCK;

    //fcntl(listen_fd, F_SETFL, flags);

    struct event_base *base = event_base_new();  
    assert(base != NULL);
    if(base == NULL)
    {
        oops("ERROR: event_base_new()");
    }


    struct event *listen_event = event_new(base, listen_fd, EV_READ|EV_PERSIST, do_accept_cb, (void*)base);
    event_base_set(base, listen_event);
    event_add(listen_event, NULL); ///< NULL表示无超时设置>

    event_base_dispatch(base);

    std::cout << "End...";
}

void Server::StartUp()
{
    
}

void Server::Terminate()
{

}