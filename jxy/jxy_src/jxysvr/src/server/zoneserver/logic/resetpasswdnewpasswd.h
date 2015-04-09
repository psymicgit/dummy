

#ifndef _RESET_PASSWD_NEWPASSWD_CMD_H_
#define _RESET_PASSWD_NEWPASSWD_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CResetPasswdNewPassCMD : public CBaseDBCMD
{
public:
    CResetPasswdNewPassCMD();
    ~CResetPasswdNewPassCMD();

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
        return EDBCT_RESETPASSWDNEWPASSWD ;
    }

protected:
    BOOL NewPasswd(SGDP::ISDDBConnection* poDBConn, PKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ* pstReq);

private:
	PKT_CLIZS_RESET_PASSWD_NEWPASSWD_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

