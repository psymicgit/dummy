///<------------------------------------------------------------------------------
//< @file:   server\db\dbfactory.h
//< @author: 洪坤安
//< @date:   2015年3月26日 20:3:29
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbfactory_h_
#define _dbfactory_h_

#include "db.h"
#include "basic/lock.h"
#include "basic/thread.h"

class DBSession;
class DBConnection;
struct DBAccount;

// db操作中心类，维护与一个数据库服务器的多个连接
class DBFactory : public Singleton<DBFactory>
{
public:
	DBFactory();

	DBSession* createDBSession(DBAccount&, int minPoolSize = 1, int maxPoolSize = 1);

	void close();

	void del(DBSession*);

public:
	std::string name();

	bool init(const DBAccount &dbaccount);

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

	void stop();

private:
	void executeCmd(DBCommand*);

	DBConnection* connect();

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

	Thread m_dbthread;

private:
	typedef std::vector<DBSession*> DBSessions;
	DBSessions m_dbsessions;
};

#endif //_dbfactory_h_