

#ifndef _RECORD_INFO_H_
#define _RECORD_INFO_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CRecordInfoCMD : public CBaseDBCMD
{
public:
	CRecordInfoCMD();
	~CRecordInfoCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_RECORD_INFO ; }
};

#endif //#ifndef _LOGINCMD_H_

