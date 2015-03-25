///<------------------------------------------------------------------------------
//< @file:   echoutil.h
//< @author: hongkunan
//< @date:   2013Äê12ÔÂ14ÈÕ 10:38:49
//< @brief:  
//< Copyright (c) 2013 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _echoutil_h_
#define _echoutil_h_

#include <string>

enum GTestColor {
    COLOR_DEFAULT,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW
};

void ColoredPrintf(GTestColor color, const char* fmt, ...);

#define ECHO_ERR(fmt, ...) ColoredPrintf(COLOR_RED, fmt"\n", __VA_ARGS__)
#define ECHO_OK(fmt, ...) ColoredPrintf(COLOR_GREEN, fmt"\n", __VA_ARGS__)
#define ECHO_WARN(fmt, ...) ColoredPrintf(COLOR_YELLOW, fmt"\n", __VA_ARGS__)
#define ECHO_INFO(fmt, ...) fprintf(stdout, fmt"\n", __VA_ARGS__);

namespace echoutil
{
    std::string getmsg(const char* fmt, ...);
}

using namespace echoutil;

#endif //_echoutil_h_