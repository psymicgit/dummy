#include "robot.h"

#include <stdio.h>
#include "util.h"

void robot_t::start(svr_t *svr)
{
    start_conn_time = now();

    int robot_socket = robot_connect(svr);
    if(robot_socket < 0)
    {
        err_cache();

        char   buf[128];
        str_err(error_no, buf, sizeof(buf));

        LOG_ERR("robot<idx=%d> connect server<%s:%d> fail! <error=%d>'%s'\n", idx, svr->addr, svr->port, error_no, buf);
        return;
    }

    sockfd = robot_socket;
    conn_succ_time = now();

    LOG_TRACE("robot<idx=%d, fd=%d> connect to server<%s:%d>success\n", idx, sockfd, svr->addr, svr->port);

    static const char buf[] = "<?xml version=\"1.0\"?>*****************";
    int len = strlen(buf) + 1;

    bool succ = robot_send(buf, len);
    if(false == succ)
    {
        err_cache();

        char   buff[128];
        str_err(error_no, buff, sizeof(buf));

        LOG_ERR("robot<idx=%d, fd=%d> send data to server<%s:%d> fail! <error=%d>'%s'\n", idx, sockfd, svr->addr, svr->port, error_no, buf);
        return;
    }

    char recv_buf[256] = {0};
    ssize_t n_recv = 0;

    succ = robot_recv(recv_buf, sizeof(recv_buf), n_recv);
    if(false == succ)
    {
        err_cache();

        char   buf[128];
        str_err(error_no, buf, sizeof(buf));

        LOG_ERR("robot<idx=%d, fd=%d> recv msg from svr<%s:%d> fail! <error=%d>'%s'\n", idx, sockfd, svr->addr, svr->port, error_no, buf);
        return;
    }

    // 安全沙箱策略数据
    static const char* expected_data = 
        "<?xml version=\"1.0\"?>"
        "<!DOCTYPE cross-domain-policy SYSTEM \"http://www.macromedia.com/xml/dtds/cross-domain-policy.dtd\">"
        "<cross-domain-policy>"
        "	<allow-access-from domain=\"*\" to-ports=\"*\" />"
        "</cross-domain-policy>";

    // 校验匹配
    ssize_t expected_len = strlen(expected_data) + 1;
    if(strncmp(recv_buf, expected_data, strlen(expected_data)) || n_recv != expected_len)
    {
        LOG_ERR("recv data err, unexpected data:%s, expecting len is %zd, recv len is %zd, robot<idx=%d, fd=%d>\n", 
            recv_buf, expected_len, n_recv, idx, sockfd);
        LOG_ERR("             expecting data is:%s,\n", expected_data);
    }

    // LOG_TRACE("robot <%d> life is %d ms\n", i, (int)(end_ms - start_ms));

    end();
}

void robot_t::end()
{
    if(0 == error_no)
    {
        finish_time = now();    
    }

    closesocket(sockfd);
}

int robot_t::robot_connect(svr_t *svr)
{
    int robot_socket = socket(AF_INET, SOCK_STREAM, 0);  
    if(robot_socket < 0)
    {
        oops("socket initiating error...");
        return -1;
    }

    struct sockaddr_in svr_addr;  
    memset(&svr_addr, 0, sizeof(sockaddr));

    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = svr->cast_addr;  
    svr_addr.sin_port =  htons(svr->port); 

    int connect_result = connect(robot_socket, (struct sockaddr*)&svr_addr, sizeof(svr_addr));  
    if(connect_result < 0)
    {
        // oops("connect error...");
        return -1;
    }

    // setnonblocking(client_socket);
    return robot_socket;
}

bool robot_t::robot_send(const char buf[], int n)
{
    set_socket_send_timeout(sockfd, 5, 0);

    ssize_t write_length = send(sockfd, buf, n, 0);
    if(write_length < n)
    {
        // oops("error: write ...");
        // close(client_socket);

        return false;
    }

    return true;
}

bool robot_t::robot_recv(char buf[], int buf_len, ssize_t &n_recv)
{
    set_socket_recv_timeout(sockfd, 5, 0);

    n_recv = recv(sockfd, buf, buf_len, MSG_WAITALL);  
    if(n_recv == -1)
    {
        // LOG_TRACE("robot<idx=%d, fd=%d> recv msg fail! error code is <%d>, error msg is <'%s'>\n", robot->idx, robot->sockfd, errno, strerror(errno));

        // oops("error: read data from socket...");
        return false;
    }

    buf[n_recv] = '\0';

    return true;
}

// 记载任务失败原因
void robot_t::err_cache()
{
    error_no = WSAGetLastError();
    error_occur_time = now();
}