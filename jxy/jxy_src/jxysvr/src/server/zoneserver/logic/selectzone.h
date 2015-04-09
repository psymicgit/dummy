

#ifndef _SELECTZONECMD_H_
#define _SELECTZONECMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSelectZoneCMD : public CBaseDBCMD
{
public:
	CSelectZoneCMD();
	~CSelectZoneCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SELECT_ZONE ; }

protected:

	BOOL SelectInfo(SGDP::ISDDBConnection* poDBConn, PKT_CLIGT_SELECT_ZONE_REQ* pstReq);

private:
	PKT_CLIGT_SELECT_ZONE_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

