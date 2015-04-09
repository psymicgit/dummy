#include "mssqldb.h"

#if (defined(WIN32) || defined(WIN64))
namespace SGDP
{

    CMSSQLConnection::CMSSQLConnection() : m_pDBConnection(NULL), m_dwErrCode(0)
    {

    }

    CMSSQLConnection::~CMSSQLConnection()
    {
        if (IsConnected())
            Close();
    }

    BOOL CMSSQLConnection::Connect(const CHAR* pszConnectionString)
    {
        SetConnectionString(pszConnectionString);
        return  Connect();
    }


    BOOL CMSSQLConnection::Connect()
    {
        if (m_strConnectionString.empty())
            return FALSE;
        try
        {
            if (m_pDBConnection == NULL)
            {
                if (FAILED(m_pDBConnection.CreateInstance(__uuidof(Connection))))
                    return FALSE;  //无法初始化ADO连接,请更新MDAC
            }
            TESTHR(m_pDBConnection->Open(_bstr_t(m_strConnectionString.c_str()), "", "", -1));
        }
        catch(_com_error& e)
        {
            SetLastError(e);
            return FALSE; 
        }

        return TRUE;
    }

    BOOL CMSSQLConnection::IsConnected()
    {
        if (m_pDBConnection == NULL)
            return FALSE;
        return (m_pDBConnection->State == adStateOpen);
    }

    void CMSSQLConnection::Close()
    {
        if (m_pDBConnection != NULL && m_pDBConnection->State != adStateClosed)
        {
            try
            {
                m_pDBConnection->Close();
            }
            catch(...)
            {
            }
        }
    }

    BOOL CMSSQLConnection::Execute(CMSSQLCommand& command)
    {
        try
        {
            command.m_pCommand->ActiveConnection = m_pDBConnection;
            command.m_pCommand->Execute(NULL, NULL, command.m_pCommand->CommandType);
        }
        catch(_com_error& e)
        {
            SetLastError(e);
            WORD wError = e.WCode();
            if(wError == 0)
                Close();
            return FALSE;
        }
        return TRUE;
    }

    BOOL CMSSQLConnection::Execute(CMSSQLCommand& command, CMSSQLRecordSet& pSet)
    {
        try
        {
            command.m_pCommand->ActiveConnection = m_pDBConnection;
            pSet.m_pRecordSet = command.m_pCommand->Execute(NULL, NULL, command.m_pCommand->CommandType);
        }
        catch(_com_error& e)
        {
            SetLastError(e);
            if(e.WCode() == 0)
                Close();
            return FALSE;
        }
        return TRUE;
    }

    BOOL CMSSQLConnection::Execute(CHAR* pszSQL, CMSSQLRecordSet& pSet)
    {
        try
        {
            pSet.m_pRecordSet = m_pDBConnection->Execute(_bstr_t(pszSQL), NULL, adCmdText);
        }
        catch(_com_error& e)
        {
            SetLastError(e);
            if(e.WCode() == 0)
                Close();
            return FALSE;
        }
        return TRUE;
    }

    BOOL CMSSQLConnection::Execute( CHAR* pszSQL)
    {
        try
        {
            m_pDBConnection->Execute(_bstr_t(pszSQL), NULL, adCmdText);
        }
        catch(_com_error& e)
        {
            SetLastError(e);
            if(e.WCode() == 0)
                Close();
            return FALSE;
        }
        return TRUE;
    }

}

#endif

