
#ifndef SGDP_DB_ADO_RECODRSET_H_20100824
#define SGDP_DB_ADO_RECODRSET_H_20100824

#if (defined(WIN32) || defined(WIN64))

#include "sddb.h"
#include <list>
#include "SDMutex.h"
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF")
// 
// 类名称   : CADORecordSet
// 功能     : <继承并实现了ISDDBRecordSet类的接口，提供SQL执行结果记录集的相关操作>
//
class CADORecordSet : public SGDP::ISDDBRecordSet
{
	friend class CADOConn;
public:
	CADORecordSet();
	~CADORecordSet();

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

private:
	void ClearDatas();

private:
	_RecordsetPtr m_rsPtr;
	volatile bool m_bFirstRecord;
	std::list<CHAR *> m_Datas;
};

#endif

#endif

