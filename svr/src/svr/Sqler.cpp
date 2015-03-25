#include "Sqler.h"

#include <cppconn/prepared_statement.h>

#include <base/Logging.h>

#include <assert.h>

#define SQL_TRY try {
#define SQL_CATCH } catch (sql::SQLException &e) \
    { \
    LOG_ERROR << "# ERR: SQLException in " << __FILE__; \
    LOG_ERROR << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl; \
    LOG_ERROR << "# ERR: " << e.what(); \
    LOG_ERROR << " (MySQL error code: " << e.getErrorCode(); \
    LOG_ERROR << ", SQLState: " << e.getSQLState() << " )" << endl; \
    }

int SQLer::Connect(const char *addr, const char *user, const char *passwd,
            const char *dbname)
{
    memcpy(m_addr, addr, sizeof(m_addr));
    memcpy(m_user, user, sizeof(m_user));
    memcpy(m_passwd, passwd, sizeof(m_passwd));
    memcpy(m_dbname, dbname, sizeof(m_dbname));

    sql::Driver *driver = get_driver_instance();
    delete m_con;

    SQL_TRY
        m_con = driver->connect(addr, user, passwd);
        m_con->setSchema(dbname);
    SQL_CATCH
        if (m_con)
            return 0;
    return -1;
}

int SQLer::Update(const char *sql)
{
    assert(m_con != NULL);
    sql::PreparedStatement *perp_stmt = NULL;
    int n;
    SQL_TRY
        perp_stmt = m_con->prepareStatement(sql);
            
        m_con->setAutoCommit(0);
        n = perp_stmt->executeUpdate();

        m_con->commit();
    SQL_CATCH
        delete perp_stmt;
    return n;
}

sql::ResultSet* SQLer::Query(const char *sql)
{
    assert(m_con != NULL);
    sql::PreparedStatement *prep_stmt = NULL;
    sql::ResultSet *res = NULL;

    SQL_TRY
        prep_stmt = m_con->prepareStatement(sql);
        res = prep_stmt->executeQuery(sql);
    SQL_CATCH
    
    cout << "ResultSet ptr " << res << endl;
    delete prep_stmt;

    return res;
}