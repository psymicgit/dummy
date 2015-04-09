///<------------------------------------------------------------------------------
//< @file:   gameserver\db\addgvgift.h
//< @author: Kang_zhiquan
//< @date:   2014年5月8日 10:53:40
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _addgvgift_h_
#define _addgvgift_h_
#include <db/basedbccmd.h>
#include <sddb.h>
#include "dbmgr.h"
#include "logic/gvg/local/gvggiftmgr.h"
class CAddGvGiftCMD : public CBaseDBCMD
{
public:
	CAddGvGiftCMD();
	virtual ~CAddGvGiftCMD();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_ADD_GVG_GIFT; }
private: 
	CVecGiftRewardData m_vecGigtLstData;
};
#endif //_addgvgift_h_