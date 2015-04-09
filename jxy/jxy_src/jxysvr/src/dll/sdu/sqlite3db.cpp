#include "sqlite3db.h"

namespace SGDP
{

    CSqlite3Conn::CSqlite3Conn()
    {
        m_poConnection = NULL;
        memset(m_szDBName, 0, sizeof(m_szDBName));
        m_bConnected = FALSE;
    }

    CSqlite3Conn::~CSqlite3Conn()
    {
        Close();
    }

    BOOL CSqlite3Conn::Connect(const CHAR* pszDBName)
    {
        strncpy(m_szDBName, pszDBName, sizeof(m_szDBName) - 1);
        INT32 rc = sqlite3_open(SDLocal2UTF8(pszDBName).c_str(),&m_poConnection);
        if(rc != 0)
        {
            fprintf(stderr,"Can't open %s\n",sqlite3_errmsg(m_poConnection));
            sqlite3_close(m_poConnection);
            return FALSE;
        }
        m_bConnected = TRUE;
        return TRUE;
    }

    VOID CSqlite3Conn::Close()
    {
        if (m_poConnection)
        {
            sqlite3_close(m_poConnection);
            m_poConnection = NULL;
        }
        m_bConnected = FALSE;
    }

    BOOL CSqlite3Conn::IsConnected()
    {
        return m_bConnected;
    }

    BOOL CSqlite3Conn::Execute(const CHAR* pszSQL)
    {
        if(m_bConnected == FALSE)
        {
            return FALSE;
        }
        CHAR* szErrMsg = NULL;
        INT32 rc = sqlite3_exec(m_poConnection,pszSQL,0,0,&szErrMsg);
        if(rc != SQLITE_OK)
        {
            fprintf(stderr,"SQL error :%s \n",szErrMsg);
            sqlite3_free(szErrMsg);
            return FALSE;
        }
        return TRUE;
    }

    BOOL CSqlite3Conn::Execute(const CHAR* pszSQL, CSqlite3RecordSet* rcdSet)
    {
        if(m_bConnected == FALSE)
        {
            return FALSE;
        }

        sqlite3_prepare_v2(m_poConnection,pszSQL,-1,&(rcdSet->m_poRes),0);
        return TRUE;
    }

    BOOL CSqlite3Conn::ExecuteEx(const CHAR* pszSQL, CSqlite3RecordSet* rcdSet, UINT32 nIndex, VOID* pData, INT32 nLength)
    {
        Execute(pszSQL,rcdSet);//二进制数据插入处理的sql语句中会出现"?"
        sqlite3_bind_blob(rcdSet->m_poRes,nIndex,pData,nLength,NULL);//nIndex表示第几个问号
        INT32 rc = sqlite3_step(rcdSet->m_poRes);//bind之后一定要调用，否则插入不进数据库
        if(rc == SQLITE_BUSY)
        {
            fprintf(stderr,"SQL error :Sqlite busy!\n");
            return FALSE;
        }
        sqlite3_finalize(rcdSet->m_poRes);
        return (rc == SQLITE_ROW);
    }
}

