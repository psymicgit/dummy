///<------------------------------------------------------------------------------
//< @file:   gameserver\db\setreadprivatemsgcmd.h
//< @author: Kang_zhiquan
//< @date:   2014年6月9日 16:26:39
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _setreadprivatemsgcmd_h_
#define _setreadprivatemsgcmd_h_
#include <db/basedbccmd.h>
#include <sddb.h>
#include "dbmgr.h"
#include "logic/base/basedefine.h"
class CSetReadPrivateMsgCMD : public CBaseDBCMD
{
public:
	CSetReadPrivateMsgCMD();
	virtual ~CSetReadPrivateMsgCMD();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SET_READ_PRIVATE_MSG; }
private: 
	C64Set m_setReadID;
};
#endif //_setreadprivatemsgcmd_h_