

#ifndef _SYNCZONENOTICECMD_H_
#define _SYNCZONENOTICECMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"
#include "dbmgr.h"

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSyncZoneNoticeCMD : public CBaseDBCMD
{
public:
	CSyncZoneNoticeCMD();
	~CSyncZoneNoticeCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SYNC_ZONENOTICE ; }

protected:

	BOOL CkZone(SGDP::ISDDBConnection* poDBConn);
	BOOL CkVersionNotice(SGDP::ISDDBConnection* poDBConn);
	BOOL CkVersionNotice2(SGDP::ISDDBConnection* poDBConn);
	BOOL CkResVer(SGDP::ISDDBConnection* poDBConn);

private:
	CZoneInfoVec			m_vecZoneInfo;
	SVersionNoticeInfo	m_stVersionNoticeInfo;
	SVersionNoticeInfo2	m_stVersionNoticeInfo2;
	CVerID2CliVerInfoMap	m_mapVerID2CliVerInfo;
};

#endif //#ifndef _LOGINCMD_H_

