/*
*File: connection_pool.h
*Author: csc
*/
#ifndef _CONNECTION_POOL_H
#define	_CONNECTION_POOL_H

#include <base/Mutex.h>

#include<list>
#include<cppconn/connection.h>

#include<mysql_driver.h>

using namespace std;
using namespace sql;

class ConnPool{
public:
    //static ConnPool* GetInstance();//获取数据库连接池对象

    ConnPool(const char* url, const char* usr, const char* pwd, const char* dbname, int maxSize);//构造方法
    ~ConnPool();

    void Init();

    Connection* GetConnection();//获得数据库连接
    void ReleaseConnection(Connection *conn);//将数据库连接放回到连接池的容器中


private:
    

    Connection* CreateConnection();//创建一个连接
    void InitConnection(int iInitialSize);//初始化数据库连接池
    void DestoryConnection(Connection *conn);//销毁数据库连接对象
    void DestoryConnPool();//销毁数据库连接池

private:
    int m_curSize;//当前已建立的数据库连接数量
    int m_maxSize;//连接池中定义的最大数据库连接数
    string m_username;
    string m_password;
    string m_dbname;
    string m_url;

    list<Connection*> connList;//连接池的容器队列

    mutable MutexLock m_mutex; // 线程锁

    static ConnPool* connPool;
    sql::mysql::MySQL_Driver *m_driver;
};

#endif	/*_CONNECTION_POOL_H */