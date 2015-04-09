#ifndef _GET_BATTLE_DATA_H_
#define _GET_BATTLE_DATA_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include "recordbattledata.h"

struct SBattleDataParam
{
	SBattleDataParam()
	{
		memset(this, 0, sizeof( SBattleDataParam ));
	}
	
	UINT32 dwPlayerID;
	UINT64 qwDataIndex; 
};

class CGetBattleData: public CBaseDBCMD
{
public:
	CGetBattleData();
	virtual ~CGetBattleData();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GETBATTLEDATA; }

private:
	string m_strSql; //执行的SQL语句的记录
	DT_BATTLE_DATA5 m_battleData;
	SBattleDataParam m_oDataParam;
	BOOL m_bHaveBattleData;
};

#endif