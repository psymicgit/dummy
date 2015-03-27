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
	int inc(volatile int *mem)
	{
#if defined(WIN)
		return InterlockedIncrement((volatile LONG *)mem);
#else
		return add(mem, 1);
#endif
	}

	int dec(volatile int *mem)
	{
#if defined(WIN)
		return InterlockedDecrement((volatile LONG *)mem);
#else
		unsigned char prev;
		asm volatile ("lock; decl %0; setnz %1"
		              : "=m" (*mem), "=qm" (prev)
		              : "m" (*mem)
		              : "memory");

		return prev;
#endif
	}

	int add(volatile int *mem, int val)
	{
#if defined(WIN)
		return InterlockedExchangeAdd((volatile LONG *)mem, val);
#else
		asm volatile ("lock; xaddl %0,%1"
		              : "=r" (val), "=m" (*mem)
		              : "0" (val), "m" (*mem)
		              : "memory", "cc");
		return val;
#endif
	}

	int sub(volatile int *mem, int val)
	{
#if defined(WIN)
		return InterlockedExchangeAdd((volatile LONG *)mem, -val);
#else
		asm volatile ("lock; subl %1, %0"
		              : /* no output */
		              : "m" (*(mem)), "r" (val)
		              : "memory", "cc");
		return *mem;
#endif
	}
}