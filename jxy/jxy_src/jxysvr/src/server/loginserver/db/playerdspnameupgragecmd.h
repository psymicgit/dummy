

#ifndef _PLAYERDSPNAMEUPGRADECMD_H_
#define _PLAYERDSPNAMEUPGRADECMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CPlayDspNameUpgradeCMD : public CBaseDBCMD
{
public:
	CPlayDspNameUpgradeCMD();
	~CPlayDspNameUpgradeCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_DSPNAMEUP ; }

protected:


	BOOL UpdateDspName(SGDP::ISDDBConnection* poDBConn,
		UINT32	dwPlayerID,
		TCHAR* pszDspName);

private:

};

#endif //#ifndef _LOGINCMD_H_

