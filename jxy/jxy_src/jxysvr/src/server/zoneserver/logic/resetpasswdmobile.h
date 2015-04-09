

#ifndef _RESET_PASSWD_MOBILE_CMD_H_
#define _RESET_PASSWD_MOBILE_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CResetPasswdMobileCMD : public CBaseDBCMD
{
public:
    CResetPasswdMobileCMD();
    ~CResetPasswdMobileCMD();

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
    virtual EDBCmdType GetDBCmdType()
    {
        return EDBCT_RESETPASSWDMOBILE ;
    }

protected:
    BOOL CheckMobile(SGDP::ISDDBConnection* poDBConn, PKT_CLIZS_RESET_PASSWD_MOBILE_REQ* pstReq);

private:
	UINT32							m_dwUserID;
	TCHAR							m_aszUserName[USERNAME_LEN]; //用户名
	PKT_CLIZS_RESET_PASSWD_MOBILE_ACK	m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

