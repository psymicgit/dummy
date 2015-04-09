#ifndef _GET_BATTLE_DATA2_H_
#define _GET_BATTLE_DATA2_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include "recordbattledata2.h"

struct SBattleDataParam2
{
    SBattleDataParam2()
    {
        memset(this, 0, sizeof( SBattleDataParam2 ));
    }

    UINT32 dwPlayerID;
    UINT64 qwDataIndex; 
};

class CGetBattleData2: public CBaseDBCMD
{
public:
	CGetBattleData2();
	virtual ~CGetBattleData2();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GETBATTLEDATA2; }

private:
	string m_strSql; //执行的SQL语句的记录
	DT_BATTLE_DATA5 m_battleData;
	SBattleDataParam2 m_oDataParam;
	BOOL m_bHaveBattleData;
};

#endif