
#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#pragma once

#include <sddb.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>

using namespace SGDP;

//DB√¸¡Ó¿‡–Õ
enum EDBCmdType
{
	EDBCT_SAVE_FEED_BACK  = 1,
	EDBCT_GET_PLAYER_NOTIFY_INFO = 2,
	EDBCT_TWO_DAYS_NOT_ONLINE_NOTICE_INFO = 3,
};


class CBaseDBCMD;
class CSaveFeedBackCMD;
class CGetPlayerNotifyInfoCMD;
class CTwoDaysNotOnlineNoticeInfoCMD;


class CDBMgr
{
public:
	CDBMgr();
	~CDBMgr();

	BOOL Init();
	VOID Release();
	BOOL Run();
	BOOL AddCommand(SGDP::ISDDBCommand* poDBCommand);
	UINT32 GetDBCommandCount();
	SGDP::ISDDBSession* GetDBSession();
	/////////////////
	CBaseDBCMD*	CreateCmd(EDBCmdType eDbCmdType);
	VOID		FreeCmd(CBaseDBCMD* poBaseDBCMD);

private:
	CDBMgr(const CDBMgr& rhs);
	CDBMgr& operator = (const CDBMgr& rhs);

private:
	SGDP::ISDDBModule*  m_pDBModule;
	SGDP::ISDDBSession* m_pDBSession;	
	CSDObjectPool<CSaveFeedBackCMD, CSDMutex>	m_oSaveFeedBackCMDPool;
	CSDObjectPool<CGetPlayerNotifyInfoCMD, CSDMutex>	m_oGetPlayerNotifyInfoCMDPool;
	CSDObjectPool<CTwoDaysNotOnlineNoticeInfoCMD, CSDMutex>	m_oTwoDaysNotOnlineNoticeInfoCMDPool;
};

#endif 

