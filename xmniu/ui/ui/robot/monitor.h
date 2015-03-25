#ifndef monitor_h__
#define monitor_h__

#include <map>

#include "types.h"

class robot_t;
struct svr_t;

namespace chart
{
    struct robot_chart
    {
        int n_succ;
        int n_fail;

        typedef int error_type;
        typedef int occur_num;

        typedef std::map<error_type, occur_num> error_map_t;
        error_map_t error_map;
    };
}

using namespace chart;

class monitor_t
{
public:
    monitor_t();
    ~monitor_t();

    void start();

    void end();

    void print(const robot_t robots[], int n_robot, svr_t *svrs, int n_svr);

public:
    time_in_ms start_ms; // 本次机器人测试开始时间
    time_in_ms end_ms; // 所有机器人终止时的时间

    robot_chart *m_chart;
};

#endif // monitor_h__
