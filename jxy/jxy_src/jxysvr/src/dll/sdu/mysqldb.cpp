#include "mysqldb.h"
#include "sdstring.h"
#include "sddebug.h"
#pragma warning(disable:4996)

namespace SGDP{
    const static CHAR* g_stpCharSets[] =
    {
        "big5",
        "dec8",
        "cp850",
        "hp8",
        "koi8r",
        "latin1",
        "latin2",
        "swe7",
        "ascii",
        "ujis",
        "sjis",
        "hebrew",
        "tis620",
        "euckr",
        "koi8u",
        "gb2312",
        "greek",
        "cp1250",
        "gbk",
        "latin5",
        "armscii8",
        "utf8",
        "ucs2",
        "cp866",
        "keybcs2",
        "macce",
        "macroman",
        "cp852",
        "latin7",
        "cp1251",
        "cp1256",
        "cp1257",
        "binary",
        "geostd8",
        "cp932",
        "eucjpms",
        NULL
    };
    CMySQLConnection::CMySQLConnection()
    {
        memset(m_szServerAddr, 0, sizeof(m_szServerAddr));
        memset(m_szDBName, 0, sizeof(m_szDBName));
        memset(m_szUserID, 0, sizeof(m_szUserID));
        memset(m_szPassword, 0, sizeof(m_szPassword));
        SGDP::SDStrncpy(m_szCharset, "latin1",sizeof(m_szCharset));
        m_poMyConnection = mysql_init(NULL);

        m_bConnected   = FALSE;
    }

    CMySQLConnection::~CMySQLConnection()
    {
        if (m_poMyConnection)
        {
            mysql_close(m_poMyConnection);
        }
    }

    VOID CMySQLConnection::SetConnectionString(const CHAR* pszServerAddr, 
        INT32 nServerPort, 
        const CHAR* pszDBName,
        const CHAR* pszUserID, 
        const CHAR* pszPassword,
        const CHAR* pszCharset)
    {
        SGDP::SDStrncpy(m_szServerAddr, pszServerAddr, sizeof(m_szServerAddr));
        SGDP::SDStrncpy(m_szDBName, pszDBName, sizeof(m_szDBName));
        SGDP::SDStrncpy(m_szUserID, pszUserID, sizeof(m_szUserID));
        SGDP::SDStrncpy(m_szPassword, pszPassword, sizeof(m_szPassword));
        SGDP::SDStrncpy(m_szCharset, pszCharset, sizeof(m_szCharset));
        m_nServerPort = nServerPort ;
    }

    BOOL CMySQLConnection::Connect(const CHAR* pszServerAddr, 
        INT32 nServerPort, 
        const CHAR* pszDBName,
        const CHAR* pszUserID,
        const CHAR* pszPassword,
        const CHAR* pszCharset)
    {
        SetConnectionString(pszServerAddr, nServerPort, pszDBName, pszUserID, pszPassword, pszCharset);
        return Connect();
    }

    BOOL CMySQLConnection::Connect()
    {
        if (m_poMyConnection == NULL)
        {
            m_poMyConnection = mysql_init(NULL);
        }
        if (m_poMyConnection == NULL)
        {
            return FALSE;
        }

        if('\0' != m_szCharset[0])
        {
            if (FALSE == _IsValidCharacterSet(m_szCharset))
            {
                return FALSE;
            }
            if(mysql_options(m_poMyConnection, MYSQL_SET_CHARSET_NAME, m_szCharset))
            {
                return FALSE;
            }
        }


        MYSQL* myConn = mysql_real_connect(m_poMyConnection, 
            m_szServerAddr, 
            m_szUserID, 
            m_szPassword,
            m_szDBName,
            m_nServerPort,
            NULL,
            0);
        if (!myConn)
        {
            CHAR szErrMsg[128] = {0};
            sprintf(szErrMsg, "mysql_real_connect failed:%s", mysql_error(m_poMyConnection));
            Close();
            return FALSE;
        }
        m_bConnected = TRUE;

        //设置字符集
        if(FALSE == _SetServerNames())
        {
            return FALSE;
        }

        //获取服务器端的字符集
        if(FALSE == _GetDBSCharSetInfo())
        {
            return FALSE;
        }

        //检测设置后的字符集是否符合要求
        if(FALSE == _IsCompatiableCharSet())
        {
            return FALSE;
        }

        return TRUE;
    }

    VOID CMySQLConnection::Close()
    {
        mysql_close(m_poMyConnection);
        m_poMyConnection = NULL;
        m_bConnected   = FALSE;
    }

    BOOL CMySQLConnection::IsConnected()
    {
        if(!m_bConnected)
        {
            return  FALSE;
        }

        INT32 nReturn = mysql_ping(m_poMyConnection);
        return (nReturn==0);
    }

    INT32 CMySQLConnection::Ping()
    {
        return mysql_ping(m_poMyConnection);
    }
    BOOL CMySQLConnection::Execute(const CHAR* pszSQL)
    {
        return (0 == mysql_query(m_poMyConnection, pszSQL));
    }

    BOOL CMySQLConnection::Execute(const CHAR* pszSQL, CMySQLRecordSet& rcdSet)
    {
        if (0 != mysql_query(m_poMyConnection, pszSQL))
        {
            return FALSE;
        }

        if (rcdSet.m_poRes)
        {
            mysql_free_result(rcdSet.m_poRes); 
        }

        rcdSet.m_poRes = mysql_store_result(m_poMyConnection);
        return (rcdSet.m_poRes != NULL);
    }

