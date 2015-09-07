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

namespace tool
{
	// 获取当前进程所处的绝对路径
	std::string GetAbsolutePath();
}

#endif //_filetool_h_