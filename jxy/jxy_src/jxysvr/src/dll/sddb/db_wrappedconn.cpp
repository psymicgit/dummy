#include "db_wrappedconn.h"

using namespace SGDP;

CWrappedConn::CWrappedConn() :
m_pPool(NULL),
m_pConn(NULL)
{
}

CWrappedConn::~CWrappedConn()
{
	this->Release();
}

//初始化包装的连接
bool CWrappedConn::Init(CConnPool * pPool, IConn * pConn)
{
	if(NULL == pPool || NULL == pConn)
	{
		return false;
	}
	this->m_pPool = pPool;
	this->m_pConn = pConn;
	return true;
}

bool CWrappedConn::CheckConnection()
{
	if(NULL == this->m_pConn)
	{
		return false;
	}
	return this->m_pConn->CheckConnection();
}

UINT32 CWrappedConn::EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize)
{
	if(NULL == m_pConn)
	{
		return 0;
	}
	return this->m_pConn->EscapeString(pSrc, nSrcSize, pDest, nDstSize);
}

INT32 CWrappedConn::ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId)
{
	if(NULL == m_pConn)
	{
		return SDDB_ERR_CONN;
	}
	return this->m_pConn->ExecuteSql(pSQL, pInsertId);
}

INT32 CWrappedConn::ExecuteSqlRs(const CHAR *pSQL, ISDDBRecordSet **ppoRs)
{
	if(NULL == m_pConn)
	{
		return SDDB_ERR_CONN;
	}
	return this->m_pConn->ExecuteSqlRs(pSQL, ppoRs);
}

void CWrappedConn::BeginTransaction()
{
	if(NULL == m_pConn)
	{
		return;
	}
	this->m_pConn->BeginTransaction();
}

void CWrappedConn::CommitTransaction()
{
	if(NULL == m_pConn)
	{
		return;
	}
	this->m_pConn->CommitTransaction();
}

void CWrappedConn::RollbackTransaction()
{
	if(NULL == m_pConn)
	{
		return;
	}
	this->m_pConn->RollbackTransaction();
}

//关闭连接，不真正的关闭连接，仅将连接归还给连接池
void CWrappedConn::Release()
{
	//if(NULL != m_pPool)
	//{
	//	this->m_pPool->_Return(this);
	//}
	//this->m_pConn = NULL;
}

bool CWrappedConn::CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet)
{
	if(NULL == m_pConn)
	{
		return false;
	}
	return this->m_pConn->CreateDB(pcDBName, bForce, pcCharSet);
}

bool CWrappedConn::SelectDB(const CHAR *pcDBName)
{
	if(NULL == m_pConn)
	{
		return false;
	}
	return this->m_pConn->SelectDB(pcDBName);
}

bool CWrappedConn::IsUsed()
{
	return this->m_pConn->IsUsed();
}

void CWrappedConn::SetUsed(bool bUsed)
{
	this->m_pConn->SetUsed(bUsed);
}

