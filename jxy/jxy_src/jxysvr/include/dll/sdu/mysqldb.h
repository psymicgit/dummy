/******************************************************************************


本文件包含类CMySqlConnection的定义，mysql数据库不区分字段大小写

文件历史：
1.修改时间：	2011-07-25
修改描述：    创建文件 
修改者  ： 	xujun
……
******************************************************************************/

#ifndef _MYSQLDB_INCLUDE_H_
#define _MYSQLDB_INCLUDE_H_

#include <sdnetutils.h>
#include "mysql.h"
#include <sdstring.h>
#include <sdtime.h>
#include <map>

namespace SGDP
{
    /**
    * @brief 封装MySQL的记录集
    *
    *	封装MySQL的记录集，不可由多个线程同时操作本对象实例
    */

    class CMySQLRecordSet
    {
//         struct CmpObj
//         {
//             BOOL operator()(std::string pSrc, std::string pDst) const 
//             {
//                 return SDStrcmp(pSrc.c_str(), pDst.c_str()) < 0;
//             }
// 
//         }; 
        typedef std::map<std::string, INT32> CNameToIndexMap;              //存储名字与索引的映射关系
        typedef std::map<std::string, INT32>::iterator CNameToIndexMapItr;
        friend class CMySQLConnection;
    public:
        /*mysql支持的常用数据类型*/
        enum mysqltype
        {
            MYSQL_INT32=1,
            MYSQL_INT64,
            MYSQL_DOUBLE,
            MYSQL_BLOB,
            MYSQL_NULL,
            MYSQL_STRING};

	/**
	* @brief 构造函数：初始化mysql记录集和列
	*/
        CMySQLRecordSet(): m_poRes(NULL), m_oRow(NULL)
        {
        }

	/**
	* @brief 析构函数：释放mysql记录集
	*/
        ~CMySQLRecordSet()
        {
            if(m_poRes)
                mysql_free_result(m_poRes);
        }

        /**
        * @brief  获取记录个数
        * @return 记录个数
        */
        UINT32  GetRecordCount() ;

        /**
        * @brief  获取字段个数
        * @return 字段个数
        */
        UINT32  GetFieldCount();

        /**
        * @brief  获取下一个记录行
        * @return TRUE:存在下一个记录 FALSE:当前记录为最后一个记录
        */
        BOOL  GetRecord();

