///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmgr.cpp
//< @author: 洪坤安
//< @date:   2015年3月16日 11:43:52
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "robotmgr.h"
#include "robot.h"
#include "robotmsghandler.h"

#include <log/log.h>

bool RobotMgr::init()
{
	logging::init("robot", "log_robot_");

	m_wan.init(4);

	for(int i = 0; i < 500; i++) {
		Robot *robot = createRobot();

		// LOG_INFO << "robot " << i << " start connecting to server";
		m_wan.connect("127.0.0.1", 20001, *robot);
	}

	RobotMsgHandler *robotMsgHandler = new RobotMsgHandler(&m_dispatcher);

	m_run = true;
	return true;
}

Robot* RobotMgr::createRobot()
{
	Robot *robot = new Robot;

	robot->m_taskQueue = &m_taskQueue;
	robot->m_robotMgr = this;
	robot->m_robotId = allocRobotId();

	m_robotMap[robot->m_robotId]  = robot;
	return robot;
}

void RobotMgr::start()
{
	m_wan.start();

	while(m_run) {
		run();
	}
}

void RobotMgr::stop()
{
	m_wan.stop();
	run();

	m_run = false;
}

void RobotMgr::run()
{
	m_taskQueue.run();
	sleep(10);
}

void RobotMgr::handleMsg(Robot &robot, int msgId, Buffer &buf, Timestamp receiveTime)
{
	m_dispatcher.dispatch(robot, msgId, buf.peek(), buf.readableBytes(), receiveTime);
}

void RobotMgr::onRobotDisconnect(Robot *robot)
{
	m_robotMap.erase(robot->m_robotId);
	delete robot;

	if (m_robotMap.empty()) {
		stop();
	}
}
