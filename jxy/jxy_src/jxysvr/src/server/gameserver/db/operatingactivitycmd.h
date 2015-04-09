///<------------------------------------------------------------------------------
//< @file:   gameserver\db\operatingactivitycmd.h
//< @author: Kang_zhiquan
//< @date:   2014年7月29日 15:3:10
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _operatingactivitycmd_h_
#define _operatingactivitycmd_h_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "dbmgr.h"
#include <string>

struct SOperatingActivity
{
	UINT32 dwStartZoneID;
	UINT32 dwEndZoneID;
	string strStartTime;
	string strEndTime;
	SOperatingActivity()
	{
		dwStartZoneID = 0;
		dwEndZoneID = 0;
		strStartTime = "";
		strEndTime = "";
	}
};

class COperatingActivityCMD : public CBaseDBCMD
{
public:
	COperatingActivityCMD();
	virtual ~COperatingActivityCMD();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_OPERATING_ACTIVITY; }
private: 
	SOperatingActivity m_stActivity;
};

#endif //_operatingactivitycmd_h_