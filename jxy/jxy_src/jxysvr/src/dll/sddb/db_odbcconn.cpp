#include "db_odbcconn.h"
#include "sddbconfig.h"
#include "sdstring.h"
#include "sddebug.h"

using namespace SGDP;
#if (defined(WIN32) || defined(WIN64))

//使用获取错误信息SQLGetDiagRec

CODBCConn::CODBCConn() : 
henv(NULL),
hdbc(NULL),
hstmt(NULL),
m_bUsed(false),
m_bConnected(false)
{
	memset(m_szServerAddr, 0, SDDB_MAX_NAME_SIZE);
	memset(m_szDBName, 0, SDDB_MAX_NAME_SIZE);
	memset(m_szLoginName, 0, SDDB_MAX_NAME_SIZE);
	memset(m_szPassword, 0, SDDB_MAX_NAME_SIZE);
	memset(m_szCharactSet, 0, SDDB_MAX_CHARACTSET_SIZE);
}

CODBCConn::~CODBCConn()
{
	this->Release();
}

bool CODBCConn::CheckConnection()
{
	return m_bConnected && NULL != hdbc;
}

UINT32 CODBCConn::EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize)
{
	//TODO
	return -1;
}

INT32 CODBCConn::ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId, UINT32* pEffectRowNum, UINT32* pdwErr, string* pstrErr)
{
	if(false == this->_CheckConnection())
	{
		return SGDP::SDDB_ERR_CONN;
	}
#ifdef UNICODE
	SQLRETURN retcode = SQLExecDirect(hstmt, (SQLWCHAR *)_SDTA2T((const CHAR*)(SQLCHAR *)pSQL), sizeof(pSQL));
#else
	SQLRETURN retcode = SQLExecDirect(hstmt, (SQLCHAR *)pSQL, sizeof(pSQL));
#endif
	if (retcode == SQL_SUCCESS)
	{
		if(NULL != pInsertId)
		{
			*pInsertId = -1;
		}
		SQLLEN RowCount;
		retcode = SQLRowCount(hstmt, &RowCount);
		if (retcode == SQL_SUCCESS)
		{
			return (INT32)RowCount;
		}
		else
		{
			return SGDP::SDDB_ERR_UNKNOWN;
		}
	}
	else
	{
		return SGDP::SDDB_ERR_UNKNOWN;
	}
}

INT32 CODBCConn::ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr, string* pstrErr)
{
	if(false == this->_CheckConnection())
	{
		return SGDP::SDDB_ERR_CONN;
	}
#ifdef UNICODE
	SQLRETURN retcode = SQLExecDirect(hstmt, (SQLWCHAR *)_SDTA2T((const CHAR*)(SQLCHAR *)pSQL), sizeof(pSQL));

#else
	SQLRETURN retcode = SQLExecDirect(hstmt, (SQLCHAR *)pSQL, sizeof(pSQL));
#endif
	if (retcode == SQL_SUCCESS)
	{
		SQLLEN RowCount;
		retcode = SQLRowCount(hstmt, &RowCount);
		if (retcode != SQL_SUCCESS)
		{
			*ppoRs = NULL;
			return SGDP::SDDB_ERR_UNKNOWN;
		}
		if(RowCount <= 0)
		{
			*ppoRs = NULL;
			return SGDP::SDDB_NO_RECORDSET;
		}

		CODBCRecordSet * pRs = SDNew CODBCRecordSet();
		if(NULL == pRs)
		{
			CRITICAL(_SDT("[%s] alloc CODBCRecordSet Error"),_SDTA2T(__FUNCTION__));
			return SGDP::SDDB_ERR_UNKNOWN;
		}
		pRs->m_hstmt = hstmt;
		pRs->m_RowCount = RowCount;
		*ppoRs = pRs;
		return SGDP::SDDB_HAS_RECORDSET;
	}
	return SGDP::SDDB_ERR_UNKNOWN;
}

void CODBCConn::BeginTransaction()
{
	SQLRETURN nRet = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (void *)SQL_AUTOCOMMIT_OFF, SQL_IS_INTEGER);
	if(SQL_SUCCESS != nRet && SQL_SUCCESS_WITH_INFO != nRet)
	{
		CRITICAL(_SDT("[%s] BeginTransaction Error"),_SDTA2T(__FUNCTION__));
	}
}

void CODBCConn::CommitTransaction()
{
	SQLRETURN nRet = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	if(SQL_SUCCESS != nRet && SQL_SUCCESS_WITH_INFO != nRet)
	{
		CRITICAL(_SDT("[%s] CommitTransaction SQLEndTran Error"), _SDTA2T(__FUNCTION__));
	}
	nRet = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (void *)SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER);
	if(SQL_SUCCESS != nRet && SQL_SUCCESS_WITH_INFO != nRet)
	{
		CRITICAL(_SDT("[%s] CommitTransaction AutoCommit Error"), _SDTA2T(__FUNCTION__));
	}
}

void CODBCConn::RollbackTransaction()
{
	SQLRETURN nRet = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
	if(SQL_SUCCESS != nRet && SQL_SUCCESS_WITH_INFO != nRet)
	{
		CRITICAL(_SDT("[%s] RollbackTransaction SQLEndTran Error"), _SDTA2T(__FUNCTION__));
	}
	nRet = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (void *)SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER);
	if(SQL_SUCCESS != nRet && SQL_SUCCESS_WITH_INFO != nRet)
	{
		CRITICAL(_SDT("[%s] RollbackTransaction AutoCommit Error"), _SDTA2T(__FUNCTION__));
	}
}

