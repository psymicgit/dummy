
#ifndef SGDP_DB_RECODRSET_H_20070613
#define SGDP_DB_RECODRSET_H_20070613

#include "mysql.h"
#include "sddb.h"
#include <map>


struct CmpObj
{
	bool operator()(const CHAR *pSrc, const CHAR *pDst) const 
	{
		return strcmp(pSrc, pDst) < 0;
	}

}; 
typedef std::map<const CHAR*, INT32, CmpObj> NameToIndexMap;

// 
// 类名称   : CMySQLRecordSet
// 功能     : <继承并实现了ISDDBRecordSet类的接口，提供SQL执行结果记录集的相关操作>
//
class CMySQLRecordSet : public SGDP::ISDDBRecordSet
{
	friend class CMySQLConn;
public:
	CMySQLRecordSet();
	~CMySQLRecordSet();

	//
	// 函数名:  GetRecordCount
	// 功能:    获取记录条数
	// 返回值:  记录条数
	//
	UINT32 SDAPI GetRecordCount();

	//
	// 函数名:  GetFieldCount
	// 功能:    获取字段个数
	// 返回值:  字段个数
	//
	UINT32 SDAPI GetFieldCount();

	//
	// 函数名:  GetRecord
	// 功能:    获取下一个记录行
	// 返回值:  如果返回值为TRUE表示获取成功，如果为false表示获取失败
	//
	bool SDAPI GetRecord();

	//
	// 函数名:  GetFieldValue
	// 功能:    获取当前记录行中某一个字段的值
	// 参数:    [in] dwIndex:字段对应的索引，从0开始
	// 返回值:  对应索引的字段值
	//
	const CHAR * SDAPI GetFieldValue(UINT32 dwIndex);

	//
	// 函数名:  GetFieldLength
	// 功能:    获取字段缓冲区长度
	// 参数:    [in] dwIndex:字段对应的索引，从0开始
	// 返回值:  索引对应字段的缓冲区长度
	//
	INT32 SDAPI GetFieldLength(UINT32 dwIndex);

	/**
	* @brief 获取字段名
	*
	* @param dwIndex 字段的Index
	* @return 如果有该Index的域，返回字段名；
	* 如果没有该Index的域，返回NULL；
	*/
	const CHAR * SDAPI GetFieldName(UINT32 dwIndex) ;

	//
	// 函数名:  Release
	// 功能:    释放结果记录集
	//
	void SDAPI Release();

	//
	// 函数名：
	// 功能：获取当前记录行中的某一字段值
	// 参数：[in] pszFieldName:字段的列名
	// 返回值：列名对应的字段值
	const CHAR * SDAPI GetFieldValueByName(const CHAR *pszFieldName);

	//
	// 函数名:  GetFieldLengthByName
	// 功能:    获取字段缓冲区长度
	// 参数:    [in] pszFieldName:字段的列名
	// 返回值:  列名对应字段的缓冲区长度
	//
	INT32 SDAPI GetFieldLengthByName(const CHAR *pszFieldName);

protected:
	bool	_FillMap();

protected:
	MYSQL_RES *m_res;
	MYSQL_ROW m_row; 
	unsigned long *m_adwLengths;
	NameToIndexMap m_mapFieldName;
};

#endif

