///<------------------------------------------------------------------------------
//< @file:   server\db\db.h
//< @author: 洪坤安
//< @date:   2015年3月26日 20:9:33
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _db_h_
#define _db_h_

class DBConnection;
typedef struct st_mysql MYSQL;
typedef struct st_mysql_stmt MYSQL_STMT;
class DBRecordSet;
class DBCommand;

// 数据库信息名称的最大长度
#define SDDB_MAX_NAME_SIZE		        64

// 数据库字符集名称的最大长度
#define SDDB_MAX_CHARACTSET_SIZE		32

namespace DB
{
	enum DBExecuteCode {
		EXECUTE_ERR_UNKNOWN		  = -999, ///< 未知错误
		EXECUTE_ERR_DUPLICATE	  = -3,   ///< 主键重复
		EXECUTE_ERR_CONN		  = -2,   ///< 连接错误
		EXECUTE_ERR_DB_DISCONNECT = -1,   ///< 断开与服务器的连接
		EXECUTE_OK			  = 0,    ///< 操作成功
	};

	enum DBQueryCode {
		QUERY_ERR_UNKNOWN       = -999, ///< 未知错误
		QUERY_ERR_CONN          = -2,   ///< 连接错误
		QUERY_ERR_DB_DISCONNECT = -1,   ///< 断开与服务器的连接
		QUERY_OK_NO_RECORDSET   = 1,    ///< 操作成功，但没有结果集
		QUERY_OK_HAVE_RECORDSET = 2     ///< 操作成功，有结果集
	};
}

// 数据库服务器登陆信息
struct DBAccount {
	std::string m_dbIp;       // 数据库服务器IP地址
	std::string m_dbName;     // 欲连接的数据库名称

	std::string m_loginName;  // 登陆用户名
	std::string m_loginPwd;   // 登陆密码

	std::string m_charactset; // 数据库编码集
	std::string m_unixSocket; // mysql的unix socket

	uint16 m_dbPort;		  // 数据库监听端口

	DBAccount()
		: m_dbPort(0)
	{
	}
};

#endif //_db_h_