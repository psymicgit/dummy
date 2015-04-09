

#ifndef _GETUSERINFO_H_
#define _GETUSERINFO_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetUserInfoCMD : public CBaseDBCMD
{
public:
	CGetUserInfoCMD();
	~CGetUserInfoCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GET_USERINFO ; }

protected:
	BOOL UserInfo(SGDP::ISDDBConnection* poDBConn, PKT_CLIZS_GET_USERINFO_REQ* pstReq);
private:
	PKT_CLIZS_GET_USERINFO_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

