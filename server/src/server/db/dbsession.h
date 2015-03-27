///<------------------------------------------------------------------------------
//< @file:   server\db\dbsession.h
//< @author: 洪坤安
//< @date:   2015年3月26日 20:25:16
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbsession_h_
#define _dbsession_h_

#include "db.h"
#include "basic/lock.h"
#include "basic/thread.h"

// DBSession接口类，代表与一个数据库服务器连接的会话，该会话可以维护多个连接
class DBSession
{
	DBSession();

public:
	bool init(DBAccount &dbaccount, int minPoolSize, int maxPoolSize);

	// 将二进制字符串转化为数据库可识别的以0结尾的字符串
	bool escape(const char *pSrc, int nSrcSize, char *pDest, int nDstSize);

	DB::DBExecuteCode execute(const char *sql, uint32 strlen = 0, uint64 *pInsertId = NULL, uint32* pEffectRowNum = NULL);

	DB::DBQueryCode query(const char *sql, DBRecordSet **pRes);

	// 执行命令，返回实际执行的命令数
	uint32 run(uint32 loopcnt);

	// 分配出的数据库线程执行的run调用
	void threadrun();

	void addDBCommand(DBCommand*);

	DBConnection* getConnection(int groupId);

	void recycle(DBConnection*);

private:
	void executeCmd(DBCommand*);

	DBConnection* createConnection();

	DBConnection* borrow();

	DBConnection* onUsed(DBConnection*);

	//删除在ConnectionMap中的一个连接
	void removeConnFromMap(DBConnection*);


public:
	DBAccount m_dbaccount;

private:
	typedef std::vector<DBCommand*> DBCommandList;
	typedef tr1::unordered_map<int /* 组Id */, DBConnection*> ConnectionMap; // 组别Id <-> 连接
	typedef tr1::unordered_set<DBConnection*> ConnectionPool;

	friend class DBFactory;

	DBCommandList m_cmdList;
	DBCommandList m_executedCmdList;

	mutex_t m_cmdLock;
	mutex_t m_executedCmdLock;
	mutex_t m_poolLock;
	mutex_t m_mapLock;

	condition_var_t m_cmdCondition;

	volatile bool m_running;

	int m_minPoolSize;
	int m_maxPoolSize;

	ConnectionPool m_connPool;
	ConnectionMap m_connMap;

	volatile int m_connCount;
	volatile int m_freeConnCount;

	thread_t m_dbthread;
};

#endif //_dbsession_h_