///<------------------------------------------------------------------------------
//< @file:   server\db\dbcommand.h
//< @author: 洪坤安
//< @date:   2015年3月27日 11:49:23
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dbcommand_h_
#define _dbcommand_h_

#include "db.h"

// DBCommand接口类，代表异步执行SQL的DB命令，整个过程为：
// 插入命令->OnExecuteSql(异步)->(执行ISDDBSession::Run时)OnExecuted(同步)->Release(同步)
class DBCommand
{
public:
	static thread_local char s_sql[2048];

public:
	/**
	* @brief 该命令的分组ID。
	*/
	virtual int getGroupId() {return 0;};

	// 在数据库线程执行
	virtual void execute(DBConnection &poDBConnection) = 0;

	// 在业务逻辑线程执行
	virtual void onExecuted() = 0;

	// 执行完业务逻辑后，执行此release操作，对用户而言，此函数中的执行是同步的
	virtual void release() = 0;
};

#endif //_dbcommand_h_