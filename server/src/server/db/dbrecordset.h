///<------------------------------------------------------------------------------
//< @file:   server\db\dbrecordset.h
//< @author: 洪坤安
//< @date:   2015年3月26日 22:31:34
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbrecordset_h_
#define _dbrecordset_h_

#include <mysql.h>

struct CmpName {
	bool operator()(const char *pSrc, const char *pDst) const
	{
		return strcmp(pSrc, pDst) < 0;
	}
};

typedef std::map<const char*, int, CmpName> NameToIndexMap;

class DBRecordSet
{
	friend class DBConnection;

public:
	DBRecordSet();
	~DBRecordSet();

	//
	// 函数名:  GetRecordCount
	// 功能:    获取记录条数
	// 返回值:  记录条数
	//
	uint32 GetRecordCount();

	//
	// 函数名:  GetFieldCount
	// 功能:    获取字段个数
	// 返回值:  字段个数
	//
	uint32 GetFieldCount();

	//
	// 函数名:  GetRecord
	// 功能:    获取下一个记录行
	// 返回值:  如果返回值为TRUE表示获取成功，如果为false表示获取失败
	//
	bool GetRecord();

	//
	// 函数名:  GetFieldValue
	// 功能:    获取当前记录行中某一个字段的值
	// 参数:    [in] dwIndex:字段对应的索引，从0开始
	// 返回值:  对应索引的字段值
	//
	const char * GetFieldValue(uint32 dwIndex);

	//
	// 函数名:  GetFieldLength
	// 功能:    获取字段缓冲区长度
	// 参数:    [in] dwIndex:字段对应的索引，从0开始
	// 返回值:  索引对应字段的缓冲区长度
	//
	int GetFieldLength(uint32 dwIndex);

	/**
	* @brief 获取字段名
	*
	* @param dwIndex 字段的Index
	* @return 如果有该Index的域，返回字段名；
	* 如果没有该Index的域，返回NULL；
	*/
	const char* GetFieldName(uint32 dwIndex) ;

	//
	// 函数名:  Release
	// 功能:    释放结果记录集
	//
	void Release();

	//
	// 函数名：
	// 功能：获取当前记录行中的某一字段值
	// 参数：[in] pszFieldName:字段的列名
	// 返回值：列名对应的字段值
	const char * GetFieldValueByName(const char *pszFieldName);

	//
	// 函数名:  GetFieldLengthByName
	// 功能:    获取字段缓冲区长度
	// 参数:    [in] pszFieldName:字段的列名
	// 返回值:  列名对应字段的缓冲区长度
	//
	int GetFieldLengthByName(const char *pszFieldName);

	void print();

protected:
	bool	_FillMap();

protected:
	MYSQL_RES *m_res;
	MYSQL_ROW m_row;
	unsigned long *m_adwLengths;

	NameToIndexMap m_mapFieldName;
};

#endif //_dbrecordset_h_