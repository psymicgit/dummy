///<------------------------------------------------------------------------------
//< @file:   server\tool\atomictool.cpp
//< @author: 洪坤安
//< @date:   2015年3月27日 15:15:18
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "atomictool.h"

namespace atomictool
{
	int cas(int *x, int y, int z)
	{
#if defined(WIN)
		return InterlockedCompareExchange((volatile LONG*)x, y, z);
#else
		return __sync_val_compare_and_swap(x, y, z);
#endif
	}

	int inc(volatile int *x)
	{
#if defined(WIN)
		return InterlockedIncrement((volatile LONG *)x);
#else
		return __sync_add_and_fetch(x, 1);
#endif
	}

	int dec(volatile int *x)
	{
#if defined(WIN)
		return InterlockedDecrement((volatile LONG *)x);
#else
		return __sync_sub_and_fetch(x, 1);
#endif
	}

	int add(volatile int *x, int y)
	{
#if defined(WIN)
		return InterlockedExchangeAdd((volatile LONG *)x, y);
#else
		return __sync_add_and_fetch(x, y);
#endif
	}

	int sub(volatile int *x, int y)
	{
#if defined(WIN)
		return InterlockedExchangeAdd((volatile LONG *)x, -y);
#else
		return __sync_sub_and_fetch(x, y);
#endif
	}
}