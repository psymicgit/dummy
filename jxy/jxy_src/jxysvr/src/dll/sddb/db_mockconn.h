
#ifndef SGDP_DB_MOCK_CONN_H_20100728
#define SGDP_DB_MOCK_CONN_H_20100728

#include "db_conn.h"
#include "db_pool.h"

// 
// 类名称   : CMockConn
// 功能     : <实现了sql连接相关的操作>
//
class CMockConn : public IConn
{
private:
	CMockConn();

	CMockConn(const CMockConn & rhs);

	CMockConn & operator = (const CMockConn & rhs);

public:
	~CMockConn();

	bool operator == (const CMockConn & rhs);

	bool SDAPI CheckConnection();

    UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize);

	INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL, UINT32* pEffectRowNum = NULL, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr = NULL, string* pstrErr = NULL);

	void SDAPI BeginTransaction();

	void SDAPI CommitTransaction();

	void SDAPI RollbackTransaction();

	void SDAPI Release();

	bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet);

	bool SDAPI SelectDB(const CHAR *pcDBName);

	bool IsUsed();

	void SetUsed(bool bUsed);

private:
	friend class CConnFactory;
	friend class CConnPool;

	CConnPool * m_pPool;
	volatile bool m_bUsed;
};

#endif

