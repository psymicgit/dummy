///<------------------------------------------------------------------------------
//< @file:   server\tool\ticktool.h
//< @author: 洪坤安
//< @date:   2015年3月27日 21:42:31
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _ticktool_h_
#define _ticktool_h_

#include "log/log.h"

namespace ticktool
{
	uint64 tick();

	// 返回两次时钟周期的秒差
	double tickDiff(uint64 old_tick);
}

class Tick
{
public:
	Tick(const char* msg)
		: m_msg(msg)
	{
		LOG_WARN << "------- start " << m_msg << std::endl;
		startTick();
	}

	~Tick()
	{
		LOG_WARN << "------ end " << m_msg << " , cost time = " << endTick() << std::endl;
	}

	// 开始计时
	void startTick();

	// 返回秒
	double endTick();

private:
	uint64 m_tick;
	std::string m_msg;
};


#endif //_ticktool_h_