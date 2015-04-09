
#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <sddb.h>
#include <list>
#include <string>
#include <map>
#include <set>
#include "sdframework.h"
#include "sdxmlfile.h"
using namespace std;


class CDBConfig
{
public:

	CDBConfig();
	~CDBConfig();

    virtual VOID        UnInit();
    virtual BOOL        Init(const TCHAR* pszFilename);

	BOOL				GetDBAccount(SGDP::SDDBAccount& DBAccount);

	UINT32				GetPoolCoreSize();
	UINT32				GetPoolMaxSize();

	UINT32				GetIntervalSaveTime();

private:
	CDBConfig(const CDBConfig& rhs);
	CDBConfig& operator = (const CDBConfig& rhs);

private:
	
	UINT32							 m_dwSaveDBInterval;
    UINT32                           m_dwPoolCoreSize;
    UINT32                           m_dwPoolMaxSize;
    SGDP::SDDBAccount                m_oDBAccount;
};

#endif 

