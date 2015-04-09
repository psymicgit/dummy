

#ifndef _GETPLAYERDATACMD2_H_
#define _GETPLAYERDATACMD2_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetPlayerDataCMD2 : public CBaseDBCMD
{
public:
	CGetPlayerDataCMD2();
	~CGetPlayerDataCMD2();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GET2 ; }

protected:

	/**
	* @brief 获取用户信息
	* @param poDBConn 数据库连接
	* @param pUserType 用户类型
	* @param pUserName 用户名
	* @param pstData 输出参数,获取的用户数据
	* @return 操作是否异常，TRUE:操作不异常，FALSE：操作异常，异常需清理内存
	*/
	BOOL GetData(SGDP::ISDDBConnection* poDBConn);

private:
	SGetPlayerData2* m_pstGetPlayerData;	
};

#endif //#ifndef _GETPLAYERDATACMD2_H_

