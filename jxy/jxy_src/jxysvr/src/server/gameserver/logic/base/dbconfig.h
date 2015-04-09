
#ifndef _DBCONFIG_H_
#define _DBCONFIG_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sqlite3db.h>

using namespace SGDP;

/**
* @brief 数据库连接配置信息访问类
*/
class CDBConfig
{
    DECLARE_SINGLETON(CDBConfig);

public:
    /**
     * @brief 初始化，从数据源获取数据
     * @return 初始化结果
     */
    BOOL Init();

    /**
     * @brief 卸载
     * @return 卸载结果
     */
    VOID UnInit();

    /**
     * @brief 获取数据库连接信息对象
     * @return 数据库连接信息对象的指针，失败返回NULL
     */
    SGDP::SDDBAccount* GetDBAccount();

    /**
     * @brief 获取配置SQLite数据文件路径
     * @return 配置SQLite数据文件路径
     */
    const CHAR* GetConfigDataPath();

    /**
     * @brief 获取SQLite数据库连接
     * @return SQLite数据库连接
     */
    //CSqlite3Conn* GetConnection();

protected:
    CDBConfig();
    virtual ~CDBConfig();

protected:
    SGDP::SDDBAccount   m_oDBAccount;   ///< 数据库连接信息
    CHAR                m_szPath[256];  ///< 配置SQLite数据文件路径
    BOOL                m_bStatus;      ///< 数据是否可用
    CSqlite3Conn*       m_poConnection; ///< SQLite数据库连接
};

#endif // #ifndef _DBCONFIG_H_

