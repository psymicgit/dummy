
#ifndef SGDP_DB_WRAPPED_CONN_H_20100720
#define SGDP_DB_WRAPPED_CONN_H_20100720

#include "db_conn.h"
#include "db_pool.h"

// 
// 类名称   : CMySQLConn
// 功能     : <实现了sql连接相关的操作>
//
class CWrappedConn : public IConn
{
private:
	CWrappedConn();
	bool Init(CConnPool * pPool, IConn * pConn);

public:
	~CWrappedConn();

	bool SDAPI CheckConnection();

    UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize);

	INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL);

	INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs);

	void SDAPI BeginTransaction();

	void SDAPI CommitTransaction();

	void SDAPI RollbackTransaction();

	void SDAPI Release();

	bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet);

	bool SDAPI SelectDB(const CHAR *pcDBName);

	bool IsUsed();

	void SetUsed(bool bUsed);

private:
	friend class CConnPool;

	CConnPool * m_pPool;
	IConn * m_pConn;
};

#endif

