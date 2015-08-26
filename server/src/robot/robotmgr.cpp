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
	global::init();
	logging::init("robot", "log_robot_");

	LOG_WARN << "starting robotmgr ...";

	m_wan.init(4);

	if (!m_httpMgr.init()) {
		LOG_ERROR << "robothttpmgr init failed, aborted";
		return false;
	}

	for(int i = 0; i < 10; i++) {
		Robot *robot = createRobot();
		// robot->start();

		// LOG_INFO << "robot " << i << " start connecting to server";
		m_wan.connect("127.0.0.1", 20001, *robot, "gateserver");
		// m_wan.connect("127.0.0.1", 80, *robot);
	}

	m_dispatcher.addMsgHandler(new RobotMsgHandler(&m_dispatcher));

	m_run = true;

	LOG_WARN << "start robotmgr successfully!";
	return true;
}

bool RobotMgr::uninit()
{
	m_dispatcher.clear();
	global::uninit();

	return true;
}

Robot* RobotMgr::createRobot()
{
	Robot *robot = new Robot;

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

	uninit();
	LOG_WARN << "close robotmgr successfully!";
}

void RobotMgr::stop()
{
	LOG_WARN << "start closing robotmgr ...";
	LOG_WARN << "	<m_taskQueue.size() = " << m_taskQueue.size() << ">";

	m_taskQueue.put(boost::bind(&RobotMgr::stopping, this));
}

void RobotMgr::stopping()
{
	run();

	m_wan.stop();

	// 将关闭网络时产生的网络任务执行完
	run();

	m_run = false;
}

void RobotMgr::run()
{
	m_taskQueue.run();

	m_httpMgr.run();
	sleep(10);
}

void RobotMgr::handleMsg(Robot *robot, int msgId, Buffer &buf, Timestamp receiveTime)
{
	if (!m_run) {
		return;
	}

	m_dispatcher.dispatch(*robot, msgId, buf.peek(), buf.readableBytes(), receiveTime);
}

void RobotMgr::onRobotDisconnect(Robot *robot)
{
	m_robotMap.erase(robot->m_robotId);
	delete robot;

	if (m_robotMap.empty()) {
		stop();
	}
}
