///<------------------------------------------------------------------------------
//< @file:   gameserver\db\deleteovergvgift.h
//< @author: Kang_zhiquan
//< @date:   2014年5月8日 9:52:16
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _deleteovergvgift_h_
#define _deleteovergvgift_h_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "dbmgr.h"

class CDeleteOverGvGift: public CBaseDBCMD
{
public:
	CDeleteOverGvGift();
	virtual ~CDeleteOverGvGift();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_DELETE_GVG_GIFT; }

private:
	UINT16 m_wSessionID;//过期届数
};
#endif //_deleteovergvgift_h_