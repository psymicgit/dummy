

#ifndef _DB_SESSION_H_20100726
#define _DB_SESSION_H_20100726

#include "sddb.h"
#include "db_pool.h"

class CSDDBModule;

class CDBSession : public SGDP::ISDDBSession
{
private:
	CDBSession();
	CDBSession(CConnPool * pPool);

public:
	~CDBSession();

    UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize, INT32 timeout = -1);

	INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL, INT32 timeout = -1);

	INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs,  UINT32* pdwErr = NULL, string* pstrErr = NULL, INT32 timeout = -1);

	bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet, INT32 timeout = -1);

	bool SDAPI SelectDB(const CHAR *pcDBName, INT32 timeout = -1);

	bool SDAPI AddDBCommand(SGDP::ISDDBCommand *poDBCommand);

    bool SDAPI QuickAddDBCommand(SGDP::ISDDBCommand *poDBCommand);

	bool SDAPI Run(UINT32& dwCount);

	UINT32 SDAPI GetExcuteCmdCount();
	UINT32 SDAPI GetExcutedCmdCount();

		/**
	* @brief 获取一个连接
	*/
	virtual SGDP::ISDDBConnection* SDAPI GetDBConnection();

	/**
	* @brief 释放一个连接
	*/
	virtual void SDAPI FreeDBConnection(SGDP::ISDDBConnection*) ;


private:
	friend class CSDDBModule;
	CConnPool * m_pPool;
};

#endif

