///<------------------------------------------------------------------------------
//< @file:   server\tool\atomictool.h
//< @author: 洪坤安
//< @date:   2015年3月27日 15:15:8
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _atomictool_h_
#define _atomictool_h_

namespace atomictool
{
	// 32位原子加1算法，将原始值加1
	int inc(volatile int *mem);

	// 32位原子加1算法，将原始值加1
	int dec(volatile int *mem);

	// 32位原子加法
	int add(volatile int *mem, int val);

	// 32位原子减法
	int sub(volatile int *mem, int val);
}

#endif //_atomictool_h_