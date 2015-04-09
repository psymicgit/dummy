

#ifndef _LOGINCMD_H_
#define _LOGINCMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include <protocol/server/protogsdb.h>


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CLoginCMD : public CBaseDBCMD
{
public:
	CLoginCMD();
	~CLoginCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_LOGIN ; }

protected:

	BOOL GetVersionNotice(UINT32 dwCliVersion, DT_VERSION_DATA& stVersionInfo, DT_NOTICE_DATA& stNoticeInfo1, DT_NOTICE_DATA& stNoticeInfo2);	

	BOOL Auth(SGDP::ISDDBConnection* poDBConn,
		UINT8  byAuthType,
		TCHAR* pszDeviceID,
		TCHAR* pszNotifyID,
		TCHAR* pszUserName,
		TCHAR* pszUserPwd,
		UINT16 wCareerID,
		UINT32 dwPlayerID,
		UINT16 wZoneID,
		UINT8 byAuthMethod,
		TCHAR* pszIp,
		UINT32 dwSerialNumber,
		UINT32 dwCliVersion);

private:

	TCHAR				m_aszZoneName[USERNAME_LEN];
	PKT_GTLS_LOGIN_ACK	m_stAck;



};

#endif //#ifndef _LOGINCMD_H_

