
#ifndef SGDP_DB_CONNFACTORY_H_20100721
#define SGDP_DB_CONNFACTORY_H_20100721

#include "db_conn.h"

/**
  @desc IConn的简单工厂类，用于创建DBConnection
**/
class CConnFactory
{
private:
	/**
	  @desc CConnFactory的构造函数
	**/
	CConnFactory();

public:
	/**
	  @desc CConnFactory的析构函数
	**/
	~CConnFactory();

	/**
	  @param pstDBAccount 数据库连接信息
	  @return IConn* 返回一个Connection
	  @desc 获取一个Connection
	**/
	static IConn * GetConnection(SGDP::SDDBAccount *pstDBAccount);

	/**
	  @param wDBType 数据库类型
	  @param pszServerAddr 数据库服务器的IP地址
	  @param nServerPort 数据库服务器的端口号
	  @param pszDBName 数据库DB的名称
	  @param pszLoginName 数据库的登陆名
	  @param pszPassword 数据库的密码
	  @param pszCharactSet数据库的字符集
	  @return IConn* 返回一个Connection
	  @desc 获取一个Connection
	**/
	static IConn * GetConnection(INT32 wDBType, const CHAR * pszServerAddr, int nServerPort, const CHAR * pszDBName, 
				const CHAR * pszLoginName, const CHAR * pszPassword, const CHAR * pszCharactSet, const CHAR* pszUnixSocket);

	/**
	  @return 返回一个模拟的Connection
	  @desc 获取一个模拟的Connection
	**/
	static IConn * GetMockConnection();
};

#endif

