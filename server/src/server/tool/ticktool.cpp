///<------------------------------------------------------------------------------
//< @file:   server\tool\ticktool.cpp
//< @author: 洪坤安
//< @date:   2015年3月27日 21:43:37
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "ticktool.h"
#include "timetool.h"

namespace ticktool
{
#ifdef WIN
	// 获取CPU每秒的滴答次数
	uint64 GetTickFrequency()
	{
		static LARGE_INTEGER static_perfFreq = {0};
		if(0 == static_perfFreq.QuadPart) {
			QueryPerformanceFrequency(&static_perfFreq);
		}

		return static_perfFreq.QuadPart;
	}

	uint64 tick()
	{
		LARGE_INTEGER tick;

		QueryPerformanceCounter(&tick);
		return tick.QuadPart;
	}

	// 返回两次时钟周期的秒差
	double tickDiff(uint64 old_tick)
	{
		uint64 tick_now	= tick();
		uint64 diff			= tick_now - old_tick;

		double s = (double)diff / GetTickFrequency();
		return s;
	}

#else
	uint64 tick()
	{
		return (uint64)timetool::getTimeOfDay();
	}

	// 返回相差秒
	double tickDiff(uint64 old_tick)
	{
		uint64 tick_now	= tick();
		uint64 msDiff		= tick_now - old_tick;

		double s = (double)msDiff / 1000;
		return s;
	}
#endif
}

void Tick::startTick()
{
	m_tick = ticktool::tick();
	++m_retickNum;
}


// 返回秒
double Tick::endTick()
{
	return ticktool::tickDiff(m_tick);
}

double Tick::life()
{
	double lifetime = ticktool::tickDiff(m_born);
	return lifetime;
}
