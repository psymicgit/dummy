#ifndef Sqler_h__
#define Sqler_h__

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class SQLer
{
public:
    SQLer() : m_con(NULL) {}

    ~SQLer()
    {
        delete m_con;
    }

    int Connect(const char *addr, const char *user, const char *passwd, const char *dbname);

    int Update(const char *sql);

    sql::ResultSet* Query(const char *sql);

private:
    sql::Connection *m_con;

    char m_addr[64];
    char m_user[64];
    char m_passwd[64];
    char m_dbname[64];
};

#endif // Sqler_h__
