///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmgr.h
//< @author: ������
//< @date:   2015��3��16�� 11:43:59
//< @brief:  ����������
//< Copyright (c) 2015 ������. All rights reserved.
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

// �����˹�����
class RobotMgr : public Singleton<RobotMgr>
{
	typedef std::map<uint32, Robot*> RobotMap;

public:
	virtual std::string name() { return string("robotmgr"); }

public:
	bool init(const char* jsonConfig);

	bool uninit();

	void start();

	void creareRobots();

	// ִֹͣ�У�������һ��ֹͣ����Ͷ���������
	void stop();

	// ִ������ֹͣ����
	void stopping();

	// ÿ����Ϸѭ����ִ�еĲ���
	void run();

	void onRobotDisconnect(Robot*);

	Robot* createRobot();

private:
	uint32 allocRobotId() { return ++m_allocRobotId; }

public:

	TaskQueue				m_taskQueue;	// �������
	TimerQueue				m_timers;		// ��ʱ������
	bool					m_run;			// <�Ƿ�������>��־λ
	uint32					m_allocRobotId;	// ���еĻ�����id
	RobotMap				m_robotMap;		// �������б�
	MsgDispatcher<Robot>	m_dispatcher;	// ��Ϣ�ɷ���
	Net						m_wan;			// ��������
	RobotHttpMgr			m_httpMgr;		// http��������
	RobotConfig				m_config;		// ����������
};

#endif //_robotmgr_h_