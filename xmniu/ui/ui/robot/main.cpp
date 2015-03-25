
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "robot.h"
#include <map>
#include <process.h>

#pragma comment(lib,"ws2_32.lib")

/*
bool parse_arg(int argc, char **argv)
{
    if (argc < 5 || (argc % 2 == 0))
    {
        // 格式为：robot 连接数量 是否打印全部日志 服务器1地址 服务器1端口 服务器2地址 服务器2端口 .....
        LOG_ERR("your command is invalid, check if it is:\r\n");
        LOG_ERR("   <<< robot robot_num log_all_or_not server_url1 port1 [server_url2 port2] ...[server_url10 port10] >>>\r\n");

        LOG_ERR("\r\n");

        LOG_ERR("   for example: robot 10000 1 s0.9.game2.com.cn 843 s0.9.game2.com.cn 844\r\n");
        LOG_ERR("   for example: robot 999 0 127.0.0.1 10023 127.0.0.1 10024\r\n");

        return false;
    }

    if( (g_n_robot = atoi(argv[1])) < 0 )
    {
        LOG_ERR("robot number:<%s> is invalid\a\r\n",argv[3]);
        return false;
    }

    int log_all_arg = atoi(argv[2]);
    if(log_all_arg !=0 && log_all_arg != 1)
    {
        LOG_ERR("option log_all_or_not:<%s> is invalid, must be 0 or 1\r\n", argv[2]);
        return false;
    }

    g_xxx::g_log_all = (log_all_arg ? true : false);

    g_n_svr = 0;

    int svr_num_by_argc = (argc - 2) / 2;
    g_svrs = new svr_t[svr_num_by_argc];

    char *svr_addr = NULL;
    int port = 0;
    unsigned long cast_addr = 0;

    for(int i = 3; i < argc; i+=2)
    {
        svr_addr = argv[i];

        cast_addr = name_resolve(svr_addr);
        if(INADDR_NONE  == cast_addr)
        {
            LOG_ERR("parse %dth argument fail: <%s> is not a valid address\r\n", i, svr_addr);
            return false;
        }

        port = atoi(argv[i + 1]);
        if(port <= 0 )
        {
            LOG_ERR("parse %dth argument fail: <%s> is not a valid port\r\n", i, argv[i + 1]);
            return false;
        }

        svr_t &svr = g_svrs[g_n_svr++];
        svr.addr = svr_addr;
        svr.port = port;
        svr.cast_addr = cast_addr;
    }

    if(g_n_svr == 0)
    {
        LOG_ERR("error: can't find a valid server");
        return false;
    }

    // LOG_TRACE("\r\n");

    return true;
}

// 机器人
int main(int argc, char **argv)
{
    socket_startup();

    logutil::enable_log();

    if(false == parse_arg(argc, argv))
    {
        return 1;
    }

    LOG_TRACE("start to launch <%d> robot\r\n", g_n_robot);

    if(false == g_xxx::g_log_all)
    {
        logutil::disable_log();
    }

    robots_power_on();
    // LOG_TRACE("mission accomplished, the robot num is %d\r\n", g_n_robot);

    system("pause");
    return 0;
}
*/