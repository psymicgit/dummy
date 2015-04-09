/******************************************************************************
            

本文件包含类CSqlite3Connection的定义,支持UTF_8和UTF_16编码,sqlite数据库区分字段大小写

文件历史：
1.修改时间：	2011-07-20
  修改描述：    创建文件 
  修改者  ： 	xujun
  ……
******************************************************************************/

#ifndef _SQLITE3DB_INCLUDE_H_
#define _SQLITE3DB_INCLUDE_H_

#include "sqlite3.h"
#include "stdio.h"
#include "sddebug.h"
#include "sdstring.h"
#include "sdtime.h"
#include <map>
#include <string.h>
using namespace std;

namespace SGDP
{
/**
@brief 封装Sqlite3的记录集
*
*功能	:实现了sqlite3的记录集操作
*/
	class CSqlite3RecordSet
	{
#define SDSQLITE_MAX_NAME_SIZE 64       //文件名最大长度
#define SDSQLITE_MAX_NUMBER_SIZE 64     //最大字节数

        typedef std::map<string, INT32> CNameToIndexMap;  //保存字段名与索引值之间的映射关系
        typedef std::map<string, INT32>::iterator CNameToIndexMapItr;

		friend class CSqlite3Conn;
	public:

		/**
		* @brief 构造函数，初始化sqlite记录集对象
		*/
		CSqlite3RecordSet():m_poRes(NULL)
		{
		}

		/**
		* @brief 析构函数，释放sqlite记录集
		*/
		~CSqlite3RecordSet()
		{
			if(NULL != m_poRes)
			{
				sqlite3_finalize(m_poRes);
			}
		}

		/**
        * @brief  获取字段个数
        * @return 字段个数
        */
        UINT32  GetFieldCount();
	
		/**
        * @brief  获取下一个记录行
        * @return TRUE:存在下一个记录 FALSE:当前记录为最后一个记录
        */
		BOOL  GetRecord()
		{
			if(m_poRes == NULL)
				return FALSE;
			INT32 rc = sqlite3_step(m_poRes);//查找结果集
			if(rc == SQLITE_BUSY)
			{
				fprintf(stderr,"SQL error :Sqlite busy!\n");
				return FALSE;
			}
			return (rc == SQLITE_ROW);      //返回单行集
		}

		/**
        * @brief 根据字段名获取字段值（std::string类型)<注意：通用接口，从数据库中取出的数据都为string>
        * @param nIndex : 字段的索引位置
        * @return std::string类型的字段值
        */
        const std::string  GetFieldValue(UINT32 nIndex)
		{
			CHAR* buf;
			string strValue = "";
			INT32 n = sqlite3_column_type(m_poRes,nIndex);
			switch(n)
			{
			case SQLITE_INTEGER:
				{
					CHAR tmpStr[SDSQLITE_MAX_NUMBER_SIZE] = {0};
					SDSnprintf(tmpStr,SDSQLITE_MAX_NUMBER_SIZE,"%d",sqlite3_column_int(m_poRes,nIndex));
                    strValue = tmpStr;
				}
				break;
			case SQLITE_FLOAT:
				{
					CHAR tmpStr[SDSQLITE_MAX_NUMBER_SIZE] = {0};
					SDSnprintf(tmpStr,SDSQLITE_MAX_NUMBER_SIZE,"%f",sqlite3_column_double(m_poRes,nIndex));
                    strValue = tmpStr;
				}
				break;
			case SQLITE_TEXT:
				{
					CHAR tmpStr[SDSQLITE_MAX_NAME_SIZE] = {0};
					SDSnprintf(tmpStr,SDSQLITE_MAX_NAME_SIZE,"%s",sqlite3_column_text(m_poRes,nIndex));
                    strValue = tmpStr;
				}
				break;
			case SQLITE_BLOB:
				{
					INT32 lenth = sqlite3_column_bytes(m_poRes,nIndex);//二进制数据的长度
					buf = SDNew CHAR[lenth];
					const VOID* bData = sqlite3_column_blob(m_poRes,nIndex);//二进制数据
					memcpy(buf,bData,lenth);
                    for(INT32 i=0;i<sizeof(buf);i++)
                        strValue.push_back(buf[i]);
                    SDDelete[] buf;
				}
				break;
			case SQLITE_NULL://返回空字符串类型时
				{
                    strValue = "\0";
				}
				break;
			default:break;
			}

			return strValue;
		}
		
		/**
        * @brief  获取当前记录中指定索引位置字段的类型
        * @param nIndex : 字段的索引位置
        * @return 字段类型
        */
		UINT32  GetFieldType(UINT32 nIndex);		

