#include "sddbconfig.h"
#include "db_mockconn.h"
#include "sdstring.h"
#include <errmsg.h>


CMockConn::CMockConn()
{
}

CMockConn::CMockConn(const CMockConn & rhs)
{
	//不允许拷贝构造CMockConn
}

CMockConn & CMockConn::operator = (const CMockConn & rhs)
{
	//不允许赋值CMockConn
	return *this;
}

CMockConn::~CMockConn()
{
}

bool CMockConn::operator == (const CMockConn & rhs)
{
	return this == &rhs;
}

bool CMockConn::CheckConnection()
{
	return true;
}

UINT32 CMockConn::EscapeString(const CHAR *pSrc, INT32 nSrcSize, CHAR *pDest, INT32 nDstSize)
{
	return 0;
}

INT32 CMockConn::ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId, UINT32* pEffectRowNum, UINT32* pdwErr, string* pstrErr)
{
	return SGDP::SDDB_ERR_CONN;
}

INT32 CMockConn::ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr, string* pstrErr)
{
	return SGDP::SDDB_ERR_CONN;
}

void CMockConn::BeginTransaction()
{
}

void CMockConn::CommitTransaction()
{
}

void CMockConn::RollbackTransaction()
{
}

void CMockConn::Release()
{
}

bool CMockConn::CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet)
{
	return false;
}

bool CMockConn::SelectDB(const CHAR *pcDBName)
{
	return false;
}

bool CMockConn::IsUsed()
{
	return m_bUsed;
}

void CMockConn::SetUsed(bool bUsed)
{
	this->m_bUsed = bUsed;
}

