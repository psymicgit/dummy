///<------------------------------------------------------------------------------
//< @file:   dbagent.h
//< @author: 洪坤安
//< @date:   2015年11月12日
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbagent_h_
#define _dbagent_h_

#include <game_db.pb.h>
#include <basic/thread.h>

class DBCommand;

// 作为数据存储服务器的本地代理类，所有数据存储的操作由本类代理
class DbAgent
{
	typedef std::vector<DBCommand*> DBCommandList;

public:
	DbAgent(SqlDBType dbtype);

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