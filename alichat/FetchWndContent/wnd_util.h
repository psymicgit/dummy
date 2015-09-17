#ifndef WND_UTIL
#define WND_UTIL

#include <vector>
#include <string>

#include <Windows.h>

typedef std::vector<HWND> HWNDVec;
typedef std::vector<std::string> WndNameVec;

namespace wndutil
{
    HWNDVec FindTopWindowsByList(WndNameVec &wnd_name_vec);

    HWNDVec FindTopWindowsByName(std::string &wnd_name);
}

#endif