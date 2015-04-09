
#ifndef SGDP_DB_MYSQLCONN_H_20070613
#define SGDP_DB_MYSQLCONN_H_20070613

#include "sdnetutils.h"
#include "mysql.h"
#include "db_mysql_rs.h"
#include <map>
#include <string>
#include "db_conn.h"

class CConnFactory;

// 
// 类名称   : CMySQLConn
// 功能     : <实现了sql连接相关的操作>
//
class CMySQLConn : public IConn
{
private:
	CMySQLConn();

	explicit CMySQLConn(const CMySQLConn & rhs);

	CMySQLConn & operator = (const CMySQLConn & rhs);

public:
	~CMySQLConn();

	bool operator == (const CMySQLConn & rhs);

	bool SDAPI CheckConnection();

    UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize);

	INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL, UINT32* pEffectRowNum = NULL, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr = NULL, string* pstrErr = NULL);
	
	INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, std::vector<SGDP::ISDDBRecordSet*>& vecRs, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	void SDAPI BeginTransaction();

	void SDAPI CommitTransaction();

	void SDAPI RollbackTransaction();

	bool Connect(const CHAR *pszServerAddr, INT32 nServerPort, 
				 const CHAR *pszDBName, const CHAR *pszLoginName, 
				 const CHAR *pszPassword, const CHAR *pszCharactSet,
				 const CHAR* pszUnixSocket);

	void SDAPI Release();

	bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet);

	bool SDAPI SelectDB(const CHAR *pcDBName);

	bool IsUsed();

	void SetUsed(bool bUsed);

	//成功返回非空
	MYSQL_STMT *GetStatementPrepared( const string &strStrSql );

	BOOL BindParam( MYSQL_STMT *pStmt, MYSQL_BIND *pBind  );

	BOOL ExecuteStatementPrepared( MYSQL_STMT *pStmt );
private:
	friend class CConnFactory;
	UINT64 _GetInsertId();

	//以下函数用于数据库的连接Charset的设置
    bool _GetDBSCharSetInfo();
    bool _IsCompatiableCharSet();
    bool _SetServerNames();
    bool _IsValidCharacterSet(const CHAR *pszSetName);
	////////////////////////////////////////////////
	void _SetConnectionString(const CHAR* pszServerAddr, INT32 nServerPort, const CHAR* pszDBName, 
								const CHAR* pszLoginName, const CHAR* pszPassword, const CHAR* pszCharactSet, const CHAR* pszUnixSocket);
	bool _Connect();
	bool _CheckConnection();
	bool _ReConnect();

private:
	CHAR m_szServerAddr[SDDB_MAX_NAME_SIZE];
	CHAR m_szDBName[SDDB_MAX_NAME_SIZE];
	CHAR m_szLoginName[SDDB_MAX_NAME_SIZE];
	CHAR m_szPassword[SDDB_MAX_NAME_SIZE];
	CHAR m_szCharactSet[SDDB_MAX_CHARACTSET_SIZE];
	CHAR m_szUnixSocket[SDDB_MAX_NAME_SIZE];

	INT32 m_nServerPort;
	MYSQL *m_poConnection;

	volatile bool m_bUsed;
	volatile bool m_bConnected;

	typedef std::map<std::string, std::string> Charsets;
	Charsets m_oCharsets;

	typedef std::map< std::string, MYSQL_STMT * > MAP_STMT_TYPE;
	MAP_STMT_TYPE m_mapStmtPtr;
};

#endif

