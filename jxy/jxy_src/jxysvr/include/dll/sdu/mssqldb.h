
#ifndef _MSSQLDB_INCLUDE_H_
#define _MSSQLDB_INCLUDE_H_

#if (defined(WIN32) || defined(WIN64))
#include <sdtype.h>
#include <string>
#include <sdstring.h>
#include <comutil.h>
#import "c:\program files\common files\system\ado\msado25.tlb" no_namespace rename ("EOF", "adoEOF")


namespace SGDP
{
class CMSSQLConnection;

/**
@brief 封装ADO中_CommandPtr对象，主要用于配合CMSSQLConnection执行存储过程
*
* CMSSQLCommand封装了ADO中的Command对象，不可由多个线程同时操作本对象实例
* 主要用于配合CMSSQLConnection执行存储过程
* 创建CMSSQLCommand时需要传入CMSSQLConnection的对象指针
* 在首次调用AddParameter时需要确保参数列表已清空
* 由调用方负责捕获_com_error 异常
*/

class CMSSQLCommand
{
    friend class CMSSQLConnection;
public:
    /**
    * @brief 构造函数
    */
    CMSSQLCommand()
    {
        m_pCommand.CreateInstance(__uuidof(Command));
    }

    /**
    * @brief 析构函数
    */
    ~CMSSQLCommand()
    {
    }

    /**
    * @brief 设置Command Type
    * @param Type : 指定的命令类型
    */
    VOID SetCommandType(enum CommandTypeEnum Type)
    {
        m_pCommand->CommandType = Type;
    }

    /**
    * @brief 设置Command 内容
    * @param Type : 指定的命令内容
    */
    VOID SetCommandText(const CHAR* pszText)
    {
        m_pCommand->CommandText = pszText;
    }

    /**
    * @brief 增加命令中的一个参数
    * @param Type : 参数数据类型
    * @param Direction : 参数方向
    * @param Size : 参数长度
    * @param Value : 参数值
    */
    VOID AddParameter(const CHAR* Name, enum DataTypeEnum Type, enum ParameterDirectionEnum Direction,
                      long Size, const _variant_t& Value = vtMissing)
    {
        m_pCommand->Parameters->Append(m_pCommand->CreateParameter(Name, Type, Direction, Size, Value));
    }

    /**
    * @brief 获取某个参数的返回值
    * @param Name : 指定的命令内容
    */
    _variant_t GetItemValue(const CHAR* Name)
    {
        return m_pCommand->Parameters->GetItem(Name)->GetValue();
    }
    /**
    * @brief 清除所有参数，目前未提供
    */
    void ClearParameters()
    {
        for (long i = m_pCommand->Parameters->Count - 1; i >= 0; i--)
            m_pCommand->Parameters->Delete(i);
    }
    /**
    * @brief 删除一个参数
    * @param Name : 指定的命令内容
    */
    void DeleteParameter(const CHAR* Name)
    {
        m_pCommand->Parameters->Delete(Name);
    }

private:
    _CommandPtr m_pCommand;   ///< 命令对象
};

/**
@brief 封装ADO中_RecordsetPtr对象
*
*	封装ADO中_RecordsetPtr对象，不可由多个线程同时操作本对象实例
*  由调用方负责捕获_com_error 异常
*/

class CMSSQLRecordSet
{
    friend class CMSSQLConnection;
public:
    /**
    * @brief 构造函数
    */
    CMSSQLRecordSet() {}
    /**
    * @brief 析构函数
    */
    ~CMSSQLRecordSet() {};

    /**
    * @brief 移动到记录集首部
    */
    void MoveFirst()
    {
        if (m_pRecordSet)  m_pRecordSet->MoveFirst();
    }
    /**
    * @brief 移动到记录集下一行
    */
    void MoveNext()
    {
        if (m_pRecordSet) m_pRecordSet->MoveNext();
    }

    ///移动到记录集上一行
    void MovePrevious()
    {
        if (m_pRecordSet)  m_pRecordSet->MovePrevious();
    }

    /**
    * @brief 移动到记录集尾部
    */
    void MoveLast()
    {
        if (m_pRecordSet) m_pRecordSet->MoveLast();
    }

