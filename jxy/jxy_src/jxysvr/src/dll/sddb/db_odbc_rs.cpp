#include "db_odbc_rs.h"

#if (defined(WIN32) || defined(WIN64))

CODBCRecordSet::CODBCRecordSet() : 
m_ColumnCount(-1)
{
}

CODBCRecordSet::~CODBCRecordSet()
{
	
}

UINT32 CODBCRecordSet::GetRecordCount()
{
	return m_RowCount;
}

UINT32 CODBCRecordSet::GetFieldCount()
{
	if(m_ColumnCount < 0)
	{
		SQLRETURN retcode = SQLNumResultCols(m_hstmt, &m_ColumnCount);
		if (retcode != SQL_SUCCESS)
		{
			m_ColumnCount = SGDP::SDDB_ERR_UNKNOWN;
			return SGDP::SDDB_ERR_UNKNOWN;
		}
		return m_ColumnCount;
	}
	else
	{
		return m_ColumnCount;
	}
}

bool CODBCRecordSet::GetRecord()
{
	SQLRETURN retcode = SQLFetch(m_hstmt);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		return true;
	}
	return false;
}

const CHAR * CODBCRecordSet::GetFieldValue(UINT32 dwIndex)
{
	SQLRETURN retcode = SQLGetData(m_hstmt, dwIndex, SQL_C_BINARY, &buffer, 65535, NULL);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		return buffer;
	}
	return NULL;
}

INT32 CODBCRecordSet::GetFieldLength(UINT32 dwIndex)
{
	//TODO
	return -1;
}

void CODBCRecordSet::Release()
{
	delete this;
}

const CHAR * CODBCRecordSet::GetFieldValueByName(const CHAR *pszFieldName)
{
	//TODO
	return NULL;
}

INT32 CODBCRecordSet::GetFieldLengthByName(const CHAR *pszFieldName)
{
	//TODO
	return -1;
}

#endif

