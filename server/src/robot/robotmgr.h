///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmgr.h
//< @author: 洪坤安
//< @date:   2015年3月16日 11:43:59
//< @brief:  机器人中心
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robotmgr_h_
#define _robotmgr_h_

#include <net/netreactor.h>
#include <basic/taskqueue.h>
#include <net/msgdispatcher.h>
#include <net/netfactory.h>

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
	// 任务队列
	TaskQueue m_taskQueue;

	// 定时器队列
	TimerQueue m_timers;

	// <是否运行中>标志位
	bool m_run;

	// 空闲的机器人id
	uint32 m_allocRobotId;

	// 机器人列表
	RobotMap m_robotMap;

	// 消息派发器
	MsgDispatcher<Robot> m_dispatcher;

	// 网络操作中心
	NetFactory m_wan;

	// http操作中心
	RobotHttpMgr m_httpMgr;

	// 机器人配置
	RobotConfig m_config;
};

#endif //_robotmgr_h_