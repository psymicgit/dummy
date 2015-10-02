///<------------------------------------------------------------------------------
//< @file:   server\db\dbconnection.h
//< @author: 洪坤安
//< @date:   2015年3月26日 20:27:31
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbconnection_h_
#define _dbconnection_h_

#include "db.h"

class DBConnection
{
	friend class DBSession;
	friend class DBFactory;

	typedef tr1::unordered_map<std::string, std::string> Charsets;

public:
	DBConnection()
		: m_connected(false)
		, m_used(false)
		, m_mysql(NULL)
	{
	}

public:
	bool connect();

	void release();

	bool reconnect();

	bool escape(const char *pSrc, int nSrcSize, char *pDest, int nDstSize);

	DB::DBExecuteCode execute(const char *sql, uint32 strlen = 0, uint64 *pInsertId = NULL, uint32* pEffectRowNum = NULL);

	DB::DBQueryCode query(const char *sql, DBRecordSet **pRes);

	void startTransaction();

	void commit();

	void rollback();

	bool selectdb(const char* dbname);

private:
	bool setCharsetName();

	bool getCharsets(Charsets&);

	bool isCompatiableCharset(Charsets&);

	bool checkConnection();

public:
	DBAccount m_account;
	volatile bool m_connected;
	volatile bool m_used;

private:
	MYSQL *m_mysql;

	typedef std::tr1::unordered_map<std::string, MYSQL_STMT*> PreparedStmtMap;
	PreparedStmtMap m_preparedStmtMap;
};

#endif //_dbconnection_h_