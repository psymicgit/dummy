///<------------------------------------------------------------------------------
//< @file:   dbagent.h
//< @author: ������
//< @date:   2015��11��12��
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbagent_h_
#define _dbagent_h_

#include <game_db.pb.h>
#include <basic/thread.h>

class DBCommand;

// ��Ϊ���ݴ洢�������ı��ش����࣬�������ݴ洢�Ĳ����ɱ������
class DbAgent
{
	typedef std::vector<DBCommand*> DBCommandList;

public:
	DbAgent();

	bool Init(SqlDBType dbtype);

	bool start();

	bool post(DBCommand*);

	void run();

public:
	SqlDBType		m_dbtype;

	DBCommandList	m_cmds;

	mutex_t			m_lock;

	condition_var_t m_cond;

	bool			m_running;

	Thread			m_dbthread;
};

#endif // _dbagent_h_