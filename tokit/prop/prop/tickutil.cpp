///<------------------------------------------------------------------------------
//< @file:   tickutil.cpp
//< @author: hongkunan
//< @date:   2013年12月12日 11:46:36
//< @brief:  
//< Copyright (c) 2013 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "tickutil.h"

#include <windows.h>

namespace tickutil
{
    // 获取CPU每秒的滴答次数
    Tick GetTickFrequency()
    {
        static LARGE_INTEGER s_perfFreq = {0};
        if(s_perfFreq.QuadPart == 0)
        {
            QueryPerformanceFrequency(&s_perfFreq); 
        }

        return s_perfFreq.QuadPart;
    }

    Tick get_tick()
    {
        LARGE_INTEGER tick;
        
        QueryPerformanceCounter(&tick);
        return tick.QuadPart;
    }

    dword tick_diff(Tick pre_tick)
    {
        Tick tick_now = get_tick();
        
        dword ms = (dword)((tick_now - pre_tick) * 1000 / GetTickFrequency());
        return ms;
    }
}