        /**
        * @brief  获取当前记录中指定索引位置字段的值(长整型)
        * @param nIndex : 字段的索引位置
        * @return 长整型字段值
        */
		const INT64  GetFieldValueInt64(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(长整型)
        * @param pszFieldName : 字段名
        * @return 长整型字段值
        */
        const INT64  GetFieldValueInt64(const CHAR* pszFieldName)
        {
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return 0;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueInt64(m_mapFieldName[pszFieldName]);
            }
            return 0;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(整型)
        * @param nIndex : 字段的索引位置
        * @return 整型字段值
        */
		const INT32  GetFieldValueInt32(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(整型)
        * @param pszFieldName : 字段名
        * @return 整型字段值
        */
        const INT32  GetFieldValueInt32(const CHAR* pszFieldName)
        {
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return 0;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueInt32(m_mapFieldName[pszFieldName]);
            }
            return 0;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(浮点型)
        * @param nIndex : 字段的索引位置
        * @return 浮点型字段值
        */
		const DOUBLE  GetFieldValueDouble(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(浮点型)
        * @param pszFieldName : 字段名
        * @return 浮点型字段值
        */
        const DOUBLE  GetFieldValueDouble(const CHAR* pszFieldName)
        {
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return 0;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueDouble(m_mapFieldName[pszFieldName]);
            }
            return 0;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(字符串型)
        * @param nIndex : 字段的索引位置
        * @return 字符串型字段值
        */
		const UCHAR*  GetFieldValueString(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(字符串型)
        * @param pszFieldName : 字段名
        * @return 字符串型字段值
        */
        const UCHAR*  GetFieldValueString(const CHAR* pszFieldName)
        {
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return NULL;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueString(m_mapFieldName[pszFieldName]);
            }
            return NULL;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(二进制类型)
        * @param nIndex : 字段的索引位置
        * @param nLength      : 二进制数据的长度
        * @return 二进制类型字段值
        */
		const VOID*  GetFieldValueBinary(UINT32 nIndex,UINT32 nLength);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(二进制类型)
        * @param pszFieldName : 字段名
        * @param nLength      : 二进制数据的长度
        * @return 二进制型字段值
        */
        const VOID*  GetFieldValueBinary(const CHAR* pszFieldName,UINT32 nLength)
        {
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return NULL;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueBinary(m_mapFieldName[pszFieldName],nLength);
            }
            return NULL;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(BOOL型)
        * @param nIndex : 字段的索引位置
        * @return BOOL型字段值
        */
        const BOOL  GetFieldValueBoolean(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(BOOL型)
        * @param pszFieldName : 字段名
        * @return BOOL型字段值
        */
        const BOOL  GetFieldValueBoolean(const CHAR* pszFieldName)
        {
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return FALSE;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueBoolean(m_mapFieldName[pszFieldName]);
            }
            return FALSE;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(DateTime类型)
        * @param nIndex : 字段的索引位置
        * @return DateTime类型字段值
        */
        const CSDDateTime  GetFieldValueDatetime(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(DateTime型)
        * @param pszFieldName : 字段名
        * @return DateTime型字段值
        */
        const CSDDateTime  GetFieldValueDatetime(const CHAR* pszFieldName)
        {
            CSDDateTime t;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return t;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueDatetime(m_mapFieldName[pszFieldName]);
            }
            return t;
        }

        /**
        * @brief 获取所指列的字节数
        * @param nIndex : 字段的索引位置
        * @return 字节数
        */
		const INT32  GetFieldBytes(UINT32 nIndex);

        /**
        * @brief 获取当前记录行中某一个字段的名字
        * @param nIndex : 字段的索引位置
        * @return 字段名
        */
		const CHAR*  GetFieldName(UINT32 nIndex);

        /**
        * @brief 根据字段名获取字段值（std::string类型)<注意：通用接口，从数据库中取出的数据都为string>
        * @param pszFieldName : 字段名
        * @return std::string类型的字段值
        */
        const std::string GetFieldValueByName(const CHAR* pszFieldName)
        {
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return NULL;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(pszFieldName);
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValue(m_mapFieldName[pszFieldName]);
            }
            return NULL;
        }

    protected:

        /**
        * @brief  填充字段名与其索引的映射关系
        * @return TRUE:填充成功 FALSE:填充失败
        */
        BOOL	_FillMap()
        {
            if(NULL == m_poRes)
            {
                return FALSE;
            }
            UINT32 nCount = GetFieldCount();
            for(UINT32 i=0;i<nCount;i++)
            {
                m_mapFieldName[GetFieldName(i)] = i;
            }
            return TRUE;
        }
	protected:
		sqlite3_stmt*   m_poRes;
        CNameToIndexMap m_mapFieldName;
	};

/**
@brief 封装Sqlite3的连接
*
*功能	:实现了sqlite3的连接操作
*/
	class CSqlite3Conn
	{
	public:

		/**
		* @brief 构造函数，初始化sqlite连接对象，数据库名字
		*/
		CSqlite3Conn();

		/**
		* @brief 析构函数，关闭sqlite连接
		*/
		~CSqlite3Conn();

		/**
        * @brief 打开连接
        * @param pszDBName : 数据库名
        * @return TRUE:连接成功 FALSE:连接失败
        */
		BOOL  Connect(const CHAR* pszDBName);

		/**
        * @brief 关闭连接
        * @return VOID
        */
		VOID  Close();

		/**
        * @brief 是否已连接
        * @return TRUE:已经连接 FALSE:没连接
        */
		BOOL  IsConnected();

		/**
        * @brief 执行一条SQL语句，不返回记录集
        * @param pszSQL : SQL语句
        * @return TRUE:执行成功 FALSE:执行SQL语句失败
        */
        BOOL  Execute(const CHAR* pszSQL);
        
        /**
        * @brief 执行一条SQL语句，返回记录集
        * @param pszSQL : SQL语句
        * @param rcdSet : sqlite记录集
        * @return TRUE:执行成功 FALSE:执行SQL语句失败
        */
        BOOL  Execute(const CHAR* pszSQL, CSqlite3RecordSet* rcdSet);

        /**
        * @brief 处理二进制数据(../../二进制数据列的索引值（从1开始递增）/二进制源数据/二进制源数据的字节数)
        * @param pszSQL : SQL语句
        * @param rcdSet : sqlite记录集
        * @param nIndex : 字段索引位置
        * @param pData  : 取得的二进制数据缓冲区
        * @param nLength: 二进制数据缓冲区长度
        * @return TRUE:执行成功 FALSE:执行SQL语句失败
        */
		BOOL  ExecuteEx(const CHAR* pszSQL,CSqlite3RecordSet* rcdSet,UINT32 nIndex,VOID* pData,INT32 nLength);

	private:
		CHAR        m_szDBName[SDSQLITE_MAX_NAME_SIZE];
		sqlite3*    m_poConnection; 
		BOOL        m_bConnected;
	};


    /*
    //sqlite3db的内联函数
    */
    //获取字段个数
    inline  UINT32 CSqlite3RecordSet::GetFieldCount()
    {
        return sqlite3_column_count(m_poRes);
    }
    //**根据类型分别获取对应字段的值**//
    //先判断当前记录行中某一列的字段类型（1-int,2-double,3-unsigned char*,4-void *,5-空字符）
    inline UINT32 CSqlite3RecordSet::GetFieldType(UINT32 nIndex)
    {
        return sqlite3_column_type(m_poRes,nIndex);
    }		

    inline	const INT64 CSqlite3RecordSet::GetFieldValueInt64(UINT32 nIndex)
    {
        return sqlite3_column_int64(m_poRes,nIndex);
    }

    inline  const INT32 CSqlite3RecordSet::GetFieldValueInt32(UINT32 nIndex)
    {
        return sqlite3_column_int(m_poRes,nIndex);
    }

    inline const DOUBLE CSqlite3RecordSet::GetFieldValueDouble(UINT32 nIndex)
    {
        return sqlite3_column_double(m_poRes,nIndex);
    }

    inline const UCHAR* CSqlite3RecordSet::GetFieldValueString(UINT32 nIndex)
    {
        return sqlite3_column_text(m_poRes,nIndex);
    }

    inline const VOID* CSqlite3RecordSet::GetFieldValueBinary(UINT32 nIndex,UINT32 nLength)
    {
        nLength = sqlite3_column_bytes(m_poRes,nIndex);//二进制数据的长度
        return sqlite3_column_blob(m_poRes,nIndex);
    }

    inline const BOOL CSqlite3RecordSet::GetFieldValueBoolean(UINT32 nIndex)
    {
        if(SDStrcmp(GetFieldValue(nIndex).c_str(),"true")==0)
            return TRUE;
        else if(SDStrcmp(GetFieldValue(nIndex).c_str(),"false")==0)
            return FALSE;
        return FALSE;
    }

    //获取日期时间值
    inline const CSDDateTime CSqlite3RecordSet::GetFieldValueDatetime(UINT32 nIndex)
    {
        CSDDateTime t;
        if( TRUE == SDTimeFromString(_SDTA2T(GetFieldValue(nIndex).c_str()),t) )
            return t;
        return t;
    }

    //获取所指列的字节数
    inline const INT32 CSqlite3RecordSet::GetFieldBytes(UINT32 nIndex)
    {
        INT32 lenth = sqlite3_column_bytes(m_poRes,nIndex);//数据的长度
        return lenth;
    }
    //获取当前记录行中某一个字段的名字
    inline const CHAR* CSqlite3RecordSet::GetFieldName(UINT32 nIndex)
    {
        return sqlite3_column_name(m_poRes,nIndex);
    }

};

#endif

