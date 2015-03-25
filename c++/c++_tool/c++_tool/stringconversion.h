/********************************************************************
            Copyright (c) 2009, 欢乐连线工作室
                   All rights reserved
         
    创建日期：  2009年12月11日 11时36分
    文件名称：  StringConversion.h
    说    明：  字符串转换工具
    
    当前版本：  1.00
    作    者：  Conserlin
    概    述：  创建  [非線程安全,限主線程使用!!!!]

*********************************************************************/
#pragma once
#include <vector>
#include <string>
#include <sstream>
using std::vector;
using std::string;
using namespace std;

class StringConversion
{
public:
	StringConversion();
	~StringConversion();

// #ifdef __LINUX__
// 	// UTF8 转成 宽字节的Unicode
// 	static const wchar_t* UTF8ToUnicode(const char *buffer);
// 	// Unicode 转成 UTF8
// 	static const char* unicodeToUTF8(const wchar_t *buffer);
// #endif // __LINUX__

	// UTF8 转成 宽字节的Unicode
	static const wchar_t* UTF8ToUnicode(const char *buffer);
	// Unicode 转成 UTF8
	static const char* unicodeToUTF8(const wchar_t *buffer);
#ifdef WIN32
	// UTF8 转成 多字节
	static const char* UTF8ToANSI(const char *buffer);

	// Unicode 转成 多字节
	static const char* unicodeToANSI(const wchar_t *buffer);

	// 多字节 转成 UTF8
	static const char* ANSIToUTF8(const char *buffer);
	// 多字节 转成 Unicode
	static const wchar_t* ANSIToUnicode(const char *buffer);
#endif // WIN32

    static void stringToLower(char* pStr);
};