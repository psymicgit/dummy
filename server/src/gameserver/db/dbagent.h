#ifndef _dbagent_h_
#define _dbagent_h_

#include <game_db.pb.h>
#include <basic/thread.h>

class DBCommand;

class DbAgent
{
	typedef std::vector<DBCommand*> DBCommandList;

public:
	DbAgent(SqlDBType dbtype);

	bool start();

	bool post(DBCommand*);

	void run();

public:
	SqlDBType m_dbtype;

	DBCommandList m_cmds;

	mutex_t m_lock;
	condition_var_t m_cond;

	bool m_running;

	Thread m_dbthread;
};

#endif