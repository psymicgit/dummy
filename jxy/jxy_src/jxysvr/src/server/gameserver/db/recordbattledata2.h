#ifndef _RECORD_BATTLE_DATA2_H_
#define _RECORD_BATTLE_DATA2_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"

//玩家战斗信息
struct SPlayerBattleData2
{
	SPlayerBattleData2()
	{
		memset(this, 0, sizeof(SPlayerBattleData2));
	}
	UINT64 qwIndex; //在数据库中的索引
	DT_BATTLE_DATA5 oBattleData;
	UINT32 dwPlayerMyself; //发起者
	UINT32 dwPlayerEnemy; //敌人
};

class CRecordBattleData2: public CBaseDBCMD
{
public:
	CRecordBattleData2();
	virtual ~CRecordBattleData2();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SAVEBATTLEDATA2; }

private:
	//string m_strSql; //执行的SQL语句的记录
	SPlayerBattleData2 m_stBattleData;  //战斗信息
};

#endif