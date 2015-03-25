#ifndef _str_util_h_
#define _str_util_h_

#include <string>
#include <wtypes.h>

using std::string;

typedef _ULonglong uint64_t;

namespace strutil
{
    // 移除掉字符串开头和末尾处的空格和\t符号
    // 例如: trim("   start xxx end   ") = "start xxx end"
    string trim(char* str);
	string trim(const string& str);

    // 替换字符串，传入的字符串将被修改
    // 例如：replace("this is an expmple", "is", "") = "th  an expmple"
    // 又如: replace("dbdb", "db", "bd") = "bdbd"
    string& replace(string &str, const char *old, const char* to);

    // 循环替换，即每次字符串被替换后，再在替换过的字符串中进行替换
    // 例如: replace("dbdb", "db", "bd") = "bbbd"
    // 又如: replace("good", "o", "oo") = "将会导致死循环"
    string& cycle_replace(string &str, const char *old, const char* to);

    // 返回文件夹路径，返回结果末尾含/或\
    // 例如：get_dir(../../xxxx.txt) = ../../
    string get_dir(const string &path);

    // 移掉路径，只返回文件名称
    // 例如：strip_dir(../../xxxx.txt) = xxxx.txt
    string strip_dir(const string &path);

    // 移掉文件名后缀
    // 例如：strip_ext(xxxx.txt) = xxxx
    string strip_ext(const string &filename);

    // 获取当前时间字符串
    void time_str();

    // 从左数起直到第一个分隔符中间的字符串
    // 例如：trip_at("123_456_789", '_') = 123
    string trip_at(const string &str, char delimiter);

    // 从左数起直到最后一个分隔符的字符串
    // 例如：trip_to("123_456_789", '_') = 123_456
    string trip_to(const string &str, char delimiter);

    // 从右数起直到指定分隔符的字符串
    // 例如：r_trip_at("123_456_789", '_') = 789
    string r_trip_at(const string &str, char delimiter);

    // 将无符号64位整数转换为字符串
    // 例如：tostr(100123) = "100123"
    string tostr(uint64_t);
};

using namespace strutil;

#endif // _str_util_h_
