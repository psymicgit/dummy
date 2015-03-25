#include "str_util.h"

#include <time.h>

#define IS_SPACE(c) (c == ' ' || c == '\t')

namespace strutil
{
    // 移除掉字符串开头和末尾处的空格和\t符号
    // 例如: trim("   start xxx end   ") = "start xxx end"
    string trim(char* str)
    {
        if(NULL == str)
        {
            return "";
        }
        
        while(IS_SPACE(*str)){str++;}
        char *start = str;

        while(*str){str++;}
        char *end   = str;

        while((end != start) && IS_SPACE(*end)){--end;}
        *(end) = '\0';

        return start;
    }

    // 替换字符串
    // 例如：replace("this is an expmple", "is", "") = "th  an expmple"
    string& replace(string &str, const char *old, const char* to)
    {
        string::size_type pos = 0;
        int len_old = strlen(old);
        int len_new = strlen(to);

        while((pos = str.find(old, pos)) != string::npos){   
            str.replace(pos, len_old, to);   
            pos += len_new;
        }

        return str;   
    }

    // 循环替换，即每次字符串被替换后，再在替换过的字符串中进行替换
    // 例如: replace("dbdb", "db", "bd") = "bbbd"
    // 又如: replace("good", "o", "oo") = "将会导致死循环"
    string& cycle_replace(string &str, const char *old, const char* to)
    {
        string::size_type pos = 0;
        int len_old = strlen(old);

        while((pos = str.find(to)) != string::npos)
        {
            str.replace(pos, len_old, to);   
        }

        return str;  
    }

    // 返回文件夹路径
    // 例如：../../xxxx.txt -> ../../
    string get_dir(const string &path)
    {
        if(path.empty())
        {
            return path;
        }

        int i = path.size() - 1;
        for(; i >= 0; i--)
        {
            if('\\' == path[i] || '/' == path[i])
            {
                break;
            }
        }

        if(i < 0)
        {
            return "";
        }

        return string(path.begin(), path.begin() + i);
    }

    // 移掉路径，只返回文件名称
    // 例如：../../xxxx.txt -> xxxx.txt
    string strip_dir(const string &path)
    {
        if(path.empty())
        {
            return path;
        }

        int i = path.size();
        for(; i >= 0; i--)
        {
            if('\\' == path[i] || '/' == path[i])
            {
                break;
            }
        }

        return path.c_str() + (i + 1);
    }

    // 移掉文件名后缀
    // 例如：strip_ext(xxxx.txt) = xxxx
    string strip_ext(const string &filename)
    {
        return trip_to(filename, '.');
    }

    // 获取当前时间字符串
    void time_str()
    {
        time_t rawtime;
        struct tm* timeinfo = NULL;
        
        time(&rawtime);
        if(localtime_s(timeinfo, &rawtime))
        {
            return;
        }

        static char buf[1024];
        sprintf_s(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }

    // 从左数起直到指定分隔符的字符串
    // 例如：trip_at("123_456", '_') = 123
    string trip_at(const string &str, char delimiter)
    {
        string::size_type pos = str.find(delimiter);
        if(pos == string::npos)
        {
            return str;
        }

        return string(str.begin(), str.begin() + pos);
    }

    // 从左数起直到最后一个分隔符的字符串
    // 例如：trip_to("123_456_789", '_') = 123_456
    string trip_to(const string &str, char delimiter)
    {
        string::size_type pos = str.rfind(delimiter);
        if(pos == string::npos)
        {
            return str;
        }

        return string(str.begin(), str.begin() + pos);
    }

    // 从右数起直到指定分隔符的字符串
    // 例如：r_trip_at("123_456", '_') = 456
    string r_trip_at(const string &str, char delimiter)
    {
        string::size_type pos = str.rfind(delimiter);
        if(pos == string::npos)
        {
            return str;
        }

        return string(str.begin() + pos + 1, str.end());
    }

    // 将无符号64位整数转换为字符串
    // 例如：tostr(100123) = "100123"
    string tostr(uint64_t n)
    {
        char buf[64] = {0};
        errno_t err = _ui64toa_s (n, buf, sizeof(buf), 10);
        if(err)
        {
            return "";
        }

        return buf;
    }
}