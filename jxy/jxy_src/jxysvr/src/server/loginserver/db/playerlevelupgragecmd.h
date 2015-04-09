

#ifndef _PLAYERLEVELUPGRADECMD_H_
#define _PLAYERLEVELUPGRADECMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CPlayUpgrateCMD : public CBaseDBCMD
{
public:
	CPlayUpgrateCMD();
	~CPlayUpgrateCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_LVLUP ; }

protected:


	BOOL UpdateLevel(SGDP::ISDDBConnection* poDBConn,
		UINT32	dwPlayerID,
		UINT16 wPlayerLevel);

private:

};

#endif //#ifndef _LOGINCMD_H_

