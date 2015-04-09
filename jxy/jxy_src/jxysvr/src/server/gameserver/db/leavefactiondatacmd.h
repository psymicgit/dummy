#ifndef _LEAVE_FACTION_DATA_CMD_H_
#define _LEAVE_FACTION_DATA_CMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "dbmgr.h"
#include "protocol/server/protogsdb.h"
#include <protocligs.h>


using namespace  SGDP;

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/

class CLeaveFactionDataCMD : public CBaseDBCMD
{
public:
	CLeaveFactionDataCMD();
	~CLeaveFactionDataCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_LEAVE_FACTION ; }

protected:
	///////////////////////////////////////////////////////////////////////////////////////////////
	BOOL	SaveData(ISDDBConnection* poDBConn);//保存
	
	

private:
	UINT16	m_wErrCode;
	PKT_CLIGS_LEAVE_FACTION_REQ m_stReq;

};

#endif //#ifndef _GETPLAYERDATACMD_H_

