///<------------------------------------------------------------------------------
//< @file:   server\db\dbsession.cpp
//< @author: 洪坤安
//< @date:   2015年3月26日 20:25:24
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "dbsession.h"

#include "dbcommand.h"
#include "dbconnection.h"

#include "tool/atomictool.h"
#include "basic/taskqueue.h"

DBSession::DBSession()
	: m_cmdCondition(m_cmdLock)
	, m_running(false)
	, m_minPoolSize(0)
	, m_maxPoolSize(0)
	, m_connCount(0)
{
}

bool DBSession::init(DBAccount &dbaccount, int minPoolSize, int maxPoolSize)
{
	m_dbaccount = dbaccount;
	m_minPoolSize = minPoolSize;
	m_maxPoolSize = maxPoolSize;

	m_running = true;

	for(int i = 0; i < m_minPoolSize; i++) {
		DBConnection *conn = createConnection();
		if(NULL == conn) {
			LOG_ERROR << "create connection failed, alrealy created " << m_connCount;
			return false;
		}

		{
			lock_guard_t<> lock(m_poolLock);
			m_connPool.insert(conn);
			atomictool::inc(&m_connCount);
		}
	}

	m_dbthread.create_thread(task_binder_t::gen(&DBSession::threadrun, this), m_maxPoolSize);
	return true;
}

bool DBSession::escape(const char *pSrc, int nSrcSize, char *pDest, int nDstSize)
{
	DBConnection *conn = getConnection(0);
	bool ret = conn->escape(pSrc, nSrcSize, pDest, nDstSize);
	recycle(conn);
	return ret;
}

DB::DBExecuteCode DBSession::execute(const char *sql, uint32 strlen, uint64 *pInsertId, uint32* pEffectRowNum)
{
	DBConnection *conn = getConnection(0);
	DB::DBExecuteCode code = conn->execute(sql, strlen, pInsertId, pEffectRowNum);
	recycle(conn);
	return code;
}

DB::DBQueryCode DBSession::query(const char *sql, DBRecordSet **pRes)
{
	DBConnection *conn = getConnection(0);
	DB::DBQueryCode code = conn->query(sql, pRes);
	recycle(conn);
	return code;
}

uint32 DBSession::run(uint32 loopcnt)
{
	if (m_executedCmdList.empty()) {
		return 0;
	}

	DBCommandList executingCmdList;

	{
		lock_guard_t<> lock(m_executedCmdLock);
		size_t copycnt = m_executedCmdList.size();

		if (loopcnt > 0) {
			copycnt = MIN(loopcnt, copycnt);
		}

		executingCmdList.assign(m_executedCmdList.begin(), m_executedCmdList.begin() + copycnt);
		m_executedCmdList.erase(m_executedCmdList.begin(), m_executedCmdList.begin() + copycnt);
	}

	DBCommand *cmd = NULL;

	for (size_t i = 0; i < executingCmdList.size(); i++) {
		cmd = executingCmdList[i];
		cmd->onExecuted();
		cmd->release();
	}

	return executingCmdList.size();
}

void DBSession::threadrun()
{
	DBCommand *cmd = NULL;

	DBCommandList cmds;

	while(m_running) {
		{
			lock_guard_t<> lock(m_cmdLock);
			cmds.clear();

			while (m_cmdList.empty()) {
				m_cmdCondition.wait();
			}

			// cmds.swap(m_cmdList);

			size_t takecnt = m_cmdList.size();
			takecnt = MIN(50, takecnt);

			cmds.assign(m_cmdList.begin(), m_cmdList.begin() + takecnt);
			m_cmdList.erase(m_cmdList.begin(), m_cmdList.begin() + takecnt);
		}

		for (size_t i = 0; i < cmds.size(); i++) {
			cmd = cmds[i];
			executeCmd(cmd);
		}
	}
}

void DBSession::executeCmd(DBCommand *cmd)
{
	DBConnection *conn = NULL;

	while(m_running) {
		conn = getConnection(cmd->getGroupId());
		if(NULL == conn) {
			sleep(10);
			continue;
		}

		break;
	}

	cmd->execute(*conn);

	{
		lock_guard_t<> lock(m_executedCmdLock);
		m_executedCmdList.push_back(cmd);
	}

	recycle(conn);
}

