#include "str_util.h"
#include <string.h>
#include <stdlib.h>

namespace strutil
{
    wchar_t* char_to_wchar(const char* char_str)
    {
        size_t len = strlen(char_str) + 1;
        size_t converted = 0;

        wchar_t *wchar_str = (wchar_t*)malloc( len * sizeof(wchar_t));
        mbstowcs_s(&converted, wchar_str, len, char_str, _TRUNCATE);

        return wchar_str;
    }

    char* wchar_to_char(wchar_t* wchar_str)
    {
        size_t len = wcslen(wchar_str) + 1;
        size_t converted = 0;

        char *char_str = (char*)malloc (len * sizeof(char));
        wcstombs_s(&converted, char_str, len, wchar_str, _TRUNCATE);

        return char_str;
    }
}