    /**
    * @brief 是否越过记录集末尾
    */
    BOOL IsEof()
    {
        if (m_pRecordSet)
            return m_pRecordSet->adoEOF;
        else
            return FALSE;
    }
    /**
    * @brief 是否越过记录集首部
    */
    BOOL IsBof()
    {
        if (m_pRecordSet)
            return m_pRecordSet->BOF;
        else
            return FALSE;
    }

    /**
    * @brief 得到记录条数
    */
    DWORD GetRecordCount()
    {
        DWORD nRows = 0;

        nRows = (DWORD)m_pRecordSet->GetRecordCount();

        if(nRows == -1)
        {
            nRows = 0;
            if(m_pRecordSet->adoEOF != VARIANT_TRUE)
                m_pRecordSet->MoveFirst();

            while(m_pRecordSet->adoEOF != VARIANT_TRUE)
            {
                nRows++;
                m_pRecordSet->MoveNext();
            }
            if(nRows > 0)
                m_pRecordSet->MoveFirst();
        }

        return nRows;
    }
    /**
    * @brief 获取某一字段的值
    */
    _variant_t GetFieldValue(const CHAR* pszFieldName)
    {
        _variant_t vtResult;
        if (m_pRecordSet)
            vtResult = m_pRecordSet->Fields->GetItem(pszFieldName)->Value;
        else
            vtResult.vt = VT_NULL;
        return vtResult;
    }

    ///
    /**
    * @brief 得到记录集中字段的个数
    */
    DWORD GetFieldCount()
    {
        if (m_pRecordSet)
            return m_pRecordSet->Fields->Count;
        else
            return 0;
    }
private:
    _RecordsetPtr m_pRecordSet;         ///< 记录集
};


/**
@brief 封装ADO中_ConnectionPtr对象
*
*  封装ADO中_ConnectionPtr对象，不可由多个线程同时操作本对象实例
*  内建了_com_error异常捕获，当返回FALSE时GetLastErrorInfo可获得错误描述
*/

class CMSSQLConnection
{
    friend class CMSSQLCommand;
    friend class CMSSQLRecordSet;
public:
    /**
    * @brief 构造函数
    */
    CMSSQLConnection(void);
    /**
    * @brief 析构函数
    */
    ~CMSSQLConnection(void);

    /**
    * @brief 设置连接字符串
    */
    void SetConnectionString(const CHAR* pszConnectionString)
    {
        m_strConnectionString = pszConnectionString;
    }

    /**
    * @brief 打开连接
    */
    BOOL Connect(const CHAR* pszConnectionString);

    /**
    * @brief 打开连接
    */
    BOOL Connect();
    /**
    * @brief 关闭连接
    */
    void Close();

    /**
    * @brief 执行一个CMSSQLCommand对象，并返回记录集
    */
    BOOL Execute(CMSSQLCommand& command, CMSSQLRecordSet& pSet);

    /**
    * @brief 执行一个CMSSQLCommand对象，不返回记录集
    */
    BOOL Execute(CMSSQLCommand& command);

    /**
    * @brief 执行一条SQL语句，并返回记录集
    */
    BOOL Execute(CHAR* pszSQL, CMSSQLRecordSet& pSet);

    /**
    * @brief 执行一条SQL语句，不返回记录集
    */
    BOOL Execute(CHAR* pszSQL);

    /**
    * @brief 是否已连接
    */
    BOOL IsConnected();

    ///获取上一个错误号
    DWORD GetLastError()
    {
        return m_dwErrCode;
    }

    ///获取上一个错误描述
    const char* GetLastErrorInfo()
    {
        return m_strErrInfo.c_str();
    }

protected:

private:
    //driver=sql Server;PWD=sa;UID=sa;database=db;server=127.0.0.1
    std::string     m_strConnectionString;
    _ConnectionPtr  m_pDBConnection;
    DWORD           m_dwErrCode;
    std::string     m_strErrInfo;
    void SetLastError(DWORD dwErrCode)
    {
        m_dwErrCode = dwErrCode;
    }
    void SetLastError(_com_error& e)
    {
        m_dwErrCode = e.WCode();
        m_strErrInfo = e.Description();
    }
    void SetLastErrorInfo(const CHAR* pszErrInfo)
    {
        m_strErrInfo = pszErrInfo;
    }
    inline void TESTHR(HRESULT x)
    {
        if FAILED(x) _com_issue_error(x);
    }
};

};
#endif
#endif