void DBSession::addDBCommand(DBCommand *cmd)
{
	if(NULL == cmd) {
		return;
	}

	lock_guard_t<> lock(m_cmdLock);

	bool needNotify = m_cmdList.empty();
	m_cmdList.push_back(cmd);

	if (needNotify)	{
		m_cmdCondition.broadcast();
	}
}

DBConnection* DBSession::getConnection(int groupId)
{
	if (0 ==  groupId) {
		return borrow();
	}

	DBConnection * conn = NULL;

	lock_guard_t<> lock(m_mapLock);

	ConnectionMap::iterator itr = m_connMap.find(groupId);
	if(m_connMap.end() == itr) {
		conn = borrow();

		if(NULL != conn) {
			m_connMap.insert(std::make_pair(groupId, conn));
		}
	}
	else {
		conn = itr->second;
		conn = onUsed(conn);
	}

	return conn;
}

DBConnection* DBSession::onUsed(DBConnection *conn)
{
	lock_guard_t<> lock(m_poolLock);
	if(conn->m_used) {
		return NULL;
	}

	ConnectionPool::iterator itr = m_connPool.find(conn);
	if(itr != m_connPool.end()) {
		m_connPool.erase(itr);
	}

	conn->m_used = true;
	return conn;
}

DBConnection* DBSession::borrow()
{
	DBConnection *conn = NULL;

	lock_guard_t<> lock(m_poolLock);

	if(m_connPool.empty()) {
		if(m_connCount >= m_maxPoolSize) {
			return NULL;
		}

		conn = createConnection();
		if(NULL == conn) {
			LOG_ERROR << "create dbconnection failed, alrealy created connection count = " << m_connCount;
			return NULL;
		}

		m_connPool.insert(conn);

		atomictool::inc(&m_connCount);
	}

	ConnectionPool::iterator itr = m_connPool.begin();
	conn = *itr;

	m_connPool.erase(itr);

	conn->m_used = true;
	return conn;
}

//归还连接，这里需要锁
//归还连接时，
//1.如果该连接异常，直接关闭该连接
//2.如果连接数大于核心连接数，直接关闭该连接
void DBSession::recycle(DBConnection *conn)
{
	if(!conn->checkConnection()) {
		removeConnFromMap(conn);
		return;
	}

	lock_guard_t<> lock(m_poolLock);
	if(m_connCount > m_minPoolSize) {
		removeConnFromMap(conn);
		return;
	}

	m_connPool.insert(conn);
	conn->m_used = false;
}

void DBSession::removeConnFromMap(DBConnection *conn)
{
	{
		lock_guard_t<> lock(m_mapLock);

		ConnectionMap::iterator itr = m_connMap.begin();
		ConnectionMap::iterator end = m_connMap.end();

		for(; itr != end; ++itr) {
			if(conn == itr->second) {
				int groupId = itr->first;
				m_connMap.erase(itr);
				break;
			}
		}
	}

	conn->release();
	delete conn;

	atomictool::dec(&m_connCount);
}

DBConnection* DBSession::createConnection()
{
	DBConnection *conn = new DBConnection;
	conn->m_account = m_dbaccount;

	if(!conn->connect()) {
		delete conn;

		conn = NULL;
	}

	return conn;
}

void DBSession::stop()
{
	LOG_WARN << "	stoping dbsession ...";

	{
		lock_guard_t<> lock(m_poolLock);
		LOG_INFO << "	<dbconnection pool size = " << m_connPool.size() << ", all connection count = " << m_connCount << ">";
	}

	{
		lock_guard_t<> lock(m_executedCmdLock);
		size_t cmdCount = m_executedCmdList.size();

		LOG_INFO << "	<m_cmdList.size() = " << m_cmdList.size() << ", m_executedCmdList.size() = " << m_executedCmdList.size() << ">";
	}

	m_running = false;
	m_dbthread.join();

	run(0);

	{
		lock_guard_t<> lock(m_poolLock);
		for (ConnectionPool::iterator itr = m_connPool.begin(); itr != m_connPool.end(); ++itr) {
			DBConnection *dbconn = *itr;
			dbconn->release();

			delete dbconn;
		}

		m_connPool.clear();
		m_connCount = 0;
	}

	LOG_WARN << "	stop dbsession successfully!";

}
