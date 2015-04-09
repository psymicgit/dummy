

#ifndef _CGETZONEINFOCMD_H_
#define _CGETZONEINFOCMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetZoneInfoCMD : public CBaseDBCMD
{
public:
	CGetZoneInfoCMD();
	~CGetZoneInfoCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GET_ZONE_INFO ; }

protected:

	BOOL GetInfo(SGDP::ISDDBConnection* poDBConn, UINT16 wID);

private:
	PKT_CLIZS_GET_ZONE_INFO_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

