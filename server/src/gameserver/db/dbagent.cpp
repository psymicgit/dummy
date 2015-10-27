#include "dbagent.h"

#include "gameserver.h"

#include <protocol.pb.h>

#include <db/dbcommand.h>

DbAgent::DbAgent(SqlDBType dbtype)
	: m_cond(m_lock)
	, m_running(false)
	, m_dbtype(dbtype)
{
}

bool DbAgent::start()
{
	m_dbthread.createThread(boost::bind(&DbAgent::run, this), 1);
	return true;
}

bool DbAgent::post(DBCommand *cmd)
{
	lock_guard_t<mutex_t> lock(m_lock);

	bool notify = m_cmds.empty();
	m_cmds.push_back(cmd);

	if (notify)	{
		m_cond.notify();
	}

	return true;
}

void DbAgent::run()
{
	DBCommand *cmd = NULL;
	std::string sql;

	DBCommandList cmds;

	PostSql postsql;
	postsql.set_dbtype(m_dbtype);

	while(m_running) {
		{
			lock_guard_t<> lock(m_lock);
			cmds.clear();

			while (m_cmds.empty()) {
				m_cond.wait();
			}

			cmds.swap(m_cmds);
		}

		postsql.clear_sqls();
		postsql.set_time(1000);

		for (size_t i = 0; i < cmds.size(); i++) {
			cmd = cmds[i];
			cmd->getSql(sql);

			if (sql.empty()) {
				continue;
			}

			SqlMsg *singleSql = postsql.add_sqls();
			if (NULL == singleSql) {
				continue;
			}

			singleSql->set_sqltype(NormalSql);
			singleSql->set_sql(sql);
			singleSql->set_sqlid(i);
		}

		GameServer::Instance().sendToDBServer(ePostSql, postsql);
	}
}