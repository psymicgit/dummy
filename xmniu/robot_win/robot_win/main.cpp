
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

namespace g_xxx
{
#define MAX_SVR_NUM 10

    bool g_log_all = false;

    int g_n_robot = 1; // 机器人数量

    svr_t *g_svrs = NULL;
    int g_n_svr = 0;
};

using namespace g_xxx;

namespace monitoring
{
#define  MS_PER_SECOND 1000

    // 机器人测试状况监视
    struct robot_monitor
    {
        time_in_ms start_ms; // 本次机器人测试开始时间
        time_in_ms end_ms; // 所有机器人终止时的时间

        int n_robot; // 机器人数量
    };

    struct robot_chart
    {
        int n_succ;
        int n_fail;

        typedef int error_type;
        typedef int occur_num;

        typedef std::map<error_type, occur_num> error_map_t;
        error_map_t error_map;
    };

    // 获取时间（「新纪元时间」Epoch，1970年1月1日凌晨零点零分零秒距今的毫秒数）
    time_in_ms now()
    {
        struct timeval time;
        gettimeofday(&time, NULL);

        return time.tv_sec * 1000 + time.tv_usec / 1000;
    }

    void chart_init(robot_chart &chart)
    {
        chart.n_succ = 0;
        chart.n_fail = 0;
    }

    void chart_sum_up(robot_chart &chart, const robot_t *robot)
    {
        if(robot->error_no == 0)
        {
            chart.n_succ++;
        }
        else
        {
            chart.n_fail++;
            chart.error_map[robot->error_no]++;
        }
    }

    void print_robot_chart(robot_chart &chart)
    {
        typedef robot_chart::error_map_t error_map_t;

        LOG_TRACE("robot succ num = %d\n", chart.n_succ);
        LOG_TRACE("robot fail num = %d\n", chart.n_fail);


        for(error_map_t::iterator itr = chart.error_map.begin(); itr != chart.error_map.end(); itr++)
        {
            int err_type = itr->first;
            int err_occur_num = itr->second;

            LOG_TRACE("num of error<%d, %s> is %d\n", err_type, strerror(err_type), err_occur_num);
        }
    }

    double ms_to_s(double ms)
    {
        double s = ms / MS_PER_SECOND;
        return s;
    }

    void monitor_start(robot_monitor &monitor)
    {
        monitor.start_ms = now();
        monitor.n_robot = 0;
    }

    void monitor_end(robot_monitor &monitor)
    {
        monitor.end_ms = now();
    }

