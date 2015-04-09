

#ifndef _GET_GS_NOTICE_CMD_H_
#define _GET_GS_NOTICE_CMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include <protocommondata.h>

using namespace std;



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetGsNoticeCMD : public CBaseDBCMD
{
public:
	CGetGsNoticeCMD();
	~CGetGsNoticeCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GETGSNOTICE ; }

	virtual VOID SetUserData(VOID* pUserData);
protected:


private:
	DT_NOTICE_DATA_DB		m_stGsNoticeInfo; //公告信息
	UINT16				m_wZoneID;
};

#endif //#ifndef _GETPLAYERDATACMD_H_

