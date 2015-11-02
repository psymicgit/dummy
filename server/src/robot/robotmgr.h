///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmgr.h
//< @author: 洪坤安
//< @date:   2015年3月16日 11:43:59
//< @brief:  机器人中心
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robotmgr_h_
#define _robotmgr_h_

#include <net/netdefine.h>
#include <net/msgdispatcher.h>
#include <net/net.h>

#include <basic/taskqueue.h>
#include <basic/timerqueue.h>

#include "http/robothttpmgr.h"
#include "robot.h"
#include "robotconfig.h"

// 机器人管理器
class RobotMgr : public Singleton<RobotMgr>
{
	typedef tr1::unordered_map<uint32, Robot*> RobotMap;

public:
	RobotMgr()
		: m_allocRobotId(0)
		, m_run(false)
	{
	}

	virtual std::string name() { return string("robotmgr"); }

public:
	bool init(const char* jsonConfig);

	bool uninit();

	void start();

	void creareRobots();

	// 停止执行：将产生一个停止任务投到任务队列
	void stop();

	// 执行所有停止操作
	void stopping();

	// 每次游戏循环所执行的操作
	void run();

	void onRobotDisconnect(Robot*);

	Robot* createRobot();

private:
	uint32 allocRobotId() { return ++m_allocRobotId; }

public:

	TaskQueue				m_taskQueue;	// 任务队列
	TimerQueue				m_timers;		// 定时器队列
	bool					m_run;			// <是否运行中>标志位
	uint32					m_allocRobotId;	// 空闲的机器人id
	RobotMap				m_robotMap;		// 机器人列表
	MsgDispatcher<Robot>	m_dispatcher;	// 消息派发器
	Net						m_wan;			// 外网中心
	RobotHttpMgr			m_httpMgr;		// http操作中心
	RobotConfig				m_config;		// 机器人配置
};

#endif //_robotmgr_h_