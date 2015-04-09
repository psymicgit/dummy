///<------------------------------------------------------------------------------
//< @file:   gameserver\db\savegvgbuildcmd.h
//< @author: kang_zhiquan
//< @date:   2014年2月28日 16:21:00
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _savegvgbuildcmd_h_
#define _savegvgbuildcmd_h_

#include <db/basedbccmd.h>
#include <db/dbmgr.h>
#include <sddb.h>

#include "logic/gvg/global/gvgmgr.h"

class CSaveGvGBuildCMD : public CBaseDBCMD
{
public:
	CSaveGvGBuildCMD()
	{
	}

public:
	// 在数据库线程执行
	VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection*);

	// 在逻辑线程执行
	VOID SDAPI OnExecuted();

	// @impl
	virtual VOID SetUserData(VOID* pUserData);

public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SAVE_GVG_BUILD; }


private:
	BOOL SaveData(SGDP::ISDDBConnection *poDBConn);

	DT_GVG_BUILD_DATA m_stGvGBuildData;
};

#endif //_savegvgcmd_h_