    CMySQLConnection::ESDMYSQLDBCode CMySQLConnection::ExecuteEx(const CHAR* pszSQL, CMySQLRecordSet& rcdSet)
    {
        if (0 != mysql_query(m_poMyConnection, pszSQL))
        {
            return SDMYSQLDB_ERR_CONN;
        }
        if (rcdSet.m_poRes)
        {
            mysql_free_result(rcdSet.m_poRes); 
        }

        rcdSet.m_poRes = mysql_store_result(m_poMyConnection);
        if(rcdSet.m_poRes != NULL)
        {
            return SDMYSQLDB_HAS_RECORDSET;       ///Rescord Set
        }

        if(!mysql_field_count(m_poMyConnection))
        {
            return  SDMYSQLDB_NO_RECORDSET;       ///NO Rescord Set
        }

        return SDMYSQLDB_SUCCESS;
    }

    CMySQLConnection::ESDMYSQLDBCode CMySQLConnection::ExecuteEx(const CHAR* pszSQL, INT32 nLength,CMySQLRecordSet& rcdSet)
    {        
        if(0 != mysql_real_query(m_poMyConnection, pszSQL,nLength))
        {
            return SDMYSQLDB_ERR_CONN;
        }
        if(rcdSet.m_poRes)
        {
            mysql_free_result(rcdSet.m_poRes); 
        }

        rcdSet.m_poRes = mysql_store_result(m_poMyConnection);
        if(rcdSet.m_poRes != NULL)
        {
            return SDMYSQLDB_HAS_RECORDSET;       ///Rescord Set
        }

        if(!mysql_field_count(m_poMyConnection))
        {
            return  SDMYSQLDB_NO_RECORDSET;       ///NO Rescord Set
        }
        return SDMYSQLDB_SUCCESS;
    }

    BOOL CMySQLConnection::_GetDBSCharSetInfo()
    {
#if 0 //  [老版本Mysql没有character_set_system变量 ]
        //WARING:　sql 语句中变量的顺序一定和枚举定义一致
        CHAR* pszSql = "select @@character_set_client, @@character_set_connection,   \
                       @@character_set_database, @@character_set_results,@@character_set_server,\
                       @@character_set_system";
#else
        const CHAR*  pszSql = "show variables like 'character_%'";
#endif

        CMySQLRecordSet* poRs = SDNew CMySQLRecordSet();
        if(SDMYSQLDB_HAS_RECORDSET != ExecuteEx(pszSql, *poRs))
        {
            return FALSE;
        }

        while(poRs->GetRecord())
        {
            const CHAR* pszVariableName = poRs->GetFieldValueString((UINT32)0);
            const CHAR* pszCharsetName  = poRs->GetFieldValueString((UINT32)1);            

            m_oCharsets.insert(std::make_pair(std::string(pszVariableName), std::string(pszCharsetName)));
        }
        SDDelete poRs;

        Charsets::iterator i = m_oCharsets.begin();
        Charsets::iterator end = m_oCharsets.end();
        while(i != end)
        {
            ++i;
        }
        return TRUE;
    }

    BOOL CMySQLConnection::_IsCompatiableCharSet()
    {
        //检测兼容性
        //需要保证character_set_client和m_szCharactSet和（character_set_connection, character_set_results,
        //character_set_database, character_set_server）完全一致

        std::string strClientCharset;

        Charsets::iterator i = m_oCharsets.find(std::string("character_set_client"));
        if(i != m_oCharsets.end())
        {
            strClientCharset = i->second;
        }
        else
        {
            return FALSE;
        }

        i = m_oCharsets.find(std::string("character_set_connection"));
        if(i != m_oCharsets.end())
        {
            if(strClientCharset != i->second)
            {
                return FALSE;
            }
        }
        i = m_oCharsets.find(std::string("character_set_results"));
        if(i != m_oCharsets.end())
        {
            if(strClientCharset != i->second)
            {
                return FALSE;
            }
        }

        if(strClientCharset != std::string(m_szCharset))
        {
            return FALSE;
        }

        return TRUE;
    }

    //这里主要设置character_set_client, character_set_results, character_set_connection, 
    //使这三个字符集与客户端输入的字符集保持一致，这样，客户执行的SQL语句中的字符，就确保能够识别
    BOOL CMySQLConnection::_SetServerNames()
    {
        //WARING:　sql 语句中变量的顺序一定和枚举定义一致
        CHAR szSql[1024] = {0};
        sprintf(szSql, "set names %s", m_szCharset);

        if(Execute(szSql) == FALSE)
        {
            return FALSE;
        }
        return TRUE;
    }

#ifndef _WIN32
#define stricmp strcasecmp
#endif 
    //检测用户输入的编码集是否符合mysql的规范
    BOOL CMySQLConnection::_IsValidCharacterSet(const CHAR* pszSetName)
    {
        for(INT32 i = 0; g_stpCharSets[i] != NULL; i++)
        {
            if(0 == stricmp(pszSetName, g_stpCharSets[i]))
            {
                return TRUE;
            }
        }
        return FALSE;
    }
}

