#ifndef _sqldbcmd_h_
#define _sqldbcmd_h_

#include <db/dbcommand.h>

class SqlDBCmd : public DBCommand
{
public:
	virtual void execute(DBConnection &conn)
	{
		DB::DBExecuteCode code = conn.execute(m_sql.c_str());
	}

	virtual void onExecuted()
	{
	}

	virtual void release()
	{
		delete this;
	}

public:
	std::string m_sql;
};

#endif