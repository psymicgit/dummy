#include "wnd_util.h"
#include "str_util.h"

namespace wndutil
{
    HWNDVec FindTopWindowsByList(WndNameVec &wnd_name_vec)
    {
        HWNDVec wnd_vec;

        for(WndNameVec::iterator itr = wnd_name_vec.begin(); itr != wnd_name_vec.end(); ++itr)
        {
            std::string wnd_name = *itr;
            HWNDVec part_wnd_vec = FindTopWindowsByName(wnd_name);

            wnd_vec.insert(wnd_vec.end(), part_wnd_vec.begin(), part_wnd_vec.end());
        }

        // 这里不考虑效率了，直接用值传递
        return wnd_vec;
    }

    HWNDVec FindTopWindowsByName(std::string &wnd_class_name)
    {
        HWNDVec wnd_vec;
        wchar_t *wchar_name = strutil::char_to_wchar(wnd_class_name.c_str());

        HWND hFind = FindWindowEx(NULL, NULL, wchar_name, NULL);
        while(hFind)
        {
            wnd_vec.push_back(hFind);

            hFind = FindWindowEx(NULL, hFind, wchar_name, NULL);
        }

        delete wchar_name;

        return wnd_vec;
    }
}