        /**
        * @brief  获取当前记录中指定索引位置字段的长度
        * @param nIndex : 字段的索引位置
        * @return 字段长度
        */
        const UINT32  GetFieldLenth(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定索引位置字段的类型
        * @param nIndex : 字段的索引位置
        * @return 字段类型
        */
        const UINT32  GetFieldType(UINT32 nIndex)
        {
            UINT32 num_poFields = GetFieldCount();
            if (nIndex >= num_poFields)
                return -1;

            m_poField = mysql_fetch_field(m_poRes);//获取字段的结构体信息
            UINT32 nTypeIndex = 0;
            if(m_poField[nIndex].type == MYSQL_TYPE_TINY||MYSQL_TYPE_SHORT||
                MYSQL_TYPE_LONG||MYSQL_TYPE_INT24)//4种整型(<32位）
            {
                nTypeIndex = MYSQL_INT32; 
            }
            else if(m_poField[nIndex].type == MYSQL_TYPE_LONGLONG)//大整型（64位）
            {
                nTypeIndex = MYSQL_INT64;
            }
            else if(m_poField[nIndex].type == MYSQL_TYPE_DECIMAL||MYSQL_TYPE_NEWDECIMAL
                ||MYSQL_TYPE_FLOAT||MYSQL_TYPE_DOUBLE)//浮点型
            {
                nTypeIndex = MYSQL_DOUBLE;
            }
            else if(m_poField[nIndex].type == MYSQL_TYPE_TINY_BLOB||MYSQL_TYPE_MEDIUM_BLOB
                ||MYSQL_TYPE_LONG_BLOB||MYSQL_TYPE_BLOB)//二进制类型
            {
                nTypeIndex = MYSQL_BLOB;
            }
            else if(m_poField[nIndex].type == MYSQL_TYPE_NULL)//空类型
            {
                nTypeIndex = MYSQL_NULL;
            }
            else
                nTypeIndex = MYSQL_STRING;//默认字符串类型
            return nTypeIndex;
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
        const INT32  GetFieldValueInt32(CHAR* pszFieldName)
        {
            std::string tmpStr = pszFieldName;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return 0;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(SDStrlwr(tmpStr));
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueInt32(m_mapFieldName[SDStrlwr(tmpStr)]);
            }
            return 0;
        }

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
        const INT64  GetFieldValueInt64(CHAR* pszFieldName)
        {
            std::string tmpStr = pszFieldName;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return 0;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(SDStrlwr(tmpStr));
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueInt64(m_mapFieldName[SDStrlwr(tmpStr)]);
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
        const DOUBLE  GetFieldValueDouble(CHAR* pszFieldName)
        {
            std::string tmpStr = pszFieldName;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return 0;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(SDStrlwr(tmpStr));
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueDouble(m_mapFieldName[SDStrlwr(tmpStr)]);
            }
            return 0;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(字符串型)
        * @param nIndex : 字段的索引位置
        * @return 字符串型字段值
        */
        const CHAR*  GetFieldValueString(UINT32 nIndex);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(字符串型)
        * @param pszFieldName : 字段名
        * @return 字符串型字段值
        */
        const CHAR*  GetFieldValueString(CHAR* pszFieldName)
        {
            std::string tmpStr = pszFieldName;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return NULL;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(SDStrlwr(tmpStr));
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueString(m_mapFieldName[SDStrlwr(tmpStr)]);
            }
            return NULL;
        }

        /**
        * @brief  获取当前记录中指定索引位置字段的值(二进制类型)
        * @param nIndex : 字段的索引位置
        * @param nLength      : 二进制数据的长度
        * @return 二进制类型字段值
        */
        const VOID*  GetFieldValueBinary(UINT32 nIndex,UINT32& nLength);

        /**
        * @brief  获取当前记录中指定字段名对应的字段值(二进制类型)
        * @param pszFieldName : 字段名
        * @param nLength      : 二进制数据的长度
        * @return 二进制型字段值
        */
        const VOID*  GetFieldValueBinary(CHAR* pszFieldName,UINT32& nLength)
        {
            std::string tmpStr = pszFieldName;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return NULL;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(SDStrlwr(tmpStr));
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueBinary(m_mapFieldName[SDStrlwr(tmpStr)],nLength);
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
        const BOOL  GetFieldValueBoolean(CHAR* pszFieldName)
        {
            std::string tmpStr = pszFieldName;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return FALSE;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(SDStrlwr(tmpStr));
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueBoolean(m_mapFieldName[SDStrlwr(tmpStr)]);
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
        const CSDDateTime  GetFieldValueDatetime(CHAR* pszFieldName)
        {
            CSDDateTime t;
            std::string tmpStr = pszFieldName;
            // 初次调用时，填充Map，以后每次检测Map是否有值。
            if(0 == m_mapFieldName.size())
            {
                if (FALSE == _FillMap())
                {
                    return t;
                }
            }
            CNameToIndexMapItr itrFind;
            itrFind= m_mapFieldName.find(SDStrlwr(tmpStr));
            if(itrFind != m_mapFieldName.end())
            {
                return this->GetFieldValueDatetime(m_mapFieldName[SDStrlwr(tmpStr)]);
            }
            return t;
        }

        /**
        * @brief  获取当前记录行中某一个字段的名字
        * @param nIndex : 字段的索引位置
        * @return 字段名字
        */
        const CHAR* GetFieldName(UINT32 nIndex);
    private:
    
        /**
        * @brief  填充字段名与其索引的映射关系 <!注意:map中存入的字段名都是小写的>
        * @return TRUE:填充成功 FALSE:填充失败
        */
        BOOL _FillMap()
        {
            if(NULL == m_poRes)
            {
                return FALSE;
            }
            MYSQL_FIELD* field = NULL;
            INT32 nIndex = 0;
            while(field = mysql_fetch_field(m_poRes))
            {
                std::string tmpStr = field->name;
                m_mapFieldName[SDStrlwr(tmpStr)] = nIndex;
                ++nIndex;
            }
            return TRUE;
        }

    private:
        MYSQL_RES*  m_poRes;
        MYSQL_ROW   m_oRow;       
        MYSQL_FIELD* m_poField;
        CNameToIndexMap m_mapFieldName;
    };

    /**
    @brief 封装MySQL的连接
    *
    *	封装MySQL的连接，不可由多个线程同时操作本对象实例
    */

    class CMySQLConnection
    {
#define SDMYSQLDB_MAX_NAME_SIZE 64
        enum ESDMYSQLDBCode
        {
            SDMYSQLDB_ERR_UNKNOWN    = -999, // 未知错误
            SDMYSQLDB_ERR_CONN       = -2,   // 连接错误
            SDMYSQLDB_DISCONNECT     = -1,   // 断开与服务器的连接
            SDMYSQLDB_SUCCESS        = 0,    // 操作成功
            SDMYSQLDB_NO_RECORDSET   = 1,    // 操作成功，但没有结果集
            SDMYSQLDB_HAS_RECORDSET  = 2     // 操作成功，有结果集
        };
    public:

	/**
	* @brief 构造函数
	*/
        CMySQLConnection();

	/**
	* @brief 析构函数，关闭mysql连接
	*/
        ~CMySQLConnection();

        /**
        * @brief 设置连接信息
        * @param pszServerAddr : mysql数据库服务器地址
        * @param nServerPort   : mysql数据库服务器端口
        * @param pszDBName     : mysql数据库名
        * @param pszUserID     : mysql数据库用户名
        * @param pszPassword   : mysql数据库密码
        * @param pszCharset    : mysql数据库字符集
        * @return VOID
        */
        VOID  SetConnectionString(const CHAR* pszServerAddr, INT32 nServerPort, 
            const CHAR* pszDBName, const CHAR* pszUserID, const CHAR* pszPassword,
            const CHAR* pszCharset);

        /**
        * @brief 打开连接
        * @param pszServerAddr : mysql数据库服务器地址
        * @param nServerPort   : mysql数据库服务器端口
        * @param pszDBName     : mysql数据库名
        * @param pszUserID     : mysql数据库用户名
        * @param pszPassword   : mysql数据库密码
        * @param pszCharset    : mysql数据库字符集
        * @return TRUE:连接成功 FALSE:连接失败
        */
        BOOL  Connect(const CHAR* pszServerAddr, INT32 nServerPort, 
            const CHAR* pszDBName, const CHAR* pszUserID, const CHAR* pszPassword,
            const CHAR* pszCharset);

        /**
        * @brief 打开连接，在设置连接信息后调用
        * @return TRUE:连接成功 FALSE:连接失败
        */
        BOOL  Connect();

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
        * @brief mysql_ping的封装
        * @return ping值
        */
        INT32  Ping();

        /**
        * @brief 转义字符串，使之可用于MYSQL查询
        * @param pszSrc : 源字符串
        * @param nSize  : 字符串长度
        * @param pszDest: 目的字符串
        * @return 如果成功转换，则返回pszDest;否则返回NULL
        */
        CHAR*  EscapeString(const CHAR* pszSrc, INT32 nSize, CHAR* pszDest)
        {		
            if (m_poMyConnection)
            {
                mysql_real_escape_string(m_poMyConnection, pszDest, pszSrc, nSize);
                return pszDest;
            }
            else
                return NULL;
        }

        /**
        * @brief 返回收到最后一个UPDATE、DELETE或INSERT查询影响(变化)的行数
        * @return 受影响的行数
        */
        UINT32  GetAffectedRows() 
        { 
            return (UINT32)mysql_affected_rows(m_poMyConnection); 
        }

        /**
        * @brief 转义字符串，使之可用于MYSQL查询
        * @param pszSrc : 源字符串
        * @param pszDest: 目的字符串
        * @return 转义字符串
        */
        CHAR*  EscapeString(const CHAR* pszSrc, CHAR* pszDest)
        {
            return EscapeString(pszSrc, (INT32)strlen(pszSrc), pszDest);
        }

        /**
        * @brief 转义字符串，使之可用于MYSQL查询，主要用在转意2进制数据
        * @param pszSrc : 源字符串
        * @param nSize　: 源字符串大小
        * @param pszDest: 目的字符串
        * @return 0:转换失败 非0:转义成功
        */
        UINT32  EscapeStringEx(const CHAR* pszSrc, INT32 nSize, CHAR* pszDest)
        {
            if (m_poMyConnection)
            {
                return mysql_real_escape_string(m_poMyConnection, pszDest, pszSrc, nSize);
            }
            else
            {
                return 0;
            }
        }

        /**
        * @brief 无差别执行SQL语句，通过返回值判断具体效果
        * @param pszSQL : SQL语句
        * @param rcSet  : mysql记录集
        * @return ESDMYSQLDBCode(见枚举定义)
        */
        ESDMYSQLDBCode   ExecuteEx(const CHAR* pszSQL, CMySQLRecordSet& rcdSet);

        /**
        * @brief 无差别执行SQL语句，通过返回值判断具体效果(支持二进制代码数据)
        * @param pszSQL : SQL语句
        * @param nLength: 二进制数据长度
        * @param rcdSet : mysql记录集
        * @return ESDMYSQLDBCode(见枚举定义)
        */
        ESDMYSQLDBCode   ExecuteEx(const CHAR* pszSQL, INT32 nLength,CMySQLRecordSet& rcdSet);

        /**
        * @brief 执行一条SQL语句，不返回记录集
        * @param pszSQL : SQL语句
        * @return TRUE:执行成功 FALSE:执行SQL语句失败
        */
        BOOL  Execute(const CHAR* pszSQL);

        /**
        * @brief 执行一条SQL语句，返回记录集
        * @param pszSQL : SQL语句
        * @param rcdSet : mysql记录集
        * @return TRUE:执行成功 FALSE:执行SQL语句失败
        */
        BOOL  Execute(const CHAR* pszSQL, CMySQLRecordSet& rcdSet);

        /**
        * @brief 获取上一次错误号
        * @return 错误号
        */
        ULONG  GetLastError() 
        { 
            if(m_poMyConnection!=NULL)
            {
                return (ULONG)mysql_errno(m_poMyConnection);
            }
            return  0;
        }

        /**
        * @brief 获取上一次错误描述
        * @return 错误描述
        */
        const CHAR*  GetLastErrorInfo()
        { 
            if(m_poMyConnection!=NULL)
            {
                return mysql_error(m_poMyConnection);
            }
            return NULL;
        }

    private:

        /**
        * @brief 获取mysql数据库字符集信息
        * @return TRUE:获取成功 FALSE:获取失败
        */
        BOOL _GetDBSCharSetInfo();

        /**
        * @brief 检测字符集的兼容性
        * @return TRUE:兼容 FALSE:不兼容
        */
        BOOL _IsCompatiableCharSet();

        /**
        * @brief 设置服务器名字
        * @return TRUE:设置成功 FALSE:设置失败
        */
        BOOL _SetServerNames();

        /**
        * @brief 检测用户输入的编码集是否符合mysql的规范
        * @param pszSetName : 用户输入的编码集
        * @return TRUE:符合 FALSE:不符合
        */
        BOOL _IsValidCharacterSet(const CHAR* pszSetName);
        //

    private:

        CHAR    m_szServerAddr[SDMYSQLDB_MAX_NAME_SIZE];
        CHAR    m_szDBName[SDMYSQLDB_MAX_NAME_SIZE];
        CHAR    m_szCharset[SDMYSQLDB_MAX_NAME_SIZE];
        CHAR    m_szUserID[SDMYSQLDB_MAX_NAME_SIZE];
        CHAR    m_szPassword[SDMYSQLDB_MAX_NAME_SIZE];

        INT32   m_nServerPort;
        MYSQL*  m_poMyConnection;
        BOOL    m_bConnected;

        typedef std::map<std::string, std::string> Charsets;
        Charsets m_oCharsets;
    };

    /*
    //mysql内联函数
    */
    ///获取记录个数
    inline UINT32 CMySQLRecordSet::GetRecordCount() 
    { 
        return (m_poRes ? (UINT32)mysql_num_rows(m_poRes) : 0);
    }

    ///获取字段个数
    inline UINT32 CMySQLRecordSet::GetFieldCount() 
    { 
        return (m_poRes ? (UINT32)mysql_num_fields(m_poRes) : 0);
    }

    ///获取下一个记录行
    inline BOOL CMySQLRecordSet::GetRecord()
    {
        if (m_poRes == NULL)
            return FALSE;
        m_oRow = mysql_fetch_row(m_poRes);
        return (m_oRow != NULL);
    }

    //获取当前记录中某一个字段的长度
    inline const UINT32 CMySQLRecordSet::GetFieldLenth(UINT32 nIndex)
    {
        UINT32 num_poFields = GetFieldCount();
        if (nIndex >= num_poFields)
            return 0;
        ULONG* lengths = mysql_fetch_lengths(m_poRes);
        if (NULL==lengths)
        {
            return 0;
        }
        return lengths[nIndex];
    }

    //获取整型列(char*转int32)
    inline const INT32 CMySQLRecordSet::GetFieldValueInt32(UINT32 nIndex)
    {
        return SDAtoi(m_oRow[nIndex]);
    }

    //获取整型列(char*转int64)
    inline const INT64 CMySQLRecordSet::GetFieldValueInt64(UINT32 nIndex)
    {
        return SDAtoi64(m_oRow[nIndex]); 
    }

    //获取浮点型列(char*转double)
    inline const DOUBLE CMySQLRecordSet::GetFieldValueDouble(UINT32 nIndex)
    {
        return SDStrtod(m_oRow[nIndex],NULL);
    }

    //获取字符型列
    inline const CHAR* CMySQLRecordSet::GetFieldValueString(UINT32 nIndex)
    {
        if (m_oRow == NULL)
            return NULL;
        if (nIndex >= GetFieldCount())
            return NULL;
        return m_oRow[nIndex];
    }

    //获取二进制数据列
    inline const VOID* CMySQLRecordSet::GetFieldValueBinary(UINT32 nIndex,UINT32& nLength)
    {
        nLength = GetFieldLenth(nIndex);
        VOID* bData = NULL;
        memcpy(bData,m_oRow[nIndex],GetFieldLenth(nIndex));
        return bData;
    }

    //获取bool值
    inline const BOOL CMySQLRecordSet::GetFieldValueBoolean(UINT32 nIndex)
    {
        if(SDStrcmp(m_oRow[nIndex],"true")==0)
            return TRUE;
        else if(SDStrcmp(m_oRow[nIndex],"false")==0)
            return FALSE;
        return FALSE;
    }

    //获取日期时间值
    inline const CSDDateTime CMySQLRecordSet::GetFieldValueDatetime(UINT32 nIndex)
    {
        CSDDateTime t;
        if( TRUE == SDTimeFromString(_SDTA2T(m_oRow[nIndex]),t) )
            return t;
        return t;
    }

    //获取当前记录行中某一个字段的名字
    inline const CHAR* CMySQLRecordSet::GetFieldName(UINT32 nIndex)
    {
        if (m_oRow == NULL)
            return NULL;
        if (nIndex >= GetFieldCount())
            return NULL;
        return m_poField[nIndex].name;
    }

};

#endif


