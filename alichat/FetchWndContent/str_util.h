#ifndef STR_UTIL
#define STR_UTIL

namespace strutil
{
    // 把char*转换为wchar_t*, 返回的空间需手动删除
    wchar_t* char_to_wchar(const char*);

    // 把wchar_t*转换为char*, 返回的空间需手动删除
    char* wchar_to_char(wchar_t*);
}

#endif