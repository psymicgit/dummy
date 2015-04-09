#ifndef _RECORD_BATTLE_DATA_H_
#define _RECORD_BATTLE_DATA_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"

//玩家战斗信息
struct SPlayerBattleData
{
	SPlayerBattleData()
	{
		memset(this, 0, sizeof(SPlayerBattleData));
	}
	UINT64 qwIndex; //在数据库中的索引
	DT_BATTLE_DATA2 oBattleData;
	UINT32 dwPlayerMyself; //发起者
	UINT32 dwPlayerEnemy; //敌人
};

class CRecordBattleData: public CBaseDBCMD
{
public:
	CRecordBattleData();
	virtual ~CRecordBattleData();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SAVEBATTLEDATA; }

private:
	//string m_strSql; //执行的SQL语句的记录
	SPlayerBattleData m_stBattleData;  //战斗信息
};

#endif