bool CODBCConn::Connect(const CHAR *pszServerAddr, INT32 nServerPort, 
			 const CHAR *pszDBName, const CHAR *pszLoginName, 
			 const CHAR *pszPassword, const CHAR *pszCharactSet)
{
	if(NULL == pszServerAddr || nServerPort <= 0 || NULL == pszDBName ||
		NULL == pszLoginName || NULL == pszPassword || NULL == pszCharactSet)
	{
		return false;
	}
	this->Release();
	this->_SetConnectionString(pszServerAddr, nServerPort, pszDBName, pszLoginName, pszPassword, pszCharactSet);
	return _Connect();
}

void CODBCConn::Release()
{
	if(NULL != hstmt)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		hstmt = NULL;
	}
	if(NULL != hdbc)
	{
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = NULL;
	}
	if(NULL != henv)
	{
		SQLFreeEnv(henv);
		henv = NULL;
	}
}

bool CODBCConn::CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet)
{
	CHAR SQL[280];
	sprintf(SQL, "CREATE DATABASE %s", pcDBName);
	return this->ExecuteSql(SQL) >= 0;
}

bool CODBCConn::SelectDB(const CHAR *pcDBName)
{
	CHAR SQL[280];
	sprintf(SQL, "USE %s", pcDBName);
	return this->ExecuteSql(SQL) >= 0;
}

bool CODBCConn::IsUsed()
{
	return m_bUsed;
}

void CODBCConn::SetUsed(bool bUsed)
{
	this->m_bUsed = bUsed;
}

UINT64 CODBCConn::_GetInsertId()
{
	return 0;
}

void CODBCConn::_SetConnectionString(const CHAR* pszServerAddr, INT32 nServerPort, const CHAR* pszDBName, 
							const CHAR* pszLoginName, const CHAR* pszPassword, const CHAR* pszCharactSet)
{
	SGDP::SDStrncpy(m_szServerAddr, pszServerAddr, sizeof(m_szServerAddr));
	SGDP::SDStrncpy(m_szDBName, pszDBName, sizeof(m_szDBName));
	SGDP::SDStrncpy(m_szLoginName, pszLoginName, sizeof(m_szLoginName));
	SGDP::SDStrncpy(m_szPassword, pszPassword, sizeof(m_szPassword));

	if ('\0' != pszCharactSet[0])
	{
		SGDP::SDStrncpy(m_szCharactSet, pszCharactSet, sizeof(m_szCharactSet));
		INFO(_SDT("Set DB charset: %s\n"), _SDTA2T(m_szCharactSet));
	}
	else
	{
		INFO(_SDT("Using default DB charset: %s\n"), _SDTA2T(m_szCharactSet));
	}

	m_nServerPort = nServerPort;
}

bool CODBCConn::_Connect()
{
	//这里需要考虑nServerPort，pszDBName，pszCharactSet
	SQLRETURN retcode;
	int i = 5;//设置登陆超时时间
	SQLPOINTER rgbValue = &i;

	// 分配环境句柄
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		// 设置环境句柄ODBC版本属性
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			// 分配连接句柄
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); 

			// 设置登陆超时时间
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(rgbValue), 0);

				// 连接数据源
#ifdef UNICODE
				retcode = SQLConnect(hdbc,(SQLWCHAR *)_SDTA2T((const CHAR*)((SQLCHAR*) m_szServerAddr)), SDDB_MAX_NAME_SIZE,(SQLWCHAR *)_SDTA2T((CHAR*)((SQLCHAR*) m_szLoginName)) , SDDB_MAX_NAME_SIZE, (SQLWCHAR *)_SDTA2T((CHAR*)((SQLCHAR*) m_szPassword)), SDDB_MAX_NAME_SIZE);
#else
				retcode = SQLConnect(hdbc,(SQLCHAR*) m_szServerAddr, SDDB_MAX_NAME_SIZE, (SQLCHAR*) m_szLoginName, SDDB_MAX_NAME_SIZE, (SQLCHAR*) m_szPassword, SDDB_MAX_NAME_SIZE);

#endif

				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
				{
					// 分配数据库状态句柄
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 

					
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
					{
						// 此时，可以处理数据了
						m_bConnected = true;
						m_bUsed = true;
						return true;
					}
					else
					{
						// 分配数据库状态句柄失败
						SQLDisconnect(hdbc);
					}
				}
				else
				{
					// 连接数据源失败
					this->Release();
					return false;
				}
			}
			else
			{
				//分配连接句柄失败
				this->Release();
				return false;
			}
		}
		else
		{
			// 设置环境句柄ODBC版本属性失败
			this->Release();
			return false;
		}
	}
	else
	{
		// 分配环境句柄失败
		this->Release();
		return false;
	}
	this->Release();
	return false;
}

bool CODBCConn::_CheckConnection()
{
	//检测流程:
	//1.如果为“异常连接”，关闭连接，转到“没有连接”状态；
	//2.如果为“没有连接”，建立连接，期望转到“正常连接”状态；
	//3.返回是否“正常连接”
	//
	//备注:目前有三种连接状态,
	// 正常连接(m_poConnection != NULL, m_bConnected == true),
	// 异常连接(m_poConnection != NULL, m_bConnected == false),
	// 没有连接(m_poConnection == NULL, m_bConnected == false)
	if(!m_bConnected && NULL != hdbc)//“异常连接”状态
	{
		this->Release();
	}
	if(NULL == hdbc)//“没有连接”状态
	{
		m_bConnected = this->_Connect();
	}
	return m_bConnected;
}

bool CODBCConn::_ReConnect()
{
	this->Release();
	return this->_Connect();
}

#endif

