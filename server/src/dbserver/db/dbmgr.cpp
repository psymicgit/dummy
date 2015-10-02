///<------------------------------------------------------------------------------
//< @file:   gameserver\db\dbmgr.cpp
//< @author: 洪坤安
//< @date:   2015年3月27日 17:21:38
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "dbmgr.h"

#include <db/dbfactory.h>
#include <db/db.h>
#include <db/dbsession.h>
#include <db/dbrecordset.h>
#include <db/dbcommand.h>
#include <db/dbconnection.h>
#include <tool/ticktool.h>

#include "config/dbconfig.h"

class OpHistoryDBCmd : public DBCommand
{
	virtual void execute(DBConnection &conn)
	{
		sprintf_s(global::g_sql, sizeof(global::g_sql),
		          "INSERT INTO `ophistory` VALUES ('2014-03-27 00:00:00', '%d', '7', '61015', '2', '40003', '21012', '61015', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '61015', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');", m_idx);
		DB::DBExecuteCode code = conn.execute(global::g_sql);
	}

	virtual void onExecuted()
	{
		if (0 == m_idx % 1000) {
			LOG_INFO << "executed " << m_idx;
		}
	}

	virtual void release()
	{
		delete this;
	}

public:
	int m_idx;
};

class PlayerDBCmd : public DBCommand
{
public:
	virtual void execute(DBConnection &conn)
	{
		sprintf_s(global::g_sql, sizeof(global::g_sql),
		          "INSERT INTO `players` VALUES ('%d', '76C91654-E392-44C0-BA9F-EA24E2B84E12', '', 'qqqqqqq', 'dispname%d', '0', '0', '100', '39360', '48', '0', '1000', '1100', '0', '900', '0', '0', 0x01018200000080000002000000020000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, '44', '2200', '4', '2', '0', '0', '0000-00-00 00:00:00', '0', '', '0', '4975', '0', '0', '0', '0', '10', '1', '0', '0', '2013-12-06 09:42:48', '2013-12-06 09:43:06', '2013-12-06 09:42:48', '1970-01-01 08:00:00', '0', '2013-12-02 11:11:23', '2013-12-06 09:43:06');", m_idx, m_idx);

		DB::DBExecuteCode code = conn.execute(global::g_sql);
	}

	virtual void onExecuted()
	{
		if (0 == m_idx % 1000) {
			LOG_INFO << "executed " << m_idx;
		}
	}

	virtual void release()
	{
		delete this;
	}

public:
	int m_idx;
};

void DBMgr::test()
{

	char sql[1024] = {0};

	LOG_INFO << "start test";

	/*
	for (size_t i = 1; i <= 10000; i++) {
		sprintf_s(sql, sizeof(sql), "INSERT INTO `ophistory` VALUES ('2014-03-27 00:00:00', '%d', '7', '61015', '2', '40003', '21012', '61015', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '61015', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');", i);
		DB::DBExecuteCode code = m_logdb->execute(sql);

		if (code != DB::EXECUTE_OK) {
			break;
		}

		if (0 == i % 1000) {
			LOG_INFO << "executing " << i;
		}
	}

	m_logdb->execute("truncate ophistory");
	*/

	/*
	m_logdb->execute("truncate ophistory");

	for (size_t i = 1; i <= 10000; i++) {
	OpHistoryDBCmd *cmd = new OpHistoryDBCmd;
	cmd->m_idx = i;

	m_logdb->addDBCommand(cmd);
	}
	*/

	DBConnection *conn = m_gamedb->getConnection(0);

	size_t times = 10000;

	{
		Tick tick("游戏库插入测试");

		m_gamedb->execute("truncate players");
		conn->startTransaction();

		for (size_t i = 1; i <= times; i++) {
			PlayerDBCmd *cmd = new PlayerDBCmd;
			cmd->m_idx = i;

			// cmd->execute(*conn);
			// cmd->onExecuted();
			// cmd->release();
			m_gamedb->addDBCommand(cmd);
		}

		conn->commit();

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	{
		Tick tick("游戏库select速度测试");

		DBRecordSet *res = NULL;
		for (size_t i = 1; i <= times; i++) {
			sprintf_s(sql, sizeof(sql), "select * from players where PlayerID > %d limit 1", i);
			DB::DBQueryCode code = conn->query(sql, &res);

			if (code != DB::QUERY_OK_HAVE_RECORDSET) {
				break;
			}

			if (0 == i % 1000) {
				LOG_INFO << "executing " << i;
			}
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}
}

bool DBMgr::init(DBConfig &config)
{
	DBAccount &gamedbAccnt = config.m_gamedbAccnt;
	DBAccount &logdbAccnt = config.m_logdbAccnt;

	m_gamedb = new DBFactory;
	m_logdb = new DBFactory;

	if (NULL == m_gamedb) {
		LOG_ERROR << "new gamedb failed";
		return false;
	}

	if (NULL == m_logdb) {
		LOG_ERROR << "new gamedb failed";
		return false;
	}

	if (!m_gamedb->init(gamedbAccnt)) {
		LOG_ERROR << "initialize gamedb failed";
		return false;
	}

	if (!m_logdb->init(logdbAccnt)) {
		LOG_ERROR << "initialize gamedb failed";
		return false;
	}

	test();
	return true;
}

void DBMgr::run()
{
	m_gamedb->run(100);
	m_logdb->run(100);
}

void DBMgr::stop()
{
	m_gamedb->stop();
	m_logdb->stop();
}
