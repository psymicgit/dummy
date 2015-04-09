

#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#pragma once

#include <sddb.h>
#include <map>



using namespace std;
using namespace SGDP;

typedef map<string, ISDDBSession*>	CDBSessionMap;
typedef CDBSessionMap::iterator		CDBSessionMapItr;
class CDBMgr
{
public:
    static CDBMgr* Instance()
    {
        static CDBMgr oDBMgr ;
        return &oDBMgr;
    }
    BOOL Init();
    VOID UnInit();
    BOOL Run();
    BOOL AddCommand(string strKey, SGDP::ISDDBCommand* poDBCommand);
    VOID AddDBCommandCount();
    VOID DecreaseDBCommandCount();
    UINT32 GetDBCommandCount();
    ISDDBSession* GetDBSession(string strKey, SDDBAccount stAccount);

	 BOOL CloseDBSession(string strKey);
protected:
    CDBMgr();
    ~CDBMgr();
private:
    ISDDBModule*		m_pDBModule;
    CDBSessionMap		m_mapDBSession;
    INT32			m_dwCommandNum;
};
#endif

