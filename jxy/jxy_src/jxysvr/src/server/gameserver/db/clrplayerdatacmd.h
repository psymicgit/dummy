

#ifndef _CLRPLAYERDATACMD_H_
#define _CLRPLAYERDATACMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/instance/instance.h>



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CClrPlayerDataCMD : public CBaseDBCMD
{
public:
	CClrPlayerDataCMD();
	~CClrPlayerDataCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_CLR ; }

protected:

	BOOL ClrData(SGDP::ISDDBConnection* poDBConn);

private:
	UINT32 m_dwPlayerID;
};

#endif //#ifndef _GETPLAYERDATACMD_H_

