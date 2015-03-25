#include "ConnectionPool.h"

#include<stdexcept>
#include<exception>
#include<stdio.h>

#include<mysql_connection.h>
#include<mysql_driver.h>
#include<cppconn/exception.h>
#include<cppconn/driver.h>
#include<cppconn/resultset.h>
#include<cppconn/prepared_statement.h>
#include<cppconn/statement.h>

#include <base/Logging.h>

using namespace std;

ConnPool* ConnPool::connPool=NULL;

//连接池的构造函数
ConnPool::ConnPool(const char* url, const char* usr, const char* pwd, const char* dbname, int maxSize) :
    m_url(url),
    m_username(usr),
    m_password(pwd),
    m_dbname(dbname),
    m_curSize(0),
    m_maxSize(maxSize)
{
}

void ConnPool::Init()
{
    try{
        m_driver = sql::mysql::get_mysql_driver_instance();
    }
    catch(sql::SQLException &e)
    {
        LOG_ERROR << "驱动连接出错:" << e.what();
    }
    catch(std::runtime_error&)
    {
        LOG_ERROR << "运行出错了:";
    }

    this->InitConnection(m_maxSize);
}

/*
//获取连接池对象，单例模式
ConnPool* ConnPool::GetInstance(){
    if(connPool==NULL)
    {
        connPool = new ConnPool("tcp://127.0.0.1:3306","root","root",50);
    }
    return connPool;
}
}*/

//初始化连接池，创建最大连接数的一半连接数量
void ConnPool::InitConnection(int initialSize)
{
    Connection *conn;

    MutexLockGuard lock(m_mutex);

    for(int i = 0;i < initialSize; i++)
    {
        conn = this->CreateConnection();
        if(conn){
            connList.push_back(conn);
            ++(this->m_curSize);
        }
        else
        {
            perror("创建CONNECTION出错");
        }
    }
}

//创建连接,返回一个Connection
Connection* ConnPool::CreateConnection()
{
    Connection*conn;

    try{
        conn = m_driver->connect(m_url, m_username, m_password);//建立连接
        return conn;
    }
    catch(sql::SQLException &e)
    {
        LOG_ERROR << "创建连接出错:" << e.what();
        return NULL;
    }
    catch(std::runtime_error&)
    {
        perror("运行时出错");
        return NULL;
    }

    return NULL;
}

//在连接池中获得一个连接
Connection* ConnPool::GetConnection()
{
    Connection*con;
    MutexLockGuard lock(m_mutex);

    if(connList.size()>0)//连接池容器中还有连接
    {
        con = connList.front();//得到第一个连接
        connList.pop_front();//移除第一个连接
        
        if(con->isClosed())//如果连接已经被关闭，删除后重新建立一个
        {
            delete con;
            con=this->CreateConnection();
        }

        //如果连接为空，则创建连接出错
        if(con == NULL)
        {
            --m_curSize;
        }

        return con;
    }
    else{
        if(m_curSize< m_maxSize){//还可以创建新的连接
            con= this->CreateConnection();
            if(con){
                ++m_curSize;
                return con;
            }
            else{
                return NULL;
            }
        }
        else{//建立的连接数已经达到maxSize
            return NULL;
        }
    }
}

//回收数据库连接
void ConnPool::ReleaseConnection(sql::Connection * conn){
    if(conn){
        MutexLockGuard lock(m_mutex);
        connList.push_back(conn);
    }
}

//连接池的析构函数
ConnPool::~ConnPool()
{
    this->DestoryConnPool();
}

//销毁连接池,首先要先销毁连接池的中连接
void ConnPool::DestoryConnPool(){
    list<Connection*>::iterator icon;
    MutexLockGuard lock(m_mutex);
    for(icon=connList.begin();icon!=connList.end();++icon)
    {
        this->DestoryConnection(*icon);//销毁连接池中的连接
    }
    m_curSize=0;
    connList.clear();//清空连接池中的连接
}

//销毁一个连接
void ConnPool::DestoryConnection(Connection* conn)
{
    if(conn)
    {
        try{
            conn->close();
        }
        catch(sql::SQLException&e)
        {
            perror(e.what());
        }
        catch(std::exception&e)
        {
            perror(e.what());
        }
        delete conn;
    }
}
