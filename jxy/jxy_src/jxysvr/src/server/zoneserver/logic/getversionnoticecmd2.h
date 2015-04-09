

#ifndef _CGETVERSIONNOTICECMD2_H_
#define _CGETVERSIONNOTICECMD2_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetVersionNoticeCMD2 : public CBaseDBCMD
{
public:
	CGetVersionNoticeCMD2();
	~CGetVersionNoticeCMD2();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GET_VERSION_NOTICE2 ; }

	static BOOL GetVersionNotice(SGDP::ISDDBConnection* poDBConn, UINT32 dwOldVersionID, DT_VERSION_DATA2& stVersionInfo, DT_NOTICE_DATA& stNoticeInfo);
protected:

	
private:
	PKT_CLIZS_GET_VERSION_NOTICE_ACK2 m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

