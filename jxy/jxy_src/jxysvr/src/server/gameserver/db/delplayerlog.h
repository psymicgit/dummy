///<------------------------------------------------------------------------------
//< @file:   gameserver\db\delplayerlog.h
//< @author: Kang_zhiquan
//< @date:   2014年5月21日 21:7:25
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _delplayerlog_h_
#define _delplayerlog_h_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "dbmgr.h"

class CDelPlayerLogCMD : public CBaseDBCMD
{
public:
	CDelPlayerLogCMD();
	virtual ~CDelPlayerLogCMD();
	/**
	* @brief 异步执行SQL命令
	* @poDBConn 异步执行SQL命令的Connection连接
	* @return VOID
	*/						
	VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection* poDBConn);

	/**
	* @brief 同步执行SQL命令相关的逻辑部分
	* @return VOID
	*/
    VOID SDAPI OnExecuted();

	virtual VOID SetUserData(VOID* pUserData);
public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_DEL_PLAYER_LOG; }
private: 
	UINT64 m_qwKey;
};
#endif //_delplayerlog_h_