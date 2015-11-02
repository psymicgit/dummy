///<------------------------------------------------------------------------------
//< @file:   server\tool\filetool.h
//< @author: 洪坤安
//< @date:   2014年11月20日 10:45:10
//< @brief:	 文件操作方法
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _filetool_h_
#define _filetool_h_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace filetool
{
	// 获取当前进程所处的绝对路径
	std::string GetAbsolutePath();

	// 指定路径是否存在，可为文件路径或者文件夹路径
	// 例如：path = "../../example"
	// 又如：path = "../../abc.xml"
	// 又如：path = "../../"
	bool exist(const std::string &path);

	// 以二进制方式读取整个文件，根据文件大小分配缓冲区，将文件内容存入缓冲区中并返回
	// 该缓冲区通过new分配，所以要手动delete[]
	char* open(const char *path);

	// 以字符方式读取整个文件，返回文本内容字符串（文件大时不适合用此方法,因为返回的string将被深拷贝）
	bool get_whole_file_str(const std::string &path, std::string &text);

	// 覆盖文件：如果文件内容不变，则不作操作，否则直接用新的文件内容替换掉原来的（这么做是希望当文件内容不变时，防止文件的时间戳发生变化）
	bool overwrite_file(const std::string &path, const std::string &text);
}

#endif //_filetool_h_