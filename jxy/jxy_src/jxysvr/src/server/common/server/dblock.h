#ifndef DB_LOCK_H
#define DB_LOCK_H

#include <dll/sddb/sddb.h>
#include <string>
using std::string;

class CDBLock
{
public:
	CDBLock(SGDP::ISDDBConnection* poDBConn, const string &strTableName)
	{
		m_poDBConn=poDBConn;
		char szSql[256]={0};

		m_strTableName=strTableName;
		sprintf( szSql, "LOCK TABLES %s Write", strTableName.c_str() );
		INT32 nRet=m_poDBConn->ExecuteSql( szSql );
	}
	~CDBLock()
	{
		INT32 nRet=m_poDBConn->ExecuteSql( "UNLOCK TABLES" );
	}

private:
	SGDP::ISDDBConnection* m_poDBConn;
	string m_strTableName;
};

#endif