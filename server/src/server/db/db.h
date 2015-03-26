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

// 数据库信息名称的最大长度
#define SDDB_MAX_NAME_SIZE		        64

// 数据库字符集名称的最大长度
#define SDDB_MAX_CHARACTSET_SIZE		32

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

// DBCommand接口类，代表异步执行SQL的DB命令，整个过程为：
// 插入命令->OnExecuteSql(异步)->(执行ISDDBSession::Run时)OnExecuted(同步)->Release(同步)
class IDBCommand
{
public:
	/**
	* @brief 该命令的分组ID。
	* 需要注意的是，异步执行的SQL命令，如果在同一个Group中，我们保证按插入命令时的时间逐个执行SQL命令
	* 如果返回为－1，我们不保证按插入命令时的时间逐个执行SQL命令
	* 使用QuickAddDBCommand，我们同样不保证执行SQL命令的时间顺序
	*/
	virtual int GetGroupId() {return 0;};

	/**
	* @brief 执行SQL命令
	*
	* @param poDBConn : 执行命令所用的连接
	* 注意：在连接池中有多个连接的情况下，此函数“不是”线程安全的。
	*/
	virtual void OnExecuteSql(DBConnection &poDBConnection) = 0;

	/**
	* @brief 执行完毕SQL命令后，在用户线程中执行ISDDBSession::Run时，
	* 执行此函数中的业务逻辑，对用户而言，此函数中的执行是同步的。
	* 注意：对于在一个Session中执行的命令，此函数在此Session::Run()中顺序运行，
	* 也就是说，如果全局只有一个地方执行Session::Run()，则，此函数是线程安全的。
	*/
	virtual void OnExecuted() = 0;

	// 执行完业务逻辑后，执行此Release操作，对用户而言，此函数中的执行是同步的
	virtual void Release(void) = 0;
};


#endif //_db_h_