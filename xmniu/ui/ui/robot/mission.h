#ifndef mission_h__
#define mission_h__

struct svr_t;
class monitor_t;
class robot_t;

class mission_t
{
public:
    mission_t(int n_robot, bool log_all, svr_t *svrs, int n_svr);
    ~mission_t();

    void init();

    void start();

    void sum_up();

public:
    robot_t *m_robots;
    int m_n_robot; // 机器人数量

    svr_t *m_svrs;
    int m_n_svr;

    bool m_log_all;

    monitor_t *m_monitor;
};

#endif // mission_h__
