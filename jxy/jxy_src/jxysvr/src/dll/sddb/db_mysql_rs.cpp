#include "sddbconfig.h"
#if ( defined(WIN32) || defined(WIN64) )
#include <WinSock2.h>
#endif

#include "db_mysql_rs.h"
#include <sdstring.h>

using namespace SGDP;

CMySQLRecordSet::CMySQLRecordSet() :
m_res(NULL), 
m_row(NULL), 
m_adwLengths(NULL)
{
}

CMySQLRecordSet::~CMySQLRecordSet()
{
	if(m_res != NULL)
	{
		mysql_free_result(m_res);
	}
}

UINT32 CMySQLRecordSet::GetRecordCount()
{
	return ( (m_res != NULL) ? (UINT32)mysql_num_rows(m_res) : 0 );
}

UINT32 CMySQLRecordSet::GetFieldCount()
{
	return ( (m_res != NULL) ? (UINT32)mysql_num_fields(m_res) : 0 );
}

bool CMySQLRecordSet::GetRecord()
{
	if(NULL == m_res)
	{
		return false;
	}

	m_row = mysql_fetch_row(m_res);
	if(m_row != NULL)
	{
		m_adwLengths = mysql_fetch_lengths(m_res);
	}

	return (m_row != NULL);
}

const CHAR* CMySQLRecordSet::GetFieldValue(UINT32 dwIndex)
{
	if(NULL == m_row)
	{
		return NULL;
	}
	if(dwIndex >= this->GetFieldCount())
	{
		return NULL;
	}
	return m_row[dwIndex];
}

INT32 CMySQLRecordSet::GetFieldLength(UINT32 dwIndex)
{
	if(NULL == m_row)
	{
		return 0;
	}
	if(dwIndex >= this->GetFieldCount())
	{
		return 0;
	}
	return m_adwLengths[dwIndex];
}

const CHAR* CMySQLRecordSet::GetFieldName(UINT32 dwIndex)
{
	MYSQL_FIELD*	field = NULL ; 
	mysql_field_seek( m_res, dwIndex ) ;
	if( field = mysql_fetch_field( m_res ) )
	{
		return field->name ;
	}
	return NULL ;
}

void CMySQLRecordSet::Release()
{
	if(m_res != NULL)
	{
		mysql_free_result(m_res);
	}

	m_res = NULL;
	m_row = NULL;

	delete this;
}

const CHAR* SDAPI CMySQLRecordSet::GetFieldValueByName( const CHAR* pszFieldName )
{
	// 初次调用时，填充Map，以后每次检测Map是否有值。
	if(0 == m_mapFieldName.size())
	{
		if (false == _FillMap())
		{
			return NULL;
		}
	}
	NameToIndexMap::iterator itFind, itEnd;
	itFind= m_mapFieldName.find(pszFieldName);
	itEnd = m_mapFieldName.end();
	if(itFind != itEnd)
	{
		return this->GetFieldValue(m_mapFieldName[pszFieldName]);
	}
	CRITICAL(_SDT(" GetFieldValueByName : Comlumn name [%s]:Conn't be found!\n"), _SDTA2T(pszFieldName));
	return NULL;
}

INT32 SDAPI CMySQLRecordSet::GetFieldLengthByName( const CHAR* pszFieldName )
{
	// 初次调用时，填充Map，以后每次检测Map是否有值。
	if(0 == m_mapFieldName.size())
	{
		if (false == _FillMap())
		{
			return 0;
		}
	}
	if(m_mapFieldName.find(pszFieldName) != m_mapFieldName.end())
	{
		return this->GetFieldLength(m_mapFieldName[pszFieldName]);
	}
	CRITICAL(_SDT(" GetFieldLengthByName : Comlumn name [%s]:Conn't be found\n"), _SDTA2T(pszFieldName));
	return 0;
}

bool CMySQLRecordSet::_FillMap()
{
	if(NULL == m_res)
	{
		return false;
	}
	MYSQL_FIELD * field = NULL;
	int nIndex = 0;
	while(field = mysql_fetch_field(m_res))
	{
		m_mapFieldName[field->name] = nIndex;
		++nIndex;
	}
	return true;
}

