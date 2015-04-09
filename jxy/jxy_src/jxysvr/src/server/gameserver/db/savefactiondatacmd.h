

#ifndef _SAVEFACTIONDATACMD_H_
#define _SAVEFACTIONDATACMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playersavemgr.h>
#include <string>
#include <vector>
using namespace std;
using namespace  SGDP;

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSaveFactionDataCMD : public CBaseDBCMD
{
public:
	CSaveFactionDataCMD();
	~CSaveFactionDataCMD();

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
    virtual EDBCmdType GetDBCmdType()
    {
        return EDBCT_FACTION ;
    }

protected:
	///////////////////////////////////////////////////////////////////////////////////////////////
	BOOL	SaveData(ISDDBConnection* poDBConn);//保存
	
    BOOL GetSql(SGDP::ISDDBConnection* poDBConn, vector<string>&vecSql );
	

private:
	DT_FACTION_DATA m_stDT_FACTION_DATA;

};

#endif //#ifndef _GETPLAYERDATACMD_H_

