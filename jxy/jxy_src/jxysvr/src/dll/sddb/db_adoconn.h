#ifndef SGDP_DB_ADOCONN_H_20100824
#define SGDP_DB_ADOCONN_H_20100824

#if (defined(WIN32) || defined(WIN64))

#include "sddb.h"
#include <map>
#include <string>
#include "db_conn.h"
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF")

class CConnFactory;

// 
// 类名称   : CMySQLConn
// 功能     : <实现了sql连接相关的操作>
//
class CADOConn : public IConn
{
private:
	CADOConn();

	explicit CADOConn(const CADOConn & rhs);

public:
	~CADOConn();

	bool SDAPI CheckConnection();

    UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize);

	INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL, UINT32* pEffectRowNum = NULL, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	void SDAPI BeginTransaction();

	void SDAPI CommitTransaction();

	void SDAPI RollbackTransaction();

	bool Connect(const CHAR *pszServerAddr, INT32 nServerPort, 
				 const CHAR *pszDBName, const CHAR *pszLoginName, 
				 const CHAR *pszPassword, const CHAR *pszCHARactSet);

	void SDAPI Release();

	bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCHARSet);

	bool SDAPI SelectDB(const CHAR *pcDBName);

	bool IsUsed();

	void SetUsed(bool bUsed);

private:
	friend class CConnFactory;
	UINT64 _GetInsertId();

	void _SetConnectionString(const CHAR* pszServerAddr, INT32 nServerPort, const CHAR* pszDBName, 
								const CHAR* pszLoginName, const CHAR* pszPassword, const CHAR* pszCHARactSet);
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

	_ConnectionPtr m_ConPtr;

	volatile bool m_bUsed;
	volatile bool m_bConnected;
	unsigned short m_usConnTimeout;
};

#endif

#endif

