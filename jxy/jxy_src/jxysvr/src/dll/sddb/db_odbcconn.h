#ifndef SGDP_DB_ODBCCONN_H_20100823
#define SGDP_DB_ODBCCONN_H_20100823

#if (defined(WIN32) || defined(WIN64))

#include "db_odbc_rs.h"
#include <map>
#include <string>
#include "db_conn.h"
#include <sqlext.h>

class CConnFactory;

// 
// 类名称   : CMySQLConn
// 功能     : <实现了sql连接相关的操作>
//
class CODBCConn : public IConn
{
private:
	CODBCConn();

	explicit CODBCConn(const CODBCConn & rhs);

public:
	~CODBCConn();

	bool SDAPI CheckConnection();

    UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize);

	INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL, UINT32* pEffectRowNum = NULL, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	void SDAPI BeginTransaction();

	void SDAPI CommitTransaction();

	void SDAPI RollbackTransaction();

	bool Connect(const CHAR *pszServerAddr, INT32 nServerPort, 
				 const CHAR *pszDBName, const CHAR *pszLoginName, 
				 const CHAR *pszPassword, const CHAR *pszCharactSet);

	void SDAPI Release();

	bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet);

	bool SDAPI SelectDB(const CHAR *pcDBName);

	bool IsUsed();

	void SetUsed(bool bUsed);

private:
	friend class CConnFactory;
	UINT64 _GetInsertId();

	void _SetConnectionString(const CHAR* pszServerAddr, INT32 nServerPort, const CHAR* pszDBName, 
								const CHAR* pszLoginName, const CHAR* pszPassword, const CHAR* pszCharactSet);
	bool _Connect();
	bool _CheckConnection();
	bool _ReConnect();

private:
	CHAR m_szServerAddr[SDDB_MAX_NAME_SIZE];
	CHAR m_szDBName[SDDB_MAX_NAME_SIZE];
	CHAR m_szLoginName[SDDB_MAX_NAME_SIZE];
	CHAR m_szPassword[SDDB_MAX_NAME_SIZE];
	CHAR m_szCharactSet[SDDB_MAX_CHARACTSET_SIZE];

	INT32 m_nServerPort;

	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;

	volatile bool m_bUsed;
	volatile bool m_bConnected;
};

#endif

#endif