    void monitor_print(robot_monitor &monitor, const robot_t robots[], int n_robot)
    {
        robot_chart chart;
        chart_init(chart);

        int total_conn_time = 0;
        int total_send_recv_time = 0;
        int total_succ_robot_life = 0;

        int n_conn_succ = 0; // 连接成功的机器人数量
        int n_safebox_succ = 0; // 执行任务成功的机器人数量

#define MS_PER_MINUTE 60000

        int min_conn_time = MS_PER_MINUTE; // 连接速度最快的机器人花了多长时间连接
        int max_conn_time = 0;

        int min_send_recv_time = MS_PER_MINUTE; // 发送和接收数据的最快时间
        int max_send_recv_time = 0;

        int min_robot_life = MS_PER_MINUTE; // 最快完成任务的机器人花了多少时间
        int max_robot_life = 0;

        for(int i = 0; i < n_robot; i++)
        {
            // LOG_TRACE("<robot %d>", i);

            const robot_t *robot = &robots[i];
            if(NULL == robot)
            {
                LOG_TRACE("null");
                continue;
            }

            chart_sum_up(chart, robot);

            // LOG_TRACE("robot fd = %d, ", conn->fd);
            if(robot->error_no)
            {
                int err_time_diff = robot->error_occur_time - robot->start_conn_time;

                static char buf[128];
                str_err(robot->error_no, buf, sizeof(buf));

                LOG_TRACE("robot[idx=%d,fd=%d] failed after launched <%f> s, errormsg: <%d>%s\n", 
                    robot->idx, robot->sockfd, ms_to_s(err_time_diff), robot->error_no, buf);

                continue;
            }

            if(robot->conn_succ_time)
            {
                ++n_conn_succ;

                int conn_time = robot->conn_succ_time - robot->start_conn_time; // 连接耗时
                total_conn_time += conn_time;

                if(robot->finish_time)
                {
                    ++n_safebox_succ;

                    int robot_life = robot->finish_time - robot->start_conn_time; // 机器人生命长度
                    int send_recv_time = robot_life - conn_time; // 发送接收数据的耗时

                    total_succ_robot_life += robot_life;
                    total_send_recv_time += send_recv_time;

                    min_send_recv_time = min(send_recv_time, min_send_recv_time);
                    max_send_recv_time = max(send_recv_time, max_send_recv_time);

                    min_robot_life = min(robot_life, min_robot_life);
                    max_robot_life = max(robot_life, max_robot_life);
                }

                min_conn_time = min(conn_time, min_conn_time);
                max_conn_time = max(conn_time, max_conn_time);
            }
        }

        int total_cost_time = (int)(monitor.end_ms - monitor.start_ms); // 本次测试的总耗时

        double avg_conn_ms = (n_conn_succ == 0 ? 0 : (double)total_conn_time / n_conn_succ); // 平均每个机器人花了多少时间进行连接
        double avg_send_recv_ms = ((n_safebox_succ == 0) ? 0 : (double)total_send_recv_time / n_safebox_succ); // 平均数据传输时间
        double avg_robot_life = ((n_safebox_succ == 0) ? 0 : (double)total_succ_robot_life / n_safebox_succ); // 机器人的平均生命长度

        double conn_per_sec = ((n_conn_succ == 0) ? 0 : (double)n_conn_succ / ms_to_s(total_cost_time)); // 每秒多少连接
        double robots_per_sec = ((n_safebox_succ == 0) ? 0 :(double)n_safebox_succ / ms_to_s(total_cost_time)); // 每秒多少机器人

        log::enable_log();
        LOG_TRACE("/----------------------------------------\n");

        LOG_TRACE("total launched robot number = %d connect to %d svrs:\n", n_robot, g_xxx::g_n_svr);

        for(int i = 0; i < g_xxx::g_n_svr; i++)
        {
            svr_t *svr = &g_xxx::g_svrs[i];
            LOG_TRACE("    svrs[%d]:<%s:%d>\n", i + 1, svr->addr, svr->port);
        }

        LOG_TRACE("\n");

        print_robot_chart(chart);

        // LOG_TRACE("conn succ num = %d\n", n_conn_succ);
        // LOG_TRACE("safebox succ num = %d\n", n_safebox_succ);

        LOG_TRACE("\n");

        LOG_TRACE("total cost = %f s\n", ms_to_s(total_cost_time));

        LOG_TRACE("\n");

        LOG_TRACE("min conn time = %f s\n", ms_to_s(min_conn_time));
        LOG_TRACE("max conn time = %f s\n", ms_to_s(max_conn_time));

        LOG_TRACE("min send recv time = %f s\n", ms_to_s(min_send_recv_time));
        LOG_TRACE("max send recv time = %f s\n", ms_to_s(max_send_recv_time));

        LOG_TRACE("min robot life = %f s\n", ms_to_s(min_robot_life));
        LOG_TRACE("max robot life = %f s\n", ms_to_s(max_robot_life));

        LOG_TRACE("\n");

        LOG_TRACE("avg conn time = %f s\n", ms_to_s(avg_conn_ms));
        LOG_TRACE("avg send_recv time = %f s\n", ms_to_s(avg_send_recv_ms));
        LOG_TRACE("avg robot life = %f s\n", ms_to_s(avg_robot_life));

        LOG_TRACE("\n");

        LOG_TRACE("conns per second = %f\n", conn_per_sec);
        LOG_TRACE("robots per second = %f\n", robots_per_sec);//(double)MS_PER_SECOND / avg_robot_life);

        LOG_TRACE("----------------------------------------/\n");
    }
}


