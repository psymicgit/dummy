///<------------------------------------------------------------------------------
//< @file:   server\db\dbfactory.h
//< @author: 洪坤安
//< @date:   2015年3月26日 20:3:29
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbfactory_h_
#define _dbfactory_h_

class DBSession;
class DBConnection;
struct DBAccount;

class DBFactory : public Singleton<DBFactory>
{
public:
	DBSession* createDBSession(DBAccount&, int minPoolSize = 1, int maxPoolSize = 1);

	void close();

	void del(DBSession*);

private:
	typedef std::vector<DBSession*> DBSessions;
	DBSessions m_dbsessions;
};

#endif //_dbfactory_h_