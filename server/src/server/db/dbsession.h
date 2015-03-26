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

// DBSession接口类，代表与一个数据库服务器连接的会话，该会话也可以维护多个连接
class DBSession
{
public:
	// 将二进制字符串转化为数据库可识别的以0结尾的字符串
	uint32 EscapeString(const char *pSrc, int srcSize, char *pDest, int nDstSize, int timeout = -1);

	int ExecuteSql(const char *pSQL, uint64 *pInsertId = NULL, int timeout = -1);

	int ExecuteSqlRs(const char *pSQL, int **ppoRs, UINT32* pdwErr = NULL, string* pstrErr = NULL, int timeout = -1);

	bool CreateDB(const char *pcDBName, bool bForce, const char *pcCharSet, int timeout = -1);

	bool SelectDB(const char *pcDBName, int timeout = -1);

	bool AddDBCommand(IDBCommand *poDBCommand);

	bool QuickAddDBCommand(IDBCommand*poDBCommand);

	bool Run(UINT32& dwCount);

private:
	friend class CSDDBModule;
};

#endif //_dbsession_h_