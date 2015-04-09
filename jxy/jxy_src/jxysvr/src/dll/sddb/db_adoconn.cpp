#include "db_adoconn.h"
#include "sddbconfig.h"
#include "sdstring.h"
#include "db_ado_rs.h"
#include "sddebug.h"

#if (defined(WIN32) || defined(WIN64))

using namespace std;
using namespace SGDP;
CADOConn::CADOConn() : 
m_bUsed(false),
m_bConnected(false),
m_usConnTimeout(5)
{
}

CADOConn::~CADOConn()
{
	this->Release();
}

bool CADOConn::CheckConnection()
{
	return m_bConnected && NULL != m_ConPtr;
}

UINT32 CADOConn::EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize)
{
	if (nDstSize < nSrcSize * 2 + 3)
	{
		WARN(_SDT("[%s] Dest insufficient."), _SDTA2T(__FUNCTION__));
		return false;
	}

	*pDest++ = '0';
	*pDest++ = 'x';

	const CHAR tab[] = "0123456789ABCDEF";
	int i = 0;
	for (; i < nSrcSize; ++i)
	{
		*pDest++ = tab[*pSrc >> 4];
		*pDest++ = tab[*pSrc & 0x0F];
		pSrc++;
	}

	*pDest = '\0';
	return nSrcSize * 2 + 3;
}

