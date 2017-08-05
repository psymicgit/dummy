#include "strtool.h"

#include <rapidjson/internal/itoa.h>

namespace strtool
{
	std::string itoa(int n)
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
	// 是否为空白字符
	inline bool is_blank(char c)
	{
		return (c == ' ' || c == '\t');
	}

	// 是否为斜杠符号：\或/
	inline bool is_slash(char c)
	{
		return (c == '\\' || c == '/');
	}

	inline bool is_empty(const char *str)
	{
		return (str == nullptr) || (str[0] == 0x00);
	}

	inline bool IsSameIgnoreCase(char a, char b)
	{
		return ::tolower(a) == ::tolower(b);
	}

	inline bool IsSameIgnoreCase(const std::string &a, const char *b)
	{
		return 0 == strnicmp(a.c_str(), b, a.size());
	}

	inline bool IsSameIgnoreCase(const char *a, const char *b)
	{
		return 0 == strnicmp(a, b, strlen(a));
	}


	// 移除掉字符串开头和末尾处的空格和\t符号
	// 例如: trim("   start xxx end   ") = "start xxx end"
	string trim(char* str)
	{
		if(NULL == str) {
			return "";
		}

		while(is_blank(*str)) {str++;}
		char *start = str;

		while(*str) {str++;}
		char *end   = str;

		while((end != start) && is_blank(*end)) {--end;}
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

	template <typename S /* 字符串类型 */, typename C /* 字符类型 */>
	S& replace(S &str, const C *old, int len_old, const C* to, int len_to)
	{
		S::size_type len_str = str.size();

		S out;
		out.reserve(2 * len_str);

		S::size_type pre_pos = 0;
		S::size_type find_pos = 0;

		while ((find_pos = str.find(old, pre_pos)) != S::npos)
		{
			out += str.substr(pre_pos, find_pos - pre_pos);
			out += to;

			pre_pos = find_pos + len_old;
		}

		if (pre_pos < len_str)
		{
			out += str.substr(pre_pos);
		}

		str = out;
		return str;
	}

	string& replace(string &str, const char *old, const char* to)
	{
		int len_old = strlen(old);
		int len_to = strlen(to);

		return replace(str, old, len_old, to, len_to);
	}

	wstring& wide_replace(wstring &str, const wchar_t *old, const wchar_t* to)
	{
		int len_old = wcslen(old);
		int len_to = wcslen(to);

		return replace(str, old, len_old, to, len_to);
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

	// 将字符串根据分隔符分割为字符串数组
	void split(const std::string &src, std::vector<std::string> &strvec, char cut /* = ',' */)
	{
		std::string::size_type pos1 = 0, pos2 = 0;
		while (pos2 != std::string::npos)
		{
			pos1 = src.find_first_not_of(cut, pos2);
			if (pos1 == std::string::npos)
			{
				break;
			}

			pos2 = src.find_first_of(cut, pos1 + 1);
			if (pos2 == std::string::npos)
			{
				if (pos1 != src.size())
				{
					strvec.push_back(src.substr(pos1));
				}

				break;
			}

			strvec.push_back(src.substr(pos1, pos2 - pos1));
		}
	}

	int ConvertInt(const char* str)
	{
		return atoi(str);
	}

	enum
	{
		MAX_ALPHABET = 256,	// 单字节字符个数
	};

	// sunday算法
	int sunday(const char *src, const char *des)
	{
		int i, pos = 0;
		int len_s, len_d;
		int alphabet[MAX_ALPHABET] = { 0 };

		if (src == NULL || des == NULL)
			return -1;

		len_s = strlen(src);
		len_d = strlen(des);

		for (i = 0; i < MAX_ALPHABET; i++)
			alphabet[i] = len_d;

		for (i = 0; i < len_d; i++)
			alphabet[des[i]] = len_d - i - 1;

		for (pos = 1; pos <= len_s - len_d; ) {
			for (i = pos - 1; i - pos + 1 < len_d; i++) {
				if (src[i] != des[i - pos + 1])
					break;
			}

			if ((i - pos + 1) == len_d)
				return pos;
			else
				pos += alphabet[src[pos + len_d - 1]] + 1;
		}

		return -1;
	}
	
	std::wstring s2ws(const std::string& s)
	{
		std::locale old_loc = std::locale::global(std::locale(""));

		const size_t buffer_size = s.size() + 1;
		wchar_t* dst_wstr = new wchar_t[buffer_size];
		wmemset(dst_wstr, 0, buffer_size);
		mbstowcs(dst_wstr, s.c_str(), buffer_size);
		std::wstring ws = dst_wstr;
		delete[]dst_wstr;

		std::locale::global(old_loc);
		return ws;
	}

	std::string ws2s(const std::wstring& ws)
	{
		std::locale old_loc =std::locale::global(std::locale(""));

		size_t buffer_size = ws.size() * 4 + 1;
		char* dst_str = new char[buffer_size];
		memset(dst_str, 0, buffer_size);
		wcstombs(dst_str, ws.c_str(), buffer_size);
		std::string s = dst_str;
		delete[]dst_str;

		std::locale::global(old_loc);
		return s;
	}
}