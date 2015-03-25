#include "robot.h"

#include <stdio.h>
#include "util.h"

#include "RobotForm.h"

namespace robot_mission
{
}

using namespace robot_mission;

void robot_t::go(svr_t *svr)
{
    m_svr = svr;

    if(false == robot_connect(svr))
    {
        return;
    }

    char request[1024];
    sprintf(request, "GET / HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n"
                     "Host: %s\r\nConnection: Keep-Alive\r\n\r\n", svr->addr);

    static const char data[] = "<?xml version=\"1.0\"?>*****************";
    int len = strlen(data) + 1;

    if(false == robot_send(request, strlen(request) + 1))
    {
        return;
    }


    char recv_buf[2048] = {0};
    ssize_t n_recv = 0;

    if(false == robot_recv(recv_buf, sizeof(recv_buf), n_recv))
    {
        return;
    }

    LOG_TRACE("robot<idx=%d, fd=%d> recv data:<%s>, recv len = %d\r\n\r\n", m_idx, m_sockfd, recv_buf, n_recv);

    /*
    if(false == robot_recv(recv_buf, sizeof(recv_buf), n_recv))
    {
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
        LOG_ERR("recv data err, unexpected data:%s, expecting len is %d, recv len is %d, robot<idx=%d, fd=%d>\r\n\r\n", 
            recv_buf, expected_len, n_recv, m_idx, m_sockfd);
        LOG_ERR("             expecting data is:%s,\r\n\r\n", expected_data);
    }

    // LOG_TRACE("robot <%d> life is %d ms\r\n", i, (int)(end_ms - start_ms));

    */
    end();
}

void robot_t::end()
{
    if(0 == m_error_no)
    {
        m_finish_time = now();    
    }

    closesocket(m_sockfd);
}

int robot_t::robot_connect(svr_t *svr)
{
    m_start_conn_time = now();

    int robot_socket = socket(AF_INET, SOCK_STREAM, 0);  
    if(robot_socket < 0)
    {
        oops("socket initiating error...");
        on_connect_fail(svr);
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
        on_connect_fail(svr);
        return -1;
    }

    // setnonblocking(client_socket);
    set_socket_send_timeout(robot_socket, 5, 0);
    set_socket_recv_timeout(robot_socket, 5, 0);

    m_sockfd = robot_socket;
    m_conn_succ_time = now();

    LOG_TRACE("robot<idx=%d, fd=%d> connect to server<%s:%d>success\r\n\r\n", m_idx, m_sockfd, svr->addr, svr->port);

    return robot_socket;
}

void robot_t::on_connect_fail(svr_t *m_svr)
{
    char buf[128] = {0};

    err_cache();
    str_err(m_error_no, buf, sizeof(buf));

    LOG_ERR("robot<idx=%d> connect server<%s:%d> fail! <error=%d>'%s'\r\n\r\n", m_idx, m_svr->addr, m_svr->port, m_error_no, buf);
}

bool robot_t::robot_send(const char buf[], int n)
{
    ssize_t write_length = send(m_sockfd, buf, n, 0);
    if(write_length < n)
    {
        // oops("error: write ...");
        // close(client_socket);
        on_send_fail(buf, n);
        return false;
    }
    
    return true;
}

void robot_t::on_send_fail(const char data[], int n)
{
    char err_buf[128] = {0};

    err_cache();
    str_err(m_error_no, err_buf, sizeof(err_buf));

    LOG_ERR("robot<idx=%d, fd=%d> send data<%s> to server<%s:%d> fail! <error=%d>'%s'\r\n\r\n", m_idx, m_sockfd, data, m_svr->addr, m_svr->port, m_error_no, err_buf);
}


bool robot_t::robot_recv(char buf[], int buf_len, ssize_t &n_recv)
{
    n_recv = recv(m_sockfd, buf, buf_len, MSG_WAITALL);  
    if(n_recv == -1)
    {
        // LOG_TRACE("robot<idx=%d, fd=%d> recv msg fail! error code is <%d>, error msg is <'%s'>\r\n", robot->idx, robot->sockfd, errno, strerror(errno));

        // oops("error: read data from socket...");
        on_recv_fail();
        return false;
    }

    buf[n_recv] = '\0';

    return true;
}

void robot_t::on_recv_fail()
{
    char err_buf[128] = {0};
    
    err_cache();
    str_err(m_error_no, err_buf, sizeof(err_buf));

    LOG_ERR("robot<idx=%d, fd=%d> recv msg from svr<%s:%d> fail! <error=%d>'%s'\r\n\r\n", m_idx, m_sockfd, m_svr->addr, m_svr->port, m_error_no, err_buf);
}

// 记载任务失败原因
void robot_t::err_cache()
{
    m_error_no = WSAGetLastError();
    if(m_error_no == 0)
    {
        return;
    }

    m_error_occur_time = now();

    end();
}