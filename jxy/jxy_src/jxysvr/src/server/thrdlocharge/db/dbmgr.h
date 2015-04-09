

#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#pragma once

#include <sddb.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include <sdsingleton.h>
#include <map>
#include "addorderidcmd.h"


using namespace std;
using namespace SGDP;


class CDBMgr
{
public:
	 DECLARE_SINGLETON(CDBMgr);

    BOOL Init(const CHAR* pszConfig);
    VOID Release();
    BOOL Run();
	BOOL AddAddOrderIDCmd(SAddOrderInfo& stAddOrderInfo, UINT16 wTransID);
	VOID FreeAddOrderIDCmd(CAddOrderIDCMD* poCMD);
    SGDP::ISDDBSession* GetDBSession();
   
protected:
	CDBMgr();
	~CDBMgr();


	BOOL LoadDBCfg(const CHAR* pszConfig);

private:
    CDBMgr(const CDBMgr& rhs);
    CDBMgr& operator = (const CDBMgr& rhs);

private:
    SGDP::ISDDBModule*			m_pDBModule;
    SGDP::ISDDBSession*			m_pDBSession;
    CSDObjectPool<CAddOrderIDCMD, CSDMutex>	m_oAddOrderIDCMDPool;
};

#endif

