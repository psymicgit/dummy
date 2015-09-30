///<------------------------------------------------------------------------------
//< @file:   dbserver\db\dbmgr.h
//< @author: 洪坤安
//< @date:   2015年9月29日 17:21:26
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbmgr_h_
#define _dbmgr_h_

class DBFactory;
class DBSession;
struct DBConfig;

class DBMgr
{
public:
	DBMgr()
		: m_gamedb(NULL)
		, m_logdb(NULL)
	{
	}

	bool init(DBConfig&);

	void run();

	void stop();

	void test();

private:
	DBFactory *m_gamedb;
	DBFactory *m_logdb;

	std::vector<std::string> m_sqls;
};

#endif //_dbmgr_h_