

#ifndef _ACTIVITY_NOTIFYCMD_H_
#define _ACTIVITY_NOTIFYCMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CActivityNotifyCMD : public CBaseDBCMD
{
public:
	CActivityNotifyCMD();
	~CActivityNotifyCMD();

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

public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_ACTIVITY_NOTIFY ; }

protected:


	BOOL ActivityNotify(SGDP::ISDDBConnection* poDBConn,
		UINT32		dwPlayerID,
		UINT8		byActivityType,
		UINT8		byNotify);

private:

};

#endif //#ifndef _LOGINCMD_H_