using namespace monitoring;

// 机器人任务线程的回调函数
void robot_thread_cb(void *un_used)
{
    robot_t *robot = (robot_t*)un_used;

    svr_t &svr = g_svrs[robot->idx % g_n_svr];
    robot->start(&svr);

    // return NULL;
}

void new_thread_robot(robot_t *robot)
{
    pthread_t tid = 0;
    HANDLE handle = (HANDLE)_beginthread(robot_thread_cb, 0, robot);
    // pthread_create(&tid, NULL, robot_thread_cb, robot); 

    robot->t_id = handle;
    // pthread_join(tid, NULL);
}

// 启动机器人
void robots_power_on()
{
    robot_monitor monitor;
    monitor_start(monitor);

    // 所有机器人执行任务的状况：开始时间、结束时间、失败原因等，对应存在这个数组里面
    robot_t *robots = new robot_t[g_n_robot];

    // 根据机器人数量，为每个机器人开启一个线程让其执行任务    
    for(int i = 0; i < g_n_robot; i++)
    {
        robot_t *robot = &robots[i];
        memset(robot, 0, sizeof(robot_t));

        robot->idx = i;
        new_thread_robot(robot); 
    }

    // 主线程等待所有机器人线程执行完毕
    for(int i = 0; i < g_n_robot; i++)
    {
        robot_t *robot = &robots[i];

        WaitForSingleObject(robot->t_id, INFINITE);
        // pthread_join(robot->t_id, NULL);
    }

    for(int i = 0; i < g_n_robot; i++)
    {
        robot_t *robot = &robots[i];
        //CloseHandle(robot->t_id);  
    }

    monitor_end(monitor);
    monitor_print(monitor, robots, g_n_robot);
}

bool parse_arg(int argc, char **argv)
{
    if (argc < 5 || (argc % 2 == 0))
    {
        // 格式为：robot 连接数量 是否打印全部日志 服务器1地址 服务器1端口 服务器2地址 服务器2端口 .....
        LOG_ERR("your command is invalid, check if it is:\n");
        LOG_ERR("   <<< robot robot_num log_all_or_not server_url1 port1 [server_url2 port2] ...[server_url10 port10] >>>\n");

        LOG_ERR("\n");

        LOG_ERR("   for example: robot 10000 1 s0.9.game2.com.cn 843 s0.9.game2.com.cn 844\n");
        LOG_ERR("   for example: robot 999 0 127.0.0.1 10023 127.0.0.1 10024\n");

        return false;
    }

    if( (g_n_robot = atoi(argv[1])) < 0 )
    {
        LOG_ERR("robot number:<%s> is invalid\a\n",argv[3]);
        return false;
    }

    int log_all_arg = atoi(argv[2]);
    if(log_all_arg !=0 && log_all_arg != 1)
    {
        LOG_ERR("option log_all_or_not:<%s> is invalid, must be 0 or 1\n", argv[2]);
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
            LOG_ERR("parse %dth argument fail: <%s> is not a valid address\n", i, svr_addr);
            return false;
        }

        port = atoi(argv[i + 1]);
        if(port <= 0 )
        {
            LOG_ERR("parse %dth argument fail: <%s> is not a valid port\n", i, argv[i + 1]);
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

    // LOG_TRACE("\n");

    return true;
}

void socketstartup()
{
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)) 
    { 
        printf("WSA failed/n"); 
        return; 
    } 
}

// 机器人
int main(int argc, char **argv)
{
    socketstartup();

    log::enable_log();

    if(false == parse_arg(argc, argv))
    {
        return 1;
    }

    LOG_TRACE("start to launch <%d> robot\n", g_n_robot);

    if(false == g_xxx::g_log_all)
    {
        log::disable_log();
    }

    robots_power_on();
    // LOG_TRACE("mission accomplished, the robot num is %d\n", g_n_robot);

    system("pause");
    return 0;
}
