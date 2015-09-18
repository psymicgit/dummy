///<------------------------------------------------------------------------------
//< @file:   server\tool\filetool.cpp
//< @author: 洪坤安
//< @date:   2014年11月20日 10:49:56
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "filetool.h"

// #include <sys/stat.h>
#include <fstream>
#include <sstream>

#include "echotool.h"

#ifdef WIN
	#include <io.h>
	#define access _access
#else
	#include <unistd.h>
#endif

using namespace std;

namespace filetool
{
	bool exist(const std::string &path)
	{
		return access(path.c_str(), 0) != -1;
	}

	char* get_whole_file_buf(const std::string &path)
	{
		if(false == exist(path)) {
			return NULL;
		}

		/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */
		FILE *file = NULL;

#ifdef WIN
		int err = fopen_s(&file, path.c_str(), "rb" );
		if (NULL == file || err > 0) {
			LOG_SYSTEM_ERR << "打开<" << path << ">文件失败，错误号:" << err;
			return NULL;
		}
#else
		file = fopen(path.c_str(), "rb" );
		if (NULL == file) {
			LOG_SYSTEM_ERR << "打开<" << path << ">文件失败";
			return NULL;
		}
#endif

		/* 获取文件大小 */
		fseek (file , 0 , SEEK_END);
		long size = ftell (file);
		rewind (file);

		/* 分配内存存储整个文件 */
		char *buf = new char[size + 1];
		if (buf == NULL)        {
			LOG_SYSTEM_ERR << "载入<" << path << ">文件失败，错误原因:分配<" << size << ">的内存失败";
			return NULL;
		}

		/* 将文件拷贝到buffer中 */
		size_t ret = fread (buf, 1, size, file);
		if (ret != size) {
			LOG_SYSTEM_ERR << "载入<" << path << ">文件失败，错误原因:文件读取大小不符" << ret << " != " << size;
			delete[] buf;
			return NULL;
		}

		/* 现在整个文件已经在buffer中 */
		/* 关闭文件并释放内存 */
		fclose (file);

		buf[size] = '\0';
		return buf;
	}

	bool get_whole_file_str(const std::string &path, std::string &text)
	{
		if(false == exist(path)) {
			text = "";
			return false;
		}

		std::ifstream file(path.c_str());
		std::stringstream buf;
		buf << file.rdbuf();

		text = buf.str();
		return true;
	}

	bool overwrite_file(const std::string &path, const std::string &new_text)
	{
		string old_text;
		get_whole_file_str(path, old_text);
		if (old_text == new_text) {
			return true;
		}

		ofstream o(path);
		if (!o.good()) {
			LOG_SYSTEM_ERR << "无法生成文件，路径错误:" << path;
			return false;
		}

		o << new_text;
		o.flush();
		o.close();

		return true;
	}
}

namespace tool
{
	std::string GetAbsolutePath()
	{
#ifdef WIN
		char path[300];
		GetModuleFileName(NULL, path, sizeof(path));
		char *p = strrchr(path, '\\');
		*p = '\0';
		strcat_s(path, sizeof(path), "\\");

		return path;
#else
		return "";
#endif
	}

}
