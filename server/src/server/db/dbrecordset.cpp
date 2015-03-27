///<------------------------------------------------------------------------------
//< @file:   server\db\dbrecordset.cpp
//< @author: 洪坤安
//< @date:   2015年3月26日 22:31:45
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "dbrecordset.h"


DBRecordSet::DBRecordSet() :
	m_res(NULL),
	m_row(NULL),
	m_adwLengths(NULL)
{
}

DBRecordSet::~DBRecordSet()
{
	if(m_res != NULL) {
		mysql_free_result(m_res);
	}
}

uint32 DBRecordSet::GetRecordCount()
{
	return ( (m_res != NULL) ? (uint32)mysql_num_rows(m_res) : 0 );
}

uint32 DBRecordSet::GetFieldCount()
{
	return ( (m_res != NULL) ? (uint32)mysql_num_fields(m_res) : 0 );
}

bool DBRecordSet::GetRecord()
{
	if(NULL == m_res) {
		return false;
	}

	m_row = mysql_fetch_row(m_res);
	if(m_row != NULL) {
		m_adwLengths = mysql_fetch_lengths(m_res);
	}

	return (m_row != NULL);
}

const char* DBRecordSet::GetFieldValue(uint32 dwIndex)
{
	if(NULL == m_row) {
		return NULL;
	}
	if(dwIndex >= this->GetFieldCount()) {
		return NULL;
	}
	return m_row[dwIndex];
}

int DBRecordSet::GetFieldLength(uint32 dwIndex)
{
	if(NULL == m_row) {
		return 0;
	}
	if(dwIndex >= this->GetFieldCount()) {
		return 0;
	}
	return m_adwLengths[dwIndex];
}

const char* DBRecordSet::GetFieldName(uint32 dwIndex)
{
	MYSQL_FIELD*	field = NULL ;
	mysql_field_seek( m_res, dwIndex ) ;
	if( field = mysql_fetch_field( m_res ) ) {
		return field->name ;
	}
	return NULL ;
}

void DBRecordSet::Release()
{
	if(m_res != NULL) {
		mysql_free_result(m_res);
	}

	m_res = NULL;
	m_row = NULL;

	delete this;
}

const char* DBRecordSet::GetFieldValueByName( const char* pszFieldName )
{
	// 初次调用时，填充Map，以后每次检测Map是否有值。
	if(0 == m_mapFieldName.size()) {
		if (false == _FillMap()) {
			return NULL;
		}
	}
	NameToIndexMap::iterator itFind, itEnd;
	itFind = m_mapFieldName.find(pszFieldName);
	itEnd = m_mapFieldName.end();
	if(itFind != itEnd) {
		return this->GetFieldValue(m_mapFieldName[pszFieldName]);
	}

	LOG_ERROR << "GetFieldValueByName : Comlumn name [ " << pszFieldName << " ]:Conn't be found!";
	return NULL;
}

int DBRecordSet::GetFieldLengthByName( const char* pszFieldName )
{
	// 初次调用时，填充Map，以后每次检测Map是否有值。
	if(0 == m_mapFieldName.size()) {
		if (false == _FillMap()) {
			return 0;
		}
	}
	if(m_mapFieldName.find(pszFieldName) != m_mapFieldName.end()) {
		return this->GetFieldLength(m_mapFieldName[pszFieldName]);
	}
	LOG_ERROR << "GetFieldLengthByName : Comlumn name [ " << pszFieldName << " ]:Conn't be found!";
	return 0;
}

bool DBRecordSet::_FillMap()
{
	if(NULL == m_res) {
		return false;
	}
	MYSQL_FIELD * field = NULL;
	int nIndex = 0;
	while(field = mysql_fetch_field(m_res)) {
		m_mapFieldName[field->name] = nIndex;
		++nIndex;
	}
	return true;
}

void DBRecordSet::print()
{
	std::string text;

	uint32 fieldCount = GetFieldCount();
	for (uint32 fieldidx = 0; fieldidx < fieldCount; fieldidx++) {
		text += GetFieldName(fieldidx);

		if (fieldidx + 1 < fieldCount) {
			text += ",";
		}
	}

	text += "\n";

	string row;
	while(GetRecord()) {
		row.clear();

		for (uint32 fieldidx = 0; fieldidx < fieldCount; fieldidx++) {
			const char* val = GetFieldValue(fieldidx);
			row += val;

			if (fieldidx + 1 < fieldCount) {
				row += ",";
			}
		}

		row += "\n";
		text += row;
	}

	LOG_INFO << text;
}
