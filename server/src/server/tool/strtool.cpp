#include "strtool.h"

#include <rapidjson/internal/itoa.h>

namespace strtool
{
	std::string itoa(int32 n)
	{
		char buf[12];
		rapidjson::internal::i32toa(n, buf);
		return buf;
	}

	std::string itoa(int64 n)
	{
		char buf[12];
		rapidjson::internal::u32toa(n, buf);
		return buf;
	}

	std::string itoa(uint32 n)
	{
		char buf[30];
		rapidjson::internal::i64toa(n, buf);
		return buf;
	}

	std::string itoa(uint64 n)
	{
		char buf[30];
		rapidjson::internal::u64toa(n, buf);
		return buf;
	}
}

namespace strtool
{
#define IS_SPACE(c) (c == ' ' || c == '\t')

	// 移除掉字符串开头和末尾处的空格和\t符号
	// 例如: trim("   start xxx end   ") = "start xxx end"
	string trim(char* str)
	{
		if(NULL == str) {
			return "";
		}

		while(IS_SPACE(*str)) {str++;}
		char *start = str;

		while(*str) {str++;}
		char *end   = str;

		while((end != start) && IS_SPACE(*end)) {--end;}
		*(end) = '\0';

		return start;
	}

	// 移除掉字符串开头和末尾处的空格和\t符号
	// 例如: trim("   start xxx end   ") = "start xxx end"
	string trim(const string& str)
	{
		string::size_type pos = str.find_first_not_of(' ');
		if (pos == string::npos) {
			return str;
		}

		string::size_type pos2 = str.find_last_not_of(' ');
		if (pos2 != string::npos) {
			return str.substr(pos, pos2 - pos + 1);
		}

		return str.substr(pos);
	}

	// 替换字符串
	// 例如：replace("this is an expmple", "is", "") = "th  an expmple"
	string& replace(string &str, const char *old, const char* to)
	{
		string::size_type pos = 0;
		int len_old = strlen(old);
		int len_new = strlen(to);

		while((pos = str.find(old, pos)) != string::npos) {
			str.replace(pos, len_old, to);
			pos += len_new;
		}

		return str;
	}

	string& replace(string &str, const char *old, const string& to)
	{
		string::size_type pos = 0;
		int len_old = strlen(old);
		int len_new = to.size();

		while((pos = str.find(old, pos)) != string::npos) {
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

		while((pos = str.find(to)) != string::npos) {
			str.replace(pos, len_old, to);
		}

		return str;
	}

	// 返回文件夹路径
	// 例如：../../xxxx.txt -> ../../
	string get_dir(const string &path)
	{
		if(path.empty()) {
			return path;
		}

		int i = path.size() - 1;
		for(; i >= 0; i--) {
			if('\\' == path[i] || '/' == path[i]) {
				break;
			}
		}

		if(i < 0) {
			return "";
		}

		return string(path.begin(), path.begin() + i);
	}

	// 移掉路径，只返回文件名称
	// 例如：../../xxxx.txt -> xxxx.txt
	string strip_dir(const string &path)
	{
		if(path.empty()) {
			return path;
		}

		int i = path.size();
		for(; i >= 0; i--) {
			if('\\' == path[i] || '/' == path[i]) {
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

	// 从左数起直到指定分隔符的字符串
	// 例如：trip_at("123_456", '_') = 123
	string trip_at(const string &str, char delimiter)
	{
		string::size_type pos = str.find(delimiter);
		if(pos == string::npos) {
			return str;
		}

		return string(str.begin(), str.begin() + pos);
	}

	// 从左数起直到最后一个分隔符的字符串
	// 例如：trip_to("123_456_789", '_') = 123_456
	string trip_to(const string &str, char delimiter)
	{
		string::size_type pos = str.rfind(delimiter);
		if(pos == string::npos) {
			return str;
		}

		return string(str.begin(), str.begin() + pos);
	}

	// 从右数起直到指定分隔符的字符串
	// 例如：r_trip_at("123_456", '_') = 456
	string r_trip_at(const string &str, char delimiter)
	{
		string::size_type pos = str.rfind(delimiter);
		if(pos == string::npos) {
			return str;
		}

		return string(str.begin() + pos + 1, str.end());
	}

	stringvec_t split(const string &src, char cut /* = ',' */)
	{
		stringvec_t vec;

		std::string::size_type pos1 = 0, pos2 = 0;
		while (pos2 != std::string::npos) {
			pos1 = src.find_first_not_of(cut, pos2);
			if (pos1 == std::string::npos) {
				break;
			}

			pos2 = src.find_first_of(cut, pos1 + 1);
			if (pos2 == std::string::npos) {
				if (pos1 != src.size()) {
					vec.push_back(src.substr(pos1));
				}

				break;

			}

			vec.push_back(src.substr(pos1, pos2 - pos1));
		}

		return vec;
	}
}