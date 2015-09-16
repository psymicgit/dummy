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

	// 停止执行：将产生一个停止任务投到任务队列
	void stop();

	// 执行所有停止操作
	void stopping();

	// 每次游戏循环所执行的操作
	void run();

	void onRobotDisconnect(Robot*);

	void handleMsg(Robot *robot, int msgId, Buffer&, Timestamp receiveTime);

	Robot* createRobot();

private:
	uint32 allocRobotId() { return ++m_allocRobotId; }

public:
	TaskQueue m_taskQueue;
	bool m_run;

	uint32 m_allocRobotId;
	RobotMap m_robotMap;

	MsgDispatcher<Robot> m_dispatcher;
	NetFactory m_wan;

	RobotHttpMgr m_httpMgr;

	RobotConfig m_config;
};

#endif //_robotmgr_h_