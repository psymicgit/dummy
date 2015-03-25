///<------------------------------------------------------------------------------
//< @file:   tickutil.h
//< @author: hongkunan
//< @date:   2013年12月12日 11:46:18
//< @brief:  
//< Copyright (c) 2013 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _tickutil_h_
#define _tickutil_h_

#include "type.h"

typedef __int64 Tick;
namespace tickutil
{
    Tick get_tick();

    // 返回两次时钟周期的毫秒差
    dword tick_diff(Tick pre_tick);
}

#endif //_tickutil_h_