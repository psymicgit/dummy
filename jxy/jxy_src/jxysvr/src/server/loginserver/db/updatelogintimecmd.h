#ifndef		UpdateLoginTimeCMD_H
#define 	UpdateLoginTimeCMD_H

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/

class CUpdateLoginTimeCMD:public CBaseDBCMD
{
public:
	CUpdateLoginTimeCMD();
	~CUpdateLoginTimeCMD();

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

	VOID SetParam(UINT32 dwUserID, UINT16 wZoneID) {  m_dwUserID = dwUserID; m_wZoneID =  m_wZoneID; }
protected:


	BOOL UpdateLoginTime(SGDP::ISDDBConnection* poDBConn);//更新玩家最近登录的区信息

private:
	UINT32 m_dwUserID;
	UINT16 m_wZoneID;
};

#endif //#ifndef _LOGINCMD_H_

