

#ifndef _LOGINCMD_H_
#define _LOGINCMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CRegisterUserNameCMD : public CBaseDBCMD
{
public:
	CRegisterUserNameCMD();
	~CRegisterUserNameCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_REG ; }
	
protected:


	BOOL Register(SGDP::ISDDBConnection* poDBConn,
		TCHAR* pszDeviceID,
		TCHAR* pszUserName);

private:

};

#endif //#ifndef _LOGINCMD_H_

