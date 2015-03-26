///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmgr.h
//< @author: 洪坤安
//< @date:   2015年3月16日 11:43:59
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robotmgr_h_
#define _robotmgr_h_

#include <net/netreactor.h>
#include <basic/taskqueue.h>
#include <net/msgdispatcher.h>
#include <net/netfactory.h>
#include "robot.h"

class RobotMgr
{
public:
	bool init();

	void start();

	void run();

	void handleMsg(Robot &robot, int msgId, Buffer&, Timestamp receiveTime);

public:
	TaskQueue m_taskQueue;
	bool m_isquit;

	MsgDispatcher<Robot> m_dispatcher;
	NetFactory m_wan;
};

#endif //_robotmgr_h_