
#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#pragma once

#include <sddb.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include <vector>


using namespace std;
using namespace SGDP;

//DB命令类型
enum EDBCmdType
{
	EDBCT_RECORD_INFO = 1,//选区
};

enum ERI_Type
{
	ERI_ADD = 0,		//增加
	ERI_UPDATE = 1,	//修改
};


class CBaseDBCMD;
class CRecordInfoCMD;

class CDBMgr
{
public:
	CDBMgr();
	~CDBMgr();

	BOOL Init();
	VOID Release();
	BOOL Run();
	BOOL AddCommand(CBaseDBCMD* poDBCommand);
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

	CSDObjectPool<CRecordInfoCMD, CSDMutex>			m_oRecordInfoCMDPool;
};

#endif 

