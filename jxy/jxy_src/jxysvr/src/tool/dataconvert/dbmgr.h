

#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#pragma once

#include <sddb.h>


using namespace SGDP;

class CDBMgr
{
public:
	static CDBMgr* Instance() { static CDBMgr oDBMgr ;  return &oDBMgr;}
	BOOL Init(SDDBAccount stAccount);
	VOID UnInit();
	BOOL Run();
	BOOL AddCommand(SGDP::ISDDBCommand* poDBCommand);
	VOID AddDBCommandCount();
	VOID DecreaseDBCommandCount();
	UINT32 GetDBCommandCount();
	ISDDBSession* GetDBSession();
protected:
	CDBMgr();
	~CDBMgr();
private:
	ISDDBModule*	m_pDBModule;
	ISDDBSession*	m_pDBSession;
	INT32			m_dwCommandNum;
};
#endif 