INT32 CADOConn::ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId, UINT32* pEffectRowNum, UINT32* pdwErr, string* pstrErr)
{
	if(false == this->_CheckConnection())
	{
		return SGDP::SDDB_ERR_CONN;
	}

	long lRowCount = 0;
	_variant_t RowCount(lRowCount);
	try
	{
		m_ConPtr->Execute(pSQL, &RowCount, adExecuteNoRecords);
	}
	catch(_com_error &e)
	{
		string strError = "Unknown Error";
		if(e.Description().length() > 0)
		{
			strError = e.Description();
		}
		CRITICAL(_SDT("[%s] catch(_com_error)\tSQL: %s\tErr: %.8x^Description:%s\tpConn:0x%08x\tSvr:%s\tDBName:%s]"), _SDTA2T(__FUNCTION__),  
			_SDTA2T(pSQL), e.Error(), _SDTA2T(strError.c_str()), m_ConPtr, _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		if(NULL != m_ConPtr)
		{
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
		return SGDP::SDDB_ERR_UNKNOWN;
	}
	catch(...)
	{
		CRITICAL(_SDT("[%s] catch(...)\tpConn:0x%08x\tSQL: %s\tSvr:%s\tDBName:%s]"), _SDTA2T(__FUNCTION__), m_ConPtr, _SDTA2T(pSQL), _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		if(NULL != m_ConPtr)
		{
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
		return SGDP::SDDB_ERR_UNKNOWN;
	}
	if(NULL != pInsertId)
	{
		*pInsertId = this->_GetInsertId();
	}
	return RowCount.lVal;
}

INT32 CADOConn::ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr, string* pstrErr)
{
	if(false == this->_CheckConnection())
	{
		return SGDP::SDDB_ERR_CONN;
	}
	CADORecordSet * pRs = SDNew CADORecordSet();
	if(NULL == pRs)
	{
		CRITICAL(_SDT("[%s] alloc CADORecordSet Error"), _SDTA2T(__FUNCTION__));
		return SGDP::SDDB_ERR_UNKNOWN;
	}
	try
	{
		//pRs->m_rsPtr->PutActiveConnection(m_ConPtr);
		//pRs->m_rsPtr = m_ConPtr->Execute(pSQL, NULL, adOptionUnspecified | adCmdUnspecified);
		HRESULT hRet = pRs->m_rsPtr->Open(pSQL, _variant_t((IDispatch*)m_ConPtr, true), adOpenStatic, adLockReadOnly, adOptionUnspecified | adCmdUnspecified);
		if(FAILED(hRet))
		{
			CRITICAL(_SDT("[%s] ExecuteSqlRs failed, SQL: %s, nRet: %d"), _SDTA2T(__FUNCTION__), _SDTA2T(pSQL), hRet);
		}
	}
	catch(_com_error &e)
	{
		string strError = "Unknown Error";
		if(e.Description().length() > 0)
		{
			strError = e.Description();
		}
		CRITICAL(_SDT("[%s] catch(_com_error)\tSQL: %s\tErr:%.8x^Description:%s\tpConn:0x%08x\tSvr:%s\tDBName:%s]"), _SDTA2T(__FUNCTION__),  
			_SDTA2T(pSQL), e.Error(), _SDTA2T(strError.c_str()), m_ConPtr, _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		if(NULL != m_ConPtr)
		{
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
		if(NULL == pRs->m_rsPtr)
		{
			delete pRs;
			pRs = NULL;
		}
		return SGDP::SDDB_ERR_UNKNOWN;
	}
	catch(...)
	{
		CRITICAL(_SDT("[%s] catch(...)\tpConn:0x%08x\tSQL: %s\tSvr:%s\tDBName:%s]"), _SDTA2T(__FUNCTION__), m_ConPtr, _SDTA2T(pSQL), _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		if(NULL != m_ConPtr)
		{
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
		if(NULL == pRs->m_rsPtr)
		{
			delete pRs;
			pRs = NULL;
		}
		return SGDP::SDDB_ERR_UNKNOWN;
	}
	if(NULL == pRs->m_rsPtr)
	{
		delete pRs;
		pRs = NULL;
		return SGDP::SDDB_NO_RECORDSET;
	}
	*ppoRs = pRs;
	return SGDP::SDDB_HAS_RECORDSET;
}

void CADOConn::BeginTransaction()
{
	m_ConPtr->BeginTrans();
}

void CADOConn::CommitTransaction()
{
	m_ConPtr->CommitTrans();
}

void CADOConn::RollbackTransaction()
{
	m_ConPtr->RollbackTrans();
}

//
//UINT32 CADOConn::GetLastError()
//{
//	if(m_ConPtr->GetErrors()->GetCount() == 0)
//	{
//		return 0;
//	}
//	return m_ConPtr->GetErrors()->GetItem((long)0)->GetNumber();
//}
//
//const CHAR * CADOConn::GetLastErrorInfo()
//{
//	if(m_ConPtr->GetErrors()->GetCount() == 0)
//	{
//		return "No Error";
//	}
//	return m_ConPtr->GetErrors()->GetItem((long)0)->GetDescription();
//}

bool CADOConn::Connect(const CHAR *pszServerAddr, INT32 nServerPort, 
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

void CADOConn::Release()
{
	if(!m_bConnected)
	{
		return;
	}
	try
	{
		if(NULL != m_ConPtr)
		{
			if(m_ConPtr->State == adStateOpen)
			{
				m_ConPtr->Close();
			}
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
		INFO(_SDT("[%s] Close DB Successful----\tSvrIP:%s\tDBName:%s"), _SDTA2T(__FUNCTION__), _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
	}
	catch(_com_error &e)
	{
		//打印关闭连接
		string strError = "Unknown Error";
		if(e.Description().length() > 0)
		{
			strError = e.Description();
		}
		CRITICAL(_SDT("[%s] catch(_com_error)\tErr:%.8x^Description:%s\tpConn:0x%08x\tSvr:%s\tDBName:%s]"), _SDTA2T(__FUNCTION__),  
			e.Error(), _SDTA2T(strError.c_str()), m_ConPtr, _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		if(NULL != m_ConPtr)
		{
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
	}
	catch(...)
	{
		//打印关闭连接
		CRITICAL(_SDT("[%s] catch(...)\tpConn:0x%08x\tSvr:%s\tDBName:%s]"), _SDTA2T(__FUNCTION__), m_ConPtr, _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		if(NULL != m_ConPtr)
		{
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
	}
	m_bUsed = false;
	m_bConnected = false;
}

bool CADOConn::CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet)
{
	CHAR SQL[280];
	sprintf(SQL, "CREATE DATABASE %s", pcDBName);
	return this->ExecuteSql(SQL) >= -1;
}

bool CADOConn::SelectDB(const CHAR *pcDBName)
{
	CHAR SQL[280];
	sprintf(SQL, "USE %s", pcDBName);
	return this->ExecuteSql(SQL) >= -1;
}

bool CADOConn::IsUsed()
{
	return m_bUsed;
}

void CADOConn::SetUsed(bool bUsed)
{
	this->m_bUsed = bUsed;
}

UINT64 CADOConn::_GetInsertId()
{
	//TODO
	//ADO不支持此操作
	return 0;
}

void CADOConn::_SetConnectionString(const CHAR* pszServerAddr, INT32 nServerPort, const CHAR* pszDBName, 
							const CHAR* pszLoginName, const CHAR* pszPassword, const CHAR* pszCharactSet)
{
	SGDP::SDStrncpy(m_szServerAddr, pszServerAddr, sizeof(m_szServerAddr));
	SGDP::SDStrncpy(m_szDBName, pszDBName, sizeof(m_szDBName));
	SGDP::SDStrncpy(m_szLoginName, pszLoginName, sizeof(m_szLoginName));
	SGDP::SDStrncpy(m_szPassword, pszPassword, sizeof(m_szPassword));

	if ('\0' != pszCharactSet[0])
	{
		SGDP::SDStrncpy(m_szCharactSet, pszCharactSet, sizeof(m_szCharactSet));
		INFO(_SDT("Set DB Charset: %s\n"), _SDTA2T(m_szCharactSet));
	}
	else
	{
		INFO(_SDT("Using Default DB Charset: %s\n"), _SDTA2T(m_szCharactSet));
	}

	m_nServerPort = nServerPort;
}

bool CADOConn::_Connect()
{
	if(m_bConnected)
	{
		return true;
	}
	try
	{
		if(NULL != m_ConPtr)
		{
			m_ConPtr.Release();
			m_ConPtr = NULL;
		}
		// 初始化com库
		if (FAILED(CoInitialize(NULL)))
		{
			INFO(_SDT("[%s] Init com library failed. It means CoInitialize has been called before."), _SDTA2T(__FUNCTION__));
			// return -1;
		}

		HRESULT hRet = m_ConPtr.CreateInstance(__uuidof(Connection));
		if(FAILED(hRet))
		{
			CRITICAL(_SDT("[%s] Create connection failed, nRet: %d"), _SDTA2T(__FUNCTION__), hRet);
			return false;
		}

		CHAR szConnectStr[1024];
		_snprintf_s(szConnectStr, 1024, _TRUNCATE, "Provider=SQLOLEDB;Server=%s; Database=%s", m_szServerAddr, m_szDBName);
		m_ConPtr->Provider = "sqloledb";
		m_ConPtr->ConnectionTimeout = m_usConnTimeout;
		INFO(_SDT("Connecting to db. Server: %s, DB: %s."), _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		hRet = m_ConPtr->Open(szConnectStr, m_szLoginName, m_szPassword, adConnectUnspecified);
		if(FAILED(hRet))
		{
			CRITICAL(_SDT("[%s] Open() failed. nRet:%d. SvrIP:%s. DBName:%s"), _SDTA2T(__FUNCTION__), hRet, _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
			m_ConPtr.Release();
			m_ConPtr = NULL;
			return false;
		}
		m_ConPtr->CursorLocation = adUseClient;

		INFO(_SDT("[%s] Connect DB Successful----\tSvrIP:%s\tDBName:%s"), _SDTA2T(__FUNCTION__), _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
		m_bConnected = true;
		return true;
	}
	catch(_com_error &e)
	{
		string strError = "Unknown Error";
		if (e.Description().length() > 0)
		{
			strError = e.Description();
		}

		CRITICAL(_SDT("[%s] Err:%.8x. Description:%s\tpConn:0x%08x\tSvrIP:%s\tDBName:%s"), 
			_SDTA2T(__FUNCTION__), e.Error(), _SDTA2T(strError.c_str()), m_ConPtr, _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
	}
	catch(...)
	{
		CRITICAL(_SDT("[%s] pConn:0x%08x\tSvrIP:%s\tDBName:%s"), _SDTA2T(__FUNCTION__), 
			m_ConPtr, _SDTA2T(m_szServerAddr), _SDTA2T(m_szDBName));
	}

	if(m_ConPtr)
	{
		m_ConPtr.Release();
		m_ConPtr = NULL;
	}

	return false;
}

bool CADOConn::_CheckConnection()
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
	if(!m_bConnected && NULL != m_ConPtr)//“异常连接”状态
	{
		this->Release();
	}
	if(NULL == m_ConPtr)//“没有连接”状态
	{
		m_bConnected = this->_Connect();
	}
	return m_bConnected;
}

bool CADOConn::_ReConnect()
{
	this->Release();
	return this->_Connect();
}

#endif

