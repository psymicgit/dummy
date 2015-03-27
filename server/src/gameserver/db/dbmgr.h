///<------------------------------------------------------------------------------
//< @file:   gameserver\db\dbmgr.h
//< @author: 洪坤安
//< @date:   2015年3月27日 17:21:26
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbmgr_h_
#define _dbmgr_h_

class DBFactory;
class DBSession;

class DBMgr
{
public:
	DBMgr()
		: m_dbfactory(NULL)
	{
	}

	bool init();

	void run();

	void test();

private:
	DBFactory *m_dbfactory;

	DBSession *m_gamedb;
	DBSession *m_logdb;
};

#endif //_dbmgr_h_