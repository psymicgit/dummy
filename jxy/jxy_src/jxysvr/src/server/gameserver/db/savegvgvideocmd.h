///<------------------------------------------------------------------------------
//< @file:   gameserver\db\savegvgvideocmd.h
//< @author: hongkunan
//< @date:   2014年2月12日 10:12:59
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _savegvgvideocmd_h_
#define _savegvgvideocmd_h_


#include "logic/gvg/gvgvideomgr.h"

#include <db/basedbccmd.h>
#include <sddb.h>

class CSaveGvGVideoCMD : public CBaseDBCMD
{
public:
    // 在数据库线程执行
	VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection*);

    // 在逻辑线程执行
    VOID SDAPI OnExecuted();

    // @impl
	virtual VOID SetUserData(VOID* pUserData);

public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SAVE_GVG_VIDEO; }

private:
    // 把待存储的录像列表拼接成sql插入语句，返回的字符串结果形如：insert into gvgvideo( VideoID, Date, PvPVideo) values ('2014/2/12 00:00:00', '录像的二进制数据'), ('2014/2/12 00:00:00', '录像的二进制数据'), ......
    std::string BuildSql(SGDP::ISDDBConnection*);

    // 把单个录像内容拼接成sql语句，返回的字符串结果形如：('2013/12/10 00:00:00', '录像的二进制数据')
    std::string BuildGvGVideoSql(SGvGVideo &stVideo, SGDP::ISDDBConnection*);

    std::string BuildPKVideoSql(SGvGVideo &stVideo, SGDP::ISDDBConnection*);

private:
    // 待存录像
    SGvGVideo *m_pstVideo;

    VideoID m_videoID;

    BOOL m_bOK;
};

class CClearGvGVideoCMD : public CBaseDBCMD
{
public:
    // 在数据库线程执行
    VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection*);

    // 在逻辑线程执行
    VOID SDAPI OnExecuted(){}

    // @impl
    virtual VOID SetUserData(VOID* pUserData){}

public:
    virtual EDBCmdType GetDBCmdType() { return EDBCT_CLEAR_GVG_VIDEO; }
};

#endif //_savegvgvideocmd_h_