#include "dbconfig.h"
#include <framework/gsconfig.h>
#include <sdstring.h>

IMPLEMENT_SINGLETON(CDBConfig)

CDBConfig::CDBConfig()
{
    memset(m_szPath , 0, sizeof(m_szPath));
    m_poConnection = NULL;
    m_bStatus = FALSE;
}

CDBConfig::~CDBConfig()
{

}

// 初始化，从数据源获取数据
BOOL CDBConfig::Init()
{
    CGSConfig::Instance()->GetDBConfig(m_oDBAccount.m_szHostName, 
        m_oDBAccount.m_wConnPort,
        m_oDBAccount.m_szLoginName,
        m_oDBAccount.m_szLoginPwd,
        m_oDBAccount.m_szDBName,
        m_oDBAccount.m_szCharactSet,
		m_oDBAccount.m_szUnixSocket);
    m_oDBAccount.m_wDBType = SGDP::SDDB_DBTYPE_MYSQL;
    //// 获取SQLite数据库文件地址
    //CGSConfig::Instance()->GetConfigDataPath(m_szPath);
    //// 连接SQLite数据库
    //m_poConnection = SDNew CSqlite3Conn();
    //m_poConnection->Connect(m_szPath);
    //if(FALSE == m_poConnection->IsConnected())
    //{
    //    // 连接数据库失败
    //    return FALSE;
    //}

    // 执行成功
    m_bStatus = TRUE;
    return TRUE;
}

// 卸载
VOID CDBConfig::UnInit()
{
    memset(&m_oDBAccount, 0 , sizeof(SGDP::SDDBAccount));
    m_poConnection->Close();
    m_poConnection = NULL;
    m_bStatus = FALSE;
}

// 获取数据库连接信息对象
SGDP::SDDBAccount* CDBConfig::GetDBAccount()
{
    if(FALSE == m_bStatus)
    {
        return NULL;
    }
    else
    {
        return &m_oDBAccount;
    }
}

// 获取配置SQLite数据文件路径
const CHAR* CDBConfig::GetConfigDataPath()
{
    return m_szPath;
}

// 获取SQLite数据库连接
//CSqlite3Conn* CDBConfig::GetConnection()
//{
//    return m_poConnection;
//}

